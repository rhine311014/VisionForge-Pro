/**
 * @file MultiPointAlignmentTool.h
 * @brief 多点对位检测工具
 * @details 支持单图像多点和多相机多点对位检测，SVD偏差计算
 * @author VisionForge Team
 * @date 2025-12-19
 */

#pragma once

#include "algorithm/VisionTool.h"
#include "algorithm/CoordinateTransform.h"
#include "base/ImageData.h"
#include <QMap>
#include <QVector>
#include <memory>

namespace VisionForge {
namespace Algorithm {

/**
 * @brief 对位点检测类型
 */
enum class AlignmentPointType {
    TemplateMatch,      ///< 模板匹配
    ShapeMatch,         ///< 形状匹配（Halcon）
    BlobCenter,         ///< Blob中心
    CircleCenter,       ///< 圆心检测
    EdgePoint,          ///< 边缘点
    CrosshairCenter     ///< 十字中心
};

/**
 * @brief 偏差计算模式
 */
enum class DeviationCalcMode {
    SinglePoint,        ///< 单点偏差：只使用第一个点
    TwoPoints,          ///< 两点偏差：使用两点计算X/Y/θ
    MultiPoints,        ///< 多点偏差：使用所有点最小二乘
    RigidTransform      ///< 刚性变换：SVD求解最佳变换
};

/**
 * @brief 对位点配置
 */
struct AlignmentPointConfig {
    QString id;                     ///< 点ID
    QString name;                   ///< 点名称
    AlignmentPointType type;        ///< 检测类型
    QString cameraId;               ///< 关联相机ID（多相机时使用）
    QRectF searchROI;               ///< 搜索区域
    QPointF standardPosition;       ///< 标准位置（像素坐标）
    double standardAngle;           ///< 标准角度（度）
    bool enabled;                   ///< 是否启用
    bool useCoordinateTransform;    ///< 是否使用坐标转换
    QString calibrationId;          ///< 关联的标定ID

    // 检测参数（根据类型使用不同参数）
    QVariantMap parameters;         ///< 类型特定参数

    AlignmentPointConfig()
        : type(AlignmentPointType::TemplateMatch)
        , standardAngle(0.0)
        , enabled(true)
        , useCoordinateTransform(false)
    {}

    QJsonObject toJson() const;
    static AlignmentPointConfig fromJson(const QJsonObject& json);
};

/**
 * @brief 单点检测结果
 */
struct PointDetectionResult {
    QString pointId;                ///< 点ID
    bool success;                   ///< 检测是否成功
    QPointF detectedPosition;       ///< 检测到的位置（像素）
    double detectedAngle;           ///< 检测到的角度（度）
    QPointF worldPosition;          ///< 世界坐标位置（mm）
    double score;                   ///< 检测得分/置信度
    double offsetX;                 ///< X方向偏差
    double offsetY;                 ///< Y方向偏差
    double offsetAngle;             ///< 角度偏差
    QString errorMessage;           ///< 错误信息
    double processTime;             ///< 处理时间(ms)

    PointDetectionResult()
        : success(false)
        , detectedAngle(0.0)
        , score(0.0)
        , offsetX(0.0)
        , offsetY(0.0)
        , offsetAngle(0.0)
        , processTime(0.0)
    {}

    QJsonObject toJson() const;
};

/**
 * @brief 对位检测总结果
 */
struct AlignmentResult {
    bool success;                   ///< 总体是否成功
    QMap<QString, PointDetectionResult> pointResults;  ///< 各点结果

    // 综合偏差（刚性变换结果）
    double offsetX;                 ///< X方向总偏差
    double offsetY;                 ///< Y方向总偏差
    double offsetTheta;             ///< 角度总偏差（度）
    QPointF rotationCenter;         ///< 旋转中心

    // 残差信息
    double residualError;           ///< 残差误差
    double maxPointError;           ///< 最大单点误差

    // 元数据
    int totalPoints;                ///< 总点数
    int successPoints;              ///< 成功检测的点数
    int failedPoints;               ///< 失败的点数
    double totalProcessTime;        ///< 总处理时间(ms)
    qint64 timestamp;               ///< 时间戳

    AlignmentResult()
        : success(false)
        , offsetX(0.0)
        , offsetY(0.0)
        , offsetTheta(0.0)
        , residualError(0.0)
        , maxPointError(0.0)
        , totalPoints(0)
        , successPoints(0)
        , failedPoints(0)
        , totalProcessTime(0.0)
        , timestamp(0)
    {}

    QJsonObject toJson() const;
};

/**
 * @class MultiPointAlignmentTool
 * @brief 多点对位检测工具
 *
 * 功能：
 * - 管理多个对位点配置
 * - 支持多种检测类型
 * - 单图像多点检测
 * - 多相机多点检测
 * - SVD刚性变换计算X/Y/θ偏差
 * - 可选坐标转换
 */
class MultiPointAlignmentTool : public VisionTool
{
    Q_OBJECT

public:
    explicit MultiPointAlignmentTool(QObject* parent = nullptr);
    ~MultiPointAlignmentTool() override;

    // ========== VisionTool 接口 ==========

    ToolType toolType() const override { return ToolType::MultiPointAlignment; }
    QString toolName() const override { return "MultiPointAlignment"; }

    bool process(const Base::ImageData::Ptr& input, ToolResult& output) override;
    QJsonObject serializeParams() const override;
    void deserializeParams(const QJsonObject& json) override;

    // ========== 对位点管理 ==========

    /**
     * @brief 添加对位点
     * @param config 点配置
     * @return 点ID
     */
    QString addAlignmentPoint(const AlignmentPointConfig& config);

