/**
 * @file ColorConvertTool.h
 * @brief 颜色空间转换工具
 * @details 支持多种颜色空间之间的转换
 * @author VisionForge Team
 * @date 2025-12-18
 */

#pragma once

#include "algorithm/VisionTool.h"

namespace VisionForge {
namespace Algorithm {

/**
 * @brief 颜色空间转换工具
 *
 * 支持BGR、RGB、HSV、HLS、Lab、YCrCb、Gray等颜色空间转换
 */
class ColorConvertTool : public VisionTool {
    Q_OBJECT

public:
    /**
     * @brief 颜色空间类型
     */
    enum ColorSpace {
        BGR,        // OpenCV默认格式
        RGB,        // 标准RGB
        HSV,        // 色相-饱和度-明度
        HLS,        // 色相-亮度-饱和度
        Lab,        // CIE Lab
        YCrCb,      // YCrCb (用于肤色检测)
        XYZ,        // CIE XYZ
        Grayscale   // 灰度
    };
    Q_ENUM(ColorSpace)

    explicit ColorConvertTool(QObject* parent = nullptr);

    // ========== VisionTool接口实现 ==========

    ToolType toolType() const override { return ColorConvert; }
    QString toolName() const override { return tr("颜色转换"); }

    bool process(const Base::ImageData::Ptr& input, ToolResult& output) override;

    QJsonObject serializeParams() const override;
    void deserializeParams(const QJsonObject& json) override;

    // ========== 参数设置 ==========

    /**
     * @brief 设置源颜色空间
     */
    void setSourceSpace(ColorSpace space);
    ColorSpace sourceSpace() const { return sourceSpace_; }

    /**
     * @brief 设置目标颜色空间
     */
    void setTargetSpace(ColorSpace space);
    ColorSpace targetSpace() const { return targetSpace_; }

    /**
     * @brief 设置是否提取单通道
     */
    void setExtractChannel(bool extract);
    bool extractChannel() const { return extractChannel_; }

    /**
     * @brief 设置要提取的通道索引
     */
    void setChannelIndex(int index);
    int channelIndex() const { return channelIndex_; }

    // ========== 静态辅助方法 ==========

    /**
     * @brief 获取颜色空间名称
     */
    static QString colorSpaceName(ColorSpace space);

    /**
     * @brief 获取颜色空间通道数
     */
    static int channelCount(ColorSpace space);

    /**
     * @brief 获取通道名称
     */
    static QString channelName(ColorSpace space, int channel);

private:
    /**
     * @brief 获取OpenCV转换代码
     */
    int getConversionCode(ColorSpace from, ColorSpace to) const;

private:
    ColorSpace sourceSpace_;        // 源颜色空间
    ColorSpace targetSpace_;        // 目标颜色空间
    bool extractChannel_;           // 是否提取单通道
    int channelIndex_;              // 提取的通道索引
};

} // namespace Algorithm
} // namespace VisionForge
