import re

# Read the file
with open('README.md', 'r', encoding='utf-8') as f:
    content = f.read()

# Dictionary of all step headers that need to be made bold and underlined
step_headers = {
    '### **A1.** Mount the XIAO RP2040 to a blank circuit board': '### **<u>A1. Mount the XIAO RP2040 to a blank circuit board</u>**',
    '### **A2.** Connect the GND pin on the XIAO RP2040 to the common ground rail of the protoboard': '### **<u>A2. Connect the GND pin on the XIAO RP2040 to the common ground rail of the protoboard</u>**',
    '### **A3.** Connect the VCC 3.3V pin on the XIAO RP2040 to the common VCC power rail of the protoboard': '### **<u>A3. Connect the VCC 3.3V pin on the XIAO RP2040 to the common VCC power rail of the protoboard</u>**',
    '### **A4.** Pin "P26" (Left Ka):': '### **<u>A4. Pin "P26" (Left Ka):</u>**',
    '### **A8.** Mount the MCP23017 GPIO Expander on a blank board': '### **<u>A8. Mount the MCP23017 GPIO Expander on a blank board</u>**',
    '### **A9.** Connect Power': '### **<u>A9. Connect Power</u>**',
    '### **A10.** Connect SDA': '### **<u>A10. Connect SDA</u>**',
    '### **A11.** Connect SCL': '### **<u>A11. Connect SCL</u>**',
    '### **A12.** Mount the OLED display': '### **<u>A12. Mount the OLED display</u>**',
    '### **A13.** Connect SDA to display': '### **<u>A13. Connect SDA to display</u>**',
    '### **A14.** Connect SCL to display': '### **<u>A14. Connect SCL to display</u>**',
    '### **A15.** Connect VCC to display': '### **<u>A15. Connect VCC to display</u>**',
    '### **A16.** Connect GND to display': '### **<u>A16. Connect GND to display</u>**',
    '### **A17.** Solder navigation buttons': '### **<u>A17. Solder navigation buttons</u>**',
    '### **A18.** Connect button grounds': '### **<u>A18. Connect button grounds</u>**',
    '### **A19.** Connect button signals': '### **<u>A19. Connect button signals</u>**',
    '### **B1.** Cut all MDF wood pieces per SVG templates': '### **<u>B1. Cut all MDF wood pieces per SVG templates</u>**',
    '### **B2.** Sand smooth as needed': '### **<u>B2. Sand smooth as needed</u>**',
    '### **B3.** Glue rear base plates': '### **<u>B3. Glue rear base plates</u>**',
    '### **B4.** Glue drum panels': '### **<u>B4. Glue drum panels</u>**',
    '### **B5.** Router/sand Ka rim edges': '### **<u>B5. Router/sand Ka rim edges</u>**',
    '### **B6.** Drill holes for threaded inserts': '### **<u>B6. Drill holes for threaded inserts</u>**',
    '### **B7.** Chamfer holes': '### **<u>B7. Chamfer holes</u>**',
    '### **B8.** Install M6 threaded inserts': '### **<u>B8. Install M6 threaded inserts</u>**',
    '### **B9.** Install M3 threaded inserts': '### **<u>B9. Install M3 threaded inserts</u>**',
    '### **C1.** Cut nylon bolt head': '### **<u>C1. Cut nylon bolt head</u>**',
    '### **C2.** Apply Loctite to isolator': '### **<u>C2. Apply Loctite to isolator</u>**',
    '### **C3.** Install bolt in isolator': '### **<u>C3. Install bolt in isolator</u>**',
    '### **C4.** Apply Loctite to drum plate inserts': '### **<u>C4. Apply Loctite to drum plate inserts</u>**',
    '### **C5.** Install isolator assemblies': '### **<u>C5. Install isolator assemblies</u>**',
    '### **D1.** Print 4 sets of complete sensor housings': '### **<u>D1. Print 4 sets of complete sensor housings</u>**',
    '### **E1.** Cut neoprene discs': '### **<u>E1. Cut neoprene discs</u>**',
    '### **E2.** Glue neoprene to housing': '### **<u>E2. Glue neoprene to housing</u>**',
    '### **E3.** Strip Siamese wire': '### **<u>E3. Strip Siamese wire</u>**',
    '### **E4.** Solder to TRS jack': '### **<u>E4. Solder to TRS jack</u>**',
    '### **E5.** Solder to piezo': '### **<u>E5. Solder to piezo</u>**',
    '### **E6.** Glue piezo to neoprene': '### **<u>E6. Glue piezo to neoprene</u>**',
    '### **E7.** Assemble housing': '### **<u>E7. Assemble housing</u>**',
    '### **E8.** Mount housings to drum': '### **<u>E8. Mount housings to drum</u>**',
    '### **F1.** Mark mounting holes': '### **<u>F1. Mark mounting holes</u>**',
    '### **F2.** Drill mounting holes': '### **<u>F2. Drill mounting holes</u>**',
    '### **F3.** Install mounting bolts': '### **<u>F3. Install mounting bolts</u>**',
    '### **F4.** Attach speaker mount': '### **<u>F4. Attach speaker mount</u>**',
    '### **F5.** Assemble drum structure': '### **<u>F5. Assemble drum structure</u>**',
    '### **F6.** Route sensor wires': '### **<u>F6. Route sensor wires</u>**',
    '### **F7.** Secure drum faces': '### **<u>F7. Secure drum faces</u>**',
    '### **F8.** Attach TRS barrel mounts': '### **<u>F8. Attach TRS barrel mounts</u>**',
    '### **F9.** Connect sensor cables': '### **<u>F9. Connect sensor cables</u>**',
    '### **G1.** Connect sensor cables to circuit': '### **<u>G1. Connect sensor cables to circuit</u>**',
    '### **G2.** Mount TRS jacks to control box': '### **<u>G2. Mount TRS jacks to control box</u>**',
    '### **G3.** Mount USB-C to USB-A adapter': '### **<u>G3. Mount USB-C to USB-A adapter</u>**',
    '### **G4.** Connect USB hub': '### **<u>G4. Connect USB hub</u>**',
    '### **G5.** Connect XIAO to USB hub': '### **<u>G5. Connect XIAO to USB hub</u>**',
    '### **G6.** Connect USB DAC': '### **<u>G6. Connect USB DAC</u>**',
    '### **G7.** Connect DAC to extension cable': '### **<u>G7. Connect DAC to extension cable</u>**',
    '### **G8.** Mount audio jack to control box': '### **<u>G8. Mount audio jack to control box</u>**',
    '### **G9.** Mount OLED/button board': '### **<u>G9. Mount OLED/button board</u>**',
    '### **G10.** Wire management': '### **<u>G10. Wire management</u>**',
    '### **H1.** Mount bracket to speaker stand': '### **<u>H1. Mount bracket to speaker stand</u>**',
    '### **H2.** Adjust height/angle': '### **<u>H2. Adjust height/angle</u>**',
    '### **H3.** Add drum cover': '### **<u>H3. Add drum cover</u>**',
}

