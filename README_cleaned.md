<div align="center">
<img src="images/Pictures/mainlogo.png" width="100%" style="max-width: 1200px; display: block; margin: 0 auto;">
</div>

---

<div align="center">
*In Japanese, 'ouchi' (おうち) means 'home' and 'taiko' (太鼓) means 'drum.' Together, 'OuchiTaiko' represents bringing the authentic Taiko arcade experience into your own space.*
</div>

---

## **Table of Contents**
- [1: Project Overview](#1-project-overview)
  - [Key Features](#-key-features)
- [2: Parts List for Electronics](#2-parts-list-for-electronics)
- [3: Parts List for Hardware](#3-parts-list-for-hardware)
- [4: Build the Circuit](#4-build-the-circuit)
- [5: Build the Drum](#5-build-the-drum)
- [6: Control Box](#6-control-box)
- [7: Flash The Firmware](#7-flash-the-firmware)
- [8: Calibration & Settings](#8-calibration--settings)
- [9: Files & Downloads](#9-files--downloads)
- [10: Basic Troubleshooting](#10-basic-troubleshooting)
- [11: Menu Tree Structure/Quick Reference](#11-menu-system-reference)
- [12: About](#12-about)
- [13: Copyright Information](#13-copyright-information)

---

## **1: Project Overview**

#### The Finished Build

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

Welcome to the **OuchiTaiko Project** - an open-source guide for building a professional-grade arcade Taiko drum controller.

This project addresses the limited availability and high cost of commercial units through innovative software-based design. Using **Adaptive Baseline Software Intelligence (ABSI)**, it achieves arcade-level performance without complex analog circuits or custom PCBs.

With basic soldering and woodworking skills, you'll build a fully functional controller. All linked components are tested to work together, though compatible alternatives are welcome.

---

## 🔥 **Key Features**

### **World-First Innovations**

**Two-Pass Bidirectional Crosstalk Detection**
- Eliminates ghost hits from frame vibrations
- Automatic calibration in both directions
- No manual tweaking required

**Adaptive Baseline Software Intelligence (ABSI)**
- Automatic sensitivity adjustment
- Velocity-sensing for Big Note detection
- Dynamic baseline tracking
- Zero calibration drift

**Custom Arcade Sensor Suspension**
- Mimics Japanese arcade machines
- First publicly available design
- Consistent velocity response

**PS4 Always-On Mode**
- No authentication hardware needed

---

### **Complete Hardware**

- **4 Velocity-Sensitive Zones:** Enhanced false-trigger isolation
- **14 Navigation Buttons:** Full in-game navigation
- **Professional Mounting:** Adjustable angled speaker stand
- **Standalone Testing:** OLED display with real-time feedback
- **No PC Required:** Complete setup and calibration via display

---

### **14 Input Modes**

Nintendo Switch Tatacon | Nintendo Switch Pro | PS3 Dualshock3 | PS4 Tatacon | PS4 Dualshock4 | Keyboard P1/P2 | Xbox 360 | Android/iOS | Analog P1/P2 | MIDI | Debug

---

### **Zero Coding Required**

- Drag-and-drop firmware installation
- On-screen menu system
- Automatic updates
- No programming knowledge needed

---

*Built on DonCon2040 and HIDtaiko foundations*

### 🎥 Demo Videos

- [Finished Drum](https://youtu.be/Ji3sOdRHO0Q)
- [Gameplay](https://youtu.be/p4eFeo_LB5I?si=jDKb93B7uYx1qAux)
- [Roll Polling Rate](https://youtu.be/wEw9HbGcR-s)
- [Hit Velocity Detection](https://www.youtube.com/watch?v=wceSXgtBcfE)

---

## **2: Parts List for Electronics**

<div align="right"><sub><a href="#table-of-contents">↑ Back to Top</a></sub></div>

**Important:** Quantities refer to individual items needed, not packages to order.

| #  | Item | Qty | Product Link |
|----|------|-----|--------------|
| 1  | XIAO RP2040 board (Seeed Studio) | 1 | [Link](https://a.co/d/g9dFG88) |
| 2  | GODIYMODULES MCP23017 I2C expander Board | 1 | [Link](https://a.co/d/iBEbs4b) |
| 3  | 1N4148 diodes | 4 | [Link](https://a.co/d/jlNKdaJ) |
| 4  | 0.1µF/100nF Ceramic Capacitors | 4 | [Link](https://a.co/d/cG1cxOQ) |
| 5  | 3.5mm TRS FEMALE jacks w/ screw terminals | 4 | [Link](https://a.co/d/2umR9us) |
| 6  | 3.5mm TRS male Plugs w/ standard ends | 4 | [Link](https://a.co/d/414YG9z) |
| 7  | 27mm Piezo sensors | 4 | [Link](https://a.co/d/hbacbaF) |
| 8  | 128x64 Mono OLED display (I2C) | 1 | [Link](https://a.co/d/bHAtiY2) |
| 9  | 6mmx6mmx8mm Tactile Switches | 14 | [Link](https://a.co/d/aa5ppfs) |
| 10 | 22 AWG Siamese wire | 1 | [Link](https://a.co/d/0pGa2wH) |
| 11 | USB-C to USB-A Female Coupler | 1 | [Link](https://a.co/d/9WTmxTu) |
| 12 | Short USB-C to USB-C extension cable | 1 | [Link](https://a.co/d/cVbKVmQ) |
| 13 | 6 inch, coiled 3.5mm TRS audio extension cable | 4 | [Link](https://a.co/d/cSKAJ7D) |
| 14 | 3.5mm TRS barrel coupler | 4 | [Link](https://a.co/d/fOiYb5s) |
| 15 | 4-Pin, 0.2 Inch Pitch Screw Terminal Blocks | 2 | [Link](https://a.co/d/1X20DtA) |
| 16 | 22 AWG 4-Wire Ribbon Connection Wire | 1 | [Link](https://a.co/d/cXyTYy1) |
| 17 | ElectroCookie Circuit Board (88.9mm x 96.5mm) | 1 | [Link](https://a.co/d/i5jfYjs) |
| 18 | 10ft USB-A to USB-A Cable | 1 | [Link](https://a.co/d/cBqJJua) |
| 19 | M2x3x3 Threaded Inserts | 4 | [Link](https://a.co/d/8VZdEql) |
| 20 | M2x4 bolts | 4 | [Link](https://a.co/d/4cPSWVY) |
| 21 | 22 AWG Wire Ferrules and Crimper | 8 | [Link](https://a.co/d/hu5OIl3) |

---

## **3: Parts List for Hardware**

<div align="right"><sub><a href="#table-of-contents">↑ Back to Top</a></sub></div>

### **Required Tools**

1. Laser cutter or CNC machine (or alternative cutting method)
2. Soldering iron and solder
3. Wire strippers/cutters
4. Screwdrivers
5. Hot glue gun
6. Strong clamps
7. Drill
8. Router
9. Sandpaper
10. Utility knife
11. Rubber mallet

### **Hardware Parts**

| # | Item | Qty | Product Link |
|---|------|-----|--------------|
| 1 | 6mm Cabinet-Grade MDF | 1 | [Home Depot](https://www.homedepot.com/p/1-4-in-x-2-ft-x-4-ft-Medium-Density-Fiberboard-1508104/202089069) |
| 2 | Strong Wood Glue | 1 | [Link](https://a.co/d/1uKv6cR) |
| 3 | M3x8mm bolts | 8 | [Link](https://a.co/d/52Q8UtD) |
| 4 | M3x5mm threaded inserts | 8 | [Link](https://a.co/d/bKB6OpW) |
| 5 | M6x10mm wood threaded inserts | 14 | [Link](https://a.co/d/jhY5rYA) |
| 6 | M6x20mm nylon bolts | 14 | [Link](https://a.co/d/bPitQiX) |
| 7 | M6 threaded 20mmx15mm rubber isolators | 14 | [Link](https://a.co/d/9dWHezk) |
| 8 | 3D printer filament (PLA) | 1 | [Link](https://a.co/d/7cCSDtJ) |
| 9 | Gel Superglue | 1 | [Link](https://a.co/d/dxU7lfw) |
| 10 | Loctite thread adhesive - Medium | 1 | [Link](https://a.co/d/2W890aJ) |
| 11 | 2.2mm Scuba Knit Neoprene fabric (4"x4") | 1 | [Link](https://a.co/d/dfMhZ4k) |
| 12 | Finger Knobs with M6 threads | 18 | [Link](https://a.co/d/hjL3QQP) |
| 13 | Mini PA speaker stand | 1 | [Link](https://a.co/d/2YkmhPj) |
| 14 | Adjustable Angle Speaker bracket | 1 | [Link](https://a.co/d/gQioU8i) |
| 15 | Rubber Taiko Drum Cover (optional) | 1 | [Link](https://taiko.ac/products/rubber-drum-pad) |
| 16 | Spring-Loaded Phone Holder with Gooseneck | 1 | [Link](https://a.co/d/fRja2H6) |

---

## **4: Build the Circuit**

<div align="right"><sub><a href="#table-of-contents">↑ Back to Top</a></sub></div>

<div align="center">
<img src="images/Pictures/schematic.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

📸 **Note:** Early photos may show different terminal headers. Installation process is identical.

### **Circuit Overview**

Single-board design with:
- **XIAO RP2040**: Drum sensor controller
- **MCP23017**: Button and display controller
- **OLED Display**: Status and menus
- **14 Navigation Buttons**: Gamepad controls

### **Board Orientation**

This protoboard has row letters and numbers on **both sides**:
- **FRONT face** = Display and buttons (use FRONT coordinates for Steps 1-2)
- **BACK face** = XIAO, MCP23017, terminals (use BACK coordinates for Steps 3-12)

Always verify which side you're on before placing components.

---

### **Step 1: Mount OLED Display (FRONT face)**

**Location:** Row J, holes 14-17 (FRONT face)
- Screen facing up
- Solder in place

<div align="center">
<img src="images/Pictures/oledmount.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

**OLED pins** (wired in Step 7):
- VCC (Row A, hole 15)
- GND (Row A, hole 14)
- SCL (Row A, hole 16)
- SDA (Row A, hole 17)

---

### **Step 2: Mount Buttons (FRONT face)**

**Orientation:**
- Legs on left and right (NOT up/down)
- Align all button tabs the same direction

<div align="center">
<img src="images/Pictures/buttonsall.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

**Button Positions (FRONT face):**

| Button | Ground Leg | Signal Leg |
|--------|-----------|------------|
| **LEFT SIDE** | | |
| UP | Row E, hole 4 | Row E, hole 6 |
| Down | Row A, hole 4 | Row A, hole 6 |
| Left | Row C, hole 1 | Row C, hole 3 |
| Right | Row C, hole 7 | Row C, hole 9 |
| L | Row F, hole 1 | Row F, hole 3 |
| Select | Row A, hole 12 | Row A, hole 14 |
| Share | Row H, hole 7 | Row H, hole 9 |
| **RIGHT SIDE** | | |
| North | Row E, hole 27 | Row E, hole 25 |
| South | Row A, hole 27 | Row A, hole 25 |
| West | Row C, hole 24 | Row C, hole 22 |
| East | Row C, hole 30 | Row C, hole 28 |
| R | Row F, hole 30 | Row F, hole 28 |
| Start | Row A, hole 19 | Row A, hole 17 |
| Home | Row H, hole 24 | Row H, hole 22 |

**Fix Signal Shorts:**
Two button pairs share signal rows and need separation:
- NORTH/SOUTH buttons
- UP/DOWN buttons

<div align="center">
<img src="images/Pictures/drill1.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

1. Find hole centered between signal legs
2. Scrape copper with 3mm drill bit
3. Test with multimeter - no beep = success

---

### **Step 3: Mount XIAO RP2040 (BACK face)**

Flip board to BACK face for remaining steps.

<div align="center">
<img src="images/Pictures/socket.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

**Placement:**
- Right side of board
- USB-C port facing left
- Top row: Row D, holes 1-7 (BACK face)
- Bottom row: Row H, holes 1-7 (BACK face)

Connect GND to common rail and bridge all GND rails.

---

### **Step 4: Install Drum Sensor Protection (BACK face)**

Each sensor needs diode + capacitor protection.

**Locations:**

| Drum | XIAO Pin | Diode/Cap Location (BACK) |
|------|----------|---------------------------|
| Left Ka | A0 | Row J, hole 7 |
| Left Don | A1 | Row J, hole 6 |
| Right Don | A2 | Row J, hole 5 |
| Right Ka | A3 | Row J, hole 4 |

**For each sensor:**
1. Diode anode (non-striped) → specified hole
2. Diode cathode (striped) → GND rail below
3. Capacitor leg → same hole as anode
4. Capacitor other leg → same GND hole
5. Solder combined legs together

<div align="center">
<img src="images/Pictures/diodes.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

---

### **Step 5: Connect Button Grounds (BACK face)**

Connect one leg of each button to GND rail.
Daisy-chain for cleaner routing.

<div align="center">
<img src="images/Pictures/buttongrounds.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

---

### **Step 6: Prepare MCP23017 Board**

Solder single row of 10 header pins into bottom row (A2 to VCC).
Top double row stays empty.

<div align="center">
<img src="images/Pictures/expandersolder.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

---

### **Step 7: Pre-Wire I²C Lines (BACK face)**

Before mounting MCP23017, wire underneath where it will sit.

**Temporarily place** MCP centered at Row E, holes 11-21 (BACK face). Remove after noting position.

**Wire these connections underneath:**

| Connection | From (Under MCP) | To (OLED) | Purpose |
|------------|-----------------|-----------|---------|
| VCC | Row A, hole 11 | Row A, hole 15 | Power |
| GND | Row A, hole 12 | Row A, hole 14 | Ground |
| SCL | Row A, hole 13 | Row A, hole 16 | I²C Clock |
| SDA | Row A, hole 14 | Row A, hole 17 | I²C Data |

**Mount MCP23017:**
- Bottom row → Row E, holes 11-21 (BACK face)
- VCC pin in hole 11
- A2 pin in hole 21
- Solder in place

<div align="center">
<img src="images/Pictures/expanderterminalsfirst.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

---

### **Step 8: Connect Power & Ground**

Verify connections:
1. XIAO 3.3V → MCP VCC → OLED VCC
2. XIAO GND → MCP GND → OLED GND
3. All three share common ground

---

### **Step 9: Connect I²C Data Lines**

| Connection | From XIAO | To MCP |
|------------|-----------|--------|
| SDA | SDA row | SDA row |
| SCL | SCL row | SCL row |

Verify complete chain: XIAO → MCP → Display for both SDA and SCL.

<div align="center">
<img src="images/Pictures/powercomms.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

---

### **Step 10: Connect Button Signals to MCP23017 (BACK face)**

**Soldering Technique:**
1. Pre-fill MCP holes with solder
2. Tin wire ends (1-2mm max)
3. Place wire on solder, press iron down

**Button Wiring Map:**

| Button | MCP Pin | Connection |
|--------|---------|------------|
| UP | B0 | MCP B0 → UP signal row |
| Down | B1 | MCP B1 → DOWN signal row |
| Left | B2 | MCP B2 → LEFT signal row |
| Right | B3 | MCP B3 → RIGHT signal row |
| L | B4 | MCP B4 → L signal row |
| Select | B5 | MCP B5 → SELECT signal row |
| Share | B6 | MCP B6 → SHARE signal row |
| North | A0 | MCP A0 → NORTH signal row |
| South | A1 | MCP A1 → SOUTH signal row |
| West | A2 | MCP A2 → WEST signal row |
| East | A3 | MCP A3 → EAST signal row |
| R | A4 | MCP A4 → R signal row |
| Start | A5 | MCP A5 → START signal row |
| Home | A6 | MCP A6 → HOME signal row |

Wire Row A pins first, then Row B.

<div align="center">
<img src="images/Pictures/shortwire.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

---

### **Step 11: Mount Terminal Blocks (BACK face)**

**Terminal Block 1 (GND):**
- Location: Row A, holes 29, 27, 25, 23
- Wire openings facing UP
- Bridge all 4 pins together
- Connect to GND rail

**Terminal Block 2 (Signals):**
- Location: Row F, holes 30, 28, 26, 24
- Wire openings facing DOWN
- Label ports 1-4 (Ka-Left, Don-Left, Don-Right, Ka-Right)

<div align="center">
<img src="images/Pictures/terminals3.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

---

### **Step 12: Connect Drum Signal Terminals**

| Drum Pad | From Terminal | To XIAO |
|----------|--------------|---------|
| Left Ka | Row D, hole 24 | A0 |
| Left Don | Row C, hole 26 | A1 |
| Right Don | Row B, hole 28 | A2 |
| Right Ka | Row A, hole 30 | A3 |

<div align="center">
<img src="images/Pictures/drumsignalwires2.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

---

### **Circuit Complete!**

Continue to Section 5 to build the drum.

---

## **5: Build the Drum**

<div align="right"><sub><a href="#table-of-contents">↑ Back to Top</a></sub></div>

**Scale Verification:** Check SVG files are correct scale - all 14 mounting holes should be exactly 6mm diameter.

💾 [Download files here](https://ouchitaikoproject.github.io/OuchiTaikoProject/DownloadFiles/KillerQsOuchiTaikoProjectSTLandSVGFiles.zip)

---

### **5.1: Prepare the Wood**

**C1. Cut all MDF pieces per SVG templates**

Use laser cutter, CNC, or manual cutting with printed templates at 100% scale.

**C2. Sand smooth**

Remove rough edges for clean assembly.

---

### **5.2: Assemble the Drum Structure**

**D1. Assemble rear base plates**

Glue two identical rear plates together. Clamp and dry several hours.

<div align="center">
<img src="images/Pictures/baseplate.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

**D2. Assemble drum faceplates**

Four faceplates total: Left Ka, Left Don, Right Don, Right Ka.
- Glue TOP and BOTTOM plates together for each
- Clamp and dry several hours

<div align="center">
<img src="images/Pictures/kaholes.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

**D3. Rout Ka rim edges**

Router or sand the outer curved edge of Ka faces only.

**D4. Drill holes for threaded inserts**

Using 8mm drill bit:
- Locate 14 pre-cut 6mm holes
- Drill straight down to 11mm deep
- Creates 8mm wide holes for inserts

**D5. Chamfer holes**

Chamfer inner rim of each 8mm hole for flush insert seating.

**D6. Install M6 threaded inserts**

Screw 14 M6 inserts into holes until flush. Add superglue to threads.

<div align="center">
<img src="images/Pictures/kagrommetin.png" width="350px" style="display: block; margin: 0 auto;">
</div>

**D7. Install M3 threaded inserts**

Tap two M3 inserts into underside of each faceplate (8 total). Add superglue.

---

### **5.3: Install the Rubber Isolators**

**E1-E3. Prepare isolator assemblies**

Per isolator (14 total):
1. Cut head off M6 nylon bolt
2. Apply Loctite to isolator threads
3. Screw bolt into isolator until stopped

<div align="center">
<img src="images/Pictures/isolatorgrommetside.png" width="350px" style="display: block; margin: 0 auto;">
</div>

**E4-E5. Install to drum faces**

1. Add Loctite to drum face inserts
2. Screw isolator/bolt assemblies in finger-tight

<div align="center">
<img src="images/Pictures/grommetinstalled.png" width="350px" style="display: block; margin: 0 auto;">
</div>

---

### **5.4: Print Sensor Housings**

Print 4 complete sets (top + bottom each).
- **Material:** PLA
- **Settings:** 0.2mm layers, 40% Gyroid infill, no supports

---

### **5.5: Assemble Sensor Electronics**

🎥 [Video guide](https://youtu.be/tQe-xDEqEdY)

**Repeat G1-G7 four times:**

**G1. Cut neoprene discs**

Four 12mm discs using SVG template.

**G2. Glue disc to housing**

Superglue neoprene disc into bottom shell center ring.

**G3. Strip Siamese wire**

10-12" length, strip both ends.

**G4. Solder TRS jack**

- Red → TIP
- Black → SLEEVE

**G5. Solder to piezo**

- Red → center disc
- Black → outer brass ring

<div align="center">
<img src="images/Pictures/sensorsolder.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

**G6. Glue piezo to neoprene**

Superglue piezo face-up onto neoprene. Brass side down.

**G7. Assemble housing**

Add superglue to strain relief and rim. Snap shells together.

<div align="center">
<img src="images/Pictures/sensorholding.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

**G8. Mount housings to drum**

Use 2 M3x8mm screws per housing into threaded inserts.

---

### **5.6: Mounting Hardware Assembly**

**H1-H2. Mark and drill bracket holes**

Center speaker bracket on rear base, mark 4 holes, drill 6mm.

**H3-H4. Install mounting hardware**

Feed M6 bolts through base, attach speaker bracket, secure with knobs.

<div align="center">
<img src="images/Pictures/bracketconnected.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

**H5. Assemble faceplates to base**

Feed 14 M6 bolts through base holes, secure with finger knobs.

**H6. Route sensor wires**

Route wires through rear base openings.

**H7. Attach TRS barrel mounts**

3D print and attach with adhesive tape. Insert barrel couplers.

<div align="center">
<img src="images/Pictures/couplersmounted.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

**H8. Connect sensor cables**

Connect drum sensors to barrel couplers, add coiled extension cables.

---

## **6: Control Box**

<div align="right"><sub><a href="#table-of-contents">↑ Back to Top</a></sub></div>

### **6.1: 3D Print and Assemble**

**I1. Print enclosure**

Print base and lid:
- Layer: 0.20mm
- Infill: Gyroid 20%
- Supports: Automatic

**I2. Add TRS jacks & USB coupler**

Install 4 TRS jacks in back holes. Install USB coupler in bottom opening (USB-A out, USB-C in). Hot glue in place.

<div align="center">
<img src="images/Pictures/trsandusb.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

**I3. Mount circuit to lid**

1. Press M2x3x3 inserts into lid standoffs using heat
2. Align circuit board through lid holes
3. Secure with 4 M2x4mm bolts

<div align="center">
<img src="images/Pictures/buttonsfit.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

**I4. Attach hinges**

Cut 1.75mm filament sections, feed through hinge holes. Trim flush.

---

### **6.2: Connect Circuit Wiring**

**J1. Create wires**

Cut two 215mm lengths of 4-wire ribbon cable. Crimp ends with 22 AWG ferrules.

**J2. Connect ground wires**

One ribbon set:
- Circuit board: 4 wires → GND terminal block
- Control box: 4 wires → GND terminal of each TRS jack

<div align="center">
<img src="images/Pictures/trswires.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

**J3. Connect signal wires**

Other ribbon set:
- Circuit board: 4 wires → SIGNAL terminal block (pins 1-4)
- Control box: Wire 1 → TRS Jack 1 TIP, Wire 2 → Jack 2 TIP, etc.

**J4. Connect XIAO to USB coupler**

Use short USB-C to USB-C cable.

<div align="center">
<img src="images/Pictures/allwires.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

---

### **6.3: Floor Stand and Control Box Mount**

**K1. Mount drum to speaker stand**

Attach angled bracket to stand.

**K2. Mount control box**

Attach phone holder to speaker stand post. Clip control box to holder. Connect drum sensors to control box TRS jacks.

**K3. Adjust height/angle**

Set comfortable playing position.

**K4. Add drum cover**

Apply rubber cover, towel, or preferred surface material.

<div align="center">
<img src="images/Pictures/drumcover.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

---

## **7: Flash The Firmware**

<div align="right"><sub><a href="#table-of-contents">↑ Back to Top</a></sub></div>

💾 **Download firmware:**
- [OuchiTaiko Firmware v2.0](https://ouchitaikoproject.github.io/OuchiTaikoProject/DownloadFiles/KillerQsOuchiTaikoFirmware_v2.0.uf2)
- [Flash Nuke](https://ouchitaikoproject.github.io/OuchiTaikoProject/DownloadFiles/universal_flash_nuke.uf2)

**Step 1: Enter boot mode**

Hold "B" button on XIAO while connecting to PC. Board appears as removable drive.

**Step 2: Wipe board**

Drag `universal_flash_nuke.uf2` to drive. Board reboots. Drag nuke file again for clean slate.

**Step 3: Flash firmware**

Drag `KillerQsOuchiTaikoFirmware_v2.0.uf2` to drive. Board reboots automatically. If not, wait 15 seconds then unplug/replug.

Ready for calibration!

---

## **8: Calibration & Settings**

<div align="right"><sub><a href="#table-of-contents">↑ Back to Top</a></sub></div>

### **Button and Drum Test**

1. Hold SELECT for 1 second (enters menu in Switch Tatacon mode)
2. Visit [Meloncolle Taiko Controller](https://meloncolle.com/tatacon)
3. Test all buttons and drums - they should match on screen
4. Exit menu when complete

---

### **Taiko-Tune™ Auto-Calibration (Recommended)**

World's first automated drum controller calibration system.

**How it works:**
- Analyzes your playing style in real-time
- Calculates optimal thresholds automatically
- Two-pass system eliminates crosstalk
- Adapts to drum cover and playing intensity

**Quick Start:**
- Hold START for 1 second, or
- Menu: SELECT → Settings → Drum Settings → Auto Taiko-Tune → Analyze All 4 Drums

**During calibration:**
1. Wait for 3-second countdown (samples ambient noise - don't touch drum)
2. Hit indicated pad naturally until progress reaches 100%
   - Mix light and strong hits like normal gameplay
   - Don't baby it or overdo it
3. System auto-applies optimal threshold
4. Moves to next drum after 3-second results display

**Two passes:** Forward then reverse order for optimal crosstalk compensation
**Total time:** 5-8 minutes for all 4 drums

**Cancel anytime:** Press B to restore original thresholds

---

### **Manual Single Pad Adjustment**

For precise manual control or fine-tuning after Taiko-Tune.

**Enter adjustment:**
1. Hold SELECT → Settings → Drum Settings → Drum Thresholds → Single Pad Adjust
2. Choose drum
3. UP/DOWN to adjust value (0-4095)
4. A to save, B to cancel

**Troubleshooting:**
- Light hits not registering → LOWER threshold
- False hits too easily → RAISE threshold
- Crosstalk (hitting one triggers another) → RAISE triggered drum by 10-20 points

**Typical ranges:**
- Don (center): 40-80
- Ka (rim): 60-100

---

### **Extended Drum Settings**

**Big Hit Arcade Mode**
- Enables harder hits for Big Notes (like arcade)
- Start around 850 (typically 2-3x normal threshold)
- Increase value = easier Big Hits
- Decrease value = harder hits required

**SimulTap Mode**
- ON: Allows simultaneous hits for non-Taiko rhythm games
- OFF: Prevents simultaneous hits (default for Taiko)

**Hold Time**
- How long hit is "held" before accepting another
- Recommended: 25-35ms
- Increase if double hits register too fast
- Decrease if hits feel laggy

---

### **OLED Display Features**

While playing:
- Streak counter (resets after 1 second idle)
- Character animations confirm each hit
- Characters represent drum positions (left to right)

---

## **9: Files & Downloads**

<div align="right"><sub><a href="#table-of-contents">↑ Back to Top</a></sub></div>

### **🔧 Firmware Files**

**[⬇️ OuchiTaiko Firmware v2.0](https://ouchitaikoproject.github.io/OuchiTaikoProject/DownloadFiles/KillerQsOuchiTaikoFirmware_v2.0.uf2)**
Main controller firmware

**[⬇️ Flash Nuke](https://ouchitaikoproject.github.io/OuchiTaikoProject/DownloadFiles/universal_flash_nuke.uf2)**
Wipes board before firmware install

### **📐 3D Printing & Laser Cutting**

**[⬇️ All STL & SVG Files (ZIP)](https://ouchitaikoproject.github.io/OuchiTaikoProject/DownloadFiles/KillerQsOuchiTaikoProjectSTLandSVGFiles.zip)**

**Individual files:**
- [Bachi Stick Holder (STL)](https://ouchitaikoproject.github.io/OuchiTaikoProject/DownloadFiles/OuchiTaiko%20Project%20-%20Bachi%20Stick%20Holder.stl)
- [Sensor Housing (STL)](https://ouchitaikoproject.github.io/OuchiTaikoProject/DownloadFiles/OuchiTaiko%20Project%20-%20Housing.stl) - Print 4x
- [TRS Barrel Mount (STL)](https://ouchitaikoproject.github.io/OuchiTaikoProject/DownloadFiles/OuchiTaiko%20Project%20-%20TRS%20Barrel%20Connector%20Mount.stl)
- [Control Box Base (STL)](https://ouchitaikoproject.github.io/OuchiTaikoProject/DownloadFiles/OuchiTaiko%20Project%20Control%20Box%20Base.stl)
- [Control Box Lid (STL)](https://ouchitaikoproject.github.io/OuchiTaikoProject/DownloadFiles/OuchiTaiko%20Project%20Control%20Box%20Lid.stl)
- [All Laser SVG Files](https://ouchitaikoproject.github.io/OuchiTaikoProject/DownloadFiles/OuchiTaiko%20Project%20all%20Laser%20SVG%20Files.svg)
- [LightBurn Project (.lbrn2)](https://ouchitaikoproject.github.io/OuchiTaikoProject/DownloadFiles/OuchiTaiko%20Project%20SVG%20LightBurn%20Project%20File.lbrn2)

**Print settings:**
- PLA filament
- 0.2mm layers
- 40% Gyroid (sensor housings) or 20% (control box)

### **🔌 Circuit Schematic**

**[⬇️ Circuit Schematic (JPG)](https://ouchitaikoproject.github.io/OuchiTaikoProject/images/Pictures/schematic.jpg)**

---

## **10: Basic Troubleshooting**

<div align="right"><sub><a href="#table-of-contents">↑ Back to Top</a></sub></div>

**Display Issues**
- Blank OLED: Check I2C (GPIO 6/7), verify 3.3V power
- Frozen: Unplug and reconnect USB
- Menu won't open: Hold SELECT for 1 second

**Sensor Issues**
- No response: Check diode polarity, TRS wiring
- False triggers: Increase thresholds
- Missed hits: Decrease thresholds, check mounting
- Crosstalk: Increase threshold on affected pad

**Mode Switching Issues**
- Freezes: Unplug/reconnect USB-C
- No change: Press EAST to confirm

---

## **11: Menu System Reference**

<div align="right"><sub><a href="#table-of-contents">↑ Back to Top</a></sub></div>

### **Quick Access**
- Hold SELECT (1 sec): System menu
- Hold START (1 sec): Launch Taiko-Tune

### **Navigation**
- LEFT/RIGHT: Navigate/toggle/cycle
- UP/DOWN: Adjust values (hold for fast repeat)
- EAST (A): Confirm/save
- SOUTH (B): Cancel/back

### **Menu Structure**

```
SYSTEM MENU
│
├── Controller Modes
│   ├── Nintendo Switch Tatacon / Pro
│   ├── Sony PS3/PS4 Tatacon / Dualshock
│   ├── Keyboard P1/P2
│   ├── Xbox 360
│   ├── Android/iOS
│   ├── Analog P1/P2
│   ├── MIDI
│   └── Debug
│
├── Drum Settings
│   ├── Drum Thresholds
│   │   ├── Auto Taiko-Tune
│   │   ├── Manual Pad Adjust
│   │   └── Reset Thresholds
│   ├── Big Hit Mode (Off/Light/Medium/Heavy/Custom)
│   ├── SimulTap Mode (ON/OFF)
│   └── Hold Time (0-255 ms)
│
├── LED Settings
│   ├── Brightness (0-255)
│   └── Player Color (PS4)
│
├── About
├── Reset
└── USB Flash Mode
```

---

## **12: About**

<div align="right"><sub><a href="#table-of-contents">↑ Back to Top</a></sub></div>

This project builds on the excellent work of:
- **DonCon2040** by ravinrabbid (firmware foundation)
- **HIDtaiko** by kasasiki3 (circuit inspiration)
- **Pico-universal-flash-nuke** by Gadgetoid
- **Dork Design** (customizable box system)

Special thanks to Discord user 'Allspice' for testing and shaping this guide.

**Helpful Communities:**
- [Taiko no Tatsujin Modding! Discord](https://discord.com/invite/HFm37aA5zr)
- [Cons&Stuff :) Discord](https://discord.com/invite/P4CpVHrR?utm_source=Discord%20Widget&utm_medium=Connect)
- [OpenStick Community Discord](https://discord.com/invite/openstickcommunity-1049366310389289001)

### **Closing Thoughts**

Thank you for following this guide. Share your build with the community and tag me (KillerQ97)!

For questions, use GitHub Discussions or the Discord channels above.

www.ouchitaiko.com

---

## **13: Copyright Information**

<div align="right"><sub><a href="#table-of-contents">↑ Back to Top</a></sub></div>

### **Attribution Chain**

- **Original DonCon2040 firmware:** © ravinrabbid (MIT License)
- **HIDtaiko components:** © kasasiki3 (Apache License 2.0)
- **Custom modifications:** © 2025 KillerQ (Dual-licensed: MIT and Apache 2.0)

### **License Compliance**

All distributions must include:
1. Copy of MIT License from DonCon2040
2. Copy of Apache License 2.0 from HIDtaiko
3. Copyright notices from both projects
4. Attribution to all contributors

---

## **Complete License Documentation**

### **1. KillerQ's Original Contributions**

Dual-licensed under MIT License and Apache License 2.0.

**Copyright (c) 2025 KillerQ**

---

### **2. DonCon2040 Firmware**

**Copyright (c) 2021 ravinrabbid**  
**License:** MIT

```
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

---

### **3. HIDtaiko Components**

**Copyright 2022 kasasiki3**  
**License:** Apache License, Version 2.0

```
Apache License
Version 2.0, January 2004
http://www.apache.org/licenses/

[Full Apache 2.0 license text as provided in original README]
```

---

*OuchiTaiko Project | www.ouchitaiko.com*
