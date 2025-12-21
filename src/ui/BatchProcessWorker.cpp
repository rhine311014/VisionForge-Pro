/**
 * @file BatchProcessWorker.cpp
 * @brief 批量处理后台工作线程实现
 * @author VisionForge Team
 * @date 2025-12-20
 */

#include "ui/BatchProcessWorker.h"
#include "base/Logger.h"
#include "base/ImageMemoryPool.h"
#include <QFile>
#include <QFileInfo>
#include <QElapsedTimer>
#include <opencv2/imgcodecs.hpp>
#include <chrono>

namespace VisionForge {
namespace UI {

BatchProcessWorker::BatchProcessWorker(const QStringList& imageFiles,
                                       const QList<Algorithm::VisionTool*>& tools,
                                       QObject* parent)
    : QThread(parent)
    , imageFiles_(imageFiles)
    , tools_(tools)
    , state_(State::Idle)
{
    LOG_INFO(QString("BatchProcessWorker创建: %1个文件, %2个工具")
             .arg(imageFiles_.size()).arg(tools_.size()));
}

BatchProcessWorker::~BatchProcessWorker()
{
    // 确保线程已停止
    if (isRunning()) {
        cancel();
        wait(5000);  // 等待最多5秒
    }

    LOG_INFO("BatchProcessWorker销毁");
}

void BatchProcessWorker::pause()
{
    State expected = State::Running;
    if (state_.compare_exchange_strong(expected, State::Paused)) {
        setState(State::Paused);
        LOG_INFO("批量处理已暂停");
    }
}

void BatchProcessWorker::resume()
{
    State expected = State::Paused;
    if (state_.compare_exchange_strong(expected, State::Running)) {
        setState(State::Running);
        pauseCondition_.wakeAll();  // 唤醒等待线程
        LOG_INFO("批量处理已恢复");
    }
}

void BatchProcessWorker::cancel()
{
    if (state_.load() != State::Finished) {
        setState(State::Cancelled);
        pauseCondition_.wakeAll();  // 唤醒可能在暂停的线程
        LOG_INFO("批量处理已取消");
    }
}

void BatchProcessWorker::setState(State newState)
{
    state_.store(newState);
    emit stateChanged(newState);
}

void BatchProcessWorker::waitForResume()
{
    QMutexLocker locker(&pauseMutex_);
    while (state_.load() == State::Paused) {
        pauseCondition_.wait(&pauseMutex_);
    }
}

void BatchProcessWorker::run()
{
    LOG_INFO(QString("BatchProcessWorker开始: %1个文件").arg(imageFiles_.size()));

    setState(State::Running);

    QElapsedTimer totalTimer;
    totalTimer.start();

    int totalFiles = imageFiles_.size();

    for (int i = 0; i < totalFiles; ++i) {
        // 检查取消状态
        if (state_.load() == State::Cancelled) {
            LOG_WARNING("批量处理被用户取消");
            break;
        }

        // 检查暂停状态
        if (state_.load() == State::Paused) {
            LOG_INFO(QString("批量处理暂停于第%1/%2张图片").arg(i + 1).arg(totalFiles));
            waitForResume();

            // 恢复后再次检查是否被取消
            if (state_.load() == State::Cancelled) {
                LOG_WARNING("批量处理在恢复后被取消");
                break;
            }
        }

        // 处理单个文件
        const QString& filePath = imageFiles_[i];

        try {
            FileProcessResult result = processSingleFile(filePath, i);

            // 更新统计
            processedCount_++;
            if (result.success) {
                successCount_++;
            } else {
                failedCount_++;
            }

            // 发送单文件处理完成信号
            emit fileProcessed(result);

            // 发送进度信号
            int current = i + 1;
            double percentage = (current * 100.0) / totalFiles;
            emit progress(current, totalFiles, percentage);

        } catch (const std::exception& e) {
            LOG_ERROR(QString("处理文件异常: %1, 错误: %2")
                      .arg(QFileInfo(filePath).fileName())
                      .arg(e.what()));

            failedCount_++;

            FileProcessResult result;
            result.filePath = filePath;
            result.index = i;
            result.success = false;
            result.errorMessage = QString("异常: %1").arg(e.what());
            result.processTime = 0.0;

            emit fileProcessed(result);
            emit error(result.errorMessage);
        }
    }

    qint64 totalElapsed = totalTimer.elapsed();

    // 构造批量结果
    BatchProcessResult batchResult;
    batchResult.successCount = successCount_.load();
    batchResult.failedCount = failedCount_.load();
    batchResult.totalTime = totalElapsed;

    if (state_.load() == State::Cancelled) {
        batchResult.success = false;
        batchResult.errorMessage = "用户取消";
    } else {
        batchResult.success = (batchResult.failedCount == 0);
    }

    if (batchResult.successCount > 0) {
        batchResult.avgTime = batchResult.totalTime / batchResult.successCount;
    } else {
        batchResult.avgTime = 0.0;
    }

    setState(State::Finished);

    LOG_INFO(QString("BatchProcessWorker完成: 成功%1, 失败%2, 总耗时%3ms")
             .arg(batchResult.successCount)
             .arg(batchResult.failedCount)
             .arg(batchResult.totalTime));

    emit finished(batchResult);
}

FileProcessResult BatchProcessWorker::processSingleFile(const QString& filePath, int index)
{
    QElapsedTimer timer;
    timer.start();

    FileProcessResult result;
    result.filePath = filePath;
    result.index = index;
    result.success = false;

    // 1. 加载图片（使用ScopedImagePtr - 关键改进！）
    Base::ScopedImagePtr image = loadImage(filePath);

    if (!image) {
        result.errorMessage = "加载图片失败";
        result.processTime = timer.elapsed();
        return result;
    }

    // 2. 执行工具链处理
    QString errorMessage;
    bool success = processToolChain(image, errorMessage);

    result.success = success;
    result.errorMessage = errorMessage;
    result.processTime = timer.elapsed();

    // 3. ScopedImagePtr自动归还内存池 ✅

    return result;
}

Base::ScopedImagePtr BatchProcessWorker::loadImage(const QString& filePath)
{
    try {
        // 读取文件
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly)) {
            LOG_WARNING(QString("无法打开文件: %1").arg(filePath));
            return Base::ScopedImagePtr();  // 返回空指针
        }

