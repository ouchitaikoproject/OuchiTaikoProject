<div align="center">
<img src="images/Pictures/mainlogo.jpg" width="100%" style="max-width: 1200px; display: block; margin: 0 auto;">
</div>

---

<div align="center">
*In Japanese, 'ouchi' (おうち) means 'home' and 'taiko' (太鼓) means 'drum.' Together, 'OuchiTaiko' represents the joy of bringing the authentic Taiko experience from the arcade into your own space.*
</div>


---

# **Table of Contents**
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
- [11: Menu Tree Structure/Quick Reference](#11-menu-system-reference)
- [12: About](#12-about)
- [13: Copyright Information](#13-copyright-information)

---

# **1: Project Overview**

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
<img src="images/Pictures/newcompletebackangle.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

<div align="center">
<img src="images/Pictures/newcompletebackclose.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>



---

Hi, I'm KillerQ, the Creator of the **OuchiTaiko Project** - an open-source build guide for a professional arcade-grade Taiko drum controller unlike anything currently available.

This guide represents 8 months of research and development, addressing the limited availability and high cost of commercial units. Using **Adaptive Baseline Software Intelligence (ABSI)**, this design achieves arcade-level performance through intelligent software instead of complex analog circuits or custom PCBs.

All you need is basic soldering and woodworking skills. The components linked in this guide work together as tested, though compatible alternatives are welcome.

Don't be worried about the length of this guide - it was purposely created that way to guide the user through every single step of the process - leaving nothing to question.  Less thinking, more doing!

---

# **Key Features**

## **World-First Innovations** 

**The following three innovations were designed and created just for this project and, as of this writing, do not exist at all in any other Taiko Drum Controllers - DIY, or Professionally manufactured.  You're among the first to enjoy their benefits!**


**The Problem:** Traditional Taiko drum controllers require manual threshold adjustments through trial-and-error, often involving recompiling firmware or endless menu tweaking. Users struggle to find the perfect balance between sensitivity (missing hits) and false triggers (ghost hits from vibrations).

**The OuchiTaiko Solution is three-fold:**

1) Auto Calibration
2) Adaptive Baseline Analysis
3) Custom Sensor Housing


### **1) Auto Calibrate - Intelligent Threshold Detection**
The **Auto Calibrate** system intelligently analyzes your actual playing style and automatically calculates optimal sensitivity settings:



✅ **Eliminates manual tweaking** - no more trial-and-error  
✅ **Prevents false triggers** - intelligent 50-point safety margins  
✅ **Adapts to you** - learns your unique playing dynamics  
✅ **Future-proof** - recalibrate anytime conditions change

### **2) Adaptive Baseline Software Intelligence (ABSI)**

Achieves superior performance through elegant software:

- **Automatic sensitivity adjustment** based on environmental noise
- **Dynamic baseline tracking** adapts to temperature, humidity, and mounting changes
- **Zero calibration drift** over time

### **3) Custom Arcade Sensor Suspension**

Drum trigger sensors use **custom-designed suspension mounting** that improves upon the ones found in the actual Japanese Arcade machines.

Proper sensor suspension ensures:
- Consistent trigger response
- Elimination of mechanical crosstalk
- Accurate hit detection
- Long-term durability under heavy play

---

## **🎮 Complete Hardware & Features**

### **Standalone Testing & Display**

**No PC required** for setup, calibration, or troubleshooting:
- **OLED display** with real-time hit feedback and animated hit icons
- **Visual confirmation** of all 14 navigation buttons
- **Live stats:** Drum hit animations, controller mode, button press indicator, menu hints
- **Instant verification** that everything is wired correctly

Calibrate and test your entire system with just USB power--see exactly what's happening before you ever plug into a console or PC.

### **Professional Hardware**

- **Custom Sensor Housing and Optimized Software:** Enhanced mechanical and electronic false-trigger isolation
- **14 Game Navigation Buttons:** Full in-game navigation regardless of game version
- **Professional Mounting:** Optimal hardware stabilization via adjustable base and, angled drum face mount

### **14 Input Modes for Maximum Compatibility**

- Nintendo Switch Tatacon (HORI NSW-079 Taiko Drum)
- Nintendo Switch Pro Controller
- Sony PS3 Dualshock3 (PC/Steam only)
- Sony PS4 Tatacon Drum (HORI PS4-095 Taiko Drum) (PC/Steam only)
- Sony PS4 Dualshock4 (PC/Steam only)
- Keyboard Player 1
- Keyboard Player 2
- Microsoft Xbox Xbox360 (XInput)
- Android (XInput)
- iOS (XInput)
- Analog Player 1 (XInput)(Compatible with TaikoArcadeLoader)
- Analog Player 2 (XInput)(Compatible with TaikoArcadeLoader)
- MIDI Controller
- Debug Mode

### **Zero Coding or Programming Required**

- **Drag-and-drop firmware** for instant setup
- **On-screen menus** for all settings and calibration
- **Automatic updates** without recompiling
- **No technical knowledge needed**--if you can follow instructions and use a soldering iron, you can build this

---


### 🎥 Demo Videos

