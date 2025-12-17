/**
 * @file PLCManager.cpp
 * @brief PLC通信管理器实现
 * @author VisionForge Team
 * @date 2025-12-17
 */

#include "comm/PLCManager.h"
#include "base/Logger.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

namespace VisionForge {
namespace Comm {

PLCManager& PLCManager::instance()
{
    static PLCManager instance;
    return instance;
}

PLCManager::PLCManager(QObject* parent)
    : QObject(parent)
{
    LOG_INFO("PLC管理器初始化");
}

PLCManager::~PLCManager()
{
    disconnectAll();
    LOG_INFO("PLC管理器销毁");
}

bool PLCManager::addConnection(const QString& name, const PLCConfig& config)
{
    if (name.isEmpty()) {
        LOG_ERROR("连接名称不能为空");
        return false;
    }

    // 创建通信对象 (在锁外创建，避免长时间持有锁)
    auto comm = createPLCComm(config.protocol);
    if (!comm) {
        LOG_ERROR(QString("无法创建协议%1的通信对象").arg(static_cast<int>(config.protocol)));
        return false;
    }

    // 连接信号
    QObject::connect(comm.get(), &PLCComm::connectionChanged, this,
                     [this, name](bool connected) {
                         emit connectionStatusChanged(name, connected);
                     });

    QObject::connect(comm.get(), &PLCComm::errorOccurred, this,
                     [this, name](const QString& error) {
                         emit errorOccurred(name, error);
                     });

    {
        QMutexLocker locker(&mutex_);

        if (connections_.contains(name)) {
            LOG_WARNING(QString("连接'%1'已存在，将被覆盖").arg(name));
            connections_[name]->disconnect();
        }

        connections_[name] = comm;
        configs_[name] = config;
        configs_[name].name = name;
    }

    LOG_INFO(QString("添加PLC连接: %1 (%2)").arg(name).arg(getProtocolName(config.protocol)));

    // 在锁外发出信号，避免死锁
    emit connectionsChanged();

    return true;
}

bool PLCManager::removeConnection(const QString& name)
{
    {
        QMutexLocker locker(&mutex_);

        if (!connections_.contains(name)) {
            LOG_WARNING(QString("连接'%1'不存在").arg(name));
            return false;
        }

        connections_[name]->disconnect();
        connections_.remove(name);
        configs_.remove(name);
    }

    LOG_INFO(QString("移除PLC连接: %1").arg(name));

    // 在锁外发出信号，避免死锁
    emit connectionsChanged();

    return true;
}

std::shared_ptr<PLCComm> PLCManager::getConnection(const QString& name)
{
    QMutexLocker locker(&mutex_);

    if (!connections_.contains(name)) {
        return nullptr;
    }

    return connections_[name];
}

QStringList PLCManager::getConnectionNames() const
{
    QMutexLocker locker(&mutex_);
    return connections_.keys();
}

PLCConfig PLCManager::getConfig(const QString& name) const
{
    QMutexLocker locker(&mutex_);

    if (!configs_.contains(name)) {
        return PLCConfig();
    }

    return configs_[name];
}

bool PLCManager::connectPLC(const QString& name)
{
    auto comm = getConnection(name);
    if (!comm) {
        LOG_ERROR(QString("连接'%1'不存在").arg(name));
        return false;
    }

    PLCConfig config;
    {
        QMutexLocker locker(&mutex_);
        config = configs_[name];
    }

    LOG_INFO(QString("正在连接PLC: %1").arg(name));
    return comm->connect(config);
}

void PLCManager::disconnectPLC(const QString& name)
{
    auto comm = getConnection(name);
    if (comm) {
        LOG_INFO(QString("断开PLC连接: %1").arg(name));
        comm->disconnect();
    }
}

void PLCManager::connectAll()
{
    QStringList names = getConnectionNames();
    for (const QString& name : names) {
        connectPLC(name);
    }
}

void PLCManager::disconnectAll()
{
    QStringList names = getConnectionNames();
    for (const QString& name : names) {
        disconnectPLC(name);
    }
}

bool PLCManager::isConnected(const QString& name) const
{
    QMutexLocker locker(&mutex_);

    if (!connections_.contains(name)) {
        return false;
    }

    return connections_[name]->isConnected();
}

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

bool PLCManager::saveConfig(const QString& filePath)
{
    QMutexLocker locker(&mutex_);

    QJsonObject root;
    QJsonArray connectionsArray;

    for (auto it = configs_.begin(); it != configs_.end(); ++it) {
        QVariantMap configMap = serializeConfig(it.value());
        connectionsArray.append(QJsonObject::fromVariantMap(configMap));
    }

    root["connections"] = connectionsArray;

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        LOG_ERROR(QString("无法打开文件: %1").arg(filePath));
        return false;
    }

    QJsonDocument doc(root);
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();

    LOG_INFO(QString("PLC配置已保存到: %1").arg(filePath));
    return true;
}

bool PLCManager::loadConfig(const QString& filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        LOG_ERROR(QString("无法打开文件: %1").arg(filePath));
        return false;
    }

    QByteArray data = file.readAll();
    file.close();

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

    // 加载配置
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

QVariantMap PLCManager::serializeConfig(const PLCConfig& config) const
{
    QVariantMap map;
    map["name"] = config.name;
    map["protocol"] = static_cast<int>(config.protocol);
    map["slaveId"] = config.slaveId;
    map["networkNo"] = config.networkNo;
    map["pcNo"] = config.pcNo;
    map["destModuleIo"] = config.destModuleIo;
    map["destModuleStation"] = config.destModuleStation;

    if (config.commConfig) {
        QVariantMap commMap;
        commMap["type"] = static_cast<int>(config.commConfig->type);
        commMap["timeout"] = config.commConfig->timeout;
        commMap["retryCount"] = config.commConfig->retryCount;

        if (config.commConfig->type == CommType::Socket) {
            auto socketConfig = std::dynamic_pointer_cast<SocketConfig>(config.commConfig);
            if (socketConfig) {
                commMap["ipAddress"] = socketConfig->ipAddress;
                commMap["port"] = socketConfig->port;
                commMap["useTcp"] = socketConfig->useTcp;
            }
        } else if (config.commConfig->type == CommType::Serial) {
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

PLCConfig PLCManager::deserializeConfig(const QVariantMap& map) const
{
    PLCConfig config;
    config.name = map["name"].toString();
    config.protocol = static_cast<PLCProtocol>(map["protocol"].toInt());
    config.slaveId = map["slaveId"].toInt();
    config.networkNo = map["networkNo"].toInt();
    config.pcNo = map["pcNo"].toInt();
    config.destModuleIo = map["destModuleIo"].toInt();
    config.destModuleStation = map["destModuleStation"].toInt();

    QVariantMap commMap = map["commConfig"].toMap();
    CommType commType = static_cast<CommType>(commMap["type"].toInt());

    if (commType == CommType::Socket) {
        auto socketConfig = std::make_shared<SocketConfig>();
        socketConfig->timeout = commMap["timeout"].toInt();
        socketConfig->retryCount = commMap["retryCount"].toInt();
        socketConfig->ipAddress = commMap["ipAddress"].toString();
        socketConfig->port = static_cast<quint16>(commMap["port"].toInt());
        socketConfig->useTcp = commMap["useTcp"].toBool();
        config.commConfig = socketConfig;
    } else if (commType == CommType::Serial) {
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
