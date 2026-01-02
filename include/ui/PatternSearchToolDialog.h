/**
 * @file PatternSearchToolDialog.h
 * @brief 图案搜索工具对话框
 * @details 基于Halcon形状匹配的图案搜索工具，继承自SearchToolDialog基类。
 *          提供图案（模板）的创建、加载、保存功能，以及丰富的搜索参数配置。
 *
 * 主要功能：
 * - 支持从ROI创建图案模板
 * - 支持模板的保存和加载（.shm格式）
 * - 支持灰度匹配、边缘匹配、相关匹配三种匹配模式
 * - 支持角度搜索和缩放搜索
 * - 提供金字塔加速和亚像素精度设置
 *
 * @author VisionForge Team
 * @date 2025-12-30
 */

#pragma once

#include "ui/SearchToolDialog.h"
#include <QLineEdit>
#include <QRadioButton>

namespace VisionForge {

namespace Algorithm {
    class PatternSearchTool;  // 前向声明图案搜索工具类
}

namespace UI {

/**
 * @brief 图案搜索工具对话框
 *
 * 提供图案（模板）创建、参数配置和搜索功能。
 * 支持灰度匹配、边缘匹配、相关匹配等多种匹配模式。
 *
 * 使用流程：
 * 1. 加载图像
 * 2. 绘制ROI选择模板区域，或加载已有模板
 * 3. 设置匹配参数（最小分数、角度范围等）
 * 4. 执行搜索预览
 * 5. 确认参数设置
 *
 * 继承关系：PatternSearchToolDialog -> SearchToolDialog -> IToolDialog
 */
class PatternSearchToolDialog : public SearchToolDialog
{
    Q_OBJECT

public:
    /**
     * @brief 匹配模式枚举
     * @details 定义三种不同的模板匹配算法
     */
    enum MatchMode {
        GrayMatch = 0,      ///< 灰度匹配 - 基于灰度值的相关性匹配，对光照敏感
        EdgeMatch,          ///< 边缘匹配 - 基于边缘轮廓的形状匹配，对光照不敏感（推荐）
        CorrelationMatch    ///< 相关匹配 - 归一化相关系数匹配，适合纹理丰富的目标
    };

    /**
     * @brief 构造函数
     * @param tool 关联的图案搜索工具指针
     * @param parent 父窗口指针
     */
    explicit PatternSearchToolDialog(Algorithm::PatternSearchTool* tool, QWidget* parent = nullptr);

    /**
     * @brief 析构函数
     */
    ~PatternSearchToolDialog() override;

protected:
    // ==================== SearchToolDialog接口实现 ====================

    /**
     * @brief 创建参数面板
     * @details 创建图案搜索特有的参数设置控件，包括匹配模式、模板管理、搜索参数等
     * @param parent 父控件
     * @return 参数面板控件指针
     */
    QWidget* createParameterPanel(QWidget* parent) override;

    /**
     * @brief 获取工具名称
     * @return 返回"图案搜索"
     */
    QString toolName() const override { return tr("图案搜索"); }

    /**
     * @brief 执行图案搜索
     * @details 调用Halcon形状匹配算法执行搜索
     * @param results 输出搜索结果
     * @return 搜索是否成功
     */
    bool executeSearch(std::vector<SearchResult>& results) override;

    /**
     * @brief 应用参数到工具对象
     * @details 将UI控件的值同步到PatternSearchTool对象
     */
    void applySpecificParameters() override;

    /**
     * @brief 从工具对象更新UI
     * @details 从PatternSearchTool对象读取参数更新UI控件
     */
    void updateSpecificUI() override;

private:
    // ==================== UI创建方法 ====================

    /**
     * @brief 创建匹配模式选择组
     * @param layout 父布局
     */
    void createMatchModeGroup(QVBoxLayout* layout);

    /**
     * @brief 创建图案模板管理组
     * @param layout 父布局
     */
    void createPatternGroup(QVBoxLayout* layout);

    /**
     * @brief 创建搜索参数设置组
     * @param layout 父布局
     */
    void createSearchParamGroup(QVBoxLayout* layout);

    /**
     * @brief 创建角度和缩放参数组
     * @param layout 父布局
     */
    void createAngleScaleGroup(QVBoxLayout* layout);

