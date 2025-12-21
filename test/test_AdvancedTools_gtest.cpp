/**
 * @file test_AdvancedTools_gtest.cpp
 * @brief 高级图像处理工具测试（Google Test版本）
 * @details 测试ColorConvert, ROI, SaveImage工具
 * @author VisionForge Team
 * @date 2025-12-20
 */

#include <gtest/gtest.h>
#include "algorithm/ColorConvertTool.h"
#include "algorithm/ROITool.h"
#include "algorithm/SaveImageTool.h"
#include "base/ImageMemoryPool.h"
#include "base/ImageData.h"
#include <opencv2/opencv.hpp>
#include <QDir>
#include <QFile>

using namespace VisionForge;
using namespace VisionForge::Algorithm;
using namespace VisionForge::Base;

// ========== 测试夹具 ==========

class AdvancedToolsTest : public ::testing::Test {
protected:
    void SetUp() override {
        // 创建测试图像 - BGR彩色图像
        testImageColor = ImageMemoryPool::instance().allocate(640, 480, CV_8UC3);
        testImageColor->mat().setTo(cv::Scalar(255, 128, 64)); // BGR

        // 创建带图案的彩色图像
        testImagePattern = ImageMemoryPool::instance().allocate(640, 480, CV_8UC3);
        testImagePattern->mat().setTo(cv::Scalar(100, 100, 100));
        cv::rectangle(testImagePattern->mat(), cv::Point(100, 100),
                     cv::Point(300, 300), cv::Scalar(0, 0, 255), -1); // 红色矩形
        cv::circle(testImagePattern->mat(), cv::Point(450, 240),
                  80, cv::Scalar(0, 255, 0), -1); // 绿色圆形

        // 创建灰度图像
        testImageGray = ImageMemoryPool::instance().allocate(640, 480, CV_8UC1);
        testImageGray->mat().setTo(cv::Scalar(128));
    }

    void TearDown() override {
        // ScopedImagePtr自动归还内存池
    }

    ImageData::Ptr testImageColor;
    ImageData::Ptr testImagePattern;
    ImageData::Ptr testImageGray;
};

// ========== ColorConvertTool测试 ==========

TEST_F(AdvancedToolsTest, ColorConvert_BGR2RGB) {
    ColorConvertTool colorTool;

    colorTool.setSourceSpace(ColorConvertTool::BGR);
    colorTool.setTargetSpace(ColorConvertTool::RGB);

    ToolResult result;
    bool success = colorTool.process(testImageColor, result);

    EXPECT_TRUE(success);
    EXPECT_TRUE(result.success);
    ASSERT_NE(result.outputImage, nullptr);
    EXPECT_EQ(result.outputImage->channels(), 3);
}

TEST_F(AdvancedToolsTest, ColorConvert_BGR2HSV) {
    ColorConvertTool colorTool;

    colorTool.setSourceSpace(ColorConvertTool::BGR);
    colorTool.setTargetSpace(ColorConvertTool::HSV);

    ToolResult result;
    bool success = colorTool.process(testImageColor, result);

    EXPECT_TRUE(success);
    EXPECT_TRUE(result.success);
    ASSERT_NE(result.outputImage, nullptr);
    EXPECT_EQ(result.outputImage->channels(), 3);
}

TEST_F(AdvancedToolsTest, ColorConvert_BGR2Gray) {
    ColorConvertTool colorTool;

    colorTool.setSourceSpace(ColorConvertTool::BGR);
    colorTool.setTargetSpace(ColorConvertTool::Grayscale);

    ToolResult result;
    bool success = colorTool.process(testImageColor, result);

    EXPECT_TRUE(success);
    EXPECT_TRUE(result.success);
    ASSERT_NE(result.outputImage, nullptr);
    EXPECT_EQ(result.outputImage->channels(), 1);
}

