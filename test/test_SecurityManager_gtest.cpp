/**
 * @file test_SecurityManager_gtest.cpp
 * @brief SecurityManager单元测试
 * @author VisionForge Team
 * @date 2025-12-20
 */

#include <gtest/gtest.h>
#include "remote/SecurityManager.h"
#include "remote/RBACManager.h"
#include <QCoreApplication>
#include <QTemporaryDir>
#include <QTest>

using namespace VisionForge::Remote;

class SecurityManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
        // 创建临时目录用于测试
        tempDir = new QTemporaryDir();
        ASSERT_TRUE(tempDir->isValid());

        // 创建SecurityManager
        securityManager = new SecurityManager();
    }

    void TearDown() override {
        if (securityManager) {
            securityManager->close();
            delete securityManager;
            securityManager = nullptr;
        }

        if (tempDir) {
            delete tempDir;
            tempDir = nullptr;
        }
    }

    SecurityManager* securityManager = nullptr;
    QTemporaryDir* tempDir = nullptr;
};

/**
 * @brief 测试初始化
 */
TEST_F(SecurityManagerTest, Initialize) {
    SecurityConfig config;
    config.enableSecurity = true;
    config.certificateConfig.enabled = false;  // 测试环境禁用SSL（无需OpenSSL）
    config.tokenConfig.secretKey = "test-secret-key-32-bytes-long!!";  // 确保密钥长度>=32
    config.rbacConfig.databasePath = tempDir->filePath("test_security.db");

    ASSERT_TRUE(securityManager->initialize(config));
    EXPECT_TRUE(securityManager->isEnabled());
    EXPECT_NE(securityManager->certificateManager(), nullptr);
    EXPECT_NE(securityManager->tokenAuthManager(), nullptr);
    EXPECT_NE(securityManager->rbacManager(), nullptr);
}

/**
 * @brief 测试用户认证 - 成功场景
 */
TEST_F(SecurityManagerTest, AuthenticateConnection_Success) {
    SecurityConfig config;
    config.enableSecurity = true;
    config.certificateConfig.enabled = false;  // 测试环境禁用SSL
    config.tokenConfig.secretKey = "test-secret-key-32-bytes-long!!";
    config.rbacConfig.databasePath = tempDir->filePath("test_security.db");

    ASSERT_TRUE(securityManager->initialize(config));

    // 添加测试用户
    ASSERT_TRUE(securityManager->addUser("testuser", "password123", UserRole::Viewer));

    // 认证
    auto result = securityManager->authenticateConnection("testuser", "password123", "127.0.0.1");

    EXPECT_TRUE(result.authenticated);
    EXPECT_FALSE(result.accessToken.isEmpty());
    EXPECT_FALSE(result.refreshToken.isEmpty());
    EXPECT_EQ(result.username, "testuser");
    EXPECT_EQ(result.role, UserRole::Viewer);
}

/**
 * @brief 测试用户认证 - 失败场景（错误密码）
 */
TEST_F(SecurityManagerTest, AuthenticateConnection_WrongPassword) {
    SecurityConfig config;
    config.enableSecurity = true;
    config.certificateConfig.enabled = false;  // 测试环境禁用SSL
    config.tokenConfig.secretKey = "test-secret-key-32-bytes-long!!";
    config.rbacConfig.databasePath = tempDir->filePath("test_security.db");

    ASSERT_TRUE(securityManager->initialize(config));
    ASSERT_TRUE(securityManager->addUser("testuser", "password123", UserRole::Viewer));

    // 使用错误密码认证
    auto result = securityManager->authenticateConnection("testuser", "wrongpassword", "127.0.0.1");

    EXPECT_FALSE(result.authenticated);
    EXPECT_TRUE(result.accessToken.isEmpty());
    EXPECT_FALSE(result.errorMessage.isEmpty());
}

/**
 * @brief 测试用户认证 - 失败场景（用户不存在）
 */
TEST_F(SecurityManagerTest, AuthenticateConnection_UserNotFound) {
    SecurityConfig config;
    config.enableSecurity = true;
    config.certificateConfig.enabled = false;  // 测试环境禁用SSL
    config.tokenConfig.secretKey = "test-secret-key-32-bytes-long!!";
    config.rbacConfig.databasePath = tempDir->filePath("test_security.db");

    ASSERT_TRUE(securityManager->initialize(config));

    auto result = securityManager->authenticateConnection("nonexistent", "password", "127.0.0.1");

    EXPECT_FALSE(result.authenticated);
    EXPECT_TRUE(result.accessToken.isEmpty());
}

/**
 * @brief 测试Token验证
 */
