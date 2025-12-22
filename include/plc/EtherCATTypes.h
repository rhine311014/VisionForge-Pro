/**
 * @file EtherCATTypes.h
 * @brief EtherCAT通信模块类型定义
 * @author VisionForge Team
 * @date 2024
 *
 * 本文件定义了EtherCAT通信所需的所有类型、枚举和数据结构。
 * EtherCAT (Ethernet for Control Automation Technology) 是一种高性能
 * 工业以太网协议，具有微秒级实时性能和飞读飞写机制。
 */

#ifndef VISIONFORGE_PLC_ETHERCAT_TYPES_H
#define VISIONFORGE_PLC_ETHERCAT_TYPES_H

#include <QString>
#include <QByteArray>
#include <QVariantMap>
#include <QDateTime>
#include <QJsonObject>
#include <QJsonArray>
#include <vector>
#include <array>
#include <cstdint>
#include <functional>

namespace VisionForge {
namespace PLC {

// ============================================================
// EtherCAT状态机
// ============================================================

/**
 * @brief EtherCAT从站状态
 *
 * 遵循ETG.1000规范的状态机定义：
 * Init -> Pre-Operational -> Safe-Operational -> Operational
 */
enum class ECState : uint8_t {
    None        = 0x00,     ///< 无/未知状态
    Init        = 0x01,     ///< 初始化状态
    PreOp       = 0x02,     ///< 预操作状态 (可配置SDO)
    Boot        = 0x03,     ///< 引导状态 (固件升级)
    SafeOp      = 0x04,     ///< 安全操作状态 (输入有效,输出安全值)
    Operational = 0x08,     ///< 操作状态 (完全运行)

    // 状态+错误标志
    InitError   = 0x11,     ///< Init + Error
    PreOpError  = 0x12,     ///< PreOp + Error
    SafeOpError = 0x14,     ///< SafeOp + Error
    OpError     = 0x18      ///< Operational + Error
};

/**
 * @brief 状态请求类型
 */
enum class ECStateRequest : uint8_t {
    ToInit      = 0x01,
    ToPreOp     = 0x02,
    ToBoot      = 0x03,
    ToSafeOp    = 0x04,
    ToOperational = 0x08
};

/**
 * @brief 获取EtherCAT状态名称
 */
inline QString getECStateName(ECState state) {
    switch (state) {
    case ECState::None:        return QStringLiteral("None");
    case ECState::Init:        return QStringLiteral("Init");
    case ECState::PreOp:       return QStringLiteral("Pre-Operational");
    case ECState::Boot:        return QStringLiteral("Boot");
    case ECState::SafeOp:      return QStringLiteral("Safe-Operational");
    case ECState::Operational: return QStringLiteral("Operational");
    case ECState::InitError:   return QStringLiteral("Init+Error");
    case ECState::PreOpError:  return QStringLiteral("Pre-Op+Error");
    case ECState::SafeOpError: return QStringLiteral("Safe-Op+Error");
    case ECState::OpError:     return QStringLiteral("Op+Error");
    default:                   return QStringLiteral("Unknown");
    }
}

/**
 * @brief 检查状态是否包含错误
 */
inline bool hasError(ECState state) {
    return (static_cast<uint8_t>(state) & 0x10) != 0;
}

// ============================================================
// EtherCAT数据类型
// ============================================================

/**
 * @brief EtherCAT对象数据类型 (ETG.1000 Table 61)
 */
enum class ECDataType : uint16_t {
    // 基本类型
    BOOLEAN         = 0x0001,
    INTEGER8        = 0x0002,
    INTEGER16       = 0x0003,
    INTEGER32       = 0x0004,
    UNSIGNED8       = 0x0005,
    UNSIGNED16      = 0x0006,
    UNSIGNED32      = 0x0007,
    REAL32          = 0x0008,   ///< float
    VISIBLE_STRING  = 0x0009,
    OCTET_STRING    = 0x000A,
    UNICODE_STRING  = 0x000B,
    TIME_OF_DAY     = 0x000C,
    TIME_DIFFERENCE = 0x000D,
    DOMAIN_TYPE     = 0x000F,   ///< 重命名避免Windows宏冲突
    INTEGER24       = 0x0010,
    REAL64          = 0x0011,   ///< double
    INTEGER64       = 0x0015,
    UNSIGNED24      = 0x0016,
    UNSIGNED64      = 0x001B,

