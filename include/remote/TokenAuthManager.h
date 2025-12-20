/**
 * @file TokenAuthManager.h
 * @brief JWT Token认证管理器
 * @details 负责Token生成、验证、刷新和撤销
 * @author VisionForge Team
 * @date 2025-12-20
 */

#pragma once

#include <QObject>
#include <QString>
#include <QDateTime>
#include <QMap>
#include <QMutex>
#include <QJsonObject>
#include <QTimer>

namespace VisionForge {
namespace Remote {

/**
 * @brief Token配置
 */
struct TokenConfig {
    QString secretKey = "visionforge-secret-key-change-in-production";  // HMAC密钥
    int accessTokenLifetime = 3600;        // 访问Token有效期（秒，默认1小时）
    int refreshTokenLifetime = 86400;      // 刷新Token有效期（秒，默认24小时）
    bool enableAutoRefresh = true;         // 启用自动刷新
    int refreshThreshold = 300;            // 自动刷新阈值（秒，默认5分钟）
    int maxTokensPerUser = 5;              // 每用户最大Token数量
    bool enableBlacklist = true;           // 启用黑名单
    int blacklistCleanupInterval = 3600;   // 黑名单清理间隔（秒）
};

/**
 * @brief Token验证结果
 */
struct TokenValidationResult {
    bool isValid = false;                  // 是否有效
    QString userId;                        // 用户ID
    QString username;                      // 用户名
    QString role;                          // 角色
    QString tokenId;                       // Token唯一ID
    QDateTime issuedAt;                    // 签发时间
    QDateTime expiryTime;                  // 过期时间
    QString errorMessage;                  // 错误信息
    int remainingSeconds = 0;              // 剩余有效秒数
};

/**
 * @brief Token信息
 */
struct TokenInfo {
    QString tokenString;                   // 完整的Token字符串(用于黑名单撤销)
    QString tokenId;                       // Token唯一ID
    QString userId;                        // 用户ID
    QString username;                      // 用户名
    QString role;                          // 角色
    QDateTime issuedAt;                    // 签发时间
    QDateTime expiryTime;                  // 过期时间
    QString ipAddress;                     // 签发IP地址
    bool isRefreshToken = false;           // 是否为刷新Token
};

/**
 * @class TokenAuthManager
 * @brief JWT Token认证管理器
 *
 * 功能特性:
 * - JWT Token生成和验证
 * - HMAC-SHA256签名
 * - Token刷新机制
 * - Token黑名单管理
 * - 自动过期清理
 * - 多设备Token管理
 */
class TokenAuthManager : public QObject
{
    Q_OBJECT

public:
    explicit TokenAuthManager(QObject* parent = nullptr);
    ~TokenAuthManager() override;

    /**
     * @brief 初始化Token管理器
     */
    bool initialize(const TokenConfig& config = TokenConfig());

    /**
     * @brief 设置密钥
     */
    void setSecretKey(const QString& key);

    // ========== Token生成 ==========

    /**
     * @brief 生成访问Token
     */
    QString generateAccessToken(const QString& userId,
                               const QString& username,
                               const QString& role,
                               const QString& ipAddress = QString());

    /**
     * @brief 生成刷新Token
     */
    QString generateRefreshToken(const QString& userId,
                                const QString& username,
                                const QString& role,
                                const QString& ipAddress = QString());

    /**
     * @brief 生成Token对（访问Token + 刷新Token）
     */
    struct TokenPair {
        QString accessToken;
        QString refreshToken;
        QDateTime accessTokenExpiry;
        QDateTime refreshTokenExpiry;
    };

    TokenPair generateTokenPair(const QString& userId,
                               const QString& username,
                               const QString& role,
                               const QString& ipAddress = QString());

    // ========== Token验证 ==========

    /**
     * @brief 验证Token
     */
    TokenValidationResult validateToken(const QString& token);

