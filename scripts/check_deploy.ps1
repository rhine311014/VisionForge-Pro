$deployPath = "D:\KK\DESK\VisionForge Pro\deploy"
$files = Get-ChildItem -Path $deployPath -Recurse -File
$size = ($files | Measure-Object -Property Length -Sum).Sum

Write-Host "========================================"
Write-Host "Deploy Folder Summary"
Write-Host "========================================"
Write-Host "Path: $deployPath"
Write-Host "Total Files: $($files.Count)"
Write-Host "Total Size: $([math]::Round($size/1GB, 2)) GB"
Write-Host "========================================"
Write-Host ""
Write-Host "Contents:"
Get-ChildItem -Path $deployPath | ForEach-Object {
    if ($_.PSIsContainer) {
        $subCount = (Get-ChildItem -Path $_.FullName -Recurse -File).Count
        Write-Host "  [DIR] $($_.Name) ($subCount files)"
    } else {
        $sizeMB = [math]::Round($_.Length/1MB, 1)
        Write-Host "  $($_.Name) - $sizeMB MB"
    }
}