TEST_F(AdvancedToolsTest, ColorConvert_ExtractChannel) {
    ColorConvertTool colorTool;

    colorTool.setSourceSpace(ColorConvertTool::BGR);
    colorTool.setTargetSpace(ColorConvertTool::HSV);
    colorTool.setExtractChannel(true);
    colorTool.setChannelIndex(0); // H通道

    ToolResult result;
    bool success = colorTool.process(testImageColor, result);

    EXPECT_TRUE(success);
    EXPECT_TRUE(result.success);
    ASSERT_NE(result.outputImage, nullptr);
    EXPECT_EQ(result.outputImage->channels(), 1); // 提取单通道
}

TEST_F(AdvancedToolsTest, ColorConvert_BGR2Lab) {
    ColorConvertTool colorTool;

    colorTool.setSourceSpace(ColorConvertTool::BGR);
    colorTool.setTargetSpace(ColorConvertTool::Lab);

    ToolResult result;
    bool success = colorTool.process(testImageColor, result);

    EXPECT_TRUE(success);
    EXPECT_TRUE(result.success);
    ASSERT_NE(result.outputImage, nullptr);
    EXPECT_EQ(result.outputImage->channels(), 3);
}

TEST_F(AdvancedToolsTest, ColorConvert_NullInput) {
    ColorConvertTool colorTool;

    colorTool.setSourceSpace(ColorConvertTool::BGR);
    colorTool.setTargetSpace(ColorConvertTool::HSV);

    ToolResult result;
    bool success = colorTool.process(nullptr, result);

    EXPECT_FALSE(success);
}

// ========== ROITool测试 ==========

TEST_F(AdvancedToolsTest, ROI_RectangleROI) {
    ROITool roiTool;

    // 添加矩形ROI
    roiTool.addRectROI(QRectF(100, 100, 200, 200), "TestRect");

    EXPECT_EQ(roiTool.roiCollection().count(), 1);

    ToolResult result;
    bool success = roiTool.process(testImagePattern, result);

    EXPECT_TRUE(success);
    EXPECT_TRUE(result.success);
    ASSERT_NE(result.outputImage, nullptr);
}

TEST_F(AdvancedToolsTest, ROI_CircleROI) {
    ROITool roiTool;

    // 添加圆形ROI
    roiTool.addCircleROI(QPointF(320, 240), 100, "TestCircle");

    EXPECT_EQ(roiTool.roiCollection().count(), 1);

    ToolResult result;
    bool success = roiTool.process(testImagePattern, result);

    EXPECT_TRUE(success);
    EXPECT_TRUE(result.success);
}

TEST_F(AdvancedToolsTest, ROI_EllipseROI) {
    ROITool roiTool;

    // 添加椭圆ROI
    roiTool.addEllipseROI(QPointF(320, 240), 150, 100, 45, "TestEllipse");

    EXPECT_EQ(roiTool.roiCollection().count(), 1);

    ToolResult result;
    bool success = roiTool.process(testImagePattern, result);

    EXPECT_TRUE(success);
}

TEST_F(AdvancedToolsTest, ROI_PolygonROI) {
    ROITool roiTool;

    // 添加多边形ROI（三角形）
    QPolygonF polygon;
    polygon << QPointF(320, 100) << QPointF(150, 380) << QPointF(490, 380);
    roiTool.addPolygonROI(polygon, "TestTriangle");

    EXPECT_EQ(roiTool.roiCollection().count(), 1);

    ToolResult result;
    bool success = roiTool.process(testImagePattern, result);

    EXPECT_TRUE(success);
}

TEST_F(AdvancedToolsTest, ROI_RotatedRectROI) {
    ROITool roiTool;

    // 添加旋转矩形ROI
    roiTool.addRotatedRectROI(QPointF(320, 240), 200, 100, 30, "TestRotatedRect");

    EXPECT_EQ(roiTool.roiCollection().count(), 1);

    ToolResult result;
    bool success = roiTool.process(testImagePattern, result);

    EXPECT_TRUE(success);
}

