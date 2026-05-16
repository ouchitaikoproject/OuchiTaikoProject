@echo off
setlocal
set "ROOT=%~dp0.."
set "PAGE=%ROOT%\docs\ouchitaiko-live-calibrate.html"
if not exist "%PAGE%" (
  echo Live calibrate page not found:
  echo %PAGE%
  exit /b 1
)
start "" "%PAGE%"
exit /b 0
