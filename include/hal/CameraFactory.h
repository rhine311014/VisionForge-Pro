/**
 * @file CameraFactory.h
 * @brief 相机工厂类
 * @author VisionForge Team
 * @date 2025-12-17
 */

#pragma once

#include "hal/ICamera.h"
#include <QString>
#include <QList>

namespace VisionForge {
namespace HAL {

/**
 * @brief 通用相机设备信息
 */
struct GenericDeviceInfo {
    QString manufacturer;     // 厂商名称
    QString modelName;        // 型号
    QString serialNumber;     // 序列号
    QString ipAddress;        // IP地址 (网络相机)
    QString interfaceType;    // 接口类型 (GigE/USB3/Simulated等)
    int cameraType;           // 相机类型枚举值

    GenericDeviceInfo()
        : cameraType(0)
    {}
};

/**
 * @brief 相机工厂类
 *
 * 提供统一的相机创建和枚举接口
 */
class CameraFactory {
public:
    /**
     * @brief 相机类型
     */
    enum CameraType {
        Simulated = 0,    // 模拟相机
        Hikvision = 1,    // 海康威视
        Basler = 2,       // Basler
        GenTL = 3,        // GenTL通用相机（支持所有GenICam标准相机）
        // 可扩展其他厂商...
    };

    /**
     * @brief 创建相机实例
     * @param type 相机类型
     * @param parent 父对象
     * @return 相机指针，失败返回nullptr
     */
    static ICamera* create(CameraType type, QObject* parent = nullptr);

    /**
     * @brief 通过类型名称创建相机
     * @param typeName 类型名称 ("Simulated", "Hikvision", "Basler")
     * @param parent 父对象
     * @return 相机指针
     */
    static ICamera* create(const QString& typeName, QObject* parent = nullptr);

    /**
     * @brief 枚举所有可用相机
     * @return 设备信息列表
     */
    static QList<GenericDeviceInfo> enumerateAllDevices();

    /**
     * @brief 枚举指定类型的相机
     * @param type 相机类型
     * @return 设备信息列表
     */
    static QList<GenericDeviceInfo> enumerateDevices(CameraType type);

    /**
     * @brief 获取支持的相机类型列表
     * @return 类型名称列表
     */
    static QStringList supportedTypes();

    /**
     * @brief 检查是否支持指定类型
     * @param type 相机类型
     * @return true如果支持
     */
    static bool isTypeSupported(CameraType type);

    /**
     * @brief 获取类型名称
     * @param type 相机类型
     * @return 类型名称
     */
    static QString typeName(CameraType type);

    /**
     * @brief 从名称获取类型
     * @param name 类型名称
     * @return 相机类型
     */
    static CameraType typeFromName(const QString& name);

private:
    CameraFactory() = delete;
};

} // namespace HAL
} // namespace VisionForge
