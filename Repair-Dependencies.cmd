@echo off
powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0bs-workbench\Repair-Dependencies.ps1"
if errorlevel 1 pause
