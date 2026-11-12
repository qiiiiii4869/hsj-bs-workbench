param([switch]$NoBrowser)
$ErrorActionPreference = 'Stop'
$appRoot = $PSScriptRoot
$runtime = Join-Path $appRoot '.runtime'
New-Item -ItemType Directory -Force -Path $runtime | Out-Null
$url = 'http://127.0.0.1:8765'
$browserUrl = $url + '/desktop'
$existingHealth = $null
try {
    $existingHealth = Invoke-RestMethod -Uri "$url/api/health" -TimeoutSec 2
} catch {}
if ($existingHealth -and $existingHealth.application -eq 'hsj-workbench') {
    if ($existingHealth.version -eq '0.17-orbit-classic-layout') {
        if (-not $NoBrowser) { Start-Process $browserUrl }
        Write-Host "HSJ is already running: $url"
        exit 0
    }
    # The stop helper verifies both the executable and this project's script path.
    & (Join-Path $appRoot 'Stop-BS.ps1')
}
$candidates = @((Join-Path (Split-Path $appRoot -Parent) 'runtime/python.exe'), (Join-Path $appRoot '.venv/Scripts/python.exe'))
$pythonCommand = Get-Command python.exe -ErrorAction SilentlyContinue
if ($pythonCommand) { $candidates += $pythonCommand.Source }
$candidates += (Join-Path $env:LOCALAPPDATA 'Programs/Python/Python313/python.exe')
$candidates += (Join-Path $appRoot '.venv/Scripts/python.exe')
$pythonPath = $null
foreach ($candidate in ($candidates | Select-Object -Unique)) {
    if (Test-Path -LiteralPath $candidate) {
        try {
            & $candidate -c 'import sys; assert sys.version_info >= (3,11); import numpy, scipy, PIL, docx, openpyxl, fastapi, uvicorn, anyio' 2>$null
            if ($LASTEXITCODE -eq 0) { $pythonPath = $candidate; break }
        } catch { continue }
    }
}
if (-not $pythonPath) { throw 'Python with the workbench dependencies is required. Install Python 3.11+, then run: python -m pip install -r bs-workbench/requirements.txt' }
$serverFile = Join-Path $appRoot 'server.py'
$process = Start-Process -FilePath $pythonPath -ArgumentList @('-u', ('"' + $serverFile + '"'), '--port', '8765') -WorkingDirectory $appRoot -WindowStyle Hidden -PassThru -RedirectStandardOutput (Join-Path $runtime 'server.out.log') -RedirectStandardError (Join-Path $runtime 'server.err.log')
$ready = $false
for ($attempt = 0; $attempt -lt 40; $attempt++) {
    Start-Sleep -Milliseconds 250
    $process.Refresh()
    if ($process.HasExited) { throw "Service exited. Read $runtime/server.err.log" }
    try {
        $health = Invoke-RestMethod -Uri "$url/api/health" -TimeoutSec 1
        if ($health.application -eq 'hsj-workbench' -and $health.version -eq '0.17-orbit-classic-layout') { $ready = $true; break }
    } catch {}
}
if (-not $ready) { Stop-Process -Id $process.Id -ErrorAction SilentlyContinue; throw 'Service startup timed out.' }
@{ pid = $process.Id; server = $serverFile; python = $pythonPath } | ConvertTo-Json | Set-Content -LiteralPath (Join-Path $runtime 'server.json') -Encoding utf8
if (-not $NoBrowser) { Start-Process $browserUrl }
Write-Host "HSJ is running: $url"
