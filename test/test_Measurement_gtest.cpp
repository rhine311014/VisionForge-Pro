/**
 * @file test_Measurement_gtest.cpp
 * @brief 测量算法测试（Google Test）
 * @details 测试MeasureDistanceTool、MeasureAngleTool、MeasureAreaTool等算法
 * @author VisionForge Team
 * @date 2025-12-20
 */

#include <gtest/gtest.h>
#include "algorithm/MeasureDistanceTool.h"
#include "algorithm/MeasureAngleTool.h"
#include "algorithm/MeasureAreaTool.h"
#include "base/ImageMemoryPool.h"
#include "TestUtils.h"
#include <opencv2/imgproc.hpp>
#include <cmath>

using namespace VisionForge;
using namespace VisionForge::Algorithm;
using namespace VisionForge::Base;

// ========== 测试夹具 ==========

class MeasurementTest : public ::testing::Test {
protected:
    void SetUp() override {
        // 创建空白测试图像
        testImage = ImageMemoryPool::instance().allocate(640, 480, CV_8UC1);
        testImage->mat().setTo(cv::Scalar(0));

        // 创建带轮廓的测试图像（用于面积测量）
        contourImage = ImageMemoryPool::instance().allocate(640, 480, CV_8UC1);
        contourImage->mat().setTo(cv::Scalar(0));
        cv::rectangle(contourImage->mat(), cv::Point(200, 150), cv::Point(400, 350), cv::Scalar(255), -1);
        cv::circle(contourImage->mat(), cv::Point(500, 100), 50, cv::Scalar(255), -1);
    }

    void TearDown() override {
        // ScopedImagePtr自动归还，无需手动释放
    }

    ImageData::Ptr testImage;
    ImageData::Ptr contourImage;
};

// ========== MeasureDistanceTool 测试 ==========

TEST_F(MeasurementTest, MeasureDistance_PointToPoint) {
    MeasureDistanceTool distanceTool;

    distanceTool.setMeasureMode(MeasureDistanceTool::PointToPoint);
    distanceTool.setPoint1(QPointF(100, 100));
    distanceTool.setPoint2(QPointF(400, 100));

    ToolResult result;
    bool success = distanceTool.process(testImage, result);

    EXPECT_TRUE(success);
    EXPECT_TRUE(result.success);

    // 验证距离计算正确 (300像素)
    const auto& distResult = distanceTool.result();
    EXPECT_TRUE(distResult.valid);
    EXPECT_NEAR(distResult.distance, 300.0, 0.1);
}

TEST_F(MeasurementTest, MeasureDistance_PointToPoint_Diagonal) {
    MeasureDistanceTool distanceTool;

    distanceTool.setMeasureMode(MeasureDistanceTool::PointToPoint);
    distanceTool.setPoint1(QPointF(0, 0));
    distanceTool.setPoint2(QPointF(300, 400));

    ToolResult result;
    bool success = distanceTool.process(testImage, result);

    EXPECT_TRUE(success);

    // 验证距离计算正确 (3-4-5直角三角形，斜边=500)
    const auto& distResult = distanceTool.result();
    EXPECT_NEAR(distResult.distance, 500.0, 0.1);
}

TEST_F(MeasurementTest, MeasureDistance_PointToLine) {
    MeasureDistanceTool distanceTool;

    distanceTool.setMeasureMode(MeasureDistanceTool::PointToLine);
    distanceTool.setPoint1(QPointF(300, 300));  // 测试点
    distanceTool.setLine1(QLineF(QPointF(100, 100), QPointF(500, 100)));  // 水平线

    ToolResult result;
    bool success = distanceTool.process(testImage, result);

    EXPECT_TRUE(success);

    // 点到水平线的距离应该是200像素
    const auto& distResult = distanceTool.result();
    EXPECT_NEAR(distResult.distance, 200.0, 0.1);
}

