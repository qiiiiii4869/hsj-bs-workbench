@echo off
cd /d "%~dp0"
powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0bs-workbench\Restart-BS.ps1"
if errorlevel 1 pause
