/**
 * @file SaveImageToolDialog.cpp
 * @brief 图像保存工具参数编辑对话框实现
 * @author VisionForge Team
 * @date 2025-12-18
 */

#include "ui/SaveImageToolDialog.h"
#include "algorithm/SaveImageTool.h"
#include "base/Logger.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QFileDialog>
#include <QMessageBox>
#include <QDateTime>

namespace VisionForge {
namespace UI {

SaveImageToolDialog::SaveImageToolDialog(Algorithm::SaveImageTool* tool, QWidget* parent)
    : QDialog(parent)
    , tool_(tool)
    , pathEdit_(nullptr)
    , browseBtn_(nullptr)
    , formatCombo_(nullptr)
    , namingModeCombo_(nullptr)
    , prefixEdit_(nullptr)
    , filenamePreviewLabel_(nullptr)
    , jpegQualitySpin_(nullptr)
    , jpegQualityLabel_(nullptr)
    , saveConditionCombo_(nullptr)
    , dateSubfolderCheck_(nullptr)
    , saveCountLabel_(nullptr)
    , lastSavedLabel_(nullptr)
    , previewBtn_(nullptr)
    , resetCounterBtn_(nullptr)
    , okBtn_(nullptr)
    , cancelBtn_(nullptr)
    , applyBtn_(nullptr)
{
    setWindowTitle("图像保存设置");
    setMinimumSize(600, 500);
    resize(650, 550);
    setWindowFlags(windowFlags() | Qt::WindowMaximizeButtonHint);

    createUI();
    connectSignals();
    updateUI();
}

SaveImageToolDialog::~SaveImageToolDialog()
{
}

void SaveImageToolDialog::updateUI()
{
    if (!tool_) return;

    // 阻塞信号
    pathEdit_->blockSignals(true);
    formatCombo_->blockSignals(true);
    namingModeCombo_->blockSignals(true);
    prefixEdit_->blockSignals(true);
    jpegQualitySpin_->blockSignals(true);
    saveConditionCombo_->blockSignals(true);
    dateSubfolderCheck_->blockSignals(true);

    // 更新路径
    pathEdit_->setText(tool_->saveDirectory());

    // 更新格式
    formatCombo_->setCurrentIndex(static_cast<int>(tool_->imageFormat()));

    // 更新命名模式
    namingModeCombo_->setCurrentIndex(static_cast<int>(tool_->namingMode()));

    // 更新前缀
    prefixEdit_->setText(tool_->prefix());

    // 更新JPEG质量
    jpegQualitySpin_->setValue(tool_->jpegQuality());

    // 更新保存条件
    saveConditionCombo_->setCurrentIndex(static_cast<int>(tool_->saveCondition()));

    // 更新日期子文件夹
    dateSubfolderCheck_->setChecked(tool_->createDateSubfolder());

    // 恢复信号
    pathEdit_->blockSignals(false);
    formatCombo_->blockSignals(false);
    namingModeCombo_->blockSignals(false);
    prefixEdit_->blockSignals(false);
    jpegQualitySpin_->blockSignals(false);
    saveConditionCombo_->blockSignals(false);
    dateSubfolderCheck_->blockSignals(false);

    updateStatusDisplay();
    filenamePreviewLabel_->setText(generatePreviewFilename());
}

void SaveImageToolDialog::showEvent(QShowEvent* event)
{
    QDialog::showEvent(event);
    updateUI();
}

void SaveImageToolDialog::createUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(10);

    createPathGroup(mainLayout);
    createFormatGroup(mainLayout);
    createNamingGroup(mainLayout);
    createQualityGroup(mainLayout);
    createConditionGroup(mainLayout);
    createStatusGroup(mainLayout);
    createButtonGroup(mainLayout);

    mainLayout->addStretch();

    // 底部按钮
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();

    okBtn_ = new QPushButton("确定", this);
    cancelBtn_ = new QPushButton("取消", this);
    applyBtn_ = new QPushButton("应用", this);

    okBtn_->setMinimumWidth(80);
    cancelBtn_->setMinimumWidth(80);
    applyBtn_->setMinimumWidth(80);

    buttonLayout->addWidget(okBtn_);
    buttonLayout->addWidget(cancelBtn_);
    buttonLayout->addWidget(applyBtn_);

