/**
 * @file RBACManager.cpp
 * @brief RBAC权限管理器实现
 * @author VisionForge Team
 * @date 2025-12-20
 */

#include "remote/RBACManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QCryptographicHash>
#include <QUuid>
#include <QDebug>
#include <QMutexLocker>
#include <QVariant>

namespace VisionForge {
namespace Remote {

RBACManager::RBACManager(QObject* parent)
    : QObject(parent)
{
}

RBACManager::~RBACManager()
{
    close();
}

bool RBACManager::initialize(const RBACConfig& config)
{
    QMutexLocker locker(&mutex_);
    config_ = config;

    qInfo() << "RBAC管理器初始化";
    qInfo() << "  数据库路径:" << config_.databasePath;
    qInfo() << "  默认角色:" << roleToString(config_.defaultRole);
    qInfo() << "  启用访客:" << config_.enableGuestAccess;

    // 初始化数据库
    if (!initializeDatabase()) {
        return false;
    }

    // 加载用户数据
    if (!loadUsers()) {
        qWarning() << "加载用户数据失败";
    }

    // 检查是否有管理员用户，如果没有则创建默认管理员
    bool hasAdmin = false;
    for (const auto& user : users_) {
        if (user.role == UserRole::Admin && user.isActive) {
            hasAdmin = true;
            break;
        }
    }

    if (!hasAdmin) {
        qWarning() << "没有活跃管理员，创建默认管理员";
        locker.unlock();  // 解锁以调用addUser
        addUser("admin", "admin123", UserRole::Admin, "", "默认管理员");
        locker.relock();
    }

    qInfo() << "RBAC管理器初始化完成，共" << users_.size() << "个用户";
    return true;
}

void RBACManager::close()
{
    QMutexLocker locker(&mutex_);

    if (db_.isOpen()) {
        db_.close();
        qInfo() << "RBAC数据库已关闭";
    }
}

bool RBACManager::addUser(const QString& username,
                         const QString& password,
                         UserRole role,
                         const QString& email,
                         const QString& description)
{
    QMutexLocker locker(&mutex_);

    // 检查用户名是否已存在
    if (usernameToId_.contains(username)) {
        qWarning() << "用户名已存在:" << username;
        return false;
    }

    // 生成用户ID
    QString userId = generateUserId();

    // 哈希密码
    QString salt = generateSalt();
    QString passwordHash = hashPassword(password, salt);

    // 创建用户信息
    UserInfo user;
    user.userId = userId;
    user.username = username;
    user.passwordHash = passwordHash;
    user.role = role;
    user.createdAt = QDateTime::currentDateTime();
    user.lastLoginAt = QDateTime();
    user.isActive = true;
    user.email = email;
    user.description = description;
    user.loginAttempts = 0;
    user.lockedUntil = QDateTime();

    // 保存到数据库
    if (!saveUser(user)) {
        qCritical() << "保存用户到数据库失败:" << username;
        return false;
    }

    // 添加到内存
    users_[userId] = user;
    usernameToId_[username] = userId;

    qInfo() << "用户已添加:" << username << "角色:" << roleToString(role);
    emit userAdded(userId, username);

    return true;
}

bool RBACManager::removeUser(const QString& userId)
{
    QMutexLocker locker(&mutex_);

    if (!users_.contains(userId)) {
        qWarning() << "用户不存在:" << userId;
        return false;
    }

    QString username = users_[userId].username;

    // 从数据库删除
    QSqlQuery query(db_);
    query.prepare("DELETE FROM users WHERE user_id = ?");
    query.addBindValue(userId);

    if (!query.exec()) {
        qCritical() << "删除用户失败:" << query.lastError().text();
        return false;
    }

    // 从内存删除
    usernameToId_.remove(username);
    users_.remove(userId);

    qInfo() << "用户已删除:" << username;
    emit userRemoved(userId);

    return true;
}

bool RBACManager::updateUserRole(const QString& userId, UserRole newRole)
{
    QMutexLocker locker(&mutex_);

    if (!users_.contains(userId)) {
        qWarning() << "用户不存在:" << userId;
        return false;
    }

    // 更新数据库
    QSqlQuery query(db_);
    query.prepare("UPDATE users SET role = ? WHERE user_id = ?");
    query.addBindValue(static_cast<int>(newRole));
    query.addBindValue(userId);

    if (!query.exec()) {
        qCritical() << "更新用户角色失败:" << query.lastError().text();
        return false;
    }

    // 更新内存
    users_[userId].role = newRole;

    qInfo() << "用户角色已更新:" << users_[userId].username
            << "新角色:" << roleToString(newRole);
    emit roleChanged(userId, newRole);

    return true;
}

bool RBACManager::updateUserPassword(const QString& userId, const QString& newPassword)
{
    QMutexLocker locker(&mutex_);

    if (!users_.contains(userId)) {
        qWarning() << "用户不存在:" << userId;
        return false;
    }

    // 生成新的密码哈希
    QString salt = generateSalt();
    QString passwordHash = hashPassword(newPassword, salt);

    // 更新数据库
    QSqlQuery query(db_);
    query.prepare("UPDATE users SET password_hash = ? WHERE user_id = ?");
    query.addBindValue(passwordHash);
    query.addBindValue(userId);

    if (!query.exec()) {
        qCritical() << "更新密码失败:" << query.lastError().text();
        return false;
    }

    // 更新内存
    users_[userId].passwordHash = passwordHash;

    qInfo() << "用户密码已更新:" << users_[userId].username;
    return true;
}

bool RBACManager::setUserActive(const QString& userId, bool active)
{
    QMutexLocker locker(&mutex_);

    if (!users_.contains(userId)) {
        qWarning() << "用户不存在:" << userId;
        return false;
    }

    // 更新数据库
    QSqlQuery query(db_);
    query.prepare("UPDATE users SET is_active = ? WHERE user_id = ?");
    query.addBindValue(active);
    query.addBindValue(userId);

    if (!query.exec()) {
        qCritical() << "更新用户状态失败:" << query.lastError().text();
        return false;
    }

    // 更新内存
    users_[userId].isActive = active;

    qInfo() << "用户状态已更新:" << users_[userId].username << (active ? "激活" : "停用");
    return true;
}

UserInfo RBACManager::getUserInfo(const QString& userId) const
{
    QMutexLocker locker(&mutex_);

    if (users_.contains(userId)) {
        return users_[userId];
    }

    return UserInfo();
}

QList<UserInfo> RBACManager::getAllUsers() const
{
    QMutexLocker locker(&mutex_);
    return users_.values();
}

bool RBACManager::userExists(const QString& username) const
{
    QMutexLocker locker(&mutex_);
    return usernameToId_.contains(username);
}

AuthResult RBACManager::authenticate(const QString& username,
                                    const QString& password,
                                    const QString& ipAddress)
{
    QMutexLocker locker(&mutex_);

    AuthResult result;
    result.success = false;
    result.remainingAttempts = config_.maxLoginAttempts;

    // 检查用户是否存在
    if (!usernameToId_.contains(username)) {
        result.errorMessage = "用户名或密码错误";
        locker.unlock();
        logAuditEvent("", "LOGIN_FAILED", username, false, ipAddress, "用户不存在");
        emit authenticationFailed(username, result.errorMessage);
        return result;
    }

    QString userId = usernameToId_[username];
    UserInfo& user = users_[userId];

    // 检查用户是否激活
    if (!user.isActive) {
        result.errorMessage = "用户已被停用";
        locker.unlock();
        logAuditEvent(userId, "LOGIN_FAILED", username, false, ipAddress, "用户已停用");
        emit authenticationFailed(username, result.errorMessage);
        return result;
    }

    // 检查用户是否被锁定
    if (isUserLocked(userId)) {
        int minutesRemaining = QDateTime::currentDateTime().secsTo(user.lockedUntil) / 60 + 1;
        result.errorMessage = QString("账户已锁定，请%1分钟后重试").arg(minutesRemaining);
        locker.unlock();
        logAuditEvent(userId, "LOGIN_FAILED", username, false, ipAddress, "账户被锁定");
        emit authenticationFailed(username, result.errorMessage);
        return result;
    }

    // 验证密码
    if (!verifyPassword(password, user.passwordHash)) {
        // 登录失败，增加尝试次数
        user.loginAttempts++;
        result.remainingAttempts = config_.maxLoginAttempts - user.loginAttempts;

        // 更新数据库
        QSqlQuery query(db_);
        query.prepare("UPDATE users SET login_attempts = ? WHERE user_id = ?");
        query.addBindValue(user.loginAttempts);
        query.addBindValue(userId);
        query.exec();

        // 检查是否需要锁定账户
        if (user.loginAttempts >= config_.maxLoginAttempts) {
            lockUser(userId, config_.lockoutDurationMinutes);
            result.errorMessage = QString("登录失败次数过多，账户已锁定%1分钟")
                                .arg(config_.lockoutDurationMinutes);
            result.remainingAttempts = 0;
        } else {
            result.errorMessage = QString("用户名或密码错误，剩余尝试次数: %1")
                                .arg(result.remainingAttempts);
        }

        locker.unlock();
        logAuditEvent(userId, "LOGIN_FAILED", username, false, ipAddress, result.errorMessage);
        emit authenticationFailed(username, result.errorMessage);
        return result;
    }

    // 认证成功
    resetLoginAttempts(userId);

    // 更新最后登录时间
    user.lastLoginAt = QDateTime::currentDateTime();
    QSqlQuery query(db_);
    query.prepare("UPDATE users SET last_login_at = ? WHERE user_id = ?");
    query.addBindValue(user.lastLoginAt);
    query.addBindValue(userId);
    query.exec();

    result.success = true;
    result.userId = userId;
    result.username = username;
    result.role = user.role;

    locker.unlock();
    logAuditEvent(userId, "LOGIN_SUCCESS", username, true, ipAddress);
    emit authenticationSucceeded(userId, username);

    qInfo() << "用户认证成功:" << username << "角色:" << roleToString(user.role);

    return result;
}

void RBACManager::logout(const QString& userId, const QString& ipAddress)
{
    logAuditEvent(userId, "LOGOUT", "", true, ipAddress);
}

bool RBACManager::hasPermission(const QString& userId, Permission permission) const
{
    QMutexLocker locker(&mutex_);

    if (!users_.contains(userId)) {
        return false;
    }

    UserRole role = users_[userId].role;
    QSet<Permission> permissions = getRolePermissions(role);

    return permissions.contains(permission);
}

bool RBACManager::checkPermission(const QString& userId,
                                 Permission permission,
                                 const QString& resource)
{
    bool hasAccess = hasPermission(userId, permission);

    if (!hasAccess) {
        logAuditEvent(userId, "PERMISSION_DENIED",
                     resource.isEmpty() ? permissionToString(permission) : resource,
                     false);
        emit permissionDenied(userId, permission);
    }

    return hasAccess;
}

QMap<Permission, bool> RBACManager::checkPermissions(const QString& userId,
                                                     const QList<Permission>& permissions)
{
    QMap<Permission, bool> results;

    for (Permission perm : permissions) {
        results[perm] = hasPermission(userId, perm);
    }

    return results;
}

UserRole RBACManager::getUserRole(const QString& userId) const
{
    QMutexLocker locker(&mutex_);

    if (users_.contains(userId)) {
        return users_[userId].role;
    }

    return UserRole::Guest;
}

QSet<Permission> RBACManager::getUserPermissions(const QString& userId) const
{
    UserRole role = getUserRole(userId);
    return getRolePermissions(role);
}

QSet<Permission> RBACManager::getRolePermissions(UserRole role)
{
    QSet<Permission> permissions;

    switch (role) {
        case UserRole::Admin:
            // 管理员：所有权限
            permissions << Permission::ViewPerformanceMetrics
                       << Permission::ExportPerformanceData
                       << Permission::ViewLogs
                       << Permission::ExportLogs
                       << Permission::ClearLogs
                       << Permission::ViewConfiguration
                       << Permission::ModifyConfiguration
                       << Permission::StartStopServices
                       << Permission::RestartSystem
                       << Permission::ViewUsers
                       << Permission::ManageUsers
                       << Permission::ManageRoles
                       << Permission::ViewDatabase
                       << Permission::ModifyDatabase
                       << Permission::ExportDatabase
                       << Permission::CleanupDatabase;
            break;

        case UserRole::Operator:
            // 操作员：读+写（不包括配置和用户管理）
            permissions << Permission::ViewPerformanceMetrics
                       << Permission::ExportPerformanceData
                       << Permission::ViewLogs
                       << Permission::ExportLogs
                       << Permission::ViewConfiguration
                       << Permission::StartStopServices
                       << Permission::ViewDatabase
                       << Permission::ExportDatabase;
            break;

        case UserRole::Viewer:
            // 查看者：只读
            permissions << Permission::ViewPerformanceMetrics
                       << Permission::ViewLogs
                       << Permission::ViewConfiguration
                       << Permission::ViewDatabase;
            break;

        case UserRole::Guest:
            // 访客：受限只读
            permissions << Permission::ViewPerformanceMetrics
                       << Permission::ViewLogs;
            break;
    }

    return permissions;
}

void RBACManager::logAuditEvent(const QString& userId,
                               const QString& action,
                               const QString& resource,
                               bool success,
                               const QString& ipAddress,
                               const QString& details)
{
    if (!config_.enableAuditLog) {
        return;
    }

    QMutexLocker locker(&mutex_);

    QSqlQuery query(db_);
    query.prepare(R"(
        INSERT INTO security_audit_log
        (timestamp, user_id, action, resource, result, ip_address, details)
        VALUES (?, ?, ?, ?, ?, ?, ?)
    )");

    query.addBindValue(QDateTime::currentDateTime());
    query.addBindValue(userId);
    query.addBindValue(action);
    query.addBindValue(resource);
    query.addBindValue(success ? "SUCCESS" : "FAILED");
    query.addBindValue(ipAddress);
    query.addBindValue(details);

    if (!query.exec()) {
        qWarning() << "记录审计日志失败:" << query.lastError().text();
    }
}

QList<AuditLogEntry> RBACManager::queryAuditLog(const QDateTime& startTime,
                                               const QDateTime& endTime,
                                               const QString& userId,
                                               int limit)
{
    QMutexLocker locker(&mutex_);
    QList<AuditLogEntry> logs;

    QString sql = "SELECT * FROM security_audit_log WHERE timestamp >= ? AND timestamp <= ?";
    if (!userId.isEmpty()) {
        sql += " AND user_id = ?";
    }
    sql += " ORDER BY timestamp DESC LIMIT ?";

    QSqlQuery query(db_);
    query.prepare(sql);
    query.addBindValue(startTime);
    query.addBindValue(endTime);
    if (!userId.isEmpty()) {
        query.addBindValue(userId);
    }
    query.addBindValue(limit);

    if (!query.exec()) {
        qWarning() << "查询审计日志失败:" << query.lastError().text();
        return logs;
    }

    while (query.next()) {
        AuditLogEntry entry;
        entry.id = query.value("id").toLongLong();
        entry.timestamp = query.value("timestamp").toDateTime();
        entry.userId = query.value("user_id").toString();
        entry.action = query.value("action").toString();
        entry.resource = query.value("resource").toString();
        entry.result = query.value("result").toString();
        entry.ipAddress = query.value("ip_address").toString();
        entry.details = query.value("details").toString();
        logs.append(entry);
    }

    return logs;
}

QList<AuditLogEntry> RBACManager::getRecentAuditLog(int count)
{
    QMutexLocker locker(&mutex_);
    QList<AuditLogEntry> logs;

    QSqlQuery query(db_);
    query.prepare("SELECT * FROM security_audit_log ORDER BY timestamp DESC LIMIT ?");
    query.addBindValue(count);

    if (!query.exec()) {
        qWarning() << "查询审计日志失败:" << query.lastError().text();
        return logs;
    }

    while (query.next()) {
        AuditLogEntry entry;
        entry.id = query.value("id").toLongLong();
        entry.timestamp = query.value("timestamp").toDateTime();
        entry.userId = query.value("user_id").toString();
        entry.action = query.value("action").toString();
        entry.resource = query.value("resource").toString();
        entry.result = query.value("result").toString();
        entry.ipAddress = query.value("ip_address").toString();
        entry.details = query.value("details").toString();
        logs.append(entry);
    }

    return logs;
}

QString RBACManager::roleToString(UserRole role)
{
    switch (role) {
        case UserRole::Admin: return "Admin";
        case UserRole::Operator: return "Operator";
        case UserRole::Viewer: return "Viewer";
        case UserRole::Guest: return "Guest";
    }
    return "Unknown";
}

UserRole RBACManager::stringToRole(const QString& str)
{
    if (str == "Admin") return UserRole::Admin;
    if (str == "Operator") return UserRole::Operator;
    if (str == "Viewer") return UserRole::Viewer;
    if (str == "Guest") return UserRole::Guest;
    return UserRole::Guest;
}

QString RBACManager::permissionToString(Permission permission)
{
    switch (permission) {
        case Permission::ViewPerformanceMetrics: return "ViewPerformanceMetrics";
        case Permission::ExportPerformanceData: return "ExportPerformanceData";
        case Permission::ViewLogs: return "ViewLogs";
        case Permission::ExportLogs: return "ExportLogs";
        case Permission::ClearLogs: return "ClearLogs";
        case Permission::ViewConfiguration: return "ViewConfiguration";
        case Permission::ModifyConfiguration: return "ModifyConfiguration";
        case Permission::StartStopServices: return "StartStopServices";
        case Permission::RestartSystem: return "RestartSystem";
        case Permission::ViewUsers: return "ViewUsers";
        case Permission::ManageUsers: return "ManageUsers";
        case Permission::ManageRoles: return "ManageRoles";
        case Permission::ViewDatabase: return "ViewDatabase";
        case Permission::ModifyDatabase: return "ModifyDatabase";
        case Permission::ExportDatabase: return "ExportDatabase";
        case Permission::CleanupDatabase: return "CleanupDatabase";
    }
    return "Unknown";
}

bool RBACManager::initializeDatabase()
{
    // 使用唯一的连接名称
    connectionName_ = QString("RBACConnection_%1").arg(reinterpret_cast<quintptr>(this));

    db_ = QSqlDatabase::addDatabase("QSQLITE", connectionName_);
    db_.setDatabaseName(config_.databasePath);

    if (!db_.open()) {
        qCritical() << "无法打开RBAC数据库:" << db_.lastError().text();
        return false;
    }

    qInfo() << "RBAC数据库已打开:" << config_.databasePath;

    // 创建表
    if (!createUserTable() || !createAuditLogTable()) {
        return false;
    }

    return true;
}

bool RBACManager::createUserTable()
{
    QSqlQuery query(db_);

    QString sql = R"(
        CREATE TABLE IF NOT EXISTS users (
            user_id TEXT PRIMARY KEY,
            username TEXT UNIQUE NOT NULL,
            password_hash TEXT NOT NULL,
            role INTEGER NOT NULL,
            created_at DATETIME NOT NULL,
            last_login_at DATETIME,
            is_active BOOLEAN DEFAULT 1,
            email TEXT,
            description TEXT,
            login_attempts INTEGER DEFAULT 0,
            locked_until DATETIME
        )
    )";

