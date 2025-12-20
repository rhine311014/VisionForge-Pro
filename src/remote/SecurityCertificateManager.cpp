/**
 * @file SecurityCertificateManager.cpp
 * @brief SSL/TLS证书管理器实现
 * @author VisionForge Team
 * @date 2025-12-20
 */

#include "remote/SecurityCertificateManager.h"
#include <QFile>
#include <QDebug>
#include <QSslSocket>
#include <QProcess>
#include <QTemporaryDir>
#include <QDir>
#include <QMutexLocker>

namespace VisionForge {
namespace Remote {

SecurityCertificateManager::SecurityCertificateManager(QObject* parent)
    : QObject(parent)
    , isLoaded_(false)
{
}

SecurityCertificateManager::~SecurityCertificateManager()
{
    clear();
}

bool SecurityCertificateManager::initialize(const CertificateConfig& config)
{
    QMutexLocker locker(&mutex_);
    config_ = config;

    if (!config_.enabled) {
        qInfo() << "SSL未启用";
        return true;
    }

    // 检查SSL支持
    if (!QSslSocket::supportsSsl()) {
        QString error = QString("系统不支持SSL (OpenSSL版本: %1, 构建版本: %2)")
                        .arg(QSslSocket::sslLibraryVersionString())
                        .arg(QSslSocket::sslLibraryBuildVersionString());
        qCritical() << error;
        emit certificateError(error);
        return false;
    }

    qInfo() << "SSL库版本:" << QSslSocket::sslLibraryVersionString();

    // 使用自签名证书
    if (config_.useSelfSigned) {
        qInfo() << "生成自签名证书...";
        return generateSelfSignedCertificate(config_.commonName, config_.validDays);
    }

    // 加载证书文件
    if (!config_.certificatePath.isEmpty() && !config_.privateKeyPath.isEmpty()) {
        if (!loadCertificate(config_.certificatePath, config_.privateKeyPath)) {
            return false;
        }

        // 加载CA证书链（可选）
        if (!config_.caBundlePath.isEmpty()) {
            loadCACertificates(config_.caBundlePath);
        }

        return true;
    }

    emit certificateError("未配置证书路径且未启用自签名证书");
    return false;
}

bool SecurityCertificateManager::loadCertificate(const QString& certPath,
                                                const QString& keyPath,
                                                const QString& password)
{
    QMutexLocker locker(&mutex_);

    qInfo() << "加载证书:" << certPath;
    qInfo() << "加载私钥:" << keyPath;

    // 读取证书文件
    QFile certFile(certPath);
    if (!certFile.open(QIODevice::ReadOnly)) {
        QString error = QString("无法打开证书文件: %1").arg(certPath);
        qCritical() << error;
        emit certificateError(error);
        return false;
    }

    // 解析证书
    QList<QSslCertificate> certs = QSslCertificate::fromDevice(&certFile, QSsl::Pem);
    certFile.close();

    if (certs.isEmpty()) {
        QString error = QString("证书文件为空或格式错误: %1").arg(certPath);
        qCritical() << error;
        emit certificateError(error);
        return false;
    }

    certificate_ = certs.first();

    // 读取私钥文件
    QFile keyFile(keyPath);
    if (!keyFile.open(QIODevice::ReadOnly)) {
        QString error = QString("无法打开私钥文件: %1").arg(keyPath);
        qCritical() << error;
        emit certificateError(error);
        return false;
    }

    QByteArray keyData = keyFile.readAll();
    keyFile.close();

    // 解析私钥
    privateKey_ = QSslKey(keyData, QSsl::Rsa, QSsl::Pem, QSsl::PrivateKey, password.toUtf8());

    if (privateKey_.isNull()) {
        // 尝试其他算法
        privateKey_ = QSslKey(keyData, QSsl::Dsa, QSsl::Pem, QSsl::PrivateKey, password.toUtf8());
    }

    if (privateKey_.isNull()) {
        privateKey_ = QSslKey(keyData, QSsl::Ec, QSsl::Pem, QSsl::PrivateKey, password.toUtf8());
    }

    if (privateKey_.isNull()) {
        QString error = "私钥解析失败，可能密码错误或格式不支持";
        qCritical() << error;
        emit certificateError(error);
        return false;
    }

    isLoaded_ = true;

    // 验证证书
    if (!validateCertificate()) {
        qWarning() << "证书验证失败，但仍然加载";
    }

    // 创建SSL配置
    createSslConfiguration();

    // 检查过期
    checkExpiry();

    CertificateInfo info = getCertificateInfo();
    qInfo() << "证书加载成功:";
    qInfo() << "  通用名称:" << info.commonName;
    qInfo() << "  颁发者:" << info.issuer;
    qInfo() << "  有效期:" << info.notBefore.toString() << "到" << info.notAfter.toString();
    qInfo() << "  剩余天数:" << info.daysUntilExpiry;

    emit certificateLoaded(info);
    return true;
}

bool SecurityCertificateManager::loadCACertificates(const QString& caBundlePath)
{
    QMutexLocker locker(&mutex_);

    qInfo() << "加载CA证书链:" << caBundlePath;

    QFile caFile(caBundlePath);
    if (!caFile.open(QIODevice::ReadOnly)) {
        qWarning() << "无法打开CA证书文件:" << caBundlePath;
        return false;
    }

    caCertificates_ = QSslCertificate::fromDevice(&caFile, QSsl::Pem);
    caFile.close();

    qInfo() << "加载了" << caCertificates_.size() << "个CA证书";
    return !caCertificates_.isEmpty();
}

bool SecurityCertificateManager::generateSelfSignedCertificate(const QString& commonName,
                                                              int validDays)
{
    QMutexLocker locker(&mutex_);

    qInfo() << "生成自签名证书 - CN:" << commonName << "有效期:" << validDays << "天";

    // 注意：Qt本身不直接支持生成证书，需要调用OpenSSL命令行工具
    // 这里提供一个使用OpenSSL命令行的实现

    QTemporaryDir tempDir;
    if (!tempDir.isValid()) {
        emit certificateError("无法创建临时目录");
        return false;
    }

    QString certPath = tempDir.path() + "/cert.pem";
    QString keyPath = tempDir.path() + "/key.pem";

    // 检查openssl是否可用
    QProcess opensslCheck;
    opensslCheck.start("openssl", QStringList() << "version");
    if (!opensslCheck.waitForFinished(3000)) {
        qWarning() << "OpenSSL不可用，使用预生成的测试证书";
        // 创建一个简单的测试证书（仅用于测试）
        return createTestCertificate();
    }

    QString opensslVersion = opensslCheck.readAllStandardOutput().trimmed();
    qInfo() << "使用OpenSSL:" << opensslVersion;

    // 生成私钥
    QProcess genKey;
    genKey.start("openssl", QStringList()
        << "genrsa"
        << "-out" << keyPath
        << "2048");

    if (!genKey.waitForFinished(30000) || genKey.exitCode() != 0) {
        QString error = QString("生成私钥失败: %1").arg(QString(genKey.readAllStandardError()));
        qCritical() << error;
        emit certificateError(error);
        return false;
    }

    qInfo() << "私钥生成成功";

    // 生成自签名证书
    QProcess genCert;
    genCert.start("openssl", QStringList()
        << "req"
        << "-new"
        << "-x509"
        << "-key" << keyPath
        << "-out" << certPath
        << "-days" << QString::number(validDays)
        << "-subj" << QString("/CN=%1/O=VisionForge/C=CN").arg(commonName));

    if (!genCert.waitForFinished(30000) || genCert.exitCode() != 0) {
        QString error = QString("生成证书失败: %1").arg(QString(genCert.readAllStandardError()));
        qCritical() << error;
        emit certificateError(error);
        return false;
    }

    qInfo() << "自签名证书生成成功";

    // 加载生成的证书
    bool success = loadCertificate(certPath, keyPath);

    if (success) {
        qInfo() << "自签名证书已加载";
    }

    return success;
}

bool SecurityCertificateManager::createTestCertificate()
{
    // 注意：必须在锁外调用此方法，因为它会访问成员变量
    // 创建一个用于测试的证书和私钥（硬编码）
    // 注意：这仅用于开发测试，生产环境必须使用真实证书

    const char* testCert = R"(-----BEGIN CERTIFICATE-----
MIIDXTCCAkWgAwIBAgIJANxZqN0kVLmVMA0GCSqGSIb3DQEBCwUAMEUxCzAJBgNV
BAYTAkNOMRMwEQYDVQQIDApTb21lLVN0YXRlMSEwHwYDVQQKDBhJbnRlcm5ldCBX
aWRnaXRzIFB0eSBMdGQwHhcNMjUxMjIwMDAwMDAwWhcNMjYxMjIwMDAwMDAwWjBF
MQswCQYDVQQGEwJDTjETMBEGA1UECAwKU29tZS1TdGF0ZTEhMB8GA1UECgwYSW50
ZXJuZXQgV2lkZ2l0cyBQdHkgTHRkMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIB
CgKCAQEAwXbBQ2YxRQOqVsXGXNwLqmqW5wF2HZHdDQGh8k6NeL9HtpHJYLG9oGqQ
d+cPJNPH6F9zGqhPVKLNIz+MQgTZCxVm9qvRGm2HNKpqBl7gUcQRWGDUL9xCqrRw
vFqLqXmTXLlVPpHZdGLNvFqmGLqVqmGLqVqmGLqVqmGLqVqmGLqVqmGLqVqmGLqV
qmGLqVqmGLqVqmGLqVqmGLqVqmGLqVqmGLqVqmGLqVqmGLqVqmGLqVqmGLqVqmGL
qVqmGLqVqmGLqVqmGLqVqmGLqVqmGLqVqmGLqVqmGLqVqmGLqVqmGLqVqmGLqVqm
GLqVqmGLqVqmGLqVqmGLqVqmGLqVqmGLqVqmGLqVqmGLqVqmGLqVqmGLqVqmGLqV
qmGLqVqmGLqVqmGLqVqmGLqVqmGLqVqmGLqVqmGLqVqmGLqVqmGLqVqmGLqVqmGL
qVqmGLqVqmGLqVqmGLqVqmGLqVqmGLqVqmGLqVqmGLqVqmGLqVqmGLqVqmGLqVqm
GLqVqmGLqVqmGLqVqmGLqVqmGLqVqmGLqVqmGLqVqmGLqVqmGLqVqmGLqVqmGLqV
qmGLqVqmGLqVqmGLqVqmGLqVqmGLqVqmGLqVqmGLqVqmGLqVqmGLqVqmGLqVqmGL
-----END CERTIFICATE-----)";

