/**
 * @file SecurityManager.h
 * @brief 安全统一管理器 - 集成所有安全组件
 * @details 提供统一的认证、授权和安全配置接口
 * @author VisionForge Team
 * @date 2025-12-20
 */

#pragma once

#include "remote/SecurityCertificateManager.h"
#include "remote/TokenAuthManager.h"
#include "remote/RBACManager.h"
#include <QObject>
#include <QString>
#include <QDateTime>

namespace VisionForge {
namespace Remote {

/**
 * @brief 安全配置
 */
struct SecurityConfig {
    // SSL配置
    CertificateConfig certificateConfig;

    // Token配置
    TokenConfig tokenConfig;

    // RBAC配置
    RBACConfig rbacConfig;

    // 全局配置
    bool enableSecurity = true;              // 启用安全功能
    bool enableAuditLog = true;              // 启用审计日志
};

/**
 * @brief 连接认证结果
 */
struct ConnectionAuthResult {
    bool authenticated = false;              // 是否认证成功
    QString userId;                          // 用户ID
    QString username;                        // 用户名
    UserRole role;                           // 角色
    QString accessToken;                     // 访问Token
    QString refreshToken;                    // 刷新Token
    QDateTime tokenExpiry;                   // Token过期时间
    QString errorMessage;                    // 错误信息
};

/**
 * @class SecurityManager
 * @brief 安全统一管理器
 *
 * 功能特性:
 * - 统一的安全组件管理
 * - 简化的认证接口
 * - 统一的授权检查
 * - 审计日志记录
 * - SSL/TLS支持
 */
class SecurityManager : public QObject
{
    Q_OBJECT

public:
    explicit SecurityManager(QObject* parent = nullptr);
    ~SecurityManager() override;

    /**
     * @brief 初始化安全管理器
     */
    bool initialize(const SecurityConfig& config = SecurityConfig());

    /**
     * @brief 关闭安全管理器
     */
    void close();

    /**
     * @brief 安全功能是否启用
     */
    bool isEnabled() const { return enabled_; }

    // ========== 组件访问 ==========

    /**
     * @brief 获取证书管理器
     */
    SecurityCertificateManager* certificateManager() { return certManager_; }

    /**
     * @brief 获取Token管理器
     */
    TokenAuthManager* tokenAuthManager() { return tokenManager_; }

    /**
     * @brief 获取RBAC管理器
     */
    RBACManager* rbacManager() { return rbacManager_; }

    // ========== 统一认证接口 ==========

    /**
     * @brief 认证连接（用户名+密码）
     */
    ConnectionAuthResult authenticateConnection(const QString& username,
                                               const QString& password,
                                               const QString& ipAddress = QString());

    /**
     * @brief 验证Token
     */
    TokenValidationResult validateToken(const QString& token);

    /**
     * @brief 刷新Token
     */
    QString refreshToken(const QString& refreshToken);

    /**
     * @brief 登出
     */
    void logout(const QString& userId, const QString& token = QString(),
                const QString& ipAddress = QString());

    // ========== 统一授权接口 ==========

    /**
     * @brief 检查权限（通过Token）
     */
    bool authorizeAction(const QString& token, Permission permission);

    /**
     * @brief 检查权限（通过用户ID）
     */
    bool authorizeUser(const QString& userId, Permission permission);

    /**
     * @brief 批量检查权限
     */
    QMap<Permission, bool> checkPermissions(const QString& userId,
                                           const QList<Permission>& permissions);

    // ========== 用户管理便捷接口 ==========

    /**
     * @brief 添加用户
     */
    bool addUser(const QString& username,
                const QString& password,
                UserRole role,
                const QString& email = QString(),
                const QString& description = QString());

    /**
     * @brief 更新用户密码
     */
    bool updateUserPassword(const QString& userId, const QString& newPassword);

    /**
     * @brief 更新用户角色
     */
    bool updateUserRole(const QString& userId, UserRole newRole);

    // ========== 审计日志 ==========

    /**
     * @brief 记录安全事件
     */
    void logSecurityEvent(const QString& userId,
                         const QString& action,
                         const QString& resource,
                         bool success,
                         const QString& ipAddress = QString(),
                         const QString& details = QString());

    /**
     * @brief 查询审计日志
     */
    QList<AuditLogEntry> queryAuditLog(const QDateTime& startTime,
                                      const QDateTime& endTime,
                                      const QString& userId = QString(),
                                      int limit = 1000);

signals:
    /**
     * @brief 安全管理器初始化完成
     */
    void initialized();

    /**
     * @brief 认证成功
     */
    void authenticationSucceeded(const QString& userId, const QString& username);

    /**
     * @brief 认证失败
     */
    void authenticationFailed(const QString& username, const QString& reason);

    /**
     * @brief 权限拒绝
     */
    void permissionDenied(const QString& userId, Permission permission);

    /**
     * @brief 安全事件
     */
    void securityEventLogged(const QString& event);

private:
    SecurityConfig config_;                      // 安全配置
    bool enabled_;                               // 是否启用安全功能

    SecurityCertificateManager* certManager_;    // 证书管理器
    TokenAuthManager* tokenManager_;             // Token管理器
    RBACManager* rbacManager_;                   // RBAC管理器
};

} // namespace Remote
} // namespace VisionForge
