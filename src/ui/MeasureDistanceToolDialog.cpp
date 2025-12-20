/**
 * @file MeasureDistanceToolDialog.cpp
 * @brief 距离测量工具参数编辑对话框实现
 * @author VisionForge Team
 * @date 2025-12-18
 */

#include "ui/MeasureDistanceToolDialog.h"
#include "ui/ImageViewer.h"
#include "algorithm/MeasureDistanceTool.h"
#include "base/Logger.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QSplitter>
#include <QMessageBox>

namespace VisionForge {
namespace UI {

MeasureDistanceToolDialog::MeasureDistanceToolDialog(Algorithm::MeasureDistanceTool* tool, QWidget* parent)
    : QDialog(parent)
    , tool_(tool)
    , currentImage_(nullptr)
    , imageViewer_(nullptr)
    , measureModeCombo_(nullptr)
    , inputSourceCombo_(nullptr)
    , useToolChainCheck_(nullptr)
    , pointInputGroup_(nullptr)
    , point1XSpin_(nullptr)
    , point1YSpin_(nullptr)
    , point2XSpin_(nullptr)
    , point2YSpin_(nullptr)
    , lineInputGroup_(nullptr)
    , line1X1Spin_(nullptr)
    , line1Y1Spin_(nullptr)
    , line1X2Spin_(nullptr)
    , line1Y2Spin_(nullptr)
    , line2X1Spin_(nullptr)
    , line2Y1Spin_(nullptr)
    , line2X2Spin_(nullptr)
    , line2Y2Spin_(nullptr)
    , pixelToMmSpin_(nullptr)
    , distancePixelLabel_(nullptr)
    , distanceMmLabel_(nullptr)
    , point1ResultLabel_(nullptr)
    , point2ResultLabel_(nullptr)
    , previewBtn_(nullptr)
    , okBtn_(nullptr)
    , cancelBtn_(nullptr)
    , applyBtn_(nullptr)
    , previewHelper_(nullptr)
    , autoCalcCheck_(nullptr)
{
    setWindowTitle("距离测量设置");
    setMinimumSize(800, 600);
    resize(900, 650);
    setWindowFlags(windowFlags() | Qt::WindowMaximizeButtonHint);

    createUI();
    connectSignals();
    updateUI();
}

MeasureDistanceToolDialog::~MeasureDistanceToolDialog()
{
}

void MeasureDistanceToolDialog::setImage(const Base::ImageData::Ptr& image)
{
    currentImage_ = image;
    if (imageViewer_ && image) {
        imageViewer_->setImage(image);
    }
}

void MeasureDistanceToolDialog::updateUI()
{
    if (!tool_) return;

    // 阻塞信号
    measureModeCombo_->blockSignals(true);
    pixelToMmSpin_->blockSignals(true);
    point1XSpin_->blockSignals(true);
    point1YSpin_->blockSignals(true);
    point2XSpin_->blockSignals(true);
    point2YSpin_->blockSignals(true);

    // 更新测量模式
    measureModeCombo_->setCurrentIndex(static_cast<int>(tool_->measureMode()));

    // 更新像素转换系数
    pixelToMmSpin_->setValue(tool_->pixelToMm());

    // 更新点坐标
    QPointF p1 = tool_->point1();
    QPointF p2 = tool_->point2();
    point1XSpin_->setValue(p1.x());
    point1YSpin_->setValue(p1.y());
    point2XSpin_->setValue(p2.x());
    point2YSpin_->setValue(p2.y());

    // 更新线坐标
    QLineF l1 = tool_->line1();
    QLineF l2 = tool_->line2();
    line1X1Spin_->setValue(l1.x1());
    line1Y1Spin_->setValue(l1.y1());
    line1X2Spin_->setValue(l1.x2());
    line1Y2Spin_->setValue(l1.y2());
    line2X1Spin_->setValue(l2.x1());
    line2Y1Spin_->setValue(l2.y1());
    line2X2Spin_->setValue(l2.x2());
    line2Y2Spin_->setValue(l2.y2());

    // 恢复信号
    measureModeCombo_->blockSignals(false);
    pixelToMmSpin_->blockSignals(false);
    point1XSpin_->blockSignals(false);
    point1YSpin_->blockSignals(false);
    point2XSpin_->blockSignals(false);
    point2YSpin_->blockSignals(false);

    updateInputVisibility();
    updateResultDisplay();
}

void MeasureDistanceToolDialog::showEvent(QShowEvent* event)
{
    QDialog::showEvent(event);
    updateUI();
}

void MeasureDistanceToolDialog::createUI()
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
    createPointInputGroup(rightLayout);
    createLineInputGroup(rightLayout);
    createPixelConversionGroup(rightLayout);
    createResultGroup(rightLayout);
    createButtonGroup(rightLayout);

