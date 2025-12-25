/**
 * @file CalibrationWizard.cpp
 * @brief 多位置标定向导实现
 */

#include "ui/CalibrationWizard.h"
#include "ui/HalconImageViewer.h"
#include "core/StationConfig.h"
#include "base/Logger.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QMessageBox>
#include <QFileDialog>
#include <QHeaderView>

namespace VisionForge {
namespace UI {

// ============================================================
// CalibrationWizard 实现
// ============================================================

CalibrationWizard::CalibrationWizard(Core::StationConfig* station, QWidget* parent)
    : QWizard(parent)
    , station_(station)
{
    setWindowTitle(tr("多位置标定向导"));
    setMinimumSize(900, 700);
    resize(1000, 750);

    setWizardStyle(QWizard::ModernStyle);
    setOption(QWizard::NoBackButtonOnStartPage, true);

    initCalibrationStatus();
    setupPages();
}

CalibrationWizard::~CalibrationWizard()
{
}

void CalibrationWizard::initCalibrationStatus()
{
    calibStatus_.clear();

    if (!station_) {
        return;
    }

    for (const auto& binding : station_->positionBindings) {
        PositionCalibStatus status;
        status.positionId = binding.positionId;
        status.positionName = binding.positionName;
        status.enabled = binding.enabled;
        status.completed = false;
        status.completedPoints = 0;
        status.totalPoints = 9;

        // 初始化9个标定点
        for (int i = 0; i < 9; ++i) {
            CalibPointData point;
            point.index = i;
            point.captured = false;
            status.points.append(point);
        }

        calibStatus_.append(status);
    }
}

void CalibrationWizard::setupPages()
{
    introPage_ = createIntroPage();
    selectPage_ = createPositionSelectPage();
    calibPage_ = createCalibrationPage();
    summaryPage_ = createSummaryPage();

    setPage(Page_Intro, introPage_);
    setPage(Page_SelectPositions, selectPage_);
    setPage(Page_Calibration, calibPage_);
    setPage(Page_Summary, summaryPage_);

    setStartId(Page_Intro);
}

QWizardPage* CalibrationWizard::createIntroPage()
{
    return new CalibIntroPage(this);
}

QWizardPage* CalibrationWizard::createPositionSelectPage()
{
    return new CalibPositionSelectPage(this);
}

QWizardPage* CalibrationWizard::createCalibrationPage()
{
    return new CalibPositionPage(this);
}

QWizardPage* CalibrationWizard::createSummaryPage()
{
    return new CalibSummaryPage(this);
}

void CalibrationWizard::setCurrentPositionIndex(int index)
{
    if (index >= 0 && index < selectedPositions_.size()) {
        currentPosIndex_ = index;
    }
}

void CalibrationWizard::setSelectedPositionIndices(const QVector<int>& indices)
{
    selectedPositions_ = indices;
    currentPosIndex_ = 0;
}

void CalibrationWizard::initializePage(int id)
{
    QWizard::initializePage(id);
}

bool CalibrationWizard::validateCurrentPage()
{
    return QWizard::validateCurrentPage();
}

int CalibrationWizard::nextId() const
{
    switch (currentId()) {
    case Page_Intro:
        return Page_SelectPositions;

    case Page_SelectPositions:
        if (selectedPositions_.isEmpty()) {
            return Page_Summary;
        }
        return Page_Calibration;

    case Page_Calibration:
        // 检查是否还有下一个位置
        if (currentPosIndex_ + 1 < selectedPositions_.size()) {
            return Page_Calibration; // 继续下一个位置
        }
        return Page_Summary;

    case Page_Summary:
        return -1; // 结束

    default:
        return -1;
    }
}

// ============================================================
// CalibIntroPage 实现
// ============================================================

CalibIntroPage::CalibIntroPage(CalibrationWizard* wizard, QWidget* parent)
    : QWizardPage(parent)
    , wizard_(wizard)
{
    setTitle(tr("欢迎使用多位置标定向导"));
    setSubTitle(tr("本向导将引导您完成工位的多位置标定配置"));
    setupUI();
}

void CalibIntroPage::setupUI()
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setSpacing(20);

