/**
 * @file TemplateMatchToolDialog.cpp
 * @brief 模板匹配工具参数编辑对话框实现 (OpenCV版本)
 * @author VisionForge Team
 * @date 2025-12-18
 */

#include "ui/TemplateMatchToolDialog.h"
#include "ui/HalconImageViewer.h"
#include "ui/ROI.h"
#include "algorithm/TemplateMatchTool.h"
#include "base/Logger.h"
#include "base/ImageMemoryPool.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QSplitter>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QDialogButtonBox>
#include <QFrame>
#include <QTimer>
#include <QStyle>
#include <opencv2/opencv.hpp>

namespace VisionForge {
namespace UI {

TemplateMatchToolDialog::TemplateMatchToolDialog(Algorithm::TemplateMatchTool* tool, QWidget* parent)
    : QDialog(parent)
    , tool_(tool)
    , imageViewer_(nullptr)
    , templateViewer_(nullptr)
    , mainSplitter_(nullptr)
    , paramTabWidget_(nullptr)
    , matchMethodCombo_(nullptr)
    , matchMethodDesc_(nullptr)
    , thresholdSpinBox_(nullptr)
    , thresholdSlider_(nullptr)
    , maxMatchesSpinBox_(nullptr)
    , usePyramidCheckBox_(nullptr)
    , pyramidLevelsSpinBox_(nullptr)
    , nmsDistanceSpinBox_(nullptr)
    , enableAngleSearchCheckBox_(nullptr)
    , angleMinSpinBox_(nullptr)
    , angleMaxSpinBox_(nullptr)
    , angleStepSpinBox_(nullptr)
    , angleSearchWidget_(nullptr)
    , selectTemplateBtn_(nullptr)
    , createFromROIBtn_(nullptr)
    , loadTemplateBtn_(nullptr)
    , saveTemplateBtn_(nullptr)
    , clearTemplateBtn_(nullptr)
    , templateStatusLabel_(nullptr)
    , drawROIBtn_(nullptr)
    , clearROIBtn_(nullptr)
    , roiStatusLabel_(nullptr)
    , previewBtn_(nullptr)
    , okBtn_(nullptr)
    , cancelBtn_(nullptr)
    , applyBtn_(nullptr)
    , previewHelper_(nullptr)
    , autoPreviewCheck_(nullptr)
    , loadImageBtn_(nullptr)
    , captureImageBtn_(nullptr)
{
    setWindowTitle("模板匹配设置 (OpenCV)");
    setMinimumSize(900, 600);
    resize(1000, 650);
    setWindowFlags(windowFlags() | Qt::WindowMaximizeButtonHint);

    createUI();
    connectSignals();
    updateUI();
}

TemplateMatchToolDialog::~TemplateMatchToolDialog()
{
}

void TemplateMatchToolDialog::setImage(const Base::ImageData::Ptr& image)
{
    currentImage_ = image;
    if (imageViewer_ && image) {
        imageViewer_->setImage(image);
    }
}

void TemplateMatchToolDialog::updateUI()
{
    if (!tool_) return;

    // 检查关键控件是否已创建
    if (!matchMethodCombo_ || !thresholdSpinBox_ || !maxMatchesSpinBox_) {
        return;
    }

    // 阻塞所有控件信号
    matchMethodCombo_->blockSignals(true);
    thresholdSpinBox_->blockSignals(true);
    thresholdSlider_->blockSignals(true);
    maxMatchesSpinBox_->blockSignals(true);
    usePyramidCheckBox_->blockSignals(true);
    pyramidLevelsSpinBox_->blockSignals(true);
    nmsDistanceSpinBox_->blockSignals(true);
    enableAngleSearchCheckBox_->blockSignals(true);
    angleMinSpinBox_->blockSignals(true);
    angleMaxSpinBox_->blockSignals(true);
    angleStepSpinBox_->blockSignals(true);

    // 更新匹配方法
    matchMethodCombo_->setCurrentIndex(static_cast<int>(tool_->matchMethod()));

    // 更新阈值
    thresholdSpinBox_->setValue(tool_->threshold());
    thresholdSlider_->setValue(static_cast<int>(tool_->threshold() * 100));

    // 更新最大匹配数
    maxMatchesSpinBox_->setValue(tool_->maxMatches());

    // 更新金字塔参数
    usePyramidCheckBox_->setChecked(tool_->usePyramid());
    pyramidLevelsSpinBox_->setValue(tool_->pyramidLevels());
    pyramidLevelsSpinBox_->setEnabled(tool_->usePyramid());

    // 更新NMS距离
    nmsDistanceSpinBox_->setValue(tool_->nmsDistance());

    // 更新角度搜索参数
    enableAngleSearchCheckBox_->setChecked(tool_->enableAngleSearch());
    angleMinSpinBox_->setValue(tool_->angleMin());
    angleMaxSpinBox_->setValue(tool_->angleMax());
    angleStepSpinBox_->setValue(tool_->angleStep());
    angleSearchWidget_->setEnabled(tool_->enableAngleSearch());

    // 恢复所有控件信号
    matchMethodCombo_->blockSignals(false);
    thresholdSpinBox_->blockSignals(false);
    thresholdSlider_->blockSignals(false);
    maxMatchesSpinBox_->blockSignals(false);
    usePyramidCheckBox_->blockSignals(false);
    pyramidLevelsSpinBox_->blockSignals(false);
    nmsDistanceSpinBox_->blockSignals(false);
    enableAngleSearchCheckBox_->blockSignals(false);
    angleMinSpinBox_->blockSignals(false);
    angleMaxSpinBox_->blockSignals(false);
    angleStepSpinBox_->blockSignals(false);

    // 更新模板状态
    if (tool_->hasTemplate()) {
        templateStatusLabel_->setText("模板状态: 已加载");
        templateStatusLabel_->setStyleSheet("color: #4CAF50;");
    } else {
        templateStatusLabel_->setText("模板状态: 未加载");
        templateStatusLabel_->setStyleSheet("color: #FF9800;");
    }

    // 更新模板预览
    updateTemplatePreview();
}

void TemplateMatchToolDialog::showEvent(QShowEvent* event)
{
    QDialog::showEvent(event);
    updateUI();

    // 延迟设置图像
    if (currentImage_ && imageViewer_) {
        QTimer::singleShot(200, this, [this]() {
            if (currentImage_ && imageViewer_) {
                imageViewer_->setImage(currentImage_);
            }
        });
    }
}

void TemplateMatchToolDialog::createUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(8, 8, 8, 8);
    mainLayout->setSpacing(8);

