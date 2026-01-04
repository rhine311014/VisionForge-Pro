/**
 * @file PatternSearchTool.h
 * @brief 图案搜索工具 - 基于Halcon形状匹配的高精度模板匹配工具
 *
 * @details
 * 本工具是VisionForge视觉检测系统中的核心搜索工具之一，主要用于在图像中
 * 精确定位预定义的图案模板。它封装了Halcon的形状匹配(Shape-Based Matching)
 * 算法，提供了工业级别的高精度、高鲁棒性的图案搜索能力。
 *
 * 主要功能特点：
 * 1. 支持多种匹配模式：灰度匹配、边缘匹配（形状匹配）、相关匹配
 * 2. 支持角度范围搜索：可在指定角度范围内搜索旋转后的目标
 * 3. 支持缩放范围搜索：可在指定缩放范围内搜索不同大小的目标
 * 4. 支持多级金字塔加速：通过图像金字塔提高搜索速度
 * 5. 支持亚像素精度：提供多种亚像素定位方法
 * 6. 支持多目标检测：可同时检测多个匹配目标
 *
 * 典型应用场景：
 * - PCB元器件定位
 * - 产品外观检测中的标记定位
 * - 机器人抓取定位
 * - 精密对位系统
 *
 * 使用流程：
 * 1. 通过createTemplate()从图像ROI创建模板，或通过loadTemplate()加载模板文件
 * 2. 设置搜索参数（角度范围、缩放范围、最小分数等）
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
#include <QRectF>

#ifdef USE_HALCON
#include <HalconCpp.h>
#endif

namespace VisionForge {
namespace Algorithm {

/**
 * @brief 图案搜索匹配结果结构体
 *
 * 用于存储单个匹配结果的详细信息，包括位置、角度、缩放和分数。
 * 坐标系采用图像坐标系，原点在图像左上角，X轴向右，Y轴向下。
 */
struct PatternMatch {
    double row = 0;         ///< 匹配中心的行坐标（Y坐标），单位：像素
    double col = 0;         ///< 匹配中心的列坐标（X坐标），单位：像素
    double angle = 0;       ///< 匹配目标相对于模板的旋转角度，单位：弧度，正值表示逆时针旋转
    double scale = 1.0;     ///< 匹配目标相对于模板的缩放比例，1.0表示原始大小
    double score = 0;       ///< 匹配分数，范围[0,1]，值越大表示匹配越好
};

/**
 * @class PatternSearchTool
 * @brief 图案搜索工具类
 *
 * 该类封装了基于形状匹配的图案搜索算法，是VisionTool的派生类。
 *
 * 工作原理：
 * 形状匹配算法基于图像边缘或轮廓特征进行匹配，而非简单的像素值比较。
 * 这使得它对光照变化、图像噪声具有很强的鲁棒性。算法首先提取模板的
 * 边缘特征并建立金字塔模型，然后在搜索图像的金字塔各层级上进行从粗
 * 到精的搜索，最终通过亚像素插值获得高精度定位结果。
 *
 * 线程安全性：本类不是线程安全的，如需在多线程环境下使用，
 * 请为每个线程创建独立的实例。
 */
class PatternSearchTool : public VisionTool
{
    Q_OBJECT

public:
    /**
     * @brief 匹配模式枚举
     *
     * 定义了三种不同的模板匹配算法，各有特点和适用场景
     */
    enum MatchMode {
        GrayMatch = 0,      ///< 灰度匹配：基于像素灰度值的直接匹配，速度快但对光照敏感
        EdgeMatch,          ///< 边缘匹配（形状匹配）：基于边缘特征匹配，对光照变化鲁棒，推荐使用
        CorrelationMatch    ///< 相关匹配：基于归一化互相关的匹配，适用于纹理丰富的目标
    };

