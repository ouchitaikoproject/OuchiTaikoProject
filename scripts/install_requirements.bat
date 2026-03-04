@echo off
echo Checking for Python...
python --version >nul 2>&1
if %errorlevel% neq 0 (
    echo Python is not installed or not in your PATH. Please install Python first.
    pause
    exit /b
)

echo.
echo Installing OuchiTaiko script prerequisites using extended command...
echo.
python -m pip install requests beautifulsoup4
echo.
echo Done! All libraries are installed.
pause