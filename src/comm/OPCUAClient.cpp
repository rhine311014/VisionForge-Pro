/**
 * @file OPCUAClient.cpp
 * @brief OPC UA客户端实现
 * @author VisionForge Team
 * @date 2025-12-22
 */

#include "comm/OPCUAClient.h"
#include "base/Logger.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDir>

namespace VisionForge {
namespace Comm {

// ============================================================
// OPCUANodeId 实现
// ============================================================

QString OPCUANodeId::toString() const
{
    if (isNumeric) {
        return QString("ns=%1;i=%2").arg(namespaceIndex).arg(numericId);
    } else {
        return QString("ns=%1;s=%2").arg(namespaceIndex).arg(identifier);
    }
}

OPCUANodeId OPCUANodeId::fromString(const QString& str)
{
    OPCUANodeId nodeId;

    // 解析格式: ns=X;i=Y 或 ns=X;s=Y
    QStringList parts = str.split(';');
    for (const QString& part : parts) {
        if (part.startsWith("ns=")) {
            nodeId.namespaceIndex = part.mid(3).toInt();
        } else if (part.startsWith("i=")) {
            nodeId.isNumeric = true;
            nodeId.numericId = part.mid(2).toUInt();
        } else if (part.startsWith("s=")) {
            nodeId.isNumeric = false;
            nodeId.identifier = part.mid(2);
        }
    }

    return nodeId;
}

bool OPCUANodeId::operator==(const OPCUANodeId& other) const
{
    if (namespaceIndex != other.namespaceIndex) return false;
    if (isNumeric != other.isNumeric) return false;
    if (isNumeric) {
        return numericId == other.numericId;
    } else {
        return identifier == other.identifier;
    }
}

bool OPCUANodeId::operator<(const OPCUANodeId& other) const
{
    if (namespaceIndex != other.namespaceIndex) {
        return namespaceIndex < other.namespaceIndex;
    }
    if (isNumeric != other.isNumeric) {
        return isNumeric < other.isNumeric;
    }
    if (isNumeric) {
        return numericId < other.numericId;
    } else {
        return identifier < other.identifier;
    }
}

// ============================================================
// OPCUANodeInfo 实现
// ============================================================

QJsonObject OPCUANodeInfo::toJson() const
{
    QJsonObject json;
    json["nodeId"] = nodeId.toString();
    json["displayName"] = displayName;
    json["browseName"] = browseName;
    json["description"] = description;
    json["nodeClass"] = nodeClass;
    json["dataType"] = dataType;
    json["isReadable"] = isReadable;
    json["isWritable"] = isWritable;

    QJsonArray childArray;
    for (const auto& child : children) {
        childArray.append(child.toString());
    }
    json["children"] = childArray;

    return json;
}

// ============================================================
// OPCUAClientConfig 实现
// ============================================================

QJsonObject OPCUAClientConfig::toJson() const
{
    QJsonObject json;
    json["endpointUrl"] = endpointUrl;
    json["applicationName"] = applicationName;
    json["applicationUri"] = applicationUri;
    json["productUri"] = productUri;
    json["securityPolicy"] = static_cast<int>(securityPolicy);
    json["securityMode"] = static_cast<int>(securityMode);
    json["certificatePath"] = certificatePath;
    json["privateKeyPath"] = privateKeyPath;
    json["trustListPath"] = trustListPath;
    json["useAuthentication"] = useAuthentication;
    json["username"] = username;
    // 不保存密码明文
    json["connectTimeoutMs"] = connectTimeoutMs;
    json["sessionTimeoutMs"] = sessionTimeoutMs;
    json["autoReconnect"] = autoReconnect;
    json["reconnectIntervalMs"] = reconnectIntervalMs;
    json["maxReconnectAttempts"] = maxReconnectAttempts;
    json["publishingIntervalMs"] = publishingIntervalMs;
    json["samplingIntervalMs"] = samplingIntervalMs;
    json["queueSize"] = queueSize;
    return json;
}

OPCUAClientConfig OPCUAClientConfig::fromJson(const QJsonObject& json)
{
    OPCUAClientConfig config;
    config.endpointUrl = json.value("endpointUrl").toString();
    config.applicationName = json.value("applicationName").toString("VisionForge");
    config.applicationUri = json.value("applicationUri").toString();
    config.productUri = json.value("productUri").toString();
    config.securityPolicy = static_cast<OPCUASecurityPolicy>(
        json.value("securityPolicy").toInt(0));
    config.securityMode = static_cast<OPCUASecurityMode>(
        json.value("securityMode").toInt(0));
    config.certificatePath = json.value("certificatePath").toString();
    config.privateKeyPath = json.value("privateKeyPath").toString();
    config.trustListPath = json.value("trustListPath").toString();
    config.useAuthentication = json.value("useAuthentication").toBool(false);
    config.username = json.value("username").toString();
    config.connectTimeoutMs = json.value("connectTimeoutMs").toInt(5000);
    config.sessionTimeoutMs = json.value("sessionTimeoutMs").toInt(60000);
    config.autoReconnect = json.value("autoReconnect").toBool(true);
    config.reconnectIntervalMs = json.value("reconnectIntervalMs").toInt(5000);
    config.maxReconnectAttempts = json.value("maxReconnectAttempts").toInt(10);
    config.publishingIntervalMs = json.value("publishingIntervalMs").toInt(1000);
    config.samplingIntervalMs = json.value("samplingIntervalMs").toInt(500);
    config.queueSize = json.value("queueSize").toInt(10);
    return config;
}

// ============================================================
// OPCUAClient 实现
// ============================================================

OPCUAClient::OPCUAClient(QObject* parent)
    : QObject(parent)
{
    reconnectTimer_ = new QTimer(this);
    keepAliveTimer_ = new QTimer(this);

    QObject::connect(reconnectTimer_, &QTimer::timeout, this, &OPCUAClient::onReconnectTimer);
    QObject::connect(keepAliveTimer_, &QTimer::timeout, this, &OPCUAClient::onKeepAliveTimer);
}

OPCUAClient::~OPCUAClient()
{
    disconnect();
}

void OPCUAClient::setConfig(const OPCUAClientConfig& config)
{
    QMutexLocker locker(&mutex_);
    config_ = config;
}

bool OPCUAClient::loadConfig(const QString& filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        setError(QString("无法打开配置文件: %1").arg(filePath));
        return false;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    if (doc.isNull() || !doc.isObject()) {
        setError("配置文件格式错误");
        return false;
    }

    setConfig(OPCUAClientConfig::fromJson(doc.object()));
    return true;
}

bool OPCUAClient::saveConfig(const QString& filePath) const
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }

