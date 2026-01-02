/**
 * @file PatternSearchToolDialog.cpp
 * @brief 图案搜索工具对话框实现
 * @author VisionForge Team
 * @date 2025-12-30
 */

#include "ui/PatternSearchToolDialog.h"
#include "ui/HalconImageViewer.h"
#include "ui/ROI.h"
#include "algorithm/PatternSearchTool.h"
#include "base/Logger.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFormLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QStyle>

#ifdef USE_HALCON
#include <HalconCpp.h>
#endif

namespace VisionForge {
namespace UI {

PatternSearchToolDialog::PatternSearchToolDialog(Algorithm::PatternSearchTool* tool, QWidget* parent)
    : SearchToolDialog(tool, parent)
    , patternTool_(tool)
{
    connectSpecificSignals();
    updateSpecificUI();
}

PatternSearchToolDialog::~PatternSearchToolDialog()
{
}

QWidget* PatternSearchToolDialog::createParameterPanel(QWidget* parent)
{
    QWidget* panel = new QWidget(parent);
    QVBoxLayout* layout = new QVBoxLayout(panel);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->setSpacing(8);

    createMatchModeGroup(layout);
    createPatternGroup(layout);
    createSearchParamGroup(layout);
    createAngleScaleGroup(layout);

    layout->addStretch();

    return panel;
}

void PatternSearchToolDialog::createMatchModeGroup(QVBoxLayout* layout)
{
    matchModeGroup_ = new QGroupBox(tr("匹配模式"), this);
    QVBoxLayout* groupLayout = new QVBoxLayout(matchModeGroup_);

    radioGrayMatch_ = new QRadioButton(tr("灰度匹配 - 基于灰度值的模板匹配"), this);
    radioEdgeMatch_ = new QRadioButton(tr("边缘匹配 - 基于边缘轮廓的形状匹配"), this);
    radioCorrelationMatch_ = new QRadioButton(tr("相关匹配 - 归一化相关系数匹配"), this);

    radioEdgeMatch_->setChecked(true);  // 默认使用边缘匹配

    groupLayout->addWidget(radioGrayMatch_);
    groupLayout->addWidget(radioEdgeMatch_);
    groupLayout->addWidget(radioCorrelationMatch_);

    // 说明标签
    QLabel* descLabel = new QLabel(tr("提示: 边缘匹配对光照变化不敏感，适合工业应用"), this);
    descLabel->setStyleSheet("color: #666; font-size: 11px;");
    groupLayout->addWidget(descLabel);

    layout->addWidget(matchModeGroup_);
}

void PatternSearchToolDialog::createPatternGroup(QVBoxLayout* layout)
{
    patternGroup_ = new QGroupBox(tr("图案模板"), this);
    QVBoxLayout* groupLayout = new QVBoxLayout(patternGroup_);

    // 图案状态
    patternStatusLabel_ = new QLabel(tr("状态: 未创建图案模板"), this);
    patternStatusLabel_->setStyleSheet("color: #FF9800; font-weight: bold;");
    groupLayout->addWidget(patternStatusLabel_);

    // 图案预览
    patternPreviewLabel_ = new QLabel(this);
    patternPreviewLabel_->setFixedSize(150, 150);
    patternPreviewLabel_->setFrameStyle(QFrame::Box | QFrame::Sunken);
    patternPreviewLabel_->setAlignment(Qt::AlignCenter);
    patternPreviewLabel_->setText(tr("无预览"));
    patternPreviewLabel_->setStyleSheet("background-color: #f0f0f0; color: #888;");
    groupLayout->addWidget(patternPreviewLabel_, 0, Qt::AlignCenter);

    // 按钮行1
    QHBoxLayout* btnRow1 = new QHBoxLayout();
    createPatternBtn_ = new QPushButton(tr("从ROI创建"), this);
    createPatternBtn_->setIcon(style()->standardIcon(QStyle::SP_FileDialogNewFolder));
    loadPatternBtn_ = new QPushButton(tr("加载图案"), this);
    loadPatternBtn_->setIcon(style()->standardIcon(QStyle::SP_DialogOpenButton));
    btnRow1->addWidget(createPatternBtn_);
    btnRow1->addWidget(loadPatternBtn_);
    groupLayout->addLayout(btnRow1);

    // 按钮行2
    QHBoxLayout* btnRow2 = new QHBoxLayout();
    savePatternBtn_ = new QPushButton(tr("保存图案"), this);
    savePatternBtn_->setIcon(style()->standardIcon(QStyle::SP_DialogSaveButton));
    clearPatternBtn_ = new QPushButton(tr("清除图案"), this);
    clearPatternBtn_->setIcon(style()->standardIcon(QStyle::SP_DialogDiscardButton));
    btnRow2->addWidget(savePatternBtn_);
    btnRow2->addWidget(clearPatternBtn_);
    groupLayout->addLayout(btnRow2);

    layout->addWidget(patternGroup_);
}

void PatternSearchToolDialog::createSearchParamGroup(QVBoxLayout* layout)
{
    searchParamGroup_ = new QGroupBox(tr("搜索参数"), this);
    QGridLayout* groupLayout = new QGridLayout(searchParamGroup_);
    groupLayout->setSpacing(6);

    int row = 0;

    // 最小匹配分数
    groupLayout->addWidget(new QLabel(tr("最小分数:"), this), row, 0);
    QHBoxLayout* scoreLayout = new QHBoxLayout();
    minScoreSpin_ = new QDoubleSpinBox(this);
    minScoreSpin_->setRange(0.0, 1.0);
    minScoreSpin_->setSingleStep(0.05);
    minScoreSpin_->setValue(0.5);
    minScoreSlider_ = new QSlider(Qt::Horizontal, this);
    minScoreSlider_->setRange(0, 100);
    minScoreSlider_->setValue(50);
    scoreLayout->addWidget(minScoreSpin_);
    scoreLayout->addWidget(minScoreSlider_);
    groupLayout->addLayout(scoreLayout, row, 1);
    row++;

    // 最大匹配数
    groupLayout->addWidget(new QLabel(tr("最大匹配数:"), this), row, 0);
    maxMatchesSpin_ = new QSpinBox(this);
    maxMatchesSpin_->setRange(1, 1000);
    maxMatchesSpin_->setValue(10);
    groupLayout->addWidget(maxMatchesSpin_, row, 1);
    row++;

    // 重叠阈值
    groupLayout->addWidget(new QLabel(tr("重叠阈值:"), this), row, 0);
    overlapSpin_ = new QDoubleSpinBox(this);
    overlapSpin_->setRange(0.0, 1.0);
    overlapSpin_->setSingleStep(0.1);
    overlapSpin_->setValue(0.5);
    overlapSpin_->setToolTip(tr("用于非极大值抑制，防止重复检测"));
    groupLayout->addWidget(overlapSpin_, row, 1);
    row++;

    // 亚像素精度
    groupLayout->addWidget(new QLabel(tr("亚像素精度:"), this), row, 0);
    subPixelCombo_ = new QComboBox(this);
    subPixelCombo_->addItem(tr("无"), "none");
    subPixelCombo_->addItem(tr("最近邻插值"), "interpolation");
    subPixelCombo_->addItem(tr("最小二乘"), "least_squares");
    subPixelCombo_->addItem(tr("最小二乘高精度"), "least_squares_high");
    subPixelCombo_->setCurrentIndex(2);
    groupLayout->addWidget(subPixelCombo_, row, 1);
    row++;

    // 金字塔层数
    groupLayout->addWidget(new QLabel(tr("金字塔层数:"), this), row, 0);
    pyramidLevelsSpin_ = new QSpinBox(this);
    pyramidLevelsSpin_->setRange(1, 10);
    pyramidLevelsSpin_->setValue(4);
    pyramidLevelsSpin_->setToolTip(tr("金字塔层数越多搜索越快，但可能降低精度"));
    groupLayout->addWidget(pyramidLevelsSpin_, row, 1);

    layout->addWidget(searchParamGroup_);
}

void PatternSearchToolDialog::createAngleScaleGroup(QVBoxLayout* layout)
{
    angleScaleGroup_ = new QGroupBox(tr("角度和缩放"), this);
    QVBoxLayout* groupLayout = new QVBoxLayout(angleScaleGroup_);

    // 角度搜索
    enableAngleCheck_ = new QCheckBox(tr("启用角度搜索"), this);
    enableAngleCheck_->setChecked(true);
    groupLayout->addWidget(enableAngleCheck_);

    angleWidget_ = new QWidget(this);
    QGridLayout* angleLayout = new QGridLayout(angleWidget_);
    angleLayout->setContentsMargins(20, 0, 0, 0);
    angleLayout->setSpacing(4);

    angleLayout->addWidget(new QLabel(tr("最小角度:"), this), 0, 0);
    angleMinSpin_ = new QDoubleSpinBox(this);
    angleMinSpin_->setRange(-180.0, 180.0);
    angleMinSpin_->setValue(-30.0);
    angleMinSpin_->setSuffix("°");
    angleLayout->addWidget(angleMinSpin_, 0, 1);

    angleLayout->addWidget(new QLabel(tr("最大角度:"), this), 1, 0);
    angleMaxSpin_ = new QDoubleSpinBox(this);
    angleMaxSpin_->setRange(-180.0, 180.0);
    angleMaxSpin_->setValue(30.0);
    angleMaxSpin_->setSuffix("°");
    angleLayout->addWidget(angleMaxSpin_, 1, 1);

    angleLayout->addWidget(new QLabel(tr("角度步长:"), this), 2, 0);
    angleStepSpin_ = new QDoubleSpinBox(this);
    angleStepSpin_->setRange(0.01, 30.0);
    angleStepSpin_->setValue(1.0);
    angleStepSpin_->setSuffix("°");
    angleLayout->addWidget(angleStepSpin_, 2, 1);

    groupLayout->addWidget(angleWidget_);

    // 分隔线
    QFrame* line = new QFrame(this);
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    groupLayout->addWidget(line);

    // 缩放搜索
    enableScaleCheck_ = new QCheckBox(tr("启用缩放搜索"), this);
    enableScaleCheck_->setChecked(false);
    groupLayout->addWidget(enableScaleCheck_);

    scaleWidget_ = new QWidget(this);
    QGridLayout* scaleLayout = new QGridLayout(scaleWidget_);
    scaleLayout->setContentsMargins(20, 0, 0, 0);
    scaleLayout->setSpacing(4);

    scaleLayout->addWidget(new QLabel(tr("最小缩放:"), this), 0, 0);
    scaleMinSpin_ = new QDoubleSpinBox(this);
    scaleMinSpin_->setRange(0.1, 10.0);
    scaleMinSpin_->setValue(0.8);
    scaleMinSpin_->setSingleStep(0.1);
    scaleLayout->addWidget(scaleMinSpin_, 0, 1);

    scaleLayout->addWidget(new QLabel(tr("最大缩放:"), this), 1, 0);
    scaleMaxSpin_ = new QDoubleSpinBox(this);
    scaleMaxSpin_->setRange(0.1, 10.0);
    scaleMaxSpin_->setValue(1.2);
    scaleMaxSpin_->setSingleStep(0.1);
    scaleLayout->addWidget(scaleMaxSpin_, 1, 1);

    scaleLayout->addWidget(new QLabel(tr("缩放步长:"), this), 2, 0);
    scaleStepSpin_ = new QDoubleSpinBox(this);
    scaleStepSpin_->setRange(0.01, 1.0);
    scaleStepSpin_->setValue(0.05);
    scaleStepSpin_->setSingleStep(0.01);
    scaleLayout->addWidget(scaleStepSpin_, 2, 1);

    scaleWidget_->setEnabled(false);
    groupLayout->addWidget(scaleWidget_);

    layout->addWidget(angleScaleGroup_);
}

void PatternSearchToolDialog::connectSpecificSignals()
{
    // 匹配模式
    connect(radioGrayMatch_, &QRadioButton::toggled, [this](bool checked) {
        if (checked) onMatchModeChanged(GrayMatch);
    });
    connect(radioEdgeMatch_, &QRadioButton::toggled, [this](bool checked) {
        if (checked) onMatchModeChanged(EdgeMatch);
    });
    connect(radioCorrelationMatch_, &QRadioButton::toggled, [this](bool checked) {
        if (checked) onMatchModeChanged(CorrelationMatch);
    });

    // 图案管理
    connect(createPatternBtn_, &QPushButton::clicked, this, &PatternSearchToolDialog::onCreatePatternClicked);
    connect(loadPatternBtn_, &QPushButton::clicked, this, &PatternSearchToolDialog::onLoadPatternClicked);
    connect(savePatternBtn_, &QPushButton::clicked, this, &PatternSearchToolDialog::onSavePatternClicked);
    connect(clearPatternBtn_, &QPushButton::clicked, this, &PatternSearchToolDialog::onClearPatternClicked);

    // 分数滑块和数值框同步
    connect(minScoreSlider_, &QSlider::valueChanged, [this](int value) {
        minScoreSpin_->blockSignals(true);
        minScoreSpin_->setValue(value / 100.0);
        minScoreSpin_->blockSignals(false);
        if (previewHelper_) previewHelper_->requestPreview();
    });
    connect(minScoreSpin_, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [this](double value) {
        minScoreSlider_->blockSignals(true);
        minScoreSlider_->setValue(static_cast<int>(value * 100));
        minScoreSlider_->blockSignals(false);
        if (previewHelper_) previewHelper_->requestPreview();
    });

    // 角度缩放启用
    connect(enableAngleCheck_, &QCheckBox::toggled, angleWidget_, &QWidget::setEnabled);
    connect(enableScaleCheck_, &QCheckBox::toggled, scaleWidget_, &QWidget::setEnabled);

    // 参数变化触发预览
    connect(maxMatchesSpin_, QOverload<int>::of(&QSpinBox::valueChanged), [this]() {
        if (previewHelper_) previewHelper_->requestPreview();
    });
    connect(overlapSpin_, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [this]() {
        if (previewHelper_) previewHelper_->requestPreview();
    });
}

bool PatternSearchToolDialog::executeSearch(std::vector<SearchResult>& results)
{
    if (!currentImage_) {
        LOG_WARNING("执行搜索失败: 未加载图像");
        return false;
    }

#ifdef USE_HALCON
    try {
        // 这里实现Halcon形状匹配搜索
        // TODO: 调用patternTool_的搜索方法

        // 模拟搜索结果（实际应调用Halcon API）
        LOG_INFO("图案搜索执行中...");

        // 示例结果
        SearchResult r1;
        r1.index = 1;
        r1.x = 320.5;
        r1.y = 240.3;
        r1.angle = 15.2;
        r1.scale = 1.0;
        r1.score = 0.95;
        results.push_back(r1);

        LOG_INFO(QString("图案搜索完成，找到 %1 个匹配").arg(results.size()));
        return true;
    } catch (const std::exception& e) {
        LOG_ERROR(QString("图案搜索异常: %1").arg(e.what()));
        return false;
    }
#else
    LOG_WARNING("图案搜索需要Halcon支持");
    return false;
#endif
}

void PatternSearchToolDialog::applySpecificParameters()
{
    if (!patternTool_) return;

    // TODO: 将UI参数应用到工具
    // patternTool_->setMinScore(minScoreSpin_->value());
    // patternTool_->setMaxMatches(maxMatchesSpin_->value());
    // ...

    LOG_DEBUG("图案搜索参数已应用");
}

void PatternSearchToolDialog::updateSpecificUI()
{
    if (!patternTool_) return;

    // TODO: 从工具读取参数更新UI
    // minScoreSpin_->setValue(patternTool_->minScore());
    // ...
}

void PatternSearchToolDialog::onCreatePatternClicked()
{
    if (!currentImage_) {
        QMessageBox::warning(this, tr("警告"), tr("请先加载图像"));
        return;
    }

    if (!hasSearchROI()) {
        QMessageBox::warning(this, tr("警告"), tr("请先在搜索区域页面绘制ROI"));
        return;
    }

    QRect roi = getSearchROI();

#ifdef USE_HALCON
    try {
        // TODO: 从ROI创建形状模板
        LOG_INFO(QString("从ROI创建图案模板: (%1,%2) %3x%4")
            .arg(roi.x()).arg(roi.y()).arg(roi.width()).arg(roi.height()));

        patternStatusLabel_->setText(tr("状态: 图案模板已创建"));
        patternStatusLabel_->setStyleSheet("color: #4CAF50; font-weight: bold;");

        // 更新预览
        patternPreviewLabel_->setText(QString("%1x%2").arg(roi.width()).arg(roi.height()));
    } catch (const std::exception& e) {
        QMessageBox::critical(this, tr("错误"),
            tr("创建图案模板失败: %1").arg(e.what()));
    }
#else
    Q_UNUSED(roi);
    QMessageBox::warning(this, tr("警告"), tr("此功能需要Halcon支持"));
#endif
}

void PatternSearchToolDialog::onLoadPatternClicked()
{
    QString filePath = QFileDialog::getOpenFileName(
        this, tr("加载图案模板"),
        QString(),
        tr("形状模型 (*.shm);;所有文件 (*)"));

    if (filePath.isEmpty()) return;

#ifdef USE_HALCON
    try {
        // TODO: 加载形状模板
        LOG_INFO(QString("加载图案模板: %1").arg(filePath));

        patternStatusLabel_->setText(tr("状态: 已加载 - ") + QFileInfo(filePath).fileName());
        patternStatusLabel_->setStyleSheet("color: #4CAF50; font-weight: bold;");
    } catch (const std::exception& e) {
        QMessageBox::critical(this, tr("错误"),
            tr("加载图案模板失败: %1").arg(e.what()));
    }
#else
    QMessageBox::warning(this, tr("警告"), tr("此功能需要Halcon支持"));
#endif
}

void PatternSearchToolDialog::onSavePatternClicked()
{
    QString filePath = QFileDialog::getSaveFileName(
        this, tr("保存图案模板"),
        QString(),
        tr("形状模型 (*.shm)"));

    if (filePath.isEmpty()) return;

    if (!filePath.endsWith(".shm", Qt::CaseInsensitive)) {
        filePath += ".shm";
    }

#ifdef USE_HALCON
    try {
        // TODO: 保存形状模板
        LOG_INFO(QString("保存图案模板: %1").arg(filePath));
        QMessageBox::information(this, tr("成功"), tr("图案模板已保存"));
    } catch (const std::exception& e) {
        QMessageBox::critical(this, tr("错误"),
            tr("保存图案模板失败: %1").arg(e.what()));
    }
#else
    QMessageBox::warning(this, tr("警告"), tr("此功能需要Halcon支持"));
#endif
}

void PatternSearchToolDialog::onClearPatternClicked()
{
    int ret = QMessageBox::question(this, tr("确认"),
        tr("确定要清除当前图案模板吗？"),
        QMessageBox::Yes | QMessageBox::No);

    if (ret == QMessageBox::Yes) {
        // TODO: 清除模板
        patternStatusLabel_->setText(tr("状态: 未创建图案模板"));
        patternStatusLabel_->setStyleSheet("color: #FF9800; font-weight: bold;");
        patternPreviewLabel_->setPixmap(QPixmap());
        patternPreviewLabel_->setText(tr("无预览"));
        LOG_INFO("图案模板已清除");
    }
}

void PatternSearchToolDialog::onMatchModeChanged(int mode)
{
    QString modeText;
    switch (mode) {
        case GrayMatch:
            modeText = tr("灰度匹配");
            break;
        case EdgeMatch:
            modeText = tr("边缘匹配");
            break;
        case CorrelationMatch:
            modeText = tr("相关匹配");
            break;
    }

    LOG_DEBUG(QString("匹配模式切换为: %1").arg(modeText));

    // TODO: 更新工具的匹配模式
    // if (patternTool_) {
    //     patternTool_->setMatchMode(static_cast<MatchMode>(mode));
    // }

    if (previewHelper_) {
        previewHelper_->requestPreview();
    }
}

} // namespace UI
} // namespace VisionForge
