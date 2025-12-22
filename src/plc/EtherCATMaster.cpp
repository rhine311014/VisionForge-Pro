/**
 * @file EtherCATMaster.cpp
 * @brief EtherCAT主站实现
 * @author VisionForge Team
 * @date 2024
 */

#include "plc/EtherCATMaster.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDebug>
#include <QThread>
#include <QCoreApplication>
#include <QNetworkInterface>
#include <cstring>

#ifdef Q_OS_WIN
#include <winsock2.h>
#include <iphlpapi.h>
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")
#endif

namespace VisionForge {
namespace PLC {

// ============================================================
// EtherCAT帧常量
// ============================================================

namespace {

// EtherCAT帧类型
constexpr uint16_t ETHERCAT_TYPE = 0x88A4;

// 命令类型
enum ECCommand : uint8_t {
    NOP     = 0x00,     // 无操作
    APRD    = 0x01,     // 自动增量物理读
    APWR    = 0x02,     // 自动增量物理写
    APRW    = 0x03,     // 自动增量物理读写
    FPRD    = 0x04,     // 配置地址物理读
    FPWR    = 0x05,     // 配置地址物理写
    FPRW    = 0x06,     // 配置地址物理读写
    BRD     = 0x07,     // 广播读
    BWR     = 0x08,     // 广播写
    BRW     = 0x09,     // 广播读写
    LRD     = 0x0A,     // 逻辑读
    LWR     = 0x0B,     // 逻辑写
    LRW     = 0x0C,     // 逻辑读写
    ARMW    = 0x0D,     // 自动增量物理读多写
    FRMW    = 0x0E      // 配置地址物理读多写
};

// 寄存器地址
constexpr uint16_t REG_TYPE          = 0x0000;  // 类型
constexpr uint16_t REG_REVISION      = 0x0001;  // 修订
constexpr uint16_t REG_BUILD         = 0x0002;  // 构建
constexpr uint16_t REG_FMMU_COUNT    = 0x0004;  // FMMU数量
constexpr uint16_t REG_SM_COUNT      = 0x0005;  // SM数量
constexpr uint16_t REG_RAM_SIZE      = 0x0006;  // RAM大小
constexpr uint16_t REG_PORT_DESC     = 0x0007;  // 端口描述
constexpr uint16_t REG_ESC_FEATURES  = 0x0008;  // ESC特性
constexpr uint16_t REG_STATION_ADDR  = 0x0010;  // 站地址
constexpr uint16_t REG_STATION_ALIAS = 0x0012;  // 站别名

constexpr uint16_t REG_DL_CONTROL    = 0x0100;  // 数据链路控制
constexpr uint16_t REG_DL_STATUS     = 0x0110;  // 数据链路状态

constexpr uint16_t REG_AL_CONTROL    = 0x0120;  // AL控制
constexpr uint16_t REG_AL_STATUS     = 0x0130;  // AL状态
constexpr uint16_t REG_AL_STATUS_CODE = 0x0134; // AL状态码

constexpr uint16_t REG_PDI_CONTROL   = 0x0140;  // PDI控制
constexpr uint16_t REG_IRQ_MASK      = 0x0200;  // 中断掩码
constexpr uint16_t REG_RX_ERROR      = 0x0300;  // 接收错误计数器

constexpr uint16_t REG_WD_DIVIDER    = 0x0400;  // 看门狗分频器
constexpr uint16_t REG_WD_TIME_PDI   = 0x0410;  // PDI看门狗时间
constexpr uint16_t REG_WD_TIME_SM    = 0x0420;  // SM看门狗时间
constexpr uint16_t REG_WD_STATUS     = 0x0440;  // 看门狗状态

constexpr uint16_t REG_EEPROM_CONF   = 0x0500;  // EEPROM配置
constexpr uint16_t REG_EEPROM_PDI    = 0x0501;  // EEPROM PDI访问
constexpr uint16_t REG_EEPROM_CTRL   = 0x0502;  // EEPROM控制/状态
constexpr uint16_t REG_EEPROM_ADDR   = 0x0504;  // EEPROM地址
constexpr uint16_t REG_EEPROM_DATA   = 0x0508;  // EEPROM数据

constexpr uint16_t REG_FMMU_BASE     = 0x0600;  // FMMU起始地址
constexpr uint16_t REG_SM_BASE       = 0x0800;  // SM起始地址

constexpr uint16_t REG_DC_TIME_PORT0 = 0x0900;  // DC端口0接收时间
constexpr uint16_t REG_DC_RX_TIME    = 0x0918;  // DC接收时间
constexpr uint16_t REG_DC_SYSTEM_TIME = 0x0910; // DC系统时间
constexpr uint16_t REG_DC_SYSTEM_OFFSET = 0x0920; // DC系统时间偏移
constexpr uint16_t REG_DC_SYSTEM_DELAY = 0x0928; // DC系统时间延迟
constexpr uint16_t REG_DC_SPEED_COUNT = 0x0930; // DC速度计数器
constexpr uint16_t REG_DC_TIME_FILTER = 0x0934; // DC时间环路滤波深度
constexpr uint16_t REG_DC_CYCLE_SYNC  = 0x0980; // DC周期时间同步
constexpr uint16_t REG_DC_ACTIVATION  = 0x0981; // DC激活

// 邮箱协议类型
constexpr uint8_t MBX_ERR  = 0x00;  // 错误
constexpr uint8_t MBX_AOE  = 0x01;  // AoE
constexpr uint8_t MBX_EOE  = 0x02;  // EoE
constexpr uint8_t MBX_COE  = 0x03;  // CoE
constexpr uint8_t MBX_FOE  = 0x04;  // FoE
constexpr uint8_t MBX_SOE  = 0x05;  // SoE
constexpr uint8_t MBX_VOE  = 0x0F;  // VoE

// CoE服务类型
constexpr uint8_t COE_EMERGENCY        = 0x01;
constexpr uint8_t COE_SDO_REQUEST      = 0x02;
constexpr uint8_t COE_SDO_RESPONSE     = 0x03;
constexpr uint8_t COE_TXPDO            = 0x04;
constexpr uint8_t COE_RXPDO            = 0x05;
constexpr uint8_t COE_TXPDO_REMOTE_REQ = 0x06;
constexpr uint8_t COE_RXPDO_REMOTE_REQ = 0x07;
constexpr uint8_t COE_SDO_INFO         = 0x08;

// SDO命令
constexpr uint8_t SDO_DOWNLOAD_SEG_REQ  = 0x00;
constexpr uint8_t SDO_DOWNLOAD_INIT_REQ = 0x20;
constexpr uint8_t SDO_DOWNLOAD_INIT_RSP = 0x60;
constexpr uint8_t SDO_UPLOAD_INIT_REQ   = 0x40;
constexpr uint8_t SDO_UPLOAD_INIT_RSP   = 0x40;
constexpr uint8_t SDO_UPLOAD_SEG_REQ    = 0x60;
constexpr uint8_t SDO_UPLOAD_SEG_RSP    = 0x00;
constexpr uint8_t SDO_ABORT             = 0x80;

}  // anonymous namespace

// ============================================================
// EtherCATMaster 实现
// ============================================================

EtherCATMaster::EtherCATMaster(QObject* parent)
    : QObject(parent)
    , m_cyclicTimer(nullptr)
    , m_watchdogTimer(nullptr)
{
    m_statistics.startTime = QDateTime::currentDateTime();
}

EtherCATMaster::~EtherCATMaster()
{
    shutdown();
}

bool EtherCATMaster::initialize(const ECMasterConfig& config)
{
    QMutexLocker locker(&m_mutex);

    if (m_initialized.load()) {
        setLastError("Already initialized");
        return false;
    }

    m_config = config;

    // 验证配置
    if (m_config.networkInterface.isEmpty()) {
        setLastError("Network interface not specified");
        return false;
    }

    setStatus(ECMasterStatus::Initializing);

    // 初始化网络接口
    if (!initializeInterface()) {
        setStatus(ECMasterStatus::Error);
        return false;
    }

    m_initialized.store(true);
    setStatus(ECMasterStatus::Disconnected);

    qDebug() << "EtherCATMaster: Initialized on interface" << m_config.networkInterface;
    return true;
}

void EtherCATMaster::shutdown()
{
    // 先检查是否需要处理（不持有锁）
    if (!m_initialized.load()) {
        return;
    }

    // 停止周期交换（该函数有自己的锁）
    if (m_running.load()) {
        stopCyclicExchangeInternal();
    }

    // 现在获取锁进行状态清理
    QMutexLocker locker(&m_mutex);

    // 关闭网络接口
    closeInterface();

    m_slaves.clear();
    m_inputBuffer.clear();
    m_outputBuffer.clear();
    m_initialized.store(false);
    m_status.store(ECMasterStatus::Disconnected);
}

void EtherCATMaster::setConfig(const ECMasterConfig& config)
{
    QMutexLocker locker(&m_mutex);
    m_config = config;
}

// ============================================================
// 网络扫描
// ============================================================

int EtherCATMaster::scanNetwork()
{
    QMutexLocker locker(&m_mutex);

    if (!m_initialized.load()) {
        setLastError("Not initialized");
        return -1;
    }

    m_slaves.clear();

    // 模拟模式：创建测试从站
#ifndef ETHERCAT_HARDWARE_MODE
    for (int i = 0; i < 3; ++i) {
        ECSlaveInfo slave;
        slave.position = static_cast<uint16_t>(i + 1);
        slave.configuredAddress = 0x1000 + static_cast<uint16_t>(i);

        switch (i) {
        case 0:
            slave.name = "EK1100 EtherCAT Coupler";
            slave.identity.vendorId = 0x00000002;
            slave.identity.productCode = 0x044C2C52;
            slave.supportsCoE = true;
            slave.supportsDC = true;
            slave.inputBitSize = 0;
            slave.outputBitSize = 0;
            break;

        case 1:
            slave.name = "EL2008 8Ch Digital Output";
            slave.identity.vendorId = 0x00000002;
            slave.identity.productCode = 0x07D83052;
            slave.supportsCoE = true;
            slave.supportsDC = false;
            slave.inputBitSize = 0;
            slave.outputBitSize = 8;
            {
                PDOMapping rxPdo;
                rxPdo.pdoIndex = 0x1600;
                rxPdo.name = "RxPDO-Map";
                rxPdo.smIndex = 2;
                for (int j = 0; j < 8; ++j) {
                    PDOEntry entry;
                    entry.index = 0x7000 + static_cast<uint16_t>(j * 0x10);
                    entry.subIndex = 1;
                    entry.bitLength = 1;
                    entry.name = QString("Output %1").arg(j + 1);
                    entry.dataType = ECDataType::BOOLEAN;
                    rxPdo.entries.push_back(entry);
                }
                slave.rxPDOs.push_back(rxPdo);
            }
            break;

        case 2:
            slave.name = "EL1008 8Ch Digital Input";
            slave.identity.vendorId = 0x00000002;
            slave.identity.productCode = 0x03F03052;
            slave.supportsCoE = true;
            slave.supportsDC = false;
            slave.inputBitSize = 8;
            slave.outputBitSize = 0;
            {
                PDOMapping txPdo;
                txPdo.pdoIndex = 0x1A00;
                txPdo.name = "TxPDO-Map";
                txPdo.smIndex = 3;
                for (int j = 0; j < 8; ++j) {
                    PDOEntry entry;
                    entry.index = 0x6000 + static_cast<uint16_t>(j * 0x10);
                    entry.subIndex = 1;
                    entry.bitLength = 1;
                    entry.name = QString("Input %1").arg(j + 1);
                    entry.dataType = ECDataType::BOOLEAN;
                    txPdo.entries.push_back(entry);
                }
                slave.txPDOs.push_back(txPdo);
            }
            break;
        }

        slave.currentState = ECState::Init;
        slave.identity.revisionNo = 0x00110001;
        slave.identity.serialNo = 1000 + static_cast<uint32_t>(i);
        slave.mailboxOutAddr = 0x1000;
        slave.mailboxOutSize = 128;
        slave.mailboxInAddr = 0x1080;
        slave.mailboxInSize = 128;

        m_slaves.push_back(slave);
    }
#endif

    int count = static_cast<int>(m_slaves.size());

    // 更新状态（避免信号发射）
    m_status.store(count > 0 ? ECMasterStatus::Ready : ECMasterStatus::Disconnected);

    return count;
}

const ECSlaveInfo* EtherCATMaster::getSlaveInfo(uint16_t position) const
{
    QMutexLocker locker(&m_mutex);

    for (const auto& slave : m_slaves) {
        if (slave.position == position) {
            return &slave;
        }
    }
    return nullptr;
}

std::vector<uint16_t> EtherCATMaster::findSlaves(uint32_t vendorId,
                                                  uint32_t productCode) const
{
    QMutexLocker locker(&m_mutex);
    std::vector<uint16_t> result;

    for (const auto& slave : m_slaves) {
        if (slave.identity.vendorId == vendorId &&
            slave.identity.productCode == productCode) {
            result.push_back(slave.position);
        }
    }

    return result;
}

// ============================================================
// 状态机控制
// ============================================================

bool EtherCATMaster::requestState(ECState state, int timeout)
{
    QMutexLocker locker(&m_mutex);

    if (m_slaves.empty()) {
        setLastError("No slaves found");
        return false;
    }

    if (timeout <= 0) {
        timeout = m_config.stateChangeTimeout;
    }

    qDebug() << "EtherCATMaster: Requesting state" << getECStateName(state)
             << "for all slaves";

    bool allSuccess = true;

    // 逐个从站请求状态变化
    for (auto& slave : m_slaves) {
        if (slave.currentState == state) {
            continue;  // 已经在目标状态
        }

        // 写入AL控制寄存器
#ifndef ETHERCAT_HARDWARE_MODE
        // 模拟模式：直接设置状态
        ECState oldState = slave.currentState;
        slave.currentState = state;
        slave.requestedState = state;

        emit slaveStateChanged(slave.position, oldState, state);
#else
        // 实际硬件模式
        QByteArray frame = buildStateChangeFrame(slave.position, state);
        if (!sendReceiveFrame(frame)) {
            allSuccess = false;
            setLastError(QString("Failed to change state for slave %1")
                         .arg(slave.position));
            continue;
        }
#endif
    }

    // 等待状态变化完成
    if (allSuccess) {
        allSuccess = waitForState(state, timeout);
    }

    if (allSuccess) {
        if (state == ECState::Operational) {
            setStatus(ECMasterStatus::Running);
        } else if (state == ECState::PreOp || state == ECState::SafeOp) {
            setStatus(ECMasterStatus::Ready);
        }
    }

    return allSuccess;
}

bool EtherCATMaster::requestSlaveState(uint16_t slavePos, ECState state,
                                        int timeout)
{
    QMutexLocker locker(&m_mutex);

    // 查找从站
    ECSlaveInfo* slave = nullptr;
    for (auto& s : m_slaves) {
        if (s.position == slavePos) {
            slave = &s;
            break;
        }
    }

    if (!slave) {
        setLastError(QString("Slave %1 not found").arg(slavePos));
        return false;
    }

    if (timeout <= 0) {
        timeout = m_config.stateChangeTimeout;
    }

    qDebug() << "EtherCATMaster: Requesting state" << getECStateName(state)
             << "for slave" << slavePos;

#ifndef ETHERCAT_HARDWARE_MODE
    // 模拟模式
    ECState oldState = slave->currentState;
    slave->currentState = state;
    slave->requestedState = state;

    emit slaveStateChanged(slavePos, oldState, state);
    return true;
#else
    // 实际硬件模式
    QByteArray frame = buildStateChangeFrame(slavePos, state);
    if (!sendReceiveFrame(frame)) {
        setLastError(QString("Failed to change state for slave %1").arg(slavePos));
        return false;
    }

    // 等待状态变化
    QElapsedTimer timer;
    timer.start();

    while (timer.elapsed() < timeout) {
        updateSlaveStates();
        if (slave->currentState == state) {
            return true;
        }
        QThread::msleep(10);
    }

    setLastError(QString("Timeout waiting for slave %1 state change").arg(slavePos));
    return false;
#endif
}

ECState EtherCATMaster::getSlaveState(uint16_t slavePos) const
{
    QMutexLocker locker(&m_mutex);

    for (const auto& slave : m_slaves) {
        if (slave.position == slavePos) {
            return slave.currentState;
        }
    }

    return ECState::None;
}

bool EtherCATMaster::allSlavesInState(ECState state) const
{
    QMutexLocker locker(&m_mutex);

    for (const auto& slave : m_slaves) {
        if (slave.currentState != state) {
            return false;
        }
    }

    return true;
}

ALStatusCode EtherCATMaster::getSlaveALStatus(uint16_t slavePos) const
{
    QMutexLocker locker(&m_mutex);

    for (const auto& slave : m_slaves) {
        if (slave.position == slavePos) {
            return slave.alStatusCode;
        }
    }

    return ALStatusCode::NoError;
}

// ============================================================
// 从站配置
// ============================================================

bool EtherCATMaster::configureSlaves()
{
    QMutexLocker locker(&m_mutex);

    if (m_slaves.empty()) {
        setLastError("No slaves to configure");
        return false;
    }

    setStatus(ECMasterStatus::Configuring);

    qDebug() << "EtherCATMaster: Configuring" << m_slaves.size() << "slaves";

    bool allSuccess = true;

    // 计算IO映射大小
    m_inputMapSize = 0;
    m_outputMapSize = 0;

    for (auto& slave : m_slaves) {
        // 配置邮箱
        if (slave.supportsCoE) {
            if (!configureMailbox(slave.position)) {
                allSuccess = false;
                continue;
            }
        }

        // 配置同步管理器
        if (!configureSyncManagers(slave.position)) {
            allSuccess = false;
            continue;
        }

        // 配置FMMU
        if (!configureFMMU(slave.position)) {
            allSuccess = false;
            continue;
        }

        // 计算IO偏移
        slave.outputOffset = static_cast<uint32_t>(m_outputMapSize);
        slave.inputOffset = static_cast<uint32_t>(m_inputMapSize);

        m_outputMapSize += slave.outputByteSize();
        m_inputMapSize += slave.inputByteSize();
    }

    // 分配IO缓冲区
    m_inputBuffer.resize(m_inputMapSize);
    m_outputBuffer.resize(m_outputMapSize);
    m_inputBuffer.fill(0);
    m_outputBuffer.fill(0);

    // 配置分布式时钟
    if (m_config.autoDC) {
        configureDC();
    }

    if (allSuccess) {
        setStatus(ECMasterStatus::Ready);
        qDebug() << "EtherCATMaster: Configuration completed";
        qDebug() << "  Input map size:" << m_inputMapSize << "bytes";
        qDebug() << "  Output map size:" << m_outputMapSize << "bytes";
    } else {
        setStatus(ECMasterStatus::Error);
    }

    emit configurationCompleted(allSuccess);
    return allSuccess;
}

bool EtherCATMaster::configureSlave(uint16_t slavePos)
{
    QMutexLocker locker(&m_mutex);

    ECSlaveInfo* slave = nullptr;
    for (auto& s : m_slaves) {
        if (s.position == slavePos) {
            slave = &s;
            break;
        }
    }

    if (!slave) {
        setLastError(QString("Slave %1 not found").arg(slavePos));
        return false;
    }

    // 配置单个从站
    if (slave->supportsCoE && !configureMailbox(slavePos)) {
        return false;
    }

    if (!configureSyncManagers(slavePos)) {
        return false;
    }

    if (!configureFMMU(slavePos)) {
        return false;
    }

    return true;
}

bool EtherCATMaster::configureDC()
{
    qDebug() << "EtherCATMaster: Configuring Distributed Clock";

    // 查找支持DC的从站
    int dcSlaveCount = 0;
    for (const auto& slave : m_slaves) {
        if (slave.supportsDC) {
            dcSlaveCount++;
        }
    }

    if (dcSlaveCount == 0) {
        qDebug() << "EtherCATMaster: No DC-capable slaves found";
        return true;  // 不是错误，只是没有DC从站
    }

#ifndef ETHERCAT_HARDWARE_MODE
    // 模拟模式：标记DC已配置
    for (auto& slave : m_slaves) {
        if (slave.supportsDC) {
            slave.dcConfig.enabled = true;
            slave.dcConfig.syncType = DCSyncType::Sync0;
            slave.dcConfig.cycleTime0 = m_config.cycleTime * 1000;  // 转换为纳秒
        }
    }
    return true;
#else
    // 实际硬件模式
    // 1. 传播延迟补偿
    // 2. 系统时间偏移计算
    // 3. SYNC0/SYNC1配置
    // ...

    return true;
#endif
}

bool EtherCATMaster::configurePDOMapping(uint16_t slavePos,
                                          const std::vector<PDOMapping>& rxPDOs,
                                          const std::vector<PDOMapping>& txPDOs)
{
    QMutexLocker locker(&m_mutex);

    ECSlaveInfo* slave = nullptr;
    for (auto& s : m_slaves) {
        if (s.position == slavePos) {
            slave = &s;
            break;
        }
    }

    if (!slave) {
        setLastError(QString("Slave %1 not found").arg(slavePos));
        return false;
    }

    if (!slave->supportsCoE) {
        setLastError(QString("Slave %1 does not support CoE").arg(slavePos));
        return false;
    }

    // 清除现有PDO映射
    slave->rxPDOs.clear();
    slave->txPDOs.clear();

    // 通过SDO配置PDO映射
    // 1. 禁用当前PDO映射 (设置子索引0为0)
    // 2. 设置新的映射条目
    // 3. 重新启用PDO映射

    for (const auto& rxPdo : rxPDOs) {
        // 禁用
        sdoWriteUInt8(slavePos, rxPdo.pdoIndex, 0, 0);

        // 配置映射条目
        uint8_t entryIndex = 1;
        for (const auto& entry : rxPdo.entries) {
            uint32_t mappingValue = entry.toMappingValue();
            sdoWriteUInt32(slavePos, rxPdo.pdoIndex, entryIndex++, mappingValue);
        }

        // 启用
        sdoWriteUInt8(slavePos, rxPdo.pdoIndex, 0, static_cast<uint8_t>(rxPdo.entries.size()));

        slave->rxPDOs.push_back(rxPdo);
    }

    for (const auto& txPdo : txPDOs) {
        sdoWriteUInt8(slavePos, txPdo.pdoIndex, 0, 0);

        uint8_t entryIndex = 1;
        for (const auto& entry : txPdo.entries) {
            uint32_t mappingValue = entry.toMappingValue();
            sdoWriteUInt32(slavePos, txPdo.pdoIndex, entryIndex++, mappingValue);
        }

        sdoWriteUInt8(slavePos, txPdo.pdoIndex, 0, static_cast<uint8_t>(txPdo.entries.size()));

        slave->txPDOs.push_back(txPdo);
    }

    // 重新计算位大小
    slave->outputBitSize = 0;
    slave->inputBitSize = 0;

    for (const auto& pdo : slave->rxPDOs) {
        slave->outputBitSize += static_cast<uint32_t>(pdo.totalBitSize());
    }

    for (const auto& pdo : slave->txPDOs) {
        slave->inputBitSize += static_cast<uint32_t>(pdo.totalBitSize());
    }

    return true;
}

// ============================================================
// 周期数据交换
// ============================================================

bool EtherCATMaster::startCyclicExchange()
{
    QMutexLocker locker(&m_mutex);

    if (!m_initialized.load()) {
        setLastError("Not initialized");
        return false;
    }

    if (m_running.load()) {
        return true;  // 已经在运行
    }

    if (m_slaves.empty()) {
        setLastError("No slaves configured");
        return false;
    }

    // 确保所有从站在Operational状态（内部检查，不获取锁）
    bool allInOp = true;
    for (const auto& slave : m_slaves) {
        if (slave.currentState != ECState::Operational) {
            allInOp = false;
            break;
        }
    }

    if (!allInOp) {
        // 直接设置状态（模拟模式）
        for (auto& slave : m_slaves) {
            slave.currentState = ECState::Operational;
            slave.requestedState = ECState::Operational;
        }
    }

    qDebug() << "EtherCATMaster: Starting cyclic exchange with"
             << m_config.cycleTime << "us cycle time";

    m_running.store(true);
    m_cycleTimer.start();

    // 启动周期定时器
    // 注意：对于高精度周期，应使用专用实时线程而非QTimer
    int intervalMs = static_cast<int>(m_config.cycleTime / 1000);
    if (intervalMs < 1) intervalMs = 1;

    if (m_cyclicTimer) {
        m_cyclicTimer->start(intervalMs);
    } else {
        // 模拟模式：增加周期计数（模拟1个周期）
        m_statistics.cycleCount++;
    }

    // 启动看门狗
    if (m_config.enableWatchdog && m_watchdogTimer) {
        m_watchdogTimer->start(m_config.watchdogTimeout);
    }

    m_status.store(ECMasterStatus::Running);

    return true;
}

void EtherCATMaster::stopCyclicExchange()
{
    stopCyclicExchangeInternal();

    // 请求进入SafeOp状态
    requestState(ECState::SafeOp);
}

void EtherCATMaster::stopCyclicExchangeInternal()
{
    if (!m_running.load()) {
        return;
    }

    m_running.store(false);
    if (m_cyclicTimer) m_cyclicTimer->stop();
    if (m_watchdogTimer) m_watchdogTimer->stop();

    m_status.store(ECMasterStatus::Ready);
}

void EtherCATMaster::setCyclicCallback(ECCyclicCallback callback)
{
    QMutexLocker locker(&m_mutex);
    m_cyclicCallback = std::move(callback);
}

QByteArray EtherCATMaster::getInputs() const
{
    QMutexLocker locker(&m_ioMutex);
    return m_inputBuffer;
}

QByteArray EtherCATMaster::getOutputs() const
{
    QMutexLocker locker(&m_ioMutex);
    return m_outputBuffer;
}

void EtherCATMaster::setOutputs(const QByteArray& outputs)
{
    QMutexLocker locker(&m_ioMutex);
    if (outputs.size() <= m_outputBuffer.size()) {
        memcpy(m_outputBuffer.data(), outputs.constData(), outputs.size());
    }
}

std::pair<int, int> EtherCATMaster::getIOMapSize() const
{
    return {m_inputMapSize, m_outputMapSize};
}

// ============================================================
// 从站数据访问
// ============================================================

QByteArray EtherCATMaster::readSlaveInputs(uint16_t slavePos) const
{
    QMutexLocker locker(&m_ioMutex);

    for (const auto& slave : m_slaves) {
        if (slave.position == slavePos) {
            int offset = static_cast<int>(slave.inputOffset);
            int size = slave.inputByteSize();
            if (offset + size <= m_inputBuffer.size()) {
                return m_inputBuffer.mid(offset, size);
            }
            break;
        }
    }

    return QByteArray();
}

QByteArray EtherCATMaster::readSlaveOutputs(uint16_t slavePos) const
{
    QMutexLocker locker(&m_ioMutex);

    for (const auto& slave : m_slaves) {
        if (slave.position == slavePos) {
            int offset = static_cast<int>(slave.outputOffset);
            int size = slave.outputByteSize();
            if (offset + size <= m_outputBuffer.size()) {
                return m_outputBuffer.mid(offset, size);
            }
            break;
        }
    }

    return QByteArray();
}

bool EtherCATMaster::writeSlaveOutputs(uint16_t slavePos, const QByteArray& data)
{
    QMutexLocker locker(&m_ioMutex);

    for (const auto& slave : m_slaves) {
        if (slave.position == slavePos) {
            int offset = static_cast<int>(slave.outputOffset);
            int size = slave.outputByteSize();
            if (data.size() == size && offset + size <= m_outputBuffer.size()) {
                memcpy(m_outputBuffer.data() + offset, data.constData(), size);
                return true;
            }
            break;
        }
    }

    return false;
}

bool EtherCATMaster::readInputBit(uint16_t slavePos, int bitOffset) const
{
    QMutexLocker locker(&m_ioMutex);

    for (const auto& slave : m_slaves) {
        if (slave.position == slavePos) {
            int byteOffset = static_cast<int>(slave.inputOffset) + bitOffset / 8;
            int bit = bitOffset % 8;
            if (byteOffset < m_inputBuffer.size()) {
                return (m_inputBuffer.at(byteOffset) >> bit) & 0x01;
            }
            break;
        }
    }

    return false;
}

bool EtherCATMaster::writeOutputBit(uint16_t slavePos, int bitOffset, bool value)
{
    QMutexLocker locker(&m_ioMutex);

    for (const auto& slave : m_slaves) {
        if (slave.position == slavePos) {
            int byteOffset = static_cast<int>(slave.outputOffset) + bitOffset / 8;
            int bit = bitOffset % 8;
            if (byteOffset < m_outputBuffer.size()) {
                if (value) {
                    m_outputBuffer[byteOffset] |= (1 << bit);
                } else {
                    m_outputBuffer[byteOffset] &= ~(1 << bit);
                }
                return true;
            }
            break;
        }
    }

    return false;
}

uint8_t EtherCATMaster::readInputByte(uint16_t slavePos, int byteOffset) const
{
    QMutexLocker locker(&m_ioMutex);

    for (const auto& slave : m_slaves) {
        if (slave.position == slavePos) {
            int offset = static_cast<int>(slave.inputOffset) + byteOffset;
            if (offset < m_inputBuffer.size()) {
                return static_cast<uint8_t>(m_inputBuffer.at(offset));
            }
            break;
        }
    }

    return 0;
}

uint16_t EtherCATMaster::readInputWord(uint16_t slavePos, int byteOffset) const
{
    QMutexLocker locker(&m_ioMutex);

    for (const auto& slave : m_slaves) {
        if (slave.position == slavePos) {
            int offset = static_cast<int>(slave.inputOffset) + byteOffset;
            if (offset + 1 < m_inputBuffer.size()) {
                uint16_t value;
                memcpy(&value, m_inputBuffer.constData() + offset, 2);
                return value;
            }
            break;
        }
    }

    return 0;
}

uint32_t EtherCATMaster::readInputDWord(uint16_t slavePos, int byteOffset) const
{
    QMutexLocker locker(&m_ioMutex);

    for (const auto& slave : m_slaves) {
        if (slave.position == slavePos) {
            int offset = static_cast<int>(slave.inputOffset) + byteOffset;
            if (offset + 3 < m_inputBuffer.size()) {
                uint32_t value;
                memcpy(&value, m_inputBuffer.constData() + offset, 4);
                return value;
            }
            break;
        }
    }

    return 0;
}

bool EtherCATMaster::writeOutputByte(uint16_t slavePos, int byteOffset, uint8_t value)
{
    QMutexLocker locker(&m_ioMutex);

    for (const auto& slave : m_slaves) {
        if (slave.position == slavePos) {
            int offset = static_cast<int>(slave.outputOffset) + byteOffset;
            if (offset < m_outputBuffer.size()) {
                m_outputBuffer[offset] = static_cast<char>(value);
                return true;
            }
            break;
        }
    }

    return false;
}

bool EtherCATMaster::writeOutputWord(uint16_t slavePos, int byteOffset, uint16_t value)
{
    QMutexLocker locker(&m_ioMutex);

    for (const auto& slave : m_slaves) {
        if (slave.position == slavePos) {
            int offset = static_cast<int>(slave.outputOffset) + byteOffset;
            if (offset + 1 < m_outputBuffer.size()) {
                memcpy(m_outputBuffer.data() + offset, &value, 2);
                return true;
            }
            break;
        }
    }

    return false;
}

bool EtherCATMaster::writeOutputDWord(uint16_t slavePos, int byteOffset, uint32_t value)
{
    QMutexLocker locker(&m_ioMutex);

    for (const auto& slave : m_slaves) {
        if (slave.position == slavePos) {
            int offset = static_cast<int>(slave.outputOffset) + byteOffset;
            if (offset + 3 < m_outputBuffer.size()) {
                memcpy(m_outputBuffer.data() + offset, &value, 4);
                return true;
            }
            break;
        }
    }

    return false;
}

// ============================================================
// SDO访问
// ============================================================

SDOResponse EtherCATMaster::sdoRead(uint16_t slavePos, uint16_t index,
                                     uint8_t subIndex, int timeout)
{
    if (timeout <= 0) {
        timeout = m_config.sdoTimeout;
    }

    return sendSDORequest(slavePos, index, subIndex, SDOAccessType::Read,
                          QByteArray(), timeout);
}

SDOResponse EtherCATMaster::sdoWrite(uint16_t slavePos, uint16_t index,
                                      uint8_t subIndex, const QByteArray& data,
                                      int timeout)
{
    if (timeout <= 0) {
        timeout = m_config.sdoTimeout;
    }

    return sendSDORequest(slavePos, index, subIndex, SDOAccessType::Write,
                          data, timeout);
}

int EtherCATMaster::sdoReadAsync(const SDORequest& request, ECSDOCallback callback)
{
    QMutexLocker locker(&m_sdoMutex);

    int requestId = m_sdoRequestIdCounter.fetch_add(1);

    PendingSDORequest pending;
    pending.id = requestId;
    pending.request = request;
    pending.callback = std::move(callback);
    pending.timer.start();

    m_pendingSDORequests.push_back(std::move(pending));

    // TODO: 在工作线程中处理异步请求

    return requestId;
}

int EtherCATMaster::sdoWriteAsync(const SDORequest& request, ECSDOCallback callback)
{
    return sdoReadAsync(request, std::move(callback));  // 使用相同的队列
}

SDOResponse EtherCATMaster::sendSDORequest(uint16_t slavePos, uint16_t index,
                                            uint8_t subIndex, SDOAccessType accessType,
                                            const QByteArray& data, int timeout)
{
    Q_UNUSED(timeout)

    // 查找从站
    const ECSlaveInfo* slave = getSlaveInfo(slavePos);
    if (!slave) {
        return SDOResponse::Error(ECErrorCode::InvalidParameter,
                                  QString("Slave %1 not found").arg(slavePos));
    }

    if (!slave->supportsCoE) {
        return SDOResponse::Error(ECErrorCode::InvalidParameter,
                                  QString("Slave %1 does not support CoE").arg(slavePos));
    }

    m_statistics.sdoReadCount++;

#ifndef ETHERCAT_HARDWARE_MODE
    // 模拟模式：返回模拟数据
    if (accessType == SDOAccessType::Read) {
        SDOResponse resp;
        resp.success = true;

        // 根据索引返回不同数据
        if (index == 0x1000) {  // 设备类型
            uint32_t deviceType = 0x00020192;
            resp.data.resize(4);
            memcpy(resp.data.data(), &deviceType, 4);
        } else if (index == 0x1008) {  // 设备名称
            resp.data = slave->name.toUtf8();
        } else if (index == 0x1018 && subIndex == 1) {  // 厂商ID
            resp.data.resize(4);
            memcpy(resp.data.data(), &slave->identity.vendorId, 4);
        } else if (index == 0x1018 && subIndex == 2) {  // 产品代码
            resp.data.resize(4);
            memcpy(resp.data.data(), &slave->identity.productCode, 4);
        } else {
            // 默认返回4字节0
            resp.data.resize(4);
            resp.data.fill(0);
        }

        return resp;
    } else {
        // 写操作：模拟成功
        SDOResponse resp;
        resp.success = true;
        m_statistics.sdoWriteCount++;
        return resp;
    }
#else
    // 实际硬件模式
    QByteArray request;

    if (accessType == SDOAccessType::Read) {
        request = buildSDOReadRequest(index, subIndex);
    } else {
        request = buildSDOWriteRequest(index, subIndex, data);
    }

    // 发送邮箱请求
    // ...

    QByteArray response;
    // 等待响应...

    return parseSDOResponse(response);
#endif
}

// SDO便捷方法实现
int8_t EtherCATMaster::sdoReadInt8(uint16_t slavePos, uint16_t index, uint8_t subIndex)
{
    return sdoReadValue<int8_t>(slavePos, index, subIndex, 0);
}

int16_t EtherCATMaster::sdoReadInt16(uint16_t slavePos, uint16_t index, uint8_t subIndex)
{
    return sdoReadValue<int16_t>(slavePos, index, subIndex, 0);
}

int32_t EtherCATMaster::sdoReadInt32(uint16_t slavePos, uint16_t index, uint8_t subIndex)
{
    return sdoReadValue<int32_t>(slavePos, index, subIndex, 0);
}

uint8_t EtherCATMaster::sdoReadUInt8(uint16_t slavePos, uint16_t index, uint8_t subIndex)
{
    return sdoReadValue<uint8_t>(slavePos, index, subIndex, 0);
}

uint16_t EtherCATMaster::sdoReadUInt16(uint16_t slavePos, uint16_t index, uint8_t subIndex)
{
    return sdoReadValue<uint16_t>(slavePos, index, subIndex, 0);
}

uint32_t EtherCATMaster::sdoReadUInt32(uint16_t slavePos, uint16_t index, uint8_t subIndex)
{
    return sdoReadValue<uint32_t>(slavePos, index, subIndex, 0);
}

float EtherCATMaster::sdoReadFloat(uint16_t slavePos, uint16_t index, uint8_t subIndex)
{
    return sdoReadValue<float>(slavePos, index, subIndex, 0.0f);
}

QString EtherCATMaster::sdoReadString(uint16_t slavePos, uint16_t index, uint8_t subIndex)
{
    SDOResponse resp = sdoRead(slavePos, index, subIndex);
    if (resp.success) {
        return QString::fromUtf8(resp.data);
    }
    return QString();
}

bool EtherCATMaster::sdoWriteInt8(uint16_t slavePos, uint16_t index,
                                   uint8_t subIndex, int8_t value)
{
    QByteArray data(1, 0);
    data[0] = static_cast<char>(value);
    return sdoWrite(slavePos, index, subIndex, data).success;
}

bool EtherCATMaster::sdoWriteInt16(uint16_t slavePos, uint16_t index,
                                    uint8_t subIndex, int16_t value)
{
    QByteArray data(2, 0);
    memcpy(data.data(), &value, 2);
    return sdoWrite(slavePos, index, subIndex, data).success;
}

bool EtherCATMaster::sdoWriteInt32(uint16_t slavePos, uint16_t index,
                                    uint8_t subIndex, int32_t value)
{
    QByteArray data(4, 0);
    memcpy(data.data(), &value, 4);
    return sdoWrite(slavePos, index, subIndex, data).success;
}

bool EtherCATMaster::sdoWriteUInt8(uint16_t slavePos, uint16_t index,
                                    uint8_t subIndex, uint8_t value)
{
    QByteArray data(1, 0);
    data[0] = static_cast<char>(value);
    return sdoWrite(slavePos, index, subIndex, data).success;
}

bool EtherCATMaster::sdoWriteUInt16(uint16_t slavePos, uint16_t index,
                                     uint8_t subIndex, uint16_t value)
{
    QByteArray data(2, 0);
    memcpy(data.data(), &value, 2);
    return sdoWrite(slavePos, index, subIndex, data).success;
}

bool EtherCATMaster::sdoWriteUInt32(uint16_t slavePos, uint16_t index,
                                     uint8_t subIndex, uint32_t value)
{
    QByteArray data(4, 0);
    memcpy(data.data(), &value, 4);
    return sdoWrite(slavePos, index, subIndex, data).success;
}

bool EtherCATMaster::sdoWriteFloat(uint16_t slavePos, uint16_t index,
                                    uint8_t subIndex, float value)
{
    QByteArray data(4, 0);
    memcpy(data.data(), &value, 4);
    return sdoWrite(slavePos, index, subIndex, data).success;
}

bool EtherCATMaster::sdoWriteString(uint16_t slavePos, uint16_t index,
                                     uint8_t subIndex, const QString& value)
{
    return sdoWrite(slavePos, index, subIndex, value.toUtf8()).success;
}

// ============================================================
// 诊断和统计
// ============================================================

ECStatistics EtherCATMaster::getStatistics() const
{
    QMutexLocker locker(&m_mutex);
    ECStatistics stats = m_statistics;
    stats.lastUpdateTime = QDateTime::currentDateTime();
    return stats;
}

void EtherCATMaster::resetStatistics()
{
    QMutexLocker locker(&m_mutex);
    m_statistics.reset();
}

QString EtherCATMaster::getDiagnosticInfo() const
{
    QMutexLocker locker(&m_mutex);

    QString info;
    info += QStringLiteral("========== EtherCAT诊断信息 ==========\n");
    info += QString("主站名称: %1\n").arg(m_config.name);
    info += QString("网络接口: %1\n").arg(m_config.networkInterface);
    info += QString("周期时间: %1 us\n").arg(m_config.cycleTime);
    info += QString("状态: %1\n").arg(getECMasterStatusName(m_status.load()));
    info += QString("从站数量: %1\n").arg(m_slaves.size());
    info += QString("IO映射: 输入 %1 字节, 输出 %2 字节\n")
                .arg(m_inputMapSize).arg(m_outputMapSize);
    info += QStringLiteral("\n");

    info += QStringLiteral("---------- 从站信息 ----------\n");
    for (const auto& slave : m_slaves) {
        info += QString("[%1] %2\n").arg(slave.position).arg(slave.name);
        info += QString("    厂商: 0x%1  产品: 0x%2\n")
                    .arg(slave.identity.vendorId, 8, 16, QChar('0'))
                    .arg(slave.identity.productCode, 8, 16, QChar('0'));
        info += QString("    状态: %1\n").arg(getECStateName(slave.currentState));
        info += QString("    IO: 输入 %1 位, 输出 %2 位\n")
                    .arg(slave.inputBitSize).arg(slave.outputBitSize);
        if (slave.supportsCoE) info += QStringLiteral("    支持: CoE ");
        if (slave.supportsDC) info += QStringLiteral("DC ");
        info += QStringLiteral("\n");
    }

    info += QStringLiteral("\n---------- 统计信息 ----------\n");
    info += QString("周期计数: %1\n").arg(m_statistics.cycleCount);
    info += QString("平均周期时间: %1 ns\n").arg(m_statistics.avgCycleTime);
    info += QString("WC错误: %1 (%.2f%%)\n")
                .arg(m_statistics.wcErrors)
                .arg(m_statistics.getWCErrorRate());
    info += QString("SDO读取: %1  写入: %2  错误: %3\n")
                .arg(m_statistics.sdoReadCount)
                .arg(m_statistics.sdoWriteCount)
                .arg(m_statistics.sdoErrorCount);

    return info;
}

ECResult EtherCATMaster::readSlaveErrorCounters(uint16_t slavePos)
{
    Q_UNUSED(slavePos)
    // 读取0x0300-0x030D错误计数器寄存器
    // TODO: 实现实际读取
    return ECResult::Success();
}

ECResult EtherCATMaster::clearSlaveErrorCounters(uint16_t slavePos)
{
    Q_UNUSED(slavePos)
    // 写入0x0300-0x030D错误计数器寄存器
    // TODO: 实现实际清除
    return ECResult::Success();
}

// ============================================================
// 持久化
// ============================================================

QJsonObject EtherCATMaster::exportToJson() const
{
    QMutexLocker locker(&m_mutex);

    QJsonObject root;
    root["type"] = "EtherCATMaster";
    root["version"] = "1.0";
    root["config"] = m_config.toJson();

    // 导出从站配置
    QJsonArray slavesArray;
    for (const auto& slave : m_slaves) {
        slavesArray.append(slave.toJson());
    }
    root["slaves"] = slavesArray;

    return root;
}

bool EtherCATMaster::importFromJson(const QJsonObject& json)
{
    QMutexLocker locker(&m_mutex);

    if (json["type"].toString() != "EtherCATMaster") {
        setLastError("Invalid JSON type");
        return false;
    }

    m_config = ECMasterConfig::fromJson(json["config"].toObject());

    // 导入从站配置（仅用于预配置）
    m_slaves.clear();
    QJsonArray slavesArray = json["slaves"].toArray();
    for (const auto& val : slavesArray) {
        // 从JSON恢复从站配置
        // 实际从站信息需要通过扫描获取
    }

    return true;
}

bool EtherCATMaster::saveToFile(const QString& filePath) const
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }

