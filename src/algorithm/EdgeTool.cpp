/**
 * @file EdgeTool.cpp
 * @brief 边缘检测工具实现
 */

#include "algorithm/EdgeTool.h"
#include "base/Logger.h"
#include "base/ImageMemoryPool.h"
#include <opencv2/imgproc.hpp>
#include <QElapsedTimer>

namespace VisionForge {
namespace Algorithm {

EdgeTool::EdgeTool(QObject* parent)
    : VisionTool(parent)
    , edgeType_(Canny)
    , threshold1_(50.0)
    , threshold2_(150.0)
    , kernelSize_(3)
    , direction_(Both)
    , l2Gradient_(false)
    , scale_(1.0)
    , delta_(0.0)
    , subPixelEnabled_(false)
    , subPixelMethod_(SubPixelMethod::QuadraticFit)
{
    setDisplayName(toolName());
}

bool EdgeTool::process(const Base::ImageData::Ptr& input, ToolResult& output)
{
    if (!input || input->isEmpty()) {
        output.success = false;
        output.errorMessage = "输入图像为空";
        return false;
    }

    QElapsedTimer timer;
    timer.start();

    cv::Mat src = input->mat();
    cv::Mat gray;

    // 转换为灰度图
    if (src.channels() == 3) {
        cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
    } else if (src.channels() == 4) {
        cv::cvtColor(src, gray, cv::COLOR_BGRA2GRAY);
    } else {
        gray = src.clone();
    }

    cv::Mat edges;

    try {
        switch (edgeType_) {
        case Canny: {
            cv::Canny(gray, edges, threshold1_, threshold2_, kernelSize_, l2Gradient_);
            break;
        }

        case Sobel: {
            cv::Mat gradX, gradY;
            int dx = (direction_ == Vertical) ? 0 : 1;
            int dy = (direction_ == Horizontal) ? 0 : 1;

            if (direction_ == Both) {
                cv::Sobel(gray, gradX, CV_16S, 1, 0, kernelSize_, scale_, delta_);
                cv::Sobel(gray, gradY, CV_16S, 0, 1, kernelSize_, scale_, delta_);
                cv::Mat absGradX, absGradY;
                cv::convertScaleAbs(gradX, absGradX);
                cv::convertScaleAbs(gradY, absGradY);
                cv::addWeighted(absGradX, 0.5, absGradY, 0.5, 0, edges);
            } else {
                cv::Mat grad;
                cv::Sobel(gray, grad, CV_16S, dx, dy, kernelSize_, scale_, delta_);
                cv::convertScaleAbs(grad, edges);
            }
            break;
        }

        case Laplacian: {
            cv::Mat lap;
            cv::Laplacian(gray, lap, CV_16S, kernelSize_, scale_, delta_);
            cv::convertScaleAbs(lap, edges);
            break;
        }

        case Scharr: {
            cv::Mat gradX, gradY;

            if (direction_ == Both) {
                cv::Scharr(gray, gradX, CV_16S, 1, 0, scale_, delta_);
                cv::Scharr(gray, gradY, CV_16S, 0, 1, scale_, delta_);
                cv::Mat absGradX, absGradY;
                cv::convertScaleAbs(gradX, absGradX);
                cv::convertScaleAbs(gradY, absGradY);
                cv::addWeighted(absGradX, 0.5, absGradY, 0.5, 0, edges);
            } else {
                int dx = (direction_ == Vertical) ? 0 : 1;
                int dy = (direction_ == Horizontal) ? 0 : 1;
                cv::Mat grad;
                cv::Scharr(gray, grad, CV_16S, dx, dy, scale_, delta_);
                cv::convertScaleAbs(grad, edges);
            }
            break;
        }
        }
    } catch (const cv::Exception& e) {
        output.success = false;
        output.errorMessage = QString("边缘检测失败: %1").arg(e.what());
        LOG_ERROR(output.errorMessage);
        return false;
    }

    output.outputImage = Base::ImageMemoryPool::instance().allocate(
        edges.cols, edges.rows, edges.type());
    if (output.outputImage) {
        edges.copyTo(output.outputImage->mat());
    }
    output.success = true;
    output.executionTime = timer.elapsed();

    // 统计边缘像素数量
    int edgePixels = cv::countNonZero(edges);
    output.setValue("edgePixels", edgePixels);
    output.setValue("edgeRatio", static_cast<double>(edgePixels) / (edges.rows * edges.cols));

    // 如果启用亚像素模式，进行亚像素精化
    subPixelEdgePoints_.clear();
    if (subPixelEnabled_ && edgePixels > 0) {
        SubPixelEdgeTool subPixelTool;
        SubPixelEdgeConfig subPixelConfig;
        subPixelConfig.method = subPixelMethod_;
        subPixelConfig.gradientThreshold = threshold1_;
        subPixelConfig.useSubPixel = true;
        subPixelConfig.cannyLow = static_cast<int>(threshold1_);
        subPixelConfig.cannyHigh = static_cast<int>(threshold2_);

        cv::Rect roi(0, 0, gray.cols, gray.rows);
        lastSubPixelResult_ = subPixelTool.detectSubPixelEdge(gray, roi, subPixelConfig);

        subPixelEdgePoints_ = lastSubPixelResult_.edgePoints;

        output.setValue("subPixelEdgeCount", lastSubPixelResult_.validPointCount);
        output.setValue("averageSubPixelShift", lastSubPixelResult_.averageSubPixelShift);

        setDebugImage(output.outputImage);
        setStatusText(QString("边缘像素: %1，亚像素点: %2，平均偏移: %.3f像素")
                     .arg(edgePixels)
                     .arg(lastSubPixelResult_.validPointCount)
                     .arg(lastSubPixelResult_.averageSubPixelShift));
    } else {
        setDebugImage(output.outputImage);
        setStatusText(QString("边缘像素: %1 (%.2f%%)").arg(edgePixels).arg(100.0 * edgePixels / (edges.rows * edges.cols)));
    }

    LOG_DEBUG(QString("边缘检测完成，耗时: %1ms").arg(output.executionTime));

    return true;
}

QJsonObject EdgeTool::serializeParams() const
{
    QJsonObject json;
    json["edgeType"] = static_cast<int>(edgeType_);
    json["threshold1"] = threshold1_;
    json["threshold2"] = threshold2_;
    json["kernelSize"] = kernelSize_;
    json["direction"] = static_cast<int>(direction_);
    json["l2Gradient"] = l2Gradient_;
    json["scale"] = scale_;
    json["delta"] = delta_;
    json["subPixelEnabled"] = subPixelEnabled_;
    json["subPixelMethod"] = static_cast<int>(subPixelMethod_);
    return json;
}

void EdgeTool::deserializeParams(const QJsonObject& json)
{
    edgeType_ = static_cast<EdgeType>(json.value("edgeType").toInt(static_cast<int>(Canny)));
    threshold1_ = json.value("threshold1").toDouble(50.0);
    threshold2_ = json.value("threshold2").toDouble(150.0);
    kernelSize_ = json.value("kernelSize").toInt(3);
    direction_ = static_cast<Direction>(json.value("direction").toInt(static_cast<int>(Both)));
    l2Gradient_ = json.value("l2Gradient").toBool(false);
    scale_ = json.value("scale").toDouble(1.0);
    delta_ = json.value("delta").toDouble(0.0);
    subPixelEnabled_ = json.value("subPixelEnabled").toBool(false);
    subPixelMethod_ = static_cast<SubPixelMethod>(
        json.value("subPixelMethod").toInt(static_cast<int>(SubPixelMethod::QuadraticFit)));
}

void EdgeTool::setEdgeType(EdgeType type)
{
    if (edgeType_ != type) {
        edgeType_ = type;
        emit paramChanged();
    }
}

void EdgeTool::setThreshold1(double threshold1)
{
    if (qAbs(threshold1_ - threshold1) > 0.001) {
        threshold1_ = qBound(0.0, threshold1, 255.0);
        emit paramChanged();
    }
}

void EdgeTool::setThreshold2(double threshold2)
{
    if (qAbs(threshold2_ - threshold2) > 0.001) {
        threshold2_ = qBound(0.0, threshold2, 255.0);
        emit paramChanged();
    }
}

void EdgeTool::setKernelSize(int size)
{
    // 核大小必须是奇数且在1-7之间
    if (size % 2 == 0) size++;
    size = qBound(1, size, 7);

    if (kernelSize_ != size) {
        kernelSize_ = size;
        emit paramChanged();
    }
}

void EdgeTool::setDirection(Direction dir)
{
    if (direction_ != dir) {
        direction_ = dir;
        emit paramChanged();
    }
}

void EdgeTool::setL2Gradient(bool use)
{
    if (l2Gradient_ != use) {
        l2Gradient_ = use;
        emit paramChanged();
    }
}

void EdgeTool::setScale(double scale)
{
    if (qAbs(scale_ - scale) > 0.001) {
        scale_ = scale;
        emit paramChanged();
    }
}

void EdgeTool::setDelta(double delta)
{
    if (qAbs(delta_ - delta) > 0.001) {
        delta_ = delta;
        emit paramChanged();
    }
}

void EdgeTool::setSubPixelEnabled(bool enabled)
{
    if (subPixelEnabled_ != enabled) {
        subPixelEnabled_ = enabled;
        emit paramChanged();
    }
}

void EdgeTool::setSubPixelMethod(SubPixelMethod method)
{
    if (subPixelMethod_ != method) {
        subPixelMethod_ = method;
        emit paramChanged();
    }
}

} // namespace Algorithm
} // namespace VisionForge
