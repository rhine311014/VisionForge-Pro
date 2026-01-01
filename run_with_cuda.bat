@echo off
REM 设置CUDA和OpenCV CUDA环境变量
set "PATH=D:\Program Files\CUDA12.5\bin;D:\Program Files\OPENCV CUDA\x64\vc17\bin;%PATH%"

REM 运行程序
cd /d "%~dp0build\bin\Release"
start "" VisionForge.exe
