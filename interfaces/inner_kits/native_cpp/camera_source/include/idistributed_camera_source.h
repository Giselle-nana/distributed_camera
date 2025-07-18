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

#ifndef OHOS_IDISTRIBUTED_CAMERA_SOURCE_H
#define OHOS_IDISTRIBUTED_CAMERA_SOURCE_H

#include "iremote_broker.h"

#include "idcamera_source_callback.h"
#include "idistributed_hardware_source.h"

namespace OHOS {
namespace DistributedHardware {
class IDistributedCameraSource : public OHOS::IRemoteBroker {
public:
    DECLARE_INTERFACE_DESCRIPTOR(u"ohos.distributedhardware.distributedcamerasource");
    enum {
        INIT_SOURCE = 0,
        RELEASE_SOURCE = 1,
        REGISTER_DISTRIBUTED_HARDWARE = 2,
        UNREGISTER_DISTRIBUTED_HARDWARE = 3,
        CAMERA_NOTIFY = 4,
    };

    virtual ~IDistributedCameraSource() {}
    virtual int32_t InitSource(const std::string& params, const sptr<IDCameraSourceCallback>& callback) = 0;
    virtual int32_t ReleaseSource() = 0;
    virtual int32_t RegisterDistributedHardware(const std::string& devId, const std::string& dhId,
        const std::string& reqId, const EnableParam& param) = 0;
    virtual int32_t UnregisterDistributedHardware(const std::string& devId, const std::string& dhId,
        const std::string& reqId) = 0;
    virtual int32_t DCameraNotify(const std::string& devId, const std::string& dhId, std::string& events) = 0;
};
} // namespace DistributedHardware
} // namespace OHOS
#endif // OHOS_IDISTRIBUTED_CAMERA_SOURCE_H
