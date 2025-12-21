/**
 * @file AlignmentEngine.h
 * @brief VisionForge Pro 对齐引擎
 *
 * 本文件定义了视觉对位系统的对齐引擎类，提供：
 * - 单点对齐：仅计算平移偏差
 * - 两点对齐：计算平移+旋转偏差
 * - 三点对齐：完整仿射变换对齐
 * - 误差补偿和迭代优化
 *
 * 与Platform模块无缝集成，支持多种平台类型的对齐计算。
 *
 * 设计参考VisionASM的对齐算法实现。
 *
 * @author VisionForge Team
 * @version 1.2.0
 * @date 2025
 */

#ifndef VISIONFORGE_ALIGNMENT_ENGINE_H
#define VISIONFORGE_ALIGNMENT_ENGINE_H

#include "CalibrationTypes.h"
#include "CalibrationManager.h"
#include "platform/CoordPosition.h"
#include "platform/PlatformTypes.h"
#include "platform/PlatformConfig.h"

#include <QObject>
#include <QPointF>
#include <memory>
#include <vector>
#include <functional>

namespace VisionForge {
namespace Calibration {

// 前向声明
class CalibrationManager;

/**
 * @brief 对齐点数据
 *
 * 存储对齐计算所需的点对信息。
 */
struct AlignmentPoint {
    QPointF templatePos;    ///< 模板位置（标准位置/世界坐标）
    QPointF measuredPos;    ///< 测量位置（当前检测位置/世界坐标）
    double templateAngle = 0.0;     ///< 模板角度（度）
    double measuredAngle = 0.0;     ///< 测量角度（度）

    int index = 0;          ///< 点索引
    bool isValid = true;    ///< 是否有效
    double weight = 1.0;    ///< 权重

    AlignmentPoint() = default;

    AlignmentPoint(const QPointF& tmpl, const QPointF& meas,
                   double tmplAngle = 0.0, double measAngle = 0.0)
        : templatePos(tmpl), measuredPos(meas),
          templateAngle(tmplAngle), measuredAngle(measAngle) {}

    /**
     * @brief 计算位置偏差
     */
    QPointF getPositionError() const {
        return measuredPos - templatePos;
    }

    /**
     * @brief 计算角度偏差
     */
    double getAngleError() const {
        return measuredAngle - templateAngle;
    }

    /**
     * @brief 计算位置偏差距离
     */
    double getPositionErrorDistance() const {
        QPointF err = getPositionError();
        return std::sqrt(err.x() * err.x() + err.y() * err.y());
    }

    QJsonObject toJson() const;
    static AlignmentPoint fromJson(const QJsonObject& json);
};

/**
 * @brief 旋转中心配置
 */
struct RotationCenterConfig {
    QPointF center{0.0, 0.0};   ///< 旋转中心坐标（世界坐标，mm）
    bool useFixedCenter = true; ///< 使用固定旋转中心
    bool autoCalculate = false; ///< 自动计算旋转中心

    QJsonObject toJson() const;
    static RotationCenterConfig fromJson(const QJsonObject& json);
};

/**
 * @brief 对齐配置
 */
struct AlignmentConfig {
    AlignmentType type = AlignmentType::TwoPoint;   ///< 对齐类型

    // ============ 旋转配置 ============
    RotationCenterConfig rotationCenter;            ///< 旋转中心配置
    bool compensateRotation = true;                 ///< 是否补偿旋转

    // ============ 精度配置 ============
    double positionTolerance = 0.01;    ///< 位置容差（mm）
    double angleTolerance = 0.01;       ///< 角度容差（度）
    int maxIterations = 3;              ///< 最大迭代次数
    double convergenceThreshold = 0.001;///< 收敛阈值（mm）

    // ============ 补偿配置 ============
    bool enableIterativeCorrection = false;  ///< 启用迭代校正
    double correctionGain = 1.0;             ///< 校正增益（0-1，防止过补偿）

    // ============ 平台配置 ============
    Platform::PlatformType platformType = Platform::PlatformType::XYD;