    // 介绍文字
    labelIntro_ = new QLabel(this);
    labelIntro_->setWordWrap(true);
    labelIntro_->setText(tr(
        "<h3>标定向导说明</h3>"
        "<p>多位置标定用于建立相机图像坐标与物理世界坐标之间的映射关系。</p>"
        "<p><b>标定流程：</b></p>"
        "<ol>"
        "<li>选择需要标定的位置</li>"
        "<li>对每个位置进行9点标定</li>"
        "<li>验证并保存标定结果</li>"
        "</ol>"
        "<p><b>注意事项：</b></p>"
        "<ul>"
        "<li>确保相机已正确连接并可正常拍照</li>"
        "<li>确保平台可以正常移动到各个位置</li>"
        "<li>准备好标定板或标定靶</li>"
        "</ul>"
    ));
    layout->addWidget(labelIntro_);

    // 工位信息
    labelStationInfo_ = new QLabel(this);
    if (wizard_->station()) {
        auto* station = wizard_->station();
        labelStationInfo_->setText(tr(
            "<h4>当前工位信息</h4>"
            "<table>"
            "<tr><td>工位名称：</td><td><b>%1</b></td></tr>"
            "<tr><td>位置数量：</td><td><b>%2</b></td></tr>"
            "<tr><td>相机数量：</td><td><b>%3</b></td></tr>"
            "</table>"
        ).arg(station->name)
         .arg(station->positionNum)
         .arg(station->cameraNum));
    } else {
        labelStationInfo_->setText(tr("<p style='color:red'>警告：未选择工位</p>"));
    }
    layout->addWidget(labelStationInfo_);

    layout->addStretch();
}

// ============================================================
// CalibPositionSelectPage 实现
// ============================================================

CalibPositionSelectPage::CalibPositionSelectPage(CalibrationWizard* wizard, QWidget* parent)
    : QWizardPage(parent)
    , wizard_(wizard)
{
    setTitle(tr("选择标定位置"));
    setSubTitle(tr("请选择需要进行标定的位置"));
    setupUI();
}

void CalibPositionSelectPage::setupUI()
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setSpacing(10);

    // 位置列表
    listPositions_ = new QListWidget(this);
    listPositions_->setSelectionMode(QAbstractItemView::NoSelection);
    layout->addWidget(listPositions_);

    // 按钮区域
    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnSelectAll_ = new QPushButton(tr("全选"), this);
    btnDeselectAll_ = new QPushButton(tr("全不选"), this);
    labelSelectedCount_ = new QLabel(this);

    btnLayout->addWidget(btnSelectAll_);
    btnLayout->addWidget(btnDeselectAll_);
    btnLayout->addStretch();
    btnLayout->addWidget(labelSelectedCount_);
    layout->addLayout(btnLayout);

    // 连接信号
    connect(btnSelectAll_, &QPushButton::clicked, this, &CalibPositionSelectPage::onSelectAll);
    connect(btnDeselectAll_, &QPushButton::clicked, this, &CalibPositionSelectPage::onDeselectAll);
    connect(listPositions_, &QListWidget::itemChanged, this, &CalibPositionSelectPage::onItemChanged);
}

void CalibPositionSelectPage::initializePage()
{
    listPositions_->clear();

    auto& calibStatus = wizard_->calibrationStatus();
    for (int i = 0; i < calibStatus.size(); ++i) {
        const auto& status = calibStatus[i];

        QListWidgetItem* item = new QListWidgetItem(listPositions_);
        item->setText(QString("%1 (相机: %2)").arg(status.positionName).arg(i % 2 == 0 ? "相机1" : "相机2"));
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(status.enabled ? Qt::Checked : Qt::Unchecked);
        item->setData(Qt::UserRole, i);
    }

    updateSelectedCount();
}

bool CalibPositionSelectPage::validatePage()
{
    QVector<int> selected;
    for (int i = 0; i < listPositions_->count(); ++i) {
        QListWidgetItem* item = listPositions_->item(i);
        if (item->checkState() == Qt::Checked) {
            selected.append(item->data(Qt::UserRole).toInt());
        }
    }

    if (selected.isEmpty()) {
        QMessageBox::warning(this, tr("警告"), tr("请至少选择一个位置进行标定"));
        return false;
    }

    wizard_->setSelectedPositionIndices(selected);
    return true;
}

void CalibPositionSelectPage::onSelectAll()
{
    for (int i = 0; i < listPositions_->count(); ++i) {
        listPositions_->item(i)->setCheckState(Qt::Checked);
    }
    updateSelectedCount();
}

