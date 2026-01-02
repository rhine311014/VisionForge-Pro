/**
 * @file SearchToolDialog.cpp
 * @brief 搜索工具对话框基类实现
 * @details 实现了搜索工具对话框的通用UI框架和基础功能，
 *          包括界面布局、ROI管理、结果显示等。
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

/**
 * @brief 构造函数实现
 * @details 初始化成员变量，设置UI并连接信号槽
 * @param tool 关联的视觉工具指针
 * @param parent 父窗口指针
 */
SearchToolDialog::SearchToolDialog(Algorithm::VisionTool* tool, QWidget* parent)
    : IToolDialog(parent)
    , tool_(tool)
{
    // 初始化UI界面
    setupUI();
    // 连接信号槽
    connectSignals();
}

/**
 * @brief 析构函数实现
 * @details 清理资源（Qt父子关系会自动处理子控件的释放）
 */
SearchToolDialog::~SearchToolDialog()
{
}

/**
 * @brief 设置待搜索的图像
 * @details 将图像显示在图像查看器中，并更新图像信息标签
 * @param image 图像数据智能指针
 */
void SearchToolDialog::setImage(Base::ImageData::Ptr image)
{
    // 保存图像引用
    currentImage_ = image;

    // 如果图像查看器和图像都有效，则显示图像
    if (imageViewer_ && image) {
        imageViewer_->setImage(image);
        // 更新图像尺寸信息显示
        if (imageInfoLabel_) {
            imageInfoLabel_->setText(QString("图像: %1 x %2")
                .arg(image->width())
                .arg(image->height()));
        }
    }
}

/**
 * @brief 初始化整体UI布局
 * @details 创建主窗口布局，包括左右分割面板和底部按钮
 */
void SearchToolDialog::setupUI()
{
    // 设置窗口标题，包含工具名称
    setWindowTitle(toolName() + tr(" 设置"));
    // 设置窗口最小尺寸
    setMinimumSize(1000, 700);
    // 设置窗口默认尺寸
    resize(1100, 750);
    // 添加最大化按钮
    setWindowFlags(windowFlags() | Qt::WindowMaximizeButtonHint);

    // 创建主布局（垂直布局）
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(8, 8, 8, 8);  // 设置边距
    mainLayout->setSpacing(8);                    // 设置控件间距

    // 创建主分割器（水平分割，左侧图像，右侧参数）
    mainSplitter_ = new QSplitter(Qt::Horizontal, this);

    // 创建左侧面板 - 图像显示区域
    leftPanel_ = new QWidget(mainSplitter_);
    createLeftPanel(leftPanel_);

    // 创建右侧面板 - 参数设置区域
    rightPanel_ = new QWidget(mainSplitter_);
    createRightPanel(rightPanel_);

    // 将左右面板添加到分割器
    mainSplitter_->addWidget(leftPanel_);
    mainSplitter_->addWidget(rightPanel_);
    // 设置分割比例：左侧3，右侧2
    mainSplitter_->setStretchFactor(0, 3);
    mainSplitter_->setStretchFactor(1, 2);

    // 将分割器添加到主布局，并设置拉伸因子为1（占据剩余空间）
    mainLayout->addWidget(mainSplitter_, 1);

    // 创建预览辅助器（300ms防抖延迟）
    previewHelper_ = new PreviewHelper(this, 300);

    // ==================== 创建底部按钮面板 ====================
    QHBoxLayout* bottomLayout = new QHBoxLayout();

    // 实时预览复选框
    autoPreviewCheck_ = new QCheckBox(tr("实时预览"), this);
    autoPreviewCheck_->setChecked(false);  // 默认关闭实时预览
    autoPreviewCheck_->setToolTip(tr("启用后参数修改会自动更新预览"));
    bottomLayout->addWidget(autoPreviewCheck_);

    // 添加弹性空间，将按钮推到右侧
    bottomLayout->addStretch();

    // 创建操作按钮
    previewBtn_ = new QPushButton(tr("预览"), this);
    previewBtn_->setMinimumWidth(80);
    okBtn_ = new QPushButton(tr("确定"), this);
    okBtn_->setMinimumWidth(80);
    cancelBtn_ = new QPushButton(tr("取消"), this);
    cancelBtn_->setMinimumWidth(80);
    applyBtn_ = new QPushButton(tr("应用"), this);
    applyBtn_->setMinimumWidth(80);

    // 将按钮添加到布局
    bottomLayout->addWidget(previewBtn_);
    bottomLayout->addWidget(okBtn_);
    bottomLayout->addWidget(cancelBtn_);
    bottomLayout->addWidget(applyBtn_);

    // 将底部布局添加到主布局
    mainLayout->addLayout(bottomLayout);
}

