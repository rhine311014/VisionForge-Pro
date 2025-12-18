/**
 * @file MeasureAreaTool.h
 * @brief 面积测量工具
 * @details 测量轮廓、Blob、多边形等的面积
 * @author VisionForge Team
 * @date 2025-12-18
 */

#pragma once

#include "algorithm/VisionTool.h"
#include <QPointF>
#include <QPolygonF>
#include <vector>

namespace VisionForge {
namespace Algorithm {

/**
 * @brief 面积测量结果
 */
struct AreaResult {
    double area = 0.0;              // 面积 (像素)
    double areaMm2 = 0.0;           // 面积 (平方毫米)
    double perimeter = 0.0;         // 周长
    QPointF centroid;               // 质心
    QRectF boundingRect;            // 外接矩形
    double circularity = 0.0;       // 圆度
    bool valid = false;             // 是否有效
};

/**
 * @class MeasureAreaTool
 * @brief 面积测量工具
 *
 * 支持的测量模式:
 * - 多边形面积
 * - 轮廓面积 (从二值图像)
 * - 圆形面积
 * - 椭圆面积
 */
class MeasureAreaTool : public VisionTool
{
    Q_OBJECT

public:
    /**
     * @brief 测量模式
     */
    enum MeasureMode {
        Polygon,        // 多边形
        Contour,        // 轮廓 (从二值图)
        Circle,         // 圆形
        Ellipse         // 椭圆
    };
    Q_ENUM(MeasureMode)

    explicit MeasureAreaTool(QObject* parent = nullptr);
    ~MeasureAreaTool() override = default;

    // ========== VisionTool接口 ==========
    ToolType toolType() const override { return ToolType::MeasureArea; }
    QString toolName() const override { return "面积测量"; }
    bool process(const Base::ImageData::Ptr& input, ToolResult& output) override;
    QJsonObject serializeParams() const override;
    void deserializeParams(const QJsonObject& json) override;

    // ========== 测量设置 ==========

    void setMeasureMode(MeasureMode mode);
    MeasureMode measureMode() const { return mode_; }

    /**
     * @brief 设置多边形顶点
     */
    void setPolygon(const QPolygonF& polygon);
    QPolygonF polygon() const { return polygon_; }

    /**
     * @brief 设置圆形参数
     */
    void setCircle(const QPointF& center, double radius);
    QPointF circleCenter() const { return circleCenter_; }
    double circleRadius() const { return circleRadius_; }

    /**
     * @brief 设置椭圆参数
     */
    void setEllipse(const QPointF& center, double a, double b, double angle = 0);

    /**
     * @brief 设置像素到毫米换算系数
     */
    void setPixelToMm(double ratio);
    double pixelToMm() const { return pixelToMm_; }

    /**
     * @brief 设置二值化阈值 (Contour模式)
     */
    void setThreshold(int thresh) { threshold_ = thresh; emit paramChanged(); }
    int threshold() const { return threshold_; }

    /**
     * @brief 设置最小面积过滤
     */
    void setMinArea(double area) { minArea_ = area; emit paramChanged(); }
    double minArea() const { return minArea_; }

    /**
     * @brief 获取测量结果
     */
    const AreaResult& result() const { return result_; }

    /**
     * @brief 获取所有轮廓结果 (Contour模式)
     */
    const std::vector<AreaResult>& allResults() const { return allResults_; }

signals:
    void measurementCompleted(const AreaResult& result);

private:
    double measurePolygon();
    void measureContours(const cv::Mat& input);
    double measureCircle();
    double measureEllipse();

    cv::Mat drawResults(const cv::Mat& input) const;

private:
    MeasureMode mode_;
    QPolygonF polygon_;
    QPointF circleCenter_;
    double circleRadius_;
    QPointF ellipseCenter_;
    double ellipseA_;       // 长轴
    double ellipseB_;       // 短轴
    double ellipseAngle_;   // 旋转角度
    double pixelToMm_;
    int threshold_;
    double minArea_;
    AreaResult result_;
    std::vector<AreaResult> allResults_;
    std::vector<std::vector<cv::Point>> contours_;
};

} // namespace Algorithm
} // namespace VisionForge
