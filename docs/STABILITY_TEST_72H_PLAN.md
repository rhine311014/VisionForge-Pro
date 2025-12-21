# VisionForge Pro - 72小时连续运行稳定性测试方案

**版本**: 1.0
**创建日期**: 2025-12-20
**测试目标**: 验证7x24工业环境下的系统稳定性

---

## 📋 测试目标

### 主要目标
1. **内存稳定性**: 验证72小时内存增长<100MB
2. **性能稳定性**: CPU/内存/响应时间无显著退化
3. **功能稳定性**: 零崩溃,零死锁,零数据损坏
4. **资源管理**: 内存池命中率保持>80%

### 验收标准
| 指标 | 目标值 | 红线值 | 测量方法 |
|------|--------|--------|----------|
| **内存增长** | <50MB/24h | <100MB/24h | 进程内存监控 |
| **内存泄漏率** | 0 KB/s | <1 KB/s | Valgrind/ASAN |
| **CPU使用率** | <60% | <80% | 系统监控 |
| **响应时间** | <100ms | <200ms | 性能计数器 |
| **池命中率** | >85% | >75% | ImageMemoryPool统计 |
| **崩溃次数** | 0 | 0 | 进程监控 |
| **错误日志** | <10/h | <50/h | 日志分析 |

---

## 🎯 测试场景设计

### 场景1: 高频批量处理 (24小时循环)
**目标**: 验证批处理内存管理和线程安全性

```yaml
场景名称: HighFrequencyBatch
持续时间: 24小时
循环周期: 每5分钟一次

操作流程:
  1. 加载100张测试图片(1920x1080)
  2. 执行完整工具链(10个工具)
  3. 保存处理结果
  4. 清理临时数据
  5. 等待1分钟
  6. 重复

监控指标:
  - 每轮处理时间
  - 内存占用峰值
  - 线程数量变化
  - 内存池统计
  - 错误/警告日志

预期结果:
  - 处理时间波动<5%
  - 内存占用稳定(±20MB)
  - 无内存泄漏
  - 池命中率>85%
```

### 场景2: 多相机连续采集 (24小时)
**目标**: 验证相机管理和图像处理的长期稳定性

```yaml
场景名称: MultiCameraContinuous
持续时间: 24小时
采集频率: 10 FPS

相机配置:
  - 模拟相机数量: 4台
  - 图像分辨率: 1920x1080
  - 像素格式: RGB8
  - 采集模式: 连续采集

操作流程:
  1. 初始化4台模拟相机
  2. 启动连续采集(10 FPS)
  3. 每帧执行基础处理(灰度+阈值)
  4. 统计处理结果
  5. 持续24小时

监控指标:
  - 采集帧率稳定性
  - 丢帧率
  - 图像处理延迟
  - 内存占用
  - 相机状态

预期结果:
  - 实际帧率: 9.8-10.2 FPS
  - 丢帧率: <0.1%
  - 处理延迟: <50ms
  - 无相机断连
```

### 场景3: 混合压力测试 (24小时)
**目标**: 模拟真实工业环境的复杂负载

```yaml
场景名称: MixedWorkload
持续时间: 24小时

并发任务:
  任务A: 批量处理(每10分钟,50张图)
  任务B: 实时采集(2台相机,5 FPS)
  任务C: 标定操作(每小时1次)
  任务D: 模板匹配(每30秒1次)
  任务E: 形状检测(每1分钟1次)

资源竞争:
  - 同时访问ImageMemoryPool
  - 并发文件I/O
  - 多线程图像处理
  - GPU资源竞争(如启用)

监控指标:
  - 各任务完成率
  - 任务间干扰程度
  - 资源竞争冲突
  - 死锁检测
  - 整体系统负载

预期结果:
  - 所有任务完成率>99%
  - 无死锁发生
  - CPU使用率<70%
  - 内存占用稳定
```

---

## 🛠️ 测试框架实现

### 核心类设计

