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
    QTabWidget* tabWidget_ = nullptr;

    // ========== 平台类型设置 ==========
    QGroupBox* platformGroupBox_ = nullptr;
    QComboBox* platformTypeCombo_ = nullptr;
    QLabel* platformDescLabel_ = nullptr;
    QSpinBox* cameraNumSpin_ = nullptr;
    QSpinBox* positionNumSpin_ = nullptr;

    // ========== 轴参数设置 ==========
    QGroupBox* axisGroupBox_ = nullptr;

    // X轴配置
    QDoubleSpinBox* xRangeSpin_ = nullptr;
    QDoubleSpinBox* xPulseSpin_ = nullptr;
    QComboBox* xDirectionCombo_ = nullptr;

    // X2轴配置（龙门结构）
    QWidget* x2AxisWidget_ = nullptr;
    QDoubleSpinBox* x2RangeSpin_ = nullptr;
    QDoubleSpinBox* x2PulseSpin_ = nullptr;
    QComboBox* x2DirectionCombo_ = nullptr;

    // Y轴配置
    QDoubleSpinBox* yRangeSpin_ = nullptr;
    QDoubleSpinBox* yPulseSpin_ = nullptr;
    QComboBox* yDirectionCombo_ = nullptr;

    // Y2轴配置（双Y结构）
    QWidget* y2AxisWidget_ = nullptr;
    QDoubleSpinBox* y2RangeSpin_ = nullptr;
    QDoubleSpinBox* y2PulseSpin_ = nullptr;
    QComboBox* y2DirectionCombo_ = nullptr;

    // D轴配置
    QWidget* dAxisWidget_ = nullptr;
    QDoubleSpinBox* dRangeSpin_ = nullptr;
    QDoubleSpinBox* dPulseSpin_ = nullptr;
    QComboBox* dDirectionCombo_ = nullptr;
    QComboBox* dDriveTypeCombo_ = nullptr;
    QDoubleSpinBox* rotationLengthSpin_ = nullptr;

    // ========== 相机平台设置 ==========
    QGroupBox* cameraGroupBox_ = nullptr;
    QComboBox* cameraPlatformTypeCombo_ = nullptr;
    QComboBox* cam1XDirectionCombo_ = nullptr;
    QComboBox* cam1YDirectionCombo_ = nullptr;
    QComboBox* cam2XDirectionCombo_ = nullptr;
    QComboBox* cam2YDirectionCombo_ = nullptr;

    // ========== GPU加速设置 ==========
    QGroupBox* gpuGroupBox_ = nullptr;
    QRadioButton* radioDisabled_ = nullptr;
    QRadioButton* radioCUDA_ = nullptr;
    QRadioButton* radioAuto_ = nullptr;
    QButtonGroup* accelModeGroup_ = nullptr;

    // GPU状态显示
    QLabel* gpuStatusLabel_ = nullptr;
    QLabel* gpuInfoLabel_ = nullptr;

    // ========== 按钮 ==========
    QPushButton* okButton_ = nullptr;
    QPushButton* applyButton_ = nullptr;
    QPushButton* cancelButton_ = nullptr;

    // ========== 当前配置 ==========
    Base::GPUAccelMode selectedMode_;
    Platform::PlatformType selectedPlatformType_;
};

} // namespace UI
} // namespace VisionForge
