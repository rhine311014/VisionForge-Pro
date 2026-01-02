/**
 * @file OptionsDialog.cpp
 * @brief 选项设置对话框实现文件
 * @details 实现选项设置对话框的所有功能，包括：
 *          - UI界面的构建和布局
 *          - 基础设置页面（语言、图像显示、启动设置、工位名称）
 *          - 光源控制页面（多控制器配置、通道亮度控制）
 *          - 工位布局页面（布局类型选择和预览）
 *          - 配置的加载和保存
 */

#include "ui/OptionsDialog.h"
#include "comm/LightControllerManager.h"
#include "base/Logger.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QGridLayout>
#include <QFormLayout>
#include <QTabWidget>
#include <QSlider>
#include <QMessageBox>
#include <QSerialPortInfo>
#include <QCoreApplication>

namespace VisionForge {
namespace UI {

// ============================================================================
// 构造函数和UI初始化
// ============================================================================

/**
 * @brief 构造函数 - 初始化对话框
 * @param parent 父窗口指针
 * @details 设置窗口属性，构建UI，加载设置
 */
OptionsDialog::OptionsDialog(QWidget* parent)
    : QDialog(parent)
{
    // 设置窗口标题
    setWindowTitle("选项设置");

    // 设置最小窗口大小
    setMinimumSize(850, 650);

    // 移除窗口标题栏的问号帮助按钮
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    // 构建UI界面
    setupUI();

    // 加载已保存的设置
    loadSettings();
}

/**
 * @brief 构建整体UI布局
 * @details 创建垂直主布局，依次添加工具栏、页面区域、底部按钮
 */
void OptionsDialog::setupUI()
{
    // 创建主布局
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(10, 10, 10, 10);

    // ===== 顶部工具栏 =====
    setupToolBar();
    mainLayout->addWidget(toolBar_);

    // ===== 页面切换区域 =====
    // 使用QStackedWidget实现多页面切换
    stackedWidget_ = new QStackedWidget(this);

    // 创建各设置页面
    setupBasicPage();   // 基础设置页
    setupLightPage();   // 光源控制页
    setupLayoutPage();  // 工位布局页

    // 将页面添加到堆叠容器（索引0、1、2）
    stackedWidget_->addWidget(basicPage_);
    stackedWidget_->addWidget(lightPage_);
    stackedWidget_->addWidget(layoutPage_);

    // 页面区域占据主要空间（stretch=1）
    mainLayout->addWidget(stackedWidget_, 1);

    // ===== 底部按钮区域 =====
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();  // 左侧弹性空间

    // 确定按钮
    okButton_ = new QPushButton("确定", this);
    okButton_->setFixedSize(80, 30);
    connect(okButton_, &QPushButton::clicked, this, &OptionsDialog::onOkClicked);
    buttonLayout->addWidget(okButton_);

    // 取消按钮
    cancelButton_ = new QPushButton("取消", this);
    cancelButton_->setFixedSize(80, 30);
    connect(cancelButton_, &QPushButton::clicked, this, &OptionsDialog::onCancelClicked);
    buttonLayout->addWidget(cancelButton_);

    buttonLayout->addStretch();  // 右侧弹性空间

    mainLayout->addLayout(buttonLayout);

    // 默认选中第一个按钮（基础设置）
    btnBasic_->setChecked(true);
    onToolButtonClicked(0);
}

/**
 * @brief 构建顶部工具栏
 * @details 创建导航按钮组，设置按钮样式，连接信号
 */
void OptionsDialog::setupToolBar()
{
    // 创建工具栏容器
    toolBar_ = new QWidget(this);
    toolBar_->setFixedHeight(50);

    QHBoxLayout* toolLayout = new QHBoxLayout(toolBar_);
    toolLayout->setSpacing(5);
    toolLayout->setContentsMargins(5, 5, 5, 5);

    // ===== 定义按钮样式表 =====

    // 普通状态样式：灰色背景
    toolBtnNormalStyle_ = R"(
        QPushButton {
            background-color: #e0e0e0;
            color: #333333;
            border: 1px solid #cccccc;
            border-radius: 4px;
            padding: 8px 20px;
            font-size: 13px;
            font-weight: bold;
            min-width: 100px;
        }
        QPushButton:hover {
            background-color: #d0d0d0;
        }
        QPushButton:pressed {
            background-color: #c0c0c0;
        }
    )";

    // 选中状态样式：蓝色背景
    toolBtnSelectedStyle_ = R"(
        QPushButton {
            background-color: #4a90d9;
            color: white;
            border: 1px solid #3a80c9;
            border-radius: 4px;
            padding: 8px 20px;
            font-size: 13px;
            font-weight: bold;
            min-width: 100px;
        }
        QPushButton:hover {
            background-color: #5aa0e9;
        }
    )";

    // ===== 创建按钮组（互斥选择） =====
    toolButtonGroup_ = new QButtonGroup(this);
    toolButtonGroup_->setExclusive(true);

    // 基础设置按钮
    btnBasic_ = new QPushButton("基础设置", this);
    btnBasic_->setCheckable(true);
    btnBasic_->setStyleSheet(toolBtnNormalStyle_);
    toolButtonGroup_->addButton(btnBasic_, 0);  // ID=0
    toolLayout->addWidget(btnBasic_);

    // 光源控制按钮
    btnLight_ = new QPushButton("光源控制", this);
    btnLight_->setCheckable(true);
    btnLight_->setStyleSheet(toolBtnNormalStyle_);
    toolButtonGroup_->addButton(btnLight_, 1);  // ID=1
    toolLayout->addWidget(btnLight_);

    // 工位布局按钮
    btnLayout_ = new QPushButton("工位布局", this);
    btnLayout_->setCheckable(true);
    btnLayout_->setStyleSheet(toolBtnNormalStyle_);
    toolButtonGroup_->addButton(btnLayout_, 2);  // ID=2
    toolLayout->addWidget(btnLayout_);

