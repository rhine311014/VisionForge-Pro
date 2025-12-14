# VisionForge Pro - 依赖库版本清单

## 文档信息

- **版本**: v1.0
- **更新日期**: 2025-12-14
- **维护者**: VisionForge Team

---

## 📦 必需依赖库

### 核心依赖

| 库名 | 版本 | 许可证 | 用途 | 官方网站 |
|------|------|--------|------|---------|
| **Qt** | 6.9.3 | LGPL-3.0 / 商业 | UI框架 | https://www.qt.io |
| **OpenCV** | 4.8.0+ | Apache-2.0 | 计算机视觉算法 | https://opencv.org |
| **CMake** | 3.20+ | BSD-3-Clause | 构建系统 | https://cmake.org |

### 编译工具

| 工具 | 版本 | 说明 |
|------|------|------|
| **Visual Studio** | 2022 Professional | MSVC 19.38+ 编译器 |
| **Git** | 2.40+ | 版本控制 |

---

## 🔧 可选依赖库

### 高级视觉算法（可选）

| 库名 | 版本 | 许可证 | 说明 |
|------|------|--------|------|
| **Halcon** | 24.11 Progress | 商业许可 | 高级图像处理算法 |

**配置方式**：
```cmake
cmake .. -DUSE_HALCON=ON -DHALCONROOT="F:/Halcon/HALCON-24.11-Progress-Steady"
```

**注意事项**：
- ⚠️ 需要购买商业许可证
- 💡 MVP阶段可暂不使用，用OpenCV替代
- 📄 许可证文件位置：`$HALCONROOT/license/license.dat`

---

### AI推理引擎（可选）

#### ONNX Runtime

| 项目 | 版本 | 许可证 |
|------|------|--------|
| **ONNX Runtime** | 1.16.3 | MIT |

**下载链接**：
```
https://github.com/microsoft/onnxruntime/releases/download/v1.16.3/onnxruntime-win-x64-gpu-1.16.3.zip
```

**配置方式**：
```cmake
cmake .. -DUSE_ONNX_RUNTIME=ON -DONNXRUNTIME_DIR="F:/Program Files/onnxruntime"
```

**支持平台**：
- CPU版本：所有x64平台
- GPU版本：需要CUDA 11.x/12.x

---

#### TensorRT

| 项目 | 版本 | 许可证 |
|------|------|--------|
| **TensorRT** | 8.6.1 | NVIDIA专有 |
| **CUDA Toolkit** | 12.0+ | NVIDIA专有 |
| **cuDNN** | 8.9.0+ | NVIDIA专有 |

**配置方式**：
```cmake
cmake .. -DUSE_TENSORRT=ON -DTensorRT_DIR="C:/Program Files/NVIDIA/TensorRT-8.6.1"
```

**注意事项**：
- ⚠️ 仅支持NVIDIA GPU
- 💡 性能提升明显（10-50x）
- 📋 需要NVIDIA开发者账号下载

---

## 📋 详细版本矩阵

### Qt 6.9.3

**所需模块**：

| 模块 | 版本 | 说明 |
|------|------|------|
| Qt6::Core | 6.9.3 | 核心功能 |
| Qt6::Widgets | 6.9.3 | 桌面UI组件 |
| Qt6::Gui | 6.9.3 | GUI基础 |
| Qt6::Network | 6.9.3 | 网络通信 |
| Qt6::Sql | 6.9.3 | 数据库访问 |
| Qt6::Xml | 6.9.3 | XML解析 |

**编译器兼容性**：
- ✅ MSVC 2019 (v142)
- ✅ MSVC 2022 (v143) - 推荐
- ❌ MinGW (不推荐用于生产环境)

**安装路径**：
```
F:\Qt\6.9.3\msvc2019_64\
```

---

### OpenCV 4.8.0

**所需模块**：

| 模块 | 说明 |
|------|------|
| opencv_core | 核心数据结构和函数 |
| opencv_imgproc | 图像处理 |
| opencv_imgcodecs | 图像编解码 |
| opencv_highgui | 图像显示（可选） |
| opencv_videoio | 视频IO |
| opencv_features2d | 特征检测（可选） |
| opencv_calib3d | 相机标定（可选） |

**编译选项**：
- WITH_CUDA: OFF (默认)
- WITH_OPENCL: ON (启用OpenCL加速)
- BUILD_SHARED_LIBS: ON
- BUILD_opencv_world: OFF

