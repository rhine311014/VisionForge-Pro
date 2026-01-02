@echo off
chcp 65001 >nul
call "F:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvars64.bat" >nul 2>&1

cd /d "D:\KK\DESK\VisionForge Pro"

echo ========================================
echo 编译 CUDA 诊断程序...
echo ========================================

cl /EHsc /MD diagnose_cuda.cpp ^
   /I"D:\Program Files\OPENCV CUDA\include" ^
   /link /LIBPATH:"D:\Program Files\OPENCV CUDA\x64\vc17\lib" opencv_world4100.lib ^
   /out:diagnose_cuda.exe

if %ERRORLEVEL% NEQ 0 (
    echo 编译失败!
    pause
    exit /b 1
)

echo.
echo ========================================
echo 运行诊断程序...
echo ========================================
set PATH=D:\Program Files\OPENCV CUDA\x64\vc17\bin;%PATH%
set PATH=D:\Program Files\CUDA12.5\bin;%PATH%

diagnose_cuda.exe

pause
