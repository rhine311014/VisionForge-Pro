/**
 * @file HalconLineDetector.h
 * @brief Halcon线检测后端实现
 * @author VisionForge Team
 * @date 2025-12-25
 */

#pragma once

#include "algorithm/backend/ILineDetector.h"

#ifdef USE_HALCON

namespace VisionForge {
namespace Algorithm {
namespace Backend {

/**
 * @brief Halcon线检测后端实现
 */
class HalconLineDetector : public ILineDetector {
public:
    HalconLineDetector();
    ~HalconLineDetector() override = default;

    QString backendName() const override { return "Halcon"; }

    bool detect(const cv::Mat& image,
               const LineDetectParams& params,
               std::vector<LineDetectResult>& results,
               QString& errorMsg) override;

    bool supportsMethod(const QString& method) const override;
    QStringList supportedMethods() const override;

private:
    bool detectWithEdgeFit(const cv::Mat& image, const LineDetectParams& params,
                          std::vector<LineDetectResult>& results, QString& errorMsg);
};

} // namespace Backend
} // namespace Algorithm
} // namespace VisionForge

#endif // USE_HALCON
