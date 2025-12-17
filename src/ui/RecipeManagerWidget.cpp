/**
 * @file RecipeManagerWidget.cpp
 * @brief 方案管理界面实现
 */

#include "ui/RecipeManagerWidget.h"
#include "ui/Theme.h"
#include "base/Logger.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>
#include <QDateTime>
#include <QApplication>

namespace VisionForge {

RecipeManagerWidget::RecipeManagerWidget(QWidget* parent)
    : QWidget(parent)
{
    setupUI();
    setupConnections();
    createContextMenu();
    refreshRecipeList();
}

RecipeManagerWidget::~RecipeManagerWidget()
{
}

void RecipeManagerWidget::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(8, 8, 8, 8);
    mainLayout->setSpacing(8);

    // 工具栏
    mainLayout->addWidget(createToolBar());

    // 搜索面板
    mainLayout->addWidget(createSearchPanel());

    // 方案列表
    mainLayout->addWidget(createRecipeListPanel(), 1);

    // 信息面板
    mainLayout->addWidget(createInfoPanel());

    // 状态栏
    statusLabel_ = new QLabel(tr("就绪"));
    statusLabel_->setStyleSheet("color: gray; font-size: 11px;");
    mainLayout->addWidget(statusLabel_);

    updateButtonStates();
}

QWidget* RecipeManagerWidget::createToolBar()
{
    QWidget* toolbar = new QWidget;
    QHBoxLayout* layout = new QHBoxLayout(toolbar);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(4);

    newBtn_ = new QPushButton(tr("新建"));
    newBtn_->setIcon(QIcon(":/icons/add.png"));
    newBtn_->setToolTip(tr("创建新方案"));

    openBtn_ = new QPushButton(tr("打开"));
    openBtn_->setIcon(QIcon(":/icons/open.png"));
    openBtn_->setToolTip(tr("从文件打开方案"));

    saveBtn_ = new QPushButton(tr("保存"));
    saveBtn_->setIcon(QIcon(":/icons/save.png"));
    saveBtn_->setToolTip(tr("保存当前方案"));

    deleteBtn_ = new QPushButton(tr("删除"));
    deleteBtn_->setIcon(QIcon(":/icons/delete.png"));
    deleteBtn_->setToolTip(tr("删除选中方案"));

    applyBtn_ = new QPushButton(tr("应用"));
    applyBtn_->setIcon(QIcon(":/icons/apply.png"));
    applyBtn_->setToolTip(tr("应用方案到当前工具链"));
    applyBtn_->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; }");

    layout->addWidget(newBtn_);
    layout->addWidget(openBtn_);
    layout->addWidget(saveBtn_);
    layout->addWidget(deleteBtn_);
    layout->addStretch();
    layout->addWidget(applyBtn_);

    return toolbar;
}

QWidget* RecipeManagerWidget::createSearchPanel()
{
    QWidget* panel = new QWidget;
    QHBoxLayout* layout = new QHBoxLayout(panel);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(8);

    searchEdit_ = new QLineEdit;
    searchEdit_->setPlaceholderText(tr("搜索方案..."));
    searchEdit_->setClearButtonEnabled(true);

    filterCombo_ = new QComboBox;
    filterCombo_->addItem(tr("全部"), "all");
    filterCombo_->addItem(tr("最近使用"), "recent");
    filterCombo_->setMinimumWidth(100);

    layout->addWidget(searchEdit_, 1);
    layout->addWidget(filterCombo_);

    return panel;
}

QWidget* RecipeManagerWidget::createRecipeListPanel()
{
    recipeList_ = new QListWidget;
    recipeList_->setContextMenuPolicy(Qt::CustomContextMenu);
    recipeList_->setSelectionMode(QAbstractItemView::SingleSelection);
    recipeList_->setAlternatingRowColors(true);
    recipeList_->setIconSize(QSize(24, 24));

    return recipeList_;
}

