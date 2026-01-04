/**
 * @file CalcCenterToolDialog.cpp
 * @brief 中心计算工具参数编辑对话框实现
 * @author VisionForge Team
 * @date 2025-12-18
 */

#include "ui/CalcCenterToolDialog.h"
#include "ui/ImageViewer.h"
#include "algorithm/CalcCenterTool.h"
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

CalcCenterToolDialog::CalcCenterToolDialog(Algorithm::CalcCenterTool* tool, QWidget* parent)
    : QDialog(parent)
    , tool_(tool)
    , currentImage_(nullptr)
    , imageViewer_(nullptr)
    , loadImageBtn_(nullptr)
    , captureImageBtn_(nullptr)
    , calcMethodCombo_(nullptr)
    , sourceTypeCombo_(nullptr)
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
    , centerXLabel_(nullptr)
    , centerYLabel_(nullptr)
    , areaLabel_(nullptr)
    , radiusLabel_(nullptr)
    , previewBtn_(nullptr)
    , okBtn_(nullptr)
    , cancelBtn_(nullptr)
    , applyBtn_(nullptr)
    , previewHelper_(nullptr)
    , autoCalcCheck_(nullptr)
{
    setWindowTitle("中心计算设置");
    setMinimumSize(800, 600);
    resize(900, 650);
    setWindowFlags(windowFlags() | Qt::WindowMaximizeButtonHint);

    createUI();
    connectSignals();
    updateUI();
}

CalcCenterToolDialog::~CalcCenterToolDialog()
{
}

void CalcCenterToolDialog::setImage(const Base::ImageData::Ptr& image)
{
    currentImage_ = image;
    if (imageViewer_ && image) {
        imageViewer_->setImage(image);
    }
}

void CalcCenterToolDialog::updateUI()
{
    if (!tool_) return;

    // 阻塞信号
    calcMethodCombo_->blockSignals(true);
    sourceTypeCombo_->blockSignals(true);
    thresholdSpin_->blockSignals(true);
    invertCheck_->blockSignals(true);

    // 更新计算方法
    calcMethodCombo_->setCurrentIndex(static_cast<int>(tool_->calcMethod()));
    sourceTypeCombo_->setCurrentIndex(static_cast<int>(tool_->sourceType()));

    // 更新图像参数
    thresholdSpin_->setValue(tool_->threshold());
    invertCheck_->setChecked(tool_->isInverted());

    // 恢复信号
    calcMethodCombo_->blockSignals(false);
    sourceTypeCombo_->blockSignals(false);
    thresholdSpin_->blockSignals(false);
    invertCheck_->blockSignals(false);

    updateInputVisibility();
    updatePointsTable();
    updateResultDisplay();
}

void CalcCenterToolDialog::showEvent(QShowEvent* event)
{
    QDialog::showEvent(event);
    updateUI();
}

void CalcCenterToolDialog::createUI()
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

void CalcCenterToolDialog::createCalcMethodGroup(QVBoxLayout* layout)
{
    QGroupBox* group = new QGroupBox("计算方法", this);
    QFormLayout* formLayout = new QFormLayout(group);

    calcMethodCombo_ = new QComboBox(group);
    calcMethodCombo_->addItem("质心(矩计算)", static_cast<int>(Algorithm::CalcCenterTool::Centroid));
    calcMethodCombo_->addItem("几何中心", static_cast<int>(Algorithm::CalcCenterTool::GeometricCenter));
    calcMethodCombo_->addItem("边界框中心", static_cast<int>(Algorithm::CalcCenterTool::BoundingBoxCenter));
    calcMethodCombo_->addItem("最小外接圆中心", static_cast<int>(Algorithm::CalcCenterTool::MinCircleCenter));
    calcMethodCombo_->addItem("多点平均中心", static_cast<int>(Algorithm::CalcCenterTool::PointsAverage));

    formLayout->addRow("计算方法:", calcMethodCombo_);

    layout->addWidget(group);
}