    // PDO映射类型
    PDO_MAPPING     = 0x0021,
    IDENTITY        = 0x0023,

    // 厂商特定
    VENDOR_SPECIFIC = 0x0100
};

/**
 * @brief 获取数据类型大小（字节）
 */
inline int getECDataTypeSize(ECDataType type) {
    switch (type) {
    case ECDataType::BOOLEAN:
    case ECDataType::INTEGER8:
    case ECDataType::UNSIGNED8:
        return 1;
    case ECDataType::INTEGER16:
    case ECDataType::UNSIGNED16:
        return 2;
    case ECDataType::INTEGER24:
    case ECDataType::UNSIGNED24:
        return 3;
    case ECDataType::INTEGER32:
    case ECDataType::UNSIGNED32:
    case ECDataType::REAL32:
        return 4;
    case ECDataType::INTEGER64:
    case ECDataType::UNSIGNED64:
    case ECDataType::REAL64:
        return 8;
    default:
        return 0;  // 可变长度
    }
}

// ============================================================
// AL状态码 (应用层状态)
// ============================================================

/**
 * @brief AL状态错误码 (ETG.1000 Table 11)
 */
enum class ALStatusCode : uint16_t {
    NoError                     = 0x0000,
    UnspecifiedError            = 0x0001,
    NoMemory                    = 0x0002,
    InvalidDeviceSetup          = 0x0003,

    // SII (Slave Information Interface) 错误
    SIIReadError                = 0x0006,

    // 看门狗错误
    WatchdogError               = 0x001B,

    // 同步管理器错误
    InvalidSMConfiguration      = 0x001E,
    InvalidOutputConfiguration  = 0x001D,
    InvalidInputConfiguration   = 0x001F,

    // DC错误
    DCPDISyncError              = 0x0020,
    DCSyncError                 = 0x0021,
    DCCycleTimeError            = 0x0022,
    DCSyncCycleTimeError        = 0x0023,

    // 邮箱错误
    InvalidMailboxConfiguration = 0x0029,
    InvalidSMTypesConfiguration = 0x002A,

    // FMMU错误
    InvalidFMMUConfiguration    = 0x002D,

    // 引导状态错误
    BootNotSupported            = 0x0031,
    NoValidFirmware             = 0x0032
};

/**
 * @brief 获取AL状态码描述
 */
inline QString getALStatusDescription(ALStatusCode code) {
    switch (code) {
    case ALStatusCode::NoError:
        return QStringLiteral("No error");
    case ALStatusCode::UnspecifiedError:
        return QStringLiteral("Unspecified error");
    case ALStatusCode::NoMemory:
        return QStringLiteral("No memory");
    case ALStatusCode::InvalidDeviceSetup:
        return QStringLiteral("Invalid device setup");
    case ALStatusCode::WatchdogError:
        return QStringLiteral("Watchdog error");
    case ALStatusCode::InvalidSMConfiguration:
        return QStringLiteral("Invalid sync manager configuration");
    case ALStatusCode::DCPDISyncError:
        return QStringLiteral("DC PDI sync error");
    case ALStatusCode::DCSyncError:
        return QStringLiteral("DC sync error");
    default:
        return QString("AL Error: 0x%1").arg(static_cast<uint16_t>(code), 4, 16, QChar('0'));
    }
}

// ============================================================
// 从站标识信息
// ============================================================

/**
 * @brief EtherCAT从站身份信息
 */
struct ECSlaveIdentity {
    uint32_t vendorId = 0;      ///< 厂商ID (VendorID)
    uint32_t productCode = 0;   ///< 产品代码 (ProductCode)
    uint32_t revisionNo = 0;    ///< 修订版本 (RevisionNo)
    uint32_t serialNo = 0;      ///< 序列号 (SerialNo)

    bool isValid() const {
        return vendorId != 0 && productCode != 0;
    }

