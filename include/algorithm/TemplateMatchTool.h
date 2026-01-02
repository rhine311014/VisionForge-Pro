/**
 * @file TemplateMatchTool.h
 * @brief OpenCV模板匹配工具头文件
 * @details 实现基于OpenCV的模板匹配算法，支持多种匹配方法、多目标检测、
 *          金字塔加速和可选的角度搜索功能。模板匹配是机器视觉中最基础
 *          且广泛使用的目标定位算法之一。
 *
 * @author VisionForge Team
 * @version 1.0.0
 * @date 2025-12-18
 * @copyright Copyright (c) 2025 VisionForge. All rights reserved.
 *
 * @par 功能特性:
 * - 支持6种OpenCV标准匹配方法
 * - 支持多目标同时检测
 * - 金字塔多尺度加速搜索
 * - 可配置的角度搜索范围
 * - 非极大值抑制避免重复检测
 *
 * @par 使用示例:
 * @code
 * TemplateMatchTool tool;
 * tool.setTemplate(templateImage);
 * tool.setThreshold(0.8);
 * tool.process(inputImage, result);
 * @endcode
 */

#pragma once

#include "algorithm/VisionTool.h"
#include <opencv2/core.hpp>
#include <vector>

namespace VisionForge {
namespace Algorithm {

/**
 * @brief 模板匹配方法枚举
 * @details 定义OpenCV支持的6种模板匹配算法，不同方法有不同的计算特性：
 *          - 平方差方法：值越小表示匹配度越高
 *          - 相关性方法：值越大表示匹配度越高
 *          - 归一化方法：结果在[0,1]范围内，更易于设定阈值
 */
enum class MatchMethod {
    SQDIFF,         ///< 平方差匹配 - 计算模板与图像块的平方差之和，值越小越好
    SQDIFF_NORMED,  ///< 归一化平方差匹配 - 平方差除以归一化因子，结果在[0,1]范围
    CCORR,          ///< 相关匹配 - 计算模板与图像块的相关值，值越大越好
    CCORR_NORMED,   ///< 归一化相关匹配 - 相关值归一化，结果在[0,1]范围
    CCOEFF,         ///< 相关系数匹配 - 考虑均值偏移的相关计算，值越大越好
    CCOEFF_NORMED   ///< 归一化相关系数匹配 - 最常用方法，对亮度变化有一定鲁棒性
};

/**
 * @brief 模板匹配结果结构体
 * @details 存储单个匹配结果的完整信息，包括位置、分数、角度等
 *
 * @note 当启用角度搜索时，angle字段会包含匹配的旋转角度；
 *       当不启用时，angle始终为0
 */
struct MatchResult {
    QPointF position;       ///< 匹配位置 - 匹配区域左上角坐标(像素)
    QPointF center;         ///< 匹配中心点 - 匹配区域中心坐标(像素)
    double score = 0.0;     ///< 匹配分数 - 归一化后的匹配相似度[0,1]
    double angle = 0.0;     ///< 旋转角度 - 匹配时模板的旋转角度(度)
    double scale = 1.0;     ///< 缩放比例 - 保留字段，用于未来扩展多尺度匹配
    QRectF boundingRect;    ///< 匹配区域 - 匹配目标的外接矩形
    bool valid = false;     ///< 有效标志 - 指示该结果是否有效
};

/**
 * @brief OpenCV模板匹配工具类
 * @details 实现基于OpenCV的模板匹配功能，是机器视觉中常用的目标定位工具。
 *          该工具通过在搜索图像中滑动模板窗口，计算模板与图像块的相似度，
 *          从而找到与模板最匹配的位置。
 *
 * @par 算法原理:
 * 1. 将模板在搜索图像上逐像素滑动
 * 2. 在每个位置计算模板与对应图像块的相似度
 * 3. 生成相似度响应图
 * 4. 从响应图中提取高于阈值的匹配位置
 *
 * @par 性能优化:
 * - 金字塔加速：先在低分辨率图像上粗搜索，再在原图精确定位
 * - 多目标检测：一次处理可找到多个匹配目标
 * - NMS抑制：避免同一目标被重复检测
 *
 * @par 应用场景:
 * - 产品外观检测
 * - 芯片/元器件定位
 * - 包装标签识别
 * - 印刷品缺陷检测
 *
 * @note 模板匹配对旋转和缩放敏感，如需旋转不变性请启用角度搜索
 *
 * @see VisionTool, MatchMethod, MatchResult
 */
class TemplateMatchTool : public VisionTool
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param parent 父对象指针，用于Qt对象树管理
     */
    explicit TemplateMatchTool(QObject* parent = nullptr);

    /**
     * @brief 析构函数
     * @details 使用默认析构，模板图像等资源自动释放
     */
    virtual ~TemplateMatchTool() = default;

