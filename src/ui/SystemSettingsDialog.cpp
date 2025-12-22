/**
 * @file SystemSettingsDialog.cpp
 * @brief 系统设置对话框实现
 * @details 包含平台类型配置、轴参数、相机配置和GPU加速设置
 */

#include "ui/SystemSettingsDialog.h"
#include "base/Logger.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGridLayout>
#include <QMessageBox>
#include <QScrollArea>

namespace VisionForge {
namespace UI {

SystemSettingsDialog::SystemSettingsDialog(QWidget* parent)
    : QDialog(parent)
    , selectedMode_(Base::GPUAccelMode::Auto)
    , selectedPlatformType_(Platform::PlatformType::XYD)
{
    setWindowTitle("系统设置");
    setMinimumSize(650, 550);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    setupUI();
    loadSettings();
}

void SystemSettingsDialog::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(15, 15, 15, 15);

    // 创建标签页
    tabWidget_ = new QTabWidget(this);

    // 平台类型标签页
    QWidget* platformTab = new QWidget();
    setupPlatformTab(platformTab);
    tabWidget_->addTab(platformTab, "平台类型");

    // 轴参数标签页
    QWidget* axisTab = new QWidget();
    setupAxisTab(axisTab);
    tabWidget_->addTab(axisTab, "轴参数");

    // 相机配置标签页
    QWidget* cameraTab = new QWidget();
    setupCameraTab(cameraTab);
    tabWidget_->addTab(cameraTab, "相机配置");

    // GPU加速标签页
    QWidget* gpuTab = new QWidget();
    setupGPUTab(gpuTab);
    tabWidget_->addTab(gpuTab, "GPU加速");

    mainLayout->addWidget(tabWidget_);

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

void SystemSettingsDialog::setupPlatformTab(QWidget* tab)
{
    QVBoxLayout* layout = new QVBoxLayout(tab);
    layout->setSpacing(15);

    // ========== 平台类型选择 ==========
    platformGroupBox_ = new QGroupBox("平台类型选择", tab);
    QFormLayout* formLayout = new QFormLayout(platformGroupBox_);
    formLayout->setSpacing(10);

    // 平台类型下拉框
    platformTypeCombo_ = new QComboBox(this);

    // 阻止信号，避免在items添加过程中触发槽函数
    platformTypeCombo_->blockSignals(true);

    // 添加所有支持的平台类型
    QList<Platform::PlatformType> types = Platform::getSupportedPlatformTypes();
    for (Platform::PlatformType type : types) {
        QString name = Platform::getPlatformTypeName(type);
        platformTypeCombo_->addItem(name, static_cast<int>(type));
    }

    platformTypeCombo_->blockSignals(false);

    connect(platformTypeCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SystemSettingsDialog::onPlatformTypeChanged);
    formLayout->addRow("平台类型:", platformTypeCombo_);

    // 平台描述标签
    platformDescLabel_ = new QLabel(this);
    platformDescLabel_->setWordWrap(true);
    platformDescLabel_->setStyleSheet("color: #666; font-size: 11px; padding: 5px; background: #f5f5f5; border-radius: 3px;");
    formLayout->addRow("", platformDescLabel_);

    // 相机数量
    cameraNumSpin_ = new QSpinBox(this);
    cameraNumSpin_->setRange(1, 8);
    cameraNumSpin_->setValue(2);
    cameraNumSpin_->setToolTip("系统使用的相机数量");
    formLayout->addRow("相机数量:", cameraNumSpin_);

    // 对位点数量
    positionNumSpin_ = new QSpinBox(this);
    positionNumSpin_->setRange(1, 8);
    positionNumSpin_->setValue(2);
    positionNumSpin_->setToolTip("对位点数量（通常与相机数量一致）");
    formLayout->addRow("对位点数:", positionNumSpin_);

    layout->addWidget(platformGroupBox_);

    // ========== 平台说明 ==========
    QGroupBox* infoBox = new QGroupBox("平台类型说明", tab);
    QVBoxLayout* infoLayout = new QVBoxLayout(infoBox);

    QLabel* infoLabel = new QLabel(
        "<b>常用平台类型：</b><br>"
        "• <b>XYD</b> - 标准三轴平台：X轴+Y轴+旋转D轴<br>"
        "• <b>X1X2Y</b> - 龙门双X轴：适用于大幅面对位<br>"
        "• <b>XY1Y2</b> - 双Y平台：X轴+双Y轴结构<br>"
        "• <b>XY</b> - 简化双轴：仅XY平移，无旋转<br>"
        "• <b>DXY</b> - 旋转优先：先D轴旋转，再XY平移<br>"
        "<br>"
        "<b>注意：</b>切换平台类型后需重新标定！", this);
    infoLabel->setWordWrap(true);
    infoLabel->setStyleSheet("font-size: 11px;");
    infoLayout->addWidget(infoLabel);

    layout->addWidget(infoBox);
    layout->addStretch();
}

void SystemSettingsDialog::setupAxisTab(QWidget* tab)
{
    QScrollArea* scrollArea = new QScrollArea(tab);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);

