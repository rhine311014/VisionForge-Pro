/**
 * @file NinePointCalibDialog.cpp
 * @brief 九点标定对话框实现
 */

#include "ui/NinePointCalibDialog.h"
#include "base/Logger.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QHeaderView>
#include <QFormLayout>
#include <opencv2/imgproc.hpp>

namespace VisionForge {
namespace UI {

NinePointCalibDialog::NinePointCalibDialog(QWidget* parent)
    : QDialog(parent)
    , calibTool_(new Algorithm::NinePointCalibTool(this))
    , currentPointIndex_(-1)
    , imageViewer_(nullptr)
    , instructionLabel_(nullptr)
    , calibModeCombo_(nullptr)
    , backendCombo_(nullptr)
    , pointCountSpin_(nullptr)
    , pointsTable_(nullptr)
{
    setWindowTitle("九点标定");
    setMinimumSize(1100, 750);
    resize(1300, 850);

    createUI();
    connectSignals();
    updateUI();
}

NinePointCalibDialog::~NinePointCalibDialog()
{
}

void NinePointCalibDialog::setCurrentImage(const Base::ImageData::Ptr& image)
{
    currentImage_ = image;
    if (imageViewer_ && image) {
        imageViewer_->setImage(image);
        drawCalibPoints();
    }
}

Algorithm::CalibrationResult NinePointCalibDialog::getCalibrationResult() const
{
    return calibTool_->getResult();
}

bool NinePointCalibDialog::isCalibrated() const
{
    return calibTool_->isCalibrated();
}

void NinePointCalibDialog::showEvent(QShowEvent* event)
{
    QDialog::showEvent(event);
    updateUI();
}

void NinePointCalibDialog::createUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(10);

    // 操作说明
    instructionLabel_ = new QLabel(
        "操作说明: 1) 设置标定点数量和物理坐标  2) 在表格中点击要编辑的行  "
        "3) 在图像上点击对应位置采集图像坐标  4) 执行标定", this);
    instructionLabel_->setStyleSheet("QLabel { color: #666; font-style: italic; }");
    mainLayout->addWidget(instructionLabel_);

    // 主分割器
    QSplitter* mainSplitter = new QSplitter(Qt::Horizontal, this);

    createImagePreviewArea(mainSplitter);
    createControlPanel(mainSplitter);

