import sys
import os
import time
import queue
import numpy as np

from PyQt5.QtWidgets import (
    QApplication, QWidget, QVBoxLayout, QHBoxLayout,
    QPushButton, QTextEdit, QLabel, QComboBox ,QLineEdit
)
from PyQt5.QtCore import QThread, pyqtSignal

#import open3d as o3d

SAVE_QUEUE_SIZE = 100


# ================= 保存线程 =================
class SaveWorker(QThread):
    log_signal = pyqtSignal(str)

    def __init__(self, save_dir, q):
        super().__init__()
        self.save_dir = save_dir
        self.q = q
        self.running = True
        self.count = 0
    def save_pcd_xyzi(self, filename, points):
        """
        points: Nx4 (x, y, z, intensity)
        """
        with open(filename, 'w') as f:
            f.write("# .PCD v0.7 - Point Cloud Data file\n")
            f.write("VERSION 0.7\n")
            f.write("FIELDS x y z intensity\n")
            f.write("SIZE 4 4 4 4\n")
            f.write("TYPE F F F F\n")
            f.write("COUNT 1 1 1 1\n")
            f.write(f"WIDTH {points.shape[0]}\n")
            f.write("HEIGHT 1\n")
            f.write("VIEWPOINT 0 0 0 1 0 0 0\n")
            f.write(f"POINTS {points.shape[0]}\n")
            f.write("DATA ascii\n")
            for p in points:
                f.write(f"{p[0]} {p[1]} {p[2]} {p[3]}\n")
                
    def run(self):
        while self.running:
            try:
                cloud = self.q.get(timeout=1)

                #pcd = o3d.geometry.PointCloud()
                #pcd.points = o3d.utility.Vector3dVector(cloud)

                filename = os.path.join(
                    self.save_dir,
                    f"cloud_{self.count}_{int(time.time()*1000)}.pcd"
                )
                #print(filename)
                #o3d.io.write_point_cloud(filename, pcd, write_ascii=False)
                self.save_pcd_xyzi(filename,cloud)
                self.count += 1

                if self.count % 10 == 0:
                    self.log_signal.emit(f"已保存 {self.count} 帧")

            except queue.Empty:
                continue

    def stop(self):
        self.running = False


# ================= ROS2 =================
class ROS2Worker(QThread):
    log_signal = pyqtSignal(str)

    def __init__(self, topic, q):
        super().__init__()
        self.topic = topic
        self.q = q
        self.running = True

    def run(self):
        import rclpy
        from rclpy.node import Node
        from sensor_msgs.msg import PointCloud2
        import sensor_msgs_py.point_cloud2 as pc2

        rclpy.init()
        node = Node("ros_to_pcd")

        def callback(msg):
            #point_count = msg.width*msg.height
            #print(f"point_count: {point_count} ")
            #print(msg.fields)
            fields = ["x", "y", "z", "intensity"] #, "ring", "timestamp"
            #pts_gen = pc2.read_points(msg, field_names=fields, skip_nans=True)
            pts_list = np.array(list(pc2.read_points(msg, field_names=["x", "y", "z", "intensity"], skip_nans=True)))
            #pts_list = list(pts_gen)
            x = pts_list['x']
            y = pts_list['y']
            z = pts_list['z']
            i = pts_list['intensity']
            
            #pts = np.array(pts_list, dtype=np.float32)
            pts = np.column_stack((x, y, z, i)).astype(np.float32)
            #pts = np.array(list(pc2.read_points(msg,field_names=fields,skip_nans=True)), dtype=np.float32)
            #if pts.shape[0] == 0:
            #    return
            #pts = np.array(list(pts_gen), dtype=np.float32)
            cloud = pts[:, :4]

            if not self.q.full():
                self.q.put(cloud)
            else:
                self.log_signal.emit("⚠️ 队列满，丢帧")

        node.create_subscription(PointCloud2, self.topic, callback, 100)

        while rclpy.ok() and self.running:
            rclpy.spin_once(node, timeout_sec=0.05)

        node.destroy_node()
        rclpy.shutdown()

    def stop(self):
        self.running = False