    // 右侧弹性空间
    toolLayout->addStretch();

    // 连接按钮组点击信号到槽函数
    connect(toolButtonGroup_, QOverload<int>::of(&QButtonGroup::idClicked),
            this, &OptionsDialog::onToolButtonClicked);
}

// ============================================================================
// 基础设置页面构建
// ============================================================================

/**
 * @brief 构建基础设置页面
 * @details 创建语言设置、图像显示、启动设置、工位名称四个配置区域
 */
void OptionsDialog::setupBasicPage()
{
    basicPage_ = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(basicPage_);
    layout->setSpacing(15);
    layout->setContentsMargins(10, 10, 10, 10);

    // ===== 语言设置分组 =====
    languageGroup_ = new QGroupBox("语言设置", basicPage_);
    QHBoxLayout* langLayout = new QHBoxLayout(languageGroup_);

    // 简体中文选项（默认选中）
    radioChineseSimplified_ = new QRadioButton("简体中文", languageGroup_);
    radioChineseSimplified_->setChecked(true);
    langLayout->addWidget(radioChineseSimplified_);

    // 英文选项
    radioEnglish_ = new QRadioButton("English", languageGroup_);
    langLayout->addWidget(radioEnglish_);

    langLayout->addStretch();

    layout->addWidget(languageGroup_);

    // ===== 图像显示设置分组 =====
    imageDisplayGroup_ = new QGroupBox("图像显示", basicPage_);
    QGridLayout* imageLayout = new QGridLayout(imageDisplayGroup_);
    imageLayout->setSpacing(10);

    // 抽帧数量设置
    QLabel* frameSkipLabel = new QLabel("抽帧数量:", imageDisplayGroup_);
    imageLayout->addWidget(frameSkipLabel, 0, 0);

    frameSkipSpin_ = new QSpinBox(imageDisplayGroup_);
    frameSkipSpin_->setRange(1, 100);   // 范围1-100
    frameSkipSpin_->setValue(1);        // 默认值1（不抽帧）
    frameSkipSpin_->setFixedWidth(80);
    imageLayout->addWidget(frameSkipSpin_, 0, 1);

    // 显示分辨率设置
    QLabel* displayResLabel = new QLabel("显示图像:", imageDisplayGroup_);
    imageLayout->addWidget(displayResLabel, 1, 0);

    displayResolutionCombo_ = new QComboBox(imageDisplayGroup_);
    displayResolutionCombo_->addItems({"原始大小", "1/2", "1/4", "1/8", "自适应"});
    displayResolutionCombo_->setCurrentIndex(4);  // 默认"自适应"
    displayResolutionCombo_->setFixedWidth(120);
    imageLayout->addWidget(displayResolutionCombo_, 1, 1);

    // 第三列占据剩余空间
    imageLayout->setColumnStretch(2, 1);

    layout->addWidget(imageDisplayGroup_);

    // ===== 启动设置分组 =====
    startupGroup_ = new QGroupBox("启动设置", basicPage_);
    QVBoxLayout* startupLayout = new QVBoxLayout(startupGroup_);

    // 相机通道设置选项
    chkCameraChannel_ = new QCheckBox("相机通道设置", startupGroup_);
    startupLayout->addWidget(chkCameraChannel_);

    // 虚拟屏幕键盘选项
    chkVirtualKeyboard_ = new QCheckBox("虚拟屏幕键盘", startupGroup_);
    startupLayout->addWidget(chkVirtualKeyboard_);

    // 通信指令显示选项
    chkCommDisplay_ = new QCheckBox("通信指令显示", startupGroup_);
    startupLayout->addWidget(chkCommDisplay_);

    layout->addWidget(startupGroup_);

    // ===== 工位名称设置分组 =====
    stationNameGroup_ = new QGroupBox("工位名称设置", basicPage_);
    QVBoxLayout* stationLayout = new QVBoxLayout(stationNameGroup_);

    // 创建工位名称表格（8行2列）
    stationNameTable_ = new QTableWidget(stationNameGroup_);
    stationNameTable_->setColumnCount(2);
    stationNameTable_->setHorizontalHeaderLabels({"NO.", "自定义名称"});

    // 设置表格样式
    stationNameTable_->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);      // 序号列固定宽度
    stationNameTable_->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);   // 名称列自动拉伸
    stationNameTable_->setColumnWidth(0, 60);
    stationNameTable_->verticalHeader()->setVisible(false);                                  // 隐藏行号
    stationNameTable_->setSelectionBehavior(QAbstractItemView::SelectRows);                 // 整行选择

    // 添加8行默认数据
    stationNameTable_->setRowCount(8);
    for (int i = 0; i < 8; ++i) {
        // 序号列（只读）
        QTableWidgetItem* noItem = new QTableWidgetItem(QString::number(i + 1));
        noItem->setTextAlignment(Qt::AlignCenter);
        noItem->setFlags(noItem->flags() & ~Qt::ItemIsEditable);  // 禁止编辑
        stationNameTable_->setItem(i, 0, noItem);

        // 名称列（可编辑）
        QTableWidgetItem* nameItem = new QTableWidgetItem(QString("工位%1").arg(i + 1));
        stationNameTable_->setItem(i, 1, nameItem);

        // 设置行高
        stationNameTable_->setRowHeight(i, 30);
    }

    stationLayout->addWidget(stationNameTable_);

    // 工位名称分组占据剩余垂直空间
    layout->addWidget(stationNameGroup_, 1);
}

// ============================================================================
// 光源控制页面构建
// ============================================================================

/**
 * @brief 构建光源控制页面
 * @details 创建TabWidget容器，为4个控制器各创建一个标签页
 */
