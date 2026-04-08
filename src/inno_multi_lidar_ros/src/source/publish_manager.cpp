#include "publish_manager.h"
#if ROS_FOUND==1
#include <ros/ros.h>
#include <sensor_msgs/point_cloud2_iterator.h>
#include <sensor_msgs/PointCloud.h>
#ifdef ENABLE_IMU_MSG_PARSE
#include <sensor_msgs/Imu.h>
#endif
#else
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/point_cloud2_iterator.hpp>
#include <sstream>
#ifdef ENABLE_IMU_MSG_PARSE
#include <sensor_msgs/msg/imu.hpp>
#endif
#endif
#if ROS_FOUND==1
typedef sensor_msgs::PointCloud2 LidarCloud2;
typedef sensor_msgs::PointField  LidarField;
typedef sensor_msgs::Imu         IMU;
#else
typedef sensor_msgs::msg::PointCloud2 LidarCloud2;
typedef sensor_msgs::msg::PointField  LidarField;
typedef sensor_msgs::msg::Imu         IMU;
#endif
namespace innolight
{
namespace lidar
{
struct PublishManager::Impl
{
public:
    Impl()
    {

    }
    inline LidarCloud2 toRosMsg(const RosPointCloud& inno_msg, const std::string& frame_id, bool send_by_rows)
    {
        LidarCloud2 ros_msg;
        int fields =5;
#ifdef POINT_TYPE_TIME
        fields = 6;
#elif defined(POINT_TYPE_SOURCE)
        fields = 10;
#else
#endif
        ros_msg.fields.clear();
        ros_msg.fields.reserve(fields);
        ros_msg.width = inno_msg.points.size(); 
        ros_msg.height = 1; 
  
        int offset = 0;
        offset = addPointField(ros_msg, "x", 1, LidarField::FLOAT32, offset);
        offset = addPointField(ros_msg, "y", 1, LidarField::FLOAT32, offset);
        offset = addPointField(ros_msg, "z", 1, LidarField::FLOAT32, offset);
        offset = addPointField(ros_msg, "intensity", 1, LidarField::FLOAT32, offset);
        offset = addPointField(ros_msg, "ring", 1, LidarField::UINT16, offset);
#ifdef POINT_TYPE_TIME  
        offset = addPointField(ros_msg, "timestamp", 1, LidarField::FLOAT64, offset);
#elif defined(POINT_TYPE_SOURCE)
        offset = addPointField(ros_msg, "timestamp", 1, LidarField::FLOAT64, offset);
        offset = addPointField(ros_msg, "distance", 1, LidarField::UINT32, offset);
        offset = addPointField(ros_msg, "horizontal", 1, LidarField::UINT32, offset);
        offset = addPointField(ros_msg, "vertical", 1, LidarField::UINT32, offset);
        offset = addPointField(ros_msg, "speed", 1, LidarField::UINT8, offset);
#else
#endif
        ros_msg.point_step = offset;
        ros_msg.row_step = ros_msg.width * ros_msg.point_step;
        ros_msg.is_dense = inno_msg.is_dense;
        ros_msg.data.resize(ros_msg.point_step * ros_msg.width * ros_msg.height);
        sensor_msgs::PointCloud2Iterator<float> iter_x(ros_msg, "x");
        sensor_msgs::PointCloud2Iterator<float> iter_y(ros_msg, "y");
        sensor_msgs::PointCloud2Iterator<float> iter_z(ros_msg, "z");
        sensor_msgs::PointCloud2Iterator<float> iter_intensity(ros_msg, "intensity");
        sensor_msgs::PointCloud2Iterator<uint16_t> iter_ring(ros_msg, "ring");
#ifdef POINT_TYPE_TIME
        sensor_msgs::PointCloud2Iterator<double> iter_timestamp(ros_msg, "timestamp");
#elif defined(POINT_TYPE_SOURCE)
        sensor_msgs::PointCloud2Iterator<double> iter_timestamp(ros_msg, "timestamp");
        sensor_msgs::PointCloud2Iterator<uint32_t> iter_distance(ros_msg, "distance");
        sensor_msgs::PointCloud2Iterator<uint32_t> iter_horizontal(ros_msg, "horizontal");
        sensor_msgs::PointCloud2Iterator<uint32_t> iter_vertical(ros_msg, "vertical");
        sensor_msgs::PointCloud2Iterator<uint8_t> iter_speed(ros_msg, "speed");
#else
#endif
        for (auto &point:inno_msg.points)
        {
            *iter_x = point.x;
            *iter_y = point.y;
            *iter_z = point.z;
            *iter_intensity = point.intensity;
            *iter_ring = point.ring;
            ++iter_x;
            ++iter_y;;
            ++iter_z;
            ++iter_intensity;
            ++iter_ring;
#ifdef POINT_TYPE_TIME
            *iter_timestamp = point.timestamp;
            ++iter_timestamp;
#elif defined(POINT_TYPE_SOURCE)
            *iter_timestamp = point.timestamp;ros_msg
            *iter_distance = point.distance;
            *iter_horizontal = point.horizontal;
            *iter_vertical = point.vertical;
            *iter_speed = point.speed;
            ++iter_timestamp;
            ++iter_distance;
            ++iter_horizontal;
            ++iter_vertical;
            ++iter_speed;
#endif
        }
        ros_msg.header.frame_id = frame_id;
        //ros_msg.header.stamp=ros::Time()::now();
        //ros_msg.header.stamp=std::chrono::system_clock::now();
        //ros_msg.header.stamp = ros_msg.header.stamp.fromSec(inno_msg.timestamp);

        ros_msg.header.stamp.sec = (uint32_t)floor(inno_msg.timestamp);
        //std::cout<<"inno_msg.timestamp:"<<inno_msg.timestamp<<std::endl;
#if ROS_FOUND==1
        ros_msg.header.seq = inno_msg.seq;
        ros_msg.header.stamp.nsec = (uint32_t)round((inno_msg.timestamp - ros_msg.header.stamp.sec) * 1e9);
#else
        //ros_msg.header.stamp.nanosec = (uint32_t)round((inno_msg.timestamp - ros_msg.header.stamp.sec) * 1e9);
        ros_msg.header.stamp.nanosec =(uint32_t)round((inno_msg.timestamp - ros_msg.header.stamp.sec) * 1e9);
#endif
        ros_msg.header.frame_id = frame_id;
        return ros_msg;
    }
    void Init(const YAML::Node& config)
    {
        yamlRead<std::string>(config,"ros_frame_id", m_frame_id, "innolidar");
        std::string ros_send_topic;
        yamlRead<std::string>(config,"ros_send_point_cloud_topic", ros_send_topic, "innolidar_points");
        static int node_index = 0;
        std::stringstream node_name;
        node_name << "inno_points_destination_" << node_index++;
#if ROS_FOUND==1
        m_nh = std::unique_ptr<ros::NodeHandle>(new ros::NodeHandle());
        m_pub = m_nh->advertise<sensor_msgs::PointCloud2>(ros_send_topic, 100);
#else
        m_nh=std::make_shared<rclcpp::Node>(node_name.str());
        m_pub = m_nh->create_publisher<sensor_msgs::msg::PointCloud2>(ros_send_topic, 100);
#endif
    }
    void SendPointCloud(const RosPointCloud& msg)
    {
#if ROS_FOUND==1
        m_pub.publish(toRosMsg(msg, m_frame_id, m_send_by_rows)); 
#else
        m_pub->publish(toRosMsg(msg, m_frame_id, m_send_by_rows));
#endif
    }
private:
#if ROS_FOUND==1
    std::shared_ptr<ros::NodeHandle> m_nh;
    ros::Publisher  m_pub;
#else
    std::shared_ptr<rclcpp::Node>    m_nh;
    rclcpp::Publisher<sensor_msgs::msg::PointCloud2>::SharedPtr m_pub;
#endif
    std::string m_frame_id{""};
    bool m_send_by_rows{false};
};

PublishManager::PublishManager()
:m_impl(std::make_shared<PublishManager::Impl>())
{

}

PublishManager::~PublishManager()
{
    m_impl.reset();
    m_impl=nullptr;
}
void PublishManager::Init(const YAML::Node& config)
{
    m_impl->Init(config);
}
void PublishManager::SendPointCloud(const RosPointCloud& msg)
{
    m_impl->SendPointCloud(msg);
}
}
}
