/**
 * @file EtherCATMaster.h
 * @brief EtherCAT主站类
 * @author VisionForge Team
 * @date 2024
 *
 * 本文件定义了EtherCAT主站类，提供完整的EtherCAT主站功能：
 * - 网络扫描和从站发现
 * - 状态机管理
 * - PDO循环数据交换
 * - SDO参数访问
 * - 分布式时钟同步
 */

#ifndef VISIONFORGE_PLC_ETHERCAT_MASTER_H
#define VISIONFORGE_PLC_ETHERCAT_MASTER_H

#include "EtherCATTypes.h"
#include <QObject>
#include <QTimer>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QElapsedTimer>
#include <memory>
#include <atomic>

namespace VisionForge {
namespace PLC {

// 前向声明
class EtherCATWorker;

/**
 * @brief EtherCAT主站类
 *
 * 提供完整的EtherCAT主站功能，包括：
 * - 从站扫描和配置
 * - 状态机管理 (Init -> PreOp -> SafeOp -> Op)
 * - PDO过程数据循环交换
 * - SDO服务数据访问
 * - 分布式时钟(DC)同步
 * - 诊断和统计
 *
 * @note 需要管理员/root权限访问原始网络接口
 *
 * 使用示例:
 * @code
 * EtherCATMaster master;
 * ECMasterConfig config;
 * config.networkInterface = "eth0";
 * config.cycleTime = 1000;  // 1ms
 *
 * if (master.initialize(config)) {
 *     if (master.scanNetwork()) {
 *         master.configureSlaves();
 *         master.requestState(ECState::Operational);
 *         master.startCyclicExchange();
 *     }
 * }
 * @endcode
 */
class EtherCATMaster : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param parent 父对象
     */
    explicit EtherCATMaster(QObject* parent = nullptr);

    /**
     * @brief 析构函数
     */
    ~EtherCATMaster() override;

    // ============================================================
    // 初始化和连接
    // ============================================================

    /**
     * @brief 初始化主站
     * @param config 主站配置
     * @return 成功返回true
     */
    bool initialize(const ECMasterConfig& config);

    /**
     * @brief 关闭主站
     */
    void shutdown();

    /**
     * @brief 检查是否已初始化
     */
    bool isInitialized() const { return m_initialized.load(); }

    /**
     * @brief 获取主站配置
     */
    const ECMasterConfig& getConfig() const { return m_config; }

    /**
     * @brief 设置主站配置
     */
    void setConfig(const ECMasterConfig& config);

    // ============================================================
    // 网络扫描和从站管理
    // ============================================================

    /**
     * @brief 扫描网络上的从站
     * @return 发现的从站数量，-1表示失败
     */
    int scanNetwork();

    /**
     * @brief 获取从站数量
     */
    int getSlaveCount() const { return static_cast<int>(m_slaves.size()); }

    /**
     * @brief 获取从站信息
     * @param position 从站位置 (从1开始)
     * @return 从站信息指针，不存在返回nullptr
     */
    const ECSlaveInfo* getSlaveInfo(uint16_t position) const;

    /**
     * @brief 获取所有从站信息
     */
    const std::vector<ECSlaveInfo>& getAllSlaves() const { return m_slaves; }

    /**
     * @brief 按厂商ID和产品代码查找从站
     * @param vendorId 厂商ID
     * @param productCode 产品代码
     * @return 匹配的从站位置列表
     */
    std::vector<uint16_t> findSlaves(uint32_t vendorId, uint32_t productCode) const;

    // ============================================================
    // 状态机控制
    // ============================================================

    /**
     * @brief 请求所有从站进入指定状态
     * @param state 目标状态
     * @param timeout 超时时间 (ms)，0使用默认值
     * @return 成功返回true
     */
    bool requestState(ECState state, int timeout = 0);