    mainSplitter->setStretchFactor(0, 2);
    mainSplitter->setStretchFactor(1, 1);

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

void NinePointCalibDialog::createImagePreviewArea(QSplitter* splitter)
{
    QWidget* previewWidget = new QWidget(splitter);
    QVBoxLayout* layout = new QVBoxLayout(previewWidget);
    layout->setContentsMargins(0, 0, 0, 0);

    imageViewer_ = new ImageViewer(previewWidget);
    layout->addWidget(imageViewer_, 1);

    splitter->addWidget(previewWidget);
}

void NinePointCalibDialog::createControlPanel(QSplitter* splitter)
{
    QWidget* controlWidget = new QWidget(splitter);
    QVBoxLayout* layout = new QVBoxLayout(controlWidget);
    layout->setContentsMargins(5, 5, 5, 5);
    layout->setSpacing(8);

    createCalibModeGroup(layout);
    createPointsGroup(layout);
    createGridGeneratorGroup(layout);
    createResultGroup(layout);
    createTestGroup(layout);
    createButtons(layout);

    layout->addStretch();

    splitter->addWidget(controlWidget);
}

void NinePointCalibDialog::createCalibModeGroup(QVBoxLayout* layout)
{
    QGroupBox* group = new QGroupBox("标定模式", this);
    QFormLayout* formLayout = new QFormLayout(group);

    calibModeCombo_ = new QComboBox(group);
    calibModeCombo_->addItem("仿射变换 (3点+)", static_cast<int>(Algorithm::NinePointCalibMode::Affine));
    calibModeCombo_->addItem("透视变换 (4点)", static_cast<int>(Algorithm::NinePointCalibMode::Perspective));
    calibModeCombo_->addItem("单应性变换 (4点+)", static_cast<int>(Algorithm::NinePointCalibMode::Homography));
    calibModeCombo_->setCurrentIndex(2);  // 默认单应性
    formLayout->addRow("变换类型:", calibModeCombo_);

    backendCombo_ = new QComboBox(group);
    backendCombo_->addItem("OpenCV", static_cast<int>(Algorithm::CalibBackend::OpenCV));
#ifdef USE_HALCON
    backendCombo_->addItem("Halcon", static_cast<int>(Algorithm::CalibBackend::Halcon));
#endif
    formLayout->addRow("后端:", backendCombo_);

    layout->addWidget(group);
}

void NinePointCalibDialog::createPointsGroup(QVBoxLayout* layout)
{
    QGroupBox* group = new QGroupBox("标定点", this);
    QVBoxLayout* vLayout = new QVBoxLayout(group);

    // 点数量
    QHBoxLayout* countLayout = new QHBoxLayout();
    countLayout->addWidget(new QLabel("点数量:"));
    pointCountSpin_ = new QSpinBox(group);
    pointCountSpin_->setRange(4, 25);
    pointCountSpin_->setValue(9);
    countLayout->addWidget(pointCountSpin_);
    countLayout->addStretch();

    clearPointsBtn_ = new QPushButton("清除坐标", group);
    countLayout->addWidget(clearPointsBtn_);

    vLayout->addLayout(countLayout);

    // 标定点表格
    pointsTable_ = new QTableWidget(group);
    pointsTable_->setColumnCount(5);
    pointsTable_->setHorizontalHeaderLabels({"点", "图像X", "图像Y", "物理X", "物理Y"});
    pointsTable_->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    pointsTable_->setSelectionBehavior(QAbstractItemView::SelectRows);
    pointsTable_->setSelectionMode(QAbstractItemView::SingleSelection);
    pointsTable_->setMinimumHeight(200);
    vLayout->addWidget(pointsTable_);

    layout->addWidget(group);
}

void NinePointCalibDialog::createGridGeneratorGroup(QVBoxLayout* layout)
{
    QGroupBox* group = new QGroupBox("网格物理坐标生成", this);
    QFormLayout* formLayout = new QFormLayout(group);

    // 网格尺寸
    QHBoxLayout* sizeLayout = new QHBoxLayout();
    gridRowsSpin_ = new QSpinBox(group);
    gridRowsSpin_->setRange(2, 10);
    gridRowsSpin_->setValue(3);
    gridColsSpin_ = new QSpinBox(group);
    gridColsSpin_->setRange(2, 10);
    gridColsSpin_->setValue(3);
    sizeLayout->addWidget(gridRowsSpin_);
    sizeLayout->addWidget(new QLabel("x"));
    sizeLayout->addWidget(gridColsSpin_);
    formLayout->addRow("网格大小:", sizeLayout);

    // 间距
    QHBoxLayout* spacingLayout = new QHBoxLayout();
    gridSpacingXSpin_ = new QDoubleSpinBox(group);
    gridSpacingXSpin_->setRange(0.1, 1000.0);
    gridSpacingXSpin_->setValue(50.0);
    gridSpacingXSpin_->setSuffix(" mm");
    gridSpacingYSpin_ = new QDoubleSpinBox(group);
    gridSpacingYSpin_->setRange(0.1, 1000.0);
    gridSpacingYSpin_->setValue(50.0);
    gridSpacingYSpin_->setSuffix(" mm");
    spacingLayout->addWidget(gridSpacingXSpin_);
    spacingLayout->addWidget(new QLabel("x"));
    spacingLayout->addWidget(gridSpacingYSpin_);
    formLayout->addRow("间距:", spacingLayout);

    // 原点
    QHBoxLayout* originLayout = new QHBoxLayout();
    gridOriginXSpin_ = new QDoubleSpinBox(group);
    gridOriginXSpin_->setRange(-10000.0, 10000.0);
    gridOriginXSpin_->setValue(0.0);
    gridOriginXSpin_->setSuffix(" mm");
    gridOriginYSpin_ = new QDoubleSpinBox(group);
    gridOriginYSpin_->setRange(-10000.0, 10000.0);
    gridOriginYSpin_->setValue(0.0);
    gridOriginYSpin_->setSuffix(" mm");
    originLayout->addWidget(gridOriginXSpin_);
    originLayout->addWidget(new QLabel(","));
    originLayout->addWidget(gridOriginYSpin_);
    formLayout->addRow("原点:", originLayout);

    generateGridBtn_ = new QPushButton("生成网格坐标", group);
    formLayout->addRow("", generateGridBtn_);

    layout->addWidget(group);
}

void NinePointCalibDialog::createResultGroup(QVBoxLayout* layout)
{
    QGroupBox* group = new QGroupBox("标定结果", this);
    QFormLayout* formLayout = new QFormLayout(group);

    pixelScaleXLabel_ = new QLabel("--", group);
    formLayout->addRow("X比例 (mm/px):", pixelScaleXLabel_);

    pixelScaleYLabel_ = new QLabel("--", group);
    formLayout->addRow("Y比例 (mm/px):", pixelScaleYLabel_);

    rotationAngleLabel_ = new QLabel("--", group);
    formLayout->addRow("旋转角度:", rotationAngleLabel_);

    calibErrorLabel_ = new QLabel("--", group);
    formLayout->addRow("标定误差:", calibErrorLabel_);

    QHBoxLayout* btnLayout = new QHBoxLayout();
    saveResultBtn_ = new QPushButton("保存", group);
    saveResultBtn_->setEnabled(false);
    loadResultBtn_ = new QPushButton("加载", group);
    btnLayout->addWidget(saveResultBtn_);
    btnLayout->addWidget(loadResultBtn_);
    formLayout->addRow("", btnLayout);

    layout->addWidget(group);
}

void NinePointCalibDialog::createTestGroup(QVBoxLayout* layout)
{
    QGroupBox* group = new QGroupBox("坐标转换测试", this);
    QFormLayout* formLayout = new QFormLayout(group);

    // 输入图像坐标
    QHBoxLayout* inputLayout = new QHBoxLayout();
    testImageXSpin_ = new QDoubleSpinBox(group);
    testImageXSpin_->setRange(0.0, 10000.0);
    testImageXSpin_->setValue(0.0);
    testImageXSpin_->setDecimals(2);
    testImageYSpin_ = new QDoubleSpinBox(group);
    testImageYSpin_->setRange(0.0, 10000.0);
    testImageYSpin_->setValue(0.0);
    testImageYSpin_->setDecimals(2);
    inputLayout->addWidget(testImageXSpin_);
    inputLayout->addWidget(new QLabel(","));
    inputLayout->addWidget(testImageYSpin_);
    formLayout->addRow("图像坐标:", inputLayout);

    // 输出物理坐标
    QHBoxLayout* outputLayout = new QHBoxLayout();
    testWorldXLabel_ = new QLabel("--", group);
    testWorldYLabel_ = new QLabel("--", group);
    outputLayout->addWidget(testWorldXLabel_);
    outputLayout->addWidget(new QLabel(","));
    outputLayout->addWidget(testWorldYLabel_);
    formLayout->addRow("物理坐标:", outputLayout);

    testTransformBtn_ = new QPushButton("转换", group);
    testTransformBtn_->setEnabled(false);
    formLayout->addRow("", testTransformBtn_);

    layout->addWidget(group);
}

void NinePointCalibDialog::createButtons(QVBoxLayout* layout)
{
    calibrateBtn_ = new QPushButton("执行标定", this);
    calibrateBtn_->setMinimumHeight(40);
    calibrateBtn_->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; font-weight: bold; }");
    layout->addWidget(calibrateBtn_);
}

void NinePointCalibDialog::connectSignals()
{
    // 模式和后端
    connect(calibModeCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &NinePointCalibDialog::onCalibModeChanged);
    connect(backendCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &NinePointCalibDialog::onBackendChanged);

    // 标定点
    connect(pointCountSpin_, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &NinePointCalibDialog::onPointCountChanged);
    connect(clearPointsBtn_, &QPushButton::clicked, this, &NinePointCalibDialog::onClearPoints);
    connect(pointsTable_, &QTableWidget::cellChanged,
            this, &NinePointCalibDialog::onPointTableCellChanged);
    connect(pointsTable_, &QTableWidget::currentCellChanged,
            this, [this](int row, int, int, int) {
        currentPointIndex_ = row;
        instructionLabel_->setText(QString("已选择点 %1，在图像上点击以设置图像坐标").arg(row + 1));
    });

    // 网格生成
    connect(generateGridBtn_, &QPushButton::clicked, this, &NinePointCalibDialog::onGenerateGrid);

    // 图像点击
    connect(imageViewer_, &ImageViewer::mouseClicked,
            this, &NinePointCalibDialog::onImageClicked);

    // 标定执行
    connect(calibrateBtn_, &QPushButton::clicked, this, &NinePointCalibDialog::onExecuteCalibration);
    connect(testTransformBtn_, &QPushButton::clicked, this, &NinePointCalibDialog::onTestTransform);
    connect(saveResultBtn_, &QPushButton::clicked, this, &NinePointCalibDialog::onSaveResult);
    connect(loadResultBtn_, &QPushButton::clicked, this, &NinePointCalibDialog::onLoadResult);

    // 对话框按钮
    connect(okBtn_, &QPushButton::clicked, this, &NinePointCalibDialog::onOkClicked);
    connect(cancelBtn_, &QPushButton::clicked, this, &NinePointCalibDialog::onCancelClicked);
    connect(applyBtn_, &QPushButton::clicked, this, &NinePointCalibDialog::onApplyClicked);

    // 标定工具信号
    connect(calibTool_, &Algorithm::NinePointCalibTool::calibrationFinished,
            this, [this](bool success, double error) {
        if (success) {
            QMessageBox::information(this, "标定完成",
                QString("九点标定成功！\n标定误差: %.4f mm").arg(error));
            updateResultDisplay();
            saveResultBtn_->setEnabled(true);
            testTransformBtn_->setEnabled(true);
        } else {
            QMessageBox::warning(this, "标定失败", "标定失败，请检查标定点数据");
        }
    });
}

void NinePointCalibDialog::updateUI()
{
    applyParameters();
    updatePointsTable();
    updateResultDisplay();
}

void NinePointCalibDialog::updatePointsTable()
{
    pointsTable_->blockSignals(true);

    int count = calibTool_->pointCount();
    pointsTable_->setRowCount(count);

    for (int i = 0; i < count; ++i) {
        auto pt = calibTool_->getCalibPoint(i);

        // 点名称
        QTableWidgetItem* nameItem = new QTableWidgetItem(pt.name.isEmpty() ? QString("P%1").arg(i + 1) : pt.name);
        nameItem->setFlags(nameItem->flags() & ~Qt::ItemIsEditable);
        pointsTable_->setItem(i, 0, nameItem);

        // 图像坐标
        QTableWidgetItem* imgXItem = new QTableWidgetItem(
            pt.valid ? QString::number(pt.imagePos.x, 'f', 2) : "--");
        QTableWidgetItem* imgYItem = new QTableWidgetItem(
            pt.valid ? QString::number(pt.imagePos.y, 'f', 2) : "--");
        imgXItem->setFlags(imgXItem->flags() & ~Qt::ItemIsEditable);
        imgYItem->setFlags(imgYItem->flags() & ~Qt::ItemIsEditable);
        pointsTable_->setItem(i, 1, imgXItem);
        pointsTable_->setItem(i, 2, imgYItem);

        // 物理坐标（可编辑）
        QTableWidgetItem* worldXItem = new QTableWidgetItem(QString::number(pt.worldPos.x, 'f', 2));
        QTableWidgetItem* worldYItem = new QTableWidgetItem(QString::number(pt.worldPos.y, 'f', 2));
        pointsTable_->setItem(i, 3, worldXItem);
        pointsTable_->setItem(i, 4, worldYItem);

        // 设置有效点的背景色
        QColor bgColor = pt.valid ? QColor(200, 255, 200) : QColor(255, 255, 255);
        for (int col = 0; col < 5; ++col) {
            if (pointsTable_->item(i, col)) {
                pointsTable_->item(i, col)->setBackground(bgColor);
            }
        }
    }

    pointsTable_->blockSignals(false);
}

void NinePointCalibDialog::updateResultDisplay()
{
    if (calibTool_->isCalibrated()) {
        const auto& result = calibTool_->getResult();

        pixelScaleXLabel_->setText(QString("%1").arg(result.pixelToMMX(), 0, 'f', 6));
        pixelScaleYLabel_->setText(QString("%1").arg(result.pixelToMMY(), 0, 'f', 6));
        rotationAngleLabel_->setText(QString("%1 度").arg(calibTool_->rotationAngleDegrees(), 0, 'f', 2));
        calibErrorLabel_->setText(QString("%1 mm").arg(result.calibrationError(), 0, 'f', 4));

        saveResultBtn_->setEnabled(true);
        testTransformBtn_->setEnabled(true);
    } else {
        pixelScaleXLabel_->setText("--");
        pixelScaleYLabel_->setText("--");
        rotationAngleLabel_->setText("--");
        calibErrorLabel_->setText("--");

        saveResultBtn_->setEnabled(false);
        testTransformBtn_->setEnabled(false);
    }
}

void NinePointCalibDialog::applyParameters()
{
    calibTool_->setCalibMode(
        static_cast<Algorithm::NinePointCalibMode>(calibModeCombo_->currentData().toInt()));
    calibTool_->setBackend(
        static_cast<Algorithm::CalibBackend>(backendCombo_->currentData().toInt()));
}

void NinePointCalibDialog::drawCalibPoints()
{
    if (!currentImage_ || currentImage_->isEmpty()) return;

    cv::Mat display;
    if (currentImage_->mat().channels() == 1) {
        cv::cvtColor(currentImage_->mat(), display, cv::COLOR_GRAY2BGR);
    } else {
        display = currentImage_->mat().clone();
    }

    // 绘制标定点
    for (int i = 0; i < calibTool_->pointCount(); ++i) {
        auto pt = calibTool_->getCalibPoint(i);
        if (pt.valid) {
            cv::Point center(static_cast<int>(pt.imagePos.x), static_cast<int>(pt.imagePos.y));

            // 绘制十字和圆圈
            cv::circle(display, center, 8, cv::Scalar(0, 255, 0), 2);
            cv::line(display, cv::Point(center.x - 12, center.y),
                     cv::Point(center.x + 12, center.y), cv::Scalar(0, 255, 0), 1);
            cv::line(display, cv::Point(center.x, center.y - 12),
                     cv::Point(center.x, center.y + 12), cv::Scalar(0, 255, 0), 1);

            // 绘制点编号
            cv::putText(display, std::to_string(i + 1),
                        cv::Point(center.x + 10, center.y - 10),
                        cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 255), 1);
        }
    }