    QString toString() const {
        return QString("Vendor:0x%1 Product:0x%2 Rev:%3 SN:%4")
            .arg(vendorId, 8, 16, QChar('0'))
            .arg(productCode, 8, 16, QChar('0'))
            .arg(revisionNo)
            .arg(serialNo);
    }

    QJsonObject toJson() const {
        QJsonObject obj;
        obj["vendorId"] = static_cast<qint64>(vendorId);
        obj["productCode"] = static_cast<qint64>(productCode);
        obj["revisionNo"] = static_cast<qint64>(revisionNo);
        obj["serialNo"] = static_cast<qint64>(serialNo);
        return obj;
    }

    static ECSlaveIdentity fromJson(const QJsonObject& json) {
        ECSlaveIdentity id;
        id.vendorId = static_cast<uint32_t>(json["vendorId"].toDouble());
        id.productCode = static_cast<uint32_t>(json["productCode"].toDouble());
        id.revisionNo = static_cast<uint32_t>(json["revisionNo"].toDouble());
        id.serialNo = static_cast<uint32_t>(json["serialNo"].toDouble());
        return id;
    }
};

// ============================================================
// 同步管理器配置
// ============================================================

/**
 * @brief 同步管理器类型
 */
enum class SMType : uint8_t {
    Unused      = 0x00,     ///< 未使用
    MailboxOut  = 0x01,     ///< 主站->从站邮箱 (写)
    MailboxIn   = 0x02,     ///< 从站->主站邮箱 (读)
    ProcessOut  = 0x03,     ///< 输出过程数据 (TxPDO)
    ProcessIn   = 0x04      ///< 输入过程数据 (RxPDO)
};

/**
 * @brief 同步管理器配置
 */
struct SMConfig {
    uint8_t  index = 0;         ///< SM索引 (0-7)
    uint16_t startAddress = 0;  ///< 起始物理地址
    uint16_t length = 0;        ///< 数据长度
    SMType   type = SMType::Unused;
    bool     enabled = false;
    uint8_t  control = 0;       ///< 控制字节
    uint8_t  status = 0;        ///< 状态字节

    QJsonObject toJson() const {
        QJsonObject obj;
        obj["index"] = index;
        obj["startAddress"] = startAddress;
        obj["length"] = length;
        obj["type"] = static_cast<int>(type);
        obj["enabled"] = enabled;
        return obj;
    }
};

// ============================================================
// FMMU配置
// ============================================================

/**
 * @brief FMMU配置
 */
struct FMMUConfig {
    uint8_t  index = 0;             ///< FMMU索引 (0-15)
    uint32_t logicalStartAddr = 0;  ///< 逻辑起始地址
    uint16_t logicalLength = 0;     ///< 逻辑数据长度
    uint8_t  logicalStartBit = 0;   ///< 起始位
    uint8_t  logicalEndBit = 7;     ///< 结束位
    uint16_t physicalStartAddr = 0; ///< 物理起始地址
    uint8_t  physicalStartBit = 0;  ///< 物理起始位
    bool     readAccess = false;    ///< 读访问
    bool     writeAccess = false;   ///< 写访问
    bool     enabled = false;       ///< 启用
};

// ============================================================
// 分布式时钟配置
// ============================================================

/**
 * @brief DC同步类型
 */
enum class DCSyncType : uint8_t {
    FreeRun     = 0x00,     ///< 自由运行
    Sync0       = 0x01,     ///< SYNC0同步
    Sync01      = 0x03      ///< SYNC0+SYNC1同步
};

/**
 * @brief 分布式时钟配置
 */
struct DCConfig {
    bool     enabled = false;       ///< 是否启用DC
    DCSyncType syncType = DCSyncType::FreeRun;
    uint32_t cycleTime0 = 1000000;  ///< SYNC0周期 (纳秒)
    uint32_t cycleTime1 = 0;        ///< SYNC1周期 (纳秒)
    int32_t  shiftTime = 0;         ///< 移位时间 (纳秒)
    uint64_t systemTimeOffset = 0;  ///< 系统时间偏移

    QJsonObject toJson() const {
        QJsonObject obj;
        obj["enabled"] = enabled;
        obj["syncType"] = static_cast<int>(syncType);
        obj["cycleTime0"] = static_cast<qint64>(cycleTime0);
        obj["cycleTime1"] = static_cast<qint64>(cycleTime1);
        obj["shiftTime"] = shiftTime;
        return obj;
    }

