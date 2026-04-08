#pragma once
#include "inno_driver.h"
#if defined(_WIN32)
  #define LIDAR_API __declspec(dllexport)
#else
  #define LIDAR_API __attribute__((visibility("default")))
#endif
template<typename PointT>
LIDAR_API InnoDriver<PointT>* CreatorDriver();