/**
 * @file CameraConfigDialog.cpp
 * @brief 相机配置对话框实现
 * @author VisionForge Team
 * @date 2025-12-17
 */

#include "ui/CameraConfigDialog.h"
#include "hal/CameraFactory.h"
#include "base/Logger.h"
#include "base/ConfigManager.h"

#ifdef USE_HIKVISION_MVS
#include "hal/HikvisionCamera.h"
#endif

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QSlider>
#include <QTableWidget>
#include <QHeaderView>
#include <QCheckBox>
#include <QMessageBox>
#include <QDateTime>
#include <opencv2/imgproc.hpp>

namespace VisionForge {

CameraConfigDialog::CameraConfigDialog(QWidget* parent)
    : QDialog(parent)
    , camera_(nullptr)
    , ownsCamera_(false)
    , previewTimer_(new QTimer(this))
    , frameCount_(0)
    , lastFpsTime_(0)
    , rotationAngle_(0)
    , flipHorizontal_(false)
    , flipVertical_(false)
{
    setWindowTitle("相机配置");
    setMinimumSize(900, 700);

    setupUI();

    connect(previewTimer_, &QTimer::timeout, this, &CameraConfigDialog::onPreviewTimer);

    // 初始化设备列表
    updateDeviceList();
}

CameraConfigDialog::~CameraConfigDialog()
{
    if (previewTimer_->isActive()) {
        previewTimer_->stop();
    }

    if (camera_ && ownsCamera_) {
        camera_->close();
        delete camera_;
    }
}

void CameraConfigDialog::setupUI()
{
    auto* mainLayout = new QHBoxLayout(this);

    // 左侧：设备列表和参数
    auto* leftWidget = new QWidget();
    auto* leftLayout = new QVBoxLayout(leftWidget);
    leftLayout->setContentsMargins(0, 0, 0, 0);

    createDeviceListGroup();
    createParameterGroup();
    createTransformGroup();

    leftLayout->addWidget(deviceGroup_);
    leftLayout->addWidget(paramGroup_);
    leftLayout->addWidget(transformGroup_);
    leftLayout->addStretch();

    // 右侧：预览和按钮
    auto* rightWidget = new QWidget();
    auto* rightLayout = new QVBoxLayout(rightWidget);
    rightLayout->setContentsMargins(0, 0, 0, 0);

    createPreviewGroup();
    createButtonGroup();

    rightLayout->addWidget(previewGroup_, 1);

    // 按钮行
    auto* btnLayout = new QHBoxLayout();
    btnLayout->addStretch();
    btnLayout->addWidget(applyBtn_);
    btnLayout->addWidget(okBtn_);
    btnLayout->addWidget(cancelBtn_);
    rightLayout->addLayout(btnLayout);

    mainLayout->addWidget(leftWidget, 1);
    mainLayout->addWidget(rightWidget, 2);

    setControlsEnabled(false);
}

void CameraConfigDialog::createDeviceListGroup()
{
    deviceGroup_ = new QGroupBox("设备列表");
    auto* layout = new QVBoxLayout(deviceGroup_);

    // 相机类型选择
    auto* typeLayout = new QHBoxLayout();
    typeLayout->addWidget(new QLabel("相机类型:"));
    cameraTypeCombo_ = new QComboBox();
    cameraTypeCombo_->addItem("全部", -1);
    cameraTypeCombo_->addItem("模拟相机", HAL::CameraFactory::Simulated);
#ifdef USE_HIKVISION_MVS
    cameraTypeCombo_->addItem("海康威视", HAL::CameraFactory::Hikvision);
#endif
#ifdef USE_BASLER_PYLON
    cameraTypeCombo_->addItem("Basler", HAL::CameraFactory::Basler);
#endif
    typeLayout->addWidget(cameraTypeCombo_);
    typeLayout->addStretch();

    refreshBtn_ = new QPushButton("刷新");
    connect(refreshBtn_, &QPushButton::clicked, this, &CameraConfigDialog::onRefreshDevices);
    typeLayout->addWidget(refreshBtn_);

    layout->addLayout(typeLayout);

    // 设备表格
    deviceTable_ = new QTableWidget();
    deviceTable_->setColumnCount(5);
    deviceTable_->setHorizontalHeaderLabels({"类型", "厂商", "型号", "序列号", "IP地址"});
    deviceTable_->horizontalHeader()->setStretchLastSection(true);
    deviceTable_->setSelectionBehavior(QAbstractItemView::SelectRows);
    deviceTable_->setSelectionMode(QAbstractItemView::SingleSelection);
    deviceTable_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    deviceTable_->verticalHeader()->setVisible(false);

    connect(deviceTable_, &QTableWidget::itemSelectionChanged,
            this, &CameraConfigDialog::onDeviceSelectionChanged);

    layout->addWidget(deviceTable_);

    // 连接按钮
    auto* connLayout = new QHBoxLayout();
    connectBtn_ = new QPushButton("连接");
    disconnectBtn_ = new QPushButton("断开");
    disconnectBtn_->setEnabled(false);

    connect(connectBtn_, &QPushButton::clicked, this, &CameraConfigDialog::onConnectCamera);
    connect(disconnectBtn_, &QPushButton::clicked, this, &CameraConfigDialog::onDisconnectCamera);

    connLayout->addWidget(connectBtn_);
    connLayout->addWidget(disconnectBtn_);
    connLayout->addStretch();
    layout->addLayout(connLayout);
}

void CameraConfigDialog::createParameterGroup()
{
    paramGroup_ = new QGroupBox("参数配置");
    auto* layout = new QGridLayout(paramGroup_);

    int row = 0;

    // 曝光时间
    layout->addWidget(new QLabel("曝光时间(μs):"), row, 0);
    exposureSpin_ = new QDoubleSpinBox();
    exposureSpin_->setRange(1, 1000000);
    exposureSpin_->setValue(10000);
    exposureSpin_->setDecimals(1);
    layout->addWidget(exposureSpin_, row, 1);

    exposureSlider_ = new QSlider(Qt::Horizontal);
    exposureSlider_->setRange(1, 100000);
    exposureSlider_->setValue(10000);
    layout->addWidget(exposureSlider_, row, 2);

    connect(exposureSpin_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &CameraConfigDialog::onExposureChanged);
    connect(exposureSlider_, &QSlider::valueChanged, [this](int value) {
        exposureSpin_->setValue(value);
    });

    row++;

    // 增益
    layout->addWidget(new QLabel("增益(dB):"), row, 0);
    gainSpin_ = new QDoubleSpinBox();
    gainSpin_->setRange(0, 48);
    gainSpin_->setValue(0);
    gainSpin_->setDecimals(2);
    layout->addWidget(gainSpin_, row, 1);

    gainSlider_ = new QSlider(Qt::Horizontal);
    gainSlider_->setRange(0, 480);
    gainSlider_->setValue(0);
    layout->addWidget(gainSlider_, row, 2);

    connect(gainSpin_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &CameraConfigDialog::onGainChanged);
    connect(gainSlider_, &QSlider::valueChanged, [this](int value) {
        gainSpin_->setValue(value / 10.0);
    });

    row++;

    // 触发模式
    layout->addWidget(new QLabel("触发模式:"), row, 0);
    triggerModeCombo_ = new QComboBox();
    triggerModeCombo_->addItem("连续采集", HAL::ICamera::Continuous);
    triggerModeCombo_->addItem("软件触发", HAL::ICamera::Software);
    triggerModeCombo_->addItem("硬件触发", HAL::ICamera::Hardware);
    layout->addWidget(triggerModeCombo_, row, 1, 1, 2);

    connect(triggerModeCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &CameraConfigDialog::onTriggerModeChanged);

    row++;

    // 分辨率
    layout->addWidget(new QLabel("宽度:"), row, 0);
    widthSpin_ = new QSpinBox();
    widthSpin_->setRange(16, 8192);
    widthSpin_->setValue(1920);
    layout->addWidget(widthSpin_, row, 1);

    layout->addWidget(new QLabel("高度:"), row + 1, 0);
    heightSpin_ = new QSpinBox();
    heightSpin_->setRange(16, 8192);
    heightSpin_->setValue(1080);
    layout->addWidget(heightSpin_, row + 1, 1);

    row += 2;

    // 偏移
    layout->addWidget(new QLabel("X偏移:"), row, 0);
    offsetXSpin_ = new QSpinBox();
    offsetXSpin_->setRange(0, 8192);
    offsetXSpin_->setValue(0);
    layout->addWidget(offsetXSpin_, row, 1);

    layout->addWidget(new QLabel("Y偏移:"), row + 1, 0);
    offsetYSpin_ = new QSpinBox();
    offsetYSpin_->setRange(0, 8192);
    offsetYSpin_->setValue(0);
    layout->addWidget(offsetYSpin_, row + 1, 1);
}

void CameraConfigDialog::createTransformGroup()
{
    transformGroup_ = new QGroupBox("图像变换");
    auto* layout = new QGridLayout(transformGroup_);

    int row = 0;

    // 旋转
    layout->addWidget(new QLabel("旋转:"), row, 0);
    rotationCombo_ = new QComboBox();
    rotationCombo_->addItem("0°（不旋转）", 0);
    rotationCombo_->addItem("90°（顺时针）", 90);
    rotationCombo_->addItem("180°", 180);
    rotationCombo_->addItem("270°（逆时针）", 270);
    layout->addWidget(rotationCombo_, row, 1, 1, 2);

    connect(rotationCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int index) {
        rotationAngle_ = rotationCombo_->itemData(index).toInt();
        LOG_DEBUG(QString("设置旋转角度: %1°").arg(rotationAngle_));
    });