    if (!query.exec(sql)) {
        qCritical() << "创建用户表失败:" << query.lastError().text();
        return false;
    }

    // 创建索引
    query.exec("CREATE INDEX IF NOT EXISTS idx_username ON users(username)");
    query.exec("CREATE INDEX IF NOT EXISTS idx_role ON users(role)");

    return true;
}

bool RBACManager::createAuditLogTable()
{
    QSqlQuery query(db_);

    QString sql = R"(
        CREATE TABLE IF NOT EXISTS security_audit_log (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            timestamp DATETIME NOT NULL,
            user_id TEXT,
            action TEXT NOT NULL,
            resource TEXT,
            result TEXT,
            ip_address TEXT,
            details TEXT
        )
    )";

    if (!query.exec(sql)) {
        qCritical() << "创建审计日志表失败:" << query.lastError().text();
        return false;
    }

    // 创建索引
    query.exec("CREATE INDEX IF NOT EXISTS idx_audit_timestamp ON security_audit_log(timestamp)");
    query.exec("CREATE INDEX IF NOT EXISTS idx_audit_user ON security_audit_log(user_id)");
    query.exec("CREATE INDEX IF NOT EXISTS idx_audit_action ON security_audit_log(action)");

    return true;
}

bool RBACManager::loadUsers()
{
    QSqlQuery query(db_);

    if (!query.exec("SELECT * FROM users")) {
        qWarning() << "加载用户失败:" << query.lastError().text();
        return false;
    }

    int count = 0;
    while (query.next()) {
        UserInfo user;
        user.userId = query.value("user_id").toString();
        user.username = query.value("username").toString();
        user.passwordHash = query.value("password_hash").toString();
        user.role = static_cast<UserRole>(query.value("role").toInt());
        user.createdAt = query.value("created_at").toDateTime();
        user.lastLoginAt = query.value("last_login_at").toDateTime();
        user.isActive = query.value("is_active").toBool();
        user.email = query.value("email").toString();
        user.description = query.value("description").toString();
        user.loginAttempts = query.value("login_attempts").toInt();
        user.lockedUntil = query.value("locked_until").toDateTime();

        users_[user.userId] = user;
        usernameToId_[user.username] = user.userId;
        count++;
    }

    qInfo() << "已加载" << count << "个用户";
    return true;
}

