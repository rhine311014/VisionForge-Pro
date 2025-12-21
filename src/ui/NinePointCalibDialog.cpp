/**
 * @file NinePointCalibDialog.cpp
 * @brief 九点标定对话框实现（简化版）
 * @details 参考VisionASM设计的向导式标定界面
 */

#include "ui/NinePointCalibDialog.h"
#include "base/Logger.h"
#include "base/ImageMemoryPool.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QFormLayout>
#include <QMessageBox>
#include <QFrame>
#include <opencv2/imgproc.hpp>

namespace VisionForge {
namespace UI {

// 九点标定的标准布局（参考VisionASM）:
// 7 --- 8 --- 1
// |     |     |
// 6 --- 0 --- 2
// |     |     |
// 5 --- 4 --- 3
// 中心点为0，顺时针排列1-8

// 映射到3x3网格的索引 (row, col) -> point index
static const int GRID_TO_POINT[3][3] = {
    {7, 8, 1},  // 第一行
    {6, 0, 2},  // 第二行（中心）
    {5, 4, 3}   // 第三行
};

// 点索引到网格位置的映射
static const int POINT_TO_GRID_ROW[9] = {1, 0, 1, 2, 2, 2, 1, 0, 0};
static const int POINT_TO_GRID_COL[9] = {1, 2, 2, 2, 1, 0, 0, 0, 1};

NinePointCalibDialog::NinePointCalibDialog(QWidget* parent)
    : QDialog(parent)
    , calibTool_(new Algorithm::NinePointCalibTool(this))
    , currentStep_(0)
    , currentPointIndex_(-1)
    , stackedWidget_(nullptr)
    , imageViewer_(nullptr)
{
    setWindowTitle("九点标定向导");
    setMinimumSize(900, 650);
    resize(1000, 700);

    // 初始化指针数组
    for (int i = 0; i < 3; ++i) {
        stepIndicators_[i] = nullptr;
        stepLabels_[i] = nullptr;
    }
    for (int i = 0; i < 9; ++i) {
        gridButtons_[i] = nullptr;
        pointStatusLabels_[i] = nullptr;
    }

    // 设置固定9点
    calibTool_->setPointCount(9);

    createUI();
    updateStepIndicator();
}

NinePointCalibDialog::~NinePointCalibDialog()
{
}

void NinePointCalibDialog::setCurrentImage(const Base::ImageData::Ptr& image)
{
    currentImage_ = image;
    if (imageViewer_ && image) {
        imageViewer_->setImage(image);
        drawCalibPoints();
    }
}

Algorithm::CalibrationResult NinePointCalibDialog::getCalibrationResult() const
{
    return calibTool_->getResult();
}

bool NinePointCalibDialog::isCalibrated() const
{
    return calibTool_->isCalibrated();
}

void NinePointCalibDialog::createUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(15, 15, 15, 15);
    mainLayout->setSpacing(15);

    // ========== 步骤指示器 ==========
    QHBoxLayout* indicatorLayout = new QHBoxLayout();
    indicatorLayout->setSpacing(10);

    QString stepNames[3] = {"1. 设置参数", "2. 采集点位", "3. 标定结果"};
    for (int i = 0; i < 3; ++i) {
        QVBoxLayout* stepLayout = new QVBoxLayout();
        stepLayout->setAlignment(Qt::AlignCenter);

        // 圆形指示器
        stepIndicators_[i] = new QLabel(QString::number(i + 1), this);
        stepIndicators_[i]->setFixedSize(36, 36);
        stepIndicators_[i]->setAlignment(Qt::AlignCenter);
        stepIndicators_[i]->setStyleSheet(
            "QLabel { background-color: #e0e0e0; color: #666; "
            "border-radius: 18px; font-weight: bold; font-size: 14px; }");
        stepLayout->addWidget(stepIndicators_[i], 0, Qt::AlignCenter);

        // 步骤名称
        stepLabels_[i] = new QLabel(stepNames[i], this);
        stepLabels_[i]->setStyleSheet("QLabel { color: #666; font-size: 12px; }");
        stepLayout->addWidget(stepLabels_[i], 0, Qt::AlignCenter);

        indicatorLayout->addLayout(stepLayout);

        // 连接线
        if (i < 2) {
            QFrame* line = new QFrame(this);
            line->setFrameShape(QFrame::HLine);
            line->setFixedWidth(60);
            line->setStyleSheet("QFrame { background-color: #ccc; }");
            indicatorLayout->addWidget(line, 0, Qt::AlignCenter);
        }
    }
    indicatorLayout->addStretch();
    mainLayout->addLayout(indicatorLayout);

