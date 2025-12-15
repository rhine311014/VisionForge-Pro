/**
 * @file ShapeModelLibraryDialog.cpp
 * @brief 形状模板库对话框实现
 */

#ifdef _WIN32

#include "ui/ShapeModelLibraryDialog.h"
#include "base/Logger.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QFileDialog>
#include <QMessageBox>
#include <QSplitter>

namespace VisionForge {
namespace UI {

// ========== ModelListItemWidget ==========

ModelListItemWidget::ModelListItemWidget(Algorithm::ShapeModelInfoPtr info, QWidget* parent)
    : QWidget(parent)
    , modelId_(info->id)
{
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setContentsMargins(5, 5, 5, 5);
    layout->setSpacing(10);

    // 缩略图
    thumbnailLabel_ = new QLabel(this);
    thumbnailLabel_->setFixedSize(64, 64);
    thumbnailLabel_->setScaledContents(true);
    thumbnailLabel_->setStyleSheet("border: 1px solid #cccccc; background: black;");

    QImage thumbnail = Algorithm::ShapeModelManager::instance().getThumbnail(info->id);
    if (!thumbnail.isNull()) {
        thumbnailLabel_->setPixmap(QPixmap::fromImage(thumbnail));
    }

    layout->addWidget(thumbnailLabel_);

    // 文本信息
    QVBoxLayout* textLayout = new QVBoxLayout();
    textLayout->setSpacing(2);

    nameLabel_ = new QLabel(info->name, this);
    QFont nameFont = nameLabel_->font();
    nameFont.setBold(true);
    nameFont.setPointSize(10);
    nameLabel_->setFont(nameFont);

    infoLabel_ = new QLabel(this);
    infoLabel_->setStyleSheet("color: #666666; font-size: 9pt;");
    QString infoText = QString("使用次数: %1 | 创建: %2")
        .arg(info->usageCount)
        .arg(info->createTime.toString("yyyy-MM-dd"));
    infoLabel_->setText(infoText);

    textLayout->addWidget(nameLabel_);
    textLayout->addWidget(infoLabel_);
    textLayout->addStretch();

    layout->addLayout(textLayout, 1);
}

// ========== ShapeModelLibraryDialog ==========

ShapeModelLibraryDialog::ShapeModelLibraryDialog(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle("形状模板库");
    resize(900, 600);

    createUI();
    connectSignals();
}

void ShapeModelLibraryDialog::createUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // 顶部搜索栏
    QHBoxLayout* searchLayout = new QHBoxLayout();
    searchLayout->addWidget(new QLabel("搜索:", this));
    searchEdit_ = new QLineEdit(this);
    searchEdit_->setPlaceholderText("输入模板名称或描述...");
    searchLayout->addWidget(searchEdit_, 1);
    mainLayout->addLayout(searchLayout);

    // 中间内容区域（分割器）
    QSplitter* splitter = new QSplitter(Qt::Horizontal, this);

    // 左侧：模板列表
    QWidget* listWidget = new QWidget(this);
    QVBoxLayout* listLayout = new QVBoxLayout(listWidget);
    listLayout->setContentsMargins(0, 0, 0, 0);

    QLabel* listTitle = new QLabel("模板列表", this);
    QFont titleFont = listTitle->font();
    titleFont.setBold(true);
    titleFont.setPointSize(10);
    listTitle->setFont(titleFont);
    listLayout->addWidget(listTitle);

    modelList_ = new QListWidget(this);
    modelList_->setSelectionMode(QAbstractItemView::SingleSelection);
    listLayout->addWidget(modelList_);

    splitter->addWidget(listWidget);

    // 右侧：预览和详细信息
    QWidget* previewWidget = new QWidget(this);
    QVBoxLayout* previewLayout = new QVBoxLayout(previewWidget);
    previewLayout->setContentsMargins(0, 0, 0, 0);

    QLabel* previewTitle = new QLabel("模板预览", this);
    previewTitle->setFont(titleFont);
    previewLayout->addWidget(previewTitle);

    // 缩略图
    thumbnailLabel_ = new QLabel(this);
    thumbnailLabel_->setFixedSize(256, 256);
    thumbnailLabel_->setScaledContents(true);
    thumbnailLabel_->setAlignment(Qt::AlignCenter);
    thumbnailLabel_->setStyleSheet("border: 2px solid #cccccc; background: black;");
    previewLayout->addWidget(thumbnailLabel_, 0, Qt::AlignCenter);

    // 基本信息
    nameLabel_ = new QLabel(this);
    QFont nameFont = nameLabel_->font();
    nameFont.setBold(true);
    nameFont.setPointSize(12);
    nameLabel_->setFont(nameFont);
    previewLayout->addWidget(nameLabel_);

    descLabel_ = new QLabel(this);
    descLabel_->setWordWrap(true);
    descLabel_->setStyleSheet("color: #666666;");
    previewLayout->addWidget(descLabel_);

    // 详细信息
    QLabel* detailsTitle = new QLabel("详细信息", this);
    detailsTitle->setFont(titleFont);
    previewLayout->addWidget(detailsTitle);

