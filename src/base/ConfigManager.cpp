/**
 * @file ConfigManager.cpp
 * @brief 配置管理器实现
 * @author VisionForge Team
 * @date 2025-12-14
 *
 * @details
 * 本文件实现ConfigManager类的所有成员函数。
 *
 * ## 实现细节
 *
 * ### 配置文件位置
 * 默认配置文件存储在系统标准应用数据目录：
 * - Windows: %APPDATA%/VisionForge Pro/config.ini
 * - Linux: ~/.local/share/VisionForge Pro/config.ini
 * - macOS: ~/Library/Application Support/VisionForge Pro/config.ini
 *
 * ### INI文件格式
 * 使用Qt的INI格式，支持分组：
 * @code
 * [Camera]
 * Exposure=1000
 * Gain=1.5
 *
 * [PLC]
 * IP=192.168.1.100
 * Port=502
 * @endcode
 *
 * ### 信号机制
 * - valueChanged：仅在值实际发生变化时发出
 * - saved/loaded：配置同步完成后发出
 */

#include "base/ConfigManager.h"
#include <QCoreApplication>
#include <QStandardPaths>
#include <QDir>
#include <QFileInfo>

namespace VisionForge {
namespace Base {

ConfigManager::ConfigManager()
    : QObject(nullptr)  // 单例不需要父对象
{
    configPath_ = getDefaultConfigPath();
    settings_ = std::make_unique<QSettings>(configPath_, QSettings::IniFormat);
}

ConfigManager::~ConfigManager()
{
    if (settings_) {
        settings_->sync();
        // unique_ptr自动释放，无需delete
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
        QVariant oldValue = settings_->value(key);
        settings_->setValue(key, value);

        // 仅在值发生变化时发出信号
        if (oldValue != value) {
            emit valueChanged(key, value);
        }
    }
}

bool ConfigManager::save()
{
    if (settings_) {
        settings_->sync();
        bool success = (settings_->status() == QSettings::NoError);
        if (success) {
            emit saved();
        }
        return success;
    }
    return false;
}

bool ConfigManager::load()
{
    if (settings_) {
        settings_->sync();
        bool success = (settings_->status() == QSettings::NoError);
        if (success) {
            emit loaded();
        }
        return success;
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
    }

    // 创建新配置
    configPath_ = path;

    // 确保目录存在
    QFileInfo fileInfo(configPath_);
    QDir dir = fileInfo.absoluteDir();
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    settings_ = std::make_unique<QSettings>(configPath_, QSettings::IniFormat);
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
