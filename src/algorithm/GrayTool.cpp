/**
 * @file GrayTool.cpp
 * @brief 灰度转换工具实现
 */

#include "algorithm/GrayTool.h"
#include "base/Logger.h"
#include "base/ImageMemoryPool.h"
#include <opencv2/imgproc.hpp>
#include <QElapsedTimer>

namespace VisionForge {
namespace Algorithm {

GrayTool::GrayTool(QObject* parent)
    : VisionTool(parent)
    , convertMode_(Weighted)
    , channel_(Blue)
{
    setDisplayName("灰度转换");
}

bool GrayTool::process(const Base::ImageData::Ptr& input, ToolResult& output)
{
    QElapsedTimer timer;
    timer.start();

    emit processingStarted();

    // 参数检查
    if (!input || input->isEmpty()) {
        output.success = false;
        output.errorMessage = "输入图像为空";
        setStatusText(output.errorMessage);
        emit processingFinished(false, timer.elapsed());
        return false;
    }

    if (!enabled_) {
        // 工具未启用，直接传递输入
        output.success = true;
        output.outputImage = input;
        emit processingFinished(true, timer.elapsed());
        return true;
    }

    try {
        const cv::Mat& inputMat = input->mat();

        // 如果已经是灰度图，直接返回
        if (inputMat.channels() == 1) {
            output.success = true;
            output.outputImage = input;
            setStatusText("输入已是灰度图");
            emit processingFinished(true, timer.elapsed());
            return true;
        }

        // 执行转换
        cv::Mat grayMat;

        switch (convertMode_) {
        case Average:
            grayMat = convertAverage(inputMat);
            break;

        case Weighted:
            grayMat = convertWeighted(inputMat);
            break;

        case Desaturation:
            grayMat = convertDesaturation(inputMat);
            break;

        case SingleChannel:
            grayMat = convertSingleChannel(inputMat);
            break;

        default:
            grayMat = convertWeighted(inputMat);
            break;
        }

        // 创建输出图像
        output.outputImage = std::make_shared<Base::ImageData>(grayMat);
        output.outputImage->setTimestamp(input->timestamp());
        output.success = true;

        // 设置调试图像
        setDebugImage(output.outputImage);

        // 更新状态
        qint64 elapsed = timer.elapsed();
        setStatusText(QString("转换完成，耗时 %1 ms").arg(elapsed));

        emit processingFinished(true, elapsed);

        LOG_DEBUG(QString("灰度转换: %1x%2, 模式=%3, 耗时=%4ms")
            .arg(input->width()).arg(input->height())
            .arg(convertMode_).arg(elapsed));

        return true;

    } catch (const std::exception& e) {
        output.success = false;
        output.errorMessage = QString("灰度转换异常: %1").arg(e.what());
        setStatusText(output.errorMessage);

        LOG_ERROR(output.errorMessage);

        emit processingFinished(false, timer.elapsed());
        return false;
    }
}

QJsonObject GrayTool::serializeParams() const
{
    QJsonObject json;
    json["enabled"] = enabled_;
    json["displayName"] = displayName_;
    json["convertMode"] = static_cast<int>(convertMode_);
    json["channel"] = static_cast<int>(channel_);
    return json;
}

void GrayTool::deserializeParams(const QJsonObject& json)
{
    if (json.contains("enabled")) {
        enabled_ = json["enabled"].toBool();
    }

    if (json.contains("displayName")) {
        displayName_ = json["displayName"].toString();
    }

    if (json.contains("convertMode")) {
        convertMode_ = static_cast<ConvertMode>(json["convertMode"].toInt());
    }

    if (json.contains("channel")) {
        channel_ = static_cast<ChannelType>(json["channel"].toInt());
    }

    emit paramChanged();
}

void GrayTool::setConvertMode(ConvertMode mode)
{
    if (convertMode_ != mode) {
        convertMode_ = mode;
        emit paramChanged();
    }
}

void GrayTool::setChannel(ChannelType channel)
{
    if (channel_ != channel) {
        channel_ = channel;
        emit paramChanged();
    }
}

// ========== 私有转换方法 ==========

cv::Mat GrayTool::convertAverage(const cv::Mat& input)
{
    cv::Mat gray;
    std::vector<cv::Mat> channels;
    cv::split(input, channels);

    // 平均法: (B + G + R) / 3
    gray = (channels[0] + channels[1] + channels[2]) / 3;

    return gray;
}

cv::Mat GrayTool::convertWeighted(const cv::Mat& input)
{
    cv::Mat gray;

    // 使用OpenCV标准加权法
    // Gray = 0.299*R + 0.587*G + 0.114*B
    cv::cvtColor(input, gray, cv::COLOR_BGR2GRAY);

    return gray;
}

cv::Mat GrayTool::convertDesaturation(const cv::Mat& input)
{
    cv::Mat gray(input.rows, input.cols, CV_8UC1);

    for (int y = 0; y < input.rows; ++y) {
        for (int x = 0; x < input.cols; ++x) {
            cv::Vec3b pixel = input.at<cv::Vec3b>(y, x);

            // 去饱和法: (max + min) / 2
            uchar maxVal = std::max({pixel[0], pixel[1], pixel[2]});
            uchar minVal = std::min({pixel[0], pixel[1], pixel[2]});
            uchar grayVal = (maxVal + minVal) / 2;

            gray.at<uchar>(y, x) = grayVal;
        }
    }

    return gray;
}

cv::Mat GrayTool::convertSingleChannel(const cv::Mat& input)
{
    std::vector<cv::Mat> channels;
    cv::split(input, channels);

    // 返回指定通道
    return channels[static_cast<int>(channel_)].clone();
}

} // namespace Algorithm
} // namespace VisionForge
