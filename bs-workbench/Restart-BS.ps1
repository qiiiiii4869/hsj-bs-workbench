param([switch]$NoBrowser)
$ErrorActionPreference = 'Stop'
& (Join-Path $PSScriptRoot 'Stop-BS.ps1')
& (Join-Path $PSScriptRoot 'Start-BS.ps1') -NoBrowser:$NoBrowser