    row++;

    // 水平镜像
    flipHorizontalCheck_ = new QCheckBox("水平镜像（左右翻转）");
    layout->addWidget(flipHorizontalCheck_, row, 0, 1, 3);

    connect(flipHorizontalCheck_, &QCheckBox::toggled, [this](bool checked) {
        flipHorizontal_ = checked;
        LOG_DEBUG(QString("水平镜像: %1").arg(checked ? "开" : "关"));
    });

    row++;

    // 垂直镜像
    flipVerticalCheck_ = new QCheckBox("垂直镜像（上下翻转）");
    layout->addWidget(flipVerticalCheck_, row, 0, 1, 3);

    connect(flipVerticalCheck_, &QCheckBox::toggled, [this](bool checked) {
        flipVertical_ = checked;
        LOG_DEBUG(QString("垂直镜像: %1").arg(checked ? "开" : "关"));
    });

    row++;

    // 提示
    auto* hintLabel = new QLabel("提示: 变换顺序为 旋转 → 镜像");
    hintLabel->setStyleSheet("color: gray; font-size: 10px;");
    layout->addWidget(hintLabel, row, 0, 1, 3);
}

void CameraConfigDialog::createPreviewGroup()
{
    previewGroup_ = new QGroupBox("预览");
    auto* layout = new QVBoxLayout(previewGroup_);

    // 预览图像
    previewLabel_ = new QLabel();
    previewLabel_->setMinimumSize(640, 480);
    previewLabel_->setAlignment(Qt::AlignCenter);
    previewLabel_->setStyleSheet("QLabel { background-color: #2a2a2a; border: 1px solid #555; }");
    previewLabel_->setText("未连接相机");
    layout->addWidget(previewLabel_, 1);

    // 预览控制
    auto* ctrlLayout = new QHBoxLayout();

    startPreviewBtn_ = new QPushButton("开始预览");
    stopPreviewBtn_ = new QPushButton("停止预览");
    stopPreviewBtn_->setEnabled(false);

    connect(startPreviewBtn_, &QPushButton::clicked, this, &CameraConfigDialog::onStartPreview);
    connect(stopPreviewBtn_, &QPushButton::clicked, this, &CameraConfigDialog::onStopPreview);

    ctrlLayout->addWidget(startPreviewBtn_);
    ctrlLayout->addWidget(stopPreviewBtn_);
    ctrlLayout->addStretch();

    fpsLabel_ = new QLabel("FPS: --");
    ctrlLayout->addWidget(fpsLabel_);

    layout->addLayout(ctrlLayout);
}

