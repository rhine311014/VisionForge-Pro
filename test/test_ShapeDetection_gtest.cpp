/**
 * @file test_ShapeDetection_gtest.cpp
 * @brief 形状检测算法测试（Google Test）
 * @details 测试CircleTool、LineTool、BlobTool等算法
 * @author VisionForge Team
 * @date 2025-12-20
 */

#include <gtest/gtest.h>
#include "algorithm/CircleTool.h"
#include "algorithm/LineTool.h"
#include "algorithm/BlobTool.h"
#include "base/ImageMemoryPool.h"
#include "TestUtils.h"
#include <opencv2/imgproc.hpp>

using namespace VisionForge;
using namespace VisionForge::Algorithm;
using namespace VisionForge::Base;

// ========== 测试夹具 ==========

class ShapeDetectionTest : public ::testing::Test {
protected:
    void SetUp() override {
        // 创建带圆形的测试图像
        imageWithCircles = ImageMemoryPool::instance().allocate(640, 480, CV_8UC1);
        imageWithCircles->mat().setTo(cv::Scalar(0));
        cv::circle(imageWithCircles->mat(), cv::Point(200, 200), 80, cv::Scalar(255), 2);
        cv::circle(imageWithCircles->mat(), cv::Point(450, 300), 60, cv::Scalar(255), 2);

        // 创建带直线的测试图像
        imageWithLines = ImageMemoryPool::instance().allocate(640, 480, CV_8UC1);
        imageWithLines->mat().setTo(cv::Scalar(0));
        cv::line(imageWithLines->mat(), cv::Point(100, 100), cv::Point(500, 100), cv::Scalar(255), 2);
        cv::line(imageWithLines->mat(), cv::Point(320, 50), cv::Point(320, 400), cv::Scalar(255), 2);

        // 创建Blob测试图像
        blobImage = ImageMemoryPool::instance().allocate(640, 480, CV_8UC1);
        blobImage->mat().setTo(cv::Scalar(0));
        // 添加几个不同大小的白色区域
        cv::circle(blobImage->mat(), cv::Point(150, 150), 40, cv::Scalar(255), -1);
        cv::rectangle(blobImage->mat(), cv::Point(300, 100), cv::Point(450, 200), cv::Scalar(255), -1);
        cv::circle(blobImage->mat(), cv::Point(400, 350), 25, cv::Scalar(255), -1);
    }

    void TearDown() override {
        // ScopedImagePtr自动归还，无需手动释放
    }

    ImageData::Ptr imageWithCircles;
    ImageData::Ptr imageWithLines;
    ImageData::Ptr blobImage;
};

// ========== CircleTool 测试 ==========

TEST_F(ShapeDetectionTest, CircleTool_HoughCircle) {
    CircleTool circleTool;

    circleTool.setBackend(CircleTool::OpenCV);
    circleTool.setMethod(CircleTool::HoughCircle);
    circleTool.setMinRadius(50);
    circleTool.setMaxRadius(100);
    circleTool.setHoughDp(1.0);
    circleTool.setMinDist(100);
    circleTool.setCannyThreshold(50);
    circleTool.setAccumThreshold(30);

    ToolResult result;
    bool success = circleTool.process(imageWithCircles, result);

    EXPECT_TRUE(success);
    EXPECT_TRUE(result.success);

    // 应该检测到至少1个圆
    int circleCount = circleTool.circleCount();
    EXPECT_GE(circleCount, 1);

    // 验证圆的基本属性
    if (circleCount > 0) {
        const auto& circles = circleTool.circles();
        EXPECT_GT(circles[0].radius, 50);
        EXPECT_LT(circles[0].radius, 100);
        EXPECT_GT(circles[0].area, 0);
    }
}

TEST_F(ShapeDetectionTest, CircleTool_ContourFit) {
    CircleTool circleTool;

    circleTool.setBackend(CircleTool::OpenCV);
    circleTool.setMethod(CircleTool::ContourFit);
    circleTool.setMinRadius(50);
    circleTool.setMaxRadius(100);
    circleTool.setMinCircularity(0.7);
    circleTool.setMinArea(3000);

    ToolResult result;
    bool success = circleTool.process(imageWithCircles, result);

    EXPECT_TRUE(success);
    EXPECT_TRUE(result.success);
}

TEST_F(ShapeDetectionTest, CircleTool_NullInput) {
    CircleTool circleTool;
    ToolResult result;

    bool success = circleTool.process(nullptr, result);

    EXPECT_FALSE(success);
    EXPECT_FALSE(result.success);
}

