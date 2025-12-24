/**
 * @file QRCalibDialog.cpp
 * @brief QR码/DM码自动标定对话框实现
 */

#include "ui/QRCalibDialog.h"
#include "base/Logger.h"

#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFormLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>

namespace VisionForge {
namespace UI {

QRCalibDialog::QRCalibDialog(QWidget* parent)
    : QDialog(parent)
    , qrCalibrator_(new Calibration::QRCodeCalibration(this))
    , calibrated_(false)
{
    setWindowTitle(tr("QR码/DM码自动标定"));
    setMinimumSize(1000, 700);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    createUI();

    // 连接信号
    connect(qrCalibrator_, &Calibration::QRCodeCalibration::progressUpdated,
            this, &QRCalibDialog::onProgressUpdated);
    connect(qrCalibrator_, &Calibration::QRCodeCalibration::calibrationCompleted,
            this, [this](bool success, const Calibration::CalibrationResult& result) {
        if (success) {
            calibrated_ = true;
            updateCalibrationResult();
            statusLabel_->setText(tr("标定完成"));
        } else {
            statusLabel_->setText(tr("标定失败: %1").arg(qrCalibrator_->getLastError()));
        }
    });
}

QRCalibDialog::~QRCalibDialog()
{
}

void QRCalibDialog::setCurrentImage(const Base::ImageData::Ptr& image)
{
    currentImage_ = image;
    if (imageViewer_ && image) {
        imageViewer_->setImage(image);
    }
}

Algorithm::CalibrationResult QRCalibDialog::getCalibrationResult() const
{
    if (qrCalibrator_ && qrCalibrator_->isCalibrated()) {
        // 转换Calibration::CalibrationResult到Algorithm::CalibrationResult
        Algorithm::CalibrationResult result;
        const auto& qrResult = qrCalibrator_->getResult();
        result.setType(Algorithm::CalibrationType::NinePoint);
        result.setPixelToMMX(qrResult.scaleX);
        result.setPixelToMMY(qrResult.scaleY);
        result.setRotationAngle(qrResult.rotation);
        result.setCalibrationError(qrResult.errorStats.meanError);
        return result;
    }
    return Algorithm::CalibrationResult();
}

bool QRCalibDialog::isCalibrated() const
{
    return calibrated_ && qrCalibrator_ && qrCalibrator_->isCalibrated();
}

void QRCalibDialog::createUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(10);

    // 上部区域：配置 + 预览
    QHBoxLayout* topLayout = new QHBoxLayout();

    createConfigGroup();
    createPreviewGroup();

    topLayout->addWidget(configGroup_);
    topLayout->addWidget(previewGroup_, 1);

    mainLayout->addLayout(topLayout);

    // 下部区域：结果
    createResultGroup();
    mainLayout->addWidget(resultGroup_);

