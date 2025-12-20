/**
 * @file RangeJudgeToolDialog.h
 * @brief 范围判定工具参数编辑对话框
 * @author VisionForge Team
 * @date 2025-12-18
 */

#pragma once

#include <QDialog>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QLabel>
#include <QGroupBox>
#include <QCheckBox>
#include <QTableWidget>
#include <QLineEdit>
#include <QVBoxLayout>

#include "ui/PreviewHelper.h"

namespace VisionForge {

namespace Algorithm {
    class RangeJudgeTool;
}

namespace UI {

/**
 * @class RangeJudgeToolDialog
 * @brief 范围判定工具参数编辑对话框
 *
 * 支持的功能:
 * - 多条件组合判定(AND/OR)
 * - 范围检查
 * - 输入值来源选择
 */
class RangeJudgeToolDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RangeJudgeToolDialog(Algorithm::RangeJudgeTool* tool, QWidget* parent = nullptr);
    ~RangeJudgeToolDialog() override;

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
     * @brief 请求执行判定
     */
    void executeJudgeRequested();

protected:
    void showEvent(QShowEvent* event) override;

private slots:
    // 参数槽
    void onCombineLogicChanged(int index);

    // 条件操作
    void onAddCondition();
    void onRemoveCondition();
    void onClearConditions();
    void onConditionCellChanged(int row, int column);

    // 测试槽
    void onTestValueChanged(double value);

    // 对话框按钮槽
    void onPreviewClicked();
    void onOkClicked();
    void onCancelClicked();
    void onApplyClicked();
    void onAutoCalc();

private:
    void createUI();
    void createCombineLogicGroup(QVBoxLayout* layout);
    void createConditionsGroup(QVBoxLayout* layout);
    void createTestGroup(QVBoxLayout* layout);
    void createResultGroup(QVBoxLayout* layout);
    void createButtonGroup(QVBoxLayout* layout);
    void connectSignals();
    void applyParameters();
    void updateConditionsTable();
    void updateResultDisplay();
    void syncConditionsFromTable();

private:
    Algorithm::RangeJudgeTool* tool_;

    // 组合逻辑
    QComboBox* combineLogicCombo_;

    // 条件表格
    QTableWidget* conditionsTable_;
    QPushButton* addConditionBtn_;
    QPushButton* removeConditionBtn_;
    QPushButton* clearConditionsBtn_;

    // 测试区域
    QLineEdit* testKeyEdit_;
    QDoubleSpinBox* testValueSpin_;
    QPushButton* testBtn_;

    // 结果显示
    QLabel* overallResultLabel_;
    QLabel* passCountLabel_;
    QLabel* failCountLabel_;
    QTableWidget* detailsTable_;

    // 对话框按钮
    QPushButton* previewBtn_;
    QPushButton* okBtn_;
    QPushButton* cancelBtn_;
    QPushButton* applyBtn_;

    // 实时计算
    PreviewHelper* previewHelper_;
    QCheckBox* autoCalcCheck_;
};

} // namespace UI
} // namespace VisionForge
