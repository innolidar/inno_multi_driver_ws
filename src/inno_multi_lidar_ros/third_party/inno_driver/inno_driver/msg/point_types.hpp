#pragma once

// PointT 
struct Point
{
  // basic 
  float x;
  float y;
  float z;
  uint16_t intensity;
  uint16_t ring;
};

struct PointTime:public Point
{
  // time 
  double  timestamp;
};

struct PointSource:public PointTime
{
  // optional
  int32_t distance;
  int32_t horizontal;
  int32_t vertical;
  int8_t  speed;
};

template <typename PointT>
class PointCloud
{
public:
    std::string frame_id{""};
    typedef std::vector<PointT> VectorT;
    uint32_t    height{0};          ///< Height of point cloud
    uint32_t    width{0};           ///< Width of point cloud
    bool        is_dense{false};    ///< If is_dense is true, the point cloud does not contain NAN points,
    double      timestamp{0.0};     ///< cloud timestamp
    uint32_t    seq{0};             ///< Sequence number of message
    uint16_t    device_number;
    double      trx_temperature{0.f};
    double      main_temperature{0.f};
    uint8_t     abnormal_flag{0}; 
    std::vector<PointT> points{}; 
public:
    virtual void PushPoint(const PointT &point)
    {
      points.push_back(point);
    }
};

typedef PointCloud<Point>       BasePointCloud;
typedef PointCloud<PointTime>   TimePointCloud;
typedef PointCloud<PointSource> SourcePointCloud;