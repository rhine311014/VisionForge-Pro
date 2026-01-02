/**
 * @file ModbusTCPComm.cpp
 * @brief Modbus TCP通信协议实现文件
 * @author VisionForge Team
 * @date 2025-12-17
 *
 * @details
 * 本文件实现了Modbus TCP客户端的完整功能。
 *
 * ## 实现特点
 * - 使用Qt的QTcpSocket进行网络通信
 * - 支持所有常用的Modbus功能码
 * - 完善的异常处理和错误报告
 * - 线程安全的请求/响应机制
 *
 * ## 通信流程
 * 1. 建立TCP连接
 * 2. 构建Modbus请求帧（包含MBAP头和PDU）
 * 3. 发送请求并等待响应
 * 4. 解析响应帧，提取数据或错误信息
 *
 * ## 错误处理
 * - 连接超时：等待连接建立超时
 * - 发送超时：数据发送超时
 * - 接收超时：等待响应超时
 * - 协议错误：Transaction ID不匹配、功能码不匹配
 * - 异常响应：PLC返回的Modbus异常码
 */

#include "comm/ModbusTCPComm.h"
#include "base/Logger.h"
#include <QDataStream>

namespace VisionForge {
namespace Comm {

/**
 * @namespace ModbusFunction
 * @brief Modbus功能码定义
 *
 * @details
 * 定义了Modbus协议中使用的标准功能码。
 *
 * ## 功能码分类
 * - 读取类（0x01-0x04）：用于读取线圈和寄存器
 * - 写入类（0x05-0x06）：用于写入单个线圈或寄存器
 * - 批量写入类（0x0F-0x10）：用于写入多个线圈或寄存器
 */
namespace ModbusFunction {
    constexpr quint8 ReadCoils = 0x01;              ///< 读取线圈（输出线圈）
    constexpr quint8 ReadDiscreteInputs = 0x02;     ///< 读取离散输入（输入触点）
    constexpr quint8 ReadHoldingRegisters = 0x03;   ///< 读取保持寄存器
    constexpr quint8 ReadInputRegisters = 0x04;     ///< 读取输入寄存器
    constexpr quint8 WriteSingleCoil = 0x05;        ///< 写单个线圈
    constexpr quint8 WriteSingleRegister = 0x06;    ///< 写单个寄存器
    constexpr quint8 WriteMultipleCoils = 0x0F;     ///< 写多个线圈
    constexpr quint8 WriteMultipleRegisters = 0x10; ///< 写多个寄存器
}

/**
 * @brief 构造函数
 * @param parent 父对象指针
 *
 * @details
 * 初始化Modbus TCP通信对象：
 * 1. 创建TCP套接字
 * 2. 初始化事务ID计数器
 * 3. 设置默认从站地址为1
 * 4. 连接套接字信号（connected/disconnected/errorOccurred）
 */
ModbusTCPComm::ModbusTCPComm(QObject* parent)
    : PLCComm(parent)
    , socket_(new QTcpSocket(this))
    , transactionId_(0)
    , slaveId_(1)
{
    // 设置协议类型
    protocol_ = PLCProtocol::ModbusTCP;

    // 连接TCP连接成功信号
    QObject::connect(socket_, &QTcpSocket::connected, this, [this]() {
        LOG_INFO("Modbus TCP已连接");
        emit connectionChanged(true);
    });

    // 连接TCP断开信号
    QObject::connect(socket_, &QTcpSocket::disconnected, this, [this]() {
        LOG_INFO("Modbus TCP已断开");
        emit connectionChanged(false);
    });

    // 连接TCP错误信号
    QObject::connect(socket_, &QTcpSocket::errorOccurred, this, [this](QAbstractSocket::SocketError error) {
        Q_UNUSED(error);
        lastError_ = socket_->errorString();
        LOG_ERROR(QString("Modbus TCP错误: %1").arg(lastError_));
        emit errorOccurred(lastError_);
    });
}

/**
 * @brief 析构函数
 *
 * @details
 * 确保在对象销毁前断开TCP连接。
 */
ModbusTCPComm::~ModbusTCPComm()
{
    disconnect();
}

/**
 * @brief 连接到Modbus TCP服务器
 * @param config PLC配置
 * @return 连接成功返回true
 *
 * @details
 * 连接流程：
 * 1. 从配置中提取从站地址
 * 2. 获取并验证SocketConfig
 * 3. 使用阻塞方式建立TCP连接
 * 4. 等待连接完成或超时
 *
 * ## 错误情况
 * - 配置无效：commConfig不是SocketConfig类型
 * - 连接超时：在指定时间内无法建立连接
 */
bool ModbusTCPComm::connect(const PLCConfig& config)
{
    QMutexLocker locker(&mutex_);

    // 保存配置
    config_ = config;
    slaveId_ = static_cast<quint8>(config.slaveId);

    // 获取Socket配置
    auto socketConfig = std::dynamic_pointer_cast<SocketConfig>(config.commConfig);
    if (!socketConfig) {
        lastError_ = "无效的网络配置";
        LOG_ERROR(lastError_);
        return false;
    }

    // 设置超时时间
    timeout_ = socketConfig->timeout;

    LOG_INFO(QString("正在连接Modbus TCP: %1:%2").arg(socketConfig->ipAddress).arg(socketConfig->port));

    // 建立TCP连接
    socket_->connectToHost(socketConfig->ipAddress, socketConfig->port);

    // 等待连接完成
    if (!socket_->waitForConnected(timeout_)) {
        lastError_ = QString("连接超时: %1").arg(socket_->errorString());
        LOG_ERROR(lastError_);
        return false;
    }

    LOG_INFO("Modbus TCP连接成功");
    return true;
}

/**
 * @brief 断开TCP连接
 *
 * @details
 * 断开流程：
 * 1. 检查当前连接状态
 * 2. 调用disconnectFromHost()发起断开
 * 3. 等待断开完成（最多1秒）
 */
void ModbusTCPComm::disconnect()
{
    QMutexLocker locker(&mutex_);

    if (socket_->state() != QAbstractSocket::UnconnectedState) {
        socket_->disconnectFromHost();
        // 如果还未断开，等待断开完成
        if (socket_->state() != QAbstractSocket::UnconnectedState) {
            socket_->waitForDisconnected(1000);
        }
    }
}

/**
 * @brief 检查连接状态
 * @return 已连接返回true
 */
bool ModbusTCPComm::isConnected() const
{
    return socket_->state() == QAbstractSocket::ConnectedState;
}

/**
 * @brief 读取保持寄存器
 * @param address 起始地址
 * @param count 读取数量
 * @return 读取结果
 *
 * @details
 * 使用功能码0x03读取保持寄存器。
 *
 * ## 请求帧格式
 * | 字节 | 内容 | 说明 |
 * |------|------|------|
 * | 0-1 | 起始地址 | 大端序 |
 * | 2-3 | 寄存器数量 | 大端序 |
 *
 * ## 响应帧格式
 * | 字节 | 内容 | 说明 |
 * |------|------|------|
 * | 0 | 字节数 | N*2 |
 * | 1-N*2 | 寄存器值 | 大端序，每个2字节 |
 *
 * @note Modbus协议限制单次最多读取125个寄存器
 */
PLCResult ModbusTCPComm::readRegisters(int address, int count)
{
    QMutexLocker locker(&mutex_);

    PLCResult result;

    // 检查连接状态
    if (!isConnected()) {
        result.status = CommStatus::Disconnected;
        result.errorMessage = "未连接";
        return result;
    }

    // 验证参数：Modbus协议限制单次最多读取125个寄存器
    if (count <= 0 || count > 125) {
        result.status = CommStatus::InvalidParam;
        result.errorMessage = "读取数量无效 (1-125)";
        return result;
    }

    // 构建PDU数据：起始地址(2字节) + 寄存器数量(2字节)
    QByteArray pduData;
    QDataStream stream(&pduData, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);  // Modbus使用大端序
    stream << static_cast<quint16>(address);
    stream << static_cast<quint16>(count);

    // 构建完整请求帧并发送
    QByteArray request = buildFrame(ModbusFunction::ReadHoldingRegisters, pduData);
    QByteArray response = sendRequest(request);

    // 解析响应
    return parseResponse(response, ModbusFunction::ReadHoldingRegisters);
}

/**
 * @brief 写入保持寄存器
 * @param address 起始地址
 * @param values 要写入的值
 * @return 写入结果
 *
 * @details
 * 根据写入数量选择不同的功能码：
 * - 单个值：功能码0x06 (Write Single Register)
 * - 多个值：功能码0x10 (Write Multiple Registers)
 *
 * ## 写单个寄存器请求帧
 * | 字节 | 内容 |
 * |------|------|
 * | 0-1 | 寄存器地址 |
 * | 2-3 | 寄存器值 |
 *
 * ## 写多个寄存器请求帧
 * | 字节 | 内容 |
 * |------|------|
 * | 0-1 | 起始地址 |
 * | 2-3 | 寄存器数量 |
 * | 4 | 字节数 |
 * | 5-N | 寄存器值 |
 */
PLCResult ModbusTCPComm::writeRegisters(int address, const std::vector<int>& values)
{
    QMutexLocker locker(&mutex_);

    PLCResult result;

    // 检查连接状态
    if (!isConnected()) {
        result.status = CommStatus::Disconnected;
        result.errorMessage = "未连接";
        return result;
    }

    // 验证参数：Modbus协议限制单次最多写入123个寄存器
    if (values.empty() || values.size() > 123) {
        result.status = CommStatus::InvalidParam;
        result.errorMessage = "写入数量无效 (1-123)";
        return result;
    }

    QByteArray pduData;
    QDataStream stream(&pduData, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);

    if (values.size() == 1) {
        // 写单个寄存器 - 功能码0x06
        stream << static_cast<quint16>(address);
        stream << static_cast<quint16>(values[0] & 0xFFFF);  // 截断为16位

        QByteArray request = buildFrame(ModbusFunction::WriteSingleRegister, pduData);
        QByteArray response = sendRequest(request);

        return parseResponse(response, ModbusFunction::WriteSingleRegister);
    } else {
        // 写多个寄存器 - 功能码0x10
        stream << static_cast<quint16>(address);
        stream << static_cast<quint16>(values.size());
        stream << static_cast<quint8>(values.size() * 2);  // 字节数

        // 写入每个寄存器值
        for (int value : values) {
            stream << static_cast<quint16>(value & 0xFFFF);
        }

        QByteArray request = buildFrame(ModbusFunction::WriteMultipleRegisters, pduData);
        QByteArray response = sendRequest(request);

        return parseResponse(response, ModbusFunction::WriteMultipleRegisters);
    }
}

/**
 * @brief 读取线圈
 * @param address 起始地址
 * @param count 读取数量
 * @return 读取结果
 *
 * @details
 * 使用功能码0x01读取线圈状态。
 *
 * ## 响应数据格式
 * 线圈状态以字节打包，每个字节包含8个线圈状态。
 * 低位在前，即bit0对应第一个线圈。
 *
 * @note Modbus协议限制单次最多读取2000个线圈
 */
PLCResult ModbusTCPComm::readCoils(int address, int count)
{
    QMutexLocker locker(&mutex_);

    PLCResult result;

    // 检查连接状态
    if (!isConnected()) {
        result.status = CommStatus::Disconnected;
        result.errorMessage = "未连接";
        return result;
    }

    // 验证参数：Modbus协议限制单次最多读取2000个线圈
    if (count <= 0 || count > 2000) {
        result.status = CommStatus::InvalidParam;
        result.errorMessage = "读取数量无效 (1-2000)";
        return result;
    }

    // 构建读取线圈请求
    QByteArray pduData;
    QDataStream stream(&pduData, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);
    stream << static_cast<quint16>(address);
    stream << static_cast<quint16>(count);

    QByteArray request = buildFrame(ModbusFunction::ReadCoils, pduData);
    QByteArray response = sendRequest(request);

    return parseResponse(response, ModbusFunction::ReadCoils);
}

/**
 * @brief 写入线圈
 * @param address 起始地址
 * @param values 要写入的布尔值数组
 * @return 写入结果
 *
 * @details
 * 根据写入数量选择不同的功能码：
 * - 单个值：功能码0x05 (Write Single Coil)
 *   - ON: 发送0xFF00
 *   - OFF: 发送0x0000
 * - 多个值：功能码0x0F (Write Multiple Coils)
 *   - 布尔值打包为字节，低位在前
 *
 * @note Modbus协议限制单次最多写入1968个线圈
 */
PLCResult ModbusTCPComm::writeCoils(int address, const std::vector<bool>& values)
{
    QMutexLocker locker(&mutex_);

    PLCResult result;

    // 检查连接状态
    if (!isConnected()) {
        result.status = CommStatus::Disconnected;
        result.errorMessage = "未连接";
        return result;
    }

    // 验证参数
    if (values.empty() || values.size() > 1968) {
        result.status = CommStatus::InvalidParam;
        result.errorMessage = "写入数量无效 (1-1968)";
        return result;
    }

    QByteArray pduData;
    QDataStream stream(&pduData, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);

    if (values.size() == 1) {
        // 写单个线圈 - 功能码0x05
        // ON = 0xFF00, OFF = 0x0000
        stream << static_cast<quint16>(address);
        stream << static_cast<quint16>(values[0] ? 0xFF00 : 0x0000);

        QByteArray request = buildFrame(ModbusFunction::WriteSingleCoil, pduData);
        QByteArray response = sendRequest(request);

        return parseResponse(response, ModbusFunction::WriteSingleCoil);
    } else {
        // 写多个线圈 - 功能码0x0F
        int byteCount = (values.size() + 7) / 8;  // 向上取整
        stream << static_cast<quint16>(address);
        stream << static_cast<quint16>(values.size());
        stream << static_cast<quint8>(byteCount);

        // 将布尔值打包为字节（低位在前）
        for (int i = 0; i < byteCount; ++i) {
            quint8 byte = 0;
            for (int j = 0; j < 8 && i * 8 + j < static_cast<int>(values.size()); ++j) {
                if (values[i * 8 + j]) {
                    byte |= (1 << j);  // 设置对应位
                }
            }
            stream << byte;
        }

        QByteArray request = buildFrame(ModbusFunction::WriteMultipleCoils, pduData);
        QByteArray response = sendRequest(request);

        return parseResponse(response, ModbusFunction::WriteMultipleCoils);
    }
}

/**
 * @brief 读取输入寄存器
 * @param address 起始地址
 * @param count 读取数量
 * @return 读取结果
 *
 * @details
 * 使用功能码0x04读取输入寄存器。
 * 输入寄存器是只读的，通常用于模拟量输入等。
 */
PLCResult ModbusTCPComm::readInputRegisters(int address, int count)
{
    QMutexLocker locker(&mutex_);

    PLCResult result;

    if (!isConnected()) {
        result.status = CommStatus::Disconnected;
        result.errorMessage = "未连接";
        return result;
    }

    if (count <= 0 || count > 125) {
        result.status = CommStatus::InvalidParam;
        result.errorMessage = "读取数量无效 (1-125)";
        return result;
    }

    // 构建读取输入寄存器请求
    QByteArray pduData;
    QDataStream stream(&pduData, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);
    stream << static_cast<quint16>(address);
    stream << static_cast<quint16>(count);

    QByteArray request = buildFrame(ModbusFunction::ReadInputRegisters, pduData);
    QByteArray response = sendRequest(request);

    return parseResponse(response, ModbusFunction::ReadInputRegisters);
}

/**
 * @brief 读取离散输入
 * @param address 起始地址
 * @param count 读取数量
 * @return 读取结果
 *
 * @details
 * 使用功能码0x02读取离散输入。
 * 离散输入是只读的位元件，通常连接外部开关等。
 */
PLCResult ModbusTCPComm::readDiscreteInputs(int address, int count)
{
    QMutexLocker locker(&mutex_);

    PLCResult result;

    if (!isConnected()) {
        result.status = CommStatus::Disconnected;
        result.errorMessage = "未连接";
        return result;
    }

    if (count <= 0 || count > 2000) {
        result.status = CommStatus::InvalidParam;
        result.errorMessage = "读取数量无效 (1-2000)";
        return result;
    }

    // 构建读取离散输入请求
    QByteArray pduData;
    QDataStream stream(&pduData, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);
    stream << static_cast<quint16>(address);
    stream << static_cast<quint16>(count);

    QByteArray request = buildFrame(ModbusFunction::ReadDiscreteInputs, pduData);
    QByteArray response = sendRequest(request);

    return parseResponse(response, ModbusFunction::ReadDiscreteInputs);
}

/**
 * @brief 发送请求并接收响应
 * @param request 请求帧数据
 * @return 响应帧数据
 *
 * @details
 * 通信流程：
 * 1. 发送请求数据
 * 2. 等待发送完成
 * 3. 等待响应到达
 * 4. 读取MBAP头（7字节）
 * 5. 根据MBAP头中的长度字段读取剩余数据
 *
 * ## MBAP头解析
 * - 字节4-5：后续数据长度（包含Unit ID）
 * - 需要读取的剩余数据 = length - 1
 */
QByteArray ModbusTCPComm::sendRequest(const QByteArray& request)
{
    // 检查套接字状态
    if (!socket_->isValid()) {
        return QByteArray();
    }

    // 发送请求数据
    socket_->write(request);
    if (!socket_->waitForBytesWritten(timeout_)) {
        LOG_ERROR("发送请求超时");
        return QByteArray();
    }

    // 等待响应数据到达
    if (!socket_->waitForReadyRead(timeout_)) {
        LOG_ERROR("接收响应超时");
        return QByteArray();
    }

    // 读取响应数据
    QByteArray response;

    // 首先读取MBAP头（7字节）
    while (response.size() < 7) {
        if (!socket_->waitForReadyRead(timeout_)) {
            break;
        }
        response.append(socket_->read(7 - response.size()));
    }

    if (response.size() < 7) {
        LOG_ERROR("MBAP头接收不完整");
        return QByteArray();
    }

    // 从MBAP头获取数据长度（字节4-5，大端序）
    quint16 length = (static_cast<quint8>(response[4]) << 8) | static_cast<quint8>(response[5]);

    // 读取剩余数据（减去已包含在length中的Unit ID）
    int remaining = length - 1;
    while (response.size() < 7 + remaining) {
        if (!socket_->waitForReadyRead(timeout_)) {
            break;
        }
        response.append(socket_->read(7 + remaining - response.size()));
    }

    return response;
}

/**
 * @brief 构建Modbus TCP请求帧
 * @param functionCode 功能码
 * @param data PDU数据
 * @return 完整的Modbus TCP帧
 *
 * @details
 * Modbus TCP帧结构：
 * ```
 * | Transaction ID | Protocol ID | Length  | Unit ID | Function | Data  |
 * |    2 bytes     |   2 bytes   | 2 bytes | 1 byte  | 1 byte   | N bytes|
 * ```
 *
 * - Transaction ID: 自动递增，用于匹配响应
 * - Protocol ID: 固定为0x0000（Modbus协议）
 * - Length: Unit ID + Function Code + Data的长度
 * - Unit ID: 从站地址
 * - Function: 功能码
 * - Data: PDU数据
 */
QByteArray ModbusTCPComm::buildFrame(quint8 functionCode, const QByteArray& data)
{
    QByteArray frame;
    QDataStream stream(&frame, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);

    // MBAP Header
    stream << ++transactionId_;                          // 事务ID（自增）
    stream << static_cast<quint16>(0);                   // 协议ID（固定为0）
    stream << static_cast<quint16>(data.size() + 2);     // 长度（Unit ID + Function Code + Data）
    stream << slaveId_;                                   // 从站地址

    // PDU
    stream << functionCode;                               // 功能码
    frame.append(data);                                   // 数据

    return frame;
}

/**
 * @brief 解析Modbus响应帧
 * @param response 响应数据
 * @param expectedFunctionCode 期望的功能码
 * @return 解析结果
 *
 * @details
 * 解析流程：
 * 1. 验证响应最小长度（9字节）
 * 2. 检查Transaction ID是否匹配
 * 3. 检查是否为异常响应（功能码最高位为1）
 * 4. 验证功能码是否匹配
 * 5. 根据功能码类型解析数据
 *
 * ## 数据位置
 * - response[0-1]: Transaction ID
 * - response[2-3]: Protocol ID
 * - response[4-5]: Length
 * - response[6]: Unit ID
 * - response[7]: Function Code
 * - response[8...]: Data
 */
PLCResult ModbusTCPComm::parseResponse(const QByteArray& response, quint8 expectedFunctionCode)
{
    PLCResult result;

    // 验证最小响应长度
    if (response.size() < 9) {
        result.status = CommStatus::Error;
        result.errorMessage = "响应数据不完整";
        return result;
    }

    // 检查Transaction ID是否匹配
    quint16 txId = (static_cast<quint8>(response[0]) << 8) | static_cast<quint8>(response[1]);
    if (txId != transactionId_) {
        result.status = CommStatus::Error;
        result.errorMessage = "Transaction ID不匹配";
        return result;
    }

    // 获取功能码
    quint8 functionCode = static_cast<quint8>(response[7]);

    // 检查异常响应（功能码最高位为1表示异常）
    if (functionCode & 0x80) {
        quint8 exceptionCode = static_cast<quint8>(response[8]);
        result.status = CommStatus::Error;
        result.errorMessage = getExceptionMessage(exceptionCode);
        return result;
    }

    // 验证功能码是否匹配
    if (functionCode != expectedFunctionCode) {
        result.status = CommStatus::Error;
        result.errorMessage = QString("功能码不匹配: 期望0x%1, 收到0x%2")
                                  .arg(expectedFunctionCode, 2, 16, QChar('0'))
                                  .arg(functionCode, 2, 16, QChar('0'));
        return result;
    }

    // 解析成功，设置状态
    result.status = CommStatus::Success;

    // 根据功能码类型解析数据
    switch (functionCode) {
    case ModbusFunction::ReadHoldingRegisters:
    case ModbusFunction::ReadInputRegisters:
    {
        // 寄存器读取响应：字节数在response[8]，后续是寄存器值
        int byteCount = static_cast<quint8>(response[8]);
        int registerCount = byteCount / 2;

        for (int i = 0; i < registerCount; ++i) {
            int offset = 9 + i * 2;
            if (offset + 1 < response.size()) {
                // 大端序：高字节在前
                quint16 value = (static_cast<quint8>(response[offset]) << 8) |
                                static_cast<quint8>(response[offset + 1]);
                result.intValues.push_back(static_cast<int>(value));
            }
        }
        break;
    }

    case ModbusFunction::ReadCoils:
    case ModbusFunction::ReadDiscreteInputs:
    {
        // 线圈/离散输入读取响应：字节数在response[8]，后续是打包的位数据
        int byteCount = static_cast<quint8>(response[8]);

        for (int i = 0; i < byteCount; ++i) {
            int offset = 9 + i;
            if (offset < response.size()) {
                quint8 byte = static_cast<quint8>(response[offset]);
                // 解包每个字节中的8个位
                for (int j = 0; j < 8; ++j) {
                    result.boolValues.push_back((byte >> j) & 0x01);
                }
            }
        }
        break;
    }

    case ModbusFunction::WriteSingleCoil:
    case ModbusFunction::WriteSingleRegister:
    case ModbusFunction::WriteMultipleCoils:
    case ModbusFunction::WriteMultipleRegisters:
        // 写入操作：响应表示成功即可，无需解析数据
        break;
    }

    return result;
}

/**
 * @brief 获取Modbus异常码的错误描述
 * @param exceptionCode 异常码
 * @return 错误描述字符串
 *
 * @details
 * Modbus标准定义的异常码：
 * - 0x01: 非法功能码 - 从站不支持该功能
 * - 0x02: 非法数据地址 - 地址越界
 * - 0x03: 非法数据值 - 值超出允许范围
 * - 0x04: 从站设备故障 - 内部错误
 * - 0x05: 确认 - 请求已接受但需要较长时间处理
 * - 0x06: 从站设备忙 - 正在处理其他请求
 * - 0x08: 存储奇偶性差错 - 内存校验错误
 * - 0x0A: 网关路径不可用 - 网关配置错误
 * - 0x0B: 网关目标设备未响应 - 目标设备无响应
 */
QString ModbusTCPComm::getExceptionMessage(quint8 exceptionCode)
{
    switch (exceptionCode) {
    case 0x01: return "非法功能码";
    case 0x02: return "非法数据地址";
    case 0x03: return "非法数据值";
    case 0x04: return "从站设备故障";
    case 0x05: return "确认";
    case 0x06: return "从站设备忙";
    case 0x08: return "存储奇偶性差错";
    case 0x0A: return "网关路径不可用";
    case 0x0B: return "网关目标设备未响应";
    default:   return QString("未知异常码: 0x%1").arg(exceptionCode, 2, 16, QChar('0'));
    }
}

} // namespace Comm
} // namespace VisionForge
