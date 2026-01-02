/**
 * @file OptionsDialog.cpp
 * @brief 选项设置对话框实现
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

OptionsDialog::OptionsDialog(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle("选项设置");
    setMinimumSize(850, 650);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    setupUI();
    loadSettings();
}

void OptionsDialog::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(10, 10, 10, 10);

    // 顶部工具栏
    setupToolBar();
    mainLayout->addWidget(toolBar_);

    // 页面切换区域
    stackedWidget_ = new QStackedWidget(this);

    // 创建各页面
    setupBasicPage();
    setupLightPage();
    setupLayoutPage();

    stackedWidget_->addWidget(basicPage_);
    stackedWidget_->addWidget(lightPage_);
    stackedWidget_->addWidget(layoutPage_);

    mainLayout->addWidget(stackedWidget_, 1);

    // 底部按钮
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();

    okButton_ = new QPushButton("确定", this);
    okButton_->setFixedSize(80, 30);
    connect(okButton_, &QPushButton::clicked, this, &OptionsDialog::onOkClicked);
    buttonLayout->addWidget(okButton_);

    cancelButton_ = new QPushButton("取消", this);
    cancelButton_->setFixedSize(80, 30);
    connect(cancelButton_, &QPushButton::clicked, this, &OptionsDialog::onCancelClicked);
    buttonLayout->addWidget(cancelButton_);

    buttonLayout->addStretch();

    mainLayout->addLayout(buttonLayout);

    // 默认选中第一个按钮
    btnBasic_->setChecked(true);
    onToolButtonClicked(0);
}

void OptionsDialog::setupToolBar()
{
    toolBar_ = new QWidget(this);
    toolBar_->setFixedHeight(50);

    QHBoxLayout* toolLayout = new QHBoxLayout(toolBar_);
    toolLayout->setSpacing(5);
    toolLayout->setContentsMargins(5, 5, 5, 5);

    // 工具栏按钮样式
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

    toolButtonGroup_ = new QButtonGroup(this);
    toolButtonGroup_->setExclusive(true);

    btnBasic_ = new QPushButton("基础设置", this);
    btnBasic_->setCheckable(true);
    btnBasic_->setStyleSheet(toolBtnNormalStyle_);
    toolButtonGroup_->addButton(btnBasic_, 0);
    toolLayout->addWidget(btnBasic_);

    btnLight_ = new QPushButton("光源控制", this);
    btnLight_->setCheckable(true);
    btnLight_->setStyleSheet(toolBtnNormalStyle_);
    toolButtonGroup_->addButton(btnLight_, 1);
    toolLayout->addWidget(btnLight_);

    btnLayout_ = new QPushButton("工位布局", this);
    btnLayout_->setCheckable(true);
    btnLayout_->setStyleSheet(toolBtnNormalStyle_);
    toolButtonGroup_->addButton(btnLayout_, 2);
    toolLayout->addWidget(btnLayout_);

    toolLayout->addStretch();

    // 连接信号
    connect(toolButtonGroup_, QOverload<int>::of(&QButtonGroup::idClicked),
            this, &OptionsDialog::onToolButtonClicked);
}

void OptionsDialog::setupBasicPage()
{
    basicPage_ = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(basicPage_);
    layout->setSpacing(15);
    layout->setContentsMargins(10, 10, 10, 10);

    // ===== 语言设置 =====
    languageGroup_ = new QGroupBox("语言设置", basicPage_);
    QHBoxLayout* langLayout = new QHBoxLayout(languageGroup_);

    radioChineseSimplified_ = new QRadioButton("简体中文", languageGroup_);
    radioChineseSimplified_->setChecked(true);
    langLayout->addWidget(radioChineseSimplified_);

    radioEnglish_ = new QRadioButton("English", languageGroup_);
    langLayout->addWidget(radioEnglish_);

    langLayout->addStretch();

    layout->addWidget(languageGroup_);

    // ===== 图像显示 =====
    imageDisplayGroup_ = new QGroupBox("图像显示", basicPage_);
    QGridLayout* imageLayout = new QGridLayout(imageDisplayGroup_);
    imageLayout->setSpacing(10);

    QLabel* frameSkipLabel = new QLabel("抽帧数量:", imageDisplayGroup_);
    imageLayout->addWidget(frameSkipLabel, 0, 0);

    frameSkipSpin_ = new QSpinBox(imageDisplayGroup_);
    frameSkipSpin_->setRange(1, 100);
    frameSkipSpin_->setValue(1);
    frameSkipSpin_->setFixedWidth(80);
    imageLayout->addWidget(frameSkipSpin_, 0, 1);

    QLabel* displayResLabel = new QLabel("显示图像:", imageDisplayGroup_);
    imageLayout->addWidget(displayResLabel, 1, 0);

    displayResolutionCombo_ = new QComboBox(imageDisplayGroup_);
    displayResolutionCombo_->addItems({"原始大小", "1/2", "1/4", "1/8", "自适应"});
    displayResolutionCombo_->setCurrentIndex(4);
    displayResolutionCombo_->setFixedWidth(120);
    imageLayout->addWidget(displayResolutionCombo_, 1, 1);

    imageLayout->setColumnStretch(2, 1);

    layout->addWidget(imageDisplayGroup_);

    // ===== 启动设置 =====
    startupGroup_ = new QGroupBox("启动设置", basicPage_);
    QVBoxLayout* startupLayout = new QVBoxLayout(startupGroup_);

    chkCameraChannel_ = new QCheckBox("相机通道设置", startupGroup_);
    startupLayout->addWidget(chkCameraChannel_);

    chkVirtualKeyboard_ = new QCheckBox("虚拟屏幕键盘", startupGroup_);
    startupLayout->addWidget(chkVirtualKeyboard_);

    chkCommDisplay_ = new QCheckBox("通信指令显示", startupGroup_);
    startupLayout->addWidget(chkCommDisplay_);

    layout->addWidget(startupGroup_);

    // ===== 工位名称设置 =====
    stationNameGroup_ = new QGroupBox("工位名称设置", basicPage_);
    QVBoxLayout* stationLayout = new QVBoxLayout(stationNameGroup_);

    stationNameTable_ = new QTableWidget(stationNameGroup_);
    stationNameTable_->setColumnCount(2);
    stationNameTable_->setHorizontalHeaderLabels({"NO.", "自定义名称"});

    // 设置表格样式
    stationNameTable_->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    stationNameTable_->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    stationNameTable_->setColumnWidth(0, 60);
    stationNameTable_->verticalHeader()->setVisible(false);
    stationNameTable_->setSelectionBehavior(QAbstractItemView::SelectRows);

    // 添加默认行
    stationNameTable_->setRowCount(8);
    for (int i = 0; i < 8; ++i) {
        QTableWidgetItem* noItem = new QTableWidgetItem(QString::number(i + 1));
        noItem->setTextAlignment(Qt::AlignCenter);
        noItem->setFlags(noItem->flags() & ~Qt::ItemIsEditable);
        stationNameTable_->setItem(i, 0, noItem);

        QTableWidgetItem* nameItem = new QTableWidgetItem(QString("工位%1").arg(i + 1));
        stationNameTable_->setItem(i, 1, nameItem);

        stationNameTable_->setRowHeight(i, 30);
    }

    stationLayout->addWidget(stationNameTable_);

    layout->addWidget(stationNameGroup_, 1);
}

void OptionsDialog::setupLightPage()
{
    lightPage_ = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(lightPage_);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(10, 10, 10, 10);

    // ===== 光源控制器标签页 =====
    lightControllerTabs_ = new QTabWidget(lightPage_);

    // 为每个控制器创建标签页
    for (int i = 0; i < 4; ++i) {
        QWidget* controllerPage = new QWidget(this);
        setupLightControllerPage(i, controllerPage);
        lightControllerTabs_->addTab(controllerPage, QString("光源控制器%1").arg(i + 1));
    }

    // 连接标签页切换信号
    connect(lightControllerTabs_, &QTabWidget::currentChanged,
            this, &OptionsDialog::onLightControllerTabChanged);

    mainLayout->addWidget(lightControllerTabs_);

    // 初始化第一个控制器的UI
    syncLightConfigToUI(0);
}

void OptionsDialog::setupLightControllerPage(int index, QWidget* page)
{
    LightControllerPageWidgets& widgets = lightPageWidgets_[index];

    QVBoxLayout* controllerLayout = new QVBoxLayout(page);
    controllerLayout->setSpacing(15);

    // ===== 光源连接设置 =====
    QGroupBox* connectionGroup = new QGroupBox("光源连接设置", page);
    QGridLayout* connLayout = new QGridLayout(connectionGroup);
    connLayout->setSpacing(10);

    // 第一行：启用、控制器类型、通信类型、打开按钮
    widgets.chkEnable = new QCheckBox("启用", connectionGroup);
    connLayout->addWidget(widgets.chkEnable, 0, 0);

    connLayout->addWidget(new QLabel("控制器类型:", connectionGroup), 0, 1);
    widgets.typeCombo = new QComboBox(connectionGroup);
    widgets.typeCombo->addItem("康视达", static_cast<int>(Comm::LightControllerType::KangShiDa));
    widgets.typeCombo->addItem("锐视", static_cast<int>(Comm::LightControllerType::RuiShi));
    widgets.typeCombo->addItem("奥普特", static_cast<int>(Comm::LightControllerType::OPT));
    widgets.typeCombo->addItem("CCS", static_cast<int>(Comm::LightControllerType::CCS));
    widgets.typeCombo->addItem("力科胜", static_cast<int>(Comm::LightControllerType::LKS));
    widgets.typeCombo->setFixedWidth(100);
    connLayout->addWidget(widgets.typeCombo, 0, 2);

    connLayout->addWidget(new QLabel("通信类型:", connectionGroup), 0, 3);
    widgets.commTypeCombo = new QComboBox(connectionGroup);
    widgets.commTypeCombo->addItem("串口", 0);
    widgets.commTypeCombo->addItem("网口", 1);
    widgets.commTypeCombo->setFixedWidth(80);
    connLayout->addWidget(widgets.commTypeCombo, 0, 4);

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

    // 第二行：串口号、波特率
    connLayout->addWidget(new QLabel("串口号:", connectionGroup), 1, 1);
    widgets.serialPortCombo = new QComboBox(connectionGroup);
    // 动态获取可用串口
    const auto portInfos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo& info : portInfos) {
        widgets.serialPortCombo->addItem(info.portName());
    }
    if (widgets.serialPortCombo->count() == 0) {
        for (int p = 1; p <= 8; ++p) {
            widgets.serialPortCombo->addItem(QString("COM%1").arg(p));
        }
    }
    widgets.serialPortCombo->setFixedWidth(100);
    connLayout->addWidget(widgets.serialPortCombo, 1, 2);

    connLayout->addWidget(new QLabel("波特率:", connectionGroup), 1, 3);
    widgets.baudRateCombo = new QComboBox(connectionGroup);
    widgets.baudRateCombo->addItems({"9600", "19200", "38400", "57600", "115200"});
    widgets.baudRateCombo->setCurrentIndex(1);  // 默认19200
    widgets.baudRateCombo->setFixedWidth(100);
    connLayout->addWidget(widgets.baudRateCombo, 1, 4);

    connLayout->setColumnStretch(6, 1);
    controllerLayout->addWidget(connectionGroup);

    // ===== 光源通道亮度设置 =====
    QGroupBox* brightnessGroup = new QGroupBox("光源通道亮度设置", page);
    QVBoxLayout* brightnessMainLayout = new QVBoxLayout(brightnessGroup);

    // 全部开启/关闭按钮
    QHBoxLayout* btnLayout = new QHBoxLayout();
    widgets.btnEnableAll = new QPushButton("全部开启", brightnessGroup);
    widgets.btnEnableAll->setFixedWidth(80);
    widgets.btnDisableAll = new QPushButton("全部关闭", brightnessGroup);
    widgets.btnDisableAll->setFixedWidth(80);
    btnLayout->addWidget(widgets.btnEnableAll);
    btnLayout->addWidget(widgets.btnDisableAll);
    btnLayout->addStretch();
    brightnessMainLayout->addLayout(btnLayout);

    // 通道滑块区域
    QHBoxLayout* channelLayout = new QHBoxLayout();
    channelLayout->setSpacing(15);

    // 创建8个通道
    for (int ch = 0; ch < 8; ++ch) {
        QVBoxLayout* chLayout = new QVBoxLayout();
        chLayout->setSpacing(5);
        chLayout->setAlignment(Qt::AlignHCenter);

        // 通道开关按钮
        widgets.channelBtns[ch] = new QPushButton(QString("CH%1").arg(ch + 1), brightnessGroup);
        widgets.channelBtns[ch]->setCheckable(true);
        widgets.channelBtns[ch]->setFixedWidth(50);
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

        // 当前值标签
        widgets.valueLabels[ch] = new QLabel("0", brightnessGroup);
        widgets.valueLabels[ch]->setAlignment(Qt::AlignCenter);
        widgets.valueLabels[ch]->setFixedWidth(50);
        widgets.valueLabels[ch]->setStyleSheet("font-weight: bold; font-size: 12px;");
        chLayout->addWidget(widgets.valueLabels[ch]);

        // 垂直滑块
        widgets.sliders[ch] = new QSlider(Qt::Vertical, brightnessGroup);
        widgets.sliders[ch]->setRange(0, 255);
        widgets.sliders[ch]->setValue(0);
        widgets.sliders[ch]->setFixedHeight(180);
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

        // 数值输入框
        widgets.valueEdits[ch] = new QLineEdit("0", brightnessGroup);
        widgets.valueEdits[ch]->setFixedWidth(50);
        widgets.valueEdits[ch]->setAlignment(Qt::AlignCenter);
        chLayout->addWidget(widgets.valueEdits[ch]);

        channelLayout->addLayout(chLayout);

        // 连接信号
        const int ctrlIdx = index;
        const int chIdx = ch;

        connect(widgets.sliders[ch], &QSlider::valueChanged, [this, ctrlIdx, chIdx, &widgets](int value) {
            widgets.valueLabels[chIdx]->setText(QString::number(value));
            onLightSliderValueChanged(chIdx, value);
        });

        connect(widgets.valueEdits[ch], &QLineEdit::editingFinished, [&widgets, chIdx]() {
            bool ok;
            int value = widgets.valueEdits[chIdx]->text().toInt(&ok);
            if (ok && value >= 0 && value <= 255) {
                widgets.sliders[chIdx]->setValue(value);
            }
        });

        connect(widgets.channelBtns[ch], &QPushButton::toggled, [this, ctrlIdx, chIdx](bool checked) {
            auto& mgr = Comm::LightControllerManager::instance();
            mgr.setChannelEnabled(ctrlIdx, chIdx, checked);
        });
    }

    channelLayout->addStretch();
    brightnessMainLayout->addLayout(channelLayout);
    controllerLayout->addWidget(brightnessGroup, 1);

    // 保存第一个控制器的控件引用（兼容旧代码）
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

    // 连接信号
    connect(widgets.chkEnable, &QCheckBox::toggled, this, &OptionsDialog::onLightEnableChanged);
    connect(widgets.typeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &OptionsDialog::onLightTypeChanged);
    connect(widgets.commTypeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &OptionsDialog::onCommTypeChanged);
    connect(widgets.btnOpen, &QPushButton::clicked, this, &OptionsDialog::onOpenLightClicked);
    connect(widgets.btnEnableAll, &QPushButton::clicked, this, &OptionsDialog::onEnableAllChannelsClicked);
    connect(widgets.btnDisableAll, &QPushButton::clicked, this, &OptionsDialog::onDisableAllChannelsClicked);
}

void OptionsDialog::setupLayoutPage()
{
    layoutPage_ = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(layoutPage_);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(10, 10, 10, 10);

    // ===== 工位布局设置 =====
    QGroupBox* layoutGroup = new QGroupBox("工位布局设置", layoutPage_);
    QVBoxLayout* layoutGroupLayout = new QVBoxLayout(layoutGroup);
    layoutGroupLayout->setSpacing(15);

    // 创建布局类型按钮组
    QButtonGroup* layoutTypeGroup = new QButtonGroup(this);

    // 3x2 网格布局
    QGridLayout* gridLayout = new QGridLayout();
    gridLayout->setSpacing(20);

    // 预览框样式
    QString previewStyle = R"(
        QFrame {
            background-color: #ffffff;
            border: 1px solid #a0a0a0;
        }
    )";

    QString stationStyle = R"(
        QLabel {
            background-color: #ffffff;
            border: 1px solid #808080;
            font-size: 11px;
        }
    )";

    // 创建6种布局类型
    for (int i = 0; i < 6; ++i) {
        int row = i / 3;  // 0 或 1
        int col = i % 3;  // 0, 1, 2

        // 单个类型的容器
        QVBoxLayout* typeLayout = new QVBoxLayout();
        typeLayout->setSpacing(5);

        // 单选按钮
        QRadioButton* radio = new QRadioButton(QString("类型%1").arg(i + 1), this);
        if (i == 0) radio->setChecked(true);
        layoutTypeGroup->addButton(radio, i);
        typeLayout->addWidget(radio);

        // 预览区域
        QFrame* previewFrame = new QFrame(this);
        previewFrame->setFixedSize(150, 120);
        previewFrame->setStyleSheet(previewStyle);
        previewFrame->setFrameShape(QFrame::Box);

        // 预览区域内的布局示意
        QVBoxLayout* previewLayout = new QVBoxLayout(previewFrame);
        previewLayout->setContentsMargins(8, 8, 8, 8);
        previewLayout->setSpacing(4);

        // 根据类型创建不同的预览布局
        if (i == 0) {
            // 类型1: 单个工位1
            QLabel* station1 = new QLabel("工位1", previewFrame);
            station1->setAlignment(Qt::AlignLeft | Qt::AlignTop);
            station1->setStyleSheet(stationStyle);
            station1->setFixedSize(40, 25);
            previewLayout->addWidget(station1, 0, Qt::AlignLeft | Qt::AlignTop);
            previewLayout->addStretch();
        } else if (i == 1) {
            // 类型2: 上下两个工位
            QLabel* station1 = new QLabel("工位1", previewFrame);
            station1->setAlignment(Qt::AlignCenter);
            station1->setStyleSheet(stationStyle);
            previewLayout->addWidget(station1);

            QLabel* station2 = new QLabel("工位2", previewFrame);
            station2->setAlignment(Qt::AlignCenter);
            station2->setStyleSheet(stationStyle);
            previewLayout->addWidget(station2);
        } else if (i == 2) {
            // 类型3: 左右两个工位
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
            // 类型4: 2x2四个工位
            QGridLayout* grid = new QGridLayout();
            grid->setSpacing(4);

            for (int s = 0; s < 4; ++s) {
                QLabel* station = new QLabel(QString("工位%1").arg(s + 1), previewFrame);
                station->setAlignment(Qt::AlignCenter);
                station->setStyleSheet(stationStyle + "font-size: 9px;");
                grid->addWidget(station, s / 2, s % 2);
            }
            previewLayout->addLayout(grid);
        } else if (i == 4) {
            // 类型5: 左大右两小
            QHBoxLayout* hLayout = new QHBoxLayout();
            hLayout->setSpacing(4);

            QLabel* station1 = new QLabel("工位1", previewFrame);
            station1->setAlignment(Qt::AlignCenter);
            station1->setStyleSheet(stationStyle);
            hLayout->addWidget(station1, 2);

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
            // 类型6: 3x2六个工位
            QGridLayout* grid = new QGridLayout();
            grid->setSpacing(3);

            for (int s = 0; s < 6; ++s) {
                QLabel* station = new QLabel(QString("工位%1").arg(s + 1), previewFrame);
                station->setAlignment(Qt::AlignCenter);
                station->setStyleSheet(stationStyle + "font-size: 8px;");
                grid->addWidget(station, s / 3, s % 3);
            }
            previewLayout->addLayout(grid);
        }

        typeLayout->addWidget(previewFrame);

        // 添加到网格
        QWidget* typeWidget = new QWidget(this);
        typeWidget->setLayout(typeLayout);
        gridLayout->addWidget(typeWidget, row, col);
    }

    layoutGroupLayout->addLayout(gridLayout);
    mainLayout->addWidget(layoutGroup);
    mainLayout->addStretch();
}

void OptionsDialog::onToolButtonClicked(int index)
{
    // 更新按钮样式
    btnBasic_->setStyleSheet(index == 0 ? toolBtnSelectedStyle_ : toolBtnNormalStyle_);
    btnLight_->setStyleSheet(index == 1 ? toolBtnSelectedStyle_ : toolBtnNormalStyle_);
    btnLayout_->setStyleSheet(index == 2 ? toolBtnSelectedStyle_ : toolBtnNormalStyle_);

    // 切换页面
    stackedWidget_->setCurrentIndex(index);
}

void OptionsDialog::loadSettings()
{
    // 初始化光源控制器管理器
    auto& lightMgr = Comm::LightControllerManager::instance();
    QString configPath = QCoreApplication::applicationDirPath() + "/config/light_config.json";
    lightMgr.initialize(configPath);

    // 同步所有控制器的配置到UI
    for (int i = 0; i < 4; ++i) {
        syncLightConfigToUI(i);
    }

    LOG_INFO("选项设置已加载");
}

void OptionsDialog::applySettings()
{
    // 保存所有控制器的配置
    for (int i = 0; i < 4; ++i) {
        syncLightConfigFromUI(i);
    }

    // 保存光源配置
    auto& lightMgr = Comm::LightControllerManager::instance();
    lightMgr.saveConfig();

    LOG_INFO("选项设置已保存");
}

void OptionsDialog::onOkClicked()
{
    applySettings();
    accept();
}

void OptionsDialog::onCancelClicked()
{
    reject();
}

// ========== 光源控制槽函数实现 ==========

void OptionsDialog::onLightControllerTabChanged(int tabIndex)
{
    currentLightControllerIndex_ = tabIndex;
    syncLightConfigToUI(tabIndex);
}

void OptionsDialog::onLightEnableChanged(bool enabled)
{
    int idx = currentLightControllerIndex_;
    auto& mgr = Comm::LightControllerManager::instance();
    mgr.controllerConfig(idx).used = enabled;

    // 更新控件使能状态
    updateLightControllerUI(idx);
}

void OptionsDialog::onLightTypeChanged(int index)
{
    int idx = currentLightControllerIndex_;
    auto& mgr = Comm::LightControllerManager::instance();
    auto& widgets = lightPageWidgets_[idx];

    int typeValue = widgets.typeCombo->itemData(index).toInt();
    mgr.controllerConfig(idx).type = static_cast<Comm::LightControllerType>(typeValue);

    // 更新亮度范围
    int minVal, maxVal;
    Comm::LightControllerManager::getBrightnessRange(
        static_cast<Comm::LightControllerType>(typeValue), minVal, maxVal);

    for (int ch = 0; ch < 8; ++ch) {
        widgets.sliders[ch]->setRange(minVal, maxVal);
    }
}

void OptionsDialog::onCommTypeChanged(int index)
{
    int idx = currentLightControllerIndex_;
    auto& mgr = Comm::LightControllerManager::instance();

    mgr.controllerConfig(idx).commType = static_cast<Comm::LightCommType>(index);

    // TODO: 根据通信类型显示/隐藏串口或网络配置
}

void OptionsDialog::onOpenLightClicked()
{
    int idx = currentLightControllerIndex_;
    auto& mgr = Comm::LightControllerManager::instance();
    auto& widgets = lightPageWidgets_[idx];

    if (mgr.isControllerOpen(idx)) {
        // 关闭控制器
        mgr.closeController(idx);
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
        // 保存配置
        syncLightConfigFromUI(idx);

        // 打开控制器
        if (mgr.openController(idx)) {
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
            QMessageBox::warning(this, "错误",
                QString("打开光源控制器%1失败，请检查连接设置！").arg(idx + 1));
        }
    }

    updateLightConnectionStatus(idx);
}

void OptionsDialog::onLightSliderValueChanged(int channelIndex, int value)
{
    int idx = currentLightControllerIndex_;
    auto& mgr = Comm::LightControllerManager::instance();

    // 设置亮度
    mgr.setChannelBrightness(idx, channelIndex, value);

    // 更新输入框
    auto& widgets = lightPageWidgets_[idx];
    widgets.valueEdits[channelIndex]->setText(QString::number(value));
}

void OptionsDialog::onEnableAllChannelsClicked()
{
    int idx = currentLightControllerIndex_;
    auto& mgr = Comm::LightControllerManager::instance();
    auto& widgets = lightPageWidgets_[idx];

    mgr.enableAllChannels(idx);

    // 更新UI
    for (int ch = 0; ch < 8; ++ch) {
        widgets.channelBtns[ch]->setChecked(true);
    }
}

void OptionsDialog::onDisableAllChannelsClicked()
{
    int idx = currentLightControllerIndex_;
    auto& mgr = Comm::LightControllerManager::instance();
    auto& widgets = lightPageWidgets_[idx];

    mgr.disableAllChannels(idx);

    // 更新UI
    for (int ch = 0; ch < 8; ++ch) {
        widgets.channelBtns[ch]->setChecked(false);
    }
}

// ========== 光源控制辅助函数实现 ==========

void OptionsDialog::updateLightControllerUI(int index)
{
    auto& mgr = Comm::LightControllerManager::instance();
    const auto& cfg = mgr.controllerConfig(index);
    auto& widgets = lightPageWidgets_[index];

    bool enabled = cfg.used;
    bool connected = mgr.isControllerOpen(index);

    // 连接设置在未连接时可编辑
    widgets.typeCombo->setEnabled(enabled && !connected);
    widgets.commTypeCombo->setEnabled(enabled && !connected);
    widgets.serialPortCombo->setEnabled(enabled && !connected);
    widgets.baudRateCombo->setEnabled(enabled && !connected);
    widgets.btnOpen->setEnabled(enabled);

    // 通道控制在连接后可用
    widgets.btnEnableAll->setEnabled(enabled && connected);
    widgets.btnDisableAll->setEnabled(enabled && connected);

    for (int ch = 0; ch < 8; ++ch) {
        bool chEnabled = (ch < cfg.maxChannels) && enabled && connected;
        widgets.channelBtns[ch]->setEnabled(chEnabled);
        widgets.sliders[ch]->setEnabled(chEnabled && widgets.channelBtns[ch]->isChecked());
        widgets.valueEdits[ch]->setEnabled(chEnabled && widgets.channelBtns[ch]->isChecked());
    }
}

void OptionsDialog::updateLightConnectionStatus(int index)
{
    auto& mgr = Comm::LightControllerManager::instance();
    auto& widgets = lightPageWidgets_[index];

    bool connected = mgr.isControllerOpen(index);

    if (connected) {
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

    updateLightControllerUI(index);
}

void OptionsDialog::syncLightConfigFromUI(int index)
{
    auto& mgr = Comm::LightControllerManager::instance();
    auto& cfg = mgr.controllerConfig(index);
    auto& widgets = lightPageWidgets_[index];

    cfg.used = widgets.chkEnable->isChecked();
    cfg.type = static_cast<Comm::LightControllerType>(
        widgets.typeCombo->currentData().toInt());
    cfg.commType = static_cast<Comm::LightCommType>(
        widgets.commTypeCombo->currentData().toInt());

    // 解析串口号
    QString portName = widgets.serialPortCombo->currentText();
    if (portName.startsWith("COM", Qt::CaseInsensitive)) {
        cfg.serialPort = portName.mid(3).toInt() - 1;
    }

    cfg.baudRate = widgets.baudRateCombo->currentText().toInt();

    // 通道亮度
    for (int ch = 0; ch < 8; ++ch) {
        cfg.channels[ch].brightness = widgets.sliders[ch]->value();
        cfg.channels[ch].enabled = widgets.channelBtns[ch]->isChecked();
    }
}

void OptionsDialog::syncLightConfigToUI(int index)
{
    auto& mgr = Comm::LightControllerManager::instance();
    const auto& cfg = mgr.controllerConfig(index);
    auto& widgets = lightPageWidgets_[index];

    // 阻止信号
    widgets.chkEnable->blockSignals(true);
    widgets.typeCombo->blockSignals(true);
    widgets.commTypeCombo->blockSignals(true);
    widgets.serialPortCombo->blockSignals(true);
    widgets.baudRateCombo->blockSignals(true);

    widgets.chkEnable->setChecked(cfg.used);

    // 查找类型索引
    for (int i = 0; i < widgets.typeCombo->count(); ++i) {
        if (widgets.typeCombo->itemData(i).toInt() == static_cast<int>(cfg.type)) {
            widgets.typeCombo->setCurrentIndex(i);
            break;
        }
    }

    widgets.commTypeCombo->setCurrentIndex(static_cast<int>(cfg.commType));

    // 设置串口
    QString portName = QString("COM%1").arg(cfg.serialPort + 1);
    int portIdx = widgets.serialPortCombo->findText(portName);
    if (portIdx >= 0) {
        widgets.serialPortCombo->setCurrentIndex(portIdx);
    }

    // 设置波特率
    int baudIdx = widgets.baudRateCombo->findText(QString::number(cfg.baudRate));
    if (baudIdx >= 0) {
        widgets.baudRateCombo->setCurrentIndex(baudIdx);
    }

    // 恢复信号
    widgets.chkEnable->blockSignals(false);
    widgets.typeCombo->blockSignals(false);
    widgets.commTypeCombo->blockSignals(false);
    widgets.serialPortCombo->blockSignals(false);
    widgets.baudRateCombo->blockSignals(false);

    // 通道状态
    for (int ch = 0; ch < 8; ++ch) {
        widgets.sliders[ch]->blockSignals(true);
        widgets.channelBtns[ch]->blockSignals(true);

        widgets.sliders[ch]->setValue(cfg.channels[ch].brightness);
        widgets.valueLabels[ch]->setText(QString::number(cfg.channels[ch].brightness));
        widgets.valueEdits[ch]->setText(QString::number(cfg.channels[ch].brightness));
        widgets.channelBtns[ch]->setChecked(cfg.channels[ch].enabled);

        widgets.sliders[ch]->blockSignals(false);
        widgets.channelBtns[ch]->blockSignals(false);
    }

    updateLightConnectionStatus(index);
}

} // namespace UI
} // namespace VisionForge
