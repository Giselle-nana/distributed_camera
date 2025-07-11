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

#include "dcamera_input_callback.h"

#include "camera_util.h"
#include "dcamera_utils_tools.h"
#include "distributed_camera_constants.h"
#include "distributed_hardware_log.h"
#include "metadata_utils.h"

namespace OHOS {
namespace DistributedHardware {
DCameraInputCallback::DCameraInputCallback(const std::shared_ptr<StateCallback>& callback) : callback_(callback)
{
}

void DCameraInputCallback::OnError(const int32_t errorType, const int32_t errorMsg) const
{
    DHLOGE("DCameraInputCallback::OnError, errorType: %d, errorMsg: %d", errorType, errorMsg);
    if (callback_ == nullptr) {
        DHLOGE("DCameraInputCallback::OnError StateCallback is null");
        return;
    }

    std::shared_ptr<DCameraEvent> event = std::make_shared<DCameraEvent>();
    event->eventType_ = DCAMERA_MESSAGE;
    if (errorType == CameraStandard::CamServiceError::CAMERA_DEVICE_PREEMPTED) {
        event->eventResult_ = DCAMERA_EVENT_DEVICE_PREEMPT;
    } else {
        event->eventResult_ = DCAMERA_EVENT_DEVICE_ERROR;
    }
    callback_->OnStateChanged(event);
}

void DCameraInputCallback::OnFocusState(FocusState state)
{
    DHLOGI("DCameraInputCallback::OnFocusState, state: %d", state);
    if (callback_ == nullptr) {
        DHLOGE("DCameraInputCallback::OnFocusState StateCallback is null");
        return;
    }

    auto iter = focusStateMap_.find(state);
    if (iter == focusStateMap_.end()) {
        DHLOGE("DCameraInputCallback::OnFocusState focusStateMap find %d state failed", state);
        return;
    }

    int32_t itemCapacity = 10;
    int32_t dataCapacity = 100;
    int32_t dataCount = 1;
    uint8_t focusState = iter->second;
    std::shared_ptr<Camera::CameraMetadata> cameraMetadata =
        std::make_shared<Camera::CameraMetadata>(itemCapacity, dataCapacity);
    if (!cameraMetadata->addEntry(OHOS_CONTROL_FOCUS_STATE, &focusState, dataCount)) {
        DHLOGE("DCameraInputCallback::OnFocusState cameraMetadata add entry failed");
        return;
    }

    std::string abilityString = Camera::MetadataUtils::EncodeToString(cameraMetadata);
    std::string encodeString = Base64Encode(reinterpret_cast<const unsigned char *>(abilityString.c_str()),
        abilityString.length());

    std::shared_ptr<DCameraSettings> dcSetting = std::make_shared<DCameraSettings>();
    dcSetting->type_ = DCSettingsType::METADATA_RESULT;
    dcSetting->value_ = encodeString;
    std::vector<std::shared_ptr<DCameraSettings>> settings;
    settings.push_back(dcSetting);
    callback_->OnMetadataResult(settings);
}
} // namespace DistributedHardware
} // namespace OHOS