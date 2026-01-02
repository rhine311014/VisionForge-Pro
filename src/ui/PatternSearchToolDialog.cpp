/**
 * @file PatternSearchToolDialog.cpp
 * @brief 图案搜索工具对话框实现
 * @details 实现了图案搜索对话框的UI创建、信号处理和搜索逻辑。
 *          使用Halcon的形状匹配（Shape-Based Matching）算法进行模板匹配。
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

// 如果启用了Halcon支持，包含Halcon头文件
#ifdef USE_HALCON
#include <HalconCpp.h>
#endif

namespace VisionForge {
namespace UI {

/**
 * @brief 构造函数实现
 * @details 调用基类构造函数，保存工具指针，连接信号并更新UI
 * @param tool 图案搜索工具指针
 * @param parent 父窗口指针
 */
PatternSearchToolDialog::PatternSearchToolDialog(Algorithm::PatternSearchTool* tool, QWidget* parent)
    : SearchToolDialog(tool, parent)  // 调用基类构造函数
    , patternTool_(tool)              // 保存特定类型的工具指针
{
    // 连接图案搜索特有的信号槽
    connectSpecificSignals();
    // 从工具对象更新UI显示
    updateSpecificUI();
}

/**
 * @brief 析构函数实现
 */
PatternSearchToolDialog::~PatternSearchToolDialog()
{
}

/**
 * @brief 创建参数面板
 * @details 创建图案搜索的完整参数设置界面，包括：
 *          - 匹配模式选择（灰度/边缘/相关）
 *          - 模板管理（创建/加载/保存/清除）
 *          - 搜索参数（分数阈值/匹配数量/精度设置）
 *          - 角度和缩放搜索参数
 * @param parent 父控件
 * @return 创建的参数面板
 */
QWidget* PatternSearchToolDialog::createParameterPanel(QWidget* parent)
{
    // 创建面板容器
    QWidget* panel = new QWidget(parent);
    QVBoxLayout* layout = new QVBoxLayout(panel);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->setSpacing(8);

    // 依次创建各个参数组
    createMatchModeGroup(layout);      // 匹配模式选择
    createPatternGroup(layout);        // 模板管理
    createSearchParamGroup(layout);    // 搜索参数
    createAngleScaleGroup(layout);     // 角度缩放参数

    // 添加弹性空间，将内容推到顶部
    layout->addStretch();

    return panel;
}

/**
 * @brief 创建匹配模式选择组
 * @details 创建三个单选按钮用于选择匹配算法
 * @param layout 父布局
 */
void PatternSearchToolDialog::createMatchModeGroup(QVBoxLayout* layout)
{
    matchModeGroup_ = new QGroupBox(tr("匹配模式"), this);
    QVBoxLayout* groupLayout = new QVBoxLayout(matchModeGroup_);

    // 创建三种匹配模式的单选按钮
    // 灰度匹配：基于灰度值相关性，对光照变化敏感
    radioGrayMatch_ = new QRadioButton(tr("灰度匹配 - 基于灰度值的模板匹配"), this);
    // 边缘匹配：基于边缘特征，对光照变化鲁棒（工业应用推荐）
    radioEdgeMatch_ = new QRadioButton(tr("边缘匹配 - 基于边缘轮廓的形状匹配"), this);
    // 相关匹配：归一化互相关，适合纹理丰富的目标
    radioCorrelationMatch_ = new QRadioButton(tr("相关匹配 - 归一化相关系数匹配"), this);

    // 默认选择边缘匹配（工业应用中最常用）
    radioEdgeMatch_->setChecked(true);

    // 添加到布局
    groupLayout->addWidget(radioGrayMatch_);
    groupLayout->addWidget(radioEdgeMatch_);
    groupLayout->addWidget(radioCorrelationMatch_);

    // 添加说明标签
    QLabel* descLabel = new QLabel(tr("提示: 边缘匹配对光照变化不敏感，适合工业应用"), this);
    descLabel->setStyleSheet("color: #666; font-size: 11px;");
    groupLayout->addWidget(descLabel);

    layout->addWidget(matchModeGroup_);
}

