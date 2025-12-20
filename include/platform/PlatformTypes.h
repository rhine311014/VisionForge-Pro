/**
 * @file PlatformTypes.h
 * @brief VisionForge Pro 工业平台类型定义
 *
 * 本文件定义了工业视觉对位系统中的核心平台类型和轴位置结构。
 * 设计参考VisionASM成熟的工业架构，支持13+种常见工业平台配置。
 *
 * 平台类型对应VisionASM的enumPlatformType：
 * - XYD: 标准三轴对位平台
 * - X1X2Y: 龙门双X轴结构（大幅面对位）
 * - XY1Y2: 双Y平台结构
 * - DXY: 旋转优先型平台
 * - 等等
 *
 * @author VisionForge Team
 * @version 1.2.0
 * @date 2025
 */

#ifndef VISIONFORGE_PLATFORM_TYPES_H
#define VISIONFORGE_PLATFORM_TYPES_H

#include <QString>
#include <QJsonObject>
#include <QDebug>
#include <cmath>
#include <array>

namespace VisionForge {
namespace Platform {

/**
 * @brief 工业对位平台类型枚举
 *
 * 定义了工业视觉对位系统中常见的平台类型。
 * 每种类型对应不同的运动轴配置和对位算法。
 *
 * 命名规则：
 * - X/Y: 直线运动轴
 * - D: 旋转轴（Degree）
 * - P: 俯仰轴（Pitch）
 * - 1/2: 表示双轴配置（如X1X2表示双X轴）
 * - 前缀位置表示运动优先级（如DXY表示先转后移）
 */
enum class PlatformType {
    Unknown = -1,       ///< 未知平台类型

    // ============ 标准三轴平台 ============
    XYD = 0,            ///< 标准XYD三轴：X轴+Y轴+旋转D轴

    // ============ 龙门/双轴平台 ============
    X1X2Y = 1,          ///< 龙门双X轴：双X轴+Y轴（适用于大幅面对位）
    XY1Y2 = 2,          ///< 双Y平台：X轴+双Y轴

    // ============ 简化平台 ============
    XY = 3,             ///< 双轴平台：仅X轴+Y轴，无旋转

    // ============ 复杂多轴平台 ============
    YX1X2PD = 4,        ///< Y+双X+俯仰+D轴
    YDPX1X2 = 5,        ///< Y+D+俯仰+双X轴

    // ============ 旋转优先平台 ============
    DXY = 6,            ///< 旋转优先：先D轴旋转，再XY平移

    // ============ 特殊双轴平台 ============
    XD = 7,             ///< X轴+旋转轴
    YD = 8,             ///< Y轴+旋转轴

    // ============ 分离式平台 ============
    XDPY = 9,           ///< XD分离+Y轴（标定时使用）
    XDPY1 = 10,         ///< XD分离+Y1轴变体

    // ============ XY+独立D平台 ============
    XYPD = 11,          ///< XY平移+独立D旋转

    // ============ 单轴平台 ============
    X = 12,             ///< 仅X轴（一维定位）
    Y = 13,             ///< 仅Y轴（一维定位）

    // ============ 扩展平台类型 ============
    XYDP = 14,          ///< 四轴：XYD+俯仰P轴
    UVW = 15,           ///< UVW平台（精密对位）
};

/**
 * @brief 轴方向类型
 *
 * 定义轴的正向方向与应用坐标系的对应关系。
 * 应用坐标系采用标准机器视觉坐标系：X向右、Y向下、顺时针为正。
 */
enum class AxisDirectionType {
    Negative = -1,      ///< 负方向：轴正向与应用坐标系相反
    Positive = 1,       ///< 正方向：轴正向与应用坐标系相同
    None = 0            ///< 无方向：该轴不存在或不参与
};

/**
 * @brief D轴驱动类型
 *
 * 定义旋转轴的驱动方式，影响对位补偿算法。
 */
enum class DDriveType {
    Direct = 0,         ///< 直接驱动：DD马达直驱
    Linear = 1          ///< 直线驱动：通过直线电机+连杆实现旋转
};

/**
 * @brief 直线驱动旋转类型
 *
 * 当D轴采用直线驱动时的连接点类型。
 */
enum class DLinearDriveType {
    Fixed = 0,          ///< 固定连接：旋转臂固定
    Shift = 1           ///< 移动连接：旋转臂可移动
};

/**
 * @brief 平台轴位置结构体
 *
 * 用于表示各类平台的实际轴位置。
 * 设计为通用结构，可适应所有平台类型。
 *
 * 对应VisionASM的CPlatformAxisPos系列类，但采用统一结构简化使用。
 *
 * 使用示例：
 * @code
 * PlatformAxisPos pos;
 * pos.x1 = 100.0;      // 主X轴位置 100mm
 * pos.y1 = 50.0;       // 主Y轴位置 50mm
 * pos.d = 1.5;         // 旋转角度 1.5度
 * @endcode
 */
struct PlatformAxisPos {
    // ============ 主轴位置 ============
    double x1 = 0.0;        ///< 主X轴位置(mm)，对应X或X1
    double x2 = 0.0;        ///< 辅助X轴位置(mm)，用于龙门结构
    double y1 = 0.0;        ///< 主Y轴位置(mm)，对应Y或Y1
    double y2 = 0.0;        ///< 辅助Y轴位置(mm)，用于双Y结构
    double d = 0.0;         ///< 旋转轴角度(degree)
    double p = 0.0;         ///< 俯仰轴角度(degree)

    // ============ 扩展轴位置（UVW平台等） ============
    double u = 0.0;         ///< U轴位置
    double v = 0.0;         ///< V轴位置
    double w = 0.0;         ///< W轴位置

    // ============ 构造函数 ============

