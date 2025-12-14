/**
 * @file BlurTool.h
 * @brief 图像模糊工具
 * @author VisionForge Team
 * @date 2025-12-15
 */

#pragma once

#include "algorithm/VisionTool.h"

namespace VisionForge {
namespace Algorithm {

/**
 * @brief 图像模糊工具
 *
 * 支持多种模糊算法：均值模糊、高斯模糊、中值模糊、双边滤波
 */
class BlurTool : public VisionTool {
    Q_OBJECT

public:
    /**
     * @brief 模糊类型
     */
    enum BlurType {
        Mean,           // 均值模糊（快速，但边缘模糊）
        Gaussian,       // 高斯模糊（常用，边缘保留较好）
        Median,         // 中值模糊（去椒盐噪声效果好）
        Bilateral       // 双边滤波（边缘保留最好，但速度慢）
    };
    Q_ENUM(BlurType)

    explicit BlurTool(QObject* parent = nullptr);

    // ========== VisionTool接口实现 ==========

    ToolType toolType() const override { return Blur; }
    QString toolName() const override { return tr("图像模糊"); }

    bool process(const Base::ImageData::Ptr& input, ToolResult& output) override;

    QJsonObject serializeParams() const override;
    void deserializeParams(const QJsonObject& json) override;

    // ========== 参数访问 ==========

    /**
     * @brief 设置模糊类型
     */
    void setBlurType(BlurType type);

    /**
     * @brief 获取模糊类型
     */
    BlurType blurType() const { return blurType_; }

    /**
     * @brief 设置核大小（必须是奇数，如3, 5, 7, 9...）
     */
    void setKernelSize(int size);

    /**
     * @brief 获取核大小
     */
    int kernelSize() const { return kernelSize_; }

    /**
     * @brief 设置高斯核sigma值（仅Gaussian模式）
     */
    void setSigma(double sigma);

    /**
     * @brief 获取sigma值
     */
    double sigma() const { return sigma_; }

    /**
     * @brief 设置双边滤波颜色空间sigma（仅Bilateral模式）
     */
    void setSigmaColor(double sigmaColor);

    /**
     * @brief 获取颜色sigma
     */
    double sigmaColor() const { return sigmaColor_; }

    /**
     * @brief 设置双边滤波空间sigma（仅Bilateral模式）
     */
    void setSigmaSpace(double sigmaSpace);

    /**
     * @brief 获取空间sigma
     */
    double sigmaSpace() const { return sigmaSpace_; }

private:
    BlurType blurType_;         // 模糊类型
    int kernelSize_;            // 核大小（奇数）
    double sigma_;              // 高斯sigma（0表示自动计算）
    double sigmaColor_;         // 双边滤波颜色sigma
    double sigmaSpace_;         // 双边滤波空间sigma
};

} // namespace Algorithm
} // namespace VisionForge