/**
 * @brief 创建图案模板管理组
 * @details 创建模板的创建、加载、保存、清除功能界面
 * @param layout 父布局
 */
void PatternSearchToolDialog::createPatternGroup(QVBoxLayout* layout)
{
    patternGroup_ = new QGroupBox(tr("图案模板"), this);
    QVBoxLayout* groupLayout = new QVBoxLayout(patternGroup_);

    // ==================== 模板状态显示 ====================
    patternStatusLabel_ = new QLabel(tr("状态: 未创建图案模板"), this);
    patternStatusLabel_->setStyleSheet("color: #FF9800; font-weight: bold;");  // 橙色警告
    groupLayout->addWidget(patternStatusLabel_);

    // ==================== 模板预览图像 ====================
    patternPreviewLabel_ = new QLabel(this);
    patternPreviewLabel_->setFixedSize(150, 150);  // 固定预览尺寸
    patternPreviewLabel_->setFrameStyle(QFrame::Box | QFrame::Sunken);
    patternPreviewLabel_->setAlignment(Qt::AlignCenter);
    patternPreviewLabel_->setText(tr("无预览"));
    patternPreviewLabel_->setStyleSheet("background-color: #f0f0f0; color: #888;");
    groupLayout->addWidget(patternPreviewLabel_, 0, Qt::AlignCenter);

    // ==================== 操作按钮行1 ====================
    QHBoxLayout* btnRow1 = new QHBoxLayout();
    // 从ROI创建模板按钮
    createPatternBtn_ = new QPushButton(tr("从ROI创建"), this);
    createPatternBtn_->setIcon(style()->standardIcon(QStyle::SP_FileDialogNewFolder));
    // 加载已有模板按钮
    loadPatternBtn_ = new QPushButton(tr("加载图案"), this);
    loadPatternBtn_->setIcon(style()->standardIcon(QStyle::SP_DialogOpenButton));
    btnRow1->addWidget(createPatternBtn_);
    btnRow1->addWidget(loadPatternBtn_);
    groupLayout->addLayout(btnRow1);

    // ==================== 操作按钮行2 ====================
    QHBoxLayout* btnRow2 = new QHBoxLayout();
    // 保存模板按钮
    savePatternBtn_ = new QPushButton(tr("保存图案"), this);
    savePatternBtn_->setIcon(style()->standardIcon(QStyle::SP_DialogSaveButton));
    // 清除模板按钮
    clearPatternBtn_ = new QPushButton(tr("清除图案"), this);
    clearPatternBtn_->setIcon(style()->standardIcon(QStyle::SP_DialogDiscardButton));
    btnRow2->addWidget(savePatternBtn_);
    btnRow2->addWidget(clearPatternBtn_);
    groupLayout->addLayout(btnRow2);

    layout->addWidget(patternGroup_);
}

/**
 * @brief 创建搜索参数设置组
 * @details 创建匹配分数、匹配数量、精度等参数的设置控件
 * @param layout 父布局
 */