    QJsonObject toJson() const;
    static AlignmentConfig fromJson(const QJsonObject& json);
};

/**
 * @brief 对齐引擎类
 *
 * 提供完整的视觉对位计算功能，是对位系统的核心算法类。
 *
 * 对齐流程：
 * 1. 设置模板位置（示教点）
 * 2. 获取当前测量位置（视觉检测结果）
 * 3. 计算对齐偏差
 * 4. 转换为平台运动量
 *
 * 使用示例：
 * @code
 * AlignmentEngine engine;
 *
 * // 配置对齐
 * AlignmentConfig config;
 * config.type = AlignmentType::TwoPoint;
 * engine.setConfig(config);
 *
 * // 设置标定管理器
 * engine.setCalibrationManager(calibManager);
 *
 * // 设置模板位置（示教时记录）
 * engine.setTemplatePoint(0, QPointF(100, 100), 0.0);
 * engine.setTemplatePoint(1, QPointF(200, 100), 0.0);
 *
 * // 设置测量位置（当前检测结果）
 * engine.setMeasuredPoint(0, QPointF(102, 101), 0.5);
 * engine.setMeasuredPoint(1, QPointF(203, 100), 0.5);
 *
 * // 计算对齐
 * AlignmentResult result = engine.calculate();
 * if (result.success) {
 *     // 获取平台运动量
 *     PlatformAxisPos motion = engine.toAxisMotion(result.offset);
 * }
 * @endcode
 */
class AlignmentEngine : public QObject {
    Q_OBJECT

public:
    // ============================================================
    // 构造和析构
    // ============================================================

    explicit AlignmentEngine(QObject* parent = nullptr);
    ~AlignmentEngine() override;

    // 禁用拷贝
    AlignmentEngine(const AlignmentEngine&) = delete;
    AlignmentEngine& operator=(const AlignmentEngine&) = delete;

    // ============================================================
    // 配置管理
    // ============================================================

    /**
     * @brief 设置对齐配置
     */
    void setConfig(const AlignmentConfig& config);

    /**
     * @brief 获取当前配置
     */
    const AlignmentConfig& getConfig() const { return m_config; }

    /**
     * @brief 设置对齐类型
     */
    void setAlignmentType(AlignmentType type);

    /**
     * @brief 获取对齐类型
     */
    AlignmentType getAlignmentType() const { return m_config.type; }

    /**
     * @brief 设置旋转中心
     */
    void setRotationCenter(const QPointF& center);

    /**
     * @brief 获取旋转中心
     */
    QPointF getRotationCenter() const { return m_config.rotationCenter.center; }

    /**
     * @brief 设置平台类型
     */
    void setPlatformType(Platform::PlatformType type);

    // ============================================================
    // 标定集成
    // ============================================================

    /**
     * @brief 设置标定管理器
     * @param manager 标定管理器指针（外部管理生命周期）
     */
    void setCalibrationManager(CalibrationManager* manager);

    /**
     * @brief 获取标定管理器
     */
    CalibrationManager* getCalibrationManager() const { return m_calibManager; }

    /**
     * @brief 检查是否已设置标定
     */
    bool hasCalibration() const;

    // ============================================================
    // 模板点管理
    // ============================================================

    /**
     * @brief 设置模板点（世界坐标）
     * @param index 点索引
     * @param worldPos 世界坐标（mm）
     * @param angle 角度（度）
     * @return 是否成功
     */
    bool setTemplatePoint(int index, const QPointF& worldPos, double angle = 0.0);

    /**
     * @brief 设置模板点（像素坐标，自动转换）
     * @param index 点索引
     * @param pixelPos 像素坐标
     * @param angle 角度（度）
     * @return 是否成功
     */
    bool setTemplatePointPixel(int index, const QPointF& pixelPos, double angle = 0.0);

    /**
     * @brief 设置模板点（使用CoordPosition）
     */
    bool setTemplatePoint(int index, const Platform::CoordPosition& pos);

    /**
     * @brief 获取模板点
     */
    const AlignmentPoint* getTemplatePoint(int index) const;

    /**
     * @brief 清空模板点
     */
    void clearTemplatePoints();

    /**
     * @brief 获取模板点数量
     */
    int getTemplatePointCount() const;

    // ============================================================
    // 测量点管理
    // ============================================================

    /**
     * @brief 设置测量点（世界坐标）
     * @param index 点索引
     * @param worldPos 世界坐标（mm）
     * @param angle 角度（度）
     * @return 是否成功
     */
    bool setMeasuredPoint(int index, const QPointF& worldPos, double angle = 0.0);

