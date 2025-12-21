/**
 * @file StabilityTestFramework.cpp
 * @brief 稳定性测试框架实现
 * @author VisionForge Team
 * @date 2025-12-20
 */

#include "test/StabilityTestFramework.h"
#include "base/Logger.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDateTime>

namespace VisionForge {
namespace Test {

// ========== TestResult实现 ==========

TestResult::TestResult()
    : success(false)
    , durationSeconds(0)
    , totalOperations(0)
    , failedOperations(0)
    , passMemoryGrowth(false)
    , passCpuUsage(false)
    , passPoolHitRate(false)
    , passNoCrash(false)
{
}

QJsonObject TestResult::toJson() const
{
    QJsonObject json;

    json["success"] = success;
    json["scenarioName"] = scenarioName;
    json["durationSeconds"] = static_cast<double>(durationSeconds);
    json["totalOperations"] = totalOperations;
    json["failedOperations"] = failedOperations;

    // 资源统计
    QJsonObject resourceJson;
    resourceJson["memoryInitialMB"] = static_cast<double>(resourceStats.memoryInitialMB);
    resourceJson["memoryCurrentMB"] = static_cast<double>(resourceStats.memoryCurrentMB);
    resourceJson["memoryPeakMB"] = static_cast<double>(resourceStats.memoryPeakMB);
    resourceJson["memoryGrowthRateKBh"] = resourceStats.memoryGrowthRateKBh;
    resourceJson["memoryGrowthTotalMB"] = resourceStats.memoryGrowthTotalMB;
    resourceJson["cpuAvgPercent"] = resourceStats.cpuAvgPercent;
    resourceJson["cpuPeakPercent"] = resourceStats.cpuPeakPercent;
    resourceJson["threadCountAvg"] = resourceStats.threadCountAvg;
    resourceJson["threadCountPeak"] = resourceStats.threadCountPeak;
    json["resourceStats"] = resourceJson;

    // 池性能统计
    QJsonObject poolJson;
    poolJson["avgHitRate"] = poolStats.avgHitRate;
    poolJson["minHitRate"] = poolStats.minHitRate;
    poolJson["currentHitRate"] = poolStats.currentHitRate;
    poolJson["avgPoolSize"] = static_cast<double>(poolStats.avgPoolSize);
    poolJson["peakPoolSize"] = static_cast<double>(poolStats.peakPoolSize);
    poolJson["avgMemoryMB"] = static_cast<double>(poolStats.avgMemoryMB);
    poolJson["peakMemoryMB"] = static_cast<double>(poolStats.peakMemoryMB);
    json["poolStats"] = poolJson;

    // 错误列表
    QJsonArray errorsArray;
    for (const QString& error : errors) {
        errorsArray.append(error);
    }
    json["errors"] = errorsArray;

    // 验收标准检查
    json["passMemoryGrowth"] = passMemoryGrowth;
    json["passCpuUsage"] = passCpuUsage;
    json["passPoolHitRate"] = passPoolHitRate;
    json["passNoCrash"] = passNoCrash;

    return json;
}

TestResult TestResult::fromJson(const QJsonObject& json)
{
    TestResult result;

    result.success = json["success"].toBool();
    result.scenarioName = json["scenarioName"].toString();
    result.durationSeconds = static_cast<qint64>(json["durationSeconds"].toDouble());
    result.totalOperations = json["totalOperations"].toInt();
    result.failedOperations = json["failedOperations"].toInt();

    // 资源统计
    QJsonObject resourceJson = json["resourceStats"].toObject();
    result.resourceStats.memoryInitialMB = static_cast<qint64>(resourceJson["memoryInitialMB"].toDouble());
    result.resourceStats.memoryCurrentMB = static_cast<qint64>(resourceJson["memoryCurrentMB"].toDouble());
    result.resourceStats.memoryPeakMB = static_cast<qint64>(resourceJson["memoryPeakMB"].toDouble());
    result.resourceStats.memoryGrowthRateKBh = resourceJson["memoryGrowthRateKBh"].toDouble();
    result.resourceStats.memoryGrowthTotalMB = resourceJson["memoryGrowthTotalMB"].toDouble();
    result.resourceStats.cpuAvgPercent = resourceJson["cpuAvgPercent"].toDouble();
    result.resourceStats.cpuPeakPercent = resourceJson["cpuPeakPercent"].toDouble();
    result.resourceStats.threadCountAvg = resourceJson["threadCountAvg"].toInt();
    result.resourceStats.threadCountPeak = resourceJson["threadCountPeak"].toInt();

    // 池性能统计
    QJsonObject poolJson = json["poolStats"].toObject();
    result.poolStats.avgHitRate = poolJson["avgHitRate"].toDouble();
    result.poolStats.minHitRate = poolJson["minHitRate"].toDouble();
    result.poolStats.currentHitRate = poolJson["currentHitRate"].toDouble();
    result.poolStats.avgPoolSize = static_cast<size_t>(poolJson["avgPoolSize"].toDouble());
    result.poolStats.peakPoolSize = static_cast<size_t>(poolJson["peakPoolSize"].toDouble());
    result.poolStats.avgMemoryMB = static_cast<size_t>(poolJson["avgMemoryMB"].toDouble());
    result.poolStats.peakMemoryMB = static_cast<size_t>(poolJson["peakMemoryMB"].toDouble());

    // 错误列表
    QJsonArray errorsArray = json["errors"].toArray();
    for (const QJsonValue& errorValue : errorsArray) {
        result.errors.append(errorValue.toString());
    }

    // 验收标准检查
    result.passMemoryGrowth = json["passMemoryGrowth"].toBool();
    result.passCpuUsage = json["passCpuUsage"].toBool();
    result.passPoolHitRate = json["passPoolHitRate"].toBool();
    result.passNoCrash = json["passNoCrash"].toBool();

    return result;
}

// ========== TestScenario实现 ==========

TestScenario::TestScenario(const QString& name, QObject* parent)
    : QObject(parent)
    , name_(name)
    , state_(State::Idle)
    , totalOperations_(0)
    , completedOperations_(0)
    , failedOperations_(0)
{
}

TestScenario::~TestScenario()
{
}

void TestScenario::pause()
{
    if (state_ == State::Running) {
        setState(State::Paused);
    }
}

void TestScenario::resume()
{
    if (state_ == State::Paused) {
        setState(State::Running);
    }
}

void TestScenario::setState(State newState)
{
    if (state_ != newState) {
        state_ = newState;
        emit stateChanged(newState);
    }
}

// ========== StabilityTestFramework实现 ==========

StabilityTestFramework::StabilityTestFramework(QObject* parent)
    : QObject(parent)
    , isRunning_(false)
    , resourceMonitor_(&Base::SystemResourceMonitor::instance())
    , poolMonitor_(&Base::PoolPerformanceMonitor::instance())
    , memoryGrowthThreshold_(100.0)      // 默认100MB/72h
    , cpuUsageThreshold_(60.0)           // 默认60%
    , poolHitRateThreshold_(80.0)        // 默认80%
    , totalOperations_(0)
    , failedOperations_(0)
{
    // 连接资源监控器信号
    connect(resourceMonitor_, &Base::SystemResourceMonitor::snapshotReady,
            this, &StabilityTestFramework::onResourceSnapshot);

    // 连接池监控器信号
    connect(poolMonitor_, &Base::PoolPerformanceMonitor::snapshotReady,
            this, &StabilityTestFramework::onPoolSnapshot);

    LOG_INFO("StabilityTestFramework创建完成");
}

StabilityTestFramework::~StabilityTestFramework()
{
    clearScenarios();
    LOG_INFO("StabilityTestFramework销毁");
}

void StabilityTestFramework::addScenario(TestScenario* scenario)
{
    if (!scenario) {
        LOG_WARNING("尝试添加空场景");
        return;
    }

    scenarios_.append(scenario);
    scenario->setParent(this);

    // 连接场景信号
    connect(scenario, &TestScenario::finished,
            this, &StabilityTestFramework::onScenarioFinished);
    connect(scenario, &TestScenario::error,
            this, &StabilityTestFramework::onScenarioError);

    LOG_INFO(QString("添加测试场景: %1").arg(scenario->name()));
}

void StabilityTestFramework::clearScenarios()
{
    qDeleteAll(scenarios_);
    scenarios_.clear();
}

void StabilityTestFramework::setMemoryGrowthThreshold(double mbPer72h)
{
    memoryGrowthThreshold_ = mbPer72h;
    LOG_INFO(QString("设置内存增长阈值: %1 MB/72h").arg(mbPer72h));
}

void StabilityTestFramework::setCpuUsageThreshold(double percent)
{
    cpuUsageThreshold_ = percent;
    LOG_INFO(QString("设置CPU使用率阈值: %1%").arg(percent));
}

void StabilityTestFramework::setPoolHitRateThreshold(double percent)
{
    poolHitRateThreshold_ = percent;
    LOG_INFO(QString("设置池命中率阈值: %1%").arg(percent));
}

void StabilityTestFramework::startTest(const QString& testName)
{
    if (isRunning_) {
        LOG_WARNING("测试已在运行中");
        return;
    }

    if (scenarios_.isEmpty()) {
        LOG_ERROR("没有测试场景可运行");
        return;
    }

    testName_ = testName;
    isRunning_ = true;

    // 重置统计数据
    resourceSnapshots_.clear();
    poolSnapshots_.clear();
    errors_.clear();
    totalOperations_ = 0;
    failedOperations_ = 0;

    // 启动监控
    startMonitoring();

    // 启动所有场景
    testTimer_.start();

    for (TestScenario* scenario : scenarios_) {
        scenario->start();
    }

    LOG_INFO(QString("开始稳定性测试: %1").arg(testName));
    emit testStarted(testName);
}

void StabilityTestFramework::stopTest()
{
    if (!isRunning_) {
        return;
    }

    // 停止所有场景
    for (TestScenario* scenario : scenarios_) {
        scenario->stop();
    }

    // 停止监控
    stopMonitoring();

    isRunning_ = false;

    // 检查验收标准
    checkAcceptanceCriteria();

    // 构建结果
    TestResult result = buildResult();

    LOG_INFO(QString("停止稳定性测试: %1").arg(testName_));
    emit testFinished(result);
}

void StabilityTestFramework::pauseTest()
{
    if (!isRunning_) {
        return;
    }

    for (TestScenario* scenario : scenarios_) {
        scenario->pause();
    }

    LOG_INFO("暂停稳定性测试");
}

void StabilityTestFramework::resumeTest()
{
    if (!isRunning_) {
        return;
    }

    for (TestScenario* scenario : scenarios_) {
        scenario->resume();
    }

    LOG_INFO("恢复稳定性测试");
}

TestResult StabilityTestFramework::getResult() const
{
    return buildResult();
}

qint64 StabilityTestFramework::getElapsedSeconds() const
{
    if (!testTimer_.isValid()) {
        return 0;
    }

    return testTimer_.elapsed() / 1000;
}

int StabilityTestFramework::getTotalProgress() const
{
    if (scenarios_.isEmpty()) {
        return 0;
    }

    int totalProgress = 0;
    for (const TestScenario* scenario : scenarios_) {
        totalProgress += scenario->progress();
    }

    return totalProgress / scenarios_.size();
}

bool StabilityTestFramework::generateReport(const QString& filePath)
{
    TestResult result = buildResult();

    QJsonObject reportJson;
    reportJson["testName"] = testName_;
    reportJson["startTime"] = QDateTime::currentDateTime().toString(Qt::ISODate);
    reportJson["result"] = result.toJson();

    // 添加快照数据（可选，数据量较大）
    // ...

    QJsonDocument doc(reportJson);

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        LOG_ERROR(QString("无法创建报告文件: %1").arg(filePath));
        return false;
    }

    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();

