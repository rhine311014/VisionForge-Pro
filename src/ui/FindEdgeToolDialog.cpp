/**
 * @file FindEdgeToolDialog.cpp
 * @brief 边缘查找工具参数编辑对话框实现
 * @author VisionForge Team
 * @date 2025-12-18
 */

#include "ui/FindEdgeToolDialog.h"
#include "ui/HalconImageViewer.h"
#include "ui/ROI.h"
#include "algorithm/FindEdgeTool.h"
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

namespace VisionForge {
namespace UI {

FindEdgeToolDialog::FindEdgeToolDialog(Algorithm::FindEdgeTool* tool, QWidget* parent)
    : QDialog(parent)
    , tool_(tool)
    , imageViewer_(nullptr)
    , mainSplitter_(nullptr)
    , paramTabWidget_(nullptr)
    , searchModeCombo_(nullptr)
    , lineSearchGroup_(nullptr)
    , drawSearchLineBtn_(nullptr)
    , searchWidthSpinBox_(nullptr)
    , lineStatusLabel_(nullptr)
    , rectSearchGroup_(nullptr)
    , drawSearchRectBtn_(nullptr)
    , rectStatusLabel_(nullptr)
    , rectAngleSpinBox_(nullptr)
    , clearSearchRegionBtn_(nullptr)
    , edgeTypeCombo_(nullptr)
    , selectModeCombo_(nullptr)
    , minAmplitudeSpinBox_(nullptr)
    , sigmaSpinBox_(nullptr)
    , previewBtn_(nullptr)
    , okBtn_(nullptr)
    , cancelBtn_(nullptr)
    , applyBtn_(nullptr)
    , previewHelper_(nullptr)
    , autoPreviewCheck_(nullptr)
    , loadImageBtn_(nullptr)
    , captureImageBtn_(nullptr)
{
    setWindowTitle("边缘查找设置");
    setMinimumSize(800, 550);
    resize(900, 600);
    setWindowFlags(windowFlags() | Qt::WindowMaximizeButtonHint);

    createUI();
    connectSignals();
    updateUI();
}

FindEdgeToolDialog::~FindEdgeToolDialog()
{
}

void FindEdgeToolDialog::setImage(const Base::ImageData::Ptr& image)
{
    currentImage_ = image;
    if (imageViewer_ && image) {
        imageViewer_->setImage(image);
    }
}

void FindEdgeToolDialog::updateUI()
{
    if (!tool_) return;

    if (!searchModeCombo_ || !edgeTypeCombo_ || !selectModeCombo_) {
        return;
    }

    // 阻塞信号
    searchModeCombo_->blockSignals(true);
    edgeTypeCombo_->blockSignals(true);
    selectModeCombo_->blockSignals(true);
    minAmplitudeSpinBox_->blockSignals(true);
    sigmaSpinBox_->blockSignals(true);
    searchWidthSpinBox_->blockSignals(true);

    // 更新值
    searchModeCombo_->setCurrentIndex(static_cast<int>(tool_->searchMode()));
    edgeTypeCombo_->setCurrentIndex(static_cast<int>(tool_->edgeType()));
    selectModeCombo_->setCurrentIndex(static_cast<int>(tool_->selectMode()));
    minAmplitudeSpinBox_->setValue(tool_->minAmplitude());
    sigmaSpinBox_->setValue(tool_->sigma());
    searchWidthSpinBox_->setValue(tool_->searchWidth());

    // 恢复信号
    searchModeCombo_->blockSignals(false);
    edgeTypeCombo_->blockSignals(false);
    selectModeCombo_->blockSignals(false);
    minAmplitudeSpinBox_->blockSignals(false);
    sigmaSpinBox_->blockSignals(false);
    searchWidthSpinBox_->blockSignals(false);

    updateSearchModeVisibility();
}

void FindEdgeToolDialog::showEvent(QShowEvent* event)
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

void FindEdgeToolDialog::createUI()
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

void FindEdgeToolDialog::createLeftPanel(QWidget* parent)
{
    QVBoxLayout* layout = new QVBoxLayout(parent);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(4);

    // 标题栏布局
    QHBoxLayout* titleLayout = new QHBoxLayout();
    QLabel* titleLabel = new QLabel("检测图像", parent);
    titleLabel->setStyleSheet("font-weight: bold; font-size: 12px;");
    titleLayout->addWidget(titleLabel);
    titleLayout->addStretch();

    // 加载图片按钮
    loadImageBtn_ = new QPushButton(tr("加载图片"), parent);
    loadImageBtn_->setIcon(style()->standardIcon(QStyle::SP_DialogOpenButton));
    loadImageBtn_->setToolTip(tr("从文件加载图片"));
    loadImageBtn_->setMaximumWidth(100);
    titleLayout->addWidget(loadImageBtn_);

    // 采集图像按钮
    captureImageBtn_ = new QPushButton(tr("采集图像"), parent);
    captureImageBtn_->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    captureImageBtn_->setToolTip(tr("从相机采集图像"));
    captureImageBtn_->setMaximumWidth(100);
    titleLayout->addWidget(captureImageBtn_);

    imageViewer_ = new HalconImageViewer(parent);
    imageViewer_->setMinimumSize(400, 300);
    imageViewer_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    layout->addLayout(titleLayout);
    layout->addWidget(imageViewer_, 1);
}

void FindEdgeToolDialog::createRightPanel(QWidget* parent)
{
    QVBoxLayout* layout = new QVBoxLayout(parent);
    layout->setContentsMargins(4, 0, 0, 0);
    layout->setSpacing(8);

    // 选项卡控件
    paramTabWidget_ = new QTabWidget(parent);

    // Tab 1: 搜索区域
    QWidget* searchTab = new QWidget(paramTabWidget_);
    QVBoxLayout* searchLayout = new QVBoxLayout(searchTab);
    searchLayout->setContentsMargins(8, 8, 8, 8);
    searchLayout->setSpacing(8);
    createSearchModeGroup(searchLayout);
    createSearchRegionGroup(searchLayout);
    searchLayout->addStretch();
    paramTabWidget_->addTab(searchTab, "搜索区域");

    // Tab 2: 边缘参数
    QWidget* edgeTab = new QWidget(paramTabWidget_);
    QVBoxLayout* edgeLayout = new QVBoxLayout(edgeTab);
    edgeLayout->setContentsMargins(8, 8, 8, 8);
    edgeLayout->setSpacing(8);
    createEdgeParamGroup(edgeLayout);
    createFilterParamGroup(edgeLayout);
    edgeLayout->addStretch();
    paramTabWidget_->addTab(edgeTab, "边缘参数");

    layout->addWidget(paramTabWidget_, 1);
}

void FindEdgeToolDialog::createSearchModeGroup(QVBoxLayout* layout)
{
    QGroupBox* group = new QGroupBox("搜索模式", this);
    QVBoxLayout* groupLayout = new QVBoxLayout(group);

    QHBoxLayout* modeLayout = new QHBoxLayout();
    modeLayout->addWidget(new QLabel("模式:"));

    searchModeCombo_ = new QComboBox(this);
    searchModeCombo_->addItem("线性搜索 (LineSearch)", static_cast<int>(Algorithm::FindEdgeTool::LineSearch));
    searchModeCombo_->addItem("矩形搜索 (RectSearch)", static_cast<int>(Algorithm::FindEdgeTool::RectSearch));
    modeLayout->addWidget(searchModeCombo_, 1);

    groupLayout->addLayout(modeLayout);
    layout->addWidget(group);
}

void FindEdgeToolDialog::createSearchRegionGroup(QVBoxLayout* layout)
{
    // 线性搜索设置
    lineSearchGroup_ = new QGroupBox("线性搜索设置", this);
    QVBoxLayout* lineLayout = new QVBoxLayout(lineSearchGroup_);

    QHBoxLayout* lineWidthLayout = new QHBoxLayout();
    lineWidthLayout->addWidget(new QLabel("搜索宽度:"));
    searchWidthSpinBox_ = new QSpinBox(this);
    searchWidthSpinBox_->setRange(1, 100);
    searchWidthSpinBox_->setValue(5);
    searchWidthSpinBox_->setSuffix(" px");
    searchWidthSpinBox_->setToolTip("垂直于搜索方向的采样宽度");
    lineWidthLayout->addWidget(searchWidthSpinBox_);
    lineLayout->addLayout(lineWidthLayout);

    drawSearchLineBtn_ = new QPushButton("绘制搜索线", this);
    drawSearchLineBtn_->setToolTip("在图像上绘制搜索线段");
    lineLayout->addWidget(drawSearchLineBtn_);

    lineStatusLabel_ = new QLabel("状态: 未设置", this);
    lineStatusLabel_->setStyleSheet("color: #888;");
    lineLayout->addWidget(lineStatusLabel_);

    layout->addWidget(lineSearchGroup_);

    // 矩形搜索设置
    rectSearchGroup_ = new QGroupBox("矩形搜索设置", this);
    QVBoxLayout* rectLayout = new QVBoxLayout(rectSearchGroup_);

    QHBoxLayout* rectAngleLayout = new QHBoxLayout();
    rectAngleLayout->addWidget(new QLabel("搜索角度:"));
    rectAngleSpinBox_ = new QDoubleSpinBox(this);
    rectAngleSpinBox_->setRange(-180.0, 180.0);
    rectAngleSpinBox_->setValue(0.0);
    rectAngleSpinBox_->setSuffix("°");
    rectAngleSpinBox_->setToolTip("矩形区域的搜索方向角度");
    rectAngleLayout->addWidget(rectAngleSpinBox_);
    rectLayout->addLayout(rectAngleLayout);

    drawSearchRectBtn_ = new QPushButton("绘制搜索矩形", this);
    drawSearchRectBtn_->setToolTip("在图像上绘制搜索矩形区域");
    rectLayout->addWidget(drawSearchRectBtn_);

    rectStatusLabel_ = new QLabel("状态: 未设置", this);
    rectStatusLabel_->setStyleSheet("color: #888;");
    rectLayout->addWidget(rectStatusLabel_);

    layout->addWidget(rectSearchGroup_);

    // 清除按钮
    clearSearchRegionBtn_ = new QPushButton("清除搜索区域", this);
    layout->addWidget(clearSearchRegionBtn_);
}

void FindEdgeToolDialog::createEdgeParamGroup(QVBoxLayout* layout)
{
    QGroupBox* group = new QGroupBox("边缘检测参数", this);
    QGridLayout* groupLayout = new QGridLayout(group);
    groupLayout->setSpacing(6);

    int row = 0;

    // 边缘类型
    groupLayout->addWidget(new QLabel("边缘类型:"), row, 0);
    edgeTypeCombo_ = new QComboBox(this);
    edgeTypeCombo_->addItem("所有边缘 (All)", static_cast<int>(Algorithm::FindEdgeTool::AllEdges));
    edgeTypeCombo_->addItem("上升沿 (Rising)", static_cast<int>(Algorithm::FindEdgeTool::RisingEdge));
    edgeTypeCombo_->addItem("下降沿 (Falling)", static_cast<int>(Algorithm::FindEdgeTool::FallingEdge));
    groupLayout->addWidget(edgeTypeCombo_, row, 1);
    row++;

    // 选择模式
    groupLayout->addWidget(new QLabel("选择模式:"), row, 0);
    selectModeCombo_ = new QComboBox(this);
    selectModeCombo_->addItem("第一个边缘 (First)", static_cast<int>(Algorithm::FindEdgeTool::FirstEdge));
    selectModeCombo_->addItem("最后一个边缘 (Last)", static_cast<int>(Algorithm::FindEdgeTool::LastEdge));
    selectModeCombo_->addItem("最强边缘 (Strongest)", static_cast<int>(Algorithm::FindEdgeTool::StrongestEdge));
    selectModeCombo_->addItem("所有边缘 (All)", static_cast<int>(Algorithm::FindEdgeTool::AllEdgesFound));
    groupLayout->addWidget(selectModeCombo_, row, 1);
    row++;

    // 最小幅值阈值
    groupLayout->addWidget(new QLabel("最小幅值:"), row, 0);
    minAmplitudeSpinBox_ = new QDoubleSpinBox(this);
    minAmplitudeSpinBox_->setRange(0.0, 255.0);
    minAmplitudeSpinBox_->setValue(20.0);
    minAmplitudeSpinBox_->setToolTip("边缘梯度的最小强度阈值");
    groupLayout->addWidget(minAmplitudeSpinBox_, row, 1);

    layout->addWidget(group);
}

void FindEdgeToolDialog::createFilterParamGroup(QVBoxLayout* layout)
{
    QGroupBox* group = new QGroupBox("滤波参数", this);
    QGridLayout* groupLayout = new QGridLayout(group);
    groupLayout->setSpacing(6);

    // 高斯sigma
    groupLayout->addWidget(new QLabel("高斯Sigma:"), 0, 0);
    sigmaSpinBox_ = new QDoubleSpinBox(this);
    sigmaSpinBox_->setRange(0.1, 10.0);
    sigmaSpinBox_->setSingleStep(0.1);
    sigmaSpinBox_->setValue(1.0);
    sigmaSpinBox_->setToolTip("高斯平滑的sigma值，较大值可减少噪声影响");
    groupLayout->addWidget(sigmaSpinBox_, 0, 1);

    layout->addWidget(group);

    // 添加说明
    QLabel* noteLabel = new QLabel(
        "提示:\n"
        "- 上升沿: 从暗到亮的边缘\n"
        "- 下降沿: 从亮到暗的边缘\n"
        "- 增大Sigma可减少噪声，但会降低定位精度", this);
    noteLabel->setStyleSheet("color: #888; font-size: 11px;");
    noteLabel->setWordWrap(true);
    layout->addWidget(noteLabel);
}

void FindEdgeToolDialog::connectSignals()
{
    // 搜索模式
    connect(searchModeCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &FindEdgeToolDialog::onSearchModeChanged);

    // 边缘参数
    connect(edgeTypeCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &FindEdgeToolDialog::onEdgeTypeChanged);
    connect(selectModeCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &FindEdgeToolDialog::onSelectModeChanged);
    connect(minAmplitudeSpinBox_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &FindEdgeToolDialog::onMinAmplitudeChanged);
    connect(sigmaSpinBox_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &FindEdgeToolDialog::onSigmaChanged);
    connect(searchWidthSpinBox_, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &FindEdgeToolDialog::onSearchWidthChanged);

    // 搜索区域按钮
    connect(drawSearchLineBtn_, &QPushButton::clicked, this, &FindEdgeToolDialog::onDrawSearchLineClicked);
    connect(drawSearchRectBtn_, &QPushButton::clicked, this, &FindEdgeToolDialog::onDrawSearchRectClicked);
    connect(clearSearchRegionBtn_, &QPushButton::clicked, this, &FindEdgeToolDialog::onClearSearchRegionClicked);

    // 实时预览
    connect(autoPreviewCheck_, &QCheckBox::toggled,
            previewHelper_, &PreviewHelper::setAutoPreviewEnabled);
    connect(previewHelper_, &PreviewHelper::previewTriggered,
            this, &FindEdgeToolDialog::onAutoPreview);

    // 对话框按钮
    connect(previewBtn_, &QPushButton::clicked, this, &FindEdgeToolDialog::onPreviewClicked);
    connect(okBtn_, &QPushButton::clicked, this, &FindEdgeToolDialog::onOkClicked);
    connect(cancelBtn_, &QPushButton::clicked, this, &FindEdgeToolDialog::onCancelClicked);
    connect(applyBtn_, &QPushButton::clicked, this, &FindEdgeToolDialog::onApplyClicked);

    // 图像操作按钮
    connect(loadImageBtn_, &QPushButton::clicked, this, &FindEdgeToolDialog::onLoadImageClicked);
    connect(captureImageBtn_, &QPushButton::clicked, this, &FindEdgeToolDialog::onCaptureImageClicked);
}

void FindEdgeToolDialog::onSearchModeChanged(int index)
{
    if (!tool_) return;
    tool_->setSearchMode(static_cast<Algorithm::FindEdgeTool::SearchMode>(
        searchModeCombo_->itemData(index).toInt()));
    updateSearchModeVisibility();
    emit parameterChanged();
    previewHelper_->requestPreview();
}

void FindEdgeToolDialog::onEdgeTypeChanged(int index)
{
    if (!tool_) return;
    tool_->setEdgeType(static_cast<Algorithm::FindEdgeTool::EdgeType>(
        edgeTypeCombo_->itemData(index).toInt()));
    emit parameterChanged();
    previewHelper_->requestPreview();
}

void FindEdgeToolDialog::onSelectModeChanged(int index)
{
    if (!tool_) return;
    tool_->setSelectMode(static_cast<Algorithm::FindEdgeTool::SelectMode>(
        selectModeCombo_->itemData(index).toInt()));
    emit parameterChanged();
    previewHelper_->requestPreview();
}

void FindEdgeToolDialog::onMinAmplitudeChanged(double value)
{
    if (!tool_) return;
    tool_->setMinAmplitude(value);
    emit parameterChanged();
    previewHelper_->requestPreview();
}

void FindEdgeToolDialog::onSigmaChanged(double value)
{
    if (!tool_) return;
    tool_->setSigma(value);
    emit parameterChanged();
    previewHelper_->requestPreview();
}

void FindEdgeToolDialog::onSearchWidthChanged(int value)
{
    if (!tool_) return;
    tool_->setSearchWidth(value);
    emit parameterChanged();
    previewHelper_->requestPreview();
}

void FindEdgeToolDialog::onDrawSearchLineClicked()
{
    if (!currentImage_) {
        QMessageBox::warning(this, "警告", "请先加载图像");
        return;
    }

    if (imageViewer_) {
        imageViewer_->setInteractionMode(HalconImageViewer::DrawLine);
        lineStatusLabel_->setText("状态: 正在绘制...");
        lineStatusLabel_->setStyleSheet("color: #2196F3;");
    }
}

void FindEdgeToolDialog::onDrawSearchRectClicked()
{
    if (!currentImage_) {
        QMessageBox::warning(this, "警告", "请先加载图像");
        return;
    }

    if (imageViewer_) {
        imageViewer_->setInteractionMode(HalconImageViewer::DrawRectangle);
        rectStatusLabel_->setText("状态: 正在绘制...");
        rectStatusLabel_->setStyleSheet("color: #2196F3;");
    }
}

void FindEdgeToolDialog::onClearSearchRegionClicked()
{
    if (imageViewer_) {
        imageViewer_->clearROIs();
        lineStatusLabel_->setText("状态: 未设置");
        lineStatusLabel_->setStyleSheet("color: #888;");
        rectStatusLabel_->setText("状态: 未设置");
        rectStatusLabel_->setStyleSheet("color: #888;");
    }
}

void FindEdgeToolDialog::onPreviewClicked()
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
        LOG_INFO(QString("边缘查找完成，找到 %1 个边缘点").arg(tool_->edgeCount()));
    } else {
        QMessageBox::warning(this, "检测失败", result.errorMessage);
    }
}

