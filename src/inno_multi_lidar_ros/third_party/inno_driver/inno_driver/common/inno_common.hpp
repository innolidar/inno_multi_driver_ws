#pragma once
#include <iostream>
#include <cstring>
#include <string>
#include <memory>
#include <functional>
//
// define ntohs()
// 
#ifdef _WIN32
#include <ws2tcpip.h>
#else //__linux__
#include <arpa/inet.h>
#endif

#include <math.h>

typedef unsigned long uint64_t;
typedef unsigned int uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char uint8_t;

typedef int int32_t;
typedef short int16_t;


namespace innolight
{
namespace lidar
{
inline int16_t SWAP_INT16(int16_t value)
{
    uint8_t* v = (uint8_t*)&value;
    uint8_t temp;
    temp = v[0];
    v[0] = v[1];
    v[1] = temp;
    return value;
}

inline int32_t U8ArrayToInt32(const uint8_t* data, uint8_t len) 
{
    if(len != 4)
    {
      printf("u8ArrayToInt32: len is not 4\n");
      return 0;
    }
    uint32_t uintValue = ntohl(*reinterpret_cast<const uint32_t*>(data));
    return static_cast<int32_t>(uintValue);
}

inline float ConvertUint32ToFloat(uint32_t byteArray) {
    float floatValue;
    std::memcpy(&floatValue, &byteArray, sizeof(float));
    return floatValue;
}

}
}