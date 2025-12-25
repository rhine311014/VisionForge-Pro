/**
 * @file test_GPUAccelerator.cpp
 * @brief GPUAccelerator类单元测试
 */

#include <QtTest/QtTest>
#include "base/GPUAccelerator.h"
#include "TestUtils.h"

using namespace VisionForge;
using namespace VisionForge::Test;

class TestGPUAccelerator : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    // 设备检测测试
    void testIsCudaAvailable();
    void testGetDeviceCount();
    void testGetDeviceInfo();
    void testGetAllDevices();

    // 加速模式测试
    void testSetAccelMode();
    void testGetAccelMode();
    void testIsUsingGPU();
    void testGetAccelModeName();
    void testSetEnabled();

    // 设备切换测试
    void testSetDevice();
    void testGetCurrentDevice();

    // 设置持久化测试
    void testSaveLoadSettings();

#ifdef USE_CUDA
    // GPU操作测试（仅在CUDA可用时）
    void testUploadDownload();
    void testGaussianBlur();
    void testCvtColor();
    void testThreshold();
    void testSobel();
    void testCanny();
    void testResize();
    void testErode();
    void testDilate();
    void testMedianFilter();
    void testBilateralFilter();
    void testArithmeticOperations();
    void testAsyncOperations();

    // GpuMemoryPool测试 - 已禁用，因为GpuMemoryPool现在是内部实现细节
    // void testGpuMemoryPoolAllocate();
    // void testGpuMemoryPoolRelease();
    // void testGpuMemoryPoolPreallocate();
    // void testGpuMemoryPoolStatistics();
    // void testGpuMemoryPoolClear();
#endif

private:
    cv::Mat testGrayImage_;
    cv::Mat testColorImage_;
};

void TestGPUAccelerator::initTestCase()
{
    testGrayImage_ = TestUtils::createGrayImage(640, 480, 128);
    testColorImage_ = TestUtils::createColorImage(640, 480, 100, 150, 200);
}

void TestGPUAccelerator::cleanupTestCase()
{
    testGrayImage_.release();
    testColorImage_.release();
}

// ========== 设备检测测试 ==========

void TestGPUAccelerator::testIsCudaAvailable()
{
    auto& gpu = Base::GPUAccelerator::instance();
    // 只要不崩溃就算通过，返回值取决于系统配置
    bool available = gpu.isCudaAvailable();
    Q_UNUSED(available);
    QVERIFY(true);
}

void TestGPUAccelerator::testGetDeviceCount()
{
    auto& gpu = Base::GPUAccelerator::instance();
    int count = gpu.getDeviceCount();
    QVERIFY(count >= 0);
}

void TestGPUAccelerator::testGetDeviceInfo()
{
    auto& gpu = Base::GPUAccelerator::instance();

    if (gpu.getDeviceCount() == 0) {
        QSKIP("No GPU devices available");
    }

    auto info = gpu.getDeviceInfo(0);
    QVERIFY(info.deviceId == 0);
    QVERIFY(!info.name.isEmpty());
    QVERIFY(info.totalMemory > 0);
}

void TestGPUAccelerator::testGetAllDevices()
{
    auto& gpu = Base::GPUAccelerator::instance();
    auto devices = gpu.getAllDevices();

    QCOMPARE(devices.size(), gpu.getDeviceCount());

    for (int i = 0; i < devices.size(); ++i) {
        QCOMPARE(devices[i].deviceId, i);
    }
}

// ========== 加速模式测试 ==========

void TestGPUAccelerator::testSetAccelMode()
{
    auto& gpu = Base::GPUAccelerator::instance();

    gpu.setAccelMode(Base::GPUAccelMode::Disabled);
    QCOMPARE(gpu.getAccelMode(), Base::GPUAccelMode::Disabled);

    gpu.setAccelMode(Base::GPUAccelMode::Auto);
    QCOMPARE(gpu.getAccelMode(), Base::GPUAccelMode::Auto);

    gpu.setAccelMode(Base::GPUAccelMode::CUDA);
    QCOMPARE(gpu.getAccelMode(), Base::GPUAccelMode::CUDA);
}

