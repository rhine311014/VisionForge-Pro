/**
 * @file RecipeEditorDialog.cpp
 * @brief 方案编辑对话框实现
 */

#include "ui/RecipeEditorDialog.h"
#include "base/Logger.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFormLayout>
#include <QTabWidget>
#include <QHeaderView>
#include <QDateTime>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>

namespace VisionForge {

RecipeEditorDialog::RecipeEditorDialog(Core::Recipe* recipe, QWidget* parent)
    : QDialog(parent)
    , recipe_(recipe)
{
    setWindowTitle(tr("编辑方案 - %1").arg(recipe ? recipe->name() : tr("新方案")));
    setMinimumSize(600, 500);
    resize(700, 600);

    setupUI();
    setupConnections();
    loadFromRecipe();
}

RecipeEditorDialog::~RecipeEditorDialog()
{
}

void RecipeEditorDialog::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(12, 12, 12, 12);
    mainLayout->setSpacing(12);

    // 使用选项卡组织内容
    QTabWidget* tabWidget = new QTabWidget;

    // 基本信息选项卡
    QWidget* basicTab = new QWidget;
    QVBoxLayout* basicLayout = new QVBoxLayout(basicTab);
    basicLayout->addWidget(createBasicInfoPanel());
    basicLayout->addWidget(createProductInfoPanel());
    basicLayout->addStretch();
    tabWidget->addTab(basicTab, tr("基本信息"));

    // 标签选项卡
    tabWidget->addTab(createTagsPanel(), tr("标签"));

    // 自定义参数选项卡
    tabWidget->addTab(createCustomParamsPanel(), tr("自定义参数"));

    // 工具链预览选项卡
    tabWidget->addTab(createToolChainPreviewPanel(), tr("工具链预览"));

    mainLayout->addWidget(tabWidget, 1);

    // 按钮面板
    mainLayout->addWidget(createButtonPanel());
}

QWidget* RecipeEditorDialog::createBasicInfoPanel()
{
    QGroupBox* group = new QGroupBox(tr("基本信息"));
    QFormLayout* layout = new QFormLayout(group);
    layout->setContentsMargins(12, 16, 12, 12);
    layout->setSpacing(8);

    nameEdit_ = new QLineEdit;
    nameEdit_->setPlaceholderText(tr("输入方案名称"));
    layout->addRow(tr("名称:"), nameEdit_);

    descEdit_ = new QTextEdit;
    descEdit_->setPlaceholderText(tr("输入方案描述"));
    descEdit_->setMaximumHeight(80);
    layout->addRow(tr("描述:"), descEdit_);

    versionEdit_ = new QLineEdit;
    versionEdit_->setPlaceholderText("1.0.0");
    layout->addRow(tr("版本:"), versionEdit_);

    authorEdit_ = new QLineEdit;
    layout->addRow(tr("作者:"), authorEdit_);

    createTimeLabel_ = new QLabel;
    createTimeLabel_->setStyleSheet("color: gray;");
    layout->addRow(tr("创建时间:"), createTimeLabel_);

    modifyTimeLabel_ = new QLabel;
    modifyTimeLabel_->setStyleSheet("color: gray;");
    layout->addRow(tr("修改时间:"), modifyTimeLabel_);

    return group;
}

QWidget* RecipeEditorDialog::createProductInfoPanel()
{
    QGroupBox* group = new QGroupBox(tr("产品信息"));
    QFormLayout* layout = new QFormLayout(group);
    layout->setContentsMargins(12, 16, 12, 12);
    layout->setSpacing(8);

    productCodeEdit_ = new QLineEdit;
    productCodeEdit_->setPlaceholderText(tr("输入产品编号（可选）"));
    layout->addRow(tr("产品编号:"), productCodeEdit_);

    return group;
}

