/**
 * @file PLCConfigDialog.cpp
 * @brief PLC通信配置对话框实现
 * @author VisionForge Team
 * @date 2025-12-17
 */

#include "ui/PLCConfigDialog.h"
#include "comm/PLCManager.h"
#include "base/Logger.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFormLayout>
#include <QHeaderView>
#include <QMessageBox>

namespace VisionForge {

PLCConfigDialog::PLCConfigDialog(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle("通信配置");
    setMinimumSize(1400, 800);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    setupStyles();
    setupUI();
    setupConnections();

    // 初始化数据表格
    updateDataTable();
}

PLCConfigDialog::~PLCConfigDialog()
{
}

void PLCConfigDialog::setupStyles()
{
    // 使用系统原生风格，仅做轻微调整
    // GroupBox样式 - 简洁边框
    groupBoxStyle_ = R"(
        QGroupBox {
            font-weight: bold;
            border: 1px solid #c0c0c0;
            border-radius: 2px;
            margin-top: 12px;
            padding-top: 8px;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            subcontrol-position: top left;
            left: 8px;
            padding: 0 4px;
        }
    )";

    // 普通按钮样式 - 使用系统默认
    buttonStyle_ = R"(
        QPushButton {
            min-height: 22px;
            padding: 3px 12px;
        }
    )";

    // 大按钮样式（确定/取消）
    largeButtonStyle_ = R"(
        QPushButton {
            min-width: 70px;
            min-height: 60px;
            font-size: 13px;
            font-weight: bold;
        }
    )";
}

void PLCConfigDialog::setupUI()
{
    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(10, 10, 10, 10);

    // 主内容区域
    auto* contentLayout = new QHBoxLayout();
    contentLayout->setSpacing(10);

    // 左侧面板
    QWidget* leftPanel = createLeftPanel();
    leftPanel->setFixedWidth(380);
    contentLayout->addWidget(leftPanel);

    // 中间面板
    QWidget* middlePanel = createMiddlePanel();
    contentLayout->addWidget(middlePanel, 1);

    // 右侧面板
    QWidget* rightPanel = createRightPanel();
    rightPanel->setFixedWidth(280);
    contentLayout->addWidget(rightPanel);

    mainLayout->addLayout(contentLayout, 1);

    // 底部状态栏
    auto* bottomLayout = new QHBoxLayout();

    // 状态指示灯和文本
    auto* statusLayout = new QHBoxLayout();
    QLabel* statusIcon = new QLabel();
    statusIcon->setFixedSize(12, 12);
    statusIcon->setStyleSheet("background-color: #cc0000; border-radius: 6px; border: 1px solid #990000;");
    statusLayout->addWidget(statusIcon);

    statusLabel_ = new QLabel("网络连接打开失败");
    statusLayout->addWidget(statusLabel_);
    statusLayout->addStretch();

    bottomLayout->addLayout(statusLayout);
    bottomLayout->addStretch();

    // 确定/取消按钮（大图标样式）
    auto* btnLayout = new QVBoxLayout();
    btnLayout->setSpacing(10);

    confirmBtn_ = new QPushButton("确定");
    confirmBtn_->setStyleSheet(largeButtonStyle_);
    btnLayout->addWidget(confirmBtn_);

    cancelBtn_ = new QPushButton("取消");
    cancelBtn_->setStyleSheet(largeButtonStyle_);
    btnLayout->addWidget(cancelBtn_);

    bottomLayout->addLayout(btnLayout);

    mainLayout->addLayout(bottomLayout);
}

