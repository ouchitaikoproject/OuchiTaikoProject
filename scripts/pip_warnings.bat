@echo off
title Python Site-Packages Cleanup
echo Searching for invalid 'tilde' distributions...
set "SITE_PACKAGES=C:\Users\mattb\AppData\Local\Python\pythoncore-3.14-64\Lib\site-packages"

if exist "%SITE_PACKAGES%" (
    pushd "%SITE_PACKAGES%"
    :: This looks for any folder starting with ~
    for /d %%i in (~*) do (
        echo Removing ghost folder: %%i
        rd /s /q "%%i"
    )
    popd
    echo.
    echo Cleanup complete! Running a test check...
    :: Using your preferred -m pip command
    python -m pip list
) else (
    echo Error: Could not find the site-packages folder.
)
echo.
pause