    /**
     * @brief 亚像素精度模式枚举
     *
     * 亚像素定位可以将定位精度提高到像素级以下，通常可达0.1-0.01像素
     */
    enum SubPixelMode {
        None = 0,               ///< 无亚像素：只提供整数像素精度
        Interpolation,          ///< 插值法：通过周围像素插值获得亚像素位置，速度较快
        LeastSquares,           ///< 最小二乘法：通过最小二乘拟合获得亚像素位置，精度较高
        LeastSquaresHigh        ///< 高精度最小二乘法：更精细的最小二乘拟合，精度最高但速度较慢
    };

    /**
     * @brief 构造函数
     * @param parent 父对象指针，用于Qt对象树管理
     */
    explicit PatternSearchTool(QObject* parent = nullptr);

    /**
     * @brief 析构函数
     *
     * 负责清理模板资源，释放Halcon形状模型内存
     */
    ~PatternSearchTool() override;

    // ==================== VisionTool接口实现 ====================

    /**
     * @brief 获取工具类型
     * @return 返回Custom类型，表示这是一个自定义工具
     */
    ToolType toolType() const override { return PatternSearch; }

    /**
     * @brief 获取工具显示名称
     * @return 返回本地化的工具名称"图案搜索"
     */
    QString toolName() const override { return tr("图案搜索"); }

    /**
     * @brief 执行图案搜索处理
     *
     * 这是工具的核心方法，在输入图像上执行模板匹配搜索。
     *
     * @param input 输入图像数据指针，支持灰度图和彩色图（内部会自动转换为灰度）
     * @param output 输出结果结构，包含匹配数量和详细匹配信息
     * @return true表示搜索执行成功（即使没有找到匹配），false表示执行出错
     *
     * @note 调用此方法前必须先创建或加载模板
     * @see createTemplate(), loadTemplate()
     */
    bool process(const Base::ImageData::Ptr& input, ToolResult& output) override;

    /**
     * @brief 序列化参数到JSON
     *
     * 将当前所有参数设置导出为JSON格式，用于保存配置
     *
     * @return 包含所有参数的JSON对象
     */
    QJsonObject serializeParams() const override;

    /**
     * @brief 从JSON反序列化参数
     *
     * 从JSON对象恢复参数设置，用于加载配置
     *
     * @param json 包含参数的JSON对象
     */
    void deserializeParams(const QJsonObject& json) override;

    // ==================== 模板管理方法 ====================

    /**
     * @brief 从图像ROI区域创建模板
     *
     * 从输入图像的指定ROI区域提取特征并创建形状模型。
     * 创建成功后，模板将保存在内存中，可以用于后续的搜索操作。
     *
     * @param image 源图像数据指针
     * @param roi ROI区域，指定模板在图像中的位置和大小
     * @return true表示创建成功，false表示创建失败（图像为空或ROI无效）
     *
     * @note 建议选择具有明显边缘特征的区域作为模板
     * @note ROI区域应该完全在图像范围内
     */
    bool createTemplate(const Base::ImageData::Ptr& image, const QRectF& roi);

    /**
     * @brief 从文件加载模板
     *
     * 加载之前保存的Halcon形状模型文件(.shm格式)
     *
     * @param filePath 模板文件的完整路径
     * @return true表示加载成功，false表示加载失败（文件不存在或格式错误）
     */
    bool loadTemplate(const QString& filePath);

    /**
     * @brief 保存模板到文件
     *
     * 将当前模板保存为Halcon形状模型文件(.shm格式)
     *
     * @param filePath 保存文件的完整路径
     * @return true表示保存成功，false表示保存失败（无模板或写入失败）
     */
    bool saveTemplate(const QString& filePath);

    /**
     * @brief 清除当前模板
     *
     * 释放模板占用的内存，清除所有匹配结果
     */
    void clearTemplate();

    /**
     * @brief 检查是否有有效模板
     * @return true表示有模板，false表示无模板
     */
    bool hasTemplate() const { return hasTemplate_; }

    // ==================== 参数设置方法 ====================

    // ---------- 匹配模式相关 ----------

    /**
     * @brief 获取当前匹配模式
     * @return 当前设置的匹配模式
     */
    MatchMode matchMode() const { return matchMode_; }

    /**
     * @brief 设置匹配模式
     * @param mode 匹配模式，参见MatchMode枚举
     */
    void setMatchMode(MatchMode mode) { matchMode_ = mode; }