    // ========== VisionTool接口实现 ==========

    /**
     * @brief 获取工具类型
     * @return ToolType::TemplateMatch 模板匹配类型
     */
    ToolType toolType() const override { return ToolType::TemplateMatch; }

    /**
     * @brief 获取工具名称
     * @return 返回中文名称"模板匹配"
     */
    QString toolName() const override { return "模板匹配"; }

    /**
     * @brief 执行模板匹配处理
     * @param input 输入图像数据指针
     * @param output 输出结果结构体
     * @return 成功返回true，失败返回false
     *
     * @par 处理流程:
     * -# 验证输入图像和模板有效性
     * -# 转换为灰度图像
     * -# 执行模板匹配（支持金字塔加速和角度搜索）
     * -# 应用非极大值抑制
     * -# 生成结果图像和数据
     */
    bool process(const Base::ImageData::Ptr& input, ToolResult& output) override;

    // ========== 模板管理接口 ==========

    /**
     * @brief 设置模板图像
     * @param templateImage 模板图像(cv::Mat格式)
     * @details 直接从cv::Mat设置模板，会克隆输入图像
     * @note 设置后会触发 templateChanged() 和 paramChanged() 信号
     */
    void setTemplate(const cv::Mat& templateImage);

    /**
     * @brief 从文件加载模板图像
     * @param filePath 模板图像文件路径(支持常见图像格式)
     * @return 加载成功返回true，失败返回false
     * @details 使用OpenCV imread加载图像，支持png/jpg/bmp等格式
     */
    bool loadTemplate(const QString& filePath);

    /**
     * @brief 保存模板到文件
     * @param filePath 保存路径
     * @return 保存成功返回true，失败返回false
     * @details 使用OpenCV imwrite保存模板图像
     */
    bool saveTemplate(const QString& filePath) const;

    /**
     * @brief 从图像ROI区域创建模板
     * @param image 源图像
     * @param roi 感兴趣区域(矩形)
     * @details 从指定图像中截取ROI区域作为新模板，
     *          会自动进行边界检查和裁剪
     */
    void createTemplateFromROI(const cv::Mat& image, const QRectF& roi);

    /**
     * @brief 获取当前模板图像
     * @return 模板图像的cv::Mat对象
     */
    cv::Mat templateImage() const { return template_; }

    /**
     * @brief 检查是否已设置有效模板
     * @return 有模板返回true，无模板返回false
     */
    bool hasTemplate() const { return !template_.empty(); }

    // ========== 匹配参数设置 ==========

    /**
     * @brief 设置匹配方法
     * @param method 匹配方法枚举值
     * @see MatchMethod
     */
    void setMatchMethod(MatchMethod method);

    /**
     * @brief 获取当前匹配方法
     * @return 当前使用的匹配方法
     */
    MatchMethod matchMethod() const { return method_; }

    /**
     * @brief 设置匹配阈值
     * @param threshold 阈值，范围[0.0, 1.0]
     * @details 只有匹配分数高于此阈值的结果才会被保留
     *          推荐值：0.7-0.9，根据实际应用调整
     */
    void setThreshold(double threshold);

    /**
     * @brief 获取匹配阈值
     * @return 当前阈值
     */
    double threshold() const { return threshold_; }

    /**
     * @brief 设置最大匹配数量
     * @param count 最大返回的匹配结果数量
     * @details 限制返回的匹配数量，按分数从高到低排序后截取
     */
    void setMaxMatches(int count);

    /**
     * @brief 获取最大匹配数量
     * @return 最大匹配数
     */
    int maxMatches() const { return maxMatches_; }

    /**
     * @brief 设置是否使用金字塔加速
     * @param use true启用金字塔加速，false禁用
     * @details 金字塔加速通过多尺度搜索显著提升大图像上的匹配速度
     */
    void setUsePyramid(bool use);

    /**
     * @brief 获取金字塔加速状态
     * @return 是否启用金字塔加速
     */
    bool usePyramid() const { return usePyramid_; }

    /**
     * @brief 设置金字塔层数
     * @param levels 金字塔层数，范围[1, 6]
     * @details 层数越多搜索越快，但可能降低精度
     *          推荐值：2-4层
     */
    void setPyramidLevels(int levels);

    /**
     * @brief 获取金字塔层数
     * @return 当前金字塔层数
     */
    int pyramidLevels() const { return pyramidLevels_; }

    /**
     * @brief 设置是否启用角度搜索
     * @param enable true启用角度搜索，false禁用
     * @details 启用后会在指定角度范围内旋转模板进行匹配，
     *          可以检测有旋转的目标，但会增加计算时间
     */
    void setEnableAngleSearch(bool enable);

    /**
     * @brief 获取角度搜索状态
     * @return 是否启用角度搜索
     */
    bool enableAngleSearch() const { return enableAngleSearch_; }

