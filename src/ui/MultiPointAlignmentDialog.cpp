/**
 * @file MultiPointAlignmentDialog.cpp
 * @brief 多点对位配置对话框实现
 * @author VisionForge Team
 * @date 2025-12-19
 */

#include "ui/MultiPointAlignmentDialog.h"
#include "hal/CameraManager.h"
#include "base/Logger.h"

#include <opencv2/imgproc.hpp>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QTableWidget>
#include <QTabWidget>
#include <QLineEdit>
#include <QCheckBox>
#include <QHeaderView>
#include <QMessageBox>
#include <QInputDialog>
#include <QFileDialog>

namespace VisionForge {
namespace UI {

MultiPointAlignmentDialog::MultiPointAlignmentDialog(QWidget* parent)
    : QDialog(parent)
    , alignmentTool_(nullptr)
{
    setWindowTitle(tr("多点对位配置"));
    setMinimumSize(800, 600);

    setupUI();
}

MultiPointAlignmentDialog::~MultiPointAlignmentDialog()
{
}

void MultiPointAlignmentDialog::setAlignmentTool(Algorithm::MultiPointAlignmentTool* tool)
{
    alignmentTool_ = tool;
    updateFromTool();
}

Algorithm::MultiPointAlignmentTool* MultiPointAlignmentDialog::getAlignmentTool() const
{
    return alignmentTool_;
}

void MultiPointAlignmentDialog::setupUI()
{
    auto* mainLayout = new QVBoxLayout(this);

    // 标签页
    tabWidget_ = new QTabWidget();

    createPointListTab();
    createDeviationSettingsTab();
    createCoordinateTab();

    tabWidget_->addTab(pointListTab_, tr("对位点配置"));
    tabWidget_->addTab(deviationTab_, tr("偏差计算"));
    tabWidget_->addTab(coordinateTab_, tr("坐标转换"));

    mainLayout->addWidget(tabWidget_);

    // 底部按钮
    createButtonGroup();
    auto* btnLayout = new QHBoxLayout();
    btnLayout->addWidget(testBtn_);
    btnLayout->addStretch();
    btnLayout->addWidget(applyBtn_);
    btnLayout->addWidget(okBtn_);
    btnLayout->addWidget(cancelBtn_);

    mainLayout->addLayout(btnLayout);
}

void MultiPointAlignmentDialog::createPointListTab()
{
    pointListTab_ = new QWidget();
    auto* layout = new QHBoxLayout(pointListTab_);

    // 左侧：点列表
    auto* leftWidget = new QWidget();
    auto* leftLayout = new QVBoxLayout(leftWidget);
    leftLayout->setContentsMargins(0, 0, 0, 0);

    // 点表格
    pointTable_ = new QTableWidget();
    pointTable_->setColumnCount(5);
    pointTable_->setHorizontalHeaderLabels({
        tr("ID"), tr("名称"), tr("类型"), tr("相机"), tr("启用")
    });
    pointTable_->horizontalHeader()->setStretchLastSection(true);
    pointTable_->setSelectionBehavior(QAbstractItemView::SelectRows);
    pointTable_->setSelectionMode(QAbstractItemView::SingleSelection);
    pointTable_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    pointTable_->verticalHeader()->setVisible(false);

    connect(pointTable_, &QTableWidget::itemSelectionChanged,
            this, &MultiPointAlignmentDialog::onPointSelectionChanged);

    leftLayout->addWidget(pointTable_);

    // 操作按钮
    auto* btnLayout = new QHBoxLayout();
    addPointBtn_ = new QPushButton(tr("添加"));
    removePointBtn_ = new QPushButton(tr("删除"));
    editPointBtn_ = new QPushButton(tr("编辑"));
    moveUpBtn_ = new QPushButton(tr("上移"));
    moveDownBtn_ = new QPushButton(tr("下移"));

    removePointBtn_->setEnabled(false);
    editPointBtn_->setEnabled(false);
    moveUpBtn_->setEnabled(false);
    moveDownBtn_->setEnabled(false);

    connect(addPointBtn_, &QPushButton::clicked, this, &MultiPointAlignmentDialog::onAddPoint);
    connect(removePointBtn_, &QPushButton::clicked, this, &MultiPointAlignmentDialog::onRemovePoint);
    connect(editPointBtn_, &QPushButton::clicked, this, &MultiPointAlignmentDialog::onEditPoint);
    connect(moveUpBtn_, &QPushButton::clicked, this, &MultiPointAlignmentDialog::onMovePointUp);
    connect(moveDownBtn_, &QPushButton::clicked, this, &MultiPointAlignmentDialog::onMovePointDown);

    btnLayout->addWidget(addPointBtn_);
    btnLayout->addWidget(removePointBtn_);
    btnLayout->addWidget(editPointBtn_);
    btnLayout->addStretch();
    btnLayout->addWidget(moveUpBtn_);
    btnLayout->addWidget(moveDownBtn_);

    leftLayout->addLayout(btnLayout);

    layout->addWidget(leftWidget, 1);

    // 右侧：点详情
    auto* rightWidget = new QWidget();
    auto* rightLayout = new QVBoxLayout(rightWidget);
    rightLayout->setContentsMargins(0, 0, 0, 0);

    // 基本信息
    pointDetailGroup_ = new QGroupBox(tr("点信息"));
    auto* detailLayout = new QGridLayout(pointDetailGroup_);

    detailLayout->addWidget(new QLabel(tr("名称:")), 0, 0);
    pointNameEdit_ = new QLineEdit();
    detailLayout->addWidget(pointNameEdit_, 0, 1);

    detailLayout->addWidget(new QLabel(tr("检测类型:")), 1, 0);
    pointTypeCombo_ = new QComboBox();
    pointTypeCombo_->addItem(tr("模板匹配"), static_cast<int>(Algorithm::AlignmentPointType::TemplateMatch));
    pointTypeCombo_->addItem(tr("形状匹配"), static_cast<int>(Algorithm::AlignmentPointType::ShapeMatch));
    pointTypeCombo_->addItem(tr("Blob中心"), static_cast<int>(Algorithm::AlignmentPointType::BlobCenter));
    pointTypeCombo_->addItem(tr("圆心检测"), static_cast<int>(Algorithm::AlignmentPointType::CircleCenter));
    pointTypeCombo_->addItem(tr("边缘点"), static_cast<int>(Algorithm::AlignmentPointType::EdgePoint));
    pointTypeCombo_->addItem(tr("十字中心"), static_cast<int>(Algorithm::AlignmentPointType::CrosshairCenter));
    detailLayout->addWidget(pointTypeCombo_, 1, 1);

    detailLayout->addWidget(new QLabel(tr("关联相机:")), 2, 0);
    pointCameraCombo_ = new QComboBox();
    pointCameraCombo_->addItem(tr("(默认)"), "");
    // 动态添加相机列表
    auto& cameraMgr = HAL::CameraManager::instance();
    for (const QString& cameraId : cameraMgr.getAllCameraIds()) {
        HAL::CameraInfo info = cameraMgr.getCameraInfo(cameraId);
        pointCameraCombo_->addItem(info.name, cameraId);
    }
    detailLayout->addWidget(pointCameraCombo_, 2, 1);

    pointEnabledCheck_ = new QCheckBox(tr("启用"));
    pointEnabledCheck_->setChecked(true);
    detailLayout->addWidget(pointEnabledCheck_, 3, 0, 1, 2);

    rightLayout->addWidget(pointDetailGroup_);

    // ROI设置
    roiGroup_ = new QGroupBox(tr("搜索区域 (ROI)"));
    auto* roiLayout = new QGridLayout(roiGroup_);

    roiLayout->addWidget(new QLabel(tr("X:")), 0, 0);
    roiXSpin_ = new QDoubleSpinBox();
    roiXSpin_->setRange(0, 10000);
    roiXSpin_->setDecimals(1);
    roiLayout->addWidget(roiXSpin_, 0, 1);

    roiLayout->addWidget(new QLabel(tr("Y:")), 0, 2);
    roiYSpin_ = new QDoubleSpinBox();
    roiYSpin_->setRange(0, 10000);
    roiYSpin_->setDecimals(1);
    roiLayout->addWidget(roiYSpin_, 0, 3);

    roiLayout->addWidget(new QLabel(tr("宽度:")), 1, 0);
    roiWidthSpin_ = new QDoubleSpinBox();
    roiWidthSpin_->setRange(1, 10000);
    roiWidthSpin_->setValue(100);
    roiWidthSpin_->setDecimals(1);
    roiLayout->addWidget(roiWidthSpin_, 1, 1);

    roiLayout->addWidget(new QLabel(tr("高度:")), 1, 2);
    roiHeightSpin_ = new QDoubleSpinBox();
    roiHeightSpin_->setRange(1, 10000);
    roiHeightSpin_->setValue(100);
    roiHeightSpin_->setDecimals(1);
    roiLayout->addWidget(roiHeightSpin_, 1, 3);

    setROIBtn_ = new QPushButton(tr("从图像设置"));
    connect(setROIBtn_, &QPushButton::clicked, this, &MultiPointAlignmentDialog::onSetROI);
    roiLayout->addWidget(setROIBtn_, 2, 0, 1, 4);

    rightLayout->addWidget(roiGroup_);

    // 标准位置
    standardPosGroup_ = new QGroupBox(tr("标准位置"));
    auto* stdPosLayout = new QGridLayout(standardPosGroup_);

    stdPosLayout->addWidget(new QLabel(tr("X:")), 0, 0);
    stdPosXSpin_ = new QDoubleSpinBox();
    stdPosXSpin_->setRange(-10000, 10000);
    stdPosXSpin_->setDecimals(3);
    stdPosLayout->addWidget(stdPosXSpin_, 0, 1);

    stdPosLayout->addWidget(new QLabel(tr("Y:")), 0, 2);
    stdPosYSpin_ = new QDoubleSpinBox();
    stdPosYSpin_->setRange(-10000, 10000);
    stdPosYSpin_->setDecimals(3);
    stdPosLayout->addWidget(stdPosYSpin_, 0, 3);

    stdPosLayout->addWidget(new QLabel(tr("角度:")), 1, 0);
    stdAngleSpin_ = new QDoubleSpinBox();
    stdAngleSpin_->setRange(-180, 180);
    stdAngleSpin_->setDecimals(3);
    stdAngleSpin_->setSuffix(tr("°"));
    stdPosLayout->addWidget(stdAngleSpin_, 1, 1);

    rightLayout->addWidget(standardPosGroup_);

    // 模板
    templateGroup_ = new QGroupBox(tr("模板"));
    auto* templateLayout = new QVBoxLayout(templateGroup_);

    templatePreviewLabel_ = new QLabel();
    templatePreviewLabel_->setMinimumSize(150, 150);
    templatePreviewLabel_->setAlignment(Qt::AlignCenter);
    templatePreviewLabel_->setStyleSheet("QLabel { background-color: #333; border: 1px solid #555; }");
    templatePreviewLabel_->setText(tr("无模板"));
    templateLayout->addWidget(templatePreviewLabel_);

    auto* templateBtnLayout = new QHBoxLayout();
    createTemplateBtn_ = new QPushButton(tr("创建模板"));
    clearTemplateBtn_ = new QPushButton(tr("清除"));

    connect(createTemplateBtn_, &QPushButton::clicked, this, &MultiPointAlignmentDialog::onCreateTemplate);
    connect(clearTemplateBtn_, &QPushButton::clicked, this, &MultiPointAlignmentDialog::onClearTemplate);

    templateBtnLayout->addWidget(createTemplateBtn_);
    templateBtnLayout->addWidget(clearTemplateBtn_);
    templateLayout->addLayout(templateBtnLayout);

    rightLayout->addWidget(templateGroup_);
    rightLayout->addStretch();

    layout->addWidget(rightWidget, 1);
}

void MultiPointAlignmentDialog::createDeviationSettingsTab()
{
    deviationTab_ = new QWidget();
    auto* layout = new QVBoxLayout(deviationTab_);

    // 偏差计算模式
    auto* modeGroup = new QGroupBox(tr("偏差计算模式"));
    auto* modeLayout = new QVBoxLayout(modeGroup);

    deviationModeCombo_ = new QComboBox();
    deviationModeCombo_->addItem(tr("单点偏差 - 只使用第一个点"),
        static_cast<int>(Algorithm::DeviationCalcMode::SinglePoint));
    deviationModeCombo_->addItem(tr("两点偏差 - 使用两点计算X/Y/θ"),
        static_cast<int>(Algorithm::DeviationCalcMode::TwoPoints));
    deviationModeCombo_->addItem(tr("多点最小二乘 - 使用所有点"),
        static_cast<int>(Algorithm::DeviationCalcMode::MultiPoints));
    deviationModeCombo_->addItem(tr("SVD刚性变换 - 求解最佳变换矩阵"),
        static_cast<int>(Algorithm::DeviationCalcMode::RigidTransform));

    connect(deviationModeCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MultiPointAlignmentDialog::onDeviationModeChanged);

    modeLayout->addWidget(deviationModeCombo_);

    // 模式说明
    auto* modeDescLabel = new QLabel();
    modeDescLabel->setWordWrap(true);
    modeDescLabel->setStyleSheet("color: gray;");
    modeDescLabel->setText(tr(
        "单点: 直接使用检测点与标准点的差值\n"
        "两点: 通过两点计算平移和旋转\n"
        "多点最小二乘: 最小化所有点的误差平方和\n"
        "SVD刚性变换: 使用奇异值分解求解最优刚性变换"
    ));
    modeLayout->addWidget(modeDescLabel);

    layout->addWidget(modeGroup);

    // 旋转中心
    auto* rotationGroup = new QGroupBox(tr("旋转中心"));
    auto* rotationLayout = new QGridLayout(rotationGroup);

    autoRotationCenterCheck_ = new QCheckBox(tr("自动计算旋转中心"));
    autoRotationCenterCheck_->setChecked(true);
    connect(autoRotationCenterCheck_, &QCheckBox::toggled,
            this, &MultiPointAlignmentDialog::onAutoRotationCenterChanged);
    rotationLayout->addWidget(autoRotationCenterCheck_, 0, 0, 1, 4);

    rotationLayout->addWidget(new QLabel(tr("X:")), 1, 0);
    rotationCenterXSpin_ = new QDoubleSpinBox();
    rotationCenterXSpin_->setRange(-10000, 10000);
    rotationCenterXSpin_->setDecimals(3);
    rotationCenterXSpin_->setEnabled(false);
    connect(rotationCenterXSpin_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MultiPointAlignmentDialog::onRotationCenterChanged);
    rotationLayout->addWidget(rotationCenterXSpin_, 1, 1);

    rotationLayout->addWidget(new QLabel(tr("Y:")), 1, 2);
    rotationCenterYSpin_ = new QDoubleSpinBox();
    rotationCenterYSpin_->setRange(-10000, 10000);
    rotationCenterYSpin_->setDecimals(3);
    rotationCenterYSpin_->setEnabled(false);
    connect(rotationCenterYSpin_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MultiPointAlignmentDialog::onRotationCenterChanged);
    rotationLayout->addWidget(rotationCenterYSpin_, 1, 3);

    layout->addWidget(rotationGroup);

    layout->addStretch();
}

void MultiPointAlignmentDialog::createCoordinateTab()
{
    coordinateTab_ = new QWidget();
    auto* layout = new QVBoxLayout(coordinateTab_);

    // 坐标转换设置
    auto* coordGroup = new QGroupBox(tr("坐标转换"));
    auto* coordLayout = new QVBoxLayout(coordGroup);

    coordTransformEnabledCheck_ = new QCheckBox(tr("启用坐标转换（像素坐标 → 世界坐标）"));
    connect(coordTransformEnabledCheck_, &QCheckBox::toggled,
            this, &MultiPointAlignmentDialog::onCoordTransformEnabledChanged);
    coordLayout->addWidget(coordTransformEnabledCheck_);

    auto* calibLayout = new QHBoxLayout();
    calibLayout->addWidget(new QLabel(tr("标定数据:")));
    calibrationCombo_ = new QComboBox();
    calibrationCombo_->addItem(tr("(未选择)"), "");
    calibLayout->addWidget(calibrationCombo_, 1);

    loadCalibrationBtn_ = new QPushButton(tr("加载..."));
    connect(loadCalibrationBtn_, &QPushButton::clicked, this, &MultiPointAlignmentDialog::onLoadCalibration);
    calibLayout->addWidget(loadCalibrationBtn_);

    coordLayout->addLayout(calibLayout);

    // 标定信息
    calibrationInfoLabel_ = new QLabel(tr("未加载标定数据"));
    calibrationInfoLabel_->setStyleSheet("color: gray; padding: 10px;");
    calibrationInfoLabel_->setWordWrap(true);
    coordLayout->addWidget(calibrationInfoLabel_);

    layout->addWidget(coordGroup);

    // 说明
    auto* infoGroup = new QGroupBox(tr("说明"));
    auto* infoLayout = new QVBoxLayout(infoGroup);

    auto* infoLabel = new QLabel(tr(
        "坐标转换功能将检测到的像素坐标转换为世界坐标（机械坐标）。\n\n"
        "使用方法:\n"
        "1. 首先进行相机标定（9点标定）\n"
        "2. 保存标定结果\n"
        "3. 在此处加载标定数据\n"
        "4. 启用坐标转换\n\n"
        "支持的变换类型: 仿射变换、透视变换、相似变换、刚性变换"
    ));
    infoLabel->setWordWrap(true);
    infoLayout->addWidget(infoLabel);

    layout->addWidget(infoGroup);

    layout->addStretch();
}

void MultiPointAlignmentDialog::createButtonGroup()
{
    testBtn_ = new QPushButton(tr("测试检测"));
    applyBtn_ = new QPushButton(tr("应用"));
    okBtn_ = new QPushButton(tr("确定"));
    cancelBtn_ = new QPushButton(tr("取消"));

    connect(testBtn_, &QPushButton::clicked, this, &MultiPointAlignmentDialog::onTestDetection);
    connect(applyBtn_, &QPushButton::clicked, this, &MultiPointAlignmentDialog::onApply);
    connect(okBtn_, &QPushButton::clicked, [this]() {
        onApply();
        accept();
    });
    connect(cancelBtn_, &QPushButton::clicked, this, &QDialog::reject);
}

void MultiPointAlignmentDialog::updatePointTable()
{
    pointTable_->setRowCount(0);

    if (!alignmentTool_) return;

    QVector<Algorithm::AlignmentPointConfig> points = alignmentTool_->getAllAlignmentPoints();

    for (int i = 0; i < points.size(); ++i) {
        const auto& point = points[i];

        pointTable_->insertRow(i);
        pointTable_->setItem(i, 0, new QTableWidgetItem(point.id));
        pointTable_->setItem(i, 1, new QTableWidgetItem(point.name));
        pointTable_->setItem(i, 2, new QTableWidgetItem(getPointTypeText(point.type)));
        pointTable_->setItem(i, 3, new QTableWidgetItem(point.cameraId.isEmpty() ? tr("默认") : point.cameraId));
        pointTable_->setItem(i, 4, new QTableWidgetItem(point.enabled ? tr("是") : tr("否")));
    }

    pointTable_->resizeColumnsToContents();
}

void MultiPointAlignmentDialog::updatePointDetails()
{
    bool hasSelection = !selectedPointId_.isEmpty() && alignmentTool_;

    pointDetailGroup_->setEnabled(hasSelection);
    roiGroup_->setEnabled(hasSelection);
    standardPosGroup_->setEnabled(hasSelection);
    templateGroup_->setEnabled(hasSelection);

    if (!hasSelection) return;

    Algorithm::AlignmentPointConfig config = alignmentTool_->getAlignmentPoint(selectedPointId_);

    pointNameEdit_->setText(config.name);

    int typeIndex = pointTypeCombo_->findData(static_cast<int>(config.type));
    if (typeIndex >= 0) {
        pointTypeCombo_->setCurrentIndex(typeIndex);
    }

    int cameraIndex = pointCameraCombo_->findData(config.cameraId);
    if (cameraIndex >= 0) {
        pointCameraCombo_->setCurrentIndex(cameraIndex);
    }

    pointEnabledCheck_->setChecked(config.enabled);

    roiXSpin_->setValue(config.searchROI.x());
    roiYSpin_->setValue(config.searchROI.y());
    roiWidthSpin_->setValue(config.searchROI.width());
    roiHeightSpin_->setValue(config.searchROI.height());

    stdPosXSpin_->setValue(config.standardPosition.x());
    stdPosYSpin_->setValue(config.standardPosition.y());
    stdAngleSpin_->setValue(config.standardAngle);

    // 更新模板预览
    cv::Mat templateImg = alignmentTool_->getPointTemplate(selectedPointId_);
    if (!templateImg.empty()) {
        // 转换为QImage显示
        QImage qimg;
        if (templateImg.channels() == 1) {
            qimg = QImage(templateImg.data, templateImg.cols, templateImg.rows,
                         templateImg.step, QImage::Format_Grayscale8).copy();
        } else {
            cv::Mat rgb;
            cv::cvtColor(templateImg, rgb, cv::COLOR_BGR2RGB);
            qimg = QImage(rgb.data, rgb.cols, rgb.rows, rgb.step, QImage::Format_RGB888).copy();
        }

        QPixmap pixmap = QPixmap::fromImage(qimg).scaled(
            templatePreviewLabel_->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        templatePreviewLabel_->setPixmap(pixmap);
    } else {
        templatePreviewLabel_->clear();
        templatePreviewLabel_->setText(tr("无模板"));
    }
}

void MultiPointAlignmentDialog::updateFromTool()
{
    if (!alignmentTool_) return;

    updatePointTable();

    // 偏差计算模式
    int modeIndex = deviationModeCombo_->findData(
        static_cast<int>(alignmentTool_->getDeviationCalcMode()));
    if (modeIndex >= 0) {
        deviationModeCombo_->setCurrentIndex(modeIndex);
    }

    // 旋转中心
    autoRotationCenterCheck_->setChecked(alignmentTool_->isAutoRotationCenter());
    QPointF center = alignmentTool_->getRotationCenter();
    rotationCenterXSpin_->setValue(center.x());
    rotationCenterYSpin_->setValue(center.y());

    // 坐标转换
    coordTransformEnabledCheck_->setChecked(alignmentTool_->isCoordinateTransformEnabled());
}

void MultiPointAlignmentDialog::applyToTool()
{
    if (!alignmentTool_) return;

    // 更新当前选中的点
    if (!selectedPointId_.isEmpty()) {
        Algorithm::AlignmentPointConfig config = alignmentTool_->getAlignmentPoint(selectedPointId_);

        config.name = pointNameEdit_->text();
        config.type = static_cast<Algorithm::AlignmentPointType>(
            pointTypeCombo_->currentData().toInt());
        config.cameraId = pointCameraCombo_->currentData().toString();
        config.enabled = pointEnabledCheck_->isChecked();

        config.searchROI = QRectF(roiXSpin_->value(), roiYSpin_->value(),
                                  roiWidthSpin_->value(), roiHeightSpin_->value());

        config.standardPosition = QPointF(stdPosXSpin_->value(), stdPosYSpin_->value());
        config.standardAngle = stdAngleSpin_->value();

        alignmentTool_->updateAlignmentPoint(selectedPointId_, config);
    }

    // 偏差计算模式
    alignmentTool_->setDeviationCalcMode(static_cast<Algorithm::DeviationCalcMode>(
        deviationModeCombo_->currentData().toInt()));

    // 旋转中心
    alignmentTool_->setAutoRotationCenter(autoRotationCenterCheck_->isChecked());
    if (!autoRotationCenterCheck_->isChecked()) {
        alignmentTool_->setRotationCenter(QPointF(
            rotationCenterXSpin_->value(), rotationCenterYSpin_->value()));
    }

    // 坐标转换
    alignmentTool_->setCoordinateTransformEnabled(coordTransformEnabledCheck_->isChecked());
}

QString MultiPointAlignmentDialog::getPointTypeText(Algorithm::AlignmentPointType type) const
{
    switch (type) {
        case Algorithm::AlignmentPointType::TemplateMatch: return tr("模板匹配");
        case Algorithm::AlignmentPointType::ShapeMatch: return tr("形状匹配");
        case Algorithm::AlignmentPointType::BlobCenter: return tr("Blob中心");
        case Algorithm::AlignmentPointType::CircleCenter: return tr("圆心检测");
        case Algorithm::AlignmentPointType::EdgePoint: return tr("边缘点");
        case Algorithm::AlignmentPointType::CrosshairCenter: return tr("十字中心");
        default: return tr("未知");
    }
}

QString MultiPointAlignmentDialog::getDeviationModeText(Algorithm::DeviationCalcMode mode) const
{
    switch (mode) {
        case Algorithm::DeviationCalcMode::SinglePoint: return tr("单点偏差");
        case Algorithm::DeviationCalcMode::TwoPoints: return tr("两点偏差");
        case Algorithm::DeviationCalcMode::MultiPoints: return tr("多点最小二乘");
        case Algorithm::DeviationCalcMode::RigidTransform: return tr("SVD刚性变换");
        default: return tr("未知");
    }
}

// ========== 对位点管理槽函数 ==========

void MultiPointAlignmentDialog::onAddPoint()
{
    if (!alignmentTool_) return;

    bool ok;
    QString name = QInputDialog::getText(this, tr("添加对位点"),
        tr("点名称:"), QLineEdit::Normal, tr("Point"), &ok);

    if (ok && !name.isEmpty()) {
        Algorithm::AlignmentPointConfig config;
        config.name = name;
        config.type = Algorithm::AlignmentPointType::TemplateMatch;
        config.enabled = true;
        config.searchROI = QRectF(0, 0, 100, 100);

        QString id = alignmentTool_->addAlignmentPoint(config);
        updatePointTable();

        // 选中新添加的点
        for (int i = 0; i < pointTable_->rowCount(); ++i) {
            if (pointTable_->item(i, 0)->text() == id) {
                pointTable_->selectRow(i);
                break;
            }
        }

        LOG_INFO(QString("添加对位点: %1").arg(name));
    }
}

void MultiPointAlignmentDialog::onRemovePoint()
{
    if (selectedPointId_.isEmpty() || !alignmentTool_) return;

    int ret = QMessageBox::question(this, tr("确认"),
        tr("确定要删除对位点 %1 吗?").arg(selectedPointId_),
        QMessageBox::Yes | QMessageBox::No);

    if (ret == QMessageBox::Yes) {
        alignmentTool_->removeAlignmentPoint(selectedPointId_);
        selectedPointId_.clear();
        updatePointTable();
        updatePointDetails();
    }
}

void MultiPointAlignmentDialog::onEditPoint()
{
    // 已通过详情面板编辑
    onApply();
}

void MultiPointAlignmentDialog::onPointSelectionChanged()
{
    int row = pointTable_->currentRow();
    bool hasSelection = (row >= 0);

    removePointBtn_->setEnabled(hasSelection);
    editPointBtn_->setEnabled(hasSelection);
    moveUpBtn_->setEnabled(hasSelection && row > 0);
    moveDownBtn_->setEnabled(hasSelection && row < pointTable_->rowCount() - 1);

    if (hasSelection) {
        selectedPointId_ = pointTable_->item(row, 0)->text();
    } else {
        selectedPointId_.clear();
    }

    updatePointDetails();
}

void MultiPointAlignmentDialog::onMovePointUp()
{
    // TODO: 实现点顺序调整
}

void MultiPointAlignmentDialog::onMovePointDown()
{
    // TODO: 实现点顺序调整
}

void MultiPointAlignmentDialog::onSetROI()
{
    if (selectedPointId_.isEmpty()) return;

    emit requestSetROI(selectedPointId_);
}

void MultiPointAlignmentDialog::onCreateTemplate()
{
    if (selectedPointId_.isEmpty()) return;

    emit requestCreateTemplate(selectedPointId_);
}

void MultiPointAlignmentDialog::onClearTemplate()
{
    if (selectedPointId_.isEmpty() || !alignmentTool_) return;

    alignmentTool_->setPointTemplate(selectedPointId_, cv::Mat());
    templatePreviewLabel_->clear();
    templatePreviewLabel_->setText(tr("无模板"));
}

void MultiPointAlignmentDialog::onDeviationModeChanged(int index)
{
    Q_UNUSED(index)
    // 模式变化时可能需要更新UI状态
}

void MultiPointAlignmentDialog::onAutoRotationCenterChanged(bool checked)
{
    rotationCenterXSpin_->setEnabled(!checked);
    rotationCenterYSpin_->setEnabled(!checked);
}

void MultiPointAlignmentDialog::onRotationCenterChanged()
{
    // 旋转中心变化
}

void MultiPointAlignmentDialog::onCoordTransformEnabledChanged(bool enabled)
{
    calibrationCombo_->setEnabled(enabled);
    loadCalibrationBtn_->setEnabled(enabled);
}

void MultiPointAlignmentDialog::onLoadCalibration()
{
    QString filePath = QFileDialog::getOpenFileName(this,
        tr("加载标定数据"), "", tr("JSON文件 (*.json)"));

    if (!filePath.isEmpty()) {
        // TODO: 加载标定数据
        calibrationInfoLabel_->setText(tr("已加载: %1").arg(filePath));
        LOG_INFO(QString("加载标定数据: %1").arg(filePath));
    }
}

void MultiPointAlignmentDialog::onTestDetection()
{
    if (!alignmentTool_) {
        QMessageBox::warning(this, tr("警告"), tr("未设置对位工具"));
        return;
    }

    applyToTool();

    // 执行检测测试
    // TODO: 需要有当前图像才能测试
    QMessageBox::information(this, tr("测试"),
        tr("请在主窗口中加载图像后进行检测测试"));
}

void MultiPointAlignmentDialog::onApply()
{
    applyToTool();
    emit configurationChanged();
}

} // namespace UI
} // namespace VisionForge
