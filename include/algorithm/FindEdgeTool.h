/**
 * @file FindEdgeTool.h
 * @brief 边缘查找工具
 * @details 在指定搜索区域内查找边缘点，返回边缘位置坐标
 * @author VisionForge Team
 * @date 2025-12-18
 */

#pragma once

#include "algorithm/VisionTool.h"
#include <QPointF>
#include <QLineF>
#include <vector>

namespace VisionForge {
namespace Algorithm {

/**
 * @brief 边缘查找结果
 */
struct EdgePoint {
    int id;                     // 边缘点ID
    QPointF position;           // 边缘位置
    double amplitude;           // 边缘幅值（梯度强度）
    double direction;           // 边缘方向（度）
    bool isRising;              // 是否为上升沿（暗到亮）

    EdgePoint()
        : id(0), amplitude(0), direction(0), isRising(true) {}

    EdgePoint(int i, const QPointF& pos, double amp, double dir, bool rising)
        : id(i), position(pos), amplitude(amp), direction(dir), isRising(rising) {}
};

/**
 * @brief 边缘查找工具
 *
 * 在指定的搜索线段或矩形区域内查找边缘点
 * 支持单边缘查找和多边缘查找
 */
class FindEdgeTool : public VisionTool {
    Q_OBJECT

public:
    /**
     * @brief 搜索模式
     */
    enum SearchMode {
        LineSearch,         // 沿线段搜索
        RectSearch          // 矩形区域搜索
    };
    Q_ENUM(SearchMode)

    /**
     * @brief 边缘类型
     */
    enum EdgeType {
        AllEdges,           // 所有边缘
        RisingEdge,         // 上升沿（暗到亮）
        FallingEdge         // 下降沿（亮到暗）
    };
    Q_ENUM(EdgeType)

    /**
     * @brief 选择模式
     */
    enum SelectMode {
        FirstEdge,          // 第一个边缘
        LastEdge,           // 最后一个边缘
        StrongestEdge,      // 最强边缘
        AllEdgesFound       // 所有边缘
    };
    Q_ENUM(SelectMode)

    explicit FindEdgeTool(QObject* parent = nullptr);

    // ========== VisionTool接口实现 ==========

    ToolType toolType() const override { return FindEdge; }
    QString toolName() const override { return tr("边缘查找"); }

    bool process(const Base::ImageData::Ptr& input, ToolResult& output) override;

    QJsonObject serializeParams() const override;
    void deserializeParams(const QJsonObject& json) override;

    // ========== 搜索区域设置 ==========

    /**
     * @brief 设置搜索模式
     */
    void setSearchMode(SearchMode mode);
    SearchMode searchMode() const { return searchMode_; }

    /**
     * @brief 设置搜索线段（LineSearch模式）
     */
    void setSearchLine(const QPointF& start, const QPointF& end);
    QLineF searchLine() const { return QLineF(lineStart_, lineEnd_); }

    /**
     * @brief 设置搜索宽度（垂直于搜索方向的采样宽度）
     */
    void setSearchWidth(int width);
    int searchWidth() const { return searchWidth_; }

    /**
     * @brief 设置搜索矩形（RectSearch模式）
     */
    void setSearchRect(const QRectF& rect, double angle = 0);
    QRectF searchRect() const { return searchRect_; }
    double searchAngle() const { return searchAngle_; }

    // ========== 边缘检测参数 ==========

    /**
     * @brief 设置边缘类型
     */
    void setEdgeType(EdgeType type);
    EdgeType edgeType() const { return edgeType_; }

    /**
     * @brief 设置选择模式
     */
    void setSelectMode(SelectMode mode);
    SelectMode selectMode() const { return selectMode_; }

    /**
     * @brief 设置最小边缘幅值阈值
     */
    void setMinAmplitude(double amplitude);
    double minAmplitude() const { return minAmplitude_; }

    /**
     * @brief 设置高斯平滑sigma
     */
    void setSigma(double sigma);
    double sigma() const { return sigma_; }

    // ========== 结果获取 ==========

    /**
     * @brief 获取找到的边缘点
     */
    const std::vector<EdgePoint>& edgePoints() const { return edgePoints_; }

    /**
     * @brief 获取边缘点数量
     */
    int edgeCount() const { return static_cast<int>(edgePoints_.size()); }

    /**
     * @brief 获取主边缘点（根据SelectMode选择）
     */
    EdgePoint primaryEdge() const;

private:
    /**
     * @brief 沿线段查找边缘
     */
    bool findEdgesAlongLine(const cv::Mat& gray, ToolResult& output);

    /**
     * @brief 在矩形区域查找边缘
     */
    bool findEdgesInRect(const cv::Mat& gray, ToolResult& output);

    /**
     * @brief 计算一维边缘
     */
    std::vector<EdgePoint> detect1DEdges(const std::vector<double>& profile,
                                         const QPointF& start, const QPointF& dir);

    /**
     * @brief 提取投影剖面
     */
    std::vector<double> extractProfile(const cv::Mat& gray,
                                       const QPointF& start, const QPointF& end,
                                       int width);

    /**
     * @brief 绘制结果
     */
    cv::Mat drawResults(const cv::Mat& input) const;

private:
    // 搜索参数
    SearchMode searchMode_;
    QPointF lineStart_;
    QPointF lineEnd_;
    int searchWidth_;
    QRectF searchRect_;
    double searchAngle_;

    // 边缘检测参数
    EdgeType edgeType_;
    SelectMode selectMode_;
    double minAmplitude_;
    double sigma_;

    // 结果
    std::vector<EdgePoint> edgePoints_;
};

} // namespace Algorithm
} // namespace VisionForge
