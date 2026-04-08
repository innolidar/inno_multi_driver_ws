#pragma once
#include <string>
#include <inno_driver/common/inno_log.hpp>

enum class LidarType:int
{
    None=0x00,
    AOD192=0x01,
    IFN56=0x10,
    FW192SA=0x20,
    FW192SB=0x21,
    FW192A=0x30,
    IFW192S=0x40,
};

enum class InputType:int
{
    ONLINE_LIDAR=0,
    PCAP_FILE=1,
};

inline size_t GetLidarList(const LidarType** types)
{
  const static LidarType lidar_types[] = 
  {
    LidarType::AOD192,
    LidarType::IFN56,
    LidarType::FW192SA,
    LidarType::FW192SB,
    LidarType::FW192A,
    LidarType::IFW192S
  };

  if (types) 
  {
    *types = lidar_types;
  }
  return (sizeof(lidar_types)/sizeof(lidar_types[0]));
}

inline size_t GetLaserNum(LidarType type)
{
  switch (type)
  {
    case LidarType::AOD192:
      return 256;
    case LidarType::IFN56:
      return 56;
    case LidarType::FW192SA:
      return 192;
    case LidarType::FW192SB:
      return 192;
    case LidarType::FW192A:
      return 192;
    case LidarType::IFW192S:
      return 192;
    default:
      return 1;
  }
  return 0;
}

inline std::string LidarTypeToStr(const LidarType& type)
{
  std::string str = "";
  switch (type)
  {
    case LidarType::AOD192:
      str = "AOD192";
      break;
    case LidarType::IFN56:
      str= "IFN56";
      break;
    case LidarType::FW192SA:
      str= "FW192SA";
      break;
    case LidarType::FW192SB:
      str= "FW192SB";
      break;
    case LidarType::FW192A:
      str= "FW192A";
      break;
    case LidarType::IFW192S:
      str= "IFW192S";
      break;
    default:
      str = "ERROR";
      INNO_ERROR << "INNO_ERROR" << INNO_REND;
  }
  return str;
}

inline std::string InputTypeToStr(const InputType& type)
{
  std::string str = "";
  switch (type)
  {
    case InputType::ONLINE_LIDAR:
      str = "ONLINE_LIDAR";
      break;
    case InputType::PCAP_FILE:
      str = "PCAP_FILE";
      break;
    default:
      str = "ERROR";
      INNO_ERROR << "INNO_ERROR" << INNO_REND;
  }
  return str;
}

inline LidarType StrToLidarType(const std::string& type)
{
  if (type == "AOD192")
  {
    return LidarType::AOD192;
  }
  else if (type == "IFN56")
  {
    return LidarType::IFN56;
  }
  else if (type == "FW192SA")
  {
    return LidarType::FW192SA;
  }
  else if (type == "FW192SB")
  {
    return LidarType::FW192SB;
  }
  else if (type == "FW192A")
  {
    return LidarType::FW192A;
  }
  else if (type == "IFW192S")
  {
    return LidarType::IFW192S;
  }
  else
  {
    INNO_ERROR << "Wrong lidar type: " << type << INNO_REND;
    INNO_ERROR << "Please give correct type: AOD192,IFN56,FW192S,FW192A,IFW192S" << INNO_REND;
    exit(-1);
  }
}
struct CalibrationParam
{
  void Print()
  {
    INNO_INFO << "------------------------------------------------------" << INNO_REND;
    INNO_INFO << "             InnoLight Calibration Parameters " << INNO_REND;
    INNO_INFOL << "local_port: " << local_port << INNO_REND;
    INNO_INFOL << "remote_port: " << remote_port << INNO_REND;
    INNO_INFOL << "remote_ip: " << remote_ip << INNO_REND;
    INNO_INFOL << "calibration_folders: " << calibration_folders << INNO_REND;
    INNO_INFO << "------------------------------------------------------" << INNO_REND;
  }
  int               local_port{0};
  std::string       local_ip{0};
  int               remote_port{0};
  std::string       remote_ip{0};
  std::string       calibration_folders{""};
};
struct TransformParam  ///< The Point transform parameter
{
  TransformParam()
   : valid(false), x(0.0f), y(0.0f), z(0.0f), roll(0.0f), pitch(0.0f), yaw(0.0f)
  {
  }

