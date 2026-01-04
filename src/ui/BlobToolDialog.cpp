/**
 * @file BlobToolDialog.cpp
 * @brief Blob分析工具参数编辑对话框实现
 * @author VisionForge Team
 * @date 2025-12-18
 */

#include "ui/BlobToolDialog.h"
#include "ui/HalconImageViewer.h"
#include "algorithm/BlobTool.h"
#include "base/Logger.h"

#include <opencv2/imgcodecs.hpp>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QSplitter>
#include <QMessageBox>
#include <QFileDialog>
#include <QStyle>
#include <QTimer>
#include <QSlider>
#include <QHeaderView>

namespace VisionForge {
namespace UI {

BlobToolDialog::BlobToolDialog(Algorithm::BlobTool* tool, QWidget* parent)
    : QDialog(parent)
    , tool_(tool)
    , imageViewer_(nullptr)
    , mainSplitter_(nullptr)
    , paramTabWidget_(nullptr)
    , backendCombo_(nullptr)
    , connectivityCombo_(nullptr)
    , polarityCombo_(nullptr)
    , thresholdGroup_(nullptr)
    , autoThresholdCheckBox_(nullptr)
    , thresholdSpinBox_(nullptr)
    , thresholdSlider_(nullptr)
    , minAreaSpinBox_(nullptr)
    , maxAreaSpinBox_(nullptr)
    , minCircularitySpinBox_(nullptr)
    , maxCircularitySpinBox_(nullptr)
    , sortByCombo_(nullptr)
    , sortDescendingCheckBox_(nullptr)
    , maxCountSpinBox_(nullptr)
    , resultsTable_(nullptr)
    , previewBtn_(nullptr)
    , okBtn_(nullptr)
    , cancelBtn_(nullptr)
    , applyBtn_(nullptr)
    , previewHelper_(nullptr)
    , autoPreviewCheck_(nullptr)
    , loadImageBtn_(nullptr)
    , captureImageBtn_(nullptr)
{
    setWindowTitle("Blob分析设置");
    setMinimumSize(900, 600);
    resize(1000, 700);
    setWindowFlags(windowFlags() | Qt::WindowMaximizeButtonHint);

    createUI();
    connectSignals();
    updateUI();
}

BlobToolDialog::~BlobToolDialog()
{
}

void BlobToolDialog::setImage(const Base::ImageData::Ptr& image)
{
    currentImage_ = image;
    if (imageViewer_ && image) {
        imageViewer_->setImage(image);
    }
}

void BlobToolDialog::updateUI()
{
    if (!tool_) return;

    if (!backendCombo_ || !connectivityCombo_ || !polarityCombo_) {
        return;
    }

    // 阻塞信号
    backendCombo_->blockSignals(true);
    connectivityCombo_->blockSignals(true);
    polarityCombo_->blockSignals(true);
    autoThresholdCheckBox_->blockSignals(true);
    thresholdSpinBox_->blockSignals(true);
    thresholdSlider_->blockSignals(true);
    minAreaSpinBox_->blockSignals(true);
    maxAreaSpinBox_->blockSignals(true);
    minCircularitySpinBox_->blockSignals(true);
    maxCircularitySpinBox_->blockSignals(true);
    sortByCombo_->blockSignals(true);
    sortDescendingCheckBox_->blockSignals(true);
    maxCountSpinBox_->blockSignals(true);

    // 更新值
    backendCombo_->setCurrentIndex(static_cast<int>(tool_->backend()));
    connectivityCombo_->setCurrentIndex(static_cast<int>(tool_->connectivity()));
    polarityCombo_->setCurrentIndex(static_cast<int>(tool_->polarity()));
    autoThresholdCheckBox_->setChecked(tool_->autoThreshold());
    thresholdSpinBox_->setValue(tool_->threshold());
    thresholdSlider_->setValue(tool_->threshold());
    thresholdSpinBox_->setEnabled(!tool_->autoThreshold());
    thresholdSlider_->setEnabled(!tool_->autoThreshold());
    sortByCombo_->setCurrentIndex(static_cast<int>(tool_->sortBy()));
    sortDescendingCheckBox_->setChecked(tool_->sortDescending());
    maxCountSpinBox_->setValue(tool_->maxCount());

    // 恢复信号
    backendCombo_->blockSignals(false);
    connectivityCombo_->blockSignals(false);
    polarityCombo_->blockSignals(false);
    autoThresholdCheckBox_->blockSignals(false);
    thresholdSpinBox_->blockSignals(false);
    thresholdSlider_->blockSignals(false);
    minAreaSpinBox_->blockSignals(false);
    maxAreaSpinBox_->blockSignals(false);
    minCircularitySpinBox_->blockSignals(false);
    maxCircularitySpinBox_->blockSignals(false);
    sortByCombo_->blockSignals(false);
    sortDescendingCheckBox_->blockSignals(false);
    maxCountSpinBox_->blockSignals(false);
}

void BlobToolDialog::showEvent(QShowEvent* event)
{
    QDialog::showEvent(event);
    updateUI();

    if (currentImage_ && imageViewer_) {
        QTimer::singleShot(200, this, [this]() {
            if (currentImage_ && imageViewer_) {
                imageViewer_->setImage(currentImage_);
            }
        });
    }
}

void BlobToolDialog::createUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(8, 8, 8, 8);
    mainLayout->setSpacing(8);

