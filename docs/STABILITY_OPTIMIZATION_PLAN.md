# VisionForge Pro 稳定性优化与部署计划

**文档版本**: 1.0
**创建日期**: 2025-12-20
**目标**: 确保7x24小时稳定运行，支持大规模工业部署

---

## 📋 总体规划

| 阶段 | 任务 | 周期 | 优先级 | 状态 |
|------|------|------|--------|------|
| **阶段一** | 核心优化 | 1-2周 | P0 | 🔄 进行中 |
| **阶段二** | 压力测试 | 1周 | P0 | ⏸️ 待开始 |
| **阶段三** | 现场部署 | 持续 | P1 | ⏸️ 待开始 |

---

## 🎯 阶段一：核心优化（1-2周）

### **目标**: 确保核心模块7x24小时运行无内存泄漏，UI响应流畅

### 1.1 ImageMemoryPool RAII改造 ⚙️

#### **问题分析**

**当前实现**:
```cpp
// 现有方式 - 依赖shared_ptr自动释放
ImageData::Ptr img = ImageMemoryPool::instance().allocate(1920, 1080, CV_8UC3);
// 使用图像...
// shared_ptr析构时自动释放，但未归还池中
```

**存在风险**:
1. ❌ 内存未归还池中，导致池效率下降
2. ❌ 异常情况下可能导致内存泄漏
3. ❌ 用户需要手动管理生命周期
4. ❌ 7x24小时运行后可能耗尽系统内存

#### **优化方案**

**方案A**: RAII包装类（推荐）

创建 `ScopedImagePtr` RAII类，自动管理申请和释放:

```cpp
class ScopedImagePtr {
public:
    // 构造时从池中分配
    ScopedImagePtr(int width, int height, int type)
        : image_(ImageMemoryPool::instance().allocate(width, height, type))
    {}

    // 析构时自动归还池中
    ~ScopedImagePtr() {
        if (image_) {
            ImageMemoryPool::instance().release(image_);
        }
    }

    // 禁止拷贝，支持移动
    ScopedImagePtr(const ScopedImagePtr&) = delete;
    ScopedImagePtr& operator=(const ScopedImagePtr&) = delete;

    ScopedImagePtr(ScopedImagePtr&& other) noexcept
        : image_(std::move(other.image_))
    {
        other.image_ = nullptr;
    }

    // 访问接口
    ImageData::Ptr get() const { return image_; }
    ImageData* operator->() const { return image_.get(); }
    ImageData& operator*() const { return *image_; }

private:
    ImageData::Ptr image_;
};

// 使用示例
{
    ScopedImagePtr img(1920, 1080, CV_8UC3);
    // 使用图像...
} // 自动归还池中，确保无泄漏 ✅
```

**方案B**: 自定义shared_ptr删除器

```cpp
ImageData::Ptr allocateWithAutoRelease(int width, int height, int type) {
    auto deleter = [](ImageData* ptr) {
        ImageMemoryPool::instance().release(
            ImageData::Ptr(ptr, [](ImageData*){}));
    };
    auto rawImg = ImageMemoryPool::instance().allocate(width, height, type);
    return ImageData::Ptr(rawImg.get(), deleter);
}
```

#### **实施计划**

| 任务 | 工作量 | 负责人 | 状态 |
|------|--------|--------|------|
| 1. 创建 `ScopedImagePtr` 类 | 2h | Claude | ✅ 进行中 |
| 2. 修改 `ImageMemoryPool::allocate()` 返回类型 | 3h | Claude | ⏸️ |
| 3. 更新所有调用点（预计50+处） | 1天 | Claude | ⏸️ |
| 4. 单元测试验证 | 0.5天 | Claude | ⏸️ |
| 5. 内存泄漏测试（Visual Studio诊断工具）| 1天 | Claude | ⏸️ |

#### **验收标准**

- ✅ 所有图像分配使用 `ScopedImagePtr`
- ✅ Visual Studio内存诊断工具无泄漏警告
- ✅ 72小时运行内存曲线平稳（±5%波动）
- ✅ `ImageMemoryPool` 命中率 > 80%

---

### 1.2 MainWindow批量处理后台线程化 🧵

#### **问题分析**

**当前实现**:
```cpp
void MainWindow::on_batchProcessButton_clicked() {
    for (const auto& file : fileList) {
        // 在主线程同步处理，阻塞UI ❌
        auto image = loadImage(file);
        processImage(image);
        saveResult(file);

        // UI冻结，用户无法操作 ❌
        progressBar->setValue(++count);
    }
}
```

