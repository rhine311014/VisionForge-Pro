/**
 * @file ThresholdTool.cpp
 * @brief 图像二值化工具实现
 */

#include "algorithm/ThresholdTool.h"
#include "base/Logger.h"
#include <opencv2/imgproc.hpp>
#include <QElapsedTimer>

namespace VisionForge {
namespace Algorithm {

ThresholdTool::ThresholdTool(QObject* parent)
    : VisionTool(parent)
    , thresholdType_(Binary)
    , thresholdMethod_(Manual)
    , threshold_(128)
    , maxValue_(255)
    , blockSize_(11)
    , constantC_(2.0)
    , computedThreshold_(0)
{
    setDisplayName("图像二值化");
}

bool ThresholdTool::process(const Base::ImageData::Ptr& input, ToolResult& output)
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
        cv::Mat grayMat;

        // 转为灰度图（如果是彩色）
        if (inputMat.channels() == 3 || inputMat.channels() == 4) {
            cv::cvtColor(inputMat, grayMat, cv::COLOR_BGR2GRAY);
        } else if (inputMat.channels() == 1) {
            grayMat = inputMat;
        } else {
            output.success = false;
            output.errorMessage = QString("不支持的通道数：%1").arg(inputMat.channels());
            setStatusText(output.errorMessage);
            emit processingFinished(false, timer.elapsed());
            return false;
        }

        cv::Mat thresholdMat;

        // 根据阈值方法执行不同算法
        switch (thresholdMethod_) {
        case Manual: {
            // 手动阈值
            int cvType = 0;
            switch (thresholdType_) {
            case Binary: cvType = cv::THRESH_BINARY; break;
            case BinaryInv: cvType = cv::THRESH_BINARY_INV; break;
            case Truncate: cvType = cv::THRESH_TRUNC; break;
            case ToZero: cvType = cv::THRESH_TOZERO; break;
            case ToZeroInv: cvType = cv::THRESH_TOZERO_INV; break;
            }
            cv::threshold(grayMat, thresholdMat, threshold_, maxValue_, cvType);
            computedThreshold_ = threshold_;
            break;
        }

        case OTSU: {
            // OTSU自动阈值
            int cvType = cv::THRESH_OTSU;
            switch (thresholdType_) {
            case Binary: cvType |= cv::THRESH_BINARY; break;
            case BinaryInv: cvType |= cv::THRESH_BINARY_INV; break;
            case Truncate: cvType |= cv::THRESH_TRUNC; break;
            case ToZero: cvType |= cv::THRESH_TOZERO; break;
            case ToZeroInv: cvType |= cv::THRESH_TOZERO_INV; break;
            }
            computedThreshold_ = static_cast<int>(
                cv::threshold(grayMat, thresholdMat, 0, maxValue_, cvType)
            );
            break;
        }

        case Triangle: {
            // Triangle自动阈值
            int cvType = cv::THRESH_TRIANGLE;
            switch (thresholdType_) {
            case Binary: cvType |= cv::THRESH_BINARY; break;
            case BinaryInv: cvType |= cv::THRESH_BINARY_INV; break;
            case Truncate: cvType |= cv::THRESH_TRUNC; break;
            case ToZero: cvType |= cv::THRESH_TOZERO; break;
            case ToZeroInv: cvType |= cv::THRESH_TOZERO_INV; break;
            }
            computedThreshold_ = static_cast<int>(
                cv::threshold(grayMat, thresholdMat, 0, maxValue_, cvType)
            );
            break;
        }

        case AdaptiveMean: {
            // 自适应均值阈值（仅支持Binary和BinaryInv）
            int cvType = (thresholdType_ == BinaryInv) ?
                        cv::THRESH_BINARY_INV : cv::THRESH_BINARY;

            if (blockSize_ < 3 || blockSize_ % 2 == 0) {
                output.success = false;
                output.errorMessage = QString("块大小必须是大于等于3的奇数，当前值：%1").arg(blockSize_);
                setStatusText(output.errorMessage);
                emit processingFinished(false, timer.elapsed());
                return false;
            }

            cv::adaptiveThreshold(grayMat, thresholdMat,
                                maxValue_,
                                cv::ADAPTIVE_THRESH_MEAN_C,
                                cvType,
                                blockSize_,
                                constantC_);
            computedThreshold_ = -1; // 自适应阈值无单一值
            break;
        }

        case AdaptiveGaussian: {
            // 自适应高斯阈值（仅支持Binary和BinaryInv）
            int cvType = (thresholdType_ == BinaryInv) ?
                        cv::THRESH_BINARY_INV : cv::THRESH_BINARY;

            if (blockSize_ < 3 || blockSize_ % 2 == 0) {
                output.success = false;
                output.errorMessage = QString("块大小必须是大于等于3的奇数，当前值：%1").arg(blockSize_);
                setStatusText(output.errorMessage);
                emit processingFinished(false, timer.elapsed());
                return false;
            }

            cv::adaptiveThreshold(grayMat, thresholdMat,
                                maxValue_,
                                cv::ADAPTIVE_THRESH_GAUSSIAN_C,
                                cvType,
                                blockSize_,
                                constantC_);
            computedThreshold_ = -1; // 自适应阈值无单一值
            break;
        }

        default:
            cv::threshold(grayMat, thresholdMat, threshold_, maxValue_, cv::THRESH_BINARY);
            computedThreshold_ = threshold_;
            break;
        }

