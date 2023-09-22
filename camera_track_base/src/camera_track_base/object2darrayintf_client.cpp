/* *
 * CLASS: Object2dArrayIntf client implementation
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2022. All rights reserved.
 * */

#include "mdc/camera_track_base/object2darrayintf_client.h"
#include "mdc/camera_track_base/object2darrayintf_client_impl.h"
#include <cstring>
#include <iostream>
#include <algorithm>
#include <unistd.h>
#include <stdio.h>
#include <cstdint>
#include "ara/core/string.h"

namespace adsfi {
class Object2dArrayIntfClient::Impl {
public:
    Impl(const uint32_t instanceId)
    {
        Object2dArrayIntfClientPtr_ = std::make_unique<Object2dArrayIntfClientImpl>(instanceId);
    }
    ~Impl() {}
    const std::unique_ptr<Object2dArrayIntfClientImpl>& GetObject2dArrayIntfClient()
    {
        return Object2dArrayIntfClientPtr_;
    }
private:
    std::unique_ptr<Object2dArrayIntfClientImpl> Object2dArrayIntfClientPtr_;
};

Object2dArrayIntfClient::Object2dArrayIntfClient(const uint32_t instanceId)
{
    pImpl_ = std::make_unique<Object2dArrayIntfClient::Impl>(instanceId);
}

Object2dArrayIntfClient::~Object2dArrayIntfClient()
{}

bool Object2dArrayIntfClient::Init()
{
    return pImpl_->GetObject2dArrayIntfClient()->Init();
}

void Object2dArrayIntfClient::Stop()
{
    pImpl_->GetObject2dArrayIntfClient()->Stop();
}

/* event relative */
void Object2dArrayIntfClient::RegisterObject2dArrayNotifyHandler(const Object2dArrayHandlerType handler)
{
    pImpl_->GetObject2dArrayIntfClient()->RegisterObject2dArrayNotifyHandler(handler);
}
void Object2dArrayIntfClient::Object2dArrayNotify()
{
    pImpl_->GetObject2dArrayIntfClient()->Object2dArrayNotify();
}
void Object2dArrayIntfClient::Object2dArrayContainerClear()
{
    pImpl_->GetObject2dArrayIntfClient()->Object2dArrayContainerClear();
}
bool Object2dArrayIntfClient::Object2dArrayContainerEmpty()
{
    return pImpl_->GetObject2dArrayIntfClient()->Object2dArrayContainerEmpty();
}
RecvObject2dArrayType Object2dArrayIntfClient::GetObject2dArrayOneData(const uint32_t freshDataTime)
{
    return pImpl_->GetObject2dArrayIntfClient()->GetObject2dArrayOneData(freshDataTime);
}
RecvObject2dArrayType Object2dArrayIntfClient::GetObject2dArrayOneDataBlocking(const uint32_t blockTimeout)
{
    return pImpl_->GetObject2dArrayIntfClient()->GetObject2dArrayOneDataBlocking(blockTimeout);
}
std::vector<RecvObject2dArrayType> Object2dArrayIntfClient::GetObject2dArrayNdata(const size_t n)
{
    return pImpl_->GetObject2dArrayIntfClient()->GetObject2dArrayNdata(n);
}

uint32_t Object2dArrayIntfClient::GetRecvQSize() const
{
    return pImpl_->GetObject2dArrayIntfClient()->GetRecvQSize();
}
bool Object2dArrayIntfClient::IsStop() const
{
    return pImpl_->GetObject2dArrayIntfClient()->IsStop();
}
uint32_t Object2dArrayIntfClient::GetInstanceId() const
{
    return pImpl_->GetObject2dArrayIntfClient()->GetInstanceId();
}



} /* namespace adsfi */
