/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2022. All rights reserved.
 * Description: MDC平台AP标准框架
 */
#ifndef ARA_FRONTCAMERATRACKER2_FRONTCAMERATRACKER2_MANAGER_H
#define ARA_FRONTCAMERATRACKER2_FRONTCAMERATRACKER2_MANAGER_H

#include "mdc/mdc_adaptive_application.h"
#include "ara/core/string.h"
#include "mdc/sample.h"
namespace mdc {
namespace frontcameratracker2 {
class Frontcameratracker2Manager : public MdcAdaptiveApplication {
public:
    /* 若需传入参数，请手动添加 */
    Frontcameratracker2Manager() = default;
    virtual ~Frontcameratracker2Manager() = default;

protected:
    virtual bool OnInitialize() override;
    virtual void Run() override;
    virtual void OnTerminate() override;
private:
    std::unique_ptr<mdc::Sample> samplePtr_;
};
} /* namespace frontcameratracker2 */
} /* namespace mdc */
#endif /* ARA_FRONTCAMERATRACKER2_FRONTCAMERATRACKER2_MANAGER_H */