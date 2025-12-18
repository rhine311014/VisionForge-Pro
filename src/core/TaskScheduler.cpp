/**
 * @file TaskScheduler.cpp
 * @brief 任务调度器实现
 */

#include "core/TaskScheduler.h"
#include "base/Logger.h"
#include <QDateTime>
#include <QEventLoop>
#include <QTimer>

namespace VisionForge {
namespace Core {

// ========== TaskWorker实现 ==========

TaskWorker::TaskWorker(QObject* parent)
    : QThread(parent)
    , stopRequested_(false)
    , busy_(false)
    , taskIdCounter_(0)
    , currentTaskId_()
{
}

TaskWorker::~TaskWorker()
{
    stop();
    wait();
}

QString TaskWorker::submitTask(ToolChain* toolChain, const Base::ImageData::Ptr& inputImage)
{
    QMutexLocker locker(&mutex_);

    TaskInfo task;
    task.taskId = generateTaskId();
    task.toolChain = toolChain;
    task.inputImage = inputImage;
    task.state = TaskState::Pending;
    task.submitTime = QDateTime::currentMSecsSinceEpoch();

    taskQueue_.enqueue(task);

    LOG_DEBUG(QString("[Worker] 提交任务: %1, 队列长度: %2")
             .arg(task.taskId).arg(taskQueue_.size()));

    condition_.wakeOne();

    return task.taskId;
}

void TaskWorker::stop()
{
    QMutexLocker locker(&mutex_);
    stopRequested_ = true;
    condition_.wakeOne();
}

int TaskWorker::pendingTaskCount() const
{
    QMutexLocker locker(&mutex_);
    return taskQueue_.size();
}

bool TaskWorker::isBusy() const
{
    QMutexLocker locker(&mutex_);
    return busy_;
}

bool TaskWorker::cancelTask(const QString& taskId)
{
    QMutexLocker locker(&mutex_);

    // 检查任务是否在队列中
    for (int i = 0; i < taskQueue_.size(); ++i) {
        if (taskQueue_[i].taskId == taskId) {
            // 从队列中移除任务
            taskQueue_.removeAt(i);
            LOG_INFO(QString("[Worker] 从队列移除任务: %1").arg(taskId));
            emit taskCancelled(taskId);
            return true;
        }
    }

    // 检查是否是当前正在执行的任务
    if (currentTaskId_ == taskId) {
        // 标记为已取消，但无法立即中断（需要等待当前工具执行完毕）
        cancelledTasks_.insert(taskId);
        LOG_INFO(QString("[Worker] 标记任务为取消: %1 (正在执行中)").arg(taskId));
        return true;
    }

    return false;
}

QString TaskWorker::currentTaskId() const
{
    QMutexLocker locker(&mutex_);
    return currentTaskId_;
}

bool TaskWorker::isTaskCancelled(const QString& taskId) const
{
    QMutexLocker locker(&mutex_);
    return cancelledTasks_.contains(taskId);
}

void TaskWorker::run()
{
    LOG_INFO(QString("[Worker] 工作线程启动: %1").arg(reinterpret_cast<qulonglong>(QThread::currentThreadId())));

    while (true) {
        TaskInfo task;

        // 获取任务
        {
            QMutexLocker locker(&mutex_);

            while (taskQueue_.isEmpty() && !stopRequested_) {
                condition_.wait(&mutex_);
            }

            if (stopRequested_) {
                break;
            }

            task = taskQueue_.dequeue();
            currentTaskId_ = task.taskId;
            busy_ = true;
        }

        // 检查任务是否已被取消
        if (isTaskCancelled(task.taskId)) {
            LOG_INFO(QString("[Worker] 任务已取消，跳过执行: %1").arg(task.taskId));
            {
                QMutexLocker locker(&mutex_);
                cancelledTasks_.remove(task.taskId);
                currentTaskId_.clear();
                busy_ = false;
            }
            emit taskCancelled(task.taskId);
            continue;
        }

        // 处理任务
        processTask(task);

        // 任务完成
        {
            QMutexLocker locker(&mutex_);
            currentTaskId_.clear();
            busy_ = false;
        }
    }

    LOG_INFO(QString("[Worker] 工作线程退出: %1").arg(reinterpret_cast<qulonglong>(QThread::currentThreadId())));
}

QString TaskWorker::generateTaskId()
{
    return QString("task_%1_%2")
           .arg(reinterpret_cast<qulonglong>(QThread::currentThreadId()))
           .arg(++taskIdCounter_);
}

void TaskWorker::processTask(TaskInfo& task)
{
    task.state = TaskState::Running;
    task.startTime = QDateTime::currentMSecsSinceEpoch();

    LOG_INFO(QString("[Worker] 开始执行任务: %1").arg(task.taskId));

    emit taskStarted(task.taskId);

    // 连接工具链进度信号
    connect(task.toolChain, &ToolChain::executionProgress,
            this, [this, taskId = task.taskId](int current, int total) {
        emit toolProgress(taskId, current, total);
    });

    // 执行工具链
    bool success = task.toolChain->execute(task.inputImage, task.result);

    // 断开连接
    disconnect(task.toolChain, &ToolChain::executionProgress, this, nullptr);

    task.endTime = QDateTime::currentMSecsSinceEpoch();

    // 检查任务是否在执行过程中被取消
    if (isTaskCancelled(task.taskId)) {
        task.state = TaskState::Cancelled;
        {
            QMutexLocker locker(&mutex_);
            cancelledTasks_.remove(task.taskId);
        }
        LOG_INFO(QString("[Worker] 任务被取消: %1").arg(task.taskId));
        emit taskCancelled(task.taskId);
        return;
    }

    if (success) {
        task.state = TaskState::Completed;
        LOG_INFO(QString("[Worker] 任务完成: %1, 耗时: %2 ms")
                .arg(task.taskId)
                .arg(task.result.totalExecutionTime));
        emit taskCompleted(task.taskId, task.result);
    } else {
        task.state = TaskState::Failed;
        LOG_ERROR(QString("[Worker] 任务失败: %1, 错误: %2")
                 .arg(task.taskId)
                 .arg(task.result.errorMessage));
        emit taskFailed(task.taskId, task.result.errorMessage);
    }
}

// ========== TaskScheduler实现 ==========

TaskScheduler::TaskScheduler()
{
    // 默认创建一个工作线程
    setWorkerCount(1);
}

TaskScheduler::~TaskScheduler()
{
    // 停止所有工作线程
    for (TaskWorker* worker : workers_) {
        worker->stop();
        worker->wait();
        delete worker;
    }
    workers_.clear();
}

TaskScheduler& TaskScheduler::instance()
{
    static TaskScheduler instance;
    return instance;
}

void TaskScheduler::setWorkerCount(int count)
{
    if (count < 1) {
        count = 1;
    }

    // 停止现有线程
    for (TaskWorker* worker : workers_) {
        worker->stop();
        worker->wait();
        delete worker;
    }
    workers_.clear();

    // 创建新线程
    for (int i = 0; i < count; ++i) {
        TaskWorker* worker = new TaskWorker(this);

        connect(worker, &TaskWorker::taskStarted,
                this, &TaskScheduler::onTaskStarted);
        connect(worker, &TaskWorker::taskCompleted,
                this, &TaskScheduler::onTaskCompleted);
        connect(worker, &TaskWorker::taskFailed,
                this, &TaskScheduler::onTaskFailed);
        connect(worker, &TaskWorker::toolProgress,
                this, &TaskScheduler::taskProgress);
        connect(worker, &TaskWorker::taskCancelled,
                this, &TaskScheduler::onTaskCancelled);

        worker->start();
        workers_.append(worker);
    }

    LOG_INFO(QString("任务调度器：设置工作线程数 = %1").arg(count));
}

bool TaskScheduler::executeSync(ToolChain* toolChain,
                                const Base::ImageData::Ptr& inputImage,
                                ToolChainResult& result)
{
    if (!toolChain) {
        LOG_ERROR("同步执行失败：工具链为空");
        return false;
    }

    if (!inputImage || inputImage->isEmpty()) {
        LOG_ERROR("同步执行失败：输入图像为空");
        return false;
    }

    LOG_INFO(QString("同步执行工具链: %1").arg(toolChain->name()));

    // 直接在当前线程执行
    bool success = toolChain->execute(inputImage, result);

    return success;
}

QString TaskScheduler::executeAsync(ToolChain* toolChain,
                                    const Base::ImageData::Ptr& inputImage)
{
    if (!toolChain) {
        LOG_ERROR("异步执行失败：工具链为空");
        return QString();
    }

    if (!inputImage || inputImage->isEmpty()) {
        LOG_ERROR("异步执行失败：输入图像为空");
        return QString();
    }

    // 选择最空闲的工作线程
    TaskWorker* selectedWorker = nullptr;
    int minPending = INT_MAX;

    for (TaskWorker* worker : workers_) {
        int pending = worker->pendingTaskCount();
        if (pending < minPending) {
            minPending = pending;
            selectedWorker = worker;
        }
    }

    if (!selectedWorker) {
        LOG_ERROR("异步执行失败：没有可用的工作线程");
        return QString();
    }

    // 提交任务
    QString taskId = selectedWorker->submitTask(toolChain, inputImage);

    {
        QMutexLocker locker(&tasksMutex_);
        runningTasks_[taskId] = TaskState::Pending;
    }

    LOG_INFO(QString("异步提交任务: %1, 工具链: %2")
            .arg(taskId).arg(toolChain->name()));

    return taskId;
}

bool TaskScheduler::cancelTask(const QString& taskId)
{
    {
        QMutexLocker locker(&tasksMutex_);

        if (!runningTasks_.contains(taskId)) {
            LOG_WARNING(QString("取消任务失败：未找到任务 %1").arg(taskId));
            return false;
        }
    }

    // 遍历所有worker尝试取消任务
    bool cancelled = false;
    for (TaskWorker* worker : workers_) {
        if (worker->cancelTask(taskId)) {
            cancelled = true;
            break;
        }
    }

    if (cancelled) {
        QMutexLocker locker(&tasksMutex_);
        runningTasks_[taskId] = TaskState::Cancelled;
        LOG_INFO(QString("取消任务: %1").arg(taskId));
    } else {
        LOG_WARNING(QString("取消任务失败：任务 %1 不在任何worker队列中").arg(taskId));
    }

    return cancelled;
}

TaskState TaskScheduler::getTaskState(const QString& taskId) const
{
    QMutexLocker locker(&tasksMutex_);

    if (runningTasks_.contains(taskId)) {
        return runningTasks_[taskId];
    }

    if (completedTasks_.contains(taskId)) {
        return completedTasks_[taskId].state;
    }

    return TaskState::Pending;
}

bool TaskScheduler::getTaskResult(const QString& taskId, ToolChainResult& result) const
{
    QMutexLocker locker(&tasksMutex_);

    if (!completedTasks_.contains(taskId)) {
        return false;
    }

    result = completedTasks_[taskId].result;
    return true;
}

bool TaskScheduler::waitForTask(const QString& taskId, int timeout)
{
    QElapsedTimer timer;
    timer.start();

    while (true) {
        TaskState state = getTaskState(taskId);

        if (state == TaskState::Completed || state == TaskState::Failed ||
            state == TaskState::Cancelled) {
            return (state == TaskState::Completed);
        }

        if (timeout >= 0 && timer.elapsed() >= timeout) {
            LOG_WARNING(QString("等待任务超时: %1").arg(taskId));
            return false;
        }

        // 等待一小段时间
        QThread::msleep(10);
    }
}

void TaskScheduler::cleanupCompletedTasks()
{
    QMutexLocker locker(&tasksMutex_);

    int count = completedTasks_.size();
    completedTasks_.clear();

    LOG_INFO(QString("清理已完成任务: %1 个").arg(count));
}

int TaskScheduler::totalPendingTasks() const
{
    int total = 0;
    for (TaskWorker* worker : workers_) {
        total += worker->pendingTaskCount();
    }
    return total;
}

int TaskScheduler::runningTaskCount() const
{
    int count = 0;
    for (TaskWorker* worker : workers_) {
        if (worker->isBusy()) {
            ++count;
        }
    }
    return count;
}

void TaskScheduler::onTaskStarted(const QString& taskId)
{
    {
        QMutexLocker locker(&tasksMutex_);
        runningTasks_[taskId] = TaskState::Running;
    }

    emit taskStarted(taskId);
}

void TaskScheduler::onTaskCompleted(const QString& taskId, const ToolChainResult& result)
{
    {
        QMutexLocker locker(&tasksMutex_);

        runningTasks_.remove(taskId);

        TaskInfo task;
        task.taskId = taskId;
        task.result = result;
        task.state = TaskState::Completed;
        task.endTime = QDateTime::currentMSecsSinceEpoch();

        completedTasks_[taskId] = task;
    }

    emit taskCompleted(taskId, result);
}

void TaskScheduler::onTaskFailed(const QString& taskId, const QString& errorMessage)
{
    {
        QMutexLocker locker(&tasksMutex_);

        runningTasks_.remove(taskId);

        TaskInfo task;
        task.taskId = taskId;
        task.state = TaskState::Failed;
        task.result.success = false;
        task.result.errorMessage = errorMessage;
        task.endTime = QDateTime::currentMSecsSinceEpoch();

        completedTasks_[taskId] = task;
    }

    emit taskFailed(taskId, errorMessage);
}

void TaskScheduler::onTaskCancelled(const QString& taskId)
{
    {
        QMutexLocker locker(&tasksMutex_);

        runningTasks_.remove(taskId);

        TaskInfo task;
        task.taskId = taskId;
        task.state = TaskState::Cancelled;
        task.result.success = false;
        task.result.errorMessage = "任务已取消";
        task.endTime = QDateTime::currentMSecsSinceEpoch();

        completedTasks_[taskId] = task;
    }

    LOG_INFO(QString("任务取消完成: %1").arg(taskId));
    emit taskCancelled(taskId);
}

} // namespace Core
} // namespace VisionForge