    // 主分割器
    mainSplitter_ = new QSplitter(Qt::Horizontal, this);

    // 左侧面板
    QWidget* leftPanel = new QWidget(mainSplitter_);
    createLeftPanel(leftPanel);

    // 右侧面板
    QWidget* rightPanel = new QWidget(mainSplitter_);
    createRightPanel(rightPanel);

    mainSplitter_->addWidget(leftPanel);
    mainSplitter_->addWidget(rightPanel);
    mainSplitter_->setStretchFactor(0, 3);
    mainSplitter_->setStretchFactor(1, 2);

    mainLayout->addWidget(mainSplitter_, 1);

    // 创建预览辅助器
    previewHelper_ = new PreviewHelper(this, 200);

    // 预览选项行
    QHBoxLayout* previewLayout = new QHBoxLayout();
    autoPreviewCheck_ = new QCheckBox("实时预览", this);
    autoPreviewCheck_->setChecked(previewHelper_->isAutoPreviewEnabled());
    autoPreviewCheck_->setToolTip("启用后参数修改会自动更新预览");
    previewLayout->addWidget(autoPreviewCheck_);
    previewLayout->addStretch();

    previewBtn_ = new QPushButton("预览", this);
    previewBtn_->setMinimumWidth(80);
    previewLayout->addWidget(previewBtn_);