    /**
     * @brief 请求单个从站进入指定状态
     * @param slavePos 从站位置
     * @param state 目标状态
     * @param timeout 超时时间 (ms)
     * @return 成功返回true
     */
    bool requestSlaveState(uint16_t slavePos, ECState state, int timeout = 0);

    /**
     * @brief 获取当前主站状态
     */
    ECMasterStatus getStatus() const { return m_status.load(); }

    /**
     * @brief 获取从站当前状态
     * @param slavePos 从站位置
     */
    ECState getSlaveState(uint16_t slavePos) const;

    /**
     * @brief 检查所有从站是否在指定状态
     */
    bool allSlavesInState(ECState state) const;

    /**
     * @brief 读取从站AL状态
     * @param slavePos 从站位置
     */
    ALStatusCode getSlaveALStatus(uint16_t slavePos) const;

    // ============================================================
    // 从站配置
    // ============================================================

    /**
     * @brief 配置所有从站
     * @return 成功返回true
     */
    bool configureSlaves();

    /**
     * @brief 配置单个从站
     * @param slavePos 从站位置
     * @return 成功返回true
     */
    bool configureSlave(uint16_t slavePos);

    /**
     * @brief 配置分布式时钟
     * @return 成功返回true
     */
    bool configureDC();

    /**
     * @brief 设置从站PDO映射
     * @param slavePos 从站位置
     * @param rxPDOs RxPDO映射
     * @param txPDOs TxPDO映射
     * @return 成功返回true
     */
    bool configurePDOMapping(uint16_t slavePos,
                             const std::vector<PDOMapping>& rxPDOs,
                             const std::vector<PDOMapping>& txPDOs);

    // ============================================================
    // 周期数据交换 (PDO)
    // ============================================================

    /**
     * @brief 启动周期数据交换
     * @return 成功返回true
     */
    bool startCyclicExchange();

    /**
     * @brief 停止周期数据交换
     */
    void stopCyclicExchange();

    /**
     * @brief 检查是否正在运行周期交换
     */
    bool isRunning() const { return m_running.load(); }

    /**
     * @brief 设置周期回调函数
     * @param callback 每个周期调用的回调
     */
    void setCyclicCallback(ECCyclicCallback callback);

    /**
     * @brief 获取输入数据 (从站->主站)
     * @return 输入数据缓冲区
     */
    QByteArray getInputs() const;

    /**
     * @brief 获取输出数据 (主站->从站)
     * @return 输出数据缓冲区
     */
    QByteArray getOutputs() const;

    /**
     * @brief 设置输出数据
     * @param outputs 输出数据
     */
    void setOutputs(const QByteArray& outputs);

    /**
     * @brief 获取IO映射总大小
     * @return {inputSize, outputSize}
     */
    std::pair<int, int> getIOMapSize() const;

    // ============================================================
    // 从站数据访问 (便捷接口)
    // ============================================================

    /**
     * @brief 读取从站输入数据
     * @param slavePos 从站位置
     * @return 输入数据
     */
    QByteArray readSlaveInputs(uint16_t slavePos) const;

    /**
     * @brief 读取从站输出数据
     * @param slavePos 从站位置
     * @return 输出数据
     */
    QByteArray readSlaveOutputs(uint16_t slavePos) const;

    /**
     * @brief 写入从站输出数据
     * @param slavePos 从站位置
     * @param data 输出数据
     * @return 成功返回true
     */
    bool writeSlaveOutputs(uint16_t slavePos, const QByteArray& data);

    /**
     * @brief 读取从站输入位
     * @param slavePos 从站位置
     * @param bitOffset 位偏移
     */
    bool readInputBit(uint16_t slavePos, int bitOffset) const;

    /**
     * @brief 写入从站输出位
     * @param slavePos 从站位置
     * @param bitOffset 位偏移
     * @param value 值
     */
    bool writeOutputBit(uint16_t slavePos, int bitOffset, bool value);

    /**
     * @brief 读取从站输入字节
     */
    uint8_t readInputByte(uint16_t slavePos, int byteOffset) const;

