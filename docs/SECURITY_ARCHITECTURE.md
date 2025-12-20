# VisionForge Pro - 远程诊断安全架构设计

**版本**: v1.2.0
**日期**: 2025-12-20
**作者**: VisionForge Team

---

## 1. 概述

### 1.1 安全目标

- **机密性**: 确保通信数据加密传输（WSS）
- **完整性**: 防止数据被篡改（Token签名验证）
- **可用性**: 防止未授权访问（认证+授权）
- **可审计性**: 记录所有安全相关操作

### 1.2 安全组件

```
┌─────────────────────────────────────────────────────────┐
│              远程诊断安全架构                              │
├─────────────────────────────────────────────────────────┤
│                                                         │
│  ┌─────────────────────────────────────────────────┐   │
│  │  1. SSL/TLS 证书管理 (SecurityCertificateManager)│   │
│  │     - 证书加载/验证                               │   │
│  │     - 自签名证书生成                              │   │
│  │     - 证书链验证                                  │   │
│  └─────────────────────────────────────────────────┘   │
│                          ↓                              │
│  ┌─────────────────────────────────────────────────┐   │
│  │  2. WSS 支持 (WebSocketServer 升级)             │   │
│  │     - QSslConfiguration                          │   │
│  │     - wss:// 协议支持                            │   │
│  │     - TLS 1.2/1.3                                │   │
│  └─────────────────────────────────────────────────┘   │
│                          ↓                              │
│  ┌─────────────────────────────────────────────────┐   │
│  │  3. Token 认证管理 (TokenAuthManager)           │   │
│  │     - JWT Token 生成/验证                        │   │
│  │     - HMAC-SHA256 签名                           │   │
│  │     - Token 过期/刷新                            │   │
│  │     - 黑名单管理                                 │   │
│  └─────────────────────────────────────────────────┘   │
│                          ↓                              │
│  ┌─────────────────────────────────────────────────┐   │
│  │  4. RBAC 权限控制 (RBACManager)                 │   │
│  │     - 角色定义 (Admin/Operator/Viewer)          │   │
│  │     - 权限检查                                   │   │
│  │     - 用户-角色映射                              │   │
│  │     - 数据库持久化                               │   │
│  └─────────────────────────────────────────────────┘   │
│                                                         │
└─────────────────────────────────────────────────────────┘
```

---

## 2. WSS (WebSocket Secure) 支持

### 2.1 SSL/TLS 证书管理

#### 2.1.1 SecurityCertificateManager 类设计

**职责**:
- 加载和管理SSL证书
- 支持自签名证书生成（开发/测试环境）
- 证书验证和链验证
- 证书更新管理

**接口设计**:
```cpp
class SecurityCertificateManager : public QObject {
    Q_OBJECT
public:
    // 加载证书和私钥
    bool loadCertificate(const QString& certPath, const QString& keyPath);

    // 生成自签名证书（用于开发/测试）
    bool generateSelfSignedCertificate(
        const QString& commonName = "VisionForge Server",
        int validDays = 365
    );

    // 获取SSL配置
    QSslConfiguration getSslConfiguration() const;

    // 验证证书
    bool verifyCertificate() const;

    // 证书信息
    QSslCertificate certificate() const;
    QSslKey privateKey() const;
    QDateTime expiryDate() const;

signals:
    void certificateLoaded();
    void certificateError(const QString& error);
    void certificateExpiringSoon(int daysRemaining);

private:
    QSslCertificate certificate_;
    QSslKey privateKey_;
    QList<QSslCertificate> caCertificates_;
};
```

#### 2.1.2 证书配置

**生产环境**:
```json
{
    "ssl": {
        "enabled": true,
        "certificatePath": "/etc/visionforge/certs/server.crt",
        "privateKeyPath": "/etc/visionforge/certs/server.key",
        "caBundlePath": "/etc/visionforge/certs/ca-bundle.crt",
        "tlsVersion": "TLS_1_3"
    }
}
```

