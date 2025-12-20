/**
 * @file test_RBACManager_gtest.cpp
 * @brief RBACManager 单元测试 (简化版)
 *
 * 测试覆盖:
 * - 用户管理 (添加、删除)
 * - 用户认证
 * - 权限检查
 * - 审计日志
 */

#include <gtest/gtest.h>
#include <QCoreApplication>
#include <QTest>
#include <QSignalSpy>
#include <QTemporaryDir>
#include <QDebug>
#include <QSqlDatabase>
#include "remote/RBACManager.h"

using namespace VisionForge::Remote;

class RBACManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
        tempDir = new QTemporaryDir();
        ASSERT_TRUE(tempDir->isValid());
        qDebug() << "临时目录:" << tempDir->path();

        // 检查SQLite驱动
        QStringList drivers = QSqlDatabase::drivers();
        qDebug() << "可用SQL驱动:" << drivers;
        ASSERT_TRUE(drivers.contains("QSQLITE")) << "QSQLITE驱动不可用";

        rbacManager = new RBACManager();

        RBACConfig config;
        config.databasePath = tempDir->path() + "/test_rbac.db";
        config.enableAuditLog = true;
        config.maxLoginAttempts = 5;
        config.lockoutDurationMinutes = 15;

        qDebug() << "初始化RBAC管理器，数据库路径:" << config.databasePath;
        bool initResult = rbacManager->initialize(config);
        if (!initResult) {
            qCritical() << "RBACManager初始化失败!";
        }
        ASSERT_TRUE(initResult);

        // 添加测试用户（注意：initialize已自动创建admin用户）
        ASSERT_TRUE(rbacManager->addUser("operator", "oper123", UserRole::Operator));
        ASSERT_TRUE(rbacManager->addUser("viewer", "view123", UserRole::Viewer));
    }

    void TearDown() override {
        delete rbacManager;
        rbacManager = nullptr;
        delete tempDir;
        tempDir = nullptr;
    }

    RBACManager* rbacManager = nullptr;
    QTemporaryDir* tempDir = nullptr;
};

/**
 * 测试1: 添加用户
 */
TEST_F(RBACManagerTest, AddUser_Success) {
    bool result = rbacManager->addUser("testuser", "password123", UserRole::Viewer);

    EXPECT_TRUE(result);
    EXPECT_TRUE(rbacManager->userExists("testuser"));

    qDebug() << "添加用户成功";
}

/**
 * 测试2: 添加重复用户
 */
TEST_F(RBACManagerTest, AddUser_Duplicate) {
    bool result = rbacManager->addUser("admin", "newpassword", UserRole::Viewer);

    EXPECT_FALSE(result);

    qDebug() << "重复用户添加失败(预期)";
}

/**
 * 测试3: 用户认证 - 成功
 */
TEST_F(RBACManagerTest, Authenticate_Success) {
    AuthResult result = rbacManager->authenticate("admin", "admin123");

    EXPECT_TRUE(result.success);
    EXPECT_FALSE(result.userId.isEmpty());
    EXPECT_EQ(result.username, "admin");
    EXPECT_EQ(result.role, UserRole::Admin);
    EXPECT_TRUE(result.errorMessage.isEmpty());

    qDebug() << "用户认证成功 - UserID:" << result.userId;
}

/**
 * 测试4: 用户认证 - 密码错误
 */
TEST_F(RBACManagerTest, Authenticate_WrongPassword) {
    AuthResult result = rbacManager->authenticate("admin", "wrongpassword");

    EXPECT_FALSE(result.success);
    EXPECT_TRUE(result.userId.isEmpty());
    EXPECT_FALSE(result.errorMessage.isEmpty());

    qDebug() << "密码错误,认证失败(预期):" << result.errorMessage;
}

/**
 * 测试5: 用户认证 - 用户不存在
 */
TEST_F(RBACManagerTest, Authenticate_UserNotFound) {
    AuthResult result = rbacManager->authenticate("nonexistent", "password");

    EXPECT_FALSE(result.success);

    qDebug() << "用户不存在,认证失败(预期):" << result.errorMessage;
}

/**
 * 测试6: 权限检查 - Admin角色
 */
TEST_F(RBACManagerTest, HasPermission_Admin) {
    AuthResult authResult = rbacManager->authenticate("admin", "admin123");
    ASSERT_TRUE(authResult.success);

    QString userId = authResult.userId;

    EXPECT_TRUE(rbacManager->hasPermission(userId, Permission::ViewPerformanceMetrics));
    EXPECT_TRUE(rbacManager->hasPermission(userId, Permission::ViewLogs));
    EXPECT_TRUE(rbacManager->hasPermission(userId, Permission::ModifyConfiguration));
    EXPECT_TRUE(rbacManager->hasPermission(userId, Permission::ManageUsers));

    qDebug() << "Admin角色拥有所有权限";
}

/**
 * 测试7: 权限检查 - Viewer角色
 */
TEST_F(RBACManagerTest, HasPermission_Viewer) {
    AuthResult authResult = rbacManager->authenticate("viewer", "view123");
    ASSERT_TRUE(authResult.success);

    QString userId = authResult.userId;

    EXPECT_TRUE(rbacManager->hasPermission(userId, Permission::ViewPerformanceMetrics));
    EXPECT_TRUE(rbacManager->hasPermission(userId, Permission::ViewLogs));

    EXPECT_FALSE(rbacManager->hasPermission(userId, Permission::ModifyConfiguration));
    EXPECT_FALSE(rbacManager->hasPermission(userId, Permission::ManageUsers));

    qDebug() << "Viewer角色只有查看权限";
}