    QJsonDocument doc(config_.toJson());
    file.write(doc.toJson(QJsonDocument::Indented));
    return true;
}

bool OPCUAClient::connect()
{
    QMutexLocker locker(&mutex_);

    if (state_ == OPCUAClientState::Connected) {
        return true;
    }

    if (config_.endpointUrl.isEmpty()) {
        setError("未配置服务器端点URL");
        return false;
    }

    setState(OPCUAClientState::Connecting);
    reconnectAttempts_ = 0;

    if (!initializeClient()) {
        setState(OPCUAClientState::Error);
        return false;
    }

    if (!doConnect()) {
        if (config_.autoReconnect) {
            startReconnect();
        } else {
            setState(OPCUAClientState::Error);
        }
        return false;
    }

    setState(OPCUAClientState::Connected);
    keepAliveTimer_->start(10000);  // 10秒心跳

    LOG_INFO(QString("OPC UA已连接到: %1").arg(config_.endpointUrl));
    emit connected();

    return true;
}

void OPCUAClient::disconnect()
{
    QMutexLocker locker(&mutex_);

    reconnectTimer_->stop();
    keepAliveTimer_->stop();

    unsubscribeAll();
    doDisconnect();
    cleanupClient();

    setState(OPCUAClientState::Disconnected);
    LOG_INFO("OPC UA已断开连接");
    emit disconnected();
}

