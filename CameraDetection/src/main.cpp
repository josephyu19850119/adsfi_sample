/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2022. All rights reserved.
 * Description: main代码模板
 */
#include <iostream>
#include "cameradetection_manager.h"
#include "mdc/cameradetection_logger.h"

int32_t main(int32_t argc, char* argv[])
{
    (void)argc;
    (void)argv;
    try {
        mdc::cameradetection::CameradetectionManager cameradetectionManager;
        return cameradetectionManager.Execute();
    } catch (const std::exception &exc) {
        std::cerr << "Cameradetection process catch an exception: " << exc.what() << &std::endl;
    } catch (...) {
        std::cerr << "Cameradetection process catch an unknown exception" << &std::endl;
    }
    return 0;
}