    const char* testKey = R"(-----BEGIN RSA PRIVATE KEY-----
MIIEpAIBAAKCAQEAwXbBQ2YxRQOqVsXGXNwLqmqW5wF2HZHdDQGh8k6NeL9HtpHJ
YLG9oGqQd+cPJNPH6F9zGqhPVKLNIz+MQgTZCxVm9qvRGm2HNKpqBl7gUcQRWGDU
L9xCqrRwvFqLqXmTXLlVPpHZdGLNvFqmGLqVqmGLqVqmGLqVqmGLqVqmGLqVqmGL
-----END RSA PRIVATE KEY-----)";

    qWarning() << "=== 警告：使用测试证书，仅用于开发环境 ===";

    certificate_ = QSslCertificate(QByteArray(testCert), QSsl::Pem);
    privateKey_ = QSslKey(QByteArray(testKey), QSsl::Rsa, QSsl::Pem);

    if (certificate_.isNull() || privateKey_.isNull()) {
        emit certificateError("测试证书创建失败");
        return false;
    }

    isLoaded_ = true;
    createSslConfiguration();

    qWarning() << "测试证书已加载（仅用于开发）";
    return true;
}

QSslConfiguration SecurityCertificateManager::getSslConfiguration() const
{
    QMutexLocker locker(&mutex_);
    return sslConfig_;
}

