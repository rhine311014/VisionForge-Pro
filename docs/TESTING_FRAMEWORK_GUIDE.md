# VisionForge Pro 测试框架使用指南

**文档版本**: 1.0
**创建日期**: 2025-12-20
**目标**: 统一项目的单元测试和集成测试标准

---

## 📋 概述

VisionForge Pro支持两种主流的C++测试框架：

1. **Qt Test** - Qt官方测试框架，与Qt应用深度集成
2. **Google Test** - Google开源测试框架，业界标准

两种框架各有优势，可根据测试场景选择使用。

---

## 🎯 框架选择指南

### Qt Test - 推荐用于

✅ **Qt组件测试**: 测试QWidget、QObject派生类
✅ **信号槽测试**: 需要QSignalSpy等Qt专用功能
✅ **现有测试**: 项目已有的Qt Test测试

**示例场景**:
- UI组件测试（MainWindow、Dialog等）
- Qt元对象系统测试（信号/槽、属性）
- ConfigManager、Logger等Qt依赖类

---

### Google Test - 推荐用于

✅ **算法测试**: 纯C++算法、数学计算
✅ **性能测试**: 提供benchmark支持
✅ **跨平台测试**: 更好的跨IDE/CI支持
✅ **参数化测试**: 强大的参数化测试功能

**示例场景**:
- 图像处理算法（GrayTool、ThresholdTool）
- 内存池（ImageMemoryPool）
- 并行处理（ParallelProcessor）
- 数据结构（ImageData、AlignedMemory）

---

## 🏗️ 项目测试结构

```
VisionForge Pro/
├── test/
│   ├── CMakeLists.txt                  # 测试构建配置
│   ├── TestUtils.h/cpp                 # 通用测试工具
│   │
│   ├── # Qt Test 测试文件
│   ├── test_MainWindow.cpp             # UI测试（Qt Test）
│   ├── test_ConfigManager.cpp          # Qt配置测试
│   ├── test_Logger.cpp                 # Qt日志测试
│   │
│   ├── # Google Test 测试文件
│   ├── test_ImageData_gtest.cpp        # 图像数据测试（GTest）
│   ├── test_ParallelProcessor_gtest.cpp# 并行处理测试
│   ├── test_Algorithm_gtest.cpp        # 算法测试
│   │
│   └── # 混合测试（两种框架均可）
│       ├── test_ImageMemoryPool.cpp    # Qt Test版本（现有）
│       └── test_ImageMemoryPool_gtest.cpp # Google Test版本（可选）
│
└── cmake/
    └── GoogleTest.cmake                # Google Test集成配置
```

---

## 📝 Qt Test 使用示例

### 基本结构

```cpp
#include <QtTest/QtTest>
#include "base/ImageData.h"

class TestImageData : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();    // 所有测试前执行一次
    void cleanupTestCase(); // 所有测试后执行一次
    void init();            // 每个测试前执行
    void cleanup();         // 每个测试后执行

    // 测试用例
    void testConstruction();
    void testCopy();
    void testClone();
};

void TestImageData::testConstruction()
{
    ImageData image(cv::Mat(480, 640, CV_8UC3));

    QCOMPARE(image.width(), 640);
    QCOMPARE(image.height(), 480);
    QVERIFY(!image.isEmpty());
}

// Qt Test需要的宏
QTEST_MAIN(TestImageData)
#include "test_ImageData.moc"
```

### Qt Test常用断言

```cpp
QVERIFY(condition);                 // 验证条件为真
QCOMPARE(actual, expected);         // 比较相等
QVERIFY2(condition, "message");     // 带消息的验证

// 浮点数比较
QCOMPARE(actual, expected);         // 精确比较
qFuzzyCompare(a, b);               // 模糊比较（浮点）

// 异常测试
QEXPECT_FAIL("", "message", mode);  // 预期失败
```

### Qt Test信号槽测试

```cpp
void TestWorker::testSignals()
{
    Worker worker;
    QSignalSpy spy(&worker, &Worker::finished);

    worker.start();

    // 等待信号
    QVERIFY(spy.wait(5000));  // 最多等待5秒
    QCOMPARE(spy.count(), 1);

    // 检查信号参数
    QList<QVariant> arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toBool(), true);
}
```

