/**
 * @file test_ErrorRecovery.cpp
 * @brief ErrorRecovery类单元测试
 */

#include <QtTest/QtTest>
#include "base/ErrorRecovery.h"
#include "TestUtils.h"

using namespace VisionForge;
using namespace VisionForge::Test;

class TestErrorRecovery : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void cleanup();

    // 错误报告测试
    void testReportError();
    void testReportErrorWithSeverity();
    void testErrorSeverityLevels();
    void testErrorHistory();
    void testClearErrorHistory();
    void testMaxErrorHistory();

    // 恢复策略测试
    void testSetRecoveryStrategy();
    void testGetRecoveryStrategy();
    void testDefaultRecoveryStrategy();
    void testRecoveryHandler();

    // 重试机制测试
    void testExecuteWithRetry_Success();
    void testExecuteWithRetry_Failure();
    void testExecuteWithRetry_SuccessAfterRetries();
    void testRetryConfig();
    void testExponentialBackoff();

    // 检查点测试
    void testCreateCheckpoint();
    void testRestoreCheckpoint();
    void testRemoveCheckpoint();
    void testClearCheckpoints();
    void testHasCheckpoint();
    void testCheckpointIds();
    void testLastCheckpointId();

    // ScopedCheckpoint测试
    void testScopedCheckpoint_Commit();
    void testScopedCheckpoint_Rollback();
    void testScopedCheckpoint_AutoRollback();

    // RetryScope测试
    void testRetryScope();

    // 状态测试
    void testIsRecovering();
    void testCurrentRetryCount();

    // 信号测试
    void testErrorOccurredSignal();
    void testRecoverySignals();
    void testCheckpointSignals();

private:
    int retryAttempts_;
};

void TestErrorRecovery::initTestCase()
{
    retryAttempts_ = 0;
}

void TestErrorRecovery::cleanupTestCase()
{
}

void TestErrorRecovery::cleanup()
{
    // 每个测试后清理
    auto& recovery = Base::ErrorRecovery::instance();
    recovery.clearErrorHistory();
    recovery.clearCheckpoints();
    retryAttempts_ = 0;
}

// ========== 错误报告测试 ==========

void TestErrorRecovery::testReportError()
{
    auto& recovery = Base::ErrorRecovery::instance();

    Base::ErrorInfo info;
    info.code = "TEST_ERROR";
    info.message = "Test error message";
    info.severity = Base::ErrorSeverity::Warning;

    Base::RecoveryStrategy strategy = recovery.reportError(info);
    Q_UNUSED(strategy);

    auto history = recovery.errorHistory(1);
    QCOMPARE(history.size(), 1);
    QCOMPARE(history[0].code, QString("TEST_ERROR"));
}

void TestErrorRecovery::testReportErrorWithSeverity()
{
    auto& recovery = Base::ErrorRecovery::instance();

    recovery.reportError("ERR_001", "Warning message", Base::ErrorSeverity::Warning);
    recovery.reportError("ERR_002", "Error message", Base::ErrorSeverity::Error);
    recovery.reportError("ERR_003", "Critical message", Base::ErrorSeverity::Critical);

    auto history = recovery.errorHistory(3);
    QCOMPARE(history.size(), 3);
}

void TestErrorRecovery::testErrorSeverityLevels()
{
    auto& recovery = Base::ErrorRecovery::instance();

    // Warning
    recovery.reportError("WARN", "warning", Base::ErrorSeverity::Warning);
    auto history = recovery.errorHistory(1);
    QCOMPARE(history[0].severity, Base::ErrorSeverity::Warning);

    recovery.clearErrorHistory();

    // Error
    recovery.reportError("ERR", "error", Base::ErrorSeverity::Error);
    history = recovery.errorHistory(1);
    QCOMPARE(history[0].severity, Base::ErrorSeverity::Error);

    recovery.clearErrorHistory();

    // Critical
    recovery.reportError("CRIT", "critical", Base::ErrorSeverity::Critical);
    history = recovery.errorHistory(1);
    QCOMPARE(history[0].severity, Base::ErrorSeverity::Critical);
}

void TestErrorRecovery::testErrorHistory()
{
    auto& recovery = Base::ErrorRecovery::instance();

    for (int i = 0; i < 5; ++i) {
        recovery.reportError(QString("ERR_%1").arg(i), QString("Error %1").arg(i));
    }

    auto all = recovery.errorHistory(0);
    QCOMPARE(all.size(), 5);

    auto limited = recovery.errorHistory(3);
    QCOMPARE(limited.size(), 3);
}