    // ---------- 匹配阈值相关 ----------

    /**
     * @brief 获取最小匹配分数阈值
     * @return 最小分数阈值，范围[0,1]
     */
    double minScore() const { return minScore_; }

    /**
     * @brief 设置最小匹配分数阈值
     *
     * 只有分数大于等于此阈值的匹配结果才会被保留。
     * 较高的阈值可以减少误检，但可能漏检；
     * 较低的阈值可以提高检出率，但可能增加误检。
     *
     * @param score 最小分数阈值，范围[0,1]，推荐值0.5-0.8
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
     * 限制返回的最大匹配结果数量，超过此数量的较低分数结果将被忽略
     *
     * @param count 最大数量，最小值为1
     */
    void setMaxMatches(int count) { maxMatches_ = qMax(1, count); }

    // ---------- 重叠控制相关 ----------

    /**
     * @brief 获取重叠阈值
     * @return 重叠阈值，范围[0,1]
     */
    double overlapThreshold() const { return overlapThreshold_; }

    /**
     * @brief 设置重叠阈值
     *
     * 当两个匹配结果的重叠度超过此阈值时，只保留分数较高的一个。
     * 这用于避免同一目标被多次检测。
     *
     * @param threshold 重叠阈值，范围[0,1]，0表示不允许任何重叠，1表示允许完全重叠
     */
    void setOverlapThreshold(double threshold) { overlapThreshold_ = qBound(0.0, threshold, 1.0); }

    // ---------- 亚像素精度相关 ----------

    /**
     * @brief 获取亚像素精度模式
     * @return 当前亚像素模式
     */
    SubPixelMode subPixelMode() const { return subPixelMode_; }

    /**
     * @brief 设置亚像素精度模式
     * @param mode 亚像素模式，参见SubPixelMode枚举
     */
    void setSubPixelMode(SubPixelMode mode) { subPixelMode_ = mode; }

    // ---------- 金字塔相关 ----------

    /**
     * @brief 获取金字塔层数
     * @return 金字塔层数
     */
    int pyramidLevels() const { return pyramidLevels_; }

    /**
     * @brief 设置金字塔层数
     *
     * 金字塔层数越多，搜索速度越快，但可能降低对小目标的检测能力。
     * 建议根据模板大小和搜索速度要求进行调整。
     *
     * @param levels 金字塔层数，范围[1,10]
     */
    void setPyramidLevels(int levels) { pyramidLevels_ = qBound(1, levels, 10); }

    // ---------- 角度搜索相关 ----------

    /**
     * @brief 检查是否启用角度搜索
     * @return true表示启用，false表示禁用
     */
    bool angleSearchEnabled() const { return angleSearchEnabled_; }

    /**
     * @brief 设置是否启用角度搜索
     *
     * 启用后，将在指定角度范围内搜索旋转后的目标
     *
     * @param enabled true启用，false禁用
     */
    void setAngleSearchEnabled(bool enabled) { angleSearchEnabled_ = enabled; }

    /**
     * @brief 获取角度搜索的最小角度
     * @return 最小角度，单位：度
     */
    double angleMin() const { return angleMin_; }

    /**
     * @brief 获取角度搜索的最大角度
     * @return 最大角度，单位：度
     */
    double angleMax() const { return angleMax_; }

    /**
     * @brief 获取角度搜索步长
     * @return 角度步长，单位：度
     */
    double angleStep() const { return angleStep_; }

    /**
     * @brief 设置角度搜索范围
     *
     * @param min 最小角度，单位：度（负值表示顺时针方向）
     * @param max 最大角度，单位：度（正值表示逆时针方向）
     * @param step 角度步长，单位：度，影响搜索精度和速度
     */
    void setAngleRange(double min, double max, double step);

    // ---------- 缩放搜索相关 ----------

    /**
     * @brief 检查是否启用缩放搜索
     * @return true表示启用，false表示禁用
     */
    bool scaleSearchEnabled() const { return scaleSearchEnabled_; }

