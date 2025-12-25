/**
 * @file OpenCVCircleDetector.cpp
 * @brief OpenCV圆检测后端实现
 */

#include "algorithm/backend/OpenCVCircleDetector.h"
#include "base/Logger.h"
#include <opencv2/imgproc.hpp>
#include <cmath>
#include <algorithm>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace VisionForge {
namespace Algorithm {
namespace Backend {

OpenCVCircleDetector::OpenCVCircleDetector()
    : method_(OpenCVCircleMethod::HoughCircle)
{
}

bool OpenCVCircleDetector::detect(const cv::Mat& image,
                                  const CircleDetectParams& params,
                                  std::vector<CircleDetectResult>& results,
                                  QString& errorMsg)
{
    results.clear();

    if (image.empty()) {
        errorMsg = "输入图像为空";
        return false;
    }

    // 确保是灰度图
    cv::Mat gray;
    if (image.channels() == 3) {
        cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    } else if (image.channels() == 4) {
        cv::cvtColor(image, gray, cv::COLOR_BGRA2GRAY);
    } else {
        gray = image;
    }

    bool success = false;
    switch (method_) {
        case OpenCVCircleMethod::HoughCircle:
            success = detectWithHough(gray, params, results, errorMsg);
            break;
        case OpenCVCircleMethod::ContourFit:
            success = detectWithContourFit(gray, params, results, errorMsg);
            break;
        default:
            success = detectWithHough(gray, params, results, errorMsg);
            break;
    }

    if (success) {
        // 限制数量
        if (params.maxCount > 0 && results.size() > static_cast<size_t>(params.maxCount)) {
            results.resize(params.maxCount);
        }
    }

    return success;
}

bool OpenCVCircleDetector::supportsMethod(const QString& method) const
{
    return method == "HoughCircle" || method == "ContourFit";
}

QStringList OpenCVCircleDetector::supportedMethods() const
{
    return QStringList() << "HoughCircle" << "ContourFit";
}

bool OpenCVCircleDetector::detectWithHough(const cv::Mat& image,
                                           const CircleDetectParams& params,
                                           std::vector<CircleDetectResult>& results,
                                           QString& errorMsg)
{
    try {
        // 高斯模糊减少噪声
        cv::Mat blurred;
        cv::GaussianBlur(image, blurred, cv::Size(9, 9), 2.0);

        // 霍夫圆检测
        std::vector<cv::Vec3f> houghCircles;
        cv::HoughCircles(blurred, houghCircles, cv::HOUGH_GRADIENT,
                        params.houghDp,
                        params.minDist,
                        params.cannyThreshold,
                        params.accumThreshold,
                        static_cast<int>(params.minRadius),
                        static_cast<int>(params.maxRadius));

        // 转换结果
        results.reserve(houghCircles.size());
        for (size_t i = 0; i < houghCircles.size(); ++i) {
            CircleDetectResult result;
            result.id = static_cast<int>(i + 1);
            result.center = QPointF(houghCircles[i][0], houghCircles[i][1]);
            result.radius = houghCircles[i][2];
            result.score = 1.0;  // 霍夫检测没有置信度
            result.circularity = 1.0;  // 假设为完美圆
            result.area = M_PI * result.radius * result.radius;
            result.isSubPixel = false;
            results.push_back(result);
        }

        // 按半径排序（降序）
        std::sort(results.begin(), results.end(),
                 [](const CircleDetectResult& a, const CircleDetectResult& b) {
                     return a.radius > b.radius;
                 });

        return true;

    } catch (const cv::Exception& e) {
        errorMsg = QString("霍夫圆检测失败: %1").arg(e.what());
        LOG_ERROR(errorMsg);
        return false;
    }
}

bool OpenCVCircleDetector::detectWithContourFit(const cv::Mat& image,
                                                const CircleDetectParams& params,
                                                std::vector<CircleDetectResult>& results,
                                                QString& errorMsg)
{
    try {
        // 二值化
        cv::Mat binary;
        cv::threshold(image, binary, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

        // 查找轮廓
        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(binary, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        int circleId = 0;
        for (const auto& contour : contours) {
            if (contour.size() < 5) continue;  // 至少需要5个点进行椭圆拟合

            // 计算面积
            double area = cv::contourArea(contour);
            if (area < params.minArea) continue;

            // 计算周长
            double perimeter = cv::arcLength(contour, true);

            // 计算圆度
            double circularity = 4.0 * M_PI * area / (perimeter * perimeter);
            if (circularity < params.minCircularity) continue;

            // 拟合最小外接圆
            cv::Point2f center;
            float radius;
            cv::minEnclosingCircle(contour, center, radius);

            // 检查半径范围
            if (radius < params.minRadius || radius > params.maxRadius) continue;

            CircleDetectResult result;
            result.id = ++circleId;
            result.center = QPointF(center.x, center.y);
            result.radius = radius;
            result.circularity = circularity;
            result.area = area;
            result.score = circularity;  // 使用圆度作为置信度
            result.isSubPixel = false;

            results.push_back(result);
        }

        // 按圆度排序（降序）
        std::sort(results.begin(), results.end(),
                 [](const CircleDetectResult& a, const CircleDetectResult& b) {
                     return a.circularity > b.circularity;
                 });

        return true;

    } catch (const cv::Exception& e) {
        errorMsg = QString("轮廓拟合圆检测失败: %1").arg(e.what());
        LOG_ERROR(errorMsg);
        return false;
    }
}

} // namespace Backend
} // namespace Algorithm
} // namespace VisionForge
