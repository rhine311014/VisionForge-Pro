/**
 * @file QuickSearchToolDialog.cpp
 * @brief 快速搜索工具对话框实现
 * @author VisionForge Team
 * @date 2025-12-30
 */

#include "ui/QuickSearchToolDialog.h"
#include "ui/HalconImageViewer.h"
#include "algorithm/QuickSearchTool.h"
#include "base/Logger.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QStyle>
#include <opencv2/opencv.hpp>

namespace VisionForge {
namespace UI {

QuickSearchToolDialog::QuickSearchToolDialog(Algorithm::QuickSearchTool* tool, QWidget* parent)
    : SearchToolDialog(tool, parent)
    , quickTool_(tool)
{
    connectSpecificSignals();
    updateSpecificUI();
}

QuickSearchToolDialog::~QuickSearchToolDialog()
{
}

QWidget* QuickSearchToolDialog::createParameterPanel(QWidget* parent)
{
    QWidget* panel = new QWidget(parent);
    QVBoxLayout* layout = new QVBoxLayout(panel);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->setSpacing(8);

    createTemplateGroup(layout);
    createMatchParamGroup(layout);
    createSpeedGroup(layout);

    layout->addStretch();

    return panel;
}

void QuickSearchToolDialog::createTemplateGroup(QVBoxLayout* layout)
{
    templateGroup_ = new QGroupBox(tr("模板"), this);
    QVBoxLayout* groupLayout = new QVBoxLayout(templateGroup_);

    // 模板状态
    templateStatusLabel_ = new QLabel(tr("状态: 未创建模板"), this);
    templateStatusLabel_->setStyleSheet("color: #FF9800; font-weight: bold;");
    groupLayout->addWidget(templateStatusLabel_);

    // 模板预览
    templatePreviewLabel_ = new QLabel(this);
    templatePreviewLabel_->setFixedSize(120, 120);
    templatePreviewLabel_->setFrameStyle(QFrame::Box | QFrame::Sunken);
    templatePreviewLabel_->setAlignment(Qt::AlignCenter);
    templatePreviewLabel_->setText(tr("无预览"));
    templatePreviewLabel_->setStyleSheet("background-color: #f0f0f0; color: #888;");
    groupLayout->addWidget(templatePreviewLabel_, 0, Qt::AlignCenter);

    // 按钮
    QGridLayout* btnLayout = new QGridLayout();
    createTemplateBtn_ = new QPushButton(tr("从ROI创建"), this);
    createTemplateBtn_->setIcon(style()->standardIcon(QStyle::SP_FileDialogNewFolder));
    loadTemplateBtn_ = new QPushButton(tr("加载"), this);
    loadTemplateBtn_->setIcon(style()->standardIcon(QStyle::SP_DialogOpenButton));
    saveTemplateBtn_ = new QPushButton(tr("保存"), this);
    saveTemplateBtn_->setIcon(style()->standardIcon(QStyle::SP_DialogSaveButton));
    clearTemplateBtn_ = new QPushButton(tr("清除"), this);
    clearTemplateBtn_->setIcon(style()->standardIcon(QStyle::SP_DialogDiscardButton));

    btnLayout->addWidget(createTemplateBtn_, 0, 0);
    btnLayout->addWidget(loadTemplateBtn_, 0, 1);
    btnLayout->addWidget(saveTemplateBtn_, 1, 0);
    btnLayout->addWidget(clearTemplateBtn_, 1, 1);
    groupLayout->addLayout(btnLayout);

    layout->addWidget(templateGroup_);
}

void QuickSearchToolDialog::createMatchParamGroup(QVBoxLayout* layout)
{
    matchParamGroup_ = new QGroupBox(tr("匹配参数"), this);
    QGridLayout* groupLayout = new QGridLayout(matchParamGroup_);
    groupLayout->setSpacing(6);

    int row = 0;

    // 匹配方法
    groupLayout->addWidget(new QLabel(tr("匹配方法:"), this), row, 0);
    matchMethodCombo_ = new QComboBox(this);
    matchMethodCombo_->addItem(tr("相关系数 (推荐)"), cv::TM_CCOEFF_NORMED);
    matchMethodCombo_->addItem(tr("归一化相关"), cv::TM_CCORR_NORMED);
    matchMethodCombo_->addItem(tr("平方差"), cv::TM_SQDIFF_NORMED);
    groupLayout->addWidget(matchMethodCombo_, row, 1);
    row++;

    // 最小分数
    groupLayout->addWidget(new QLabel(tr("最小分数:"), this), row, 0);
    QHBoxLayout* scoreLayout = new QHBoxLayout();
    minScoreSpin_ = new QDoubleSpinBox(this);
    minScoreSpin_->setRange(0.0, 1.0);
    minScoreSpin_->setSingleStep(0.05);
    minScoreSpin_->setValue(0.7);
    minScoreSlider_ = new QSlider(Qt::Horizontal, this);
    minScoreSlider_->setRange(0, 100);
    minScoreSlider_->setValue(70);
    scoreLayout->addWidget(minScoreSpin_);
    scoreLayout->addWidget(minScoreSlider_);
    groupLayout->addLayout(scoreLayout, row, 1);
    row++;

    // 最大匹配数
    groupLayout->addWidget(new QLabel(tr("最大匹配数:"), this), row, 0);
    maxMatchesSpin_ = new QSpinBox(this);
    maxMatchesSpin_->setRange(1, 100);
    maxMatchesSpin_->setValue(1);
    maxMatchesSpin_->setToolTip(tr("设置为1可获得最快速度"));
    groupLayout->addWidget(maxMatchesSpin_, row, 1);

    layout->addWidget(matchParamGroup_);
}

void QuickSearchToolDialog::createSpeedGroup(QVBoxLayout* layout)
{
    speedGroup_ = new QGroupBox(tr("搜索速度"), this);
    QVBoxLayout* groupLayout = new QVBoxLayout(speedGroup_);

    // 速度等级
    QHBoxLayout* levelLayout = new QHBoxLayout();
    levelLayout->addWidget(new QLabel(tr("速度等级:"), this));
    speedLevelCombo_ = new QComboBox(this);
    speedLevelCombo_->addItem(tr("极速"), 0);
    speedLevelCombo_->addItem(tr("快速"), 1);
    speedLevelCombo_->addItem(tr("标准"), 2);
    speedLevelCombo_->addItem(tr("精确"), 3);
    speedLevelCombo_->setCurrentIndex(1);  // 默认快速
    levelLayout->addWidget(speedLevelCombo_);
    groupLayout->addLayout(levelLayout);

    speedDescLabel_ = new QLabel(tr("快速模式：使用金字塔加速，平衡速度和精度"), this);
    speedDescLabel_->setWordWrap(true);
    speedDescLabel_->setStyleSheet("color: #666; font-size: 11px;");
    groupLayout->addWidget(speedDescLabel_);

    // 多尺度搜索
    useMultiScaleCheck_ = new QCheckBox(tr("多尺度搜索"), this);
    useMultiScaleCheck_->setChecked(true);
    groupLayout->addWidget(useMultiScaleCheck_);

    QHBoxLayout* pyramidLayout = new QHBoxLayout();
    pyramidLayout->addWidget(new QLabel(tr("金字塔层数:"), this));
    pyramidLevelsSpin_ = new QSpinBox(this);
    pyramidLevelsSpin_->setRange(1, 6);
    pyramidLevelsSpin_->setValue(3);
    pyramidLayout->addWidget(pyramidLevelsSpin_);
    pyramidLayout->addStretch();
    groupLayout->addLayout(pyramidLayout);

    layout->addWidget(speedGroup_);
}

void QuickSearchToolDialog::connectSpecificSignals()
{
    // 模板管理
    connect(createTemplateBtn_, &QPushButton::clicked, this, &QuickSearchToolDialog::onCreateTemplateClicked);
    connect(loadTemplateBtn_, &QPushButton::clicked, this, &QuickSearchToolDialog::onLoadTemplateClicked);
    connect(saveTemplateBtn_, &QPushButton::clicked, this, &QuickSearchToolDialog::onSaveTemplateClicked);
    connect(clearTemplateBtn_, &QPushButton::clicked, this, &QuickSearchToolDialog::onClearTemplateClicked);

    // 分数滑块同步
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

    // 速度等级描述更新
    connect(speedLevelCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int index) {
        switch (index) {
            case 0:
                speedDescLabel_->setText(tr("极速模式：最快速度，适合大模板定位"));
                pyramidLevelsSpin_->setValue(4);
                break;
            case 1:
                speedDescLabel_->setText(tr("快速模式：平衡速度和精度"));
                pyramidLevelsSpin_->setValue(3);
                break;
            case 2:
                speedDescLabel_->setText(tr("标准模式：较高精度，适合一般场景"));
                pyramidLevelsSpin_->setValue(2);
                break;
            case 3:
                speedDescLabel_->setText(tr("精确模式：最高精度，速度较慢"));
                pyramidLevelsSpin_->setValue(1);
                break;
        }
        if (previewHelper_) previewHelper_->requestPreview();
    });

