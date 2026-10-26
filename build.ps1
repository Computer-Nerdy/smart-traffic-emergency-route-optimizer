# PowerShell build script for Smart Traffic & Emergency Route Optimizer
$env:Path = [System.Environment]::GetEnvironmentVariable("Path","Machine") + ";" + [System.Environment]::GetEnvironmentVariable("Path","User")
$wingetMinGW = "C:\Users\91807\AppData\Local\Microsoft\WinGet\Packages\BrechtSanders.WinLibs.POSIX.UCRT_Microsoft.Winget.Source_8wekyb3d8bbwe\mingw64\bin"
if (Test-Path $wingetMinGW) {
    $env:Path = "$wingetMinGW;" + $env:Path
}

Write-Host "==========================================================" -ForegroundColor Cyan
Write-Host " Compiling Smart Traffic Optimization Project (GCC C11)    " -ForegroundColor Cyan
Write-Host " SRM Institute of Science and Technology, Ramapuram       " -ForegroundColor Cyan
Write-Host "==========================================================" -ForegroundColor Cyan

$sources = @(
    "src/main.c",
    "src/min_heap.c",
    "src/city_graph.c",
    "src/dijkstra.c",
    "src/iot_sensor.c",
    "src/cli.c"
)

$target = "smart_traffic.exe"

gcc -Wall -Wextra -O2 -std=c11 -Iinclude -o $target $sources

if ($LASTEXITCODE -eq 0) {
    Write-Host "[SUCCESS] Compilation successful! Generated $target" -ForegroundColor Green
} else {
    Write-Host "[ERROR] Compilation failed with exit code $LASTEXITCODE" -ForegroundColor Red
    exit $LASTEXITCODE
}
