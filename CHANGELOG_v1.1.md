# OuchiTaiko Project - Firmware v1.1 Changelog
**Release Date:** November 25, 2025

## 🎯 Summary
Version 1.1 includes all features from v2.0 (two-pass crosstalk detection) PLUS critical bug fixes for drum sensitivity and display initialization. This is the most stable and feature-complete version to date.

---

## ✨ New Features & Major Improvements

### 1. **Two-Pass Bidirectional Crosstalk Detection** (from v2.0)
- **What it does:** Analyzes drums from BOTH directions to eliminate false triggers
- **Impact:** Dramatically reduces unwanted hits from vibrations and crosstalk
- **Pass 1:** Analyzes left→right (Ka-Left → Don-Left → Don-Right → Ka-Right)
- **Pass 2:** Analyzes opposite direction (Ka-Right → Don-Right → Don-Left → Ka-Left)
- **Result:** More accurate threshold calibration for all drum pads

### 2. **"All 4 Drums" Auto-Calibration Sequence Fix** (from v2.0)
- **Problem Fixed:** Taiko-Tune would stop after Ka-Left instead of continuing through all 4 drums
- **Solution:** Complete sequence now works properly:
  - Pass 1: Ka-Left → Don-Left → Don-Right → Ka-Right
  - Pass 2: Ka-Right → Don-Right → Don-Left → Ka-Left
- **User Experience:** 2.5 second results display between drums so you can see threshold values
- **Auto-saves:** Thresholds automatically saved after each drum completes

### 3. **Drum Sensitivity Restoration** (NEW in v1.1)
- **Problem Fixed:** Light, rapid hits weren't registering properly
- **Root Cause:** Missing raw/analog value processing code
- **What was restored:**
  - Raw value assignments immediately after ADC read
  - 12-bit to 16-bit analog scaling conversion
  - Proper USB output and visual feedback accuracy
- **Impact:** Drums now respond correctly to light touches and fast rolls

### 4. **Display Initialization Fix** (NEW in v1.1)
- **Problem Fixed:** OLED display not initializing on boot
- **Root Cause:** Missing `ws2812.pio.h` file after code reorganization
- **Solution:** Restored proper PIO library initialization
- **Impact:** Display now works reliably on every boot

---

## 🐛 Bug Fixes

### Taiko-Tune Improvements
- ✅ Fixed premature exit in "All 4 Drums" mode
- ✅ Fixed frozen menu navigation during calibration
- ✅ Added explicit state handling for single vs. All 4 Drums mode
- ✅ Fixed phase transition timing between passes
- ✅ Added completion screen after All 4 Drums finishes

### Menu & Display
- ✅ Fixed hotkey state reset when exiting Taiko-Tune
- ✅ Improved results display timing (2.5s per drum)
- ✅ Better user feedback during calibration process

### Code Quality
- ✅ Reorganized project structure to standard GitHub layout
- ✅ Removed deprecated backup files and old folders
- ✅ Updated .gitignore for cleaner repository

---

## 📦 What's Included in v1.1

### All v2.0 Features:
- ✅ Two-pass crosstalk detection
- ✅ Complete "All 4 Drums" auto-calibration sequence
- ✅ Bidirectional threshold analysis
- ✅ Improved results display between drums
- ✅ Auto-save after each drum completion

### PLUS New v1.1 Fixes:
- ✅ Drum sensitivity restoration (light hits work again!)
- ✅ Display initialization fix (OLED works on every boot)
- ✅ Status LED support (ws2812.pio.h properly included)
- ✅ Code reorganization (standard GitHub structure)

---

## 🔧 Technical Changes

### Code Files Modified:
- `src/peripherals/Drum.cpp` - Sensitivity fix (raw/analog processing)
- `src/peripherals/Display.cpp` - Maintained from v2.0
- `src/Main.cpp` - All 4 Drums sequence logic from v2.0
- `libs/pio_ws2812/generated/ws2812.pio.h` - Restored for LED support

### Build System:
- ✅ Proper Pico SDK integration via VS Code extension
- ✅ CMake configuration validated
- ✅ All libraries correctly linked (pico_ssd1306, pio_ws2812, etc.)

---

## 🎮 Testing Checklist

### ✅ Verified Working:
- [x] OLED display shows boot splash
- [x] Display shows firmware "v1.1" in About menu
- [x] All 4 Drums auto-calibration completes full sequence
- [x] Light drum hits register correctly
- [x] Fast drum rolls work without missing hits
- [x] Two-pass crosstalk detection runs both directions
- [x] Thresholds save automatically after calibration
- [x] Menu navigation works during and after Taiko-Tune
- [x] Status LED initializes (if connected)
- [x] USB device enumerates correctly ("ding" on boot)

---

## 🚀 How to Flash v1.1

1. **Download:** `build/OuchiTaikoProject.uf2`
2. **Enter Bootloader:**
   - Hold BOOTSEL button on Pico
   - Plug in USB (or press reset)
   - Release BOOTSEL
3. **Flash:** Copy .uf2 file to RPI-RP2 drive
4. **Verify:** Check "About" menu shows "Firmware v1.1"

---

## 📝 Notes

### Differences from v2.0:
- v2.0 was built without Pico extension configuration (caused boot issues)
- v1.1 properly built with Pico SDK extension (stable boot)
- v1.1 adds sensitivity fix that was missing in v2.0
- v1.1 has display initialization fix

### Why Version Number Went Backwards:
- v2.0 was created during development (Nov 23)
- v1.0 was set for "first public release" (Nov 24)
- v1.1 is the bug-fix release (Nov 25)
- All v2.0 features ARE included in v1.1!

---

## 🙏 Credits
- **Original Code:** DonCon2040 (MIT License) & HIDtaiko (Apache 2.0)
- **OuchiTaiko Enhancements:** KillerQ
- **Claude AI Contributions:**
  - Two-pass crosstalk detection algorithm
  - All 4 Drums sequence fixes
  - Sensitivity restoration
  - Display initialization debugging
  - Build system configuration

---

## 🔗 Links
- **Full Guide:** [ouchitaiko.com](https://ouchitaiko.com)
- **GitHub:** [github.com/ouchitaikoproject/OuchiTaikoProject](https://github.com/ouchitaikoproject/OuchiTaikoProject)

---

**Enjoy your perfectly calibrated drums! 🥁✨**
