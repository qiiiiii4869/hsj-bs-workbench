$ErrorActionPreference = 'Stop'
$expected = Join-Path $PSScriptRoot 'server.py'
$record = Join-Path $PSScriptRoot '.runtime/server.json'
$scriptArgument = '"' + $expected + '"'
$projectProcesses = @(Get-CimInstance Win32_Process -Filter "Name='python.exe'" | Where-Object {
    $_.CommandLine -and $_.CommandLine.Contains($scriptArgument) -and
    $_.CommandLine -match '--port\s+8765(?:\s|$)'
})
if (Test-Path -LiteralPath $record) {
    $info = Get-Content -LiteralPath $record -Raw | ConvertFrom-Json
    $ownedProcess = Get-CimInstance Win32_Process -Filter "ProcessId=$([int]$info.pid)"
    if ($ownedProcess -and ($ownedProcess.ExecutablePath -ne $info.python -or -not $ownedProcess.CommandLine.Contains($scriptArgument))) {
        throw 'Process identity does not match this HSJ service.'
    }
}
# Windows virtualenv launchers may create a second Python process which owns
# the listener. Stop verified children before launchers, including orphaned
# listeners whose runtime record was lost.
$verifiedIds = @($projectProcesses | ForEach-Object { [int]$_.ProcessId })
foreach ($child in ($projectProcesses | Where-Object { $verifiedIds -contains [int]$_.ParentProcessId })) {
    Stop-Process -Id ([int]$child.ProcessId) -ErrorAction SilentlyContinue
}
foreach ($launcher in $projectProcesses) {
    Stop-Process -Id ([int]$launcher.ProcessId) -ErrorAction SilentlyContinue
}
if (Test-Path -LiteralPath $record) { Remove-Item -LiteralPath $record }
Write-Host 'HSJ service stopped.'
