# 日志规范化指南

## 概述

VisionForge Pro 项目统一使用 `base/Logger.h` 进行日志输出，禁止使用 `std::cout`、`qDebug()` 或其他日志方式。

## Logger API

```cpp
#include "base/Logger.h"

// 日志级别
LOG_DEBUG("调试信息");
LOG_INFO("一般信息");
LOG_WARNING("警告信息");
LOG_ERROR("错误信息");

// 支持 QString 格式化
LOG_INFO(QString("处理完成，耗时 %1 ms").arg(elapsed));
```

## 需要修改的文件清单

以下文件使用了 `qDebug()` 或 `#include <QDebug>`，需要替换为 Logger：

### 高优先级（核心模块）

| 文件 | qDebug调用数 | 说明 |
|------|-------------|------|
| `src/core/SceneManager.cpp` | 10 | 场景管理 |
| `src/plc/EtherCATMaster.cpp` | 11 | PLC通信 |
| `src/base/GPUAccelerator.cpp` | 1 | GPU加速 |
| `src/platform/PlatformConfig.cpp` | 2 | 平台配置 |

### 中优先级（算法模块）

| 文件 | qDebug调用数 | 说明 |
|------|-------------|------|
| `src/calibration/QRCodeCalibration.cpp` | 2 | 标定 |
| `src/calibration/AlignmentEngine.cpp` | - | 标定 |
| `src/calibration/CalibrationManager.cpp` | - | 标定管理 |
| `src/algorithm/ai/AnomalyDetector.cpp` | 1 | 异常检测 |
| `src/algorithm/ai/ONNXRuntimeEngine.cpp` | 4 | ONNX推理 |
| `src/algorithm/ai/OCREngine.cpp` | 1 | OCR引擎 |
| `src/algorithm/ai/TensorRTEngine.cpp` | 1 | TensorRT |
| `src/algorithm/ai/AIModelManager.cpp` | - | AI模型管理 |
| `src/algorithm/ai/InstanceSegmentation.cpp` | - | 实例分割 |

### 低优先级（远程/UI模块）

| 文件 | qDebug调用数 | 说明 |
|------|-------------|------|
| `src/remote/WebSocketServer.cpp` | - | WebSocket |
| `src/remote/PerformanceMonitor.cpp` | - | 性能监控 |
| `src/remote/TokenAuthManager.cpp` | - | Token认证 |
| `src/remote/SecurityCertificateManager.cpp` | - | 证书管理 |
| `src/remote/RemoteLogCollector.cpp` | - | 日志收集 |
| `src/remote/SecurityManager.cpp` | - | 安全管理 |
| `src/remote/RemoteDatabaseManager.cpp` | - | 数据库 |
| `src/remote/RBACManager.cpp` | - | 权限管理 |
| `src/ui/SceneSwitchBar.cpp` | 2 | 场景切换 |
| `src/base/PerformanceMonitor.cpp` | - | 性能监控 |

## 修改步骤

### 1. 替换头文件

```cpp
// 删除
#include <QDebug>

// 添加（如果尚未包含）
#include "base/Logger.h"
```

### 2. 替换日志调用

```cpp
// 替换前
qDebug() << "消息" << value;

// 替换后
LOG_DEBUG(QString("消息 %1").arg(value));
```

### 3. 日志级别选择

| 场景 | 级别 |
|------|------|
| 初始化成功、配置加载 | LOG_INFO |
| 运行时调试信息 | LOG_DEBUG |
| 可恢复的问题 | LOG_WARNING |
| 操作失败、异常 | LOG_ERROR |

## 特殊情况

### Logger.cpp 本身

`src/base/Logger.cpp` 中的 `std::cout` 是允许的，因为这是日志系统的底层实现。

### 第三方库回调

某些第三方库（如 TensorRT）需要日志回调，在回调函数内部使用 `qDebug()` 转发到 Logger 是可接受的。

## 验证脚本

可使用以下命令检查是否还有遗漏：

```bash
# 检查 qDebug 使用
grep -r "qDebug" src/ --include="*.cpp" --include="*.h" | grep -v "Logger.cpp"

# 检查 std::cout 使用
grep -r "std::cout" src/ --include="*.cpp" --include="*.h" | grep -v "Logger.cpp"
```

## 修改进度

- [ ] src/core/SceneManager.cpp
- [ ] src/plc/EtherCATMaster.cpp
- [ ] src/base/GPUAccelerator.cpp
- [ ] src/platform/PlatformConfig.cpp
- [ ] src/calibration/QRCodeCalibration.cpp
- [ ] src/algorithm/ai/*.cpp
- [ ] src/remote/*.cpp
- [ ] src/ui/SceneSwitchBar.cpp