# Apply all step header replacements
for old, new in step_headers.items():
    content = content.replace(old, new)

# Now standardize "Back to Top" links - place them right after each major section header
# Remove all existing "Back to Top" divs that appear in various places
content = re.sub(r'<div align="right"><sub><a href="#table-of-contents">↑ Back to Top</a></sub></div>\s*\n+', '', content)

# Add standardized "Back to Top" after each major section (##)
# Pattern: Find section headers, add back to top link right after
sections_to_add_back_to_top = [
    ('## 1: Project Overview', '## 1: Project Overview\n\n<div align="right"><sub><a href="#table-of-contents">↑ Back to Top</a></sub></div>'),
    ('## 2: Parts List for Electronics', '## 2: Parts List for Electronics\n\n<div align="right"><sub><a href="#table-of-contents">↑ Back to Top</a></sub></div>'),
    ('## 3: Parts List for Hardware', '## 3: Parts List for Hardware\n\n<div align="right"><sub><a href="#table-of-contents">↑ Back to Top</a></sub></div>'),
    ('## 4: Build the Circuit', '## 4: Build the Circuit\n\n<div align="right"><sub><a href="#table-of-contents">↑ Back to Top</a></sub></div>'),
    ('## 5: Build the Drum', '## 5: Build the Drum\n\n<div align="right"><sub><a href="#table-of-contents">↑ Back to Top</a></sub></div>'),
    ('## 6: Control Box', '## 6: Control Box\n\n<div align="right"><sub><a href="#table-of-contents">↑ Back to Top</a></sub></div>'),
    ('## 7: Flash The Firmware', '## 7: Flash The Firmware\n\n<div align="right"><sub><a href="#table-of-contents">↑ Back to Top</a></sub></div>'),
    ('## 8: Calibration & Setup', '## 8: Calibration & Setup\n\n<div align="right"><sub><a href="#table-of-contents">↑ Back to Top</a></sub></div>'),
    ('## 9: Files & Downloads', '## 9: Files & Downloads\n\n<div align="right"><sub><a href="#table-of-contents">↑ Back to Top</a></sub></div>'),
    ('## 10: Basic Troubleshooting', '## 10: Basic Troubleshooting\n\n<div align="right"><sub><a href="#table-of-contents">↑ Back to Top</a></sub></div>'),
    ('## 11: About', '## 11: About\n\n<div align="right"><sub><a href="#table-of-contents">↑ Back to Top</a></sub></div>'),
    ('## 12: Copyright Information', '## 12: Copyright Information\n\n<div align="right"><sub><a href="#table-of-contents">↑ Back to Top</a></sub></div>'),
]