QWidget* PLCConfigDialog::createLeftPanel()
{
    auto* panel = new QWidget();
    auto* layout = new QVBoxLayout(panel);
    layout->setSpacing(10);
    layout->setContentsMargins(0, 0, 0, 0);

    // ===== 通信参数 =====
    auto* paramGroup = new QGroupBox("通信参数");
    paramGroup->setStyleSheet(groupBoxStyle_);
    auto* paramLayout = new QFormLayout(paramGroup);
    paramLayout->setSpacing(8);

    commTypeCombo_ = new QComboBox();
    commTypeCombo_->addItems({"网口通信", "串口通信"});
    paramLayout->addRow("通信类型：", commTypeCombo_);

    commProtocolCombo_ = new QComboBox();
    commProtocolCombo_->addItems({
        "三菱Q_网口ASCII协议",
        "三菱Q_网口二进制协议",
        "三菱FX_网口ASCII协议",
        "Modbus TCP",
        "欧姆龙FINS TCP",
        "西门子S7"
    });
    paramLayout->addRow("通信协议：", commProtocolCombo_);

    commDataCombo_ = new QComboBox();
    commDataCombo_->addItems({"单字", "双字"});
    commDataCombo_->setCurrentIndex(1);
    paramLayout->addRow("通信数据：", commDataCombo_);

    commStorageCombo_ = new QComboBox();
    commStorageCombo_->addItems({"D寄存器", "W寄存器", "R寄存器"});
    paramLayout->addRow("通信存储：", commStorageCombo_);

    layout->addWidget(paramGroup);

    // ===== 通信指令 =====
    auto* cmdGroup = new QGroupBox("通信指令");
    cmdGroup->setStyleSheet(groupBoxStyle_);
    auto* cmdLayout = new QGridLayout(cmdGroup);
    cmdLayout->setSpacing(8);

    cmdLayout->addWidget(new QLabel("PLC指令基地址："), 0, 0);
    plcCmdAddrEdit_ = new QLineEdit("20500");
    plcCmdAddrEdit_->setFixedWidth(80);
    cmdLayout->addWidget(plcCmdAddrEdit_, 0, 1);
    cmdLayout->addWidget(new QLabel("长度："), 0, 2);
    plcCmdLenSpin_ = new QSpinBox();
    plcCmdLenSpin_->setRange(1, 100);
    plcCmdLenSpin_->setValue(10);
    plcCmdLenSpin_->setFixedWidth(60);
    cmdLayout->addWidget(plcCmdLenSpin_, 0, 3);

    cmdLayout->addWidget(new QLabel("视觉指令基地址："), 1, 0);
    visionCmdAddrEdit_ = new QLineEdit("20550");
    visionCmdAddrEdit_->setFixedWidth(80);
    cmdLayout->addWidget(visionCmdAddrEdit_, 1, 1);

    layout->addWidget(cmdGroup);

    // ===== 通信数据 =====
    auto* dataGroup = new QGroupBox("通信数据");
    dataGroup->setStyleSheet(groupBoxStyle_);
    auto* dataLayout = new QGridLayout(dataGroup);
    dataLayout->setSpacing(8);

    dataLayout->addWidget(new QLabel("PLC数据基地址："), 0, 0);
    plcDataAddrEdit_ = new QLineEdit("20500");
    plcDataAddrEdit_->setFixedWidth(80);
    dataLayout->addWidget(plcDataAddrEdit_, 0, 1);
    dataLayout->addWidget(new QLabel("长度："), 0, 2);
    plcDataLenSpin_ = new QSpinBox();
    plcDataLenSpin_->setRange(1, 100);
    plcDataLenSpin_->setValue(10);
    plcDataLenSpin_->setFixedWidth(60);
    dataLayout->addWidget(plcDataLenSpin_, 0, 3);

    dataLayout->addWidget(new QLabel("视觉数据基地址："), 1, 0);
    visionDataAddrEdit_ = new QLineEdit("20580");
    visionDataAddrEdit_->setFixedWidth(80);
    dataLayout->addWidget(visionDataAddrEdit_, 1, 1);

    layout->addWidget(dataGroup);

    // ===== 通信设置 =====
    auto* settingsGroup = new QGroupBox("通信设置");
    settingsGroup->setStyleSheet(groupBoxStyle_);
    auto* settingsLayout = new QGridLayout(settingsGroup);
    settingsLayout->setSpacing(8);

    int row = 0;
    settingsLayout->addWidget(new QLabel("接收延时："), row, 0);
    recvDelaySpin_ = new QSpinBox();
    recvDelaySpin_->setRange(0, 10000);
    recvDelaySpin_->setValue(0);
    recvDelaySpin_->setFixedWidth(80);
    settingsLayout->addWidget(recvDelaySpin_, row, 1);
    settingsLayout->addWidget(new QLabel("ms"), row, 2);

    row++;
    settingsLayout->addWidget(new QLabel("发送延时："), row, 0);
    sendDelaySpin_ = new QSpinBox();
    sendDelaySpin_->setRange(0, 10000);
    sendDelaySpin_->setValue(0);
    sendDelaySpin_->setFixedWidth(80);
    settingsLayout->addWidget(sendDelaySpin_, row, 1);
    settingsLayout->addWidget(new QLabel("ms"), row, 2);

    row++;
    settingsLayout->addWidget(new QLabel("通信超时："), row, 0);
    commTimeoutSpin_ = new QSpinBox();
    commTimeoutSpin_->setRange(1, 60000);
    commTimeoutSpin_->setValue(30);
    commTimeoutSpin_->setFixedWidth(80);
    settingsLayout->addWidget(commTimeoutSpin_, row, 1);
    settingsLayout->addWidget(new QLabel("ms"), row, 2);

    row++;
    settingsLayout->addWidget(new QLabel("重发次数："), row, 0);
    retryCountSpin_ = new QSpinBox();
    retryCountSpin_->setRange(0, 10);
    retryCountSpin_->setValue(3);
    retryCountSpin_->setFixedWidth(80);
    settingsLayout->addWidget(retryCountSpin_, row, 1);

    row++;
    settingsLayout->addWidget(new QLabel("X单位幂："), row, 0);
    xUnitPowerSpin_ = new QSpinBox();
    xUnitPowerSpin_->setRange(-6, 6);
    xUnitPowerSpin_->setValue(1);
    xUnitPowerSpin_->setFixedWidth(80);
    settingsLayout->addWidget(xUnitPowerSpin_, row, 1);

    row++;
    settingsLayout->addWidget(new QLabel("Y单位幂："), row, 0);
    yUnitPowerSpin_ = new QSpinBox();
    yUnitPowerSpin_->setRange(-6, 6);
    yUnitPowerSpin_->setValue(3);
    yUnitPowerSpin_->setFixedWidth(80);
    settingsLayout->addWidget(yUnitPowerSpin_, row, 1);

    row++;
    settingsLayout->addWidget(new QLabel("θ单位幂："), row, 0);
    thetaUnitPowerSpin_ = new QSpinBox();
    thetaUnitPowerSpin_->setRange(-6, 6);
    thetaUnitPowerSpin_->setValue(1);
    thetaUnitPowerSpin_->setFixedWidth(80);
    settingsLayout->addWidget(thetaUnitPowerSpin_, row, 1);

    row++;
    settingsLayout->addWidget(new QLabel("数据长度类型："), row, 0);
    dataLenTypeCombo_ = new QComboBox();
    dataLenTypeCombo_->addItems({"实际长度", "固定长度"});
    settingsLayout->addWidget(dataLenTypeCombo_, row, 1, 1, 2);

    layout->addWidget(settingsGroup);

    layout->addStretch();

    return panel;
}

