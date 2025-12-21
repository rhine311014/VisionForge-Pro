# VisionForge Pro 算法测试用例总结

**文档版本**: 1.0
**创建日期**: 2025-12-20
**目标**: 记录已完成的算法测试用例及使用指南

---

## 📋 概述

本文档总结了为 VisionForge Pro v1.2.0 编写的 Google Test 单元测试套件。这些测试套件覆盖了项目中的核心图像处理和机器视觉算法，确保代码质量和7x24工业级稳定性。

---

## ✅ 已完成的测试套件

### 1. 基础数据结构测试

**文件**: `test/test_ImageData_gtest.cpp`

**测试类**:
- `ImageDataTest` - ImageData类基础功能测试
- `ImageDataTypeTest` - 参数化测试（6种图像类型）

**测试覆盖** (18个测试用例):
- ✅ 构造函数测试 (Construction, EmptyImage)
- ✅ 拷贝和克隆测试 (CopyConstructor, Clone)
- ✅ ROI操作测试 (SetROI, ClearROI, InvalidROI)
- ✅ 时间戳测试 (Timestamp, DefaultTimestamp)
- ✅ 类型转换测试 (ConvertToGray, ConvertTo16Bit)
- ✅ 元数据测试 (SetGetMetadata, HasMetadata, RemoveMetadata)
- ✅ 参数化测试 (6种图像类型: CV_8UC1, CV_8UC3, CV_16UC1, CV_16UC3, CV_32FC1, CV_32FC3)
- ✅ 性能测试 (DISABLED_PerformanceLargeImage)

**关键模式**:
```cpp
// Test Fixture
class ImageDataTest : public ::testing::Test {
protected:
    void SetUp() override {
        testMat = cv::Mat(480, 640, CV_8UC3, cv::Scalar(255, 0, 0));
    }
    cv::Mat testMat;
};

// Parameterized Test
INSTANTIATE_TEST_SUITE_P(
    AllImageTypes,
    ImageDataTypeTest,
    ::testing::Values(CV_8UC1, CV_8UC3, CV_16UC1, CV_16UC3, CV_32FC1, CV_32FC3)
);
```

---

### 2. 基础图像处理测试

**文件**: `test/test_BasicImageProcessing_gtest.cpp`

**测试类**:
- `BasicImageProcessingTest` - 基础图像处理算法测试
- `BlurKernelSizeTest` - 参数化测试（5种核大小）
- `ThresholdTypeTest` - 参数化测试（5种阈值类型）

**测试覆盖** (12个测试用例 + 参数化测试):

**GrayTool (灰度转换)**:
- ✅ BGR到Gray转换 (GrayTool_BGR2Gray)
- ✅ 已是灰度图处理 (GrayTool_AlreadyGray)
- ✅ 空输入处理 (GrayTool_NullInput)

**ThresholdTool (二值化)**:
- ✅ 二值阈值化 (ThresholdTool_BinaryThreshold)
- ✅ Otsu自动阈值 (ThresholdTool_OtsuThreshold)
- ✅ 参数化测试 (5种阈值类型: BINARY, BINARY_INV, TRUNC, TOZERO, TOZERO_INV)

**BlurTool (模糊)**:
- ✅ 高斯模糊 (BlurTool_GaussianBlur)
- ✅ 中值滤波 (BlurTool_MedianBlur)
- ✅ 无效核大小处理 (BlurTool_InvalidKernelSize)
- ✅ 参数化测试 (5种核大小: 3, 5, 7, 9, 11)

**边界条件测试**:
- ✅ 极小图像处理 (EdgeCase_TinyImage: 1x1)
- ✅ 大图像处理 (EdgeCase_LargeImage: 4096x4096)

**性能测试**:
- ✅ 灰度转换性能 (DISABLED_Performance_GrayConversion: 100次1920x1080)

---

### 3. 边缘检测和形态学处理测试

**文件**: `test/test_EdgeAndMorphology_gtest.cpp`

**测试类**:
- `EdgeAndMorphologyTest` - 边缘检测和形态学测试
- `EdgeTypeTest` - 参数化测试（4种边缘检测算法）
- `MorphTypeTest` - 参数化测试（7种形态学操作）
- `KernelShapeTest` - 参数化测试（3种核形状）

