/**
 * @file AlignmentOutputTool.cpp
 * @brief 对位结果输出工具实现
 * @author VisionForge Team
 * @date 2025-12-19
 */

#include "algorithm/AlignmentOutputTool.h"
#include "base/Logger.h"
#include <QElapsedTimer>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDataStream>
#include <QtEndian>

namespace VisionForge {
namespace Algorithm {

// ============================================================
// 配置序列化
// ============================================================

QJsonObject PLCOutputConfig::toJson() const
{
    QJsonObject obj;
    obj["protocolType"] = protocolType;
    obj["ipAddress"] = ipAddress;
    obj["port"] = port;
    obj["offsetXRegister"] = offsetXRegister;
    obj["offsetYRegister"] = offsetYRegister;
    obj["offsetThetaRegister"] = offsetThetaRegister;
    obj["resultRegister"] = resultRegister;
    obj["triggerRegister"] = triggerRegister;
    obj["scaleFactor"] = scaleFactor;
    obj["signedValue"] = signedValue;
    obj["dataType"] = dataType;
    return obj;
}

PLCOutputConfig PLCOutputConfig::fromJson(const QJsonObject& json)
{
    PLCOutputConfig config;
    config.protocolType = json["protocolType"].toString("Modbus");
    config.ipAddress = json["ipAddress"].toString();
    config.port = json["port"].toInt(502);
    config.offsetXRegister = json["offsetXRegister"].toInt(0);
    config.offsetYRegister = json["offsetYRegister"].toInt(2);
    config.offsetThetaRegister = json["offsetThetaRegister"].toInt(4);
    config.resultRegister = json["resultRegister"].toInt(6);
    config.triggerRegister = json["triggerRegister"].toInt(7);
    config.scaleFactor = json["scaleFactor"].toDouble(1000.0);
    config.signedValue = json["signedValue"].toBool(true);
    config.dataType = json["dataType"].toInt(0);
    return config;
}

QJsonObject TCPOutputConfig::toJson() const
{
    QJsonObject obj;
    obj["serverAddress"] = serverAddress;
    obj["port"] = port;
    obj["dataFormat"] = dataFormat;
    obj["customFormat"] = customFormat;
    obj["delimiter"] = delimiter;
    obj["addChecksum"] = addChecksum;
    obj["endMarker"] = endMarker;
    obj["timeoutMs"] = timeoutMs;
    return obj;
}

TCPOutputConfig TCPOutputConfig::fromJson(const QJsonObject& json)
{
    TCPOutputConfig config;
    config.serverAddress = json["serverAddress"].toString();
    config.port = json["port"].toInt(5000);
    config.dataFormat = json["dataFormat"].toString("JSON");
    config.customFormat = json["customFormat"].toString();
    config.delimiter = json["delimiter"].toString(",");
    config.addChecksum = json["addChecksum"].toBool(false);
    config.endMarker = json["endMarker"].toString("\r\n");
    config.timeoutMs = json["timeoutMs"].toInt(1000);
    return config;
}

QJsonObject SerialOutputConfig::toJson() const
{
    QJsonObject obj;
    obj["portName"] = portName;
    obj["baudRate"] = baudRate;
    obj["dataBits"] = dataBits;
    obj["stopBits"] = stopBits;
    obj["parity"] = parity;
    obj["dataFormat"] = dataFormat;
    obj["customFormat"] = customFormat;
    obj["endMarker"] = endMarker;
    obj["timeoutMs"] = timeoutMs;
    return obj;
}

SerialOutputConfig SerialOutputConfig::fromJson(const QJsonObject& json)
{
    SerialOutputConfig config;
    config.portName = json["portName"].toString();
    config.baudRate = json["baudRate"].toInt(9600);
    config.dataBits = json["dataBits"].toInt(8);
    config.stopBits = json["stopBits"].toInt(1);
    config.parity = json["parity"].toString("None");
    config.dataFormat = json["dataFormat"].toString("CSV");
    config.customFormat = json["customFormat"].toString();
    config.endMarker = json["endMarker"].toString("\r\n");
    config.timeoutMs = json["timeoutMs"].toInt(1000);
    return config;
}

// ============================================================
// AlignmentOutputTool 实现
// ============================================================

AlignmentOutputTool::AlignmentOutputTool(QObject* parent)
    : VisionTool(parent)
    , outputMethod_(OutputMethod::None)
    , offsetX_(0.0)
    , offsetY_(0.0)
    , offsetTheta_(0.0)
    , judgeResult_(true)
{
}

AlignmentOutputTool::~AlignmentOutputTool()
{
    disconnectPLC();
    disconnectTCP();
    closeSerial();
}

// ========== VisionTool 接口 ==========

bool AlignmentOutputTool::process(const Base::ImageData::Ptr& input, ToolResult& output)
{
    Q_UNUSED(input);

    OutputResult result = executeOutput();
    lastResult_ = result;

    if (result.success) {
        output.success = true;
        output.data["outputSuccess"] = true;
        output.data["outputTime"] = result.outputTime;
    } else {
        output.success = false;
        output.errorMessage = result.errorMessage;
        return false;
    }

    return result.success;
}

QJsonObject AlignmentOutputTool::serializeParams() const
{
    QJsonObject params;
    params["outputMethod"] = static_cast<int>(outputMethod_);
    params["plcConfig"] = plcConfig_.toJson();
    params["tcpConfig"] = tcpConfig_.toJson();
    params["serialConfig"] = serialConfig_.toJson();
    return params;
}

void AlignmentOutputTool::deserializeParams(const QJsonObject& params)
{
    outputMethod_ = static_cast<OutputMethod>(params["outputMethod"].toInt());
    plcConfig_ = PLCOutputConfig::fromJson(params["plcConfig"].toObject());
    tcpConfig_ = TCPOutputConfig::fromJson(params["tcpConfig"].toObject());
    serialConfig_ = SerialOutputConfig::fromJson(params["serialConfig"].toObject());
}

// ========== 配置管理 ==========

void AlignmentOutputTool::setPLCConfig(const PLCOutputConfig& config)
{
    plcConfig_ = config;
}

PLCOutputConfig AlignmentOutputTool::getPLCConfig() const
{
    return plcConfig_;
}

void AlignmentOutputTool::setTCPConfig(const TCPOutputConfig& config)
{
    tcpConfig_ = config;
}

TCPOutputConfig AlignmentOutputTool::getTCPConfig() const
{
    return tcpConfig_;
}

void AlignmentOutputTool::setSerialConfig(const SerialOutputConfig& config)
{
    serialConfig_ = config;
}

SerialOutputConfig AlignmentOutputTool::getSerialConfig() const
{
    return serialConfig_;
}

void AlignmentOutputTool::setOutputMethod(OutputMethod method)
{
    outputMethod_ = method;
}

OutputMethod AlignmentOutputTool::getOutputMethod() const
{
    return outputMethod_;
}

// ========== 数据设置 ==========

void AlignmentOutputTool::setAlignmentResult(const AlignmentResult& result)
{
    alignmentResult_ = result;
    offsetX_ = result.offsetX;
    offsetY_ = result.offsetY;
    offsetTheta_ = result.offsetTheta;
    judgeResult_ = result.success;
}

void AlignmentOutputTool::setOffset(double x, double y, double theta)
{
    offsetX_ = x;
    offsetY_ = y;
    offsetTheta_ = theta;
}

void AlignmentOutputTool::setJudgeResult(bool isOK)
{
    judgeResult_ = isOK;
}

// ========== 输出执行 ==========

OutputResult AlignmentOutputTool::executeOutput()
{
    OutputResult result;
    QElapsedTimer timer;
    timer.start();

    bool anyOutput = false;
    bool anySuccess = false;

    // PLC输出
    if (hasFlag(outputMethod_, OutputMethod::PLC)) {
        anyOutput = true;
        result.plcSuccess = outputToPLC();
        if (result.plcSuccess) anySuccess = true;
    }

    // TCP输出
    if (hasFlag(outputMethod_, OutputMethod::TCP)) {
        anyOutput = true;
        result.tcpSuccess = outputToTCP();
        if (result.tcpSuccess) anySuccess = true;
    }

    // 串口输出
    if (hasFlag(outputMethod_, OutputMethod::Serial)) {
        anyOutput = true;
        result.serialSuccess = outputToSerial();
        if (result.serialSuccess) anySuccess = true;
    }

    result.outputTime = timer.elapsed();

    if (!anyOutput) {
        result.success = true; // 没有配置输出也算成功
    } else {
        result.success = anySuccess;
        if (!result.success) {
            result.errorMessage = "所有输出方式都失败";
        }
    }

    lastResult_ = result;
    emit outputCompleted(result);

    return result;
}

bool AlignmentOutputTool::outputToPLC()
{
    if (plcConfig_.ipAddress.isEmpty()) {
        LOG_WARNING("PLC IP地址未配置");
        return false;
    }

    // 确保连接
    if (!isPLCConnected()) {
        if (!connectPLC()) {
            return false;
        }
    }

    // 根据协议类型发送数据
    if (plcConfig_.protocolType == "Modbus") {
        // Modbus TCP协议
        QVector<quint16> values;

        // 根据数据类型处理
        if (plcConfig_.dataType == 0) {
            // Int16
            qint16 scaledX = static_cast<qint16>(offsetX_ * plcConfig_.scaleFactor);
            qint16 scaledY = static_cast<qint16>(offsetY_ * plcConfig_.scaleFactor);
            qint16 scaledTheta = static_cast<qint16>(offsetTheta_ * plcConfig_.scaleFactor);

            values.append(static_cast<quint16>(scaledX));
            values.append(static_cast<quint16>(scaledY));
            values.append(static_cast<quint16>(scaledTheta));
            values.append(judgeResult_ ? 1 : 0);

            return writeModbusRegisters(plcConfig_.offsetXRegister, values);
        } else if (plcConfig_.dataType == 2) {
            // Float - 每个值占2个寄存器
            if (!writeModbusFloat(plcConfig_.offsetXRegister, static_cast<float>(offsetX_))) {
                return false;
            }
            if (!writeModbusFloat(plcConfig_.offsetYRegister, static_cast<float>(offsetY_))) {
                return false;
            }
            if (!writeModbusFloat(plcConfig_.offsetThetaRegister, static_cast<float>(offsetTheta_))) {
                return false;
            }

            QVector<quint16> resultVal;
            resultVal.append(judgeResult_ ? 1 : 0);
            return writeModbusRegisters(plcConfig_.resultRegister, resultVal);
        }
    } else if (plcConfig_.protocolType == "MC") {
        // 三菱MC协议
        // 构建MC协议帧
        QByteArray frame;
        // 子头
        frame.append(static_cast<char>(0x50)); // 请求
        frame.append(static_cast<char>(0x00));
        // 网络号
        frame.append(static_cast<char>(0x00));
        // PLC号
        frame.append(static_cast<char>(0xFF));
        // 请求目标模块I/O号
        frame.append(static_cast<char>(0xFF));
        frame.append(static_cast<char>(0x03));
        // 请求目标模块站号
        frame.append(static_cast<char>(0x00));

        // 数据长度（后续计算）
        // 命令 + 子命令 + 起始地址 + 点数 + 数据
        int dataLen = 2 + 2 + 4 + 2 + 8; // 简化版
        frame.append(static_cast<char>(dataLen & 0xFF));
        frame.append(static_cast<char>((dataLen >> 8) & 0xFF));

        // CPU监视定时器
        frame.append(static_cast<char>(0x10));
        frame.append(static_cast<char>(0x00));

        // 命令：批量写入 0x1401
        frame.append(static_cast<char>(0x01));
        frame.append(static_cast<char>(0x14));

        // 子命令：0x0000（按字）
        frame.append(static_cast<char>(0x00));
        frame.append(static_cast<char>(0x00));

        // 起始软元件（D寄存器）
        int startAddr = plcConfig_.offsetXRegister;
        frame.append(static_cast<char>(startAddr & 0xFF));
        frame.append(static_cast<char>((startAddr >> 8) & 0xFF));
        frame.append(static_cast<char>((startAddr >> 16) & 0xFF));
        frame.append(static_cast<char>(0xA8)); // D寄存器代码

        // 点数
        frame.append(static_cast<char>(0x04));
        frame.append(static_cast<char>(0x00));

        // 数据
        qint16 scaledX = static_cast<qint16>(offsetX_ * plcConfig_.scaleFactor);
        qint16 scaledY = static_cast<qint16>(offsetY_ * plcConfig_.scaleFactor);
        qint16 scaledTheta = static_cast<qint16>(offsetTheta_ * plcConfig_.scaleFactor);
        qint16 result = judgeResult_ ? 1 : 0;

        frame.append(static_cast<char>(scaledX & 0xFF));
        frame.append(static_cast<char>((scaledX >> 8) & 0xFF));
        frame.append(static_cast<char>(scaledY & 0xFF));
        frame.append(static_cast<char>((scaledY >> 8) & 0xFF));
        frame.append(static_cast<char>(scaledTheta & 0xFF));
        frame.append(static_cast<char>((scaledTheta >> 8) & 0xFF));
        frame.append(static_cast<char>(result & 0xFF));
        frame.append(static_cast<char>((result >> 8) & 0xFF));

        if (plcSocket_->write(frame) != frame.size()) {
            LOG_ERROR("MC协议发送失败");
            return false;
        }

        // 等待响应
        if (!plcSocket_->waitForReadyRead(1000)) {
            LOG_ERROR("MC协议响应超时");
            return false;
        }

        QByteArray response = plcSocket_->readAll();
        // 检查响应（简化）
        if (response.size() >= 11) {
            quint16 endCode = static_cast<quint8>(response[9]) | (static_cast<quint8>(response[10]) << 8);
            if (endCode != 0) {
                LOG_ERROR(QString("MC协议错误码: 0x%1").arg(endCode, 4, 16, QChar('0')));
                return false;
            }
        }

        LOG_INFO("MC协议输出成功");
        return true;
    }

    return false;
}

bool AlignmentOutputTool::outputToTCP()
{
    if (tcpConfig_.serverAddress.isEmpty()) {
        LOG_WARNING("TCP服务器地址未配置");
        return false;
    }

    // 确保连接
    if (!isTCPConnected()) {
        if (!connectTCP()) {
            return false;
        }
    }

    // 格式化数据
    QByteArray data = formatData(tcpConfig_.dataFormat);

    // 添加校验和
    if (tcpConfig_.addChecksum) {
        quint16 checksum = calculateChecksum(data);
        data.append(static_cast<char>(checksum & 0xFF));
        data.append(static_cast<char>((checksum >> 8) & 0xFF));
    }

    // 添加结束标记
    data.append(tcpConfig_.endMarker.toUtf8());

    // 发送数据
    if (tcpSocket_->write(data) != data.size()) {
        LOG_ERROR("TCP发送失败");
        return false;
    }

    if (!tcpSocket_->waitForBytesWritten(tcpConfig_.timeoutMs)) {
        LOG_ERROR("TCP发送超时");
        return false;
    }

    LOG_INFO(QString("TCP输出成功: %1 bytes").arg(data.size()));
    return true;
}

bool AlignmentOutputTool::outputToSerial()
{
    if (serialConfig_.portName.isEmpty()) {
        LOG_WARNING("串口名称未配置");
        return false;
    }

    // 确保打开
    if (!isSerialOpen()) {
        if (!openSerial()) {
            return false;
        }
    }

    // 格式化数据
    QByteArray data = formatData(serialConfig_.dataFormat);

    // 添加结束标记
    data.append(serialConfig_.endMarker.toUtf8());

    // 发送数据
    if (serialPort_->write(data) != data.size()) {
        LOG_ERROR("串口发送失败");
        return false;
    }

    if (!serialPort_->waitForBytesWritten(serialConfig_.timeoutMs)) {
        LOG_ERROR("串口发送超时");
        return false;
    }

    LOG_INFO(QString("串口输出成功: %1 bytes").arg(data.size()));
    return true;
}

OutputResult AlignmentOutputTool::getLastOutputResult() const
{
    return lastResult_;
}

// ========== 连接管理 ==========

bool AlignmentOutputTool::connectPLC()
{
    if (!plcSocket_) {
        plcSocket_ = std::make_unique<QTcpSocket>();
    }

    if (plcSocket_->state() == QAbstractSocket::ConnectedState) {
        return true;
    }

    plcSocket_->connectToHost(plcConfig_.ipAddress, plcConfig_.port);

    if (!plcSocket_->waitForConnected(3000)) {
        LOG_ERROR(QString("PLC连接失败: %1:%2 - %3")
                  .arg(plcConfig_.ipAddress)
                  .arg(plcConfig_.port)
                  .arg(plcSocket_->errorString()));
        return false;
    }

    LOG_INFO(QString("PLC连接成功: %1:%2").arg(plcConfig_.ipAddress).arg(plcConfig_.port));
    emit plcConnectionChanged(true);
    return true;
}

void AlignmentOutputTool::disconnectPLC()
{
    if (plcSocket_ && plcSocket_->state() == QAbstractSocket::ConnectedState) {
        plcSocket_->disconnectFromHost();
        LOG_INFO("PLC连接已断开");
        emit plcConnectionChanged(false);
    }
}

bool AlignmentOutputTool::isPLCConnected() const
{
    return plcSocket_ && plcSocket_->state() == QAbstractSocket::ConnectedState;
}

bool AlignmentOutputTool::connectTCP()
{
    if (!tcpSocket_) {
        tcpSocket_ = std::make_unique<QTcpSocket>();
    }

    if (tcpSocket_->state() == QAbstractSocket::ConnectedState) {
        return true;
    }

    tcpSocket_->connectToHost(tcpConfig_.serverAddress, tcpConfig_.port);

    if (!tcpSocket_->waitForConnected(tcpConfig_.timeoutMs)) {
        LOG_ERROR(QString("TCP连接失败: %1:%2 - %3")
                  .arg(tcpConfig_.serverAddress)
                  .arg(tcpConfig_.port)
                  .arg(tcpSocket_->errorString()));
        return false;
    }

    LOG_INFO(QString("TCP连接成功: %1:%2").arg(tcpConfig_.serverAddress).arg(tcpConfig_.port));
    emit tcpConnectionChanged(true);
    return true;
}

void AlignmentOutputTool::disconnectTCP()
{
    if (tcpSocket_ && tcpSocket_->state() == QAbstractSocket::ConnectedState) {
        tcpSocket_->disconnectFromHost();
        LOG_INFO("TCP连接已断开");
        emit tcpConnectionChanged(false);
    }
}

bool AlignmentOutputTool::isTCPConnected() const
{
    return tcpSocket_ && tcpSocket_->state() == QAbstractSocket::ConnectedState;
}

bool AlignmentOutputTool::openSerial()
{
    if (!serialPort_) {
        serialPort_ = std::make_unique<QSerialPort>();
    }

    if (serialPort_->isOpen()) {
        return true;
    }

    serialPort_->setPortName(serialConfig_.portName);
    serialPort_->setBaudRate(serialConfig_.baudRate);
    serialPort_->setDataBits(static_cast<QSerialPort::DataBits>(serialConfig_.dataBits));

    if (serialConfig_.stopBits == 1) {
        serialPort_->setStopBits(QSerialPort::OneStop);
    } else if (serialConfig_.stopBits == 2) {
        serialPort_->setStopBits(QSerialPort::TwoStop);
    }

    if (serialConfig_.parity == "None") {
        serialPort_->setParity(QSerialPort::NoParity);
    } else if (serialConfig_.parity == "Even") {
        serialPort_->setParity(QSerialPort::EvenParity);
    } else if (serialConfig_.parity == "Odd") {
        serialPort_->setParity(QSerialPort::OddParity);
    }

    if (!serialPort_->open(QIODevice::ReadWrite)) {
        LOG_ERROR(QString("串口打开失败: %1 - %2")
                  .arg(serialConfig_.portName)
                  .arg(serialPort_->errorString()));
        return false;
    }

    LOG_INFO(QString("串口打开成功: %1").arg(serialConfig_.portName));
    emit serialStateChanged(true);
    return true;
}

void AlignmentOutputTool::closeSerial()
{
    if (serialPort_ && serialPort_->isOpen()) {
        serialPort_->close();
        LOG_INFO("串口已关闭");
        emit serialStateChanged(false);
    }
}

bool AlignmentOutputTool::isSerialOpen() const
{
    return serialPort_ && serialPort_->isOpen();
}

// ========== 私有方法 ==========

QByteArray AlignmentOutputTool::formatData(const QString& format) const
{
    if (format == "JSON") {
        return formatJSON();
    } else if (format == "CSV") {
        return formatCSV();
    } else if (format == "Binary") {
        return formatBinary();
    } else if (format == "Custom") {
        return formatCustom(tcpConfig_.customFormat);
    }
    return formatCSV();
}

QByteArray AlignmentOutputTool::formatJSON() const
{
    QJsonObject obj;
    obj["offsetX"] = offsetX_;
    obj["offsetY"] = offsetY_;
    obj["offsetTheta"] = offsetTheta_;
    obj["result"] = judgeResult_ ? "OK" : "NG";
    obj["timestamp"] = alignmentResult_.timestamp;

    if (!alignmentResult_.pointResults.isEmpty()) {
        obj["successPoints"] = alignmentResult_.successPoints;
        obj["failedPoints"] = alignmentResult_.failedPoints;
        obj["residualError"] = alignmentResult_.residualError;
    }

    QJsonDocument doc(obj);
    return doc.toJson(QJsonDocument::Compact);
}

QByteArray AlignmentOutputTool::formatCSV() const
{
    QString csv;
    csv += QString::number(offsetX_, 'f', 4) + tcpConfig_.delimiter;
    csv += QString::number(offsetY_, 'f', 4) + tcpConfig_.delimiter;
    csv += QString::number(offsetTheta_, 'f', 4) + tcpConfig_.delimiter;
    csv += (judgeResult_ ? "1" : "0");
    return csv.toUtf8();
}

QByteArray AlignmentOutputTool::formatBinary() const
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::LittleEndian);
    stream.setFloatingPointPrecision(QDataStream::SinglePrecision);