/**
 * @brief 创建左侧面板（图像显示区域）
 * @details 包含标题栏和Halcon图像查看器
 * @param parent 父控件
 */
void SearchToolDialog::createLeftPanel(QWidget* parent)
{
    QVBoxLayout* layout = new QVBoxLayout(parent);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(4);

    // ==================== 标题栏 ====================
    QHBoxLayout* titleLayout = new QHBoxLayout();

    // 标题标签
    QLabel* titleLabel = new QLabel(tr("搜索图像"), parent);
    titleLabel->setStyleSheet("font-weight: bold; font-size: 12px;");
    titleLayout->addWidget(titleLabel);

    // 图像信息标签（显示图像尺寸）
    imageInfoLabel_ = new QLabel(tr("图像: 未加载"), parent);
    imageInfoLabel_->setStyleSheet("color: #888; font-size: 11px;");
    titleLayout->addWidget(imageInfoLabel_);
    titleLayout->addStretch();  // 将信息推到左侧

    layout->addLayout(titleLayout);

    // ==================== 图像显示器 ====================
    imageViewer_ = new HalconImageViewer(parent);
    imageViewer_->setMinimumSize(500, 400);  // 设置最小尺寸
    // 设置尺寸策略：可扩展
    imageViewer_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    // 添加到布局，拉伸因子为1（占据剩余空间）
    layout->addWidget(imageViewer_, 1);
}

/**
 * @brief 创建右侧面板（参数设置区域）
 * @details 创建选项卡控件，包含搜索参数、ROI设置、搜索结果三个标签页
 * @param parent 父控件
 */
void SearchToolDialog::createRightPanel(QWidget* parent)
{
    QVBoxLayout* layout = new QVBoxLayout(parent);
    layout->setContentsMargins(4, 0, 0, 0);
    layout->setSpacing(8);

    // 创建选项卡控件
    paramTabWidget_ = new QTabWidget(parent);

    // ==================== Tab 1: 搜索参数（由子类实现） ====================
    parameterPanel_ = createParameterPanel(paramTabWidget_);
    if (parameterPanel_) {
        paramTabWidget_->addTab(parameterPanel_, tr("搜索参数"));
    }

    // ==================== Tab 2: ROI设置 ====================
    QWidget* roiTab = new QWidget(paramTabWidget_);
    QVBoxLayout* roiLayout = new QVBoxLayout(roiTab);
    roiLayout->setContentsMargins(8, 8, 8, 8);
    createROIGroup(roiLayout);
    roiLayout->addStretch();  // 添加弹性空间
    paramTabWidget_->addTab(roiTab, tr("搜索区域"));

    // ==================== Tab 3: 搜索结果 ====================
    QWidget* resultTab = new QWidget(paramTabWidget_);
    QVBoxLayout* resultLayout = new QVBoxLayout(resultTab);
    resultLayout->setContentsMargins(8, 8, 8, 8);
    createResultGroup(resultLayout);
    paramTabWidget_->addTab(resultTab, tr("搜索结果"));

    // 将选项卡控件添加到布局
    layout->addWidget(paramTabWidget_, 1);
}

/**
 * @brief 创建ROI设置组
 * @details 包含ROI类型选择、绘制/清除按钮和状态显示
 * @param layout 父布局
 */