    // 主分割器
    mainSplitter_ = new QSplitter(Qt::Horizontal, this);

    // 左侧面板 - 图像显示
    QWidget* leftPanel = new QWidget(mainSplitter_);
    createLeftPanel(leftPanel);

    // 右侧面板 - 参数设置
    QWidget* rightPanel = new QWidget(mainSplitter_);
    createRightPanel(rightPanel);

    mainSplitter_->addWidget(leftPanel);
    mainSplitter_->addWidget(rightPanel);
    mainSplitter_->setStretchFactor(0, 3);
    mainSplitter_->setStretchFactor(1, 2);

    mainLayout->addWidget(mainSplitter_, 1);

    // 创建预览辅助器
    previewHelper_ = new PreviewHelper(this, 200);  // 模板匹配稍长延迟

    // 底部按钮
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

void TemplateMatchToolDialog::createLeftPanel(QWidget* parent)
{
    QVBoxLayout* layout = new QVBoxLayout(parent);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(4);

    // 使用垂直分割器分隔图像和模板预览
    QSplitter* splitter = new QSplitter(Qt::Vertical, parent);

    // 主图像显示器
    QWidget* imageWidget = new QWidget(splitter);
    QVBoxLayout* imageLayout = new QVBoxLayout(imageWidget);
    imageLayout->setContentsMargins(0, 0, 0, 0);

    // 标题栏布局
    QHBoxLayout* titleLayout = new QHBoxLayout();
    QLabel* titleLabel = new QLabel("搜索图像", imageWidget);
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

    // 模板预览
    QWidget* templateWidget = new QWidget(splitter);
    QVBoxLayout* templateLayout = new QVBoxLayout(templateWidget);
    templateLayout->setContentsMargins(0, 0, 0, 0);

    QLabel* templateTitle = new QLabel("模板预览", templateWidget);
    templateTitle->setStyleSheet("font-weight: bold; font-size: 12px;");

    templateViewer_ = new HalconImageViewer(templateWidget);
    templateViewer_->setFixedHeight(150);
    templateViewer_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    templateLayout->addWidget(templateTitle);
    templateLayout->addWidget(templateViewer_);

    splitter->addWidget(imageWidget);
    splitter->addWidget(templateWidget);
    splitter->setStretchFactor(0, 3);
    splitter->setStretchFactor(1, 1);

    layout->addWidget(splitter);
}

void TemplateMatchToolDialog::createRightPanel(QWidget* parent)
{
    QVBoxLayout* layout = new QVBoxLayout(parent);
    layout->setContentsMargins(4, 0, 0, 0);
    layout->setSpacing(8);

    // 选项卡控件
    paramTabWidget_ = new QTabWidget(parent);

    // Tab 1: 匹配方法
    QWidget* methodTab = new QWidget(paramTabWidget_);
    QVBoxLayout* methodLayout = new QVBoxLayout(methodTab);
    methodLayout->setContentsMargins(8, 8, 8, 8);
    methodLayout->setSpacing(8);
    createMatchMethodGroup(methodLayout);
    methodLayout->addStretch();
    paramTabWidget_->addTab(methodTab, "匹配方法");

    // Tab 2: 匹配参数
    QWidget* paramTab = new QWidget(paramTabWidget_);
    QVBoxLayout* paramLayout = new QVBoxLayout(paramTab);
    paramLayout->setContentsMargins(8, 8, 8, 8);
    paramLayout->setSpacing(8);
    createMatchParamGroup(paramLayout);
    createAngleSearchGroup(paramLayout);
    paramLayout->addStretch();
    paramTabWidget_->addTab(paramTab, "匹配参数");

    // Tab 3: 模板管理
    QWidget* templateTab = new QWidget(paramTabWidget_);
    QVBoxLayout* templateTabLayout = new QVBoxLayout(templateTab);
    templateTabLayout->setContentsMargins(8, 8, 8, 8);
    templateTabLayout->setSpacing(8);
    createTemplateGroup(templateTabLayout);
    templateTabLayout->addStretch();
    paramTabWidget_->addTab(templateTab, "模板管理");

    layout->addWidget(paramTabWidget_, 1);
}

void TemplateMatchToolDialog::createMatchMethodGroup(QVBoxLayout* layout)
{
    QGroupBox* group = new QGroupBox("匹配方法", this);
    QVBoxLayout* groupLayout = new QVBoxLayout(group);

    // 匹配方法选择
    QHBoxLayout* methodLayout = new QHBoxLayout();
    methodLayout->addWidget(new QLabel("方法:"));

    matchMethodCombo_ = new QComboBox(this);
    matchMethodCombo_->addItem("平方差匹配 (SQDIFF)", static_cast<int>(Algorithm::MatchMethod::SQDIFF));
    matchMethodCombo_->addItem("归一化平方差 (SQDIFF_NORMED)", static_cast<int>(Algorithm::MatchMethod::SQDIFF_NORMED));
    matchMethodCombo_->addItem("相关匹配 (CCORR)", static_cast<int>(Algorithm::MatchMethod::CCORR));
    matchMethodCombo_->addItem("归一化相关 (CCORR_NORMED)", static_cast<int>(Algorithm::MatchMethod::CCORR_NORMED));
    matchMethodCombo_->addItem("相关系数 (CCOEFF)", static_cast<int>(Algorithm::MatchMethod::CCOEFF));
    matchMethodCombo_->addItem("归一化相关系数 (CCOEFF_NORMED)", static_cast<int>(Algorithm::MatchMethod::CCOEFF_NORMED));
    methodLayout->addWidget(matchMethodCombo_, 1);

    groupLayout->addLayout(methodLayout);

    // 方法描述
    matchMethodDesc_ = new QLabel(this);
    matchMethodDesc_->setWordWrap(true);
    matchMethodDesc_->setStyleSheet("color: #888; font-size: 11px;");
    matchMethodDesc_->setText("CCOEFF_NORMED: 归一化相关系数匹配，值越大匹配越好，范围[-1, 1]");
    groupLayout->addWidget(matchMethodDesc_);

    layout->addWidget(group);
}

void TemplateMatchToolDialog::createMatchParamGroup(QVBoxLayout* layout)
{
    QGroupBox* group = new QGroupBox("匹配参数", this);
    QGridLayout* groupLayout = new QGridLayout(group);
    groupLayout->setSpacing(6);

    int row = 0;

    // 匹配阈值
    groupLayout->addWidget(new QLabel("匹配阈值:"), row, 0);
    QHBoxLayout* thresholdLayout = new QHBoxLayout();
    thresholdSpinBox_ = new QDoubleSpinBox(this);
    thresholdSpinBox_->setRange(0.0, 1.0);
    thresholdSpinBox_->setSingleStep(0.05);
    thresholdSpinBox_->setValue(0.8);
    thresholdSlider_ = new QSlider(Qt::Horizontal, this);
    thresholdSlider_->setRange(0, 100);
    thresholdSlider_->setValue(80);
    thresholdLayout->addWidget(thresholdSpinBox_);
    thresholdLayout->addWidget(thresholdSlider_);
    groupLayout->addLayout(thresholdLayout, row, 1);
    row++;

    // 最大匹配数量
    groupLayout->addWidget(new QLabel("最大匹配数:"), row, 0);
    maxMatchesSpinBox_ = new QSpinBox(this);
    maxMatchesSpinBox_->setRange(1, 100);
    maxMatchesSpinBox_->setValue(10);
    groupLayout->addWidget(maxMatchesSpinBox_, row, 1);
    row++;

    // 金字塔加速
    groupLayout->addWidget(new QLabel("金字塔加速:"), row, 0);
    QHBoxLayout* pyramidLayout = new QHBoxLayout();
    usePyramidCheckBox_ = new QCheckBox("启用", this);
    usePyramidCheckBox_->setChecked(true);
    pyramidLayout->addWidget(usePyramidCheckBox_);
    pyramidLayout->addWidget(new QLabel("层数:"));
    pyramidLevelsSpinBox_ = new QSpinBox(this);
    pyramidLevelsSpinBox_->setRange(1, 6);
    pyramidLevelsSpinBox_->setValue(3);
    pyramidLayout->addWidget(pyramidLevelsSpinBox_);
    pyramidLayout->addStretch();
    groupLayout->addLayout(pyramidLayout, row, 1);
    row++;

    // NMS距离
    groupLayout->addWidget(new QLabel("NMS距离:"), row, 0);
    nmsDistanceSpinBox_ = new QDoubleSpinBox(this);
    nmsDistanceSpinBox_->setRange(1.0, 500.0);
    nmsDistanceSpinBox_->setValue(50.0);
    nmsDistanceSpinBox_->setSuffix(" px");
    groupLayout->addWidget(nmsDistanceSpinBox_, row, 1);

    layout->addWidget(group);
}

void TemplateMatchToolDialog::createAngleSearchGroup(QVBoxLayout* layout)
{
    QGroupBox* group = new QGroupBox("角度搜索", this);
    QVBoxLayout* groupLayout = new QVBoxLayout(group);

    // 启用角度搜索
    enableAngleSearchCheckBox_ = new QCheckBox("启用角度搜索", this);
    groupLayout->addWidget(enableAngleSearchCheckBox_);

    // 角度参数容器
    angleSearchWidget_ = new QWidget(this);
    QGridLayout* angleLayout = new QGridLayout(angleSearchWidget_);
    angleLayout->setContentsMargins(0, 0, 0, 0);

    angleLayout->addWidget(new QLabel("最小角度:"), 0, 0);
    angleMinSpinBox_ = new QDoubleSpinBox(this);
    angleMinSpinBox_->setRange(-180.0, 180.0);
    angleMinSpinBox_->setValue(-30.0);
    angleMinSpinBox_->setSuffix("°");
    angleLayout->addWidget(angleMinSpinBox_, 0, 1);

    angleLayout->addWidget(new QLabel("最大角度:"), 1, 0);
    angleMaxSpinBox_ = new QDoubleSpinBox(this);
    angleMaxSpinBox_->setRange(-180.0, 180.0);
    angleMaxSpinBox_->setValue(30.0);
    angleMaxSpinBox_->setSuffix("°");
    angleLayout->addWidget(angleMaxSpinBox_, 1, 1);

    angleLayout->addWidget(new QLabel("角度步长:"), 2, 0);
    angleStepSpinBox_ = new QDoubleSpinBox(this);
    angleStepSpinBox_->setRange(0.1, 30.0);
    angleStepSpinBox_->setValue(5.0);
    angleStepSpinBox_->setSuffix("°");
    angleLayout->addWidget(angleStepSpinBox_, 2, 1);

    groupLayout->addWidget(angleSearchWidget_);
    angleSearchWidget_->setEnabled(false);

    layout->addWidget(group);
}

void TemplateMatchToolDialog::createTemplateGroup(QVBoxLayout* layout)
{
    QGroupBox* group = new QGroupBox("模板管理", this);
    QVBoxLayout* groupLayout = new QVBoxLayout(group);

    // 模板状态
    templateStatusLabel_ = new QLabel("模板状态: 未加载", this);
    templateStatusLabel_->setStyleSheet("color: #FF9800;");
    groupLayout->addWidget(templateStatusLabel_);

    // ROI操作
    QGroupBox* roiGroup = new QGroupBox("ROI设置", this);
    QVBoxLayout* roiLayout = new QVBoxLayout(roiGroup);

    QHBoxLayout* roiBtnLayout = new QHBoxLayout();
    drawROIBtn_ = new QPushButton("绘制ROI", this);
    clearROIBtn_ = new QPushButton("清除ROI", this);
    roiBtnLayout->addWidget(drawROIBtn_);
    roiBtnLayout->addWidget(clearROIBtn_);
    roiLayout->addLayout(roiBtnLayout);

    roiStatusLabel_ = new QLabel("ROI状态: 未设置", this);
    roiStatusLabel_->setStyleSheet("color: #888;");
    roiLayout->addWidget(roiStatusLabel_);

    groupLayout->addWidget(roiGroup);

    // 模板操作按钮
    QHBoxLayout* btnRow1 = new QHBoxLayout();
    selectTemplateBtn_ = new QPushButton("选择模板图像", this);
    createFromROIBtn_ = new QPushButton("从ROI创建", this);
    btnRow1->addWidget(selectTemplateBtn_);
    btnRow1->addWidget(createFromROIBtn_);
    groupLayout->addLayout(btnRow1);

    QHBoxLayout* btnRow2 = new QHBoxLayout();
    loadTemplateBtn_ = new QPushButton("加载模板", this);
    saveTemplateBtn_ = new QPushButton("保存模板", this);
    btnRow2->addWidget(loadTemplateBtn_);
    btnRow2->addWidget(saveTemplateBtn_);
    groupLayout->addLayout(btnRow2);

    clearTemplateBtn_ = new QPushButton("清除模板", this);
    groupLayout->addWidget(clearTemplateBtn_);

    layout->addWidget(group);
}

void TemplateMatchToolDialog::connectSignals()
{
    // 匹配方法
    connect(matchMethodCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &TemplateMatchToolDialog::onMatchMethodChanged);

    // 匹配参数
    connect(thresholdSpinBox_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &TemplateMatchToolDialog::onThresholdChanged);
    connect(thresholdSlider_, &QSlider::valueChanged, this, [this](int value) {
        thresholdSpinBox_->setValue(value / 100.0);
    });
    connect(maxMatchesSpinBox_, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &TemplateMatchToolDialog::onMaxMatchesChanged);
    connect(usePyramidCheckBox_, &QCheckBox::toggled,
            this, &TemplateMatchToolDialog::onUsePyramidChanged);
    connect(pyramidLevelsSpinBox_, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &TemplateMatchToolDialog::onPyramidLevelsChanged);
    connect(nmsDistanceSpinBox_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &TemplateMatchToolDialog::onNmsDistanceChanged);

    // 角度搜索
    connect(enableAngleSearchCheckBox_, &QCheckBox::toggled,
            this, &TemplateMatchToolDialog::onEnableAngleSearchChanged);
    connect(angleMinSpinBox_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &TemplateMatchToolDialog::onAngleRangeChanged);
    connect(angleMaxSpinBox_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &TemplateMatchToolDialog::onAngleRangeChanged);
    connect(angleStepSpinBox_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &TemplateMatchToolDialog::onAngleRangeChanged);

    // ROI操作
    connect(drawROIBtn_, &QPushButton::clicked, this, &TemplateMatchToolDialog::onDrawROIClicked);
    connect(clearROIBtn_, &QPushButton::clicked, this, &TemplateMatchToolDialog::onClearROIClicked);

    // 模板操作
    connect(selectTemplateBtn_, &QPushButton::clicked, this, &TemplateMatchToolDialog::onSelectTemplateClicked);
    connect(createFromROIBtn_, &QPushButton::clicked, this, &TemplateMatchToolDialog::onCreateTemplateFromROIClicked);
    connect(loadTemplateBtn_, &QPushButton::clicked, this, &TemplateMatchToolDialog::onLoadTemplateClicked);
    connect(saveTemplateBtn_, &QPushButton::clicked, this, &TemplateMatchToolDialog::onSaveTemplateClicked);
    connect(clearTemplateBtn_, &QPushButton::clicked, this, &TemplateMatchToolDialog::onClearTemplateClicked);

    // ROI创建完成信号
    if (imageViewer_) {
        connect(imageViewer_, &HalconImageViewer::roiCreated, this, [this](ROIShapePtr roi) {
            Q_UNUSED(roi);
            std::vector<ROIShapePtr> rois = imageViewer_->getROIs();
            roiStatusLabel_->setText(QString("ROI状态: 已设置 (%1个)").arg(rois.size()));
            roiStatusLabel_->setStyleSheet("color: #4CAF50;");
            imageViewer_->setInteractionMode(HalconImageViewer::SelectMode);
        });
    }

    // 实时预览
    connect(autoPreviewCheck_, &QCheckBox::toggled,
            previewHelper_, &PreviewHelper::setAutoPreviewEnabled);
    connect(previewHelper_, &PreviewHelper::previewTriggered,
            this, &TemplateMatchToolDialog::onAutoPreview);

    // 预览和对话框按钮
    connect(previewBtn_, &QPushButton::clicked, this, &TemplateMatchToolDialog::onPreviewClicked);
    connect(okBtn_, &QPushButton::clicked, this, &TemplateMatchToolDialog::onOkClicked);
    connect(cancelBtn_, &QPushButton::clicked, this, &TemplateMatchToolDialog::onCancelClicked);
    connect(applyBtn_, &QPushButton::clicked, this, &TemplateMatchToolDialog::onApplyClicked);

    // 图像操作按钮
    connect(loadImageBtn_, &QPushButton::clicked, this, &TemplateMatchToolDialog::onLoadImageClicked);
    connect(captureImageBtn_, &QPushButton::clicked, this, &TemplateMatchToolDialog::onCaptureImageClicked);
}

void TemplateMatchToolDialog::onMatchMethodChanged(int index)
{
    if (!tool_) return;

    Algorithm::MatchMethod method = static_cast<Algorithm::MatchMethod>(
        matchMethodCombo_->itemData(index).toInt());
    tool_->setMatchMethod(method);

    // 更新描述
    switch (method) {
        case Algorithm::MatchMethod::SQDIFF:
            matchMethodDesc_->setText("SQDIFF: 平方差匹配，值越小匹配越好");
            break;
        case Algorithm::MatchMethod::SQDIFF_NORMED:
            matchMethodDesc_->setText("SQDIFF_NORMED: 归一化平方差匹配，值越小匹配越好，范围[0, 1]");
            break;
        case Algorithm::MatchMethod::CCORR:
            matchMethodDesc_->setText("CCORR: 相关匹配，值越大匹配越好");
            break;
        case Algorithm::MatchMethod::CCORR_NORMED:
            matchMethodDesc_->setText("CCORR_NORMED: 归一化相关匹配，值越大匹配越好，范围[0, 1]");
            break;
        case Algorithm::MatchMethod::CCOEFF:
            matchMethodDesc_->setText("CCOEFF: 相关系数匹配，值越大匹配越好");
            break;
        case Algorithm::MatchMethod::CCOEFF_NORMED:
            matchMethodDesc_->setText("CCOEFF_NORMED: 归一化相关系数匹配，值越大匹配越好，范围[-1, 1]");
            break;
    }

    emit parameterChanged();
    previewHelper_->requestPreview();
}

void TemplateMatchToolDialog::onThresholdChanged(double value)
{
    if (!tool_) return;
    tool_->setThreshold(value);
    thresholdSlider_->blockSignals(true);
    thresholdSlider_->setValue(static_cast<int>(value * 100));
    thresholdSlider_->blockSignals(false);
    emit parameterChanged();
    previewHelper_->requestPreview();
}

void TemplateMatchToolDialog::onMaxMatchesChanged(int value)
{
    if (!tool_) return;
    tool_->setMaxMatches(value);
    emit parameterChanged();
    previewHelper_->requestPreview();
}

void TemplateMatchToolDialog::onPyramidLevelsChanged(int value)
{
    if (!tool_) return;
    tool_->setPyramidLevels(value);
    emit parameterChanged();
    previewHelper_->requestPreview();
}

void TemplateMatchToolDialog::onUsePyramidChanged(bool checked)
{
    if (!tool_) return;
    tool_->setUsePyramid(checked);
    pyramidLevelsSpinBox_->setEnabled(checked);
    emit parameterChanged();
    previewHelper_->requestPreview();
}

void TemplateMatchToolDialog::onEnableAngleSearchChanged(bool checked)
{
    if (!tool_) return;
    tool_->setEnableAngleSearch(checked);
    angleSearchWidget_->setEnabled(checked);
    emit parameterChanged();
    previewHelper_->requestPreview();
}

void TemplateMatchToolDialog::onAngleRangeChanged()
{
    if (!tool_) return;
    tool_->setAngleRange(
        angleMinSpinBox_->value(),
        angleMaxSpinBox_->value(),
        angleStepSpinBox_->value()
    );
    emit parameterChanged();
    previewHelper_->requestPreview();
}

void TemplateMatchToolDialog::onNmsDistanceChanged(double value)
{
    if (!tool_) return;
    tool_->setNmsDistance(value);
    emit parameterChanged();
    previewHelper_->requestPreview();
}

void TemplateMatchToolDialog::onSelectTemplateClicked()
{
    QString filePath = QFileDialog::getOpenFileName(
        this, "选择模板图像",
        QString(),
        "图像文件 (*.png *.jpg *.bmp *.tif);;所有文件 (*)");

    if (!filePath.isEmpty() && tool_) {
        cv::Mat templateImg = cv::imread(filePath.toStdString());
        if (!templateImg.empty()) {
            tool_->setTemplate(templateImg);
            updateUI();
            LOG_INFO(QString("已设置模板图像: %1").arg(filePath));
        } else {
            QMessageBox::warning(this, "错误", "无法加载图像文件");
        }
    }
}

void TemplateMatchToolDialog::onCreateTemplateFromROIClicked()
{
    if (!tool_ || !currentImage_) {
        QMessageBox::warning(this, "警告", "请先加载图像");
        return;
    }

    std::vector<ROIShapePtr> rois = imageViewer_->getROIs();
    if (rois.empty()) {
        QMessageBox::warning(this, "警告", "请先绘制ROI区域");
        return;
    }

    ROIShapePtr roi = rois.front();
    QRect rect = roi->boundingRect();

    cv::Mat mat = currentImage_->mat();
    QRectF roiRect(rect.x(), rect.y(), rect.width(), rect.height());
    tool_->createTemplateFromROI(mat, roiRect);

    updateUI();
    LOG_INFO("已从ROI创建模板");
}

void TemplateMatchToolDialog::onLoadTemplateClicked()
{
    QString filePath = QFileDialog::getOpenFileName(
        this, "加载模板",
        QString(),
        "模板文件 (*.png *.jpg *.bmp *.tif);;所有文件 (*)");

    if (!filePath.isEmpty() && tool_) {
        if (tool_->loadTemplate(filePath)) {
            updateUI();
            LOG_INFO(QString("已加载模板: %1").arg(filePath));
        } else {
            QMessageBox::warning(this, "错误", "无法加载模板文件");
        }
    }
}

void TemplateMatchToolDialog::onSaveTemplateClicked()
{
    if (!tool_ || !tool_->hasTemplate()) {
        QMessageBox::warning(this, "警告", "没有模板可保存");
        return;
    }

    QString filePath = QFileDialog::getSaveFileName(
        this, "保存模板",
        QString(),
        "PNG图像 (*.png);;JPEG图像 (*.jpg);;所有文件 (*)");

    if (!filePath.isEmpty()) {
        if (tool_->saveTemplate(filePath)) {
            LOG_INFO(QString("已保存模板: %1").arg(filePath));
        } else {
            QMessageBox::warning(this, "错误", "无法保存模板文件");
        }
    }
}

void TemplateMatchToolDialog::onClearTemplateClicked()
{
    if (!tool_) return;

    int ret = QMessageBox::question(this, "确认",
                                    "确定要清除当前模板吗？",
                                    QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::Yes) {
        tool_->setTemplate(cv::Mat());
        updateUI();
        LOG_INFO("模板已清除");
    }
}

void TemplateMatchToolDialog::onDrawROIClicked()
{
    if (!currentImage_) {
        QMessageBox::warning(this, "警告", "请先加载图像");
        return;
    }

    if (imageViewer_) {
        imageViewer_->setInteractionMode(HalconImageViewer::DrawRectangle);
        roiStatusLabel_->setText("ROI状态: 正在绘制...");
        roiStatusLabel_->setStyleSheet("color: #2196F3;");
    }
}

void TemplateMatchToolDialog::onClearROIClicked()
{
    if (imageViewer_) {
        imageViewer_->clearROIs();
        roiStatusLabel_->setText("ROI状态: 未设置");
        roiStatusLabel_->setStyleSheet("color: #888;");
    }
}

void TemplateMatchToolDialog::onPreviewClicked()
{
    if (!tool_ || !currentImage_) {
        QMessageBox::warning(this, "警告", "请先加载图像和模板");
        return;
    }

    if (!tool_->hasTemplate()) {
        QMessageBox::warning(this, "警告", "请先设置模板");
        return;
    }

    applyParameters();

    Algorithm::ToolResult result;
    if (tool_->process(currentImage_, result)) {
        if (result.outputImage) {
            imageViewer_->setImage(result.outputImage);
        }
        LOG_INFO(QString("匹配完成，找到 %1 个结果").arg(tool_->allMatches().size()));
    } else {
        QMessageBox::warning(this, "预览失败", result.errorMessage);
    }
}

void TemplateMatchToolDialog::onOkClicked()
{
    applyParameters();
    accept();
}

void TemplateMatchToolDialog::onCancelClicked()
{
    reject();
}

void TemplateMatchToolDialog::onApplyClicked()
{
    applyParameters();
    emit parameterChanged();
}

void TemplateMatchToolDialog::applyParameters()
{
    if (!tool_) return;

    tool_->setMatchMethod(static_cast<Algorithm::MatchMethod>(
        matchMethodCombo_->currentData().toInt()));
    tool_->setThreshold(thresholdSpinBox_->value());
    tool_->setMaxMatches(maxMatchesSpinBox_->value());
    tool_->setUsePyramid(usePyramidCheckBox_->isChecked());
    tool_->setPyramidLevels(pyramidLevelsSpinBox_->value());
    tool_->setNmsDistance(nmsDistanceSpinBox_->value());
    tool_->setEnableAngleSearch(enableAngleSearchCheckBox_->isChecked());
    tool_->setAngleRange(
        angleMinSpinBox_->value(),
        angleMaxSpinBox_->value(),
        angleStepSpinBox_->value()
    );

    LOG_INFO("参数已应用");
}

void TemplateMatchToolDialog::updateTemplatePreview()
{
    if (!tool_ || !templateViewer_) return;

    if (tool_->hasTemplate()) {
        cv::Mat templ = tool_->templateImage();
        // 使用内存池分配模板图像
        auto templateImage = Base::ImageMemoryPool::instance().allocate(
            templ.cols, templ.rows, templ.type());
        if (templateImage) {
            templ.copyTo(templateImage->mat());
            templateViewer_->setImage(templateImage);
        }
    } else {
        templateViewer_->clearImage();
    }
}

void TemplateMatchToolDialog::onAutoPreview()
{
    // 自动预览需要有图像和模板
    if (!tool_ || !currentImage_ || !tool_->hasTemplate()) {
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

void TemplateMatchToolDialog::onLoadImageClicked()
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

void TemplateMatchToolDialog::onCaptureImageClicked()
{
    emit captureImageRequested();
    LOG_INFO("请求采集图像");
}

} // namespace UI
} // namespace VisionForge
