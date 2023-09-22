/* *
 * CLASS: Object2dArrayIntf client implementation
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2022. All rights reserved.
 * */

#include "mdc/camera_track_base/object2darrayintf_client_impl.h"
#include <cstring>
#include <iostream>
#include <algorithm>
#include <unistd.h>
#include <stdio.h>
#include <cstdint>
#include "ara/core/string.h"

namespace adsfi {
Object2dArrayIntfClientImpl::Object2dArrayIntfClientImpl(const uint32_t instanceId)
    : instanceIdx_(instanceId),
      workFlag_(true),
      findServFlag_(false),
      registerFlag_(false),
      emReport_(false)
{}

Object2dArrayIntfClientImpl::~Object2dArrayIntfClientImpl()
{
    if (workFlag_) {
        Stop();
    }
}

bool Object2dArrayIntfClientImpl::Init()
{
    if (findServFlag_) {
        return true;
    }
    ara::core::String insIdStr = std::to_string(instanceIdx_);
    serviceHandle_ = Object2dArrayIntfProxy::StartFindService([this](
        ara::com::ServiceHandleContainer<Object2dArrayIntfProxy::HandleType> handles,
        const ara::com::FindServiceHandle findServiceHandle) {
            Object2dArrayIntfCallback(std::move(handles), findServiceHandle);
            },
        ara::com::InstanceIdentifier(ara::core::StringView(insIdStr)));
    (void)serviceHandle_;
    const uint32_t maxTimeoutCount = 10U;
    for (uint32_t timeoutCount = 0U; timeoutCount < maxTimeoutCount; timeoutCount++) {
        if (proxyPtr_ != nullptr) {
            findServFlag_ = true;
            return true;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100U));
    }
    LOG_SPACE::GetLoggerIns("CLNT")->LogError() << "Failed to find the service. instance id:" << insIdStr;
    Object2dArrayIntfProxy::StopFindService(serviceHandle_);
    return false;
}

void Object2dArrayIntfClientImpl::Object2dArrayIntfCallback(
    ara::com::ServiceHandleContainer<Object2dArrayIntfProxy::HandleType> handles,
    const ara::com::FindServiceHandle findServiceHandle)
{
    (void)findServiceHandle;
    if (proxyPtr_ != nullptr) {
        return;
    }

    for (auto singleHandle : handles) {
        ara::core::String insIdStr = std::to_string(instanceIdx_);
        if (singleHandle.GetInstanceId().ToString() == insIdStr) {
            /* 查找到指定服务实例后, 创建服务的Proxy实例 */
            std::call_once(callFlag_, [&singleHandle, this]() {
                this->proxyPtr_ = std::make_unique<Object2dArrayIntfProxy>(singleHandle);
            });
            break;
        }
    }
    return;
}

void Object2dArrayIntfClientImpl::Stop()
{
    workFlag_ = false;
    if (findServFlag_) {
        Object2dArrayIntfProxy::StopFindService(serviceHandle_);
    }
    if (proxyPtr_ != nullptr) {
        if (proxyPtr_->Object2dArray.IsSubscribed()) {
            proxyPtr_->Object2dArray.Unsubscribe();
            proxyPtr_->Object2dArray.UnsetReceiveHandler();
        }
        proxyPtr_ = nullptr;
    }
    LOG_SPACE::GetLoggerIns("CLNT")->LogInfo() << "Data receive baes stoped. idx: " << instanceIdx_;
}

void Object2dArrayIntfClientImpl::EmReportExec()
{
    const auto sleepInterval= std::chrono::milliseconds(5U);
    const uint8_t EM_REPORT_TIME_MAX = 3U;
    int count = 0;
    ara::core::Result<void> res;
    while (count < EM_REPORT_TIME_MAX) {
        res = execClient_.ReportExecutionState(ara::exec::ExecutionState::kRunning);
        if (res.HasValue()) {
            break;
        } else {
            std::this_thread::sleep_for(sleepInterval);
            count++;
        }
    }
    if (count == EM_REPORT_TIME_MAX) {
        LOG_SPACE::GetLoggerIns("CLNT")->LogError() << "EM report kRunning failed.";
    }
    return;
}