    QWidget* scrollContent = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(scrollContent);
    layout->setSpacing(15);

    // ========== X轴配置 ==========
    QGroupBox* xAxisBox = new QGroupBox("X轴配置", scrollContent);
    QFormLayout* xLayout = new QFormLayout(xAxisBox);

    xRangeSpin_ = new QDoubleSpinBox(this);
    xRangeSpin_->setRange(0.1, 10000.0);
    xRangeSpin_->setValue(1000.0);
    xRangeSpin_->setSuffix(" mm");
    xRangeSpin_->setDecimals(1);
    xLayout->addRow("行程:", xRangeSpin_);

    xPulseSpin_ = new QDoubleSpinBox(this);
    xPulseSpin_->setRange(1, 1000000);
    xPulseSpin_->setValue(1000.0);
    xPulseSpin_->setSuffix(" 脉冲/mm");
    xPulseSpin_->setDecimals(2);
    xLayout->addRow("脉冲当量:", xPulseSpin_);

    xDirectionCombo_ = new QComboBox(this);
    xDirectionCombo_->addItem("正向 (+)", static_cast<int>(Platform::AxisDirectionType::Positive));
    xDirectionCombo_->addItem("负向 (-)", static_cast<int>(Platform::AxisDirectionType::Negative));
    xLayout->addRow("运动方向:", xDirectionCombo_);

    layout->addWidget(xAxisBox);

    // ========== X2轴配置（龙门结构专用） ==========
    x2AxisWidget_ = new QGroupBox("X2轴配置 (龙门结构)", scrollContent);
    QFormLayout* x2Layout = new QFormLayout(static_cast<QGroupBox*>(x2AxisWidget_));

    x2RangeSpin_ = new QDoubleSpinBox(this);
    x2RangeSpin_->setRange(0.1, 10000.0);
    x2RangeSpin_->setValue(1000.0);
    x2RangeSpin_->setSuffix(" mm");
    x2RangeSpin_->setDecimals(1);
    x2Layout->addRow("行程:", x2RangeSpin_);

    x2PulseSpin_ = new QDoubleSpinBox(this);
    x2PulseSpin_->setRange(1, 1000000);
    x2PulseSpin_->setValue(1000.0);
    x2PulseSpin_->setSuffix(" 脉冲/mm");
    x2PulseSpin_->setDecimals(2);
    x2Layout->addRow("脉冲当量:", x2PulseSpin_);

    x2DirectionCombo_ = new QComboBox(this);
    x2DirectionCombo_->addItem("正向 (+)", static_cast<int>(Platform::AxisDirectionType::Positive));
    x2DirectionCombo_->addItem("负向 (-)", static_cast<int>(Platform::AxisDirectionType::Negative));
    x2Layout->addRow("运动方向:", x2DirectionCombo_);

    layout->addWidget(x2AxisWidget_);

    // ========== Y轴配置 ==========
    QGroupBox* yAxisBox = new QGroupBox("Y轴配置", scrollContent);
    QFormLayout* yLayout = new QFormLayout(yAxisBox);

    yRangeSpin_ = new QDoubleSpinBox(this);
    yRangeSpin_->setRange(0.1, 10000.0);
    yRangeSpin_->setValue(1000.0);
    yRangeSpin_->setSuffix(" mm");
    yRangeSpin_->setDecimals(1);
    yLayout->addRow("行程:", yRangeSpin_);