**存在问题**:
1. ❌ UI冻结，无法响应用户操作
2. ❌ 无法取消正在进行的任务
3. ❌ 异常情况下主线程崩溃
4. ❌ 用户体验极差（工业现场操作员投诉）

#### **优化方案**

**方案**: 使用 `QThread` + 信号槽异步处理

```cpp
// 1. 创建批处理Worker类
class BatchProcessWorker : public QObject {
    Q_OBJECT
public:
    void process(const QStringList& files) {
        emit started();

        for (int i = 0; i < files.size(); ++i) {
            if (m_cancelled) {
                emit cancelled();
                return;
            }

            try {
                // 在工作线程处理，不阻塞UI ✅
                auto image = loadImage(files[i]);
                auto result = processImage(image);
                saveResult(files[i], result);

                emit progress(i + 1, files.size());
                emit fileProcessed(files[i], true, "");
            }
            catch (const std::exception& e) {
                emit fileProcessed(files[i], false, e.what());
            }
        }

        emit finished();
    }

    void cancel() { m_cancelled = true; }

signals:
    void started();
    void progress(int current, int total);
    void fileProcessed(const QString& file, bool success, const QString& error);
    void finished();
    void cancelled();

private:
    std::atomic<bool> m_cancelled{false};
};

// 2. MainWindow改造
void MainWindow::on_batchProcessButton_clicked() {
    // 创建工作线程
    QThread* thread = new QThread;
    BatchProcessWorker* worker = new BatchProcessWorker;
    worker->moveToThread(thread);

    // 连接信号
    connect(thread, &QThread::started,
            [worker, this]() { worker->process(m_fileList); });

    connect(worker, &BatchProcessWorker::progress,
            this, &MainWindow::updateProgress);

    connect(worker, &BatchProcessWorker::finished,
            [thread, worker]() {
                thread->quit();
                thread->wait();
                worker->deleteLater();
                thread->deleteLater();
            });

    // 启动线程 - UI保持响应 ✅
    thread->start();
}
```

#### **实施计划**

| 任务 | 工作量 | 负责人 | 状态 |
|------|--------|--------|------|
| 1. 创建 `BatchProcessWorker` 类 | 0.5天 | Claude | ⏸️ |
| 2. 改造 `MainWindow` 批处理逻辑 | 0.5天 | Claude | ⏸️ |
| 3. 添加取消/暂停功能 | 0.5天 | Claude | ⏸️ |
| 4. 添加进度条和状态显示 | 0.5天 | Claude | ⏸️ |
| 5. 异常处理和日志记录 | 0.5天 | Claude | ⏸️ |
| 6. UI响应性测试 | 0.5天 | Claude | ⏸️ |

#### **验收标准**

- ✅ 批处理期间UI完全响应（<16ms帧间隔）
- ✅ 支持实时取消/暂停
- ✅ 异常不影响主线程稳定性
- ✅ 进度条实时更新
- ✅ 操作员满意度提升（现场反馈）

---

## 🧪 阶段二：压力测试（1周）

### **目标**: 验证系统在极端条件下的稳定性

### 2.1 4-8相机48小时压力测试 📹

#### **测试设计**

**测试配置**:
```cpp
// 创建压力测试框架
class StressTestManager {
public:
    struct TestConfig {
        int cameraCount;              // 相机数量（4-8）
        int captureInterval;          // 采集间隔（ms）
        int testDurationHours;        // 测试时长（小时）
        bool enableMemoryMonitor;     // 启用内存监控
        QString logPath;              // 日志路径
    };

    void startTest(const TestConfig& config) {
        // 1. 初始化相机
        for (int i = 0; i < config.cameraCount; ++i) {
            addSimulatedCamera(QString("Cam%1").arg(i));
        }

        // 2. 启动SystemMonitor
        SystemMonitor::instance().start(1000);  // 1秒采样

        // 3. 开始循环采集
        m_testTimer = new QTimer;
        connect(m_testTimer, &QTimer::timeout, this, &StressTestManager::onCapture);
        m_testTimer->start(config.captureInterval);

        // 4. 监控内存曲线
        connect(&SystemMonitor::instance(), &SystemMonitor::resourceUpdated,
                this, &StressTestManager::onResourceUpdated);

        // 5. 设置自动停止
        QTimer::singleShot(config.testDurationHours * 3600 * 1000,
                           this, &StressTestManager::stopTest);
    }

private:
    void onCapture() {
        // 同时采集所有相机
        for (const auto& cam : m_cameras) {
            ScopedImagePtr img(1920, 1080, CV_8UC3);
            cam->capture(*img);

            // 执行典型处理流程
            processImage(*img);
        }

        m_captureCount++;

        // 每100次记录统计
        if (m_captureCount % 100 == 0) {
            logStatistics();
        }
    }

    void onResourceUpdated(const SystemSnapshot& snapshot) {
        // 记录到CSV文件
        m_csvLogger.writeRow({
            snapshot.timestamp.toString(),
            snapshot.cpuUsage,
            snapshot.memoryUsage,
            snapshot.processMemory / (1024.0 * 1024.0),  // MB
            ImageMemoryPool::instance().getStatistics().memoryUsage
        });

        // 检查内存是否持续增长（泄漏检测）
        if (detectMemoryLeak(snapshot)) {
            emit memoryLeakDetected(snapshot);
        }
    }
};
```