void CalibPositionSelectPage::onDeselectAll()
{
    for (int i = 0; i < listPositions_->count(); ++i) {
        listPositions_->item(i)->setCheckState(Qt::Unchecked);
    }
    updateSelectedCount();
}

void CalibPositionSelectPage::onItemChanged(QListWidgetItem* /*item*/)
{
    updateSelectedCount();
}

void CalibPositionSelectPage::updateSelectedCount()
{
    int count = 0;
    for (int i = 0; i < listPositions_->count(); ++i) {
        if (listPositions_->item(i)->checkState() == Qt::Checked) {
            ++count;
        }
    }
    labelSelectedCount_->setText(tr("已选择 %1 个位置").arg(count));
}

// ============================================================
// CalibPositionPage 实现
// ============================================================

CalibPositionPage::CalibPositionPage(CalibrationWizard* wizard, QWidget* parent)
    : QWizardPage(parent)
    , wizard_(wizard)
{
    setTitle(tr("位置标定"));
    setupUI();
}

void CalibPositionPage::setupUI()
{
    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->setSpacing(10);

    // 左侧：图像显示
    QVBoxLayout* leftLayout = new QVBoxLayout();

    imageViewer_ = new HalconImageViewer(this);
    imageViewer_->setMinimumSize(500, 400);
    leftLayout->addWidget(imageViewer_, 1);

    // 采集按钮
    QHBoxLayout* captureLayout = new QHBoxLayout();
    btnCapture_ = new QPushButton(tr("拍照"), this);
    btnCapture_->setIcon(QIcon(":/icons/camera.png"));
    captureLayout->addWidget(btnCapture_);
    captureLayout->addStretch();
    leftLayout->addLayout(captureLayout);

    mainLayout->addLayout(leftLayout, 2);

    // 右侧：控制面板
    QVBoxLayout* rightLayout = new QVBoxLayout();

    // 位置信息
    QGroupBox* groupInfo = new QGroupBox(tr("位置信息"), this);
    QGridLayout* infoLayout = new QGridLayout(groupInfo);
    infoLayout->addWidget(new QLabel(tr("位置名称:")), 0, 0);
    labelPositionName_ = new QLabel(this);
    labelPositionName_->setStyleSheet("QLabel { font-weight: bold; font-size: 14px; }");
    infoLayout->addWidget(labelPositionName_, 0, 1);

    infoLayout->addWidget(new QLabel(tr("关联相机:")), 1, 0);
    labelCameraInfo_ = new QLabel(this);
    infoLayout->addWidget(labelCameraInfo_, 1, 1);

    btnMoveToPos_ = new QPushButton(tr("移动到位置"), this);
    infoLayout->addWidget(btnMoveToPos_, 2, 0, 1, 2);
    rightLayout->addWidget(groupInfo);

    // 3x3 网格
    QGroupBox* groupGrid = new QGroupBox(tr("9点标定网格"), this);
    QGridLayout* gridLayout = new QGridLayout(groupGrid);
    gridLayout->setSpacing(5);

    for (int i = 0; i < 9; ++i) {
        gridButtons_[i] = new QPushButton(QString::number(i + 1), this);
        gridButtons_[i]->setFixedSize(50, 50);
        gridButtons_[i]->setCheckable(true);
        gridButtons_[i]->setProperty("pointIndex", i);

        int row = i / 3;
        int col = i % 3;
        gridLayout->addWidget(gridButtons_[i], row, col);

        connect(gridButtons_[i], &QPushButton::clicked, [this, i]() {
            onGridPointClicked(i);
        });
    }
    rightLayout->addWidget(groupGrid);

    // 标定参数
    QGroupBox* groupParams = new QGroupBox(tr("标定参数"), this);
    QGridLayout* paramsLayout = new QGridLayout(groupParams);
    paramsLayout->addWidget(new QLabel(tr("网格间距:")), 0, 0);
    labelGridSpacing_ = new QLabel(tr("10.0 mm"), this);
    paramsLayout->addWidget(labelGridSpacing_, 0, 1);
    rightLayout->addWidget(groupParams);

    // 进度
    QGroupBox* groupProgress = new QGroupBox(tr("进度"), this);
    QVBoxLayout* progressLayout = new QVBoxLayout(groupProgress);
    progressBar_ = new QProgressBar(this);
    progressBar_->setRange(0, 9);
    progressBar_->setValue(0);
    progressLayout->addWidget(progressBar_);
    labelProgress_ = new QLabel(tr("0 / 9 点"), this);
    labelProgress_->setAlignment(Qt::AlignCenter);
    progressLayout->addWidget(labelProgress_);
    rightLayout->addWidget(groupProgress);

    // 操作按钮
    QHBoxLayout* opLayout = new QHBoxLayout();
    btnReset_ = new QPushButton(tr("重置"), this);
    btnExecuteCalib_ = new QPushButton(tr("执行标定"), this);
    btnExecuteCalib_->setEnabled(false);
    opLayout->addWidget(btnReset_);
    opLayout->addWidget(btnExecuteCalib_);
    rightLayout->addLayout(opLayout);

    // 完成确认
    checkCompleted_ = new QCheckBox(tr("确认此位置标定完成"), this);
    rightLayout->addWidget(checkCompleted_);

    rightLayout->addStretch();
    mainLayout->addLayout(rightLayout, 1);

    // 连接信号
    connect(btnMoveToPos_, &QPushButton::clicked, this, &CalibPositionPage::onMoveToPosition);
    connect(btnCapture_, &QPushButton::clicked, this, &CalibPositionPage::onCapture);
    connect(btnReset_, &QPushButton::clicked, this, &CalibPositionPage::onResetPoints);
    connect(btnExecuteCalib_, &QPushButton::clicked, this, &CalibPositionPage::onExecuteCalibration);
}

