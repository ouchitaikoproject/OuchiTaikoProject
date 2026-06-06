# Tantrum Telemetry Capture

Use this command from repo root:

```powershell
powershell -ExecutionPolicy Bypass -File .\scripts\capture_tantrum_logs.ps1 -Runs 4
```

Optional explicit COM port:

```powershell
powershell -ExecutionPolicy Bypass -File .\scripts\capture_tantrum_logs.ps1 -Port COM7 -Runs 4
```

What it does:
- Creates a timestamped folder at `logs/calibration/YYYYMMDD_HHMMSS/`
- Saves:
  - `session.txt`
  - `raw.log`
  - `tantrum_runs.csv`
- Updates `logs/calibration/latest.txt` with the latest folder path

Recommended workflow:
1. Run auto-calibration in your normal gameplay mode (for example Tatacon).
2. After calibration completes, switch to `Calibrate/Debug` mode.
3. Start this script to download persisted telemetry from flash.
4. Repeat calibration + debug-download as needed.