**开发环境**:
```json
{
    "ssl": {
        "enabled": true,
        "useSelfSigned": true,
        "commonName": "localhost",
        "validDays": 365
    }
}
```

### 2.2 WebSocketServer WSS升级

#### 2.2.1 修改点

1. **添加SSL模式支持**:
```cpp
class WebSocketServer : public QObject {
    Q_OBJECT
public:
    enum SecureMode {
        NonSecure,      // ws://
        Secure          // wss://
    };

    bool start(quint16 port = 8765, SecureMode mode = NonSecure);
    void setSslConfiguration(const QSslConfiguration& config);

private:
    SecureMode secureMode_;
    QSslConfiguration sslConfig_;
};
```

2. **SSL握手处理**:
```cpp
void WebSocketServer::onNewConnection() {
    QWebSocket* socket = server_->nextPendingConnection();

    if (secureMode_ == Secure) {
        // 配置SSL
        socket->setSslConfiguration(sslConfig_);

        // 连接SSL错误信号
        connect(socket, &QWebSocket::sslErrors, this, &WebSocketServer::onSslErrors);
    }

    // ... 其他处理
}

void WebSocketServer::onSslErrors(const QList<QSslError>& errors) {
    for (const auto& error : errors) {
        qWarning() << "SSL Error:" << error.errorString();
    }
}
```

---

## 3. Token 认证机制

### 3.1 JWT (JSON Web Token) 实现

#### 3.1.1 Token 结构

```
Header.Payload.Signature
```

**Header** (Base64):
```json
{
    "alg": "HS256",
    "typ": "JWT"
}
```

**Payload** (Base64):
```json
{
    "sub": "user123",              // 用户ID
    "username": "admin",           // 用户名
    "role": "Admin",               // 角色
    "iat": 1703001600,            // 签发时间
    "exp": 1703088000,            // 过期时间
    "jti": "uuid-xxxx"            // Token唯一ID
}
```

**Signature** (HMAC-SHA256):
```
HMACSHA256(
    base64UrlEncode(header) + "." + base64UrlEncode(payload),
    secret_key
)
```

#### 3.1.2 TokenAuthManager 类设计

```cpp
class TokenAuthManager : public QObject {
    Q_OBJECT
public:
    // 生成Token
    QString generateToken(
        const QString& userId,
        const QString& username,
        const QString& role,
        int expiresInSeconds = 3600  // 默认1小时
    );

    // 验证Token
    struct TokenValidationResult {
        bool isValid;
        QString userId;
        QString username;
        QString role;
        QString errorMessage;
        QDateTime expiryTime;
    };

    TokenValidationResult validateToken(const QString& token);

    // 刷新Token
    QString refreshToken(const QString& oldToken);

    // 撤销Token（加入黑名单）
    void revokeToken(const QString& token);

    // 检查Token是否在黑名单
    bool isTokenRevoked(const QString& token) const;

    // 清理过期Token
    void cleanupExpiredTokens();

    // 设置密钥
    void setSecretKey(const QString& key);

signals:
    void tokenGenerated(const QString& userId, const QString& token);
    void tokenValidated(const QString& userId);
    void tokenRevoked(const QString& token);
    void tokenExpired(const QString& token);

private:
    QString generateJWT(const QJsonObject& payload);
    QJsonObject parseJWT(const QString& token);
    QString signPayload(const QString& data);
    bool verifySignature(const QString& data, const QString& signature);

    QString secretKey_;
    QMap<QString, QDateTime> tokenBlacklist_;  // token -> 过期时间
    QMutex mutex_;
};
```

#### 3.1.3 认证流程

