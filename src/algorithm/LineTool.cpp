/**
 * @file LineTool.cpp
 * @brief 线检测工具实现
 * @details 使用抽象后端接口，无需 #ifdef USE_HALCON
 */

#include "algorithm/LineTool.h"
#include "algorithm/backend/OpenCVLineDetector.h"
#include "base/Logger.h"
#include "base/ImageMemoryPool.h"
#include <opencv2/imgproc.hpp>
#include <QElapsedTimer>
#include <QJsonArray>
#include <algorithm>
#include <cmath>

namespace VisionForge {
namespace Algorithm {

LineTool::LineTool(QObject* parent)
    : VisionTool(parent)
    , backend_(Auto)
    , method_(HoughLinesP)
    , minLength_(50.0)
    , maxGap_(10.0)
    , maxCount_(20)
    , rho_(1.0)
    , theta_(1.0)
    , threshold_(50)
    , cannyThreshold1_(50.0)
    , cannyThreshold2_(150.0)
{
    setDisplayName(toolName());
}

bool LineTool::process(const Base::ImageData::Ptr& input, ToolResult& output)
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

    lines_.clear();

    // 确保检测器已创建
    ensureDetector();

    // 使用抽象接口进行检测（无需 #ifdef）
    std::vector<Backend::LineDetectResult> backendResults;
    QString errorMsg;
    bool success = detector_->detect(gray, toBackendParams(), backendResults, errorMsg);

    if (success) {
        // 转换后端结果到本地格式
        lines_.reserve(backendResults.size());
        for (const auto& br : backendResults) {
            LineResult lr;
            lr.id = br.id;
            lr.startPoint = br.startPoint;
            lr.endPoint = br.endPoint;
            lr.angle = br.angle;
            lr.length = br.length;
            lr.score = br.score;
            lr.a = br.a;
            lr.b = br.b;
            lr.c = br.c;
            lines_.push_back(lr);
        }

        // 限制数量
        if (maxCount_ > 0 && lines_.size() > static_cast<size_t>(maxCount_)) {
            lines_.resize(maxCount_);
        }

        output.success = true;
        output.executionTime = timer.elapsed();
        output.setValue("lineCount", static_cast<int>(lines_.size()));
        output.setValue("backend", detector_->backendName());

        // 记录每条线的信息
        QJsonArray lineArray;
        for (const auto& line : lines_) {
            QJsonObject lineObj;
            lineObj["id"] = line.id;
            lineObj["startX"] = line.startPoint.x();
            lineObj["startY"] = line.startPoint.y();
            lineObj["endX"] = line.endPoint.x();
            lineObj["endY"] = line.endPoint.y();
            lineObj["angle"] = line.angle;
            lineObj["length"] = line.length;
            lineObj["score"] = line.score;
            lineArray.append(lineObj);
        }
        output.data["lines"] = lineArray;

        // 绘制结果
        cv::Mat resultImage = drawResults(src);
        output.outputImage = Base::ImageMemoryPool::instance().allocate(
            resultImage.cols, resultImage.rows, resultImage.type());
        if (output.outputImage) {
            resultImage.copyTo(output.outputImage->mat());
        }

        setDebugImage(output.outputImage);
        setStatusText(QString("检测到 %1 条线").arg(lines_.size()));

        LOG_DEBUG(QString("线检测完成，检测到 %1 条线，耗时: %2ms，后端: %3")
                 .arg(lines_.size())
                 .arg(output.executionTime)
                 .arg(detector_->backendName()));
    } else {
        output.success = false;
        output.errorMessage = errorMsg;
    }

