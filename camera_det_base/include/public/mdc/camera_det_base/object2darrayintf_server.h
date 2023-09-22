/* *
 * Class: Object2dArrayIntf server declaration
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2022. All rights reserved.
 * */
#ifndef ADSFI_OBJECT2DARRAYINTFSERVER
#define ADSFI_OBJECT2DARRAYINTFSERVER
#include <memory>

#include "adsfi/impl_type_object2darray.h"





namespace adsfi {

using Object2dArrayDataType = adsfi::Object2dArray;
using SendObject2dArrayType = std::shared_ptr<Object2dArrayDataType>;


class Object2dArrayIntfServer final {
public:
    Object2dArrayIntfServer() = delete;

    explicit Object2dArrayIntfServer(const uint32_t id);

    virtual ~Object2dArrayIntfServer();

    bool Init();

    void Stop();

    bool IsStop() const;

    uint32_t GetInstanceId() const;

    /* event relative */
    bool SendObject2dArrayData(const SendObject2dArrayType& data);
    void ClearObject2dArrayContainer();
    
    

private:
    class Impl;
    std::unique_ptr<Impl> pImpl_;
};
} /* namespace adsfi */

#endif