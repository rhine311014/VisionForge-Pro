/**
 * @file PermissionManager.cpp
 * @brief 权限管理器实现
 * @author VisionForge Team
 * @date 2025-12-18
 *
 * @details
 * 本文件实现PermissionManager类的所有成员函数。
 *
 * ## 实现细节
 *
 * ### 密码哈希算法
 * hashPassword()使用SHA-256：
 * hash = SHA256(password + salt)
 * 默认盐值："VisionForge_Salt_2025"
 *
 * ### 默认用户
 * 系统初始化时创建三个默认账户：
 * - admin / admin123 (管理员)
 * - engineer / eng123 (工程师)
 * - operator / op123 (操作员)
 *
 * ### 角色权限初始化
 * initializeRolePermissions()定义每个角色的权限集：
 * - Guest: 仅LogView
 * - Operator: 运行、查看相关权限
 * - Engineer: 编辑、配置相关权限
 * - Administrator: 所有权限
 *
 * ### 配置文件格式
 * JSON格式存储用户数据：
 * @code
 * {
 *   "version": "1.0",
 *   "users": [
 *     {
 *       "username": "admin",
 *       "displayName": "系统管理员",
 *       "passwordHash": "...",
 *       "role": 3,
 *       "enabled": true
 *     }
 *   ]
 * }
 * @endcode
 */

#include "base/PermissionManager.h"
#include "base/Logger.h"

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDateTime>
#include <QUuid>