    return success;
}

void LineTool::ensureDetector()
{
    if (!detector_) {
        Backend::BackendType backendType = Backend::BackendType::Auto;
        switch (backend_) {
            case Auto:   backendType = Backend::BackendType::Auto; break;
            case OpenCV: backendType = Backend::BackendType::OpenCV; break;
            case Halcon: backendType = Backend::BackendType::Halcon; break;
        }
        detector_ = Backend::AlgorithmBackendFactory::instance().createLineDetector(backendType);
    }
}

Backend::LineDetectParams LineTool::toBackendParams() const
{
    Backend::LineDetectParams params;

    // 设置方法名称
    switch (method_) {
        case HoughLinesP: params.method = "HoughLinesP"; break;
        case HoughLines:  params.method = "HoughLines"; break;
        case ContourFit:  params.method = "ContourFit"; break;
        case EdgeFit:     params.method = "EdgeFit"; break;
    }

    params.minLength = minLength_;
    params.maxGap = maxGap_;
    params.maxCount = maxCount_;
    params.rho = rho_;
    params.theta = theta_;
    params.threshold = threshold_;
    params.cannyThreshold1 = cannyThreshold1_;
    params.cannyThreshold2 = cannyThreshold2_;

    return params;
}

cv::Mat LineTool::drawResults(const cv::Mat& input) const
{
    cv::Mat result;
    if (input.channels() == 1) {
        cv::cvtColor(input, result, cv::COLOR_GRAY2BGR);
    } else {
        result = input.clone();
    }

    for (const auto& line : lines_) {
        cv::Point pt1(static_cast<int>(line.startPoint.x()),
                     static_cast<int>(line.startPoint.y()));
        cv::Point pt2(static_cast<int>(line.endPoint.x()),
                     static_cast<int>(line.endPoint.y()));

        // 绘制线段
        cv::line(result, pt1, pt2, cv::Scalar(0, 255, 0), 2);

        // 绘制端点
        cv::circle(result, pt1, 4, cv::Scalar(0, 0, 255), -1);
        cv::circle(result, pt2, 4, cv::Scalar(0, 0, 255), -1);

        // 绘制信息
        cv::Point midPt((pt1.x + pt2.x) / 2, (pt1.y + pt2.y) / 2);
        QString text = QString("#%1 L:%.0f A:%.1f").arg(line.id).arg(line.length).arg(line.angle);
        cv::putText(result, text.toStdString(),
                   cv::Point(midPt.x + 5, midPt.y - 5),
                   cv::FONT_HERSHEY_SIMPLEX, 0.4, cv::Scalar(0, 255, 255), 1);
    }

    return result;
}

QJsonObject LineTool::serializeParams() const
{
    QJsonObject json;
    json["backend"] = static_cast<int>(backend_);
    json["method"] = static_cast<int>(method_);
    json["minLength"] = minLength_;
    json["maxGap"] = maxGap_;
    json["maxCount"] = maxCount_;
    json["rho"] = rho_;
    json["theta"] = theta_;
    json["threshold"] = threshold_;
    json["cannyThreshold1"] = cannyThreshold1_;
    json["cannyThreshold2"] = cannyThreshold2_;
    return json;
}

void LineTool::deserializeParams(const QJsonObject& json)
{
    backend_ = static_cast<BackendType>(json.value("backend").toInt(static_cast<int>(Auto)));
    method_ = static_cast<DetectionMethod>(json.value("method").toInt(static_cast<int>(HoughLinesP)));
    minLength_ = json.value("minLength").toDouble(50.0);
    maxGap_ = json.value("maxGap").toDouble(10.0);
    maxCount_ = json.value("maxCount").toInt(20);
    rho_ = json.value("rho").toDouble(1.0);
    theta_ = json.value("theta").toDouble(1.0);
    threshold_ = json.value("threshold").toInt(50);
    cannyThreshold1_ = json.value("cannyThreshold1").toDouble(50.0);
    cannyThreshold2_ = json.value("cannyThreshold2").toDouble(150.0);
}

void LineTool::setBackend(BackendType backend)
{
    if (backend_ != backend) {
        backend_ = backend;
        detector_.reset();  // 重置检测器，下次 process() 时重新创建
        emit paramChanged();
    }
}

void LineTool::setMethod(DetectionMethod method)
{
    if (method_ != method) {
        method_ = method;
        emit paramChanged();
    }
}

void LineTool::setMinLength(double length)
{
    if (qAbs(minLength_ - length) > 0.001) {
        minLength_ = length;
        emit paramChanged();
    }
}

void LineTool::setMaxGap(double gap)
{
    if (qAbs(maxGap_ - gap) > 0.001) {
        maxGap_ = gap;
        emit paramChanged();
    }
}

void LineTool::setMaxCount(int count)
{
    if (maxCount_ != count) {
        maxCount_ = count;
        emit paramChanged();
    }
}

void LineTool::setRho(double rho)
{
    if (qAbs(rho_ - rho) > 0.001) {
        rho_ = rho;
        emit paramChanged();
    }
}

void LineTool::setTheta(double theta)
{
    if (qAbs(theta_ - theta) > 0.001) {
        theta_ = theta;
        emit paramChanged();
    }
}

void LineTool::setThreshold(int threshold)
{
    if (threshold_ != threshold) {
        threshold_ = threshold;
        emit paramChanged();
    }
}

void LineTool::setCannyThreshold1(double threshold)
{
    if (qAbs(cannyThreshold1_ - threshold) > 0.001) {
        cannyThreshold1_ = threshold;
        emit paramChanged();
    }
}

void LineTool::setCannyThreshold2(double threshold)
{
    if (qAbs(cannyThreshold2_ - threshold) > 0.001) {
        cannyThreshold2_ = threshold;
        emit paramChanged();
    }
}

} // namespace Algorithm
} // namespace VisionForge
