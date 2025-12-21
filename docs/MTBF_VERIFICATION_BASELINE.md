# MTBF验证基准文档

**文档版本**: 1.0
**创建日期**: 2025-12-20
**目标**: 建立VisionForge Pro v1.2.0稳定性验证标准和基准数据

---

## 📋 概述

本文档定义了VisionForge Pro在7x24工业环境下的MTBF（Mean Time Between Failures，平均无故障时间）验证标准、测试方法和验收基准。

## 🎯 MTBF目标

| 指标 | 目标值 | 红线值 | 备注 |
|------|--------|--------|------|
| **MTBF** | >720小时（30天） | >168小时（7天） | 生产环境要求 |
| **内存增长率** | <50MB/72h | <100MB/72h | v1.2.0优化后 |
| **CPU平均使用率** | <40% | <60% | i5-9400F基准 |
| **池命中率** | >85% | >80% | ImageMemoryPool |
| **单次故障恢复时间** | <10秒 | <30秒 | 自动恢复机制 |

---

## 🏗️ 测试框架架构

### 核心组件

```
VisionForge Pro
├── 测试框架层 (test/)
│   ├── StabilityTestFramework       # 主测试框架
│   ├── TestScenario                 # 场景基类
│   ├── HighFrequencyBatchScenario   # 高频批量场景
│   ├── MultiCameraContinuous        # 多相机连续场景（待实现）
│   └── MixedWorkload                # 混合负载场景（待实现）
│
├── 监控工具层 (base/)
│   ├── SystemResourceMonitor        # 系统资源监控
│   └── PoolPerformanceMonitor       # 内存池性能监控
│
└── 自动化脚本层 (test/scripts/)
    ├── run_72h_test.py              # 测试执行脚本
    └── analyze_results.py           # 结果分析脚本
```

### 数据流

```
┌─────────────────┐
│ 被测应用程序    │ ← 测试场景驱动
│ VisionForge.exe│
└────────┬────────┘
         │ 监控数据
         ↓
┌─────────────────┐
│ 监控器          │ → 快照数据 → snapshots.jsonl
│ - SystemResource│
│ - PoolPerformance│
└─────────────────┘
         │
         ↓
┌─────────────────┐
│ 分析脚本        │ → 报告文件
│ analyze_results.py│
└─────────────────┘
```

---

## 🔬 测试场景说明

### 场景1: 高频批量处理 ✅ 已实现

**参数**:
- 批量大小: 100张/批
- 批量间隔: 5分钟
- 测试时长: 24小时
- 总操作数: 28,800张

**验证目标**:
- BatchProcessWorker内存管理
- ScopedImagePtr自动归还
- ImageMemoryPool命中率

**实现文件**:
- [include/test/HighFrequencyBatchScenario.h](../include/test/HighFrequencyBatchScenario.h)
- [src/test/HighFrequencyBatchScenario.cpp](../src/test/HighFrequencyBatchScenario.cpp)

---

### 场景2: 多相机连续采集（待实现）

**参数**:
- 相机数量: 4个
- 采样帧率: 10 FPS/相机
- 测试时长: 24小时
- 总帧数: 3,456,000帧

**验证目标**:
- 多线程并发性能
- 内存池并发安全性
- 相机采集稳定性

---

### 场景3: 混合负载（待实现）

**参数**:
- 并发任务: 批量处理 + 实时采集 + UI交互
- 测试时长: 24小时

**验证目标**:
- 综合压力测试
- UI响应性能
- 多任务调度稳定性

---

## 🛠️ 使用指南

### 步骤1: 准备测试环境

```bash
# 1. 确保编译Release版本
cd VisionForge Pro
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --config Release

# 2. 准备测试图像
mkdir test_images
# 将100+张测试图像复制到test_images目录

# 3. 安装Python依赖
pip install psutil
```

### 步骤2: 启动72小时测试

```bash
# 方法A: 使用Python自动化脚本（推荐）
cd test/scripts
python run_72h_test.py \
    --exe-path "../../build/bin/Release/VisionForge.exe" \
    --duration 72 \
    --output-dir ./test_results \
    --auto-restart

# 方法B: 手动启动应用程序
# 在应用程序中通过UI启动稳定性测试场景
```

### 步骤3: 监控测试进度

Python脚本会每5分钟采集一次快照，每分钟输出进度：

```
进度: 12.5% | 已运行: 9:00:00 | 剩余: 63:00:00 | CPU: 45.2% | 内存: 512MB | 线程: 24
进度: 13.9% | 已运行: 10:00:00 | 剩余: 62:00:00 | CPU: 43.8% | 内存: 515MB | 线程: 24
...
```

### 步骤4: 分析测试结果

```bash
# 生成分析报告
python analyze_results.py \
    --input test_results/snapshots.jsonl \
    --output analysis_report

# 输出文件：
# - analysis_report.txt   # 文本摘要
# - analysis_report.html  # 可视化报告（包含图表）
```

