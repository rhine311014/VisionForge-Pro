/**
 * @file CameraCalibDialog.cpp
 * @brief 相机标定对话框实现
 */

#include "ui/CameraCalibDialog.h"
#include "base/Logger.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QGridLayout>
#include <QFormLayout>
#include <QDir>
#include <QApplication>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>

namespace VisionForge {
namespace UI {

CameraCalibDialog::CameraCalibDialog(QWidget* parent)
    : QDialog(parent)
    , calibTool_(new Algorithm::CameraCalibTool(this))
    , imageViewer_(nullptr)
    , patternTypeCombo_(nullptr)
    , boardWidthSpin_(nullptr)
    , boardHeightSpin_(nullptr)
    , squareSizeSpin_(nullptr)
    , backendCombo_(nullptr)
    , subPixelCheck_(nullptr)
    , adaptiveThreshCheck_(nullptr)
    , fastCheckCheck_(nullptr)
    , minImageCountSpin_(nullptr)
    , imageList_(nullptr)
    , imageCountLabel_(nullptr)
    , progressBar_(nullptr)
{
    setWindowTitle("相机标定");
    setMinimumSize(1000, 700);
    resize(1200, 800);

    createUI();
    connectSignals();
    updateUI();
}

CameraCalibDialog::~CameraCalibDialog()
{
}

void CameraCalibDialog::setCurrentImage(const Base::ImageData::Ptr& image)
{
    currentImage_ = image;
    if (imageViewer_ && image) {
        imageViewer_->setImage(image);
    }
}

Algorithm::CalibrationResult CameraCalibDialog::getCalibrationResult() const
{
    return calibTool_->getResult();
}

bool CameraCalibDialog::isCalibrated() const
{
    return calibTool_->isCalibrated();
}

void CameraCalibDialog::showEvent(QShowEvent* event)
{
    QDialog::showEvent(event);
    updateUI();
}

void CameraCalibDialog::closeEvent(QCloseEvent* event)
{
    QDialog::closeEvent(event);
}

void CameraCalibDialog::createUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(10);

    // 主分割器
    QSplitter* mainSplitter = new QSplitter(Qt::Horizontal, this);

    createImagePreviewArea(mainSplitter);
    createControlPanel(mainSplitter);

    mainSplitter->setStretchFactor(0, 2);  // 图像预览区域
    mainSplitter->setStretchFactor(1, 1);  // 控制面板

    mainLayout->addWidget(mainSplitter, 1);

    // 对话框按钮
    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->addStretch();

    okBtn_ = new QPushButton("确定", this);
    cancelBtn_ = new QPushButton("取消", this);
    applyBtn_ = new QPushButton("应用", this);

    btnLayout->addWidget(okBtn_);
    btnLayout->addWidget(cancelBtn_);
    btnLayout->addWidget(applyBtn_);

