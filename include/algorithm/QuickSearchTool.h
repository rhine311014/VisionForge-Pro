/**
 * @file QuickSearchTool.h
 * @brief 快速搜索工具 - 基于OpenCV模板匹配的轻量级快速搜索工具
 *
 * @details
 * 本工具是VisionForge视觉检测系统中的轻量级模板匹配工具，使用OpenCV的
 * 模板匹配算法实现快速的图像搜索。相比PatternSearchTool（基于Halcon），
 * 本工具更加轻量，不依赖商业库，适合对速度要求高但精度要求相对较低的场景。
 *
 * 主要功能特点：
 * 1. 纯OpenCV实现：无需Halcon等商业库依赖
 * 2. 多种匹配方法：支持平方差、相关性、相关系数等多种匹配算法
 * 3. 金字塔加速：支持多尺度金字塔搜索，显著提高搜索速度
 * 4. 多目标检测：支持在同一图像中检测多个匹配目标
 * 5. 速度优化：提供多种速度等级选择
 *
 * 支持的匹配方法（对应OpenCV TemplateMatchModes）：
 * - TM_SQDIFF: 平方差匹配，值越小匹配越好
 * - TM_SQDIFF_NORMED: 归一化平方差匹配
 * - TM_CCORR: 相关匹配
 * - TM_CCORR_NORMED: 归一化相关匹配
 * - TM_CCOEFF: 相关系数匹配
 * - TM_CCOEFF_NORMED: 归一化相关系数匹配（推荐）
 *
 * 与PatternSearchTool的区别：
 * - 不支持旋转不变性搜索
 * - 不支持缩放不变性搜索
 * - 速度更快，资源占用更少
 * - 适合目标位置和大小基本固定的场景
 *
 * 典型应用场景：
 * - 固定位置的标识检测
 * - 简单的有无检测
 * - 实时视频流中的目标追踪
 * - 对速度要求高的批量检测
 *
 * 使用流程：
 * 1. 通过setTemplate()或createTemplate()设置模板
 * 2. 设置搜索参数（匹配方法、最小分数等）
 * 3. 调用process()执行搜索
 * 4. 通过matches()获取搜索结果
 *
 * @author VisionForge Team
 * @date 2025-12-30
 * @version 1.0
 */

#pragma once

#include "algorithm/VisionTool.h"
#include <vector>
#include <opencv2/opencv.hpp>

namespace VisionForge {
namespace Algorithm {

/**
 * @brief 快速搜索匹配结果结构体
 *
 * 用于存储单个匹配结果的详细信息，包括位置、分数和匹配区域。
 */
struct QuickMatch {
    double x = 0;           ///< 匹配中心X坐标，单位：像素
    double y = 0;           ///< 匹配中心Y坐标，单位：像素
    double score = 0;       ///< 匹配分数，范围[0,1]，值越大表示匹配越好
    cv::Rect rect;          ///< 匹配矩形区域，表示模板在图像中的位置和大小
};

/**
 * @class QuickSearchTool
 * @brief 快速搜索工具类
 *
 * 该类封装了OpenCV模板匹配算法，是VisionTool的派生类。
 * 提供了简单高效的模板搜索功能。
 *
 * 工作原理：
 * 模板匹配通过在待搜索图像上滑动模板窗口，计算每个位置的相似度，
 * 然后找到相似度最高（或最低，取决于匹配方法）的位置作为匹配结果。
 * 金字塔加速通过先在低分辨率图像上快速定位候选区域，然后在原始
 * 分辨率上精确匹配，大大提高了搜索速度。
 *
 * 线程安全性：本类不是线程安全的，如需在多线程环境下使用，
 * 请为每个线程创建独立的实例。
 */
class QuickSearchTool : public VisionTool
{
    Q_OBJECT

public:
    /**
     * @brief 速度等级枚举
     *
     * 定义了不同的搜索速度等级，速度与精度的权衡
     */
    enum SpeedLevel {
        UltraFast = 0,      ///< 极速模式：使用最高层金字塔，速度最快但精度最低
        Fast,               ///< 快速模式：平衡速度和精度
        Normal,             ///< 标准模式：默认设置
        Accurate            ///< 精确模式：使用更多金字塔层级，精度最高但速度较慢
    };

