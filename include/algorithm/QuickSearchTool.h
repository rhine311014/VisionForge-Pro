/**
 * @file QuickSearchTool.h
 * @brief 快速搜索工具
 * @details 轻量级的快速模板匹配工具
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
 * @brief 快速搜索匹配结果
 */
struct QuickMatch {
    double x = 0;           ///< X坐标
    double y = 0;           ///< Y坐标
    double score = 0;       ///< 匹配分数 [0,1]
    cv::Rect rect;          ///< 匹配矩形
};

/**
 * @brief 快速搜索工具
 *
 * 使用OpenCV模板匹配的轻量级快速搜索工具。
 * 特点：
 * - 简单高效的模板匹配
 * - 多尺度金字塔加速
 * - 支持多种匹配方法
 */
class QuickSearchTool : public VisionTool
{
    Q_OBJECT

public:
    /**
     * @brief 速度等级
     */
    enum SpeedLevel {
        UltraFast = 0,      ///< 极速
        Fast,               ///< 快速
        Normal,             ///< 标准
        Accurate            ///< 精确
    };

    explicit QuickSearchTool(QObject* parent = nullptr);
    ~QuickSearchTool() override = default;

    // VisionTool接口实现
    ToolType toolType() const override { return Custom; }
    QString toolName() const override { return tr("快速搜索"); }
    bool process(const Base::ImageData::Ptr& input, ToolResult& output) override;
    QJsonObject serializeParams() const override;
    void deserializeParams(const QJsonObject& json) override;

    // ========== 模板管理 ==========

    /**
     * @brief 设置模板图像
     */
    bool setTemplate(const cv::Mat& templateImg);

    /**
     * @brief 从ROI创建模板
     */
    bool createTemplate(const Base::ImageData::Ptr& image, const QRect& roi);

    /**
     * @brief 加载模板
     */
    bool loadTemplate(const QString& filePath);

    /**
     * @brief 保存模板
     */
    bool saveTemplate(const QString& filePath);

    /**
     * @brief 清除模板
     */
    void clearTemplate();

    /**
     * @brief 是否有模板
     */
    bool hasTemplate() const { return !templateImage_.empty(); }

    /**
     * @brief 获取模板图像
     */
    cv::Mat templateImage() const { return templateImage_; }

    // ========== 参数设置 ==========

    // 匹配方法
    int matchMethod() const { return matchMethod_; }
    void setMatchMethod(int method) { matchMethod_ = method; }

    // 最小分数
    double minScore() const { return minScore_; }
    void setMinScore(double score) { minScore_ = qBound(0.0, score, 1.0); }

    // 最大匹配数
    int maxMatches() const { return maxMatches_; }
    void setMaxMatches(int count) { maxMatches_ = qMax(1, count); }

    // 速度等级
    SpeedLevel speedLevel() const { return speedLevel_; }
    void setSpeedLevel(SpeedLevel level) { speedLevel_ = level; }

    // 多尺度搜索
    bool useMultiScale() const { return useMultiScale_; }
    void setUseMultiScale(bool use) { useMultiScale_ = use; }

    // 金字塔层数
    int pyramidLevels() const { return pyramidLevels_; }
    void setPyramidLevels(int levels) { pyramidLevels_ = qBound(1, levels, 6); }

    // ========== 结果 ==========

    const std::vector<QuickMatch>& matches() const { return matches_; }

private:
    bool processMultiScale(const cv::Mat& image, const cv::Mat& templ);
    bool processSingleScale(const cv::Mat& image, const cv::Mat& templ);
    void findLocalMaxima(const cv::Mat& result, std::vector<QuickMatch>& matches);

private:
    // 模板
    cv::Mat templateImage_;

    // 参数
    int matchMethod_ = cv::TM_CCOEFF_NORMED;
    double minScore_ = 0.7;
    int maxMatches_ = 1;
    SpeedLevel speedLevel_ = Fast;
    bool useMultiScale_ = true;
    int pyramidLevels_ = 3;

    // 结果
    std::vector<QuickMatch> matches_;
};

} // namespace Algorithm
} // namespace VisionForge