void SearchToolDialog::createROIGroup(QVBoxLayout* layout)
{
    roiGroup_ = new QGroupBox(tr("搜索区域设置"), this);
    QVBoxLayout* groupLayout = new QVBoxLayout(roiGroup_);

    // ==================== ROI类型选择 ====================
    QHBoxLayout* typeLayout = new QHBoxLayout();
    typeLayout->addWidget(new QLabel(tr("区域类型:"), this));

    // 创建ROI类型下拉框
    roiTypeCombo_ = new QComboBox(this);
    roiTypeCombo_->addItem(tr("矩形"), 0);    // 矩形ROI
    roiTypeCombo_->addItem(tr("椭圆"), 1);    // 椭圆ROI
    roiTypeCombo_->addItem(tr("多边形"), 2);  // 多边形ROI
    typeLayout->addWidget(roiTypeCombo_);
    typeLayout->addStretch();
    groupLayout->addLayout(typeLayout);

    // ==================== ROI操作按钮 ====================
    QHBoxLayout* btnLayout = new QHBoxLayout();

    // 绘制ROI按钮
    drawROIBtn_ = new QPushButton(tr("绘制搜索区域"), this);
    drawROIBtn_->setIcon(style()->standardIcon(QStyle::SP_DialogOpenButton));

    // 清除ROI按钮
    clearROIBtn_ = new QPushButton(tr("清除区域"), this);
    clearROIBtn_->setIcon(style()->standardIcon(QStyle::SP_DialogDiscardButton));

    btnLayout->addWidget(drawROIBtn_);
    btnLayout->addWidget(clearROIBtn_);
    groupLayout->addLayout(btnLayout);

    // ==================== ROI状态显示 ====================
    roiStatusLabel_ = new QLabel(tr("状态: 未设置搜索区域（将搜索全图）"), this);
    roiStatusLabel_->setStyleSheet("color: #888; font-style: italic;");
    roiStatusLabel_->setWordWrap(true);  // 允许文字换行
    groupLayout->addWidget(roiStatusLabel_);

    // 提示信息标签
    QLabel* tipLabel = new QLabel(tr("提示: 设置搜索区域可以加快搜索速度，提高搜索精度。"), this);
    tipLabel->setStyleSheet("color: #666; font-size: 11px;");
    tipLabel->setWordWrap(true);
    groupLayout->addWidget(tipLabel);

    layout->addWidget(roiGroup_);
}

/**
 * @brief 创建搜索结果显示组
 * @details 包含结果统计信息和详细结果表格
 * @param layout 父布局
 */
void SearchToolDialog::createResultGroup(QVBoxLayout* layout)
{
    resultGroup_ = new QGroupBox(tr("搜索结果"), this);
    QVBoxLayout* groupLayout = new QVBoxLayout(resultGroup_);

    // ==================== 结果统计信息 ====================
    QHBoxLayout* statsLayout = new QHBoxLayout();

    // 结果数量标签
    resultCountLabel_ = new QLabel(tr("找到: 0 个目标"), this);
    resultCountLabel_->setStyleSheet("font-weight: bold;");
    statsLayout->addWidget(resultCountLabel_);

    // 搜索耗时标签
    searchTimeLabel_ = new QLabel(tr("耗时: -- ms"), this);
    searchTimeLabel_->setStyleSheet("color: #666;");
    statsLayout->addWidget(searchTimeLabel_);
    statsLayout->addStretch();
    groupLayout->addLayout(statsLayout);

    // ==================== 结果详细表格 ====================
    resultTable_ = new QTableWidget(this);
    resultTable_->setColumnCount(6);  // 6列
    // 设置表头
    resultTable_->setHorizontalHeaderLabels({
        tr("序号"), tr("X"), tr("Y"), tr("角度"), tr("缩放"), tr("得分")
    });
    // 最后一列自动拉伸
    resultTable_->horizontalHeader()->setStretchLastSection(true);
    // 列宽自适应内容
    resultTable_->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    // 整行选择模式
    resultTable_->setSelectionBehavior(QAbstractItemView::SelectRows);
    // 单选模式
    resultTable_->setSelectionMode(QAbstractItemView::SingleSelection);
    // 禁止编辑
    resultTable_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    // 交替行颜色
    resultTable_->setAlternatingRowColors(true);
    groupLayout->addWidget(resultTable_, 1);

    layout->addWidget(resultGroup_, 1);
}