void CameraConfigDialog::createButtonGroup()
{
    applyBtn_ = new QPushButton("应用");
    okBtn_ = new QPushButton("确定");
    cancelBtn_ = new QPushButton("取消");

    connect(applyBtn_, &QPushButton::clicked, this, &CameraConfigDialog::onApplySettings);
    connect(okBtn_, &QPushButton::clicked, [this]() {
        onApplySettings();
        accept();
    });
    connect(cancelBtn_, &QPushButton::clicked, this, &QDialog::reject);
}

void CameraConfigDialog::updateDeviceList()
{
    deviceTable_->setRowCount(0);
    deviceList_.clear();

    int filterType = cameraTypeCombo_->currentData().toInt();

    if (filterType == -1) {
        // 枚举所有类型
        deviceList_ = HAL::CameraFactory::enumerateAllDevices();
    } else {
        // 枚举指定类型
        deviceList_ = HAL::CameraFactory::enumerateDevices(
            static_cast<HAL::CameraFactory::CameraType>(filterType));
    }

    for (int i = 0; i < deviceList_.size(); i++) {
        const auto& info = deviceList_[i];

        deviceTable_->insertRow(i);
        deviceTable_->setItem(i, 0, new QTableWidgetItem(info.interfaceType));
        deviceTable_->setItem(i, 1, new QTableWidgetItem(info.manufacturer));
        deviceTable_->setItem(i, 2, new QTableWidgetItem(info.modelName));
        deviceTable_->setItem(i, 3, new QTableWidgetItem(info.serialNumber));
        deviceTable_->setItem(i, 4, new QTableWidgetItem(info.ipAddress));
    }

    deviceTable_->resizeColumnsToContents();

    LOG_INFO(QString("发现 %1 个相机设备").arg(deviceList_.size()));
}

