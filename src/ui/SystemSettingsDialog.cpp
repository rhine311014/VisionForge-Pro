/**
 * @file SystemSettingsDialog.cpp
 * @brief 系统设置对话框实现 - VisionASM风格
 */

#include "ui/SystemSettingsDialog.h"
#include "ui/ImageSourceDialog.h"
#include "base/Logger.h"
#include "base/ConfigManager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGridLayout>
#include <QFileDialog>
#include <QMessageBox>

namespace VisionForge {
namespace UI {

SystemSettingsDialog::SystemSettingsDialog(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle("系统设置");
    setMinimumSize(900, 700);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    // 初始化按钮样式
    toolBtnNormalStyle_ = R"(
        QPushButton {
            background-color: #e0e0e0;
            border: 1px solid #b0b0b0;
            border-radius: 5px;
            padding: 8px;
            min-width: 70px;
            min-height: 70px;
            font-size: 12px;
        }
        QPushButton:hover {
            background-color: #d0d0d0;
        }
    )";

    toolBtnSelectedStyle_ = R"(
        QPushButton {
            background-color: #0078d4;
            border: 1px solid #005a9e;
            border-radius: 5px;
            padding: 8px;
            min-width: 70px;
            min-height: 70px;
            font-size: 12px;
            color: white;
        }
    )";

    setupUI();
    loadSettings();
}

void SystemSettingsDialog::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(10, 10, 10, 10);

    // 顶部工具栏
    setupToolBar();
    mainLayout->addWidget(toolBar_);

    // 页面切换区域
    stackedWidget_ = new QStackedWidget(this);

    // 创建各个页面
    setupImagePage();
    setupLogPage();
    setupLayoutPage();
    setupPlatformPage();

    stackedWidget_->addWidget(imagePage_);
    stackedWidget_->addWidget(logPage_);
    stackedWidget_->addWidget(layoutPage_);
    stackedWidget_->addWidget(platformPage_);

    mainLayout->addWidget(stackedWidget_, 1);

    // 底部按钮区域
    QHBoxLayout* bottomLayout = new QHBoxLayout();
    bottomLayout->addStretch();

    // 确定按钮
    okButton_ = new QPushButton(this);
    okButton_->setFixedSize(80, 80);
    okButton_->setText("确定");
    okButton_->setStyleSheet(R"(
        QPushButton {
            background-color: #4fc3f7;
            border: none;
            border-radius: 5px;
            font-size: 14px;
            font-weight: bold;
        }
        QPushButton:hover {
            background-color: #29b6f6;
        }
    )");
    connect(okButton_, &QPushButton::clicked, this, &SystemSettingsDialog::onOkClicked);
    bottomLayout->addWidget(okButton_);

    // 取消按钮
    cancelButton_ = new QPushButton(this);
    cancelButton_->setFixedSize(80, 80);
    cancelButton_->setText("取消");
    cancelButton_->setStyleSheet(R"(
        QPushButton {
            background-color: #ef5350;
            border: none;
            border-radius: 5px;
            font-size: 14px;
            font-weight: bold;
            color: white;
        }
        QPushButton:hover {
            background-color: #e53935;
        }
    )");
    connect(cancelButton_, &QPushButton::clicked, this, &SystemSettingsDialog::onCancelClicked);
    bottomLayout->addWidget(cancelButton_);

    mainLayout->addLayout(bottomLayout);

    // 默认选中图像页面
    onToolButtonClicked(0);
}

void SystemSettingsDialog::setupToolBar()
{
    toolBar_ = new QWidget(this);
    QHBoxLayout* toolLayout = new QHBoxLayout(toolBar_);
    toolLayout->setSpacing(10);
    toolLayout->setContentsMargins(0, 0, 0, 0);

    toolButtonGroup_ = new QButtonGroup(this);

    // 图像按钮
    btnImage_ = new QPushButton("图像", this);
    btnImage_->setStyleSheet(toolBtnNormalStyle_);
    toolButtonGroup_->addButton(btnImage_, 0);
    toolLayout->addWidget(btnImage_);

    // 日志按钮
    btnLog_ = new QPushButton("日志", this);
    btnLog_->setStyleSheet(toolBtnNormalStyle_);
    toolButtonGroup_->addButton(btnLog_, 1);
    toolLayout->addWidget(btnLog_);

    // 布局按钮
    btnLayout_ = new QPushButton("布局", this);
    btnLayout_->setStyleSheet(toolBtnNormalStyle_);
    toolButtonGroup_->addButton(btnLayout_, 2);
    toolLayout->addWidget(btnLayout_);

    // 平台按钮
    btnPlatform_ = new QPushButton("平台", this);
    btnPlatform_->setStyleSheet(toolBtnNormalStyle_);
    toolButtonGroup_->addButton(btnPlatform_, 3);
    toolLayout->addWidget(btnPlatform_);

    toolLayout->addStretch();

    // 右上角边检按钮
    btnBianJian_ = new QPushButton("边检", this);
    btnBianJian_->setFixedSize(70, 70);
    btnBianJian_->setStyleSheet(R"(
        QPushButton {
            background-color: #e3f2fd;
            border: 1px solid #90caf9;
            border-radius: 5px;
            font-size: 12px;
        }
        QPushButton:hover {
            background-color: #bbdefb;
        }
    )");
    toolLayout->addWidget(btnBianJian_);

    connect(toolButtonGroup_, QOverload<int>::of(&QButtonGroup::idClicked),
            this, &SystemSettingsDialog::onToolButtonClicked);
}