/**
 * @brief 连接所有信号槽
 * @details 建立UI控件与槽函数之间的连接
 */
void SearchToolDialog::connectSignals()
{
    // ==================== ROI操作信号 ====================
    connect(drawROIBtn_, &QPushButton::clicked, this, &SearchToolDialog::onDrawROIClicked);
    connect(clearROIBtn_, &QPushButton::clicked, this, &SearchToolDialog::onClearROIClicked);

    // ROI创建完成信号处理
    if (imageViewer_) {
        connect(imageViewer_, &HalconImageViewer::roiCreated, this, [this](ROIShapePtr roi) {
            Q_UNUSED(roi);
            // 获取当前ROI列表
            std::vector<ROIShapePtr> rois = imageViewer_->getROIs();
            // 更新ROI状态显示
            roiStatusLabel_->setText(QString(tr("状态: 已设置 %1 个搜索区域")).arg(rois.size()));
            roiStatusLabel_->setStyleSheet("color: #4CAF50;");  // 绿色表示已设置
            // 切换回选择模式
            imageViewer_->setInteractionMode(HalconImageViewer::SelectMode);
        });
    }

    // ==================== 结果表格选择信号 ====================
    connect(resultTable_, &QTableWidget::cellClicked, this, [this](int row, int) {
        onResultTableRowSelected(row);
    });

    // ==================== 实时预览信号 ====================
    // 复选框状态变化时启用/禁用自动预览
    connect(autoPreviewCheck_, &QCheckBox::toggled,
            previewHelper_, &PreviewHelper::setAutoPreviewEnabled);
    // 预览辅助器触发预览
    connect(previewHelper_, &PreviewHelper::previewTriggered,
            this, &SearchToolDialog::onAutoPreview);

    // ==================== 按钮信号 ====================
    connect(previewBtn_, &QPushButton::clicked, this, &SearchToolDialog::onPreviewClicked);
    connect(okBtn_, &QPushButton::clicked, this, &SearchToolDialog::onOkClicked);
    connect(cancelBtn_, &QPushButton::clicked, this, &SearchToolDialog::onCancelClicked);
    connect(applyBtn_, &QPushButton::clicked, this, &SearchToolDialog::onApplyClicked);
}

/**
 * @brief 更新UI显示
 * @details 调用子类特定的UI更新方法
 */
void SearchToolDialog::updateUI()
{
    updateSpecificUI();
}

/**
 * @brief 更新搜索结果表格
 * @details 将搜索结果填充到表格中，并根据得分设置行颜色
 * @param results 搜索结果列表
 */
void SearchToolDialog::updateResultTable(const std::vector<SearchResult>& results)
{
    // 设置表格行数
    resultTable_->setRowCount(static_cast<int>(results.size()));

    // 遍历所有结果，填充表格
    for (size_t i = 0; i < results.size(); ++i) {
        const auto& result = results[i];
        int row = static_cast<int>(i);

        // 填充各列数据
        resultTable_->setItem(row, 0, new QTableWidgetItem(QString::number(result.index)));
        resultTable_->setItem(row, 1, new QTableWidgetItem(QString::number(result.x, 'f', 2)));
        resultTable_->setItem(row, 2, new QTableWidgetItem(QString::number(result.y, 'f', 2)));
        resultTable_->setItem(row, 3, new QTableWidgetItem(QString::number(result.angle, 'f', 2) + "°"));
        resultTable_->setItem(row, 4, new QTableWidgetItem(QString::number(result.scale, 'f', 3)));
        resultTable_->setItem(row, 5, new QTableWidgetItem(QString::number(result.score, 'f', 4)));

        // 根据匹配得分设置行背景颜色
        QColor bgColor;
        if (result.score >= 0.9) {
            bgColor = QColor(200, 255, 200);  // 绿色 - 高分（>=0.9）
        } else if (result.score >= 0.7) {
            bgColor = QColor(255, 255, 200);  // 黄色 - 中等（0.7-0.9）
        } else {
            bgColor = QColor(255, 220, 220);  // 红色 - 低分（<0.7）
        }

        // 为该行所有单元格设置背景颜色
        for (int col = 0; col < resultTable_->columnCount(); ++col) {
            if (auto item = resultTable_->item(row, col)) {
                item->setBackground(bgColor);
            }
        }
    }

    // 更新结果数量显示
    resultCountLabel_->setText(QString(tr("找到: %1 个目标")).arg(results.size()));
}