    static DCConfig fromJson(const QJsonObject& json) {
        DCConfig cfg;
        cfg.enabled = json["enabled"].toBool();
        cfg.syncType = static_cast<DCSyncType>(json["syncType"].toInt());
        cfg.cycleTime0 = static_cast<uint32_t>(json["cycleTime0"].toDouble());
        cfg.cycleTime1 = static_cast<uint32_t>(json["cycleTime1"].toDouble());
        cfg.shiftTime = json["shiftTime"].toInt();
        return cfg;
    }
};

// ============================================================
// PDO映射
// ============================================================

/**
 * @brief PDO映射条目
 */
struct PDOEntry {
    uint16_t index = 0;         ///< 对象字典索引
    uint8_t  subIndex = 0;      ///< 子索引
    uint8_t  bitLength = 0;     ///< 位长度
    QString  name;              ///< 条目名称
    ECDataType dataType = ECDataType::UNSIGNED8;

    uint32_t toMappingValue() const {
        // 格式: [Index:16][SubIndex:8][BitLength:8]
        return (static_cast<uint32_t>(index) << 16) |
               (static_cast<uint32_t>(subIndex) << 8) |
               bitLength;
    }

    static PDOEntry fromMappingValue(uint32_t value) {
        PDOEntry entry;
        entry.index = static_cast<uint16_t>(value >> 16);
        entry.subIndex = static_cast<uint8_t>((value >> 8) & 0xFF);
        entry.bitLength = static_cast<uint8_t>(value & 0xFF);
        return entry;
    }

    QJsonObject toJson() const {
        QJsonObject obj;
        obj["index"] = index;
        obj["subIndex"] = subIndex;
        obj["bitLength"] = bitLength;
        obj["name"] = name;
        obj["dataType"] = static_cast<int>(dataType);
        return obj;
    }

    static PDOEntry fromJson(const QJsonObject& json) {
        PDOEntry entry;
        entry.index = static_cast<uint16_t>(json["index"].toInt());
        entry.subIndex = static_cast<uint8_t>(json["subIndex"].toInt());
        entry.bitLength = static_cast<uint8_t>(json["bitLength"].toInt());
        entry.name = json["name"].toString();
        entry.dataType = static_cast<ECDataType>(json["dataType"].toInt());
        return entry;
    }
};

/**
 * @brief PDO映射 (Rx或Tx)
 */
struct PDOMapping {
    uint16_t pdoIndex = 0;      ///< PDO索引 (0x1600-0x17FF RxPDO, 0x1A00-0x1BFF TxPDO)
    QString  name;              ///< PDO名称
    uint8_t  smIndex = 0;       ///< 关联的SM索引
    std::vector<PDOEntry> entries;

    int totalBitSize() const {
        int bits = 0;
        for (const auto& e : entries) bits += e.bitLength;
        return bits;
    }

    int totalByteSize() const {
        return (totalBitSize() + 7) / 8;
    }

    QJsonObject toJson() const {
        QJsonObject obj;
        obj["pdoIndex"] = pdoIndex;
        obj["name"] = name;
        obj["smIndex"] = smIndex;

        QJsonArray entriesArray;
        for (const auto& entry : entries) {
            entriesArray.append(entry.toJson());
        }
        obj["entries"] = entriesArray;
        return obj;
    }