    /**
     * @brief 移除对位点
     * @param pointId 点ID
     * @return 是否成功
     */
    bool removeAlignmentPoint(const QString& pointId);

    /**
     * @brief 更新对位点
     * @param pointId 点ID
     * @param config 新配置
     * @return 是否成功
     */
    bool updateAlignmentPoint(const QString& pointId, const AlignmentPointConfig& config);

    /**
     * @brief 获取对位点配置
     * @param pointId 点ID
     * @return 点配置
     */
    AlignmentPointConfig getAlignmentPoint(const QString& pointId) const;

    /**
     * @brief 获取所有对位点
     */
    QVector<AlignmentPointConfig> getAllAlignmentPoints() const;

    /**
     * @brief 获取对位点数量
     */
    int alignmentPointCount() const;

    /**
     * @brief 清空所有对位点
     */
    void clearAlignmentPoints();

    // ========== 偏差计算设置 ==========

    /**
     * @brief 设置偏差计算模式
     */
    void setDeviationCalcMode(DeviationCalcMode mode);

    /**
     * @brief 获取偏差计算模式
     */
    DeviationCalcMode getDeviationCalcMode() const;

    /**
     * @brief 设置旋转中心
     * @param center 旋转中心（像素坐标）
     * @note 用于RigidTransform模式
     */
    void setRotationCenter(const QPointF& center);

    /**
     * @brief 获取旋转中心
     */
    QPointF getRotationCenter() const;

    /**
     * @brief 设置是否自动计算旋转中心
     */
    void setAutoRotationCenter(bool autoCalc);

    /**
     * @brief 是否自动计算旋转中心
     */
    bool isAutoRotationCenter() const;

    // ========== 坐标转换 ==========

    /**
     * @brief 设置坐标转换结果
     * @param calibId 标定ID
     * @param transform 坐标转换对象
     */
    void setCoordinateTransform(const QString& calibId, std::shared_ptr<CoordinateTransform> transform);

    /**
     * @brief 获取坐标转换对象
     */
    std::shared_ptr<CoordinateTransform> getCoordinateTransform(const QString& calibId) const;

    /**
     * @brief 设置是否启用坐标转换
     */
    void setCoordinateTransformEnabled(bool enabled);

    /**
     * @brief 是否启用坐标转换
     */
    bool isCoordinateTransformEnabled() const;

    // ========== 检测执行 ==========

    /**
     * @brief 检测单张图像
     * @param image 输入图像
     * @return 对位结果
     */
    AlignmentResult detectSingleImage(const Base::ImageData::Ptr& image);

    /**
     * @brief 检测多相机图像
     * @param images 各相机图像 <cameraId, image>
     * @return 对位结果
     */
    AlignmentResult detectMultiCamera(const QMap<QString, Base::ImageData::Ptr>& images);

    /**
     * @brief 获取最近一次检测结果
     */
    AlignmentResult getLastResult() const;

    // ========== 模板管理 ==========

    /**
     * @brief 设置点的模板图像
     * @param pointId 点ID
     * @param templateImage 模板图像
     */
    void setPointTemplate(const QString& pointId, const cv::Mat& templateImage);

    /**
     * @brief 获取点的模板图像
     */
    cv::Mat getPointTemplate(const QString& pointId) const;

    /**
     * @brief 从当前图像区域创建模板
     * @param pointId 点ID
     * @param image 源图像
     * @param roi 模板区域
     * @return 是否成功
     */
    bool createTemplateFromImage(const QString& pointId,
                                  const Base::ImageData::Ptr& image,
                                  const QRectF& roi);

signals:
    /**
     * @brief 点配置变化信号
     */
    void alignmentPointChanged(const QString& pointId);

    /**
     * @brief 检测完成信号
     */
    void detectionCompleted(const AlignmentResult& result);

    /**
     * @brief 单点检测完成信号
     */
    void pointDetected(const QString& pointId, const PointDetectionResult& result);

private:
    // 单点检测方法
    PointDetectionResult detectPoint(const AlignmentPointConfig& config,
                                      const cv::Mat& image);

    // 各类型检测实现
    PointDetectionResult detectTemplateMatch(const AlignmentPointConfig& config,
                                              const cv::Mat& image);
    PointDetectionResult detectBlobCenter(const AlignmentPointConfig& config,
                                           const cv::Mat& image);
    PointDetectionResult detectCircleCenter(const AlignmentPointConfig& config,
                                             const cv::Mat& image);
    PointDetectionResult detectEdgePoint(const AlignmentPointConfig& config,
                                          const cv::Mat& image);

    // 计算综合偏差
    void calculateOverallDeviation(AlignmentResult& result);

    // SVD刚性变换
    void calculateRigidTransformDeviation(AlignmentResult& result);

    // 应用坐标转换
    void applyCoordinateTransform(PointDetectionResult& result,
                                   const AlignmentPointConfig& config);

    // 生成点ID
    QString generatePointId();

private:
    QMap<QString, AlignmentPointConfig> alignmentPoints_;   ///< 对位点配置
    QMap<QString, cv::Mat> pointTemplates_;                 ///< 点模板图像
    QMap<QString, std::shared_ptr<CoordinateTransform>> transforms_;  ///< 坐标转换

    DeviationCalcMode deviationMode_;       ///< 偏差计算模式
    QPointF rotationCenter_;                ///< 旋转中心
    bool autoRotationCenter_;               ///< 自动计算旋转中心
    bool coordTransformEnabled_;            ///< 是否启用坐标转换

    AlignmentResult lastResult_;            ///< 最近检测结果
    int pointIdCounter_;                    ///< 点ID计数器
};

} // namespace Algorithm
} // namespace VisionForge
