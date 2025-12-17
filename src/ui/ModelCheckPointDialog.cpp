/**
 * @file ModelCheckPointDialog.cpp
 * @brief 模型校验点编辑对话框实现
 */

#include "ui/ModelCheckPointDialog.h"
#include "ui/HalconImageViewer.h"
#include "algorithm/ShapeMatchTool.h"
#include "base/Logger.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QSplitter>
#include <cmath>

#ifdef _WIN32
#include "HalconCpp.h"
using namespace HalconCpp;
#endif

namespace VisionForge {
namespace UI {

ModelCheckPointDialog::ModelCheckPointDialog(Algorithm::ShapeMatchTool* tool, QWidget* parent)
    : QDialog(parent)
    , tool_(tool)
    , matchRow_(0)
    , matchCol_(0)
    , matchAngle_(0)
    , hasMatchResult_(false)
    , pickingMode_(false)
    , lastMouseRow_(0)
    , lastMouseCol_(0)
{
    setWindowTitle("模型校验点设置");
    setMinimumSize(900, 600);
    resize(1000, 700);

    createUI();
    connectSignals();
    loadCheckPoints();
    updateTable();
}

ModelCheckPointDialog::~ModelCheckPointDialog() = default;

void ModelCheckPointDialog::setImage(const Base::ImageData::Ptr& image)
{
    currentImage_ = image;
    if (imageViewer_ && image) {
        imageViewer_->setImage(image);
        updateVisualization();
    }
}

void ModelCheckPointDialog::setMatchResult(double row, double col, double angle)
{
    matchRow_ = row;
    matchCol_ = col;
    matchAngle_ = angle;
    hasMatchResult_ = true;
    updateVisualization();
}

void ModelCheckPointDialog::createUI()
{
    QHBoxLayout* mainLayout = new QHBoxLayout(this);

    // 左侧图像面板
    QWidget* leftPanel = new QWidget;
    QVBoxLayout* leftLayout = new QVBoxLayout(leftPanel);
    leftLayout->setContentsMargins(0, 0, 0, 0);
    createImagePanel(leftLayout);

    // 右侧设置面板
    QWidget* rightPanel = new QWidget;
    rightPanel->setFixedWidth(400);
    QVBoxLayout* rightLayout = new QVBoxLayout(rightPanel);

    createCheckPointTable(rightLayout);
    createEditPanel(rightLayout);
    createButtonPanel(rightLayout);

    // 分割器
    QSplitter* splitter = new QSplitter(Qt::Horizontal);
    splitter->addWidget(leftPanel);
    splitter->addWidget(rightPanel);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 0);