    static PDOMapping fromJson(const QJsonObject& json) {
        PDOMapping pdo;
        pdo.pdoIndex = static_cast<uint16_t>(json["pdoIndex"].toInt());
        pdo.name = json["name"].toString();
        pdo.smIndex = static_cast<uint8_t>(json["smIndex"].toInt());

        QJsonArray entriesArray = json["entries"].toArray();
        for (const auto& val : entriesArray) {
            pdo.entries.push_back(PDOEntry::fromJson(val.toObject()));
        }
        return pdo;
    }
};

// ============================================================
// SDO访问
// ============================================================

/**
 * @brief SDO访问类型
 */
enum class SDOAccessType : uint8_t {
    Read  = 0x01,   ///< 上传 (从站->主站)
    Write = 0x02    ///< 下载 (主站->从站)
};

/**
 * @brief SDO传输类型
 */
enum class SDOTransferType : uint8_t {
    Expedited = 0x00,   ///< 快速传输 (<=4字节)
    Normal    = 0x01,   ///< 普通传输 (分段)
    Segmented = 0x02    ///< 分段传输
};

/**
 * @brief SDO请求结构
 */
struct SDORequest {
    uint16_t slavePosition = 0; ///< 从站位置
    uint16_t index = 0;         ///< 对象字典索引
    uint8_t  subIndex = 0;      ///< 子索引
    SDOAccessType accessType = SDOAccessType::Read;
    QByteArray data;            ///< 写入数据 (仅写操作)
    int timeout = 3000;         ///< 超时时间 (ms)
    bool completeAccess = false; ///< 完整访问模式
};

/**
 * @brief SDO响应结构
 */
struct SDOResponse {
    bool success = false;
    int errorCode = 0;
    QString errorMessage;
    QByteArray data;            ///< 读取数据
    uint32_t abortCode = 0;     ///< SDO中止码
    qint64 responseTime = 0;    ///< 响应时间 (ms)

    static SDOResponse Success(const QByteArray& data = QByteArray()) {
        SDOResponse resp;
        resp.success = true;
        resp.data = data;
        return resp;
    }

    static SDOResponse Error(int code, const QString& message) {
        SDOResponse resp;
        resp.success = false;
        resp.errorCode = code;
        resp.errorMessage = message;
        return resp;
    }

    static SDOResponse Abort(uint32_t abortCode) {
        SDOResponse resp;
        resp.success = false;
        resp.abortCode = abortCode;
        resp.errorMessage = getSDOAbortMessage(abortCode);
        return resp;
    }

    static QString getSDOAbortMessage(uint32_t code) {
        switch (code) {
        case 0x05030000: return QStringLiteral("Toggle bit not changed");
        case 0x05040001: return QStringLiteral("SDO command specifier invalid");
        case 0x06010000: return QStringLiteral("Object does not exist");
        case 0x06010002: return QStringLiteral("Object cannot be mapped to PDO");
        case 0x06020000: return QStringLiteral("Object does not exist in dictionary");
        case 0x06040041: return QStringLiteral("Object cannot be read");
        case 0x06040042: return QStringLiteral("Object cannot be written");
        case 0x06060000: return QStringLiteral("Access failed due to hardware error");
        case 0x06070010: return QStringLiteral("Data type does not match");
        case 0x06070012: return QStringLiteral("Data type length too high");
        case 0x06070013: return QStringLiteral("Data type length too low");
        case 0x06090011: return QStringLiteral("Sub-index does not exist");
        case 0x06090030: return QStringLiteral("Value range exceeded");
        case 0x08000000: return QStringLiteral("General error");
        case 0x08000020: return QStringLiteral("Data cannot be transferred");
        case 0x08000021: return QStringLiteral("Data cannot be transferred - local control");
        case 0x08000022: return QStringLiteral("Data cannot be transferred - device state");
        default: return QString("SDO Abort: 0x%1").arg(code, 8, 16, QChar('0'));
        }
    }
};

// ============================================================
// 从站信息
// ============================================================

/**
 * @brief EtherCAT从站完整信息
 */
struct ECSlaveInfo {
    // 基本信息
    uint16_t position = 0;          ///< 网络位置 (从1开始)
    uint16_t configuredAddress = 0; ///< 配置站地址
    QString  name;                  ///< 从站名称
    ECSlaveIdentity identity;       ///< 身份标识

    // 状态
    ECState currentState = ECState::None;
    ECState requestedState = ECState::None;
    ALStatusCode alStatusCode = ALStatusCode::NoError;

    // 邮箱信息
    uint16_t mailboxOutAddr = 0;    ///< 发送邮箱地址
    uint16_t mailboxOutSize = 0;    ///< 发送邮箱大小
    uint16_t mailboxInAddr = 0;     ///< 接收邮箱地址
    uint16_t mailboxInSize = 0;     ///< 接收邮箱大小