    /**
     * @brief 设置角度搜索范围
     * @param minAngle 最小角度(度)
     * @param maxAngle 最大角度(度)
     * @param step 角度步长(度)，最小0.5度
     * @details 在[minAngle, maxAngle]范围内以step步长搜索
     */
    void setAngleRange(double minAngle, double maxAngle, double step);

    /**
     * @brief 获取最小搜索角度
     * @return 最小角度(度)
     */
    double angleMin() const { return angleMin_; }

    /**
     * @brief 获取最大搜索角度
     * @return 最大角度(度)
     */
    double angleMax() const { return angleMax_; }

    /**
     * @brief 获取角度步长
     * @return 角度步长(度)
     */
    double angleStep() const { return angleStep_; }

    /**
     * @brief 设置非极大值抑制距离
     * @param distance NMS距离阈值(像素)
     * @details 距离小于此值的匹配结果会被抑制，只保留分数最高的
     *          用于避免同一目标被重复检测
     */
    void setNmsDistance(double distance);

    /**
     * @brief 获取NMS距离
     * @return NMS距离阈值
     */
    double nmsDistance() const { return nmsDistance_; }

    // ========== 结果获取接口 ==========

    /**
     * @brief 获取最佳匹配结果
     * @return 分数最高的匹配结果，如无结果则返回空MatchResult
     */
    MatchResult bestMatch() const;

    /**
     * @brief 获取所有匹配结果
     * @return 匹配结果向量的常量引用，按分数降序排列
     */
    const std::vector<MatchResult>& allMatches() const { return results_; }

    // ========== 序列化接口 ==========

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

signals:
    /**
     * @brief 匹配完成信号
     * @param matchCount 找到的匹配数量
     * @details 每次process()完成后发出此信号
     */
    void matchCompleted(int matchCount);

    /**
     * @brief 模板更新信号
     * @details 当模板被设置或修改时发出此信号
     */
    void templateChanged();

private:
    /**
     * @brief 执行单次模板匹配
     * @param image 搜索图像(灰度)
     * @param templ 模板图像(灰度)
     * @param angle 模板旋转角度(度)，默认0
     * @details 在指定图像上执行一次完整的模板匹配，
     *          将高于阈值的结果添加到results_向量
     */
    void performMatch(const cv::Mat& image, const cv::Mat& templ,
                     double angle = 0.0);

    /**
     * @brief 金字塔加速匹配
     * @param image 搜索图像(灰度)
     * @param templ 模板图像(灰度)
     * @details 算法步骤：
     *          1. 构建图像金字塔和模板金字塔
     *          2. 在最小尺度层进行全局搜索找到候选点
     *          3. 在原图上对候选区域进行精确匹配
     */
    void pyramidMatch(const cv::Mat& image, const cv::Mat& templ);

    /**
     * @brief 非极大值抑制
     * @details 对results_中的结果进行NMS处理：
     *          1. 按分数降序排序
     *          2. 保留最高分结果
     *          3. 抑制距离过近的其他结果
     */
    void nonMaximumSuppression();

    /**
     * @brief 旋转模板图像
     * @param templ 原始模板
     * @param angle 旋转角度(度)
     * @return 旋转后的模板图像
     * @details 使用仿射变换旋转模板，自动调整输出尺寸
     *          以包含完整的旋转后图像
     */
    cv::Mat rotateTemplate(const cv::Mat& templ, double angle);

    /**
     * @brief 绘制匹配结果到图像
     * @param input 输入图像
     * @return 带有匹配结果标注的图像
     * @details 绘制内容：匹配矩形、中心点、序号和分数
     */
    cv::Mat drawResults(const cv::Mat& input) const;

    /**
     * @brief 获取OpenCV匹配方法常量
     * @return OpenCV的cv::TemplateMatchModes枚举值
     */
    int getOpenCVMatchMethod() const;

private:
    // ========== 成员变量 ==========
    cv::Mat template_;              ///< 模板图像
    QString templatePath_;          ///< 模板文件路径(用于序列化)
    MatchMethod method_;            ///< 匹配方法
    double threshold_;              ///< 匹配阈值 [0,1]
    int maxMatches_;                ///< 最大匹配数量
    bool usePyramid_;               ///< 是否使用金字塔加速
    int pyramidLevels_;             ///< 金字塔层数 [1,6]
    bool enableAngleSearch_;        ///< 是否启用角度搜索
    double angleMin_;               ///< 最小搜索角度(度)
    double angleMax_;               ///< 最大搜索角度(度)
    double angleStep_;              ///< 角度搜索步长(度)
    double nmsDistance_;            ///< NMS抑制距离(像素)
    std::vector<MatchResult> results_;  ///< 匹配结果列表
};

} // namespace Algorithm
} // namespace VisionForge
