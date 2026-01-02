@echo off
chcp 65001 >nul

call "F:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvars64.bat" >nul 2>&1

echo.
echo ========================================
echo Building VisionForge Pro (Debug)...
echo ========================================

cd /d "D:\KK\DESK\VisionForge Pro"

cmake --build build\debug --parallel 8

if %ERRORLEVEL% NEQ 0 (
    echo Build FAILED with error code: %ERRORLEVEL%
    exit /b %ERRORLEVEL%
) else (
    echo Build SUCCEEDED
)

echo.
echo ========================================
echo Checking executable...
echo ========================================
dir "build\debug\bin\VisionForge.exe"
