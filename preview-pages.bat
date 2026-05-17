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
echo Starting local preview at http://localhost:8000
start "" http://localhost:8000
python -m http.server 8000 --directory site_src
