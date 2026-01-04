/**
 * @file CalcOrientationToolDialog.cpp
 * @brief 方向计算工具参数编辑对话框实现
 * @author VisionForge Team
 * @date 2025-12-18
 */

#include "ui/CalcOrientationToolDialog.h"
#include "ui/ImageViewer.h"
#include "algorithm/CalcOrientationTool.h"
#include "base/Logger.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QSplitter>
#include <QMessageBox>
#include <QHeaderView>
#include <QFileDialog>
#include <QStyle>

namespace VisionForge {
namespace UI {

CalcOrientationToolDialog::CalcOrientationToolDialog(Algorithm::CalcOrientationTool* tool, QWidget* parent)
    : QDialog(parent)
    , tool_(tool)
    , currentImage_(nullptr)
    , imageViewer_(nullptr)
    , calcMethodCombo_(nullptr)
    , sourceTypeCombo_(nullptr)
    , angleRangeCombo_(nullptr)
    , imageParamsGroup_(nullptr)
    , thresholdSpin_(nullptr)
    , invertCheck_(nullptr)
    , pointsInputGroup_(nullptr)
    , pointsTable_(nullptr)
    , newPointXSpin_(nullptr)
    , newPointYSpin_(nullptr)
    , addPointBtn_(nullptr)
    , removePointBtn_(nullptr)
    , clearPointsBtn_(nullptr)
    , angleLabel_(nullptr)
    , centerLabel_(nullptr)
    , majorAxisLabel_(nullptr)
    , minorAxisLabel_(nullptr)
    , previewBtn_(nullptr)
    , okBtn_(nullptr)
    , cancelBtn_(nullptr)
    , applyBtn_(nullptr)
    , loadImageBtn_(nullptr)
    , captureImageBtn_(nullptr)
    , previewHelper_(nullptr)
    , autoCalcCheck_(nullptr)
{
    setWindowTitle("方向计算设置");
    setMinimumSize(800, 600);
    resize(900, 650);
    setWindowFlags(windowFlags() | Qt::WindowMaximizeButtonHint);

    createUI();
    connectSignals();
    updateUI();
}

CalcOrientationToolDialog::~CalcOrientationToolDialog()
{
}

void CalcOrientationToolDialog::setImage(const Base::ImageData::Ptr& image)
{
    currentImage_ = image;
    if (imageViewer_ && image) {
        imageViewer_->setImage(image);
    }
}

void CalcOrientationToolDialog::updateUI()
{
    if (!tool_) return;

    // 阻塞信号
    calcMethodCombo_->blockSignals(true);
    sourceTypeCombo_->blockSignals(true);
    angleRangeCombo_->blockSignals(true);
    thresholdSpin_->blockSignals(true);
    invertCheck_->blockSignals(true);

    // 更新计算方法
    calcMethodCombo_->setCurrentIndex(static_cast<int>(tool_->calcMethod()));
    sourceTypeCombo_->setCurrentIndex(static_cast<int>(tool_->sourceType()));
    angleRangeCombo_->setCurrentIndex(tool_->angleRange());

    // 更新图像参数
    thresholdSpin_->setValue(tool_->threshold());
    invertCheck_->setChecked(tool_->isInverted());

    // 恢复信号
    calcMethodCombo_->blockSignals(false);
    sourceTypeCombo_->blockSignals(false);
    angleRangeCombo_->blockSignals(false);
    thresholdSpin_->blockSignals(false);
    invertCheck_->blockSignals(false);

    updateInputVisibility();
    updatePointsTable();
    updateResultDisplay();
}

void CalcOrientationToolDialog::showEvent(QShowEvent* event)
{
    QDialog::showEvent(event);
    updateUI();
}

void CalcOrientationToolDialog::createUI()
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