    QJsonDocument doc(exportToJson());
    file.write(doc.toJson(QJsonDocument::Indented));
    return true;
}

bool EtherCATMaster::loadFromFile(const QString& filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        setLastError(QString("Cannot open file: %1").arg(filePath));
        return false;
    }

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &error);

    if (error.error != QJsonParseError::NoError) {
        setLastError(QString("JSON parse error: %1").arg(error.errorString()));
        return false;
    }

    return importFromJson(doc.object());
}

// ============================================================
// 私有槽函数
// ============================================================

void EtherCATMaster::onCyclicTimerTimeout()
{
    if (!m_running.load()) {
        return;
    }

    processCyclicData();
}

void EtherCATMaster::onWatchdogTimeout()
{
    if (!m_running.load()) {
        return;
    }

    // 检查从站状态
    updateSlaveStates();

    // 检查是否有从站掉线
    for (const auto& slave : m_slaves) {
        if (slave.currentState != ECState::Operational) {
            emit slaveError(slave.position, ECErrorCode::SlaveError,
                           QString("Slave %1 not in Operational state").arg(slave.position));
        }
    }
}

// ============================================================
// 私有方法
// ============================================================

bool EtherCATMaster::initializeInterface()
{
#ifdef Q_OS_WIN
    // Windows: 使用WinPcap/Npcap
    qDebug() << "EtherCATMaster: Initializing WinPcap interface";
    // TODO: 实现WinPcap初始化
    return true;  // 模拟模式返回true
#else
    // Linux: 使用原始套接字
    qDebug() << "EtherCATMaster: Initializing raw socket";
    // TODO: 实现原始套接字初始化
    return true;  // 模拟模式返回true
#endif
}

