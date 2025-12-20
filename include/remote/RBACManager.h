/**
 * @file RBACManager.h
 * @brief RBAC (Role-Based Access Control) 权限管理器
 * @details 负责用户管理、角色定义和权限检查
 * @author VisionForge Team
 * @date 2025-12-20
 */

#pragma once

#include <QObject>
#include <QString>
#include <QSet>
#include <QMap>
#include <QDateTime>
#include <QMutex>
#include <QSqlDatabase>

namespace VisionForge {
namespace Remote {

// 前向声明
class RemoteDatabaseManager;

/**
 * @brief 用户角色
 */
enum class UserRole {
    Guest = 0,          // 访客：受限只读
    Viewer = 1,         // 查看者：只读
    Operator = 2,       // 操作员：读+写（不包括配置）
    Admin = 3           // 管理员：所有权限
};

/**
 * @brief 权限枚举
 */
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

/**
 * @brief RBAC配置
 */
struct RBACConfig {
    QString databasePath = "./data/diagnostics.db";  // 数据库路径（与RemoteDatabaseManager共享）
    UserRole defaultRole = UserRole::Viewer;         // 默认角色
    bool enableGuestAccess = false;                  // 启用访客访问
    int maxLoginAttempts = 5;                        // 最大登录尝试次数
    int lockoutDurationMinutes = 15;                 // 锁定时长（分钟）
    bool enableAuditLog = true;                      // 启用审计日志
};

/**
 * @brief 用户信息
 */
struct UserInfo {
    QString userId;
    QString username;
    QString passwordHash;
    UserRole role;
    QDateTime createdAt;
    QDateTime lastLoginAt;
    bool isActive;
    QString email;
    QString description;
    int loginAttempts;                               // 登录尝试次数
    QDateTime lockedUntil;                           // 锁定截止时间
};

/**
 * @brief 认证结果
 */
struct AuthResult {
    bool success;
    QString userId;
    QString username;
    UserRole role;
    QString errorMessage;
    int remainingAttempts;                           // 剩余尝试次数
};

/**
 * @brief 审计日志条目
 */
struct AuditLogEntry {
    qint64 id;
    QDateTime timestamp;
    QString userId;
    QString action;                                  // LOGIN, LOGOUT, PERMISSION_DENIED等
    QString resource;                                // 访问的资源
    QString result;                                  // SUCCESS, FAILED
    QString ipAddress;
    QString details;
};

/**
 * @class RBACManager
 * @brief RBAC权限管理器
 *
 * 功能特性:
 * - 用户CRUD操作
 * - 角色权限管理
 * - 用户认证
 * - 权限检查
 * - 审计日志
 * - 防暴力破解（登录尝试限制）
 */
class RBACManager : public QObject
{
    Q_OBJECT

public:
    explicit RBACManager(QObject* parent = nullptr);
    ~RBACManager() override;

    /**
     * @brief 初始化RBAC管理器
     */
    bool initialize(const RBACConfig& config = RBACConfig());

    /**
     * @brief 关闭并保存
     */
    void close();

    // ========== 用户管理 ==========

    /**
     * @brief 添加用户
     */
    bool addUser(const QString& username,
                 const QString& password,
                 UserRole role,
                 const QString& email = QString(),
                 const QString& description = QString());

    /**
     * @brief 移除用户
     */
    bool removeUser(const QString& userId);

    /**
     * @brief 更新用户角色
     */
    bool updateUserRole(const QString& userId, UserRole newRole);

    /**
     * @brief 更新用户密码
     */
    bool updateUserPassword(const QString& userId, const QString& newPassword);

    /**
     * @brief 激活/停用用户
     */
    bool setUserActive(const QString& userId, bool active);

    /**
     * @brief 获取用户信息
     */
    UserInfo getUserInfo(const QString& userId) const;

    /**
     * @brief 获取所有用户
     */
    QList<UserInfo> getAllUsers() const;