```
Client                          Server
  │                               │
  │  1. 登录请求 (username/pwd)    │
  │ ──────────────────────────────>│
  │                               │ 验证用户名密码
  │                               │ 生成JWT Token
  │  2. 返回Token                  │
  │ <──────────────────────────────│
  │                               │
  │  3. WebSocket连接 (带Token)    │
  │ ──────────────────────────────>│
  │                               │ 验证Token
  │                               │ 提取用户信息
  │  4. 连接成功                   │
  │ <──────────────────────────────│
  │                               │
  │  5. 后续请求 (带Token)         │
  │ ──────────────────────────────>│
  │                               │ 每次请求验证Token
  │                               │ 检查权限
  │  6. 响应                       │
  │ <──────────────────────────────│
```

#### 3.1.4 Token 刷新策略

```cpp
// Token配置
struct TokenConfig {
    int accessTokenLifetime = 3600;      // 访问Token有效期: 1小时
    int refreshTokenLifetime = 86400;    // 刷新Token有效期: 24小时
    bool enableAutoRefresh = true;       // 启用自动刷新
    int refreshThreshold = 300;          // 剩余5分钟时自动刷新
};

// 客户端自动刷新逻辑
void checkTokenExpiry() {
    int remainingSeconds = tokenExpiryTime.secsTo(QDateTime::currentDateTime());

    if (remainingSeconds < config.refreshThreshold) {
        // 即将过期，请求刷新
        QString newToken = requestTokenRefresh(currentToken);
        updateToken(newToken);
    }
}
```

---

## 4. RBAC 权限控制

### 4.1 角色定义

#### 4.1.1 预定义角色

```cpp
enum class UserRole {
    Admin,          // 管理员：所有权限
    Operator,       // 操作员：读+写（不包括配置）
    Viewer,         // 查看者：只读
    Guest           // 访客：受限只读
};

// 角色权限映射
struct RolePermissions {
    UserRole role;
    QSet<Permission> permissions;
};

// 权限枚举
enum class Permission {
    // 性能监控
    ViewPerformanceMetrics,
    ExportPerformanceData,

    // 日志
    ViewLogs,
    ExportLogs,
    ClearLogs,

    // 配置
    ViewConfiguration,
    ModifyConfiguration,

    // 系统控制
    StartStopServices,
    RestartSystem,

    // 用户管理
    ViewUsers,
    ManageUsers,
    ManageRoles,

    // 数据库
    ViewDatabase,
    ModifyDatabase,
    ExportDatabase,
    CleanupDatabase
};
```

#### 4.1.2 默认角色权限

```cpp
RolePermissions getDefaultPermissions(UserRole role) {
    switch (role) {
        case UserRole::Admin:
            return {role, {
                // 所有权限
                Permission::ViewPerformanceMetrics,
                Permission::ExportPerformanceData,
                Permission::ViewLogs,
                Permission::ExportLogs,
                Permission::ClearLogs,
                Permission::ViewConfiguration,
                Permission::ModifyConfiguration,
                Permission::StartStopServices,
                Permission::RestartSystem,
                Permission::ViewUsers,
                Permission::ManageUsers,
                Permission::ManageRoles,
                Permission::ViewDatabase,
                Permission::ModifyDatabase,
                Permission::ExportDatabase,
                Permission::CleanupDatabase
            }};

        case UserRole::Operator:
            return {role, {
                Permission::ViewPerformanceMetrics,
                Permission::ExportPerformanceData,
                Permission::ViewLogs,
                Permission::ExportLogs,
                Permission::ViewConfiguration,
                Permission::StartStopServices,
                Permission::ViewDatabase,
                Permission::ExportDatabase
            }};

        case UserRole::Viewer:
            return {role, {
                Permission::ViewPerformanceMetrics,
                Permission::ViewLogs,
                Permission::ViewConfiguration,
                Permission::ViewDatabase
            }};

        case UserRole::Guest:
            return {role, {
                Permission::ViewPerformanceMetrics,
                Permission::ViewLogs
            }};
    }
}
```

### 4.2 RBACManager 类设计