/**
 * @brief 在图像上绘制搜索结果
 * @details 将匹配结果以图形方式显示在图像上
 * @param results 搜索结果列表
 */
void SearchToolDialog::drawSearchResults(const std::vector<SearchResult>& results)
{
    // 检查图像查看器是否有效且有结果
    if (!imageViewer_ || results.empty()) {
        return;
    }

    // 在图像上绘制搜索结果标记
    // 遍历每个结果，绘制对应的标记
    for (const auto& result : results) {
        // TODO: 调用HalconImageViewer的绘制方法
        // 绘制十字标记
        // imageViewer_->drawCross(result.x, result.y, 20, Qt::green);
        // 绘制角度指示线
        // imageViewer_->drawAngle(result.x, result.y, result.angle, 30, Qt::blue);
        Q_UNUSED(result);
    }

    // 刷新图像显示
    imageViewer_->update();
}

/**
 * @brief 获取搜索ROI区域
 * @details 获取用户绘制的第一个ROI的边界矩形
 * @return ROI矩形区域，如果未设置则返回空矩形
 */
QRect SearchToolDialog::getSearchROI() const
{
    if (!imageViewer_) {
        return QRect();
    }

    // 获取所有ROI
    std::vector<ROIShapePtr> rois = imageViewer_->getROIs();
    if (rois.empty()) {
        return QRect();
    }

    // 返回第一个ROI的边界矩形
    return rois.front()->boundingRect();
}

/**
 * @brief 检查是否已设置搜索ROI
 * @return 是否已设置ROI
 */
bool SearchToolDialog::hasSearchROI() const
{
    if (!imageViewer_) {
        return false;
    }
    return !imageViewer_->getROIs().empty();
}

/**
 * @brief 绘制ROI按钮点击槽
 * @details 根据选择的ROI类型，进入相应的绘制模式
 */
void SearchToolDialog::onDrawROIClicked()
{
    // 检查是否已加载图像
    if (!currentImage_) {
        QMessageBox::warning(this, tr("警告"), tr("请先加载图像"));
        return;
    }

    if (imageViewer_) {
        // 获取选择的ROI类型
        int roiType = roiTypeCombo_->currentData().toInt();
        // 根据类型设置图像查看器的交互模式
        switch (roiType) {
            case 0:  // 矩形
                imageViewer_->setInteractionMode(HalconImageViewer::DrawRectangle);
                break;
            case 1:  // 椭圆
                imageViewer_->setInteractionMode(HalconImageViewer::DrawEllipse);
                break;
            case 2:  // 多边形
                imageViewer_->setInteractionMode(HalconImageViewer::DrawPolygon);
                break;
            default:  // 默认矩形
                imageViewer_->setInteractionMode(HalconImageViewer::DrawRectangle);
                break;
        }
        // 更新状态显示
        roiStatusLabel_->setText(tr("状态: 正在绘制..."));
        roiStatusLabel_->setStyleSheet("color: #2196F3;");  // 蓝色表示正在绘制
    }
}

/**
 * @brief 清除ROI按钮点击槽
 * @details 清除所有已绘制的ROI区域
 */
