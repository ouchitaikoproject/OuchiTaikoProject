param(
    [string]$Port = "AUTO",
    [int]$Baud = 115200,
    [int]$Runs = 4,
    [int]$PollMs = 250
)

Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

function Resolve-Port {
    param([string]$RequestedPort)

    if ($RequestedPort -and $RequestedPort -ne "AUTO") {
        return $RequestedPort.ToUpperInvariant()
    }

    $ports = [System.IO.Ports.SerialPort]::GetPortNames() | Sort-Object
    if (-not $ports -or $ports.Count -eq 0) {
        throw "No serial ports detected. Connect controller in Debug/Calibrate mode first."
    }

    return $ports[0]
}

function Parse-TantrumLine {
    param([string]$Line)

    if (-not $Line.StartsWith("TLG:VER=")) {
        return $null
    }

    $payload = $Line.Substring(4)
    $parts = @{}
    foreach ($segment in ($payload -split ";")) {
        $kv = $segment -split "=", 2
        if ($kv.Count -eq 2) {
            $parts[$kv[0]] = $kv[1]
        }
    }

    if (-not $parts.ContainsKey("VER")) {
        return $null
    }

    [PSCustomObject]@{
        Version = [int]$parts["VER"]
        NR = $parts["NR"]
        MH = $parts["MH"]
        XT = $parts["XT"]
        TH = $parts["TH"]
        WARN = $parts["WARN"]
        Raw = $payload
    }
}

$repoRoot = Split-Path -Parent $PSScriptRoot
$timestamp = Get-Date -Format "yyyyMMdd_HHmmss"
$runDir = Join-Path $repoRoot ("logs\calibration\" + $timestamp)
New-Item -ItemType Directory -Path $runDir -Force | Out-Null

$resolvedPort = Resolve-Port -RequestedPort $Port
$metaPath = Join-Path $runDir "session.txt"
$rawPath = Join-Path $runDir "raw.log"
$csvPath = Join-Path $runDir "tantrum_runs.csv"

@(
    "timestamp=$timestamp"
    "port=$resolvedPort"
    "baud=$Baud"
    "runs_target=$Runs"
    "poll_ms=$PollMs"
) | Set-Content -Path $metaPath -Encoding UTF8

$serial = [System.IO.Ports.SerialPort]::new($resolvedPort, $Baud, [System.IO.Ports.Parity]::None, 8, [System.IO.Ports.StopBits]::One)
$serial.ReadTimeout = 350
$serial.WriteTimeout = 350
$serial.NewLine = "`n"

$results = New-Object System.Collections.Generic.List[object]
$seenVersions = @{}

try {
    $serial.Open()
    Start-Sleep -Milliseconds 200
    $serial.DiscardInBuffer()

    Write-Host "Logging Tantrum telemetry from $resolvedPort"
    Write-Host "Output folder: $runDir"
    Write-Host "Waiting for $Runs unique calibration run(s)..."

    while ($results.Count -lt $Runs) {
        $serial.Write([char]0xBC)

        try {
            $line = $serial.ReadLine().Trim()
        } catch [System.TimeoutException] {
            Start-Sleep -Milliseconds $PollMs
            continue
        }

        if (-not [string]::IsNullOrWhiteSpace($line)) {
            Add-Content -Path $rawPath -Value ("{0} {1}" -f (Get-Date -Format "o"), $line)
        }

        $parsed = Parse-TantrumLine -Line $line
        if ($null -eq $parsed) {
            Start-Sleep -Milliseconds $PollMs
            continue
        }

        if (-not $seenVersions.ContainsKey($parsed.Version)) {
            $seenVersions[$parsed.Version] = $true
            $record = [PSCustomObject]@{
                Timestamp = Get-Date -Format "o"
                Version = $parsed.Version
                NR = $parsed.NR
                MH = $parsed.MH
                XT = $parsed.XT
                TH = $parsed.TH
                WARN = $parsed.WARN
                Raw = $parsed.Raw
            }
            $results.Add($record)
            Write-Host ("Captured run {0}/{1} (VER={2})" -f $results.Count, $Runs, $parsed.Version)
        }

        Start-Sleep -Milliseconds $PollMs
    }
}
finally {
    if ($serial.IsOpen) {
        $serial.Close()
    }
}

$results | Export-Csv -Path $csvPath -NoTypeInformation -Encoding UTF8

$latestPath = Join-Path $repoRoot "logs\calibration\latest.txt"
$runDir | Set-Content -Path $latestPath -Encoding UTF8

Write-Host ""
Write-Host "Done."
Write-Host "Session: $runDir"
Write-Host "CSV: $csvPath"