    mainLayout->addLayout(buttonLayout);
}

void SaveImageToolDialog::createPathGroup(QVBoxLayout* layout)
{
    QGroupBox* group = new QGroupBox("保存路径", this);
    QHBoxLayout* groupLayout = new QHBoxLayout(group);

    pathEdit_ = new QLineEdit(group);
    pathEdit_->setPlaceholderText("选择图像保存目录...");

    browseBtn_ = new QPushButton("浏览...", group);

    groupLayout->addWidget(pathEdit_);
    groupLayout->addWidget(browseBtn_);

    layout->addWidget(group);
}

void SaveImageToolDialog::createFormatGroup(QVBoxLayout* layout)
{
    QGroupBox* group = new QGroupBox("图像格式", this);
    QFormLayout* formLayout = new QFormLayout(group);

    formatCombo_ = new QComboBox(group);
    formatCombo_->addItem("PNG - 无损压缩", static_cast<int>(Algorithm::SaveImageTool::PNG));
    formatCombo_->addItem("JPEG - 有损压缩", static_cast<int>(Algorithm::SaveImageTool::JPEG));
    formatCombo_->addItem("BMP - 无压缩", static_cast<int>(Algorithm::SaveImageTool::BMP));
    formatCombo_->addItem("TIFF - 高质量", static_cast<int>(Algorithm::SaveImageTool::TIFF));

    formLayout->addRow("格式:", formatCombo_);

    layout->addWidget(group);
}

void SaveImageToolDialog::createNamingGroup(QVBoxLayout* layout)
{
    QGroupBox* group = new QGroupBox("命名规则", this);
    QVBoxLayout* groupLayout = new QVBoxLayout(group);

    QFormLayout* formLayout = new QFormLayout();

    namingModeCombo_ = new QComboBox(group);
    namingModeCombo_->addItem("时间戳 (20251218_143025_123)", static_cast<int>(Algorithm::SaveImageTool::Timestamp));
    namingModeCombo_->addItem("序号 (000001, 000002, ...)", static_cast<int>(Algorithm::SaveImageTool::Sequential));
    namingModeCombo_->addItem("自定义前缀 + 时间戳", static_cast<int>(Algorithm::SaveImageTool::Custom));

    formLayout->addRow("命名模式:", namingModeCombo_);

    prefixEdit_ = new QLineEdit(group);
    prefixEdit_->setPlaceholderText("文件名前缀");
    prefixEdit_->setText("image");
    formLayout->addRow("前缀:", prefixEdit_);

    groupLayout->addLayout(formLayout);

    // 文件名预览
    QHBoxLayout* previewLayout = new QHBoxLayout();
    previewLayout->addWidget(new QLabel("预览:"));
    filenamePreviewLabel_ = new QLabel("image_20251218_143025.png", group);
    filenamePreviewLabel_->setStyleSheet("color: #666; font-family: monospace;");
    previewLayout->addWidget(filenamePreviewLabel_);
    previewLayout->addStretch();
    groupLayout->addLayout(previewLayout);

    layout->addWidget(group);
}

void SaveImageToolDialog::createQualityGroup(QVBoxLayout* layout)
{
    QGroupBox* group = new QGroupBox("质量参数", this);
    QFormLayout* formLayout = new QFormLayout(group);

    QHBoxLayout* qualityLayout = new QHBoxLayout();
    jpegQualitySpin_ = new QSpinBox(group);
    jpegQualitySpin_->setRange(1, 100);
    jpegQualitySpin_->setValue(95);
    qualityLayout->addWidget(jpegQualitySpin_);

    jpegQualityLabel_ = new QLabel("(仅JPEG格式有效)", group);
    jpegQualityLabel_->setStyleSheet("color: #888;");
    qualityLayout->addWidget(jpegQualityLabel_);
    qualityLayout->addStretch();

    formLayout->addRow("JPEG质量 (1-100):", qualityLayout);

    layout->addWidget(group);
}

void SaveImageToolDialog::createConditionGroup(QVBoxLayout* layout)
{
    QGroupBox* group = new QGroupBox("保存条件", this);
    QVBoxLayout* groupLayout = new QVBoxLayout(group);

    QFormLayout* formLayout = new QFormLayout();

    saveConditionCombo_ = new QComboBox(group);
    saveConditionCombo_->addItem("总是保存 (Always)", static_cast<int>(Algorithm::SaveImageTool::Always));
    saveConditionCombo_->addItem("仅OK时保存 (OnlyOK)", static_cast<int>(Algorithm::SaveImageTool::OnlyOK));
    saveConditionCombo_->addItem("仅NG时保存 (OnlyNG)", static_cast<int>(Algorithm::SaveImageTool::OnlyNG));

    formLayout->addRow("保存条件:", saveConditionCombo_);
    groupLayout->addLayout(formLayout);

    dateSubfolderCheck_ = new QCheckBox("按日期创建子文件夹 (如: 2025-12-18/)", group);
    groupLayout->addWidget(dateSubfolderCheck_);

    layout->addWidget(group);
}

void SaveImageToolDialog::createStatusGroup(QVBoxLayout* layout)
{
    QGroupBox* group = new QGroupBox("保存状态", this);
    QHBoxLayout* groupLayout = new QHBoxLayout(group);

    groupLayout->addWidget(new QLabel("已保存:"));
    saveCountLabel_ = new QLabel("0", group);
    saveCountLabel_->setStyleSheet("font-weight: bold; color: #4CAF50;");
    groupLayout->addWidget(saveCountLabel_);

    groupLayout->addSpacing(20);

    groupLayout->addWidget(new QLabel("最近保存:"));
    lastSavedLabel_ = new QLabel("--", group);
    lastSavedLabel_->setStyleSheet("color: #666;");
    groupLayout->addWidget(lastSavedLabel_);

    groupLayout->addStretch();

    resetCounterBtn_ = new QPushButton("重置计数", group);
    groupLayout->addWidget(resetCounterBtn_);

    layout->addWidget(group);
}

void SaveImageToolDialog::createButtonGroup(QVBoxLayout* layout)
{
    QHBoxLayout* btnLayout = new QHBoxLayout();

    previewBtn_ = new QPushButton("测试保存", this);
    previewBtn_->setMinimumHeight(35);
    previewBtn_->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; font-weight: bold; }");