QWidget* PLCConfigDialog::createMiddlePanel()
{
    auto* panel = new QWidget();
    auto* layout = new QVBoxLayout(panel);
    layout->setSpacing(10);
    layout->setContentsMargins(0, 0, 0, 0);

    // ===== 网络设置 =====
    auto* networkGroup = new QGroupBox("网络设置");
    networkGroup->setStyleSheet(groupBoxStyle_);
    auto* networkLayout = new QGridLayout(networkGroup);
    networkLayout->setSpacing(8);

    int row = 0;
    networkLayout->addWidget(new QLabel("类型"), row, 0);
    serverRadio_ = new QRadioButton("服务端");
    clientRadio_ = new QRadioButton("客户端");
    clientRadio_->setChecked(true);
    modeGroup_ = new QButtonGroup(this);
    modeGroup_->addButton(serverRadio_, 0);
    modeGroup_->addButton(clientRadio_, 1);
    auto* modeLayout = new QHBoxLayout();
    modeLayout->addWidget(serverRadio_);
    modeLayout->addWidget(clientRadio_);
    modeLayout->addStretch();
    networkLayout->addLayout(modeLayout, row, 1, 1, 4);

    row++;
    networkLayout->addWidget(new QLabel("IP"), row, 0);
    auto* ipLayout = new QHBoxLayout();
    ipEdit1_ = new QLineEdit("192");
    ipEdit1_->setFixedWidth(45);
    ipEdit1_->setAlignment(Qt::AlignCenter);
    ipEdit2_ = new QLineEdit("168");
    ipEdit2_->setFixedWidth(45);
    ipEdit2_->setAlignment(Qt::AlignCenter);
    ipEdit3_ = new QLineEdit("20");
    ipEdit3_->setFixedWidth(45);
    ipEdit3_->setAlignment(Qt::AlignCenter);
    ipEdit4_ = new QLineEdit("1");
    ipEdit4_->setFixedWidth(45);
    ipEdit4_->setAlignment(Qt::AlignCenter);
    ipLayout->addWidget(ipEdit1_);
    ipLayout->addWidget(new QLabel("."));
    ipLayout->addWidget(ipEdit2_);
    ipLayout->addWidget(new QLabel("."));
    ipLayout->addWidget(ipEdit3_);
    ipLayout->addWidget(new QLabel("."));
    ipLayout->addWidget(ipEdit4_);
    ipLayout->addStretch();
    networkLayout->addLayout(ipLayout, row, 1, 1, 4);

    row++;
    networkLayout->addWidget(new QLabel("端口"), row, 0);
    portSpin_ = new QSpinBox();
    portSpin_->setRange(1, 65535);
    portSpin_->setValue(7110);
    portSpin_->setFixedWidth(100);
    networkLayout->addWidget(portSpin_, row, 1);

    row++;
    networkLayout->addWidget(new QLabel("命令终止符"), row, 0);
    cmdTermCombo_ = new QComboBox();
    cmdTermCombo_->addItems({"无", "CR", "LF", "CR+LF"});
    cmdTermCombo_->setFixedWidth(100);
    networkLayout->addWidget(cmdTermCombo_, row, 1);

    row++;
    networkLayout->addWidget(new QLabel("通讯终止符"), row, 0);
    commTermCombo_ = new QComboBox();
    commTermCombo_->addItems({"无", "CR", "LF", "CR+LF"});
    commTermCombo_->setFixedWidth(100);
    networkLayout->addWidget(commTermCombo_, row, 1);

    row++;
    auto* netBtnLayout = new QHBoxLayout();
    moreSettingsBtn_ = new QPushButton("更多设置");
    moreSettingsBtn_->setStyleSheet(buttonStyle_);
    netBtnLayout->addWidget(moreSettingsBtn_);
    netBtnLayout->addStretch();
    networkLayout->addLayout(netBtnLayout, row, 0, 1, 2);

    row++;
    openBtn_ = new QPushButton("打开");
    openBtn_->setStyleSheet(buttonStyle_);
    openBtn_->setFixedWidth(100);
    networkLayout->addWidget(openBtn_, row, 0, 1, 2, Qt::AlignCenter);

    layout->addWidget(networkGroup);

    // ===== 读写测试 =====
    auto* testGroup = new QGroupBox("读写测试");
    testGroup->setStyleSheet(groupBoxStyle_);
    auto* testLayout = new QVBoxLayout(testGroup);
    testLayout->setSpacing(8);

    // 测试控制行
    auto* testCtrlLayout = new QHBoxLayout();
    testCtrlLayout->addWidget(new QLabel("通信数据："));
    testDataTypeCombo_ = new QComboBox();
    testDataTypeCombo_->addItems({"单字", "双字"});
    testDataTypeCombo_->setFixedWidth(80);
    testCtrlLayout->addWidget(testDataTypeCombo_);
    testCtrlLayout->addWidget(new QLabel("数据地址："));
    testAddrEdit_ = new QLineEdit("100");
    testAddrEdit_->setFixedWidth(80);
    testCtrlLayout->addWidget(testAddrEdit_);
    testCtrlLayout->addStretch();
    readBtn_ = new QPushButton("读");
    readBtn_->setStyleSheet(buttonStyle_);
    readBtn_->setFixedWidth(60);
    testCtrlLayout->addWidget(readBtn_);
    writeBtn_ = new QPushButton("写");
    writeBtn_->setStyleSheet(buttonStyle_);
    writeBtn_->setFixedWidth(60);
    testCtrlLayout->addWidget(writeBtn_);
    testLayout->addLayout(testCtrlLayout);

    // 数据表格
    dataTable_ = new QTableWidget();
    dataTable_->setColumnCount(8);
    dataTable_->setHorizontalHeaderLabels({"地址", "值", "地址", "值", "地址", "值", "地址", "值"});
    dataTable_->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    dataTable_->verticalHeader()->setVisible(false);
    dataTable_->setAlternatingRowColors(true);
    testLayout->addWidget(dataTable_);

    layout->addWidget(testGroup, 1);

    return panel;
}

