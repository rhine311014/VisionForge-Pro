/**
 * @file ConfigManager.h
 * @brief 配置管理器 - 应用程序配置的集中管理
 * @author VisionForge Team
 * @date 2025-12-14
 *
 * @details
 * 本文件实现了VisionForge项目的配置管理系统，提供统一的配置读写接口。
 *
 * ## 设计模式
 * - **单例模式 (Singleton)**：全局唯一的配置管理器实例
 * - **观察者模式 (Observer)**：配置变更时通过Qt信号通知订阅者
 *
 * ## 核心功能
 * - 键值对存储：使用QString作为键，QVariant作为值
 * - 持久化存储：配置自动保存到INI格式文件
 * - 类型安全：QVariant支持多种数据类型
 * - 变更通知：配置变化时发出valueChanged信号
 * - 分组管理：支持使用"/"分隔的键实现分组
 *
 * ## 线程安全
 * - QSettings内部实现了线程安全
 * - 配置变更信号可跨线程发送
 * - 建议在主线程进行配置操作
 *
 * ## 内存管理
 * - QSettings使用std::unique_ptr智能指针管理
 * - 析构时自动同步配置到磁盘
 *
 * ## 使用示例
 * @code
 * // 读写配置
 * ConfigManager::instance().setValue("Camera/Exposure", 1000);
 * int exposure = ConfigManager::instance().getValue("Camera/Exposure", 500).toInt();
 *
 * // 监听配置变化
 * connect(&ConfigManager::instance(), &ConfigManager::valueChanged,
 *         [](const QString& key, const QVariant& value) {
 *             qDebug() << "Config changed:" << key << "=" << value;
 *         });
 *
 * // 保存配置
 * ConfigManager::instance().save();
 * @endcode
 */

#pragma once

#include <QString>
#include <QVariant>
#include <QSettings>
#include <QObject>
#include <memory>

namespace VisionForge {
namespace Base {

/**
 * @brief 配置管理器（单例）
 *
 * 核心功能：
 * - 配置读写：键值对方式存储配置
 * - 持久化：自动保存到INI文件
 * - 类型安全：使用QVariant支持多种类型
 * - 变更通知：配置变化时发出信号
 */
class ConfigManager : public QObject {
    Q_OBJECT

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

signals:
    /**
     * @brief 配置值变更信号
     * @param key 配置键
     * @param value 新值
     */
    void valueChanged(const QString& key, const QVariant& value);

    /**
     * @brief 配置保存完成信号
     */
    void saved();

    /**
     * @brief 配置加载完成信号
     */
    void loaded();

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
    QString configPath_;                        // 配置文件路径
    std::unique_ptr<QSettings> settings_;       // Qt配置对象（智能指针）
};

} // namespace Base
} // namespace VisionForge
