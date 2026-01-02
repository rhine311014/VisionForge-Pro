/**
 * @file PermissionManager.h
 * @brief 权限管理器 - 用户角色与权限控制
 * @author VisionForge Team
 * @date 2025-12-18
 *
 * @details
 * 本文件实现了VisionForge项目的用户权限管理系统，提供基于角色的访问控制(RBAC)。
 *
 * ## 设计模式
 * - **单例模式 (Singleton)**：全局唯一的权限管理器实例
 * - **观察者模式 (Observer)**：登录/登出/权限变更通过信号通知
 * - **策略模式 (Strategy)**：不同角色有不同的权限集合
 *
 * ## 角色层次
 * @code
 * Administrator (管理员)
 *     ↑ 继承
 * Engineer (工程师)
 *     ↑ 继承
 * Operator (操作员)
 *     ↑ 继承
 * Guest (访客)
 * @endcode
 *
 * ## 权限分类
 * - 项目操作：新建、打开、保存、关闭
 * - 工具操作：添加、删除、编辑、排序
 * - 运行操作：启动、停止、单步
 * - 相机操作：连接、断开、设置
 * - 通信操作：通讯设置、PLC控制
 * - 系统操作：系统设置、用户管理、日志
 * - 数据操作：导入、导出、统计
 *
 * ## 安全机制
 *
 * ### 密码存储
 * - 使用SHA-256哈希存储密码
 * - 固定盐值防止彩虹表攻击
 * - 生产环境建议使用随机盐值
 *
 * ### 用户数据
 * - 用户信息存储在JSON文件
 * - 支持禁用用户账户
 * - 记录最后登录时间
 *
 * ## 线程安全
 * - 单例访问线程安全（C++11保证）
 * - 用户数据操作建议在主线程进行
 * - Qt信号可跨线程发送
 *
 * ## 内存管理
 * - 当前用户使用unique_ptr管理
 * - 析构时自动保存配置
 *
 * ## 使用示例
 * @code
 * // 用户登录
 * if (PERMISSION_MANAGER.login("admin", "admin123")) {
 *     LOG_INFO("登录成功");
 * }
 *
 * // 检查权限
 * if (HAS_PERMISSION(ToolEdit)) {
 *     // 允许编辑工具
 * } else {
 *     QMessageBox::warning(this, "权限不足", "您没有编辑工具的权限");
 * }
 *
 * // 监听登录状态
 * connect(&PERMISSION_MANAGER, &PermissionManager::userLoggedIn,
 *         this, &MainWindow::onUserLoggedIn);
 * @endcode
 */

#pragma once

#include <QObject>
#include <QString>
#include <QStringList>
#include <QMap>
#include <QSet>
#include <QCryptographicHash>
#include <QCoreApplication>
#include <memory>

namespace VisionForge {
namespace Base {

/**
 * @brief 用户角色枚举
 */
enum class UserRole {
    Guest,          // 访客 - 只能查看
    Operator,       // 操作员 - 运行程序
    Engineer,       // 工程师 - 编辑程序
    Administrator   // 管理员 - 完全权限
};

/**
 * @brief 权限类型枚举
 */
enum class Permission {
    // 项目操作
    ProjectNew,             // 新建项目
    ProjectOpen,            // 打开项目
    ProjectSave,            // 保存项目
    ProjectClose,           // 关闭项目

    // 工具操作
    ToolAdd,                // 添加工具
    ToolDelete,             // 删除工具
    ToolEdit,               // 编辑工具参数
    ToolReorder,            // 调整工具顺序

    // 运行操作
    RunStart,               // 启动运行
    RunStop,                // 停止运行
    RunStep,                // 单步执行

    // 相机操作
    CameraConnect,          // 连接相机
    CameraDisconnect,       // 断开相机
    CameraSettings,         // 相机设置

    // 通信操作
    CommSettings,           // 通讯设置
    PLCControl,             // PLC控制

    // 系统操作
    SystemSettings,         // 系统设置
    UserManagement,         // 用户管理
    LogView,                // 查看日志
    LogExport,              // 导出日志

    // 数据操作
    DataExport,             // 导出数据
    DataImport,             // 导入数据
    StatisticsView,         // 查看统计
};

/**
 * @brief 用户信息结构
 */
struct UserInfo {
    QString username;           // 用户名
    QString displayName;        // 显示名称
    QString passwordHash;       // 密码哈希
    UserRole role;              // 用户角色
    bool enabled;               // 是否启用
    QString lastLogin;          // 最后登录时间
    QString createTime;         // 创建时间