```cpp
class RBACManager : public QObject {
    Q_OBJECT
public:
    // 用户管理
    bool addUser(const QString& userId, const QString& username,
                 const QString& passwordHash, UserRole role);
    bool removeUser(const QString& userId);
    bool updateUserRole(const QString& userId, UserRole newRole);

    // 认证
    struct AuthResult {
        bool success;
        QString userId;
        QString username;
        UserRole role;
        QString errorMessage;
    };

    AuthResult authenticate(const QString& username, const QString& password);

    // 权限检查
    bool hasPermission(const QString& userId, Permission permission) const;
    bool checkPermission(const QString& userId, Permission permission);

    // 角色查询
    UserRole getUserRole(const QString& userId) const;
    QSet<Permission> getUserPermissions(const QString& userId) const;

    // 批量权限检查
    QMap<Permission, bool> checkPermissions(const QString& userId,
                                           const QList<Permission>& permissions);

    // 数据库持久化
    bool loadFromDatabase();
    bool saveToDatabase();

signals:
    void userAdded(const QString& userId);
    void userRemoved(const QString& userId);
    void roleChanged(const QString& userId, UserRole newRole);
    void authenticationFailed(const QString& username, const QString& reason);
    void permissionDenied(const QString& userId, Permission permission);

private:
    struct UserInfo {
        QString userId;
        QString username;
        QString passwordHash;  // SHA256
        UserRole role;
        QDateTime createdAt;
        QDateTime lastLoginAt;
    };

    QMap<QString, UserInfo> users_;              // userId -> UserInfo
    QMap<QString, QString> usernameToId_;        // username -> userId
    QMap<UserRole, QSet<Permission>> rolePermissions_;

    QString hashPassword(const QString& password) const;
    bool verifyPassword(const QString& password, const QString& hash) const;

    mutable QMutex mutex_;
};
```

### 4.3 数据库架构

#### 4.3.1 用户表

```sql
CREATE TABLE users (
    user_id TEXT PRIMARY KEY,
    username TEXT UNIQUE NOT NULL,
    password_hash TEXT NOT NULL,
    role INTEGER NOT NULL,              -- 0=Guest, 1=Viewer, 2=Operator, 3=Admin
    created_at DATETIME NOT NULL,
    last_login_at DATETIME,
    is_active BOOLEAN DEFAULT 1,
    email TEXT,
    description TEXT
);

CREATE INDEX idx_username ON users(username);
CREATE INDEX idx_role ON users(role);
```

#### 4.3.2 角色权限表

```sql
CREATE TABLE role_permissions (
    role INTEGER NOT NULL,
    permission INTEGER NOT NULL,
    PRIMARY KEY (role, permission)
);
```

#### 4.3.3 审计日志表

```sql
CREATE TABLE security_audit_log (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    timestamp DATETIME NOT NULL,
    user_id TEXT,
    action TEXT NOT NULL,               -- LOGIN, LOGOUT, PERMISSION_DENIED, etc.
    resource TEXT,                       -- 访问的资源
    result TEXT,                         -- SUCCESS, FAILED
    ip_address TEXT,
    details TEXT
);

CREATE INDEX idx_audit_timestamp ON security_audit_log(timestamp);
CREATE INDEX idx_audit_user ON security_audit_log(user_id);
CREATE INDEX idx_audit_action ON security_audit_log(action);
```

---

## 5. 集成方案

### 5.1 安全层架构

```cpp
class SecurityLayer : public QObject {
    Q_OBJECT
public:
    // 初始化
    bool initialize(const SecurityConfig& config);

    // 子组件访问
    SecurityCertificateManager* certificateManager() { return certManager_; }
    TokenAuthManager* tokenAuthManager() { return tokenManager_; }
    RBACManager* rbacManager() { return rbacManager_; }

    // 统一认证接口
    struct ConnectionAuthResult {
        bool authenticated;
        QString userId;
        QString username;
        UserRole role;
        QString token;
        QString errorMessage;
    };

    ConnectionAuthResult authenticateConnection(
        const QString& username,
        const QString& password
    );

    // 统一授权接口
    bool authorizeAction(const QString& token, Permission permission);

    // 审计日志
    void logSecurityEvent(const QString& userId, const QString& action,
                         const QString& resource, bool success);

signals:
    void authenticationSucceeded(const QString& userId);
    void authenticationFailed(const QString& username);
    void permissionDenied(const QString& userId, Permission permission);
    void securityEventLogged(const QString& event);

private:
    SecurityCertificateManager* certManager_;
    TokenAuthManager* tokenManager_;
    RBACManager* rbacManager_;
    RemoteDatabaseManager* dbManager_;  // 用于审计日志
};
```

