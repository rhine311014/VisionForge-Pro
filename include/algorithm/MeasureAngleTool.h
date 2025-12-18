/**
 * @file MeasureAngleTool.h
 * @brief 角度测量工具
 * @details 测量两条线之间的角度、三点角度等
 * @author VisionForge Team
 * @date 2025-12-18
 */

#pragma once

#include "algorithm/VisionTool.h"
#include <QPointF>
#include <QLineF>

namespace VisionForge {
namespace Algorithm {

/**
 * @brief 角度测量结果
 */
struct AngleResult {
    double angle = 0.0;             // 角度值 (度)
    double angleRad = 0.0;          // 角度值 (弧度)
    QPointF vertex;                 // 角的顶点
    QPointF point1;                 // 第一条边上的点
    QPointF point2;                 // 第二条边上的点
    bool valid = false;             // 是否有效
    QString description;            // 描述
};

/**
 * @class MeasureAngleTool
 * @brief 角度测量工具
 *
 * 支持的测量模式:
 * - 两条线之间的夹角
 * - 三点角度 (顶点 + 两个端点)
 * - 线与水平方向的角度
 * - 线与垂直方向的角度
 */
class MeasureAngleTool : public VisionTool
{
    Q_OBJECT

public:
    /**
     * @brief 测量模式
     */
    enum MeasureMode {
        LineToLine,         // 两条线夹角
        ThreePoints,        // 三点角度
        LineToHorizontal,   // 线与水平方向
        LineToVertical      // 线与垂直方向
    };
    Q_ENUM(MeasureMode)

    /**
     * @brief 角度范围
     */
    enum AngleRange {
        Range0To180,        // 0-180度
        Range0To360,        // 0-360度
        RangeMinus180To180  // -180到180度
    };
    Q_ENUM(AngleRange)

    explicit MeasureAngleTool(QObject* parent = nullptr);
    ~MeasureAngleTool() override = default;

    // ========== VisionTool接口 ==========
    ToolType toolType() const override { return ToolType::MeasureAngle; }
    QString toolName() const override { return "角度测量"; }
    bool process(const Base::ImageData::Ptr& input, ToolResult& output) override;
    QJsonObject serializeParams() const override;
    void deserializeParams(const QJsonObject& json) override;

    // ========== 测量设置 ==========

    void setMeasureMode(MeasureMode mode);
    MeasureMode measureMode() const { return mode_; }

    void setAngleRange(AngleRange range);
    AngleRange angleRange() const { return angleRange_; }

    /**
     * @brief 设置第一条线
     */
    void setLine1(const QLineF& line) { line1_ = line; emit paramChanged(); }
    QLineF line1() const { return line1_; }

    /**
     * @brief 设置第二条线
     */
    void setLine2(const QLineF& line) { line2_ = line; emit paramChanged(); }
    QLineF line2() const { return line2_; }

    /**
     * @brief 设置三点 (顶点, 点1, 点2)
     */
    void setVertex(const QPointF& pt) { vertex_ = pt; emit paramChanged(); }
    void setPoint1(const QPointF& pt) { point1_ = pt; emit paramChanged(); }
    void setPoint2(const QPointF& pt) { point2_ = pt; emit paramChanged(); }

    QPointF vertex() const { return vertex_; }
    QPointF point1() const { return point1_; }
    QPointF point2() const { return point2_; }

    /**
     * @brief 获取测量结果
     */
    const AngleResult& result() const { return result_; }

signals:
    void measurementCompleted(const AngleResult& result);

private:
    double measureLineToLine();
    double measureThreePoints();
    double measureLineToHorizontal();
    double measureLineToVertical();
    double normalizeAngle(double angle);

    cv::Mat drawResults(const cv::Mat& input) const;

private:
    MeasureMode mode_;
    AngleRange angleRange_;
    QLineF line1_;
    QLineF line2_;
    QPointF vertex_;
    QPointF point1_;
    QPointF point2_;
    AngleResult result_;
};

} // namespace Algorithm
} // namespace VisionForge
