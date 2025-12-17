/**
 * @file MorphologyTool.h
 * @brief 形态学处理工具
 * @author VisionForge Team
 * @date 2025-12-17
 */

#pragma once

#include "algorithm/VisionTool.h"

namespace VisionForge {
namespace Algorithm {

/**
 * @brief 形态学处理工具
 *
 * 支持多种形态学操作：腐蚀、膨胀、开运算、闭运算、梯度、顶帽、黑帽
 */
class MorphologyTool : public VisionTool {
    Q_OBJECT

public:
    /**
     * @brief 形态学操作类型
     */
    enum MorphType {
        Erode,          // 腐蚀（缩小白色区域）
        Dilate,         // 膨胀（扩大白色区域）
        Open,           // 开运算（先腐蚀后膨胀，去除小白点）
        Close,          // 闭运算（先膨胀后腐蚀，填充小黑洞）
        Gradient,       // 形态学梯度（膨胀-腐蚀，提取边缘）
        TopHat,         // 顶帽变换（原图-开运算，提取亮细节）
        BlackHat        // 黑帽变换（闭运算-原图，提取暗细节）
    };
    Q_ENUM(MorphType)

    /**
     * @brief 结构元素形状
     */
    enum KernelShape {
        Rect,           // 矩形
        Cross,          // 十字形
        Ellipse         // 椭圆形
    };
    Q_ENUM(KernelShape)

    explicit MorphologyTool(QObject* parent = nullptr);

    // ========== VisionTool接口实现 ==========

    ToolType toolType() const override { return Morphology; }
    QString toolName() const override { return tr("形态学处理"); }

    bool process(const Base::ImageData::Ptr& input, ToolResult& output) override;

    QJsonObject serializeParams() const override;
    void deserializeParams(const QJsonObject& json) override;

    // ========== 参数访问 ==========

    /**
     * @brief 设置形态学操作类型
     */
    void setMorphType(MorphType type);
    MorphType morphType() const { return morphType_; }

    /**
     * @brief 设置结构元素形状
     */
    void setKernelShape(KernelShape shape);
    KernelShape kernelShape() const { return kernelShape_; }

    /**
     * @brief 设置结构元素宽度
     */
    void setKernelWidth(int width);
    int kernelWidth() const { return kernelWidth_; }

    /**
     * @brief 设置结构元素高度
     */
    void setKernelHeight(int height);
    int kernelHeight() const { return kernelHeight_; }

    /**
     * @brief 设置迭代次数
     */
    void setIterations(int iterations);
    int iterations() const { return iterations_; }

    /**
     * @brief 设置锚点X坐标（-1表示中心）
     */
    void setAnchorX(int x);
    int anchorX() const { return anchorX_; }

    /**
     * @brief 设置锚点Y坐标（-1表示中心）
     */
    void setAnchorY(int y);
    int anchorY() const { return anchorY_; }

private:
    MorphType morphType_;       // 形态学操作类型
    KernelShape kernelShape_;   // 结构元素形状
    int kernelWidth_;           // 结构元素宽度
    int kernelHeight_;          // 结构元素高度
    int iterations_;            // 迭代次数
    int anchorX_;               // 锚点X
    int anchorY_;               // 锚点Y
};

} // namespace Algorithm
} // namespace VisionForge
