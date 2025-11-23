<div align="center">
<img src="images/Pictures/mainlogo.png" width="100%" style="max-width: 1200px; display: block; margin: 0 auto;">
</div>

---

<div align="center">
*In Japanese, 'ouchi' (おうち) means 'home' and 'taiko' (太鼓) means 'drum.' Together, 'OuchiTaiko' represents the joy of bringing the authentic Taiko experience from the Arcade into your own space.*



---

## **Table of Contents**
- [1: Project Overview](#1-project-overview)
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



Hi, I'm KillerQ. Welcome to my  **"OuchiTaiko Project" Arcade Controller Build Guide.** 

Congratulations, you're about to build the most functional and comprehensive Arcade-scale home Taiko drum controller ever created. This open-source guide represents 8 months of research and development, bringing the authentic arcade experience home.

Completing the OuchiTaiko Project will leave you with a pioneering adaptive drum design—the first of its kind for any home Arcade controller—addressing the limited availability and high cost of commercial units, or other units that claim to be professional.

**Smarter. Simpler. Better.**

Instead of complex analog circuits with operational amplifiers and custom PCBs, this design uses custom-designed **Adaptive Baseline Software Intelligence (ABSI)** to achieve superior performance through elegant simplicity.

All you need is your enthusiasm and some basic soldering and woodworking skills. The components linked in this guide create a perfectly functional controller when used together. Feel free to substitute compatible alternatives if you find cheaper options that achieve the same results.

### **What You'll Build: Next-Generation Features**

You will construct a professional arcade-scale Taiko drum controller engineered for premium performance and maximum compatibility, featuring:

**Performance & Design**

- **4 Velocity-Sensitive Zones:** Enhanced mechanical and electronic false-trigger isolation
- **Custom Arcade Sensors:** Drum trigger sensors with custom-designed suspension mounting that precisely mimics Japanese arcade machines (a first time available to the Public)
- **Adaptive Baseline Software Technology (ABSI):** Custom-designed, velocity-sensing triggers for automatic sensitivity adjustment (a first for any home arcade controller) as well as authentic scoring by the option of hitting Big Notes *harder* for full points - 
- **Taiko-Tune™ Auto-Calibration System:** Revolutionary automatic drum calibration system (see Section 8 for details)
- **Zero Coding or Programming Skills Required:** Simply Drag-and-drop my completed firmware for instant use and simple setup

**Hardware & Connectivity**

**OLED Display:** On-the-fly mode selection, settings, calibration, and real-time hit feedback

**COMPLETE STANDALONE TESTING:**

- **No PC Required:** Calibrate and test entire system with just USB power
- **Visual drum feedback:** 4 animated icons (Left Ka, Left Don, Right Don, Right Ka) react to hits
- **Button mapping confirmation:** Real-time indicators verify all 14 navigation buttons and button naming/labeling
- **Live stats:** Streak counter, controller mode, menu hints
- Instant confirmation that everything is wired correctly

**HARDWARE:**

- **14 Game Navigation Buttons:** Full in-game navigation regardless of game version
- **Professional Mounting:** Optimal hardware stabilization via adjustable, angled speaker stand
- **Performance:** Rivals high-end manufactured controllers and commercial Arcade units

**14 Input Modes for Maximum Compatibility**

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

### 🎥 Demo Videos

- [Finished Drum](https://youtu.be/Ji3sOdRHO0Q)
- [Some Gameplay](https://youtu.be/p4eFeo_LB5I?si=jDKb93B7uYx1qAux)
- [Roll Polling Rate Demo](https://youtu.be/wEw9HbGcR-s)
- [Hit Velocity Detection](https://www.youtube.com/watch?v=wceSXgtBcfE)

---

## **2: Parts List for Electronics**

<div align="right"><sub><a href="#table-of-contents">↑ Back to Top</a></sub></div>

The Amazon links reference parts that are 100% compatible when used together. You can source these items from any supplier as long as they meet the specifications.

**Important:** "Quantity" refers to the number of individual items you need, NOT the number of packages you need to order since some items come with multiple items in one package.

<div style="overflow-x: auto;">

| #    | Item                                                         | Qty  | Product Link                   |
| ---- | ------------------------------------------------------------ | ---- | ------------------------------ |
| 1    | XIAO RP2040 board (Seeed Studio)                             | 1    | [Link](https://a.co/d/g9dFG88) |
| 2    | GODIYMODULES MCP23017 I2C expander Board                     | 1    | [Link](https://a.co/d/iBEbs4b) |
| 3    | 1N4148 diodes                                                | 4    | [Link](https://a.co/d/jlNKdaJ) |
| 4    | 0.47µF/470nF Ceramic Capacitors                              | 4    | [Link](https://a.co/d/8XWar8W) |
| 5    | 3.5mm TRS FEMALE jacks w/ screw terminals                    | 4    | [Link](https://a.co/d/2umR9us) |
| 6    | 3.5mm TRS male Plugs w/ standard ends                        | 4    | [Link](https://a.co/d/414YG9z) |
| 7    | 27mm Piezo sensors                                           | 4    | [Link](https://a.co/d/hbacbaF) |
| 8    | 128x64 Mono OLED display (I2C)                               | 1    | [Link](https://a.co/d/bHAtiY2) |
| 9    | 6mmx6mmx8mm Tactile Switches                                 | 14   | [Link](https://a.co/d/aa5ppfs) |
| 10   | 22 AWG Siamese wire                                          | 1    | [Link](https://a.co/d/0pGa2wH) |
| 11   | USB-C to USB-A Female Coupler                                | 1    | [Link](https://a.co/d/9WTmxTu) |
| 12   | Short USB-C to USB-C extension cable                         | 1    | [Link](https://a.co/d/cVbKVmQ) |
| 13   | 6 inch, coiled 3.5mm TRS audio extension cable (Male to Male) | 4    | [Link](https://a.co/d/cSKAJ7D) |
| 14   | 3.5mm TRS barrel coupler                                     | 4    | [Link](https://a.co/d/fOiYb5s) |
| 15   | 4-Pin, 0.2 Inch Pitch Screw Terminal Blocks                  | 2    | [Link](https://a.co/d/1X20DtA) |
| 16   | 22 AWG 4-Wire Ribbon Connection Wire                         | 1    | [Link](https://a.co/d/cXyTYy1) |
| 17   | ElectroCookie Circuit Board(88.9mm x 96.5mm)                 | 1    | [Link](https://a.co/d/i5jfYjs) |
| 18   | 10ft USB-A to USB-A Cable                                    | 1    | [Link](https://a.co/d/cBqJJua) |
| 19   | M2x3x3 Threaded Inserts                                      | 4    | [Link](https://a.co/d/8VZdEql) |
| 20   | M2x4 bolts                                                   | 4    | [Link](https://a.co/d/4cPSWVY) |
| 21   | 22 AWG Wire Ferrules and Crimper                             | 8    | [Link](https://a.co/d/hu5OIl3) |

</div>

---

## **3: Parts List for Hardware**

<div align="right"><sub><a href="#table-of-contents">↑ Back to Top</a></sub></div>

### **Required Tools and Supplies**

This guide assumes you have access to the following:

1. Laser cutter or CNC machine (or other means to cut wood)
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

### **Hardware Parts List**

<div style="overflow-x: auto;">

| #    | Item                                                         | Qty  | Product Link                                                 |
| ---- | ------------------------------------------------------------ | ---- | ------------------------------------------------------------ |
| 1    | 6mm Cabinet-Grade MDF (1/4 inch thick works too). Amount needed dictated by sizes in SVG files | 1    | [Home Depot](https://www.homedepot.com/p/1-4-in-x-2-ft-x-4-ft-Medium-Density-Fiberboard-1508104/202089069) |
| 2    | Strong Wood Glue                                             | 1    | [Link](https://a.co/d/1uKv6cR)                               |
| 3    | M3x8mm bolts                                                 | 8    | [Link](https://a.co/d/52Q8UtD)                               |
| 4    | M3x5mm threaded inserts                                      | 8    | [Link](https://a.co/d/bKB6OpW)                               |
| 5    | M6x10mm wood threaded inserts                                | 14   | [Link](https://a.co/d/jhY5rYA)                               |
| 6    | M6x20mm nylon bolts                                          | 14   | [Link](https://a.co/d/bPitQiX)                               |
| 7    | M6 threaded 20mmx15mm rubber isolators (one side M6 Female, other side with M6x18mm bolt) | 14   | [Link](https://a.co/d/9dWHezk)                               |
| 8    | 3D printer filament (PLA)                                    | 1    | [Link](https://a.co/d/7cCSDtJ)                               |
| 9    | Gel Superglue                                                | 1    | [Link](https://a.co/d/dxU7lfw)                               |
| 10   | Loctite thread adhesive - Medium                             | 1    | [Link](https://a.co/d/2W890aJ)                               |
| 11   | 2.2mm thick Scuba Knit Neoprene fabric (only 4"x4" total needed) | 1    | [Link](https://a.co/d/dfMhZ4k)                               |
| 12   | Finger Knobs with pass-through M6 threads                    | 18   | [Link](https://a.co/d/hjL3QQP)                               |
| 13   | Mini PA speaker stand (or a stand with a compliant interface that matches your mount below) | 1    | [Link](https://a.co/d/2YkmhPj)                               |
| 14   | Adjustable Angle Speaker bracket (or a compliant interface that matches your stand above) | 1    | [Link](https://a.co/d/gQioU8i)                               |
| 15   | Rubber Taiko Drum Cover (optional but crucial for Arcade feel. Any thin sound-dampening material with bounce works: towel, mouse pad, etc.) | 1    | [Link](https://taiko.ac/products/rubber-drum-pad)            |
| 16   | Spring-Loaded Phone Holder with Gooseneck Arm                | 1    | [Link](https://a.co/d/fRja2H6)                               |

</div>

---

## **4: Build the Circuit**

<div align="right"><sub><a href="#table-of-contents">↑ Back to Top</a></sub></div>

<div align="center">
<img src="images/Pictures/schematic.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>
📸 Photo Documentation Note
Early assembly photos show different terminal headers than what's in your parts list, and some signal wires may be routed slightly differently. Don't worry about these mismatches - the installation process is identical. You'll see the correct terminals when you reach Step 8.

### **Circuit Overview**

This build uses a **single circuit board** design with everything mounted on one ElectroCookie double-column protoboard:

- **XIAO RP2040**: Brain of the system, handles drum sensors
- **MCP23017**: Controls OLED display and navigation buttons
- **OLED Display**: Status and menu display
- **14 Navigation Buttons**: Gamepad controls

### **Understanding Shared Rows**

The ElectroCookie protoboard has **shared rows** where multiple holes are electrically connected. This allows you to:

- Solder components into any hole in that row
- Connect wires between shared row holes without additional soldering
- Keep the build clean and organized

**When this guide specifies a hole location** (e.g., "Row D, hole 5"), you **must** use that exact hole. **When it says "any hole in the shared row,"** you have flexibility to choose the most convenient location.

---

### **IMPORTANT: Board Orientation**

This protoboard has **row letters and numbers printed on both sides**.

- **FRONT face** = Display and buttons side (use the rows and hole numbers on the front face when placing the display and button placement for Steps 1-2)
- **BACK face** = XIAO, MCP23017, and screw terminals side (use the rows and hole numbers on the BACK face when referencing Steps 3-12)

Always verify which side you're working on before placing components.

---

### **Step 1: Mount OLED Display (FRONT face)**

Mount OLED Display on the **top center** of the FRONT face.

**OLED Placement:**

- **Location:** Row J, holes 14-17 (FRONT face coordinates)
- **Orientation:** Screen facing up
- Solder in place

<div align="center">
<img src="images/Pictures/oledmount.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

**OLED pins** (will be wired in Step 7):

- VCC (Row A, hole 15)
- GND (Row A, hole 14)
- SCL (Row A, hole 16)
- SDA (Row A, hole 17)

<div align="center">
<img src="images/Pictures/oledback.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

---

### **Step 2: Mount Buttons (FRONT face)**

**Button Orientation:**

- Orient buttons with legs on the **left and right side** (NOT up and down)
- For visual consistency: align all button tabs the same way (all tabs up OR all tabs down)

**CRITICAL - Use Exact Positions:**
The table below specifies exact FRONT face hole positions. These align with the control box openings to ensure proper fit of the controller face plate.

<div align="center">
<img src="images/Pictures/buttonsall.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

**Button Position Reference (FRONT face):**

| Button                 | Ground Leg Position | Signal Leg Position |
| ---------------------- | ------------------- | ------------------- |
| **LEFT SIDE BUTTONS**  |                     |                     |
| UP                     | Row E, hole 4       | Row E, hole 6       |
| Down                   | Row A, hole 4       | Row A, hole 6       |
| Left                   | Row C, hole 1       | Row C, hole 3       |
| Right                  | Row C, hole 7       | Row C, hole 9       |
| L                      | Row F, hole 1       | Row F, hole 3       |
| Select                 | Row A, hole 12      | Row A, hole 14      |
| Share                  | Row H, hole 7       | Row H, hole 9       |
| **RIGHT SIDE BUTTONS** |                     |                     |
| North                  | Row E, hole 27      | Row E, hole 25      |
| South                  | Row A, hole 27      | Row A, hole 25      |
| West                   | Row C, hole 24      | Row C, hole 22      |
| East                   | Row C, hole 30      | Row C, hole 28      |
| R                      | Row F, hole 30      | Row F, hole 28      |
| Start                  | Row A, hole 19      | Row A, hole 17      |
| Home                   | Row H, hole 24      | Row H, hole 22      |

**Installation:**

1. Insert each button into its specified position, bend legs behind board to hold into place
2. Solder both legs in place
3. Verify button alignment matches reference photos

<div align="center">
<img src="images/Pictures/buttonback.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

**CRITICAL - Prevent 2 existing Signal Shorts:**
Because of the shared rows, two button pairs share signal rows and will short without this fix:

- **NORTH/SOUTH buttons**
- **UP/DOWN buttons**

<div align="center">
<img src="images/Pictures/drill1.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

<div align="center">
<img src="images/Pictures/drill2.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

**Quick Fix (30 seconds per pair):**

1. Find the hole centered between the two signal legs
2. Scrape away copper with 3mm drill bit or knife
3. Test with multi-meter: touch both signal legs - NO beep = success.  If you hear a beep, remove more copper metal and try again.
4. Repeat for second button pair

---

### **Step 3: Mount XIAO RP2040 (BACK face)**

**Flip board to BACK face.** All remaining steps use BACK face coordinates.

<div align="center">
<img src="images/Pictures/socket.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

<div align="center">
<img src="images/Pictures/socketandboard.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

**XIAO Placement:**

- Solder XIAO RP2040 on the **right side** of board using header pins or IC socket for easy removal
- Orient with **USB-C port facing left**
- **Top row of pins:** Row D, holes 1-7 (BACK face)
- **Bottom row of pins:** Row H, holes 1-7 (BACK face)

**Connect Ground:**

- Connect any pin in XIAO's **GND** shared row to the common GND rail
- Take a moment to jump/connect all GND rails together across the board

---

### **Step 4: Install Drum Sensor Protection Circuits (BACK face)**

Each of the 4 drum sensors requires identical protection circuitry.

**Diode & Capacitor Placement:**

| Drum Zone | XIAO Pin | Diode Anode + Capacitor Location (BACK face) |
| --------- | -------- | -------------------------------------------- |
| Left Ka   | A0       | Row J, hole 7                                |
| Left Don  | A1       | Row J, hole 6                                |
| Right Don | A2       | Row J, hole 5                                |
| Right Ka  | A3       | Row J, hole 4                                |

**For Each Sensor:**

1. **Diode:** Place anode (non-striped end) of 1N4148 diode into the specific hole listed below (do not solder yet)

2. Place cathode (striped end) into GND rail hole directly below (do not solder yet)
   
   <div align="center">
   <img src="images/Pictures/diodes.jpg" width="350px" style="display: block; margin: 0 auto;">
   </div>
   
   
   <div align="center">
   <img src="images/Pictures/protectionbelow.jpg" width="350px" style="display: block; margin: 0 auto;">
   </div>

3. **Capacitor:** Place either leg of 0.47µF capacitor into same hole as diode anode (do not solder yet)

4. Place other leg into same GND rail hole as the diode cathode (do not solder yet)
   
   <div align="center">
   <img src="images/Pictures/doubleprotection.jpg" width="350px" style="display: block; margin: 0 auto;">
   </div>
   
   
   <div align="center">
   <img src="images/Pictures/protectionbelow2.jpg" width="350px" style="display: block; margin: 0 auto;">
   </div>

5. **Solder:** NOW, you can solder the combined diode + capacitor legs together. This saves space, and keeps things looking clean.
   
   <div align="center">
   <img src="images/Pictures/protectionbeneath.jpg" width="350px" style="display: block; margin: 0 auto;">
   </div>

---

### **Step 5: Connect Button Grounds (BACK face)**

**Flip board as needed** - button legs are now accessible on BACK.

- Connect one leg of each button (ground leg) to GND rail
- **Recommended:** Daisy-chain grounds for cleaner routing - see reference photo for layout Conversely, on the right set of buttons, designate the GND to all of the legs on the right side of the button (toward the left edge of the board) as ground legs.

<div align="center">
<img src="images/Pictures/buttongrounds.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

---

### **Step 6: Prepare MCP23017 Board**

**Solder Header Pins:**

1. Solder the **single row of 10 header pins** into the bottom row of holes on the MCP23017
2. This row starts with **A2** (left) and ends with **VCC** (right)
3. The double row of pins on top remains empty (for button signal connections later on)

<div align="center">
<img src="images/Pictures/expandersolder.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

---

### **Step 7: Pre-Wire I²C Lines Under MCP (BACK face)**

Before mounting the MCP23017, pre-wire power and I²C connections underneath where it will sit.

**Temporarily place** MCP23017 centered horizontally (Row E, holes 11-21, BACK face, double pins facing up). Note where it sits, then remove it.

<div align="center">
<img src="images/Pictures/expanderterminals.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

**Make these 4 connections underneath:**

| Connection | From (Under MCP) | To (OLED)      | Purpose              |
| ---------- | ---------------- | -------------- | -------------------- |
| VCC        | Row A, hole 11   | Row A, hole 15 | Power to display     |
| GND        | Row A, hole 12   | Row A, hole 14 | Ground to display    |
| SCL        | Row A, hole 13   | Row A, hole 16 | I²C Clock to display |
| SDA        | Row A, hole 14   | Row A, hole 17 | I²C Data to display  |

<div align="center">
<img src="images/Pictures/sdasclunderexpander.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

(photo shows SDA and SCL connection progress.  You will also connect VCC and GND to the Display in this step)

**Now mount MCP23017:**

- Place bottom single row of 10 pins into Row E, holes 11-21 (BACK face)
- Verify **VCC pin** is in Row E, hole 11
- Verify **A2 pin** is in Row E, hole 21
- Solder these header pins to the circuit board

<div align="center">
<img src="images/Pictures/expanderterminalsfirst.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

(in the above photo the VCC and GND were routed different than the guide, but work the same)

---

### **Step 8: Connect Power & Ground**

Connect/verify power and ground between XIAO, MCP23017, and Display.

**Connections:**

1. **XIAO 3.3V** → **MCP23017 VCC** → **OLED VCC** (OLED already connected via Step 7)
2. **XIAO GND** → **MCP23017 GND** → **OLED GND** (OLED already connected via Step 7)
3. Verify all three components (XIAO, MCP, Display) share common ground

---

### **Step 9: Connect I²C Data Lines**

Connect I²C between XIAO and MCP23017.

| Connection | From XIAO                 | To MCP23017               |
| ---------- | ------------------------- | ------------------------- |
| SDA        | Any pin in SDA shared row | Any pin in SDA shared row |
| SCL        | Any pin in SCL shared row | Any pin in SCL shared row |

**Verify I²C connections:**

- XIAO SDA → MCP23017 SDA → Display SDA (all connected)
- XIAO SCL → MCP23017 SCL → Display SCL (all connected)

<div align="center">
<img src="images/Pictures/powercomms.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

---

### **Step 10: Connect Button Signals to MCP23017 (BACK face)**

Connect button signal legs to MCP23017 pins. Button legs are accessible from the BACK face.

**Soldering Technique** (MCP holes are small):

1. **Prepare MCP holes:** Fill each MCP pin hole with small amount of solder
2. **Prepare wires:** Strip to 1-2mm max, tin the ends with solder
3. **Solder:** Place tinned wire on solder-filled hole, press iron down onto solder pad, wire sinks into solder
4. **Order:** Start with Row A pins (top), then Row B pins

**Complete Button Wiring Map:**

| Button                 | MCP Pin | Wire Connection                                      |
| ---------------------- | ------- | ---------------------------------------------------- |
| **LEFT SIDE BUTTONS**  |         |                                                      |
| UP                     | B0      | MCP B0 → any hole in UP button signal shared row     |
| Down                   | B1      | MCP B1 → any hole in DOWN button signal shared row   |
| Left                   | B2      | MCP B2 → any hole in LEFT button signal shared row   |
| Right                  | B3      | MCP B3 → any hole in RIGHT button signal shared row  |
| L                      | B4      | MCP B4 → any hole in L button signal shared row      |
| Select                 | B5      | MCP B5 → any hole in SELECT button signal shared row |
| Share                  | B6      | MCP B6 → any hole in SHARE button signal shared row  |
| **RIGHT SIDE BUTTONS** |         |                                                      |
| North                  | A0      | MCP A0 → any hole in NORTH button signal shared row  |
| South                  | A1      | MCP A1 → any hole in SOUTH button signal shared row  |
| West                   | A2      | MCP A2 → any hole in WEST button signal shared row   |
| East                   | A3      | MCP A3 → any hole in EAST button signal shared row   |
| R                      | A4      | MCP A4 → any hole in R button signal shared row      |
| Start                  | A5      | MCP A5 → any hole in START button signal shared row  |
| Home                   | A6      | MCP A6 → any hole in HOME button signal shared row   |

**Wiring Tips:**

- Wire Row A pins first (work from closest to furthest button)
- Then wire Row B pins
- Route wires to minimize crossover and blocking

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

---

### **Step 11: Mount Terminal Blocks (BACK face)**

Place two 4-pin screw terminal blocks on BACK face 

**Terminal Block 1 (GND):**

- **Location:** Row A, holes 29, 27, 25, and 23 (BACK face)
- **Orientation:** Wire openings facing UP toward top of board.
- Bridge all 4 pins together with solder or wire.
- Connect this common GND cluster to nearest GND rail

**Terminal Block 2 ( For Drum Signals):**

- **Location:** Row F, holes 30, 28, 26, and 24 (BACK face)
- **Orientation:** Wire openings facing DOWN toward bottom of board
- Leave one hole space between this block and the GND block

<div align="center">
<img src="images/Pictures/terminals3.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

**Label the signal block ports:**

- Pad 1/Left Ka (Row F, hole 24)
- Pad 2/Left Don (Row F, hole 26) 
- Pad 3/Right Don (Row F, hole 28) 
- Pad 4/Right Ka (Row F, hole 30)

These will be connected in the next step.

---

### **Step 12: Connect Drum Signal Terminals (FRONT face)**

Connect terminal block signal pins to XIAO analog pins.

<div align="center">
<img src="images/Pictures/onesignalwire.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

<div align="center">
<img src="images/Pictures/drumsignalwires2.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

**Wiring Map:**

| Drum Pad  | From Terminal Block                   | To XIAO Pin |
| --------- | ------------------------------------- | ----------- |
| Left Ka   | Any hole in Row D, hole 24 shared row | A0          |
| Left Don  | Any hole in Row C, hole 26 shared row | A1          |
| Right Don | Any hole in Row B, hole 28 shared row | A2          |
| Right Ka  | Any hole in Row A, hole 30 shared row | A3          |

These screw terminals will connect to the drum sensor TRS jacks in a step later in this guide

---

### **Circuit Complete!**

You now have a single-board circuit with:

- ✅ OLED Display
- ✅ 14 Navigation Buttons
- ✅ XIAO RP2040 (drum sensor controller)
- ✅ MCP23017 (button/display controller)
- ✅ Screw terminals for drum sensors

Continue to Section 5 to build the drum.

---

## **5: Build the Drum**

<div align="right"><sub><a href="#table-of-contents">↑ Back to Top</a></sub></div>

**Important Scale Notice:** The SVG files provided in the download are the correct scale and should **NOT** be resized. The drum dimensions are precisely calculated to work with the sensor housings and other non-scalable components. If you try and make the drum smaller, other parts will not fit later on during the project.

💡 **Scale Verification:** Before cutting, verify the SVG files are at correct scale by checking that all 14 mounting holes in the drum base measure exactly **6mm** in diameter.

**No Laser cutter or CNC access?** No worries - there are other options. Ask a friend, local shop, or check if your area has a Makerspace. Alternatively, you can print the SVG files full-size across multiple sheets (ensure your printer is set to 100% scale / "Actual Size"), overlay the paper on your wood as a template, and cut and drill by hand. Double check that your printed templates are sized properly before cutting or drilling anything.

---

### **5.1: Prepare the Wood**

#### **C1. Cut all MDF wood pieces per SVG templates**

💾 [File packet located here](https://ouchitaikoproject.github.io/OuchiTaikoProject/DownloadFiles/KillerQsOuchiTaikoProjectSTLandSVGFiles.zip)

Use your Laser or CNC machine to cut out all of the parts in template files.

---

#### **C2. Sand smooth as needed**

Sand down any rough edges from the cutting or drilling to ensure that the pieces marry up well during assembly later.

---

### **5.2: Assemble the Drum Structure**

#### **D1. Assemble and glue the rear base plates together**

Use **Wood Glue** to glue the two identical rear base plates together (they are 100% identical, just align the holes and glue together). Clamp securely or weigh down and let dry for several hours.

<div align="center">
<img src="images/Pictures/baseplate.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

---

#### **D2. Assemble and glue the drum face plate panels together**

There will be 4 finished drum faceplates that you will be assembling in during this step. Left Ka, Left Don, Right Don, and Right Ka. Each faceplate has a smooth top plate along with a corresponding bottom plate with holes in it.

**For Ka Plates:**

Let's start with the Ka plates - specifically, the Left Ka. You will be using the Left Ka TOP and the Left Ka BOTTOM Plate.

Apply wood glue to the underside of the top Ka plate and apply wood glue to the topside of the bottom Ka plate. Press the two pieces together and clamp or weigh them down for several hours. Check on them to ensure that no shifting occurs during the drying process.

**Repeat this exact same process for the Right Ka.**

**For Don Plates:**

Now, let's move on to the Don plates - specifically, the Left Don. You will be using the Left Don TOP and the Left Don BOTTOM Plate.

Apply wood glue to the underside of the top Don plate and apply wood glue to the topside of the bottom Don plate. Press the two pieces together and clamp or weigh them down for several hours. Check on them to ensure that no shifting occurs during the drying process.

**Repeat this exact same process for the Right Don.**

💡 **Note:** Ignore the 4 smaller holes in the center in this photo, your version will be different and only have 2 holes.

<div align="center">
<img src="images/Pictures/kaholes.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

---

#### **D3. Rout/sand the Ka rim edges**

Use a router or at least sand down the top sharp outer, curved edge of the **Ka rim faces**. This helps prevent stick damage and wear and tear on your drum and cover as well. Do not smooth/route the inside edges at all, just the outside curve.

<div align="center">
<img src="images/Pictures/kafront.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

---

#### **D4. Drill holes for threaded inserts**

Using an **8mm** diameter drill bit (or the specialized drill bit that came with your threaded wood inserts), locate the 14 pre-cut **6mm** holes where the rubber grommets will go, and use those holes as a drill guide. Drill **straight** down into those 6mm holes and turn them into new **8mm wide x 11mm deep** holes. Mark your drill bit at the 11mm mark with tape for easy reference so you know when to stop.

<div align="center">
<img src="images/Pictures/kahole.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

---

#### **D5. Chamfer holes**

Chamfer the (inner) rim of each 8mm hole so that the angled head of the threaded inserts will tighten down flush and feel smooth when your fingers pass over them. This chamfering can be done with a sharp screwdriver, knife, etc.

<div align="center">
<img src="images/Pictures/kagrommetout.png" width="350px" style="display: block; margin: 0 auto;">
</div>

---

#### **D6. Install the M6 threaded inserts**

Screw in the **14 individual M6 threaded wood inserts** into the corresponding holes until flush (add **Superglue** to the *outside* of threads to help permanently secure them to wood).

<div align="center">
<img src="images/Pictures/kagrommettool.png" width="350px" style="display: block; margin: 0 auto;">
</div>

<div align="center">
<img src="images/Pictures/kagrommetin.png" width="350px" style="display: block; margin: 0 auto;">
</div>


---

#### **D7. Install M3 threaded inserts**

Using a small hammer or rubber mallet, tap two M3 threaded inserts into the holes on the underside of each of the 4 faceplates. 8 inserts total. Be sure to tap them flush to the wood - add Superglue to *outside* of threads to help permanently secure to the wood. These threaded inserts are where the sensor housings will mount later on.

💡 **Note:** Your orientation will be different, this is just an example showing the sensor housing next to a threaded insert.

<div align="center">
<img src="images/Pictures/threadedinserts.png" width="350px" style="display: block; margin: 0 auto;">
</div>

---

### **5.3: Install the Rubber Isolators**

#### **E1. Cut nylon bolt head**

Cut the head off of a **20mm M6 nylon bolt**.

<div align="center">
<img src="images/Pictures/nyloncut.png" width="350px" style="display: block; margin: 0 auto;">
</div>

---

#### **E2. Apply Loctite to isolator**

Apply one drop or less of **Loctite** to threads on the inside of the rubber isolator threaded hole.

---

#### **E3. Install bolt in isolator**

Screw one end of the headless bolt into isolator until it stops.

<div align="center">
<img src="images/Pictures/isolatorgrommetside.png" width="350px" style="display: block; margin: 0 auto;">
</div>
<div align="center">
<img src="images/Pictures/grommetmeasure.png" width="350px" style="display: block; margin: 0 auto;">
</div>

---

#### **E4. Apply Loctite to drum plate inserts**

Add one drop or less of **Loctite** to inside threads of the M6 threaded inserts on the bottom face of the drum plates.

---

#### **E5. Install isolator/bolt assemblies**

Screw the nylon bolt of the rubber grommet assembly into the threaded insert in the bottom of the drum faces - finger-tighten until flush.

**Repeat steps E1-E5 for the remaining 13 similar holes.**

This metal bolt will go through the matching holes in the drum baseplate in a later step.

<div align="center">
<img src="images/Pictures/grommetinstalled.png" width="350px" style="display: block; margin: 0 auto;">
</div>

💡 **Note:** You will be directed to complete the rest of drum assembly after sensors are installed (end of Part G later in the guide).

---

### **5.4: Print Sensor Housings**

#### **F1. Print The Sensor Housings**

💾 [Files are in the file packet here](https://ouchitaikoproject.github.io/OuchiTaikoProject/DownloadFiles/KillerQsOuchiTaikoProjectSTLandSVGFiles.zip)

Print 4 complete sets of Sensor Housings (each set has a top and bottom).

Use **PLA filament**.

**Printer Settings:** 0.2mm layer height, 40% Gyroid infill, no supports needed.

---

### **5.5: Assemble Sensor Electronics**

🎥 [Video overview of sensor housing assembly](https://youtu.be/tQe-xDEqEdY)

💡 **Note:** Steps G1-G7 will be repeated 4 times - this will give you a total of 4 complete sensor dongles.

#### **G1. Cut neoprene discs**

Cut four **12mm neoprene discs** by using the SVG template.

💾 [Template found in the file packet here](https://ouchitaikoproject.github.io/OuchiTaikoProject/DownloadFiles/KillerQsOuchiTaikoProjectSTLandSVGFiles.zip)

---

#### **G2. Glue neoprene disc to housing**

Place several drops of **Superglue** into the raised center ring in the bottom shell of the housing. Place a single neoprene disc in this ring on top of the glue. Press lightly for 30 seconds.

<div align="center">
<img src="images/Pictures/sensorandhousing.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

---

#### **G3. Strip Siamese wire**

Take **10"-12" length of Siamese wire**, strip both ends exposing the two wires within.

---

#### **G4. Solder TRS Jack**

Take one end of the wire and solder the two exposed wires to the male TRS jack:

- **Red** connects to the **TIP** of the male TRS jack
- **Black** connects to the **SLEEVE** of the male TRS jack

💡 **Tip:** You can use your multimeter in Continuity mode to check which terminal is the Sleeve and which is the Tip.

<div align="center">
<img src="images/Pictures/trssolder.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

---

#### **G5. Solder to Piezo**

Take the *other* end of the stripped wire, and solder the **red** wire to piezo center disc, and solder the **black** wire to outer brass ring. The video linked at the beginning of this section illustrates this well for clarity.

<div align="center">
<img src="images/Pictures/sensorsolder.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

---

#### **G6. Glue Piezo to Neoprene Mount**

Add several drops of **Superglue** onto the top surface of neoprene that is already glued to the bottom housing shell. Center the **piezo sensor** face up (the all-brass side faces *down*, your wires will be on the top) onto the neoprene and press together. Press lightly for 30 seconds. Be sure that the wire is laying across the strain relief channel portion on one side of the bottom housing channel.

💡 **Note:** Pic varies slightly from your version - this was an earlier version. You will have a more pronounced strain relief channel.

<div align="center">
<img src="images/Pictures/sensorinhousing.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

---

#### **G7. Assemble housing**

Add a drop of **Superglue** to the strain relief channel *below* the wire, as well as on top, and add a few drops to the upper housing around the inside rim. Now assemble the top and bottom housing pieces together, press and hold for 30 seconds. The top shell of the housing will nest into place when aligned properly.

<div align="center">
<img src="images/Pictures/sensorholding.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

---

#### **G8. Mount the Sensor Housings to The Drum**

Mount your 4 completed housings to the underside of drum faces using **2 M3x8mm screws** for each housing and screw them into the threaded inserts. Be sure that the bottom (side with the neoprene disc inside) of the housing is against the wood.

Tighten snug so that the sensor housing is firmly pressed against the wood - but don't over-tighten.

Here's a photo of one housing right before it's screwed into the threaded insert:

<div align="center">
<img src="images/Pictures/housinginsert.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

---

### **5.6: Mounting Hardware Assembly**

#### **H1. Mark mounting holes**

Place your **speaker bracket** against the backside of your rear base plate so that it is centered and not blocking any other holes. Mark the 4 holes in the speaker bracket that you will use to mount it with.

<div align="center">
<img src="images/Pictures/speakerbracket.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

---

#### **H2. Drill mounting holes**

Remove the speaker plate and drill your 4 marked holes using a **6mm drill bit**.

<div align="center">
<img src="images/Pictures/bracketholes.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

---

#### **H3. Install mounting bolts**

Feed **4x M6x16 bolts** with washers through these mounting holes on baseplate so that they protrude from the rear to attach the speaker mounting plate.

<div align="center">
<img src="images/Pictures/bracketbolts.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

---

#### **H4. Attach Speaker Mount**

Attach adjustable speaker mount to the speaker bracket, secure with **M6 knobs**.

<div align="center">
<img src="images/Pictures/bracketconnected.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

---

#### **H5. Assemble Drum Faceplates to Rear Baseplate**

Now assemble the rest of the drum structure by feeding the 14 **M6x18 bolts** on the bottom of the 4 drum faces through base plate holes of the rear baseplate. It will only fit one way. Secure the drum faces against the baseplate by screwing the **M6 knobs** onto the exposed bolts. Tighten them a little more finger-tight.

<div align="center">
<img src="images/Pictures/boltsthrough1.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

---

#### **H6. Route Sensor Through Rear Baseplate**

Route the sensor wires through the nearest opening so they hang out of the back of the Drum.

<div align="center">
<img src="images/Pictures/wireroute.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

---

#### **H7. Attach TRS barrel mounts**

3D Print and Attach **TRS barrel mounts** with adhesive tape as seen in pictures, and then place the couplers in them. You will have one set of Barrel Mounts on each side of the drum.

<div align="center">
<img src="images/Pictures/couplersempty.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

<div align="center">
<img src="images/Pictures/couplersmounted.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

---

#### **H8. Connect sensor cables**

Connect the ends of the **3.5mm TRS male Drum sensors** to the top of connector of the barrel couplers, and then also connect your short, coiled 3.5mm extension cables - these extension cables will eventually plug into the female TRS jacks on your control box. The wires should connect cleanly from left to right.

<div align="center">
<img src="images/Pictures/couplersconnected.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

<div align="center">
<img src="images/Pictures/shortcables.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

---

## **6: Control Box**

<div align="right"><sub><a href="#table-of-contents">↑ Back to Top</a></sub></div>

You're almost there!

If you built the board to the exact specifications in my guide, you'll be able to 3D print the included enclosure box for a professional finish to your circuit.

---

### **6.1: 3D Print and Assemble The Control Box**

#### **I1. Print the Enclosure**

<div align="center">
<img src="images/Pictures/printer.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

Using the free .STL files provided HERE, print the control box base and lid using the following settings:

--Layer Height: .20mm

--Infill: Gyroid fill @ 20%

--Supports: Automatic



#### **I2. Add TRS Terminal Jacks & USB Coupler**

<div align="center">
<img src="images/Pictures/trsports.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>
Add the 4 TRS jacks into the 4 holes in the back of the base. You will see 4 slight depressions in the base floor to help you align them. Push the female ports all the way into the hole, through the back wall, until it stops. Use a small amount of hot glue to ensure the jacks stay in place.

<div align="center">
<img src="images/Pictures/usb.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

Place the USB coupler in the bottom opening in the base.  Use the slight depression in the base floor to help you align it.  Orient the coupler so that the USB-A port is facing out, and the USB-C port is facing inside the box.  Use a small of hot glue to ensure the coupler stays in place.

<div align="center">
<img src="images/Pictures/trsandusb.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

#### **I3. Mount The Circuit To The Enclosure Lid**

<div align="center">
<img src="images/Pictures/grommetstages.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>
Set one M2x4 heat insert into each of the 4 built-in standoffs on the lid.  Make sure the smooth lip of the grommet is facing down. 

<div align="center">
<img src="images/Pictures/grommetin.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>
Using the included soldering iron heat insert tip to gently press the inserts into the standoffs until the top of the insert is flush with the standoff.  Be sure that the insert remains vertical and does not go in at an angle.

<div align="center">
<img src="images/Pictures/buttonsfit.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

Place the circuit board face with buttons and display through the holes in the lid.  Everything should align perfectly.


<div align="center">
<img src="images/Pictures/terminals1.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>
Using 4 M4x4mm bolts, attach the circuit board to the standoffs that you outfitted with threaded inserts.  Tighten snug.

#### **I3. Attach Lid and Box Hinges**

<div align="center">
<img src="images/Pictures/hinge.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>


The hinges on the box were designed to be fastened using standard, 1.75mm filament as opposed to a metal hinge pin.

<div align="center">
<img src="images/Pictures/filamentinsert.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

Cut off a small section of filament that is just long enough to fit through each set of three hinge sections.  Cut the end at an angle, and gently, yet firmly, feed it all the way through all 3 sections.  Cut each end flush.  Repeat this for the other hinge as well.  

For a cleaner look, once the hinges are complete, push a few millimeters of the filament out of one end, cut it off, and then push the remainder back in, but push it in a millimeter or so.  this makes the edges look more uniform since there's no filament right up to the edge.

---

### **6.2: Connect Circuit Wiring To Control Box**

#### **J1. Create the Wires**

<div align="center">
<img src="images/Pictures/ribboncable.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

From the spool of 20 AWG 4-wire ribbon cable, cut 2 lengths of ribbon cable approximately 215mm each.  Crimp the ends using 22 AWG wire ferrules (with small gauge wire like this, going a size smaller on the ferrule helps secure the crimp better and prevent it from pulling off)



#### **J2. Connect The Ground Wires**

<div align="center">
<img src="images/Pictures/newgndterminal.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

Connect the 4 wires on one end of one of the sets of ribbon wire to the green, GND terminal block on the circuit board.

<div align="center">
<img src="images/Pictures/trswires.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

Connect the opposite end of that strip of wire to the GND terminal in each of the 4 TRS jacks in your control box.  There is no specific order for the the GND wires hear.  Any GND terminal on the circuit board can connect to any TRS GND terminal.

#### **J2. Connect The Signal Wires**

<div align="center">
<img src="images/Pictures/signalterminalnumbers.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>


<div align="center">
<img src="images/Pictures/newsignalterminal.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

Connect the 4 wires on one end of one of other set of ribbon wire to the blue, SIGNAL terminal block on the circuit board.  

<div align="center">
<img src="images/Pictures/trswires.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

Connect the other end to the TIP terminal of the respective TRS jack in the control box. 

Terminal pin 1 connects to the TIP terminal of TRS Jack 1.

Terminal pin 2connects to the TIP terminal of TRS Jack 2.

Terminal pin 3 connects to the TIP terminal of TRS Jack 3.

Terminal pin 4 connects to the TIP terminal of TRS Jack 4.

<div align="center">
<img src="images/Pictures/signalterminalnumbers.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>



#### **J2. Connect The XIAO Board To The Control Box USB Coupler**

Using the short USB-C to USB-C cable, connect the XIAO board to the USB-C coupler inside the control box.

------

This is how the final, wired product should look:

<div align="center">
<img src="images/Pictures/allwires.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

<div align="center">
<img src="images/Pictures/finalbox.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

<div align="center">
<img src="images/Pictures/closedbox.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>



------



### **6.3: Floor Stand And Control Box Mount**

#### **K1. Mount Drum to speaker stand**

Mount the angled speaker bracket to the speaker stand.

<div align="center">
<img src="images/Pictures/wirerouteold.png" width="350px" style="display: block; margin: 0 auto;">
</div>
#### **K1. Mount Control Box and Connect Signal Wires**

Attach the phone holder arm to the central post of the Speaker Stand.  Attach the control box to the Spring-Loaded phone bracket.  Adjust to an appropriate position, and connect the Drum Sensor TRS male ends that are hanging down on the drum to the matching TRS Female Jack on the Control Box.


---

#### **K2. Adjust height/angle**

Adjust height/angle for playing comfort.

---

#### **K3. Add drum cover**

Add your Drum cover, skin, towel, padding, or whichever you have. As my dimensions for the drum in this project are exactly the same as the Arcade Drum, I recommend locating an official Arcade drum skin. One source that seems to always have them in stock is [here](https://taiko.ac/products/rubber-drum-pad).

If that isn't an option for you, you can try using a towel, blanket, large mouse pad, thin foam, whatever you want that gives you sound-reducing qualities as well as the amount of bounce that you're looking for.

💡 **Pro Tip:** The beauty of this Project is that you can adjust the Drum Thresholds and make it perform just how you want regardless of Drum cover.

<div align="center">
<img src="images/Pictures/drumcover.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

---

## **7: Flash The Firmware**

<div align="right"><sub><a href="#table-of-contents">↑ Back to Top</a></sub></div>

Great job making it this far! Now it's time to prepare the board and flash the firmware. This is the quickest and easiest part of the entire build.

💾 [Download the two flash files in the .zip archive](https://ouchitaikoproject.github.io/OuchiTaikoProject/DownloadFiles/KillerQsOuchiTaikoProjectFlashFiles.zip)

---

### **Step 1: Enter Boot Mode**

Hold down the small **"B" button** on your XIAO RP2040 micro controller board while connecting it to your PC. The board will appear as a removable drive. The board is now in BOOT mode, and is waiting for you to load a file to it.

---

### **Step 2: Wipe the Board**

A good practice is to always flash a cleaning/nuke file to your board before you flash your actual firmware. This clears out any residual memory or cache that, while rare, may still be hanging around that would conflict with your desired firmware.

To do this, drag the `universal_flash_nuke.uf2` to the root of the removable drive. This wipes the board for a fresh start and automatically reboots it. The drive will reappear as before, in BOOT mode. Drag the `universal_flash_nuke.uf2` file one more time to be extra clean. Now, when the board pops up again as a removeable drive, you'll be ready to flash the controller firmware...

---

### **Step 3: Drag the Firmware File To The Board to Flash It**

Drag `KillerQsOuchiTaikoFirmware.uf2` to the removable drive. After a few seconds, the board will upload the file and automatically reboot. This time, the drive won't reappear—that's correct! Your board is now in controller mode and ready to play. If for some reason your board doesn't reboot automatically after dragging the firmware file, wait about 15 seconds, and then unplug the board and plug it back it. This will manually reboot it, and it's ready to go!

You're ready for calibration and testing!

---

## **8: Calibration & Settings**

<div align="right"><sub><a href="#table-of-contents">↑ Back to Top</a></sub></div>

### **Complete Button and Drum Pad Test (Before Calibration)**

💡 **Important:** By doing this step, you're just ensuring that they are all wired correctly, registering properly, and in the correct location.

**Test Procedure:**

1. Hold **SELECT** for 1 second to enter the system menu
2. The controller, by default, will be in **SWITCH TATACON mode**
3. Stay inside the Menu structure for this test (this prevents any false triggers from interrupting your button test)
4. Visit [Meloncolle Taiko Controller website](https://meloncolle.com/tatacon) and tap all four drum sensors and press all 14 navigation buttons
5. The button you press and the drum you tap on your controller should also light up the same drum or button on the screen

You can now exit the menu screen. If you notice any automatic/false triggering of the drum panels, you can ignore that as the below calibration will solve that temporary issue.

---

### **Taiko-Tune™ Auto-Calibration System (Recommended)**

**The world's first automated calibration system for Taiko drum controllers** - a revolutionary feature that eliminates the manual threshold guesswork.

#### **What is Taiko-Tune?**

Instead of manually adjusting thresholds through trial and error, Taiko-Tune analyzes your actual playing style and automatically calculates optimal sensitivity settings for each drum pad. This intelligent system:

- Monitors hits in real-time until progress bar reaches 100%
- Analyzes velocity patterns and strike consistency
- Detects cross-talk between adjacent pads and adjusts accordingly
- Automatically applies mathematically optimal threshold values
- Adapts to your unique playing style and drum cover choice

---

#### **How to Use Taiko-Tune:**

**Quick Start - Calibrate All 4 Pads Automatically:**

Launch calibration using either method:
- **Hold START** for 1 second (fastest - launches directly)
- **System Menu:** Hold SELECT → Settings → Drum Settings → Drum Thresholds → Auto Taiko-Tune → Analyze All 4 Drums → Start Analysis

---

**During the All 4 Drums Wizard:**

- The system will guide you through **2 complete passes** (8 calibrations total)
  - **Pass 1:** Left Ka → Left Don → Right Don → Right Ka
  - **Pass 2:** Right Ka → Right Don → Left Don → Left Ka (reverse order for optimal crosstalk compensation)
- For each drum:
  1. **Wait for the 3-second countdown** (samples ambient noise - do NOT touch the drum during countdown)
  2. **Hit the indicated pad naturally** until progress reaches 100%
     - Mix light taps and stronger hits like you would during actual gameplay
     - Don't baby it or go crazy - use your normal playing intensity
  3. **Results auto-apply** - the system calculates and saves optimal threshold instantly
  4. After a 3-second results display, it automatically moves to the next drum
- Between Pass 1 and Pass 2, a transition screen appears briefly
- **Total time:** Approximately 5-8 minutes for all 4 drums (both passes)

---

**Calibrate a Single Pad using Taiko-Tune:**

Use this method if you only want to recalibrate one specific drum without affecting the others.

1. **Enter the System Menu:** Hold **SELECT** for 1 second
2. **Navigate:**
   - Settings → Drum Settings → Drum Thresholds → Auto Taiko-Tune
3. **Choose your drum:**
   - Analyze Left Ka
   - Analyze Left Don
   - Analyze Right Don
   - Analyze Right Ka
4. **Select "Start Analysis"**
5. **Wait for the 3-second countdown** (samples ambient noise - do NOT touch the drum during countdown)
6. **Hit the indicated pad naturally** until progress reaches 100%
   - Mix light taps and stronger hits like you would during actual gameplay
   - Progress bar shows completion status
7. **Results auto-apply:** Optimal threshold is calculated and saved instantly

---

**Canceling Taiko-Tune:**

- Press **B** at any time during analysis to cancel
- Original thresholds are automatically restored
- Returns you to where you started (menu or idle)

---

#### **Why Use Taiko-Tune Instead of Manual Calibration?**

- **Faster:** 5-8 minutes for all pads vs. 30+ minutes of manual testing
- **More accurate:** Mathematical analysis vs. guesswork
- **Adapts to you:** Learns your specific playing dynamics
- **Eliminates crosstalk:** Two-pass system automatically detects and compensates for pad interference
- **Future-proof:** Recalibrate anytime if you change drum covers or playing style

---

### **Manual Single Pad Threshold Adjustment**

💡 **Note:** If you prefer automated calibration, use the **Taiko-Tune™ Auto-Calibration System** above - it's faster and more accurate.

Use this method when you want precise manual control over individual drum sensitivity values, or need to fine-tune after Taiko-Tune calibration.

---

#### **Step 1: Enter Manual Adjustment Mode**

1. **Enter the System Menu:** Hold **SELECT** for 1 second
2. **Navigate:**
   - Settings → Drum Settings → Drum Thresholds → **Single Pad Adjust**
3. **Choose your drum:**
   - Left Ka
   - Left Don
   - Right Don
   - Right Ka

---

#### **Step 2: Adjust Threshold Values**

Each pad has an adjustable threshold value (0-4095) that determines how hard you must hit before it registers.

- **D-PAD UP:** Increase threshold (less sensitive)
- **D-PAD DOWN:** Decrease threshold (more sensitive)
- **A Button:** Save and exit
- **B Button:** Cancel and restore original value

---

#### **Troubleshooting Guide:**

- **Light/regular hits don't register:** **LOWER** the threshold value
- **False hits happen too easily:** **RAISE** the threshold value
- **Hitting one drum triggers another drum (crosstalk):** **RAISE the threshold** of the drum that accidentally triggered by 10-20 points. Repeat until all drums function independently.

---

#### **Threshold Value Guidelines:**

- **Don (Center) Pads:** Typically 40-80
- **Ka (Rim) Pads:** Typically 60-100
- Values depend heavily on your drum construction, covers, and playing style

---

#### **When to Recalibrate:**

- After changing drum covers or padding
- If sensitivity changes over time
- When switching between soft and aggressive playing styles
- After any hardware modifications

💡 **Pro Tip:** You can mix Taiko-Tune with manual adjustments - let Taiko-Tune set the baseline, then fine-tune individual pads ±10-20 points if desired.

---

### **Extended Drum Setting Features**

#### **Big Hit Arcade Mode**

This controls how the drum reacts to very strong strikes.

In the Arcade, to get full points for Big Notes, you have to hit the any Don or Ka *much harder* than your regular note hits. Because most existing home Taiko drums didn't have the technology to detect when a hit was harder than other hits, home Console ports and PC versions of Taiko games changed the method up a bit, and would give full points for Big Notes by simply hitting both Don or Ka at the same time - instead of just hitting a single Don or Ka harder.

My **Big Hit Arcade Mode** solves that, and allows for that "Arcade Hit" where you can simply hit the Don or Ka of your choice *harder* than typical hits to get credit for the Big Notes - Just like how Official Taiko Arcade Machines operate.

**Settings:**

- Set Mode to: **ON**
- Adjust Threshold Level: Start around **850** (Big Hit threshold is typically 2-3x normal threshold)
- **Increase** the value to lower the threshold of what counts as a Big Hit
- **Decrease** the value to make it so that you have to hit much harder for the Big Note to register

---

#### **SimulTap Mode**

By Default, the firmware makes it so that you cannot hit a Don and Ka at exactly the same time (since that situation doesn't exist in gameplay, and helps ensure clean hits). If you wanted to use this controller to play your other favorite 4-lane rhythm games, this would be an issue. So, I added this mode that lets you toggle whether there are hit limitations.

**Settings:**

- Turn **SimulTap ON** if you want to use the Drum in non-Taiko Rhythm Games (since you'll want the ability to hit more than one drum/note at the same time)
- Leave it **OFF** when playing Taiko Games

---

#### **Hold Time**

This defines how long a hit is "held" before the drum accepts another.

**Settings:**

- Recommended range: **25—35ms**
- **Increase** if double hits register too fast
- **Decrease** if hits feel laggy or delayed
- In most cases, this is not needed — some say a level of at least 25ms is needed when playing on the Nintendo Switch

---

### **OLED Display Featured While Playing**

- Streak counter resets after 1 second of inactivity
- Main Screen Character animations confirm every registered hit
- Each character represents its drum position (left to right)

---

### **Pro Tips**

- Different drum covers may need threshold adjustment
- Use Debug mode to see raw ADC values (should be 200-400 baseline)

There you have it - You've just built a professional controller circuit with adaptive baseline technology that automatically maintains perfect calibration—no manual adjustments needed as components wear or age!

**You're Ready to Play!**

---

## **9: Files & Downloads**

<div align="right"><sub><a href="#table-of-contents">↑ Back to Top</a></sub></div>

<div align="center">
<img src="images/Pictures/downloadimage.png" width="350px" style="display: block; margin: 0 auto;">
</div>

💾 Here are the files referenced throughout the guide for quick reference:

- Download all SVG and STL Files (see Section 9: Files & Downloads)

- [Download PDF file of the Circuit Schematic](https://ouchitaikoproject.github.io/OuchiTaikoProject/DownloadFiles/KillerQsOuchiTaikoProjectCircuitSchematic.pdf)

- [Download the Firmware File + Nuke File](https://ouchitaikoproject.github.io/OuchiTaikoProject/DownloadFiles/KillerQsOuchiTaikoProjectFlashFiles.zip)

---

## **10: Basic Troubleshooting**

<div align="right"><sub><a href="#table-of-contents">↑ Back to Top</a></sub></div>

**🖥️ DISPLAY Issues**

- **Blank OLED:** Check I2C (GPIO 6/7), verify 3.3V power
- **Frozen:** Unplug USB cable and reconnect
- **Menu won't open:** Hold SELECT for 1 second
- **No animation:** Check if sensors register (LED flash)

**🥁 SENSOR Issues**

- **No response:** Check diode polarity, TRS wiring
- **False triggers:** Increase thresholds
- **Missed hits:** Decrease thresholds, check mounting
- **Crosstalk:** Increase threshold on triggering pad

**🎮 MODE SWITCHING Issues**

- **Freezes:** Unplug/reconnect USB-C
- **No change:** Press EAST to confirm

💡 **Tip:** If you have trouble with a particular version of a Taiko game, note your current controller emulation mode and research what others have done to get that controller series working with your particular game version.

---

## **11: Menu System Reference**

<div align="right"><sub><a href="#table-of-contents">↑ Back to Top</a></sub></div>

### **Quick Access**

- **Hold SELECT (1 sec):** Enter system menu
- **Hold START (1 sec):** Launch Taiko-Tune (calibrate all 4 drums)

---

### **Navigation Controls**

| Button         | Action                                                 |
| -------------- | ------------------------------------------------------ |
| **LEFT/RIGHT** | Navigate menu items / Toggle ON-OFF / Cycle selections |
| **UP/DOWN**    | Adjust values (hold to fast-repeat)                    |
| **EAST (A)**   | Confirm selection / Save changes                       |
| **SOUTH (B)**  | Cancel / Go back / Restore original value              |

---

### **Complete Menu Structure**

```
SYSTEM MENU
│
├── 🎮 Controller Modes
│   ├── Nintendo Switch Tatacon Drum
│   ├── Nintendo Switch Pro Controller
│   ├── Sony PS3 Dualshock3
│   ├── Sony PS4 Tatacon Drum
│   ├── Sony PS4 Dualshock4
│   ├── Keyboard Player 1
│   ├── Keyboard Player 2
│   ├── Microsoft Xbox Xbox360
│   ├── Android (XInput)
│   ├── iOS (XInput)
│   ├── Analog Player 1
│   ├── Analog Player 2
│   ├── MIDI Controller
│   └── Debug Mode
│   [LEFT/RIGHT to select, A to confirm & reboot]
│
├── 🥁 Drum Settings
│   │
│   ├── Drum Thresholds
│   │   │
│   │   ├── Auto Taiko-Tune ⚡
│   │   │   ├── Analyze All 4 Drums
│   │   │   ├── Analyze Left Ka
│   │   │   ├── Analyze Left Don
│   │   │   ├── Analyze Right Don
│   │   │   └── Analyze Right Ka
│   │   │   [LEFT/RIGHT to select, A to start]
│   │   │
│   │   ├── Manual Pad Adjust
│   │   │   ├── Left Ka (0-4095)
│   │   │   ├── Left Don (0-4095)
│   │   │   ├── Right Don (0-4095)
│   │   │   └── Right Ka (0-4095)
│   │   │   [UP/DOWN to adjust, A to save, B to cancel]
│   │   │
│   │   └── Reset Thresholds
│   │       └── Yes/No [LEFT/RIGHT, A to confirm]
│   │
│   ├── Big Hit Mode
│   │   ├── Off
│   │   ├── Light (2000)
│   │   ├── Medium (2500)
│   │   ├── Heavy (3000)
│   │   └── Custom...
│   │       └── Threshold (0-4095) [UP/DOWN adjust]
│   ├── SimulTap Mode
│   │   └── ON/OFF [LEFT/RIGHT toggle]
│   │
│   └── Hold Time
│       └── Adjust (0-255 ms) [UP/DOWN adjust]
│
├── 💡 LED Settings
│   ├── Brightness (0-255) [UP/DOWN adjust]
│   └── Player Color (PS4)
│       └── ON/OFF [LEFT/RIGHT toggle]
│
├── ℹ️ About
│   ├── OuchiTaiko Project by KillerQ
│   ├── Full Guide & Info: ouchitaiko.com
│   ├── Firmware v1.0 October 2025
│   ├── Based on: DonCon2040 (MIT)
│   └── & HIDtaiko (Apache 2.0)
│   [LEFT/RIGHT to scroll info]
│
├── 🔄 Reset
│   └── Yes/No [LEFT/RIGHT, A to confirm]
│
└── 💾 USB Flash Mode
    └── Yes/No [LEFT/RIGHT, A to confirm]
```

---

### **Idle Screen Display**

When not in menu, the OLED shows:

- **Top:** Current controller mode
- **Center:** Live streak counter (resets after 1 sec idle)
- **Bottom:** 4 animated feedback spheres (Left Ka, Left Don, Right Don, Right Ka)

---

## **12: About**

<div align="right"><sub><a href="#table-of-contents">↑ Back to Top</a></sub></div>

This project is a one-of-a-kind Hybrid creation that pulls from several amazing resources. As they say, "We stand on the shoulders of Giants…"

The core firmware version as well the navigation and OSD hardware portion of the Circuit was adapted from the amazing work by 'ravinrabbid' who created the **[DonCon 2040](https://github.com/ravinrabbid/DonCon2040)** Project. You can visit there for more of the nitty-gritty details of the software.

The inspiration for the circuit came from the great, amazingly straightforward circuitry work by 'kasasiki3' who created the **[HIDtaiko](https://github.com/kasasiki3/HIDtaiko/tree/master)** Project (2040 edition).

Without these two projects, none of this would have been possible.

Credit to 'Gadgetoid' on GitHub for his [pico-universal-flash-nuke](https://github.com/Gadgetoid/pico-universal-flash-nuke) file that helps clean things up between flashes.

Credit also goes to the creative minds over at [Boxes.py](https://boxes.hackerspace-bamberg.de/) as well for creating the ultimate box-creating resource. It offers endless ideas and guides on how to make enclosure boxes for pretty much anything - especially projects like this.

I want to thank Discord user 'Allspice.' I discovered a random message from him in my inbox one day asking if he could help test my Taiko Project when I was finished creating it. He said he had only basic soldering skills, yet he was able to successfully build the entire circuit from scratch as well as the drum! Even more importantly, he helped shape this how-to guide that you are reading right now.

I also want to take a moment to mention a few, more general resources that were invaluable for me during this entire process. These are great if you ever want to go down the rabbit hole of the world of Taiko no Tatsujin modding or Custom Controller creation - with these assets at your disposal, you have the power to create anything imaginable:

- [Taiko no Tatsujin Modding! Discord Channel](https://discord.com/invite/HFm37aA5zr)
- [Cons&Stuff :) Discord Channel](https://discord.com/invite/P4CpVHrR?utm_source=Discord%20Widget&utm_medium=Connect)
- [OpenStick Community - GP2040-ce Project Discord Channel](https://discord.com/invite/openstickcommunity-1049366310389289001)

### **Closing Thoughts**

I can't thank you enough for taking the time to follow along with my guide. It really means a lot.

If you have any project-specific questions or suggestions, please use the Discussion panel on my GitHub. You can also ask more general questions in any of the Discord Channels I mentioned above - they are all very helpful in their own way.

#### **Spread The Word**

Please share your results and excitement as well as this guide with your favorite DIY gaming communities. Tag me (KillerQ97) when you do!

Enjoy, Have Fun, and Peace Out!

www.ouchitaiko.com

---

## **13: Copyright Information**

<div align="right"><sub><a href="#table-of-contents">↑ Back to Top</a></sub></div>

### Copyright & License Transparency

As mentioned above, this project builds upon the outstanding work of the open-source community and strictly adheres to all licensing requirements. **OuchiTaiko** is primarily based on [ravinrabbid's DonCon2040](https://github.com/ravinrabbid/DonCon2040), which is licensed under the **MIT License**, and also incorporates elements from [kasasiki3's HIDtaiko](https://github.com/kasasiki3/HIDtaiko), licensed under the **Apache License, Version 2.0**.

All custom features, modifications, and improvements documented in this project (including KillerQ's creation of Big Notes detection, SimulTap mode, PS4 always-on, adaptive baseline tracking, critical mode-switching fixes, animated display system, enhanced menu navigation, and any other custom creations or modifications to include code, physical circuitry, Drum, or Drum-related design features and components) are transparently documented and shared under the same open-source spirit. I believe in giving credit where credit is due and maintaining the collaborative nature of the maker community. If you build upon this work, please honor these licenses by including proper attribution and license notices in your derivative projects as well as mentioning and/or tagging me, 'KillerQ.'

---

### Attribution Chain

- **Original DonCon2040 firmware:** © ravinrabbid (MIT License)
- **HIDtaiko components:** © kasasiki3 (Apache License 2.0)
- **Custom modifications and features:** Created by, and documented in this repository by KillerQ (Dual-licensed under MIT and Apache 2.0)

---

### Legal Compliance Notice

Any distribution of this firmware (binary or source) must include:

1. A copy of the MIT License from DonCon2040
2. A copy of the Apache License 2.0 from HIDtaiko
3. Copyright notices from both original projects
4. Attribution to all contributors

The complete license texts are provided below for full transparency and legal compliance.

---

## Complete License Documentation

### 1. License for KillerQ's Original Contributions

The modifications and original code contributed to this repository by **KillerQ** are **dual-licensed** under the **MIT License** and the **Apache License, Version 2.0**.

**Copyright (c) 2025 KillerQ**

---

### 2. External Component: DonCon2040 Firmware

**Applies to portions of OuchiTaiko firmware derived from DonCon2040.**

**Original Author:** ravinrabbid  
**License:** MIT License  
**Copyright (c) 2021 ravinrabbid**

#### Full Text: MIT License

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

### 3. External Component: HIDtaiko Components

**Applies to HID components and related code derived from HIDtaiko.**

**Original Author:** kasasiki3  
**License:** Apache License, Version 2.0  
**Copyright 2022 kasasiki3**

#### Full Text: Apache License, Version 2.0

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

APPENDIX: How to apply the Apache License to your work.

      To apply the Apache License to your work, attach the following
      boilerplate notice, with the fields enclosed by brackets "[]"
      replaced with your own identifying information. (Don't include
      the brackets!) The text should be enclosed in the appropriate
      comment syntax for the file format. We also recommend that a
      file or class name and description of purpose be included on the
      same "printed page" as the copyright notice for easier
      identification within third-party archives.

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

This project is fully transparent about its licensing and gives proper credit to all contributors. By including the complete license texts above, this README serves as the authoritative legal documentation for the OuchiTaiko Project.

Thank you to ravinrabbid and kasasiki3 for their incredible open-source contributions that made this project possible.

**KillerQ**

www.ouchitaiko.com

---

*OuchiTaiko Project | Smarter. Simpler. Better.*

