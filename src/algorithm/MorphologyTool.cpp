/**
 * @file MorphologyTool.cpp
 * @brief 形态学处理工具实现
 */

#include "algorithm/MorphologyTool.h"
#include "base/Logger.h"
#include "base/ImageMemoryPool.h"
#include <opencv2/imgproc.hpp>
#include <QElapsedTimer>

namespace VisionForge {
namespace Algorithm {

MorphologyTool::MorphologyTool(QObject* parent)
    : VisionTool(parent)
    , morphType_(Erode)
    , kernelShape_(Rect)
    , kernelWidth_(3)
    , kernelHeight_(3)
    , iterations_(1)
    , anchorX_(-1)
    , anchorY_(-1)
{
    setDisplayName(toolName());
}

bool MorphologyTool::process(const Base::ImageData::Ptr& input, ToolResult& output)
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

    // 创建结构元素
    int shape = cv::MORPH_RECT;
    switch (kernelShape_) {
    case Rect:
        shape = cv::MORPH_RECT;
        break;
    case Cross:
        shape = cv::MORPH_CROSS;
        break;
    case Ellipse:
        shape = cv::MORPH_ELLIPSE;
        break;
    }

    cv::Mat kernel = cv::getStructuringElement(
        shape,
        cv::Size(kernelWidth_, kernelHeight_),
        cv::Point(anchorX_, anchorY_)
    );

    try {
        switch (morphType_) {
        case Erode:
            cv::erode(src, result, kernel, cv::Point(-1, -1), iterations_);
            break;

        case Dilate:
            cv::dilate(src, result, kernel, cv::Point(-1, -1), iterations_);
            break;

        case Open:
            cv::morphologyEx(src, result, cv::MORPH_OPEN, kernel,
                           cv::Point(-1, -1), iterations_);
            break;

        case Close:
            cv::morphologyEx(src, result, cv::MORPH_CLOSE, kernel,
                           cv::Point(-1, -1), iterations_);
            break;

        case Gradient:
            cv::morphologyEx(src, result, cv::MORPH_GRADIENT, kernel,
                           cv::Point(-1, -1), iterations_);
            break;

        case TopHat:
            cv::morphologyEx(src, result, cv::MORPH_TOPHAT, kernel,
                           cv::Point(-1, -1), iterations_);
            break;

        case BlackHat:
            cv::morphologyEx(src, result, cv::MORPH_BLACKHAT, kernel,
                           cv::Point(-1, -1), iterations_);
            break;
        }
    } catch (const cv::Exception& e) {
        output.success = false;
        output.errorMessage = QString("形态学处理失败: %1").arg(e.what());
        LOG_ERROR(output.errorMessage);
        return false;
    }

    output.outputImage = Base::ImageMemoryPool::instance().allocate(
        result.cols, result.rows, result.type());
    if (output.outputImage) {
        result.copyTo(output.outputImage->mat());
    }
    output.success = true;
    output.executionTime = timer.elapsed();

    // 记录参数信息
    output.setValue("morphType", static_cast<int>(morphType_));
    output.setValue("kernelSize", QString("%1x%2").arg(kernelWidth_).arg(kernelHeight_));
    output.setValue("iterations", iterations_);

    setDebugImage(output.outputImage);

    static const char* morphNames[] = {
        "腐蚀", "膨胀", "开运算", "闭运算", "梯度", "顶帽", "黑帽"
    };
    setStatusText(QString("%1 %2x%3 迭代%4次")
        .arg(morphNames[morphType_])
        .arg(kernelWidth_).arg(kernelHeight_)
        .arg(iterations_));

    LOG_DEBUG(QString("形态学处理完成，耗时: %1ms").arg(output.executionTime));

    return true;
}

QJsonObject MorphologyTool::serializeParams() const
{
    QJsonObject json;
    json["morphType"] = static_cast<int>(morphType_);
    json["kernelShape"] = static_cast<int>(kernelShape_);
    json["kernelWidth"] = kernelWidth_;
    json["kernelHeight"] = kernelHeight_;
    json["iterations"] = iterations_;
    json["anchorX"] = anchorX_;
    json["anchorY"] = anchorY_;
    return json;
}

void MorphologyTool::deserializeParams(const QJsonObject& json)
{
    morphType_ = static_cast<MorphType>(json.value("morphType").toInt(static_cast<int>(Erode)));
    kernelShape_ = static_cast<KernelShape>(json.value("kernelShape").toInt(static_cast<int>(Rect)));
    kernelWidth_ = json.value("kernelWidth").toInt(3);
    kernelHeight_ = json.value("kernelHeight").toInt(3);
    iterations_ = json.value("iterations").toInt(1);
    anchorX_ = json.value("anchorX").toInt(-1);
    anchorY_ = json.value("anchorY").toInt(-1);
}

void MorphologyTool::setMorphType(MorphType type)
{
    if (morphType_ != type) {
        morphType_ = type;
        emit paramChanged();
    }
}

void MorphologyTool::setKernelShape(KernelShape shape)
{
    if (kernelShape_ != shape) {
        kernelShape_ = shape;
        emit paramChanged();
    }
}

void MorphologyTool::setKernelWidth(int width)
{
    width = qBound(1, width, 99);
    if (kernelWidth_ != width) {
        kernelWidth_ = width;
        emit paramChanged();
    }
}

void MorphologyTool::setKernelHeight(int height)
{
    height = qBound(1, height, 99);
    if (kernelHeight_ != height) {
        kernelHeight_ = height;
        emit paramChanged();
    }
}

void MorphologyTool::setIterations(int iterations)
{
    iterations = qBound(1, iterations, 100);
    if (iterations_ != iterations) {
        iterations_ = iterations;
        emit paramChanged();
    }
}

void MorphologyTool::setAnchorX(int x)
{
    if (anchorX_ != x) {
        anchorX_ = x;
        emit paramChanged();
    }
}

void MorphologyTool::setAnchorY(int y)
{
    if (anchorY_ != y) {
        anchorY_ = y;
        emit paramChanged();
    }
}

} // namespace Algorithm
} // namespace VisionForge
