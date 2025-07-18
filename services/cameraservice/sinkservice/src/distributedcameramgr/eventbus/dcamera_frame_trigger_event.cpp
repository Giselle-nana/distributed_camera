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

#include "dcamera_frame_trigger_event.h"

namespace OHOS {
namespace DistributedHardware {
DCameraFrameTriggerEvent::DCameraFrameTriggerEvent(EventSender& sender) : Event(sender)
{
}

DCameraFrameTriggerEvent::DCameraFrameTriggerEvent(EventSender& sender, std::string& param)
    : Event(sender), param_(param)
{
}

std::string DCameraFrameTriggerEvent::GetParam()
{
    return param_;
}
} // namespace DistributedHardware
} // namespace OHOS