TEST_F(AdvancedToolsTest, ROI_MultipleROIs_Union) {
    ROITool roiTool;

    roiTool.setCombineMode(ROITool::Union);
    roiTool.addRectROI(QRectF(100, 100, 150, 150), "Rect1");
    roiTool.addCircleROI(QPointF(400, 300), 80, "Circle1");

    EXPECT_EQ(roiTool.roiCollection().count(), 2);

    ToolResult result;
    bool success = roiTool.process(testImagePattern, result);

    EXPECT_TRUE(success);
}

TEST_F(AdvancedToolsTest, ROI_MultipleROIs_Intersection) {
    ROITool roiTool;

    roiTool.setCombineMode(ROITool::Intersection);
    roiTool.addRectROI(QRectF(100, 100, 300, 300), "Rect1");
    roiTool.addCircleROI(QPointF(250, 250), 120, "Circle1");

    EXPECT_EQ(roiTool.roiCollection().count(), 2);

    ToolResult result;
    bool success = roiTool.process(testImagePattern, result);

    EXPECT_TRUE(success);
}

TEST_F(AdvancedToolsTest, ROI_InvertMask) {
    ROITool roiTool;

    roiTool.addRectROI(QRectF(200, 150, 240, 180), "CenterRect");
    roiTool.setInvertMask(true);

    ToolResult result;
    bool success = roiTool.process(testImagePattern, result);

    EXPECT_TRUE(success);
    EXPECT_TRUE(result.success);
}

TEST_F(AdvancedToolsTest, ROI_ClearROIs) {
    ROITool roiTool;

    roiTool.addRectROI(QRectF(100, 100, 200, 200));
    roiTool.addCircleROI(QPointF(320, 240), 100);
    EXPECT_EQ(roiTool.roiCollection().count(), 2);

    roiTool.clearROIs();
    EXPECT_EQ(roiTool.roiCollection().count(), 0);
}

TEST_F(AdvancedToolsTest, ROI_RemoveROI) {
    ROITool roiTool;

    roiTool.addRectROI(QRectF(100, 100, 200, 200));
    roiTool.addCircleROI(QPointF(320, 240), 100);
    EXPECT_EQ(roiTool.roiCollection().count(), 2);

    roiTool.removeROI(0);
    EXPECT_EQ(roiTool.roiCollection().count(), 1);
}

// ========== SaveImageTool测试 ==========

TEST_F(AdvancedToolsTest, SaveImage_BasicSave) {
    SaveImageTool saveTool;

    // 设置临时保存目录
    QString tempDir = QDir::temp().absoluteFilePath("VisionForgeTest");
    saveTool.setSaveDirectory(tempDir);
    saveTool.setImageFormat(SaveImageTool::PNG);
    saveTool.setPrefix("test");

    ToolResult result;
    bool success = saveTool.process(testImageColor, result);

    EXPECT_TRUE(success);
    EXPECT_TRUE(result.success);

    // 验证文件已保存
    QString savedPath = saveTool.lastSavedPath();
    EXPECT_FALSE(savedPath.isEmpty());
    EXPECT_TRUE(QFile::exists(savedPath));

    // 清理
    QFile::remove(savedPath);
    QDir().rmdir(tempDir);
}

TEST_F(AdvancedToolsTest, SaveImage_JPEG_Quality) {
    SaveImageTool saveTool;

    QString tempDir = QDir::temp().absoluteFilePath("VisionForgeTest");
    saveTool.setSaveDirectory(tempDir);
    saveTool.setImageFormat(SaveImageTool::JPEG);
    saveTool.setJpegQuality(85);
    saveTool.setPrefix("jpeg_test");

    ToolResult result;
    bool success = saveTool.process(testImageColor, result);

    EXPECT_TRUE(success);
    EXPECT_EQ(saveTool.jpegQuality(), 85);

    // 清理
    if (!saveTool.lastSavedPath().isEmpty()) {
        QFile::remove(saveTool.lastSavedPath());
    }
    QDir().rmdir(tempDir);
}

