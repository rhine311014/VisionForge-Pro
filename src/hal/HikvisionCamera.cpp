/**
 * @file HikvisionCamera.cpp
 * @brief 海康威视工业相机封装实现
 * @author VisionForge Team
 * @date 2025-12-17
 */

#ifdef USE_HIKVISION_MVS

#include "hal/HikvisionCamera.h"
#include "base/Logger.h"
#include <MvCameraControl.h>
#include <opencv2/imgproc.hpp>

namespace VisionForge {
namespace HAL {

// 静态设备列表缓存
static MV_CC_DEVICE_INFO_LIST g_deviceList = {0};

HikvisionCamera::HikvisionCamera(QObject* parent)
    : ICamera(parent)
    , handle_(nullptr)
    , isOpen_(false)
    , isGrabbing_(false)
    , selectedDeviceIndex_(-1)
    , imageBuffer_(nullptr)
    , bufferSize_(0)
{
}

HikvisionCamera::~HikvisionCamera()
{
    close();

    if (imageBuffer_) {
        delete[] imageBuffer_;
        imageBuffer_ = nullptr;
    }
}

QString HikvisionCamera::sdkVersion()
{
    unsigned int version = MV_CC_GetSDKVersion();
    return QString("MVS SDK %1.%2.%3")
        .arg((version >> 24) & 0xFF)
        .arg((version >> 16) & 0xFF)
        .arg((version >> 8) & 0xFF);
}

QList<CameraDeviceInfo> HikvisionCamera::enumerateDevices()
{
    QList<CameraDeviceInfo> devices;

    LOG_INFO("开始枚举海康相机设备...");
    LOG_INFO(QString("MVS SDK版本: %1").arg(sdkVersion()));

    // 枚举所有类型的设备
    memset(&g_deviceList, 0, sizeof(MV_CC_DEVICE_INFO_LIST));
    int ret = MV_CC_EnumDevices(MV_GIGE_DEVICE | MV_USB_DEVICE, &g_deviceList);
    if (ret != MV_OK) {
        QString errorMsg;
        unsigned int errCode = static_cast<unsigned int>(ret);
        switch (errCode) {
        case 0x8000000C:  // MV_E_RESOURCE
            errorMsg = "资源被占用 - 请关闭海康MVS客户端后重试";
            break;
        case 0x80000000:  // MV_E_HANDLE
            errorMsg = "句柄错误";
            break;
        case 0x80000001:  // MV_E_SUPPORT
            errorMsg = "不支持的操作";
            break;
        default:
            errorMsg = QString("未知错误 0x%1").arg(errCode, 8, 16, QChar('0'));
            break;
        }
        LOG_ERROR(QString("枚举设备失败: %1").arg(errorMsg));
        return devices;
    }

    LOG_INFO(QString("MV_CC_EnumDevices返回成功, 发现 %1 个海康相机").arg(g_deviceList.nDeviceNum));

    for (unsigned int i = 0; i < g_deviceList.nDeviceNum; i++) {
        MV_CC_DEVICE_INFO* pDeviceInfo = g_deviceList.pDeviceInfo[i];
        if (!pDeviceInfo) continue;

        CameraDeviceInfo info;
        info.deviceType = pDeviceInfo->nTLayerType;

        if (pDeviceInfo->nTLayerType == MV_GIGE_DEVICE) {
            // GigE相机
            MV_GIGE_DEVICE_INFO* pGigEInfo = &pDeviceInfo->SpecialInfo.stGigEInfo;

            info.manufacturer = QString::fromLatin1(reinterpret_cast<const char*>(pGigEInfo->chManufacturerName));
            info.modelName = QString::fromLatin1(reinterpret_cast<const char*>(pGigEInfo->chModelName));
            info.serialNumber = QString::fromLatin1(reinterpret_cast<const char*>(pGigEInfo->chSerialNumber));
            info.deviceVersion = QString::fromLatin1(reinterpret_cast<const char*>(pGigEInfo->chDeviceVersion));
            info.userDefinedName = QString::fromLatin1(reinterpret_cast<const char*>(pGigEInfo->chUserDefinedName));

            // IP地址
            unsigned int ip = pGigEInfo->nCurrentIp;
            info.ipAddress = QString("%1.%2.%3.%4")
                .arg((ip >> 24) & 0xFF)
                .arg((ip >> 16) & 0xFF)
                .arg((ip >> 8) & 0xFF)
                .arg(ip & 0xFF);

            LOG_DEBUG(QString("GigE相机: %1 (%2) IP: %3")
                .arg(info.modelName).arg(info.serialNumber).arg(info.ipAddress));

        } else if (pDeviceInfo->nTLayerType == MV_USB_DEVICE) {
            // USB3相机
            MV_USB3_DEVICE_INFO* pUSB3Info = &pDeviceInfo->SpecialInfo.stUsb3VInfo;

            info.manufacturer = QString::fromLatin1(reinterpret_cast<const char*>(pUSB3Info->chManufacturerName));
            info.modelName = QString::fromLatin1(reinterpret_cast<const char*>(pUSB3Info->chModelName));
            info.serialNumber = QString::fromLatin1(reinterpret_cast<const char*>(pUSB3Info->chSerialNumber));
            info.deviceVersion = QString::fromLatin1(reinterpret_cast<const char*>(pUSB3Info->chDeviceVersion));
            info.userDefinedName = QString::fromLatin1(reinterpret_cast<const char*>(pUSB3Info->chUserDefinedName));

            LOG_DEBUG(QString("USB3相机: %1 (%2)")
                .arg(info.modelName).arg(info.serialNumber));
        }

        devices.append(info);
    }

    return devices;
}

void HikvisionCamera::selectDevice(int index)
{
    if (index >= 0 && index < static_cast<int>(g_deviceList.nDeviceNum)) {
        selectedDeviceIndex_ = index;

        // 更新设备信息
        MV_CC_DEVICE_INFO* pDeviceInfo = g_deviceList.pDeviceInfo[index];
        if (pDeviceInfo) {
            if (pDeviceInfo->nTLayerType == MV_GIGE_DEVICE) {
                MV_GIGE_DEVICE_INFO* pGigEInfo = &pDeviceInfo->SpecialInfo.stGigEInfo;
                deviceInfo_.modelName = QString::fromLatin1(reinterpret_cast<const char*>(pGigEInfo->chModelName));
                deviceInfo_.serialNumber = QString::fromLatin1(reinterpret_cast<const char*>(pGigEInfo->chSerialNumber));
                unsigned int ip = pGigEInfo->nCurrentIp;
                deviceInfo_.ipAddress = QString("%1.%2.%3.%4")
                    .arg((ip >> 24) & 0xFF).arg((ip >> 16) & 0xFF)
                    .arg((ip >> 8) & 0xFF).arg(ip & 0xFF);
            } else if (pDeviceInfo->nTLayerType == MV_USB_DEVICE) {
                MV_USB3_DEVICE_INFO* pUSB3Info = &pDeviceInfo->SpecialInfo.stUsb3VInfo;
                deviceInfo_.modelName = QString::fromLatin1(reinterpret_cast<const char*>(pUSB3Info->chModelName));
                deviceInfo_.serialNumber = QString::fromLatin1(reinterpret_cast<const char*>(pUSB3Info->chSerialNumber));
            }
            deviceInfo_.deviceType = pDeviceInfo->nTLayerType;
        }

        LOG_INFO(QString("已选择设备: %1 (%2)").arg(deviceInfo_.modelName).arg(deviceInfo_.serialNumber));
    }
}

bool HikvisionCamera::selectBySerialNumber(const QString& serialNumber)
{
    // 重新枚举设备
    enumerateDevices();

    for (unsigned int i = 0; i < g_deviceList.nDeviceNum; i++) {
        MV_CC_DEVICE_INFO* pDeviceInfo = g_deviceList.pDeviceInfo[i];
        if (!pDeviceInfo) continue;

        QString sn;
        if (pDeviceInfo->nTLayerType == MV_GIGE_DEVICE) {
            sn = QString::fromLatin1(reinterpret_cast<const char*>(
                pDeviceInfo->SpecialInfo.stGigEInfo.chSerialNumber));
        } else if (pDeviceInfo->nTLayerType == MV_USB_DEVICE) {
            sn = QString::fromLatin1(reinterpret_cast<const char*>(
                pDeviceInfo->SpecialInfo.stUsb3VInfo.chSerialNumber));
        }

        if (sn == serialNumber) {
            selectDevice(i);
            return true;
        }
    }

    LOG_WARNING(QString("未找到序列号为 %1 的相机").arg(serialNumber));
    return false;
}

bool HikvisionCamera::selectByIP(const QString& ipAddress)
{
    // 重新枚举设备
    enumerateDevices();

    // 解析目标IP
    QStringList parts = ipAddress.split('.');
    if (parts.size() != 4) return false;

    unsigned int targetIp = (parts[0].toUInt() << 24) |
                           (parts[1].toUInt() << 16) |
                           (parts[2].toUInt() << 8) |
                           parts[3].toUInt();

    for (unsigned int i = 0; i < g_deviceList.nDeviceNum; i++) {
        MV_CC_DEVICE_INFO* pDeviceInfo = g_deviceList.pDeviceInfo[i];
        if (!pDeviceInfo || pDeviceInfo->nTLayerType != MV_GIGE_DEVICE) continue;

        if (pDeviceInfo->SpecialInfo.stGigEInfo.nCurrentIp == targetIp) {
            selectDevice(i);
            return true;
        }
    }

    LOG_WARNING(QString("未找到IP为 %1 的相机").arg(ipAddress));
    return false;
}

bool HikvisionCamera::createHandle()
{
    if (selectedDeviceIndex_ < 0 || selectedDeviceIndex_ >= static_cast<int>(g_deviceList.nDeviceNum)) {
        LOG_ERROR("未选择有效设备");
        return false;
    }

    int ret = MV_CC_CreateHandle(&handle_, g_deviceList.pDeviceInfo[selectedDeviceIndex_]);
    if (ret != MV_OK) {
        LOG_ERROR(QString("创建句柄失败, 错误码: 0x%1").arg(ret, 8, 16, QChar('0')));
        handle_ = nullptr;
        return false;
    }

    return true;
}

void HikvisionCamera::destroyHandle()
{
    if (handle_) {
        MV_CC_DestroyHandle(handle_);
        handle_ = nullptr;
    }
}

bool HikvisionCamera::open()
{
    QMutexLocker locker(&mutex_);

    if (isOpen_) {
        LOG_WARNING("相机已经打开");
        return true;
    }

    // 如果没有选择设备，枚举并选择第一个
    if (selectedDeviceIndex_ < 0) {
        enumerateDevices();
        if (g_deviceList.nDeviceNum == 0) {
            LOG_ERROR("没有找到可用的相机");
            emit errorOccurred("没有找到可用的相机");
            return false;
        }
        selectDevice(0);
    }

    // 创建句柄
    if (!createHandle()) {
        emit errorOccurred("创建相机句柄失败");
        return false;
    }

    // 打开设备
    int ret = MV_CC_OpenDevice(handle_, MV_ACCESS_Exclusive, 0);
    if (ret != MV_OK) {
        LOG_ERROR(QString("打开设备失败, 错误码: 0x%1").arg(ret, 8, 16, QChar('0')));
        destroyHandle();
        emit errorOccurred(QString("打开设备失败: 0x%1").arg(ret, 8, 16, QChar('0')));
        return false;
    }

    // 设置GigE相机的最佳包大小
    MV_CC_DEVICE_INFO* pDeviceInfo = g_deviceList.pDeviceInfo[selectedDeviceIndex_];
    if (pDeviceInfo && pDeviceInfo->nTLayerType == MV_GIGE_DEVICE) {
        int packetSize = MV_CC_GetOptimalPacketSize(handle_);
        if (packetSize > 0) {
            ret = MV_CC_SetIntValue(handle_, "GevSCPSPacketSize", packetSize);
            if (ret != MV_OK) {
                LOG_WARNING(QString("设置包大小失败: 0x%1").arg(ret, 8, 16, QChar('0')));
            }
        }
    }

    // 获取图像参数
    MVCC_INTVALUE width = {0}, height = {0};
    MV_CC_GetIntValue(handle_, "Width", &width);
    MV_CC_GetIntValue(handle_, "Height", &height);
    config_.width = static_cast<int>(width.nCurValue);
    config_.height = static_cast<int>(height.nCurValue);

    // 分配图像缓冲区
    bufferSize_ = config_.width * config_.height * 3;  // 假设最大3通道
    if (imageBuffer_) delete[] imageBuffer_;
    imageBuffer_ = new unsigned char[bufferSize_];

    // 设置为连续采集模式（关闭触发模式）
    ret = MV_CC_SetEnumValue(handle_, "TriggerMode", MV_TRIGGER_MODE_OFF);
    if (ret != MV_OK) {
        LOG_WARNING(QString("设置触发模式失败: 0x%1").arg(static_cast<unsigned int>(ret), 8, 16, QChar('0')));
    } else {
        config_.triggerMode = Continuous;
        LOG_DEBUG("已设置为连续采集模式");
    }

    isOpen_ = true;
    LOG_INFO(QString("相机已打开: %1 (%2) 分辨率: %3x%4")
        .arg(deviceInfo_.modelName).arg(deviceInfo_.serialNumber)
        .arg(config_.width).arg(config_.height));

    return true;
}

void HikvisionCamera::close()
{
    QMutexLocker locker(&mutex_);

    if (!isOpen_) return;

    // 直接停止采集，避免调用 stopGrabbing() 造成死锁
    if (isGrabbing_) {
        MV_CC_StopGrabbing(handle_);
        isGrabbing_ = false;
    }

    if (handle_) {
        MV_CC_CloseDevice(handle_);
        destroyHandle();
    }

    isOpen_ = false;
    LOG_INFO("相机已关闭");
}

bool HikvisionCamera::startGrabbing()
{
    QMutexLocker locker(&mutex_);

    if (!isOpen_) {
        LOG_ERROR("相机未打开");
        return false;
    }

    if (isGrabbing_) {
        return true;
    }

    int ret = MV_CC_StartGrabbing(handle_);
    if (ret != MV_OK) {
        LOG_ERROR(QString("开始采集失败, 错误码: 0x%1").arg(ret, 8, 16, QChar('0')));
        emit errorOccurred(QString("开始采集失败: 0x%1").arg(ret, 8, 16, QChar('0')));
        return false;
    }

    isGrabbing_ = true;
    LOG_INFO("开始采集");
    return true;
}

void HikvisionCamera::stopGrabbing()
{
    QMutexLocker locker(&mutex_);

    if (!isGrabbing_) return;

    int ret = MV_CC_StopGrabbing(handle_);
    if (ret != MV_OK) {
        LOG_WARNING(QString("停止采集失败, 错误码: 0x%1").arg(ret, 8, 16, QChar('0')));
    }

    isGrabbing_ = false;
    LOG_INFO("停止采集");
}

bool HikvisionCamera::trigger()
{
    if (!isOpen_ || !handle_) return false;

    int ret = MV_CC_SetCommandValue(handle_, "TriggerSoftware");
    if (ret != MV_OK) {
        LOG_ERROR(QString("软件触发失败, 错误码: 0x%1").arg(ret, 8, 16, QChar('0')));
        return false;
    }

    return true;
}

Base::ImageData::Ptr HikvisionCamera::grabImage(int timeoutMs)
{
    QMutexLocker locker(&mutex_);

    if (!isOpen_ || !handle_) {
        LOG_ERROR("相机未打开或句柄无效");
        return nullptr;
    }

    // 如果未在采集状态，先启动采集
    if (!isGrabbing_) {
        int ret = MV_CC_StartGrabbing(handle_);
        if (ret != MV_OK) {
            LOG_ERROR(QString("启动采集失败, 错误码: 0x%1").arg(ret, 8, 16, QChar('0')));
            return nullptr;
        }
        isGrabbing_ = true;
        LOG_DEBUG("自动启动采集");
    }

    MV_FRAME_OUT frameOut = {0};
    int ret = MV_CC_GetImageBuffer(handle_, &frameOut, timeoutMs);
    if (ret != MV_OK) {
        unsigned int errCode = static_cast<unsigned int>(ret);
        if (errCode == 0x80000006) {  // MV_E_NODATA - 无数据
            // 软触发模式下可能需要等待
            LOG_DEBUG("等待图像数据...");
        } else if (errCode == 0x80000008) {  // MV_E_CALLORDER - 调用顺序错误
            LOG_ERROR("调用顺序错误，请确保相机已开始采集");
        } else if (errCode == 0x80000003) {  // MV_E_BUFOVER - 缓冲区溢出
            LOG_ERROR("图像缓冲区溢出");
        } else {
            LOG_ERROR(QString("获取图像失败, 错误码: 0x%1").arg(errCode, 8, 16, QChar('0')));
        }
        return nullptr;
    }

    // 获取图像信息
    MV_FRAME_OUT_INFO_EX* pFrameInfo = &frameOut.stFrameInfo;
    int width = pFrameInfo->nWidth;
    int height = pFrameInfo->nHeight;
    unsigned int pixelFormat = pFrameInfo->enPixelType;

    // 根据像素格式创建OpenCV Mat
    cv::Mat image;

    if (pixelFormat == PixelType_Gvsp_Mono8) {
        // 8位灰度图
        image = cv::Mat(height, width, CV_8UC1, frameOut.pBufAddr).clone();
    }
    else if (pixelFormat == PixelType_Gvsp_RGB8_Packed) {
        // RGB格式，转换为BGR
        cv::Mat rgb(height, width, CV_8UC3, frameOut.pBufAddr);
        cv::cvtColor(rgb, image, cv::COLOR_RGB2BGR);
    }
    else if (pixelFormat == PixelType_Gvsp_BGR8_Packed) {
        // BGR格式，直接克隆
        image = cv::Mat(height, width, CV_8UC3, frameOut.pBufAddr).clone();
    }
    else if (pixelFormat == PixelType_Gvsp_BayerRG8 ||
             pixelFormat == PixelType_Gvsp_BayerGB8 ||
             pixelFormat == PixelType_Gvsp_BayerGR8 ||
             pixelFormat == PixelType_Gvsp_BayerBG8) {
        // Bayer格式，需要转换
        MV_CC_PIXEL_CONVERT_PARAM convertParam = {0};
        convertParam.nWidth = width;
        convertParam.nHeight = height;
        convertParam.pSrcData = frameOut.pBufAddr;
        convertParam.nSrcDataLen = pFrameInfo->nFrameLen;
        convertParam.enSrcPixelType = static_cast<MvGvspPixelType>(pixelFormat);
        convertParam.enDstPixelType = PixelType_Gvsp_BGR8_Packed;
        convertParam.pDstBuffer = imageBuffer_;
        convertParam.nDstBufferSize = bufferSize_;

        ret = MV_CC_ConvertPixelType(handle_, &convertParam);
        if (ret == MV_OK) {
            image = cv::Mat(height, width, CV_8UC3, imageBuffer_).clone();
        } else {
            LOG_WARNING(QString("像素格式转换失败: 0x%1").arg(ret, 8, 16, QChar('0')));
            // 回退：作为灰度图处理
            image = cv::Mat(height, width, CV_8UC1, frameOut.pBufAddr).clone();
        }
    }
    else {
        // 其他格式尝试直接使用
        LOG_WARNING(QString("未知像素格式: 0x%1").arg(pixelFormat, 8, 16, QChar('0')));
        image = cv::Mat(height, width, CV_8UC1, frameOut.pBufAddr).clone();
    }

    // 释放图像缓冲区
    MV_CC_FreeImageBuffer(handle_, &frameOut);

    // 创建ImageData
    auto imageData = std::make_shared<Base::ImageData>(image);

    emit imageGrabbed(imageData);
    return imageData;
}

void HikvisionCamera::setConfig(const Config& config)
{
    QMutexLocker locker(&mutex_);

    if (!isOpen_ || !handle_) {
        config_ = config;
        return;
    }

    // 如果正在采集，先停止
    bool wasGrabbing = isGrabbing_;
    if (wasGrabbing) {
        MV_CC_StopGrabbing(handle_);
        isGrabbing_ = false;
    }

    // 设置ROI
    setROI(config.offsetX, config.offsetY, config.width, config.height);

    // 设置曝光
    setFloatValue("ExposureTime", config.exposure);

    // 设置增益
    setFloatValue("Gain", config.gain);

    // 设置触发模式（直接调用，避免死锁）
    switch (config.triggerMode) {
    case Continuous:
        setEnumValue("TriggerMode", MV_TRIGGER_MODE_OFF);
        break;
    case Software:
        setEnumValue("TriggerMode", MV_TRIGGER_MODE_ON);
        setEnumValue("TriggerSource", MV_TRIGGER_SOURCE_SOFTWARE);
        break;
    case Hardware:
        setEnumValue("TriggerMode", MV_TRIGGER_MODE_ON);
        setEnumValue("TriggerSource", MV_TRIGGER_SOURCE_LINE0);
        break;
    }

    config_ = config;

    // 恢复采集
    if (wasGrabbing) {
        MV_CC_StartGrabbing(handle_);
        isGrabbing_ = true;
    }
}

void HikvisionCamera::setExposure(double exposure)
{
    QMutexLocker locker(&mutex_);
    config_.exposure = exposure;

    if (isOpen_ && handle_) {
        setFloatValue("ExposureTime", exposure);
    }
}

double HikvisionCamera::getExposure() const
{
    if (isOpen_ && handle_) {
        return getFloatValue("ExposureTime");
    }
    return config_.exposure;
}

void HikvisionCamera::setGain(double gain)
{
    QMutexLocker locker(&mutex_);
    config_.gain = gain;

    if (isOpen_ && handle_) {
        setFloatValue("Gain", gain);
    }
}

double HikvisionCamera::getGain() const
{
    if (isOpen_ && handle_) {
        return getFloatValue("Gain");
    }
    return config_.gain;
}

void HikvisionCamera::setTriggerMode(TriggerMode mode)
{
    QMutexLocker locker(&mutex_);
    config_.triggerMode = mode;

    if (!isOpen_ || !handle_) return;

    switch (mode) {
    case Continuous:
        setEnumValue("TriggerMode", MV_TRIGGER_MODE_OFF);
        break;

    case Software:
        setEnumValue("TriggerMode", MV_TRIGGER_MODE_ON);
        setEnumValue("TriggerSource", MV_TRIGGER_SOURCE_SOFTWARE);
        break;

    case Hardware:
        setEnumValue("TriggerMode", MV_TRIGGER_MODE_ON);
        setEnumValue("TriggerSource", MV_TRIGGER_SOURCE_LINE0);
        break;
    }
}

void HikvisionCamera::getExposureRange(double& minVal, double& maxVal) const
{
    getFloatRange("ExposureTime", minVal, maxVal);
}

void HikvisionCamera::getGainRange(double& minVal, double& maxVal) const
{
    getFloatRange("Gain", minVal, maxVal);
}

void HikvisionCamera::getWidthRange(int& minVal, int& maxVal) const
{
    int64_t min64, max64;
    getIntRange("Width", min64, max64);
    minVal = static_cast<int>(min64);
    maxVal = static_cast<int>(max64);
}

void HikvisionCamera::getHeightRange(int& minVal, int& maxVal) const
{
    int64_t min64, max64;
    getIntRange("Height", min64, max64);
    minVal = static_cast<int>(min64);
    maxVal = static_cast<int>(max64);
}

bool HikvisionCamera::setROI(int offsetX, int offsetY, int width, int height)
{
    if (!isOpen_ || !handle_) return false;

    // 先设置偏移为0，再设置宽高，最后设置偏移
    setIntValue("OffsetX", 0);
    setIntValue("OffsetY", 0);

    if (!setIntValue("Width", width)) return false;
    if (!setIntValue("Height", height)) return false;
    if (!setIntValue("OffsetX", offsetX)) return false;
    if (!setIntValue("OffsetY", offsetY)) return false;

    config_.width = width;
    config_.height = height;
    config_.offsetX = offsetX;
    config_.offsetY = offsetY;

    // 重新分配缓冲区
    bufferSize_ = width * height * 3;
    if (imageBuffer_) delete[] imageBuffer_;
    imageBuffer_ = new unsigned char[bufferSize_];

    return true;
}

double HikvisionCamera::getFrameRate() const
{
    if (isOpen_ && handle_) {
        return getFloatValue("ResultingFrameRate");
    }
    return 0.0;
}

bool HikvisionCamera::saveParameters()
{
    if (!isOpen_ || !handle_) return false;

    int ret = MV_CC_SetCommandValue(handle_, "UserSetSave");
    if (ret != MV_OK) {
        LOG_ERROR(QString("保存参数失败: 0x%1").arg(ret, 8, 16, QChar('0')));
        return false;
    }

    LOG_INFO("相机参数已保存");
    return true;
}

bool HikvisionCamera::loadParameters()
{
    if (!isOpen_ || !handle_) return false;

    int ret = MV_CC_SetCommandValue(handle_, "UserSetLoad");
    if (ret != MV_OK) {
        LOG_ERROR(QString("加载参数失败: 0x%1").arg(ret, 8, 16, QChar('0')));
        return false;
    }

    LOG_INFO("相机参数已加载");
    return true;
}

// ==================== 辅助方法 ====================

bool HikvisionCamera::setIntValue(const char* key, int64_t value)
{
    if (!handle_) return false;
    int ret = MV_CC_SetIntValue(handle_, key, value);
    if (ret != MV_OK) {
        LOG_WARNING(QString("设置 %1 = %2 失败: 0x%3").arg(key).arg(value).arg(ret, 8, 16, QChar('0')));
        return false;
    }
    return true;
}

bool HikvisionCamera::setFloatValue(const char* key, double value)
{
    if (!handle_) return false;
    int ret = MV_CC_SetFloatValue(handle_, key, static_cast<float>(value));
    if (ret != MV_OK) {
        LOG_WARNING(QString("设置 %1 = %2 失败: 0x%3").arg(key).arg(value).arg(ret, 8, 16, QChar('0')));
        return false;
    }
    return true;
}

bool HikvisionCamera::setEnumValue(const char* key, unsigned int value)
{
    if (!handle_) return false;
    int ret = MV_CC_SetEnumValue(handle_, key, value);
    if (ret != MV_OK) {
        LOG_WARNING(QString("设置 %1 = %2 失败: 0x%3").arg(key).arg(value).arg(ret, 8, 16, QChar('0')));
        return false;
    }
    return true;
}

bool HikvisionCamera::setBoolValue(const char* key, bool value)
{
    if (!handle_) return false;
    int ret = MV_CC_SetBoolValue(handle_, key, value);
    if (ret != MV_OK) {
        LOG_WARNING(QString("设置 %1 = %2 失败: 0x%3").arg(key).arg(value).arg(ret, 8, 16, QChar('0')));
        return false;
    }
    return true;
}

int64_t HikvisionCamera::getIntValue(const char* key) const
{
    if (!handle_) return 0;
    MVCC_INTVALUE value = {0};
    int ret = MV_CC_GetIntValue(handle_, key, &value);
    if (ret != MV_OK) {
        LOG_WARNING(QString("获取 %1 失败: 0x%2").arg(key).arg(ret, 8, 16, QChar('0')));
        return 0;
    }
    return value.nCurValue;
}

double HikvisionCamera::getFloatValue(const char* key) const
{
    if (!handle_) return 0.0;
    MVCC_FLOATVALUE value = {0};
    int ret = MV_CC_GetFloatValue(handle_, key, &value);
    if (ret != MV_OK) {
        LOG_WARNING(QString("获取 %1 失败: 0x%2").arg(key).arg(ret, 8, 16, QChar('0')));
        return 0.0;
    }
    return value.fCurValue;
}

unsigned int HikvisionCamera::getEnumValue(const char* key) const
{
    if (!handle_) return 0;
    MVCC_ENUMVALUE value = {0};
    int ret = MV_CC_GetEnumValue(handle_, key, &value);
    if (ret != MV_OK) {
        LOG_WARNING(QString("获取 %1 失败: 0x%2").arg(key).arg(ret, 8, 16, QChar('0')));
        return 0;
    }
    return value.nCurValue;
}

bool HikvisionCamera::getBoolValue(const char* key) const
{
    if (!handle_) return false;
    bool value = false;
    int ret = MV_CC_GetBoolValue(handle_, key, &value);
    if (ret != MV_OK) {
        LOG_WARNING(QString("获取 %1 失败: 0x%2").arg(key).arg(ret, 8, 16, QChar('0')));
        return false;
    }
    return value;
}

void HikvisionCamera::getIntRange(const char* key, int64_t& minVal, int64_t& maxVal) const
{
    minVal = 0;
    maxVal = 0;
    if (!handle_) return;

    MVCC_INTVALUE value = {0};
    int ret = MV_CC_GetIntValue(handle_, key, &value);
    if (ret == MV_OK) {
        minVal = value.nMin;
        maxVal = value.nMax;
    }
}

void HikvisionCamera::getFloatRange(const char* key, double& minVal, double& maxVal) const
{
    minVal = 0.0;
    maxVal = 0.0;
    if (!handle_) return;

    MVCC_FLOATVALUE value = {0};
    int ret = MV_CC_GetFloatValue(handle_, key, &value);
    if (ret == MV_OK) {
        minVal = value.fMin;
        maxVal = value.fMax;
    }
}

QString HikvisionCamera::getLastError() const
{
    // 海康SDK没有直接的错误消息获取，返回通用错误
    return "相机操作失败";
}

} // namespace HAL
} // namespace VisionForge

#endif // USE_HIKVISION_MVS