**测试覆盖** (22个测试用例 + 参数化测试):

**EdgeTool (边缘检测)**:
- ✅ Canny边缘检测 (EdgeTool_Canny)
- ✅ Sobel边缘检测 (EdgeTool_Sobel)
- ✅ Laplacian边缘检测 (EdgeTool_Laplacian)
- ✅ Scharr边缘检测 (EdgeTool_Scharr)
- ✅ 空输入处理 (EdgeTool_NullInput)
- ✅ 参数化测试 (4种算法: Canny, Sobel, Laplacian, Scharr)

**FindEdgeTool (边缘查找)**:
- ✅ 线段搜索 (FindEdgeTool_LineSearch)
- ✅ 第一边缘查找 (FindEdgeTool_FirstEdge)
- ✅ 矩形区域搜索 (FindEdgeTool_RectSearch)

**MorphologyTool (形态学处理)**:
- ✅ 腐蚀操作 (MorphologyTool_Erode)
- ✅ 膨胀操作 (MorphologyTool_Dilate)
- ✅ 开运算 (MorphologyTool_Open)
- ✅ 闭运算 (MorphologyTool_Close)
- ✅ 形态学梯度 (MorphologyTool_Gradient)
- ✅ 顶帽变换 (MorphologyTool_TopHat)
- ✅ 黑帽变换 (MorphologyTool_BlackHat)
- ✅ 参数化测试 (7种操作: Erode, Dilate, Open, Close, Gradient, TopHat, BlackHat)
- ✅ 参数化测试 (3种核形状: Rect, Cross, Ellipse)

**性能测试**:
- ✅ Canny边缘检测性能 (DISABLED_Performance_CannyEdgeDetection: 100次1920x1080)

---

### 4. 形状检测测试

**文件**: `test/test_ShapeDetection_gtest.cpp`

**测试类**:
- `ShapeDetectionTest` - 形状检测算法测试
- `CircleMethodTest` - 参数化测试（2种圆检测方法）
- `BlobConnectivityTest` - 参数化测试（2种连通性）
- `BlobSortTest` - 参数化测试（5种排序方式）

**测试覆盖** (18个测试用例 + 参数化测试):

**CircleTool (圆检测)**:
- ✅ 霍夫圆检测 (CircleTool_HoughCircle)
- ✅ 轮廓拟合 (CircleTool_ContourFit)
- ✅ 空输入处理 (CircleTool_NullInput)
- ✅ 半径范围过滤 (CircleTool_RadiusRange)
- ✅ 参数化测试 (2种方法: HoughCircle, ContourFit)

**LineTool (线检测)**:
- ✅ 概率霍夫变换 (LineTool_HoughLinesP)
- ✅ 标准霍夫变换 (LineTool_HoughLines)
- ✅ 最小长度过滤 (LineTool_MinLength)
- ✅ 空输入处理 (LineTool_NullInput)

**BlobTool (Blob分析)**:
- ✅ 基本检测 (BlobTool_BasicDetection)
- ✅ 面积过滤 (BlobTool_AreaFilter)
- ✅ 圆度过滤 (BlobTool_CircularityFilter)
- ✅ 特征计算 (BlobTool_FeatureCalculation)
- ✅ 4连通vs8连通 (BlobTool_Connect4vs8)
- ✅ 空输入处理 (BlobTool_NullInput)
- ✅ 参数化测试 (2种连通性: Connect4, Connect8)
- ✅ 参数化测试 (5种排序: NoSort, SortByArea, SortByX, SortByY, SortByCircularity)

**性能测试**:
- ✅ 圆检测性能 (DISABLED_Performance_CircleDetection: 100次1920x1080)

---

### 5. 测量算法测试

**文件**: `test/test_Measurement_gtest.cpp`

**测试类**:
- `MeasurementTest` - 测量算法测试
- `DistanceModeTest` - 参数化测试（6种距离测量模式）
- `AngleModeTest` - 参数化测试（4种角度测量模式）
- `AreaModeTest` - 参数化测试（4种面积测量模式）

**测试覆盖** (25个测试用例 + 参数化测试):

