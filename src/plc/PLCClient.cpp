/**
 * @file PLCClient.cpp
 * @brief PLC客户端实现
 * @author VisionForge Team
 * @date 2025-12-21
 *
 * 基于Qt 6 QTcpSocket的异步PLC通信客户端实现
 */

#include "plc/PLCClient.h"
#include <QEventLoop>
#include <QThread>
#include <QCoreApplication>

namespace VisionForge {
namespace PLC {

// ============================================================
// PLCClient 实现
// ============================================================

PLCClient::PLCClient(QObject* parent)
    : QObject(parent)
{
    // 创建Socket
    socket_ = new QTcpSocket(this);

    // 创建定时器
    heartbeatTimer_ = new QTimer(this);
    reconnectTimer_ = new QTimer(this);
    responseTimer_ = new QTimer(this);

    // 配置定时器
    heartbeatTimer_->setTimerType(Qt::PreciseTimer);
    reconnectTimer_->setSingleShot(true);
    responseTimer_->setSingleShot(true);

    // 连接Socket信号
    connect(socket_, &QTcpSocket::connected,
            this, &PLCClient::onSocketConnected);
    connect(socket_, &QTcpSocket::disconnected,
            this, &PLCClient::onSocketDisconnected);
    connect(socket_, &QTcpSocket::errorOccurred,
            this, &PLCClient::onSocketError);
    connect(socket_, &QTcpSocket::readyRead,
            this, &PLCClient::onSocketReadyRead);

    // 连接定时器信号
    connect(heartbeatTimer_, &QTimer::timeout,
            this, &PLCClient::onHeartbeatTimeout);
    connect(reconnectTimer_, &QTimer::timeout,
            this, &PLCClient::onReconnectTimeout);
    connect(responseTimer_, &QTimer::timeout,
            this, &PLCClient::onResponseTimeout);

    // 初始化统计
    statistics_.startTime = QDateTime::currentDateTime();
}

PLCClient::~PLCClient()
{
    disconnectFromPLC();
}

void PLCClient::initProtocol()
{
    protocol_ = PLCProtocolFactory::create(config_.protocol, this);
    if (protocol_) {
        protocol_->setConfig(config_);

        // 设置MC协议特有参数
        if (config_.protocol == ProtocolType::MitsubishiMC3E) {
            auto* mcProtocol = dynamic_cast<MitsubishiMCProtocol*>(protocol_.get());
            if (mcProtocol) {
                mcProtocol->setNetworkNo(config_.networkNo);
                mcProtocol->setPCNo(config_.pcNo);
                mcProtocol->setDestModuleIO(config_.destModuleIo);
                mcProtocol->setDestModuleStation(config_.destModuleStation);
                mcProtocol->setMonitoringTimer(config_.monitoringTimer);
            }
        }
    }
}

bool PLCClient::connectToPLC(const PLCConfig& config)
{
    if (!config.isValid()) {
        emit connectionError(ErrorCode::InvalidParameter, QStringLiteral("Invalid configuration"));
        return false;
    }

    config_ = config;
    initProtocol();

    if (!protocol_) {
        emit connectionError(ErrorCode::ProtocolError,
                           QStringLiteral("Unsupported protocol: %1").arg(getProtocolTypeName(config.protocol)));
        return false;
    }

    setStatus(CommStatus::Connecting);
    statistics_.totalConnections++;

    socket_->connectToHost(config_.ipAddress, config_.port);

    // 等待连接完成
    if (!socket_->waitForConnected(config_.connectTimeout)) {
        setStatus(CommStatus::Error);
        statistics_.failedConnections++;
        emit connectionError(ErrorCode::ConnectionFailed,
                           QStringLiteral("Connection timeout: %1").arg(socket_->errorString()));
        return false;
    }

    return true;
}

void PLCClient::connectToPLCAsync(const PLCConfig& config)
{
    if (!config.isValid()) {
        emit connectionError(ErrorCode::InvalidParameter, QStringLiteral("Invalid configuration"));
        return;
    }

    config_ = config;
    initProtocol();

    if (!protocol_) {
        emit connectionError(ErrorCode::ProtocolError,
                           QStringLiteral("Unsupported protocol: %1").arg(getProtocolTypeName(config.protocol)));
        return;
    }

    setStatus(CommStatus::Connecting);
    statistics_.totalConnections++;

    socket_->connectToHost(config_.ipAddress, config_.port);
}

void PLCClient::disconnectFromPLC()
{
    stopHeartbeat();
    reconnectTimer_->stop();
    responseTimer_->stop();

    if (socket_->state() != QAbstractSocket::UnconnectedState) {
        socket_->disconnectFromHost();
        if (socket_->state() != QAbstractSocket::UnconnectedState) {
            socket_->waitForDisconnected(1000);
        }
    }

    setStatus(CommStatus::Disconnected);
}

bool PLCClient::isConnected() const
{
    return status_.load() == CommStatus::Connected;
}

CommStatus PLCClient::status() const
{
    return status_.load();
}

void PLCClient::setStatus(CommStatus status)
{
    CommStatus oldStatus = status_.exchange(status);
    if (oldStatus != status) {
        emit statusChanged(status);

        if (status == CommStatus::Connected) {
            emit connected();
            emit connectedChanged(true);
        } else if (status == CommStatus::Disconnected) {
            emit disconnected();
            emit connectedChanged(false);
        }
    }
}

// ============================================================
// 同步读取实现
// ============================================================

PLCResult PLCClient::sendRequest(const QByteArray& request, RegisterType regType)
{
    if (!isConnected()) {
        return PLCResult::Error(ErrorCode::NotConnected, QStringLiteral("Not connected to PLC"));
    }

    QElapsedTimer timer;
    timer.start();

    // 发送请求
    qint64 written = socket_->write(request);
    if (written != request.size()) {
        return PLCResult::Error(ErrorCode::DataError, QStringLiteral("Failed to send request"));
    }

    if (!socket_->waitForBytesWritten(config_.responseTimeout)) {
        return PLCResult::Error(ErrorCode::Timeout, QStringLiteral("Write timeout"));
    }

    {
        QMutexLocker locker(&statisticsMutex_);
        statistics_.bytesSent += written;
    }

    // 等待响应
    QByteArray response;
    while (true) {
        if (!socket_->waitForReadyRead(config_.responseTimeout)) {
            return PLCResult::Error(ErrorCode::Timeout, QStringLiteral("Read timeout"));
        }

        response.append(socket_->readAll());

        // 检查响应是否完整
        if (protocol_ && protocol_->isResponseComplete(response)) {
            break;
        }

        // 超时检查
        if (timer.elapsed() > config_.responseTimeout) {
            return PLCResult::Error(ErrorCode::Timeout, QStringLiteral("Response timeout"));
        }
    }

    {
        QMutexLocker locker(&statisticsMutex_);
        statistics_.bytesReceived += response.size();
    }

    // 解析响应
    PLCResult result = protocol_->parseResponse(response, regType);
    result.responseTime = timer.elapsed();

    // 更新统计
    updateStatistics(result.success, true, result.responseTime);

    return result;
}

PLCResult PLCClient::readInt16(RegisterType regType, int address)
{
    return readInt16Array(regType, address, 1);
}

PLCResult PLCClient::readInt16Array(RegisterType regType, int address, int count)
{
    if (!protocol_) {
        return PLCResult::Error(ErrorCode::ProtocolError, QStringLiteral("Protocol not initialized"));
    }

    QByteArray request = protocol_->packReadRequest(regType, address, count);
    PLCResult result = sendRequest(request, regType);

    if (result.success) {
        // uint16转换为int16
        result.int16Values.clear();
        for (uint16_t val : result.uint16Values) {
            result.int16Values.push_back(static_cast<int16_t>(val));
        }
    }

    return result;
}

PLCResult PLCClient::readInt32(RegisterType regType, int address)
{
    return readInt32Array(regType, address, 1);
}

PLCResult PLCClient::readInt32Array(RegisterType regType, int address, int count)
{
    if (!protocol_) {
        return PLCResult::Error(ErrorCode::ProtocolError, QStringLiteral("Protocol not initialized"));
    }

    // 读取2*count个寄存器
    QByteArray request = protocol_->packReadRequest(regType, address, count * 2);
    PLCResult result = sendRequest(request, regType);

    if (result.success && !result.uint16Values.empty()) {
        // 转换为int32
        result.int32Values = PLCProtocol::toInt32(result.uint16Values, config_.byteOrder);
    }

    return result;
}

PLCResult PLCClient::readFloat(RegisterType regType, int address)
{
    return readFloatArray(regType, address, 1);
}

PLCResult PLCClient::readFloatArray(RegisterType regType, int address, int count)
{
    if (!protocol_) {
        return PLCResult::Error(ErrorCode::ProtocolError, QStringLiteral("Protocol not initialized"));
    }

    // 读取2*count个寄存器
    QByteArray request = protocol_->packReadRequest(regType, address, count * 2);
    PLCResult result = sendRequest(request, regType);

    if (result.success && !result.uint16Values.empty()) {
        // 转换为float
        result.floatValues = PLCProtocol::toFloat(result.uint16Values, config_.byteOrder);
    }

    return result;
}

PLCResult PLCClient::readBit(RegisterType regType, int address)
{
    return readBitArray(regType, address, 1);
}

PLCResult PLCClient::readBitArray(RegisterType regType, int address, int count)
{
    if (!protocol_) {
        return PLCResult::Error(ErrorCode::ProtocolError, QStringLiteral("Protocol not initialized"));
    }

    QByteArray request = protocol_->packReadRequest(regType, address, count);
    return sendRequest(request, regType);
}

PLCResult PLCClient::readString(RegisterType regType, int address, int length)
{
    if (!protocol_) {
        return PLCResult::Error(ErrorCode::ProtocolError, QStringLiteral("Protocol not initialized"));
    }

    // 计算需要读取的寄存器数量
    int regCount = (length + 1) / 2;
    QByteArray request = protocol_->packReadRequest(regType, address, regCount);
    PLCResult result = sendRequest(request, regType);

    if (result.success && !result.uint16Values.empty()) {
        // 转换为字符串
        QByteArray bytes;
        for (uint16_t val : result.uint16Values) {
            if (config_.byteOrder == ByteOrder::BigEndian) {
                bytes.append(static_cast<char>((val >> 8) & 0xFF));
                bytes.append(static_cast<char>(val & 0xFF));
            } else {
                bytes.append(static_cast<char>(val & 0xFF));
                bytes.append(static_cast<char>((val >> 8) & 0xFF));
            }
        }
        // 截取指定长度并移除尾部空字符
        bytes = bytes.left(length);
        while (!bytes.isEmpty() && bytes.back() == '\0') {
            bytes.chop(1);
        }
        result.stringValue = QString::fromLatin1(bytes);
    }

    return result;
}

// ============================================================
// 同步写入实现
// ============================================================

PLCResult PLCClient::writeInt16(RegisterType regType, int address, int16_t value)
{
    return writeInt16Array(regType, address, {value});
}

PLCResult PLCClient::writeInt16Array(RegisterType regType, int address, const std::vector<int16_t>& values)
{
    if (!protocol_) {
        return PLCResult::Error(ErrorCode::ProtocolError, QStringLiteral("Protocol not initialized"));
    }

    // 转换为uint16
    std::vector<uint16_t> uint16Values;
    uint16Values.reserve(values.size());
    for (int16_t val : values) {
        uint16Values.push_back(static_cast<uint16_t>(val));
    }

    QByteArray request = protocol_->packWriteRequest(regType, address, uint16Values);
    PLCResult result = sendRequest(request, regType);

    updateStatistics(result.success, false, result.responseTime);
    return result;
}

PLCResult PLCClient::writeInt32(RegisterType regType, int address, int32_t value)
{
    return writeInt32Array(regType, address, {value});
}

PLCResult PLCClient::writeInt32Array(RegisterType regType, int address, const std::vector<int32_t>& values)
{
    if (!protocol_) {
        return PLCResult::Error(ErrorCode::ProtocolError, QStringLiteral("Protocol not initialized"));
    }

    // 转换为uint16数组
    std::vector<uint16_t> uint16Values = PLCProtocol::fromInt32(values, config_.byteOrder);

    QByteArray request = protocol_->packWriteRequest(regType, address, uint16Values);
    PLCResult result = sendRequest(request, regType);

    updateStatistics(result.success, false, result.responseTime);
    return result;
}

PLCResult PLCClient::writeFloat(RegisterType regType, int address, float value)
{
    return writeFloatArray(regType, address, {value});
}

PLCResult PLCClient::writeFloatArray(RegisterType regType, int address, const std::vector<float>& values)
{
    if (!protocol_) {
        return PLCResult::Error(ErrorCode::ProtocolError, QStringLiteral("Protocol not initialized"));
    }

    // 转换为uint16数组
    std::vector<uint16_t> uint16Values = PLCProtocol::fromFloat(values, config_.byteOrder);

    QByteArray request = protocol_->packWriteRequest(regType, address, uint16Values);
    PLCResult result = sendRequest(request, regType);

    updateStatistics(result.success, false, result.responseTime);
    return result;
}

PLCResult PLCClient::writeBit(RegisterType regType, int address, bool value)
{
    return writeBitArray(regType, address, {value});
}

PLCResult PLCClient::writeBitArray(RegisterType regType, int address, const std::vector<bool>& values)
{
    if (!protocol_) {
        return PLCResult::Error(ErrorCode::ProtocolError, QStringLiteral("Protocol not initialized"));
    }

    QByteArray request = protocol_->packWriteBitRequest(regType, address, values);
    PLCResult result = sendRequest(request, regType);

    updateStatistics(result.success, false, result.responseTime);
    return result;
}

PLCResult PLCClient::writeString(RegisterType regType, int address, const QString& value)
{
    if (!protocol_) {
        return PLCResult::Error(ErrorCode::ProtocolError, QStringLiteral("Protocol not initialized"));
    }

    // 转换为uint16数组
    QByteArray bytes = value.toLatin1();
    if (bytes.size() % 2 != 0) {
        bytes.append('\0');
    }

    std::vector<uint16_t> uint16Values;
    for (int i = 0; i < bytes.size(); i += 2) {
        uint16_t val;
        if (config_.byteOrder == ByteOrder::BigEndian) {
            val = (static_cast<uint8_t>(bytes[i]) << 8) |
                  static_cast<uint8_t>(bytes[i + 1]);
        } else {
            val = (static_cast<uint8_t>(bytes[i + 1]) << 8) |
                  static_cast<uint8_t>(bytes[i]);
        }
        uint16Values.push_back(val);
    }

    QByteArray request = protocol_->packWriteRequest(regType, address, uint16Values);
    PLCResult result = sendRequest(request, regType);

    updateStatistics(result.success, false, result.responseTime);
    return result;
}

// ============================================================
// 异步读写实现
// ============================================================

int PLCClient::generateRequestId()
{
    return ++requestIdCounter_;
}

int PLCClient::sendAsyncRequest(const QByteArray& request, RegisterType regType,
                                 int address, int count, bool isWrite,
                                 std::function<void(const PLCResult&)> callback)
{
    if (!isConnected()) {
        PLCResult error = PLCResult::Error(ErrorCode::NotConnected, QStringLiteral("Not connected"));
        if (callback) {
            callback(error);
        }
        return -1;
    }

    AsyncRequest asyncReq;
    asyncReq.id = generateRequestId();
    asyncReq.data = request;
    asyncReq.regType = regType;
    asyncReq.address = address;
    asyncReq.count = count;
    asyncReq.isWrite = isWrite;
    asyncReq.callback = callback;
    asyncReq.timer.start();

    {
        QMutexLocker locker(&requestMutex_);
        requestQueue_.enqueue(asyncReq);
    }

    // 如果当前没有进行中的请求，立即发送
    if (!hasCurrentRequest_) {
        processResponse();
    }

    return asyncReq.id;
}

int PLCClient::readInt16Async(RegisterType regType, int address, int count,
                               std::function<void(const PLCResult&)> callback)
{
    if (!protocol_) {
        PLCResult error = PLCResult::Error(ErrorCode::ProtocolError, QStringLiteral("Protocol not initialized"));
        if (callback) callback(error);
        return -1;
    }

    QByteArray request = protocol_->packReadRequest(regType, address, count);
    return sendAsyncRequest(request, regType, address, count, false, callback);
}

int PLCClient::readInt32Async(RegisterType regType, int address, int count,
                               std::function<void(const PLCResult&)> callback)
{
    if (!protocol_) {
        PLCResult error = PLCResult::Error(ErrorCode::ProtocolError, QStringLiteral("Protocol not initialized"));
        if (callback) callback(error);
        return -1;
    }

    QByteArray request = protocol_->packReadRequest(regType, address, count * 2);
    return sendAsyncRequest(request, regType, address, count * 2, false,
        [callback, this](const PLCResult& result) {
            PLCResult modifiedResult = result;
            if (result.success && !result.uint16Values.empty()) {
                modifiedResult.int32Values = PLCProtocol::toInt32(result.uint16Values, config_.byteOrder);
            }
            if (callback) callback(modifiedResult);
        });
}

int PLCClient::readFloatAsync(RegisterType regType, int address, int count,
                               std::function<void(const PLCResult&)> callback)
{
    if (!protocol_) {
        PLCResult error = PLCResult::Error(ErrorCode::ProtocolError, QStringLiteral("Protocol not initialized"));
        if (callback) callback(error);
        return -1;
    }

    QByteArray request = protocol_->packReadRequest(regType, address, count * 2);
    return sendAsyncRequest(request, regType, address, count * 2, false,
        [callback, this](const PLCResult& result) {
            PLCResult modifiedResult = result;
            if (result.success && !result.uint16Values.empty()) {
                modifiedResult.floatValues = PLCProtocol::toFloat(result.uint16Values, config_.byteOrder);
            }
            if (callback) callback(modifiedResult);
        });
}

int PLCClient::readBitAsync(RegisterType regType, int address, int count,
                             std::function<void(const PLCResult&)> callback)
{
    if (!protocol_) {
        PLCResult error = PLCResult::Error(ErrorCode::ProtocolError, QStringLiteral("Protocol not initialized"));
        if (callback) callback(error);
        return -1;
    }

    QByteArray request = protocol_->packReadRequest(regType, address, count);
    return sendAsyncRequest(request, regType, address, count, false, callback);
}

int PLCClient::writeInt16Async(RegisterType regType, int address, const std::vector<int16_t>& values,
                                std::function<void(const PLCResult&)> callback)
{
    if (!protocol_) {
        PLCResult error = PLCResult::Error(ErrorCode::ProtocolError, QStringLiteral("Protocol not initialized"));
        if (callback) callback(error);
        return -1;
    }

    std::vector<uint16_t> uint16Values;
    uint16Values.reserve(values.size());
    for (int16_t val : values) {
        uint16Values.push_back(static_cast<uint16_t>(val));
    }

    QByteArray request = protocol_->packWriteRequest(regType, address, uint16Values);
    return sendAsyncRequest(request, regType, address, static_cast<int>(values.size()), true, callback);
}

int PLCClient::writeInt32Async(RegisterType regType, int address, const std::vector<int32_t>& values,
                                std::function<void(const PLCResult&)> callback)
{
    if (!protocol_) {
        PLCResult error = PLCResult::Error(ErrorCode::ProtocolError, QStringLiteral("Protocol not initialized"));
        if (callback) callback(error);
        return -1;
    }

    std::vector<uint16_t> uint16Values = PLCProtocol::fromInt32(values, config_.byteOrder);
    QByteArray request = protocol_->packWriteRequest(regType, address, uint16Values);
    return sendAsyncRequest(request, regType, address, static_cast<int>(uint16Values.size()), true, callback);
}

int PLCClient::writeFloatAsync(RegisterType regType, int address, const std::vector<float>& values,
                                std::function<void(const PLCResult&)> callback)
{
    if (!protocol_) {
        PLCResult error = PLCResult::Error(ErrorCode::ProtocolError, QStringLiteral("Protocol not initialized"));
        if (callback) callback(error);
        return -1;
    }

    std::vector<uint16_t> uint16Values = PLCProtocol::fromFloat(values, config_.byteOrder);
    QByteArray request = protocol_->packWriteRequest(regType, address, uint16Values);
    return sendAsyncRequest(request, regType, address, static_cast<int>(uint16Values.size()), true, callback);
}

int PLCClient::writeBitAsync(RegisterType regType, int address, const std::vector<bool>& values,
                              std::function<void(const PLCResult&)> callback)
{
    if (!protocol_) {
        PLCResult error = PLCResult::Error(ErrorCode::ProtocolError, QStringLiteral("Protocol not initialized"));
        if (callback) callback(error);
        return -1;
    }

    QByteArray request = protocol_->packWriteBitRequest(regType, address, values);
    return sendAsyncRequest(request, regType, address, static_cast<int>(values.size()), true, callback);
}

// ============================================================
// Socket槽函数
// ============================================================

void PLCClient::onSocketConnected()
{
    setStatus(CommStatus::Connected);
    statistics_.successfulConnections++;
    reconnectAttempts_ = 0;

    if (config_.enableHeartbeat) {
        startHeartbeat();
    }

    emit reconnected();
}

void PLCClient::onSocketDisconnected()
{
    stopHeartbeat();
    responseTimer_->stop();

    if (autoReconnect_.load() && status_.load() != CommStatus::Disconnected) {
        setStatus(CommStatus::Reconnecting);
        tryReconnect();
    } else {
        setStatus(CommStatus::Disconnected);
    }
}

void PLCClient::onSocketError(QAbstractSocket::SocketError error)
{
    QString errorMsg = socket_->errorString();

    {
        QMutexLocker locker(&statisticsMutex_);
        statistics_.lastError = errorMsg;
        statistics_.lastErrorTime = QDateTime::currentDateTime();
    }

    if (status_.load() == CommStatus::Connecting) {
        statistics_.failedConnections++;
    }

    emit connectionError(static_cast<int>(error), errorMsg);

    if (error != QAbstractSocket::RemoteHostClosedError) {
        setStatus(CommStatus::Error);
    }
}

void PLCClient::onSocketReadyRead()
{
    QMutexLocker locker(&bufferMutex_);
    receiveBuffer_.append(socket_->readAll());

    {
        QMutexLocker statsLocker(&statisticsMutex_);
        statistics_.bytesReceived += receiveBuffer_.size();
    }

    // 处理响应
    processResponse();
}

void PLCClient::processResponse()
{
    QMutexLocker locker(&bufferMutex_);

    // 检查是否有完整的响应
    if (!protocol_ || !protocol_->isResponseComplete(receiveBuffer_)) {
        return;
    }

    responseTimer_->stop();

    // 解析响应
    PLCResult result = protocol_->parseResponse(receiveBuffer_, currentRequest_.regType);
    result.responseTime = currentRequest_.timer.elapsed();

    // 清空缓冲区
    receiveBuffer_.clear();

    // 更新统计
    updateStatistics(result.success, !currentRequest_.isWrite, result.responseTime);

    // 发送信号
    emit dataReceived(result);

    // 调用回调
    if (hasCurrentRequest_ && currentRequest_.callback) {
        currentRequest_.callback(result);
    }

    hasCurrentRequest_ = false;

    // 处理队列中的下一个请求
    {
        QMutexLocker reqLocker(&requestMutex_);
        if (!requestQueue_.isEmpty()) {
            currentRequest_ = requestQueue_.dequeue();
            hasCurrentRequest_ = true;

            socket_->write(currentRequest_.data);
            responseTimer_->start(config_.responseTimeout);

            QMutexLocker statsLocker(&statisticsMutex_);
            statistics_.bytesSent += currentRequest_.data.size();
        }
    }
}

void PLCClient::onResponseTimeout()
{
    if (hasCurrentRequest_) {
        PLCResult error = PLCResult::Error(ErrorCode::Timeout, QStringLiteral("Response timeout"));

        if (currentRequest_.callback) {
            currentRequest_.callback(error);
        }

        hasCurrentRequest_ = false;

        {
            QMutexLocker locker(&bufferMutex_);
            receiveBuffer_.clear();
        }

        updateStatistics(false, !currentRequest_.isWrite, config_.responseTimeout);

        emit communicationError(ErrorCode::Timeout, QStringLiteral("Response timeout"));
    }
}

// ============================================================
// 心跳实现
// ============================================================

void PLCClient::startHeartbeat()
{
    if (!heartbeatTimer_->isActive()) {
        heartbeatTimer_->start(config_.heartbeatInterval);
    }
}

void PLCClient::stopHeartbeat()
{
    heartbeatTimer_->stop();
}

bool PLCClient::isHeartbeatRunning() const
{
    return heartbeatTimer_->isActive();
}

void PLCClient::onHeartbeatTimeout()
{
    if (!isConnected()) {
        return;
    }

    // 读取心跳寄存器
    PLCResult result = readInt16(config_.heartbeatRegister, config_.heartbeatAddress);

    emit heartbeatStatus(result.success);

    if (!result.success) {
        emit communicationError(result.errorCode, result.errorMessage);
    }
}

// ============================================================
// 重连实现
// ============================================================

void PLCClient::setAutoReconnect(bool enable)
{
    autoReconnect_.store(enable);
}

bool PLCClient::autoReconnect() const
{
    return autoReconnect_.load();
}

void PLCClient::tryReconnect()
{
    if (config_.maxReconnectAttempts >= 0 &&
        reconnectAttempts_ >= config_.maxReconnectAttempts) {
        setStatus(CommStatus::Error);
        emit connectionError(ErrorCode::ConnectionFailed,
                           QStringLiteral("Max reconnect attempts reached"));
        return;
    }

    reconnectAttempts_++;
    emit reconnectAttempt(reconnectAttempts_);

    reconnectTimer_->start(config_.reconnectInterval);
}

void PLCClient::onReconnectTimeout()
{
    if (status_.load() == CommStatus::Reconnecting) {
        statistics_.reconnectCount++;
        socket_->connectToHost(config_.ipAddress, config_.port);
    }
}

void PLCClient::reconnectNow()
{
    if (status_.load() != CommStatus::Connected) {
        reconnectAttempts_ = 0;
        tryReconnect();
    }
}

// ============================================================
// 统计实现
// ============================================================

void PLCClient::updateStatistics(bool success, bool isRead, qint64 responseTime)
{
    QMutexLocker locker(&statisticsMutex_);

    if (isRead) {
        statistics_.totalReadCount++;
        if (success) {
            statistics_.successReadCount++;
        } else {
            statistics_.failedReadCount++;
        }
    } else {
        statistics_.totalWriteCount++;
        if (success) {
            statistics_.successWriteCount++;
        } else {
            statistics_.failedWriteCount++;
        }
    }

    if (success) {
        statistics_.totalResponseTime += responseTime;
        if (responseTime < statistics_.minResponseTime) {
            statistics_.minResponseTime = responseTime;
        }
        if (responseTime > statistics_.maxResponseTime) {
            statistics_.maxResponseTime = responseTime;
        }
        statistics_.lastSuccessTime = QDateTime::currentDateTime();
    }
}

CommStatistics PLCClient::statistics() const
{
    QMutexLocker locker(&statisticsMutex_);
    return statistics_;
}

void PLCClient::resetStatistics()
{
    QMutexLocker locker(&statisticsMutex_);
    statistics_.reset();
}

// ============================================================
// PLCClientManager 实现
// ============================================================

PLCClientManager& PLCClientManager::instance()
{
    static PLCClientManager instance;
    return instance;
}

PLCClientManager::PLCClientManager(QObject* parent)
    : QObject(parent)
{
}

PLCClientManager::~PLCClientManager()
{
    disconnectAll();
}

PLCClient* PLCClientManager::createClient(const QString& name, const PLCConfig& config)
{
    QMutexLocker locker(&mutex_);

    // 如果已存在同名客户端，先删除
    if (clients_.contains(name)) {
        delete clients_.take(name);
    }

    PLCClient* client = new PLCClient(this);
    clients_.insert(name, client);

    // 自动连接
    client->connectToPLCAsync(config);

    return client;
}

PLCClient* PLCClientManager::getClient(const QString& name) const
{
    QMutexLocker locker(&mutex_);
    return clients_.value(name, nullptr);
}

void PLCClientManager::removeClient(const QString& name)
{
    QMutexLocker locker(&mutex_);
    if (clients_.contains(name)) {
        PLCClient* client = clients_.take(name);
        client->disconnectFromPLC();
        delete client;
    }
}

QStringList PLCClientManager::clientNames() const
{
    QMutexLocker locker(&mutex_);
    return clients_.keys();
}

void PLCClientManager::disconnectAll()
{
    QMutexLocker locker(&mutex_);
    for (PLCClient* client : clients_) {
        client->disconnectFromPLC();
        delete client;
    }
    clients_.clear();
}

int PLCClientManager::clientCount() const
{
    QMutexLocker locker(&mutex_);
    return clients_.size();
}

} // namespace PLC
} // namespace VisionForge