    yPulseSpin_ = new QDoubleSpinBox(this);
    yPulseSpin_->setRange(1, 1000000);
    yPulseSpin_->setValue(1000.0);
    yPulseSpin_->setSuffix(" 脉冲/mm");
    yPulseSpin_->setDecimals(2);
    yLayout->addRow("脉冲当量:", yPulseSpin_);

    yDirectionCombo_ = new QComboBox(this);
    yDirectionCombo_->addItem("正向 (+)", static_cast<int>(Platform::AxisDirectionType::Positive));
    yDirectionCombo_->addItem("负向 (-)", static_cast<int>(Platform::AxisDirectionType::Negative));
    yLayout->addRow("运动方向:", yDirectionCombo_);

    layout->addWidget(yAxisBox);

    // ========== Y2轴配置（双Y结构专用） ==========
    y2AxisWidget_ = new QGroupBox("Y2轴配置 (双Y结构)", scrollContent);
    QFormLayout* y2Layout = new QFormLayout(static_cast<QGroupBox*>(y2AxisWidget_));

    y2RangeSpin_ = new QDoubleSpinBox(this);
    y2RangeSpin_->setRange(0.1, 10000.0);
    y2RangeSpin_->setValue(1000.0);
    y2RangeSpin_->setSuffix(" mm");
    y2RangeSpin_->setDecimals(1);
    y2Layout->addRow("行程:", y2RangeSpin_);

    y2PulseSpin_ = new QDoubleSpinBox(this);
    y2PulseSpin_->setRange(1, 1000000);
    y2PulseSpin_->setValue(1000.0);
    y2PulseSpin_->setSuffix(" 脉冲/mm");
    y2PulseSpin_->setDecimals(2);
    y2Layout->addRow("脉冲当量:", y2PulseSpin_);

    y2DirectionCombo_ = new QComboBox(this);
    y2DirectionCombo_->addItem("正向 (+)", static_cast<int>(Platform::AxisDirectionType::Positive));
    y2DirectionCombo_->addItem("负向 (-)", static_cast<int>(Platform::AxisDirectionType::Negative));
    y2Layout->addRow("运动方向:", y2DirectionCombo_);

    layout->addWidget(y2AxisWidget_);

    // ========== D轴配置（旋转轴） ==========
    dAxisWidget_ = new QGroupBox("D轴配置 (旋转轴)", scrollContent);
    QFormLayout* dLayout = new QFormLayout(static_cast<QGroupBox*>(dAxisWidget_));

    dRangeSpin_ = new QDoubleSpinBox(this);
    dRangeSpin_->setRange(0.1, 360.0);
    dRangeSpin_->setValue(360.0);
    dRangeSpin_->setSuffix(" °");
    dRangeSpin_->setDecimals(1);
    dLayout->addRow("旋转范围:", dRangeSpin_);

    dPulseSpin_ = new QDoubleSpinBox(this);
    dPulseSpin_->setRange(1, 1000000);
    dPulseSpin_->setValue(1000.0);
    dPulseSpin_->setSuffix(" 脉冲/°");
    dPulseSpin_->setDecimals(2);
    dLayout->addRow("脉冲当量:", dPulseSpin_);

    dDirectionCombo_ = new QComboBox(this);
    dDirectionCombo_->addItem("正向 (顺时针)", static_cast<int>(Platform::AxisDirectionType::Positive));
    dDirectionCombo_->addItem("负向 (逆时针)", static_cast<int>(Platform::AxisDirectionType::Negative));
    dLayout->addRow("旋转方向:", dDirectionCombo_);

    dDriveTypeCombo_ = new QComboBox(this);
    dDriveTypeCombo_->addItem("直接驱动 (DD马达)", static_cast<int>(Platform::DDriveType::Direct));
    dDriveTypeCombo_->addItem("直线驱动 (连杆)", static_cast<int>(Platform::DDriveType::Linear));
    dLayout->addRow("驱动类型:", dDriveTypeCombo_);

    rotationLengthSpin_ = new QDoubleSpinBox(this);
    rotationLengthSpin_->setRange(1.0, 1000.0);
    rotationLengthSpin_->setValue(100.0);
    rotationLengthSpin_->setSuffix(" mm");
    rotationLengthSpin_->setDecimals(2);
    rotationLengthSpin_->setToolTip("直线驱动时的旋转臂长度");
    dLayout->addRow("旋转臂长:", rotationLengthSpin_);