    /**
     * @brief 读取从站输入字
     */
    uint16_t readInputWord(uint16_t slavePos, int byteOffset) const;

    /**
     * @brief 读取从站输入双字
     */
    uint32_t readInputDWord(uint16_t slavePos, int byteOffset) const;

    /**
     * @brief 写入从站输出字节
     */
    bool writeOutputByte(uint16_t slavePos, int byteOffset, uint8_t value);

    /**
     * @brief 写入从站输出字
     */
    bool writeOutputWord(uint16_t slavePos, int byteOffset, uint16_t value);

    /**
     * @brief 写入从站输出双字
     */
    bool writeOutputDWord(uint16_t slavePos, int byteOffset, uint32_t value);

    // ============================================================
    // SDO访问 (参数配置)
    // ============================================================

    /**
     * @brief 同步SDO读取
     * @param slavePos 从站位置
     * @param index 对象字典索引
     * @param subIndex 子索引
     * @param timeout 超时时间 (ms)
     * @return SDO响应
     */
    SDOResponse sdoRead(uint16_t slavePos, uint16_t index, uint8_t subIndex,
                        int timeout = 0);

    /**
     * @brief 同步SDO写入
     * @param slavePos 从站位置
     * @param index 对象字典索引
     * @param subIndex 子索引
     * @param data 写入数据
     * @param timeout 超时时间 (ms)
     * @return SDO响应
     */
    SDOResponse sdoWrite(uint16_t slavePos, uint16_t index, uint8_t subIndex,
                         const QByteArray& data, int timeout = 0);

    /**
     * @brief 异步SDO读取
     * @param request SDO请求
     * @param callback 完成回调
     * @return 请求ID，-1表示失败
     */
    int sdoReadAsync(const SDORequest& request, ECSDOCallback callback);

    /**
     * @brief 异步SDO写入
     * @param request SDO请求
     * @param callback 完成回调
     * @return 请求ID，-1表示失败
     */
    int sdoWriteAsync(const SDORequest& request, ECSDOCallback callback);

    // SDO便捷读取方法
    template<typename T>
    T sdoReadValue(uint16_t slavePos, uint16_t index, uint8_t subIndex, T defaultValue = T());

    int8_t sdoReadInt8(uint16_t slavePos, uint16_t index, uint8_t subIndex);
    int16_t sdoReadInt16(uint16_t slavePos, uint16_t index, uint8_t subIndex);
    int32_t sdoReadInt32(uint16_t slavePos, uint16_t index, uint8_t subIndex);
    uint8_t sdoReadUInt8(uint16_t slavePos, uint16_t index, uint8_t subIndex);
    uint16_t sdoReadUInt16(uint16_t slavePos, uint16_t index, uint8_t subIndex);
    uint32_t sdoReadUInt32(uint16_t slavePos, uint16_t index, uint8_t subIndex);
    float sdoReadFloat(uint16_t slavePos, uint16_t index, uint8_t subIndex);
    QString sdoReadString(uint16_t slavePos, uint16_t index, uint8_t subIndex);

    // SDO便捷写入方法
    bool sdoWriteInt8(uint16_t slavePos, uint16_t index, uint8_t subIndex, int8_t value);
    bool sdoWriteInt16(uint16_t slavePos, uint16_t index, uint8_t subIndex, int16_t value);
    bool sdoWriteInt32(uint16_t slavePos, uint16_t index, uint8_t subIndex, int32_t value);
    bool sdoWriteUInt8(uint16_t slavePos, uint16_t index, uint8_t subIndex, uint8_t value);
    bool sdoWriteUInt16(uint16_t slavePos, uint16_t index, uint8_t subIndex, uint16_t value);
    bool sdoWriteUInt32(uint16_t slavePos, uint16_t index, uint8_t subIndex, uint32_t value);
    bool sdoWriteFloat(uint16_t slavePos, uint16_t index, uint8_t subIndex, float value);
    bool sdoWriteString(uint16_t slavePos, uint16_t index, uint8_t subIndex, const QString& value);