void EtherCATMaster::closeInterface()
{
#ifdef Q_OS_WIN
    if (m_pcapHandle) {
        // pcap_close(m_pcapHandle);
        m_pcapHandle = nullptr;
    }
#else
    if (m_rawSocket >= 0) {
        // close(m_rawSocket);
        m_rawSocket = -1;
    }
#endif
}

bool EtherCATMaster::sendReceiveFrame(QByteArray& frame)
{
    Q_UNUSED(frame)

#ifndef ETHERCAT_HARDWARE_MODE
    // 模拟模式
    return true;
#else
    // 实际硬件模式
    // TODO: 实现实际的帧发送和接收
    return false;
#endif
}

void EtherCATMaster::updateSlaveStates()
{
    // 读取所有从站的AL状态
#ifndef ETHERCAT_HARDWARE_MODE
    // 模拟模式：状态已经在内存中
#else
    // 实际硬件模式
    // 使用BRD命令读取所有从站的AL状态寄存器
#endif
}

void EtherCATMaster::processCyclicData()
{
    QMutexLocker locker(&m_ioMutex);

    uint64_t cycleStart = m_cycleTimer.nsecsElapsed();

#ifndef ETHERCAT_HARDWARE_MODE
    // 模拟模式：模拟输入数据变化
    static uint8_t simCounter = 0;
    if (m_inputMapSize > 0) {
        m_inputBuffer[0] = static_cast<char>(simCounter++);
    }
#else
    // 实际硬件模式
    // 构建LRW帧，发送输出，接收输入
#endif

    // 调用用户回调
    if (m_cyclicCallback) {
        QByteArray outputs = m_outputBuffer;
        m_cyclicCallback(m_inputBuffer, outputs);
        if (outputs != m_outputBuffer) {
            m_outputBuffer = outputs;
        }
    }

    // 更新统计
    uint64_t cycleEnd = m_cycleTimer.nsecsElapsed();
    uint64_t cycleTime = cycleEnd - cycleStart;

    m_statistics.cycleCount++;
    m_statistics.lastCycleTime = cycleTime;

    if (cycleTime < m_statistics.minCycleTime) {
        m_statistics.minCycleTime = cycleTime;
    }
    if (cycleTime > m_statistics.maxCycleTime) {
        m_statistics.maxCycleTime = cycleTime;
    }

    // 移动平均
    m_statistics.avgCycleTime = (m_statistics.avgCycleTime * (m_statistics.cycleCount - 1)
                                  + cycleTime) / m_statistics.cycleCount;
}

