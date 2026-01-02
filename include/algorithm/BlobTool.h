/**
 * @file BlobTool.h
 * @brief Blob分析工具头文件
 * @details 实现连通域分析(Blob Analysis)算法，用于检测和分析图像中的
 *          连通区域。支持Halcon和OpenCV双后端，可根据需要灵活切换。
 *
 * @author VisionForge Team
 * @version 1.0.0
 * @date 2025-12-18
 * @copyright Copyright (c) 2025 VisionForge. All rights reserved.
 *
 * @par 功能特性:
 * - 连通域检测（4连通/8连通）
 * - 丰富的几何特征计算
 * - 灵活的过滤条件配置
 * - 多种排序方式支持
 * - Halcon/OpenCV双后端
 *
 * @par 应用场景:
 * - 缺陷检测（划痕、污点、气泡）
 * - 目标计数
 * - 尺寸测量
 * - 形状分类
 */

#pragma once

#include "algorithm/VisionTool.h"
#include "algorithm/backend/IBlobAnalyzer.h"
#include "algorithm/backend/AlgorithmBackendFactory.h"
#include <vector>
#include <QRectF>
#include <QPointF>

namespace VisionForge {
namespace Algorithm {

/**
 * @brief Blob特征结构体
 * @details 存储单个Blob(连通域)的完整几何特征信息，
 *          用于后续的形状分析和筛选
 */
struct BlobFeature {
    // ========== 基本属性 ==========
    int id;                     ///< Blob唯一标识ID
    double area;                ///< 面积 - 像素数量
    double perimeter;           ///< 周长 - 边界像素长度

    // ========== 形状因子 ==========
    double circularity;         ///< 圆度 = 4*PI*area/perimeter^2，完美圆=1.0
    double rectangularity;      ///< 矩形度 = 面积/最小外接矩形面积，完美矩形=1.0
    double aspectRatio;         ///< 长宽比 = 外接矩形宽/高
    double compactness;         ///< 紧凑度 - 形状紧凑程度度量
    double convexity;           ///< 凸度 = 面积/凸包面积，完全凸=1.0
    double elongation;          ///< 伸长率 - 形状拉伸程度
    double orientation;         ///< 主方向角(度) - 主轴与水平方向夹角

    // ========== 位置信息 ==========
    QPointF center;             ///< 质心坐标 - 区域几何中心
    QRectF boundingBox;         ///< 外接矩形 - 轴对齐包围盒

    // ========== 最小外接矩形 ==========
    double minBoundingArea;     ///< 最小外接矩形面积
    double minBoundingWidth;    ///< 最小外接矩形宽度
    double minBoundingHeight;   ///< 最小外接矩形高度
    double minBoundingAngle;    ///< 最小外接矩形旋转角度(度)

    // ========== 等效特征 ==========
    double equivalentDiameter;  ///< 等效圆直径 = sqrt(4*area/PI)
    double majorAxis;           ///< 椭圆拟合长轴长度
    double minorAxis;           ///< 椭圆拟合短轴长度

    // ========== 轮廓数据 ==========
    std::vector<cv::Point> contour;   ///< 轮廓点集(OpenCV格式)

    /**
     * @brief 默认构造函数
     * @details 初始化所有数值成员为0
     */
    BlobFeature()
        : id(0), area(0), perimeter(0), circularity(0), rectangularity(0)
        , aspectRatio(0), compactness(0), convexity(0), elongation(0), orientation(0)
        , minBoundingArea(0), minBoundingWidth(0), minBoundingHeight(0), minBoundingAngle(0)
        , equivalentDiameter(0), majorAxis(0), minorAxis(0) {}
};

/**
 * @brief Blob过滤条件结构体
 * @details 定义用于筛选Blob的条件，支持多种特征的范围过滤
 */
struct BlobFilter {
    /**
     * @brief 过滤器类型枚举
     * @details 定义可用于过滤的特征类型，
     *          Min表示最小值约束，Max表示最大值约束
     */
    enum FilterType {
        AreaMin,                ///< 最小面积约束
        AreaMax,                ///< 最大面积约束
        PerimeterMin,           ///< 最小周长约束
        PerimeterMax,           ///< 最大周长约束
        CircularityMin,         ///< 最小圆度约束 [0,1]
        CircularityMax,         ///< 最大圆度约束 [0,1]
        RectangularityMin,      ///< 最小矩形度约束 [0,1]
        RectangularityMax,      ///< 最大矩形度约束 [0,1]
        AspectRatioMin,         ///< 最小长宽比约束
        AspectRatioMax,         ///< 最大长宽比约束
        WidthMin,               ///< 最小宽度约束
        WidthMax,               ///< 最大宽度约束
        HeightMin,              ///< 最小高度约束
        HeightMax               ///< 最大高度约束
    };

    FilterType type;    ///< 过滤器类型
    double value;       ///< 过滤阈值
    bool enabled;       ///< 是否启用此过滤器

    /** @brief 默认构造函数 */
    BlobFilter() : type(AreaMin), value(0), enabled(false) {}

