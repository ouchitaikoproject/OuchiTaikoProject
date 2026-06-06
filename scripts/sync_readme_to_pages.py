# -*- coding: utf-8 -*-
from pathlib import Path
import shutil
import re

ROOT = Path(__file__).resolve().parents[1]
SITE_SRC = ROOT / "site_src"
TOOLS_DST = SITE_SRC / "tools"
IMAGES_DST = SITE_SRC / "images"
DOCS_SRC = ROOT / "docs"
IMAGES_SRC = ROOT / "images"
CNAME_TEXT = "www.ouchitaiko.com\n"

README_CANDIDATES = [ROOT / "README.md", ROOT / "readme.md"]
README_SRC = next((path for path in README_CANDIDATES if path.exists()), None)
if README_SRC is None:
    raise FileNotFoundError("Could not find README.md or readme.md in repo root.")

SITE_SRC.mkdir(parents=True, exist_ok=True)
cname_path = SITE_SRC / "CNAME"
if not cname_path.exists() or cname_path.read_text(encoding="utf-8", errors="replace") != CNAME_TEXT:
    cname_path.write_text(CNAME_TEXT, encoding="utf-8")
(SITE_SRC / "index.md").unlink(missing_ok=True)

stale_styles_dir = SITE_SRC / "stylesheets"
if stale_styles_dir.exists():
    shutil.rmtree(stale_styles_dir)


def normalize_anchor_text(text: str) -> str:
    text = text.strip()
    text = text.replace("**", "").replace("`", "")
    text = re.sub(r"^\d+(?:\.\d+)?[.:)]\s*", "", text)
    return text.strip()


def slugify(text: str) -> str:
    text = normalize_anchor_text(text).lower()
    text = text.replace(chr(0x2011), "-")
    text = text.replace(chr(0x2013), "-")
    text = text.replace(chr(0x2014), "-")
    text = re.sub(r"[^\w\s-]", "", text)
    text = re.sub(r"\s+", "-", text)
    text = re.sub(r"-+", "-", text).strip("-")
    return text


def add_explicit_heading_anchors(markdown: str) -> str:
    out_lines = []
    used = set()
    for line in markdown.splitlines():
        match = re.match(r"^(#{1,6})\s+(.+?)\s*$", line)
        if match:
            heading_text = match.group(2)
            anchor = slugify(heading_text)
            if anchor and anchor not in used:
                out_lines.append(f'<a id="{anchor}"></a>')
                used.add(anchor)
        out_lines.append(line)
    return "\n".join(out_lines) + "\n"


readme_text = README_SRC.read_text(encoding="utf-8", errors="replace")
(SITE_SRC / "README.md").write_text(add_explicit_heading_anchors(readme_text), encoding="utf-8")

if IMAGES_DST.exists():
    shutil.rmtree(IMAGES_DST)
if IMAGES_SRC.exists():
    shutil.copytree(IMAGES_SRC, IMAGES_DST)

if TOOLS_DST.exists():
    shutil.rmtree(TOOLS_DST)
TOOLS_DST.mkdir(parents=True, exist_ok=True)

html_files = []
md_files = []
if DOCS_SRC.exists():
    for item in sorted(DOCS_SRC.iterdir()):
        if item.is_file():
            shutil.copy2(item, TOOLS_DST / item.name)
            if item.suffix.lower() == ".html":
                html_files.append(item.name)
            elif item.suffix.lower() == ".md":
                md_files.append(item.name)

lines = [
    "# Calibration Tools",
    "",
    "These files are synced automatically from the repository docs/ folder when GitHub Pages builds.",
    "",
]

if html_files:
    lines.extend(["## HTML Tools", ""])
    for name in html_files:
        label = name.replace(".html", "").replace("-", " ").replace("_", " ").title()
        lines.append(f"- [{label}]({name})")
    lines.append("")

if md_files:
    lines.extend(["## Notes", ""])
    for name in md_files:
        label = name.replace(".md", "").replace("-", " ").replace("_", " ").title()
        lines.append(f"- [{label}]({name})")
    lines.append("")

(TOOLS_DST / "index.md").write_text("\n".join(lines), encoding="utf-8")

sidebar_lines = ["- [Build Guide](/)"]

