/**
 * @file PluginManager.cpp
 * @brief 插件管理系统实现
 * @author VisionForge Team
 * @date 2025-12-19
 */

#include "base/PluginManager.h"
#include "base/Logger.h"

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>
#include <algorithm>

namespace VisionForge {
namespace Base {

PluginManager::PluginManager()
    : QObject(nullptr)
    , initialized_(false)
{
}

PluginManager::~PluginManager()
{
    shutdown();
}

PluginManager& PluginManager::instance()
{
    static PluginManager instance;
    return instance;
}

void PluginManager::initialize(const QStringList& pluginDirs)
{
    if (initialized_) return;

    // 设置配置文件路径
    configPath_ = QCoreApplication::applicationDirPath() + "/config/plugins.json";

    // 添加默认插件路径
    QString appDir = QCoreApplication::applicationDirPath();
    pluginPaths_ << appDir + "/plugins";
    pluginPaths_ << appDir + "/plugins/algorithms";
    pluginPaths_ << appDir + "/plugins/cameras";
    pluginPaths_ << appDir + "/plugins/communications";

    // 添加自定义路径
    for (const QString& dir : pluginDirs) {
        if (!pluginPaths_.contains(dir)) {
            pluginPaths_ << dir;
        }
    }

    // 加载配置
    loadPluginConfigs();

    initialized_ = true;
    LOG_INFO(tr("插件管理器初始化完成"));
}

void PluginManager::shutdown()
{
    if (!initialized_) return;

    // 保存配置
    savePluginConfigs();

    // 卸载所有插件
    unloadAllPlugins();

    plugins_.clear();
    pluginPaths_.clear();
    initialized_ = false;

    LOG_INFO(tr("插件管理器已关闭"));
}

int PluginManager::scanPlugins()
{
    int foundCount = 0;

    for (const QString& path : pluginPaths_) {
        QDir dir(path);
        if (!dir.exists()) {
            continue;
        }

        // 扫描动态库文件
#ifdef Q_OS_WIN
        QStringList filters = {"*.dll"};
#elif defined(Q_OS_MACOS)
        QStringList filters = {"*.dylib", "*.bundle"};
#else
        QStringList filters = {"*.so"};
#endif

        QFileInfoList files = dir.entryInfoList(filters, QDir::Files);
        for (const QFileInfo& fileInfo : files) {
            QString filePath = fileInfo.absoluteFilePath();

            // 尝试加载元信息
            PluginMetaData metaData;
            if (loadPluginMetaData(filePath, metaData)) {
                // 检查是否已存在
                if (!plugins_.contains(metaData.id)) {
                    PluginInfo info;
                    info.metaData = metaData;
                    info.state = disabledPlugins_.contains(metaData.id) ?
                                 PluginState::Disabled : PluginState::NotLoaded;

                    plugins_[metaData.id] = info;
                    foundCount++;

                    LOG_DEBUG(tr("发现插件: %1 (%2)")
                             .arg(metaData.name).arg(metaData.version));
                }
            }
        }
    }

    emit pluginsScanCompleted(foundCount);
    LOG_INFO(tr("插件扫描完成，发现 %1 个插件").arg(foundCount));

    return foundCount;
}

void PluginManager::addPluginPath(const QString& path)
{
    if (!pluginPaths_.contains(path)) {
        pluginPaths_ << path;
    }
}

bool PluginManager::loadPlugin(const QString& pluginId)
{
    if (!plugins_.contains(pluginId)) {
        LOG_WARNING(tr("插件不存在: %1").arg(pluginId));
        return false;
    }

    PluginInfo& info = plugins_[pluginId];

    if (info.state == PluginState::Loaded || info.state == PluginState::Active) {
        return true;  // 已加载
    }

    if (info.state == PluginState::Disabled) {
        LOG_WARNING(tr("插件已禁用: %1").arg(pluginId));
        return false;
    }

    // 检查依赖
    if (!checkDependencies(pluginId)) {
        info.state = PluginState::Error;
        info.errorString = tr("依赖检查失败");
        emit pluginError(pluginId, info.errorString);
        return false;
    }

    // 创建加载器
    info.loader = new QPluginLoader(info.metaData.filePath);

    // 加载插件
    QObject* pluginObject = info.loader->instance();
    if (!pluginObject) {
        info.state = PluginState::Error;
        info.errorString = info.loader->errorString();
        LOG_ERROR(tr("加载插件失败: %1 - %2")
                 .arg(pluginId).arg(info.errorString));
        emit pluginError(pluginId, info.errorString);

        delete info.loader;
        info.loader = nullptr;
        return false;
    }

    // 转换为插件接口
    info.instance = qobject_cast<IPlugin*>(pluginObject);
    if (!info.instance) {
        info.state = PluginState::Error;
        info.errorString = tr("无效的插件接口");
        LOG_ERROR(tr("插件接口无效: %1").arg(pluginId));
        emit pluginError(pluginId, info.errorString);

        info.loader->unload();
        delete info.loader;
        info.loader = nullptr;
        return false;
    }

    // 初始化插件
    if (!info.instance->initialize()) {
        info.state = PluginState::Error;
        info.errorString = tr("插件初始化失败");
        LOG_ERROR(tr("插件初始化失败: %1").arg(pluginId));
        emit pluginError(pluginId, info.errorString);

        info.instance = nullptr;
        info.loader->unload();
        delete info.loader;
        info.loader = nullptr;
        return false;
    }

    info.state = PluginState::Active;
    LOG_INFO(tr("插件加载成功: %1 v%2")
            .arg(info.metaData.name).arg(info.metaData.version));

    emit pluginLoaded(pluginId);
    emit pluginStateChanged(pluginId, PluginState::Active);

    return true;
}

bool PluginManager::unloadPlugin(const QString& pluginId)
{
    if (!plugins_.contains(pluginId)) {
        return false;
    }

    PluginInfo& info = plugins_[pluginId];

    if (info.state == PluginState::NotLoaded || info.state == PluginState::Disabled) {
        return true;  // 未加载
    }

    // 关闭插件
    if (info.instance) {
        info.instance->shutdown();
        info.instance = nullptr;
    }

    // 卸载动态库
    if (info.loader) {
        info.loader->unload();
        delete info.loader;
        info.loader = nullptr;
    }

    info.state = PluginState::NotLoaded;
    LOG_INFO(tr("插件已卸载: %1").arg(info.metaData.name));

    emit pluginUnloaded(pluginId);
    emit pluginStateChanged(pluginId, PluginState::NotLoaded);

    return true;
}

bool PluginManager::enablePlugin(const QString& pluginId)
{
    if (!plugins_.contains(pluginId)) {
        return false;
    }

    PluginInfo& info = plugins_[pluginId];

    if (info.state != PluginState::Disabled) {
        return true;
    }

    disabledPlugins_.removeAll(pluginId);
    info.state = PluginState::NotLoaded;

    emit pluginStateChanged(pluginId, PluginState::NotLoaded);
    return true;
}

bool PluginManager::disablePlugin(const QString& pluginId)
{
    if (!plugins_.contains(pluginId)) {
        return false;
    }

    // 先卸载
    unloadPlugin(pluginId);

    PluginInfo& info = plugins_[pluginId];
    info.state = PluginState::Disabled;

    if (!disabledPlugins_.contains(pluginId)) {
        disabledPlugins_ << pluginId;
    }

    emit pluginStateChanged(pluginId, PluginState::Disabled);
    return true;
}

void PluginManager::loadAllPlugins()
{
    // 按依赖顺序排序
    QStringList sortedIds = sortPluginsByDependency();

    for (const QString& id : sortedIds) {
        if (plugins_[id].state != PluginState::Disabled) {
            loadPlugin(id);
        }
    }
}

void PluginManager::unloadAllPlugins()
{
    // 逆序卸载（依赖关系）
    QStringList sortedIds = sortPluginsByDependency();
    std::reverse(sortedIds.begin(), sortedIds.end());

    for (const QString& id : sortedIds) {
        unloadPlugin(id);
    }
}

QVector<PluginInfo> PluginManager::allPlugins() const
{
    return QVector<PluginInfo>(plugins_.values().begin(), plugins_.values().end());
}

QVector<PluginInfo> PluginManager::pluginsByType(PluginType type) const
{
    QVector<PluginInfo> result;
    for (const auto& info : plugins_) {
        if (info.metaData.type == type) {
            result.append(info);
        }
    }
    return result;
}

const PluginInfo* PluginManager::getPluginInfo(const QString& pluginId) const
{
    auto it = plugins_.find(pluginId);
    if (it != plugins_.end()) {
        return &it.value();
    }
    return nullptr;
}

IPlugin* PluginManager::getPlugin(const QString& pluginId) const
{
    const PluginInfo* info = getPluginInfo(pluginId);
    return info ? info->instance : nullptr;
}

IAlgorithmPlugin* PluginManager::getAlgorithmPlugin(const QString& pluginId) const
{
    IPlugin* plugin = getPlugin(pluginId);
    return plugin ? dynamic_cast<IAlgorithmPlugin*>(plugin) : nullptr;
}

ICameraPlugin* PluginManager::getCameraPlugin(const QString& pluginId) const
{
    IPlugin* plugin = getPlugin(pluginId);
    return plugin ? dynamic_cast<ICameraPlugin*>(plugin) : nullptr;
}

ICommPlugin* PluginManager::getCommPlugin(const QString& pluginId) const
{
    IPlugin* plugin = getPlugin(pluginId);
    return plugin ? dynamic_cast<ICommPlugin*>(plugin) : nullptr;
}

bool PluginManager::isPluginLoaded(const QString& pluginId) const
{
    const PluginInfo* info = getPluginInfo(pluginId);
    return info && (info->state == PluginState::Loaded ||
                    info->state == PluginState::Active);
}

int PluginManager::loadedPluginCount() const
{
    int count = 0;
    for (const auto& info : plugins_) {
        if (info.state == PluginState::Loaded ||
            info.state == PluginState::Active) {
            count++;
        }
    }
    return count;
}

bool PluginManager::savePluginConfigs()
{
    QJsonObject root;

    // 保存禁用列表
    QJsonArray disabled;
    for (const QString& id : disabledPlugins_) {
        disabled.append(id);
    }
    root["disabledPlugins"] = disabled;

    // 保存各插件配置
    QJsonObject pluginConfigs;
    for (auto it = plugins_.begin(); it != plugins_.end(); ++it) {
        if (it->instance) {
            QJsonObject cfg = it->instance->saveConfig();
            if (!cfg.isEmpty()) {
                pluginConfigs[it.key()] = cfg;
            }
        }
    }
    root["pluginConfigs"] = pluginConfigs;

    // 写入文件
    QFileInfo fileInfo(configPath_);
    QDir dir = fileInfo.absoluteDir();
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    QFile file(configPath_);
    if (!file.open(QIODevice::WriteOnly)) {
        LOG_ERROR(tr("无法保存插件配置: %1").arg(configPath_));
        return false;
    }

    QJsonDocument doc(root);
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();

    return true;
}

bool PluginManager::loadPluginConfigs()
{
    QFile file(configPath_);
    if (!file.exists() || !file.open(QIODevice::ReadOnly)) {
        return false;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(data, &error);
    if (error.error != QJsonParseError::NoError) {
        LOG_WARNING(tr("解析插件配置失败: %1").arg(error.errorString()));
        return false;
    }

    QJsonObject root = doc.object();

    // 加载禁用列表
    QJsonArray disabled = root["disabledPlugins"].toArray();
    disabledPlugins_.clear();
    for (const auto& value : disabled) {
        disabledPlugins_ << value.toString();
    }

    // 插件配置将在插件加载后应用
    return true;
}

void PluginManager::setDisabledPlugins(const QStringList& plugins)
{
    disabledPlugins_ = plugins;
}

QString PluginManager::pluginTypeName(PluginType type)
{
    switch (type) {
    case PluginType::Algorithm:      return tr("算法插件");
    case PluginType::Camera:         return tr("相机插件");
    case PluginType::Communication:  return tr("通信插件");
    case PluginType::UI:             return tr("界面插件");
    case PluginType::Export:         return tr("导出插件");
    default:                         return tr("未知类型");
    }
}

QString PluginManager::pluginStateName(PluginState state)
{
    switch (state) {
    case PluginState::NotLoaded:  return tr("未加载");
    case PluginState::Loaded:     return tr("已加载");
    case PluginState::Active:     return tr("已激活");
    case PluginState::Error:      return tr("错误");
    case PluginState::Disabled:   return tr("已禁用");
    default:                      return tr("未知");
    }
}

bool PluginManager::loadPluginMetaData(const QString& filePath, PluginMetaData& metaData)
{
    QPluginLoader loader(filePath);
    QJsonObject metaDataObj = loader.metaData();

    if (metaDataObj.isEmpty()) {
        return false;
    }

    // 检查是否是Qt插件
    if (!metaDataObj.contains("MetaData")) {
        return false;
    }

    QJsonObject pluginMeta = metaDataObj["MetaData"].toObject();

    // 检查是否是VisionForge插件
    if (!pluginMeta.contains("id") || !pluginMeta.contains("name")) {
        return false;
    }

    metaData.id = pluginMeta["id"].toString();
    metaData.name = pluginMeta["name"].toString();
    metaData.version = pluginMeta.contains("version") ? pluginMeta["version"].toString() : "1.0.0";
    metaData.author = pluginMeta.contains("author") ? pluginMeta["author"].toString() : QString();
    metaData.description = pluginMeta.contains("description") ? pluginMeta["description"].toString() : QString();
    metaData.website = pluginMeta.contains("website") ? pluginMeta["website"].toString() : QString();
    metaData.license = pluginMeta.contains("license") ? pluginMeta["license"].toString() : QString();
    metaData.minAppVersion = pluginMeta.contains("minAppVersion") ? pluginMeta["minAppVersion"].toString() : QString();
    metaData.filePath = filePath;

    // 解析类型
    QString typeStr = pluginMeta.contains("type") ? pluginMeta["type"].toString().toLower() : "unknown";
    if (typeStr == "algorithm") metaData.type = PluginType::Algorithm;
    else if (typeStr == "camera") metaData.type = PluginType::Camera;
    else if (typeStr == "communication") metaData.type = PluginType::Communication;
    else if (typeStr == "ui") metaData.type = PluginType::UI;
    else if (typeStr == "export") metaData.type = PluginType::Export;
    else metaData.type = PluginType::Unknown;

    // 解析依赖
    QJsonArray deps = pluginMeta.contains("dependencies") ? pluginMeta["dependencies"].toArray() : QJsonArray();
    for (const auto& dep : deps) {
        metaData.dependencies << dep.toString();
    }

    return true;
}

bool PluginManager::checkDependencies(const QString& pluginId) const
{
    const PluginInfo* info = getPluginInfo(pluginId);
    if (!info) return false;

    for (const QString& depId : info->metaData.dependencies) {
        if (!isPluginLoaded(depId)) {
            LOG_WARNING(tr("插件 %1 依赖 %2 未加载")
                       .arg(pluginId).arg(depId));
            return false;
        }
    }

    return true;
}

QStringList PluginManager::sortPluginsByDependency() const
{
    QStringList sorted;
    QSet<QString> visited;
    QSet<QString> inProgress;

    std::function<bool(const QString&)> visit = [&](const QString& id) -> bool {
        if (visited.contains(id)) return true;
        if (inProgress.contains(id)) {
            LOG_WARNING(tr("检测到插件循环依赖: %1").arg(id));
            return false;  // 循环依赖
        }

        inProgress.insert(id);

        const PluginInfo* info = getPluginInfo(id);
        if (info) {
            for (const QString& depId : info->metaData.dependencies) {
                if (plugins_.contains(depId)) {
                    if (!visit(depId)) return false;
                }
            }
        }

        inProgress.remove(id);
        visited.insert(id);
        sorted.append(id);
        return true;
    };

    for (auto it = plugins_.begin(); it != plugins_.end(); ++it) {
        visit(it.key());
    }

    return sorted;
}

} // namespace Base
} // namespace VisionForge