    // ========== 内容区域 ==========
    stackedWidget_ = new QStackedWidget(this);
    createStep1_Settings();
    createStep2_Capture();
    createStep3_Result();
    mainLayout->addWidget(stackedWidget_, 1);

    // ========== 导航按钮 ==========
    createNavigationButtons();
    mainLayout->addLayout(static_cast<QHBoxLayout*>(layout()->itemAt(layout()->count() - 1)->layout()));
}

void NinePointCalibDialog::createStep1_Settings()
{
    QWidget* page = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setSpacing(20);

    // 说明文字
    QLabel* descLabel = new QLabel(
        "设置标定网格的物理参数。标定将使用3x3共9个点进行。", this);
    descLabel->setStyleSheet("QLabel { color: #333; font-size: 13px; }");
    descLabel->setWordWrap(true);
    layout->addWidget(descLabel);

    // 参数设置组
    QGroupBox* paramGroup = new QGroupBox("网格参数", page);
    QFormLayout* formLayout = new QFormLayout(paramGroup);
    formLayout->setSpacing(15);

    // 网格间距
    gridSpacingSpin_ = new QDoubleSpinBox(paramGroup);
    gridSpacingSpin_->setRange(0.1, 1000.0);
    gridSpacingSpin_->setValue(50.0);
    gridSpacingSpin_->setSuffix(" mm");
    gridSpacingSpin_->setDecimals(2);
    gridSpacingSpin_->setMinimumWidth(150);
    formLayout->addRow("网格间距:", gridSpacingSpin_);

    // 原点坐标
    QHBoxLayout* originLayout = new QHBoxLayout();
    originXSpin_ = new QDoubleSpinBox(paramGroup);
    originXSpin_->setRange(-10000.0, 10000.0);
    originXSpin_->setValue(0.0);
    originXSpin_->setSuffix(" mm");
    originXSpin_->setDecimals(2);
    originYSpin_ = new QDoubleSpinBox(paramGroup);
    originYSpin_->setRange(-10000.0, 10000.0);
    originYSpin_->setValue(0.0);
    originYSpin_->setSuffix(" mm");
    originYSpin_->setDecimals(2);
    originLayout->addWidget(new QLabel("X:", paramGroup));
    originLayout->addWidget(originXSpin_);
    originLayout->addSpacing(20);
    originLayout->addWidget(new QLabel("Y:", paramGroup));
    originLayout->addWidget(originYSpin_);
    originLayout->addStretch();
    formLayout->addRow("原点坐标:", originLayout);

    layout->addWidget(paramGroup);

    // 网格预览图
    QGroupBox* previewGroup = new QGroupBox("标定点布局预览", page);
    QGridLayout* gridPreview = new QGridLayout(previewGroup);
    gridPreview->setSpacing(5);

    // 显示3x3网格布局说明
    QString positions[3][3] = {
        {"7", "8", "1"},
        {"6", "0(中心)", "2"},
        {"5", "4", "3"}
    };
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            QLabel* label = new QLabel(positions[row][col], previewGroup);
            label->setFixedSize(80, 40);
            label->setAlignment(Qt::AlignCenter);
            label->setStyleSheet(
                "QLabel { background-color: #f0f0f0; border: 1px solid #ccc; "
                "border-radius: 4px; font-weight: bold; }");
            gridPreview->addWidget(label, row, col);
        }
    }
    layout->addWidget(previewGroup);

    layout->addStretch();
    stackedWidget_->addWidget(page);
}

