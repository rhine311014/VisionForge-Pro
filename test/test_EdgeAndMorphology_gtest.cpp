/**
 * @file test_EdgeAndMorphology_gtest.cpp
 * @brief 边缘检测和形态学处理算法测试（Google Test）
 * @details 测试EdgeTool、FindEdgeTool、MorphologyTool等算法
 * @author VisionForge Team
 * @date 2025-12-20
 */

#include <gtest/gtest.h>
#include "algorithm/EdgeTool.h"
#include "algorithm/FindEdgeTool.h"
#include "algorithm/MorphologyTool.h"
#include "base/ImageMemoryPool.h"
#include "TestUtils.h"
#include <opencv2/imgproc.hpp>

using namespace VisionForge;
using namespace VisionForge::Algorithm;
using namespace VisionForge::Base;

// ========== 测试夹具 ==========

class EdgeAndMorphologyTest : public ::testing::Test {
protected:
    void SetUp() override {
        // 创建带边缘的测试图像
        testImageWithEdge = ImageMemoryPool::instance().allocate(640, 480, CV_8UC1);
        testImageWithEdge->mat().setTo(cv::Scalar(0));
        // 添加一个矩形（用于边缘检测）
        cv::rectangle(testImageWithEdge->mat(), cv::Point(200, 150), cv::Point(400, 350), cv::Scalar(255), -1);

        // 创建简单的二值图像（用于形态学测试）
        binaryImage = ImageMemoryPool::instance().allocate(640, 480, CV_8UC1);
        binaryImage->mat().setTo(cv::Scalar(0));
        // 添加一些白色圆点
        cv::circle(binaryImage->mat(), cv::Point(320, 240), 50, cv::Scalar(255), -1);
        cv::circle(binaryImage->mat(), cv::Point(150, 120), 30, cv::Scalar(255), -1);
    }

    void TearDown() override {
        // ScopedImagePtr自动归还，无需手动释放
    }

    ImageData::Ptr testImageWithEdge;
    ImageData::Ptr binaryImage;
};

// ========== EdgeTool 测试 ==========

TEST_F(EdgeAndMorphologyTest, EdgeTool_Canny) {
    EdgeTool edgeTool;

    edgeTool.setEdgeType(EdgeTool::Canny);
    edgeTool.setThreshold1(50.0);
    edgeTool.setThreshold2(150.0);
    edgeTool.setKernelSize(3);

    ToolResult result;
    bool success = edgeTool.process(testImageWithEdge, result);

    EXPECT_TRUE(success);
    EXPECT_TRUE(result.success);
    ASSERT_NE(result.outputImage, nullptr);

    // 验证输出是二值图（边缘图）
    EXPECT_EQ(result.outputImage->channels(), 1);

    // 验证找到了边缘
    cv::Mat& edges = result.outputImage->mat();
    int edgePixels = cv::countNonZero(edges);
    EXPECT_GT(edgePixels, 0);  // 应该检测到边缘
}

TEST_F(EdgeAndMorphologyTest, EdgeTool_Sobel) {
    EdgeTool edgeTool;

    edgeTool.setEdgeType(EdgeTool::Sobel);
    edgeTool.setKernelSize(3);
    edgeTool.setDirection(EdgeTool::Both);

    ToolResult result;
    bool success = edgeTool.process(testImageWithEdge, result);

    EXPECT_TRUE(success);
    EXPECT_TRUE(result.success);
    ASSERT_NE(result.outputImage, nullptr);

    // 验证输出尺寸不变
    EXPECT_EQ(result.outputImage->width(), 640);
    EXPECT_EQ(result.outputImage->height(), 480);
}

TEST_F(EdgeAndMorphologyTest, EdgeTool_Laplacian) {
    EdgeTool edgeTool;

    edgeTool.setEdgeType(EdgeTool::Laplacian);
    edgeTool.setKernelSize(3);

    ToolResult result;
    bool success = edgeTool.process(testImageWithEdge, result);

    EXPECT_TRUE(success);
    EXPECT_TRUE(result.success);
    ASSERT_NE(result.outputImage, nullptr);
}