void EtherCATMaster::handleSlaveError(uint16_t slavePos, ECState state)
{
    // 读取AL状态码
    ALStatusCode alStatus = getSlaveALStatus(slavePos);

    QString message = QString("Slave %1 error: %2 (State: %3)")
                          .arg(slavePos)
                          .arg(getALStatusDescription(alStatus))
                          .arg(getECStateName(state));

    emit slaveError(slavePos, ECErrorCode::SlaveError, message);
}

bool EtherCATMaster::waitForState(ECState targetState, int timeout)
{
    QElapsedTimer timer;
    timer.start();

    while (timer.elapsed() < timeout) {
        updateSlaveStates();

        bool allInState = true;
        for (const auto& slave : m_slaves) {
            if (slave.currentState != targetState) {
                allInState = false;
                break;
            }
        }

        if (allInState) {
            return true;
        }

        QThread::msleep(10);
    }

    return false;
}

bool EtherCATMaster::configureMailbox(uint16_t slavePos)
{
    Q_UNUSED(slavePos)
    // 配置邮箱同步管理器 SM0 (输出) 和 SM1 (输入)
    return true;
}

bool EtherCATMaster::configureSyncManagers(uint16_t slavePos)
{
    Q_UNUSED(slavePos)
    // 配置SM2 (输出PDO) 和 SM3 (输入PDO)
    return true;
}

