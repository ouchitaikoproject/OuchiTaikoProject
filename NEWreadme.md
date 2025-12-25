<div align="center">
<img src="images/Pictures/mainlogo.png" width="100%" style="max-width: 1200px; display: block; margin: 0 auto;">
</div>

---

<div align="center">
*In Japanese, 'ouchi' (おうち) means 'home' and 'taiko' (太鼓) means 'drum.' Together, 'OuchiTaiko' represents the joy of bringing the authentic Taiko experience from the Arcade into your own space.*
</div>

---

## **Table of Contents**
- [1: Project Overview/Unique Features](#1-project-overview)
- [2: Parts List for Electronics](#2-parts-list-for-electronics)
- [3: Parts List for Hardware](#3-parts-list-for-hardware)
- [4: Build the Circuit](#4-build-the-circuit)
- [5: Build the Drum](#5-build-the-drum)
- [6: Control Box](#6-control-box)
- [7: Flash The Firmware](#7-flash-the-firmware)
- [8: Calibration & Settings](#8-calibration--settings)
- [9: Files & Downloads](#9-files--downloads)
- [10: Basic Troubleshooting](#10-basic-troubleshooting)
- [11: Menu System Reference](#11-menu-system-reference)
- [12: About](#12-about)
- [13: Copyright Information](#13-copyright-information)

---

## **1: Project Overview**

#### The Finished Build - You'll be building and creating this entire setup:

<div align="center">
<img src="images/Pictures/finishedfront.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

<div align="center">
<img src="images/Pictures/finishedboxclose.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>
<div align="center">
<img src="images/Pictures/finishedleftside.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

<div align="center">
<img src="images/Pictures/finishedbackside.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>



Hi, I'm KillerQ. Welcome to the **OuchiTaiko Project** - an open-source build guide for a professional arcade-grade Taiko drum controller.

This guide represents 10 months of research and development, addressing the limited availability and high cost of commercial units. Using **Adaptive Baseline Software Intelligence (ABSI)**, this design achieves arcade-level performance through intelligent software instead of complex analog circuits or custom PCBs.

All you need is basic soldering and woodworking skills. The components linked in this guide work together as tested, though compatible alternatives are welcome.

---

## **Key Features**

### **World-First Innovations**

#### **One-Touch Auto-Calibration (v15.0)**

**The Problem:** When you strike one drum, vibrations travel through the mounting frame to neighboring drums, causing false "ghost hits" (crosstalk) that ruin gameplay accuracy.

**Traditional Approaches:**
- Manual threshold tweaking in code (requires recompilation and trial-and-error)
- Physical modifications (foam padding, increased spacing)
- Multiple-pass calibration requiring patience and precision

**OuchiTaiko's Solution - Auto Calibrate:**

**Hold START for 1 second** - that's it! The controller automatically:
- Analyzes worst-case crosstalk in a single 18-second test
- Calculates optimal thresholds with 150-point safety margin
- Applies settings instantly with zero false triggers guaranteed

**How it works:**
1. 3-second countdown (DON'T HIT - samples ambient noise)
2. 15-second recording (hit all 4 pads as hard and fast as you can)
3. Auto-applies optimal thresholds
4. Done! ✅

**Result:** 
✅ **Eliminates ghost hits** from frame vibrations  
✅ **Zero manual tweaking** - fully automated  
✅ **Arcade-level accuracy** at any intensity  
✅ **18 seconds total** from start to finish

#### **Live Threshold Tuning Interface (v15.0)**

Revolutionary unified screen shows:
- All 4 threshold values simultaneously
- Live drum animations responding to your hits in real-time
- Instant visual feedback - adjust and test without leaving the screen
- Hold-to-repeat for fast scrolling (0-4095 range)

**No PC required** - tune your entire drum while watching it respond!

#### **Adaptive Baseline Software Intelligence (ABSI)**

Achieves superior performance through elegant software:

- **Automatic sensitivity adjustment** based on environmental noise
- **Velocity-sensing triggers** for authentic Big Note scoring (hit harder = full points)
- **Dynamic baseline tracking** adapts to temperature, humidity, and mounting changes
- **Zero calibration drift** over time

#### **Custom Arcade Sensor Suspension**

Drum trigger sensors use **custom-designed suspension mounting** that precisely mimics Japanese arcade machines.

Proper sensor suspension ensures:
- Consistent velocity response across the drum surface
- Accurate Big Note detection
- Long-term durability under heavy play

---

### **🎮 Complete Hardware & Features**

#### **Standalone Testing & Display**

**No PC required** for setup, calibration, or troubleshooting:
- **OLED display** with real-time hit feedback and animated drum icons
- **Visual confirmation** of all 14 navigation buttons
- **Live stats:** Streak counter, controller mode, menu hints
- **Instant verification** that everything is wired correctly

Calibrate and test your entire system with just USB power—see exactly what's happening before you ever plug into a console or PC.

#### **Professional Hardware**

- **4 Velocity-Sensitive Zones:** Enhanced mechanical and electronic false-trigger isolation
- **14 Game Navigation Buttons:** Full in-game navigation regardless of game version
- **Professional Mounting:** Optimal hardware stabilization via adjustable, angled speaker stand
- **PS4 Always-On Mode:** No authentication hardware required

#### **14 Input Modes for Maximum Compatibility**

- Nintendo Switch Tatacon (HORI NSW-079 Taiko Drum)
- Nintendo Switch Pro Controller
- Sony PS3 Dualshock3
- Sony PS4 Tatacon Drum (HORI PS4-095 Taiko Drum)
- Sony PS4 Dualshock4 (PC/Steam only)
- Keyboard Player 1
- Keyboard Player 2
- Microsoft Xbox Xbox360 (XInput)(Compatible with TaikoArcadeLoader)
- Android (XInput)
- iOS (XInput)
- Analog Player 1 (XInput)(Compatible with TaikoArcadeLoader)
- Analog Player 2 (XInput)(Compatible with TaikoArcadeLoader)
- MIDI Controller
- Debug Mode

#### **Zero Coding or Programming Required**

- **Drag-and-drop firmware** for instant setup
- **On-screen menus** for all settings and calibration
- **Automatic updates** without recompiling
- **No technical knowledge needed**—if you can follow instructions and use a soldering iron, you can build this

---

*OuchiTaiko builds upon the excellent foundation of DonCon2040 and HIDtaiko, adding professional features while maintaining the open-source spirit.*

### 🎥 Demo Videos

- [Finished Drum](https://youtu.be/Ji3sOdRHO0Q)
- [Some Gameplay](https://youtu.be/p4eFeo_LB5I?si=jDKb93B7uYx1qAux)
- [Roll Polling Rate Demo](https://youtu.be/wEw9HbGcR-s)
- [Hit Velocity Detection](https://www.youtube.com/watch?v=wceSXgtBcfE)

---