    createCalcMethodGroup(rightLayout);
    createSourceTypeGroup(rightLayout);
    createAngleRangeGroup(rightLayout);
    createImageParamsGroup(rightLayout);
    createPointsInputGroup(rightLayout);
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

void CalcOrientationToolDialog::createCalcMethodGroup(QVBoxLayout* layout)
{
    QGroupBox* group = new QGroupBox("计算方法", this);
    QFormLayout* formLayout = new QFormLayout(group);

    calcMethodCombo_ = new QComboBox(group);
    calcMethodCombo_->addItem("主成分分析(PCA)", static_cast<int>(Algorithm::CalcOrientationTool::PCA));
    calcMethodCombo_->addItem("最小外接矩形", static_cast<int>(Algorithm::CalcOrientationTool::MinAreaRect));
    calcMethodCombo_->addItem("惯性矩(二阶矩)", static_cast<int>(Algorithm::CalcOrientationTool::Moments));
    calcMethodCombo_->addItem("拟合椭圆", static_cast<int>(Algorithm::CalcOrientationTool::FitEllipse));

    formLayout->addRow("计算方法:", calcMethodCombo_);

    layout->addWidget(group);
}

void CalcOrientationToolDialog::createSourceTypeGroup(QVBoxLayout* layout)
{
    QGroupBox* group = new QGroupBox("数据源类型", this);
    QFormLayout* formLayout = new QFormLayout(group);

    sourceTypeCombo_ = new QComboBox(group);
    sourceTypeCombo_->addItem("从图像计算", static_cast<int>(Algorithm::CalcOrientationTool::FromImage));
    sourceTypeCombo_->addItem("从轮廓计算", static_cast<int>(Algorithm::CalcOrientationTool::FromContour));
    sourceTypeCombo_->addItem("从点列表计算", static_cast<int>(Algorithm::CalcOrientationTool::FromPoints));

    formLayout->addRow("数据源:", sourceTypeCombo_);

    layout->addWidget(group);
}

void CalcOrientationToolDialog::createAngleRangeGroup(QVBoxLayout* layout)
{
    QGroupBox* group = new QGroupBox("角度范围", this);
    QFormLayout* formLayout = new QFormLayout(group);

    angleRangeCombo_ = new QComboBox(group);
    angleRangeCombo_->addItem("0 ~ 360", 0);
    angleRangeCombo_->addItem("-180 ~ 180", 1);
    angleRangeCombo_->addItem("0 ~ 180", 2);

    formLayout->addRow("输出范围:", angleRangeCombo_);

    layout->addWidget(group);
}

void CalcOrientationToolDialog::createImageParamsGroup(QVBoxLayout* layout)
{
    imageParamsGroup_ = new QGroupBox("图像处理参数", this);
    QFormLayout* formLayout = new QFormLayout(imageParamsGroup_);

    thresholdSpin_ = new QSpinBox(imageParamsGroup_);
    thresholdSpin_->setRange(0, 255);
    thresholdSpin_->setValue(128);
    formLayout->addRow("二值化阈值:", thresholdSpin_);

    invertCheck_ = new QCheckBox("反转(白色背景)", imageParamsGroup_);
    formLayout->addRow("", invertCheck_);

    layout->addWidget(imageParamsGroup_);
}

void CalcOrientationToolDialog::createPointsInputGroup(QVBoxLayout* layout)
{
    pointsInputGroup_ = new QGroupBox("点列表输入", this);
    QVBoxLayout* groupLayout = new QVBoxLayout(pointsInputGroup_);

    // 点列表表格
    pointsTable_ = new QTableWidget(pointsInputGroup_);
    pointsTable_->setColumnCount(3);
    pointsTable_->setHorizontalHeaderLabels({"序号", "X", "Y"});
    pointsTable_->horizontalHeader()->setStretchLastSection(true);
    pointsTable_->setMinimumHeight(100);
    pointsTable_->setSelectionBehavior(QAbstractItemView::SelectRows);
    groupLayout->addWidget(pointsTable_);

    // 添加点
    QHBoxLayout* addLayout = new QHBoxLayout();
    addLayout->addWidget(new QLabel("X:"));
    newPointXSpin_ = new QDoubleSpinBox(pointsInputGroup_);
    newPointXSpin_->setRange(-100000, 100000);
    newPointXSpin_->setDecimals(2);
    addLayout->addWidget(newPointXSpin_);

    addLayout->addWidget(new QLabel("Y:"));
    newPointYSpin_ = new QDoubleSpinBox(pointsInputGroup_);
    newPointYSpin_->setRange(-100000, 100000);
    newPointYSpin_->setDecimals(2);
    addLayout->addWidget(newPointYSpin_);

    addPointBtn_ = new QPushButton("添加", pointsInputGroup_);
    addLayout->addWidget(addPointBtn_);
    groupLayout->addLayout(addLayout);

    // 操作按钮
    QHBoxLayout* btnLayout = new QHBoxLayout();
    removePointBtn_ = new QPushButton("删除选中", pointsInputGroup_);
    clearPointsBtn_ = new QPushButton("清空全部", pointsInputGroup_);
    btnLayout->addWidget(removePointBtn_);
    btnLayout->addWidget(clearPointsBtn_);
    btnLayout->addStretch();
    groupLayout->addLayout(btnLayout);

    layout->addWidget(pointsInputGroup_);
}

void CalcOrientationToolDialog::createResultGroup(QVBoxLayout* layout)
{
    QGroupBox* group = new QGroupBox("计算结果", this);
    QFormLayout* formLayout = new QFormLayout(group);

    angleLabel_ = new QLabel("--", group);
    centerLabel_ = new QLabel("--", group);
    majorAxisLabel_ = new QLabel("--", group);
    minorAxisLabel_ = new QLabel("--", group);

    formLayout->addRow("方向角度:", angleLabel_);
    formLayout->addRow("中心点:", centerLabel_);
    formLayout->addRow("主轴长度:", majorAxisLabel_);
    formLayout->addRow("次轴长度:", minorAxisLabel_);

    layout->addWidget(group);
}

void CalcOrientationToolDialog::createButtonGroup(QVBoxLayout* layout)
{
    QHBoxLayout* btnLayout = new QHBoxLayout();

    previewBtn_ = new QPushButton("预览计算", this);
    previewBtn_->setMinimumHeight(35);
    previewBtn_->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; font-weight: bold; }");