namespace VisionForge {
namespace Base {

PermissionManager::PermissionManager()
    : QObject(nullptr)
    , currentUser_(nullptr)
    , initialized_(false)
{
}

PermissionManager::~PermissionManager()
{
    if (initialized_) {
        saveConfig();
    }
}

PermissionManager& PermissionManager::instance()
{
    static PermissionManager instance;
    return instance;
}

void PermissionManager::initialize(const QString& configPath)
{
    if (initialized_) return;

    // 设置配置文件路径
    if (configPath.isEmpty()) {
        configPath_ = QCoreApplication::applicationDirPath() + "/config/users.json";
    } else {
        configPath_ = configPath;
    }

    // 确保目录存在
    QFileInfo fileInfo(configPath_);
    QDir dir = fileInfo.dir();
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    // 初始化角色权限
    initializeRolePermissions();

    // 加载配置或创建默认用户
    if (!loadConfig()) {
        createDefaultUsers();
        saveConfig();
    }

    initialized_ = true;
    LOG_INFO(tr("权限管理器初始化完成"));
}

void PermissionManager::initializeRolePermissions()
{
    // 访客权限 - 只能查看
    rolePermissions_[UserRole::Guest] = {
        Permission::LogView
    };

    // 操作员权限 - 可以运行程序
    rolePermissions_[UserRole::Operator] = {
        Permission::ProjectOpen,
        Permission::RunStart,
        Permission::RunStop,
        Permission::RunStep,
        Permission::CameraConnect,
        Permission::CameraDisconnect,
        Permission::LogView,
        Permission::StatisticsView
    };

    // 工程师权限 - 可以编辑程序
    rolePermissions_[UserRole::Engineer] = {
        Permission::ProjectNew,
        Permission::ProjectOpen,
        Permission::ProjectSave,
        Permission::ProjectClose,
        Permission::ToolAdd,
        Permission::ToolDelete,
        Permission::ToolEdit,
        Permission::ToolReorder,
        Permission::RunStart,
        Permission::RunStop,
        Permission::RunStep,
        Permission::CameraConnect,
        Permission::CameraDisconnect,
        Permission::CameraSettings,
        Permission::CommSettings,
        Permission::PLCControl,
        Permission::LogView,
        Permission::LogExport,
        Permission::DataExport,
        Permission::DataImport,
        Permission::StatisticsView
    };

    // 管理员权限 - 完全权限
    rolePermissions_[UserRole::Administrator] = {
        Permission::ProjectNew,
        Permission::ProjectOpen,
        Permission::ProjectSave,
        Permission::ProjectClose,
        Permission::ToolAdd,
        Permission::ToolDelete,
        Permission::ToolEdit,
        Permission::ToolReorder,
        Permission::RunStart,
        Permission::RunStop,
        Permission::RunStep,
        Permission::CameraConnect,
        Permission::CameraDisconnect,
        Permission::CameraSettings,
        Permission::CommSettings,
        Permission::PLCControl,
        Permission::SystemSettings,
        Permission::UserManagement,
        Permission::LogView,
        Permission::LogExport,
        Permission::DataExport,
        Permission::DataImport,
        Permission::StatisticsView
    };
}

void PermissionManager::createDefaultUsers()
{
    // 创建管理员账户
    UserInfo admin;
    admin.username = "admin";
    admin.displayName = tr("系统管理员");
    admin.passwordHash = hashPassword("admin123");
    admin.role = UserRole::Administrator;
    admin.enabled = true;
    admin.createTime = QDateTime::currentDateTime().toString(Qt::ISODate);
    users_["admin"] = admin;

    // 创建工程师账户
    UserInfo engineer;
    engineer.username = "engineer";
    engineer.displayName = tr("工程师");
    engineer.passwordHash = hashPassword("eng123");
    engineer.role = UserRole::Engineer;
    engineer.enabled = true;
    engineer.createTime = QDateTime::currentDateTime().toString(Qt::ISODate);
    users_["engineer"] = engineer;

    // 创建操作员账户
    UserInfo op;
    op.username = "operator";
    op.displayName = tr("操作员");
    op.passwordHash = hashPassword("op123");
    op.role = UserRole::Operator;
    op.enabled = true;
    op.createTime = QDateTime::currentDateTime().toString(Qt::ISODate);
    users_["operator"] = op;

    LOG_INFO(tr("已创建默认用户账户"));
}

bool PermissionManager::login(const QString& username, const QString& password)
{
    if (!users_.contains(username)) {
        LOG_WARNING(tr("登录失败: 用户不存在 - %1").arg(username));
        return false;
    }

    UserInfo& user = users_[username];

    if (!user.enabled) {
        LOG_WARNING(tr("登录失败: 用户已禁用 - %1").arg(username));
        return false;
    }

    if (!verifyPassword(password, user.passwordHash)) {
        LOG_WARNING(tr("登录失败: 密码错误 - %1").arg(username));
        return false;
    }

    // 登录成功
    currentUser_ = std::make_unique<UserInfo>(user);
    user.lastLogin = QDateTime::currentDateTime().toString(Qt::ISODate);

    LOG_INFO(tr("用户登录成功: %1 (%2)").arg(username).arg(roleName(user.role)));

    emit userLoggedIn(username);
    emit permissionChanged();

    saveConfig();
    return true;
}

void PermissionManager::logout()
{
    if (currentUser_) {
        LOG_INFO(tr("用户登出: %1").arg(currentUser_->username));
        currentUser_.reset();
        emit userLoggedOut();
        emit permissionChanged();
    }
}

bool PermissionManager::addUser(const UserInfo& user, const QString& password)
{
    if (users_.contains(user.username)) {
        LOG_WARNING(tr("添加用户失败: 用户已存在 - %1").arg(user.username));
        return false;
    }

    UserInfo newUser = user;
    newUser.passwordHash = hashPassword(password);
    newUser.createTime = QDateTime::currentDateTime().toString(Qt::ISODate);

    users_[user.username] = newUser;
    saveConfig();

    LOG_INFO(tr("添加用户: %1").arg(user.username));
    return true;
}

bool PermissionManager::removeUser(const QString& username)
{
    if (username == "admin") {
        LOG_WARNING(tr("无法删除管理员账户"));
        return false;
    }

    if (!users_.contains(username)) {
        return false;
    }

    users_.remove(username);
    saveConfig();

    LOG_INFO(tr("删除用户: %1").arg(username));
    return true;
}

bool PermissionManager::changePassword(const QString& username,
                                        const QString& oldPassword,
                                        const QString& newPassword)
{
    if (!users_.contains(username)) {
        return false;
    }

    UserInfo& user = users_[username];

    if (!verifyPassword(oldPassword, user.passwordHash)) {
        LOG_WARNING(tr("修改密码失败: 原密码错误 - %1").arg(username));
        return false;
    }

    user.passwordHash = hashPassword(newPassword);
    saveConfig();

    LOG_INFO(tr("用户密码已修改: %1").arg(username));
    return true;
}

QList<UserInfo> PermissionManager::getAllUsers() const
{
    return users_.values();
}

UserInfo* PermissionManager::getUser(const QString& username)
{
    if (users_.contains(username)) {
        return &users_[username];
    }
    return nullptr;
}

bool PermissionManager::hasPermission(Permission permission) const
{
    if (!currentUser_) {
        // 未登录时使用访客权限
        return roleHasPermission(UserRole::Guest, permission);
    }
    return roleHasPermission(currentUser_->role, permission);
}

bool PermissionManager::roleHasPermission(UserRole role, Permission permission) const
{
    if (rolePermissions_.contains(role)) {
        return rolePermissions_[role].contains(permission);
    }
    return false;
}

UserRole PermissionManager::currentRole() const
{
    if (currentUser_) {
        return currentUser_->role;
    }
    return UserRole::Guest;
}

QString PermissionManager::roleName(UserRole role)
{
    switch (role) {
    case UserRole::Guest:           return tr("访客");
    case UserRole::Operator:        return tr("操作员");
    case UserRole::Engineer:        return tr("工程师");
    case UserRole::Administrator:   return tr("管理员");
    default:                        return tr("未知");
    }
}

QString PermissionManager::permissionName(Permission permission)
{
    switch (permission) {
    case Permission::ProjectNew:        return tr("新建项目");
    case Permission::ProjectOpen:       return tr("打开项目");
    case Permission::ProjectSave:       return tr("保存项目");
    case Permission::ProjectClose:      return tr("关闭项目");
    case Permission::ToolAdd:           return tr("添加工具");
    case Permission::ToolDelete:        return tr("删除工具");
    case Permission::ToolEdit:          return tr("编辑工具");
    case Permission::ToolReorder:       return tr("调整顺序");
    case Permission::RunStart:          return tr("启动运行");
    case Permission::RunStop:           return tr("停止运行");
    case Permission::RunStep:           return tr("单步执行");
    case Permission::CameraConnect:     return tr("连接相机");
    case Permission::CameraDisconnect:  return tr("断开相机");
    case Permission::CameraSettings:    return tr("相机设置");
    case Permission::CommSettings:      return tr("通讯设置");
    case Permission::PLCControl:        return tr("PLC控制");
    case Permission::SystemSettings:    return tr("系统设置");
    case Permission::UserManagement:    return tr("用户管理");
    case Permission::LogView:           return tr("查看日志");
    case Permission::LogExport:         return tr("导出日志");
    case Permission::DataExport:        return tr("导出数据");
    case Permission::DataImport:        return tr("导入数据");
    case Permission::StatisticsView:    return tr("查看统计");
    default:                            return tr("未知权限");
    }
}

bool PermissionManager::saveConfig()
{
    QJsonObject root;
    QJsonArray usersArray;

    for (const auto& user : users_) {
        QJsonObject userObj;
        userObj["username"] = user.username;
        userObj["displayName"] = user.displayName;
        userObj["passwordHash"] = user.passwordHash;
        userObj["role"] = static_cast<int>(user.role);
        userObj["enabled"] = user.enabled;
        userObj["lastLogin"] = user.lastLogin;
        userObj["createTime"] = user.createTime;
        usersArray.append(userObj);
    }

    root["users"] = usersArray;
    root["version"] = "1.0";

    QFile file(configPath_);
    if (!file.open(QIODevice::WriteOnly)) {
        LOG_ERROR(tr("无法保存用户配置: %1").arg(configPath_));
        return false;
    }

    QJsonDocument doc(root);
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();

    return true;
}

bool PermissionManager::loadConfig()
{
    QFile file(configPath_);
    if (!file.exists() || !file.open(QIODevice::ReadOnly)) {
        return false;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(data, &error);
    if (error.error != QJsonParseError::NoError) {
        LOG_ERROR(tr("解析用户配置失败: %1").arg(error.errorString()));
        return false;
    }

    QJsonObject root = doc.object();
    QJsonArray usersArray = root["users"].toArray();

    users_.clear();
    for (const auto& value : usersArray) {
        QJsonObject userObj = value.toObject();

        UserInfo user;
        user.username = userObj["username"].toString();
        user.displayName = userObj["displayName"].toString();
        user.passwordHash = userObj["passwordHash"].toString();
        user.role = static_cast<UserRole>(userObj["role"].toInt());
        user.enabled = userObj["enabled"].toBool(true);
        user.lastLogin = userObj["lastLogin"].toString();
        user.createTime = userObj["createTime"].toString();

        users_[user.username] = user;
    }

    LOG_INFO(tr("加载用户配置成功，共 %1 个用户").arg(users_.size()));
    return true;
}

QString PermissionManager::hashPassword(const QString& password, const QString& salt)
{
    QString data = password;
    if (!salt.isEmpty()) {
        data += salt;
    } else {
        // 使用固定盐值（生产环境应使用随机盐值）
        data += "VisionForge_Salt_2025";
    }

    QByteArray hash = QCryptographicHash::hash(data.toUtf8(), QCryptographicHash::Sha256);
    return QString(hash.toHex());
}

bool PermissionManager::verifyPassword(const QString& password, const QString& hash) const
{
    QString computed = hashPassword(password);
    return computed == hash;
}

} // namespace Base
} // namespace VisionForge
