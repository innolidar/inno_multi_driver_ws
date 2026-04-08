#pragma once
#include <inno_driver/common/inno_common.hpp>
#include <inno_driver/msg/data_types.hpp>
#include <inno_driver/msg/point_types.hpp>
#include <inno_driver/msg/imu_types.hpp>
#include <inno_driver/common/error_code.hpp>
#include "export.h"
template<typename PointCloudT>
class INNODRIVER_API InnoDriver
{
public:
    InnoDriver();
    virtual ~InnoDriver();
    /************************************ 
    函数名：InitDriver
    功  能：初始化驱动
    形  参：
    param:输入驱动参数
    返回值：true(成功),false(失败)
    *************************************/
    bool InitDriver(const DriverParam &param);
    /************************************ 
    函数名：Start
    功  能：启动驱动
    形  参：void
    返回值：true(成功),false(失败)
    *************************************/
    bool Start();
    /************************************ 
    函数名：Stop
    功  能：停止驱动
    形  参：void
    返回值：void
    *************************************/
    void Stop();
    /************************************ 
    函数名：GetLocalFileStatus
    功  能：获取本地数据加载情况
    形  参：
    frame_count:输出本地数据总帧数
    返回值：true(本地数据加载完成),false(本地数据加载未完成)
    *************************************/
    bool GetLocalFileStatus(int &frame_count);
    /************************************ 
    函数名：SetLocalFrame
    功  能：获取到本地数据第index帧点云
    形  参：
    index:输入第index帧点云
    返回值：true(可获取第index帧点云),false(不可获取第index帧点云)
    *************************************/
    bool SetLocalFrame(const int &index);
    /************************************ 
    函数名：RegisterPointCloudCallBack
    功  能：注册点云获取与发布回调函数
    形  参：
    put_cloud_call_back:输入点云发布回调函数
    get_cloud_call_back:输入点云获取回调函数
    返回值：void
    *************************************/
    void RegisterPointCloudCallBack(const std::function<void(std::shared_ptr<PointCloudT> )> &put_cloud_call_back,
                         const std::function<std::shared_ptr<PointCloudT>()> &get_cloud_call_back);
    /************************************ 
    函数名：RegisterPointCloudCallBack
    功  能：注册抛出异常回调函数
    形  参：
    call_back:输入异常回调函数
    返回值：void
    *************************************/
    void RegisterExceptionCallBack(const std::function<void(const Error&)> &call_back);
    /************************************ 
    函数名：RegisterImuCallBack
    功  能：注册IMU获取与发布回调函数
    形  参：
    put_imu_call_back:输入IMU发布回调函数
    get_imu_call_back:输入IMU获取回调函数
    返回值：void
    *************************************/
    void RegisterImuCallBack(const std::function<void(std::shared_ptr<ImuMsg> )> &put_imu_call_back,
                         const std::function<std::shared_ptr<ImuMsg>()> &get_imu_call_back);
private:
    struct Impl;
    std::shared_ptr<Impl> m_pShareImpl{nullptr};
};