void FindEdgeToolDialog::onOkClicked()
{
    applyParameters();
    accept();
}

void FindEdgeToolDialog::onCancelClicked()
{
    reject();
}

void FindEdgeToolDialog::onApplyClicked()
{
    applyParameters();
    emit parameterChanged();
}

void FindEdgeToolDialog::applyParameters()
{
    if (!tool_) return;

    tool_->setSearchMode(static_cast<Algorithm::FindEdgeTool::SearchMode>(
        searchModeCombo_->currentData().toInt()));
    tool_->setEdgeType(static_cast<Algorithm::FindEdgeTool::EdgeType>(
        edgeTypeCombo_->currentData().toInt()));
    tool_->setSelectMode(static_cast<Algorithm::FindEdgeTool::SelectMode>(
        selectModeCombo_->currentData().toInt()));
    tool_->setMinAmplitude(minAmplitudeSpinBox_->value());
    tool_->setSigma(sigmaSpinBox_->value());
    tool_->setSearchWidth(searchWidthSpinBox_->value());

    LOG_INFO("边缘查找参数已应用");
}

void FindEdgeToolDialog::updateSearchModeVisibility()
{
    int mode = searchModeCombo_->currentData().toInt();
    bool isLineMode = (mode == static_cast<int>(Algorithm::FindEdgeTool::LineSearch));

    lineSearchGroup_->setVisible(isLineMode);
    rectSearchGroup_->setVisible(!isLineMode);
}

void FindEdgeToolDialog::onAutoPreview()
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
    }
}

void FindEdgeToolDialog::onLoadImageClicked()
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

void FindEdgeToolDialog::onCaptureImageClicked()
{
    emit captureImageRequested();
    LOG_INFO("请求采集图像");
}

} // namespace UI
} // namespace VisionForge
