/**
 * @file CircleTool.cpp
 * @brief 圆检测工具实现
 * @details 使用抽象后端接口，无需 #ifdef USE_HALCON
 */

#include "algorithm/CircleTool.h"
#include "algorithm/SubPixelEdgeTool.h"
#include "algorithm/backend/OpenCVCircleDetector.h"
#include "base/Logger.h"
#include "base/ImageMemoryPool.h"
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <QElapsedTimer>
#include <QJsonArray>
#include <algorithm>
#include <cmath>

namespace VisionForge {
namespace Algorithm {

CircleTool::CircleTool(QObject* parent)
    : VisionTool(parent)
    , backend_(Auto)
    , method_(HoughCircle)
    , minRadius_(10.0)
    , maxRadius_(200.0)
    , maxCount_(10)
    , houghDp_(1.0)
    , minDist_(50.0)
    , cannyThreshold_(100.0)
    , accumThreshold_(30.0)
    , minCircularity_(0.8)
    , minArea_(100.0)
    , subPixelEnabled_(false)
    , subPixelMethod_(SubPixelMethod::QuadraticFit)
{
    setDisplayName(toolName());
}

bool CircleTool::process(const Base::ImageData::Ptr& input, ToolResult& output)
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

    circles_.clear();

    // 确保检测器已创建
    ensureDetector();

    // 使用抽象接口进行检测（无需 #ifdef）
    std::vector<Backend::CircleDetectResult> backendResults;
    QString errorMsg;
    bool success = detector_->detect(gray, toBackendParams(), backendResults, errorMsg);

    if (success) {
        // 转换后端结果到本地格式
        circles_.reserve(backendResults.size());
        for (const auto& br : backendResults) {
            circles_.push_back(CircleResult::fromBackendResult(br));
        }

        // 限制数量
        if (maxCount_ > 0 && circles_.size() > static_cast<size_t>(maxCount_)) {
            circles_.resize(maxCount_);
        }

        output.success = true;
        output.executionTime = timer.elapsed();
        output.setValue("circleCount", static_cast<int>(circles_.size()));
        output.setValue("backend", detector_->backendName());

        // 记录每个圆的信息
        QJsonArray circleArray;
        for (const auto& circle : circles_) {
            QJsonObject circleObj;
            circleObj["id"] = circle.id;
            circleObj["centerX"] = circle.center.x();
            circleObj["centerY"] = circle.center.y();
            circleObj["radius"] = circle.radius;
            circleObj["score"] = circle.score;
            circleObj["circularity"] = circle.circularity;
            circleObj["area"] = circle.area;
            circleArray.append(circleObj);
        }
        output.data["circles"] = circleArray;

        // 绘制结果
        cv::Mat resultImage = drawResults(src);
        output.outputImage = Base::ImageMemoryPool::instance().allocate(
            resultImage.cols, resultImage.rows, resultImage.type());
        if (output.outputImage) {
            resultImage.copyTo(output.outputImage->mat());
        }

        setDebugImage(output.outputImage);
        setStatusText(QString("检测到 %1 个圆").arg(circles_.size()));

        LOG_DEBUG(QString("圆检测完成，检测到 %1 个圆，耗时: %2ms，后端: %3")
                 .arg(circles_.size())
                 .arg(output.executionTime)
                 .arg(detector_->backendName()));
    } else {
        output.success = false;
        output.errorMessage = errorMsg;
    }

