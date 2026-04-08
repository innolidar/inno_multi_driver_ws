#pragma once

#include <yaml-cpp/yaml.h>
#include <inno_driver/common/inno_log.hpp>
//#include "utility/common.hpp"

namespace innolight
{
namespace lidar
{

template <typename T>
inline void yamlReadAbort(const YAML::Node& yaml, const std::string& key, T& out_val)
{
  if (!yaml[key] || yaml[key].Type() == YAML::NodeType::Null)
  {
    INNO_ERROR << " : Not set " << key;
    INNO_ERROR << " value, Aborting!!!" << INNO_REND;
    exit(-1);
  }
  else
  {
    out_val = yaml[key].as<T>();
  }
}

template <typename T>
inline bool yamlRead(const YAML::Node& yaml, const std::string& key, T& out_val, const T& default_val)
{
  if (!yaml[key] || yaml[key].Type() == YAML::NodeType::Null)
  {
    out_val = default_val;
    return false;
  }
  else
  {
    out_val = yaml[key].as<T>();
    return true;
  }
}

inline YAML::Node yamlSubNodeAbort(const YAML::Node& yaml, const std::string& node)
{
  YAML::Node ret = yaml[node.c_str()];
  if (!ret)
  {
    INNO_ERROR << " : Cannot find subnode " << node << ". Aborting!!!" << INNO_REND;
    exit(-1);
  }
  return ret;
}

}  // namespace lidar
}  // namespace innolight
