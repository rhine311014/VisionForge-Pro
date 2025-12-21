/**
 * @file test_BasicImageProcessing_gtest.cpp
 * @brief 基础图像处理算法测试（Google Test）
 * @details 测试GrayTool、ThresholdTool、BlurTool等基础算法
 * @author VisionForge Team
 * @date 2025-12-20
 */

#include <gtest/gtest.h>
#include "algorithm/GrayTool.h"
#include "algorithm/ThresholdTool.h"
#include "algorithm/BlurTool.h"
#include "base/ImageMemoryPool.h"
#include "TestUtils.h"
#include <opencv2/imgproc.hpp>

using namespace VisionForge;
using namespace VisionForge::Algorithm;
using namespace VisionForge::Base;

// ========== 测试夹具 ==========

class BasicImageProcessingTest : public ::testing::Test {
protected:
    void SetUp() override {
        // 创建测试图像
        testImageColor = ImageMemoryPool::instance().allocate(640, 480, CV_8UC3);
        testImageColor->mat().setTo(cv::Scalar(255, 128, 64));

        // 创建有对比度的灰度图像（用于阈值测试）
        testImageGray = ImageMemoryPool::instance().allocate(640, 480, CV_8UC1);
        testImageGray->mat().setTo(cv::Scalar(100));  // 背景为100
        // 添加一些亮区域（用于阈值测试）
        cv::rectangle(testImageGray->mat(), cv::Point(100, 100), cv::Point(300, 300), cv::Scalar(200), -1);
    }

    void TearDown() override {
        // ScopedImagePtr自动归还，无需手动释放
    }

    ImageData::Ptr testImageColor;
    ImageData::Ptr testImageGray;
};

// ========== GrayTool 测试 ==========

TEST_F(BasicImageProcessingTest, GrayTool_BGR2Gray) {
    GrayTool grayTool;
    ToolResult result;

    bool success = grayTool.process(testImageColor, result);

    EXPECT_TRUE(success);
    EXPECT_TRUE(result.success);
    ASSERT_NE(result.outputImage, nullptr);

    // 验证输出是灰度图
    EXPECT_EQ(result.outputImage->channels(), 1);
    EXPECT_EQ(result.outputImage->width(), 640);
    EXPECT_EQ(result.outputImage->height(), 480);
}

TEST_F(BasicImageProcessingTest, GrayTool_AlreadyGray) {
    GrayTool grayTool;
    ToolResult result;

    // 输入已经是灰度图
    bool success = grayTool.process(testImageGray, result);

    EXPECT_TRUE(success);
    EXPECT_TRUE(result.success);
    ASSERT_NE(result.outputImage, nullptr);

    // 输出应该仍然是灰度图
    EXPECT_EQ(result.outputImage->channels(), 1);
}

TEST_F(BasicImageProcessingTest, GrayTool_NullInput) {
    GrayTool grayTool;
    ToolResult result;

    bool success = grayTool.process(nullptr, result);

    EXPECT_FALSE(success);
    EXPECT_FALSE(result.success);
    EXPECT_FALSE(result.errorMessage.isEmpty());
}

// ========== ThresholdTool 测试 ==========

TEST_F(BasicImageProcessingTest, ThresholdTool_BinaryThreshold) {
    ThresholdTool thresholdTool;

    // 设置参数：二值化阈值
    thresholdTool.setThreshold(128);
    thresholdTool.setMaxValue(255);
    thresholdTool.setThresholdType(ThresholdTool::Binary);

    ToolResult result;
    bool success = thresholdTool.process(testImageGray, result);

    EXPECT_TRUE(success);
    EXPECT_TRUE(result.success);
    ASSERT_NE(result.outputImage, nullptr);

    // 验证输出仍是灰度图
    EXPECT_EQ(result.outputImage->channels(), 1);

    // 验证像素值只有0或255
    cv::Mat& mat = result.outputImage->mat();
    double minVal, maxVal;
    cv::minMaxLoc(mat, &minVal, &maxVal);

    EXPECT_EQ(minVal, 0.0);
    EXPECT_EQ(maxVal, 255.0);
}

TEST_F(BasicImageProcessingTest, ThresholdTool_OtsuThreshold) {
    ThresholdTool thresholdTool;

    // 设置参数：Otsu自动阈值
    thresholdTool.setThresholdMethod(ThresholdTool::OTSU);
    thresholdTool.setThresholdType(ThresholdTool::Binary);

    ToolResult result;
    bool success = thresholdTool.process(testImageGray, result);

    EXPECT_TRUE(success);
    EXPECT_TRUE(result.success);
    ASSERT_NE(result.outputImage, nullptr);

    // Otsu应该自动计算阈值
    EXPECT_GT(thresholdTool.getComputedThreshold(), 0);
}

// ========== BlurTool 测试 ==========

TEST_F(BasicImageProcessingTest, BlurTool_GaussianBlur) {
    BlurTool blurTool;

    // 设置参数：高斯模糊
    blurTool.setBlurType(BlurTool::Gaussian);
    blurTool.setKernelSize(5);
    blurTool.setSigma(1.5);

    ToolResult result;
    bool success = blurTool.process(testImageGray, result);

    EXPECT_TRUE(success);
    EXPECT_TRUE(result.success);
    ASSERT_NE(result.outputImage, nullptr);

    // 验证输出尺寸和类型不变
    EXPECT_EQ(result.outputImage->width(), 640);
    EXPECT_EQ(result.outputImage->height(), 480);
    EXPECT_EQ(result.outputImage->channels(), 1);
}

