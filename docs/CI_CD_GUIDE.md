# VisionForge Pro - CI/CD自动化测试指南

**版本**: 1.0.0
**更新日期**: 2025-12-20
**作者**: Claude Sonnet 4.5

---

## 📋 目录

1. [概述](#概述)
2. [GitHub Actions](#github-actions)
3. [GitLab CI](#gitlab-ci)
4. [代码覆盖率](#代码覆盖率)
5. [本地使用](#本地使用)
6. [故障排查](#故障排查)

---

## 概述

VisionForge Pro已集成完整的CI/CD自动化测试流程，支持：

### ✅ 主要功能

- **多平台测试**: Windows + Linux
- **自动化测试**: 5个测试套件，119个测试用例
- **代码覆盖率**: lcov/gcov + Codecov集成
- **性能基准**: 可选的性能测试
- **测试报告**: XML格式 + HTML可视化

### 📊 测试覆盖

| 测试套件 | 测试数量 | 覆盖工具类 |
|---------|---------|----------|
| test_ImageData_gtest | 15 | ImageData基础功能 |
| test_BasicImageProcessing_gtest | 20 | Gray, Threshold, Blur |
| test_EdgeAndMorphology_gtest | 29 | Edge, FindEdge, Morphology |
| test_ShapeDetection_gtest | 23 | Circle, Line, Blob |
| test_Measurement_gtest | 32 | Distance, Angle, Area |
| **总计** | **119** | **12个工具类** |

---

## GitHub Actions

### 📁 配置文件位置

```
.github/workflows/test.yml
```

### 🔄 触发条件

- **Push**: 推送到 `master`, `develop`, `feature/*` 分支
- **Pull Request**: 针对 `master`, `develop` 的PR
- **手动触发**: 通过GitHub界面手动运行

### 🎯 工作流程

#### 1. Windows测试 (`test-windows`)

```yaml
- 环境: windows-latest
- 编译器: MSVC 2019/2022
- Qt版本: 6.5.3
- OpenCV: 通过vcpkg安装
```

**步骤**:
1. 检出代码（包含子模块）
2. 设置MSVC环境
3. 缓存vcpkg依赖
4. 安装Qt和OpenCV
5. 配置CMake
6. 编译项目
7. 运行5个测试套件
8. 发布测试报告

#### 2. Linux测试 + 代码覆盖率 (`test-linux-coverage`)

```yaml
- 环境: ubuntu-22.04
- 编译器: GCC
- 构建类型: Debug (启用覆盖率)
```

**步骤**:
1. 安装依赖（CMake, Qt6, OpenCV, lcov）
2. 配置CMake（启用 `-DENABLE_COVERAGE=ON`）
3. 编译项目
4. 运行所有测试
5. 生成覆盖率报告（lcov + HTML）
6. 上传到Codecov
7. 发布测试报告

#### 3. 性能基准测试 (`benchmark-tests`)

```yaml
- 触发条件: 仅在Pull Request时
- 允许失败: true
```

运行所有带 `DISABLED_Performance` 前缀的性能测试。

### 📊 查看结果

#### 测试报告
在PR或commit的"Checks"标签页查看：
- ✅ 通过的测试数量
- ❌ 失败的测试详情
- ⏱️ 执行时间

#### 代码覆盖率
访问Codecov仪表板：
```
https://codecov.io/gh/your-org/VisionForge-Pro
```

#### Artifacts下载
测试报告XML文件和覆盖率HTML报告可在Actions运行详情页下载。

---

## GitLab CI

### 📁 配置文件位置

```
.gitlab-ci.yml
```

### 🔄 CI/CD流水线

```
stages:
  - build    # 编译项目
  - test     # 运行测试
  - coverage # 生成覆盖率报告
  - deploy   # 部署到GitLab Pages
```

### 🎯 作业详情

#### 1. build:windows

- **标签**: `windows`, `msvc`
- **产物**: `.exe` 和 `.lib` 文件
- **过期时间**: 1天

#### 2. build:linux

- **镜像**: `ubuntu:22.04`
- **标签**: `docker`
- **产物**: 测试可执行文件
- **覆盖率**: 启用

#### 3. test:windows

- **依赖**: `build:windows`
- **运行**: 5个测试套件
- **报告**: JUnit XML格式

#### 4. test:linux

- **依赖**: `build:linux`
- **运行**: 5个测试套件
- **报告**: JUnit XML + 覆盖率百分比

#### 5. coverage:report

- **依赖**: `build:linux`, `test:linux`
- **工具**: lcov + gcovr
- **输出**:
  - HTML报告: `coverage_html/`
  - XML报告: `coverage.xml`
  - Cobertura格式（GitLab内置支持）

#### 6. pages

- **触发**: 仅在 `master` 分支
- **部署**: 覆盖率HTML报告到GitLab Pages
- **访问**: `https://your-gitlab.com/your-project/pages`

#### 7. scheduled:nightly

- **触发**: 仅在定时任务
- **运行**: 完整测试（包含性能测试）
- **允许失败**: false

### 📊 查看结果

#### 流水线状态
在项目的"CI/CD > Pipelines"查看每个作业的状态。

#### 测试报告
GitLab自动解析JUnit XML报告并显示在MR页面。

#### 代码覆盖率
在"Analytics > Repository Analytics"查看覆盖率趋势。

#### GitLab Pages
访问覆盖率HTML报告：
```
https://your-gitlab.com/your-project/-/pages
```

### 🕒 定时任务设置

在GitLab项目设置中：
1. 进入"CI/CD > Schedules"
2. 创建新定时任务
3. Cron表达式示例：
   - 每日凌晨2点: `0 2 * * *`
   - 每周一凌晨: `0 2 * * 1`

---

## 代码覆盖率

### 🔧 本地生成覆盖率报告

#### 1. 配置CMake（启用覆盖率）

```bash
cmake -B build \
  -DCMAKE_BUILD_TYPE=Debug \
  -DUSE_GTEST=ON \
  -DENABLE_COVERAGE=ON
```

#### 2. 编译项目

```bash
cmake --build build -j $(nproc)
```

#### 3. 运行测试

```bash
cd build
ctest --verbose
```

#### 4. 生成覆盖率报告

使用CMake目标：

```bash
cd build
make coverage  # 或 cmake --build . --target coverage
```

生成的报告位置：
- **HTML报告**: `build/coverage_html/index.html`
- **原始数据**: `build/coverage_filtered.info`

#### 5. 查看报告

```bash
# Linux
xdg-open build/coverage_html/index.html

# macOS
open build/coverage_html/index.html

# Windows（WSL）
explorer.exe build/coverage_html/index.html
```

### 📊 覆盖率指标

| 指标 | 说明 | 目标 |
|-----|------|-----|
| **Line Coverage** | 行覆盖率 | ≥ 80% |
| **Function Coverage** | 函数覆盖率 | ≥ 85% |
| **Branch Coverage** | 分支覆盖率 | ≥ 70% |

### 🛠️ 覆盖率工具

#### lcov
生成HTML可视化报告：
```bash
lcov --capture --directory . --output-file coverage.info
lcov --remove coverage.info '/usr/*' '*/test/*' --output-file coverage_filtered.info
genhtml coverage_filtered.info --output-directory coverage_html
```

#### gcovr
生成多种格式报告：
```bash
# XML格式（Cobertura）
gcovr --xml-pretty -o coverage.xml

# HTML格式
gcovr --html --html-details -o coverage.html

# 终端摘要
gcovr --print-summary
```

---

## 本地使用

### 🏃 本地运行完整测试流程

模拟CI/CD环境：

```bash
#!/bin/bash
# test-local.sh - 本地CI/CD测试脚本

set -e

echo "=== 清理构建目录 ==="
rm -rf build
mkdir -p build

echo "=== 配置CMake ==="
cmake -B build \
  -DCMAKE_BUILD_TYPE=Debug \
  -DUSE_GTEST=ON \
  -DENABLE_COVERAGE=ON

echo "=== 编译项目 ==="
cmake --build build -j $(nproc)

echo "=== 运行测试 ==="
cd build/bin
for test in test_*_gtest; do
    echo ">>> 运行 $test <<<"
    ./$test --gtest_output=xml:${test}_results.xml
done

echo "=== 生成覆盖率报告 ==="
cd ..
make coverage

echo "=== ✅ 所有检查完成 ==="
echo "测试报告: build/bin/*_results.xml"
echo "覆盖率报告: build/coverage_html/index.html"
```

使用：
```bash
chmod +x test-local.sh
./test-local.sh
```

### 🔍 单个测试套件

```bash
cd build/bin

# 运行特定测试套件
./test_ImageData_gtest

# 生成XML报告
./test_ImageData_gtest --gtest_output=xml:results.xml

# 运行性能测试
./test_ImageData_gtest --gtest_also_run_disabled_tests

# 过滤特定测试
./test_ImageData_gtest --gtest_filter="ImageDataTest.Clone"

# 详细输出
./test_ImageData_gtest --gtest_print_time=1
```

---

## 故障排查

### ❌ 常见问题

#### 1. Windows编译失败：找不到Qt

**解决方案**:
```powershell
# 设置Qt路径
set CMAKE_PREFIX_PATH=C:\Qt\6.5.3\msvc2019_64

# 或在CMake命令中指定
cmake -B build -DCMAKE_PREFIX_PATH="C:\Qt\6.5.3\msvc2019_64"
```

#### 2. Linux编译失败：找不到OpenCV

**解决方案**:
```bash
# 安装OpenCV开发包
sudo apt-get install libopencv-dev

# 或指定OpenCV路径
cmake -B build -DOpenCV_DIR=/path/to/opencv/lib/cmake/opencv4
```

#### 3. 覆盖率报告为空

**原因**: 未在Debug模式下编译

**解决方案**:
```bash
cmake -B build -DCMAKE_BUILD_TYPE=Debug -DENABLE_COVERAGE=ON
cmake --build build
ctest  # 必须运行测试生成.gcda文件
make coverage
```

#### 4. GitHub Actions vcpkg缓存失效

**解决方案**:
- 清除缓存：在Actions界面手动删除缓存
- 修改vcpkg.json触发重新缓存
- 检查vcpkg版本一致性

#### 5. GitLab CI找不到lcov

**解决方案**:
```yaml
before_script:
  - apt-get update
  - apt-get install -y lcov gcovr
```

### 🔧 调试技巧

#### 查看详细日志

GitHub Actions:
```yaml
- name: 🧪 运行测试
  run: |
    ./test_ImageData_gtest --gtest_print_time=1 --gtest_color=yes
```

GitLab CI:
```yaml
script:
  - ctest --verbose --output-on-failure
```

#### 本地复现CI环境

使用Docker复现Linux CI环境：
```bash
docker run -it ubuntu:22.04 /bin/bash

# 在容器内执行CI脚本
apt-get update
apt-get install -y build-essential cmake qt6-base-dev libopencv-dev
# ... 执行构建和测试命令
```

---

## 📈 最佳实践

### 1. 提交前本地测试

```bash
# 快速测试
cmake --build build && ctest

# 完整测试
./test-local.sh
```

### 2. 编写可靠的测试

- ✅ 独立性：测试之间不相互依赖
- ✅ 确定性：相同输入总是产生相同输出
- ✅ 快速：单个测试< 1秒
- ✅ 清晰：失败时能明确定位问题

### 3. 监控覆盖率

- 每次MR都查看覆盖率变化
- 新功能必须有测试覆盖
- 定期审查低覆盖率模块

### 4. 性能测试策略

- 性能测试默认禁用（DISABLED_前缀）
- 定期在CI中运行（定时任务）
- 建立性能基准并监控回归

---

## 📚 相关文档

- [测试框架使用指南](TESTING_FRAMEWORK_GUIDE.md)
- [算法测试总结](ALGORITHM_TESTS_SUMMARY.md)
- [测试工作总结](TESTING_WORK_SUMMARY.md)

---

**维护者**: VisionForge Team
**最后更新**: 2025-12-20
**反馈**: 请在Issues中报告问题或建议
