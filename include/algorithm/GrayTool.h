/**
 * @file GrayTool.h
 * @brief 灰度转换工具
 * @author VisionForge Team
 * @date 2025-12-14
 */

#pragma once

#include "algorithm/VisionTool.h"

namespace VisionForge {
namespace Algorithm {

/**
 * @brief 灰度转换工具
 *
 * 将彩色图像转换为灰度图像
 */
class GrayTool : public VisionTool {
    Q_OBJECT

public:
    /**
     * @brief 转换模式
     */
    enum ConvertMode {
        Average,        // 平均法 R+G+B/3
        Weighted,       // 加权法 0.299R + 0.587G + 0.114B (OpenCV默认)
        Desaturation,   // 去饱和法 (max+min)/2
        SingleChannel   // 单通道（蓝/绿/红）
    };
    Q_ENUM(ConvertMode)

    /**
     * @brief 单通道选择（仅SingleChannel模式）
     */
    enum ChannelType {
        Blue = 0,
        Green = 1,
        Red = 2
    };
    Q_ENUM(ChannelType)

    explicit GrayTool(QObject* parent = nullptr);

    // ========== VisionTool接口实现 ==========

    ToolType toolType() const override { return Gray; }
    QString toolName() const override { return tr("灰度转换"); }

    bool process(const Base::ImageData::Ptr& input, ToolResult& output) override;

    QJsonObject serializeParams() const override;
    void deserializeParams(const QJsonObject& json) override;

    // ========== 参数访问 ==========

    /**
     * @brief 设置转换模式
     */
    void setConvertMode(ConvertMode mode);

    /**
     * @brief 获取转换模式
     */
    ConvertMode convertMode() const { return convertMode_; }

    /**
     * @brief 设置通道（仅SingleChannel模式）
     */
    void setChannel(ChannelType channel);

    /**
     * @brief 获取通道
     */
    ChannelType channel() const { return channel_; }

private:
    // 转换方法（直接写入输出Mat，避免临时对象）
    void convertAverageTo(const cv::Mat& input, cv::Mat& output);
    void convertWeightedTo(const cv::Mat& input, cv::Mat& output);
    void convertDesaturationTo(const cv::Mat& input, cv::Mat& output);
    void convertSingleChannelTo(const cv::Mat& input, cv::Mat& output);

private:
    ConvertMode convertMode_;
    ChannelType channel_;
};

} // namespace Algorithm
} // namespace VisionForge