TEST_F(BasicImageProcessingTest, BlurTool_MedianBlur) {
    BlurTool blurTool;

    // 设置参数：中值滤波
    blurTool.setBlurType(BlurTool::Median);
    blurTool.setKernelSize(5);

    ToolResult result;
    bool success = blurTool.process(testImageGray, result);

    EXPECT_TRUE(success);
    EXPECT_TRUE(result.success);
    ASSERT_NE(result.outputImage, nullptr);
}

TEST_F(BasicImageProcessingTest, BlurTool_InvalidKernelSize) {
    BlurTool blurTool;

    // 设置无效参数：偶数核大小
    blurTool.setBlurType(BlurTool::Gaussian);
    blurTool.setKernelSize(4);  // 应该是奇数

    ToolResult result;
    bool success = blurTool.process(testImageGray, result);

    // 工具应该拒绝或自动修正
    EXPECT_FALSE(success || result.errorMessage.contains("kernel"));
}

// ========== 参数化测试：不同核大小的模糊 ==========

class BlurKernelSizeTest : public ::testing::TestWithParam<int> {
protected:
    void SetUp() override {
        testImage = ImageMemoryPool::instance().allocate(640, 480, CV_8UC1);
        testImage->mat().setTo(cv::Scalar(128));
    }

    ImageData::Ptr testImage;
};

TEST_P(BlurKernelSizeTest, GaussianBlurWithDifferentKernels) {
    int kernelSize = GetParam();

    BlurTool blurTool;
    blurTool.setBlurType(BlurTool::Gaussian);
    blurTool.setKernelSize(kernelSize);

    ToolResult result;
    bool success = blurTool.process(testImage, result);

    EXPECT_TRUE(success);
    EXPECT_TRUE(result.success);
    ASSERT_NE(result.outputImage, nullptr);

    // 验证输出尺寸不变
    EXPECT_EQ(result.outputImage->width(), 640);
    EXPECT_EQ(result.outputImage->height(), 480);
}

// 测试奇数核大小：3, 5, 7, 9, 11
INSTANTIATE_TEST_SUITE_P(
    OddKernelSizes,
    BlurKernelSizeTest,
    ::testing::Values(3, 5, 7, 9, 11)
);

// ========== 参数化测试：阈值类型 ==========

class ThresholdTypeTest : public ::testing::TestWithParam<ThresholdTool::ThresholdType> {
protected:
    void SetUp() override {
        testImage = ImageMemoryPool::instance().allocate(640, 480, CV_8UC1);
        testImage->mat().setTo(cv::Scalar(100));  // 背景为100
        // 添加亮区域用于阈值测试
        cv::rectangle(testImage->mat(), cv::Point(100, 100), cv::Point(300, 300), cv::Scalar(200), -1);
    }

    ImageData::Ptr testImage;
};

TEST_P(ThresholdTypeTest, DifferentThresholdTypes) {
    ThresholdTool::ThresholdType thresholdType = GetParam();

    ThresholdTool thresholdTool;
    thresholdTool.setThreshold(128);
    thresholdTool.setMaxValue(255);
    thresholdTool.setThresholdType(thresholdType);

    ToolResult result;
    bool success = thresholdTool.process(testImage, result);

    EXPECT_TRUE(success);
    EXPECT_TRUE(result.success);
    ASSERT_NE(result.outputImage, nullptr);
}

// 测试各种阈值类型
INSTANTIATE_TEST_SUITE_P(
    AllThresholdTypes,
    ThresholdTypeTest,
    ::testing::Values(
        ThresholdTool::Binary,
        ThresholdTool::BinaryInv,
        ThresholdTool::Truncate,
        ThresholdTool::ToZero,
        ThresholdTool::ToZeroInv
    )
);

// ========== 性能测试 ==========

TEST_F(BasicImageProcessingTest, DISABLED_Performance_GrayConversion) {
    // 性能测试（默认禁用）
    // 运行: ./test_BasicImageProcessing_gtest --gtest_also_run_disabled_tests

    auto largeImage = ImageMemoryPool::instance().allocate(1920, 1080, CV_8UC3);
    largeImage->mat().setTo(cv::Scalar(128, 128, 128));

    GrayTool grayTool;
    ToolResult result;

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < 100; ++i) {
        grayTool.process(largeImage, result);
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "100次灰度转换(1920x1080)耗时: " << duration.count() << "ms" << std::endl;
    std::cout << "平均耗时: " << duration.count() / 100.0 << "ms/次" << std::endl;

    // 验证耗时在合理范围内
    EXPECT_LT(duration.count(), 1000);  // 100次应该在1秒内完成
}

// ========== 边界条件测试 ==========

TEST_F(BasicImageProcessingTest, EdgeCase_TinyImage) {
    // 测试极小图像
    auto tinyImage = ImageMemoryPool::instance().allocate(1, 1, CV_8UC3);
    tinyImage->mat().setTo(cv::Scalar(128, 128, 128));

    GrayTool grayTool;
    ToolResult result;

    bool success = grayTool.process(tinyImage, result);

    EXPECT_TRUE(success);
    EXPECT_TRUE(result.success);
}

TEST_F(BasicImageProcessingTest, EdgeCase_LargeImage) {
    // 测试大图像
    auto largeImage = ImageMemoryPool::instance().allocate(4096, 4096, CV_8UC3);
    largeImage->mat().setTo(cv::Scalar(128, 128, 128));

    GrayTool grayTool;
    ToolResult result;

    bool success = grayTool.process(largeImage, result);

    EXPECT_TRUE(success);
    EXPECT_TRUE(result.success);
}

// ========== 主函数 ==========

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::FLAGS_gtest_color = "yes";
    ::testing::FLAGS_gtest_print_time = true;

    return RUN_ALL_TESTS();
}
