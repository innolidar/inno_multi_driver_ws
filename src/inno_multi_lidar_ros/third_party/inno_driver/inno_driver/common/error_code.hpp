#pragma once
#include <string>
#include <ctime>
//namespace innolight
//{
//namespace lidar
//{
enum ErrCodeType
{
  INFO_CODE,     // 0x00 ~ 0x3F
  WARNING_CODE,  // 0x40 ~ 0x7F
  ERROR_CODE     // 0x80 ~ 0xBF
};

enum ErrCode
{
  // info
  INFO_SUCCESS         = 0x00,  ///< Normal Status
  INFO_PCAPREPEAT      = 0x01,  ///< Reach file end, and play PCAP file again.
  INFO_PCAPEXIT        = 0x02,  ///< Reach file end, and exit parsing PCAP file

  // warning
  WARN_MSOPTIMEOUT     = 0x40,  ///< Timeout (1s) of receiving MSOP Packets
  WARN_MSOPLEN         = 0x41,  ///< MSOP Packet length is wrong
  WARN_MSOPID          = 0x42,  ///< MSOP Packet ID is wrong
  WARN_ZEROPOINTS      = 0x43,  ///< No points in PointCloud
  WARN_PKTBUFOVERFLOW  = 0x44,  ///< Packet queue is overflow
  WARN_CLOUDOVERFLOW   = 0x45,  ///< Point cloud buffer is overflow
  WARN_CRC32           = 0x46,  ///< Wrong CRC32 value of MSOP Packet
  WARN_PCAPPARSE       = 0x47,   ///< Parse msop data frome pcap file failed
  
  // error
  ERROR_STARTBEFOREINIT = 0x80,  ///< User calls start() before init()
  ERROR_PCAPWRONGPATH   = 0x81,  ///< Path of pcap file is wrong
  ERROR_POINTCLOUDNULL  = 0x82,   ///< User provided PointCloud buffer is invalid
};


struct Error
{
  ErrCode     error_code;
  ErrCodeType error_code_type;

  Error ()
    : error_code(INFO_SUCCESS)
  {
  }

  explicit Error(const ErrCode& code) 
   : error_code(code)
  {
    if (error_code < 0x40)
    {
      error_code_type = INFO_CODE;
    }
    else if (error_code < 0x80)
    {
      error_code_type = WARNING_CODE;
    }
    else
    {
      error_code_type = ERROR_CODE;
    }
  }
  std::string toString() const
  {
    switch (error_code)
    {
      // info
      case INFO_PCAPREPEAT:
        return "Info_PcapRepeat";
      case INFO_PCAPEXIT:
        return "Info_PcapExit";

      // warning
      case WARN_MSOPTIMEOUT:
        return "WARN_MSOPTIMEOUT";
      case WARN_MSOPID:
        return "WARN_MSOPID";
      case WARN_MSOPLEN:
        return "WARN_MSOPLEN";
      case WARN_ZEROPOINTS:
        return "WARN_ZEROPOINTS";
      case WARN_PKTBUFOVERFLOW:
        return "WARN_PKTBUFOVERFLOW";
      case WARN_CLOUDOVERFLOW:
        return "WARN_CLOUDOVERFLOW";

      // error
      case ERROR_STARTBEFOREINIT:
        return "ERROR_STARTBEFOREINIT";
      case ERROR_PCAPWRONGPATH:
        return "ERROR_PCAPWRONGPATH";
      case ERROR_POINTCLOUDNULL:
        return "ERROR_POINTCLOUDNULL";

      //default
      default:
        return "INFO_SUCCESS";
    }
  }
};

#define LIMIT_CALL(func, sec)   \
{                               \
  static time_t prev_tm = 0;    \
  time_t cur_tm = time(NULL);   \
  if ((cur_tm - prev_tm) > sec) \
  {                             \
    func;                       \
    prev_tm = cur_tm;           \
  }                             \
}

#define DELAY_LIMIT_CALL(func, sec)   \
{                                     \
  static time_t prev_tm = time(NULL); \
  time_t cur_tm = time(NULL);   \
  if ((cur_tm - prev_tm) > sec) \
  {                             \
    func;                       \
    prev_tm = cur_tm;           \
  }                             \
}
//}
//}