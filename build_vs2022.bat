@echo off
REM ============================================================
REM VisionForge Pro - VS2022 解决方案构建脚本
REM ============================================================

REM 初始化 VS2022 开发者环境
call "F:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvars64.bat"

REM 设置依赖路径
set PATH=D:\Program Files\OPENCV CUDA\x64\vc17\bin;%PATH%
set PATH=F:\Qt\6.9.3\msvc2022_64\bin;%PATH%
set HALCONROOT=F:\Halcon\HALCON-24.11-Progress-Steady
set PATH=%HALCONROOT%\bin\x64-win64;%PATH%
set PATH=D:\Program Files\CUDA12.5\bin;%PATH%
set QT_QPA_PLATFORM_PLUGIN_PATH=F:\Qt\6.9.3\msvc2022_64\plugins\platforms

cd /d "D:\KK\DESK\VisionForge Pro"

echo.
echo ========================================
echo 配置 CMake (VS2022)...
echo ========================================
cmake --preset vs2022-debug

if %ERRORLEVEL% NEQ 0 (
    echo CMake 配置失败!
    pause
    exit /b 1
)

echo.
echo ========================================
echo 打开 Visual Studio 2022...
echo ========================================
start "" "F:\Program Files\Microsoft Visual Studio\2022\Professional\Common7\IDE\devenv.exe" "D:\KK\DESK\VisionForge Pro\build\vs2022\VisionForge.sln"

echo.
echo VS2022 已启动，解决方案已打开
echo.