    mainLayout->addLayout(btnLayout);
}

void CameraCalibDialog::createImagePreviewArea(QSplitter* splitter)
{
    QWidget* previewWidget = new QWidget(splitter);
    QVBoxLayout* layout = new QVBoxLayout(previewWidget);
    layout->setContentsMargins(0, 0, 0, 0);

    // 图像查看器
    imageViewer_ = new ImageViewer(previewWidget);
    layout->addWidget(imageViewer_, 1);

    // 检测按钮
    QHBoxLayout* btnLayout = new QHBoxLayout();
    detectBtn_ = new QPushButton("检测角点", previewWidget);
    detectBtn_->setMinimumHeight(35);
    btnLayout->addWidget(detectBtn_);

    layout->addLayout(btnLayout);

    splitter->addWidget(previewWidget);
}

void CameraCalibDialog::createControlPanel(QSplitter* splitter)
{
    QWidget* controlWidget = new QWidget(splitter);
    QVBoxLayout* layout = new QVBoxLayout(controlWidget);
    layout->setContentsMargins(5, 5, 5, 5);
    layout->setSpacing(10);

    createPatternParamsGroup(layout);
    createDetectionParamsGroup(layout);
    createImageListGroup(layout);
    createResultGroup(layout);
    createButtons(layout);

    layout->addStretch();

    splitter->addWidget(controlWidget);
}

void CameraCalibDialog::createPatternParamsGroup(QVBoxLayout* layout)
{
    QGroupBox* group = new QGroupBox("标定板参数", this);
    QFormLayout* formLayout = new QFormLayout(group);

    // 标定板类型
    patternTypeCombo_ = new QComboBox(group);
    patternTypeCombo_->addItem("棋盘格", static_cast<int>(Algorithm::CalibPatternType::Chessboard));
    patternTypeCombo_->addItem("对称圆点阵", static_cast<int>(Algorithm::CalibPatternType::CircleGrid));
    patternTypeCombo_->addItem("非对称圆点阵", static_cast<int>(Algorithm::CalibPatternType::AsymCircleGrid));
    formLayout->addRow("类型:", patternTypeCombo_);

    // 标定板尺寸
    QHBoxLayout* sizeLayout = new QHBoxLayout();
    boardWidthSpin_ = new QSpinBox(group);
    boardWidthSpin_->setRange(3, 30);
    boardWidthSpin_->setValue(9);
    boardWidthSpin_->setSuffix(" 个角点");

    boardHeightSpin_ = new QSpinBox(group);
    boardHeightSpin_->setRange(3, 30);
    boardHeightSpin_->setValue(6);
    boardHeightSpin_->setSuffix(" 个角点");

    sizeLayout->addWidget(boardWidthSpin_);
    sizeLayout->addWidget(new QLabel("x"));
    sizeLayout->addWidget(boardHeightSpin_);
    formLayout->addRow("尺寸:", sizeLayout);

    // 格子大小
    squareSizeSpin_ = new QDoubleSpinBox(group);
    squareSizeSpin_->setRange(1.0, 500.0);
    squareSizeSpin_->setValue(25.0);
    squareSizeSpin_->setSuffix(" mm");
    squareSizeSpin_->setDecimals(2);
    formLayout->addRow("格子尺寸:", squareSizeSpin_);

    // 后端选择
    backendCombo_ = new QComboBox(group);
    backendCombo_->addItem("OpenCV", static_cast<int>(Algorithm::CalibBackend::OpenCV));
#ifdef USE_HALCON
    backendCombo_->addItem("Halcon", static_cast<int>(Algorithm::CalibBackend::Halcon));
#endif
    formLayout->addRow("后端:", backendCombo_);

    layout->addWidget(group);
}

void CameraCalibDialog::createDetectionParamsGroup(QVBoxLayout* layout)
{
    QGroupBox* group = new QGroupBox("检测参数", this);
    QVBoxLayout* vLayout = new QVBoxLayout(group);

    subPixelCheck_ = new QCheckBox("亚像素精度优化", group);
    subPixelCheck_->setChecked(true);
    vLayout->addWidget(subPixelCheck_);

    adaptiveThreshCheck_ = new QCheckBox("自适应阈值", group);
    adaptiveThreshCheck_->setChecked(true);
    vLayout->addWidget(adaptiveThreshCheck_);

    fastCheckCheck_ = new QCheckBox("快速检测模式", group);
    fastCheckCheck_->setChecked(true);
    vLayout->addWidget(fastCheckCheck_);

    QHBoxLayout* minLayout = new QHBoxLayout();
    minLayout->addWidget(new QLabel("最少图像数:"));
    minImageCountSpin_ = new QSpinBox(group);
    minImageCountSpin_->setRange(3, 50);
    minImageCountSpin_->setValue(10);
    minLayout->addWidget(minImageCountSpin_);
    minLayout->addStretch();
    vLayout->addLayout(minLayout);

    layout->addWidget(group);
}

void CameraCalibDialog::createImageListGroup(QVBoxLayout* layout)
{
    QGroupBox* group = new QGroupBox("标定图像", this);
    QVBoxLayout* vLayout = new QVBoxLayout(group);

    // 图像计数
    imageCountLabel_ = new QLabel("已采集: 0 张", group);
    vLayout->addWidget(imageCountLabel_);

    // 图像列表
    imageList_ = new QListWidget(group);
    imageList_->setSelectionMode(QAbstractItemView::ExtendedSelection);
    imageList_->setMinimumHeight(120);
    vLayout->addWidget(imageList_);

    // 按钮
    QGridLayout* btnLayout = new QGridLayout();

    addImageBtn_ = new QPushButton("添加当前图像", group);
    loadFolderBtn_ = new QPushButton("加载文件夹", group);
    removeImageBtn_ = new QPushButton("移除选中", group);
    clearAllBtn_ = new QPushButton("清除全部", group);

    btnLayout->addWidget(addImageBtn_, 0, 0);
    btnLayout->addWidget(loadFolderBtn_, 0, 1);
    btnLayout->addWidget(removeImageBtn_, 1, 0);
    btnLayout->addWidget(clearAllBtn_, 1, 1);

    vLayout->addLayout(btnLayout);

    layout->addWidget(group);
}

void CameraCalibDialog::createResultGroup(QVBoxLayout* layout)
{
    QGroupBox* group = new QGroupBox("标定结果", this);
    QFormLayout* formLayout = new QFormLayout(group);

    rmsErrorLabel_ = new QLabel("--", group);
    formLayout->addRow("重投影误差:", rmsErrorLabel_);

    focalLengthLabel_ = new QLabel("--", group);
    formLayout->addRow("焦距 (fx, fy):", focalLengthLabel_);

    principalPointLabel_ = new QLabel("--", group);
    formLayout->addRow("光心 (cx, cy):", principalPointLabel_);

    imagesSizeLabel_ = new QLabel("--", group);
    formLayout->addRow("图像尺寸:", imagesSizeLabel_);

    saveResultBtn_ = new QPushButton("保存结果...", group);
    saveResultBtn_->setEnabled(false);
    formLayout->addRow("", saveResultBtn_);

    layout->addWidget(group);
}

void CameraCalibDialog::createButtons(QVBoxLayout* layout)
{
    // 进度条
    progressBar_ = new QProgressBar(this);
    progressBar_->setRange(0, 100);
    progressBar_->setValue(0);
    progressBar_->setVisible(false);
    layout->addWidget(progressBar_);

    // 标定按钮
    calibrateBtn_ = new QPushButton("执行标定", this);
    calibrateBtn_->setMinimumHeight(40);
    calibrateBtn_->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; font-weight: bold; }");
    layout->addWidget(calibrateBtn_);
}