    /**
     * @brief 连接特定信号槽
     */
    void connectSpecificSignals();

private slots:
    // ==================== 槽函数 ====================

    /**
     * @brief 从ROI创建模板按钮点击槽
     */
    void onCreatePatternClicked();

    /**
     * @brief 加载模板按钮点击槽
     */
    void onLoadPatternClicked();

    /**
     * @brief 保存模板按钮点击槽
     */
    void onSavePatternClicked();

    /**
     * @brief 清除模板按钮点击槽
     */
    void onClearPatternClicked();

    /**
     * @brief 匹配模式切换槽
     * @param mode 新选择的匹配模式
     */
    void onMatchModeChanged(int mode);

private:
    // ==================== 成员变量 ====================

    Algorithm::PatternSearchTool* patternTool_ = nullptr;  ///< 图案搜索工具指针

    // ==================== UI组件 - 匹配模式选择 ====================

    QGroupBox* matchModeGroup_ = nullptr;          ///< 匹配模式分组框
    QRadioButton* radioGrayMatch_ = nullptr;       ///< 灰度匹配单选按钮
    QRadioButton* radioEdgeMatch_ = nullptr;       ///< 边缘匹配单选按钮
    QRadioButton* radioCorrelationMatch_ = nullptr; ///< 相关匹配单选按钮

    // ==================== UI组件 - 图案模板管理 ====================

    QGroupBox* patternGroup_ = nullptr;            ///< 图案模板分组框
    QPushButton* createPatternBtn_ = nullptr;      ///< 从ROI创建模板按钮
    QPushButton* loadPatternBtn_ = nullptr;        ///< 加载模板按钮
    QPushButton* savePatternBtn_ = nullptr;        ///< 保存模板按钮
    QPushButton* clearPatternBtn_ = nullptr;       ///< 清除模板按钮
    QLabel* patternStatusLabel_ = nullptr;         ///< 模板状态标签（显示是否已创建）
    QLabel* patternPreviewLabel_ = nullptr;        ///< 模板预览图像标签

    // ==================== UI组件 - 搜索参数 ====================

    QGroupBox* searchParamGroup_ = nullptr;        ///< 搜索参数分组框
    QDoubleSpinBox* minScoreSpin_ = nullptr;       ///< 最小匹配分数输入框（0.0-1.0）
    QSlider* minScoreSlider_ = nullptr;            ///< 最小匹配分数滑块
    QSpinBox* maxMatchesSpin_ = nullptr;           ///< 最大匹配数量输入框
    QDoubleSpinBox* overlapSpin_ = nullptr;        ///< 重叠阈值输入框（用于NMS）
    QComboBox* subPixelCombo_ = nullptr;           ///< 亚像素精度选择下拉框
    QSpinBox* pyramidLevelsSpin_ = nullptr;        ///< 金字塔层数输入框

    // ==================== UI组件 - 角度缩放参数 ====================

    QGroupBox* angleScaleGroup_ = nullptr;         ///< 角度缩放参数分组框
    QCheckBox* enableAngleCheck_ = nullptr;        ///< 启用角度搜索复选框
    QDoubleSpinBox* angleMinSpin_ = nullptr;       ///< 最小搜索角度输入框
    QDoubleSpinBox* angleMaxSpin_ = nullptr;       ///< 最大搜索角度输入框
    QDoubleSpinBox* angleStepSpin_ = nullptr;      ///< 角度搜索步长输入框
    QCheckBox* enableScaleCheck_ = nullptr;        ///< 启用缩放搜索复选框
    QDoubleSpinBox* scaleMinSpin_ = nullptr;       ///< 最小缩放比例输入框
    QDoubleSpinBox* scaleMaxSpin_ = nullptr;       ///< 最大缩放比例输入框
    QDoubleSpinBox* scaleStepSpin_ = nullptr;      ///< 缩放搜索步长输入框
    QWidget* angleWidget_ = nullptr;               ///< 角度参数容器（用于启用/禁用控制）
    QWidget* scaleWidget_ = nullptr;               ///< 缩放参数容器（用于启用/禁用控制）
};

} // namespace UI
} // namespace VisionForge
