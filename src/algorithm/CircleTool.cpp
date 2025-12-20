/**
 * @file CircleTool.cpp
 * @brief 圆检测工具实现
 */

#include "algorithm/CircleTool.h"
#include "algorithm/SubPixelEdgeTool.h"
#include "base/Logger.h"
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <QElapsedTimer>
#include <QJsonArray>
#include <algorithm>
#include <cmath>

#ifdef USE_HALCON
#include <HalconCpp.h>
using namespace HalconCpp;
#endif

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

    bool success = false;
    circles_.clear();

    // 根据后端和方法选择处理方式
#ifdef USE_HALCON
    if (backend_ == Halcon || (backend_ == Auto && (method_ == EdgeFit || method_ == BlobFit))) {
        if (method_ == EdgeFit) {
            success = processWithHalconEdgeFit(gray, output);
        } else if (method_ == BlobFit) {
            success = processWithHalconBlobFit(gray, output);
        }
        if (success) {
            output.setValue("backend", "Halcon");
        }
    }
#endif

    if (!success) {
        if (method_ == HoughCircle) {
            success = processWithHoughCircles(gray, output);
        } else if (method_ == ContourFit) {
            success = processWithContourFit(gray, output);
        } else {
            // 默认使用霍夫
            success = processWithHoughCircles(gray, output);
        }
        if (success) {
            output.setValue("backend", "OpenCV");
        }
    }

    if (success) {
        // 限制数量
        if (maxCount_ > 0 && circles_.size() > static_cast<size_t>(maxCount_)) {
            circles_.resize(maxCount_);
        }

        output.executionTime = timer.elapsed();
        output.setValue("circleCount", static_cast<int>(circles_.size()));

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
        output.outputImage = std::make_shared<Base::ImageData>(resultImage);

        setDebugImage(output.outputImage);
        setStatusText(QString("检测到 %1 个圆").arg(circles_.size()));

        LOG_DEBUG(QString("圆检测完成，检测到 %1 个圆，耗时: %2ms")
                 .arg(circles_.size())
                 .arg(output.executionTime));
    }

    return success;
}

bool CircleTool::processWithHoughCircles(const cv::Mat& input, ToolResult& output)
{
    try {
        // 高斯模糊减少噪声
        cv::Mat blurred;
        cv::GaussianBlur(input, blurred, cv::Size(9, 9), 2.0);

        // 霍夫圆检测
        std::vector<cv::Vec3f> houghCircles;
        cv::HoughCircles(blurred, houghCircles, cv::HOUGH_GRADIENT,
                        houghDp_,
                        minDist_,
                        cannyThreshold_,
                        accumThreshold_,
                        static_cast<int>(minRadius_),
                        static_cast<int>(maxRadius_));

        // 转换结果
        circles_.reserve(houghCircles.size());
        for (size_t i = 0; i < houghCircles.size(); ++i) {
            CircleResult result;
            result.id = static_cast<int>(i + 1);
            result.center = QPointF(houghCircles[i][0], houghCircles[i][1]);
            result.radius = houghCircles[i][2];
            result.score = 1.0;  // 霍夫检测没有置信度
            result.circularity = 1.0;  // 假设为完美圆
            result.area = M_PI * result.radius * result.radius;
            circles_.push_back(result);
        }

        // 按半径排序（降序）
        std::sort(circles_.begin(), circles_.end(),
                 [](const CircleResult& a, const CircleResult& b) {
                     return a.radius > b.radius;
                 });

        output.success = true;
        return true;

    } catch (const cv::Exception& e) {
        output.success = false;
        output.errorMessage = QString("霍夫圆检测失败: %1").arg(e.what());
        LOG_ERROR(output.errorMessage);
        return false;
    }
}

