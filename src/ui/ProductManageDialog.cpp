/**
 * @file ProductManageDialog.cpp
 * @brief 产品管理对话框实现
 * @author VisionForge Team
 * @date 2025-01-04
 */

#include "ui/ProductManageDialog.h"
#include "ui/ToolChainEditorDialog.h"
#include "base/Logger.h"

#include <QVBoxLayout>
#include <QEvent>
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
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QCheckBox>
#include <QRadioButton>
#include <QTableView>
#include <QStandardItemModel>
#include <QButtonGroup>

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
    , currentPanelIndex_(0)
    , contentStack_(nullptr)
    , scenePanel_(nullptr)
    , calibPanel_(nullptr)
    , alignPanel_(nullptr)
    , calibMatrixModel_(nullptr)
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

    // 内容堆栈
    contentStack_ = new QStackedWidget(contentGroup_);

    // 创建各面板
    createScenePanel();
    createCalibPanel();
    createAlignPanel();

    contentStack_->addWidget(scenePanel_);   // 索引0
    contentStack_->addWidget(calibPanel_);   // 索引1
    contentStack_->addWidget(alignPanel_);   // 索引2

    contentLayout->addWidget(contentStack_, 1);

    layout->addWidget(contentGroup_, 1);
}

void ProductManageDialog::createScenePanel()
{
    scenePanel_ = new QWidget();
    QVBoxLayout* panelLayout = new QVBoxLayout(scenePanel_);
    panelLayout->setContentsMargins(0, 0, 0, 0);
    panelLayout->setSpacing(8);

    // 场景滚动区域
    sceneScrollArea_ = new QScrollArea(scenePanel_);
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
        // 安装事件过滤器以捕获双击事件
        imageLabel->installEventFilter(this);
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
    panelLayout->addWidget(sceneScrollArea_, 1);
}