QString OPCUAClient::stateString() const
{
    switch (state_) {
    case OPCUAClientState::Disconnected: return "已断开";
    case OPCUAClientState::Connecting: return "连接中";
    case OPCUAClientState::Connected: return "已连接";
    case OPCUAClientState::Reconnecting: return "重连中";
    case OPCUAClientState::Error: return "错误";
    default: return "未知";
    }
}

QList<OPCUANodeInfo> OPCUAClient::browseNodes(const OPCUANodeId& parentId)
{
    QList<OPCUANodeInfo> nodes;

    if (state_ != OPCUAClientState::Connected) {
        setError("未连接到服务器");
        return nodes;
    }

    // 模拟实现 - 实际应调用OPC UA库
    // TODO: 实现实际的节点浏览

    LOG_DEBUG(QString("浏览节点: %1").arg(parentId.toString()));
    return nodes;
}

OPCUANodeInfo OPCUAClient::getNodeInfo(const OPCUANodeId& nodeId)
{
    OPCUANodeInfo info;
    info.nodeId = nodeId;

    if (state_ != OPCUAClientState::Connected) {
        setError("未连接到服务器");
        return info;
    }

    // TODO: 实现实际的节点信息获取
    return info;
}

OPCUANodeId OPCUAClient::findNode(const QString& browsePath)
{
    OPCUANodeId nodeId;

    if (state_ != OPCUAClientState::Connected) {
        setError("未连接到服务器");
        return nodeId;
    }

    // TODO: 实现路径解析
    LOG_DEBUG(QString("查找节点: %1").arg(browsePath));
    return nodeId;
}

OPCUADataValue OPCUAClient::readValue(const OPCUANodeId& nodeId)
{
    QMutexLocker locker(&mutex_);

    if (state_ != OPCUAClientState::Connected) {
        OPCUADataValue value;
        value.isGood = false;
        setError("未连接到服务器");
        return value;
    }

    return doReadValue(nodeId);
}

QMap<OPCUANodeId, OPCUADataValue> OPCUAClient::readValues(const QList<OPCUANodeId>& nodeIds)
{
    QMap<OPCUANodeId, OPCUADataValue> results;

    for (const auto& nodeId : nodeIds) {
        results[nodeId] = readValue(nodeId);
    }

    return results;
}

QVariant OPCUAClient::readAttribute(const OPCUANodeId& nodeId, int attributeId)
{
    Q_UNUSED(attributeId)

    auto value = readValue(nodeId);
    return value.value;
}

bool OPCUAClient::writeValue(const OPCUANodeId& nodeId, const QVariant& value)
{
    QMutexLocker locker(&mutex_);

    if (state_ != OPCUAClientState::Connected) {
        setError("未连接到服务器");
        return false;
    }

    return doWriteValue(nodeId, value);
}

bool OPCUAClient::writeValues(const QMap<OPCUANodeId, QVariant>& nodeValues)
{
    bool allSuccess = true;

    for (auto it = nodeValues.begin(); it != nodeValues.end(); ++it) {
        if (!writeValue(it.key(), it.value())) {
            allSuccess = false;
        }
    }

    return allSuccess;
}

uint32_t OPCUAClient::subscribe(const OPCUANodeId& nodeId, OPCUADataChangeCallback callback)
{
    QMutexLocker locker(&mutex_);

    if (state_ != OPCUAClientState::Connected) {
        setError("未连接到服务器");
        return 0;
    }

    uint32_t subId = nextSubscriptionId_++;

    SubscriptionInfo info;
    info.id = subId;
    info.nodeId = nodeId;
    info.callback = callback;

    subscriptions_[subId] = info;

    LOG_INFO(QString("已订阅节点: %1 (ID: %2)").arg(nodeId.toString()).arg(subId));
    return subId;
}

QList<uint32_t> OPCUAClient::subscribe(const QList<OPCUANodeId>& nodeIds, OPCUADataChangeCallback callback)
{
    QList<uint32_t> subIds;

    for (const auto& nodeId : nodeIds) {
        uint32_t id = subscribe(nodeId, callback);
        if (id > 0) {
            subIds.append(id);
        }
    }

    return subIds;
}