TEST_F(MeasurementTest, MeasureDistance_LineToLine_Parallel) {
    MeasureDistanceTool distanceTool;

    distanceTool.setMeasureMode(MeasureDistanceTool::LineToLine);
    distanceTool.setLine1(QLineF(QPointF(100, 100), QPointF(500, 100)));  // 第一条水平线
    distanceTool.setLine2(QLineF(QPointF(100, 200), QPointF(500, 200)));  // 第二条水平线

    ToolResult result;
    bool success = distanceTool.process(testImage, result);

    EXPECT_TRUE(success);

    // 两条平行线距离应该是100像素
    const auto& distResult = distanceTool.result();
    EXPECT_NEAR(distResult.distance, 100.0, 0.1);
}

TEST_F(MeasurementTest, MeasureDistance_PixelToMm) {
    MeasureDistanceTool distanceTool;

    distanceTool.setMeasureMode(MeasureDistanceTool::PointToPoint);
    distanceTool.setPoint1(QPointF(0, 0));
    distanceTool.setPoint2(QPointF(100, 0));
    distanceTool.setPixelToMm(0.1);  // 1像素 = 0.1毫米

    ToolResult result;
    bool success = distanceTool.process(testImage, result);

    EXPECT_TRUE(success);

    const auto& distResult = distanceTool.result();
    EXPECT_NEAR(distResult.distance, 100.0, 0.1);
    EXPECT_NEAR(distResult.distanceMm, 10.0, 0.01);  // 100像素 * 0.1 = 10毫米
}

TEST_F(MeasurementTest, MeasureDistance_NullInput) {
    MeasureDistanceTool distanceTool;
    ToolResult result;

    bool success = distanceTool.process(nullptr, result);

    // 距离测量工具不依赖图像内容，应该仍然成功
    // 但如果实现要求图像，则应该失败
    // 这里假设实现需要图像作为绘制底图
    if (!success) {
        EXPECT_FALSE(result.success);
    }
}

// ========== MeasureAngleTool 测试 ==========

TEST_F(MeasurementTest, MeasureAngle_LineToLine_Perpendicular) {
    MeasureAngleTool angleTool;

    angleTool.setMeasureMode(MeasureAngleTool::LineToLine);
    angleTool.setLine1(QLineF(QPointF(100, 100), QPointF(400, 100)));  // 水平线
    angleTool.setLine2(QLineF(QPointF(250, 50), QPointF(250, 350)));   // 垂直线

    ToolResult result;
    bool success = angleTool.process(testImage, result);

    EXPECT_TRUE(success);
    EXPECT_TRUE(result.success);

    // 两条垂直线的夹角应该是90度
    const auto& angleResult = angleTool.result();
    EXPECT_TRUE(angleResult.valid);
    EXPECT_NEAR(angleResult.angle, 90.0, 0.1);
}

TEST_F(MeasurementTest, MeasureAngle_LineToLine_Parallel) {
    MeasureAngleTool angleTool;

    angleTool.setMeasureMode(MeasureAngleTool::LineToLine);
    angleTool.setLine1(QLineF(QPointF(100, 100), QPointF(400, 100)));
    angleTool.setLine2(QLineF(QPointF(100, 200), QPointF(400, 200)));

    ToolResult result;
    bool success = angleTool.process(testImage, result);

    EXPECT_TRUE(success);

    // 两条平行线的夹角应该是0度（或180度）
    const auto& angleResult = angleTool.result();
    EXPECT_TRUE(angleResult.angle == 0.0 || angleResult.angle == 180.0);
}

TEST_F(MeasurementTest, MeasureAngle_ThreePoints) {
    MeasureAngleTool angleTool;

    angleTool.setMeasureMode(MeasureAngleTool::ThreePoints);
    angleTool.setVertex(QPointF(300, 200));
    angleTool.setPoint1(QPointF(100, 200));
    angleTool.setPoint2(QPointF(300, 400));

    ToolResult result;
    bool success = angleTool.process(testImage, result);

    EXPECT_TRUE(success);

    // 三点构成直角
    const auto& angleResult = angleTool.result();
    EXPECT_NEAR(angleResult.angle, 90.0, 0.1);
}