for old_pattern, new_pattern in sections_to_add_back_to_top:
    # Only replace if not already there
    if new_pattern not in content:
        content = content.replace(old_pattern, new_pattern)

# Add Back to Top links after major subsection headers
subsection_patterns = [
    ('### A: XIAO Board Trigger Circuit', '### A: XIAO Board Trigger Circuit\n\n<div align="right"><sub><a href="#table-of-contents">↑ Back to Top</a></sub></div>'),
    ('### I2C Control Board Circuit Mounting and Power', '### I2C Control Board Circuit Mounting and Power\n\n<div align="right"><sub><a href="#table-of-contents">↑ Back to Top</a></sub></div>'),
    ('### I2C OLED Display Circuit', '### I2C OLED Display Circuit\n\n<div align="right"><sub><a href="#table-of-contents">↑ Back to Top</a></sub></div>'),
    ('### Navigation/Gamepad Buttons', '### Navigation/Gamepad Buttons\n\n<div align="right"><sub><a href="#table-of-contents">↑ Back to Top</a></sub></div>'),
    ('### B: Prepare the Wood', '### B: Prepare the Wood\n\n<div align="right"><sub><a href="#table-of-contents">↑ Back to Top</a></sub></div>'),
    ('### Assemble the Drum Structure', '### Assemble the Drum Structure\n\n<div align="right"><sub><a href="#table-of-contents">↑ Back to Top</a></sub></div>'),
    ('### C: Install Rubber Isolators', '### C: Install Rubber Isolators\n\n<div align="right"><sub><a href="#table-of-contents">↑ Back to Top</a></sub></div>'),
    ('### D: Print Housings', '### D: Print Housings\n\n<div align="right"><sub><a href="#table-of-contents">↑ Back to Top</a></sub></div>'),
    ('### E: Assemble Sensors', '### E: Assemble Sensors\n\n<div align="right"><sub><a href="#table-of-contents">↑ Back to Top</a></sub></div>'),
    ('### F: Mounting Hardware Assembly', '### F: Mounting Hardware Assembly\n\n<div align="right"><sub><a href="#table-of-contents">↑ Back to Top</a></sub></div>'),
    ('### G: Connect Sensors To The Circuit', '### G: Connect Sensors To The Circuit\n\n<div align="right"><sub><a href="#table-of-contents">↑ Back to Top</a></sub></div>'),
    ('### Final Connections and Housing', '### Final Connections and Housing\n\n<div align="right"><sub><a href="#table-of-contents">↑ Back to Top</a></sub></div>'),
    ('### H: Floor Stand', '### H: Floor Stand\n\n<div align="right"><sub><a href="#table-of-contents">↑ Back to Top</a></sub></div>'),
    ('### Extended Drum Setting Features', '### Extended Drum Setting Features\n\n<div align="right"><sub><a href="#table-of-contents">↑ Back to Top</a></sub></div>'),
    ('### Display Tips While Playing', '### Display Tips While Playing\n\n<div align="right"><sub><a href="#table-of-contents">↑ Back to Top</a></sub></div>'),
]

for old_pattern, new_pattern in subsection_patterns:
    if new_pattern not in content:
        content = content.replace(old_pattern, new_pattern)

# Write the updated content
with open('README.md', 'w', encoding='utf-8') as f:
    f.write(content)

print("README.md has been updated successfully!")
print("- All step headers (A1, A2, B1, etc.) are now bold and underlined")
print("- All 'Back to Top' links have been standardized and placed consistently")