void Object2dArrayIntfClientImpl::RegisterObject2dArrayNotifyHandler(const Object2dArrayHandlerType handler)
{
    if (proxyPtr_ == nullptr) {
        LOG_SPACE::GetLoggerIns("CLNT")->LogError() << "Failed to find the service instance.";
        return;
    }
    if (registerFlag_) {
        return;
    }
    auto NotifyCallback = [this, handler]() {
        (void)proxyPtr_->Object2dArray.GetNewSamples([this, handler](
            const ara::com::SamplePtr<Object2dArrayDataType const> ptr) {
            const auto& notifyData = ptr.Get();
            auto data = *notifyData;
            handler(data);
            PushObject2dArrayDataToContainer(std::move(data));
            LOG_SPACE::GetLoggerIns("CLNT")->LogInfo() << "Object2dArray: data received. instanceId: " << instanceIdx_;
        });
    };
    proxyPtr_->Object2dArray.SetReceiveHandler([NotifyCallback]() { NotifyCallback(); });
    proxyPtr_->Object2dArray.Subscribe(recvQSize_);
    EmReportExec();
    registerFlag_ = true;
    return;
}

void Object2dArrayIntfClientImpl::PushObject2dArrayDataToContainer(Object2dArrayDataType&& data)
{
    RecvObject2dArrayType ptr = std::make_shared<Object2dArrayDataType>(std::forward<Object2dArrayDataType>(data));
    Object2dArrayContainer_.Push(std::move(ptr));
    Object2dArrayCv_.notify_all();
    return;
}

RecvObject2dArrayType Object2dArrayIntfClientImpl::GetObject2dArrayOneData(const uint32_t freshDataTime)
{
    if (!workFlag_) {
        LOG_SPACE::GetLoggerIns("CLNT")->LogError() << "Data receive got stopFlag signal.";
        return nullptr;
    }
    auto dataPtr = Object2dArrayContainer_.GetOneData(freshDataTime);
    if (dataPtr == nullptr) {
        LOG_SPACE::GetLoggerIns("CLNT")->LogError() << "Get data is nullptr.";
        return nullptr;
    }
    LOG_SPACE::GetLoggerIns("CLNT")->LogInfo() << "Get one data successfully.";
    return *dataPtr;
}

RecvObject2dArrayType Object2dArrayIntfClientImpl::GetObject2dArrayOneDataBlocking(const uint32_t blockTimeout)
{
    std::unique_lock<std::mutex> recvLk(Object2dArrayMtx_);
    if (Object2dArrayCv_.wait_for(recvLk, std::chrono::milliseconds(blockTimeout)) == std::cv_status::timeout) {
        LOG_SPACE::GetLoggerIns("CLNT")->LogError() << "No new data arrived since this api called. timeout(ms): " <<
            blockTimeout;
        return nullptr;
    }
    if (!workFlag_) {
        LOG_SPACE::GetLoggerIns("CLNT")->LogWarn() << "Data receive got stopFlag.";
        return nullptr;
    }
    auto dataPtr = Object2dArrayContainer_.GetOneData();
    recvLk.unlock();
    if (dataPtr == nullptr) {
        LOG_SPACE::GetLoggerIns("CLNT")->LogError() << "Get one data in block mode is nullptr.";
        return nullptr;
    }
    LOG_SPACE::GetLoggerIns("CLNT")->LogInfo() << "Get one data in block mode successfully.";
    return *dataPtr;
}

std::vector<RecvObject2dArrayType> Object2dArrayIntfClientImpl::GetObject2dArrayNdata(const size_t n)
{
    if (!workFlag_) {
        LOG_SPACE::GetLoggerIns("CLNT")->LogWarn() << "Data receive got stopFlag";
        return {};
    }

    auto result = Object2dArrayContainer_.GetNdata(n);

    std::vector<RecvObject2dArrayType> output;
    output.reserve(result.size());

    for (auto& it : result) {
        if (it != nullptr) {
            output.push_back(*it);
        }
    }
    if (output.size() < n) {
        LOG_SPACE::GetLoggerIns("CLNT")->LogWarn()
            << "Failed to obtain " << n << " data records, because the length of the received data is "<< output.size();
        return output;
    }
    LOG_SPACE::GetLoggerIns("CLNT")->LogInfo() << "Get " << n << " data successfully.";
    return output;
}
} /* namespace adsfi */
