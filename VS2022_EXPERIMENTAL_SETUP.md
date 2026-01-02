# VisionForge Pro - VS2022 实验实例配置指南

## 环境要求

### 已验证的环境配置

| 组件 | 版本 | 路径 |
|------|------|------|
| Visual Studio 2022 | Professional 17.14.13 | F:\Program Files\Microsoft Visual Studio\2022\Professional |
| Qt | 6.9.3 (msvc2022_64) | F:\Qt\6.9.3\msvc2022_64 |
| OpenCV | 4.10.0 (CUDA) | D:\Program Files\OPENCV CUDA |
| Halcon | 24.11-Progress-Steady | F:\Halcon\HALCON-24.11-Progress-Steady |
| CUDA Toolkit | 12.5.40 | D:\Program Files\CUDA12.5 |
| CMake | 4.2.0-rc2 | 系统 PATH |

## 快速启动

### 方法一：使用批处理脚本（推荐）

双击运行 `start_vs2022_exp.bat`，该脚本会：
1. 自动配置所有必要的环境变量
2. 启动 VS2022 实验实例
3. 打开 VisionForge.sln 解决方案

### 方法二：手动启动

1. 打开命令提示符（管理员权限）
2. 设置环境变量：

```batch
set PATH=D:\Program Files\OPENCV CUDA\x64\vc17\bin;%PATH%
set PATH=F:\Qt\6.9.3\msvc2022_64\bin;%PATH%
set HALCONROOT=F:\Halcon\HALCON-24.11-Progress-Steady
set PATH=%HALCONROOT%\bin\x64-win64;%PATH%
set PATH=D:\Program Files\CUDA12.5\bin;%PATH%
```

3. 启动 VS2022 实验实例：

```batch
"F:\Program Files\Microsoft Visual Studio\2022\Professional\Common7\IDE\devenv.exe" /RootSuffix Exp "D:\KK\DESK\VisionForge Pro\build\VisionForge.sln"
```

## 编译配置

### 推荐的构建配置

- **配置**: Release
- **平台**: x64
- **C++ 标准**: C++17

### 功能模块状态

| 功能 | 状态 |
|------|------|
| Halcon 集成 | ✅ 已启用 |
| OpenMP 并行 | ✅ 已启用 |
| CUDA GPU 加速 | ✅ 已启用 |
| 海康威视 MVS | ❌ 未启用 |
| Basler Pylon | ❌ 未启用 |
| ONNX Runtime | ❌ 未启用 |
| TensorRT | ❌ 未启用 |
| 单元测试 | ✅ 已启用 |

## 关于 VS2022 实验实例

实验实例 (`/RootSuffix Exp`) 是一个独立的 VS 环境，用于：
- 开发和测试 Visual Studio 扩展
- 隔离实验性设置，不影响主 VS 安装
- 调试 VSIX 扩展项目

### 实验实例的特点

1. **独立的配置文件** - 不会影响主 VS 设置
2. **独立的扩展目录** - 用于测试扩展
3. **可以并行运行** - 可以同时运行主实例和实验实例

## 故障排除

### 问题：找不到 Qt 库

确保 Qt 路径正确设置：
```batch
set PATH=F:\Qt\6.9.3\msvc2022_64\bin;%PATH%
```

### 问题：OpenCV DLL 找不到

运行时需要确保 OpenCV bin 目录在 PATH 中：
```batch
set PATH=D:\Program Files\OPENCV CUDA\x64\vc17\bin;%PATH%
```

### 问题：Halcon 初始化失败

检查 HALCONROOT 环境变量：
```batch
set HALCONROOT=F:\Halcon\HALCON-24.11-Progress-Steady
```

## 重新配置 CMake

如需重新生成解决方案：

```batch
cd "D:\KK\DESK\VisionForge Pro"
rmdir /s /q build
mkdir build
cd build
cmake -G "Visual Studio 17 2022" -A x64 -DCMAKE_PREFIX_PATH="F:/Qt/6.9.3/msvc2022_64" ..
```