    // 底部按钮
    createButtons();
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(detectBtn_);
    buttonLayout->addWidget(calibrateBtn_);
    buttonLayout->addWidget(resetBtn_);
    buttonLayout->addStretch();
    buttonLayout->addWidget(okBtn_);
    buttonLayout->addWidget(cancelBtn_);
    mainLayout->addLayout(buttonLayout);
}

void QRCalibDialog::createConfigGroup()
{
    configGroup_ = new QGroupBox(tr("标定配置"), this);
    QFormLayout* layout = new QFormLayout(configGroup_);

    // 码类型
    codeTypeCombo_ = new QComboBox(this);
    codeTypeCombo_->addItem(tr("自动检测"), static_cast<int>(Calibration::Code2DType::Auto));
    codeTypeCombo_->addItem(tr("QR码"), static_cast<int>(Calibration::Code2DType::QRCode));
    codeTypeCombo_->addItem(tr("DataMatrix码"), static_cast<int>(Calibration::Code2DType::DataMatrix));
    layout->addRow(tr("码类型:"), codeTypeCombo_);

    // 标定板布局
    boardLayoutCombo_ = new QComboBox(this);
    boardLayoutCombo_->addItem(tr("单个码"), static_cast<int>(Calibration::CalibrationBoardLayout::Single));
    boardLayoutCombo_->addItem(tr("2x2网格"), static_cast<int>(Calibration::CalibrationBoardLayout::Grid2x2));
    boardLayoutCombo_->addItem(tr("3x3网格"), static_cast<int>(Calibration::CalibrationBoardLayout::Grid3x3));
    boardLayoutCombo_->addItem(tr("4x4网格"), static_cast<int>(Calibration::CalibrationBoardLayout::Grid4x4));
    layout->addRow(tr("标定板布局:"), boardLayoutCombo_);

    // 码尺寸
    codeSizeSpin_ = new QDoubleSpinBox(this);
    codeSizeSpin_->setRange(1.0, 500.0);
    codeSizeSpin_->setValue(10.0);
    codeSizeSpin_->setSuffix(" mm");
    codeSizeSpin_->setDecimals(2);
    layout->addRow(tr("码尺寸:"), codeSizeSpin_);

    // X间距
    codeSpacingXSpin_ = new QDoubleSpinBox(this);
    codeSpacingXSpin_->setRange(0.0, 1000.0);
    codeSpacingXSpin_->setValue(30.0);
    codeSpacingXSpin_->setSuffix(" mm");
    codeSpacingXSpin_->setDecimals(2);
    layout->addRow(tr("X间距:"), codeSpacingXSpin_);

    // Y间距
    codeSpacingYSpin_ = new QDoubleSpinBox(this);
    codeSpacingYSpin_->setRange(0.0, 1000.0);
    codeSpacingYSpin_->setValue(30.0);
    codeSpacingYSpin_->setSuffix(" mm");
    codeSpacingYSpin_->setDecimals(2);
    layout->addRow(tr("Y间距:"), codeSpacingYSpin_);

    connect(codeTypeCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &QRCalibDialog::onCodeTypeChanged);
    connect(boardLayoutCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &QRCalibDialog::onBoardLayoutChanged);
}

void QRCalibDialog::createPreviewGroup()
{
    previewGroup_ = new QGroupBox(tr("图像预览"), this);
    QVBoxLayout* layout = new QVBoxLayout(previewGroup_);

    // 图像操作按钮
    QHBoxLayout* imageButtonLayout = new QHBoxLayout();

    loadImageBtn_ = new QPushButton(tr("加载图像"), this);
    loadImageBtn_->setMinimumWidth(100);
    loadImageBtn_->setToolTip(tr("从文件加载图像"));
    connect(loadImageBtn_, &QPushButton::clicked, this, &QRCalibDialog::onLoadImageClicked);
    imageButtonLayout->addWidget(loadImageBtn_);

    captureImageBtn_ = new QPushButton(tr("采集图像"), this);
    captureImageBtn_->setMinimumWidth(100);
    captureImageBtn_->setToolTip(tr("从相机采集图像"));
    connect(captureImageBtn_, &QPushButton::clicked, this, &QRCalibDialog::onCaptureImageClicked);
    imageButtonLayout->addWidget(captureImageBtn_);

    imageButtonLayout->addStretch();
    layout->addLayout(imageButtonLayout);

    imageViewer_ = new ImageViewer(this);
    imageViewer_->setMinimumSize(400, 300);
    layout->addWidget(imageViewer_, 1);

    detectionStatusLabel_ = new QLabel(tr("请加载图像并点击'检测码'"), this);
    detectionStatusLabel_->setStyleSheet("QLabel { color: #666; }");
    layout->addWidget(detectionStatusLabel_);
}

void QRCalibDialog::createResultGroup()
{
    resultGroup_ = new QGroupBox(tr("标定结果"), this);
    QHBoxLayout* mainLayout = new QHBoxLayout(resultGroup_);

    // 左侧：检测点列表
    pointsTable_ = new QTableWidget(this);
    pointsTable_->setColumnCount(5);
    pointsTable_->setHorizontalHeaderLabels({tr("索引"), tr("像素X"), tr("像素Y"), tr("物理X"), tr("物理Y")});
    pointsTable_->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    pointsTable_->setMaximumWidth(400);
    mainLayout->addWidget(pointsTable_);

    // 右侧：标定参数
    QVBoxLayout* resultLayout = new QVBoxLayout();

    pixelScaleLabel_ = new QLabel(tr("像素比例: --"), this);
    resultLayout->addWidget(pixelScaleLabel_);

    rotationLabel_ = new QLabel(tr("旋转角度: --"), this);
    resultLayout->addWidget(rotationLabel_);

    calibErrorLabel_ = new QLabel(tr("标定误差: --"), this);
    resultLayout->addWidget(calibErrorLabel_);

    resultLayout->addStretch();

    progressBar_ = new QProgressBar(this);
    progressBar_->setRange(0, 100);
    progressBar_->setValue(0);
    resultLayout->addWidget(progressBar_);

    statusLabel_ = new QLabel(tr("就绪"), this);
    statusLabel_->setStyleSheet("QLabel { color: #00AA00; }");
    resultLayout->addWidget(statusLabel_);

    mainLayout->addLayout(resultLayout);
}

void QRCalibDialog::createButtons()
{
    detectBtn_ = new QPushButton(tr("检测码"), this);
    detectBtn_->setMinimumWidth(100);
    connect(detectBtn_, &QPushButton::clicked, this, &QRCalibDialog::onDetectClicked);

    calibrateBtn_ = new QPushButton(tr("执行标定"), this);
    calibrateBtn_->setMinimumWidth(100);
    calibrateBtn_->setEnabled(false);
    connect(calibrateBtn_, &QPushButton::clicked, this, &QRCalibDialog::onCalibrateClicked);

    resetBtn_ = new QPushButton(tr("重置"), this);
    resetBtn_->setMinimumWidth(80);
    connect(resetBtn_, &QPushButton::clicked, this, &QRCalibDialog::onResetClicked);

    okBtn_ = new QPushButton(tr("确定"), this);
    okBtn_->setMinimumWidth(80);
    okBtn_->setEnabled(false);
    connect(okBtn_, &QPushButton::clicked, this, &QRCalibDialog::onOkClicked);

    cancelBtn_ = new QPushButton(tr("取消"), this);
    cancelBtn_->setMinimumWidth(80);
    connect(cancelBtn_, &QPushButton::clicked, this, &QRCalibDialog::onCancelClicked);
}

void QRCalibDialog::onLoadImageClicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("选择图像文件"),
        QString(),
        tr("图像文件 (*.png *.jpg *.jpeg *.bmp *.tif *.tiff);;所有文件 (*.*)")
    );

    if (fileName.isEmpty()) {
        return;
    }

    // 使用QFile读取文件（支持中文路径）
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, tr("错误"), tr("无法打开图像文件: %1").arg(fileName));
        return;
    }

    QByteArray fileData = file.readAll();
    file.close();

    // 使用cv::imdecode解码图像数据
    std::vector<uchar> buffer(fileData.begin(), fileData.end());
    cv::Mat mat = cv::imdecode(buffer, cv::IMREAD_COLOR);

    if (mat.empty()) {
        QMessageBox::warning(this, tr("错误"), tr("无法解码图像文件: %1").arg(fileName));
        return;
    }

    // 创建ImageData并设置（从cv::Mat构造会自动深拷贝）
    currentImage_ = std::make_shared<Base::ImageData>(mat);

    imageViewer_->setImage(currentImage_);
    detectionStatusLabel_->setText(tr("已加载图像: %1x%2").arg(mat.cols).arg(mat.rows));
    statusLabel_->setText(tr("图像已加载，请点击'检测码'"));

    // 重置检测状态
    calibrateBtn_->setEnabled(false);
    okBtn_->setEnabled(false);
}