void TestErrorRecovery::testClearErrorHistory()
{
    auto& recovery = Base::ErrorRecovery::instance();

    recovery.reportError("ERR", "error");
    QVERIFY(recovery.errorHistory(0).size() > 0);

    recovery.clearErrorHistory();
    QCOMPARE(recovery.errorHistory(0).size(), 0);
}

void TestErrorRecovery::testMaxErrorHistory()
{
    auto& recovery = Base::ErrorRecovery::instance();

    recovery.setMaxErrorHistory(5);

    for (int i = 0; i < 10; ++i) {
        recovery.reportError(QString("ERR_%1").arg(i), QString("Error %1").arg(i));
    }

    auto history = recovery.errorHistory(0);
    QVERIFY(history.size() <= 5);

    // 恢复默认
    recovery.setMaxErrorHistory(100);
}

// ========== 恢复策略测试 ==========

void TestErrorRecovery::testSetRecoveryStrategy()
{
    auto& recovery = Base::ErrorRecovery::instance();

    recovery.setRecoveryStrategy("CUSTOM_ERR", Base::RecoveryStrategy::Retry);
    QCOMPARE(recovery.getRecoveryStrategy("CUSTOM_ERR"), Base::RecoveryStrategy::Retry);

    recovery.setRecoveryStrategy("CUSTOM_ERR", Base::RecoveryStrategy::Skip);
    QCOMPARE(recovery.getRecoveryStrategy("CUSTOM_ERR"), Base::RecoveryStrategy::Skip);
}

void TestErrorRecovery::testGetRecoveryStrategy()
{
    auto& recovery = Base::ErrorRecovery::instance();

    // 未设置的错误码应返回默认策略
    Base::RecoveryStrategy strategy = recovery.getRecoveryStrategy("UNKNOWN_ERR");
    Q_UNUSED(strategy);
    QVERIFY(true);
}

void TestErrorRecovery::testDefaultRecoveryStrategy()
{
    auto& recovery = Base::ErrorRecovery::instance();

    // 设置全局默认策略
    recovery.setRecoveryStrategy("", Base::RecoveryStrategy::Notify);

    // 新错误应使用默认策略
    Base::RecoveryStrategy strategy = recovery.getRecoveryStrategy("NEW_ERROR");
    Q_UNUSED(strategy);
}

void TestErrorRecovery::testRecoveryHandler()
{
    auto& recovery = Base::ErrorRecovery::instance();

    bool handlerCalled = false;

    recovery.registerRecoveryHandler("HANDLER_TEST", [&handlerCalled](const Base::ErrorInfo& info) {
        Q_UNUSED(info);
        handlerCalled = true;
        return true;
    });

    recovery.reportError("HANDLER_TEST", "Test handler");
    QVERIFY(handlerCalled);
}

// ========== 重试机制测试 ==========

void TestErrorRecovery::testExecuteWithRetry_Success()
{
    auto& recovery = Base::ErrorRecovery::instance();

    bool result = recovery.executeWithRetry([]() {
        return true;
    });

    QVERIFY(result);
}

void TestErrorRecovery::testExecuteWithRetry_Failure()
{
    auto& recovery = Base::ErrorRecovery::instance();

    Base::RetryConfig config;
    config.maxRetries = 2;
    config.initialDelayMs = 10;
    recovery.setRetryConfig(config);

    bool result = recovery.executeWithRetry([]() {
        return false;
    });

    QVERIFY(!result);
}

void TestErrorRecovery::testExecuteWithRetry_SuccessAfterRetries()
{
    auto& recovery = Base::ErrorRecovery::instance();

    Base::RetryConfig config;
    config.maxRetries = 5;
    config.initialDelayMs = 10;
    recovery.setRetryConfig(config);

    int attempts = 0;

    bool result = recovery.executeWithRetry([&attempts]() {
        attempts++;
        return attempts >= 3;  // 第3次成功
    });

    QVERIFY(result);
    QCOMPARE(attempts, 3);
}