bool RBACManager::saveUser(const UserInfo& user)
{
    QSqlQuery query(db_);

    query.prepare(R"(
        INSERT OR REPLACE INTO users
        (user_id, username, password_hash, role, created_at, last_login_at,
         is_active, email, description, login_attempts, locked_until)
        VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
    )");

    query.addBindValue(user.userId);
    query.addBindValue(user.username);
    query.addBindValue(user.passwordHash);
    query.addBindValue(static_cast<int>(user.role));
    query.addBindValue(user.createdAt);
    query.addBindValue(user.lastLoginAt);
    query.addBindValue(user.isActive);
    query.addBindValue(user.email);
    query.addBindValue(user.description);
    query.addBindValue(user.loginAttempts);
    query.addBindValue(user.lockedUntil);

    if (!query.exec()) {
        qCritical() << "保存用户失败:" << query.lastError().text();
        return false;
    }

    return true;
}

QString RBACManager::hashPassword(const QString& password, const QString& salt) const
{
    QString saltedPassword = password + (salt.isEmpty() ? "" : salt);
    QByteArray hash = QCryptographicHash::hash(saltedPassword.toUtf8(), QCryptographicHash::Sha256);
    QString hashHex = hash.toHex();

    // 将盐值附加到哈希后面
    if (!salt.isEmpty()) {
        hashHex += ":" + salt;
    }

    return hashHex;
}

