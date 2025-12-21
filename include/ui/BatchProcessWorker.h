/**
 * @file BatchProcessWorker.h
 * @brief 批量处理后台工作线程（改进版）
 * @details 解决UI冻结和内存泄漏问题
 * @author VisionForge Team
 * @date 2025-12-20
 */

#pragma once

#include "base/ImageData.h"
#include "base/ScopedImagePtr.h"
#include "algorithm/VisionTool.h"
#include <QThread>
#include <QString>
#include <QStringList>
#include <QList>
#include <QMutex>
#include <QWaitCondition>
#include <atomic>
#include <vector>

namespace VisionForge {
namespace UI {

/**
 * @brief 批量处理结果
 */
struct BatchProcessResult {
    bool success;           ///< 是否成功
    int successCount;       ///< 成功数量
    int failedCount;        ///< 失败数量
    double totalTime;       ///< 总耗时(ms)
    double avgTime;         ///< 平均耗时(ms)
    QString errorMessage;   ///< 错误信息
};

/**
 * @brief 单文件处理结果
 */
struct FileProcessResult {
    QString filePath;       ///< 文件路径
    bool success;           ///< 是否成功
    QString errorMessage;   ///< 错误信息
    double processTime;     ///< 处理耗时(ms)
    int index;              ///< 索引
};

/**
 * @class BatchProcessWorker
 * @brief 批量处理后台工作线程（改进版InspectionWorker）
 *
 * **改进点**:
 * 1. ✅ 使用ScopedImagePtr避免内存泄漏
 * 2. ✅ 支持暂停/恢复功能
 * 3. ✅ 完善的异常处理
 * 4. ✅ 详细的进度报告
 * 5. ✅ 线程安全的状态管理
 *
 * **使用示例**:
 * @code
 * // 创建工作线程
 * auto worker = new BatchProcessWorker(fileList, tools);
 *
 * // 连接信号
 * connect(worker, &BatchProcessWorker::progress, this, &MainWindow::onProgress);
 * connect(worker, &BatchProcessWorker::fileProcessed, this, &MainWindow::onFileProcessed);
 * connect(worker, &BatchProcessWorker::finished, this, &MainWindow::onBatchFinished);
 *
 * // 启动处理
 * worker->start();
 *
 * // 暂停
 * worker->pause();
 *
 * // 恢复
 * worker->resume();
 *
 * // 取消
 * worker->cancel();
 * @endcode
 */
class BatchProcessWorker : public QThread
{
    Q_OBJECT

public:
    /**
     * @brief 工作状态
     */
    enum class State {
        Idle,       ///< 空闲
        Running,    ///< 运行中
        Paused,     ///< 已暂停
        Cancelled,  ///< 已取消
        Finished    ///< 已完成
    };

    /**
     * @brief 构造函数
     * @param imageFiles 图像文件列表
     * @param tools 工具链
     * @param parent 父对象
     */
    explicit BatchProcessWorker(const QStringList& imageFiles,
                                const QList<Algorithm::VisionTool*>& tools,
                                QObject* parent = nullptr);

    /**
     * @brief 析构函数
     */
    ~BatchProcessWorker() override;

    // ========== 状态控制 ==========

    /**
     * @brief 暂停处理
     * @note 线程安全
     */
    void pause();

    /**
     * @brief 恢复处理
     * @note 线程安全
     */
    void resume();

    /**
     * @brief 取消处理
     * @note 线程安全，会等待当前图片处理完成
     */
    void cancel();

    /**
     * @brief 获取当前状态
     */
    State currentState() const { return state_.load(); }

    /**
     * @brief 检查是否正在运行
     */
    bool isRunning() const { return state_.load() == State::Running; }

    /**
     * @brief 检查是否已暂停
     */
    bool isPaused() const { return state_.load() == State::Paused; }

    /**
     * @brief 检查是否已取消
     */
    bool isCancelled() const { return state_.load() == State::Cancelled; }

signals:
    /**
     * @brief 进度信号
     * @param current 当前索引（从1开始）
     * @param total 总数
     * @param percentage 百分比（0-100）
     */
    void progress(int current, int total, double percentage);

    /**
     * @brief 单文件处理完成
     * @param result 处理结果
     */
    void fileProcessed(const FileProcessResult& result);

    /**
     * @brief 状态变化信号
     * @param state 新状态
     */
    void stateChanged(State state);

    /**
     * @brief 批量处理完成
     * @param result 批量结果
     */
    void finished(const BatchProcessResult& result);

    /**
     * @brief 错误信号
     * @param errorMessage 错误信息
     */
    void error(const QString& errorMessage);

protected:
    /**
     * @brief 线程执行函数
     */
    void run() override;

private:
    /**
     * @brief 处理单张图片
     * @param filePath 文件路径
     * @param index 索引
     * @return 处理结果
     */
    FileProcessResult processSingleFile(const QString& filePath, int index);

    /**
     * @brief 加载图片（使用ScopedImagePtr）
     * @param filePath 文件路径
     * @return 图片指针
     */
    Base::ScopedImagePtr loadImage(const QString& filePath);

    /**
     * @brief 执行工具链处理
     * @param image 输入图片
     * @return 是否成功
     */
    bool processToolChain(Base::ScopedImagePtr& image, QString& errorMessage);

    /**
     * @brief 设置状态
     * @param newState 新状态
     */
    void setState(State newState);

    /**
     * @brief 等待恢复（暂停时调用）
     */
    void waitForResume();

private:
    QStringList imageFiles_;                        ///< 图像文件列表
    QList<Algorithm::VisionTool*> tools_;           ///< 工具链
    std::atomic<State> state_;                      ///< 当前状态
    QMutex pauseMutex_;                             ///< 暂停互斥锁
    QWaitCondition pauseCondition_;                 ///< 暂停条件变量

    // 统计信息
    std::atomic<int> processedCount_{0};            ///< 已处理数量
    std::atomic<int> successCount_{0};              ///< 成功数量
    std::atomic<int> failedCount_{0};               ///< 失败数量
    std::atomic<double> totalProcessTime_{0.0};     ///< 总处理时间
};

/**
 * @brief 状态名称转换
 */
QString batchProcessStateToString(BatchProcessWorker::State state);

} // namespace UI
} // namespace VisionForge