void SystemSettingsDialog::setupImagePage()
{
    imagePage_ = new QWidget(this);
    QHBoxLayout* pageLayout = new QHBoxLayout(imagePage_);
    pageLayout->setSpacing(15);

    // 左侧列
    QVBoxLayout* leftColumn = new QVBoxLayout();
    leftColumn->setSpacing(10);

    // ===== 存储模式 =====
    storageModeGroup_ = new QGroupBox("存储模式", imagePage_);
    QVBoxLayout* storageModeLayout = new QVBoxLayout(storageModeGroup_);

    QHBoxLayout* saveModeLayout = new QHBoxLayout();
    radioFlowSave_ = new QRadioButton("流程保存", this);
    radioThreadSave_ = new QRadioButton("线程保存", this);
    radioThreadSave_->setChecked(true);
    saveModeLayout->addWidget(radioFlowSave_);
    saveModeLayout->addWidget(radioThreadSave_);
    saveModeLayout->addStretch();
    storageModeLayout->addLayout(saveModeLayout);

    QHBoxLayout* diskLayout = new QHBoxLayout();
    diskLayout->addWidget(new QLabel("磁盘使用百分比:", this));
    diskUsageSpin_ = new QSpinBox(this);
    diskUsageSpin_->setRange(1, 100);
    diskUsageSpin_->setValue(90);
    diskUsageSpin_->setFixedWidth(80);
    diskLayout->addWidget(diskUsageSpin_);
    diskLayout->addWidget(new QLabel("%", this));
    diskLayout->addStretch();
    storageModeLayout->addLayout(diskLayout);

    leftColumn->addWidget(storageModeGroup_);

    // ===== 存图路径 =====
    savePathGroup_ = new QGroupBox("存图路径", imagePage_);
    savePathGroup_->setCheckable(true);
    savePathGroup_->setChecked(true);
    QHBoxLayout* pathLayout = new QHBoxLayout(savePathGroup_);

    btnBrowsePath_ = new QPushButton("路径设置", this);
    connect(btnBrowsePath_, &QPushButton::clicked, this, &SystemSettingsDialog::onBrowsePath);
    pathLayout->addWidget(btnBrowsePath_);

    savePathEdit_ = new QLineEdit("E:\\VSImage\\WS0", this);
    savePathEdit_->setReadOnly(true);
    pathLayout->addWidget(savePathEdit_);

    leftColumn->addWidget(savePathGroup_);

    // ===== 时刻存图 =====
    timeSaveGroup_ = new QGroupBox("时刻存图", imagePage_);
    timeSaveGroup_->setCheckable(true);
    timeSaveGroup_->setChecked(true);
    QVBoxLayout* timeSaveLayout = new QVBoxLayout(timeSaveGroup_);

    QHBoxLayout* timeTypeLayout = new QHBoxLayout();
    radioHourSave_ = new QRadioButton("小时保存", this);
    radioDaySave_ = new QRadioButton("按天保存", this);
    radioDaySave_->setChecked(true);
    timeTypeLayout->addWidget(radioHourSave_);
    timeTypeLayout->addWidget(radioDaySave_);
    timeTypeLayout->addStretch();
    timeSaveLayout->addLayout(timeTypeLayout);

    QHBoxLayout* dailyStartLayout = new QHBoxLayout();
    dailyStartLayout->addWidget(new QLabel("每天记录起始时间(24H):", this));
    dailyStartTimeSpin_ = new QSpinBox(this);
    dailyStartTimeSpin_->setRange(0, 23);
    dailyStartTimeSpin_->setValue(8);
    dailyStartTimeSpin_->setFixedWidth(60);
    dailyStartLayout->addWidget(dailyStartTimeSpin_);
    dailyStartLayout->addWidget(new QLabel("h", this));
    dailyStartLayout->addStretch();
    timeSaveLayout->addLayout(dailyStartLayout);

    QHBoxLayout* maxFoldersLayout = new QHBoxLayout();
    maxFoldersLayout->addWidget(new QLabel("时间记录最大文件夹数量:", this));
    maxFoldersSpin_ = new QSpinBox(this);
    maxFoldersSpin_->setRange(1, 1000);
    maxFoldersSpin_->setValue(48);
    maxFoldersSpin_->setFixedWidth(60);
    maxFoldersLayout->addWidget(maxFoldersSpin_);
    maxFoldersLayout->addStretch();
    timeSaveLayout->addLayout(maxFoldersLayout);

    leftColumn->addWidget(timeSaveGroup_);

    // ===== 图像文件 =====
    imageFileGroup_ = new QGroupBox("图像文件", imagePage_);
    imageFileGroup_->setCheckable(true);
    imageFileGroup_->setChecked(true);
    QVBoxLayout* imageFileLayout = new QVBoxLayout(imageFileGroup_);

    chkProductIdNaming_ = new QCheckBox("产品ID命名存图", this);
    chkProductIdNaming_->setChecked(true);
    imageFileLayout->addWidget(chkProductIdNaming_);

    QHBoxLayout* failedLayout = new QHBoxLayout();
    chkSaveFailedImages_ = new QCheckBox("保存失败图像", this);
    chkSaveFailedImages_->setChecked(true);
    failedLayout->addWidget(chkSaveFailedImages_);
    failedLayout->addWidget(new QLabel("存储数量:", this));
    failedImageCountSpin_ = new QSpinBox(this);
    failedImageCountSpin_->setRange(1, 10000);
    failedImageCountSpin_->setValue(50);
    failedImageCountSpin_->setFixedWidth(60);
    failedLayout->addWidget(failedImageCountSpin_);
    failedLayout->addStretch();
    imageFileLayout->addLayout(failedLayout);

    QHBoxLayout* allLayout = new QHBoxLayout();
    chkSaveAllImages_ = new QCheckBox("保存所有图像", this);
    chkSaveAllImages_->setChecked(true);
    allLayout->addWidget(chkSaveAllImages_);
    allLayout->addWidget(new QLabel("存储数量:", this));
    allImageCountSpin_ = new QSpinBox(this);
    allImageCountSpin_->setRange(1, 10000);
    allImageCountSpin_->setValue(30);
    allImageCountSpin_->setFixedWidth(60);
    allLayout->addWidget(allImageCountSpin_);
    allLayout->addStretch();
    imageFileLayout->addLayout(allLayout);

    QHBoxLayout* formatLayout = new QHBoxLayout();
    formatLayout->addWidget(new QLabel("存储格式:", this));
    imageFormatCombo_ = new QComboBox(this);
    imageFormatCombo_->addItems({"BMP", "JPEG", "PNG", "TIFF"});
    imageFormatCombo_->setFixedWidth(80);
    formatLayout->addWidget(imageFormatCombo_);
    formatLayout->addStretch();
    imageFileLayout->addLayout(formatLayout);

    leftColumn->addWidget(imageFileGroup_);
    leftColumn->addStretch();

    pageLayout->addLayout(leftColumn);

    // 右侧列
    QVBoxLayout* rightColumn = new QVBoxLayout();
    rightColumn->setSpacing(10);

    // ===== VDB文件 =====
    vdbFileGroup_ = new QGroupBox("VDB文件", imagePage_);
    chkVdbEnabled_ = new QCheckBox("", this);
    chkVdbEnabled_->setChecked(true);
    vdbFileGroup_->setCheckable(true);
    vdbFileGroup_->setChecked(true);
    QVBoxLayout* vdbLayout = new QVBoxLayout(vdbFileGroup_);

    chkVdbProductIdNaming_ = new QCheckBox("产品ID命名存图", this);
    chkVdbProductIdNaming_->setChecked(true);
    vdbLayout->addWidget(chkVdbProductIdNaming_);

    QHBoxLayout* vdbFailedLayout = new QHBoxLayout();
    chkVdbSaveFailedImages_ = new QCheckBox("保存失败图像", this);
    chkVdbSaveFailedImages_->setChecked(true);
    vdbFailedLayout->addWidget(chkVdbSaveFailedImages_);
    vdbFailedLayout->addWidget(new QLabel("存储数量:", this));
    vdbFailedImageCountSpin_ = new QSpinBox(this);
    vdbFailedImageCountSpin_->setRange(1, 10000);
    vdbFailedImageCountSpin_->setValue(40);
    vdbFailedImageCountSpin_->setFixedWidth(60);
    vdbFailedLayout->addWidget(vdbFailedImageCountSpin_);
    vdbFailedLayout->addStretch();
    vdbLayout->addLayout(vdbFailedLayout);

    QHBoxLayout* vdbAllLayout = new QHBoxLayout();
    chkVdbSaveAllImages_ = new QCheckBox("保存所有图像", this);
    chkVdbSaveAllImages_->setChecked(true);
    vdbAllLayout->addWidget(chkVdbSaveAllImages_);
    vdbAllLayout->addWidget(new QLabel("存储数量:", this));
    vdbAllImageCountSpin_ = new QSpinBox(this);
    vdbAllImageCountSpin_->setRange(1, 10000);
    vdbAllImageCountSpin_->setValue(40);
    vdbAllImageCountSpin_->setFixedWidth(60);
    vdbAllLayout->addWidget(vdbAllImageCountSpin_);
    vdbAllLayout->addStretch();
    vdbLayout->addLayout(vdbAllLayout);

    QHBoxLayout* vdbFormatLayout = new QHBoxLayout();
    vdbFormatLayout->addWidget(new QLabel("存储格式:", this));
    vdbFormatCombo_ = new QComboBox(this);
    vdbFormatCombo_->addItems({"JPEG", "BMP", "PNG"});
    vdbFormatCombo_->setFixedWidth(80);
    vdbFormatLayout->addWidget(vdbFormatCombo_);
    vdbFormatLayout->addStretch();
    vdbLayout->addLayout(vdbFormatLayout);

    QHBoxLayout* compressionLayout = new QHBoxLayout();
    compressionLayout->addWidget(new QLabel("压缩比例:", this));
    vdbCompressionSpin_ = new QSpinBox(this);
    vdbCompressionSpin_->setRange(1, 100);
    vdbCompressionSpin_->setValue(50);
    vdbCompressionSpin_->setFixedWidth(60);
    compressionLayout->addWidget(vdbCompressionSpin_);
    compressionLayout->addWidget(new QLabel("%", this));
    compressionLayout->addStretch();
    vdbLayout->addLayout(compressionLayout);

    rightColumn->addWidget(vdbFileGroup_);

    // ===== 自定义参数 =====
    customParamsGroup_ = new QGroupBox("自定义参数", imagePage_);
    customParamsGroup_->setCheckable(true);
    customParamsGroup_->setChecked(false);
    QVBoxLayout* customLayout = new QVBoxLayout(customParamsGroup_);

    QHBoxLayout* lineWidthLayout = new QHBoxLayout();
    lineWidthLayout->addWidget(new QLabel("线宽(1~5):", this));
    lineWidthSpin_ = new QSpinBox(this);
    lineWidthSpin_->setRange(1, 5);
    lineWidthSpin_->setValue(1);
    lineWidthSpin_->setFixedWidth(60);
    lineWidthLayout->addWidget(lineWidthSpin_);
    lineWidthLayout->addStretch();
    customLayout->addLayout(lineWidthLayout);

    QHBoxLayout* fontSizeLayout = new QHBoxLayout();
    fontSizeLayout->addWidget(new QLabel("字体(5~30):", this));
    fontSizeSpin_ = new QSpinBox(this);
    fontSizeSpin_->setRange(5, 30);
    fontSizeSpin_->setValue(12);
    fontSizeSpin_->setFixedWidth(60);
    fontSizeLayout->addWidget(fontSizeSpin_);
    fontSizeLayout->addStretch();
    customLayout->addLayout(fontSizeLayout);

    QHBoxLayout* labelLayout = new QHBoxLayout();
    labelLayout->addWidget(new QLabel("Label(1~12):", this));
    labelSizeSpin_ = new QSpinBox(this);
    labelSizeSpin_->setRange(1, 12);
    labelSizeSpin_->setValue(12);
    labelSizeSpin_->setFixedWidth(60);
    labelLayout->addWidget(labelSizeSpin_);
    labelLayout->addStretch();
    customLayout->addLayout(labelLayout);

    rightColumn->addWidget(customParamsGroup_);
    rightColumn->addStretch();

    pageLayout->addLayout(rightColumn);
}

