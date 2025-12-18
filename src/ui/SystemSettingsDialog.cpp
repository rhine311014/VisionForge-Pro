/**
 * @file SystemSettingsDialog.cpp
 * @brief 系统设置对话框实现
 */

#include "ui/SystemSettingsDialog.h"
#include "base/Logger.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>

namespace VisionForge {
namespace UI {

SystemSettingsDialog::SystemSettingsDialog(QWidget* parent)
    : QDialog(parent)
    , selectedMode_(Base::GPUAccelMode::Auto)
{
    setWindowTitle("系统设置");
    setMinimumSize(450, 350);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    setupUI();
    loadSettings();
}

void SystemSettingsDialog::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    // ========== GPU加速设置组 ==========
    gpuGroupBox_ = new QGroupBox("GPU加速设置", this);
    QVBoxLayout* gpuLayout = new QVBoxLayout(gpuGroupBox_);
    gpuLayout->setSpacing(10);

    // 加速模式选择
    accelModeGroup_ = new QButtonGroup(this);

    radioDisabled_ = new QRadioButton("禁用GPU加速 (仅使用CPU)", this);
    radioDisabled_->setToolTip("禁用所有GPU加速功能，适用于没有GPU或GPU不稳定的环境");
    accelModeGroup_->addButton(radioDisabled_, static_cast<int>(Base::GPUAccelMode::Disabled));
    gpuLayout->addWidget(radioDisabled_);

    radioCUDA_ = new QRadioButton("启用CUDA GPU加速", this);
    radioCUDA_->setToolTip("强制使用CUDA GPU加速，需要NVIDIA显卡和CUDA支持");
    accelModeGroup_->addButton(radioCUDA_, static_cast<int>(Base::GPUAccelMode::CUDA));
    gpuLayout->addWidget(radioCUDA_);

    radioAuto_ = new QRadioButton("自动选择 (推荐)", this);
    radioAuto_->setToolTip("自动检测GPU可用性，有GPU时使用GPU，否则使用CPU");
    accelModeGroup_->addButton(radioAuto_, static_cast<int>(Base::GPUAccelMode::Auto));
    gpuLayout->addWidget(radioAuto_);

    connect(accelModeGroup_, QOverload<int>::of(&QButtonGroup::idClicked),
            this, &SystemSettingsDialog::onAccelModeChanged);

    // 分隔线
    QFrame* line = new QFrame(this);
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    gpuLayout->addWidget(line);

    // GPU状态显示
    gpuStatusLabel_ = new QLabel(this);
    gpuStatusLabel_->setStyleSheet("font-weight: bold;");
    gpuLayout->addWidget(gpuStatusLabel_);

    gpuInfoLabel_ = new QLabel(this);
    gpuInfoLabel_->setWordWrap(true);
    gpuInfoLabel_->setStyleSheet("color: #666; font-size: 11px;");
    gpuLayout->addWidget(gpuInfoLabel_);

    mainLayout->addWidget(gpuGroupBox_);

    // 弹性空间
    mainLayout->addStretch();

    // ========== 按钮区域 ==========
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();

    okButton_ = new QPushButton("确定", this);
    okButton_->setFixedWidth(80);
    connect(okButton_, &QPushButton::clicked, this, &SystemSettingsDialog::onOkClicked);
    buttonLayout->addWidget(okButton_);

    applyButton_ = new QPushButton("应用", this);
    applyButton_->setFixedWidth(80);
    connect(applyButton_, &QPushButton::clicked, this, &SystemSettingsDialog::onApplyClicked);
    buttonLayout->addWidget(applyButton_);

    cancelButton_ = new QPushButton("取消", this);
    cancelButton_->setFixedWidth(80);
    connect(cancelButton_, &QPushButton::clicked, this, &SystemSettingsDialog::onCancelClicked);
    buttonLayout->addWidget(cancelButton_);

