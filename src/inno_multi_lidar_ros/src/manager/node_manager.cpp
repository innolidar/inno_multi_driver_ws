#include "node_manager.h"
#include "source/source_driver.hpp"
#include <vector>
namespace innolight
{
namespace lidar
{

struct NodeManager::Impl
{
    Impl()
    {

    }
    ~Impl()
    {
        if (m_source_driver != nullptr)
        {
            m_source_driver->Stop();
            m_source_driver.reset();
            m_source_driver=nullptr;
        }
    }
    void Init(const YAML::Node& config)
    {
        YAML::Node common_config = yamlSubNodeAbort(config, "common");
        int msg_source = 0;
        yamlRead<int>(common_config, "msg_source", msg_source, 0);

        YAML::Node lidar_config = yamlSubNodeAbort(config, "lidar");
        m_source_driver = std::make_shared<SourceDriver>();
        m_source_driver->Init(SourceType::MSG_FROM_LIDAR,lidar_config,common_config);
    }
    void Start()
    {
        if (m_source_driver != nullptr)
        {
            m_source_driver->Start();
        }
    }
    void Stop()
    {
        if (m_source_driver != nullptr)
        {
            m_source_driver->Stop();
            m_source_driver.reset();
            m_source_driver=nullptr;
        }
    }
private:
    SourceDriver::Ptr m_source_driver;
};

NodeManager::~NodeManager()
{
    m_impl.reset();
    m_impl=nullptr;
}

void NodeManager::Init(const YAML::Node& config)
{
    if(m_impl==nullptr)
    {
        m_impl=std::make_shared<NodeManager::Impl>();
    }
    m_impl->Init(config);
}
void NodeManager::Start()
{
    m_impl->Start();
}

void NodeManager::Stop()
{
    m_impl->Stop();
}
}
}