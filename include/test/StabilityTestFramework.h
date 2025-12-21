/**
 * @file StabilityTestFramework.h
 * @brief 稳定性测试框架 - 用于72小时连续运行测试
 * @details 协调测试场景、监控器、数据收集和报告生成
 * @author VisionForge Team
 * @date 2025-12-20
 */

#pragma once

#include "base/SystemResourceMonitor.h"
#include "base/PoolPerformanceMonitor.h"
#include <QObject>
#include <QString>
#include <QList>
#include <QJsonObject>
#include <QElapsedTimer>
#include <memory>

namespace VisionForge {
namespace Test {

// 前向声明
class TestScenario;

/**
 * @brief 测试结果
 */
struct TestResult {
    bool success;                   ///< 是否成功
    QString scenarioName;           ///< 场景名称
    qint64 durationSeconds;         ///< 测试时长(秒)
    int totalOperations;            ///< 总操作数
    int failedOperations;           ///< 失败操作数

    // 资源统计
    Base::ResourceStats resourceStats;      ///< 系统资源统计
    Base::PoolPerformanceStats poolStats;   ///< 池性能统计

    // 错误信息
    QStringList errors;             ///< 错误列表

    // 验收标准检查结果
    bool passMemoryGrowth;          ///< 内存增长是否达标
    bool passCpuUsage;              ///< CPU使用率是否达标
    bool passPoolHitRate;           ///< 池命中率是否达标
    bool passNoCrash;               ///< 是否无崩溃

    TestResult();

    /**
     * @brief 转换为JSON
     */
    QJsonObject toJson() const;

    /**
     * @brief 从JSON加载
     */
    static TestResult fromJson(const QJsonObject& json);
};

/**
 * @brief 测试场景基类（抽象）
 */
class TestScenario : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief 场景状态
     */
    enum class State {
        Idle,       ///< 空闲
        Running,    ///< 运行中
        Paused,     ///< 已暂停
        Stopped,    ///< 已停止
        Finished    ///< 已完成
    };

    explicit TestScenario(const QString& name, QObject* parent = nullptr);
    virtual ~TestScenario();

    /**
     * @brief 获取场景名称
     */
    QString name() const { return name_; }

    /**
     * @brief 获取当前状态
     */
    State state() const { return state_; }

    /**
     * @brief 启动测试场景
     */
    virtual void start() = 0;

    /**
     * @brief 停止测试场景
     */
    virtual void stop() = 0;

    /**
     * @brief 暂停测试场景
     */
    virtual void pause();

    /**
     * @brief 恢复测试场景
     */
    virtual void resume();

    /**
     * @brief 获取当前进度(0-100)
     */
    virtual int progress() const = 0;

    /**
     * @brief 获取统计信息
     */
    virtual QString getStatistics() const = 0;

signals:
    /**
     * @brief 状态变化信号
     */
    void stateChanged(State newState);

    /**
     * @brief 进度更新信号
     */
    void progressChanged(int percentage);

    /**
     * @brief 操作完成信号
     */
    void operationCompleted(bool success, const QString& message);

    /**
     * @brief 错误信号
     */
    void error(const QString& errorMessage);

    /**
     * @brief 场景完成信号
     */
    void finished();

protected:
    void setState(State newState);

    QString name_;
    State state_;
    QElapsedTimer timer_;
    int totalOperations_;
    int completedOperations_;
    int failedOperations_;
};

/**
 * @class StabilityTestFramework
 * @brief 稳定性测试框架
 *
 * **功能**:
 * - 管理测试场景的执行
 * - 协调资源监控器
 * - 收集测试数据
 * - 生成测试报告
 * - 验收标准检查
 *
 * **使用示例**:
 * @code
 * StabilityTestFramework framework;
 *
 * // 添加测试场景
 * framework.addScenario(new HighFrequencyBatchScenario());
 *
 * // 设置验收标准
 * framework.setMemoryGrowthThreshold(100);  // MB/72h
 * framework.setCpuUsageThreshold(60.0);     // %
 * framework.setPoolHitRateThreshold(80.0);  // %
 *
 * // 启动测试
 * framework.startTest("72小时稳定性测试");
 *
 * // 等待完成...
 *
 * // 生成报告
 * TestResult result = framework.getResult();
 * framework.generateReport("report.json");
 * @endcode
 */
class StabilityTestFramework : public QObject
{
    Q_OBJECT

public:
    explicit StabilityTestFramework(QObject* parent = nullptr);
    ~StabilityTestFramework();

