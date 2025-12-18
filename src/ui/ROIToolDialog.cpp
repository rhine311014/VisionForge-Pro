/**
 * @file ROIToolDialog.cpp
 * @brief ROI区域工具参数编辑对话框实现
 * @author VisionForge Team
 * @date 2025-12-18
 */

#include "ui/ROIToolDialog.h"
#include "ui/HalconImageViewer.h"
#include "algorithm/ROITool.h"
#include "base/Logger.h"

#include <QFormLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QMessageBox>
#include <QHeaderView>

namespace VisionForge {
namespace UI {

ROIToolDialog::ROIToolDialog(Algorithm::ROITool* tool, QWidget* parent)
    : QDialog(parent)
    , tool_(tool)
    , currentImage_(nullptr)
    , imageViewer_(nullptr)
    , mainSplitter_(nullptr)
    , roiTypeCombo_(nullptr)
    , paramsStack_(nullptr)
    , rectParamsPage_(nullptr)
    , rectXSpin_(nullptr)
    , rectYSpin_(nullptr)
    , rectWidthSpin_(nullptr)
    , rectHeightSpin_(nullptr)
    , circleParamsPage_(nullptr)
    , circleCXSpin_(nullptr)
    , circleCYSpin_(nullptr)
    , circleRadiusSpin_(nullptr)
    , ellipseParamsPage_(nullptr)
    , ellipseCXSpin_(nullptr)
    , ellipseCYSpin_(nullptr)
    , ellipseRXSpin_(nullptr)
    , ellipseRYSpin_(nullptr)
    , ellipseAngleSpin_(nullptr)
    , polygonParamsPage_(nullptr)
    , polygonPointsTable_(nullptr)
    , addPolygonPointBtn_(nullptr)
    , removePolygonPointBtn_(nullptr)
    , rotatedRectParamsPage_(nullptr)
    , rotRectCXSpin_(nullptr)
    , rotRectCYSpin_(nullptr)
    , rotRectWidthSpin_(nullptr)
    , rotRectHeightSpin_(nullptr)
    , rotRectAngleSpin_(nullptr)
    , roiListTable_(nullptr)
    , addROIBtn_(nullptr)
    , removeROIBtn_(nullptr)
    , clearROIsBtn_(nullptr)
    , combineModeCombo_(nullptr)
    , invertMaskCheck_(nullptr)
    , cropOutputCheck_(nullptr)
    , previewBtn_(nullptr)
    , okBtn_(nullptr)
    , cancelBtn_(nullptr)
    , applyBtn_(nullptr)
{
    setWindowTitle("ROI区域工具设置");
    setMinimumSize(1000, 700);
    createUI();
    connectSignals();
    updateUI();
}

ROIToolDialog::~ROIToolDialog() = default;

void ROIToolDialog::setImage(const Base::ImageData::Ptr& image)
{
    currentImage_ = image;
    if (imageViewer_ && image) {
        imageViewer_->setImage(image);
    }
}

void ROIToolDialog::updateUI()
{
    if (!tool_) return;

    // 更新组合模式
    combineModeCombo_->setCurrentIndex(static_cast<int>(tool_->combineMode()));
    invertMaskCheck_->setChecked(tool_->invertMask());
    cropOutputCheck_->setChecked(tool_->cropOutput());

    // 更新ROI列表
    updateROIList();
}

void ROIToolDialog::showEvent(QShowEvent* event)
{
    QDialog::showEvent(event);
    updateUI();
}

void ROIToolDialog::createUI()
{
    auto* mainLayout = new QHBoxLayout(this);

    mainSplitter_ = new QSplitter(Qt::Horizontal, this);

    // 左侧面板 - 图像显示
    auto* leftWidget = new QWidget(mainSplitter_);
    createLeftPanel(leftWidget);

    // 右侧面板 - 参数设置
    auto* rightWidget = new QWidget(mainSplitter_);
    createRightPanel(rightWidget);

    mainSplitter_->addWidget(leftWidget);
    mainSplitter_->addWidget(rightWidget);
    mainSplitter_->setSizes({600, 400});

    mainLayout->addWidget(mainSplitter_);
}

void ROIToolDialog::createLeftPanel(QWidget* parent)
{
    auto* layout = new QVBoxLayout(parent);

    imageViewer_ = new HalconImageViewer(parent);
    imageViewer_->setMinimumSize(400, 400);
    layout->addWidget(imageViewer_);

    // 预览按钮
    previewBtn_ = new QPushButton("预览Mask", parent);
    layout->addWidget(previewBtn_);
}

void ROIToolDialog::createRightPanel(QWidget* parent)
{
    auto* layout = new QVBoxLayout(parent);

    // 使用滚动区域
    auto* scrollArea = new QScrollArea(parent);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);