void OptionsDialog::setupLightPage()
{
    lightPage_ = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(lightPage_);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(10, 10, 10, 10);

    // ===== 创建光源控制器标签页容器 =====
    lightControllerTabs_ = new QTabWidget(lightPage_);

    // 为每个控制器创建独立的标签页（共4个）
    for (int i = 0; i < 4; ++i) {
        QWidget* controllerPage = new QWidget(this);
        setupLightControllerPage(i, controllerPage);  // 构建页面内容
        lightControllerTabs_->addTab(controllerPage, QString("光源控制器%1").arg(i + 1));
    }

    // 连接标签页切换信号
    connect(lightControllerTabs_, &QTabWidget::currentChanged,
            this, &OptionsDialog::onLightControllerTabChanged);

    mainLayout->addWidget(lightControllerTabs_);

    // 初始化第一个控制器的UI显示
    syncLightConfigToUI(0);
}

/**
 * @brief 构建单个光源控制器的配置页面
 * @param index 控制器索引（0-3）
 * @param page 页面容器Widget
 * @details 包含两个区域：连接设置区域和通道亮度控制区域
 */
void OptionsDialog::setupLightControllerPage(int index, QWidget* page)
{
    // 获取当前控制器的控件集合引用
    LightControllerPageWidgets& widgets = lightPageWidgets_[index];

    QVBoxLayout* controllerLayout = new QVBoxLayout(page);
    controllerLayout->setSpacing(15);

    // ===== 光源连接设置分组 =====
    QGroupBox* connectionGroup = new QGroupBox("光源连接设置", page);
    QGridLayout* connLayout = new QGridLayout(connectionGroup);
    connLayout->setSpacing(10);

    // ----- 第一行：启用、控制器类型、通信类型、打开按钮 -----

    // 启用复选框
    widgets.chkEnable = new QCheckBox("启用", connectionGroup);
    connLayout->addWidget(widgets.chkEnable, 0, 0);

    // 控制器类型标签和下拉框
    connLayout->addWidget(new QLabel("控制器类型:", connectionGroup), 0, 1);
    widgets.typeCombo = new QComboBox(connectionGroup);
    // 添加支持的控制器类型（显示文本和对应的枚举值）
    widgets.typeCombo->addItem("康视达", static_cast<int>(Comm::LightControllerType::KangShiDa));
    widgets.typeCombo->addItem("锐视", static_cast<int>(Comm::LightControllerType::RuiShi));
    widgets.typeCombo->addItem("奥普特", static_cast<int>(Comm::LightControllerType::OPT));
    widgets.typeCombo->addItem("CCS", static_cast<int>(Comm::LightControllerType::CCS));
    widgets.typeCombo->addItem("力科胜", static_cast<int>(Comm::LightControllerType::LKS));
    widgets.typeCombo->setFixedWidth(100);
    connLayout->addWidget(widgets.typeCombo, 0, 2);

    // 通信类型标签和下拉框
    connLayout->addWidget(new QLabel("通信类型:", connectionGroup), 0, 3);
    widgets.commTypeCombo = new QComboBox(connectionGroup);
    widgets.commTypeCombo->addItem("串口", 0);  // 串口通信
    widgets.commTypeCombo->addItem("网口", 1);  // 网络通信
    widgets.commTypeCombo->setFixedWidth(80);
    connLayout->addWidget(widgets.commTypeCombo, 0, 4);

    // 打开/关闭按钮（绿色样式）
    widgets.btnOpen = new QPushButton("打开", connectionGroup);
    widgets.btnOpen->setFixedWidth(70);
    widgets.btnOpen->setStyleSheet(R"(
        QPushButton {
            background-color: #4CAF50;
            color: white;
            border: none;
            border-radius: 4px;
            padding: 5px 10px;
        }
        QPushButton:hover {
            background-color: #45a049;
        }
        QPushButton:pressed {
            background-color: #3d8b40;
        }
    )");
    connLayout->addWidget(widgets.btnOpen, 0, 5);

    // ----- 第二行：串口号、波特率 -----

    // 串口号标签和下拉框
    connLayout->addWidget(new QLabel("串口号:", connectionGroup), 1, 1);
    widgets.serialPortCombo = new QComboBox(connectionGroup);

    // 动态获取系统中可用的串口
    const auto portInfos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo& info : portInfos) {
        widgets.serialPortCombo->addItem(info.portName());
    }
    // 如果没有检测到串口，添加默认选项
    if (widgets.serialPortCombo->count() == 0) {
        for (int p = 1; p <= 8; ++p) {
            widgets.serialPortCombo->addItem(QString("COM%1").arg(p));
        }
    }
    widgets.serialPortCombo->setFixedWidth(100);
    connLayout->addWidget(widgets.serialPortCombo, 1, 2);

    // 波特率标签和下拉框
    connLayout->addWidget(new QLabel("波特率:", connectionGroup), 1, 3);
    widgets.baudRateCombo = new QComboBox(connectionGroup);
    widgets.baudRateCombo->addItems({"9600", "19200", "38400", "57600", "115200"});
    widgets.baudRateCombo->setCurrentIndex(1);  // 默认19200
    widgets.baudRateCombo->setFixedWidth(100);
    connLayout->addWidget(widgets.baudRateCombo, 1, 4);

    // 第7列占据剩余空间
    connLayout->setColumnStretch(6, 1);
    controllerLayout->addWidget(connectionGroup);

    // ===== 光源通道亮度设置分组 =====
    QGroupBox* brightnessGroup = new QGroupBox("光源通道亮度设置", page);
    QVBoxLayout* brightnessMainLayout = new QVBoxLayout(brightnessGroup);

    // ----- 全部开启/关闭按钮区域 -----
    QHBoxLayout* btnLayout = new QHBoxLayout();
    widgets.btnEnableAll = new QPushButton("全部开启", brightnessGroup);
    widgets.btnEnableAll->setFixedWidth(80);
    widgets.btnDisableAll = new QPushButton("全部关闭", brightnessGroup);
    widgets.btnDisableAll->setFixedWidth(80);
    btnLayout->addWidget(widgets.btnEnableAll);
    btnLayout->addWidget(widgets.btnDisableAll);
    btnLayout->addStretch();
    brightnessMainLayout->addLayout(btnLayout);

    // ----- 8个通道的滑块和控件区域 -----
    QHBoxLayout* channelLayout = new QHBoxLayout();
    channelLayout->setSpacing(15);

    // 创建8个通道的控件
    for (int ch = 0; ch < 8; ++ch) {
        QVBoxLayout* chLayout = new QVBoxLayout();
        chLayout->setSpacing(5);
        chLayout->setAlignment(Qt::AlignHCenter);

        // 通道开关按钮（可切换状态）
        widgets.channelBtns[ch] = new QPushButton(QString("CH%1").arg(ch + 1), brightnessGroup);
        widgets.channelBtns[ch]->setCheckable(true);  // 设置为可切换按钮
        widgets.channelBtns[ch]->setFixedWidth(50);
        // 按钮样式：未选中灰色，选中绿色
        widgets.channelBtns[ch]->setStyleSheet(R"(
            QPushButton {
                background-color: #e0e0e0;
                border: 1px solid #ccc;
                border-radius: 4px;
                padding: 3px;
            }
            QPushButton:checked {
                background-color: #4CAF50;
                color: white;
                border: 1px solid #45a049;
            }
        )");
        chLayout->addWidget(widgets.channelBtns[ch]);

        // 当前亮度值标签
        widgets.valueLabels[ch] = new QLabel("0", brightnessGroup);
        widgets.valueLabels[ch]->setAlignment(Qt::AlignCenter);
        widgets.valueLabels[ch]->setFixedWidth(50);
        widgets.valueLabels[ch]->setStyleSheet("font-weight: bold; font-size: 12px;");
        chLayout->addWidget(widgets.valueLabels[ch]);

        // 垂直亮度滑块
        widgets.sliders[ch] = new QSlider(Qt::Vertical, brightnessGroup);
        widgets.sliders[ch]->setRange(0, 255);    // 默认范围0-255
        widgets.sliders[ch]->setValue(0);         // 默认值0
        widgets.sliders[ch]->setFixedHeight(180); // 固定高度
        // 滑块样式：蓝色滑块和轨道
        widgets.sliders[ch]->setStyleSheet(R"(
            QSlider::groove:vertical {
                background: #e0e0e0;
                width: 8px;
                border-radius: 4px;
            }
            QSlider::handle:vertical {
                background: #4a90d9;
                height: 16px;
                margin: 0 -4px;
                border-radius: 8px;
            }
            QSlider::sub-page:vertical {
                background: #4a90d9;
                border-radius: 4px;
            }
        )");
        chLayout->addWidget(widgets.sliders[ch], 0, Qt::AlignHCenter);

        // 数值输入框（可直接输入亮度值）
        widgets.valueEdits[ch] = new QLineEdit("0", brightnessGroup);
        widgets.valueEdits[ch]->setFixedWidth(50);
        widgets.valueEdits[ch]->setAlignment(Qt::AlignCenter);
        chLayout->addWidget(widgets.valueEdits[ch]);

        channelLayout->addLayout(chLayout);

        // ===== 连接通道控件的信号 =====

        // 保存索引值用于lambda捕获
        const int ctrlIdx = index;
        const int chIdx = ch;

        // 滑块值改变 -> 更新标签和发送亮度命令
        connect(widgets.sliders[ch], &QSlider::valueChanged, [this, ctrlIdx, chIdx, &widgets](int value) {
            // 更新当前值标签
            widgets.valueLabels[chIdx]->setText(QString::number(value));
            // 调用槽函数处理亮度变化
            onLightSliderValueChanged(chIdx, value);
        });

        // 输入框编辑完成 -> 同步到滑块
        connect(widgets.valueEdits[ch], &QLineEdit::editingFinished, [&widgets, chIdx]() {
            bool ok;
            int value = widgets.valueEdits[chIdx]->text().toInt(&ok);
            // 验证输入值有效性
            if (ok && value >= 0 && value <= 255) {
                widgets.sliders[chIdx]->setValue(value);
            }
        });

        // 通道按钮切换 -> 更新通道开关状态
        connect(widgets.channelBtns[ch], &QPushButton::toggled, [this, ctrlIdx, chIdx](bool checked) {
            auto& mgr = Comm::LightControllerManager::instance();
            mgr.setChannelEnabled(ctrlIdx, chIdx, checked);
        });
    }

    channelLayout->addStretch();
    brightnessMainLayout->addLayout(channelLayout);

    // 亮度设置分组占据剩余空间
    controllerLayout->addWidget(brightnessGroup, 1);

    // ===== 兼容旧代码：保存第一个控制器的控件引用 =====
    if (index == 0) {
        chkLightEnable_ = widgets.chkEnable;
        controllerTypeCombo_ = widgets.typeCombo;
        commTypeCombo_ = widgets.commTypeCombo;
        serialPortCombo_ = widgets.serialPortCombo;
        baudRateCombo_ = widgets.baudRateCombo;
        btnOpenLight_ = widgets.btnOpen;

        for (int ch = 0; ch < 8; ++ch) {
            lightSliders_[ch] = widgets.sliders[ch];
            lightValueLabels_[ch] = widgets.valueLabels[ch];
            lightValueEdits_[ch] = widgets.valueEdits[ch];
        }
    }

    // ===== 连接页面级别的信号 =====
    connect(widgets.chkEnable, &QCheckBox::toggled, this, &OptionsDialog::onLightEnableChanged);
    connect(widgets.typeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &OptionsDialog::onLightTypeChanged);
    connect(widgets.commTypeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &OptionsDialog::onCommTypeChanged);
    connect(widgets.btnOpen, &QPushButton::clicked, this, &OptionsDialog::onOpenLightClicked);
    connect(widgets.btnEnableAll, &QPushButton::clicked, this, &OptionsDialog::onEnableAllChannelsClicked);
    connect(widgets.btnDisableAll, &QPushButton::clicked, this, &OptionsDialog::onDisableAllChannelsClicked);
}

