/* *
 * Class: Object2dArrayIntf client declaration
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2022. All rights reserved.
 * */

#ifndef ADSFI_OBJECT2DARRAYINTFCLIENT_IMPL
#define ADSFI_OBJECT2DARRAYINTFCLIENT_IMPL

#include <memory>
#include <vector>
#include <atomic>
#include "adsfi/object2darrayintf_proxy.h"
#include "ara/exec/execution_client.h"
#include <mutex>
#include <condition_variable>
#include "mdc/utility/thread_safe_stack.h"
#include "mdc/camera_track_base_logger.h"
namespace adsfi {
using Object2dArrayIntfProxy = adsfi::proxy::Object2dArrayIntfProxy;

using Object2dArrayHandlerType = std::function<void (adsfi::Object2dArray)>;
using Object2dArrayDataType = adsfi::Object2dArray;
using RecvObject2dArrayType = std::shared_ptr<Object2dArrayDataType>;
namespace {
using LOG_SPACE = mdc::camera_track_base::CameraTrackBaseLogger;
}

class Object2dArrayIntfClientImpl {
public:
    explicit Object2dArrayIntfClientImpl(const uint32_t instanceId);
    virtual ~Object2dArrayIntfClientImpl();
    bool Init();
    void Stop();

    /* event relative */
    void RegisterObject2dArrayNotifyHandler(const Object2dArrayHandlerType handler);
    void Object2dArrayNotify()
    {
        std::lock_guard<std::mutex> recvLk(Object2dArrayMtx_);
        Object2dArrayCv_.notify_all();
    }

    void Object2dArrayContainerClear()
    {
        Object2dArrayContainer_.Clear();
    }

    bool Object2dArrayContainerEmpty()
    {
        return Object2dArrayContainer_.Empty();
    }

    RecvObject2dArrayType GetObject2dArrayOneData(const uint32_t freshDataTime = UINT32_MAX);

    RecvObject2dArrayType GetObject2dArrayOneDataBlocking(const uint32_t blockTimeout = UINT32_MAX);

    std::vector<RecvObject2dArrayType> GetObject2dArrayNdata(const size_t n);
    uint32_t GetRecvQSize() const
    {
        return recvQSize_;
    }

    bool IsStop() const
    {
        return !workFlag_;
    }

    uint32_t GetInstanceId() const
    {
        return instanceIdx_;
    }
    
private:
    /* 服务实例ID */
    uint32_t instanceIdx_;

    /* 服务标识 */
    std::atomic<bool> workFlag_;

    /* 寻找服务标识 */
    std::atomic<bool> findServFlag_;

    /* 注册标识 */
    std::atomic<bool> registerFlag_;

    /* EM模块 */
    ara::exec::ExecutionClient execClient_;

    /* EM 上报标识 */
    std::atomic<bool> emReport_;

    /* 服务发现回调 避免多线程同时执行标识 */
    std::once_flag callFlag_{};

    void Object2dArrayIntfCallback(ara::com::ServiceHandleContainer<Object2dArrayIntfProxy::HandleType> handles,
        const ara::com::FindServiceHandle findServiceHandle);
    std::unique_ptr<Object2dArrayIntfProxy> proxyPtr_{nullptr};
    uint32_t recvQSize_{15U};
    ara::com::FindServiceHandle serviceHandle_{};
    void EmReportExec();

    /* event relative */
    mdc::ThreadSafeStack<RecvObject2dArrayType> Object2dArrayContainer_;
    std::mutex Object2dArrayMtx_;
    std::condition_variable Object2dArrayCv_;
    void PushObject2dArrayDataToContainer(Object2dArrayDataType&& data);
};
} /* namespace adsfi */
#endif