---

## 📝 Google Test 使用示例

### 基本结构

```cpp
#include <gtest/gtest.h>
#include "base/ImageData.h"

// 测试夹具（Test Fixture）
class ImageDataTest : public ::testing::Test {
protected:
    void SetUp() override {
        // 每个测试前执行
        testMat = cv::Mat(480, 640, CV_8UC3);
    }

    void TearDown() override {
        // 每个测试后执行
        testMat.release();
    }

    cv::Mat testMat;
};

// 测试用例
TEST_F(ImageDataTest, Construction) {
    ImageData image(testMat);

    EXPECT_EQ(image.width(), 640);
    EXPECT_EQ(image.height(), 480);
    ASSERT_FALSE(image.isEmpty());
}

// 主函数
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
```

### Google Test常用断言

```cpp
// 致命断言（失败后停止当前测试）
ASSERT_EQ(actual, expected);
ASSERT_NE(a, b);
ASSERT_LT(a, b);
ASSERT_LE(a, b);
ASSERT_GT(a, b);
ASSERT_GE(a, b);
ASSERT_TRUE(condition);
ASSERT_FALSE(condition);

// 非致命断言（失败后继续执行）
EXPECT_EQ(actual, expected);
EXPECT_NE(a, b);
EXPECT_TRUE(condition);
EXPECT_FALSE(condition);

// 浮点数比较
EXPECT_FLOAT_EQ(a, b);      // float比较
EXPECT_DOUBLE_EQ(a, b);     // double比较
EXPECT_NEAR(a, b, delta);   // 允许误差

// 字符串比较
EXPECT_STREQ(s1, s2);       // C字符串相等
EXPECT_STRNE(s1, s2);
```

### 参数化测试

```cpp
class ImageTypeTest : public ::testing::TestWithParam<int> {
protected:
    void SetUp() override {
        imageType = GetParam();
    }
    int imageType;
};

TEST_P(ImageTypeTest, AllTypes) {
    cv::Mat mat(480, 640, imageType);
    ImageData image(mat);
    EXPECT_FALSE(image.isEmpty());
}

// 注册参数
INSTANTIATE_TEST_SUITE_P(
    AllImageTypes,
    ImageTypeTest,
    ::testing::Values(CV_8UC1, CV_8UC3, CV_16UC1)
);
```

### 死亡测试

```cpp
TEST_F(ImageDataTest, InvalidAccess) {
    ImageData emptyImage;

    // 验证会导致程序终止的代码
    EXPECT_DEBUG_DEATH({
        emptyImage.mat().at<uchar>(0, 0) = 255;
    }, ".*");
}
```

---

## 🔧 编译和运行

### 配置CMake

```bash
# 启用测试
cmake -DBUILD_TESTS=ON ..

# 同时启用Qt Test和Google Test
cmake -DBUILD_TESTS=ON -DUSE_GTEST=ON ..
```

### 编译测试

```bash
# 编译所有测试
cmake --build . --target all --config Release

# 编译单个测试
cmake --build . --target test_ImageData --config Release
cmake --build . --target test_ImageData_gtest --config Release
```

### 运行测试

**Qt Test测试**:
```bash
cd build/bin/Release

# 运行单个测试
./test_ImageData

# 详细输出
./test_ImageData -v2

# 运行特定测试函数
./test_ImageData testConstruction
```

**Google Test测试**:
```bash
cd build/bin/Release

# 运行所有测试
./test_ImageData_gtest

# 运行特定测试
./test_ImageData_gtest --gtest_filter=ImageDataTest.Construction

# 运行禁用的测试
./test_ImageData_gtest --gtest_also_run_disabled_tests

# 重复运行（压力测试）
./test_ImageData_gtest --gtest_repeat=100

# 详细输出
./test_ImageData_gtest --gtest_print_time=1

# 生成XML报告
./test_ImageData_gtest --gtest_output=xml:report.xml
```

### 使用CTest

```bash
cd build

# 运行所有测试
ctest

# 详细输出
ctest -V

# 运行特定测试
ctest -R ImageData

# 并行运行
ctest -j4
```