TEST_F(ShapeDetectionTest, CircleTool_RadiusRange) {
    CircleTool circleTool;

    circleTool.setBackend(CircleTool::OpenCV);
    circleTool.setMethod(CircleTool::HoughCircle);
    circleTool.setMinRadius(70);  // 只能检测到大圆
    circleTool.setMaxRadius(90);
    circleTool.setHoughDp(1.0);
    circleTool.setMinDist(100);
    circleTool.setCannyThreshold(50);
    circleTool.setAccumThreshold(30);

    ToolResult result;
    bool success = circleTool.process(imageWithCircles, result);

    EXPECT_TRUE(success);

    // 验证检测到的圆符合半径范围
    const auto& circles = circleTool.circles();
    for (const auto& circle : circles) {
        EXPECT_GE(circle.radius, 70);
        EXPECT_LE(circle.radius, 90);
    }
}

// ========== LineTool 测试 ==========

TEST_F(ShapeDetectionTest, LineTool_HoughLinesP) {
    LineTool lineTool;

    lineTool.setBackend(LineTool::OpenCV);
    lineTool.setMethod(LineTool::HoughLinesP);
    lineTool.setMinLength(100);
    lineTool.setMaxGap(10);
    lineTool.setRho(1.0);
    lineTool.setTheta(CV_PI / 180);
    lineTool.setThreshold(100);

    ToolResult result;
    bool success = lineTool.process(imageWithLines, result);

    EXPECT_TRUE(success);
    EXPECT_TRUE(result.success);

    // 应该检测到至少1条线
    int lineCount = lineTool.lineCount();
    EXPECT_GE(lineCount, 1);

    // 验证线的基本属性
    if (lineCount > 0) {
        const auto& lines = lineTool.lines();
        EXPECT_GT(lines[0].length, 100);  // 长度应该大于最小长度
        EXPECT_GE(lines[0].angle, 0);
        EXPECT_LE(lines[0].angle, 180);
    }
}

TEST_F(ShapeDetectionTest, LineTool_HoughLines) {
    LineTool lineTool;

    lineTool.setBackend(LineTool::OpenCV);
    lineTool.setMethod(LineTool::HoughLines);
    lineTool.setRho(1.0);
    lineTool.setTheta(CV_PI / 180);
    lineTool.setThreshold(100);

    ToolResult result;
    bool success = lineTool.process(imageWithLines, result);

    EXPECT_TRUE(success);
    EXPECT_TRUE(result.success);
}

TEST_F(ShapeDetectionTest, LineTool_MinLength) {
    LineTool lineTool;

    lineTool.setBackend(LineTool::OpenCV);
    lineTool.setMethod(LineTool::HoughLinesP);
    lineTool.setMinLength(300);  // 较长的最小长度
    lineTool.setMaxGap(10);
    lineTool.setRho(1.0);
    lineTool.setTheta(CV_PI / 180);
    lineTool.setThreshold(100);

    ToolResult result;
    bool success = lineTool.process(imageWithLines, result);

    EXPECT_TRUE(success);

    // 验证所有检测到的线都满足最小长度
    const auto& lines = lineTool.lines();
    for (const auto& line : lines) {
        EXPECT_GE(line.length, 300);
    }
}

TEST_F(ShapeDetectionTest, LineTool_NullInput) {
    LineTool lineTool;
    ToolResult result;

    bool success = lineTool.process(nullptr, result);

    EXPECT_FALSE(success);
    EXPECT_FALSE(result.success);
}

// ========== BlobTool 测试 ==========

TEST_F(ShapeDetectionTest, BlobTool_BasicDetection) {
    BlobTool blobTool;

    blobTool.setBackend(BlobTool::OpenCV);
    blobTool.setConnectivity(BlobTool::Connect8);
    blobTool.setPolarity(BlobTool::Light);
    blobTool.setSortBy(BlobTool::SortByArea);
    blobTool.setSortDescending(true);

    ToolResult result;
    bool success = blobTool.process(blobImage, result);

    EXPECT_TRUE(success);
    EXPECT_TRUE(result.success);

    // 应该检测到3个Blob
    int blobCount = blobTool.blobCount();
    EXPECT_EQ(blobCount, 3);

    // 验证Blob按面积降序排列
    const auto& blobs = blobTool.blobFeatures();
    if (blobs.size() >= 2) {
        EXPECT_GE(blobs[0].area, blobs[1].area);
    }
}

TEST_F(ShapeDetectionTest, BlobTool_AreaFilter) {
    BlobTool blobTool;

    blobTool.setBackend(BlobTool::OpenCV);
    blobTool.setConnectivity(BlobTool::Connect8);
    blobTool.setPolarity(BlobTool::Light);

    // 设置面积过滤：只保留大Blob
    blobTool.setAreaRange(5000, 100000);

    ToolResult result;
    bool success = blobTool.process(blobImage, result);

    EXPECT_TRUE(success);
    EXPECT_TRUE(result.success);

    // 验证所有Blob都满足面积条件
    const auto& blobs = blobTool.blobFeatures();
    for (const auto& blob : blobs) {
        EXPECT_GE(blob.area, 5000);
        EXPECT_LE(blob.area, 100000);
    }
}

