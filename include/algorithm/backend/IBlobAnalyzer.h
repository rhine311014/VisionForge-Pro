/**
 * @file IBlobAnalyzer.h
 * @brief Blob分析算法后端接口
 * @details 定义Blob分析的抽象接口，支持OpenCV和Halcon双后端
 * @author VisionForge Team
 * @date 2025-12-25
 */

#pragma once

#include "base/ImageData.h"
#include <QString>
#include <QPointF>
#include <QRectF>
#include <vector>
#include <memory>

namespace VisionForge {
namespace Algorithm {
namespace Backend {

/**
 * @brief Blob分析结果
 */
struct BlobAnalyzeResult {
    int id;
    double area;
    double perimeter;
    double circularity;
    double rectangularity;
    double aspectRatio;
    double orientation;
    QPointF center;
    QRectF boundingBox;
    double equivalentDiameter;
    std::vector<cv::Point> contour;

    BlobAnalyzeResult()
        : id(0), area(0), perimeter(0), circularity(0), rectangularity(0)
        , aspectRatio(0), orientation(0), equivalentDiameter(0) {}
};

/**
 * @brief Blob分析参数
 */
struct BlobAnalyzeParams {
    // 连通性
    int connectivity = 8;           // 4或8连通
    bool detectLight = true;        // 检测亮区域
    bool autoThreshold = true;      // 自动二值化
    int threshold = 128;            // 手动阈值

    // 过滤条件
    double minArea = 100.0;
    double maxArea = 1e9;
    double minCircularity = 0.0;
    double maxCircularity = 1.0;

    // 结果限制
    int maxCount = 100;
    bool sortByArea = true;
    bool sortDescending = true;
};

/**
 * @brief Blob分析算法后端接口
 */
class IBlobAnalyzer {
public:
    virtual ~IBlobAnalyzer() = default;

    virtual QString backendName() const = 0;

    virtual bool analyze(const cv::Mat& image,
                        const BlobAnalyzeParams& params,
                        std::vector<BlobAnalyzeResult>& results,
                        QString& errorMsg) = 0;
};

using IBlobAnalyzerPtr = std::unique_ptr<IBlobAnalyzer>;

} // namespace Backend
} // namespace Algorithm
} // namespace VisionForge
