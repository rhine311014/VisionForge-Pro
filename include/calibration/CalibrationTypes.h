/**
 * @file CalibrationTypes.h
 * @brief VisionForge Pro 标定系统类型定义
 *
 * 本文件定义了视觉标定系统中的核心数据类型，包括：
 * - 标定类型枚举（5点标定、9点标定、DM码标定等）
 * - 标定点数据结构（像素坐标与世界坐标对）
 * - 标定结果结构（H矩阵、误差统计）
 * - 标定状态和错误码
 *
 * 设计参考VisionASM的标定模块架构，使用Eigen进行矩阵运算。
 *
 * @author VisionForge Team
 * @version 1.2.0
 * @date 2025
 */

#ifndef VISIONFORGE_CALIBRATION_TYPES_H
#define VISIONFORGE_CALIBRATION_TYPES_H

#include <QString>
#include <QPointF>
#include <QJsonObject>
#include <QJsonArray>
#include <QDateTime>
#include <vector>
#include <array>
#include <cmath>
#include <limits>

// Eigen矩阵库
#include <Eigen/Core>
#include <Eigen/Dense>

namespace VisionForge {
namespace Calibration {

// ============================================================
// 类型别名
// ============================================================

/// 3x3齐次变换矩阵
using Matrix3d = Eigen::Matrix3d;

/// 2x3仿射变换矩阵
using Matrix23d = Eigen::Matrix<double, 2, 3>;

/// 3D向量
using Vector3d = Eigen::Vector3d;

/// 2D向量
using Vector2d = Eigen::Vector2d;

/// 动态大小矩阵
using MatrixXd = Eigen::MatrixXd;

/// 动态大小向量
using VectorXd = Eigen::VectorXd;

// ============================================================
// 标定类型枚举
// ============================================================

/**
 * @brief 标定方法类型
 *
 * 定义了支持的各种标定方法，适用于不同精度和场景需求。
 */
enum class CalibrationType {
    None = 0,           ///< 无标定

    // ============ 点标定 ============
    Point3 = 3,         ///< 3点标定：仿射变换（平移+旋转+缩放）
    Point4 = 4,         ///< 4点标定：透视变换基础
    Point5 = 5,         ///< 5点标定：过约束仿射变换
    Point9 = 9,         ///< 9点标定：标准工业精度
    Point16 = 16,       ///< 16点标定：高精度标定
    Point25 = 25,       ///< 25点标定：超高精度标定

    // ============ 特殊标定 ============
    DMCode = 100,       ///< DM码标定：使用DataMatrix码自动标定
    Chessboard = 101,   ///< 棋盘格标定：适用于相机内参标定
    Circle = 102,       ///< 圆点标定：使用圆点阵列标定
    CrossMark = 103,    ///< 十字标记标定

    // ============ 高级标定 ============
    NonLinear = 200,    ///< 非线性标定：使用LM优化
    Polynomial = 201,   ///< 多项式标定：高阶畸变校正
};

/**
 * @brief 标定状态
 */
enum class CalibrationStatus {
    NotCalibrated = 0,  ///< 未标定
    Calibrating = 1,    ///< 标定中
    Calibrated = 2,     ///< 已标定
    Failed = 3,         ///< 标定失败
    Expired = 4,        ///< 标定过期
};

/**
 * @brief 标定错误码
 */
enum class CalibrationError {
    Success = 0,                ///< 成功
    InsufficientPoints = 1,     ///< 点数不足
    SingularMatrix = 2,         ///< 奇异矩阵（点共线等）
    LargeError = 3,             ///< 误差过大
    InvalidInput = 4,           ///< 无效输入
    ComputationFailed = 5,      ///< 计算失败
    FileIOError = 6,            ///< 文件读写错误
    NotCalibrated = 7,          ///< 未标定
};

/**
 * @brief 对齐类型
 */
enum class AlignmentType {
    None = 0,           ///< 无对齐
    SinglePoint = 1,    ///< 单点对齐：仅平移
    TwoPoint = 2,       ///< 两点对齐：平移+旋转
    ThreePoint = 3,     ///< 三点对齐：仿射变换
    FourPoint = 4,      ///< 四点对齐：透视变换
};

// ============================================================
// 标定点结构
// ============================================================

/**
 * @brief 标定点数据
 *
 * 存储一对标定点的像素坐标和世界坐标。
 * 这是标定系统的基本数据单元。
 */
struct CalibrationPoint {
    // ============ 坐标数据 ============
    QPointF pixelPos;       ///< 像素坐标（图像坐标系）
    QPointF worldPos;       ///< 世界坐标（平台坐标系，单位：mm）

