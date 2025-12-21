# VisionForge Pro 远程诊断系统 - 实现总结

## 📋 目录
- [实现概述](#实现概述)
- [已实现组件](#已实现组件)
- [编译验证](#编译验证)
- [使用指南](#使用指南)
- [API参考](#api参考)
- [后续扩展](#后续扩展)

---

## 实现概述

### 完成状态
✅ **核心后端组件已完成** (4/4)
- RemoteProtocol - 消息协议定义
- WebSocketServer - WebSocket服务器
- PerformanceMonitor - 性能监控器
- RemoteLogCollector - 日志收集器

⏸️ **前端组件待开发**
- Web监控界面 (Vue.js + ECharts)
- 性能数据可视化
- 日志查看器UI

### 技术栈
- **后端**: Qt 6.9.3 + Qt WebSockets
- **协议**: WebSocket + JSON
- **系统监控**: Windows API (CPU/内存/线程)
- **编译器**: MSVC 2022 (C++17)

---

## 已实现组件

### 1. RemoteProtocol - 消息协议 📡

**文件位置**:
- [include/remote/RemoteProtocol.h](../include/remote/RemoteProtocol.h)
- [src/remote/RemoteProtocol.cpp](../src/remote/RemoteProtocol.cpp)

**核心功能**:
```cpp
// 消息类型
enum class MessageType {
    ClientHello, ServerHello,           // 握手
    Subscribe, Unsubscribe,             // 订阅
    PerformanceUpdate, LogEntry,        // 数据推送
    SystemStatus,                       // 状态
    SendCommand, CommandResult,         // 命令
    QueryHistory, HistoryData,          // 查询
    Ping, Pong, Error                   // 心跳和错误
};

// 性能指标数据结构
struct PerformanceMetrics {
    double cpuUsage;
    qint64 memoryUsedMB;
    double fps;
    double avgLatencyMs;
    int errorCount;
    QDateTime timestamp;
};

// 日志条目数据结构
struct LogEntryData {
    QDateTime timestamp;
    LogLevel level;
    QString category;
    QString message;
    QString file;
    int line;
};
```

**消息格式示例**:
```json
{
  "type": "performance_update",
  "timestamp": "2025-12-20T22:00:00Z",
  "data": {
    "cpu_usage": 42.5,
    "memory_used_mb": 1024,
    "fps": 30.2,
    "avg_latency_ms": 15.3,
    "error_count": 0
  }
}
```

---

### 2. WebSocketServer - WebSocket服务器 🌐

**文件位置**:
- [include/remote/WebSocketServer.h](../include/remote/WebSocketServer.h)
- [src/remote/WebSocketServer.cpp](../src/remote/WebSocketServer.cpp)

**核心功能**:
- ✅ 多客户端连接管理
- ✅ 消息广播和单播
- ✅ 自动心跳检测 (可配置间隔)
- ✅ 订阅/取消订阅机制
- ✅ 连接状态监控

**使用示例**:
```cpp
#include "remote/WebSocketServer.h"

using namespace VisionForge::Remote;

// 创建服务器
auto* server = new WebSocketServer(this);

// 配置服务器
ServerConfig config;
config.port = 8765;
config.maxConnections = 100;
config.heartbeatIntervalMs = 10000;  // 10秒心跳
config.heartbeatTimeoutMs = 30000;   // 30秒超时

// 启动服务器
if (server->start(config)) {
    qInfo() << "WebSocket服务器启动成功，端口:" << config.port;
}

// 连接信号
connect(server, &WebSocketServer::clientConnected,
    [](const QString& sessionId, const QString& clientId) {
        qInfo() << "新客户端连接:" << clientId;
    });

// 推送性能指标
PerformanceMetrics metrics;
metrics.cpuUsage = 42.5;
metrics.memoryUsedMB = 1024;
metrics.fps = 30.2;
metrics.timestamp = QDateTime::currentDateTime();

server->pushPerformanceMetrics(metrics);

// 广播消息给所有订阅性能指标的客户端
server->broadcastToSubscribers(
    SubscriptionType::Performance,
    RemoteProtocol::createPerformanceUpdate(metrics)
);
```

**配置参数**:
```cpp
struct ServerConfig {
    quint16 port = 8765;                    // 监听端口
    int maxConnections = 100;               // 最大连接数
    int heartbeatIntervalMs = 10000;        // 心跳间隔 (ms)
    int heartbeatTimeoutMs = 30000;         // 心跳超时 (ms)
    int maxPingFailCount = 3;               // 最大心跳失败次数
    bool enableSsl = false;                 // 是否启用SSL
    bool requireAuth = true;                // 是否需要认证
};
```

---

### 3. PerformanceMonitor - 性能监控器 📊

**文件位置**:
- [include/remote/PerformanceMonitor.h](../include/remote/PerformanceMonitor.h)
- [src/remote/PerformanceMonitor.cpp](../src/remote/PerformanceMonitor.cpp)

**核心功能**:
- ✅ 实时CPU使用率采集 (Windows API)
- ✅ 内存使用情况监控
- ✅ 处理帧率统计 (FPS)
- ✅ 延迟计算 (平均/当前)
- ✅ 错误计数统计
- ✅ 移动平均值计算
- ✅ 性能异常告警

**使用示例**:
```cpp
#include "remote/PerformanceMonitor.h"

using namespace VisionForge::Remote;

// 创建性能监控器
auto* monitor = new PerformanceMonitor(this);

// 配置
monitor->setSamplingInterval(1000);  // 每秒采样一次
monitor->setStatWindowSize(60);      // 60秒统计窗口

// 启动监控
monitor->start();

// 连接信号
connect(monitor, &PerformanceMonitor::metricsUpdated,
    [](const PerformanceMetrics& metrics) {
        qDebug() << "CPU:" << metrics.cpuUsage << "%"
                 << "Memory:" << metrics.memoryUsedMB << "MB"
                 << "FPS:" << metrics.fps;
    });

// 性能异常告警
connect(monitor, &PerformanceMonitor::performanceWarning,
    [](const QString& message) {
        qWarning() << "性能警告:" << message;
    });

// 记录帧处理
monitor->recordFrameStart();
// ... 处理图像 ...
monitor->recordFrameEnd(true);  // true表示成功

// 获取当前统计
PerformanceStats stats = monitor->currentStats();
qDebug() << "平均FPS:" << stats.avgFPS;
qDebug() << "平均延迟:" << stats.avgLatencyMs << "ms";

// 获取历史数据（最近60条）
QList<PerformanceStats> history = monitor->history(60);
```

**监控指标**:
```cpp
struct PerformanceStats {
    // CPU和内存
    double cpuUsagePercent;             // CPU使用率 (%)
    qint64 memoryUsedMB;                // 已用内存 (MB)
    qint64 memoryTotalMB;               // 总内存 (MB)
    int threadCount;                    // 线程数

    // 处理性能
    double currentFPS;                  // 当前帧率
    double avgFPS;                      // 平均帧率
    double currentLatencyMs;            // 当前延迟 (ms)
    double avgLatencyMs;                // 平均延迟 (ms)
    qint64 totalFramesProcessed;        // 总处理帧数

    // 错误统计
    int errorCount;                     // 错误计数
    int warningCount;                   // 警告计数
};
```

**性能告警阈值**:
- CPU使用率 > 90% → 告警
- 内存使用率 > 90% → 告警
- 平均延迟 > 100ms → 告警
- 帧率 < 10 FPS → 告警

---

### 4. RemoteLogCollector - 日志收集器 📝

**文件位置**:
- [include/remote/RemoteLogCollector.h](../include/remote/RemoteLogCollector.h)
- [src/remote/RemoteLogCollector.cpp](../src/remote/RemoteLogCollector.cpp)

**核心功能**:
- ✅ 日志分级管理 (Trace/Debug/Info/Warn/Error/Fatal)
- ✅ 日志分类过滤
- ✅ 本地文件存储 (UTF-8编码)
- ✅ 内存缓存队列 (可配置大小)
- ✅ 日志搜索和查询
- ✅ 自动日志轮转 (10MB)
- ✅ 远程推送功能

**使用示例**:
```cpp
#include "remote/RemoteLogCollector.h"

using namespace VisionForge::Remote;

// 创建日志收集器
auto* logCollector = new RemoteLogCollector(this);

// 初始化（指定日志文件路径）
logCollector->initialize("./logs/visionforge.log");

// 配置过滤器
LogFilter filter;
filter.minLevel = LogLevel::Info;           // 最低记录INFO级别
filter.maxCacheSize = 10000;                // 缓存1万条日志
filter.enableFileLog = true;                // 启用文件日志
filter.enableRemoteLog = true;              // 启用远程推送

logCollector->setFilter(filter);

// 记录日志
logCollector->info("System", "应用程序启动成功");
logCollector->warn("Camera", "相机连接延迟较高", __FILE__, __LINE__);
logCollector->error("Processing", "图像处理失败", __FILE__, __LINE__);

// 便捷方法
logCollector->trace("Debug", "调试信息");
logCollector->debug("Module", "模块加载完成");
logCollector->fatal("System", "严重错误，应用即将退出");

// 连接信号
connect(logCollector, &RemoteLogCollector::errorOccurred,
    [](const LogEntryData& entry) {
        qCritical() << "错误日志:" << entry.message;
    });

// 查询日志
QList<LogEntryData> recentLogs = logCollector->recentLogs(100);  // 最近100条
QList<LogEntryData> errorLogs = logCollector->getLogsByLevel(LogLevel::Error, 50);
QList<LogEntryData> searchResults = logCollector->searchLogs("连接失败", 20);

// 获取统计信息
QMap<LogLevel, int> stats = logCollector->getLogStatistics();
qDebug() << "错误数量:" << stats[LogLevel::Error];
qDebug() << "警告数量:" << stats[LogLevel::Warn];
```

**日志级别**:
```cpp
enum class LogLevel {
    Trace = 0,      // 详细调试信息
    Debug = 1,      // 调试信息
    Info = 2,       // 一般信息
    Warn = 3,       // 警告信息
    Error = 4,      // 错误信息
    Fatal = 5       // 严重错误
};
```

**日志文件格式**:
```
[2025-12-20 22:15:30.123] [INFO] [System] 应用程序启动成功
[2025-12-20 22:15:31.456] [WARN] [Camera] [CameraManager.cpp:42] 相机连接延迟较高
[2025-12-20 22:15:32.789] [ERROR] [Processing] [ImageProcessor.cpp:156] 图像处理失败
```

---

## 编译验证

### 编译结果
```bash
✅ VisionForgeRemote.lib 编译成功
  - RemoteProtocol.cpp ✅
  - WebSocketServer.cpp ✅
  - PerformanceMonitor.cpp ✅
  - RemoteLogCollector.cpp ✅
```

### 编译配置
```cmake
# src/remote/CMakeLists.txt
add_library(VisionForgeRemote STATIC
    RemoteProtocol.cpp
    WebSocketServer.cpp
    PerformanceMonitor.cpp
    RemoteLogCollector.cpp
)

target_link_libraries(VisionForgeRemote PUBLIC
    Qt6::Core
    Qt6::WebSockets
    Qt6::Network
)
```

### 依赖项
- Qt 6.9.3 (Core, WebSockets, Network)
- Windows API (psapi.lib, tlhelp32.h)
- C++17标准库

---

## 使用指南

### 快速开始

#### 1. 创建完整的远程诊断系统

```cpp
#include <QCoreApplication>
#include "remote/WebSocketServer.h"
#include "remote/PerformanceMonitor.h"
#include "remote/RemoteLogCollector.h"

using namespace VisionForge::Remote;

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    // 1. 创建WebSocket服务器
    auto* server = new WebSocketServer(&app);

    ServerConfig config;
    config.port = 8765;
    server->start(config);

    // 2. 创建性能监控器
    auto* perfMonitor = new PerformanceMonitor(&app);
    perfMonitor->start(1000);  // 每秒采样

    // 3. 创建日志收集器
    auto* logCollector = new RemoteLogCollector(&app);
    logCollector->initialize();

    // 4. 连接信号：性能指标推送
    QObject::connect(perfMonitor, &PerformanceMonitor::metricsUpdated,
        server, &WebSocketServer::pushPerformanceMetrics);

    // 5. 连接信号：日志推送
    QObject::connect(logCollector, &RemoteLogCollector::logAdded,
        server, &WebSocketServer::pushLogEntry);

    // 6. 启动消息
    logCollector->info("System", "远程诊断系统已启动");

    return app.exec();
}
```

#### 2. 客户端连接示例 (JavaScript)

```javascript
// 连接到WebSocket服务器
const ws = new WebSocket('ws://localhost:8765');

// 连接成功
ws.onopen = () => {
    console.log('已连接到VisionForge远程诊断系统');

    // 发送握手消息
    ws.send(JSON.stringify({
        type: 'client_hello',
        timestamp: new Date().toISOString(),
        data: {
            client_id: 'web-monitor-1',
            version: '1.0.0'
        }
    }));
};

// 接收消息
ws.onmessage = (event) => {
    const message = JSON.parse(event.data);

    switch(message.type) {
        case 'server_hello':
            console.log('握手成功，会话ID:', message.data.session_id);
            // 订阅性能指标
            ws.send(JSON.stringify({
                type: 'subscribe',
                data: { subscription_type: 'all' }
            }));
            break;

        case 'performance_update':
            console.log('性能指标:', message.data);
            updatePerformanceUI(message.data);
            break;

        case 'log_entry':
            console.log(`[${message.data.level}] ${message.data.message}`);
            updateLogUI(message.data);
            break;
    }
};

// 心跳
setInterval(() => {
    ws.send(JSON.stringify({ type: 'ping' }));
}, 10000);
```

---

## API参考

### WebSocketServer API

```cpp
class WebSocketServer : public QObject
{
public:
    // 启动/停止
    bool start(const ServerConfig& config = ServerConfig());
    void stop();
    bool isRunning() const;

    // 客户端管理
    int clientCount() const;
    QList<ClientInfo> clients() const;
    void disconnectClient(const QString& sessionId, const QString& reason = QString());

    // 消息发送
    void broadcast(const QJsonObject& message);
    void broadcastToSubscribers(SubscriptionType type, const QJsonObject& message);
    void sendTo(const QString& sessionId, const QJsonObject& message);

    // 便捷推送
    void pushPerformanceMetrics(const PerformanceMetrics& metrics);
    void pushLogEntry(const LogEntryData& log);
    void pushSystemStatus(const SystemStatusData& status);

signals:
    void clientConnected(const QString& sessionId, const QString& clientId);
    void clientDisconnected(const QString& sessionId, const QString& reason);
    void messageReceived(const QString& sessionId, MessageType type, const QJsonObject& data);
    void commandReceived(const QString& sessionId, const QString& command, const QJsonObject& params);
};
```

### PerformanceMonitor API

```cpp
class PerformanceMonitor : public QObject
{
public:
    // 控制
    void start(int intervalMs = 1000);
    void stop();
    bool isRunning() const;

    // 配置
    void setSamplingInterval(int intervalMs);
    void setStatWindowSize(int size);

    // 帧处理记录
    void recordFrameStart();
    void recordFrameEnd(bool success = true);

    // 错误统计
    void incrementErrorCount();
    void incrementWarningCount();
    void resetErrorCount();
    void resetWarningCount();

    // 数据获取
    PerformanceStats currentStats() const;
    PerformanceMetrics currentMetrics() const;
    QList<PerformanceStats> history(int count = 60) const;

signals:
    void metricsUpdated(const PerformanceMetrics& metrics);
    void statsUpdated(const PerformanceStats& stats);
    void performanceWarning(const QString& message);
};
```

### RemoteLogCollector API

```cpp
class RemoteLogCollector : public QObject
{
public:
    // 初始化
    bool initialize(const QString& logFilePath = QString());
    void shutdown();
    void setFilter(const LogFilter& filter);

    // 日志记录
    void log(LogLevel level, const QString& category, const QString& message,
             const QString& file = QString(), int line = 0);
    void trace(const QString& category, const QString& message, ...);
    void debug(const QString& category, const QString& message, ...);
    void info(const QString& category, const QString& message, ...);
    void warn(const QString& category, const QString& message, ...);
    void error(const QString& category, const QString& message, ...);
    void fatal(const QString& category, const QString& message, ...);

    // 日志查询
    QList<LogEntryData> recentLogs(int count = 100) const;
    QList<LogEntryData> getLogsByLevel(LogLevel level, int maxCount = 100) const;
    QList<LogEntryData> getLogsByCategory(const QString& category, int maxCount = 100) const;
    QList<LogEntryData> searchLogs(const QString& keyword, int maxCount = 100) const;
    QMap<LogLevel, int> getLogStatistics() const;

    // 缓存管理
    int cacheSize() const;
    void clearCache();
    void clearFileLog();

signals:
    void logAdded(const LogEntryData& entry);
    void errorOccurred(const LogEntryData& entry);
    void fatalError(const LogEntryData& entry);
    void cacheFull();
};
```

---

## 后续扩展

### 待实现功能

#### 1. Web监控前端 (优先级: 高)
- [ ] Vue.js 3 + TypeScript 框架搭建
- [ ] ECharts 5 图表集成
- [ ] 实时性能仪表盘
- [ ] 日志查看器UI
- [ ] 历史数据查询界面

#### 2. 数据持久化 (优先级: 中)
- [ ] SQLite数据库集成
- [ ] 性能指标历史存储
- [ ] 日志数据库查询优化
- [ ] 数据导出功能 (CSV/JSON)

#### 3. 高级功能 (优先级: 中)
- [ ] 告警系统 (邮件/短信通知)
- [ ] 多设备集中管理
- [ ] AI异常检测
- [ ] 录像回放功能
- [ ] 远程配置更新

#### 4. 安全增强 (优先级: 高)
- [ ] WSS (WebSocket Secure) 支持
- [ ] Token认证机制
- [ ] 基于角色的访问控制 (RBAC)
- [ ] IP白名单
- [ ] 日志脱敏处理

#### 5. 性能优化 (优先级: 低)
- [ ] WebSocket消息压缩 (gzip)
- [ ] 大数据分批传输
- [ ] 数据缓存策略优化
- [ ] 日志文件自动清理

---

## 总结

✅ **已完成**: 远程诊断系统核心后端组件 (4/4)
- 完整的WebSocket通信框架
- 实时性能监控能力
- 完善的日志收集系统
- 清晰的消息协议定义

📊 **代码统计**:
- 新增文件: 8个 (4个头文件 + 4个源文件)
- 代码行数: ~3000行
- 编译状态: ✅ 全部通过
- 测试状态: ⏸️ 待编写

🎯 **下一步**:
1. 开发Web监控前端界面
2. 集成到主应用程序
3. 编写单元测试
4. 性能基准测试
5. 编写用户文档

---

**文档版本**: 1.0.0
**最后更新**: 2025-12-20
**作者**: VisionForge Team
