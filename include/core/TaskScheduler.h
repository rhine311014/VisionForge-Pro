/**
 * @file TaskScheduler.h
 * @brief 任务调度器
 * @details 管理工具链的异步执行、任务队列和状态监控
 * @author VisionForge Team
 * @date 2025-12-15
 */

#pragma once

#include "core/ToolChain.h"
#include "base/ImageData.h"
#include <QObject>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QQueue>
#include <memory>

namespace VisionForge {
namespace Core {

/**
 * @brief 任务状态
 */
enum class TaskState {
    Pending,        // 等待中
    Running,        // 运行中
    Completed,      // 已完成
    Failed,         // 失败
    Cancelled       // 已取消
};

/**
 * @brief 任务信息
 */
struct TaskInfo {
    QString taskId;                     // 任务ID
    ToolChain* toolChain;               // 工具链（不拥有所有权）
    Base::ImageData::Ptr inputImage;    // 输入图像
    ToolChainResult result;             // 执行结果
    TaskState state;                    // 任务状态
    qint64 submitTime;                  // 提交时间（毫秒）
    qint64 startTime;                   // 开始时间（毫秒）
    qint64 endTime;                     // 结束时间（毫秒）

    TaskInfo()
        : toolChain(nullptr)
        , state(TaskState::Pending)
        , submitTime(0)
        , startTime(0)
        , endTime(0)
    {}
};

/**
 * @brief 任务工作线程
 */
class TaskWorker : public QThread {
    Q_OBJECT

public:
    explicit TaskWorker(QObject* parent = nullptr);
    ~TaskWorker();

    /**
     * @brief 提交任务
     */
    QString submitTask(ToolChain* toolChain, const Base::ImageData::Ptr& inputImage);

    /**
     * @brief 停止工作线程
     */
    void stop();

    /**
     * @brief 获取待处理任务数
     */
    int pendingTaskCount() const;

    /**
     * @brief 是否正在执行任务
     */
    bool isBusy() const;

signals:
    /**
     * @brief 任务开始信号
     */
    void taskStarted(const QString& taskId);

    /**
     * @brief 任务完成信号
     */
    void taskCompleted(const QString& taskId, const ToolChainResult& result);

    /**
     * @brief 任务失败信号
     */
    void taskFailed(const QString& taskId, const QString& errorMessage);

    /**
     * @brief 工具执行进度信号
     */
    void toolProgress(const QString& taskId, int current, int total);

protected:
    void run() override;

private:
    QString generateTaskId();
    void processTask(TaskInfo& task);

private:
    mutable QMutex mutex_;
    QWaitCondition condition_;
    QQueue<TaskInfo> taskQueue_;
    bool stopRequested_;
    bool busy_;
    int taskIdCounter_;
};

/**
 * @brief 任务调度器
 *
 * 单例模式，管理所有工具链的执行
 */
class TaskScheduler : public QObject {
    Q_OBJECT

public:
    /**
     * @brief 获取单例实例
     */
    static TaskScheduler& instance();

    /**
     * @brief 设置工作线程数
     */
    void setWorkerCount(int count);

    /**
     * @brief 获取工作线程数
     */
    int workerCount() const { return workers_.size(); }

    /**
     * @brief 提交同步任务（阻塞执行）
     * @param toolChain 工具链
     * @param inputImage 输入图像
     * @param result 执行结果
     * @return true如果成功
     */
    bool executeSync(ToolChain* toolChain,
                    const Base::ImageData::Ptr& inputImage,
                    ToolChainResult& result);

    /**
     * @brief 提交异步任务（非阻塞）
     * @param toolChain 工具链
     * @param inputImage 输入图像
     * @return 任务ID
     */
    QString executeAsync(ToolChain* toolChain,
                        const Base::ImageData::Ptr& inputImage);

    /**
     * @brief 取消任务
     * @param taskId 任务ID
     * @return true如果成功
     */
    bool cancelTask(const QString& taskId);

    /**
     * @brief 获取任务状态
     */
    TaskState getTaskState(const QString& taskId) const;

    /**
     * @brief 获取任务结果
     */
    bool getTaskResult(const QString& taskId, ToolChainResult& result) const;

    /**
     * @brief 等待任务完成
     * @param taskId 任务ID
     * @param timeout 超时时间（毫秒，-1表示无限等待）
     * @return true如果成功完成
     */
    bool waitForTask(const QString& taskId, int timeout = -1);

    /**
     * @brief 清理已完成的任务
     */
    void cleanupCompletedTasks();

    /**
     * @brief 获取待处理任务总数
     */
    int totalPendingTasks() const;

    /**
     * @brief 获取正在运行的任务数
     */
    int runningTaskCount() const;

signals:
    /**
     * @brief 任务开始信号
     */
    void taskStarted(const QString& taskId);

    /**
     * @brief 任务完成信号
     */
    void taskCompleted(const QString& taskId, const ToolChainResult& result);

    /**
     * @brief 任务失败信号
     */
    void taskFailed(const QString& taskId, const QString& errorMessage);

    /**
     * @brief 任务进度信号
     */
    void taskProgress(const QString& taskId, int current, int total);

private:
    TaskScheduler();
    ~TaskScheduler();

    // 禁止拷贝
    TaskScheduler(const TaskScheduler&) = delete;
    TaskScheduler& operator=(const TaskScheduler&) = delete;

    void onTaskStarted(const QString& taskId);
    void onTaskCompleted(const QString& taskId, const ToolChainResult& result);
    void onTaskFailed(const QString& taskId, const QString& errorMessage);

private:
    QList<TaskWorker*> workers_;                    // 工作线程列表
    mutable QMutex tasksMutex_;                     // 任务列表互斥锁
    QMap<QString, TaskInfo> completedTasks_;        // 已完成任务映射
    QMap<QString, TaskState> runningTasks_;         // 运行中任务状态
};

} // namespace Core
} // namespace VisionForge
