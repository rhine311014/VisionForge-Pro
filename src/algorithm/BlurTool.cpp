/**
 * @file BlurTool.cpp
 * @brief 图像模糊工具实现
 */

#include "algorithm/BlurTool.h"
#include "base/Logger.h"
#include "base/ImageMemoryPool.h"
#include <opencv2/imgproc.hpp>
#include <QElapsedTimer>

namespace VisionForge {
namespace Algorithm {

BlurTool::BlurTool(QObject* parent)
    : VisionTool(parent)
    , blurType_(Gaussian)
    , kernelSize_(5)
    , sigma_(0.0)
    , sigmaColor_(75.0)
    , sigmaSpace_(75.0)
{
    setDisplayName("图像模糊");
}

bool BlurTool::process(const Base::ImageData::Ptr& input, ToolResult& output)
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

    // 核大小检查
    if (kernelSize_ < 1 || kernelSize_ % 2 == 0) {
        output.success = false;
        output.errorMessage = QString("核大小必须是大于0的奇数，当前值：%1").arg(kernelSize_);
        setStatusText(output.errorMessage);
        emit processingFinished(false, timer.elapsed());
        return false;
    }

    try {
        const cv::Mat& inputMat = input->mat();

        // 双边滤波类型检查（需要提前检查以便提前返回）
        if (blurType_ == Bilateral && inputMat.depth() != CV_8U) {
            output.success = false;
            output.errorMessage = "双边滤波仅支持8位图像";
            setStatusText(output.errorMessage);
            emit processingFinished(false, timer.elapsed());
            return false;
        }

        // 预分配输出图像（模糊操作输出尺寸和类型与输入相同）
        output.outputImage = Base::ImageMemoryPool::instance().allocate(
            inputMat.cols, inputMat.rows, inputMat.type());
        if (!output.outputImage) {
            output.success = false;
            output.errorMessage = "内存分配失败";
            setStatusText(output.errorMessage);
            emit processingFinished(false, timer.elapsed());
            return false;
        }
        output.outputImage->setTimestamp(input->timestamp());

        cv::Mat& outputMat = output.outputImage->mat();

        // 根据模糊类型执行不同算法（直接写入输出Mat）
        switch (blurType_) {
        case Mean:
            // 均值模糊（盒式滤波）
            cv::blur(inputMat, outputMat, cv::Size(kernelSize_, kernelSize_));
            break;

        case Gaussian:
            // 高斯模糊
            cv::GaussianBlur(inputMat, outputMat,
                           cv::Size(kernelSize_, kernelSize_),
                           sigma_, sigma_);
            break;

        case Median:
            // 中值模糊
            cv::medianBlur(inputMat, outputMat, kernelSize_);
            break;

        case Bilateral:
            // 双边滤波
            cv::bilateralFilter(inputMat, outputMat,
                              kernelSize_,
                              sigmaColor_,
                              sigmaSpace_);
            break;

        default:
            cv::GaussianBlur(inputMat, outputMat,
                           cv::Size(kernelSize_, kernelSize_),
                           sigma_);
            break;
        }

        output.success = true;

        // 设置调试图像
        setDebugImage(output.outputImage);

        // 更新状态
        qint64 elapsed = timer.elapsed();
        QString typeStr;
        switch (blurType_) {
        case Mean: typeStr = "均值"; break;
        case Gaussian: typeStr = "高斯"; break;
        case Median: typeStr = "中值"; break;
        case Bilateral: typeStr = "双边"; break;
        }
        setStatusText(QString("%1模糊完成，核大小=%2，耗时 %3 ms")
                     .arg(typeStr).arg(kernelSize_).arg(elapsed));

        emit processingFinished(true, elapsed);

        LOG_DEBUG(QString("模糊处理: %1x%2, 类型=%3, 核=%4, 耗时=%5ms")
            .arg(input->width()).arg(input->height())
            .arg(typeStr).arg(kernelSize_).arg(elapsed));

        return true;

    } catch (const cv::Exception& e) {
        output.success = false;
        output.errorMessage = QString("OpenCV异常: %1").arg(e.what());
        setStatusText(output.errorMessage);
        LOG_ERROR(output.errorMessage);
        emit processingFinished(false, timer.elapsed());
        return false;
    } catch (const std::exception& e) {
        output.success = false;
        output.errorMessage = QString("模糊处理异常: %1").arg(e.what());
        setStatusText(output.errorMessage);
        LOG_ERROR(output.errorMessage);
        emit processingFinished(false, timer.elapsed());
        return false;
    }
}

QJsonObject BlurTool::serializeParams() const
{
    QJsonObject json;
    json["enabled"] = enabled_;
    json["displayName"] = displayName_;
    json["blurType"] = static_cast<int>(blurType_);
    json["kernelSize"] = kernelSize_;
    json["sigma"] = sigma_;
    json["sigmaColor"] = sigmaColor_;
    json["sigmaSpace"] = sigmaSpace_;
    return json;
}

void BlurTool::deserializeParams(const QJsonObject& json)
{
    if (json.contains("enabled")) {
        enabled_ = json["enabled"].toBool();
    }

    if (json.contains("displayName")) {
        displayName_ = json["displayName"].toString();
    }

    if (json.contains("blurType")) {
        blurType_ = static_cast<BlurType>(json["blurType"].toInt());
    }

    if (json.contains("kernelSize")) {
        kernelSize_ = json["kernelSize"].toInt();
    }

    if (json.contains("sigma")) {
        sigma_ = json["sigma"].toDouble();
    }

    if (json.contains("sigmaColor")) {
        sigmaColor_ = json["sigmaColor"].toDouble();
    }

    if (json.contains("sigmaSpace")) {
        sigmaSpace_ = json["sigmaSpace"].toDouble();
    }

    emit paramChanged();
}

void BlurTool::setBlurType(BlurType type)
{
    if (blurType_ != type) {
        blurType_ = type;
        emit paramChanged();
    }
}

void BlurTool::setKernelSize(int size)
{
    if (kernelSize_ != size) {
        kernelSize_ = size;
        emit paramChanged();
    }
}

void BlurTool::setSigma(double sigma)
{
    if (sigma_ != sigma) {
        sigma_ = sigma;
        emit paramChanged();
    }
}

void BlurTool::setSigmaColor(double sigmaColor)
{
    if (sigmaColor_ != sigmaColor) {
        sigmaColor_ = sigmaColor;
        emit paramChanged();
    }
}

void BlurTool::setSigmaSpace(double sigmaSpace)
{
    if (sigmaSpace_ != sigmaSpace) {
        sigmaSpace_ = sigmaSpace;
        emit paramChanged();
    }
}

} // namespace Algorithm
} // namespace VisionForge
