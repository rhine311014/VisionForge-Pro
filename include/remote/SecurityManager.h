/**
 * @file SecurityManager.h
 * @brief 安全统一管理器 - 集成所有安全组件
 * @details 提供统一的认证、授权和安全配置接口
 *
 * @section security_overview 安全架构概述
 * SecurityManager是远程诊断系统的安全中枢，整合了以下安全模块:
 * - SecurityCertificateManager: SSL/TLS证书管理，提供传输层加密
 * - TokenAuthManager: JWT Token认证管理，实现无状态认证
 * - RBACManager: 基于角色的访问控制，实现细粒度权限管理
 *
 * @section authentication_flow 认证流程
 * @code
 * 1. 客户端 --> [用户名/密码] --> SecurityManager
 * 2. SecurityManager --> RBACManager.authenticate() --> 验证凭据
 * 3. 验证通过 --> TokenAuthManager.generateTokenPair() --> 生成JWT
 * 4. SecurityManager --> [Token对] --> 客户端
 * 5. 客户端 --> [携带AccessToken] --> 后续请求
 * 6. SecurityManager --> TokenAuthManager.validateToken() --> 验证请求
 * 7. Token即将过期 --> 使用RefreshToken刷新
 * @endcode
 *
 * @section authorization_flow 授权流程
 * @code
 * 1. 请求 --> SecurityManager.authorizeAction(token, permission)
 * 2. TokenAuthManager.validateToken() --> 获取用户角色
 * 3. RBACManager.hasPermission(userId, permission) --> 检查权限
 * 4. 返回允许/拒绝，记录审计日志
 * @endcode
 *
 * @section security_layers 安全层次
 * | 层次 | 组件 | 功能 |
 * |------|------|------|
 * | 传输层 | SecurityCertificateManager | SSL/TLS加密 |
 * | 认证层 | TokenAuthManager | JWT Token认证 |
 * | 授权层 | RBACManager | 角色权限控制 |
 * | 审计层 | RBACManager | 操作日志记录 |
 *
 * @section usage_example 使用示例
 * @code
 * SecurityManager security;
 *
 * // 初始化安全管理器
 * SecurityConfig config;
 * config.enableSecurity = true;
 * config.enableAuditLog = true;
 * config.tokenConfig.secretKey = "your-secret-key";
 * config.certificateConfig.enabled = true;
 * security.initialize(config);
 *
 * // 用户认证
 * auto result = security.authenticateConnection("admin", "password", "192.168.1.1");
 * if (result.authenticated) {
 *     QString token = result.accessToken;
 *
 *     // 检查权限
 *     if (security.authorizeAction(token, Permission::ModifyConfiguration)) {
 *         // 执行操作...
 *     }
 * }
 * @endcode
 *
 * @author VisionForge Team
 * @version 1.6.0
 * @date 2025-12-20
 * @copyright Copyright (c) 2025 VisionForge. All rights reserved.
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
