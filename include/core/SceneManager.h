/**
 * @file SceneManager.h
 * @brief 场景管理器
 *
 * 负责运行时场景切换、工具链加载和检测参数更新
 */

#ifndef VISIONFORGE_CORE_SCENE_MANAGER_H
#define VISIONFORGE_CORE_SCENE_MANAGER_H

#include <QObject>
#include <QString>
#include <QVector>
#include <QMap>

namespace VisionForge {
namespace Core {

struct SceneConfig;
struct StationConfig;

/**
 * @brief 场景管理器
 *
 * 功能：
 * - 管理当前工位的场景列表
 * - 处理场景切换逻辑
 * - 加载/卸载场景相关的工具链和模型
 * - 发送场景变更信号
 */
class SceneManager : public QObject {
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     */
    explicit SceneManager(QObject* parent = nullptr);

    /**
     * @brief 析构函数
     */
    ~SceneManager() override;

    /**
     * @brief 初始化场景管理器
     * @param stationConfig 工位配置
     * @return 是否初始化成功
     */
    bool initialize(StationConfig* stationConfig);

    /**
     * @brief 获取场景数量
     */
    int getSceneCount() const;

    /**
     * @brief 获取当前场景索引
     */
    int getCurrentSceneIndex() const;

    /**
     * @brief 获取当前场景配置
     */
    const SceneConfig* getCurrentScene() const;

    /**
     * @brief 根据索引获取场景配置
     */
    const SceneConfig* getScene(int index) const;

    /**
     * @brief 获取场景名称列表
     */
    QStringList getSceneNames() const;

    /**
     * @brief 切换到指定场景
     * @param sceneIndex 场景索引
     * @return 是否切换成功
     */
    bool switchToScene(int sceneIndex);

    /**
     * @brief 切换到指定场景（通过ID）
     * @param sceneId 场景ID
     * @return 是否切换成功
     */
    bool switchToScene(const QString& sceneId);

    /**
     * @brief 切换到下一个场景
     * @return 是否切换成功
     */
    bool switchToNextScene();

    /**
     * @brief 切换到上一个场景
     * @return 是否切换成功
     */
    bool switchToPreviousScene();

    /**
     * @brief 获取场景是否正在加载
     */
    bool isLoading() const { return isLoading_; }

    /**
     * @brief 获取上次错误信息
     */
    QString getLastError() const { return lastError_; }

signals:
    /**
     * @brief 场景即将切换信号
     * @param fromIndex 原场景索引
     * @param toIndex 目标场景索引
     */
    void sceneAboutToChange(int fromIndex, int toIndex);

    /**
     * @brief 场景已切换信号
     * @param sceneIndex 新场景索引
     * @param sceneName 新场景名称
     */
    void sceneChanged(int sceneIndex, const QString& sceneName);

    /**
     * @brief 场景切换失败信号
     * @param sceneIndex 目标场景索引
     * @param error 错误信息
     */
    void sceneChangeFailed(int sceneIndex, const QString& error);

    /**
     * @brief 场景加载进度信号
     * @param progress 进度 (0-100)
     * @param message 状态消息
     */
    void sceneLoadProgress(int progress, const QString& message);

private:
    /**
     * @brief 加载场景资源
     * @param scene 场景配置
     * @return 是否加载成功
     */
    bool loadSceneResources(const SceneConfig* scene);

    /**
     * @brief 卸载当前场景资源
     */
    void unloadCurrentSceneResources();

    /**
     * @brief 应用场景参数到检测工具
     * @param scene 场景配置
     */
    void applySceneParameters(const SceneConfig* scene);

    /**
     * @brief 设置错误信息
     */
    void setError(const QString& error);

    // 关联的工位配置
    StationConfig* stationConfig_ = nullptr;

    // 当前场景索引
    int currentSceneIndex_ = 0;

    // 状态标志
    bool isLoading_ = false;
    bool isInitialized_ = false;

    // 错误信息
    QString lastError_;
};

} // namespace Core
} // namespace VisionForge

#endif // VISIONFORGE_CORE_SCENE_MANAGER_H
