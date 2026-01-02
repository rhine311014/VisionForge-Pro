/**
 * @file PLCManager.cpp
 * @brief PLC通信管理器实现文件
 * @author VisionForge Team
 * @date 2025-12-17
 *
 * @details
 * 本文件实现了PLCManager类的所有功能，包括：
 * - 单例模式的实现
 * - PLC连接的生命周期管理
 * - 配置的JSON序列化和反序列化
 * - 线程安全的读写操作封装
 *
 * ## 线程安全策略
 * - 使用QMutex保护共享数据结构
 * - 信号在锁外发出，避免死锁
 * - 创建通信对象在锁外进行，减少锁持有时间
 *
 * ## 配置文件格式
 * 使用JSON格式存储配置，支持Socket和Serial两种通信类型。
 */

#include "comm/PLCManager.h"
#include "base/Logger.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

namespace VisionForge {
namespace Comm {

/**
 * @brief 获取PLCManager单例实例
 * @return PLCManager的引用
 *
 * @details
 * 使用C++11的静态局部变量特性实现线程安全的单例模式。
 * 静态局部变量的初始化在C++11中保证是线程安全的。
 */
PLCManager& PLCManager::instance()
{
    static PLCManager instance;
    return instance;
}

/**
 * @brief 构造函数
 * @param parent 父对象指针
 *
 * @details
 * 初始化PLC管理器，记录初始化日志。
 */
PLCManager::PLCManager(QObject* parent)
    : QObject(parent)
{
    LOG_INFO("PLC管理器初始化");
}

/**
 * @brief 析构函数
 *
 * @details
 * 在析构时断开所有PLC连接，确保资源正确释放。
 */
PLCManager::~PLCManager()
{
    disconnectAll();
    LOG_INFO("PLC管理器销毁");
}

/**
 * @brief 添加PLC连接
 * @param name 连接名称
 * @param config PLC配置
 * @return 添加成功返回true
 *
 * @details
 * 实现流程：
 * 1. 验证连接名称非空
 * 2. 在锁外创建通信对象（避免长时间持锁）
 * 3. 连接通信对象的信号到管理器的信号
 * 4. 在锁内将对象添加到映射表
 * 5. 在锁外发出connectionsChanged信号
 *
 * ## 信号转发
 * 通信对象的connectionChanged和errorOccurred信号
 * 会被转发为管理器的对应信号，并附加连接名称信息。
 */
bool PLCManager::addConnection(const QString& name, const PLCConfig& config)
{
    // 验证连接名称
    if (name.isEmpty()) {
        LOG_ERROR("连接名称不能为空");
        return false;
    }

    // 在锁外创建通信对象，避免长时间持有锁
    auto comm = createPLCComm(config.protocol);
    if (!comm) {
        LOG_ERROR(QString("无法创建协议%1的通信对象").arg(static_cast<int>(config.protocol)));
        return false;
    }

    // 连接通信对象的信号到管理器的信号
    // 使用Lambda捕获连接名称，实现信号转发
    QObject::connect(comm.get(), &PLCComm::connectionChanged, this,
                     [this, name](bool connected) {
                         emit connectionStatusChanged(name, connected);
                     });

    QObject::connect(comm.get(), &PLCComm::errorOccurred, this,
                     [this, name](const QString& error) {
                         emit errorOccurred(name, error);
                     });

    // 临界区：访问共享数据结构
    {
        QMutexLocker locker(&mutex_);

        // 如果连接已存在，先断开旧连接
        if (connections_.contains(name)) {
            LOG_WARNING(QString("连接'%1'已存在，将被覆盖").arg(name));
            connections_[name]->disconnect();
        }

        // 添加新连接
        connections_[name] = comm;
        configs_[name] = config;
        configs_[name].name = name;  // 确保配置中的名称与键一致
    }

    LOG_INFO(QString("添加PLC连接: %1 (%2)").arg(name).arg(getProtocolName(config.protocol)));

    // 在锁外发出信号，避免潜在的死锁
    emit connectionsChanged();

    return true;
}

/**
 * @brief 移除PLC连接
 * @param name 连接名称
 * @return 移除成功返回true
 *
 * @details
 * 实现流程：
 * 1. 在锁内检查连接是否存在
 * 2. 断开连接并从映射表移除
 * 3. 在锁外发出信号
 */
bool PLCManager::removeConnection(const QString& name)
{
    // 临界区
    {
        QMutexLocker locker(&mutex_);

        if (!connections_.contains(name)) {
            LOG_WARNING(QString("连接'%1'不存在").arg(name));
            return false;
        }

        // 断开并移除
        connections_[name]->disconnect();
        connections_.remove(name);
        configs_.remove(name);
    }

    LOG_INFO(QString("移除PLC连接: %1").arg(name));

    // 在锁外发出信号
    emit connectionsChanged();

    return true;
}

/**
 * @brief 获取PLC通信对象
 * @param name 连接名称
 * @return PLC通信对象的智能指针
 *
 * @details
 * 返回智能指针确保对象不会在使用过程中被意外删除。
 */
std::shared_ptr<PLCComm> PLCManager::getConnection(const QString& name)
{
    QMutexLocker locker(&mutex_);

    if (!connections_.contains(name)) {
        return nullptr;
    }

    return connections_[name];
}

/**
 * @brief 获取所有连接名称
 * @return 连接名称列表
 */
QStringList PLCManager::getConnectionNames() const
{
    QMutexLocker locker(&mutex_);
    return connections_.keys();
}

/**
 * @brief 获取连接配置
 * @param name 连接名称
 * @return PLC配置，不存在则返回默认配置
 */
PLCConfig PLCManager::getConfig(const QString& name) const
{
    QMutexLocker locker(&mutex_);

    if (!configs_.contains(name)) {
        return PLCConfig();
    }

    return configs_[name];
}

/**
 * @brief 连接指定PLC
 * @param name 连接名称
 * @return 连接成功返回true
 *
 * @details
 * 获取通信对象和配置后，调用通信对象的connect方法。
 * 连接过程在锁外进行，避免阻塞其他操作。
 */
bool PLCManager::connectPLC(const QString& name)
{
    // 获取通信对象
    auto comm = getConnection(name);
    if (!comm) {
        LOG_ERROR(QString("连接'%1'不存在").arg(name));
        return false;
    }

    // 获取配置
    PLCConfig config;
    {
        QMutexLocker locker(&mutex_);
        config = configs_[name];
    }

    LOG_INFO(QString("正在连接PLC: %1").arg(name));

    // 连接操作在锁外进行（可能耗时）
    return comm->connect(config);
}

/**
 * @brief 断开指定PLC连接
 * @param name 连接名称
 */
void PLCManager::disconnectPLC(const QString& name)
{
    auto comm = getConnection(name);
    if (comm) {
        LOG_INFO(QString("断开PLC连接: %1").arg(name));
        comm->disconnect();
    }
}

/**
 * @brief 连接所有PLC
 *
 * @details
 * 遍历所有已添加的连接并尝试连接。
 * 先获取名称列表，然后逐个连接，避免长时间持锁。
 */
void PLCManager::connectAll()
{
    // 先获取名称列表
    QStringList names = getConnectionNames();

    // 逐个连接
    for (const QString& name : names) {
        connectPLC(name);
    }
}

/**
 * @brief 断开所有PLC连接
 *
 * @details
 * 遍历所有连接并断开。
 */
void PLCManager::disconnectAll()
{
    QStringList names = getConnectionNames();
    for (const QString& name : names) {
        disconnectPLC(name);
    }
}

/**
 * @brief 检查连接状态
 * @param name 连接名称
 * @return 已连接返回true
 */
bool PLCManager::isConnected(const QString& name) const
{
    QMutexLocker locker(&mutex_);

    if (!connections_.contains(name)) {
        return false;
    }

    return connections_[name]->isConnected();
}

/**
 * @brief 读取寄存器
 * @param connectionName 连接名称
 * @param address 起始地址
 * @param count 读取数量
 * @return 读取结果
 *
 * @details
 * 封装了获取通信对象和调用读取方法的过程。
 * 连接不存在时返回Disconnected状态。
 */
PLCResult PLCManager::readRegisters(const QString& connectionName, int address, int count)
{
    auto comm = getConnection(connectionName);
    if (!comm) {
        PLCResult result;
        result.status = CommStatus::Disconnected;
        result.errorMessage = QString("连接'%1'不存在").arg(connectionName);
        return result;
    }

    return comm->readRegisters(address, count);
}

/**
 * @brief 写入寄存器
 * @param connectionName 连接名称
 * @param address 起始地址
 * @param values 要写入的值
 * @return 写入结果
 */
PLCResult PLCManager::writeRegisters(const QString& connectionName, int address, const std::vector<int>& values)
{
    auto comm = getConnection(connectionName);
    if (!comm) {
        PLCResult result;
        result.status = CommStatus::Disconnected;
        result.errorMessage = QString("连接'%1'不存在").arg(connectionName);
        return result;
    }

    return comm->writeRegisters(address, values);
}

/**
 * @brief 读取线圈
 * @param connectionName 连接名称
 * @param address 起始地址
 * @param count 读取数量
 * @return 读取结果
 */
PLCResult PLCManager::readCoils(const QString& connectionName, int address, int count)
{
    auto comm = getConnection(connectionName);
    if (!comm) {
        PLCResult result;
        result.status = CommStatus::Disconnected;
        result.errorMessage = QString("连接'%1'不存在").arg(connectionName);
        return result;
    }

    return comm->readCoils(address, count);
}

/**
 * @brief 写入线圈
 * @param connectionName 连接名称
 * @param address 起始地址
 * @param values 要写入的值
 * @return 写入结果
 */
PLCResult PLCManager::writeCoils(const QString& connectionName, int address, const std::vector<bool>& values)
{
    auto comm = getConnection(connectionName);
    if (!comm) {
        PLCResult result;
        result.status = CommStatus::Disconnected;
        result.errorMessage = QString("连接'%1'不存在").arg(connectionName);
        return result;
    }

    return comm->writeCoils(address, values);
}

/**
 * @brief 保存配置到文件
 * @param filePath 文件路径
 * @return 保存成功返回true
 *
 * @details
 * 将所有连接配置序列化为JSON格式并写入文件。
 *
 * ## JSON结构
 * @code{.json}
 * {
 *   "connections": [
 *     { ... 连接1配置 ... },
 *     { ... 连接2配置 ... }
 *   ]
 * }
 * @endcode
 */
bool PLCManager::saveConfig(const QString& filePath)
{
    QMutexLocker locker(&mutex_);

    // 构建JSON对象
    QJsonObject root;
    QJsonArray connectionsArray;

    // 遍历所有配置并序列化
    for (auto it = configs_.begin(); it != configs_.end(); ++it) {
        QVariantMap configMap = serializeConfig(it.value());
        connectionsArray.append(QJsonObject::fromVariantMap(configMap));
    }

    root["connections"] = connectionsArray;

    // 写入文件
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        LOG_ERROR(QString("无法打开文件: %1").arg(filePath));
        return false;
    }

