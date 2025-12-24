/**
 * @file SceneManager.cpp
 * @brief 场景管理器实现
 */

#include "core/SceneManager.h"
#include "core/StationConfig.h"
#include "core/SceneConfig.h"
#include <QDebug>
#include <QFile>

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
        qDebug() << "[SceneManager] 创建默认场景";
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
            qWarning() << "[SceneManager] 加载初始场景资源失败:" << lastError_;
        }
    }

    isInitialized_ = true;
    qDebug() << "[SceneManager] 初始化完成, 场景数:" << getSceneCount()
             << ", 当前场景:" << currentSceneIndex_;

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
        qDebug() << "[SceneManager] 目标场景与当前场景相同，跳过切换";
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

    qDebug() << "[SceneManager] 场景切换成功:" << oldIndex << "->" << sceneIndex
             << "(" << targetScene->sceneName << ")";

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

    qDebug() << "[SceneManager] 加载场景资源:" << scene->sceneName;

    // 检查检测模型文件
    if (!scene->detectionModelPath.isEmpty()) {
        if (!QFile::exists(scene->detectionModelPath)) {
            qWarning() << "[SceneManager] 检测模型文件不存在:" << scene->detectionModelPath;
            // 不作为错误处理，继续加载
        } else {
            qDebug() << "[SceneManager] 检测模型:" << scene->detectionModelPath;
            // TODO: 加载AI检测模型
        }
    }

    // 检查模板图像文件
    if (!scene->templateImagePath.isEmpty()) {
        if (!QFile::exists(scene->templateImagePath)) {
            qWarning() << "[SceneManager] 模板图像文件不存在:" << scene->templateImagePath;
        } else {
            qDebug() << "[SceneManager] 模板图像:" << scene->templateImagePath;
            // TODO: 加载模板匹配图像
        }
    }

    // 检查工具链配置文件
    if (!scene->toolChainFile.isEmpty()) {
        if (!QFile::exists(scene->toolChainFile)) {
            qWarning() << "[SceneManager] 工具链配置文件不存在:" << scene->toolChainFile;
        } else {
            qDebug() << "[SceneManager] 工具链配置:" << scene->toolChainFile;
            // TODO: 加载工具链配置
        }
    }

    return true;
}

void SceneManager::unloadCurrentSceneResources()
{
    qDebug() << "[SceneManager] 卸载当前场景资源";

    // TODO: 释放当前场景的资源
    // - 卸载AI模型
    // - 释放模板图像
    // - 清理工具链状态
}

void SceneManager::applySceneParameters(const SceneConfig* scene)
{
    if (!scene) return;

    qDebug() << "[SceneManager] 应用场景参数:" << scene->sceneName
             << ", 置信度阈值:" << scene->confidenceThreshold
             << ", NMS阈值:" << scene->nmsThreshold;

    // TODO: 将场景参数应用到检测工具
    // - 更新AIDetectionTool的阈值参数
    // - 更新模板匹配工具的参数
    // - 应用位置特定参数覆盖
}

void SceneManager::setError(const QString& error)
{
    lastError_ = error;
    qWarning() << "[SceneManager] 错误:" << error;
}

} // namespace Core
} // namespace VisionForge