guide_nav = [
    ("Key Features", "Key Features and World Firsts Not Seen In Any Other Existing Projects", 0),
    ("World-First Innovations", "World-First Innovations", 1),
    ("Guided Calibration", "1) Guided Calibration", 1),
    ("ABSI", "2) Adaptive Baseline Software Intelligence (ABSI)", 1),
    ("Sensor Suspension", "3) Custom Arcade Sensor Suspension", 1),
    ("Complete Hardware & Features", "Complete Hardware & Features", 1),
    ("Standalone Testing & Display", "Standalone Testing & Display", 2),
    ("Professional Hardware", "Professional Hardware", 2),
    ("Controller Modes", "Controller Modes", 2),
    ("Zero Coding Required", "Zero Coding Required", 2),
    ("Demo Video", "Demo Video", 2),
    ("1. Project Overview", "1. Project Overview", 0),
    ("2. Parts List for Electronics", "2. Parts List for Electronics", 0),
    ("3. Parts List for Hardware", "3. Parts List for Hardware", 0),
    ("Required Tools & Supplies", "Required Tools & Supplies", 1),
    ("Hardware Parts List", "Hardware Parts List", 1),
    ("4. Assemble the Controller PCB", "4. Assemble the Controller PCB", 0),
    ("Control PCB Overview", "Control PCB Overview", 1),
    ("Recommended Assembly Order", "Recommended Assembly Order", 1),
    ("Step 1: Solder Passive Components", "Step 1: Solder Passive Components", 2),
    ("Step 2: Solder the 14 Buttons", "Step 2: Solder the 14 Buttons", 2),
    ("Step 3: Mount the OLED", "Step 3: Mount the OLED", 2),
    ("Step 4: Install the TRS Drum Input Jacks", "Step 4: Install the TRS Drum Input Jacks", 2),
    ("Step 5: Mount the RP2040-Zero", "Step 5: Mount the RP2040-Zero", 2),
    ("Inspect Your Work", "Inspect Your Work", 1),
    ("5. Build the Sensor Housings", "5: Build the Sensor Housings", 0),
    ("Print The Sensor Housings", "Print The Sensor Housings", 1),
    ("5.1 Assemble Sensor Electronics", "5.1: Assemble Sensor Electronics", 1),
    ("6. Build the Drum", "6: Build the Drum", 0),
    ("6.1 Prepare the Wood", "6.1: Prepare the Wood", 1),
    ("6.2 Assemble the Drum Structure", "6.2: Assemble the Drum Structure", 1),
    ("6.3 Assemble The Rubber Isolators", "6.3: Assemble The Rubber Isolators", 1),
    ("6.4 Mounting The Hardware And Faceplates", "6.4: Mounting The Hardware And Faceplates", 1),
    ("7. Control Box Connection", "7: Control Box Connection", 0),
    ("7.1 Floor Stand Mode", "7.1: Floor Stand Mode", 1),
    ("7.2 Table-Top Mode", "7.2: Table-Top Mode", 1),
    ("8. Flash the Firmware", "8: Flash the Firmware", 0),
    ("Step 1: Enter Bootloader Mode", "Step 1: Enter Bootloader Mode", 1),
    ("Step 2: Wipe the Board", "Step 2: Wipe the Board", 1),
    ("Step 3: Flash the Firmware", "Step 3: Flash the Firmware", 1),
    ("9. Calibration & Settings", "9: Calibration & Settings", 0),
    ("Quick Hardware Check", "Quick Hardware Check", 1),
    ("Guided Calibration", "Guided Calibration", 1),
    ("Manual Threshold Adjustment", "Manual Threshold Adjustment", 1),
    ("Controller Mode Selection", "Controller Mode Selection", 1),
    ("10. Files & Downloads", "10: Files & Downloads", 0),
    ("11. Basic Troubleshooting", "11: Basic Troubleshooting", 0),
    ("12. Menu System Reference", "12: Menu System Reference", 0),
    ("Quick Access", "Quick Access", 1),
    ("Navigation Controls", "Navigation Controls", 1),
    ("Main Menu", "Main Menu", 1),
    ("Drum Tuning", "Drum Tuning", 1),
    ("Advanced", "Advanced", 1),
    ("Idle Screen", "Idle Screen", 1),
    ("13. About", "13: About", 0),
    ("Closing Thoughts", "Closing Thoughts", 1),
    ("14. Copyright Information", "14: Copyright Information", 0),
    ("Attribution Chain", "Attribution Chain", 1),
    ("Legal Compliance Notice", "Legal Compliance Notice", 1),
    ("Summary", "Summary", 1),
]

for label, anchor_title, indent_level in guide_nav:
    indent = "  " * indent_level
    sidebar_lines.append(f"{indent}- [{label}](/?id={slugify(anchor_title)})")

(SITE_SRC / "_sidebar.md").write_text("\n".join(sidebar_lines) + "\n", encoding="utf-8")


