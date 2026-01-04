/**
 * @file MeasureAngleToolDialog.cpp
 * @brief 角度测量工具参数编辑对话框实现
 * @author VisionForge Team
 * @date 2025-12-18
 */

#include "ui/MeasureAngleToolDialog.h"
#include "ui/ImageViewer.h"
#include "algorithm/MeasureAngleTool.h"
#include "base/Logger.h"

#include <opencv2/imgcodecs.hpp>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QSplitter>
#include <QMessageBox>
#include <QFileDialog>
#include <QStyle>

namespace VisionForge {
namespace UI {

MeasureAngleToolDialog::MeasureAngleToolDialog(Algorithm::MeasureAngleTool* tool, QWidget* parent)
    : QDialog(parent)
    , tool_(tool)
    , currentImage_(nullptr)
    , imageViewer_(nullptr)
    , measureModeCombo_(nullptr)
    , angleRangeCombo_(nullptr)
    , angleFormatCombo_(nullptr)
    , inputSourceCombo_(nullptr)
    , lineInputGroup_(nullptr)
    , line1X1Spin_(nullptr)
    , line1Y1Spin_(nullptr)
    , line1X2Spin_(nullptr)
    , line1Y2Spin_(nullptr)
    , line2X1Spin_(nullptr)
    , line2Y1Spin_(nullptr)
    , line2X2Spin_(nullptr)
    , line2Y2Spin_(nullptr)
    , pointInputGroup_(nullptr)
    , vertexXSpin_(nullptr)
    , vertexYSpin_(nullptr)
    , point1XSpin_(nullptr)
    , point1YSpin_(nullptr)
    , point2XSpin_(nullptr)
    , point2YSpin_(nullptr)
    , angleDegLabel_(nullptr)
    , angleRadLabel_(nullptr)
    , vertexResultLabel_(nullptr)
    , previewBtn_(nullptr)
    , okBtn_(nullptr)
    , cancelBtn_(nullptr)
    , applyBtn_(nullptr)
    , loadImageBtn_(nullptr)
    , captureImageBtn_(nullptr)
    , previewHelper_(nullptr)
    , autoCalcCheck_(nullptr)
{
    setWindowTitle("角度测量设置");
    setMinimumSize(800, 600);
    resize(900, 650);
    setWindowFlags(windowFlags() | Qt::WindowMaximizeButtonHint);

    createUI();
    connectSignals();
    updateUI();
}

MeasureAngleToolDialog::~MeasureAngleToolDialog()
{
}

void MeasureAngleToolDialog::setImage(const Base::ImageData::Ptr& image)
{
    currentImage_ = image;
    if (imageViewer_ && image) {
        imageViewer_->setImage(image);
    }
}

void MeasureAngleToolDialog::updateUI()
{
    if (!tool_) return;

    // 阻塞信号
    measureModeCombo_->blockSignals(true);
    angleRangeCombo_->blockSignals(true);

    // 更新测量模式
    measureModeCombo_->setCurrentIndex(static_cast<int>(tool_->measureMode()));
    angleRangeCombo_->setCurrentIndex(static_cast<int>(tool_->angleRange()));

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

    // 更新点坐标
    QPointF vertex = tool_->vertex();
    QPointF p1 = tool_->point1();
    QPointF p2 = tool_->point2();
    vertexXSpin_->setValue(vertex.x());
    vertexYSpin_->setValue(vertex.y());
    point1XSpin_->setValue(p1.x());
    point1YSpin_->setValue(p1.y());
    point2XSpin_->setValue(p2.x());
    point2YSpin_->setValue(p2.y());

    // 恢复信号
    measureModeCombo_->blockSignals(false);
    angleRangeCombo_->blockSignals(false);

    updateInputVisibility();
    updateResultDisplay();
}

void MeasureAngleToolDialog::showEvent(QShowEvent* event)
{
    QDialog::showEvent(event);
    updateUI();
}

void MeasureAngleToolDialog::createUI()
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

    // 标题栏布局
    QHBoxLayout* titleLayout = new QHBoxLayout();
    QLabel* titleLabel = new QLabel("图像预览", leftPanel);
    titleLabel->setStyleSheet("font-weight: bold; font-size: 12px;");
    titleLayout->addWidget(titleLabel);
    titleLayout->addStretch();

    // 加载图片按钮
    loadImageBtn_ = new QPushButton(tr("加载图片"), leftPanel);
    loadImageBtn_->setIcon(style()->standardIcon(QStyle::SP_DialogOpenButton));
    loadImageBtn_->setToolTip(tr("从文件加载图片"));
    loadImageBtn_->setMaximumWidth(100);
    titleLayout->addWidget(loadImageBtn_);

