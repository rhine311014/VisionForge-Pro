/**
 * @file test_ConfigManager.cpp
 * @brief ConfigManager类单元测试
 */

#include <QtTest/QtTest>
#include "base/ConfigManager.h"
#include "TestUtils.h"

using namespace VisionForge;
using namespace VisionForge::Test;

class TestConfigManager : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();

    // 基本读写测试
    void testSetGetValue();
    void testDefaultValue();

    // 数据类型测试
    void testIntValue();
    void testDoubleValue();
    void testBoolValue();
    void testStringValue();
    void testListValue();

    // 组管理测试
    void testGroups();
    void testContains();
    void testRemove();

private:
    ScopedTempDir tempDir_;
};

void TestConfigManager::initTestCase()
{
    // 配置管理器使用默认配置文件位置
}

void TestConfigManager::cleanupTestCase()
{
}

void TestConfigManager::init()
{
    // 每个测试前清理测试键
    auto& config = Base::ConfigManager::instance();
    config.remove("test");
}

void TestConfigManager::testSetGetValue()
{
    auto& config = Base::ConfigManager::instance();

    config.setValue("test/key1", "value1");
    QString value = config.getValue("test/key1").toString();

    QCOMPARE(value, QString("value1"));
}

void TestConfigManager::testDefaultValue()
{
    auto& config = Base::ConfigManager::instance();

    // 获取不存在的键，应返回默认值
    QString value = config.getValue("nonexistent/key", "default").toString();
    QCOMPARE(value, QString("default"));

    int intValue = config.getValue("nonexistent/intkey", 42).toInt();
    QCOMPARE(intValue, 42);
}

void TestConfigManager::testIntValue()
{
    auto& config = Base::ConfigManager::instance();

    config.setValue("test/intValue", 12345);
    int value = config.getValue("test/intValue").toInt();

    QCOMPARE(value, 12345);
}

void TestConfigManager::testDoubleValue()
{
    auto& config = Base::ConfigManager::instance();

    config.setValue("test/doubleValue", 3.14159);
    double value = config.getValue("test/doubleValue").toDouble();

    QCOMPARE(value, 3.14159);
}

void TestConfigManager::testBoolValue()
{
    auto& config = Base::ConfigManager::instance();

    config.setValue("test/boolTrue", true);
    config.setValue("test/boolFalse", false);

    QCOMPARE(config.getValue("test/boolTrue").toBool(), true);
    QCOMPARE(config.getValue("test/boolFalse").toBool(), false);
}

void TestConfigManager::testStringValue()
{
    auto& config = Base::ConfigManager::instance();

    QString testStr = "Hello, VisionForge! 你好，视觉锻造！";
    config.setValue("test/string", testStr);

    QCOMPARE(config.getValue("test/string").toString(), testStr);
}

void TestConfigManager::testListValue()
{
    auto& config = Base::ConfigManager::instance();

    QStringList list;
    list << "item1" << "item2" << "item3";
    config.setValue("test/list", list);

    QStringList result = config.getValue("test/list").toStringList();
    QCOMPARE(result.size(), 3);
    QCOMPARE(result[0], QString("item1"));
    QCOMPARE(result[1], QString("item2"));
    QCOMPARE(result[2], QString("item3"));
}

void TestConfigManager::testGroups()
{
    auto& config = Base::ConfigManager::instance();

    config.setValue("test/group1/key1", "value1");
    config.setValue("test/group1/key2", "value2");
    config.setValue("test/group2/key1", "value3");

    QCOMPARE(config.getValue("test/group1/key1").toString(), QString("value1"));
    QCOMPARE(config.getValue("test/group1/key2").toString(), QString("value2"));
    QCOMPARE(config.getValue("test/group2/key1").toString(), QString("value3"));
}

void TestConfigManager::testContains()
{
    auto& config = Base::ConfigManager::instance();

    config.setValue("test/existingKey", "value");

    QVERIFY(config.contains("test/existingKey"));
    QVERIFY(!config.contains("test/nonExistingKey"));
}

void TestConfigManager::testRemove()
{
    auto& config = Base::ConfigManager::instance();

    config.setValue("test/toRemove", "value");
    QVERIFY(config.contains("test/toRemove"));

    config.remove("test/toRemove");
    QVERIFY(!config.contains("test/toRemove"));
}

QTEST_MAIN(TestConfigManager)
#include "test_ConfigManager.moc"
