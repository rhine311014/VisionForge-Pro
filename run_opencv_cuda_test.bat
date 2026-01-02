@echo off
chcp 65001 >nul

set PATH=D:\Program Files\OPENCV CUDA\x64\vc17\bin;%PATH%
set PATH=D:\Program Files\CUDA12.5\bin;%PATH%

cd /d "D:\Program Files\OPENCV CUDA\x64\vc17\bin"

echo ========================================
echo Running OpenCV CUDA Test
echo ========================================
echo.
opencv_test_cudaarithm.exe --gtest_filter=CUDA_Arithm/DeviceInfo.* 2>&1
echo.
echo Return code: %ERRORLEVEL%
echo ========================================
pause