void CameraConfigDialog::onRefreshDevices()
{
    QString debugInfo;

    // 显示枚举过程中的信息
#ifdef USE_HIKVISION_MVS
    QString sdkVer = HAL::HikvisionCamera::sdkVersion();
    debugInfo += QString("海康SDK: %1\n").arg(sdkVer);
    LOG_INFO(QString("海康SDK版本: %1").arg(sdkVer));
#else
    debugInfo += "海康SDK: 未编译\n";
#endif

#ifdef USE_BASLER_PYLON
    debugInfo += "Basler SDK: 已启用\n";
#else
    debugInfo += "Basler SDK: 未编译\n";
#endif

    updateDeviceList();

    // 统计设备数量
    int hikCount = 0, baslerCount = 0, simCount = 0;
    for (const auto& dev : deviceList_) {
        if (dev.cameraType == HAL::CameraFactory::Hikvision) hikCount++;
        else if (dev.cameraType == HAL::CameraFactory::Basler) baslerCount++;
        else if (dev.cameraType == HAL::CameraFactory::Simulated) simCount++;
    }

    debugInfo += QString("\n发现设备:\n");
    debugInfo += QString("- 模拟相机: %1\n").arg(simCount);
    debugInfo += QString("- 海康相机: %1\n").arg(hikCount);
    debugInfo += QString("- Basler相机: %1\n").arg(baslerCount);

    // 显示结果
    if (hikCount == 0 && baslerCount == 0) {
        QMessageBox::information(this, "设备枚举结果", debugInfo +
            QString("\n提示：\n"
                    "1. 确保相机已连接并通电\n"
                    "2. 关闭海康MVS客户端后重试\n"
                    "3. 检查网络/USB连接"));
    } else {
        QMessageBox::information(this, "设备枚举结果", debugInfo);
    }
}

void CameraConfigDialog::onDeviceSelectionChanged()
{
    int row = deviceTable_->currentRow();
    connectBtn_->setEnabled(row >= 0);
}

