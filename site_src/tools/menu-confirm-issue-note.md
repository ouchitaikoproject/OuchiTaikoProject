## Menu A-Button Issue (Resolved)

### Symptom
- `A` appeared to work on some menu sections (`Menu` pages like `Advanced`, `Drum Tuning`) but not others (notably `About` and other informational `Selection` entries).
- Behavior looked selective and inconsistent.

### Root Cause
- In `Menu::update()`, `Descriptor::Type::Selection` treated confirm (`A`) as `gotoParent(false)` for most cases.
- Informational selection entries (where action is `Action::None`, such as `About`) should not have been auto-closed by `A`.
- Result: pressing `A` could enter a page and immediately trigger back behavior, making it appear like `A` did not work.

### Fix
- Updated `Selection` confirm handling:
  - If selected entry action is `Action::None`, ignore `A` and require `B` for back.
  - Keep existing explicit behavior for special pages (`TaikoTantrum`, `DeviceMode`).

### Build Marker
- First build with this fix: `Build 68 Onyx`.

