/**
 * @file SecurityCertificateManager.h
 * @brief SSL/TLS证书管理器
 * @details 负责证书加载、验证、自签名证书生成
 * @author VisionForge Team
 * @date 2025-12-20
 */

#pragma once

#include <QObject>
#include <QSslCertificate>
#include <QSslKey>
#include <QSslConfiguration>
#include <QDateTime>
#include <QList>
#include <QString>
#include <QMutex>

namespace VisionForge {
namespace Remote {

/**
 * @brief 证书配置
 */
struct CertificateConfig {
    bool enabled = false;                        // 是否启用SSL
    bool useSelfSigned = false;                  // 是否使用自签名证书
    QString certificatePath;                     // 证书文件路径
    QString privateKeyPath;                      // 私钥文件路径
    QString caBundlePath;                        // CA证书链路径
    QString commonName = "VisionForge Server";   // 通用名称（用于自签名）
    int validDays = 365;                         // 证书有效天数（用于自签名）
    QSsl::SslProtocol sslProtocol = QSsl::TlsV1_2OrLater;  // SSL协议版本
};

/**
 * @brief 证书信息
 */
struct CertificateInfo {
    QString commonName;
    QString organization;
    QString country;
    QDateTime notBefore;
    QDateTime notAfter;
    QString serialNumber;
    QString issuer;
    int daysUntilExpiry;
    bool isValid;
    bool isSelfSigned;
};

/**
 * @class SecurityCertificateManager
 * @brief SSL/TLS证书管理器
 *
 * 功能特性:
 * - 加载和管理SSL证书
 * - 生成自签名证书（用于开发/测试）
 * - 证书验证和链验证
 * - 证书过期监控
 * - 提供SSL配置
 */
class SecurityCertificateManager : public QObject
{
    Q_OBJECT

public:
    explicit SecurityCertificateManager(QObject* parent = nullptr);
    ~SecurityCertificateManager() override;

    /**
     * @brief 初始化证书管理器
     */
    bool initialize(const CertificateConfig& config = CertificateConfig());

    /**
     * @brief 加载证书和私钥
     */
    bool loadCertificate(const QString& certPath, const QString& keyPath,
                        const QString& password = QString());

    /**
     * @brief 加载CA证书链
     */
    bool loadCACertificates(const QString& caBundlePath);

    /**
     * @brief 生成自签名证书（用于开发/测试）
     * @param commonName 通用名称（如：localhost、域名）
     * @param validDays 证书有效天数
     * @return 是否成功生成
     */
    bool generateSelfSignedCertificate(const QString& commonName = "VisionForge Server",
                                      int validDays = 365);

    /**
     * @brief 获取SSL配置
     */
    QSslConfiguration getSslConfiguration() const;

    /**
     * @brief 验证证书
     */
    bool verifyCertificate() const;

    /**
     * @brief 检查证书是否有效
     */
    bool isValid() const;

    /**
     * @brief 证书是否已加载
     */
    bool isLoaded() const;

    /**
     * @brief 获取证书
     */
    QSslCertificate certificate() const;

    /**
     * @brief 获取私钥
     */
    QSslKey privateKey() const;

    /**
     * @brief 获取证书信息
     */
    CertificateInfo getCertificateInfo() const;

    /**
     * @brief 获取证书过期时间
     */
    QDateTime expiryDate() const;

    /**
     * @brief 获取证书剩余有效天数
     */
    int daysUntilExpiry() const;

    /**
     * @brief 检查证书是否即将过期
     * @param warningDays 提前多少天警告（默认30天）
     */
    bool isExpiringSoon(int warningDays = 30) const;

    /**
     * @brief 导出证书到文件
     */
    bool saveCertificate(const QString& certPath, const QString& keyPath) const;

    /**
     * @brief 清除已加载的证书
     */
    void clear();

signals:
    /**
     * @brief 证书加载成功
     */
    void certificateLoaded(const CertificateInfo& info);

    /**
     * @brief 证书加载失败
     */
    void certificateError(const QString& error);

    /**
     * @brief 证书即将过期
     */
    void certificateExpiringSoon(int daysRemaining);

    /**
     * @brief 证书已过期
     */
    void certificateExpired();

private:
    /**
     * @brief 验证证书有效性
     */
    bool validateCertificate() const;

    /**
     * @brief 创建SSL配置
     */
    void createSslConfiguration();

    /**
     * @brief 检查证书过期状态
     */
    void checkExpiry();

    /**
     * @brief 生成RSA密钥对
     */
    QSslKey generatePrivateKey();

    /**
     * @brief 创建自签名证书
     */
    QSslCertificate createSelfSignedCert(const QSslKey& privateKey,
                                        const QString& commonName,
                                        int validDays);

    /**
     * @brief 创建测试证书（用于开发）
     */
    bool createTestCertificate();

private:
    CertificateConfig config_;              // 证书配置
    QSslCertificate certificate_;           // SSL证书
    QSslKey privateKey_;                    // 私钥
    QList<QSslCertificate> caCertificates_; // CA证书链
    QSslConfiguration sslConfig_;           // SSL配置
    bool isLoaded_;                         // 是否已加载
    mutable QMutex mutex_;                  // 线程安全锁
};

} // namespace Remote
} // namespace VisionForge