    QJsonDocument doc(root);
    file.write(doc.toJson(QJsonDocument::Indented));  // 使用缩进格式，便于阅读
    file.close();

    LOG_INFO(QString("PLC配置已保存到: %1").arg(filePath));
    return true;
}

/**
 * @brief 从文件加载配置
 * @param filePath 文件路径
 * @return 加载成功返回true
 *
 * @details
 * 实现流程：
 * 1. 读取文件内容
 * 2. 解析JSON
 * 3. 清除现有连接
 * 4. 遍历配置数组并创建连接
 */
bool PLCManager::loadConfig(const QString& filePath)
{
    // 读取文件
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        LOG_ERROR(QString("无法打开文件: %1").arg(filePath));
        return false;
    }

    QByteArray data = file.readAll();
    file.close();

    // 解析JSON
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(data, &error);
    if (error.error != QJsonParseError::NoError) {
        LOG_ERROR(QString("JSON解析错误: %1").arg(error.errorString()));
        return false;
    }

    QJsonObject root = doc.object();
    QJsonArray connectionsArray = root["connections"].toArray();

    // 清除现有连接
    disconnectAll();
    {
        QMutexLocker locker(&mutex_);
        connections_.clear();
        configs_.clear();
    }

    // 加载新配置
    for (const QJsonValue& value : connectionsArray) {
        QVariantMap configMap = value.toObject().toVariantMap();
        PLCConfig config = deserializeConfig(configMap);

        if (!config.name.isEmpty()) {
            addConnection(config.name, config);
        }
    }

