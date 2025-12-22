/**
 * @file OPCUAClient.h
 * @brief OPC UA客户端 - MES/SCADA系统对接
 * @details 实现OPC UA协议客户端，支持与MES/ERP系统交互
 *
 * 功能特性：
 * - 节点浏览：支持地址空间浏览
 * - 数据读写：读写单个或批量节点
 * - 订阅监控：支持数据变化订阅
 * - 安全认证：支持用户名密码和证书认证
 * - 自动重连：断线自动重连机制
 *
 * @note 基于open62541库或Qt OPC UA模块
 * @author VisionForge Team
 * @date 2025-12-22
 */

#pragma once

#include <QObject>
#include <QString>
#include <QVariant>
#include <QUrl>
#include <QTimer>
#include <QMutex>
#include <QJsonObject>
#include <QDateTime>
#include <memory>
#include <functional>
#include <map>

namespace VisionForge {
namespace Comm {

// ============================================================
// 类型定义
// ============================================================

/**
 * @brief OPC UA节点ID
 */
struct OPCUANodeId {
    int namespaceIndex = 0;     ///< 命名空间索引
    QString identifier;          ///< 标识符（字符串或数字）
    bool isNumeric = false;      ///< 是否为数字标识符
    uint32_t numericId = 0;      ///< 数字标识符

    QString toString() const;
    static OPCUANodeId fromString(const QString& str);

    bool operator==(const OPCUANodeId& other) const;
    bool operator<(const OPCUANodeId& other) const;
};

/**
 * @brief OPC UA数据值
 */
struct OPCUADataValue {
    QVariant value;              ///< 数据值
    QDateTime sourceTimestamp;   ///< 源时间戳
    QDateTime serverTimestamp;   ///< 服务器时间戳
    uint32_t statusCode = 0;     ///< 状态码
    bool isGood = false;         ///< 数据是否有效

    bool operator==(const OPCUADataValue& other) const {
        return value == other.value && statusCode == other.statusCode;
    }
};

/**
 * @brief OPC UA节点信息
 */
struct OPCUANodeInfo {
    OPCUANodeId nodeId;          ///< 节点ID
    QString displayName;          ///< 显示名称
    QString browseName;           ///< 浏览名称
    QString description;          ///< 描述
    int nodeClass = 0;            ///< 节点类
    int dataType = 0;             ///< 数据类型
    bool isReadable = true;       ///< 可读
    bool isWritable = false;      ///< 可写
    QList<OPCUANodeId> children;  ///< 子节点

    QJsonObject toJson() const;
};

/**
 * @brief OPC UA安全策略
 */
enum class OPCUASecurityPolicy {
    None,                       ///< 无安全
    Basic128Rsa15,              ///< Basic128Rsa15
    Basic256,                   ///< Basic256
    Basic256Sha256,             ///< Basic256Sha256
    Aes128Sha256RsaOaep,        ///< Aes128Sha256RsaOaep
    Aes256Sha256RsaPss          ///< Aes256Sha256RsaPss
};

/**
 * @brief OPC UA安全模式
 */
enum class OPCUASecurityMode {
    None,                       ///< 无安全
    Sign,                       ///< 签名
    SignAndEncrypt              ///< 签名和加密
};

/**
 * @brief OPC UA连接配置
 */
struct OPCUAClientConfig {
    QString endpointUrl;                 ///< 服务器端点URL
    QString applicationName = "VisionForge";  ///< 应用程序名称
    QString applicationUri;              ///< 应用程序URI
    QString productUri;                  ///< 产品URI

    // 安全设置
    OPCUASecurityPolicy securityPolicy = OPCUASecurityPolicy::None;
    OPCUASecurityMode securityMode = OPCUASecurityMode::None;
    QString certificatePath;             ///< 客户端证书路径
    QString privateKeyPath;              ///< 私钥路径
    QString trustListPath;               ///< 信任列表路径

    // 认证设置
    bool useAuthentication = false;      ///< 是否使用认证
    QString username;                    ///< 用户名
    QString password;                    ///< 密码

    // 连接设置
    int connectTimeoutMs = 5000;         ///< 连接超时（毫秒）
    int sessionTimeoutMs = 60000;        ///< 会话超时（毫秒）
    bool autoReconnect = true;           ///< 自动重连
    int reconnectIntervalMs = 5000;      ///< 重连间隔（毫秒）
    int maxReconnectAttempts = 10;       ///< 最大重连次数

    // 订阅设置
    int publishingIntervalMs = 1000;     ///< 发布间隔（毫秒）
    int samplingIntervalMs = 500;        ///< 采样间隔（毫秒）
    int queueSize = 10;                  ///< 队列大小

