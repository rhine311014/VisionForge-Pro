@echo off
chcp 65001 >nul

call "F:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvars64.bat" >nul 2>&1

cd /d "D:\KK\DESK\VisionForge Pro"

echo.
echo ========================================
echo 删除GPUAccelerator对象文件强制重新编译...
echo ========================================

del /s /q "build\debug\CMakeFiles\VisionForgeLib.dir\src\base\GPUAccelerator.cpp.obj" 2>nul
del /s /q "build\debug\src\CMakeFiles\VisionForgeLib.dir\base\GPUAccelerator.cpp.obj" 2>nul

echo.
echo ========================================
echo 重新编译 (Debug)...
echo ========================================
cmake --build build\debug --parallel 8

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo 构建失败!
    exit /b 1
)

echo.
echo ========================================
echo 构建成功!
echo ========================================

echo.
echo 检查可执行文件时间戳:
dir "build\debug\bin\VisionForge.exe"
