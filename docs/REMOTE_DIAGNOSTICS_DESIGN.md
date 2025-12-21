# VisionForge Pro 远程诊断系统 - 架构设计

## 📋 目录
- [系统概述](#系统概述)
- [架构设计](#架构设计)
- [核心组件](#核心组件)
- [技术栈](#技术栈)
- [数据流](#数据流)
- [接口设计](#接口设计)
- [部署方案](#部署方案)

---

## 系统概述

### 设计目标
为VisionForge Pro工业视觉系统提供7x24小时的远程监控和诊断能力，确保：
- ✅ 实时监控系统运行状态
- ✅ 远程查看系统日志
- ✅ 性能指标可视化分析
- ✅ 故障预警和快速定位
- ✅ 最小化现场维护成本

### 应用场景
- **远程运维**：技术人员在办公室监控现场设备
- **故障诊断**：快速定位问题，减少停机时间
- **性能优化**：分析长期趋势，优化系统配置
- **客户支持**：为客户提供主动式技术支持

---

## 架构设计

### 总体架构

```
┌─────────────────────────────────────────────────────────────┐
│                     远程诊断系统                              │
├─────────────────────────────────────────────────────────────┤
│                                                               │
│  ┌──────────────┐       ┌──────────────┐                    │
│  │  Web监控界面  │◄──────┤ WebSocket    │                    │
│  │ (HTML/JS/CSS) │       │    服务器     │                    │
│  └──────────────┘       └──────┬───────┘                    │
│         ▲                       │                             │
│         │                       ▼                             │
│  ┌──────┴───────┐       ┌──────────────┐                    │
│  │ Qt WebEngine │       │   数据总线    │                    │
│  │    容器       │       │ (Event Bus)   │                    │
│  └──────────────┘       └──────┬───────┘                    │
│                                 │                             │
│         ┌───────────────────────┼───────────────────┐        │
│         │                       │                   │        │
│         ▼                       ▼                   ▼        │
│  ┌─────────────┐       ┌──────────────┐    ┌──────────────┐│
│  │ 性能监控器   │       │  日志收集器   │    │  系统状态    ││
│  │ Performance │       │    Logger     │    │   Monitor    ││
│  │   Monitor   │       │   Collector   │    │              ││
│  └─────────────┘       └──────────────┘    └──────────────┘│
│         │                       │                   │        │
│         └───────────────────────┴───────────────────┘        │
│                                 │                             │
│                                 ▼                             │
│                       ┌──────────────┐                       │
│                       │  VisionForge │                       │
│                       │     Core     │                       │
│                       └──────────────┘                       │
└─────────────────────────────────────────────────────────────┘
```

### 分层架构

#### 1. **展示层 (Presentation Layer)**
- Web监控界面
- Qt WebEngine容器
- 实时图表和可视化

#### 2. **服务层 (Service Layer)**
- WebSocket服务器
- RESTful API
- 实时数据推送

#### 3. **业务层 (Business Layer)**
- 性能监控器
- 日志收集器
- 告警管理器

#### 4. **数据层 (Data Layer)**
- 时序数据库（性能指标）
- 日志数据库
- 配置存储

---

## 核心组件

### 1. WebSocket服务器

**职责**：
- 提供实时双向通信通道
- 推送系统状态更新
- 处理客户端命令

**技术方案**：
- Qt WebSockets模块
- JSON消息格式
- 心跳检测机制

**关键类**：
```cpp
class WebSocketServer : public QObject {
    Q_OBJECT
public:
    // 启动服务器
    bool start(quint16 port = 8765);

    // 广播消息
    void broadcast(const QJsonObject& message);

    // 发送给特定客户端
    void sendTo(QWebSocket* client, const QJsonObject& message);

signals:
    void clientConnected(QWebSocket* client);
    void clientDisconnected(QWebSocket* client);
    void messageReceived(const QJsonObject& message);
};
```

### 2. 性能监控器

**职责**：
- 采集CPU、内存、GPU使用率
- 监控处理帧率和延迟
- 统计算法执行时间

**采集指标**：
- **系统级**：CPU使用率、内存使用率、磁盘IO
- **进程级**：应用内存、线程数、句柄数
- **业务级**：处理帧率、平均延迟、错误率

**关键类**：
```cpp
class PerformanceMonitor : public QObject {
    Q_OBJECT
public:
    struct Metrics {
        double cpuUsage;        // CPU使用率 (%)
        qint64 memoryUsed;      // 内存使用 (MB)
        double fps;             // 帧率
        double avgLatency;      // 平均延迟 (ms)
        int errorCount;         // 错误计数
        QDateTime timestamp;    // 时间戳
    };

    // 启动监控
    void start(int intervalMs = 1000);

    // 获取最新指标
    Metrics currentMetrics() const;

signals:
    void metricsUpdated(const Metrics& metrics);
};
```

### 3. 日志收集器

**职责**：
- 统一收集应用日志
- 日志分级和过滤
- 远程上传功能

**日志级别**：
- TRACE：详细调试信息
- DEBUG：调试信息
- INFO：一般信息
- WARN：警告信息
- ERROR：错误信息
- FATAL：严重错误

**关键类**：
```cpp
class RemoteLogCollector : public QObject {
    Q_OBJECT
public:
    enum LogLevel { Trace, Debug, Info, Warn, Error, Fatal };

    struct LogEntry {
        QDateTime timestamp;
        LogLevel level;
        QString category;
        QString message;
        QString file;
        int line;
    };

    // 添加日志条目
    void addLog(const LogEntry& entry);

    // 上传日志到服务器
    void uploadLogs(const QUrl& serverUrl);

signals:
    void logAdded(const LogEntry& entry);
    void uploadComplete(bool success);
};
```

### 4. Web监控界面

**技术栈**：
- HTML5 + CSS3
- Vue.js 3 (响应式框架)
- ECharts (图表库)
- WebSocket API

**主要页面**：

#### 4.1 系统总览（Dashboard）
```
┌────────────────────────────────────────────────┐
│  VisionForge Pro - 远程监控系统                  │
├────────────────────────────────────────────────┤
│  ┌─────────┐  ┌─────────┐  ┌─────────┐        │
│  │ CPU 42% │  │ MEM 68% │  │ FPS 30  │        │
│  └─────────┘  └─────────┘  └─────────┘        │
│                                                 │
│  ┌─────────────────────────────────────────┐  │
│  │  性能趋势图 (CPU/Memory/FPS)             │  │
│  │  ┌───────────────────────────────────┐  │  │
│  │  │        ╱╲    ╱╲                   │  │  │
│  │  │     ╱╲╱  ╲╱╲╱  ╲    ╱╲           │  │  │
│  │  │  ╱╲╱            ╲╱╲╱  ╲          │  │  │
│  │  └───────────────────────────────────┘  │  │
│  └─────────────────────────────────────────┘  │
│                                                 │
│  ┌─────────────────────────────────────────┐  │
│  │  实时日志                                 │  │
│  │  [INFO] System started                   │  │
│  │  [WARN] High memory usage detected       │  │
│  │  [ERROR] Camera connection timeout       │  │
│  └─────────────────────────────────────────┘  │
└────────────────────────────────────────────────┘
```

#### 4.2 日志查看器
- 实时日志流
- 日志级别过滤
- 关键字搜索
- 导出功能

#### 4.3 性能分析
- CPU/内存趋势图
- 帧率分析
- 算法性能分布
- 历史数据查询

---

## 技术栈

### 后端
- **Qt 6.5+**：核心框架
- **Qt WebSockets**：WebSocket服务器
- **Qt Network**：HTTP/REST API
- **spdlog**：日志库
- **SQLite**：本地数据存储

### 前端
- **HTML5 + CSS3**：页面结构和样式
- **Vue.js 3**：响应式框架
- **ECharts 5**：数据可视化
- **Element Plus**：UI组件库
- **Axios**：HTTP客户端

### 通信协议
- **WebSocket**：实时数据推送
- **JSON**：数据交换格式
- **HTTP/REST**：配置和历史数据查询

---

## 数据流

### 1. 实时监控数据流

```
VisionForge Core
      ↓
PerformanceMonitor (采集指标)
      ↓
Event Bus (发布数据)
      ↓
WebSocket Server (推送)
      ↓
Web Client (显示)
```

### 2. 日志上传数据流

```
应用日志 → RemoteLogCollector → 本地缓存
                                      ↓
                                 定期上传
                                      ↓
                              远程日志服务器
```

### 3. 命令下发数据流

```
Web Client (发送命令)
      ↓
WebSocket Server (接收)
      ↓
Command Dispatcher (解析)
      ↓
VisionForge Core (执行)
```

---

## 接口设计

### WebSocket消息格式

#### 1. 性能指标推送
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

#### 2. 日志推送
```json
{
  "type": "log_entry",
  "timestamp": "2025-12-20T22:00:00Z",
  "data": {
    "level": "ERROR",
    "category": "Camera",
    "message": "Failed to connect to camera",
    "file": "CameraManager.cpp",
    "line": 123
  }
}
```

#### 3. 系统状态
```json
{
  "type": "system_status",
  "timestamp": "2025-12-20T22:00:00Z",
  "data": {
    "status": "running",
    "uptime_seconds": 86400,
    "version": "1.2.0",
    "total_processed": 1000000
  }
}
```

### REST API

#### 获取历史性能数据
```
GET /api/performance/history?start=<timestamp>&end=<timestamp>

Response:
{
  "success": true,
  "data": [
    {
      "timestamp": "2025-12-20T21:00:00Z",
      "cpu_usage": 45.2,
      "memory_used_mb": 980,
      "fps": 29.8
    },
    ...
  ]
}
```

#### 查询日志
```
GET /api/logs?level=<level>&start=<timestamp>&end=<timestamp>&limit=<n>

Response:
{
  "success": true,
  "total": 1500,
  "data": [
    {
      "timestamp": "2025-12-20T21:55:00Z",
      "level": "ERROR",
      "message": "...",
      ...
    },
    ...
  ]
}
```

---

## 部署方案

### 单机部署（默认）
- WebSocket服务器运行在主应用内
- 监听端口：8765（可配置）
- Web界面集成在Qt WebEngine中

### 分离部署
- WebSocket服务器独立运行
- Web界面部署在独立Web服务器
- 支持多客户端同时访问

### 网络配置
- **本地访问**：`ws://localhost:8765`
- **局域网访问**：`ws://<ip>:8765`
- **公网访问**：需要配置反向代理（Nginx）+ SSL

---

## 安全考虑

### 1. 认证授权
- WebSocket连接需要Token认证
- 基于角色的访问控制（RBAC）
- 会话超时机制

### 2. 数据加密
- WebSocket使用WSS（TLS加密）
- 敏感数据加密存储
- 日志脱敏处理

### 3. 访问控制
- IP白名单
- 频率限制
- 异常访问检测

---

## 性能优化

### 1. 数据采样
- 性能指标：1秒采样一次
- 日志：实时采集，批量推送
- 历史数据：分钟级聚合

### 2. 数据压缩
- WebSocket消息使用gzip压缩
- 大量日志分批传输
- 图片数据Base64编码

### 3. 缓存策略
- 性能数据本地缓存（1小时）
- 日志本地缓存（24小时）
- 定期清理过期数据

---

## 扩展性

### 未来功能
- ✨ 告警系统（邮件/短信通知）
- ✨ 多设备集中管理
- ✨ AI异常检测
- ✨ 录像回放功能
- ✨ 远程配置更新

### 集成能力
- 支持Prometheus监控
- 支持Grafana可视化
- 支持ELK日志分析
- 支持钉钉/企业微信通知

---

## 总结

VisionForge Pro远程诊断系统通过以下核心能力，为工业视觉系统提供全方位的远程运维支持：

1. **实时监控**：WebSocket实时推送性能指标
2. **日志管理**：统一收集、查询、分析日志
3. **可视化**：直观的Web界面和图表
4. **远程诊断**：快速定位问题，减少现场维护

系统设计遵循模块化、可扩展、高可用的原则，为后续功能扩展预留充足空间。
