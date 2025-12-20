/**
 * @file test_TokenAuthManager_gtest.cpp
 * @brief TokenAuthManager 单元测试
 *
 * 测试覆盖:
 * - Token生成 (Access + Refresh)
 * - Token验证
 * - Token刷新
 * - Token撤销
 */

#include <gtest/gtest.h>
#include <QCoreApplication>
#include <QTest>
#include <QSignalSpy>
#include <QThread>
#include <QDebug>
#include "remote/TokenAuthManager.h"

using namespace VisionForge::Remote;

class TokenAuthManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
        tokenManager = new TokenAuthManager();

        TokenConfig config;
        config.secretKey = "test_secret_key_for_unit_testing_12345";
        config.accessTokenLifetime = 60;   // 60秒有效期(测试用)
        config.refreshTokenLifetime = 3600;
        config.enableAutoRefresh = true;
        config.enableBlacklist = true;

        ASSERT_TRUE(tokenManager->initialize(config));
    }

    void TearDown() override {
        delete tokenManager;
        tokenManager = nullptr;
    }

    TokenAuthManager* tokenManager = nullptr;
};

/**
 * 测试1: 生成访问Token
 */
TEST_F(TokenAuthManagerTest, GenerateAccessToken) {
    QString token = tokenManager->generateAccessToken(
        "user123",
        "testuser",
        "admin",
        "192.168.1.100"
    );

    EXPECT_FALSE(token.isEmpty());
    EXPECT_TRUE(token.contains("."));

    QStringList parts = token.split(".");
    EXPECT_EQ(parts.size(), 3);

    qDebug() << "生成的Access Token:" << token.left(50) << "...";
}

/**
 * 测试2: 生成Token对
 */
TEST_F(TokenAuthManagerTest, GenerateTokenPair) {
    TokenAuthManager::TokenPair tokenPair = tokenManager->generateTokenPair(
        "user123",
        "testuser",
        "admin",
        "192.168.1.100"
    );

    EXPECT_FALSE(tokenPair.accessToken.isEmpty());
    EXPECT_FALSE(tokenPair.refreshToken.isEmpty());
    EXPECT_NE(tokenPair.accessToken, tokenPair.refreshToken);

    qDebug() << "Access Token过期时间:" << tokenPair.accessTokenExpiry.toString();
    qDebug() << "Refresh Token过期时间:" << tokenPair.refreshTokenExpiry.toString();
}

/**
 * 测试3: 验证有效Token
 */
TEST_F(TokenAuthManagerTest, ValidateToken_Valid) {
    QString token = tokenManager->generateAccessToken(
        "user123",
        "testuser",
        "admin",
        "192.168.1.100"
    );

    TokenValidationResult result = tokenManager->validateToken(token);

    EXPECT_TRUE(result.isValid);
    EXPECT_EQ(result.userId, "user123");
    EXPECT_EQ(result.username, "testuser");
    EXPECT_EQ(result.role, "admin");
    EXPECT_TRUE(result.errorMessage.isEmpty());

    qDebug() << "Token验证成功 - UserID:" << result.userId
             << "Username:" << result.username
             << "Role:" << result.role;
}

/**
 * 测试4: 验证格式错误的Token
 */
TEST_F(TokenAuthManagerTest, ValidateToken_InvalidFormat) {
    TokenValidationResult result1 = tokenManager->validateToken("");
    EXPECT_FALSE(result1.isValid);

    TokenValidationResult result2 = tokenManager->validateToken("invalid.token");
    EXPECT_FALSE(result2.isValid);

    TokenValidationResult result3 = tokenManager->validateToken("not-a-jwt");
    EXPECT_FALSE(result3.isValid);

    qDebug() << "格式错误Token验证失败(预期)";
}

/**
 * 测试5: Token刷新
 */
TEST_F(TokenAuthManagerTest, RefreshToken_Success) {
    TokenAuthManager::TokenPair tokenPair = tokenManager->generateTokenPair(
        "user123",
        "testuser",
        "admin",
        "192.168.1.100"
    );

    QString oldAccessToken = tokenPair.accessToken;
    QString refreshToken = tokenPair.refreshToken;

    QString newAccessToken = tokenManager->refreshAccessToken(refreshToken);

    EXPECT_FALSE(newAccessToken.isEmpty());
    EXPECT_NE(newAccessToken, oldAccessToken);

    TokenValidationResult result = tokenManager->validateToken(newAccessToken);
    EXPECT_TRUE(result.isValid);
    EXPECT_EQ(result.userId, "user123");

    qDebug() << "Token刷新成功";
}

/**
 * 测试6: 无效的Refresh Token
 */
TEST_F(TokenAuthManagerTest, RefreshToken_Invalid) {
    QString newAccessToken = tokenManager->refreshAccessToken("invalid_refresh_token");

    EXPECT_TRUE(newAccessToken.isEmpty());

    qDebug() << "无效Refresh Token刷新失败(预期)";
}

/**
 * 测试7: Token撤销
 */
TEST_F(TokenAuthManagerTest, RevokeToken) {
    QString token = tokenManager->generateAccessToken(
        "user123",
        "testuser",
        "admin",
        "192.168.1.100"
    );

    TokenValidationResult result1 = tokenManager->validateToken(token);
    EXPECT_TRUE(result1.isValid);

    tokenManager->revokeToken(token);

    TokenValidationResult result2 = tokenManager->validateToken(token);
    EXPECT_FALSE(result2.isValid);

    qDebug() << "Token撤销成功";
}