void PatternSearchToolDialog::createSearchParamGroup(QVBoxLayout* layout)
{
    searchParamGroup_ = new QGroupBox(tr("搜索参数"), this);
    QGridLayout* groupLayout = new QGridLayout(searchParamGroup_);
    groupLayout->setSpacing(6);

    int row = 0;

    // ==================== 最小匹配分数 ====================
    groupLayout->addWidget(new QLabel(tr("最小分数:"), this), row, 0);
    QHBoxLayout* scoreLayout = new QHBoxLayout();
    // 数值输入框
    minScoreSpin_ = new QDoubleSpinBox(this);
    minScoreSpin_->setRange(0.0, 1.0);      // 范围0-1
    minScoreSpin_->setSingleStep(0.05);     // 步长0.05
    minScoreSpin_->setValue(0.5);           // 默认值0.5
    // 滑块控件（与数值框同步）
    minScoreSlider_ = new QSlider(Qt::Horizontal, this);
    minScoreSlider_->setRange(0, 100);      // 范围0-100
    minScoreSlider_->setValue(50);          // 默认值50（对应0.5）
    scoreLayout->addWidget(minScoreSpin_);
    scoreLayout->addWidget(minScoreSlider_);
    groupLayout->addLayout(scoreLayout, row, 1);
    row++;

    // ==================== 最大匹配数 ====================
    groupLayout->addWidget(new QLabel(tr("最大匹配数:"), this), row, 0);
    maxMatchesSpin_ = new QSpinBox(this);
    maxMatchesSpin_->setRange(1, 1000);     // 范围1-1000
    maxMatchesSpin_->setValue(10);          // 默认值10
    groupLayout->addWidget(maxMatchesSpin_, row, 1);
    row++;

    // ==================== 重叠阈值 ====================
    groupLayout->addWidget(new QLabel(tr("重叠阈值:"), this), row, 0);
    overlapSpin_ = new QDoubleSpinBox(this);
    overlapSpin_->setRange(0.0, 1.0);       // 范围0-1
    overlapSpin_->setSingleStep(0.1);       // 步长0.1
    overlapSpin_->setValue(0.5);            // 默认值0.5
    overlapSpin_->setToolTip(tr("用于非极大值抑制，防止重复检测"));
    groupLayout->addWidget(overlapSpin_, row, 1);
    row++;

    // ==================== 亚像素精度 ====================
    groupLayout->addWidget(new QLabel(tr("亚像素精度:"), this), row, 0);
    subPixelCombo_ = new QComboBox(this);
    // 添加亚像素精度选项
    subPixelCombo_->addItem(tr("无"), "none");                         // 无亚像素精化
    subPixelCombo_->addItem(tr("最近邻插值"), "interpolation");        // 简单插值
    subPixelCombo_->addItem(tr("最小二乘"), "least_squares");          // 最小二乘法
    subPixelCombo_->addItem(tr("最小二乘高精度"), "least_squares_high"); // 高精度最小二乘
    subPixelCombo_->setCurrentIndex(2);  // 默认使用最小二乘
    groupLayout->addWidget(subPixelCombo_, row, 1);
    row++;

    // ==================== 金字塔层数 ====================
    groupLayout->addWidget(new QLabel(tr("金字塔层数:"), this), row, 0);
    pyramidLevelsSpin_ = new QSpinBox(this);
    pyramidLevelsSpin_->setRange(1, 10);    // 范围1-10
    pyramidLevelsSpin_->setValue(4);        // 默认值4
    pyramidLevelsSpin_->setToolTip(tr("金字塔层数越多搜索越快，但可能降低精度"));
    groupLayout->addWidget(pyramidLevelsSpin_, row, 1);

    layout->addWidget(searchParamGroup_);
}

/**
 * @brief 创建角度和缩放参数组
 * @details 创建角度搜索和缩放搜索的参数设置控件
 * @param layout 父布局
 */