bool EtherCATMaster::configureFMMU(uint16_t slavePos)
{
    Q_UNUSED(slavePos)
    // 配置FMMU映射
    return true;
}

void EtherCATMaster::setStatus(ECMasterStatus status)
{
    ECMasterStatus old = m_status.exchange(status);
    if (old != status) {
        emit statusChanged(status);
    }
}

void EtherCATMaster::setLastError(const QString& error)
{
    m_lastError = error;
    qWarning() << "EtherCATMaster:" << error;
}

QByteArray EtherCATMaster::buildSDOReadRequest(uint16_t index, uint8_t subIndex)
{
    QByteArray request(8, 0);
    request[0] = static_cast<char>(SDO_UPLOAD_INIT_REQ);  // 命令
    request[1] = static_cast<char>(index & 0xFF);         // 索引低字节
    request[2] = static_cast<char>((index >> 8) & 0xFF);  // 索引高字节
    request[3] = static_cast<char>(subIndex);             // 子索引
    return request;
}

QByteArray EtherCATMaster::buildSDOWriteRequest(uint16_t index, uint8_t subIndex,
                                                 const QByteArray& data)
{
    QByteArray request(8 + data.size(), 0);

    uint8_t cmd = SDO_DOWNLOAD_INIT_REQ;
    if (data.size() <= 4) {
        // 快速传输
        cmd |= 0x02;  // expedited
        cmd |= (4 - static_cast<uint8_t>(data.size())) << 2;  // size indicator
        cmd |= 0x01;  // size indicated
    }

    request[0] = static_cast<char>(cmd);
    request[1] = static_cast<char>(index & 0xFF);
    request[2] = static_cast<char>((index >> 8) & 0xFF);
    request[3] = static_cast<char>(subIndex);

    if (data.size() <= 4) {
        memcpy(request.data() + 4, data.constData(), data.size());
    } else {
        // 普通传输
        uint32_t size = static_cast<uint32_t>(data.size());
        memcpy(request.data() + 4, &size, 4);
        memcpy(request.data() + 8, data.constData(), data.size());
    }

    return request;
}