void QRCalibDialog::onCaptureImageClicked()
{
    // TODO: 从当前活动的相机采集图像
    // 这需要访问CameraManager或者通过信号请求主窗口采集

    QMessageBox::information(this, tr("提示"),
        tr("采集图像功能需要先在主界面连接相机。\n"
           "请使用'加载图像'按钮加载本地图像，\n"
           "或者先在主界面采集图像后打开此对话框。"));
}

void QRCalibDialog::onDetectClicked()
{
    if (!currentImage_) {
        QMessageBox::warning(this, tr("警告"), tr("请先加载图像"));
        return;
    }

    // 更新检测配置
    Calibration::Code2DDetectionConfig detectConfig;
    detectConfig.expectedType = static_cast<Calibration::Code2DType>(
        codeTypeCombo_->currentData().toInt());
    qrCalibrator_->setDetectionConfig(detectConfig);

    // 更新标定板配置
    Calibration::CalibrationBoardConfig boardConfig;
    boardConfig.layout = static_cast<Calibration::CalibrationBoardLayout>(
        boardLayoutCombo_->currentData().toInt());
    boardConfig.codeSize = codeSizeSpin_->value();
    boardConfig.codeSpacingX = codeSpacingXSpin_->value();
    boardConfig.codeSpacingY = codeSpacingYSpin_->value();

    // 根据布局设置网格尺寸
    switch (boardConfig.layout) {
    case Calibration::CalibrationBoardLayout::Single:
        boardConfig.gridRows = 1;
        boardConfig.gridCols = 1;
        break;
    case Calibration::CalibrationBoardLayout::Grid2x2:
        boardConfig.gridRows = 2;
        boardConfig.gridCols = 2;
        break;
    case Calibration::CalibrationBoardLayout::Grid3x3:
        boardConfig.gridRows = 3;
        boardConfig.gridCols = 3;
        break;
    case Calibration::CalibrationBoardLayout::Grid4x4:
        boardConfig.gridRows = 4;
        boardConfig.gridCols = 4;
        break;
    default:
        break;
    }
    qrCalibrator_->setBoardConfig(boardConfig);

    statusLabel_->setText(tr("正在检测..."));
    progressBar_->setValue(10);

    // 转换图像格式
    cv::Mat mat = currentImage_->mat();
    QImage qimg;
    if (mat.channels() == 1) {
        qimg = QImage(mat.data, mat.cols, mat.rows,
                     static_cast<int>(mat.step), QImage::Format_Grayscale8).copy();
    } else {
        cv::Mat rgb;
        cv::cvtColor(mat, rgb, cv::COLOR_BGR2RGB);
        qimg = QImage(rgb.data, rgb.cols, rgb.rows,
                     static_cast<int>(rgb.step), QImage::Format_RGB888).copy();
    }

    // 执行检测
    auto results = qrCalibrator_->detect(qimg);

    progressBar_->setValue(50);

    if (results.empty()) {
        detectionStatusLabel_->setText(tr("未检测到码"));
        statusLabel_->setText(tr("检测失败: 未找到二维码"));
        calibrateBtn_->setEnabled(false);
    } else {
        detectionStatusLabel_->setText(tr("检测到 %1 个码").arg(results.size()));
        statusLabel_->setText(tr("检测完成，找到 %1 个码").arg(results.size()));
        calibrateBtn_->setEnabled(true);
        updateDetectionResults();
        drawDetectedCodes();
    }

    progressBar_->setValue(100);
}

