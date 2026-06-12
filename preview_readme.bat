@echo off
cd /d "%~dp0"

echo Starting README preview at http://localhost:8001/readme_preview.html

powershell -NoProfile -Command "try { Invoke-WebRequest -UseBasicParsing http://localhost:8001/readme_preview.html -TimeoutSec 2 | Out-Null; exit 0 } catch { exit 1 }"
if errorlevel 1 (
  start "README Preview Server" cmd /k python -m http.server 8001
  timeout /t 2 >nul
)

start "" http://localhost:8001/readme_preview.html
