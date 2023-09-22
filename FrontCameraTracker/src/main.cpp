/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2022. All rights reserved.
 * Description: main代码模板
 */
#include <iostream>
#include "frontcameratracker_manager.h"
#include "mdc/frontcameratracker_logger.h"

int32_t main(int32_t argc, char* argv[])
{
    (void)argc;
    (void)argv;
    try {
        mdc::frontcameratracker::FrontcameratrackerManager frontcameratrackerManager;
        return frontcameratrackerManager.Execute();
    } catch (const std::exception &exc) {
        std::cerr << "Frontcameratracker process catch an exception: " << exc.what() << &std::endl;
    } catch (...) {
        std::cerr << "Frontcameratracker process catch an unknown exception" << &std::endl;
    }
    return 0;
}