    // ============ 索引和状态 ============
    int index = -1;         ///< 点索引（-1表示未设置）
    bool isValid = false;   ///< 是否有效
    bool isEnabled = true;  ///< 是否启用（用于排除异常点）

    // ============ 误差信息 ============
    double error = 0.0;     ///< 重投影误差（像素）
    double weight = 1.0;    ///< 权重（用于加权最小二乘）

    // ============ 构造函数 ============

    /**
     * @brief 默认构造函数
     */
    CalibrationPoint() = default;

    /**
     * @brief 完整构造函数
     * @param pixel 像素坐标
     * @param world 世界坐标
     * @param idx 点索引
     */
    CalibrationPoint(const QPointF& pixel, const QPointF& world, int idx = -1)
        : pixelPos(pixel), worldPos(world), index(idx), isValid(true) {}

    /**
     * @brief 坐标构造函数
     */
    CalibrationPoint(double px, double py, double wx, double wy, int idx = -1)
        : pixelPos(px, py), worldPos(wx, wy), index(idx), isValid(true) {}

    // ============ 运算符 ============

    bool operator==(const CalibrationPoint& other) const {
        return pixelPos == other.pixelPos && worldPos == other.worldPos;
    }

    bool operator!=(const CalibrationPoint& other) const {
        return !(*this == other);
    }

    // ============ 序列化 ============

    /**
     * @brief 转换为JSON对象
     */
    QJsonObject toJson() const;

    /**
     * @brief 从JSON对象创建
     */
    static CalibrationPoint fromJson(const QJsonObject& json);

    /**
     * @brief 转换为字符串
     */
    QString toString() const;
};

/**
 * @brief 标定点集合
 *
 * 管理一组标定点，提供增删改查和验证功能。
 */
class CalibrationPointSet {
public:
    // ============ 构造函数 ============
    CalibrationPointSet() = default;
    explicit CalibrationPointSet(int expectedCount);

    // ============ 点管理 ============

    /**
     * @brief 添加标定点
     * @param point 标定点
     * @return 添加后的索引
     */
    int addPoint(const CalibrationPoint& point);

    /**
     * @brief 设置指定索引的标定点
     * @param index 索引
     * @param point 标定点
     * @return 是否成功
     */
    bool setPoint(int index, const CalibrationPoint& point);

    /**
     * @brief 获取指定索引的标定点
     * @param index 索引
     * @return 标定点指针，不存在返回nullptr
     */
    CalibrationPoint* getPoint(int index);
    const CalibrationPoint* getPoint(int index) const;

    /**
     * @brief 移除指定索引的标定点
     */
    bool removePoint(int index);

    /**
     * @brief 清空所有点
     */
    void clear();

    // ============ 查询 ============

    /**
     * @brief 获取点数量
     */
    int count() const { return static_cast<int>(m_points.size()); }

    /**
     * @brief 获取有效点数量
     */
    int validCount() const;

    /**
     * @brief 获取启用的点数量
     */
    int enabledCount() const;

    /**
     * @brief 是否为空
     */
    bool isEmpty() const { return m_points.empty(); }

    /**
     * @brief 获取所有点
     */
    const std::vector<CalibrationPoint>& points() const { return m_points; }
    std::vector<CalibrationPoint>& points() { return m_points; }

    /**
     * @brief 获取有效且启用的点
     */
    std::vector<CalibrationPoint> getValidEnabledPoints() const;

    // ============ 验证 ============

    /**
     * @brief 检查点数量是否满足标定要求
     * @param type 标定类型
     * @return 是否满足
     */
    bool hasEnoughPoints(CalibrationType type) const;

    /**
     * @brief 检查点是否共线
     * @param tolerance 容差（弧度）
     * @return 是否共线
     */
    bool arePointsCollinear(double tolerance = 0.01) const;

    /**
     * @brief 计算点分布质量分数
     * @return 0-1之间的分数，越高越好
     */
    double calculateDistributionScore() const;

    // ============ 序列化 ============

    QJsonArray toJson() const;
    static CalibrationPointSet fromJson(const QJsonArray& json);

private:
    std::vector<CalibrationPoint> m_points;
    int m_expectedCount = 0;
};

// ============================================================
// 标定结果结构
// ============================================================

/**
 * @brief 误差统计信息
 */
struct ErrorStatistics {
    double meanError = 0.0;     ///< 平均误差（像素）
    double maxError = 0.0;      ///< 最大误差（像素）
    double minError = 0.0;      ///< 最小误差（像素）
    double rmsError = 0.0;      ///< 均方根误差（像素）
    double stdDev = 0.0;        ///< 标准差（像素）

