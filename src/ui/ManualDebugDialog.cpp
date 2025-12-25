/**
 * @file ManualDebugDialog.cpp
 * @brief 手动调试中心实现
 */

#include "ui/ManualDebugDialog.h"
#include "ui/HalconImageViewer.h"
#include "core/StationConfig.h"
#include "core/MultiStationManager.h"
#include "hal/ICamera.h"
#include "base/Logger.h"
#include "base/ImageData.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QSplitter>
#include <QMessageBox>

namespace VisionForge {
namespace UI {

ManualDebugDialog::ManualDebugDialog(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle(tr("手动调试中心"));
    setMinimumSize(1200, 800);
    resize(1400, 900);

    setupUI();
    setupConnections();
    refreshStations();
}

ManualDebugDialog::~ManualDebugDialog()
{
    if (continuousTimer_) {
        continuousTimer_->stop();
    }
}

void ManualDebugDialog::setupUI()
{
    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->setSpacing(8);
    mainLayout->setContentsMargins(8, 8, 8, 8);

    // 左侧控制面板
    QWidget* leftPanel = new QWidget(this);
    QVBoxLayout* leftLayout = new QVBoxLayout(leftPanel);
    leftLayout->setSpacing(8);
    leftLayout->setContentsMargins(0, 0, 0, 0);

    // 添加各控制组
    leftLayout->addWidget(createSelectionGroup());
    leftLayout->addWidget(createCaptureGroup());
    leftLayout->addWidget(createAxisControlGroup());
    leftLayout->addWidget(createSearchGroup());
    leftLayout->addWidget(createResultGroup());
    leftLayout->addWidget(createPlatformPosGroup());
    leftLayout->addStretch();

    leftPanel->setFixedWidth(320);

    // 右侧图像显示
    imageViewer_ = new HalconImageViewer(this);
    imageViewer_->setMinimumSize(600, 500);

    // 使用分割器
    QSplitter* splitter = new QSplitter(Qt::Horizontal, this);
    splitter->addWidget(leftPanel);
    splitter->addWidget(imageViewer_);
    splitter->setStretchFactor(0, 0);
    splitter->setStretchFactor(1, 1);

    mainLayout->addWidget(splitter);

    // 创建连续拍照定时器
    continuousTimer_ = new QTimer(this);
    continuousTimer_->setInterval(100); // 10fps
}

QGroupBox* ManualDebugDialog::createSelectionGroup()
{
    QGroupBox* group = new QGroupBox(tr("选择"), this);
    QGridLayout* layout = new QGridLayout(group);
    layout->setSpacing(6);

    // 工位选择
    layout->addWidget(new QLabel(tr("工位:")), 0, 0);
    comboStation_ = new QComboBox(this);
    layout->addWidget(comboStation_, 0, 1);

    // 相机选择
    layout->addWidget(new QLabel(tr("相机:")), 1, 0);
    comboCamera_ = new QComboBox(this);
    layout->addWidget(comboCamera_, 1, 1);

    // 位置选择
    layout->addWidget(new QLabel(tr("位置:")), 2, 0);
    comboPosition_ = new QComboBox(this);
    layout->addWidget(comboPosition_, 2, 1);

    return group;
}

QGroupBox* ManualDebugDialog::createCaptureGroup()
{
    QGroupBox* group = new QGroupBox(tr("采集控制"), this);
    QHBoxLayout* layout = new QHBoxLayout(group);
    layout->setSpacing(6);

    btnSingleCapture_ = new QPushButton(tr("单拍"), this);
    btnSingleCapture_->setIcon(QIcon(":/icons/camera.png"));
    layout->addWidget(btnSingleCapture_);

    btnContinuous_ = new QPushButton(tr("连拍"), this);
    btnContinuous_->setCheckable(true);
    layout->addWidget(btnContinuous_);

    btnStopCapture_ = new QPushButton(tr("停止"), this);
    btnStopCapture_->setEnabled(false);
    layout->addWidget(btnStopCapture_);

    return group;
}

QGroupBox* ManualDebugDialog::createAxisControlGroup()
{
    groupAxisControl_ = new QGroupBox(tr("轴控制 (Jog)"), this);
    QGridLayout* layout = new QGridLayout(groupAxisControl_);
    layout->setSpacing(6);

    // 步进量设置
    layout->addWidget(new QLabel(tr("步进(mm):")), 0, 0);
    spinJogStep_ = new QDoubleSpinBox(this);
    spinJogStep_->setRange(0.001, 100.0);
    spinJogStep_->setValue(1.0);
    spinJogStep_->setDecimals(3);
    spinJogStep_->setSingleStep(0.1);
    layout->addWidget(spinJogStep_, 0, 1, 1, 2);

    // X轴控制
    layout->addWidget(new QLabel(tr("X轴:")), 1, 0);
    btnXMinus_ = new QPushButton(tr("-"), this);
    btnXMinus_->setFixedSize(40, 30);
    layout->addWidget(btnXMinus_, 1, 1);
    btnXPlus_ = new QPushButton(tr("+"), this);
    btnXPlus_->setFixedSize(40, 30);
    layout->addWidget(btnXPlus_, 1, 2);

    // Y轴控制
    layout->addWidget(new QLabel(tr("Y轴:")), 2, 0);
    btnYMinus_ = new QPushButton(tr("-"), this);
    btnYMinus_->setFixedSize(40, 30);
    layout->addWidget(btnYMinus_, 2, 1);
    btnYPlus_ = new QPushButton(tr("+"), this);
    btnYPlus_->setFixedSize(40, 30);
    layout->addWidget(btnYPlus_, 2, 2);

    // D轴(旋转)控制
    layout->addWidget(new QLabel(tr("D轴:")), 3, 0);
    btnDMinus_ = new QPushButton(tr("-"), this);
    btnDMinus_->setFixedSize(40, 30);
    layout->addWidget(btnDMinus_, 3, 1);
    btnDPlus_ = new QPushButton(tr("+"), this);
    btnDPlus_->setFixedSize(40, 30);
    layout->addWidget(btnDPlus_, 3, 2);

    // 移动到位置按钮
    btnMoveToPos_ = new QPushButton(tr("移动到位置"), this);
    layout->addWidget(btnMoveToPos_, 4, 0, 1, 3);

    return groupAxisControl_;
}

QGroupBox* ManualDebugDialog::createSearchGroup()
{
    QGroupBox* group = new QGroupBox(tr("定位测试"), this);
    QHBoxLayout* layout = new QHBoxLayout(group);
    layout->setSpacing(6);

    btnRunSearch_ = new QPushButton(tr("执行定位"), this);
    btnRunSearch_->setIcon(QIcon(":/icons/search.png"));
    layout->addWidget(btnRunSearch_);

    btnTeachModel_ = new QPushButton(tr("示教模板"), this);
    btnTeachModel_->setIcon(QIcon(":/icons/teach.png"));
    layout->addWidget(btnTeachModel_);

    return group;
}

QGroupBox* ManualDebugDialog::createResultGroup()
{
    QGroupBox* group = new QGroupBox(tr("定位结果"), this);
    QGridLayout* layout = new QGridLayout(group);
    layout->setSpacing(4);

    // X坐标
    layout->addWidget(new QLabel(tr("X:")), 0, 0);
    labelPosX_ = new QLabel(tr("--"), this);
    labelPosX_->setStyleSheet("QLabel { font-weight: bold; color: #00aaff; }");
    layout->addWidget(labelPosX_, 0, 1);

    // Y坐标
    layout->addWidget(new QLabel(tr("Y:")), 0, 2);
    labelPosY_ = new QLabel(tr("--"), this);
    labelPosY_->setStyleSheet("QLabel { font-weight: bold; color: #00aaff; }");
    layout->addWidget(labelPosY_, 0, 3);

    // 角度
    layout->addWidget(new QLabel(tr("角度:")), 1, 0);
    labelAngle_ = new QLabel(tr("--"), this);
    labelAngle_->setStyleSheet("QLabel { font-weight: bold; color: #00aaff; }");
    layout->addWidget(labelAngle_, 1, 1);

    // 分数
    layout->addWidget(new QLabel(tr("分数:")), 1, 2);
    labelScore_ = new QLabel(tr("--"), this);
    labelScore_->setStyleSheet("QLabel { font-weight: bold; color: #00ff00; }");
    layout->addWidget(labelScore_, 1, 3);

    // 耗时
    layout->addWidget(new QLabel(tr("耗时:")), 2, 0);
    labelTime_ = new QLabel(tr("--"), this);
    layout->addWidget(labelTime_, 2, 1);

    // 状态
    layout->addWidget(new QLabel(tr("状态:")), 2, 2);
    labelStatus_ = new QLabel(tr("就绪"), this);
    labelStatus_->setStyleSheet("QLabel { font-weight: bold; }");
    layout->addWidget(labelStatus_, 2, 3);

    return group;
}

QGroupBox* ManualDebugDialog::createPlatformPosGroup()
{
    QGroupBox* group = new QGroupBox(tr("平台坐标"), this);
    QGridLayout* layout = new QGridLayout(group);
    layout->setSpacing(4);

    // 平台X
    layout->addWidget(new QLabel(tr("平台X:")), 0, 0);
    labelPlatX_ = new QLabel(tr("0.000"), this);
    labelPlatX_->setStyleSheet("QLabel { font-family: monospace; }");
    layout->addWidget(labelPlatX_, 0, 1);

    // 平台Y
    layout->addWidget(new QLabel(tr("平台Y:")), 0, 2);
    labelPlatY_ = new QLabel(tr("0.000"), this);
    labelPlatY_->setStyleSheet("QLabel { font-family: monospace; }");
    layout->addWidget(labelPlatY_, 0, 3);

    // 平台D
    layout->addWidget(new QLabel(tr("平台D:")), 1, 0);
    labelPlatD_ = new QLabel(tr("0.000"), this);
    labelPlatD_->setStyleSheet("QLabel { font-family: monospace; }");
    layout->addWidget(labelPlatD_, 1, 1);

    return group;
}

void ManualDebugDialog::setupConnections()
{
    // 选择变化
    connect(comboStation_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ManualDebugDialog::onStationChanged);
    connect(comboCamera_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ManualDebugDialog::onCameraChanged);
    connect(comboPosition_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ManualDebugDialog::onPositionChanged);

    // 采集控制
    connect(btnSingleCapture_, &QPushButton::clicked,
            this, &ManualDebugDialog::onSingleCapture);
    connect(btnContinuous_, &QPushButton::clicked,
            this, &ManualDebugDialog::onContinuousCapture);
    connect(btnStopCapture_, &QPushButton::clicked,
            this, &ManualDebugDialog::onStopCapture);
    connect(continuousTimer_, &QTimer::timeout,
            this, &ManualDebugDialog::doContinuousCapture);

    // 轴控制
    connect(btnXPlus_, &QPushButton::clicked, this, &ManualDebugDialog::onXPlus);
    connect(btnXMinus_, &QPushButton::clicked, this, &ManualDebugDialog::onXMinus);
    connect(btnYPlus_, &QPushButton::clicked, this, &ManualDebugDialog::onYPlus);
    connect(btnYMinus_, &QPushButton::clicked, this, &ManualDebugDialog::onYMinus);
    connect(btnDPlus_, &QPushButton::clicked, this, &ManualDebugDialog::onDPlus);
    connect(btnDMinus_, &QPushButton::clicked, this, &ManualDebugDialog::onDMinus);
    connect(btnMoveToPos_, &QPushButton::clicked,
            this, &ManualDebugDialog::onMoveToPosition);

    // 定位测试
    connect(btnRunSearch_, &QPushButton::clicked,
            this, &ManualDebugDialog::onRunSearch);
    connect(btnTeachModel_, &QPushButton::clicked,
            this, &ManualDebugDialog::onTeachModel);
}

void ManualDebugDialog::setStation(Core::StationConfig* station)
{
    currentStation_ = station;

    if (station) {
        // 查找工位索引
        int index = comboStation_->findData(station->id);
        if (index >= 0) {
            comboStation_->setCurrentIndex(index);
        }
    }

    refreshCameras();
    refreshPositions();
}

void ManualDebugDialog::refreshStations()
{
    comboStation_->clear();

    auto& manager = Core::MultiStationManager::instance();
    auto stations = manager.getAllStations();

    for (const auto* station : stations) {
        comboStation_->addItem(station->name, station->id);
    }

    if (!stations.isEmpty()) {
        currentStation_ = const_cast<Core::StationConfig*>(stations.first());
        refreshCameras();
        refreshPositions();
    }
}

void ManualDebugDialog::refreshCameras()
{
    comboCamera_->clear();

    if (!currentStation_) {
        return;
    }

    for (int i = 0; i < currentStation_->cameraIds.size(); ++i) {
        const QString& cameraId = currentStation_->cameraIds[i];
        comboCamera_->addItem(QString("相机 %1 (%2)").arg(i + 1).arg(cameraId), cameraId);
    }

    if (!currentStation_->cameraIds.isEmpty()) {
        currentCameraId_ = currentStation_->cameraIds.first();
    }
}

void ManualDebugDialog::refreshPositions()
{
    comboPosition_->clear();

    if (!currentStation_) {
        return;
    }

    for (const auto& binding : currentStation_->positionBindings) {
        comboPosition_->addItem(binding.positionName, binding.positionId);
    }

    if (!currentStation_->positionBindings.isEmpty()) {
        currentPositionId_ = currentStation_->positionBindings.first().positionId;
    }
}

void ManualDebugDialog::onStationChanged(int index)
{
    if (index < 0) {
        currentStation_ = nullptr;
        return;
    }

    QString stationId = comboStation_->itemData(index).toString();
    auto& manager = Core::MultiStationManager::instance();
    currentStation_ = manager.getStation(stationId);

    refreshCameras();
    refreshPositions();

    LOG_INFO(QString("ManualDebugDialog: 切换到工位 %1").arg(stationId));
}

void ManualDebugDialog::onCameraChanged(int index)
{
    if (index < 0) {
        currentCameraId_.clear();
        return;
    }

    currentCameraId_ = comboCamera_->itemData(index).toString();
    LOG_DEBUG(QString("ManualDebugDialog: 选择相机 %1").arg(currentCameraId_));
}

void ManualDebugDialog::onPositionChanged(int index)
{
    if (index < 0) {
        currentPositionId_.clear();
        return;
    }

    currentPositionId_ = comboPosition_->itemData(index).toString();
    LOG_DEBUG(QString("ManualDebugDialog: 选择位置 %1").arg(currentPositionId_));
}

void ManualDebugDialog::onSingleCapture()
{
    doCapture();
}

void ManualDebugDialog::onContinuousCapture()
{
    if (btnContinuous_->isChecked()) {
        isContinuousMode_ = true;
        btnSingleCapture_->setEnabled(false);
        btnStopCapture_->setEnabled(true);
        continuousTimer_->start();
        labelStatus_->setText(tr("连续拍照中..."));
        labelStatus_->setStyleSheet("QLabel { font-weight: bold; color: #00ff00; }");
    } else {
        onStopCapture();
    }
}

void ManualDebugDialog::onStopCapture()
{
    isContinuousMode_ = false;
    continuousTimer_->stop();
    btnContinuous_->setChecked(false);
    btnSingleCapture_->setEnabled(true);
    btnStopCapture_->setEnabled(false);
    labelStatus_->setText(tr("就绪"));
    labelStatus_->setStyleSheet("QLabel { font-weight: bold; }");
}

void ManualDebugDialog::doContinuousCapture()
{
    if (isContinuousMode_) {
        doCapture();
    }
}

void ManualDebugDialog::doCapture()
{
    // 获取当前相机
    HAL::ICamera* camera = getCurrentCamera();
    if (!camera) {
        LOG_WARNING("ManualDebugDialog: 无法获取相机");
        labelStatus_->setText(tr("无相机"));
        labelStatus_->setStyleSheet("QLabel { font-weight: bold; color: #ff0000; }");
        return;
    }

    // 执行采集
    auto image = camera->grabImage();
    if (image) {
        imageViewer_->setImage(image);
        if (!isContinuousMode_) {
            labelStatus_->setText(tr("采集成功"));
            labelStatus_->setStyleSheet("QLabel { font-weight: bold; color: #00ff00; }");
        }
    } else {
        LOG_WARNING("ManualDebugDialog: 采集失败");
        if (!isContinuousMode_) {
            labelStatus_->setText(tr("采集失败"));
            labelStatus_->setStyleSheet("QLabel { font-weight: bold; color: #ff0000; }");
        }
    }
}

HAL::ICamera* ManualDebugDialog::getCurrentCamera()
{
    // TODO: 从 CameraManager 获取相机实例
    // 这里需要与实际的相机管理模块集成
    return nullptr;
}

void ManualDebugDialog::onXPlus()
{
    double step = spinJogStep_->value();
    emit axisJogRequested(0, +1, step);
}

void ManualDebugDialog::onXMinus()
{
    double step = spinJogStep_->value();
    emit axisJogRequested(0, -1, step);
}

void ManualDebugDialog::onYPlus()
{
    double step = spinJogStep_->value();
    emit axisJogRequested(1, +1, step);
}

void ManualDebugDialog::onYMinus()
{
    double step = spinJogStep_->value();
    emit axisJogRequested(1, -1, step);
}

void ManualDebugDialog::onDPlus()
{
    double step = spinJogStep_->value();
    emit axisJogRequested(2, +1, step);
}

void ManualDebugDialog::onDMinus()
{
    double step = spinJogStep_->value();
    emit axisJogRequested(2, -1, step);
}

void ManualDebugDialog::onMoveToPosition()
{
    if (currentPositionId_.isEmpty()) {
        QMessageBox::warning(this, tr("警告"), tr("请先选择位置"));
        return;
    }

    emit moveToPositionRequested(currentPositionId_);
    labelStatus_->setText(tr("移动中..."));
}

void ManualDebugDialog::onRunSearch()
{
    labelStatus_->setText(tr("定位中..."));
    labelStatus_->setStyleSheet("QLabel { font-weight: bold; color: #ffaa00; }");

    // TODO: 执行实际的定位搜索
    // 这里需要与定位算法模块集成

    // 临时模拟结果
    SearchResult result;
    result.posX = 100.5;
    result.posY = 200.3;
    result.angle = 1.23;
    result.score = 0.95;
    result.timeMs = 35.6;
    result.success = true;
    result.message = "定位成功";

    updateResults(result);
}

void ManualDebugDialog::onTeachModel()
{
    if (currentPositionId_.isEmpty()) {
        QMessageBox::warning(this, tr("警告"), tr("请先选择位置"));
        return;
    }

    emit teachModelRequested(currentPositionId_);
}

void ManualDebugDialog::updateResults(const SearchResult& result)
{
    labelPosX_->setText(QString::number(result.posX, 'f', 3));
    labelPosY_->setText(QString::number(result.posY, 'f', 3));
    labelAngle_->setText(QString::number(result.angle, 'f', 3) + QString::fromUtf8("\u00b0"));
    labelScore_->setText(QString::number(result.score, 'f', 3));
    labelTime_->setText(QString::number(result.timeMs, 'f', 1) + " ms");

    if (result.success) {
        labelStatus_->setText(result.message.isEmpty() ? tr("成功") : result.message);
        labelStatus_->setStyleSheet("QLabel { font-weight: bold; color: #00ff00; }");
        labelScore_->setStyleSheet("QLabel { font-weight: bold; color: #00ff00; }");
    } else {
        labelStatus_->setText(result.message.isEmpty() ? tr("失败") : result.message);
        labelStatus_->setStyleSheet("QLabel { font-weight: bold; color: #ff0000; }");
        labelScore_->setStyleSheet("QLabel { font-weight: bold; color: #ff0000; }");
    }

    emit searchCompleted(result);
}

void ManualDebugDialog::updatePlatformPos(double x, double y, double d)
{
    labelPlatX_->setText(QString::number(x, 'f', 3));
    labelPlatY_->setText(QString::number(y, 'f', 3));
    labelPlatD_->setText(QString::number(d, 'f', 3));
}

} // namespace UI
} // namespace VisionForge