### 步骤5: 查看报告

文本报告示例：

```
============================================================
72小时稳定性测试分析报告
============================================================

【内存分析】
  初始内存: 450 MB
  最终内存: 485 MB
  峰值内存: 520 MB
  内存增长: 35 MB
  增长速率: 0.49 MB/h
  预测72h增长: 35.0 MB

【CPU分析】
  平均使用率: 42.3%
  峰值使用率: 68.5%
  最低使用率: 15.2%

【验收标准检查】
  memory_growth: 35.0 MB/72h ✅
  cpu_usage: 42.3% ✅

【最终结果】: ✅ 测试通过
============================================================
```

---

## 📊 基准数据（v1.2.0目标）

### 硬件基准配置

- **CPU**: Intel Core i5-9400F @ 2.90GHz (6核)
- **内存**: 16GB DDR4 2666MHz
- **存储**: NVMe SSD 512GB
- **操作系统**: Windows 10 Pro x64

### 性能基准

| 场景 | 内存增长 | CPU平均 | 池命中率 | 吞吐量 |
|------|----------|---------|----------|--------|
| 高频批量处理 | 35MB/72h | 42% | 87% | 20张/秒 |
| 多相机采集（目标） | 50MB/72h | 55% | 85% | 40FPS |
| 混合负载（目标） | 80MB/72h | 58% | 83% | - |

---

## ✅ 验收标准详细说明

### 1. 内存增长检查

**检查逻辑**:
```python
memory_growth_72h = (final_memory - initial_memory) * (72 / actual_hours)
pass = memory_growth_72h < 100  # MB
```

**判定**:
- ✅ <50MB: 优秀
- ✅ 50-100MB: 合格
- ❌ >100MB: 不合格（可能存在泄漏）

**预期v1.2.0**:
- v1.1.0（优化前）: ~200MB/72h
- v1.2.0（优化后）: <50MB/72h

---

### 2. CPU使用率检查

**检查逻辑**:
```python
cpu_avg = sum(cpu_samples) / len(cpu_samples)
pass = cpu_avg < 60  # %
```

**判定**:
- ✅ <40%: 优秀（留有充足余量）
- ✅ 40-60%: 合格
- ❌ >60%: 不合格（性能瓶颈）

---

### 3. 池命中率检查

**检查逻辑**:
```cpp
hit_rate = (pool_hits / total_allocations) * 100
pass = hit_rate > 80  // %
```

**判定**:
- ✅ >90%: 优秀
- ✅ 80-90%: 合格
- ❌ <80%: 不合格（池效率低）

**预期v1.2.0**:
- v1.1.0（优化前）: ~25%
- v1.2.0（优化后）: >85%

---

### 4. 无崩溃检查

**检查逻辑**:
```python
pass = (crash_count == 0)
```

**判定**:
- ✅ 0次崩溃: 通过
- ❌ >0次崩溃: 不通过

---

## 📈 趋势监控

### 关键指标趋势

监控以下指标的**线性回归斜率**，判断是否存在退化：

1. **内存增长趋势**: 斜率应接近0（±5 KB/h）
2. **CPU使用率趋势**: 应保持稳定（±2%）
3. **池命中率趋势**: 应保持稳定或上升（>-1%/h）

### 异常检测

- **内存突增**: 单次增长>50MB
- **CPU尖峰**: 持续30秒>80%
- **池效率骤降**: 命中率突降>10%

---

## 🔄 持续改进

### v1.2.0验证计划

- [x] ✅ 完成ScopedImagePtr迁移
- [x] ✅ 实现监控工具
- [x] ✅ 创建测试框架
- [ ] ⏳ 执行初始72小时基准测试
- [ ] ⏳ 建立基准数据库
- [ ] ⏳ 文档化最佳实践

### v1.3.0规划

- [ ] 实现多相机连续场景
- [ ] 实现混合负载场景
- [ ] 添加GPU资源监控
- [ ] 集成远程诊断系统

---

## 📞 参考文档

- [72小时测试计划](STABILITY_TEST_72H_PLAN.md)
- [ScopedImagePtr迁移指南](SCOPED_IMAGE_PTR_MIGRATION_GUIDE.md)
- [BatchProcessWorker集成指南](BATCH_PROCESS_WORKER_INTEGRATION.md)
- [SystemResourceMonitor API](../include/base/SystemResourceMonitor.h)
- [PoolPerformanceMonitor API](../include/base/PoolPerformanceMonitor.h)

---

## 📝 变更记录

| 版本 | 日期 | 作者 | 变更内容 |
|------|------|------|----------|
| 1.0 | 2025-12-20 | Claude Sonnet 4.5 | 初始版本 |

---

**最后更新**: 2025-12-20
**负责人**: VisionForge Team