void TestGPUAccelerator::testGetAccelMode()
{
    auto& gpu = Base::GPUAccelerator::instance();
    Base::GPUAccelMode mode = gpu.getAccelMode();

    QVERIFY(mode == Base::GPUAccelMode::Disabled ||
            mode == Base::GPUAccelMode::CUDA ||
            mode == Base::GPUAccelMode::Auto);
}

void TestGPUAccelerator::testIsUsingGPU()
{
    auto& gpu = Base::GPUAccelerator::instance();

    // 禁用模式下不应使用GPU
    gpu.setAccelMode(Base::GPUAccelMode::Disabled);
    QVERIFY(!gpu.isUsingGPU());

    // 恢复Auto模式
    gpu.setAccelMode(Base::GPUAccelMode::Auto);
}

void TestGPUAccelerator::testGetAccelModeName()
{
    QString disabled = Base::GPUAccelerator::getAccelModeName(Base::GPUAccelMode::Disabled);
    QString cuda = Base::GPUAccelerator::getAccelModeName(Base::GPUAccelMode::CUDA);
    QString autoMode = Base::GPUAccelerator::getAccelModeName(Base::GPUAccelMode::Auto);

    QVERIFY(!disabled.isEmpty());
    QVERIFY(!cuda.isEmpty());
    QVERIFY(!autoMode.isEmpty());
}

void TestGPUAccelerator::testSetEnabled()
{
    auto& gpu = Base::GPUAccelerator::instance();

    gpu.setEnabled(false);
    QVERIFY(!gpu.isEnabled());

    gpu.setEnabled(true);
    // isEnabled取决于CUDA是否可用
}

// ========== 设备切换测试 ==========

void TestGPUAccelerator::testSetDevice()
{
    auto& gpu = Base::GPUAccelerator::instance();

    if (gpu.getDeviceCount() == 0) {
        QSKIP("No GPU devices available");
    }

    bool success = gpu.setDevice(0);
    QVERIFY(success);

    // 测试无效设备ID
    success = gpu.setDevice(-1);
    QVERIFY(!success);

    success = gpu.setDevice(9999);
    QVERIFY(!success);
}

void TestGPUAccelerator::testGetCurrentDevice()
{
    auto& gpu = Base::GPUAccelerator::instance();

    if (gpu.getDeviceCount() == 0) {
        QSKIP("No GPU devices available");
    }

    int device = gpu.getCurrentDevice();
    QVERIFY(device >= 0);
    QVERIFY(device < gpu.getDeviceCount());
}

// ========== 设置持久化测试 ==========

void TestGPUAccelerator::testSaveLoadSettings()
{
    auto& gpu = Base::GPUAccelerator::instance();

    // 保存当前设置
    Base::GPUAccelMode originalMode = gpu.getAccelMode();

    // 修改设置
    gpu.setAccelMode(Base::GPUAccelMode::Disabled);
    gpu.saveSettings();

    // 修改为其他值
    gpu.setAccelMode(Base::GPUAccelMode::Auto);

    // 重新加载
    gpu.loadSettings();
    QCOMPARE(gpu.getAccelMode(), Base::GPUAccelMode::Disabled);

    // 恢复原设置
    gpu.setAccelMode(originalMode);
    gpu.saveSettings();
}

#ifdef USE_CUDA

// ========== GPU操作测试 ==========

void TestGPUAccelerator::testUploadDownload()
{
    auto& gpu = Base::GPUAccelerator::instance();
    if (!gpu.isCudaAvailable()) {
        QSKIP("CUDA not available");
    }

    cv::cuda::GpuMat gpuImage;
    cv::Mat downloadedImage;

    Base::GPUAccelerator::upload(testGrayImage_, gpuImage);
    QCOMPARE(gpuImage.rows, testGrayImage_.rows);
    QCOMPARE(gpuImage.cols, testGrayImage_.cols);
    QCOMPARE(gpuImage.type(), testGrayImage_.type());

    Base::GPUAccelerator::download(gpuImage, downloadedImage);
    QVERIFY(TestUtils::compareImages(testGrayImage_, downloadedImage, 0.0));
}