QWidget* PLCConfigDialog::createRightPanel()
{
    auto* panel = new QWidget();
    auto* layout = new QVBoxLayout(panel);
    layout->setSpacing(10);
    layout->setContentsMargins(0, 0, 0, 0);

    // 右上角图标按钮
    auto* iconLayout = new QHBoxLayout();
    iconLayout->addStretch();
    iconBtn_ = new QPushButton("边检");
    iconBtn_->setFixedSize(60, 60);
    iconBtn_->setToolTip("边检");
    iconLayout->addWidget(iconBtn_);
    layout->addLayout(iconLayout);

    // ===== 发送数据 =====
    auto* sendGroup = new QGroupBox("发送数据");
    sendGroup->setStyleSheet(groupBoxStyle_);
    auto* sendLayout = new QVBoxLayout(sendGroup);

    sendDataEdit_ = new QTextEdit();
    sendDataEdit_->setMaximumHeight(80);
    sendLayout->addWidget(sendDataEdit_);

    auto* sendBtnLayout = new QHBoxLayout();
    sendBtn_ = new QPushButton("发送");
    sendBtn_->setStyleSheet(buttonStyle_);
    clearSendBtn_ = new QPushButton("清空");
    clearSendBtn_->setStyleSheet(buttonStyle_);
    sendBtnLayout->addWidget(sendBtn_);
    sendBtnLayout->addWidget(clearSendBtn_);
    sendLayout->addLayout(sendBtnLayout);

    layout->addWidget(sendGroup);

    // ===== 接收数据 =====
    auto* recvGroup = new QGroupBox("接收数据");
    recvGroup->setStyleSheet(groupBoxStyle_);
    auto* recvLayout = new QVBoxLayout(recvGroup);

    recvDataEdit_ = new QTextEdit();
    recvDataEdit_->setReadOnly(true);
    recvDataEdit_->setMaximumHeight(100);
    recvLayout->addWidget(recvDataEdit_);

    auto* recvBtnLayout = new QHBoxLayout();
    recvBtnLayout->addStretch();
    clearRecvBtn_ = new QPushButton("清空");
    clearRecvBtn_->setStyleSheet(buttonStyle_);
    recvBtnLayout->addWidget(clearRecvBtn_);
    recvLayout->addLayout(recvBtnLayout);

    layout->addWidget(recvGroup);

    layout->addStretch();

    return panel;
}

