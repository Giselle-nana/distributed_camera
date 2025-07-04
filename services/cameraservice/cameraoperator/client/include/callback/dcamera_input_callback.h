/*
 * Copyright (c) 2021-2022 Huawei Device Co., Ltd.
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

#ifndef OHOS_DCAMERA_INPUT_CALLBACK_H
#define OHOS_DCAMERA_INPUT_CALLBACK_H

#include "camera_input.h"

#include <map>

#include "camera_device_ability_items.h"
#include "icamera_operator.h"

namespace OHOS {
namespace DistributedHardware {
class DCameraInputCallback : public CameraStandard::ErrorCallback, public CameraStandard::FocusCallback {
public:
    explicit DCameraInputCallback(const std::shared_ptr<StateCallback>& callback);
    void OnError(const int32_t errorType, const int32_t errorMsg) const override;
    void OnFocusState(FocusState state) override;

private:
    std::shared_ptr<StateCallback> callback_;
    const std::map<FocusCallback::FocusState, camera_focus_state_t> focusStateMap_ = {
        {FocusCallback::SCAN, OHOS_CAMERA_FOCUS_STATE_SCAN},
        {FocusCallback::FOCUSED, OHOS_CAMERA_FOCUS_STATE_FOCUSED},
        {FocusCallback::UNFOCUSED, OHOS_CAMERA_FOCUS_STATE_UNFOCUSED}
    };
};
} // namespace DistributedHardware
} // namespace OHOS
#endif // OHOS_DCAMERA_INPUT_CALLBACK_H