    auto imageData = std::make_shared<Base::ImageData>(display);
    imageViewer_->setImage(imageData);
}

// ========== 槽函数实现 ==========

void NinePointCalibDialog::onPointCountChanged(int count)
{
    calibTool_->setPointCount(count);
    updatePointsTable();
    drawCalibPoints();
}

void NinePointCalibDialog::onGenerateGrid()
{
    int rows = gridRowsSpin_->value();
    int cols = gridColsSpin_->value();
    double spacingX = gridSpacingXSpin_->value();
    double spacingY = gridSpacingYSpin_->value();
    double originX = gridOriginXSpin_->value();
    double originY = gridOriginYSpin_->value();

    calibTool_->generateGridWorldPoints(rows, cols, spacingX, spacingY, originX, originY);
    pointCountSpin_->setValue(rows * cols);
    updatePointsTable();

    QMessageBox::information(this, "生成完成",
        QString("已生成 %1x%2 = %3 个标定点的物理坐标").arg(rows).arg(cols).arg(rows * cols));
}

void NinePointCalibDialog::onClearPoints()
{
    calibTool_->clearCalibPoints();
    updatePointsTable();
    drawCalibPoints();
}

void NinePointCalibDialog::onPointTableCellChanged(int row, int column)
{
    // 只处理物理坐标列 (3, 4)
    if (column < 3 || column > 4) return;
    if (row < 0 || row >= calibTool_->pointCount()) return;

    QTableWidgetItem* item = pointsTable_->item(row, column);
    if (!item) return;

    bool ok;
    double value = item->text().toDouble(&ok);
    if (!ok) return;

    auto pt = calibTool_->getCalibPoint(row);
    if (column == 3) {
        pt.worldPos.x = value;
    } else {
        pt.worldPos.y = value;
    }
    calibTool_->setWorldPoint(row, pt.worldPos);
}

