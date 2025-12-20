/**
 * @file PlatformConfig.h
 * @brief VisionForge Pro 平台配置管理
 *
 * 本文件定义了平台配置相关的类，用于管理工业对位平台的参数。
 * 设计参考VisionASM的CPlatformInfo系列类和SysPlatformInfo结构。
 *
 * 主要功能：
 * - 平台基本参数配置（行程、方向、脉冲当量等）
 * - 相机平台配置（相机安装位置、运动方式）
 * - 配置的持久化（JSON序列化）
 *
 * @author VisionForge Team
 * @version 1.2.0
 * @date 2025
 */

#ifndef VISIONFORGE_PLATFORM_CONFIG_H
#define VISIONFORGE_PLATFORM_CONFIG_H

#include "PlatformTypes.h"
#include "CoordPosition.h"
#include <QPointF>
#include <QString>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <memory>
#include <vector>

namespace VisionForge {
namespace Platform {

/**
 * @brief 相机平台类型
 *
 * 定义相机相对于运动平台的安装方式
 */
enum class CameraPlatformType {
    Unknown = -1,       ///< 未知
    SeparateFix = 0,    ///< 独立固定安装（最常见）
    SeparateX = 1,      ///< 安装在独立X轴上
    SeparateXY = 2,     ///< 安装在独立XY轴上
    ShareX = 3          ///< 共享平台X轴（龙门结构）
};

/**
 * @brief 平台基本信息类
 *
 * 所有平台类型的基类，定义公共接口。
 * 对应VisionASM的CPlatformInfo。
 */
class PlatformInfo {
public:
    virtual ~PlatformInfo() = default;

    /**
     * @brief 获取平台类型
     */
    virtual PlatformType getType() const = 0;

    /**
     * @brief 克隆配置
     */
    virtual std::unique_ptr<PlatformInfo> clone() const = 0;

    /**
     * @brief 序列化为JSON
     */
    virtual QJsonObject toJson() const = 0;

    /**
     * @brief 从JSON反序列化
     */
    virtual void fromJson(const QJsonObject& json) = 0;
};

/**
 * @brief XYD平台配置类
 *
 * 标准XYD三轴平台的完整配置。
 * 对应VisionASM的CPlatformXYDInfo。
 */
class PlatformXYDInfo : public PlatformInfo {
public:
    PlatformXYDInfo();
    ~PlatformXYDInfo() override = default;

    PlatformType getType() const override { return PlatformType::XYD; }
    std::unique_ptr<PlatformInfo> clone() const override;
    QJsonObject toJson() const override;
    void fromJson(const QJsonObject& json) override;

    // ============ 轴方向配置 ============
    AxisDirectionType xDirection = AxisDirectionType::Positive;     ///< X轴方向
    AxisDirectionType yDirection = AxisDirectionType::Negative;     ///< Y轴方向
    AxisDirectionType dDirection = AxisDirectionType::Negative;     ///< D轴方向

    // ============ D轴驱动配置 ============
    DDriveType dDriveType = DDriveType::Direct;         ///< D轴驱动类型
    DLinearDriveType dLinearType = DLinearDriveType::Fixed; ///< 直线驱动类型
    double rotationLength = 100.0;                      ///< 旋转臂长度(mm)
};

/**
 * @brief 龙门双X平台配置类
 *
 * X1X2Y龙门结构平台配置。
 * 对应VisionASM的CPlatformX1X2YInfo。
 */
class PlatformX1X2YInfo : public PlatformInfo {
public:
    PlatformX1X2YInfo();
    ~PlatformX1X2YInfo() override = default;

    PlatformType getType() const override { return PlatformType::X1X2Y; }
    std::unique_ptr<PlatformInfo> clone() const override;
    QJsonObject toJson() const override;
    void fromJson(const QJsonObject& json) override;

    // ============ 轴方向配置 ============
    AxisDirectionType x1Direction = AxisDirectionType::Positive;
    AxisDirectionType x2Direction = AxisDirectionType::Positive;
    AxisDirectionType yDirection = AxisDirectionType::Positive;

    // ============ UVW平台轴位置（相对于中心） ============
    CoordPosition x1Pos;    ///< X1轴作用点位置
    CoordPosition x2Pos;    ///< X2轴作用点位置
    CoordPosition yPos;     ///< Y轴作用点位置

    // ============ 安装结构类型 ============
    int sliderType = 0;     ///< 0-固定滑块, 1-移动滑块

    // ============ 坐标转换方法 ============
    /**
     * @brief 将X1X2Y轴位置转换为XYD逻辑位置
     */
    void convertToXYD(double x1, double x2, double y,
                      double& x, double& yOut, double& d) const;