    // 采集图像按钮
    captureImageBtn_ = new QPushButton(tr("采集图像"), leftPanel);
    captureImageBtn_->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    captureImageBtn_->setToolTip(tr("从相机采集图像"));
    captureImageBtn_->setMaximumWidth(100);
    titleLayout->addWidget(captureImageBtn_);

    imageViewer_ = new ImageViewer(leftPanel);
    imageViewer_->setMinimumSize(400, 300);

    leftLayout->addLayout(titleLayout);
    leftLayout->addWidget(imageViewer_, 1);

    // 右侧 - 参数设置
    QWidget* rightPanel = new QWidget(mainSplitter);
    QVBoxLayout* rightLayout = new QVBoxLayout(rightPanel);
    rightLayout->setContentsMargins(4, 0, 0, 0);
    rightLayout->setSpacing(8);

    createMeasureModeGroup(rightLayout);
    createAngleFormatGroup(rightLayout);
    createInputSourceGroup(rightLayout);
    createLineInputGroup(rightLayout);
    createPointInputGroup(rightLayout);
    createResultGroup(rightLayout);
    createButtonGroup(rightLayout);

    rightLayout->addStretch();

    mainSplitter->addWidget(leftPanel);
    mainSplitter->addWidget(rightPanel);
    mainSplitter->setStretchFactor(0, 2);
    mainSplitter->setStretchFactor(1, 1);

    mainLayout->addWidget(mainSplitter, 1);

    // 创建实时计算辅助器
    previewHelper_ = new PreviewHelper(this, 100);

