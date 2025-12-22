/**
 * @file CameraFactory.cpp
 * @brief 相机工厂类实现
 * @author VisionForge Team
 * @date 2025-12-17
 */

#include "hal/CameraFactory.h"
#include "hal/SimulatedCamera.h"
#include "base/Logger.h"

#ifdef USE_HIKVISION_MVS
#include "hal/HikvisionCamera.h"
#endif

#ifdef USE_BASLER_PYLON
#include "hal/BaslerCamera.h"
#endif

#ifdef USE_GENTL
#include "hal/GenTLCamera.h"
#endif

namespace VisionForge {
namespace HAL {

ICamera* CameraFactory::create(CameraType type, QObject* parent)
{
    switch (type) {
    case Simulated:
        LOG_INFO("创建模拟相机");
        return new SimulatedCamera(parent);

#ifdef USE_HIKVISION_MVS
    case Hikvision:
        LOG_INFO("创建海康威视相机");
        return new HikvisionCamera(parent);
#endif

#ifdef USE_BASLER_PYLON
    case Basler:
        LOG_INFO("创建Basler相机");
        return new BaslerCamera(parent);
#endif

#ifdef USE_GENTL
    case GenTL:
        LOG_INFO("创建GenTL通用相机");
        return new GenTLCamera(parent);
#endif

    default:
        LOG_WARNING(QString("不支持的相机类型: %1").arg(static_cast<int>(type)));
        return nullptr;
    }
}

ICamera* CameraFactory::create(const QString& typeName, QObject* parent)
{
    return create(typeFromName(typeName), parent);
}

QList<GenericDeviceInfo> CameraFactory::enumerateAllDevices()
{
    QList<GenericDeviceInfo> allDevices;

    // 添加模拟相机
    GenericDeviceInfo simInfo;
    simInfo.manufacturer = "VisionForge";
    simInfo.modelName = "SimulatedCamera";
    simInfo.serialNumber = "SIM-000001";
    simInfo.interfaceType = "Simulated";
    simInfo.cameraType = Simulated;
    allDevices.append(simInfo);

#ifdef USE_HIKVISION_MVS
    // 枚举海康相机
    auto hikDevices = enumerateDevices(Hikvision);
    allDevices.append(hikDevices);
#endif

#ifdef USE_BASLER_PYLON
    // 枚举Basler相机
    auto baslerDevices = enumerateDevices(Basler);
    allDevices.append(baslerDevices);
#endif

#ifdef USE_GENTL
    // 枚举GenTL通用相机
    auto gentlDevices = enumerateDevices(GenTL);
    allDevices.append(gentlDevices);
#endif

    return allDevices;
}

QList<GenericDeviceInfo> CameraFactory::enumerateDevices(CameraType type)
{
    QList<GenericDeviceInfo> devices;

    switch (type) {
    case Simulated: {
        GenericDeviceInfo info;
        info.manufacturer = "VisionForge";
        info.modelName = "SimulatedCamera";
        info.serialNumber = "SIM-000001";
        info.interfaceType = "Simulated";
        info.cameraType = Simulated;
        devices.append(info);
        break;
    }

#ifdef USE_HIKVISION_MVS
    case Hikvision: {
        auto hikDevices = HikvisionCamera::enumerateDevices();
        for (const auto& hikInfo : hikDevices) {
            GenericDeviceInfo info;
            info.manufacturer = hikInfo.manufacturer.isEmpty() ? "Hikvision" : hikInfo.manufacturer;
            info.modelName = hikInfo.modelName;
            info.serialNumber = hikInfo.serialNumber;
            info.ipAddress = hikInfo.ipAddress;
            info.interfaceType = hikInfo.deviceType == 1 ? "GigE" : "USB3";
            info.cameraType = Hikvision;
            devices.append(info);
        }
        break;
    }
#endif

#ifdef USE_BASLER_PYLON
    case Basler: {
        auto baslerDevices = BaslerCamera::enumerateDevices();
        for (const auto& baslerInfo : baslerDevices) {
            GenericDeviceInfo info;
            info.manufacturer = "Basler";
            info.modelName = baslerInfo.modelName;
            info.serialNumber = baslerInfo.serialNumber;
            info.ipAddress = baslerInfo.ipAddress;
            info.interfaceType = baslerInfo.interfaceType;
            info.cameraType = Basler;
            devices.append(info);
        }
        break;
    }
#endif

#ifdef USE_GENTL
    case GenTL: {
        auto gentlDevices = GenTLCamera::enumerateDevices();
        for (const auto& gentlInfo : gentlDevices) {
            GenericDeviceInfo info;
            info.manufacturer = gentlInfo.vendorName;
            info.modelName = gentlInfo.modelName;
            info.serialNumber = gentlInfo.serialNumber;
            info.ipAddress = gentlInfo.ipAddress;
            info.interfaceType = gentlInfo.tlType;
            info.cameraType = GenTL;
            devices.append(info);
        }
        break;
    }
#endif

    default:
        break;
    }

    return devices;
}

QStringList CameraFactory::supportedTypes()
{
    QStringList types;
    types << "Simulated";

#ifdef USE_HIKVISION_MVS
    types << "Hikvision";
#endif

#ifdef USE_BASLER_PYLON
    types << "Basler";
#endif

#ifdef USE_GENTL
    types << "GenTL";
#endif

    return types;
}

bool CameraFactory::isTypeSupported(CameraType type)
{
    switch (type) {
    case Simulated:
        return true;

#ifdef USE_HIKVISION_MVS
    case Hikvision:
        return true;
#endif

#ifdef USE_BASLER_PYLON
    case Basler:
        return true;
#endif

#ifdef USE_GENTL
    case GenTL:
        return true;
#endif

    default:
        return false;
    }
}

QString CameraFactory::typeName(CameraType type)
{
    switch (type) {
    case Simulated:
        return "Simulated";
    case Hikvision:
        return "Hikvision";
    case Basler:
        return "Basler";
    case GenTL:
        return "GenTL";
    default:
        return "Unknown";
    }
}

CameraFactory::CameraType CameraFactory::typeFromName(const QString& name)
{
    QString lowerName = name.toLower();

    if (lowerName == "simulated" || lowerName == "sim") {
        return Simulated;
    }
    if (lowerName == "hikvision" || lowerName == "hik" || lowerName == "hikrobot") {
        return Hikvision;
    }
    if (lowerName == "basler" || lowerName == "pylon") {
        return Basler;
    }
    if (lowerName == "gentl" || lowerName == "genicam" || lowerName == "generic") {
        return GenTL;
    }

    return Simulated;  // 默认返回模拟相机
}

} // namespace HAL
} // namespace VisionForge
