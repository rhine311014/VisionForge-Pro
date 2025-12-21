/**
 * @file PLCClient.h
 * @brief 工业级PLC客户端
 * @author VisionForge Team
 * @date 2025-12-21
 *
 * 基于Qt 6 QTcpSocket的异步PLC通信客户端，
 * 支持自动重连、心跳检测、高级读写接口。
 */

#pragma once

#include "plc/PLCTypes.h"
#include "plc/PLCProtocol.h"

#include <QObject>
#include <QTcpSocket>
#include <QTimer>
#include <QMutex>
#include <QMutexLocker>
#include <QQueue>
#include <QElapsedTimer>
#include <QThread>

#include <memory>
#include <functional>
#include <atomic>

namespace VisionForge {
namespace PLC {

// ============================================================
// 前向声明
// ============================================================
class PLCClientPrivate;

// ============================================================
// 读写请求结构
// ============================================================

/**
 * @brief 异步请求结构
 */
struct AsyncRequest {
    int id = 0;                         ///< 请求ID
    QByteArray data;                    ///< 请求数据
    RegisterType regType = RegisterType::Unknown;
    int address = 0;
    int count = 0;
    bool isWrite = false;
    QElapsedTimer timer;                ///< 计时器
    std::function<void(const PLCResult&)> callback;
};

// ============================================================
// PLC客户端类
// ============================================================

/**
 * @class PLCClient
 * @brief 工业级PLC通信客户端
 *
 * 特性：
 * - 基于QTcpSocket的异步非阻塞通信
 * - 自动重连机制
 * - 心跳检测
 * - 高级读写接口 (int16/int32/float)
 * - 信号槽机制
 * - 线程安全
 * - 通信统计
 */
class PLCClient : public QObject
{
    Q_OBJECT
    Q_PROPERTY(CommStatus status READ status NOTIFY statusChanged)
    Q_PROPERTY(bool connected READ isConnected NOTIFY connectedChanged)

public:
    /**
     * @brief 构造函数
     * @param parent 父对象
     */
    explicit PLCClient(QObject* parent = nullptr);

    /**
     * @brief 析构函数
     */
    ~PLCClient() override;

    // ============================================================
    // 连接管理
    // ============================================================

    /**
     * @brief 连接到PLC
     * @param config 连接配置
     * @return 成功返回true
     */
    bool connectToPLC(const PLCConfig& config);

    /**
     * @brief 异步连接到PLC
     * @param config 连接配置
     */
    void connectToPLCAsync(const PLCConfig& config);

    /**
     * @brief 断开连接
     */
    void disconnectFromPLC();

    /**
     * @brief 是否已连接
     */
    bool isConnected() const;

    /**
     * @brief 获取连接状态
     */
    CommStatus status() const;

    /**
     * @brief 获取当前配置
     */
    const PLCConfig& config() const { return config_; }

    // ============================================================
    // 同步读取接口
    // ============================================================

    /**
     * @brief 读取单个int16值
     * @param regType 寄存器类型
     * @param address 地址
     * @return 读取结果
     */
    PLCResult readInt16(RegisterType regType, int address);

    /**
     * @brief 读取多个int16值
     * @param regType 寄存器类型
     * @param address 起始地址
     * @param count 读取数量
     * @return 读取结果
     */
    PLCResult readInt16Array(RegisterType regType, int address, int count);

    /**
     * @brief 读取单个int32值
     * @param regType 寄存器类型
     * @param address 地址
     * @return 读取结果
     */
    PLCResult readInt32(RegisterType regType, int address);

    /**
     * @brief 读取多个int32值
     * @param regType 寄存器类型
     * @param address 起始地址
     * @param count 读取数量
     * @return 读取结果
     */
    PLCResult readInt32Array(RegisterType regType, int address, int count);

    /**
     * @brief 读取单个float值
     * @param regType 寄存器类型
     * @param address 地址
     * @return 读取结果
     */
    PLCResult readFloat(RegisterType regType, int address);

    /**
     * @brief 读取多个float值
     * @param regType 寄存器类型
     * @param address 起始地址
     * @param count 读取数量
     * @return 读取结果
     */
    PLCResult readFloatArray(RegisterType regType, int address, int count);

    /**
     * @brief 读取单个位
     * @param regType 寄存器类型
     * @param address 地址
     * @return 读取结果
     */
    PLCResult readBit(RegisterType regType, int address);

    /**
     * @brief 读取多个位
     * @param regType 寄存器类型
     * @param address 起始地址
     * @param count 读取数量
     * @return 读取结果
     */
    PLCResult readBitArray(RegisterType regType, int address, int count);

    /**
     * @brief 读取字符串
     * @param regType 寄存器类型
     * @param address 起始地址
     * @param length 字符长度
     * @return 读取结果
     */
    PLCResult readString(RegisterType regType, int address, int length);

    // ============================================================
    // 同步写入接口
    // ============================================================