void SystemSettingsDialog::setupLogPage()
{
    logPage_ = new QWidget(this);
    QHBoxLayout* pageLayout = new QHBoxLayout(logPage_);
    pageLayout->setSpacing(15);

    // 左侧列
    QVBoxLayout* leftColumn = new QVBoxLayout();
    leftColumn->setSpacing(10);

    // ===== 日志设置 =====
    QGroupBox* logSettingsGroup = new QGroupBox("日志设置", logPage_);
    QVBoxLayout* logSettingsLayout = new QVBoxLayout(logSettingsGroup);

    QHBoxLayout* row1Layout = new QHBoxLayout();
    QCheckBox* chkCommCmd = new QCheckBox("通信指令", this);
    chkCommCmd->setChecked(true);
    row1Layout->addWidget(chkCommCmd);
    QCheckBox* chkTimeStats = new QCheckBox("耗时统计", this);
    chkTimeStats->setChecked(true);
    row1Layout->addWidget(chkTimeStats);
    row1Layout->addStretch();
    logSettingsLayout->addLayout(row1Layout);

    QCheckBox* chkDetectionLog = new QCheckBox("检测流程日志", this);
    chkDetectionLog->setChecked(true);
    logSettingsLayout->addWidget(chkDetectionLog);

    QCheckBox* chkCsvData = new QCheckBox("检测数据文件(csv格式)", this);
    chkCsvData->setChecked(true);
    logSettingsLayout->addWidget(chkCsvData);

    leftColumn->addWidget(logSettingsGroup);

    // ===== 模拟运行 =====
    QGroupBox* simRunGroup = new QGroupBox("模拟运行", logPage_);
    QVBoxLayout* simRunLayout = new QVBoxLayout(simRunGroup);

    QHBoxLayout* simEnableLayout = new QHBoxLayout();
    QCheckBox* chkSimEnable = new QCheckBox("启用模拟运行", this);
    simEnableLayout->addWidget(chkSimEnable);
    QPushButton* btnImageSource = new QPushButton("图像数据源", this);
    connect(btnImageSource, &QPushButton::clicked, this, [this]() {
        ImageSourceDialog dialog(this);
        dialog.exec();
    });
    simEnableLayout->addWidget(btnImageSource);
    simEnableLayout->addStretch();
    simRunLayout->addLayout(simEnableLayout);

    QCheckBox* chkLoopData = new QCheckBox("循环使用数据", this);
    simRunLayout->addWidget(chkLoopData);

    QCheckBox* chkImageMatch = new QCheckBox("图像——对应", this);
    simRunLayout->addWidget(chkImageMatch);

    leftColumn->addWidget(simRunGroup);
    leftColumn->addStretch();

    pageLayout->addLayout(leftColumn);

    // 右侧列
    QVBoxLayout* rightColumn = new QVBoxLayout();
    rightColumn->setSpacing(10);

    // ===== 平台通信设置 =====
    QGroupBox* platformCommGroup = new QGroupBox("平台通信设置", logPage_);
    QVBoxLayout* platformCommLayout = new QVBoxLayout(platformCommGroup);

    QCheckBox* chkVisionCommStatus = new QCheckBox("视觉通信链接状态(寄存器通信)", this);
    chkVisionCommStatus->setChecked(true);
    platformCommLayout->addWidget(chkVisionCommStatus);

    rightColumn->addWidget(platformCommGroup);
    rightColumn->addStretch();

    pageLayout->addLayout(rightColumn);
    pageLayout->addStretch();
}