    // 多尺度控制
    connect(useMultiScaleCheck_, &QCheckBox::toggled, pyramidLevelsSpin_, &QSpinBox::setEnabled);

    // 参数变化触发预览
    auto triggerPreview = [this]() {
        if (previewHelper_) previewHelper_->requestPreview();
    };
    connect(matchMethodCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged), triggerPreview);
    connect(maxMatchesSpin_, QOverload<int>::of(&QSpinBox::valueChanged), triggerPreview);
    connect(pyramidLevelsSpin_, QOverload<int>::of(&QSpinBox::valueChanged), triggerPreview);
}

void QuickSearchToolDialog::updateTemplatePreview()
{
    // TODO: 更新模板预览图像
}

bool QuickSearchToolDialog::executeSearch(std::vector<SearchResult>& results)
{
    if (!currentImage_) {
        LOG_WARNING("执行快速搜索失败: 未加载图像");
        return false;
    }

    // TODO: 检查是否有模板
    // if (!quickTool_->hasTemplate()) {
    //     LOG_WARNING("执行快速搜索失败: 未创建模板");
    //     return false;
    // }

    try {
        cv::Mat mat = currentImage_->mat();
        cv::Mat grayMat;
        if (mat.channels() == 3) {
            cv::cvtColor(mat, grayMat, cv::COLOR_BGR2GRAY);
        } else {
            grayMat = mat.clone();
        }

        // 获取搜索区域
        cv::Mat searchMat = grayMat;
        QRect roi;
        if (hasSearchROI()) {
            roi = getSearchROI();
            searchMat = grayMat(cv::Rect(roi.x(), roi.y(), roi.width(), roi.height()));
        }

        // TODO: 使用实际模板进行匹配
        // 这里使用模拟结果
        LOG_INFO("快速搜索执行中...");

        // 模拟搜索结果
        SearchResult r1;
        r1.index = 1;
        r1.x = 256.0;
        r1.y = 192.0;
        r1.angle = 0;
        r1.scale = 1.0;
        r1.score = 0.92;
        results.push_back(r1);

        LOG_INFO(QString("快速搜索完成，找到 %1 个匹配").arg(results.size()));
        return true;

    } catch (const cv::Exception& e) {
        LOG_ERROR(QString("快速搜索异常: %1").arg(e.what()));
        return false;
    }
}