TEST_F(ShapeDetectionTest, BlobTool_CircularityFilter) {
    BlobTool blobTool;

    blobTool.setBackend(BlobTool::OpenCV);
    blobTool.setConnectivity(BlobTool::Connect8);
    blobTool.setPolarity(BlobTool::Light);

    // 设置圆度过滤：只保留接近圆形的Blob
    blobTool.setCircularityRange(0.7, 1.0);

    ToolResult result;
    bool success = blobTool.process(blobImage, result);

    EXPECT_TRUE(success);

    // 验证所有Blob都满足圆度条件
    const auto& blobs = blobTool.blobFeatures();
    for (const auto& blob : blobs) {
        EXPECT_GE(blob.circularity, 0.7);
        EXPECT_LE(blob.circularity, 1.0);
    }
}

TEST_F(ShapeDetectionTest, BlobTool_FeatureCalculation) {
    BlobTool blobTool;

    blobTool.setBackend(BlobTool::OpenCV);
    blobTool.setConnectivity(BlobTool::Connect8);
    blobTool.setPolarity(BlobTool::Light);

    ToolResult result;
    bool success = blobTool.process(blobImage, result);

    EXPECT_TRUE(success);

    // 验证特征计算正确
    const auto& blobs = blobTool.blobFeatures();
    ASSERT_GT(blobs.size(), 0);

    const auto& blob = blobs[0];

    // 基本特征应该有效
    EXPECT_GT(blob.area, 0);
    EXPECT_GT(blob.perimeter, 0);
    EXPECT_GT(blob.circularity, 0);
    EXPECT_LE(blob.circularity, 1.0);

    // 质心应该在图像范围内
    EXPECT_GE(blob.center.x(), 0);
    EXPECT_LT(blob.center.x(), 640);
    EXPECT_GE(blob.center.y(), 0);
    EXPECT_LT(blob.center.y(), 480);

    // 外接矩形应该有效
    EXPECT_GT(blob.boundingBox.width(), 0);
    EXPECT_GT(blob.boundingBox.height(), 0);
}

TEST_F(ShapeDetectionTest, BlobTool_Connect4vs8) {
    BlobTool blobTool4;
    blobTool4.setBackend(BlobTool::OpenCV);
    blobTool4.setConnectivity(BlobTool::Connect4);
    blobTool4.setPolarity(BlobTool::Light);

    BlobTool blobTool8;
    blobTool8.setBackend(BlobTool::OpenCV);
    blobTool8.setConnectivity(BlobTool::Connect8);
    blobTool8.setPolarity(BlobTool::Light);

    ToolResult result4, result8;
    blobTool4.process(blobImage, result4);
    blobTool8.process(blobImage, result8);

    // 4连通和8连通的结果应该相同或相似
    // （对于规则形状，结果应该相同）
    EXPECT_EQ(blobTool4.blobCount(), blobTool8.blobCount());
}

TEST_F(ShapeDetectionTest, BlobTool_NullInput) {
    BlobTool blobTool;
    ToolResult result;

    bool success = blobTool.process(nullptr, result);

    EXPECT_FALSE(success);
    EXPECT_FALSE(result.success);
}

// ========== 参数化测试：不同圆检测方法 ==========

class CircleMethodTest : public ::testing::TestWithParam<CircleTool::DetectionMethod> {
protected:
    void SetUp() override {
        testImage = ImageMemoryPool::instance().allocate(640, 480, CV_8UC1);
        testImage->mat().setTo(cv::Scalar(0));
        cv::circle(testImage->mat(), cv::Point(320, 240), 80, cv::Scalar(255), 2);
    }

    ImageData::Ptr testImage;
};

TEST_P(CircleMethodTest, AllCircleMethods) {
    CircleTool::DetectionMethod method = GetParam();

    CircleTool circleTool;
    circleTool.setBackend(CircleTool::OpenCV);
    circleTool.setMethod(method);
    circleTool.setMinRadius(50);
    circleTool.setMaxRadius(100);

    // 根据方法设置特定参数
    if (method == CircleTool::HoughCircle) {
        circleTool.setHoughDp(1.0);
        circleTool.setMinDist(100);
        circleTool.setCannyThreshold(50);
        circleTool.setAccumThreshold(30);
    } else if (method == CircleTool::ContourFit) {
        circleTool.setMinCircularity(0.7);
        circleTool.setMinArea(3000);
    }

    ToolResult result;
    bool success = circleTool.process(testImage, result);

    EXPECT_TRUE(success);
    EXPECT_TRUE(result.success);
}

INSTANTIATE_TEST_SUITE_P(
    AllMethods,
    CircleMethodTest,
    ::testing::Values(
        CircleTool::HoughCircle,
        CircleTool::ContourFit
        // EdgeFit和BlobFit需要Halcon，这里省略
    )
);

