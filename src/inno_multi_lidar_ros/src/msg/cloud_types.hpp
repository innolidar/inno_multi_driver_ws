#pragma once
#include <inno_driver/msg/data_types.hpp>
#include <inno_driver/msg/point_types.hpp>
#include <inno_driver/msg/imu_types.hpp>
#include <vector>
#ifdef POINT_TYPE_XYZI
typedef BasePointCloud DrawPointCloud;
typedef Point          DrawPoint;
#elif POINT_TYPE_TIME
typedef TimePointCloud DrawPointCloud;
typedef PointTime          DrawPoint;
#else 
typedef SourcePointCloud DrawPointCloud;
typedef PointSource          DrawPoint;
#endif

class RosPointCloud:public DrawPointCloud
{
public:
    RosPointCloud()
    {

    }
    virtual ~RosPointCloud()
    {

    }
    /*virtual void PushPoint(const DrawPoint &point) override
    {
      points.push_back(point);
    }
public:
    std::vector<DrawPoint> points{};*/
};