TEST_F(AdvancedToolsTest, SaveImage_SequentialNaming) {
    SaveImageTool saveTool;

    QString tempDir = QDir::temp().absoluteFilePath("VisionForgeTest");
    saveTool.setSaveDirectory(tempDir);
    saveTool.setImageFormat(SaveImageTool::PNG);
    saveTool.setNamingMode(SaveImageTool::Sequential);
    saveTool.setPrefix("seq");

    // 保存多张图像
    ToolResult result;
    saveTool.process(testImageColor, result);
    QString path1 = saveTool.lastSavedPath();

    saveTool.process(testImageColor, result);
    QString path2 = saveTool.lastSavedPath();

    EXPECT_NE(path1, path2); // 文件名应该不同
    EXPECT_EQ(saveTool.saveCount(), 2);

    // 清理
    QFile::remove(path1);
    QFile::remove(path2);
    QDir().rmdir(tempDir);
}

TEST_F(AdvancedToolsTest, SaveImage_ConditionalSave_OnlyOK) {
    SaveImageTool saveTool;

    QString tempDir = QDir::temp().absoluteFilePath("VisionForgeTest");
    saveTool.setSaveDirectory(tempDir);
    saveTool.setImageFormat(SaveImageTool::PNG);
    saveTool.setSaveCondition(SaveImageTool::OnlyOK);
    saveTool.setJudgeResult(true); // OK

    ToolResult result;
    bool success = saveTool.process(testImageColor, result);

    EXPECT_TRUE(success);
    EXPECT_FALSE(saveTool.lastSavedPath().isEmpty());

    QString okPath = saveTool.lastSavedPath();

    // 测试NG情况
    saveTool.setJudgeResult(false); // NG
    saveTool.process(testImageColor, result);

    // NG时不应该保存，路径不变
    EXPECT_EQ(saveTool.lastSavedPath(), okPath);

    // 清理
    QFile::remove(okPath);
    QDir().rmdir(tempDir);
}

TEST_F(AdvancedToolsTest, SaveImage_ConditionalSave_OnlyNG) {
    SaveImageTool saveTool;

    QString tempDir = QDir::temp().absoluteFilePath("VisionForgeTest");
    saveTool.setSaveDirectory(tempDir);
    saveTool.setImageFormat(SaveImageTool::PNG);
    saveTool.setSaveCondition(SaveImageTool::OnlyNG);

    // OK情况不保存
    saveTool.setJudgeResult(true);
    ToolResult result;
    saveTool.process(testImageColor, result);
    EXPECT_TRUE(saveTool.lastSavedPath().isEmpty());

    // NG情况保存
    saveTool.setJudgeResult(false);
    saveTool.process(testImageColor, result);
    EXPECT_FALSE(saveTool.lastSavedPath().isEmpty());

    // 清理
    if (!saveTool.lastSavedPath().isEmpty()) {
        QFile::remove(saveTool.lastSavedPath());
    }
    QDir().rmdir(tempDir);
}

TEST_F(AdvancedToolsTest, SaveImage_ResetCounter) {
    SaveImageTool saveTool;

    QString tempDir = QDir::temp().absoluteFilePath("VisionForgeTest");
    saveTool.setSaveDirectory(tempDir);
    saveTool.setImageFormat(SaveImageTool::PNG);

    ToolResult result;
    saveTool.process(testImageColor, result);
    saveTool.process(testImageColor, result);
    saveTool.process(testImageColor, result);

    EXPECT_EQ(saveTool.saveCount(), 3);

    saveTool.resetCounter();
    EXPECT_EQ(saveTool.saveCount(), 0);

    // 清理（删除所有保存的文件）
    QDir dir(tempDir);
    dir.removeRecursively();
}

// ========== 参数化测试：不同颜色空间转换 ==========

class ColorSpaceTest : public ::testing::TestWithParam<ColorConvertTool::ColorSpace> {
protected:
    void SetUp() override {
        testImage = ImageMemoryPool::instance().allocate(640, 480, CV_8UC3);
        testImage->mat().setTo(cv::Scalar(255, 128, 64));
    }