void PLCConfigDialog::setupConnections()
{
    // 网络操作
    connect(openBtn_, &QPushButton::clicked, this, &PLCConfigDialog::onOpenConnection);
    connect(moreSettingsBtn_, &QPushButton::clicked, this, &PLCConfigDialog::onMoreSettings);

    // 数据操作
    connect(sendBtn_, &QPushButton::clicked, this, &PLCConfigDialog::onSendData);
    connect(clearSendBtn_, &QPushButton::clicked, this, &PLCConfigDialog::onClearSendData);
    connect(clearRecvBtn_, &QPushButton::clicked, this, &PLCConfigDialog::onClearRecvData);

    // 读写测试
    connect(readBtn_, &QPushButton::clicked, this, &PLCConfigDialog::onReadData);
    connect(writeBtn_, &QPushButton::clicked, this, &PLCConfigDialog::onWriteData);

    // 对话框按钮
    connect(confirmBtn_, &QPushButton::clicked, this, &PLCConfigDialog::onConfirm);
    connect(cancelBtn_, &QPushButton::clicked, this, &PLCConfigDialog::onCancel);
}

void PLCConfigDialog::updateDataTable()
{
    // 初始化数据表格 D100-D129
    int startAddr = testAddrEdit_->text().toInt();
    int rowCount = 10;
    dataTable_->setRowCount(rowCount);

    for (int row = 0; row < rowCount; ++row) {
        for (int col = 0; col < 4; ++col) {
            int addr = startAddr + row + col * rowCount;
            QString addrStr = QString("D%1").arg(addr);

            // 地址列
            auto* addrItem = new QTableWidgetItem(addrStr);
            addrItem->setFlags(addrItem->flags() & ~Qt::ItemIsEditable);
            addrItem->setTextAlignment(Qt::AlignCenter);
            dataTable_->setItem(row, col * 2, addrItem);

            // 值列
            auto* valueItem = new QTableWidgetItem("0");
            valueItem->setTextAlignment(Qt::AlignCenter);
            dataTable_->setItem(row, col * 2 + 1, valueItem);
        }
    }
}

