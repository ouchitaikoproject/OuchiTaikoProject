# Analog Mode Debugging Summary

## Original Problem

**Issue**: OuchiTaiko drum controller in Xbox 360 Analog P1 mode required extremely hard hits to register in arcade games, even with very low thresholds.

**Symptoms**:
- Firmware threshold set to 1
- Arcade game thresholds set to 5-25
- Still needed to hit VERY HARD for drums to register
- Adaptive baseline working correctly (showing 000-045 in arcade test screen)
- Suspected analog output voltage range was too low

## Hardware Specifications

- **ADC**: Raspberry Pi Pico 12-bit ADC (0-4095 range)
- **XInput Protocol**: Xbox 360 controller emulation using int16_t axes (-32768 to +32767)
- **Controller Mode**: USB_MODE_XBOX360_ANALOG_P1
- **Expected Output**: Full-range analog values (0-32767) for XInput

## Bugs Identified

### Bug 1: Missing `.analog` Field Assignment
**File**: `src/peripherals/Drum.cpp`

**Problem**: The `.analog` field in `InputState` was never being set, remaining at 0 throughout execution.

**Original Code** (lines 289-292):
```cpp
// .analog field was never assigned - stayed at 0!
input_state.drum.don_left.raw = m_pads.at(Id::DON_LEFT).getMaxValueInBuffer();
input_state.drum.ka_left.raw = m_pads.at(Id::KA_LEFT).getMaxValueInBuffer();
input_state.drum.don_right.raw = m_pads.at(Id::DON_RIGHT).getMaxValueInBuffer();
input_state.drum.ka_right.raw = m_pads.at(Id::KA_RIGHT).getMaxValueInBuffer();
```

**Initial Fix** (User applied):
```cpp
input_state.drum.don_left.analog = m_pads.at(Id::DON_LEFT).getMaxValueInBuffer();
input_state.drum.ka_left.analog = m_pads.at(Id::KA_LEFT).getMaxValueInBuffer();
input_state.drum.don_right.analog = m_pads.at(Id::DON_RIGHT).getMaxValueInBuffer();
input_state.drum.ka_right.analog = m_pads.at(Id::KA_RIGHT).getMaxValueInBuffer();
```

**Status**: ✅ Fixed (but incomplete - see Bug 3)

---

### Bug 2: Incorrect Analog Scaling (6.25% Range Utilization)
**File**: `src/utils/InputReport.cpp`

**Problem**: Original scaling of `raw >> 1` on 12-bit values (0-4095) only produced output range 0-2047, using just 6.25% of the full XInput range (0-32767).

**Original Code** (line 225):
```cpp
auto map_to_axis = [](uint16_t raw) {
    return (int16_t)(raw >> 1);  // 12-bit: 0-4095 → 0-2047 (only 6.25% of range!)
};
```

**Diagnostic Test** (User applied 256x multiplier):
```cpp
auto map_to_axis = [](uint16_t raw) {
    return (int16_t)(raw * 256);  // Testing: moved sticks to full range in joy.cpl
};
```

**Later Diagnostic Test** (64x multiplier):
```cpp
auto map_to_axis = [](uint16_t raw) {
    return (int16_t)(raw * 64);  // Testing: confirmed full range movement
};
```

**Status**: ⚠️ Addressed by Bug 3 fix (proper bit-shifting makes original >> 1 correct)

---

### Bug 3: Missing 12-bit to 16-bit ADC Transformation
**Files**: `include/peripherals/Drum.h`, `src/peripherals/Drum.cpp`

**Problem**: Original DonCon2040 code includes a critical `getAnalog()` method with bit-shifting transformation to convert 12-bit ADC values (0-4095) to full 16-bit range (0-65535). This transformation was completely missing from the OuchiTaiko implementation.

**Discovery**: Found by comparing user's code to original DonCon2040 source files.

**Original DonCon2040 Code**:
```cpp
uint16_t Drum::Pad::getAnalog() {
    const auto raw_to_uint16 = [](uint16_t raw) {
        return ((raw << 4) & 0xFFF0) | ((raw >> 8) & 0x000F);
    };
    return raw_to_uint16(std::ranges::max_element(m_analog_buffer, ...)->value);
}
```

**Bit-shifting Explanation**:
- `raw << 4`: Left-shift by 4 bits (multiply by 16) to scale 0-4095 → 0-65520
- `& 0xFFF0`: Mask to preserve upper 12 bits in lower 16-bit space
- `raw >> 8`: Right-shift by 8 bits to get upper 4 bits of original value
- `& 0x000F`: Mask to preserve only lower 4 bits
- Combines both to fill full 16-bit range (0-65535)