**安装路径**：
```
F:\Program Files\OPENCV\build\
```

---

### Halcon 24.11

**所需组件**：

| 组件 | 说明 |
|------|------|
| halconcpp.lib | C++ API |
| halcon.dll | 运行时库 |
| license.dat | 许可证文件 |

**许可证类型**：
- 开发许可：包含HDevelop IDE
- 运行时许可：仅包含Runtime
- 浮动许可：网络授权

**安装路径**：
```
F:\Halcon\HALCON-24.11-Progress-Steady\
```

---

## 🔒 许可证合规性

### 开源许可证

| 依赖库 | 许可证 | 商业使用 | 分发要求 |
|--------|--------|---------|---------|
| Qt | LGPL-3.0 | ✅ 允许（动态链接） | 需提供LGPL库源码链接 |
| OpenCV | Apache-2.0 | ✅ 允许 | 保留版权声明 |
| ONNX Runtime | MIT | ✅ 允许 | 保留版权声明 |

**Qt LGPL合规要点**：
- ✅ 动态链接Qt库（不修改Qt源码）
- ✅ 提供Qt库下载链接
- ✅ 允许用户替换Qt DLL
- ❌ 不可静态链接Qt（除非购买商业许可）

---

### 商业许可证

| 依赖库 | 许可证类型 | 费用估算 | 购买方式 |
|--------|-----------|---------|---------|
| Halcon | 节点锁定 | ¥15,000-50,000/节点 | 联系MVTec代理商 |
| Qt (商业版) | 开发者许可 | $5,000-10,000/年/开发者 | https://www.qt.io/pricing |

**建议**：
- 💡 MVP阶段使用开源方案（Qt LGPL + OpenCV）
- 💡 Halcon作为可选模块，按需购买
- 💡 正式产品发布前购买Qt商业许可（如需静态链接）

---

## 📥 下载链接

### 官方下载

| 软件 | 下载链接 |
|------|---------|
| Qt 6.9.3 | https://www.qt.io/download-open-source |
| OpenCV 4.8.0 | https://github.com/opencv/opencv/releases/tag/4.8.0 |
| CMake | https://cmake.org/download/ |
| Visual Studio 2022 | https://visualstudio.microsoft.com/zh-hans/downloads/ |
| Git | https://git-scm.com/download/win |
| ONNX Runtime | https://github.com/microsoft/onnxruntime/releases |
| TensorRT | https://developer.nvidia.com/tensorrt (需登录) |

---

### 第三方镜像（国内加速）

| 软件 | 镜像地址 |
|------|---------|
| Qt | https://mirrors.tuna.tsinghua.edu.cn/qt/ |
| OpenCV | https://mirrors.tuna.tsinghua.edu.cn/github-release/opencv/opencv/ |

---

## 🛠️ 安装脚本

### 自动安装依赖（PowerShell）

创建 `install_dependencies.ps1`：

```powershell
# VisionForge Pro - 依赖库安装脚本

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "VisionForge Pro 依赖库安装" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

# 检查管理员权限
if (-NOT ([Security.Principal.WindowsPrincipal][Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole([Security.Principal.WindowsBuiltInRole] "Administrator"))
{
    Write-Warning "请以管理员身份运行此脚本！"
    pause
    exit
}

# 安装Chocolatey（包管理器）
if (!(Get-Command choco -ErrorAction SilentlyContinue)) {
    Write-Host "[1/5] 安装 Chocolatey..." -ForegroundColor Yellow
    Set-ExecutionPolicy Bypass -Scope Process -Force
    [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072
    iex ((New-Object System.Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1'))
}

# 安装CMake
Write-Host "[2/5] 安装 CMake..." -ForegroundColor Yellow
choco install cmake --installargs 'ADD_CMAKE_TO_PATH=System' -y

# 安装Git
Write-Host "[3/5] 安装 Git..." -ForegroundColor Yellow
choco install git -y

# 安装Visual Studio Build Tools（如果没有完整VS）
Write-Host "[4/5] 安装 Visual Studio Build Tools..." -ForegroundColor Yellow
choco install visualstudio2022buildtools --package-parameters "--add Microsoft.VisualStudio.Workload.VCTools --includeRecommended" -y

# 提示手动安装Qt和OpenCV
Write-Host "[5/5] 需要手动安装的组件:" -ForegroundColor Yellow
Write-Host "  - Qt 6.9.3: https://www.qt.io/download-open-source" -ForegroundColor White
Write-Host "  - OpenCV 4.8.0: https://github.com/opencv/opencv/releases" -ForegroundColor White
Write-Host "  - Halcon 24.11 (可选): 联系MVTec代理商" -ForegroundColor White

Write-Host ""
Write-Host "========================================" -ForegroundColor Green
Write-Host "基础工具安装完成！" -ForegroundColor Green
Write-Host "========================================" -ForegroundColor Green

pause
```