    ImageData::Ptr testImage;
};

TEST_P(ColorSpaceTest, AllColorSpaceConversions) {
    ColorConvertTool::ColorSpace targetSpace = GetParam();

    // 跳过灰度作为源（只有1通道）
    if (targetSpace == ColorConvertTool::Grayscale) {
        return;
    }

    ColorConvertTool colorTool;
    colorTool.setSourceSpace(ColorConvertTool::BGR);
    colorTool.setTargetSpace(targetSpace);

    ToolResult result;
    bool success = colorTool.process(testImage, result);

    EXPECT_TRUE(success);
    EXPECT_TRUE(result.success);
    ASSERT_NE(result.outputImage, nullptr);
}

INSTANTIATE_TEST_SUITE_P(
    AllColorSpaces,
    ColorSpaceTest,
    ::testing::Values(
        ColorConvertTool::RGB,
        ColorConvertTool::HSV,
        ColorConvertTool::HLS,
        ColorConvertTool::Lab,
        ColorConvertTool::YCrCb,
        ColorConvertTool::XYZ,
        ColorConvertTool::Grayscale
    )
);

// ========== 参数化测试：不同图像格式保存 ==========

class ImageFormatTest : public ::testing::TestWithParam<SaveImageTool::ImageFormat> {
protected:
    void SetUp() override {
        testImage = ImageMemoryPool::instance().allocate(640, 480, CV_8UC3);
        testImage->mat().setTo(cv::Scalar(255, 128, 64));
        tempDir = QDir::temp().absoluteFilePath("VisionForgeTest_Format");
    }

    void TearDown() override {
        // 清理临时目录
        QDir dir(tempDir);
        dir.removeRecursively();
    }

    ImageData::Ptr testImage;
    QString tempDir;
};

TEST_P(ImageFormatTest, AllImageFormats) {
    SaveImageTool::ImageFormat format = GetParam();

    SaveImageTool saveTool;
    saveTool.setSaveDirectory(tempDir);
    saveTool.setImageFormat(format);
    saveTool.setPrefix("format_test");

    ToolResult result;
    bool success = saveTool.process(testImage, result);

    EXPECT_TRUE(success);
    EXPECT_TRUE(result.success);

    QString savedPath = saveTool.lastSavedPath();
    EXPECT_FALSE(savedPath.isEmpty());
    EXPECT_TRUE(QFile::exists(savedPath));
}

INSTANTIATE_TEST_SUITE_P(
    AllFormats,
    ImageFormatTest,
    ::testing::Values(
        SaveImageTool::PNG,
        SaveImageTool::JPEG,
        SaveImageTool::BMP,
        SaveImageTool::TIFF
    )
);

// ========== 性能测试（默认禁用）==========

TEST_F(AdvancedToolsTest, DISABLED_Performance_ColorConversion) {
    ColorConvertTool colorTool;
    colorTool.setSourceSpace(ColorConvertTool::BGR);
    colorTool.setTargetSpace(ColorConvertTool::HSV);

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < 100; ++i) {
        ToolResult result;
        colorTool.process(testImageColor, result);
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "100次颜色转换耗时: " << duration.count() << "ms" << std::endl;
    EXPECT_LT(duration.count(), 1000); // 期望<1秒
}

TEST_F(AdvancedToolsTest, DISABLED_Performance_ROIMasking) {
    ROITool roiTool;
    roiTool.addRectROI(QRectF(100, 100, 400, 300));

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < 100; ++i) {
        ToolResult result;
        roiTool.process(testImagePattern, result);
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "100次ROI处理耗时: " << duration.count() << "ms" << std::endl;
    EXPECT_LT(duration.count(), 500); // 期望<0.5秒
}

// ========== 主函数 ==========

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);

    // 设置测试输出格式
    ::testing::FLAGS_gtest_color = "yes";
    ::testing::FLAGS_gtest_print_time = true;

    return RUN_ALL_TESTS();
}