    int worstPointIndex = -1;   ///< 最大误差点索引
    int pointCount = 0;         ///< 参与计算的点数

    /**
     * @brief 检查误差是否在可接受范围内
     * @param maxAllowedRMS 允许的最大RMS误差
     */
    bool isAcceptable(double maxAllowedRMS = 1.0) const {
        return rmsError <= maxAllowedRMS;
    }

    QJsonObject toJson() const;
    static ErrorStatistics fromJson(const QJsonObject& json);
    QString toString() const;
};

/**
 * @brief 标定结果
 *
 * 存储完整的标定结果，包括变换矩阵和误差统计。
 */
struct CalibrationResult {
    // ============ 标定状态 ============
    CalibrationStatus status = CalibrationStatus::NotCalibrated;
    CalibrationError error = CalibrationError::NotCalibrated;
    CalibrationType type = CalibrationType::None;

    // ============ 变换矩阵 ============

    /**
     * @brief 3x3齐次变换矩阵（像素->世界）
     *
     * 矩阵形式：
     * | h00 h01 h02 |   | px |   | wx |
     * | h10 h11 h12 | * | py | = | wy |
     * | h20 h21 h22 |   | 1  |   | w  |
     *
     * 世界坐标 = (wx/w, wy/w)
     */
    Matrix3d homography = Matrix3d::Identity();

    /**
     * @brief 逆变换矩阵（世界->像素）
     */
    Matrix3d inverseHomography = Matrix3d::Identity();

    /**
     * @brief 2x3仿射变换矩阵（仅用于仿射标定）
     */
    Matrix23d affineMatrix = Matrix23d::Zero();

    // ============ 缩放因子 ============
    double scaleX = 1.0;        ///< X方向缩放（mm/pixel）
    double scaleY = 1.0;        ///< Y方向缩放（mm/pixel）
    double rotation = 0.0;      ///< 旋转角度（度）

    // ============ 误差统计 ============
    ErrorStatistics errorStats;
    std::vector<double> pointErrors;    ///< 每个点的重投影误差

    // ============ 标定信息 ============
    QDateTime calibrationTime;          ///< 标定时间
    int pointCount = 0;                 ///< 使用的标定点数
    QString description;                ///< 标定描述

    // ============ 构造函数 ============
    CalibrationResult() {
        affineMatrix << 1, 0, 0,
                        0, 1, 0;
    }

    // ============ 坐标变换方法 ============

    /**
     * @brief 像素坐标转世界坐标
     * @param pixelPos 像素坐标
     * @return 世界坐标
     */
    QPointF pixelToWorld(const QPointF& pixelPos) const;

    /**
     * @brief 世界坐标转像素坐标
     * @param worldPos 世界坐标
     * @return 像素坐标
     */
    QPointF worldToPixel(const QPointF& worldPos) const;

    /**
     * @brief 批量像素坐标转世界坐标
     */
    std::vector<QPointF> pixelToWorld(const std::vector<QPointF>& pixelPoints) const;

    /**
     * @brief 批量世界坐标转像素坐标
     */
    std::vector<QPointF> worldToPixel(const std::vector<QPointF>& worldPoints) const;

    // ============ 验证 ============

    /**
     * @brief 检查标定是否有效
     */
    bool isValid() const {
        return status == CalibrationStatus::Calibrated &&
               error == CalibrationError::Success;
    }

    /**
     * @brief 检查矩阵是否可逆
     */
    bool isMatrixInvertible() const;

    // ============ 序列化 ============

    QJsonObject toJson() const;
    static CalibrationResult fromJson(const QJsonObject& json);
    QString toString() const;
};

// ============================================================
// 对齐结果结构
// ============================================================

/**
 * @brief 对齐偏差
 *
 * 存储对齐计算得到的位置偏差。
 */
struct AlignmentOffset {
    double dx = 0.0;            ///< X方向偏移（mm）
    double dy = 0.0;            ///< Y方向偏移（mm）
    double dAngle = 0.0;        ///< 角度偏移（度）

    // 仿射变换的额外参数
    double scaleX = 1.0;        ///< X方向缩放
    double scaleY = 1.0;        ///< Y方向缩放
    double shear = 0.0;         ///< 剪切参数

