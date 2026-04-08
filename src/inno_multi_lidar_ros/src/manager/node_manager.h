#pragma once
#include "utility/yaml_reader.hpp"
#include <memory>
namespace innolight
{
namespace lidar
{
class NodeManager
{
public:
    NodeManager() = default;
    ~NodeManager();
    void Init(const YAML::Node& config);
    void Start();
    void Stop();
private:
    struct Impl;
    std::shared_ptr<Impl> m_impl{nullptr};
};
}
}
