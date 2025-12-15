/**
 * @file ShapeMatchParamPanel.cpp
 * @brief 形状匹配参数面板实现
 */

#include "ui/ShapeMatchParamPanel.h"
#include "algorithm/ShapeMatchTool.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <cmath>

namespace VisionForge {
namespace UI {

ShapeMatchParamPanel::ShapeMatchParamPanel(Algorithm::ShapeMatchTool* tool, QWidget* parent)
    : QWidget(parent)
    , tool_(tool)
{
    createUI();
    connectSignals();
    updateUI();
}

void ShapeMatchParamPanel::createUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(10);

    // ========== 模板管理组 ==========
    QGroupBox* modelGroup = new QGroupBox("模板管理", this);
    QVBoxLayout* modelLayout = new QVBoxLayout(modelGroup);

    // 模板路径
    QHBoxLayout* pathLayout = new QHBoxLayout();
    pathLayout->addWidget(new QLabel("模板文件:", this));
    modelPathEdit_ = new QLineEdit(this);
    modelPathEdit_->setReadOnly(true);
    modelPathEdit_->setPlaceholderText("未加载模板");
    pathLayout->addWidget(modelPathEdit_, 1);
    modelLayout->addLayout(pathLayout);

    // 模板状态
    modelStatusLabel_ = new QLabel("状态: 未加载", this);
    modelStatusLabel_->setStyleSheet("QLabel { color: #ff6b6b; font-weight: bold; }");
    modelLayout->addWidget(modelStatusLabel_);

    // 按钮行
    QHBoxLayout* btnLayout = new QHBoxLayout();
    loadModelBtn_ = new QPushButton("加载模板", this);
    saveModelBtn_ = new QPushButton("保存模板", this);
    trainModelBtn_ = new QPushButton("训练模板", this);
    clearModelBtn_ = new QPushButton("清除模板", this);

    saveModelBtn_->setEnabled(false);
    clearModelBtn_->setEnabled(false);

    btnLayout->addWidget(loadModelBtn_);
    btnLayout->addWidget(saveModelBtn_);
    btnLayout->addWidget(trainModelBtn_);
    btnLayout->addWidget(clearModelBtn_);
    modelLayout->addLayout(btnLayout);

    mainLayout->addWidget(modelGroup);

    // ========== 匹配参数组 ==========
    QGroupBox* matchGroup = new QGroupBox("匹配参数", this);
    QGridLayout* matchLayout = new QGridLayout(matchGroup);
    matchLayout->setColumnStretch(1, 1);

    int row = 0;

    // 最小匹配分数
    matchLayout->addWidget(new QLabel("最小分数:", this), row, 0);
    QHBoxLayout* scoreLayout = new QHBoxLayout();
    minScoreSpinBox_ = new QDoubleSpinBox(this);
    minScoreSpinBox_->setRange(0.0, 1.0);
    minScoreSpinBox_->setSingleStep(0.01);
    minScoreSpinBox_->setDecimals(2);
    minScoreSpinBox_->setValue(0.7);
    scoreLayout->addWidget(minScoreSpinBox_);

    minScoreSlider_ = new QSlider(Qt::Horizontal, this);
    minScoreSlider_->setRange(0, 100);
    minScoreSlider_->setValue(70);
    scoreLayout->addWidget(minScoreSlider_, 1);
    matchLayout->addLayout(scoreLayout, row++, 1);

    // 查找数量
    matchLayout->addWidget(new QLabel("查找数量:", this), row, 0);
    numMatchesSpinBox_ = new QSpinBox(this);
    numMatchesSpinBox_->setRange(0, 100);
    numMatchesSpinBox_->setValue(1);
    numMatchesSpinBox_->setSpecialValueText("全部");
    matchLayout->addWidget(numMatchesSpinBox_, row++, 1);

    // 角度范围 - 起始角度
    matchLayout->addWidget(new QLabel("起始角度(°):", this), row, 0);
    angleStartSpinBox_ = new QDoubleSpinBox(this);
    angleStartSpinBox_->setRange(-180.0, 180.0);
    angleStartSpinBox_->setSingleStep(1.0);
    angleStartSpinBox_->setDecimals(1);
    angleStartSpinBox_->setValue(-22.5);
    matchLayout->addWidget(angleStartSpinBox_, row++, 1);

    // 角度范围 - 角度范围
    matchLayout->addWidget(new QLabel("角度范围(°):", this), row, 0);
    angleExtentSpinBox_ = new QDoubleSpinBox(this);
    angleExtentSpinBox_->setRange(0.0, 360.0);
    angleExtentSpinBox_->setSingleStep(1.0);
    angleExtentSpinBox_->setDecimals(1);
    angleExtentSpinBox_->setValue(45.0);
    matchLayout->addWidget(angleExtentSpinBox_, row++, 1);