// ========== 参数化测试：不同连通性 ==========

class BlobConnectivityTest : public ::testing::TestWithParam<BlobTool::Connectivity> {
protected:
    void SetUp() override {
        testImage = ImageMemoryPool::instance().allocate(640, 480, CV_8UC1);
        testImage->mat().setTo(cv::Scalar(0));
        cv::circle(testImage->mat(), cv::Point(320, 240), 50, cv::Scalar(255), -1);
    }

    ImageData::Ptr testImage;
};

TEST_P(BlobConnectivityTest, AllConnectivities) {
    BlobTool::Connectivity connectivity = GetParam();

    BlobTool blobTool;
    blobTool.setBackend(BlobTool::OpenCV);
    blobTool.setConnectivity(connectivity);
    blobTool.setPolarity(BlobTool::Light);

    ToolResult result;
    bool success = blobTool.process(testImage, result);

    EXPECT_TRUE(success);
    EXPECT_TRUE(result.success);
    EXPECT_GE(blobTool.blobCount(), 1);
}

INSTANTIATE_TEST_SUITE_P(
    AllConnectivities,
    BlobConnectivityTest,
    ::testing::Values(
        BlobTool::Connect4,
        BlobTool::Connect8
    )
);

// ========== 参数化测试：Blob排序方式 ==========

class BlobSortTest : public ::testing::TestWithParam<BlobTool::SortBy> {
protected:
    void SetUp() override {
        testImage = ImageMemoryPool::instance().allocate(640, 480, CV_8UC1);
        testImage->mat().setTo(cv::Scalar(0));
        cv::circle(testImage->mat(), cv::Point(150, 150), 30, cv::Scalar(255), -1);
        cv::circle(testImage->mat(), cv::Point(400, 300), 50, cv::Scalar(255), -1);
        cv::circle(testImage->mat(), cv::Point(500, 100), 40, cv::Scalar(255), -1);
    }

    ImageData::Ptr testImage;
};

TEST_P(BlobSortTest, AllSortMethods) {
    BlobTool::SortBy sortBy = GetParam();

    BlobTool blobTool;
    blobTool.setBackend(BlobTool::OpenCV);
    blobTool.setConnectivity(BlobTool::Connect8);
    blobTool.setPolarity(BlobTool::Light);
    blobTool.setSortBy(sortBy);
    blobTool.setSortDescending(true);

    ToolResult result;
    bool success = blobTool.process(testImage, result);

    EXPECT_TRUE(success);
    EXPECT_TRUE(result.success);

    const auto& blobs = blobTool.blobFeatures();
    ASSERT_GE(blobs.size(), 2);

    // 验证排序
    if (sortBy == BlobTool::SortByArea && blobs.size() >= 2) {
        EXPECT_GE(blobs[0].area, blobs[1].area);
    } else if (sortBy == BlobTool::SortByX && blobs.size() >= 2) {
        EXPECT_GE(blobs[0].center.x(), blobs[1].center.x());
    } else if (sortBy == BlobTool::SortByY && blobs.size() >= 2) {
        EXPECT_GE(blobs[0].center.y(), blobs[1].center.y());
    }
}

INSTANTIATE_TEST_SUITE_P(
    AllSortMethods,
    BlobSortTest,
    ::testing::Values(
        BlobTool::NoSort,
        BlobTool::SortByArea,
        BlobTool::SortByX,
        BlobTool::SortByY,
        BlobTool::SortByCircularity
    )
);

// ========== 性能测试 ==========

TEST_F(ShapeDetectionTest, DISABLED_Performance_CircleDetection) {
    // 性能测试（默认禁用）
    auto largeImage = ImageMemoryPool::instance().allocate(1920, 1080, CV_8UC1);
    largeImage->mat().setTo(cv::Scalar(0));
    cv::circle(largeImage->mat(), cv::Point(960, 540), 200, cv::Scalar(255), 2);

    CircleTool circleTool;
    circleTool.setBackend(CircleTool::OpenCV);
    circleTool.setMethod(CircleTool::HoughCircle);
    circleTool.setMinRadius(150);
    circleTool.setMaxRadius(250);

    ToolResult result;

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < 100; ++i) {
        circleTool.process(largeImage, result);
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "100次圆检测(1920x1080)耗时: " << duration.count() << "ms" << std::endl;
    std::cout << "平均耗时: " << duration.count() / 100.0 << "ms/次" << std::endl;

    EXPECT_LT(duration.count(), 5000);  // 100次应该在5秒内完成
}

// ========== 主函数 ==========

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::FLAGS_gtest_color = "yes";
    ::testing::FLAGS_gtest_print_time = true;

    return RUN_ALL_TESTS();
}