    btnLayout->addWidget(previewBtn_);

    layout->addLayout(btnLayout);
}

void CalcOrientationToolDialog::connectSignals()
{
    // 参数变化
    connect(calcMethodCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &CalcOrientationToolDialog::onCalcMethodChanged);
    connect(sourceTypeCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &CalcOrientationToolDialog::onSourceTypeChanged);
    connect(angleRangeCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &CalcOrientationToolDialog::onAngleRangeChanged);
    connect(thresholdSpin_, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &CalcOrientationToolDialog::onThresholdChanged);
    connect(invertCheck_, &QCheckBox::toggled,
            this, &CalcOrientationToolDialog::onInvertedChanged);

    // 点列表操作
    connect(addPointBtn_, &QPushButton::clicked, this, &CalcOrientationToolDialog::onAddPoint);
    connect(removePointBtn_, &QPushButton::clicked, this, &CalcOrientationToolDialog::onRemovePoint);
    connect(clearPointsBtn_, &QPushButton::clicked, this, &CalcOrientationToolDialog::onClearPoints);

    // 实时计算
    connect(autoCalcCheck_, &QCheckBox::toggled,
            previewHelper_, &PreviewHelper::setAutoPreviewEnabled);
    connect(previewHelper_, &PreviewHelper::previewTriggered,
            this, &CalcOrientationToolDialog::onAutoCalc);

    // 按钮
    connect(previewBtn_, &QPushButton::clicked, this, &CalcOrientationToolDialog::onPreviewClicked);
    connect(okBtn_, &QPushButton::clicked, this, &CalcOrientationToolDialog::onOkClicked);
    connect(cancelBtn_, &QPushButton::clicked, this, &CalcOrientationToolDialog::onCancelClicked);
    connect(applyBtn_, &QPushButton::clicked, this, &CalcOrientationToolDialog::onApplyClicked);

    // 图像操作按钮
    connect(loadImageBtn_, &QPushButton::clicked, this, &CalcOrientationToolDialog::onLoadImageClicked);
    connect(captureImageBtn_, &QPushButton::clicked, this, &CalcOrientationToolDialog::onCaptureImageClicked);
}

void CalcOrientationToolDialog::onCalcMethodChanged(int index)
{
    if (!tool_) return;

    auto method = static_cast<Algorithm::CalcOrientationTool::CalcMethod>(
        calcMethodCombo_->currentData().toInt());
    tool_->setCalcMethod(method);
    emit parameterChanged();
    previewHelper_->requestPreview();
}

void CalcOrientationToolDialog::onSourceTypeChanged(int index)
{
    if (!tool_) return;

    auto type = static_cast<Algorithm::CalcOrientationTool::SourceType>(
        sourceTypeCombo_->currentData().toInt());
    tool_->setSourceType(type);
    updateInputVisibility();
    emit parameterChanged();
    previewHelper_->requestPreview();
}

void CalcOrientationToolDialog::onAngleRangeChanged(int index)
{
    if (!tool_) return;
    tool_->setAngleRange(angleRangeCombo_->currentData().toInt());
    emit parameterChanged();
    previewHelper_->requestPreview();
}

void CalcOrientationToolDialog::onThresholdChanged(int value)
{
    if (!tool_) return;
    tool_->setThreshold(value);
    emit parameterChanged();
    previewHelper_->requestPreview();
}

void CalcOrientationToolDialog::onInvertedChanged(bool checked)
{
    if (!tool_) return;
    tool_->setInverted(checked);
    emit parameterChanged();
    previewHelper_->requestPreview();
}

void CalcOrientationToolDialog::onAddPoint()
{
    if (!tool_) return;

    auto points = tool_->inputPoints();
    points.push_back(QPointF(newPointXSpin_->value(), newPointYSpin_->value()));
    tool_->setInputPoints(points);

    updatePointsTable();
    emit parameterChanged();
    previewHelper_->requestPreview();
}

void CalcOrientationToolDialog::onRemovePoint()
{
    if (!tool_) return;

    int row = pointsTable_->currentRow();
    if (row < 0) return;

    auto points = tool_->inputPoints();
    if (row < static_cast<int>(points.size())) {
        points.erase(points.begin() + row);
        tool_->setInputPoints(points);
        updatePointsTable();
        emit parameterChanged();
        previewHelper_->requestPreview();
    }
}

void CalcOrientationToolDialog::onClearPoints()
{
    if (!tool_) return;

    if (QMessageBox::question(this, "确认", "确定清空所有点吗?") == QMessageBox::Yes) {
        tool_->setInputPoints({});
        updatePointsTable();
        emit parameterChanged();
    }
}

void CalcOrientationToolDialog::onPreviewClicked()
{
    applyParameters();
    emit executeCalcRequested();
    updateResultDisplay();
}

void CalcOrientationToolDialog::onOkClicked()
{
    applyParameters();
    accept();
}

void CalcOrientationToolDialog::onCancelClicked()
{
    reject();
}

void CalcOrientationToolDialog::onApplyClicked()
{
    applyParameters();
    emit parameterChanged();
}

void CalcOrientationToolDialog::applyParameters()
{
    if (!tool_) return;

    tool_->setCalcMethod(static_cast<Algorithm::CalcOrientationTool::CalcMethod>(
        calcMethodCombo_->currentData().toInt()));
    tool_->setSourceType(static_cast<Algorithm::CalcOrientationTool::SourceType>(
        sourceTypeCombo_->currentData().toInt()));
    tool_->setAngleRange(angleRangeCombo_->currentData().toInt());
    tool_->setThreshold(thresholdSpin_->value());
    tool_->setInverted(invertCheck_->isChecked());

    LOG_INFO("方向计算参数已应用");
}

void CalcOrientationToolDialog::updateInputVisibility()
{
    auto sourceType = static_cast<Algorithm::CalcOrientationTool::SourceType>(
        sourceTypeCombo_->currentData().toInt());

    imageParamsGroup_->setVisible(sourceType == Algorithm::CalcOrientationTool::FromImage);
    pointsInputGroup_->setVisible(sourceType == Algorithm::CalcOrientationTool::FromPoints);
}

void CalcOrientationToolDialog::updatePointsTable()
{
    if (!tool_) return;

    const auto& points = tool_->inputPoints();
    pointsTable_->setRowCount(points.size());

    for (size_t i = 0; i < points.size(); ++i) {
        pointsTable_->setItem(i, 0, new QTableWidgetItem(QString::number(i + 1)));
        pointsTable_->setItem(i, 1, new QTableWidgetItem(QString::number(points[i].x(), 'f', 2)));
        pointsTable_->setItem(i, 2, new QTableWidgetItem(QString::number(points[i].y(), 'f', 2)));
    }
}

void CalcOrientationToolDialog::updateResultDisplay()
{
    if (!tool_) return;

    double angle = tool_->angle();
    QPointF center = tool_->center();
    double majorAxis = tool_->majorAxisLength();
    double minorAxis = tool_->minorAxisLength();

    if (majorAxis > 0 || minorAxis > 0) {
        angleLabel_->setText(QString::fromUtf8("%1\u00B0").arg(angle, 0, 'f', 2));
        centerLabel_->setText(QString("(%1, %2)").arg(center.x(), 0, 'f', 2).arg(center.y(), 0, 'f', 2));
        majorAxisLabel_->setText(QString("%1 px").arg(majorAxis, 0, 'f', 2));
        minorAxisLabel_->setText(QString("%1 px").arg(minorAxis, 0, 'f', 2));
    } else {
        angleLabel_->setText("--");
        centerLabel_->setText("--");
        majorAxisLabel_->setText("--");
        minorAxisLabel_->setText("--");
    }
}

void CalcOrientationToolDialog::onAutoCalc()
{
    if (!tool_) return;

    applyParameters();
    Algorithm::ToolResult result;
    tool_->process(currentImage_, result);
    updateResultDisplay();
}

void CalcOrientationToolDialog::onLoadImageClicked()
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

    Base::ImageData::Ptr image = Base::ImageData::loadFromFile(filePath);
    if (image) {
        setImage(image);
        LOG_INFO(QString("加载图片成功: %1").arg(filePath));
    } else {
        QMessageBox::warning(this, tr("加载失败"), tr("无法加载图片文件: %1").arg(filePath));
    }
}

void CalcOrientationToolDialog::onCaptureImageClicked()
{
    emit captureImageRequested();
    LOG_INFO("请求采集图像");
}

} // namespace UI
} // namespace VisionForge
