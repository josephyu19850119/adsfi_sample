/* *
 * CLASS: Object2dArrayIntf server implementation
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2022. All rights reserved.
 * */

#include "mdc/camera_det_base/object2darrayintf_server_impl.h"

namespace adsfi {
Object2dArrayIntfServerImpl::Object2dArrayIntfServerImpl(const uint32_t id,
    const ara::com::MethodCallProcessingMode& mode)
    : Object2dArrayIntfSkeleton(InstanceIdentifier(ara::core::StringView(std::to_string(id).c_str())),
          mode),
      instanceId_(id),
      workFlag_(true),
      offerServFlag_(false)
{}

Object2dArrayIntfServerImpl::~Object2dArrayIntfServerImpl()
{
    if (workFlag_) {
        Stop();
    }
}

bool Object2dArrayIntfServerImpl::Init()
{
    if (offerServFlag_) {
        return true;
    }

    CreateObject2dArrayThread();
    this->OfferService();
    offerServFlag_ = true;
    return true;
}

void Object2dArrayIntfServerImpl::Stop()
{
    workFlag_ = false;
    if (offerServFlag_) {
        this->StopOfferService();
    }
    offerServFlag_ = false;
    this->Object2dArrayNotify();
    if ((object2dArrayThreadPtr_ != nullptr) && (object2dArrayThreadPtr_->joinable())) {
        object2dArrayThreadPtr_->join();
    }
    object2dArrayThreadPtr_ = nullptr;
    LOG_SPACE::GetLoggerIns("SERV")->LogInfo() << "Data send baes stops offer service. idx: " << instanceId_;
}


void Object2dArrayIntfServerImpl::Object2dArrayNotify()
{
    sendObject2dArrayCv_.notify_one();
    return;
}

bool Object2dArrayIntfServerImpl::SendObject2dArrayData(const SendObject2dArrayType& data)
{
    std::lock_guard<std::mutex> lck(sendObject2dArrayMtx_);
    object2dArrayContainer_.Push(data);
    sendObject2dArrayCv_.notify_one();
    return true;
}

void Object2dArrayIntfServerImpl::TransferObject2dArrayData()
{
    while (workFlag_) {
        std::unique_lock<std::mutex> lck(sendObject2dArrayMtx_);
        sendObject2dArrayCv_.wait(lck);
        if (!workFlag_) {
            LOG_SPACE::GetLoggerIns("SERV")->LogWarn() << "Got StopFlag! Sending function return";
            return;
        }

        auto data = *object2dArrayContainer_.Pop();
        if (!data) {
            LOG_SPACE::GetLoggerIns("SERV")->LogError() << "The data to be sent is empty";
            continue;
        }
        lck.unlock();

        auto val = this->Object2dArray.Allocate();
        *val = *data;
        this->Object2dArray.Send(std::move(val));
        LOG_SPACE::GetLoggerIns("SERV")->LogInfo() << "Skeleton send object success! Instance id: "
                  << GetInstanceId();
    }
}

void Object2dArrayIntfServerImpl::ClearObject2dArrayContainer()
{
    std::lock_guard<std::mutex> lck(sendObject2dArrayMtx_);
    object2dArrayContainer_.Clear();
    return;
}

void Object2dArrayIntfServerImpl::CreateObject2dArrayThread()
{
    object2dArrayThreadPtr_ = std::make_unique<std::thread>(
        std::bind(&Object2dArrayIntfServerImpl::TransferObject2dArrayData, this));
}

} /* namespace adsfi */
