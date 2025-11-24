# Download Files Inventory

This folder contains all the downloadable files referenced in the main README.

## ✅ Available Files

### 3D Printing Files (STL)
1. **OuchiTaiko Project - Bachi Stick Holder.stl** - Holder for drumstick storage
2. **OuchiTaiko Project - Housing.stl** - Sensor housing (print 4x)
3. **OuchiTaiko Project - TRS Barrel Connector Mount.stl** - Cable connector mount
4. **OuchiTaiko Project Control Box Base.stl** - Control box enclosure base
5. **OuchiTaiko Project Control Box Lid.stl** - Control box enclosure lid with button/display cutouts

### Laser Cutting Files (SVG)
1. **OuchiTaiko Project all Laser SVG Files.svg** - All drum face plates and base plate templates for laser cutting
2. **OuchiTaiko Project SVG LightBurn Project File.lbrn2** - LightBurn project file for laser cutting

### Firmware Files
1. **universal_flash_nuke.uf2** - Board wipe file (flash before uploading firmware)

### Archive Files
1. **KillerQsOuchiTaikoProjectSTLandSVGFiles.zip** - All STL and SVG files packaged together
2. **KillerQsOuchiTaikoProjectFlashFiles.zip** - Firmware flash files

## ⚠️ Missing Files (Need to be Added)

### Critical - Firmware
- **KillerQsOuchiTaikoFirmware.uf2** - Main controller firmware (MISSING!)
  - This is referenced in the README sections 7 (Flash The Firmware) and 9 (Files & Downloads)
  - Users cannot complete the build without this file
  - **Action needed**: Build the firmware from OuchiTaikoProject-main source code or provide pre-compiled binary

### Documentation
- **KillerQsOuchiTaikoProjectCircuitSchematic.pdf** - Circuit schematic in PDF format
  - Currently exists as schematic.jpg in images/Pictures/
  - **Action needed**: Convert schematic.jpg to PDF or create detailed schematic using KiCad/EasyEDA

## 📝 Notes

- All STL files should be printed with PLA filament
- STL print settings: 0.2mm layer height, 40% Gyroid infill (sensor housings) or 20% (control box)
- SVG files are scaled correctly - verify 6mm mounting holes before cutting
- The neoprene disc template is included in the main SVG file

## 🔗 Referenced in README

These files are referenced at the following locations in the main README:

- **STL/SVG Archive**: Lines 569, 722, 742, 1330
- **Flash Files Archive**: Lines 1111, 1332
- **Circuit Schematic PDF**: Line 1331
