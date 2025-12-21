/**
 * @file test_ScopedImagePtr.cpp
 * @brief ScopedImagePtr单元测试 - 验证RAII内存管理
 * @author VisionForge Team
 * @date 2025-12-20
 */

#include "base/ScopedImagePtr.h"
#include "base/ImageMemoryPool.h"
#include <QtTest/QtTest>
#include <opencv2/core.hpp>

using namespace VisionForge::Base;

class TestScopedImagePtr : public QObject
{
    Q_OBJECT

private slots:
    // 初始化和清理
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    // 基础功能测试
    void test_construction();
    void test_moveSemantics();
    void test_reset();
    void test_release();

    // RAII测试
    void test_autoRelease();
    void test_exceptionSafety();

    // 内存池集成测试
    void test_poolIntegration();
    void test_poolHitRate();

    // 性能测试
    void test_performance();
};

void TestScopedImagePtr::initTestCase()
{
    // 配置内存池
    ImageMemoryPool::instance().setMaxPoolSize(20);
    ImageMemoryPool::instance().clear();
    ImageMemoryPool::instance().resetStatistics();
}

void TestScopedImagePtr::cleanupTestCase()
{
    ImageMemoryPool::instance().clear();
}

void TestScopedImagePtr::init()
{
    // 每个测试前清空池
    ImageMemoryPool::instance().clear();
    ImageMemoryPool::instance().resetStatistics();
}

void TestScopedImagePtr::cleanup()
{
}

/**
 * @brief 测试基本构造和访问
 */
void TestScopedImagePtr::test_construction()
{
    // 1. 默认构造
    {
        ScopedImagePtr img;
        QVERIFY(img.isNull());
        QVERIFY(!img);
    }

    // 2. 从池中分配
    {
        ScopedImagePtr img(1920, 1080, CV_8UC3);

        QVERIFY(!img.isNull());
        QVERIFY(bool(img));
        QVERIFY(img.get() != nullptr);

        // 验证图像参数
        QCOMPARE(img->mat().cols, 1920);
        QCOMPARE(img->mat().rows, 1080);
        QCOMPARE(img->mat().type(), CV_8UC3);
    }

    // 3. 成员访问
    {
        ScopedImagePtr img(640, 480, CV_8UC1);

        // operator->
        int width = img->width();
        QCOMPARE(width, 640);

        // operator*
        ImageData& data = *img;
        QCOMPARE(data.height(), 480);
    }
}

/**
 * @brief 测试移动语义
 */
void TestScopedImagePtr::test_moveSemantics()
{
    // 1. 移动构造
    {
        ScopedImagePtr img1(1920, 1080, CV_8UC3);
        ImageData::Ptr ptr1 = img1.get();

        ScopedImagePtr img2(std::move(img1));

        QVERIFY(img1.isNull());  // img1变为空
        QVERIFY(!img2.isNull());  // img2接管所有权
        QCOMPARE(img2.get(), ptr1);  // 指针不变
    }

    // 2. 移动赋值
    {
        ScopedImagePtr img1(1920, 1080, CV_8UC3);
        ScopedImagePtr img2(640, 480, CV_8UC1);

        ImageData::Ptr ptr1 = img1.get();

        img2 = std::move(img1);

        QVERIFY(img1.isNull());
        QVERIFY(!img2.isNull());
        QCOMPARE(img2.get(), ptr1);
    }

    // 3. 函数返回值（自动移动）
    {
        auto createImage = []() -> ScopedImagePtr {
            ScopedImagePtr img(1920, 1080, CV_8UC3);
            return img;  // NRVO或移动
        };

        ScopedImagePtr img = createImage();
        QVERIFY(!img.isNull());
        QCOMPARE(img->width(), 1920);
    }
}

/**
 * @brief 测试reset功能
 */
void TestScopedImagePtr::test_reset()
{
    // 1. reset() - 释放当前图像
    {
        ScopedImagePtr img(1920, 1080, CV_8UC3);
        QVERIFY(!img.isNull());

        img.reset();
        QVERIFY(img.isNull());
    }

    // 2. reset(width, height, type) - 重新分配
    {
        ScopedImagePtr img(1920, 1080, CV_8UC3);

        img.reset(640, 480, CV_8UC1);

        QVERIFY(!img.isNull());
        QCOMPARE(img->width(), 640);
        QCOMPARE(img->height(), 480);
    }

    // 3. reset(ImageData::Ptr) - 接管新指针
    {
        ScopedImagePtr img1(1920, 1080, CV_8UC3);
        auto ptr = ImageMemoryPool::instance().allocate(640, 480, CV_8UC1);

        img1.reset(ptr);

        QVERIFY(!img1.isNull());
        QCOMPARE(img1->width(), 640);
    }
}

/**
 * @brief 测试release功能
 */
void TestScopedImagePtr::test_release()
{
    ScopedImagePtr img(1920, 1080, CV_8UC3);
    ImageData::Ptr ptr = img.get();

    // release后,本对象变为空,但图像不归还池
    ImageData::Ptr released = img.release();

    QVERIFY(img.isNull());
    QCOMPARE(released, ptr);
    QVERIFY(released != nullptr);

    // 手动归还池
    ImageMemoryPool::instance().release(released);
}

