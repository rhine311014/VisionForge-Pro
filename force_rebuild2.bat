@echo off
chcp 65001 >nul

call "F:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvars64.bat" >nul 2>&1

cd /d "D:\KK\DESK\VisionForge Pro"

echo.
echo ========================================
echo 删除GPUAccelerator对象文件...
echo ========================================

if exist "build\debug\src\base\CMakeFiles\VisionForgeBase.dir\GPUAccelerator.cpp.obj" (
    del /f "build\debug\src\base\CMakeFiles\VisionForgeBase.dir\GPUAccelerator.cpp.obj"
    echo 已删除: GPUAccelerator.cpp.obj
) else (
    echo 对象文件不存在
)

echo.
echo ========================================
echo 重新编译 (Debug)...
echo ========================================
cmake --build build\debug --parallel 8 --verbose

echo.
echo ========================================
echo 完成，退出码: %ERRORLEVEL%
echo ========================================
