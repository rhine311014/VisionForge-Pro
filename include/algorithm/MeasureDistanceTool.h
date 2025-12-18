/**
 * @file MeasureDistanceTool.h
 * @brief 距离测量工具
 * @details 测量两点、点到线、线到线等距离
 * @author VisionForge Team
 * @date 2025-12-18
 */

#pragma once

#include "algorithm/VisionTool.h"
#include <QPointF>
#include <QLineF>
#include <vector>

namespace VisionForge {
namespace Algorithm {

/**
 * @brief 测量目标类型
 */
enum class MeasureTargetType {
    Point,      // 点
    Line,       // 线
    Circle,     // 圆
    Contour     // 轮廓
};

/**
 * @brief 距离测量结果
 */
struct DistanceResult {
    double distance = 0.0;          // 测量距离
    QPointF point1;                 // 第一个测量点
    QPointF point2;                 // 第二个测量点
    double pixelToMm = 1.0;         // 像素到毫米换算系数
    double distanceMm = 0.0;        // 毫米距离
    bool valid = false;             // 是否有效
    QString description;            // 描述
};

/**
 * @class MeasureDistanceTool
 * @brief 距离测量工具
 *
 * 支持的测量模式:
 * - 两点之间距离
 * - 点到直线距离
 * - 两条平行线距离
 * - 圆心到点/线距离
 * - 最短距离 (轮廓间)
 */
class MeasureDistanceTool : public VisionTool
{
    Q_OBJECT

public:
    /**
     * @brief 测量模式
     */
    enum MeasureMode {
        PointToPoint,       // 点到点
        PointToLine,        // 点到线
        LineToLine,         // 线到线 (平行线间距)
        CircleToPoint,      // 圆心到点
        CircleToLine,       // 圆心到线
        CircleToCircle,     // 圆心到圆心
        MinDistance         // 最短距离
    };
    Q_ENUM(MeasureMode)

    explicit MeasureDistanceTool(QObject* parent = nullptr);
    ~MeasureDistanceTool() override = default;

    // ========== VisionTool接口 ==========
    ToolType toolType() const override { return ToolType::MeasureDistance; }
    QString toolName() const override { return "距离测量"; }
    bool process(const Base::ImageData::Ptr& input, ToolResult& output) override;
    QJsonObject serializeParams() const override;
    void deserializeParams(const QJsonObject& json) override;

    // ========== 测量设置 ==========

    /**
     * @brief 设置测量模式
     */
    void setMeasureMode(MeasureMode mode);
    MeasureMode measureMode() const { return mode_; }

    /**
     * @brief 设置第一个点
     */
    void setPoint1(const QPointF& pt) { point1_ = pt; emit paramChanged(); }
    QPointF point1() const { return point1_; }

    /**
     * @brief 设置第二个点
     */
    void setPoint2(const QPointF& pt) { point2_ = pt; emit paramChanged(); }
    QPointF point2() const { return point2_; }

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
     * @brief 设置圆心和半径
     */
    void setCircle1(const QPointF& center, double radius);
    void setCircle2(const QPointF& center, double radius);

    /**
     * @brief 设置像素到毫米换算系数
     */
    void setPixelToMm(double ratio);
    double pixelToMm() const { return pixelToMm_; }

    /**
     * @brief 获取测量结果
     */
    const DistanceResult& result() const { return result_; }

    /**
     * @brief 从工具链输入获取数据
     */
    void setInputFromToolChain(bool enable) { useToolChainInput_ = enable; }

signals:
    void measurementCompleted(const DistanceResult& result);

private:
    double measurePointToPoint();
    double measurePointToLine();
    double measureLineToLine();
    double measureCircleToPoint();
    double measureCircleToLine();
    double measureCircleToCircle();

    cv::Mat drawResults(const cv::Mat& input) const;

private:
    MeasureMode mode_;
    QPointF point1_;
    QPointF point2_;
    QLineF line1_;
    QLineF line2_;
    QPointF circle1Center_;
    double circle1Radius_;
    QPointF circle2Center_;
    double circle2Radius_;
    double pixelToMm_;
    bool useToolChainInput_;
    DistanceResult result_;
};

} // namespace Algorithm
} // namespace VisionForge
