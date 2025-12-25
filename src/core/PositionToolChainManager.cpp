/**
 * @file PositionToolChainManager.cpp
 * @brief 位置级工具链管理器实现
 */

#include "core/PositionToolChainManager.h"
#include "core/StationConfig.h"
#include "core/MultiStationManager.h"
#include "algorithm/VisionTool.h"
#include "algorithm/ToolFactory.h"
#include "base/Logger.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>
#include <QDir>
#include <QStandardPaths>

namespace VisionForge {
namespace Core {

// ========== 单例实现 ==========

PositionToolChainManager& PositionToolChainManager::instance()
{
    static PositionToolChainManager instance;
    return instance;
}

PositionToolChainManager::PositionToolChainManager()
    : QObject(nullptr)
{
    // 设置配置基目录
    // 使用固定路径，确保主程序和配置工具使用相同的配置目录
    configBaseDir_ = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation)
                     + "/VisionForge Pro/stations";
}

PositionToolChainManager::~PositionToolChainManager()
{
    // 保存所有修改的工具链
    saveAll();

    // 清理所有工具链
    for (auto* chain : toolChains_) {
        cleanupTools(chain);
        delete chain;
    }
    toolChains_.clear();
}

// ========== 当前位置管理 ==========

bool PositionToolChainManager::setCurrentPosition(const QString& stationId, const QString& positionId)
{
    if (stationId.isEmpty() || positionId.isEmpty()) {
        return false;
    }

    // 如果位置没变，直接返回
    if (currentStationId_ == stationId && currentPositionId_ == positionId) {
        return true;
    }

    // 保存当前工具链（如果有修改）
    auto* currentChain = currentToolChain();
    if (currentChain && currentChain->modified) {
        saveCurrentToolChain();
    }

    // 切换到新位置
    QString oldStation = currentStationId_;
    QString oldPosition = currentPositionId_;
    currentStationId_ = stationId;
    currentPositionId_ = positionId;

    // 确保新位置的工具链存在
    getOrCreateToolChain(stationId, positionId);

    LOG_INFO(QString("切换位置: %1/%2 -> %3/%4")
             .arg(oldStation).arg(oldPosition)
             .arg(stationId).arg(positionId));

    emit currentPositionChanged(stationId, positionId);
    return true;
}

QString PositionToolChainManager::currentKey() const
{
    if (currentStationId_.isEmpty() || currentPositionId_.isEmpty()) {
        return QString();
    }
    return currentStationId_ + "_" + currentPositionId_;
}

// ========== 工具链访问 ==========

PositionToolChain* PositionToolChainManager::currentToolChain()
{
    QString key = currentKey();
    if (key.isEmpty()) {
        return nullptr;
    }
    return toolChains_.value(key, nullptr);
}

PositionToolChain* PositionToolChainManager::getToolChain(const QString& stationId, const QString& positionId)
{
    QString key = stationId + "_" + positionId;
    return toolChains_.value(key, nullptr);
}

QList<Algorithm::VisionTool*> PositionToolChainManager::currentTools()
{
    auto* chain = currentToolChain();
    return chain ? chain->tools : QList<Algorithm::VisionTool*>();
}

QList<Algorithm::VisionTool*> PositionToolChainManager::getTools(const QString& stationId, const QString& positionId)
{
    auto* chain = getToolChain(stationId, positionId);
    return chain ? chain->tools : QList<Algorithm::VisionTool*>();
}

// ========== 工具操作 ==========

void PositionToolChainManager::addTool(Algorithm::VisionTool* tool)
{
    auto* chain = currentToolChain();
    if (!chain || !tool) {
        return;
    }

    chain->tools.append(tool);
    chain->modified = true;

    LOG_DEBUG(QString("添加工具到 %1/%2: %3")
              .arg(currentStationId_).arg(currentPositionId_)
              .arg(tool->displayName()));

    emit toolChainChanged(currentStationId_, currentPositionId_);
}

void PositionToolChainManager::removeTool(Algorithm::VisionTool* tool)
{
    auto* chain = currentToolChain();
    if (!chain || !tool) {
        return;
    }

    int index = chain->tools.indexOf(tool);
    if (index >= 0) {
        chain->tools.removeAt(index);
        chain->modified = true;

        LOG_DEBUG(QString("移除工具从 %1/%2: %3")
                  .arg(currentStationId_).arg(currentPositionId_)
                  .arg(tool->toolName()));

        delete tool;
        emit toolChainChanged(currentStationId_, currentPositionId_);
    }
}

void PositionToolChainManager::clearCurrentToolChain()
{
    auto* chain = currentToolChain();
    if (!chain) {
        return;
    }

    cleanupTools(chain);
    chain->modified = true;

    LOG_INFO(QString("清空工具链: %1/%2")
             .arg(currentStationId_).arg(currentPositionId_));

    emit toolChainChanged(currentStationId_, currentPositionId_);
}

void PositionToolChainManager::setCurrentTools(const QList<Algorithm::VisionTool*>& tools)
{
    auto* chain = currentToolChain();
    if (!chain) {
        return;
    }

    // 不删除旧工具，只替换引用（工具由外部管理）
    chain->tools = tools;
    chain->modified = true;

    emit toolChainChanged(currentStationId_, currentPositionId_);
}

// ========== 工位初始化 ==========

void PositionToolChainManager::initializeStation(StationConfig* station)
{
    if (!station) {
        return;
    }

    // 为工位的每个位置创建工具链
    for (const auto& binding : station->positionBindings) {
        getOrCreateToolChain(station->id, binding.positionId);
    }

    // 尝试加载已有的工具链配置
    loadStationToolChains(station->id);

    LOG_INFO(QString("初始化工位工具链: %1, %2 个位置")
             .arg(station->id).arg(station->positionBindings.size()));
}

void PositionToolChainManager::cleanupStation(const QString& stationId)
{
    QStringList keysToRemove;
    for (auto it = toolChains_.begin(); it != toolChains_.end(); ++it) {
        if (it.value()->stationId == stationId) {
            keysToRemove.append(it.key());
        }
    }

    for (const QString& key : keysToRemove) {
        auto* chain = toolChains_.take(key);
        if (chain) {
            cleanupTools(chain);
            delete chain;
        }
    }

    // 如果当前位置属于被清理的工位，重置当前位置
    if (currentStationId_ == stationId) {
        currentStationId_.clear();
        currentPositionId_.clear();
    }

    LOG_INFO(QString("清理工位工具链: %1, 移除 %2 个")
             .arg(stationId).arg(keysToRemove.size()));
}

// ========== 保存/加载 ==========

bool PositionToolChainManager::saveCurrentToolChain()
{
    auto* chain = currentToolChain();
    if (!chain) {
        return false;
    }

    QString filePath = getToolChainFilePath(currentStationId_, currentPositionId_);

    // 确保目录存在
    QFileInfo fileInfo(filePath);
    QDir().mkpath(fileInfo.absolutePath());

    // 序列化工具链
    QJsonObject root;
    root["stationId"] = chain->stationId;
    root["positionId"] = chain->positionId;
    root["positionName"] = chain->positionName;

    QJsonArray toolsArray;
    for (auto* tool : chain->tools) {
        QJsonObject toolObj;
        toolObj["type"] = static_cast<int>(tool->toolType());
        toolObj["typeName"] = tool->toolName();
        toolObj["displayName"] = tool->displayName();
        toolObj["enabled"] = tool->isEnabled();
        // 序列化工具参数
        toolObj["params"] = tool->serializeParams();
        toolsArray.append(toolObj);
    }
    root["tools"] = toolsArray;

    // 写入文件
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        LOG_ERROR(QString("无法保存工具链文件: %1").arg(filePath));
        return false;
    }

