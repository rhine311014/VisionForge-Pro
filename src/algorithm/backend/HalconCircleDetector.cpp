/**
 * @file HalconCircleDetector.cpp
 * @brief Halcon圆检测后端实现
 */

#include "algorithm/backend/HalconCircleDetector.h"

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

HalconCircleDetector::HalconCircleDetector()
    : method_(HalconCircleMethod::EdgeFit)
{
}

bool HalconCircleDetector::detect(const cv::Mat& image,
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
        case HalconCircleMethod::EdgeFit:
            success = detectWithEdgeFit(gray, params, results, errorMsg);
            break;
        case HalconCircleMethod::BlobFit:
            success = detectWithBlobFit(gray, params, results, errorMsg);
            break;
        default:
            success = detectWithEdgeFit(gray, params, results, errorMsg);
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

bool HalconCircleDetector::supportsMethod(const QString& method) const
{
    return method == "EdgeFit" || method == "BlobFit";
}

QStringList HalconCircleDetector::supportedMethods() const
{
    return QStringList() << "EdgeFit" << "BlobFit";
}

bool HalconCircleDetector::detectWithEdgeFit(const cv::Mat& image,
                                             const CircleDetectParams& params,
                                             std::vector<CircleDetectResult>& results,
                                             QString& errorMsg)
{
    try {
        // 创建Halcon图像
        HImage hImage;
        hImage.GenImage1("byte", image.cols, image.rows, (void*)image.data);

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
            if (r < params.minRadius || r > params.maxRadius) continue;

            CircleDetectResult result;
            result.id = ++circleId;
            result.center = QPointF(col.D(), row.D());
            result.radius = r;
            result.circularity = 1.0;
            result.area = M_PI * r * r;
            result.score = 1.0;
            result.isSubPixel = true;  // Halcon边缘检测是亚像素精度

            results.push_back(result);
        }

        return true;

    } catch (const HException& e) {
        errorMsg = QString("Halcon边缘圆检测失败 [%1] 在 '%2': %3")
            .arg(static_cast<int>(e.ErrorCode()))
            .arg(QString::fromUtf8(e.ProcName().Text()))
            .arg(QString::fromUtf8(e.ErrorMessage().Text()));
        LOG_ERROR(errorMsg);
        return false;
    }
}

bool HalconCircleDetector::detectWithBlobFit(const cv::Mat& image,
                                             const CircleDetectParams& params,
                                             std::vector<CircleDetectResult>& results,
                                             QString& errorMsg)
{
    try {
        // 创建Halcon图像
        HImage hImage;
        hImage.GenImage1("byte", image.cols, image.rows, (void*)image.data);

        // 自动二值化
        HTuple usedThreshold;
        HRegion region = hImage.BinaryThreshold("max_separability", "light", &usedThreshold);

        // 连通域
        HRegion connectedRegions = region.Connection();

        // 选择圆形区域
        HRegion circleRegions = connectedRegions.SelectShape("circularity", "and", params.minCircularity, 1.0);

        // 按面积过滤
        circleRegions = circleRegions.SelectShape("area", "and", params.minArea, 99999999);

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
            if (r < params.minRadius || r > params.maxRadius) continue;

            CircleDetectResult result;
            result.id = ++circleId;
            result.center = QPointF(cCol.D(), cRow.D());
            result.radius = r;
            result.circularity = circularity.D();
            result.area = area.D();
            result.score = circularity.D();
            result.isSubPixel = false;

            results.push_back(result);
        }

        // 按圆度排序（降序）
        std::sort(results.begin(), results.end(),
                 [](const CircleDetectResult& a, const CircleDetectResult& b) {
                     return a.circularity > b.circularity;
                 });

        return true;

    } catch (const HException& e) {
        errorMsg = QString("Halcon Blob圆检测失败 [%1] 在 '%2': %3")
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