**Implementation**:

Added to `include/peripherals/Drum.h` (line 238):
```cpp
[[nodiscard]] uint16_t getAnalog() const;
```

Added to `src/peripherals/Drum.cpp` (lines 111-119):
```cpp
uint16_t Drum::Pad::getAnalog() const {
    // Transform 12-bit ADC value (0-4095) to 16-bit range (0-65535)
    const auto raw_to_uint16 = [](uint16_t raw) {
        return ((raw << 4) & 0xFFF0) | ((raw >> 8) & 0x000F);
    };

    uint16_t max_value = getMaxValueInBuffer();
    return raw_to_uint16(max_value);
}
```

Updated `src/peripherals/Drum.cpp` (lines 289-292):
```cpp
input_state.drum.don_left.analog = m_pads.at(Id::DON_LEFT).getAnalog();
input_state.drum.ka_left.analog = m_pads.at(Id::KA_LEFT).getAnalog();
input_state.drum.don_right.analog = m_pads.at(Id::DON_RIGHT).getAnalog();
input_state.drum.ka_right.analog = m_pads.at(Id::KA_RIGHT).getAnalog();
```

Restored original scaling in `src/utils/InputReport.cpp` (line 225):
```cpp
auto map_to_axis = [](uint16_t raw) {
    return (int16_t)(raw >> 1);  // Now correct: 16-bit (0-65535) → int16_t (0-32767)
};
```

**Status**: ✅ Fixed

---

### Bug 4: Git Push Permission Errors (403)
**Problem**: Could not push directly to main branch or create new branches due to GitHub repository permissions.

**Solution**: Used existing feature branch `claude/debug-analog-drum-triggers-01C9NYqNfyXAJ2Zu4HATUPRn` and cherry-picked commits.

**Status**: ✅ Resolved

---

### Bug 5: Incomplete GUID in gamecontrollerdb.txt
**File**: External - TaikoArcadeLoader `gamecontrollerdb.txt`

**Problem**: Xbox 360 Controller GUID was only 16 characters instead of required 32 characters for SDL2 gamecontrollerdb format.

**Fix Applied**:
```
030000005e0400000000000000000000,OuchiTaiko Xbox Mode,a:b0,b:b1,x:b2,y:b3,back:b6,start:b7,leftstick:b8,rightstick:b9,leftshoulder:b4,rightshoulder:b5,dpup:h0.1,dpdown:h0.4,dpleft:h0.8,dpright:h0.2,leftx:a0,lefty:a1,rightx:a3,righty:a4,lefttrigger:a2,righttrigger:a5,platform:Windows,
```

**Status**: ✅ Fixed (GUID padded to exactly 32 characters)

---

## Complete Analog Data Flow Pipeline

### After All Fixes:

1. **ADC Read**: Raspberry Pi Pico reads 12-bit analog value (0-4095)
2. **getAnalog() Transformation**: Bit-shifting converts to 16-bit range (0-65535)
   - Formula: `((raw << 4) & 0xFFF0) | ((raw >> 8) & 0x000F)`
3. **map_to_axis() Scaling**: Right-shift by 1 to fit int16_t range (0-32767)
   - Formula: `raw >> 1`
4. **XInput Output**: Full-range analog values sent to game via left stick axes
   - X axis: ka_left (negative) vs don_left (positive)
   - Y axis: ka_right (positive) vs don_right (negative)

### Comparison:

| Stage | Before Fixes | After Fixes |
|-------|-------------|-------------|
| ADC Read | 0-4095 | 0-4095 |
| getAnalog() | ❌ Not called (field stayed 0) | ✅ 0-65535 (bit-shifted) |
| map_to_axis() | 0-4095 >> 1 = 0-2047 | 0-65535 >> 1 = 0-32767 |
| XInput Range | 6.25% utilization | 100% utilization |

---

## Testing Results

### Windows joy.cpl Testing:
- **256x multiplier**: ✅ Sticks moved all the way to the edge
- **64x multiplier**: ✅ Sticks moved all the way to the edge
- **Proper bit-shifting + >> 1**: ✅ Expected to show full range (not tested yet)

