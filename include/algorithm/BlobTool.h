/**
 * @file BlobTool.h
 * @brief Blob分析工具
 * @details 连通域分析，支持Halcon和OpenCV双后端
 * @author VisionForge Team
 * @date 2025-12-18
 */

#pragma once

#include "algorithm/VisionTool.h"
#include <vector>
#include <QRectF>
#include <QPointF>

namespace VisionForge {
namespace Algorithm {

/**
 * @brief Blob特征结构
 */
struct BlobFeature {
    int id;                     // Blob ID
    double area;                // 面积（像素数）
    double perimeter;           // 周长
    double circularity;         // 圆度 (4 * PI * area / perimeter^2)
    double rectangularity;      // 矩形度（面积/最小外接矩形面积）
    double aspectRatio;         // 长宽比
    double compactness;         // 紧凑度
    double convexity;           // 凸度（面积/凸包面积）
    double elongation;          // 伸长率
    double orientation;         // 方向角（度）

    QPointF center;             // 质心
    QRectF boundingBox;         // 外接矩形
    double minBoundingArea;     // 最小外接矩形面积
    double minBoundingWidth;    // 最小外接矩形宽
    double minBoundingHeight;   // 最小外接矩形高
    double minBoundingAngle;    // 最小外接矩形角度

    double equivalentDiameter;  // 等效圆直径
    double majorAxis;           // 椭圆拟合长轴
    double minorAxis;           // 椭圆拟合短轴

    std::vector<cv::Point> contour;   // 轮廓点（OpenCV格式）

    BlobFeature()
        : id(0), area(0), perimeter(0), circularity(0), rectangularity(0)
        , aspectRatio(0), compactness(0), convexity(0), elongation(0), orientation(0)
        , minBoundingArea(0), minBoundingWidth(0), minBoundingHeight(0), minBoundingAngle(0)
        , equivalentDiameter(0), majorAxis(0), minorAxis(0) {}
};

/**
 * @brief Blob过滤条件
 */
struct BlobFilter {
    enum FilterType {
        AreaMin,                // 最小面积
        AreaMax,                // 最大面积
        PerimeterMin,           // 最小周长
        PerimeterMax,           // 最大周长
        CircularityMin,         // 最小圆度
        CircularityMax,         // 最大圆度
        RectangularityMin,      // 最小矩形度
        RectangularityMax,      // 最大矩形度
        AspectRatioMin,         // 最小长宽比
        AspectRatioMax,         // 最大长宽比
        WidthMin,               // 最小宽度
        WidthMax,               // 最大宽度
        HeightMin,              // 最小高度
        HeightMax               // 最大高度
    };

    FilterType type;
    double value;
    bool enabled;

    BlobFilter() : type(AreaMin), value(0), enabled(false) {}
    BlobFilter(FilterType t, double v, bool e = true) : type(t), value(v), enabled(e) {}
};

/**
 * @brief Blob分析工具
 *
 * 支持连通域分析和特征提取，
 * 可使用Halcon或OpenCV后端
 */
class BlobTool : public VisionTool {
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
     * @brief 连通性类型
     */
    enum Connectivity {
        Connect4,       // 4连通
        Connect8        // 8连通
    };
    Q_ENUM(Connectivity)

    /**
     * @brief 极性（检测亮区域还是暗区域）
     */
    enum Polarity {
        Light,          // 检测亮区域（前景为白色）
        Dark,           // 检测暗区域（前景为黑色）
        Both            // 两者都检测
    };
    Q_ENUM(Polarity)

    /**
     * @brief 排序方式
     */
    enum SortBy {
        NoSort,         // 不排序
        SortByArea,     // 按面积排序
        SortByX,        // 按X坐标排序
        SortByY,        // 按Y坐标排序
        SortByCircularity   // 按圆度排序
    };
    Q_ENUM(SortBy)

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
     * @brief 使用OpenCV处理
     */
    bool processWithOpenCV(const cv::Mat& input, ToolResult& output);

#ifdef USE_HALCON
    /**
     * @brief 使用Halcon处理
     */
    bool processWithHalcon(const cv::Mat& input, ToolResult& output);
#endif

    /**
     * @brief 应用过滤器
     */
    std::vector<BlobFeature> applyFilters(const std::vector<BlobFeature>& blobs) const;

    /**
     * @brief 排序Blob
     */
    void sortBlobs(std::vector<BlobFeature>& blobs) const;

    /**
     * @brief 计算Blob特征（OpenCV）
     */
    BlobFeature calculateFeatures(const std::vector<cv::Point>& contour, int id) const;

    /**
     * @brief 绘制结果到图像
     */
    cv::Mat drawResults(const cv::Mat& input) const;

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
};

} // namespace Algorithm
} // namespace VisionForge
