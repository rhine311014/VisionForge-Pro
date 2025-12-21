/**
 * @file BaslerCamera.cpp
 * @brief Basler工业相机封装实现 (预留框架)
 * @author VisionForge Team
 * @date 2025-12-17
 *
 * @note 此为预留框架，需要安装Basler Pylon SDK后完善实现
 */

#ifdef USE_BASLER_PYLON

#include "hal/BaslerCamera.h"
#include "base/Logger.h"
#include "base/ImageMemoryPool.h"

// Pylon SDK头文件
#include <pylon/PylonIncludes.h>

namespace VisionForge {
namespace HAL {

BaslerCamera::BaslerCamera(QObject* parent)
    : ICamera(parent)
    , isOpen_(false)
    , isGrabbing_(false)
    , selectedDeviceIndex_(-1)
    , camera_(nullptr)
{
    // 初始化Pylon运行时
    Pylon::PylonInitialize();
}

BaslerCamera::~BaslerCamera()
{
    close();

    // 释放Pylon运行时
    Pylon::PylonTerminate();
}

QString BaslerCamera::sdkVersion()
{
    return QString("Pylon %1.%2.%3")
        .arg(PYLON_VERSION_MAJOR)
        .arg(PYLON_VERSION_MINOR)
        .arg(PYLON_VERSION_SUBMINOR);
}

QList<BaslerDeviceInfo> BaslerCamera::enumerateDevices()
{
    QList<BaslerDeviceInfo> devices;

    try {
        Pylon::CTlFactory& tlFactory = Pylon::CTlFactory::GetInstance();
        Pylon::DeviceInfoList_t deviceList;

        if (tlFactory.EnumerateDevices(deviceList) == 0) {
            LOG_INFO("未发现Basler相机");
            return devices;
        }

        LOG_INFO(QString("发现 %1 个Basler相机").arg(deviceList.size()));

        for (size_t i = 0; i < deviceList.size(); i++) {
            const Pylon::CDeviceInfo& devInfo = deviceList[i];

            BaslerDeviceInfo info;
            info.modelName = QString::fromStdString(devInfo.GetModelName().c_str());
            info.serialNumber = QString::fromStdString(devInfo.GetSerialNumber().c_str());
            info.vendorName = QString::fromStdString(devInfo.GetVendorName().c_str());

            // 获取接口类型
            QString deviceClass = QString::fromStdString(devInfo.GetDeviceClass().c_str());
            if (deviceClass.contains("GigE", Qt::CaseInsensitive)) {
                info.interfaceType = "GigE";
                info.ipAddress = QString::fromStdString(devInfo.GetIpAddress().c_str());
            } else if (deviceClass.contains("USB", Qt::CaseInsensitive)) {
                info.interfaceType = "USB3";
            } else {
                info.interfaceType = deviceClass;
            }

            LOG_DEBUG(QString("Basler相机: %1 (%2) %3")
                .arg(info.modelName).arg(info.serialNumber).arg(info.interfaceType));

            devices.append(info);
        }
    }
    catch (const Pylon::GenericException& e) {
        LOG_ERROR(QString("枚举Basler设备失败: %1").arg(e.GetDescription()));
    }

    return devices;
}

void BaslerCamera::selectDevice(int index)
{
    selectedDeviceIndex_ = index;
    LOG_INFO(QString("已选择Basler设备索引: %1").arg(index));
}

bool BaslerCamera::selectBySerialNumber(const QString& serialNumber)
{
    try {
        Pylon::CTlFactory& tlFactory = Pylon::CTlFactory::GetInstance();
        Pylon::DeviceInfoList_t deviceList;
        tlFactory.EnumerateDevices(deviceList);

        for (size_t i = 0; i < deviceList.size(); i++) {
            if (QString::fromStdString(deviceList[i].GetSerialNumber().c_str()) == serialNumber) {
                selectDevice(static_cast<int>(i));
                deviceInfo_.serialNumber = serialNumber;
                deviceInfo_.modelName = QString::fromStdString(deviceList[i].GetModelName().c_str());
                return true;
            }
        }
    }
    catch (const Pylon::GenericException& e) {
        LOG_ERROR(QString("选择设备失败: %1").arg(e.GetDescription()));
    }

    LOG_WARNING(QString("未找到序列号为 %1 的Basler相机").arg(serialNumber));
    return false;
}

bool BaslerCamera::selectByIP(const QString& ipAddress)
{
    try {
        Pylon::CTlFactory& tlFactory = Pylon::CTlFactory::GetInstance();
        Pylon::DeviceInfoList_t deviceList;
        tlFactory.EnumerateDevices(deviceList);

        for (size_t i = 0; i < deviceList.size(); i++) {
            if (QString::fromStdString(deviceList[i].GetIpAddress().c_str()) == ipAddress) {
                selectDevice(static_cast<int>(i));
                deviceInfo_.ipAddress = ipAddress;
                deviceInfo_.modelName = QString::fromStdString(deviceList[i].GetModelName().c_str());
                deviceInfo_.serialNumber = QString::fromStdString(deviceList[i].GetSerialNumber().c_str());
                return true;
            }
        }
    }
    catch (const Pylon::GenericException& e) {
        LOG_ERROR(QString("选择设备失败: %1").arg(e.GetDescription()));
    }

    LOG_WARNING(QString("未找到IP为 %1 的Basler相机").arg(ipAddress));
    return false;
}

bool BaslerCamera::open()
{
    QMutexLocker locker(&mutex_);

    if (isOpen_) {
        return true;
    }

    try {
        Pylon::CTlFactory& tlFactory = Pylon::CTlFactory::GetInstance();
        Pylon::DeviceInfoList_t deviceList;

        if (tlFactory.EnumerateDevices(deviceList) == 0) {
            LOG_ERROR("没有找到Basler相机");
            emit errorOccurred("没有找到Basler相机");
            return false;
        }

        if (selectedDeviceIndex_ < 0) {
            selectedDeviceIndex_ = 0;
        }

        if (selectedDeviceIndex_ >= static_cast<int>(deviceList.size())) {
            LOG_ERROR("设备索引超出范围");
            emit errorOccurred("设备索引超出范围");
            return false;
        }

        // 创建相机对象
        Pylon::CInstantCamera* pCamera = new Pylon::CInstantCamera(
            tlFactory.CreateDevice(deviceList[selectedDeviceIndex_]));
        camera_ = pCamera;

        // 打开相机
        pCamera->Open();

        // 获取设备信息
        deviceInfo_.modelName = QString::fromStdString(
            pCamera->GetDeviceInfo().GetModelName().c_str());
        deviceInfo_.serialNumber = QString::fromStdString(
            pCamera->GetDeviceInfo().GetSerialNumber().c_str());

        // 获取图像参数
        GenApi::INodeMap& nodemap = pCamera->GetNodeMap();
        config_.width = static_cast<int>(
            GenApi::CIntegerPtr(nodemap.GetNode("Width"))->GetValue());
        config_.height = static_cast<int>(
            GenApi::CIntegerPtr(nodemap.GetNode("Height"))->GetValue());

        isOpen_ = true;
        LOG_INFO(QString("Basler相机已打开: %1 (%2)")
            .arg(deviceInfo_.modelName).arg(deviceInfo_.serialNumber));

        return true;
    }
    catch (const Pylon::GenericException& e) {
        LOG_ERROR(QString("打开Basler相机失败: %1").arg(e.GetDescription()));
        emit errorOccurred(QString("打开相机失败: %1").arg(e.GetDescription()));
        return false;
    }
}

void BaslerCamera::close()
{
    QMutexLocker locker(&mutex_);

    if (!isOpen_) return;

    if (isGrabbing_) {
        stopGrabbing();
    }

    try {
        if (camera_) {
            Pylon::CInstantCamera* pCamera = static_cast<Pylon::CInstantCamera*>(camera_);
            pCamera->Close();
            delete pCamera;
            camera_ = nullptr;
        }
    }
    catch (const Pylon::GenericException& e) {
        LOG_WARNING(QString("关闭相机时发生异常: %1").arg(e.GetDescription()));
    }

    isOpen_ = false;
    LOG_INFO("Basler相机已关闭");
}

bool BaslerCamera::startGrabbing()
{
    QMutexLocker locker(&mutex_);

    if (!isOpen_ || !camera_) {
        return false;
    }

    try {
        Pylon::CInstantCamera* pCamera = static_cast<Pylon::CInstantCamera*>(camera_);
        pCamera->StartGrabbing(Pylon::GrabStrategy_LatestImageOnly);
        isGrabbing_ = true;
        LOG_INFO("Basler开始采集");
        return true;
    }
    catch (const Pylon::GenericException& e) {
        LOG_ERROR(QString("开始采集失败: %1").arg(e.GetDescription()));
        emit errorOccurred(QString("开始采集失败: %1").arg(e.GetDescription()));
        return false;
    }
}

void BaslerCamera::stopGrabbing()
{
    QMutexLocker locker(&mutex_);

    if (!isGrabbing_ || !camera_) return;

    try {
        Pylon::CInstantCamera* pCamera = static_cast<Pylon::CInstantCamera*>(camera_);
        pCamera->StopGrabbing();
    }
    catch (const Pylon::GenericException& e) {
        LOG_WARNING(QString("停止采集时发生异常: %1").arg(e.GetDescription()));
    }

    isGrabbing_ = false;
    LOG_INFO("Basler停止采集");
}

bool BaslerCamera::trigger()
{
    if (!isOpen_ || !camera_) return false;

    try {
        Pylon::CInstantCamera* pCamera = static_cast<Pylon::CInstantCamera*>(camera_);
        GenApi::INodeMap& nodemap = pCamera->GetNodeMap();
        GenApi::CCommandPtr triggerCmd(nodemap.GetNode("TriggerSoftware"));
        if (triggerCmd.IsValid()) {
            triggerCmd->Execute();
            return true;
        }
    }
    catch (const Pylon::GenericException& e) {
        LOG_ERROR(QString("软件触发失败: %1").arg(e.GetDescription()));
    }

    return false;
}

Base::ImageData::Ptr BaslerCamera::grabImage(int timeoutMs)
{
    QMutexLocker locker(&mutex_);

    if (!isOpen_ || !camera_) {
        return nullptr;
    }

    try {
        Pylon::CInstantCamera* pCamera = static_cast<Pylon::CInstantCamera*>(camera_);
        Pylon::CGrabResultPtr ptrGrabResult;

        if (pCamera->RetrieveResult(timeoutMs, ptrGrabResult, Pylon::TimeoutHandling_Return)) {
            if (ptrGrabResult->GrabSucceeded()) {
                int width = static_cast<int>(ptrGrabResult->GetWidth());
                int height = static_cast<int>(ptrGrabResult->GetHeight());

                // 转换为OpenCV Mat
                cv::Mat image;
                Pylon::EPixelType pixelType = ptrGrabResult->GetPixelType();

                if (Pylon::IsMono(pixelType)) {
                    // 灰度图
                    image = cv::Mat(height, width, CV_8UC1,
                        const_cast<void*>(ptrGrabResult->GetBuffer())).clone();
                }
                else if (pixelType == Pylon::PixelType_BGR8packed) {
                    // BGR格式
                    image = cv::Mat(height, width, CV_8UC3,
                        const_cast<void*>(ptrGrabResult->GetBuffer())).clone();
                }
                else {
                    // 其他格式，使用Pylon格式转换器
                    Pylon::CImageFormatConverter converter;
                    converter.OutputPixelFormat = Pylon::PixelType_BGR8packed;
                    Pylon::CPylonImage pylonImage;
                    converter.Convert(pylonImage, ptrGrabResult);

                    image = cv::Mat(height, width, CV_8UC3,
                        pylonImage.GetBuffer()).clone();
                }

                // 使用内存池分配ImageData，避免内存泄漏
                auto imageData = Base::ImageMemoryPool::instance().allocate(
                    image.cols, image.rows, image.type());
                if (!imageData) {
                    LOG_ERROR("内存池分配失败");
                    return nullptr;
                }
                image.copyTo(imageData->mat());

                emit imageGrabbed(imageData);
                return imageData;
            }
            else {
                LOG_WARNING(QString("采集失败: %1").arg(
                    ptrGrabResult->GetErrorDescription().c_str()));
            }
        }
    }
    catch (const Pylon::GenericException& e) {
        LOG_ERROR(QString("获取图像失败: %1").arg(e.GetDescription()));
    }

    return nullptr;
}

void BaslerCamera::setConfig(const Config& config)
{
    config_ = config;

    if (isOpen_ && camera_) {
        setExposure(config.exposure);
        setGain(config.gain);
        setTriggerMode(config.triggerMode);
        setROI(config.offsetX, config.offsetY, config.width, config.height);
    }
}

void BaslerCamera::setExposure(double exposure)
{
    config_.exposure = exposure;

    if (!isOpen_ || !camera_) return;

    try {
        Pylon::CInstantCamera* pCamera = static_cast<Pylon::CInstantCamera*>(camera_);
        GenApi::INodeMap& nodemap = pCamera->GetNodeMap();
        GenApi::CFloatPtr exposureTime(nodemap.GetNode("ExposureTime"));
        if (exposureTime.IsValid()) {
            exposureTime->SetValue(exposure);
        }
    }
    catch (const Pylon::GenericException& e) {
        LOG_WARNING(QString("设置曝光时间失败: %1").arg(e.GetDescription()));
    }
}

double BaslerCamera::getExposure() const
{
    if (!isOpen_ || !camera_) return config_.exposure;

    try {
        Pylon::CInstantCamera* pCamera = static_cast<Pylon::CInstantCamera*>(camera_);
        GenApi::INodeMap& nodemap = pCamera->GetNodeMap();
        GenApi::CFloatPtr exposureTime(nodemap.GetNode("ExposureTime"));
        if (exposureTime.IsValid()) {
            return exposureTime->GetValue();
        }
    }
    catch (const Pylon::GenericException& e) {
        LOG_DEBUG(QString("获取曝光时间时发生异常，使用配置值: %1").arg(e.GetDescription()));
    }

    return config_.exposure;
}

void BaslerCamera::setGain(double gain)
{
    config_.gain = gain;

    if (!isOpen_ || !camera_) return;

    try {
        Pylon::CInstantCamera* pCamera = static_cast<Pylon::CInstantCamera*>(camera_);
        GenApi::INodeMap& nodemap = pCamera->GetNodeMap();
        GenApi::CFloatPtr gainNode(nodemap.GetNode("Gain"));
        if (gainNode.IsValid()) {
            gainNode->SetValue(gain);
        }
    }
    catch (const Pylon::GenericException& e) {
        LOG_WARNING(QString("设置增益失败: %1").arg(e.GetDescription()));
    }
}

double BaslerCamera::getGain() const
{
    if (!isOpen_ || !camera_) return config_.gain;

    try {
        Pylon::CInstantCamera* pCamera = static_cast<Pylon::CInstantCamera*>(camera_);
        GenApi::INodeMap& nodemap = pCamera->GetNodeMap();
        GenApi::CFloatPtr gainNode(nodemap.GetNode("Gain"));
        if (gainNode.IsValid()) {
            return gainNode->GetValue();
        }
    }
    catch (const Pylon::GenericException& e) {
        LOG_DEBUG(QString("获取增益值时发生异常，使用配置值: %1").arg(e.GetDescription()));
    }

    return config_.gain;
}

void BaslerCamera::setTriggerMode(TriggerMode mode)
{
    config_.triggerMode = mode;

    if (!isOpen_ || !camera_) return;

    try {
        Pylon::CInstantCamera* pCamera = static_cast<Pylon::CInstantCamera*>(camera_);
        GenApi::INodeMap& nodemap = pCamera->GetNodeMap();

        GenApi::CEnumerationPtr triggerMode(nodemap.GetNode("TriggerMode"));
        GenApi::CEnumerationPtr triggerSource(nodemap.GetNode("TriggerSource"));

        if (mode == Continuous) {
            if (triggerMode.IsValid()) {
                triggerMode->FromString("Off");
            }
        }
        else if (mode == Software) {
            if (triggerMode.IsValid()) {
                triggerMode->FromString("On");
            }
            if (triggerSource.IsValid()) {
                triggerSource->FromString("Software");
            }
        }
        else if (mode == Hardware) {
            if (triggerMode.IsValid()) {
                triggerMode->FromString("On");
            }
            if (triggerSource.IsValid()) {
                triggerSource->FromString("Line1");
            }
        }
    }
    catch (const Pylon::GenericException& e) {
        LOG_WARNING(QString("设置触发模式失败: %1").arg(e.GetDescription()));
    }
}

void BaslerCamera::getExposureRange(double& minVal, double& maxVal) const
{
    minVal = 1.0;
    maxVal = 1000000.0;

    if (!isOpen_ || !camera_) return;

    try {
        Pylon::CInstantCamera* pCamera = static_cast<Pylon::CInstantCamera*>(camera_);
        GenApi::INodeMap& nodemap = pCamera->GetNodeMap();
        GenApi::CFloatPtr exposureTime(nodemap.GetNode("ExposureTime"));
        if (exposureTime.IsValid()) {
            minVal = exposureTime->GetMin();
            maxVal = exposureTime->GetMax();
        }
    }
    catch (const Pylon::GenericException& e) {
        LOG_DEBUG(QString("获取曝光范围时发生异常，使用默认范围: %1").arg(e.GetDescription()));
    }
}

void BaslerCamera::getGainRange(double& minVal, double& maxVal) const
{
    minVal = 0.0;
    maxVal = 48.0;

    if (!isOpen_ || !camera_) return;

    try {
        Pylon::CInstantCamera* pCamera = static_cast<Pylon::CInstantCamera*>(camera_);
        GenApi::INodeMap& nodemap = pCamera->GetNodeMap();
        GenApi::CFloatPtr gainNode(nodemap.GetNode("Gain"));
        if (gainNode.IsValid()) {
            minVal = gainNode->GetMin();
            maxVal = gainNode->GetMax();
        }
    }
    catch (const Pylon::GenericException& e) {
        LOG_DEBUG(QString("获取增益范围时发生异常，使用默认范围: %1").arg(e.GetDescription()));
    }
}

void BaslerCamera::getWidthRange(int& minVal, int& maxVal) const
{
    minVal = 16;
    maxVal = 8192;

    if (!isOpen_ || !camera_) return;

    try {
        Pylon::CInstantCamera* pCamera = static_cast<Pylon::CInstantCamera*>(camera_);
        GenApi::INodeMap& nodemap = pCamera->GetNodeMap();
        GenApi::CIntegerPtr widthNode(nodemap.GetNode("Width"));
        if (widthNode.IsValid()) {
            minVal = static_cast<int>(widthNode->GetMin());
            maxVal = static_cast<int>(widthNode->GetMax());
        }
    }
    catch (const Pylon::GenericException& e) {
        LOG_DEBUG(QString("获取宽度范围时发生异常，使用默认范围: %1").arg(e.GetDescription()));
    }
}

void BaslerCamera::getHeightRange(int& minVal, int& maxVal) const
{
    minVal = 16;
    maxVal = 8192;

    if (!isOpen_ || !camera_) return;

    try {
        Pylon::CInstantCamera* pCamera = static_cast<Pylon::CInstantCamera*>(camera_);
        GenApi::INodeMap& nodemap = pCamera->GetNodeMap();
        GenApi::CIntegerPtr heightNode(nodemap.GetNode("Height"));
        if (heightNode.IsValid()) {
            minVal = static_cast<int>(heightNode->GetMin());
            maxVal = static_cast<int>(heightNode->GetMax());
        }
    }
    catch (const Pylon::GenericException& e) {
        LOG_DEBUG(QString("获取高度范围时发生异常，使用默认范围: %1").arg(e.GetDescription()));
    }
}

bool BaslerCamera::setROI(int offsetX, int offsetY, int width, int height)
{
    if (!isOpen_ || !camera_) return false;

    try {
        Pylon::CInstantCamera* pCamera = static_cast<Pylon::CInstantCamera*>(camera_);
        GenApi::INodeMap& nodemap = pCamera->GetNodeMap();

        // 先重置偏移
        GenApi::CIntegerPtr offsetXNode(nodemap.GetNode("OffsetX"));
        GenApi::CIntegerPtr offsetYNode(nodemap.GetNode("OffsetY"));
        if (offsetXNode.IsValid()) offsetXNode->SetValue(0);
        if (offsetYNode.IsValid()) offsetYNode->SetValue(0);

        // 设置宽高
        GenApi::CIntegerPtr widthNode(nodemap.GetNode("Width"));
        GenApi::CIntegerPtr heightNode(nodemap.GetNode("Height"));
        if (widthNode.IsValid()) widthNode->SetValue(width);
        if (heightNode.IsValid()) heightNode->SetValue(height);

        // 设置偏移
        if (offsetXNode.IsValid()) offsetXNode->SetValue(offsetX);
        if (offsetYNode.IsValid()) offsetYNode->SetValue(offsetY);

        config_.width = width;
        config_.height = height;
        config_.offsetX = offsetX;
        config_.offsetY = offsetY;

        return true;
    }
    catch (const Pylon::GenericException& e) {
        LOG_WARNING(QString("设置ROI失败: %1").arg(e.GetDescription()));
        return false;
    }
}

} // namespace HAL
} // namespace VisionForge

#endif // USE_BASLER_PYLON
