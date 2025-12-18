/**
 * @file PLCOutputToolDialog.h
 * @brief PLC输出工具参数编辑对话框
 * @author VisionForge Team
 * @date 2025-12-18
 */

#pragma once

#include <QDialog>
#include <QComboBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QLabel>
#include <QGroupBox>
#include <QCheckBox>
#include <QTableWidget>
#include <QLineEdit>
#include <QVBoxLayout>

namespace VisionForge {

namespace Algorithm {
    class PLCOutputTool;
}

namespace UI {

/**
 * @class PLCOutputToolDialog
 * @brief PLC输出工具参数编辑对话框
 *
 * 支持的功能:
 * - 输出映射配置（数据源到PLC地址）
 * - 结果输出地址设置（OK/NG）
 * - 完成信号地址设置
 * - 数据缩放和偏移
 */
class PLCOutputToolDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PLCOutputToolDialog(Algorithm::PLCOutputTool* tool, QWidget* parent = nullptr);
    ~PLCOutputToolDialog() override;

    /**
     * @brief 更新UI显示
     */
    void updateUI();

signals:
    /**
     * @brief 参数已改变
     */
    void parameterChanged();

protected:
    void showEvent(QShowEvent* event) override;

private slots:
    // 映射操作
    void onAddMappingClicked();
    void onRemoveMappingClicked();
    void onClearMappingsClicked();
    void onMappingSelected(int row);
    void onMappingCellChanged(int row, int column);

    // 参数变更
    void onResultAddressChanged(int value);
    void onCompleteAddressChanged(int value);
    void onOutputResultChanged(bool checked);
    void onOutputCompleteChanged(bool checked);

    // 测试输出
    void onTestOutputClicked();

    // 对话框按钮槽
    void onOkClicked();
    void onCancelClicked();
    void onApplyClicked();

private:
    void createUI();
    void createMappingGroup(QVBoxLayout* layout);
    void createResultGroup(QVBoxLayout* layout);
    void createCompleteGroup(QVBoxLayout* layout);
    void createTestGroup(QVBoxLayout* layout);
    void createButtonGroup(QVBoxLayout* layout);
    void connectSignals();
    void applyParameters();
    void updateMappingTable();
    void syncMappingsFromTable();

private:
    Algorithm::PLCOutputTool* tool_;

    // 映射表格
    QTableWidget* mappingTable_;
    QPushButton* addMappingBtn_;
    QPushButton* removeMappingBtn_;
    QPushButton* clearMappingsBtn_;

    // 结果输出设置
    QCheckBox* outputResultCheck_;
    QSpinBox* resultAddressSpin_;
    QLabel* resultStatusLabel_;

    // 完成信号设置
    QCheckBox* outputCompleteCheck_;
    QSpinBox* completeAddressSpin_;
    QLabel* completeStatusLabel_;

    // 测试区域
    QLineEdit* testKeyEdit_;
    QDoubleSpinBox* testValueSpin_;
    QPushButton* testOutputBtn_;
    QLabel* testResultLabel_;

    // 对话框按钮
    QPushButton* okBtn_;
    QPushButton* cancelBtn_;
    QPushButton* applyBtn_;
};

} // namespace UI
} // namespace VisionForge