void PLCConfigDialog::updateConnectionStatus(bool connected, const QString& message)
{
    if (connected) {
        statusLabel_->setText(message.isEmpty() ? "网络连接已打开" : message);
    } else {
        statusLabel_->setText(message.isEmpty() ? "网络连接打开失败" : message);
    }
}

void PLCConfigDialog::appendLog(const QString& message)
{
    recvDataEdit_->append(message);
}

void PLCConfigDialog::onOpenConnection()
{
    // 构建IP地址
    QString ip = QString("%1.%2.%3.%4")
        .arg(ipEdit1_->text())
        .arg(ipEdit2_->text())
        .arg(ipEdit3_->text())
        .arg(ipEdit4_->text());
    int port = portSpin_->value();

    LOG_INFO(QString("尝试连接: %1:%2").arg(ip).arg(port));

    // 创建连接配置
    Comm::PLCConfig config;
    config.name = "DefaultPLC";

    // 根据协议选择
    int protocolIndex = commProtocolCombo_->currentIndex();
    switch (protocolIndex) {
        case 0:  // 三菱Q_网口ASCII协议
        case 1:  // 三菱Q_网口二进制协议
        case 2:  // 三菱FX_网口ASCII协议
            config.protocol = Comm::PLCProtocol::MitsubishiMC_TCP;
            break;
        case 3:  // Modbus TCP
            config.protocol = Comm::PLCProtocol::ModbusTCP;
            break;
        case 4:  // 欧姆龙FINS TCP
            config.protocol = Comm::PLCProtocol::OmronFINS_TCP;
            break;
        case 5:  // 西门子S7
            config.protocol = Comm::PLCProtocol::SiemensS7;
            break;
        default:
            config.protocol = Comm::PLCProtocol::MitsubishiMC_TCP;
    }

    auto socketConfig = std::make_shared<Comm::SocketConfig>();
    socketConfig->ipAddress = ip;
    socketConfig->port = static_cast<quint16>(port);
    socketConfig->timeout = commTimeoutSpin_->value();
    socketConfig->retryCount = retryCountSpin_->value();
    config.commConfig = socketConfig;

    // 移除旧连接并添加新连接
    Comm::PLCManager::instance().removeConnection("DefaultPLC");
    if (Comm::PLCManager::instance().addConnection("DefaultPLC", config)) {
        if (Comm::PLCManager::instance().connectPLC("DefaultPLC")) {
            updateConnectionStatus(true);
            appendLog(QString("连接成功: %1:%2").arg(ip).arg(port));
        } else {
            updateConnectionStatus(false);
            appendLog(QString("连接失败: %1:%2").arg(ip).arg(port));
        }
    } else {
        updateConnectionStatus(false, "创建连接失败");
    }
}