### TaikoArcadeLoader Testing:
- **Initial state**: ❌ Numbers stayed at baseline (45), hits not registering
- **With fixes**: ⚠️ Slightly better, but crosstalk issues remain
- **Crosstalk issue**: Hitting right ka triggers BOTH right ka AND right don

---

## Remaining Issues

### Issue 1: Baseline Not Centered at Zero
**Problem**: Analog axes output baseline values (~360 after bit-shifting) even at rest, instead of 0 (center). This causes XInput axes to be slightly off-center, which games may interpret as input.

**Current Behavior**:
- Rest position: ~360 (slightly positive from center 0)
- Hit position: ~5000+ (strongly positive)
- **Both exceed thresholds** → Both drums trigger

**Potential Solution** (Not Implemented):
- Subtract baseline from analog value before outputting
- Results in 0 at rest, only positive/negative when actually hit
- Would require tracking per-drum baselines

**Status**: 🔴 Unresolved

---

### Issue 2: Drum Crosstalk in Game
**Problem**: Hitting right ka triggers both right ka AND right don in arcade game, despite no crosstalk in firmware.

**Suspected Cause**: Baseline offset issue (see Issue 1) causes game to see both:
- Baseline as a small don_right hit (negative from center)
- Actual hit as a large ka_right hit (positive)

**Status**: 🔴 Unresolved

---

## Version Changes

**File**: `src/utils/Menu.cpp` (line 206)

Changed firmware version to "6.6" to verify firmware was actually flashed:
```cpp
{"Firmware v6.6\nDecember 2025", Menu::Descriptor::Action::None},
```

---

## Configuration Files

### TaikoArcadeLoader config.toml
**Location**: External configuration file

**Required Setting** (User confirmed already set):
```toml
[controller]
analog_input = true
```

### Firmware Thresholds
**File**: `include/GlobalConfiguration.h` (lines 33-39)

**Stock Values** (Restored after testing):
```cpp
.trigger_thresholds = {
    .don_left = 75,
    .ka_left = 95,
    .don_right = 75,
    .ka_right = 95,
},
```

---

## Commits and Pull Requests

### Branch: `claude/debug-analog-drum-triggers-01C9NYqNfyXAJ2Zu4HATUPRn`

**Commit History** (most recent first):
1. `ee0ce52` - Fix analog mode: Add proper bit-shifting transformation
2. `56c6654` - Diagnostic v6.6: Increase analog multiplier to 64x for testing
3. `c5a1956` - Merge branch 'main' into claude/debug-analog-drum-triggers-01C9NYqNfyXAJ2Zu4HATUPRn
4. `8c2b0b1` - Fix analog mode sensitivity issues
5. `13cc353` - ok

### Pull Request: #21
**Status**: ✅ Merged to main

**Merge Conflict Resolution**:
- File: `src/utils/InputReport.cpp` (line 225)
- Conflict: `>> 1` (correct) vs `* 64` (diagnostic)
- Resolution: Accepted current change (`>> 1`)

---

## Conclusion

### What Was Fixed:
1. ✅ Missing `.analog` field assignment
2. ✅ Missing 12-bit to 16-bit ADC bit-shifting transformation
3. ✅ Proper XInput scaling to full range
4. ✅ Version tracking (v6.6)
5. ✅ gamecontrollerdb.txt GUID format

### What Still Needs Work:
1. 🔴 Baseline offset causing non-zero resting position
2. 🔴 Drum crosstalk in game (likely due to baseline issue)
3. 🔴 Overall analog mode performance vs keyboard mode

### Recommendation:
Keyboard mode works reliably and is simpler. Analog mode would require additional work to implement baseline subtraction and proper axis centering to function correctly in games.

---

## Technical Reference

### Key Files Modified:
- `include/peripherals/Drum.h` - Added getAnalog() declaration
- `src/peripherals/Drum.cpp` - Implemented getAnalog() with bit-shifting, updated analog field assignments
- `src/utils/InputReport.cpp` - Verified correct >> 1 scaling
- `src/utils/Menu.cpp` - Version bump to 6.6

### External Files Examined:
- `TaikoArcadeLoader/config.toml` - Verified analog_input = true
- `TaikoArcadeLoader/gamecontrollerdb.txt` - Fixed Xbox 360 GUID format

### Session Info:
- **Date**: December 6, 2025
- **Firmware Version**: 6.6
- **Branch**: `claude/debug-analog-drum-triggers-01C9NYqNfyXAJ2Zu4HATUPRn`
- **Pull Request**: #21 (merged)
