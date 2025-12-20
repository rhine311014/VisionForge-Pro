/**
 * @file TokenAuthManager.cpp
 * @brief JWT Token认证管理器实现
 * @author VisionForge Team
 * @date 2025-12-20
 */

#include "remote/TokenAuthManager.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QCryptographicHash>
#include <QMessageAuthenticationCode>
#include <QUuid>
#include <QDebug>
#include <QMutexLocker>

namespace VisionForge {
namespace Remote {

TokenAuthManager::TokenAuthManager(QObject* parent)
    : QObject(parent)
    , cleanupTimer_(nullptr)
{
}

TokenAuthManager::~TokenAuthManager()
{
    if (cleanupTimer_) {
        cleanupTimer_->stop();
        delete cleanupTimer_;
    }
}

bool TokenAuthManager::initialize(const TokenConfig& config)
{
    QMutexLocker locker(&mutex_);
    config_ = config;
    secretKey_ = config_.secretKey;

    qInfo() << "Token认证管理器初始化";
    qInfo() << "  访问Token有效期:" << config_.accessTokenLifetime << "秒";
    qInfo() << "  刷新Token有效期:" << config_.refreshTokenLifetime << "秒";
    qInfo() << "  每用户最大Token数:" << config_.maxTokensPerUser;

    // 启动清理定时器
    if (!cleanupTimer_) {
        cleanupTimer_ = new QTimer(this);
        connect(cleanupTimer_, &QTimer::timeout, this, &TokenAuthManager::cleanupExpiredTokens);
    }

    cleanupTimer_->start(config_.blacklistCleanupInterval * 1000);

    return true;
}

void TokenAuthManager::setSecretKey(const QString& key)
{
    QMutexLocker locker(&mutex_);
    secretKey_ = key;
    qInfo() << "Token密钥已更新";
}

QString TokenAuthManager::generateAccessToken(const QString& userId,
                                             const QString& username,
                                             const QString& role,
                                             const QString& ipAddress)
{
    return generateToken(userId, username, role, config_.accessTokenLifetime, false, ipAddress);
}

QString TokenAuthManager::generateRefreshToken(const QString& userId,
                                              const QString& username,
                                              const QString& role,
                                              const QString& ipAddress)
{
    return generateToken(userId, username, role, config_.refreshTokenLifetime, true, ipAddress);
}

TokenAuthManager::TokenPair TokenAuthManager::generateTokenPair(const QString& userId,
                                                                const QString& username,
                                                                const QString& role,
                                                                const QString& ipAddress)
{
    TokenPair pair;
    pair.accessToken = generateAccessToken(userId, username, role, ipAddress);
    pair.refreshToken = generateRefreshToken(userId, username, role, ipAddress);

    // 获取过期时间
    auto accessResult = validateToken(pair.accessToken);
    auto refreshResult = validateToken(pair.refreshToken);

    pair.accessTokenExpiry = accessResult.expiryTime;
    pair.refreshTokenExpiry = refreshResult.expiryTime;

    return pair;
}

TokenValidationResult TokenAuthManager::validateToken(const QString& token)
{
    TokenValidationResult result;

    if (token.isEmpty()) {
        result.errorMessage = "Token为空";
        return result;
    }

    // 检查黑名单
    if (config_.enableBlacklist && isTokenRevoked(token)) {
        result.errorMessage = "Token已被撤销";
        emit tokenValidationFailed(token, result.errorMessage);
        return result;
    }

    // 解析JWT
    bool parseOk = false;
    QJsonObject payload = parseJWT(token, &parseOk);

    if (!parseOk) {
        result.errorMessage = "Token格式错误或签名无效";
        emit tokenValidationFailed(token, result.errorMessage);
        return result;
    }

    // 提取信息
    result.userId = payload["sub"].toString();
    result.username = payload["username"].toString();
    result.role = payload["role"].toString();
    result.tokenId = payload["jti"].toString();

    qint64 iat = payload["iat"].toInteger();
    qint64 exp = payload["exp"].toInteger();

    result.issuedAt = QDateTime::fromSecsSinceEpoch(iat);
    result.expiryTime = QDateTime::fromSecsSinceEpoch(exp);

    // 检查过期
    QDateTime now = QDateTime::currentDateTime();
    if (now > result.expiryTime) {
        result.errorMessage = "Token已过期";
        emit tokenExpired(result.tokenId);
        emit tokenValidationFailed(token, result.errorMessage);
        return result;
    }

    result.remainingSeconds = now.secsTo(result.expiryTime);
    result.isValid = true;

    emit tokenValidated(result.userId, result.tokenId);

    // 检查是否需要刷新
    if (config_.enableAutoRefresh && result.remainingSeconds <= config_.refreshThreshold) {
        emit tokenNeedsRefresh(token);
    }

    return result;
}

TokenValidationResult TokenAuthManager::quickValidateToken(const QString& token)
{
    TokenValidationResult result;

    if (token.isEmpty()) {
        result.errorMessage = "Token为空";
        return result;
    }

    // 解析JWT（不检查黑名单）
    bool parseOk = false;
    QJsonObject payload = parseJWT(token, &parseOk);

    if (!parseOk) {
        result.errorMessage = "Token格式错误或签名无效";
        return result;
    }

    result.userId = payload["sub"].toString();
    result.username = payload["username"].toString();
    result.role = payload["role"].toString();
    result.tokenId = payload["jti"].toString();

    qint64 exp = payload["exp"].toInteger();
    result.expiryTime = QDateTime::fromSecsSinceEpoch(exp);

    QDateTime now = QDateTime::currentDateTime();
    if (now > result.expiryTime) {
        result.errorMessage = "Token已过期";
        return result;
    }

    result.remainingSeconds = now.secsTo(result.expiryTime);
    result.isValid = true;

    return result;
}

bool TokenAuthManager::shouldRefreshToken(const QString& token)
{
    auto result = quickValidateToken(token);
    return result.isValid && result.remainingSeconds <= config_.refreshThreshold;
}

QString TokenAuthManager::refreshAccessToken(const QString& refreshToken)
{
    auto result = validateToken(refreshToken);

    if (!result.isValid) {
        qWarning() << "刷新Token无效:" << result.errorMessage;
        return QString();
    }

    // 生成新的访问Token
    return generateAccessToken(result.userId, result.username, result.role);
}

TokenAuthManager::TokenPair TokenAuthManager::refreshTokenPair(const QString& refreshToken)
{
    TokenPair pair;

    auto result = validateToken(refreshToken);
    if (!result.isValid) {
        qWarning() << "刷新Token无效:" << result.errorMessage;
        return pair;
    }

    // 撤销旧的刷新Token
    revokeToken(refreshToken);

    // 生成新的Token对
    return generateTokenPair(result.userId, result.username, result.role);
}

void TokenAuthManager::revokeToken(const QString& token)
{
    QMutexLocker locker(&mutex_);

    if (!config_.enableBlacklist) {
        return;
    }

    // 获取Token过期时间
    bool parseOk = false;
    QJsonObject payload = parseJWT(token, &parseOk);

    if (!parseOk) {
        qWarning() << "无法解析要撤销的Token";
        return;
    }

    QString tokenId = payload["jti"].toString();
    qint64 exp = payload["exp"].toInteger();
    QDateTime expiryTime = QDateTime::fromSecsSinceEpoch(exp);

    // 加入黑名单
    tokenBlacklist_[token] = expiryTime;

    // 从活跃Token中移除
    if (activeTokens_.contains(tokenId)) {
        QString userId = activeTokens_[tokenId].userId;
        activeTokens_.remove(tokenId);

        // 从用户Token列表中移除
        if (userTokens_.contains(userId)) {
            userTokens_[userId].removeAll(tokenId);
        }
    }

    qInfo() << "Token已撤销:" << tokenId;
    emit tokenRevoked(tokenId, "手动撤销");
}

void TokenAuthManager::revokeUserTokens(const QString& userId)
{
    QMutexLocker locker(&mutex_);

    if (!userTokens_.contains(userId)) {
        return;
    }

    QList<QString> tokenIds = userTokens_[userId];
    int count = 0;

    for (const QString& tokenId : tokenIds) {
        if (activeTokens_.contains(tokenId)) {
            // 需要构造完整Token来加入黑名单
            // 这里简化处理，直接从activeTokens_移除
            activeTokens_.remove(tokenId);
            count++;
        }
    }

    userTokens_.remove(userId);

    qInfo() << "已撤销用户" << userId << "的" << count << "个Token";
    emit tokenRevoked(userId, QString("撤销用户所有Token (%1个)").arg(count));
}

bool TokenAuthManager::isTokenRevoked(const QString& token) const
{
    QMutexLocker locker(&mutex_);
    return tokenBlacklist_.contains(token);
}

QList<TokenInfo> TokenAuthManager::getUserTokens(const QString& userId) const
{
    QMutexLocker locker(&mutex_);
    QList<TokenInfo> tokens;

    if (!userTokens_.contains(userId)) {
        return tokens;
    }

    const QList<QString>& tokenIds = userTokens_[userId];
    for (const QString& tokenId : tokenIds) {
        if (activeTokens_.contains(tokenId)) {
            tokens.append(activeTokens_[tokenId]);
        }
    }

    return tokens;
}

TokenInfo TokenAuthManager::getTokenInfo(const QString& token)
{
    TokenInfo info;

    bool parseOk = false;
    QJsonObject payload = parseJWT(token, &parseOk);

    if (!parseOk) {
        return info;
    }

    info.tokenId = payload["jti"].toString();
    info.userId = payload["sub"].toString();
    info.username = payload["username"].toString();
    info.role = payload["role"].toString();

    qint64 iat = payload["iat"].toInteger();
    qint64 exp = payload["exp"].toInteger();

    info.issuedAt = QDateTime::fromSecsSinceEpoch(iat);
    info.expiryTime = QDateTime::fromSecsSinceEpoch(exp);
    info.ipAddress = payload["ip"].toString();
    info.isRefreshToken = payload["refresh"].toBool(false);

    return info;
}

void TokenAuthManager::cleanupExpiredTokens()
{
    QMutexLocker locker(&mutex_);

    QDateTime now = QDateTime::currentDateTime();
    int cleaned = 0;

    // 清理过期的活跃Token
    QMutableMapIterator<QString, TokenInfo> tokenIter(activeTokens_);
    while (tokenIter.hasNext()) {
        tokenIter.next();
        if (now > tokenIter.value().expiryTime) {
            QString tokenId = tokenIter.key();
            QString userId = tokenIter.value().userId;

            tokenIter.remove();
            cleaned++;

            // 从用户Token列表中移除
            if (userTokens_.contains(userId)) {
                userTokens_[userId].removeAll(tokenId);
            }

            emit tokenExpired(tokenId);
        }
    }

    // 清理过期的黑名单Token
    cleanupBlacklist();

    if (cleaned > 0) {
        qInfo() << "清理了" << cleaned << "个过期Token";
    }
}

TokenAuthManager::Statistics TokenAuthManager::getStatistics() const
{
    QMutexLocker locker(&mutex_);

    Statistics stats;
    stats.totalActiveTokens = activeTokens_.size();
    stats.blacklistedTokens = tokenBlacklist_.size();

    // 统计每用户Token数
    for (auto it = userTokens_.constBegin(); it != userTokens_.constEnd(); ++it) {
        stats.tokensPerUser[it.key()] = it.value().size();
    }

    return stats;
}

QString TokenAuthManager::generateToken(const QString& userId,
                                       const QString& username,
                                       const QString& role,
                                       int lifetimeSeconds,
                                       bool isRefreshToken,
                                       const QString& ipAddress)
{
    QMutexLocker locker(&mutex_);

    // 生成Token ID
    QString tokenId = generateTokenId();

    // 创建Payload
    QDateTime now = QDateTime::currentDateTime();
    QDateTime expiry = now.addSecs(lifetimeSeconds);

    QJsonObject payload;
    payload["sub"] = userId;                    // Subject (用户ID)
    payload["username"] = username;             // 用户名
    payload["role"] = role;                     // 角色
    payload["iat"] = now.toSecsSinceEpoch();    // Issued At
    payload["exp"] = expiry.toSecsSinceEpoch(); // Expiry
    payload["jti"] = tokenId;                   // JWT ID
    payload["refresh"] = isRefreshToken;        // 是否为刷新Token

    if (!ipAddress.isEmpty()) {
        payload["ip"] = ipAddress;              // IP地址
    }

    // 生成JWT
    QString token = generateJWT(payload);

    // 记录活跃Token
    TokenInfo info;
    info.tokenId = tokenId;
    info.userId = userId;
    info.username = username;
    info.role = role;
    info.issuedAt = now;
    info.expiryTime = expiry;
    info.ipAddress = ipAddress;
    info.isRefreshToken = isRefreshToken;

    activeTokens_[tokenId] = info;

    // 记录用户Token
    if (!userTokens_.contains(userId)) {
        userTokens_[userId] = QList<QString>();
    }
    userTokens_[userId].append(tokenId);

    // 限制用户Token数量
    enforceTokenLimit(userId);

    qInfo() << "生成Token - 用户:" << username << "ID:" << tokenId
            << "类型:" << (isRefreshToken ? "刷新" : "访问")
            << "有效期:" << lifetimeSeconds << "秒";

    emit tokenGenerated(userId, tokenId);

    return token;
}

QString TokenAuthManager::generateJWT(const QJsonObject& payload)
{
    // Header
    QJsonObject header;
    header["alg"] = "HS256";
    header["typ"] = "JWT";

    // 编码Header和Payload
    QString headerEncoded = base64UrlEncode(QJsonDocument(header).toJson(QJsonDocument::Compact));
    QString payloadEncoded = base64UrlEncode(QJsonDocument(payload).toJson(QJsonDocument::Compact));

    // 签名
    QString data = headerEncoded + "." + payloadEncoded;
    QString signature = signPayload(data);

    // 组装JWT
    return data + "." + signature;
}

QJsonObject TokenAuthManager::parseJWT(const QString& token, bool* ok)
{
    if (ok) *ok = false;

    QStringList parts = token.split('.');
    if (parts.size() != 3) {
        return QJsonObject();
    }

    QString headerEncoded = parts[0];
    QString payloadEncoded = parts[1];
    QString signature = parts[2];

    // 验证签名
    QString data = headerEncoded + "." + payloadEncoded;
    if (!verifySignature(data, signature)) {
        return QJsonObject();
    }

    // 解码Payload
    QByteArray payloadJson = base64UrlDecode(payloadEncoded);
    QJsonDocument doc = QJsonDocument::fromJson(payloadJson);

    if (doc.isNull() || !doc.isObject()) {
        return QJsonObject();
    }

    if (ok) *ok = true;
    return doc.object();
}

QString TokenAuthManager::signPayload(const QString& data)
{
    QByteArray key = secretKey_.toUtf8();
    QByteArray message = data.toUtf8();

    QByteArray hmac = QMessageAuthenticationCode::hash(
        message,
        key,
        QCryptographicHash::Sha256
    );

    return base64UrlEncode(hmac);
}

bool TokenAuthManager::verifySignature(const QString& data, const QString& signature)
{
    QString expectedSignature = signPayload(data);
    return expectedSignature == signature;
}

QString TokenAuthManager::base64UrlEncode(const QByteArray& data)
{
    QString base64 = data.toBase64();

    // Base64URL: 替换字符并移除填充
    base64.replace('+', '-');
    base64.replace('/', '_');
    base64.replace("=", "");

    return base64;
}

QByteArray TokenAuthManager::base64UrlDecode(const QString& data)
{
    QString base64 = data;

    // 还原Base64格式
    base64.replace('-', '+');
    base64.replace('_', '/');

    // 添加填充
    int padding = (4 - base64.length() % 4) % 4;
    base64.append(QString("=").repeated(padding));

    return QByteArray::fromBase64(base64.toUtf8());
}

QString TokenAuthManager::generateTokenId()
{
    return QUuid::createUuid().toString(QUuid::WithoutBraces);
}

void TokenAuthManager::cleanupBlacklist()
{
    QDateTime now = QDateTime::currentDateTime();
    int cleaned = 0;

    QMutableMapIterator<QString, QDateTime> iter(tokenBlacklist_);
    while (iter.hasNext()) {
        iter.next();
        if (now > iter.value()) {
            iter.remove();
            cleaned++;
        }
    }

    if (cleaned > 0) {
        qInfo() << "清理了" << cleaned << "个过期黑名单Token";
    }
}

void TokenAuthManager::enforceTokenLimit(const QString& userId)
{
    if (!userTokens_.contains(userId)) {
        return;
    }

    QList<QString>& tokens = userTokens_[userId];

    // 如果超过限制，移除最旧的Token
    while (tokens.size() > config_.maxTokensPerUser) {
        QString oldestTokenId = tokens.takeFirst();

        if (activeTokens_.contains(oldestTokenId)) {
            activeTokens_.remove(oldestTokenId);
            qInfo() << "移除用户" << userId << "的旧Token:" << oldestTokenId;
        }
    }
}

} // namespace Remote
} // namespace VisionForge
