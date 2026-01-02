/**
 * @file QuickSearchToolDialog.h
 * @brief 快速搜索工具对话框
 * @details 轻量级的快速模板匹配工具
 * @author VisionForge Team
 * @date 2025-12-30
 */

#pragma once

#include "ui/SearchToolDialog.h"

namespace VisionForge {

namespace Algorithm {
    class QuickSearchTool;
}

namespace UI {

/**
 * @brief 快速搜索工具对话框
 *
 * 提供简化参数的快速模板匹配功能。
 * 特点：
 * - 简单易用的参数设置
 * - 快速的搜索速度
 * - 适合快速定位场景
 */
class QuickSearchToolDialog : public SearchToolDialog
{
    Q_OBJECT

public:
    explicit QuickSearchToolDialog(Algorithm::QuickSearchTool* tool, QWidget* parent = nullptr);
    ~QuickSearchToolDialog() override;

protected:
    // SearchToolDialog接口实现
    QWidget* createParameterPanel(QWidget* parent) override;
    QString toolName() const override { return tr("快速搜索"); }
    bool executeSearch(std::vector<SearchResult>& results) override;
    void applySpecificParameters() override;
    void updateSpecificUI() override;

private:
    void createTemplateGroup(QVBoxLayout* layout);
    void createMatchParamGroup(QVBoxLayout* layout);
    void createSpeedGroup(QVBoxLayout* layout);
    void connectSpecificSignals();
    void updateTemplatePreview();

private slots:
    void onCreateTemplateClicked();
    void onLoadTemplateClicked();
    void onSaveTemplateClicked();
    void onClearTemplateClicked();

private:
    Algorithm::QuickSearchTool* quickTool_ = nullptr;

    // 模板管理
    QGroupBox* templateGroup_ = nullptr;
    QPushButton* createTemplateBtn_ = nullptr;
    QPushButton* loadTemplateBtn_ = nullptr;
    QPushButton* saveTemplateBtn_ = nullptr;
    QPushButton* clearTemplateBtn_ = nullptr;
    QLabel* templateStatusLabel_ = nullptr;
    QLabel* templatePreviewLabel_ = nullptr;

    // 匹配参数
    QGroupBox* matchParamGroup_ = nullptr;
    QDoubleSpinBox* minScoreSpin_ = nullptr;
    QSlider* minScoreSlider_ = nullptr;
    QSpinBox* maxMatchesSpin_ = nullptr;
    QComboBox* matchMethodCombo_ = nullptr;

    // 搜索速度
    QGroupBox* speedGroup_ = nullptr;
    QComboBox* speedLevelCombo_ = nullptr;
    QLabel* speedDescLabel_ = nullptr;
    QCheckBox* useMultiScaleCheck_ = nullptr;
    QSpinBox* pyramidLevelsSpin_ = nullptr;
};

} // namespace UI
} // namespace VisionForge