QWidget* RecipeManagerWidget::createInfoPanel()
{
    infoGroup_ = new QGroupBox(tr("方案信息"));
    QGridLayout* layout = new QGridLayout(infoGroup_);
    layout->setContentsMargins(8, 12, 8, 8);
    layout->setSpacing(4);

    int row = 0;

    layout->addWidget(new QLabel(tr("名称:")), row, 0);
    nameLabel_ = new QLabel("-");
    nameLabel_->setStyleSheet("font-weight: bold;");
    layout->addWidget(nameLabel_, row++, 1);

    layout->addWidget(new QLabel(tr("描述:")), row, 0);
    descLabel_ = new QLabel("-");
    descLabel_->setWordWrap(true);
    layout->addWidget(descLabel_, row++, 1);

    layout->addWidget(new QLabel(tr("产品编号:")), row, 0);
    productCodeLabel_ = new QLabel("-");
    layout->addWidget(productCodeLabel_, row++, 1);

    layout->addWidget(new QLabel(tr("标签:")), row, 0);
    tagsLabel_ = new QLabel("-");
    layout->addWidget(tagsLabel_, row++, 1);

    layout->addWidget(new QLabel(tr("工具数量:")), row, 0);
    toolCountLabel_ = new QLabel("-");
    layout->addWidget(toolCountLabel_, row++, 1);

    layout->addWidget(new QLabel(tr("修改时间:")), row, 0);
    modifyTimeLabel_ = new QLabel("-");
    layout->addWidget(modifyTimeLabel_, row++, 1);

    // 编辑按钮
    editBtn_ = new QPushButton(tr("编辑详情"));
    editBtn_->setEnabled(false);
    layout->addWidget(editBtn_, row, 0, 1, 2);

    layout->setColumnStretch(1, 1);

    return infoGroup_;
}

void RecipeManagerWidget::setupConnections()
{
    // 工具栏按钮
    connect(newBtn_, &QPushButton::clicked, this, &RecipeManagerWidget::onNewRecipe);
    connect(openBtn_, &QPushButton::clicked, this, &RecipeManagerWidget::onOpenRecipe);
    connect(saveBtn_, &QPushButton::clicked, this, &RecipeManagerWidget::onSaveRecipe);
    connect(deleteBtn_, &QPushButton::clicked, this, &RecipeManagerWidget::onDeleteRecipe);
    connect(applyBtn_, &QPushButton::clicked, this, &RecipeManagerWidget::onApplyRecipe);

    // 编辑按钮
    connect(editBtn_, &QPushButton::clicked, this, &RecipeManagerWidget::onEditRecipe);

    // 列表操作
    connect(recipeList_, &QListWidget::itemSelectionChanged, this, [this]() {
        QListWidgetItem* item = recipeList_->currentItem();
        onRecipeSelected(item);
    });
    connect(recipeList_, &QListWidget::itemDoubleClicked, this, &RecipeManagerWidget::onRecipeDoubleClicked);
    connect(recipeList_, &QListWidget::customContextMenuRequested, this, &RecipeManagerWidget::onContextMenu);

    // 搜索和过滤
    connect(searchEdit_, &QLineEdit::textChanged, this, &RecipeManagerWidget::onSearchTextChanged);
    connect(filterCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &RecipeManagerWidget::onFilterChanged);

    // RecipeManager信号
    connect(&Core::RecipeManager::instance(), &Core::RecipeManager::recipeListChanged,
            this, &RecipeManagerWidget::onRecipeListChanged);
    connect(&Core::RecipeManager::instance(), &Core::RecipeManager::currentRecipeChanged,
            this, &RecipeManagerWidget::onCurrentRecipeChanged);
}

void RecipeManagerWidget::createContextMenu()
{
    contextMenu_ = new QMenu(this);

    contextMenu_->addAction(QIcon(":/icons/apply.png"), tr("应用方案"), this, &RecipeManagerWidget::onApplyRecipe);
    contextMenu_->addSeparator();
    contextMenu_->addAction(QIcon(":/icons/edit.png"), tr("编辑"), this, &RecipeManagerWidget::onEditRecipe);
    contextMenu_->addAction(QIcon(":/icons/copy.png"), tr("复制"), this, &RecipeManagerWidget::onDuplicateRecipe);
    contextMenu_->addAction(QIcon(":/icons/save.png"), tr("另存为..."), this, &RecipeManagerWidget::onSaveRecipeAs);
    contextMenu_->addSeparator();
    contextMenu_->addAction(QIcon(":/icons/export.png"), tr("导出..."), this, &RecipeManagerWidget::onExportRecipe);
    contextMenu_->addSeparator();
    contextMenu_->addAction(QIcon(":/icons/delete.png"), tr("删除"), this, &RecipeManagerWidget::onDeleteRecipe);
}