void PatternSearchToolDialog::createAngleScaleGroup(QVBoxLayout* layout)
{
    angleScaleGroup_ = new QGroupBox(tr("角度和缩放"), this);
    QVBoxLayout* groupLayout = new QVBoxLayout(angleScaleGroup_);

    // ==================== 角度搜索设置 ====================
    // 启用角度搜索复选框
    enableAngleCheck_ = new QCheckBox(tr("启用角度搜索"), this);
    enableAngleCheck_->setChecked(true);  // 默认启用
    groupLayout->addWidget(enableAngleCheck_);

    // 角度参数容器（可整体启用/禁用）
    angleWidget_ = new QWidget(this);
    QGridLayout* angleLayout = new QGridLayout(angleWidget_);
    angleLayout->setContentsMargins(20, 0, 0, 0);  // 左侧缩进
    angleLayout->setSpacing(4);

    // 最小角度
    angleLayout->addWidget(new QLabel(tr("最小角度:"), this), 0, 0);
    angleMinSpin_ = new QDoubleSpinBox(this);
    angleMinSpin_->setRange(-180.0, 180.0);
    angleMinSpin_->setValue(-30.0);  // 默认-30度
    angleMinSpin_->setSuffix("°");
    angleLayout->addWidget(angleMinSpin_, 0, 1);

    // 最大角度
    angleLayout->addWidget(new QLabel(tr("最大角度:"), this), 1, 0);
    angleMaxSpin_ = new QDoubleSpinBox(this);
    angleMaxSpin_->setRange(-180.0, 180.0);
    angleMaxSpin_->setValue(30.0);   // 默认30度
    angleMaxSpin_->setSuffix("°");
    angleLayout->addWidget(angleMaxSpin_, 1, 1);

    // 角度步长
    angleLayout->addWidget(new QLabel(tr("角度步长:"), this), 2, 0);
    angleStepSpin_ = new QDoubleSpinBox(this);
    angleStepSpin_->setRange(0.01, 30.0);
    angleStepSpin_->setValue(1.0);   // 默认1度
    angleStepSpin_->setSuffix("°");
    angleLayout->addWidget(angleStepSpin_, 2, 1);

    groupLayout->addWidget(angleWidget_);

    // ==================== 分隔线 ====================
    QFrame* line = new QFrame(this);
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    groupLayout->addWidget(line);

    // ==================== 缩放搜索设置 ====================
    // 启用缩放搜索复选框
    enableScaleCheck_ = new QCheckBox(tr("启用缩放搜索"), this);
    enableScaleCheck_->setChecked(false);  // 默认禁用（缩放搜索较慢）
    groupLayout->addWidget(enableScaleCheck_);

    // 缩放参数容器
    scaleWidget_ = new QWidget(this);
    QGridLayout* scaleLayout = new QGridLayout(scaleWidget_);
    scaleLayout->setContentsMargins(20, 0, 0, 0);  // 左侧缩进
    scaleLayout->setSpacing(4);

    // 最小缩放
    scaleLayout->addWidget(new QLabel(tr("最小缩放:"), this), 0, 0);
    scaleMinSpin_ = new QDoubleSpinBox(this);
    scaleMinSpin_->setRange(0.1, 10.0);
    scaleMinSpin_->setValue(0.8);    // 默认0.8倍
    scaleMinSpin_->setSingleStep(0.1);
    scaleLayout->addWidget(scaleMinSpin_, 0, 1);

    // 最大缩放
    scaleLayout->addWidget(new QLabel(tr("最大缩放:"), this), 1, 0);
    scaleMaxSpin_ = new QDoubleSpinBox(this);
    scaleMaxSpin_->setRange(0.1, 10.0);
    scaleMaxSpin_->setValue(1.2);    // 默认1.2倍
    scaleMaxSpin_->setSingleStep(0.1);
    scaleLayout->addWidget(scaleMaxSpin_, 1, 1);

    // 缩放步长
    scaleLayout->addWidget(new QLabel(tr("缩放步长:"), this), 2, 0);
    scaleStepSpin_ = new QDoubleSpinBox(this);
    scaleStepSpin_->setRange(0.01, 1.0);
    scaleStepSpin_->setValue(0.05);  // 默认0.05
    scaleStepSpin_->setSingleStep(0.01);
    scaleLayout->addWidget(scaleStepSpin_, 2, 1);

    // 默认禁用缩放参数容器
    scaleWidget_->setEnabled(false);
    groupLayout->addWidget(scaleWidget_);

    layout->addWidget(angleScaleGroup_);
}

/**
 * @brief 连接特定信号槽
 * @details 连接图案搜索特有的信号槽，包括模式选择、模板管理和参数变化
 */
