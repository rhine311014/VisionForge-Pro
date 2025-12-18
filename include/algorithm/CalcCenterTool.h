/**
 * @file CalcCenterTool.h
 * @brief 中心计算工具
 * @details 从多个点或区域计算几何中心
 * @author VisionForge Team
 * @date 2025-12-18
 */

#pragma once

#include "algorithm/VisionTool.h"
#include <QPointF>
#include <vector>

namespace VisionForge {
namespace Algorithm {

/**
 * @brief 中心计算工具
 *
 * 支持多种中心计算方法：
 * - 质心（灰度/二值）
 * - 几何中心
 * - 边界框中心
 * - 最小外接圆中心
 * - 多点平均中心
 */
class CalcCenterTool : public VisionTool {
    Q_OBJECT

public:
    /**
     * @brief 计算方法
     */
    enum CalcMethod {
        Centroid,           // 质心（矩计算）
        GeometricCenter,    // 几何中心（轮廓点平均）
        BoundingBoxCenter,  // 边界框中心
        MinCircleCenter,    // 最小外接圆中心
        PointsAverage       // 输入点平均
    };
    Q_ENUM(CalcMethod)

    /**
     * @brief 数据源类型
     */
    enum SourceType {
        FromImage,          // 从图像计算（需要二值图）
        FromContour,        // 从轮廓计算
        FromPoints          // 从点列表计算
    };
    Q_ENUM(SourceType)

    explicit CalcCenterTool(QObject* parent = nullptr);

    // ========== VisionTool接口实现 ==========

    ToolType toolType() const override { return CalcCenter; }
    QString toolName() const override { return tr("中心计算"); }

    bool process(const Base::ImageData::Ptr& input, ToolResult& output) override;

    QJsonObject serializeParams() const override;
    void deserializeParams(const QJsonObject& json) override;

    // ========== 参数设置 ==========

    /**
     * @brief 设置计算方法
     */
    void setCalcMethod(CalcMethod method);
    CalcMethod calcMethod() const { return calcMethod_; }

    /**
     * @brief 设置数据源类型
     */
    void setSourceType(SourceType type);
    SourceType sourceType() const { return sourceType_; }

    /**
     * @brief 设置输入点列表（PointsAverage模式）
     */
    void setInputPoints(const std::vector<QPointF>& points);
    const std::vector<QPointF>& inputPoints() const { return inputPoints_; }

    /**
     * @brief 设置二值化阈值（FromImage模式）
     */
    void setThreshold(int threshold);
    int threshold() const { return threshold_; }

    /**
     * @brief 设置是否反转（前景/背景互换）
     */
    void setInverted(bool inverted);
    bool isInverted() const { return inverted_; }

    // ========== 结果获取 ==========

    /**
     * @brief 获取计算的中心点
     */
    QPointF center() const { return center_; }

    /**
     * @brief 获取计算的面积（如适用）
     */
    double area() const { return area_; }

private:
    /**
     * @brief 从图像计算中心
     */
    bool calcFromImage(const cv::Mat& gray, ToolResult& output);

    /**
     * @brief 从轮廓计算中心
     */
    bool calcFromContour(const std::vector<cv::Point>& contour, ToolResult& output);

    /**
     * @brief 从点列表计算中心
     */
    bool calcFromPoints(ToolResult& output);

    /**
     * @brief 绘制结果
     */
    cv::Mat drawResults(const cv::Mat& input) const;

private:
    CalcMethod calcMethod_;
    SourceType sourceType_;
    std::vector<QPointF> inputPoints_;
    int threshold_;
    bool inverted_;

    // 结果
    QPointF center_;
    double area_;
    double radius_;  // 最小外接圆半径
};

} // namespace Algorithm
} // namespace VisionForge