    // 支持的协议
    bool supportsCoE = false;       ///< CAN over EtherCAT
    bool supportsEoE = false;       ///< Ethernet over EtherCAT
    bool supportsFoE = false;       ///< File over EtherCAT
    bool supportsSoE = false;       ///< Servo over EtherCAT
    bool supportsVoE = false;       ///< Vendor over EtherCAT

    // PDO映射
    std::vector<PDOMapping> rxPDOs; ///< RxPDO (主站->从站)
    std::vector<PDOMapping> txPDOs; ///< TxPDO (从站->主站)

    // SM配置
    std::vector<SMConfig> smConfigs;

    // DC配置
    DCConfig dcConfig;
    bool supportsDC = false;

    // 过程数据偏移
    uint32_t outputOffset = 0;      ///< 输出数据在IOmap中的偏移
    uint32_t outputBitSize = 0;     ///< 输出位数
    uint32_t inputOffset = 0;       ///< 输入数据在IOmap中的偏移
    uint32_t inputBitSize = 0;      ///< 输入位数

    // 诊断计数器
    struct {
        uint32_t rxErrors = 0;
        uint32_t forwardedRxErrors = 0;
        uint32_t processingErrors = 0;
        uint32_t lostLinks = 0;
    } errorCounters;

    int outputByteSize() const { return (outputBitSize + 7) / 8; }
    int inputByteSize() const { return (inputBitSize + 7) / 8; }

    bool hasError() const {
        return VisionForge::PLC::hasError(currentState) ||
               alStatusCode != ALStatusCode::NoError;
    }

    QString toString() const {
        return QString("Slave[%1] %2 - %3 (%4)")
            .arg(position)
            .arg(name)
            .arg(getECStateName(currentState))
            .arg(identity.toString());
    }

    QJsonObject toJson() const {
        QJsonObject obj;
        obj["position"] = position;
        obj["configuredAddress"] = configuredAddress;
        obj["name"] = name;
        obj["identity"] = identity.toJson();
        obj["currentState"] = static_cast<int>(currentState);
        obj["supportsCoE"] = supportsCoE;
        obj["supportsDC"] = supportsDC;
        obj["dcConfig"] = dcConfig.toJson();
        obj["outputBitSize"] = static_cast<qint64>(outputBitSize);
        obj["inputBitSize"] = static_cast<qint64>(inputBitSize);

        // PDO映射
        QJsonArray rxPDOsArray, txPDOsArray;
        for (const auto& pdo : rxPDOs) rxPDOsArray.append(pdo.toJson());
        for (const auto& pdo : txPDOs) txPDOsArray.append(pdo.toJson());
        obj["rxPDOs"] = rxPDOsArray;
        obj["txPDOs"] = txPDOsArray;

        return obj;
    }
};

// ============================================================
// 主站配置
// ============================================================

/**
 * @brief EtherCAT主站配置
 */
struct ECMasterConfig {
    QString name = "EtherCAT Master";
    QString networkInterface;       ///< 网络接口名 (eth0, \\Device\\NPF_...)
    uint32_t cycleTime = 1000;      ///< 周期时间 (微秒)

    // 超时设置
    int connectTimeout = 5000;      ///< 连接超时 (ms)
    int sdoTimeout = 3000;          ///< SDO超时 (ms)
    int stateChangeTimeout = 10000; ///< 状态切换超时 (ms)

    // 自动配置
    bool autoConfig = true;         ///< 自动配置从站
    bool autoDC = true;             ///< 自动配置分布式时钟

    // 重试设置
    int maxRetries = 3;
    int retryInterval = 500;        ///< ms

    // 看门狗
    bool enableWatchdog = true;
    int watchdogTimeout = 100;      ///< ms

    // 日志
    bool enableDetailLog = false;

    QJsonObject toJson() const {
        QJsonObject obj;
        obj["name"] = name;
        obj["networkInterface"] = networkInterface;
        obj["cycleTime"] = static_cast<qint64>(cycleTime);
        obj["connectTimeout"] = connectTimeout;
        obj["sdoTimeout"] = sdoTimeout;
        obj["stateChangeTimeout"] = stateChangeTimeout;
        obj["autoConfig"] = autoConfig;
        obj["autoDC"] = autoDC;
        obj["maxRetries"] = maxRetries;
        obj["enableWatchdog"] = enableWatchdog;
        obj["watchdogTimeout"] = watchdogTimeout;
        return obj;
    }