void ProductManageDialog::createCalibPanel()
{
    calibPanel_ = new QWidget();
    QVBoxLayout* panelLayout = new QVBoxLayout(calibPanel_);
    panelLayout->setContentsMargins(8, 8, 8, 8);
    panelLayout->setSpacing(12);

    // 顶部：位置和模式选择
    QHBoxLayout* topLayout = new QHBoxLayout();

    QLabel* locationLabel = new QLabel(tr("位置1"), calibPanel_);
    topLayout->addWidget(locationLabel);

    calibLocationCombo_ = new QComboBox(calibPanel_);
    calibLocationCombo_->addItem(tr("自动标定"));
    calibLocationCombo_->addItem(tr("手动标定"));
    calibLocationCombo_->addItem(tr("九点标定"));
    calibLocationCombo_->setMinimumWidth(120);
    topLayout->addWidget(calibLocationCombo_);
    topLayout->addStretch();

    panelLayout->addLayout(topLayout);

    // 标定矩阵表格
    QGroupBox* matrixGroup = new QGroupBox(calibPanel_);
    QVBoxLayout* matrixLayout = new QVBoxLayout(matrixGroup);

    calibMatrixModel_ = new QStandardItemModel(4, 3, this);
    calibMatrixView_ = new QTableView(calibPanel_);
    calibMatrixView_->setModel(calibMatrixModel_);
    calibMatrixView_->horizontalHeader()->setVisible(false);
    calibMatrixView_->verticalHeader()->setVisible(false);
    calibMatrixView_->setMinimumHeight(120);
    calibMatrixView_->setMaximumHeight(160);

    // 设置默认矩阵值（单位矩阵扩展）
    QStringList defaultValues = {
        "1.000000", "0.000000", "0.000000",
        "0.000000", "1.000000", "0.000000",
        "0.000000", "0.000000", "1.000000"
    };
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            QStandardItem* item = new QStandardItem(defaultValues[row * 3 + col]);
            item->setTextAlignment(Qt::AlignCenter);
            calibMatrixModel_->setItem(row, col, item);
        }
    }
    // 第四行：坐标标签
    calibMatrixModel_->setItem(3, 0, new QStandardItem(tr("平台坐标")));
    calibMatrixModel_->setItem(3, 1, new QStandardItem("0.000000"));
    calibMatrixModel_->setItem(3, 2, new QStandardItem("0.000000"));
    calibMatrixModel_->item(3, 0)->setTextAlignment(Qt::AlignCenter);

    calibMatrixView_->resizeColumnsToContents();
    matrixLayout->addWidget(calibMatrixView_);

    // 坐标输入行
    QGridLayout* coordLayout = new QGridLayout();

    coordLayout->addWidget(new QLabel(tr("平台坐标")), 0, 0);
    calibPlatformXSpin_ = new QDoubleSpinBox(calibPanel_);
    calibPlatformXSpin_->setRange(-99999, 99999);
    calibPlatformXSpin_->setDecimals(6);
    calibPlatformXSpin_->setValue(0.0);
    coordLayout->addWidget(calibPlatformXSpin_, 0, 1);
    calibPlatformYSpin_ = new QDoubleSpinBox(calibPanel_);
    calibPlatformYSpin_->setRange(-99999, 99999);
    calibPlatformYSpin_->setDecimals(6);
    calibPlatformYSpin_->setValue(0.0);
    coordLayout->addWidget(calibPlatformYSpin_, 0, 2);

    coordLayout->addWidget(new QLabel(tr("图像坐标")), 1, 0);
    calibImageXSpin_ = new QDoubleSpinBox(calibPanel_);
    calibImageXSpin_->setRange(-99999, 99999);
    calibImageXSpin_->setDecimals(6);
    calibImageXSpin_->setValue(0.0);
    coordLayout->addWidget(calibImageXSpin_, 1, 1);
    calibImageYSpin_ = new QDoubleSpinBox(calibPanel_);
    calibImageYSpin_->setRange(-99999, 99999);
    calibImageYSpin_->setDecimals(6);
    calibImageYSpin_->setValue(0.0);
    coordLayout->addWidget(calibImageYSpin_, 1, 2);

    coordLayout->addWidget(new QLabel(tr("标定位置")), 2, 0);
    calibPositionXSpin_ = new QDoubleSpinBox(calibPanel_);
    calibPositionXSpin_->setRange(-99999, 99999);
    calibPositionXSpin_->setDecimals(6);
    calibPositionXSpin_->setValue(0.0);
    coordLayout->addWidget(calibPositionXSpin_, 2, 1);
    calibPositionYSpin_ = new QDoubleSpinBox(calibPanel_);
    calibPositionYSpin_->setRange(-99999, 99999);
    calibPositionYSpin_->setDecimals(6);
    calibPositionYSpin_->setValue(0.0);
    coordLayout->addWidget(calibPositionYSpin_, 2, 2);

    matrixLayout->addLayout(coordLayout);
    panelLayout->addWidget(matrixGroup);

    // 底部按钮行
    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->setSpacing(8);

    calibSettingBtn_ = new QPushButton(tr("设置"), calibPanel_);
    calibSettingBtn_->setMinimumWidth(60);
    btnLayout->addWidget(calibSettingBtn_);

    calibTemplateBtn_ = new QPushButton(tr("模板"), calibPanel_);
    calibTemplateBtn_->setMinimumWidth(60);
    btnLayout->addWidget(calibTemplateBtn_);

    calibLinkBtn_ = new QPushButton(tr("关联"), calibPanel_);
    calibLinkBtn_->setMinimumWidth(60);
    btnLayout->addWidget(calibLinkBtn_);

    calibCopyBtn_ = new QPushButton(tr("复制"), calibPanel_);
    calibCopyBtn_->setMinimumWidth(60);
    btnLayout->addWidget(calibCopyBtn_);

    calibClearBtn_ = new QPushButton(tr("清空"), calibPanel_);
    calibClearBtn_->setMinimumWidth(60);
    btnLayout->addWidget(calibClearBtn_);

    btnLayout->addStretch();
    panelLayout->addLayout(btnLayout);

    panelLayout->addStretch();
}