void CalcCenterToolDialog::createSourceTypeGroup(QVBoxLayout* layout)
{
    QGroupBox* group = new QGroupBox("数据源类型", this);
    QFormLayout* formLayout = new QFormLayout(group);

    sourceTypeCombo_ = new QComboBox(group);
    sourceTypeCombo_->addItem("从图像计算", static_cast<int>(Algorithm::CalcCenterTool::FromImage));
    sourceTypeCombo_->addItem("从轮廓计算", static_cast<int>(Algorithm::CalcCenterTool::FromContour));
    sourceTypeCombo_->addItem("从点列表计算", static_cast<int>(Algorithm::CalcCenterTool::FromPoints));

    formLayout->addRow("数据源:", sourceTypeCombo_);

    layout->addWidget(group);
}

void CalcCenterToolDialog::createImageParamsGroup(QVBoxLayout* layout)
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

void CalcCenterToolDialog::createPointsInputGroup(QVBoxLayout* layout)
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

void CalcCenterToolDialog::createResultGroup(QVBoxLayout* layout)
{
    QGroupBox* group = new QGroupBox("计算结果", this);
    QFormLayout* formLayout = new QFormLayout(group);

    centerXLabel_ = new QLabel("--", group);
    centerYLabel_ = new QLabel("--", group);
    areaLabel_ = new QLabel("--", group);
    radiusLabel_ = new QLabel("--", group);

    formLayout->addRow("中心 X:", centerXLabel_);
    formLayout->addRow("中心 Y:", centerYLabel_);
    formLayout->addRow("面积:", areaLabel_);
    formLayout->addRow("外接圆半径:", radiusLabel_);

    layout->addWidget(group);
}

void CalcCenterToolDialog::createButtonGroup(QVBoxLayout* layout)
{
    QHBoxLayout* btnLayout = new QHBoxLayout();

    previewBtn_ = new QPushButton("预览计算", this);
    previewBtn_->setMinimumHeight(35);
    previewBtn_->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; font-weight: bold; }");

    btnLayout->addWidget(previewBtn_);

    layout->addLayout(btnLayout);
}

void CalcCenterToolDialog::connectSignals()
{
    // 参数变化
    connect(calcMethodCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &CalcCenterToolDialog::onCalcMethodChanged);
    connect(sourceTypeCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &CalcCenterToolDialog::onSourceTypeChanged);
    connect(thresholdSpin_, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &CalcCenterToolDialog::onThresholdChanged);
    connect(invertCheck_, &QCheckBox::toggled,
            this, &CalcCenterToolDialog::onInvertedChanged);

    // 点列表操作
    connect(addPointBtn_, &QPushButton::clicked, this, &CalcCenterToolDialog::onAddPoint);
    connect(removePointBtn_, &QPushButton::clicked, this, &CalcCenterToolDialog::onRemovePoint);
    connect(clearPointsBtn_, &QPushButton::clicked, this, &CalcCenterToolDialog::onClearPoints);

    // 实时计算
    connect(autoCalcCheck_, &QCheckBox::toggled,
            previewHelper_, &PreviewHelper::setAutoPreviewEnabled);
    connect(previewHelper_, &PreviewHelper::previewTriggered,
            this, &CalcCenterToolDialog::onAutoCalc);

    // 按钮
    connect(previewBtn_, &QPushButton::clicked, this, &CalcCenterToolDialog::onPreviewClicked);
    connect(okBtn_, &QPushButton::clicked, this, &CalcCenterToolDialog::onOkClicked);
    connect(cancelBtn_, &QPushButton::clicked, this, &CalcCenterToolDialog::onCancelClicked);
    connect(applyBtn_, &QPushButton::clicked, this, &CalcCenterToolDialog::onApplyClicked);

    // 图像操作按钮
    connect(loadImageBtn_, &QPushButton::clicked, this, &CalcCenterToolDialog::onLoadImageClicked);
    connect(captureImageBtn_, &QPushButton::clicked, this, &CalcCenterToolDialog::onCaptureImageClicked);
}

void CalcCenterToolDialog::onCalcMethodChanged(int index)
{
    Q_UNUSED(index);
    if (!tool_) return;

    auto method = static_cast<Algorithm::CalcCenterTool::CalcMethod>(
        calcMethodCombo_->currentData().toInt());
    tool_->setCalcMethod(method);
    emit parameterChanged();
    previewHelper_->requestPreview();
}

