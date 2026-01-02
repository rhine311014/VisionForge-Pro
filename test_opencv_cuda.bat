@echo off
chcp 65001 >nul
call "F:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvars64.bat" >nul 2>&1

set PATH=D:\Program Files\OPENCV CUDA\x64\vc17\bin;%PATH%
set PATH=D:\Program Files\CUDA12.5\bin;%PATH%

cd /d "D:\Program Files\OPENCV CUDA\x64\vc17\bin"

echo ========================================
echo 运行 OpenCV CUDA 测试
echo ========================================
echo.
echo 检测 CUDA 设备...
opencv_test_cudaarithm.exe --gtest_filter=*DeviceInfo* 2>&1

echo.
echo ========================================
pause
