/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef OHOS_DCAMERA_SERVICE_STATE_LISTENER_H
#define OHOS_DCAMERA_SERVICE_STATE_LISTENER_H

#include <mutex>

#include "icamera_state_listener.h"
#include "idcamera_source_callback.h"

namespace OHOS {
namespace DistributedHardware {
class DCameraServiceStateListener : public ICameraStateListener {
public:
    explicit DCameraServiceStateListener();
    ~DCameraServiceStateListener();

    int32_t OnRegisterNotify(const std::string& devId, const std::string& dhId, const std::string& reqId,
        int32_t status, std::string& data) override;
    int32_t OnUnregisterNotify(const std::string& devId, const std::string& dhId, const std::string& reqId,
        int32_t status, std::string& data) override;
    void SetCallback(sptr<IDCameraSourceCallback> callback) override;
private:
    sptr<IDCameraSourceCallback> callbackProxy_;
    std::mutex proxyMutex_;
};
} // namespace DistributedHardware
} // namespace OHOS
#endif // OHOS_DCAMERA_SERVICE_STATE_LISTENER_H
