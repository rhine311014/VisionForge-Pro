/**
 * @file ModelPreloader.cpp
 * @brief 模型预加载器实现
 */

#include "core/ModelPreloader.h"
#include "algorithm/ShapeMatchTool.h"
#include "algorithm/AIDetectionTool.h"
#include "algorithm/CodeReadTool.h"
#include "base/Logger.h"
#include <QtConcurrent>
#include <QElapsedTimer>

namespace VisionForge {
namespace Core {

ModelPreloader::ModelPreloader(QObject* parent)
    : QObject(parent)
    , isLoading_(false)
    , cancelRequested_(false)
    , watcher_(nullptr)
    , completedCount_(0)
    , startTime_(0)
{
}

ModelPreloader::~ModelPreloader()
{
    cancel();
    if (watcher_) {
        watcher_->waitForFinished();
        delete watcher_;
    }
}

int ModelPreloader::preloadModels(ToolChain* toolChain, bool parallel)
{
    if (!toolChain) {
        LOG_WARNING("ModelPreloader: 工具链为空");
        return 0;
    }

    if (isLoading_) {
        LOG_WARNING("ModelPreloader: 已有预加载任务正在进行");
        return 0;
    }

    // 扫描需要预加载的任务
    tasks_ = scanToolChain(toolChain);

    if (tasks_.isEmpty()) {
        LOG_INFO("ModelPreloader: 无需预加载的模型");
        emit finished(0, 0, 0);
        return 0;
    }

    LOG_INFO(QString("ModelPreloader: 开始预加载 %1 个模型").arg(tasks_.size()));

    isLoading_ = true;
    cancelRequested_ = false;
    results_.clear();
    completedCount_ = 0;

    QElapsedTimer timer;
    timer.start();
    startTime_ = timer.msecsSinceReference();

    emit started(tasks_.size());

    if (parallel) {
        // 并行加载
        auto loadFunc = [this](const PreloadTask& task) {
            if (cancelRequested_) {
                return;
            }

            PreloadResult result = loadModel(task);

            QMutexLocker locker(&mutex_);
            results_.append(result);
            completedCount_++;

            // 在主线程发送信号
            QMetaObject::invokeMethod(this, [this, result]() {
                emit taskFinished(result);
                emit progressChanged(completedCount_, tasks_.size(), result.toolName);

                if (completedCount_ >= tasks_.size()) {
                    onTaskFinished();
                }
            }, Qt::QueuedConnection);
        };

        // 使用 QtConcurrent 并行执行
        QtConcurrent::map(tasks_, loadFunc);
    }
    else {
        // 串行加载
        for (const PreloadTask& task : tasks_) {
            if (cancelRequested_) {
                break;
            }

            emit progressChanged(completedCount_, tasks_.size(), task.toolName);

            PreloadResult result = loadModel(task);
            results_.append(result);
            completedCount_++;

            emit taskFinished(result);
        }

        onTaskFinished();
    }

    return tasks_.size();
}

QList<PreloadTask> ModelPreloader::scanToolChain(ToolChain* toolChain) const
{
    QList<PreloadTask> tasks;

    if (!toolChain) {
        return tasks;
    }

    for (int i = 0; i < toolChain->toolCount(); i++) {
        Algorithm::VisionTool* tool = toolChain->getTool(i);
        if (!tool) {
            continue;
        }

        if (needsPreload(tool)) {
            QString modelPath = getModelPath(tool);
            if (!modelPath.isEmpty()) {
                QString nodeId = toolChain->getNodeId(i);
                const ToolChainNode* node = toolChain->getNode(i);
                QString toolName = node ? node->name : tool->toolName();

                tasks.append(PreloadTask(nodeId, toolName, modelPath, tool));

                LOG_DEBUG(QString("ModelPreloader: 发现需预加载的模型 - %1: %2")
                    .arg(toolName).arg(modelPath));
            }
        }
    }

    return tasks;
}

void ModelPreloader::cancel()
{
    if (!isLoading_) {
        return;
    }

    LOG_INFO("ModelPreloader: 取消预加载");
    cancelRequested_ = true;

    if (watcher_) {
        watcher_->cancel();
    }

    emit cancelled();
}

bool ModelPreloader::waitForFinished(int timeout)
{
    if (!isLoading_) {
        return true;
    }

    if (watcher_) {
        if (timeout < 0) {
            watcher_->waitForFinished();
            return true;
        }
        else {
            // QtConcurrent doesn't have timed wait, use polling
            QElapsedTimer timer;
            timer.start();
            while (!watcher_->isFinished() && timer.elapsed() < timeout) {
                QThread::msleep(10);
            }
            return watcher_->isFinished();
        }
    }

    return true;
}

int ModelPreloader::successCount() const
{
    int count = 0;
    for (const PreloadResult& result : results_) {
        if (result.success) {
            count++;
        }
    }
    return count;
}

int ModelPreloader::failureCount() const
{
    int count = 0;
    for (const PreloadResult& result : results_) {
        if (!result.success) {
            count++;
        }
    }
    return count;
}

void ModelPreloader::onTaskFinished()
{
    QElapsedTimer timer;
    qint64 totalTime = timer.msecsSinceReference() - startTime_;

    isLoading_ = false;

    int success = successCount();
    int failure = failureCount();

    LOG_INFO(QString("ModelPreloader: 预加载完成 - 成功: %1, 失败: %2, 耗时: %3ms")
        .arg(success).arg(failure).arg(totalTime));

    emit finished(success, failure, totalTime);
}

PreloadResult ModelPreloader::loadModel(const PreloadTask& task)
{
    PreloadResult result;
    result.toolId = task.toolId;
    result.toolName = task.toolName;

    QElapsedTimer timer;
    timer.start();

    if (!task.tool) {
        result.success = false;
        result.errorMessage = "工具指针为空";
        result.loadTimeMs = timer.elapsed();
        return result;
    }

    try {
        // 根据工具类型调用相应的加载方法
        Algorithm::VisionTool::ToolType type = task.tool->toolType();

        switch (type) {
        case Algorithm::VisionTool::Match:
        case Algorithm::VisionTool::TemplateMatch:
            {
                auto* shapeTool = qobject_cast<Algorithm::ShapeMatchTool*>(task.tool);
                if (shapeTool && !task.modelPath.isEmpty()) {
                    if (shapeTool->loadModel(task.modelPath)) {
                        result.success = true;
                        LOG_DEBUG(QString("预加载形状模板成功: %1").arg(task.modelPath));
                    }
                    else {
                        result.success = false;
                        result.errorMessage = "形状模板加载失败";
                    }
                }
                else {
                    result.success = true;  // 无模型路径，跳过
                }
            }
            break;

        case Algorithm::VisionTool::AIDetection:
            {
                auto* aiTool = qobject_cast<Algorithm::AIDetectionTool*>(task.tool);
                if (aiTool && !task.modelPath.isEmpty()) {
                    if (aiTool->loadModel(task.modelPath)) {
                        result.success = true;
                        LOG_DEBUG(QString("预加载AI模型成功: %1").arg(task.modelPath));
                    }
                    else {
                        result.success = false;
                        result.errorMessage = "AI模型加载失败";
                    }
                }
                else {
                    result.success = true;
                }
            }
            break;

        case Algorithm::VisionTool::CodeRead:
            {
                auto* codeTool = qobject_cast<Algorithm::CodeReadTool*>(task.tool);
                if (codeTool) {
                    // CodeReadTool 内部会预加载 ShapeMatchTool
                    // 检查其内部的形状匹配工具是否需要加载模型
                    result.success = true;  // CodeReadTool 会在首次使用时自动加载
                    LOG_DEBUG(QString("CodeReadTool 已准备就绪"));
                }
            }
            break;

        default:
            result.success = true;  // 其他类型不需要预加载
            break;
        }
    }
    catch (const std::exception& e) {
        result.success = false;
        result.errorMessage = QString("加载异常: %1").arg(e.what());
        LOG_ERROR(QString("模型预加载异常: %1 - %2").arg(task.toolName).arg(e.what()));
    }

    result.loadTimeMs = timer.elapsed();
    return result;
}

bool ModelPreloader::needsPreload(Algorithm::VisionTool* tool) const
{
    if (!tool) {
        return false;
    }

    Algorithm::VisionTool::ToolType type = tool->toolType();

    switch (type) {
    case Algorithm::VisionTool::Match:
    case Algorithm::VisionTool::TemplateMatch:
    case Algorithm::VisionTool::AIDetection:
    case Algorithm::VisionTool::CodeRead:
        return true;

    default:
        return false;
    }
}

QString ModelPreloader::getModelPath(Algorithm::VisionTool* tool) const
{
    if (!tool) {
        return QString();
    }

    Algorithm::VisionTool::ToolType type = tool->toolType();

    switch (type) {
    case Algorithm::VisionTool::Match:
    case Algorithm::VisionTool::TemplateMatch:
        {
            auto* shapeTool = qobject_cast<Algorithm::ShapeMatchTool*>(tool);
            if (shapeTool) {
                return shapeTool->modelPath();
            }
        }
        break;

    case Algorithm::VisionTool::AIDetection:
        {
            auto* aiTool = qobject_cast<Algorithm::AIDetectionTool*>(tool);
            if (aiTool) {
                return aiTool->modelPath();
            }
        }
        break;

    case Algorithm::VisionTool::CodeRead:
        {
            // CodeReadTool 内部使用 ShapeMatchTool
            // 返回一个标记表示需要预热
            return "internal:shapematch";
        }
        break;

    default:
        break;
    }

    return QString();
}

} // namespace Core
} // namespace VisionForge
