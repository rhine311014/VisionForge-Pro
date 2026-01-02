/**
 * @file VirtualObjectToolDialog.cpp
 * @brief 虚拟对象计算工具对话框实现
 * @author VisionForge Team
 * @date 2025-12-30
 */

#include "ui/VirtualObjectToolDialog.h"
#include "ui/HalconImageViewer.h"
#include "algorithm/VirtualObjectTool.h"
#include "base/Logger.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QStyle>

namespace VisionForge {
namespace UI {

VirtualObjectToolDialog::VirtualObjectToolDialog(Algorithm::VirtualObjectTool* tool, QWidget* parent)
    : IToolDialog(parent)
    , tool_(tool)
{
    setupUI();
    connectSignals();
    updateMethodPanel();
}

VirtualObjectToolDialog::~VirtualObjectToolDialog()
{
}

void VirtualObjectToolDialog::setImage(Base::ImageData::Ptr image)
{
    currentImage_ = image;
    if (imageViewer_ && image) {
        imageViewer_->setImage(image);
        if (imageInfoLabel_) {
            imageInfoLabel_->setText(QString(tr("图像: %1 x %2"))
                .arg(image->width())
                .arg(image->height()));
        }
    }
}

Algorithm::VisionTool* VirtualObjectToolDialog::tool() const
{
    return tool_;
}

void VirtualObjectToolDialog::setupUI()
{
    setWindowTitle(tr("虚拟对象计算"));
    setMinimumSize(1000, 700);
    resize(1100, 750);
    setWindowFlags(windowFlags() | Qt::WindowMaximizeButtonHint);

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
    previewHelper_ = new PreviewHelper(this, 100);

    // 底部按钮
    QHBoxLayout* bottomLayout = new QHBoxLayout();

    calculateBtn_ = new QPushButton(tr("计算"), this);
    calculateBtn_->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    calculateBtn_->setMinimumWidth(80);

    previewBtn_ = new QPushButton(tr("预览"), this);
    previewBtn_->setMinimumWidth(80);

    bottomLayout->addWidget(calculateBtn_);
    bottomLayout->addWidget(previewBtn_);
    bottomLayout->addStretch();

    okBtn_ = new QPushButton(tr("确定"), this);
    okBtn_->setMinimumWidth(80);
    cancelBtn_ = new QPushButton(tr("取消"), this);
    cancelBtn_->setMinimumWidth(80);
    applyBtn_ = new QPushButton(tr("应用"), this);
    applyBtn_->setMinimumWidth(80);

    bottomLayout->addWidget(okBtn_);
    bottomLayout->addWidget(cancelBtn_);
    bottomLayout->addWidget(applyBtn_);

    mainLayout->addLayout(bottomLayout);
}

void VirtualObjectToolDialog::createLeftPanel(QWidget* parent)
{
    QVBoxLayout* layout = new QVBoxLayout(parent);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(4);

    // 标题
    QHBoxLayout* titleLayout = new QHBoxLayout();
    QLabel* titleLabel = new QLabel(tr("预览图像"), parent);
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

void VirtualObjectToolDialog::createRightPanel(QWidget* parent)
{
    QVBoxLayout* layout = new QVBoxLayout(parent);
    layout->setContentsMargins(4, 0, 0, 0);
    layout->setSpacing(8);

    // 对象类型选择
    createObjectTypeGroup(layout);

    // 方法选择面板
    methodGroup_ = new QGroupBox(tr("计算方法"), parent);
    QVBoxLayout* methodLayout = new QVBoxLayout(methodGroup_);

    methodStack_ = new QStackedWidget(methodGroup_);
    createPointMethodPanel();
    createLineMethodPanel();
    createCircleMethodPanel();
    methodStack_->addWidget(pointMethodPanel_);
    methodStack_->addWidget(lineMethodPanel_);
    methodStack_->addWidget(circleMethodPanel_);
    methodLayout->addWidget(methodStack_);

    layout->addWidget(methodGroup_);

    // 输入源选择
    createInputSourceGroup(layout);

    // 结果显示
    createResultGroup(layout);
}

void VirtualObjectToolDialog::createObjectTypeGroup(QVBoxLayout* layout)
{
    objectTypeGroup_ = new QGroupBox(tr("对象类型"), this);
    QVBoxLayout* groupLayout = new QVBoxLayout(objectTypeGroup_);

    QHBoxLayout* comboLayout = new QHBoxLayout();
    comboLayout->addWidget(new QLabel(tr("类型:"), this));
    objectTypeCombo_ = new QComboBox(this);
    objectTypeCombo_->addItem(tr("虚拟点"), VirtualPoint);
    objectTypeCombo_->addItem(tr("虚拟线"), VirtualLine);
    objectTypeCombo_->addItem(tr("虚拟圆"), VirtualCircle);
    comboLayout->addWidget(objectTypeCombo_, 1);
    groupLayout->addLayout(comboLayout);

    objectTypeDescLabel_ = new QLabel(tr("虚拟点：通过几何关系计算得到的点坐标"), this);
    objectTypeDescLabel_->setWordWrap(true);
    objectTypeDescLabel_->setStyleSheet("color: #666; font-size: 11px;");
    groupLayout->addWidget(objectTypeDescLabel_);

    layout->addWidget(objectTypeGroup_);
}

void VirtualObjectToolDialog::createPointMethodPanel()
{
    pointMethodPanel_ = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(pointMethodPanel_);
    layout->setContentsMargins(0, 0, 0, 0);

    // 方法选择
    QHBoxLayout* comboLayout = new QHBoxLayout();
    comboLayout->addWidget(new QLabel(tr("计算方法:"), this));
    pointMethodCombo_ = new QComboBox(this);
    pointMethodCombo_->addItem(tr("两线交点"), LineLineIntersection);
    pointMethodCombo_->addItem(tr("两圆交点"), CircleCircleIntersection);
    pointMethodCombo_->addItem(tr("两点中点"), PointMidpoint);
    pointMethodCombo_->addItem(tr("多点重心"), PointsCentroid);
    pointMethodCombo_->addItem(tr("点到线投影"), PointToLineProjection);
    pointMethodCombo_->addItem(tr("线圆交点"), LineCircleIntersection);
    pointMethodCombo_->addItem(tr("固定点坐标"), FixedPoint);
    comboLayout->addWidget(pointMethodCombo_, 1);
    layout->addLayout(comboLayout);

    pointMethodDescLabel_ = new QLabel(tr("计算两条直线的交点坐标"), this);
    pointMethodDescLabel_->setStyleSheet("color: #666; font-size: 11px;");
    layout->addWidget(pointMethodDescLabel_);

    // 参数面板（堆叠）
    pointParamStack_ = new QStackedWidget(this);

    // 固定点参数
    QWidget* fixedPointParams = new QWidget(this);
    QGridLayout* fixedLayout = new QGridLayout(fixedPointParams);
    fixedLayout->addWidget(new QLabel(tr("X坐标:"), this), 0, 0);
    fixedXSpin_ = new QDoubleSpinBox(this);
    fixedXSpin_->setRange(-99999, 99999);
    fixedXSpin_->setDecimals(3);
    fixedLayout->addWidget(fixedXSpin_, 0, 1);
    fixedLayout->addWidget(new QLabel(tr("Y坐标:"), this), 1, 0);
    fixedYSpin_ = new QDoubleSpinBox(this);
    fixedYSpin_->setRange(-99999, 99999);
    fixedYSpin_->setDecimals(3);
    fixedLayout->addWidget(fixedYSpin_, 1, 1);

    // 交点索引参数
    QWidget* intersectionParams = new QWidget(this);
    QHBoxLayout* intersectionLayout = new QHBoxLayout(intersectionParams);
    intersectionLayout->addWidget(new QLabel(tr("交点索引:"), this));
    intersectionIndexSpin_ = new QSpinBox(this);
    intersectionIndexSpin_->setRange(1, 2);
    intersectionIndexSpin_->setValue(1);
    intersectionIndexSpin_->setToolTip(tr("两圆可能有两个交点，选择第1个或第2个"));
    intersectionLayout->addWidget(intersectionIndexSpin_);
    intersectionLayout->addStretch();

    pointParamStack_->addWidget(new QWidget(this));  // 占位
    pointParamStack_->addWidget(intersectionParams);
    pointParamStack_->addWidget(new QWidget(this));
    pointParamStack_->addWidget(new QWidget(this));
    pointParamStack_->addWidget(new QWidget(this));
    pointParamStack_->addWidget(intersectionParams);
    pointParamStack_->addWidget(fixedPointParams);

    layout->addWidget(pointParamStack_);
    layout->addStretch();
}

void VirtualObjectToolDialog::createLineMethodPanel()
{
    lineMethodPanel_ = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(lineMethodPanel_);
    layout->setContentsMargins(0, 0, 0, 0);

    // 方法选择
    QHBoxLayout* comboLayout = new QHBoxLayout();
    comboLayout->addWidget(new QLabel(tr("计算方法:"), this));
    lineMethodCombo_ = new QComboBox(this);
    lineMethodCombo_->addItem(tr("两点连线"), TwoPointsLine);
    lineMethodCombo_->addItem(tr("平行线"), ParallelLine);
    lineMethodCombo_->addItem(tr("垂直线"), PerpendicularLine);
    lineMethodCombo_->addItem(tr("角平分线"), AngleBisector);
    lineMethodCombo_->addItem(tr("切线"), TangentLine);
    lineMethodCombo_->addItem(tr("多点拟合线"), PointsRegression);
    lineMethodCombo_->addItem(tr("固定线参数"), FixedLine);
    comboLayout->addWidget(lineMethodCombo_, 1);
    layout->addLayout(comboLayout);

    lineMethodDescLabel_ = new QLabel(tr("通过两个点计算连线"), this);
    lineMethodDescLabel_->setStyleSheet("color: #666; font-size: 11px;");
    layout->addWidget(lineMethodDescLabel_);

    // 参数面板
    lineParamStack_ = new QStackedWidget(this);

    // 偏移距离参数（用于平行线）
    QWidget* offsetParams = new QWidget(this);
    QHBoxLayout* offsetLayout = new QHBoxLayout(offsetParams);
    offsetLayout->addWidget(new QLabel(tr("偏移距离:"), this));
    offsetDistanceSpin_ = new QDoubleSpinBox(this);
    offsetDistanceSpin_->setRange(-9999, 9999);
    offsetDistanceSpin_->setDecimals(3);
    offsetDistanceSpin_->setSuffix(" px");
    offsetLayout->addWidget(offsetDistanceSpin_);
    offsetLayout->addStretch();

    // 固定线参数
    QWidget* fixedLineParams = new QWidget(this);
    QGridLayout* fixedLineLayout = new QGridLayout(fixedLineParams);
    fixedLineLayout->addWidget(new QLabel(tr("过点X:"), this), 0, 0);
    QDoubleSpinBox* lineXSpin = new QDoubleSpinBox(this);
    lineXSpin->setRange(-99999, 99999);
    fixedLineLayout->addWidget(lineXSpin, 0, 1);
    fixedLineLayout->addWidget(new QLabel(tr("过点Y:"), this), 1, 0);
    QDoubleSpinBox* lineYSpin = new QDoubleSpinBox(this);
    lineYSpin->setRange(-99999, 99999);
    fixedLineLayout->addWidget(lineYSpin, 1, 1);
    fixedLineLayout->addWidget(new QLabel(tr("角度:"), this), 2, 0);
    fixedAngleSpin_ = new QDoubleSpinBox(this);
    fixedAngleSpin_->setRange(-180, 180);
    fixedAngleSpin_->setSuffix("°");
    fixedLineLayout->addWidget(fixedAngleSpin_, 2, 1);

    lineParamStack_->addWidget(new QWidget(this));  // 两点连线无额外参数
    lineParamStack_->addWidget(offsetParams);       // 平行线
    lineParamStack_->addWidget(new QWidget(this));  // 垂直线
    lineParamStack_->addWidget(new QWidget(this));  // 角平分线
    lineParamStack_->addWidget(new QWidget(this));  // 切线
    lineParamStack_->addWidget(new QWidget(this));  // 多点拟合
    lineParamStack_->addWidget(fixedLineParams);    // 固定线

    layout->addWidget(lineParamStack_);
    layout->addStretch();
}

void VirtualObjectToolDialog::createCircleMethodPanel()
{
    circleMethodPanel_ = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(circleMethodPanel_);
    layout->setContentsMargins(0, 0, 0, 0);

    // 方法选择
    QHBoxLayout* comboLayout = new QHBoxLayout();
    comboLayout->addWidget(new QLabel(tr("计算方法:"), this));
    circleMethodCombo_ = new QComboBox(this);
    circleMethodCombo_->addItem(tr("三点拟合圆"), ThreePointsCircle);
    circleMethodCombo_->addItem(tr("同心圆"), ConcentricCircle);
    circleMethodCombo_->addItem(tr("过点的圆"), ThroughPointCircle);
    circleMethodCombo_->addItem(tr("多点拟合圆"), PointsRegressionCircle);
    circleMethodCombo_->addItem(tr("固定圆参数"), FixedCircle);
    comboLayout->addWidget(circleMethodCombo_, 1);
    layout->addLayout(comboLayout);

    circleMethodDescLabel_ = new QLabel(tr("通过三个点拟合圆"), this);
    circleMethodDescLabel_->setStyleSheet("color: #666; font-size: 11px;");
    layout->addWidget(circleMethodDescLabel_);

    // 参数面板
    circleParamStack_ = new QStackedWidget(this);

    // 同心圆参数
    QWidget* concentricParams = new QWidget(this);
    QHBoxLayout* concentricLayout = new QHBoxLayout(concentricParams);
    concentricLayout->addWidget(new QLabel(tr("半径偏移:"), this));
    QDoubleSpinBox* radiusOffsetSpin = new QDoubleSpinBox(this);
    radiusOffsetSpin->setRange(-9999, 9999);
    radiusOffsetSpin->setSuffix(" px");
    concentricLayout->addWidget(radiusOffsetSpin);
    concentricLayout->addStretch();

    // 固定圆参数
    QWidget* fixedCircleParams = new QWidget(this);
    QGridLayout* fixedCircleLayout = new QGridLayout(fixedCircleParams);
    fixedCircleLayout->addWidget(new QLabel(tr("圆心X:"), this), 0, 0);
    QDoubleSpinBox* circleCXSpin = new QDoubleSpinBox(this);
    circleCXSpin->setRange(-99999, 99999);
    fixedCircleLayout->addWidget(circleCXSpin, 0, 1);
    fixedCircleLayout->addWidget(new QLabel(tr("圆心Y:"), this), 1, 0);
    QDoubleSpinBox* circleCYSpin = new QDoubleSpinBox(this);
    circleCYSpin->setRange(-99999, 99999);
    fixedCircleLayout->addWidget(circleCYSpin, 1, 1);
    fixedCircleLayout->addWidget(new QLabel(tr("半径:"), this), 2, 0);
    fixedRadiusSpin_ = new QDoubleSpinBox(this);
    fixedRadiusSpin_->setRange(0, 99999);
    fixedRadiusSpin_->setSuffix(" px");
    fixedCircleLayout->addWidget(fixedRadiusSpin_, 2, 1);

    circleParamStack_->addWidget(new QWidget(this));  // 三点拟合
    circleParamStack_->addWidget(concentricParams);   // 同心圆
    circleParamStack_->addWidget(new QWidget(this));  // 过点的圆
    circleParamStack_->addWidget(new QWidget(this));  // 多点拟合
    circleParamStack_->addWidget(fixedCircleParams);  // 固定圆

    layout->addWidget(circleParamStack_);
    layout->addStretch();
}

void VirtualObjectToolDialog::createInputSourceGroup(QVBoxLayout* layout)
{
    inputSourceGroup_ = new QGroupBox(tr("输入源"), this);
    QHBoxLayout* groupLayout = new QHBoxLayout(inputSourceGroup_);

    // 可用输入源
    QVBoxLayout* availableLayout = new QVBoxLayout();
    availableLayout->addWidget(new QLabel(tr("可用对象:"), this));
    availableSourcesList_ = new QListWidget(this);
    availableSourcesList_->setMaximumHeight(120);
    // 添加示例对象
    availableSourcesList_->addItem(tr("Line1 - 直线检测结果"));
    availableSourcesList_->addItem(tr("Line2 - 直线检测结果"));
    availableSourcesList_->addItem(tr("Circle1 - 圆检测结果"));
    availableSourcesList_->addItem(tr("Point1 - 角点检测结果"));
    availableLayout->addWidget(availableSourcesList_);
    groupLayout->addLayout(availableLayout);

    // 添加/移除按钮
    QVBoxLayout* btnLayout = new QVBoxLayout();
    btnLayout->addStretch();
    addSourceBtn_ = new QPushButton(">>", this);
    addSourceBtn_->setMaximumWidth(40);
    removeSourceBtn_ = new QPushButton("<<", this);
    removeSourceBtn_->setMaximumWidth(40);
    btnLayout->addWidget(addSourceBtn_);
    btnLayout->addWidget(removeSourceBtn_);
    btnLayout->addStretch();
    groupLayout->addLayout(btnLayout);

    // 已选输入源
    QVBoxLayout* selectedLayout = new QVBoxLayout();
    selectedLayout->addWidget(new QLabel(tr("已选对象:"), this));
    selectedSourcesList_ = new QListWidget(this);
    selectedSourcesList_->setMaximumHeight(120);
    selectedLayout->addWidget(selectedSourcesList_);
    groupLayout->addLayout(selectedLayout);

    layout->addWidget(inputSourceGroup_);
}

void VirtualObjectToolDialog::createResultGroup(QVBoxLayout* layout)
{
    resultGroup_ = new QGroupBox(tr("计算结果"), this);
    QVBoxLayout* groupLayout = new QVBoxLayout(resultGroup_);

    resultStatusLabel_ = new QLabel(tr("状态: 等待计算"), this);
    resultStatusLabel_->setStyleSheet("color: #888;");
    groupLayout->addWidget(resultStatusLabel_);

    resultTable_ = new QTableWidget(this);
    resultTable_->setColumnCount(3);
    resultTable_->setHorizontalHeaderLabels({tr("属性"), tr("值"), tr("单位")});
    resultTable_->horizontalHeader()->setStretchLastSection(true);
    resultTable_->setMaximumHeight(100);
    resultTable_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    groupLayout->addWidget(resultTable_);

    layout->addWidget(resultGroup_);
}

void VirtualObjectToolDialog::connectSignals()
{
    connect(objectTypeCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &VirtualObjectToolDialog::onObjectTypeChanged);

    connect(pointMethodCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &VirtualObjectToolDialog::onMethodChanged);
    connect(lineMethodCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &VirtualObjectToolDialog::onMethodChanged);
    connect(circleMethodCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &VirtualObjectToolDialog::onMethodChanged);

    connect(addSourceBtn_, &QPushButton::clicked, [this]() {
        auto items = availableSourcesList_->selectedItems();
        for (auto item : items) {
            selectedSourcesList_->addItem(item->text());
        }
    });

    connect(removeSourceBtn_, &QPushButton::clicked, [this]() {
        auto items = selectedSourcesList_->selectedItems();
        for (auto item : items) {
            delete selectedSourcesList_->takeItem(selectedSourcesList_->row(item));
        }
    });

    connect(calculateBtn_, &QPushButton::clicked, this, &VirtualObjectToolDialog::onCalculateClicked);
    connect(previewBtn_, &QPushButton::clicked, this, &VirtualObjectToolDialog::onPreviewClicked);
    connect(okBtn_, &QPushButton::clicked, this, &VirtualObjectToolDialog::onOkClicked);
    connect(cancelBtn_, &QPushButton::clicked, this, &VirtualObjectToolDialog::onCancelClicked);
    connect(applyBtn_, &QPushButton::clicked, this, &VirtualObjectToolDialog::onApplyClicked);
}

void VirtualObjectToolDialog::updateMethodPanel()
{
    ObjectType type = static_cast<ObjectType>(objectTypeCombo_->currentData().toInt());
    methodStack_->setCurrentIndex(static_cast<int>(type));

    switch (type) {
        case VirtualPoint:
            objectTypeDescLabel_->setText(tr("虚拟点：通过几何关系计算得到的点坐标"));
            break;
        case VirtualLine:
            objectTypeDescLabel_->setText(tr("虚拟线：通过几何关系计算得到的直线"));
            break;
        case VirtualCircle:
            objectTypeDescLabel_->setText(tr("虚拟圆：通过几何关系计算得到的圆"));
            break;
    }
}

void VirtualObjectToolDialog::updateInputSources()
{
    // TODO: 从工具链获取可用的检测结果
}

void VirtualObjectToolDialog::updateResult()
{
    // TODO: 更新计算结果显示
}

void VirtualObjectToolDialog::drawVirtualObject()
{
    if (!imageViewer_) return;

    // TODO: 在图像上绘制虚拟对象
    imageViewer_->update();
}

void VirtualObjectToolDialog::onObjectTypeChanged(int index)
{
    Q_UNUSED(index);
    updateMethodPanel();
}

void VirtualObjectToolDialog::onMethodChanged(int index)
{
    ObjectType type = static_cast<ObjectType>(objectTypeCombo_->currentData().toInt());

    switch (type) {
        case VirtualPoint: {
            PointMethod method = static_cast<PointMethod>(pointMethodCombo_->currentData().toInt());
            pointParamStack_->setCurrentIndex(static_cast<int>(method));
            switch (method) {
                case LineLineIntersection:
                    pointMethodDescLabel_->setText(tr("计算两条直线的交点坐标"));
                    break;
                case CircleCircleIntersection:
                    pointMethodDescLabel_->setText(tr("计算两个圆的交点坐标（可能有0-2个交点）"));
                    break;
                case PointMidpoint:
                    pointMethodDescLabel_->setText(tr("计算两个点的中点坐标"));
                    break;
                case PointsCentroid:
                    pointMethodDescLabel_->setText(tr("计算多个点的几何重心"));
                    break;
                case PointToLineProjection:
                    pointMethodDescLabel_->setText(tr("计算点到直线的垂足（投影点）"));
                    break;
                case LineCircleIntersection:
                    pointMethodDescLabel_->setText(tr("计算直线与圆的交点"));
                    break;
                case FixedPoint:
                    pointMethodDescLabel_->setText(tr("使用固定的坐标值"));
                    break;
            }
            break;
        }
        case VirtualLine: {
            LineMethod method = static_cast<LineMethod>(lineMethodCombo_->currentData().toInt());
            lineParamStack_->setCurrentIndex(static_cast<int>(method));
            switch (method) {
                case TwoPointsLine:
                    lineMethodDescLabel_->setText(tr("通过两个点计算连线"));
                    break;
                case ParallelLine:
                    lineMethodDescLabel_->setText(tr("计算与已知直线平行的线"));
                    break;
                case PerpendicularLine:
                    lineMethodDescLabel_->setText(tr("计算过点且垂直于已知直线的线"));
                    break;
                case AngleBisector:
                    lineMethodDescLabel_->setText(tr("计算两条直线的角平分线"));
                    break;
                case TangentLine:
                    lineMethodDescLabel_->setText(tr("计算过点且与圆相切的切线"));
                    break;
                case PointsRegression:
                    lineMethodDescLabel_->setText(tr("通过多个点拟合直线"));
                    break;
                case FixedLine:
                    lineMethodDescLabel_->setText(tr("使用固定的线参数"));
                    break;
            }
            break;
        }
        case VirtualCircle: {
            CircleMethod method = static_cast<CircleMethod>(circleMethodCombo_->currentData().toInt());
            circleParamStack_->setCurrentIndex(static_cast<int>(method));
            switch (method) {
                case ThreePointsCircle:
                    circleMethodDescLabel_->setText(tr("通过三个点拟合圆"));
                    break;
                case ConcentricCircle:
                    circleMethodDescLabel_->setText(tr("创建与已知圆同心的圆"));
                    break;
                case ThroughPointCircle:
                    circleMethodDescLabel_->setText(tr("创建过指定点且圆心在已知线上的圆"));
                    break;
                case PointsRegressionCircle:
                    circleMethodDescLabel_->setText(tr("通过多个点拟合圆"));
                    break;
                case FixedCircle:
                    circleMethodDescLabel_->setText(tr("使用固定的圆参数"));
                    break;
            }
            break;
        }
    }

    Q_UNUSED(index);
}

void VirtualObjectToolDialog::onCalculateClicked()
{
    if (selectedSourcesList_->count() == 0) {
        QMessageBox::warning(this, tr("警告"), tr("请先选择输入源"));
        return;
    }

    // TODO: 执行计算
    LOG_INFO("执行虚拟对象计算...");

    // 模拟计算结果
    resultTable_->setRowCount(0);

    ObjectType type = static_cast<ObjectType>(objectTypeCombo_->currentData().toInt());
    switch (type) {
        case VirtualPoint: {
            resultTable_->setRowCount(2);
            resultTable_->setItem(0, 0, new QTableWidgetItem(tr("X坐标")));
            resultTable_->setItem(0, 1, new QTableWidgetItem("256.35"));
            resultTable_->setItem(0, 2, new QTableWidgetItem("px"));
            resultTable_->setItem(1, 0, new QTableWidgetItem(tr("Y坐标")));
            resultTable_->setItem(1, 1, new QTableWidgetItem("192.78"));
            resultTable_->setItem(1, 2, new QTableWidgetItem("px"));
            break;
        }
        case VirtualLine: {
            resultTable_->setRowCount(3);
            resultTable_->setItem(0, 0, new QTableWidgetItem(tr("起点")));
            resultTable_->setItem(0, 1, new QTableWidgetItem("(100.0, 150.0)"));
            resultTable_->setItem(0, 2, new QTableWidgetItem("px"));
            resultTable_->setItem(1, 0, new QTableWidgetItem(tr("终点")));
            resultTable_->setItem(1, 1, new QTableWidgetItem("(400.0, 300.0)"));
            resultTable_->setItem(1, 2, new QTableWidgetItem("px"));
            resultTable_->setItem(2, 0, new QTableWidgetItem(tr("角度")));
            resultTable_->setItem(2, 1, new QTableWidgetItem("26.57"));
            resultTable_->setItem(2, 2, new QTableWidgetItem("°"));
            break;
        }
        case VirtualCircle: {
            resultTable_->setRowCount(3);
            resultTable_->setItem(0, 0, new QTableWidgetItem(tr("圆心X")));
            resultTable_->setItem(0, 1, new QTableWidgetItem("320.0"));
            resultTable_->setItem(0, 2, new QTableWidgetItem("px"));
            resultTable_->setItem(1, 0, new QTableWidgetItem(tr("圆心Y")));
            resultTable_->setItem(1, 1, new QTableWidgetItem("240.0"));
            resultTable_->setItem(1, 2, new QTableWidgetItem("px"));
            resultTable_->setItem(2, 0, new QTableWidgetItem(tr("半径")));
            resultTable_->setItem(2, 1, new QTableWidgetItem("85.6"));
            resultTable_->setItem(2, 2, new QTableWidgetItem("px"));
            break;
        }
    }

    resultStatusLabel_->setText(tr("状态: 计算成功"));
    resultStatusLabel_->setStyleSheet("color: #4CAF50; font-weight: bold;");

    drawVirtualObject();
}

void VirtualObjectToolDialog::onPreviewClicked()
{
    onCalculateClicked();
}

void VirtualObjectToolDialog::onOkClicked()
{
    accept();
}

void VirtualObjectToolDialog::onCancelClicked()
{
    reject();
}

void VirtualObjectToolDialog::onApplyClicked()
{
    // TODO: 应用设置
    emit parametersChanged();
}

} // namespace UI
} // namespace VisionForge