    static ECMasterConfig fromJson(const QJsonObject& json) {
        ECMasterConfig cfg;
        cfg.name = json["name"].toString("EtherCAT Master");
        cfg.networkInterface = json["networkInterface"].toString();
        cfg.cycleTime = static_cast<uint32_t>(json["cycleTime"].toDouble(1000));
        cfg.connectTimeout = json["connectTimeout"].toInt(5000);
        cfg.sdoTimeout = json["sdoTimeout"].toInt(3000);
        cfg.stateChangeTimeout = json["stateChangeTimeout"].toInt(10000);
        cfg.autoConfig = json["autoConfig"].toBool(true);
        cfg.autoDC = json["autoDC"].toBool(true);
        cfg.maxRetries = json["maxRetries"].toInt(3);
        cfg.enableWatchdog = json["enableWatchdog"].toBool(true);
        cfg.watchdogTimeout = json["watchdogTimeout"].toInt(100);
        return cfg;
    }
};

// ============================================================
// 主站状态
// ============================================================

/**
 * @brief 主站连接状态
 */
enum class ECMasterStatus {
    Disconnected = 0,   ///< 未连接
    Initializing,       ///< 正在初始化
    Scanning,           ///< 正在扫描从站
    Configuring,        ///< 正在配置
    Ready,              ///< 就绪 (所有从站PreOp)
    Running,            ///< 运行中 (所有从站Operational)
    Error               ///< 错误状态
};

/**
 * @brief 获取主站状态名称
 */
inline QString getECMasterStatusName(ECMasterStatus status) {
    switch (status) {
    case ECMasterStatus::Disconnected:  return QStringLiteral("Disconnected");
    case ECMasterStatus::Initializing:  return QStringLiteral("Initializing");
    case ECMasterStatus::Scanning:      return QStringLiteral("Scanning");
    case ECMasterStatus::Configuring:   return QStringLiteral("Configuring");
    case ECMasterStatus::Ready:         return QStringLiteral("Ready");
    case ECMasterStatus::Running:       return QStringLiteral("Running");
    case ECMasterStatus::Error:         return QStringLiteral("Error");
    default:                            return QStringLiteral("Unknown");
    }
}

// ============================================================
// 操作结果
// ============================================================

/**
 * @brief EtherCAT操作结果
 */
struct ECResult {
    bool success = false;
    int errorCode = 0;
    QString errorMessage;
    QByteArray data;
    qint64 responseTime = 0;

    static ECResult Success() {
        ECResult r;
        r.success = true;
        return r;
    }

    static ECResult Success(const QByteArray& data) {
        ECResult r;
        r.success = true;
        r.data = data;
        return r;
    }