QWidget* RecipeEditorDialog::createTagsPanel()
{
    QWidget* widget = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout(widget);
    layout->setContentsMargins(12, 12, 12, 12);
    layout->setSpacing(8);

    QLabel* hintLabel = new QLabel(tr("标签可用于分类和搜索方案"));
    hintLabel->setStyleSheet("color: gray;");
    layout->addWidget(hintLabel);

    // 标签列表
    tagList_ = new QListWidget;
    tagList_->setAlternatingRowColors(true);
    layout->addWidget(tagList_, 1);

    // 添加标签
    QHBoxLayout* addLayout = new QHBoxLayout;
    tagEdit_ = new QLineEdit;
    tagEdit_->setPlaceholderText(tr("输入新标签"));
    addTagBtn_ = new QPushButton(tr("添加"));
    addLayout->addWidget(tagEdit_, 1);
    addLayout->addWidget(addTagBtn_);
    layout->addLayout(addLayout);

    // 删除按钮
    removeTagBtn_ = new QPushButton(tr("删除选中标签"));
    removeTagBtn_->setEnabled(false);
    layout->addWidget(removeTagBtn_);

    // 连接选择变化
    connect(tagList_, &QListWidget::itemSelectionChanged, this, [this]() {
        removeTagBtn_->setEnabled(tagList_->currentItem() != nullptr);
    });

    return widget;
}

QWidget* RecipeEditorDialog::createCustomParamsPanel()
{
    QWidget* widget = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout(widget);
    layout->setContentsMargins(12, 12, 12, 12);
    layout->setSpacing(8);

    QLabel* hintLabel = new QLabel(tr("自定义参数可用于存储产品相关的额外配置"));
    hintLabel->setStyleSheet("color: gray;");
    layout->addWidget(hintLabel);

    // 参数表格
    paramTable_ = new QTableWidget;
    paramTable_->setColumnCount(2);
    paramTable_->setHorizontalHeaderLabels(QStringList() << tr("参数名") << tr("参数值"));
    paramTable_->horizontalHeader()->setStretchLastSection(true);
    paramTable_->setAlternatingRowColors(true);
    paramTable_->setSelectionBehavior(QAbstractItemView::SelectRows);
    layout->addWidget(paramTable_, 1);

    // 按钮
    QHBoxLayout* btnLayout = new QHBoxLayout;
    addParamBtn_ = new QPushButton(tr("添加参数"));
    removeParamBtn_ = new QPushButton(tr("删除参数"));
    removeParamBtn_->setEnabled(false);
    btnLayout->addWidget(addParamBtn_);
    btnLayout->addWidget(removeParamBtn_);
    btnLayout->addStretch();
    layout->addLayout(btnLayout);

    // 连接选择变化
    connect(paramTable_, &QTableWidget::itemSelectionChanged, this, [this]() {
        removeParamBtn_->setEnabled(paramTable_->currentRow() >= 0);
    });

    return widget;
}

QWidget* RecipeEditorDialog::createToolChainPreviewPanel()
{
    QWidget* widget = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout(widget);
    layout->setContentsMargins(12, 12, 12, 12);
    layout->setSpacing(8);

    QLabel* hintLabel = new QLabel(tr("方案中包含的工具链配置（只读）"));
    hintLabel->setStyleSheet("color: gray;");
    layout->addWidget(hintLabel);

    toolChainTable_ = new QTableWidget;
    toolChainTable_->setColumnCount(4);
    toolChainTable_->setHorizontalHeaderLabels(
        QStringList() << tr("序号") << tr("工具名称") << tr("工具类型") << tr("状态")
    );
    toolChainTable_->horizontalHeader()->setStretchLastSection(true);
    toolChainTable_->setAlternatingRowColors(true);
    toolChainTable_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    toolChainTable_->setSelectionBehavior(QAbstractItemView::SelectRows);
    layout->addWidget(toolChainTable_, 1);

    return widget;
}

QWidget* RecipeEditorDialog::createButtonPanel()
{
    QWidget* widget = new QWidget;
    QHBoxLayout* layout = new QHBoxLayout(widget);
    layout->setContentsMargins(0, 0, 0, 0);

    exportBtn_ = new QPushButton(tr("导出报告"));
    exportBtn_->setIcon(QIcon(":/icons/export.png"));

    layout->addWidget(exportBtn_);
    layout->addStretch();

    applyBtn_ = new QPushButton(tr("应用"));
    okBtn_ = new QPushButton(tr("确定"));
    cancelBtn_ = new QPushButton(tr("取消"));

    okBtn_->setDefault(true);

    layout->addWidget(applyBtn_);
    layout->addWidget(okBtn_);
    layout->addWidget(cancelBtn_);

    return widget;
}

