/**
 * @file LogicOperationToolDialog.h
 * @brief 逻辑运算工具参数编辑对话框
 * @author VisionForge Team
 * @date 2025-12-18
 */

#pragma once

#include <QDialog>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QGroupBox>
#include <QCheckBox>
#include <QTableWidget>
#include <QLineEdit>
#include <QVBoxLayout>

namespace VisionForge {

namespace Algorithm {
    class LogicOperationTool;
}

namespace UI {

/**
 * @class LogicOperationToolDialog
 * @brief 逻辑运算工具参数编辑对话框
 *
 * 支持的运算类型:
 * - AND (与运算)
 * - OR (或运算)
 * - NOT (非运算)
 * - XOR (异或运算)
 * - NAND (与非运算)
 * - NOR (或非运算)
 */
class LogicOperationToolDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LogicOperationToolDialog(Algorithm::LogicOperationTool* tool, QWidget* parent = nullptr);
    ~LogicOperationToolDialog() override;

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
     * @brief 请求执行运算
     */
    void executeOperationRequested();

protected:
    void showEvent(QShowEvent* event) override;

private slots:
    // 参数槽
    void onLogicTypeChanged(int index);

    // 输入操作
    void onAddInput();
    void onRemoveInput();
    void onClearInputs();
    void onInputCellChanged(int row, int column);

    // 对话框按钮槽
    void onPreviewClicked();
    void onOkClicked();
    void onCancelClicked();
    void onApplyClicked();

private:
    void createUI();
    void createLogicTypeGroup(QVBoxLayout* layout);
    void createInputsGroup(QVBoxLayout* layout);
    void createOutputGroup(QVBoxLayout* layout);
    void createResultGroup(QVBoxLayout* layout);
    void createButtonGroup(QVBoxLayout* layout);
    void connectSignals();
    void applyParameters();
    void updateInputsTable();
    void updateResultDisplay();
    void syncInputsFromTable();

private:
    Algorithm::LogicOperationTool* tool_;

    // 运算类型
    QComboBox* logicTypeCombo_;

    // 输入表格
    QTableWidget* inputsTable_;
    QPushButton* addInputBtn_;
    QPushButton* removeInputBtn_;
    QPushButton* clearInputsBtn_;

    // 输出设置
    QLineEdit* outputNameEdit_;

    // 结果显示
    QLabel* resultLabel_;
    QLabel* expressionLabel_;
    QLabel* trueCountLabel_;
    QLabel* falseCountLabel_;

    // 对话框按钮
    QPushButton* previewBtn_;
    QPushButton* okBtn_;
    QPushButton* cancelBtn_;
    QPushButton* applyBtn_;
};

} // namespace UI
} // namespace VisionForge