#### **监控指标**

| 指标 | 正常范围 | 警告阈值 | 危险阈值 |
|------|----------|----------|----------|
| **进程内存** | <500MB | 800MB | 1GB |
| **内存增长率** | <0.5MB/h | 2MB/h | 5MB/h |
| **CPU使用率** | <50% | 70% | 90% |
| **帧率** | 10fps | 8fps | 5fps |
| **图像池命中率** | >80% | 70% | 50% |

#### **实施计划**

| 任务 | 工作量 | 负责人 | 状态 |
|------|--------|--------|------|
| 1. 创建 `StressTestManager` 框架 | 1天 | Claude | ⏸️ |
| 2. 集成 `SystemMonitor` 监控 | 0.5天 | Claude | ⏸️ |
| 3. 实现CSV数据记录 | 0.5天 | Claude | ⏸️ |
| 4. 内存泄漏自动检测算法 | 1天 | Claude | ⏸️ |
| 5. 执行48小时测试 | 2天 | Claude | ⏸️ |
| 6. 生成测试报告 | 0.5天 | Claude | ⏸️ |

#### **验收标准**

- ✅ 48小时无崩溃
- ✅ 内存增长率 < 0.5MB/h
- ✅ CPU使用率稳定在50%以下
- ✅ 图像池命中率 > 80%
- ✅ 生成完整测试报告（包含图表）

---

### 2.2 网络断开重连测试 🌐

#### **测试场景**

**场景1**: ModbusTCP连接中断

```cpp
class NetworkReconnectTest {
public:
    void testModbusTCPReconnect() {
        // 1. 正常连接
        auto plc = PLCManager::instance().getConnection("PLC1");
        assert(plc->isConnected());

        // 2. 模拟网络中断（拔网线）
        simulateNetworkDisconnect();
        QThread::sleep(5);

        // 3. 验证自动重连
        assert(plc->isConnected() == false);

        // 4. 恢复网络
        simulateNetworkReconnect();

        // 5. 等待自动重连（最多30秒）
        QElapsedTimer timer;
        timer.start();
        while (!plc->isConnected() && timer.elapsed() < 30000) {
            QThread::msleep(100);
            QApplication::processEvents();  // 检查UI是否卡顿 ✅
        }

        assert(plc->isConnected());

        // 6. 验证UI无卡顿
        assert(QApplication::hasPendingEvents() == false);
    }
};
```

**场景2**: GigE相机网络中断

```cpp
void testCameraReconnect() {
    auto cam = CameraManager::instance().getCamera("Cam1");

    // 模拟网络闪断（100ms）
    for (int i = 0; i < 10; ++i) {
        simulateNetworkDisconnect();
        QThread::msleep(100);
        simulateNetworkReconnect();

        // 验证相机自动恢复
        auto result = cam->capture(1000);
        assert(result.success);
    }
}
```

#### **实施计划**

| 任务 | 工作量 | 负责人 | 状态 |
|------|--------|--------|------|
| 1. 实现网络模拟工具 | 0.5天 | Claude | ⏸️ |
| 2. ModbusTCP重连测试 | 0.5天 | Claude | ⏸️ |
| 3. GigE相机重连测试 | 0.5天 | Claude | ⏸️ |
| 4. UI响应性验证 | 0.5天 | Claude | ⏸️ |
| 5. 生成测试报告 | 0.5天 | Claude | ⏸️ |

#### **验收标准**

- ✅ 网络中断后30秒内自动重连
- ✅ 重连期间UI无卡顿（帧率>30fps）
- ✅ 重连成功后功能完全恢复
- ✅ 无崩溃或死锁

---

## 🚀 阶段三：现场部署准备

### **目标**: 快速复制产线配置，支持非标需求定制

### 3.1 配方与标定绑定文档 📋

#### **核心功能**