    mainLayout->addLayout(buttonLayout);
}

void SystemSettingsDialog::loadSettings()
{
    Base::GPUAccelerator& gpu = Base::GPUAccelerator::instance();
    selectedMode_ = gpu.getAccelMode();

    // 设置单选按钮
    QAbstractButton* button = accelModeGroup_->button(static_cast<int>(selectedMode_));
    if (button) {
        button->setChecked(true);
    }

    // 更新状态显示
    updateGPUStatusDisplay();
}

void SystemSettingsDialog::applySettings()
{
    Base::GPUAccelerator& gpu = Base::GPUAccelerator::instance();
    gpu.setAccelMode(selectedMode_);

    LOG_INFO(QString("GPU加速模式已更改为: %1")
            .arg(Base::GPUAccelerator::getAccelModeName(selectedMode_)));

    // 更新状态显示
    updateGPUStatusDisplay();
}

void SystemSettingsDialog::updateGPUStatusDisplay()
{
    Base::GPUAccelerator& gpu = Base::GPUAccelerator::instance();

    // 当前状态
    QString statusText;
    if (gpu.isCudaAvailable()) {
        int deviceCount = gpu.getDeviceCount();
        statusText = QString("CUDA状态: 可用 (检测到 %1 个GPU设备)").arg(deviceCount);
        gpuStatusLabel_->setStyleSheet("font-weight: bold; color: #2e7d32;");
    } else {
        statusText = "CUDA状态: 不可用";
        gpuStatusLabel_->setStyleSheet("font-weight: bold; color: #c62828;");
    }
    gpuStatusLabel_->setText(statusText);

    // 设备详细信息
    QString infoText;
    if (gpu.isCudaAvailable()) {
        QList<Base::GPUDeviceInfo> devices = gpu.getAllDevices();
        for (const Base::GPUDeviceInfo& dev : devices) {
            if (!infoText.isEmpty()) {
                infoText += "\n";
            }
            infoText += QString("GPU %1: %2\n  内存: %3 MB, 计算能力: %4")
                .arg(dev.deviceId)
                .arg(dev.name)
                .arg(dev.totalMemory / 1024 / 1024)
                .arg(dev.computeCapability / 10.0, 0, 'f', 1);
        }
    } else {
        infoText = "未检测到CUDA兼容的GPU设备。\n"
                   "请确保已安装NVIDIA显卡和CUDA驱动。\n"
                   "若选择\"自动\"模式，将自动使用CPU处理。";
    }

    // 添加当前模式说明
    infoText += QString("\n\n当前模式: %1")
        .arg(Base::GPUAccelerator::getAccelModeName(selectedMode_));

    if (gpu.isUsingGPU()) {
        infoText += " (实际使用: GPU)";
    } else {
        infoText += " (实际使用: CPU)";
    }

    gpuInfoLabel_->setText(infoText);

    // 如果CUDA不可用，禁用CUDA选项但显示提示
    radioCUDA_->setEnabled(gpu.isCudaAvailable());
    if (!gpu.isCudaAvailable()) {
        radioCUDA_->setText("启用CUDA GPU加速 (不可用)");
    } else {
        radioCUDA_->setText("启用CUDA GPU加速");
    }
}

void SystemSettingsDialog::onAccelModeChanged(int id)
{
    selectedMode_ = static_cast<Base::GPUAccelMode>(id);

    // 如果选择了CUDA但CUDA不可用，给出警告
    Base::GPUAccelerator& gpu = Base::GPUAccelerator::instance();
    if (selectedMode_ == Base::GPUAccelMode::CUDA && !gpu.isCudaAvailable()) {
        QMessageBox::warning(this, "警告",
            "当前系统不支持CUDA，选择此选项后将自动回退到CPU处理。\n"
            "建议选择\"自动选择\"模式。");
    }
}

void SystemSettingsDialog::onOkClicked()
{
    applySettings();
    accept();
}

void SystemSettingsDialog::onApplyClicked()
{
    applySettings();
    QMessageBox::information(this, "提示", "设置已应用");
}

void SystemSettingsDialog::onCancelClicked()
{
    reject();
}

} // namespace UI
} // namespace VisionForge
