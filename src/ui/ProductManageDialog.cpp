/**
 * @file ProductManageDialog.cpp
 * @brief 产品管理对话框实现
 * @author VisionForge Team
 * @date 2025-01-04
 */

#include "ui/ProductManageDialog.h"
#include "base/Logger.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QSplitter>
#include <QTableWidget>
#include <QHeaderView>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QGroupBox>
#include <QScrollArea>
#include <QStackedWidget>
#include <QMessageBox>
#include <QInputDialog>
#include <QStyle>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>

namespace VisionForge {
namespace UI {

// ============================================================
// ProductInfo JSON序列化
// ============================================================

QJsonObject ProductInfo::toJson() const
{
    QJsonObject json;
    json["index"] = index;
    json["name"] = name;
    json["description"] = description;
    json["enabled"] = enabled;
    json["workStationId"] = workStationId;

    // 场景列表
    QJsonArray scenesArray;
    for (const auto& scene : scenes) {
        QJsonObject sceneObj;
        sceneObj["id"] = scene.id;
        sceneObj["name"] = scene.name;
        sceneObj["imagePath"] = scene.imagePath;
        sceneObj["cameraParams"] = scene.cameraParams;
        sceneObj["enabled"] = scene.enabled;
        scenesArray.append(sceneObj);
    }
    json["scenes"] = scenesArray;

    json["alignParams"] = alignParams;
    json["calibParams"] = calibParams;
    json["customParams"] = customParams;
    json["inspectParams"] = inspectParams;

    return json;
}

ProductInfo ProductInfo::fromJson(const QJsonObject& json)
{
    ProductInfo info;
    info.index = json["index"].toInt();
    info.name = json["name"].toString();
    info.description = json["description"].toString();
    info.enabled = json["enabled"].toBool(true);
    info.workStationId = json["workStationId"].toString();

    // 场景列表
    QJsonArray scenesArray = json["scenes"].toArray();
    for (const auto& sceneVal : scenesArray) {
        QJsonObject sceneObj = sceneVal.toObject();
        SceneInfo scene;
        scene.id = sceneObj["id"].toString();
        scene.name = sceneObj["name"].toString();
        scene.imagePath = sceneObj["imagePath"].toString();
        scene.cameraParams = sceneObj["cameraParams"].toObject();
        scene.enabled = sceneObj["enabled"].toBool(true);
        info.scenes.append(scene);
    }

    info.alignParams = json["alignParams"].toObject();
    info.calibParams = json["calibParams"].toObject();
    info.customParams = json["customParams"].toObject();
    info.inspectParams = json["inspectParams"].toObject();

    return info;
}

// ============================================================
// ProductManageDialog 实现
// ============================================================

ProductManageDialog::ProductManageDialog(QWidget* parent)
    : QDialog(parent)
    , currentProductIndex_(-1)
    , currentSceneIndex_(0)
{
    setupUI();
    initDefaultProducts(80);
    updateProductTable();
    updateCopyComboBoxes();
}

ProductManageDialog::~ProductManageDialog()
{
}

void ProductManageDialog::setupUI()
{
    setWindowTitle(tr("产品管理"));
    setMinimumSize(1200, 800);
    resize(1400, 900);
    setWindowFlags(windowFlags() | Qt::WindowMaximizeButtonHint);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(8, 8, 8, 8);
    mainLayout->setSpacing(8);

    // 主分割器
    mainSplitter_ = new QSplitter(Qt::Horizontal, this);

    // 左侧面板 - 产品列表
    leftPanel_ = new QWidget(mainSplitter_);
    createLeftPanel(leftPanel_);

    // 右侧面板 - 内容区域
    rightPanel_ = new QWidget(mainSplitter_);
    createRightPanel(rightPanel_);

    mainSplitter_->addWidget(leftPanel_);
    mainSplitter_->addWidget(rightPanel_);
    mainSplitter_->setStretchFactor(0, 1);  // 左侧较窄
    mainSplitter_->setStretchFactor(1, 4);  // 右侧较宽
    mainSplitter_->setSizes({220, 980});

    mainLayout->addWidget(mainSplitter_, 1);

    // 底部按钮
    createButtonPanel();
}

void ProductManageDialog::createLeftPanel(QWidget* parent)
{
    QVBoxLayout* layout = new QVBoxLayout(parent);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(4);

    // 工位标签
    QHBoxLayout* headerLayout = new QHBoxLayout();
    stationLabel_ = new QLabel(tr("工位1"), parent);
    stationLabel_->setStyleSheet("font-weight: bold; font-size: 12px; padding: 4px;");
    headerLayout->addWidget(stationLabel_);

    QLabel* productIdLabel = new QLabel(parent);
    productIdLabel->setStyleSheet("font-size: 11px; color: #666;");
    headerLayout->addWidget(productIdLabel);
    headerLayout->addStretch();

    layout->addLayout(headerLayout);

    // 产品列表表格
    productTable_ = new QTableWidget(parent);
    productTable_->setColumnCount(2);
    productTable_->setHorizontalHeaderLabels({tr("序号"), tr("产品名称")});
    productTable_->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    productTable_->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    productTable_->setColumnWidth(0, 50);
    productTable_->setSelectionBehavior(QAbstractItemView::SelectRows);
    productTable_->setSelectionMode(QAbstractItemView::SingleSelection);
    productTable_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    productTable_->verticalHeader()->setVisible(false);
    productTable_->setAlternatingRowColors(true);
    productTable_->setStyleSheet(
        "QTableWidget { border: 1px solid #ccc; }"
        "QTableWidget::item:selected { background-color: #0078d7; color: white; }"
    );

    connect(productTable_, &QTableWidget::itemSelectionChanged,
            this, &ProductManageDialog::onProductSelectionChanged);
    connect(productTable_, &QTableWidget::cellDoubleClicked,
            this, &ProductManageDialog::onProductDoubleClicked);

    layout->addWidget(productTable_, 1);
}

void ProductManageDialog::createRightPanel(QWidget* parent)
{
    QVBoxLayout* layout = new QVBoxLayout(parent);
    layout->setContentsMargins(4, 0, 0, 0);
    layout->setSpacing(8);

    // 工具栏
    createToolBar(layout);

    // 内容区域
    createContentArea(layout);

    // 产品拷贝面板
    createCopyPanel(layout);
}

void ProductManageDialog::createToolBar(QVBoxLayout* layout)
{
    toolBarWidget_ = new QWidget(this);
    QHBoxLayout* toolLayout = new QHBoxLayout(toolBarWidget_);
    toolLayout->setContentsMargins(0, 0, 0, 0);
    toolLayout->setSpacing(8);

    // 按钮样式
    QString btnStyle =
        "QPushButton {"
        "  padding: 8px 16px;"
        "  font-size: 12px;"
        "  border: 1px solid #ccc;"
        "  border-radius: 4px;"
        "  background-color: #f0f0f0;"
        "}"
        "QPushButton:hover {"
        "  background-color: #e0e0e0;"
        "}"
        "QPushButton:pressed {"
        "  background-color: #d0d0d0;"
        "}";

    // 预览按钮
    previewBtn_ = new QPushButton(tr("预览"), toolBarWidget_);
    previewBtn_->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    previewBtn_->setStyleSheet(btnStyle);
    previewBtn_->setMinimumSize(80, 60);
    connect(previewBtn_, &QPushButton::clicked, this, &ProductManageDialog::onPreviewClicked);
    toolLayout->addWidget(previewBtn_);

    // 场景按钮
    sceneBtn_ = new QPushButton(tr("场景1"), toolBarWidget_);
    sceneBtn_->setIcon(style()->standardIcon(QStyle::SP_DesktopIcon));
    sceneBtn_->setStyleSheet(btnStyle);
    sceneBtn_->setMinimumSize(80, 60);
    connect(sceneBtn_, &QPushButton::clicked, this, &ProductManageDialog::onSceneClicked);
    toolLayout->addWidget(sceneBtn_);

    // 标定按钮
    calibBtn_ = new QPushButton(tr("标定1"), toolBarWidget_);
    calibBtn_->setIcon(style()->standardIcon(QStyle::SP_FileDialogContentsView));
    calibBtn_->setStyleSheet(btnStyle);
    calibBtn_->setMinimumSize(80, 60);
    connect(calibBtn_, &QPushButton::clicked, this, &ProductManageDialog::onCalibrationClicked);
    toolLayout->addWidget(calibBtn_);

    // 对位按钮
    alignBtn_ = new QPushButton(tr("对位"), toolBarWidget_);
    alignBtn_->setIcon(style()->standardIcon(QStyle::SP_CommandLink));
    alignBtn_->setStyleSheet(btnStyle);
    alignBtn_->setMinimumSize(80, 60);
    connect(alignBtn_, &QPushButton::clicked, this, &ProductManageDialog::onAlignmentClicked);
    toolLayout->addWidget(alignBtn_);

    // 定制按钮
    customBtn_ = new QPushButton(tr("定制1"), toolBarWidget_);
    customBtn_->setIcon(style()->standardIcon(QStyle::SP_FileDialogDetailedView));
    customBtn_->setStyleSheet(btnStyle);
    customBtn_->setMinimumSize(80, 60);
    connect(customBtn_, &QPushButton::clicked, this, &ProductManageDialog::onCustomClicked);
    toolLayout->addWidget(customBtn_);

    toolLayout->addStretch();

    // 边检按钮（右侧）
    inspectBtn_ = new QPushButton(tr("边检"), toolBarWidget_);
    inspectBtn_->setIcon(style()->standardIcon(QStyle::SP_DialogApplyButton));
    inspectBtn_->setStyleSheet(
        "QPushButton {"
        "  padding: 8px 16px;"
        "  font-size: 12px;"
        "  border: 1px solid #0078d7;"
        "  border-radius: 4px;"
        "  background-color: #0078d7;"
        "  color: white;"
        "}"
        "QPushButton:hover {"
        "  background-color: #005a9e;"
        "}"
    );
    inspectBtn_->setMinimumSize(80, 60);
    connect(inspectBtn_, &QPushButton::clicked, this, &ProductManageDialog::onInspectionClicked);
    toolLayout->addWidget(inspectBtn_);

    layout->addWidget(toolBarWidget_);
}

void ProductManageDialog::createContentArea(QVBoxLayout* layout)
{
    contentGroup_ = new QGroupBox(this);
    contentGroup_->setStyleSheet(
        "QGroupBox {"
        "  border: 1px solid #ccc;"
        "  border-radius: 4px;"
        "  margin-top: 8px;"
        "  padding-top: 16px;"
        "}"
    );

    QVBoxLayout* contentLayout = new QVBoxLayout(contentGroup_);
    contentLayout->setContentsMargins(8, 8, 8, 8);
    contentLayout->setSpacing(8);

    // 产品标题
    productTitleLabel_ = new QLabel(tr("-产品1-场景1-1"), contentGroup_);
    productTitleLabel_->setStyleSheet("font-size: 14px; font-weight: bold; color: #333;");
    contentLayout->addWidget(productTitleLabel_);

    // 场景滚动区域
    sceneScrollArea_ = new QScrollArea(contentGroup_);
    sceneScrollArea_->setWidgetResizable(true);
    sceneScrollArea_->setFrameShape(QFrame::NoFrame);

    sceneContainer_ = new QWidget(sceneScrollArea_);
    QVBoxLayout* sceneLayout = new QVBoxLayout(sceneContainer_);
    sceneLayout->setContentsMargins(0, 0, 0, 0);
    sceneLayout->setSpacing(8);

    // 创建3个场景图片区域
    for (int i = 0; i < 3; ++i) {
        QLabel* imageLabel = new QLabel(sceneContainer_);
        imageLabel->setMinimumSize(120, 100);
        imageLabel->setMaximumSize(150, 120);
        imageLabel->setStyleSheet(
            "QLabel {"
            "  border: 1px solid #999;"
            "  background-color: #1a1a2e;"
            "}"
        );
        imageLabel->setAlignment(Qt::AlignCenter);
        sceneLayout->addWidget(imageLabel);
        sceneImageLabels_.append(imageLabel);
    }

    // 拍照参数按钮
    QPushButton* cameraParamBtn = new QPushButton(tr("拍照参数1"), sceneContainer_);
    cameraParamBtn->setStyleSheet(
        "QPushButton {"
        "  padding: 6px 12px;"
        "  border: 1px solid #ccc;"
        "  border-radius: 3px;"
        "  background-color: #f5f5f5;"
        "}"
    );
    connect(cameraParamBtn, &QPushButton::clicked, this, &ProductManageDialog::onCameraParamClicked);
    sceneLayout->addWidget(cameraParamBtn);
    cameraParamBtns_.append(cameraParamBtn);

    sceneLayout->addStretch();
    sceneScrollArea_->setWidget(sceneContainer_);
    contentLayout->addWidget(sceneScrollArea_, 1);

    layout->addWidget(contentGroup_, 1);
}

void ProductManageDialog::createCopyPanel(QVBoxLayout* layout)
{
    copyPanel_ = new QWidget(this);
    QHBoxLayout* copyLayout = new QHBoxLayout(copyPanel_);
    copyLayout->setContentsMargins(0, 8, 0, 0);
    copyLayout->setSpacing(8);

    // 源产品下拉框
    copyFromCombo_ = new QComboBox(copyPanel_);
    copyFromCombo_->setMinimumWidth(200);
    copyLayout->addWidget(copyFromCombo_);

    // 箭头按钮
    copyArrowBtn_ = new QPushButton("=>>", copyPanel_);
    copyArrowBtn_->setMaximumWidth(50);
    copyLayout->addWidget(copyArrowBtn_);

    // 目标产品下拉框
    copyToCombo_ = new QComboBox(copyPanel_);
    copyToCombo_->setMinimumWidth(200);
    copyLayout->addWidget(copyToCombo_);

    copyLayout->addStretch();

    // 产品拷贝按钮
    copyProductBtn_ = new QPushButton(tr("产品拷贝"), copyPanel_);
    copyProductBtn_->setStyleSheet(
        "QPushButton {"
        "  padding: 8px 20px;"
        "  border: 1px solid #ccc;"
        "  border-radius: 4px;"
        "  background-color: #f0f0f0;"
        "}"
        "QPushButton:hover {"
        "  background-color: #e0e0e0;"
        "}"
    );
    connect(copyProductBtn_, &QPushButton::clicked, this, &ProductManageDialog::onCopyProduct);
    copyLayout->addWidget(copyProductBtn_);

    layout->addWidget(copyPanel_);
}

void ProductManageDialog::createButtonPanel()
{
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->setContentsMargins(0, 8, 0, 0);
    buttonLayout->addStretch();

    // 确定按钮
    okBtn_ = new QPushButton(this);
    okBtn_->setIcon(style()->standardIcon(QStyle::SP_DialogApplyButton));
    okBtn_->setIconSize(QSize(48, 48));
    okBtn_->setMinimumSize(80, 80);
    okBtn_->setStyleSheet(
        "QPushButton {"
        "  border: none;"
        "  background-color: #0078d7;"
        "  border-radius: 8px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #005a9e;"
        "}"
    );
    okBtn_->setToolTip(tr("确定"));
    connect(okBtn_, &QPushButton::clicked, this, &ProductManageDialog::onOkClicked);
    buttonLayout->addWidget(okBtn_);

    // 取消按钮
    cancelBtn_ = new QPushButton(this);
    cancelBtn_->setIcon(style()->standardIcon(QStyle::SP_DialogCancelButton));
    cancelBtn_->setIconSize(QSize(48, 48));
    cancelBtn_->setMinimumSize(80, 80);
    cancelBtn_->setStyleSheet(
        "QPushButton {"
        "  border: none;"
        "  background-color: #0078d7;"
        "  border-radius: 8px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #005a9e;"
        "}"
    );
    cancelBtn_->setToolTip(tr("取消"));
    connect(cancelBtn_, &QPushButton::clicked, this, &ProductManageDialog::onCancelClicked);
    buttonLayout->addWidget(cancelBtn_);

    static_cast<QVBoxLayout*>(layout())->addLayout(buttonLayout);
}

void ProductManageDialog::initDefaultProducts(int count)
{
    products_.clear();
    for (int i = 0; i < count; ++i) {
        ProductInfo product(i + 1, generateProductName(i + 1));

        // 默认添加一个场景
        SceneInfo scene(QString("scene_%1_1").arg(i + 1), tr("场景1"));
        product.scenes.append(scene);

        products_.append(product);
    }
}

QString ProductManageDialog::generateProductName(int index)
{
    return QString("Job%1").arg(index, 3, 10, QChar('0'));
}

void ProductManageDialog::updateProductTable()
{
    productTable_->setRowCount(products_.size());

    for (int i = 0; i < products_.size(); ++i) {
        const auto& product = products_[i];

        // 序号
        QTableWidgetItem* indexItem = new QTableWidgetItem(QString::number(product.index).rightJustified(3, '0'));
        indexItem->setTextAlignment(Qt::AlignCenter);
        productTable_->setItem(i, 0, indexItem);

        // 产品名称
        QTableWidgetItem* nameItem = new QTableWidgetItem(product.name);
        productTable_->setItem(i, 1, nameItem);
    }

    // 选中当前产品
    if (currentProductIndex_ >= 0 && currentProductIndex_ < products_.size()) {
        productTable_->selectRow(currentProductIndex_);
    }
}

void ProductManageDialog::updateProductContent()
{
    if (currentProductIndex_ < 0 || currentProductIndex_ >= products_.size()) {
        productTitleLabel_->setText(tr("-未选择产品-"));
        return;
    }

    const auto& product = products_[currentProductIndex_];

    // 更新标题
    QString title = QString("-产品%1-场景%2-1")
        .arg(product.index)
        .arg(currentSceneIndex_ + 1);
    productTitleLabel_->setText(title);

    // 更新场景显示
    updateSceneDisplay();
}

void ProductManageDialog::updateSceneDisplay()
{
    // 可以在这里加载场景图片
    // 目前只显示占位
}

void ProductManageDialog::updateCopyComboBoxes()
{
    copyFromCombo_->clear();
    copyToCombo_->clear();

    for (const auto& product : products_) {
        QString text = QString("%1|%2").arg(product.index, 3, 10, QChar('0')).arg(product.name);
        copyFromCombo_->addItem(text, product.index);
        copyToCombo_->addItem(text, product.index);
    }

    // 设置当前选择
    if (currentProductIndex_ >= 0) {
        copyToCombo_->setCurrentIndex(currentProductIndex_);
    }
}

// ============================================================
// 槽函数实现
// ============================================================

void ProductManageDialog::onProductSelectionChanged()
{
    int row = productTable_->currentRow();
    if (row >= 0 && row < products_.size()) {
        currentProductIndex_ = row;
        updateProductContent();
        emit productSelected(currentProductIndex_);

        // 更新工位标签显示
        if (row < products_.size()) {
            QString productId = QString("%1|%2")
                .arg(products_[row].index, 3, 10, QChar('0'))
                .arg(products_[row].name);
            // 可以更新显示
        }
    }
}

void ProductManageDialog::onProductDoubleClicked(int row, int column)
{
    Q_UNUSED(column);
    if (row >= 0 && row < products_.size()) {
        onRenameProduct();
    }
}

void ProductManageDialog::onAddProduct()
{
    int newIndex = products_.size() + 1;
    addProduct(generateProductName(newIndex));
}

void ProductManageDialog::onRemoveProduct()
{
    if (currentProductIndex_ < 0 || currentProductIndex_ >= products_.size()) {
        return;
    }

    int ret = QMessageBox::question(this, tr("确认删除"),
        tr("确定要删除产品 %1 吗？").arg(products_[currentProductIndex_].name),
        QMessageBox::Yes | QMessageBox::No);

    if (ret == QMessageBox::Yes) {
        removeProduct(currentProductIndex_);
    }
}

void ProductManageDialog::onRenameProduct()
{
    if (currentProductIndex_ < 0 || currentProductIndex_ >= products_.size()) {
        return;
    }

    bool ok;
    QString newName = QInputDialog::getText(this, tr("重命名产品"),
        tr("请输入新的产品名称:"),
        QLineEdit::Normal,
        products_[currentProductIndex_].name, &ok);

    if (ok && !newName.isEmpty()) {
        products_[currentProductIndex_].name = newName;
        updateProductTable();
        updateCopyComboBoxes();
        emit productChanged(currentProductIndex_);
    }
}

void ProductManageDialog::onPreviewClicked()
{
    if (currentProductIndex_ >= 0) {
        emit previewRequested(currentProductIndex_, currentSceneIndex_);
    }
}

void ProductManageDialog::onSceneClicked()
{
    // 切换场景
    if (currentProductIndex_ >= 0 && currentProductIndex_ < products_.size()) {
        int sceneCount = products_[currentProductIndex_].scenes.size();
        if (sceneCount > 0) {
            currentSceneIndex_ = (currentSceneIndex_ + 1) % sceneCount;
            sceneBtn_->setText(tr("场景%1").arg(currentSceneIndex_ + 1));
            updateProductContent();
        }
    }
}

void ProductManageDialog::onCalibrationClicked()
{
    if (currentProductIndex_ >= 0) {
        emit calibrationRequested(currentProductIndex_);
    }
}

void ProductManageDialog::onAlignmentClicked()
{
    if (currentProductIndex_ >= 0) {
        emit alignmentRequested(currentProductIndex_);
    }
}

void ProductManageDialog::onCustomClicked()
{
    if (currentProductIndex_ >= 0) {
        emit customSettingRequested(currentProductIndex_);
    }
}

void ProductManageDialog::onInspectionClicked()
{
    if (currentProductIndex_ >= 0) {
        emit inspectionRequested(currentProductIndex_);
    }
}

void ProductManageDialog::onCameraParamClicked()
{
    if (currentProductIndex_ >= 0) {
        emit cameraParamRequested(currentProductIndex_, currentSceneIndex_);
    }
}

void ProductManageDialog::onSceneImageClicked(int sceneIndex)
{
    currentSceneIndex_ = sceneIndex;
    updateProductContent();
}

void ProductManageDialog::onCopyProduct()
{
    int fromIndex = copyFromCombo_->currentData().toInt() - 1;
    int toIndex = copyToCombo_->currentData().toInt() - 1;

    if (fromIndex == toIndex) {
        QMessageBox::warning(this, tr("警告"), tr("源产品和目标产品相同！"));
        return;
    }

    if (fromIndex < 0 || fromIndex >= products_.size() ||
        toIndex < 0 || toIndex >= products_.size()) {
        QMessageBox::warning(this, tr("警告"), tr("无效的产品索引！"));
        return;
    }

    int ret = QMessageBox::question(this, tr("确认拷贝"),
        tr("确定要将产品 %1 的配置拷贝到产品 %2 吗？\n这将覆盖目标产品的现有配置。")
            .arg(products_[fromIndex].name)
            .arg(products_[toIndex].name),
        QMessageBox::Yes | QMessageBox::No);

    if (ret == QMessageBox::Yes) {
        if (copyProduct(fromIndex, toIndex)) {
            QMessageBox::information(this, tr("成功"), tr("产品配置拷贝成功！"));
        }
    }
}

void ProductManageDialog::onOkClicked()
{
    accept();
}

void ProductManageDialog::onCancelClicked()
{
    reject();
}

// ============================================================
// 公共接口
// ============================================================

void ProductManageDialog::setWorkStationId(const QString& wsId)
{
    workStationId_ = wsId;
    stationLabel_->setText(tr("工位1  %1").arg(wsId));
}

int ProductManageDialog::currentProductIndex() const
{
    return currentProductIndex_;
}

void ProductManageDialog::setCurrentProductIndex(int index)
{
    if (index >= 0 && index < products_.size()) {
        currentProductIndex_ = index;
        productTable_->selectRow(index);
        updateProductContent();
    }
}

int ProductManageDialog::productCount() const
{
    return products_.size();
}

ProductInfo ProductManageDialog::getProduct(int index) const
{
    if (index >= 0 && index < products_.size()) {
        return products_[index];
    }
    return ProductInfo();
}

void ProductManageDialog::setProduct(int index, const ProductInfo& product)
{
    if (index >= 0 && index < products_.size()) {
        products_[index] = product;
        updateProductTable();
        updateCopyComboBoxes();
        if (index == currentProductIndex_) {
            updateProductContent();
        }
        emit productChanged(index);
    }
}

int ProductManageDialog::addProduct(const QString& name)
{
    int newIndex = products_.size() + 1;
    ProductInfo product(newIndex, name.isEmpty() ? generateProductName(newIndex) : name);

    // 添加默认场景
    SceneInfo scene(QString("scene_%1_1").arg(newIndex), tr("场景1"));
    product.scenes.append(scene);

    products_.append(product);
    updateProductTable();
    updateCopyComboBoxes();

    return newIndex - 1;
}

bool ProductManageDialog::removeProduct(int index)
{
    if (index < 0 || index >= products_.size()) {
        return false;
    }

    products_.removeAt(index);

    // 重新编号
    for (int i = 0; i < products_.size(); ++i) {
        products_[i].index = i + 1;
        products_[i].name = generateProductName(i + 1);
    }

    updateProductTable();
    updateCopyComboBoxes();

    if (currentProductIndex_ >= products_.size()) {
        currentProductIndex_ = products_.size() - 1;
    }
    updateProductContent();

    return true;
}

bool ProductManageDialog::copyProduct(int fromIndex, int toIndex)
{
    if (fromIndex < 0 || fromIndex >= products_.size() ||
        toIndex < 0 || toIndex >= products_.size()) {
        return false;
    }

    // 保留目标产品的索引和名称
    int savedIndex = products_[toIndex].index;
    QString savedName = products_[toIndex].name;

    // 拷贝数据
    products_[toIndex] = products_[fromIndex];

    // 恢复索引和名称
    products_[toIndex].index = savedIndex;
    products_[toIndex].name = savedName;

    updateProductTable();
    emit productChanged(toIndex);

    LOG_INFO(QString("产品配置已从 %1 拷贝到 %2")
        .arg(products_[fromIndex].name)
        .arg(products_[toIndex].name));

    return true;
}

bool ProductManageDialog::loadConfig(const QString& configPath)
{
    QFile file(configPath);
    if (!file.open(QIODevice::ReadOnly)) {
        LOG_ERROR(QString("无法打开产品配置文件: %1").arg(configPath));
        return false;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    if (!doc.isObject()) {
        LOG_ERROR("产品配置文件格式错误");
        return false;
    }

    QJsonObject root = doc.object();
    QJsonArray productsArray = root["products"].toArray();

    products_.clear();
    for (const auto& productVal : productsArray) {
        products_.append(ProductInfo::fromJson(productVal.toObject()));
    }

    updateProductTable();
    updateCopyComboBoxes();

    LOG_INFO(QString("已加载 %1 个产品配置").arg(products_.size()));
    return true;
}

bool ProductManageDialog::saveConfig(const QString& configPath)
{
    QJsonObject root;

    QJsonArray productsArray;
    for (const auto& product : products_) {
        productsArray.append(product.toJson());
    }
    root["products"] = productsArray;
    root["workStationId"] = workStationId_;

    QFile file(configPath);
    if (!file.open(QIODevice::WriteOnly)) {
        LOG_ERROR(QString("无法保存产品配置文件: %1").arg(configPath));
        return false;
    }

    QJsonDocument doc(root);
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();

    LOG_INFO(QString("产品配置已保存到: %1").arg(configPath));
    return true;
}

} // namespace UI
} // namespace VisionForge
