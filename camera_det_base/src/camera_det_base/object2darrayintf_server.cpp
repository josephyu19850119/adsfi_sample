/* *
 * CLASS: Object2dArrayIntf server implementation
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2022. All rights reserved.
 * */

#include "mdc/camera_det_base/object2darrayintf_server.h"
#include "mdc/camera_det_base/object2darrayintf_server_impl.h"

namespace adsfi {
class Object2dArrayIntfServer::Impl {
public:
    Impl(const uint32_t id,
        const ara::com::MethodCallProcessingMode& mode = ara::com::MethodCallProcessingMode::kPoll)
    {
        Object2dArrayIntfServerPtr_ = std::make_unique<Object2dArrayIntfServerImpl>(id, mode);
    }
    ~Impl() {}
    const std::unique_ptr<Object2dArrayIntfServerImpl>& GetObject2dArrayIntfServer()
    {
        return Object2dArrayIntfServerPtr_;
    }
private:
    std::unique_ptr<Object2dArrayIntfServerImpl> Object2dArrayIntfServerPtr_;
};

Object2dArrayIntfServer::Object2dArrayIntfServer(const uint32_t id)
{
    pImpl_ = std::make_unique<Object2dArrayIntfServer::Impl>(id);
}

Object2dArrayIntfServer::~Object2dArrayIntfServer()
{}

bool Object2dArrayIntfServer::Init()
{
    return pImpl_->GetObject2dArrayIntfServer()->Init();
}

void Object2dArrayIntfServer::Stop()
{
    pImpl_->GetObject2dArrayIntfServer()->Stop();
}

bool Object2dArrayIntfServer::IsStop() const
{
    return pImpl_->GetObject2dArrayIntfServer()->IsStop();
}

uint32_t Object2dArrayIntfServer::GetInstanceId() const
{
    return pImpl_->GetObject2dArrayIntfServer()->GetInstanceId();
}

/* event relative */
bool Object2dArrayIntfServer::SendObject2dArrayData(const SendObject2dArrayType& data)
{
    return pImpl_->GetObject2dArrayIntfServer()->SendObject2dArrayData(data);
}
void Object2dArrayIntfServer::ClearObject2dArrayContainer()
{
    pImpl_->GetObject2dArrayIntfServer()->ClearObject2dArrayContainer();
}

} /* namespace adsfi */
