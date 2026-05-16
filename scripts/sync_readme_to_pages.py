from pathlib import Path
import shutil
import re


ROOT = Path(__file__).resolve().parents[1]
SITE_SRC = ROOT / "site_src"
TOOLS_DST = SITE_SRC / "tools"
IMAGES_DST = SITE_SRC / "images"
DOCS_SRC = ROOT / "docs"
IMAGES_SRC = ROOT / "images"

README_CANDIDATES = [ROOT / "README.md", ROOT / "readme.md"]
README_SRC = next((path for path in README_CANDIDATES if path.exists()), None)
if README_SRC is None:
    raise FileNotFoundError("Could not find README.md or readme.md in repo root.")

SITE_SRC.mkdir(parents=True, exist_ok=True)
(SITE_SRC / "CNAME").write_text("www.ouchitaiko.com\n", encoding="utf-8")
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


def slugify(text: str) -> str:
    text = text.strip().lower()
    text = re.sub(r"[^\w\s-]", "", text)
    text = re.sub(r"\s+", "-", text)
    return text


sidebar_lines = ["- [Build Guide](/)"]
major_sections = {
    "worldâ€‘first innovations",
    "complete hardware & features",
    "required tools & supplies",
    "hardware parts list",
    "control pcb overview",
    "recommended assembly order",
    "inspect your work",
    "quick hardware check",
    "guided calibration",
    "manual threshold adjustment",
    "controller mode selection",
    "quick access",
    "main menu",
    "drum tuning",
    "advanced",
    "closing thoughts",
    "attribution chain",
    "legal compliance notice",
    "summary",
}

for line in readme_text.splitlines():
    match = re.match(r"^(##)\s+(.*)$", line.strip())
    if not match:
        continue

    title = match.group(2).strip()
    normalized_title = title.replace("*", "").strip().lower()

    if normalized_title not in major_sections:
        continue

    sidebar_lines.append(f"- [{title}](#{slugify(title)})")

sidebar_lines.extend(
    [
        "- [Calibration Tools](/tools/)",
        "  - [Slim Calibrate](/tools/ouchitaiko-slim-calibrate.html)",
        "  - [Live Calibrate](/tools/ouchitaiko-live-calibrate.html)",
    ]
)

(SITE_SRC / "_sidebar.md").write_text("\n".join(sidebar_lines) + "\n", encoding="utf-8")