/**
 * 测试8: 更新用户密码
 */
TEST_F(RBACManagerTest, UpdateUserPassword) {
    AuthResult authResult = rbacManager->authenticate("admin", "admin123");
    ASSERT_TRUE(authResult.success);

    QString userId = authResult.userId;

    bool result = rbacManager->updateUserPassword(userId, "newpassword123");
    EXPECT_TRUE(result);

    AuthResult oldPassResult = rbacManager->authenticate("admin", "admin123");
    EXPECT_FALSE(oldPassResult.success);

    AuthResult newPassResult = rbacManager->authenticate("admin", "newpassword123");
    EXPECT_TRUE(newPassResult.success);

    qDebug() << "密码更新成功";
}

/**
 * 测试9: 更新用户角色
 */
TEST_F(RBACManagerTest, UpdateUserRole) {
    AuthResult authResult = rbacManager->authenticate("viewer", "view123");
    ASSERT_TRUE(authResult.success);

    QString userId = authResult.userId;
    EXPECT_EQ(authResult.role, UserRole::Viewer);

    bool result = rbacManager->updateUserRole(userId, UserRole::Operator);
    EXPECT_TRUE(result);

    AuthResult newAuthResult = rbacManager->authenticate("viewer", "view123");
    EXPECT_TRUE(newAuthResult.success);
    EXPECT_EQ(newAuthResult.role, UserRole::Operator);

    qDebug() << "用户角色更新成功: Viewer -> Operator";
}

/**
 * 测试10: 删除用户
 */
TEST_F(RBACManagerTest, RemoveUser) {
    AuthResult authResult = rbacManager->authenticate("viewer", "view123");
    ASSERT_TRUE(authResult.success);

    QString userId = authResult.userId;

    bool result = rbacManager->removeUser(userId);
    EXPECT_TRUE(result);

    AuthResult deletedResult = rbacManager->authenticate("viewer", "view123");
    EXPECT_FALSE(deletedResult.success);

    qDebug() << "用户删除成功";
}

/**
 * 测试11: 获取用户信息
 */
TEST_F(RBACManagerTest, GetUserInfo) {
    AuthResult authResult = rbacManager->authenticate("admin", "admin123");
    ASSERT_TRUE(authResult.success);

    QString userId = authResult.userId;

    UserInfo userInfo = rbacManager->getUserInfo(userId);

    EXPECT_EQ(userInfo.userId, userId);
    EXPECT_EQ(userInfo.username, "admin");
    EXPECT_EQ(userInfo.role, UserRole::Admin);

    qDebug() << "用户信息 - ID:" << userInfo.userId
             << "Username:" << userInfo.username;
}

/**
 * 测试12: 获取所有用户列表
 */
TEST_F(RBACManagerTest, GetAllUsers) {
    QList<UserInfo> users = rbacManager->getAllUsers();

    EXPECT_GE(users.size(), 3);

    for (const auto& user : users) {
        EXPECT_FALSE(user.userId.isEmpty());
        EXPECT_FALSE(user.username.isEmpty());
        qDebug() << "用户:" << user.username;
    }
}

/**
 * 测试13: 检查用户是否存在
 */
TEST_F(RBACManagerTest, UserExists) {
    EXPECT_TRUE(rbacManager->userExists("admin"));
    EXPECT_TRUE(rbacManager->userExists("operator"));
    EXPECT_TRUE(rbacManager->userExists("viewer"));
    EXPECT_FALSE(rbacManager->userExists("nonexistent"));

    qDebug() << "用户存在性检查成功";
}

/**
 * 测试14: 审计日志记录
 */
TEST_F(RBACManagerTest, AuditLog) {
    AuthResult authResult = rbacManager->authenticate("admin", "admin123");
    ASSERT_TRUE(authResult.success);

    QString userId = authResult.userId;

    rbacManager->logAuditEvent(
        userId,
        "TEST_ACTION",
        "test_resource",
        true,
        "192.168.1.100",
        "执行单元测试"
    );

    QList<AuditLogEntry> logs = rbacManager->getRecentAuditLog(100);

    EXPECT_GT(logs.size(), 0);

    qDebug() << "审计日志数量:" << logs.size();
}

/**
 * 测试15: 信号发射 - 用户添加
 */
TEST_F(RBACManagerTest, Signal_UserAdded) {
    QSignalSpy spy(rbacManager, &RBACManager::userAdded);

    rbacManager->addUser("signaltest", "password123", UserRole::Viewer);

    EXPECT_EQ(spy.count(), 1);

    QList<QVariant> arguments = spy.takeFirst();
    EXPECT_FALSE(arguments.at(0).toString().isEmpty());
    EXPECT_EQ(arguments.at(1).toString(), "signaltest");

    qDebug() << "userAdded信号已发射";
}

/**
 * Main函数
 */
int main(int argc, char** argv) {
    QCoreApplication app(argc, argv);

    // 添加Qt插件路径以加载SQLite驱动
    QCoreApplication::addLibraryPath("F:/Qt/6.9.3/msvc2022_64/plugins");

    qDebug() << "Qt插件路径:" << QCoreApplication::libraryPaths();
    qDebug() << "可用SQL驱动:" << QSqlDatabase::drivers();

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
