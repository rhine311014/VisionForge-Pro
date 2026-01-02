/**
 * @file PatternSearchTool.h
 * @brief 图案搜索工具
 * @details 基于Halcon形状匹配的高精度图案搜索
 * @author VisionForge Team
 * @date 2025-12-30
 */

#pragma once

#include "algorithm/VisionTool.h"
#include <vector>
#include <QRectF>

#ifdef USE_HALCON
#include <HalconCpp.h>
#endif

namespace VisionForge {
namespace Algorithm {

/**
 * @brief 图案搜索匹配结果
 */
struct PatternMatch {
    double row = 0;         ///< 行坐标（Y）
    double col = 0;         ///< 列坐标（X）
    double angle = 0;       ///< 角度（弧度）
    double scale = 1.0;     ///< 缩放比例
    double score = 0;       ///< 匹配分数 [0,1]
};

/**
 * @brief 图案搜索工具
 *
 * 使用Halcon形状匹配算法进行高精度图案搜索。
 * 支持：
 * - 灰度匹配/边缘匹配/相关匹配
 * - 角度范围搜索
 * - 缩放范围搜索
 * - 亚像素精度
 */
class PatternSearchTool : public VisionTool
{
    Q_OBJECT

public:
    /**
     * @brief 匹配模式
     */
    enum MatchMode {
        GrayMatch = 0,      ///< 灰度匹配
        EdgeMatch,          ///< 边缘匹配（形状匹配）
        CorrelationMatch    ///< 相关匹配
    };

    /**
     * @brief 亚像素精度
     */
    enum SubPixelMode {
        None = 0,               ///< 无亚像素
        Interpolation,          ///< 插值
        LeastSquares,           ///< 最小二乘
        LeastSquaresHigh        ///< 高精度最小二乘
    };

    explicit PatternSearchTool(QObject* parent = nullptr);
    ~PatternSearchTool() override;

    // VisionTool接口实现
    ToolType toolType() const override { return Custom; }  // 或扩展ToolType
    QString toolName() const override { return tr("图案搜索"); }
    bool process(const Base::ImageData::Ptr& input, ToolResult& output) override;
    QJsonObject serializeParams() const override;
    void deserializeParams(const QJsonObject& json) override;

    // ========== 模板管理 ==========

    /**
     * @brief 从ROI创建模板
     * @param image 源图像
     * @param roi ROI区域
     * @return 是否成功
     */
    bool createTemplate(const Base::ImageData::Ptr& image, const QRectF& roi);

    /**
     * @brief 加载模板文件
     */
    bool loadTemplate(const QString& filePath);

    /**
     * @brief 保存模板文件
     */
    bool saveTemplate(const QString& filePath);

    /**
     * @brief 清除模板
     */
    void clearTemplate();

    /**
     * @brief 是否有模板
     */
    bool hasTemplate() const { return hasTemplate_; }

    // ========== 参数设置 ==========

    // 匹配模式
    MatchMode matchMode() const { return matchMode_; }
    void setMatchMode(MatchMode mode) { matchMode_ = mode; }

    // 最小匹配分数
    double minScore() const { return minScore_; }
    void setMinScore(double score) { minScore_ = qBound(0.0, score, 1.0); }

    // 最大匹配数
    int maxMatches() const { return maxMatches_; }
    void setMaxMatches(int count) { maxMatches_ = qMax(1, count); }

    // 重叠阈值
    double overlapThreshold() const { return overlapThreshold_; }
    void setOverlapThreshold(double threshold) { overlapThreshold_ = qBound(0.0, threshold, 1.0); }

    // 亚像素模式
    SubPixelMode subPixelMode() const { return subPixelMode_; }
    void setSubPixelMode(SubPixelMode mode) { subPixelMode_ = mode; }

    // 金字塔层数
    int pyramidLevels() const { return pyramidLevels_; }
    void setPyramidLevels(int levels) { pyramidLevels_ = qBound(1, levels, 10); }

    // 角度搜索
    bool angleSearchEnabled() const { return angleSearchEnabled_; }
    void setAngleSearchEnabled(bool enabled) { angleSearchEnabled_ = enabled; }

    double angleMin() const { return angleMin_; }
    double angleMax() const { return angleMax_; }
    double angleStep() const { return angleStep_; }
    void setAngleRange(double min, double max, double step);

    // 缩放搜索
    bool scaleSearchEnabled() const { return scaleSearchEnabled_; }
    void setScaleSearchEnabled(bool enabled) { scaleSearchEnabled_ = enabled; }

    double scaleMin() const { return scaleMin_; }
    double scaleMax() const { return scaleMax_; }
    double scaleStep() const { return scaleStep_; }
    void setScaleRange(double min, double max, double step);

    // ========== 搜索结果 ==========

    /**
     * @brief 获取所有匹配结果
     */
    const std::vector<PatternMatch>& matches() const { return matches_; }

    /**
     * @brief 获取最佳匹配结果
     */
    PatternMatch bestMatch() const;

private:
    bool processHalcon(const Base::ImageData::Ptr& input, ToolResult& output);
    bool processOpenCV(const Base::ImageData::Ptr& input, ToolResult& output);

private:
    // 参数
    MatchMode matchMode_ = EdgeMatch;
    double minScore_ = 0.5;
    int maxMatches_ = 10;
    double overlapThreshold_ = 0.5;
    SubPixelMode subPixelMode_ = LeastSquares;
    int pyramidLevels_ = 4;

    // 角度搜索
    bool angleSearchEnabled_ = true;
    double angleMin_ = -30.0;
    double angleMax_ = 30.0;
    double angleStep_ = 1.0;

    // 缩放搜索
    bool scaleSearchEnabled_ = false;
    double scaleMin_ = 0.8;
    double scaleMax_ = 1.2;
    double scaleStep_ = 0.05;

    // 状态
    bool hasTemplate_ = false;
    std::vector<PatternMatch> matches_;

#ifdef USE_HALCON
    HalconCpp::HShapeModel shapeModel_;
    HalconCpp::HImage templateImage_;
#endif
};

} // namespace Algorithm
} // namespace VisionForge