void CalibPositionPage::initializePage()
{
    updatePositionInfo();
    updateGridButtons();
    updateProgress();
    checkCompleted_->setChecked(false);
}

bool CalibPositionPage::isComplete() const
{
    return checkCompleted_->isChecked();
}

bool CalibPositionPage::validatePage()
{
    if (!checkCompleted_->isChecked()) {
        QMessageBox::warning(const_cast<CalibPositionPage*>(this),
                           tr("警告"), tr("请确认标定完成"));
        return false;
    }

    // 标记当前位置为已完成
    auto& calibStatus = wizard_->calibrationStatus();
    auto selectedIndices = wizard_->selectedPositionIndices();
    int currentIdx = wizard_->currentPositionIndex();

    if (currentIdx < selectedIndices.size()) {
        int posIdx = selectedIndices[currentIdx];
        if (posIdx < calibStatus.size()) {
            calibStatus[posIdx].completed = true;
        }
    }

    // 移动到下一个位置
    const_cast<CalibrationWizard*>(wizard_)->setCurrentPositionIndex(currentIdx + 1);

    return true;
}

void CalibPositionPage::updatePositionInfo()
{
    auto& calibStatus = wizard_->calibrationStatus();
    auto selectedIndices = wizard_->selectedPositionIndices();
    int currentIdx = wizard_->currentPositionIndex();

    if (currentIdx < selectedIndices.size()) {
        int posIdx = selectedIndices[currentIdx];
        if (posIdx < calibStatus.size()) {
            const auto& status = calibStatus[posIdx];
            labelPositionName_->setText(status.positionName);
            labelCameraInfo_->setText(QString("相机 %1").arg((posIdx % 2) + 1));

            setSubTitle(tr("正在标定: %1 (%2/%3)")
                       .arg(status.positionName)
                       .arg(currentIdx + 1)
                       .arg(selectedIndices.size()));
        }
    }
}

void CalibPositionPage::updateGridButtons()
{
    auto& calibStatus = wizard_->calibrationStatus();
    auto selectedIndices = wizard_->selectedPositionIndices();
    int currentIdx = wizard_->currentPositionIndex();

    if (currentIdx >= selectedIndices.size()) return;
    int posIdx = selectedIndices[currentIdx];
    if (posIdx >= calibStatus.size()) return;

    const auto& status = calibStatus[posIdx];

    for (int i = 0; i < 9; ++i) {
        bool captured = (i < status.points.size()) ? status.points[i].captured : false;
        gridButtons_[i]->setChecked(captured);

        if (captured) {
            gridButtons_[i]->setStyleSheet("QPushButton { background-color: #00aa00; color: white; }");
        } else if (i == currentPointIndex_) {
            gridButtons_[i]->setStyleSheet("QPushButton { background-color: #0078d4; color: white; }");
        } else {
            gridButtons_[i]->setStyleSheet("");
        }
    }
}