    /**
     * @brief 快速验证Token（仅检查签名和过期，不检查黑名单）
     */
    TokenValidationResult quickValidateToken(const QString& token);

    /**
     * @brief 检查Token是否需要刷新
     */
    bool shouldRefreshToken(const QString& token);

    // ========== Token刷新 ==========

    /**
     * @brief 刷新Token（使用刷新Token换取新的访问Token）
     */
    QString refreshAccessToken(const QString& refreshToken);

    /**
     * @brief 刷新Token对
     */
    TokenPair refreshTokenPair(const QString& refreshToken);

    // ========== Token撤销 ==========

    /**
     * @brief 撤销Token（加入黑名单）
     */
    void revokeToken(const QString& token);

    /**
     * @brief 撤销用户的所有Token
     */
    void revokeUserTokens(const QString& userId);

    /**
     * @brief 检查Token是否已撤销
     */
    bool isTokenRevoked(const QString& token) const;

    // ========== Token管理 ==========

    /**
     * @brief 获取用户的所有活跃Token
     */
    QList<TokenInfo> getUserTokens(const QString& userId) const;

    /**
     * @brief 获取Token信息
     */
    TokenInfo getTokenInfo(const QString& token);

    /**
     * @brief 清理过期Token
     */
    void cleanupExpiredTokens();

    /**
     * @brief 获取统计信息
     */
    struct Statistics {
        int totalActiveTokens = 0;
        int blacklistedTokens = 0;
        int expiredTokensCleaned = 0;
        QMap<QString, int> tokensPerUser;
    };

    Statistics getStatistics() const;

signals:
    /**
     * @brief Token生成
     */
    void tokenGenerated(const QString& userId, const QString& tokenId);

    /**
     * @brief Token验证成功
     */
    void tokenValidated(const QString& userId, const QString& tokenId);

    /**
     * @brief Token验证失败
     */
    void tokenValidationFailed(const QString& token, const QString& reason);

    /**
     * @brief Token撤销
     */
    void tokenRevoked(const QString& tokenId, const QString& reason);

    /**
     * @brief Token过期
     */
    void tokenExpired(const QString& tokenId);

    /**
     * @brief Token需要刷新
     */
    void tokenNeedsRefresh(const QString& token);

private:
    /**
     * @brief 生成Token（内部方法）
     */
    QString generateToken(const QString& userId,
                         const QString& username,
                         const QString& role,
                         int lifetimeSeconds,
                         bool isRefreshToken,
                         const QString& ipAddress);

    /**
     * @brief 生成JWT
     */
    QString generateJWT(const QJsonObject& payload);

    /**
     * @brief 解析JWT
     */
    QJsonObject parseJWT(const QString& token, bool* ok = nullptr);

    /**
     * @brief 签名数据
     */
    QString signPayload(const QString& data);

    /**
     * @brief 验证签名
     */
    bool verifySignature(const QString& data, const QString& signature);

    /**
     * @brief Base64URL编码
     */
    QString base64UrlEncode(const QByteArray& data);

    /**
     * @brief Base64URL解码
     */
    QByteArray base64UrlDecode(const QString& data);

    /**
     * @brief 生成唯一Token ID
     */
    QString generateTokenId();

    /**
     * @brief 清理黑名单
     */
    void cleanupBlacklist();

    /**
     * @brief 限制用户Token数量
     */
    void enforceTokenLimit(const QString& userId);

private:
    TokenConfig config_;                                // Token配置
    QString secretKey_;                                 // HMAC密钥
    QMap<QString, TokenInfo> activeTokens_;             // tokenId -> TokenInfo
    QMap<QString, QDateTime> tokenBlacklist_;           // token -> 过期时间
    QMap<QString, QList<QString>> userTokens_;          // userId -> [tokenId]
    QTimer* cleanupTimer_;                              // 清理定时器
    mutable QMutex mutex_;                              // 线程安全锁
};

} // namespace Remote
} // namespace VisionForge