bool SecurityCertificateManager::verifyCertificate() const
{
    QMutexLocker locker(&mutex_);
    return validateCertificate();
}

bool SecurityCertificateManager::isValid() const
{
    QMutexLocker locker(&mutex_);

    if (!isLoaded_) {
        return false;
    }

    QDateTime now = QDateTime::currentDateTime();
    return now >= certificate_.effectiveDate() && now <= certificate_.expiryDate();
}

bool SecurityCertificateManager::isLoaded() const
{
    QMutexLocker locker(&mutex_);
    return isLoaded_;
}

QSslCertificate SecurityCertificateManager::certificate() const
{
    QMutexLocker locker(&mutex_);
    return certificate_;
}

QSslKey SecurityCertificateManager::privateKey() const
{
    QMutexLocker locker(&mutex_);
    return privateKey_;
}

CertificateInfo SecurityCertificateManager::getCertificateInfo() const
{
    QMutexLocker locker(&mutex_);

    CertificateInfo info;

    if (!isLoaded_) {
        return info;
    }

    // 提取证书信息
    auto subjectInfo = certificate_.subjectInfo(QSslCertificate::CommonName);
    info.commonName = subjectInfo.isEmpty() ? "" : subjectInfo.first();

    auto orgInfo = certificate_.subjectInfo(QSslCertificate::Organization);
    info.organization = orgInfo.isEmpty() ? "" : orgInfo.first();

    auto countryInfo = certificate_.subjectInfo(QSslCertificate::CountryName);
    info.country = countryInfo.isEmpty() ? "" : countryInfo.first();

    info.notBefore = certificate_.effectiveDate();
    info.notAfter = certificate_.expiryDate();

    auto serialInfo = certificate_.serialNumber();
    info.serialNumber = QString::fromUtf8(serialInfo.toHex());

    auto issuerInfo = certificate_.issuerInfo(QSslCertificate::CommonName);
    info.issuer = issuerInfo.isEmpty() ? "" : issuerInfo.first();

    // 计算剩余天数
    QDateTime now = QDateTime::currentDateTime();
    info.daysUntilExpiry = now.daysTo(info.notAfter);

    // 验证状态
    info.isValid = isValid();
    info.isSelfSigned = (info.commonName == info.issuer);

    return info;
}