void RecipeEditorDialog::setupConnections()
{
    // 标签管理
    connect(addTagBtn_, &QPushButton::clicked, this, &RecipeEditorDialog::onAddTag);
    connect(removeTagBtn_, &QPushButton::clicked, this, &RecipeEditorDialog::onRemoveTag);
    connect(tagEdit_, &QLineEdit::returnPressed, this, &RecipeEditorDialog::onAddTag);

    // 参数管理
    connect(addParamBtn_, &QPushButton::clicked, this, &RecipeEditorDialog::onAddParam);
    connect(removeParamBtn_, &QPushButton::clicked, this, &RecipeEditorDialog::onRemoveParam);
    connect(paramTable_, &QTableWidget::cellChanged, this, &RecipeEditorDialog::onParamChanged);

    // 对话框按钮
    connect(applyBtn_, &QPushButton::clicked, this, &RecipeEditorDialog::onApply);
    connect(okBtn_, &QPushButton::clicked, this, &RecipeEditorDialog::onOK);
    connect(cancelBtn_, &QPushButton::clicked, this, &RecipeEditorDialog::onCancel);

    // 导出
    connect(exportBtn_, &QPushButton::clicked, this, &RecipeEditorDialog::onExportReport);
}

void RecipeEditorDialog::loadFromRecipe()
{
    if (!recipe_) {
        return;
    }

    const Core::RecipeInfo& info = recipe_->info();

    // 基本信息
    nameEdit_->setText(info.name);
    descEdit_->setPlainText(info.description);
    versionEdit_->setText(info.version);
    authorEdit_->setText(info.author);

    QDateTime createTime = QDateTime::fromMSecsSinceEpoch(info.createTime);
    QDateTime modifyTime = QDateTime::fromMSecsSinceEpoch(info.modifyTime);
    createTimeLabel_->setText(createTime.toString("yyyy-MM-dd HH:mm:ss"));
    modifyTimeLabel_->setText(modifyTime.toString("yyyy-MM-dd HH:mm:ss"));

    // 产品信息
    productCodeEdit_->setText(info.productCode);

    // 标签
    tagList_->clear();
    for (const QString& tag : info.tags) {
        tagList_->addItem(tag);
    }

    // 自定义参数
    paramTable_->setRowCount(0);
    QStringList keys = recipe_->customParamKeys();
    for (const QString& key : keys) {
        int row = paramTable_->rowCount();
        paramTable_->insertRow(row);
        paramTable_->setItem(row, 0, new QTableWidgetItem(key));
        paramTable_->setItem(row, 1, new QTableWidgetItem(recipe_->customParam(key).toString()));
    }

    // 工具链预览
    updateToolChainPreview();
}

void RecipeEditorDialog::saveToRecipe()
{
    if (!recipe_) {
        return;
    }

    // 基本信息
    recipe_->setName(nameEdit_->text().trimmed());
    recipe_->setDescription(descEdit_->toPlainText().trimmed());
    recipe_->info().version = versionEdit_->text().trimmed();
    recipe_->info().author = authorEdit_->text().trimmed();

    // 产品信息
    recipe_->setProductCode(productCodeEdit_->text().trimmed());

    // 标签
    QStringList tags;
    for (int i = 0; i < tagList_->count(); ++i) {
        tags.append(tagList_->item(i)->text());
    }
    recipe_->setTags(tags);

    // 自定义参数
    recipe_->clearCustomParams();
    for (int i = 0; i < paramTable_->rowCount(); ++i) {
        QString key = paramTable_->item(i, 0)->text().trimmed();
        QString value = paramTable_->item(i, 1)->text().trimmed();
        if (!key.isEmpty()) {
            recipe_->setCustomParam(key, value);
        }
    }
}

