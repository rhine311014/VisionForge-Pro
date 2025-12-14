/**
 * @file EdgeDetectionTool.cpp
 * @brief 边缘检测工具实现
 */

#include "algorithm/EdgeDetectionTool.h"
#include "base/Logger.h"
#include <opencv2/imgproc.hpp>
#include <QElapsedTimer>

namespace VisionForge {
namespace Algorithm {

EdgeDetectionTool::EdgeDetectionTool(QObject* parent)
    : VisionTool(parent)
    , detectionType_(Canny)
    , kernelSize_(3)
    , gradientDirection_(Both)
    , cannyLowThreshold_(50.0)
    , cannyHighThreshold_(150.0)
    , cannyApertureSize_(3)
    , useL2Gradient_(false)
{
    setDisplayName("边缘检测");
}

bool EdgeDetectionTool::process(const Base::ImageData::Ptr& input, ToolResult& output)
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

        cv::Mat edgeMat;

        // 根据检测类型执行不同算法
        switch (detectionType_) {
        case Sobel: {
            // Sobel算子
            cv::Mat gradX, gradY;
            int ddepth = CV_16S; // 使用16位有符号数防止溢出

            if (gradientDirection_ == Both || gradientDirection_ == X) {
                cv::Sobel(grayMat, gradX, ddepth, 1, 0, kernelSize_);
            }

            if (gradientDirection_ == Both || gradientDirection_ == Y) {
                cv::Sobel(grayMat, gradY, ddepth, 0, 1, kernelSize_);
            }

            // 计算梯度幅值
            if (gradientDirection_ == Both) {
                cv::Mat absGradX, absGradY;
                cv::convertScaleAbs(gradX, absGradX);
                cv::convertScaleAbs(gradY, absGradY);
                cv::addWeighted(absGradX, 0.5, absGradY, 0.5, 0, edgeMat);
            } else if (gradientDirection_ == X) {
                cv::convertScaleAbs(gradX, edgeMat);
            } else {
                cv::convertScaleAbs(gradY, edgeMat);
            }
            break;
        }

        case Scharr: {
            // Scharr算子（kernelSize参数无效，固定3x3）
            cv::Mat gradX, gradY;
            int ddepth = CV_16S;

            if (gradientDirection_ == Both || gradientDirection_ == X) {
                cv::Scharr(grayMat, gradX, ddepth, 1, 0);
            }

            if (gradientDirection_ == Both || gradientDirection_ == Y) {
                cv::Scharr(grayMat, gradY, ddepth, 0, 1);
            }

            // 计算梯度幅值
            if (gradientDirection_ == Both) {
                cv::Mat absGradX, absGradY;
                cv::convertScaleAbs(gradX, absGradX);
                cv::convertScaleAbs(gradY, absGradY);
                cv::addWeighted(absGradX, 0.5, absGradY, 0.5, 0, edgeMat);
            } else if (gradientDirection_ == X) {
                cv::convertScaleAbs(gradX, edgeMat);
            } else {
                cv::convertScaleAbs(gradY, edgeMat);
            }
            break;
        }

        case Laplacian: {
            // Laplacian算子（二阶导数）
            cv::Mat laplacianMat;
            int ddepth = CV_16S;

            cv::Laplacian(grayMat, laplacianMat, ddepth, kernelSize_);
            cv::convertScaleAbs(laplacianMat, edgeMat);
            break;
        }

        case Canny: {
            // Canny边缘检测
            cv::Canny(grayMat, edgeMat,
                     cannyLowThreshold_,
                     cannyHighThreshold_,
                     cannyApertureSize_,
                     useL2Gradient_);
            break;
        }

        default:
            cv::Canny(grayMat, edgeMat, 50, 150);
            break;
        }

        // 创建输出图像
        output.outputImage = std::make_shared<Base::ImageData>(edgeMat);
        output.outputImage->setTimestamp(input->timestamp());
        output.success = true;

        // 设置调试图像
        setDebugImage(output.outputImage);