    LOG_INFO(QString("PLC配置已从%1加载").arg(filePath));
    return true;
}

/**
 * @brief 序列化PLC配置
 * @param config 要序列化的配置
 * @return QVariantMap格式的配置
 *
 * @details
 * 根据通信类型（Socket/Serial）序列化不同的参数。
 *
 * ## Socket配置参数
 * - ipAddress: IP地址
 * - port: 端口号
 * - useTcp: 是否使用TCP（false则为UDP）
 *
 * ## Serial配置参数
 * - portName: 串口名称（如COM1）
 * - baudRate: 波特率
 * - dataBits: 数据位
 * - stopBits: 停止位
 * - parity: 校验位
 */
QVariantMap PLCManager::serializeConfig(const PLCConfig& config) const
{
    QVariantMap map;

    // 基本配置
    map["name"] = config.name;
    map["protocol"] = static_cast<int>(config.protocol);
    map["slaveId"] = config.slaveId;

    // 三菱MC协议特有参数
    map["networkNo"] = config.networkNo;
    map["pcNo"] = config.pcNo;
    map["destModuleIo"] = config.destModuleIo;
    map["destModuleStation"] = config.destModuleStation;

    // 通信配置
    if (config.commConfig) {
        QVariantMap commMap;
        commMap["type"] = static_cast<int>(config.commConfig->type);
        commMap["timeout"] = config.commConfig->timeout;
        commMap["retryCount"] = config.commConfig->retryCount;

        // 根据通信类型序列化不同的参数
        if (config.commConfig->type == CommType::Socket) {
            // Socket配置
            auto socketConfig = std::dynamic_pointer_cast<SocketConfig>(config.commConfig);
            if (socketConfig) {
                commMap["ipAddress"] = socketConfig->ipAddress;
                commMap["port"] = socketConfig->port;
                commMap["useTcp"] = socketConfig->useTcp;
            }
        } else if (config.commConfig->type == CommType::Serial) {
            // 串口配置
            auto serialConfig = std::dynamic_pointer_cast<SerialConfig>(config.commConfig);
            if (serialConfig) {
                commMap["portName"] = serialConfig->portName;
                commMap["baudRate"] = serialConfig->baudRate;
                commMap["dataBits"] = serialConfig->dataBits;
                commMap["stopBits"] = serialConfig->stopBits;
                commMap["parity"] = serialConfig->parity;
            }
        }

        map["commConfig"] = commMap;
    }

    return map;
}