void TestGPUAccelerator::testGaussianBlur()
{
    auto& gpu = Base::GPUAccelerator::instance();
    if (!gpu.isCudaAvailable()) {
        QSKIP("CUDA not available");
    }

    cv::cuda::GpuMat gpuSrc, gpuDst;
    Base::GPUAccelerator::upload(testGrayImage_, gpuSrc);

    Base::GPUAccelerator::gaussianBlur(gpuSrc, gpuDst, cv::Size(5, 5), 1.0);

    QCOMPARE(gpuDst.rows, gpuSrc.rows);
    QCOMPARE(gpuDst.cols, gpuSrc.cols);
}

void TestGPUAccelerator::testCvtColor()
{
    auto& gpu = Base::GPUAccelerator::instance();
    if (!gpu.isCudaAvailable()) {
        QSKIP("CUDA not available");
    }

    cv::cuda::GpuMat gpuSrc, gpuDst;
    Base::GPUAccelerator::upload(testColorImage_, gpuSrc);

    Base::GPUAccelerator::cvtColor(gpuSrc, gpuDst, cv::COLOR_BGR2GRAY);

    QCOMPARE(gpuDst.rows, gpuSrc.rows);
    QCOMPARE(gpuDst.cols, gpuSrc.cols);
    QCOMPARE(gpuDst.channels(), 1);
}

void TestGPUAccelerator::testThreshold()
{
    auto& gpu = Base::GPUAccelerator::instance();
    if (!gpu.isCudaAvailable()) {
        QSKIP("CUDA not available");
    }

    cv::cuda::GpuMat gpuSrc, gpuDst;
    Base::GPUAccelerator::upload(testGrayImage_, gpuSrc);

    Base::GPUAccelerator::threshold(gpuSrc, gpuDst, 128, 255, cv::THRESH_BINARY);

    QCOMPARE(gpuDst.rows, gpuSrc.rows);
    QCOMPARE(gpuDst.cols, gpuSrc.cols);
}

void TestGPUAccelerator::testSobel()
{
    auto& gpu = Base::GPUAccelerator::instance();
    if (!gpu.isCudaAvailable()) {
        QSKIP("CUDA not available");
    }

    cv::cuda::GpuMat gpuSrc, gpuDst;
    Base::GPUAccelerator::upload(testGrayImage_, gpuSrc);

    Base::GPUAccelerator::sobel(gpuSrc, gpuDst, CV_32F, 1, 0, 3);

    QCOMPARE(gpuDst.rows, gpuSrc.rows);
    QCOMPARE(gpuDst.cols, gpuSrc.cols);
}

void TestGPUAccelerator::testCanny()
{
    auto& gpu = Base::GPUAccelerator::instance();
    if (!gpu.isCudaAvailable()) {
        QSKIP("CUDA not available");
    }

    cv::cuda::GpuMat gpuSrc, gpuDst;
    Base::GPUAccelerator::upload(testGrayImage_, gpuSrc);

    Base::GPUAccelerator::canny(gpuSrc, gpuDst, 50, 150);

    QCOMPARE(gpuDst.rows, gpuSrc.rows);
    QCOMPARE(gpuDst.cols, gpuSrc.cols);
}

void TestGPUAccelerator::testResize()
{
    auto& gpu = Base::GPUAccelerator::instance();
    if (!gpu.isCudaAvailable()) {
        QSKIP("CUDA not available");
    }

    cv::cuda::GpuMat gpuSrc, gpuDst;
    Base::GPUAccelerator::upload(testGrayImage_, gpuSrc);

    cv::Size newSize(320, 240);
    Base::GPUAccelerator::resize(gpuSrc, gpuDst, newSize);

    QCOMPARE(gpuDst.rows, newSize.height);
    QCOMPARE(gpuDst.cols, newSize.width);
}

