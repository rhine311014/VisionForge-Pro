/**
 * @file LineTool.h
 * @brief 直线检测工具头文件
 * @details 实现图像中直线目标的检测与定位，支持霍夫变换、轮廓拟合、
 *          边缘拟合等多种算法，并提供OpenCV和Halcon双后端支持。
 *
 * @author VisionForge Team
 * @version 1.0.0
 * @date 2025-12-18
 * @copyright Copyright (c) 2025 VisionForge. All rights reserved.
 *
 * @par 功能特性:
 * - 标准霍夫变换检测
 * - 概率霍夫变换检测
 * - 轮廓拟合检测
 * - 边缘拟合检测
 * - OpenCV/Halcon双后端
 *
 * @par 应用场景:
 * - 边缘检测与测量
 * - 平行度/垂直度测量
 * - 引导线检测
 * - 基准线定位
 */

#pragma once

#include "algorithm/VisionTool.h"
#include "algorithm/backend/ILineDetector.h"
#include "algorithm/backend/AlgorithmBackendFactory.h"
#include <vector>
#include <QPointF>
#include <QLineF>

namespace VisionForge {
namespace Algorithm {

/**
 * @brief 直线检测结果结构体
 * @details 存储单条检测到的直线的完整信息，包括端点、角度、直线方程等
 */
struct LineResult {
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

    LineResult()
        : id(0), angle(0), length(0), score(0), a(0), b(1), c(0) {}

    LineResult(int i, const QPointF& start, const QPointF& end, double s = 1.0)
        : id(i), startPoint(start), endPoint(end), score(s)
    {
        // 计算长度
        double dx = end.x() - start.x();
        double dy = end.y() - start.y();
        length = std::sqrt(dx * dx + dy * dy);

        // 计算角度（相对于水平方向）
        angle = std::atan2(dy, dx) * 180.0 / M_PI;
        if (angle < 0) angle += 180;

        // 计算直线方程参数
        a = dy;
        b = -dx;
        c = dx * start.y() - dy * start.x();

        // 归一化
        double norm = std::sqrt(a * a + b * b);
        if (norm > 0) {
            a /= norm;
            b /= norm;
            c /= norm;
        }
    }

    /**
     * @brief 获取QLineF表示
     */
    QLineF toQLineF() const {
        return QLineF(startPoint, endPoint);
    }
};

/**
 * @brief 线检测工具
 *
 * 支持多种线检测算法，可使用Halcon或OpenCV后端
 */
class LineTool : public VisionTool {
    Q_OBJECT

public:
    /**
     * @brief 后端类型
     */
    enum BackendType {
        Auto,           // 自动选择（优先Halcon）
        OpenCV,         // OpenCV后端
        Halcon          // Halcon后端
    };
    Q_ENUM(BackendType)

    /**
     * @brief 检测方法
     */
    enum DetectionMethod {
        HoughLinesP,        // 概率霍夫变换（OpenCV）
        HoughLines,         // 标准霍夫变换（OpenCV）
        ContourFit,         // 轮廓拟合（通用）
        EdgeFit             // 边缘拟合（Halcon）
    };
    Q_ENUM(DetectionMethod)

    explicit LineTool(QObject* parent = nullptr);

    // ========== VisionTool接口实现 ==========

    ToolType toolType() const override { return FindLine; }
    QString toolName() const override { return tr("线检测"); }

    bool process(const Base::ImageData::Ptr& input, ToolResult& output) override;

    QJsonObject serializeParams() const override;
    void deserializeParams(const QJsonObject& json) override;

    // ========== 参数设置 ==========

    /**
     * @brief 设置后端类型
     */
    void setBackend(BackendType backend);
    BackendType backend() const { return backend_; }

    /**
     * @brief 设置检测方法
     */
    void setMethod(DetectionMethod method);
    DetectionMethod method() const { return method_; }

    /**
     * @brief 设置最小线段长度
     */
    void setMinLength(double length);
    double minLength() const { return minLength_; }

    /**
     * @brief 设置最大间隙（用于合并线段）
     */
    void setMaxGap(double gap);
    double maxGap() const { return maxGap_; }

    /**
     * @brief 设置最大检测数量
     */
    void setMaxCount(int count);
    int maxCount() const { return maxCount_; }

    /**
     * @brief 设置霍夫参数 - 距离分辨率
     */
    void setRho(double rho);
    double rho() const { return rho_; }

    /**
     * @brief 设置霍夫参数 - 角度分辨率（度）
     */
    void setTheta(double theta);
    double theta() const { return theta_; }

    /**
     * @brief 设置霍夫参数 - 累加器阈值
     */
    void setThreshold(int threshold);
    int threshold() const { return threshold_; }

    /**
     * @brief 设置Canny边缘检测阈值1
     */
    void setCannyThreshold1(double threshold);
    double cannyThreshold1() const { return cannyThreshold1_; }

    /**
     * @brief 设置Canny边缘检测阈值2
     */
    void setCannyThreshold2(double threshold);
    double cannyThreshold2() const { return cannyThreshold2_; }

    // ========== 结果获取 ==========

    /**
     * @brief 获取检测到的线列表
     */
    const std::vector<LineResult>& lines() const { return lines_; }

    /**
     * @brief 获取线数量
     */
    int lineCount() const { return static_cast<int>(lines_.size()); }

private:
    /**
     * @brief 绘制检测结果
     */
    cv::Mat drawResults(const cv::Mat& input) const;

    /**
     * @brief 确保检测器已创建
     */
    void ensureDetector();

    /**
     * @brief 转换为后端参数
     */
    Backend::LineDetectParams toBackendParams() const;

private:
    BackendType backend_;           // 后端类型
    DetectionMethod method_;        // 检测方法

    // 通用参数
    double minLength_;              // 最小线段长度
    double maxGap_;                 // 最大间隙
    int maxCount_;                  // 最大检测数量

    // 霍夫参数
    double rho_;                    // 距离分辨率
    double theta_;                  // 角度分辨率（度）
    int threshold_;                 // 累加器阈值

    // Canny参数
    double cannyThreshold1_;        // Canny低阈值
    double cannyThreshold2_;        // Canny高阈值

    std::vector<LineResult> lines_; // 检测结果

    // 后端检测器（无需 #ifdef）
    Backend::ILineDetectorPtr detector_;
};

} // namespace Algorithm
} // namespace VisionForge
