# OpenCV CUDA 编译脚本 (使用已下载的源码)
# 用法: powershell -ExecutionPolicy Bypass -File build_opencv_cuda.ps1

param(
    [string]$OpenCVSourceDir = "F:\Program Files\OPENCV\opencv\sources",
    [string]$BuildDir = "F:\Program Files\OPENCV\opencv\build_cuda",
    [string]$InstallDir = "F:\Program Files\OPENCV\opencv_cuda",
    [string]$CudaPath = "D:\Program Files\NVIDIA CUDA",
    [string]$CudnnPath = "D:\Program Files\cuDNN\cudnn-windows-x86_64-8.9.7.29_cuda12-archive",
    [string]$Generator = "Visual Studio 17 2022"
)

$ErrorActionPreference = "Stop"

Write-Host "============================================" -ForegroundColor Cyan
Write-Host " OpenCV CUDA 编译脚本" -ForegroundColor Cyan
Write-Host "============================================" -ForegroundColor Cyan
Write-Host ""
Write-Host "配置信息:" -ForegroundColor Yellow
Write-Host "  OpenCV源码: $OpenCVSourceDir"
Write-Host "  构建目录: $BuildDir"
Write-Host "  安装目录: $InstallDir"
Write-Host "  CUDA路径: $CudaPath"
Write-Host "  cuDNN路径: $CudnnPath"
Write-Host "  生成器: $Generator"
Write-Host ""

# 验证源码目录
if (-not (Test-Path "$OpenCVSourceDir\CMakeLists.txt")) {
    Write-Host "错误: OpenCV源码目录无效!" -ForegroundColor Red
    exit 1
}

# 创建构建目录
Write-Host "[1/3] 配置CMake..." -ForegroundColor Green
if (-not (Test-Path $BuildDir)) {
    New-Item -ItemType Directory -Path $BuildDir -Force | Out-Null
}
Set-Location $BuildDir

# 检测CUDA架构 (RTX 20系列: 7.5, RTX 30系列: 8.6, RTX 40系列: 8.9)
# 直接调用cmake，避免Invoke-Expression解析问题
Write-Host "  执行CMake配置..." -ForegroundColor Gray

# 转换路径为CMake兼容格式 (使用正斜杠)
$CudaPathCMake = $CudaPath -replace '\\', '/'
$CudnnPathCMake = $CudnnPath -replace '\\', '/'
$InstallDirCMake = $InstallDir -replace '\\', '/'
$OpenCVSourceDirCMake = $OpenCVSourceDir -replace '\\', '/'

& cmake -G "$Generator" -A x64 `
    -D CMAKE_BUILD_TYPE=Release `
    -D CMAKE_INSTALL_PREFIX="$InstallDirCMake" `
    -D WITH_CUDA=ON `
    -D CUDA_TOOLKIT_ROOT_DIR="$CudaPathCMake" `
    -D "CUDA_ARCH_BIN=7.5;8.0;8.6;8.9;9.0" `
    -D CUDA_FAST_MATH=ON `
    -D ENABLE_FAST_MATH=ON `
    -D WITH_CUBLAS=ON `
    -D WITH_CUFFT=ON `
    -D WITH_NVCUVID=OFF `
    -D WITH_NVCUVENC=OFF `
    -D WITH_CUDNN=ON `
    -D CUDNN_INCLUDE_DIR="$CudnnPathCMake/include" `
    -D CUDNN_LIBRARY="$CudnnPathCMake/lib/x64/cudnn.lib" `
    -D OPENCV_DNN_CUDA=ON `
    -D BUILD_opencv_world=ON `
    -D WITH_OPENMP=ON `
    -D WITH_TBB=OFF `
    -D BUILD_TESTS=OFF `
    -D BUILD_PERF_TESTS=OFF `
    -D BUILD_EXAMPLES=OFF `
    -D BUILD_DOCS=OFF `
    -D BUILD_opencv_python3=OFF `
    -D BUILD_opencv_python2=OFF `
    -D BUILD_opencv_java=OFF `
    -D BUILD_WITH_STATIC_CRT=OFF `
    -D OPENCV_ENABLE_NONFREE=ON `
    "$OpenCVSourceDirCMake"

if ($LASTEXITCODE -ne 0) {
    Write-Host "CMake配置失败!" -ForegroundColor Red
    exit 1
}

# 编译
Write-Host "[2/3] 编译OpenCV (这可能需要30-60分钟)..." -ForegroundColor Green
Write-Host "  并行编译线程数: $env:NUMBER_OF_PROCESSORS" -ForegroundColor Gray
cmake --build . --config Release --parallel $env:NUMBER_OF_PROCESSORS

if ($LASTEXITCODE -ne 0) {
    Write-Host "编译失败!" -ForegroundColor Red
    exit 1
}

# 安装
Write-Host "[3/3] 安装OpenCV..." -ForegroundColor Green
cmake --install . --config Release

if ($LASTEXITCODE -ne 0) {
    Write-Host "安装失败!" -ForegroundColor Red
    exit 1
}

Write-Host ""
Write-Host "============================================" -ForegroundColor Green
Write-Host " OpenCV CUDA 编译完成!" -ForegroundColor Green
Write-Host "============================================" -ForegroundColor Green
Write-Host ""
Write-Host "安装路径: $InstallDir" -ForegroundColor Yellow
Write-Host ""
Write-Host "下一步:" -ForegroundColor Cyan
Write-Host "  1. 更新VisionForge Pro的CMakeLists.txt:"
Write-Host "     set(OpenCV_DIR `"$InstallDir`")"
Write-Host ""
Write-Host "  2. 添加到系统PATH:"
Write-Host "     $InstallDir\x64\vc17\bin"
Write-Host ""
Write-Host "  3. 重新配置并编译项目:"
Write-Host "     cd `"d:\KK\DESK\VisionForge Pro`""
Write-Host "     rmdir /s /q build"
Write-Host "     cmake -B build -G `"$Generator`" -A x64"
Write-Host "     cmake --build build --config Release"
