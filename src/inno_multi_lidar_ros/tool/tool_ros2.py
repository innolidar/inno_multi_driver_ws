import rclpy
from rclpy.node import Node
from sensor_msgs.msg import PointCloud2
#python3 tool.py _topic:=/innolidar_points
class PointCloudCounter(Node):
    def __init__(self):
        super().__init__('pointcloud_counter')

        # 参数（ROS2写法）
        self.declare_parameter('topic', '/points_raw')
        topic_name = self.get_parameter('topic').get_parameter_value().string_value

        # 订阅
        self.subscription = self.create_subscription(
            PointCloud2,
            topic_name,
            self.callback,
            10
        )

        self.get_logger().info(f"PointCloud counter started, topic: {topic_name}")

    def callback(self, msg):
        # 方法1：直接计算（推荐）
        point_count = msg.width * msg.height

        self.get_logger().info(f"Point count: {point_count}")


def main(args=None):
    rclpy.init(args=args)

    node = PointCloudCounter()
    rclpy.spin(node)

    node.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()