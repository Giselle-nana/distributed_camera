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

#ifndef OHOS_ICAMERA_SINK_DATA_PROCESS_H
#define OHOS_ICAMERA_SINK_DATA_PROCESS_H

#include <cstdint>
#include <memory>

#include "data_buffer.h"
#include "dcamera_capture_info_cmd.h"

namespace OHOS {
namespace DistributedHardware {
class ICameraSinkDataProcess {
public:
    ICameraSinkDataProcess() = default;
    virtual ~ICameraSinkDataProcess() = default;

    virtual int32_t StartCapture(std::shared_ptr<DCameraCaptureInfo>& captureInfo) = 0;
    virtual int32_t StopCapture() = 0;
    virtual int32_t FeedStream(std::shared_ptr<DataBuffer>& dataBuffer) = 0;
};
} // namespace DistributedHardware
} // namespace OHOS
#endif // OHOS_ICAMERA_SINK_DATA_PROCESS_H