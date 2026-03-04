@echo off
echo Cleaning up invalid Python distributions...
set "TARGET_DIR=C:\Users\mattb\AppData\Local\Python\pythoncore-3.14-64\Lib\site-packages"

if exist "%TARGET_DIR%" (
    pushd "%TARGET_DIR%"
    for /d %%i in (~*) do (
        echo Deleting ghost folder: %%i
        rd /s /q "%%i"
    )
    popd
    echo.
    echo Done! The warnings should be gone now.
) else (
    echo Path not found. Please check your Python installation path.
)
pause