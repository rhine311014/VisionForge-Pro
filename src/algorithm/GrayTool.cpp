/**
 * @file GrayTool.cpp
 * @brief 灰度转换工具实现
 */

#include "algorithm/GrayTool.h"
#include "base/Logger.h"
#include "base/ImageMemoryPool.h"
#include "base/PerformanceMonitor.h"
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

        // 预分配输出图像（灰度图输出尺寸与输入相同，类型为CV_8UC1）
        output.outputImage = Base::ImageMemoryPool::instance().allocate(
            inputMat.cols, inputMat.rows, CV_8UC1);
        if (!output.outputImage) {
            output.success = false;
            output.errorMessage = "内存分配失败";
            setStatusText(output.errorMessage);
            emit processingFinished(false, timer.elapsed());
            return false;
        }
        output.outputImage->setTimestamp(input->timestamp());

        cv::Mat& outputMat = output.outputImage->mat();

        // 执行转换（直接写入输出Mat）
        {
            // 性能计时
            PERF_TIMER("GrayTool::convert");

            switch (convertMode_) {
            case Average:
                convertAverageTo(inputMat, outputMat);
                break;

            case Weighted:
                convertWeightedTo(inputMat, outputMat);
                break;

            case Desaturation:
                convertDesaturationTo(inputMat, outputMat);
                break;

            case SingleChannel:
                convertSingleChannelTo(inputMat, outputMat);
                break;

            default:
                convertWeightedTo(inputMat, outputMat);
                break;
            }
        }

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

// ========== 私有转换方法（直接写入输出Mat）==========

void GrayTool::convertAverageTo(const cv::Mat& input, cv::Mat& output)
{
    std::vector<cv::Mat> channels;
    cv::split(input, channels);

    // 平均法: (B + G + R) / 3
    cv::Mat temp = (channels[0] + channels[1] + channels[2]) / 3;
    temp.copyTo(output);
}

void GrayTool::convertWeightedTo(const cv::Mat& input, cv::Mat& output)
{
    // 使用OpenCV标准加权法（直接写入输出）
    // Gray = 0.299*R + 0.587*G + 0.114*B
    cv::cvtColor(input, output, cv::COLOR_BGR2GRAY);
}

void GrayTool::convertDesaturationTo(const cv::Mat& input, cv::Mat& output)
{
#ifdef USE_OPENMP
    // OpenMP并行化加速
    #pragma omp parallel for
#endif
    for (int y = 0; y < input.rows; ++y) {
        const uchar* inputRow = input.ptr<uchar>(y);
        uchar* outputRow = output.ptr<uchar>(y);
        for (int x = 0; x < input.cols; ++x) {
            int idx = x * 3;
            uchar b = inputRow[idx];
            uchar g = inputRow[idx + 1];
            uchar r = inputRow[idx + 2];

            // 去饱和法: (max + min) / 2
            uchar maxVal = std::max({b, g, r});
            uchar minVal = std::min({b, g, r});
            outputRow[x] = (maxVal + minVal) / 2;
        }
    }
}

void GrayTool::convertSingleChannelTo(const cv::Mat& input, cv::Mat& output)
{
    int channelIdx = static_cast<int>(channel_);

    // 使用cv::extractChannel直接提取到输出，避免split+clone的开销
    cv::extractChannel(input, output, channelIdx);
}

} // namespace Algorithm
} // namespace VisionForge