    auto* scrollWidget = new QWidget();
    auto* scrollLayout = new QVBoxLayout(scrollWidget);

    createROITypeGroup(scrollLayout);
    createROIParamsGroup(scrollLayout);
    createROIListGroup(scrollLayout);
    createCombineGroup(scrollLayout);

    scrollLayout->addStretch();

    scrollArea->setWidget(scrollWidget);
    layout->addWidget(scrollArea);

    // 对话框按钮
    auto* buttonLayout = new QHBoxLayout();
    okBtn_ = new QPushButton("确定", parent);
    cancelBtn_ = new QPushButton("取消", parent);
    applyBtn_ = new QPushButton("应用", parent);

    buttonLayout->addStretch();
    buttonLayout->addWidget(okBtn_);
    buttonLayout->addWidget(cancelBtn_);
    buttonLayout->addWidget(applyBtn_);

    layout->addLayout(buttonLayout);
}

void ROIToolDialog::createROITypeGroup(QVBoxLayout* layout)
{
    auto* group = new QGroupBox("ROI类型", layout->parentWidget());
    auto* groupLayout = new QFormLayout(group);

    roiTypeCombo_ = new QComboBox(group);
    roiTypeCombo_->addItem("矩形", 0);
    roiTypeCombo_->addItem("圆形", 1);
    roiTypeCombo_->addItem("椭圆", 2);
    roiTypeCombo_->addItem("多边形", 3);
    roiTypeCombo_->addItem("旋转矩形", 4);

    groupLayout->addRow("类型:", roiTypeCombo_);

    layout->addWidget(group);
}

void ROIToolDialog::createROIParamsGroup(QVBoxLayout* layout)
{
    auto* group = new QGroupBox("ROI参数", layout->parentWidget());
    auto* groupLayout = new QVBoxLayout(group);

    paramsStack_ = new QStackedWidget(group);

    createRectParamsPage();
    createCircleParamsPage();
    createEllipseParamsPage();
    createPolygonParamsPage();
    createRotatedRectParamsPage();

    paramsStack_->addWidget(rectParamsPage_);
    paramsStack_->addWidget(circleParamsPage_);
    paramsStack_->addWidget(ellipseParamsPage_);
    paramsStack_->addWidget(polygonParamsPage_);
    paramsStack_->addWidget(rotatedRectParamsPage_);

    groupLayout->addWidget(paramsStack_);

    // 添加ROI按钮
    addROIBtn_ = new QPushButton("添加ROI", group);
    groupLayout->addWidget(addROIBtn_);

    layout->addWidget(group);
}

void ROIToolDialog::createRectParamsPage()
{
    rectParamsPage_ = new QWidget();
    auto* layout = new QFormLayout(rectParamsPage_);

    rectXSpin_ = new QDoubleSpinBox(rectParamsPage_);
    rectXSpin_->setRange(0, 10000);
    rectXSpin_->setValue(0);

    rectYSpin_ = new QDoubleSpinBox(rectParamsPage_);
    rectYSpin_->setRange(0, 10000);
    rectYSpin_->setValue(0);

    rectWidthSpin_ = new QDoubleSpinBox(rectParamsPage_);
    rectWidthSpin_->setRange(1, 10000);
    rectWidthSpin_->setValue(100);

    rectHeightSpin_ = new QDoubleSpinBox(rectParamsPage_);
    rectHeightSpin_->setRange(1, 10000);
    rectHeightSpin_->setValue(100);

    layout->addRow("X:", rectXSpin_);
    layout->addRow("Y:", rectYSpin_);
    layout->addRow("宽度:", rectWidthSpin_);
    layout->addRow("高度:", rectHeightSpin_);
}

void ROIToolDialog::createCircleParamsPage()
{
    circleParamsPage_ = new QWidget();
    auto* layout = new QFormLayout(circleParamsPage_);

    circleCXSpin_ = new QDoubleSpinBox(circleParamsPage_);
    circleCXSpin_->setRange(0, 10000);
    circleCXSpin_->setValue(100);

    circleCYSpin_ = new QDoubleSpinBox(circleParamsPage_);
    circleCYSpin_->setRange(0, 10000);
    circleCYSpin_->setValue(100);

    circleRadiusSpin_ = new QDoubleSpinBox(circleParamsPage_);
    circleRadiusSpin_->setRange(1, 5000);
    circleRadiusSpin_->setValue(50);

    layout->addRow("圆心X:", circleCXSpin_);
    layout->addRow("圆心Y:", circleCYSpin_);
    layout->addRow("半径:", circleRadiusSpin_);
}

void ROIToolDialog::createEllipseParamsPage()
{
    ellipseParamsPage_ = new QWidget();
    auto* layout = new QFormLayout(ellipseParamsPage_);

    ellipseCXSpin_ = new QDoubleSpinBox(ellipseParamsPage_);
    ellipseCXSpin_->setRange(0, 10000);
    ellipseCXSpin_->setValue(100);

    ellipseCYSpin_ = new QDoubleSpinBox(ellipseParamsPage_);
    ellipseCYSpin_->setRange(0, 10000);
    ellipseCYSpin_->setValue(100);

    ellipseRXSpin_ = new QDoubleSpinBox(ellipseParamsPage_);
    ellipseRXSpin_->setRange(1, 5000);
    ellipseRXSpin_->setValue(60);

    ellipseRYSpin_ = new QDoubleSpinBox(ellipseParamsPage_);
    ellipseRYSpin_->setRange(1, 5000);
    ellipseRYSpin_->setValue(40);

    ellipseAngleSpin_ = new QDoubleSpinBox(ellipseParamsPage_);
    ellipseAngleSpin_->setRange(-180, 180);
    ellipseAngleSpin_->setValue(0);
    ellipseAngleSpin_->setSuffix("°");

    layout->addRow("中心X:", ellipseCXSpin_);
    layout->addRow("中心Y:", ellipseCYSpin_);
    layout->addRow("半长轴:", ellipseRXSpin_);
    layout->addRow("半短轴:", ellipseRYSpin_);
    layout->addRow("旋转角度:", ellipseAngleSpin_);
}

void ROIToolDialog::createPolygonParamsPage()
{
    polygonParamsPage_ = new QWidget();
    auto* layout = new QVBoxLayout(polygonParamsPage_);

    polygonPointsTable_ = new QTableWidget(polygonParamsPage_);
    polygonPointsTable_->setColumnCount(2);
    polygonPointsTable_->setHorizontalHeaderLabels({"X", "Y"});
    polygonPointsTable_->horizontalHeader()->setStretchLastSection(true);
    polygonPointsTable_->setSelectionBehavior(QAbstractItemView::SelectRows);
    layout->addWidget(polygonPointsTable_);

    auto* btnLayout = new QHBoxLayout();
    addPolygonPointBtn_ = new QPushButton("添加点", polygonParamsPage_);
    removePolygonPointBtn_ = new QPushButton("删除点", polygonParamsPage_);
    btnLayout->addWidget(addPolygonPointBtn_);
    btnLayout->addWidget(removePolygonPointBtn_);
    btnLayout->addStretch();
    layout->addLayout(btnLayout);
}

void ROIToolDialog::createRotatedRectParamsPage()
{
    rotatedRectParamsPage_ = new QWidget();
    auto* layout = new QFormLayout(rotatedRectParamsPage_);

    rotRectCXSpin_ = new QDoubleSpinBox(rotatedRectParamsPage_);
    rotRectCXSpin_->setRange(0, 10000);
    rotRectCXSpin_->setValue(100);

    rotRectCYSpin_ = new QDoubleSpinBox(rotatedRectParamsPage_);
    rotRectCYSpin_->setRange(0, 10000);
    rotRectCYSpin_->setValue(100);

    rotRectWidthSpin_ = new QDoubleSpinBox(rotatedRectParamsPage_);
    rotRectWidthSpin_->setRange(1, 10000);
    rotRectWidthSpin_->setValue(100);

    rotRectHeightSpin_ = new QDoubleSpinBox(rotatedRectParamsPage_);
    rotRectHeightSpin_->setRange(1, 10000);
    rotRectHeightSpin_->setValue(50);

    rotRectAngleSpin_ = new QDoubleSpinBox(rotatedRectParamsPage_);
    rotRectAngleSpin_->setRange(-180, 180);
    rotRectAngleSpin_->setValue(0);
    rotRectAngleSpin_->setSuffix("°");

    layout->addRow("中心X:", rotRectCXSpin_);
    layout->addRow("中心Y:", rotRectCYSpin_);
    layout->addRow("宽度:", rotRectWidthSpin_);
    layout->addRow("高度:", rotRectHeightSpin_);
    layout->addRow("旋转角度:", rotRectAngleSpin_);
}

void ROIToolDialog::createROIListGroup(QVBoxLayout* layout)
{
    auto* group = new QGroupBox("ROI列表", layout->parentWidget());
    auto* groupLayout = new QVBoxLayout(group);

    roiListTable_ = new QTableWidget(group);
    roiListTable_->setColumnCount(4);
    roiListTable_->setHorizontalHeaderLabels({"名称", "类型", "启用", "操作"});
    roiListTable_->horizontalHeader()->setStretchLastSection(true);
    roiListTable_->setSelectionBehavior(QAbstractItemView::SelectRows);
    roiListTable_->setSelectionMode(QAbstractItemView::SingleSelection);
    roiListTable_->setMaximumHeight(150);
    groupLayout->addWidget(roiListTable_);

    auto* btnLayout = new QHBoxLayout();
    removeROIBtn_ = new QPushButton("删除选中", group);
    clearROIsBtn_ = new QPushButton("清空全部", group);
    btnLayout->addWidget(removeROIBtn_);
    btnLayout->addWidget(clearROIsBtn_);
    btnLayout->addStretch();
    groupLayout->addLayout(btnLayout);

    layout->addWidget(group);
}

void ROIToolDialog::createCombineGroup(QVBoxLayout* layout)
{
    auto* group = new QGroupBox("组合设置", layout->parentWidget());
    auto* groupLayout = new QFormLayout(group);

    combineModeCombo_ = new QComboBox(group);
    combineModeCombo_->addItem("并集 (Union)", 0);
    combineModeCombo_->addItem("交集 (Intersection)", 1);
    combineModeCombo_->addItem("异或 (XOR)", 2);

    invertMaskCheck_ = new QCheckBox("反转Mask", group);
    cropOutputCheck_ = new QCheckBox("裁剪输出图像", group);

    groupLayout->addRow("组合模式:", combineModeCombo_);
    groupLayout->addRow("", invertMaskCheck_);
    groupLayout->addRow("", cropOutputCheck_);

    layout->addWidget(group);
}

void ROIToolDialog::connectSignals()
{
    // ROI类型选择
    connect(roiTypeCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ROIToolDialog::onROITypeChanged);

    // ROI操作
    connect(addROIBtn_, &QPushButton::clicked, this, &ROIToolDialog::onAddROIClicked);
    connect(removeROIBtn_, &QPushButton::clicked, this, &ROIToolDialog::onRemoveROIClicked);
    connect(clearROIsBtn_, &QPushButton::clicked, this, &ROIToolDialog::onClearROIsClicked);
    connect(roiListTable_, &QTableWidget::cellClicked, this, &ROIToolDialog::onROISelected);

    // 组合模式
    connect(combineModeCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ROIToolDialog::onCombineModeChanged);
    connect(invertMaskCheck_, &QCheckBox::toggled, this, &ROIToolDialog::onInvertMaskChanged);
    connect(cropOutputCheck_, &QCheckBox::toggled, this, &ROIToolDialog::onCropOutputChanged);

    // 预览
    connect(previewBtn_, &QPushButton::clicked, this, &ROIToolDialog::onPreviewClicked);

    // 对话框按钮
    connect(okBtn_, &QPushButton::clicked, this, &ROIToolDialog::onOkClicked);
    connect(cancelBtn_, &QPushButton::clicked, this, &ROIToolDialog::onCancelClicked);
    connect(applyBtn_, &QPushButton::clicked, this, &ROIToolDialog::onApplyClicked);
}

void ROIToolDialog::onROITypeChanged(int index)
{
    paramsStack_->setCurrentIndex(index);
}

void ROIToolDialog::onAddROIClicked()
{
    if (!tool_) return;

    int type = roiTypeCombo_->currentIndex();
    QString name = QString("ROI_%1").arg(tool_->roiCollection().count() + 1);

    switch (type) {
    case 0: // 矩形
        tool_->addRectROI(QRectF(rectXSpin_->value(), rectYSpin_->value(),
                                  rectWidthSpin_->value(), rectHeightSpin_->value()), name);
        break;
    case 1: // 圆形
        tool_->addCircleROI(QPointF(circleCXSpin_->value(), circleCYSpin_->value()),
                            circleRadiusSpin_->value(), name);
        break;
    case 2: // 椭圆
        tool_->addEllipseROI(QPointF(ellipseCXSpin_->value(), ellipseCYSpin_->value()),
                             ellipseRXSpin_->value(), ellipseRYSpin_->value(),
                             ellipseAngleSpin_->value(), name);
        break;
    case 3: // 多边形
        {
            QPolygonF polygon;
            for (int i = 0; i < polygonPointsTable_->rowCount(); ++i) {
                double x = polygonPointsTable_->item(i, 0)->text().toDouble();
                double y = polygonPointsTable_->item(i, 1)->text().toDouble();
                polygon.append(QPointF(x, y));
            }
            if (polygon.size() >= 3) {
                tool_->addPolygonROI(polygon, name);
            } else {
                QMessageBox::warning(this, "警告", "多边形至少需要3个点");
                return;
            }
        }
        break;
    case 4: // 旋转矩形
        tool_->addRotatedRectROI(QPointF(rotRectCXSpin_->value(), rotRectCYSpin_->value()),
                                  rotRectWidthSpin_->value(), rotRectHeightSpin_->value(),
                                  rotRectAngleSpin_->value(), name);
        break;
    }

    updateROIList();
    emit parameterChanged();
}

void ROIToolDialog::onRemoveROIClicked()
{
    if (!tool_) return;

    int row = roiListTable_->currentRow();
    if (row >= 0) {
        tool_->removeROI(row);
        updateROIList();
        emit parameterChanged();
    }
}

void ROIToolDialog::onClearROIsClicked()
{
    if (!tool_) return;

    if (QMessageBox::question(this, "确认", "确定要清空所有ROI吗?") == QMessageBox::Yes) {
        tool_->clearROIs();
        updateROIList();
        emit parameterChanged();
    }
}

void ROIToolDialog::onROISelected(int row)
{
    Q_UNUSED(row);
    updateParamsFromSelection();
}

void ROIToolDialog::onRectParamsChanged()
{
    // 实时更新选中的ROI参数
}

void ROIToolDialog::onCircleParamsChanged()
{
    // 实时更新选中的ROI参数
}

void ROIToolDialog::onEllipseParamsChanged()
{
    // 实时更新选中的ROI参数
}

void ROIToolDialog::onRotatedRectParamsChanged()
{
    // 实时更新选中的ROI参数
}

void ROIToolDialog::onCombineModeChanged(int index)
{
    if (!tool_) return;
    tool_->setCombineMode(static_cast<Algorithm::ROITool::CombineMode>(index));
    emit parameterChanged();
}

void ROIToolDialog::onInvertMaskChanged(bool checked)
{
    if (!tool_) return;
    tool_->setInvertMask(checked);
    emit parameterChanged();
}

void ROIToolDialog::onCropOutputChanged(bool checked)
{
    if (!tool_) return;
    tool_->setCropOutput(checked);
    emit parameterChanged();
}

void ROIToolDialog::onPreviewClicked()
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
        LOG_INFO(QString("ROI处理完成，共 %1 个ROI").arg(tool_->roiCollection().count()));
    } else {
        QMessageBox::warning(this, "处理失败", result.errorMessage);
    }
}