void NinePointCalibDialog::createStep2_Capture()
{
    QWidget* page = new QWidget(this);
    QHBoxLayout* layout = new QHBoxLayout(page);
    layout->setSpacing(15);

    // 左侧：图像显示
    QVBoxLayout* imageLayout = new QVBoxLayout();
    captureInstructionLabel_ = new QLabel("点击右侧网格按钮选择要采集的点，然后在图像上点击对应位置", this);
    captureInstructionLabel_->setStyleSheet(
        "QLabel { color: #1976D2; font-size: 12px; padding: 8px; "
        "background-color: #E3F2FD; border-radius: 4px; }");
    captureInstructionLabel_->setWordWrap(true);
    imageLayout->addWidget(captureInstructionLabel_);

    imageViewer_ = new ImageViewer(page);
    imageViewer_->setMinimumSize(500, 400);
    imageLayout->addWidget(imageViewer_, 1);

    // 连接图像点击信号
    connect(imageViewer_, &ImageViewer::mouseClicked,
            this, &NinePointCalibDialog::onImageClicked);

    layout->addLayout(imageLayout, 2);

    // 右侧：3x3网格按钮
    QVBoxLayout* gridLayout = new QVBoxLayout();

    QGroupBox* gridGroup = new QGroupBox("采集点位", page);
    QGridLayout* buttonGrid = new QGridLayout(gridGroup);
    buttonGrid->setSpacing(8);

    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            int pointIndex = GRID_TO_POINT[row][col];

            QVBoxLayout* cellLayout = new QVBoxLayout();

            // 点按钮
            gridButtons_[pointIndex] = new QPushButton(QString::number(pointIndex), gridGroup);
            gridButtons_[pointIndex]->setFixedSize(60, 60);
            gridButtons_[pointIndex]->setStyleSheet(
                "QPushButton { background-color: #f5f5f5; border: 2px solid #ccc; "
                "border-radius: 8px; font-size: 16px; font-weight: bold; }"
                "QPushButton:hover { background-color: #e0e0e0; }"
                "QPushButton:checked { background-color: #2196F3; color: white; border-color: #1976D2; }");
            gridButtons_[pointIndex]->setCheckable(true);

            // 连接点击信号
            connect(gridButtons_[pointIndex], &QPushButton::clicked, this,
                    [this, pointIndex]() { onGridPointClicked(pointIndex); });

            cellLayout->addWidget(gridButtons_[pointIndex], 0, Qt::AlignCenter);

            // 状态标签
            pointStatusLabels_[pointIndex] = new QLabel("未采集", gridGroup);
            pointStatusLabels_[pointIndex]->setAlignment(Qt::AlignCenter);
            pointStatusLabels_[pointIndex]->setStyleSheet("QLabel { color: #999; font-size: 10px; }");
            cellLayout->addWidget(pointStatusLabels_[pointIndex], 0, Qt::AlignCenter);

            buttonGrid->addLayout(cellLayout, row, col);
        }
    }

    gridLayout->addWidget(gridGroup);

    // 重置按钮
    QPushButton* resetBtn = new QPushButton("重置所有点", page);
    resetBtn->setStyleSheet(
        "QPushButton { background-color: #ff5722; color: white; padding: 8px; "
        "border-radius: 4px; }"
        "QPushButton:hover { background-color: #e64a19; }");
    connect(resetBtn, &QPushButton::clicked, this, &NinePointCalibDialog::onResetCalibration);
    gridLayout->addWidget(resetBtn);

    // 采集进度
    QLabel* progressLabel = new QLabel("已采集: 0/9 点", page);
    progressLabel->setObjectName("progressLabel");
    progressLabel->setStyleSheet("QLabel { font-size: 13px; color: #666; }");
    gridLayout->addWidget(progressLabel);

    gridLayout->addStretch();
    layout->addLayout(gridLayout, 1);

    stackedWidget_->addWidget(page);
}