void ProductManageDialog::createAlignPanel()
{
    alignPanel_ = new QWidget();
    QHBoxLayout* mainLayout = new QHBoxLayout(alignPanel_);
    mainLayout->setContentsMargins(8, 8, 8, 8);
    mainLayout->setSpacing(16);

    // ========== 左侧：对位精度 + 检查设置 ==========
    QVBoxLayout* leftLayout = new QVBoxLayout();
    leftLayout->setSpacing(12);

    // 对位精度组
    QGroupBox* precisionGroup = new QGroupBox(tr("对位精度"), alignPanel_);
    QGridLayout* precisionLayout = new QGridLayout(precisionGroup);
    precisionLayout->setSpacing(6);

    precisionLayout->addWidget(new QLabel(tr("设定精度X:")), 0, 0);
    alignPrecisionXSpin_ = new QDoubleSpinBox(alignPanel_);
    alignPrecisionXSpin_->setRange(0.001, 10.0);
    alignPrecisionXSpin_->setDecimals(2);
    alignPrecisionXSpin_->setValue(0.01);
    alignPrecisionXSpin_->setSuffix("°");
    precisionLayout->addWidget(alignPrecisionXSpin_, 0, 1);

    precisionLayout->addWidget(new QLabel(tr("设定精度Y:")), 1, 0);
    alignPrecisionYSpin_ = new QDoubleSpinBox(alignPanel_);
    alignPrecisionYSpin_->setRange(0.001, 10.0);
    alignPrecisionYSpin_->setDecimals(2);
    alignPrecisionYSpin_->setValue(0.01);
    alignPrecisionYSpin_->setSuffix("°");
    precisionLayout->addWidget(alignPrecisionYSpin_, 1, 1);

    precisionLayout->addWidget(new QLabel(tr("设定精度U:")), 2, 0);
    alignPrecisionUSpin_ = new QDoubleSpinBox(alignPanel_);
    alignPrecisionUSpin_->setRange(0.001, 10.0);
    alignPrecisionUSpin_->setDecimals(2);
    alignPrecisionUSpin_->setValue(0.01);
    alignPrecisionUSpin_->setSuffix("°");
    precisionLayout->addWidget(alignPrecisionUSpin_, 2, 1);

    precisionLayout->addWidget(new QLabel(tr("对位次数:")), 3, 0);
    alignCountSpin_ = new QSpinBox(alignPanel_);
    alignCountSpin_->setRange(1, 10);
    alignCountSpin_->setValue(1);
    alignCountSpin_->setSuffix(tr(" 次"));
    precisionLayout->addWidget(alignCountSpin_, 3, 1);

    leftLayout->addWidget(precisionGroup);

    // 检查设置组
    QGroupBox* checkGroup = new QGroupBox(tr("检查设置"), alignPanel_);
    QVBoxLayout* checkLayout = new QVBoxLayout(checkGroup);

    alignDistanceValidCheck_ = new QCheckBox(tr("检测距离是否有效"), alignPanel_);
    checkLayout->addWidget(alignDistanceValidCheck_);

    QHBoxLayout* modeLayout = new QHBoxLayout();
    alignInputModeRadio_ = new QRadioButton(tr("输入模式"), alignPanel_);
    alignCheckModeRadio_ = new QRadioButton(tr("检查模式"), alignPanel_);
    alignInputModeRadio_->setChecked(true);
    QButtonGroup* modeGroup = new QButtonGroup(this);
    modeGroup->addButton(alignInputModeRadio_);
    modeGroup->addButton(alignCheckModeRadio_);
    modeLayout->addWidget(alignInputModeRadio_);
    modeLayout->addWidget(alignCheckModeRadio_);
    checkLayout->addLayout(modeLayout);

    QGridLayout* distLayout = new QGridLayout();
    distLayout->addWidget(new QLabel(tr("对象距离偏差(mm):")), 0, 0);
    alignObjectDistSpin_ = new QDoubleSpinBox(alignPanel_);
    alignObjectDistSpin_->setRange(0.001, 100.0);
    alignObjectDistSpin_->setDecimals(2);
    alignObjectDistSpin_->setValue(0.01);
    distLayout->addWidget(alignObjectDistSpin_, 0, 1);

    distLayout->addWidget(new QLabel(tr("目标距离偏差(mm):")), 1, 0);
    alignTargetDistSpin_ = new QDoubleSpinBox(alignPanel_);
    alignTargetDistSpin_->setRange(0.001, 100.0);
    alignTargetDistSpin_->setDecimals(2);
    alignTargetDistSpin_->setValue(0.01);
    distLayout->addWidget(alignTargetDistSpin_, 1, 1);
    checkLayout->addLayout(distLayout);

    // 产品图示
    QHBoxLayout* productImgLayout = new QHBoxLayout();
    QPushButton* productImgBtn = new QPushButton(tr("产品图示"), alignPanel_);
    productImgLayout->addWidget(productImgBtn);
    alignProductImageLabel_ = new QLabel(alignPanel_);
    alignProductImageLabel_->setFixedSize(100, 80);
    alignProductImageLabel_->setStyleSheet("border: 1px solid #999; background-color: #1a1a2e;");
    productImgLayout->addWidget(alignProductImageLabel_);
    productImgLayout->addStretch();
    checkLayout->addLayout(productImgLayout);

    leftLayout->addWidget(checkGroup);
    leftLayout->addStretch();

    mainLayout->addLayout(leftLayout);

    // ========== 右侧：对位计算 + 对象补偿 ==========
    QVBoxLayout* rightLayout = new QVBoxLayout();
    rightLayout->setSpacing(12);

    // 对位计算组
    QGroupBox* calcGroup = new QGroupBox(tr("对位计算"), alignPanel_);
    QGridLayout* calcLayout = new QGridLayout(calcGroup);
    calcLayout->setSpacing(6);

    calcLayout->addWidget(new QLabel(tr("对位点个数:")), 0, 0);
    alignPointCountSpin_ = new QSpinBox(alignPanel_);
    alignPointCountSpin_->setRange(1, 10);
    alignPointCountSpin_->setValue(1);
    calcLayout->addWidget(alignPointCountSpin_, 0, 1);

    calcLayout->addWidget(new QLabel(tr("对象到目标角度:")), 0, 2);
    alignTargetAngleCombo_ = new QComboBox(alignPanel_);
    alignTargetAngleCombo_->addItems({tr("无"), tr("90°"), tr("180°"), tr("270°")});
    calcLayout->addWidget(alignTargetAngleCombo_, 0, 3);

    calcLayout->addWidget(new QLabel(tr("对象移动类型:")), 1, 0);
    alignObjectMoveCombo_ = new QComboBox(alignPanel_);
    alignObjectMoveCombo_->addItems({"XYQ", "XY", "XQ", "YQ", "X", "Y", "Q"});
    calcLayout->addWidget(alignObjectMoveCombo_, 1, 1);

    calcLayout->addWidget(new QLabel(tr("目标移动类型:")), 1, 2);
    alignTargetMoveCombo_ = new QComboBox(alignPanel_);
    alignTargetMoveCombo_->addItems({tr("移动"), tr("固定")});
    calcLayout->addWidget(alignTargetMoveCombo_, 1, 3);

    calcLayout->addWidget(new QLabel(tr("1VS1对位模式:")), 2, 0);
    alignModeCombo_ = new QComboBox(alignPanel_);
    alignModeCombo_->addItem(tr("位置1_定位点_中心对位"));
    alignModeCombo_->addItem(tr("位置1_定位点_边缘对位"));
    calcLayout->addWidget(alignModeCombo_, 2, 1, 1, 3);

    // 位置偏差方向
    QHBoxLayout* offsetLayout = new QHBoxLayout();
    offsetLayout->addWidget(new QLabel(tr("位置偏差方向:")));
    alignOffsetXInvertCheck_ = new QCheckBox(tr("X取反"), alignPanel_);
    alignOffsetYInvertCheck_ = new QCheckBox(tr("Y取反"), alignPanel_);
    alignOffsetQInvertCheck_ = new QCheckBox(tr("Q取反"), alignPanel_);
    offsetLayout->addWidget(alignOffsetXInvertCheck_);
    offsetLayout->addWidget(alignOffsetYInvertCheck_);
    offsetLayout->addWidget(alignOffsetQInvertCheck_);
    offsetLayout->addStretch();
    calcLayout->addLayout(offsetLayout, 3, 0, 1, 4);

    rightLayout->addWidget(calcGroup);

    // 对象补偿组
    QGroupBox* compGroup = new QGroupBox(tr("对象补偿"), alignPanel_);
    QVBoxLayout* compLayout = new QVBoxLayout(compGroup);

    QHBoxLayout* compTypeLayout = new QHBoxLayout();
    compTypeLayout->addWidget(new QLabel(tr("补偿类型:")));
    alignCompTypeCombo_ = new QComboBox(alignPanel_);
    alignCompTypeCombo_->addItems({tr("产品坐标补偿"), tr("图像坐标补偿"), tr("无补偿")});
    compTypeLayout->addWidget(alignCompTypeCombo_);
    compTypeLayout->addStretch();
    compLayout->addLayout(compTypeLayout);

    QGridLayout* compValueLayout = new QGridLayout();
    compValueLayout->addWidget(new QLabel(tr("基础")), 0, 1);
    compValueLayout->addWidget(new QLabel(tr("补偿")), 0, 2);

    alignCompIndexCombo_ = new QComboBox(alignPanel_);
    alignCompIndexCombo_->addItems({"1", "2", "3", "4", "5"});
    compValueLayout->addWidget(alignCompIndexCombo_, 0, 3);

    compValueLayout->addWidget(new QLabel("X:"), 1, 0);
    alignBaseXSpin_ = new QDoubleSpinBox(alignPanel_);
    alignBaseXSpin_->setRange(-9999, 9999);
    alignBaseXSpin_->setDecimals(2);
    alignBaseXSpin_->setSuffix(" mm");
    compValueLayout->addWidget(alignBaseXSpin_, 1, 1);
    alignCompXSpin_ = new QDoubleSpinBox(alignPanel_);
    alignCompXSpin_->setRange(-9999, 9999);
    alignCompXSpin_->setDecimals(2);
    alignCompXSpin_->setSuffix(" mm");
    compValueLayout->addWidget(alignCompXSpin_, 1, 2);

    compValueLayout->addWidget(new QLabel("Y:"), 2, 0);
    alignBaseYSpin_ = new QDoubleSpinBox(alignPanel_);
    alignBaseYSpin_->setRange(-9999, 9999);
    alignBaseYSpin_->setDecimals(2);
    alignBaseYSpin_->setSuffix(" mm");
    compValueLayout->addWidget(alignBaseYSpin_, 2, 1);
    alignCompYSpin_ = new QDoubleSpinBox(alignPanel_);
    alignCompYSpin_->setRange(-9999, 9999);
    alignCompYSpin_->setDecimals(2);
    alignCompYSpin_->setSuffix(" mm");
    compValueLayout->addWidget(alignCompYSpin_, 2, 2);

    compValueLayout->addWidget(new QLabel("Q:"), 3, 0);
    alignBaseQSpin_ = new QDoubleSpinBox(alignPanel_);
    alignBaseQSpin_->setRange(-360, 360);
    alignBaseQSpin_->setDecimals(2);
    alignBaseQSpin_->setSuffix("°");
    compValueLayout->addWidget(alignBaseQSpin_, 3, 1);
    alignCompQSpin_ = new QDoubleSpinBox(alignPanel_);
    alignCompQSpin_->setRange(-360, 360);
    alignCompQSpin_->setDecimals(2);
    alignCompQSpin_->setSuffix("°");
    compValueLayout->addWidget(alignCompQSpin_, 3, 2);

    compLayout->addLayout(compValueLayout);

    // 补偿图示
    QHBoxLayout* compImgLayout = new QHBoxLayout();
    QPushButton* compImgBtn = new QPushButton(tr("补偿图示"), alignPanel_);
    compImgLayout->addWidget(compImgBtn);
    alignCompImageLabel_ = new QLabel(alignPanel_);
    alignCompImageLabel_->setFixedSize(100, 80);
    alignCompImageLabel_->setStyleSheet("border: 1px solid #999; background-color: #1a1a2e;");
    compImgLayout->addWidget(alignCompImageLabel_);
    compImgLayout->addStretch();
    compLayout->addLayout(compImgLayout);

    // 补偿方向
    QHBoxLayout* compDirLayout = new QHBoxLayout();
    compDirLayout->addWidget(new QLabel(tr("补偿方向")));
    alignCompXInvertCheck_ = new QCheckBox(tr("X取反"), alignPanel_);
    alignCompYInvertCheck_ = new QCheckBox(tr("Y取反"), alignPanel_);
    alignCompQInvertCheck_ = new QCheckBox(tr("Q取反"), alignPanel_);
    alignCompXYSwapCheck_ = new QCheckBox(tr("X与Y对调"), alignPanel_);
    compDirLayout->addWidget(alignCompXInvertCheck_);
    compDirLayout->addWidget(alignCompYInvertCheck_);
    compDirLayout->addWidget(alignCompQInvertCheck_);
    compDirLayout->addWidget(alignCompXYSwapCheck_);
    compDirLayout->addStretch();
    compLayout->addLayout(compDirLayout);

    rightLayout->addWidget(compGroup);
    rightLayout->addStretch();

    mainLayout->addLayout(rightLayout);
}

