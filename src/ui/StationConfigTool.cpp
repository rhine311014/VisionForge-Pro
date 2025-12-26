/**
 * @file StationConfigTool.cpp
 * @brief 工位配置工具实现
 */

#include "ui/StationConfigTool.h"
#include "core/MultiStationManager.h"
#include "core/StationConfig.h"
#include "platform/PlatformTypes.h"
#include "base/Logger.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QSplitter>
#include <QScrollArea>
#include <QMessageBox>
#include <QHeaderView>
#include <QApplication>

namespace VisionForge {
namespace UI {

// ============================================================
// 构造与析构
// ============================================================

StationConfigTool::StationConfigTool(QWidget* parent)
    : QDialog(parent)
    , treeWidget_(nullptr)
    , rootItem_(nullptr)
    , settingsGroup_(nullptr)
    , comboPlatformIndex_(nullptr)
    , comboCameraCount_(nullptr)
    , checkSharedCamera_(nullptr)
    , comboSceneCount_(nullptr)
    , comboPositionCount_(nullptr)
    , comboDisplayMode_(nullptr)
    , comboProductCount_(nullptr)
    , comboPreloadCount_(nullptr)
    , comboFunctionMode_(nullptr)
    , detectionGroup_(nullptr)
    , spinDetectionConfidence_(nullptr)
    , spinDetectionNms_(nullptr)
    , alignmentGroup_(nullptr)
    , spinAlignToleranceX_(nullptr)
    , spinAlignToleranceY_(nullptr)
    , spinAlignToleranceD_(nullptr)
    , btnAddPlatform_(nullptr)
    , btnDeletePlatform_(nullptr)
    , btnSaveExit_(nullptr)
    , btnCancelExit_(nullptr)
    , currentPlatformIndex_(-1)
    , hasChanges_(false)
    , saveAndExit_(false)
    , isUpdating_(false)
{
    setWindowTitle(tr("VisionForge 工位配置工具"));
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setMinimumSize(800, 600);
    resize(950, 700);

    setupUI();
    setupConnections();
    loadExistingConfig();

    // 如果没有配置，创建一个默认平台
    if (platforms_.isEmpty()) {
        onAddPlatform();
        hasChanges_ = false;  // 首次创建不算修改
    }

    // 选中第一个平台
    if (!platforms_.isEmpty() && rootItem_ && rootItem_->childCount() > 0) {
        treeWidget_->setCurrentItem(rootItem_->child(0));
        onTreeItemClicked(rootItem_->child(0), 0);
    }
}

StationConfigTool::~StationConfigTool()
{
}

int StationConfigTool::getStationCount() const
{
    return platforms_.size();
}

// ============================================================
// UI 初始化
// ============================================================

void StationConfigTool::setupUI()
{
    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(10);

    // 创建分割器
    QSplitter* splitter = new QSplitter(Qt::Horizontal, this);

    // 左侧树形区域
    QWidget* treeArea = createTreeArea();
    splitter->addWidget(treeArea);

    // 右侧设置区域和按钮
    QWidget* rightArea = new QWidget(this);
    QVBoxLayout* rightLayout = new QVBoxLayout(rightArea);
    rightLayout->setContentsMargins(0, 0, 0, 0);
    rightLayout->setSpacing(10);

    // 创建滚动区域包装设置面板
    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setFrameShape(QFrame::NoFrame);

    // 设置面板
    QGroupBox* settingsArea = createSettingsArea();
    scrollArea->setWidget(settingsArea);
    rightLayout->addWidget(scrollArea, 1);  // 给滚动区域更多空间

    // 按钮区域
    QWidget* buttonArea = createButtonArea();
    rightLayout->addWidget(buttonArea);

    // 底部保存/取消按钮
    QHBoxLayout* bottomLayout = new QHBoxLayout();
    bottomLayout->addStretch();
    btnSaveExit_ = new QPushButton(tr("保存退出"), this);
    btnCancelExit_ = new QPushButton(tr("取消退出"), this);
    btnSaveExit_->setMinimumWidth(80);
    btnCancelExit_->setMinimumWidth(80);
    bottomLayout->addWidget(btnSaveExit_);
    bottomLayout->addWidget(btnCancelExit_);
    rightLayout->addLayout(bottomLayout);

    splitter->addWidget(rightArea);

    // 设置分割比例
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 2);
    splitter->setSizes({300, 500});

