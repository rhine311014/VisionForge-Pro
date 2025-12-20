# VisionForge Pro - 集成测试指南

## 📋 目录

1. [概述](#概述)
2. [前置条件](#前置条件)
3. [后端服务启动](#后端服务启动)
4. [前端服务启动](#前端服务启动)
5. [集成测试步骤](#集成测试步骤)
6. [测试场景](#测试场景)
7. [故障排除](#故障排除)
8. [性能测试](#性能测试)

---

## 概述

本指南描述了VisionForge Pro远程诊断系统的前后端集成测试流程。集成测试确保:
- WebSocket服务器正常运行
- 前端能够成功连接到后端
- 认证和权限控制正常工作
- 实时数据传输正常
- 前端UI正确显示后端数据

### 系统架构
```
┌─────────────┐         WebSocket (WSS)        ┌─────────────┐
│ Vue.js 3    │ ◄──────────────────────────► │ Qt 6 C++    │
│ Frontend    │    Token Authentication       │ Backend     │
│             │    Real-time Data             │             │
│ - Dashboard │                               │ - Monitor   │
│ - Logs      │                               │ - Security  │
│ - Settings  │                               │ - Database  │
└─────────────┘                               └─────────────┘
  Port: 3000                                    Port: 8765
```

---

## 前置条件

### 开发环境

**后端依赖:**
- Qt 6.9.3
- CMake 3.20+
- MSVC 2019+ 或 GCC 9+
- OpenCV 4.10.0
- OpenSSL (用于WSS)
- SQLite (用于数据库)

**前端依赖:**
- Node.js 18+
- npm 或 yarn

### 编译后端

```bash
cd "d:/KK/DESK/VisionForge Pro"

# 配置CMake
cd build
cmake ..

# 编译
cmake --build . --config Release

# 编译完成后,可执行文件位于: build/bin/Release/
```

### 安装前端依赖

```bash
cd frontend
npm install
```

---

## 后端服务启动

### 方式一: 直接运行主程序

```bash
cd "d:/KK/DESK/VisionForge Pro/build/bin/Release"
./VisionForge.exe
```

### 方式二: 单独运行WebSocket服务器(如果有单独的服务器程序)

```bash
cd "d:/KK/DESK/VisionForge Pro/build/bin/Release"
./VisionForgeServer.exe --port 8765 --ssl
```

### 配置文件

后端配置文件位于 `config/remote_config.json`:

```json
{
  "websocket": {
    "port": 8765,
    "enable_ssl": false,
    "ssl_cert_path": "./certs/server.crt",
    "ssl_key_path": "./certs/server.key"
  },
  "security": {
    "enable_authentication": true,
    "token_secret_key": "your-secret-key-here",
    "access_token_lifetime": 3600,
    "refresh_token_lifetime": 86400
  },
  "database": {
    "path": "./data/diagnostics.db"
  },
  "performance": {
    "collect_interval_ms": 1000,
    "history_retention_hours": 24
  }
}
```

### 验证后端服务

打开浏览器开发者工具,在Console中测试WebSocket连接:

```javascript
const ws = new WebSocket('ws://localhost:8765/ws');
ws.onopen = () => console.log('Connected');
ws.onmessage = (event) => console.log('Message:', event.data);
ws.onerror = (error) => console.error('Error:', error);
```

---

## 前端服务启动

### 开发模式

```bash
cd frontend
npm run dev
```

前端将在 `http://localhost:3000` 启动,并自动打开浏览器。

### 生产模式

```bash
# 构建
cd frontend
npm run build

# 预览构建结果
npm run preview
```

### 环境变量

创建 `.env` 文件配置环境变量:

```env
# WebSocket服务器地址
VITE_WS_URL=ws://localhost:8765/ws

# API服务器地址
VITE_API_BASE=http://localhost:8080/api
```

---

## 集成测试步骤

### 1. 启动服务

按以下顺序启动服务:

```bash
# 终端1: 启动后端
cd "d:/KK/DESK/VisionForge Pro/build/bin/Release"
./VisionForge.exe

# 终端2: 启动前端
cd "d:/KK/DESK/VisionForge Pro/frontend"
npm run dev
```

### 2. 基础连接测试

1. **打开前端页面**
   - 访问 `http://localhost:3000`
   - 应该自动重定向到登录页面

2. **登录测试**
   - 默认管理员账号: `admin` / `admin123`
   - 点击登录按钮
   - 检查是否成功跳转到Dashboard页面

3. **WebSocket连接检查**
   - 打开浏览器开发者工具 > Network > WS标签
   - 查看WebSocket连接状态
   - 应该显示 "101 Switching Protocols"

### 3. 功能测试清单

#### 认证功能

- [ ] 登录成功
- [ ] 登录失败(错误密码)
- [ ] Token自动刷新
- [ ] 登出功能

#### Dashboard页面

- [ ] CPU使用率实时更新
- [ ] 内存使用率实时更新
- [ ] FPS实时更新
- [ ] 延迟实时更新
- [ ] 图表自动滚动
- [ ] WebSocket连接状态显示

#### 性能分析页面

- [ ] 时间范围过滤器工作正常
- [ ] 统计数据正确计算
- [ ] 图表类型切换(折线图/柱状图)
- [ ] 数据表格分页
- [ ] CSV导出功能

#### 日志页面

- [ ] 实时日志接收
- [ ] 日志级别过滤
- [ ] 日志分类过滤
- [ ] 关键词搜索
- [ ] 自动滚动开关
- [ ] CSV导出功能

#### 设置页面

- [ ] WebSocket配置保存
- [ ] 性能监控配置保存
- [ ] 日志配置保存
- [ ] 用户管理(需要Admin权限)
  - [ ] 添加用户
  - [ ] 编辑用户
  - [ ] 删除用户
  - [ ] 角色分配

---

## 测试场景

### 场景1: 用户登录流程

```
1. 访问 http://localhost:3000
2. 输入用户名: admin
3. 输入密码: admin123
4. 点击登录
5. 验证:
   - 跳转到Dashboard页面
   - Token保存到localStorage
   - WebSocket自动连接
   - 开始接收实时数据
```

### 场景2: 实时性能监控

```
1. 登录后进入Dashboard
2. 观察指标卡片实时更新(每秒更新)
3. 观察图表数据点增加
4. 验证:
   - CPU/内存/FPS/延迟数值变化
   - 图表曲线绘制正常
   - 时间轴正确显示
   - 数据点不超过60个
```

### 场景3: WebSocket断线重连

```
1. 在Dashboard页面
2. 停止后端服务
3. 观察连接状态变为"未连接"
4. 重启后端服务
5. 验证:
   - 前端自动重连(3-5秒后)
   - 连接状态恢复为"已连接"
   - 数据接收恢复正常
```

### 场景4: Token过期处理

```
1. 登录后等待Token过期(默认1小时)
2. 或手动修改localStorage中的Token过期时间
3. 进行任何需要认证的操作
4. 验证:
   - 自动刷新Token
   - 或跳转到登录页面
   - 不出现错误提示
```

### 场景5: 权限控制

```
# 测试Viewer角色
1. 使用Viewer账号登录
2. 尝试访问Settings页面
3. 验证:
   - 被阻止访问或跳转
   - 提示权限不足

# 测试Admin角色
1. 使用Admin账号登录
2. 访问Settings页面
3. 验证:
   - 能够访问所有功能
   - 用户管理功能可用
```

---

## 故障排除

### 问题1: 前端无法连接到后端

**症状:**
- Console显示 "WebSocket connection failed"
- 连接状态显示"未连接"

**解决方案:**
1. 检查后端服务是否运行
2. 检查端口8765是否被占用
   ```bash
   netstat -ano | findstr :8765
   ```
3. 检查防火墙设置
4. 验证WebSocket URL配置正确

### 问题2: 登录失败

**症状:**
- 显示"登录失败"错误

**解决方案:**
1. 检查后端数据库是否初始化
2. 查看后端日志确认错误原因
3. 验证用户名和密码正确
4. 检查RBAC配置

### 问题3: Token认证失败

**症状:**
- 频繁跳转到登录页面
- Console显示401错误

**解决方案:**
1. 检查Token配置的secretKey一致
2. 验证Token有效期配置
3. 清除浏览器localStorage
4. 重新登录

### 问题4: 实时数据不更新

**症状:**
- Dashboard数据静止不动

**解决方案:**
1. 检查WebSocket连接状态
2. 查看Network标签确认消息接收
3. 检查后端性能监控是否启用
4. 验证消息订阅是否成功

### 问题5: 前端构建失败

**症状:**
- npm run build报错

**解决方案:**
1. 删除node_modules重新安装
   ```bash
   rm -rf node_modules
   npm install
   ```
2. 检查Node.js版本
3. 清除npm缓存
   ```bash
   npm cache clean --force
   ```

---

## 性能测试

### 后端性能指标

使用性能监控工具测试:

```bash
# CPU和内存使用
# 预期: CPU < 10%, Memory < 500MB (空闲时)

# WebSocket连接数
# 预期: 支持100+并发连接

# 消息吞吐量
# 预期: 1000+ 消息/秒
```

### 前端性能指标

使用Chrome DevTools Lighthouse测试:

- **Performance**: > 90分
- **Accessibility**: > 90分
- **Best Practices**: > 90分
- **SEO**: > 80分

### 网络性能

```javascript
// WebSocket延迟测试
const start = performance.now();
wsClient.send({ type: 'ping', timestamp: start });

wsClient.on('pong', (data) => {
  const latency = performance.now() - data.timestamp;
  console.log(`WebSocket延迟: ${latency.toFixed(2)}ms`);
});

// 预期延迟: < 50ms (本地)
```

### 压力测试

创建多个WebSocket连接并发送数据:

```javascript
// 创建100个连接
const clients = [];
for (let i = 0; i < 100; i++) {
  const ws = new WebSocket('ws://localhost:8765/ws');
  clients.push(ws);
}

// 每秒发送100条消息
setInterval(() => {
  clients.forEach((ws, i) => {
    ws.send(JSON.stringify({
      type: 'performance',
      clientId: i,
      data: { cpu: Math.random() * 100 }
    }));
  });
}, 1000);
```

---

## 自动化集成测试

### 使用Playwright进行E2E测试

安装Playwright:

```bash
cd frontend
npm install -D @playwright/test
npx playwright install
```

示例测试脚本 `tests/integration.spec.ts`:

```typescript
import { test, expect } from '@playwright/test';

test('用户登录流程', async ({ page }) => {
  // 访问登录页面
  await page.goto('http://localhost:3000');

  // 填写登录表单
  await page.fill('input[placeholder="用户名"]', 'admin');
  await page.fill('input[placeholder="密码"]', 'admin123');

  // 点击登录按钮
  await page.click('button:has-text("登录")');

  // 验证跳转到Dashboard
  await expect(page).toHaveURL('http://localhost:3000/dashboard');

  // 验证WebSocket连接状态
  await expect(page.locator('text=已连接')).toBeVisible();
});

test('性能监控数据显示', async ({ page }) => {
  // 登录
  await page.goto('http://localhost:3000');
  await page.fill('input[placeholder="用户名"]', 'admin');
  await page.fill('input[placeholder="密码"]', 'admin123');
  await page.click('button:has-text("登录")');

  // 等待Dashboard加载
  await page.waitForSelector('.dashboard-container');

  // 验证性能指标卡片存在
  await expect(page.locator('text=CPU使用率')).toBeVisible();
  await expect(page.locator('text=内存使用率')).toBeVisible();
  await expect(page.locator('text=帧率')).toBeVisible();
  await expect(page.locator('text=网络延迟')).toBeVisible();

  // 等待数据更新
  await page.waitForTimeout(2000);

  // 验证图表渲染
  const charts = page.locator('.chart-container');
  await expect(charts).toHaveCount(4);
});
```

运行测试:

```bash
npx playwright test
```

---

## 持续集成(CI)

### GitHub Actions配置示例

`.github/workflows/integration-test.yml`:

```yaml
name: Integration Tests

on: [push, pull_request]

jobs:
  integration-test:
    runs-on: ubuntu-latest

    services:
      backend:
        image: visionforge-backend:latest
        ports:
          - 8765:8765

    steps:
      - uses: actions/checkout@v3

      - name: Setup Node.js
        uses: actions/setup-node@v3
        with:
          node-version: '18'

      - name: Install frontend dependencies
        working-directory: ./frontend
        run: npm ci

      - name: Build frontend
        working-directory: ./frontend
        run: npm run build

      - name: Run integration tests
        working-directory: ./frontend
        run: npm run test:e2e
```

---

## 总结

集成测试是确保VisionForge Pro系统正常运行的关键步骤。遵循本指南,您可以:

1. ✅ 正确启动前后端服务
2. ✅ 验证WebSocket连接
3. ✅ 测试所有核心功能
4. ✅ 识别和解决常见问题
5. ✅ 进行性能测试
6. ✅ 自动化集成测试流程

**下一步:**
- 查看[用户手册](USER_MANUAL.md)了解详细使用说明
- 查看[API文档](API_REFERENCE.md)了解接口详情
- 查看[安全架构文档](SECURITY_ARCHITECTURE.md)了解安全设计

---

**文档版本:** v1.2.0
**最后更新:** 2025-12-20
**维护者:** VisionForge Team