bool CircleTool::processWithContourFit(const cv::Mat& input, ToolResult& output)
{
    try {
        // 二值化
        cv::Mat binary;
        cv::threshold(input, binary, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

        // 查找轮廓
        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(binary, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        int circleId = 0;
        for (const auto& contour : contours) {
            if (contour.size() < 5) continue;  // 至少需要5个点进行椭圆拟合

            // 计算面积
            double area = cv::contourArea(contour);
            if (area < minArea_) continue;

            // 计算周长
            double perimeter = cv::arcLength(contour, true);

            // 计算圆度
            double circularity = 4.0 * M_PI * area / (perimeter * perimeter);
            if (circularity < minCircularity_) continue;

            // 拟合最小外接圆
            cv::Point2f center;
            float radius;
            cv::minEnclosingCircle(contour, center, radius);

            // 检查半径范围
            if (radius < minRadius_ || radius > maxRadius_) continue;

            CircleResult result;
            result.id = ++circleId;
            result.center = QPointF(center.x, center.y);
            result.radius = radius;
            result.circularity = circularity;
            result.area = area;
            result.score = circularity;  // 使用圆度作为置信度

            circles_.push_back(result);
        }

        // 按圆度排序（降序）
        std::sort(circles_.begin(), circles_.end(),
                 [](const CircleResult& a, const CircleResult& b) {
                     return a.circularity > b.circularity;
                 });

        output.success = true;
        return true;

    } catch (const cv::Exception& e) {
        output.success = false;
        output.errorMessage = QString("轮廓拟合圆检测失败: %1").arg(e.what());
        LOG_ERROR(output.errorMessage);
        return false;
    }
}

#ifdef USE_HALCON
bool CircleTool::processWithHalconEdgeFit(const cv::Mat& input, ToolResult& output)
{
    try {
        // 创建Halcon图像
        HImage hImage;
        hImage.GenImage1("byte", input.cols, input.rows, (void*)input.data);

        // 边缘检测
        HImage edges = hImage.EdgesSubPix("canny", 1.0, 20, 40);

        // 获取XLD轮廓
        HXLDCont contours = edges.EdgesSubPix("canny", 1.0, 20, 40);

        // 分割轮廓
        HXLDCont segContours = contours.SegmentContoursXld("lines_circles", 5, 4, 2);

        // 选择圆形轮廓
        HTuple attribName("cont_approx");
        HTuple attribMin(2);  // 圆形
        HTuple attribMax(2);
        HXLDCont circleContours = segContours.SelectContoursXld(attribName, attribMin, attribMax, 0, 0);

        // 获取轮廓数量
        Hlong count = circleContours.CountObj();

        int circleId = 0;
        for (Hlong i = 1; i <= count; ++i) {
            HXLDCont singleContour = circleContours.SelectObj(i);

            // 拟合圆
            HTuple row, col, radius, startPhi, endPhi, pointOrder;
            singleContour.FitCircleContourXld("algebraic", -1, 0, 0, 3, 2,
                                              &row, &col, &radius, &startPhi, &endPhi, &pointOrder);

            if (radius.Length() == 0) continue;

            double r = radius.D();

            // 检查半径范围
            if (r < minRadius_ || r > maxRadius_) continue;

            CircleResult result;
            result.id = ++circleId;
            result.center = QPointF(col.D(), row.D());
            result.radius = r;
            result.circularity = 1.0;
            result.area = M_PI * r * r;
            result.score = 1.0;

            circles_.push_back(result);
        }

        output.success = true;
        return true;

    } catch (const HException& e) {
        output.success = false;
        output.errorMessage = QString("Halcon边缘圆检测失败: %1").arg(e.ErrorMessage().Text());
        LOG_ERROR(output.errorMessage);
        return false;
    }
}

bool CircleTool::processWithHalconBlobFit(const cv::Mat& input, ToolResult& output)
{
    try {
        // 创建Halcon图像
        HImage hImage;
        hImage.GenImage1("byte", input.cols, input.rows, (void*)input.data);

        // 自动二值化
        HTuple usedThreshold;
        HRegion region = hImage.BinaryThreshold("max_separability", "light", &usedThreshold);

        // 连通域
        HRegion connectedRegions = region.Connection();

        // 选择圆形区域
        HRegion circleRegions = connectedRegions.SelectShape("circularity", "and", minCircularity_, 1.0);

        // 按面积过滤
        circleRegions = circleRegions.SelectShape("area", "and", minArea_, 99999999);

        // 获取区域数量
        Hlong count = circleRegions.CountObj();

        int circleId = 0;
        for (Hlong i = 1; i <= count; ++i) {
            HRegion singleRegion = circleRegions.SelectObj(i);

            // 获取面积和质心
            HTuple row, col;
            HTuple area = singleRegion.AreaCenter(&row, &col);

            // 获取圆度
            HTuple circularity = singleRegion.Circularity();

            // 拟合最小外接圆
            HTuple cRow, cCol, cRadius;
            singleRegion.SmallestCircle(&cRow, &cCol, &cRadius);

            double r = cRadius.D();

            // 检查半径范围
            if (r < minRadius_ || r > maxRadius_) continue;

            CircleResult result;
            result.id = ++circleId;
            result.center = QPointF(cCol.D(), cRow.D());
            result.radius = r;
            result.circularity = circularity.D();
            result.area = area.D();
            result.score = circularity.D();

            circles_.push_back(result);
        }

        // 按圆度排序（降序）
        std::sort(circles_.begin(), circles_.end(),
                 [](const CircleResult& a, const CircleResult& b) {
                     return a.circularity > b.circularity;
                 });

        output.success = true;
        return true;

    } catch (const HException& e) {
        output.success = false;
        output.errorMessage = QString("Halcon Blob圆检测失败: %1").arg(e.ErrorMessage().Text());
        LOG_ERROR(output.errorMessage);
        return false;
    }
}
#endif

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