TEST_F(EdgeAndMorphologyTest, EdgeTool_Scharr) {
    EdgeTool edgeTool;

    edgeTool.setEdgeType(EdgeTool::Scharr);
    edgeTool.setDirection(EdgeTool::Horizontal);

    ToolResult result;
    bool success = edgeTool.process(testImageWithEdge, result);

    EXPECT_TRUE(success);
    EXPECT_TRUE(result.success);
    ASSERT_NE(result.outputImage, nullptr);
}

TEST_F(EdgeAndMorphologyTest, EdgeTool_NullInput) {
    EdgeTool edgeTool;
    ToolResult result;

    bool success = edgeTool.process(nullptr, result);

    EXPECT_FALSE(success);
    EXPECT_FALSE(result.success);
}

// ========== FindEdgeTool 测试 ==========

TEST_F(EdgeAndMorphologyTest, FindEdgeTool_LineSearch) {
    FindEdgeTool findEdgeTool;

    // 设置搜索线（水平穿过矩形边缘）
    findEdgeTool.setSearchMode(FindEdgeTool::LineSearch);
    findEdgeTool.setSearchLine(QPointF(100, 240), QPointF(500, 240));
    findEdgeTool.setSearchWidth(10);

    findEdgeTool.setEdgeType(FindEdgeTool::AllEdges);
    findEdgeTool.setSelectMode(FindEdgeTool::AllEdgesFound);
    findEdgeTool.setMinAmplitude(50.0);

    ToolResult result;
    bool success = findEdgeTool.process(testImageWithEdge, result);

    EXPECT_TRUE(success);
    EXPECT_TRUE(result.success);

    // 应该找到至少2个边缘（矩形的左右边）
    int edgeCount = findEdgeTool.edgeCount();
    EXPECT_GE(edgeCount, 2);
}

TEST_F(EdgeAndMorphologyTest, FindEdgeTool_FirstEdge) {
    FindEdgeTool findEdgeTool;

    findEdgeTool.setSearchMode(FindEdgeTool::LineSearch);
    findEdgeTool.setSearchLine(QPointF(100, 240), QPointF(500, 240));
    findEdgeTool.setSearchWidth(10);
    findEdgeTool.setSelectMode(FindEdgeTool::FirstEdge);
    findEdgeTool.setMinAmplitude(50.0);

    ToolResult result;
    bool success = findEdgeTool.process(testImageWithEdge, result);

    EXPECT_TRUE(success);
    EXPECT_TRUE(result.success);

    // 验证找到了主边缘
    EdgePoint primary = findEdgeTool.primaryEdge();
    EXPECT_GT(primary.amplitude, 0);
}

TEST_F(EdgeAndMorphologyTest, FindEdgeTool_RectSearch) {
    FindEdgeTool findEdgeTool;

    // 设置矩形搜索区域
    findEdgeTool.setSearchMode(FindEdgeTool::RectSearch);
    findEdgeTool.setSearchRect(QRectF(180, 130, 240, 240), 0);
    findEdgeTool.setEdgeType(FindEdgeTool::AllEdges);
    findEdgeTool.setMinAmplitude(50.0);

    ToolResult result;
    bool success = findEdgeTool.process(testImageWithEdge, result);

    EXPECT_TRUE(success);
    EXPECT_TRUE(result.success);
}

// ========== MorphologyTool 测试 ==========

TEST_F(EdgeAndMorphologyTest, MorphologyTool_Erode) {
    MorphologyTool morphTool;

    morphTool.setMorphType(MorphologyTool::Erode);
    morphTool.setKernelShape(MorphologyTool::Rect);
    morphTool.setKernelWidth(5);
    morphTool.setKernelHeight(5);
    morphTool.setIterations(1);

    ToolResult result;
    bool success = morphTool.process(binaryImage, result);

    EXPECT_TRUE(success);
    EXPECT_TRUE(result.success);
    ASSERT_NE(result.outputImage, nullptr);

    // 腐蚀后白色像素应该减少
    int originalWhite = cv::countNonZero(binaryImage->mat());
    int erodedWhite = cv::countNonZero(result.outputImage->mat());
    EXPECT_LT(erodedWhite, originalWhite);
}

