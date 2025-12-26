/**
 * @file SceneManager.cpp
 * @brief 场景管理器实现
 */

#include "core/SceneManager.h"
#include "core/StationConfig.h"
#include "core/SceneConfig.h"
#include "core/PositionToolChainManager.h"
#include "core/ToolChain.h"
#include "base/Logger.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>

namespace VisionForge {
namespace Core {

SceneManager::SceneManager(QObject* parent)
    : QObject(parent)
{
}

SceneManager::~SceneManager()
{
    unloadCurrentSceneResources();
}

bool SceneManager::initialize(StationConfig* stationConfig)
{
    if (!stationConfig) {
        setError("工位配置为空");
        return false;
    }

    stationConfig_ = stationConfig;
    currentSceneIndex_ = stationConfig_->currentSceneIndex;

    // 检查场景列表是否有效
    if (stationConfig_->scenes.isEmpty()) {
        // 如果没有场景，创建默认场景
        SceneConfig defaultScene;
        defaultScene.sceneId = "scene_default";
        defaultScene.sceneName = "默认场景";
        defaultScene.sceneIndex = 0;
        defaultScene.enabled = true;
        stationConfig_->addScene(defaultScene);
        LOG_DEBUG("[SceneManager] 创建默认场景");
    }

    // 确保当前场景索引有效
    if (currentSceneIndex_ < 0 || currentSceneIndex_ >= stationConfig_->scenes.size()) {
        currentSceneIndex_ = 0;
        stationConfig_->currentSceneIndex = 0;
    }

    // 加载当前场景资源
    const SceneConfig* currentScene = getCurrentScene();
    if (currentScene) {
        if (!loadSceneResources(currentScene)) {
            LOG_WARNING(QString("[SceneManager] 加载初始场景资源失败: %1").arg(lastError_));
        }
    }

    isInitialized_ = true;
    LOG_DEBUG(QString("[SceneManager] 初始化完成, 场景数: %1, 当前场景: %2")
              .arg(getSceneCount()).arg(currentSceneIndex_));

    return true;
}

int SceneManager::getSceneCount() const
{
    if (!stationConfig_) return 0;
    return stationConfig_->scenes.size();
}

int SceneManager::getCurrentSceneIndex() const
{
    return currentSceneIndex_;
}

const SceneConfig* SceneManager::getCurrentScene() const
{
    if (!stationConfig_) return nullptr;
    return stationConfig_->getSceneByIndex(currentSceneIndex_);
}

const SceneConfig* SceneManager::getScene(int index) const
{
    if (!stationConfig_) return nullptr;
    return stationConfig_->getSceneByIndex(index);
}

QStringList SceneManager::getSceneNames() const
{
    QStringList names;
    if (!stationConfig_) return names;

    for (const auto& scene : stationConfig_->scenes) {
        names.append(scene.sceneName);
    }
    return names;
}

bool SceneManager::switchToScene(int sceneIndex)
{
    if (!stationConfig_) {
        setError("场景管理器未初始化");
        return false;
    }

    if (sceneIndex < 0 || sceneIndex >= stationConfig_->scenes.size()) {
        setError(QString("无效的场景索引: %1").arg(sceneIndex));
        emit sceneChangeFailed(sceneIndex, lastError_);
        return false;
    }

    const SceneConfig* targetScene = stationConfig_->getSceneByIndex(sceneIndex);
    if (!targetScene) {
        setError("获取目标场景配置失败");
        emit sceneChangeFailed(sceneIndex, lastError_);
        return false;
    }

    if (!targetScene->enabled) {
        setError(QString("场景 '%1' 已禁用").arg(targetScene->sceneName));
        emit sceneChangeFailed(sceneIndex, lastError_);
        return false;
    }

    // 如果是同一个场景，不需要切换
    if (sceneIndex == currentSceneIndex_) {
        LOG_DEBUG("[SceneManager] 目标场景与当前场景相同，跳过切换");
        return true;
    }

    int oldIndex = currentSceneIndex_;

    // 发送即将切换信号
    emit sceneAboutToChange(oldIndex, sceneIndex);

    isLoading_ = true;
    emit sceneLoadProgress(0, QString("正在切换到场景: %1").arg(targetScene->sceneName));

    // 卸载当前场景资源
    emit sceneLoadProgress(20, "卸载当前场景资源...");
    unloadCurrentSceneResources();

    // 加载新场景资源
    emit sceneLoadProgress(40, "加载新场景资源...");
    if (!loadSceneResources(targetScene)) {
        isLoading_ = false;
        emit sceneChangeFailed(sceneIndex, lastError_);
        return false;
    }

    // 应用场景参数
    emit sceneLoadProgress(70, "应用场景参数...");
    applySceneParameters(targetScene);

    // 更新当前场景索引
    currentSceneIndex_ = sceneIndex;
    stationConfig_->currentSceneIndex = sceneIndex;

    emit sceneLoadProgress(100, "场景切换完成");
    isLoading_ = false;

    // 发送场景已切换信号
    emit sceneChanged(sceneIndex, targetScene->sceneName);

    LOG_DEBUG(QString("[SceneManager] 场景切换成功: %1 -> %2 (%3)")
              .arg(oldIndex).arg(sceneIndex).arg(targetScene->sceneName));

    return true;
}

bool SceneManager::switchToScene(const QString& sceneId)
{
    if (!stationConfig_) {
        setError("场景管理器未初始化");
        return false;
    }

    for (int i = 0; i < stationConfig_->scenes.size(); ++i) {
        if (stationConfig_->scenes[i].sceneId == sceneId) {
            return switchToScene(i);
        }
    }

    setError(QString("未找到场景: %1").arg(sceneId));
    return false;
}

bool SceneManager::switchToNextScene()
{
    if (!stationConfig_ || stationConfig_->scenes.isEmpty()) {
        setError("没有可用的场景");
        return false;
    }

    int nextIndex = (currentSceneIndex_ + 1) % stationConfig_->scenes.size();

    // 跳过禁用的场景
    int startIndex = nextIndex;
    while (!stationConfig_->scenes[nextIndex].enabled) {
        nextIndex = (nextIndex + 1) % stationConfig_->scenes.size();
        if (nextIndex == startIndex) {
            setError("没有可用的启用场景");
            return false;
        }
    }

    return switchToScene(nextIndex);
}

bool SceneManager::switchToPreviousScene()
{
    if (!stationConfig_ || stationConfig_->scenes.isEmpty()) {
        setError("没有可用的场景");
        return false;
    }

    int prevIndex = currentSceneIndex_ - 1;
    if (prevIndex < 0) {
        prevIndex = stationConfig_->scenes.size() - 1;
    }

    // 跳过禁用的场景
    int startIndex = prevIndex;
    while (!stationConfig_->scenes[prevIndex].enabled) {
        prevIndex--;
        if (prevIndex < 0) {
            prevIndex = stationConfig_->scenes.size() - 1;
        }
        if (prevIndex == startIndex) {
            setError("没有可用的启用场景");
            return false;
        }
    }

    return switchToScene(prevIndex);
}

bool SceneManager::loadSceneResources(const SceneConfig* scene)
{
    if (!scene) {
        setError("场景配置为空");
        return false;
    }

    LOG_DEBUG(QString("[SceneManager] 加载场景资源: %1").arg(scene->sceneName));

    // 检查并通知AI检测模型路径变更
    if (!scene->detectionModelPath.isEmpty()) {
        if (!QFile::exists(scene->detectionModelPath)) {
            LOG_WARNING(QString("[SceneManager] 检测模型文件不存在: %1").arg(scene->detectionModelPath));
        } else {
            LOG_DEBUG(QString("[SceneManager] 检测模型: %1").arg(scene->detectionModelPath));
        }
        // 发送模型路径变更信号，让外部加载模型
        emit sceneModelPathChanged(scene->detectionModelPath);
    }

    // 检查并通知模板图像路径变更
    if (!scene->templateImagePath.isEmpty()) {
        if (!QFile::exists(scene->templateImagePath)) {
            LOG_WARNING(QString("[SceneManager] 模板图像文件不存在: %1").arg(scene->templateImagePath));
        } else {
            LOG_DEBUG(QString("[SceneManager] 模板图像: %1").arg(scene->templateImagePath));
        }
        // 发送模板路径变更信号
        emit sceneTemplatePathChanged(scene->templateImagePath);
    }

    // 检查并通知工具链配置文件变更
    if (!scene->toolChainFile.isEmpty()) {
        if (!QFile::exists(scene->toolChainFile)) {
            LOG_WARNING(QString("[SceneManager] 工具链配置文件不存在: %1").arg(scene->toolChainFile));
        } else {
            LOG_DEBUG(QString("[SceneManager] 加载工具链配置: %1").arg(scene->toolChainFile));
        }
        // 发送工具链文件变更信号，让外部（如MainWindow/ToolChainPanel）加载工具链
        emit sceneToolChainFileChanged(scene->toolChainFile);
    } else {
        // 场景没有工具链文件，发送空路径信号清空工具链
        emit sceneToolChainFileChanged(QString());
    }

    return true;
}

void SceneManager::unloadCurrentSceneResources()
{
    LOG_DEBUG("[SceneManager] 卸载当前场景资源");

    // TODO: 释放当前场景的资源
    // - 卸载AI模型
    // - 释放模板图像
    // - 清理工具链状态
}

void SceneManager::applySceneParameters(const SceneConfig* scene)
{
    if (!scene) return;

    LOG_DEBUG(QString("[SceneManager] 应用场景参数: %1, 置信度阈值: %2, NMS阈值: %3")
              .arg(scene->sceneName)
              .arg(scene->confidenceThreshold)
              .arg(scene->nmsThreshold));

    // 发送检测参数变更信号，让外部更新检测工具的参数
    emit sceneDetectionParamsChanged(scene->confidenceThreshold, scene->nmsThreshold);
}

void SceneManager::setError(const QString& error)
{
    lastError_ = error;
    LOG_WARNING(QString("[SceneManager] 错误: %1").arg(error));
}

} // namespace Core
} // namespace VisionForge