bool OPCUAClient::unsubscribe(uint32_t subscriptionId)
{
    QMutexLocker locker(&mutex_);

    auto it = subscriptions_.find(subscriptionId);
    if (it == subscriptions_.end()) {
        return false;
    }

    LOG_INFO(QString("已取消订阅: %1").arg(subscriptionId));
    subscriptions_.erase(it);
    return true;
}

void OPCUAClient::unsubscribeAll()
{
    QMutexLocker locker(&mutex_);
    subscriptions_.clear();
    LOG_INFO("已取消所有订阅");
}

QVariantList OPCUAClient::callMethod(const OPCUANodeId& objectId,
                                      const OPCUANodeId& methodId,
                                      const QVariantList& inputArgs)
{
    QVariantList outputArgs;

    if (state_ != OPCUAClientState::Connected) {
        setError("未连接到服务器");
        return outputArgs;
    }

    // TODO: 实现方法调用
    LOG_DEBUG(QString("调用方法: %1 on %2").arg(methodId.toString()).arg(objectId.toString()));
    Q_UNUSED(inputArgs)

    return outputArgs;
}

QStringList OPCUAClient::discoverServers(const QString& discoveryUrl)
{
    QStringList servers;

    // TODO: 实现服务器发现
    LOG_DEBUG(QString("发现服务器: %1").arg(discoveryUrl));

    return servers;
}

QList<QJsonObject> OPCUAClient::getEndpoints(const QString& serverUrl)
{
    QList<QJsonObject> endpoints;

    // TODO: 实现端点获取
    LOG_DEBUG(QString("获取端点: %1").arg(serverUrl));

    return endpoints;
}

void OPCUAClient::onReconnectTimer()
{
    reconnectAttempts_++;

    emit reconnecting(reconnectAttempts_, config_.maxReconnectAttempts);
    LOG_INFO(QString("OPC UA重连尝试 %1/%2")
            .arg(reconnectAttempts_)
            .arg(config_.maxReconnectAttempts));

    if (doConnect()) {
        reconnectTimer_->stop();
        setState(OPCUAClientState::Connected);
        keepAliveTimer_->start(10000);
        emit connected();
        LOG_INFO("OPC UA重连成功");
    } else if (reconnectAttempts_ >= config_.maxReconnectAttempts) {
        reconnectTimer_->stop();
        setState(OPCUAClientState::Error);
        setError("达到最大重连次数");
        LOG_ERROR("OPC UA重连失败，已达最大重试次数");
    }
}

void OPCUAClient::onKeepAliveTimer()
{
    // 处理订阅
    processSubscriptions();

    // 检查连接状态
    // TODO: 实现连接检查
}

void OPCUAClient::setState(OPCUAClientState newState)
{
    if (state_ != newState) {
        state_ = newState;
        emit stateChanged(newState);
    }
}

void OPCUAClient::setError(const QString& error)
{
    lastError_ = error;
    LOG_ERROR(QString("OPC UA错误: %1").arg(error));
    emit errorOccurred(error);
}

bool OPCUAClient::initializeClient()
{
    // TODO: 初始化OPC UA客户端库
    // 可使用open62541或Qt OPC UA
    return true;
}

void OPCUAClient::cleanupClient()
{
    // TODO: 清理OPC UA客户端资源
    clientHandle_ = nullptr;
}

void OPCUAClient::startReconnect()
{
    setState(OPCUAClientState::Reconnecting);
    reconnectTimer_->start(config_.reconnectIntervalMs);
}

void OPCUAClient::processSubscriptions()
{
    QMutexLocker locker(&mutex_);

    for (auto& [id, info] : subscriptions_) {
        auto newValue = doReadValue(info.nodeId);

        if (newValue.isGood && !(newValue == info.lastValue)) {
            info.lastValue = newValue;

            if (info.callback) {
                info.callback(info.nodeId, newValue);
            }

            emit dataChanged(info.nodeId, newValue);
        }
    }
}

bool OPCUAClient::doConnect()
{
    // TODO: 实际的OPC UA连接实现
    // 此处为模拟实现

    LOG_INFO(QString("正在连接到 OPC UA 服务器: %1").arg(config_.endpointUrl));

    // 模拟连接成功
    // 实际实现需要调用open62541或Qt OPC UA库

    return true;  // 模拟成功
}

