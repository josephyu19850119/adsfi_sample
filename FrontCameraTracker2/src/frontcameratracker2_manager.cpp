/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2022. All rights reserved.
 * Description: MDC平台AP标准框架
 */
#include "frontcameratracker2_manager.h"
#include "mdc/frontcameratracker2_logger.h"

namespace mdc {
namespace frontcameratracker2 {
/**
* @brief 请在此方法中添加组件代码初始化逻辑，若有阻塞操作，建议开启新线程来运行阻塞操作
*
*/
bool Frontcameratracker2Manager::OnInitialize()
{
    return true;
}

/**
* @brief 请在此方法中添加组件代码运行逻辑，若有阻塞操作，建议开启新线程来运行阻塞操作
*
*/
void Frontcameratracker2Manager::Run()
{
    return;
}

/**
* @brief 若组件作为CM通信服务端，请在此处停止服务
*
*/
void Frontcameratracker2Manager::OnTerminate()
{
    return;
}
} /* namespace frontcameratracker2 */
} /* namespace mdc */