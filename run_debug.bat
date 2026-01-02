@echo off
chcp 65001 >nul

REM 设置CUDA环境
set PATH=D:\Program Files\CUDA12.5\bin;%PATH%
set PATH=D:\Program Files\OPENCV CUDA\x64\vc17\bin;%PATH%
set PATH=F:\Qt\6.9.3\msvc2022_64\bin;%PATH%
set HALCONROOT=F:\Halcon\HALCON-24.11-Progress-Steady
set PATH=%HALCONROOT%\bin\x64-win64;%PATH%
set QT_QPA_PLATFORM_PLUGIN_PATH=F:\Qt\6.9.3\msvc2022_64\plugins\platforms

cd /d "D:\KK\DESK\VisionForge Pro\build\debug\bin"

echo Starting VisionForge...
VisionForge.exe
