/**
 * @file CircleTool.h
 * @brief 圆检测工具
 * @details 检测图像中的圆形，通过抽象接口支持OpenCV和Halcon双后端
 * @author VisionForge Team
 * @date 2025-12-18
 */

#pragma once

#include "algorithm/VisionTool.h"
#include "algorithm/SubPixelEdgeTool.h"
#include "algorithm/backend/ICircleDetector.h"
#include "algorithm/backend/AlgorithmBackendFactory.h"
#include <vector>
#include <QPointF>

namespace VisionForge {
namespace Algorithm {

/**
 * @brief 检测到的圆特征
 * @note 为保持向后兼容，保留此结构体，内部使用 Backend::CircleDetectResult
 */
struct CircleResult {
    int id;                     // 圆ID
    QPointF center;             // 圆心坐标
    double radius;              // 半径
    double score;               // 匹配得分/置信度
    double circularity;         // 圆度（1.0为完美圆）
    double area;                // 面积
    bool isSubPixel;            // 是否为亚像素精度
    double fitResidual;         // 拟合残差

    CircleResult()
        : id(0), radius(0), score(0), circularity(0), area(0),
          isSubPixel(false), fitResidual(0) {}

    CircleResult(int i, const QPointF& c, double r, double s = 1.0)
        : id(i), center(c), radius(r), score(s), circularity(1.0),
          isSubPixel(false), fitResidual(0)
    {
        area = M_PI * r * r;
    }

    // 从后端结果转换
    static CircleResult fromBackendResult(const Backend::CircleDetectResult& br) {
        CircleResult r;
        r.id = br.id;
        r.center = br.center;
        r.radius = br.radius;
        r.score = br.score;
        r.circularity = br.circularity;
        r.area = br.area;
        r.isSubPixel = br.isSubPixel;
        r.fitResidual = br.fitResidual;
        return r;
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

    // ========== 亚像素参数 ==========

    /**
     * @brief 设置是否启用亚像素精度
     */
    void setSubPixelEnabled(bool enabled);
    bool subPixelEnabled() const { return subPixelEnabled_; }

    /**
     * @brief 设置亚像素检测方法
     */
    void setSubPixelMethod(SubPixelMethod method);
    SubPixelMethod subPixelMethod() const { return subPixelMethod_; }

    /**
     * @brief 亚像素圆拟合
     * @param edgePoints 边缘点集合
     * @param result 输出结果
     * @return 拟合残差
     */
    double fitCircleSubPixel(const std::vector<cv::Point2f>& edgePoints, CircleResult& result);

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
     * @brief 绘制检测结果
     */
    cv::Mat drawResults(const cv::Mat& input) const;

    /**
     * @brief 创建或更新检测器
     */
    void ensureDetector();

    /**
     * @brief 将当前参数转换为后端参数
     */
    Backend::CircleDetectParams toBackendParams() const;

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

    // 亚像素参数
    bool subPixelEnabled_;          // 是否启用亚像素
    SubPixelMethod subPixelMethod_; // 亚像素方法

    std::vector<CircleResult> circles_;  // 检测结果

    // 后端检测器（通过抽象接口使用，无需 #ifdef）
    Backend::ICircleDetectorPtr detector_;
};

} // namespace Algorithm
} // namespace VisionForge