TEST_F(SecurityManagerTest, ValidateToken) {
    SecurityConfig config;
    config.enableSecurity = true;
    config.certificateConfig.enabled = false;  // 测试环境禁用SSL
    config.tokenConfig.secretKey = "test-secret-key-32-bytes-long!!";
    config.rbacConfig.databasePath = tempDir->filePath("test_security.db");

    ASSERT_TRUE(securityManager->initialize(config));
    ASSERT_TRUE(securityManager->addUser("testuser", "password123", UserRole::Operator));

    // 认证获取Token
    auto authResult = securityManager->authenticateConnection("testuser", "password123");
    ASSERT_TRUE(authResult.authenticated);

    // 验证Token
    auto validateResult = securityManager->validateToken(authResult.accessToken);

    EXPECT_TRUE(validateResult.isValid);
    EXPECT_EQ(validateResult.username, "testuser");
    EXPECT_EQ(validateResult.role, "Operator");
    EXPECT_GT(validateResult.remainingSeconds, 0);
}

/**
 * @brief 测试Token刷新
 */
TEST_F(SecurityManagerTest, RefreshToken) {
    SecurityConfig config;
    config.enableSecurity = true;
    config.certificateConfig.enabled = false;  // 测试环境禁用SSL
    config.tokenConfig.secretKey = "test-secret-key-32-bytes-long!!";
    config.rbacConfig.databasePath = tempDir->filePath("test_security.db");

    ASSERT_TRUE(securityManager->initialize(config));
    ASSERT_TRUE(securityManager->addUser("testuser", "password123", UserRole::Admin));

    // 认证获取Token
    auto authResult = securityManager->authenticateConnection("testuser", "password123");
    ASSERT_TRUE(authResult.authenticated);

    QString oldAccessToken = authResult.accessToken;

    // 刷新Token
    QString newAccessToken = securityManager->refreshToken(authResult.refreshToken);

    EXPECT_FALSE(newAccessToken.isEmpty());
    EXPECT_NE(newAccessToken, oldAccessToken);

    // 验证新Token
    auto validateResult = securityManager->validateToken(newAccessToken);
    EXPECT_TRUE(validateResult.isValid);
}

/**
 * @brief 测试权限检查 - Viewer角色
 */
TEST_F(SecurityManagerTest, AuthorizeAction_ViewerRole) {
    SecurityConfig config;
    config.enableSecurity = true;
    config.certificateConfig.enabled = false;  // 测试环境禁用SSL
    config.tokenConfig.secretKey = "test-secret-key-32-bytes-long!!";
    config.rbacConfig.databasePath = tempDir->filePath("test_security.db");

    ASSERT_TRUE(securityManager->initialize(config));
    ASSERT_TRUE(securityManager->addUser("viewer", "password123", UserRole::Viewer));

    auto authResult = securityManager->authenticateConnection("viewer", "password123");
    ASSERT_TRUE(authResult.authenticated);

    // Viewer可以查看
    EXPECT_TRUE(securityManager->authorizeAction(authResult.accessToken, Permission::ViewLogs));
    EXPECT_TRUE(securityManager->authorizeAction(authResult.accessToken, Permission::ViewPerformanceMetrics));

    // Viewer不能导出或修改
    EXPECT_FALSE(securityManager->authorizeAction(authResult.accessToken, Permission::ExportLogs));
    EXPECT_FALSE(securityManager->authorizeAction(authResult.accessToken, Permission::ModifyConfiguration));
}

/**
 * @brief 测试权限检查 - Admin角色
 */
TEST_F(SecurityManagerTest, AuthorizeAction_AdminRole) {
    SecurityConfig config;
    config.enableSecurity = true;
    config.certificateConfig.enabled = false;  // 测试环境禁用SSL
    config.tokenConfig.secretKey = "test-secret-key-32-bytes-long!!";
    config.rbacConfig.databasePath = tempDir->filePath("test_security.db");

    ASSERT_TRUE(securityManager->initialize(config));
    // 使用系统自动创建的默认管理员账户（用户名:admin, 密码:admin123）
    auto authResult = securityManager->authenticateConnection("admin", "admin123");
    ASSERT_TRUE(authResult.authenticated);

    // Admin拥有所有权限
    EXPECT_TRUE(securityManager->authorizeAction(authResult.accessToken, Permission::ViewLogs));
    EXPECT_TRUE(securityManager->authorizeAction(authResult.accessToken, Permission::ExportLogs));
    EXPECT_TRUE(securityManager->authorizeAction(authResult.accessToken, Permission::ModifyConfiguration));
    EXPECT_TRUE(securityManager->authorizeAction(authResult.accessToken, Permission::ManageUsers));
}

/**
 * @brief 测试用户管理 - 添加用户
 */
