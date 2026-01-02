# VisionForge Pro - 环境配置脚本
# 用于 VS2022, VSCode, Antigravity 等 IDE

param(
    [switch]$Permanent,
    [switch]$Debug
)

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "VisionForge Pro 环境配置" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan

# 定义路径
$QtPath = "F:\Qt\6.9.3\msvc2022_64"
$OpenCVPath = "D:\Program Files\OPENCV CUDA"
$HalconPath = "F:\Halcon\HALCON-24.11-Progress-Steady"
$CUDAPath = "D:\Program Files\CUDA12.5"
$VSPath = "F:\Program Files\Microsoft Visual Studio\2022\Professional"

# 验证路径
$paths = @{
    "Qt" = "$QtPath\bin"
    "OpenCV" = "$OpenCVPath\x64\vc17\bin"
    "Halcon" = "$HalconPath\bin\x64-win64"
    "CUDA" = "$CUDAPath\bin"
    "VS2022" = "$VSPath\Common7\IDE"
}

Write-Host "`n检查依赖路径..." -ForegroundColor Yellow
foreach ($name in $paths.Keys) {
    $path = $paths[$name]
    if (Test-Path $path) {
        Write-Host "  [OK] $name : $path" -ForegroundColor Green
    } else {
        Write-Host "  [MISSING] $name : $path" -ForegroundColor Red
    }
}

# 设置环境变量
Write-Host "`n设置环境变量..." -ForegroundColor Yellow

$env:HALCONROOT = $HalconPath
$env:QT_QPA_PLATFORM_PLUGIN_PATH = "$QtPath\plugins\platforms"

# 构建 PATH
$newPaths = @(
    "$QtPath\bin",
    "$OpenCVPath\x64\vc17\bin",
    "$HalconPath\bin\x64-win64",
    "$CUDAPath\bin"
)

foreach ($p in $newPaths) {
    if ($env:PATH -notlike "*$p*") {
        $env:PATH = "$p;$env:PATH"
    }
}

Write-Host "  HALCONROOT = $env:HALCONROOT" -ForegroundColor Green
Write-Host "  QT_QPA_PLATFORM_PLUGIN_PATH = $env:QT_QPA_PLATFORM_PLUGIN_PATH" -ForegroundColor Green

# 如果需要永久设置
if ($Permanent) {
    Write-Host "`n永久设置环境变量（用户级）..." -ForegroundColor Yellow
    [Environment]::SetEnvironmentVariable("HALCONROOT", $HalconPath, "User")
    [Environment]::SetEnvironmentVariable("QT_QPA_PLATFORM_PLUGIN_PATH", "$QtPath\plugins\platforms", "User")

    $currentPath = [Environment]::GetEnvironmentVariable("PATH", "User")
    foreach ($p in $newPaths) {
        if ($currentPath -notlike "*$p*") {
            $currentPath = "$p;$currentPath"
        }
    }
    [Environment]::SetEnvironmentVariable("PATH", $currentPath, "User")
    Write-Host "  环境变量已永久保存到用户配置" -ForegroundColor Green
}

# 显示 CMake 预设信息
Write-Host "`n========================================" -ForegroundColor Cyan
Write-Host "可用的 CMake 预设:" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host "  debug          - Ninja Debug 配置 (VSCode/Antigravity)" -ForegroundColor White
Write-Host "  release        - Ninja Release 配置" -ForegroundColor White
Write-Host "  relwithdebinfo - Ninja RelWithDebInfo 配置" -ForegroundColor White
Write-Host "  vs2022-debug   - VS2022 Debug 配置" -ForegroundColor White
Write-Host "  vs2022-release - VS2022 Release 配置" -ForegroundColor White

Write-Host "`n========================================" -ForegroundColor Cyan
Write-Host "使用示例:" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host "  # VSCode/Antigravity Debug 构建" -ForegroundColor Gray
Write-Host "  cmake --preset debug" -ForegroundColor White
Write-Host "  cmake --build --preset debug-build" -ForegroundColor White
Write-Host ""
Write-Host "  # VS2022 Debug 构建" -ForegroundColor Gray
Write-Host "  cmake --preset vs2022-debug" -ForegroundColor White
Write-Host "  cmake --build --preset vs2022-debug-build" -ForegroundColor White
Write-Host ""
Write-Host "  # 打开 VS2022 解决方案" -ForegroundColor Gray
Write-Host "  start build\vs2022\VisionForge.sln" -ForegroundColor White

Write-Host "`n环境配置完成!" -ForegroundColor Green