    btnLayout->addWidget(previewBtn_);

    layout->addLayout(btnLayout);
}

void SaveImageToolDialog::connectSignals()
{
    // 路径选择
    connect(browseBtn_, &QPushButton::clicked, this, &SaveImageToolDialog::onBrowseClicked);

    // 参数变化
    connect(formatCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SaveImageToolDialog::onFormatChanged);
    connect(namingModeCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SaveImageToolDialog::onNamingModeChanged);
    connect(saveConditionCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SaveImageToolDialog::onSaveConditionChanged);
    connect(jpegQualitySpin_, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &SaveImageToolDialog::onJpegQualityChanged);
    connect(dateSubfolderCheck_, &QCheckBox::toggled,
            this, &SaveImageToolDialog::onDateSubfolderChanged);
    connect(prefixEdit_, &QLineEdit::textChanged,
            this, &SaveImageToolDialog::onPrefixChanged);

    // 重置计数器
    connect(resetCounterBtn_, &QPushButton::clicked, this, [this]() {
        if (tool_) {
            tool_->resetCounter();
            updateStatusDisplay();
        }
    });

    // 按钮
    connect(previewBtn_, &QPushButton::clicked, this, &SaveImageToolDialog::onPreviewClicked);
    connect(okBtn_, &QPushButton::clicked, this, &SaveImageToolDialog::onOkClicked);
    connect(cancelBtn_, &QPushButton::clicked, this, &SaveImageToolDialog::onCancelClicked);
    connect(applyBtn_, &QPushButton::clicked, this, &SaveImageToolDialog::onApplyClicked);
}

void SaveImageToolDialog::onBrowseClicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, "选择保存目录",
        pathEdit_->text().isEmpty() ? QDir::homePath() : pathEdit_->text());

    if (!dir.isEmpty()) {
        pathEdit_->setText(dir);
        if (tool_) {
            tool_->setSaveDirectory(dir);
        }
        emit parameterChanged();
    }
}

void SaveImageToolDialog::onFormatChanged(int index)
{
    if (!tool_) return;

    auto format = static_cast<Algorithm::SaveImageTool::ImageFormat>(
        formatCombo_->currentData().toInt());
    tool_->setImageFormat(format);

    // JPEG质量仅在JPEG格式时启用
    jpegQualitySpin_->setEnabled(format == Algorithm::SaveImageTool::JPEG);

    filenamePreviewLabel_->setText(generatePreviewFilename());
    emit parameterChanged();
}

