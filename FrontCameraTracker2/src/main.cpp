/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2022. All rights reserved.
 * Description: main代码模板
 */
#include <iostream>
#include "frontcameratracker2_manager.h"
#include "mdc/frontcameratracker2_logger.h"

int32_t main(int32_t argc, char* argv[])
{
    (void)argc;
    (void)argv;
    try {
        mdc::frontcameratracker2::Frontcameratracker2Manager frontcameratracker2Manager;
        return frontcameratracker2Manager.Execute();
    } catch (const std::exception &exc) {
        std::cerr << "Frontcameratracker2 process catch an exception: " << exc.what() << &std::endl;
    } catch (...) {
        std::cerr << "Frontcameratracker2 process catch an unknown exception" << &std::endl;
    }
    return 0;
}