/**
 * @file HighFrequencyBatchScenario.cpp
 * @brief 高频批量处理测试场景实现
 * @author VisionForge Team
 * @date 2025-12-20
 */

#include "test/HighFrequencyBatchScenario.h"
#include "base/Logger.h"
#include "base/ImageMemoryPool.h"
#include <QFile>
#include <QFileInfo>
#include <opencv2/imgcodecs.hpp>
#include <vector>

namespace VisionForge {
namespace Test {

HighFrequencyBatchScenario::HighFrequencyBatchScenario(const QStringList& imageFiles,
                                                       const QList<Algorithm::VisionTool*>& tools,
                                                       int durationSeconds,
                                                       int batchSize,
                                                       int batchIntervalSeconds,
                                                       QObject* parent)
    : TestScenario("高频批量处理", parent)
    , imageFiles_(imageFiles)
    , tools_(tools)
    , durationSeconds_(durationSeconds)
    , batchSize_(batchSize)
    , batchIntervalSeconds_(batchIntervalSeconds)
    , batchTimer_(new QTimer(this))
    , totalBatches_(0)
    , completedBatches_(0)
    , currentBatchIndex_(0)
    , shouldStop_(false)
    , isPaused_(false)
{
    // 计算总批次数
    totalBatches_ = durationSeconds_ / batchIntervalSeconds_;

    connect(batchTimer_, &QTimer::timeout, this, &HighFrequencyBatchScenario::onBatchTimer);

    LOG_INFO(QString("HighFrequencyBatchScenario创建: %1批次, 每批%2张, 间隔%3秒")
             .arg(totalBatches_).arg(batchSize_).arg(batchIntervalSeconds_));
}

HighFrequencyBatchScenario::~HighFrequencyBatchScenario()
{
    stop();
}

void HighFrequencyBatchScenario::start()
{
    if (state_ != State::Idle) {
        LOG_WARNING("场景已在运行中");
        return;
    }

    setState(State::Running);

    shouldStop_.store(false);
    isPaused_.store(false);
    completedBatches_ = 0;
    currentBatchIndex_ = 0;

    scenarioTimer_.start();

    // 立即执行第一批
    processBatch();

    // 启动定时器
    batchTimer_->start(batchIntervalSeconds_ * 1000);

    LOG_INFO("开始高频批量处理场景");
}

void HighFrequencyBatchScenario::stop()
{
    if (state_ == State::Idle || state_ == State::Stopped) {
        return;
    }

    batchTimer_->stop();
    shouldStop_.store(true);
    setState(State::Stopped);

    LOG_INFO("停止高频批量处理场景");
}

void HighFrequencyBatchScenario::pause()
{
    TestScenario::pause();
    isPaused_.store(true);
    LOG_INFO("暂停高频批量处理场景");
}

void HighFrequencyBatchScenario::resume()
{
    TestScenario::resume();
    isPaused_.store(false);
    LOG_INFO("恢复高频批量处理场景");
}

int HighFrequencyBatchScenario::progress() const
{
    if (totalBatches_ == 0) {
        return 0;
    }

    return (completedBatches_ * 100) / totalBatches_;
}

QString HighFrequencyBatchScenario::getStatistics() const
{
    qint64 elapsedSeconds = scenarioTimer_.elapsed() / 1000;

    QString stats;
    stats += QString("高频批量处理场景统计:\n");
    stats += QString("  运行时长: %1 秒\n").arg(elapsedSeconds);
    stats += QString("  已完成批次: %1/%2\n").arg(completedBatches_).arg(totalBatches_);
    stats += QString("  总操作数: %1\n").arg(totalOperations_);
    stats += QString("  失败操作数: %1\n").arg(failedOperations_);
    stats += QString("  进度: %1%\n").arg(progress());

    return stats;
}

void HighFrequencyBatchScenario::onBatchTimer()
{
    // 检查是否暂停
    if (isPaused_.load()) {
        return;
    }

    // 检查是否停止
    if (shouldStop_.load()) {
        stop();
        return;
    }

    // 检查是否超时
    qint64 elapsedSeconds = scenarioTimer_.elapsed() / 1000;
    if (elapsedSeconds >= durationSeconds_) {
        LOG_INFO("高频批量处理场景完成（时间到）");
        setState(State::Finished);
        batchTimer_->stop();
        emit finished();
        return;
    }

    // 检查是否完成所有批次
    if (completedBatches_ >= totalBatches_) {
        LOG_INFO("高频批量处理场景完成（批次完成）");
        setState(State::Finished);
        batchTimer_->stop();
        emit finished();
        return;
    }

    // 处理下一批
    processBatch();
}

void HighFrequencyBatchScenario::onBatchFinished()
{
    completedBatches_++;
    emit progressChanged(progress());

    LOG_DEBUG(QString("批次 %1/%2 完成").arg(completedBatches_).arg(totalBatches_));
}

void HighFrequencyBatchScenario::processBatch()
{
    if (imageFiles_.isEmpty()) {
        LOG_ERROR("图像文件列表为空");
        emit error("图像文件列表为空");
        return;
    }

    LOG_DEBUG(QString("开始处理批次 %1/%2").arg(completedBatches_ + 1).arg(totalBatches_));

    // 处理一批图像
    for (int i = 0; i < batchSize_; ++i) {
        // 检查暂停或停止
        if (isPaused_.load() || shouldStop_.load()) {
            break;
        }

        // 循环选择图像
        int imageIndex = currentBatchIndex_ % imageFiles_.size();
        currentBatchIndex_++;

        processSingleImage(imageIndex);
    }

    onBatchFinished();
}

void HighFrequencyBatchScenario::processSingleImage(int index)
{
    totalOperations_++;

    try {
        const QString& filePath = imageFiles_[index];

        // 读取图像文件
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly)) {
            failedOperations_++;
            QString errorMsg = QString("无法打开文件: %1").arg(filePath);
            emit operationCompleted(false, errorMsg);
            return;
        }

