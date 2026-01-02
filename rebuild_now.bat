@echo off
chcp 65001 >nul

call "F:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvars64.bat" >nul 2>&1

cd /d "D:\KK\DESK\VisionForge Pro"

echo ========================================
echo 更新源文件时间戳...
echo ========================================
copy /b "src\base\GPUAccelerator.cpp"+,, "src\base\GPUAccelerator.cpp" >nul

echo.
echo ========================================
echo 删除对象文件...
echo ========================================
del /f "build\debug\src\base\CMakeFiles\VisionForgeBase.dir\GPUAccelerator.cpp.obj" 2>nul

echo.
echo ========================================
echo 开始编译...
echo ========================================
cmake --build build\debug --target VisionForgeBase -j8 2>&1

echo.
echo ========================================
echo 链接主程序...
echo ========================================
cmake --build build\debug --target VisionForge -j8 2>&1

echo.
echo 编译退出码: %ERRORLEVEL%
echo ========================================
