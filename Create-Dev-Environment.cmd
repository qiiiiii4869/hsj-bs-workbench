@echo off
powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0bs-workbench\Create-Dev-Environment.ps1"
if errorlevel 1 pause