    QJsonObject toJson() const;
    static OPCUAClientConfig fromJson(const QJsonObject& json);
};

/**
 * @brief 订阅回调函数类型
 */
using OPCUADataChangeCallback = std::function<void(const OPCUANodeId&, const OPCUADataValue&)>;

/**
 * @brief OPC UA客户端状态
 */
enum class OPCUAClientState {
    Disconnected,           ///< 已断开
    Connecting,             ///< 连接中
    Connected,              ///< 已连接
    Reconnecting,           ///< 重连中
    Error                   ///< 错误
};

// ============================================================
// OPCUAClient 类
// ============================================================

/**
 * @class OPCUAClient
 * @brief OPC UA客户端类
 *
 * 使用方法：
 * @code
 * OPCUAClient client;
 * OPCUAClientConfig config;
 * config.endpointUrl = "opc.tcp://localhost:4840";
 * client.setConfig(config);
 *
 * if (client.connect()) {
 *     // 读取节点
 *     auto value = client.readValue(OPCUANodeId::fromString("ns=2;s=MyVariable"));
 *
 *     // 订阅数据变化
 *     client.subscribe(nodeId, [](const OPCUANodeId& id, const OPCUADataValue& value) {
 *         qDebug() << "Value changed:" << value.value;
 *     });
 * }
 * @endcode
 */
class OPCUAClient : public QObject
{
    Q_OBJECT

public:
    explicit OPCUAClient(QObject* parent = nullptr);
    ~OPCUAClient() override;

    // ========== 配置管理 ==========

    /**
     * @brief 设置配置
     */
    void setConfig(const OPCUAClientConfig& config);

    /**
     * @brief 获取配置
     */
    OPCUAClientConfig config() const { return config_; }

    /**
     * @brief 从文件加载配置
     */
    bool loadConfig(const QString& filePath);

    /**
     * @brief 保存配置到文件
     */
    bool saveConfig(const QString& filePath) const;

    // ========== 连接管理 ==========

    /**
     * @brief 连接到服务器
     * @return 是否成功
     */
    bool connect();

    /**
     * @brief 断开连接
     */
    void disconnect();

    /**
     * @brief 是否已连接
     */
    bool isConnected() const { return state_ == OPCUAClientState::Connected; }

    /**
     * @brief 获取当前状态
     */
    OPCUAClientState state() const { return state_; }

    /**
     * @brief 获取状态字符串
     */
    QString stateString() const;

    /**
     * @brief 获取最后错误信息
     */
    QString lastError() const { return lastError_; }

    // ========== 节点浏览 ==========

    /**
     * @brief 浏览节点
     * @param parentId 父节点ID（空则从根节点开始）
     * @return 子节点列表
     */
    QList<OPCUANodeInfo> browseNodes(const OPCUANodeId& parentId = OPCUANodeId());

    /**
     * @brief 获取节点信息
     */
    OPCUANodeInfo getNodeInfo(const OPCUANodeId& nodeId);

    /**
     * @brief 查找节点（按路径）
     * @param browsePath 浏览路径（如 "Objects/MyFolder/MyVariable"）
     */
    OPCUANodeId findNode(const QString& browsePath);

    // ========== 数据读取 ==========

    /**
     * @brief 读取单个节点值
     */
    OPCUADataValue readValue(const OPCUANodeId& nodeId);

    /**
     * @brief 批量读取节点值
     */
    QMap<OPCUANodeId, OPCUADataValue> readValues(const QList<OPCUANodeId>& nodeIds);

    /**
     * @brief 读取节点属性
     */
    QVariant readAttribute(const OPCUANodeId& nodeId, int attributeId);

    // ========== 数据写入 ==========

    /**
     * @brief 写入单个节点值
     */
    bool writeValue(const OPCUANodeId& nodeId, const QVariant& value);

    /**
     * @brief 批量写入节点值
     */
    bool writeValues(const QMap<OPCUANodeId, QVariant>& nodeValues);

    // ========== 订阅管理 ==========

    /**
     * @brief 订阅数据变化
     * @param nodeId 节点ID
     * @param callback 数据变化回调
     * @return 订阅ID（用于取消订阅）
     */
    uint32_t subscribe(const OPCUANodeId& nodeId, OPCUADataChangeCallback callback);

    /**
     * @brief 批量订阅
     */
    QList<uint32_t> subscribe(const QList<OPCUANodeId>& nodeIds, OPCUADataChangeCallback callback);

    /**
     * @brief 取消订阅
     */
    bool unsubscribe(uint32_t subscriptionId);

    /**
     * @brief 取消所有订阅
     */
    void unsubscribeAll();

    // ========== 方法调用 ==========