SDOResponse EtherCATMaster::parseSDOResponse(const QByteArray& response)
{
    if (response.size() < 8) {
        return SDOResponse::Error(ECErrorCode::PDOSizeError, "Response too short");
    }

    uint8_t cmd = static_cast<uint8_t>(response[0]);

    // 检查是否是中止响应
    if (cmd == SDO_ABORT) {
        uint32_t abortCode = 0;
        memcpy(&abortCode, response.constData() + 4, 4);
        return SDOResponse::Abort(abortCode);
    }

    SDOResponse resp;
    resp.success = true;

    // 解析数据
    if (cmd & 0x02) {
        // 快速传输
        int size = 4 - ((cmd >> 2) & 0x03);
        resp.data = response.mid(4, size);
    } else {
        // 普通传输 - 需要处理分段
        resp.data = response.mid(4);
    }

    return resp;
}

// ============================================================
// EtherCATWorker 实现
// ============================================================

EtherCATWorker::EtherCATWorker(EtherCATMaster* master, QObject* parent)
    : QObject(parent)
    , m_master(master)
{
}

EtherCATWorker::~EtherCATWorker()
{
    stop();
}

void EtherCATWorker::start()
{
    m_running.store(true);
    m_stopRequested.store(false);

    // 启动处理循环
    QTimer::singleShot(0, this, &EtherCATWorker::processLoop);
}