void NinePointCalibDialog::createStep3_Result()
{
    QWidget* page = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setSpacing(20);

    // 结果状态
    resultStatusLabel_ = new QLabel("尚未执行标定", this);
    resultStatusLabel_->setStyleSheet(
        "QLabel { font-size: 16px; font-weight: bold; padding: 15px; "
        "background-color: #f5f5f5; border-radius: 8px; }");
    resultStatusLabel_->setAlignment(Qt::AlignCenter);
    layout->addWidget(resultStatusLabel_);

    // 标定结果详情
    QGroupBox* resultGroup = new QGroupBox("标定结果", page);
    QFormLayout* formLayout = new QFormLayout(resultGroup);
    formLayout->setSpacing(12);

    pixelScaleLabel_ = new QLabel("--", resultGroup);
    pixelScaleLabel_->setStyleSheet("QLabel { font-size: 14px; font-weight: bold; }");
    formLayout->addRow("像素比例 (mm/pixel):", pixelScaleLabel_);

    rotationLabel_ = new QLabel("--", resultGroup);
    rotationLabel_->setStyleSheet("QLabel { font-size: 14px; font-weight: bold; }");
    formLayout->addRow("坐标系旋转角度:", rotationLabel_);

    calibErrorLabel_ = new QLabel("--", resultGroup);
    calibErrorLabel_->setStyleSheet("QLabel { font-size: 14px; font-weight: bold; }");
    formLayout->addRow("标定误差 (mm):", calibErrorLabel_);

    layout->addWidget(resultGroup);

    // 标定按钮
    calibrateBtn_ = new QPushButton("执行标定", page);
    calibrateBtn_->setMinimumHeight(50);
    calibrateBtn_->setStyleSheet(
        "QPushButton { background-color: #4CAF50; color: white; font-size: 16px; "
        "font-weight: bold; border-radius: 8px; }"
        "QPushButton:hover { background-color: #43A047; }"
        "QPushButton:disabled { background-color: #ccc; }");
    connect(calibrateBtn_, &QPushButton::clicked, this, &NinePointCalibDialog::onExecuteCalibration);
    layout->addWidget(calibrateBtn_);

    layout->addStretch();
    stackedWidget_->addWidget(page);
}

void NinePointCalibDialog::createNavigationButtons()
{
    QHBoxLayout* btnLayout = new QHBoxLayout();

    prevBtn_ = new QPushButton("上一步", this);
    prevBtn_->setMinimumWidth(100);
    prevBtn_->setStyleSheet(
        "QPushButton { padding: 10px 20px; border: 1px solid #ccc; border-radius: 4px; }"
        "QPushButton:hover { background-color: #f0f0f0; }");
    connect(prevBtn_, &QPushButton::clicked, this, &NinePointCalibDialog::onPrevStep);

    nextBtn_ = new QPushButton("下一步", this);
    nextBtn_->setMinimumWidth(100);
    nextBtn_->setStyleSheet(
        "QPushButton { padding: 10px 20px; background-color: #2196F3; color: white; "
        "border-radius: 4px; }"
        "QPushButton:hover { background-color: #1976D2; }");
    connect(nextBtn_, &QPushButton::clicked, this, &NinePointCalibDialog::onNextStep);

    btnLayout->addWidget(prevBtn_);
    btnLayout->addStretch();

    cancelBtn_ = new QPushButton("取消", this);
    cancelBtn_->setMinimumWidth(80);
    connect(cancelBtn_, &QPushButton::clicked, this, &NinePointCalibDialog::onCancelClicked);
    btnLayout->addWidget(cancelBtn_);

    okBtn_ = new QPushButton("完成", this);
    okBtn_->setMinimumWidth(80);
    okBtn_->setStyleSheet(
        "QPushButton { padding: 10px 20px; background-color: #4CAF50; color: white; "
        "border-radius: 4px; }"
        "QPushButton:hover { background-color: #43A047; }");
    okBtn_->setEnabled(false);
    connect(okBtn_, &QPushButton::clicked, this, &NinePointCalibDialog::onOkClicked);
    btnLayout->addWidget(okBtn_);

    btnLayout->addWidget(nextBtn_);

    static_cast<QVBoxLayout*>(layout())->addLayout(btnLayout);
}

void NinePointCalibDialog::updateStepIndicator()
{
    for (int i = 0; i < 3; ++i) {
        if (i < currentStep_) {
            // 已完成
            stepIndicators_[i]->setStyleSheet(
                "QLabel { background-color: #4CAF50; color: white; "
                "border-radius: 18px; font-weight: bold; font-size: 14px; }");
            stepIndicators_[i]->setText("✓");
        } else if (i == currentStep_) {
            // 当前步骤
            stepIndicators_[i]->setStyleSheet(
                "QLabel { background-color: #2196F3; color: white; "
                "border-radius: 18px; font-weight: bold; font-size: 14px; }");
            stepIndicators_[i]->setText(QString::number(i + 1));
        } else {
            // 未到达
            stepIndicators_[i]->setStyleSheet(
                "QLabel { background-color: #e0e0e0; color: #666; "
                "border-radius: 18px; font-weight: bold; font-size: 14px; }");
            stepIndicators_[i]->setText(QString::number(i + 1));
        }
    }

    // 更新按钮状态
    prevBtn_->setEnabled(currentStep_ > 0);
    nextBtn_->setVisible(currentStep_ < 2);
    okBtn_->setVisible(currentStep_ == 2);
}