    // 最小对比度
    matchLayout->addWidget(new QLabel("最小对比度:", this), row, 0);
    minContrastSpinBox_ = new QSpinBox(this);
    minContrastSpinBox_->setRange(0, 255);
    minContrastSpinBox_->setValue(10);
    matchLayout->addWidget(minContrastSpinBox_, row++, 1);

    // 缩放范围 - 最小
    matchLayout->addWidget(new QLabel("最小缩放:", this), row, 0);
    scaleMinSpinBox_ = new QDoubleSpinBox(this);
    scaleMinSpinBox_->setRange(0.1, 10.0);
    scaleMinSpinBox_->setSingleStep(0.1);
    scaleMinSpinBox_->setDecimals(2);
    scaleMinSpinBox_->setValue(0.9);
    matchLayout->addWidget(scaleMinSpinBox_, row++, 1);

    // 缩放范围 - 最大
    matchLayout->addWidget(new QLabel("最大缩放:", this), row, 0);
    scaleMaxSpinBox_ = new QDoubleSpinBox(this);
    scaleMaxSpinBox_->setRange(0.1, 10.0);
    scaleMaxSpinBox_->setSingleStep(0.1);
    scaleMaxSpinBox_->setDecimals(2);
    scaleMaxSpinBox_->setValue(1.1);
    matchLayout->addWidget(scaleMaxSpinBox_, row++, 1);

    mainLayout->addWidget(matchGroup);

