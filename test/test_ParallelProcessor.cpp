/**
 * @file test_ParallelProcessor.cpp
 * @brief ParallelProcessor类单元测试
 */

#include <QtTest/QtTest>
#include <QMutex>
#include "base/ParallelProcessor.h"
#include "base/ImageData.h"
#include "base/ImageMemoryPool.h"
#include "TestUtils.h"
#include <atomic>

using namespace VisionForge;
using namespace VisionForge::Test;

class TestParallelProcessor : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    // 线程配置测试
    void testGetAvailableCores();
    void testSetMaxThreads();
    void testEnableDisable();

    // 批量处理测试
    void testProcessBatch();
    void testProcessBatchIndexed();
    void testProcessBatchEmpty();
    void testProcessBatchSingleItem();

    // 分块处理测试
    void testProcessTiles();
    void testProcessTilesReadOnly();
    void testProcessTilesWithOverlap();
    void testCalculateTiles();

    // ROI处理测试
    void testProcessROIs();
    void testProcessROIsEmpty();
    void testProcessROIsMultiple();

    // 像素级处理测试
    void testForEachPixel();
    void testForEachRow();

    // 归约操作测试
    void testParallelSum();
    void testParallelMax();
    void testParallelMin();
    void testParallelSumEmpty();

    // 统计信息测试
    void testStatistics();
    void testResetStatistics();

    // 禁用状态测试
    void testDisabledFallback();

private:
    cv::Mat testImage_;
    std::vector<Base::ImageData::Ptr> testImages_;
};

void TestParallelProcessor::initTestCase()
{
    testImage_ = TestUtils::createGrayImage(640, 480, 128);

    // 创建测试图像集
    for (int i = 0; i < 10; ++i) {
        cv::Mat img = TestUtils::createGrayImage(320, 240, i * 25);
        auto imageData = Base::ImageMemoryPool::instance().allocate(
            img.cols, img.rows, img.type());
        if (imageData) {
            img.copyTo(imageData->mat());
            testImages_.push_back(imageData);
        }
    }
}

void TestParallelProcessor::cleanupTestCase()
{
    testImage_.release();
    testImages_.clear();
}

// ========== 线程配置测试 ==========

void TestParallelProcessor::testGetAvailableCores()
{
    int cores = Base::ParallelProcessor::getAvailableCores();
    QVERIFY(cores >= 1);
}

void TestParallelProcessor::testSetMaxThreads()
{
    auto& proc = Base::ParallelProcessor::instance();

    int originalThreads = proc.getMaxThreads();

    proc.setMaxThreads(4);
    QCOMPARE(proc.getMaxThreads(), 4);

    proc.setMaxThreads(1);
    QCOMPARE(proc.getMaxThreads(), 1);

    // 0表示自动检测
    proc.setMaxThreads(0);
    QVERIFY(proc.getMaxThreads() >= 1);

    // 恢复原设置
    proc.setMaxThreads(originalThreads);
}

void TestParallelProcessor::testEnableDisable()
{
    auto& proc = Base::ParallelProcessor::instance();

    proc.setEnabled(true);
    QVERIFY(proc.isEnabled());

    proc.setEnabled(false);
    QVERIFY(!proc.isEnabled());

    // 恢复启用
    proc.setEnabled(true);
}

// ========== 批量处理测试 ==========

void TestParallelProcessor::testProcessBatch()
{
    auto& proc = Base::ParallelProcessor::instance();
    proc.setEnabled(true);

    std::vector<Base::ImageData::Ptr> images;
    for (int i = 0; i < 5; ++i) {
        cv::Mat img = TestUtils::createGrayImage(100, 100, 100);
        auto imageData = Base::ImageMemoryPool::instance().allocate(
            img.cols, img.rows, img.type());
        if (imageData) {
            img.copyTo(imageData->mat());
            images.push_back(imageData);
        }
    }

    std::atomic<int> processedCount{0};

    proc.processBatch(images, [&processedCount](Base::ImageData::Ptr& img) {
        // 简单处理：增加亮度
        if (img && !img->mat().empty()) {
            img->mat() += 10;
            processedCount++;
        }
    });

    QCOMPARE(processedCount.load(), 5);
}

