/**
 * @file ConfigManager.h
 * @brief 配置管理器
 * @author VisionForge Team
 * @date 2025-12-14
 */

#pragma once

#include <QString>
#include <QVariant>
#include <QSettings>

namespace VisionForge {
namespace Base {

/**
 * @brief 配置管理器（单例）
 *
 * 核心功能：
 * - 配置读写：键值对方式存储配置
 * - 持久化：自动保存到INI文件
 * - 类型安全：使用QVariant支持多种类型
 */
class ConfigManager {
public:
    /**
     * @brief 获取单例实例
     */
    static ConfigManager& instance();

    /**
     * @brief 读取配置值
     * @param key 配置键
     * @param defaultValue 默认值
     * @return 配置值
     */
    QVariant getValue(const QString& key,
                     const QVariant& defaultValue = QVariant()) const;

    /**
     * @brief 写入配置值
     * @param key 配置键
     * @param value 配置值
     */
    void setValue(const QString& key, const QVariant& value);

    /**
     * @brief 保存配置到文件
     * @return 是否成功
     */
    bool save();

    /**
     * @brief 从文件加载配置
     * @return 是否成功
     */
    bool load();

    /**
     * @brief 获取配置文件路径
     */
    QString configFilePath() const;

    /**
     * @brief 设置配置文件路径
     * @param path 配置文件路径
     */
    void setConfigFilePath(const QString& path);

    /**
     * @brief 检查键是否存在
     * @param key 配置键
     * @return true如果存在
     */
    bool contains(const QString& key) const;

    /**
     * @brief 删除配置项
     * @param key 配置键
     */
    void remove(const QString& key);

    /**
     * @brief 清空所有配置
     */
    void clear();

    /**
     * @brief 获取所有键
     * @return 键列表
     */
    QStringList allKeys() const;

private:
    // 私有构造函数（单例）
    ConfigManager();
    ~ConfigManager();

    // 禁止拷贝
    ConfigManager(const ConfigManager&) = delete;
    ConfigManager& operator=(const ConfigManager&) = delete;

    /**
     * @brief 获取默认配置文件路径
     * @return 默认路径
     */
    QString getDefaultConfigPath() const;

private:
    QString configPath_;        // 配置文件路径
    QSettings* settings_;       // Qt配置对象
};

} // namespace Base
} // namespace VisionForge
