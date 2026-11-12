@echo off
powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0bs-workbench\Stop-BS.ps1"
if errorlevel 1 pause
