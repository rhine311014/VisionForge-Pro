/**
 * @file TemplateMatchTool.h
 * @brief OpenCV模板匹配工具
 */

#pragma once

#include "algorithm/VisionTool.h"
#include <opencv2/core.hpp>
#include <vector>

namespace VisionForge {
namespace Algorithm {

/**
 * @brief 匹配方法
 */
enum class MatchMethod {
    SQDIFF,         // 平方差匹配 (值越小越好)
    SQDIFF_NORMED,  // 归一化平方差匹配
    CCORR,          // 相关匹配 (值越大越好)
    CCORR_NORMED,   // 归一化相关匹配
    CCOEFF,         // 相关系数匹配 (值越大越好)
    CCOEFF_NORMED   // 归一化相关系数匹配
};

/**
 * @brief 匹配结果
 */
struct MatchResult {
    QPointF position;       // 匹配位置 (左上角)
    QPointF center;         // 匹配中心点
    double score = 0.0;     // 匹配分数
    double angle = 0.0;     // 旋转角度 (如果启用)
    double scale = 1.0;     // 缩放比例 (如果启用)
    QRectF boundingRect;    // 匹配区域
    bool valid = false;     // 是否有效
};

/**
 * @brief OpenCV模板匹配工具
 *
 * 使用OpenCV的模板匹配算法在图像中查找目标:
 * - 支持多种匹配方法
 * - 支持多目标检测
 * - 支持金字塔加速
 * - 支持角度搜索 (可选)
 */
class TemplateMatchTool : public VisionTool
{
    Q_OBJECT

public:
    explicit TemplateMatchTool(QObject* parent = nullptr);
    virtual ~TemplateMatchTool() = default;

    // ========== VisionTool接口 ==========
    ToolType toolType() const override { return ToolType::TemplateMatch; }
    QString toolName() const override { return "模板匹配"; }
    bool process(const Base::ImageData::Ptr& input, ToolResult& output) override;

    // ========== 模板管理 ==========

    /**
     * @brief 设置模板图像
     */
    void setTemplate(const cv::Mat& templateImage);

    /**
     * @brief 从文件加载模板
     */
    bool loadTemplate(const QString& filePath);

    /**
     * @brief 保存模板到文件
     */
    bool saveTemplate(const QString& filePath) const;

    /**
     * @brief 从ROI创建模板
     */
    void createTemplateFromROI(const cv::Mat& image, const QRectF& roi);

    /**
     * @brief 获取模板图像
     */
    cv::Mat templateImage() const { return template_; }

    /**
     * @brief 模板是否有效
     */
    bool hasTemplate() const { return !template_.empty(); }

    // ========== 参数设置 ==========

    /**
     * @brief 设置匹配方法
     */
    void setMatchMethod(MatchMethod method);
    MatchMethod matchMethod() const { return method_; }

    /**
     * @brief 设置匹配阈值
     */
    void setThreshold(double threshold);
    double threshold() const { return threshold_; }

    /**
     * @brief 设置最大匹配数量
     */
    void setMaxMatches(int count);
    int maxMatches() const { return maxMatches_; }

    /**
     * @brief 设置是否使用金字塔加速
     */
    void setUsePyramid(bool use);
    bool usePyramid() const { return usePyramid_; }

    /**
     * @brief 设置金字塔层数
     */
    void setPyramidLevels(int levels);
    int pyramidLevels() const { return pyramidLevels_; }

    /**
     * @brief 设置是否启用角度搜索
     */
    void setEnableAngleSearch(bool enable);
    bool enableAngleSearch() const { return enableAngleSearch_; }

    /**
     * @brief 设置角度搜索范围
     */
    void setAngleRange(double minAngle, double maxAngle, double step);
    double angleMin() const { return angleMin_; }
    double angleMax() const { return angleMax_; }
    double angleStep() const { return angleStep_; }

    /**
     * @brief 设置非极大值抑制距离
     */
    void setNmsDistance(double distance);
    double nmsDistance() const { return nmsDistance_; }

    // ========== 结果获取 ==========

    /**
     * @brief 获取最佳匹配结果
     */
    MatchResult bestMatch() const;

    /**
     * @brief 获取所有匹配结果
     */
    const std::vector<MatchResult>& allMatches() const { return results_; }

    // ========== 序列化 ==========
    QJsonObject serializeParams() const override;
    void deserializeParams(const QJsonObject& json) override;

signals:
    /**
     * @brief 匹配完成信号
     */
    void matchCompleted(int matchCount);

    /**
     * @brief 模板更新信号
     */
    void templateChanged();

private:
    /**
     * @brief 执行单次匹配
     */
    void performMatch(const cv::Mat& image, const cv::Mat& templ,
                     double angle = 0.0);

    /**
     * @brief 使用金字塔加速匹配
     */
    void pyramidMatch(const cv::Mat& image, const cv::Mat& templ);

    /**
     * @brief 非极大值抑制
     */
    void nonMaximumSuppression();

    /**
     * @brief 旋转模板
     */
    cv::Mat rotateTemplate(const cv::Mat& templ, double angle);

    /**
     * @brief 绘制匹配结果
     */
    cv::Mat drawResults(const cv::Mat& input) const;

    /**
     * @brief 将OpenCV匹配方法转换为整数
     */
    int getOpenCVMatchMethod() const;

private:
    cv::Mat template_;              // 模板图像
    QString templatePath_;          // 模板文件路径
    MatchMethod method_;            // 匹配方法
    double threshold_;              // 匹配阈值
    int maxMatches_;                // 最大匹配数量
    bool usePyramid_;               // 使用金字塔加速
    int pyramidLevels_;             // 金字塔层数
    bool enableAngleSearch_;        // 启用角度搜索
    double angleMin_;               // 最小角度
    double angleMax_;               // 最大角度
    double angleStep_;              // 角度步长
    double nmsDistance_;            // NMS距离
    std::vector<MatchResult> results_;  // 匹配结果
};

} // namespace Algorithm
} // namespace VisionForge
