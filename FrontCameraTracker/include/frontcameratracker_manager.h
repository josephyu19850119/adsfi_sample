/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2022. All rights reserved.
 * Description: MDC平台AP标准框架
 */
#ifndef ARA_FRONTCAMERATRACKER_FRONTCAMERATRACKER_MANAGER_H
#define ARA_FRONTCAMERATRACKER_FRONTCAMERATRACKER_MANAGER_H

#include "mdc/mdc_adaptive_application.h"
#include "ara/core/string.h"
#include "mdc/sample.h"
namespace mdc {
namespace frontcameratracker {
class FrontcameratrackerManager : public MdcAdaptiveApplication {
public:
    /* 若需传入参数，请手动添加 */
    FrontcameratrackerManager() = default;
    virtual ~FrontcameratrackerManager() = default;

protected:
    virtual bool OnInitialize() override;
    virtual void Run() override;
    virtual void OnTerminate() override;
private:
    std::unique_ptr<mdc::Sample> samplePtr_;
};
} /* namespace frontcameratracker */
} /* namespace mdc */
#endif /* ARA_FRONTCAMERATRACKER_FRONTCAMERATRACKER_MANAGER_H */