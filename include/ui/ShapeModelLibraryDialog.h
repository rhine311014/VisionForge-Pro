/**
 * @file ShapeModelLibraryDialog.h
 * @brief 形状模板库对话框
 * @author VisionForge Team
 * @date 2025-12-15
 */

#pragma once

#include "algorithm/ShapeModelManager.h"
#include <QDialog>
#include <QListWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>

namespace VisionForge {
namespace UI {

/**
 * @class ShapeModelLibraryDialog
 * @brief 形状模板库对话框
 *
 * 功能：
 * - 显示模板库中的所有模板
 * - 支持模板预览
 * - 支持导入/导出/删除
 * - 支持搜索和排序
 */
class ShapeModelLibraryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ShapeModelLibraryDialog(QWidget* parent = nullptr);
    ~ShapeModelLibraryDialog() override = default;

    /**
     * @brief 设置模板库路径
     */
    void setLibraryPath(const QString& path);

    /**
     * @brief 获取选中的模板ID
     */
    QString getSelectedModelId() const;

    /**
     * @brief 获取选中的模板文件路径
     */
    QString getSelectedModelPath() const;

signals:
    /**
     * @brief 模板被选择（双击或点击"使用"按钮）
     */
    void modelSelected(const QString& modelId);

private slots:
    void onImportClicked();
    void onExportClicked();
    void onDeleteClicked();
    void onRefreshClicked();
    void onSearchTextChanged(const QString& text);
    void onModelSelectionChanged();
    void onModelDoubleClicked(QListWidgetItem* item);
    void onUseModelClicked();

    void onLibraryLoaded();
    void onModelAdded(const QString& modelId);
    void onModelRemoved(const QString& modelId);

private:
    void createUI();
    void connectSignals();
    void loadModels();
    void updatePreview(Algorithm::ShapeModelInfoPtr info);
    void clearPreview();

private:
    // UI组件
    QLineEdit* searchEdit_;
    QListWidget* modelList_;

    // 预览区域
    QLabel* thumbnailLabel_;
    QLabel* nameLabel_;
    QLabel* descLabel_;
    QTextEdit* detailsText_;

    // 按钮
    QPushButton* importBtn_;
    QPushButton* exportBtn_;
    QPushButton* deleteBtn_;
    QPushButton* refreshBtn_;
    QPushButton* useBtn_;
    QPushButton* closeBtn_;

    // 数据
    QString currentModelId_;
};

/**
 * @class ModelListItemWidget
 * @brief 模板列表项Widget
 */
class ModelListItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ModelListItemWidget(Algorithm::ShapeModelInfoPtr info, QWidget* parent = nullptr);

    QString getModelId() const { return modelId_; }

private:
    QString modelId_;
    QLabel* thumbnailLabel_;
    QLabel* nameLabel_;
    QLabel* infoLabel_;
};

} // namespace UI
} // namespace VisionForge