void RecipeManagerWidget::refreshRecipeList()
{
    recipeList_->clear();

    QString filterType = filterCombo_->currentData().toString();
    QString searchText = searchEdit_->text().toLower();

    QList<Core::Recipe*> recipes;

    if (filterType == "recent") {
        // 最近使用
        QStringList recent = Core::RecipeManager::instance().getRecentRecipes();
        for (const QString& name : recent) {
            Core::Recipe* recipe = Core::RecipeManager::instance().getRecipe(name);
            if (recipe) {
                recipes.append(recipe);
            }
        }
    } else {
        // 全部
        recipes = Core::RecipeManager::instance().getAllRecipes();
    }

    for (Core::Recipe* recipe : recipes) {
        // 搜索过滤
        if (!searchText.isEmpty()) {
            bool match = recipe->name().toLower().contains(searchText) ||
                        recipe->description().toLower().contains(searchText) ||
                        recipe->productCode().toLower().contains(searchText);

            // 检查标签
            if (!match) {
                for (const QString& tag : recipe->tags()) {
                    if (tag.toLower().contains(searchText)) {
                        match = true;
                        break;
                    }
                }
            }

            if (!match) {
                continue;
            }
        }

        QListWidgetItem* item = new QListWidgetItem(getRecipeDisplayName(recipe));
        item->setData(Qt::UserRole, recipe->name());
        item->setIcon(QIcon(":/icons/recipe.png"));

        // 标记当前方案
        if (recipe == Core::RecipeManager::instance().currentRecipe()) {
            item->setBackground(QColor(200, 230, 200));
            QFont font = item->font();
            font.setBold(true);
            item->setFont(font);
        }

        // 标记已修改
        if (recipe->isModified()) {
            item->setText(item->text() + " *");
        }

        recipeList_->addItem(item);
    }

    statusLabel_->setText(tr("共 %1 个方案").arg(recipeList_->count()));
    updateButtonStates();
}

Core::Recipe* RecipeManagerWidget::currentRecipe() const
{
    QListWidgetItem* item = recipeList_->currentItem();
    if (!item) {
        return nullptr;
    }

    QString name = item->data(Qt::UserRole).toString();
    return Core::RecipeManager::instance().getRecipe(name);
}

void RecipeManagerWidget::setRecipeDirectory(const QString& path)
{
    Core::RecipeManager::instance().setRecipeDirectory(path);
    Core::RecipeManager::instance().scanRecipeDirectory();
    refreshRecipeList();
}

// ========== 方案操作槽函数 ==========

void RecipeManagerWidget::onNewRecipe()
{
    bool ok;
    QString name = QInputDialog::getText(this, tr("新建方案"),
                                         tr("方案名称:"), QLineEdit::Normal,
                                         tr("新方案"), &ok);
    if (!ok || name.isEmpty()) {
        return;
    }

    // 检查是否已存在
    if (Core::RecipeManager::instance().hasRecipe(name)) {
        QMessageBox::warning(this, tr("错误"), tr("方案 \"%1\" 已存在").arg(name));
        return;
    }

    Core::Recipe* recipe = Core::RecipeManager::instance().createRecipe(name);
    if (recipe) {
        refreshRecipeList();
        emit recipeCreated(recipe);
        statusLabel_->setText(tr("已创建方案: %1").arg(name));

        // 选中新创建的方案
        for (int i = 0; i < recipeList_->count(); ++i) {
            if (recipeList_->item(i)->data(Qt::UserRole).toString() == name) {
                recipeList_->setCurrentRow(i);
                break;
            }
        }
    }
}

void RecipeManagerWidget::onOpenRecipe()
{
    QString filePath = QFileDialog::getOpenFileName(
        this, tr("打开方案"),
        Core::RecipeManager::instance().recipeDirectory(),
        tr("VisionForge方案 (*.vfr);;所有文件 (*)")
    );

    if (filePath.isEmpty()) {
        return;
    }

    Core::Recipe* recipe = Core::RecipeManager::instance().loadRecipe(filePath);
    if (recipe) {
        refreshRecipeList();
        statusLabel_->setText(tr("已加载方案: %1").arg(recipe->name()));
    } else {
        QMessageBox::warning(this, tr("错误"), tr("无法加载方案文件"));
    }
}

void RecipeManagerWidget::onSaveRecipe()
{
    Core::Recipe* recipe = currentRecipe();
    if (!recipe) {
        return;
    }

    if (Core::RecipeManager::instance().saveRecipe(recipe)) {
        refreshRecipeList();
        statusLabel_->setText(tr("已保存方案: %1").arg(recipe->name()));
    } else {
        QMessageBox::warning(this, tr("错误"), tr("保存方案失败"));
    }
}

