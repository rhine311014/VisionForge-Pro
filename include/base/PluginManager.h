/**
 * @file PluginManager.h
 * @brief 插件管理系统
 * @details 支持动态加载和管理扩展插件
 * @author VisionForge Team
 * @date 2025-12-19
 */

#pragma once

#include <QObject>
#include <QString>
#include <QStringList>
#include <QMap>
#include <QVector>
#include <QPluginLoader>
#include <QJsonObject>
#include <memory>

namespace VisionForge {
namespace Base {

/**
 * @brief 插件类型枚举
 */
enum class PluginType {
    Unknown,        // 未知类型
    Algorithm,      // 算法插件
    Camera,         // 相机插件
    Communication,  // 通信插件
    UI,             // UI插件
    Export          // 导出插件
};

/**
 * @brief 插件状态枚举
 */
enum class PluginState {
    NotLoaded,      // 未加载
    Loaded,         // 已加载
    Active,         // 已激活
    Error,          // 错误状态
    Disabled        // 已禁用
};

/**
 * @brief 插件元信息结构
 */
struct PluginMetaData {
    QString id;                 // 插件ID（唯一标识）
    QString name;               // 显示名称
    QString version;            // 版本号
    QString author;             // 作者
    QString description;        // 描述
    QString website;            // 网站
    QString license;            // 许可证
    PluginType type;            // 插件类型
    QStringList dependencies;   // 依赖的其他插件ID
    QString minAppVersion;      // 最低应用版本要求
    QString filePath;           // 插件文件路径

    PluginMetaData()
        : type(PluginType::Unknown)
    {}
};

/**
 * @brief 插件接口基类
 *
 * 所有插件必须实现此接口
 */
class IPlugin
{
public:
    virtual ~IPlugin() = default;

    /**
     * @brief 获取插件元信息
     */
    virtual PluginMetaData metaData() const = 0;

    /**
     * @brief 初始化插件
     * @return true如果成功
     */
    virtual bool initialize() = 0;

    /**
     * @brief 关闭插件
     */
    virtual void shutdown() = 0;

    /**
     * @brief 获取插件配置界面（如果有）
     * @return 配置界面指针，无则返回nullptr
     */
    virtual QWidget* configWidget() { return nullptr; }

    /**
     * @brief 加载插件配置
     * @param config 配置JSON
     */
    virtual void loadConfig(const QJsonObject& config) { Q_UNUSED(config) }

    /**
     * @brief 保存插件配置
     * @return 配置JSON
     */
    virtual QJsonObject saveConfig() const { return QJsonObject(); }
};

/**
 * @brief 算法插件接口
 */
class IAlgorithmPlugin : public IPlugin
{
public:
    /**
     * @brief 获取提供的算法工具类型列表
     */
    virtual QStringList providedTools() const = 0;

    /**
     * @brief 创建算法工具实例
     * @param toolType 工具类型
     * @return 工具实例指针
     */
    virtual void* createTool(const QString& toolType) = 0;
};

/**
 * @brief 相机插件接口
 */
class ICameraPlugin : public IPlugin
{
public:
    /**
     * @brief 获取支持的相机类型列表
     */
    virtual QStringList supportedCameraTypes() const = 0;

    /**
     * @brief 枚举可用相机
     * @return 相机信息列表
     */
    virtual QStringList enumerateCameras() const = 0;

    /**
     * @brief 创建相机实例
     * @param cameraId 相机ID
     * @return 相机实例指针
     */
    virtual void* createCamera(const QString& cameraId) = 0;
};

/**
 * @brief 通信插件接口
 */
class ICommPlugin : public IPlugin
{
public:
    /**
     * @brief 获取支持的通信协议列表
     */
    virtual QStringList supportedProtocols() const = 0;

    /**
     * @brief 创建通信实例
     * @param protocol 协议名称
     * @return 通信实例指针
     */
    virtual void* createComm(const QString& protocol) = 0;
};

/**
 * @brief 插件信息（运行时）
 */
struct PluginInfo {
    PluginMetaData metaData;        // 元信息
    PluginState state;              // 状态
    IPlugin* instance;              // 插件实例
    QPluginLoader* loader;          // 加载器
    QString errorString;            // 错误信息

    PluginInfo()
        : state(PluginState::NotLoaded)
        , instance(nullptr)
        , loader(nullptr)
    {}
};

/**
 * @class PluginManager
 * @brief 插件管理器（单例）
 *
 * 功能：
 * - 扫描和发现插件
 * - 加载/卸载插件
 * - 管理插件生命周期
 * - 处理插件依赖
 */
class PluginManager : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief 获取单例实例
     */
    static PluginManager& instance();

    /**
     * @brief 初始化插件管理器
     * @param pluginDirs 插件搜索目录列表
     */
    void initialize(const QStringList& pluginDirs = QStringList());

    /**
     * @brief 关闭插件管理器
     */
    void shutdown();

    // ==================== 插件发现 ====================

    /**
     * @brief 扫描插件目录
     * @return 发现的插件数量
     */
    int scanPlugins();

    /**
     * @brief 添加插件搜索路径
     */
    void addPluginPath(const QString& path);

    /**
     * @brief 获取插件搜索路径列表
     */
    QStringList pluginPaths() const { return pluginPaths_; }

    // ==================== 插件管理 ====================