void ProductManageDialog::switchToPanel(int index)
{
    if (contentStack_ && index >= 0 && index < contentStack_->count()) {
        contentStack_->setCurrentIndex(index);
        currentPanelIndex_ = index;
    }
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
    // 切换到场景面板
    switchToPanel(0);

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
    // 切换到标定面板
    switchToPanel(1);

    if (currentProductIndex_ >= 0) {
        emit calibrationRequested(currentProductIndex_);
    }
}

void ProductManageDialog::onAlignmentClicked()
{
    // 切换到对位面板
    switchToPanel(2);

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

void ProductManageDialog::onSceneImageDoubleClicked(int sceneIndex)
{
    currentSceneIndex_ = sceneIndex;

    // 打开工具链编辑器对话框
    ToolChainEditorDialog dialog(this);
    dialog.setWindowTitle(tr("工具链编辑器 - 产品%1 场景%2")
        .arg(currentProductIndex_ + 1)
        .arg(sceneIndex + 1));

    if (dialog.exec() == QDialog::Accepted) {
        // 更新场景配置
        updateProductContent();
    }

    // 发出信号通知外部
    emit toolChainEditRequested(currentProductIndex_, sceneIndex);
}

bool ProductManageDialog::eventFilter(QObject* watched, QEvent* event)
{
    // 处理场景图片的双击事件
    if (event->type() == QEvent::MouseButtonDblClick) {
        for (int i = 0; i < sceneImageLabels_.size(); ++i) {
            if (watched == sceneImageLabels_[i]) {
                onSceneImageDoubleClicked(i);
                return true;
            }
        }
    }
    return QDialog::eventFilter(watched, event);
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