    /**
     * @brief 将XYD逻辑位置转换为X1X2Y轴位置
     */
    void convertFromXYD(double x, double y, double d,
                        double& x1, double& x2, double& yOut) const;
};

/**
 * @brief 双Y平台配置类
 *
 * XY1Y2双Y结构平台配置。
 * 对应VisionASM的CPlatformXY1Y2Info。
 */
class PlatformXY1Y2Info : public PlatformInfo {
public:
    PlatformXY1Y2Info();
    ~PlatformXY1Y2Info() override = default;

    PlatformType getType() const override { return PlatformType::XY1Y2; }
    std::unique_ptr<PlatformInfo> clone() const override;
    QJsonObject toJson() const override;
    void fromJson(const QJsonObject& json) override;

    // ============ 轴方向配置 ============
    AxisDirectionType y1Direction = AxisDirectionType::Positive;
    AxisDirectionType y2Direction = AxisDirectionType::Positive;
    AxisDirectionType xDirection = AxisDirectionType::Positive;

    // ============ 轴位置 ============
    CoordPosition y1Pos;
    CoordPosition y2Pos;
    CoordPosition xPos;

    int sliderType = 0;

    // ============ 坐标转换 ============
    void convertToXYD(double y1, double y2, double x,
                      double& xOut, double& y, double& d) const;
    void convertFromXYD(double x, double y, double d,
                        double& y1, double& y2, double& xOut) const;
};

/**
 * @brief DXY平台配置类
 *
 * 旋转优先型平台配置。
 * 对应VisionASM的CPlatformDXYInfo。
 */
class PlatformDXYInfo : public PlatformInfo {
public:
    PlatformDXYInfo();
    ~PlatformDXYInfo() override = default;

    PlatformType getType() const override { return PlatformType::DXY; }
    std::unique_ptr<PlatformInfo> clone() const override;
    QJsonObject toJson() const override;
    void fromJson(const QJsonObject& json) override;

    AxisDirectionType xDirection = AxisDirectionType::Positive;
    AxisDirectionType yDirection = AxisDirectionType::Negative;
    AxisDirectionType dDirection = AxisDirectionType::Negative;

    DDriveType dDriveType = DDriveType::Linear;
    double rotationLength = 100.0;
};

/**
 * @brief 相机平台配置类
 *
 * 配置相机相对于运动平台的安装方式和运动方向。
 * 对应VisionASM的CCamPlatformInfo。
 */
class CameraPlatformInfo {
public:
    CameraPlatformInfo();

    /**
     * @brief 设置相机数量和平台类型
     */
    void setCameraNumAndType(int cameraNum, CameraPlatformType type);

    int getCameraNum() const { return m_cameraNum; }
    CameraPlatformType getCamPlatformType() const { return m_camPlatformType; }

    /**
     * @brief 设置相机X方向
     */
    bool setCamDirectionX(int index, AxisDirectionType direction);

    /**
     * @brief 设置相机Y方向
     */
    bool setCamDirectionY(int index, AxisDirectionType direction);

    /**
     * @brief 获取相机X方向
     */
    AxisDirectionType getCamDirectionX(int index) const;

    /**
     * @brief 获取相机Y方向
     */
    AxisDirectionType getCamDirectionY(int index) const;

    QJsonObject toJson() const;
    void fromJson(const QJsonObject& json);

private:
    int m_cameraNum = 2;
    CameraPlatformType m_camPlatformType = CameraPlatformType::SeparateFix;
    std::vector<AxisDirectionType> m_camDirectX;
    std::vector<AxisDirectionType> m_camDirectY;
    std::vector<AxisDirectionType> m_camAxisDirectX;
    std::vector<AxisDirectionType> m_camAxisDirectY;
};

/**
 * @brief 平台完整配置类
 *
 * 整合所有平台配置信息的顶层类。
 * 对应VisionASM的SysPlatformInfo。
 */
class PlatformConfig {
public:
    PlatformConfig();
    ~PlatformConfig() = default;

    // 拷贝和移动语义
    PlatformConfig(const PlatformConfig& other);
    PlatformConfig(PlatformConfig&& other) noexcept;
    PlatformConfig& operator=(const PlatformConfig& other);
    PlatformConfig& operator=(PlatformConfig&& other) noexcept;

    // ============ 基本信息 ============
    QString name;                   ///< 平台名称
    int platformIndex = 0;          ///< 平台索引

    // ============ 平台类型 ============
    PlatformType type = PlatformType::XYD;  ///< 当前平台类型

    // ============ 平台参数 ============
    double xRange = 1000.0;         ///< X轴行程(mm)
    double yRange = 1000.0;         ///< Y轴行程(mm)
    double dRange = 360.0;          ///< D轴范围(degree)