    /**
     * @brief 加载插件
     * @param pluginId 插件ID
     * @return true如果成功
     */
    bool loadPlugin(const QString& pluginId);

    /**
     * @brief 卸载插件
     * @param pluginId 插件ID
     * @return true如果成功
     */
    bool unloadPlugin(const QString& pluginId);

    /**
     * @brief 启用插件
     * @param pluginId 插件ID
     * @return true如果成功
     */
    bool enablePlugin(const QString& pluginId);

    /**
     * @brief 禁用插件
     * @param pluginId 插件ID
     * @return true如果成功
     */
    bool disablePlugin(const QString& pluginId);

    /**
     * @brief 加载所有已发现的插件
     */
    void loadAllPlugins();

    /**
     * @brief 卸载所有插件
     */
    void unloadAllPlugins();

    // ==================== 插件查询 ====================

    /**
     * @brief 获取所有插件信息
     */
    QVector<PluginInfo> allPlugins() const;

    /**
     * @brief 按类型获取插件列表
     * @param type 插件类型
     */
    QVector<PluginInfo> pluginsByType(PluginType type) const;

    /**
     * @brief 获取插件信息
     * @param pluginId 插件ID
     */
    const PluginInfo* getPluginInfo(const QString& pluginId) const;

    /**
     * @brief 获取插件实例
     * @param pluginId 插件ID
     */
    IPlugin* getPlugin(const QString& pluginId) const;

    /**
     * @brief 获取算法插件
     * @param pluginId 插件ID
     */
    IAlgorithmPlugin* getAlgorithmPlugin(const QString& pluginId) const;

    /**
     * @brief 获取相机插件
     * @param pluginId 插件ID
     */
    ICameraPlugin* getCameraPlugin(const QString& pluginId) const;

    /**
     * @brief 获取通信插件
     * @param pluginId 插件ID
     */
    ICommPlugin* getCommPlugin(const QString& pluginId) const;

    /**
     * @brief 检查插件是否已加载
     */
    bool isPluginLoaded(const QString& pluginId) const;

    /**
     * @brief 获取已加载插件数量
     */
    int loadedPluginCount() const;

    // ==================== 配置管理 ====================

    /**
     * @brief 保存插件配置
     */
    bool savePluginConfigs();

    /**
     * @brief 加载插件配置
     */
    bool loadPluginConfigs();

    /**
     * @brief 获取禁用的插件列表
     */
    QStringList disabledPlugins() const { return disabledPlugins_; }

    /**
     * @brief 设置禁用的插件列表
     */
    void setDisabledPlugins(const QStringList& plugins);

    // ==================== 工具方法 ====================

    /**
     * @brief 获取插件类型名称
     */
    static QString pluginTypeName(PluginType type);

    /**
     * @brief 获取插件状态名称
     */
    static QString pluginStateName(PluginState state);

signals:
    /**
     * @brief 插件加载信号
     */
    void pluginLoaded(const QString& pluginId);

    /**
     * @brief 插件卸载信号
     */
    void pluginUnloaded(const QString& pluginId);

    /**
     * @brief 插件状态改变信号
     */
    void pluginStateChanged(const QString& pluginId, PluginState state);

    /**
     * @brief 插件扫描完成信号
     */
    void pluginsScanCompleted(int count);

    /**
     * @brief 插件错误信号
     */
    void pluginError(const QString& pluginId, const QString& error);

private:
    PluginManager();
    ~PluginManager();
    PluginManager(const PluginManager&) = delete;
    PluginManager& operator=(const PluginManager&) = delete;

    /**
     * @brief 从文件加载插件元信息
     */
    bool loadPluginMetaData(const QString& filePath, PluginMetaData& metaData);

    /**
     * @brief 检查插件依赖
     */
    bool checkDependencies(const QString& pluginId) const;

    /**
     * @brief 按依赖顺序排序插件
     */
    QStringList sortPluginsByDependency() const;

private:
    QStringList pluginPaths_;                       // 插件搜索路径
    QMap<QString, PluginInfo> plugins_;             // 插件信息映射
    QStringList disabledPlugins_;                   // 禁用的插件列表
    QString configPath_;                            // 配置文件路径
    bool initialized_;
};

} // namespace Base
} // namespace VisionForge

// Qt插件接口宏
#define VisionForge_IPlugin_iid "com.visionforge.IPlugin/1.0"
Q_DECLARE_INTERFACE(VisionForge::Base::IPlugin, VisionForge_IPlugin_iid)

#define VisionForge_IAlgorithmPlugin_iid "com.visionforge.IAlgorithmPlugin/1.0"
Q_DECLARE_INTERFACE(VisionForge::Base::IAlgorithmPlugin, VisionForge_IAlgorithmPlugin_iid)

#define VisionForge_ICameraPlugin_iid "com.visionforge.ICameraPlugin/1.0"
Q_DECLARE_INTERFACE(VisionForge::Base::ICameraPlugin, VisionForge_ICameraPlugin_iid)

#define VisionForge_ICommPlugin_iid "com.visionforge.ICommPlugin/1.0"
Q_DECLARE_INTERFACE(VisionForge::Base::ICommPlugin, VisionForge_ICommPlugin_iid)

// 便捷宏
#define PLUGIN_MANAGER VisionForge::Base::PluginManager::instance()