void SystemSettingsDialog::setupLayoutPage()
{
    layoutPage_ = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(layoutPage_);
    mainLayout->setSpacing(15);

    // ===== 视图布局 =====
    QGroupBox* viewLayoutGroup = new QGroupBox("视图布局", layoutPage_);
    QVBoxLayout* viewLayoutGroupLayout = new QVBoxLayout(viewLayoutGroup);
    viewLayoutGroupLayout->setSpacing(10);

    // 创建布局类型按钮组
    QButtonGroup* layoutTypeGroup = new QButtonGroup(this);

    // 3x2 网格布局
    QGridLayout* gridLayout = new QGridLayout();
    gridLayout->setSpacing(15);

    // 预览框样式
    QString previewStyle = R"(
        QFrame {
            background-color: #f0f0f0;
            border: 1px solid #c0c0c0;
            border-radius: 3px;
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
        previewFrame->setFixedSize(180, 120);
        previewFrame->setStyleSheet(previewStyle);
        previewFrame->setFrameShape(QFrame::StyledPanel);

        // 预览区域内的布局示意
        QVBoxLayout* previewLayout = new QVBoxLayout(previewFrame);
        previewLayout->setContentsMargins(5, 5, 5, 5);
        previewLayout->setSpacing(3);

        // 根据类型创建不同的预览布局
        if (i == 0) {
            // 类型1: 单个位置1
            QLabel* pos1 = new QLabel("位置1", previewFrame);
            pos1->setAlignment(Qt::AlignCenter);
            pos1->setStyleSheet("background-color: #e8e8e8; border: 1px solid #b0b0b0;");
            previewLayout->addWidget(pos1);
        } else if (i == 1) {
            // 类型2: 上下两个位置
            QLabel* pos1 = new QLabel("位置1", previewFrame);
            pos1->setAlignment(Qt::AlignCenter);
            pos1->setStyleSheet("background-color: #e8e8e8; border: 1px solid #b0b0b0;");
            previewLayout->addWidget(pos1);

            QLabel* pos2 = new QLabel("位置2", previewFrame);
            pos2->setAlignment(Qt::AlignCenter);
            pos2->setStyleSheet("background-color: #e8e8e8; border: 1px solid #b0b0b0;");
            previewLayout->addWidget(pos2);
        } else if (i == 2) {
            // 类型3: 左右两个位置
            QHBoxLayout* hLayout = new QHBoxLayout();
            hLayout->setSpacing(3);

            QLabel* pos1 = new QLabel("位置1", previewFrame);
            pos1->setAlignment(Qt::AlignCenter);
            pos1->setStyleSheet("background-color: #e8e8e8; border: 1px solid #b0b0b0;");
            hLayout->addWidget(pos1);

            QLabel* pos2 = new QLabel("位置2", previewFrame);
            pos2->setAlignment(Qt::AlignCenter);
            pos2->setStyleSheet("background-color: #e8e8e8; border: 1px solid #b0b0b0;");
            hLayout->addWidget(pos2);

            previewLayout->addLayout(hLayout);
        } else if (i == 3) {
            // 类型4: 2x2四个位置
            QGridLayout* grid = new QGridLayout();
            grid->setSpacing(3);

            for (int p = 0; p < 4; ++p) {
                QLabel* pos = new QLabel(QString("位置%1").arg(p + 1), previewFrame);
                pos->setAlignment(Qt::AlignCenter);
                pos->setStyleSheet("background-color: #e8e8e8; border: 1px solid #b0b0b0; font-size: 9px;");
                grid->addWidget(pos, p / 2, p % 2);
            }
            previewLayout->addLayout(grid);
        } else if (i == 4) {
            // 类型5: 左大右两小
            QHBoxLayout* hLayout = new QHBoxLayout();
            hLayout->setSpacing(3);

            QLabel* pos1 = new QLabel("位置1", previewFrame);
            pos1->setAlignment(Qt::AlignCenter);
            pos1->setStyleSheet("background-color: #e8e8e8; border: 1px solid #b0b0b0;");
            hLayout->addWidget(pos1, 2);

            QVBoxLayout* rightLayout = new QVBoxLayout();
            rightLayout->setSpacing(3);

            QLabel* pos2 = new QLabel("位置2", previewFrame);
            pos2->setAlignment(Qt::AlignCenter);
            pos2->setStyleSheet("background-color: #e8e8e8; border: 1px solid #b0b0b0; font-size: 9px;");
            rightLayout->addWidget(pos2);

            QLabel* pos3 = new QLabel("位置3", previewFrame);
            pos3->setAlignment(Qt::AlignCenter);
            pos3->setStyleSheet("background-color: #e8e8e8; border: 1px solid #b0b0b0; font-size: 9px;");
            rightLayout->addWidget(pos3);

            hLayout->addLayout(rightLayout, 1);
            previewLayout->addLayout(hLayout);
        } else if (i == 5) {
            // 类型6: 3x2六个位置
            QGridLayout* grid = new QGridLayout();
            grid->setSpacing(2);

            for (int p = 0; p < 6; ++p) {
                QLabel* pos = new QLabel(QString("位置%1").arg(p + 1), previewFrame);
                pos->setAlignment(Qt::AlignCenter);
                pos->setStyleSheet("background-color: #e8e8e8; border: 1px solid #b0b0b0; font-size: 8px;");
                grid->addWidget(pos, p / 3, p % 3);
            }
            previewLayout->addLayout(grid);
        }

        typeLayout->addWidget(previewFrame);

        // 添加到网格
        QWidget* typeWidget = new QWidget(this);
        typeWidget->setLayout(typeLayout);
        gridLayout->addWidget(typeWidget, row, col);
    }

    viewLayoutGroupLayout->addLayout(gridLayout);
    mainLayout->addWidget(viewLayoutGroup);

    // ===== 检测高度 =====
    QHBoxLayout* heightLayout = new QHBoxLayout();
    heightLayout->addWidget(new QLabel("检测高度：（↑。↓）", this));

    QSpinBox* heightSpin = new QSpinBox(this);
    heightSpin->setRange(0, 1000);
    heightSpin->setValue(260);
    heightSpin->setFixedWidth(80);
    heightLayout->addWidget(heightSpin);

    heightLayout->addStretch();
    mainLayout->addLayout(heightLayout);

    mainLayout->addStretch();
}