    mainLayout->addLayout(previewLayout);

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

void BlobToolDialog::createLeftPanel(QWidget* parent)
{
    QVBoxLayout* layout = new QVBoxLayout(parent);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(4);

    // 使用垂直分割器
    QSplitter* splitter = new QSplitter(Qt::Vertical, parent);

    // 图像显示
    QWidget* imageWidget = new QWidget(splitter);
    QVBoxLayout* imageLayout = new QVBoxLayout(imageWidget);
    imageLayout->setContentsMargins(0, 0, 0, 0);

    // 标题栏布局
    QHBoxLayout* titleLayout = new QHBoxLayout();
    QLabel* titleLabel = new QLabel("检测图像", imageWidget);
    titleLabel->setStyleSheet("font-weight: bold; font-size: 12px;");
    titleLayout->addWidget(titleLabel);
    titleLayout->addStretch();

    // 加载图片按钮
    loadImageBtn_ = new QPushButton(tr("加载图片"), imageWidget);
    loadImageBtn_->setIcon(style()->standardIcon(QStyle::SP_DialogOpenButton));
    loadImageBtn_->setToolTip(tr("从文件加载图片"));
    loadImageBtn_->setMaximumWidth(100);
    titleLayout->addWidget(loadImageBtn_);

    // 采集图像按钮
    captureImageBtn_ = new QPushButton(tr("采集图像"), imageWidget);
    captureImageBtn_->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    captureImageBtn_->setToolTip(tr("从相机采集图像"));
    captureImageBtn_->setMaximumWidth(100);
    titleLayout->addWidget(captureImageBtn_);

    imageViewer_ = new HalconImageViewer(imageWidget);
    imageViewer_->setMinimumSize(400, 250);
    imageViewer_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    imageLayout->addLayout(titleLayout);
    imageLayout->addWidget(imageViewer_, 1);

    // 结果表格
    QWidget* tableWidget = new QWidget(splitter);
    QVBoxLayout* tableLayout = new QVBoxLayout(tableWidget);
    tableLayout->setContentsMargins(0, 0, 0, 0);

    QLabel* tableTitle = new QLabel("检测结果", tableWidget);
    tableTitle->setStyleSheet("font-weight: bold; font-size: 12px;");

    resultsTable_ = new QTableWidget(tableWidget);
    resultsTable_->setColumnCount(5);
    resultsTable_->setHorizontalHeaderLabels({"ID", "面积", "圆度", "中心X", "中心Y"});
    resultsTable_->horizontalHeader()->setStretchLastSection(true);
    resultsTable_->setSelectionBehavior(QAbstractItemView::SelectRows);
    resultsTable_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    resultsTable_->setFixedHeight(150);

    tableLayout->addWidget(tableTitle);
    tableLayout->addWidget(resultsTable_);

    splitter->addWidget(imageWidget);
    splitter->addWidget(tableWidget);
    splitter->setStretchFactor(0, 3);
    splitter->setStretchFactor(1, 1);

    layout->addWidget(splitter);
}

void BlobToolDialog::createRightPanel(QWidget* parent)
{
    QVBoxLayout* layout = new QVBoxLayout(parent);
    layout->setContentsMargins(4, 0, 0, 0);
    layout->setSpacing(8);

    // 选项卡控件
    paramTabWidget_ = new QTabWidget(parent);

    // Tab 1: 基本参数
    QWidget* basicTab = new QWidget(paramTabWidget_);
    QVBoxLayout* basicLayout = new QVBoxLayout(basicTab);
    basicLayout->setContentsMargins(8, 8, 8, 8);
    basicLayout->setSpacing(8);
    createBasicParamGroup(basicLayout);
    createThresholdGroup(basicLayout);
    basicLayout->addStretch();
    paramTabWidget_->addTab(basicTab, "基本参数");

    // Tab 2: 过滤参数
    QWidget* filterTab = new QWidget(paramTabWidget_);
    QVBoxLayout* filterLayout = new QVBoxLayout(filterTab);
    filterLayout->setContentsMargins(8, 8, 8, 8);
    filterLayout->setSpacing(8);
    createFilterGroup(filterLayout);
    createSortGroup(filterLayout);
    filterLayout->addStretch();
    paramTabWidget_->addTab(filterTab, "过滤排序");

    layout->addWidget(paramTabWidget_, 1);
}

void BlobToolDialog::createBasicParamGroup(QVBoxLayout* layout)
{
    QGroupBox* group = new QGroupBox("检测设置", this);
    QGridLayout* groupLayout = new QGridLayout(group);
    groupLayout->setSpacing(6);

    int row = 0;

    // 后端类型
    groupLayout->addWidget(new QLabel("后端类型:"), row, 0);
    backendCombo_ = new QComboBox(this);
    backendCombo_->addItem("自动选择", static_cast<int>(Algorithm::BlobTool::Auto));
    backendCombo_->addItem("OpenCV", static_cast<int>(Algorithm::BlobTool::OpenCV));
    backendCombo_->addItem("Halcon", static_cast<int>(Algorithm::BlobTool::Halcon));
    groupLayout->addWidget(backendCombo_, row, 1);
    row++;

    // 连通性
    groupLayout->addWidget(new QLabel("连通性:"), row, 0);
    connectivityCombo_ = new QComboBox(this);
    connectivityCombo_->addItem("4连通", static_cast<int>(Algorithm::BlobTool::Connect4));
    connectivityCombo_->addItem("8连通", static_cast<int>(Algorithm::BlobTool::Connect8));
    groupLayout->addWidget(connectivityCombo_, row, 1);
    row++;

    // 极性
    groupLayout->addWidget(new QLabel("极性:"), row, 0);
    polarityCombo_ = new QComboBox(this);
    polarityCombo_->addItem("亮区域 (Light)", static_cast<int>(Algorithm::BlobTool::Light));
    polarityCombo_->addItem("暗区域 (Dark)", static_cast<int>(Algorithm::BlobTool::Dark));
    polarityCombo_->addItem("两者 (Both)", static_cast<int>(Algorithm::BlobTool::Both));
    groupLayout->addWidget(polarityCombo_, row, 1);

    layout->addWidget(group);
}

void BlobToolDialog::createThresholdGroup(QVBoxLayout* layout)
{
    thresholdGroup_ = new QGroupBox("二值化阈值", this);
    QVBoxLayout* groupLayout = new QVBoxLayout(thresholdGroup_);

    // 自动阈值
    autoThresholdCheckBox_ = new QCheckBox("自动阈值 (Otsu)", this);
    groupLayout->addWidget(autoThresholdCheckBox_);

    // 手动阈值
    QHBoxLayout* thresholdLayout = new QHBoxLayout();
    thresholdLayout->addWidget(new QLabel("阈值:"));

    thresholdSpinBox_ = new QSpinBox(this);
    thresholdSpinBox_->setRange(0, 255);
    thresholdSpinBox_->setValue(128);
    thresholdLayout->addWidget(thresholdSpinBox_);

    thresholdSlider_ = new QSlider(Qt::Horizontal, this);
    thresholdSlider_->setRange(0, 255);
    thresholdSlider_->setValue(128);
    thresholdLayout->addWidget(thresholdSlider_);

    groupLayout->addLayout(thresholdLayout);

    layout->addWidget(thresholdGroup_);
}

void BlobToolDialog::createFilterGroup(QVBoxLayout* layout)
{
    QGroupBox* group = new QGroupBox("过滤条件", this);
    QGridLayout* groupLayout = new QGridLayout(group);
    groupLayout->setSpacing(6);

    int row = 0;

    // 面积范围
    groupLayout->addWidget(new QLabel("面积范围:"), row, 0);
    QHBoxLayout* areaLayout = new QHBoxLayout();
    minAreaSpinBox_ = new QDoubleSpinBox(this);
    minAreaSpinBox_->setRange(0, 10000000);
    minAreaSpinBox_->setValue(100);
    minAreaSpinBox_->setSuffix(" px2");
    areaLayout->addWidget(minAreaSpinBox_);
    areaLayout->addWidget(new QLabel("~"));
    maxAreaSpinBox_ = new QDoubleSpinBox(this);
    maxAreaSpinBox_->setRange(0, 10000000);
    maxAreaSpinBox_->setValue(1000000);
    maxAreaSpinBox_->setSuffix(" px2");
    areaLayout->addWidget(maxAreaSpinBox_);
    groupLayout->addLayout(areaLayout, row, 1);
    row++;

    // 圆度范围
    groupLayout->addWidget(new QLabel("圆度范围:"), row, 0);
    QHBoxLayout* circLayout = new QHBoxLayout();
    minCircularitySpinBox_ = new QDoubleSpinBox(this);
    minCircularitySpinBox_->setRange(0.0, 1.0);
    minCircularitySpinBox_->setSingleStep(0.05);
    minCircularitySpinBox_->setValue(0.0);
    circLayout->addWidget(minCircularitySpinBox_);
    circLayout->addWidget(new QLabel("~"));
    maxCircularitySpinBox_ = new QDoubleSpinBox(this);
    maxCircularitySpinBox_->setRange(0.0, 1.0);
    maxCircularitySpinBox_->setSingleStep(0.05);
    maxCircularitySpinBox_->setValue(1.0);
    circLayout->addWidget(maxCircularitySpinBox_);
    groupLayout->addLayout(circLayout, row, 1);

    layout->addWidget(group);

    // 说明
    QLabel* noteLabel = new QLabel(
        "说明:\n"
        "- 圆度范围: 0~1, 值越接近1越圆\n"
        "- 面积: 像素平方单位", this);
    noteLabel->setStyleSheet("color: #888; font-size: 11px;");
    noteLabel->setWordWrap(true);
    layout->addWidget(noteLabel);
}

void BlobToolDialog::createSortGroup(QVBoxLayout* layout)
{
    QGroupBox* group = new QGroupBox("排序设置", this);
    QGridLayout* groupLayout = new QGridLayout(group);
    groupLayout->setSpacing(6);

    int row = 0;

    // 排序方式
    groupLayout->addWidget(new QLabel("排序方式:"), row, 0);
    sortByCombo_ = new QComboBox(this);
    sortByCombo_->addItem("不排序", static_cast<int>(Algorithm::BlobTool::NoSort));
    sortByCombo_->addItem("按面积", static_cast<int>(Algorithm::BlobTool::SortByArea));
    sortByCombo_->addItem("按X坐标", static_cast<int>(Algorithm::BlobTool::SortByX));
    sortByCombo_->addItem("按Y坐标", static_cast<int>(Algorithm::BlobTool::SortByY));
    sortByCombo_->addItem("按圆度", static_cast<int>(Algorithm::BlobTool::SortByCircularity));
    groupLayout->addWidget(sortByCombo_, row, 1);
    row++;

    // 降序排列
    groupLayout->addWidget(new QLabel("排序方向:"), row, 0);
    sortDescendingCheckBox_ = new QCheckBox("降序排列", this);
    groupLayout->addWidget(sortDescendingCheckBox_, row, 1);
    row++;

    // 最大数量
    groupLayout->addWidget(new QLabel("最大数量:"), row, 0);
    maxCountSpinBox_ = new QSpinBox(this);
    maxCountSpinBox_->setRange(0, 10000);
    maxCountSpinBox_->setValue(0);
    maxCountSpinBox_->setSpecialValueText("不限制");
    groupLayout->addWidget(maxCountSpinBox_, row, 1);

    layout->addWidget(group);
}

void BlobToolDialog::connectSignals()
{
    // 基本参数
    connect(backendCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &BlobToolDialog::onBackendChanged);
    connect(connectivityCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &BlobToolDialog::onConnectivityChanged);
    connect(polarityCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &BlobToolDialog::onPolarityChanged);

    // 二值化参数
    connect(autoThresholdCheckBox_, &QCheckBox::toggled,
            this, &BlobToolDialog::onAutoThresholdChanged);
    connect(thresholdSpinBox_, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &BlobToolDialog::onThresholdChanged);
    connect(thresholdSlider_, &QSlider::valueChanged, this, [this](int value) {
        thresholdSpinBox_->setValue(value);
    });

    // 过滤参数
    connect(minAreaSpinBox_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &BlobToolDialog::onAreaRangeChanged);
    connect(maxAreaSpinBox_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &BlobToolDialog::onAreaRangeChanged);
    connect(minCircularitySpinBox_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &BlobToolDialog::onCircularityRangeChanged);
    connect(maxCircularitySpinBox_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &BlobToolDialog::onCircularityRangeChanged);

    // 排序参数
    connect(sortByCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &BlobToolDialog::onSortByChanged);
    connect(sortDescendingCheckBox_, &QCheckBox::toggled,
            this, &BlobToolDialog::onSortDescendingChanged);
    connect(maxCountSpinBox_, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &BlobToolDialog::onMaxCountChanged);

    // 实时预览
    connect(autoPreviewCheck_, &QCheckBox::toggled,
            previewHelper_, &PreviewHelper::setAutoPreviewEnabled);
    connect(previewHelper_, &PreviewHelper::previewTriggered,
            this, &BlobToolDialog::onAutoPreview);

    // 对话框按钮
    connect(previewBtn_, &QPushButton::clicked, this, &BlobToolDialog::onPreviewClicked);
    connect(okBtn_, &QPushButton::clicked, this, &BlobToolDialog::onOkClicked);
    connect(cancelBtn_, &QPushButton::clicked, this, &BlobToolDialog::onCancelClicked);
    connect(applyBtn_, &QPushButton::clicked, this, &BlobToolDialog::onApplyClicked);

    // 图像操作按钮
    connect(loadImageBtn_, &QPushButton::clicked, this, &BlobToolDialog::onLoadImageClicked);
    connect(captureImageBtn_, &QPushButton::clicked, this, &BlobToolDialog::onCaptureImageClicked);
}

void BlobToolDialog::onBackendChanged(int index)
{
    if (!tool_) return;
    tool_->setBackend(static_cast<Algorithm::BlobTool::BackendType>(
        backendCombo_->itemData(index).toInt()));
    emit parameterChanged();
    previewHelper_->requestPreview();
}

void BlobToolDialog::onConnectivityChanged(int index)
{
    if (!tool_) return;
    tool_->setConnectivity(static_cast<Algorithm::BlobTool::Connectivity>(
        connectivityCombo_->itemData(index).toInt()));
    emit parameterChanged();
    previewHelper_->requestPreview();
}

void BlobToolDialog::onPolarityChanged(int index)
{
    if (!tool_) return;
    tool_->setPolarity(static_cast<Algorithm::BlobTool::Polarity>(
        polarityCombo_->itemData(index).toInt()));
    emit parameterChanged();
    previewHelper_->requestPreview();
}

void BlobToolDialog::onSortByChanged(int index)
{
    if (!tool_) return;
    tool_->setSortBy(static_cast<Algorithm::BlobTool::SortBy>(
        sortByCombo_->itemData(index).toInt()));
    emit parameterChanged();
    previewHelper_->requestPreview();
}

void BlobToolDialog::onSortDescendingChanged(bool checked)
{
    if (!tool_) return;
    tool_->setSortDescending(checked);
    emit parameterChanged();
    previewHelper_->requestPreview();
}

void BlobToolDialog::onMaxCountChanged(int value)
{
    if (!tool_) return;
    tool_->setMaxCount(value);
    emit parameterChanged();
    previewHelper_->requestPreview();
}

void BlobToolDialog::onThresholdChanged(int value)
{
    if (!tool_) return;
    tool_->setThreshold(value);
    thresholdSlider_->blockSignals(true);
    thresholdSlider_->setValue(value);
    thresholdSlider_->blockSignals(false);
    emit parameterChanged();
    previewHelper_->requestPreview();
}

void BlobToolDialog::onAutoThresholdChanged(bool checked)
{
    if (!tool_) return;
    tool_->setAutoThreshold(checked);
    thresholdSpinBox_->setEnabled(!checked);
    thresholdSlider_->setEnabled(!checked);
    emit parameterChanged();
    previewHelper_->requestPreview();
}

void BlobToolDialog::onAreaRangeChanged()
{
    if (!tool_) return;
    tool_->setAreaRange(minAreaSpinBox_->value(), maxAreaSpinBox_->value());
    emit parameterChanged();
    previewHelper_->requestPreview();
}

void BlobToolDialog::onCircularityRangeChanged()
{
    if (!tool_) return;
    tool_->setCircularityRange(minCircularitySpinBox_->value(), maxCircularitySpinBox_->value());
    emit parameterChanged();
    previewHelper_->requestPreview();
}

void BlobToolDialog::onPreviewClicked()
{
    if (!tool_ || !currentImage_) {
        QMessageBox::warning(this, "警告", "请先加载图像");
        return;
    }

    applyParameters();

    Algorithm::ToolResult result;
    if (tool_->process(currentImage_, result)) {
        if (result.outputImage) {
            imageViewer_->setImage(result.outputImage);
        }
        updateResults();
        LOG_INFO(QString("Blob分析完成，找到 %1 个Blob").arg(tool_->blobCount()));
    } else {
        QMessageBox::warning(this, "分析失败", result.errorMessage);
    }
}

void BlobToolDialog::onOkClicked()
{
    applyParameters();
    accept();
}

void BlobToolDialog::onCancelClicked()
{
    reject();
}

void BlobToolDialog::onApplyClicked()
{
    applyParameters();
    emit parameterChanged();
}

void BlobToolDialog::applyParameters()
{
    if (!tool_) return;

    tool_->setBackend(static_cast<Algorithm::BlobTool::BackendType>(
        backendCombo_->currentData().toInt()));
    tool_->setConnectivity(static_cast<Algorithm::BlobTool::Connectivity>(
        connectivityCombo_->currentData().toInt()));
    tool_->setPolarity(static_cast<Algorithm::BlobTool::Polarity>(
        polarityCombo_->currentData().toInt()));
    tool_->setAutoThreshold(autoThresholdCheckBox_->isChecked());
    tool_->setThreshold(thresholdSpinBox_->value());
    tool_->setAreaRange(minAreaSpinBox_->value(), maxAreaSpinBox_->value());
    tool_->setCircularityRange(minCircularitySpinBox_->value(), maxCircularitySpinBox_->value());
    tool_->setSortBy(static_cast<Algorithm::BlobTool::SortBy>(
        sortByCombo_->currentData().toInt()));
    tool_->setSortDescending(sortDescendingCheckBox_->isChecked());
    tool_->setMaxCount(maxCountSpinBox_->value());

    LOG_INFO("Blob分析参数已应用");
}

void BlobToolDialog::updateResults()
{
    if (!tool_ || !resultsTable_) return;

    const auto& blobs = tool_->blobFeatures();
    resultsTable_->setRowCount(static_cast<int>(blobs.size()));

    for (size_t i = 0; i < blobs.size(); ++i) {
        const auto& blob = blobs[i];
        int row = static_cast<int>(i);

        resultsTable_->setItem(row, 0, new QTableWidgetItem(QString::number(blob.id)));
        resultsTable_->setItem(row, 1, new QTableWidgetItem(QString::number(blob.area, 'f', 1)));
        resultsTable_->setItem(row, 2, new QTableWidgetItem(QString::number(blob.circularity, 'f', 3)));
        resultsTable_->setItem(row, 3, new QTableWidgetItem(QString::number(blob.center.x(), 'f', 1)));
        resultsTable_->setItem(row, 4, new QTableWidgetItem(QString::number(blob.center.y(), 'f', 1)));
    }
}

void BlobToolDialog::onAutoPreview()
{
    if (!tool_ || !currentImage_) {
        return;
    }

    applyParameters();

    Algorithm::ToolResult result;
    if (tool_->process(currentImage_, result)) {
        if (result.outputImage && imageViewer_) {
            imageViewer_->setImage(result.outputImage);
        }
        updateResults();
    }
}

void BlobToolDialog::onLoadImageClicked()
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

void BlobToolDialog::onCaptureImageClicked()
{
    emit captureImageRequested();
    LOG_INFO("请求采集图像");
}

} // namespace UI
} // namespace VisionForge