        // 创建输出图像
        output.outputImage = std::make_shared<Base::ImageData>(thresholdMat);
        output.outputImage->setTimestamp(input->timestamp());
        output.success = true;

        // 保存计算的阈值到结果
        if (computedThreshold_ >= 0) {
            output.setValue("computedThreshold", computedThreshold_);
        }

        // 设置调试图像
        setDebugImage(output.outputImage);

        // 更新状态
        qint64 elapsed = timer.elapsed();
        QString methodStr;
        switch (thresholdMethod_) {
        case Manual: methodStr = "手动"; break;
        case OTSU: methodStr = "OTSU"; break;
        case Triangle: methodStr = "Triangle"; break;
        case AdaptiveMean: methodStr = "自适应均值"; break;
        case AdaptiveGaussian: methodStr = "自适应高斯"; break;
        }

        QString statusMsg = QString("%1二值化完成").arg(methodStr);
        if (computedThreshold_ >= 0) {
            statusMsg += QString("，阈值=%1").arg(computedThreshold_);
        }
        statusMsg += QString("，耗时 %1 ms").arg(elapsed);
        setStatusText(statusMsg);

        emit processingFinished(true, elapsed);

        LOG_DEBUG(QString("二值化处理: %1x%2, 方法=%3, 阈值=%4, 耗时=%5ms")
            .arg(input->width()).arg(input->height())
            .arg(methodStr)
            .arg(computedThreshold_)
            .arg(elapsed));

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
        output.errorMessage = QString("二值化处理异常: %1").arg(e.what());
        setStatusText(output.errorMessage);
        LOG_ERROR(output.errorMessage);
        emit processingFinished(false, timer.elapsed());
        return false;
    }
}

QJsonObject ThresholdTool::serializeParams() const
{
    QJsonObject json;
    json["enabled"] = enabled_;
    json["displayName"] = displayName_;
    json["thresholdType"] = static_cast<int>(thresholdType_);
    json["thresholdMethod"] = static_cast<int>(thresholdMethod_);
    json["threshold"] = threshold_;
    json["maxValue"] = maxValue_;
    json["blockSize"] = blockSize_;
    json["constantC"] = constantC_;
    return json;
}

void ThresholdTool::deserializeParams(const QJsonObject& json)
{
    if (json.contains("enabled")) {
        enabled_ = json["enabled"].toBool();
    }

    if (json.contains("displayName")) {
        displayName_ = json["displayName"].toString();
    }

    if (json.contains("thresholdType")) {
        thresholdType_ = static_cast<ThresholdType>(json["thresholdType"].toInt());
    }

    if (json.contains("thresholdMethod")) {
        thresholdMethod_ = static_cast<ThresholdMethod>(json["thresholdMethod"].toInt());
    }

    if (json.contains("threshold")) {
        threshold_ = json["threshold"].toInt();
    }

    if (json.contains("maxValue")) {
        maxValue_ = json["maxValue"].toInt();
    }

    if (json.contains("blockSize")) {
        blockSize_ = json["blockSize"].toInt();
    }

    if (json.contains("constantC")) {
        constantC_ = json["constantC"].toDouble();
    }

    emit paramChanged();
}

void ThresholdTool::setThresholdType(ThresholdType type)
{
    if (thresholdType_ != type) {
        thresholdType_ = type;
        emit paramChanged();
    }
}

void ThresholdTool::setThresholdMethod(ThresholdMethod method)
{
    if (thresholdMethod_ != method) {
        thresholdMethod_ = method;
        emit paramChanged();
    }
}

void ThresholdTool::setThreshold(int threshold)
{
    if (threshold_ != threshold) {
        threshold_ = threshold;
        emit paramChanged();
    }
}

void ThresholdTool::setMaxValue(int maxValue)
{
    if (maxValue_ != maxValue) {
        maxValue_ = maxValue;
        emit paramChanged();
    }
}

void ThresholdTool::setBlockSize(int blockSize)
{
    if (blockSize_ != blockSize) {
        blockSize_ = blockSize;
        emit paramChanged();
    }
}

void ThresholdTool::setConstantC(double constantC)
{
    if (constantC_ != constantC) {
        constantC_ = constantC;
        emit paramChanged();
    }
}

} // namespace Algorithm
} // namespace VisionForge
