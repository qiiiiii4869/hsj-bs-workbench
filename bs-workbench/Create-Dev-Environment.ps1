$ErrorActionPreference='Stop'
$transferRoot=Split-Path $PSScriptRoot -Parent
$wheelhouse=Join-Path $transferRoot 'wheelhouse'
$devPython=Join-Path $PSScriptRoot '.venv/Scripts/python.exe'
if(Test-Path -LiteralPath $devPython) {throw 'A development environment already exists; it will not be overwritten.'}
$pythonCandidates=@()
$pythonLauncher=Get-Command py.exe -ErrorAction SilentlyContinue
if($pythonLauncher) {
    $resolved=& $pythonLauncher.Source -3.13 -c 'import sys; print(sys.executable)' 2>$null
    if($LASTEXITCODE -eq 0) {$pythonCandidates+=($resolved | Select-Object -Last 1)}
}
$systemPython=Get-Command python.exe -ErrorAction SilentlyContinue
if($systemPython) {$pythonCandidates+=$systemPython.Source}
$pythonCandidates+=(Join-Path $env:LOCALAPPDATA 'Programs/Python/Python313/python.exe')
$basePython=$null
foreach($candidate in ($pythonCandidates | Select-Object -Unique)) {
    if(Test-Path -LiteralPath $candidate) {
        & $candidate -c 'import sys,struct; assert sys.version_info[:2]==(3,13) and struct.calcsize("P")==8; import venv,ensurepip' 2>$null
        if($LASTEXITCODE -eq 0) {$basePython=$candidate;break}
    }
}
if(-not $basePython) {throw 'Install Python 3.13 x64 with venv/pip for development. Normal use only needs Start-BS.cmd and the bundled runtime.'}
& $basePython -m venv (Join-Path $PSScriptRoot '.venv')
if($LASTEXITCODE -ne 0) {throw 'Development virtual environment creation failed.'}
& $devPython -m pip install --no-index --find-links $wheelhouse -r (Join-Path $PSScriptRoot 'requirements-transfer.lock')
if($LASTEXITCODE -ne 0) {throw 'Offline development dependency installation failed.'}
Write-Host 'Development environment created. No application tests were run.'
Write-Host 'Use bs-workbench/.venv/Scripts/python.exe to run bs-workbench/server.py --port 8765.'