    UserInfo() : role(UserRole::Guest), enabled(true) {}
};

/**
 * @class PermissionManager
 * @brief 权限管理器（单例）
 *
 * 功能：
 * - 用户角色管理
 * - 权限检查
 * - 用户认证
 */
class PermissionManager : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief 获取单例实例
     */
    static PermissionManager& instance();

    /**
     * @brief 初始化权限系统
     * @param configPath 配置文件路径（可选）
     */
    void initialize(const QString& configPath = QString());

    // ==================== 用户管理 ====================

    /**
     * @brief 用户登录
     * @param username 用户名
     * @param password 密码（明文）
     * @return true如果登录成功
     */
    bool login(const QString& username, const QString& password);

    /**
     * @brief 用户登出
     */
    void logout();

    /**
     * @brief 获取当前用户
     */
    const UserInfo* currentUser() const { return currentUser_.get(); }

    /**
     * @brief 检查是否已登录
     */
    bool isLoggedIn() const { return currentUser_ != nullptr; }

    /**
     * @brief 添加用户
     * @param user 用户信息
     * @param password 初始密码（明文）
     * @return true如果成功
     */
    bool addUser(const UserInfo& user, const QString& password);

    /**
     * @brief 删除用户
     * @param username 用户名
     * @return true如果成功
     */
    bool removeUser(const QString& username);

    /**
     * @brief 修改用户密码
     * @param username 用户名
     * @param oldPassword 旧密码
     * @param newPassword 新密码
     * @return true如果成功
     */
    bool changePassword(const QString& username,
                       const QString& oldPassword,
                       const QString& newPassword);

    /**
     * @brief 获取所有用户列表
     */
    QList<UserInfo> getAllUsers() const;

    /**
     * @brief 获取用户信息
     * @param username 用户名
     */
    UserInfo* getUser(const QString& username);

    // ==================== 权限管理 ====================

    /**
     * @brief 检查当前用户是否有指定权限
     * @param permission 权限类型
     * @return true如果有权限
     */
    bool hasPermission(Permission permission) const;

    /**
     * @brief 检查指定角色是否有指定权限
     * @param role 角色
     * @param permission 权限类型
     * @return true如果有权限
     */
    bool roleHasPermission(UserRole role, Permission permission) const;

    /**
     * @brief 获取当前用户角色
     */
    UserRole currentRole() const;

    /**
     * @brief 获取角色名称
     */
    static QString roleName(UserRole role);

    /**
     * @brief 获取权限名称
     */
    static QString permissionName(Permission permission);

    // ==================== 配置管理 ====================

    /**
     * @brief 保存配置到文件
     */
    bool saveConfig();

    /**
     * @brief 从文件加载配置
     */
    bool loadConfig();

signals:
    /**
     * @brief 用户登录信号
     */
    void userLoggedIn(const QString& username);

    /**
     * @brief 用户登出信号
     */
    void userLoggedOut();

    /**
     * @brief 权限变更信号
     */
    void permissionChanged();

private:
    PermissionManager();
    ~PermissionManager();
    PermissionManager(const PermissionManager&) = delete;
    PermissionManager& operator=(const PermissionManager&) = delete;

    /**
     * @brief 初始化角色权限映射
     */
    void initializeRolePermissions();

    /**
     * @brief 创建默认用户
     */
    void createDefaultUsers();

    /**
     * @brief 计算密码哈希
     */
    static QString hashPassword(const QString& password, const QString& salt = QString());

    /**
     * @brief 验证密码
     */
    bool verifyPassword(const QString& password, const QString& hash) const;

private:
    QString configPath_;                                    // 配置文件路径
    std::unique_ptr<UserInfo> currentUser_;                 // 当前登录用户
    QMap<QString, UserInfo> users_;                         // 用户数据库
    QMap<UserRole, QSet<Permission>> rolePermissions_;      // 角色权限映射
    bool initialized_;
};

} // namespace Base
} // namespace VisionForge

// 便捷宏
#define PERMISSION_MANAGER VisionForge::Base::PermissionManager::instance()
#define HAS_PERMISSION(p) VisionForge::Base::PermissionManager::instance().hasPermission(VisionForge::Base::Permission::p)
