/**
 * @file HalconCircleDetector.h
 * @brief Halcon圆检测后端实现
 * @details 使用Halcon库实现高精度圆检测，支持边缘拟合和Blob拟合
 * @author VisionForge Team
 * @date 2025-12-25
 */

#pragma once

#include "algorithm/backend/ICircleDetector.h"

#ifdef USE_HALCON

namespace VisionForge {
namespace Algorithm {
namespace Backend {

/**
 * @brief Halcon检测方法枚举
 */
enum class HalconCircleMethod {
    EdgeFit,    // 边缘拟合（亚像素精度）
    BlobFit     // Blob区域拟合
};

/**
 * @brief Halcon圆检测后端实现
 *
 * 使用Halcon实现高精度圆检测，支持亚像素边缘拟合和Blob区域分析。
 */
class HalconCircleDetector : public ICircleDetector {
public:
    HalconCircleDetector();
    ~HalconCircleDetector() override = default;

    // ========== ICircleDetector 接口实现 ==========

    QString backendName() const override { return "Halcon"; }

    bool detect(const cv::Mat& image,
               const CircleDetectParams& params,
               std::vector<CircleDetectResult>& results,
               QString& errorMsg) override;

    bool supportsMethod(const QString& method) const override;

    QStringList supportedMethods() const override;

    // ========== Halcon特有接口 ==========

    /**
     * @brief 设置检测方法
     * @param method 检测方法
     */
    void setMethod(HalconCircleMethod method) { method_ = method; }

    /**
     * @brief 获取当前检测方法
     */
    HalconCircleMethod method() const { return method_; }

private:
    /**
     * @brief 使用边缘拟合检测圆
     */
    bool detectWithEdgeFit(const cv::Mat& image,
                          const CircleDetectParams& params,
                          std::vector<CircleDetectResult>& results,
                          QString& errorMsg);

    /**
     * @brief 使用Blob拟合检测圆
     */
    bool detectWithBlobFit(const cv::Mat& image,
                          const CircleDetectParams& params,
                          std::vector<CircleDetectResult>& results,
                          QString& errorMsg);

private:
    HalconCircleMethod method_;
};

} // namespace Backend
} // namespace Algorithm
} // namespace VisionForge

#endif // USE_HALCON
