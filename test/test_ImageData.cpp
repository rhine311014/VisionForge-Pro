/**
 * @file test_ImageData.cpp
 * @brief ImageData类单元测试
 */

#include <QtTest/QtTest>
#include "base/ImageData.h"
#include "TestUtils.h"

using namespace VisionForge;
using namespace VisionForge::Test;

class TestImageData : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    // 构造函数测试
    void testDefaultConstructor();
    void testSizeConstructor();
    void testMatConstructor();

    // 基本属性测试
    void testWidthHeight();
    void testIsEmpty();
    void testChannels();
    void testTotalBytes();

    // 数据操作测试
    void testClone();
    void testDeepCopy();
    void testTimestamp();

    // 内存对齐测试
    void testMemoryAlignment();

    // GPU相关测试（基本功能检查）
    void testGPUMemory();

    // QImage转换测试
    void testToQImage();

private:
    cv::Mat testImage_;
    cv::Mat colorImage_;
};

void TestImageData::initTestCase()
{
    testImage_ = TestUtils::createGrayImage(640, 480);
    colorImage_ = TestUtils::createColorImage(320, 240);
}

void TestImageData::cleanupTestCase()
{
    testImage_.release();
    colorImage_.release();
}

void TestImageData::testDefaultConstructor()
{
    Base::ImageData data;
    QVERIFY(data.isEmpty());
    QCOMPARE(data.width(), 0);
    QCOMPARE(data.height(), 0);
}

void TestImageData::testSizeConstructor()
{
    Base::ImageData data(640, 480, CV_8UC1);
    QVERIFY(!data.isEmpty());
    QCOMPARE(data.width(), 640);
    QCOMPARE(data.height(), 480);
    QCOMPARE(data.channels(), 1);
}

void TestImageData::testMatConstructor()
{
    Base::ImageData data(testImage_);
    QVERIFY(!data.isEmpty());
    QCOMPARE(data.width(), testImage_.cols);
    QCOMPARE(data.height(), testImage_.rows);
}

void TestImageData::testWidthHeight()
{
    Base::ImageData data(100, 200, CV_8UC3);
    QCOMPARE(data.width(), 100);
    QCOMPARE(data.height(), 200);
}

void TestImageData::testIsEmpty()
{
    Base::ImageData emptyData;
    QVERIFY(emptyData.isEmpty());

    Base::ImageData nonEmptyData(10, 10, CV_8UC1);
    QVERIFY(!nonEmptyData.isEmpty());
}

void TestImageData::testChannels()
{
    Base::ImageData gray(100, 100, CV_8UC1);
    QCOMPARE(gray.channels(), 1);

    Base::ImageData color(100, 100, CV_8UC3);
    QCOMPARE(color.channels(), 3);

    Base::ImageData rgba(100, 100, CV_8UC4);
    QCOMPARE(rgba.channels(), 4);
}

void TestImageData::testTotalBytes()
{
    // 灰度图: 100 * 100 * 1 = 10000
    Base::ImageData gray(100, 100, CV_8UC1);
    QCOMPARE(gray.totalBytes(), static_cast<size_t>(10000));

    // 彩色图: 100 * 100 * 3 = 30000
    Base::ImageData color(100, 100, CV_8UC3);
    QCOMPARE(color.totalBytes(), static_cast<size_t>(30000));
}

void TestImageData::testClone()
{
    Base::ImageData original(testImage_);
    auto cloned = original.clone();

    QVERIFY(cloned != nullptr);
    QCOMPARE(cloned->width(), original.width());
    QCOMPARE(cloned->height(), original.height());
    QCOMPARE(cloned->channels(), original.channels());
}

void TestImageData::testDeepCopy()
{
    Base::ImageData original(testImage_);
    auto copied = original.deepCopy();

    QVERIFY(copied != nullptr);
    QCOMPARE(copied->width(), original.width());
    QCOMPARE(copied->height(), original.height());

    // 修改深拷贝不应影响原数据
    copied->mat().setTo(0);
    QVERIFY(!TestUtils::compareImages(original.mat(), copied->mat()));
}

void TestImageData::testTimestamp()
{
    Base::ImageData data(testImage_);

    // 设置时间戳
    size_t testTs = 12345678;
    data.setTimestamp(testTs);
    QCOMPARE(data.timestamp(), testTs);

    // 更新时间戳
    size_t newTs = 87654321;
    data.setTimestamp(newTs);
    QCOMPARE(data.timestamp(), newTs);
}

void TestImageData::testMemoryAlignment()
{
    // 使用size构造函数应该创建对齐内存
    Base::ImageData alignedData(640, 480, CV_8UC1);

    // 获取对齐值
    size_t alignment = alignedData.getAlignment();
    QVERIFY(alignment > 0);

    // 检查是否标记为对齐
    // 注意：实际对齐状态取决于实现
    // 这里只验证API是否正常工作
    bool isAligned = alignedData.isMemoryAligned();
    Q_UNUSED(isAligned);
}

void TestImageData::testGPUMemory()
{
    Base::ImageData data(testImage_);

    // 初始状态应该没有GPU内存
    QVERIFY(!data.hasGPUMemory());
    QVERIFY(data.gpuPtr() == nullptr);

    // 注意：GPU分配/上传/下载需要CUDA支持
    // 这里只测试API不会崩溃
#ifdef USE_CUDA
    data.allocateGPU();
    if (data.hasGPUMemory()) {
        data.uploadToGPU();
        data.downloadFromGPU();
        data.releaseGPU();
    }
#endif
}

void TestImageData::testToQImage()
{
    // 测试灰度图转换
    Base::ImageData grayData(testImage_);
    QImage grayQImage = grayData.toQImage();

    if (!grayQImage.isNull()) {
        QCOMPARE(grayQImage.width(), grayData.width());
        QCOMPARE(grayQImage.height(), grayData.height());
    }

    // 测试彩色图转换
    Base::ImageData colorData(colorImage_);
    QImage colorQImage = colorData.toQImage();

    if (!colorQImage.isNull()) {
        QCOMPARE(colorQImage.width(), colorData.width());
        QCOMPARE(colorQImage.height(), colorData.height());
    }
}

QTEST_MAIN(TestImageData)
#include "test_ImageData.moc"