void EtherCATWorker::stop()
{
    m_stopRequested.store(true);
    m_running.store(false);
}

void EtherCATWorker::processLoop()
{
    if (m_stopRequested.load()) {
        m_running.store(false);
        return;
    }

    // 处理周期数据
    emit cyclicDataProcessed();

    // 继续下一次循环
    if (m_running.load()) {
        QTimer::singleShot(1, this, &EtherCATWorker::processLoop);
    }
}

// ============================================================
// EtherCATMasterManager 实现
// ============================================================

EtherCATMasterManager& EtherCATMasterManager::instance()
{
    static EtherCATMasterManager instance;
    return instance;
}

EtherCATMaster* EtherCATMasterManager::createMaster(const QString& name)
{
    QMutexLocker locker(&m_mutex);

    if (m_masters.find(name) != m_masters.end()) {
        return m_masters[name].get();
    }

    auto master = std::make_unique<EtherCATMaster>();
    EtherCATMaster* ptr = master.get();
    m_masters[name] = std::move(master);

    return ptr;
}

EtherCATMaster* EtherCATMasterManager::getMaster(const QString& name)
{
    QMutexLocker locker(&m_mutex);

    auto it = m_masters.find(name);
    if (it != m_masters.end()) {
        return it->second.get();
    }

    return nullptr;
}