void RecipeManagerWidget::onSaveRecipeAs()
{
    Core::Recipe* recipe = currentRecipe();
    if (!recipe) {
        return;
    }

    QString filePath = QFileDialog::getSaveFileName(
        this, tr("另存为"),
        Core::RecipeManager::instance().recipeDirectory() + "/" + recipe->name() + ".vfr",
        tr("VisionForge方案 (*.vfr)")
    );

    if (filePath.isEmpty()) {
        return;
    }

    if (Core::RecipeManager::instance().saveRecipe(recipe, filePath)) {
        statusLabel_->setText(tr("已保存方案: %1").arg(filePath));
    } else {
        QMessageBox::warning(this, tr("错误"), tr("保存方案失败"));
    }
}

void RecipeManagerWidget::onDeleteRecipe()
{
    Core::Recipe* recipe = currentRecipe();
    if (!recipe) {
        return;
    }

    QString name = recipe->name();

    int ret = QMessageBox::question(
        this, tr("确认删除"),
        tr("确定要删除方案 \"%1\" 吗？\n此操作不可撤销。").arg(name),
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No
    );

    if (ret != QMessageBox::Yes) {
        return;
    }

    if (Core::RecipeManager::instance().removeRecipe(name, true)) {
        refreshRecipeList();
        emit recipeDeleted(name);
        statusLabel_->setText(tr("已删除方案: %1").arg(name));
    } else {
        QMessageBox::warning(this, tr("错误"), tr("删除方案失败"));
    }
}

void RecipeManagerWidget::onDuplicateRecipe()
{
    Core::Recipe* recipe = currentRecipe();
    if (!recipe) {
        return;
    }

    bool ok;
    QString newName = QInputDialog::getText(this, tr("复制方案"),
                                            tr("新方案名称:"), QLineEdit::Normal,
                                            recipe->name() + tr("_副本"), &ok);
    if (!ok || newName.isEmpty()) {
        return;
    }

    if (Core::RecipeManager::instance().hasRecipe(newName)) {
        QMessageBox::warning(this, tr("错误"), tr("方案 \"%1\" 已存在").arg(newName));
        return;
    }

    Core::Recipe* newRecipe = Core::RecipeManager::instance().duplicateRecipe(recipe->name(), newName);
    if (newRecipe) {
        refreshRecipeList();
        statusLabel_->setText(tr("已复制方案: %1 -> %2").arg(recipe->name()).arg(newName));
    } else {
        QMessageBox::warning(this, tr("错误"), tr("复制方案失败"));
    }
}

void RecipeManagerWidget::onExportRecipe()
{
    Core::Recipe* recipe = currentRecipe();
    if (!recipe) {
        return;
    }

    QString filePath = QFileDialog::getSaveFileName(
        this, tr("导出方案"),
        QDir::homePath() + "/" + recipe->name() + ".vfr",
        tr("VisionForge方案 (*.vfr)")
    );

    if (filePath.isEmpty()) {
        return;
    }

    if (Core::RecipeManager::instance().exportRecipe(recipe->name(), filePath)) {
        statusLabel_->setText(tr("已导出方案: %1").arg(filePath));
    } else {
        QMessageBox::warning(this, tr("错误"), tr("导出方案失败"));
    }
}

void RecipeManagerWidget::onImportRecipe()
{
    QString filePath = QFileDialog::getOpenFileName(
        this, tr("导入方案"),
        QDir::homePath(),
        tr("VisionForge方案 (*.vfr);;所有文件 (*)")
    );

    if (filePath.isEmpty()) {
        return;
    }

    Core::Recipe* recipe = Core::RecipeManager::instance().importRecipe(filePath);
    if (recipe) {
        refreshRecipeList();
        statusLabel_->setText(tr("已导入方案: %1").arg(recipe->name()));
    } else {
        QMessageBox::warning(this, tr("错误"), tr("导入方案失败"));
    }
}

// ========== 列表操作槽函数 ==========

void RecipeManagerWidget::onRecipeSelected(QListWidgetItem* item)
{
    if (!item) {
        updateRecipeInfo(nullptr);
        return;
    }

    QString name = item->data(Qt::UserRole).toString();
    Core::Recipe* recipe = Core::RecipeManager::instance().getRecipe(name);
    updateRecipeInfo(recipe);
    emit recipeSelected(recipe);
}

