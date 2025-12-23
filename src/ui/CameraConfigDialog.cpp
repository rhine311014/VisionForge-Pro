/**
 * @file CameraConfigDialog.cpp
 * @brief 相机配置对话框实现
 * @author VisionForge Team
 * @date 2025-12-17
 */

#include "ui/CameraConfigDialog.h"
#include "hal/CameraFactory.h"
#include "base/Logger.h"

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
#include <QToolButton>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QSlider>
#include <QCheckBox>
#include <QRadioButton>
#include <QButtonGroup>
#include <QMessageBox>
#include <QDateTime>
#include <opencv2/imgproc.hpp>

namespace VisionForge {

CameraConfigDialog::CameraConfigDialog(QWidget* parent)
    : QDialog(parent)
    // 预览区域
    , previewLabel_(nullptr)
    // 位置选择
    , positionGroup_(nullptr)
    // 参数设置
    , paramGroup_(nullptr)
    , exposureSlider_(nullptr)
    , exposureSpin_(nullptr)
    , exposureDecBtn_(nullptr)
    , exposureIncBtn_(nullptr)
    , gainSlider_(nullptr)
    , gainSpin_(nullptr)
    , gainDecBtn_(nullptr)
    , gainIncBtn_(nullptr)
    , gammaCheck_(nullptr)
    , gammaSlider_(nullptr)
    , gammaSpin_(nullptr)
    , lightSettingsBtn_(nullptr)
    , triggerSettingsBtn_(nullptr)
    // 显示设置
    , displayGroup_(nullptr)
    , flipHorizontalCheck_(nullptr)
    , flipVerticalCheck_(nullptr)
    , rotationGroup_(nullptr)
    // 十字线设置
    , crosslineGroup_(nullptr)
    , crosslineCheck_(nullptr)
    , crosslineXSpin_(nullptr)
    , crosslineYSpin_(nullptr)
    , crosslineCenterBtn_(nullptr)
    // 采集按钮
    , startCaptureBtn_(nullptr)
    // 侧边按钮
    , okBtn_(nullptr)
    , cancelBtn_(nullptr)
    // 相机和状态
    , camera_(nullptr)
    , ownsCamera_(false)
    , previewTimer_(new QTimer(this))
    , frameCount_(0)
    , lastFpsTime_(0)
    , isCapturing_(false)
    // 图像变换设置
    , rotationAngle_(270)  // 默认270度
    , flipHorizontal_(false)
    , flipVertical_(false)
    // 十字线设置
    , crosslineEnabled_(false)
    , crosslineX_(100.0)
    , crosslineY_(100.0)
    // Gamma设置
    , gammaEnabled_(false)
    , gammaValue_(1.0)
    // 当前位置
    , currentPosition_(0)
{
    // 初始化数组
    for (int i = 0; i < 4; i++) {
        positionRadios_[i] = nullptr;
        rotationRadios_[i] = nullptr;
    }
    setWindowTitle("相机配置");
    setMinimumSize(1200, 700);
    setWindowFlags(windowFlags() | Qt::WindowMaximizeButtonHint);

    setupUI();

    connect(previewTimer_, &QTimer::timeout, this, &CameraConfigDialog::onPreviewTimer);
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
    mainLayout->setContentsMargins(5, 5, 5, 5);
    mainLayout->setSpacing(5);

    // 左侧：预览区域
    auto* previewWidget = new QWidget();
    createPreviewArea(previewWidget);
    mainLayout->addWidget(previewWidget, 3);

    // 中间：控制面板
    auto* controlWidget = new QWidget();
    createControlPanel(controlWidget);
    mainLayout->addWidget(controlWidget, 2);

    // 右侧：确定/取消按钮
    auto* sideWidget = new QWidget();
    createSideButtons(sideWidget);
    mainLayout->addWidget(sideWidget, 0);

    setControlsEnabled(true);
}

void CameraConfigDialog::createPreviewArea(QWidget* parent)
{
    auto* layout = new QVBoxLayout(parent);
    layout->setContentsMargins(0, 0, 0, 0);

    // 预览图像
    previewLabel_ = new QLabel();
    previewLabel_->setMinimumSize(640, 480);
    previewLabel_->setAlignment(Qt::AlignCenter);
    previewLabel_->setStyleSheet("QLabel { background-color: #1a1a3a; border: 1px solid #555; }");
    previewLabel_->setText("");
    layout->addWidget(previewLabel_, 1);
}

void CameraConfigDialog::createControlPanel(QWidget* parent)
{
    auto* layout = new QVBoxLayout(parent);
    layout->setContentsMargins(5, 5, 5, 5);
    layout->setSpacing(10);

    createPositionGroup(layout);
    createParameterGroup(layout);
    createDisplayGroup(layout);
    createCrosslineGroup(layout);

    // 开始采集按钮
    startCaptureBtn_ = new QPushButton("开始采集");
    startCaptureBtn_->setMinimumHeight(35);
    connect(startCaptureBtn_, &QPushButton::clicked, this, &CameraConfigDialog::onStartCapture);
    layout->addWidget(startCaptureBtn_);

    layout->addStretch();
}

void CameraConfigDialog::createPositionGroup(QVBoxLayout* layout)
{
    auto* posLayout = new QHBoxLayout();
    positionGroup_ = new QButtonGroup(this);

    for (int i = 0; i < 4; i++) {
        positionRadios_[i] = new QRadioButton(QString("位置%1").arg(i + 1));
        positionGroup_->addButton(positionRadios_[i], i);
        posLayout->addWidget(positionRadios_[i]);
    }
    positionRadios_[0]->setChecked(true);

    connect(positionGroup_, QOverload<int>::of(&QButtonGroup::idClicked),
            this, &CameraConfigDialog::onPositionChanged);

    layout->addLayout(posLayout);
}

void CameraConfigDialog::createParameterGroup(QVBoxLayout* layout)
{
    paramGroup_ = new QGroupBox("参数设置");
    auto* groupLayout = new QVBoxLayout(paramGroup_);
    groupLayout->setSpacing(8);

    // 曝光
    auto* exposureLayout = new QHBoxLayout();
    exposureLayout->addWidget(new QLabel("曝光："));

    exposureSlider_ = new QSlider(Qt::Horizontal);
    exposureSlider_->setRange(1, 100000);
    exposureSlider_->setValue(50000);
    exposureLayout->addWidget(exposureSlider_, 1);

    exposureDecBtn_ = new QToolButton();
    exposureDecBtn_->setText("-0.01");
    exposureDecBtn_->setFixedWidth(45);
    exposureLayout->addWidget(exposureDecBtn_);

    exposureSpin_ = new QDoubleSpinBox();
    exposureSpin_->setRange(0.01, 1000.0);
    exposureSpin_->setValue(50.0);
    exposureSpin_->setDecimals(2);
    exposureSpin_->setSuffix("");
    exposureSpin_->setFixedWidth(70);
    exposureLayout->addWidget(exposureSpin_);

    exposureIncBtn_ = new QToolButton();
    exposureIncBtn_->setText("+0.01");
    exposureIncBtn_->setFixedWidth(45);
    exposureLayout->addWidget(exposureIncBtn_);

    auto* exposureUnitLabel = new QLabel("ms");
    exposureUnitLabel->setFixedWidth(30);
    exposureLayout->addWidget(exposureUnitLabel);

    groupLayout->addLayout(exposureLayout);

    // 曝光信号连接
    connect(exposureSlider_, &QSlider::valueChanged, [this](int value) {
        exposureSpin_->blockSignals(true);
        exposureSpin_->setValue(value / 1000.0);
        exposureSpin_->blockSignals(false);
        onExposureChanged(value / 1000.0);
    });
    connect(exposureSpin_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &CameraConfigDialog::onExposureChanged);
    connect(exposureDecBtn_, &QToolButton::clicked, [this]() {
        exposureSpin_->setValue(exposureSpin_->value() - 0.01);
    });
    connect(exposureIncBtn_, &QToolButton::clicked, [this]() {
        exposureSpin_->setValue(exposureSpin_->value() + 0.01);
    });

    // 增益
    auto* gainLayout = new QHBoxLayout();
    gainLayout->addWidget(new QLabel("增益："));

    gainSlider_ = new QSlider(Qt::Horizontal);
    gainSlider_->setRange(0, 480);
    gainSlider_->setValue(150);
    gainLayout->addWidget(gainSlider_, 1);

    gainDecBtn_ = new QToolButton();
    gainDecBtn_->setText("-0.01");
    gainDecBtn_->setFixedWidth(45);
    gainLayout->addWidget(gainDecBtn_);

    gainSpin_ = new QDoubleSpinBox();
    gainSpin_->setRange(0, 48.0);
    gainSpin_->setValue(15.0);
    gainSpin_->setDecimals(2);
    gainSpin_->setFixedWidth(70);
    gainLayout->addWidget(gainSpin_);

    gainIncBtn_ = new QToolButton();
    gainIncBtn_->setText("+0.01");
    gainIncBtn_->setFixedWidth(45);
    gainLayout->addWidget(gainIncBtn_);

    auto* gainUnitLabel = new QLabel("db");
    gainUnitLabel->setFixedWidth(30);
    gainLayout->addWidget(gainUnitLabel);

    groupLayout->addLayout(gainLayout);

    // 增益信号连接
    connect(gainSlider_, &QSlider::valueChanged, [this](int value) {
        gainSpin_->blockSignals(true);
        gainSpin_->setValue(value / 10.0);
        gainSpin_->blockSignals(false);
        onGainChanged(value / 10.0);
    });
    connect(gainSpin_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &CameraConfigDialog::onGainChanged);
    connect(gainDecBtn_, &QToolButton::clicked, [this]() {
        gainSpin_->setValue(gainSpin_->value() - 0.01);
    });
    connect(gainIncBtn_, &QToolButton::clicked, [this]() {
        gainSpin_->setValue(gainSpin_->value() + 0.01);
    });

    // Gamma
    auto* gammaLayout = new QHBoxLayout();
    gammaCheck_ = new QCheckBox("Gamma");
    gammaCheck_->setFixedWidth(70);
    gammaLayout->addWidget(gammaCheck_);

    gammaSlider_ = new QSlider(Qt::Horizontal);
    gammaSlider_->setRange(10, 300);
    gammaSlider_->setValue(100);
    gammaSlider_->setEnabled(false);
    gammaLayout->addWidget(gammaSlider_, 1);

    gammaSpin_ = new QDoubleSpinBox();
    gammaSpin_->setRange(0.1, 3.0);
    gammaSpin_->setValue(1.0);
    gammaSpin_->setDecimals(2);
    gammaSpin_->setFixedWidth(70);
    gammaSpin_->setEnabled(false);
    gammaLayout->addWidget(gammaSpin_);

    groupLayout->addLayout(gammaLayout);

    // Gamma信号连接
    connect(gammaCheck_, &QCheckBox::toggled, this, &CameraConfigDialog::onGammaEnabled);
    connect(gammaSlider_, &QSlider::valueChanged, [this](int value) {
        gammaSpin_->blockSignals(true);
        gammaSpin_->setValue(value / 100.0);
        gammaSpin_->blockSignals(false);
        onGammaChanged(value / 100.0);
    });
    connect(gammaSpin_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &CameraConfigDialog::onGammaChanged);

    // 光源设置和触发模式按钮
    auto* btnLayout = new QHBoxLayout();
    lightSettingsBtn_ = new QPushButton("光源设置");
    triggerSettingsBtn_ = new QPushButton("触发模式");
    connect(lightSettingsBtn_, &QPushButton::clicked, this, &CameraConfigDialog::onLightSettings);
    connect(triggerSettingsBtn_, &QPushButton::clicked, this, &CameraConfigDialog::onTriggerSettings);
    btnLayout->addWidget(lightSettingsBtn_);
    btnLayout->addWidget(triggerSettingsBtn_);
    groupLayout->addLayout(btnLayout);

    layout->addWidget(paramGroup_);
}

void CameraConfigDialog::createDisplayGroup(QVBoxLayout* layout)
{
    displayGroup_ = new QGroupBox("显示设置");
    auto* groupLayout = new QVBoxLayout(displayGroup_);

    // 镜像
    auto* mirrorLayout = new QHBoxLayout();
    mirrorLayout->addWidget(new QLabel("镜像："));
    flipHorizontalCheck_ = new QCheckBox("水平镜像");
    flipVerticalCheck_ = new QCheckBox("垂直镜像");
    mirrorLayout->addWidget(flipHorizontalCheck_);
    mirrorLayout->addWidget(flipVerticalCheck_);
    mirrorLayout->addStretch();
    groupLayout->addLayout(mirrorLayout);

    connect(flipHorizontalCheck_, &QCheckBox::toggled, [this](bool checked) {
        flipHorizontal_ = checked;
    });
    connect(flipVerticalCheck_, &QCheckBox::toggled, [this](bool checked) {
        flipVertical_ = checked;
    });

    // 旋转
    auto* rotateLayout = new QHBoxLayout();
    rotateLayout->addWidget(new QLabel("旋转："));
    rotationGroup_ = new QButtonGroup(this);

    const char* rotLabels[] = {"0 度", "90 度", "180 度", "270 度"};
    const int rotValues[] = {0, 90, 180, 270};

    for (int i = 0; i < 4; i++) {
        rotationRadios_[i] = new QRadioButton(rotLabels[i]);
        rotationGroup_->addButton(rotationRadios_[i], rotValues[i]);
        rotateLayout->addWidget(rotationRadios_[i]);
    }
    rotationRadios_[3]->setChecked(true);  // 默认270度
    rotateLayout->addStretch();
    groupLayout->addLayout(rotateLayout);

    connect(rotationGroup_, QOverload<int>::of(&QButtonGroup::idClicked),
            this, &CameraConfigDialog::onRotationChanged);

    layout->addWidget(displayGroup_);
}

void CameraConfigDialog::createCrosslineGroup(QVBoxLayout* layout)
{
    crosslineGroup_ = new QGroupBox("十字线");
    auto* groupLayout = new QHBoxLayout(crosslineGroup_);

    crosslineCheck_ = new QCheckBox("启用");
    groupLayout->addWidget(crosslineCheck_);

    groupLayout->addWidget(new QLabel("X"));
    crosslineXSpin_ = new QDoubleSpinBox();
    crosslineXSpin_->setRange(0, 10000);
    crosslineXSpin_->setValue(100.0);
    crosslineXSpin_->setDecimals(1);
    crosslineXSpin_->setEnabled(false);
    groupLayout->addWidget(crosslineXSpin_);

    groupLayout->addWidget(new QLabel("Y"));
    crosslineYSpin_ = new QDoubleSpinBox();
    crosslineYSpin_->setRange(0, 10000);
    crosslineYSpin_->setValue(100.0);
    crosslineYSpin_->setDecimals(1);
    crosslineYSpin_->setEnabled(false);
    groupLayout->addWidget(crosslineYSpin_);

    crosslineCenterBtn_ = new QPushButton("图像中心");
    crosslineCenterBtn_->setEnabled(false);
    groupLayout->addWidget(crosslineCenterBtn_);

    groupLayout->addStretch();

    connect(crosslineCheck_, &QCheckBox::toggled, this, &CameraConfigDialog::onCrosslineEnabled);
    connect(crosslineCenterBtn_, &QPushButton::clicked, this, &CameraConfigDialog::onCrosslineCenterClicked);
    connect(crosslineXSpin_, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [this](double value) {
        crosslineX_ = value;
    });
    connect(crosslineYSpin_, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [this](double value) {
        crosslineY_ = value;
    });

    layout->addWidget(crosslineGroup_);
}

void CameraConfigDialog::createSideButtons(QWidget* parent)
{
    auto* layout = new QVBoxLayout(parent);
    layout->setContentsMargins(5, 5, 5, 5);
    layout->setSpacing(10);

    // 工位标签
    auto* stationLabel = new QLabel("工位1");
    stationLabel->setAlignment(Qt::AlignCenter);
    stationLabel->setStyleSheet("font-size: 14px; font-weight: bold;");
    layout->addWidget(stationLabel);

    layout->addStretch();

    // 确定按钮
    okBtn_ = new QToolButton();
    okBtn_->setText("确定");
    okBtn_->setIcon(QIcon(":/icons/check.png"));
    okBtn_->setIconSize(QSize(48, 48));
    okBtn_->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    okBtn_->setMinimumSize(80, 80);
    okBtn_->setStyleSheet(
        "QToolButton { background-color: #4a90d9; color: white; border-radius: 5px; font-size: 14px; }"
        "QToolButton:hover { background-color: #5aa0e9; }");
    connect(okBtn_, &QToolButton::clicked, this, &QDialog::accept);
    layout->addWidget(okBtn_);

    // 取消按钮
    cancelBtn_ = new QToolButton();
    cancelBtn_->setText("取消");
    cancelBtn_->setIcon(QIcon(":/icons/cancel.png"));
    cancelBtn_->setIconSize(QSize(48, 48));
    cancelBtn_->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    cancelBtn_->setMinimumSize(80, 80);
    cancelBtn_->setStyleSheet(
        "QToolButton { background-color: #d94a4a; color: white; border-radius: 5px; font-size: 14px; }"
        "QToolButton:hover { background-color: #e95a5a; }");
    connect(cancelBtn_, &QToolButton::clicked, this, &QDialog::reject);
    layout->addWidget(cancelBtn_);

    layout->addStretch();
}

void CameraConfigDialog::onPositionChanged(int id)
{
    currentPosition_ = id;
    LOG_DEBUG(QString("切换到位置 %1").arg(id + 1));
}

void CameraConfigDialog::onStartCapture()
{
    if (!camera_ || !camera_->isOpen()) {
        QMessageBox::warning(this, "警告", "请先连接相机");
        return;
    }

    if (isCapturing_) {
        // 停止采集
        previewTimer_->stop();
        if (camera_->isGrabbing()) {
            camera_->stopGrabbing();
        }
        isCapturing_ = false;
        startCaptureBtn_->setText("开始采集");
    } else {
        // 开始采集
        if (!camera_->startGrabbing()) {
            QMessageBox::critical(this, "错误", "启动采集失败");
            return;
        }

        frameCount_ = 0;
        lastFpsTime_ = QDateTime::currentMSecsSinceEpoch();
        previewTimer_->start(33);  // 约30fps

        isCapturing_ = true;
        startCaptureBtn_->setText("停止采集");
    }
}

void CameraConfigDialog::onExposureChanged(double value)
{
    exposureSlider_->blockSignals(true);
    exposureSlider_->setValue(static_cast<int>(value * 1000));
    exposureSlider_->blockSignals(false);

    if (camera_ && camera_->isOpen()) {
        camera_->setExposure(value * 1000);  // 转换为微秒
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

void CameraConfigDialog::onGammaChanged(double value)
{
    gammaSlider_->blockSignals(true);
    gammaSlider_->setValue(static_cast<int>(value * 100));
    gammaSlider_->blockSignals(false);

    gammaValue_ = value;
}

void CameraConfigDialog::onGammaEnabled(bool enabled)
{
    gammaEnabled_ = enabled;
    gammaSlider_->setEnabled(enabled);
    gammaSpin_->setEnabled(enabled);
}

void CameraConfigDialog::onLightSettings()
{
    QMessageBox::information(this, "光源设置", "光源设置功能开发中...");
}

void CameraConfigDialog::onTriggerSettings()
{
    QMessageBox::information(this, "触发模式", "触发模式设置功能开发中...");
}

void CameraConfigDialog::onRotationChanged(int id)
{
    rotationAngle_ = id;
    LOG_DEBUG(QString("设置旋转角度: %1°").arg(rotationAngle_));
}

void CameraConfigDialog::onCrosslineEnabled(bool enabled)
{
    crosslineEnabled_ = enabled;
    crosslineXSpin_->setEnabled(enabled);
    crosslineYSpin_->setEnabled(enabled);
    crosslineCenterBtn_->setEnabled(enabled);
}

void CameraConfigDialog::onCrosslineCenterClicked()
{
    if (camera_ && camera_->isOpen()) {
        auto config = camera_->getConfig();
        crosslineXSpin_->setValue(config.width / 2.0);
        crosslineYSpin_->setValue(config.height / 2.0);
        crosslineX_ = config.width / 2.0;
        crosslineY_ = config.height / 2.0;
    }
}

void CameraConfigDialog::onPreviewTimer()
{
    if (!camera_ || !camera_->isOpen()) return;

    // 获取图像
    auto image = camera_->grabImage(100);
    if (image) {
        updatePreviewImage(image);
        frameCount_++;
    }
}

void CameraConfigDialog::updatePreviewImage(Base::ImageData::Ptr image)
{
    if (!image) return;

    cv::Mat mat = image->mat();
    if (mat.empty()) return;

    // 应用变换
    cv::Mat transformed = applyTransform(mat);

    // 应用Gamma校正
    if (gammaEnabled_ && gammaValue_ != 1.0) {
        cv::Mat lookUpTable(1, 256, CV_8U);
        uchar* p = lookUpTable.ptr();
        for (int i = 0; i < 256; i++) {
            p[i] = cv::saturate_cast<uchar>(pow(i / 255.0, 1.0 / gammaValue_) * 255.0);
        }
        cv::LUT(transformed, lookUpTable, transformed);
    }

    // 绘制十字线
    if (crosslineEnabled_) {
        drawCrossline(transformed);
    }

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
        cv::Mat flipped;
        cv::flip(result, flipped, -1);
        result = flipped;
    } else if (flipHorizontal_) {
        cv::Mat flipped;
        cv::flip(result, flipped, 1);
        result = flipped;
    } else if (flipVertical_) {
        cv::Mat flipped;
        cv::flip(result, flipped, 0);
        result = flipped;
    }

    return result;
}

void CameraConfigDialog::drawCrossline(cv::Mat& image)
{
    if (image.empty()) return;

    int x = static_cast<int>(crosslineX_);
    int y = static_cast<int>(crosslineY_);

    // 绘制十字线（绿色）
    cv::Scalar color = (image.channels() == 3) ? cv::Scalar(0, 255, 0) : cv::Scalar(255);

    // 水平线
    cv::line(image, cv::Point(0, y), cv::Point(image.cols - 1, y), color, 1);
    // 垂直线
    cv::line(image, cv::Point(x, 0), cv::Point(x, image.rows - 1), color, 1);
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
        exposureSpin_->setRange(expMin / 1000.0, expMax / 1000.0);
        exposureSlider_->setRange(static_cast<int>(expMin), static_cast<int>(expMax));

        // 增益范围
        double gainMin, gainMax;
        hikCamera->getGainRange(gainMin, gainMax);
        gainSpin_->setRange(gainMin, gainMax);
        gainSlider_->setRange(static_cast<int>(gainMin * 10), static_cast<int>(gainMax * 10));
    }
#endif
}

void CameraConfigDialog::setControlsEnabled(bool enabled)
{
    if (paramGroup_) paramGroup_->setEnabled(enabled);
    if (displayGroup_) displayGroup_->setEnabled(enabled);
    if (crosslineGroup_) crosslineGroup_->setEnabled(enabled);
    if (startCaptureBtn_) startCaptureBtn_->setEnabled(enabled);
}

HAL::ICamera* CameraConfigDialog::takeCamera()
{
    HAL::ICamera* cam = camera_;
    if (cam) {
        cam->setParent(nullptr);
    }
    camera_ = nullptr;
    ownsCamera_ = false;
    return cam;
}

void CameraConfigDialog::setCamera(HAL::ICamera* camera)
{
    // 停止当前采集
    if (isCapturing_) {
        if (previewTimer_) previewTimer_->stop();
        if (camera_ && camera_->isGrabbing()) {
            camera_->stopGrabbing();
        }
        isCapturing_ = false;
        if (startCaptureBtn_) startCaptureBtn_->setText("开始采集");
    }

    camera_ = camera;
    ownsCamera_ = false;

    if (camera_ && camera_->isOpen()) {
        setControlsEnabled(true);
        updateParameterRanges();

        HAL::ICamera::Config config = camera_->getConfig();
        if (exposureSpin_) exposureSpin_->setValue(config.exposure / 1000.0);  // 转换为毫秒
        if (gainSpin_) gainSpin_->setValue(config.gain);

        // 设置十字线默认为图像中心
        if (crosslineXSpin_) crosslineXSpin_->setValue(config.width / 2.0);
        if (crosslineYSpin_) crosslineYSpin_->setValue(config.height / 2.0);
        crosslineX_ = config.width / 2.0;
        crosslineY_ = config.height / 2.0;
    } else {
        setControlsEnabled(false);
    }
}

} // namespace VisionForge