TEST_F(EdgeAndMorphologyTest, MorphologyTool_Dilate) {
    MorphologyTool morphTool;

    morphTool.setMorphType(MorphologyTool::Dilate);
    morphTool.setKernelShape(MorphologyTool::Rect);
    morphTool.setKernelWidth(5);
    morphTool.setKernelHeight(5);
    morphTool.setIterations(1);

    ToolResult result;
    bool success = morphTool.process(binaryImage, result);

    EXPECT_TRUE(success);
    EXPECT_TRUE(result.success);
    ASSERT_NE(result.outputImage, nullptr);

    // 膨胀后白色像素应该增加
    int originalWhite = cv::countNonZero(binaryImage->mat());
    int dilatedWhite = cv::countNonZero(result.outputImage->mat());
    EXPECT_GT(dilatedWhite, originalWhite);
}

TEST_F(EdgeAndMorphologyTest, MorphologyTool_Open) {
    MorphologyTool morphTool;

    morphTool.setMorphType(MorphologyTool::Open);
    morphTool.setKernelShape(MorphologyTool::Ellipse);
    morphTool.setKernelWidth(3);
    morphTool.setKernelHeight(3);
    morphTool.setIterations(1);

    ToolResult result;
    bool success = morphTool.process(binaryImage, result);

    EXPECT_TRUE(success);
    EXPECT_TRUE(result.success);
    ASSERT_NE(result.outputImage, nullptr);
}

TEST_F(EdgeAndMorphologyTest, MorphologyTool_Close) {
    MorphologyTool morphTool;

    morphTool.setMorphType(MorphologyTool::Close);
    morphTool.setKernelShape(MorphologyTool::Ellipse);
    morphTool.setKernelWidth(3);
    morphTool.setKernelHeight(3);
    morphTool.setIterations(1);

    ToolResult result;
    bool success = morphTool.process(binaryImage, result);

    EXPECT_TRUE(success);
    EXPECT_TRUE(result.success);
    ASSERT_NE(result.outputImage, nullptr);
}

TEST_F(EdgeAndMorphologyTest, MorphologyTool_Gradient) {
    MorphologyTool morphTool;

    morphTool.setMorphType(MorphologyTool::Gradient);
    morphTool.setKernelShape(MorphologyTool::Rect);
    morphTool.setKernelWidth(3);
    morphTool.setKernelHeight(3);

    ToolResult result;
    bool success = morphTool.process(binaryImage, result);

    EXPECT_TRUE(success);
    EXPECT_TRUE(result.success);
    ASSERT_NE(result.outputImage, nullptr);
}

TEST_F(EdgeAndMorphologyTest, MorphologyTool_TopHat) {
    MorphologyTool morphTool;

    morphTool.setMorphType(MorphologyTool::TopHat);
    morphTool.setKernelShape(MorphologyTool::Rect);
    morphTool.setKernelWidth(15);
    morphTool.setKernelHeight(15);

    ToolResult result;
    bool success = morphTool.process(binaryImage, result);

    EXPECT_TRUE(success);
    EXPECT_TRUE(result.success);
    ASSERT_NE(result.outputImage, nullptr);
}

TEST_F(EdgeAndMorphologyTest, MorphologyTool_BlackHat) {
    MorphologyTool morphTool;

    morphTool.setMorphType(MorphologyTool::BlackHat);
    morphTool.setKernelShape(MorphologyTool::Rect);
    morphTool.setKernelWidth(15);
    morphTool.setKernelHeight(15);

    ToolResult result;
    bool success = morphTool.process(binaryImage, result);

    EXPECT_TRUE(success);
    EXPECT_TRUE(result.success);
    ASSERT_NE(result.outputImage, nullptr);
}

// ========== 参数化测试：不同边缘检测算法 ==========

class EdgeTypeTest : public ::testing::TestWithParam<EdgeTool::EdgeType> {
protected:
    void SetUp() override {
        testImage = ImageMemoryPool::instance().allocate(640, 480, CV_8UC1);
        testImage->mat().setTo(cv::Scalar(0));
        cv::rectangle(testImage->mat(), cv::Point(200, 150), cv::Point(400, 350), cv::Scalar(255), -1);
    }

    ImageData::Ptr testImage;
};

TEST_P(EdgeTypeTest, AllEdgeAlgorithms) {
    EdgeTool::EdgeType edgeType = GetParam();

    EdgeTool edgeTool;
    edgeTool.setEdgeType(edgeType);
    edgeTool.setThreshold1(50.0);
    edgeTool.setThreshold2(150.0);
    edgeTool.setKernelSize(3);

    ToolResult result;
    bool success = edgeTool.process(testImage, result);

    EXPECT_TRUE(success);
    EXPECT_TRUE(result.success);
    ASSERT_NE(result.outputImage, nullptr);
}

