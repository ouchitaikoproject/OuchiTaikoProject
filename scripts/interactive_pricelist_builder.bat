@echo off
title OuchiTaiko Interactive Builder
echo Starting the Interactive Pricing Tool...
echo.
echo 1. Split your screen (CMD on one side, Browser on the other).
echo 2. The script will open each link automatically.
echo 3. Enter the price and pack details when prompted.
echo.
python interactive_pricelist_builder.py
echo.
if exist OuchiTaiko_Cost_Summary.md (
    echo Opening your new Markdown report...
    start OuchiTaiko_Cost_Summary.md
)
pause