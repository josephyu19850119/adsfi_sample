/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2022. All rights reserved.
 * Description: MDC平台AP标准框架
 */
#ifndef ARA_CAMERADETECTION_CAMERADETECTION_MANAGER_H
#define ARA_CAMERADETECTION_CAMERADETECTION_MANAGER_H

#include "mdc/mdc_adaptive_application.h"
#include "ara/core/string.h"
#include "mdc/sample.h"
namespace mdc {
namespace cameradetection {
class CameradetectionManager : public MdcAdaptiveApplication {
public:
    /* 若需传入参数，请手动添加 */
    CameradetectionManager() = default;
    virtual ~CameradetectionManager() = default;

protected:
    virtual bool OnInitialize() override;
    virtual void Run() override;
    virtual void OnTerminate() override;
private:
    std::unique_ptr<mdc::Sample> samplePtr_;
};
} /* namespace cameradetection */
} /* namespace mdc */
#endif /* ARA_CAMERADETECTION_CAMERADETECTION_MANAGER_H */