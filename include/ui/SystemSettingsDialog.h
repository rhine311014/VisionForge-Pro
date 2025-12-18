/**
 * @file SystemSettingsDialog.h
 * @brief 系统设置对话框
 * @details 配置系统全局设置，如GPU加速模式等
 * @author VisionForge Team
 * @date 2025-12-18
 */

#pragma once

#include <QDialog>
#include <QRadioButton>
#include <QButtonGroup>
#include <QLabel>
#include <QGroupBox>
#include <QPushButton>
#include "base/GPUAccelerator.h"

namespace VisionForge {
namespace UI {

/**
 * @class SystemSettingsDialog
 * @brief 系统设置对话框
 *
 * 功能：
 * - GPU加速模式选择（禁用/CUDA/自动）
 * - 显示GPU设备信息
 * - 保存/加载配置
 */
class SystemSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SystemSettingsDialog(QWidget* parent = nullptr);
    ~SystemSettingsDialog() override = default;

private slots:
    void onAccelModeChanged(int id);
    void onOkClicked();
    void onApplyClicked();
    void onCancelClicked();

private:
    void setupUI();
    void loadSettings();
    void applySettings();
    void updateGPUStatusDisplay();

private:
    // GPU加速设置
    QGroupBox* gpuGroupBox_;
    QRadioButton* radioDisabled_;
    QRadioButton* radioCUDA_;
    QRadioButton* radioAuto_;
    QButtonGroup* accelModeGroup_;

    // GPU状态显示
    QLabel* gpuStatusLabel_;
    QLabel* gpuInfoLabel_;

    // 按钮
    QPushButton* okButton_;
    QPushButton* applyButton_;
    QPushButton* cancelButton_;

    // 当前选择的模式
    Base::GPUAccelMode selectedMode_;
};

} // namespace UI
} // namespace VisionForge
