/**
 * @file ShapeMatchParamPanel.cpp
 * @brief 形状匹配参数面板实现
 */

#include "ui/ShapeMatchParamPanel.h"
#include "ui/ShapeModelLibraryDialog.h"
#include "algorithm/ShapeMatchTool.h"
#include "algorithm/ShapeModelManager.h"
#include "base/Logger.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QStandardPaths>
#include <cmath>

namespace VisionForge {
namespace UI {

ShapeMatchParamPanel::ShapeMatchParamPanel(Algorithm::ShapeMatchTool* tool, QWidget* parent)
    : QWidget(parent)
    , tool_(tool)
    , matchTypeCombo_(nullptr)
    , minScoreSpinBox_(nullptr)
    , minScoreSlider_(nullptr)
    , numMatchesSpinBox_(nullptr)
    , angleStartSpinBox_(nullptr)
    , angleExtentSpinBox_(nullptr)
    , minContrastSpinBox_(nullptr)
    , scaleMinSpinBox_(nullptr)
    , scaleMaxSpinBox_(nullptr)
    , scaleStepRowLabel_(nullptr)
    , scaleStepRowSpinBox_(nullptr)
    , scaleStepColLabel_(nullptr)
    , scaleStepColSpinBox_(nullptr)
    , useXLDDisplayCheckBox_(nullptr)
    , modelPathEdit_(nullptr)
    , loadModelBtn_(nullptr)
    , saveModelBtn_(nullptr)
    , trainModelBtn_(nullptr)
    , clearModelBtn_(nullptr)
    , modelLibraryBtn_(nullptr)
    , modelStatusLabel_(nullptr)
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

    // 按钮行1
    QHBoxLayout* btnLayout1 = new QHBoxLayout();
    loadModelBtn_ = new QPushButton("加载模板", this);
    saveModelBtn_ = new QPushButton("保存模板", this);
    trainModelBtn_ = new QPushButton("训练模板", this);

    saveModelBtn_->setEnabled(false);

    btnLayout1->addWidget(loadModelBtn_);
    btnLayout1->addWidget(saveModelBtn_);
    btnLayout1->addWidget(trainModelBtn_);
    modelLayout->addLayout(btnLayout1);

    // 按钮行2
    QHBoxLayout* btnLayout2 = new QHBoxLayout();
    clearModelBtn_ = new QPushButton("清除模板", this);
    modelLibraryBtn_ = new QPushButton("模板库", this);

    clearModelBtn_->setEnabled(false);

    btnLayout2->addWidget(clearModelBtn_);
    btnLayout2->addWidget(modelLibraryBtn_);
    modelLayout->addLayout(btnLayout2);

    mainLayout->addWidget(modelGroup);

    // ========== 匹配参数组 ==========
    QGroupBox* matchGroup = new QGroupBox("匹配参数", this);
    QGridLayout* matchLayout = new QGridLayout(matchGroup);
    matchLayout->setColumnStretch(1, 1);

    int row = 0;

    // 匹配类型
    matchLayout->addWidget(new QLabel("匹配类型:", this), row, 0);
    matchTypeCombo_ = new QComboBox(this);
    matchTypeCombo_->addItem("标准匹配（仅旋转）", 0);  // Standard
    matchTypeCombo_->addItem("缩放匹配（旋转+缩放）", 1);  // Scaled
    matchTypeCombo_->addItem("仿射匹配（旋转+各向异性缩放）", 2);  // Anisotropic
    matchTypeCombo_->setToolTip("选择匹配模式：标准（仅旋转）、缩放（旋转+各向同性缩放）、仿射（旋转+各向异性缩放）");
    matchLayout->addWidget(matchTypeCombo_, row++, 1);

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

    // 缩放步长 - 行方向（仅仿射匹配）
    scaleStepRowLabel_ = new QLabel("行缩放步长:", this);
    matchLayout->addWidget(scaleStepRowLabel_, row, 0);
    scaleStepRowSpinBox_ = new QDoubleSpinBox(this);
    scaleStepRowSpinBox_->setRange(0.5, 1.0);
    scaleStepRowSpinBox_->setSingleStep(0.01);
    scaleStepRowSpinBox_->setDecimals(2);
    scaleStepRowSpinBox_->setValue(0.95);
    scaleStepRowSpinBox_->setToolTip("仿射匹配时行方向的缩放步长，值越接近1搜索越精细但速度越慢");
    matchLayout->addWidget(scaleStepRowSpinBox_, row++, 1);

