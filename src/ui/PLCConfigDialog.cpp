/**
 * @file PLCConfigDialog.cpp
 * @brief PLC配置对话框实现
 * @author VisionForge Team
 * @date 2025-12-17
 */

#include "ui/PLCConfigDialog.h"
#include "comm/PLCManager.h"
#include "base/Logger.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QSplitter>
#include <QMessageBox>
#include <QInputDialog>
#include <QFileDialog>
#include <QDateTime>
#include <QHeaderView>

namespace VisionForge {

PLCConfigDialog::PLCConfigDialog(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle("PLC通信配置");
    setMinimumSize(900, 600);

    setupUI();
    setupConnections();
    updateConnectionList();
}

PLCConfigDialog::~PLCConfigDialog()
{
}

void PLCConfigDialog::setupUI()
{
    auto* mainLayout = new QHBoxLayout(this);

    // 左侧：连接列表
    QWidget* listPanel = createConnectionListPanel();
    listPanel->setFixedWidth(200);

    // 中间：配置面板
    QWidget* configPanel = createConfigPanel();

    // 右侧：测试面板
    QWidget* testPanel = createTestPanel();

    // 使用分割器
    auto* splitter = new QSplitter(Qt::Horizontal);
    splitter->addWidget(listPanel);
    splitter->addWidget(configPanel);
    splitter->addWidget(testPanel);
    splitter->setStretchFactor(0, 0);
    splitter->setStretchFactor(1, 1);
    splitter->setStretchFactor(2, 1);

    mainLayout->addWidget(splitter);
}

QWidget* PLCConfigDialog::createConnectionListPanel()
{
    auto* panel = new QWidget();
    auto* layout = new QVBoxLayout(panel);

    // 连接列表
    connectionList_ = new QListWidget();
    layout->addWidget(new QLabel("PLC连接列表:"));
    layout->addWidget(connectionList_);

    // 按钮
    auto* btnLayout = new QHBoxLayout();
    addBtn_ = new QPushButton("添加");
    removeBtn_ = new QPushButton("删除");
    btnLayout->addWidget(addBtn_);
    btnLayout->addWidget(removeBtn_);
    layout->addLayout(btnLayout);

    // 保存/加载按钮
    auto* fileLayout = new QHBoxLayout();
    saveConfigBtn_ = new QPushButton("保存配置");
    loadConfigBtn_ = new QPushButton("加载配置");
    fileLayout->addWidget(saveConfigBtn_);
    fileLayout->addWidget(loadConfigBtn_);
    layout->addLayout(fileLayout);

    return panel;
}

QWidget* PLCConfigDialog::createConfigPanel()
{
    auto* panel = new QWidget();
    auto* layout = new QVBoxLayout(panel);

    // 基本配置
    auto* basicGroup = new QGroupBox("基本配置");
    auto* basicLayout = new QGridLayout(basicGroup);

    basicLayout->addWidget(new QLabel("连接名称:"), 0, 0);
    nameEdit_ = new QLineEdit();
    basicLayout->addWidget(nameEdit_, 0, 1);

    basicLayout->addWidget(new QLabel("通信协议:"), 1, 0);
    protocolCombo_ = new QComboBox();
    protocolCombo_->addItem("Modbus TCP", static_cast<int>(Comm::PLCProtocol::ModbusTCP));
    protocolCombo_->addItem("Modbus RTU", static_cast<int>(Comm::PLCProtocol::ModbusRTU));
    protocolCombo_->addItem("三菱MC协议(TCP)", static_cast<int>(Comm::PLCProtocol::MitsubishiMC_TCP));
    protocolCombo_->addItem("基恩士PC-LINK上位链路", static_cast<int>(Comm::PLCProtocol::KeyencePCLink));
    protocolCombo_->addItem("欧姆龙FINS(TCP)", static_cast<int>(Comm::PLCProtocol::OmronFINS_TCP));
    protocolCombo_->addItem("西门子S7", static_cast<int>(Comm::PLCProtocol::SiemensS7));
    basicLayout->addWidget(protocolCombo_, 1, 1);

    basicLayout->addWidget(new QLabel("超时时间(ms):"), 2, 0);
    timeoutSpin_ = new QSpinBox();
    timeoutSpin_->setRange(100, 30000);
    timeoutSpin_->setValue(3000);
    basicLayout->addWidget(timeoutSpin_, 2, 1);

    basicLayout->addWidget(new QLabel("重试次数:"), 3, 0);
    retryCountSpin_ = new QSpinBox();
    retryCountSpin_->setRange(0, 10);
    retryCountSpin_->setValue(3);
    basicLayout->addWidget(retryCountSpin_, 3, 1);

    layout->addWidget(basicGroup);

    // 网络配置
    networkGroup_ = new QGroupBox("网络配置");
    auto* networkLayout = new QGridLayout(networkGroup_);

    networkLayout->addWidget(new QLabel("IP地址:"), 0, 0);
    ipEdit_ = new QLineEdit();
    ipEdit_->setText("127.0.0.1");
    ipEdit_->setPlaceholderText("127.0.0.1");
    networkLayout->addWidget(ipEdit_, 0, 1);

    networkLayout->addWidget(new QLabel("端口号:"), 1, 0);
    portSpin_ = new QSpinBox();
    portSpin_->setRange(1, 65535);
    portSpin_->setValue(20007);
    networkLayout->addWidget(portSpin_, 1, 1);

    layout->addWidget(networkGroup_);

    // 串口配置
    serialGroup_ = new QGroupBox("串口配置");
    auto* serialLayout = new QGridLayout(serialGroup_);

    serialLayout->addWidget(new QLabel("串口:"), 0, 0);
    portCombo_ = new QComboBox();
    for (int i = 1; i <= 16; ++i) {
        portCombo_->addItem(QString("COM%1").arg(i));
    }
    serialLayout->addWidget(portCombo_, 0, 1);

    serialLayout->addWidget(new QLabel("波特率:"), 1, 0);
    baudRateCombo_ = new QComboBox();
    baudRateCombo_->addItems({"9600", "19200", "38400", "57600", "115200"});
    serialLayout->addWidget(baudRateCombo_, 1, 1);

    serialLayout->addWidget(new QLabel("数据位:"), 2, 0);
    dataBitsCombo_ = new QComboBox();
    dataBitsCombo_->addItems({"7", "8"});
    dataBitsCombo_->setCurrentText("8");
    serialLayout->addWidget(dataBitsCombo_, 2, 1);

    serialLayout->addWidget(new QLabel("停止位:"), 3, 0);
    stopBitsCombo_ = new QComboBox();
    stopBitsCombo_->addItems({"1", "2"});
    serialLayout->addWidget(stopBitsCombo_, 3, 1);

    serialLayout->addWidget(new QLabel("校验:"), 4, 0);
    parityCombo_ = new QComboBox();
    parityCombo_->addItems({"无", "奇校验", "偶校验"});
    serialLayout->addWidget(parityCombo_, 4, 1);

    serialGroup_->setVisible(false);
    layout->addWidget(serialGroup_);

    // Modbus配置
    modbusGroup_ = new QGroupBox("Modbus配置");
    auto* modbusLayout = new QGridLayout(modbusGroup_);

    modbusLayout->addWidget(new QLabel("从站地址:"), 0, 0);
    slaveIdSpin_ = new QSpinBox();
    slaveIdSpin_->setRange(1, 247);
    slaveIdSpin_->setValue(1);
    modbusLayout->addWidget(slaveIdSpin_, 0, 1);

    layout->addWidget(modbusGroup_);

    // 按钮
    auto* btnLayout = new QHBoxLayout();
    applyBtn_ = new QPushButton("应用配置");
    testBtn_ = new QPushButton("测试连接");
    btnLayout->addStretch();
    btnLayout->addWidget(applyBtn_);
    btnLayout->addWidget(testBtn_);
    layout->addLayout(btnLayout);

    // 状态
    statusLabel_ = new QLabel("未连接");
    statusLabel_->setStyleSheet("color: gray;");
    layout->addWidget(statusLabel_);

    layout->addStretch();

    return panel;
}

QWidget* PLCConfigDialog::createTestPanel()
{
    auto* panel = new QWidget();
    auto* layout = new QVBoxLayout(panel);

    // 读写测试
    auto* testGroup = new QGroupBox("读写测试");
    auto* testLayout = new QGridLayout(testGroup);

    testLayout->addWidget(new QLabel("地址:"), 0, 0);
    testAddressSpin_ = new QSpinBox();
    testAddressSpin_->setRange(0, 65535);
    testAddressSpin_->setValue(0);
    testLayout->addWidget(testAddressSpin_, 0, 1);

    testLayout->addWidget(new QLabel("数量:"), 1, 0);
    testCountSpin_ = new QSpinBox();
    testCountSpin_->setRange(1, 125);
    testCountSpin_->setValue(10);
    testLayout->addWidget(testCountSpin_, 1, 1);

    testLayout->addWidget(new QLabel("写入值:"), 2, 0);
    testValueEdit_ = new QLineEdit();
    testValueEdit_->setPlaceholderText("多个值用逗号分隔");
    testLayout->addWidget(testValueEdit_, 2, 1);

    auto* btnLayout1 = new QHBoxLayout();
    readRegBtn_ = new QPushButton("读寄存器");
    writeRegBtn_ = new QPushButton("写寄存器");
    btnLayout1->addWidget(readRegBtn_);
    btnLayout1->addWidget(writeRegBtn_);
    testLayout->addLayout(btnLayout1, 3, 0, 1, 2);

    auto* btnLayout2 = new QHBoxLayout();
    readCoilBtn_ = new QPushButton("读线圈");
    writeCoilBtn_ = new QPushButton("写线圈");
    btnLayout2->addWidget(readCoilBtn_);
    btnLayout2->addWidget(writeCoilBtn_);
    testLayout->addLayout(btnLayout2, 4, 0, 1, 2);

    layout->addWidget(testGroup);

    // 结果表格
    layout->addWidget(new QLabel("读取结果:"));
    resultTable_ = new QTableWidget();
    resultTable_->setColumnCount(2);
    resultTable_->setHorizontalHeaderLabels({"地址", "值"});
    resultTable_->horizontalHeader()->setStretchLastSection(true);
    resultTable_->setMaximumHeight(150);
    layout->addWidget(resultTable_);

    // 日志
    layout->addWidget(new QLabel("通信日志:"));
    logText_ = new QTextEdit();
    logText_->setReadOnly(true);
    logText_->setMaximumHeight(150);
    layout->addWidget(logText_);

    layout->addStretch();

    return panel;
}

void PLCConfigDialog::setupConnections()
{
    // 连接列表
    connect(addBtn_, &QPushButton::clicked, this, &PLCConfigDialog::onAddConnection);
    connect(removeBtn_, &QPushButton::clicked, this, &PLCConfigDialog::onRemoveConnection);
    connect(connectionList_, &QListWidget::currentRowChanged, this, &PLCConfigDialog::onConnectionSelected);

    // 协议切换
    connect(protocolCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &PLCConfigDialog::onProtocolChanged);

    // 配置
    connect(applyBtn_, &QPushButton::clicked, this, &PLCConfigDialog::onApplyConfig);
    connect(testBtn_, &QPushButton::clicked, this, &PLCConfigDialog::onTestConnection);

    // 读写测试
    connect(readRegBtn_, &QPushButton::clicked, this, &PLCConfigDialog::onReadRegister);
    connect(writeRegBtn_, &QPushButton::clicked, this, &PLCConfigDialog::onWriteRegister);
    connect(readCoilBtn_, &QPushButton::clicked, this, &PLCConfigDialog::onReadCoil);
    connect(writeCoilBtn_, &QPushButton::clicked, this, &PLCConfigDialog::onWriteCoil);

    // 保存/加载
    connect(saveConfigBtn_, &QPushButton::clicked, this, &PLCConfigDialog::onSaveConfig);
    connect(loadConfigBtn_, &QPushButton::clicked, this, &PLCConfigDialog::onLoadConfig);

    // 管理器信号
    connect(&Comm::PLCManager::instance(), &Comm::PLCManager::connectionStatusChanged,
            this, [this](const QString& name, bool connected) {
                if (name == currentConnection_) {
                    statusLabel_->setText(connected ? "已连接" : "未连接");
                    statusLabel_->setStyleSheet(connected ? "color: green;" : "color: gray;");
                }
                appendLog(QString("[%1] %2").arg(name).arg(connected ? "已连接" : "已断开"));
            });

    connect(&Comm::PLCManager::instance(), &Comm::PLCManager::errorOccurred,
            this, [this](const QString& name, const QString& error) {
                appendLog(QString("[%1] 错误: %2").arg(name).arg(error));
            });

    connect(&Comm::PLCManager::instance(), &Comm::PLCManager::connectionsChanged,
            this, &PLCConfigDialog::updateConnectionList);
}

void PLCConfigDialog::updateConnectionList()
{
    connectionList_->clear();
    QStringList names = Comm::PLCManager::instance().getConnectionNames();
    connectionList_->addItems(names);

    if (!currentConnection_.isEmpty()) {
        for (int i = 0; i < connectionList_->count(); ++i) {
            if (connectionList_->item(i)->text() == currentConnection_) {
                connectionList_->setCurrentRow(i);
                break;
            }
        }
    }
}

void PLCConfigDialog::onAddConnection()
{
    bool ok;
    QString name = QInputDialog::getText(this, "添加连接", "连接名称:",
                                         QLineEdit::Normal, "PLC1", &ok);
    if (ok && !name.isEmpty()) {
        Comm::PLCConfig config;
        config.name = name;
        config.protocol = Comm::PLCProtocol::ModbusTCP;

        auto socketConfig = std::make_shared<Comm::SocketConfig>();
        socketConfig->ipAddress = "127.0.0.1";
        socketConfig->port = 20007;
        config.commConfig = socketConfig;

        if (Comm::PLCManager::instance().addConnection(name, config)) {
            currentConnection_ = name;
            loadConnectionConfig(name);
            appendLog(QString("添加连接: %1").arg(name));
        }
    }
}

void PLCConfigDialog::onRemoveConnection()
{
    if (currentConnection_.isEmpty()) {
        return;
    }

    if (QMessageBox::question(this, "确认删除",
                              QString("确定要删除连接'%1'吗?").arg(currentConnection_),
                              QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        Comm::PLCManager::instance().removeConnection(currentConnection_);
        currentConnection_.clear();
        clearConfigFields();
        appendLog(QString("删除连接: %1").arg(currentConnection_));
    }
}

void PLCConfigDialog::onConnectionSelected(int row)
{
    if (row < 0) {
        currentConnection_.clear();
        clearConfigFields();
        return;
    }

    currentConnection_ = connectionList_->item(row)->text();
    loadConnectionConfig(currentConnection_);

    // 更新连接状态
    bool connected = Comm::PLCManager::instance().isConnected(currentConnection_);
    statusLabel_->setText(connected ? "已连接" : "未连接");
    statusLabel_->setStyleSheet(connected ? "color: green;" : "color: gray;");
}

void PLCConfigDialog::loadConnectionConfig(const QString& name)
{
    Comm::PLCConfig config = Comm::PLCManager::instance().getConfig(name);

    nameEdit_->setText(config.name);

    // 设置协议
    for (int i = 0; i < protocolCombo_->count(); ++i) {
        if (protocolCombo_->itemData(i).toInt() == static_cast<int>(config.protocol)) {
            protocolCombo_->setCurrentIndex(i);
            break;
        }
    }

    // 设置通信配置
    if (config.commConfig) {
        timeoutSpin_->setValue(config.commConfig->timeout);
        retryCountSpin_->setValue(config.commConfig->retryCount);

        if (config.commConfig->type == Comm::CommType::Socket) {
            auto socketConfig = std::dynamic_pointer_cast<Comm::SocketConfig>(config.commConfig);
            if (socketConfig) {
                ipEdit_->setText(socketConfig->ipAddress);
                portSpin_->setValue(socketConfig->port);
            }
        } else if (config.commConfig->type == Comm::CommType::Serial) {
            auto serialConfig = std::dynamic_pointer_cast<Comm::SerialConfig>(config.commConfig);
            if (serialConfig) {
                portCombo_->setCurrentText(serialConfig->portName);
                baudRateCombo_->setCurrentText(QString::number(serialConfig->baudRate));
                dataBitsCombo_->setCurrentText(QString::number(serialConfig->dataBits));
                stopBitsCombo_->setCurrentText(QString::number(serialConfig->stopBits));
                parityCombo_->setCurrentIndex(serialConfig->parity);
            }
        }
    }

    // Modbus配置
    slaveIdSpin_->setValue(config.slaveId);

    onProtocolChanged(protocolCombo_->currentIndex());
}

void PLCConfigDialog::clearConfigFields()
{
    nameEdit_->clear();
    ipEdit_->clear();
    statusLabel_->setText("未连接");
    statusLabel_->setStyleSheet("color: gray;");
}

void PLCConfigDialog::onProtocolChanged(int index)
{
    int protocol = protocolCombo_->itemData(index).toInt();

    // 根据协议显示/隐藏配置组
    bool isModbus = (protocol >= static_cast<int>(Comm::PLCProtocol::ModbusTCP) &&
                     protocol <= static_cast<int>(Comm::PLCProtocol::ModbusASCII));
    bool isSerial = (protocol == static_cast<int>(Comm::PLCProtocol::ModbusRTU) ||
                     protocol == static_cast<int>(Comm::PLCProtocol::ModbusASCII) ||
                     protocol == static_cast<int>(Comm::PLCProtocol::MitsubishiFX_Serial));

    networkGroup_->setVisible(!isSerial);
    serialGroup_->setVisible(isSerial);
    modbusGroup_->setVisible(isModbus);

    // 设置默认端口 (统一使用20007)
    if (!isSerial) {
        portSpin_->setValue(20007);
    }
}

void PLCConfigDialog::onApplyConfig()
{
    if (currentConnection_.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先选择或创建一个连接");
        return;
    }

    Comm::PLCConfig config = getCurrentConfig();

    // 先断开再重新配置
    Comm::PLCManager::instance().disconnectPLC(currentConnection_);
    Comm::PLCManager::instance().removeConnection(currentConnection_);

    if (Comm::PLCManager::instance().addConnection(config.name, config)) {
        currentConnection_ = config.name;
        appendLog(QString("配置已应用: %1").arg(config.name));
    } else {
        QMessageBox::warning(this, "错误", "应用配置失败");
    }
}

Comm::PLCConfig PLCConfigDialog::getCurrentConfig() const
{
    Comm::PLCConfig config;
    config.name = nameEdit_->text();
    config.protocol = static_cast<Comm::PLCProtocol>(protocolCombo_->currentData().toInt());
    config.slaveId = slaveIdSpin_->value();
    // 三菱MC协议使用固定默认值
    config.networkNo = 0;            // 网络号
    config.pcNo = 0xFF;              // PC号
    config.destModuleIo = 0x03FF;    // 请求目标模块IO号
    config.destModuleStation = 0;     // 请求目标模块站号

    if (serialGroup_->isVisible()) {
        auto serialConfig = std::make_shared<Comm::SerialConfig>();
        serialConfig->timeout = timeoutSpin_->value();
        serialConfig->retryCount = retryCountSpin_->value();
        serialConfig->portName = portCombo_->currentText();
        serialConfig->baudRate = baudRateCombo_->currentText().toInt();
        serialConfig->dataBits = dataBitsCombo_->currentText().toInt();
        serialConfig->stopBits = stopBitsCombo_->currentText().toInt();
        serialConfig->parity = parityCombo_->currentIndex();
        config.commConfig = serialConfig;
    } else {
        auto socketConfig = std::make_shared<Comm::SocketConfig>();
        socketConfig->timeout = timeoutSpin_->value();
        socketConfig->retryCount = retryCountSpin_->value();
        socketConfig->ipAddress = ipEdit_->text();
        socketConfig->port = static_cast<quint16>(portSpin_->value());
        config.commConfig = socketConfig;
    }

    return config;
}

void PLCConfigDialog::onTestConnection()
{
    if (currentConnection_.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先选择一个连接");
        return;
    }

    appendLog(QString("正在测试连接: %1").arg(currentConnection_));

    if (Comm::PLCManager::instance().connectPLC(currentConnection_)) {
        QMessageBox::information(this, "成功", "连接成功");
        appendLog("连接测试成功");
    } else {
        QMessageBox::warning(this, "失败", "连接失败");
        appendLog("连接测试失败");
    }
}

void PLCConfigDialog::onReadRegister()
{
    if (currentConnection_.isEmpty() || !Comm::PLCManager::instance().isConnected(currentConnection_)) {
        QMessageBox::warning(this, "警告", "请先连接PLC");
        return;
    }

    int address = testAddressSpin_->value();
    int count = testCountSpin_->value();

    appendLog(QString("读取寄存器: 地址=%1, 数量=%2").arg(address).arg(count));

    // 打印预期帧格式供调试
    QByteArray debugFrame;
    debugFrame.append(static_cast<char>(0x50)); // 子头
    debugFrame.append(static_cast<char>(0x00));
    debugFrame.append(static_cast<char>(0x00)); // 网络号
    debugFrame.append(static_cast<char>(0xFF)); // PC号
    debugFrame.append(static_cast<char>(0xFF)); // 目标IO低
    debugFrame.append(static_cast<char>(0x03)); // 目标IO高
    debugFrame.append(static_cast<char>(0x00)); // 站号
    quint16 dataLen = 2 + 2 + 2 + 6; // 监视定时器+命令+子命令+数据(地址3+设备1+点数2)
    debugFrame.append(static_cast<char>(dataLen & 0xFF));
    debugFrame.append(static_cast<char>((dataLen >> 8) & 0xFF));
    debugFrame.append(static_cast<char>(0x10)); // 监视定时器
    debugFrame.append(static_cast<char>(0x00));
    debugFrame.append(static_cast<char>(0x01)); // 命令0x0401
    debugFrame.append(static_cast<char>(0x04));
    debugFrame.append(static_cast<char>(0x00)); // 子命令
    debugFrame.append(static_cast<char>(0x00));
    debugFrame.append(static_cast<char>(address & 0xFF)); // 地址
    debugFrame.append(static_cast<char>((address >> 8) & 0xFF));
    debugFrame.append(static_cast<char>((address >> 16) & 0xFF));
    debugFrame.append(static_cast<char>(0xA8)); // D寄存器设备代码
    debugFrame.append(static_cast<char>(count & 0xFF)); // 点数
    debugFrame.append(static_cast<char>((count >> 8) & 0xFF));
    appendLog(QString("预期发送: %1").arg(QString(debugFrame.toHex(' ').toUpper())));

    auto result = Comm::PLCManager::instance().readRegisters(currentConnection_, address, count);

    if (result.isSuccess()) {
        resultTable_->setRowCount(static_cast<int>(result.intValues.size()));
        for (size_t i = 0; i < result.intValues.size(); ++i) {
            resultTable_->setItem(static_cast<int>(i), 0, new QTableWidgetItem(QString::number(address + static_cast<int>(i))));
            resultTable_->setItem(static_cast<int>(i), 1, new QTableWidgetItem(QString::number(result.intValues[i])));
        }
        appendLog(QString("读取成功, 返回%1个值").arg(result.intValues.size()));
    } else {
        appendLog(QString("读取失败: %1").arg(result.errorMessage));
    }
}

void PLCConfigDialog::onWriteRegister()
{
    if (currentConnection_.isEmpty() || !Comm::PLCManager::instance().isConnected(currentConnection_)) {
        QMessageBox::warning(this, "警告", "请先连接PLC");
        return;
    }

    int address = testAddressSpin_->value();
    QString valueStr = testValueEdit_->text();

    if (valueStr.isEmpty()) {
        QMessageBox::warning(this, "警告", "请输入写入值");
        return;
    }

    std::vector<int> values;
    QStringList parts = valueStr.split(',', Qt::SkipEmptyParts);
    for (const QString& part : parts) {
        values.push_back(part.trimmed().toInt());
    }

    appendLog(QString("写入寄存器: 地址=%1, 值=%2").arg(address).arg(valueStr));

    auto result = Comm::PLCManager::instance().writeRegisters(currentConnection_, address, values);

    if (result.isSuccess()) {
        appendLog("写入成功");
        QMessageBox::information(this, "成功", "写入成功");
    } else {
        appendLog(QString("写入失败: %1").arg(result.errorMessage));
        QMessageBox::warning(this, "写入失败", result.errorMessage);
    }
}

void PLCConfigDialog::onReadCoil()
{
    if (currentConnection_.isEmpty() || !Comm::PLCManager::instance().isConnected(currentConnection_)) {
        QMessageBox::warning(this, "警告", "请先连接PLC");
        return;
    }

    int address = testAddressSpin_->value();
    int count = testCountSpin_->value();

    appendLog(QString("读取线圈: 地址=%1, 数量=%2").arg(address).arg(count));

    auto result = Comm::PLCManager::instance().readCoils(currentConnection_, address, count);

    if (result.isSuccess()) {
        resultTable_->setRowCount(qMin(static_cast<int>(result.boolValues.size()), count));
        for (int i = 0; i < qMin(static_cast<int>(result.boolValues.size()), count); ++i) {
            resultTable_->setItem(i, 0, new QTableWidgetItem(QString::number(address + i)));
            resultTable_->setItem(i, 1, new QTableWidgetItem(result.boolValues[i] ? "ON" : "OFF"));
        }
        appendLog(QString("读取成功, 返回%1个值").arg(result.boolValues.size()));
    } else {
        appendLog(QString("读取失败: %1").arg(result.errorMessage));
        QMessageBox::warning(this, "读取失败", result.errorMessage);
    }
}

void PLCConfigDialog::onWriteCoil()
{
    if (currentConnection_.isEmpty() || !Comm::PLCManager::instance().isConnected(currentConnection_)) {
        QMessageBox::warning(this, "警告", "请先连接PLC");
        return;
    }

    int address = testAddressSpin_->value();
    QString valueStr = testValueEdit_->text();

    if (valueStr.isEmpty()) {
        QMessageBox::warning(this, "警告", "请输入写入值 (1/0 或 ON/OFF)");
        return;
    }

    std::vector<bool> values;
    QStringList parts = valueStr.split(',', Qt::SkipEmptyParts);
    for (const QString& part : parts) {
        QString v = part.trimmed().toUpper();
        values.push_back(v == "1" || v == "ON" || v == "TRUE");
    }

    appendLog(QString("写入线圈: 地址=%1, 值=%2").arg(address).arg(valueStr));

    auto result = Comm::PLCManager::instance().writeCoils(currentConnection_, address, values);

    if (result.isSuccess()) {
        appendLog("写入成功");
        QMessageBox::information(this, "成功", "写入成功");
    } else {
        appendLog(QString("写入失败: %1").arg(result.errorMessage));
        QMessageBox::warning(this, "写入失败", result.errorMessage);
    }
}

void PLCConfigDialog::onSaveConfig()
{
    QString filePath = QFileDialog::getSaveFileName(this, "保存配置",
                                                    QString(), "JSON文件 (*.json)");
    if (filePath.isEmpty()) {
        return;
    }

    if (Comm::PLCManager::instance().saveConfig(filePath)) {
        appendLog(QString("配置已保存到: %1").arg(filePath));
        QMessageBox::information(this, "成功", "配置保存成功");
    } else {
        QMessageBox::warning(this, "失败", "配置保存失败");
    }
}

void PLCConfigDialog::onLoadConfig()
{
    QString filePath = QFileDialog::getOpenFileName(this, "加载配置",
                                                    QString(), "JSON文件 (*.json)");
    if (filePath.isEmpty()) {
        return;
    }

    if (Comm::PLCManager::instance().loadConfig(filePath)) {
        appendLog(QString("配置已从%1加载").arg(filePath));
        QMessageBox::information(this, "成功", "配置加载成功");
    } else {
        QMessageBox::warning(this, "失败", "配置加载失败");
    }
}

void PLCConfigDialog::appendLog(const QString& message)
{
    QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
    logText_->append(QString("[%1] %2").arg(timestamp).arg(message));
}

} // namespace VisionForge