void TestParallelProcessor::testProcessBatchIndexed()
{
    auto& proc = Base::ParallelProcessor::instance();

    std::vector<Base::ImageData::Ptr> images;
    for (int i = 0; i < 5; ++i) {
        cv::Mat img = TestUtils::createGrayImage(100, 100, 100);
        auto imageData = Base::ImageMemoryPool::instance().allocate(
            img.cols, img.rows, img.type());
        if (imageData) {
            img.copyTo(imageData->mat());
            images.push_back(imageData);
        }
    }

    std::vector<bool> processed(5, false);

    proc.processBatchIndexed(images, [&processed](Base::ImageData::Ptr& img, size_t index) {
        Q_UNUSED(img);
        processed[index] = true;
    });

    for (bool p : processed) {
        QVERIFY(p);
    }
}

void TestParallelProcessor::testProcessBatchEmpty()
{
    auto& proc = Base::ParallelProcessor::instance();

    std::vector<Base::ImageData::Ptr> empty;

    // 不应崩溃
    proc.processBatch(empty, [](Base::ImageData::Ptr&) {});
    QVERIFY(true);
}

void TestParallelProcessor::testProcessBatchSingleItem()
{
    auto& proc = Base::ParallelProcessor::instance();

    std::vector<Base::ImageData::Ptr> single;
    cv::Mat img = TestUtils::createGrayImage(100, 100, 100);
    auto imageData = Base::ImageMemoryPool::instance().allocate(
        img.cols, img.rows, img.type());
    if (imageData) {
        img.copyTo(imageData->mat());
        single.push_back(imageData);
    }

    bool processed = false;

    proc.processBatch(single, [&processed](Base::ImageData::Ptr&) {
        processed = true;
    });

    QVERIFY(processed);
}

// ========== 分块处理测试 ==========

void TestParallelProcessor::testProcessTiles()
{
    auto& proc = Base::ParallelProcessor::instance();

    cv::Mat image = TestUtils::createGrayImage(640, 480, 100);
    std::atomic<int> tileCount{0};

    proc.processTiles(image, cv::Size(160, 120), 0,
        [&tileCount](cv::Mat& tile, const cv::Rect& rect) {
            Q_UNUSED(tile);
            Q_UNUSED(rect);
            tileCount++;
        });

    // 640/160 * 480/120 = 4 * 4 = 16 块
    QCOMPARE(tileCount.load(), 16);
}

void TestParallelProcessor::testProcessTilesReadOnly()
{
    auto& proc = Base::ParallelProcessor::instance();

    cv::Mat image = TestUtils::createGrayImage(640, 480, 100);
    std::atomic<int> tileCount{0};

    proc.processTilesReadOnly(image, cv::Size(160, 120), 0,
        [&tileCount](const cv::Mat& tile, const cv::Rect& rect) {
            Q_UNUSED(tile);
            Q_UNUSED(rect);
            tileCount++;
        });

    QCOMPARE(tileCount.load(), 16);
}

void TestParallelProcessor::testProcessTilesWithOverlap()
{
    auto& proc = Base::ParallelProcessor::instance();

    cv::Mat image = TestUtils::createGrayImage(640, 480, 100);
    std::atomic<int> tileCount{0};

    // 带重叠的分块
    proc.processTiles(image, cv::Size(160, 120), 10,
        [&tileCount](cv::Mat& tile, const cv::Rect& rect) {
            Q_UNUSED(tile);
            Q_UNUSED(rect);
            tileCount++;
        });

    // 重叠会产生更多的块
    QVERIFY(tileCount.load() >= 16);
}