# ================= ROS1 =================
class ROS1Worker(QThread):
    log_signal = pyqtSignal(str)

    def __init__(self, topic, q):
        super().__init__()
        self.topic = topic
        self.q = q
        import rospy
        from sensor_msgs.msg import PointCloud2
        import sensor_msgs.point_cloud2 as pc2
        rospy.init_node("ros_to_pcd", anonymous=True)

    def run(self):
        import rospy
        from sensor_msgs.msg import PointCloud2
        import sensor_msgs.point_cloud2 as pc2
        def callback(msg):
            #print(msg.fields)
            #point_count = msg.width * msg.height
            #rospy.loginfo("Point count: %d", point_count)
			
            pts = np.array(list(pc2.read_points(msg, skip_nans=True)), dtype=np.float32)
            if pts.shape[0] == 0:
                return

            cloud = pts[:, :4]

            if not self.q.full():
                self.q.put(cloud)

        rospy.Subscriber(self.topic, PointCloud2, callback)
        rospy.spin()


# ================= GUI =================
class MainWindow(QWidget):
    def __init__(self):
        super().__init__()

        self.setWindowTitle("实时点云录制工具（ROS1/ROS2）")
        self.resize(500, 400)

        layout = QVBoxLayout()
		
		
        self.mode_box = QComboBox()
        self.mode_box.addItems(["ROS2", "ROS1"])

        self.label = QLabel("状态：未启动")
		
        self.topic_label = QLabel("Topic:")
		
        self.topic_edit = QLineEdit("/innolidar_points")
		
        h_layout = QHBoxLayout()
		
        h_layout.addWidget(self.topic_label)
		
        h_layout.addWidget(self.topic_edit)
		
        self.start_btn = QPushButton("开始录制")
        self.stop_btn = QPushButton("停止")

        self.log = QTextEdit()

        layout.addWidget(self.mode_box)
        layout.addWidget(self.label)
		
        layout.addLayout(h_layout)
        layout.addWidget(self.start_btn)
        layout.addWidget(self.stop_btn)
        layout.addWidget(self.log)

        self.setLayout(layout)

        self.q = queue.Queue(maxsize=SAVE_QUEUE_SIZE)
        print(os.path.abspath(__file__))
        current_dir = os.path.dirname(os.path.abspath(__file__))
        print(current_dir)
        
        self.save_dir = "pcd_output"
        os.makedirs(self.save_dir, exist_ok=True)
        self.save_dir = os.path.join(current_dir,self.save_dir)
        self.ros_worker = None
        self.save_worker = None

        self.start_btn.clicked.connect(self.start)
        self.stop_btn.clicked.connect(self.stop)

    def start(self):
        mode = self.mode_box.currentText()

        #topic = "/innolidar_points"
        topic = self.topic_edit.text()

        # 保存线程
        self.save_worker = SaveWorker(self.save_dir, self.q)
        self.save_worker.log_signal.connect(self.log.append)
        self.save_worker.start()

        # ROS线程
        if mode == "ROS2":
            self.ros_worker = ROS2Worker(topic, self.q)
        else:
            self.ros_worker = ROS1Worker(topic, self.q)

        self.ros_worker.log_signal.connect(self.log.append)
        self.ros_worker.start()

        self.label.setText(f"录制中：{mode}")

    def stop(self):
        if self.ros_worker:
            try:
                self.ros_worker.stop()
            except:
                pass

        if self.save_worker:
            self.save_worker.stop()

        self.label.setText("已停止")
        self.log.append("录制结束")


# ================= main =================
def main():
    app = QApplication(sys.argv)
    win = MainWindow()
    win.show()
    sys.exit(app.exec_())


if __name__ == "__main__":
    main()
