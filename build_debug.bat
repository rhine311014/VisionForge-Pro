@echo off
REM ============================================================
REM VisionForge Pro - Debug 构建脚本
REM 适用于 VSCode, Antigravity, 命令行
REM ============================================================

REM 初始化 VS2022 开发者环境
call "F:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvars64.bat"

REM 设置依赖路径
set PATH=F:\Program Files\Microsoft Visual Studio\2022\Professional\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja;%PATH%
set PATH=D:\Program Files\OPENCV CUDA\x64\vc17\bin;%PATH%
set PATH=F:\Qt\6.9.3\msvc2022_64\bin;%PATH%
set HALCONROOT=F:\Halcon\HALCON-24.11-Progress-Steady
set PATH=%HALCONROOT%\bin\x64-win64;%PATH%
set PATH=D:\Program Files\CUDA12.5\bin;%PATH%
set QT_QPA_PLATFORM_PLUGIN_PATH=F:\Qt\6.9.3\msvc2022_64\plugins\platforms

cd /d "D:\KK\DESK\VisionForge Pro"

echo.
echo ========================================
echo 配置 CMake (Debug)...
echo ========================================
cmake --preset debug

if %ERRORLEVEL% NEQ 0 (
    echo CMake 配置失败!
    pause
    exit /b 1
)

echo.
echo ========================================
echo 构建项目 (Debug)...
echo ========================================
cmake --build --preset debug-build -j

if %ERRORLEVEL% NEQ 0 (
    echo 构建失败!
    pause
    exit /b 1
)

echo.
echo ========================================
echo 构建完成!
echo 输出目录: build\debug\bin
echo ========================================
pause