void SearchToolDialog::onClearROIClicked()
{
    if (imageViewer_) {
        // 清除所有ROI
        imageViewer_->clearROIs();
        // 更新状态显示
        roiStatusLabel_->setText(tr("状态: 未设置搜索区域（将搜索全图）"));
        roiStatusLabel_->setStyleSheet("color: #888; font-style: italic;");
    }
}

/**
 * @brief 预览按钮点击槽
 * @details 执行搜索并显示结果预览
 */
void SearchToolDialog::onPreviewClicked()
{
    // 检查是否已加载图像
    if (!currentImage_) {
        QMessageBox::warning(this, tr("警告"), tr("请先加载图像"));
        return;
    }

    // 应用子类特定的参数设置
    applySpecificParameters();

    // 创建计时器，记录搜索耗时
    QElapsedTimer timer;
    timer.start();

    // 执行搜索
    std::vector<SearchResult> results;
    if (executeSearch(results)) {
        // 计算耗时
        qint64 elapsed = timer.elapsed();
        searchTimeLabel_->setText(QString(tr("耗时: %1 ms")).arg(elapsed));

        // 保存搜索结果
        searchResults_ = results;
        // 更新结果表格
        updateResultTable(results);
        // 在图像上绘制结果
        drawSearchResults(results);

        // 发出搜索完成信号
        emit searchCompleted(results);
        LOG_INFO(QString("搜索完成，找到 %1 个结果，耗时 %2 ms")
            .arg(results.size()).arg(elapsed));
    } else {
        // 搜索失败
        searchTimeLabel_->setText(tr("耗时: -- ms"));
        QMessageBox::warning(this, tr("搜索失败"), tr("搜索过程中发生错误"));
    }
}

/**
 * @brief 自动预览触发槽
 * @details 参数变化时自动执行的简化版预览（不显示消息框）
 */
void SearchToolDialog::onAutoPreview()
{
    // 检查是否已加载图像
    if (!currentImage_) {
        return;
    }

    // 应用子类特定的参数设置
    applySpecificParameters();

    // 执行搜索
    std::vector<SearchResult> results;
    if (executeSearch(results)) {
        // 保存并显示结果
        searchResults_ = results;
        updateResultTable(results);
        drawSearchResults(results);
    }
}

/**
 * @brief 确定按钮点击槽
 * @details 应用参数设置并关闭对话框
 */
void SearchToolDialog::onOkClicked()
{
    applySpecificParameters();
    accept();  // 关闭对话框并返回Accepted
}

/**
 * @brief 取消按钮点击槽
 * @details 放弃修改并关闭对话框
 */
void SearchToolDialog::onCancelClicked()
{
    reject();  // 关闭对话框并返回Rejected
}

/**
 * @brief 应用按钮点击槽
 * @details 应用参数设置但不关闭对话框
 */
void SearchToolDialog::onApplyClicked()
{
    applySpecificParameters();
    // 发出参数变化信号
    emit parameterChanged();
}

/**
 * @brief 结果表格行选中槽
 * @details 当用户选中某一行时，在图像上高亮显示对应的匹配结果
 * @param row 选中的行号
 */
void SearchToolDialog::onResultTableRowSelected(int row)
{
    // 检查行号是否有效
    if (row < 0 || row >= static_cast<int>(searchResults_.size())) {
        return;
    }

    // 获取选中的结果
    const auto& result = searchResults_[row];

    // 在图像上高亮显示选中的结果
    if (imageViewer_) {
        // TODO: 添加居中显示功能
        // imageViewer_->centerOn(result.x, result.y);
    }

    // 记录日志
    LOG_DEBUG(QString("选中搜索结果 #%1: (%2, %3), 角度=%4°, 得分=%5")
        .arg(result.index)
        .arg(result.x, 0, 'f', 2)
        .arg(result.y, 0, 'f', 2)
        .arg(result.angle, 0, 'f', 2)
        .arg(result.score, 0, 'f', 4));
}

} // namespace UI
} // namespace VisionForge