// ============================================================================
// 工位布局页面构建
// ============================================================================

/**
 * @brief 构建工位布局页面
 * @details 创建6种预设布局类型的选择界面，每种布局带有可视化预览
 */
void OptionsDialog::setupLayoutPage()
{
    layoutPage_ = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(layoutPage_);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(10, 10, 10, 10);

    // ===== 工位布局设置分组 =====
    QGroupBox* layoutGroup = new QGroupBox("工位布局设置", layoutPage_);
    QVBoxLayout* layoutGroupLayout = new QVBoxLayout(layoutGroup);
    layoutGroupLayout->setSpacing(15);

    // 创建布局类型按钮组（互斥选择）
    QButtonGroup* layoutTypeGroup = new QButtonGroup(this);

    // 使用3x2网格排列6种布局类型
    QGridLayout* gridLayout = new QGridLayout();
    gridLayout->setSpacing(20);

    // 预览框样式
    QString previewStyle = R"(
        QFrame {
            background-color: #ffffff;
            border: 1px solid #a0a0a0;
        }
    )";

    // 工位标签样式
    QString stationStyle = R"(
        QLabel {
            background-color: #ffffff;
            border: 1px solid #808080;
            font-size: 11px;
        }
    )";

    // 创建6种布局类型
    for (int i = 0; i < 6; ++i) {
        int row = i / 3;  // 行号：0 或 1
        int col = i % 3;  // 列号：0, 1, 2

        // 单个布局类型的容器
        QVBoxLayout* typeLayout = new QVBoxLayout();
        typeLayout->setSpacing(5);

        // 单选按钮
        QRadioButton* radio = new QRadioButton(QString("类型%1").arg(i + 1), this);
        if (i == 0) radio->setChecked(true);  // 默认选中第一个
        layoutTypeGroup->addButton(radio, i);
        typeLayout->addWidget(radio);

        // 预览区域框架
        QFrame* previewFrame = new QFrame(this);
        previewFrame->setFixedSize(150, 120);
        previewFrame->setStyleSheet(previewStyle);
        previewFrame->setFrameShape(QFrame::Box);

        // 预览区域内的布局
        QVBoxLayout* previewLayout = new QVBoxLayout(previewFrame);
        previewLayout->setContentsMargins(8, 8, 8, 8);
        previewLayout->setSpacing(4);

        // 根据类型创建不同的预览布局示意
        if (i == 0) {
            // ----- 类型1: 单个工位（左上角） -----
            QLabel* station1 = new QLabel("工位1", previewFrame);
            station1->setAlignment(Qt::AlignLeft | Qt::AlignTop);
            station1->setStyleSheet(stationStyle);
            station1->setFixedSize(40, 25);
            previewLayout->addWidget(station1, 0, Qt::AlignLeft | Qt::AlignTop);
            previewLayout->addStretch();
        } else if (i == 1) {
            // ----- 类型2: 上下两个工位（垂直排列） -----
            QLabel* station1 = new QLabel("工位1", previewFrame);
            station1->setAlignment(Qt::AlignCenter);
            station1->setStyleSheet(stationStyle);
            previewLayout->addWidget(station1);

            QLabel* station2 = new QLabel("工位2", previewFrame);
            station2->setAlignment(Qt::AlignCenter);
            station2->setStyleSheet(stationStyle);
            previewLayout->addWidget(station2);
        } else if (i == 2) {
            // ----- 类型3: 左右两个工位（水平排列） -----
            QHBoxLayout* hLayout = new QHBoxLayout();
            hLayout->setSpacing(4);

            QLabel* station1 = new QLabel("工位1", previewFrame);
            station1->setAlignment(Qt::AlignCenter);
            station1->setStyleSheet(stationStyle);
            hLayout->addWidget(station1);

            QLabel* station2 = new QLabel("工位2", previewFrame);
            station2->setAlignment(Qt::AlignCenter);
            station2->setStyleSheet(stationStyle);
            hLayout->addWidget(station2);

            previewLayout->addLayout(hLayout);
        } else if (i == 3) {
            // ----- 类型4: 2x2四个工位（网格排列） -----
            QGridLayout* grid = new QGridLayout();
            grid->setSpacing(4);

            for (int s = 0; s < 4; ++s) {
                QLabel* station = new QLabel(QString("工位%1").arg(s + 1), previewFrame);
                station->setAlignment(Qt::AlignCenter);
                station->setStyleSheet(stationStyle + "font-size: 9px;");
                grid->addWidget(station, s / 2, s % 2);  // 行=s/2, 列=s%2
            }
            previewLayout->addLayout(grid);
        } else if (i == 4) {
            // ----- 类型5: 左大右两小（1+2布局） -----
            QHBoxLayout* hLayout = new QHBoxLayout();
            hLayout->setSpacing(4);

            // 左侧大工位（占2份宽度）
            QLabel* station1 = new QLabel("工位1", previewFrame);
            station1->setAlignment(Qt::AlignCenter);
            station1->setStyleSheet(stationStyle);
            hLayout->addWidget(station1, 2);

            // 右侧两个小工位（垂直排列，占1份宽度）
            QVBoxLayout* rightLayout = new QVBoxLayout();
            rightLayout->setSpacing(4);

            QLabel* station2 = new QLabel("工位2", previewFrame);
            station2->setAlignment(Qt::AlignCenter);
            station2->setStyleSheet(stationStyle + "font-size: 9px;");
            rightLayout->addWidget(station2);

            QLabel* station3 = new QLabel("工位3", previewFrame);
            station3->setAlignment(Qt::AlignCenter);
            station3->setStyleSheet(stationStyle + "font-size: 9px;");
            rightLayout->addWidget(station3);

            hLayout->addLayout(rightLayout, 1);
            previewLayout->addLayout(hLayout);
        } else if (i == 5) {
            // ----- 类型6: 3x2六个工位（网格排列） -----
            QGridLayout* grid = new QGridLayout();
            grid->setSpacing(3);

            for (int s = 0; s < 6; ++s) {
                QLabel* station = new QLabel(QString("工位%1").arg(s + 1), previewFrame);
                station->setAlignment(Qt::AlignCenter);
                station->setStyleSheet(stationStyle + "font-size: 8px;");
                grid->addWidget(station, s / 3, s % 3);  // 行=s/3, 列=s%3
            }
            previewLayout->addLayout(grid);
        }

        typeLayout->addWidget(previewFrame);

        // 将布局类型容器添加到网格
        QWidget* typeWidget = new QWidget(this);
        typeWidget->setLayout(typeLayout);
        gridLayout->addWidget(typeWidget, row, col);
    }

    layoutGroupLayout->addLayout(gridLayout);
    mainLayout->addWidget(layoutGroup);
    mainLayout->addStretch();
}