```cpp
/**
 * @brief 72小时稳定性测试框架
 */
class StabilityTestFramework : public QObject
{
    Q_OBJECT

public:
    struct TestConfig {
        QString testName;               ///< 测试名称
        int durationHours;              ///< 持续时间(小时)
        int reportIntervalMinutes;      ///< 报告间隔(分钟)
        bool enableMemoryMonitor;       ///< 启用内存监控
        bool enablePerformanceMonitor;  ///< 启用性能监控
        QString outputDir;              ///< 输出目录
    };

    struct TestMetrics {
        // 内存指标
        qint64 memoryUsageMB;           ///< 当前内存(MB)
        qint64 memoryPeakMB;            ///< 峰值内存(MB)
        double memoryGrowthRate;        ///< 内存增长率(KB/h)

        // 性能指标
        double cpuUsagePercent;         ///< CPU使用率(%)
        int threadCount;                ///< 线程数量
        double avgResponseTimeMs;       ///< 平均响应时间(ms)

        // 池统计
        double poolHitRate;             ///< 池命中率(%)
        int poolSize;                   ///< 池大小
        qint64 poolMemoryMB;            ///< 池内存(MB)

        // 错误统计
        int errorCount;                 ///< 错误数量
        int warningCount;               ///< 警告数量
        int crashCount;                 ///< 崩溃数量
    };

    explicit StabilityTestFramework(QObject* parent = nullptr);

    // 测试控制
    bool startTest(const TestConfig& config);
    void stopTest();
    void pauseTest();
    void resumeTest();

    // 场景注册
    void registerScenario(std::unique_ptr<TestScenario> scenario);

    // 监控
    TestMetrics getCurrentMetrics() const;
    QList<TestMetrics> getMetricsHistory() const;

signals:
    void testStarted(const QString& testName);
    void testProgress(int hoursElapsed, int totalHours);
    void metricsUpdated(const TestMetrics& metrics);
    void testCompleted(bool success, const QString& report);
    void testError(const QString& error);

private slots:
    void onMonitorTimer();
    void onReportTimer();

private:
    void collectMetrics();
    void generateReport();
    bool validateMetrics(const TestMetrics& metrics);

private:
    TestConfig config_;
    QList<std::unique_ptr<TestScenario>> scenarios_;
    QTimer* monitorTimer_;
    QTimer* reportTimer_;
    QElapsedTimer runTimer_;
    QList<TestMetrics> metricsHistory_;
    bool isRunning_;
};

/**
 * @brief 测试场景基类
 */
class TestScenario : public QObject
{
    Q_OBJECT

public:
    virtual ~TestScenario() = default;

    virtual QString name() const = 0;
    virtual QString description() const = 0;
    virtual bool setup() = 0;
    virtual bool execute() = 0;
    virtual bool cleanup() = 0;

signals:
    void scenarioStarted(const QString& name);
    void scenarioProgress(int percentage);
    void scenarioCompleted(bool success);
    void scenarioError(const QString& error);
};
```

### 具体场景实现

```cpp
/**
 * @brief 高频批量处理场景
 */
class HighFrequencyBatchScenario : public TestScenario
{
    Q_OBJECT

public:
    struct Config {
        int imageCount = 100;           ///< 每批图片数量
        int cycleIntervalMinutes = 5;   ///< 循环间隔(分钟)
        QStringList toolChain;          ///< 工具链配置
    };

    explicit HighFrequencyBatchScenario(const Config& config);

    QString name() const override { return "HighFrequencyBatch"; }
    QString description() const override;
    bool setup() override;
    bool execute() override;
    bool cleanup() override;

private:
    void executeBatch();

private:
    Config config_;
    QTimer* cycleTimer_;
    int batchCount_;
    QList<double> processingTimes_;
};
```

---

## 📊 监控指标实现

### 系统资源监控器

```cpp
/**
 * @brief 系统资源监控器
 */
class SystemResourceMonitor : public QObject
{
    Q_OBJECT

public:
    struct ResourceSnapshot {
        qint64 timestamp;               ///< 时间戳

        // 进程内存
        qint64 virtualMemoryMB;         ///< 虚拟内存(MB)
        qint64 physicalMemoryMB;        ///< 物理内存(MB)
        qint64 privateMemoryMB;         ///< 私有内存(MB)

        // CPU
        double cpuUsagePercent;         ///< CPU使用率(%)
        double cpuUserPercent;          ///< 用户态CPU(%)
        double cpuKernelPercent;        ///< 内核态CPU(%)

        // 线程
        int threadCount;                ///< 线程数量
        int handleCount;                ///< 句柄数量

        // I/O
        qint64 readBytesSec;            ///< 读取速率(B/s)
        qint64 writeBytesSec;           ///< 写入速率(B/s)
    };

    explicit SystemResourceMonitor(QObject* parent = nullptr);

    // 监控控制
    void start(int intervalMs = 1000);
    void stop();

    // 数据获取
    ResourceSnapshot getCurrentSnapshot() const;
    QList<ResourceSnapshot> getHistory(int seconds) const;

    // 统计分析
    double getMemoryGrowthRate() const;  ///< 内存增长率(KB/h)
    double getAvgCpuUsage(int seconds) const;

signals:
    void snapshotReady(const ResourceSnapshot& snapshot);
    void memoryLeakDetected(double rateKBs);
    void highCpuUsage(double percent);

private:
    void collectSnapshot();

#ifdef _WIN32
    void collectWindows(ResourceSnapshot& snapshot);
#else
    void collectLinux(ResourceSnapshot& snapshot);
#endif

private:
    QTimer* timer_;
    QList<ResourceSnapshot> history_;
    int maxHistorySize_;
};
```

