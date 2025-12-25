/**
 * @file OpenCVCircleDetector.h
 * @brief OpenCV圆检测后端实现
 * @author VisionForge Team
 * @date 2025-12-25
 */

#pragma once

#include "algorithm/backend/ICircleDetector.h"
#include <opencv2/imgproc.hpp>

namespace VisionForge {
namespace Algorithm {
namespace Backend {

/**
 * @brief 检测方法枚举
 */
enum class OpenCVCircleMethod {
    HoughCircle,    // 霍夫圆检测
    ContourFit      // 轮廓拟合
};

/**
 * @brief OpenCV圆检测后端实现
 *
 * 使用OpenCV实现圆检测功能，支持霍夫圆检测和轮廓拟合两种方法。
 */
class OpenCVCircleDetector : public ICircleDetector {
public:
    OpenCVCircleDetector();
    ~OpenCVCircleDetector() override = default;

    // ========== ICircleDetector 接口实现 ==========

    QString backendName() const override { return "OpenCV"; }

    bool detect(const cv::Mat& image,
               const CircleDetectParams& params,
               std::vector<CircleDetectResult>& results,
               QString& errorMsg) override;

    bool supportsMethod(const QString& method) const override;

    QStringList supportedMethods() const override;

    // ========== OpenCV特有接口 ==========

    /**
     * @brief 设置检测方法
     * @param method 检测方法
     */
    void setMethod(OpenCVCircleMethod method) { method_ = method; }

    /**
     * @brief 获取当前检测方法
     */
    OpenCVCircleMethod method() const { return method_; }

private:
    /**
     * @brief 使用霍夫变换检测圆
     */
    bool detectWithHough(const cv::Mat& image,
                        const CircleDetectParams& params,
                        std::vector<CircleDetectResult>& results,
                        QString& errorMsg);

    /**
     * @brief 使用轮廓拟合检测圆
     */
    bool detectWithContourFit(const cv::Mat& image,
                             const CircleDetectParams& params,
                             std::vector<CircleDetectResult>& results,
                             QString& errorMsg);

private:
    OpenCVCircleMethod method_;
};

} // namespace Backend
} // namespace Algorithm
} // namespace VisionForge