    QJsonDocument doc(root);
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();

    chain->modified = false;
    chain->configFile = filePath;

    LOG_INFO(QString("保存工具链: %1/%2 -> %3")
             .arg(currentStationId_).arg(currentPositionId_).arg(filePath));

    emit toolChainSaved(currentStationId_, currentPositionId_);
    return true;
}

bool PositionToolChainManager::loadToolChain(const QString& stationId, const QString& positionId)
{
    QString filePath = getToolChainFilePath(stationId, positionId);

    if (!QFile::exists(filePath)) {
        return false;
    }

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        LOG_ERROR(QString("无法读取工具链文件: %1").arg(filePath));
        return false;
    }

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &parseError);
    file.close();

    if (parseError.error != QJsonParseError::NoError) {
        LOG_ERROR(QString("解析工具链文件失败: %1").arg(parseError.errorString()));
        return false;
    }

    QJsonObject root = doc.object();

    // 获取或创建工具链
    auto* chain = getOrCreateToolChain(stationId, positionId);

    // 清空现有工具
    cleanupTools(chain);

    // 加载工具
    QJsonArray toolsArray = root["tools"].toArray();
    for (const auto& toolVal : toolsArray) {
        QJsonObject toolObj = toolVal.toObject();

        // 读取工具类型（保存时用 static_cast<int>(toolType())）
        auto toolType = static_cast<Algorithm::VisionTool::ToolType>(toolObj["type"].toInt());
        QString displayName = toolObj["displayName"].toString();
        bool enabled = toolObj["enabled"].toBool(true);
        QJsonObject params = toolObj["params"].toObject();

        // 使用工厂创建工具
        auto* tool = Algorithm::ToolFactory::instance().createTool(toolType);
        if (tool) {
            tool->setDisplayName(displayName);
            tool->setEnabled(enabled);
            // 反序列化工具参数
            if (!params.isEmpty()) {
                tool->deserializeParams(params);
            }
            chain->tools.append(tool);
        }
    }

    chain->configFile = filePath;
    chain->modified = false;

    LOG_INFO(QString("加载工具链: %1/%2, %3 个工具")
             .arg(stationId).arg(positionId).arg(chain->tools.size()));

    emit toolChainLoaded(stationId, positionId);
    return true;
}