    mainLayout->addWidget(splitter);
}

QWidget* StationConfigTool::createTreeArea()
{
    QWidget* widget = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(widget);
    layout->setContentsMargins(0, 0, 0, 0);

    treeWidget_ = new QTreeWidget(widget);
    treeWidget_->setHeaderHidden(true);
    treeWidget_->setIndentation(20);
    treeWidget_->setAnimated(true);

    // 创建根节点
    rootItem_ = new QTreeWidgetItem(treeWidget_);
    rootItem_->setText(0, tr("视觉检测系统"));
    rootItem_->setExpanded(true);
    rootItem_->setFlags(rootItem_->flags() & ~Qt::ItemIsSelectable);

    layout->addWidget(treeWidget_);
    return widget;
}

QGroupBox* StationConfigTool::createSettingsArea()
{
    settingsGroup_ = new QGroupBox(tr("平台设置"), this);
    QGridLayout* grid = new QGridLayout(settingsGroup_);
    grid->setContentsMargins(15, 20, 15, 15);
    grid->setHorizontalSpacing(15);
    grid->setVerticalSpacing(12);

    int row = 0;

    // 平台序
    QLabel* labelPlatformIndex = new QLabel(tr("平台序:"), settingsGroup_);
    comboPlatformIndex_ = new QComboBox(settingsGroup_);
    for (int i = 1; i <= 6; ++i) {
        comboPlatformIndex_->addItem(QString::number(i), i);
    }
    comboPlatformIndex_->setMinimumWidth(120);
    grid->addWidget(labelPlatformIndex, row, 0);
    grid->addWidget(comboPlatformIndex_, row, 1);
    ++row;

    // 相机数
    QLabel* labelCameraCount = new QLabel(tr("相机数:"), settingsGroup_);
    comboCameraCount_ = new QComboBox(settingsGroup_);
    for (int i = 1; i <= 8; ++i) {
        comboCameraCount_->addItem(QString::number(i), i);
    }
    comboCameraCount_->setMinimumWidth(120);
    checkSharedCamera_ = new QCheckBox(tr("相机是否共享系统"), settingsGroup_);
    grid->addWidget(labelCameraCount, row, 0);
    grid->addWidget(comboCameraCount_, row, 1);
    grid->addWidget(checkSharedCamera_, row, 2);
    ++row;

    // 场景数量
    QLabel* labelSceneCount = new QLabel(tr("场景数量:"), settingsGroup_);
    comboSceneCount_ = new QComboBox(settingsGroup_);
    for (int i = 1; i <= 8; ++i) {
        comboSceneCount_->addItem(QString::number(i), i);
    }
    comboSceneCount_->setMinimumWidth(120);
    grid->addWidget(labelSceneCount, row, 0);
    grid->addWidget(comboSceneCount_, row, 1);
    ++row;

    // 位置数量
    QLabel* labelPositionCount = new QLabel(tr("位置数量:"), settingsGroup_);
    comboPositionCount_ = new QComboBox(settingsGroup_);
    for (int i = 1; i <= 8; ++i) {
        comboPositionCount_->addItem(QString::number(i), i);
    }
    comboPositionCount_->setMinimumWidth(120);
    grid->addWidget(labelPositionCount, row, 0);
    grid->addWidget(comboPositionCount_, row, 1);
    ++row;

    // 显示模式
    QLabel* labelDisplayMode = new QLabel(tr("显示模式:"), settingsGroup_);
    comboDisplayMode_ = new QComboBox(settingsGroup_);
    comboDisplayMode_->addItem(tr("按照位置显示"), 0);
    comboDisplayMode_->addItem(tr("按照相机显示"), 1);
    comboDisplayMode_->setMinimumWidth(120);
    grid->addWidget(labelDisplayMode, row, 0);
    grid->addWidget(comboDisplayMode_, row, 1);
    ++row;

    // 产品数量
    QLabel* labelProductCount = new QLabel(tr("产品数量:"), settingsGroup_);
    comboProductCount_ = new QComboBox(settingsGroup_);
    for (int i = 1; i <= 20; ++i) {
        comboProductCount_->addItem(QString::number(i), i);
    }
    comboProductCount_->setMinimumWidth(120);
    grid->addWidget(labelProductCount, row, 0);
    grid->addWidget(comboProductCount_, row, 1);
    ++row;

    // 产品预加载数量
    QLabel* labelPreloadCount = new QLabel(tr("产品预加载数量:"), settingsGroup_);
    comboPreloadCount_ = new QComboBox(settingsGroup_);
    for (int i = 1; i <= 5; ++i) {
        comboPreloadCount_->addItem(QString::number(i), i);
    }
    comboPreloadCount_->setMinimumWidth(120);
    grid->addWidget(labelPreloadCount, row, 0);
    grid->addWidget(comboPreloadCount_, row, 1);
    ++row;

    // ========== 功能模式选择 ==========
    QLabel* labelFunctionMode = new QLabel(tr("功能模式:"), settingsGroup_);
    comboFunctionMode_ = new QComboBox(settingsGroup_);
    comboFunctionMode_->addItem(tr("仅检测"), 0);
    comboFunctionMode_->addItem(tr("仅对位"), 1);
    comboFunctionMode_->addItem(tr("检测+对位"), 2);
    comboFunctionMode_->setCurrentIndex(2);  // 默认选择检测+对位
    comboFunctionMode_->setMinimumWidth(120);
    grid->addWidget(labelFunctionMode, row, 0);
    grid->addWidget(comboFunctionMode_, row, 1);
    ++row;

    // ========== 检测参数组 ==========
    detectionGroup_ = new QGroupBox(tr("检测参数"), settingsGroup_);
    QGridLayout* detectionLayout = new QGridLayout(detectionGroup_);
    detectionLayout->setContentsMargins(10, 15, 10, 10);
    detectionLayout->setHorizontalSpacing(10);
    detectionLayout->setVerticalSpacing(8);

    QLabel* labelConfidence = new QLabel(tr("置信度阈值:"), detectionGroup_);
    spinDetectionConfidence_ = new QDoubleSpinBox(detectionGroup_);
    spinDetectionConfidence_->setRange(0.0, 1.0);
    spinDetectionConfidence_->setSingleStep(0.05);
    spinDetectionConfidence_->setDecimals(2);
    spinDetectionConfidence_->setValue(0.5);
    spinDetectionConfidence_->setMinimumWidth(100);
    detectionLayout->addWidget(labelConfidence, 0, 0);
    detectionLayout->addWidget(spinDetectionConfidence_, 0, 1);

    QLabel* labelNms = new QLabel(tr("NMS阈值:"), detectionGroup_);
    spinDetectionNms_ = new QDoubleSpinBox(detectionGroup_);
    spinDetectionNms_->setRange(0.0, 1.0);
    spinDetectionNms_->setSingleStep(0.05);
    spinDetectionNms_->setDecimals(2);
    spinDetectionNms_->setValue(0.4);
    spinDetectionNms_->setMinimumWidth(100);
    detectionLayout->addWidget(labelNms, 1, 0);
    detectionLayout->addWidget(spinDetectionNms_, 1, 1);

    grid->addWidget(detectionGroup_, row, 0, 1, 3);
    ++row;

    // ========== 对位参数组 ==========
    alignmentGroup_ = new QGroupBox(tr("对位参数"), settingsGroup_);
    QGridLayout* alignmentLayout = new QGridLayout(alignmentGroup_);
    alignmentLayout->setContentsMargins(10, 15, 10, 10);
    alignmentLayout->setHorizontalSpacing(10);
    alignmentLayout->setVerticalSpacing(8);

    QLabel* labelToleranceX = new QLabel(tr("X容差 (mm):"), alignmentGroup_);
    spinAlignToleranceX_ = new QDoubleSpinBox(alignmentGroup_);
    spinAlignToleranceX_->setRange(0.001, 10.0);
    spinAlignToleranceX_->setSingleStep(0.01);
    spinAlignToleranceX_->setDecimals(3);
    spinAlignToleranceX_->setValue(0.1);
    spinAlignToleranceX_->setMinimumWidth(100);
    alignmentLayout->addWidget(labelToleranceX, 0, 0);
    alignmentLayout->addWidget(spinAlignToleranceX_, 0, 1);

    QLabel* labelToleranceY = new QLabel(tr("Y容差 (mm):"), alignmentGroup_);
    spinAlignToleranceY_ = new QDoubleSpinBox(alignmentGroup_);
    spinAlignToleranceY_->setRange(0.001, 10.0);
    spinAlignToleranceY_->setSingleStep(0.01);
    spinAlignToleranceY_->setDecimals(3);
    spinAlignToleranceY_->setValue(0.1);
    spinAlignToleranceY_->setMinimumWidth(100);
    alignmentLayout->addWidget(labelToleranceY, 1, 0);
    alignmentLayout->addWidget(spinAlignToleranceY_, 1, 1);

    QLabel* labelToleranceD = new QLabel(tr("角度容差 (°):"), alignmentGroup_);
    spinAlignToleranceD_ = new QDoubleSpinBox(alignmentGroup_);
    spinAlignToleranceD_->setRange(0.001, 10.0);
    spinAlignToleranceD_->setSingleStep(0.01);
    spinAlignToleranceD_->setDecimals(3);
    spinAlignToleranceD_->setValue(0.1);
    spinAlignToleranceD_->setMinimumWidth(100);
    alignmentLayout->addWidget(labelToleranceD, 2, 0);
    alignmentLayout->addWidget(spinAlignToleranceD_, 2, 1);

    grid->addWidget(alignmentGroup_, row, 0, 1, 3);
    ++row;

    // 添加弹性空间
    grid->setRowStretch(row, 1);

    return settingsGroup_;
}

QWidget* StationConfigTool::createButtonArea()
{
    QWidget* widget = new QWidget(this);
    QHBoxLayout* layout = new QHBoxLayout(widget);
    layout->setContentsMargins(0, 0, 0, 0);

    btnAddPlatform_ = new QPushButton(tr("新增平台"), widget);
    btnDeletePlatform_ = new QPushButton(tr("删除平台"), widget);

    btnAddPlatform_->setMinimumWidth(80);
    btnDeletePlatform_->setMinimumWidth(80);

    layout->addWidget(btnAddPlatform_);
    layout->addWidget(btnDeletePlatform_);
    layout->addStretch();

    return widget;
}

void StationConfigTool::setupConnections()
{
    // 树形控件
    connect(treeWidget_, &QTreeWidget::itemClicked,
            this, &StationConfigTool::onTreeItemClicked);

    // 参数变更
    connect(comboPlatformIndex_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &StationConfigTool::onPlatformIndexChanged);
    connect(comboCameraCount_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &StationConfigTool::onCameraCountChanged);
    connect(checkSharedCamera_, &QCheckBox::stateChanged,
            this, &StationConfigTool::onSharedCameraChanged);
    connect(comboSceneCount_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &StationConfigTool::onSceneCountChanged);
    connect(comboPositionCount_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &StationConfigTool::onPositionCountChanged);
    connect(comboDisplayMode_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &StationConfigTool::onDisplayModeChanged);
    connect(comboProductCount_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &StationConfigTool::onProductCountChanged);
    connect(comboPreloadCount_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &StationConfigTool::onPreloadCountChanged);
    connect(comboFunctionMode_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &StationConfigTool::onFunctionModeChanged);

    // 检测参数变更
    connect(spinDetectionConfidence_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [this](double) { markAsChanged(); });
    connect(spinDetectionNms_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [this](double) { markAsChanged(); });

    // 对位参数变更
    connect(spinAlignToleranceX_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [this](double) { markAsChanged(); });
    connect(spinAlignToleranceY_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [this](double) { markAsChanged(); });
    connect(spinAlignToleranceD_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [this](double) { markAsChanged(); });

    // 按钮
    connect(btnAddPlatform_, &QPushButton::clicked,
            this, &StationConfigTool::onAddPlatform);
    connect(btnDeletePlatform_, &QPushButton::clicked,
            this, &StationConfigTool::onDeletePlatform);
    connect(btnSaveExit_, &QPushButton::clicked,
            this, &StationConfigTool::onSaveAndExit);
    connect(btnCancelExit_, &QPushButton::clicked,
            this, &StationConfigTool::onCancelExit);
}

// ============================================================
// 配置加载与保存
// ============================================================

void StationConfigTool::loadExistingConfig()
{
    auto& manager = Core::MultiStationManager::instance();

    // 尝试加载现有配置
    manager.loadConfig();

    // 转换为内部格式
    auto stations = manager.getAllStations();
    platforms_.clear();

    for (const auto* station : stations) {
        PlatformConfig config;
        config.index = station->index + 1;  // 内部0-based，显示1-based
        config.cameraCount = station->cameraNum;
        config.sharedCamera = station->shareCameraSystem;
        config.sceneCount = 2;  // 默认值
        config.positionCount = station->positionNum;
        config.displayMode = station->displayMode;
        config.productCount = 5;  // 默认值
        config.preloadCount = 1;  // 默认值
        config.functionMode = static_cast<int>(station->functionMode);

        // 从第一个位置绑定获取检测/对位参数（如有）
        if (!station->positionBindings.isEmpty()) {
            const auto& binding = station->positionBindings.first();
            config.detectionConfidence = binding.detectionConfidenceThreshold;
            config.detectionNms = binding.detectionNmsThreshold;
            config.alignmentToleranceX = binding.alignmentToleranceX;
            config.alignmentToleranceY = binding.alignmentToleranceY;
            config.alignmentToleranceD = binding.alignmentToleranceD;
        }

        platforms_.append(config);
    }

    refreshTree();
}

bool StationConfigTool::saveConfig()
{
    auto& manager = Core::MultiStationManager::instance();

    // 清空现有配置
    manager.clearAll();

    // 创建新配置
    for (int i = 0; i < platforms_.size(); ++i) {
        const auto& config = platforms_[i];

        QString stationId = manager.createStation(
            QString("平台%1").arg(config.index),
            Platform::PlatformType::X1X2Y
        );

        if (!stationId.isEmpty()) {
            auto* station = manager.getStation(stationId);
            if (station) {
                station->index = config.index - 1;  // 显示1-based，内部0-based
                station->cameraNum = config.cameraCount;
                station->shareCameraSystem = config.sharedCamera;
                station->positionNum = config.positionCount;
                station->displayMode = static_cast<Core::StationConfig::DisplayMode>(config.displayMode);
                station->functionMode = static_cast<Core::StationConfig::FunctionMode>(config.functionMode);

                // 生成相机ID
                station->cameraIds.clear();
                for (int c = 0; c < config.cameraCount; ++c) {
                    station->cameraIds.append(QString("cam_%1_%2").arg(stationId).arg(c));
                }

                // 根据功能模式设置启用状态
                bool enableDetection = (config.functionMode == 0 || config.functionMode == 2);
                bool enableAlignment = (config.functionMode == 1 || config.functionMode == 2);

                // 生成位置绑定
                station->positionBindings.clear();
                for (int p = 0; p < config.positionCount; ++p) {
                    Core::PositionCameraBinding binding;
                    binding.positionId = QString("pos%1").arg(p);
                    binding.positionName = QString("Pos%1").arg(p + 1);
                    binding.cameraIndex = p % config.cameraCount;
                    binding.displayIndex = p;
                    binding.captureOrder = p;
                    binding.positionType = (p < config.positionCount / 2)
                                           ? Core::PositionCameraBinding::Target
                                           : Core::PositionCameraBinding::Object;

                    // 设置检测参数
                    binding.enableDetection = enableDetection;
                    binding.detectionConfidenceThreshold = config.detectionConfidence;
                    binding.detectionNmsThreshold = config.detectionNms;

                    // 设置对位参数
                    binding.enableAlignment = enableAlignment;
                    binding.alignmentToleranceX = config.alignmentToleranceX;
                    binding.alignmentToleranceY = config.alignmentToleranceY;
                    binding.alignmentToleranceD = config.alignmentToleranceD;

                    station->positionBindings.append(binding);
                }

                // 生成场景配置
                station->scenes.clear();
                station->sceneNum = config.sceneCount;
                for (int s = 0; s < config.sceneCount; ++s) {
                    Core::SceneConfig scene;
                    scene.sceneId = QString("scene%1").arg(s);
                    scene.sceneName = QString("场景%1").arg(s + 1);
                    scene.sceneIndex = s;
                    scene.enabled = true;
                    station->scenes.append(scene);
                }
                station->currentSceneIndex = 0;
            }
        }
    }

    // 保存到文件
    if (manager.saveConfig()) {
        LOG_INFO(QString("工位配置已保存: %1 个平台").arg(platforms_.size()));
        return true;
    }

    LOG_ERROR("保存工位配置失败");
    return false;
}

// ============================================================
// 树形控件操作
// ============================================================

void StationConfigTool::refreshTree()
{
    if (!rootItem_) return;

    // 清空子节点
    while (rootItem_->childCount() > 0) {
        delete rootItem_->takeChild(0);
    }

    // 添加平台节点
    for (int i = 0; i < platforms_.size(); ++i) {
        QTreeWidgetItem* platformItem = new QTreeWidgetItem(rootItem_);
        platformItem->setData(0, Qt::UserRole, i);  // 存储索引
        updateTreeItem(platformItem, i);
    }

    rootItem_->setExpanded(true);
}

void StationConfigTool::updateTreeItem(QTreeWidgetItem* item, int platformIndex)
{
    if (!item || platformIndex < 0 || platformIndex >= platforms_.size()) return;

    const auto& config = platforms_[platformIndex];

    // 设置平台名称
    item->setText(0, QString("平台%1").arg(config.index));

    // 清空子节点
    while (item->childCount() > 0) {
        delete item->takeChild(0);
    }

    // 添加详细信息子节点
    auto addInfoItem = [item](const QString& text) {
        QTreeWidgetItem* child = new QTreeWidgetItem(item);
        child->setText(0, text);
        child->setFlags(child->flags() & ~Qt::ItemIsSelectable);
    };

    addInfoItem(QString("相机数量: %1").arg(config.cameraCount));
    addInfoItem(QString("场景数量: %1").arg(config.sceneCount));
    addInfoItem(QString("位置数量: %1").arg(config.positionCount));
    addInfoItem(QString("视窗显示模式: %1").arg(getDisplayModeText(config.displayMode)));
    addInfoItem(QString("功能模式: %1").arg(getFunctionModeText(config.functionMode)));
    addInfoItem(QString("产品数量: %1").arg(config.productCount));
    addInfoItem(QString("产品预加载数量: %1").arg(config.preloadCount));
    addInfoItem(QString("是否为共享相机: %1").arg(config.sharedCamera ? "是" : "否"));

    item->setExpanded(true);
}

void StationConfigTool::onTreeItemClicked(QTreeWidgetItem* item, int column)
{
    Q_UNUSED(column);

    if (!item || item == rootItem_) {
        setSettingsPanelEnabled(false);
        currentPlatformIndex_ = -1;
        return;
    }

    // 检查是否是平台节点（有UserRole数据）
    QVariant data = item->data(0, Qt::UserRole);
    if (!data.isValid()) {
        // 可能是子节点，获取父节点
        QTreeWidgetItem* parent = item->parent();
        if (parent && parent != rootItem_) {
            data = parent->data(0, Qt::UserRole);
        }
    }

    if (data.isValid()) {
        int index = data.toInt();
        if (index >= 0 && index < platforms_.size()) {
            currentPlatformIndex_ = index;
            updateSettingsPanel(index);
            setSettingsPanelEnabled(true);
        }
    }
}

void StationConfigTool::onTreeItemChanged(QTreeWidgetItem* item, int column)
{
    Q_UNUSED(item);
    Q_UNUSED(column);
}

// ============================================================
// 设置面板操作
// ============================================================

void StationConfigTool::updateSettingsPanel(int platformIndex)
{
    if (platformIndex < 0 || platformIndex >= platforms_.size()) return;

    isUpdating_ = true;

    const auto& config = platforms_[platformIndex];

    comboPlatformIndex_->setCurrentIndex(config.index - 1);
    comboCameraCount_->setCurrentIndex(config.cameraCount - 1);
    checkSharedCamera_->setChecked(config.sharedCamera);
    comboSceneCount_->setCurrentIndex(config.sceneCount - 1);
    comboPositionCount_->setCurrentIndex(config.positionCount - 1);
    comboDisplayMode_->setCurrentIndex(config.displayMode);
    comboProductCount_->setCurrentIndex(config.productCount - 1);
    comboPreloadCount_->setCurrentIndex(config.preloadCount - 1);
    comboFunctionMode_->setCurrentIndex(config.functionMode);

    // 检测参数
    spinDetectionConfidence_->setValue(config.detectionConfidence);
    spinDetectionNms_->setValue(config.detectionNms);

    // 对位参数
    spinAlignToleranceX_->setValue(config.alignmentToleranceX);
    spinAlignToleranceY_->setValue(config.alignmentToleranceY);
    spinAlignToleranceD_->setValue(config.alignmentToleranceD);

    // 根据功能模式更新UI可见性
    updateFunctionModeUI(config.functionMode);

    isUpdating_ = false;
}

void StationConfigTool::setSettingsPanelEnabled(bool enabled)
{
    comboPlatformIndex_->setEnabled(enabled);
    comboCameraCount_->setEnabled(enabled);
    checkSharedCamera_->setEnabled(enabled);
    comboSceneCount_->setEnabled(enabled);
    comboPositionCount_->setEnabled(enabled);
    comboDisplayMode_->setEnabled(enabled);
    comboProductCount_->setEnabled(enabled);
    comboPreloadCount_->setEnabled(enabled);
    comboFunctionMode_->setEnabled(enabled);
    detectionGroup_->setEnabled(enabled);
    alignmentGroup_->setEnabled(enabled);
    btnDeletePlatform_->setEnabled(enabled && platforms_.size() > 1);

    // 如果启用，根据功能模式更新参数组可见性
    if (enabled && currentPlatformIndex_ >= 0 && currentPlatformIndex_ < platforms_.size()) {
        updateFunctionModeUI(platforms_[currentPlatformIndex_].functionMode);
    }
}

void StationConfigTool::markAsChanged()
{
    if (!isUpdating_) {
        hasChanges_ = true;
    }
}

QString StationConfigTool::getDisplayModeText(int mode) const
{
    switch (mode) {
    case 0: return tr("按位置数显示");
    case 1: return tr("按相机数显示");
    default: return tr("未知");
    }
}

QString StationConfigTool::getFunctionModeText(int mode) const
{
    switch (mode) {
    case 0: return tr("仅检测");
    case 1: return tr("仅对位");
    case 2: return tr("检测+对位");
    default: return tr("未知");
    }
}

void StationConfigTool::updateFunctionModeUI(int mode)
{
    // 根据功能模式显示/隐藏参数组
    // mode: 0=仅检测, 1=仅对位, 2=检测+对位
    bool showDetection = (mode == 0 || mode == 2);
    bool showAlignment = (mode == 1 || mode == 2);

    detectionGroup_->setVisible(showDetection);
    alignmentGroup_->setVisible(showAlignment);
}

// ============================================================
// 参数变更槽函数
// ============================================================

void StationConfigTool::onPlatformIndexChanged(int index)
{
    if (isUpdating_ || currentPlatformIndex_ < 0) return;

    int newIndex = index + 1;

    // 检查是否与其他平台冲突
    for (int i = 0; i < platforms_.size(); ++i) {
        if (i != currentPlatformIndex_ && platforms_[i].index == newIndex) {
            QMessageBox::warning(this, tr("警告"),
                tr("平台序 %1 已被使用，请选择其他序号").arg(newIndex));
            isUpdating_ = true;
            comboPlatformIndex_->setCurrentIndex(platforms_[currentPlatformIndex_].index - 1);
            isUpdating_ = false;
            return;
        }
    }

    platforms_[currentPlatformIndex_].index = newIndex;
    markAsChanged();

    // 更新树形显示
    if (rootItem_ && currentPlatformIndex_ < rootItem_->childCount()) {
        updateTreeItem(rootItem_->child(currentPlatformIndex_), currentPlatformIndex_);
    }
}

void StationConfigTool::onCameraCountChanged(int value)
{
    if (isUpdating_ || currentPlatformIndex_ < 0) return;

    platforms_[currentPlatformIndex_].cameraCount = value + 1;
    markAsChanged();

    if (rootItem_ && currentPlatformIndex_ < rootItem_->childCount()) {
        updateTreeItem(rootItem_->child(currentPlatformIndex_), currentPlatformIndex_);
    }
}

void StationConfigTool::onSharedCameraChanged(int state)
{
    if (isUpdating_ || currentPlatformIndex_ < 0) return;

    platforms_[currentPlatformIndex_].sharedCamera = (state == Qt::Checked);
    markAsChanged();

    if (rootItem_ && currentPlatformIndex_ < rootItem_->childCount()) {
        updateTreeItem(rootItem_->child(currentPlatformIndex_), currentPlatformIndex_);
    }
}

void StationConfigTool::onSceneCountChanged(int value)
{
    if (isUpdating_ || currentPlatformIndex_ < 0) return;

    platforms_[currentPlatformIndex_].sceneCount = value + 1;
    markAsChanged();

    if (rootItem_ && currentPlatformIndex_ < rootItem_->childCount()) {
        updateTreeItem(rootItem_->child(currentPlatformIndex_), currentPlatformIndex_);
    }
}

void StationConfigTool::onPositionCountChanged(int value)
{
    if (isUpdating_ || currentPlatformIndex_ < 0) return;

    platforms_[currentPlatformIndex_].positionCount = value + 1;
    markAsChanged();

    if (rootItem_ && currentPlatformIndex_ < rootItem_->childCount()) {
        updateTreeItem(rootItem_->child(currentPlatformIndex_), currentPlatformIndex_);
    }
}

void StationConfigTool::onDisplayModeChanged(int index)
{
    if (isUpdating_ || currentPlatformIndex_ < 0) return;

    platforms_[currentPlatformIndex_].displayMode = index;
    markAsChanged();

    if (rootItem_ && currentPlatformIndex_ < rootItem_->childCount()) {
        updateTreeItem(rootItem_->child(currentPlatformIndex_), currentPlatformIndex_);
    }
}

void StationConfigTool::onProductCountChanged(int value)
{
    if (isUpdating_ || currentPlatformIndex_ < 0) return;

    platforms_[currentPlatformIndex_].productCount = value + 1;
    markAsChanged();

    if (rootItem_ && currentPlatformIndex_ < rootItem_->childCount()) {
        updateTreeItem(rootItem_->child(currentPlatformIndex_), currentPlatformIndex_);
    }
}

void StationConfigTool::onPreloadCountChanged(int value)
{
    if (isUpdating_ || currentPlatformIndex_ < 0) return;

    platforms_[currentPlatformIndex_].preloadCount = value + 1;
    markAsChanged();

    if (rootItem_ && currentPlatformIndex_ < rootItem_->childCount()) {
        updateTreeItem(rootItem_->child(currentPlatformIndex_), currentPlatformIndex_);
    }
}

void StationConfigTool::onFunctionModeChanged(int index)
{
    if (isUpdating_ || currentPlatformIndex_ < 0) return;

    platforms_[currentPlatformIndex_].functionMode = index;

    // 同步更新当前spinbox的值到platforms数据
    platforms_[currentPlatformIndex_].detectionConfidence = static_cast<float>(spinDetectionConfidence_->value());
    platforms_[currentPlatformIndex_].detectionNms = static_cast<float>(spinDetectionNms_->value());
    platforms_[currentPlatformIndex_].alignmentToleranceX = spinAlignToleranceX_->value();
    platforms_[currentPlatformIndex_].alignmentToleranceY = spinAlignToleranceY_->value();
    platforms_[currentPlatformIndex_].alignmentToleranceD = spinAlignToleranceD_->value();

    markAsChanged();

    // 更新参数组可见性
    updateFunctionModeUI(index);

    if (rootItem_ && currentPlatformIndex_ < rootItem_->childCount()) {
        updateTreeItem(rootItem_->child(currentPlatformIndex_), currentPlatformIndex_);
    }
}

// ============================================================
// 按钮操作
// ============================================================

void StationConfigTool::onAddPlatform()
{
    if (platforms_.size() >= 6) {
        QMessageBox::warning(this, tr("警告"), tr("最多只能添加6个平台"));
        return;
    }

    // 查找可用的平台序号
    int newIndex = 1;
    QSet<int> usedIndices;
    for (const auto& p : platforms_) {
        usedIndices.insert(p.index);
    }
    while (usedIndices.contains(newIndex) && newIndex <= 6) {
        ++newIndex;
    }

    if (newIndex > 6) {
        QMessageBox::warning(this, tr("警告"), tr("没有可用的平台序号"));
        return;
    }

    // 创建新平台配置
    PlatformConfig config;
    config.index = newIndex;
    config.cameraCount = 1;
    config.sharedCamera = false;
    config.sceneCount = 2;
    config.positionCount = 4;
    config.displayMode = 0;
    config.productCount = 5;
    config.preloadCount = 1;

    platforms_.append(config);
    markAsChanged();

    // 刷新树形显示
    refreshTree();

    // 选中新添加的平台
    if (rootItem_ && rootItem_->childCount() > 0) {
        QTreeWidgetItem* newItem = rootItem_->child(rootItem_->childCount() - 1);
        treeWidget_->setCurrentItem(newItem);
        onTreeItemClicked(newItem, 0);
    }

    LOG_INFO(QString("添加平台%1").arg(newIndex));
}

void StationConfigTool::onDeletePlatform()
{
    if (currentPlatformIndex_ < 0 || currentPlatformIndex_ >= platforms_.size()) {
        return;
    }

    if (platforms_.size() <= 1) {
        QMessageBox::warning(this, tr("警告"), tr("至少需要保留一个平台"));
        return;
    }

    int platformIndex = platforms_[currentPlatformIndex_].index;

    int ret = QMessageBox::question(this, tr("确认删除"),
        tr("确定要删除平台%1吗？").arg(platformIndex),
        QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

    if (ret != QMessageBox::Yes) {
        return;
    }

    platforms_.removeAt(currentPlatformIndex_);
    markAsChanged();

    // 刷新树形显示
    refreshTree();

    // 选中第一个平台
    currentPlatformIndex_ = -1;
    if (!platforms_.isEmpty() && rootItem_ && rootItem_->childCount() > 0) {
        treeWidget_->setCurrentItem(rootItem_->child(0));
        onTreeItemClicked(rootItem_->child(0), 0);
    } else {
        setSettingsPanelEnabled(false);
    }

    LOG_INFO(QString("删除平台%1").arg(platformIndex));
}

void StationConfigTool::onSaveAndExit()
{
    if (saveConfig()) {
        saveAndExit_ = true;
        accept();
    } else {
        QMessageBox::critical(this, tr("错误"), tr("保存配置失败"));
    }
}

void StationConfigTool::onCancelExit()
{
    if (hasChanges_) {
        int ret = QMessageBox::question(this, tr("确认"),
            tr("配置已修改，确定要放弃更改吗？"),
            QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

        if (ret != QMessageBox::Yes) {
            return;
        }
    }

    saveAndExit_ = false;
    reject();
}

} // namespace UI
} // namespace VisionForge