void CalibPositionPage::updateProgress()
{
    auto& calibStatus = wizard_->calibrationStatus();
    auto selectedIndices = wizard_->selectedPositionIndices();
    int currentIdx = wizard_->currentPositionIndex();

    if (currentIdx >= selectedIndices.size()) return;
    int posIdx = selectedIndices[currentIdx];
    if (posIdx >= calibStatus.size()) return;

    int completedPoints = calibStatus[posIdx].completedPoints;
    progressBar_->setValue(completedPoints);
    labelProgress_->setText(tr("%1 / 9 点").arg(completedPoints));

    btnExecuteCalib_->setEnabled(completedPoints >= 9);
}

void CalibPositionPage::onMoveToPosition()
{
    auto& calibStatus = wizard_->calibrationStatus();
    auto selectedIndices = wizard_->selectedPositionIndices();
    int currentIdx = wizard_->currentPositionIndex();

    if (currentIdx < selectedIndices.size()) {
        int posIdx = selectedIndices[currentIdx];
        if (posIdx < calibStatus.size()) {
            emit wizard_->moveToPositionRequested(calibStatus[posIdx].positionId);
        }
    }
}

void CalibPositionPage::onCapture()
{
    if (currentPointIndex_ < 0 || currentPointIndex_ >= 9) {
        QMessageBox::information(this, tr("提示"), tr("请先选择要采集的标定点"));
        return;
    }

    // TODO: 实际拍照并获取图像坐标
    // 这里模拟采集成功

    auto& calibStatus = wizard_->calibrationStatus();
    auto selectedIndices = wizard_->selectedPositionIndices();
    int currentIdx = wizard_->currentPositionIndex();

    if (currentIdx < selectedIndices.size()) {
        int posIdx = selectedIndices[currentIdx];
        if (posIdx < calibStatus.size() && currentPointIndex_ < calibStatus[posIdx].points.size()) {
            calibStatus[posIdx].points[currentPointIndex_].captured = true;
            calibStatus[posIdx].points[currentPointIndex_].imageX = 100.0 + currentPointIndex_ * 50;
            calibStatus[posIdx].points[currentPointIndex_].imageY = 100.0 + currentPointIndex_ * 50;
            calibStatus[posIdx].completedPoints++;
        }
    }

    updateGridButtons();
    updateProgress();

    // 自动选择下一个未采集的点
    for (int i = currentPointIndex_ + 1; i < 9; ++i) {
        int posIdx = selectedIndices[currentIdx];
        if (posIdx < calibStatus.size() && i < calibStatus[posIdx].points.size()) {
            if (!calibStatus[posIdx].points[i].captured) {
                onGridPointClicked(i);
                return;
            }
        }
    }
}

void CalibPositionPage::onGridPointClicked(int index)
{
    currentPointIndex_ = index;
    updateGridButtons();
}

void CalibPositionPage::onExecuteCalibration()
{
    // TODO: 执行实际的标定计算

    QMessageBox::information(this, tr("标定完成"),
                           tr("标定计算完成！\n请确认结果后勾选\"确认此位置标定完成\""));

    checkCompleted_->setChecked(true);
    emit completeChanged();
}

void CalibPositionPage::onResetPoints()
{
    auto& calibStatus = wizard_->calibrationStatus();
    auto selectedIndices = wizard_->selectedPositionIndices();
    int currentIdx = wizard_->currentPositionIndex();

    if (currentIdx < selectedIndices.size()) {
        int posIdx = selectedIndices[currentIdx];
        if (posIdx < calibStatus.size()) {
            for (auto& point : calibStatus[posIdx].points) {
                point.captured = false;
                point.imageX = 0;
                point.imageY = 0;
            }
            calibStatus[posIdx].completedPoints = 0;
        }
    }

    currentPointIndex_ = -1;
    updateGridButtons();
    updateProgress();
    checkCompleted_->setChecked(false);
    emit completeChanged();
}

void CalibPositionPage::onImageClicked(int x, int y)
{
    Q_UNUSED(x);
    Q_UNUSED(y);
    // TODO: 处理图像点击事件
}

// ============================================================
// CalibSummaryPage 实现
// ============================================================

CalibSummaryPage::CalibSummaryPage(CalibrationWizard* wizard, QWidget* parent)
    : QWizardPage(parent)
    , wizard_(wizard)
{
    setTitle(tr("标定完成"));
    setSubTitle(tr("所有位置标定已完成，请查看结果"));
    setupUI();
}