**MeasureDistanceTool (距离测量)**:
- ✅ 点到点距离 (MeasureDistance_PointToPoint)
- ✅ 点到点对角线距离 (MeasureDistance_PointToPoint_Diagonal)
- ✅ 点到线距离 (MeasureDistance_PointToLine)
- ✅ 平行线距离 (MeasureDistance_LineToLine_Parallel)
- ✅ 像素到毫米换算 (MeasureDistance_PixelToMm)
- ✅ 空输入处理 (MeasureDistance_NullInput)
- ✅ 参数化测试 (6种模式: PointToPoint, PointToLine, LineToLine, CircleToPoint, CircleToLine, CircleToCircle)

**MeasureAngleTool (角度测量)**:
- ✅ 线到线垂直 (MeasureAngle_LineToLine_Perpendicular)
- ✅ 线到线平行 (MeasureAngle_LineToLine_Parallel)
- ✅ 三点角度 (MeasureAngle_ThreePoints)
- ✅ 线到水平 (MeasureAngle_LineToHorizontal)
- ✅ 线到垂直 (MeasureAngle_LineToVertical)
- ✅ 45度角测试 (MeasureAngle_45Degrees)
- ✅ 参数化测试 (4种模式: LineToLine, ThreePoints, LineToHorizontal, LineToVertical)

**MeasureAreaTool (面积测量)**:
- ✅ 多边形三角形 (MeasureArea_Polygon_Triangle)
- ✅ 多边形矩形 (MeasureArea_Polygon_Rectangle)
- ✅ 圆形面积 (MeasureArea_Circle)
- ✅ 椭圆面积 (MeasureArea_Ellipse)
- ✅ 轮廓面积 (MeasureArea_Contour)
- ✅ 像素到毫米换算 (MeasureArea_PixelToMm)
- ✅ 参数化测试 (4种模式: Polygon, Contour, Circle, Ellipse)

---

## 📊 测试覆盖统计

| 测试套件 | 测试类数量 | 测试用例数量 | 参数化测试 | 覆盖的工具类 |
|---------|-----------|------------|-----------|------------|
| **test_ImageData_gtest** | 2 | 18 | 6种图像类型 | ImageData |
| **test_BasicImageProcessing_gtest** | 3 | 12 | 5种核大小, 5种阈值类型 | GrayTool, ThresholdTool, BlurTool |
| **test_EdgeAndMorphology_gtest** | 4 | 22 | 4种边缘算法, 7种形态学操作, 3种核形状 | EdgeTool, FindEdgeTool, MorphologyTool |
| **test_ShapeDetection_gtest** | 4 | 18 | 2种圆检测方法, 2种连通性, 5种排序方式 | CircleTool, LineTool, BlobTool |
| **test_Measurement_gtest** | 4 | 25 | 6种距离模式, 4种角度模式, 4种面积模式 | MeasureDistanceTool, MeasureAngleTool, MeasureAreaTool |
| **合计** | **17** | **95** | **多维度覆盖** | **12个算法工具类** |

---

## 🎯 测试模式和最佳实践

### 1. Test Fixture模式

所有测试套件都使用Test Fixture来管理测试设置和清理：

```cpp
class BasicImageProcessingTest : public ::testing::Test {
protected:
    void SetUp() override {
        // 使用ImageMemoryPool进行RAII管理
        testImageColor = ImageMemoryPool::instance().allocate(640, 480, CV_8UC3);
        testImageGray = ImageMemoryPool::instance().allocate(640, 480, CV_8UC1);
    }

    void TearDown() override {
        // ScopedImagePtr自动归还，无需手动释放
    }

    ImageData::Ptr testImageColor;
    ImageData::Ptr testImageGray;
};
```

**优势**:
- 自动资源管理（RAII）
- 测试隔离（每个测试独立）
- 代码复用（共享测试数据）

---

### 2. 参数化测试模式

用于测试相同逻辑的多种参数组合：

```cpp
class EdgeTypeTest : public ::testing::TestWithParam<EdgeTool::EdgeType> {
protected:
    void SetUp() override {
        testImage = ImageMemoryPool::instance().allocate(640, 480, CV_8UC1);
        // 创建测试图像
    }
    ImageData::Ptr testImage;
};

TEST_P(EdgeTypeTest, AllEdgeAlgorithms) {
    EdgeTool::EdgeType edgeType = GetParam();

    EdgeTool edgeTool;
    edgeTool.setEdgeType(edgeType);

    ToolResult result;
    bool success = edgeTool.process(testImage, result);

    EXPECT_TRUE(success);
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
```

