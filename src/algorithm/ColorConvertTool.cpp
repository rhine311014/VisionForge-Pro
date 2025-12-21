/**
 * @file ColorConvertTool.cpp
 * @brief 颜色空间转换工具实现
 */

#include "algorithm/ColorConvertTool.h"
#include "base/Logger.h"
#include "base/ImageMemoryPool.h"
#include <opencv2/imgproc.hpp>
#include <QElapsedTimer>

namespace VisionForge {
namespace Algorithm {

ColorConvertTool::ColorConvertTool(QObject* parent)
    : VisionTool(parent)
    , sourceSpace_(BGR)
    , targetSpace_(HSV)
    , extractChannel_(false)
    , channelIndex_(0)
{
    setDisplayName(toolName());
}

bool ColorConvertTool::process(const Base::ImageData::Ptr& input, ToolResult& output)
{
    if (!input || input->isEmpty()) {
        output.success = false;
        output.errorMessage = "输入图像为空";
        return false;
    }

    QElapsedTimer timer;
    timer.start();

    cv::Mat src = input->mat();
    cv::Mat result;

    try {
        // 如果源和目标相同，直接复制
        if (sourceSpace_ == targetSpace_) {
            result = src.clone();
        }
        // 转换到灰度
        else if (targetSpace_ == Grayscale) {
            if (src.channels() == 1) {
                result = src.clone();
            } else {
                int code = (sourceSpace_ == RGB) ? cv::COLOR_RGB2GRAY : cv::COLOR_BGR2GRAY;
                cv::cvtColor(src, result, code);
            }
        }
        // 从灰度转换（需要先转为BGR）
        else if (sourceSpace_ == Grayscale) {
            cv::Mat bgr;
            cv::cvtColor(src, bgr, cv::COLOR_GRAY2BGR);
            int code = getConversionCode(BGR, targetSpace_);
            if (code >= 0) {
                cv::cvtColor(bgr, result, code);
            } else {
                result = bgr;
            }
        }
        // 其他转换
        else {
            int code = getConversionCode(sourceSpace_, targetSpace_);
            if (code >= 0) {
                cv::cvtColor(src, result, code);
            } else {
                // 两步转换：先转BGR，再转目标
                cv::Mat bgr;
                int code1 = getConversionCode(sourceSpace_, BGR);
                if (code1 >= 0) {
                    cv::cvtColor(src, bgr, code1);
                } else {
                    bgr = src.clone();
                }

                int code2 = getConversionCode(BGR, targetSpace_);
                if (code2 >= 0) {
                    cv::cvtColor(bgr, result, code2);
                } else {
                    result = bgr;
                }
            }
        }

        // 提取单通道
        if (extractChannel_ && result.channels() > 1) {
            std::vector<cv::Mat> channels;
            cv::split(result, channels);

            int idx = std::min(channelIndex_, static_cast<int>(channels.size()) - 1);
            idx = std::max(0, idx);
            result = channels[idx].clone();
        }

        output.success = true;
        output.executionTime = timer.elapsed();
        output.outputImage = Base::ImageMemoryPool::instance().allocate(
            result.cols, result.rows, result.type());
        if (output.outputImage) {
            result.copyTo(output.outputImage->mat());
        }

        // 设置结果数据
        output.setValue("sourceSpace", colorSpaceName(sourceSpace_));
        output.setValue("targetSpace", colorSpaceName(targetSpace_));
        output.setValue("channels", result.channels());
        if (extractChannel_) {
            output.setValue("extractedChannel", channelName(targetSpace_, channelIndex_));
        }

        setDebugImage(output.outputImage);
        setStatusText(QString("%1 -> %2").arg(colorSpaceName(sourceSpace_))
                     .arg(colorSpaceName(targetSpace_)));

        LOG_DEBUG(QString("颜色转换完成: %1 -> %2, 耗时: %3ms")
                 .arg(colorSpaceName(sourceSpace_))
                 .arg(colorSpaceName(targetSpace_))
                 .arg(output.executionTime));

        return true;

    } catch (const cv::Exception& e) {
        output.success = false;
        output.errorMessage = QString("颜色转换失败: %1").arg(e.what());
        LOG_ERROR(output.errorMessage);
        return false;
    }
}

int ColorConvertTool::getConversionCode(ColorSpace from, ColorSpace to) const
{
    // BGR作为中间格式
    if (from == BGR) {
        switch (to) {
        case RGB:   return cv::COLOR_BGR2RGB;
        case HSV:   return cv::COLOR_BGR2HSV;
        case HLS:   return cv::COLOR_BGR2HLS;
        case Lab:   return cv::COLOR_BGR2Lab;
        case YCrCb: return cv::COLOR_BGR2YCrCb;
        case XYZ:   return cv::COLOR_BGR2XYZ;
        case Grayscale: return cv::COLOR_BGR2GRAY;
        default: return -1;
        }
    }
    else if (from == RGB) {
        switch (to) {
        case BGR:   return cv::COLOR_RGB2BGR;
        case HSV:   return cv::COLOR_RGB2HSV;
        case HLS:   return cv::COLOR_RGB2HLS;
        case Lab:   return cv::COLOR_RGB2Lab;
        case YCrCb: return cv::COLOR_RGB2YCrCb;
        case XYZ:   return cv::COLOR_RGB2XYZ;
        case Grayscale: return cv::COLOR_RGB2GRAY;
        default: return -1;
        }
    }
    else if (from == HSV) {
        switch (to) {
        case BGR: return cv::COLOR_HSV2BGR;
        case RGB: return cv::COLOR_HSV2RGB;
        default: return -1;
        }
    }
    else if (from == HLS) {
        switch (to) {
        case BGR: return cv::COLOR_HLS2BGR;
        case RGB: return cv::COLOR_HLS2RGB;
        default: return -1;
        }
    }
    else if (from == Lab) {
        switch (to) {
        case BGR: return cv::COLOR_Lab2BGR;
        case RGB: return cv::COLOR_Lab2RGB;
        default: return -1;
        }
    }
    else if (from == YCrCb) {
        switch (to) {
        case BGR: return cv::COLOR_YCrCb2BGR;
        case RGB: return cv::COLOR_YCrCb2RGB;
        default: return -1;
        }
    }
    else if (from == XYZ) {
        switch (to) {
        case BGR: return cv::COLOR_XYZ2BGR;
        case RGB: return cv::COLOR_XYZ2RGB;
        default: return -1;
        }
    }

    return -1;
}

QJsonObject ColorConvertTool::serializeParams() const
{
    QJsonObject json;
    json["sourceSpace"] = static_cast<int>(sourceSpace_);
    json["targetSpace"] = static_cast<int>(targetSpace_);
    json["extractChannel"] = extractChannel_;
    json["channelIndex"] = channelIndex_;
    return json;
}

void ColorConvertTool::deserializeParams(const QJsonObject& json)
{
    sourceSpace_ = static_cast<ColorSpace>(json.value("sourceSpace").toInt(static_cast<int>(BGR)));
    targetSpace_ = static_cast<ColorSpace>(json.value("targetSpace").toInt(static_cast<int>(HSV)));
    extractChannel_ = json.value("extractChannel").toBool(false);
    channelIndex_ = json.value("channelIndex").toInt(0);
}

void ColorConvertTool::setSourceSpace(ColorSpace space)
{
    if (sourceSpace_ != space) {
        sourceSpace_ = space;
        emit paramChanged();
    }
}

void ColorConvertTool::setTargetSpace(ColorSpace space)
{
    if (targetSpace_ != space) {
        targetSpace_ = space;
        emit paramChanged();
    }
}

void ColorConvertTool::setExtractChannel(bool extract)
{
    if (extractChannel_ != extract) {
        extractChannel_ = extract;
        emit paramChanged();
    }
}

void ColorConvertTool::setChannelIndex(int index)
{
    if (channelIndex_ != index) {
        channelIndex_ = index;
        emit paramChanged();
    }
}

QString ColorConvertTool::colorSpaceName(ColorSpace space)
{
    switch (space) {
    case BGR:       return "BGR";
    case RGB:       return "RGB";
    case HSV:       return "HSV";
    case HLS:       return "HLS";
    case Lab:       return "Lab";
    case YCrCb:     return "YCrCb";
    case XYZ:       return "XYZ";
    case Grayscale: return "Gray";
    default:        return "Unknown";
    }
}

int ColorConvertTool::channelCount(ColorSpace space)
{
    if (space == Grayscale) return 1;
    return 3;
}

QString ColorConvertTool::channelName(ColorSpace space, int channel)
{
    switch (space) {
    case BGR:
        if (channel == 0) return "B";
        if (channel == 1) return "G";
        if (channel == 2) return "R";
        break;
    case RGB:
        if (channel == 0) return "R";
        if (channel == 1) return "G";
        if (channel == 2) return "B";
        break;
    case HSV:
        if (channel == 0) return "H (色相)";
        if (channel == 1) return "S (饱和度)";
        if (channel == 2) return "V (明度)";
        break;
    case HLS:
        if (channel == 0) return "H (色相)";
        if (channel == 1) return "L (亮度)";
        if (channel == 2) return "S (饱和度)";
        break;
    case Lab:
        if (channel == 0) return "L (亮度)";
        if (channel == 1) return "a (绿-红)";
        if (channel == 2) return "b (蓝-黄)";
        break;
    case YCrCb:
        if (channel == 0) return "Y (亮度)";
        if (channel == 1) return "Cr (红色差)";
        if (channel == 2) return "Cb (蓝色差)";
        break;
    case XYZ:
        if (channel == 0) return "X";
        if (channel == 1) return "Y";
        if (channel == 2) return "Z";
        break;
    case Grayscale:
        return "Gray";
    default:
        break;
    }
    return QString("Channel %1").arg(channel);
}

} // namespace Algorithm
} // namespace VisionForge
