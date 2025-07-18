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

#include "dcamera_handler.h"

#include <functional>

#include "anonymous_string.h"
#include "avcodec_info.h"
#include "avcodec_list.h"
#include "dcamera_manager_callback.h"
#include "dcamera_utils_tools.h"
#include "distributed_camera_constants.h"
#include "distributed_camera_errno.h"
#include "distributed_hardware_log.h"
#include "metadata_utils.h"

namespace OHOS {
namespace DistributedHardware {
IMPLEMENT_SINGLE_INSTANCE(DCameraHandler);

DCameraHandler::~DCameraHandler()
{
    DHLOGI("~DCameraHandler");
}

int32_t DCameraHandler::Initialize()
{
    DHLOGI("DCameraHandler::Initialize");
    cameraManager_ = CameraStandard::CameraManager::GetInstance();
    if (cameraManager_ == nullptr) {
        DHLOGE("DCameraHandler::Initialize cameraManager getInstance failed");
        return DCAMERA_INIT_ERR;
    }
    std::shared_ptr<DCameraManagerCallback> cameraMgrCallback = std::make_shared<DCameraManagerCallback>();
    cameraManager_->SetCallback(cameraMgrCallback);
    DHLOGI("DCameraHandler::Initialize success");
    return DCAMERA_OK;
}

std::vector<DHItem> DCameraHandler::Query()
{
    std::vector<DHItem> itemList;
    std::vector<sptr<CameraStandard::CameraInfo>> cameraList = cameraManager_->GetCameras();
    DHLOGI("DCameraHandler::Query get %d cameras", cameraList.size());
    if (cameraList.empty()) {
        DHLOGE("DCameraHandler::Query no camera device");
        return itemList;
    }
    for (auto& info : cameraList) {
        if (info->GetConnectionType() != OHOS_CAMERA_CONNECTION_TYPE_BUILTIN) {
            DHLOGI("DCameraHandler::Query connection type: %d", info->GetConnectionType());
            continue;
        }
        if ((info->GetPosition() == OHOS_CAMERA_POSITION_OTHER) ||
            (info->GetPosition() == OHOS_CAMERA_POSITION_FRONT) ||
            (info->GetPosition() == OHOS_CAMERA_POSITION_BACK &&
             info->GetCameraType() == OHOS_CAMERA_TYPE_LOGICAL)) {
            DHItem item = CreateDHItem(info);
            itemList.emplace_back(item);
        }
    }
    DHLOGI("DCameraHandler::Query success, get %d items", itemList.size());
    return itemList;
}

std::map<std::string, std::string> DCameraHandler::QueryExtraInfo()
{
    DHLOGI("DCameraHandler::QueryExtraInfo");
    std::map<std::string, std::string> extraInfo;
    return extraInfo;
}

bool DCameraHandler::IsSupportPlugin()
{
    DHLOGI("DCameraHandler::IsSupportPlugin");
    return false;
}

void DCameraHandler::RegisterPluginListener(std::shared_ptr<PluginListener> listener)
{
    DHLOGI("DCameraHandler::RegisterPluginListener");
    if (listener == nullptr) {
        DHLOGE("DCameraHandler unregistering plugin listener");
    }
    pluginListener_ = listener;
}

void DCameraHandler::UnRegisterPluginListener()
{
    DHLOGI("DCameraHandler::UnRegisterPluginListener");
    pluginListener_ = nullptr;
}

std::vector<std::string> DCameraHandler::GetCameras()
{
    std::vector<std::string> cameras;
    std::vector<sptr<CameraStandard::CameraInfo>> cameraList = cameraManager_->GetCameras();
    DHLOGI("DCameraHandler::GetCameras get %d cameras", cameraList.size());
    if (cameraList.empty()) {
        DHLOGE("DCameraHandler::GetCameras no camera device");
        return cameras;
    }
    for (auto& info : cameraList) {
        if (info->GetConnectionType() != OHOS_CAMERA_CONNECTION_TYPE_BUILTIN) {
            DHLOGI("DCameraHandler::GetCameras connection type: %d", info->GetConnectionType());
            continue;
        }
        if ((info->GetPosition() == OHOS_CAMERA_POSITION_OTHER) ||
            (info->GetPosition() == OHOS_CAMERA_POSITION_FRONT) ||
            (info->GetPosition() == OHOS_CAMERA_POSITION_BACK &&
             info->GetCameraType() == OHOS_CAMERA_TYPE_LOGICAL)) {
            std::string dhId = CAMERA_ID_PREFIX + info->GetID();
            cameras.emplace_back(dhId);
        }
    }
    DHLOGI("DCameraHandler::GetCameras success, get %d items", cameras.size());
    return cameras;
}

DHItem DCameraHandler::CreateDHItem(sptr<CameraStandard::CameraInfo>& info)
{
    DHItem item;
    std::string id = info->GetID();
    item.dhId = CAMERA_ID_PREFIX + id;
    DHLOGI("DCameraHandler::CreateDHItem camera id: %s", GetAnonyString(id).c_str());

    Json::Value root;
    root[CAMERA_PROTOCOL_VERSION_KEY] = Json::Value(CAMERA_PROTOCOL_VERSION_VALUE);
    root[CAMERA_POSITION_KEY] = Json::Value(GetCameraPosition(info->GetPosition()));

    std::shared_ptr<Media::AVCodecList> avCodecList = Media::AVCodecListFactory::CreateAVCodecList();
    std::vector<std::shared_ptr<Media::VideoCaps>> videoCapsList = avCodecList->GetVideoEncoderCaps();
    for (auto& videoCaps : videoCapsList) {
        std::shared_ptr<Media::AVCodecInfo> codecInfo = videoCaps->GetCodecInfo();
        std::string name = codecInfo->GetName();
        root[CAMERA_CODEC_TYPE_KEY].append(name);
        DHLOGI("DCameraHandler::CreateDHItem codec type: %s", name.c_str());
    }

    sptr<CameraStandard::CameraInput> cameraInput = cameraManager_->CreateCameraInput(info);
    if (cameraInput == nullptr) {
        DHLOGE("DCameraHandler::CreateDHItem create cameraInput failed");
        return item;
    }

    Json::Value outputFormat;
    Json::Value resolution;
    std::set<camera_format_t> formatSet;

    std::vector<camera_format_t> videoFormats = cameraInput->GetSupportedVideoFormats();
    ConfigInfo videoConfig = {CONTINUOUS_FRAME, CAMERA_FORMAT_VIDEO, cameraInput};
    ConfigFormatAndResolution(videoConfig, outputFormat,  resolution, videoFormats, formatSet);

    std::vector<camera_format_t> previewFormats = cameraInput->GetSupportedPreviewFormats();
    ConfigInfo previewInfo = {CONTINUOUS_FRAME, CAMERA_FORMAT_PREVIEW, cameraInput};
    ConfigFormatAndResolution(previewInfo, outputFormat, resolution, previewFormats, formatSet);

    std::vector<camera_format_t> photoFormats = cameraInput->GetSupportedPhotoFormats();
    ConfigInfo photoConfig = {SNAPSHOT_FRAME, CAMERA_FORMAT_PHOTO, cameraInput};
    ConfigFormatAndResolution(photoConfig, outputFormat, resolution, photoFormats, formatSet);

    root[CAMERA_FORMAT_KEY] = outputFormat;
    root[CAMERA_RESOLUTION_KEY] = resolution;

    std::hash<std::string> h;
    std::string abilityString = cameraInput->GetCameraSettings();
    DHLOGI("DCameraHandler::CreateDHItem abilityString hash: %zu, length: %zu",
        h(abilityString), abilityString.length());

    std::string encodeString = Base64Encode(reinterpret_cast<const unsigned char *>(abilityString.c_str()),
        abilityString.length());
    DHLOGI("DCameraHandler::CreateDHItem encodeString hash: %zu, length: %zu",
        h(encodeString), encodeString.length());
    root[CAMERA_METADATA_KEY] = Json::Value(encodeString);

    item.attrs = root.toStyledString();
    cameraInput->Release();
    return item;
}

std::string DCameraHandler::GetCameraPosition(camera_position_enum_t position)
{
    DHLOGI("DCameraHandler::GetCameraPosition position: %d", position);
    std::string ret = "";
    switch (position) {
        case OHOS_CAMERA_POSITION_BACK: {
            ret = CAMERA_POSITION_BACK;
            break;
        }
        case OHOS_CAMERA_POSITION_FRONT: {
            ret = CAMERA_POSITION_FRONT;
            break;
        }
        case OHOS_CAMERA_POSITION_OTHER: {
            ret = CAMERA_POSITION_UNSPECIFIED;
            break;
        }
        default: {
            DHLOGE("DCameraHandler::GetCameraPosition unknown camera position");
            break;
        }
    }
    DHLOGI("DCameraHandler::GetCameraPosition success ret: %s", ret.c_str());
    return ret;
}

void DCameraHandler::ConfigFormatAndResolution(ConfigInfo& info, Json::Value& outputFormat, Json::Value& resolution,
    std::vector<camera_format_t>& formatList, std::set<camera_format_t>& formatSet)
{
    DHLOGI("DCameraHandler::ConfigFormatAndResolution camera format size: %d", formatList.size());
    for (auto& format : formatList) {
        DHLOGI("DCameraHandler::ConfigFormatAndResolution %s format: %d", info.formatKey.c_str(), format);
        outputFormat[info.formatKey].append(format);
        if (formatSet.insert(format).second) {
            std::vector<CameraStandard::CameraPicSize> picSizes = info.cameraInput->getSupportedSizes(format);
            std::string keyName = std::to_string(format);
            DHLOGI("DCameraHandler::ConfigFormatAndResolution get %d supported camera pic size", picSizes.size());
            for (auto& size : picSizes) {
                if (IsValid(info.type, size)) {
                    std::string value = std::to_string(size.width) + "*" + std::to_string(size.height);
                    resolution[keyName].append(value);
                    DHLOGI("DCameraHandler::ConfigFormatAndResolution format %d resolution %s", format, value.c_str());
                }
            }
        }
    }
}

bool DCameraHandler::IsValid(const DCStreamType type, const CameraStandard::CameraPicSize& size)
{
    bool ret = false;
    switch (type) {
        case CONTINUOUS_FRAME: {
            ret = (size.width >= RESOLUTION_MIN_WIDTH) &&
                    (size.height >= RESOLUTION_MIN_HEIGHT) &&
                    (size.width <= RESOLUTION_MAX_WIDTH_CONTINUOUS) &&
                    (size.height <= RESOLUTION_MAX_HEIGHT_CONTINUOUS);
            break;
        }
        case SNAPSHOT_FRAME: {
            uint64_t dcResolution = static_cast<uint64_t>(size.width * size.width);
            uint64_t dcMaxResolution = static_cast<uint64_t>(RESOLUTION_MAX_WIDTH_SNAPSHOT *
                                                             RESOLUTION_MAX_HEIGHT_SNAPSHOT);
            uint64_t dcMinResolution = static_cast<uint64_t>(RESOLUTION_MIN_WIDTH *
                                                             RESOLUTION_MIN_HEIGHT);
            ret = (dcResolution >= dcMinResolution) && (dcResolution <= dcMaxResolution);
            break;
        }
        default: {
            DHLOGE("DCameraHandler::isValid unknown stream type");
            break;
        }
    }
    return ret;
}

IHardwareHandler* GetHardwareHandler()
{
    DHLOGI("DCameraHandler::GetHardwareHandler");
    return &DCameraHandler::GetInstance();
}
} // namespace DistributedHardware
} // namespace OHOS