/**
 * @brief 反序列化PLC配置
 * @param map QVariantMap格式的配置
 * @return 反序列化后的PLCConfig
 *
 * @details
 * 根据通信类型创建相应的SocketConfig或SerialConfig对象。
 */
PLCConfig PLCManager::deserializeConfig(const QVariantMap& map) const
{
    PLCConfig config;

    // 基本配置
    config.name = map["name"].toString();
    config.protocol = static_cast<PLCProtocol>(map["protocol"].toInt());
    config.slaveId = map["slaveId"].toInt();

    // 三菱MC协议特有参数
    config.networkNo = map["networkNo"].toInt();
    config.pcNo = map["pcNo"].toInt();
    config.destModuleIo = map["destModuleIo"].toInt();
    config.destModuleStation = map["destModuleStation"].toInt();

    // 通信配置
    QVariantMap commMap = map["commConfig"].toMap();
    CommType commType = static_cast<CommType>(commMap["type"].toInt());

    if (commType == CommType::Socket) {
        // 创建Socket配置
        auto socketConfig = std::make_shared<SocketConfig>();
        socketConfig->timeout = commMap["timeout"].toInt();
        socketConfig->retryCount = commMap["retryCount"].toInt();
        socketConfig->ipAddress = commMap["ipAddress"].toString();
        socketConfig->port = static_cast<quint16>(commMap["port"].toInt());
        socketConfig->useTcp = commMap["useTcp"].toBool();
        config.commConfig = socketConfig;
    } else if (commType == CommType::Serial) {
        // 创建串口配置
        auto serialConfig = std::make_shared<SerialConfig>();
        serialConfig->timeout = commMap["timeout"].toInt();
        serialConfig->retryCount = commMap["retryCount"].toInt();
        serialConfig->portName = commMap["portName"].toString();
        serialConfig->baudRate = commMap["baudRate"].toInt();
        serialConfig->dataBits = commMap["dataBits"].toInt();
        serialConfig->stopBits = commMap["stopBits"].toInt();
        serialConfig->parity = commMap["parity"].toInt();
        config.commConfig = serialConfig;
    }

    return config;
}

} // namespace Comm
} // namespace VisionForge