    /**
     * @brief 获取平移距离
     */
    double getTranslationDistance() const {
        return std::sqrt(dx * dx + dy * dy);
    }

    /**
     * @brief 检查是否在容差范围内
     */
    bool isWithinTolerance(double posTolerance, double angleTolerance) const {
        return getTranslationDistance() <= posTolerance &&
               std::abs(dAngle) <= angleTolerance;
    }

    /**
     * @brief 反转偏差（用于补偿）
     */
    AlignmentOffset inverted() const {
        AlignmentOffset inv;
        inv.dx = -dx;
        inv.dy = -dy;
        inv.dAngle = -dAngle;
        return inv;
    }

    QJsonObject toJson() const;
    static AlignmentOffset fromJson(const QJsonObject& json);
    QString toString() const;
};

/**
 * @brief 对齐结果
 */
struct AlignmentResult {
    AlignmentType type = AlignmentType::None;
    bool success = false;
    CalibrationError error = CalibrationError::NotCalibrated;

    AlignmentOffset offset;     ///< 计算得到的偏差
    Matrix3d transform = Matrix3d::Identity();  ///< 完整变换矩阵

    double confidence = 0.0;    ///< 置信度（0-1）
    double residualError = 0.0; ///< 残差误差

    QJsonObject toJson() const;
    static AlignmentResult fromJson(const QJsonObject& json);
};

// ============================================================
// 标定配置
// ============================================================

/**
 * @brief 标定配置参数
 */
struct CalibrationConfig {
    // ============ 基本配置 ============
    CalibrationType type = CalibrationType::Point9;
    QString name = "Default";
    QString description;

    // ============ 点阵配置（用于9点、16点等） ============
    int gridRows = 3;           ///< 行数
    int gridCols = 3;           ///< 列数
    double gridSpacingX = 10.0; ///< X方向间距（mm）
    double gridSpacingY = 10.0; ///< Y方向间距（mm）

    // ============ 精度配置 ============
    double maxAllowedRMS = 1.0;     ///< 允许的最大RMS误差（像素）
    double maxAllowedMax = 3.0;     ///< 允许的最大单点误差（像素）
    bool useWeightedLeastSquares = false;  ///< 使用加权最小二乘
    bool enableOutlierRejection = true;    ///< 启用异常点剔除
    double outlierThreshold = 3.0;          ///< 异常点阈值（标准差倍数）

    // ============ 优化配置 ============
    bool enableNonlinearOptimization = false;  ///< 启用非线性优化
    int maxIterations = 100;                    ///< 最大迭代次数
    double convergenceThreshold = 1e-6;         ///< 收敛阈值

    // ============ 有效期配置 ============
    int validityDays = 30;          ///< 有效期（天）
    bool autoExpire = false;        ///< 自动过期检查

    QJsonObject toJson() const;
    static CalibrationConfig fromJson(const QJsonObject& json);
};

// ============================================================
// 工具函数
// ============================================================

/**
 * @brief 获取标定类型名称
 */
QString getCalibrationTypeName(CalibrationType type);

/**
 * @brief 从名称获取标定类型
 */
CalibrationType getCalibrationTypeFromName(const QString& name);

/**
 * @brief 获取标定类型所需的最小点数
 */
int getMinPointsForType(CalibrationType type);

/**
 * @brief 获取标定状态名称
 */
QString getCalibrationStatusName(CalibrationStatus status);

/**
 * @brief 获取标定错误描述
 */
QString getCalibrationErrorDescription(CalibrationError error);

/**
 * @brief 获取对齐类型名称
 */
QString getAlignmentTypeName(AlignmentType type);

/**
 * @brief Matrix3d转QJsonArray
 */
QJsonArray matrix3dToJson(const Matrix3d& matrix);

/**
 * @brief QJsonArray转Matrix3d
 */
Matrix3d matrix3dFromJson(const QJsonArray& json);

} // namespace Calibration
} // namespace VisionForge

// 注册Qt元类型
Q_DECLARE_METATYPE(VisionForge::Calibration::CalibrationType)
Q_DECLARE_METATYPE(VisionForge::Calibration::CalibrationStatus)
Q_DECLARE_METATYPE(VisionForge::Calibration::AlignmentType)
Q_DECLARE_METATYPE(VisionForge::Calibration::CalibrationPoint)
Q_DECLARE_METATYPE(VisionForge::Calibration::CalibrationResult)
Q_DECLARE_METATYPE(VisionForge::Calibration::AlignmentResult)

#endif // VISIONFORGE_CALIBRATION_TYPES_H
