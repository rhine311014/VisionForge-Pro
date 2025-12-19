/**
 * @file test_Logger.cpp
 * @brief Logger类单元测试
 */

#include <QtTest/QtTest>
#include "base/Logger.h"
#include "TestUtils.h"

using namespace VisionForge;
using namespace VisionForge::Test;

class TestLogger : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    // 基本日志测试
    void testDebugLog();
    void testInfoLog();
    void testWarningLog();
    void testErrorLog();

    // 日志级别测试
    void testLogLevel();

    // 日志历史测试
    void testLogHistory();
    void testMaxHistorySize();

    // 日志导出测试
    void testExportPlainText();
    void testExportCSV();
    void testExportJSON();

    // 性能测试
    void testLogPerformance();

private:
    ScopedTempDir tempDir_;
};

void TestLogger::initTestCase()
{
    // 确保日志系统已初始化
    Base::Logger::instance();
}

void TestLogger::cleanupTestCase()
{
}

void TestLogger::testDebugLog()
{
    // 调试级别的日志不应该抛出异常
    LOG_DEBUG("Test debug message");
    LOG_DEBUG(QString("Test debug with number: %1").arg(42));
}

void TestLogger::testInfoLog()
{
    LOG_INFO("Test info message");
    LOG_INFO(QString("Test info: %1").arg("important"));
}

void TestLogger::testWarningLog()
{
    LOG_WARNING("Test warning message");
    LOG_WARNING(QString("Warning: value %1 out of range").arg(999));
}

void TestLogger::testErrorLog()
{
    LOG_ERROR("Test error message");
    LOG_ERROR(QString("Error code: %1").arg(-1));
}

void TestLogger::testLogLevel()
{
    auto& logger = Base::Logger::instance();

    // 测试设置不同日志级别
    logger.setLogLevel(Base::Logger::Debug);
    QCOMPARE(logger.logLevel(), Base::Logger::Debug);

    logger.setLogLevel(Base::Logger::Info);
    QCOMPARE(logger.logLevel(), Base::Logger::Info);

    logger.setLogLevel(Base::Logger::Warning);
    QCOMPARE(logger.logLevel(), Base::Logger::Warning);

    logger.setLogLevel(Base::Logger::Error);
    QCOMPARE(logger.logLevel(), Base::Logger::Error);

    // 恢复默认级别
    logger.setLogLevel(Base::Logger::Debug);
}

void TestLogger::testLogHistory()
{
    auto& logger = Base::Logger::instance();
    logger.clearHistory();

    // 记录一些日志
    LOG_INFO("History test 1");
    LOG_INFO("History test 2");
    LOG_INFO("History test 3");

    // 获取日志历史
    auto history = logger.getLogHistory();
    QVERIFY(history.size() >= 3);
}

void TestLogger::testMaxHistorySize()
{
    auto& logger = Base::Logger::instance();

    // 设置最大历史大小
    logger.setMaxHistorySize(10);
    logger.clearHistory();

    // 记录超过限制的日志
    for (int i = 0; i < 20; ++i) {
        LOG_INFO(QString("History overflow test %1").arg(i));
    }

    // 历史大小不应超过限制
    auto history = logger.getLogHistory();
    QVERIFY(history.size() <= 10);

    // 恢复默认限制
    logger.setMaxHistorySize(1000);
}

void TestLogger::testExportPlainText()
{
    auto& logger = Base::Logger::instance();
    logger.clearHistory();

    LOG_INFO("Export test message");

    QString exported = logger.exportLogsToString(Base::LogExportFormat::PlainText);
    QVERIFY(!exported.isEmpty());
    QVERIFY(exported.contains("Export test message"));
}

void TestLogger::testExportCSV()
{
    auto& logger = Base::Logger::instance();
    logger.clearHistory();

    LOG_INFO("CSV export test");

    QString exported = logger.exportLogsToString(Base::LogExportFormat::CSV);
    QVERIFY(!exported.isEmpty());
    // CSV应该包含分隔符
    QVERIFY(exported.contains(","));
}

void TestLogger::testExportJSON()
{
    auto& logger = Base::Logger::instance();
    logger.clearHistory();

    LOG_INFO("JSON export test");

    QString exported = logger.exportLogsToString(Base::LogExportFormat::JSON);
    QVERIFY(!exported.isEmpty());
    // JSON应该包含方括号
    QVERIFY(exported.contains("[") || exported.contains("{"));
}

void TestLogger::testLogPerformance()
{
    auto& logger = Base::Logger::instance();

    // 性能测试：记录1000条日志的时间
    QElapsedTimer timer;
    timer.start();

    for (int i = 0; i < 1000; ++i) {
        LOG_DEBUG(QString("Performance test log %1").arg(i));
    }

    qint64 elapsed = timer.elapsed();

    // 1000条日志应该在1秒内完成
    QVERIFY2(elapsed < 1000, qPrintable(QString("Logging 1000 messages took %1ms").arg(elapsed)));
}

QTEST_MAIN(TestLogger)
#include "test_Logger.moc"