    /**
     * @brief 设置是否启用缩放搜索
     *
     * 启用后，将在指定缩放范围内搜索不同大小的目标。
     * 注意：启用缩放搜索会显著增加计算时间。
     *
     * @param enabled true启用，false禁用
     */
    void setScaleSearchEnabled(bool enabled) { scaleSearchEnabled_ = enabled; }

    /**
     * @brief 获取缩放搜索的最小比例
     * @return 最小缩放比例
     */
    double scaleMin() const { return scaleMin_; }

    /**
     * @brief 获取缩放搜索的最大比例
     * @return 最大缩放比例
     */
    double scaleMax() const { return scaleMax_; }

    /**
     * @brief 获取缩放搜索步长
     * @return 缩放步长
     */
    double scaleStep() const { return scaleStep_; }

    /**
     * @brief 设置缩放搜索范围
     *
     * @param min 最小缩放比例（例如0.8表示80%大小）
     * @param max 最大缩放比例（例如1.2表示120%大小）
     * @param step 缩放步长，影响搜索精度和速度
     */
    void setScaleRange(double min, double max, double step);

    // ==================== 搜索结果获取 ====================

    /**
     * @brief 获取所有匹配结果
     *
     * 返回最近一次process()调用产生的所有匹配结果。
     * 结果按匹配分数降序排列（最佳匹配在前）。
     *
     * @return 匹配结果向量的常引用
     */
    const std::vector<PatternMatch>& matches() const { return matches_; }

    /**
     * @brief 获取最佳匹配结果
     *
     * 返回分数最高的匹配结果。如果没有匹配结果，返回默认初始化的PatternMatch。
     *
     * @return 最佳匹配结果
     */
    PatternMatch bestMatch() const;

private:
    /**
     * @brief 使用Halcon执行搜索
     *
     * @param input 输入图像
     * @param output 输出结果
     * @return 是否成功
     */
    bool processHalcon(const Base::ImageData::Ptr& input, ToolResult& output);

    /**
     * @brief 使用OpenCV执行搜索（备用方案）
     *
     * 当Halcon不可用时的备用实现，功能受限
     *
     * @param input 输入图像
     * @param output 输出结果
     * @return 是否成功
     */
    bool processOpenCV(const Base::ImageData::Ptr& input, ToolResult& output);

private:
    // ==================== 搜索参数 ====================
    MatchMode matchMode_ = EdgeMatch;           ///< 匹配模式，默认使用边缘匹配
    double minScore_ = 0.5;                     ///< 最小匹配分数阈值
    int maxMatches_ = 10;                       ///< 最大匹配数量
    double overlapThreshold_ = 0.5;             ///< 重叠阈值，用于非极大值抑制
    SubPixelMode subPixelMode_ = LeastSquares;  ///< 亚像素精度模式
    int pyramidLevels_ = 4;                     ///< 金字塔层数

    // ==================== 角度搜索参数 ====================
    bool angleSearchEnabled_ = true;            ///< 是否启用角度搜索
    double angleMin_ = -30.0;                   ///< 最小搜索角度（度）
    double angleMax_ = 30.0;                    ///< 最大搜索角度（度）
    double angleStep_ = 1.0;                    ///< 角度搜索步长（度）

    // ==================== 缩放搜索参数 ====================
    bool scaleSearchEnabled_ = false;           ///< 是否启用缩放搜索，默认禁用以提高速度
    double scaleMin_ = 0.8;                     ///< 最小缩放比例
    double scaleMax_ = 1.2;                     ///< 最大缩放比例
    double scaleStep_ = 0.05;                   ///< 缩放搜索步长

    // ==================== 运行状态 ====================
    bool hasTemplate_ = false;                  ///< 是否已创建/加载模板
    std::vector<PatternMatch> matches_;         ///< 存储搜索结果

#ifdef USE_HALCON
    HalconCpp::HShapeModel shapeModel_;         ///< Halcon形状模型对象
    HalconCpp::HImage templateImage_;           ///< 模板图像（用于显示和调试）
#endif
};

} // namespace Algorithm
} // namespace VisionForge