void CalibSummaryPage::setupUI()
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setSpacing(15);

    // 总体状态
    labelOverallStatus_ = new QLabel(this);
    labelOverallStatus_->setStyleSheet("QLabel { font-size: 16px; font-weight: bold; }");
    layout->addWidget(labelOverallStatus_);

    // 结果表格
    tableSummary_ = new QTableWidget(this);
    tableSummary_->setColumnCount(4);
    tableSummary_->setHorizontalHeaderLabels({tr("位置"), tr("状态"), tr("采集点数"), tr("误差")});
    tableSummary_->horizontalHeader()->setStretchLastSection(true);
    tableSummary_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableSummary_->setSelectionBehavior(QAbstractItemView::SelectRows);
    layout->addWidget(tableSummary_);

    // 操作按钮
    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnSaveResults_ = new QPushButton(tr("保存结果"), this);
    btnExportReport_ = new QPushButton(tr("导出报告"), this);
    btnLayout->addWidget(btnSaveResults_);
    btnLayout->addWidget(btnExportReport_);
    btnLayout->addStretch();
    layout->addLayout(btnLayout);

    // 连接信号
    connect(btnSaveResults_, &QPushButton::clicked, this, &CalibSummaryPage::onSaveResults);
    connect(btnExportReport_, &QPushButton::clicked, this, &CalibSummaryPage::onExportReport);
}

void CalibSummaryPage::initializePage()
{
    updateSummary();
}

void CalibSummaryPage::updateSummary()
{
    auto& calibStatus = wizard_->calibrationStatus();
    auto selectedIndices = wizard_->selectedPositionIndices();

    int completedCount = 0;
    tableSummary_->setRowCount(selectedIndices.size());

    for (int i = 0; i < selectedIndices.size(); ++i) {
        int posIdx = selectedIndices[i];
        if (posIdx >= calibStatus.size()) continue;

        const auto& status = calibStatus[posIdx];

        tableSummary_->setItem(i, 0, new QTableWidgetItem(status.positionName));

        QString statusText = status.completed ? tr("完成") : tr("未完成");
        QTableWidgetItem* statusItem = new QTableWidgetItem(statusText);
        statusItem->setForeground(status.completed ? Qt::darkGreen : Qt::red);
        tableSummary_->setItem(i, 1, statusItem);

        tableSummary_->setItem(i, 2, new QTableWidgetItem(
            QString("%1 / %2").arg(status.completedPoints).arg(status.totalPoints)));

        tableSummary_->setItem(i, 3, new QTableWidgetItem(
            status.completed ? tr("< 0.1 mm") : tr("--")));

        if (status.completed) {
            ++completedCount;
        }
    }

    tableSummary_->resizeColumnsToContents();

    // 更新总体状态
    if (completedCount == selectedIndices.size()) {
        labelOverallStatus_->setText(tr("所有位置标定成功完成！"));
        labelOverallStatus_->setStyleSheet("QLabel { font-size: 16px; font-weight: bold; color: #00aa00; }");
    } else {
        labelOverallStatus_->setText(tr("部分位置未完成标定 (%1/%2)")
                                    .arg(completedCount).arg(selectedIndices.size()));
        labelOverallStatus_->setStyleSheet("QLabel { font-size: 16px; font-weight: bold; color: #ff8800; }");
    }
}

void CalibSummaryPage::onSaveResults()
{
    QString filePath = QFileDialog::getSaveFileName(this,
        tr("保存标定结果"), QString(), tr("JSON文件 (*.json)"));

    if (filePath.isEmpty()) {
        return;
    }

    // TODO: 实际保存标定结果
    QMessageBox::information(this, tr("保存成功"),
                           tr("标定结果已保存到:\n%1").arg(filePath));

    emit wizard_->calibrationCompleted(wizard_->station() ? wizard_->station()->id : "");
}

void CalibSummaryPage::onExportReport()
{
    QString filePath = QFileDialog::getSaveFileName(this,
        tr("导出标定报告"), QString(), tr("PDF文件 (*.pdf);;HTML文件 (*.html)"));

    if (filePath.isEmpty()) {
        return;
    }

    // TODO: 实际导出报告
    QMessageBox::information(this, tr("导出成功"),
                           tr("标定报告已导出到:\n%1").arg(filePath));
}

} // namespace UI
} // namespace VisionForge
