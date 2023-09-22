/* *
 * Class: Object2dArrayIntf client declaration
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2022. All rights reserved.
 * */

#ifndef ADSFI_OBJECT2DARRAYINTFCLIENT
#define ADSFI_OBJECT2DARRAYINTFCLIENT

#include <memory>
#include <vector>
#include <atomic>

#include "adsfi/impl_type_object2darray.h"





namespace adsfi {

using Object2dArrayHandlerType = std::function<void (adsfi::Object2dArray)>;
using Object2dArrayDataType = adsfi::Object2dArray;
using RecvObject2dArrayType = std::shared_ptr<Object2dArrayDataType>;
class Object2dArrayIntfClient final {
public:
    Object2dArrayIntfClient() = delete;
    explicit Object2dArrayIntfClient(const uint32_t instanceId);
    virtual ~Object2dArrayIntfClient();
    bool Init();
    void Stop();

    /* event relative */
    void RegisterObject2dArrayNotifyHandler(const Object2dArrayHandlerType handler);
    void Object2dArrayNotify();
    void Object2dArrayContainerClear();
    bool Object2dArrayContainerEmpty();
    RecvObject2dArrayType GetObject2dArrayOneData(const uint32_t freshDataTime = UINT32_MAX);
    RecvObject2dArrayType GetObject2dArrayOneDataBlocking(const uint32_t blockTimeout = UINT32_MAX);
    std::vector<RecvObject2dArrayType> GetObject2dArrayNdata(const size_t n);

    uint32_t GetRecvQSize() const;
    bool IsStop() const;
    uint32_t GetInstanceId() const;

    
private:
    class Impl;
    std::unique_ptr<Impl> pImpl_;
};
} /* namespace adsfi */

#endif