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

#ifndef OHOS_DCAMERA_SOURCE_INIT_STATE_H
#define OHOS_DCAMERA_SOURCE_INIT_STATE_H

#include <map>
#include "dcamera_source_state.h"

namespace OHOS {
namespace DistributedHardware {
class DCameraSourceInitState : public DCameraSourceState {
public:
    explicit DCameraSourceInitState(std::shared_ptr<DCameraSourceStateMachine>& stateMachine);
    ~DCameraSourceInitState() {}

    int32_t Execute(std::shared_ptr<DCameraSourceDev>& camDev, DCAMERA_EVENT eventType,
        DCameraSourceEvent& event) override;
    DCameraStateType GetStateType() override;

private:
    int32_t DoRegisterTask(std::shared_ptr<DCameraSourceDev>& camDev, DCameraSourceEvent& event);
    int32_t DoUnregisterTask(std::shared_ptr<DCameraSourceDev>& camDev, DCameraSourceEvent& event);

    using DCameraFunc = int32_t (DCameraSourceInitState::*)(std::shared_ptr<DCameraSourceDev>& camDev,
        DCameraSourceEvent& event);

private:
    std::weak_ptr<DCameraSourceStateMachine> stateMachine_;
    std::map<uint32_t, DCameraFunc> memberFuncMap_;
};
} // namespace DistributedHardware
} // namespace OHOS
#endif // OHOS_DCAMERA_SOURCE_INIT_STATE_H
