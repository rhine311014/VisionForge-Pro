/**
 * @file ModelPreloader.h
 * @brief 模型预加载器
 * @details 在配方切换或程序启动时异步预加载算法模型，避免首次运行卡顿
 * @author VisionForge Team
 * @date 2025-12-25
 */

#pragma once

#include "core/ToolChain.h"
#include <QObject>
#include <QFuture>
#include <QFutureWatcher>
#include <QMutex>
#include <functional>

namespace VisionForge {
namespace Core {

/**
 * @brief 预加载优先级（数值越小优先级越高）
 */
enum class PreloadPriority {
    Critical = 0,   ///< 关键（AI模型等大型模型）
    High = 1,       ///< 高（形状模板等）
    Normal = 2,     ///< 普通
    Low = 3         ///< 低
};

/**
 * @brief 模型预加载任务信息
 */
struct PreloadTask {
    QString toolId;             ///< 工具ID
    QString toolName;           ///< 工具名称
    QString modelPath;          ///< 模型路径
    Algorithm::VisionTool* tool; ///< 工具指针
    PreloadPriority priority;   ///< 加载优先级

    PreloadTask()
        : tool(nullptr)
        , priority(PreloadPriority::Normal)
    {}

    PreloadTask(const QString& id, const QString& name,
                const QString& path, Algorithm::VisionTool* t,
                PreloadPriority p = PreloadPriority::Normal)
        : toolId(id)
        , toolName(name)
        , modelPath(path)
        , tool(t)
        , priority(p)
    {}

    /// 用于排序（优先级高的排在前面）
    bool operator<(const PreloadTask& other) const {
        return static_cast<int>(priority) < static_cast<int>(other.priority);
    }
};

/**
 * @brief 预加载结果
 */
struct PreloadResult {
    QString toolId;             ///< 工具ID
    QString toolName;           ///< 工具名称
    bool success;               ///< 是否成功
    QString errorMessage;       ///< 错误消息
    qint64 loadTimeMs;          ///< 加载耗时(毫秒)

    PreloadResult()
        : success(false)
        , loadTimeMs(0)
    {}
};

/**
 * @class ModelPreloader
 * @brief 模型预加载器
 *
 * 功能：
 * - 扫描工具链中需要预加载模型的工具
 * - 异步并行加载模型
 * - 提供加载进度回调
 * - 支持取消加载
 *
 * 使用示例：
 * @code
 * ModelPreloader preloader;
 * connect(&preloader, &ModelPreloader::progressChanged,
 *         this, &MainWindow::onPreloadProgress);
 * connect(&preloader, &ModelPreloader::finished,
 *         this, &MainWindow::onPreloadFinished);
 * preloader.preloadModels(toolChain);
 * @endcode
 */
class ModelPreloader : public QObject {
    Q_OBJECT

public:
    explicit ModelPreloader(QObject* parent = nullptr);
    ~ModelPreloader();

    /**
     * @brief 预加载工具链中的所有模型
     * @param toolChain 工具链指针
     * @param parallel 是否并行加载（默认true）
     * @return 需要预加载的模型数量
     */
    int preloadModels(ToolChain* toolChain, bool parallel = true);

    /**
     * @brief 扫描工具链，获取需要预加载的任务列表
     * @param toolChain 工具链指针
     * @return 预加载任务列表
     */
    QList<PreloadTask> scanToolChain(ToolChain* toolChain) const;

    /**
     * @brief 取消预加载
     */
    void cancel();

    /**
     * @brief 是否正在加载
     */
    bool isLoading() const { return isLoading_; }

    /**
     * @brief 等待加载完成
     * @param timeout 超时时间(毫秒)，-1表示无限等待
     * @return true如果加载完成，false如果超时
     */
    bool waitForFinished(int timeout = -1);

    /**
     * @brief 获取加载结果
     */
    QList<PreloadResult> results() const { return results_; }

    /**
     * @brief 获取成功加载的模型数量
     */
    int successCount() const;

    /**
     * @brief 获取失败的模型数量
     */
    int failureCount() const;

signals:
    /**
     * @brief 预加载开始信号
     * @param totalCount 总任务数
     */
    void started(int totalCount);

    /**
     * @brief 进度变化信号
     * @param current 当前完成数
     * @param total 总任务数
     * @param currentTaskName 当前任务名称
     */
    void progressChanged(int current, int total, const QString& currentTaskName);

    /**
     * @brief 单个任务完成信号
     * @param result 加载结果
     */
    void taskFinished(const PreloadResult& result);

    /**
     * @brief 全部完成信号
     * @param successCount 成功数量
     * @param failureCount 失败数量
     * @param totalTimeMs 总耗时(毫秒)
     */
    void finished(int successCount, int failureCount, qint64 totalTimeMs);

    /**
     * @brief 加载取消信号
     */
    void cancelled();

private slots:
    void onTaskFinished();

private:
    /**
     * @brief 加载单个模型
     */
    PreloadResult loadModel(const PreloadTask& task);

    /**
     * @brief 检查工具是否需要预加载
     */
    bool needsPreload(Algorithm::VisionTool* tool) const;

    /**
     * @brief 获取工具的模型路径
     */
    QString getModelPath(Algorithm::VisionTool* tool) const;

private:
    bool isLoading_;
    bool cancelRequested_;
    QList<PreloadTask> tasks_;
    QList<PreloadResult> results_;
    QFutureWatcher<void>* watcher_;
    QMutex mutex_;
    int completedCount_;
    qint64 startTime_;
};

} // namespace Core
} // namespace VisionForge
