/**
 * @file test_ImageMemoryPool.cpp
 * @brief ImageMemoryPool类单元测试
 */

#include <QtTest/QtTest>
#include "base/ImageMemoryPool.h"
#include "TestUtils.h"

using namespace VisionForge;
using namespace VisionForge::Test;

class TestImageMemoryPool : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    // 基本功能测试
    void testAllocate();
    void testRelease();
    void testPreallocate();

    // 池状态测试
    void testSize();
    void testMemoryUsage();

    // 缓存命中测试
    void testCacheHit();
    void testCacheMiss();

    // 配置测试
    void testMaxPoolSize();
    void testMemoryLimit();

    // 清理测试
    void testClear();
    void testCleanupStrategy();

    // 统计信息测试
    void testStatistics();
};

void TestImageMemoryPool::initTestCase()
{
    // 初始化测试环境
}

void TestImageMemoryPool::cleanupTestCase()
{
    // 清理测试环境
}

void TestImageMemoryPool::init()
{
    // 每个测试前清空池
    Base::ImageMemoryPool::instance().clear();
    Base::ImageMemoryPool::instance().resetStatistics();
}

void TestImageMemoryPool::cleanup()
{
    // 每个测试后清空池
    Base::ImageMemoryPool::instance().clear();
}

void TestImageMemoryPool::testAllocate()
{
    auto& pool = Base::ImageMemoryPool::instance();

    auto image = pool.allocate(640, 480, CV_8UC1);
    QVERIFY(image != nullptr);
    QCOMPARE(image->width(), 640);
    QCOMPARE(image->height(), 480);
}

void TestImageMemoryPool::testRelease()
{
    auto& pool = Base::ImageMemoryPool::instance();

    // 分配并释放
    auto image = pool.allocate(640, 480, CV_8UC1);
    pool.release(image);

    // 池中应该有一个对象
    QCOMPARE(pool.size(640, 480, CV_8UC1), static_cast<size_t>(1));
}

void TestImageMemoryPool::testPreallocate()
{
    auto& pool = Base::ImageMemoryPool::instance();

    pool.preallocate(320, 240, CV_8UC3, 5);
    QCOMPARE(pool.size(320, 240, CV_8UC3), static_cast<size_t>(5));
}

void TestImageMemoryPool::testSize()
{
    auto& pool = Base::ImageMemoryPool::instance();

    QCOMPARE(pool.size(), static_cast<size_t>(0));

    pool.preallocate(100, 100, CV_8UC1, 3);
    pool.preallocate(200, 200, CV_8UC3, 2);

    QCOMPARE(pool.size(), static_cast<size_t>(5));
    QCOMPARE(pool.size(100, 100, CV_8UC1), static_cast<size_t>(3));
    QCOMPARE(pool.size(200, 200, CV_8UC3), static_cast<size_t>(2));
}

void TestImageMemoryPool::testMemoryUsage()
{
    auto& pool = Base::ImageMemoryPool::instance();

    pool.preallocate(100, 100, CV_8UC1, 2);  // 2 * 100 * 100 * 1 = 20000 bytes

    size_t expected = 2 * 100 * 100 * 1;
    QCOMPARE(pool.getTotalMemoryUsage(), expected);
}

void TestImageMemoryPool::testCacheHit()
{
    auto& pool = Base::ImageMemoryPool::instance();

    // 预分配
    pool.preallocate(640, 480, CV_8UC1, 1);

    // 分配应该命中缓存
    auto image = pool.allocate(640, 480, CV_8UC1);

    auto stats = pool.getStatistics();
    QVERIFY(stats.cacheHits > 0);
}

void TestImageMemoryPool::testCacheMiss()
{
    auto& pool = Base::ImageMemoryPool::instance();
    pool.resetStatistics();

    // 没有预分配，应该未命中
    auto image = pool.allocate(640, 480, CV_8UC1);

    auto stats = pool.getStatistics();
    QVERIFY(stats.cacheMisses > 0);
}

void TestImageMemoryPool::testMaxPoolSize()
{
    auto& pool = Base::ImageMemoryPool::instance();

    pool.setMaxPoolSize(3);
    QCOMPARE(pool.getMaxPoolSize(), static_cast<size_t>(3));

    // 预分配超过限制
    pool.preallocate(100, 100, CV_8UC1, 5);
    // 实际数量应该是5（预分配不受限制）
    QCOMPARE(pool.size(100, 100, CV_8UC1), static_cast<size_t>(5));
}

void TestImageMemoryPool::testMemoryLimit()
{
    auto& pool = Base::ImageMemoryPool::instance();

    pool.setMemoryLimit(1024 * 1024);  // 1MB
    QCOMPARE(pool.getMemoryLimit(), static_cast<size_t>(1024 * 1024));
}

void TestImageMemoryPool::testClear()
{
    auto& pool = Base::ImageMemoryPool::instance();

    pool.preallocate(100, 100, CV_8UC1, 5);
    QVERIFY(pool.size() > 0);

    pool.clear();
    QCOMPARE(pool.size(), static_cast<size_t>(0));
}

void TestImageMemoryPool::testCleanupStrategy()
{
    auto& pool = Base::ImageMemoryPool::instance();

    pool.setCleanupStrategy(Base::PoolCleanupStrategy::LRU);
    QCOMPARE(pool.getCleanupStrategy(), Base::PoolCleanupStrategy::LRU);

    pool.setCleanupStrategy(Base::PoolCleanupStrategy::LFU);
    QCOMPARE(pool.getCleanupStrategy(), Base::PoolCleanupStrategy::LFU);

    pool.setCleanupStrategy(Base::PoolCleanupStrategy::FIFO);
    QCOMPARE(pool.getCleanupStrategy(), Base::PoolCleanupStrategy::FIFO);

    pool.setCleanupStrategy(Base::PoolCleanupStrategy::Size);
    QCOMPARE(pool.getCleanupStrategy(), Base::PoolCleanupStrategy::Size);
}

void TestImageMemoryPool::testStatistics()
{
    auto& pool = Base::ImageMemoryPool::instance();
    pool.resetStatistics();

    // 执行一些操作
    auto img1 = pool.allocate(100, 100, CV_8UC1);
    pool.release(img1);
    auto img2 = pool.allocate(100, 100, CV_8UC1);

    auto stats = pool.getStatistics();

    QVERIFY(stats.totalAllocations >= 2);
    QVERIFY(stats.cacheHits >= 1);
    QVERIFY(stats.cacheMisses >= 1);

    // 命中率应该在0-1之间
    QVERIFY(stats.hitRate >= 0.0 && stats.hitRate <= 1.0);

    // 测试JSON导出
    auto json = stats.toJson();
    QVERIFY(json.contains("totalAllocations"));
    QVERIFY(json.contains("cacheHits"));
    QVERIFY(json.contains("hitRate"));
}

QTEST_MAIN(TestImageMemoryPool)
#include "test_ImageMemoryPool.moc"
