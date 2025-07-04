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

#ifndef OHOS_DCAMERA_SOURCE_INPUT_CHANNEL_LISTENER_H
#define OHOS_DCAMERA_SOURCE_INPUT_CHANNEL_LISTENER_H

#include <memory>

#include "icamera_channel_listener.h"
#include "v1_0/dcamera_types.h"

namespace OHOS {
namespace DistributedHardware {
using namespace OHOS::HDI::DistributedCamera::V1_0;
class DCameraSourceInput;
class DCameraSourceInputChannelListener : public ICameraChannelListener {
public:
    DCameraSourceInputChannelListener(std::shared_ptr<DCameraSourceInput>& Input, DCStreamType streamType);
    ~DCameraSourceInputChannelListener();
    void OnSessionState(int32_t state) override;
    void OnSessionError(int32_t eventType, int32_t eventReason, std::string detail) override;
    void OnDataReceived(std::vector<std::shared_ptr<DataBuffer>>& buffers) override;

private:
    std::weak_ptr<DCameraSourceInput> input_;
    DCStreamType streamType_;
};
} // namespace DistributedHardware
} // namespace OHOS
#endif // OHOS_DCAMERA_SOURCE_INPUT_CHANNEL_LISTENER_H