    // ============ 脉冲当量 ============
    double xPulsePerMM = 1000.0;    ///< X轴每毫米脉冲数
    double yPulsePerMM = 1000.0;    ///< Y轴每毫米脉冲数
    double dPulsePerDegree = 1000.0;///< D轴每度脉冲数

    // ============ 原点配置 ============
    QPointF origin{0.0, 0.0};       ///< 原点偏移

    // ============ 相机配置 ============
    int cameraNum = 2;              ///< 相机数量
    CameraPlatformInfo cameraPlatform;  ///< 相机平台配置

    // ============ 对位配置 ============
    int positionNum = 2;            ///< 对位点数量

    // ============ 详细平台配置 ============

    /**
     * @brief 获取详细平台配置
     * @return 平台配置指针
     */
    const PlatformInfo* getPlatformInfo() const {
        return m_platformInfo.get();
    }

    /**
     * @brief 设置详细平台配置
     * @param info 平台配置
     */
    void setPlatformInfo(std::unique_ptr<PlatformInfo> info) {
        m_platformInfo = std::move(info);
        if (m_platformInfo) {
            type = m_platformInfo->getType();
        }
    }

    /**
     * @brief 根据平台类型创建默认配置
     */
    void createDefaultInfo(PlatformType platType);

    // ============ 序列化 ============

    /**
     * @brief 保存配置到文件
     * @param filePath 文件路径
     * @return 是否成功
     */
    bool save(const QString& filePath) const;

    /**
     * @brief 从文件加载配置
     * @param filePath 文件路径
     * @return 是否成功
     */
    bool load(const QString& filePath);

    /**
     * @brief 转换为JSON对象
     */
    QJsonObject toJson() const;

    /**
     * @brief 从JSON对象加载
     */
    void fromJson(const QJsonObject& json);

    // ============ 坐标转换 ============

    /**
     * @brief 毫米转脉冲
     */
    double mmToPulseX(double mm) const { return mm * xPulsePerMM; }
    double mmToPulseY(double mm) const { return mm * yPulsePerMM; }
    double degreeToPulseD(double degree) const { return degree * dPulsePerDegree; }

    /**
     * @brief 脉冲转毫米
     */
    double pulseToMmX(double pulse) const {
        return xPulsePerMM != 0 ? pulse / xPulsePerMM : 0;
    }
    double pulseToMmY(double pulse) const {
        return yPulsePerMM != 0 ? pulse / yPulsePerMM : 0;
    }
    double pulseToDegreeD(double pulse) const {
        return dPulsePerDegree != 0 ? pulse / dPulsePerDegree : 0;
    }

    /**
     * @brief 检查位置是否在行程范围内
     */
    bool isInRange(const PlatformAxisPos& pos) const;

    /**
     * @brief 限制位置到行程范围内
     */
    PlatformAxisPos clampToRange(const PlatformAxisPos& pos) const;

private:
    std::unique_ptr<PlatformInfo> m_platformInfo;   ///< 详细平台配置

    /**
     * @brief 从JSON创建平台配置对象
     */
    static std::unique_ptr<PlatformInfo> createPlatformInfo(
        PlatformType type, const QJsonObject& json);
};

/**
 * @brief 平台配置管理器
 *
 * 单例类，管理所有平台配置。
 * 对应VisionASM的全局平台管理功能。
 */
class PlatformConfigManager {
public:
    /**
     * @brief 获取单例实例
     */
    static PlatformConfigManager& instance();

    /**
     * @brief 获取当前平台配置
     */
    PlatformConfig& currentConfig();

    /**
     * @brief 获取当前平台配置（只读）
     */
    const PlatformConfig& currentConfig() const;

    /**
     * @brief 获取指定索引的平台配置
     */
    PlatformConfig* getConfig(int index);

    /**
     * @brief 添加平台配置
     */
    int addConfig(const PlatformConfig& config);

    /**
     * @brief 移除平台配置
     */
    bool removeConfig(int index);

    /**
     * @brief 获取平台数量
     */
    int getConfigCount() const;

    /**
     * @brief 设置当前平台索引
     */
    void setCurrentIndex(int index);

    /**
     * @brief 获取当前平台索引
     */
    int getCurrentIndex() const;

    /**
     * @brief 保存所有配置
     */
    bool saveAll(const QString& dirPath);

    /**
     * @brief 加载所有配置
     */
    bool loadAll(const QString& dirPath);

private:
    PlatformConfigManager();
    ~PlatformConfigManager() = default;

    PlatformConfigManager(const PlatformConfigManager&) = delete;
    PlatformConfigManager& operator=(const PlatformConfigManager&) = delete;

    std::vector<PlatformConfig> m_configs;
    int m_currentIndex = 0;
};

} // namespace Platform
} // namespace VisionForge

#endif // VISIONFORGE_PLATFORM_CONFIG_H