bool RBACManager::verifyPassword(const QString& password, const QString& hash) const
{
    // 提取盐值（如果有）
    QStringList parts = hash.split(':');
    QString storedHash = parts[0];
    QString salt = parts.size() > 1 ? parts[1] : "";

    // 计算输入密码的哈希
    QString inputHash = hashPassword(password, salt);

    return inputHash == hash;
}

QString RBACManager::generateSalt() const
{
    return QUuid::createUuid().toString(QUuid::WithoutBraces).left(16);
}

QString RBACManager::generateUserId() const
{
    return QUuid::createUuid().toString(QUuid::WithoutBraces);
}

bool RBACManager::isUserLocked(const QString& userId)
{
    if (!users_.contains(userId)) {
        return false;
    }

    QDateTime now = QDateTime::currentDateTime();
    QDateTime& lockedUntil = users_[userId].lockedUntil;

    if (!lockedUntil.isValid() || now >= lockedUntil) {
        // 锁定已过期，清除锁定状态
        if (lockedUntil.isValid()) {
            lockedUntil = QDateTime();
            users_[userId].loginAttempts = 0;

            QSqlQuery query(db_);
            query.prepare("UPDATE users SET locked_until = NULL, login_attempts = 0 WHERE user_id = ?");
            query.addBindValue(userId);
            query.exec();

            qInfo() << "用户锁定已解除:" << users_[userId].username;
        }
        return false;
    }

    return true;
}

void RBACManager::handleLoginFailure(const QString& username)
{
    Q_UNUSED(username);
    // 已在authenticate中处理
}

void RBACManager::resetLoginAttempts(const QString& userId)
{
    if (!users_.contains(userId)) {
        return;
    }

    users_[userId].loginAttempts = 0;
    users_[userId].lockedUntil = QDateTime();

    QSqlQuery query(db_);
    query.prepare("UPDATE users SET login_attempts = 0, locked_until = NULL WHERE user_id = ?");
    query.addBindValue(userId);
    query.exec();
}

void RBACManager::lockUser(const QString& userId, int minutes)
{
    if (!users_.contains(userId)) {
        return;
    }

    QDateTime lockedUntil = QDateTime::currentDateTime().addSecs(minutes * 60);
    users_[userId].lockedUntil = lockedUntil;

    QSqlQuery query(db_);
    query.prepare("UPDATE users SET locked_until = ? WHERE user_id = ?");
    query.addBindValue(lockedUntil);
    query.addBindValue(userId);
    query.exec();

    qWarning() << "用户已锁定:" << users_[userId].username << "锁定时长:" << minutes << "分钟";
    emit userLocked(userId, minutes);
}

} // namespace Remote
} // namespace VisionForge