    /**
     * @brief 用户是否存在
     */
    bool userExists(const QString& username) const;

    // ========== 认证 ==========

    /**
     * @brief 认证用户（验证用户名和密码）
     */
    AuthResult authenticate(const QString& username,
                           const QString& password,
                           const QString& ipAddress = QString());

    /**
     * @brief 登出（记录审计日志）
     */
    void logout(const QString& userId, const QString& ipAddress = QString());

    // ========== 权限检查 ==========

    /**
     * @brief 检查用户是否有指定权限
     */
    bool hasPermission(const QString& userId, Permission permission) const;

    /**
     * @brief 检查用户权限（会记录审计日志）
     */
    bool checkPermission(const QString& userId,
                        Permission permission,
                        const QString& resource = QString());

    /**
     * @brief 批量检查权限
     */
    QMap<Permission, bool> checkPermissions(const QString& userId,
                                           const QList<Permission>& permissions);

    /**
     * @brief 获取用户角色
     */
    UserRole getUserRole(const QString& userId) const;

    /**
     * @brief 获取用户的所有权限
     */
    QSet<Permission> getUserPermissions(const QString& userId) const;

    /**
     * @brief 获取角色的默认权限
     */
    static QSet<Permission> getRolePermissions(UserRole role);

    // ========== 审计日志 ==========

    /**
     * @brief 记录审计日志
     */
    void logAuditEvent(const QString& userId,
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

    /**
     * @brief 获取最近的审计日志
     */
    QList<AuditLogEntry> getRecentAuditLog(int count = 100);

    // ========== 工具方法 ==========

    /**
     * @brief 角色转字符串
     */
    static QString roleToString(UserRole role);

    /**
     * @brief 字符串转角色
     */
    static UserRole stringToRole(const QString& str);

    /**
     * @brief 权限转字符串
     */
    static QString permissionToString(Permission permission);

signals:
    /**
     * @brief 用户添加
     */
    void userAdded(const QString& userId, const QString& username);

    /**
     * @brief 用户移除
     */
    void userRemoved(const QString& userId);

    /**
     * @brief 角色改变
     */
    void roleChanged(const QString& userId, UserRole newRole);

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
     * @brief 用户被锁定
     */
    void userLocked(const QString& userId, int minutes);

private:
    /**
     * @brief 初始化数据库
     */
    bool initializeDatabase();

    /**
     * @brief 创建用户表
     */
    bool createUserTable();

    /**
     * @brief 创建审计日志表
     */
    bool createAuditLogTable();

    /**
     * @brief 加载用户数据
     */
    bool loadUsers();

    /**
     * @brief 保存用户到数据库
     */
    bool saveUser(const UserInfo& user);

    /**
     * @brief 哈希密码
     */
    QString hashPassword(const QString& password, const QString& salt = QString()) const;

    /**
     * @brief 验证密码
     */
    bool verifyPassword(const QString& password, const QString& hash) const;

    /**
     * @brief 生成盐值
     */
    QString generateSalt() const;

    /**
     * @brief 生成用户ID
     */
    QString generateUserId() const;

    /**
     * @brief 检查用户是否被锁定
     */
    bool isUserLocked(const QString& userId);

    /**
     * @brief 处理登录失败
     */
    void handleLoginFailure(const QString& username);

    /**
     * @brief 重置登录尝试次数
     */
    void resetLoginAttempts(const QString& userId);

    /**
     * @brief 锁定用户
     */
    void lockUser(const QString& userId, int minutes);

private:
    RBACConfig config_;                             // 配置
    QSqlDatabase db_;                               // 数据库连接
    QString connectionName_;                        // 数据库连接名称
    QMap<QString, UserInfo> users_;                 // userId -> UserInfo
    QMap<QString, QString> usernameToId_;           // username -> userId
    mutable QMutex mutex_;                          // 线程安全锁
};

} // namespace Remote
} // namespace VisionForge