    // 缩放步长 - 列方向（仅仿射匹配）
    scaleStepColLabel_ = new QLabel("列缩放步长:", this);
    matchLayout->addWidget(scaleStepColLabel_, row, 0);
    scaleStepColSpinBox_ = new QDoubleSpinBox(this);
    scaleStepColSpinBox_->setRange(0.5, 1.0);
    scaleStepColSpinBox_->setSingleStep(0.01);
    scaleStepColSpinBox_->setDecimals(2);
    scaleStepColSpinBox_->setValue(0.95);
    scaleStepColSpinBox_->setToolTip("仿射匹配时列方向的缩放步长，值越接近1搜索越精细但速度越慢");
    matchLayout->addWidget(scaleStepColSpinBox_, row++, 1);

    // 默认隐藏缩放步长参数（仅在仿射匹配时显示）
    scaleStepRowLabel_->setVisible(false);
    scaleStepRowSpinBox_->setVisible(false);
    scaleStepColLabel_->setVisible(false);
    scaleStepColSpinBox_->setVisible(false);

    mainLayout->addWidget(matchGroup);

    // ========== 显示选项组 ==========
    QGroupBox* displayGroup = new QGroupBox("显示选项", this);
    QVBoxLayout* displayLayout = new QVBoxLayout(displayGroup);

    useXLDDisplayCheckBox_ = new QCheckBox("使用XLD轮廓显示（非破坏性）", this);
    useXLDDisplayCheckBox_->setChecked(true);
    useXLDDisplayCheckBox_->setToolTip("启用后将直接显示轮廓而不烧录到图像上");
    displayLayout->addWidget(useXLDDisplayCheckBox_);

    mainLayout->addWidget(displayGroup);