void NinePointCalibDialog::updateGridButtons()
{
    int collectedCount = 0;

    for (int i = 0; i < 9; ++i) {
        auto pt = calibTool_->getCalibPoint(i);
        bool isCollected = pt.valid;

        if (isCollected) {
            collectedCount++;
            gridButtons_[i]->setStyleSheet(
                "QPushButton { background-color: #4CAF50; color: white; border: 2px solid #388E3C; "
                "border-radius: 8px; font-size: 16px; font-weight: bold; }"
                "QPushButton:hover { background-color: #43A047; }");
            pointStatusLabels_[i]->setText(QString("(%1, %2)")
                .arg(pt.imagePos.x, 0, 'f', 0).arg(pt.imagePos.y, 0, 'f', 0));
            pointStatusLabels_[i]->setStyleSheet("QLabel { color: #4CAF50; font-size: 10px; }");
        } else if (i == currentPointIndex_) {
            gridButtons_[i]->setStyleSheet(
                "QPushButton { background-color: #2196F3; color: white; border: 2px solid #1976D2; "
                "border-radius: 8px; font-size: 16px; font-weight: bold; }"
                "QPushButton:hover { background-color: #1976D2; }");
            pointStatusLabels_[i]->setText("等待采集");
            pointStatusLabels_[i]->setStyleSheet("QLabel { color: #2196F3; font-size: 10px; }");
        } else {
            gridButtons_[i]->setStyleSheet(
                "QPushButton { background-color: #f5f5f5; border: 2px solid #ccc; "
                "border-radius: 8px; font-size: 16px; font-weight: bold; }"
                "QPushButton:hover { background-color: #e0e0e0; }");
            pointStatusLabels_[i]->setText("未采集");
            pointStatusLabels_[i]->setStyleSheet("QLabel { color: #999; font-size: 10px; }");
        }
    }

    // 更新进度标签
    QLabel* progressLabel = stackedWidget_->widget(1)->findChild<QLabel*>("progressLabel");
    if (progressLabel) {
        progressLabel->setText(QString("已采集: %1/9 点").arg(collectedCount));
        if (collectedCount == 9) {
            progressLabel->setStyleSheet("QLabel { font-size: 13px; color: #4CAF50; font-weight: bold; }");
        } else {
            progressLabel->setStyleSheet("QLabel { font-size: 13px; color: #666; }");
        }
    }
}

void NinePointCalibDialog::updateResultDisplay()
{
    if (calibTool_->isCalibrated()) {
        const auto& result = calibTool_->getResult();

        resultStatusLabel_->setText("标定成功");
        resultStatusLabel_->setStyleSheet(
            "QLabel { font-size: 16px; font-weight: bold; padding: 15px; "
            "background-color: #E8F5E9; color: #4CAF50; border-radius: 8px; }");

        double scaleX = result.pixelToMMX();
        double scaleY = result.pixelToMMY();
        pixelScaleLabel_->setText(QString("X: %1, Y: %2").arg(scaleX, 0, 'f', 6).arg(scaleY, 0, 'f', 6));

        rotationLabel_->setText(QString("%1°").arg(calibTool_->rotationAngleDegrees(), 0, 'f', 2));

        double error = result.calibrationError();
        calibErrorLabel_->setText(QString("%1").arg(error, 0, 'f', 4));
        if (error < 0.1) {
            calibErrorLabel_->setStyleSheet("QLabel { font-size: 14px; font-weight: bold; color: #4CAF50; }");
        } else if (error < 0.5) {
            calibErrorLabel_->setStyleSheet("QLabel { font-size: 14px; font-weight: bold; color: #FF9800; }");
        } else {
            calibErrorLabel_->setStyleSheet("QLabel { font-size: 14px; font-weight: bold; color: #F44336; }");
        }

        okBtn_->setEnabled(true);
    } else {
        resultStatusLabel_->setText("尚未执行标定");
        resultStatusLabel_->setStyleSheet(
            "QLabel { font-size: 16px; font-weight: bold; padding: 15px; "
            "background-color: #f5f5f5; border-radius: 8px; }");
        pixelScaleLabel_->setText("--");
        rotationLabel_->setText("--");
        calibErrorLabel_->setText("--");
        calibErrorLabel_->setStyleSheet("QLabel { font-size: 14px; font-weight: bold; }");
        okBtn_->setEnabled(false);
    }
}

