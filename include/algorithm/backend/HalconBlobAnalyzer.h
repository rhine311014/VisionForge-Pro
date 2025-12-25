/**
 * @file HalconBlobAnalyzer.h
 * @brief Halcon Blob分析后端实现
 * @author VisionForge Team
 * @date 2025-12-25
 */

#pragma once

#include "algorithm/backend/IBlobAnalyzer.h"

#ifdef USE_HALCON

namespace VisionForge {
namespace Algorithm {
namespace Backend {

/**
 * @brief Halcon Blob分析后端实现
 */
class HalconBlobAnalyzer : public IBlobAnalyzer {
public:
    HalconBlobAnalyzer() = default;
    ~HalconBlobAnalyzer() override = default;

    QString backendName() const override { return "Halcon"; }

    bool analyze(const cv::Mat& image,
                const BlobAnalyzeParams& params,
                std::vector<BlobAnalyzeResult>& results,
                QString& errorMsg) override;
};

} // namespace Backend
} // namespace Algorithm
} // namespace VisionForge

#endif // USE_HALCON