void TestGPUAccelerator::testErode()
{
    auto& gpu = Base::GPUAccelerator::instance();
    if (!gpu.isCudaAvailable()) {
        QSKIP("CUDA not available");
    }

    cv::cuda::GpuMat gpuSrc, gpuDst;
    Base::GPUAccelerator::upload(testGrayImage_, gpuSrc);

    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
    Base::GPUAccelerator::erode(gpuSrc, gpuDst, kernel, 2);

    QCOMPARE(gpuDst.rows, gpuSrc.rows);
    QCOMPARE(gpuDst.cols, gpuSrc.cols);
}

void TestGPUAccelerator::testDilate()
{
    auto& gpu = Base::GPUAccelerator::instance();
    if (!gpu.isCudaAvailable()) {
        QSKIP("CUDA not available");
    }

    cv::cuda::GpuMat gpuSrc, gpuDst;
    Base::GPUAccelerator::upload(testGrayImage_, gpuSrc);

    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
    Base::GPUAccelerator::dilate(gpuSrc, gpuDst, kernel, 2);

    QCOMPARE(gpuDst.rows, gpuSrc.rows);
    QCOMPARE(gpuDst.cols, gpuSrc.cols);
}

void TestGPUAccelerator::testMedianFilter()
{
    auto& gpu = Base::GPUAccelerator::instance();
    if (!gpu.isCudaAvailable()) {
        QSKIP("CUDA not available");
    }

    cv::cuda::GpuMat gpuSrc, gpuDst;
    Base::GPUAccelerator::upload(testGrayImage_, gpuSrc);

    Base::GPUAccelerator::medianFilter(gpuSrc, gpuDst, 5);

    QCOMPARE(gpuDst.rows, gpuSrc.rows);
    QCOMPARE(gpuDst.cols, gpuSrc.cols);
}

void TestGPUAccelerator::testBilateralFilter()
{
    auto& gpu = Base::GPUAccelerator::instance();
    if (!gpu.isCudaAvailable()) {
        QSKIP("CUDA not available");
    }

    cv::cuda::GpuMat gpuSrc, gpuDst;
    Base::GPUAccelerator::upload(testGrayImage_, gpuSrc);

    Base::GPUAccelerator::bilateralFilter(gpuSrc, gpuDst, 9, 75, 75);

    QCOMPARE(gpuDst.rows, gpuSrc.rows);
    QCOMPARE(gpuDst.cols, gpuSrc.cols);
}

void TestGPUAccelerator::testArithmeticOperations()
{
    auto& gpu = Base::GPUAccelerator::instance();
    if (!gpu.isCudaAvailable()) {
        QSKIP("CUDA not available");
    }

    cv::cuda::GpuMat gpuSrc1, gpuSrc2, gpuDst;
    Base::GPUAccelerator::upload(testGrayImage_, gpuSrc1);
    Base::GPUAccelerator::upload(testGrayImage_, gpuSrc2);

    // 加法
    Base::GPUAccelerator::add(gpuSrc1, gpuSrc2, gpuDst);
    QCOMPARE(gpuDst.size(), gpuSrc1.size());

    // 减法
    Base::GPUAccelerator::subtract(gpuSrc1, gpuSrc2, gpuDst);
    QCOMPARE(gpuDst.size(), gpuSrc1.size());

    // 绝对差值
    Base::GPUAccelerator::absdiff(gpuSrc1, gpuSrc2, gpuDst);
    QCOMPARE(gpuDst.size(), gpuSrc1.size());
}