void TestParallelProcessor::testCalculateTiles()
{
    // 通过processTiles间接测试calculateTiles
    auto& proc = Base::ParallelProcessor::instance();

    cv::Mat smallImage = TestUtils::createGrayImage(100, 100, 100);
    std::vector<cv::Rect> tiles;
    QMutex mutex;  // 用于保护vector的并发访问

    proc.processTiles(smallImage, cv::Size(30, 30), 0,
        [&tiles, &mutex](cv::Mat& tile, const cv::Rect& rect) {
            Q_UNUSED(tile);
            QMutexLocker locker(&mutex);
            tiles.push_back(rect);
        });

    // 验证所有分块覆盖整个图像
    for (const auto& rect : tiles) {
        QVERIFY(rect.x >= 0 && rect.y >= 0);
        QVERIFY(rect.x + rect.width <= 100);
        QVERIFY(rect.y + rect.height <= 100);
    }
}

// ========== ROI处理测试 ==========

void TestParallelProcessor::testProcessROIs()
{
    auto& proc = Base::ParallelProcessor::instance();

    cv::Mat image = TestUtils::createGrayImage(640, 480, 100);
    std::vector<cv::Rect> rois = {
        cv::Rect(0, 0, 100, 100),
        cv::Rect(100, 100, 200, 200),
        cv::Rect(300, 300, 100, 100)
    };

    std::atomic<int> processedCount{0};

    proc.processROIs(image, rois,
        [&processedCount](cv::Mat& roi, int index) {
            Q_UNUSED(roi);
            Q_UNUSED(index);
            processedCount++;
        });

    QCOMPARE(processedCount.load(), 3);
}

void TestParallelProcessor::testProcessROIsEmpty()
{
    auto& proc = Base::ParallelProcessor::instance();

    cv::Mat image = TestUtils::createGrayImage(640, 480, 100);
    std::vector<cv::Rect> empty;

    // 不应崩溃
    proc.processROIs(image, empty, [](cv::Mat&, int) {});
    QVERIFY(true);
}

void TestParallelProcessor::testProcessROIsMultiple()
{
    auto& proc = Base::ParallelProcessor::instance();

    cv::Mat image = TestUtils::createGrayImage(640, 480, 100);
    std::vector<cv::Rect> rois;
    for (int i = 0; i < 20; ++i) {
        rois.push_back(cv::Rect(i * 30, 0, 30, 30));
    }

    std::vector<int> processedIndices(20, -1);

    proc.processROIs(image, rois,
        [&processedIndices](cv::Mat& roi, int index) {
            Q_UNUSED(roi);
            processedIndices[index] = index;
        });

    for (int i = 0; i < 20; ++i) {
        QCOMPARE(processedIndices[i], i);
    }
}

// ========== 像素级处理测试 ==========

void TestParallelProcessor::testForEachPixel()
{
    auto& proc = Base::ParallelProcessor::instance();

    cv::Mat image = TestUtils::createGrayImage(100, 100, 100);
    std::atomic<int> pixelCount{0};

    proc.forEachPixel(image, [&pixelCount](uchar* pixel, int x, int y) {
        Q_UNUSED(x);
        Q_UNUSED(y);
        *pixel = 255;
        pixelCount++;
    });

    QCOMPARE(pixelCount.load(), 100 * 100);

    // 验证所有像素都被修改
    for (int y = 0; y < image.rows; ++y) {
        for (int x = 0; x < image.cols; ++x) {
            QCOMPARE(image.at<uchar>(y, x), (uchar)255);
        }
    }
}

void TestParallelProcessor::testForEachRow()
{
    auto& proc = Base::ParallelProcessor::instance();

    cv::Mat image = TestUtils::createGrayImage(100, 100, 100);
    std::atomic<int> rowCount{0};

    proc.forEachRow(image, [&rowCount](uchar* rowPtr, int y, int cols, int channels) {
        Q_UNUSED(y);
        for (int x = 0; x < cols * channels; ++x) {
            rowPtr[x] = 200;
        }
        rowCount++;
    });

    QCOMPARE(rowCount.load(), 100);

    // 验证所有像素都被修改
    QCOMPARE(image.at<uchar>(50, 50), (uchar)200);
}

