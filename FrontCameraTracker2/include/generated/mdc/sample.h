/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2022. All rights reserved.
 * Description: adsfi sample
 */

#ifndef MDC_SAMPLE_H
#define MDC_SAMPLE_H
#include <memory>
#include <iostream>
#include <atomic>
#include <thread>
#include "frontcameratracker2_logger.h"
#include "mdc/camera_track_base/camera_track_base_swc.h"
#include "mdc/camera_track_base/object2darrayintf_client.h"

namespace mdc {
namespace {
using SPL_LOG_SPACE = mdc::frontcameratracker2::Frontcameratracker2Logger;
}
class Sample {
public:
    Sample();
    ~Sample();

    /**
     * @brief 初始化swc、服务端和客户端
     *
     */
    bool Init();
    /**
     * @brief 运行服务端和客户端
     *
     */
    void Run();
    /**
     * @brief 停止接收/发送数据
     *
     */
    void Stop();
private:
    
    /**
     * @brief 客户端接收数据
     *
     */
    void Object2dArrayIntfClientRun();
    
    /**
     * @brief 用户在初始化过程中自定义操作
     *
     */
    static bool InitHandle();

    /**
     * @brief 用户在停止服务时自定义操作
     *
     */
    static void StopHandle();

    /**
     * @brief 用户在接收到数据时的自定义操作
     *
     */
    static void ReceiveAdsfiObject2dArrayDataHandle(const adsfi::Object2dArrayDataType& data);
    
    std::unique_ptr<mdc::camera_track_base::CameraTrackBaseSwC> swcPtr_;
    std::atomic<bool> workFlag_;
    
    std::unique_ptr<std::thread> Object2dArrayIntfClientThreadPtr_{nullptr};
    
};
}
#endif