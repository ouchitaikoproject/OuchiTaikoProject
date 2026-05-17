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

readme_text = README_SRC.read_text(encoding="utf-8", errors="replace")
(SITE_SRC / "README.md").write_text(readme_text, encoding="utf-8")

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
    "These files are synced automatically from the repository `docs/` folder when GitHub Pages builds.",
    "",
]

if html_files:
    lines.extend(["## HTML Tools", ""])
    for name in html_files:
        label = name.replace(".html", "").replace("-", " ").title()
        lines.append(f"- [{label}]({name})")
    lines.append("")

if md_files:
    lines.extend(["## Notes", ""])
    for name in md_files:
        label = name.replace(".md", "").replace("-", " ").title()
        lines.append(f"- [{label}]({name})")
    lines.append("")

(TOOLS_DST / "index.md").write_text("\n".join(lines), encoding="utf-8")


def normalize_anchor_text(text: str) -> str:
    text = text.strip()
    # Docsify heading ids for major numbered sections behave better without the leading chapter number.
    text = re.sub(r"^\d+(?:\.\d+)?[.:]\s*", "", text)
    return text


def slugify(text: str) -> str:
    text = normalize_anchor_text(text).lower()
    text = re.sub(r"[^\w\s-]", "", text)
    text = re.sub(r"\s+", "-", text)
    return text


sidebar_lines = ["- [Build Guide](/)"]

guide_nav = [
    ("Key Features", "Key Features", 0),
    ("World‑First Innovations", "World‑First Innovations", 1),
    ("Complete Hardware & Features", "Complete Hardware & Features", 1),
    ("1. Project Overview", "1. Project Overview", 0),
    ("2. Parts List for Electronics", "2. Parts List for Electronics", 0),
    ("3. Parts List for Hardware", "3. Parts List for Hardware", 0),
    ("Required Tools & Supplies", "Required Tools & Supplies", 1),
    ("Hardware Parts List", "Hardware Parts List", 1),
    ("4. Assemble the Control PCB", "4. Assemble the Control PCB", 0),
    ("Control PCB Overview", "Control PCB Overview", 1),
    ("Recommended Assembly Order", "Recommended Assembly Order", 1),
    ("Inspect Your Work", "Inspect Your Work", 1),
    ("5. Build the Drum", "5: Build the Drum", 0),
    ("5.1 Prepare the Wood", "5.1: Prepare the Wood", 1),
    ("5.2 Assemble the Drum Structure", "5.2: Assemble the Drum Structure", 1),
    ("5.3 Assemble the Rubber Isolators", "5.3: Assemble The Rubber Isolators", 1),
    ("5.4 Print Sensor Housings", "5.4: Print Sensor Housings", 1),
    ("5.5 Assemble Sensor Electronics", "5.5: Assemble Sensor Electronics", 1),
    ("5.6 Mounting Hardware and Faceplates", "5.6: Mounting Hardware And Faceplates", 1),
    ("6. Control Box Connection", "6: Control Box Connection", 0),
    ("6.1 Floor Stand Mode", "6.1: Floor Stand Mode", 1),
    ("6.2 Table-Top Mode", "6.2: Table-Top Mode", 1),
    ("7. Flash the Firmware", "7: Flash the Firmware", 0),
    ("Step 1: Enter Bootloader Mode", "Step 1: Enter Bootloader Mode", 1),
    ("Step 2: Wipe the Board", "Step 2: Wipe the Board", 1),
    ("Step 3: Flash the Firmware", "Step 3: Flash the Firmware", 1),
    ("8. Calibration & Settings", "8: Calibration & Settings", 0),
    ("Quick Hardware Check", "Quick Hardware Check", 1),
    ("Guided Calibration", "Guided Calibration", 1),
    ("Manual Threshold Adjustment", "Manual Threshold Adjustment", 1),
    ("Controller Mode Selection", "Controller Mode Selection", 1),
    ("9. Files & Downloads", "9: Files & Downloads", 0),
    ("10. Basic Troubleshooting", "10: Basic Troubleshooting", 0),
    ("11. Menu System Reference", "11: Menu System Reference", 0),
    ("Quick Access", "Quick Access", 1),
    ("Navigation Controls", "Navigation Controls", 1),
    ("Main Menu", "Main Menu", 1),
    ("Drum Tuning", "Drum Tuning", 1),
    ("Advanced", "Advanced", 1),
    ("Idle Screen", "Idle Screen", 1),
    ("12. About", "12: About", 0),
    ("Closing Thoughts", "Closing Thoughts", 1),
    ("13. Copyright Information", "13: Copyright Information", 0),
    ("Attribution Chain", "Attribution Chain", 1),
    ("Legal Compliance Notice", "Legal Compliance Notice", 1),
    ("Summary", "Summary", 1),
]

for label, anchor_title, indent_level in guide_nav:
    indent = "  " * indent_level
    sidebar_lines.append(f"{indent}- [{label}](#{slugify(anchor_title)})")

(SITE_SRC / "_sidebar.md").write_text("\n".join(sidebar_lines) + "\n", encoding="utf-8")