### ImageMemoryPool监控

```cpp
/**
 * @brief 内存池性能监控
 */
class PoolPerformanceMonitor : public QObject
{
    Q_OBJECT

public:
    struct PoolMetrics {
        // 基本统计
        size_t totalAllocations;        ///< 总分配次数
        size_t cacheHits;               ///< 缓存命中次数
        size_t cacheMisses;             ///< 缓存未命中次数
        double hitRate;                 ///< 命中率

        // 容量统计
        size_t poolSize;                ///< 池大小
        size_t activeObjects;           ///< 活跃对象数
        qint64 memoryUsageMB;           ///< 内存占用(MB)

        // 性能统计
        double avgAllocTimeUs;          ///< 平均分配时间(微秒)
        double avgReleaseTimeUs;        ///< 平均释放时间(微秒)
        int fragmentationPercent;       ///< 碎片率(%)
    };

    static PoolPerformanceMonitor& instance();

    void startMonitoring(int intervalMs = 5000);
    void stopMonitoring();

    PoolMetrics getCurrentMetrics() const;
    QList<PoolMetrics> getHistory() const;

    // 分析
    bool isPerformanceDegrading() const;
    QString generateReport() const;

signals:
    void metricsUpdated(const PoolMetrics& metrics);
    void hitRateDropped(double rate);
    void memoryFragmentation(int percent);

private:
    PoolPerformanceMonitor() = default;
    void collectMetrics();

private:
    QTimer* timer_;
    QList<PoolMetrics> history_;
};
```

---

## 🔍 自动化执行流程

### 测试执行脚本

```python
#!/usr/bin/env python3
"""
72小时稳定性测试自动化脚本
"""

import subprocess
import time
import json
import datetime
import psutil
from pathlib import Path

class StabilityTestRunner:
    def __init__(self, config_file):
        self.config = self.load_config(config_file)
        self.test_start = None
        self.metrics_log = []

    def load_config(self, file):
        with open(file, 'r', encoding='utf-8') as f:
            return json.load(f)

    def run_test(self):
        """执行72小时测试"""
        print(f"开始72小时稳定性测试: {datetime.datetime.now()}")

        # 1. 启动VisionForge测试模式
        self.start_application()

        # 2. 开始监控
        self.start_monitoring()

        # 3. 执行测试场景
        duration_hours = 72
        for hour in range(duration_hours):
            self.execute_scenarios()
            self.collect_metrics()
            self.check_health()
            time.sleep(3600)  # 1小时

        # 4. 生成报告
        self.generate_report()

    def start_application(self):
        """启动应用"""
        cmd = [
            self.config['app_path'],
            '--test-mode',
            '--stability-test',
            '--duration', '72h'
        ]
        self.process = subprocess.Popen(cmd)
        time.sleep(10)  # 等待启动

    def start_monitoring(self):
        """启动系统监控"""
        self.process_monitor = psutil.Process(self.process.pid)

    def collect_metrics(self):
        """收集指标"""
        metrics = {
            'timestamp': datetime.datetime.now().isoformat(),
            'memory_mb': self.process_monitor.memory_info().rss / 1024 / 1024,
            'cpu_percent': self.process_monitor.cpu_percent(interval=1),
            'threads': self.process_monitor.num_threads(),
        }
        self.metrics_log.append(metrics)

    def check_health(self):
        """健康检查"""
        if not self.process.poll() is None:
            raise RuntimeError("应用崩溃!")

        # 内存检查
        current_mem = self.metrics_log[-1]['memory_mb']
        if len(self.metrics_log) > 24:
            initial_mem = self.metrics_log[0]['memory_mb']
            growth = current_mem - initial_mem
            if growth > 200:  # 200MB红线
                print(f"警告: 内存增长过大 {growth}MB")

    def generate_report(self):
        """生成测试报告"""
        report = {
            'test_duration': '72h',
            'test_end': datetime.datetime.now().isoformat(),
            'metrics': self.metrics_log,
            'summary': self.calculate_summary()
        }

        output_file = Path('stability_test_report.json')
        with open(output_file, 'w', encoding='utf-8') as f:
            json.dump(report, f, indent=2, ensure_ascii=False)

        print(f"测试报告已生成: {output_file}")

    def calculate_summary(self):
        """计算摘要统计"""
        mem_values = [m['memory_mb'] for m in self.metrics_log]
        cpu_values = [m['cpu_percent'] for m in self.metrics_log]

        return {
            'memory_initial_mb': mem_values[0],
            'memory_final_mb': mem_values[-1],
            'memory_peak_mb': max(mem_values),
            'memory_growth_mb': mem_values[-1] - mem_values[0],
            'cpu_avg_percent': sum(cpu_values) / len(cpu_values),
            'cpu_peak_percent': max(cpu_values),
        }

if __name__ == '__main__':
    runner = StabilityTestRunner('test_config.json')
    runner.run_test()
```