**覆盖场景**:
- 不同图像类型 (CV_8UC1, CV_8UC3等)
- 不同算法类型 (Canny, Sobel, HoughCircle等)
- 不同核大小 (3, 5, 7, 9, 11)
- 不同阈值类型 (BINARY, TRUNC等)

---

### 3. 断言策略

**致命断言** (ASSERT_*): 失败后停止当前测试
```cpp
ASSERT_NE(result.outputImage, nullptr);  // 输出图像必须存在
```

**非致命断言** (EXPECT_*): 失败后继续执行
```cpp
EXPECT_TRUE(success);
EXPECT_EQ(result.outputImage->channels(), 1);
EXPECT_GT(edgePixels, 0);
```

**浮点数比较**:
```cpp
EXPECT_NEAR(distResult.distance, 300.0, 0.1);  // 允许0.1误差
```

---

### 4. 性能测试

所有性能测试默认禁用（DISABLED_前缀），需要显式运行：

```cpp
TEST_F(BasicImageProcessingTest, DISABLED_Performance_GrayConversion) {
    auto largeImage = ImageMemoryPool::instance().allocate(1920, 1080, CV_8UC3);

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < 100; ++i) {
        grayTool.process(largeImage, result);
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "100次灰度转换(1920x1080)耗时: " << duration.count() << "ms" << std::endl;

    EXPECT_LT(duration.count(), 1000);  // 性能基准
}
```

**运行性能测试**:
```bash
./test_BasicImageProcessing_gtest --gtest_also_run_disabled_tests
```

---

## 🔧 编译和运行

### 编译测试

```bash
cd build

# 重新配置CMake（启用Google Test）
cmake -DUSE_GTEST=ON ..

# 编译所有测试
cmake --build . --target all --config Release

# 编译单个测试套件
cmake --build . --target test_BasicImageProcessing_gtest --config Release
cmake --build . --target test_EdgeAndMorphology_gtest --config Release
cmake --build . --target test_ShapeDetection_gtest --config Release
cmake --build . --target test_Measurement_gtest --config Release
```

---

### 运行测试

**运行所有Google Test测试**:
```bash
cd build/bin/Release

# 运行所有测试套件
./test_ImageData_gtest
./test_BasicImageProcessing_gtest
./test_EdgeAndMorphology_gtest
./test_ShapeDetection_gtest
./test_Measurement_gtest
```

**运行特定测试**:
```bash
# 运行单个测试用例
./test_BasicImageProcessing_gtest --gtest_filter=BasicImageProcessingTest.GrayTool_BGR2Gray

# 运行特定测试类
./test_BasicImageProcessing_gtest --gtest_filter=BasicImageProcessingTest.*

# 运行参数化测试
./test_BasicImageProcessing_gtest --gtest_filter=BlurKernelSizeTest.*
```

**运行性能测试**:
```bash
./test_BasicImageProcessing_gtest --gtest_also_run_disabled_tests --gtest_filter=*Performance*
```

**使用CTest**:
```bash
cd build

# 运行所有测试
ctest

# 详细输出
ctest -V

# 运行特定测试
ctest -R BasicImageProcessing

# 并行运行
ctest -j4
```

---

### 测试输出示例

```
[==========] Running 12 tests from 3 test suites.
[----------] Global test environment set-up.
[----------] 3 tests from BasicImageProcessingTest
[ RUN      ] BasicImageProcessingTest.GrayTool_BGR2Gray
[       OK ] BasicImageProcessingTest.GrayTool_BGR2Gray (5 ms)
[ RUN      ] BasicImageProcessingTest.ThresholdTool_BinaryThreshold
[       OK ] BasicImageProcessingTest.ThresholdTool_BinaryThreshold (8 ms)
[ RUN      ] BasicImageProcessingTest.BlurTool_GaussianBlur
[       OK ] BasicImageProcessingTest.BlurTool_GaussianBlur (12 ms)
[----------] 3 tests from BasicImageProcessingTest (25 ms total)

[----------] 5 tests from BlurKernelSizeTest/OddKernelSizes
[ RUN      ] BlurKernelSizeTest/OddKernelSizes.GaussianBlurWithDifferentKernels/0
[       OK ] BlurKernelSizeTest/OddKernelSizes.GaussianBlurWithDifferentKernels/0 (10 ms)
...
[----------] 5 tests from BlurKernelSizeTest/OddKernelSizes (52 ms total)

[==========] 12 tests from 3 test suites ran. (100 ms total)
[  PASSED  ] 12 tests.
```