  bool valid;
  float x;      ///< unit, m
  float y;      ///< unit, m
  float z;      ///< unit, m
  float roll;   ///< unit, radian
  float pitch;  ///< unit, radian
  float yaw;    ///< unit, radian

  void Print() const
  {
    INNO_INFO  << "------------------------------------------------------" << INNO_REND;
    INNO_INFO  << "             InnoLight Transform Parameters " << INNO_REND;
    INNO_INFOL << "valid: " << valid << INNO_REND;
    INNO_INFOL << "x: "     << x << INNO_REND;
    INNO_INFOL << "y: "     << y << INNO_REND;
    INNO_INFOL << "z: "     << z << INNO_REND;
    INNO_INFOL << "roll: "  << roll << INNO_REND;
    INNO_INFOL << "pitch: " << pitch << INNO_REND;
    INNO_INFOL << "yaw: "   << yaw << INNO_REND;
    INNO_INFO  << "------------------------------------------------------" << INNO_REND;
  }
};

typedef TransformParam Transform;  ///< Alias for TransformParam
struct InputParam
{
  InputParam()
  {

  }
  ~InputParam()
  {

  }
  void Print()
  {
    INNO_INFO  << "------------------------------------------------------" << INNO_REND;
    INNO_INFO  << "             InnoLight Input Parameters " << INNO_REND;
    INNO_INFOL << "local_ip: " << local_ip << INNO_REND;
    INNO_INFOL << "local_port: " << local_port << INNO_REND;
    INNO_INFOL << "pcap_file: " << pcap_file << INNO_REND;
    INNO_INFO  << "------------------------------------------------------" << INNO_REND;
  }

  std::string local_ip{""};
  uint16_t    local_port{0};
  std::string pcap_file{""};
};

struct DecoderParam
{
  DecoderParam()
  {

  }
  ~DecoderParam()
  {

  }
  void Print()
  {
      transform_param.Print();
  }
public:
  float           min_distance;            ///< Minimum distance of point cloud range
  float           max_distance;            ///< Max distance of point cloud range
  float           h_start_angle;             ///< Start angle of point cloud
  float           h_end_angle;               ///< End angle of point cloud
  float           v_start_angle;             ///< Start angle of point cloud
  float           v_end_angle;               ///< End angle of point cloud
  TransformParam  transform_param;
};

struct DriverParam
{
  void Print()
  {
    INNO_INFO  << "------------------------------------------------------" << INNO_REND;
    INNO_INFO  << "             InnoLight Lidar Driver Parameters " << INNO_REND;
    INNO_INFOL << "is_device_load: " << is_device_load << INNO_REND;
    INNO_INFOL << "InputType: " << InputTypeToStr(input_type) << INNO_REND;
    INNO_INFOL << "LidarType: " << LidarTypeToStr(lidar_type) << INNO_REND;
    INNO_INFO  << "------------------------------------------------------" << INNO_REND;
    input_param.Print();
    decoder_param.Print();
    calibration_param.Print();
  }
  bool              is_device_load{false};
  InputType         input_type{InputType::ONLINE_LIDAR};
  LidarType         lidar_type{LidarType::None};
  InputParam        input_param{};
  DecoderParam      decoder_param{};
  CalibrationParam  calibration_param{};
};

struct MultiDriverParam
{
    MultiDriverParam(const size_t &num_lidars_in)
        :num_lidars(num_lidars_in)
    {
        driver_params = new DriverParam[num_lidars];
    }
    ~MultiDriverParam()
    {
        if (driver_params != nullptr)
        {
            delete[] driver_params;
            driver_params = nullptr;
        }
    }
    void Print()
    {
        for(size_t i=0;i<num_lidars;i++)
        {
            driver_params[i].Print();
        }
    }
    bool              is_device_load{false};
    size_t            num_lidars{0};    
    DriverParam*      driver_params{nullptr};
};

struct MultiTransformParam
{
    MultiTransformParam(const size_t &num_lidars_in)
        :num_lidars(num_lidars_in)
    {
        transform_params = new TransformParam[num_lidars];
    }
    ~MultiTransformParam()
    {
        if (transform_params != nullptr)
        {
            delete[] transform_params;
            transform_params = nullptr;
        }
    }
    void Print()
    {
        for(size_t i=0;i<num_lidars;i++)
        {
            transform_params[i].Print();
        }
    }
    size_t            num_lidars{0};    
    TransformParam*   transform_params{nullptr};
};
