/**
 * @file OpenCVBlobAnalyzer.cpp
 * @brief OpenCV Blob分析后端实现
 */

#include "algorithm/backend/OpenCVBlobAnalyzer.h"
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

bool OpenCVBlobAnalyzer::analyze(const cv::Mat& image,
                                 const BlobAnalyzeParams& params,
                                 std::vector<BlobAnalyzeResult>& results,
                                 QString& errorMsg)
{
    results.clear();

    if (image.empty()) {
        errorMsg = "输入图像为空";
        return false;
    }

    try {
        cv::Mat gray;
        if (image.channels() == 3) {
            cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
        } else if (image.channels() == 4) {
            cv::cvtColor(image, gray, cv::COLOR_BGRA2GRAY);
        } else {
            gray = image.clone();
        }

        // 二值化
        cv::Mat binary;
        if (params.autoThreshold) {
            cv::threshold(gray, binary, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
        } else {
            cv::threshold(gray, binary, params.threshold, 255, cv::THRESH_BINARY);
        }

        // 根据极性反转
        if (!params.detectLight) {
            cv::bitwise_not(binary, binary);
        }

        // 查找轮廓
        std::vector<std::vector<cv::Point>> contours;
        int connectivity = (params.connectivity == 4) ? cv::RETR_EXTERNAL : cv::RETR_EXTERNAL;
        cv::findContours(binary, contours, connectivity, cv::CHAIN_APPROX_SIMPLE);

        int blobId = 0;
        for (const auto& contour : contours) {
            if (contour.size() < 5) continue;

            BlobAnalyzeResult result = calculateFeatures(contour, ++blobId);

            // 应用过滤条件
            if (result.area < params.minArea || result.area > params.maxArea) continue;
            if (result.circularity < params.minCircularity || result.circularity > params.maxCircularity) continue;

            results.push_back(result);
        }

        // 排序
        if (params.sortByArea) {
            if (params.sortDescending) {
                std::sort(results.begin(), results.end(),
                         [](const BlobAnalyzeResult& a, const BlobAnalyzeResult& b) {
                             return a.area > b.area;
                         });
            } else {
                std::sort(results.begin(), results.end(),
                         [](const BlobAnalyzeResult& a, const BlobAnalyzeResult& b) {
                             return a.area < b.area;
                         });
            }
        }

        // 限制数量
        if (params.maxCount > 0 && results.size() > static_cast<size_t>(params.maxCount)) {
            results.resize(params.maxCount);
        }

        // 重新编号
        for (size_t i = 0; i < results.size(); ++i) {
            results[i].id = static_cast<int>(i + 1);
        }

        return true;

    } catch (const cv::Exception& e) {
        errorMsg = QString("Blob分析失败: %1").arg(e.what());
        LOG_ERROR(errorMsg);
        return false;
    }
}

BlobAnalyzeResult OpenCVBlobAnalyzer::calculateFeatures(const std::vector<cv::Point>& contour, int id) const
{
    BlobAnalyzeResult result;
    result.id = id;
    result.contour = contour;

    // 面积
    result.area = cv::contourArea(contour);

    // 周长
    result.perimeter = cv::arcLength(contour, true);

    // 圆度
    if (result.perimeter > 0) {
        result.circularity = 4.0 * M_PI * result.area / (result.perimeter * result.perimeter);
    }

    // 外接矩形
    cv::Rect rect = cv::boundingRect(contour);
    result.boundingBox = QRectF(rect.x, rect.y, rect.width, rect.height);

    // 矩形度
    double rectArea = rect.width * rect.height;
    if (rectArea > 0) {
        result.rectangularity = result.area / rectArea;
    }

    // 长宽比
    if (rect.height > 0) {
        result.aspectRatio = static_cast<double>(rect.width) / rect.height;
    }

    // 质心
    cv::Moments m = cv::moments(contour);
    if (m.m00 > 0) {
        result.center = QPointF(m.m10 / m.m00, m.m01 / m.m00);
    }

    // 等效圆直径
    result.equivalentDiameter = std::sqrt(4.0 * result.area / M_PI);

    // 方向（如果能拟合椭圆）
    if (contour.size() >= 5) {
        try {
            cv::RotatedRect ellipse = cv::fitEllipse(contour);
            result.orientation = ellipse.angle;
        } catch (...) {
            result.orientation = 0;
        }
    }

    return result;
}

} // namespace Backend
} // namespace Algorithm
} // namespace VisionForge