INSTANTIATE_TEST_SUITE_P(
    AllEdgeTypes,
    EdgeTypeTest,
    ::testing::Values(
        EdgeTool::Canny,
        EdgeTool::Sobel,
        EdgeTool::Laplacian,
        EdgeTool::Scharr
    )
);

// ========== 参数化测试：不同形态学操作 ==========

class MorphTypeTest : public ::testing::TestWithParam<MorphologyTool::MorphType> {
protected:
    void SetUp() override {
        testImage = ImageMemoryPool::instance().allocate(640, 480, CV_8UC1);
        testImage->mat().setTo(cv::Scalar(0));
        cv::circle(testImage->mat(), cv::Point(320, 240), 50, cv::Scalar(255), -1);
    }

    ImageData::Ptr testImage;
};

TEST_P(MorphTypeTest, AllMorphOperations) {
    MorphologyTool::MorphType morphType = GetParam();

    MorphologyTool morphTool;
    morphTool.setMorphType(morphType);
    morphTool.setKernelShape(MorphologyTool::Rect);
    morphTool.setKernelWidth(5);
    morphTool.setKernelHeight(5);
    morphTool.setIterations(1);

    ToolResult result;
    bool success = morphTool.process(testImage, result);

    EXPECT_TRUE(success);
    EXPECT_TRUE(result.success);
    ASSERT_NE(result.outputImage, nullptr);
}

INSTANTIATE_TEST_SUITE_P(
    AllMorphTypes,
    MorphTypeTest,
    ::testing::Values(
        MorphologyTool::Erode,
        MorphologyTool::Dilate,
        MorphologyTool::Open,
        MorphologyTool::Close,
        MorphologyTool::Gradient,
        MorphologyTool::TopHat,
        MorphologyTool::BlackHat
    )
);

// ========== 参数化测试：不同核形状 ==========

class KernelShapeTest : public ::testing::TestWithParam<MorphologyTool::KernelShape> {
protected:
    void SetUp() override {
        testImage = ImageMemoryPool::instance().allocate(640, 480, CV_8UC1);
        testImage->mat().setTo(cv::Scalar(0));
        cv::circle(testImage->mat(), cv::Point(320, 240), 50, cv::Scalar(255), -1);
    }

    ImageData::Ptr testImage;
};

TEST_P(KernelShapeTest, AllKernelShapes) {
    MorphologyTool::KernelShape shape = GetParam();

    MorphologyTool morphTool;
    morphTool.setMorphType(MorphologyTool::Dilate);
    morphTool.setKernelShape(shape);
    morphTool.setKernelWidth(5);
    morphTool.setKernelHeight(5);

    ToolResult result;
    bool success = morphTool.process(testImage, result);

    EXPECT_TRUE(success);
    EXPECT_TRUE(result.success);
    ASSERT_NE(result.outputImage, nullptr);
}

INSTANTIATE_TEST_SUITE_P(
    AllShapes,
    KernelShapeTest,
    ::testing::Values(
        MorphologyTool::Rect,
        MorphologyTool::Cross,
        MorphologyTool::Ellipse
    )
);

// ========== 性能测试 ==========

TEST_F(EdgeAndMorphologyTest, DISABLED_Performance_CannyEdgeDetection) {
    // 性能测试（默认禁用）
    auto largeImage = ImageMemoryPool::instance().allocate(1920, 1080, CV_8UC1);
    largeImage->mat().setTo(cv::Scalar(128));

    EdgeTool edgeTool;
    edgeTool.setEdgeType(EdgeTool::Canny);
    edgeTool.setThreshold1(50.0);
    edgeTool.setThreshold2(150.0);

    ToolResult result;

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < 100; ++i) {
        edgeTool.process(largeImage, result);
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "100次Canny边缘检测(1920x1080)耗时: " << duration.count() << "ms" << std::endl;
    std::cout << "平均耗时: " << duration.count() / 100.0 << "ms/次" << std::endl;

    EXPECT_LT(duration.count(), 2000);  // 100次应该在2秒内完成
}

// ========== 主函数 ==========

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::FLAGS_gtest_color = "yes";
    ::testing::FLAGS_gtest_print_time = true;

    return RUN_ALL_TESTS();
}