    /**
     * @brief 写入单个int16值
     * @param regType 寄存器类型
     * @param address 地址
     * @param value 值
     * @return 写入结果
     */
    PLCResult writeInt16(RegisterType regType, int address, int16_t value);

    /**
     * @brief 写入多个int16值
     * @param regType 寄存器类型
     * @param address 起始地址
     * @param values 值列表
     * @return 写入结果
     */
    PLCResult writeInt16Array(RegisterType regType, int address, const std::vector<int16_t>& values);

    /**
     * @brief 写入单个int32值
     * @param regType 寄存器类型
     * @param address 地址
     * @param value 值
     * @return 写入结果
     */
    PLCResult writeInt32(RegisterType regType, int address, int32_t value);

    /**
     * @brief 写入多个int32值
     * @param regType 寄存器类型
     * @param address 起始地址
     * @param values 值列表
     * @return 写入结果
     */
    PLCResult writeInt32Array(RegisterType regType, int address, const std::vector<int32_t>& values);

    /**
     * @brief 写入单个float值
     * @param regType 寄存器类型
     * @param address 地址
     * @param value 值
     * @return 写入结果
     */
    PLCResult writeFloat(RegisterType regType, int address, float value);

    /**
     * @brief 写入多个float值
     * @param regType 寄存器类型
     * @param address 起始地址
     * @param values 值列表
     * @return 写入结果
     */
    PLCResult writeFloatArray(RegisterType regType, int address, const std::vector<float>& values);

    /**
     * @brief 写入单个位
     * @param regType 寄存器类型
     * @param address 地址
     * @param value 值
     * @return 写入结果
     */
    PLCResult writeBit(RegisterType regType, int address, bool value);

    /**
     * @brief 写入多个位
     * @param regType 寄存器类型
     * @param address 起始地址
     * @param values 值列表
     * @return 写入结果
     */
    PLCResult writeBitArray(RegisterType regType, int address, const std::vector<bool>& values);

    /**
     * @brief 写入字符串
     * @param regType 寄存器类型
     * @param address 起始地址
     * @param value 字符串值
     * @return 写入结果
     */
    PLCResult writeString(RegisterType regType, int address, const QString& value);

    // ============================================================
    // 异步读取接口
    // ============================================================

    /**
     * @brief 异步读取int16数组
     * @param regType 寄存器类型
     * @param address 起始地址
     * @param count 读取数量
     * @param callback 完成回调
     * @return 请求ID
     */
    int readInt16Async(RegisterType regType, int address, int count,
                       std::function<void(const PLCResult&)> callback);

    /**
     * @brief 异步读取int32数组
     */
    int readInt32Async(RegisterType regType, int address, int count,
                       std::function<void(const PLCResult&)> callback);

    /**
     * @brief 异步读取float数组
     */
    int readFloatAsync(RegisterType regType, int address, int count,
                       std::function<void(const PLCResult&)> callback);

    /**
     * @brief 异步读取位数组
     */
    int readBitAsync(RegisterType regType, int address, int count,
                     std::function<void(const PLCResult&)> callback);

    // ============================================================
    // 异步写入接口
    // ============================================================

    /**
     * @brief 异步写入int16数组
     */
    int writeInt16Async(RegisterType regType, int address, const std::vector<int16_t>& values,
                        std::function<void(const PLCResult&)> callback);

    /**
     * @brief 异步写入int32数组
     */
    int writeInt32Async(RegisterType regType, int address, const std::vector<int32_t>& values,
                        std::function<void(const PLCResult&)> callback);

    /**
     * @brief 异步写入float数组
     */
    int writeFloatAsync(RegisterType regType, int address, const std::vector<float>& values,
                        std::function<void(const PLCResult&)> callback);

    /**
     * @brief 异步写入位数组
     */
    int writeBitAsync(RegisterType regType, int address, const std::vector<bool>& values,
                      std::function<void(const PLCResult&)> callback);

    // ============================================================
    // 心跳和统计
    // ============================================================

    /**
     * @brief 启动心跳检测
     */
    void startHeartbeat();

    /**
     * @brief 停止心跳检测
     */
    void stopHeartbeat();

    /**
     * @brief 心跳是否运行
     */
    bool isHeartbeatRunning() const;

    /**
     * @brief 获取通信统计
     */
    CommStatistics statistics() const;

    /**
     * @brief 重置统计
     */
    void resetStatistics();

    /**
     * @brief 获取协议对象
     */
    PLCProtocol* protocol() const { return protocol_.get(); }

    // ============================================================
    // 重连控制
    // ============================================================

    /**
     * @brief 启用/禁用自动重连
     */
    void setAutoReconnect(bool enable);

    /**
     * @brief 是否启用自动重连
     */
    bool autoReconnect() const;

    /**
     * @brief 立即尝试重连
     */
    void reconnectNow();

signals:
    // ============================================================
    // 信号
    // ============================================================

    /**
     * @brief 连接状态变化信号
     * @param status 新状态
     */
    void statusChanged(CommStatus status);