    return success;
}

void CircleTool::ensureDetector()
{
    // 根据后端类型和方法创建或更新检测器
    Backend::BackendType backendType;
    switch (backend_) {
        case OpenCV:
            backendType = Backend::BackendType::OpenCV;
            break;
        case Halcon:
            backendType = Backend::BackendType::Halcon;
            break;
        default:
            // Auto: 根据方法选择后端
            if (method_ == EdgeFit || method_ == BlobFit) {
                backendType = Backend::BackendType::Halcon;  // 优先Halcon
            } else {
                backendType = Backend::BackendType::OpenCV;
            }
            break;
    }

    // 创建检测器（如果尚未创建或后端类型改变）
    if (!detector_) {
        detector_ = Backend::AlgorithmBackendFactory::instance().createCircleDetector(backendType);
    }

    // 设置OpenCV检测方法
    if (auto* opencvDetector = dynamic_cast<Backend::OpenCVCircleDetector*>(detector_.get())) {
        if (method_ == HoughCircle) {
            opencvDetector->setMethod(Backend::OpenCVCircleMethod::HoughCircle);
        } else if (method_ == ContourFit) {
            opencvDetector->setMethod(Backend::OpenCVCircleMethod::ContourFit);
        }
    }
}

Backend::CircleDetectParams CircleTool::toBackendParams() const
{
    Backend::CircleDetectParams params;
    params.minRadius = minRadius_;
    params.maxRadius = maxRadius_;
    params.maxCount = maxCount_;
    params.houghDp = houghDp_;
    params.minDist = minDist_;
    params.cannyThreshold = cannyThreshold_;
    params.accumThreshold = accumThreshold_;
    params.minCircularity = minCircularity_;
    params.minArea = minArea_;
    params.subPixelEnabled = subPixelEnabled_;
    return params;
}

cv::Mat CircleTool::drawResults(const cv::Mat& input) const
{
    cv::Mat result;
    if (input.channels() == 1) {
        cv::cvtColor(input, result, cv::COLOR_GRAY2BGR);
    } else {
        result = input.clone();
    }

    for (const auto& circle : circles_) {
        cv::Point center(static_cast<int>(circle.center.x()),
                        static_cast<int>(circle.center.y()));
        int radius = static_cast<int>(circle.radius);

        // 绘制圆
        cv::circle(result, center, radius, cv::Scalar(0, 255, 0), 2);

        // 绘制圆心
        cv::circle(result, center, 4, cv::Scalar(0, 0, 255), -1);

        // 绘制十字
        cv::line(result, cv::Point(center.x - 10, center.y),
                cv::Point(center.x + 10, center.y), cv::Scalar(0, 0, 255), 1);
        cv::line(result, cv::Point(center.x, center.y - 10),
                cv::Point(center.x, center.y + 10), cv::Scalar(0, 0, 255), 1);

        // 绘制信息
        QString text = QString("#%1 R:%.1f").arg(circle.id).arg(circle.radius);
        cv::putText(result, text.toStdString(),
                   cv::Point(center.x + radius + 5, center.y),
                   cv::FONT_HERSHEY_SIMPLEX, 0.4, cv::Scalar(0, 255, 255), 1);
    }

    return result;
}

QJsonObject CircleTool::serializeParams() const
{
    QJsonObject json;
    json["backend"] = static_cast<int>(backend_);
    json["method"] = static_cast<int>(method_);
    json["minRadius"] = minRadius_;
    json["maxRadius"] = maxRadius_;
    json["maxCount"] = maxCount_;
    json["houghDp"] = houghDp_;
    json["minDist"] = minDist_;
    json["cannyThreshold"] = cannyThreshold_;
    json["accumThreshold"] = accumThreshold_;
    json["minCircularity"] = minCircularity_;
    json["minArea"] = minArea_;
    json["subPixelEnabled"] = subPixelEnabled_;
    json["subPixelMethod"] = static_cast<int>(subPixelMethod_);
    return json;
}

void CircleTool::deserializeParams(const QJsonObject& json)
{
    backend_ = static_cast<BackendType>(json.value("backend").toInt(static_cast<int>(Auto)));
    method_ = static_cast<DetectionMethod>(json.value("method").toInt(static_cast<int>(HoughCircle)));
    minRadius_ = json.value("minRadius").toDouble(10.0);
    maxRadius_ = json.value("maxRadius").toDouble(200.0);
    maxCount_ = json.value("maxCount").toInt(10);
    houghDp_ = json.value("houghDp").toDouble(1.0);
    minDist_ = json.value("minDist").toDouble(50.0);
    cannyThreshold_ = json.value("cannyThreshold").toDouble(100.0);
    accumThreshold_ = json.value("accumThreshold").toDouble(30.0);
    minCircularity_ = json.value("minCircularity").toDouble(0.8);
    minArea_ = json.value("minArea").toDouble(100.0);
    subPixelEnabled_ = json.value("subPixelEnabled").toBool(false);
    subPixelMethod_ = static_cast<SubPixelMethod>(
        json.value("subPixelMethod").toInt(static_cast<int>(SubPixelMethod::QuadraticFit)));
}

void CircleTool::setBackend(BackendType backend)
{
    if (backend_ != backend) {
        backend_ = backend;
        detector_.reset();  // 重置检测器，下次使用时重新创建
        emit paramChanged();
    }
}

void CircleTool::setMethod(DetectionMethod method)
{
    if (method_ != method) {
        method_ = method;
        emit paramChanged();
    }
}

void CircleTool::setMinRadius(double radius)
{
    if (qAbs(minRadius_ - radius) > 0.001) {
        minRadius_ = radius;
        emit paramChanged();
    }
}

void CircleTool::setMaxRadius(double radius)
{
    if (qAbs(maxRadius_ - radius) > 0.001) {
        maxRadius_ = radius;
        emit paramChanged();
    }
}

void CircleTool::setMaxCount(int count)
{
    if (maxCount_ != count) {
        maxCount_ = count;
        emit paramChanged();
    }
}

void CircleTool::setHoughDp(double dp)
{
    if (qAbs(houghDp_ - dp) > 0.001) {
        houghDp_ = dp;
        emit paramChanged();
    }
}

void CircleTool::setMinDist(double dist)
{
    if (qAbs(minDist_ - dist) > 0.001) {
        minDist_ = dist;
        emit paramChanged();
    }
}

void CircleTool::setCannyThreshold(double threshold)
{
    if (qAbs(cannyThreshold_ - threshold) > 0.001) {
        cannyThreshold_ = threshold;
        emit paramChanged();
    }
}

void CircleTool::setAccumThreshold(double threshold)
{
    if (qAbs(accumThreshold_ - threshold) > 0.001) {
        accumThreshold_ = threshold;
        emit paramChanged();
    }
}

void CircleTool::setMinCircularity(double circ)
{
    if (qAbs(minCircularity_ - circ) > 0.001) {
        minCircularity_ = qBound(0.0, circ, 1.0);
        emit paramChanged();
    }
}

void CircleTool::setMinArea(double area)
{
    if (qAbs(minArea_ - area) > 0.001) {
        minArea_ = area;
        emit paramChanged();
    }
}

void CircleTool::setSubPixelEnabled(bool enabled)
{
    if (subPixelEnabled_ != enabled) {
        subPixelEnabled_ = enabled;
        emit paramChanged();
    }
}

void CircleTool::setSubPixelMethod(SubPixelMethod method)
{
    if (subPixelMethod_ != method) {
        subPixelMethod_ = method;
        emit paramChanged();
    }
}

double CircleTool::fitCircleSubPixel(const std::vector<cv::Point2f>& edgePoints, CircleResult& result)
{
    if (edgePoints.size() < 3) {
        return -1.0;
    }

    // 使用最小二乘法拟合圆
    // 圆方程: (x - cx)^2 + (y - cy)^2 = r^2
    // 展开: x^2 + y^2 - 2*cx*x - 2*cy*y + (cx^2 + cy^2 - r^2) = 0
    // 令 A = -2*cx, B = -2*cy, C = cx^2 + cy^2 - r^2
    // 则: x^2 + y^2 + A*x + B*y + C = 0

    int n = static_cast<int>(edgePoints.size());
    cv::Mat A(n, 3, CV_64F);
    cv::Mat b(n, 1, CV_64F);

    for (int i = 0; i < n; ++i) {
        double x = edgePoints[i].x;
        double y = edgePoints[i].y;

        A.at<double>(i, 0) = x;
        A.at<double>(i, 1) = y;
        A.at<double>(i, 2) = 1.0;

        b.at<double>(i, 0) = -(x * x + y * y);
    }

    // 最小二乘求解 (使用SVD分解)
    cv::Mat params;
    cv::solve(A, b, params, cv::DECOMP_SVD);

    double centerX = -params.at<double>(0, 0) / 2.0;
    double centerY = -params.at<double>(1, 0) / 2.0;
    double radius = std::sqrt(centerX * centerX + centerY * centerY - params.at<double>(2, 0));

    // 计算残差
    double residual = 0;
    for (const auto& pt : edgePoints) {
        double dx = pt.x - centerX;
        double dy = pt.y - centerY;
        double dist = std::sqrt(dx * dx + dy * dy);
        residual += (dist - radius) * (dist - radius);
    }

    double rms = std::sqrt(residual / n);

    result.center = QPointF(centerX, centerY);
    result.radius = radius;
    result.isSubPixel = true;
    result.fitResidual = rms;
    result.area = M_PI * radius * radius;
    result.circularity = 1.0;
    result.score = 1.0 / (1.0 + rms);  // 残差越小得分越高

    return rms;
}

} // namespace Algorithm
} // namespace VisionForge