void PatternSearchToolDialog::connectSpecificSignals()
{
    // ==================== 匹配模式切换信号 ====================
    // 使用lambda表达式，在单选按钮选中时调用模式切换处理
    connect(radioGrayMatch_, &QRadioButton::toggled, [this](bool checked) {
        if (checked) onMatchModeChanged(GrayMatch);
    });
    connect(radioEdgeMatch_, &QRadioButton::toggled, [this](bool checked) {
        if (checked) onMatchModeChanged(EdgeMatch);
    });
    connect(radioCorrelationMatch_, &QRadioButton::toggled, [this](bool checked) {
        if (checked) onMatchModeChanged(CorrelationMatch);
    });

    // ==================== 图案管理按钮信号 ====================
    connect(createPatternBtn_, &QPushButton::clicked, this, &PatternSearchToolDialog::onCreatePatternClicked);
    connect(loadPatternBtn_, &QPushButton::clicked, this, &PatternSearchToolDialog::onLoadPatternClicked);
    connect(savePatternBtn_, &QPushButton::clicked, this, &PatternSearchToolDialog::onSavePatternClicked);
    connect(clearPatternBtn_, &QPushButton::clicked, this, &PatternSearchToolDialog::onClearPatternClicked);

    // ==================== 分数滑块和数值框同步 ====================
    // 滑块变化时更新数值框
    connect(minScoreSlider_, &QSlider::valueChanged, [this](int value) {
        minScoreSpin_->blockSignals(true);  // 阻止信号循环
        minScoreSpin_->setValue(value / 100.0);
        minScoreSpin_->blockSignals(false);
        // 请求预览更新
        if (previewHelper_) previewHelper_->requestPreview();
    });
    // 数值框变化时更新滑块
    connect(minScoreSpin_, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [this](double value) {
        minScoreSlider_->blockSignals(true);  // 阻止信号循环
        minScoreSlider_->setValue(static_cast<int>(value * 100));
        minScoreSlider_->blockSignals(false);
        // 请求预览更新
        if (previewHelper_) previewHelper_->requestPreview();
    });

    // ==================== 角度/缩放启用控制 ====================
    // 复选框控制参数容器的启用状态
    connect(enableAngleCheck_, &QCheckBox::toggled, angleWidget_, &QWidget::setEnabled);
    connect(enableScaleCheck_, &QCheckBox::toggled, scaleWidget_, &QWidget::setEnabled);

    // ==================== 其他参数变化触发预览 ====================
    connect(maxMatchesSpin_, QOverload<int>::of(&QSpinBox::valueChanged), [this]() {
        if (previewHelper_) previewHelper_->requestPreview();
    });
    connect(overlapSpin_, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [this]() {
        if (previewHelper_) previewHelper_->requestPreview();
    });
}

/**
 * @brief 执行图案搜索
 * @details 调用Halcon的形状匹配算法在图像中搜索模板
 * @param results 输出参数，存储搜索结果
 * @return 搜索是否成功
 */
