/**
 * @file SecurityManager.cpp
 * @brief 安全统一管理器实现
 * @author VisionForge Team
 * @date 2025-12-20
 */

#include "remote/SecurityManager.h"
#include <QDebug>

namespace VisionForge {
namespace Remote {

SecurityManager::SecurityManager(QObject* parent)
    : QObject(parent)
    , enabled_(false)
    , certManager_(nullptr)
    , tokenManager_(nullptr)
    , rbacManager_(nullptr)
{
}

SecurityManager::~SecurityManager()
{
    close();
}

bool SecurityManager::initialize(const SecurityConfig& config)
{
    config_ = config;
    enabled_ = config_.enableSecurity;

    qInfo() << "===========================================";
    qInfo() << "初始化安全管理器...";
    qInfo() << "  启用安全: " << enabled_;
    qInfo() << "  启用审计日志: " << config_.enableAuditLog;
    qInfo() << "===========================================";

    if (!enabled_) {
        qWarning() << "安全功能未启用";
        emit initialized();
        return true;
    }

    // 1. 初始化证书管理器
    certManager_ = new SecurityCertificateManager(this);
    if (!certManager_->initialize(config_.certificateConfig)) {
        qCritical() << "证书管理器初始化失败";
        return false;
    }
    qInfo() << "✓ 证书管理器初始化成功";

    // 2. 初始化Token管理器
    tokenManager_ = new TokenAuthManager(this);
    if (!tokenManager_->initialize(config_.tokenConfig)) {
        qCritical() << "Token管理器初始化失败";
        return false;
    }
    qInfo() << "✓ Token管理器初始化成功";

    // 3. 初始化RBAC管理器
    rbacManager_ = new RBACManager(this);
    if (!rbacManager_->initialize(config_.rbacConfig)) {
        qCritical() << "RBAC管理器初始化失败";
        return false;
    }
    qInfo() << "✓ RBAC管理器初始化成功";

    // 4. 连接信号
    connect(rbacManager_, &RBACManager::authenticationSucceeded,
            this, &SecurityManager::authenticationSucceeded);
    connect(rbacManager_, &RBACManager::authenticationFailed,
            this, &SecurityManager::authenticationFailed);
    connect(rbacManager_, &RBACManager::permissionDenied,
            this, &SecurityManager::permissionDenied);

    qInfo() << "===========================================";
    qInfo() << "安全管理器初始化完成";
    qInfo() << "===========================================";

    emit initialized();
    return true;
}

void SecurityManager::close()
{
    if (certManager_) {
        certManager_->clear();
        certManager_->deleteLater();
        certManager_ = nullptr;
    }

    if (tokenManager_) {
        tokenManager_->deleteLater();
        tokenManager_ = nullptr;
    }

    if (rbacManager_) {
        rbacManager_->close();
        rbacManager_->deleteLater();
        rbacManager_ = nullptr;
    }

    enabled_ = false;
    qInfo() << "安全管理器已关闭";
}

ConnectionAuthResult SecurityManager::authenticateConnection(const QString& username,
                                                            const QString& password,
                                                            const QString& ipAddress)
{
    ConnectionAuthResult result;

    if (!enabled_) {
        result.errorMessage = "安全功能未启用";
        return result;
    }

    // 1. RBAC认证（验证用户名和密码）
    AuthResult authResult = rbacManager_->authenticate(username, password, ipAddress);

    if (!authResult.success) {
        result.errorMessage = authResult.errorMessage;
        logSecurityEvent("", "LOGIN_FAILED", username, false, ipAddress, authResult.errorMessage);
        return result;
    }

    // 2. 生成Token对
    auto tokenPair = tokenManager_->generateTokenPair(
        authResult.userId,
        authResult.username,
        RBACManager::roleToString(authResult.role),
        ipAddress
    );

    // 3. 构造结果
    result.authenticated = true;
    result.userId = authResult.userId;
    result.username = authResult.username;
    result.role = authResult.role;
    result.accessToken = tokenPair.accessToken;
    result.refreshToken = tokenPair.refreshToken;
    result.tokenExpiry = tokenPair.accessTokenExpiry;

    qInfo() << "连接认证成功 - 用户:" << username << "角色:" << RBACManager::roleToString(authResult.role);
    logSecurityEvent(authResult.userId, "LOGIN_SUCCESS", username, true, ipAddress);

    return result;
}

TokenValidationResult SecurityManager::validateToken(const QString& token)
{
    if (!enabled_) {
        TokenValidationResult result;
        result.errorMessage = "安全功能未启用";
        return result;
    }

    return tokenManager_->validateToken(token);
}

QString SecurityManager::refreshToken(const QString& refreshToken)
{
    if (!enabled_) {
        return QString();
    }

    return tokenManager_->refreshAccessToken(refreshToken);
}

void SecurityManager::logout(const QString& userId,
                            const QString& token,
                            const QString& ipAddress)
{
    if (!enabled_) {
        return;
    }

    // 撤销Token
    if (!token.isEmpty()) {
        tokenManager_->revokeToken(token);
    }

    // 记录登出事件
    rbacManager_->logout(userId, ipAddress);

    qInfo() << "用户登出 - ID:" << userId;
}

bool SecurityManager::authorizeAction(const QString& token, Permission permission)
{
    if (!enabled_) {
        return true;  // 安全功能未启用，默认允许
    }

    // 验证Token
    auto validationResult = tokenManager_->validateToken(token);
    if (!validationResult.isValid) {
        logSecurityEvent("", "AUTHORIZATION_FAILED", RBACManager::permissionToString(permission),
                        false, "", "Token无效");
        return false;
    }

    // 检查权限
    bool hasPermission = rbacManager_->hasPermission(validationResult.userId, permission);

    if (!hasPermission) {
        logSecurityEvent(validationResult.userId, "PERMISSION_DENIED",
                        RBACManager::permissionToString(permission), false);
    }

    return hasPermission;
}

bool SecurityManager::authorizeUser(const QString& userId, Permission permission)
{
    if (!enabled_) {
        return true;  // 安全功能未启用，默认允许
    }

    return rbacManager_->checkPermission(userId, permission);
}

QMap<Permission, bool> SecurityManager::checkPermissions(const QString& userId,
                                                        const QList<Permission>& permissions)
{
    if (!enabled_) {
        QMap<Permission, bool> result;
        for (auto perm : permissions) {
            result[perm] = true;
        }
        return result;
    }

    return rbacManager_->checkPermissions(userId, permissions);
}

bool SecurityManager::addUser(const QString& username,
                             const QString& password,
                             UserRole role,
                             const QString& email,
                             const QString& description)
{
    if (!enabled_) {
        return false;
    }

    bool success = rbacManager_->addUser(username, password, role, email, description);

    if (success) {
        logSecurityEvent("", "USER_ADDED", username, true, "",
                        QString("角色: %1").arg(RBACManager::roleToString(role)));
    }

    return success;
}

bool SecurityManager::updateUserPassword(const QString& userId, const QString& newPassword)
{
    if (!enabled_) {
        return false;
    }

    bool success = rbacManager_->updateUserPassword(userId, newPassword);

    if (success) {
        logSecurityEvent(userId, "PASSWORD_CHANGED", "", true);
    }

    return success;
}

bool SecurityManager::updateUserRole(const QString& userId, UserRole newRole)
{
    if (!enabled_) {
        return false;
    }

    bool success = rbacManager_->updateUserRole(userId, newRole);

    if (success) {
        logSecurityEvent(userId, "ROLE_CHANGED", RBACManager::roleToString(newRole), true);
    }

    return success;
}

void SecurityManager::logSecurityEvent(const QString& userId,
                                      const QString& action,
                                      const QString& resource,
                                      bool success,
                                      const QString& ipAddress,
                                      const QString& details)
{
    if (!enabled_ || !config_.enableAuditLog) {
        return;
    }

    rbacManager_->logAuditEvent(userId, action, resource, success, ipAddress, details);

    QString event = QString("[%1] %2 - %3 %4")
                    .arg(success ? "SUCCESS" : "FAILED")
                    .arg(action)
                    .arg(resource)
                    .arg(details.isEmpty() ? "" : "(" + details + ")");

    emit securityEventLogged(event);
}

QList<AuditLogEntry> SecurityManager::queryAuditLog(const QDateTime& startTime,
                                                   const QDateTime& endTime,
                                                   const QString& userId,
                                                   int limit)
{
    if (!enabled_) {
        return QList<AuditLogEntry>();
    }

    return rbacManager_->queryAuditLog(startTime, endTime, userId, limit);
}

} // namespace Remote
} // namespace VisionForge
