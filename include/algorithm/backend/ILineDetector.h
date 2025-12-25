/**
 * @file ILineDetector.h
 * @brief 线检测算法后端接口
 * @details 定义线检测的抽象接口，支持OpenCV和Halcon双后端
 * @author VisionForge Team
 * @date 2025-12-25
 */

#pragma once

#include "base/ImageData.h"
#include <QString>
#include <QPointF>
#include <vector>
#include <memory>

namespace VisionForge {
namespace Algorithm {
namespace Backend {

/**
 * @brief 线检测结果
 */
struct LineDetectResult {
    int id;                     // 线ID
    QPointF startPoint;         // 起点
    QPointF endPoint;           // 终点
    double angle;               // 角度（度，0-180）
    double length;              // 长度
    double score;               // 匹配得分/置信度

    // 直线方程参数 (ax + by + c = 0)
    double a;
    double b;
    double c;

    LineDetectResult()
        : id(0), angle(0), length(0), score(0), a(0), b(1), c(0) {}
};

/**
 * @brief 线检测参数
 */
struct LineDetectParams {
    // 检测方法
    QString method = "HoughLinesP"; // HoughLinesP, HoughLines, ContourFit, EdgeFit

    // 通用参数
    double minLength = 50.0;        // 最小线段长度
    double maxGap = 10.0;           // 最大间隙（用于合并线段）
    int maxCount = 20;              // 最大检测数量

    // 霍夫参数 (OpenCV)
    double rho = 1.0;               // 距离分辨率
    double theta = 1.0;             // 角度分辨率（度）
    int threshold = 50;             // 累加器阈值

    // Canny参数
    double cannyThreshold1 = 50.0;  // Canny低阈值
    double cannyThreshold2 = 150.0; // Canny高阈值
};

/**
 * @brief 线检测算法后端接口
 */
class ILineDetector {
public:
    virtual ~ILineDetector() = default;

    /**
     * @brief 获取后端名称
     */
    virtual QString backendName() const = 0;

    /**
     * @brief 检测直线
     * @param image 输入图像（灰度图）
     * @param params 检测参数
     * @param results 输出结果列表
     * @param errorMsg 错误消息（如果失败）
     * @return 是否成功
     */
    virtual bool detect(const cv::Mat& image,
                       const LineDetectParams& params,
                       std::vector<LineDetectResult>& results,
                       QString& errorMsg) = 0;

    /**
     * @brief 是否支持特定检测方法
     */
    virtual bool supportsMethod(const QString& method) const = 0;

    /**
     * @brief 获取支持的检测方法列表
     */
    virtual QStringList supportedMethods() const = 0;
};

using ILineDetectorPtr = std::unique_ptr<ILineDetector>;

} // namespace Backend
} // namespace Algorithm
} // namespace VisionForge
