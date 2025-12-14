# 第三方库目录

本目录用于存放第三方库的源码或预编译库（可选）。

## 说明

VisionForge Pro 的依赖库主要通过以下方式管理：

1. **系统级安装**（推荐）
   - Qt: 安装到 `F:\Qt\6.9.3`
   - OpenCV: 安装到 `F:\Program Files\OPENCV`
   - Halcon: 安装到 `F:\Halcon\HALCON-24.11-Progress-Steady`

2. **本目录存放**（可选）
   - 如果需要特定版本或修改过的第三方库，可以放在这里
   - Git会忽略这些库文件（见.gitignore）

详细的依赖库安装说明，请参考：[DEPENDENCIES.md](../DEPENDENCIES.md)
