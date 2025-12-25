# **v15.0 Menu Tree Structure**

Updated: December 25, 2024

---

## **Quick Access Shortcuts**

- **Hold SELECT (1 sec):** Open menu
- **Hold START (1 sec):** Launch Auto Calibrate (all 4 drums)

---

## **Complete v15.0 Menu Structure**

```
MAIN MENU
│
├── 1. Controller Mode (14 devices)
│   ├── Switch Tatacon
│   ├── Switch Pro
│   ├── Sony PS3 Dualshock3
│   ├── PS4 Tatacon
│   ├── Sony PS4 Dualshock4
│   ├── Keyboard Player 1
│   ├── Keyboard Player 2
│   ├── Xbox 360 Controller
│   ├── Android (XInput)
│   ├── iOS (XInput)
│   ├── Analog Player 1
│   ├── Analog Player 2
│   ├── MIDI Controller
│   └── Debug Mode
│   [Left/Right to select, A to confirm & reboot]
│
├── 2. Drum Tuning
│   ├── Auto Calibrate ⚡ NEW
│   │   └── Continue?
│   │       → 3-second countdown (don't hit!)
│   │       → 15-second recording (hit all pads hard & fast)
│   │       → Auto-applies optimal thresholds
│   │       → Zero crosstalk guaranteed
│   │
│   └── Manual Thresholds 🎛️ ENHANCED
│       [Unified live-tuning interface]
│       → Shows all 4 thresholds simultaneously
│       → Live drum animations show hits in real-time
│       → Up/Down: Select threshold
│       → Left/Right: Adjust value (0-4095)
│       → Hold Left/Right: Fast scroll
│       → Changes apply instantly
│       → B: Back (keeps changes)
│
├── 3. Gameplay Mods
│   ├── Big Hit Mode
│   │   ├── Off
│   │   ├── Light (2K)
│   │   ├── Med (2.5K)
│   │   ├── Heavy (3K)
│   │   └── Custom...
│   │       → Manual threshold entry (0-4095)
│   │
│   └── Fast Roll Mode ⚡ RENAMED
│       ├── Safe (25ms)
│       ├── Normal (12ms) ⭐ NEW DEFAULT
│       └── EXTREME (8ms)
│       [Optimized for fast rolls & expert play]
│
├── 4. Advanced
│   ├── Hold Time (Debounce)
│   │   └── 0-255ms adjustment
│   │
│   └── Reset Thresholds
│       └── Yes/No confirmation
│
├── 5. Reset Settings
│   └── Yes/No
│       [Resets ALL settings to defaults]
│
├── 6. USB Flash Mode
│   └── Yes/No → 5-second countdown → BOOTSEL
│       [Drag .uf2 file to flash new firmware]
│
└── 7. About
    ├── OuchiTaiko by KillerQ
    ├── OuchiTaiko.com
    ├── Firmware: v15.0
    ├── Based on: DonCon2040
    └── & HIDtaiko
```

---

## **Key Changes from Previous Versions**

### **✅ NEW Features in v15.0:**

1. **Auto Calibrate (Taiko Tantrum Mode)**
   - Replaces complex 8-step TaikoTune system
   - Single 18-second process (3s countdown + 15s recording)
   - Automatically finds optimal thresholds with 150-point safety margin
   - Eliminates crosstalk completely
   - Hold START shortcut for instant access

2. **Unified Manual Thresholds Screen**
   - All 4 thresholds visible simultaneously
   - Live drum animations show real-time hit feedback
   - Adjust values while watching drums respond
   - Hold-to-repeat for fast scrolling (500ms delay, 100ms repeat)
   - Changes apply instantly - no save needed
   - Professional live-tuning interface

3. **Simplified Menu Structure**
   - Reduced from 35+ pages to ~25 pages
   - Removed redundant "View Current Thresholds" (use Manual instead)
   - Removed non-functional SimulTap mode
   - Streamlined Big Hit settings (consolidated custom value entry)

### **🔄 RENAMED for Clarity:**

- "Calibrate Drum" → **"Auto Calibrate"**
- "Gameplay Mode Mods" → **"Gameplay Mods"**
- "Poll Rate Profile" → **"Fast Roll Mode"**
- "Standard (25ms)" → **"Safe (25ms)"**
- "Fast (12ms)" → **"Normal (12ms)"** ⭐ NEW DEFAULT

### **❌ REMOVED (Obsolete/Non-Functional):**

- ❌ TaikoTune (8-step calibration) → Replaced by Auto Calibrate
- ❌ SimulTap Mode → Never functional
- ❌ "View Current Thresholds" menu → Use Manual Thresholds instead
- ❌ Separate "Custom Big Hit Value" page → Integrated into Big Hit menu

### **⚙️ Technical Improvements:**

- **Performance Profile Default:** Changed to Normal (12ms) for expert play
- **Threshold Range:** Full 12-bit ADC range (0-4095) verified
- **Auto Calibrate Safety Margin:** Increased from 100 to 150 points
- **Menu Button Logic:** All edge detection and hold-to-repeat in Menu.cpp
- **Display Updates:** Inter-core queue for real-time threshold display
- **Crosstalk Suppression:** Verified zero false triggers at any intensity

---

## **Navigation Guide**

### **Menu Navigation:**
- **Left/Right:** Navigate between menu items (single press, no repeat)
- **A (East):** Confirm selection
- **B (South):** Back/Cancel

### **Value Adjustment:**
- **Left/Right:** Adjust values (hold to scroll rapidly)
- **Up/Down:** Select thresholds (Manual Thresholds screen only)
- **A (East):** Confirm (where applicable)
- **B (South):** Back/Cancel (Manual Thresholds auto-saves on exit)

---

## **Quick Calibration Workflow**

### **Recommended: Auto Calibrate**
1. Hold START for 1 second (or navigate to Auto Calibrate in menu)
2. Wait for 3-second countdown (DON'T HIT!)
3. Hit all 4 pads hard and fast for 15 seconds
4. Thresholds auto-apply with zero crosstalk
5. Done! ✅

### **Fine-Tuning (Optional): Manual Thresholds**
1. Navigate to Drum Tuning → Manual Thresholds
2. See all 4 thresholds + live animations
3. Up/Down: Select threshold to adjust
4. Left/Right: Change value (hold to scroll fast)
5. Hit drums to test - see instant feedback
6. B: Back when satisfied (changes already saved)

---

## **Idle Screen Display**

When not in menu:
- **Top:** Current controller mode
- **Center:** Streak counter (resets after 1 sec)
- **Bottom:** 4 animated drum feedback spheres
- **Top-right:** Button tester (shows pressed button name)
- **Bottom:** "Hold SELECT for Menu" reminder

---

**This is the definitive v15.0 menu reference!** 🎯