    /**
     * @brief 构造函数
     * @param parent 父对象指针，用于Qt对象树管理
     */
    explicit QuickSearchTool(QObject* parent = nullptr);

    /**
     * @brief 析构函数
     *
     * 使用默认实现
     */
    ~QuickSearchTool() override = default;

    // ==================== VisionTool接口实现 ====================

    /**
     * @brief 获取工具类型
     * @return 返回Custom类型
     */
    ToolType toolType() const override { return QuickSearch; }

    /**
     * @brief 获取工具显示名称
     * @return 返回本地化的工具名称"快速搜索"
     */
    QString toolName() const override { return tr("快速搜索"); }

    /**
     * @brief 执行快速搜索处理
     *
     * 在输入图像上执行模板匹配搜索。
     *
     * @param input 输入图像数据指针，支持灰度图和彩色图
     * @param output 输出结果结构，包含匹配数量和详细匹配信息
     * @return true表示搜索执行成功，false表示执行出错
     *
     * @note 调用此方法前必须先设置模板
     */
    bool process(const Base::ImageData::Ptr& input, ToolResult& output) override;

    /**
     * @brief 序列化参数到JSON
     * @return 包含所有参数的JSON对象
     */
    QJsonObject serializeParams() const override;

    /**
     * @brief 从JSON反序列化参数
     * @param json 包含参数的JSON对象
     */
    void deserializeParams(const QJsonObject& json) override;

    // ==================== 模板管理方法 ====================

    /**
     * @brief 直接设置模板图像
     *
     * 使用cv::Mat对象直接设置模板
     *
     * @param templateImg 模板图像，将被克隆存储
     * @return true表示设置成功，false表示图像为空
     */
    bool setTemplate(const cv::Mat& templateImg);

    /**
     * @brief 从图像ROI创建模板
     *
     * 从输入图像的指定ROI区域提取模板
     *
     * @param image 源图像数据指针
     * @param roi ROI区域，指定模板在图像中的位置和大小
     * @return true表示创建成功，false表示失败（图像为空或ROI无效）
     */
    bool createTemplate(const Base::ImageData::Ptr& image, const QRect& roi);

    /**
     * @brief 从文件加载模板
     *
     * 从图像文件加载模板
     *
     * @param filePath 模板图像文件路径
     * @return true表示加载成功，false表示失败
     */
    bool loadTemplate(const QString& filePath);

    /**
     * @brief 保存模板到文件
     *
     * 将当前模板保存为图像文件
     *
     * @param filePath 保存文件路径
     * @return true表示保存成功，false表示失败
     */
    bool saveTemplate(const QString& filePath);

    /**
     * @brief 清除当前模板
     *
     * 释放模板占用的内存
     */
    void clearTemplate();

    /**
     * @brief 检查是否有有效模板
     * @return true表示有模板，false表示无模板
     */
    bool hasTemplate() const { return !templateImage_.empty(); }

    /**
     * @brief 获取当前模板图像
     * @return 模板图像的副本
     */
    cv::Mat templateImage() const { return templateImage_; }

    // ==================== 参数设置方法 ====================

    // ---------- 匹配方法相关 ----------

    /**
     * @brief 获取当前匹配方法
     * @return OpenCV模板匹配方法（cv::TemplateMatchModes枚举值）
     */
    int matchMethod() const { return matchMethod_; }

    /**
     * @brief 设置匹配方法
     *
     * 设置OpenCV模板匹配使用的算法。推荐使用TM_CCOEFF_NORMED。
     *
     * @param method OpenCV模板匹配方法，参见cv::TemplateMatchModes
     */
    void setMatchMethod(int method) { matchMethod_ = method; }

    // ---------- 分数阈值相关 ----------

    /**
     * @brief 获取最小匹配分数
     * @return 最小分数阈值，范围[0,1]
     */
    double minScore() const { return minScore_; }

    /**
     * @brief 设置最小匹配分数
     *
     * 只有分数大于等于此阈值的匹配结果才会被保留。
     *
     * @param score 最小分数阈值，范围[0,1]
     */
    void setMinScore(double score) { minScore_ = qBound(0.0, score, 1.0); }

