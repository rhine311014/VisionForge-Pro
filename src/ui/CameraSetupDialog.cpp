/**
 * @file CameraSetupDialog.cpp
 * @brief 相机设置对话框实现
 */

#include "ui/CameraSetupDialog.h"
#include "core/MultiStationManager.h"
#include "hal/CameraManager.h"
#include "hal/CameraFactory.h"
#include "hal/ICamera.h"
#include "base/Logger.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QFile>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QStandardPaths>
#include <QApplication>

#include <opencv2/imgproc.hpp>

namespace VisionForge {
namespace UI {

CameraSetupDialog::CameraSetupDialog(QWidget* parent)
    : QDialog(parent)
    , previewTimer_(new QTimer(this))
{
    setWindowTitle(tr("相机设置"));
    setMinimumSize(1200, 850);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    setupUI();
    loadExistingConfig();
    refreshCameraList();

    // 连接预览定时器
    connect(previewTimer_, &QTimer::timeout, this, &CameraSetupDialog::onCaptureTimerTimeout);
}

CameraSetupDialog::~CameraSetupDialog()
{
    // 确保停止连续采集
    if (isCapturing_) {
        stopCapture();
    } else if (previewTimer_->isActive()) {
        previewTimer_->stop();
    }
}

bool CameraSetupDialog::hasExistingConfig()
{
    return QFile::exists(configFilePath());
}

QString CameraSetupDialog::configFilePath()
{
    return QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation)
           + "/VisionForge Pro/camera_setup.json";
}

void CameraSetupDialog::setupUI()
{
    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(10);

    // 左侧主内容区
    QVBoxLayout* contentLayout = new QVBoxLayout();
    contentLayout->setSpacing(15);

    // 创建各个组
    createCameraTestGroup();
    createChannelSettingsGroup();
    createPositionSettingsGroup();

    contentLayout->addWidget(cameraTestGroup_);

    // 下半部分两列布局
    QHBoxLayout* bottomLayout = new QHBoxLayout();
    bottomLayout->addWidget(channelSettingsGroup_);
    bottomLayout->addWidget(positionSettingsGroup_);
    contentLayout->addLayout(bottomLayout);

    // 状态栏
    statusLabel_ = new QLabel(tr("枚举相机个数: 0"));
    statusLabel_->setStyleSheet("QLabel { color: #00AA00; padding: 5px; }");
    contentLayout->addWidget(statusLabel_);

    mainLayout->addLayout(contentLayout, 1);

    // 右侧按钮区
    createSideButtons();
    QVBoxLayout* sideLayout = new QVBoxLayout();
    sideLayout->addWidget(stationButton_);
    sideLayout->addStretch();
    sideLayout->addWidget(confirmButton_);
    sideLayout->addWidget(skipButton_);
    sideLayout->addWidget(cancelButton_);

    mainLayout->addLayout(sideLayout);
}

void CameraSetupDialog::createCameraTestGroup()
{
    cameraTestGroup_ = new QGroupBox(tr("相机采集测试"), this);
    QHBoxLayout* layout = new QHBoxLayout(cameraTestGroup_);

    // 预览区域
    previewLabel_ = new QLabel(this);
    previewLabel_->setMinimumSize(450, 300);
    previewLabel_->setMaximumSize(450, 300);
    previewLabel_->setStyleSheet("QLabel { background-color: #001428; border: 1px solid #555; }");
    previewLabel_->setAlignment(Qt::AlignCenter);
    layout->addWidget(previewLabel_);

    // 右侧控制区域
    QVBoxLayout* controlLayout = new QVBoxLayout();
    controlLayout->setSpacing(15);

    // 相机ID选择
    QHBoxLayout* cameraLayout = new QHBoxLayout();
    cameraLayout->addWidget(new QLabel(tr("相机ID:"), this));
    cameraIdCombo_ = new QComboBox(this);
    cameraIdCombo_->setMinimumWidth(200);
    cameraLayout->addWidget(cameraIdCombo_);
    captureButton_ = new QPushButton(tr("采集"), this);
    captureButton_->setMinimumWidth(80);
    cameraLayout->addWidget(captureButton_);
    controlLayout->addLayout(cameraLayout);

    // 曝光滑块
    QHBoxLayout* exposureLayout = new QHBoxLayout();
    exposureLayout->addWidget(new QLabel(tr("曝光:"), this));
    exposureSlider_ = new QSlider(Qt::Horizontal, this);
    exposureSlider_->setRange(10, 100000);  // 0.01ms - 100ms
    exposureSlider_->setValue(1500);
    exposureLayout->addWidget(exposureSlider_);
    exposureValueLabel_ = new QLabel("1.50ms", this);
    exposureValueLabel_->setMinimumWidth(60);
    exposureLayout->addWidget(exposureValueLabel_);
    controlLayout->addLayout(exposureLayout);

    // 增益滑块
    QHBoxLayout* gainLayout = new QHBoxLayout();
    gainLayout->addWidget(new QLabel(tr("增益:"), this));
    gainSlider_ = new QSlider(Qt::Horizontal, this);
    gainSlider_->setRange(0, 2000);  // 0 - 20dB
    gainSlider_->setValue(0);
    gainLayout->addWidget(gainSlider_);
    gainValueLabel_ = new QLabel("0.00dB", this);
    gainValueLabel_->setMinimumWidth(60);
    gainLayout->addWidget(gainValueLabel_);
    controlLayout->addLayout(gainLayout);

    controlLayout->addStretch();
    layout->addLayout(controlLayout);

    // 连接信号
    connect(cameraIdCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &CameraSetupDialog::onCameraIdChanged);
    connect(captureButton_, &QPushButton::clicked,
            this, &CameraSetupDialog::onCaptureClicked);
    connect(exposureSlider_, &QSlider::valueChanged,
            this, &CameraSetupDialog::onExposureChanged);
    connect(gainSlider_, &QSlider::valueChanged,
            this, &CameraSetupDialog::onGainChanged);
}

void CameraSetupDialog::createChannelSettingsGroup()
{
    channelSettingsGroup_ = new QGroupBox(tr("相机通道设置"), this);
    QVBoxLayout* layout = new QVBoxLayout(channelSettingsGroup_);

    channelTable_ = new QTableWidget(this);
    channelTable_->setColumnCount(3);
    channelTable_->setHorizontalHeaderLabels({tr(""), tr("原相机ID"), tr("新相机ID")});
    channelTable_->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    channelTable_->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    channelTable_->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    channelTable_->setColumnWidth(0, 80);
    channelTable_->verticalHeader()->setVisible(false);
    channelTable_->setSelectionBehavior(QAbstractItemView::SelectRows);
    channelTable_->setEditTriggers(QAbstractItemView::NoEditTriggers);

    layout->addWidget(channelTable_);
}

void CameraSetupDialog::createPositionSettingsGroup()
{
    positionSettingsGroup_ = new QGroupBox(tr("位置信息设置"), this);
    QVBoxLayout* layout = new QVBoxLayout(positionSettingsGroup_);

    positionTable_ = new QTableWidget(this);
    positionTable_->setColumnCount(3);
    positionTable_->setHorizontalHeaderLabels({tr(""), tr("原相机通道"), tr("新相机通道")});
    positionTable_->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    positionTable_->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    positionTable_->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    positionTable_->setColumnWidth(0, 60);
    positionTable_->verticalHeader()->setVisible(false);
    positionTable_->setSelectionBehavior(QAbstractItemView::SelectRows);

    layout->addWidget(positionTable_);

    connect(positionTable_, &QTableWidget::cellChanged,
            this, &CameraSetupDialog::onPositionMappingChanged);
}

void CameraSetupDialog::createSideButtons()
{
    // 工位按钮
    stationButton_ = new QPushButton(this);
    stationButton_->setFixedSize(80, 80);
    stationButton_->setText(tr("工位1"));
    stationButton_->setStyleSheet(R"(
        QPushButton {
            background-color: #E0E0E0;
            border: 2px solid #999;
            border-radius: 5px;
            font-size: 14px;
            font-weight: bold;
        }
        QPushButton:hover {
            background-color: #D0D0D0;
        }
    )");

    // 确定按钮
    confirmButton_ = new QPushButton(this);
    confirmButton_->setFixedSize(80, 80);
    confirmButton_->setText(tr("确定"));
    confirmButton_->setStyleSheet(R"(
        QPushButton {
            background-color: #4FC3F7;
            border: none;
            border-radius: 5px;
            font-size: 16px;
            font-weight: bold;
            color: white;
        }
        QPushButton:hover {
            background-color: #29B6F6;
        }
    )");

    // 跳过按钮
    skipButton_ = new QPushButton(this);
    skipButton_->setFixedSize(80, 80);
    skipButton_->setText(tr("跳过"));
    skipButton_->setStyleSheet(R"(
        QPushButton {
            background-color: #FFA726;
            border: none;
            border-radius: 5px;
            font-size: 16px;
            font-weight: bold;
            color: white;
        }
        QPushButton:hover {
            background-color: #FF9800;
        }
    )");

    // 取消按钮
    cancelButton_ = new QPushButton(this);
    cancelButton_->setFixedSize(80, 80);
    cancelButton_->setText(tr("取消"));
    cancelButton_->setStyleSheet(R"(
        QPushButton {
            background-color: #EF5350;
            border: none;
            border-radius: 5px;
            font-size: 16px;
            font-weight: bold;
            color: white;
        }
        QPushButton:hover {
            background-color: #E53935;
        }
    )");

    connect(stationButton_, &QPushButton::clicked,
            this, &CameraSetupDialog::onStationButtonClicked);
    connect(confirmButton_, &QPushButton::clicked,
            this, &CameraSetupDialog::onConfirmClicked);
    connect(skipButton_, &QPushButton::clicked,
            this, &CameraSetupDialog::onSkipClicked);
    connect(cancelButton_, &QPushButton::clicked,
            this, &CameraSetupDialog::onCancelClicked);
}

void CameraSetupDialog::refreshCameraList()
{
    availableCameras_.clear();
    cameraIdCombo_->clear();

    // 使用 CameraFactory 枚举所有可用相机
    QList<HAL::GenericDeviceInfo> devices = HAL::CameraFactory::enumerateAllDevices();

    LOG_INFO(QString("[相机设置] 枚举到 %1 个相机").arg(devices.size()));

    for (const auto& device : devices) {
        // 使用序列号作为唯一ID
        QString camId = device.serialNumber;
        if (camId.isEmpty()) {
            // 如果没有序列号，使用IP地址或生成一个ID
            camId = device.ipAddress.isEmpty() ?
                QString("CAM_%1_%2").arg(device.manufacturer).arg(availableCameras_.size()) :
                device.ipAddress;
        }

        availableCameras_.append(camId);

        // 显示格式: 型号(序列号/IP)
        QString displayText = QString("%1-%2(%3)")
            .arg(device.manufacturer)
            .arg(device.modelName)
            .arg(device.serialNumber.isEmpty() ? device.ipAddress : device.serialNumber);
        cameraIdCombo_->addItem(displayText, camId);

        LOG_DEBUG(QString("[相机设置] 发现相机: %1, 型号: %2, 接口: %3")
                  .arg(camId).arg(device.modelName).arg(device.interfaceType));
    }

    statusLabel_->setText(tr("枚举相机个数: %1").arg(availableCameras_.size()));

    // 更新通道映射表
    updateChannelMappings();
    updatePositionMappings();
}

void CameraSetupDialog::updateChannelMappings()
{
    // 根据工位配置获取通道数
    auto& stationManager = Core::MultiStationManager::instance();
    int stationCount = stationManager.getStationCount();

    if (stationCount > 0) {
        auto* station = stationManager.getStationByIndex(currentStationIndex_);
        if (station) {
            channelCount_ = station->cameraNum;
        }
    }

    channelTable_->setRowCount(channelCount_);

    for (int i = 0; i < channelCount_; ++i) {
        // 通道名称
        QTableWidgetItem* channelItem = new QTableWidgetItem(QString("CH%1通道:").arg(i + 1));
        channelItem->setTextAlignment(Qt::AlignCenter);
        channelTable_->setItem(i, 0, channelItem);

        // 原相机ID (只读)
        QString originalId;
        if (i < channelConfigs_.size()) {
            originalId = channelConfigs_[i].originalCameraId;
        } else if (i < availableCameras_.size()) {
            originalId = availableCameras_[i];
        }
        QTableWidgetItem* originalItem = new QTableWidgetItem(originalId);
        originalItem->setFlags(originalItem->flags() & ~Qt::ItemIsEditable);
        channelTable_->setItem(i, 1, originalItem);

        // 新相机ID (下拉框)
        QComboBox* cameraCombo = new QComboBox();
        for (const QString& camId : availableCameras_) {
            cameraCombo->addItem(camId);
        }
        if (i < channelConfigs_.size() && !channelConfigs_[i].mappedCameraId.isEmpty()) {
            int idx = cameraCombo->findText(channelConfigs_[i].mappedCameraId);
            if (idx >= 0) cameraCombo->setCurrentIndex(idx);
        } else if (i < availableCameras_.size()) {
            cameraCombo->setCurrentIndex(i);
        }
        channelTable_->setCellWidget(i, 2, cameraCombo);

        connect(cameraCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
                this, &CameraSetupDialog::onChannelMappingChanged);
    }
}

void CameraSetupDialog::updatePositionMappings()
{
    // 根据工位配置获取位置数
    auto& stationManager = Core::MultiStationManager::instance();
    int stationCount = stationManager.getStationCount();

    if (stationCount > 0) {
        auto* station = stationManager.getStationByIndex(currentStationIndex_);
        if (station) {
            positionCount_ = station->positionNum;
        }
    }

    positionTable_->blockSignals(true);
    positionTable_->setRowCount(positionCount_);

    for (int i = 0; i < positionCount_; ++i) {
        // 位置名称
        QTableWidgetItem* posItem = new QTableWidgetItem(QString("位置%1:").arg(i + 1));
        posItem->setTextAlignment(Qt::AlignCenter);
        positionTable_->setItem(i, 0, posItem);

        // 原相机通道 (只读)
        QString originalChannel = QString("CH%1").arg((i % channelCount_) + 1);
        if (i < positionMappings_.size()) {
            originalChannel = positionMappings_[i].originalChannel;
        }
        QTableWidgetItem* originalItem = new QTableWidgetItem(originalChannel);
        originalItem->setFlags(originalItem->flags() & ~Qt::ItemIsEditable);
        positionTable_->setItem(i, 1, originalItem);

        // 新相机通道 (下拉框)
        QComboBox* channelCombo = new QComboBox();
        for (int c = 0; c < channelCount_; ++c) {
            channelCombo->addItem(QString("CH%1").arg(c + 1));
        }
        if (i < positionMappings_.size()) {
            QString channel = positionMappings_[i].mappedChannel;
            int idx = channelCombo->findText(channel);
            if (idx >= 0) channelCombo->setCurrentIndex(idx);
        } else {
            channelCombo->setCurrentIndex(i % channelCount_);
        }
        positionTable_->setCellWidget(i, 2, channelCombo);
    }

    positionTable_->blockSignals(false);
}

void CameraSetupDialog::onCameraIdChanged(int index)
{
    Q_UNUSED(index)
    // 切换相机时停止当前采集
    if (isCapturing_) {
        stopCapture();
    }
}

void CameraSetupDialog::onCaptureClicked()
{
    LOG_INFO(QString("[相机设置] onCaptureClicked() 被调用, isCapturing_=%1").arg(isCapturing_));

    if (isCapturing_) {
        // 正在采集，停止采集
        stopCapture();
    } else {
        // 未采集，开始采集
        startCapture();
    }
}

HAL::ICamera* CameraSetupDialog::ensureCameraReady(const QString& cameraId)
{
    if (cameraId.isEmpty()) {
        return nullptr;
    }

    auto& cameraManager = HAL::CameraManager::instance();

    // 检查相机是否已在CameraManager中
    HAL::ICamera* camera = cameraManager.getCamera(cameraId);

    if (!camera) {
        // 相机不在CameraManager中，需要先添加
        int currentIndex = cameraIdCombo_->currentIndex();
        if (currentIndex < 0) {
            return nullptr;
        }

        // 重新枚举获取设备信息
        QList<HAL::GenericDeviceInfo> devices = HAL::CameraFactory::enumerateAllDevices();
        if (currentIndex >= devices.size()) {
            return nullptr;
        }

        const HAL::GenericDeviceInfo& deviceInfo = devices[currentIndex];

        // 添加相机到CameraManager
        HAL::CameraInfo camInfo;
        camInfo.id = cameraId;
        camInfo.name = QString("%1-%2").arg(deviceInfo.manufacturer).arg(deviceInfo.modelName);
        camInfo.type = static_cast<HAL::CameraFactory::CameraType>(deviceInfo.cameraType);
        camInfo.deviceInfo = deviceInfo;
        camInfo.enabled = true;

        if (!cameraManager.addCamera(camInfo)) {
            return nullptr;
        }

        camera = cameraManager.getCamera(cameraId);
        if (!camera) {
            return nullptr;
        }

        LOG_INFO(QString("[相机设置] 已添加相机: %1").arg(cameraId));
    }

    // 确保相机已打开
    if (!camera->isOpen()) {
        if (!camera->open()) {
            return nullptr;
        }
    }

    return camera;
}

void CameraSetupDialog::startCapture()
{
    LOG_INFO("[相机设置] startCapture() 开始执行");

    QString cameraId = cameraIdCombo_->currentData().toString();
    LOG_INFO(QString("[相机设置] 获取到相机ID: '%1'").arg(cameraId));

    if (cameraId.isEmpty()) {
        QMessageBox::warning(this, tr("警告"), tr("请先选择相机"));
        return;
    }

    HAL::ICamera* camera = ensureCameraReady(cameraId);
    if (!camera) {
        QMessageBox::warning(this, tr("错误"), tr("无法打开相机: %1").arg(cameraId));
        return;
    }

    LOG_INFO(QString("[相机设置] 相机已准备好: %1").arg(cameraId));

    // 设置曝光和增益
    double exposure = exposureSlider_->value();  // 微秒
    double gain = gainSlider_->value() / 100.0;  // 转换为dB
    LOG_INFO(QString("[相机设置] 设置参数: 曝光=%1us, 增益=%2dB").arg(exposure).arg(gain));
    camera->setExposure(exposure);
    camera->setGain(gain);

    // 保存当前相机ID
    currentCameraId_ = cameraId;
    isCapturing_ = true;

    // 更新按钮文字
    LOG_INFO("[相机设置] 更新按钮为'停止采集'");
    captureButton_->setText(tr("停止采集"));
    captureButton_->setStyleSheet("QPushButton { background-color: #FF5722; color: white; }");

    // 禁用相机选择
    cameraIdCombo_->setEnabled(false);

    // 立即采集一帧
    onCaptureTimerTimeout();

    // 启动定时器，约30fps
    previewTimer_->start(33);

    LOG_INFO(QString("[相机设置] 开始连续采集: %1").arg(cameraId));
}

void CameraSetupDialog::stopCapture()
{
    // 停止定时器
    previewTimer_->stop();

    isCapturing_ = false;
    currentCameraId_.clear();

    // 更新按钮文字
    captureButton_->setText(tr("采集"));
    captureButton_->setStyleSheet("");

    // 重新启用相机选择
    cameraIdCombo_->setEnabled(true);

    LOG_INFO("[相机设置] 停止采集");
}

void CameraSetupDialog::onCaptureTimerTimeout()
{
    if (!isCapturing_ || currentCameraId_.isEmpty()) {
        return;
    }

    auto& cameraManager = HAL::CameraManager::instance();

    // 采集图像
    HAL::CaptureResult result = cameraManager.capture(currentCameraId_, 500);
    if (result.success && result.image) {
        // 获取cv::Mat
        cv::Mat image = result.image->mat();

        // 转换为QImage显示
        QImage qimg;
        if (image.channels() == 1) {
            qimg = QImage(image.data, image.cols, image.rows,
                         static_cast<int>(image.step), QImage::Format_Grayscale8).copy();
        } else {
            cv::Mat rgb;
            cv::cvtColor(image, rgb, cv::COLOR_BGR2RGB);
            qimg = QImage(rgb.data, rgb.cols, rgb.rows,
                         static_cast<int>(rgb.step), QImage::Format_RGB888).copy();
        }
        updatePreview(qimg);
    }
}

void CameraSetupDialog::onExposureChanged(int value)
{
    double exposureMs = value / 1000.0;
    exposureValueLabel_->setText(QString("%1ms").arg(exposureMs, 0, 'f', 2));

    // 只有在采集状态下才实时更新相机参数
    if (!isCapturing_ || currentCameraId_.isEmpty()) {
        return;
    }

    // 实时更新相机曝光
    auto& cameraManager = HAL::CameraManager::instance();
    HAL::ICamera* camera = cameraManager.getCamera(currentCameraId_);
    if (camera && camera->isOpen()) {
        LOG_INFO(QString("[相机设置] 设置曝光: %1 us").arg(value));
        camera->setExposure(value);  // 微秒
    } else {
        LOG_WARNING(QString("[相机设置] 相机未打开，无法设置曝光"));
    }
}

void CameraSetupDialog::onGainChanged(int value)
{
    double gainDb = value / 100.0;
    gainValueLabel_->setText(QString("%1dB").arg(gainDb, 0, 'f', 2));

    // 只有在采集状态下才实时更新相机参数
    if (!isCapturing_ || currentCameraId_.isEmpty()) {
        return;
    }

    // 实时更新相机增益
    auto& cameraManager = HAL::CameraManager::instance();
    HAL::ICamera* camera = cameraManager.getCamera(currentCameraId_);
    if (camera && camera->isOpen()) {
        LOG_INFO(QString("[相机设置] 设置增益: %1 dB").arg(gainDb, 0, 'f', 2));
        camera->setGain(gainDb);  // dB
    } else {
        LOG_WARNING(QString("[相机设置] 相机未打开，无法设置增益"));
    }
}

void CameraSetupDialog::onStationButtonClicked()
{
    auto& stationManager = Core::MultiStationManager::instance();
    int stationCount = stationManager.getStationCount();

    if (stationCount <= 1) {
        return;
    }

    // 循环切换工位
    currentStationIndex_ = (currentStationIndex_ + 1) % stationCount;
    stationButton_->setText(tr("工位%1").arg(currentStationIndex_ + 1));

    // 更新通道和位置映射
    updateChannelMappings();
    updatePositionMappings();
}

void CameraSetupDialog::onConfirmClicked()
{
    if (saveConfig()) {
        saveAndExit_ = true;
        emit configurationSaved();
        accept();
    }
}

void CameraSetupDialog::onCancelClicked()
{
    reject();
}

void CameraSetupDialog::onSkipClicked()
{
    int ret = QMessageBox::question(this, tr("确认跳过"),
        tr("跳过相机配置后，软件将使用默认设置。\n确定要跳过吗？"),
        QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

    if (ret == QMessageBox::Yes) {
        skipped_ = true;
        emit configurationSkipped();
        accept();
    }
}

void CameraSetupDialog::onChannelMappingChanged()
{
    // 收集通道映射更改
    channelConfigs_.clear();
    for (int i = 0; i < channelTable_->rowCount(); ++i) {
        CameraChannelConfig config;
        config.channelId = QString("CH%1").arg(i + 1);

        auto* originalItem = channelTable_->item(i, 1);
        if (originalItem) {
            config.originalCameraId = originalItem->text();
        }

        auto* combo = qobject_cast<QComboBox*>(channelTable_->cellWidget(i, 2));
        if (combo) {
            config.mappedCameraId = combo->currentText();
        }

        channelConfigs_.append(config);
    }
}

void CameraSetupDialog::onPositionMappingChanged(int row, int column)
{
    Q_UNUSED(row)
    Q_UNUSED(column)
    // 位置映射更改处理
}

void CameraSetupDialog::updatePreview(const QImage& image)
{
    if (image.isNull()) {
        return;
    }

    QPixmap pixmap = QPixmap::fromImage(image);
    pixmap = pixmap.scaled(previewLabel_->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    previewLabel_->setPixmap(pixmap);
}

void CameraSetupDialog::loadExistingConfig()
{
    QString path = configFilePath();
    if (!QFile::exists(path)) {
        return;
    }

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        LOG_WARNING(QString("无法读取相机配置文件: %1").arg(path));
        return;
    }

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &parseError);
    file.close();

    if (parseError.error != QJsonParseError::NoError) {
        LOG_WARNING(QString("解析相机配置文件失败: %1").arg(parseError.errorString()));
        return;
    }

    QJsonObject root = doc.object();

    // 加载通道配置
    QJsonArray channelsArray = root["channels"].toArray();
    channelConfigs_.clear();
    for (const auto& val : channelsArray) {
        QJsonObject obj = val.toObject();
        CameraChannelConfig config;
        config.channelId = obj["channelId"].toString();
        config.originalCameraId = obj["originalCameraId"].toString();
        config.mappedCameraId = obj["mappedCameraId"].toString();
        channelConfigs_.append(config);
    }

    // 加载位置映射
    QJsonArray positionsArray = root["positions"].toArray();
    positionMappings_.clear();
    for (const auto& val : positionsArray) {
        QJsonObject obj = val.toObject();
        PositionCameraMapping mapping;
        mapping.positionId = obj["positionId"].toString();
        mapping.originalChannel = obj["originalChannel"].toString();
        mapping.mappedChannel = obj["mappedChannel"].toString();
        positionMappings_.append(mapping);
    }

    LOG_INFO(QString("已加载相机配置: %1 个通道, %2 个位置")
             .arg(channelConfigs_.size()).arg(positionMappings_.size()));
}

bool CameraSetupDialog::saveConfig()
{
    // 收集当前配置
    onChannelMappingChanged();

    // 收集位置映射
    positionMappings_.clear();
    for (int i = 0; i < positionTable_->rowCount(); ++i) {
        PositionCameraMapping mapping;
        mapping.positionId = QString("Position%1").arg(i + 1);

        auto* originalItem = positionTable_->item(i, 1);
        if (originalItem) {
            mapping.originalChannel = originalItem->text();
        }

        auto* combo = qobject_cast<QComboBox*>(positionTable_->cellWidget(i, 2));
        if (combo) {
            mapping.mappedChannel = combo->currentText();
        }

        positionMappings_.append(mapping);
    }

    // 构建JSON
    QJsonObject root;

    QJsonArray channelsArray;
    for (const auto& config : channelConfigs_) {
        QJsonObject obj;
        obj["channelId"] = config.channelId;
        obj["originalCameraId"] = config.originalCameraId;
        obj["mappedCameraId"] = config.mappedCameraId;
        channelsArray.append(obj);
    }
    root["channels"] = channelsArray;

    QJsonArray positionsArray;
    for (const auto& mapping : positionMappings_) {
        QJsonObject obj;
        obj["positionId"] = mapping.positionId;
        obj["originalChannel"] = mapping.originalChannel;
        obj["mappedChannel"] = mapping.mappedChannel;
        positionsArray.append(obj);
    }
    root["positions"] = positionsArray;

    // 保存到文件
    QString path = configFilePath();
    QDir().mkpath(QFileInfo(path).absolutePath());

    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, tr("错误"), tr("无法保存配置文件: %1").arg(path));
        return false;
    }

    QJsonDocument doc(root);
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();

    LOG_INFO(QString("相机配置已保存: %1").arg(path));
    return true;
}

void CameraSetupDialog::populateCameraList()
{
    refreshCameraList();
}

} // namespace UI
} // namespace VisionForge