    /**
     * @brief 设置测量点（像素坐标，自动转换）
     */
    bool setMeasuredPointPixel(int index, const QPointF& pixelPos, double angle = 0.0);

    /**
     * @brief 设置测量点（使用CoordPosition）
     */
    bool setMeasuredPoint(int index, const Platform::CoordPosition& pos);

    /**
     * @brief 获取测量点
     */
    const AlignmentPoint* getMeasuredPoint(int index) const;

    /**
     * @brief 清空测量点
     */
    void clearMeasuredPoints();

    /**
     * @brief 获取测量点数量
     */
    int getMeasuredPointCount() const;

    // ============================================================
    // 对齐计算
    // ============================================================

    /**
     * @brief 执行对齐计算
     * @return 对齐结果
     */
    AlignmentResult calculate();

    /**
     * @brief 单点对齐计算
     *
     * 仅计算平移偏差：
     * dx = measured_x - template_x
     * dy = measured_y - template_y
     *
     * @return 对齐结果
     */
    AlignmentResult calculateSinglePoint();

    /**
     * @brief 两点对齐计算
     *
     * 计算平移+旋转偏差。
     * 使用两点连线计算旋转角度，然后计算绕旋转中心的位移。
     *
     * @return 对齐结果
     */
    AlignmentResult calculateTwoPoint();

    /**
     * @brief 三点对齐计算
     *
     * 计算完整仿射变换（平移+旋转+缩放）。
     *
     * @return 对齐结果
     */
    AlignmentResult calculateThreePoint();

    /**
     * @brief 四点对齐计算
     *
     * 计算透视变换对齐。
     *
     * @return 对齐结果
     */
    AlignmentResult calculateFourPoint();

    /**
     * @brief 使用自定义点进行对齐
     * @param templatePoints 模板点列表
     * @param measuredPoints 测量点列表
     * @return 对齐结果
     */
    AlignmentResult calculate(const std::vector<AlignmentPoint>& templatePoints,
                              const std::vector<AlignmentPoint>& measuredPoints);

    // ============================================================
    // 结果获取
    // ============================================================

    /**
     * @brief 获取最近一次计算结果
     */
    const AlignmentResult& getResult() const { return m_result; }

    /**
     * @brief 获取对齐偏差
     */
    AlignmentOffset getOffset() const { return m_result.offset; }

    /**
     * @brief 检查对齐是否在容差内
     */
    bool isWithinTolerance() const;

    // ============================================================
    // 平台运动转换
    // ============================================================

    /**
     * @brief 将对齐偏差转换为平台轴运动量
     *
     * 考虑平台类型和旋转中心，计算实际的轴运动量。
     *
     * @param offset 对齐偏差
     * @return 平台轴位置增量
     */
    Platform::PlatformAxisPos toAxisMotion(const AlignmentOffset& offset) const;

    /**
     * @brief 将对齐偏差转换为XYD运动量
     * @param offset 对齐偏差
     * @param rotCenter 旋转中心
     * @return XYD运动量
     */
    Platform::PlatformAxisPos toXYDMotion(const AlignmentOffset& offset,
                                          const QPointF& rotCenter) const;

    /**
     * @brief 将对齐偏差转换为X1X2Y运动量
     * @param offset 对齐偏差
     * @param platformInfo 平台配置信息
     * @return X1X2Y运动量
     */
    Platform::PlatformAxisPos toX1X2YMotion(const AlignmentOffset& offset,
                                             const Platform::PlatformX1X2YInfo* platformInfo) const;

    /**
     * @brief 获取补偿后的目标位置
     * @param currentPos 当前平台位置
     * @return 目标平台位置
     */
    Platform::PlatformAxisPos getCompensatedPosition(
        const Platform::PlatformAxisPos& currentPos) const;

    // ============================================================
    // 迭代优化
    // ============================================================

    /**
     * @brief 执行迭代对齐
     *
     * 多次测量和计算，逐步减小误差。
     *
     * @param measureCallback 测量回调函数（返回当前测量结果）
     * @param moveCallback 运动回调函数（执行平台运动）
     * @return 最终对齐结果
     */
    using MeasureCallback = std::function<std::vector<AlignmentPoint>()>;
    using MoveCallback = std::function<bool(const Platform::PlatformAxisPos&)>;

