/**
 * @file OpenCVBlobAnalyzer.h
 * @brief OpenCV Blob分析后端实现
 * @author VisionForge Team
 * @date 2025-12-25
 */

#pragma once

#include "algorithm/backend/IBlobAnalyzer.h"

namespace VisionForge {
namespace Algorithm {
namespace Backend {

/**
 * @brief OpenCV Blob分析后端实现
 */
class OpenCVBlobAnalyzer : public IBlobAnalyzer {
public:
    OpenCVBlobAnalyzer() = default;
    ~OpenCVBlobAnalyzer() override = default;

    QString backendName() const override { return "OpenCV"; }

    bool analyze(const cv::Mat& image,
                const BlobAnalyzeParams& params,
                std::vector<BlobAnalyzeResult>& results,
                QString& errorMsg) override;

private:
    BlobAnalyzeResult calculateFeatures(const std::vector<cv::Point>& contour, int id) const;
};

} // namespace Backend
} // namespace Algorithm
} // namespace VisionForge
