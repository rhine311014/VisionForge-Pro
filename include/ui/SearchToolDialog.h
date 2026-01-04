/**
 * @file SearchToolDialog.h
 * @brief 搜索工具对话框基类
 * @details 为图案搜索、角点搜索、快速搜索等提供统一的基类框架。
 *          该类采用模板方法设计模式，定义了搜索工具对话框的通用结构和行为，
 *          子类只需实现特定的参数面板创建和搜索逻辑即可。
 *
 * 主要功能：
 * - 提供统一的UI布局框架（左侧图像显示，右侧参数设置）
 * - 提供ROI（感兴趣区域）绘制和管理功能
 * - 提供搜索结果的表格显示和可视化
 * - 支持实时预览功能
 *
 * @author VisionForge Team
 * @date 2025-12-30
 */

#pragma once

#include "ui/IToolDialog.h"
#include "ui/PreviewHelper.h"
#include "base/ImageData.h"

#include <QDialog>
#include <QSplitter>
#include <QTabWidget>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QSlider>
#include <QTableWidget>

// 前向声明
class QVBoxLayout;
class QHBoxLayout;

namespace VisionForge {

namespace Algorithm {
    class VisionTool;  // 前向声明视觉工具类
}

namespace UI {

class HalconImageViewer;  // 前向声明Halcon图像查看器
class ROIShape;           // 前向声明ROI形状类
using ROIShapePtr = std::shared_ptr<ROIShape>;  // ROI形状智能指针类型定义

/**
 * @brief 搜索工具对话框基类
 *
 * 提供搜索类工具（图案搜索、角点搜索、快速搜索等）的通用UI框架。
 * 采用模板方法模式，子类只需实现以下纯虚函数：
 * - createParameterPanel(): 创建特定的参数设置面板
 * - toolName(): 返回工具名称
 * - executeSearch(): 执行实际的搜索算法
 *
 * 继承关系：SearchToolDialog -> IToolDialog -> QDialog
 */
class SearchToolDialog : public IToolDialog
{
    Q_OBJECT

public:
    /**
     * @brief 搜索结果结构体
     * @details 用于存储单个搜索匹配结果的所有信息
     */
    struct SearchResult {
        double x = 0;           ///< 匹配位置的X坐标（像素）
        double y = 0;           ///< 匹配位置的Y坐标（像素）
        double angle = 0;       ///< 匹配目标的旋转角度（度，相对于模板）
        double scale = 1.0;     ///< 匹配目标的缩放比例（相对于模板）
        double score = 0;       ///< 匹配分数/置信度（0.0-1.0）
        int index = 0;          ///< 结果索引号（从1开始）
    };

    /**
     * @brief 构造函数
     * @param tool 关联的视觉工具指针，用于参数同步
     * @param parent 父窗口指针
     */
    explicit SearchToolDialog(Algorithm::VisionTool* tool, QWidget* parent = nullptr);

    /**
     * @brief 析构函数
     */
    ~SearchToolDialog() override;

    // ==================== IToolDialog接口实现 ====================

    /**
     * @brief 设置待搜索的图像
     * @param image 图像数据智能指针
     */
    void setImage(Base::ImageData::Ptr image) override;

    /**
     * @brief 获取关联的视觉工具
     * @return 视觉工具指针
     */
    Algorithm::VisionTool* tool() const override { return tool_; }

protected:
    // ==================== 子类必须实现的纯虚函数 ====================

    /**
     * @brief 创建参数面板 - 子类必须实现
     * @details 子类在此方法中创建特定的参数设置控件
     * @param parent 父控件指针
     * @return 创建的参数面板控件指针
     */
    virtual QWidget* createParameterPanel(QWidget* parent) = 0;

    /**
     * @brief 获取搜索工具名称 - 子类必须实现
     * @details 返回的名称将显示在对话框标题栏
     * @return 工具名称字符串
     */
    virtual QString toolName() const = 0;

    /**
     * @brief 执行搜索 - 子类必须实现
     * @details 子类在此方法中实现具体的搜索算法
     * @param results 输出参数，用于存储搜索结果
     * @return 搜索是否成功
     */
    virtual bool executeSearch(std::vector<SearchResult>& results) = 0;

    // ==================== 子类可选实现的虚函数 ====================

    /**
     * @brief 应用子类特有的参数
     * @details 将UI控件的值应用到工具对象，子类可覆盖此方法
     */
    virtual void applySpecificParameters() {}

    /**
     * @brief 更新子类特有的UI
     * @details 从工具对象读取参数更新UI控件，子类可覆盖此方法
     */
    virtual void updateSpecificUI() {}

    // ==================== 通用UI创建方法 ====================

    /**
     * @brief 初始化整体UI布局
     */
    void setupUI();

    /**
     * @brief 创建左侧面板（图像显示区域）
     * @param parent 父控件
     */
    void createLeftPanel(QWidget* parent);

    /**
     * @brief 创建右侧面板（参数设置区域）
     * @param parent 父控件
     */
    void createRightPanel(QWidget* parent);

    /**
     * @brief 创建ROI设置组
     * @param layout 父布局
     */
    void createROIGroup(QVBoxLayout* layout);

    /**
     * @brief 创建搜索结果显示组
     * @param layout 父布局
     */
    void createResultGroup(QVBoxLayout* layout);