/**
 * 测试8: 撤销用户所有Token
 */
TEST_F(TokenAuthManagerTest, RevokeUserTokens) {
    QString token1 = tokenManager->generateAccessToken("user123", "testuser", "admin", "192.168.1.100");
    QThread::msleep(10);
    QString token2 = tokenManager->generateAccessToken("user123", "testuser", "admin", "192.168.1.101");
    QThread::msleep(10);
    QString token3 = tokenManager->generateAccessToken("user456", "otheruser", "viewer", "192.168.1.102");

    EXPECT_TRUE(tokenManager->validateToken(token1).isValid);
    EXPECT_TRUE(tokenManager->validateToken(token2).isValid);
    EXPECT_TRUE(tokenManager->validateToken(token3).isValid);

    tokenManager->revokeUserTokens("user123");

    EXPECT_FALSE(tokenManager->validateToken(token1).isValid);
    EXPECT_FALSE(tokenManager->validateToken(token2).isValid);
    EXPECT_TRUE(tokenManager->validateToken(token3).isValid);

    qDebug() << "批量撤销user123的Token成功";
}

/**
 * 测试9: 获取统计信息
 */
TEST_F(TokenAuthManagerTest, GetStatistics) {
    tokenManager->generateAccessToken("user1", "user1", "admin", "192.168.1.1");
    tokenManager->generateAccessToken("user2", "user2", "viewer", "192.168.1.2");
    tokenManager->generateAccessToken("user3", "user3", "operator", "192.168.1.3");

    TokenAuthManager::Statistics stats = tokenManager->getStatistics();

    EXPECT_GE(stats.totalActiveTokens, 3);

    qDebug() << "活跃Token数量:" << stats.totalActiveTokens;
    qDebug() << "黑名单Token数量:" << stats.blacklistedTokens;
}

/**
 * 测试10: 获取Token信息
 */
TEST_F(TokenAuthManagerTest, GetTokenInfo) {
    QString token = tokenManager->generateAccessToken("user123", "testuser", "admin", "192.168.1.100");

    TokenInfo info = tokenManager->getTokenInfo(token);

    EXPECT_FALSE(info.tokenId.isEmpty());
    EXPECT_EQ(info.userId, "user123");
    EXPECT_EQ(info.username, "testuser");
    EXPECT_EQ(info.role, "admin");
    EXPECT_EQ(info.ipAddress, "192.168.1.100");

    qDebug() << "Token信息 - ID:" << info.tokenId
             << "UserID:" << info.userId
             << "发行时间:" << info.issuedAt.toString();
}

/**
 * 测试11: 快速验证Token
 */
TEST_F(TokenAuthManagerTest, QuickValidateToken) {
    QString token = tokenManager->generateAccessToken("user123", "testuser", "admin", "192.168.1.100");

    TokenValidationResult result = tokenManager->quickValidateToken(token);

    EXPECT_TRUE(result.isValid);
    EXPECT_EQ(result.userId, "user123");

    qDebug() << "快速验证成功";
}

/**
 * 测试12: 检查Token是否需要刷新
 */
TEST_F(TokenAuthManagerTest, ShouldRefreshToken) {
    QString token = tokenManager->generateAccessToken("user123", "testuser", "admin", "192.168.1.100");

    bool shouldRefresh = tokenManager->shouldRefreshToken(token);

    qDebug() << "Token需要刷新:" << (shouldRefresh ? "是" : "否");
}

/**
 * 测试13: 信号发射 - Token生成
 */
TEST_F(TokenAuthManagerTest, Signal_TokenGenerated) {
    QSignalSpy spy(tokenManager, &TokenAuthManager::tokenGenerated);

    tokenManager->generateAccessToken("user123", "testuser", "admin", "192.168.1.100");

    EXPECT_EQ(spy.count(), 1);

    QList<QVariant> arguments = spy.takeFirst();
    EXPECT_EQ(arguments.at(0).toString(), "user123");

    qDebug() << "tokenGenerated信号已发射";
}

/**
 * 测试14: 信号发射 - Token验证失败
 */
TEST_F(TokenAuthManagerTest, Signal_TokenValidationFailed) {
    QSignalSpy spy(tokenManager, &TokenAuthManager::tokenValidationFailed);

    tokenManager->validateToken("invalid_token");

    EXPECT_GE(spy.count(), 1);

    qDebug() << "tokenValidationFailed信号已发射";
}

/**
 * 测试15: 信号发射 - Token撤销
 */
TEST_F(TokenAuthManagerTest, Signal_TokenRevoked) {
    QSignalSpy spy(tokenManager, &TokenAuthManager::tokenRevoked);

    QString token = tokenManager->generateAccessToken("user123", "testuser", "admin", "192.168.1.100");
    tokenManager->revokeToken(token);

    EXPECT_EQ(spy.count(), 1);

    qDebug() << "tokenRevoked信号已发射";
}

/**
 * Main函数
 */
int main(int argc, char** argv) {
    QCoreApplication app(argc, argv);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
