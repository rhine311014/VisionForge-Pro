@echo off
REM ============================================================
REM VisionForge Pro - VS2022 实验实例启动脚本
REM ============================================================

REM 设置 OpenCV 路径
set PATH=D:\Program Files\OPENCV CUDA\x64\vc17\bin;%PATH%

REM 设置 Qt 路径
set PATH=F:\Qt\6.9.3\msvc2022_64\bin;%PATH%

REM 设置 Halcon 路径
set HALCONROOT=F:\Halcon\HALCON-24.11-Progress-Steady
set PATH=%HALCONROOT%\bin\x64-win64;%PATH%

REM 设置 CUDA 路径
set PATH=D:\Program Files\CUDA12.5\bin;%PATH%

REM 启动 VS2022 实验实例并打开解决方案
echo 正在启动 Visual Studio 2022 实验实例...
echo 解决方案路径: D:\KK\DESK\VisionForge Pro\build\VisionForge.sln
echo.

"F:\Program Files\Microsoft Visual Studio\2022\Professional\Common7\IDE\devenv.exe" /RootSuffix Exp "D:\KK\DESK\VisionForge Pro\build\VisionForge.sln"
