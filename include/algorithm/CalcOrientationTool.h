/**
 * @file CalcOrientationTool.h
 * @brief 方向计算工具
 * @details 计算目标的主轴方向
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
 * @brief 方向计算工具
 *
 * 支持多种方向计算方法：
 * - 主成分分析（PCA）
 * - 最小外接矩形
 * - 惯性矩
 * - 拟合椭圆
 */
class CalcOrientationTool : public VisionTool {
    Q_OBJECT

public:
    /**
     * @brief 计算方法
     */
    enum CalcMethod {
        PCA,                // 主成分分析
        MinAreaRect,        // 最小外接矩形
        Moments,            // 惯性矩（二阶矩）
        FitEllipse          // 拟合椭圆
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

    explicit CalcOrientationTool(QObject* parent = nullptr);

    // ========== VisionTool接口实现 ==========

    ToolType toolType() const override { return CalcOrientation; }
    QString toolName() const override { return tr("方向计算"); }

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
     * @brief 设置输入点列表
     */
    void setInputPoints(const std::vector<QPointF>& points);
    const std::vector<QPointF>& inputPoints() const { return inputPoints_; }

    /**
     * @brief 设置二值化阈值
     */
    void setThreshold(int threshold);
    int threshold() const { return threshold_; }

    /**
     * @brief 设置是否反转
     */
    void setInverted(bool inverted);
    bool isInverted() const { return inverted_; }

    /**
     * @brief 设置角度归一化范围
     * @param range 0=0~360, 1=-180~180, 2=0~180
     */
    void setAngleRange(int range);
    int angleRange() const { return angleRange_; }

    // ========== 结果获取 ==========

    /**
     * @brief 获取计算的方向角度（度）
     */
    double angle() const { return angle_; }

    /**
     * @brief 获取中心点
     */
    QPointF center() const { return center_; }

    /**
     * @brief 获取主轴长度
     */
    double majorAxisLength() const { return majorAxisLength_; }

    /**
     * @brief 获取次轴长度
     */
    double minorAxisLength() const { return minorAxisLength_; }

private:
    /**
     * @brief 从图像计算方向
     */
    bool calcFromImage(const cv::Mat& gray, ToolResult& output);

    /**
     * @brief 从轮廓计算方向
     */
    bool calcFromContour(const std::vector<cv::Point>& contour, ToolResult& output);

    /**
     * @brief 从点列表计算方向
     */
    bool calcFromPoints(ToolResult& output);

    /**
     * @brief 使用PCA计算方向
     */
    void calcPCA(const std::vector<cv::Point2f>& points);

    /**
     * @brief 使用矩计算方向
     */
    void calcMoments(const std::vector<cv::Point>& contour);

    /**
     * @brief 归一化角度
     */
    double normalizeAngle(double angle) const;

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
    int angleRange_;

    // 结果
    double angle_;
    QPointF center_;
    double majorAxisLength_;
    double minorAxisLength_;
};

} // namespace Algorithm
} // namespace VisionForge