    // 实时计算选项
    QHBoxLayout* autoCalcLayout = new QHBoxLayout();
    autoCalcCheck_ = new QCheckBox("实时计算", this);
    autoCalcCheck_->setChecked(true);
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

void MeasureAngleToolDialog::createMeasureModeGroup(QVBoxLayout* layout)
{
    QGroupBox* group = new QGroupBox("测量模式", this);
    QFormLayout* formLayout = new QFormLayout(group);

    measureModeCombo_ = new QComboBox(group);
    measureModeCombo_->addItem("两线夹角", static_cast<int>(Algorithm::MeasureAngleTool::LineToLine));
    measureModeCombo_->addItem("三点角度", static_cast<int>(Algorithm::MeasureAngleTool::ThreePoints));
    measureModeCombo_->addItem("线与水平方向", static_cast<int>(Algorithm::MeasureAngleTool::LineToHorizontal));
    measureModeCombo_->addItem("线与垂直方向", static_cast<int>(Algorithm::MeasureAngleTool::LineToVertical));

    formLayout->addRow("测量模式:", measureModeCombo_);

    layout->addWidget(group);
}

void MeasureAngleToolDialog::createAngleFormatGroup(QVBoxLayout* layout)
{
    QGroupBox* group = new QGroupBox("角度输出格式", this);
    QFormLayout* formLayout = new QFormLayout(group);

    angleRangeCombo_ = new QComboBox(group);
    angleRangeCombo_->addItem("0 ~ 180", static_cast<int>(Algorithm::MeasureAngleTool::Range0To180));
    angleRangeCombo_->addItem("0 ~ 360", static_cast<int>(Algorithm::MeasureAngleTool::Range0To360));
    angleRangeCombo_->addItem("-180 ~ 180", static_cast<int>(Algorithm::MeasureAngleTool::RangeMinus180To180));

    angleFormatCombo_ = new QComboBox(group);
    angleFormatCombo_->addItem("度 (Degree)");
    angleFormatCombo_->addItem("弧度 (Radian)");

    formLayout->addRow("角度范围:", angleRangeCombo_);
    formLayout->addRow("显示格式:", angleFormatCombo_);

    layout->addWidget(group);
}

void MeasureAngleToolDialog::createInputSourceGroup(QVBoxLayout* layout)
{
    QGroupBox* group = new QGroupBox("输入源", this);
    QVBoxLayout* groupLayout = new QVBoxLayout(group);

    inputSourceCombo_ = new QComboBox(group);
    inputSourceCombo_->addItem("手动输入");
    inputSourceCombo_->addItem("从工具链获取");

    groupLayout->addWidget(inputSourceCombo_);

    layout->addWidget(group);
}

void MeasureAngleToolDialog::createLineInputGroup(QVBoxLayout* layout)
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

void MeasureAngleToolDialog::createPointInputGroup(QVBoxLayout* layout)
{
    pointInputGroup_ = new QGroupBox("三点坐标输入", this);
    QGridLayout* gridLayout = new QGridLayout(pointInputGroup_);

    // 顶点
    gridLayout->addWidget(new QLabel("顶点 X:"), 0, 0);
    vertexXSpin_ = new QDoubleSpinBox(pointInputGroup_);
    vertexXSpin_->setRange(-100000, 100000);
    vertexXSpin_->setDecimals(2);
    gridLayout->addWidget(vertexXSpin_, 0, 1);

    gridLayout->addWidget(new QLabel("Y:"), 0, 2);
    vertexYSpin_ = new QDoubleSpinBox(pointInputGroup_);
    vertexYSpin_->setRange(-100000, 100000);
    vertexYSpin_->setDecimals(2);
    gridLayout->addWidget(vertexYSpin_, 0, 3);

    // 点1
    gridLayout->addWidget(new QLabel("点1 X:"), 1, 0);
    point1XSpin_ = new QDoubleSpinBox(pointInputGroup_);
    point1XSpin_->setRange(-100000, 100000);
    point1XSpin_->setDecimals(2);
    gridLayout->addWidget(point1XSpin_, 1, 1);

    gridLayout->addWidget(new QLabel("Y:"), 1, 2);
    point1YSpin_ = new QDoubleSpinBox(pointInputGroup_);
    point1YSpin_->setRange(-100000, 100000);
    point1YSpin_->setDecimals(2);
    gridLayout->addWidget(point1YSpin_, 1, 3);

    // 点2
    gridLayout->addWidget(new QLabel("点2 X:"), 2, 0);
    point2XSpin_ = new QDoubleSpinBox(pointInputGroup_);
    point2XSpin_->setRange(-100000, 100000);
    point2XSpin_->setDecimals(2);
    gridLayout->addWidget(point2XSpin_, 2, 1);

    gridLayout->addWidget(new QLabel("Y:"), 2, 2);
    point2YSpin_ = new QDoubleSpinBox(pointInputGroup_);
    point2YSpin_->setRange(-100000, 100000);
    point2YSpin_->setDecimals(2);
    gridLayout->addWidget(point2YSpin_, 2, 3);

    layout->addWidget(pointInputGroup_);
}

void MeasureAngleToolDialog::createResultGroup(QVBoxLayout* layout)
{
    QGroupBox* group = new QGroupBox("测量结果", this);
    QFormLayout* formLayout = new QFormLayout(group);

    angleDegLabel_ = new QLabel("--", group);
    angleRadLabel_ = new QLabel("--", group);
    vertexResultLabel_ = new QLabel("--", group);

    formLayout->addRow("角度(度):", angleDegLabel_);
    formLayout->addRow("角度(弧度):", angleRadLabel_);
    formLayout->addRow("角顶点:", vertexResultLabel_);

    layout->addWidget(group);
}

void MeasureAngleToolDialog::createButtonGroup(QVBoxLayout* layout)
{
    QHBoxLayout* btnLayout = new QHBoxLayout();

    previewBtn_ = new QPushButton("预览测量", this);
    previewBtn_->setMinimumHeight(35);
    previewBtn_->setStyleSheet("QPushButton { background-color: #2196F3; color: white; font-weight: bold; }");

    btnLayout->addWidget(previewBtn_);

    layout->addLayout(btnLayout);
}

void MeasureAngleToolDialog::connectSignals()
{
    // 参数变化
    connect(measureModeCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MeasureAngleToolDialog::onMeasureModeChanged);
    connect(angleRangeCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MeasureAngleToolDialog::onAngleRangeChanged);
    connect(inputSourceCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MeasureAngleToolDialog::onInputSourceChanged);

    connect(line1X1Spin_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MeasureAngleToolDialog::onLine1Changed);
    connect(line1Y1Spin_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MeasureAngleToolDialog::onLine1Changed);
    connect(line1X2Spin_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MeasureAngleToolDialog::onLine1Changed);
    connect(line1Y2Spin_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MeasureAngleToolDialog::onLine1Changed);
    connect(line2X1Spin_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MeasureAngleToolDialog::onLine2Changed);
    connect(line2Y1Spin_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MeasureAngleToolDialog::onLine2Changed);
    connect(line2X2Spin_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MeasureAngleToolDialog::onLine2Changed);
    connect(line2Y2Spin_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MeasureAngleToolDialog::onLine2Changed);

    connect(vertexXSpin_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MeasureAngleToolDialog::onVertexChanged);
    connect(vertexYSpin_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MeasureAngleToolDialog::onVertexChanged);
    connect(point1XSpin_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MeasureAngleToolDialog::onPoint1Changed);
    connect(point1YSpin_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MeasureAngleToolDialog::onPoint1Changed);
    connect(point2XSpin_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MeasureAngleToolDialog::onPoint2Changed);
    connect(point2YSpin_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MeasureAngleToolDialog::onPoint2Changed);

    // 实时计算
    connect(autoCalcCheck_, &QCheckBox::toggled,
            previewHelper_, &PreviewHelper::setAutoPreviewEnabled);
    connect(previewHelper_, &PreviewHelper::previewTriggered,
            this, &MeasureAngleToolDialog::onAutoCalc);

    // 按钮
    connect(previewBtn_, &QPushButton::clicked, this, &MeasureAngleToolDialog::onPreviewClicked);
    connect(okBtn_, &QPushButton::clicked, this, &MeasureAngleToolDialog::onOkClicked);
    connect(cancelBtn_, &QPushButton::clicked, this, &MeasureAngleToolDialog::onCancelClicked);
    connect(applyBtn_, &QPushButton::clicked, this, &MeasureAngleToolDialog::onApplyClicked);

    // 图像操作按钮
    connect(loadImageBtn_, &QPushButton::clicked, this, &MeasureAngleToolDialog::onLoadImageClicked);
    connect(captureImageBtn_, &QPushButton::clicked, this, &MeasureAngleToolDialog::onCaptureImageClicked);
}

void MeasureAngleToolDialog::onMeasureModeChanged(int index)
{
    Q_UNUSED(index);
    if (!tool_) return;

    auto mode = static_cast<Algorithm::MeasureAngleTool::MeasureMode>(
        measureModeCombo_->currentData().toInt());
    tool_->setMeasureMode(mode);
    updateInputVisibility();
    emit parameterChanged();
    previewHelper_->requestPreview();
}

void MeasureAngleToolDialog::onAngleRangeChanged(int index)
{
    Q_UNUSED(index);
    if (!tool_) return;

    auto range = static_cast<Algorithm::MeasureAngleTool::AngleRange>(
        angleRangeCombo_->currentData().toInt());
    tool_->setAngleRange(range);
    emit parameterChanged();
    previewHelper_->requestPreview();
}

void MeasureAngleToolDialog::onInputSourceChanged(int index)
{
    bool useToolChain = (index == 1);
    lineInputGroup_->setEnabled(!useToolChain);
    pointInputGroup_->setEnabled(!useToolChain);
}

void MeasureAngleToolDialog::onLine1Changed()
{
    if (!tool_) return;
    QLineF line(line1X1Spin_->value(), line1Y1Spin_->value(),
                line1X2Spin_->value(), line1Y2Spin_->value());
    tool_->setLine1(line);
    emit parameterChanged();
    previewHelper_->requestPreview();
}

void MeasureAngleToolDialog::onLine2Changed()
{
    if (!tool_) return;
    QLineF line(line2X1Spin_->value(), line2Y1Spin_->value(),
                line2X2Spin_->value(), line2Y2Spin_->value());
    tool_->setLine2(line);
    emit parameterChanged();
    previewHelper_->requestPreview();
}

void MeasureAngleToolDialog::onVertexChanged()
{
    if (!tool_) return;
    tool_->setVertex(QPointF(vertexXSpin_->value(), vertexYSpin_->value()));
    emit parameterChanged();
    previewHelper_->requestPreview();
}

void MeasureAngleToolDialog::onPoint1Changed()
{
    if (!tool_) return;
    tool_->setPoint1(QPointF(point1XSpin_->value(), point1YSpin_->value()));
    emit parameterChanged();
    previewHelper_->requestPreview();
}

void MeasureAngleToolDialog::onPoint2Changed()
{
    if (!tool_) return;
    tool_->setPoint2(QPointF(point2XSpin_->value(), point2YSpin_->value()));
    emit parameterChanged();
    previewHelper_->requestPreview();
}

void MeasureAngleToolDialog::onPreviewClicked()
{
    applyParameters();
    emit executeMeasureRequested();
    updateResultDisplay();
}

void MeasureAngleToolDialog::onOkClicked()
{
    applyParameters();
    accept();
}

void MeasureAngleToolDialog::onCancelClicked()
{
    reject();
}

void MeasureAngleToolDialog::onApplyClicked()
{
    applyParameters();
    emit parameterChanged();
}

void MeasureAngleToolDialog::applyParameters()
{
    if (!tool_) return;

    tool_->setMeasureMode(static_cast<Algorithm::MeasureAngleTool::MeasureMode>(
        measureModeCombo_->currentData().toInt()));
    tool_->setAngleRange(static_cast<Algorithm::MeasureAngleTool::AngleRange>(
        angleRangeCombo_->currentData().toInt()));
    tool_->setLine1(QLineF(line1X1Spin_->value(), line1Y1Spin_->value(),
                           line1X2Spin_->value(), line1Y2Spin_->value()));
    tool_->setLine2(QLineF(line2X1Spin_->value(), line2Y1Spin_->value(),
                           line2X2Spin_->value(), line2Y2Spin_->value()));
    tool_->setVertex(QPointF(vertexXSpin_->value(), vertexYSpin_->value()));
    tool_->setPoint1(QPointF(point1XSpin_->value(), point1YSpin_->value()));
    tool_->setPoint2(QPointF(point2XSpin_->value(), point2YSpin_->value()));

    LOG_INFO("角度测量参数已应用");
}

void MeasureAngleToolDialog::updateInputVisibility()
{
    auto mode = static_cast<Algorithm::MeasureAngleTool::MeasureMode>(
        measureModeCombo_->currentData().toInt());

    bool showLines = (mode == Algorithm::MeasureAngleTool::LineToLine ||
                      mode == Algorithm::MeasureAngleTool::LineToHorizontal ||
                      mode == Algorithm::MeasureAngleTool::LineToVertical);
    bool showPoints = (mode == Algorithm::MeasureAngleTool::ThreePoints);
    bool showLine2 = (mode == Algorithm::MeasureAngleTool::LineToLine);

    lineInputGroup_->setVisible(showLines);
    pointInputGroup_->setVisible(showPoints);

    // 仅显示线1或两条线
    if (lineInputGroup_->isVisible()) {
        line2X1Spin_->setVisible(showLine2);
        line2Y1Spin_->setVisible(showLine2);
        line2X2Spin_->setVisible(showLine2);
        line2Y2Spin_->setVisible(showLine2);
    }
}

void MeasureAngleToolDialog::updateResultDisplay()
{
    if (!tool_) return;

    const auto& result = tool_->result();
    if (result.valid) {
        angleDegLabel_->setText(QString::fromUtf8("%1\u00B0").arg(result.angle, 0, 'f', 4));
        angleRadLabel_->setText(QString("%1 rad").arg(result.angleRad, 0, 'f', 6));
        vertexResultLabel_->setText(QString("(%1, %2)").arg(result.vertex.x(), 0, 'f', 2).arg(result.vertex.y(), 0, 'f', 2));
    } else {
        angleDegLabel_->setText("--");
        angleRadLabel_->setText("--");
        vertexResultLabel_->setText("--");
    }
}

void MeasureAngleToolDialog::onAutoCalc()
{
    if (!tool_) return;

    applyParameters();

    // 执行测量
    Algorithm::ToolResult result;
    tool_->process(currentImage_, result);
    updateResultDisplay();
}

void MeasureAngleToolDialog::onLoadImageClicked()
{
    QString filePath = QFileDialog::getOpenFileName(
        this,
        tr("选择图片文件"),
        QString(),
        tr("图片文件 (*.bmp *.png *.jpg *.jpeg *.tiff *.tif);;所有文件 (*.*)")
    );

    if (filePath.isEmpty()) {
        return;
    }

    try {
        cv::Mat mat = cv::imread(filePath.toStdString());
        if (!mat.empty()) {
            Base::ImageData::Ptr image = std::make_shared<Base::ImageData>(mat);
            setImage(image);
            LOG_INFO(QString("加载图片成功: %1").arg(filePath));
        } else {
            QMessageBox::warning(this, tr("加载失败"), tr("无法加载图片文件: %1").arg(filePath));
        }
    } catch (const std::exception& e) {
        QMessageBox::warning(this, tr("加载失败"), tr("加载图片时发生错误: %1").arg(e.what()));
        LOG_ERROR(QString("加载图片失败: %1").arg(e.what()));
    }
}

void MeasureAngleToolDialog::onCaptureImageClicked()
{
    emit captureImageRequested();
    LOG_INFO("请求采集图像");
}

} // namespace UI
} // namespace VisionForge
