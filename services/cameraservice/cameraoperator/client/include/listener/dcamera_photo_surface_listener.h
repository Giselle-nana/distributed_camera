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

#ifndef OHOS_DCAMERA_PHOTO_SURFACE_LISTENER_H
#define OHOS_DCAMERA_PHOTO_SURFACE_LISTENER_H

#include "surface.h"
#include "icamera_operator.h"

namespace OHOS {
namespace DistributedHardware {
class DCameraPhotoSurfaceListener : public IBufferConsumerListener {
public:
    DCameraPhotoSurfaceListener(const sptr<Surface>& surface,
                                const std::shared_ptr<ResultCallback>& callback);
    void OnBufferAvailable() override;

private:
    sptr<Surface> surface_;
    std::shared_ptr<ResultCallback> callback_;
};
} // namespace DistributedHardware
} // namespace OHOS
#endif // OHOS_DCAMERA_PHOTO_SURFACE_LISTENER_H