void SaveImageToolDialog::onNamingModeChanged(int index)
{
    if (!tool_) return;

    auto mode = static_cast<Algorithm::SaveImageTool::NamingMode>(
        namingModeCombo_->currentData().toInt());
    tool_->setNamingMode(mode);

    // 前缀仅在自定义模式时启用
    prefixEdit_->setEnabled(mode == Algorithm::SaveImageTool::Custom ||
                           mode == Algorithm::SaveImageTool::Timestamp);

    filenamePreviewLabel_->setText(generatePreviewFilename());
    emit parameterChanged();
}

void SaveImageToolDialog::onSaveConditionChanged(int index)
{
    if (!tool_) return;

    auto condition = static_cast<Algorithm::SaveImageTool::SaveCondition>(
        saveConditionCombo_->currentData().toInt());
    tool_->setSaveCondition(condition);
    emit parameterChanged();
}

void SaveImageToolDialog::onJpegQualityChanged(int value)
{
    if (!tool_) return;
    tool_->setJpegQuality(value);
    emit parameterChanged();
}

void SaveImageToolDialog::onDateSubfolderChanged(bool checked)
{
    if (!tool_) return;
    tool_->setCreateDateSubfolder(checked);
    emit parameterChanged();
}

void SaveImageToolDialog::onPrefixChanged(const QString& text)
{
    if (!tool_) return;
    tool_->setPrefix(text);
    filenamePreviewLabel_->setText(generatePreviewFilename());
    emit parameterChanged();
}

void SaveImageToolDialog::onPreviewClicked()
{
    applyParameters();
    emit executeSaveRequested();
    updateStatusDisplay();
}

void SaveImageToolDialog::onOkClicked()
{
    applyParameters();
    accept();
}

void SaveImageToolDialog::onCancelClicked()
{
    reject();
}

void SaveImageToolDialog::onApplyClicked()
{
    applyParameters();
    emit parameterChanged();
}

void SaveImageToolDialog::applyParameters()
{
    if (!tool_) return;

    tool_->setSaveDirectory(pathEdit_->text());
    tool_->setImageFormat(static_cast<Algorithm::SaveImageTool::ImageFormat>(
        formatCombo_->currentData().toInt()));
    tool_->setNamingMode(static_cast<Algorithm::SaveImageTool::NamingMode>(
        namingModeCombo_->currentData().toInt()));
    tool_->setPrefix(prefixEdit_->text());
    tool_->setJpegQuality(jpegQualitySpin_->value());
    tool_->setSaveCondition(static_cast<Algorithm::SaveImageTool::SaveCondition>(
        saveConditionCombo_->currentData().toInt()));
    tool_->setCreateDateSubfolder(dateSubfolderCheck_->isChecked());

    LOG_INFO("图像保存参数已应用");
}

void SaveImageToolDialog::updateStatusDisplay()
{
    if (!tool_) return;

    saveCountLabel_->setText(QString::number(tool_->saveCount()));

    QString lastPath = tool_->lastSavedPath();
    if (!lastPath.isEmpty()) {
        // 仅显示文件名
        QFileInfo fi(lastPath);
        lastSavedLabel_->setText(fi.fileName());
        lastSavedLabel_->setToolTip(lastPath);
    } else {
        lastSavedLabel_->setText("--");
    }
}

QString SaveImageToolDialog::generatePreviewFilename()
{
    QString preview;
    QString prefix = prefixEdit_->text();
    if (prefix.isEmpty()) prefix = "image";

    auto mode = static_cast<Algorithm::SaveImageTool::NamingMode>(
        namingModeCombo_->currentData().toInt());
    auto format = static_cast<Algorithm::SaveImageTool::ImageFormat>(
        formatCombo_->currentData().toInt());

    QString ext;
    switch (format) {
        case Algorithm::SaveImageTool::PNG: ext = ".png"; break;
        case Algorithm::SaveImageTool::JPEG: ext = ".jpg"; break;
        case Algorithm::SaveImageTool::BMP: ext = ".bmp"; break;
        case Algorithm::SaveImageTool::TIFF: ext = ".tif"; break;
    }

    QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss");

    switch (mode) {
        case Algorithm::SaveImageTool::Timestamp:
            preview = QString("%1_%2%3").arg(prefix).arg(timestamp).arg(ext);
            break;
        case Algorithm::SaveImageTool::Sequential:
            preview = QString("%1_000001%2").arg(prefix).arg(ext);
            break;
        case Algorithm::SaveImageTool::Custom:
            preview = QString("%1_%2%3").arg(prefix).arg(timestamp).arg(ext);
            break;
    }

    return preview;
}

} // namespace UI
} // namespace VisionForge