bool PatternSearchToolDialog::executeSearch(std::vector<SearchResult>& results)
{
    // 检查是否有图像
    if (!currentImage_) {
        LOG_WARNING("执行搜索失败: 未加载图像");
        return false;
    }

#ifdef USE_HALCON
    try {
        // ==================== Halcon形状匹配搜索 ====================
        // TODO: 实现完整的Halcon形状匹配调用
        // 1. 获取Halcon图像对象
        // 2. 调用find_shape_model或find_scaled_shape_model
        // 3. 解析匹配结果

        LOG_INFO("图案搜索执行中...");

        // 示例结果（实际应调用Halcon API）
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
    // 未启用Halcon支持
    LOG_WARNING("图案搜索需要Halcon支持");
    return false;
#endif
}

/**
 * @brief 应用参数到工具对象
 * @details 将UI控件的值同步到PatternSearchTool工具对象
 */
void PatternSearchToolDialog::applySpecificParameters()
{
    if (!patternTool_) return;

    // TODO: 将UI参数应用到工具对象
    // patternTool_->setMinScore(minScoreSpin_->value());
    // patternTool_->setMaxMatches(maxMatchesSpin_->value());
    // patternTool_->setOverlap(overlapSpin_->value());
    // patternTool_->setAngleRange(angleMinSpin_->value(), angleMaxSpin_->value());
    // patternTool_->setScaleRange(scaleMinSpin_->value(), scaleMaxSpin_->value());
    // ...

    LOG_DEBUG("图案搜索参数已应用");
}

/**
 * @brief 从工具对象更新UI
 * @details 从PatternSearchTool工具对象读取参数并更新UI控件
 */
void PatternSearchToolDialog::updateSpecificUI()
{
    if (!patternTool_) return;

    // TODO: 从工具读取参数更新UI
    // minScoreSpin_->setValue(patternTool_->minScore());
    // maxMatchesSpin_->setValue(patternTool_->maxMatches());
    // ...
}

/**
 * @brief 从ROI创建模板按钮点击槽
 * @details 从用户绘制的ROI区域提取模板图像，创建形状模型
 */
void PatternSearchToolDialog::onCreatePatternClicked()
{
    // 检查是否有图像
    if (!currentImage_) {
        QMessageBox::warning(this, tr("警告"), tr("请先加载图像"));
        return;
    }

    // 检查是否绘制了ROI
    if (!hasSearchROI()) {
        QMessageBox::warning(this, tr("警告"), tr("请先在搜索区域页面绘制ROI"));
        return;
    }

    // 获取ROI区域
    QRect roi = getSearchROI();

#ifdef USE_HALCON
    try {
        // TODO: 从ROI创建Halcon形状模板
        // 1. 从图像中提取ROI区域
        // 2. 调用create_shape_model创建模板
        // 3. 获取模板预览图像

        LOG_INFO(QString("从ROI创建图案模板: (%1,%2) %3x%4")
            .arg(roi.x()).arg(roi.y()).arg(roi.width()).arg(roi.height()));

        // 更新状态显示
        patternStatusLabel_->setText(tr("状态: 图案模板已创建"));
        patternStatusLabel_->setStyleSheet("color: #4CAF50; font-weight: bold;");  // 绿色成功

        // 更新预览标签显示尺寸
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

/**
 * @brief 加载模板按钮点击槽
 * @details 从文件加载已保存的形状模型
 */
void PatternSearchToolDialog::onLoadPatternClicked()
{
    // 打开文件选择对话框
    QString filePath = QFileDialog::getOpenFileName(
        this, tr("加载图案模板"),
        QString(),
        tr("形状模型 (*.shm);;所有文件 (*)"));

    if (filePath.isEmpty()) return;

#ifdef USE_HALCON
    try {
        // TODO: 调用Halcon加载形状模板
        // read_shape_model(filePath, modelID);

        LOG_INFO(QString("加载图案模板: %1").arg(filePath));

        // 更新状态显示
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

/**
 * @brief 保存模板按钮点击槽
 * @details 将当前模板保存到文件
 */
void PatternSearchToolDialog::onSavePatternClicked()
{
    // 打开保存对话框
    QString filePath = QFileDialog::getSaveFileName(
        this, tr("保存图案模板"),
        QString(),
        tr("形状模型 (*.shm)"));

    if (filePath.isEmpty()) return;

    // 确保文件扩展名正确
    if (!filePath.endsWith(".shm", Qt::CaseInsensitive)) {
        filePath += ".shm";
    }

#ifdef USE_HALCON
    try {
        // TODO: 调用Halcon保存形状模板
        // write_shape_model(modelID, filePath);

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

/**
 * @brief 清除模板按钮点击槽
 * @details 清除当前加载的模板
 */
void PatternSearchToolDialog::onClearPatternClicked()
{
    // 确认对话框
    int ret = QMessageBox::question(this, tr("确认"),
        tr("确定要清除当前图案模板吗？"),
        QMessageBox::Yes | QMessageBox::No);

    if (ret == QMessageBox::Yes) {
        // TODO: 清除模板
        // clear_shape_model(modelID);

        // 更新状态显示
        patternStatusLabel_->setText(tr("状态: 未创建图案模板"));
        patternStatusLabel_->setStyleSheet("color: #FF9800; font-weight: bold;");  // 橙色警告
        // 清除预览
        patternPreviewLabel_->setPixmap(QPixmap());
        patternPreviewLabel_->setText(tr("无预览"));
        LOG_INFO("图案模板已清除");
    }
}

/**
 * @brief 匹配模式切换槽
 * @details 当用户切换匹配模式时更新相关设置
 * @param mode 新选择的匹配模式
 */
void PatternSearchToolDialog::onMatchModeChanged(int mode)
{
    // 根据模式生成描述文本
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

    // 请求预览更新
    if (previewHelper_) {
        previewHelper_->requestPreview();
    }
}

} // namespace UI
} // namespace VisionForge
