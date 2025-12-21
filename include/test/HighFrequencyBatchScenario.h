/**
 * @file HighFrequencyBatchScenario.h
 * @brief 高频批量处理测试场景
 * @details 每5分钟处理100张图片，持续24小时（288次循环）
 * @author VisionForge Team
 * @date 2025-12-20
 */

#pragma once

#include "test/StabilityTestFramework.h"
#include "algorithm/VisionTool.h"
#include <QTimer>
#include <QStringList>
#include <atomic>

namespace VisionForge {
namespace Test {

/**
 * @class HighFrequencyBatchScenario
 * @brief 高频批量处理场景
 *
 * **测试参数**:
 * - 总时长: 24小时
 * - 批量大小: 100张图片/批
 * - 批量间隔: 5分钟
 * - 总循环数: 288次
 *
 * **测试目标**:
 * - 验证批量处理内存管理
 * - 检测内存池效率
 * - 确保无内存泄漏
 *
 * **使用示例**:
 * @code
 * QList<Algorithm::VisionTool*> tools = {
 *     new GrayTool(),
 *     new ThresholdTool()
 * };
 *
 * auto scenario = new HighFrequencyBatchScenario(
 *     imageList,
 *     tools,
 *     24 * 3600  // 24小时
 * );
 *
 * framework.addScenario(scenario);
 * @endcode
 */
class HighFrequencyBatchScenario : public TestScenario
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param imageFiles 图像文件列表（用于循环加载）
     * @param tools 工具链（不获取所有权）
     * @param durationSeconds 测试时长(秒)，默认24小时
     * @param batchSize 批量大小，默认100
     * @param batchIntervalSeconds 批量间隔(秒)，默认5分钟
     * @param parent 父对象
     */
    explicit HighFrequencyBatchScenario(const QStringList& imageFiles,
                                        const QList<Algorithm::VisionTool*>& tools,
                                        int durationSeconds = 24 * 3600,
                                        int batchSize = 100,
                                        int batchIntervalSeconds = 5 * 60,
                                        QObject* parent = nullptr);

    ~HighFrequencyBatchScenario() override;

    // ========== TestScenario接口实现 ==========

    void start() override;
    void stop() override;
    void pause() override;
    void resume() override;
    int progress() const override;
    QString getStatistics() const override;

private slots:
    void onBatchTimer();
    void onBatchFinished();

private:
    void processBatch();
    void processSingleImage(int index);

private:
    QStringList imageFiles_;
    QList<Algorithm::VisionTool*> tools_;
    int durationSeconds_;
    int batchSize_;
    int batchIntervalSeconds_;

    QTimer* batchTimer_;
    QElapsedTimer scenarioTimer_;

    int totalBatches_;          ///< 总批次数
    int completedBatches_;      ///< 已完成批次数
    int currentBatchIndex_;     ///< 当前批次索引

    std::atomic<bool> shouldStop_;
    std::atomic<bool> isPaused_;
};

} // namespace Test
} // namespace VisionForge
