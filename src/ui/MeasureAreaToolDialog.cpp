/**
 * @file MeasureAreaToolDialog.cpp
 * @brief 面积测量工具参数编辑对话框实现
 * @author VisionForge Team
 * @date 2025-12-18
 */

#include "ui/MeasureAreaToolDialog.h"
#include "ui/ImageViewer.h"
#include "algorithm/MeasureAreaTool.h"
#include "base/Logger.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QSplitter>
#include <QMessageBox>
#include <QHeaderView>

namespace VisionForge {
namespace UI {

MeasureAreaToolDialog::MeasureAreaToolDialog(Algorithm::MeasureAreaTool* tool, QWidget* parent)
    : QDialog(parent)
    , tool_(tool)
    , currentImage_(nullptr)
    , imageViewer_(nullptr)
    , measureModeCombo_(nullptr)
    , inputSourceCombo_(nullptr)
    , contourParamsGroup_(nullptr)
    , thresholdSpin_(nullptr)
    , minAreaSpin_(nullptr)
    , invertCheck_(nullptr)
    , circleParamsGroup_(nullptr)
    , circleCenterXSpin_(nullptr)
    , circleCenterYSpin_(nullptr)
    , circleRadiusSpin_(nullptr)
    , ellipseParamsGroup_(nullptr)
    , ellipseCenterXSpin_(nullptr)
    , ellipseCenterYSpin_(nullptr)
    , ellipseASpin_(nullptr)
    , ellipseBSpin_(nullptr)
    , ellipseAngleSpin_(nullptr)
    , pixelToMmSpin_(nullptr)
    , areaPixelLabel_(nullptr)
    , areaMmLabel_(nullptr)
    , perimeterLabel_(nullptr)
    , centroidLabel_(nullptr)
    , circularityLabel_(nullptr)
    , resultsTable_(nullptr)
    , previewBtn_(nullptr)
    , okBtn_(nullptr)
    , cancelBtn_(nullptr)
    , applyBtn_(nullptr)
{
    setWindowTitle("面积测量设置");
    setMinimumSize(900, 700);
    resize(1000, 750);
    setWindowFlags(windowFlags() | Qt::WindowMaximizeButtonHint);

    createUI();
    connectSignals();
    updateUI();
}

MeasureAreaToolDialog::~MeasureAreaToolDialog()
{
}

void MeasureAreaToolDialog::setImage(const Base::ImageData::Ptr& image)
{
    currentImage_ = image;
    if (imageViewer_ && image) {
        imageViewer_->setImage(image);
    }
}

void MeasureAreaToolDialog::updateUI()
{
    if (!tool_) return;

    // 阻塞信号
    measureModeCombo_->blockSignals(true);
    thresholdSpin_->blockSignals(true);
    minAreaSpin_->blockSignals(true);
    pixelToMmSpin_->blockSignals(true);

    // 更新测量模式
    measureModeCombo_->setCurrentIndex(static_cast<int>(tool_->measureMode()));

    // 更新轮廓参数
    thresholdSpin_->setValue(tool_->threshold());
    minAreaSpin_->setValue(tool_->minArea());

    // 更新圆形参数
    QPointF circleCenter = tool_->circleCenter();
    circleCenterXSpin_->setValue(circleCenter.x());
    circleCenterYSpin_->setValue(circleCenter.y());
    circleRadiusSpin_->setValue(tool_->circleRadius());

    // 更新像素转换系数
    pixelToMmSpin_->setValue(tool_->pixelToMm());

    // 恢复信号
    measureModeCombo_->blockSignals(false);
    thresholdSpin_->blockSignals(false);
    minAreaSpin_->blockSignals(false);
    pixelToMmSpin_->blockSignals(false);

    updateInputVisibility();
    updateResultDisplay();
}

void MeasureAreaToolDialog::showEvent(QShowEvent* event)
{
    QDialog::showEvent(event);
    updateUI();
}

void MeasureAreaToolDialog::createUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(8, 8, 8, 8);
    mainLayout->setSpacing(8);

    // 主分割器
    QSplitter* mainSplitter = new QSplitter(Qt::Horizontal, this);

    // 左侧 - 图像显示
    QWidget* leftPanel = new QWidget(mainSplitter);
    QVBoxLayout* leftLayout = new QVBoxLayout(leftPanel);
    leftLayout->setContentsMargins(0, 0, 0, 0);

    QLabel* titleLabel = new QLabel("图像预览", leftPanel);
    titleLabel->setStyleSheet("font-weight: bold; font-size: 12px;");