        QByteArray fileData = file.readAll();
        file.close();

        if (fileData.isEmpty()) {
            LOG_WARNING(QString("文件为空: %1").arg(filePath));
            return Base::ScopedImagePtr();
        }

        // 解码图像
        std::vector<uchar> buffer(fileData.begin(), fileData.end());
        cv::Mat mat = cv::imdecode(buffer, cv::IMREAD_COLOR);

        if (mat.empty()) {
            LOG_WARNING(QString("无法解码图像: %1").arg(filePath));
            return Base::ScopedImagePtr();
        }

        // 创建ScopedImagePtr（从内存池分配）✅
        Base::ScopedImagePtr image(mat.cols, mat.rows, mat.type());

        if (!image) {
            LOG_ERROR(QString("内存池分配失败: %1x%2").arg(mat.cols).arg(mat.rows));
            return Base::ScopedImagePtr();
        }

        // 复制数据到内存池图像
        mat.copyTo(image->mat());

        return image;  // 移动语义返回

    } catch (const cv::Exception& e) {
        LOG_ERROR(QString("OpenCV异常: %1, 文件: %2")
                  .arg(e.what()).arg(QFileInfo(filePath).fileName()));
        return Base::ScopedImagePtr();
    } catch (const std::exception& e) {
        LOG_ERROR(QString("加载图片异常: %1, 文件: %2")
                  .arg(e.what()).arg(QFileInfo(filePath).fileName()));
        return Base::ScopedImagePtr();
    }
}

bool BatchProcessWorker::processToolChain(Base::ScopedImagePtr& image, QString& errorMessage)
{
    if (!image) {
        errorMessage = "输入图像为空";
        return false;
    }

    // 获取底层shared_ptr用于传递给VisionTool
    Base::ImageData::Ptr currentImage = image.get();

    for (Algorithm::VisionTool* tool : tools_) {
        // 再次检查取消状态
        if (state_.load() == State::Cancelled) {
            errorMessage = "用户取消";
            return false;
        }

        if (!tool) {
            errorMessage = "工具指针为空";
            LOG_ERROR(errorMessage);
            return false;
        }

        try {
            Algorithm::ToolResult result;
            bool success = tool->process(currentImage, result);

            if (!success || !result.success) {
                errorMessage = QString("工具 %1 处理失败: %2")
                               .arg(tool->toolName())
                               .arg(result.errorMessage);
                LOG_WARNING(errorMessage);
                return false;
            }

            // 更新当前图像（如果工具输出了新图像）
            if (result.outputImage) {
                currentImage = result.outputImage;
            }

        } catch (const std::exception& e) {
            errorMessage = QString("工具 %1 抛出异常: %2")
                           .arg(tool->toolName())
                           .arg(e.what());
            LOG_ERROR(errorMessage);
            return false;
        }
    }

    return true;
}

QString batchProcessStateToString(BatchProcessWorker::State state)
{
    switch (state) {
        case BatchProcessWorker::State::Idle:
            return "空闲";
        case BatchProcessWorker::State::Running:
            return "运行中";
        case BatchProcessWorker::State::Paused:
            return "已暂停";
        case BatchProcessWorker::State::Cancelled:
            return "已取消";
        case BatchProcessWorker::State::Finished:
            return "已完成";
        default:
            return "未知";
    }
}

} // namespace UI
} // namespace VisionForge