    stream << static_cast<float>(offsetX_);
    stream << static_cast<float>(offsetY_);
    stream << static_cast<float>(offsetTheta_);
    stream << static_cast<quint8>(judgeResult_ ? 1 : 0);

    return data;
}

QByteArray AlignmentOutputTool::formatCustom(const QString& format) const
{
    QString result = format;

    // 替换占位符
    result.replace("{X}", QString::number(offsetX_, 'f', 4));
    result.replace("{Y}", QString::number(offsetY_, 'f', 4));
    result.replace("{T}", QString::number(offsetTheta_, 'f', 4));
    result.replace("{THETA}", QString::number(offsetTheta_, 'f', 4));
    result.replace("{R}", judgeResult_ ? "OK" : "NG");
    result.replace("{RESULT}", judgeResult_ ? "1" : "0");

    // 支持整数格式
    result.replace("{XI}", QString::number(static_cast<int>(offsetX_ * 1000)));
    result.replace("{YI}", QString::number(static_cast<int>(offsetY_ * 1000)));
    result.replace("{TI}", QString::number(static_cast<int>(offsetTheta_ * 1000)));

    return result.toUtf8();
}

bool AlignmentOutputTool::writeModbusRegisters(int startAddr, const QVector<quint16>& values)
{
    if (!plcSocket_ || plcSocket_->state() != QAbstractSocket::ConnectedState) {
        return false;
    }

    // 构建Modbus TCP帧
    QByteArray frame;

    // 事务标识符（2字节）
    static quint16 transactionId = 0;
    transactionId++;
    frame.append(static_cast<char>((transactionId >> 8) & 0xFF));
    frame.append(static_cast<char>(transactionId & 0xFF));

    // 协议标识符（2字节，始终为0）
    frame.append(static_cast<char>(0x00));
    frame.append(static_cast<char>(0x00));

    // 长度（2字节）
    int dataLen = 7 + values.size() * 2;
    frame.append(static_cast<char>((dataLen >> 8) & 0xFF));
    frame.append(static_cast<char>(dataLen & 0xFF));

    // 单元标识符（1字节）
    frame.append(static_cast<char>(0x01));

    // 功能码：0x10 写多个寄存器
    frame.append(static_cast<char>(0x10));

    // 起始地址（2字节）
    frame.append(static_cast<char>((startAddr >> 8) & 0xFF));
    frame.append(static_cast<char>(startAddr & 0xFF));

    // 寄存器数量（2字节）
    frame.append(static_cast<char>((values.size() >> 8) & 0xFF));
    frame.append(static_cast<char>(values.size() & 0xFF));

    // 字节数（1字节）
    frame.append(static_cast<char>(values.size() * 2));

    // 数据
    for (quint16 val : values) {
        frame.append(static_cast<char>((val >> 8) & 0xFF));
        frame.append(static_cast<char>(val & 0xFF));
    }

    // 发送
    if (plcSocket_->write(frame) != frame.size()) {
        LOG_ERROR("Modbus发送失败");
        return false;
    }

    // 等待响应
    if (!plcSocket_->waitForReadyRead(1000)) {
        LOG_ERROR("Modbus响应超时");
        return false;
    }

    QByteArray response = plcSocket_->readAll();

    // 检查响应
    if (response.size() < 12) {
        LOG_ERROR("Modbus响应长度不足");
        return false;
    }

    // 检查功能码（异常时为原功能码+0x80）
    if (static_cast<quint8>(response[7]) == 0x90) {
        quint8 errorCode = static_cast<quint8>(response[8]);
        LOG_ERROR(QString("Modbus异常: 0x%1").arg(errorCode, 2, 16, QChar('0')));
        return false;
    }

    LOG_DEBUG("Modbus写入成功");
    return true;
}

bool AlignmentOutputTool::writeModbusFloat(int addr, float value)
{
    // 将float拆分为两个16位寄存器
    union {
        float f;
        quint32 u;
    } converter;
    converter.f = value;

    QVector<quint16> values;
    values.append(static_cast<quint16>((converter.u >> 16) & 0xFFFF));
    values.append(static_cast<quint16>(converter.u & 0xFFFF));

    return writeModbusRegisters(addr, values);
}

quint16 AlignmentOutputTool::calculateChecksum(const QByteArray& data) const
{
    quint16 sum = 0;
    for (char c : data) {
        sum += static_cast<quint8>(c);
    }
    return sum;
}

} // namespace Algorithm
} // namespace VisionForge
