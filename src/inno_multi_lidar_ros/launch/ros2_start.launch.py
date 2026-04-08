from launch import LaunchDescription
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory
import os
def generate_launch_description():
    package_name = 'inno_lidar_ros'
    rviz_config = 'rviz2.rviz'

    # 获取功能包路径
    pkg_share = get_package_share_directory(package_name)

    # 设置RViz配置路径
    rviz_config_path = os.path.join(pkg_share, 'rviz', rviz_config)
    print("RViz config path:", rviz_config_path)
    return LaunchDescription([
        Node(
            package='inno_lidar_ros',
            executable='inno_lidar_node',
            name='inno_lidar_node'
        ),
        # 启动RViz2
        Node(
            package='rviz2',
            executable='rviz2',
            name='rviz2',
            arguments=['-d', rviz_config_path],
            output='screen'
        )
    ])