// ============================================================================
// 工具栏导航槽函数
// ============================================================================

/**
 * @brief 工具栏按钮点击响应
 * @param index 按钮索引
 * @details 更新按钮样式并切换显示对应的设置页面
 */
void OptionsDialog::onToolButtonClicked(int index)
{
    // 更新按钮样式：选中的按钮使用蓝色样式，其他使用灰色样式
    btnBasic_->setStyleSheet(index == 0 ? toolBtnSelectedStyle_ : toolBtnNormalStyle_);
    btnLight_->setStyleSheet(index == 1 ? toolBtnSelectedStyle_ : toolBtnNormalStyle_);
    btnLayout_->setStyleSheet(index == 2 ? toolBtnSelectedStyle_ : toolBtnNormalStyle_);

    // 切换堆叠页面的当前索引
    stackedWidget_->setCurrentIndex(index);
}

// ============================================================================
// 配置加载和保存
// ============================================================================

/**
 * @brief 加载已保存的设置
 * @details 初始化光源控制器管理器，从配置文件加载设置，更新UI显示
 */
void OptionsDialog::loadSettings()
{
    // 获取光源控制器管理器单例
    auto& lightMgr = Comm::LightControllerManager::instance();

    // 构建配置文件路径（应用程序目录下的config文件夹）
    QString configPath = QCoreApplication::applicationDirPath() + "/config/light_config.json";

    // 初始化管理器并加载配置
    lightMgr.initialize(configPath);

    // 将配置同步到所有控制器的UI
    for (int i = 0; i < 4; ++i) {
        syncLightConfigToUI(i);
    }

    LOG_INFO("选项设置已加载");
}

