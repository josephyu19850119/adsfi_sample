/* *
 * Class: Object2dArrayIntf server declaration
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2022. All rights reserved.
 * */
#ifndef ADSFI_OBJECT2DARRAYINTFSERVERIMPL
#define ADSFI_OBJECT2DARRAYINTFSERVERIMPL

#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include "mdc/camera_det_base_logger.h"
#include "ara/core/future.h"
#include "ara/com/types.h"
#include "mdc/utility/thread_safe_stack.h"

#include "adsfi/object2darrayintf_skeleton.h"

namespace adsfi {

using ara::com::InstanceIdentifier;
using ara::com::MethodCallProcessingMode;
using adsfi::skeleton::Object2dArrayIntfSkeleton;
using Object2dArrayDataType = adsfi::Object2dArray;
using SendObject2dArrayType = std::shared_ptr<Object2dArrayDataType>;

namespace {
using LOG_SPACE = mdc::camera_det_base::CameraDetBaseLogger;
}

class Object2dArrayIntfServerImpl : public Object2dArrayIntfSkeleton {
public:
    explicit Object2dArrayIntfServerImpl(const uint32_t id,
        const ara::com::MethodCallProcessingMode& mode = ara::com::MethodCallProcessingMode::kPoll);

    virtual ~Object2dArrayIntfServerImpl();

    bool Init();

    void Stop();

    inline bool IsStop() const
    {
        return !workFlag_;
    }

    inline uint32_t GetInstanceId() const
    {
        return instanceId_;
    }
    /* event relative */
    bool SendObject2dArrayData(const SendObject2dArrayType& data);
    void ClearObject2dArrayContainer();
    
    

private:
    /* event relative */
    std::unique_ptr<std::thread> object2dArrayThreadPtr_{nullptr};
    void CreateObject2dArrayThread();
    void Object2dArrayNotify();
    void TransferObject2dArrayData();

    /* 服务实例ID */
    uint32_t instanceId_;

    /* 服务标识 */
    std::atomic<bool> workFlag_;

    /* 提供服务标识 */
    std::atomic<bool> offerServFlag_;

    /* event relative */
    mdc::ThreadSafeStack<SendObject2dArrayType> object2dArrayContainer_;
    std::mutex sendObject2dArrayMtx_;
    std::condition_variable sendObject2dArrayCv_;
};
} /* namespace adsfi */

#endif