    layout->addWidget(dAxisWidget_);

    layout->addStretch();
    scrollArea->setWidget(scrollContent);

    QVBoxLayout* tabLayout = new QVBoxLayout(tab);
    tabLayout->setContentsMargins(0, 0, 0, 0);
    tabLayout->addWidget(scrollArea);
}

void SystemSettingsDialog::setupCameraTab(QWidget* tab)
{
    QVBoxLayout* layout = new QVBoxLayout(tab);
    layout->setSpacing(15);

    // ========== 相机平台配置 ==========
    cameraGroupBox_ = new QGroupBox("相机平台配置", tab);
    QFormLayout* formLayout = new QFormLayout(cameraGroupBox_);
    formLayout->setSpacing(10);

    cameraPlatformTypeCombo_ = new QComboBox(this);
    cameraPlatformTypeCombo_->addItem("独立固定安装", static_cast<int>(Platform::CameraPlatformType::SeparateFix));
    cameraPlatformTypeCombo_->addItem("安装在独立X轴", static_cast<int>(Platform::CameraPlatformType::SeparateX));
    cameraPlatformTypeCombo_->addItem("安装在独立XY轴", static_cast<int>(Platform::CameraPlatformType::SeparateXY));
    cameraPlatformTypeCombo_->addItem("共享平台X轴", static_cast<int>(Platform::CameraPlatformType::ShareX));
    formLayout->addRow("安装方式:", cameraPlatformTypeCombo_);

    layout->addWidget(cameraGroupBox_);

    // ========== 相机1方向 ==========
    QGroupBox* cam1Box = new QGroupBox("相机1 方向配置", tab);
    QFormLayout* cam1Layout = new QFormLayout(cam1Box);

    cam1XDirectionCombo_ = new QComboBox(this);
    cam1XDirectionCombo_->addItem("正向 (+)", static_cast<int>(Platform::AxisDirectionType::Positive));
    cam1XDirectionCombo_->addItem("负向 (-)", static_cast<int>(Platform::AxisDirectionType::Negative));
    cam1Layout->addRow("X方向:", cam1XDirectionCombo_);

    cam1YDirectionCombo_ = new QComboBox(this);
    cam1YDirectionCombo_->addItem("正向 (+)", static_cast<int>(Platform::AxisDirectionType::Positive));
    cam1YDirectionCombo_->addItem("负向 (-)", static_cast<int>(Platform::AxisDirectionType::Negative));
    cam1Layout->addRow("Y方向:", cam1YDirectionCombo_);

    layout->addWidget(cam1Box);

    // ========== 相机2方向 ==========
    QGroupBox* cam2Box = new QGroupBox("相机2 方向配置", tab);
    QFormLayout* cam2Layout = new QFormLayout(cam2Box);

    cam2XDirectionCombo_ = new QComboBox(this);
    cam2XDirectionCombo_->addItem("正向 (+)", static_cast<int>(Platform::AxisDirectionType::Positive));
    cam2XDirectionCombo_->addItem("负向 (-)", static_cast<int>(Platform::AxisDirectionType::Negative));
    cam2Layout->addRow("X方向:", cam2XDirectionCombo_);

    cam2YDirectionCombo_ = new QComboBox(this);
    cam2YDirectionCombo_->addItem("正向 (+)", static_cast<int>(Platform::AxisDirectionType::Positive));
    cam2YDirectionCombo_->addItem("负向 (-)", static_cast<int>(Platform::AxisDirectionType::Negative));
    cam2Layout->addRow("Y方向:", cam2YDirectionCombo_);

    layout->addWidget(cam2Box);

    layout->addStretch();
}

void SystemSettingsDialog::setupGPUTab(QWidget* tab)
{
    QVBoxLayout* layout = new QVBoxLayout(tab);
    layout->setSpacing(15);

    // ========== GPU加速设置组 ==========
    gpuGroupBox_ = new QGroupBox("GPU加速设置", tab);
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

    layout->addWidget(gpuGroupBox_);
    layout->addStretch();
}

void SystemSettingsDialog::loadSettings()
{
    // 加载GPU设置
    Base::GPUAccelerator& gpu = Base::GPUAccelerator::instance();
    selectedMode_ = gpu.getAccelMode();

    QAbstractButton* button = accelModeGroup_->button(static_cast<int>(selectedMode_));
    if (button) {
        button->setChecked(true);
    }

    updateGPUStatusDisplay();

    // 加载平台设置
    loadPlatformSettings();
}

void SystemSettingsDialog::loadPlatformSettings()
{
    // 安全检查：确保所有必要控件已创建
    if (!platformTypeCombo_ || !cameraNumSpin_ || !positionNumSpin_ ||
        !xRangeSpin_ || !xPulseSpin_ || !yRangeSpin_ || !yPulseSpin_ ||
        !dRangeSpin_ || !dPulseSpin_ || !xDirectionCombo_ || !yDirectionCombo_ ||
        !dDirectionCombo_ || !dDriveTypeCombo_ || !rotationLengthSpin_ ||
        !cameraPlatformTypeCombo_ || !cam1XDirectionCombo_ || !cam1YDirectionCombo_ ||
        !cam2XDirectionCombo_ || !cam2YDirectionCombo_) {
        return;
    }

    Platform::PlatformConfigManager& mgr = Platform::PlatformConfigManager::instance();
    const Platform::PlatformConfig& config = mgr.currentConfig();

    // 阻止信号，避免重复触发槽函数
    platformTypeCombo_->blockSignals(true);

    // 设置平台类型
    selectedPlatformType_ = config.type;
    int typeIndex = platformTypeCombo_->findData(static_cast<int>(config.type));
    if (typeIndex >= 0) {
        platformTypeCombo_->setCurrentIndex(typeIndex);
    }

    platformTypeCombo_->blockSignals(false);

    // 基本参数
    cameraNumSpin_->setValue(config.cameraNum);
    positionNumSpin_->setValue(config.positionNum);

    // 轴参数
    xRangeSpin_->setValue(config.xRange);
    xPulseSpin_->setValue(config.xPulsePerMM);
    yRangeSpin_->setValue(config.yRange);
    yPulseSpin_->setValue(config.yPulsePerMM);
    dRangeSpin_->setValue(config.dRange);
    dPulseSpin_->setValue(config.dPulsePerDegree);

    // 根据平台类型加载详细配置
    const Platform::PlatformInfo* info = config.getPlatformInfo();
    if (info) {
        if (auto xydInfo = dynamic_cast<const Platform::PlatformXYDInfo*>(info)) {
            xDirectionCombo_->setCurrentIndex(
                xydInfo->xDirection == Platform::AxisDirectionType::Positive ? 0 : 1);
            yDirectionCombo_->setCurrentIndex(
                xydInfo->yDirection == Platform::AxisDirectionType::Positive ? 0 : 1);
            dDirectionCombo_->setCurrentIndex(
                xydInfo->dDirection == Platform::AxisDirectionType::Positive ? 0 : 1);
            dDriveTypeCombo_->setCurrentIndex(static_cast<int>(xydInfo->dDriveType));
            rotationLengthSpin_->setValue(xydInfo->rotationLength);
        }
        else if (auto x1x2yInfo = dynamic_cast<const Platform::PlatformX1X2YInfo*>(info)) {
            xDirectionCombo_->setCurrentIndex(
                x1x2yInfo->x1Direction == Platform::AxisDirectionType::Positive ? 0 : 1);
            if (x2DirectionCombo_) {
                x2DirectionCombo_->setCurrentIndex(
                    x1x2yInfo->x2Direction == Platform::AxisDirectionType::Positive ? 0 : 1);
            }
            yDirectionCombo_->setCurrentIndex(
                x1x2yInfo->yDirection == Platform::AxisDirectionType::Positive ? 0 : 1);
        }
    }

    // 相机平台配置
    int camTypeIndex = cameraPlatformTypeCombo_->findData(
        static_cast<int>(config.cameraPlatform.getCamPlatformType()));
    if (camTypeIndex >= 0) {
        cameraPlatformTypeCombo_->setCurrentIndex(camTypeIndex);
    }

    // 相机方向
    cam1XDirectionCombo_->setCurrentIndex(
        config.cameraPlatform.getCamDirectionX(0) == Platform::AxisDirectionType::Positive ? 0 : 1);
    cam1YDirectionCombo_->setCurrentIndex(
        config.cameraPlatform.getCamDirectionY(0) == Platform::AxisDirectionType::Positive ? 0 : 1);
    cam2XDirectionCombo_->setCurrentIndex(
        config.cameraPlatform.getCamDirectionX(1) == Platform::AxisDirectionType::Positive ? 0 : 1);
    cam2YDirectionCombo_->setCurrentIndex(
        config.cameraPlatform.getCamDirectionY(1) == Platform::AxisDirectionType::Positive ? 0 : 1);

    // 更新轴可见性
    updateAxisVisibility();
}

void SystemSettingsDialog::applySettings()
{
    // 应用GPU设置
    Base::GPUAccelerator& gpu = Base::GPUAccelerator::instance();
    gpu.setAccelMode(selectedMode_);

    LOG_INFO(QString("GPU加速模式已更改为: %1")
            .arg(Base::GPUAccelerator::getAccelModeName(selectedMode_)));

    updateGPUStatusDisplay();

    // 应用平台设置
    applyPlatformSettings();
}

void SystemSettingsDialog::applyPlatformSettings()
{
    Platform::PlatformConfigManager& mgr = Platform::PlatformConfigManager::instance();
    Platform::PlatformConfig& config = mgr.currentConfig();

    // 检查平台类型是否变化
    Platform::PlatformType newType = static_cast<Platform::PlatformType>(
        platformTypeCombo_->currentData().toInt());
    bool typeChanged = (newType != config.type);

    // 更新基本参数
    config.type = newType;
    config.cameraNum = cameraNumSpin_->value();
    config.positionNum = positionNumSpin_->value();

    // 更新轴参数
    config.xRange = xRangeSpin_->value();
    config.xPulsePerMM = xPulseSpin_->value();
    config.yRange = yRangeSpin_->value();
    config.yPulsePerMM = yPulseSpin_->value();
    config.dRange = dRangeSpin_->value();
    config.dPulsePerDegree = dPulseSpin_->value();

    // 如果类型变化，创建新的详细配置
    if (typeChanged) {
        config.createDefaultInfo(newType);
    }

    // 更新详细配置
    Platform::PlatformInfo* info = const_cast<Platform::PlatformInfo*>(config.getPlatformInfo());
    if (info) {
        if (auto xydInfo = dynamic_cast<Platform::PlatformXYDInfo*>(info)) {
            xydInfo->xDirection = xDirectionCombo_->currentIndex() == 0
                ? Platform::AxisDirectionType::Positive : Platform::AxisDirectionType::Negative;
            xydInfo->yDirection = yDirectionCombo_->currentIndex() == 0
                ? Platform::AxisDirectionType::Positive : Platform::AxisDirectionType::Negative;
            xydInfo->dDirection = dDirectionCombo_->currentIndex() == 0
                ? Platform::AxisDirectionType::Positive : Platform::AxisDirectionType::Negative;
            xydInfo->dDriveType = static_cast<Platform::DDriveType>(dDriveTypeCombo_->currentIndex());
            xydInfo->rotationLength = rotationLengthSpin_->value();
        }
        else if (auto x1x2yInfo = dynamic_cast<Platform::PlatformX1X2YInfo*>(info)) {
            x1x2yInfo->x1Direction = xDirectionCombo_->currentIndex() == 0
                ? Platform::AxisDirectionType::Positive : Platform::AxisDirectionType::Negative;
            x1x2yInfo->x2Direction = x2DirectionCombo_->currentIndex() == 0
                ? Platform::AxisDirectionType::Positive : Platform::AxisDirectionType::Negative;
            x1x2yInfo->yDirection = yDirectionCombo_->currentIndex() == 0
                ? Platform::AxisDirectionType::Positive : Platform::AxisDirectionType::Negative;
        }
    }

    // 更新相机平台配置
    Platform::CameraPlatformType camType = static_cast<Platform::CameraPlatformType>(
        cameraPlatformTypeCombo_->currentData().toInt());
    config.cameraPlatform.setCameraNumAndType(config.cameraNum, camType);

    config.cameraPlatform.setCamDirectionX(0, cam1XDirectionCombo_->currentIndex() == 0
        ? Platform::AxisDirectionType::Positive : Platform::AxisDirectionType::Negative);
    config.cameraPlatform.setCamDirectionY(0, cam1YDirectionCombo_->currentIndex() == 0
        ? Platform::AxisDirectionType::Positive : Platform::AxisDirectionType::Negative);
    config.cameraPlatform.setCamDirectionX(1, cam2XDirectionCombo_->currentIndex() == 0
        ? Platform::AxisDirectionType::Positive : Platform::AxisDirectionType::Negative);
    config.cameraPlatform.setCamDirectionY(1, cam2YDirectionCombo_->currentIndex() == 0
        ? Platform::AxisDirectionType::Positive : Platform::AxisDirectionType::Negative);

    LOG_INFO(QString("平台配置已更新: 类型=%1, 相机数=%2")
        .arg(Platform::getPlatformTypeName(config.type))
        .arg(config.cameraNum));

    if (typeChanged) {
        QMessageBox::information(this, "提示",
            QString("平台类型已更改为: %1\n\n注意：切换平台类型后需要重新进行标定！")
                .arg(Platform::getPlatformTypeName(newType)));
    }
}

void SystemSettingsDialog::updateGPUStatusDisplay()
{
    Base::GPUAccelerator& gpu = Base::GPUAccelerator::instance();

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

    infoText += QString("\n\n当前模式: %1")
        .arg(Base::GPUAccelerator::getAccelModeName(selectedMode_));

    if (gpu.isUsingGPU()) {
        infoText += " (实际使用: GPU)";
    } else {
        infoText += " (实际使用: CPU)";
    }

    gpuInfoLabel_->setText(infoText);

    radioCUDA_->setEnabled(gpu.isCudaAvailable());
    if (!gpu.isCudaAvailable()) {
        radioCUDA_->setText("启用CUDA GPU加速 (不可用)");
    } else {
        radioCUDA_->setText("启用CUDA GPU加速");
    }
}

void SystemSettingsDialog::updateAxisVisibility()
{
    // 安全检查：确保控件已创建
    if (!x2AxisWidget_ || !y2AxisWidget_ || !dAxisWidget_ ||
        !platformTypeCombo_ || !platformDescLabel_) {
        return;
    }

    Platform::PlatformType type = static_cast<Platform::PlatformType>(
        platformTypeCombo_->currentData().toInt());

    // X2轴仅对龙门结构可见
    bool showX2 = Platform::platformIsDualX(type);
    x2AxisWidget_->setVisible(showX2);

    // Y2轴仅对双Y结构可见
    bool showY2 = Platform::platformIsDualY(type);
    y2AxisWidget_->setVisible(showY2);

    // D轴对有旋转的平台可见
    bool showD = Platform::platformHasRotation(type);
    dAxisWidget_->setVisible(showD);

    // 更新描述
    QString desc;
    switch (type) {
        case Platform::PlatformType::XYD:
            desc = "标准三轴对位平台，包含X轴、Y轴和旋转D轴，适用于大多数对位场景。";
            break;
        case Platform::PlatformType::X1X2Y:
            desc = "龙门双X轴结构，双X轴+Y轴，适用于大幅面高精度对位。";
            break;
        case Platform::PlatformType::XY1Y2:
            desc = "双Y平台结构，X轴+双Y轴，适用于特殊结构需求。";
            break;
        case Platform::PlatformType::XY:
            desc = "简化双轴平台，仅X轴+Y轴，无旋转功能。";
            break;
        case Platform::PlatformType::DXY:
            desc = "旋转优先型平台，先进行D轴旋转再进行XY平移。";
            break;
        case Platform::PlatformType::UVW:
            desc = "UVW精密对位平台，用于高精度微调。";
            break;
        default:
            desc = Platform::getPlatformTypeName(type);
            break;
    }
    platformDescLabel_->setText(desc);
}

void SystemSettingsDialog::onPlatformTypeChanged(int index)
{
    Q_UNUSED(index);
    selectedPlatformType_ = static_cast<Platform::PlatformType>(
        platformTypeCombo_->currentData().toInt());
    updateAxisVisibility();
}

void SystemSettingsDialog::onAxisDirectionChanged()
{
    // 可用于实时预览方向变化
}

void SystemSettingsDialog::onAccelModeChanged(int id)
{
    selectedMode_ = static_cast<Base::GPUAccelMode>(id);

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