---

## 📊 依赖库大小

| 库名 | 安装大小 | 下载大小 |
|------|---------|---------|
| Qt 6.9.3 (msvc2019_64) | ~2.5 GB | ~800 MB |
| OpenCV 4.8.0 (预编译) | ~500 MB | ~200 MB |
| Halcon 24.11 | ~1.5 GB | ~600 MB |
| ONNX Runtime GPU | ~300 MB | ~150 MB |
| TensorRT 8.6.1 | ~2.0 GB | ~800 MB |
| CUDA 12.0 Toolkit | ~3.0 GB | ~1.5 GB |
| **总计（全部安装）** | **~10 GB** | **~4 GB** |

**建议**：
- MVP开发仅需：Qt + OpenCV + CMake + VS = ~3.5 GB
- 完整开发环境：上述 + ONNX Runtime = ~4 GB
- AI加速环境：上述 + TensorRT + CUDA = ~9 GB

---

## 🔄 版本更新策略

### 主依赖库

| 库 | 更新频率 | 升级策略 |
|-----|---------|---------|
| Qt | 每6个月发布一个版本 | 跟随LTS版本（6.5/6.8/6.9） |
| OpenCV | 每3-4个月发布一个版本 | 升级到稳定版（x.x.0） |
| CMake | 频繁 | 保持3.20+即可 |

### 可选依赖库

| 库 | 更新频率 | 升级策略 |
|-----|---------|---------|
| Halcon | 每年1-2次 | 保持Progress版本 |
| ONNX Runtime | 每2-3个月 | 跟随稳定版 |
| TensorRT | 每年2-3次 | CUDA版本兼容性优先 |

---

## ⚠️ 已知兼容性问题

### Qt 6.9.3 + MSVC 2022

- ✅ 完全兼容
- 💡 推荐使用 Qt 6.9.3 msvc2019_64 或 msvc2022_64 版本

### OpenCV 4.8 + Qt 6.x

- ✅ 无冲突
- ⚠️ 如果OpenCV编译时启用了Qt支持，可能与项目的Qt版本不一致
- 💡 建议：使用不带Qt支持的OpenCV预编译版

### Halcon 24.11 + OpenCV

- ⚠️ Halcon的图像数据结构与OpenCV的`cv::Mat`不兼容
- 💡 需要编写转换函数（见技术文档）

### TensorRT + ONNX Runtime

- ⚠️ 两者可以共存，但不要同时推理同一模型
- 💡 建议：开发用ONNX Runtime，部署用TensorRT

---

## 📝 版本确认清单

在开始开发前，请确认以下版本：

```bash
# 检查CMake版本
cmake --version
# 期望: cmake version 3.20+

# 检查Git版本
git --version
# 期望: git version 2.40+

# 检查Qt版本
qmake --version
# 期望: QMake version 3.x Using Qt version 6.9.3

# 检查MSVC版本
cl
# 期望: Microsoft (R) C/C++ 优化编译器版本 19.38+

# 检查OpenCV版本（在CMake配置后）
# 查看CMake输出中的 "OpenCV版本: x.x.x"
```

---

## 🆘 支持与帮助

### 官方文档

- Qt文档: https://doc.qt.io/qt-6/
- OpenCV文档: https://docs.opencv.org/4.x/
- CMake文档: https://cmake.org/documentation/
- Halcon文档: https://www.mvtec.com/support

### 社区支持

- Qt论坛: https://forum.qt.io/
- OpenCV论坛: https://forum.opencv.org/
- Stack Overflow: https://stackoverflow.com/

---

**文档维护者**: VisionForge Team
**最后更新**: 2025-12-14
**下次审查**: 2026-03-14