void CameraConfigDialog::onConnectCamera()
{
    int row = deviceTable_->currentRow();
    if (row < 0 || row >= deviceList_.size()) {
        QMessageBox::warning(this, "警告", "请先选择一个相机设备");
        return;
    }

    // 断开当前相机
    if (camera_) {
        onDisconnectCamera();
    }

    const auto& info = deviceList_[row];

    // 创建相机
    camera_ = HAL::CameraFactory::create(
        static_cast<HAL::CameraFactory::CameraType>(info.cameraType), this);

    if (!camera_) {
        QMessageBox::critical(this, "错误", "创建相机实例失败");
        return;
    }

    ownsCamera_ = true;

    // 连接错误信号
    connect(camera_, &HAL::ICamera::errorOccurred,
            this, &CameraConfigDialog::onCameraError);

    // 如果是海康相机，选择设备
#ifdef USE_HIKVISION_MVS
    if (info.cameraType == HAL::CameraFactory::Hikvision) {
        auto* hikCamera = qobject_cast<HAL::HikvisionCamera*>(camera_);
        if (hikCamera) {
            if (!info.serialNumber.isEmpty()) {
                hikCamera->selectBySerialNumber(info.serialNumber);
            } else if (!info.ipAddress.isEmpty()) {
                hikCamera->selectByIP(info.ipAddress);
            } else {
                hikCamera->selectDevice(row);
            }
        }
    }
#endif

    // 打开相机
    if (!camera_->open()) {
        QMessageBox::critical(this, "错误", "打开相机失败");
        delete camera_;
        camera_ = nullptr;
        ownsCamera_ = false;
        return;
    }

    // 更新UI状态
    connectBtn_->setEnabled(false);
    disconnectBtn_->setEnabled(true);
    setControlsEnabled(true);
    updateParameterRanges();

    // 更新参数显示
    HAL::ICamera::Config config = camera_->getConfig();
    exposureSpin_->setValue(config.exposure);
    gainSpin_->setValue(config.gain);
    widthSpin_->setValue(config.width);
    heightSpin_->setValue(config.height);
    offsetXSpin_->setValue(config.offsetX);
    offsetYSpin_->setValue(config.offsetY);

    int triggerIndex = triggerModeCombo_->findData(config.triggerMode);
    if (triggerIndex >= 0) {
        triggerModeCombo_->setCurrentIndex(triggerIndex);
    }

    previewLabel_->setText("已连接: " + info.modelName);
    LOG_INFO(QString("已连接相机: %1 (%2)").arg(info.modelName).arg(info.serialNumber));

    // 保存相机配置到配置文件，以便下次自动连接
    Base::ConfigManager& configMgr = Base::ConfigManager::instance();
    configMgr.setValue("Camera/Type", info.cameraType);
    configMgr.setValue("Camera/SerialNumber", info.serialNumber);
    configMgr.setValue("Camera/IPAddress", info.ipAddress);
    configMgr.setValue("Camera/ModelName", info.modelName);
    configMgr.setValue("Camera/Manufacturer", info.manufacturer);
    configMgr.save();
    LOG_INFO("相机配置已保存");
}

void CameraConfigDialog::onDisconnectCamera()
{
    if (previewTimer_->isActive()) {
        onStopPreview();
    }

    if (camera_) {
        camera_->close();
        if (ownsCamera_) {
            delete camera_;
        }
        camera_ = nullptr;
        ownsCamera_ = false;
    }

    connectBtn_->setEnabled(true);
    disconnectBtn_->setEnabled(false);
    setControlsEnabled(false);
    previewLabel_->setText("未连接相机");

    LOG_INFO("已断开相机");
}

void CameraConfigDialog::onStartPreview()
{
    if (!camera_ || !camera_->isOpen()) {
        QMessageBox::warning(this, "警告", "请先连接相机");
        return;
    }

    // 设置为连续模式或软件触发模式
    auto mode = static_cast<HAL::ICamera::TriggerMode>(
        triggerModeCombo_->currentData().toInt());

    camera_->setTriggerMode(mode);

    if (!camera_->startGrabbing()) {
        QMessageBox::critical(this, "错误", "启动采集失败");
        return;
    }

    frameCount_ = 0;
    lastFpsTime_ = QDateTime::currentMSecsSinceEpoch();

    // 根据触发模式设置定时器间隔
    int interval = (mode == HAL::ICamera::Continuous) ? 33 : 100;
    previewTimer_->start(interval);

    startPreviewBtn_->setEnabled(false);
    stopPreviewBtn_->setEnabled(true);

    LOG_INFO("开始预览");
}