    static ECResult Error(int code, const QString& message) {
        ECResult r;
        r.success = false;
        r.errorCode = code;
        r.errorMessage = message;
        return r;
    }
};

// ============================================================
// 错误码
// ============================================================

namespace ECErrorCode {
    constexpr int Success               = 0;
    constexpr int InvalidParameter      = 1;
    constexpr int NotConnected          = 2;
    constexpr int Timeout               = 3;
    constexpr int InitFailed            = 4;
    constexpr int NoSlaveFound          = 5;
    constexpr int ConfigFailed          = 6;
    constexpr int StateChangeFailed     = 7;
    constexpr int SDOAborted            = 8;
    constexpr int PDOSizeError          = 9;
    constexpr int WorkcounterError      = 10;
    constexpr int DCConfigFailed        = 11;
    constexpr int SlaveError            = 12;
    constexpr int InterfaceNotFound     = 13;
    constexpr int PermissionDenied      = 14;
    constexpr int InternalError         = 99;
}

/**
 * @brief 获取错误码描述
 */
inline QString getECErrorMessage(int code) {
    switch (code) {
    case ECErrorCode::Success:          return QStringLiteral("Success");
    case ECErrorCode::InvalidParameter: return QStringLiteral("Invalid parameter");
    case ECErrorCode::NotConnected:     return QStringLiteral("Not connected");
    case ECErrorCode::Timeout:          return QStringLiteral("Timeout");
    case ECErrorCode::InitFailed:       return QStringLiteral("Initialization failed");
    case ECErrorCode::NoSlaveFound:     return QStringLiteral("No slave found");
    case ECErrorCode::ConfigFailed:     return QStringLiteral("Configuration failed");
    case ECErrorCode::StateChangeFailed:return QStringLiteral("State change failed");
    case ECErrorCode::SDOAborted:       return QStringLiteral("SDO aborted");
    case ECErrorCode::PDOSizeError:     return QStringLiteral("PDO size mismatch");
    case ECErrorCode::WorkcounterError: return QStringLiteral("Workcounter error");
    case ECErrorCode::DCConfigFailed:   return QStringLiteral("DC configuration failed");
    case ECErrorCode::SlaveError:       return QStringLiteral("Slave error");
    case ECErrorCode::InterfaceNotFound:return QStringLiteral("Network interface not found");
    case ECErrorCode::PermissionDenied: return QStringLiteral("Permission denied");
    case ECErrorCode::InternalError:    return QStringLiteral("Internal error");
    default:                            return QString("Error code: %1").arg(code);
    }
}

// ============================================================
// 统计信息
// ============================================================

/**
 * @brief EtherCAT通信统计
 */
struct ECStatistics {
    // 帧统计
    uint64_t txFrameCount = 0;      ///< 发送帧数
    uint64_t rxFrameCount = 0;      ///< 接收帧数
    uint64_t lostFrameCount = 0;    ///< 丢失帧数

    // 循环统计
    uint64_t cycleCount = 0;        ///< 周期计数
    uint64_t wcErrors = 0;          ///< Workcounter错误

    // 时间统计 (纳秒)
    uint64_t minCycleTime = UINT64_MAX;
    uint64_t maxCycleTime = 0;
    uint64_t avgCycleTime = 0;
    uint64_t lastCycleTime = 0;

    // SDO统计
    uint64_t sdoReadCount = 0;
    uint64_t sdoWriteCount = 0;
    uint64_t sdoErrorCount = 0;

    // 状态变化计数
    uint64_t stateChangeCount = 0;

    QDateTime startTime;
    QDateTime lastUpdateTime;

    double getLostFrameRate() const {
        uint64_t total = txFrameCount + rxFrameCount;
        return total > 0 ? static_cast<double>(lostFrameCount) / total * 100.0 : 0.0;
    }

    double getWCErrorRate() const {
        return cycleCount > 0 ? static_cast<double>(wcErrors) / cycleCount * 100.0 : 0.0;
    }

    void reset() {
        *this = ECStatistics();
        startTime = QDateTime::currentDateTime();
    }

    QJsonObject toJson() const {
        QJsonObject obj;
        obj["txFrameCount"] = static_cast<qint64>(txFrameCount);
        obj["rxFrameCount"] = static_cast<qint64>(rxFrameCount);
        obj["lostFrameCount"] = static_cast<qint64>(lostFrameCount);
        obj["cycleCount"] = static_cast<qint64>(cycleCount);
        obj["wcErrors"] = static_cast<qint64>(wcErrors);
        obj["avgCycleTime"] = static_cast<qint64>(avgCycleTime);
        obj["sdoReadCount"] = static_cast<qint64>(sdoReadCount);
        obj["sdoWriteCount"] = static_cast<qint64>(sdoWriteCount);
        return obj;
    }
};

// ============================================================
// 回调类型定义
// ============================================================

/// 状态变化回调
using ECStateChangeCallback = std::function<void(uint16_t slavePos, ECState oldState, ECState newState)>;

/// 错误回调
using ECErrorCallback = std::function<void(uint16_t slavePos, int errorCode, const QString& message)>;

/// 周期数据回调
using ECCyclicCallback = std::function<void(const QByteArray& inputs, QByteArray& outputs)>;

/// SDO完成回调
using ECSDOCallback = std::function<void(const SDOResponse& response)>;

}  // namespace PLC
}  // namespace VisionForge

#endif // VISIONFORGE_PLC_ETHERCAT_TYPES_H
