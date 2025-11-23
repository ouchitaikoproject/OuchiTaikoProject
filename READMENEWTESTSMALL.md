<div align="center">
<img src="images/Pictures/mainlogo.png" width="100%" style="max-width: 1200px; display: block; margin: 0 auto;">
</div>

---

<div align="center">
*'Ouchi' (おうち) = home, 'Taiko' (太鼓) = drum. Bringing authentic arcade experience home.*
</div>

---

## **Table of Contents**
- [1: Overview](#1-overview)
- [2: Electronics Parts](#2-electronics-parts)
- [3: Hardware Parts](#3-hardware-parts)
- [4: Build Circuit](#4-build-circuit)
- [5: Build Drum](#5-build-drum)
- [6: Control Box](#6-control-box)
- [7: Flash Firmware](#7-flash-firmware)
- [8: Calibration](#8-calibration)
- [9: Downloads](#9-downloads)
- [10: Troubleshooting](#10-troubleshooting)
- [11: Menu Reference](#11-menu-reference)
- [12: About](#12-about)
- [13: Copyright](#13-copyright)

---

## **1: Overview**

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

**KillerQ's OuchiTaiko Project** - Professional arcade-scale Taiko drum controller build guide.

**Key Features:**
- **4 Velocity-Sensitive Zones** with mechanical/electronic false-trigger isolation
- **Custom Arcade Sensors** with suspension mounting mimicking Japanese arcade machines
- **Adaptive Baseline Software Intelligence (ABSI)** - Auto sensitivity adjustment, velocity sensing for authentic Big Note scoring
- **Taiko-Tune™ Auto-Calibration** - Revolutionary automatic drum calibration
- **Zero Coding Required** - Drag-and-drop firmware
- **OLED Display** - Real-time mode selection, settings, calibration, hit feedback
- **Complete Standalone Testing** - No PC required, visual feedback for all 4 pads + 14 buttons
- **14 Navigation Buttons** - Full in-game navigation
- **14 Input Modes** - Switch Tatacon, Switch Pro, PS3/PS4 Tatacon, PS4 DS4, Keyboard P1/P2, Xbox360, Android, iOS, Analog P1/P2, MIDI, Debug

🎥 **Demo Videos:** [Finished Drum](https://youtu.be/Ji3sOdRHO0Q) | [Gameplay](https://youtu.be/p4eFeo_LB5I) | [Roll Polling](https://youtu.be/wEw9HbGcR-s) | [Velocity Detection](https://www.youtube.com/watch?v=wceSXgtBcfE)

---

## **2: Electronics Parts**

<div align="right"><sub><a href="#table-of-contents">↑ Back to Top</a></sub></div>

Compatible parts list. "Quantity" = individual items needed (not packages).

| #  | Item | Qty | Link |
|----|------|-----|------|
| 1  | XIAO RP2040 (Seeed Studio) | 1 | [Link](https://a.co/d/g9dFG88) |
| 2  | MCP23017 I2C Expander | 1 | [Link](https://a.co/d/iBEbs4b) |
| 3  | 1N4148 Diodes | 4 | [Link](https://a.co/d/jlNKdaJ) |
| 4  | 0.47µF/470nF Ceramic Caps | 4 | [Link](https://a.co/d/8XWar8W) |
| 5  | 3.5mm TRS Female Jacks (screw terminals) | 4 | [Link](https://a.co/d/2umR9us) |
| 6  | 3.5mm TRS Male Plugs | 4 | [Link](https://a.co/d/414YG9z) |
| 7  | 27mm Piezo Sensors | 4 | [Link](https://a.co/d/hbacbaF) |
| 8  | 128x64 OLED Display (I2C) | 1 | [Link](https://a.co/d/bHAtiY2) |
| 9  | 6x6x8mm Tactile Switches | 14 | [Link](https://a.co/d/aa5ppfs) |
| 10 | 22 AWG Siamese Wire | 1 | [Link](https://a.co/d/0pGa2wH) |
| 11 | USB-C to USB-A Female Coupler | 1 | [Link](https://a.co/d/9WTmxTu) |
| 12 | Short USB-C Extension Cable | 1 | [Link](https://a.co/d/cVbKVmQ) |
| 13 | 6" Coiled 3.5mm TRS Extension (M-M) | 4 | [Link](https://a.co/d/cSKAJ7D) |
| 14 | 3.5mm TRS Barrel Coupler | 4 | [Link](https://a.co/d/fOiYb5s) |
| 15 | 4-Pin 0.2" Screw Terminal Blocks | 2 | [Link](https://a.co/d/1X20DtA) |
| 16 | 22 AWG 4-Wire Ribbon Cable | 1 | [Link](https://a.co/d/cXyTYy1) |
| 17 | ElectroCookie Board (88.9x96.5mm) | 1 | [Link](https://a.co/d/i5jfYjs) |
| 18 | 10ft USB-A to USB-A Cable | 1 | [Link](https://a.co/d/cBqJJua) |
| 19 | M2x3x3 Threaded Inserts | 4 | [Link](https://a.co/d/8VZdEql) |
| 20 | M2x4 Bolts | 4 | [Link](https://a.co/d/4cPSWVY) |
| 21 | 22 AWG Wire Ferrules + Crimper | 8 | [Link](https://a.co/d/hu5OIl3) |

---

## **3: Hardware Parts**

<div align="right"><sub><a href="#table-of-contents">↑ Back to Top</a></sub></div>

**Required Tools:** Laser/CNC, soldering iron, wire strippers, screwdrivers, hot glue gun, clamps, drill, router, sandpaper, utility knife, rubber mallet

| # | Item | Qty | Link |
|---|------|-----|------|
| 1 | 6mm Cabinet MDF (per SVG sizes) | 1 | [Home Depot](https://www.homedepot.com/p/1-4-in-x-2-ft-x-4-ft-Medium-Density-Fiberboard-1508104/202089069) |
| 2 | Wood Glue | 1 | [Link](https://a.co/d/1uKv6cR) |
| 3 | M3x8mm Bolts | 8 | [Link](https://a.co/d/52Q8UtD) |
| 4 | M3x5mm Threaded Inserts | 8 | [Link](https://a.co/d/bKB6OpW) |
| 5 | M6x10mm Wood Threaded Inserts | 14 | [Link](https://a.co/d/jhY5rYA) |
| 6 | M6x20mm Nylon Bolts | 14 | [Link](https://a.co/d/bPitQiX) |
| 7 | M6 Threaded 20x15mm Rubber Isolators | 14 | [Link](https://a.co/d/9dWHezk) |
| 8 | PLA Filament | 1 | [Link](https://a.co/d/7cCSDtJ) |
| 9 | Gel Superglue | 1 | [Link](https://a.co/d/dxU7lfw) |
| 10 | Loctite Thread Adhesive (Medium) | 1 | [Link](https://a.co/d/2W890aJ) |
| 11 | 2.2mm Scuba Neoprene (4"x4" total) | 1 | [Link](https://a.co/d/dfMhZ4k) |
| 12 | M6 Pass-Through Finger Knobs | 18 | [Link](https://a.co/d/hjL3QQP) |
| 13 | Mini PA Speaker Stand | 1 | [Link](https://a.co/d/2YkmhPj) |
| 14 | Adjustable Angle Speaker Bracket | 1 | [Link](https://a.co/d/gQioU8i) |
| 15 | Rubber Drum Cover (optional) | 1 | [Link](https://taiko.ac/products/rubber-drum-pad) |
| 16 | Spring-Loaded Phone Holder w/ Gooseneck | 1 | [Link](https://a.co/d/fRja2H6) |

---

## **4: Build Circuit**

<div align="right"><sub><a href="#table-of-contents">↑ Back to Top</a></sub></div>

<div align="center">
<img src="images/Pictures/schematic.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

📸 **Note:** Early photos show different terminals than parts list. Installation process identical.

**Single Board Layout:**
- **XIAO RP2040** - Drum sensor controller
- **MCP23017** - Button/display I2C expander
- **OLED Display** - Status/menu
- **14 Buttons** - Navigation

**ElectroCookie Board:** Shared rows = electrically connected holes. Use exact holes when specified, any hole in shared row when flexible.

**CRITICAL - Board Orientation:**
- **FRONT** = Display/buttons (use FRONT coordinates for Steps 1-2)
- **BACK** = XIAO/MCP/terminals (use BACK coordinates for Steps 3-12)

---

### **Step 1: Mount OLED (FRONT)**

<div align="center">
<img src="images/Pictures/oledmount.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

- **Location:** Row J, holes 14-17 (FRONT)
- **Orientation:** Screen up
- Solder in place

**Pin locations:**
- VCC: Row A, hole 15
- GND: Row A, hole 14
- SCL: Row A, hole 16
- SDA: Row A, hole 17

<div align="center">
<img src="images/Pictures/oledback.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

---

### **Step 2: Mount Buttons (FRONT)**

<div align="center">
<img src="images/Pictures/buttonsall.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

**Orient buttons:** Legs left/right (not up/down). Align tabs consistently.

**CRITICAL - Exact Positions (FRONT):**

| Button | Ground | Signal |
|--------|--------|--------|
| **LEFT SIDE** |||
| UP | Row E, hole 4 | Row E, hole 6 |
| Down | Row A, hole 4 | Row A, hole 6 |
| Left | Row C, hole 1 | Row C, hole 3 |
| Right | Row C, hole 7 | Row C, hole 9 |
| L | Row F, hole 1 | Row F, hole 3 |
| Select | Row A, hole 12 | Row A, hole 14 |
| Share | Row H, hole 7 | Row H, hole 9 |
| **RIGHT SIDE** |||
| North | Row E, hole 27 | Row E, hole 25 |
| South | Row A, hole 27 | Row A, hole 25 |
| West | Row C, hole 24 | Row C, hole 22 |
| East | Row C, hole 30 | Row C, hole 28 |
| R | Row F, hole 30 | Row F, hole 28 |
| Start | Row A, hole 19 | Row A, hole 17 |
| Home | Row H, hole 24 | Row H, hole 22 |

Insert, bend legs, solder.

<div align="center">
<img src="images/Pictures/buttonback.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

**CRITICAL - Prevent Signal Shorts:**

Shared rows short NORTH/SOUTH and UP/DOWN buttons.

<div align="center">
<img src="images/Pictures/drill1.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>
<div align="center">
<img src="images/Pictures/drill2.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

**Fix (30 sec/pair):**
1. Find hole centered between signal legs
2. Scrape copper with 3mm drill/knife
3. Test with multimeter - NO beep = success
4. Repeat for second pair

---

### **Step 3: Mount XIAO (BACK)**

**Flip to BACK. All remaining steps use BACK coordinates.**

<div align="center">
<img src="images/Pictures/socket.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>
<div align="center">
<img src="images/Pictures/socketandboard.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

- Solder XIAO right side using headers/IC socket
- **USB-C port faces left**
- Top row: Row D, holes 1-7 (BACK)
- Bottom row: Row H, holes 1-7 (BACK)
- Connect XIAO GND to common GND rail
- Jump all GND rails together

---

### **Step 4: Drum Protection Circuits (BACK)**

4 identical circuits (diode + capacitor per sensor).

**Locations:**

| Drum | XIAO Pin | Diode Anode + Cap Location (BACK) |
|------|----------|-----------------------------------|
| Left Ka | A0 | Row J, hole 7 |
| Left Don | A1 | Row J, hole 6 |
| Right Don | A2 | Row J, hole 5 |
| Right Ka | A3 | Row J, hole 4 |

**Per Sensor:**

<div align="center">
<img src="images/Pictures/diodes.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>
<div align="center">
<img src="images/Pictures/protectionbelow.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

1. Place 1N4148 anode (non-stripe) in specified hole
2. Place cathode (stripe) in GND rail below
3. Place one capacitor leg in same hole as anode
4. Place other leg in same GND hole as cathode

<div align="center">
<img src="images/Pictures/doubleprotection.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>
<div align="center">
<img src="images/Pictures/protectionbelow2.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

5. Solder combined legs together

<div align="center">
<img src="images/Pictures/protectionbeneath.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

---

### **Step 5: Button Grounds (BACK)**

<div align="center">
<img src="images/Pictures/buttongrounds.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

- Connect one leg per button to GND rail
- Daisy-chain for cleaner routing

---

### **Step 6: Prepare MCP23017**

<div align="center">
<img src="images/Pictures/expandersolder.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

- Solder 10-pin header row into bottom holes (A2 left → VCC right)
- Top double row stays empty

---

### **Step 7: Pre-Wire I²C Under MCP (BACK)**

Temporarily place MCP Row E, holes 11-21 (BACK), note position, remove.

<div align="center">
<img src="images/Pictures/expanderterminals.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

**Wire underneath:**

| Connection | From (Under MCP) | To (OLED) |
|------------|------------------|-----------|
| VCC | Row A, hole 11 | Row A, hole 15 |
| GND | Row A, hole 12 | Row A, hole 14 |
| SCL | Row A, hole 13 | Row A, hole 16 |
| SDA | Row A, hole 14 | Row A, hole 17 |

<div align="center">
<img src="images/Pictures/sdasclunderexpander.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

**Mount MCP:**
- Row E, holes 11-21 (BACK)
- VCC at hole 11, A2 at hole 21
- Solder

<div align="center">
<img src="images/Pictures/expanderterminalsfirst.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

---

### **Step 8: Power & Ground**

Connect/verify:
1. XIAO 3.3V → MCP VCC → OLED VCC
2. XIAO GND → MCP GND → OLED GND
3. All three share common ground

---

### **Step 9: I²C Data**

<div align="center">
<img src="images/Pictures/powercomms.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

| Connection | From XIAO | To MCP |
|------------|-----------|--------|
| SDA | Any SDA row hole | Any SDA row hole |
| SCL | Any SCL row hole | Any SCL row hole |

Verify: XIAO SDA → MCP SDA → Display SDA (all connected)

---

### **Step 10: Button Signals to MCP (BACK)**

<div align="center">
<img src="images/Pictures/shortwire.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>
<div align="center">
<img src="images/Pictures/wirestand.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>
<div align="center">
<img src="images/Pictures/wiresharedrow.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>
<div align="center">
<img src="images/Pictures/firstwire.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>
<div align="center">
<img src="images/Pictures/secondwire.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

**Soldering:** Fill MCP holes with solder, tin wire 1-2mm, press together.

| Button | MCP Pin | Connection |
|--------|---------|------------|
| **LEFT** |||
| UP | B0 | → UP signal row |
| Down | B1 | → DOWN signal row |
| Left | B2 | → LEFT signal row |
| Right | B3 | → RIGHT signal row |
| L | B4 | → L signal row |
| Select | B5 | → SELECT signal row |
| Share | B6 | → SHARE signal row |
| **RIGHT** |||
| North | A0 | → NORTH signal row |
| South | A1 | → SOUTH signal row |
| West | A2 | → WEST signal row |
| East | A3 | → EAST signal row |
| R | A4 | → R signal row |
| Start | A5 | → START signal row |
| Home | A6 | → HOME signal row |

---

### **Step 11: Mount Terminals (BACK)**

<div align="center">
<img src="images/Pictures/terminals3.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

**Terminal Block 1 (GND):**
- Row A, holes 29, 27, 25, 23 (BACK)
- Wire openings UP
- Bridge all 4 pins, connect to GND rail

**Terminal Block 2 (Drum Signals):**
- Row F, holes 30, 28, 26, 24 (BACK)
- Wire openings DOWN
- Label: Pad 1/LK (hole 24), Pad 2/LD (26), Pad 3/RD (28), Pad 4/RK (30)

---

### **Step 12: Drum Signal Wiring (BACK)**

<div align="center">
<img src="images/Pictures/onesignalwire.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>
<div align="center">
<img src="images/Pictures/drumsignalwires2.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

| Pad | Terminal Block | To XIAO |
|-----|----------------|---------|
| Left Ka | Row D, hole 24 row | A0 |
| Left Don | Row C, hole 26 row | A1 |
| Right Don | Row B, hole 28 row | A2 |
| Right Ka | Row A, hole 30 row | A3 |

**Circuit Complete!** ✅ OLED ✅ 14 Buttons ✅ XIAO ✅ MCP ✅ Terminals

---

## **5: Build Drum**

<div align="right"><sub><a href="#table-of-contents">↑ Back to Top</a></sub></div>

**CRITICAL - Scale:** SVG files correct scale - DO NOT resize. Verify: 14 mounting holes = exactly 6mm diameter.

💾 [SVG/STL Files](https://ouchitaikoproject.github.io/OuchiTaikoProject/DownloadFiles/KillerQsOuchiTaikoProjectSTLandSVGFiles.zip)

**No laser/CNC?** Print SVGs full-size (100% / "Actual Size") as template, cut by hand.

---

### **5.1: Prepare Wood**

**C1. Cut MDF per SVG templates**
**C2. Sand smooth**

---

### **5.2: Assemble Drum**

**D1. Glue rear base plates**

<div align="center">
<img src="images/Pictures/baseplate.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

Glue two identical rear plates together. Clamp, dry several hours.

---

**D2. Glue drum faceplates**

4 faceplates: Left Ka, Left Don, Right Don, Right Ka. Each = smooth top + bottom w/ holes.

**Ka Plates:**
- Glue Left Ka TOP + Left Ka BOTTOM together
- Clamp, dry
- Repeat for Right Ka

**Don Plates:**
- Glue Left Don TOP + Left Don BOTTOM together
- Clamp, dry
- Repeat for Right Don

💡 Ignore 4 smaller holes in photo - your version has 2 holes only.

<div align="center">
<img src="images/Pictures/kaholes.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

---

**D3. Rout Ka rim edges**

<div align="center">
<img src="images/Pictures/kafront.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

Route/sand top outer curved edge of Ka rims. Prevents stick damage. Don't touch inside edges.

---

**D4. Drill for threaded inserts**

<div align="center">
<img src="images/Pictures/kahole.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

Using 8mm bit, drill 14 pre-cut 6mm holes to 8mm wide x 11mm deep. Mark bit at 11mm with tape.

---

**D5. Chamfer holes**

<div align="center">
<img src="images/Pictures/kagrommetout.png" width="350px" style="display: block; margin: 0 auto;">
</div>

Chamfer inner rim of 8mm holes for flush insert seating. Use screwdriver/knife.

---

**D6. Install M6 threaded inserts**

<div align="center">
<img src="images/Pictures/kagrommettool.png" width="350px" style="display: block; margin: 0 auto;">
</div>
<div align="center">
<img src="images/Pictures/kagrommetin.png" width="350px" style="display: block; margin: 0 auto;">
</div>

Screw 14 M6 inserts until flush. Add superglue to outside of threads.

---

**D7. Install M3 inserts**

<div align="center">
<img src="images/Pictures/threadedinserts.png" width="350px" style="display: block; margin: 0 auto;">
</div>

Tap two M3 inserts into underside of each faceplate (8 total). Flush, superglue threads. For sensor housing mounting.

---

### **5.3: Rubber Isolators**

**E1. Cut nylon bolt head**

<div align="center">
<img src="images/Pictures/nyloncut.png" width="350px" style="display: block; margin: 0 auto;">
</div>

Cut head off M6x20mm nylon bolt.

---

**E2. Loctite isolator**

Add drop of Loctite to isolator threads.

---

**E3. Install bolt in isolator**

<div align="center">
<img src="images/Pictures/isolatorgrommetside.png" width="350px" style="display: block; margin: 0 auto;">
</div>
<div align="center">
<img src="images/Pictures/grommetmeasure.png" width="350px" style="display: block; margin: 0 auto;">
</div>

Screw headless bolt into isolator until stops.

---

**E4. Loctite drum inserts**

Add Loctite to M6 inserts on drum plate underside.

---

**E5. Install assemblies**

<div align="center">
<img src="images/Pictures/grommetinstalled.png" width="350px" style="display: block; margin: 0 auto;">
</div>

Screw isolator/bolt into drum inserts, finger-tighten flush. **Repeat E1-E5 for 13 remaining holes.**

---

### **5.4: Print Sensor Housings**

**F1. Print housings**

💾 [Files](https://ouchitaikoproject.github.io/OuchiTaikoProject/DownloadFiles/KillerQsOuchiTaikoProjectSTLandSVGFiles.zip)

Print 4 sets (top + bottom each). PLA, 0.2mm layer, 40% Gyroid infill, no supports.

---

### **5.5: Assemble Sensors**

🎥 [Video](https://youtu.be/tQe-xDEqEdY)

💡 Repeat G1-G7 four times for 4 sensor dongles.

**G1. Cut neoprene discs**

Cut four 12mm neoprene discs per SVG template.

---

**G2. Glue disc to housing**

<div align="center">
<img src="images/Pictures/sensorandhousing.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

Drops of superglue in bottom shell center ring. Place disc, press 30 sec.

---

**G3. Strip Siamese wire**

Cut 10-12" Siamese wire, strip both ends.

---

**G4. Solder TRS jack**

<div align="center">
<img src="images/Pictures/trssolder.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

- Red → TIP
- Black → SLEEVE

💡 Use multimeter continuity to ID terminals.

---

**G5. Solder to piezo**

<div align="center">
<img src="images/Pictures/sensorsolder.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

Other end:
- Red → center disc
- Black → outer brass ring

---

**G6. Glue piezo to neoprene**

<div align="center">
<img src="images/Pictures/sensorinhousing.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

Superglue on neoprene top. Center piezo (brass down, wires up). Wire in strain relief channel. Press 30 sec.

---

**G7. Assemble housing**

<div align="center">
<img src="images/Pictures/sensorholding.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

Superglue below wire, on wire, inside rim of top shell. Nest top/bottom, hold 30 sec.

---

**G8. Mount to drum**

<div align="center">
<img src="images/Pictures/housinginsert.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

Mount 4 housings to drum underside using 2x M3x8mm screws per housing. Neoprene side against wood. Snug, don't over-tighten.

---

### **5.6: Mounting Hardware**

**H1. Mark mounting holes**

<div align="center">
<img src="images/Pictures/speakerbracket.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

Place speaker bracket on rear baseplate, centered. Mark 4 mounting holes.

---

**H2. Drill mounting holes**

<div align="center">
<img src="images/Pictures/bracketholes.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

Drill 4 marked holes with 6mm bit.

---

**H3. Install bolts**

<div align="center">
<img src="images/Pictures/bracketbolts.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

Feed 4x M6x16 bolts + washers through baseplate, protruding rear.

---

**H4. Attach speaker mount**

<div align="center">
<img src="images/Pictures/bracketconnected.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

Attach adjustable mount to bracket, secure with M6 knobs.

---

**H5. Assemble faceplates to baseplate**

<div align="center">
<img src="images/Pictures/boltsthrough1.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

Feed 14x M6x18 bolts (on drum faces) through baseplate holes. Secure with M6 knobs, finger-tight+.

---

**H6. Route sensor wires**

<div align="center">
<img src="images/Pictures/wireroute.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

Route sensor wires through nearest opening to drum back.

---

**H7. Attach TRS barrel mounts**

<div align="center">
<img src="images/Pictures/couplersempty.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>
<div align="center">
<img src="images/Pictures/couplersmounted.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

3D print barrel mounts, attach with tape. One set each side. Insert couplers.

---

**H8. Connect sensor cables**

<div align="center">
<img src="images/Pictures/couplersconnected.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>
<div align="center">
<img src="images/Pictures/shortcables.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

Connect drum sensor TRS to top of couplers. Connect coiled extension cables. Wires left-to-right.

---

## **6: Control Box**

<div align="right"><sub><a href="#table-of-contents">↑ Back to Top</a></sub></div>

### **6.1: 3D Print Enclosure**

**I1. Print enclosure**

<div align="center">
<img src="images/Pictures/printer.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

Print base + lid. 0.2mm layer, Gyroid 20%, auto supports.

---

**I2. Add TRS jacks & USB coupler**

<div align="center">
<img src="images/Pictures/trsports.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>
<div align="center">
<img src="images/Pictures/usb.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>
<div align="center">
<img src="images/Pictures/trsandusb.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

- Insert 4x TRS jacks into back holes, align with depressions. Hot glue.
- Insert USB coupler in bottom opening (USB-A out, USB-C in). Hot glue.

---

**I3. Mount circuit to lid**

<div align="center">
<img src="images/Pictures/grommetstages.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>
<div align="center">
<img src="images/Pictures/grommetin.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

Set M2x4 inserts in 4 standoffs (smooth lip down). Heat insert with soldering iron tip, press flush vertical.

<div align="center">
<img src="images/Pictures/buttonsfit.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>
<div align="center">
<img src="images/Pictures/terminals1.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

Place circuit (buttons/display through lid holes). Attach with 4x M4x4mm bolts. Snug.

---

**I4. Attach hinges**

<div align="center">
<img src="images/Pictures/hinge.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>
<div align="center">
<img src="images/Pictures/filamentinsert.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

Cut 1.75mm filament to fit 3 hinge sections. Cut end at angle, feed through, trim flush. Repeat other hinge.

Optional: Push filament out few mm, trim, push back in 1mm for cleaner edge.

---

### **6.2: Connect Wiring**

**J1. Create wires**

<div align="center">
<img src="images/Pictures/ribboncable.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

Cut 2x 215mm 4-wire ribbon cable. Crimp with 22 AWG ferrules.

---

**J2. Connect ground wires**

<div align="center">
<img src="images/Pictures/newgndterminal.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>
<div align="center">
<img src="images/Pictures/trswires.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

Connect one ribbon set: 4 wires → green GND terminal block (circuit). Opposite end → GND terminals of 4 TRS jacks (any order).

---

**J3. Connect signal wires**

<div align="center">
<img src="images/Pictures/signalterminalnumbers.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>
<div align="center">
<img src="images/Pictures/newsignalterminal.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

Connect other ribbon set: 4 wires → blue SIGNAL terminal block (circuit). Opposite end:
- Terminal 1 → TIP of TRS Jack 1
- Terminal 2 → TIP of TRS Jack 2
- Terminal 3 → TIP of TRS Jack 3
- Terminal 4 → TIP of TRS Jack 4

---

**J4. Connect XIAO to USB coupler**

Using short USB-C cable, connect XIAO → USB-C coupler.

**Final wired result:**

<div align="center">
<img src="images/Pictures/allwires.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>
<div align="center">
<img src="images/Pictures/finalbox.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>
<div align="center">
<img src="images/Pictures/closedbox.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

---

### **6.3: Floor Stand & Mount**

**K1. Mount drum to stand**

<div align="center">
<img src="images/Pictures/wirerouteold.png" width="350px" style="display: block; margin: 0 auto;">
</div>

Mount speaker bracket to stand.

---

**K2. Mount control box**

Attach phone holder to stand center post. Attach control box to holder. Connect drum sensor TRS → control box TRS jacks.

---

**K3. Adjust height/angle**

Adjust for comfort.

---

**K4. Add drum cover**

<div align="center">
<img src="images/Pictures/drumcover.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

Add rubber cover, towel, mouse pad, etc. Arcade drum skin recommended: [Link](https://taiko.ac/products/rubber-drum-pad)

💡 Adjust thresholds for any cover via Taiko-Tune.

---

## **7: Flash Firmware**

<div align="right"><sub><a href="#table-of-contents">↑ Back to Top</a></sub></div>

💾 [Download flash files](https://ouchitaikoproject.github.io/OuchiTaikoProject/DownloadFiles/KillerQsOuchiTaikoProjectFlashFiles.zip)

**Step 1: Boot mode**

Hold "B" button on XIAO while connecting USB. Board appears as removable drive.

---

**Step 2: Wipe board**

Drag `universal_flash_nuke.uf2` to drive. Auto-reboots. Drag again. Auto-reboots.

---

**Step 3: Flash firmware**

Drag `KillerQsOuchiTaikoFirmware.uf2` to drive. Auto-reboots. Drive won't reappear = correct.

If no auto-reboot: wait 15 sec, unplug/replug.

---

## **8: Calibration**

<div align="right"><sub><a href="#table-of-contents">↑ Back to Top</a></sub></div>

### **Button/Pad Test (Pre-Calibration)**

1. Hold SELECT 1 sec → system menu
2. Default: SWITCH TATACON mode
3. Stay in menu (prevents false triggers)
4. Visit [Meloncolle Taiko](https://meloncolle.com/tatacon), test all 4 pads + 14 buttons
5. Verify screen matches physical input

Ignore false triggers - calibration fixes this.

---

### **Taiko-Tune™ Auto-Calibration**

World's first automated Taiko calibration system. Analyzes playing style, calculates optimal thresholds, detects crosstalk.

**Quick Start - All 4 Pads:**

Launch:
- **Hold START** 1 sec (fastest)
- **Menu:** Hold SELECT → Settings → Drum Settings → Drum Thresholds → Auto Taiko-Tune → Analyze All 4 Drums → Start Analysis

**Wizard Process:**
- 2 passes, 8 calibrations total
- **Pass 1:** LK → LD → RD → RK
- **Pass 2:** RK → RD → LD → LK (reverse for crosstalk compensation)
- Per drum:
  1. Wait 3 sec countdown (samples noise - DON'T touch drum)
  2. Hit indicated pad naturally until 100%
     - Mix light taps + strong hits like gameplay
     - Use normal playing intensity
  3. Results auto-apply, 3 sec display, auto-advance
- **Time:** ~5-8 min total

---

**Single Pad Calibration:**

1. Hold SELECT 1 sec
2. Settings → Drum Settings → Drum Thresholds → Auto Taiko-Tune
3. Choose drum (LK/LD/RD/RK)
4. Start Analysis
5. Wait 3 sec countdown
6. Hit naturally until 100%
7. Results auto-apply

**Cancel:** Press B anytime, restores original thresholds.

---

**Why Taiko-Tune vs Manual?**
- Faster (5-8 min vs 30+ min)
- More accurate (math vs guesswork)
- Adapts to playing style
- Eliminates crosstalk
- Recalibrate anytime

---

### **Manual Threshold Adjustment**

Use for precise control or fine-tuning after Taiko-Tune.

**Step 1: Enter manual mode**

1. Hold SELECT 1 sec
2. Settings → Drum Settings → Drum Thresholds → Single Pad Adjust
3. Choose drum

**Step 2: Adjust (0-4095)**

- **D-PAD UP:** ↑ threshold (less sensitive)
- **D-PAD DOWN:** ↓ threshold (more sensitive)
- **A:** Save
- **B:** Cancel

**Troubleshooting:**
- Light hits don't register → LOWER value
- False hits → RAISE value
- Crosstalk (one drum triggers another) → RAISE victim drum by 10-20, repeat

**Guidelines:**
- Don (center): 40-80
- Ka (rim): 60-100
- Varies by construction/cover/style

**Recalibrate when:**
- Change covers/padding
- Sensitivity drifts
- Switch playing styles
- Hardware mods

💡 Mix methods: Taiko-Tune baseline, manual fine-tune ±10-20.

---

### **Extended Settings**

**Big Hit Arcade Mode**

Authentic arcade scoring - hit harder for Big Notes (vs simultaneous home method).

- Set ON
- Threshold: Start ~850 (2-3x normal threshold)
- ↑ value = lower Big Hit threshold
- ↓ value = harder hit required

---

**SimulTap Mode**

Toggle simultaneous drum hits.

- ON: Use drum for non-Taiko rhythm games (allows simultaneous hits)
- OFF: Taiko games (prevents impossible simultaneous hits)

---

**Hold Time**

Hit hold duration before accepting next hit.

- Range: 25-35ms
- ↑ if double hits register too fast
- ↓ if hits feel laggy
- 25ms+ recommended for Nintendo Switch

---

### **OLED Display**

- Streak counter (resets after 1 sec idle)
- 4 animated characters (LK/LD/RD/RK) confirm hits
- Live mode/stats

---

### **Pro Tips**

- Different covers need threshold adjustment
- Debug mode shows raw ADC values (200-400 baseline)

**You're Ready to Play!**

---

## **9: Downloads**

<div align="right"><sub><a href="#table-of-contents">↑ Back to Top</a></sub></div>

<div align="center">
<img src="images/Pictures/downloadimage.png" width="350px" style="display: block; margin: 0 auto;">
</div>

- [SVG/STL Files](https://ouchitaikoproject.github.io/OuchiTaikoProject/DownloadFiles/KillerQsOuchiTaikoProjectSTLandSVGFiles.zip)
- [Circuit Schematic PDF](https://ouchitaikoproject.github.io/OuchiTaikoProject/DownloadFiles/KillerQsOuchiTaikoProjectCircuitSchematic.pdf)
- [Firmware + Nuke](https://ouchitaikoproject.github.io/OuchiTaikoProject/DownloadFiles/KillerQsOuchiTaikoProjectFlashFiles.zip)

---

## **10: Troubleshooting**

<div align="right"><sub><a href="#table-of-contents">↑ Back to Top</a></sub></div>

**🖥️ DISPLAY**
- Blank: Check I2C (GPIO 6/7), 3.3V power
- Frozen: Unplug/reconnect USB
- Menu won't open: Hold SELECT 1 sec
- No animation: Check sensor registration

**🥁 SENSORS**
- No response: Check diode polarity, TRS wiring
- False triggers: Increase thresholds
- Missed hits: Decrease thresholds, check mounting
- Crosstalk: Increase threshold on triggering pad

**🎮 MODE SWITCHING**
- Freezes: Unplug/reconnect USB-C
- No change: Press EAST to confirm

💡 Research controller mode compatibility for specific game versions.

---

## **11: Menu Reference**

<div align="right"><sub><a href="#table-of-contents">↑ Back to Top</a></sub></div>

### **Quick Access**

- **Hold SELECT (1 sec):** System menu
- **Hold START (1 sec):** Taiko-Tune (all 4 drums)

---

### **Navigation**

| Button | Action |
|--------|--------|
| LEFT/RIGHT | Navigate / Toggle / Cycle |
| UP/DOWN | Adjust (hold = fast-repeat) |
| EAST (A) | Confirm / Save |
| SOUTH (B) | Cancel / Back / Restore |

---

### **Menu Tree**

```
SYSTEM MENU
│
├── 🎮 Controller Modes
│   ├── Switch Tatacon │ Switch Pro │ PS3 DS3 │ PS4 Tatacon │ PS4 DS4
│   ├── Keyboard P1 │ Keyboard P2 │ Xbox360 │ Android │ iOS
│   └── Analog P1 │ Analog P2 │ MIDI │ Debug
│   [L/R select, A confirm & reboot]
│
├── 🥁 Drum Settings
│   ├── Drum Thresholds
│   │   ├── Auto Taiko-Tune ⚡
│   │   │   ├── All 4 Drums │ LK │ LD │ RD │ RK
│   │   │   [L/R select, A start]
│   │   ├── Manual Pad Adjust
│   │   │   ├── LK (0-4095) │ LD │ RD │ RK
│   │   │   [U/D adjust, A save, B cancel]
│   │   └── Reset Thresholds [Yes/No]
│   ├── Big Hit Mode
│   │   ├── Off │ Light (2000) │ Medium (2500) │ Heavy (3000)
│   │   └── Custom (0-4095) [U/D adjust]
│   ├── SimulTap [ON/OFF]
│   └── Hold Time (0-255ms) [U/D]
│
├── 💡 LED Settings
│   ├── Brightness (0-255)
│   └── Player Color (PS4) [ON/OFF]
│
├── ℹ️ About
│   ├── OuchiTaiko Project by KillerQ
│   ├── ouchitaiko.com
│   ├── Firmware v1.0 Oct 2025
│   └── Based on: DonCon2040 (MIT) & HIDtaiko (Apache 2.0)
│
├── 🔄 Reset [Yes/No]
│
└── 💾 USB Flash Mode [Yes/No]
```

**Idle Screen:** Mode (top), streak counter (center), 4 animated spheres (LK/LD/RD/RK bottom)

---

## **12: About**

<div align="right"><sub><a href="#table-of-contents">↑ Back to Top</a></sub></div>

Hybrid creation building on:
- **[DonCon 2040](https://github.com/ravinrabbid/DonCon2040)** by ravinrabbid - Core firmware, navigation, OSD hardware
- **[HIDtaiko](https://github.com/kasasiki3/HIDtaiko)** by kasasiki3 - Circuit inspiration

Credit to:
- Gadgetoid - [pico-universal-flash-nuke](https://github.com/Gadgetoid/pico-universal-flash-nuke)
- [Boxes.py](https://boxes.hackerspace-bamberg.de/) - Enclosure design resource
- Allspice (Discord) - Build testing, guide refinement

**Communities:**
- [Taiko no Tatsujin Modding! Discord](https://discord.com/invite/HFm37aA5zr)
- [Cons&Stuff :) Discord](https://discord.com/invite/P4CpVHrR)
- [OpenStick - GP2040-ce Discord](https://discord.com/invite/openstickcommunity-1049366310389289001)

**Questions?** Use GitHub Discussions or Discord channels above.

**Share your build!** Tag KillerQ97 in your favorite DIY gaming communities.

www.ouchitaiko.com

---

## **13: Copyright**

<div align="right"><sub><a href="#table-of-contents">↑ Back to Top</a></sub></div>

### License Transparency

Project adheres to all licensing. Based on [DonCon2040](https://github.com/ravinrabbid/DonCon2040) (MIT) and [HIDtaiko](https://github.com/kasasiki3/HIDtaiko) (Apache 2.0).

Custom features (Big Notes detection, SimulTap, PS4 always-on, adaptive baseline, mode-switching fixes, animated display, enhanced menu, circuit/drum design) shared under same open-source spirit.

**Build upon this? Honor licenses with attribution + license notices. Tag KillerQ.**

---

### Attribution Chain

- **DonCon2040 firmware:** © ravinrabbid (MIT)
- **HIDtaiko components:** © kasasiki3 (Apache 2.0)
- **Custom modifications:** © KillerQ (Dual MIT/Apache 2.0)

---

### Distribution Requirements

Include:
1. MIT License copy (DonCon2040)
2. Apache 2.0 copy (HIDtaiko)
3. Copyright notices (both projects)
4. Contributor attribution

---

## License Documentation

### 1. KillerQ's Contributions

**Dual-licensed:** MIT + Apache 2.0

**Copyright (c) 2025 KillerQ**

---

### 2. DonCon2040 Firmware

**Author:** ravinrabbid
**License:** MIT
**Copyright (c) 2021 ravinrabbid**

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

### 3. HIDtaiko Components

**Author:** kasasiki3
**License:** Apache 2.0
**Copyright 2022 kasasiki3**

```
Apache License
Version 2.0, January 2004
http://www.apache.org/licenses/

TERMS AND CONDITIONS FOR USE, REPRODUCTION, AND DISTRIBUTION

1. Definitions.

   "License" shall mean the terms and conditions for use, reproduction,
   and distribution as defined by Sections 1 through 9 of this document.

   "Licensor" shall mean the copyright owner or entity authorized by
   the copyright owner that is granting the License.

   "Legal Entity" shall mean the union of the acting entity and all
   other entities that control, are controlled by, or are under common
   control with that entity. For the purposes of this definition,
   "control" means (i) the power, direct or indirect, to cause the
   direction or management of such entity, whether by contract or
   otherwise, or (ii) ownership of fifty percent (50%) or more of the
   outstanding shares, or (iii) beneficial ownership of such entity.

   "You" (or "Your") shall mean an individual or Legal Entity
   exercising permissions granted by this License.

   "Source" form shall mean the preferred form for making modifications,
   including but not limited to software source code, documentation
   source, and configuration files.

   "Object" form shall mean any form resulting from mechanical
   transformation or translation of a Source form, including but
   not limited to compiled object code, generated documentation,
   and conversions to other media types.

   "Work" shall mean the work of authorship, whether in Source or
   Object form, made available under the License, as indicated by a
   copyright notice that is attached to or included in the work (an
   example is provided in the Appendix below).

   "Derivative Works" shall mean any work, whether in Source or Object
   form, that is based on (or derived from) the Work and for which the
   editorial revisions, annotations, elaborations, or other modifications
   represent, as a whole, an original work of authorship. For the purposes
   of this License, Derivative Works shall not include works that remain
   separable from, or merely link (or bind by name) to the interfaces of,
   the Work and Derivative Works thereof.

   "Contribution" shall mean any work of authorship, including
   the original version of the Work and any modifications or additions
   to that Work or Derivative Works thereof, that is intentionally
   submitted to Licensor for inclusion in the Work by the copyright owner
   or by an individual or Legal Entity authorized to submit on behalf of
   the copyright owner. For the purposes of this definition, "submitted"
   means any form of electronic, verbal, or written communication sent
   to the Licensor or its representatives, including but not limited to
   communication on electronic mailing lists, source code control systems,
   and issue tracking systems that are managed by, or on behalf of, the
   Licensor for the purpose of discussing and improving the Work, but
   excluding communication that is conspicuously marked or otherwise
   designated in writing by the copyright owner as "Not a Contribution."

   "Contributor" shall mean Licensor and any individual or Legal Entity
   on behalf of whom a Contribution has been received by Licensor and
   subsequently incorporated within the Work.

2. Grant of Copyright License. Subject to the terms and conditions of
   this License, each Contributor hereby grants to You a perpetual,
   worldwide, non-exclusive, no-charge, royalty-free, irrevocable
   copyright license to reproduce, prepare Derivative Works of,
   publicly display, publicly perform, sublicense, and distribute the
   Work and such Derivative Works in Source or Object form.

3. Grant of Patent License. Subject to the terms and conditions of
   this License, each Contributor hereby grants to You a perpetual,
   worldwide, non-exclusive, no-charge, royalty-free, irrevocable
   (except as stated in this section) patent license to make, have made,
   use, offer to sell, sell, import, and otherwise transfer the Work,
   where such license applies only to those patent claims licensable
   by such Contributor that are necessarily infringed by their
   Contribution(s) alone or by combination of their Contribution(s)
   with the Work to which such Contribution(s) was submitted. If You
   institute patent litigation against any entity (including a
   cross-claim or counterclaim in a lawsuit) alleging that the Work
   or a Contribution incorporated within the Work constitutes direct
   or contributory patent infringement, then any patent licenses
   granted to You under this License for that Work shall terminate
   as of the date such litigation is filed.

4. Redistribution. You may reproduce and distribute copies of the
   Work or Derivative Works thereof in any medium, with or without
   modifications, and in Source or Object form, provided that You
   meet the following conditions:

   (a) You must give any other recipients of the Work or
       Derivative Works a copy of this License; and

   (b) You must cause any modified files to carry prominent notices
       stating that You changed the files; and

   (c) You must retain, in the Source form of any Derivative Works
       that You distribute, all copyright, patent, trademark, and
       attribution notices from the Source form of the Work,
       excluding those notices that do not pertain to any part of
       the Derivative Works; and

   (d) If the Work includes a "NOTICE" text file as part of its
       distribution, then any Derivative Works that You distribute must
       include a readable copy of the attribution notices contained
       within such NOTICE file, excluding those notices that do not
       pertain to any part of the Derivative Works, in at least one
       of the following places: within a NOTICE text file distributed
       as part of the Derivative Works; within the Source form or
       documentation, if provided along with the Derivative Works; or,
       within a display generated by the Derivative Works, if and
       wherever such third-party notices normally appear. The contents
       of the NOTICE file are for informational purposes only and
       do not modify the License. You may add Your own attribution
       notices within Derivative Works that You distribute, alongside
       or as an addendum to the NOTICE text from the Work, provided
       that such additional attribution notices cannot be construed
       as modifying the License.

   You may add Your own copyright statement to Your modifications and
   may provide additional or different license terms and conditions
   for use, reproduction, or distribution of Your modifications, or
   for any such Derivative Works as a whole, provided Your use,
   reproduction, and distribution of the Work otherwise complies with
   the conditions stated in this License.

5. Submission of Contributions. Unless You explicitly state otherwise,
   any Contribution intentionally submitted for inclusion in the Work
   by You to the Licensor shall be under the terms and conditions of
   this License, without any additional terms or conditions.
   Notwithstanding the above, nothing herein shall supersede or modify
   the terms of any separate license agreement you may have executed
   with Licensor regarding such Contributions.

6. Trademarks. This License does not grant permission to use the trade
   names, trademarks, service marks, or product names of the Licensor,
   except as required for reasonable and customary use in describing the
   origin of the Work and reproducing the content of the NOTICE file.

7. Disclaimer of Warranty. Unless required by applicable law or
   agreed to in writing, Licensor provides the Work (and each
   Contributor provides its Contributions) on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
   implied, including, without limitation, any warranties or conditions
   of TITLE, NON-INFRINGEMENT, MERCHANTABILITY, or FITNESS FOR A
   PARTICULAR PURPOSE. You are solely responsible for determining the
   appropriateness of using or redistributing the Work and assume any
   risks associated with Your exercise of permissions under this License.

8. Limitation of Liability. In no event and under no legal theory,
   whether in tort (including negligence), contract, or otherwise,
   unless required by applicable law (such as deliberate and grossly
   negligent acts) or agreed to in writing, shall any Contributor be
   liable to You for damages, including any direct, indirect, special,
   incidental, or consequential damages of any character arising as a
   result of this License or out of the use or inability to use the
   Work (including but not limited to damages for loss of goodwill,
   work stoppage, computer failure or malfunction, or any and all
   other commercial damages or losses), even if such Contributor
   has been advised of the possibility of such damages.

9. Accepting Warranty or Additional Liability. While redistributing
   the Work or Derivative Works thereof, You may choose to offer,
   and charge a fee for, acceptance of support, warranty, indemnity,
   or other liability obligations and/or rights consistent with this
   License. However, in accepting such obligations, You may act only
   on Your own behalf and on Your sole responsibility, not on behalf
   of any other Contributor, and only if You agree to indemnify,
   defend, and hold each Contributor harmless for any liability
   incurred by, or claims asserted against, such Contributor by reason
   of your accepting any such warranty or additional liability.

END OF TERMS AND CONDITIONS

   Copyright [2022] [kasasiki3]

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
```

---

### Summary

Full transparency, proper credit to all contributors. README serves as authoritative legal documentation.

Thank you ravinrabbid and kasasiki3 for making this possible.

**KillerQ**

www.ouchitaiko.com

---

*OuchiTaiko Project | Smarter. Simpler. Better.*
