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

#ifndef OHOS_DCAMERA_PREVIEW_CALLBACK_H
#define OHOS_DCAMERA_PREVIEW_CALLBACK_H

#include <cstdint>
#include <memory>

#include "icamera_operator.h"
#include "preview_output.h"

namespace OHOS {
namespace DistributedHardware {
class DCameraPreviewCallback : public CameraStandard::PreviewCallback {
public:
    explicit DCameraPreviewCallback(const std::shared_ptr<StateCallback>& callback);
    void OnFrameStarted() const override;
    void OnFrameEnded(const int32_t frameCount) const override;
    void OnError(const int32_t errorCode) const override;

private:
    std::shared_ptr<StateCallback> callback_;
};
} // namespace DistributedHardware
} // namespace OHOS
#endif // OHOS_DCAMERA_PREVIEW_CALLBACK_H