        QByteArray fileData = file.readAll();
        file.close();

        // 解码图像
        std::vector<uchar> buffer(fileData.begin(), fileData.end());
        cv::Mat mat = cv::imdecode(buffer, cv::IMREAD_COLOR);

        if (mat.empty()) {
            failedOperations_++;
            QString errorMsg = QString("无法解码图像: %1").arg(filePath);
            emit operationCompleted(false, errorMsg);
            return;
        }

        // 从内存池分配图像（关键：使用RAII）
        auto poolImage = Base::ImageMemoryPool::instance().allocate(
            mat.cols, mat.rows, mat.type());

        if (!poolImage) {
            failedOperations_++;
            QString errorMsg = "内存池分配失败";
            emit operationCompleted(false, errorMsg);
            return;
        }

        mat.copyTo(poolImage->mat());

        // 执行工具链处理
        Base::ImageData::Ptr currentImage = poolImage;

        for (Algorithm::VisionTool* tool : tools_) {
            if (!tool) {
                failedOperations_++;
                QString errorMsg = "工具指针为空";
                emit operationCompleted(false, errorMsg);
                return;
            }

            Algorithm::ToolResult result;
            bool success = tool->process(currentImage, result);

            if (!success || !result.success) {
                failedOperations_++;
                QString errorMsg = QString("工具 %1 处理失败: %2")
                                   .arg(tool->toolName())
                                   .arg(result.errorMessage);
                emit operationCompleted(false, errorMsg);
                return;
            }

            // 更新当前图像
            if (result.outputImage) {
                currentImage = result.outputImage;
            }
        }

        // 成功
        emit operationCompleted(true, QString("图像 %1 处理成功").arg(QFileInfo(filePath).fileName()));

    } catch (const std::exception& e) {
        failedOperations_++;
        QString errorMsg = QString("处理异常: %1").arg(e.what());
        emit operationCompleted(false, errorMsg);
        LOG_ERROR(errorMsg);
    }

    // poolImage自动归还内存池 ✅
}

} // namespace Test
} // namespace VisionForge