    LOG_INFO(QString("生成测试报告: %1").arg(filePath));
    return true;
}

QString StabilityTestFramework::generateSummary() const
{
    TestResult result = buildResult();

    QString summary;
    summary += "========== 稳定性测试摘要 ==========\n\n";
    summary += QString("测试名称: %1\n").arg(testName_);
    summary += QString("测试时长: %1 秒\n").arg(result.durationSeconds);
    summary += QString("总操作数: %1\n").arg(result.totalOperations);
    summary += QString("失败操作数: %1\n").arg(result.failedOperations);
    summary += QString("成功率: %1%\n\n")
               .arg(result.totalOperations > 0 ?
                    (result.totalOperations - result.failedOperations) * 100.0 / result.totalOperations : 0.0, 0, 'f', 2);

    // 资源统计
    summary += "【资源统计】\n";
    summary += QString("  内存初始: %1 MB\n").arg(result.resourceStats.memoryInitialMB);
    summary += QString("  内存当前: %1 MB\n").arg(result.resourceStats.memoryCurrentMB);
    summary += QString("  内存峰值: %1 MB\n").arg(result.resourceStats.memoryPeakMB);
    summary += QString("  内存增长: %1 MB\n").arg(result.resourceStats.memoryGrowthTotalMB, 0, 'f', 2);
    summary += QString("  内存增长率: %1 KB/h\n").arg(result.resourceStats.memoryGrowthRateKBh, 0, 'f', 2);
    summary += QString("  平均CPU: %1%\n").arg(result.resourceStats.cpuAvgPercent, 0, 'f', 1);
    summary += QString("  峰值CPU: %1%\n\n").arg(result.resourceStats.cpuPeakPercent, 0, 'f', 1);

    // 池性能统计
    summary += "【池性能统计】\n";
    summary += QString("  平均命中率: %1%\n").arg(result.poolStats.avgHitRate, 0, 'f', 1);
    summary += QString("  当前命中率: %1%\n").arg(result.poolStats.currentHitRate, 0, 'f', 1);
    summary += QString("  平均池大小: %1\n").arg(result.poolStats.avgPoolSize);
    summary += QString("  峰值池大小: %1\n\n").arg(result.poolStats.peakPoolSize);

    // 验收标准检查
    summary += "【验收标准检查】\n";
    summary += QString("  内存增长达标: %1\n").arg(result.passMemoryGrowth ? "✅ 通过" : "❌ 失败");
    summary += QString("  CPU使用率达标: %1\n").arg(result.passCpuUsage ? "✅ 通过" : "❌ 失败");
    summary += QString("  池命中率达标: %1\n").arg(result.passPoolHitRate ? "✅ 通过" : "❌ 失败");
    summary += QString("  无崩溃: %1\n\n").arg(result.passNoCrash ? "✅ 通过" : "❌ 失败");

    // 最终结果
    summary += QString("【最终结果】: %1\n\n")
               .arg(result.success ? "✅ 测试通过" : "❌ 测试失败");

    summary += "========================================\n";

    return summary;
}

void StabilityTestFramework::onScenarioFinished()
{
    LOG_INFO("场景完成");

    // 检查是否所有场景都完成
    bool allFinished = true;
    for (const TestScenario* scenario : scenarios_) {
        if (scenario->state() != TestScenario::State::Finished) {
            allFinished = false;
            break;
        }
    }

    if (allFinished && isRunning_) {
        stopTest();
    }
}

void StabilityTestFramework::onScenarioError(const QString& error)
{
    errors_.append(error);
    LOG_ERROR(QString("场景错误: %1").arg(error));
}

void StabilityTestFramework::onResourceSnapshot(const Base::ResourceSnapshot& snapshot)
{
    resourceSnapshots_.append(snapshot);
}

void StabilityTestFramework::onPoolSnapshot(const Base::PoolSnapshot& snapshot)
{
    poolSnapshots_.append(snapshot);
}

void StabilityTestFramework::startMonitoring()
{
    // 重置监控器
    resourceMonitor_->reset();
    poolMonitor_->reset();

    // 启动监控（每秒采样一次）
    resourceMonitor_->start(1000);
    poolMonitor_->start(1000);

    LOG_INFO("启动资源监控");
}

void StabilityTestFramework::stopMonitoring()
{
    resourceMonitor_->stop();
    poolMonitor_->stop();

    LOG_INFO("停止资源监控");
}

void StabilityTestFramework::checkAcceptanceCriteria()
{
    // 获取统计数据
    Base::ResourceStats resourceStats = resourceMonitor_->getStatistics();
    Base::PoolPerformanceStats poolStats = poolMonitor_->getStatistics();

    // 检查内存增长（转换为72小时的增长量）
    qint64 durationHours = getElapsedSeconds() / 3600;
    if (durationHours == 0) durationHours = 1;  // 避免除零

    double memoryGrowth72h = (resourceStats.memoryGrowthRateKBh * 72) / 1024.0;  // MB

    if (memoryGrowth72h > memoryGrowthThreshold_) {
        QString reason = QString("内存增长 %1 MB/72h 超过阈值 %2 MB/72h")
                         .arg(memoryGrowth72h, 0, 'f', 2)
                         .arg(memoryGrowthThreshold_);
        emit acceptanceCriteriaFailed("内存增长", reason);
        LOG_WARNING(reason);
    }

    // 检查CPU使用率
    if (resourceStats.cpuAvgPercent > cpuUsageThreshold_) {
        QString reason = QString("平均CPU使用率 %1% 超过阈值 %2%")
                         .arg(resourceStats.cpuAvgPercent, 0, 'f', 1)
                         .arg(cpuUsageThreshold_);
        emit acceptanceCriteriaFailed("CPU使用率", reason);
        LOG_WARNING(reason);
    }

    // 检查池命中率
    if (poolStats.avgHitRate < poolHitRateThreshold_) {
        QString reason = QString("平均池命中率 %1% 低于阈值 %2%")
                         .arg(poolStats.avgHitRate, 0, 'f', 1)
                         .arg(poolHitRateThreshold_);
        emit acceptanceCriteriaFailed("池命中率", reason);
        LOG_WARNING(reason);
    }
}

TestResult StabilityTestFramework::buildResult() const
{
    TestResult result;

    // 基本信息
    result.scenarioName = scenarios_.isEmpty() ? "" : scenarios_.first()->name();
    result.durationSeconds = getElapsedSeconds();
    result.totalOperations = totalOperations_;
    result.failedOperations = failedOperations_;

    // 资源统计
    result.resourceStats = resourceMonitor_->getStatistics();

    // 池性能统计
    result.poolStats = poolMonitor_->getStatistics();

    // 错误列表
    result.errors = errors_;

    // 验收标准检查
    qint64 durationHours = result.durationSeconds / 3600;
    if (durationHours == 0) durationHours = 1;

    double memoryGrowth72h = (result.resourceStats.memoryGrowthRateKBh * 72) / 1024.0;  // MB

    result.passMemoryGrowth = (memoryGrowth72h <= memoryGrowthThreshold_);
    result.passCpuUsage = (result.resourceStats.cpuAvgPercent <= cpuUsageThreshold_);
    result.passPoolHitRate = (result.poolStats.avgHitRate >= poolHitRateThreshold_);
    result.passNoCrash = true;  // 如果程序还在运行，说明没有崩溃

    // 最终结果
    result.success = result.passMemoryGrowth &&
                     result.passCpuUsage &&
                     result.passPoolHitRate &&
                     result.passNoCrash &&
                     (result.failedOperations == 0);

    return result;
}

QString testScenarioStateToString(TestScenario::State state)
{
    switch (state) {
        case TestScenario::State::Idle:
            return "空闲";
        case TestScenario::State::Running:
            return "运行中";
        case TestScenario::State::Paused:
            return "已暂停";
        case TestScenario::State::Stopped:
            return "已停止";
        case TestScenario::State::Finished:
            return "已完成";
        default:
            return "未知";
    }
}

} // namespace Test
} // namespace VisionForge