TEST_F(MeasurementTest, MeasureAngle_LineToHorizontal) {
    MeasureAngleTool angleTool;

    angleTool.setMeasureMode(MeasureAngleTool::LineToHorizontal);
    angleTool.setLine1(QLineF(QPointF(100, 100), QPointF(400, 100)));  // 水平线

    ToolResult result;
    bool success = angleTool.process(testImage, result);

    EXPECT_TRUE(success);

    // 水平线与水平方向夹角应该是0度
    const auto& angleResult = angleTool.result();
    EXPECT_NEAR(angleResult.angle, 0.0, 0.1);
}

TEST_F(MeasurementTest, MeasureAngle_LineToVertical) {
    MeasureAngleTool angleTool;

    angleTool.setMeasureMode(MeasureAngleTool::LineToVertical);
    angleTool.setLine1(QLineF(QPointF(200, 50), QPointF(200, 350)));  // 垂直线

    ToolResult result;
    bool success = angleTool.process(testImage, result);

    EXPECT_TRUE(success);

    // 垂直线与垂直方向夹角应该是0度
    const auto& angleResult = angleTool.result();
    EXPECT_NEAR(angleResult.angle, 0.0, 0.1);
}

TEST_F(MeasurementTest, MeasureAngle_45Degrees) {
    MeasureAngleTool angleTool;

    angleTool.setMeasureMode(MeasureAngleTool::LineToHorizontal);
    angleTool.setLine1(QLineF(QPointF(100, 100), QPointF(400, 400)));  // 45度线

    ToolResult result;
    bool success = angleTool.process(testImage, result);

    EXPECT_TRUE(success);

    const auto& angleResult = angleTool.result();
    EXPECT_NEAR(angleResult.angle, 45.0, 0.1);
}

// ========== MeasureAreaTool 测试 ==========

TEST_F(MeasurementTest, MeasureArea_Polygon_Triangle) {
    MeasureAreaTool areaTool;

    areaTool.setMeasureMode(MeasureAreaTool::Polygon);

    // 创建一个三角形
    QPolygonF triangle;
    triangle << QPointF(0, 0) << QPointF(100, 0) << QPointF(50, 100);
    areaTool.setPolygon(triangle);

    ToolResult result;
    bool success = areaTool.process(testImage, result);

    EXPECT_TRUE(success);
    EXPECT_TRUE(result.success);

    // 三角形面积 = 底 * 高 / 2 = 100 * 100 / 2 = 5000
    const auto& areaResult = areaTool.result();
    EXPECT_TRUE(areaResult.valid);
    EXPECT_NEAR(areaResult.area, 5000.0, 1.0);
}

TEST_F(MeasurementTest, MeasureArea_Polygon_Rectangle) {
    MeasureAreaTool areaTool;

    areaTool.setMeasureMode(MeasureAreaTool::Polygon);

    // 创建一个矩形
    QPolygonF rectangle;
    rectangle << QPointF(0, 0) << QPointF(200, 0) << QPointF(200, 100) << QPointF(0, 100);
    areaTool.setPolygon(rectangle);

    ToolResult result;
    bool success = areaTool.process(testImage, result);

    EXPECT_TRUE(success);

    // 矩形面积 = 200 * 100 = 20000
    const auto& areaResult = areaTool.result();
    EXPECT_NEAR(areaResult.area, 20000.0, 1.0);
}

TEST_F(MeasurementTest, MeasureArea_Circle) {
    MeasureAreaTool areaTool;

    areaTool.setMeasureMode(MeasureAreaTool::Circle);
    areaTool.setCircle(QPointF(320, 240), 100.0);  // 半径100

    ToolResult result;
    bool success = areaTool.process(testImage, result);

    EXPECT_TRUE(success);

    // 圆形面积 = π * r² = π * 100² = 31415.93
    const auto& areaResult = areaTool.result();
    EXPECT_NEAR(areaResult.area, M_PI * 100 * 100, 1.0);
}