void PLCConfigDialog::onMoreSettings()
{
    QMessageBox::information(this, "更多设置", "更多设置功能待实现");
}

void PLCConfigDialog::onSendData()
{
    QString data = sendDataEdit_->toPlainText();
    if (data.isEmpty()) {
        return;
    }

    appendLog(QString("发送: %1").arg(data));
    // TODO: 实际发送数据
}

void PLCConfigDialog::onClearSendData()
{
    sendDataEdit_->clear();
}

void PLCConfigDialog::onClearRecvData()
{
    recvDataEdit_->clear();
}

void PLCConfigDialog::onReadData()
{
    if (!Comm::PLCManager::instance().isConnected("DefaultPLC")) {
        QMessageBox::warning(this, "警告", "请先连接PLC");
        return;
    }

    int addr = testAddrEdit_->text().toInt();
    bool isDoubleWord = (testDataTypeCombo_->currentIndex() == 1);
    int count = isDoubleWord ? 20 : 40;  // 读取40个字或20个双字

    auto result = Comm::PLCManager::instance().readRegisters("DefaultPLC", addr, count);

    if (result.isSuccess()) {
        // 更新表格
        for (int row = 0; row < dataTable_->rowCount(); ++row) {
            for (int col = 0; col < 4; ++col) {
                int idx = row + col * 10;
                if (idx < static_cast<int>(result.intValues.size())) {
                    auto* item = dataTable_->item(row, col * 2 + 1);
                    if (item) {
                        item->setText(QString::number(result.intValues[idx]));
                    }
                }
            }
        }
        appendLog(QString("读取成功: %1个寄存器").arg(result.intValues.size()));
    } else {
        appendLog(QString("读取失败: %1").arg(result.errorMessage));
    }
}

void PLCConfigDialog::onWriteData()
{
    if (!Comm::PLCManager::instance().isConnected("DefaultPLC")) {
        QMessageBox::warning(this, "警告", "请先连接PLC");
        return;
    }

    int addr = testAddrEdit_->text().toInt();

    // 从表格收集数据
    std::vector<int> values;
    for (int row = 0; row < dataTable_->rowCount(); ++row) {
        for (int col = 0; col < 4; ++col) {
            auto* item = dataTable_->item(row, col * 2 + 1);
            if (item) {
                values.push_back(item->text().toInt());
            }
        }
    }

    auto result = Comm::PLCManager::instance().writeRegisters("DefaultPLC", addr, values);

    if (result.isSuccess()) {
        appendLog(QString("写入成功: %1个寄存器").arg(values.size()));
    } else {
        appendLog(QString("写入失败: %1").arg(result.errorMessage));
    }
}

void PLCConfigDialog::onConfirm()
{
    accept();
}

void PLCConfigDialog::onCancel()
{
    reject();
}

} // namespace VisionForge
