/**
 * @file EdgeDetectionTool.h
 * @brief 边缘检测工具
 * @author VisionForge Team
 * @date 2025-12-15
 */

#pragma once

#include "algorithm/VisionTool.h"

namespace VisionForge {
namespace Algorithm {

/**
 * @brief 边缘检测工具
 *
 * 支持多种边缘检测算法：Sobel、Scharr、Laplacian、Canny
 */
class EdgeDetectionTool : public VisionTool {
    Q_OBJECT

public:
    /**
     * @brief 边缘检测算法类型
     */
    enum EdgeDetectionType {
        Sobel,              // Sobel算子（一阶导数，检测梯度）
        Scharr,             // Scharr算子（Sobel的优化版本，精度更高）
        Laplacian,          // Laplacian算子（二阶导数，对噪声敏感）
        Canny               // Canny边缘检测（最优边缘检测，效果最好）
    };
    Q_ENUM(EdgeDetectionType)

    /**
     * @brief 梯度方向（仅Sobel和Scharr）
     */
    enum GradientDirection {
        Both,               // X和Y方向的梯度合成
        X,                  // 仅X方向（垂直边缘）
        Y                   // 仅Y方向（水平边缘）
    };
    Q_ENUM(GradientDirection)

    explicit EdgeDetectionTool(QObject* parent = nullptr);

    // ========== VisionTool接口实现 ==========

    ToolType toolType() const override { return FindEdge; }
    QString toolName() const override { return tr("边缘检测"); }

    bool process(const Base::ImageData::Ptr& input, ToolResult& output) override;

    QJsonObject serializeParams() const override;
    void deserializeParams(const QJsonObject& json) override;

    // ========== 参数访问 ==========

    /**
     * @brief 设置边缘检测类型
     */
    void setDetectionType(EdgeDetectionType type);

    /**
     * @brief 获取边缘检测类型
     */
    EdgeDetectionType detectionType() const { return detectionType_; }

    /**
     * @brief 设置核大小（Sobel和Scharr：1/3/5/7，Laplacian：1/3/5）
     */
    void setKernelSize(int size);

    /**
     * @brief 获取核大小
     */
    int kernelSize() const { return kernelSize_; }

    /**
     * @brief 设置梯度方向（仅Sobel和Scharr）
     */
    void setGradientDirection(GradientDirection direction);

    /**
     * @brief 获取梯度方向
     */
    GradientDirection gradientDirection() const { return gradientDirection_; }

    /**
     * @brief 设置Canny低阈值（仅Canny）
     */
    void setCannyLowThreshold(double threshold);

    /**
     * @brief 获取Canny低阈值
     */
    double cannyLowThreshold() const { return cannyLowThreshold_; }

    /**
     * @brief 设置Canny高阈值（仅Canny）
     */
    void setCannyHighThreshold(double threshold);

    /**
     * @brief 获取Canny高阈值
     */
    double cannyHighThreshold() const { return cannyHighThreshold_; }

    /**
     * @brief 设置Canny孔径大小（3/5/7，仅Canny）
     */
    void setCannyApertureSize(int size);

    /**
     * @brief 获取Canny孔径大小
     */
    int cannyApertureSize() const { return cannyApertureSize_; }

    /**
     * @brief 设置是否使用L2梯度（仅Canny）
     */
    void setUseL2Gradient(bool useL2);

    /**
     * @brief 获取是否使用L2梯度
     */
    bool useL2Gradient() const { return useL2Gradient_; }

private:
    EdgeDetectionType detectionType_;   // 检测类型
    int kernelSize_;                    // 核大小
    GradientDirection gradientDirection_; // 梯度方向
    double cannyLowThreshold_;          // Canny低阈值
    double cannyHighThreshold_;         // Canny高阈值
    int cannyApertureSize_;             // Canny孔径大小
    bool useL2Gradient_;                // 是否使用L2梯度
};

} // namespace Algorithm
} // namespace VisionForge