    detailsText_ = new QTextEdit(this);
    detailsText_->setReadOnly(true);
    detailsText_->setMaximumHeight(150);
    previewLayout->addWidget(detailsText_);

    previewLayout->addStretch();

    splitter->addWidget(previewWidget);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 1);

    mainLayout->addWidget(splitter, 1);

    // 底部按钮区域
    QHBoxLayout* buttonLayout = new QHBoxLayout();

    importBtn_ = new QPushButton("导入模板", this);
    exportBtn_ = new QPushButton("导出模板", this);
    deleteBtn_ = new QPushButton("删除模板", this);
    refreshBtn_ = new QPushButton("刷新", this);

    exportBtn_->setEnabled(false);
    deleteBtn_->setEnabled(false);

    buttonLayout->addWidget(importBtn_);
    buttonLayout->addWidget(exportBtn_);
    buttonLayout->addWidget(deleteBtn_);
    buttonLayout->addWidget(refreshBtn_);
    buttonLayout->addStretch();

    useBtn_ = new QPushButton("使用此模板", this);
    useBtn_->setEnabled(false);
    closeBtn_ = new QPushButton("关闭", this);

    buttonLayout->addWidget(useBtn_);
    buttonLayout->addWidget(closeBtn_);

    mainLayout->addLayout(buttonLayout);
}

void ShapeModelLibraryDialog::connectSignals()
{
    connect(importBtn_, &QPushButton::clicked, this, &ShapeModelLibraryDialog::onImportClicked);
    connect(exportBtn_, &QPushButton::clicked, this, &ShapeModelLibraryDialog::onExportClicked);
    connect(deleteBtn_, &QPushButton::clicked, this, &ShapeModelLibraryDialog::onDeleteClicked);
    connect(refreshBtn_, &QPushButton::clicked, this, &ShapeModelLibraryDialog::onRefreshClicked);
    connect(useBtn_, &QPushButton::clicked, this, &ShapeModelLibraryDialog::onUseModelClicked);
    connect(closeBtn_, &QPushButton::clicked, this, &QDialog::reject);

    connect(searchEdit_, &QLineEdit::textChanged,
            this, &ShapeModelLibraryDialog::onSearchTextChanged);

    connect(modelList_, &QListWidget::itemSelectionChanged,
            this, &ShapeModelLibraryDialog::onModelSelectionChanged);
    connect(modelList_, &QListWidget::itemDoubleClicked,
            this, &ShapeModelLibraryDialog::onModelDoubleClicked);

    // 连接管理器信号
    Algorithm::ShapeModelManager& manager = Algorithm::ShapeModelManager::instance();
    connect(&manager, &Algorithm::ShapeModelManager::libraryLoaded,
            this, &ShapeModelLibraryDialog::onLibraryLoaded);
    connect(&manager, &Algorithm::ShapeModelManager::modelAdded,
            this, &ShapeModelLibraryDialog::onModelAdded);
    connect(&manager, &Algorithm::ShapeModelManager::modelRemoved,
            this, &ShapeModelLibraryDialog::onModelRemoved);
}

void ShapeModelLibraryDialog::setLibraryPath(const QString& path)
{
    Algorithm::ShapeModelManager& manager = Algorithm::ShapeModelManager::instance();
    manager.loadLibrary(path);
}

QString ShapeModelLibraryDialog::getSelectedModelId() const
{
    return currentModelId_;
}

QString ShapeModelLibraryDialog::getSelectedModelPath() const
{
    if (currentModelId_.isEmpty()) {
        return QString();
    }

    Algorithm::ShapeModelManager& manager = Algorithm::ShapeModelManager::instance();
    auto info = manager.getModel(currentModelId_);

    if (!info) {
        return QString();
    }

    QDir dir(manager.getLibraryPath());
    return dir.filePath(info->filePath);
}

void ShapeModelLibraryDialog::onImportClicked()
{
    QStringList filePaths = QFileDialog::getOpenFileNames(this,
        "导入模板", QDir::homePath(), "Halcon模板文件 (*.shm);;所有文件 (*.*)");

    if (filePaths.isEmpty()) {
        return;
    }

    Algorithm::ShapeModelManager& manager = Algorithm::ShapeModelManager::instance();
    int count = manager.importModels(filePaths);

    QMessageBox::information(this, "导入完成",
        QString("成功导入 %1 个模板").arg(count));
}

void ShapeModelLibraryDialog::onExportClicked()
{
    if (currentModelId_.isEmpty()) {
        return;
    }

    QString filePath = QFileDialog::getSaveFileName(this,
        "导出模板", QDir::homePath(), "Halcon模板文件 (*.shm);;所有文件 (*.*)");

    if (filePath.isEmpty()) {
        return;
    }

    Algorithm::ShapeModelManager& manager = Algorithm::ShapeModelManager::instance();
    if (manager.exportModel(currentModelId_, filePath)) {
        QMessageBox::information(this, "导出成功", "模板已导出");
    } else {
        QMessageBox::warning(this, "导出失败", "模板导出失败");
    }
}

