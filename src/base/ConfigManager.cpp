/**
 * @file ConfigManager.cpp
 * @brief 配置管理器实现
 */

#include "base/ConfigManager.h"
#include <QCoreApplication>
#include <QStandardPaths>
#include <QDir>
#include <QFileInfo>

namespace VisionForge {
namespace Base {

ConfigManager::ConfigManager()
    : settings_(nullptr)
{
    configPath_ = getDefaultConfigPath();
    settings_ = new QSettings(configPath_, QSettings::IniFormat);
}

ConfigManager::~ConfigManager()
{
    if (settings_) {
        settings_->sync();
        delete settings_;
    }
}

ConfigManager& ConfigManager::instance()
{
    static ConfigManager instance;
    return instance;
}

QVariant ConfigManager::getValue(const QString& key, const QVariant& defaultValue) const
{
    if (settings_) {
        return settings_->value(key, defaultValue);
    }
    return defaultValue;
}

void ConfigManager::setValue(const QString& key, const QVariant& value)
{
    if (settings_) {
        settings_->setValue(key, value);
    }
}

bool ConfigManager::save()
{
    if (settings_) {
        settings_->sync();
        return settings_->status() == QSettings::NoError;
    }
    return false;
}

bool ConfigManager::load()
{
    if (settings_) {
        settings_->sync();
        return settings_->status() == QSettings::NoError;
    }
    return false;
}

QString ConfigManager::configFilePath() const
{
    return configPath_;
}

void ConfigManager::setConfigFilePath(const QString& path)
{
    if (path == configPath_) {
        return;
    }

    // 保存旧配置
    if (settings_) {
        settings_->sync();
        delete settings_;
    }

    // 创建新配置
    configPath_ = path;

    // 确保目录存在
    QFileInfo fileInfo(configPath_);
    QDir dir = fileInfo.absoluteDir();
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    settings_ = new QSettings(configPath_, QSettings::IniFormat);
}

bool ConfigManager::contains(const QString& key) const
{
    if (settings_) {
        return settings_->contains(key);
    }
    return false;
}

void ConfigManager::remove(const QString& key)
{
    if (settings_) {
        settings_->remove(key);
    }
}

void ConfigManager::clear()
{
    if (settings_) {
        settings_->clear();
    }
}

QStringList ConfigManager::allKeys() const
{
    if (settings_) {
        return settings_->allKeys();
    }
    return QStringList();
}

QString ConfigManager::getDefaultConfigPath() const
{
    // 获取应用程序数据目录
    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

    // 创建目录（如果不存在）
    QDir dir(appDataPath);
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    // 返回配置文件路径
    return dir.filePath("config.ini");
}

} // namespace Base
} // namespace VisionForge