/**
 * @brief 测试自动释放（RAII核心功能）
 */
void TestScopedImagePtr::test_autoRelease()
{
    auto& pool = ImageMemoryPool::instance();

    // 初始状态
    QCOMPARE(pool.size(), static_cast<size_t>(0));

    {
        // 分配3个图像
        ScopedImagePtr img1(1920, 1080, CV_8UC3);
        ScopedImagePtr img2(1920, 1080, CV_8UC3);
        ScopedImagePtr img3(640, 480, CV_8UC1);

        // 池中无闲置对象（全部在使用中）
        QCOMPARE(pool.size(1920, 1080, CV_8UC3), static_cast<size_t>(0));
    }
    // 离开作用域，自动归还池中 ✅

    // 验证图像已归还池中
    QCOMPARE(pool.size(1920, 1080, CV_8UC3), static_cast<size_t>(2));
    QCOMPARE(pool.size(640, 480, CV_8UC1), static_cast<size_t>(1));

    // 再次分配，命中缓存
    {
        ScopedImagePtr img4(1920, 1080, CV_8UC3);

        auto stats = pool.getStatistics();
        QVERIFY(stats.cacheHits > 0);  // 命中缓存 ✅
    }
}

/**
 * @brief 测试异常安全（异常情况下也自动释放）
 */
void TestScopedImagePtr::test_exceptionSafety()
{
    auto& pool = ImageMemoryPool::instance();

    try {
        ScopedImagePtr img(1920, 1080, CV_8UC3);

        // 模拟异常
        throw std::runtime_error("Test exception");

        // 永远不会执行到这里
        QFAIL("Should not reach here");
    }
    catch (const std::exception&) {
        // 捕获异常,但图像应该已自动归还 ✅
    }

    // 验证图像已归还池中
    QCOMPARE(pool.size(1920, 1080, CV_8UC3), static_cast<size_t>(1));
}

/**
 * @brief 测试与ImageMemoryPool集成
 */
void TestScopedImagePtr::test_poolIntegration()
{
    auto& pool = ImageMemoryPool::instance();

    // 预分配10个图像
    pool.preallocate(1920, 1080, CV_8UC3, 10);
    QCOMPARE(pool.size(1920, 1080, CV_8UC3), static_cast<size_t>(10));

    // 分配5个
    std::vector<ScopedImagePtr> images;
    for (int i = 0; i < 5; ++i) {
        images.emplace_back(1920, 1080, CV_8UC3);
    }

    // 池中还剩5个
    QCOMPARE(pool.size(1920, 1080, CV_8UC3), static_cast<size_t>(5));

    // 清空vector,图像自动归还
    images.clear();

    // 池中恢复10个
    QCOMPARE(pool.size(1920, 1080, CV_8UC3), static_cast<size_t>(10));
}

/**
 * @brief 测试池命中率
 */
void TestScopedImagePtr::test_poolHitRate()
{
    auto& pool = ImageMemoryPool::instance();

    // 第一次分配 - 未命中
    {
        ScopedImagePtr img(1920, 1080, CV_8UC3);
    }

    // 第二次分配 - 命中
    {
        ScopedImagePtr img(1920, 1080, CV_8UC3);
    }

    auto stats = pool.getStatistics();
    QCOMPARE(stats.totalAllocations, static_cast<size_t>(2));
    QCOMPARE(stats.cacheHits, static_cast<size_t>(1));
    QCOMPARE(stats.cacheMisses, static_cast<size_t>(1));
    QCOMPARE(stats.hitRate, 0.5);  // 50%命中率
}

/**
 * @brief 性能测试 - 验证RAII无显著性能损失
 */
void TestScopedImagePtr::test_performance()
{
    const int iterations = 1000;

    // 测试1: ScopedImagePtr分配释放
    QElapsedTimer timer;
    timer.start();

    for (int i = 0; i < iterations; ++i) {
        ScopedImagePtr img(1920, 1080, CV_8UC3);
        // 使用图像...
        img->mat().setTo(cv::Scalar(255, 0, 0));
    }

    qint64 scopedTime = timer.elapsed();

    // 测试2: 原始shared_ptr方式（对比）
    ImageMemoryPool::instance().clear();
    timer.restart();

    for (int i = 0; i < iterations; ++i) {
        auto img = ImageMemoryPool::instance().allocate(1920, 1080, CV_8UC3);
        img->mat().setTo(cv::Scalar(255, 0, 0));
        ImageMemoryPool::instance().release(img);
    }

    qint64 rawTime = timer.elapsed();

    qDebug() << "Performance comparison:";
    qDebug() << "  ScopedImagePtr:" << scopedTime << "ms";
    qDebug() << "  Raw shared_ptr:" << rawTime << "ms";
    qDebug() << "  Overhead:" << (scopedTime - rawTime) << "ms"
             << QString("(%1%)").arg(100.0 * (scopedTime - rawTime) / rawTime, 0, 'f', 1);

    // 性能损失应小于10%
    QVERIFY(scopedTime < rawTime * 1.1);
}

QTEST_MAIN(TestScopedImagePtr)
#include "test_ScopedImagePtr.moc"
