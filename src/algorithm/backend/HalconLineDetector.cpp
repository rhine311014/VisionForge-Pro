/**
 * @file HalconLineDetector.cpp
 * @brief Halcon线检测后端实现
 */

#include "algorithm/backend/HalconLineDetector.h"

#ifdef USE_HALCON

#include "base/Logger.h"
#include <opencv2/imgproc.hpp>
#include <HalconCpp.h>
#include <cmath>
#include <algorithm>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using namespace HalconCpp;

namespace VisionForge {
namespace Algorithm {
namespace Backend {

HalconLineDetector::HalconLineDetector()
{
}

bool HalconLineDetector::detect(const cv::Mat& image,
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

    bool success = detectWithEdgeFit(gray, params, results, errorMsg);

    if (success && params.maxCount > 0 && results.size() > static_cast<size_t>(params.maxCount)) {
        results.resize(params.maxCount);
    }

    return success;
}

bool HalconLineDetector::supportsMethod(const QString& method) const
{
    return method == "EdgeFit";
}

QStringList HalconLineDetector::supportedMethods() const
{
    return QStringList() << "EdgeFit";
}

bool HalconLineDetector::detectWithEdgeFit(const cv::Mat& image,
                                           const LineDetectParams& params,
                                           std::vector<LineDetectResult>& results,
                                           QString& errorMsg)
{
    try {
        // 创建Halcon图像
        HImage hImage;
        hImage.GenImage1("byte", image.cols, image.rows, (void*)image.data);

        // 边缘检测
        HXLDCont contours = hImage.EdgesSubPix("canny", 1.0,
                                               params.cannyThreshold1 / 5,
                                               params.cannyThreshold2 / 5);

        // 分割轮廓为线段
        HXLDCont segContours = contours.SegmentContoursXld("lines", 5, 4, 2);

        // 获取轮廓数量
        Hlong count = segContours.CountObj();

        int lineId = 0;
        for (Hlong i = 1; i <= count; ++i) {
            HXLDCont singleContour = segContours.SelectObj(i);

            // 直接拟合直线（不检查轮廓类型，让拟合结果决定是否有效）
            // 拟合直线
            HTuple rowBegin, colBegin, rowEnd, colEnd, nr, nc, dist;
            singleContour.FitLineContourXld("tukey", -1, 0, 5, 2,
                                            &rowBegin, &colBegin, &rowEnd, &colEnd, &nr, &nc, &dist);

            if (rowBegin.Length() == 0) continue;

            double x1 = colBegin.D();
            double y1 = rowBegin.D();
            double x2 = colEnd.D();
            double y2 = rowEnd.D();

            double dx = x2 - x1;
            double dy = y2 - y1;
            double length = std::sqrt(dx * dx + dy * dy);

            if (length < params.minLength) continue;

            LineDetectResult result;
            result.id = ++lineId;
            result.startPoint = QPointF(x1, y1);
            result.endPoint = QPointF(x2, y2);
            result.length = length;
            result.angle = std::atan2(dy, dx) * 180.0 / M_PI;
            if (result.angle < 0) result.angle += 180;
            result.score = 1.0 / (1.0 + dist.D());
            result.a = nr.D();
            result.b = nc.D();
            result.c = -(nr.D() * y1 + nc.D() * x1);

            results.push_back(result);
        }

        std::sort(results.begin(), results.end(),
                 [](const LineDetectResult& a, const LineDetectResult& b) {
                     return a.length > b.length;
                 });

        return true;

    } catch (const HException& e) {
        errorMsg = QString("Halcon线检测失败 [%1] 在 '%2': %3")
            .arg(static_cast<int>(e.ErrorCode()))
            .arg(QString::fromUtf8(e.ProcName().Text()))
            .arg(QString::fromUtf8(e.ErrorMessage().Text()));
        LOG_ERROR(errorMsg);
        return false;
    }
}

} // namespace Backend
} // namespace Algorithm
} // namespace VisionForge

#endif // USE_HALCON