    imageViewer_ = new ImageViewer(leftPanel);
    imageViewer_->setMinimumSize(400, 300);

    leftLayout->addWidget(titleLabel);
    leftLayout->addWidget(imageViewer_, 1);

    // 右侧 - 参数设置
    QWidget* rightPanel = new QWidget(mainSplitter);
    QVBoxLayout* rightLayout = new QVBoxLayout(rightPanel);
    rightLayout->setContentsMargins(4, 0, 0, 0);
    rightLayout->setSpacing(8);

    createMeasureModeGroup(rightLayout);
    createInputSourceGroup(rightLayout);
    createContourParamsGroup(rightLayout);
    createCircleParamsGroup(rightLayout);
    createEllipseParamsGroup(rightLayout);
    createUnitConversionGroup(rightLayout);
    createResultGroup(rightLayout);
    createButtonGroup(rightLayout);

    rightLayout->addStretch();

    mainSplitter->addWidget(leftPanel);
    mainSplitter->addWidget(rightPanel);
    mainSplitter->setStretchFactor(0, 2);
    mainSplitter->setStretchFactor(1, 1);

    mainLayout->addWidget(mainSplitter, 1);

    // 底部按钮
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();

    okBtn_ = new QPushButton("确定", this);
    cancelBtn_ = new QPushButton("取消", this);
    applyBtn_ = new QPushButton("应用", this);

    okBtn_->setMinimumWidth(80);
    cancelBtn_->setMinimumWidth(80);
    applyBtn_->setMinimumWidth(80);

    buttonLayout->addWidget(okBtn_);
    buttonLayout->addWidget(cancelBtn_);
    buttonLayout->addWidget(applyBtn_);