void CalcCenterToolDialog::onSourceTypeChanged(int index)
{
    Q_UNUSED(index);
    if (!tool_) return;

    auto type = static_cast<Algorithm::CalcCenterTool::SourceType>(
        sourceTypeCombo_->currentData().toInt());
    tool_->setSourceType(type);
    updateInputVisibility();
    emit parameterChanged();
    previewHelper_->requestPreview();
}

void CalcCenterToolDialog::onThresholdChanged(int value)
{
    Q_UNUSED(value);
    if (!tool_) return;
    tool_->setThreshold(thresholdSpin_->value());
    emit parameterChanged();
    previewHelper_->requestPreview();
}

void CalcCenterToolDialog::onInvertedChanged(bool checked)
{
    if (!tool_) return;
    tool_->setInverted(checked);
    emit parameterChanged();
    previewHelper_->requestPreview();
}

void CalcCenterToolDialog::onAddPoint()
{
    if (!tool_) return;

    auto points = tool_->inputPoints();
    points.push_back(QPointF(newPointXSpin_->value(), newPointYSpin_->value()));
    tool_->setInputPoints(points);

    updatePointsTable();
    emit parameterChanged();
    previewHelper_->requestPreview();
}

void CalcCenterToolDialog::onRemovePoint()
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

void CalcCenterToolDialog::onClearPoints()
{
    if (!tool_) return;

    if (QMessageBox::question(this, "确认", "确定清空所有点吗?") == QMessageBox::Yes) {
        tool_->setInputPoints({});
        updatePointsTable();
        emit parameterChanged();
    }
}

void CalcCenterToolDialog::onPreviewClicked()
{
    applyParameters();
    emit executeCalcRequested();
    updateResultDisplay();
}

void CalcCenterToolDialog::onOkClicked()
{
    applyParameters();
    accept();
}

void CalcCenterToolDialog::onCancelClicked()
{
    reject();
}

void CalcCenterToolDialog::onApplyClicked()
{
    applyParameters();
    emit parameterChanged();
}

void CalcCenterToolDialog::applyParameters()
{
    if (!tool_) return;

    tool_->setCalcMethod(static_cast<Algorithm::CalcCenterTool::CalcMethod>(
        calcMethodCombo_->currentData().toInt()));
    tool_->setSourceType(static_cast<Algorithm::CalcCenterTool::SourceType>(
        sourceTypeCombo_->currentData().toInt()));
    tool_->setThreshold(thresholdSpin_->value());
    tool_->setInverted(invertCheck_->isChecked());

    LOG_INFO("中心计算参数已应用");
}

void CalcCenterToolDialog::updateInputVisibility()
{
    auto sourceType = static_cast<Algorithm::CalcCenterTool::SourceType>(
        sourceTypeCombo_->currentData().toInt());

    imageParamsGroup_->setVisible(sourceType == Algorithm::CalcCenterTool::FromImage);
    pointsInputGroup_->setVisible(sourceType == Algorithm::CalcCenterTool::FromPoints);
}

void CalcCenterToolDialog::updatePointsTable()
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

void CalcCenterToolDialog::updateResultDisplay()
{
    if (!tool_) return;

    QPointF center = tool_->center();
    double area = tool_->area();

    if (center.x() != 0 || center.y() != 0 || area != 0) {
        centerXLabel_->setText(QString("%1 px").arg(center.x(), 0, 'f', 2));
        centerYLabel_->setText(QString("%1 px").arg(center.y(), 0, 'f', 2));
        areaLabel_->setText(QString("%1 px").arg(area, 0, 'f', 2));
        radiusLabel_->setText("--");
    } else {
        centerXLabel_->setText("--");
        centerYLabel_->setText("--");
        areaLabel_->setText("--");
        radiusLabel_->setText("--");
    }
}

void CalcCenterToolDialog::onAutoCalc()
{
    if (!tool_) return;

    applyParameters();

    // 执行计算
    Algorithm::ToolResult result;
    tool_->process(currentImage_, result);
    updateResultDisplay();
}

void CalcCenterToolDialog::onLoadImageClicked()
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

void CalcCenterToolDialog::onCaptureImageClicked()
{
    emit captureImageRequested();
    LOG_INFO("请求采集图像");
}

} // namespace UI
} // namespace VisionForge
