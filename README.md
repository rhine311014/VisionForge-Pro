# VisionForge Pro

<div align="center">

![VisionForge Logo](docs/images/logo.png)

**通用工业视觉检测平台**

[![License](https://img.shields.io/badge/license-Proprietary-blue.svg)]()
[![Qt](https://img.shields.io/badge/Qt-6.9.3-green.svg)](https://www.qt.io/)
[![OpenCV](https://img.shields.io/badge/OpenCV-4.8+-orange.svg)](https://opencv.org/)
[![CMake](https://img.shields.io/badge/CMake-3.20+-blue.svg)](https://cmake.org/)
[![Platform](https://img.shields.io/badge/platform-Windows-lightgrey.svg)]()

极致易用 · 算法先进 · 性能极限 · 开放生态

[功能特性](#-功能特性) · [快速开始](#-快速开始) · [文档](#-文档) · [开发指南](#-开发指南)

</div>

---

## 📖 项目简介

VisionForge Pro 是一款面向工业现场的通用视觉检测平台，旨在提供**Keyence级别的易用性**，同时具备**先进的AI算法**和**极致的性能表现**。

### 核心目标

- 🎯 **极致易用性** - 5分钟快速上手，像Keyence一样简单
- 🤖 **算法先进性** - 集成最新AI技术 + 传统CV算法
- ⚡ **性能极限** - 典型场景 <10ms，简单场景 <3ms
- 🔌 **开放生态** - 支持插件扩展和二次开发

### 设计理念

```
简单的事情简单做，复杂的事情也能做

新手模式：向导式，隐藏复杂性
    ↕ 一键切换
专家模式：完整功能，深度定制
```

---

## ✨ 功能特性

### 双模式UI

<table>
<tr>
<td width="50%">

#### 🟢 简单模式
- 步骤列表式界面
- 核心参数滑块调整
- ROI鼠标示教
- 实时预览效果
- **5分钟上手**

</td>
<td width="50%">

#### 🔵 专业模式
- 完整三栏布局
- 项目树管理
- 高级参数配置
- 单步调试功能
- **深度定制**

</td>
</tr>
</table>

### 检测算法

| 类别 | 算法 |
|------|------|
| **传统CV** | 边缘检测、圆检测、模板匹配、Blob分析、颜色识别 |
| **AI检测** | YOLOv5/v8缺陷检测、图像分类、语义分割 |
| **测量** | 距离、角度、面积、坐标测量 |
| **OCR** | 字符识别、条码/二维码识别 |

### 硬件支持

| 设备 | 支持情况 |
|------|---------|
| **工业相机** | GenICam (GigE/USB3) ✅ |
| **光源控制** | 串口/网口光源控制器 ✅ |
| **PLC通信** | Modbus TCP/RTU, OPC UA ✅ |
| **运动控制** | 待支持 🚧 |

---

## 🚀 快速开始

### 前置要求

| 软件 | 版本 | 安装路径 |
|------|------|---------|
| CMake | 3.20+ | `D:\Program Files\CMake` |
| Visual Studio | 2022 | `F:\Program Files\Microsoft Visual Studio\2022` |
| Qt | 6.9.3 | `F:\Qt\6.9.3` |
| OpenCV | 4.8+ | `F:\Program Files\OPENCV` |

📖 详细环境配置请参考：[开发环境配置指南](docs/00-开发环境配置.md)

### 克隆仓库

```bash
git clone https://github.com/your-org/VisionForge-Pro.git
cd VisionForge-Pro
```

### 编译项目

#### 方式1：使用CMake命令行

```bash
# 创建构建目录
mkdir build
cd build

# 配置项目
cmake .. -G "Visual Studio 17 2022" -A x64

# 编译
cmake --build . --config Release

# 运行
.\bin\Release\VisionForge.exe
```

#### 方式2：使用Visual Studio

```bash
# 生成VS解决方案
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022" -A x64

# 打开生成的.sln文件
start VisionForge.sln
```

然后在Visual Studio中按 `F5` 运行。

#### 方式3：直接打开文件夹（推荐）

```
Visual Studio → 文件 → 打开 → 文件夹 → 选择项目根目录
```

Visual Studio会自动识别CMakeLists.txt并配置项目。

---

## 📚 文档

### 设计文档

| 文档 | 说明 |
|------|------|
| [📐 整体架构设计](docs/architecture/01-整体架构设计.md) | 六层架构、双模式设计、性能优化策略 |
| [🔧 核心类图设计](docs/design/02-核心类图设计.md) | UI层、业务层、算法层、HAL层类图 |
| [🔄 交互流程设计](docs/design/03-交互流程设计.md) | 用户操作流程、异常处理流程 |
| [💾 数据结构设计](docs/design/04-数据结构设计.md) | 配方格式、检测结果、数据库Schema |
| [🎨 UI组件设计](docs/design/05-UI组件详细设计.md) | 所有UI组件的详细实现 |

### 开发文档

| 文档 | 说明 |
|------|------|
| [⚙️ 开发环境配置](docs/00-开发环境配置.md) | 环境搭建、工具安装、常见问题 |
| [📦 依赖库清单](DEPENDENCIES.md) | 所有依赖库的版本、许可证、下载链接 |
| [🔨 构建指南](docs/BUILD.md) | CMake配置、编译选项、打包发布 |
| [🧪 测试指南](docs/TESTING.md) | 单元测试、集成测试、性能测试 |

---

## 🛠️ 开发指南

### 项目结构

```
VisionForge Pro/
├── CMakeLists.txt              # 主CMake配置
├── README.md                   # 本文件
├── DEPENDENCIES.md             # 依赖库清单
├── LICENSE                     # 许可证
│
├── docs/                       # 文档目录
│   ├── 00-开发环境配置.md
│   ├── README.md               # 文档导航
│   ├── architecture/           # 架构设计文档
│   └── design/                 # 详细设计文档
│
├── src/                        # 源代码
│   ├── main.cpp                # 主程序入口
│   ├── ui/                     # UI层（Qt界面）
│   ├── core/                   # 核心业务层
│   ├── algorithm/              # 算法服务层
│   ├── hal/                    # 硬件抽象层
│   └── base/                   # 基础设施层
│
├── include/                    # 公共头文件
├── resources/                  # 资源文件
│   ├── icons/                  # 图标
│   ├── images/                 # 图片
│   ├── styles/                 # 样式表
│   └── translations/           # 翻译文件
│
├── test/                       # 测试代码
├── tools/                      # 工具脚本
├── 3rdparty/                   # 第三方库
└── build/                      # 构建目录（不提交）
```

### 编码规范

- **C++标准**: C++17
- **命名风格**:
  - 类名：`PascalCase` (如 `VisionTool`)
  - 函数名：`camelCase` (如 `processImage`)
  - 成员变量：`camelCase_` (如 `currentImage_`)
  - 常量：`UPPER_CASE` (如 `MAX_IMAGE_SIZE`)
- **缩进**: 4个空格
- **注释**: Doxygen风格

### Git工作流

```bash
# 创建功能分支
git checkout -b feature/your-feature-name

# 提交更改
git add .
git commit -m "feat: 添加xxx功能"

# 推送到远程
git push origin feature/your-feature-name

# 创建Pull Request
```

**提交信息规范**（Angular风格）：
- `feat:` 新功能
- `fix:` Bug修复
- `docs:` 文档更新
- `style:` 代码格式调整
- `refactor:` 重构
- `test:` 测试相关
- `chore:` 构建/工具变更

---

## 🗺️ 开发路线图

### Phase 1: MVP (9个月) - 2026年Q3

- [x] 架构设计完成
- [x] 技术文档编写
- [ ] M1 (月1-3): 技术验证
  - [ ] 基础框架
  - [ ] HAL层实现
  - [ ] 单相机Demo
- [ ] M2 (月4-6): Alpha版
  - [ ] 简单模式UI
  - [ ] 8-10个核心工具
  - [ ] 业务逻辑层
- [ ] M3 (月7-9): Beta版
  - [ ] 专业模式UI
  - [ ] 故障诊断系统
  - [ ] 1个行业模板
  - [ ] 种子客户试用

### Phase 2: 增强版 (+6个月) - 2027年Q1

- [ ] 多相机同步
- [ ] AI模型集成（TensorRT）
- [ ] 3个行业模板
- [ ] 高级统计分析

### Phase 3: 生态版 (+6个月) - 2027年Q3

- [ ] 插件SDK
- [ ] 插件市场
- [ ] 云端协同
- [ ] 移动端监控

---

## 🤝 贡献指南

我们欢迎任何形式的贡献！

### 如何贡献

1. Fork本仓库
2. 创建功能分支 (`git checkout -b feature/AmazingFeature`)
3. 提交更改 (`git commit -m 'feat: 添加某某功能'`)
4. 推送到分支 (`git push origin feature/AmazingFeature`)
5. 提交Pull Request

### 报告Bug

请使用 [GitHub Issues](https://github.com/your-org/VisionForge-Pro/issues) 报告Bug，包含以下信息：

- 操作系统和版本
- VisionForge版本
- 复现步骤
- 期望行为
- 实际行为
- 错误日志（如有）

---

## 📄 许可证

本项目采用 **专有许可证**（Proprietary License）。

- ✅ 允许内部使用和测试
- ❌ 禁止未授权的商业分发
- 📧 商业授权请联系：contact@visionforge.com

**第三方库许可证**：
- Qt: LGPL-3.0 / 商业许可
- OpenCV: Apache-2.0
- 详见：[DEPENDENCIES.md](DEPENDENCIES.md)

---

## 👥 团队

### 核心开发团队

- **技术负责人**: [待定]
- **架构师**: [待定]
- **UI/UX设计**: [待定]
- **算法工程师**: [待定]

### 致谢

感谢以下开源项目：

- [Qt](https://www.qt.io/) - 强大的跨平台UI框架
- [OpenCV](https://opencv.org/) - 计算机视觉基础库
- [ONNX Runtime](https://onnxruntime.ai/) - 高性能AI推理引擎

---

## 📞 联系我们

- **项目主页**: https://github.com/your-org/VisionForge-Pro
- **问题反馈**: https://github.com/your-org/VisionForge-Pro/issues
- **电子邮件**: contact@visionforge.com
- **技术文档**: https://docs.visionforge.com

---

## 📊 项目状态

| 指标 | 状态 |
|------|------|
| **开发阶段** | 📋 设计阶段 |
| **代码覆盖率** | 0% (待开发) |
| **文档完成度** | 100% ✅ |
| **测试通过率** | N/A |
| **最后更新** | 2025-12-14 |

---

<div align="center">

**VisionForge Pro** - 锻造极致视觉体验 🔧✨

Made with ❤️ by VisionForge Team

</div>