void NinePointCalibDialog::drawCalibPoints()
{
    if (!currentImage_ || currentImage_->isEmpty()) return;

    cv::Mat display;
    if (currentImage_->mat().channels() == 1) {
        cv::cvtColor(currentImage_->mat(), display, cv::COLOR_GRAY2BGR);
    } else {
        display = currentImage_->mat().clone();
    }

    // 绘制标定点
    for (int i = 0; i < 9; ++i) {
        auto pt = calibTool_->getCalibPoint(i);
        if (pt.valid) {
            cv::Point center(static_cast<int>(pt.imagePos.x), static_cast<int>(pt.imagePos.y));

            // 颜色：已采集=绿色，当前选中=蓝色
            cv::Scalar color = (i == currentPointIndex_) ?
                cv::Scalar(255, 165, 0) : cv::Scalar(0, 255, 0);

            // 绘制十字 + 圆圈
            cv::circle(display, center, 12, color, 2);
            cv::line(display, cv::Point(center.x - 18, center.y),
                     cv::Point(center.x + 18, center.y), color, 2);
            cv::line(display, cv::Point(center.x, center.y - 18),
                     cv::Point(center.x, center.y + 18), color, 2);

            // 绘制点编号
            cv::putText(display, std::to_string(i),
                        cv::Point(center.x + 15, center.y - 15),
                        cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 255, 255), 2);
        }
    }

    // 如果当前有选中点但未采集，显示十字光标提示
    if (currentPointIndex_ >= 0 && currentPointIndex_ < 9) {
        auto pt = calibTool_->getCalibPoint(currentPointIndex_);
        if (!pt.valid) {
            // 在图像顶部显示待采集提示
            int cx = display.cols / 2;
            cv::putText(display, QString("Click to set Point %1").arg(currentPointIndex_).toStdString(),
                        cv::Point(cx - 100, 30),
                        cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 165, 255), 2);
        }
    }

    // 使用内存池分配图像
    auto imageData = Base::ImageMemoryPool::instance().allocate(
        display.cols, display.rows, display.type());
    if (imageData) {
        display.copyTo(imageData->mat());
        imageViewer_->setImage(imageData);
    }
}

void NinePointCalibDialog::goToStep(int step)
{
    if (step < 0 || step > 2) return;

    // 离开当前步骤前的处理
    if (currentStep_ == 0 && step == 1) {
        // 从步骤1进入步骤2时，生成网格坐标
        generateGridCoordinates();
    }

    currentStep_ = step;
    stackedWidget_->setCurrentIndex(step);
    updateStepIndicator();

    // 进入步骤2时更新网格按钮
    if (step == 1) {
        updateGridButtons();
        drawCalibPoints();
    }

    // 进入步骤3时更新结果显示
    if (step == 2) {
        updateResultDisplay();
    }
}

void NinePointCalibDialog::generateGridCoordinates()
{
    double spacing = gridSpacingSpin_->value();
    double originX = originXSpin_->value();
    double originY = originYSpin_->value();

    // 生成3x3网格的物理坐标
    // 按照VisionASM的点位布局:
    // 点0在中心，点1-8围绕中心
    for (int i = 0; i < 9; ++i) {
        int gridRow = POINT_TO_GRID_ROW[i];
        int gridCol = POINT_TO_GRID_COL[i];

        // 计算相对于中心的偏移 (中心在row=1, col=1)
        double offsetX = (gridCol - 1) * spacing;
        double offsetY = (1 - gridRow) * spacing;  // Y轴向上为正

        cv::Point2d worldPos(originX + offsetX, originY + offsetY);
        calibTool_->setWorldPoint(i, worldPos);
    }

    LOG_INFO(QString("已生成9点网格坐标, 间距: %1mm, 原点: (%2, %3)")
             .arg(spacing).arg(originX).arg(originY));
}