void SystemSettingsDialog::setupPlatformPage()
{
    platformPage_ = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(platformPage_);
    mainLayout->setSpacing(15);

    // ===== 平台类型选择 =====
    QHBoxLayout* typeLayout = new QHBoxLayout();
    typeLayout->addWidget(new QLabel("平台类型：", this));

    QComboBox* platformTypeCombo = new QComboBox(this);
    platformTypeCombo->addItems({"XYΘ", "XY", "XΘ", "YΘ", "X1X2Y", "XY1Y2"});
    platformTypeCombo->setFixedWidth(100);
    typeLayout->addWidget(platformTypeCombo);
    typeLayout->addStretch();
    mainLayout->addLayout(typeLayout);

    // ===== 主内容区域（左右布局） =====
    QHBoxLayout* contentLayout = new QHBoxLayout();
    contentLayout->setSpacing(30);

    // ===== 左侧：XYΘ平台方向 =====
    QGroupBox* xyqDirectionGroup = new QGroupBox("XYΘ平台方向", platformPage_);
    QFormLayout* xyqFormLayout = new QFormLayout(xyqDirectionGroup);
    xyqFormLayout->setSpacing(10);

    QComboBox* xDirCombo = new QComboBox(this);
    xDirCombo->addItems({"向右", "向左"});
    xDirCombo->setFixedWidth(100);
    xyqFormLayout->addRow("X轴正方向：", xDirCombo);

    QComboBox* yDirCombo = new QComboBox(this);
    yDirCombo->addItems({"向下", "向上"});
    yDirCombo->setFixedWidth(100);
    xyqFormLayout->addRow("Y轴正方向：", yDirCombo);

    QComboBox* qDirCombo = new QComboBox(this);
    qDirCombo->addItems({"顺时针", "逆时针"});
    qDirCombo->setFixedWidth(100);
    xyqFormLayout->addRow("Θ轴正方向：", qDirCombo);

    contentLayout->addWidget(xyqDirectionGroup);

    // ===== 右侧：X1X2Y平台设置（动态显示） =====
    QWidget* x1x2yPanel = new QWidget(this);
    QVBoxLayout* x1x2yLayout = new QVBoxLayout(x1x2yPanel);
    x1x2yLayout->setSpacing(10);
    x1x2yLayout->setContentsMargins(0, 0, 0, 0);

    // X1X2Y平台结构类型
    QGroupBox* structTypeGroup = new QGroupBox("X1X2Y平台结构类型", x1x2yPanel);
    QFormLayout* structFormLayout = new QFormLayout(structTypeGroup);

    QComboBox* structTypeCombo = new QComboBox(this);
    structTypeCombo->addItems({"固定滑块", "活动滑块"});
    structTypeCombo->setFixedWidth(120);
    structFormLayout->addRow("结构类型", structTypeCombo);

    x1x2yLayout->addWidget(structTypeGroup);

    // X1X2Y平台方向
    QGroupBox* x1x2yDirGroup = new QGroupBox("X1X2Y平台方向", x1x2yPanel);
    QFormLayout* x1x2yDirFormLayout = new QFormLayout(x1x2yDirGroup);

    QComboBox* x1DirCombo = new QComboBox(this);
    x1DirCombo->addItems({"向右", "向左"});
    x1DirCombo->setFixedWidth(100);
    x1x2yDirFormLayout->addRow("X1轴正方向", x1DirCombo);

    QComboBox* x2DirCombo = new QComboBox(this);
    x2DirCombo->addItems({"向右", "向左"});
    x2DirCombo->setFixedWidth(100);
    x1x2yDirFormLayout->addRow("X2轴正方向", x2DirCombo);

    QComboBox* y1DirCombo = new QComboBox(this);
    y1DirCombo->addItems({"向下", "向上"});
    y1DirCombo->setFixedWidth(100);
    x1x2yDirFormLayout->addRow("Y轴正方向", y1DirCombo);

    x1x2yLayout->addWidget(x1x2yDirGroup);

    // X1X2Y平台参数
    QGroupBox* x1x2yParamsGroup = new QGroupBox("X1X2Y平台参数", x1x2yPanel);
    QGridLayout* paramsGridLayout = new QGridLayout(x1x2yParamsGroup);
    paramsGridLayout->setSpacing(8);

    // X1轴连接点坐标
    paramsGridLayout->addWidget(new QLabel("X1轴连接点坐标：", this), 0, 0);
    paramsGridLayout->addWidget(new QLabel("X", this), 0, 1);
    QDoubleSpinBox* x1CoordX = new QDoubleSpinBox(this);
    x1CoordX->setRange(-1000, 1000);
    x1CoordX->setValue(67.5);
    x1CoordX->setFixedWidth(70);
    paramsGridLayout->addWidget(x1CoordX, 0, 2);
    paramsGridLayout->addWidget(new QLabel("mm  Y", this), 0, 3);
    QDoubleSpinBox* x1CoordY = new QDoubleSpinBox(this);
    x1CoordY->setRange(-1000, 1000);
    x1CoordY->setValue(-57);
    x1CoordY->setFixedWidth(70);
    paramsGridLayout->addWidget(x1CoordY, 0, 4);
    paramsGridLayout->addWidget(new QLabel("mm", this), 0, 5);

    // X2轴连接点坐标
    paramsGridLayout->addWidget(new QLabel("X2轴连接点坐标：", this), 1, 0);
    paramsGridLayout->addWidget(new QLabel("X", this), 1, 1);
    QDoubleSpinBox* x2CoordX = new QDoubleSpinBox(this);
    x2CoordX->setRange(-1000, 1000);
    x2CoordX->setValue(67.5);
    x2CoordX->setFixedWidth(70);
    paramsGridLayout->addWidget(x2CoordX, 1, 2);
    paramsGridLayout->addWidget(new QLabel("mm  Y", this), 1, 3);
    QDoubleSpinBox* x2CoordY = new QDoubleSpinBox(this);
    x2CoordY->setRange(-1000, 1000);
    x2CoordY->setValue(57);
    x2CoordY->setFixedWidth(70);
    paramsGridLayout->addWidget(x2CoordY, 1, 4);
    paramsGridLayout->addWidget(new QLabel("mm", this), 1, 5);

    // Y轴连接点坐标
    paramsGridLayout->addWidget(new QLabel("Y轴连接点坐标：", this), 2, 0);
    paramsGridLayout->addWidget(new QLabel("X", this), 2, 1);
    QDoubleSpinBox* yCoordX = new QDoubleSpinBox(this);
    yCoordX->setRange(-1000, 1000);
    yCoordX->setValue(-57);
    yCoordX->setFixedWidth(70);
    paramsGridLayout->addWidget(yCoordX, 2, 2);
    paramsGridLayout->addWidget(new QLabel("mm  Y", this), 2, 3);
    QDoubleSpinBox* yCoordY = new QDoubleSpinBox(this);
    yCoordY->setRange(-1000, 1000);
    yCoordY->setValue(67.5);
    yCoordY->setFixedWidth(70);
    paramsGridLayout->addWidget(yCoordY, 2, 4);
    paramsGridLayout->addWidget(new QLabel("mm", this), 2, 5);

    x1x2yLayout->addWidget(x1x2yParamsGroup);
    x1x2yLayout->addStretch();

    // 默认隐藏X1X2Y面板
    x1x2yPanel->setVisible(false);

    contentLayout->addWidget(x1x2yPanel);
    contentLayout->addStretch();

    mainLayout->addLayout(contentLayout);
    mainLayout->addStretch();

    // 连接平台类型切换信号
    connect(platformTypeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [x1x2yPanel, xyqDirectionGroup](int index) {
        // X1X2Y = 4, XY1Y2 = 5
        bool showX1X2Y = (index == 4 || index == 5);
        x1x2yPanel->setVisible(showX1X2Y);

        // 更新XYΘ平台方向的标题
        if (index == 4) {
            // X1X2Y模式保持原标题
        } else if (index == 5) {
            // XY1Y2模式
        }
    });
}