bool PositionToolChainManager::saveAll()
{
    bool success = true;
    for (auto* chain : toolChains_) {
        if (chain->modified) {
            // 临时切换当前位置以保存
            QString oldStation = currentStationId_;
            QString oldPosition = currentPositionId_;
            currentStationId_ = chain->stationId;
            currentPositionId_ = chain->positionId;

            if (!saveCurrentToolChain()) {
                success = false;
            }

            currentStationId_ = oldStation;
            currentPositionId_ = oldPosition;
        }
    }
    return success;
}

bool PositionToolChainManager::loadStationToolChains(const QString& stationId)
{
    auto& manager = MultiStationManager::instance();
    auto* station = manager.getStation(stationId);
    if (!station) {
        return false;
    }

    bool anyLoaded = false;
    for (const auto& binding : station->positionBindings) {
        if (loadToolChain(stationId, binding.positionId)) {
            anyLoaded = true;
        }
    }

    return anyLoaded;
}

// ========== 辅助功能 ==========

bool PositionToolChainManager::copyToolChain(const QString& srcStationId, const QString& srcPositionId,
                                              const QString& dstStationId, const QString& dstPositionId)
{
    auto* srcChain = getToolChain(srcStationId, srcPositionId);
    if (!srcChain) {
        return false;
    }

    auto* dstChain = getOrCreateToolChain(dstStationId, dstPositionId);

    // 清空目标工具链
    cleanupTools(dstChain);

    // 复制工具（需要深拷贝）
    for (auto* srcTool : srcChain->tools) {
        auto* newTool = Algorithm::ToolFactory::instance().createTool(srcTool->toolType());
        if (newTool) {
            newTool->setDisplayName(srcTool->displayName());
            newTool->setEnabled(srcTool->isEnabled());
            // 复制参数
            QJsonObject params = srcTool->serializeParams();
            if (!params.isEmpty()) {
                newTool->deserializeParams(params);
            }
            dstChain->tools.append(newTool);
        }
    }

    dstChain->modified = true;

    LOG_INFO(QString("复制工具链: %1/%2 -> %3/%4, %5 个工具")
             .arg(srcStationId).arg(srcPositionId)
             .arg(dstStationId).arg(dstPositionId)
             .arg(dstChain->tools.size()));

    return true;
}

bool PositionToolChainManager::hasToolChain(const QString& stationId, const QString& positionId) const
{
    QString key = stationId + "_" + positionId;
    return toolChains_.contains(key);
}

QStringList PositionToolChainManager::allPositionKeys() const
{
    return toolChains_.keys();
}

void PositionToolChainManager::markCurrentAsModified()
{
    auto* chain = currentToolChain();
    if (chain) {
        chain->modified = true;
    }
}

// ========== 私有方法 ==========

PositionToolChain* PositionToolChainManager::getOrCreateToolChain(const QString& stationId, const QString& positionId)
{
    QString key = stationId + "_" + positionId;

    if (!toolChains_.contains(key)) {
        auto* chain = new PositionToolChain();
        chain->stationId = stationId;
        chain->positionId = positionId;

        // 获取位置名称
        auto& manager = MultiStationManager::instance();
        auto* station = manager.getStation(stationId);
        if (station) {
            auto* binding = station->getPositionBinding(positionId);
            if (binding) {
                chain->positionName = binding->positionName;
            }
        }

        toolChains_[key] = chain;
    }

    return toolChains_[key];
}

QString PositionToolChainManager::getToolChainFilePath(const QString& stationId, const QString& positionId) const
{
    return configBaseDir_ + "/" + stationId + "/toolchain_" + positionId + ".json";
}

void PositionToolChainManager::cleanupTools(PositionToolChain* chain)
{
    if (!chain) {
        return;
    }

    for (auto* tool : chain->tools) {
        delete tool;
    }
    chain->tools.clear();
}

} // namespace Core
} // namespace VisionForge
