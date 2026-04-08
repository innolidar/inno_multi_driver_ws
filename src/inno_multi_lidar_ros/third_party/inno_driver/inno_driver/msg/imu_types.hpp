#pragma once

struct ImuMsg {
    bool state;
    double timestamp;  // Time in nanoseconds
    float orientation_x;
    float orientation_y;
    float orientation_z;
    float orientation_w;
    float angular_velocity_x;
    float angular_velocity_y;
    float angular_velocity_z;
    float linear_acceleration_x;
    float linear_acceleration_y;
    float linear_acceleration_z;
    ImuMsg()
        : state{false},
          timestamp(0),
          orientation_x(0.0),
          orientation_y(0.0),
          orientation_z(0.0),
          orientation_w(1.0),
          angular_velocity_x(0.0),
          angular_velocity_y(0.0),
          angular_velocity_z(0.0),
          linear_acceleration_x(0.0),
          linear_acceleration_y(0.0),
          linear_acceleration_z(0.0) {}

    // Parameterized constructor to initialize all members
    ImuMsg(bool valid, double ts, float ori_x, float ori_y, float ori_z, float ori_w,
            float ang_vel_x, float ang_vel_y, float ang_vel_z,
            float lin_acc_x, float lin_acc_y, float lin_acc_z)
        : state{valid},
          timestamp(ts),
          orientation_x(ori_x),
          orientation_y(ori_y),
          orientation_z(ori_z),
          orientation_w(ori_w),
          angular_velocity_x(ang_vel_x),
          angular_velocity_y(ang_vel_y),
          angular_velocity_z(ang_vel_z),
          linear_acceleration_x(lin_acc_x),
          linear_acceleration_y(lin_acc_y),
          linear_acceleration_z(lin_acc_z) 
          {

          }
    ImuMsg& operator=(const ImuMsg& other) 
    {
        if (this != &other) {
            state = other.state;
            timestamp = other.timestamp;
            orientation_x = other.orientation_x;
            orientation_y = other.orientation_y;
            orientation_z = other.orientation_z;
            orientation_w = other.orientation_w;
            angular_velocity_x = other.angular_velocity_x;
            angular_velocity_y = other.angular_velocity_y;
            angular_velocity_z = other.angular_velocity_z;
            linear_acceleration_x = other.linear_acceleration_x;
            linear_acceleration_y = other.linear_acceleration_y;
            linear_acceleration_z = other.linear_acceleration_z;
        }
        return *this;
    }
    void init()
    {
      state = false;
      timestamp = 0.0;
      orientation_x = 0.0;
      orientation_y = 0.0;
      orientation_z = 0.0;
      orientation_w = 1.0;
      angular_velocity_x = 0.0;
      angular_velocity_y = 0.0;
      angular_velocity_z = 0.0;
      linear_acceleration_x = 0.0;
      linear_acceleration_y = 0.0;
      linear_acceleration_z = 0.0;
    }
};