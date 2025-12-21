/**
 * @file test_ImageData_gtest.cpp
 * @brief ImageData类单元测试（Google Test版本）
 * @details 演示Google Test框架的使用，与Qt Test版本对比
 * @author VisionForge Team
 * @date 2025-12-20
 */

#include <gtest/gtest.h>
#include "base/ImageData.h"
#include "TestUtils.h"
#include <opencv2/core.hpp>

using namespace VisionForge;
using namespace VisionForge::Base;

// ========== 测试夹具（Test Fixture）==========

/**
 * @brief ImageData测试夹具
 *
 * Google Test使用测试夹具来共享测试设置和清理代码
 * 等价于Qt Test的initTestCase/init
 */
class ImageDataTest : public ::testing::Test {
protected:
    // 每个测试前调用
    void SetUp() override {
        // 创建测试图像
        testMat = cv::Mat(480, 640, CV_8UC3, cv::Scalar(255, 0, 0));
    }

    // 每个测试后调用
    void TearDown() override {
        testMat.release();
    }

    // 测试数据
    cv::Mat testMat;
};

// ========== 基本功能测试 ==========

TEST_F(ImageDataTest, Construction) {
    // 测试构造函数
    ImageData image(testMat);

    EXPECT_EQ(image.width(), 640);
    EXPECT_EQ(image.height(), 480);
    EXPECT_EQ(image.channels(), 3);
    EXPECT_FALSE(image.isEmpty());
}

TEST_F(ImageDataTest, EmptyImage) {
    // 测试空图像
    ImageData emptyImage;

    EXPECT_TRUE(emptyImage.isEmpty());
    EXPECT_EQ(emptyImage.width(), 0);
    EXPECT_EQ(emptyImage.height(), 0);
}

TEST_F(ImageDataTest, DeepCopy) {
    // 测试深拷贝方法（ImageData禁用了拷贝构造函数）
    ImageData original(testMat);
    auto copy = original.deepCopy();

    ASSERT_NE(copy, nullptr);
    EXPECT_EQ(copy->width(), original.width());
    EXPECT_EQ(copy->height(), original.height());

    // 验证深拷贝（数据分离）
    EXPECT_NE(copy->mat().data, original.mat().data);
}

TEST_F(ImageDataTest, Clone) {
    // 测试克隆方法（浅拷贝，共享数据）
    ImageData original(testMat);
    auto cloned = original.clone();

    ASSERT_NE(cloned, nullptr);
    EXPECT_EQ(cloned->width(), original.width());
    EXPECT_EQ(cloned->height(), original.height());

    // 验证浅拷贝（共享相同的数据）
    EXPECT_EQ(cloned->mat().data, original.mat().data);
}

// ========== 内存管理测试 ==========

TEST_F(ImageDataTest, TotalBytes) {
    ImageData image(testMat);

    // 640x480 * 3通道 = 921600字节
    EXPECT_EQ(image.totalBytes(), 640 * 480 * 3);
}

TEST_F(ImageDataTest, MemoryAlignment) {
    ImageData image(640, 480, CV_8UC3);

    // 检查内存对齐（默认32字节对齐）
    EXPECT_TRUE(image.isMemoryAligned());
    EXPECT_EQ(image.getAlignment(), 32);
}

TEST_F(ImageDataTest, GPUMemory) {
    ImageData image(testMat);

    // 初始状态没有GPU内存
    EXPECT_FALSE(image.hasGPUMemory());
    EXPECT_EQ(image.gpuPtr(), nullptr);

    // 分配GPU内存
    image.allocateGPU();
    EXPECT_TRUE(image.hasGPUMemory());
    EXPECT_NE(image.gpuPtr(), nullptr);

    // 释放GPU内存
    image.releaseGPU();
    EXPECT_FALSE(image.hasGPUMemory());
    EXPECT_EQ(image.gpuPtr(), nullptr);
}

// ========== 时间戳测试 ==========

TEST_F(ImageDataTest, Timestamp) {
    ImageData image(testMat);

    size_t timestamp = 1234567890;
    image.setTimestamp(timestamp);

    EXPECT_EQ(image.timestamp(), timestamp);
}

// ========== Qt集成测试 ==========

TEST_F(ImageDataTest, ConvertToQImage) {
    ImageData image(testMat);

    QImage qImage = image.toQImage();

    EXPECT_FALSE(qImage.isNull());
    EXPECT_EQ(qImage.width(), 640);
    EXPECT_EQ(qImage.height(), 480);

    // 验证格式正确（BGR -> RGB转换）
    EXPECT_EQ(qImage.format(), QImage::Format_RGB888);
}

// ========== 性能测试示例 ==========

TEST_F(ImageDataTest, DISABLED_PerformanceLargeImage) {
    // 性能测试（默认禁用）
    // 运行时使用: ./test_ImageData_gtest --gtest_also_run_disabled_tests

    cv::Mat largeMat(4096, 4096, CV_8UC3);

    auto start = std::chrono::high_resolution_clock::now();

    ImageData image(largeMat);
    auto clone = image.clone();

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    // 输出性能信息
    std::cout << "大图像克隆耗时: " << duration.count() << "ms" << std::endl;

    // 验证耗时在合理范围内（<100ms）
    EXPECT_LT(duration.count(), 100);
}

// ========== 参数化测试示例 ==========

/**
 * @brief 参数化测试：测试不同图像类型
 *
 * Google Test支持参数化测试，可以用相同的测试逻辑测试多组参数
 */
class ImageDataTypeTest : public ::testing::TestWithParam<int> {
protected:
    void SetUp() override {
        imageType = GetParam();
    }

    int imageType;
};

TEST_P(ImageDataTypeTest, SupportedTypes) {
    // 测试ImageData支持的各种OpenCV类型
    cv::Mat mat(480, 640, imageType);
    ImageData image(mat);

    EXPECT_FALSE(image.isEmpty());
    EXPECT_EQ(image.width(), 640);
    EXPECT_EQ(image.height(), 480);
}

// 注册参数化测试
INSTANTIATE_TEST_SUITE_P(
    AllImageTypes,
    ImageDataTypeTest,
    ::testing::Values(
        CV_8UC1,   // 8位单通道
        CV_8UC3,   // 8位三通道
        CV_16UC1,  // 16位单通道
        CV_16UC3,  // 16位三通道
        CV_32FC1,  // 32位浮点单通道
        CV_32FC3   // 32位浮点三通道
    )
);

// ========== 死亡测试（Death Test）示例 ==========

#ifdef NDEBUG
// 仅在Debug模式下运行死亡测试
TEST_F(ImageDataTest, DISABLED_InvalidMatrixAccess) {
#else
TEST_F(ImageDataTest, InvalidMatrixAccess) {
#endif
    ImageData emptyImage;

    // 验证访问空图像会触发断言
    // 注意：这需要在Debug模式下编译
    EXPECT_DEBUG_DEATH({
        emptyImage.mat().at<uchar>(0, 0) = 255;
    }, ".*");
}

// ========== 主函数 ==========

int main(int argc, char** argv) {
    // 初始化Google Test
    ::testing::InitGoogleTest(&argc, argv);

    // 设置测试输出格式
    ::testing::FLAGS_gtest_color = "yes";
    ::testing::FLAGS_gtest_print_time = true;

    // 运行所有测试
    return RUN_ALL_TESTS();
}