    // 添加弹簧
    mainLayout->addStretch();
}

void ShapeMatchParamPanel::connectSignals()
{
    // 匹配参数
    connect(matchTypeCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ShapeMatchParamPanel::onMatchTypeChanged);

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

    connect(scaleStepRowSpinBox_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &ShapeMatchParamPanel::onScaleStepRowChanged);
    connect(scaleStepColSpinBox_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &ShapeMatchParamPanel::onScaleStepColChanged);

    // 显示选项
    connect(useXLDDisplayCheckBox_, &QCheckBox::checkStateChanged,
            this, &ShapeMatchParamPanel::onUseXLDDisplayChanged);

    // 模板管理按钮
    connect(loadModelBtn_, &QPushButton::clicked,
            this, &ShapeMatchParamPanel::onLoadModelClicked);
    connect(saveModelBtn_, &QPushButton::clicked,
            this, &ShapeMatchParamPanel::onSaveModelClicked);
    connect(trainModelBtn_, &QPushButton::clicked,
            this, &ShapeMatchParamPanel::onTrainModelClicked);
    connect(clearModelBtn_, &QPushButton::clicked,
            this, &ShapeMatchParamPanel::onClearModelClicked);
    connect(modelLibraryBtn_, &QPushButton::clicked,
            this, &ShapeMatchParamPanel::onModelLibraryClicked);
}

void ShapeMatchParamPanel::updateUI()
{
    if (!tool_) return;

    // 更新匹配参数
    matchTypeCombo_->blockSignals(true);
    minScoreSpinBox_->blockSignals(true);
    minScoreSlider_->blockSignals(true);
    numMatchesSpinBox_->blockSignals(true);
    angleStartSpinBox_->blockSignals(true);
    angleExtentSpinBox_->blockSignals(true);
    minContrastSpinBox_->blockSignals(true);
    scaleMinSpinBox_->blockSignals(true);
    scaleMaxSpinBox_->blockSignals(true);
    scaleStepRowSpinBox_->blockSignals(true);
    scaleStepColSpinBox_->blockSignals(true);
    useXLDDisplayCheckBox_->blockSignals(true);

    matchTypeCombo_->setCurrentIndex(static_cast<int>(tool_->getMatchType()));
    minScoreSpinBox_->setValue(tool_->getMinScore());
    minScoreSlider_->setValue(static_cast<int>(tool_->getMinScore() * 100));
    numMatchesSpinBox_->setValue(tool_->getNumMatches());
    angleStartSpinBox_->setValue(tool_->getAngleStart() * 180.0 / M_PI);
    angleExtentSpinBox_->setValue(tool_->getAngleExtent() * 180.0 / M_PI);
    minContrastSpinBox_->setValue(tool_->getMinContrast());
    scaleMinSpinBox_->setValue(tool_->getScaleMin());
    scaleMaxSpinBox_->setValue(tool_->getScaleMax());
    scaleStepRowSpinBox_->setValue(tool_->getScaleStepRow());
    scaleStepColSpinBox_->setValue(tool_->getScaleStepCol());
    useXLDDisplayCheckBox_->setChecked(tool_->getUseXLDDisplay());

    // 根据匹配类型显示/隐藏缩放步长参数
    bool showScaleStep = (tool_->getMatchType() == Algorithm::ShapeMatchTool::Anisotropic);
    scaleStepRowLabel_->setVisible(showScaleStep);
    scaleStepRowSpinBox_->setVisible(showScaleStep);
    scaleStepColLabel_->setVisible(showScaleStep);
    scaleStepColSpinBox_->setVisible(showScaleStep);

    matchTypeCombo_->blockSignals(false);
    minScoreSpinBox_->blockSignals(false);
    minScoreSlider_->blockSignals(false);
    numMatchesSpinBox_->blockSignals(false);
    angleStartSpinBox_->blockSignals(false);
    angleExtentSpinBox_->blockSignals(false);
    minContrastSpinBox_->blockSignals(false);
    scaleMinSpinBox_->blockSignals(false);
    scaleMaxSpinBox_->blockSignals(false);
    scaleStepRowSpinBox_->blockSignals(false);
    scaleStepColSpinBox_->blockSignals(false);
    useXLDDisplayCheckBox_->blockSignals(false);

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

    tool_->blockSignals(true);
    tool_->setMinScore(value);
    tool_->blockSignals(false);
    emit parameterChanged();
}

void ShapeMatchParamPanel::onNumMatchesChanged(int value)
{
    if (!tool_) return;
    tool_->blockSignals(true);
    tool_->setNumMatches(value);
    tool_->blockSignals(false);
    emit parameterChanged();
}

void ShapeMatchParamPanel::onAngleRangeChanged()
{
    if (!tool_) return;

    double startDeg = angleStartSpinBox_->value();
    double extentDeg = angleExtentSpinBox_->value();

    // 转换为弧度
    tool_->blockSignals(true);
    tool_->setAngleStart(startDeg * M_PI / 180.0);
    tool_->setAngleExtent(extentDeg * M_PI / 180.0);
    tool_->blockSignals(false);

    emit parameterChanged();
}

void ShapeMatchParamPanel::onContrastChanged(int value)
{
    if (!tool_) return;
    tool_->blockSignals(true);
    tool_->setMinContrast(value);
    tool_->blockSignals(false);
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

    tool_->blockSignals(true);
    tool_->setScaleMin(scaleMin);
    tool_->setScaleMax(scaleMax);
    tool_->blockSignals(false);

    emit parameterChanged();
}

void ShapeMatchParamPanel::onMatchTypeChanged(int index)
{
    if (!tool_) return;
    // 检查索引有效性
    if (index < 0 || index > 2) return;
    // 检查控件有效性
    if (!scaleStepRowLabel_ || !scaleStepRowSpinBox_ ||
        !scaleStepColLabel_ || !scaleStepColSpinBox_) return;

    Algorithm::ShapeMatchTool::MatchType matchType =
        static_cast<Algorithm::ShapeMatchTool::MatchType>(index);

    // 阻塞tool信号，避免触发外部组件更新导致递归
    tool_->blockSignals(true);
    tool_->setMatchType(matchType);
    tool_->blockSignals(false);

    // 根据匹配类型显示/隐藏缩放步长参数
    bool showScaleStep = (matchType == Algorithm::ShapeMatchTool::Anisotropic);
    scaleStepRowLabel_->setVisible(showScaleStep);
    scaleStepRowSpinBox_->setVisible(showScaleStep);
    scaleStepColLabel_->setVisible(showScaleStep);
    scaleStepColSpinBox_->setVisible(showScaleStep);

    emit parameterChanged();
}

void ShapeMatchParamPanel::onScaleStepRowChanged(double value)
{
    if (!tool_) return;
    tool_->blockSignals(true);
    tool_->setScaleStepRow(value);
    tool_->blockSignals(false);
    emit parameterChanged();
}

void ShapeMatchParamPanel::onScaleStepColChanged(double value)
{
    if (!tool_) return;
    tool_->blockSignals(true);
    tool_->setScaleStepCol(value);
    tool_->blockSignals(false);
    emit parameterChanged();
}

void ShapeMatchParamPanel::onUseXLDDisplayChanged(int state)
{
    if (!tool_) return;
    tool_->blockSignals(true);
    tool_->setUseXLDDisplay(state == Qt::Checked);
    tool_->blockSignals(false);
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

void ShapeMatchParamPanel::onModelLibraryClicked()
{
    if (!tool_) return;

    LOG_DEBUG("onModelLibraryClicked: 开始");

    // 设置模板库路径（默认为用户文档目录下的VisionForge/ShapeModels）
    Algorithm::ShapeModelManager& manager = Algorithm::ShapeModelManager::instance();
    QString defaultLibraryPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
                                + "/VisionForge/ShapeModels";

    if (manager.getLibraryPath().isEmpty()) {
        LOG_DEBUG("onModelLibraryClicked: 加载模板库");
        manager.loadLibrary(defaultLibraryPath);
    }

    LOG_DEBUG("onModelLibraryClicked: 创建对话框");

    // 打开模板库对话框
    ShapeModelLibraryDialog dialog(this);
    dialog.setLibraryPath(manager.getLibraryPath());

    LOG_DEBUG("onModelLibraryClicked: 执行对话框");

    // 执行对话框，等待用户选择
    int result = dialog.exec();

    LOG_DEBUG(QString("onModelLibraryClicked: 对话框返回 %1").arg(result));

    if (result == QDialog::Accepted) {
        LOG_DEBUG("onModelLibraryClicked: 用户接受了对话框");

        // 获取选中的模板路径（在对话框关闭前获取）
        QString modelPath = dialog.getSelectedModelPath();
        QString modelId = dialog.getSelectedModelId();

        LOG_DEBUG(QString("onModelLibraryClicked: 获取的路径=%1, ID=%2").arg(modelPath).arg(modelId));

        if (modelPath.isEmpty()) {
            LOG_WARNING("onModelLibraryClicked: 模板路径为空");
            QMessageBox::warning(this, "警告", "未能获取模板路径");
            return;
        }

        // 检查文件是否存在
        QFileInfo fileInfo(modelPath);
        if (!fileInfo.exists()) {
            LOG_ERROR(QString("onModelLibraryClicked: 模板文件不存在: %1").arg(modelPath));
            QMessageBox::warning(this, "错误", QString("模板文件不存在:\n%1").arg(modelPath));
            return;
        }

        LOG_DEBUG("onModelLibraryClicked: 开始加载模板");

        // 阻塞工具信号，避免加载过程中触发更新
        tool_->blockSignals(true);

        bool loadSuccess = false;
        try {
            loadSuccess = tool_->loadModel(modelPath);
            LOG_DEBUG(QString("onModelLibraryClicked: 加载结果=%1").arg(loadSuccess));
        } catch (const std::exception& e) {
            LOG_ERROR(QString("加载模板时发生异常: %1").arg(e.what()));
        } catch (...) {
            LOG_ERROR("加载模板时发生未知异常");
        }

        tool_->blockSignals(false);

        LOG_DEBUG("onModelLibraryClicked: 准备更新UI");

        if (loadSuccess) {
            // 增加使用计数
            if (!modelId.isEmpty()) {
                try {
                    manager.incrementUsage(modelId);
                } catch (...) {
                    LOG_WARNING("增加使用计数时发生异常");
                }
            }

            LOG_DEBUG("onModelLibraryClicked: 调用 updateUI");
            updateUI();

            LOG_DEBUG("onModelLibraryClicked: 发送 parameterChanged 信号");
            emit parameterChanged();

            auto modelInfo = manager.getModel(modelId);
            QString modelName = modelInfo ? modelInfo->name : "模板";

            LOG_DEBUG("onModelLibraryClicked: 显示成功消息");
            QMessageBox::information(this, "成功",
                QString("已加载模板: %1").arg(modelName));
        } else {
            QMessageBox::warning(this, "失败", "模板加载失败");
        }
    }

    LOG_DEBUG("onModelLibraryClicked: 结束");
}

} // namespace UI
} // namespace VisionForge
