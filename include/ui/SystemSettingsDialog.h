/**
 * @file SystemSettingsDialog.h
 * @brief 系统设置对话框
 * @details 配置系统全局设置，包括平台类型、GPU加速模式等
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
#include <QComboBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QTabWidget>
#include <QCheckBox>
#include "base/GPUAccelerator.h"
#include "platform/PlatformTypes.h"
#include "platform/PlatformConfig.h"

namespace VisionForge {
namespace UI {

/**
 * @class SystemSettingsDialog
 * @brief 系统设置对话框
 *
 * 功能：
 * - 平台类型选择和配置
 * - 轴参数配置（行程、方向、脉冲当量）
 * - 相机配置
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
    void onPlatformTypeChanged(int index);
    void onAxisDirectionChanged();
    void onOkClicked();
    void onApplyClicked();
    void onCancelClicked();

private:
    void setupUI();
    void setupPlatformTab(QWidget* tab);
    void setupAxisTab(QWidget* tab);
    void setupCameraTab(QWidget* tab);
    void setupGPUTab(QWidget* tab);
    void loadSettings();
    void loadPlatformSettings();
    void applySettings();
    void applyPlatformSettings();
    void updateGPUStatusDisplay();
    void updateAxisVisibility();

private:
    // 主标签页
    QTabWidget* tabWidget_;

    // ========== 平台类型设置 ==========
    QGroupBox* platformGroupBox_;
    QComboBox* platformTypeCombo_;
    QLabel* platformDescLabel_;
    QSpinBox* cameraNumSpin_;
    QSpinBox* positionNumSpin_;

    // ========== 轴参数设置 ==========
    QGroupBox* axisGroupBox_;

    // X轴配置
    QDoubleSpinBox* xRangeSpin_;
    QDoubleSpinBox* xPulseSpin_;
    QComboBox* xDirectionCombo_;

    // X2轴配置（龙门结构）
    QWidget* x2AxisWidget_;
    QDoubleSpinBox* x2RangeSpin_;
    QDoubleSpinBox* x2PulseSpin_;
    QComboBox* x2DirectionCombo_;

    // Y轴配置
    QDoubleSpinBox* yRangeSpin_;
    QDoubleSpinBox* yPulseSpin_;
    QComboBox* yDirectionCombo_;

    // Y2轴配置（双Y结构）
    QWidget* y2AxisWidget_;
    QDoubleSpinBox* y2RangeSpin_;
    QDoubleSpinBox* y2PulseSpin_;
    QComboBox* y2DirectionCombo_;

    // D轴配置
    QWidget* dAxisWidget_;
    QDoubleSpinBox* dRangeSpin_;
    QDoubleSpinBox* dPulseSpin_;
    QComboBox* dDirectionCombo_;
    QComboBox* dDriveTypeCombo_;
    QDoubleSpinBox* rotationLengthSpin_;

    // ========== 相机平台设置 ==========
    QGroupBox* cameraGroupBox_;
    QComboBox* cameraPlatformTypeCombo_;
    QComboBox* cam1XDirectionCombo_;
    QComboBox* cam1YDirectionCombo_;
    QComboBox* cam2XDirectionCombo_;
    QComboBox* cam2YDirectionCombo_;

    // ========== GPU加速设置 ==========
    QGroupBox* gpuGroupBox_;
    QRadioButton* radioDisabled_;
    QRadioButton* radioCUDA_;
    QRadioButton* radioAuto_;
    QButtonGroup* accelModeGroup_;

    // GPU状态显示
    QLabel* gpuStatusLabel_;
    QLabel* gpuInfoLabel_;

    // ========== 按钮 ==========
    QPushButton* okButton_;
    QPushButton* applyButton_;
    QPushButton* cancelButton_;

    // ========== 当前配置 ==========
    Base::GPUAccelMode selectedMode_;
    Platform::PlatformType selectedPlatformType_;
};

} // namespace UI
} // namespace VisionForge