void TestGPUAccelerator::testAsyncOperations()
{
    auto& gpu = Base::GPUAccelerator::instance();
    if (!gpu.isCudaAvailable()) {
        QSKIP("CUDA not available");
    }

    cv::cuda::Stream& stream = gpu.getDefaultStream();
    cv::cuda::GpuMat gpuImage;
    cv::Mat downloadedImage;

    Base::GPUAccelerator::uploadAsync(testGrayImage_, gpuImage, stream);
    Base::GPUAccelerator::waitStream(stream);

    QCOMPARE(gpuImage.rows, testGrayImage_.rows);
    QCOMPARE(gpuImage.cols, testGrayImage_.cols);

    Base::GPUAccelerator::downloadAsync(gpuImage, downloadedImage, stream);
    Base::GPUAccelerator::waitStream(stream);

    QVERIFY(TestUtils::compareImages(testGrayImage_, downloadedImage, 0.0));
}

// ========== GpuMemoryPool测试 - 已禁用 ==========
// GpuMemoryPool现在是内部实现细节，不再公开API
// 这些测试已被禁用

/*
void TestGPUAccelerator::testGpuMemoryPoolAllocate()
{
    auto& gpu = Base::GPUAccelerator::instance();
    if (!gpu.isCudaAvailable()) {
        QSKIP("CUDA not available");
    }

    auto& pool = Base::GpuMemoryPool::instance();

    cv::cuda::GpuMat mat = pool.allocate(480, 640, CV_8UC1);
    QCOMPARE(mat.rows, 480);
    QCOMPARE(mat.cols, 640);
    QCOMPARE(mat.type(), CV_8UC1);
}

void TestGPUAccelerator::testGpuMemoryPoolRelease()
{
    auto& gpu = Base::GPUAccelerator::instance();
    if (!gpu.isCudaAvailable()) {
        QSKIP("CUDA not available");
    }

    auto& pool = Base::GpuMemoryPool::instance();

    cv::cuda::GpuMat mat = pool.allocate(480, 640, CV_8UC1);
    pool.release(mat);

    // 再次分配相同规格应该命中缓存
    cv::cuda::GpuMat mat2 = pool.allocate(480, 640, CV_8UC1);
    QCOMPARE(mat2.rows, 480);
    QCOMPARE(mat2.cols, 640);
}

void TestGPUAccelerator::testGpuMemoryPoolPreallocate()
{
    auto& gpu = Base::GPUAccelerator::instance();
    if (!gpu.isCudaAvailable()) {
        QSKIP("CUDA not available");
    }

    auto& pool = Base::GpuMemoryPool::instance();

    size_t sizeBefore = pool.getPoolSize();
    pool.preallocate(240, 320, CV_8UC3, 3);
    size_t sizeAfter = pool.getPoolSize();

    QVERIFY(sizeAfter >= sizeBefore + 3);
}

void TestGPUAccelerator::testGpuMemoryPoolStatistics()
{
    auto& gpu = Base::GPUAccelerator::instance();
    if (!gpu.isCudaAvailable()) {
        QSKIP("CUDA not available");
    }

    auto& pool = Base::GpuMemoryPool::instance();
    pool.resetStatistics();

    // 分配和释放
    cv::cuda::GpuMat mat = pool.allocate(100, 100, CV_8UC1);
    pool.release(mat);

    auto stats = pool.getStatistics();
    QVERIFY(stats.totalAllocations > 0);
}

void TestGPUAccelerator::testGpuMemoryPoolClear()
{
    auto& gpu = Base::GPUAccelerator::instance();
    if (!gpu.isCudaAvailable()) {
        QSKIP("CUDA not available");
    }

    auto& pool = Base::GpuMemoryPool::instance();

    // 分配一些内存
    cv::cuda::GpuMat mat = pool.allocate(100, 100, CV_8UC1);
    pool.release(mat);

    // 清空
    pool.clear();
    QCOMPARE(pool.getPoolSize(), (size_t)0);
}
*/

#endif // USE_CUDA

QTEST_MAIN(TestGPUAccelerator)
#include "test_GPUAccelerator.moc"