    // ---------- 匹配数量相关 ----------

    /**
     * @brief 获取最大匹配数量
     * @return 最大匹配数量
     */
    int maxMatches() const { return maxMatches_; }

    /**
     * @brief 设置最大匹配数量
     *
     * 限制返回的最大匹配结果数量
     *
     * @param count 最大数量，最小值为1
     */
    void setMaxMatches(int count) { maxMatches_ = qMax(1, count); }

    // ---------- 速度相关 ----------

    /**
     * @brief 获取当前速度等级
     * @return 速度等级
     */
    SpeedLevel speedLevel() const { return speedLevel_; }

    /**
     * @brief 设置速度等级
     *
     * 调整搜索的速度/精度平衡
     *
     * @param level 速度等级，参见SpeedLevel枚举
     */
    void setSpeedLevel(SpeedLevel level) { speedLevel_ = level; }

    // ---------- 金字塔相关 ----------

    /**
     * @brief 获取是否使用多尺度搜索
     * @return true表示使用金字塔加速
     */
    bool useMultiScale() const { return useMultiScale_; }

    /**
     * @brief 设置是否使用多尺度搜索
     *
     * 启用金字塔加速可以显著提高搜索速度，
     * 但可能在极端情况下略微降低精度。
     *
     * @param use true启用，false禁用
     */
    void setUseMultiScale(bool use) { useMultiScale_ = use; }

    /**
     * @brief 获取金字塔层数
     * @return 金字塔层数
     */
    int pyramidLevels() const { return pyramidLevels_; }

    /**
     * @brief 设置金字塔层数
     *
     * 金字塔层数越多，搜索速度越快，但精度可能下降。
     * 同时，过多的层数可能导致模板变得太小而无法有效匹配。
     *
     * @param levels 金字塔层数，范围[1,6]
     */
    void setPyramidLevels(int levels) { pyramidLevels_ = qBound(1, levels, 6); }

    // ==================== 结果获取 ====================

    /**
     * @brief 获取所有匹配结果
     *
     * 返回最近一次process()调用产生的所有匹配结果。
     * 结果按匹配分数降序排列。
     *
     * @return 匹配结果向量的常引用
     */
    const std::vector<QuickMatch>& matches() const { return matches_; }

private:
    /**
     * @brief 使用金字塔加速的多尺度搜索
     *
     * 先在低分辨率图像上快速定位，然后在原始分辨率上精确匹配
     *
     * @param image 待搜索的灰度图像
     * @param templ 模板灰度图像
     * @return 是否成功
     */
    bool processMultiScale(const cv::Mat& image, const cv::Mat& templ);

    /**
     * @brief 单尺度直接搜索
     *
     * 在原始分辨率上直接进行模板匹配
     *
     * @param image 待搜索的灰度图像
     * @param templ 模板灰度图像
     * @return 是否成功
     */
    bool processSingleScale(const cv::Mat& image, const cv::Mat& templ);

    /**
     * @brief 在匹配结果图上查找局部极大值
     *
     * 通过非极大值抑制找到多个匹配位置
     *
     * @param result 模板匹配的结果图
     * @param matches 输出的匹配结果向量
     */
    void findLocalMaxima(const cv::Mat& result, std::vector<QuickMatch>& matches);

private:
    // ==================== 模板数据 ====================
    cv::Mat templateImage_;     ///< 存储的模板图像

    // ==================== 搜索参数 ====================
    int matchMethod_ = cv::TM_CCOEFF_NORMED;    ///< 匹配方法，默认归一化相关系数
    double minScore_ = 0.7;                      ///< 最小匹配分数阈值
    int maxMatches_ = 1;                         ///< 最大匹配数量，默认只找1个
    SpeedLevel speedLevel_ = Fast;               ///< 速度等级
    bool useMultiScale_ = true;                  ///< 是否使用金字塔加速
    int pyramidLevels_ = 3;                      ///< 金字塔层数

    // ==================== 搜索结果 ====================
    std::vector<QuickMatch> matches_;           ///< 存储搜索结果
};

} // namespace Algorithm
} // namespace VisionForge