    mainLayout->addWidget(splitter);
}

void ModelCheckPointDialog::createImagePanel(QVBoxLayout* layout)
{
    QGroupBox* imageGroup = new QGroupBox("图像预览");
    QVBoxLayout* imageLayout = new QVBoxLayout(imageGroup);

    imageViewer_ = new HalconImageViewer;
    imageViewer_->setMinimumSize(400, 300);
    imageLayout->addWidget(imageViewer_);

    // 拾取按钮
    QHBoxLayout* pickLayout = new QHBoxLayout;
    pickFromImageBtn_ = new QPushButton("从图像拾取校验点");
    pickFromImageBtn_->setCheckable(true);
    pickLayout->addWidget(pickFromImageBtn_);
    pickLayout->addStretch();
    imageLayout->addLayout(pickLayout);

    layout->addWidget(imageGroup);
}

void ModelCheckPointDialog::createCheckPointTable(QVBoxLayout* layout)
{
    QGroupBox* tableGroup = new QGroupBox("校验点列表");
    QVBoxLayout* tableLayout = new QVBoxLayout(tableGroup);

    // 启用复选框
    enableCheckPointsCheckBox_ = new QCheckBox("启用校验点验证");
    enableCheckPointsCheckBox_->setChecked(tool_->isCheckPointsEnabled());
    tableLayout->addWidget(enableCheckPointsCheckBox_);

    // 表格
    checkPointTable_ = new QTableWidget;
    checkPointTable_->setColumnCount(6);
    checkPointTable_->setHorizontalHeaderLabels(
        {"启用", "名称", "行偏移", "列偏移", "灰度值", "容差"});
    checkPointTable_->horizontalHeader()->setStretchLastSection(true);
    checkPointTable_->setSelectionBehavior(QAbstractItemView::SelectRows);
    checkPointTable_->setSelectionMode(QAbstractItemView::SingleSelection);
    checkPointTable_->verticalHeader()->setVisible(false);
    tableLayout->addWidget(checkPointTable_);

    // 操作按钮
    QHBoxLayout* btnLayout = new QHBoxLayout;
    addBtn_ = new QPushButton("添加");
    removeBtn_ = new QPushButton("删除");
    clearAllBtn_ = new QPushButton("清除全部");
    btnLayout->addWidget(addBtn_);
    btnLayout->addWidget(removeBtn_);
    btnLayout->addWidget(clearAllBtn_);
    btnLayout->addStretch();
    tableLayout->addLayout(btnLayout);

    layout->addWidget(tableGroup);
}

void ModelCheckPointDialog::createEditPanel(QVBoxLayout* layout)
{
    QGroupBox* editGroup = new QGroupBox("校验点编辑");
    QVBoxLayout* editLayout = new QVBoxLayout(editGroup);

    // 名称
    QHBoxLayout* nameLayout = new QHBoxLayout;
    nameLayout->addWidget(new QLabel("名称:"));
    nameEdit_ = new QLineEdit;
    nameEdit_->setPlaceholderText("校验点名称（可选）");
    nameLayout->addWidget(nameEdit_);
    editLayout->addLayout(nameLayout);

    // 相对位置
    QHBoxLayout* posLayout = new QHBoxLayout;
    posLayout->addWidget(new QLabel("行偏移:"));
    relRowSpinBox_ = new QDoubleSpinBox;
    relRowSpinBox_->setRange(-10000, 10000);
    relRowSpinBox_->setDecimals(1);
    posLayout->addWidget(relRowSpinBox_);
    posLayout->addWidget(new QLabel("列偏移:"));
    relColSpinBox_ = new QDoubleSpinBox;
    relColSpinBox_->setRange(-10000, 10000);
    relColSpinBox_->setDecimals(1);
    posLayout->addWidget(relColSpinBox_);
    editLayout->addLayout(posLayout);

    // 灰度值
    QHBoxLayout* grayLayout = new QHBoxLayout;
    grayLayout->addWidget(new QLabel("期望灰度:"));
    expectedGraySpinBox_ = new QSpinBox;
    expectedGraySpinBox_->setRange(0, 255);
    expectedGraySpinBox_->setValue(128);
    grayLayout->addWidget(expectedGraySpinBox_);
    grayLayout->addWidget(new QLabel("容差:"));
    toleranceSpinBox_ = new QSpinBox;
    toleranceSpinBox_->setRange(0, 255);
    toleranceSpinBox_->setValue(30);
    grayLayout->addWidget(toleranceSpinBox_);
    editLayout->addLayout(grayLayout);

    // 启用
    pointEnabledCheckBox_ = new QCheckBox("启用此校验点");
    pointEnabledCheckBox_->setChecked(true);
    editLayout->addWidget(pointEnabledCheckBox_);

    // 测试按钮
    testValidateBtn_ = new QPushButton("测试校验");
    editLayout->addWidget(testValidateBtn_);

    layout->addWidget(editGroup);
}

void ModelCheckPointDialog::createButtonPanel(QVBoxLayout* layout)
{
    layout->addStretch();

    QHBoxLayout* btnLayout = new QHBoxLayout;
    okBtn_ = new QPushButton("确定");
    cancelBtn_ = new QPushButton("取消");
    applyBtn_ = new QPushButton("应用");
    btnLayout->addStretch();
    btnLayout->addWidget(okBtn_);
    btnLayout->addWidget(cancelBtn_);
    btnLayout->addWidget(applyBtn_);

    layout->addLayout(btnLayout);
}

void ModelCheckPointDialog::connectSignals()
{
    connect(addBtn_, &QPushButton::clicked, this, &ModelCheckPointDialog::onAddCheckPointClicked);
    connect(removeBtn_, &QPushButton::clicked, this, &ModelCheckPointDialog::onRemoveCheckPointClicked);
    connect(clearAllBtn_, &QPushButton::clicked, this, &ModelCheckPointDialog::onClearAllClicked);
    connect(pickFromImageBtn_, &QPushButton::toggled, this, [this](bool checked) {
        pickingMode_ = checked;
        if (checked) {
            imageViewer_->setCursor(Qt::CrossCursor);
            // 启用绘制小矩形模式来拾取点
            imageViewer_->setInteractionMode(HalconImageViewer::DrawRectangle);
        } else {
            imageViewer_->setCursor(Qt::ArrowCursor);
            imageViewer_->setInteractionMode(HalconImageViewer::SelectMode);
        }
    });
    connect(testValidateBtn_, &QPushButton::clicked, this, &ModelCheckPointDialog::onTestValidateClicked);
    connect(enableCheckPointsCheckBox_, &QCheckBox::toggled, this, &ModelCheckPointDialog::onEnableCheckPointsChanged);

    connect(checkPointTable_, &QTableWidget::itemSelectionChanged, this, &ModelCheckPointDialog::onTableSelectionChanged);
    connect(checkPointTable_, &QTableWidget::cellChanged, this, &ModelCheckPointDialog::onTableCellChanged);

    // 使用mousePositionChanged信号获取鼠标位置
    connect(imageViewer_, &HalconImageViewer::mousePositionChanged, this, [this](int x, int y, bool valid) {
        if (pickingMode_ && valid) {
            lastMouseRow_ = y;
            lastMouseCol_ = x;
        }
    });

    // 使用ROI创建信号来实现点击拾取（在拾取模式下创建一个点ROI）
    connect(imageViewer_, &HalconImageViewer::roiCreated, this, [this](ROIShapePtr roi) {
        if (pickingMode_ && roi) {
            // 获取ROI中心点作为拾取位置
            QRect rect = roi->boundingRect();
            double row = rect.center().y();
            double col = rect.center().x();
            onImageClicked(row, col);
            // 清除临时ROI
            imageViewer_->clearROIs();
        }
    });

    connect(okBtn_, &QPushButton::clicked, this, &ModelCheckPointDialog::onOkClicked);
    connect(cancelBtn_, &QPushButton::clicked, this, &ModelCheckPointDialog::onCancelClicked);
    connect(applyBtn_, &QPushButton::clicked, this, &ModelCheckPointDialog::onApplyClicked);
}

void ModelCheckPointDialog::loadCheckPoints()
{
    checkPoints_.clear();
    const auto& points = tool_->getCheckPoints();
    for (const auto& cp : points) {
        checkPoints_.push_back(cp);
    }
}

void ModelCheckPointDialog::updateTable()
{
    checkPointTable_->blockSignals(true);
    checkPointTable_->setRowCount(static_cast<int>(checkPoints_.size()));

    for (size_t i = 0; i < checkPoints_.size(); i++) {
        const auto& cp = checkPoints_[i];
        int row = static_cast<int>(i);

        // 启用复选框
        QTableWidgetItem* enableItem = new QTableWidgetItem;
        enableItem->setCheckState(cp.enabled ? Qt::Checked : Qt::Unchecked);
        checkPointTable_->setItem(row, 0, enableItem);

        // 名称
        checkPointTable_->setItem(row, 1, new QTableWidgetItem(cp.name));

        // 行偏移
        checkPointTable_->setItem(row, 2, new QTableWidgetItem(QString::number(cp.relRow, 'f', 1)));

        // 列偏移
        checkPointTable_->setItem(row, 3, new QTableWidgetItem(QString::number(cp.relCol, 'f', 1)));

        // 灰度值
        checkPointTable_->setItem(row, 4, new QTableWidgetItem(QString::number(cp.expectedGray)));

        // 容差
        checkPointTable_->setItem(row, 5, new QTableWidgetItem(QString::number(cp.tolerance)));
    }

    checkPointTable_->blockSignals(false);
    checkPointTable_->resizeColumnsToContents();
    updateVisualization();
}

void ModelCheckPointDialog::updateVisualization()
{
#ifdef _WIN32
    if (!currentImage_ || !imageViewer_) return;

    QList<HXLDCont> contours;
    QStringList colors;

    double cosA = hasMatchResult_ ? std::cos(matchAngle_) : 1.0;
    double sinA = hasMatchResult_ ? std::sin(matchAngle_) : 0.0;
    double baseRow = hasMatchResult_ ? matchRow_ : currentImage_->height() / 2.0;
    double baseCol = hasMatchResult_ ? matchCol_ : currentImage_->width() / 2.0;

    // 为每个校验点创建标记
    for (size_t i = 0; i < checkPoints_.size(); i++) {
        const auto& cp = checkPoints_[i];

        // 计算绝对位置
        double absRow = baseRow + cp.relRow * cosA - cp.relCol * sinA;
        double absCol = baseCol + cp.relRow * sinA + cp.relCol * cosA;

        // 创建十字标记
        HXLDCont crossH, crossV, crossCont;
        try {
            GenContourPolygonXld(&crossH,
                HTuple(absRow).TupleConcat(absRow),
                HTuple(absCol - 10).TupleConcat(absCol + 10));
            GenContourPolygonXld(&crossV,
                HTuple(absRow - 10).TupleConcat(absRow + 10),
                HTuple(absCol).TupleConcat(absCol));
            crossCont = crossH.ConcatObj(crossV);

            contours.append(crossCont);
            colors.append(cp.enabled ? "green" : "gray");
        }
        catch (const HException& e) {
            LOG_ERROR(QString("创建校验点标记失败: %1").arg(e.ErrorMessage().Text()));
        }
    }

    // 显示匹配位置中心点
    if (hasMatchResult_) {
        try {
            HXLDCont centerH, centerV, centerCont;
            GenContourPolygonXld(&centerH,
                HTuple(matchRow_).TupleConcat(matchRow_),
                HTuple(matchCol_ - 20).TupleConcat(matchCol_ + 20));
            GenContourPolygonXld(&centerV,
                HTuple(matchRow_ - 20).TupleConcat(matchRow_ + 20),
                HTuple(matchCol_).TupleConcat(matchCol_));
            centerCont = centerH.ConcatObj(centerV);
            contours.append(centerCont);
            colors.append("cyan");
        }
        catch (const HException&) {}
    }

    imageViewer_->setXLDContours(contours, colors);
#endif
}

void ModelCheckPointDialog::onAddCheckPointClicked()
{
    Algorithm::ModelCheckPoint cp;
    cp.name = nameEdit_->text();
    cp.relRow = relRowSpinBox_->value();
    cp.relCol = relColSpinBox_->value();
    cp.expectedGray = expectedGraySpinBox_->value();
    cp.tolerance = toleranceSpinBox_->value();
    cp.enabled = pointEnabledCheckBox_->isChecked();

    checkPoints_.push_back(cp);
    updateTable();

    // 清空编辑面板
    nameEdit_->clear();
    relRowSpinBox_->setValue(0);
    relColSpinBox_->setValue(0);
}

void ModelCheckPointDialog::onRemoveCheckPointClicked()
{
    int row = checkPointTable_->currentRow();
    if (row >= 0 && row < static_cast<int>(checkPoints_.size())) {
        checkPoints_.erase(checkPoints_.begin() + row);
        updateTable();
    }
}

void ModelCheckPointDialog::onClearAllClicked()
{
    if (QMessageBox::question(this, "确认", "确定要清除所有校验点吗？") == QMessageBox::Yes) {
        checkPoints_.clear();
        updateTable();
    }
}

void ModelCheckPointDialog::onPickFromImageClicked()
{
    // 由按钮的toggled信号处理
}

void ModelCheckPointDialog::onTestValidateClicked()
{
    if (!currentImage_) {
        QMessageBox::warning(this, "警告", "请先加载图像");
        return;
    }

    if (!hasMatchResult_) {
        QMessageBox::warning(this, "警告", "没有匹配结果，无法测试校验点");
        return;
    }

    // 临时应用校验点到工具进行测试
    auto originalPoints = tool_->getCheckPoints();
    bool originalEnabled = tool_->isCheckPointsEnabled();

    tool_->setCheckPoints(checkPoints_);
    tool_->setCheckPointsEnabled(enableCheckPointsCheckBox_->isChecked());

    std::vector<int> failedPoints;
    bool allPassed = tool_->validateCheckPoints(currentImage_, matchRow_, matchCol_, matchAngle_, &failedPoints);

    // 恢复原始状态
    tool_->setCheckPoints(originalPoints);
    tool_->setCheckPointsEnabled(originalEnabled);

    // 显示结果
    if (allPassed) {
        QMessageBox::information(this, "测试结果", "所有校验点验证通过！");
    } else {
        QString failedList;
        for (int idx : failedPoints) {
            if (idx >= 0 && idx < static_cast<int>(checkPoints_.size())) {
                failedList += QString("\n  - %1: %2")
                    .arg(idx)
                    .arg(checkPoints_[idx].name.isEmpty() ? QString("校验点%1").arg(idx) : checkPoints_[idx].name);
            }
        }
        QMessageBox::warning(this, "测试结果",
            QString("校验点验证失败！\n失败的校验点:%1").arg(failedList));
    }
}

void ModelCheckPointDialog::onEnableCheckPointsChanged(bool enabled)
{
    Q_UNUSED(enabled)
    updateVisualization();
}

void ModelCheckPointDialog::onTableSelectionChanged()
{
    int row = checkPointTable_->currentRow();
    if (row >= 0 && row < static_cast<int>(checkPoints_.size())) {
        const auto& cp = checkPoints_[row];
        nameEdit_->setText(cp.name);
        relRowSpinBox_->setValue(cp.relRow);
        relColSpinBox_->setValue(cp.relCol);
        expectedGraySpinBox_->setValue(cp.expectedGray);
        toleranceSpinBox_->setValue(cp.tolerance);
        pointEnabledCheckBox_->setChecked(cp.enabled);
    }
}

void ModelCheckPointDialog::onTableCellChanged(int row, int column)
{
    if (row < 0 || row >= static_cast<int>(checkPoints_.size())) return;

    auto& cp = checkPoints_[row];
    QTableWidgetItem* item = checkPointTable_->item(row, column);
    if (!item) return;

    switch (column) {
    case 0:  // 启用
        cp.enabled = item->checkState() == Qt::Checked;
        break;
    case 1:  // 名称
        cp.name = item->text();
        break;
    case 2:  // 行偏移
        cp.relRow = item->text().toDouble();
        break;
    case 3:  // 列偏移
        cp.relCol = item->text().toDouble();
        break;
    case 4:  // 灰度值
        cp.expectedGray = item->text().toInt();
        break;
    case 5:  // 容差
        cp.tolerance = item->text().toInt();
        break;
    }

    updateVisualization();
}

void ModelCheckPointDialog::onImageClicked(double row, double col)
{
    if (!pickingMode_) return;
    if (!currentImage_) return;

    // 计算相对于匹配中心的偏移
    double baseRow = hasMatchResult_ ? matchRow_ : currentImage_->height() / 2.0;
    double baseCol = hasMatchResult_ ? matchCol_ : currentImage_->width() / 2.0;
    double cosA = hasMatchResult_ ? std::cos(matchAngle_) : 1.0;
    double sinA = hasMatchResult_ ? std::sin(matchAngle_) : 0.0;

    // 逆变换：从绝对坐标计算相对坐标
    double dx = col - baseCol;
    double dy = row - baseRow;
    double relRow = dy * cosA + dx * sinA;
    double relCol = -dy * sinA + dx * cosA;

    // 获取该点的灰度值
    int grayValue = 128;
    const cv::Mat& mat = currentImage_->mat();
    int pixelRow = static_cast<int>(std::round(row));
    int pixelCol = static_cast<int>(std::round(col));
    if (pixelRow >= 0 && pixelRow < mat.rows && pixelCol >= 0 && pixelCol < mat.cols) {
        if (mat.channels() == 1) {
            grayValue = mat.at<uchar>(pixelRow, pixelCol);
        } else if (mat.channels() == 3) {
            cv::Vec3b bgr = mat.at<cv::Vec3b>(pixelRow, pixelCol);
            grayValue = static_cast<int>(0.299 * bgr[2] + 0.587 * bgr[1] + 0.114 * bgr[0]);
        }
    }

    // 更新编辑面板
    relRowSpinBox_->setValue(relRow);
    relColSpinBox_->setValue(relCol);
    expectedGraySpinBox_->setValue(grayValue);

    // 退出拾取模式
    pickFromImageBtn_->setChecked(false);
}

void ModelCheckPointDialog::applyChanges()
{
    tool_->setCheckPoints(checkPoints_);
    tool_->setCheckPointsEnabled(enableCheckPointsCheckBox_->isChecked());
    emit checkPointsChanged();
}

void ModelCheckPointDialog::onOkClicked()
{
    applyChanges();
    accept();
}

void ModelCheckPointDialog::onCancelClicked()
{
    reject();
}

void ModelCheckPointDialog::onApplyClicked()
{
    applyChanges();
}

} // namespace UI
} // namespace VisionForge
