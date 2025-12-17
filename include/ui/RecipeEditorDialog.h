/**
 * @file RecipeEditorDialog.h
 * @brief 方案编辑对话框
 * @author VisionForge Team
 * @date 2025-12-17
 */

#pragma once

#include "core/Recipe.h"
#include <QDialog>
#include <QLineEdit>
#include <QTextEdit>
#include <QListWidget>
#include <QPushButton>
#include <QLabel>
#include <QGroupBox>
#include <QTableWidget>

namespace VisionForge {

/**
 * @class RecipeEditorDialog
 * @brief 方案编辑对话框
 *
 * 编辑方案的详细信息，包括：
 * - 基本信息（名称、描述、版本、作者）
 * - 产品信息（产品编号）
 * - 标签管理
 * - 自定义参数
 * - 工具链预览
 */
class RecipeEditorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RecipeEditorDialog(Core::Recipe* recipe, QWidget* parent = nullptr);
    ~RecipeEditorDialog();

    /**
     * @brief 获取编辑的方案
     */
    Core::Recipe* recipe() const { return recipe_; }

private slots:
    // 标签管理
    void onAddTag();
    void onRemoveTag();

    // 自定义参数管理
    void onAddParam();
    void onRemoveParam();
    void onParamChanged(int row, int column);

    // 对话框按钮
    void onApply();
    void onOK();
    void onCancel();

    // 导出报告
    void onExportReport();

private:
    void setupUI();
    void setupConnections();

    // 创建UI组件
    QWidget* createBasicInfoPanel();
    QWidget* createProductInfoPanel();
    QWidget* createTagsPanel();
    QWidget* createCustomParamsPanel();
    QWidget* createToolChainPreviewPanel();
    QWidget* createButtonPanel();

    // 加载/保存数据
    void loadFromRecipe();
    void saveToRecipe();
    void updateToolChainPreview();

private:
    Core::Recipe* recipe_;

    // 基本信息
    QLineEdit* nameEdit_;
    QTextEdit* descEdit_;
    QLineEdit* versionEdit_;
    QLineEdit* authorEdit_;
    QLabel* createTimeLabel_;
    QLabel* modifyTimeLabel_;

    // 产品信息
    QLineEdit* productCodeEdit_;

    // 标签
    QListWidget* tagList_;
    QLineEdit* tagEdit_;
    QPushButton* addTagBtn_;
    QPushButton* removeTagBtn_;

    // 自定义参数
    QTableWidget* paramTable_;
    QPushButton* addParamBtn_;
    QPushButton* removeParamBtn_;

    // 工具链预览
    QTableWidget* toolChainTable_;

    // 按钮
    QPushButton* applyBtn_;
    QPushButton* okBtn_;
    QPushButton* cancelBtn_;
    QPushButton* exportBtn_;
};

} // namespace VisionForge