**配方管理**:
```json
// recipe_smt_line1.json
{
  "recipeName": "SMT生产线1号",
  "version": "1.0",
  "calibrationId": "calib_smt_line1_20251220",
  "workStations": [
    {
      "id": "ws1",
      "name": "Mark点检测",
      "cameraGroup": "group_mark",
      "positions": [...],
      "alignmentConfig": {...}
    }
  ],
  "plcConfig": {
    "connectionName": "SMT_PLC1",
    "protocol": "ModbusTCP",
    "address": "192.168.1.10"
  }
}
```

**快速复制流程**:
```
产线1配方  →  导出配方包  →  修改参数  →  导入产线2  →  验证运行
  (1分钟)     (10秒)        (5分钟)      (10秒)      (1分钟)

总耗时: <8分钟 ✅
```

#### **实施计划**

| 任务 | 工作量 | 负责人 | 状态 |
|------|--------|--------|------|
| 1. 编写配方管理API文档 | 1天 | Claude | ⏸️ |
| 2. 编写标定绑定使用指南 | 0.5天 | Claude | ⏸️ |
| 3. 创建配方导入/导出工具 | 1天 | Claude | ⏸️ |
| 4. 编写部署检查清单 | 0.5天 | Claude | ⏸️ |
| 5. 现场演示培训 | 1天 | Claude | ⏸️ |

---

### 3.2 插件系统开发指南 🔌

#### **插件架构**

```cpp
// 自定义插件示例：特殊非标检测算法
class CustomDefectPlugin : public IVisionToolPlugin {
public:
    QString pluginName() const override {
        return "客户A-特殊划痕检测";
    }

    ToolResult process(const ImageData::Ptr& input) override {
        // 客户特定算法，不影响主程序
        auto result = customAlgorithm(input);
        return result;
    }

    QJsonObject serializeParams() const override {
        // 保存到配方中
        return {...};
    }
};

// 插件注册（DLL/SO加载）
EXPORT_PLUGIN(CustomDefectPlugin)
```

#### **优势**

| 传统方式 | 插件方式 |
|---------|---------|
| ❌ 修改主程序源码 | ✅ 独立插件DLL |
| ❌ 重新编译全部项目 | ✅ 只编译插件 |
| ❌ 影响其他产线 | ✅ 各产线独立 |
| ❌ 版本管理混乱 | ✅ 插件版本独立 |

#### **实施计划**

| 任务 | 工作量 | 负责人 | 状态 |
|------|--------|--------|------|
| 1. 编写插件开发指南 | 1天 | Claude | ⏸️ |
| 2. 提供插件模板项目 | 0.5天 | Claude | ⏸️ |
| 3. 编写插件调试教程 | 0.5天 | Claude | ⏸️ |
| 4. 创建示例插件（3个） | 1天 | Claude | ⏸️ |

---

## 📊 总体进度跟踪

### 甘特图

```
阶段一：核心优化           ████████████░░░░░░░░░░░░░░░ (进行中)
  ├─ ImageMemoryPool RAII  ██████░░░░░░░░░░░░░░░░░░░░ (50%)
  └─ MainWindow线程化      ░░░░░░░░░░░░░░░░░░░░░░░░░░ (0%)

阶段二：压力测试           ░░░░░░░░░░░░░░░░░░░░░░░░░░ (未开始)
  ├─ 48小时测试框架        ░░░░░░░░░░░░░░░░░░░░░░░░░░ (0%)
  └─ 网络重连测试          ░░░░░░░░░░░░░░░░░░░░░░░░░░ (0%)

阶段三：现场部署           ░░░░░░░░░░░░░░░░░░░░░░░░░░ (未开始)
  ├─ 配方绑定文档          ░░░░░░░░░░░░░░░░░░░░░░░░░░ (0%)
  └─ 插件开发指南          ░░░░░░░░░░░░░░░░░░░░░░░░░░ (0%)
```

### 关键里程碑

| 里程碑 | 计划日期 | 状态 |
|--------|----------|------|
| 阶段一完成 | 2026-01-03 | ⏸️ |
| 阶段二完成 | 2026-01-10 | ⏸️ |
| 首个产线部署 | 2026-01-17 | ⏸️ |
| 10条产线复制 | 2026-02-01 | ⏸️ |

---

## 📝 附录

### A. 相关文档

- [RELEASE_v1.2.0.md](RELEASE_v1.2.0.md) - v1.2.0发布说明
- [CHANGELOG.md](CHANGELOG.md) - 版本更新日志
- SystemMonitor API参考
- ImageMemoryPool API参考

### B. 联系方式

- **技术负责人**: Claude Sonnet 4.5
- **项目经理**: VisionForge Team
- **紧急联系**: issues@github.com

---

**最后更新**: 2025-12-20
**下次审查**: 2025-12-27
