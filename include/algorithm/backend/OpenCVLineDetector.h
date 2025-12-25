/**
 * @file OpenCVLineDetector.h
 * @brief OpenCV线检测后端实现
 * @author VisionForge Team
 * @date 2025-12-25
 */

#pragma once

#include "algorithm/backend/ILineDetector.h"
#include <opencv2/imgproc.hpp>

namespace VisionForge {
namespace Algorithm {
namespace Backend {

/**
 * @brief OpenCV线检测方法
 */
enum class OpenCVLineMethod {
    HoughLinesP,    // 概率霍夫变换
    HoughLines,     // 标准霍夫变换
    ContourFit      // 轮廓拟合
};

/**
 * @brief OpenCV线检测后端实现
 */
class OpenCVLineDetector : public ILineDetector {
public:
    OpenCVLineDetector();
    ~OpenCVLineDetector() override = default;

    QString backendName() const override { return "OpenCV"; }

    bool detect(const cv::Mat& image,
               const LineDetectParams& params,
               std::vector<LineDetectResult>& results,
               QString& errorMsg) override;

    bool supportsMethod(const QString& method) const override;
    QStringList supportedMethods() const override;

    void setMethod(OpenCVLineMethod method) { method_ = method; }
    OpenCVLineMethod method() const { return method_; }

private:
    bool detectWithHoughLinesP(const cv::Mat& image, const LineDetectParams& params,
                               std::vector<LineDetectResult>& results, QString& errorMsg);
    bool detectWithHoughLines(const cv::Mat& image, const LineDetectParams& params,
                              std::vector<LineDetectResult>& results, QString& errorMsg);
    bool detectWithContourFit(const cv::Mat& image, const LineDetectParams& params,
                              std::vector<LineDetectResult>& results, QString& errorMsg);

    OpenCVLineMethod method_;
};

} // namespace Backend
} // namespace Algorithm
} // namespace VisionForge