void RecipeManagerWidget::onRecipeDoubleClicked(QListWidgetItem* item)
{
    if (!item) {
        return;
    }

    QString name = item->data(Qt::UserRole).toString();
    Core::Recipe* recipe = Core::RecipeManager::instance().getRecipe(name);
    if (recipe) {
        emit recipeActivated(recipe);
    }
}

void RecipeManagerWidget::onContextMenu(const QPoint& pos)
{
    QListWidgetItem* item = recipeList_->itemAt(pos);
    if (item) {
        contextMenu_->exec(recipeList_->mapToGlobal(pos));
    }
}

void RecipeManagerWidget::onSearchTextChanged(const QString& /*text*/)
{
    refreshRecipeList();
}

void RecipeManagerWidget::onFilterChanged(int /*index*/)
{
    // 更新标签过滤选项
    filterCombo_->clear();
    filterCombo_->addItem(tr("全部"), "all");
    filterCombo_->addItem(tr("最近使用"), "recent");

    // 添加所有标签
    QStringList tags = Core::RecipeManager::instance().getAllTags();
    if (!tags.isEmpty()) {
        filterCombo_->insertSeparator(filterCombo_->count());
        for (const QString& tag : tags) {
            filterCombo_->addItem(tr("标签: %1").arg(tag), "tag:" + tag);
        }
    }

    // 添加所有产品编号
    QStringList codes = Core::RecipeManager::instance().getAllProductCodes();
    if (!codes.isEmpty()) {
        filterCombo_->insertSeparator(filterCombo_->count());
        for (const QString& code : codes) {
            filterCombo_->addItem(tr("产品: %1").arg(code), "product:" + code);
        }
    }

    refreshRecipeList();
}

void RecipeManagerWidget::onApplyRecipe()
{
    Core::Recipe* recipe = currentRecipe();
    if (recipe) {
        Core::RecipeManager::instance().setCurrentRecipe(recipe->name());
        emit recipeActivated(recipe);
        refreshRecipeList();
        statusLabel_->setText(tr("已应用方案: %1").arg(recipe->name()));
    }
}

void RecipeManagerWidget::onEditRecipe()
{
    Core::Recipe* recipe = currentRecipe();
    if (recipe) {
        emit editRecipeRequested(recipe);
    }
}

void RecipeManagerWidget::onRecipeListChanged()
{
    refreshRecipeList();
}

void RecipeManagerWidget::onCurrentRecipeChanged(const QString& name)
{
    refreshRecipeList();
    statusLabel_->setText(tr("当前方案: %1").arg(name.isEmpty() ? tr("无") : name));
}

// ========== 私有方法 ==========

void RecipeManagerWidget::updateRecipeInfo(Core::Recipe* recipe)
{
    if (!recipe) {
        nameLabel_->setText("-");
        descLabel_->setText("-");
        productCodeLabel_->setText("-");
        tagsLabel_->setText("-");
        toolCountLabel_->setText("-");
        modifyTimeLabel_->setText("-");
        editBtn_->setEnabled(false);
        return;
    }

    nameLabel_->setText(recipe->name());
    descLabel_->setText(recipe->description().isEmpty() ? tr("(无描述)") : recipe->description());
    productCodeLabel_->setText(recipe->productCode().isEmpty() ? "-" : recipe->productCode());
    tagsLabel_->setText(recipe->tags().isEmpty() ? "-" : recipe->tags().join(", "));

    if (recipe->toolChain()) {
        toolCountLabel_->setText(QString::number(recipe->toolChain()->toolCount()));
    } else {
        toolCountLabel_->setText("0");
    }

    QDateTime modifyTime = QDateTime::fromMSecsSinceEpoch(recipe->info().modifyTime);
    modifyTimeLabel_->setText(modifyTime.toString("yyyy-MM-dd HH:mm:ss"));

    editBtn_->setEnabled(true);
}

void RecipeManagerWidget::updateButtonStates()
{
    bool hasSelection = recipeList_->currentItem() != nullptr;

    saveBtn_->setEnabled(hasSelection);
    deleteBtn_->setEnabled(hasSelection);
    applyBtn_->setEnabled(hasSelection);
}

QString RecipeManagerWidget::getRecipeDisplayName(Core::Recipe* recipe) const
{
    if (!recipe) {
        return QString();
    }

    QString name = recipe->name();

    if (!recipe->productCode().isEmpty()) {
        name += QString(" [%1]").arg(recipe->productCode());
    }

    return name;
}

} // namespace VisionForge
