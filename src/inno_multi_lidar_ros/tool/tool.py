#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#python3 tool.py _topic:=/innolidar_points
import rospy
from sensor_msgs.msg import PointCloud2

class PointCloudCounter:
    def __init__(self):
        rospy.init_node('pointcloud_counter', anonymous=True)

        topic_name = rospy.get_param("~topic", "/points_raw")

        rospy.Subscriber(topic_name, PointCloud2, self.callback, queue_size=1)

        rospy.loginfo("PointCloud counter node started, subscribing to: %s", topic_name)

    def callback(self, msg):
        # 方法1：直接计算（推荐）
        point_count = msg.width * msg.height

        rospy.loginfo("Point count: %d", point_count)


if __name__ == '__main__':
    try:
        PointCloudCounter()
        rospy.spin()
    except rospy.ROSInterruptException:
        pass