void QRCalibDialog::onCalibrateClicked()
{
    if (!currentImage_) {
        return;
    }

    statusLabel_->setText(tr("正在标定..."));
    progressBar_->setValue(10);

    // 转换图像格式
    cv::Mat mat = currentImage_->mat();
    QImage qimg;
    if (mat.channels() == 1) {
        qimg = QImage(mat.data, mat.cols, mat.rows,
                     static_cast<int>(mat.step), QImage::Format_Grayscale8).copy();
    } else {
        cv::Mat rgb;
        cv::cvtColor(mat, rgb, cv::COLOR_BGR2RGB);
        qimg = QImage(rgb.data, rgb.cols, rgb.rows,
                     static_cast<int>(rgb.step), QImage::Format_RGB888).copy();
    }

    // 执行标定
    auto result = qrCalibrator_->detectAndCalibrate(qimg);

    if (result.isValid()) {
        calibrated_ = true;
        updateCalibrationResult();
        okBtn_->setEnabled(true);
        statusLabel_->setText(tr("标定成功"));

        // 发送标定完成信号
        emit calibrationCompleted(getCalibrationResult());
    } else {
        statusLabel_->setText(tr("标定失败: %1").arg(qrCalibrator_->getLastError()));
    }

    progressBar_->setValue(100);
}