### 5.2 配置文件

```json
{
    "security": {
        "ssl": {
            "enabled": true,
            "certificatePath": "./certs/server.crt",
            "privateKeyPath": "./certs/server.key",
            "useSelfSigned": false
        },
        "token": {
            "secretKey": "your-secret-key-here",
            "accessTokenLifetime": 3600,
            "refreshTokenLifetime": 86400,
            "enableAutoRefresh": true
        },
        "rbac": {
            "defaultRole": "Viewer",
            "enableGuestAccess": false,
            "maxLoginAttempts": 5,
            "lockoutDurationMinutes": 15
        },
        "audit": {
            "enabled": true,
            "logAuthentication": true,
            "logPermissionDenials": true,
            "logDataAccess": true
        }
    }
}
```

---

## 6. 使用示例

### 6.1 服务端初始化

```cpp
// 1. 创建安全层
auto* securityLayer = new SecurityLayer(this);

SecurityConfig secConfig;
secConfig.sslEnabled = true;
secConfig.useSelfSignedCert = true;
secConfig.tokenSecretKey = "your-secret-key";

if (!securityLayer->initialize(secConfig)) {
    qCritical() << "安全层初始化失败";
    return;
}

// 2. 配置WebSocket服务器使用WSS
auto* server = new WebSocketServer(this);
server->setSslConfiguration(
    securityLayer->certificateManager()->getSslConfiguration()
);
server->start(8765, WebSocketServer::Secure);

// 3. 连接认证处理
connect(server, &WebSocketServer::newConnection, this, [=](QWebSocket* socket) {
    // 等待客户端发送Token
    connect(socket, &QWebSocket::textMessageReceived, this, [=](const QString& message) {
        QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());
        QString token = doc["token"].toString();

        // 验证Token
        auto result = securityLayer->tokenAuthManager()->validateToken(token);
        if (!result.isValid) {
            socket->sendTextMessage(R"({"error": "Invalid token"})");
            socket->close();
            return;
        }

        // Token有效，标记连接
        socket->setProperty("userId", result.userId);
        socket->setProperty("role", result.role);
    });
});
```

### 6.2 请求权限检查

```cpp
void handleRequest(QWebSocket* socket, const QJsonObject& request) {
    QString action = request["action"].toString();
    QString userId = socket->property("userId").toString();

    // 权限检查
    Permission requiredPermission;
    if (action == "export_logs") {
        requiredPermission = Permission::ExportLogs;
    } else if (action == "modify_config") {
        requiredPermission = Permission::ModifyConfiguration;
    }
    // ... 其他action

    if (!rbacManager->hasPermission(userId, requiredPermission)) {
        QJsonObject response;
        response["error"] = "Permission denied";
        socket->sendTextMessage(QJsonDocument(response).toJson());

        // 记录审计日志
        securityLayer->logSecurityEvent(userId, action, "denied", false);
        return;
    }

    // 执行实际操作
    // ...
}
```

### 6.3 客户端连接示例