    /**
     * @brief 已连接信号
     */
    void connected();

    /**
     * @brief 已断开信号
     */
    void disconnected();

    /**
     * @brief 连接状态变化信号
     * @param connected 是否已连接
     */
    void connectedChanged(bool connected);

    /**
     * @brief 连接错误信号
     * @param errorCode 错误码
     * @param errorMessage 错误信息
     */
    void connectionError(int errorCode, const QString& errorMessage);

    /**
     * @brief 数据接收信号
     * @param result 接收的数据
     */
    void dataReceived(const PLCResult& result);

    /**
     * @brief 通信错误信号
     * @param errorCode 错误码
     * @param errorMessage 错误信息
     */
    void communicationError(int errorCode, const QString& errorMessage);

    /**
     * @brief 心跳信号
     * @param alive 心跳是否正常
     */
    void heartbeatStatus(bool alive);

    /**
     * @brief 重连尝试信号
     * @param attempt 尝试次数
     */
    void reconnectAttempt(int attempt);

    /**
     * @brief 重连成功信号
     */
    void reconnected();

private slots:
    // ============================================================
    // 内部槽函数
    // ============================================================

    void onSocketConnected();
    void onSocketDisconnected();
    void onSocketError(QAbstractSocket::SocketError error);
    void onSocketReadyRead();
    void onHeartbeatTimeout();
    void onReconnectTimeout();
    void onResponseTimeout();

private:
    // ============================================================
    // 私有方法
    // ============================================================

    /**
     * @brief 初始化协议
     */
    void initProtocol();

    /**
     * @brief 设置连接状态
     */
    void setStatus(CommStatus status);

    /**
     * @brief 发送请求并等待响应 (同步)
     */
    PLCResult sendRequest(const QByteArray& request, RegisterType regType);

    /**
     * @brief 发送异步请求
     */
    int sendAsyncRequest(const QByteArray& request, RegisterType regType,
                         int address, int count, bool isWrite,
                         std::function<void(const PLCResult&)> callback);

    /**
     * @brief 处理响应数据
     */
    void processResponse();

    /**
     * @brief 更新统计信息
     */
    void updateStatistics(bool success, bool isRead, qint64 responseTime);

    /**
     * @brief 尝试重连
     */
    void tryReconnect();

    /**
     * @brief 生成请求ID
     */
    int generateRequestId();

private:
    // ============================================================
    // 成员变量
    // ============================================================

    // 配置
    PLCConfig config_;

    // 网络
    QTcpSocket* socket_ = nullptr;

    // 协议
    std::unique_ptr<PLCProtocol> protocol_;

    // 状态
    std::atomic<CommStatus> status_{CommStatus::Disconnected};
    mutable QMutex statusMutex_;

    // 接收缓冲区
    QByteArray receiveBuffer_;
    mutable QMutex bufferMutex_;

    // 请求队列
    QQueue<AsyncRequest> requestQueue_;
    AsyncRequest currentRequest_;
    bool hasCurrentRequest_ = false;
    mutable QMutex requestMutex_;
    std::atomic<int> requestIdCounter_{0};

    // 定时器
    QTimer* heartbeatTimer_ = nullptr;
    QTimer* reconnectTimer_ = nullptr;
    QTimer* responseTimer_ = nullptr;

    // 重连
    std::atomic<bool> autoReconnect_{true};
    int reconnectAttempts_ = 0;

    // 统计
    CommStatistics statistics_;
    mutable QMutex statisticsMutex_;

    // 同步读写用
    QByteArray syncResponse_;
    bool syncResponseReceived_ = false;
    QMutex syncMutex_;
};

// ============================================================
// PLC客户端管理器
// ============================================================

/**
 * @class PLCClientManager
 * @brief PLC客户端管理器 (单例)
 *
 * 管理多个PLC连接实例
 */
class PLCClientManager : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief 获取单例实例
     */
    static PLCClientManager& instance();

    /**
     * @brief 创建PLC客户端
     * @param name 连接名称
     * @param config 配置
     * @return 客户端指针
     */
    PLCClient* createClient(const QString& name, const PLCConfig& config);

    /**
     * @brief 获取客户端
     * @param name 连接名称
     * @return 客户端指针，不存在返回nullptr
     */
    PLCClient* getClient(const QString& name) const;

    /**
     * @brief 移除客户端
     * @param name 连接名称
     */
    void removeClient(const QString& name);

    /**
     * @brief 获取所有客户端名称
     */
    QStringList clientNames() const;

    /**
     * @brief 断开所有连接
     */
    void disconnectAll();

    /**
     * @brief 获取客户端数量
     */
    int clientCount() const;

private:
    PLCClientManager(QObject* parent = nullptr);
    ~PLCClientManager() override;
    Q_DISABLE_COPY(PLCClientManager)

private:
    QMap<QString, PLCClient*> clients_;
    mutable QMutex mutex_;
};

} // namespace PLC
} // namespace VisionForge