---

## 📊 框架对比

| 特性 | Qt Test | Google Test |
|------|---------|-------------|
| **Qt集成** | ✅ 原生支持 | ⚠️ 需要手动处理 |
| **信号槽测试** | ✅ QSignalSpy | ❌ 不支持 |
| **参数化测试** | ⚠️ 有限支持 | ✅ 强大支持 |
| **死亡测试** | ❌ 不支持 | ✅ 支持 |
| **Mock支持** | ❌ 不支持 | ✅ Google Mock |
| **性能测试** | ⚠️ 基础支持 | ✅ Google Benchmark |
| **输出格式** | 文本 | 文本/XML/JSON |
| **CI集成** | ⚠️ 一般 | ✅ 优秀 |
| **学习曲线** | ⭐⭐ 简单 | ⭐⭐⭐ 中等 |
| **社区支持** | Qt社区 | 全球开源社区 |

---

## 🎯 最佳实践

### 1. 测试命名规范

**Qt Test**:
```cpp
class TestImageData : public QObject      // Test前缀
{
private slots:
    void testConstruction();              // test前缀
    void testCopy();
};
```

**Google Test**:
```cpp
class ImageDataTest : public ::testing::Test  // Test后缀
{
};

TEST_F(ImageDataTest, Construction) {     // PascalCase测试名
    // ...
}
```

### 2. 测试组织

```cpp
// 按功能分组
TEST_F(ImageDataTest, Construction_Default) { }
TEST_F(ImageDataTest, Construction_FromMat) { }
TEST_F(ImageDataTest, Construction_Copy) { }

TEST_F(ImageDataTest, ROI_Set) { }
TEST_F(ImageDataTest, ROI_Clear) { }
TEST_F(ImageDataTest, ROI_Invalid) { }
```

### 3. 测试数据

```cpp
// 使用测试夹具共享测试数据
class ImageDataTest : public ::testing::Test {
protected:
    void SetUp() override {
        // 准备测试数据
        testImage640x480 = createTestImage(640, 480);
        testImage1920x1080 = createTestImage(1920, 1080);
    }

    ImageData::Ptr testImage640x480;
    ImageData::Ptr testImage1920x1080;
};
```

### 4. 断言选择

```cpp
// ✅ 好的做法
EXPECT_EQ(image.width(), 640);              // 明确的预期值
EXPECT_GT(image.timestamp(), 0);            // 合理的范围检查
EXPECT_NEAR(value, 3.14, 0.01);            // 浮点数带误差

// ❌ 不好的做法
EXPECT_TRUE(image.width() == 640);          // 应该用EXPECT_EQ
EXPECT_NE(image.timestamp(), 0);            // 应该用EXPECT_GT
EXPECT_EQ(doubleValue, 3.14);              // 浮点数不应精确比较
```

### 5. 测试隔离

```cpp
// ✅ 好的做法 - 每个测试独立
TEST_F(ImageDataTest, Test1) {
    ImageData image(createTestMat());
    // 测试逻辑
}

TEST_F(ImageDataTest, Test2) {
    ImageData image(createTestMat());  // 重新创建
    // 测试逻辑
}

// ❌ 不好的做法 - 测试间有依赖
class BadTest : public ::testing::Test {
protected:
    static ImageData sharedImage;  // 跨测试共享
};
```

---

## 🚀 下一步

- [ ] 为所有核心算法编写Google Test测试
- [ ] 集成测试覆盖率工具（gcov/lcov）
- [ ] 建立CI/CD自动化测试流程
- [ ] 添加性能基准测试（Google Benchmark）
- [ ] 创建测试数据集管理系统

---

## 📚 参考资料

### Qt Test文档
- [Qt Test官方文档](https://doc.qt.io/qt-6/qttest-index.html)
- [Qt Test教程](https://doc.qt.io/qt-6/qttestlib-tutorial.html)

### Google Test文档
- [Google Test GitHub](https://github.com/google/googletest)
- [Google Test文档](https://google.github.io/googletest/)
- [Google Test高级指南](https://google.github.io/googletest/advanced.html)

---

**最后更新**: 2025-12-20
**负责人**: VisionForge Team