QDateTime SecurityCertificateManager::expiryDate() const
{
    QMutexLocker locker(&mutex_);
    return certificate_.expiryDate();
}

int SecurityCertificateManager::daysUntilExpiry() const
{
    QMutexLocker locker(&mutex_);
    return QDateTime::currentDateTime().daysTo(certificate_.expiryDate());
}

bool SecurityCertificateManager::isExpiringSoon(int warningDays) const
{
    return daysUntilExpiry() <= warningDays && daysUntilExpiry() > 0;
}

bool SecurityCertificateManager::saveCertificate(const QString& certPath,
                                                const QString& keyPath) const
{
    QMutexLocker locker(&mutex_);

    if (!isLoaded_) {
        qWarning() << "没有已加载的证书可以保存";
        return false;
    }

    // 保存证书
    QFile certFile(certPath);
    if (!certFile.open(QIODevice::WriteOnly)) {
        qCritical() << "无法写入证书文件:" << certPath;
        return false;
    }

    certFile.write(certificate_.toPem());
    certFile.close();

    // 保存私钥
    QFile keyFile(keyPath);
    if (!keyFile.open(QIODevice::WriteOnly)) {
        qCritical() << "无法写入私钥文件:" << keyPath;
        return false;
    }

    keyFile.write(privateKey_.toPem());
    keyFile.close();

    qInfo() << "证书已保存到:" << certPath;
    qInfo() << "私钥已保存到:" << keyPath;

    return true;
}

void SecurityCertificateManager::clear()
{
    QMutexLocker locker(&mutex_);

    certificate_.clear();
    privateKey_.clear();
    caCertificates_.clear();
    sslConfig_ = QSslConfiguration();
    isLoaded_ = false;

    qInfo() << "证书已清除";
}

bool SecurityCertificateManager::validateCertificate() const
{
    if (certificate_.isNull()) {
        qWarning() << "证书为空";
        return false;
    }

    if (privateKey_.isNull()) {
        qWarning() << "私钥为空";
        return false;
    }

    // 检查证书是否过期
    QDateTime now = QDateTime::currentDateTime();
    if (now < certificate_.effectiveDate()) {
        qWarning() << "证书尚未生效";
        return false;
    }

    if (now > certificate_.expiryDate()) {
        qWarning() << "证书已过期";
        // 注意：不能在const方法中emit信号
        return false;
    }

    return true;
}

void SecurityCertificateManager::createSslConfiguration()
{
    sslConfig_ = QSslConfiguration::defaultConfiguration();

    // 设置证书和私钥
    sslConfig_.setLocalCertificate(certificate_);
    sslConfig_.setPrivateKey(privateKey_);

    // 设置CA证书链
    if (!caCertificates_.isEmpty()) {
        sslConfig_.setCaCertificates(caCertificates_);
    }

    // 设置SSL协议版本
    sslConfig_.setProtocol(config_.sslProtocol);

    // 设置对等验证模式
    sslConfig_.setPeerVerifyMode(QSslSocket::VerifyNone);  // 服务器模式不验证客户端

    qInfo() << "SSL配置已创建";
}

void SecurityCertificateManager::checkExpiry()
{
    int days = daysUntilExpiry();

    if (days < 0) {
        qCritical() << "证书已过期";
        emit certificateExpired();
    } else if (days <= 30) {
        qWarning() << "证书即将过期，剩余天数:" << days;
        emit certificateExpiringSoon(days);
    }
}

QSslKey SecurityCertificateManager::generatePrivateKey()
{
    // Qt不直接支持密钥生成，需要使用OpenSSL
    // 这里返回空，实际生成在generateSelfSignedCertificate中通过OpenSSL完成
    return QSslKey();
}

QSslCertificate SecurityCertificateManager::createSelfSignedCert(const QSslKey& privateKey,
                                                                 const QString& commonName,
                                                                 int validDays)
{
    // Qt不直接支持证书生成，需要使用OpenSSL
    // 这里返回空，实际生成在generateSelfSignedCertificate中通过OpenSSL完成
    Q_UNUSED(privateKey);
    Q_UNUSED(commonName);
    Q_UNUSED(validDays);
    return QSslCertificate();
}

} // namespace Remote
} // namespace VisionForge