void CameraCalibDialog::connectSignals()
{
    // 参数变更
    connect(patternTypeCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &CameraCalibDialog::onPatternTypeChanged);
    connect(boardWidthSpin_, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &CameraCalibDialog::onBoardSizeChanged);
    connect(boardHeightSpin_, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &CameraCalibDialog::onBoardSizeChanged);
    connect(squareSizeSpin_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &CameraCalibDialog::onSquareSizeChanged);
    connect(backendCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &CameraCalibDialog::onBackendChanged);

    // 图像管理
    connect(addImageBtn_, &QPushButton::clicked, this, &CameraCalibDialog::onAddCurrentImage);
    connect(loadFolderBtn_, &QPushButton::clicked, this, &CameraCalibDialog::onLoadImagesFromFolder);
    connect(removeImageBtn_, &QPushButton::clicked, this, &CameraCalibDialog::onRemoveSelectedImages);
    connect(clearAllBtn_, &QPushButton::clicked, this, &CameraCalibDialog::onClearAllImages);
    connect(imageList_, &QListWidget::currentRowChanged, this, &CameraCalibDialog::onImageSelected);

    // 检测与标定
    connect(detectBtn_, &QPushButton::clicked, this, &CameraCalibDialog::onDetectCorners);
    connect(calibrateBtn_, &QPushButton::clicked, this, &CameraCalibDialog::onExecuteCalibration);
    connect(saveResultBtn_, &QPushButton::clicked, this, &CameraCalibDialog::onSaveResult);

    // 对话框按钮
    connect(okBtn_, &QPushButton::clicked, this, &CameraCalibDialog::onOkClicked);
    connect(cancelBtn_, &QPushButton::clicked, this, &CameraCalibDialog::onCancelClicked);
    connect(applyBtn_, &QPushButton::clicked, this, &CameraCalibDialog::onApplyClicked);

    // 标定工具信号
    connect(calibTool_, &Algorithm::CameraCalibTool::cornersDetected,
            this, [this](bool success, int count) {
        if (success) {
            LOG_INFO(QString("检测到 %1 个角点").arg(count));
        }
    });

    connect(calibTool_, &Algorithm::CameraCalibTool::calibrationFinished,
            this, [this](bool success, double rmsError) {
        progressBar_->setVisible(false);
        if (success) {
            QMessageBox::information(this, "标定完成",
                QString("相机标定成功！\n重投影误差: %.4f 像素").arg(rmsError));
            updateResultDisplay();
            saveResultBtn_->setEnabled(true);
        } else {
            QMessageBox::warning(this, "标定失败", "相机标定失败，请检查图像质量和参数设置");
        }
    });
}

void CameraCalibDialog::updateUI()
{
    applyParameters();
    updateImageList();
    updateResultDisplay();
}

