## 安全功能使用指南

**版本**: v1.2.0
**日期**: 2025-12-20
**作者**: VisionForge Team

---

## 目录

1. [概述](#1-概述)
2. [快速开始](#2-快速开始)
3. [组件详解](#3-组件详解)
4. [完整集成示例](#4-完整集成示例)
5. [最佳实践](#5-最佳实践)
6. [故障排除](#6-故障排除)

---

## 1. 概述

VisionForge Pro v1.2.0 引入了完整的安全功能，包括：

- **WSS (WebSocket Secure)**: 加密的WebSocket通信
- **JWT Token认证**: 无状态的Token认证机制
- **RBAC权限控制**: 基于角色的访问控制
- **审计日志**: 完整的安全事件记录

### 1.1 核心组件

| 组件                        | 功能                          | 头文件                             |
|----------------------------|-------------------------------|-----------------------------------|
| SecurityManager            | 安全统一管理器                 | remote/SecurityManager.h           |
| SecurityCertificateManager | SSL/TLS证书管理               | remote/SecurityCertificateManager.h |
| TokenAuthManager          | JWT Token认证                 | remote/TokenAuthManager.h          |
| RBACManager               | 角色权限管理                   | remote/RBACManager.h               |
| WebSocketServer           | WSS支持的WebSocket服务器       | remote/WebSocketServer.h           |

---

## 2. 快速开始

### 2.1 最简配置（自签名证书 + 默认设置）

```cpp
#include "remote/SecurityManager.h"
#include "remote/WebSocketServer.h"

// 1. 创建安全管理器
auto* security = new SecurityManager(this);

// 2. 配置（使用自签名证书）
SecurityConfig secConfig;
secConfig.enableSecurity = true;
secConfig.certificateConfig.useSelfSigned = true;
secConfig.certificateConfig.commonName = "localhost";

// 3. 初始化
if (!security->initialize(secConfig)) {
    qCritical() << "安全管理器初始化失败";
    return;
}

// 4. 创建WSS服务器
auto* server = new WebSocketServer(this);

// 5. 配置SSL
server->setSslConfiguration(
    security->certificateManager()->getSslConfiguration()
);

// 6. 启动服务器
ServerConfig config;
config.port = 8765;
config.enableSsl = true;
server->start(config);

qInfo() << "安全的远程诊断服务器已启动在 wss://localhost:8765";
```

### 2.2 生产环境配置（正式证书）

```cpp
// 1. 配置安全管理器
SecurityConfig secConfig;
secConfig.enableSecurity = true;

// SSL证书配置
secConfig.certificateConfig.enabled = true;
secConfig.certificateConfig.useSelfSigned = false;
secConfig.certificateConfig.certificatePath = "/etc/visionforge/certs/server.crt";
secConfig.certificateConfig.privateKeyPath = "/etc/visionforge/certs/server.key";
secConfig.certificateConfig.caBundlePath = "/etc/visionforge/certs/ca-bundle.crt";

// Token配置
secConfig.tokenConfig.secretKey = "your-production-secret-key-32-bytes";
secConfig.tokenConfig.accessTokenLifetime = 3600;      // 1小时
secConfig.tokenConfig.refreshTokenLifetime = 86400;    // 24小时

// RBAC配置
secConfig.rbacConfig.databasePath = "/var/lib/visionforge/security.db";
secConfig.rbacConfig.defaultRole = UserRole::Viewer;
secConfig.rbacConfig.maxLoginAttempts = 5;
secConfig.rbacConfig.lockoutDurationMinutes = 15;

// 2. 初始化
auto* security = new SecurityManager(this);
if (!security->initialize(secConfig)) {
    qCritical() << "安全管理器初始化失败";
    return;
}
```

---

## 3. 组件详解

### 3.1 SecurityManager - 安全统一管理器

SecurityManager是安全功能的入口点，整合了所有安全组件。

#### 3.1.1 初始化

```cpp
#include "remote/SecurityManager.h"

auto* security = new SecurityManager(this);

SecurityConfig config;
config.enableSecurity = true;
config.enableAuditLog = true;

// 证书配置
config.certificateConfig.useSelfSigned = true;
config.certificateConfig.commonName = "visionforge.local";

// Token配置
config.tokenConfig.secretKey = "change-me-in-production";
config.tokenConfig.accessTokenLifetime = 3600;

// RBAC配置
config.rbacConfig.defaultRole = UserRole::Viewer;

security->initialize(config);
```

#### 3.1.2 用户认证

```cpp
// 登录认证
ConnectionAuthResult result = security->authenticateConnection(
    "admin",           // 用户名
    "password123",     // 密码
    "192.168.1.100"    // IP地址（可选）
);

if (result.authenticated) {
    qInfo() << "认证成功";
    qInfo() << "  用户ID:" << result.userId;
    qInfo() << "  用户名:" << result.username;
    qInfo() << "  角色:" << RBACManager::roleToString(result.role);
    qInfo() << "  访问Token:" << result.accessToken;
    qInfo() << "  刷新Token:" << result.refreshToken;
    qInfo() << "  Token过期时间:" << result.tokenExpiry;
} else {
    qWarning() << "认证失败:" << result.errorMessage;
}
```

#### 3.1.3 Token验证

```cpp
// 验证访问Token
TokenValidationResult tokenResult = security->validateToken(accessToken);

if (tokenResult.isValid) {
    qInfo() << "Token有效";
    qInfo() << "  用户ID:" << tokenResult.userId;
    qInfo() << "  剩余时间:" << tokenResult.remainingSeconds << "秒";
} else {
    qWarning() << "Token无效:" << tokenResult.errorMessage;
}
```

#### 3.1.4 权限检查

```cpp
// 检查权限
bool canExport = security->authorizeAction(token, Permission::ExportLogs);
if (canExport) {
    // 执行导出操作
    exportLogs();
} else {
    qWarning() << "权限不足";
}

// 批量检查权限
QList<Permission> permissions = {
    Permission::ViewLogs,
    Permission::ExportLogs,
    Permission::ClearLogs
};

auto results = security->checkPermissions(userId, permissions);
for (auto it = results.constBegin(); it != results.constEnd(); ++it) {
    qInfo() << RBACManager::permissionToString(it.key()) << ":" << it.value();
}
```

#### 3.1.5 用户管理

```cpp
// 添加用户
bool success = security->addUser(
    "operator1",                // 用户名
    "password123",              // 密码
    UserRole::Operator,         // 角色
    "operator@example.com",     // 邮箱
    "操作员账户"                // 描述
);

// 更新密码
security->updateUserPassword("user_id", "new_password");

// 更新角色
security->updateUserRole("user_id", UserRole::Admin);
```

#### 3.1.6 登出

```cpp
security->logout(
    result.userId,              // 用户ID
    result.accessToken,         // 访问Token（会被撤销）
    "192.168.1.100"            // IP地址
);
```

---

### 3.2 SecurityCertificateManager - SSL/TLS证书管理

#### 3.2.1 生成自签名证书

```cpp
#include "remote/SecurityCertificateManager.h"

auto* certManager = new SecurityCertificateManager(this);

// 方法1: 使用initialize自动生成
CertificateConfig config;
config.useSelfSigned = true;
config.commonName = "localhost";
config.validDays = 365;

certManager->initialize(config);

// 方法2: 手动生成
if (certManager->generateSelfSignedCertificate("visionforge.local", 365)) {
    qInfo() << "自签名证书生成成功";
}
```

#### 3.2.2 加载正式证书

```cpp
CertificateConfig config;
config.enabled = true;
config.useSelfSigned = false;
config.certificatePath = "./certs/server.crt";
config.privateKeyPath = "./certs/server.key";
config.caBundlePath = "./certs/ca-bundle.crt";

if (certManager->initialize(config)) {
    qInfo() << "证书加载成功";
}
```

#### 3.2.3 证书信息

```cpp
CertificateInfo info = certManager->getCertificateInfo();

qInfo() << "证书信息:";
qInfo() << "  通用名称:" << info.commonName;
qInfo() << "  组织:" << info.organization;
qInfo() << "  颁发者:" << info.issuer;
qInfo() << "  生效时间:" << info.notBefore;
qInfo() << "  过期时间:" << info.notAfter;
qInfo() << "  剩余天数:" << info.daysUntilExpiry;
qInfo() << "  是否有效:" << info.isValid;
qInfo() << "  是否自签名:" << info.isSelfSigned;
```

#### 3.2.4 证书过期监控

```cpp
// 连接过期警告信号
connect(certManager, &SecurityCertificateManager::certificateExpiringSoon,
        this, [](int daysRemaining) {
    qWarning() << "证书即将过期，剩余" << daysRemaining << "天";
    // 发送邮件通知管理员
});

connect(certManager, &SecurityCertificateManager::certificateExpired,
        this, []() {
    qCritical() << "证书已过期，请立即更新";
});
```

---

### 3.3 TokenAuthManager - JWT Token认证

#### 3.3.1 生成Token

```cpp
#include "remote/TokenAuthManager.h"

auto* tokenManager = new TokenAuthManager(this);

// 初始化
TokenConfig config;
config.secretKey = "your-secret-key-32-bytes-long";
config.accessTokenLifetime = 3600;       // 1小时
config.refreshTokenLifetime = 86400;     // 24小时
tokenManager->initialize(config);

// 生成访问Token
QString accessToken = tokenManager->generateAccessToken(
    "user_123",              // 用户ID
    "admin",                 // 用户名
    "Admin",                 // 角色
    "192.168.1.100"         // IP地址
);

// 生成Token对（访问+刷新）
auto tokenPair = tokenManager->generateTokenPair(
    "user_123",
    "admin",
    "Admin",
    "192.168.1.100"
);

qInfo() << "访问Token:" << tokenPair.accessToken;
qInfo() << "刷新Token:" << tokenPair.refreshToken;
qInfo() << "访问Token过期时间:" << tokenPair.accessTokenExpiry;
qInfo() << "刷新Token过期时间:" << tokenPair.refreshTokenExpiry;
```

#### 3.3.2 验证Token

```cpp
// 完整验证（包括黑名单检查）
TokenValidationResult result = tokenManager->validateToken(token);

if (result.isValid) {
    qInfo() << "Token有效";
    qInfo() << "  用户ID:" << result.userId;
    qInfo() << "  用户名:" << result.username;
    qInfo() << "  角色:" << result.role;
    qInfo() << "  剩余时间:" << result.remainingSeconds << "秒";
} else {
    qWarning() << "Token无效:" << result.errorMessage;
}

// 快速验证（跳过黑名单检查，性能更好）
TokenValidationResult quickResult = tokenManager->quickValidateToken(token);
```

#### 3.3.3 刷新Token

```cpp
// 使用刷新Token获取新的访问Token
QString newAccessToken = tokenManager->refreshAccessToken(refreshToken);

if (!newAccessToken.isEmpty()) {
    qInfo() << "Token刷新成功:" << newAccessToken;
} else {
    qWarning() << "Token刷新失败";
}

// 刷新整个Token对
auto newTokenPair = tokenManager->refreshTokenPair(refreshToken);
```

#### 3.3.4 撤销Token

```cpp
// 撤销单个Token
tokenManager->revokeToken(token);

// 撤销用户的所有Token
tokenManager->revokeUserTokens("user_123");

// 检查Token是否已撤销
if (tokenManager->isTokenRevoked(token)) {
    qWarning() << "Token已被撤销";
}
```

#### 3.3.5 Token管理

```cpp
// 获取用户的所有活跃Token
QList<TokenInfo> userTokens = tokenManager->getUserTokens("user_123");

for (const auto& tokenInfo : userTokens) {
    qInfo() << "Token ID:" << tokenInfo.tokenId;
    qInfo() << "  签发时间:" << tokenInfo.issuedAt;
    qInfo() << "  过期时间:" << tokenInfo.expiryTime;
    qInfo() << "  IP地址:" << tokenInfo.ipAddress;
    qInfo() << "  是否刷新Token:" << tokenInfo.isRefreshToken;
}

// 获取统计信息
auto stats = tokenManager->getStatistics();
qInfo() << "活跃Token总数:" << stats.totalActiveTokens;
qInfo() << "黑名单Token数:" << stats.blacklistedTokens;
```

---

### 3.4 RBACManager - 角色权限管理

#### 3.4.1 用户管理

```cpp
#include "remote/RBACManager.h"

auto* rbac = new RBACManager(this);

// 初始化
RBACConfig config;
config.databasePath = "./data/security.db";
config.defaultRole = UserRole::Viewer;
config.maxLoginAttempts = 5;
config.lockoutDurationMinutes = 15;
rbac->initialize(config);

// 添加用户
bool success = rbac->addUser(
    "operator1",                // 用户名
    "password123",              // 密码
    UserRole::Operator,         // 角色
    "op@example.com",           // 邮箱
    "操作员账户"                // 描述
);

// 获取用户信息
UserInfo user = rbac->getUserInfo("user_id");
qInfo() << "用户:" << user.username;
qInfo() << "角色:" << RBACManager::roleToString(user.role);
qInfo() << "创建时间:" << user.createdAt;
qInfo() << "最后登录:" << user.lastLoginAt;

// 获取所有用户
QList<UserInfo> allUsers = rbac->getAllUsers();
```

#### 3.4.2 角色权限

```cpp
// 预定义角色
enum class UserRole {
    Guest = 0,      // 访客：受限只读
    Viewer = 1,     // 查看者：只读
    Operator = 2,   // 操作员：读+写（不包括配置）
    Admin = 3       // 管理员：所有权限
};

// 获取角色权限
QSet<Permission> permissions = RBACManager::getRolePermissions(UserRole::Operator);

qInfo() << "操作员权限:";
for (Permission perm : permissions) {
    qInfo() << "  -" << RBACManager::permissionToString(perm);
}

// 用户权限
QSet<Permission> userPerms = rbac->getUserPermissions("user_id");
```

#### 3.4.3 权限检查

```cpp
// 单个权限检查
if (rbac->hasPermission("user_id", Permission::ExportLogs)) {
    // 允许导出日志
}

// 带审计日志的权限检查
bool allowed = rbac->checkPermission(
    "user_id",
    Permission::ModifyConfiguration,
    "system.config"  // 资源名称
);

// 批量权限检查
QList<Permission> permsToCheck = {
    Permission::ViewLogs,
    Permission::ExportLogs,
    Permission::ClearLogs
};

auto results = rbac->checkPermissions("user_id", permsToCheck);
```

#### 3.4.4 认证

```cpp
AuthResult result = rbac->authenticate(
    "admin",                    // 用户名
    "password123",              // 密码
    "192.168.1.100"            // IP地址
);

if (result.success) {
    qInfo() << "认证成功";
    qInfo() << "  用户ID:" << result.userId;
    qInfo() << "  用户名:" << result.username;
    qInfo() << "  角色:" << RBACManager::roleToString(result.role);
} else {
    qWarning() << "认证失败:" << result.errorMessage;
    qWarning() << "  剩余尝试次数:" << result.remainingAttempts;
}
```

#### 3.4.5 审计日志

```cpp
// 记录安全事件
rbac->logAuditEvent(
    "user_123",                 // 用户ID
    "EXPORT_DATA",              // 操作
    "performance_metrics",      // 资源
    true,                       // 是否成功
    "192.168.1.100",           // IP地址
    "导出100条记录"             // 详情
);

// 查询审计日志
QDateTime start = QDateTime::currentDateTime().addDays(-7);
QDateTime end = QDateTime::currentDateTime();

QList<AuditLogEntry> logs = rbac->queryAuditLog(start, end, "user_123", 100);

for (const auto& log : logs) {
    qInfo() << log.timestamp << log.action << log.result;
}

// 获取最近的审计日志
QList<AuditLogEntry> recentLogs = rbac->getRecentAuditLog(50);
```

---

### 3.5 WebSocketServer - WSS支持

#### 3.5.1 配置WSS

```cpp
#include "remote/WebSocketServer.h"
#include "remote/SecurityCertificateManager.h"

// 1. 创建证书管理器
auto* certManager = new SecurityCertificateManager(this);
certManager->initialize(certificateConfig);

// 2. 创建WebSocket服务器
auto* server = new WebSocketServer(this);

// 3. 配置SSL
server->setSslConfiguration(certManager->getSslConfiguration());

// 4. 启动服务器（启用SSL）
ServerConfig config;
config.port = 8765;
config.enableSsl = true;
config.requireAuth = true;

if (server->start(config)) {
    qInfo() << "WSS服务器启动成功在端口" << config.port;
}
```

#### 3.5.2 处理SSL错误

```cpp
// WebSocketServer会自动处理SSL错误并记录日志
// SSL错误会通过日志输出，格式如下：
// WSS SSL错误:
//   - <错误详情>

// 可以连接serverError信号获取错误通知
connect(server, &WebSocketServer::serverError, this, [](const QString& error) {
    qCritical() << "WebSocket服务器错误:" << error;
    // 处理错误（如：发送通知、重启服务器等）
});
```

---

## 4. 完整集成示例

### 4.1 安全的远程诊断服务

```cpp
#include "remote/SecurityManager.h"
#include "remote/WebSocketServer.h"
#include "remote/PerformanceMonitor.h"
#include "remote/RemoteLogCollector.h"
#include "remote/RemoteDatabaseManager.h"

class SecureRemoteDiagnosticsService : public QObject
{
    Q_OBJECT

public:
    explicit SecureRemoteDiagnosticsService(QObject* parent = nullptr)
        : QObject(parent)
    {
        // 创建组件
        security_ = new SecurityManager(this);
        server_ = new WebSocketServer(this);
        perfMonitor_ = new PerformanceMonitor(this);
        logCollector_ = new RemoteLogCollector(this);
        database_ = new RemoteDatabaseManager(this);

        // 连接信号
        setupConnections();
    }

    bool start(quint16 port = 8765)
    {
        qInfo() << "=== 启动安全的远程诊断服务 ===";

        // 1. 初始化安全管理器
        SecurityConfig secConfig;
        secConfig.enableSecurity = true;
        secConfig.enableAuditLog = true;
        secConfig.certificateConfig.useSelfSigned = true;
        secConfig.certificateConfig.commonName = "visionforge.local";
        secConfig.tokenConfig.secretKey = "production-secret-key-change-me";

        if (!security_->initialize(secConfig)) {
            qCritical() << "安全管理器初始化失败";
            return false;
        }

        // 2. 初始化数据库
        if (!database_->initialize()) {
            qCritical() << "数据库初始化失败";
            return false;
        }

        // 3. 初始化日志收集器
        if (!logCollector_->initialize()) {
            qCritical() << "日志收集器初始化失败";
            return false;
        }

        // 4. 配置并启动WebSocket服务器（WSS）
        server_->setSslConfiguration(
            security_->certificateManager()->getSslConfiguration()
        );

        ServerConfig serverConfig;
        serverConfig.port = port;
        serverConfig.enableSsl = true;
        serverConfig.requireAuth = true;

        if (!server_->start(serverConfig)) {
            qCritical() << "WebSocket服务器启动失败";
            return false;
        }

        // 5. 启动性能监控
        if (!perfMonitor_->start(1000)) {
            qCritical() << "性能监控启动失败";
            return false;
        }

        qInfo() << "=== 安全的远程诊断服务已启动 ===";
        qInfo() << "WSS地址: wss://localhost:" << port;
        return true;
    }

    void stop()
    {
        perfMonitor_->stop();
        server_->stop();
        security_->close();
        database_->close();

        qInfo() << "安全的远程诊断服务已停止";
    }

private:
    void setupConnections()
    {
        // 客户端连接处理
        connect(server_, &WebSocketServer::clientConnected,
                this, &SecureRemoteDiagnosticsService::onClientConnected);

        connect(server_, &WebSocketServer::commandReceived,
                this, &SecureRemoteDiagnosticsService::onCommandReceived);

        // 性能监控数据推送
        connect(perfMonitor_, &PerformanceMonitor::metricsUpdated,
                this, [this](const PerformanceMetrics& metrics) {
            // 保存到数据库
            database_->insertPerformanceMetrics(metrics);
            // 推送给订阅的客户端
            server_->pushPerformanceMetrics(metrics);
        });

        // 日志数据推送
        connect(logCollector_, &RemoteLogCollector::logEntryReceived,
                this, [this](const LogEntryData& log) {
            // 保存到数据库
            database_->insertLogEntry(log);
            // 推送给订阅的客户端
            server_->pushLogEntry(log);
        });
    }

    void onClientConnected(const QString& sessionId, const QString& clientId)
    {
        qInfo() << "客户端连接:" << clientId << "会话:" << sessionId;
        // 这里可以进行额外的认证检查
    }

    void onCommandReceived(const QString& sessionId,
                          const QString& command,
                          const QJsonObject& params)
    {
        qInfo() << "收到命令:" << command << "来自会话:" << sessionId;

        // 获取Token
        QString token = params["token"].toString();
        if (token.isEmpty()) {
            sendError(sessionId, "缺少认证Token");
            return;
        }

        // 验证Token
        auto validationResult = security_->validateToken(token);
        if (!validationResult.isValid) {
            sendError(sessionId, "Token无效: " + validationResult.errorMessage);
            security_->logSecurityEvent("", "UNAUTHORIZED_ACCESS", command, false);
            return;
        }

        // 处理命令
        handleCommand(sessionId, validationResult.userId, command, params);
    }

    void handleCommand(const QString& sessionId,
                      const QString& userId,
                      const QString& command,
                      const QJsonObject& params)
    {
        if (command == "query_performance") {
            // 检查权限
            if (!security_->authorizeUser(userId, Permission::ViewPerformanceMetrics)) {
                sendError(sessionId, "权限不足");
                return;
            }

            // 查询性能数据
            int limit = params["limit"].toInt(100);
            auto metrics = database_->getLatestPerformanceMetrics(limit);

            // 发送响应
            QJsonObject response;
            response["command"] = "query_performance";
            response["status"] = "success";
            response["data"] = serializeMetrics(metrics);
            server_->sendTo(sessionId, response);

            // 记录审计日志
            security_->logSecurityEvent(userId, "QUERY_PERFORMANCE",
                                       QString::number(metrics.size()), true);
        }
        else if (command == "export_logs") {
            // 检查权限
            if (!security_->authorizeUser(userId, Permission::ExportLogs)) {
                sendError(sessionId, "权限不足");
                return;
            }

            // 导出日志
            QString filePath = params["filePath"].toString();
            LogQuery query;
            query.startTime = QDateTime::fromString(params["start"].toString(), Qt::ISODate);
            query.endTime = QDateTime::fromString(params["end"].toString(), Qt::ISODate);

            bool success = database_->exportLogsToCSV(filePath, query);

            QJsonObject response;
            response["command"] = "export_logs";
            response["status"] = success ? "success" : "failed";
            server_->sendTo(sessionId, response);

            // 记录审计日志
            security_->logSecurityEvent(userId, "EXPORT_LOGS", filePath, success);
        }
    }

    void sendError(const QString& sessionId, const QString& error)
    {
        QJsonObject response;
        response["status"] = "error";
        response["error"] = error;
        server_->sendTo(sessionId, response);
    }

    QJsonArray serializeMetrics(const QList<PerformanceMetrics>& metrics)
    {
        QJsonArray array;
        for (const auto& m : metrics) {
            QJsonObject obj;
            obj["timestamp"] = m.timestamp.toString(Qt::ISODate);
            obj["cpu_usage"] = m.cpuUsage;
            obj["memory_used_mb"] = m.memoryUsedMB;
            obj["fps"] = m.fps;
            array.append(obj);
        }
        return array;
    }

private:
    SecurityManager* security_;
    WebSocketServer* server_;
    PerformanceMonitor* perfMonitor_;
    RemoteLogCollector* logCollector_;
    RemoteDatabaseManager* database_;
};
```

### 4.2 使用服务

```cpp
// main.cpp
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    SecureRemoteDiagnosticsService service;
    if (!service.start(8765)) {
        return -1;
    }

    return app.exec();
}
```

---

## 5. 最佳实践

### 5.1 密钥管理

```cpp
// ❌ 错误：硬编码密钥
config.tokenConfig.secretKey = "my-secret-key";

// ✅ 正确：从环境变量或配置文件读取
QString secretKey = qgetenv("VISIONFORGE_SECRET_KEY");
if (secretKey.isEmpty()) {
    // 从配置文件读取
    QSettings settings("config.ini", QSettings::IniFormat);
    secretKey = settings.value("security/secretKey").toString();
}
config.tokenConfig.secretKey = secretKey;

// ✅ 更好：使用密钥管理服务（如AWS KMS、Azure Key Vault）
```

### 5.2 证书管理

```cpp
// 开发环境：使用自签名证书
if (isDevEnvironment()) {
    config.certificateConfig.useSelfSigned = true;
    config.certificateConfig.commonName = "localhost";
}

// 生产环境：使用正式证书
else {
    config.certificateConfig.useSelfSigned = false;
    config.certificateConfig.certificatePath = "/etc/certs/server.crt";
    config.certificateConfig.privateKeyPath = "/etc/certs/server.key";

    // 监控证书过期
    connect(certManager, &SecurityCertificateManager::certificateExpiringSoon,
            this, [](int days) {
        // 发送邮件通知
        sendEmailNotification("证书即将过期", QString("剩余%1天").arg(days));
    });
}
```

### 5.3 Token生命周期

```cpp
// ✅ 推荐配置
config.tokenConfig.accessTokenLifetime = 900;       // 15分钟（短）
config.tokenConfig.refreshTokenLifetime = 604800;   // 7天（长）
config.tokenConfig.enableAutoRefresh = true;
config.tokenConfig.refreshThreshold = 300;          // 剩余5分钟时自动提示刷新
```

### 5.4 权限最小化原则

```cpp
// ❌ 错误：给所有用户Admin权限
addUser("user1", "password", UserRole::Admin);

// ✅ 正确：根据实际需求分配权限
addUser("viewer1", "password", UserRole::Viewer);      // 只读
addUser("operator1", "password", UserRole::Operator);  // 读+写
addUser("admin", "password", UserRole::Admin);         // 仅必要的管理员
```

### 5.5 审计日志

```cpp
// 记录所有关键操作
void performSensitiveOperation(const QString& userId)
{
    bool success = false;
    QString details;

    try {
        // 执行操作
        success = doSomething();
        details = "操作成功";
    } catch (const std::exception& e) {
        success = false;
        details = QString("操作失败: %1").arg(e.what());
    }

    // 记录审计日志
    security->logSecurityEvent(
        userId,
        "SENSITIVE_OPERATION",
        "resource_name",
        success,
        getClientIpAddress(),
        details
    );
}
```

---

## 6. 故障排除

### 6.1 SSL相关问题

#### 问题：系统不支持SSL

```
错误: 系统不支持SSL (OpenSSL版本: , 构建版本: )
```

**解决方案**:
1. 确保系统已安装OpenSSL
2. Windows: 下载并安装OpenSSL DLL (https://slproweb.com/products/Win32OpenSSL.html)
3. Linux: `sudo apt-get install libssl-dev` (Debian/Ubuntu) 或 `sudo yum install openssl-devel` (CentOS/RHEL)

#### 问题：证书加载失败

```
错误: 无法打开证书文件: /path/to/cert.pem
```

**解决方案**:
1. 检查文件路径是否正确
2. 检查文件权限
3. 确保证书格式为PEM

### 6.2 Token相关问题

#### 问题：Token验证失败

```
错误: Token格式错误或签名无效
```

**解决方案**:
1. 确保secretKey在所有实例中一致
2. 检查Token是否完整传输（未被截断）
3. 验证Token格式是否正确（Header.Payload.Signature）

#### 问题：Token频繁过期

```
错误: Token已过期
```

**解决方案**:
1. 增加`accessTokenLifetime`
2. 启用自动刷新机制：`enableAutoRefresh = true`
3. 客户端实现Token刷新逻辑

### 6.3 RBAC相关问题

#### 问题：用户被锁定

```
错误: 账户已锁定，请15分钟后重试
```

**解决方案**:
1. 等待锁定时间过期
2. 管理员手动解锁：
```cpp
rbac->setUserActive(userId, false);  // 停用
rbac->setUserActive(userId, true);   // 重新激活（会重置锁定状态）
```

#### 问题：权限检查失败

```
错误: 权限不足
```

**解决方案**:
1. 检查用户角色：`rbac->getUserRole(userId)`
2. 检查角色权限：`RBACManager::getRolePermissions(role)`
3. 必要时提升用户角色

### 6.4 数据库相关问题

#### 问题：数据库打开失败

```
错误: 无法打开RBAC数据库: unable to open database file
```

**解决方案**:
1. 确保数据库目录存在
2. 检查文件权限
3. 检查磁盘空间

```cpp
// 创建数据库目录
QDir dir;
dir.mkpath(QFileInfo(config.rbacConfig.databasePath).absolutePath());
```

---

## 附录

### A. 角色权限矩阵

| 权限                        | Guest | Viewer | Operator | Admin |
|----------------------------|-------|--------|----------|-------|
| ViewPerformanceMetrics     | ✓     | ✓      | ✓        | ✓     |
| ExportPerformanceData      |       |        | ✓        | ✓     |
| ViewLogs                   | ✓     | ✓      | ✓        | ✓     |
| ExportLogs                 |       |        | ✓        | ✓     |
| ClearLogs                  |       |        |          | ✓     |
| ViewConfiguration          |       | ✓      | ✓        | ✓     |
| ModifyConfiguration        |       |        |          | ✓     |
| StartStopServices          |       |        | ✓        | ✓     |
| RestartSystem              |       |        |          | ✓     |
| ViewUsers                  |       |        |          | ✓     |
| ManageUsers                |       |        |          | ✓     |
| ManageRoles                |       |        |          | ✓     |
| ViewDatabase               |       | ✓      | ✓        | ✓     |
| ModifyDatabase             |       |        |          | ✓     |
| ExportDatabase             |       |        | ✓        | ✓     |
| CleanupDatabase            |       |        |          | ✓     |

### B. 配置模板

```json
{
  "security": {
    "enabled": true,
    "auditLog": true,
    "certificate": {
      "useSelfSigned": false,
      "certificatePath": "/etc/visionforge/certs/server.crt",
      "privateKeyPath": "/etc/visionforge/certs/server.key",
      "caBundlePath": "/etc/visionforge/certs/ca-bundle.crt"
    },
    "token": {
      "secretKey": "CHANGE_THIS_TO_A_RANDOM_32_BYTE_STRING",
      "accessTokenLifetime": 900,
      "refreshTokenLifetime": 604800,
      "enableAutoRefresh": true,
      "refreshThreshold": 300
    },
    "rbac": {
      "databasePath": "/var/lib/visionforge/security.db",
      "defaultRole": "Viewer",
      "maxLoginAttempts": 5,
      "lockoutDurationMinutes": 15
    }
  },
  "server": {
    "port": 8765,
    "enableSsl": true,
    "requireAuth": true
  }
}
```

---

**版权所有 © 2025 VisionForge Team. 保留所有权利。**
