@echo off
cd /d "%~dp0"
python scripts\sync_readme_to_pages.py
if errorlevel 1 (
  echo.
  echo Sync failed.
  pause
  exit /b 1
)
echo.
echo Pages content synced from README.md to site_src\README.md
pause