void CameraCalibDialog::updateImageList()
{
    imageList_->clear();
    int count = calibTool_->imageCount();

    for (int i = 0; i < count; ++i) {
        auto corners = calibTool_->getImageCorners(i);
        QString text = QString("图像 %1 - %2 个角点")
            .arg(i + 1)
            .arg(corners.size());
        imageList_->addItem(text);
    }

    imageCountLabel_->setText(QString("已采集: %1 张").arg(count));

    // 更新按钮状态
    bool hasImages = count > 0;
    removeImageBtn_->setEnabled(hasImages);
    clearAllBtn_->setEnabled(hasImages);
    calibrateBtn_->setEnabled(count >= minImageCountSpin_->value());
}

void CameraCalibDialog::updateResultDisplay()
{
    if (calibTool_->isCalibrated()) {
        const auto& result = calibTool_->getResult();

        rmsErrorLabel_->setText(QString("%1 像素").arg(result.rmsError(), 0, 'f', 4));

        cv::Point2d focal = result.focalLength();
        focalLengthLabel_->setText(QString("%.2f, %.2f").arg(focal.x).arg(focal.y));

        cv::Point2d pp = result.principalPoint();
        principalPointLabel_->setText(QString("%.2f, %.2f").arg(pp.x).arg(pp.y));

        cv::Size imgSize = result.imageSize();
        imagesSizeLabel_->setText(QString("%1 x %2").arg(imgSize.width).arg(imgSize.height));

        saveResultBtn_->setEnabled(true);
    } else {
        rmsErrorLabel_->setText("--");
        focalLengthLabel_->setText("--");
        principalPointLabel_->setText("--");
        imagesSizeLabel_->setText("--");
        saveResultBtn_->setEnabled(false);
    }
}

void CameraCalibDialog::applyParameters()
{
    // 应用标定板参数
    calibTool_->setPatternType(
        static_cast<Algorithm::CalibPatternType>(patternTypeCombo_->currentData().toInt()));
    calibTool_->setBoardSize(boardWidthSpin_->value(), boardHeightSpin_->value());
    calibTool_->setSquareSize(squareSizeSpin_->value());
    calibTool_->setBackend(
        static_cast<Algorithm::CalibBackend>(backendCombo_->currentData().toInt()));

    // 应用检测参数
    calibTool_->setSubPixelRefinement(subPixelCheck_->isChecked());
    calibTool_->setAdaptiveThreshold(adaptiveThreshCheck_->isChecked());
    calibTool_->setFastCheck(fastCheckCheck_->isChecked());
    calibTool_->setMinImageCount(minImageCountSpin_->value());
}

void CameraCalibDialog::drawCornersOnPreview(const cv::Mat& image,
                                              const std::vector<cv::Point2f>& corners,
                                              bool found)
{
    cv::Mat display;
    if (image.channels() == 1) {
        cv::cvtColor(image, display, cv::COLOR_GRAY2BGR);
    } else {
        display = image.clone();
    }

    cv::drawChessboardCorners(display, calibTool_->boardSize(), corners, found);

    auto imageData = std::make_shared<Base::ImageData>(display);
    imageViewer_->setImage(imageData);
}

// ========== 图像管理槽函数 ==========

void CameraCalibDialog::onAddCurrentImage()
{
    if (!currentImage_ || currentImage_->isEmpty()) {
        QMessageBox::warning(this, "警告", "请先加载或采集图像");
        return;
    }

    applyParameters();

    if (calibTool_->addCalibImage(currentImage_->mat())) {
        updateImageList();
        LOG_INFO("添加标定图像成功");
    } else {
        QMessageBox::warning(this, "添加失败", "未能在图像中检测到角点，请调整图像或参数");
    }
}

void CameraCalibDialog::onLoadImagesFromFolder()
{
    QString dir = QFileDialog::getExistingDirectory(this, "选择图像文件夹");
    if (dir.isEmpty()) return;

    applyParameters();

    QDir folder(dir);
    QStringList filters;
    filters << "*.jpg" << "*.jpeg" << "*.png" << "*.bmp" << "*.tif" << "*.tiff";
    QFileInfoList files = folder.entryInfoList(filters, QDir::Files);

    int successCount = 0;
    progressBar_->setVisible(true);
    progressBar_->setRange(0, files.size());

    for (int i = 0; i < files.size(); ++i) {
        progressBar_->setValue(i + 1);
        QApplication::processEvents();

        cv::Mat image = cv::imread(files[i].absoluteFilePath().toStdString());
        if (!image.empty()) {
            if (calibTool_->addCalibImage(image)) {
                successCount++;
            }
        }
    }

    progressBar_->setVisible(false);
    updateImageList();

    QMessageBox::information(this, "加载完成",
        QString("共加载 %1 张图像\n成功检测角点: %2 张")
        .arg(files.size()).arg(successCount));
}

