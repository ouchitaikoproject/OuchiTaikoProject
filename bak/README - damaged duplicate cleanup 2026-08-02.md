<div align="center">
<img src="images/pictures/mainlogo.jpg" width="100%" style="max-width: 1200px; display: block; margin: 0 auto;">
</div>

---

<div align="center">
<i>In Japanese, “ouchi” means “home,” and “taiko” means “drum.”
</br>  
Together, OuchiTaiko represents the joy of bringing the arcade Taiko experience into your own space.</i>
</div>

---

<a id="table-of-contents"></a>

# Table of Contents

- [1. Project Overview](#project-overview)
- [Key Features and World Firsts Not Seen In Any Other Existing Projects](#key-features)
  - [1) Guided Calibrate](#guided-calibrate)
  - [2) Adaptive Baseline Software Intelligence (ABSI)](#absi)
  - [3) Custom Arcade Sensor Suspension Modeled After Japanese Taiko Arcade Hardware](#sensor-suspension)
  - [Complete Hardware & Features](#complete-hardware-features)
- [2. Parts List for Electronics](#parts-list-electronics)
- [3. Parts List for Hardware](#parts-list-hardware)
- [4. Assemble the Controller PCB](#assemble-controller-pcb)
- [5: Build the Sensor Housings](#build-sensor-housings)
  - [5.0: Print The Sensor Housings](#print-sensor-housings)
  - [5.1: Assemble Sensor Electronics](#assemble-sensor-electronics)
- [6: Build the Drum](#build-drum)
  - [6.1: Prepare the Wood](#prepare-the-wood)
  - [6.2: Assemble the Drum Structure](#assemble-drum-structure)
  - [6.3: Assemble The Rubber Isolators](#assemble-rubber-isolators)
  - [6.4: Mount the Sensor Housings to The Drum](#mount-sensor-housings-to-drum)
  - [6.5: Mounting The Hardware And Faceplates](#mounting-hardware-and-faceplates)
- [7: Control Box Connection](#control-box-connection)
  - [7.1: Floor Stand Mode](#floor-stand-mode)
  - [7.2: Table-Top Mode](#table-top-mode)
- [8: Flash the Firmware](#flash-the-firmware)
- [9: Calibration & Settings](#calibration-settings)
- [10: Files & Downloads](#files-downloads)
- [11: Basic Troubleshooting](#basic-troubleshooting)
- [12: Menu System Reference](#menu-system-reference)
- [13: About](#about)
- [14: Copyright Information](#copyright-information)

---

<a id="project-overview"></a>
# 1. Project Overview

## The Finished Build
This guide walks you through building a full Arcade-Scale Taiko drum and controller that can be used in either **Floor‑Standing Tripod Mode** or **Table‑Top Mode**.

<div align="center">
<img src="images/pictures/tripodboth.jpg" style="width: 100%; max-width: 800px; height: auto;">
</div>

<div align="center">
<img src="images/pictures/tabletopboth.jpg" style="width: 100%; max-width: 800px; height: auto;">
</div>

---

Hi, I’m KillerQ — creator of the **OuchiTaiko Project**, an open‑source guide for building a professional, arcade‑grade Taiko drum controller.

This project is the result of a year and a half of R&D focused on solving the high cost, limited availability, and inconsistent performance of commercial Taiko controllers. By combining a completely new software performance engine, a custom through‑hole PCB, and a mechanically isolated sensor system, OuchiTaiko delivers arcade‑level responsiveness with approachable DIY construction.

All you need are very basic soldering and woodworking skills.

This guide is intentionally thorough and detailed — the goal is simple: **less guessing, more building.**

---

<a id="key-features"></a>
# Key Features and World Firsts Not Seen In Any Other Existing Projects

Traditional Taiko controllers often require constant threshold tuning, extensive coding and firmware recompiling, or compromises between sensitivity and false triggers.

**OuchiTaiko solves these issues with three core systems:**

1. Guided Calibrate  
2. Adaptive Baseline Software Intelligence (ABSI)  
3. Custom Sensor Suspension System
---

<a id="guided-calibrate"></a>
## 1) Guided Calibrate
OuchiTaiko includes two Guided Calibrate options:

- [`guided_calibrate.html`](https://raw.githubusercontent.com/ouchitaikoproject/OuchiTaikoProject/main/download_files/guided_calibrate.html) - web-based guided calibration tool
- On-device Guided Calibrate for the same method, but on the controller  
- [`universal_flash_nuke.uf2`](https://raw.githubusercontent.com/ouchitaikoproject/OuchiTaikoProject/main/download_files/universal_flash_nuke.uf2) - flash reset utility
- [`universal_flash_nuke.uf2`](https://raw.githubusercontent.com/ouchitaikoproject/OuchiTaikoProject/main/download_files/universal_flash_nuke.uf2) - flash reset utility
- [`universal_flash_nuke.uf2`](https://raw.githubusercontent.com/ouchitaikoproject/OuchiTaikoProject/main/download_files/universal_flash_nuke.uf2) - flash reset utility
- Quick retesting after hardware changes  

---

<a id="absi"></a>
## 2) Adaptive Baseline Software Intelligence (ABSI)
ABSI was created by me just for this project, and I am very proud of it. It drastically improves stability through software rather than complex analog circuitry.

- [`universal_flash_nuke.uf2`](https://raw.githubusercontent.com/ouchitaikoproject/OuchiTaikoProject/main/download_files/universal_flash_nuke.uf2) - flash reset utility
- [`guided_calibrate.html`](https://raw.githubusercontent.com/ouchitaikoproject/OuchiTaikoProject/main/download_files/guided_calibrate.html) - web-based guided calibration tool
- [`universal_flash_nuke.uf2`](https://raw.githubusercontent.com/ouchitaikoproject/OuchiTaikoProject/main/download_files/universal_flash_nuke.uf2) - flash reset utility

---

<a id="sensor-suspension"></a>
## 3) Custom Arcade Sensor Suspension Modeled After Japanese Taiko Arcade Hardware
A mechanically isolated sensor housing improves consistency and reduces noise.

Benefits include:

- [`guided_calibrate.html`](https://raw.githubusercontent.com/ouchitaikoproject/OuchiTaikoProject/main/download_files/guided_calibrate.html) - web-based guided calibration tool
- [`universal_flash_nuke.uf2`](https://raw.githubusercontent.com/ouchitaikoproject/OuchiTaikoProject/main/download_files/universal_flash_nuke.uf2) - flash reset utility
- [`guided_calibrate.html`](https://raw.githubusercontent.com/ouchitaikoproject/OuchiTaikoProject/main/download_files/guided_calibrate.html) - web-based guided calibration tool
- [`universal_flash_nuke.uf2`](https://raw.githubusercontent.com/ouchitaikoproject/OuchiTaikoProject/main/download_files/universal_flash_nuke.uf2) - flash reset utility

---

<a id="complete-hardware-features"></a>
## Complete Hardware & Features

### Standalone Testing & Display
No PC required for basic verification.

- OLED display with live feedback  
- [`universal_flash_nuke.uf2`](https://raw.githubusercontent.com/ouchitaikoproject/OuchiTaikoProject/main/download_files/universal_flash_nuke.uf2) - flash reset utility
- [`universal_flash_nuke.uf2`](https://raw.githubusercontent.com/ouchitaikoproject/OuchiTaikoProject/main/download_files/universal_flash_nuke.uf2) - flash reset utility
- [`universal_flash_nuke.uf2`](https://raw.githubusercontent.com/ouchitaikoproject/OuchiTaikoProject/main/download_files/universal_flash_nuke.uf2) - flash reset utility

---

### Professional Hardware

- [`guided_calibrate.html`](https://raw.githubusercontent.com/ouchitaikoproject/OuchiTaikoProject/main/download_files/guided_calibrate.html) - web-based guided calibration tool
- 14 navigation buttons for full game control  
- [`universal_flash_nuke.uf2`](https://raw.githubusercontent.com/ouchitaikoproject/OuchiTaikoProject/main/download_files/universal_flash_nuke.uf2) - flash reset utility
- [`guided_calibrate.html`](https://raw.githubusercontent.com/ouchitaikoproject/OuchiTaikoProject/main/download_files/guided_calibrate.html) - web-based guided calibration tool
- [`guided_calibrate.html`](https://raw.githubusercontent.com/ouchitaikoproject/OuchiTaikoProject/main/download_files/guided_calibrate.html) - web-based guided calibration tool

---

### Controller Modes

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
| **Guided Calibrate** | Development/Testing

---

### Zero Coding Required

- [`guided_calibrate.html`](https://raw.githubusercontent.com/ouchitaikoproject/OuchiTaikoProject/main/download_files/guided_calibrate.html) - web-based guided calibration tool
- On‑device menus  
- [`universal_flash_nuke.uf2`](https://raw.githubusercontent.com/ouchitaikoproject/OuchiTaikoProject/main/download_files/universal_flash_nuke.uf2) - flash reset utility
- [`universal_flash_nuke.uf2`](https://raw.githubusercontent.com/ouchitaikoproject/OuchiTaikoProject/main/download_files/universal_flash_nuke.uf2) - flash reset utility

---

### Demo Video

- [`guided_calibrate.html`](https://raw.githubusercontent.com/ouchitaikoproject/OuchiTaikoProject/main/download_files/guided_calibrate.html) - web-based guided calibration tool

---

<a id="parts-list-electronics"></a>
# 2. Parts List for Electronics

<div align="right"><sub><a href="#table-of-contents">-Back to Top-</a></sub></div>

These linked parts are verified to work together as tested. Similar/comparable parts can be used, but at your own risk.

| Item | Pieces Needed | Product Link |
| --- | --- | --- |
| Waveshare RP2040‑Zero | 1 | https://www.amazon.com/dp/B09SBCKYSC |
| 1N4148 diodes | 4 | https://www.amazon.com/dp/B0DN62QFYS |
| 0.1uF / 100nF ceramic capacitors | 4 | https://www.amazon.com/dp/B08B3VCK42 |
| 3.5mm female audio jack, PJ‑320A | 4 | https://www.amazon.com/dp/B07KY862P6 |
| 3.5mm TRS male plugs with leads | 4 | https://www.amazon.com/dp/B0D72TV7Y5 |
| 1K ohm resistor, 1/2W | 4 | https://www.amazon.com/dp/B0FP1YFMVM |
| 27mm piezo sensors | 4 | https://www.amazon.com/dp/B07RK2TQ8D |
| 128x64 mono OLED display (I2C) | 1 | https://www.amazon.com/dp/B09T6SJBV5 |
| 6mm tactile switches with caps | 14 | https://www.amazon.com/dp/B07VQF8P2Y |
| 6" coiled 3.5mm TRS extension cable | 4 | https://www.amazon.com/dp/B0D7CXJ5LQ |
| 3.5mm TRS barrel coupler | 4 | https://www.amazon.com/dp/B07YSCGBL7 |
| 6ft USB‑A to USB‑C cable | 1 | https://www.amazon.com/dp/B0CKXZML11 |

---

<a id="parts-list-hardware"></a>
# 3. Parts List for Hardware

<div align="right"><sub><a href="#table-of-contents">-Back to Top-</a></sub></div>

<a id="required-tools-supplies"></a>
## Required Tools & Supplies

1. Laser cutter, CNC, or alternative cutting method  
2. Soldering iron + solder  
3. Wire strippers / cutters  
4. Screwdrivers  
5. Hot glue gun  
6. Strong clamps  
7. Drill  
8. Router  
9. Sandpaper  
10. Utility knife  
11. Rubber mallet  

<a id="hardware-parts-list"></a>
## Hardware Parts List

| Item | Pieces Needed | Product Link |
| --- | --- | --- |
| 1/4" (6mm) MDF, 2ft x 4ft | 2 | https://www.homedepot.com/p/1-4-in-x-2-ft-x-4-ft-MDF-1508104/202089069 |
| 1/4" x 1‑1/2" OD stainless fender washers | 46 | https://www.amazon.com/dp/B0D8ZRL1CP |
| Wood glue | 1 | https://www.amazon.com/dp/B0002YQ378 |
| M3 x 8mm bolts | 8 | https://www.amazon.com/dp/B07CMQ1SQH |
| M3 x 5mm threaded inserts | 8 | https://www.amazon.com/dp/B0CLKCQ2SH |
| M6 x 10mm wood threaded inserts | 14 | https://www.amazon.com/dp/B0CNLRFFH1 |
| M6 x 35mm nylon bolts | 14 | https://www.amazon.com/dp/B07L9ZS21T |
| M6 threaded 20mm x 15mm rubber isolators | 14 | https://www.amazon.com/dp/B0BKPHT6Y9 |
| PLA filament | 1 | https://www.amazon.com/dp/B081S5N5PC |
| Gel superglue | 1 | https://www.amazon.com/dp/B0006HUJCQ |
| Medium thread adhesive / Loctite | 1 | https://www.amazon.com/dp/B000FIXQXK |
| 2.2mm scuba knit neoprene fabric | 1 | https://www.amazon.com/dp/B0DK1B5LZ7 |
| Finger knobs with pass‑through M6 threads | 18 | https://www.amazon.com/dp/B07RW9ZH4H |
| Mini PA speaker tripod stand (optional) | 1 | https://www.amazon.com/dp/B094N9YG72 |
| Desktop monitor stand (optional) | 1 | https://www.amazon.com/dp/B072QDMRS8 |
| Adjustable angle speaker bracket | 1 | https://www.amazon.com/dp/B0DR8NCZP6 |
| Phone holder w/ gooseneck arm (optional) | 1 | https://www.amazon.com/dp/B0F32MLBZX |
| Rubber Taiko drum cover (recommended) | 1 | https://taiko.ac/products/rubber-drum-pad |
| 1/4" roundover router bit | 1 | https://www.amazon.com/dp/B0C5DVBNLS |
| Clear spray glaze / lacquer | 1 | https://www.amazon.com/dp/B00D0293SA |
| Deburring tool (optional) | 1 | https://www.amazon.com/dp/B07RM1D6WD |


---

<a id="assemble-controller-pcb"></a>
# 4. Assemble the Controller PCB

<div align="right"><sub><a href="#table-of-contents">-Back to Top-</a></sub></div>

<div align="center">
<img src="images/pictures/schematic.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

<div align="center">
<img src="images/pictures/pcbfilledtop.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

## Purchase the OuchiTaiko Controller PCB (Circuit Board).

My project is set up to where you will order the through-hole circuit board and then solder the basic components in yourself.  This saves time, money, and eliminates any room for error.

I <i>highly</i> recommend purchasing the raw Controller PCB from OSHPARK PCB Fabrications. They are in the US, and all manufacturing is done in the US. They are very reasonably priced, and you can even pay for expedited production/shipping. They sell in sets of 3 only, but that price is still reasonable, and that way you have a spare in case you damage one, or you can split the order with a friend.  You may notice that this company charges more than others that manufacture outside of the US, but if you read around their site, you'll see that they quality is above-and-beyond and the boards have an amazing look and feel to them.

If you decide use OSHPARK Here is a link to the Board that's already uploaded and verified on their site, and you can hit purchase right away. I do not make any money off of this link or benefit in any way - they just offer the convenient service of allowing me to upload the board so that people can simply add it to their cart and check out without the hassle of uploading the raw files and hoping everything is correct and verified after the upload, etc.


<a href="https://oshpark.com/shared_projects/mY0Hmilt">HERE is the link to the OSHPARK OuchiTaiko Controller Board Page</a>


However, since I made my project open source, you have 100% control over where to have the board manufactured.  If you have a company that you trust that you would like use instead, here is the Gerber .zip file that contains all of the universal project files that the company will need to produce my OuchiTaiko PCB.  

[`ouchitaiko_pcb_gerber_v1_final.zip`](https://raw.githubusercontent.com/ouchitaikoproject/OuchiTaikoProject/main/download_files/ouchitaiko_pcb_gerber_v1_final.zip) is the PCB Gerber file package.


One person used https://jlcpcb.com/ and ordered my board (by uploading the above Gerber file) and had 5 boards made for about $6 total including shipping and it took around 2 weeks.

<a id="control-pcb-overview"></a>
## Control PCB Overview

My custom OuchiTaiko PCB integrates controller logic, display wiring, navigation buttons, and drum input circuitry all together into a single compact board.

To finish building it, you will solder the following components to the board you have made:

- **1× Waveshare RP2040‑Zero**  
- **14× tactile buttons**  
- **4× 1N4148 diodes**  
- **4× 1K resistors**  
- **4× 0.1uF capacitors**  
- **1× I2C OLED**  
- **4× PJ‑320A TRS jacks**

That's it!  Everything is just attached via basic soldering.

---

<a id="recommended-assembly-order"></a>
## Recommended Assembly Order

### Step 1: Solder Passive Components
Install:

- `R1-R4` = 1K resistors

<div align="center">
<img src="images/pictures/pcbresistors.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

- `C1-C4` = 0.1uF capacitors

<div align="center">
<img src="images/pictures/pcbcapacitors.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

- `D1-D4` = 1N4148 diodes

<div align="center">
<img src="images/pictures/pcbdiodes.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

Important: Ensure diode black bands match the silkscreen (cathode to the **left** when viewed from above).



---

### Step 2: Solder the 14 Buttons
Install all navigation buttons.

<div align="center">
<img src="images/pictures/pcbbuttons.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

The button legs will actually appear in a rectangular layout orientation - longer on one side. The holes in the board will match that slightly rectangular shape - there is no polarity as long as that orientation is adhered to. Ensure each button sits flat before soldering all four legs.

---

### Step 3: Mount the OLED
Install the OLED using the following pin order (verify that yours matches):

- [`guided_calibrate.html`](https://raw.githubusercontent.com/ouchitaikoproject/OuchiTaikoProject/main/download_files/guided_calibrate.html) - web-based guided calibration tool
- [`universal_flash_nuke.uf2`](https://raw.githubusercontent.com/ouchitaikoproject/OuchiTaikoProject/main/download_files/universal_flash_nuke.uf2) - flash reset utility
- [`universal_flash_nuke.uf2`](https://raw.githubusercontent.com/ouchitaikoproject/OuchiTaikoProject/main/download_files/universal_flash_nuke.uf2) - flash reset utility
- [`universal_flash_nuke.uf2`](https://raw.githubusercontent.com/ouchitaikoproject/OuchiTaikoProject/main/download_files/universal_flash_nuke.uf2) - flash reset utility

<div align="center">
<img src="images/pictures/pcboled.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>



---

### Step 4: Install the TRS Drum Input Jacks
Install the four PJ‑320A jacks. Their pin layout only fits into one orientation.

<div align="center">
<img src="images/pictures/pcbjacks.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>



---

### Step 5: Mount the RP2040‑Zero
Install the Waveshare RP2040‑Zero. Only the 20 castellated <i>perimeter pads</i> need soldering.

<div align="center">
<img src="images/pictures/pcbwaveshare.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>


A helpful soldering demonstration for soldering castellated pads:  
https://youtu.be/rGvvwXrv310?t=312

---

<a id="inspect-your-work"></a>
## Inspect Your Work

Before powering on, confirm:

- [`universal_flash_nuke.uf2`](https://raw.githubusercontent.com/ouchitaikoproject/OuchiTaikoProject/main/download_files/universal_flash_nuke.uf2) - flash reset utility
- OLED pins are correct  
- [`universal_flash_nuke.uf2`](https://raw.githubusercontent.com/ouchitaikoproject/OuchiTaikoProject/main/download_files/universal_flash_nuke.uf2) - flash reset utility
- [`universal_flash_nuke.uf2`](https://raw.githubusercontent.com/ouchitaikoproject/OuchiTaikoProject/main/download_files/universal_flash_nuke.uf2) - flash reset utility
- [`universal_flash_nuke.uf2`](https://raw.githubusercontent.com/ouchitaikoproject/OuchiTaikoProject/main/download_files/universal_flash_nuke.uf2) - flash reset utility
- [`universal_flash_nuke.uf2`](https://raw.githubusercontent.com/ouchitaikoproject/OuchiTaikoProject/main/download_files/universal_flash_nuke.uf2) - flash reset utility


If you were just building the controller and sensors for an existing drum that you already had, you can skip ahead to **Section 8: Flash the Firmware**.


---



<a id="build-sensor-housings"></a>
# **5: Build the Sensor Housings**

<a id="print-sensor-housings"></a>
## **5.0: Print The Sensor Housings**

\*\* download \*\* [`ouchitaiko_sensor_housing.stl`](https://raw.githubusercontent.com/ouchitaikoproject/OuchiTaikoProject/main/download_files/ouchitaiko_sensor_housing.stl) is the sensor housing STL file

Print 4 complete sets of Sensor Housings (each set has a top and bottom).

Use **PLA filament**.

**Printer Settings:** 0.2mm layer height, 40% Gyroid infill, no supports needed.



<a id="assemble-sensor-electronics"></a>
## **5.1: Assemble Sensor Electronics**

\*\* video \*\* [Video overview of sensor housing assembly](https://youtu.be/tQe-xDEqEdY)

### **Cut neoprene discs**

Cut four **12mm neoprene discs** by using the SVG template.

\*\* download \*\* [Neoprene Disc Template Files are in the file packet here](#9-files--downloads)



### **Glue neoprene disc to housing**

Place several drops of **Superglue** into the raised center ring in the bottom shell of the housing. Place a single neoprene disc in this ring on top of the glue. Press lightly for 30 seconds.

<div align="center">
<img src="images/pictures/sensorandhousing.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>



### **Prepare end of TRS Sensor Wires**

Since you are starting with long lengths of wire, you have two choices to proceed.

1. If you plan on building the drum in my guide, then you can cut the length of wire down to approximately 12" so that it will route cleanly through the drum and end up at the connectors at the base of the drum.
2. If you plan on using these sensors to add to a different or already existing drum, I suggest that you leave the wires at their full length. This allows more freedom of mounting and routing when using them with another drum.

With your chosen length of wire, strip the ends of each so that they expose approximately 10mm of red wire and approximately 8mm of black wire. Now strip off approximately 1mm of red and black insulation to expose the bare wire and tin each end with solder.

Repeat this process for all 4 wires.

The difference in length is because you want them to be slightly offset as in this photo: 

<div align="center">
<img src="images/pictures/piezoleads.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>





### **Solder to Piezo**

Now solder the **red** wire to piezo center disc, and solder the **black** wire to outer brass ring. The video linked at the beginning of this section illustrates how to do this easily.

<div align="center">
<img src="images/pictures/sensorsolder.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>



### **Glue Piezo to Neoprene Mount**

Add several drops of **Superglue** onto the top surface of neoprene that is already glued to the bottom housing shell. Center the **piezo sensor** face up (the all-brass side faces *down*, your wires will be on the top) onto the neoprene. Press lightly for 30 seconds. 

\*\* note \*\* Pic varies slightly from your version - this was an earlier version.

<div align="center">
<img src="images/pictures/piezopremount.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

<div align="center">
<img src="images/pictures/piezomount.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>


### **Enclose Housing**

Add a drop of **Superglue** around the inner rim of the housing and then press the two halves together. Press and hold for 30 seconds. The top shell of the housing will nest into place when aligned properly.

After 30 seconds, add a little bit of **Hot Glue** to the wire where it meets against the outside of the housing - this will act as additional strain relief. 


<div align="center">
<img src="images/pictures/sensorholding.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

> Repeat these steps until you have 4 complete sensor assemblies.
>
> **Using these sensors with an existing drum that you have?**
> You can skip the full drum build. Jump to [**Section 6.4: Mount the Sensor Housings to The Drum**](#mount-sensor-housings-to-drum), then continue to [**Section 8: Flash the Firmware**](#flash-the-firmware).
>
> **Building a new drum using my guide?**
> If you are building the complete OuchiTaiko drum, continue below to Step #6 to begin constructing your Taiko Drum.


<a id="build-drum"></a>
# **6: Build the Drum**

<div align="right"><sub><a href="#table-of-contents">-Back to Top-</a></sub></div>

**Important Scale Notice:** The SVG files provided in the download are the correct scale and should **NOT** be resized. The drum dimensions are precisely calculated to work with the sensor housings and other non-scalable components. If you try to make the drum smaller, other parts will not fit later in the project.

\*\* note \*\* Before cutting, verify the SVG files are at correct scale by checking the shapes in the SVG file against the listed dimensions noted in the KEY section of that same SVG file. 

**No laser cutter or CNC access?** No worries - there are other options. Ask a friend, local shop, or check if your area has a Makerspace. Alternatively, you can print the SVG files full-size across multiple sheets of paper (ensure your printer is set to 100% scale / "Actual Size"), and then overlay the paper on your wood as a template.  You would then cut and drill by hand. Double check that your printed templates are sized properly before cutting or drilling anything.



<a id="prepare-the-wood"></a>
## **6.1: Prepare the Wood**

### **Cut all MDF wood pieces per SVG templates**

\*\* download \*\* [`ouchitaiko_laser_files.svg`](https://raw.githubusercontent.com/ouchitaikoproject/OuchiTaikoProject/main/download_files/ouchitaiko_laser_files.svg) and [`ouchitaiko_laser_files.lbrn2`](https://raw.githubusercontent.com/ouchitaikoproject/OuchiTaikoProject/main/download_files/ouchitaiko_laser_files.lbrn2) are the wood template files

Use your laser or CNC machine (or the method available to you) to cut out all of the wood components in the template files.

- You'll be printing one set of upper faceplates
- You'll be printing one set of lower faceplates
- You'll be printing 2x identical baseplates

<div align="center">
<img src="images/pictures/frontcuts.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

<div align="center">
<img src="images/pictures/bothbaseplates.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>



### **Sand smooth as needed**

Sand down any rough edges and surfaces from the cutting step, and wipe off sawdust to prepare for gluing.



<a id="assemble-drum-structure"></a>
## **6.2: Assemble the Drum Structure**

### **Glue the rear baseplates together**

Use **wood glue** to glue the two identical rear baseplates together (they are 100% identical, just align the holes and glue together). Clamp securely or weigh down and let dry for several hours. You can also temporarily insert some 6mm bolts into the holes to ensure the two plates remain exactly aligned. Until the glue really starts to set, periodically check the plates to make sure no shifting has occurred - if so, re-align and clamp harder.

Leave the clamps and weight on for at least 6 hours. Let the glue fully cure for 24 hours before continuing past the gluing steps.

<div align="center">
<img src="images/pictures/woodgluealloverback.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

<div align="center">
<img src="images/pictures/basegluescrews.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

<div align="center">
<img src="images/pictures/weightclamps.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>



### **Assemble and glue the drum faceplates together**

There will be 4 finished drum faceplates that you will be assembling during this step: Left Ka, Left Don, Right Don, and Right Ka. Each faceplate consists of TWO pieces - a smooth TOP plate along with a corresponding BOTTOM plate with holes in it.

<div align="center">
<img src="images/pictures/woodglue.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

<div align="center">
<img src="images/pictures/donkaglued.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

#### For Ka Plates

Let's start with the Ka plates - specifically, the Left Ka. You will be using the Left Ka TOP and the Left Ka BOTTOM plate.

Apply wood glue to the underside of the top Ka plate and apply wood glue to the topside of the lower Ka plate. Press the two pieces together and clamp or weigh them down. Until the glue really starts to set, periodically check the plates to make sure no shifting has occurred - if so, re-align and clamp harder. Leave the clamps and weight on for at least 6 hours. Let the glue fully cure for 24 hours before continuing past the gluing steps.

**Repeat this exact same process for the Right Ka.**

#### For Don Plates

Apply wood glue to the underside of the top Don plate and apply wood glue to the topside of the lower Don plate (don't forget, each piece is exactly the same in this step only, so it is up to you which is the top part and which is the bottom part). Press the two pieces together and clamp or weigh them down. Until the glue really starts to set, periodically check the plates to make sure no shifting has occurred - if so, re-align and clamp harder. Leave the clamps and weight on for at least 6 hours. Let the glue fully cure for 24 hours before continuing past the gluing steps.

**Repeat this exact same process for the Right Don.**

When complete, you will have 4 faceplates - each consisting of a top half and a bottom half.




### **Sand all Don, Ka, and Baseplate Edges**
- [`universal_flash_nuke.uf2`](https://raw.githubusercontent.com/ouchitaikoproject/OuchiTaikoProject/main/download_files/universal_flash_nuke.uf2) - flash reset utility
- [`guided_calibrate.html`](https://raw.githubusercontent.com/ouchitaikoproject/OuchiTaikoProject/main/download_files/guided_calibrate.html) - web-based guided calibration tool


<div align="center">
<img src="images/pictures/laseredges.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>



### **Rout/Chamfer all Don Ka rim edges**


Using a router and a 1/4" roundover bit, route down every edge on the top and bottom of all glued pieces. If you don't have a router, you can use a deburring tool to lightly shave off the outer edges on the Don and Ka pieces. Sandpaper can also achieve a similar effect.

This procedure helps prevent stick damage and wear and tear on your drum and cover, as well as gives your drum a professional, clean look by softening/rounding down all of the sharp, 90 degree edges.

<div align="center">
<img src="images/pictures/routerbit2.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

<div align="center">
<img src="images/pictures/routerbit.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

<div align="center">
<img src="images/pictures/deburr2.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

<div align="center">
<img src="images/pictures/routededges.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

<div align="center">
<img src="images/pictures/routholes.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

<div align="center">
<img src="images/pictures/allfaces.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>



### **Spray All Wood With Clear Lacquer**
You may now spray several coats of Clear Lacquer Spray Paint to the front back and sides all wood pieces.  This will help protect and seal the wood and reduce wear and tear.  Spray a single coat, let sit for 45 minutes.  Spray two more coats in a similar manner.  Let dry completely for 4 hours after the final coat before continuing.



### **Drill Holes For Threaded Inserts**

The following step allows the drum to be built with no visible mounting holes on the drum face. This improves aesthetics and reduces wear on sticks and the drum cover. You will drill holes into the underside of the top faceplates so the threaded inserts for the grommets can be installed from beneath and remain invisible.

The holes you will be working with are on the underside of the bottom Drum Face Plates.

Using a Drill press, insert a **9mm** diameter drill bit (or the specialized drill bit that came with your threaded wood inserts), and set it up so that when you drill press is fully lowered, the bottom of the drill bit is exactly 2mm above your drill press plate/table.  This way, you can ensure that every time you lower the drill press, you're getting the exact depth needed each time.

<div align="center">
<img src="images/pictures/drillpre.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

If you don't have a drill press, and only have a standard drill, that's still ok.  In that case, measure the exact thickness of your Don and Ka Plates (should be 12mm-13mm) and subtract 2mm from that measurement and mark that new, lower number on your 9mm drill bit with a piece of tape. 

For example, if your Plates are exactly 13mm thick, take a piece of tape and put it on your drill bit so that the bottom edge of the tape is at exactly 11mm from the tip of the drill bit.  You'll simply drill down until the bottom of the tape is at the surface of your plate.  It is suggested to practice holes on a scrap piece of wood to make sure you have the hang of it, and that everything is working as planned.

<div align="center">
<img src="images/pictures/11mmdrill2.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

<div align="center">
<img src="images/pictures/11mmdrill.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

Now that your drill depth is set, locate the 14 pre-cut **9.5mm** holes located on the underside of the top plates where the rubber grommets will go - you will be using those holes as a drill guide.

Carefully Drill **straight** down into those 9.5mm holes and turn them into new **9.5mm wide x 11mm deep** holes (or whatever your calculated depth was). **Repeat this for all 14 similar holes.**

<div align="center">
<img src="images/pictures/kahole.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>



### **Chamfer/Bevel holes**

Using a Deburring tool, Chamfer/Bevel the (inner) rim of each of the 14 holes you just drilled hole so that the tapered head of the threaded inserts will tighten down flush and feel smooth when your fingers pass over them. This chamfering can be done with a sharp screwdriver or a sharp knife if you don't have a deburring tool. 

Additionally, if you have a drill press, or you can actually just twist the drill bit using your hand, you can use an 11mm drill bit and drill straight down into the top of your new 9.5mm holes for the depth of only about 1-2mm. This will create a chamfered rim just the same as the above steps.

Make sure you repeat this exact same process for all 14 holes.

<div align="center">
<img src="images/pictures/deburred2.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

<div align="center">
<img src="images/pictures/6mminsertpartial.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>




### **Install the M6 threaded inserts**

Screw in the **14 individual M6 threaded wood inserts** into the corresponding holes until flush (add a drop of **Superglue** to the *outside* of threads to help permanently secure them to wood).  

If you feel that your threaded insert won't go flush, deburr a little more material to make the taper in the wood larger.

It is absolutely crucial that these threaded inserts are flush or JUST below the surface to ensure that the grommets in future steps will sit flush.

<div align="center">
<img src="images/pictures/kagrommettool.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

<div align="center">
<img src="images/pictures/6mminsertflush.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>





### **Install M3 threaded inserts into wooden faceplates**

Each faceplate has **two  4.5mm holes on the underside** where sensor housings will mount using M3 threaded inserts.  You'll be inserting a total of 8 M3x3mm threaded inserts

#### Installation

Place a small drop of superglue on the outer edge of each M3x3 threaded insert before you install it.  Rest the insert on top of the 4.5mm hole with the narrow side down, and make sure the insert is level.


<div align="center">
<img src="images/pictures/threadedinsertout.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>
Using a hammer or rubber mallet, tap the insert into the wood until it's perfectly flush with the surface.  **Two M3x3mm threaded inserts** will be installed beneath each Don and Ka Top Plate.

<div align="center">
<img src="images/pictures/installedinserts.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

<div align="center">
<img src="images/pictures/doninserts.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

This is how it will look when all M6 and M3 threaded inserts are installed:

<div align="center">
<img src="images/pictures/allinserts.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>




### **Prepare The Stainless Steel Mounts**

The Official Taiko Arcade Drum Controllers use large, machine-cut steel metal plates mounted to the rear of the wooden faceplates.  These metal plates add rigidity which, in turn, help absorb vibration, improve stick feedback, as well as help dampen the sound.  My original design for this project included plans to make your own metal plates, but that required heavy machinery which was extremely impractical and dangerous, not to mention, Steel plates can get very expensive.

I wouldn't let that stop me from creating the proper Arcade experience, however. I knew I needed to design a new method - a method that would replicate ALL of the benefits and feeling of the Steel Arcade Plates, without actual typical Steel Plate method.  After a few days of brainstorming, was able to create a solution that replaces the Steel Plates while retaining 100% of their benefit.

The 1/4" x 1-1/2 OD" Stainless Steel Fender Washers mounts you will be constructing in this next step is my perfect solution.  

You will still get the true arcade benefits of rigidity, vibration absorption, improved stick feedback, and sound dampening without ANY of trouble of using actual steel plates...and for only a few dollars!

#### Prepare the Stainless Steel Washer Stacks

(Note, the washers may have one side that is slightly sloped/rounded, if so, make that the top face and orient them all the same)

1. Use rubbing alcohol to clean the top and bottom surfaces of 42 1/4" x 1-1/2 OD" Stainless Steel Fender Washers .

2. Each stack will consist of 3 washers superglued together to make a single unit.

   <div align="center">
   <img src="images/pictures/3washers.jpg" width="350px" style="display: block; margin: 0 auto;">
   </div>

3. Place 3 drops of superglue on top of one of the washers.

   <div align="center">
   <img src="images/pictures/3washersglue.jpg" width="350px" style="display: block; margin: 0 auto;">
   </div>

4. Place another washer on top of the glue dots.  Hold the stack of two washers in your fingers and press them together for 30 seconds, making sure the edges stay evenly aligned.

   <div align="center">
   <img src="images/pictures/2washerspreglue.jpg" width="350px" style="display: block; margin: 0 auto;">
   </div>

5. Place 3 drops of superglue on top of the new stack of two washers that you just fastened together.  Place the third washer on top of those new glue dots.

   <div align="center">
   <img src="images/pictures/2washersglue.jpg" width="350px" style="display: block; margin: 0 auto;">
   </div>

6. Hold the new stack of three washers in your fingers and press them together for 30 seconds, making sure the edges stay evenly aligned.  This stack is complete, set it to the side.

   <div align="center">
   <img src="images/pictures/washers3stack.jpg" width="350px" style="display: block; margin: 0 auto;">
   </div>

7. Repeat this until you have 14 stacks of 3 washers superglued together.

   <div align="center">
   <img src="images/pictures/washersall.jpg" width="350px" style="display: block; margin: 0 auto;">
   </div>

8. Set these stacks off to the side and let them cure for at least 30 minutes while you assemble the rubber isolators in the next step.

   

<a id="assemble-rubber-isolators"></a>
## **6.3: Assemble The Rubber Isolators**

### **Cut nylon bolt head**

1. Measure 20mm of length on one of the 35mm Nylon Bolts.
2. Using a pair of cutters, cut and discard the remaining portion of the bolt that has the head attached. We will only be using the straight 20mm bolt portion for this project.
3. Repeat this process until you have 14 20mm bolts.

<div align="center">
<img src="images/pictures/20mmnylon.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

<div align="center">
<img src="images/pictures/nylonseparate.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>




### **Apply Loctite to isolator**

Apply one drop or less of **Loctite** to threads on the inside of the rubber isolator threaded hole.



### **Install bolt in isolator**

Screw one end of the 20mm bolt into isolator until it stops.

<div align="center">
<img src="images/pictures/isolatorcomplete.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>





### **Apply Loctite to drum plate inserts**

Add one drop or less of **Loctite** to inside threads of the M6 threaded inserts on the bottom face of the drum plates.



### **Install isolator/Steel Washer assemblies**

(<a href="https://youtu.be/N6h3QC_hvl8?si=WZEyYCb-pd2_z-tV">HERE</a> is a timelapse video demonstrating how to mount the Isolators and Washers to the faceplate)

1. Place a washer stack over one of the 6mm threaded insert holes on the bottom of one of the faceplates.

   <div align="center">
   <img src="images/pictures/washersready.jpg" width="350px" style="display: block; margin: 0 auto;">
   </div>

2. Feed the nylon end of the Isolator through the washer stack, and into the threaded insert.  Finger tighten and ensure that the washers do not move/spin and that there's no gap between the wood and the washers, or the washers and the grommet.

   <div align="center">
   <img src="images/pictures/washersinstalled.jpg" width="350px" style="display: block; margin: 0 auto;">
   </div>

3. This is how the sideview of the assembled Isolator/Washer set should look.  If there is a gap, ensure that the 6mm threaded insert is in fact flush to the wood surface.  If the washer stack IS flush, and a gap still remains, unscrew the nylon bolt from the wood, and carefully cut off an additional 1mm of length, and try again.  Repeat if necessary, only removing the smallest amount of bolt needed until everything is flush and snug.

   <div align="center">
   <img src="images/pictures/washersflush.jpg" width="350px" style="display: block; margin: 0 auto;">
   </div>

4. Repeat this step for all 14 mounting holes.

   <div align="center">
   <img src="images/pictures/allisolatorsandwashers.jpg" width="350px" style="display: block; margin: 0 auto;">
   </div>




<a id="mount-sensor-housings-to-drum"></a>
## **6.4: Mount the Sensor Housings to The Drum**

(<a href="https://youtu.be/N6h3QC_hvl8?t=265">HERE</a> is a timelapse video demonstrating how to mount the sensor housings to the underside of the faceplates)

Mount your 4 completed housings to the underside of drum faces using **two M3x8mm screws** for each housing and screwing them into the threaded inserts. Be sure that the bottom of the housing (the side with the neoprene disc glued to it) is flat against the wood.

Tighten snug so that the sensor housing is firmly pressed against the wood - but don't over-tighten.


<div align="center">
<img src="images/pictures/sensorpreinstall.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

<div align="center">
<img src="images/pictures/sensorinstalled.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

Repeat this for each Don and Ka faceplate.

<div align="center">
<img src="images/pictures/allsensorsinstalled.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>



<a id="mounting-hardware-and-faceplates"></a>
## **6.5: Mounting The Hardware And Faceplates**

### **Mount Adjustable Speaker Plate**


1. Place your **speaker bracket** against the rear/bottom of your baseplate so aligns with the 4 pre-cut mounting holes.

2. Feed **Four M6x16 bolts** through a single 1/4" x 1-1/2 OD" Stainless Steel Fender Washer and then through the four mounting holes on the baseplate so that they come out through the speaker mounting plate holes. Use 4 M6 knobs to tightly secure the mounting plate against the rear of the Drum

<div align="center">
<img src="images/pictures/speakerplate2.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

<div align="center">
<img src="images/pictures/speakerplateback.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>




### **Assemble Drum Faceplates to Rear Baseplate**

(<a href="https://youtu.be/N6h3QC_hvl8?t=488">HERE</a> is a timelapse video demonstrating how to mount the faceplates to the baseplate)

Now assemble the rest of the drum structure by feeding the 14 **M6x18 bolts** on the bottom of the 4 drum faces through baseplate holes of the rear baseplate. Be sure to route the sensor wires through the nearest, round cut-out in the baseplate.

<div align="center">
<img src="images/pictures/kainstalled.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

<div align="center">
<img src="images/pictures/frontdone.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>




### **Attach TRS barrel mounts**

\*\* download \*\* [`ouchitaiko_barrel_connector_mount.stl`](https://raw.githubusercontent.com/ouchitaikoproject/OuchiTaikoProject/main/download_files/ouchitaiko_barrel_connector_mount.stl) is the barrel mount STL file

1. Carefully flip the partially assembled drum over so that the drum faces don't fall out.
2. 3D print and attach **TRS barrel mounts** with adhesive tape as shown in the picture.
3. Place the four TRS couplers in the mounts. You will have one pair of barrel mounts on each side of the drum.
4. Attach the sensor wires to the top of their respective barrel connector.

<div align="center">
<img src="images/pictures/rearwithnylon.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>




### **Attach the 6mm Knobs**

Attach a 6mm knob to all of the exposed bolts and tighten finger tight.  Now all 4 faceplates are securely fastened to the baseplate.

<div align="center">
<img src="images/pictures/backalmostdone.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

### **Connect Control Box Extension cables**

Connect the ends of the coiled 3.5mm extension cables to the bottom of the Barrel Connectors.  These extension cables will eventually plug into the female TRS jacks on your Control Box. 

<div align="center">
<img src="images/pictures/baseplatecomplete.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

---

<a id="control-box-connection"></a>
# **7: Control Box Connection**

<div align="right"><sub><a href="#table-of-contents">-Back to Top-</a></sub></div>

<a id="floor-stand-mode"></a>
## **7.1: Floor Stand Mode**

### **Mount drum to speaker stand**

Attach the drum and bracket to the speaker stand.

<div align="center">
<img src="images/pictures/speakermount.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

### **Mount control box and connect signal wires**

Mount the control box to the phone holder arm and connect the drum TRS plugs to the matching control box TRS jacks. (this photo shows an older version of the control box, yours will be much smaller)

<div align="center">
<img src="images/pictures/controllermounted.jpg" width="350px" style="display: block; margin: 0 auto;">
</div>

<a id="table-top-mode"></a>
## **7.2: Table-Top Mode**

### **Cut monitor stand tubing**

Shorten the monitor stand tube until the drum height feels right for your desk. The example shown is around `150mm`.

### **Place control box and connect wires**

Position the control box on the table and connect the drum signal wires.

### **Add drum cover**

Install an arcade drum cover if possible. Because the drum matches arcade diameter, official-style covers fit properly.

If needed, you can experiment with other materials such as:

- [`guided_calibrate.html`](https://raw.githubusercontent.com/ouchitaikoproject/OuchiTaikoProject/main/download_files/guided_calibrate.html) - web-based guided calibration tool
- [`guided_calibrate.html`](https://raw.githubusercontent.com/ouchitaikoproject/OuchiTaikoProject/main/download_files/guided_calibrate.html) - web-based guided calibration tool
- [`guided_calibrate.html`](https://raw.githubusercontent.com/ouchitaikoproject/OuchiTaikoProject/main/download_files/guided_calibrate.html) - web-based guided calibration tool
- [`guided_calibrate.html`](https://raw.githubusercontent.com/ouchitaikoproject/OuchiTaikoProject/main/download_files/guided_calibrate.html) - web-based guided calibration tool

Thresholds can always be adjusted later to match the cover you choose.

### **Adjust height and angle**

Set the final angle and height for comfort.

---

<a id="flash-the-firmware"></a>
# **8: Flash the Firmware**

<div align="right"><sub><a href="#table-of-contents">-Back to Top-</a></sub></div>

**Download the firmware files:**
- [`ouchitaiko_project_firmware_v1.uf2`](https://raw.githubusercontent.com/ouchitaikoproject/OuchiTaikoProject/main/download_files/ouchitaiko_project_firmware_v1.uf2)
- [`universal_flash_nuke.uf2`](https://raw.githubusercontent.com/ouchitaikoproject/OuchiTaikoProject/main/download_files/universal_flash_nuke.uf2)

## **Step 1: Enter Bootloader Mode**

Hold the **BOOT** button on the RP2040-Zero while connecting it to your PC. The board should appear as a removable `RPI-RP2` drive.

## **Step 2: Wipe the Board**

It is good practice to flash `universal_flash_nuke.uf2` before flashing the main firmware, especially when switching between builds.

1. Drag `universal_flash_nuke.uf2` onto the drive
2. Let the board reboot
3. Repeat once more if you want a completely clean start

## **Step 3: Flash the Firmware**

Drag `ouchitaiko_project_firmware_v1.uf2` onto the drive.

After the copy finishes, the board should automatically reboot into controller mode.

If it does not reboot on its own, wait a few seconds, unplug it, and reconnect it.

---

<a id="calibration-settings"></a>
# **9: Calibration & Settings**

<div align="right"><sub><a href="#table-of-contents">-Back to Top-</a></sub></div>

## **Quick Hardware Check**

Before calibration:

1. Power the controller over USB
2. Verify the OLED turns on
3. Tap each drum lane and confirm you see hit feedback
4. Press each navigation button and confirm the correct label appears

## Guided Calibrate

The preferred calibration method is **Guided Calibrate**.

Note: Use the Web Browser/HTML method first whenever possible.

### Starting Thresholds

Start at:

- Ka Left: `265`
- Don Left: `190`
- Don Right: `190`
- Ka Right: `265`

### Guided Calibrate (Web Tool)

1. Connect the controller over USB.
2. Put the controller in `Guided Calibrate` mode under `Controller Mode`.
3. Open the Guided Calibrate web tool using EDGE or CHROME.
4. Confirm the (default) thresholds are `265 / 190 / 190 / 265`.
5. Follow the target pad shown on screen.
6. For each pad, do:
   - `5` fast normal hits
   - `1` hard hit
7. If another pad bleeds, raise only that pad and continue when prompted.
8. Repeat until the target pad stays clean.
9. Continue through all four pads.
10. Review the final thresholds.
11. Save to the controller.
12. Test in-game.

### Guided Calibrate (On-Device)

The same method is also available on the controller.

Menu path:

`Drum Tuning -> Guided Calibrate`

1. Select Guided Calibrate.
2. Read the intro and instruction screens, then follow the target pad shown on screen.
3. For each pad, do:
   - `5` fast normal hits
   - `1` hard hit
4. If bleed is detected, the controller will <b><i>automatically</i></b> raise only the offending pad and ask you to hit the same target pad again.
5. Continue until all four pads pass.
6. Review the final thresholds.
7. Save.

### Notes

- If one pad still false triggers, raise only that pad and test again.
- Good physical isolation still helps reduce crosstalk.

## **Manual Threshold Adjustment**

Use **Manual Thresholds** to fine-tune settings after testing in Guided Calibrate.

### **Access**

1. Hold **SELECT** for 1 second
2. Go to **Drum Tuning**
3. Select **Manual Thresholds**

### **Layout**

`KL   DL   DR   KR`

### **Controls**

| Button | Action |
| --- | --- |
| **LEFT / RIGHT** | Select drum |
| **UP / DOWN** | Adjust threshold |
| **A (EAST)** | Save |
| **B (SOUTH)** | Cancel |

### **Threshold Guide**

- lower value = more sensitive
- higher value = less sensitive
- range = `0-4095`


## **Controller Mode Selection**

Open the menu, select **Controller Mode**, choose the desired mode, and confirm with **A**. The controller will reboot automatically when the mode changes.

Available modes:

- Switch Tatacon
- Switch Pro
- PS3 Dualshock3
- PS4 Tatacon
- PS4 Dualshock4
- Keyboard Player 1
- Keyboard Player 2
- Xbox 360
- Android
- iOS
- Analog Player 1
- Analog Player 2
- MIDI Controller
- Guided Calibrate



---

<a id="files-downloads"></a>
# **10: Files & Downloads**

<div align="right"><sub><a href="#table-of-contents">-Back to Top-</a></sub></div>

Current download files:

- [`ouchitaiko_pcb_gerber_v1_final.zip`](https://raw.githubusercontent.com/ouchitaikoproject/OuchiTaikoProject/main/download_files/ouchitaiko_pcb_gerber_v1_final.zip) - PCB Gerber files
- [`ouchitaiko_laser_files.svg`](https://raw.githubusercontent.com/ouchitaikoproject/OuchiTaikoProject/main/download_files/ouchitaiko_laser_files.svg) - wood cutting templates
- [`ouchitaiko_laser_files.lbrn2`](https://raw.githubusercontent.com/ouchitaikoproject/OuchiTaikoProject/main/download_files/ouchitaiko_laser_files.lbrn2) - LightBurn project
- [`ouchitaiko_sensor_housing.stl`](https://raw.githubusercontent.com/ouchitaikoproject/OuchiTaikoProject/main/download_files/ouchitaiko_sensor_housing.stl) - sensor housing STL
- [`ouchitaiko_barrel_connector_mount.stl`](https://raw.githubusercontent.com/ouchitaikoproject/OuchiTaikoProject/main/download_files/ouchitaiko_barrel_connector_mount.stl) - TRS barrel mount STL
- [`ouchitaiko_controller_shell.stl`](https://raw.githubusercontent.com/ouchitaikoproject/OuchiTaikoProject/main/download_files/ouchitaiko_controller_shell.stl) - controller shell STL
- [`ouchitaiko_project_firmware_v1.uf2`](https://raw.githubusercontent.com/ouchitaikoproject/OuchiTaikoProject/main/download_files/ouchitaiko_project_firmware_v1.uf2) - main controller firmware
- [`universal_flash_nuke.uf2`](https://raw.githubusercontent.com/ouchitaikoproject/OuchiTaikoProject/main/download_files/universal_flash_nuke.uf2) - flash reset utility
- [`guided_calibrate.html`](https://raw.githubusercontent.com/ouchitaikoproject/OuchiTaikoProject/main/download_files/guided_calibrate.html) - web-based guided calibration tool

Additional files can be added here later:

- neoprene template

---
<a id="basic-troubleshooting"></a>
# **11: Basic Troubleshooting**

<div align="right"><sub><a href="#table-of-contents">-Back to Top-</a></sub></div>

## **OLED does not turn on**
- check OLED pin order
- inspect solder joints
- confirm the RP2040-Zero is installed correctly

## **A drum lane does not register**
- check piezo solder joints
- confirm the TRS wiring
- verify the lane components are populated on the PCB
- confirm the sensor jack is fully inserted

## **A lane causes false hits on another lane**
- switch to **Guided Calibrate**
- use the Guided Calibrate web tool
- raise the threshold of the falsely triggered lane
- retest

## **Board does not appear as `RPI-RP2`**
- reconnect while holding **BOOT**
- try another USB cable
- try another USB port

---

<a id="menu-system-reference"></a>
# **12: Menu System Reference**

<div align="right"><sub><a href="#table-of-contents">-Back to Top-</a></sub></div>

## **Quick Access**

- Hold **SELECT** for about 1 second to open the system menu

## **Navigation Controls**

| Button | Action |
| --- | --- |
| **LEFT / RIGHT** | Navigate menu items or move between threshold fields |
| **UP / DOWN** | Adjust selected threshold |
| **EAST (A)** | Confirm / save |
| **SOUTH (B)** | Back / cancel |

## **Main Menu**

- Controller Mode
- Drum Tuning
- Advanced
- USB Flash Mode
- About

## **Drum Tuning**

- Guided Calibrate
- Manual Thresholds
- Reset Thresholds

## **Advanced**

- Reset ALL Settings
- Hold Time (Debounce)

## **Idle Screen**

When not in the menu, the OLED shows:

- current controller mode
- live drum hit feedback
- button press indicator
- menu hint text

---

<a id="about"></a>
# **13: About**

<div align="right"><sub><a href="#table-of-contents">-Back to Top-</a></sub></div>

This project builds on the work of several excellent open-source projects and community members.

The original firmware base, menu logic, and OSD direction were adapted from **[DonCon2040](https://github.com/ravinrabbid/DonCon2040)** by **ravinrabbid**.

Circuit inspiration came from **[HIDtaiko](https://github.com/kasasiki3/HIDtaiko/tree/master)** by **kasasiki3**.

Additional thanks:

- **Gadgetoid** for [pico-universal-flash-nuke](https://github.com/Gadgetoid/pico-universal-flash-nuke)
- **Allspice** for early prototype testing from a beginner-builder perspective as well as excellent guide formatting suggestions.
- **Moshir** for helping review the guide for clarity and flow.
- **Train** for converting my custom circuit diagram circuit into a proper Gerber File. Amazing Work!
- **Frost** for being the first user to order a PCB and assemble the complete contoller.
- **Ermagerd** for testing the sensors and controller and putting it to the ultimate player test - since I feel he's the best, and most engaging live streaming Taiko Player on the Internet!

Useful community resources:

- [Taiko no Tatsujin Modding Discord](https://discord.com/invite/HFm37aA5zr)
- [Cons&Stuff :) Discord](https://discord.gg/qEpzyvf8DY)
- [OpenStick Community / GP2040-ce Discord](https://discord.com/invite/openstickcommunity-1049366310389289001)

## **Closing Thoughts**

Thanks for taking the time to build OuchiTaiko.

If you have project-specific questions or suggestions, use the GitHub Discussions page. You can also ask in the Discord communities above.

### **Spread the Word**

If you build one, share it. Tag me (`KillerQ97`) if you post your results.

Enjoy, have fun, and peace out.

[www.ouchitaiko.com](https://www.ouchitaiko.com)

---

<a id="copyright-information"></a>
# **14: Copyright Information**

<div align="right"><sub><a href="#table-of-contents">-Back to Top-</a></sub></div>

## **Attribution Chain**

- **Original DonCon2040 firmware:** (c) ravinrabbid (MIT License)
- **HIDtaiko components:** (c) kasasiki3 (Apache License 2.0)
- **Custom modifications and features:** Created by KillerQ

**Copyright (c) 2025 KillerQ**

## **Legal Compliance Notice**

Any distribution of this firmware, binary or source, must include:

1. the MIT license from DonCon2040
2. the Apache 2.0 license from HIDtaiko
3. copyright notices from both upstream projects
4. attribution to contributors

Repository license files:

- [LICENSE-MIT](LICENSE-MIT)
- [LICENSE-APACHE](LICENSE-APACHE)

## **Summary**

This project is transparent about its licensing and gives full credit to the upstream projects that made it possible.

Thank you to ravinrabbid and kasasiki3 for their open-source work.

**KillerQ**

[www.ouchitaiko.com](https://www.ouchitaiko.com)

---

*OuchiTaiko Project | Smarter. Simpler. Better.*
















