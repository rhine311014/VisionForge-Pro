# VisionForge Pro 部署脚本
# 用于创建包含所有运行时依赖的部署文件夹

param(
    [string]$OutputDir = "D:\KK\DESK\VisionForge Pro\deploy",
    [string]$BuildDir = "D:\KK\DESK\VisionForge Pro\build\bin\Release"
)

Write-Host "============================================" -ForegroundColor Cyan
Write-Host "VisionForge Pro 部署工具" -ForegroundColor Cyan
Write-Host "============================================" -ForegroundColor Cyan

# 检查构建目录
if (-not (Test-Path $BuildDir)) {
    Write-Host "错误: 构建目录不存在: $BuildDir" -ForegroundColor Red
    exit 1
}

# 检查exe是否存在
$exePath = Join-Path $BuildDir "VisionForge.exe"
if (-not (Test-Path $exePath)) {
    Write-Host "错误: VisionForge.exe不存在" -ForegroundColor Red
    exit 1
}

# 创建输出目录
if (Test-Path $OutputDir) {
    Write-Host "清理旧的部署目录..." -ForegroundColor Yellow
    Remove-Item -Path $OutputDir -Recurse -Force
}

Write-Host "创建部署目录: $OutputDir" -ForegroundColor Green
New-Item -ItemType Directory -Path $OutputDir -Force | Out-Null

# 需要排除的文件模式
$excludePatterns = @(
    "*.log",
    "*.pdb",
    "*.shm",
    "*.gshm",
    "*_thumb.png",
    "nul"
)

# 复制主程序
Write-Host "`n[1/5] 复制主程序..." -ForegroundColor Green
Copy-Item -Path $exePath -Destination $OutputDir

# 复制DLL文件
Write-Host "[2/5] 复制DLL文件..." -ForegroundColor Green
$dllFiles = Get-ChildItem -Path $BuildDir -Filter "*.dll"
$copiedDlls = 0
foreach ($dll in $dllFiles) {
    Copy-Item -Path $dll.FullName -Destination $OutputDir
    $copiedDlls++
}
Write-Host "  - 复制了 $copiedDlls 个DLL文件" -ForegroundColor Gray

# 复制Qt插件目录
Write-Host "[3/5] 复制Qt插件目录..." -ForegroundColor Green
$qtPluginDirs = @(
    "platforms",
    "imageformats",
    "styles",
    "iconengines",
    "networkinformation",
    "tls",
    "generic"
)

foreach ($dir in $qtPluginDirs) {
    $srcDir = Join-Path $BuildDir $dir
    if (Test-Path $srcDir) {
        $destDir = Join-Path $OutputDir $dir
        Copy-Item -Path $srcDir -Destination $destDir -Recurse -Force
        Write-Host "  - $dir" -ForegroundColor Gray
    }
}

# 复制其他必要目录
Write-Host "[4/5] 复制资源目录..." -ForegroundColor Green
$otherDirs = @(
    "translations",
    "recipes",
    "model_library"
)

foreach ($dir in $otherDirs) {
    $srcDir = Join-Path $BuildDir $dir
    if (Test-Path $srcDir) {
        $destDir = Join-Path $OutputDir $dir
        Copy-Item -Path $srcDir -Destination $destDir -Recurse -Force
        Write-Host "  - $dir" -ForegroundColor Gray
    }
}

# 创建启动脚本
Write-Host "[5/5] 创建启动脚本..." -ForegroundColor Green
$launcherContent = @"
@echo off
cd /d "%~dp0"
start "" "VisionForge.exe"
"@
Set-Content -Path (Join-Path $OutputDir "启动VisionForge.bat") -Value $launcherContent -Encoding Default

# 创建说明文件
$readmeContent = @"
VisionForge Pro 部署包
=======================

运行方式:
  双击 "启动VisionForge.bat" 或直接运行 VisionForge.exe

系统要求:
  - Windows 10/11 64位
  - 显卡: 支持OpenGL 3.3+
  - 内存: 建议8GB以上

可选依赖:
  - NVIDIA GPU (CUDA加速需要): 需要安装NVIDIA显卡驱动
  - Halcon许可证: 如需使用Halcon功能，需要安装Halcon许可证

注意事项:
  1. 如果程序无法启动，请安装 Visual C++ Redistributable 2019
     下载地址: https://aka.ms/vs/17/release/vc_redist.x64.exe

  2. 如果提示缺少DLL，请确保所有文件都已正确解压

  3. Halcon功能需要有效的Halcon许可证才能使用

部署日期: $(Get-Date -Format "yyyy-MM-dd HH:mm:ss")
"@
Set-Content -Path (Join-Path $OutputDir "说明.txt") -Value $readmeContent -Encoding UTF8

# 统计信息
Write-Host "`n============================================" -ForegroundColor Cyan
Write-Host "部署完成!" -ForegroundColor Green
Write-Host "============================================" -ForegroundColor Cyan

$totalFiles = (Get-ChildItem -Path $OutputDir -Recurse -File).Count
$totalSize = (Get-ChildItem -Path $OutputDir -Recurse -File | Measure-Object -Property Length -Sum).Sum
$totalSizeMB = [math]::Round($totalSize / 1MB, 2)
$totalSizeGB = [math]::Round($totalSize / 1GB, 2)

Write-Host "部署目录: $OutputDir" -ForegroundColor White
Write-Host "文件总数: $totalFiles" -ForegroundColor White
Write-Host "总大小: $totalSizeMB MB ($totalSizeGB GB)" -ForegroundColor White
Write-Host "`n提示: 将deploy文件夹复制到目标电脑即可运行" -ForegroundColor Yellow