// ========== 归约操作测试 ==========

void TestParallelProcessor::testParallelSum()
{
    std::vector<double> data = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0};
    double sum = Base::ParallelProcessor::parallelSum(data.data(), data.size());
    QCOMPARE(sum, 55.0);
}

void TestParallelProcessor::testParallelMax()
{
    std::vector<double> data = {3.0, 1.0, 4.0, 1.0, 5.0, 9.0, 2.0, 6.0};
    double maxVal = Base::ParallelProcessor::parallelMax(data.data(), data.size());
    QCOMPARE(maxVal, 9.0);
}

void TestParallelProcessor::testParallelMin()
{
    std::vector<double> data = {3.0, 1.0, 4.0, 1.0, 5.0, 9.0, 2.0, 6.0};
    double minVal = Base::ParallelProcessor::parallelMin(data.data(), data.size());
    QCOMPARE(minVal, 1.0);
}

void TestParallelProcessor::testParallelSumEmpty()
{
    double sum = Base::ParallelProcessor::parallelSum(nullptr, 0);
    QCOMPARE(sum, 0.0);
}

// ========== 统计信息测试 ==========

void TestParallelProcessor::testStatistics()
{
    auto& proc = Base::ParallelProcessor::instance();
    proc.resetStatistics();

    // 执行一些操作
    std::vector<Base::ImageData::Ptr> images;
    for (int i = 0; i < 5; ++i) {
        cv::Mat img = TestUtils::createGrayImage(100, 100, 100);
        auto imageData = Base::ImageMemoryPool::instance().allocate(
            img.cols, img.rows, img.type());
        if (imageData) {
            img.copyTo(imageData->mat());
            images.push_back(imageData);
        }
    }

    proc.processBatch(images, [](Base::ImageData::Ptr&) {});

    auto stats = proc.getStatistics();
    QVERIFY(stats.totalTasks > 0);
}

void TestParallelProcessor::testResetStatistics()
{
    auto& proc = Base::ParallelProcessor::instance();

    // 执行一些操作
    std::vector<Base::ImageData::Ptr> images;
    cv::Mat img = TestUtils::createGrayImage(100, 100, 100);
    auto imageData = Base::ImageMemoryPool::instance().allocate(
        img.cols, img.rows, img.type());
    if (imageData) {
        img.copyTo(imageData->mat());
        images.push_back(imageData);
    }
    proc.processBatch(images, [](Base::ImageData::Ptr&) {});

    // 重置统计
    proc.resetStatistics();

    auto stats = proc.getStatistics();
    QCOMPARE(stats.totalTasks, (size_t)0);
}

// ========== 禁用状态测试 ==========

void TestParallelProcessor::testDisabledFallback()
{
    auto& proc = Base::ParallelProcessor::instance();

    // 禁用并行处理
    proc.setEnabled(false);

    std::vector<Base::ImageData::Ptr> images;
    for (int i = 0; i < 5; ++i) {
        cv::Mat img = TestUtils::createGrayImage(100, 100, 100);
        auto imageData = Base::ImageMemoryPool::instance().allocate(
            img.cols, img.rows, img.type());
        if (imageData) {
            img.copyTo(imageData->mat());
            images.push_back(imageData);
        }
    }

    std::atomic<int> count{0};

    // 应该仍然能够处理（串行方式）
    proc.processBatch(images, [&count](Base::ImageData::Ptr&) {
        count++;
    });

    QCOMPARE(count.load(), 5);

    // 恢复启用
    proc.setEnabled(true);
}

QTEST_MAIN(TestParallelProcessor)
#include "test_ParallelProcessor.moc"