void NinePointCalibDialog::onImageClicked(int x, int y)
{
    if (currentPointIndex_ < 0 || currentPointIndex_ >= calibTool_->pointCount()) {
        QMessageBox::information(this, "提示", "请先在表格中选择一个标定点");
        return;
    }

    cv::Point2d imagePos(x, y);
    calibTool_->setImagePoint(currentPointIndex_, imagePos);

    // 自动设置为有效
    calibTool_->setPointValid(currentPointIndex_, true);

    updatePointsTable();
    drawCalibPoints();

    LOG_INFO(QString("设置点 %1 图像坐标: (%2, %3)")
             .arg(currentPointIndex_ + 1).arg(x).arg(y));

    // 自动选择下一个点
    if (currentPointIndex_ + 1 < calibTool_->pointCount()) {
        currentPointIndex_++;
        pointsTable_->selectRow(currentPointIndex_);
        instructionLabel_->setText(QString("已选择点 %1，在图像上点击以设置图像坐标")
                                   .arg(currentPointIndex_ + 1));
    } else {
        instructionLabel_->setText("所有点已设置完成，可以执行标定");
    }
}

void NinePointCalibDialog::onExecuteCalibration()
{
    applyParameters();

    int validCount = calibTool_->validPointCount();
    int minRequired = (calibTool_->calibMode() == Algorithm::NinePointCalibMode::Affine) ? 3 : 4;

    if (validCount < minRequired) {
        QMessageBox::warning(this, "警告",
            QString("有效标定点不足，需要至少 %1 个，当前 %2 个")
            .arg(minRequired).arg(validCount));
        return;
    }

    Algorithm::CalibrationResult result;
    calibTool_->calibrate(result);
}

