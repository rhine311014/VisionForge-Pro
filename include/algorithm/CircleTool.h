/**
 * @file CircleTool.h
 * @brief 圆检测工具
 * @details 检测图像中的圆形，支持Halcon和OpenCV双后端
 * @author VisionForge Team
 * @date 2025-12-18
 */

#pragma once

#include "algorithm/VisionTool.h"
#include <vector>
#include <QPointF>

namespace VisionForge {
namespace Algorithm {

/**
 * @brief 检测到的圆特征
 */
struct CircleResult {
    int id;                     // 圆ID
    QPointF center;             // 圆心坐标
    double radius;              // 半径
    double score;               // 匹配得分/置信度
    double circularity;         // 圆度（1.0为完美圆）
    double area;                // 面积

    CircleResult()
        : id(0), radius(0), score(0), circularity(0), area(0) {}

    CircleResult(int i, const QPointF& c, double r, double s = 1.0)
        : id(i), center(c), radius(r), score(s), circularity(1.0)
    {
        area = M_PI * r * r;
    }
};

/**
 * @brief 圆检测工具
 *
 * 支持多种圆检测算法，可使用Halcon或OpenCV后端
 */
class CircleTool : public VisionTool {
    Q_OBJECT

public:
    /**
     * @brief 后端类型
     */
    enum BackendType {
        Auto,           // 自动选择（优先Halcon）
        OpenCV,         // OpenCV后端
        Halcon          // Halcon后端
    };
    Q_ENUM(BackendType)

    /**
     * @brief 检测方法
     */
    enum DetectionMethod {
        HoughCircle,        // 霍夫圆检测（OpenCV）
        ContourFit,         // 轮廓拟合（通用）
        EdgeFit,            // 边缘拟合（Halcon）
        BlobFit             // Blob区域拟合（Halcon）
    };
    Q_ENUM(DetectionMethod)

    explicit CircleTool(QObject* parent = nullptr);

    // ========== VisionTool接口实现 ==========

    ToolType toolType() const override { return FindCircle; }
    QString toolName() const override { return tr("圆检测"); }

    bool process(const Base::ImageData::Ptr& input, ToolResult& output) override;

    QJsonObject serializeParams() const override;
    void deserializeParams(const QJsonObject& json) override;

    // ========== 参数设置 ==========

    /**
     * @brief 设置后端类型
     */
    void setBackend(BackendType backend);
    BackendType backend() const { return backend_; }

    /**
     * @brief 设置检测方法
     */
    void setMethod(DetectionMethod method);
    DetectionMethod method() const { return method_; }

    /**
     * @brief 设置最小半径
     */
    void setMinRadius(double radius);
    double minRadius() const { return minRadius_; }

    /**
     * @brief 设置最大半径
     */
    void setMaxRadius(double radius);
    double maxRadius() const { return maxRadius_; }

    /**
     * @brief 设置最大检测数量
     */
    void setMaxCount(int count);
    int maxCount() const { return maxCount_; }

    /**
     * @brief 设置霍夫检测参数 - dp
     */
    void setHoughDp(double dp);
    double houghDp() const { return houghDp_; }

    /**
     * @brief 设置霍夫检测参数 - 最小圆心距离
     */
    void setMinDist(double dist);
    double minDist() const { return minDist_; }

    /**
     * @brief 设置霍夫检测参数 - Canny高阈值
     */
    void setCannyThreshold(double threshold);
    double cannyThreshold() const { return cannyThreshold_; }

    /**
     * @brief 设置霍夫检测参数 - 累加器阈值
     */
    void setAccumThreshold(double threshold);
    double accumThreshold() const { return accumThreshold_; }

    /**
     * @brief 设置轮廓拟合的最小圆度
     */
    void setMinCircularity(double circ);
    double minCircularity() const { return minCircularity_; }

    /**
     * @brief 设置轮廓拟合的最小面积
     */
    void setMinArea(double area);
    double minArea() const { return minArea_; }

    // ========== 结果获取 ==========

    /**
     * @brief 获取检测到的圆列表
     */
    const std::vector<CircleResult>& circles() const { return circles_; }

    /**
     * @brief 获取圆数量
     */
    int circleCount() const { return static_cast<int>(circles_.size()); }

private:
    /**
     * @brief 使用OpenCV霍夫圆检测
     */
    bool processWithHoughCircles(const cv::Mat& input, ToolResult& output);

    /**
     * @brief 使用OpenCV轮廓拟合
     */
    bool processWithContourFit(const cv::Mat& input, ToolResult& output);

#ifdef USE_HALCON
    /**
     * @brief 使用Halcon边缘拟合
     */
    bool processWithHalconEdgeFit(const cv::Mat& input, ToolResult& output);

    /**
     * @brief 使用Halcon Blob拟合
     */
    bool processWithHalconBlobFit(const cv::Mat& input, ToolResult& output);
#endif

    /**
     * @brief 绘制检测结果
     */
    cv::Mat drawResults(const cv::Mat& input) const;

private:
    BackendType backend_;           // 后端类型
    DetectionMethod method_;        // 检测方法

    // 通用参数
    double minRadius_;              // 最小半径
    double maxRadius_;              // 最大半径
    int maxCount_;                  // 最大检测数量

    // 霍夫参数
    double houghDp_;                // 分辨率比例
    double minDist_;                // 最小圆心距离
    double cannyThreshold_;         // Canny边缘检测阈值
    double accumThreshold_;         // 累加器阈值

    // 轮廓拟合参数
    double minCircularity_;         // 最小圆度
    double minArea_;                // 最小面积

    std::vector<CircleResult> circles_;  // 检测结果
};

} // namespace Algorithm
} // namespace VisionForge