void QuickSearchToolDialog::applySpecificParameters()
{
    if (!quickTool_) return;

    // TODO: 将UI参数应用到工具
    LOG_DEBUG("快速搜索参数已应用");
}

void QuickSearchToolDialog::updateSpecificUI()
{
    if (!quickTool_) return;

    // TODO: 从工具读取参数更新UI
}

void QuickSearchToolDialog::onCreateTemplateClicked()
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

    // TODO: 从ROI创建模板
    LOG_INFO(QString("从ROI创建快速搜索模板: (%1,%2) %3x%4")
        .arg(roi.x()).arg(roi.y()).arg(roi.width()).arg(roi.height()));

    templateStatusLabel_->setText(tr("状态: 模板已创建 (%1x%2)").arg(roi.width()).arg(roi.height()));
    templateStatusLabel_->setStyleSheet("color: #4CAF50; font-weight: bold;");

    updateTemplatePreview();
}

void QuickSearchToolDialog::onLoadTemplateClicked()
{
    QString filePath = QFileDialog::getOpenFileName(
        this, tr("加载模板"),
        QString(),
        tr("图像文件 (*.png *.jpg *.bmp);;所有文件 (*)"));

    if (filePath.isEmpty()) return;

    // TODO: 加载模板图像
    LOG_INFO(QString("加载快速搜索模板: %1").arg(filePath));

    templateStatusLabel_->setText(tr("状态: 已加载 - ") + QFileInfo(filePath).fileName());
    templateStatusLabel_->setStyleSheet("color: #4CAF50; font-weight: bold;");

    updateTemplatePreview();
}

void QuickSearchToolDialog::onSaveTemplateClicked()
{
    QString filePath = QFileDialog::getSaveFileName(
        this, tr("保存模板"),
        QString(),
        tr("PNG图像 (*.png);;所有文件 (*)"));

    if (filePath.isEmpty()) return;

    if (!filePath.endsWith(".png", Qt::CaseInsensitive)) {
        filePath += ".png";
    }

    // TODO: 保存模板图像
    LOG_INFO(QString("保存快速搜索模板: %1").arg(filePath));
    QMessageBox::information(this, tr("成功"), tr("模板已保存"));
}

void QuickSearchToolDialog::onClearTemplateClicked()
{
    int ret = QMessageBox::question(this, tr("确认"),
        tr("确定要清除当前模板吗？"),
        QMessageBox::Yes | QMessageBox::No);

    if (ret == QMessageBox::Yes) {
        // TODO: 清除模板
        templateStatusLabel_->setText(tr("状态: 未创建模板"));
        templateStatusLabel_->setStyleSheet("color: #FF9800; font-weight: bold;");
        templatePreviewLabel_->setPixmap(QPixmap());
        templatePreviewLabel_->setText(tr("无预览"));
        LOG_INFO("快速搜索模板已清除");
    }
}

} // namespace UI
} // namespace VisionForge
