@echo off
cd /d "%~dp0"

echo.
echo Syncing README into site_src...
python scripts\sync_readme_to_pages.py
if errorlevel 1 (
  echo.
  echo Sync failed.
  pause
  exit /b 1
)

echo.
echo Checking preview server...
powershell -NoProfile -Command "try { $r = Invoke-WebRequest -UseBasicParsing http://localhost:8000 -TimeoutSec 2; exit 0 } catch { exit 1 }"
if errorlevel 1 (
  echo Starting preview server at http://localhost:8000
  start "" http://localhost:8000
  start "OuchiTaiko Pages Preview" cmd /k python -m http.server 8000 --directory site_src
) else (
  echo Preview server already running.
  start "" http://localhost:8000
)

echo.
echo Edit README.md, then run sync-pages.bat and refresh the browser with Ctrl+F5.
pause