TEST_F(MeasurementTest, MeasureArea_Ellipse) {
    MeasureAreaTool areaTool;

    areaTool.setMeasureMode(MeasureAreaTool::Ellipse);
    areaTool.setEllipse(QPointF(320, 240), 100.0, 50.0);  // 长轴100，短轴50

    ToolResult result;
    bool success = areaTool.process(testImage, result);

    EXPECT_TRUE(success);

    // 椭圆面积 = π * a * b = π * 100 * 50 = 15707.96
    const auto& areaResult = areaTool.result();
    EXPECT_NEAR(areaResult.area, M_PI * 100 * 50, 1.0);
}

TEST_F(MeasurementTest, MeasureArea_Contour) {
    MeasureAreaTool areaTool;

    areaTool.setMeasureMode(MeasureAreaTool::Contour);
    areaTool.setThreshold(128);
    areaTool.setMinArea(1000);  // 过滤小轮廓

    ToolResult result;
    bool success = areaTool.process(contourImage, result);

    EXPECT_TRUE(success);

    // 应该检测到轮廓
    const auto& allResults = areaTool.allResults();
    EXPECT_GT(allResults.size(), 0);

    // 验证矩形轮廓面积 (200x200 = 40000)
    bool foundRectangle = false;
    for (const auto& r : allResults) {
        if (std::abs(r.area - 40000.0) < 100.0) {
            foundRectangle = true;
            break;
        }
    }
    EXPECT_TRUE(foundRectangle);
}

TEST_F(MeasurementTest, MeasureArea_PixelToMm) {
    MeasureAreaTool areaTool;

    areaTool.setMeasureMode(MeasureAreaTool::Circle);
    areaTool.setCircle(QPointF(320, 240), 100.0);
    areaTool.setPixelToMm(0.1);  // 1像素 = 0.1毫米

    ToolResult result;
    bool success = areaTool.process(testImage, result);

    EXPECT_TRUE(success);

    const auto& areaResult = areaTool.result();
    double expectedAreaPixels = M_PI * 100 * 100;
    double expectedAreaMm2 = expectedAreaPixels * 0.1 * 0.1;  // 面积换算需要平方

    EXPECT_NEAR(areaResult.area, expectedAreaPixels, 1.0);
    EXPECT_NEAR(areaResult.areaMm2, expectedAreaMm2, 0.1);
}

// ========== 参数化测试：不同距离测量模式 ==========

class DistanceModeTest : public ::testing::TestWithParam<MeasureDistanceTool::MeasureMode> {
protected:
    void SetUp() override {
        testImage = ImageMemoryPool::instance().allocate(640, 480, CV_8UC1);
        testImage->mat().setTo(cv::Scalar(0));
    }

    ImageData::Ptr testImage;
};

TEST_P(DistanceModeTest, AllDistanceModes) {
    MeasureDistanceTool::MeasureMode mode = GetParam();

    MeasureDistanceTool distanceTool;
    distanceTool.setMeasureMode(mode);

    // 根据模式设置参数（确保点不在线上，产生非零距离）
    distanceTool.setPoint1(QPointF(100, 150));  // 不在line1上
    distanceTool.setPoint2(QPointF(400, 100));
    distanceTool.setLine1(QLineF(QPointF(100, 100), QPointF(500, 100)));
    distanceTool.setLine2(QLineF(QPointF(100, 200), QPointF(500, 200)));
    distanceTool.setCircle1(QPointF(300, 300), 50);
    distanceTool.setCircle2(QPointF(500, 300), 30);

    ToolResult result;
    bool success = distanceTool.process(testImage, result);

    EXPECT_TRUE(success);
    EXPECT_TRUE(result.success);

    const auto& distResult = distanceTool.result();
    EXPECT_TRUE(distResult.valid);
    EXPECT_GT(distResult.distance, 0);
}