void SystemSettingsDialog::onToolButtonClicked(int index)
{
    // 更新按钮样式
    btnImage_->setStyleSheet(index == 0 ? toolBtnSelectedStyle_ : toolBtnNormalStyle_);
    btnLog_->setStyleSheet(index == 1 ? toolBtnSelectedStyle_ : toolBtnNormalStyle_);
    btnLayout_->setStyleSheet(index == 2 ? toolBtnSelectedStyle_ : toolBtnNormalStyle_);
    btnPlatform_->setStyleSheet(index == 3 ? toolBtnSelectedStyle_ : toolBtnNormalStyle_);

    // 切换页面
    stackedWidget_->setCurrentIndex(index);
}

void SystemSettingsDialog::onBrowsePath()
{
    QString dir = QFileDialog::getExistingDirectory(this, "选择存图路径",
        savePathEdit_->text(),
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if (!dir.isEmpty()) {
        savePathEdit_->setText(dir);
    }
}

void SystemSettingsDialog::loadSettings()
{
    Base::ConfigManager& config = Base::ConfigManager::instance();

    // 加载存图路径
    QString savePath = config.getValue("ImageSave/Path", "E:\\VSImage\\WS0").toString();
    savePathEdit_->setText(savePath);

    // 加载磁盘使用百分比
    int diskUsage = config.getValue("ImageSave/DiskUsage", 90).toInt();
    diskUsageSpin_->setValue(diskUsage);

    // 加载其他设置...
    LOG_DEBUG("系统设置已加载");
}

void SystemSettingsDialog::applySettings()
{
    Base::ConfigManager& config = Base::ConfigManager::instance();

    // 保存存图路径
    config.setValue("ImageSave/Path", savePathEdit_->text());

    // 保存磁盘使用百分比
    config.setValue("ImageSave/DiskUsage", diskUsageSpin_->value());

    // 保存存储模式
    config.setValue("ImageSave/ThreadMode", radioThreadSave_->isChecked());

    // 保存时刻存图设置
    config.setValue("ImageSave/TimeSaveEnabled", timeSaveGroup_->isChecked());
    config.setValue("ImageSave/DailySave", radioDaySave_->isChecked());
    config.setValue("ImageSave/DailyStartTime", dailyStartTimeSpin_->value());
    config.setValue("ImageSave/MaxFolders", maxFoldersSpin_->value());

    // 保存图像文件设置
    config.setValue("ImageSave/ProductIdNaming", chkProductIdNaming_->isChecked());
    config.setValue("ImageSave/SaveFailedImages", chkSaveFailedImages_->isChecked());
    config.setValue("ImageSave/FailedImageCount", failedImageCountSpin_->value());
    config.setValue("ImageSave/SaveAllImages", chkSaveAllImages_->isChecked());
    config.setValue("ImageSave/AllImageCount", allImageCountSpin_->value());
    config.setValue("ImageSave/ImageFormat", imageFormatCombo_->currentText());

    // 保存VDB设置
    config.setValue("VDB/Enabled", vdbFileGroup_->isChecked());
    config.setValue("VDB/ProductIdNaming", chkVdbProductIdNaming_->isChecked());
    config.setValue("VDB/SaveFailedImages", chkVdbSaveFailedImages_->isChecked());
    config.setValue("VDB/FailedImageCount", vdbFailedImageCountSpin_->value());
    config.setValue("VDB/SaveAllImages", chkVdbSaveAllImages_->isChecked());
    config.setValue("VDB/AllImageCount", vdbAllImageCountSpin_->value());
    config.setValue("VDB/Format", vdbFormatCombo_->currentText());
    config.setValue("VDB/Compression", vdbCompressionSpin_->value());

    // 保存自定义参数
    config.setValue("CustomParams/Enabled", customParamsGroup_->isChecked());
    config.setValue("CustomParams/LineWidth", lineWidthSpin_->value());
    config.setValue("CustomParams/FontSize", fontSizeSpin_->value());
    config.setValue("CustomParams/LabelSize", labelSizeSpin_->value());

    LOG_INFO("系统设置已保存");
}

void SystemSettingsDialog::onOkClicked()
{
    applySettings();
    accept();
}

void SystemSettingsDialog::onCancelClicked()
{
    reject();
}

} // namespace UI
} // namespace VisionForge