/**
 * @brief 应用当前设置
 * @details 从UI读取所有设置，保存到配置文件
 */
void OptionsDialog::applySettings()
{
    // 从UI同步所有控制器的配置
    for (int i = 0; i < 4; ++i) {
        syncLightConfigFromUI(i);
    }

    // 保存光源配置到文件
    auto& lightMgr = Comm::LightControllerManager::instance();
    lightMgr.saveConfig();

    LOG_INFO("选项设置已保存");
}

/**
 * @brief 确定按钮点击响应
 * @details 应用设置并以Accepted状态关闭对话框
 */
void OptionsDialog::onOkClicked()
{
    applySettings();
    accept();  // 关闭对话框，返回QDialog::Accepted
}

/**
 * @brief 取消按钮点击响应
 * @details 不保存修改，以Rejected状态关闭对话框
 */
void OptionsDialog::onCancelClicked()
{
    reject();  // 关闭对话框，返回QDialog::Rejected
}

// ============================================================================
// 光源控制槽函数实现
// ============================================================================

/**
 * @brief 光源控制器标签页切换响应
 * @param tabIndex 新的标签页索引
 * @details 更新当前控制器索引，同步UI显示
 */
void OptionsDialog::onLightControllerTabChanged(int tabIndex)
{
    // 记录当前选中的控制器索引
    currentLightControllerIndex_ = tabIndex;
    // 从管理器同步配置到UI
    syncLightConfigToUI(tabIndex);
}

/**
 * @brief 光源控制器启用状态改变响应
 * @param enabled 新的启用状态
 * @details 更新配置并刷新UI控件的可用状态
 */
void OptionsDialog::onLightEnableChanged(bool enabled)
{
    int idx = currentLightControllerIndex_;
    auto& mgr = Comm::LightControllerManager::instance();

    // 更新配置
    mgr.controllerConfig(idx).used = enabled;

    // 更新控件使能状态
    updateLightControllerUI(idx);
}

/**
 * @brief 光源控制器类型改变响应
 * @param index 下拉框选中索引
 * @details 更新控制器类型配置，调整亮度滑块的范围
 */
void OptionsDialog::onLightTypeChanged(int index)
{
    int idx = currentLightControllerIndex_;
    auto& mgr = Comm::LightControllerManager::instance();
    auto& widgets = lightPageWidgets_[idx];

    // 从下拉框获取类型枚举值并更新配置
    int typeValue = widgets.typeCombo->itemData(index).toInt();
    mgr.controllerConfig(idx).type = static_cast<Comm::LightControllerType>(typeValue);

    // 根据控制器类型获取亮度范围
    int minVal, maxVal;
    Comm::LightControllerManager::getBrightnessRange(
        static_cast<Comm::LightControllerType>(typeValue), minVal, maxVal);

    // 更新所有滑块的范围
    for (int ch = 0; ch < 8; ++ch) {
        widgets.sliders[ch]->setRange(minVal, maxVal);
    }
}

/**
 * @brief 通信类型改变响应
 * @param index 下拉框选中索引（0=串口, 1=网口）
 * @details 更新配置，TODO: 根据通信类型显示/隐藏对应的配置区域
 */
void OptionsDialog::onCommTypeChanged(int index)
{
    int idx = currentLightControllerIndex_;
    auto& mgr = Comm::LightControllerManager::instance();

    // 更新配置
    mgr.controllerConfig(idx).commType = static_cast<Comm::LightCommType>(index);

    // TODO: 根据通信类型显示/隐藏串口或网络配置
}

