/**
 * @file AlignmentOutputDialog.cpp
 * @brief 对位输出配置对话框实现
 * @author VisionForge Team
 * @date 2025-12-19
 */

#include "ui/AlignmentOutputDialog.h"
#include "base/Logger.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QCheckBox>
#include <QTextEdit>
#include <QTabWidget>
#include <QHeaderView>
#include <QMessageBox>
#include <QSerialPortInfo>

namespace VisionForge {
namespace UI {

AlignmentOutputDialog::AlignmentOutputDialog(QWidget* parent)
    : QDialog(parent)
    , outputTool_(nullptr)
{
    setWindowTitle(tr("对位输出配置"));
    setMinimumSize(700, 550);

    setupUI();
}

AlignmentOutputDialog::~AlignmentOutputDialog()
{
}

void AlignmentOutputDialog::setOutputTool(Algorithm::AlignmentOutputTool* tool)
{
    outputTool_ = tool;
    updateFromTool();
}

Algorithm::AlignmentOutputTool* AlignmentOutputDialog::getOutputTool() const
{
    return outputTool_;
}

void AlignmentOutputDialog::setupUI()
{
    auto* mainLayout = new QVBoxLayout(this);

    // 输出方式选择
    createOutputMethodGroup();
    mainLayout->addWidget(outputMethodGroup_);

    // 标签页
    tabWidget_ = new QTabWidget();

    createPLCTab();
    createTCPTab();
    createSerialTab();

    tabWidget_->addTab(plcTab_, tr("PLC输出"));
    tabWidget_->addTab(tcpTab_, tr("TCP输出"));
    tabWidget_->addTab(serialTab_, tr("串口输出"));

    mainLayout->addWidget(tabWidget_);

    // 底部按钮
    createButtonGroup();
    auto* btnLayout = new QHBoxLayout();
    btnLayout->addStretch();
    btnLayout->addWidget(applyBtn_);
    btnLayout->addWidget(okBtn_);
    btnLayout->addWidget(cancelBtn_);

    mainLayout->addLayout(btnLayout);
}

void AlignmentOutputDialog::createOutputMethodGroup()
{
    outputMethodGroup_ = new QGroupBox(tr("输出方式"));
    auto* layout = new QHBoxLayout(outputMethodGroup_);

    plcEnabledCheck_ = new QCheckBox(tr("PLC输出"));
    tcpEnabledCheck_ = new QCheckBox(tr("TCP输出"));
    serialEnabledCheck_ = new QCheckBox(tr("串口输出"));

    connect(plcEnabledCheck_, &QCheckBox::toggled, this, &AlignmentOutputDialog::onOutputMethodChanged);
    connect(tcpEnabledCheck_, &QCheckBox::toggled, this, &AlignmentOutputDialog::onOutputMethodChanged);
    connect(serialEnabledCheck_, &QCheckBox::toggled, this, &AlignmentOutputDialog::onOutputMethodChanged);

    layout->addWidget(plcEnabledCheck_);
    layout->addWidget(tcpEnabledCheck_);
    layout->addWidget(serialEnabledCheck_);
    layout->addStretch();

    auto* hintLabel = new QLabel(tr("(可同时启用多种输出方式)"));
    hintLabel->setStyleSheet("color: gray;");
    layout->addWidget(hintLabel);
}

void AlignmentOutputDialog::createPLCTab()
{
    plcTab_ = new QWidget();
    auto* layout = new QVBoxLayout(plcTab_);

    // 连接设置
    auto* connGroup = new QGroupBox(tr("连接设置"));
    auto* connLayout = new QGridLayout(connGroup);

    connLayout->addWidget(new QLabel(tr("协议类型:")), 0, 0);
    plcProtocolCombo_ = new QComboBox();
    plcProtocolCombo_->addItem("Modbus TCP", "Modbus");
    plcProtocolCombo_->addItem(tr("三菱MC协议"), "MC");
    plcProtocolCombo_->addItem(tr("基恩士PC-LINK"), "Keyence");
    plcProtocolCombo_->addItem(tr("欧姆龙FINS"), "Omron");
    connect(plcProtocolCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &AlignmentOutputDialog::onPLCProtocolChanged);
    connLayout->addWidget(plcProtocolCombo_, 0, 1);

    connLayout->addWidget(new QLabel(tr("IP地址:")), 1, 0);
    plcIPEdit_ = new QLineEdit("192.168.1.1");
    connLayout->addWidget(plcIPEdit_, 1, 1);

    connLayout->addWidget(new QLabel(tr("端口:")), 2, 0);
    plcPortSpin_ = new QSpinBox();
    plcPortSpin_->setRange(1, 65535);
    plcPortSpin_->setValue(502);
    connLayout->addWidget(plcPortSpin_, 2, 1);

    layout->addWidget(connGroup);

    // 寄存器映射
    auto* regGroup = new QGroupBox(tr("寄存器映射"));
    auto* regLayout = new QGridLayout(regGroup);

    regLayout->addWidget(new QLabel(tr("X偏差寄存器:")), 0, 0);
    regOffsetXSpin_ = new QSpinBox();
    regOffsetXSpin_->setRange(0, 65535);
    regOffsetXSpin_->setValue(0);
    regLayout->addWidget(regOffsetXSpin_, 0, 1);

    regLayout->addWidget(new QLabel(tr("Y偏差寄存器:")), 0, 2);
    regOffsetYSpin_ = new QSpinBox();
    regOffsetYSpin_->setRange(0, 65535);
    regOffsetYSpin_->setValue(2);
    regLayout->addWidget(regOffsetYSpin_, 0, 3);

    regLayout->addWidget(new QLabel(tr("角度偏差寄存器:")), 1, 0);
    regOffsetThetaSpin_ = new QSpinBox();
    regOffsetThetaSpin_->setRange(0, 65535);
    regOffsetThetaSpin_->setValue(4);
    regLayout->addWidget(regOffsetThetaSpin_, 1, 1);

    regLayout->addWidget(new QLabel(tr("结果寄存器:")), 1, 2);
    regResultSpin_ = new QSpinBox();
    regResultSpin_->setRange(0, 65535);
    regResultSpin_->setValue(6);
    regLayout->addWidget(regResultSpin_, 1, 3);

    regLayout->addWidget(new QLabel(tr("触发完成寄存器:")), 2, 0);
    regTriggerSpin_ = new QSpinBox();
    regTriggerSpin_->setRange(0, 65535);
    regTriggerSpin_->setValue(7);
    regLayout->addWidget(regTriggerSpin_, 2, 1);

    layout->addWidget(regGroup);

    // 数据格式
    auto* formatGroup = new QGroupBox(tr("数据格式"));
    auto* formatLayout = new QGridLayout(formatGroup);

    formatLayout->addWidget(new QLabel(tr("缩放因子:")), 0, 0);
    plcScaleFactorSpin_ = new QDoubleSpinBox();
    plcScaleFactorSpin_->setRange(0.001, 1000000);
    plcScaleFactorSpin_->setValue(1000);
    plcScaleFactorSpin_->setDecimals(3);
    formatLayout->addWidget(plcScaleFactorSpin_, 0, 1);

    formatLayout->addWidget(new QLabel(tr("(例: 1000表示mm转μm)")), 0, 2);

    plcSignedCheck_ = new QCheckBox(tr("带符号数"));
    plcSignedCheck_->setChecked(true);
    formatLayout->addWidget(plcSignedCheck_, 1, 0, 1, 2);

    formatLayout->addWidget(new QLabel(tr("数据类型:")), 2, 0);
    plcDataTypeCombo_ = new QComboBox();
    plcDataTypeCombo_->addItem("Int16 (16位整数)", 0);
    plcDataTypeCombo_->addItem("Int32 (32位整数)", 1);
    plcDataTypeCombo_->addItem("Float (32位浮点)", 2);
    formatLayout->addWidget(plcDataTypeCombo_, 2, 1);

    layout->addWidget(formatGroup);

    // 连接操作
    auto* operLayout = new QHBoxLayout();
    plcConnectBtn_ = new QPushButton(tr("连接"));
    plcDisconnectBtn_ = new QPushButton(tr("断开"));
    plcTestBtn_ = new QPushButton(tr("测试输出"));
    plcStatusLabel_ = new QLabel(tr("未连接"));
    plcStatusLabel_->setStyleSheet("color: gray;");

    plcDisconnectBtn_->setEnabled(false);

    connect(plcConnectBtn_, &QPushButton::clicked, this, &AlignmentOutputDialog::onConnectPLC);
    connect(plcDisconnectBtn_, &QPushButton::clicked, this, &AlignmentOutputDialog::onDisconnectPLC);
    connect(plcTestBtn_, &QPushButton::clicked, this, &AlignmentOutputDialog::onTestPLCOutput);

    operLayout->addWidget(plcConnectBtn_);
    operLayout->addWidget(plcDisconnectBtn_);
    operLayout->addWidget(plcTestBtn_);
    operLayout->addStretch();
    operLayout->addWidget(plcStatusLabel_);

    layout->addLayout(operLayout);
    layout->addStretch();
}

void AlignmentOutputDialog::createTCPTab()
{
    tcpTab_ = new QWidget();
    auto* layout = new QVBoxLayout(tcpTab_);

    // 连接设置
    auto* connGroup = new QGroupBox(tr("连接设置"));
    auto* connLayout = new QGridLayout(connGroup);

    connLayout->addWidget(new QLabel(tr("服务器地址:")), 0, 0);
    tcpServerEdit_ = new QLineEdit("127.0.0.1");
    connLayout->addWidget(tcpServerEdit_, 0, 1);

    connLayout->addWidget(new QLabel(tr("端口:")), 0, 2);
    tcpPortSpin_ = new QSpinBox();
    tcpPortSpin_->setRange(1, 65535);
    tcpPortSpin_->setValue(5000);
    connLayout->addWidget(tcpPortSpin_, 0, 3);

    connLayout->addWidget(new QLabel(tr("超时(ms):")), 1, 0);
    tcpTimeoutSpin_ = new QSpinBox();
    tcpTimeoutSpin_->setRange(100, 30000);
    tcpTimeoutSpin_->setValue(1000);
    connLayout->addWidget(tcpTimeoutSpin_, 1, 1);

    layout->addWidget(connGroup);

    // 数据格式
    auto* formatGroup = new QGroupBox(tr("数据格式"));
    auto* formatLayout = new QGridLayout(formatGroup);

    formatLayout->addWidget(new QLabel(tr("格式:")), 0, 0);
    tcpFormatCombo_ = new QComboBox();
    tcpFormatCombo_->addItem("JSON", "JSON");
    tcpFormatCombo_->addItem("CSV", "CSV");
    tcpFormatCombo_->addItem(tr("二进制"), "Binary");
    tcpFormatCombo_->addItem(tr("自定义"), "Custom");
    connect(tcpFormatCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &AlignmentOutputDialog::onTCPFormatChanged);
    formatLayout->addWidget(tcpFormatCombo_, 0, 1);

    formatLayout->addWidget(new QLabel(tr("自定义格式:")), 1, 0);
    tcpCustomFormatEdit_ = new QLineEdit("{X},{Y},{Theta}");
    tcpCustomFormatEdit_->setEnabled(false);
    tcpCustomFormatEdit_->setToolTip(tr("可用变量: {X}, {Y}, {Theta}, {Result}, {Time}"));
    formatLayout->addWidget(tcpCustomFormatEdit_, 1, 1, 1, 3);

    formatLayout->addWidget(new QLabel(tr("分隔符:")), 2, 0);
    tcpDelimiterEdit_ = new QLineEdit(",");
    formatLayout->addWidget(tcpDelimiterEdit_, 2, 1);

    formatLayout->addWidget(new QLabel(tr("结束标记:")), 2, 2);
    tcpEndMarkerEdit_ = new QLineEdit("\\r\\n");
    formatLayout->addWidget(tcpEndMarkerEdit_, 2, 3);

    tcpChecksumCheck_ = new QCheckBox(tr("添加校验和"));
    formatLayout->addWidget(tcpChecksumCheck_, 3, 0, 1, 2);

    layout->addWidget(formatGroup);

    // 预览
    auto* previewGroup = new QGroupBox(tr("输出预览"));
    auto* previewLayout = new QVBoxLayout(previewGroup);

    tcpPreviewEdit_ = new QTextEdit();
    tcpPreviewEdit_->setReadOnly(true);
    tcpPreviewEdit_->setMaximumHeight(80);
    tcpPreviewEdit_->setPlaceholderText(tr("输出数据预览..."));
    previewLayout->addWidget(tcpPreviewEdit_);

    layout->addWidget(previewGroup);

    // 连接操作
    auto* operLayout = new QHBoxLayout();
    tcpConnectBtn_ = new QPushButton(tr("连接"));
    tcpDisconnectBtn_ = new QPushButton(tr("断开"));
    tcpTestBtn_ = new QPushButton(tr("测试输出"));
    tcpStatusLabel_ = new QLabel(tr("未连接"));
    tcpStatusLabel_->setStyleSheet("color: gray;");

    tcpDisconnectBtn_->setEnabled(false);

    connect(tcpConnectBtn_, &QPushButton::clicked, this, &AlignmentOutputDialog::onConnectTCP);
    connect(tcpDisconnectBtn_, &QPushButton::clicked, this, &AlignmentOutputDialog::onDisconnectTCP);
    connect(tcpTestBtn_, &QPushButton::clicked, this, &AlignmentOutputDialog::onTestTCPOutput);

    operLayout->addWidget(tcpConnectBtn_);
    operLayout->addWidget(tcpDisconnectBtn_);
    operLayout->addWidget(tcpTestBtn_);
    operLayout->addStretch();
    operLayout->addWidget(tcpStatusLabel_);

    layout->addLayout(operLayout);
    layout->addStretch();
}

void AlignmentOutputDialog::createSerialTab()
{
    serialTab_ = new QWidget();
    auto* layout = new QVBoxLayout(serialTab_);

    // 串口设置
    auto* portGroup = new QGroupBox(tr("串口设置"));
    auto* portLayout = new QGridLayout(portGroup);

    portLayout->addWidget(new QLabel(tr("串口:")), 0, 0);
    serialPortCombo_ = new QComboBox();
    portLayout->addWidget(serialPortCombo_, 0, 1);

    serialRefreshBtn_ = new QPushButton(tr("刷新"));
    connect(serialRefreshBtn_, &QPushButton::clicked, this, &AlignmentOutputDialog::onRefreshSerialPorts);
    portLayout->addWidget(serialRefreshBtn_, 0, 2);

    portLayout->addWidget(new QLabel(tr("波特率:")), 1, 0);
    serialBaudRateCombo_ = new QComboBox();
    serialBaudRateCombo_->addItems({"9600", "19200", "38400", "57600", "115200"});
    serialBaudRateCombo_->setCurrentText("9600");
    portLayout->addWidget(serialBaudRateCombo_, 1, 1);

    portLayout->addWidget(new QLabel(tr("数据位:")), 2, 0);
    serialDataBitsCombo_ = new QComboBox();
    serialDataBitsCombo_->addItems({"5", "6", "7", "8"});
    serialDataBitsCombo_->setCurrentText("8");
    portLayout->addWidget(serialDataBitsCombo_, 2, 1);

    portLayout->addWidget(new QLabel(tr("停止位:")), 2, 2);
    serialStopBitsCombo_ = new QComboBox();
    serialStopBitsCombo_->addItems({"1", "1.5", "2"});
    serialStopBitsCombo_->setCurrentText("1");
    portLayout->addWidget(serialStopBitsCombo_, 2, 3);

    portLayout->addWidget(new QLabel(tr("校验:")), 3, 0);
    serialParityCombo_ = new QComboBox();
    serialParityCombo_->addItem(tr("无"), "None");
    serialParityCombo_->addItem(tr("偶校验"), "Even");
    serialParityCombo_->addItem(tr("奇校验"), "Odd");
    portLayout->addWidget(serialParityCombo_, 3, 1);

    layout->addWidget(portGroup);

    // 数据格式
    auto* formatGroup = new QGroupBox(tr("数据格式"));
    auto* formatLayout = new QGridLayout(formatGroup);

    formatLayout->addWidget(new QLabel(tr("格式:")), 0, 0);
    serialFormatCombo_ = new QComboBox();
    serialFormatCombo_->addItem("CSV", "CSV");
    serialFormatCombo_->addItem(tr("自定义"), "Custom");
    formatLayout->addWidget(serialFormatCombo_, 0, 1);

    formatLayout->addWidget(new QLabel(tr("自定义格式:")), 1, 0);
    serialCustomFormatEdit_ = new QLineEdit("X:{X},Y:{Y},T:{Theta}");
    formatLayout->addWidget(serialCustomFormatEdit_, 1, 1, 1, 3);

    formatLayout->addWidget(new QLabel(tr("结束标记:")), 2, 0);
    serialEndMarkerEdit_ = new QLineEdit("\\r\\n");
    formatLayout->addWidget(serialEndMarkerEdit_, 2, 1);

    formatLayout->addWidget(new QLabel(tr("超时(ms):")), 2, 2);
    serialTimeoutSpin_ = new QSpinBox();
    serialTimeoutSpin_->setRange(100, 30000);
    serialTimeoutSpin_->setValue(1000);
    formatLayout->addWidget(serialTimeoutSpin_, 2, 3);

    layout->addWidget(formatGroup);

    // 连接操作
    auto* operLayout = new QHBoxLayout();
    serialOpenBtn_ = new QPushButton(tr("打开"));
    serialCloseBtn_ = new QPushButton(tr("关闭"));
    serialTestBtn_ = new QPushButton(tr("测试输出"));
    serialStatusLabel_ = new QLabel(tr("未打开"));
    serialStatusLabel_->setStyleSheet("color: gray;");

    serialCloseBtn_->setEnabled(false);

    connect(serialOpenBtn_, &QPushButton::clicked, this, &AlignmentOutputDialog::onOpenSerial);
    connect(serialCloseBtn_, &QPushButton::clicked, this, &AlignmentOutputDialog::onCloseSerial);
    connect(serialTestBtn_, &QPushButton::clicked, this, &AlignmentOutputDialog::onTestSerialOutput);

    operLayout->addWidget(serialOpenBtn_);
    operLayout->addWidget(serialCloseBtn_);
    operLayout->addWidget(serialTestBtn_);
    operLayout->addStretch();
    operLayout->addWidget(serialStatusLabel_);

    layout->addLayout(operLayout);
    layout->addStretch();

    // 初始化串口列表
    onRefreshSerialPorts();
}

void AlignmentOutputDialog::createButtonGroup()
{
    applyBtn_ = new QPushButton(tr("应用"));
    okBtn_ = new QPushButton(tr("确定"));
    cancelBtn_ = new QPushButton(tr("取消"));

    connect(applyBtn_, &QPushButton::clicked, this, &AlignmentOutputDialog::onApply);
    connect(okBtn_, &QPushButton::clicked, [this]() {
        onApply();
        accept();
    });
    connect(cancelBtn_, &QPushButton::clicked, this, &QDialog::reject);
}

void AlignmentOutputDialog::updateFromTool()
{
    if (!outputTool_) return;

    // 输出方式
    Algorithm::OutputMethod method = outputTool_->getOutputMethod();
    plcEnabledCheck_->setChecked(Algorithm::hasFlag(method, Algorithm::OutputMethod::PLC));
    tcpEnabledCheck_->setChecked(Algorithm::hasFlag(method, Algorithm::OutputMethod::TCP));
    serialEnabledCheck_->setChecked(Algorithm::hasFlag(method, Algorithm::OutputMethod::Serial));

    // PLC配置
    Algorithm::PLCOutputConfig plcConfig = outputTool_->getPLCConfig();
    int protocolIndex = plcProtocolCombo_->findData(plcConfig.protocolType);
    if (protocolIndex >= 0) plcProtocolCombo_->setCurrentIndex(protocolIndex);
    plcIPEdit_->setText(plcConfig.ipAddress);
    plcPortSpin_->setValue(plcConfig.port);
    regOffsetXSpin_->setValue(plcConfig.offsetXRegister);
    regOffsetYSpin_->setValue(plcConfig.offsetYRegister);
    regOffsetThetaSpin_->setValue(plcConfig.offsetThetaRegister);
    regResultSpin_->setValue(plcConfig.resultRegister);
    regTriggerSpin_->setValue(plcConfig.triggerRegister);
    plcScaleFactorSpin_->setValue(plcConfig.scaleFactor);
    plcSignedCheck_->setChecked(plcConfig.signedValue);
    plcDataTypeCombo_->setCurrentIndex(plcConfig.dataType);

    // TCP配置
    Algorithm::TCPOutputConfig tcpConfig = outputTool_->getTCPConfig();
    tcpServerEdit_->setText(tcpConfig.serverAddress);
    tcpPortSpin_->setValue(tcpConfig.port);
    int formatIndex = tcpFormatCombo_->findData(tcpConfig.dataFormat);
    if (formatIndex >= 0) tcpFormatCombo_->setCurrentIndex(formatIndex);
    tcpCustomFormatEdit_->setText(tcpConfig.customFormat);
    tcpDelimiterEdit_->setText(tcpConfig.delimiter);
    tcpChecksumCheck_->setChecked(tcpConfig.addChecksum);
    tcpEndMarkerEdit_->setText(tcpConfig.endMarker);
    tcpTimeoutSpin_->setValue(tcpConfig.timeoutMs);

    // 串口配置
    Algorithm::SerialOutputConfig serialConfig = outputTool_->getSerialConfig();
    int portIndex = serialPortCombo_->findText(serialConfig.portName);
    if (portIndex >= 0) serialPortCombo_->setCurrentIndex(portIndex);
    serialBaudRateCombo_->setCurrentText(QString::number(serialConfig.baudRate));
    serialDataBitsCombo_->setCurrentText(QString::number(serialConfig.dataBits));
    serialStopBitsCombo_->setCurrentText(QString::number(serialConfig.stopBits));
    int parityIndex = serialParityCombo_->findData(serialConfig.parity);
    if (parityIndex >= 0) serialParityCombo_->setCurrentIndex(parityIndex);
    serialCustomFormatEdit_->setText(serialConfig.customFormat);
    serialEndMarkerEdit_->setText(serialConfig.endMarker);
    serialTimeoutSpin_->setValue(serialConfig.timeoutMs);

    updateConnectionStatus();
}

void AlignmentOutputDialog::applyToTool()
{
    if (!outputTool_) return;

    // 输出方式
    Algorithm::OutputMethod method = Algorithm::OutputMethod::None;
    if (plcEnabledCheck_->isChecked())
        method = method | Algorithm::OutputMethod::PLC;
    if (tcpEnabledCheck_->isChecked())
        method = method | Algorithm::OutputMethod::TCP;
    if (serialEnabledCheck_->isChecked())
        method = method | Algorithm::OutputMethod::Serial;
    outputTool_->setOutputMethod(method);

    // PLC配置
    Algorithm::PLCOutputConfig plcConfig;
    plcConfig.protocolType = plcProtocolCombo_->currentData().toString();
    plcConfig.ipAddress = plcIPEdit_->text();
    plcConfig.port = plcPortSpin_->value();
    plcConfig.offsetXRegister = regOffsetXSpin_->value();
    plcConfig.offsetYRegister = regOffsetYSpin_->value();
    plcConfig.offsetThetaRegister = regOffsetThetaSpin_->value();
    plcConfig.resultRegister = regResultSpin_->value();
    plcConfig.triggerRegister = regTriggerSpin_->value();
    plcConfig.scaleFactor = plcScaleFactorSpin_->value();
    plcConfig.signedValue = plcSignedCheck_->isChecked();
    plcConfig.dataType = plcDataTypeCombo_->currentIndex();
    outputTool_->setPLCConfig(plcConfig);

    // TCP配置
    Algorithm::TCPOutputConfig tcpConfig;
    tcpConfig.serverAddress = tcpServerEdit_->text();
    tcpConfig.port = tcpPortSpin_->value();
    tcpConfig.dataFormat = tcpFormatCombo_->currentData().toString();
    tcpConfig.customFormat = tcpCustomFormatEdit_->text();
    tcpConfig.delimiter = tcpDelimiterEdit_->text();
    tcpConfig.addChecksum = tcpChecksumCheck_->isChecked();
    tcpConfig.endMarker = tcpEndMarkerEdit_->text();
    tcpConfig.timeoutMs = tcpTimeoutSpin_->value();
    outputTool_->setTCPConfig(tcpConfig);

    // 串口配置
    Algorithm::SerialOutputConfig serialConfig;
    serialConfig.portName = serialPortCombo_->currentText();
    serialConfig.baudRate = serialBaudRateCombo_->currentText().toInt();
    serialConfig.dataBits = serialDataBitsCombo_->currentText().toInt();
    serialConfig.stopBits = serialStopBitsCombo_->currentText().toInt();
    serialConfig.parity = serialParityCombo_->currentData().toString();
    serialConfig.customFormat = serialCustomFormatEdit_->text();
    serialConfig.endMarker = serialEndMarkerEdit_->text();
    serialConfig.timeoutMs = serialTimeoutSpin_->value();
    outputTool_->setSerialConfig(serialConfig);

    LOG_INFO("对位输出配置已更新");
}

void AlignmentOutputDialog::updateConnectionStatus()
{
    if (!outputTool_) return;

    // PLC状态
    if (outputTool_->isPLCConnected()) {
        plcStatusLabel_->setText(tr("已连接"));
        plcStatusLabel_->setStyleSheet("color: green;");
        plcConnectBtn_->setEnabled(false);
        plcDisconnectBtn_->setEnabled(true);
    } else {
        plcStatusLabel_->setText(tr("未连接"));
        plcStatusLabel_->setStyleSheet("color: gray;");
        plcConnectBtn_->setEnabled(true);
        plcDisconnectBtn_->setEnabled(false);
    }

    // TCP状态
    if (outputTool_->isTCPConnected()) {
        tcpStatusLabel_->setText(tr("已连接"));
        tcpStatusLabel_->setStyleSheet("color: green;");
        tcpConnectBtn_->setEnabled(false);
        tcpDisconnectBtn_->setEnabled(true);
    } else {
        tcpStatusLabel_->setText(tr("未连接"));
        tcpStatusLabel_->setStyleSheet("color: gray;");
        tcpConnectBtn_->setEnabled(true);
        tcpDisconnectBtn_->setEnabled(false);
    }

    // 串口状态
    if (outputTool_->isSerialOpen()) {
        serialStatusLabel_->setText(tr("已打开"));
        serialStatusLabel_->setStyleSheet("color: green;");
        serialOpenBtn_->setEnabled(false);
        serialCloseBtn_->setEnabled(true);
    } else {
        serialStatusLabel_->setText(tr("未打开"));
        serialStatusLabel_->setStyleSheet("color: gray;");
        serialOpenBtn_->setEnabled(true);
        serialCloseBtn_->setEnabled(false);
    }
}

// ========== PLC槽函数 ==========

void AlignmentOutputDialog::onPLCProtocolChanged(int index)
{
    QString protocol = plcProtocolCombo_->itemData(index).toString();

    // 根据协议设置默认端口
    if (protocol == "Modbus") {
        plcPortSpin_->setValue(502);
    } else if (protocol == "MC") {
        plcPortSpin_->setValue(5000);
    } else if (protocol == "Keyence") {
        plcPortSpin_->setValue(8501);
    } else if (protocol == "Omron") {
        plcPortSpin_->setValue(9600);
    }
}

void AlignmentOutputDialog::onConnectPLC()
{
    if (!outputTool_) return;

    applyToTool();

    if (outputTool_->connectPLC()) {
        updateConnectionStatus();
        LOG_INFO("PLC连接成功");
    } else {
        QMessageBox::warning(this, tr("错误"), tr("PLC连接失败"));
    }
}

void AlignmentOutputDialog::onDisconnectPLC()
{
    if (!outputTool_) return;

    outputTool_->disconnectPLC();
    updateConnectionStatus();
    LOG_INFO("PLC已断开");
}

void AlignmentOutputDialog::onTestPLCOutput()
{
    if (!outputTool_) return;

    applyToTool();

    // 设置测试数据
    outputTool_->setOffset(1.234, -0.567, 0.123);
    outputTool_->setJudgeResult(true);

    if (outputTool_->outputToPLC()) {
        QMessageBox::information(this, tr("成功"), tr("PLC输出测试成功"));
    } else {
        QMessageBox::warning(this, tr("失败"), tr("PLC输出测试失败"));
    }
}

// ========== TCP槽函数 ==========

void AlignmentOutputDialog::onTCPFormatChanged(int index)
{
    QString format = tcpFormatCombo_->itemData(index).toString();
    tcpCustomFormatEdit_->setEnabled(format == "Custom");

    // 更新预览
    QString preview;
    if (format == "JSON") {
        preview = "{\"x\": 1.234, \"y\": -0.567, \"theta\": 0.123, \"result\": true}";
    } else if (format == "CSV") {
        preview = "1.234,-0.567,0.123,1";
    } else if (format == "Binary") {
        preview = tr("[二进制数据: 16字节]");
    } else {
        preview = tcpCustomFormatEdit_->text()
            .replace("{X}", "1.234")
            .replace("{Y}", "-0.567")
            .replace("{Theta}", "0.123")
            .replace("{Result}", "1");
    }
    tcpPreviewEdit_->setText(preview);
}

void AlignmentOutputDialog::onConnectTCP()
{
    if (!outputTool_) return;

    applyToTool();

    if (outputTool_->connectTCP()) {
        updateConnectionStatus();
        LOG_INFO("TCP连接成功");
    } else {
        QMessageBox::warning(this, tr("错误"), tr("TCP连接失败"));
    }
}

void AlignmentOutputDialog::onDisconnectTCP()
{
    if (!outputTool_) return;

    outputTool_->disconnectTCP();
    updateConnectionStatus();
    LOG_INFO("TCP已断开");
}

void AlignmentOutputDialog::onTestTCPOutput()
{
    if (!outputTool_) return;

    applyToTool();

    outputTool_->setOffset(1.234, -0.567, 0.123);
    outputTool_->setJudgeResult(true);

    if (outputTool_->outputToTCP()) {
        QMessageBox::information(this, tr("成功"), tr("TCP输出测试成功"));
    } else {
        QMessageBox::warning(this, tr("失败"), tr("TCP输出测试失败"));
    }
}

// ========== 串口槽函数 ==========

void AlignmentOutputDialog::onRefreshSerialPorts()
{
    serialPortCombo_->clear();

    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
    for (const auto& port : ports) {
        serialPortCombo_->addItem(
            QString("%1 (%2)").arg(port.portName()).arg(port.description()),
            port.portName());
    }

    if (ports.isEmpty()) {
        serialPortCombo_->addItem(tr("(无可用串口)"));
    }
}

void AlignmentOutputDialog::onOpenSerial()
{
    if (!outputTool_) return;

    applyToTool();

    if (outputTool_->openSerial()) {
        updateConnectionStatus();
        LOG_INFO("串口已打开");
    } else {
        QMessageBox::warning(this, tr("错误"), tr("打开串口失败"));
    }
}

void AlignmentOutputDialog::onCloseSerial()
{
    if (!outputTool_) return;

    outputTool_->closeSerial();
    updateConnectionStatus();
    LOG_INFO("串口已关闭");
}

void AlignmentOutputDialog::onTestSerialOutput()
{
    if (!outputTool_) return;

    applyToTool();

    outputTool_->setOffset(1.234, -0.567, 0.123);
    outputTool_->setJudgeResult(true);

    if (outputTool_->outputToSerial()) {
        QMessageBox::information(this, tr("成功"), tr("串口输出测试成功"));
    } else {
        QMessageBox::warning(this, tr("失败"), tr("串口输出测试失败"));
    }
}

void AlignmentOutputDialog::onOutputMethodChanged()
{
    // 可以根据选择启用/禁用相应的标签页
}

void AlignmentOutputDialog::onApply()
{
    applyToTool();
    emit configurationChanged();
}

} // namespace UI
} // namespace VisionForge