void QRCalibDialog::onResetClicked()
{
    qrCalibrator_->clearPoints();
    qrCalibrator_->clearDetectionResults();
    calibrated_ = false;

    pointsTable_->setRowCount(0);
    pixelScaleLabel_->setText(tr("像素比例: --"));
    rotationLabel_->setText(tr("旋转角度: --"));
    calibErrorLabel_->setText(tr("标定误差: --"));
    progressBar_->setValue(0);
    statusLabel_->setText(tr("已重置"));
    detectionStatusLabel_->setText(tr("请加载图像并点击'检测码'"));

    calibrateBtn_->setEnabled(false);
    okBtn_->setEnabled(false);

    // 清除图像上的标记
    if (currentImage_) {
        setCurrentImage(currentImage_);
    }
}

void QRCalibDialog::onCodeTypeChanged(int index)
{
    Q_UNUSED(index)
    // 可以根据码类型调整默认参数
}

void QRCalibDialog::onBoardLayoutChanged(int index)
{
    Q_UNUSED(index)
    // 可以根据布局调整间距默认值
}

void QRCalibDialog::onOkClicked()
{
    if (calibrated_) {
        accept();
    }
}

void QRCalibDialog::onCancelClicked()
{
    reject();
}

void QRCalibDialog::onProgressUpdated(int progress, const QString& message)
{
    progressBar_->setValue(progress);
    statusLabel_->setText(message);
}

void QRCalibDialog::updateDetectionResults()
{
    const auto& results = qrCalibrator_->getLastDetectionResults();

    pointsTable_->setRowCount(0);
    int row = 0;

    for (const auto& result : results) {
        // 每个码有4个角点
        for (int i = 0; i < 4; ++i) {
            pointsTable_->insertRow(row);
            pointsTable_->setItem(row, 0, new QTableWidgetItem(
                QString("%1-%2").arg(result.index).arg(i)));
            pointsTable_->setItem(row, 1, new QTableWidgetItem(
                QString::number(result.corners[i].x(), 'f', 2)));
            pointsTable_->setItem(row, 2, new QTableWidgetItem(
                QString::number(result.corners[i].y(), 'f', 2)));
            // 物理坐标需要从标定板配置计算
            pointsTable_->setItem(row, 3, new QTableWidgetItem("--"));
            pointsTable_->setItem(row, 4, new QTableWidgetItem("--"));
            ++row;
        }
    }
}

void QRCalibDialog::updateCalibrationResult()
{
    if (!qrCalibrator_->isCalibrated()) {
        return;
    }

    const auto& result = qrCalibrator_->getResult();

    // 使用scaleX和scaleY的平均值作为像素比例
    double avgScale = (result.scaleX + result.scaleY) / 2.0;
    pixelScaleLabel_->setText(tr("像素比例: %1 mm/pixel").arg(avgScale, 0, 'f', 4));
    rotationLabel_->setText(tr("旋转角度: %1 度").arg(result.rotation, 0, 'f', 3));
    calibErrorLabel_->setText(tr("标定误差: %1 pixel").arg(result.errorStats.meanError, 0, 'f', 3));
}

void QRCalibDialog::drawDetectedCodes()
{
    // TODO: 在ImageViewer上绘制检测到的码和角点
    // 这需要ImageViewer支持叠加绘图
}

} // namespace UI
} // namespace VisionForge
