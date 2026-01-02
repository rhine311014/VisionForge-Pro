/**
 * @file SearchToolDialog.cpp
 * @brief 搜索工具对话框基类实现
 * @author VisionForge Team
 * @date 2025-12-30
 */

#include "ui/SearchToolDialog.h"
#include "ui/HalconImageViewer.h"
#include "ui/ROI.h"
#include "algorithm/VisionTool.h"
#include "base/Logger.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QHeaderView>
#include <QElapsedTimer>
#include <QMessageBox>

namespace VisionForge {
namespace UI {

SearchToolDialog::SearchToolDialog(Algorithm::VisionTool* tool, QWidget* parent)
    : IToolDialog(parent)
    , tool_(tool)
{
    setupUI();
    connectSignals();
}

SearchToolDialog::~SearchToolDialog()
{
}

void SearchToolDialog::setImage(Base::ImageData::Ptr image)
{
    currentImage_ = image;
    if (imageViewer_ && image) {
        imageViewer_->setImage(image);
        // 更新图像信息
        if (imageInfoLabel_) {
            imageInfoLabel_->setText(QString("图像: %1 x %2")
                .arg(image->width())
                .arg(image->height()));
        }
    }
}

void SearchToolDialog::setupUI()
{
    setWindowTitle(toolName() + tr(" 设置"));
    setMinimumSize(1000, 700);
    resize(1100, 750);
    setWindowFlags(windowFlags() | Qt::WindowMaximizeButtonHint);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(8, 8, 8, 8);
    mainLayout->setSpacing(8);

    // 主分割器
    mainSplitter_ = new QSplitter(Qt::Horizontal, this);

    // 左侧面板 - 图像显示
    leftPanel_ = new QWidget(mainSplitter_);
    createLeftPanel(leftPanel_);

    // 右侧面板 - 参数设置
    rightPanel_ = new QWidget(mainSplitter_);
    createRightPanel(rightPanel_);

    mainSplitter_->addWidget(leftPanel_);
    mainSplitter_->addWidget(rightPanel_);
    mainSplitter_->setStretchFactor(0, 3);
    mainSplitter_->setStretchFactor(1, 2);

    mainLayout->addWidget(mainSplitter_, 1);

    // 创建预览辅助器
    previewHelper_ = new PreviewHelper(this, 300);

    // 底部按钮面板
    QHBoxLayout* bottomLayout = new QHBoxLayout();

    autoPreviewCheck_ = new QCheckBox(tr("实时预览"), this);
    autoPreviewCheck_->setChecked(false);
    autoPreviewCheck_->setToolTip(tr("启用后参数修改会自动更新预览"));
    bottomLayout->addWidget(autoPreviewCheck_);

    bottomLayout->addStretch();

    previewBtn_ = new QPushButton(tr("预览"), this);
    previewBtn_->setMinimumWidth(80);
    okBtn_ = new QPushButton(tr("确定"), this);
    okBtn_->setMinimumWidth(80);
    cancelBtn_ = new QPushButton(tr("取消"), this);
    cancelBtn_->setMinimumWidth(80);
    applyBtn_ = new QPushButton(tr("应用"), this);
    applyBtn_->setMinimumWidth(80);

    bottomLayout->addWidget(previewBtn_);
    bottomLayout->addWidget(okBtn_);
    bottomLayout->addWidget(cancelBtn_);
    bottomLayout->addWidget(applyBtn_);

    mainLayout->addLayout(bottomLayout);
}

void SearchToolDialog::createLeftPanel(QWidget* parent)
{
    QVBoxLayout* layout = new QVBoxLayout(parent);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(4);

    // 标题和图像信息
    QHBoxLayout* titleLayout = new QHBoxLayout();
    QLabel* titleLabel = new QLabel(tr("搜索图像"), parent);
    titleLabel->setStyleSheet("font-weight: bold; font-size: 12px;");
    titleLayout->addWidget(titleLabel);

    imageInfoLabel_ = new QLabel(tr("图像: 未加载"), parent);
    imageInfoLabel_->setStyleSheet("color: #888; font-size: 11px;");
    titleLayout->addWidget(imageInfoLabel_);
    titleLayout->addStretch();

    layout->addLayout(titleLayout);

    // 图像显示器
    imageViewer_ = new HalconImageViewer(parent);
    imageViewer_->setMinimumSize(500, 400);
    imageViewer_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    layout->addWidget(imageViewer_, 1);
}

void SearchToolDialog::createRightPanel(QWidget* parent)
{
    QVBoxLayout* layout = new QVBoxLayout(parent);
    layout->setContentsMargins(4, 0, 0, 0);
    layout->setSpacing(8);

    // 选项卡控件
    paramTabWidget_ = new QTabWidget(parent);

    // Tab 1: 搜索参数（由子类实现）
    parameterPanel_ = createParameterPanel(paramTabWidget_);
    if (parameterPanel_) {
        paramTabWidget_->addTab(parameterPanel_, tr("搜索参数"));
    }

    // Tab 2: ROI设置
    QWidget* roiTab = new QWidget(paramTabWidget_);
    QVBoxLayout* roiLayout = new QVBoxLayout(roiTab);
    roiLayout->setContentsMargins(8, 8, 8, 8);
    createROIGroup(roiLayout);
    roiLayout->addStretch();
    paramTabWidget_->addTab(roiTab, tr("搜索区域"));

    // Tab 3: 搜索结果
    QWidget* resultTab = new QWidget(paramTabWidget_);
    QVBoxLayout* resultLayout = new QVBoxLayout(resultTab);
    resultLayout->setContentsMargins(8, 8, 8, 8);
    createResultGroup(resultLayout);
    paramTabWidget_->addTab(resultTab, tr("搜索结果"));

    layout->addWidget(paramTabWidget_, 1);
}

void SearchToolDialog::createROIGroup(QVBoxLayout* layout)
{
    roiGroup_ = new QGroupBox(tr("搜索区域设置"), this);
    QVBoxLayout* groupLayout = new QVBoxLayout(roiGroup_);

    // ROI类型选择
    QHBoxLayout* typeLayout = new QHBoxLayout();
    typeLayout->addWidget(new QLabel(tr("区域类型:"), this));
    roiTypeCombo_ = new QComboBox(this);
    roiTypeCombo_->addItem(tr("矩形"), 0);
    roiTypeCombo_->addItem(tr("椭圆"), 1);
    roiTypeCombo_->addItem(tr("多边形"), 2);
    typeLayout->addWidget(roiTypeCombo_);
    typeLayout->addStretch();
    groupLayout->addLayout(typeLayout);

    // ROI操作按钮
    QHBoxLayout* btnLayout = new QHBoxLayout();
    drawROIBtn_ = new QPushButton(tr("绘制搜索区域"), this);
    drawROIBtn_->setIcon(style()->standardIcon(QStyle::SP_DialogOpenButton));
    clearROIBtn_ = new QPushButton(tr("清除区域"), this);
    clearROIBtn_->setIcon(style()->standardIcon(QStyle::SP_DialogDiscardButton));
    btnLayout->addWidget(drawROIBtn_);
    btnLayout->addWidget(clearROIBtn_);
    groupLayout->addLayout(btnLayout);

    // ROI状态
    roiStatusLabel_ = new QLabel(tr("状态: 未设置搜索区域（将搜索全图）"), this);
    roiStatusLabel_->setStyleSheet("color: #888; font-style: italic;");
    roiStatusLabel_->setWordWrap(true);
    groupLayout->addWidget(roiStatusLabel_);

    // 提示信息
    QLabel* tipLabel = new QLabel(tr("提示: 设置搜索区域可以加快搜索速度，提高搜索精度。"), this);
    tipLabel->setStyleSheet("color: #666; font-size: 11px;");
    tipLabel->setWordWrap(true);
    groupLayout->addWidget(tipLabel);

    layout->addWidget(roiGroup_);
}

void SearchToolDialog::createResultGroup(QVBoxLayout* layout)
{
    resultGroup_ = new QGroupBox(tr("搜索结果"), this);
    QVBoxLayout* groupLayout = new QVBoxLayout(resultGroup_);

    // 结果统计
    QHBoxLayout* statsLayout = new QHBoxLayout();
    resultCountLabel_ = new QLabel(tr("找到: 0 个目标"), this);
    resultCountLabel_->setStyleSheet("font-weight: bold;");
    statsLayout->addWidget(resultCountLabel_);

    searchTimeLabel_ = new QLabel(tr("耗时: -- ms"), this);
    searchTimeLabel_->setStyleSheet("color: #666;");
    statsLayout->addWidget(searchTimeLabel_);
    statsLayout->addStretch();
    groupLayout->addLayout(statsLayout);

    // 结果表格
    resultTable_ = new QTableWidget(this);
    resultTable_->setColumnCount(6);
    resultTable_->setHorizontalHeaderLabels({
        tr("序号"), tr("X"), tr("Y"), tr("角度"), tr("缩放"), tr("得分")
    });
    resultTable_->horizontalHeader()->setStretchLastSection(true);
    resultTable_->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    resultTable_->setSelectionBehavior(QAbstractItemView::SelectRows);
    resultTable_->setSelectionMode(QAbstractItemView::SingleSelection);
    resultTable_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    resultTable_->setAlternatingRowColors(true);
    groupLayout->addWidget(resultTable_, 1);

    layout->addWidget(resultGroup_, 1);
}

void SearchToolDialog::connectSignals()
{
    // ROI操作
    connect(drawROIBtn_, &QPushButton::clicked, this, &SearchToolDialog::onDrawROIClicked);
    connect(clearROIBtn_, &QPushButton::clicked, this, &SearchToolDialog::onClearROIClicked);

    // ROI创建完成信号
    if (imageViewer_) {
        connect(imageViewer_, &HalconImageViewer::roiCreated, this, [this](ROIShapePtr roi) {
            Q_UNUSED(roi);
            std::vector<ROIShapePtr> rois = imageViewer_->getROIs();
            roiStatusLabel_->setText(QString(tr("状态: 已设置 %1 个搜索区域")).arg(rois.size()));
            roiStatusLabel_->setStyleSheet("color: #4CAF50;");
            imageViewer_->setInteractionMode(HalconImageViewer::SelectMode);
        });
    }

    // 结果表格选择
    connect(resultTable_, &QTableWidget::cellClicked, this, [this](int row, int) {
        onResultTableRowSelected(row);
    });

    // 实时预览
    connect(autoPreviewCheck_, &QCheckBox::toggled,
            previewHelper_, &PreviewHelper::setAutoPreviewEnabled);
    connect(previewHelper_, &PreviewHelper::previewTriggered,
            this, &SearchToolDialog::onAutoPreview);

    // 按钮
    connect(previewBtn_, &QPushButton::clicked, this, &SearchToolDialog::onPreviewClicked);
    connect(okBtn_, &QPushButton::clicked, this, &SearchToolDialog::onOkClicked);
    connect(cancelBtn_, &QPushButton::clicked, this, &SearchToolDialog::onCancelClicked);
    connect(applyBtn_, &QPushButton::clicked, this, &SearchToolDialog::onApplyClicked);
}

void SearchToolDialog::updateUI()
{
    updateSpecificUI();
}

void SearchToolDialog::updateResultTable(const std::vector<SearchResult>& results)
{
    resultTable_->setRowCount(static_cast<int>(results.size()));

    for (size_t i = 0; i < results.size(); ++i) {
        const auto& result = results[i];
        int row = static_cast<int>(i);

        resultTable_->setItem(row, 0, new QTableWidgetItem(QString::number(result.index)));
        resultTable_->setItem(row, 1, new QTableWidgetItem(QString::number(result.x, 'f', 2)));
        resultTable_->setItem(row, 2, new QTableWidgetItem(QString::number(result.y, 'f', 2)));
        resultTable_->setItem(row, 3, new QTableWidgetItem(QString::number(result.angle, 'f', 2) + "°"));
        resultTable_->setItem(row, 4, new QTableWidgetItem(QString::number(result.scale, 'f', 3)));
        resultTable_->setItem(row, 5, new QTableWidgetItem(QString::number(result.score, 'f', 4)));

        // 根据得分设置颜色
        QColor bgColor;
        if (result.score >= 0.9) {
            bgColor = QColor(200, 255, 200);  // 绿色 - 高分
        } else if (result.score >= 0.7) {
            bgColor = QColor(255, 255, 200);  // 黄色 - 中等
        } else {
            bgColor = QColor(255, 220, 220);  // 红色 - 低分
        }

        for (int col = 0; col < resultTable_->columnCount(); ++col) {
            if (auto item = resultTable_->item(row, col)) {
                item->setBackground(bgColor);
            }
        }
    }

    resultCountLabel_->setText(QString(tr("找到: %1 个目标")).arg(results.size()));
}

void SearchToolDialog::drawSearchResults(const std::vector<SearchResult>& results)
{
    if (!imageViewer_ || results.empty()) {
        return;
    }

    // 绘制搜索结果到图像上
    // 这里调用HalconImageViewer的绘制方法
    // 具体实现依赖于HalconImageViewer的接口
    for (const auto& result : results) {
        // 绘制十字标记
        // imageViewer_->drawCross(result.x, result.y, 20, Qt::green);
        // 绘制角度指示
        // imageViewer_->drawAngle(result.x, result.y, result.angle, 30, Qt::blue);
        Q_UNUSED(result);
    }

    imageViewer_->update();
}

QRect SearchToolDialog::getSearchROI() const
{
    if (!imageViewer_) {
        return QRect();
    }

    std::vector<ROIShapePtr> rois = imageViewer_->getROIs();
    if (rois.empty()) {
        return QRect();
    }

    return rois.front()->boundingRect();
}

bool SearchToolDialog::hasSearchROI() const
{
    if (!imageViewer_) {
        return false;
    }
    return !imageViewer_->getROIs().empty();
}

void SearchToolDialog::onDrawROIClicked()
{
    if (!currentImage_) {
        QMessageBox::warning(this, tr("警告"), tr("请先加载图像"));
        return;
    }

    if (imageViewer_) {
        int roiType = roiTypeCombo_->currentData().toInt();
        switch (roiType) {
            case 0:
                imageViewer_->setInteractionMode(HalconImageViewer::DrawRectangle);
                break;
            case 1:
                imageViewer_->setInteractionMode(HalconImageViewer::DrawEllipse);
                break;
            case 2:
                imageViewer_->setInteractionMode(HalconImageViewer::DrawPolygon);
                break;
            default:
                imageViewer_->setInteractionMode(HalconImageViewer::DrawRectangle);
                break;
        }
        roiStatusLabel_->setText(tr("状态: 正在绘制..."));
        roiStatusLabel_->setStyleSheet("color: #2196F3;");
    }
}

void SearchToolDialog::onClearROIClicked()
{
    if (imageViewer_) {
        imageViewer_->clearROIs();
        roiStatusLabel_->setText(tr("状态: 未设置搜索区域（将搜索全图）"));
        roiStatusLabel_->setStyleSheet("color: #888; font-style: italic;");
    }
}

void SearchToolDialog::onPreviewClicked()
{
    if (!currentImage_) {
        QMessageBox::warning(this, tr("警告"), tr("请先加载图像"));
        return;
    }

    applySpecificParameters();

    QElapsedTimer timer;
    timer.start();

    std::vector<SearchResult> results;
    if (executeSearch(results)) {
        qint64 elapsed = timer.elapsed();
        searchTimeLabel_->setText(QString(tr("耗时: %1 ms")).arg(elapsed));

        searchResults_ = results;
        updateResultTable(results);
        drawSearchResults(results);

        emit searchCompleted(results);
        LOG_INFO(QString("搜索完成，找到 %1 个结果，耗时 %2 ms")
            .arg(results.size()).arg(elapsed));
    } else {
        searchTimeLabel_->setText(tr("耗时: -- ms"));
        QMessageBox::warning(this, tr("搜索失败"), tr("搜索过程中发生错误"));
    }
}

void SearchToolDialog::onAutoPreview()
{
    if (!currentImage_) {
        return;
    }

    applySpecificParameters();

    std::vector<SearchResult> results;
    if (executeSearch(results)) {
        searchResults_ = results;
        updateResultTable(results);
        drawSearchResults(results);
    }
}

void SearchToolDialog::onOkClicked()
{
    applySpecificParameters();
    accept();
}

void SearchToolDialog::onCancelClicked()
{
    reject();
}

void SearchToolDialog::onApplyClicked()
{
    applySpecificParameters();
    emit parameterChanged();
}

void SearchToolDialog::onResultTableRowSelected(int row)
{
    if (row < 0 || row >= static_cast<int>(searchResults_.size())) {
        return;
    }

    const auto& result = searchResults_[row];

    // 在图像上高亮显示选中的结果
    if (imageViewer_) {
        // TODO: 添加居中显示功能
        // imageViewer_->centerOn(result.x, result.y);
    }

    LOG_DEBUG(QString("选中搜索结果 #%1: (%2, %3), 角度=%4°, 得分=%5")
        .arg(result.index)
        .arg(result.x, 0, 'f', 2)
        .arg(result.y, 0, 'f', 2)
        .arg(result.angle, 0, 'f', 2)
        .arg(result.score, 0, 'f', 4));
}

} // namespace UI
} // namespace VisionForge
