/**
 * @file BaslerCamera.cpp
 * @brief Basler工业相机驱动实现
 * @details 基于Basler Pylon SDK实现的工业相机驱动。
 *          封装了Pylon SDK的底层操作，提供简洁的相机控制接口。
 *          支持设备枚举、连接管理、参数配置和图像采集等功能。
 * @author VisionForge Team
 * @date 2025-12-17
 * @version 1.0
 *
 * @note 硬件通信协议说明：
 *       1. 设备发现：通过GVCP广播包（端口3956）发现GigE相机
 *       2. 控制通信：通过GenApi节点读写相机寄存器
 *       3. 图像传输：GigE使用GVSP协议，USB3使用USB3 Vision协议
 *
 * @note 依赖说明：
 *       此文件需要Basler Pylon SDK支持，编译时需定义USE_BASLER_PYLON宏
 */

#ifdef USE_BASLER_PYLON

#include "hal/BaslerCamera.h"
#include "base/Logger.h"
#include "base/ImageMemoryPool.h"

// Pylon SDK头文件 - 提供相机控制和图像采集接口
#include <pylon/PylonIncludes.h>

namespace VisionForge {
namespace HAL {

// ============================================================
// 构造函数和析构函数
// ============================================================

/**
 * @brief 构造函数实现
 * @details 初始化成员变量并调用Pylon运行时初始化。
 *          Pylon运行时需要在使用任何Pylon功能前初始化。
 */
BaslerCamera::BaslerCamera(QObject* parent)
    : ICamera(parent)
    , isOpen_(false)
    , isGrabbing_(false)
    , selectedDeviceIndex_(-1)
    , camera_(nullptr)
{
    // 初始化Pylon运行时环境
    // 注意：PylonInitialize()可以多次调用，内部有引用计数
    Pylon::PylonInitialize();
}

/**
 * @brief 析构函数实现
 * @details 确保相机正确关闭并释放Pylon运行时资源
 */
BaslerCamera::~BaslerCamera()
{
    // 确保相机已关闭
    close();

    // 释放Pylon运行时
    // 注意：与PylonInitialize()配对调用
    Pylon::PylonTerminate();
}

// ============================================================
// 静态方法实现
// ============================================================

/**
 * @brief 获取Pylon SDK版本号
 * @details 通过Pylon宏获取版本信息
 */
QString BaslerCamera::sdkVersion()
{
    return QString("Pylon %1.%2.%3")
        .arg(PYLON_VERSION_MAJOR)
        .arg(PYLON_VERSION_MINOR)
        .arg(PYLON_VERSION_SUBMINOR);
}

/**
 * @brief 枚举所有在线Basler设备
 * @details 使用Pylon传输层工厂枚举所有可用的Basler相机。
 *          GigE相机通过网络广播发现，USB相机通过USB总线枚举发现。
 */
QList<BaslerDeviceInfo> BaslerCamera::enumerateDevices()
{
    QList<BaslerDeviceInfo> devices;

    try {
        // 获取传输层工厂单例
        Pylon::CTlFactory& tlFactory = Pylon::CTlFactory::GetInstance();
        Pylon::DeviceInfoList_t deviceList;

        // 枚举所有设备，返回发现的设备数量
        if (tlFactory.EnumerateDevices(deviceList) == 0) {
            LOG_INFO("未发现Basler相机");
            return devices;
        }

        LOG_INFO(QString("发现 %1 个Basler相机").arg(deviceList.size()));

        // 遍历设备列表，提取设备信息
        for (size_t i = 0; i < deviceList.size(); i++) {
            const Pylon::CDeviceInfo& devInfo = deviceList[i];

            BaslerDeviceInfo info;
            info.modelName = QString::fromStdString(devInfo.GetModelName().c_str());
            info.serialNumber = QString::fromStdString(devInfo.GetSerialNumber().c_str());
            info.vendorName = QString::fromStdString(devInfo.GetVendorName().c_str());

            // 根据设备类名判断接口类型
            QString deviceClass = QString::fromStdString(devInfo.GetDeviceClass().c_str());
            if (deviceClass.contains("GigE", Qt::CaseInsensitive)) {
                info.interfaceType = "GigE";
                // GigE相机有IP地址
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
        // Pylon SDK异常处理
        LOG_ERROR(QString("枚举Basler设备失败: %1").arg(e.GetDescription()));
    }

    return devices;
}

// ============================================================
// 设备选择方法
// ============================================================

/**
 * @brief 通过索引选择设备
 */
void BaslerCamera::selectDevice(int index)
{
    selectedDeviceIndex_ = index;
    LOG_INFO(QString("已选择Basler设备索引: %1").arg(index));
}

/**
 * @brief 通过序列号选择设备
 * @details 遍历所有设备，匹配序列号
 */
bool BaslerCamera::selectBySerialNumber(const QString& serialNumber)
{
    try {
        Pylon::CTlFactory& tlFactory = Pylon::CTlFactory::GetInstance();
        Pylon::DeviceInfoList_t deviceList;
        tlFactory.EnumerateDevices(deviceList);

        // 遍历查找匹配的设备
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

/**
 * @brief 通过IP地址选择设备
 * @details 遍历所有设备，匹配IP地址（仅GigE相机）
 */
bool BaslerCamera::selectByIP(const QString& ipAddress)
{
    try {
        Pylon::CTlFactory& tlFactory = Pylon::CTlFactory::GetInstance();
        Pylon::DeviceInfoList_t deviceList;
        tlFactory.EnumerateDevices(deviceList);

        // 遍历查找匹配的设备
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

// ============================================================
// 设备管理方法
// ============================================================

/**
 * @brief 打开相机设备
 * @details 创建Pylon相机对象，建立与相机的连接。
 *          连接成功后读取相机基本参数。
 */
bool BaslerCamera::open()
{
    QMutexLocker locker(&mutex_);

    if (isOpen_) {
        return true;  // 已打开，直接返回成功
    }

    try {
        Pylon::CTlFactory& tlFactory = Pylon::CTlFactory::GetInstance();
        Pylon::DeviceInfoList_t deviceList;

        // 检查是否有可用设备
        if (tlFactory.EnumerateDevices(deviceList) == 0) {
            LOG_ERROR("没有找到Basler相机");
            emit errorOccurred("没有找到Basler相机");
            return false;
        }

        // 如果未选择设备，默认选择第一个
        if (selectedDeviceIndex_ < 0) {
            selectedDeviceIndex_ = 0;
        }

        // 检查索引范围
        if (selectedDeviceIndex_ >= static_cast<int>(deviceList.size())) {
            LOG_ERROR("设备索引超出范围");
            emit errorOccurred("设备索引超出范围");
            return false;
        }

        // 创建Pylon相机对象
        // CInstantCamera是Pylon SDK的高级相机类，封装了常用操作
        Pylon::CInstantCamera* pCamera = new Pylon::CInstantCamera(
            tlFactory.CreateDevice(deviceList[selectedDeviceIndex_]));
        camera_ = pCamera;

        // 打开相机连接（建立GVCP连接）
        pCamera->Open();

        // 从设备读取信息
        deviceInfo_.modelName = QString::fromStdString(
            pCamera->GetDeviceInfo().GetModelName().c_str());
        deviceInfo_.serialNumber = QString::fromStdString(
            pCamera->GetDeviceInfo().GetSerialNumber().c_str());

        // 获取当前图像参数
        // 通过GenApi访问相机的参数节点
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

/**
 * @brief 关闭相机设备
 * @details 停止采集、关闭连接、释放资源
 */
void BaslerCamera::close()
{
    QMutexLocker locker(&mutex_);

    if (!isOpen_) return;

    // 如果正在采集，先停止
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

// ============================================================
// 采集控制方法
// ============================================================

/**
 * @brief 开始图像采集
 * @details 使用LatestImageOnly策略，确保始终获取最新图像帧
 */
bool BaslerCamera::startGrabbing()
{
    QMutexLocker locker(&mutex_);

    if (!isOpen_ || !camera_) {
        return false;
    }

    try {
        Pylon::CInstantCamera* pCamera = static_cast<Pylon::CInstantCamera*>(camera_);
        // 使用LatestImageOnly策略：只保留最新帧，丢弃旧帧
        // 适用于实时视觉检测场景
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

/**
 * @brief 停止图像采集
 */
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

/**
 * @brief 发送软件触发命令
 * @details 执行TriggerSoftware命令节点触发一帧采集
 */
bool BaslerCamera::trigger()
{
    if (!isOpen_ || !camera_) return false;

    try {
        Pylon::CInstantCamera* pCamera = static_cast<Pylon::CInstantCamera*>(camera_);
        GenApi::INodeMap& nodemap = pCamera->GetNodeMap();

        // 获取并执行TriggerSoftware命令节点
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

/**
 * @brief 采集一帧图像
 * @details 等待并获取图像帧，自动进行像素格式转换。
 *          支持灰度、BGR和Bayer格式的相机。
 */
Base::ImageData::Ptr BaslerCamera::grabImage(int timeoutMs)
{
    QMutexLocker locker(&mutex_);

    if (!isOpen_ || !camera_) {
        return nullptr;
    }

    try {
        Pylon::CInstantCamera* pCamera = static_cast<Pylon::CInstantCamera*>(camera_);
        Pylon::CGrabResultPtr ptrGrabResult;

        // 等待图像帧，TimeoutHandling_Return表示超时返回而不抛异常
        if (pCamera->RetrieveResult(timeoutMs, ptrGrabResult, Pylon::TimeoutHandling_Return)) {
            if (ptrGrabResult->GrabSucceeded()) {
                int width = static_cast<int>(ptrGrabResult->GetWidth());
                int height = static_cast<int>(ptrGrabResult->GetHeight());

                // 转换为OpenCV Mat格式
                cv::Mat image;
                Pylon::EPixelType pixelType = ptrGrabResult->GetPixelType();

                if (Pylon::IsMono(pixelType)) {
                    // 灰度图像：直接复制
                    image = cv::Mat(height, width, CV_8UC1,
                        const_cast<void*>(ptrGrabResult->GetBuffer())).clone();
                }
                else if (pixelType == Pylon::PixelType_BGR8packed) {
                    // BGR格式：直接复制
                    image = cv::Mat(height, width, CV_8UC3,
                        const_cast<void*>(ptrGrabResult->GetBuffer())).clone();
                }
                else {
                    // 其他格式（如Bayer）：使用Pylon格式转换器转换为BGR
                    Pylon::CImageFormatConverter converter;
                    converter.OutputPixelFormat = Pylon::PixelType_BGR8packed;
                    Pylon::CPylonImage pylonImage;
                    converter.Convert(pylonImage, ptrGrabResult);

                    image = cv::Mat(height, width, CV_8UC3,
                        pylonImage.GetBuffer()).clone();
                }

                // 使用内存池分配ImageData，提高内存使用效率
                auto imageData = Base::ImageMemoryPool::instance().allocate(
                    image.cols, image.rows, image.type());
                if (!imageData) {
                    LOG_ERROR("内存池分配失败");
                    return nullptr;
                }
                image.copyTo(imageData->mat());

                // 发送图像采集完成信号
                emit imageGrabbed(imageData);
                return imageData;
            }
            else {
                // 采集失败，输出错误信息
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

// ============================================================
// 参数配置方法
// ============================================================

/**
 * @brief 设置相机配置
 * @details 批量应用相机参数到硬件
 */
void BaslerCamera::setConfig(const Config& config)
{
    config_ = config;

    // 如果相机已打开，立即应用参数
    if (isOpen_ && camera_) {
        setExposure(config.exposure);
        setGain(config.gain);
        setTriggerMode(config.triggerMode);
        setROI(config.offsetX, config.offsetY, config.width, config.height);
    }
}

/**
 * @brief 设置曝光时间
 * @details 通过GenApi写入ExposureTime节点
 */
void BaslerCamera::setExposure(double exposure)
{
    config_.exposure = exposure;

    if (!isOpen_ || !camera_) return;

    try {
        Pylon::CInstantCamera* pCamera = static_cast<Pylon::CInstantCamera*>(camera_);
        GenApi::INodeMap& nodemap = pCamera->GetNodeMap();

        // 获取并设置ExposureTime节点
        GenApi::CFloatPtr exposureTime(nodemap.GetNode("ExposureTime"));
        if (exposureTime.IsValid()) {
            exposureTime->SetValue(exposure);
        }
    }
    catch (const Pylon::GenericException& e) {
        LOG_WARNING(QString("设置曝光时间失败: %1").arg(e.GetDescription()));
    }
}

/**
 * @brief 获取当前曝光时间
 * @details 从硬件读取实际曝光值
 */
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

/**
 * @brief 设置增益
 * @details 通过GenApi写入Gain节点
 */
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

/**
 * @brief 获取当前增益
 * @details 从硬件读取实际增益值
 */
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

/**
 * @brief 设置触发模式
 * @details 配置TriggerMode和TriggerSource节点
 */
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
            // 连续模式：关闭触发
            if (triggerMode.IsValid()) {
                triggerMode->FromString("Off");
            }
        }
        else if (mode == Software) {
            // 软件触发模式
            if (triggerMode.IsValid()) {
                triggerMode->FromString("On");
            }
            if (triggerSource.IsValid()) {
                triggerSource->FromString("Software");
            }
        }
        else if (mode == Hardware) {
            // 硬件触发模式：使用Line1作为触发源
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

// ============================================================
// Basler特有功能方法
// ============================================================

/**
 * @brief 获取曝光时间范围
 * @details 从ExposureTime节点读取Min/Max属性
 */
void BaslerCamera::getExposureRange(double& minVal, double& maxVal) const
{
    // 设置默认值
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

/**
 * @brief 获取增益范围
 * @details 从Gain节点读取Min/Max属性
 */
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

/**
 * @brief 获取图像宽度范围
 * @details 从Width节点读取Min/Max属性
 */
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

/**
 * @brief 获取图像高度范围
 * @details 从Height节点读取Min/Max属性
 */
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

/**
 * @brief 设置ROI区域
 * @details 按照正确顺序设置ROI：先重置偏移，再设置宽高，最后设置偏移。
 *          这个顺序是必要的，因为偏移值有依赖于当前宽高的约束。
 */
bool BaslerCamera::setROI(int offsetX, int offsetY, int width, int height)
{
    if (!isOpen_ || !camera_) return false;

    try {
        Pylon::CInstantCamera* pCamera = static_cast<Pylon::CInstantCamera*>(camera_);
        GenApi::INodeMap& nodemap = pCamera->GetNodeMap();

        // 第一步：重置偏移为0，确保可以设置更大的宽高
        GenApi::CIntegerPtr offsetXNode(nodemap.GetNode("OffsetX"));
        GenApi::CIntegerPtr offsetYNode(nodemap.GetNode("OffsetY"));
        if (offsetXNode.IsValid()) offsetXNode->SetValue(0);
        if (offsetYNode.IsValid()) offsetYNode->SetValue(0);

        // 第二步：设置宽度和高度
        GenApi::CIntegerPtr widthNode(nodemap.GetNode("Width"));
        GenApi::CIntegerPtr heightNode(nodemap.GetNode("Height"));
        if (widthNode.IsValid()) widthNode->SetValue(width);
        if (heightNode.IsValid()) heightNode->SetValue(height);

        // 第三步：设置偏移
        if (offsetXNode.IsValid()) offsetXNode->SetValue(offsetX);
        if (offsetYNode.IsValid()) offsetYNode->SetValue(offsetY);

        // 更新配置
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