    // ============================================================
    // 诊断和统计
    // ============================================================

    /**
     * @brief 获取统计信息
     */
    ECStatistics getStatistics() const;

    /**
     * @brief 重置统计信息
     */
    void resetStatistics();

    /**
     * @brief 获取最后错误信息
     */
    QString getLastError() const { return m_lastError; }

    /**
     * @brief 获取诊断信息
     */
    QString getDiagnosticInfo() const;

    /**
     * @brief 读取从站错误计数器
     * @param slavePos 从站位置
     */
    ECResult readSlaveErrorCounters(uint16_t slavePos);

    /**
     * @brief 清除从站错误计数器
     * @param slavePos 从站位置
     */
    ECResult clearSlaveErrorCounters(uint16_t slavePos);

    // ============================================================
    // 持久化
    // ============================================================

    /**
     * @brief 导出配置到JSON
     */
    QJsonObject exportToJson() const;

    /**
     * @brief 从JSON导入配置
     */
    bool importFromJson(const QJsonObject& json);

    /**
     * @brief 保存配置到文件
     */
    bool saveToFile(const QString& filePath) const;

    /**
     * @brief 从文件加载配置
     */
    bool loadFromFile(const QString& filePath);

signals:
    /**
     * @brief 主站状态变化信号
     */
    void statusChanged(ECMasterStatus status);

    /**
     * @brief 从站状态变化信号
     */
    void slaveStateChanged(uint16_t slavePos, ECState oldState, ECState newState);

    /**
     * @brief 从站错误信号
     */
    void slaveError(uint16_t slavePos, int errorCode, const QString& message);

    /**
     * @brief 网络扫描完成信号
     */
    void scanCompleted(int slaveCount);

    /**
     * @brief 配置完成信号
     */
    void configurationCompleted(bool success);

    /**
     * @brief 周期交换启动信号
     */
    void cyclicExchangeStarted();

    /**
     * @brief 周期交换停止信号
     */
    void cyclicExchangeStopped();

    /**
     * @brief Workcounter错误信号
     */
    void workcounterError(int expected, int actual);

    /**
     * @brief 错误信号
     */
    void errorOccurred(int errorCode, const QString& message);

private slots:
    void onCyclicTimerTimeout();
    void onWatchdogTimeout();

private:
    // 内部方法
    bool initializeInterface();
    void closeInterface();
    bool sendReceiveFrame(QByteArray& frame);
    void updateSlaveStates();
    void processCyclicData();
    void handleSlaveError(uint16_t slavePos, ECState state);
    bool waitForState(ECState targetState, int timeout);
    bool configureMailbox(uint16_t slavePos);
    bool configureSyncManagers(uint16_t slavePos);
    bool configureFMMU(uint16_t slavePos);
    void setStatus(ECMasterStatus status);
    void setLastError(const QString& error);
    void stopCyclicExchangeInternal();  ///< 内部停止方法（无锁）

    // SDO内部方法
    SDOResponse sendSDORequest(uint16_t slavePos, uint16_t index, uint8_t subIndex,
                               SDOAccessType accessType, const QByteArray& data, int timeout);
    QByteArray buildSDOReadRequest(uint16_t index, uint8_t subIndex);
    QByteArray buildSDOWriteRequest(uint16_t index, uint8_t subIndex, const QByteArray& data);
    SDOResponse parseSDOResponse(const QByteArray& response);

    // 成员变量
    ECMasterConfig m_config;
    std::vector<ECSlaveInfo> m_slaves;
    std::atomic<ECMasterStatus> m_status{ECMasterStatus::Disconnected};
    std::atomic<bool> m_initialized{false};
    std::atomic<bool> m_running{false};