void ROIToolDialog::onOkClicked()
{
    applyParameters();
    accept();
}

void ROIToolDialog::onCancelClicked()
{
    reject();
}

void ROIToolDialog::onApplyClicked()
{
    applyParameters();
    emit parameterChanged();
}

void ROIToolDialog::applyParameters()
{
    if (!tool_) return;

    tool_->setCombineMode(static_cast<Algorithm::ROITool::CombineMode>(combineModeCombo_->currentIndex()));
    tool_->setInvertMask(invertMaskCheck_->isChecked());
    tool_->setCropOutput(cropOutputCheck_->isChecked());
}

void ROIToolDialog::updateROIList()
{
    if (!tool_) return;

    roiListTable_->setRowCount(0);
    const auto& rois = tool_->roiCollection().getAllROIs();

    for (size_t i = 0; i < rois.size(); ++i) {
        const auto& roi = rois[i];
        int row = roiListTable_->rowCount();
        roiListTable_->insertRow(row);

        // 名称
        roiListTable_->setItem(row, 0, new QTableWidgetItem(roi->name()));

        // 类型
        QString typeStr;
        switch (roi->type()) {
        case Algorithm::ROIShape::Rectangle: typeStr = "矩形"; break;
        case Algorithm::ROIShape::Circle: typeStr = "圆形"; break;
        case Algorithm::ROIShape::Ellipse: typeStr = "椭圆"; break;
        case Algorithm::ROIShape::Polygon: typeStr = "多边形"; break;
        case Algorithm::ROIShape::RotatedRect: typeStr = "旋转矩形"; break;
        }
        roiListTable_->setItem(row, 1, new QTableWidgetItem(typeStr));

        // 启用状态
        auto* enabledItem = new QTableWidgetItem();
        enabledItem->setCheckState(roi->isEnabled() ? Qt::Checked : Qt::Unchecked);
        roiListTable_->setItem(row, 2, enabledItem);

        // 操作按钮可以后续添加
        roiListTable_->setItem(row, 3, new QTableWidgetItem(""));
    }
}