    // ========== 场景管理 ==========

    /**
     * @brief 添加测试场景
     * @param scenario 场景对象（框架获取所有权）
     */
    void addScenario(TestScenario* scenario);

    /**
     * @brief 移除所有场景
     */
    void clearScenarios();

    /**
     * @brief 获取场景列表
     */
    QList<TestScenario*> scenarios() const { return scenarios_; }

    // ========== 验收标准配置 ==========

    /**
     * @brief 设置内存增长阈值
     * @param mbPer72h 72小时内允许的最大内存增长(MB)
     */
    void setMemoryGrowthThreshold(double mbPer72h);

    /**
     * @brief 设置CPU使用率阈值
     * @param percent CPU使用率阈值(%)
     */
    void setCpuUsageThreshold(double percent);

    /**
     * @brief 设置池命中率阈值
     * @param percent 池命中率阈值(%)
     */
    void setPoolHitRateThreshold(double percent);

    // ========== 测试控制 ==========

    /**
     * @brief 启动测试
     * @param testName 测试名称
     */
    void startTest(const QString& testName);

    /**
     * @brief 停止测试
     */
    void stopTest();

    /**
     * @brief 暂停测试
     */
    void pauseTest();

    /**
     * @brief 恢复测试
     */
    void resumeTest();

    /**
     * @brief 是否正在运行
     */
    bool isRunning() const { return isRunning_; }

    // ========== 数据获取 ==========

    /**
     * @brief 获取测试结果
     */
    TestResult getResult() const;

    /**
     * @brief 获取当前测试时长(秒)
     */
    qint64 getElapsedSeconds() const;

    /**
     * @brief 获取总进度(0-100)
     */
    int getTotalProgress() const;

    // ========== 报告生成 ==========

    /**
     * @brief 生成JSON报告
     * @param filePath 报告文件路径
     * @return 是否成功
     */
    bool generateReport(const QString& filePath);

    /**
     * @brief 生成文本摘要
     */
    QString generateSummary() const;

signals:
    /**
     * @brief 测试开始信号
     */
    void testStarted(const QString& testName);

    /**
     * @brief 测试完成信号
     */
    void testFinished(const TestResult& result);

    /**
     * @brief 进度更新信号
     */
    void progressUpdated(int percentage);

    /**
     * @brief 验收标准失败信号
     */
    void acceptanceCriteriaFailed(const QString& criteria, const QString& reason);

private slots:
    void onScenarioFinished();
    void onScenarioError(const QString& error);
    void onResourceSnapshot(const Base::ResourceSnapshot& snapshot);
    void onPoolSnapshot(const Base::PoolSnapshot& snapshot);

private:
    void startMonitoring();
    void stopMonitoring();
    void checkAcceptanceCriteria();
    TestResult buildResult() const;

private:
    QString testName_;
    QList<TestScenario*> scenarios_;
    bool isRunning_;
    QElapsedTimer testTimer_;

    // 监控器
    Base::SystemResourceMonitor* resourceMonitor_;
    Base::PoolPerformanceMonitor* poolMonitor_;

    // 验收标准
    double memoryGrowthThreshold_;      ///< MB/72h
    double cpuUsageThreshold_;          ///< %
    double poolHitRateThreshold_;       ///< %

    // 测试数据
    QList<Base::ResourceSnapshot> resourceSnapshots_;
    QList<Base::PoolSnapshot> poolSnapshots_;
    QStringList errors_;

    // 结果统计
    int totalOperations_;
    int failedOperations_;
};

/**
 * @brief 测试场景状态转字符串
 */
QString testScenarioStateToString(TestScenario::State state);

} // namespace Test
} // namespace VisionForge
