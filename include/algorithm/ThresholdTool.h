/**
 * @file ThresholdTool.h
 * @brief 图像二值化工具
 * @author VisionForge Team
 * @date 2025-12-15
 */

#pragma once

#include "algorithm/VisionTool.h"

namespace VisionForge {
namespace Algorithm {

/**
 * @brief 图像二值化工具
 *
 * 支持多种阈值算法：固定阈值、自适应阈值、OTSU、Triangle等
 */
class ThresholdTool : public VisionTool {
    Q_OBJECT

public:
    /**
     * @brief 阈值类型
     */
    enum ThresholdType {
        Binary,             // 二值化：大于阈值=255，否则=0
        BinaryInv,          // 反二值化：大于阈值=0，否则=255
        Truncate,           // 截断：大于阈值=阈值，否则=原值
        ToZero,             // 归零：大于阈值=原值，否则=0
        ToZeroInv           // 反归零：大于阈值=0，否则=原值
    };
    Q_ENUM(ThresholdType)

    /**
     * @brief 阈值计算方法
     */
    enum ThresholdMethod {
        Manual,             // 手动设置阈值
        OTSU,               // OTSU自动阈值（最大类间方差法）
        Triangle,           // Triangle自动阈值（适合单峰分布）
        AdaptiveMean,       // 自适应均值阈值
        AdaptiveGaussian    // 自适应高斯阈值
    };
    Q_ENUM(ThresholdMethod)

    explicit ThresholdTool(QObject* parent = nullptr);

    // ========== VisionTool接口实现 ==========

    ToolType toolType() const override { return VisionTool::Binary; }
    QString toolName() const override { return tr("图像二值化"); }

    bool process(const Base::ImageData::Ptr& input, ToolResult& output) override;

    QJsonObject serializeParams() const override;
    void deserializeParams(const QJsonObject& json) override;

    // ========== 参数访问 ==========

    /**
     * @brief 设置阈值类型
     */
    void setThresholdType(ThresholdType type);

    /**
     * @brief 获取阈值类型
     */
    ThresholdType thresholdType() const { return thresholdType_; }

    /**
     * @brief 设置阈值方法
     */
    void setThresholdMethod(ThresholdMethod method);

    /**
     * @brief 获取阈值方法
     */
    ThresholdMethod thresholdMethod() const { return thresholdMethod_; }

    /**
     * @brief 设置手动阈值（0-255）
     */
    void setThreshold(int threshold);

    /**
     * @brief 获取阈值
     */
    int threshold() const { return threshold_; }

    /**
     * @brief 设置最大值（用于Binary和BinaryInv）
     */
    void setMaxValue(int maxValue);

    /**
     * @brief 获取最大值
     */
    int maxValue() const { return maxValue_; }

    /**
     * @brief 设置自适应块大小（奇数，仅自适应方法）
     */
    void setBlockSize(int blockSize);

    /**
     * @brief 获取块大小
     */
    int blockSize() const { return blockSize_; }

    /**
     * @brief 设置自适应常数C（从均值或加权均值中减去的值）
     */
    void setConstantC(double constantC);

    /**
     * @brief 获取常数C
     */
    double constantC() const { return constantC_; }

    /**
     * @brief 获取最后计算的自动阈值（仅OTSU和Triangle）
     */
    int getComputedThreshold() const { return computedThreshold_; }

private:
    ThresholdType thresholdType_;       // 阈值类型
    ThresholdMethod thresholdMethod_;   // 阈值方法
    int threshold_;                     // 手动阈值
    int maxValue_;                      // 最大值
    int blockSize_;                     // 自适应块大小
    double constantC_;                  // 自适应常数C
    int computedThreshold_;             // 计算出的自动阈值
};

} // namespace Algorithm
} // namespace VisionForge