    /**
     * @brief 默认构造函数
     */
    PlatformAxisPos() = default;

    /**
     * @brief XYD平台构造函数
     * @param x X轴位置(mm)
     * @param y Y轴位置(mm)
     * @param angle 旋转角度(degree)
     */
    PlatformAxisPos(double x, double y, double angle = 0.0)
        : x1(x), y1(y), d(angle) {}

    /**
     * @brief 完整构造函数
     */
    PlatformAxisPos(double _x1, double _x2, double _y1, double _y2,
                   double _d, double _p = 0.0)
        : x1(_x1), x2(_x2), y1(_y1), y2(_y2), d(_d), p(_p) {}

    // ============ 运算符重载 ============

    /**
     * @brief 相等比较
     */
    bool operator==(const PlatformAxisPos& other) const {
        constexpr double eps = 1e-9;
        return std::abs(x1 - other.x1) < eps &&
               std::abs(x2 - other.x2) < eps &&
               std::abs(y1 - other.y1) < eps &&
               std::abs(y2 - other.y2) < eps &&
               std::abs(d - other.d) < eps &&
               std::abs(p - other.p) < eps;
    }

    bool operator!=(const PlatformAxisPos& other) const {
        return !(*this == other);
    }

    /**
     * @brief 位置加法
     */
    PlatformAxisPos operator+(const PlatformAxisPos& other) const {
        PlatformAxisPos result;
        result.x1 = x1 + other.x1;
        result.x2 = x2 + other.x2;
        result.y1 = y1 + other.y1;
        result.y2 = y2 + other.y2;
        result.d = d + other.d;
        result.p = p + other.p;
        return result;
    }

    /**
     * @brief 位置减法
     */
    PlatformAxisPos operator-(const PlatformAxisPos& other) const {
        PlatformAxisPos result;
        result.x1 = x1 - other.x1;
        result.x2 = x2 - other.x2;
        result.y1 = y1 - other.y1;
        result.y2 = y2 - other.y2;
        result.d = d - other.d;
        result.p = p - other.p;
        return result;
    }

    /**
     * @brief 标量乘法
     */
    PlatformAxisPos operator*(double scalar) const {
        PlatformAxisPos result;
        result.x1 = x1 * scalar;
        result.x2 = x2 * scalar;
        result.y1 = y1 * scalar;
        result.y2 = y2 * scalar;
        result.d = d * scalar;
        result.p = p * scalar;
        return result;
    }

    // ============ 工具方法 ============

    /**
     * @brief 转换为字符串表示
     * @return 格式化的位置字符串
     */
    QString toString() const;

    /**
     * @brief 从字符串解析
     * @param str 位置字符串
     * @return 解析后的轴位置
     */
    static PlatformAxisPos fromString(const QString& str);

    /**
     * @brief 转换为JSON对象
     * @return JSON对象
     */
    QJsonObject toJson() const;

    /**
     * @brief 从JSON对象创建
     * @param json JSON对象
     * @return 轴位置
     */
    static PlatformAxisPos fromJson(const QJsonObject& json);

    /**
     * @brief 检查位置是否为零
     * @param tolerance 容差值
     * @return 是否为零位置
     */
    bool isZero(double tolerance = 1e-6) const {
        return std::abs(x1) < tolerance &&
               std::abs(x2) < tolerance &&
               std::abs(y1) < tolerance &&
               std::abs(y2) < tolerance &&
               std::abs(d) < tolerance &&
               std::abs(p) < tolerance;
    }

    /**
     * @brief 获取主XY位置
     * @return (x1, y1)的简化表示
     */
    std::pair<double, double> getXY() const {
        return {x1, y1};
    }

    /**
     * @brief 设置主XY位置
     */
    void setXY(double x, double y) {
        x1 = x;
        y1 = y;
    }

    /**
     * @brief 归一化角度到[-180, 180]范围
     */
    void normalizeAngles() {
        auto normalize = [](double angle) -> double {
            while (angle > 180.0) angle -= 360.0;
            while (angle < -180.0) angle += 360.0;
            return angle;
        };
        d = normalize(d);
        p = normalize(p);
    }
};

// ============ 平台类型工具函数 ============

/**
 * @brief 获取平台类型名称
 * @param type 平台类型
 * @return 平台类型的中文名称
 */
QString getPlatformTypeName(PlatformType type);

/**
 * @brief 从名称获取平台类型
 * @param name 平台类型名称
 * @return 平台类型枚举值
 */
PlatformType getPlatformTypeFromName(const QString& name);

/**
 * @brief 获取平台类型的轴数量
 * @param type 平台类型
 * @return 轴数量
 */
int getPlatformAxisCount(PlatformType type);

/**
 * @brief 检查平台是否有旋转轴
 * @param type 平台类型
 * @return 是否有D轴
 */
bool platformHasRotation(PlatformType type);

/**
 * @brief 检查平台是否为双X轴结构
 * @param type 平台类型
 * @return 是否为龙门/双X结构
 */
bool platformIsDualX(PlatformType type);

/**
 * @brief 检查平台是否为双Y轴结构
 * @param type 平台类型
 * @return 是否为双Y结构
 */
bool platformIsDualY(PlatformType type);

/**
 * @brief 获取所有支持的平台类型列表
 * @return 平台类型列表
 */
QList<PlatformType> getSupportedPlatformTypes();

} // namespace Platform
} // namespace VisionForge

// 为Qt元对象系统注册枚举（便于信号槽使用）
Q_DECLARE_METATYPE(VisionForge::Platform::PlatformType)
Q_DECLARE_METATYPE(VisionForge::Platform::PlatformAxisPos)

#endif // VISIONFORGE_PLATFORM_TYPES_H