    mainLayout->addLayout(buttonLayout);
}

void MeasureAreaToolDialog::createMeasureModeGroup(QVBoxLayout* layout)
{
    QGroupBox* group = new QGroupBox("测量模式", this);
    QFormLayout* formLayout = new QFormLayout(group);

    measureModeCombo_ = new QComboBox(group);
    measureModeCombo_->addItem("多边形", static_cast<int>(Algorithm::MeasureAreaTool::Polygon));
    measureModeCombo_->addItem("轮廓(二值图)", static_cast<int>(Algorithm::MeasureAreaTool::Contour));
    measureModeCombo_->addItem("圆形", static_cast<int>(Algorithm::MeasureAreaTool::Circle));
    measureModeCombo_->addItem("椭圆", static_cast<int>(Algorithm::MeasureAreaTool::Ellipse));

    formLayout->addRow("测量模式:", measureModeCombo_);

    layout->addWidget(group);
}

void MeasureAreaToolDialog::createInputSourceGroup(QVBoxLayout* layout)
{
    QGroupBox* group = new QGroupBox("输入源", this);
    QVBoxLayout* groupLayout = new QVBoxLayout(group);

    inputSourceCombo_ = new QComboBox(group);
    inputSourceCombo_->addItem("手动输入");
    inputSourceCombo_->addItem("从工具链获取");

    groupLayout->addWidget(inputSourceCombo_);

    layout->addWidget(group);
}

void MeasureAreaToolDialog::createContourParamsGroup(QVBoxLayout* layout)
{
    contourParamsGroup_ = new QGroupBox("轮廓检测参数", this);
    QFormLayout* formLayout = new QFormLayout(contourParamsGroup_);

    thresholdSpin_ = new QSpinBox(contourParamsGroup_);
    thresholdSpin_->setRange(0, 255);
    thresholdSpin_->setValue(128);
    formLayout->addRow("二值化阈值:", thresholdSpin_);

    minAreaSpin_ = new QDoubleSpinBox(contourParamsGroup_);
    minAreaSpin_->setRange(0, 10000000);
    minAreaSpin_->setValue(100);
    minAreaSpin_->setSuffix(" px");
    formLayout->addRow("最小面积:", minAreaSpin_);

    invertCheck_ = new QCheckBox("反转二值化", contourParamsGroup_);
    formLayout->addRow("", invertCheck_);

    layout->addWidget(contourParamsGroup_);
}

void MeasureAreaToolDialog::createCircleParamsGroup(QVBoxLayout* layout)
{
    circleParamsGroup_ = new QGroupBox("圆形参数", this);
    QGridLayout* gridLayout = new QGridLayout(circleParamsGroup_);

    gridLayout->addWidget(new QLabel("圆心 X:"), 0, 0);
    circleCenterXSpin_ = new QDoubleSpinBox(circleParamsGroup_);
    circleCenterXSpin_->setRange(-100000, 100000);
    circleCenterXSpin_->setDecimals(2);
    gridLayout->addWidget(circleCenterXSpin_, 0, 1);

    gridLayout->addWidget(new QLabel("Y:"), 0, 2);
    circleCenterYSpin_ = new QDoubleSpinBox(circleParamsGroup_);
    circleCenterYSpin_->setRange(-100000, 100000);
    circleCenterYSpin_->setDecimals(2);
    gridLayout->addWidget(circleCenterYSpin_, 0, 3);

    gridLayout->addWidget(new QLabel("半径:"), 1, 0);
    circleRadiusSpin_ = new QDoubleSpinBox(circleParamsGroup_);
    circleRadiusSpin_->setRange(0, 100000);
    circleRadiusSpin_->setDecimals(2);
    gridLayout->addWidget(circleRadiusSpin_, 1, 1);

    layout->addWidget(circleParamsGroup_);
}

void MeasureAreaToolDialog::createEllipseParamsGroup(QVBoxLayout* layout)
{
    ellipseParamsGroup_ = new QGroupBox("椭圆参数", this);
    QGridLayout* gridLayout = new QGridLayout(ellipseParamsGroup_);

    gridLayout->addWidget(new QLabel("中心 X:"), 0, 0);
    ellipseCenterXSpin_ = new QDoubleSpinBox(ellipseParamsGroup_);
    ellipseCenterXSpin_->setRange(-100000, 100000);
    ellipseCenterXSpin_->setDecimals(2);
    gridLayout->addWidget(ellipseCenterXSpin_, 0, 1);

    gridLayout->addWidget(new QLabel("Y:"), 0, 2);
    ellipseCenterYSpin_ = new QDoubleSpinBox(ellipseParamsGroup_);
    ellipseCenterYSpin_->setRange(-100000, 100000);
    ellipseCenterYSpin_->setDecimals(2);
    gridLayout->addWidget(ellipseCenterYSpin_, 0, 3);

    gridLayout->addWidget(new QLabel("长轴 a:"), 1, 0);
    ellipseASpin_ = new QDoubleSpinBox(ellipseParamsGroup_);
    ellipseASpin_->setRange(0, 100000);
    ellipseASpin_->setDecimals(2);
    gridLayout->addWidget(ellipseASpin_, 1, 1);

    gridLayout->addWidget(new QLabel("短轴 b:"), 1, 2);
    ellipseBSpin_ = new QDoubleSpinBox(ellipseParamsGroup_);
    ellipseBSpin_->setRange(0, 100000);
    ellipseBSpin_->setDecimals(2);
    gridLayout->addWidget(ellipseBSpin_, 1, 3);

    gridLayout->addWidget(new QLabel("旋转角度:"), 2, 0);
    ellipseAngleSpin_ = new QDoubleSpinBox(ellipseParamsGroup_);
    ellipseAngleSpin_->setRange(-180, 180);
    ellipseAngleSpin_->setDecimals(2);
    ellipseAngleSpin_->setSuffix(QString::fromUtf8("\u00B0"));
    gridLayout->addWidget(ellipseAngleSpin_, 2, 1);

    layout->addWidget(ellipseParamsGroup_);
}

void MeasureAreaToolDialog::createUnitConversionGroup(QVBoxLayout* layout)
{
    QGroupBox* group = new QGroupBox("单位转换", this);
    QFormLayout* formLayout = new QFormLayout(group);

    pixelToMmSpin_ = new QDoubleSpinBox(group);
    pixelToMmSpin_->setRange(0.0001, 100.0);
    pixelToMmSpin_->setDecimals(6);
    pixelToMmSpin_->setValue(1.0);
    pixelToMmSpin_->setSuffix(" mm/pixel");

    formLayout->addRow("像素比例:", pixelToMmSpin_);

    layout->addWidget(group);
}

void MeasureAreaToolDialog::createResultGroup(QVBoxLayout* layout)
{
    QGroupBox* group = new QGroupBox("测量结果", this);
    QVBoxLayout* groupLayout = new QVBoxLayout(group);

    QFormLayout* formLayout = new QFormLayout();

    areaPixelLabel_ = new QLabel("--", group);
    areaMmLabel_ = new QLabel("--", group);
    perimeterLabel_ = new QLabel("--", group);
    centroidLabel_ = new QLabel("--", group);
    circularityLabel_ = new QLabel("--", group);

    formLayout->addRow("面积(像素):", areaPixelLabel_);
    formLayout->addRow("面积(mm2):", areaMmLabel_);
    formLayout->addRow("周长:", perimeterLabel_);
    formLayout->addRow("质心:", centroidLabel_);
    formLayout->addRow("圆度:", circularityLabel_);

    groupLayout->addLayout(formLayout);

    // 多轮廓结果表格
    resultsTable_ = new QTableWidget(group);
    resultsTable_->setColumnCount(5);
    resultsTable_->setHorizontalHeaderLabels({"序号", "面积(px)", "周长", "质心", "圆度"});
    resultsTable_->horizontalHeader()->setStretchLastSection(true);
    resultsTable_->setMinimumHeight(100);
    resultsTable_->setVisible(false);

    groupLayout->addWidget(resultsTable_);

    layout->addWidget(group);
}

void MeasureAreaToolDialog::createButtonGroup(QVBoxLayout* layout)
{
    QHBoxLayout* btnLayout = new QHBoxLayout();

    previewBtn_ = new QPushButton("预览测量", this);
    previewBtn_->setMinimumHeight(35);
    previewBtn_->setStyleSheet("QPushButton { background-color: #2196F3; color: white; font-weight: bold; }");

    btnLayout->addWidget(previewBtn_);

    layout->addLayout(btnLayout);
}

void MeasureAreaToolDialog::connectSignals()
{
    // 参数变化
    connect(measureModeCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MeasureAreaToolDialog::onMeasureModeChanged);
    connect(inputSourceCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MeasureAreaToolDialog::onInputSourceChanged);
    connect(thresholdSpin_, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &MeasureAreaToolDialog::onThresholdChanged);
    connect(minAreaSpin_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MeasureAreaToolDialog::onMinAreaChanged);
    connect(pixelToMmSpin_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MeasureAreaToolDialog::onPixelToMmChanged);

    connect(circleCenterXSpin_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MeasureAreaToolDialog::onCircleParamsChanged);
    connect(circleCenterYSpin_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MeasureAreaToolDialog::onCircleParamsChanged);
    connect(circleRadiusSpin_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MeasureAreaToolDialog::onCircleParamsChanged);

    connect(ellipseCenterXSpin_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MeasureAreaToolDialog::onEllipseParamsChanged);
    connect(ellipseCenterYSpin_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MeasureAreaToolDialog::onEllipseParamsChanged);
    connect(ellipseASpin_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MeasureAreaToolDialog::onEllipseParamsChanged);
    connect(ellipseBSpin_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MeasureAreaToolDialog::onEllipseParamsChanged);
    connect(ellipseAngleSpin_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MeasureAreaToolDialog::onEllipseParamsChanged);

    // 按钮
    connect(previewBtn_, &QPushButton::clicked, this, &MeasureAreaToolDialog::onPreviewClicked);
    connect(okBtn_, &QPushButton::clicked, this, &MeasureAreaToolDialog::onOkClicked);
    connect(cancelBtn_, &QPushButton::clicked, this, &MeasureAreaToolDialog::onCancelClicked);
    connect(applyBtn_, &QPushButton::clicked, this, &MeasureAreaToolDialog::onApplyClicked);
}

void MeasureAreaToolDialog::onMeasureModeChanged(int index)
{
    if (!tool_) return;

    auto mode = static_cast<Algorithm::MeasureAreaTool::MeasureMode>(
        measureModeCombo_->currentData().toInt());
    tool_->setMeasureMode(mode);
    updateInputVisibility();
    emit parameterChanged();
}

void MeasureAreaToolDialog::onPixelToMmChanged(double value)
{
    if (!tool_) return;
    tool_->setPixelToMm(value);
    emit parameterChanged();
}

void MeasureAreaToolDialog::onThresholdChanged(int value)
{
    if (!tool_) return;
    tool_->setThreshold(value);
    emit parameterChanged();
}

void MeasureAreaToolDialog::onMinAreaChanged(double value)
{
    if (!tool_) return;
    tool_->setMinArea(value);
    emit parameterChanged();
}

void MeasureAreaToolDialog::onInputSourceChanged(int index)
{
    bool useToolChain = (index == 1);
    contourParamsGroup_->setEnabled(!useToolChain);
    circleParamsGroup_->setEnabled(!useToolChain);
    ellipseParamsGroup_->setEnabled(!useToolChain);
}

void MeasureAreaToolDialog::onCircleParamsChanged()
{
    if (!tool_) return;
    tool_->setCircle(QPointF(circleCenterXSpin_->value(), circleCenterYSpin_->value()),
                     circleRadiusSpin_->value());
    emit parameterChanged();
}

void MeasureAreaToolDialog::onEllipseParamsChanged()
{
    if (!tool_) return;
    tool_->setEllipse(QPointF(ellipseCenterXSpin_->value(), ellipseCenterYSpin_->value()),
                      ellipseASpin_->value(), ellipseBSpin_->value(),
                      ellipseAngleSpin_->value());
    emit parameterChanged();
}

void MeasureAreaToolDialog::onPreviewClicked()
{
    applyParameters();
    emit executeMeasureRequested();
    updateResultDisplay();
}

void MeasureAreaToolDialog::onOkClicked()
{
    applyParameters();
    accept();
}

void MeasureAreaToolDialog::onCancelClicked()
{
    reject();
}

void MeasureAreaToolDialog::onApplyClicked()
{
    applyParameters();
    emit parameterChanged();
}

void MeasureAreaToolDialog::applyParameters()
{
    if (!tool_) return;

    tool_->setMeasureMode(static_cast<Algorithm::MeasureAreaTool::MeasureMode>(
        measureModeCombo_->currentData().toInt()));
    tool_->setPixelToMm(pixelToMmSpin_->value());
    tool_->setThreshold(thresholdSpin_->value());
    tool_->setMinArea(minAreaSpin_->value());
    tool_->setCircle(QPointF(circleCenterXSpin_->value(), circleCenterYSpin_->value()),
                     circleRadiusSpin_->value());
    tool_->setEllipse(QPointF(ellipseCenterXSpin_->value(), ellipseCenterYSpin_->value()),
                      ellipseASpin_->value(), ellipseBSpin_->value(),
                      ellipseAngleSpin_->value());

    LOG_INFO("面积测量参数已应用");
}

void MeasureAreaToolDialog::updateInputVisibility()
{
    auto mode = static_cast<Algorithm::MeasureAreaTool::MeasureMode>(
        measureModeCombo_->currentData().toInt());

    contourParamsGroup_->setVisible(mode == Algorithm::MeasureAreaTool::Contour);
    circleParamsGroup_->setVisible(mode == Algorithm::MeasureAreaTool::Circle);
    ellipseParamsGroup_->setVisible(mode == Algorithm::MeasureAreaTool::Ellipse);
    resultsTable_->setVisible(mode == Algorithm::MeasureAreaTool::Contour);
}

void MeasureAreaToolDialog::updateResultDisplay()
{
    if (!tool_) return;

    const auto& result = tool_->result();
    if (result.valid) {
        areaPixelLabel_->setText(QString("%1 px").arg(result.area, 0, 'f', 2));
        areaMmLabel_->setText(QString("%1 mm2").arg(result.areaMm2, 0, 'f', 4));
        perimeterLabel_->setText(QString("%1 px").arg(result.perimeter, 0, 'f', 2));
        centroidLabel_->setText(QString("(%1, %2)").arg(result.centroid.x(), 0, 'f', 2).arg(result.centroid.y(), 0, 'f', 2));
        circularityLabel_->setText(QString("%1").arg(result.circularity, 0, 'f', 4));
    } else {
        areaPixelLabel_->setText("--");
        areaMmLabel_->setText("--");
        perimeterLabel_->setText("--");
        centroidLabel_->setText("--");
        circularityLabel_->setText("--");
    }

    // 更新多轮廓结果表格
    const auto& allResults = tool_->allResults();
    if (!allResults.empty() && tool_->measureMode() == Algorithm::MeasureAreaTool::Contour) {
        resultsTable_->setRowCount(allResults.size());
        for (size_t i = 0; i < allResults.size(); ++i) {
            const auto& r = allResults[i];
            resultsTable_->setItem(i, 0, new QTableWidgetItem(QString::number(i + 1)));
            resultsTable_->setItem(i, 1, new QTableWidgetItem(QString::number(r.area, 'f', 2)));
            resultsTable_->setItem(i, 2, new QTableWidgetItem(QString::number(r.perimeter, 'f', 2)));
            resultsTable_->setItem(i, 3, new QTableWidgetItem(QString("(%1, %2)")
                .arg(r.centroid.x(), 0, 'f', 1).arg(r.centroid.y(), 0, 'f', 1)));
            resultsTable_->setItem(i, 4, new QTableWidgetItem(QString::number(r.circularity, 'f', 4)));
        }
    } else {
        resultsTable_->setRowCount(0);
    }
}

} // namespace UI
} // namespace VisionForge