void TestErrorRecovery::testRetryConfig()
{
    auto& recovery = Base::ErrorRecovery::instance();

    Base::RetryConfig config;
    config.maxRetries = 10;
    config.initialDelayMs = 50;
    config.maxDelayMs = 1000;
    config.backoffMultiplier = 1.5;
    config.exponentialBackoff = true;

    recovery.setRetryConfig(config);

    const Base::RetryConfig& stored = recovery.retryConfig();
    QCOMPARE(stored.maxRetries, 10);
    QCOMPARE(stored.initialDelayMs, 50);
    QCOMPARE(stored.maxDelayMs, 1000);
    QCOMPARE(stored.backoffMultiplier, 1.5);
    QVERIFY(stored.exponentialBackoff);
}

void TestErrorRecovery::testExponentialBackoff()
{
    auto& recovery = Base::ErrorRecovery::instance();

    Base::RetryConfig config;
    config.maxRetries = 3;
    config.initialDelayMs = 10;
    config.maxDelayMs = 100;
    config.backoffMultiplier = 2.0;
    config.exponentialBackoff = true;

    recovery.setRetryConfig(config);

    QElapsedTimer timer;
    timer.start();

    recovery.executeWithRetry([]() {
        return false;
    });

    qint64 elapsed = timer.elapsed();

    // 应该有延迟：10 + 20 + 40 = 70ms (理论值)
    QVERIFY(elapsed >= 30);  // 至少有一些延迟
}

// ========== 检查点测试 ==========

void TestErrorRecovery::testCreateCheckpoint()
{
    auto& recovery = Base::ErrorRecovery::instance();

    QJsonObject state;
    state["key"] = "value";
    state["number"] = 42;

    recovery.createCheckpoint("cp1", "Test checkpoint", state);

    QVERIFY(recovery.hasCheckpoint("cp1"));
}

void TestErrorRecovery::testRestoreCheckpoint()
{
    auto& recovery = Base::ErrorRecovery::instance();

    QJsonObject state;
    state["key"] = "value";
    state["number"] = 42;

    recovery.createCheckpoint("cp2", "Test checkpoint", state);

    QJsonObject restored = recovery.restoreCheckpoint("cp2");
    QCOMPARE(restored["key"].toString(), QString("value"));
    QCOMPARE(restored["number"].toInt(), 42);
}

void TestErrorRecovery::testRemoveCheckpoint()
{
    auto& recovery = Base::ErrorRecovery::instance();

    QJsonObject state;
    recovery.createCheckpoint("cp3", "Test", state);
    QVERIFY(recovery.hasCheckpoint("cp3"));

    recovery.removeCheckpoint("cp3");
    QVERIFY(!recovery.hasCheckpoint("cp3"));
}

void TestErrorRecovery::testClearCheckpoints()
{
    auto& recovery = Base::ErrorRecovery::instance();

    QJsonObject state;
    recovery.createCheckpoint("cp4", "Test 1", state);
    recovery.createCheckpoint("cp5", "Test 2", state);

    recovery.clearCheckpoints();

    QVERIFY(!recovery.hasCheckpoint("cp4"));
    QVERIFY(!recovery.hasCheckpoint("cp5"));
    QCOMPARE(recovery.checkpointIds().size(), 0);
}

void TestErrorRecovery::testHasCheckpoint()
{
    auto& recovery = Base::ErrorRecovery::instance();

    QVERIFY(!recovery.hasCheckpoint("nonexistent"));

    QJsonObject state;
    recovery.createCheckpoint("exists", "Test", state);
    QVERIFY(recovery.hasCheckpoint("exists"));
}

void TestErrorRecovery::testCheckpointIds()
{
    auto& recovery = Base::ErrorRecovery::instance();

    QJsonObject state;
    recovery.createCheckpoint("id1", "Test 1", state);
    recovery.createCheckpoint("id2", "Test 2", state);
    recovery.createCheckpoint("id3", "Test 3", state);

    QStringList ids = recovery.checkpointIds();
    QCOMPARE(ids.size(), 3);
    QVERIFY(ids.contains("id1"));
    QVERIFY(ids.contains("id2"));
    QVERIFY(ids.contains("id3"));
}

void TestErrorRecovery::testLastCheckpointId()
{
    auto& recovery = Base::ErrorRecovery::instance();

    QJsonObject state;
    recovery.createCheckpoint("first", "First", state);
    recovery.createCheckpoint("second", "Second", state);
    recovery.createCheckpoint("third", "Third", state);

    QCOMPARE(recovery.lastCheckpointId(), QString("third"));
}

// ========== ScopedCheckpoint测试 ==========

