$ErrorActionPreference='Stop'
$transferRoot=Split-Path $PSScriptRoot -Parent
$portablePython=Join-Path $transferRoot 'runtime/python.exe'
$wheelhouse=Join-Path $transferRoot 'wheelhouse'
$dependencyLock=Join-Path $PSScriptRoot 'requirements-transfer.lock'
if(-not(Test-Path -LiteralPath $portablePython)) {throw 'Portable runtime is missing; extract the complete transfer archive first.'}
if(-not(Test-Path -LiteralPath $wheelhouse)) {throw 'Offline wheelhouse is missing.'}
& $portablePython -m pip install --no-index --find-links $wheelhouse --upgrade --force-reinstall --no-compile -r $dependencyLock
if($LASTEXITCODE -ne 0) {throw 'Offline dependency repair failed.'}
Write-Host 'Offline runtime dependencies restored. No application tests were run.'