void ROIToolDialog::updateParamsFromSelection()
{
    if (!tool_) return;

    int row = roiListTable_->currentRow();
    if (row < 0 || row >= tool_->roiCollection().count()) return;

    auto roi = tool_->roiCollection().getROI(row);
    if (!roi) return;

    // 根据ROI类型更新参数页面
    int typeIndex = static_cast<int>(roi->type());
    roiTypeCombo_->setCurrentIndex(typeIndex);
    paramsStack_->setCurrentIndex(typeIndex);

    switch (roi->type()) {
    case Algorithm::ROIShape::Rectangle:
        if (auto* rectROI = dynamic_cast<Algorithm::RectROI*>(roi.get())) {
            rectXSpin_->setValue(rectROI->rect().x());
            rectYSpin_->setValue(rectROI->rect().y());
            rectWidthSpin_->setValue(rectROI->rect().width());
            rectHeightSpin_->setValue(rectROI->rect().height());
        }
        break;
    case Algorithm::ROIShape::Circle:
        if (auto* circleROI = dynamic_cast<Algorithm::CircleROI*>(roi.get())) {
            circleCXSpin_->setValue(circleROI->center().x());
            circleCYSpin_->setValue(circleROI->center().y());
            circleRadiusSpin_->setValue(circleROI->radius());
        }
        break;
    case Algorithm::ROIShape::Ellipse:
        if (auto* ellipseROI = dynamic_cast<Algorithm::EllipseROI*>(roi.get())) {
            ellipseCXSpin_->setValue(ellipseROI->center().x());
            ellipseCYSpin_->setValue(ellipseROI->center().y());
            ellipseRXSpin_->setValue(ellipseROI->radiusX());
            ellipseRYSpin_->setValue(ellipseROI->radiusY());
            ellipseAngleSpin_->setValue(ellipseROI->angle());
        }
        break;
    case Algorithm::ROIShape::RotatedRect:
        if (auto* rotRectROI = dynamic_cast<Algorithm::RotatedRectROI*>(roi.get())) {
            rotRectCXSpin_->setValue(rotRectROI->center().x());
            rotRectCYSpin_->setValue(rotRectROI->center().y());
            rotRectWidthSpin_->setValue(rotRectROI->width());
            rotRectHeightSpin_->setValue(rotRectROI->height());
            rotRectAngleSpin_->setValue(rotRectROI->angle());
        }
        break;
    default:
        break;
    }
}

} // namespace UI
} // namespace VisionForge