void OPCUAClient::doDisconnect()
{
    // TODO: 实际的OPC UA断开实现
    LOG_INFO("断开OPC UA连接");
}

OPCUADataValue OPCUAClient::doReadValue(const OPCUANodeId& nodeId)
{
    OPCUADataValue value;

    // TODO: 实际的读取实现
    // 此处为模拟实现

    value.sourceTimestamp = QDateTime::currentDateTime();
    value.serverTimestamp = QDateTime::currentDateTime();
    value.statusCode = 0;  // Good
    value.isGood = true;

    // 模拟返回值
    value.value = QVariant(0);

    Q_UNUSED(nodeId)
    return value;
}

bool OPCUAClient::doWriteValue(const OPCUANodeId& nodeId, const QVariant& value)
{
    // TODO: 实际的写入实现
    LOG_DEBUG(QString("写入节点 %1 = %2").arg(nodeId.toString()).arg(value.toString()));
    return true;  // 模拟成功
}

// ============================================================
// UnifiedTag 实现
// ============================================================

QJsonObject UnifiedTag::toJson() const
{
    QJsonObject json;
    json["name"] = name;
    json["address"] = address;
    json["value"] = QJsonValue::fromVariant(value);
    json["timestamp"] = timestamp.toString(Qt::ISODate);
    json["isGood"] = isGood;
    json["protocol"] = protocol;
    json["metadata"] = metadata;
    return json;
}

UnifiedTag UnifiedTag::fromJson(const QJsonObject& json)
{
    UnifiedTag tag;
    tag.name = json.value("name").toString();
    tag.address = json.value("address").toString();
    tag.value = json.value("value").toVariant();
    tag.timestamp = QDateTime::fromString(json.value("timestamp").toString(), Qt::ISODate);
    tag.isGood = json.value("isGood").toBool(false);
    tag.protocol = json.value("protocol").toString();
    tag.metadata = json.value("metadata").toObject();
    return tag;
}

// ============================================================
// TagManager 实现
// ============================================================

TagManager::TagManager()
{
}

TagManager::~TagManager()
{
}

TagManager& TagManager::instance()
{
    static TagManager instance;
    return instance;
}

bool TagManager::registerTag(const UnifiedTag& tag)
{
    QMutexLocker locker(&mutex_);

    if (tag.name.isEmpty()) {
        return false;
    }

    tags_[tag.name] = tag;
    LOG_INFO(QString("注册Tag: %1 (%2)").arg(tag.name, tag.protocol));
    return true;
}

bool TagManager::unregisterTag(const QString& name)
{
    QMutexLocker locker(&mutex_);

    auto it = tags_.find(name);
    if (it == tags_.end()) {
        return false;
    }

    tags_.erase(it);
    callbacks_.erase(name);
    return true;
}

QVariant TagManager::readTag(const QString& name)
{
    QMutexLocker locker(&mutex_);

    auto it = tags_.find(name);
    if (it == tags_.end()) {
        return QVariant();
    }

    return it->second.value;
}

bool TagManager::writeTag(const QString& name, const QVariant& value)
{
    QMutexLocker locker(&mutex_);

    auto it = tags_.find(name);
    if (it == tags_.end()) {
        return false;
    }

    it->second.value = value;
    it->second.timestamp = QDateTime::currentDateTime();

    emit tagValueChanged(name, value);

    auto callbackIt = callbacks_.find(name);
    if (callbackIt != callbacks_.end() && callbackIt->second) {
        callbackIt->second(it->second);
    }

    return true;
}

QList<UnifiedTag> TagManager::allTags() const
{
    QMutexLocker locker(&mutex_);

    QList<UnifiedTag> list;
    for (const auto& [name, tag] : tags_) {
        list.append(tag);
    }
    return list;
}

void TagManager::subscribeTag(const QString& name, std::function<void(const UnifiedTag&)> callback)
{
    QMutexLocker locker(&mutex_);
    callbacks_[name] = callback;
}

} // namespace Comm
} // namespace VisionForge