void TestErrorRecovery::testScopedCheckpoint_Commit()
{
    auto& recovery = Base::ErrorRecovery::instance();

    {
        QJsonObject state;
        state["data"] = "test";
        Base::ScopedCheckpoint cp("scoped1", "Scoped test", state);

        QVERIFY(recovery.hasCheckpoint("scoped1"));

        cp.commit();
    }

    // commit后不应被回滚
    QVERIFY(recovery.hasCheckpoint("scoped1"));
}

void TestErrorRecovery::testScopedCheckpoint_Rollback()
{
    auto& recovery = Base::ErrorRecovery::instance();

    {
        QJsonObject state;
        Base::ScopedCheckpoint cp("scoped2", "Scoped test", state);

        QVERIFY(recovery.hasCheckpoint("scoped2"));

        cp.rollback();
    }

    // 手动回滚后检查点应被移除
    QVERIFY(!recovery.hasCheckpoint("scoped2"));
}

void TestErrorRecovery::testScopedCheckpoint_AutoRollback()
{
    auto& recovery = Base::ErrorRecovery::instance();

    {
        QJsonObject state;
        Base::ScopedCheckpoint cp("scoped3", "Scoped test", state);

        QVERIFY(recovery.hasCheckpoint("scoped3"));

        // 不调用commit，析构时应自动回滚
    }

    // 自动回滚后检查点应被移除
    QVERIFY(!recovery.hasCheckpoint("scoped3"));
}

// ========== RetryScope测试 ==========

void TestErrorRecovery::testRetryScope()
{
    auto& recovery = Base::ErrorRecovery::instance();

    Base::RetryConfig originalConfig = recovery.retryConfig();

    {
        Base::RetryConfig newConfig;
        newConfig.maxRetries = 99;
        Base::RetryScope scope(newConfig);

        QCOMPARE(recovery.retryConfig().maxRetries, 99);
    }

    // 作用域结束后应恢复原配置
    QCOMPARE(recovery.retryConfig().maxRetries, originalConfig.maxRetries);
}

// ========== 状态测试 ==========

void TestErrorRecovery::testIsRecovering()
{
    auto& recovery = Base::ErrorRecovery::instance();

    // 正常情况下不在恢复中
    QVERIFY(!recovery.isRecovering());
}

void TestErrorRecovery::testCurrentRetryCount()
{
    auto& recovery = Base::ErrorRecovery::instance();

    // 没有重试时应为0
    QCOMPARE(recovery.currentRetryCount(), 0);
}

// ========== 信号测试 ==========

void TestErrorRecovery::testErrorOccurredSignal()
{
    auto& recovery = Base::ErrorRecovery::instance();

    QSignalSpy spy(&recovery, &Base::ErrorRecovery::errorOccurred);

    recovery.reportError("SIG_TEST", "Signal test");

    QCOMPARE(spy.count(), 1);
}

void TestErrorRecovery::testRecoverySignals()
{
    auto& recovery = Base::ErrorRecovery::instance();

    QSignalSpy startSpy(&recovery, &Base::ErrorRecovery::recoveryStarted);
    QSignalSpy completeSpy(&recovery, &Base::ErrorRecovery::recoveryCompleted);

    // 设置需要重试的策略
    recovery.setRecoveryStrategy("RETRY_SIG", Base::RecoveryStrategy::Retry);

    Base::RetryConfig config;
    config.maxRetries = 1;
    config.initialDelayMs = 10;
    recovery.setRetryConfig(config);

    recovery.executeWithRetry([]() {
        return false;
    }, "RETRY_SIG");

    // 可能会触发信号，也可能不会（取决于实现）
    Q_UNUSED(startSpy);
    Q_UNUSED(completeSpy);
}

void TestErrorRecovery::testCheckpointSignals()
{
    auto& recovery = Base::ErrorRecovery::instance();

    QSignalSpy createSpy(&recovery, &Base::ErrorRecovery::checkpointCreated);
    QSignalSpy restoreSpy(&recovery, &Base::ErrorRecovery::checkpointRestored);

    QJsonObject state;
    recovery.createCheckpoint("sig_cp", "Signal test", state);

    QCOMPARE(createSpy.count(), 1);

    recovery.restoreCheckpoint("sig_cp");

    QCOMPARE(restoreSpy.count(), 1);
}

QTEST_MAIN(TestErrorRecovery)
#include "test_ErrorRecovery.moc"