    // 添加弹簧
    mainLayout->addStretch();
}

void ShapeMatchParamPanel::connectSignals()
{
    // 匹配参数
    connect(minScoreSpinBox_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &ShapeMatchParamPanel::onMinScoreChanged);
    connect(minScoreSlider_, &QSlider::valueChanged, this, [this](int value) {
        minScoreSpinBox_->setValue(value / 100.0);
    });

    connect(numMatchesSpinBox_, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &ShapeMatchParamPanel::onNumMatchesChanged);

    connect(angleStartSpinBox_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &ShapeMatchParamPanel::onAngleRangeChanged);
    connect(angleExtentSpinBox_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &ShapeMatchParamPanel::onAngleRangeChanged);

    connect(minContrastSpinBox_, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &ShapeMatchParamPanel::onContrastChanged);

    connect(scaleMinSpinBox_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &ShapeMatchParamPanel::onScaleRangeChanged);
    connect(scaleMaxSpinBox_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &ShapeMatchParamPanel::onScaleRangeChanged);

    // 模板管理按钮
    connect(loadModelBtn_, &QPushButton::clicked,
            this, &ShapeMatchParamPanel::onLoadModelClicked);
    connect(saveModelBtn_, &QPushButton::clicked,
            this, &ShapeMatchParamPanel::onSaveModelClicked);
    connect(trainModelBtn_, &QPushButton::clicked,
            this, &ShapeMatchParamPanel::onTrainModelClicked);
    connect(clearModelBtn_, &QPushButton::clicked,
            this, &ShapeMatchParamPanel::onClearModelClicked);
}

void ShapeMatchParamPanel::updateUI()
{
    if (!tool_) return;

    // 更新匹配参数
    minScoreSpinBox_->blockSignals(true);
    minScoreSlider_->blockSignals(true);
    numMatchesSpinBox_->blockSignals(true);
    angleStartSpinBox_->blockSignals(true);
    angleExtentSpinBox_->blockSignals(true);
    minContrastSpinBox_->blockSignals(true);
    scaleMinSpinBox_->blockSignals(true);
    scaleMaxSpinBox_->blockSignals(true);

    minScoreSpinBox_->setValue(tool_->getMinScore());
    minScoreSlider_->setValue(static_cast<int>(tool_->getMinScore() * 100));
    numMatchesSpinBox_->setValue(tool_->getNumMatches());
    angleStartSpinBox_->setValue(tool_->getAngleStart() * 180.0 / M_PI);
    angleExtentSpinBox_->setValue(tool_->getAngleExtent() * 180.0 / M_PI);
    minContrastSpinBox_->setValue(tool_->getMinContrast());
    scaleMinSpinBox_->setValue(tool_->getScaleMin());
    scaleMaxSpinBox_->setValue(tool_->getScaleMax());

    minScoreSpinBox_->blockSignals(false);
    minScoreSlider_->blockSignals(false);
    numMatchesSpinBox_->blockSignals(false);
    angleStartSpinBox_->blockSignals(false);
    angleExtentSpinBox_->blockSignals(false);
    minContrastSpinBox_->blockSignals(false);
    scaleMinSpinBox_->blockSignals(false);
    scaleMaxSpinBox_->blockSignals(false);

    // 更新模板状态
    if (tool_->hasModel()) {
        modelStatusLabel_->setText("状态: 已加载模板");
        modelStatusLabel_->setStyleSheet("QLabel { color: #51cf66; font-weight: bold; }");
        saveModelBtn_->setEnabled(true);
        clearModelBtn_->setEnabled(true);

        QString modelPath = tool_->getModelPath();
        if (!modelPath.isEmpty()) {
            modelPathEdit_->setText(modelPath);
        } else {
            modelPathEdit_->setText("(从ROI训练)");
        }
    } else {
        modelStatusLabel_->setText("状态: 未加载");
        modelStatusLabel_->setStyleSheet("QLabel { color: #ff6b6b; font-weight: bold; }");
        saveModelBtn_->setEnabled(false);
        clearModelBtn_->setEnabled(false);
        modelPathEdit_->clear();
    }
}

void ShapeMatchParamPanel::onMinScoreChanged(double value)
{
    if (!tool_) return;

    minScoreSlider_->blockSignals(true);
    minScoreSlider_->setValue(static_cast<int>(value * 100));
    minScoreSlider_->blockSignals(false);

    tool_->setMinScore(value);
    emit parameterChanged();
}

void ShapeMatchParamPanel::onNumMatchesChanged(int value)
{
    if (!tool_) return;
    tool_->setNumMatches(value);
    emit parameterChanged();
}

void ShapeMatchParamPanel::onAngleRangeChanged()
{
    if (!tool_) return;

    double startDeg = angleStartSpinBox_->value();
    double extentDeg = angleExtentSpinBox_->value();

    // 转换为弧度
    tool_->setAngleStart(startDeg * M_PI / 180.0);
    tool_->setAngleExtent(extentDeg * M_PI / 180.0);

    emit parameterChanged();
}

void ShapeMatchParamPanel::onContrastChanged(int value)
{
    if (!tool_) return;
    tool_->setMinContrast(value);
    emit parameterChanged();
}

void ShapeMatchParamPanel::onScaleRangeChanged()
{
    if (!tool_) return;

    double scaleMin = scaleMinSpinBox_->value();
    double scaleMax = scaleMaxSpinBox_->value();

    if (scaleMin > scaleMax) {
        QMessageBox::warning(this, "参数错误", "最小缩放不能大于最大缩放");
        scaleMinSpinBox_->setValue(scaleMax);
        return;
    }

    tool_->setScaleMin(scaleMin);
    tool_->setScaleMax(scaleMax);

    emit parameterChanged();
}

void ShapeMatchParamPanel::onLoadModelClicked()
{
    if (!tool_) return;

    QString path = getModelPath(false);
    if (path.isEmpty()) {
        return;
    }

    if (tool_->loadModel(path)) {
        QMessageBox::information(this, "成功", "模板加载成功");
        updateUI();
        emit parameterChanged();
    } else {
        QMessageBox::warning(this, "失败", "模板加载失败");
    }
}

void ShapeMatchParamPanel::onSaveModelClicked()
{
    if (!tool_ || !tool_->hasModel()) return;

    QString path = getModelPath(true);
    if (path.isEmpty()) {
        return;
    }

    if (tool_->saveModel(path)) {
        QMessageBox::information(this, "成功", "模板保存成功");
        updateUI();
    } else {
        QMessageBox::warning(this, "失败", "模板保存失败");
    }
}

void ShapeMatchParamPanel::onTrainModelClicked()
{
    // 发送信号给主窗口，让用户在图像上绘制ROI
    emit trainModelRequested();
}

void ShapeMatchParamPanel::onClearModelClicked()
{
    if (!tool_) return;

    int ret = QMessageBox::question(this, "确认",
        "确定要清除当前模板吗？",
        QMessageBox::Yes | QMessageBox::No);

    if (ret == QMessageBox::Yes) {
        tool_->clearModel();
        updateUI();
        emit parameterChanged();
    }
}

QString ShapeMatchParamPanel::getModelPath(bool forSave)
{
    QString filter = "Halcon模板文件 (*.shm);;所有文件 (*.*)";
    QString defaultPath = tool_->getModelPath();

    if (defaultPath.isEmpty()) {
        defaultPath = QDir::homePath();
    }

    QString path;
    if (forSave) {
        path = QFileDialog::getSaveFileName(this, "保存模板", defaultPath, filter);
    } else {
        path = QFileDialog::getOpenFileName(this, "加载模板", defaultPath, filter);
    }

    // 确保扩展名
    if (!path.isEmpty() && !path.endsWith(".shm", Qt::CaseInsensitive)) {
        path += ".shm";
    }

    return path;
}

} // namespace UI
} // namespace VisionForge