    AlignmentResult iterativeAlign(MeasureCallback measureCallback,
                                   MoveCallback moveCallback);

    /**
     * @brief 计算迭代校正量
     * @param offset 当前偏差
     * @param iteration 当前迭代次数
     * @return 校正后的偏差
     */
    AlignmentOffset calculateIterativeCorrection(const AlignmentOffset& offset,
                                                  int iteration);

    // ============================================================
    // 辅助计算
    // ============================================================

    /**
     * @brief 计算两点连线角度
     * @param p1 点1
     * @param p2 点2
     * @return 角度（度）
     */
    static double calculateLineAngle(const QPointF& p1, const QPointF& p2);

    /**
     * @brief 计算点绕中心旋转后的位置
     * @param point 原始点
     * @param center 旋转中心
     * @param angleDeg 旋转角度（度）
     * @return 旋转后的位置
     */
    static QPointF rotatePoint(const QPointF& point, const QPointF& center, double angleDeg);

    /**
     * @brief 计算旋转导致的位移
     * @param point 点位置
     * @param center 旋转中心
     * @param angleDeg 旋转角度（度）
     * @return 位移向量
     */
    static QPointF calculateRotationDisplacement(const QPointF& point,
                                                  const QPointF& center,
                                                  double angleDeg);

    /**
     * @brief 从三点计算仿射变换矩阵
     * @param srcPoints 源点（3个）
     * @param dstPoints 目标点（3个）
     * @return 2x3仿射变换矩阵
     */
    static Matrix23d computeAffineTransform(const std::vector<QPointF>& srcPoints,
                                            const std::vector<QPointF>& dstPoints);

    // ============================================================
    // 持久化
    // ============================================================

    /**
     * @brief 保存模板点到文件
     */
    bool saveTemplatePoints(const QString& filePath) const;

    /**
     * @brief 从文件加载模板点
     */
    bool loadTemplatePoints(const QString& filePath);

    /**
     * @brief 导出配置为JSON
     */
    QJsonObject exportToJson() const;

    /**
     * @brief 从JSON导入配置
     */
    bool importFromJson(const QJsonObject& json);

signals:
    /**
     * @brief 对齐完成信号
     */
    void alignmentCompleted(bool success, const AlignmentResult& result);

    /**
     * @brief 配置变化信号
     */
    void configChanged();

    /**
     * @brief 迭代进度信号
     */
    void iterationProgress(int current, int total, const AlignmentOffset& currentOffset);

private:
    // ============================================================
    // 内部辅助方法
    // ============================================================

    /**
     * @brief 验证点数是否满足对齐类型要求
     */
    bool validatePointCount() const;

    /**
     * @brief 准备对齐点对
     */
    std::vector<std::pair<AlignmentPoint, AlignmentPoint>> preparePointPairs() const;

    /**
     * @brief 计算置信度
     */
    double calculateConfidence(const AlignmentOffset& offset) const;

private:
    AlignmentConfig m_config;       ///< 对齐配置
    AlignmentResult m_result;       ///< 对齐结果

    std::vector<AlignmentPoint> m_templatePoints;   ///< 模板点
    std::vector<AlignmentPoint> m_measuredPoints;   ///< 测量点

    CalibrationManager* m_calibManager = nullptr;   ///< 标定管理器
};

/**
 * @brief 对齐引擎工厂
 */
class AlignmentEngineFactory {
public:
    /**
     * @brief 获取全局实例
     * @param platformIndex 平台索引
     */
    static AlignmentEngine* getInstance(int platformIndex = 0);

    /**
     * @brief 创建新实例
     */
    static std::unique_ptr<AlignmentEngine> create();

    /**
     * @brief 释放指定平台的实例
     */
    static void releaseInstance(int platformIndex);

    /**
     * @brief 释放所有实例
     */
    static void releaseAll();

private:
    static std::map<int, std::unique_ptr<AlignmentEngine>> s_instances;
};

} // namespace Calibration
} // namespace VisionForge

#endif // VISIONFORGE_ALIGNMENT_ENGINE_H