    rightLayout->addStretch();

    mainSplitter->addWidget(leftPanel);
    mainSplitter->addWidget(rightPanel);
    mainSplitter->setStretchFactor(0, 2);
    mainSplitter->setStretchFactor(1, 1);

    mainLayout->addWidget(mainSplitter, 1);

    // 创建实时计算辅助器
    previewHelper_ = new PreviewHelper(this, 100);  // 测量计算快，使用100ms延迟

    // 实时计算选项
    QHBoxLayout* autoCalcLayout = new QHBoxLayout();
    autoCalcCheck_ = new QCheckBox("实时计算", this);
    autoCalcCheck_->setChecked(true);  // 默认开启
    autoCalcCheck_->setToolTip("启用后参数修改会自动更新计算结果");
    autoCalcLayout->addWidget(autoCalcCheck_);
    autoCalcLayout->addStretch();
    mainLayout->addLayout(autoCalcLayout);

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

void MeasureDistanceToolDialog::createMeasureModeGroup(QVBoxLayout* layout)
{
    QGroupBox* group = new QGroupBox("测量模式", this);
    QFormLayout* formLayout = new QFormLayout(group);

    measureModeCombo_ = new QComboBox(group);
    measureModeCombo_->addItem("点到点", static_cast<int>(Algorithm::MeasureDistanceTool::PointToPoint));
    measureModeCombo_->addItem("点到线", static_cast<int>(Algorithm::MeasureDistanceTool::PointToLine));
    measureModeCombo_->addItem("线到线", static_cast<int>(Algorithm::MeasureDistanceTool::LineToLine));
    measureModeCombo_->addItem("圆心到点", static_cast<int>(Algorithm::MeasureDistanceTool::CircleToPoint));
    measureModeCombo_->addItem("圆心到线", static_cast<int>(Algorithm::MeasureDistanceTool::CircleToLine));
    measureModeCombo_->addItem("圆心到圆心", static_cast<int>(Algorithm::MeasureDistanceTool::CircleToCircle));
    measureModeCombo_->addItem("最短距离", static_cast<int>(Algorithm::MeasureDistanceTool::MinDistance));

    formLayout->addRow("测量模式:", measureModeCombo_);

    layout->addWidget(group);
}

void MeasureDistanceToolDialog::createInputSourceGroup(QVBoxLayout* layout)
{
    QGroupBox* group = new QGroupBox("输入源", this);
    QVBoxLayout* groupLayout = new QVBoxLayout(group);

    inputSourceCombo_ = new QComboBox(group);
    inputSourceCombo_->addItem("手动输入");
    inputSourceCombo_->addItem("从工具链获取");

    useToolChainCheck_ = new QCheckBox("启用工具链输入", group);

    groupLayout->addWidget(inputSourceCombo_);
    groupLayout->addWidget(useToolChainCheck_);

    layout->addWidget(group);
}

void MeasureDistanceToolDialog::createPointInputGroup(QVBoxLayout* layout)
{
    pointInputGroup_ = new QGroupBox("点坐标输入", this);
    QGridLayout* gridLayout = new QGridLayout(pointInputGroup_);

    // 点1
    gridLayout->addWidget(new QLabel("点1 X:"), 0, 0);
    point1XSpin_ = new QDoubleSpinBox(pointInputGroup_);
    point1XSpin_->setRange(-100000, 100000);
    point1XSpin_->setDecimals(2);
    gridLayout->addWidget(point1XSpin_, 0, 1);

    gridLayout->addWidget(new QLabel("Y:"), 0, 2);
    point1YSpin_ = new QDoubleSpinBox(pointInputGroup_);
    point1YSpin_->setRange(-100000, 100000);
    point1YSpin_->setDecimals(2);
    gridLayout->addWidget(point1YSpin_, 0, 3);

    // 点2
    gridLayout->addWidget(new QLabel("点2 X:"), 1, 0);
    point2XSpin_ = new QDoubleSpinBox(pointInputGroup_);
    point2XSpin_->setRange(-100000, 100000);
    point2XSpin_->setDecimals(2);
    gridLayout->addWidget(point2XSpin_, 1, 1);

    gridLayout->addWidget(new QLabel("Y:"), 1, 2);
    point2YSpin_ = new QDoubleSpinBox(pointInputGroup_);
    point2YSpin_->setRange(-100000, 100000);
    point2YSpin_->setDecimals(2);
    gridLayout->addWidget(point2YSpin_, 1, 3);

    layout->addWidget(pointInputGroup_);
}

void MeasureDistanceToolDialog::createLineInputGroup(QVBoxLayout* layout)
{
    lineInputGroup_ = new QGroupBox("线坐标输入", this);
    QGridLayout* gridLayout = new QGridLayout(lineInputGroup_);

    // 线1
    gridLayout->addWidget(new QLabel("线1 起点:"), 0, 0);
    line1X1Spin_ = new QDoubleSpinBox(lineInputGroup_);
    line1X1Spin_->setRange(-100000, 100000);
    line1X1Spin_->setDecimals(2);
    gridLayout->addWidget(line1X1Spin_, 0, 1);

    line1Y1Spin_ = new QDoubleSpinBox(lineInputGroup_);
    line1Y1Spin_->setRange(-100000, 100000);
    line1Y1Spin_->setDecimals(2);
    gridLayout->addWidget(line1Y1Spin_, 0, 2);

    gridLayout->addWidget(new QLabel("终点:"), 0, 3);
    line1X2Spin_ = new QDoubleSpinBox(lineInputGroup_);
    line1X2Spin_->setRange(-100000, 100000);
    line1X2Spin_->setDecimals(2);
    gridLayout->addWidget(line1X2Spin_, 0, 4);

    line1Y2Spin_ = new QDoubleSpinBox(lineInputGroup_);
    line1Y2Spin_->setRange(-100000, 100000);
    line1Y2Spin_->setDecimals(2);
    gridLayout->addWidget(line1Y2Spin_, 0, 5);

    // 线2
    gridLayout->addWidget(new QLabel("线2 起点:"), 1, 0);
    line2X1Spin_ = new QDoubleSpinBox(lineInputGroup_);
    line2X1Spin_->setRange(-100000, 100000);
    line2X1Spin_->setDecimals(2);
    gridLayout->addWidget(line2X1Spin_, 1, 1);

    line2Y1Spin_ = new QDoubleSpinBox(lineInputGroup_);
    line2Y1Spin_->setRange(-100000, 100000);
    line2Y1Spin_->setDecimals(2);
    gridLayout->addWidget(line2Y1Spin_, 1, 2);

    gridLayout->addWidget(new QLabel("终点:"), 1, 3);
    line2X2Spin_ = new QDoubleSpinBox(lineInputGroup_);
    line2X2Spin_->setRange(-100000, 100000);
    line2X2Spin_->setDecimals(2);
    gridLayout->addWidget(line2X2Spin_, 1, 4);

    line2Y2Spin_ = new QDoubleSpinBox(lineInputGroup_);
    line2Y2Spin_->setRange(-100000, 100000);
    line2Y2Spin_->setDecimals(2);
    gridLayout->addWidget(line2Y2Spin_, 1, 5);

    layout->addWidget(lineInputGroup_);
}

void MeasureDistanceToolDialog::createPixelConversionGroup(QVBoxLayout* layout)
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

void MeasureDistanceToolDialog::createResultGroup(QVBoxLayout* layout)
{
    QGroupBox* group = new QGroupBox("测量结果", this);
    QFormLayout* formLayout = new QFormLayout(group);

    distancePixelLabel_ = new QLabel("--", group);
    distanceMmLabel_ = new QLabel("--", group);
    point1ResultLabel_ = new QLabel("--", group);
    point2ResultLabel_ = new QLabel("--", group);

    formLayout->addRow("距离(像素):", distancePixelLabel_);
    formLayout->addRow("距离(mm):", distanceMmLabel_);
    formLayout->addRow("测量点1:", point1ResultLabel_);
    formLayout->addRow("测量点2:", point2ResultLabel_);

    layout->addWidget(group);
}

void MeasureDistanceToolDialog::createButtonGroup(QVBoxLayout* layout)
{
    QHBoxLayout* btnLayout = new QHBoxLayout();

    previewBtn_ = new QPushButton("预览测量", this);
    previewBtn_->setMinimumHeight(35);
    previewBtn_->setStyleSheet("QPushButton { background-color: #2196F3; color: white; font-weight: bold; }");

    btnLayout->addWidget(previewBtn_);

    layout->addLayout(btnLayout);
}

void MeasureDistanceToolDialog::connectSignals()
{
    // 参数变化
    connect(measureModeCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MeasureDistanceToolDialog::onMeasureModeChanged);
    connect(pixelToMmSpin_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MeasureDistanceToolDialog::onPixelToMmChanged);
    connect(inputSourceCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MeasureDistanceToolDialog::onInputSourceChanged);

    connect(point1XSpin_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MeasureDistanceToolDialog::onPoint1Changed);
    connect(point1YSpin_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MeasureDistanceToolDialog::onPoint1Changed);
    connect(point2XSpin_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MeasureDistanceToolDialog::onPoint2Changed);
    connect(point2YSpin_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MeasureDistanceToolDialog::onPoint2Changed);

    connect(line1X1Spin_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MeasureDistanceToolDialog::onLine1Changed);
    connect(line1Y1Spin_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MeasureDistanceToolDialog::onLine1Changed);
    connect(line1X2Spin_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MeasureDistanceToolDialog::onLine1Changed);
    connect(line1Y2Spin_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MeasureDistanceToolDialog::onLine1Changed);
    connect(line2X1Spin_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MeasureDistanceToolDialog::onLine2Changed);
    connect(line2Y1Spin_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MeasureDistanceToolDialog::onLine2Changed);
    connect(line2X2Spin_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MeasureDistanceToolDialog::onLine2Changed);
    connect(line2Y2Spin_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MeasureDistanceToolDialog::onLine2Changed);

    // 实时计算
    connect(autoCalcCheck_, &QCheckBox::toggled,
            previewHelper_, &PreviewHelper::setAutoPreviewEnabled);
    connect(previewHelper_, &PreviewHelper::previewTriggered,
            this, &MeasureDistanceToolDialog::onAutoCalc);

    // 按钮
    connect(previewBtn_, &QPushButton::clicked, this, &MeasureDistanceToolDialog::onPreviewClicked);
    connect(okBtn_, &QPushButton::clicked, this, &MeasureDistanceToolDialog::onOkClicked);
    connect(cancelBtn_, &QPushButton::clicked, this, &MeasureDistanceToolDialog::onCancelClicked);
    connect(applyBtn_, &QPushButton::clicked, this, &MeasureDistanceToolDialog::onApplyClicked);
}

void MeasureDistanceToolDialog::onMeasureModeChanged(int index)
{
    if (!tool_) return;

    auto mode = static_cast<Algorithm::MeasureDistanceTool::MeasureMode>(
        measureModeCombo_->currentData().toInt());
    tool_->setMeasureMode(mode);
    updateInputVisibility();
    emit parameterChanged();
    previewHelper_->requestPreview();
}

void MeasureDistanceToolDialog::onPixelToMmChanged(double value)
{
    if (!tool_) return;
    tool_->setPixelToMm(value);
    emit parameterChanged();
    previewHelper_->requestPreview();
}

void MeasureDistanceToolDialog::onInputSourceChanged(int index)
{
    bool useToolChain = (index == 1);
    if (tool_) {
        tool_->setInputFromToolChain(useToolChain);
    }
    pointInputGroup_->setEnabled(!useToolChain);
    lineInputGroup_->setEnabled(!useToolChain);
}

void MeasureDistanceToolDialog::onPoint1Changed()
{
    if (!tool_) return;
    tool_->setPoint1(QPointF(point1XSpin_->value(), point1YSpin_->value()));
    emit parameterChanged();
    previewHelper_->requestPreview();
}

void MeasureDistanceToolDialog::onPoint2Changed()
{
    if (!tool_) return;
    tool_->setPoint2(QPointF(point2XSpin_->value(), point2YSpin_->value()));
    emit parameterChanged();
    previewHelper_->requestPreview();
}

void MeasureDistanceToolDialog::onLine1Changed()
{
    if (!tool_) return;
    QLineF line(line1X1Spin_->value(), line1Y1Spin_->value(),
                line1X2Spin_->value(), line1Y2Spin_->value());
    tool_->setLine1(line);
    emit parameterChanged();
    previewHelper_->requestPreview();
}

void MeasureDistanceToolDialog::onLine2Changed()
{
    if (!tool_) return;
    QLineF line(line2X1Spin_->value(), line2Y1Spin_->value(),
                line2X2Spin_->value(), line2Y2Spin_->value());
    tool_->setLine2(line);
    emit parameterChanged();
    previewHelper_->requestPreview();
}

void MeasureDistanceToolDialog::onPreviewClicked()
{
    applyParameters();
    emit executeMeasureRequested();
    updateResultDisplay();
}

void MeasureDistanceToolDialog::onOkClicked()
{
    applyParameters();
    accept();
}

void MeasureDistanceToolDialog::onCancelClicked()
{
    reject();
}

void MeasureDistanceToolDialog::onApplyClicked()
{
    applyParameters();
    emit parameterChanged();
}

void MeasureDistanceToolDialog::applyParameters()
{
    if (!tool_) return;

    tool_->setMeasureMode(static_cast<Algorithm::MeasureDistanceTool::MeasureMode>(
        measureModeCombo_->currentData().toInt()));
    tool_->setPixelToMm(pixelToMmSpin_->value());
    tool_->setPoint1(QPointF(point1XSpin_->value(), point1YSpin_->value()));
    tool_->setPoint2(QPointF(point2XSpin_->value(), point2YSpin_->value()));
    tool_->setLine1(QLineF(line1X1Spin_->value(), line1Y1Spin_->value(),
                           line1X2Spin_->value(), line1Y2Spin_->value()));
    tool_->setLine2(QLineF(line2X1Spin_->value(), line2Y1Spin_->value(),
                           line2X2Spin_->value(), line2Y2Spin_->value()));

    LOG_INFO("距离测量参数已应用");
}

void MeasureDistanceToolDialog::updateInputVisibility()
{
    auto mode = static_cast<Algorithm::MeasureDistanceTool::MeasureMode>(
        measureModeCombo_->currentData().toInt());

    bool showPoints = (mode == Algorithm::MeasureDistanceTool::PointToPoint ||
                       mode == Algorithm::MeasureDistanceTool::PointToLine ||
                       mode == Algorithm::MeasureDistanceTool::CircleToPoint);
    bool showLines = (mode == Algorithm::MeasureDistanceTool::PointToLine ||
                      mode == Algorithm::MeasureDistanceTool::LineToLine ||
                      mode == Algorithm::MeasureDistanceTool::CircleToLine);

    pointInputGroup_->setVisible(showPoints);
    lineInputGroup_->setVisible(showLines);
}

void MeasureDistanceToolDialog::updateResultDisplay()
{
    if (!tool_) return;

    const auto& result = tool_->result();
    if (result.valid) {
        distancePixelLabel_->setText(QString("%1 px").arg(result.distance, 0, 'f', 2));
        distanceMmLabel_->setText(QString("%1 mm").arg(result.distanceMm, 0, 'f', 4));
        point1ResultLabel_->setText(QString("(%1, %2)").arg(result.point1.x(), 0, 'f', 2).arg(result.point1.y(), 0, 'f', 2));
        point2ResultLabel_->setText(QString("(%1, %2)").arg(result.point2.x(), 0, 'f', 2).arg(result.point2.y(), 0, 'f', 2));
    } else {
        distancePixelLabel_->setText("--");
        distanceMmLabel_->setText("--");
        point1ResultLabel_->setText("--");
        point2ResultLabel_->setText("--");
    }
}

void MeasureDistanceToolDialog::onAutoCalc()
{
    if (!tool_) return;

    applyParameters();

    // 执行测量
    Algorithm::ToolResult result;
    tool_->process(currentImage_, result);
    updateResultDisplay();
}

} // namespace UI
} // namespace VisionForge