    /**
     * @brief 创建底部按钮面板
     * @param layout 父布局
     */
    void createButtonPanel(QVBoxLayout* layout);

    /**
     * @brief 连接所有信号槽
     */
    void connectSignals();

    // ==================== 通用功能方法 ====================

    /**
     * @brief 更新UI显示
     */
    void updateUI();

    /**
     * @brief 更新搜索结果表格
     * @param results 搜索结果列表
     */
    void updateResultTable(const std::vector<SearchResult>& results);

    /**
     * @brief 在图像上绘制搜索结果
     * @param results 搜索结果列表
     */
    void drawSearchResults(const std::vector<SearchResult>& results);

    /**
     * @brief 获取搜索ROI区域
     * @return ROI矩形区域，如果未设置则返回空矩形
     */
    QRect getSearchROI() const;

    /**
     * @brief 检查是否已设置搜索ROI
     * @return 是否已设置ROI
     */
    bool hasSearchROI() const;

protected slots:
    // ==================== 槽函数 ====================

    /**
     * @brief 绘制ROI按钮点击槽
     */
    void onDrawROIClicked();

    /**
     * @brief 清除ROI按钮点击槽
     */
    void onClearROIClicked();

    /**
     * @brief 预览按钮点击槽
     */
    void onPreviewClicked();

    /**
     * @brief 自动预览触发槽（参数变化时自动调用）
     */
    void onAutoPreview();

    /**
     * @brief 确定按钮点击槽
     */
    void onOkClicked();

    /**
     * @brief 取消按钮点击槽
     */
    void onCancelClicked();

    /**
     * @brief 应用按钮点击槽
     */
    void onApplyClicked();

    /**
     * @brief 加载图片按钮点击槽
     */
    void onLoadImageClicked();

    /**
     * @brief 采集图像按钮点击槽
     */
    void onCaptureImageClicked();

    /**
     * @brief 结果表格行选中槽
     * @param row 选中的行号
     */
    void onResultTableRowSelected(int row);

signals:
    // ==================== 信号 ====================

    /**
     * @brief 参数变化信号
     * @details 当用户修改参数时发出
     */
    void parameterChanged();

    /**
     * @brief 搜索完成信号
     * @param results 搜索结果列表
     */
    void searchCompleted(const std::vector<SearchResult>& results);

    /**
     * @brief 请求采集图像信号
     * @details 当用户点击采集按钮时发出，由外部相机控制器响应
     */
    void captureImageRequested();

protected:
    // ==================== 核心成员变量 ====================

    Algorithm::VisionTool* tool_ = nullptr;          ///< 关联的视觉工具指针
    Base::ImageData::Ptr currentImage_;              ///< 当前显示的图像
    PreviewHelper* previewHelper_ = nullptr;         ///< 预览辅助器（用于防抖动）

    // ==================== UI组件 - 主布局 ====================

    QSplitter* mainSplitter_ = nullptr;              ///< 主分割器（左右分割）
    QWidget* leftPanel_ = nullptr;                   ///< 左侧面板容器
    QWidget* rightPanel_ = nullptr;                  ///< 右侧面板容器

    // ==================== UI组件 - 图像显示 ====================

    HalconImageViewer* imageViewer_ = nullptr;       ///< Halcon图像查看器
    QLabel* imageInfoLabel_ = nullptr;               ///< 图像信息标签（显示尺寸等）
    QPushButton* loadImageBtn_ = nullptr;            ///< 加载图片按钮
    QPushButton* captureImageBtn_ = nullptr;         ///< 采集图像按钮

    // ==================== UI组件 - ROI控制 ====================

    QGroupBox* roiGroup_ = nullptr;                  ///< ROI设置分组框
    QPushButton* drawROIBtn_ = nullptr;              ///< 绘制ROI按钮
    QPushButton* clearROIBtn_ = nullptr;             ///< 清除ROI按钮
    QComboBox* roiTypeCombo_ = nullptr;              ///< ROI类型选择下拉框
    QLabel* roiStatusLabel_ = nullptr;               ///< ROI状态显示标签

    // ==================== UI组件 - 参数面板 ====================

    QTabWidget* paramTabWidget_ = nullptr;           ///< 参数选项卡控件
    QWidget* parameterPanel_ = nullptr;              ///< 子类创建的参数面板

    // ==================== UI组件 - 结果显示 ====================

    QGroupBox* resultGroup_ = nullptr;               ///< 搜索结果分组框
    QTableWidget* resultTable_ = nullptr;            ///< 结果表格（显示匹配详情）
    QLabel* resultCountLabel_ = nullptr;             ///< 结果数量标签
    QLabel* searchTimeLabel_ = nullptr;              ///< 搜索耗时标签

    // ==================== UI组件 - 底部按钮 ====================

    QCheckBox* autoPreviewCheck_ = nullptr;          ///< 实时预览复选框
    QPushButton* previewBtn_ = nullptr;              ///< 预览按钮
    QPushButton* okBtn_ = nullptr;                   ///< 确定按钮
    QPushButton* cancelBtn_ = nullptr;               ///< 取消按钮
    QPushButton* applyBtn_ = nullptr;                ///< 应用按钮

    // ==================== 数据成员 ====================

    std::vector<SearchResult> searchResults_;        ///< 当前搜索结果缓存
};

} // namespace UI
} // namespace VisionForge