void ShapeModelLibraryDialog::onDeleteClicked()
{
    if (currentModelId_.isEmpty()) {
        return;
    }

    int ret = QMessageBox::question(this, "确认删除",
        "确定要删除此模板吗？此操作无法撤销。",
        QMessageBox::Yes | QMessageBox::No);

    if (ret == QMessageBox::Yes) {
        Algorithm::ShapeModelManager& manager = Algorithm::ShapeModelManager::instance();
        manager.removeModel(currentModelId_);
    }
}

void ShapeModelLibraryDialog::onRefreshClicked()
{
    loadModels();
}

void ShapeModelLibraryDialog::onSearchTextChanged(const QString& text)
{
    loadModels();
}

void ShapeModelLibraryDialog::onModelSelectionChanged()
{
    QListWidgetItem* item = modelList_->currentItem();
    if (!item) {
        currentModelId_.clear();
        clearPreview();
        exportBtn_->setEnabled(false);
        deleteBtn_->setEnabled(false);
        useBtn_->setEnabled(false);
        return;
    }

    ModelListItemWidget* itemWidget =
        qobject_cast<ModelListItemWidget*>(modelList_->itemWidget(item));

    if (itemWidget) {
        currentModelId_ = itemWidget->getModelId();

        Algorithm::ShapeModelManager& manager = Algorithm::ShapeModelManager::instance();
        auto info = manager.getModel(currentModelId_);

        if (info) {
            updatePreview(info);
            exportBtn_->setEnabled(true);
            deleteBtn_->setEnabled(true);
            useBtn_->setEnabled(true);
        }
    }
}

void ShapeModelLibraryDialog::onModelDoubleClicked(QListWidgetItem* item)
{
    if (item && !currentModelId_.isEmpty()) {
        emit modelSelected(currentModelId_);
        accept();
    }
}

void ShapeModelLibraryDialog::onUseModelClicked()
{
    if (!currentModelId_.isEmpty()) {
        emit modelSelected(currentModelId_);
        accept();
    }
}

void ShapeModelLibraryDialog::onLibraryLoaded()
{
    loadModels();
}

void ShapeModelLibraryDialog::onModelAdded(const QString& modelId)
{
    loadModels();
}

void ShapeModelLibraryDialog::onModelRemoved(const QString& modelId)
{
    if (currentModelId_ == modelId) {
        currentModelId_.clear();
        clearPreview();
    }
    loadModels();
}

void ShapeModelLibraryDialog::loadModels()
{
    modelList_->clear();

    Algorithm::ShapeModelManager& manager = Algorithm::ShapeModelManager::instance();
    QString keyword = searchEdit_->text().trimmed();

    std::vector<Algorithm::ShapeModelInfoPtr> models = keyword.isEmpty()
        ? manager.getAllModels()
        : manager.searchModels(keyword);

    for (const auto& info : models) {
        QListWidgetItem* item = new QListWidgetItem(modelList_);
        ModelListItemWidget* widget = new ModelListItemWidget(info, modelList_);

        item->setSizeHint(widget->sizeHint());
        modelList_->addItem(item);
        modelList_->setItemWidget(item, widget);
    }
}

void ShapeModelLibraryDialog::updatePreview(Algorithm::ShapeModelInfoPtr info)
{
    if (!info) {
        clearPreview();
        return;
    }

    // 缩略图
    QImage thumbnail = Algorithm::ShapeModelManager::instance().getThumbnail(info->id);
    if (!thumbnail.isNull()) {
        thumbnailLabel_->setPixmap(QPixmap::fromImage(thumbnail));
    } else {
        thumbnailLabel_->clear();
        thumbnailLabel_->setText("无预览图");
    }

    // 基本信息
    nameLabel_->setText(info->name);
    descLabel_->setText(info->description.isEmpty() ? "无描述" : info->description);

    // 详细信息
    QString details;
    details += QString("ID: %1\n").arg(info->id);
    details += QString("创建时间: %1\n").arg(info->createTime.toString("yyyy-MM-dd hh:mm:ss"));
    details += QString("修改时间: %1\n").arg(info->modifyTime.toString("yyyy-MM-dd hh:mm:ss"));
    details += QString("使用次数: %1\n").arg(info->usageCount);
    details += QString("\n参数:\n");
    details += QString("金字塔层数: %1\n").arg(info->numLevels);
    details += QString("角度范围: %1° - %2°\n")
        .arg(info->angleStart * 180.0 / M_PI, 0, 'f', 1)
        .arg((info->angleStart + info->angleExtent) * 180.0 / M_PI, 0, 'f', 1);
    details += QString("最小对比度: %1\n").arg(info->minContrast);

    detailsText_->setText(details);
}

void ShapeModelLibraryDialog::clearPreview()
{
    thumbnailLabel_->clear();
    thumbnailLabel_->setText("未选择模板");
    nameLabel_->clear();
    descLabel_->clear();
    detailsText_->clear();
}

} // namespace UI
} // namespace VisionForge

#endif // _WIN32
