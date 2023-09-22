/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2022. All rights reserved.
 * Description: class swc
 */

#ifndef MDC_CAMERADETBASESWC_H
#define MDC_CAMERADETBASESWC_H

#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <memory>
#include <atomic>
#include <mutex>
#include <sys/types.h>

#include "ara/core/string.h"
#include "ara/core/vector.h"
#include "ara/core/map.h"

namespace mdc {
class MdcYamlNode;
}

namespace adsfi {
class Object2dArrayIntfServer;
}
namespace mdc {
namespace camera_det_base {
namespace {
using adsfi::Object2dArrayIntfServer;
using InitHandleType = std::function<bool()>;
using StopHandleType = std::function<void()>;
}

class CameraDetBaseSwC {
public:
    explicit CameraDetBaseSwC();

    virtual ~CameraDetBaseSwC();

    bool Init();

    void Stop();

    inline void SetInitCallback(InitHandleType handler)
    {
        initHandler_ = handler;
    }

    inline void SetStopCallback(StopHandleType handler)
    {
        stopHandler_ = handler;
    }

    inline bool IsStop() const
    {
        return !workFlag_;
    }

    ara::core::Vector<std::string> GetObject2dArrayIntfServerVec()
    {
        std::lock_guard<std::mutex> lck(object2dArrayIntfServerPortVecMtx_);
        return object2dArrayIntfServerPortVec_;
    }
    

    std::shared_ptr<Object2dArrayIntfServer> GetObject2dArrayIntfServer(const ara::core::String& portName);
    

private:
    /**
     * @brief 加载instanceID等初始化配置
     *
     */
    bool LoadConfig();

    /**
     * @brief 解析swc管理的interface的instanceID信息
     *
     */
    bool ParsingInstanceId(const std::unique_ptr<MdcYamlNode>& config);

    /**
     * @brief 判断文件是否存在
     *
     */
    bool DoesFileExist(const ara::core::String& filePath);
    bool IsDirType(const mode_t& fileMode);

private:
    /* 初始化标识, 防止多次初始化 */
    std::atomic<bool> initFlag_;

    /* 工作标识 */
    std::atomic<bool> workFlag_;

    /* init回调函数 */
    InitHandleType initHandler_{nullptr};

    /* stop回调函数 */
    StopHandleType stopHandler_{nullptr};

    /* 参数文件路径 */
    ara::core::String configFile_{};

    /* swc标识符 */
    ara::core::String identifier_{};

    /* logger描述符 */
    ara::core::String loggerId_{};
    /* server agent */
    ara::core::Vector<uint32_t> object2dArrayIntfServerIdVec_;
    ara::core::Vector<std::string> object2dArrayIntfServerPortVec_;
    std::mutex object2dArrayIntfServerPortVecMtx_;
    ara::core::Map<std::string, uint32_t> object2dArrayIntfServerIdMap_;
    ara::core::Map<std::string, std::shared_ptr<Object2dArrayIntfServer>> object2dArrayIntfServerInsMap_;
    std::mutex object2dArrayIntfServerInsMapMtx_;

};
}  /* namespace camera_det_base */
}  /* namespace mdc */

#endif  /* MDC_CAMERADETBASESWC_H */