```javascript
// JavaScript客户端
class SecureRemoteClient {
    constructor(serverUrl, username, password) {
        this.serverUrl = serverUrl;
        this.username = username;
        this.password = password;
        this.token = null;
        this.socket = null;
    }

    async connect() {
        // 1. 登录获取Token
        const response = await fetch(`${this.serverUrl}/api/login`, {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({
                username: this.username,
                password: this.password
            })
        });

        const data = await response.json();
        if (!data.success) {
            throw new Error('Authentication failed');
        }

        this.token = data.token;

        // 2. 建立WSS连接
        this.socket = new WebSocket(`wss://${this.serverUrl}:8765`);

        this.socket.onopen = () => {
            // 发送Token进行认证
            this.socket.send(JSON.stringify({
                type: 'auth',
                token: this.token
            }));
        };

        this.socket.onmessage = (event) => {
            const message = JSON.parse(event.data);
            this.handleMessage(message);
        };
    }

    sendRequest(action, params = {}) {
        this.socket.send(JSON.stringify({
            action: action,
            token: this.token,
            params: params
        }));
    }
}

// 使用示例
const client = new SecureRemoteClient('localhost', 'admin', 'password123');
await client.connect();
client.sendRequest('get_performance_metrics', { limit: 100 });
```

---

## 7. 安全最佳实践

### 7.1 证书管理

1. **生产环境使用正式证书**: 从CA获取签名证书
2. **定期更新证书**: 设置过期提醒，提前更新
3. **私钥保护**:
   - 使用强加密存储私钥
   - 限制私钥文件权限 (chmod 600)
   - 不要将私钥提交到版本控制

### 7.2 Token安全

1. **强密钥**: 使用至少32字节的随机密钥
2. **短生命周期**: 访问Token不超过1小时
3. **HTTPS传输**: Token只通过HTTPS/WSS传输
4. **安全存储**: 客户端使用安全存储（如Keychain）

### 7.3 密码策略

1. **强密码要求**:
   - 最少8个字符
   - 包含大小写字母、数字、特殊字符
2. **密码哈希**: 使用SHA-256 + Salt
3. **防暴力破解**:
   - 限制登录尝试次数
   - 实施账户锁定机制

### 7.4 审计日志

1. **记录关键事件**:
   - 登录/登出
   - 权限拒绝
   - 配置修改
   - 数据导出
2. **日志保护**: 审计日志不可被普通用户修改
3. **定期审查**: 定期检查异常访问模式

---

## 8. 性能考虑

### 8.1 Token验证优化

```cpp
// 使用缓存减少重复验证
class TokenCache {
    QCache<QString, TokenValidationResult> cache_;

    TokenValidationResult validate(const QString& token) {
        if (cache_.contains(token)) {
            return *cache_[token];
        }

        auto result = actualValidate(token);
        cache_.insert(token, new TokenValidationResult(result),
                     result.expiryTime.secsTo(QDateTime::currentDateTime()));
        return result;
    }
};
```

### 8.2 权限检查优化

```cpp
// 预加载用户权限到内存
QSet<Permission> getUserPermissions(const QString& userId) {
    static QCache<QString, QSet<Permission>> permissionCache;

    if (!permissionCache.contains(userId)) {
        auto perms = loadPermissionsFromDB(userId);
        permissionCache.insert(userId, new QSet<Permission>(perms));
    }

    return *permissionCache[userId];
}
```

---

## 9. 测试计划

### 9.1 单元测试

- SSL证书加载测试
- Token生成/验证测试
- 权限检查测试
- 密码哈希测试

### 9.2 集成测试

- WSS连接测试
- 完整认证流程测试
- 权限拒绝测试
- Token刷新测试

### 9.3 安全测试

- SQL注入测试
- XSS测试
- CSRF测试
- 暴力破解测试
- Token伪造测试

---

## 10. 后续扩展

### 10.1 双因素认证 (2FA)

- TOTP (Time-based One-Time Password)
- SMS验证码
- 邮箱验证码

### 10.2 OAuth 2.0 集成

- 支持第三方登录
- SSO (Single Sign-On)

### 10.3 细粒度权限

- 资源级权限控制
- 动态权限分配
- 权限继承

---

**版权所有 © 2025 VisionForge Team. 保留所有权利。**