    /**
     * @brief 调用方法
     * @param objectId 对象节点ID
     * @param methodId 方法节点ID
     * @param inputArgs 输入参数
     * @return 输出参数
     */
    QVariantList callMethod(const OPCUANodeId& objectId,
                           const OPCUANodeId& methodId,
                           const QVariantList& inputArgs = QVariantList());

    // ========== 服务器发现 ==========

    /**
     * @brief 获取服务器端点列表
     */
    static QStringList discoverServers(const QString& discoveryUrl);

    /**
     * @brief 获取端点信息
     */
    static QList<QJsonObject> getEndpoints(const QString& serverUrl);

signals:
    /**
     * @brief 连接状态变化
     */
    void stateChanged(OPCUAClientState newState);

    /**
     * @brief 已连接
     */
    void connected();

    /**
     * @brief 已断开
     */
    void disconnected();

    /**
     * @brief 连接错误
     */
    void errorOccurred(const QString& error);

    /**
     * @brief 数据变化
     */
    void dataChanged(const OPCUANodeId& nodeId, const OPCUADataValue& value);

    /**
     * @brief 重连尝试
     */
    void reconnecting(int attempt, int maxAttempts);

private slots:
    void onReconnectTimer();
    void onKeepAliveTimer();

private:
    void setState(OPCUAClientState newState);
    void setError(const QString& error);
    bool initializeClient();
    void cleanupClient();
    void startReconnect();
    void processSubscriptions();

    // 内部OPC UA操作（实际实现依赖具体库）
    bool doConnect();
    void doDisconnect();
    OPCUADataValue doReadValue(const OPCUANodeId& nodeId);
    bool doWriteValue(const OPCUANodeId& nodeId, const QVariant& value);

private:
    OPCUAClientConfig config_;
    OPCUAClientState state_ = OPCUAClientState::Disconnected;
    QString lastError_;

    QTimer* reconnectTimer_ = nullptr;
    QTimer* keepAliveTimer_ = nullptr;
    int reconnectAttempts_ = 0;

    mutable QMutex mutex_;

    // 订阅管理
    struct SubscriptionInfo {
        uint32_t id;
        OPCUANodeId nodeId;
        OPCUADataChangeCallback callback;
        OPCUADataValue lastValue;
    };
    std::map<uint32_t, SubscriptionInfo> subscriptions_;
    uint32_t nextSubscriptionId_ = 1;

    // OPC UA客户端句柄（具体实现依赖库）
    void* clientHandle_ = nullptr;
};

// ============================================================
// 统一Tag接口
// ============================================================

/**
 * @brief 统一Tag结构（屏蔽底层协议差异）
 */
struct UnifiedTag {
    QString name;               ///< Tag名称
    QString address;            ///< 地址（协议相关）
    QVariant value;             ///< 当前值
    QDateTime timestamp;        ///< 时间戳
    bool isGood = false;        ///< 质量标志
    QString protocol;           ///< 协议类型（OPCUA/Modbus/...）
    QJsonObject metadata;       ///< 元数据

    QJsonObject toJson() const;
    static UnifiedTag fromJson(const QJsonObject& json);
};

/**
 * @class TagManager
 * @brief 统一Tag管理器
 *
 * 提供协议无关的Tag访问接口，屏蔽底层通信差异
 */
class TagManager : public QObject
{
    Q_OBJECT

public:
    static TagManager& instance();

    /**
     * @brief 注册Tag
     */
    bool registerTag(const UnifiedTag& tag);

    /**
     * @brief 取消注册Tag
     */
    bool unregisterTag(const QString& name);

    /**
     * @brief 读取Tag值
     */
    QVariant readTag(const QString& name);

    /**
     * @brief 写入Tag值
     */
    bool writeTag(const QString& name, const QVariant& value);

    /**
     * @brief 获取所有Tag
     */
    QList<UnifiedTag> allTags() const;

    /**
     * @brief 订阅Tag变化
     */
    void subscribeTag(const QString& name, std::function<void(const UnifiedTag&)> callback);

signals:
    void tagValueChanged(const QString& name, const QVariant& value);

private:
    TagManager();
    ~TagManager();

    std::map<QString, UnifiedTag> tags_;
    std::map<QString, std::function<void(const UnifiedTag&)>> callbacks_;
    mutable QMutex mutex_;
};

} // namespace Comm
} // namespace VisionForge

// 注册类型以便在信号槽中使用
Q_DECLARE_METATYPE(VisionForge::Comm::OPCUANodeId)
Q_DECLARE_METATYPE(VisionForge::Comm::OPCUADataValue)
Q_DECLARE_METATYPE(VisionForge::Comm::OPCUAClientState)