/**
 * @brief 打开/关闭光源按钮点击响应
 * @details 根据当前连接状态执行相反操作，更新按钮样式
 */
void OptionsDialog::onOpenLightClicked()
{
    int idx = currentLightControllerIndex_;
    auto& mgr = Comm::LightControllerManager::instance();
    auto& widgets = lightPageWidgets_[idx];

    if (mgr.isControllerOpen(idx)) {
        // ----- 当前已连接，执行关闭操作 -----
        mgr.closeController(idx);

        // 更新按钮为"打开"状态（绿色）
        widgets.btnOpen->setText("打开");
        widgets.btnOpen->setStyleSheet(R"(
            QPushButton {
                background-color: #4CAF50;
                color: white;
                border: none;
                border-radius: 4px;
                padding: 5px 10px;
            }
            QPushButton:hover { background-color: #45a049; }
        )");
        LOG_INFO(QString("光源控制器%1已关闭").arg(idx + 1));
    } else {
        // ----- 当前未连接，执行打开操作 -----

        // 先从UI同步配置（确保使用最新的串口号和波特率）
        syncLightConfigFromUI(idx);

        // 尝试打开控制器
        if (mgr.openController(idx)) {
            // 打开成功，更新按钮为"关闭"状态（红色）
            widgets.btnOpen->setText("关闭");
            widgets.btnOpen->setStyleSheet(R"(
                QPushButton {
                    background-color: #f44336;
                    color: white;
                    border: none;
                    border-radius: 4px;
                    padding: 5px 10px;
                }
                QPushButton:hover { background-color: #da190b; }
            )");
            LOG_INFO(QString("光源控制器%1已打开").arg(idx + 1));
        } else {
            // 打开失败，弹出错误提示
            QMessageBox::warning(this, "错误",
                QString("打开光源控制器%1失败，请检查连接设置！").arg(idx + 1));
        }
    }

    // 更新控件状态
    updateLightConnectionStatus(idx);
}

/**
 * @brief 光源通道亮度滑块值改变响应
 * @param channelIndex 通道索引（0-7）
 * @param value 新的亮度值
 * @details 更新配置并同步数值输入框显示
 */
void OptionsDialog::onLightSliderValueChanged(int channelIndex, int value)
{
    int idx = currentLightControllerIndex_;
    auto& mgr = Comm::LightControllerManager::instance();

    // 设置亮度到管理器（会同时更新配置和硬件）
    mgr.setChannelBrightness(idx, channelIndex, value);

    // 更新数值输入框显示
    auto& widgets = lightPageWidgets_[idx];
    widgets.valueEdits[channelIndex]->setText(QString::number(value));
}

/**
 * @brief 全部开启通道按钮点击响应
 * @details 开启当前控制器的所有通道，更新UI
 */
void OptionsDialog::onEnableAllChannelsClicked()
{
    int idx = currentLightControllerIndex_;
    auto& mgr = Comm::LightControllerManager::instance();
    auto& widgets = lightPageWidgets_[idx];

    // 调用管理器批量开启通道
    mgr.enableAllChannels(idx);

    // 更新UI：所有通道按钮设为选中状态
    for (int ch = 0; ch < 8; ++ch) {
        widgets.channelBtns[ch]->setChecked(true);
    }
}

/**
 * @brief 全部关闭通道按钮点击响应
 * @details 关闭当前控制器的所有通道，更新UI
 */
void OptionsDialog::onDisableAllChannelsClicked()
{
    int idx = currentLightControllerIndex_;
    auto& mgr = Comm::LightControllerManager::instance();
    auto& widgets = lightPageWidgets_[idx];

    // 调用管理器批量关闭通道
    mgr.disableAllChannels(idx);

    // 更新UI：所有通道按钮设为未选中状态
    for (int ch = 0; ch < 8; ++ch) {
        widgets.channelBtns[ch]->setChecked(false);
    }
}

// ============================================================================
// 光源控制辅助函数实现
// ============================================================================

/**
 * @brief 更新光源控制器UI的可用状态
 * @param index 控制器索引
 * @details 根据控制器的启用状态和连接状态，设置各控件的enabled属性：
 *          - 未启用时：大部分控件禁用
 *          - 启用但未连接：连接配置可用，通道控制禁用
 *          - 启用且已连接：连接配置禁用（锁定），通道控制可用
 */
void OptionsDialog::updateLightControllerUI(int index)
{
    auto& mgr = Comm::LightControllerManager::instance();
    const auto& cfg = mgr.controllerConfig(index);
    auto& widgets = lightPageWidgets_[index];

    bool enabled = cfg.used;                        // 控制器是否启用
    bool connected = mgr.isControllerOpen(index);   // 控制器是否已连接

    // ----- 连接设置控件：启用但未连接时可编辑 -----
    widgets.typeCombo->setEnabled(enabled && !connected);
    widgets.commTypeCombo->setEnabled(enabled && !connected);
    widgets.serialPortCombo->setEnabled(enabled && !connected);
    widgets.baudRateCombo->setEnabled(enabled && !connected);
    widgets.btnOpen->setEnabled(enabled);  // 打开按钮只要启用就可用

    // ----- 批量通道控制按钮：启用且已连接时可用 -----
    widgets.btnEnableAll->setEnabled(enabled && connected);
    widgets.btnDisableAll->setEnabled(enabled && connected);

    // ----- 单通道控件：根据通道数和连接状态设置 -----
    for (int ch = 0; ch < 8; ++ch) {
        // 通道在有效范围内、控制器启用且已连接时可用
        bool chEnabled = (ch < cfg.maxChannels) && enabled && connected;
        widgets.channelBtns[ch]->setEnabled(chEnabled);
        // 滑块和输入框只有通道开启时才可操作
        widgets.sliders[ch]->setEnabled(chEnabled && widgets.channelBtns[ch]->isChecked());
        widgets.valueEdits[ch]->setEnabled(chEnabled && widgets.channelBtns[ch]->isChecked());
    }
}

/**
 * @brief 更新光源控制器的连接状态显示
 * @param index 控制器索引
 * @details 根据连接状态更新打开/关闭按钮的文字和样式
 */
void OptionsDialog::updateLightConnectionStatus(int index)
{
    auto& mgr = Comm::LightControllerManager::instance();
    auto& widgets = lightPageWidgets_[index];

    bool connected = mgr.isControllerOpen(index);

    if (connected) {
        // ----- 已连接状态：红色"关闭"按钮 -----
        widgets.btnOpen->setText("关闭");
        widgets.btnOpen->setStyleSheet(R"(
            QPushButton {
                background-color: #f44336;
                color: white;
                border: none;
                border-radius: 4px;
                padding: 5px 10px;
            }
            QPushButton:hover { background-color: #da190b; }
        )");
    } else {
        // ----- 未连接状态：绿色"打开"按钮 -----
        widgets.btnOpen->setText("打开");
        widgets.btnOpen->setStyleSheet(R"(
            QPushButton {
                background-color: #4CAF50;
                color: white;
                border: none;
                border-radius: 4px;
                padding: 5px 10px;
            }
            QPushButton:hover { background-color: #45a049; }
        )");
    }

    // 同时更新其他控件的可用状态
    updateLightControllerUI(index);
}

/**
 * @brief 从UI同步配置到管理器
 * @param index 控制器索引
 * @details 读取UI控件的当前值，更新到LightControllerManager的配置中
 */
void OptionsDialog::syncLightConfigFromUI(int index)
{
    auto& mgr = Comm::LightControllerManager::instance();
    auto& cfg = mgr.controllerConfig(index);
    auto& widgets = lightPageWidgets_[index];

    // 同步启用状态
    cfg.used = widgets.chkEnable->isChecked();

    // 同步控制器类型（从下拉框的itemData获取枚举值）
    cfg.type = static_cast<Comm::LightControllerType>(
        widgets.typeCombo->currentData().toInt());

    // 同步通信类型
    cfg.commType = static_cast<Comm::LightCommType>(
        widgets.commTypeCombo->currentData().toInt());

    // 解析串口号（从"COM3"提取数字3，再减1得到索引2）
    QString portName = widgets.serialPortCombo->currentText();
    if (portName.startsWith("COM", Qt::CaseInsensitive)) {
        cfg.serialPort = portName.mid(3).toInt() - 1;
    }

    // 同步波特率
    cfg.baudRate = widgets.baudRateCombo->currentText().toInt();

    // 同步所有通道的亮度和开关状态
    for (int ch = 0; ch < 8; ++ch) {
        cfg.channels[ch].brightness = widgets.sliders[ch]->value();
        cfg.channels[ch].enabled = widgets.channelBtns[ch]->isChecked();
    }
}

/**
 * @brief 从管理器同步配置到UI
 * @param index 控制器索引
 * @details 读取LightControllerManager的配置，更新UI控件的显示
 */
void OptionsDialog::syncLightConfigToUI(int index)
{
    auto& mgr = Comm::LightControllerManager::instance();
    const auto& cfg = mgr.controllerConfig(index);
    auto& widgets = lightPageWidgets_[index];

    // ===== 阻止信号发送（避免更新UI时触发槽函数） =====
    widgets.chkEnable->blockSignals(true);
    widgets.typeCombo->blockSignals(true);
    widgets.commTypeCombo->blockSignals(true);
    widgets.serialPortCombo->blockSignals(true);
    widgets.baudRateCombo->blockSignals(true);

    // 同步启用状态
    widgets.chkEnable->setChecked(cfg.used);

    // 同步控制器类型（查找匹配的itemData）
    for (int i = 0; i < widgets.typeCombo->count(); ++i) {
        if (widgets.typeCombo->itemData(i).toInt() == static_cast<int>(cfg.type)) {
            widgets.typeCombo->setCurrentIndex(i);
            break;
        }
    }

    // 同步通信类型
    widgets.commTypeCombo->setCurrentIndex(static_cast<int>(cfg.commType));

    // 同步串口号（构建"COMx"字符串并查找）
    QString portName = QString("COM%1").arg(cfg.serialPort + 1);
    int portIdx = widgets.serialPortCombo->findText(portName);
    if (portIdx >= 0) {
        widgets.serialPortCombo->setCurrentIndex(portIdx);
    }

    // 同步波特率（查找匹配的文本）
    int baudIdx = widgets.baudRateCombo->findText(QString::number(cfg.baudRate));
    if (baudIdx >= 0) {
        widgets.baudRateCombo->setCurrentIndex(baudIdx);
    }

    // ===== 恢复信号发送 =====
    widgets.chkEnable->blockSignals(false);
    widgets.typeCombo->blockSignals(false);
    widgets.commTypeCombo->blockSignals(false);
    widgets.serialPortCombo->blockSignals(false);
    widgets.baudRateCombo->blockSignals(false);

    // ===== 同步通道状态 =====
    for (int ch = 0; ch < 8; ++ch) {
        // 阻止信号
        widgets.sliders[ch]->blockSignals(true);
        widgets.channelBtns[ch]->blockSignals(true);

        // 设置亮度值
        widgets.sliders[ch]->setValue(cfg.channels[ch].brightness);
        widgets.valueLabels[ch]->setText(QString::number(cfg.channels[ch].brightness));
        widgets.valueEdits[ch]->setText(QString::number(cfg.channels[ch].brightness));

        // 设置通道开关状态
        widgets.channelBtns[ch]->setChecked(cfg.channels[ch].enabled);

        // 恢复信号
        widgets.sliders[ch]->blockSignals(false);
        widgets.channelBtns[ch]->blockSignals(false);
    }

    // 更新控件的可用状态显示
    updateLightConnectionStatus(index);
}

} // namespace UI
} // namespace VisionForge
