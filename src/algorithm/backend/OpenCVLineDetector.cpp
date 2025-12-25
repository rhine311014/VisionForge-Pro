/**
 * @file OpenCVLineDetector.cpp
 * @brief OpenCV线检测后端实现
 */

#include "algorithm/backend/OpenCVLineDetector.h"
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

OpenCVLineDetector::OpenCVLineDetector()
    : method_(OpenCVLineMethod::HoughLinesP)
{
}

bool OpenCVLineDetector::detect(const cv::Mat& image,
                                const LineDetectParams& params,
                                std::vector<LineDetectResult>& results,
                                QString& errorMsg)
{
    results.clear();

    if (image.empty()) {
        errorMsg = "输入图像为空";
        return false;
    }

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
        case OpenCVLineMethod::HoughLinesP:
            success = detectWithHoughLinesP(gray, params, results, errorMsg);
            break;
        case OpenCVLineMethod::HoughLines:
            success = detectWithHoughLines(gray, params, results, errorMsg);
            break;
        case OpenCVLineMethod::ContourFit:
            success = detectWithContourFit(gray, params, results, errorMsg);
            break;
    }

    if (success && params.maxCount > 0 && results.size() > static_cast<size_t>(params.maxCount)) {
        results.resize(params.maxCount);
    }

    return success;
}

bool OpenCVLineDetector::supportsMethod(const QString& method) const
{
    return method == "HoughLinesP" || method == "HoughLines" || method == "ContourFit";
}

QStringList OpenCVLineDetector::supportedMethods() const
{
    return QStringList() << "HoughLinesP" << "HoughLines" << "ContourFit";
}

bool OpenCVLineDetector::detectWithHoughLinesP(const cv::Mat& image,
                                               const LineDetectParams& params,
                                               std::vector<LineDetectResult>& results,
                                               QString& errorMsg)
{
    try {
        cv::Mat edges;
        cv::Canny(image, edges, params.cannyThreshold1, params.cannyThreshold2);

        std::vector<cv::Vec4i> lines;
        cv::HoughLinesP(edges, lines, params.rho, params.theta * M_PI / 180.0,
                       params.threshold, params.minLength, params.maxGap);

        int lineId = 0;
        for (const auto& line : lines) {
            LineDetectResult result;
            result.id = ++lineId;
            result.startPoint = QPointF(line[0], line[1]);
            result.endPoint = QPointF(line[2], line[3]);

            double dx = line[2] - line[0];
            double dy = line[3] - line[1];
            result.length = std::sqrt(dx * dx + dy * dy);
            result.angle = std::atan2(dy, dx) * 180.0 / M_PI;
            if (result.angle < 0) result.angle += 180;
            result.score = 1.0;

            // 直线方程参数
            result.a = dy;
            result.b = -dx;
            result.c = dx * line[1] - dy * line[0];
            double norm = std::sqrt(result.a * result.a + result.b * result.b);
            if (norm > 0) {
                result.a /= norm;
                result.b /= norm;
                result.c /= norm;
            }

            results.push_back(result);
        }

        std::sort(results.begin(), results.end(),
                 [](const LineDetectResult& a, const LineDetectResult& b) {
                     return a.length > b.length;
                 });

        return true;
    } catch (const cv::Exception& e) {
        errorMsg = QString("HoughLinesP失败: %1").arg(e.what());
        return false;
    }
}

bool OpenCVLineDetector::detectWithHoughLines(const cv::Mat& image,
                                              const LineDetectParams& params,
                                              std::vector<LineDetectResult>& results,
                                              QString& errorMsg)
{
    try {
        cv::Mat edges;
        cv::Canny(image, edges, params.cannyThreshold1, params.cannyThreshold2);

        std::vector<cv::Vec2f> lines;
        cv::HoughLines(edges, lines, params.rho, params.theta * M_PI / 180.0, params.threshold);

        int lineId = 0;
        for (const auto& line : lines) {
            float rho = line[0];
            float theta = line[1];

            double a = std::cos(theta);
            double b = std::sin(theta);
            double x0 = a * rho;
            double y0 = b * rho;

            // 创建足够长的线段
            double scale = std::max(image.cols, image.rows);
            QPointF pt1(x0 + scale * (-b), y0 + scale * a);
            QPointF pt2(x0 - scale * (-b), y0 - scale * a);

            LineDetectResult result;
            result.id = ++lineId;
            result.startPoint = pt1;
            result.endPoint = pt2;
            result.angle = theta * 180.0 / M_PI;
            result.length = 2 * scale;
            result.score = 1.0;
            result.a = a;
            result.b = b;
            result.c = -rho;

            results.push_back(result);
        }

        return true;
    } catch (const cv::Exception& e) {
        errorMsg = QString("HoughLines失败: %1").arg(e.what());
        return false;
    }
}

bool OpenCVLineDetector::detectWithContourFit(const cv::Mat& image,
                                              const LineDetectParams& params,
                                              std::vector<LineDetectResult>& results,
                                              QString& errorMsg)
{
    try {
        cv::Mat edges;
        cv::Canny(image, edges, params.cannyThreshold1, params.cannyThreshold2);

        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(edges, contours, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);

        int lineId = 0;
        for (const auto& contour : contours) {
            if (contour.size() < 5) continue;

            cv::Vec4f lineParams;
            cv::fitLine(contour, lineParams, cv::DIST_L2, 0, 0.01, 0.01);

            double vx = lineParams[0];
            double vy = lineParams[1];
            double x0 = lineParams[2];
            double y0 = lineParams[3];

            // 计算线段端点
            double minT = std::numeric_limits<double>::max();
            double maxT = std::numeric_limits<double>::lowest();
            for (const auto& pt : contour) {
                double t = (pt.x - x0) * vx + (pt.y - y0) * vy;
                minT = std::min(minT, t);
                maxT = std::max(maxT, t);
            }

            QPointF pt1(x0 + minT * vx, y0 + minT * vy);
            QPointF pt2(x0 + maxT * vx, y0 + maxT * vy);

            double dx = pt2.x() - pt1.x();
            double dy = pt2.y() - pt1.y();
            double length = std::sqrt(dx * dx + dy * dy);

            if (length < params.minLength) continue;

            LineDetectResult result;
            result.id = ++lineId;
            result.startPoint = pt1;
            result.endPoint = pt2;
            result.length = length;
            result.angle = std::atan2(dy, dx) * 180.0 / M_PI;
            if (result.angle < 0) result.angle += 180;
            result.score = 1.0;
            result.a = vy;
            result.b = -vx;
            result.c = vx * y0 - vy * x0;

            results.push_back(result);
        }

        std::sort(results.begin(), results.end(),
                 [](const LineDetectResult& a, const LineDetectResult& b) {
                     return a.length > b.length;
                 });

        return true;
    } catch (const cv::Exception& e) {
        errorMsg = QString("轮廓拟合失败: %1").arg(e.what());
        return false;
    }
}

} // namespace Backend
} // namespace Algorithm
} // namespace VisionForge
