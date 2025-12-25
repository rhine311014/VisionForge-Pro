/**
 * @file ICircleDetector.h
 * @brief 圆检测算法后端接口
 * @details 定义圆检测的抽象接口，支持OpenCV和Halcon双后端
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
 * @brief 圆检测结果
 */
struct CircleDetectResult {
    int id;                     // 圆ID
    QPointF center;             // 圆心坐标
    double radius;              // 半径
    double score;               // 匹配得分/置信度
    double circularity;         // 圆度（1.0为完美圆）
    double area;                // 面积
    bool isSubPixel;            // 是否为亚像素精度
    double fitResidual;         // 拟合残差

    CircleDetectResult()
        : id(0), radius(0), score(0), circularity(0), area(0),
          isSubPixel(false), fitResidual(0) {}
};

/**
 * @brief 圆检测参数
 */
struct CircleDetectParams {
    // 通用参数
    double minRadius = 10.0;        // 最小半径
    double maxRadius = 200.0;       // 最大半径
    int maxCount = 10;              // 最大检测数量

    // 霍夫参数 (OpenCV)
    double houghDp = 1.0;           // 分辨率比例
    double minDist = 50.0;          // 最小圆心距离
    double cannyThreshold = 100.0;  // Canny边缘检测阈值
    double accumThreshold = 30.0;   // 累加器阈值

    // 轮廓拟合参数
    double minCircularity = 0.8;    // 最小圆度
    double minArea = 100.0;         // 最小面积

    // 亚像素参数
    bool subPixelEnabled = false;   // 是否启用亚像素
};

/**
 * @brief 圆检测算法后端接口
 *
 * 抽象接口定义，所有圆检测实现（OpenCV/Halcon）都必须实现此接口。
 * 这样可以在业务代码中消除 #ifdef USE_HALCON，提高代码可读性。
 */
class ICircleDetector {
public:
    virtual ~ICircleDetector() = default;

    /**
     * @brief 获取后端名称
     * @return 后端名称（如 "OpenCV" 或 "Halcon"）
     */
    virtual QString backendName() const = 0;

    /**
     * @brief 检测圆形
     * @param image 输入图像（灰度图）
     * @param params 检测参数
     * @param results 输出结果列表
     * @param errorMsg 错误消息（如果失败）
     * @return 是否成功
     */
    virtual bool detect(const cv::Mat& image,
                       const CircleDetectParams& params,
                       std::vector<CircleDetectResult>& results,
                       QString& errorMsg) = 0;

    /**
     * @brief 是否支持特定检测方法
     * @param method 方法名称
     * @return true如果支持
     */
    virtual bool supportsMethod(const QString& method) const = 0;

    /**
     * @brief 获取支持的检测方法列表
     * @return 方法名称列表
     */
    virtual QStringList supportedMethods() const = 0;
};

/**
 * @brief 圆检测器智能指针类型
 */
using ICircleDetectorPtr = std::unique_ptr<ICircleDetector>;

} // namespace Backend
} // namespace Algorithm
} // namespace VisionForge