        // 更新状态
        qint64 elapsed = timer.elapsed();
        QString typeStr;
        QString paramStr;
        switch (detectionType_) {
        case Sobel:
            typeStr = "Sobel";
            paramStr = QString("核=%1").arg(kernelSize_);
            break;
        case Scharr:
            typeStr = "Scharr";
            paramStr = "核=3×3";
            break;
        case Laplacian:
            typeStr = "Laplacian";
            paramStr = QString("核=%1").arg(kernelSize_);
            break;
        case Canny:
            typeStr = "Canny";
            paramStr = QString("阈值=%1~%2")
                      .arg(cannyLowThreshold_)
                      .arg(cannyHighThreshold_);
            break;
        }

        setStatusText(QString("%1边缘检测完成，%2，耗时 %3 ms")
                     .arg(typeStr).arg(paramStr).arg(elapsed));

        emit processingFinished(true, elapsed);

        LOG_DEBUG(QString("边缘检测: %1x%2, 类型=%3, %4, 耗时=%5ms")
            .arg(input->width()).arg(input->height())
            .arg(typeStr).arg(paramStr).arg(elapsed));

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
        output.errorMessage = QString("边缘检测异常: %1").arg(e.what());
        setStatusText(output.errorMessage);
        LOG_ERROR(output.errorMessage);
        emit processingFinished(false, timer.elapsed());
        return false;
    }
}

QJsonObject EdgeDetectionTool::serializeParams() const
{
    QJsonObject json;
    json["enabled"] = enabled_;
    json["displayName"] = displayName_;
    json["detectionType"] = static_cast<int>(detectionType_);
    json["kernelSize"] = kernelSize_;
    json["gradientDirection"] = static_cast<int>(gradientDirection_);
    json["cannyLowThreshold"] = cannyLowThreshold_;
    json["cannyHighThreshold"] = cannyHighThreshold_;
    json["cannyApertureSize"] = cannyApertureSize_;
    json["useL2Gradient"] = useL2Gradient_;
    return json;
}

void EdgeDetectionTool::deserializeParams(const QJsonObject& json)
{
    if (json.contains("enabled")) {
        enabled_ = json["enabled"].toBool();
    }

    if (json.contains("displayName")) {
        displayName_ = json["displayName"].toString();
    }

    if (json.contains("detectionType")) {
        detectionType_ = static_cast<EdgeDetectionType>(json["detectionType"].toInt());
    }

    if (json.contains("kernelSize")) {
        kernelSize_ = json["kernelSize"].toInt();
    }

    if (json.contains("gradientDirection")) {
        gradientDirection_ = static_cast<GradientDirection>(json["gradientDirection"].toInt());
    }

    if (json.contains("cannyLowThreshold")) {
        cannyLowThreshold_ = json["cannyLowThreshold"].toDouble();
    }

    if (json.contains("cannyHighThreshold")) {
        cannyHighThreshold_ = json["cannyHighThreshold"].toDouble();
    }

    if (json.contains("cannyApertureSize")) {
        cannyApertureSize_ = json["cannyApertureSize"].toInt();
    }

    if (json.contains("useL2Gradient")) {
        useL2Gradient_ = json["useL2Gradient"].toBool();
    }

    emit paramChanged();
}

void EdgeDetectionTool::setDetectionType(EdgeDetectionType type)
{
    if (detectionType_ != type) {
        detectionType_ = type;
        emit paramChanged();
    }
}

void EdgeDetectionTool::setKernelSize(int size)
{
    if (kernelSize_ != size) {
        kernelSize_ = size;
        emit paramChanged();
    }
}

void EdgeDetectionTool::setGradientDirection(GradientDirection direction)
{
    if (gradientDirection_ != direction) {
        gradientDirection_ = direction;
        emit paramChanged();
    }
}

void EdgeDetectionTool::setCannyLowThreshold(double threshold)
{
    if (cannyLowThreshold_ != threshold) {
        cannyLowThreshold_ = threshold;
        emit paramChanged();
    }
}

void EdgeDetectionTool::setCannyHighThreshold(double threshold)
{
    if (cannyHighThreshold_ != threshold) {
        cannyHighThreshold_ = threshold;
        emit paramChanged();
    }
}

void EdgeDetectionTool::setCannyApertureSize(int size)
{
    if (cannyApertureSize_ != size) {
        cannyApertureSize_ = size;
        emit paramChanged();
    }
}

void EdgeDetectionTool::setUseL2Gradient(bool useL2)
{
    if (useL2Gradient_ != useL2) {
        useL2Gradient_ = useL2;
        emit paramChanged();
    }
}

} // namespace Algorithm
} // namespace VisionForge