void CameraConfigDialog::onStopPreview()
{
    previewTimer_->stop();

    if (camera_ && camera_->isGrabbing()) {
        camera_->stopGrabbing();
    }

    startPreviewBtn_->setEnabled(true);
    stopPreviewBtn_->setEnabled(false);
    fpsLabel_->setText("FPS: --");

    LOG_INFO("停止预览");
}

void CameraConfigDialog::onPreviewTimer()
{
    if (!camera_ || !camera_->isOpen()) return;

    // 软件触发模式需要发送触发信号
    auto mode = camera_->getTriggerMode();
    if (mode == HAL::ICamera::Software) {
        camera_->trigger();
    }

    // 获取图像
    auto image = camera_->grabImage(100);
    if (image) {
        updatePreviewImage(image);
        frameCount_++;

        // 计算FPS
        qint64 now = QDateTime::currentMSecsSinceEpoch();
        if (now - lastFpsTime_ >= 1000) {
            double fps = frameCount_ * 1000.0 / (now - lastFpsTime_);
            fpsLabel_->setText(QString("FPS: %1").arg(fps, 0, 'f', 1));
            frameCount_ = 0;
            lastFpsTime_ = now;
        }
    }
}

void CameraConfigDialog::updatePreviewImage(Base::ImageData::Ptr image)
{
    if (!image) return;

    // 获取原始Mat
    cv::Mat mat = image->mat();
    if (mat.empty()) return;

    // 应用变换
    cv::Mat transformed = applyTransform(mat);

    // 转换为QImage
    QImage qimg;
    if (transformed.channels() == 3) {
        cv::Mat rgb;
        cv::cvtColor(transformed, rgb, cv::COLOR_BGR2RGB);
        qimg = QImage(rgb.data, rgb.cols, rgb.rows, rgb.step, QImage::Format_RGB888).copy();
    } else if (transformed.channels() == 1) {
        qimg = QImage(transformed.data, transformed.cols, transformed.rows, transformed.step, QImage::Format_Grayscale8).copy();
    } else {
        qimg = image->toQImage();
    }

    if (qimg.isNull()) return;

    // 缩放适应预览区域
    QSize labelSize = previewLabel_->size();
    QPixmap pixmap = QPixmap::fromImage(qimg).scaled(
        labelSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    previewLabel_->setPixmap(pixmap);
}

cv::Mat CameraConfigDialog::applyTransform(const cv::Mat& src)
{
    if (src.empty()) return src;

    cv::Mat result = src.clone();

    // 1. 旋转
    if (rotationAngle_ != 0) {
        int rotateCode = -1;
        switch (rotationAngle_) {
            case 90:
                rotateCode = cv::ROTATE_90_CLOCKWISE;
                break;
            case 180:
                rotateCode = cv::ROTATE_180;
                break;
            case 270:
                rotateCode = cv::ROTATE_90_COUNTERCLOCKWISE;
                break;
        }
        if (rotateCode >= 0) {
            cv::Mat rotated;
            cv::rotate(result, rotated, rotateCode);
            result = rotated;
        }
    }

    // 2. 镜像
    if (flipHorizontal_ && flipVertical_) {
        // 同时水平和垂直翻转 = 旋转180°
        cv::Mat flipped;
        cv::flip(result, flipped, -1);
        result = flipped;
    } else if (flipHorizontal_) {
        // 水平翻转（沿Y轴）
        cv::Mat flipped;
        cv::flip(result, flipped, 1);
        result = flipped;
    } else if (flipVertical_) {
        // 垂直翻转（沿X轴）
        cv::Mat flipped;
        cv::flip(result, flipped, 0);
        result = flipped;
    }

    return result;
}

void CameraConfigDialog::onExposureChanged(double value)
{
    exposureSlider_->blockSignals(true);
    exposureSlider_->setValue(static_cast<int>(value));
    exposureSlider_->blockSignals(false);

    if (camera_ && camera_->isOpen()) {
        camera_->setExposure(value);
    }
}

void CameraConfigDialog::onGainChanged(double value)
{
    gainSlider_->blockSignals(true);
    gainSlider_->setValue(static_cast<int>(value * 10));
    gainSlider_->blockSignals(false);

    if (camera_ && camera_->isOpen()) {
        camera_->setGain(value);
    }
}

void CameraConfigDialog::onTriggerModeChanged(int index)
{
    if (!camera_ || !camera_->isOpen()) return;

    auto mode = static_cast<HAL::ICamera::TriggerMode>(
        triggerModeCombo_->itemData(index).toInt());
    camera_->setTriggerMode(mode);
}

void CameraConfigDialog::onApplySettings()
{
    if (!camera_ || !camera_->isOpen()) return;

    HAL::ICamera::Config config;
    config.width = widthSpin_->value();
    config.height = heightSpin_->value();
    config.offsetX = offsetXSpin_->value();
    config.offsetY = offsetYSpin_->value();
    config.exposure = exposureSpin_->value();
    config.gain = gainSpin_->value();
    config.triggerMode = static_cast<HAL::ICamera::TriggerMode>(
        triggerModeCombo_->currentData().toInt());

    // 图像变换设置
    config.rotationAngle = rotationAngle_;
    config.flipHorizontal = flipHorizontal_;
    config.flipVertical = flipVertical_;

    camera_->setConfig(config);

    LOG_INFO("相机参数已应用");
}

void CameraConfigDialog::onCameraError(const QString& error)
{
    LOG_ERROR(QString("相机错误: %1").arg(error));
    QMessageBox::warning(this, "相机错误", error);
}

void CameraConfigDialog::updateParameterRanges()
{
    if (!camera_ || !camera_->isOpen()) return;

#ifdef USE_HIKVISION_MVS
    auto* hikCamera = qobject_cast<HAL::HikvisionCamera*>(camera_);
    if (hikCamera) {
        // 曝光范围
        double expMin, expMax;
        hikCamera->getExposureRange(expMin, expMax);
        exposureSpin_->setRange(expMin, expMax);
        exposureSlider_->setRange(static_cast<int>(expMin), static_cast<int>(expMax));

        // 增益范围
        double gainMin, gainMax;
        hikCamera->getGainRange(gainMin, gainMax);
        gainSpin_->setRange(gainMin, gainMax);
        gainSlider_->setRange(static_cast<int>(gainMin * 10), static_cast<int>(gainMax * 10));

        // 分辨率范围
        int wMin, wMax, hMin, hMax;
        hikCamera->getWidthRange(wMin, wMax);
        hikCamera->getHeightRange(hMin, hMax);
        widthSpin_->setRange(wMin, wMax);
        heightSpin_->setRange(hMin, hMax);
    }
#endif
}

void CameraConfigDialog::setControlsEnabled(bool enabled)
{
    paramGroup_->setEnabled(enabled);
    startPreviewBtn_->setEnabled(enabled);
    applyBtn_->setEnabled(enabled);
}

HAL::ICamera* CameraConfigDialog::takeCamera()
{
    HAL::ICamera* cam = camera_;
    if (cam) {
        // 移除父对象关系，防止对话框销毁时连带删除相机
        cam->setParent(nullptr);
    }
    camera_ = nullptr;
    ownsCamera_ = false;
    return cam;
}

void CameraConfigDialog::setCamera(HAL::ICamera* camera)
{
    if (camera_) {
        onDisconnectCamera();
    }

    camera_ = camera;
    ownsCamera_ = false;

    if (camera_ && camera_->isOpen()) {
        connectBtn_->setEnabled(false);
        disconnectBtn_->setEnabled(true);
        setControlsEnabled(true);
        updateParameterRanges();

        HAL::ICamera::Config config = camera_->getConfig();
        exposureSpin_->setValue(config.exposure);
        gainSpin_->setValue(config.gain);
        widthSpin_->setValue(config.width);
        heightSpin_->setValue(config.height);

        previewLabel_->setText("已连接: " + camera_->deviceName());
    }
}

} // namespace VisionForge