    // IO映射
    QByteArray m_inputBuffer;           ///< 输入数据缓冲区
    QByteArray m_outputBuffer;          ///< 输出数据缓冲区
    int m_inputMapSize = 0;             ///< 输入映射大小
    int m_outputMapSize = 0;            ///< 输出映射大小

    // 定时器
    QTimer* m_cyclicTimer = nullptr;
    QTimer* m_watchdogTimer = nullptr;

    // 回调
    ECCyclicCallback m_cyclicCallback;
    ECStateChangeCallback m_stateChangeCallback;
    ECErrorCallback m_errorCallback;

    // 同步
    mutable QMutex m_mutex;
    mutable QMutex m_ioMutex;
    QWaitCondition m_stateCondition;

    // 统计
    ECStatistics m_statistics;
    QElapsedTimer m_cycleTimer;

    // 错误信息
    QString m_lastError;

    // 工作线程
    QThread* m_workerThread = nullptr;
    EtherCATWorker* m_worker = nullptr;

    // SDO请求队列
    struct PendingSDORequest {
        int id;
        SDORequest request;
        ECSDOCallback callback;
        QElapsedTimer timer;
    };
    std::vector<PendingSDORequest> m_pendingSDORequests;
    std::atomic<int> m_sdoRequestIdCounter{0};
    mutable QMutex m_sdoMutex;

    // 网络接口句柄 (平台特定)
#ifdef Q_OS_WIN
    void* m_pcapHandle = nullptr;       ///< WinPcap/Npcap句柄
#else
    int m_rawSocket = -1;               ///< 原始套接字
#endif
};

// ============================================================
// 模板实现
// ============================================================

template<typename T>
T EtherCATMaster::sdoReadValue(uint16_t slavePos, uint16_t index,
                                uint8_t subIndex, T defaultValue)
{
    SDOResponse resp = sdoRead(slavePos, index, subIndex);
    if (!resp.success || resp.data.size() < static_cast<int>(sizeof(T))) {
        return defaultValue;
    }
    T value;
    memcpy(&value, resp.data.constData(), sizeof(T));
    return value;
}

// ============================================================
// EtherCAT工作线程
// ============================================================

/**
 * @brief EtherCAT周期工作线程
 */
class EtherCATWorker : public QObject
{
    Q_OBJECT

public:
    explicit EtherCATWorker(EtherCATMaster* master, QObject* parent = nullptr);
    ~EtherCATWorker() override;

    void start();
    void stop();
    bool isRunning() const { return m_running.load(); }

signals:
    void cyclicDataProcessed();
    void workcounterError(int expected, int actual);
    void error(int errorCode, const QString& message);

private slots:
    void processLoop();

private:
    EtherCATMaster* m_master;
    std::atomic<bool> m_running{false};
    std::atomic<bool> m_stopRequested{false};
};

// ============================================================
// EtherCAT主站管理器
// ============================================================

/**
 * @brief EtherCAT主站管理器 (单例)
 */
class EtherCATMasterManager
{
public:
    static EtherCATMasterManager& instance();

    /**
     * @brief 创建主站
     */
    EtherCATMaster* createMaster(const QString& name);

    /**
     * @brief 获取主站
     */
    EtherCATMaster* getMaster(const QString& name);

    /**
     * @brief 移除主站
     */
    bool removeMaster(const QString& name);

    /**
     * @brief 获取所有主站名称
     */
    QStringList getMasterNames() const;

    /**
     * @brief 列出可用的网络接口
     */
    static QStringList listNetworkInterfaces();

private:
    EtherCATMasterManager() = default;
    ~EtherCATMasterManager() = default;
    EtherCATMasterManager(const EtherCATMasterManager&) = delete;
    EtherCATMasterManager& operator=(const EtherCATMasterManager&) = delete;

    std::map<QString, std::unique_ptr<EtherCATMaster>> m_masters;
    mutable QMutex m_mutex;
};

}  // namespace PLC
}  // namespace VisionForge

#endif // VISIONFORGE_PLC_ETHERCAT_MASTER_H
