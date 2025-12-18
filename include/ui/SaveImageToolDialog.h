/**
 * @file SaveImageToolDialog.h
 * @brief 图像保存工具参数编辑对话框
 * @author VisionForge Team
 * @date 2025-12-18
 */

#pragma once

#include <QDialog>
#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>
#include <QLabel>
#include <QGroupBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QVBoxLayout>

namespace VisionForge {

namespace Algorithm {
    class SaveImageTool;
}

namespace UI {

/**
 * @class SaveImageToolDialog
 * @brief 图像保存工具参数编辑对话框
 *
 * 支持的功能:
 * - 保存路径选择
 * - 多种图像格式(PNG/JPG/BMP/TIFF)
 * - 命名规则设置
 * - JPEG质量参数
 * - 保存条件(总是/仅OK/仅NG)
 */
class SaveImageToolDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SaveImageToolDialog(Algorithm::SaveImageTool* tool, QWidget* parent = nullptr);
    ~SaveImageToolDialog() override;

    /**
     * @brief 更新UI显示
     */
    void updateUI();

signals:
    /**
     * @brief 参数已改变
     */
    void parameterChanged();

    /**
     * @brief 请求执行保存
     */
    void executeSaveRequested();

protected:
    void showEvent(QShowEvent* event) override;

private slots:
    // 参数槽
    void onBrowseClicked();
    void onFormatChanged(int index);
    void onNamingModeChanged(int index);
    void onSaveConditionChanged(int index);
    void onJpegQualityChanged(int value);
    void onDateSubfolderChanged(bool checked);
    void onPrefixChanged(const QString& text);

    // 对话框按钮槽
    void onPreviewClicked();
    void onOkClicked();
    void onCancelClicked();
    void onApplyClicked();

private:
    void createUI();
    void createPathGroup(QVBoxLayout* layout);
    void createFormatGroup(QVBoxLayout* layout);
    void createNamingGroup(QVBoxLayout* layout);
    void createQualityGroup(QVBoxLayout* layout);
    void createConditionGroup(QVBoxLayout* layout);
    void createStatusGroup(QVBoxLayout* layout);
    void createButtonGroup(QVBoxLayout* layout);
    void connectSignals();
    void applyParameters();
    void updateStatusDisplay();
    QString generatePreviewFilename();

private:
    Algorithm::SaveImageTool* tool_;

    // 保存路径
    QLineEdit* pathEdit_;
    QPushButton* browseBtn_;

    // 图像格式
    QComboBox* formatCombo_;

    // 命名规则
    QComboBox* namingModeCombo_;
    QLineEdit* prefixEdit_;
    QLabel* filenamePreviewLabel_;

    // 质量参数
    QSpinBox* jpegQualitySpin_;
    QLabel* jpegQualityLabel_;

    // 保存条件
    QComboBox* saveConditionCombo_;
    QCheckBox* dateSubfolderCheck_;

    // 状态显示
    QLabel* saveCountLabel_;
    QLabel* lastSavedLabel_;

    // 对话框按钮
    QPushButton* previewBtn_;
    QPushButton* resetCounterBtn_;
    QPushButton* okBtn_;
    QPushButton* cancelBtn_;
    QPushButton* applyBtn_;
};

} // namespace UI
} // namespace VisionForge