TEST_F(SecurityManagerTest, AddUser) {
    SecurityConfig config;
    config.enableSecurity = true;
    config.certificateConfig.enabled = false;  // 测试环境禁用SSL
    config.tokenConfig.secretKey = "test-secret-key-32-bytes-long!!";
    config.rbacConfig.databasePath = tempDir->filePath("test_security.db");

    ASSERT_TRUE(securityManager->initialize(config));

    // 添加用户
    EXPECT_TRUE(securityManager->addUser("newuser", "password123", UserRole::Operator, "user@example.com", "测试用户"));

    // 验证用户可以登录
    auto result = securityManager->authenticateConnection("newuser", "password123");
    EXPECT_TRUE(result.authenticated);
    EXPECT_EQ(result.role, UserRole::Operator);
}

/**
 * @brief 测试用户管理 - 更新密码
 */
TEST_F(SecurityManagerTest, UpdateUserPassword) {
    SecurityConfig config;
    config.enableSecurity = true;
    config.certificateConfig.enabled = false;  // 测试环境禁用SSL
    config.tokenConfig.secretKey = "test-secret-key-32-bytes-long!!";
    config.rbacConfig.databasePath = tempDir->filePath("test_security.db");

    ASSERT_TRUE(securityManager->initialize(config));
    ASSERT_TRUE(securityManager->addUser("testuser", "oldpassword", UserRole::Viewer));

    // 获取userId
    auto result1 = securityManager->authenticateConnection("testuser", "oldpassword");
    ASSERT_TRUE(result1.authenticated);
    QString userId = result1.userId;

    // 更新密码
    EXPECT_TRUE(securityManager->updateUserPassword(userId, "newpassword"));

    // 旧密码无法登录
    auto result2 = securityManager->authenticateConnection("testuser", "oldpassword");
    EXPECT_FALSE(result2.authenticated);

    // 新密码可以登录
    auto result3 = securityManager->authenticateConnection("testuser", "newpassword");
    EXPECT_TRUE(result3.authenticated);
}

/**
 * @brief 测试用户管理 - 更新角色
 */
TEST_F(SecurityManagerTest, UpdateUserRole) {
    SecurityConfig config;
    config.enableSecurity = true;
    config.certificateConfig.enabled = false;  // 测试环境禁用SSL
    config.tokenConfig.secretKey = "test-secret-key-32-bytes-long!!";
    config.rbacConfig.databasePath = tempDir->filePath("test_security.db");

    ASSERT_TRUE(securityManager->initialize(config));
    ASSERT_TRUE(securityManager->addUser("testuser", "password123", UserRole::Viewer));

    // 获取userId
    auto result1 = securityManager->authenticateConnection("testuser", "password123");
    ASSERT_TRUE(result1.authenticated);
    QString userId = result1.userId;

    // 更新角色
    EXPECT_TRUE(securityManager->updateUserRole(userId, UserRole::Admin));

    // 重新登录，验证角色已更新
    auto result2 = securityManager->authenticateConnection("testuser", "password123");
    EXPECT_TRUE(result2.authenticated);
    EXPECT_EQ(result2.role, UserRole::Admin);
}

/**
 * @brief 测试登出功能
 */
TEST_F(SecurityManagerTest, Logout) {
    SecurityConfig config;
    config.enableSecurity = true;
    config.certificateConfig.enabled = false;  // 测试环境禁用SSL
    config.tokenConfig.secretKey = "test-secret-key-32-bytes-long!!";
    config.rbacConfig.databasePath = tempDir->filePath("test_security.db");

    ASSERT_TRUE(securityManager->initialize(config));
    ASSERT_TRUE(securityManager->addUser("testuser", "password123", UserRole::Viewer));

    auto authResult = securityManager->authenticateConnection("testuser", "password123");
    ASSERT_TRUE(authResult.authenticated);

    // Token有效
    auto validateResult1 = securityManager->validateToken(authResult.accessToken);
    EXPECT_TRUE(validateResult1.isValid);

    // 登出
    securityManager->logout(authResult.userId, authResult.accessToken);

    // Token被撤销
    auto validateResult2 = securityManager->validateToken(authResult.accessToken);
    EXPECT_FALSE(validateResult2.isValid);
}

/**
 * @brief 测试安全功能禁用场景
 */
TEST_F(SecurityManagerTest, SecurityDisabled) {
    SecurityConfig config;
    config.enableSecurity = false;  // 禁用安全功能

    ASSERT_TRUE(securityManager->initialize(config));
    EXPECT_FALSE(securityManager->isEnabled());

    // 安全功能禁用时，authorizeAction应该返回true
    bool result = securityManager->authorizeUser("any_user_id", Permission::ManageUsers);
    EXPECT_TRUE(result);
}

/**
 * Main函数
 */
int main(int argc, char** argv) {
    // 初始化Qt应用（某些Qt类需要QCoreApplication）
    QCoreApplication app(argc, argv);

    // 初始化Google Test
    ::testing::InitGoogleTest(&argc, argv);

    // 运行测试
    return RUN_ALL_TESTS();
}