    /**
     * @brief 带参数构造函数
     * @param t 过滤器类型
     * @param v 阈值
     * @param e 是否启用，默认true
     */
    BlobFilter(FilterType t, double v, bool e = true) : type(t), value(v), enabled(e) {}
};

/**
 * @brief Blob分析工具类
 * @details 实现图像连通域分析功能，是机器视觉中常用的区域检测工具。
 *          通过对二值图像进行连通域标记，提取各区域的几何特征，
 *          并根据条件过滤筛选目标区域。
 *
 * @par 算法原理:
 * 1. 图像二值化（自动或手动阈值）
 * 2. 连通域标记（4连通或8连通）
 * 3. 几何特征计算
 * 4. 条件过滤
 * 5. 结果排序
 *
 * @par 支持的后端:
 * - OpenCV: 使用connectedComponentsWithStats
 * - Halcon: 使用connection和select_shape
 *
 * @see VisionTool, BlobFeature, BlobFilter
 */
class BlobTool : public VisionTool {
    Q_OBJECT

public:
    /**
     * @brief 后端类型枚举
     * @details 选择使用的图像处理库后端
     */
    enum BackendType {
        Auto,           ///< 自动选择（优先Halcon，无则使用OpenCV）
        OpenCV,         ///< 强制使用OpenCV后端
        Halcon          ///< 强制使用Halcon后端
    };
    Q_ENUM(BackendType)

    /**
     * @brief 连通性类型枚举
     * @details 定义像素的连通关系判断方式
     */
    enum Connectivity {
        Connect4,       ///< 4连通 - 仅上下左右视为相邻
        Connect8        ///< 8连通 - 包含对角线方向，共8个相邻像素
    };
    Q_ENUM(Connectivity)

    /**
     * @brief 极性枚举
     * @details 定义检测目标的灰度特性
     */
    enum Polarity {
        Light,          ///< 检测亮区域 - 前景为白色/高灰度
        Dark,           ///< 检测暗区域 - 前景为黑色/低灰度
        Both            ///< 两者都检测
    };
    Q_ENUM(Polarity)

    /**
     * @brief 排序方式枚举
     * @details 定义检测结果的排序规则
     */
    enum SortBy {
        NoSort,             ///< 不排序，保持检测顺序
        SortByArea,         ///< 按面积排序
        SortByX,            ///< 按质心X坐标排序
        SortByY,            ///< 按质心Y坐标排序
        SortByCircularity   ///< 按圆度排序
    };
    Q_ENUM(SortBy)

    /**
     * @brief 构造函数
     * @param parent 父对象指针
     */
    explicit BlobTool(QObject* parent = nullptr);

    // ========== VisionTool接口实现 ==========

    ToolType toolType() const override { return BlobAnalysis; }
    QString toolName() const override { return tr("Blob分析"); }

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
     * @brief 设置连通性
     */
    void setConnectivity(Connectivity conn);
    Connectivity connectivity() const { return connectivity_; }

    /**
     * @brief 设置极性
     */
    void setPolarity(Polarity pol);
    Polarity polarity() const { return polarity_; }

    /**
     * @brief 设置排序方式
     */
    void setSortBy(SortBy sort);
    SortBy sortBy() const { return sortBy_; }

    /**
     * @brief 设置排序方向（true=降序）
     */
    void setSortDescending(bool desc);
    bool sortDescending() const { return sortDescending_; }

    /**
     * @brief 设置最大返回数量（0=不限制）
     */
    void setMaxCount(int count);
    int maxCount() const { return maxCount_; }

    /**
     * @brief 设置二值化阈值（如果输入不是二值图像）
     */
    void setThreshold(int threshold);
    int threshold() const { return threshold_; }

    /**
     * @brief 设置是否自动二值化
     */
    void setAutoThreshold(bool autoTh);
    bool autoThreshold() const { return autoThreshold_; }

    // ========== 过滤器管理 ==========

    /**
     * @brief 添加过滤条件
     */
    void addFilter(const BlobFilter& filter);

    /**
     * @brief 移除过滤条件
     */
    void removeFilter(int index);

    /**
     * @brief 清空所有过滤条件
     */
    void clearFilters();

    /**
     * @brief 获取过滤条件列表
     */
    const std::vector<BlobFilter>& filters() const { return filters_; }

    /**
     * @brief 设置面积范围
     */
    void setAreaRange(double minArea, double maxArea);

    /**
     * @brief 设置圆度范围
     */
    void setCircularityRange(double minCirc, double maxCirc);

    // ========== 结果获取 ==========

    /**
     * @brief 获取检测到的Blob特征列表
     */
    const std::vector<BlobFeature>& blobFeatures() const { return blobFeatures_; }

    /**
     * @brief 获取Blob数量
     */
    int blobCount() const { return static_cast<int>(blobFeatures_.size()); }

private:
    /**
     * @brief 应用过滤器
     */
    std::vector<BlobFeature> applyFilters(const std::vector<BlobFeature>& blobs) const;

    /**
     * @brief 排序Blob
     */
    void sortBlobs(std::vector<BlobFeature>& blobs) const;

    /**
     * @brief 绘制结果到图像
     */
    cv::Mat drawResults(const cv::Mat& input) const;

    /**
     * @brief 确保分析器已创建
     */
    void ensureAnalyzer();

    /**
     * @brief 转换为后端参数
     */
    Backend::BlobAnalyzeParams toBackendParams() const;

private:
    BackendType backend_;           // 后端类型
    Connectivity connectivity_;     // 连通性
    Polarity polarity_;             // 极性
    SortBy sortBy_;                 // 排序方式
    bool sortDescending_;           // 降序排列
    int maxCount_;                  // 最大返回数量
    int threshold_;                 // 二值化阈值
    bool autoThreshold_;            // 自动二值化

    std::vector<BlobFilter> filters_;       // 过滤条件
    std::vector<BlobFeature> blobFeatures_; // 检测结果

    // 后端分析器
    Backend::IBlobAnalyzerPtr analyzer_;
};

} // namespace Algorithm
} // namespace VisionForge