void RecipeEditorDialog::updateToolChainPreview()
{
    toolChainTable_->setRowCount(0);

    if (!recipe_ || !recipe_->toolChain()) {
        return;
    }

    Core::ToolChain* toolChain = recipe_->toolChain();

    for (int i = 0; i < toolChain->toolCount(); ++i) {
        const Core::ToolChainNode* node = toolChain->getNode(i);
        if (!node) continue;

        int row = toolChainTable_->rowCount();
        toolChainTable_->insertRow(row);

        toolChainTable_->setItem(row, 0, new QTableWidgetItem(QString::number(i + 1)));

        QString name = node->name.isEmpty() ?
                      (node->tool ? node->tool->toolName() : tr("未知")) :
                      node->name;
        toolChainTable_->setItem(row, 1, new QTableWidgetItem(name));

        QString type = node->tool ? node->tool->toolName() : tr("未知");
        toolChainTable_->setItem(row, 2, new QTableWidgetItem(type));

        QString status = node->enabled ? tr("启用") : tr("禁用");
        QTableWidgetItem* statusItem = new QTableWidgetItem(status);
        statusItem->setForeground(node->enabled ? QColor(0, 128, 0) : QColor(128, 128, 128));
        toolChainTable_->setItem(row, 3, statusItem);
    }

    toolChainTable_->resizeColumnsToContents();
}

// ========== 槽函数 ==========

void RecipeEditorDialog::onAddTag()
{
    QString tag = tagEdit_->text().trimmed();
    if (tag.isEmpty()) {
        return;
    }

    // 检查是否已存在
    for (int i = 0; i < tagList_->count(); ++i) {
        if (tagList_->item(i)->text().compare(tag, Qt::CaseInsensitive) == 0) {
            QMessageBox::information(this, tr("提示"), tr("标签 \"%1\" 已存在").arg(tag));
            return;
        }
    }

    tagList_->addItem(tag);
    tagEdit_->clear();
}

void RecipeEditorDialog::onRemoveTag()
{
    QListWidgetItem* item = tagList_->currentItem();
    if (item) {
        delete tagList_->takeItem(tagList_->row(item));
    }
}

void RecipeEditorDialog::onAddParam()
{
    int row = paramTable_->rowCount();
    paramTable_->insertRow(row);
    paramTable_->setItem(row, 0, new QTableWidgetItem(tr("参数%1").arg(row + 1)));
    paramTable_->setItem(row, 1, new QTableWidgetItem(""));
    paramTable_->editItem(paramTable_->item(row, 0));
}

void RecipeEditorDialog::onRemoveParam()
{
    int row = paramTable_->currentRow();
    if (row >= 0) {
        paramTable_->removeRow(row);
    }
}

void RecipeEditorDialog::onParamChanged(int /*row*/, int /*column*/)
{
    // 可以在这里添加参数验证逻辑
}

void RecipeEditorDialog::onApply()
{
    if (nameEdit_->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, tr("错误"), tr("方案名称不能为空"));
        nameEdit_->setFocus();
        return;
    }

    saveToRecipe();
    LOG_INFO(QString("应用方案编辑: %1").arg(recipe_->name()));
}

void RecipeEditorDialog::onOK()
{
    onApply();
    accept();
}

void RecipeEditorDialog::onCancel()
{
    reject();
}

void RecipeEditorDialog::onExportReport()
{
    if (!recipe_) {
        return;
    }

    // 先保存当前编辑
    saveToRecipe();

    QString filePath = QFileDialog::getSaveFileName(
        this, tr("导出方案报告"),
        QDir::homePath() + "/" + recipe_->name() + "_report.txt",
        tr("文本文件 (*.txt)")
    );

    if (filePath.isEmpty()) {
        return;
    }

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("错误"), tr("无法创建文件: %1").arg(filePath));
        return;
    }

    QTextStream stream(&file);
    stream.setEncoding(QStringConverter::Utf8);
    stream << recipe_->exportAsText();
    file.close();

    QMessageBox::information(this, tr("成功"), tr("报告已导出到: %1").arg(filePath));
}

} // namespace VisionForge
