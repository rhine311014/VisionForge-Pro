/**
 * @file CornerSearchTool.h
 * @brief 角点搜索工具
 * @details 支持多种角点检测算法的搜索工具
 * @author VisionForge Team
 * @date 2025-12-30
 */

#pragma once

#include "algorithm/VisionTool.h"
#include <vector>
#include <opencv2/opencv.hpp>

namespace VisionForge {
namespace Algorithm {

/**
 * @brief 角点检测结果
 */
struct CornerPoint {
    double x = 0;           ///< X坐标
    double y = 0;           ///< Y坐标
    double response = 0;    ///< 响应值
    double angle = 0;       ///< 方向（仅ORB等有方向的特征点）
    double size = 0;        ///< 特征点大小
};

/**
 * @brief 角点搜索工具
 *
 * 支持多种角点检测算法：
 * - Harris角点检测
 * - Shi-Tomasi角点检测
 * - FAST特征点检测
 * - ORB特征点检测
 */
class CornerSearchTool : public VisionTool
{
    Q_OBJECT

public:
    /**
     * @brief 检测方法
     */
    enum Method {
        Harris = 0,         ///< Harris角点
        ShiTomasi,          ///< Shi-Tomasi角点
        FAST,               ///< FAST特征点
        ORB                 ///< ORB特征点
    };

    explicit CornerSearchTool(QObject* parent = nullptr);
    ~CornerSearchTool() override = default;

    // VisionTool接口实现
    ToolType toolType() const override { return Custom; }
    QString toolName() const override { return tr("角点搜索"); }
    bool process(const Base::ImageData::Ptr& input, ToolResult& output) override;
    QJsonObject serializeParams() const override;
    void deserializeParams(const QJsonObject& json) override;

    // ========== 参数设置 ==========

    // 检测方法
    Method method() const { return method_; }
    void setMethod(Method m) { method_ = m; }

    // Harris参数
    int harrisBlockSize() const { return harrisBlockSize_; }
    void setHarrisBlockSize(int size) { harrisBlockSize_ = qBound(2, size, 31); }

    int harrisKSize() const { return harrisKSize_; }
    void setHarrisKSize(int size) { harrisKSize_ = size | 1; }  // 确保为奇数

    double harrisK() const { return harrisK_; }
    void setHarrisK(double k) { harrisK_ = qBound(0.01, k, 0.2); }

    double harrisThreshold() const { return harrisThreshold_; }
    void setHarrisThreshold(double t) { harrisThreshold_ = qBound(0.0, t, 1.0); }

    // Shi-Tomasi参数
    int maxCorners() const { return maxCorners_; }
    void setMaxCorners(int count) { maxCorners_ = qMax(1, count); }

    double qualityLevel() const { return qualityLevel_; }
    void setQualityLevel(double q) { qualityLevel_ = qBound(0.001, q, 1.0); }

    double minDistance() const { return minDistance_; }
    void setMinDistance(double d) { minDistance_ = qMax(0.0, d); }

    // FAST参数
    int fastThreshold() const { return fastThreshold_; }
    void setFastThreshold(int t) { fastThreshold_ = qBound(1, t, 255); }

    bool fastNMS() const { return fastNMS_; }
    void setFastNMS(bool nms) { fastNMS_ = nms; }

    // 通用参数
    bool useSubPixel() const { return useSubPixel_; }
    void setUseSubPixel(bool use) { useSubPixel_ = use; }

    int subPixelWinSize() const { return subPixelWinSize_; }
    void setSubPixelWinSize(int size) { subPixelWinSize_ = size | 1; }

    // ========== 结果 ==========

    const std::vector<CornerPoint>& corners() const { return corners_; }

private:
    bool processHarris(const cv::Mat& gray);
    bool processShiTomasi(const cv::Mat& gray);
    bool processFAST(const cv::Mat& gray);
    bool processORB(const cv::Mat& gray);

private:
    Method method_ = ShiTomasi;

    // Harris参数
    int harrisBlockSize_ = 2;
    int harrisKSize_ = 3;
    double harrisK_ = 0.04;
    double harrisThreshold_ = 0.01;

    // Shi-Tomasi参数
    int maxCorners_ = 100;
    double qualityLevel_ = 0.01;
    double minDistance_ = 10.0;
    int blockSize_ = 3;

    // FAST参数
    int fastThreshold_ = 10;
    bool fastNMS_ = true;
    int fastType_ = cv::FastFeatureDetector::TYPE_9_16;

    // 通用参数
    bool useSubPixel_ = true;
    int subPixelWinSize_ = 5;

    // 结果
    std::vector<CornerPoint> corners_;
};

} // namespace Algorithm
} // namespace VisionForge