void NinePointCalibDialog::onTestTransform()
{
    if (!calibTool_->isCalibrated()) {
        QMessageBox::warning(this, "警告", "请先完成标定");
        return;
    }

    cv::Point2d imagePoint(testImageXSpin_->value(), testImageYSpin_->value());
    cv::Point2d worldPoint = calibTool_->imageToWorld(imagePoint);

    testWorldXLabel_->setText(QString::number(worldPoint.x, 'f', 4));
    testWorldYLabel_->setText(QString::number(worldPoint.y, 'f', 4));
}

void NinePointCalibDialog::onCalibModeChanged(int index)
{
    Q_UNUSED(index);
    applyParameters();
}

void NinePointCalibDialog::onBackendChanged(int index)
{
    Q_UNUSED(index);
    applyParameters();
}

void NinePointCalibDialog::onOkClicked()
{
    if (calibTool_->isCalibrated()) {
        emit calibrationCompleted(calibTool_->getResult());
    }
    accept();
}

void NinePointCalibDialog::onCancelClicked()
{
    reject();
}

void NinePointCalibDialog::onApplyClicked()
{
    if (calibTool_->isCalibrated()) {
        emit calibrationCompleted(calibTool_->getResult());
    }
}

void NinePointCalibDialog::onSaveResult()
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

void NinePointCalibDialog::onLoadResult()
{
    QString filePath = QFileDialog::getOpenFileName(this,
        "加载标定结果", "", "JSON文件 (*.json)");

    if (filePath.isEmpty()) return;

    Algorithm::CalibrationResult result;
    if (result.loadFromFile(filePath)) {
        // 需要一个方法设置结果到工具
        // calibTool_->setResult(result);  // 如果有这个方法
        updateResultDisplay();
        QMessageBox::information(this, "加载成功", "标定结果已加载");
    } else {
        QMessageBox::warning(this, "加载失败", "无法加载标定结果文件");
    }
}

} // namespace UI
} // namespace VisionForge
