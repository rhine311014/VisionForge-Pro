/**
 * @file SearchToolDialog.h
 * @brief 搜索工具对话框基类
 * @details 为图案搜索、角点搜索、快速搜索等提供统一的基类框架
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

class QVBoxLayout;
class QHBoxLayout;

namespace VisionForge {

namespace Algorithm {
    class VisionTool;
}

namespace UI {

class HalconImageViewer;
class ROIShape;
using ROIShapePtr = std::shared_ptr<ROIShape>;

/**
 * @brief 搜索工具对话框基类
 *
 * 提供搜索类工具（图案搜索、角点搜索、快速搜索等）的通用UI框架。
 * 子类只需实现特定的参数面板和搜索逻辑。
 */
class SearchToolDialog : public IToolDialog
{
    Q_OBJECT

public:
    /**
     * @brief 搜索结果结构
     */
    struct SearchResult {
        double x = 0;           ///< X坐标
        double y = 0;           ///< Y坐标
        double angle = 0;       ///< 角度（度）
        double scale = 1.0;     ///< 缩放比例
        double score = 0;       ///< 匹配分数
        int index = 0;          ///< 结果索引
    };

    explicit SearchToolDialog(Algorithm::VisionTool* tool, QWidget* parent = nullptr);
    ~SearchToolDialog() override;

    // IToolDialog接口实现
    void setImage(Base::ImageData::Ptr image) override;
    Algorithm::VisionTool* tool() const override { return tool_; }

protected:
    /**
     * @brief 创建参数面板 - 子类必须实现
     * @param parent 父控件
     * @return 参数面板控件
     */
    virtual QWidget* createParameterPanel(QWidget* parent) = 0;

    /**
     * @brief 获取搜索工具名称 - 子类必须实现
     */
    virtual QString toolName() const = 0;

    /**
     * @brief 执行搜索 - 子类必须实现
     * @param results 输出搜索结果
     * @return 是否成功
     */
    virtual bool executeSearch(std::vector<SearchResult>& results) = 0;

    /**
     * @brief 子类特有的参数应用 - 子类可选实现
     */
    virtual void applySpecificParameters() {}

    /**
     * @brief 更新子类特有的UI - 子类可选实现
     */
    virtual void updateSpecificUI() {}

    // 通用UI创建方法
    void setupUI();
    void createLeftPanel(QWidget* parent);
    void createRightPanel(QWidget* parent);
    void createROIGroup(QVBoxLayout* layout);
    void createResultGroup(QVBoxLayout* layout);
    void createButtonPanel(QVBoxLayout* layout);
    void connectSignals();

    // 通用方法
    void updateUI();
    void updateResultTable(const std::vector<SearchResult>& results);
    void drawSearchResults(const std::vector<SearchResult>& results);
    QRect getSearchROI() const;
    bool hasSearchROI() const;

protected slots:
    void onDrawROIClicked();
    void onClearROIClicked();
    void onPreviewClicked();
    void onAutoPreview();
    void onOkClicked();
    void onCancelClicked();
    void onApplyClicked();
    void onResultTableRowSelected(int row);

signals:
    void parameterChanged();
    void searchCompleted(const std::vector<SearchResult>& results);

protected:
    // 核心成员
    Algorithm::VisionTool* tool_ = nullptr;
    Base::ImageData::Ptr currentImage_;
    PreviewHelper* previewHelper_ = nullptr;

    // UI组件 - 主布局
    QSplitter* mainSplitter_ = nullptr;
    QWidget* leftPanel_ = nullptr;
    QWidget* rightPanel_ = nullptr;

    // 图像显示
    HalconImageViewer* imageViewer_ = nullptr;
    QLabel* imageInfoLabel_ = nullptr;

    // ROI控制
    QGroupBox* roiGroup_ = nullptr;
    QPushButton* drawROIBtn_ = nullptr;
    QPushButton* clearROIBtn_ = nullptr;
    QComboBox* roiTypeCombo_ = nullptr;
    QLabel* roiStatusLabel_ = nullptr;

    // 参数面板容器
    QTabWidget* paramTabWidget_ = nullptr;
    QWidget* parameterPanel_ = nullptr;

    // 结果显示
    QGroupBox* resultGroup_ = nullptr;
    QTableWidget* resultTable_ = nullptr;
    QLabel* resultCountLabel_ = nullptr;
    QLabel* searchTimeLabel_ = nullptr;

    // 底部按钮
    QCheckBox* autoPreviewCheck_ = nullptr;
    QPushButton* previewBtn_ = nullptr;
    QPushButton* okBtn_ = nullptr;
    QPushButton* cancelBtn_ = nullptr;
    QPushButton* applyBtn_ = nullptr;

    // 搜索结果
    std::vector<SearchResult> searchResults_;
};

} // namespace UI
} // namespace VisionForge
