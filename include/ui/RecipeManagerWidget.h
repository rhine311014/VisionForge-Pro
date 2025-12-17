/**
 * @file RecipeManagerWidget.h
 * @brief 方案管理界面
 * @author VisionForge Team
 * @date 2025-12-17
 */

#pragma once

#include "core/Recipe.h"
#include "core/RecipeManager.h"
#include <QWidget>
#include <QListWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QGroupBox>
#include <QMenu>

namespace VisionForge {

/**
 * @class RecipeManagerWidget
 * @brief 方案管理面板
 *
 * 提供方案的创建、加载、保存、删除、切换等功能
 */
class RecipeManagerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RecipeManagerWidget(QWidget* parent = nullptr);
    ~RecipeManagerWidget();

    /**
     * @brief 刷新方案列表
     */
    void refreshRecipeList();

    /**
     * @brief 获取当前选中的方案
     */
    Core::Recipe* currentRecipe() const;

    /**
     * @brief 设置方案目录
     */
    void setRecipeDirectory(const QString& path);

signals:
    /**
     * @brief 方案选择改变信号
     */
    void recipeSelected(Core::Recipe* recipe);

    /**
     * @brief 方案激活信号（双击或点击应用）
     */
    void recipeActivated(Core::Recipe* recipe);

    /**
     * @brief 方案创建信号
     */
    void recipeCreated(Core::Recipe* recipe);

    /**
     * @brief 方案删除信号
     */
    void recipeDeleted(const QString& name);

    /**
     * @brief 请求编辑方案信号
     */
    void editRecipeRequested(Core::Recipe* recipe);

private slots:
    // 方案操作
    void onNewRecipe();
    void onOpenRecipe();
    void onSaveRecipe();
    void onSaveRecipeAs();
    void onDeleteRecipe();
    void onDuplicateRecipe();
    void onExportRecipe();
    void onImportRecipe();

    // 列表操作
    void onRecipeSelected(QListWidgetItem* item);
    void onRecipeDoubleClicked(QListWidgetItem* item);
    void onContextMenu(const QPoint& pos);

    // 搜索和过滤
    void onSearchTextChanged(const QString& text);
    void onFilterChanged(int index);

    // 应用方案
    void onApplyRecipe();

    // 编辑方案
    void onEditRecipe();

    // RecipeManager信号响应
    void onRecipeListChanged();
    void onCurrentRecipeChanged(const QString& name);

private:
    void setupUI();
    void setupConnections();
    void createContextMenu();

    // 创建UI组件
    QWidget* createToolBar();
    QWidget* createSearchPanel();
    QWidget* createRecipeListPanel();
    QWidget* createInfoPanel();

    // 更新UI
    void updateRecipeInfo(Core::Recipe* recipe);
    void updateButtonStates();
    QString getRecipeDisplayName(Core::Recipe* recipe) const;

private:
    // 工具栏按钮
    QPushButton* newBtn_;
    QPushButton* openBtn_;
    QPushButton* saveBtn_;
    QPushButton* deleteBtn_;
    QPushButton* applyBtn_;

    // 搜索和过滤
    QLineEdit* searchEdit_;
    QComboBox* filterCombo_;

    // 方案列表
    QListWidget* recipeList_;
    QMenu* contextMenu_;

    // 信息面板
    QGroupBox* infoGroup_;
    QLabel* nameLabel_;
    QLabel* descLabel_;
    QLabel* productCodeLabel_;
    QLabel* tagsLabel_;
    QLabel* toolCountLabel_;
    QLabel* modifyTimeLabel_;
    QPushButton* editBtn_;

    // 状态
    QLabel* statusLabel_;
};

} // namespace VisionForge