INSTANTIATE_TEST_SUITE_P(
    AllModes,
    DistanceModeTest,
    ::testing::Values(
        MeasureDistanceTool::PointToPoint,
        MeasureDistanceTool::PointToLine,
        MeasureDistanceTool::LineToLine,
        MeasureDistanceTool::CircleToPoint,
        MeasureDistanceTool::CircleToLine,
        MeasureDistanceTool::CircleToCircle
    )
);

// ========== 参数化测试：不同角度测量模式 ==========

class AngleModeTest : public ::testing::TestWithParam<MeasureAngleTool::MeasureMode> {
protected:
    void SetUp() override {
        testImage = ImageMemoryPool::instance().allocate(640, 480, CV_8UC1);
        testImage->mat().setTo(cv::Scalar(0));
    }

    ImageData::Ptr testImage;
};

TEST_P(AngleModeTest, AllAngleModes) {
    MeasureAngleTool::MeasureMode mode = GetParam();

    MeasureAngleTool angleTool;
    angleTool.setMeasureMode(mode);

    // 设置参数
    angleTool.setLine1(QLineF(QPointF(100, 100), QPointF(400, 100)));
    angleTool.setLine2(QLineF(QPointF(250, 50), QPointF(250, 350)));
    angleTool.setVertex(QPointF(300, 200));
    angleTool.setPoint1(QPointF(100, 200));
    angleTool.setPoint2(QPointF(300, 400));

    ToolResult result;
    bool success = angleTool.process(testImage, result);

    EXPECT_TRUE(success);
    EXPECT_TRUE(result.success);

    const auto& angleResult = angleTool.result();
    EXPECT_TRUE(angleResult.valid);
    EXPECT_GE(angleResult.angle, 0);
    EXPECT_LE(angleResult.angle, 180);
}

INSTANTIATE_TEST_SUITE_P(
    AllModes,
    AngleModeTest,
    ::testing::Values(
        MeasureAngleTool::LineToLine,
        MeasureAngleTool::ThreePoints,
        MeasureAngleTool::LineToHorizontal,
        MeasureAngleTool::LineToVertical
    )
);

// ========== 参数化测试：不同面积测量模式 ==========

class AreaModeTest : public ::testing::TestWithParam<MeasureAreaTool::MeasureMode> {
protected:
    void SetUp() override {
        testImage = ImageMemoryPool::instance().allocate(640, 480, CV_8UC1);
        testImage->mat().setTo(cv::Scalar(0));
        cv::rectangle(testImage->mat(), cv::Point(200, 150), cv::Point(400, 350), cv::Scalar(255), -1);
    }

    ImageData::Ptr testImage;
};

TEST_P(AreaModeTest, AllAreaModes) {
    MeasureAreaTool::MeasureMode mode = GetParam();

    MeasureAreaTool areaTool;
    areaTool.setMeasureMode(mode);

    // 设置参数
    QPolygonF polygon;
    polygon << QPointF(0, 0) << QPointF(100, 0) << QPointF(100, 100) << QPointF(0, 100);
    areaTool.setPolygon(polygon);
    areaTool.setCircle(QPointF(320, 240), 50);
    areaTool.setEllipse(QPointF(320, 240), 100, 50);
    areaTool.setThreshold(128);

    ToolResult result;
    bool success = areaTool.process(testImage, result);

    EXPECT_TRUE(success);
    EXPECT_TRUE(result.success);

    const auto& areaResult = areaTool.result();
    EXPECT_TRUE(areaResult.valid);
    EXPECT_GT(areaResult.area, 0);
}

INSTANTIATE_TEST_SUITE_P(
    AllModes,
    AreaModeTest,
    ::testing::Values(
        MeasureAreaTool::Polygon,
        MeasureAreaTool::Contour,
        MeasureAreaTool::Circle,
        MeasureAreaTool::Ellipse
    )
);

// ========== 主函数 ==========

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::FLAGS_gtest_color = "yes";
    ::testing::FLAGS_gtest_print_time = true;

    return RUN_ALL_TESTS();
}
