# VisionForge Pro 远程诊断系统 - 完整使用指南

## 📋 目录
- [快速开始](#快速开始)
- [完整集成示例](#完整集成示例)
- [核心组件使用](#核心组件使用)
- [数据持久化](#数据持久化)
- [最佳实践](#最佳实践)
- [故障排查](#故障排查)

---

## 快速开始

### 1. 最简单的集成（5分钟）

```cpp
#include <QCoreApplication>
#include "remote/WebSocketServer.h"
#include "remote/PerformanceMonitor.h"
#include "remote/RemoteLogCollector.h"
#include "remote/RemoteDatabaseManager.h"

using namespace VisionForge::Remote;

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    // 创建核心组件
    auto* server = new WebSocketServer(&app);
    auto* perfMonitor = new PerformanceMonitor(&app);
    auto* logCollector = new RemoteLogCollector(&app);
    auto* database = new RemoteDatabaseManager(&app);

    // 启动服务
    server->start();
    perfMonitor->start(1000);  // 每秒采样
    logCollector->initialize();
    database->initialize();

    // 自动连接：性能指标推送到WebSocket和数据库
    QObject::connect(perfMonitor, &PerformanceMonitor::metricsUpdated,
        [server, database](const PerformanceMetrics& metrics) {
            server->pushPerformanceMetrics(metrics);
            database->insertPerformanceMetrics(metrics);
        });

    // 自动连接：日志推送到WebSocket和数据库
    QObject::connect(logCollector, &RemoteLogCollector::logAdded,
        [server, database](const LogEntryData& log) {
            server->pushLogEntry(log);
            database->insertLogEntry(log);
        });

    logCollector->info("System", "远程诊断系统已启动");

    return app.exec();
}
```

### 2. 客户端连接（JavaScript）

```javascript
// 连接WebSocket服务器
const ws = new WebSocket('ws://localhost:8765');

ws.onopen = () => {
    console.log('连接成功');

    // 握手
    ws.send(JSON.stringify({
        type: 'client_hello',
        data: {
            client_id: 'monitor-1',
            version: '1.0.0'
        }
    }));
};

ws.onmessage = (event) => {
    const msg = JSON.parse(event.data);

    switch(msg.type) {
        case 'server_hello':
            // 订阅所有数据
            ws.send(JSON.stringify({
                type: 'subscribe',
                data: { subscription_type: 'all' }
            }));
            break;

        case 'performance_update':
            updateDashboard(msg.data);
            break;

        case 'log_entry':
            appendLog(msg.data);
            break;
    }
};
```

---

## 完整集成示例

### 方案一：独立诊断服务

创建一个独立的诊断服务类，封装所有功能：

```cpp
// RemoteDiagnosticsService.h
#pragma once

#include <QObject>
#include "remote/WebSocketServer.h"
#include "remote/PerformanceMonitor.h"
#include "remote/RemoteLogCollector.h"
#include "remote/RemoteDatabaseManager.h"

class RemoteDiagnosticsService : public QObject
{
    Q_OBJECT

public:
    explicit RemoteDiagnosticsService(QObject* parent = nullptr);
    ~RemoteDiagnosticsService() override = default;

    /**
     * @brief 启动诊断服务
     */
    bool start(quint16 port = 8765);

    /**
     * @brief 停止诊断服务
     */
    void stop();

    /**
     * @brief 获取日志收集器（用于记录日志）
     */
    VisionForge::Remote::RemoteLogCollector* logCollector() { return logCollector_; }

    /**
     * @brief 获取性能监控器（用于记录帧处理）
     */
    VisionForge::Remote::PerformanceMonitor* performanceMonitor() { return perfMonitor_; }

    /**
     * @brief 获取数据库管理器（用于数据查询和导出）
     */
    VisionForge::Remote::RemoteDatabaseManager* databaseManager() { return database_; }

signals:
    void serviceStarted();
    void serviceStopped();
    void serviceError(const QString& error);

private:
    void setupConnections();

private:
    VisionForge::Remote::WebSocketServer* server_;
    VisionForge::Remote::PerformanceMonitor* perfMonitor_;
    VisionForge::Remote::RemoteLogCollector* logCollector_;
    VisionForge::Remote::RemoteDatabaseManager* database_;
};

// RemoteDiagnosticsService.cpp
#include "RemoteDiagnosticsService.h"

using namespace VisionForge::Remote;

RemoteDiagnosticsService::RemoteDiagnosticsService(QObject* parent)
    : QObject(parent)
{
    // 创建组件
    server_ = new WebSocketServer(this);
    perfMonitor_ = new PerformanceMonitor(this);
    logCollector_ = new RemoteLogCollector(this);
    database_ = new RemoteDatabaseManager(this);

    // 设置连接
    setupConnections();
}

bool RemoteDiagnosticsService::start(quint16 port)
{
    // 配置WebSocket服务器
    ServerConfig serverConfig;
    serverConfig.port = port;
    serverConfig.maxConnections = 100;
    serverConfig.heartbeatIntervalMs = 10000;

    if (!server_->start(serverConfig)) {
        emit serviceError("无法启动WebSocket服务器");
        return false;
    }

    // 启动性能监控
    perfMonitor_->setSamplingInterval(1000);  // 每秒采样
    perfMonitor_->setStatWindowSize(60);      // 60秒统计窗口
    perfMonitor_->start();

    // 初始化日志收集器
    if (!logCollector_->initialize()) {
        emit serviceError("无法初始化日志收集器");
        return false;
    }

    // 初始化数据库
    DatabaseConfig dbConfig;
    dbConfig.databasePath = "./data/diagnostics.db";
    dbConfig.maxRecordAge = 30;  // 保留30天数据
    dbConfig.enableAutoCleanup = true;

    if (!database_->initialize(dbConfig)) {
        emit serviceError("无法初始化数据库");
        return false;
    }

    logCollector_->info("RemoteDiagnostics",
        QString("远程诊断服务已启动 (端口: %1)").arg(port));

    emit serviceStarted();
    return true;
}

void RemoteDiagnosticsService::stop()
{
    server_->stop();
    perfMonitor_->stop();
    logCollector_->shutdown();
    database_->close();

    emit serviceStopped();
}

void RemoteDiagnosticsService::setupConnections()
{
    // 性能指标 → WebSocket + 数据库
    connect(perfMonitor_, &PerformanceMonitor::metricsUpdated,
        [this](const PerformanceMetrics& metrics) {
            server_->pushPerformanceMetrics(metrics);
            database_->insertPerformanceMetrics(metrics);
        });

    // 日志 → WebSocket + 数据库
    connect(logCollector_, &RemoteLogCollector::logAdded,
        [this](const LogEntryData& log) {
            // 只推送INFO及以上级别到WebSocket
            if (static_cast<int>(log.level) >= static_cast<int>(LogLevel::Info)) {
                server_->pushLogEntry(log);
            }
            // 所有级别都存入数据库
            database_->insertLogEntry(log);
        });

    // 性能警告日志
    connect(perfMonitor_, &PerformanceMonitor::performanceWarning,
        [this](const QString& message) {
            logCollector_->warn("Performance", message);
        });

    // 数据库错误日志
    connect(database_, &RemoteDatabaseManager::databaseError,
        [this](const QString& error) {
            logCollector_->error("Database", error);
        });

    // 客户端连接日志
    connect(server_, &WebSocketServer::clientConnected,
        [this](const QString& sessionId, const QString& clientId) {
            logCollector_->info("WebSocket",
                QString("客户端连接: %1 (session: %2)").arg(clientId, sessionId));
        });

    // 客户端断开日志
    connect(server_, &WebSocketServer::clientDisconnected,
        [this](const QString& sessionId, const QString& reason) {
            logCollector_->info("WebSocket",
                QString("客户端断开: %1 (%2)").arg(sessionId, reason));
        });
}
```

### 方案二：在主应用中集成

```cpp
// 在MainWindow或主应用类中
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    void setupRemoteDiagnostics();
    void recordFrameProcessing(const cv::Mat& image);

private:
    RemoteDiagnosticsService* diagnostics_;
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // 创建并启动诊断服务
    diagnostics_ = new RemoteDiagnosticsService(this);
    setupRemoteDiagnostics();
}

void MainWindow::setupRemoteDiagnostics()
{
    // 启动诊断服务（端口8765）
    if (diagnostics_->start(8765)) {
        qInfo() << "远程诊断服务已启动";
    } else {
        qCritical() << "远程诊断服务启动失败";
    }

    // 应用事件日志
    diagnostics_->logCollector()->info("App", "应用程序启动");
}

void MainWindow::recordFrameProcessing(const cv::Mat& image)
{
    auto* perfMonitor = diagnostics_->performanceMonitor();

    // 记录帧处理开始
    perfMonitor->recordFrameStart();

    try {
        // 处理图像
        processImage(image);

        // 记录成功
        perfMonitor->recordFrameEnd(true);

        diagnostics_->logCollector()->debug("Processing", "图像处理完成");

    } catch (const std::exception& e) {
        // 记录失败
        perfMonitor->recordFrameEnd(false);
        perfMonitor->incrementErrorCount();

        diagnostics_->logCollector()->error("Processing",
            QString("图像处理失败: %1").arg(e.what()));
    }
}
```

---

## 核心组件使用

### 1. WebSocketServer - 实时通信

```cpp
using namespace VisionForge::Remote;

// 创建服务器
auto* server = new WebSocketServer(this);

// 配置服务器
ServerConfig config;
config.port = 8765;
config.maxConnections = 100;
config.heartbeatIntervalMs = 10000;  // 10秒心跳
config.heartbeatTimeoutMs = 30000;   // 30秒超时
config.enableSsl = false;            // 生产环境建议启用
config.requireAuth = true;

server->start(config);

// 监听客户端连接
connect(server, &WebSocketServer::clientConnected,
    [](const QString& sessionId, const QString& clientId) {
        qInfo() << "新客户端:" << clientId;
    });

// 处理客户端命令
connect(server, &WebSocketServer::commandReceived,
    [this](const QString& sessionId, const QString& command, const QJsonObject& params) {
        if (command == "export_data") {
            // 导出数据
            exportDiagnosticsData(params);
        } else if (command == "get_statistics") {
            // 返回统计信息
            auto stats = database_->getStatistics();
            // 发送响应...
        }
    });

// 手动推送自定义消息
QJsonObject customData;
customData["event"] = "camera_connected";
customData["camera_id"] = 1;
server->broadcast(customData);
```

### 2. PerformanceMonitor - 性能监控

```cpp
auto* monitor = new PerformanceMonitor(this);

// 配置
monitor->setSamplingInterval(1000);  // 每秒采样
monitor->setStatWindowSize(60);      // 60秒统计窗口

// 启动
monitor->start();

// 监听性能更新
connect(monitor, &PerformanceMonitor::metricsUpdated,
    [](const PerformanceMetrics& metrics) {
        qDebug() << QString("CPU: %1% | Memory: %2MB | FPS: %3")
                    .arg(metrics.cpuUsage, 0, 'f', 1)
                    .arg(metrics.memoryUsedMB)
                    .arg(metrics.fps, 0, 'f', 1);
    });

// 监听性能警告
connect(monitor, &PerformanceMonitor::performanceWarning,
    [](const QString& message) {
        qWarning() << "性能警告:" << message;
    });

// 在图像处理循环中记录
void processFrame(const cv::Mat& frame) {
    monitor->recordFrameStart();

    try {
        // 处理图像
        auto result = detector->detect(frame);

        monitor->recordFrameEnd(true);  // 成功
    } catch (...) {
        monitor->recordFrameEnd(false);  // 失败
        monitor->incrementErrorCount();
    }
}

// 获取统计信息
PerformanceStats stats = monitor->currentStats();
qInfo() << "平均FPS:" << stats.avgFPS;
qInfo() << "平均延迟:" << stats.avgLatencyMs << "ms";
qInfo() << "错误数量:" << stats.errorCount;

// 获取历史数据
QList<PerformanceStats> history = monitor->history(60);  // 最近60条
```

### 3. RemoteLogCollector - 日志收集

```cpp
auto* logCollector = new RemoteLogCollector(this);

// 初始化
logCollector->initialize("./logs/app.log");

// 配置过滤器
LogFilter filter;
filter.minLevel = LogLevel::Info;           // 最低INFO级别
filter.maxCacheSize = 10000;                // 缓存10000条
filter.enableFileLog = true;                // 启用文件日志
filter.enableRemoteLog = true;              // 启用远程推送

logCollector->setFilter(filter);

// 记录各级别日志
logCollector->trace("Module", "详细调试信息");
logCollector->debug("Module", "调试信息");
logCollector->info("App", "应用程序启动成功");
logCollector->warn("Camera", "相机连接延迟", __FILE__, __LINE__);
logCollector->error("Processing", "处理失败", __FILE__, __LINE__);
logCollector->fatal("System", "严重错误");

// 监听错误日志
connect(logCollector, &RemoteLogCollector::errorOccurred,
    [](const LogEntryData& entry) {
        QMessageBox::critical(nullptr, "错误", entry.message);
    });

// 监听严重错误
connect(logCollector, &RemoteLogCollector::fatalError,
    [this](const LogEntryData& entry) {
        // 严重错误：保存状态并退出
        saveApplicationState();
        qApp->quit();
    });

// 查询日志
QList<LogEntryData> errorLogs = logCollector->getLogsByLevel(LogLevel::Error, 50);
QList<LogEntryData> recentLogs = logCollector->recentLogs(100);
QList<LogEntryData> searchResults = logCollector->searchLogs("连接失败", 20);

// 获取统计
auto stats = logCollector->getLogStatistics();
qInfo() << "错误数:" << stats[LogLevel::Error];
qInfo() << "警告数:" << stats[LogLevel::Warn];
```

### 4. RemoteDatabaseManager - 数据持久化

```cpp
auto* database = new RemoteDatabaseManager(this);

// 初始化
DatabaseConfig config;
config.databasePath = "./data/diagnostics.db";
config.maxRecordAge = 30;              // 保留30天
config.enableAutoCleanup = true;       // 自动清理
config.cleanupIntervalHours = 24;      // 每24小时清理

database->initialize(config);

// 插入性能指标
PerformanceMetrics metrics;
metrics.cpuUsage = 45.2;
metrics.memoryUsedMB = 1024;
metrics.fps = 30.5;
metrics.avgLatencyMs = 15.3;
metrics.errorCount = 0;
metrics.timestamp = QDateTime::currentDateTime();

database->insertPerformanceMetrics(metrics);

// 批量插入（高效）
QList<PerformanceMetrics> metricsList = collectMetrics();
database->insertPerformanceMetricsBatch(metricsList);

// 查询性能指标
PerformanceQuery query;
query.startTime = QDateTime::currentDateTime().addDays(-7);  // 最近7天
query.endTime = QDateTime::currentDateTime();
query.limit = 1000;
query.minFps = 20;  // FPS >= 20

QList<PerformanceMetrics> results = database->queryPerformanceMetrics(query);

// 插入日志
LogEntryData log;
log.timestamp = QDateTime::currentDateTime();
log.level = LogLevel::Error;
log.category = "Processing";
log.message = "图像处理失败";
log.file = __FILE__;
log.line = __LINE__;

database->insertLogEntry(log);

// 查询日志
LogQuery logQuery;
logQuery.startTime = QDateTime::currentDateTime().addHours(-1);  // 最近1小时
logQuery.minLevel = LogLevel::Warn;  // WARN及以上
logQuery.keyword = "失败";           // 包含"失败"

QList<LogEntryData> logs = database->queryLogEntries(logQuery);

// 获取统计信息
StatisticsData stats = database->getStatistics();
qInfo() << "性能记录数:" << stats.totalPerformanceRecords;
qInfo() << "日志记录数:" << stats.totalLogRecords;
qInfo() << "数据库大小:" << stats.databaseSizeBytes / 1024 / 1024 << "MB";

// 获取平均性能
QDateTime start = QDateTime::currentDateTime().addDays(-1);
QDateTime end = QDateTime::currentDateTime();
PerformanceMetrics avgPerf = database->getAveragePerformance(start, end);
qInfo() << "24小时平均FPS:" << avgPerf.fps;

// 导出数据
database->exportPerformanceToCSV("./export/perf.csv", query);
database->exportLogsToCSV("./export/logs.csv", logQuery);
database->exportToJSON("./export/data.json", start, end);

// 手动清理过期数据
database->cleanupExpiredData();

// 优化数据库
database->optimizeDatabase();
```

---

## 数据持久化

### 数据库表结构

#### performance_metrics表
```sql
CREATE TABLE performance_metrics (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    timestamp DATETIME NOT NULL,
    cpu_usage REAL NOT NULL,
    memory_used_mb INTEGER NOT NULL,
    fps REAL NOT NULL,
    avg_latency_ms REAL NOT NULL,
    error_count INTEGER NOT NULL DEFAULT 0
);

CREATE INDEX idx_perf_timestamp ON performance_metrics(timestamp DESC);
CREATE INDEX idx_perf_cpu ON performance_metrics(cpu_usage);
CREATE INDEX idx_perf_fps ON performance_metrics(fps);
```

#### log_entries表
```sql
CREATE TABLE log_entries (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    timestamp DATETIME NOT NULL,
    level INTEGER NOT NULL,
    category TEXT NOT NULL,
    message TEXT NOT NULL,
    file TEXT,
    line INTEGER
);

CREATE INDEX idx_log_timestamp ON log_entries(timestamp DESC);
CREATE INDEX idx_log_level ON log_entries(level);
CREATE INDEX idx_log_category ON log_entries(category);
```

### 性能优化建议

1. **批量插入**
```cpp
// ❌ 低效：逐条插入
for (const auto& metrics : metricsList) {
    database->insertPerformanceMetrics(metrics);  // 每次都有事务开销
}

// ✅ 高效：批量插入
database->insertPerformanceMetricsBatch(metricsList);  // 单次事务
```

2. **定期清理**
```cpp
// 启用自动清理
DatabaseConfig config;
config.maxRecordAge = 30;              // 保留30天
config.enableAutoCleanup = true;
config.cleanupIntervalHours = 24;      // 每24小时
```

3. **数据库优化**
```cpp
// 定期执行VACUUM（建议每月一次）
QTimer* optimizeTimer = new QTimer(this);
connect(optimizeTimer, &QTimer::timeout, [database]() {
    database->optimizeDatabase();
});
optimizeTimer->start(30 * 24 * 3600 * 1000);  // 每30天
```

---

## 最佳实践

### 1. 错误处理

```cpp
// ✅ 正确的错误处理
try {
    auto result = processImage(image);
    perfMonitor->recordFrameEnd(true);
    logCollector->debug("Processing", "处理成功");

} catch (const std::exception& e) {
    perfMonitor->recordFrameEnd(false);
    perfMonitor->incrementErrorCount();
    logCollector->error("Processing",
        QString("处理失败: %1").arg(e.what()), __FILE__, __LINE__);
}
```

### 2. 资源管理

```cpp
// ✅ 使用智能指针或Qt父子关系
class MyApp : public QObject
{
public:
    MyApp(QObject* parent = nullptr) : QObject(parent) {
        // 所有组件都使用this作为父对象，自动清理
        diagnostics_ = new RemoteDiagnosticsService(this);
    }

private:
    RemoteDiagnosticsService* diagnostics_;
};
```

### 3. 日志级别使用

```cpp
// TRACE: 极详细的调试信息（开发阶段）
logCollector->trace("Module", "进入函数processImage()");

// DEBUG: 调试信息（测试阶段）
logCollector->debug("Module", "参数: width=1920, height=1080");

// INFO: 一般信息（生产环境）
logCollector->info("App", "应用程序启动成功");

// WARN: 警告（需要注意，但不影响运行）
logCollector->warn("Camera", "相机延迟较高: 50ms");

// ERROR: 错误（影响功能，但可恢复）
logCollector->error("Processing", "图像处理失败", __FILE__, __LINE__);

// FATAL: 严重错误（不可恢复，需要退出）
logCollector->fatal("System", "内存分配失败");
```

### 4. 性能监控建议

```cpp
// ✅ 合理的采样间隔
perfMonitor->setSamplingInterval(1000);  // 1秒（推荐）
// perfMonitor->setSamplingInterval(100);   // 100ms（太频繁）
// perfMonitor->setSamplingInterval(10000); // 10秒（太稀疏）

// ✅ 合理的统计窗口
perfMonitor->setStatWindowSize(60);   // 60秒（推荐）
// perfMonitor->setStatWindowSize(300);  // 5分钟（长期趋势）
```

---

## 故障排查

### 问题1: WebSocket连接失败

**症状**: 客户端无法连接到服务器

**排查步骤**:
```cpp
// 1. 检查服务器是否成功启动
if (!server->isRunning()) {
    qCritical() << "服务器未运行";
}

// 2. 检查端口是否被占用
netstat -ano | findstr :8765  // Windows
lsof -i :8765                 // Linux/macOS

// 3. 检查防火墙设置
// Windows: 控制面板 → 防火墙 → 允许应用

// 4. 使用telnet测试连接
telnet localhost 8765
```

**解决方案**:
```cpp
// 更换端口
ServerConfig config;
config.port = 8766;  // 使用其他端口
server->start(config);
```

### 问题2: 数据库文件过大

**症状**: 数据库文件占用大量磁盘空间

**排查**:
```cpp
qint64 size = database->getDatabaseSize();
qInfo() << "数据库大小:" << size / 1024 / 1024 << "MB";

StatisticsData stats = database->getStatistics();
qInfo() << "性能记录:" << stats.totalPerformanceRecords;
qInfo() << "日志记录:" << stats.totalLogRecords;
```

**解决方案**:
```cpp
// 1. 缩短数据保留时间
DatabaseConfig config;
config.maxRecordAge = 7;  // 改为7天

// 2. 手动清理
database->cleanupExpiredData();

// 3. 优化数据库
database->optimizeDatabase();

// 4. 删除指定时间前的数据
QDateTime cutoff = QDateTime::currentDateTime().addDays(-7);
database->deletePerformanceMetricsBefore(cutoff);
database->deleteLogEntriesBefore(cutoff);
```

### 问题3: 性能监控CPU使用率异常

**症状**: CPU使用率显示为0或异常值

**排查**:
```cpp
// 检查采样间隔
int interval = perfMonitor->samplingInterval();
qDebug() << "采样间隔:" << interval << "ms";

// 手动获取CPU使用率
PerformanceStats stats = perfMonitor->currentStats();
qDebug() << "CPU使用率:" << stats.cpuUsagePercent << "%";
```

**解决方案**:
```cpp
// 1. 增加采样间隔（至少1秒）
perfMonitor->setSamplingInterval(1000);

// 2. 重启监控
perfMonitor->stop();
perfMonitor->start(1000);
```

---

## 总结

✅ **核心组件**: WebSocket服务器、性能监控器、日志收集器、数据库管理器

✅ **完整功能**: 实时通信、性能监控、日志管理、数据持久化、数据导出

✅ **易于集成**: 清晰的API、完整的示例代码、详细的文档

✅ **生产就绪**: 错误处理、资源管理、性能优化、故障排查

---

**文档版本**: 1.0.0
**最后更新**: 2025-12-20
**作者**: VisionForge Team