- [Gameplay](https://youtu.be/p4eFeo_LB5I?si=jDKb93B7uYx1qAux)

---

# **2: Parts List for Electronics**

<div align="right"><sub><a href="#table-of-contents">-Back to Top-</a></sub></div>

The Amazon links reference parts that are 100% compatible when used together. You can source these items from any supplier as long as they meet the specifications.

**Important:** "Quantity" refers to the number of individual items you need, NOT the number of packages you need to order since some items come with multiple items in one package.

<div style="overflow-x: auto;">

| #  | Item                                                          | Qty | Product Link                   |
| -- | ------------------------------------------------------------- | --- | ------------------------------ |
| 1  | XIAO RP2040 board (Seeed Studio)                              | 1   | [Link](https://www.amazon.com/dp/B0DRNTQ338) |
| 2  | GODIYMODULES MCP23017 I2C expander board                      | 1   | [Link](https://www.amazon.com/dp/B0DSLPRKKZ) |
| 3  | 1N4148 diodes                                                 | 4   | [Link](https://www.amazon.com/dp/B0DN62QFYS) |
| 4  | 0.1uF/100nF ceramic capacitors                                | 4   | [Link](https://www.amazon.com/dp/B08B3VCK42) |
| 5  | 3.5mm TRS female jacks w/ screw terminals                     | 4   | [Link](https://www.amazon.com/dp/B017CBTLJK) |
| 6  | 3.5mm TRS male plugs w/ standard ends                         | 4   | [Link](https://www.amazon.com/dp/B07Y8JGFS1) |
| 7  | 27mm piezo sensors                                            | 4   | [Link](https://www.amazon.com/dp/B07RK2TQ8D) |
| 8  | 128x64 mono OLED display (I2C)                                | 1   | [Link](https://www.amazon.com/dp/B09T6SJBV5) |
| 9  | 6mm x 6mm x 8mm tactile switches with button caps                              | 14  | [Link](https://www.amazon.com/dp/B0827LX3FV) |
| 10 | 22 AWG Siamese wire                                           | 1   | [Link](https://www.amazon.com/dp/B0CN76L8G3) |
| 11 | USB-C to USB-A female coupler                                 | 1   | [Link](https://www.amazon.com/dp/B0CY314DCJ) |
| 12 | Short USB-C to USB-A extension cable                          | 1   | [Link](https://www.amazon.com/dp/B0DPF8TFC9) |
| 13 | 6 inch, coiled 3.5mm TRS audio extension cable (male to male) | 4   | [Link](https://www.amazon.com/dp/B0D7CXJ5LQ) |
| 14 | 3.5mm TRS barrel coupler                                      | 4   | [Link](https://www.amazon.com/dp/B0BVDQ5G9X) |
| 15 | 4-pin, 0.2 inch pitch screw terminal blocks                   | 2   | [Link](https://www.amazon.com/dp/B098QMNHPJ) |
| 16 | 22 AWG 4-wire ribbon connection wire                          | 1   | [Link](https://www.amazon.com/dp/B09X47XBFS) |
| 17 | ElectroCookie circuit board (88.9mm x 96.5mm)                 | 1   | [Link](https://www.amazon.com/dp/B07YSCGBL7) |
| 18 | 6ft USB-A to USB-C cable                                     | 1   | [Link](https://www.amazon.com/dp/B0CKXZML11) |
| 19 | M2 x 3 x 3 threaded inserts                                   | 4   | [Link](https://www.amazon.com/dp/B0FD7DQS8Y) |
| 20 | M2 x 4 bolts                                                  | 4   | [Link](https://www.amazon.com/dp/B0D3X4LJD2) |
| 21 | 22 AWG wire ferrules and crimper                              | 8   | [Link](https://www.amazon.com/dp/B0DRJ9CDNG) |

</div>

---

# **3: Parts List for Hardware**

<div align="right"><sub><a href="#table-of-contents">-Back to Top-</a></sub></div>

## **Required Tools and Supplies**

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

## **Hardware Parts List**

<div style="overflow-x: auto;">

| #    | Item                                                         | Qty  | Product Link                                                 |
| ---- | ------------------------------------------------------------ | ---- | ------------------------------------------------------------ |
| 1    | 1/4" (or 6mm) 2ft x 4ft plank of cabinet-grade MDF .            | 2    | [Home Depot](https://www.homedepot.com/p/1-4-in-x-2-ft-x-4-ft-Medium-Density-Fiberboard-1508104/202089069) |
| 2    | 1/4" x 1-1/2" OD stainless steel fender washers              | 46   | [Link](https://www.amazon.com/dp/B0D8ZRL1CP)                               |
| 3    | Strong wood glue                                             | 1    | [Link](https://www.amazon.com/dp/B0002YQ378)                               |
| 4    | M3 x 8mm bolts                                               | 8    | [Link](https://www.amazon.com/dp/B07CMQ1SQH)                               |
| 5    | M3 x 5mm threaded inserts                                    | 8    | [Link](https://www.amazon.com/dp/B0CLKCQ2SH)                               |
| 6    | M6 x 10mm wood threaded inserts                              | 14   | [Link](https://www.amazon.com/dp/B0CNLRFFH1)                               |
| 7    | M6 x 35mm nylon bolts                                        | 14   | [Link](https://www.amazon.com/dp/B07L9ZS21T)                               |
| 8    | M6 threaded 20mm x 15mm rubber isolators (one side M6 female, other side with M6 x 18mm bolt) | 14   | [Link](https://www.amazon.com/dp/B0BKPHT6Y9)                               |
| 9    | 3D printer filament (PLA)                                    | 1    | [Link](https://www.amazon.com/dp/B081S5N5PC)                               |
| 10   | Gel superglue                                                | 1    | [Link](https://www.amazon.com/dp/B0006HUJCQ)                               |
| 11   | Loctite thread adhesive - medium                             | 1    | [Link](https://www.amazon.com/dp/B000FIXQXK)                               |
| 12   | 2.2mm thick scuba knit neoprene fabric (only 4" x 4" total needed) | 1    | [Link](https://www.amazon.com/dp/B0DK1B5LZ7)                               |
| 13   | Finger knobs with pass-through M6 threads                    | 18   | [Link](https://www.amazon.com/dp/B07RW9ZH4H)                               |
| 14   | Mini PA speaker stand (optional, or similar that connects to the drum mount below) | 1    | [Link](https://www.amazon.com/dp/B094N9YG72)                               |
| 15   | Adjustable angle speaker bracket (optional, or similar that connects to the drum stand above) | 1    | [Link](https://www.amazon.com/dp/B0DR8NCZP6)                               |
| 16   | Rubber Taiko drum cover (optional, but crucial for arcade feel. Any thin sound-dampening material may suffice) | 1    | [Link](https://taiko.ac/products/rubber-drum-pad)            |
| 17   | Spring-loaded phone holder with gooseneck arm (optional, or similar)                | 1    | [Link](https://www.amazon.com/dp/B0F32MLBZX)
| 18   | 1/4" Roundover Router Bit                | 1    | [Link](https://www.amazon.com/dp/B0C5DVBNLS)                                 |
| 19   | Clear Spray Glaze/Lacquer                | 1    | [Link](https://www.amazon.com/dp/B00D0293SA)
| 20   | Deburring Tool (optional, can use large drill bit instead)                | 1    | [Link](https://www.amazon.com/dp/B07RM1D6WD)
</div>

---

# **4: Build the Circuit**

<div align="right"><sub><a href="#table-of-contents">-Back to Top-</a></sub></div>

<div align="center">
<img src="images/Pictures/schematic.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

📸 **Photo Documentation Note:**
Some assembly photos show different terminal headers than what's in your parts list, and some signal wires may be routed slightly differently. Don't worry about these mismatches - the installation process is identical. You'll see the correct terminals used when you reach Step 8.

## **Circuit Overview**

This build uses a **single circuit board** design with everything mounted on one ElectroCookie double-column protoboard:

- **XIAO RP2040:** Brain of the system, handles drum sensors
- **MCP23017:** Controls OLED display and navigation buttons
- **OLED Display:** Status and menu display
- **14 Navigation Buttons:** Gamepad controls

## **Understanding Shared Rows**

The ElectroCookie protoboard has **shared rows** where multiple holes are electrically connected. This allows you to:

- Solder components into any hole in that row
- Connect wires between shared row holes without additional soldering
- Keep the build clean and organized

**When this guide specifies a hole location** (e.g., "Row D, hole 5"), you **must** use that exact hole. **When it says "any hole in the shared row,"** you have flexibility to choose the most convenient location in that shared/common row.

---

## **IMPORTANT: Board Orientation & Labeling**

This protoboard has **row letters and numbers printed on both sides**.

- **FRONT face** = Display and buttons side (use for Steps 1-2)
- **BACK face** = XIAO, MCP23017, and screw terminals side (use for Steps 3-12)

**⚠️ CRITICAL - Manufacturing Variance:**

Due to manufacturing variations within ElectroCookie production batches, **some boards MAY have reversed row labeling between front and back sides**. On affected boards, what appears as "Row A" on the FRONT may appear as "Row J" on the BACK (and vice versa).

**Before you begin:** Check your specific board's labeling on both sides. When this guide specifies a hole location (e.g., "Row D, hole 5"), **always use the row letter/number printed on the side you're currently working on**, not the opposite side.

**Example:** If the guide says to place a component in "Row E, hole 12" on the BACK face:
1. Flip to the BACK face
2. Locate the "Row E" label printed on the BACK (ignore what the front says)
3. Count to hole 12 in that row
4. Place your component

Always verify which side you're working on before placing components.

(This is not a problem, or a big deal, just be aware and make sure you don't blindly follow a step if you board is clearly labeled differently)

---

## **Step 1: Mount OLED Display (FRONT face)**

Mount the OLED display on the **top center** of the FRONT face.

**OLED Placement:**

- **Location:** Row J, holes 14-17 (FRONT face coordinates)
- **Orientation:** Screen facing up
- Solder in place

<div align="center">
<img src="images/Pictures/oledmount.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

**OLED Pin Locations** (Solder now, but these will be wired in Step 7):

- VCC (Row A, hole 15)
- GND (Row A, hole 14)
- SCL (Row A, hole 16)
- SDA (Row A, hole 17)

<div align="center">
<img src="images/Pictures/oledback.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

---

## **Step 2: Mount Buttons (FRONT face)**

**Button Orientation:**

- Orient the buttons so that the 2 legs are horizontal on the **left and right side** (NOT top and bottom)
- For visual consistency: align all button tabs the same way (your buttons may have a top or bottom edge that looks slightly different - it is best to keep them all facing the same way for visual mounting consistency)

**CRITICAL - Use Exact Positions:**  
The table below specifies exact FRONT face hole positions. These align with the Control Box openings to ensure proper fit of the controller faceplate.

<div align="center">
<img src="images/Pictures/buttonsall.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

Here are the exact hole locations for you to place your buttons in:

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

1. Insert each button into its specified position, bend legs behind board to hold in place
2. Solder both legs in place
3. Verify button alignment matches reference photos

<div align="center">
<img src="images/Pictures/buttonback.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

**CRITICAL NOTE - Prevent Signal Shorts:**  
Because of the shared row nature of the board, 2 buttons  will initially share signal rows with two other buttons—this would short without the following quick fix:


- The **NORTH Button and SOUTH button** legs will initially be sharing a row with each other

- The **UP Button and DOWN button** legs will also initially be sharing a row with each other

You will need to make a "break" in the shared row between these buttons so that the legs aren't electrically connected.

<div align="center">
<img src="images/Pictures/drill1.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

<div align="center">
<img src="images/Pictures/drill2.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

**Quick Fix (30 seconds per pair):**

1. Find the hole centered between the two signal legs of the two buttons
2. Remove/Scrape away copper with 3mm drill bit or knife
3. Test with multimeter: touch both signal legs - NO beep = success. If you hear a beep, remove more copper metal and try again
4. Repeat for second button pair


---

## **Step 3: Mount XIAO RP2040 (BACK face)**

**Flip board to BACK face.** 

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

Here is a photo showing an example of the un-jumped power rails and then two photos (from and back) showing them as jumped.  This is done with a blob of solder, or a small jumper wire that is also soldered.  There are 4 total places to jump on the board used.

<div align="center">
<img src="images/Pictures/jumpedrails3.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>


<div align="center">
<img src="images/Pictures/jumpedrails.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>


<div align="center">
<img src="images/Pictures/jumpedrails2.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

---

## **Step 4: Install Drum Sensor Protection Circuits (BACK face)**

Each of the 4 drum sensors requires identical protection circuitry.

**Diode & Capacitor Placement:**

| Drum Zone | XIAO Pin | Diode Anode + Capacitor Location (BACK face) |
| --------- | -------- | -------------------------------------------- |
| Left Ka   | A0       | Row J, hole 7                                |
| Left Don  | A1       | Row J, hole 6                                |
| Right Don | A2       | Row J, hole 5                                |
| Right Ka  | A3       | Row J, hole 4                                |

**For Each Sensor:**

1. **Diode:** Place anode (non-striped end) of 1N4148 diode into the specific hole listed above (do not solder yet)

2. Place cathode (striped end) into GND rail hole directly below (do not solder yet)
   
   <div align="center">
   <img src="images/Pictures/diodes.jpg" width="350px" style="display: block; margin: 0 auto;">
   </div>
   
   <div align="center">
   <img src="images/Pictures/protectionbelow.jpg" width="350px" style="display: block; margin: 0 auto;">
   </div>

3. **Capacitor:** Place either leg of 0.1uF capacitor into same hole as diode anode (do not solder yet)

4. Place other leg into same GND rail hole as the diode cathode (do not solder yet)
   
   <div align="center">
   <img src="images/Pictures/doubleprotection.jpg" width="350px" style="display: block; margin: 0 auto;">
   </div>
   
   <div align="center">
   <img src="images/Pictures/protectionbelow2.jpg" width="350px" style="display: block; margin: 0 auto;">
   </div>

5. **Solder:** NOW, you can solder the combined diode + capacitor legs together into their respective hole. This saves space and keeps things looking clean.
   
   <div align="center">
   <img src="images/Pictures/protectionbeneath.jpg" width="350px" style="display: block; margin: 0 auto;">
   </div>

---

## **Step 5: Connect Button Grounds (BACK face)**

**Flip board as needed** - button legs are now accessible on BACK.

- Connect one leg of each button (ground leg) to GND rail
- **Recommended:** Daisy-chain grounds for cleaner routing - see reference photo for layout. Conversely, on the right set of buttons, designate the GND to all of the legs on the right side of the button (toward the left edge of the board) as ground legs.

<div align="center">
<img src="images/Pictures/buttongrounds.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

---

## **Step 6: Prepare MCP23017 Board**

**Solder Header Pins:**

1. Solder the **single row of 10 header pins** into the bottom row of holes on the MCP23017
2. This row starts with **A2** (left) and ends with **VCC** (right)
3. The double row of pins on top remains empty (for button signal connections later)

<div align="center">
<img src="images/Pictures/expandersolder.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

---

## **Step 7: Pre-Wire I2C Lines Under MCP (BACK face)**

Before soldering/mounting the MCP23017 board onto the circuit board, pre-wire the power and I2C connections underneath where it will sit.

**Temporarily place** MCP23017 centered horizontally (Row E, holes 11-21, BACK face, double pins facing up). Note where it sits, then remove it.

<div align="center">
<img src="images/Pictures/expanderterminals.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

**Make these 4 connections underneath:**

| Connection | From (Under MCP) | To (OLED)      | Purpose              |
| ---------- | ---------------- | -------------- | -------------------- |
| VCC        | Row A, hole 11   | Row A, hole 15 | Power to display     |
| GND        | Row A, hole 12   | Row A, hole 14 | Ground to display    |
| SCL        | Row A, hole 13   | Row A, hole 16 | I2C clock to display |
| SDA        | Row A, hole 14   | Row A, hole 17 | I2C data to display  |

<div align="center">
<img src="images/Pictures/sdasclunderexpander.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

(Photo shows SDA and SCL connection progress. You will also connect VCC and GND to the display in this step)

**Now mount MCP23017:**

- Place bottom single row of 10 pins into Row E, holes 11-21 (BACK face)
- Verify **VCC pin** is in Row E, hole 11
- Verify **A2 pin** is in Row E, hole 21
- Solder these header pins to the circuit board

<div align="center">
<img src="images/Pictures/expanderterminalsfirst.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

(In the above photo the VCC and GND were routed different than the guide, but work the same)

---

## **Step 8: Connect Power & Ground**

Connect/verify power and ground between XIAO, MCP23017, and display.

**Connections:**

1. **XIAO 3.3V** -> **MCP23017 VCC** -> **OLED VCC** (OLED already connected via Step 7)
2. **XIAO GND** -> **MCP23017 GND** -> **OLED GND** (OLED already connected via Step 7)
3. Verify all three components (XIAO, MCP, display) share common ground

---

## **Step 9: Connect I2C Data Lines**

<div align="center">
<img src="images/Pictures/powercomms.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

Connect I2C between XIAO and MCP23017.

| Connection | From XIAO                 | To MCP23017               |
| ---------- | ------------------------- | ------------------------- |
| SDA        | Any pin in SDA shared row | Any pin in SDA shared row |
| SCL        | Any pin in SCL shared row | Any pin in SCL shared row |

**Verify I2C connections:**

- XIAO SDA -> MCP23017 SDA -> Display SDA (all connected)
- XIAO SCL -> MCP23017 SCL -> Display SCL (all connected)

---

## **Step 10: Connect Button Signals to MCP23017 (BACK face)**

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
| UP                     | B0      | MCP B0 -> any hole in UP button signal shared row     |
| Down                   | B1      | MCP B1 -> any hole in DOWN button signal shared row   |
| Left                   | B2      | MCP B2 -> any hole in LEFT button signal shared row   |
| Right                  | B3      | MCP B3 -> any hole in RIGHT button signal shared row  |
| L                      | B4      | MCP B4 -> any hole in L button signal shared row      |
| Select                 | B5      | MCP B5 -> any hole in SELECT button signal shared row |
| Share                  | B6      | MCP B6 -> any hole in SHARE button signal shared row  |
| **RIGHT SIDE BUTTONS** |         |                                                      |
| North                  | A0      | MCP A0 -> any hole in NORTH button signal shared row  |
| South                  | A1      | MCP A1 -> any hole in SOUTH button signal shared row  |
| West                   | A2      | MCP A2 -> any hole in WEST button signal shared row   |
| East                   | A3      | MCP A3 -> any hole in EAST button signal shared row   |
| R                      | A4      | MCP A4 -> any hole in R button signal shared row      |
| Start                  | A5      | MCP A5 -> any hole in START button signal shared row  |
| Home                   | A6      | MCP A6 -> any hole in HOME button signal shared row   |

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

The Navigation Buttons should look similar to this when complete:

<div align="center">
<img src="images/Pictures/buttonswired.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>


## **Step 11: Mount Terminal Blocks (BACK face)**

Place two 4-pin screw terminal blocks on BACK face.

**Terminal Block 1 (GND):**

- **Location:** Row A, holes 29, 27, 25, and 23 (BACK face)
- **Orientation:** Wire openings facing UP toward top of board
- Bridge all 4 pins together with solder or wire
- Connect this common GND cluster to nearest GND rail

**Terminal Block 2 (For Drum Signals):**

- **Location:** Row F, holes 30, 28, 26, and 24 (BACK face)
- **Orientation:** Wire openings facing DOWN toward bottom of board
- Leave one hole space between this block and the GND block

<div align="center">
<img src="images/Pictures/terminals3.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

**Label the signal block ports for easy future reference:**

- Pad 1/Left Ka (Row F, hole 24)
- Pad 2/Left Don (Row F, hole 26)
- Pad 3/Right Don (Row F, hole 28)
- Pad 4/Right Ka (Row F, hole 30)




## **Step 12: Connect Drum Signal Terminals (FRONT face)**

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

These screw terminals will connect to the drum sensor TRS jacks in a later step.

---

**CRITICAL: Be sure to visually go over BOTH sides of the entire completed circuit board and cut away ALL extra lengths of component legs that are still sticking through any holes.  Also make sure there are no unwanted blobs of solder or material anywhere on the board.  This is to ensure there are no unwanted electrical shorts.**

---

## **Circuit Complete!**

You now have a single-board circuit with:

✅ OLED Display  
✅ 14 Navigation Buttons  
✅ XIAO RP2040 (drum sensor controller)  
✅ MCP23017 (button/display controller)  
✅ Screw terminals for drum sensors


---

# **5: Build the Drum**

<div align="right"><sub><a href="#table-of-contents">-Back to Top-</a></sub></div>

**Important Scale Notice:** The SVG files provided in the download are the correct scale and should **NOT** be resized. The drum dimensions are precisely calculated to work with the sensor housings and other non-scalable components. If you try to make the drum smaller, other parts will not fit later in the project.

💡 **Scale Verification:** Before cutting, verify the SVG files are at correct scale by checking the shapes in the SVG file against the listed dimensions noted in the KEY section of that same SVG file. 

**No laser cutter or CNC access?** No worries - there are other options. Ask a friend, local shop, or check if your area has a Makerspace. Alternatively, you can print the SVG files full-size across multiple sheets of paper (ensure your printer is set to 100% scale / "Actual Size"), and then overlay the paper on your wood as a template.  You would then cut and drill by hand. Double check that your printed templates are sized properly before cutting or drilling anything.

---

## **5.1: Prepare the Wood**

### **Cut all MDF wood pieces per SVG templates**

💾 [SVG Template packet located here](#9-files--downloads)

Use your laser or CNC machine (or the method available to you) to cut out all of the wood components in the template files.

- You'll be printing one set of upper faceplates
- You'll be printing one set of lower faceplates
- You'll be printing 2x identical baseplates

<div align="center">
<img src="images/Pictures/frontcuts.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

<div align="center">
<img src="images/Pictures/bothbaseplates.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

---

### **Sand smooth as needed**

Sand down any rough edges and surfaces from the cutting step, and wipe off sawdust to prepare for gluing.

---

## **5.2: Assemble the Drum Structure**

### **Glue the rear baseplates together**

Use **wood glue** to glue the two identical rear baseplates together (they are 100% identical, just align the holes and glue together). Clamp securely or weigh down and let dry for several hours. You can also temporarily insert some 6mm bolts into the holes to ensure the two plates remain exactly aligned. Until the glue really starts to set, periodically check the plates to make sure no shifting has occurred - if so, re-align and clamp harder.

Leave the clamps and weight on for at least 6 hours. Let the glue fully cure for 24 hours before continuing past the gluing steps.

<div align="center">
<img src="images/Pictures/woodgluealloverback.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

<div align="center">
<img src="images/Pictures/basegluescrews.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

<div align="center">
<img src="images/Pictures/weightclamps.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

---

### **Assemble and glue the drum faceplates together**

There will be 4 finished drum faceplates that you will be assembling during this step: Left Ka, Left Don, Right Don, and Right Ka. Each faceplate consists of TWO pieces - a smooth TOP plate along with a corresponding BOTTOM plate with holes in it.

<div align="center">
<img src="images/Pictures/woodglue.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

<div align="center">
<img src="images/Pictures/donkaglued.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

**For Ka Plates:**

Let's start with the Ka plates - specifically, the Left Ka. You will be using the Left Ka TOP and the Left Ka BOTTOM plate.

Apply wood glue to the underside of the top Ka plate and apply wood glue to the topside of the lower Ka plate. Press the two pieces together and clamp or weigh them down. Until the glue really starts to set, periodically check the plates to make sure no shifting has occurred - if so, re-align and clamp harder. Leave the clamps and weight on for at least 6 hours. Let the glue fully cure for 24 hours before continuing past the gluing steps.

**Repeat this exact same process for the Right Ka.**

**For Don Plates:**

Apply wood glue to the underside of the top Don plate and apply wood glue to the topside of the lower Don plate (don't forget, each piece is exactly the same in this step only, so it is up to you which is the top part and which is the bottom part). Press the two pieces together and clamp or weigh them down. Until the glue really starts to set, periodically check the plates to make sure no shifting has occurred - if so, re-align and clamp harder. Leave the clamps and weight on for at least 6 hours. Let the glue fully cure for 24 hours before continuing past the gluing steps.

**Repeat this exact same process for the Right Don.**

When complete, you will have 4 faceplates - each consisting of a top half and a bottom half.


---
### **Sand all Don, Ka, and Baseplate Edges**
- Sand any extra glue off of the surfaces of the drum and
- Be EXTRA sure to sand off any excess glue that has dripped out of the edges/seams.  This will ensure a clean, uniform surface to work with.


<div align="center">
<img src="images/Pictures/laseredges.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

---
### **Rout/Chamfer all Don Ka rim edges**


Using a router and a 1/4" roundover bit, rout down every edge on the top and bottom of all glued pieces. If you don't have a router, you can use a deburring tool to lightly shave off the outer edges on the Don and Ka pieces.  Sandpaper can also achieve a similar effect.

This procedure helps prevent stick damage and wear and tear on your drum and cover, as well as gives your drum a professional, clean look by softening/rounding down all of the sharp, 90 degree edges.

<div align="center">
<img src="images/Pictures/routerbit2.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

<div align="center">
<img src="images/Pictures/routerbit.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

<div align="center">
<img src="images/Pictures/deburr2.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>



<div align="center">
<img src="images/Pictures/routededges.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

<div align="center">
<img src="images/Pictures/routholes.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

<div align="center">
<img src="images/Pictures/allfaces.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

---

### **Spray All Wood With Clear Lacquer**
You may now spray several coats of Clear Lacquer Spray Paint to the front back and sides all wood pieces.  This will help protect and seal the wood and reduce wear and tear.  Spray a single coat, let sit for 45 minutes.  Spray two more coats in a similar manner.  Let dry completely for 4 hours after the final coat before continuing.

---


### **Drill Holes For Threaded Inserts**

The following step allows the drum to be built with no visible mounting holes on the drum face. This improves aesthetics and reduces wear on sticks and the drum cover. You will drill holes into the underside of the top faceplates so the threaded inserts for the grommets can be installed from beneath and remain invisible.

The hols you will be workin with are on the underside of the bottom Drum Face Plates.

Using a Drill press, insert a **9mm** diameter drill bit (or the specialized drill bit that came with your threaded wood inserts), and set it up so that when you drill press is fully lowered, the bottom of the drill bit is exactly 2mm above your drill press plate/table.  This way, you can ensure that every time you lower the drill press, you're getting the exact depth needed each time.

<div align="center">
<img src="images/Pictures/drillpre.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

If you don't have a drill press, and only have a standard drill, that's still ok.  In that case, measure the exact thickness of your Don and Ka Plates (should be 12mm-13mm) and subtract 2mm from that measurement and mark that new, lower number on your 9mm drill bit with a piece of tape. 

For example, if your Plates are exactly 13mm thick, take a piece of tape and put it on your drill bit so that the bottom edge of the tape is at exactly 11mm from the tip of the drill bit.  You'll simply drill down until the bottom of the tape is at the surface of your plate.  It is suggested to practice holes on a scrap piece of wood to make sure you have the hang of it, and that everything is working as planned.

<div align="center">
<img src="images/Pictures/11mmdrill2.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

<div align="center">
<img src="images/Pictures/11mmdrill.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

Now that your drill depth is set, locate the 14 pre-cut **9.5mm** holes located on the underside of the top plates where the rubber grommets will go - you will be using those holes as a drill guide.

Carefully Drill **straight** down into those 9.5mm holes and turn them into new **9.5mm wide x 11mm deep** holes (or whatever your calculated depth was). **Repeat this for all 14 similar holes.**

<div align="center">
<img src="images/Pictures/kahole.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

---

### **Chamfer/Bevel holes**

Using a Deburring tool, Chamfer/Bevel the (inner) rim of each of the 14 holes you just drilled hole so that the tapered head of the threaded inserts will tighten down flush and feel smooth when your fingers pass over them. This chamfering can be done with a sharp screwdriver or a sharp knife if you don't have a deburring tool. 

Additionally, if you have a drill press, or you can actually just strist the drill bit using your hand, you can use an 11mm drill bit and drill straignt down intop the top of your new 9.5mm holes for the depth of ONLY anout 1-2mm.  this will create a chamfered rim just the same as the above steps.

Make sure you repeat this exact same process for all 14 holes.

<div align="center">
<img src="images/Pictures/deburred2.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

<div align="center">
<img src="images/Pictures/6mminsertpartial.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>


---

### **Install the M6 threaded inserts**

Screw in the **14 individual M6 threaded wood inserts** into the corresponding holes until flush (add a drop of **Superglue** to the *outside* of threads to help permanently secure them to wood).  

If you feel that your threaded insert won't go flush, deburr a little more material to make the taper in the wood larger.

It is absolutely crucial that these threaded inserts are flush or JUST below the surface to ensure that the grommets in future steps will sit flush.



<div align="center">
<img src="images/Pictures/kagrommettool.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

<div align="center">
<img src="images/Pictures/6mminsertflush.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>



---

### **Install M3 threaded inserts into wooden faceplates**

Each faceplate has **two  4.5mm holes on the underside** where sensor housings will mount using M3 threaded inserts.  You'll be inserting a total of 8 M3x3mm threaded inserts

**Installation:**

Place a small drop of superglue on the outer edge of each M3x3 threaded insert before you install it.  Rest the insert on top of the 4.5mm hole with the narrow side down, and make sure the insert is level.


<div align="center">
<img src="images/Pictures/threadedinsertout.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>
Using a hammer or rubber mallet, tap the insert into the wood until it's perfectly flush with the surface.  **Two M3x3mm threaded inserts** will be installed beneath each Don and Ka Top Plate.

<div align="center">
<img src="images/Pictures/installedinserts.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

<div align="center">
<img src="images/Pictures/doninserts.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

This is how it will look when all M6 and M3 threaded inserts are installed:

<div align="center">
<img src="images/Pictures/allinserts.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>


---
### **Prepare The Stainless Steel Mounts**

The Official Taiko no Tatsujin Arcade Drum Controllers use large, machine-cut steel metal plates mounted to the rear of the wooden faceplates.  These metal plates add rigidity which, in turn, help absorb vibration, improve stick feedback, as well as help dampen the sound.  My original design for this project included plans to make your own metal plates, but that required heavy machinery which was extremely impractical and dangerous, not to mention, Steel plates can get very expensive.

I wouldn't let that stop me from creating the proper Arcade experience, however. I knew I needed to design a new method - a method that would replicate ALL of the benefits and feeling of the Steel Arcade Plates, without actual typical Steel Plate method.  After a few days of brainstorming, was able to create a solution that replaces the Steel Plates while retaining 100% of their benefit.

The 1/4" x 1-1/2 OD" Stainless Steel Fender Washers mounts you will be constructing in this next step is my perfect solution.  

You will still get the true arcade benefits of rigidity, vibration absorption, improved stick feedback, and sound dampening without ANY of trouble of using actual steel plates...and for only a few dollars!



**Prepare the Stainless Steel Washer Stacks:**

(Note, the washers may have one side that is slightly sloped/rounded, if so, make that the top face and orient them all the same)

1. Use rubbing alcohol to clean the top and bottom surfaces of 42 1/4" x 1-1/2 OD" Stainless Steel Fender Washers .

2. Each stack will consist of 3 washers superglued together to make a single unit.

   <div align="center">
   <img src="images/Pictures/3washers.jpg" width="350px" style="display: block; margin: 0 auto;">
   </div>

3. Place 3 drops of superglue on top of one of the washers.

   <div align="center">
   <img src="images/Pictures/3washersglue.jpg" width="350px" style="display: block; margin: 0 auto;">
   </div>

4. Place another washer on top of the glue dots.  Hold the stack of two washers in your fingers and press them together for 30 seconds, making sure the edges stay evenly aligned.

   <div align="center">
   <img src="images/Pictures/2washerspreglue.jpg" width="350px" style="display: block; margin: 0 auto;">
   </div>

5. Place 3 drops of superglue on top of the new stack of two washers that you just fastened together.  Place the third washer on top of those new glue dots.

   <div align="center">
   <img src="images/Pictures/2washersglue.jpg" width="350px" style="display: block; margin: 0 auto;">
   </div>

6. Hold the new stack of three washers in your fingers and press them together for 30 seconds, making sure the edges stay evenly aligned.  This stack is complete, set it to the side.

   <div align="center">
   <img src="images/Pictures/washers3stack.jpg" width="350px" style="display: block; margin: 0 auto;">
   </div>

7. Repeat this until you have 14 stacks of 3 washers superglued together.

   <div align="center">
   <img src="images/Pictures/washersall.jpg" width="350px" style="display: block; margin: 0 auto;">
   </div>

8. Set these stacks off to the side and let them cure for at least 30 minutes while you assemble the rubber isolators in the next step.

   

## **5.3: Assemble The Rubber Isolators**

### **Cut nylon bolt head**

1. Measure 20mm of length on one of the 35mm Nylon Bolts.
2. Using a pair of cutters, cut and discard the remining portion of the bolt that has the head attached.  We will only be using the straight 20mm bolt portion for this project.
3. Repeat this process until you have 14 20mm bolts.

<div align="center">
<img src="images/Pictures/20mmnylon.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

<div align="center">
<img src="images/Pictures/nylonseparate.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>


---

### **Apply Loctite to isolator**

Apply one drop or less of **Loctite** to threads on the inside of the rubber isolator threaded hole.

---

### **Install bolt in isolator**

Screw one end of the 20mm bolt into isolator until it stops.

<div align="center">
<img src="images/Pictures/isolatorcomplete.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>



---

### **Apply Loctite to drum plate inserts**

Add one drop or less of **Loctite** to inside threads of the M6 threaded inserts on the bottom face of the drum plates.

---

### **Install isolator/Steel Washer assemblies**

(<a href="[URL_GOES_HERE](https://youtu.be/N6h3QC_hvl8?si=WZEyYCb-pd2_z-tV)">HERE</a> is a timelapse video demonstrating how to mount the Isolators and Washers to to the faceplate)

1. Place a washer stack over one of the 6mm threaded insert holes on the bottom of one of the faceplates.

   <div align="center">
   <img src="images/Pictures/washersready.jpg" width="350px" style="display: block; margin: 0 auto;">
   </div>

2. Feed the nylon end of the Isolator through the washer stack, and into the threaded insert.  Finger tighten and ensure that the washers do not move/spin and that there's no gap between the wood and the washers, or the washers and the grommet.

   <div align="center">
   <img src="images/Pictures/washersinstalled.jpg" width="350px" style="display: block; margin: 0 auto;">
   </div>

3. This is how the sideview of the assembled Isolator/Washer set should look.  If there is a gap, ensure that the 6mm threaded insert is in fact flush to the wood surface.  If the washer stack IS flush, and a gap still remains, unscrew the nylon bolt from the wood, and carefully cut off an additional 1mm of length, and try again.  Repeat if necessary, only removing the smallest amount of bolt needed until everything is flush and snug.

   <div align="center">
   <img src="images/Pictures/washersflush.jpg" width="350px" style="display: block; margin: 0 auto;">
   </div>

4. Repeat this step for all 14 mounting holes.

   <div align="center">
   <img src="images/Pictures/allisolatorsandwashers.jpg" width="350px" style="display: block; margin: 0 auto;">
   </div>

---

## **5.4: Print Sensor Housings**

### **Print The Sensor Housings**

💾 [Sensor Housing files are in the file packet here](#9-files--downloads)

Print 4 complete sets of Sensor Housings (each set has a top and bottom).

Use **PLA filament**.

**Printer Settings:** 0.2mm layer height, 40% Gyroid infill, no supports needed.

---

## **5.5: Assemble Sensor Electronics**

🎥 [Video overview of sensor housing assembly](https://youtu.be/tQe-xDEqEdY)

### **Cut neoprene discs**

Cut four **12mm neoprene discs** by using the SVG template.

💾 [Neoprene Disc Template Files are in the file packet here](#9-files--downloads)

---

### **Glue neoprene disc to housing**

Place several drops of **Superglue** into the raised center ring in the bottom shell of the housing. Place a single neoprene disc in this ring on top of the glue. Press lightly for 30 seconds.

<div align="center">
<img src="images/Pictures/sensorandhousing.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

---

### **Strip Siamese wire**

Take a **12" length of Siamese wire**, strip both ends exposing approximately 12mm of the two wires within.

---

### **Solder TRS Jack**

Take one end of the wire and solder the two exposed wires to the male TRS jack:

- **Red** connects to the **TIP** of the male TRS jack
- **Black** connects to the **SLEEVE** of the male TRS jack

💡 **Tip:** You can use your multimeter in Continuity mode to confirm which terminal is the Sleeve and which is the Tip.

<div align="center">
<img src="images/Pictures/trssolder.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

---

### **Solder to Piezo**

Take the *other* end of the stripped wire, and solder the **red** wire to piezo center disc, and solder the **black** wire to outer brass ring. The video linked at the beginning of this section illustrates how to do this properly.

<div align="center">
<img src="images/Pictures/sensorsolder.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

---

### **Glue Piezo to Neoprene Mount**

Add several drops of **Superglue** onto the top surface of neoprene that is already glued to the bottom housing shell. Center the **piezo sensor** face up (the all-brass side faces *down*, your wires will be on the top) onto the neoprene. Press lightly for 30 seconds. Be sure that the wire is laying across the strain relief channel portion on one side of the bottom housing channel.

💡 **Note:** Pic varies slightly from your version - this was an earlier version. You will have a more pronounced strain relief channel.

<div align="center">
<img src="images/Pictures/sensorinhousing.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

---

### **Enclose Housing**

Add a drop of **Superglue** to the strain relief channel *below* the wire, as well as on top, and add a few drops to the upper housing around the inside rim. Now assemble the top and bottom housing pieces together, press and hold for 30 seconds. The top shell of the housing will nest into place when aligned properly.


<div align="center">
<img src="images/Pictures/sensorholding.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>



#### **Repeat these steps so that you end up with 4 complete sensor assemblies**




---

### **Mount the Sensor Housings to The Drum**

(Here is a timelapse video demonstrating how to mount the sensor housing to the faceplates)

Mount your 4 completed housings to the underside of drum faces using **two M3x8mm screws** for each housing and screwing them into the threaded inserts. Be sure that the bottom of the housing (the side with the neoprene disc glued to it) is flast against the wood.

Tighten snug so that the sensor housing is firmly pressed against the wood - but don't over-tighten.


<div align="center">
<img src="images/Pictures/sensorpreinstall.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

<div align="center">
<img src="images/Pictures/sensorinstalled.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

Repeat this for each Don and Ka faceplate.

<div align="center">
<img src="images/Pictures/allsensorsinstalled.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

---

## **5.6: Mounting Hardware And Faceplates**

### **Mount Adjustable Speaker Plate**


1. Place your **speaker bracket** against the rear/bottom of your baseplate so aligns with the 4 pre-cut mounting holes.

2. Feed **Four M6x16 bolts** though a single 1/4" x 1-1/2 OD" Stainless Steel Fender Washer and then through the four mounting holes on the baseplate so that they come out through the speaker mounting plate holes.  Use 4 M6 knobs to tightly secure the mounting plate against the rear of the Drum



<div align="center">
<img src="images/Pictures/speakerplate2.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

<div align="center">
<img src="images/Pictures/speakerplateback.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>


---

### **Assemble Drum Faceplates to Rear Baseplate**

(Here is a timelapse video demonstrating how to mount the faceplates to the baseplate)

Now assemble the rest of the drum structure by feeding the 14 **M6x18 bolts** on the bottom of the 4 drum faces through baseplate holes of the rear baseplate. Be sure to route the sensor wires through the nearest, round cut-out in the baseplate.

<div align="center">
<img src="images/Pictures/kainstalled.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

<div align="center">
<img src="images/Pictures/frontdone.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>


---



### **Attach TRS barrel mounts**

💾 [Barrel Mount Files are in the file packet here](#9-files--downloads)

1. Carefully flip the partially assembled drum over so that the drum faces dont fall out.
2. 3D Print and Attach **TRS barrel mounts** with adhesive tape as seen in picture.

2. Place the four TRS couplers in the mounts. You will have one pair of Barrel Mounts on each side of the drum.
3. Attach the sensor wires to the top of their respective Barrel Connector





<div align="center">
<img src="images/Pictures/rearwithnylon.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>


---

### **Attach the 6mm Knobs**

Attach a 6mm knob to all of the exposed bolts and tighten finger tight.  Now all 4 faceplates are securely fastened to the baseplate.

<div align="center">
<img src="images/Pictures/backalmostdone.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>



### **Connect Control Box Extension cables**

Connect the ends of the coiled 3.5mm extension cables to the bottom of the Barrel Connectors.  These extension cables will eventually plug into the female TRS jacks on your Control Box. 

<div align="center">
<img src="images/Pictures/baseplatecomplete.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>




---

# **6: Control Box**

<div align="right"><sub><a href="#table-of-contents">-Back to Top-</a></sub></div>

You're almost there!

If you built the board to the exact specifications in my guide, you'll be able to 3D print the included enclosure box for a professional finish to your circuit.

---

## **6.1: 3D Print and Assemble The Control Box**

### **Print the Enclosure**

<div align="center">
<img src="images/Pictures/printer.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

💾 [Controller Enclosure Files are in the file packet here](#9-files--downloads)

Print the Control Box base and lid using the following settings:

--Layer Height: .20mm

--Infill: Gyroid fill @ 20%

--Supports: Automatic



### **Add TRS Terminal Jacks & USB Coupler**

<div align="center">
<img src="images/Pictures/trsports.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>
Add the 4 TRS jacks into the 4 holes in the back of the base. You will see 4 slight depressions in the base floor to help you align them. Push the female ports all the way into the hole, through the back wall, until it stops. Use a small amount of hot glue to ensure the jacks stay in place.

<div align="center">
<img src="images/Pictures/usb.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

Place the USB coupler in the bottom opening in the base.  Use the slight depression in the base floor to help you align it.  Orient the coupler so that the USB-A port is facing out, and the USB-C port is facing inside the box.  Use a small amount of hot glue to ensure the coupler stays in place.

<div align="center">
<img src="images/Pictures/trsandusb.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

### **Mount The Circuit To The Enclosure Lid**

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

### **Attach Lid To The Control Box Hinges**

<div align="center">
<img src="images/Pictures/hinge.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>


The hinges on the box were cleverly designed to be fastened using a piece of standard, 1.75mm filament as opposed to a metal hinge pin.

<div align="center">
<img src="images/Pictures/filamentinsert.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

Cut off a small section of filament that is  long enough to fit through each set of three hinge sections.  Cut the end at an angle, and gently, yet firmly, feed it all the way through all 3 sections of the hinge you're working on.  It will be snug, but that is by design Cut each end flush.  Repeat this for the other hinge as well.  

---

## **6.2: Connect Circuit Wiring To Control Box**

### **Create the Wires**

<div align="center">
<img src="images/Pictures/ribboncable.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

From the spool of 20 AWG 4-wire ribbon cable, cut 2 lengths of ribbon cable approximately 215mm each.  Crimp the ends using 22 AWG wire ferrules (with small gauge wire like this, going a size smaller on the ferrule helps secure the crimp better and prevent it from pulling off).

One set of 4 wires will be the SIGNAL set, and the other set of 4 wires will be the GROUND set.



### **Connect The Ground Wires**

<div align="center">
<img src="images/Pictures/newgndterminal.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

Using the GROUND wire set, connect one end of 4 wires to the GND terminal block on the circuit board.

<div align="center">
<img src="images/Pictures/trswires.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

Connect the opposite end of the GROUND wire set to the GND terminal in each of the 4 TRS jacks in your Control Box.  There is no specific order for the GND wires here.  Any GND terminal on the circuit board can connect to any TRS GND terminal.

### **Connect The Signal Wires**

<div align="center">
<img src="images/Pictures/signalterminalnumbers.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>


<div align="center">
<img src="images/Pictures/newsignalterminal.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>



Using the set of SINGNAL wires, connect the 4 wires ends to the SIGNAL terminal block on the circuit board in the following manner: 
| Terminal Pin | Connects To |
|-------------|-------------|
| Pin 1 | TIP terminal of TRS Jack 1 |
| Pin 2 | TIP terminal of TRS Jack 2 |
| Pin 3 | TIP terminal of TRS Jack 3 |
| Pin 4 | TIP terminal of TRS Jack 4 | 

<div align="center">
<img src="images/Pictures/trswires.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>


### **Connect The XIAO Board To The Control Box USB Coupler**

Using the short USB-C to USB-C cable, connect the XIAO board to the USB-C coupler inside the Control Box.



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



## **6.3: Floor Stand And Control Box Mount**

### **Mount Drum to speaker stand**

Mount the angled speaker bracket to the speaker stand.

<div align="center">
<img src="images/Pictures/speakermount.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

### **Mount Control Box and Connect Signal Wires**

<div align="center">
<img src="images/Pictures/controllermounted.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

Attach the phone holder arm to the central post of the Speaker Stand.  Attach the Control Box to the Spring-Loaded phone bracket, and adjust to your preferred position.  Connect the Drum Sensor TRS male ends that are hanging down on the drum to the corresponding TRS Female Jack on the Control Box.



### **Add drum cover**

Add your Drum cover, it will fit perfectly. Since my dimensions for the drum in this project are exactly the same as the Arcade Drum dimensions (427mm in diameter), I recommend locating an official Arcade drum skin. One source that seems to always have them in stock is [here](https://taiko.ac/products/rubber-drum-pad).

If that isn't an option for you, you can try using a towel, blanket, large mouse pad, thin foam, whatever you want that gives you sound-reducing qualities as well as the amount of bounce that you're looking for.

💡 **Pro Tip:** The beauty of this Project is that you can adjust the Drum Thresholds and make it perform just how you want regardless of Drum cover.

<div align="center">
<img src="images/Pictures/drumcover.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>



### **Adjust height/angle for Comfort**



<div align="center">
<img src="images/Pictures/angle.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

---



# **7: Flash The Firmware**

<div align="right"><sub><a href="#table-of-contents">-Back to Top-</a></sub></div>

Great job making it this far! Now it's time to prepare the board and flash the firmware. This is the quickest and easiest part of the entire build.

💾 **Download the firmware files:**
- [OuchiTaiko Firmware v1.0](#9-files--downloads)
- [Flash Nuke (Board Wipe)](#9-files--downloads)

---

## **Step 1: Enter Bootloader Mode**
<div align="center">
<img src="images/Pictures/bootloaderbutton.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

Hold down the small **"B" button** on your XIAO RP2040 micro controller board while connecting it to your PC. The board is now in BOOTLOADER mode, and will appear as a removable drive - it is waiting for you to drag a file to it.

---

## **Step 2: Wipe the Board**

Rule Of Thumb: A good practice is to **always** flash a cleaning/nuke file to your board before you flash your actual firmware. This clears out any residual memory or cache that, while rare, may still be hanging around that could potentially cause issues with your new firmware.

To do this, drag the `universal_flash_nuke.uf2` to the root of the removable drive. This wipes the board for a fresh start and automatically reboots it. The drive will reappear as before, in BOOT mode. Drag the `universal_flash_nuke.uf2` file one more time to be extra clean. Now, when the board pops up again as a removeable drive, you'll be ready to flash the controller firmware...

---

## **Step 3: Drag the Firmware File To The Board to Flash It**

Drag `OTPv1.0.uf2` to the removable drive window that popped up earlier. After a few seconds, the board will upload the file and automatically reboot. This time, the board will not reappear, this is normal behavior and means that your board is now in controller mode and ready to play. If for some reason your board doesn't reboot automatically after dragging the firmware file, wait about 15 seconds, and then unplug the board and plug it back it. This will manually reboot it, and it's ready to go!

You're ready for testing and calibration!

---

# **8: Calibration & Settings**

<div align="right"><sub><a href="#table-of-contents">-Back to Top-</a></sub></div>

## **Complete Button and Drum Pad Test (Before Calibration)**

By doing this step, you're just ensuring that they are all wired correctly, registering properly, and in the correct location.

**Circuit Test Procedure Method #1 - USB Power:**
(I created this first method so that you can test and calibrate your drum without even needing a Console or PC - jut USB Power)

1. Connect your controller to a PC or USB Power (phonr charger, etc).  After the bootup animation you will see the main screen that "Switch Tatacon Mode" on top.  
2. Now, tap each drum face and ensure that the 4 animated icons on the bottom of the screen respond to your hits (If you notice any automatic/false triggering of the drum panels, you can ignore that as the Auto Calibration in the next section of the guide will instantly solve that issue.)
3. Now, press each navigation button one at a time, and you should see the corresponding button name flash in the upper right portion of the screen.  Ensure that each button is labeled correctly.


**Circuit Test Procedure Method #2 - PC Web Browser:**

1. Hold **SELECT** for 1 second to enter the system menu
2. The controller, by default, will be in **SWITCH TATACON mode**
3. Stay inside the Menu structure for this test (this prevents any false triggers from interrupting your button test)
4. Visit the [Meloncolle Taiko Controller](https://meloncolle.com/tatacon) website and tap all four drum sensors and press all 14 navigation buttons
5. The drum you tap and the buttons you press should  light up the corresponding drum or button on Drum Controller on the screen

You can now exit the menu screen. If you notice any automatic/false triggering of the drum panels, you can ignore that as the Auto Calibration  in the next section of the guide will instantly solve that issue.


---


# **8: Calibration & Settings**
<div align="right"><sub><a href="#table-of-contents">-Back to Top-</a></sub></div>

---

## **Initial Setup After Flashing**

Upon first boot, your controller defaults to **Nintendo Switch Tatacon** mode. If this works for your setup, you're ready to play!

To change controller modes or adjust settings:

**Hold SELECT for 1 second**
1. System menu will open
2. Select "Controller Mode"
3. Choose the desired controller
4. The cotnroller will reboot into the new controller mode

---

## **Auto Calibrate - Quick Start Guide**

**Recommended for all users - takes less than 30 seconds**

The Auto Calibrate feature automatically determines optimal sensitivity for your specific build, eliminating the need for manual threshold adjustments.

### **When to Use Auto Calibrate:**

✅ **First-time setup** - Essential for new builds  
✅ **After hardware changes** - New drum cover, replaced sensors, remounted drums  
✅ **Environmental changes** - New flooring, extreme temperature/humidity shifts (if performance affected)  
✅ **Gameplay issues** - Missed hits or false triggers

### **How to Run Auto Calibrate:**

**From System Menu:**
1. **Hold SELECT (1 sec)** → System Menu
2. Navigate: **Drum Tuning** → **Auto Calibrate** and press **A**
3. **Instructions screen** appears - read at your own pace, press **A** to begin

### **During Calibration (28 seconds total):**

**Phase 1 - Instructions (wait for A press):**
- Screen shows: "When calibration starts, hit all 4 pads randomly with drum rolls & standard hits"
- **Read the directions carefully**
- Press **A (EAST button)** when ready to start

**Phase 2 - Countdown (3 seconds):**
- Screen shows: "DON'T HIT YET!" + countdown timer
- **Don't touch the drums** - system is sampling ambient noise

**Phase 3 - Calibration (20 seconds):**
- Screen shows: "HIT DRUMS!" + progress bar + time remaining
- **Mix drum rolls and standard hits:**
  - Alternate between fast rolls and single strikes
  - Use normal playing force (as you would in-game)
  - Hit **all 4 pads** multiple times
  - Vary your strength - light taps to strong hits
  - System captures maximum force, crosstalk patterns, and dynamic range

**Phase 4 - Results (5 seconds):**
- System displays calculated thresholds for each drum
- Thresholds are automatically applied and saved to memory
- Returns to Drum Tuning menu

### **What the System Analyzes:**

- **Maximum force:** Your typical in-game strikes on each pad
- **Playing dynamics:** Range between light taps and strong hits
- **Crosstalk patterns:** Vibration transfer between adjacent drums
- **Safety margins:** Automatic 50-point buffer above detected crosstalk

### **Calibration Quality Checks:**

The system validates your calibration and may require a redo if:
- Maximum hit values are too low (below 300) - you didn't hit hard enough
- Crosstalk exceeds 50% of hit strength - mechanical isolation issue

💡 **Pro Tip:** During calibration, play naturally - mix fast rolls with single hits at various strengths. The 20-second window gives you plenty of time to demonstrate your full playing style. Don't hold back, but don't hit harder than you would during actual gameplay.

---

## **Manual Threshold Adjustment**

For users who prefer hands-on control or need fine-tuning after Auto Calibrate.

### **Access Manual Thresholds:**

1. **Hold SELECT (1 sec)** -> System Menu
2. Navigate: **Drum Tuning** -> **Manual Thresholds**

### **Unified Threshold Screen:**

All 4 drum thresholds displayed on one screen with **live visual feedback:**

### **Controls:**

| Button | Action |
|--------|--------|
| **UP/DOWN** | Select drum (KaL -> DonL -> DonR -> KaR) |
| **LEFT/RIGHT** | Decrease/Increase threshold (hold to fast-repeat) |
| **A (EAST)** | Save changes |
| **B (SOUTH)** | Cancel and restore original values |

### **Understanding Thresholds:**

- **Range:** 0-4095 (12-bit ADC resolution)
- **Lower values** = More sensitive (easier to trigger, risk of false hits)
- **Higher values** = Less sensitive (harder to trigger, might miss light taps)

**Typical ranges after Auto Calibrate:**
- Ka (rim) pads: 300-400 (higher sensitivity needed)
- Don (center) pads: 150-250 (naturally stronger hits)

### **Live Testing:**

- Hit drums while adjusting - animations show when threshold is crossed
- Underline shows currently selected drum
- Changes take effect immediately for real-time feedback

💡 **Pro Tip:** Start with Auto Calibrate, then use Manual Thresholds only for minor tweaks. The Auto Calibrate algorithm accounts for crosstalk and safety margins that are difficult to set manually.

---

## **Controller Mode Selection**

**Access:** System Menu -> Controller Mode

OuchiTaiko can emulate multiple controller types for maximum compatibility.

### **Available Modes:**

| Mode | Platform Compatibility
|------|----------------------|
| **Switch Tatacon** | Nintendo Switch
| **Switch Pro** | Nintendo Switch 
| **PS3 Dualshock3** | PC/Steam only
| **PS4 Tatacon** | PC/Steam only
| **PS4 Dualshock4** | PC/Steam only
| **Keyboard P1** | PC
| **Keyboard P2** | PC
| **Xbox 360** | PC/Xbox
| **Android (XInput)** | Android devices
| **iOS (XInput)** | iOS devices
| **Analog P1** | PC (TaikoArcadeLoader)
| **Analog P2** | PC (TaikoArcadeLoader)
| **MIDI** | PC/DAW software
| **Debug** | Development/Testing

### **Changing Controller Mode:**

1. **Hold SELECT (1 sec)** -> System Menu
2. Select **Controller Mode**
3. Use **LEFT/RIGHT** to choose mode
4. Press **A** to confirm
5. Controller **automatically reboots** to apply new mode

⚠ ️ **Note:** Changing controller mode requires a reboot. Any unsaved settings in other menus will be lost.


---


## **Advanced Settings**

### **Reset ALL Settings**

**Access:** System Menu -> Advanced -> Reset ALL Settings

Restores ALL settings to factory defaults, just as if you had flashed the firmware again.


### **Hold Time (Debounce)**

**Access:** System Menu -> Advanced -> Hold Time

This controls the minimum time between registered hits on the same pad. 

-Manual debounce adjustment (0-255ms)
-Default is 25ms

You may want to adjust this if:

-You experience double-triggering from a single hit (raise the value to ignore false rebounds)
-If very fast rolls aren't registering consistently (lower the value to allow faster hits)
-Most users should leave this at the default 25ms setting

---

# **11: Menu System Reference**

<div align="right"><sub><a href="#table-of-contents">-Back to Top</a></sub></div>

## **Quick Access**

- **Hold SELECT (1 sec):** Enter system menu

---

## **Navigation Controls**

| Button         | Action                                                 |
| -------------- | ------------------------------------------------------ |
| **LEFT/RIGHT** | Navigate menu items / Adjust values / Toggle selections |
| **UP/DOWN**    | Select drum (in Manual Thresholds only)                |
| **EAST (A)**   | Confirm selection / Save changes                       |
| **SOUTH (B)**  | Cancel / Go back / Restore original value              |

---

## **Complete Menu Tree Structure**

Main Menu
├──── Controller Mode
│   ├── Nintendo Switch Tatacon Drum
│   ├── Nintendo Switch Pro Controller
│   ├── Sony PS3 Dualshock3 (PC/Steam only)
│   ├── Sony PS4 Tatacon Drum (PC/Steam only)
│   ├── Sony PS4 Dualshock4 (PC/Steam only)
│   ├── Keyboard Player 1
│   ├── Keyboard Player 2
│   ├── Microsoft Xbox Xbox360
│   ├── Android (XInput)
│   ├── iOS (XInput)
│   ├── Analog Player 1
│   ├── Analog Player 2
│   ├── MIDI Controller
│   └── Debug Mode
├──── Drum Tuning
│   ├── Auto Calibrate
│   │   └── Continue? (Yes/No)
│   ├── Manual Thresholds
│   │   ├── KaL (0-4095)
│   │   ├── DonL (0-4095)
│   │   ├── DonR (0-4095)
│   │   └── KaR (0-4095)
│   └── Reset Thresholds
│       └── Confirm? (Yes/No)
├──── Advanced
│   ├── Reset ALL Settings
│   │   └── Confirm? (Yes/No)
│   └── Hold Time (Debounce)
│       └── Value (0-255ms)
├──── USB Flash Mode
│   └── Confirm? (Yes/No)
└──── About


---

## **Idle Screen Display**

When not in menu, the OLED Main Screen shows:

- **Top:** Current controller mode
- **Center:** Live drum hit animations (expanding rings from center dots)
- **Center-top:** Button indicator (shows currently pressed button label)
- **Bottom:** Menu instructions



---

# **12: About**

<div align="right"><sub><a href="#table-of-contents">-Back to Top-</a></sub></div>

This project is a one-of-a-kind Hybrid creation that pulls from several amazing resources. As they say, "We stand on the shoulders of Giants..."

The core firmware version as well the navigation and OSD hardware portion of the Circuit was adapted from the amazing work by **'ravinrabbid'** who created the **[DonCon 2040](https://github.com/ravinrabbid/DonCon2040)** Project. You can visit that GitHub for more of the nitty-gritty details of the original software.

The inspiration for the circuit came from the amazingly straightforward circuitry work by **'kasasiki3'** who created the **[HIDtaiko](https://github.com/kasasiki3/HIDtaiko/tree/master)** Project (2040 edition).

Without these two projects, none of this would have been possible.

Credit to **'Gadgetoid'** on GitHub for his [pico-universal-flash-nuke](https://github.com/Gadgetoid/pico-universal-flash-nuke) file that helps clean things up between flashes.

Credit to Dork Design (https://www.printables.com/@DorkDesign) as well for creating the ultimate customizable box resource. I tried so many different methods and systems for creating my custom controller, and the Dork Design system worked the very first time I tried it. Accept no immitation! Check their page out, they have other awesome creations as well.

Credit to Discord User **'Allspice'** who wanted to help, but was concerned about not having any electronics or soldering skills, yet managed to build a complete circuit and drum controller from scratch using my guide. He helped shape early version of the How-To Guide that inspired me to make it as extensive as it is today.

Credit to Discord User **'Moshir'** who helped with the final once-over of this guide to make ensure that it flowed well for those with tons of experience as well as for those with no eperience at all - wgich was one of my most important goals when creating this entire project.

I also want to take a moment to mention a few, more general resources that were invaluable for me during this entire process. These are great if you ever want to go down the rabbit hole of the world of Taiko no Tatsujin modding or Custom Controller creation.  With these assets at your disposal, you have the power to create anything imaginable:

- [Taiko no Tatsujin Modding! Discord Channel](https://discord.com/invite/HFm37aA5zr)
- [Cons&Stuff :) Discord Channel](https://discord.gg/qEpzyvf8DY)
- [OpenStick Community - GP2040-ce Project Discord Channel](https://discord.com/invite/openstickcommunity-1049366310389289001)

## **Closing Thoughts**

I can't thank you enough for taking the time to follow along with my guide. It really means a lot.

If you have any project-specific questions or suggestions, please use the Discussion panel on my GitHub. You can also ask more general questions in any of the Discord Channels I mentioned above - they are all very helpful in their own way.

### **Spread The Word**

Please share your results and excitement as well as this guide with your favorite DIY gaming communities. Tag me (KillerQ97) when you do!

Enjoy, Have Fun, and Peace Out!

www.ouchitaiko.com

---

# **13: Copyright Information**

<div align="right"><sub><a href="#table-of-contents">-Back to Top-</a></sub></div>

## Attribution Chain

- **Original DonCon2040 firmware:** (c) ravinrabbid (MIT License)
- **HIDtaiko components:** (c) kasasiki3 (Apache License 2.0)
- **Custom modifications and features:** Created by KillerQ (Dual-licensed under MIT and Apache 2.0)

**Copyright (c) 2025 KillerQ**

---

## Legal Compliance Notice

Any distribution of this firmware (binary or source) must include:

1. A copy of the MIT License from DonCon2040
2. A copy of the Apache License 2.0 from HIDtaiko
3. Copyright notices from both original projects
4. Attribution to all contributors

**Complete license texts are provided in the repository:**
- [LICENSE-MIT](LICENSE-MIT) - MIT License (DonCon2040 & OuchiTaiko contributions)
- [LICENSE-APACHE](LICENSE-APACHE) - Apache 2.0 License (HIDtaiko & OuchiTaiko contributions)

---

## Summary

This project is fully transparent about its licensing and gives proper credit to all contributors. The OuchiTaiko Project is dual-licensed to ensure compatibility with both upstream projects.

Thank you to ravinrabbid and kasasiki3 for their incredible open-source contributions that made this project possible.

**KillerQ**

www.ouchitaiko.com

---

*OuchiTaiko Project | Smarter. Simpler. Better.*