---

## 📈 测试覆盖的算法类别

| 算法类别 | 覆盖的工具类 | 测试套件 |
|---------|-----------|---------|
| **数据结构** | ImageData | test_ImageData_gtest |
| **基础图像处理** | GrayTool, ThresholdTool, BlurTool | test_BasicImageProcessing_gtest |
| **边缘检测** | EdgeTool, FindEdgeTool | test_EdgeAndMorphology_gtest |
| **形态学处理** | MorphologyTool | test_EdgeAndMorphology_gtest |
| **形状检测** | CircleTool, LineTool, BlobTool | test_ShapeDetection_gtest |
| **测量算法** | MeasureDistanceTool, MeasureAngleTool, MeasureAreaTool | test_Measurement_gtest |

---

## 🚀 下一步计划

### 待添加的测试套件

- [ ] **test_ColorConversion_gtest** - 颜色空间转换测试（ColorConvertTool）
- [ ] **test_TemplateMatching_gtest** - 模板匹配测试（TemplateMatchTool, ShapeMatchTool）
- [ ] **test_CalibrationAlignment_gtest** - 标定和对位测试（CalcCenterTool, CalcOrientationTool, MultiPointAlignmentTool）
- [ ] **test_LogicAndOutput_gtest** - 逻辑运算和输出测试（LogicOperationTool, RangeJudgeTool, PLCOutputTool）
- [ ] **test_AIDetection_gtest** - AI检测测试（AIDetectionTool）

### 测试基础设施改进

- [ ] 集成测试覆盖率工具 (gcov/lcov)
- [ ] 建立CI/CD自动化测试流程 (GitHub Actions / Jenkins)
- [ ] 添加Google Benchmark性能基准测试
- [ ] 创建测试数据集管理系统
- [ ] 编写测试报告生成脚本

---

## 📚 参考资料

### 项目文档
- [测试框架使用指南](TESTING_FRAMEWORK_GUIDE.md)
- [测试框架集成总结](TESTING_INTEGRATION_SUMMARY.md)

### Google Test文档
- [Google Test GitHub](https://github.com/google/googletest)
- [Google Test文档](https://google.github.io/googletest/)
- [高级指南](https://google.github.io/googletest/advanced.html)

### 相关工具
- [CMake CTest](https://cmake.org/cmake/help/latest/manual/ctest.1.html)
- [OpenCV Testing](https://docs.opencv.org/4.x/d7/d3c/tutorial_table_of_content_core.html)

---

## 📝 维护说明

### 添加新测试用例

1. 在对应测试套件文件中添加TEST_F或TEST_P
2. 遵循现有命名规范: `ToolName_TestScenario`
3. 使用ImageMemoryPool进行图像分配
4. 添加必要的断言验证

### 添加新测试套件

1. 在test/目录创建新文件: `test_FeatureName_gtest.cpp`
2. 包含必要的头文件
3. 创建Test Fixture类
4. 编写测试用例
5. 在test/CMakeLists.txt中注册: `add_gtest_test(test_FeatureName_gtest test_FeatureName_gtest.cpp)`

### 代码审查清单

- [ ] 所有测试都有清晰的命名
- [ ] 使用ImageMemoryPool进行RAII管理
- [ ] 参数化测试覆盖了关键场景
- [ ] 包含边界条件和异常输入测试
- [ ] 性能测试使用DISABLED_前缀
- [ ] 断言选择恰当 (ASSERT vs EXPECT)
- [ ] 浮点数比较使用EXPECT_NEAR

---

**最后更新**: 2025-12-20
**负责人**: VisionForge Team
**状态**: ✅ 已完成 - 95个测试用例，覆盖12个核心算法工具类
