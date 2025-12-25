/**
 * @file HalconBlobAnalyzer.cpp
 * @brief Halcon Blob分析后端实现
 */

#include "algorithm/backend/HalconBlobAnalyzer.h"

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

bool HalconBlobAnalyzer::analyze(const cv::Mat& image,
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

        // 创建Halcon图像
        HImage hImage;
        hImage.GenImage1("byte", gray.cols, gray.rows, (void*)gray.data);

        // 二值化
        HRegion region;
        if (params.autoThreshold) {
            HTuple usedThreshold;
            region = hImage.BinaryThreshold("max_separability",
                                           params.detectLight ? "light" : "dark",
                                           &usedThreshold);
        } else {
            if (params.detectLight) {
                region = hImage.Threshold(params.threshold, 255);
            } else {
                region = hImage.Threshold(0, params.threshold);
            }
        }

        // 连通域分析
        HRegion connectedRegions = region.Connection();

        // 按面积过滤
        HRegion filteredRegions = connectedRegions.SelectShape("area", "and",
                                                               params.minArea, params.maxArea);

        // 按圆度过滤
        filteredRegions = filteredRegions.SelectShape("circularity", "and",
                                                      params.minCircularity, params.maxCircularity);

        // 获取区域数量
        Hlong count = filteredRegions.CountObj();

        for (Hlong i = 1; i <= count; ++i) {
            HRegion singleRegion = filteredRegions.SelectObj(i);

            BlobAnalyzeResult result;
            result.id = static_cast<int>(i);

            // 面积和质心
            HTuple row, col;
            HTuple area = singleRegion.AreaCenter(&row, &col);
            result.area = area.D();
            result.center = QPointF(col.D(), row.D());

            // 圆度
            HTuple circularity = singleRegion.Circularity();
            result.circularity = circularity.D();

            // 矩形度
            HTuple rectangularity = singleRegion.Rectangularity();
            result.rectangularity = rectangularity.D();

            // 外接矩形
            HTuple r1, c1, r2, c2;
            singleRegion.SmallestRectangle1(&r1, &c1, &r2, &c2);
            result.boundingBox = QRectF(c1.D(), r1.D(), c2.D() - c1.D(), r2.D() - r1.D());

            // 周长
            HTuple contourLength = singleRegion.Contlength();
            result.perimeter = contourLength.D();

            // 方向
            HTuple phi = singleRegion.OrientationRegion();
            result.orientation = phi.D() * 180.0 / M_PI;

            // 等效圆直径
            result.equivalentDiameter = std::sqrt(4.0 * result.area / M_PI);

            // 长宽比
            if (result.boundingBox.height() > 0) {
                result.aspectRatio = result.boundingBox.width() / result.boundingBox.height();
            }

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

    } catch (const HException& e) {
        errorMsg = QString("Halcon Blob分析失败: %1").arg(e.ErrorMessage().Text());
        LOG_ERROR(errorMsg);
        return false;
    }
}

} // namespace Backend
} // namespace Algorithm
} // namespace VisionForge

#endif // USE_HALCON
