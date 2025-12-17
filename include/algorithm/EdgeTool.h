/**
 * @file EdgeTool.h
 * @brief 边缘检测工具
 * @author VisionForge Team
 * @date 2025-12-17
 */

#pragma once

#include "algorithm/VisionTool.h"

namespace VisionForge {
namespace Algorithm {

/**
 * @brief 边缘检测工具
 *
 * 支持多种边缘检测算法：Canny、Sobel、Laplacian、Scharr
 */
class EdgeTool : public VisionTool {
    Q_OBJECT

public:
    /**
     * @brief 边缘检测算法类型
     */
    enum EdgeType {
        Canny,          // Canny边缘检测（最常用，双阈值，边缘连续）
        Sobel,          // Sobel算子（梯度方向，可分离卷积）
        Laplacian,      // Laplacian算子（二阶导数，对噪声敏感）
        Scharr          // Scharr算子（比Sobel更精确）
    };
    Q_ENUM(EdgeType)

    /**
     * @brief Sobel/Scharr方向
     */
    enum Direction {
        Both,           // X和Y方向组合
        Horizontal,     // 水平方向（检测垂直边缘）
        Vertical        // 垂直方向（检测水平边缘）
    };
    Q_ENUM(Direction)

    explicit EdgeTool(QObject* parent = nullptr);

    // ========== VisionTool接口实现 ==========

    ToolType toolType() const override { return Edge; }
    QString toolName() const override { return tr("边缘检测"); }

    bool process(const Base::ImageData::Ptr& input, ToolResult& output) override;

    QJsonObject serializeParams() const override;
    void deserializeParams(const QJsonObject& json) override;

    // ========== 参数访问 ==========

    /**
     * @brief 设置边缘检测类型
     */
    void setEdgeType(EdgeType type);
    EdgeType edgeType() const { return edgeType_; }

    /**
     * @brief 设置Canny低阈值
     */
    void setThreshold1(double threshold1);
    double threshold1() const { return threshold1_; }

    /**
     * @brief 设置Canny高阈值
     */
    void setThreshold2(double threshold2);
    double threshold2() const { return threshold2_; }

    /**
     * @brief 设置Sobel核大小（1, 3, 5, 7）
     */
    void setKernelSize(int size);
    int kernelSize() const { return kernelSize_; }

    /**
     * @brief 设置Sobel/Scharr方向
     */
    void setDirection(Direction dir);
    Direction direction() const { return direction_; }

    /**
     * @brief 设置是否使用L2范数（Canny）
     */
    void setL2Gradient(bool use);
    bool l2Gradient() const { return l2Gradient_; }

    /**
     * @brief 设置缩放因子（Sobel/Laplacian）
     */
    void setScale(double scale);
    double scale() const { return scale_; }

    /**
     * @brief 设置偏移量（Sobel/Laplacian）
     */
    void setDelta(double delta);
    double delta() const { return delta_; }

private:
    EdgeType edgeType_;         // 边缘检测类型
    double threshold1_;         // Canny低阈值
    double threshold2_;         // Canny高阈值
    int kernelSize_;            // 核大小
    Direction direction_;       // 方向
    bool l2Gradient_;           // 是否使用L2范数
    double scale_;              // 缩放因子
    double delta_;              // 偏移量
};

} // namespace Algorithm
} // namespace VisionForge