---

## 📈 结果分析和报告

### 报告模板

```markdown
# VisionForge Pro - 72小时稳定性测试报告

## 测试概要
- **测试版本**: v1.2.0
- **开始时间**: 2025-12-20 10:00:00
- **结束时间**: 2025-12-23 10:00:00
- **实际时长**: 72小时
- **测试环境**: Windows 10 Pro, Intel i7-9700K, 32GB RAM

## 测试结果

### ✅ 通过标准
| 指标 | 目标值 | 实际值 | 状态 |
|------|--------|--------|------|
| 内存增长 | <100MB/72h | 45MB | ✅ PASS |
| CPU使用率 | <60% | 52% | ✅ PASS |
| 池命中率 | >80% | 87.3% | ✅ PASS |
| 崩溃次数 | 0 | 0 | ✅ PASS |
| 响应时间 | <100ms | 78ms | ✅ PASS |

### 📊 详细指标

#### 内存趋势
![内存趋势图](memory_trend.png)

- 初始内存: 425MB
- 最终内存: 470MB
- 峰值内存: 485MB
- 增长率: 0.62MB/h

#### CPU使用率
![CPU使用率](cpu_usage.png)

- 平均: 52%
- 峰值: 78%
- 最低: 15%

#### 场景执行统计
| 场景 | 执行次数 | 成功率 | 平均时间 |
|------|----------|--------|----------|
| 高频批处理 | 288 | 100% | 13.2s |
| 多相机采集 | 72h连续 | 99.98% | 48ms/帧 |
| 混合压力 | 72 | 100% | 28.5s |

### 🐛 问题列表

#### 发现的问题
1. **警告**: 第48小时出现1次内存池碎片率>15%
   - 影响: 轻微
   - 解决方案: 已记录,建议添加碎片整理机制

2. **信息**: 偶发CPU使用率峰值(78%)
   - 影响: 无
   - 原因: 批量处理期间正常波动

#### 未发现的问题
- ✅ 无内存泄漏
- ✅ 无死锁
- ✅ 无崩溃
- ✅ 无数据损坏

### 🎯 MTBF评估
- 观测运行时间: 72小时
- 故障次数: 0
- **MTBF**: >72小时 (符合工业标准)

### 📝 结论
VisionForge Pro v1.2.0通过72小时稳定性测试,所有关键指标达标。
系统可用于7x24工业生产环境部署。

### 📋 建议
1. 建议添加内存池碎片整理机制
2. 建议优化批处理期间的CPU峰值
3. 建议继续进行168小时(1周)长期测试

---
**测试工程师**: VisionForge QA Team
**审核**: Claude Sonnet 4.5
**日期**: 2025-12-23
```

---

## 🛠️ 实施计划

### 第1阶段: 框架开发 (2天)
- [ ] 创建StabilityTestFramework类
- [ ] 实现SystemResourceMonitor
- [ ] 实现PoolPerformanceMonitor
- [ ] 编写Python自动化脚本

### 第2阶段: 场景实现 (2天)
- [ ] 实现HighFrequencyBatchScenario
- [ ] 实现MultiCameraContinuousScenario
- [ ] 实现MixedWorkloadScenario
- [ ] 添加场景配置文件

### 第3阶段: 集成测试 (1天)
- [ ] 单元测试各监控组件
- [ ] 集成测试完整流程
- [ ] 验证报告生成

### 第4阶段: 执行测试 (3天)
- [ ] 执行72小时测试
- [ ] 实时监控和调整
- [ ] 生成最终报告

---

## 📁 文件结构

```
VisionForge Pro/
├── test/
│   ├── stability/
│   │   ├── StabilityTestFramework.h/cpp
│   │   ├── SystemResourceMonitor.h/cpp
│   │   ├── PoolPerformanceMonitor.h/cpp
│   │   ├── scenarios/
│   │   │   ├── HighFrequencyBatchScenario.h/cpp
│   │   │   ├── MultiCameraContinuousScenario.h/cpp
│   │   │   └── MixedWorkloadScenario.h/cpp
│   │   └── test_stability.cpp
│   └── scripts/
│       ├── run_72h_test.py
│       ├── analyze_results.py
│       └── generate_report.py
├── docs/
│   └── STABILITY_TEST_72H_PLAN.md (本文档)
└── config/
    └── stability_test_config.json
```

---

**最后更新**: 2025-12-20
**状态**: 设计完成,待实施