// ========== 槽函数 ==========

void NinePointCalibDialog::onNextStep()
{
    if (currentStep_ < 2) {
        goToStep(currentStep_ + 1);
    }
}

void NinePointCalibDialog::onPrevStep()
{
    if (currentStep_ > 0) {
        goToStep(currentStep_ - 1);
    }
}

void NinePointCalibDialog::onGridPointClicked(int index)
{
    if (index < 0 || index >= 9) return;

    currentPointIndex_ = index;
    captureInstructionLabel_->setText(
        QString("已选择点 %1，请在图像上点击对应位置进行采集").arg(index));

    updateGridButtons();
    drawCalibPoints();
}

void NinePointCalibDialog::onImageClicked(int x, int y)
{
    if (currentPointIndex_ < 0 || currentPointIndex_ >= 9) {
        QMessageBox::information(this, "提示", "请先在右侧网格中点击选择要采集的点");
        return;
    }

    cv::Point2d imagePos(x, y);
    calibTool_->setImagePoint(currentPointIndex_, imagePos);
    calibTool_->setPointValid(currentPointIndex_, true);

    LOG_INFO(QString("采集点 %1 图像坐标: (%2, %3)")
             .arg(currentPointIndex_).arg(x).arg(y));

    updateGridButtons();
    drawCalibPoints();

    // 自动选择下一个未采集的点
    int nextPoint = -1;
    for (int i = 0; i < 9; ++i) {
        int checkIndex = (currentPointIndex_ + 1 + i) % 9;
        auto pt = calibTool_->getCalibPoint(checkIndex);
        if (!pt.valid) {
            nextPoint = checkIndex;
            break;
        }
    }

    if (nextPoint >= 0) {
        currentPointIndex_ = nextPoint;
        captureInstructionLabel_->setText(
            QString("已选择点 %1，请在图像上点击对应位置进行采集").arg(nextPoint));
        updateGridButtons();
    } else {
        currentPointIndex_ = -1;
        captureInstructionLabel_->setText("所有点已采集完成，请点击下一步执行标定");
        captureInstructionLabel_->setStyleSheet(
            "QLabel { color: #4CAF50; font-size: 12px; padding: 8px; "
            "background-color: #E8F5E9; border-radius: 4px; }");
    }
}

void NinePointCalibDialog::onExecuteCalibration()
{
    int validCount = calibTool_->validPointCount();
    if (validCount < 4) {
        QMessageBox::warning(this, "警告",
            QString("有效标定点不足，需要至少4个，当前%1个").arg(validCount));
        return;
    }

    calibTool_->setCalibMode(Algorithm::NinePointCalibMode::Homography);

    Algorithm::CalibrationResult result;
    bool success = calibTool_->calibrate(result);

    if (success) {
        updateResultDisplay();
        QMessageBox::information(this, "标定完成",
            QString("九点标定成功!\n标定误差: %1 mm").arg(result.calibrationError(), 0, 'f', 4));
    } else {
        QMessageBox::warning(this, "标定失败", "标定计算失败，请检查采集的点位数据");
    }
}

void NinePointCalibDialog::onResetCalibration()
{
    calibTool_->clearCalibPoints();
    currentPointIndex_ = 0;
    captureInstructionLabel_->setText("点击右侧网格按钮选择要采集的点，然后在图像上点击对应位置");
    captureInstructionLabel_->setStyleSheet(
        "QLabel { color: #1976D2; font-size: 12px; padding: 8px; "
        "background-color: #E3F2FD; border-radius: 4px; }");

    updateGridButtons();
    drawCalibPoints();
}

void NinePointCalibDialog::onOkClicked()
{
    if (calibTool_->isCalibrated()) {
        emit calibrationCompleted(calibTool_->getResult());
    }
    accept();
}

void NinePointCalibDialog::onCancelClicked()
{
    reject();
}

} // namespace UI
} // namespace VisionForge