void CameraCalibDialog::onRemoveSelectedImages()
{
    QList<QListWidgetItem*> selected = imageList_->selectedItems();
    if (selected.isEmpty()) return;

    // 从后往前删除，避免索引问题
    QList<int> indices;
    for (auto item : selected) {
        indices.append(imageList_->row(item));
    }
    std::sort(indices.begin(), indices.end(), std::greater<int>());

    for (int index : indices) {
        calibTool_->removeCalibImage(index);
    }

    updateImageList();
}

void CameraCalibDialog::onClearAllImages()
{
    if (QMessageBox::question(this, "确认", "确定要清除所有标定图像吗？")
        == QMessageBox::Yes) {
        calibTool_->clearImages();
        updateImageList();
    }
}

void CameraCalibDialog::onImageSelected(int index)
{
    if (index < 0 || index >= calibTool_->imageCount()) return;

    cv::Mat image = calibTool_->getImage(index);
    auto corners = calibTool_->getImageCorners(index);

    if (!image.empty()) {
        drawCornersOnPreview(image, corners, !corners.empty());
    }
}

// ========== 检测与标定槽函数 ==========

void CameraCalibDialog::onDetectCorners()
{
    if (!currentImage_ || currentImage_->isEmpty()) {
        QMessageBox::warning(this, "警告", "请先加载或采集图像");
        return;
    }

    applyParameters();

    std::vector<cv::Point2f> corners;
    bool found = calibTool_->detectCorners(currentImage_->mat(), corners);

    drawCornersOnPreview(currentImage_->mat(), corners, found);

    if (found) {
        QMessageBox::information(this, "检测成功",
            QString("检测到 %1 个角点").arg(corners.size()));
    } else {
        QMessageBox::warning(this, "检测失败", "未能检测到角点，请检查图像或参数设置");
    }
}

void CameraCalibDialog::onExecuteCalibration()
{
    if (calibTool_->imageCount() < minImageCountSpin_->value()) {
        QMessageBox::warning(this, "警告",
            QString("标定图像不足，需要至少 %1 张").arg(minImageCountSpin_->value()));
        return;
    }

    applyParameters();
    progressBar_->setVisible(true);
    progressBar_->setRange(0, 0);  // 不确定进度

    Algorithm::CalibrationResult result;
    calibTool_->calibrate(result);
}

// ========== 参数变更槽函数 ==========

void CameraCalibDialog::onPatternTypeChanged(int index)
{
    Q_UNUSED(index);
    applyParameters();
}

void CameraCalibDialog::onBoardSizeChanged()
{
    applyParameters();
}

void CameraCalibDialog::onSquareSizeChanged(double value)
{
    Q_UNUSED(value);
    applyParameters();
}

void CameraCalibDialog::onBackendChanged(int index)
{
    Q_UNUSED(index);
    applyParameters();
}

// ========== 对话框操作槽函数 ==========

void CameraCalibDialog::onOkClicked()
{
    if (calibTool_->isCalibrated()) {
        emit calibrationCompleted(calibTool_->getResult());
    }
    accept();
}

void CameraCalibDialog::onCancelClicked()
{
    reject();
}

void CameraCalibDialog::onApplyClicked()
{
    if (calibTool_->isCalibrated()) {
        emit calibrationCompleted(calibTool_->getResult());
    }
}

void CameraCalibDialog::onSaveResult()
{
    if (!calibTool_->isCalibrated()) {
        QMessageBox::warning(this, "警告", "尚未完成标定");
        return;
    }

    QString filePath = QFileDialog::getSaveFileName(this,
        "保存标定结果", "", "JSON文件 (*.json)");

    if (filePath.isEmpty()) return;

    if (!filePath.endsWith(".json", Qt::CaseInsensitive)) {
        filePath += ".json";
    }

    if (calibTool_->getResult().saveToFile(filePath)) {
        QMessageBox::information(this, "保存成功",
            QString("标定结果已保存到:\n%1").arg(filePath));
    } else {
        QMessageBox::warning(this, "保存失败", "无法保存标定结果文件");
    }
}

} // namespace UI
} // namespace VisionForge