bool EtherCATMasterManager::removeMaster(const QString& name)
{
    QMutexLocker locker(&m_mutex);

    auto it = m_masters.find(name);
    if (it != m_masters.end()) {
        it->second->shutdown();
        m_masters.erase(it);
        return true;
    }

    return false;
}

QStringList EtherCATMasterManager::getMasterNames() const
{
    QMutexLocker locker(&m_mutex);

    QStringList names;
    for (const auto& pair : m_masters) {
        names.append(pair.first);
    }

    return names;
}

QStringList EtherCATMasterManager::listNetworkInterfaces()
{
    QStringList interfaces;

#ifdef Q_OS_WIN
    // Windows: 使用GetAdaptersInfo获取网络适配器
    IP_ADAPTER_INFO adapterInfo[16];
    DWORD dwBufLen = sizeof(adapterInfo);

    if (GetAdaptersInfo(adapterInfo, &dwBufLen) == ERROR_SUCCESS) {
        PIP_ADAPTER_INFO pAdapter = adapterInfo;
        while (pAdapter) {
            // 转换为WinPcap格式
            QString name = QString("\\Device\\NPF_%1")
                               .arg(QString::fromLatin1(pAdapter->AdapterName));
            QString desc = QString::fromLatin1(pAdapter->Description);
            interfaces.append(QString("%1 (%2)").arg(desc).arg(name));
            pAdapter = pAdapter->Next;
        }
    }
#else
    // Linux: 使用QNetworkInterface
    for (const QNetworkInterface& iface : QNetworkInterface::allInterfaces()) {
        if (iface.flags().testFlag(QNetworkInterface::IsUp) &&
            !iface.flags().testFlag(QNetworkInterface::IsLoopBack)) {
            interfaces.append(iface.name());
        }
    }
#endif

    return interfaces;
}

}  // namespace PLC
}  // namespace VisionForge
