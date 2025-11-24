# Development Workflow & Troubleshooting Guide

## Workflow: How Changes Get from Claude to Your PC

### What Claude Does:
1. Makes changes to the code (in the Linux environment)
2. Commits the changes with descriptive messages
3. Pushes to a `claude/*` branch on GitHub
4. Reminds you to merge the PR

### What You Need to Do:
1. **Merge the PR on GitHub:**
   - Go to https://github.com/ouchitaikoproject/OuchiTaikoProject/pulls
   - Click "Compare & pull request" on the yellow banner for the `claude/*` branch
   - Review the changes (optional)
   - Click "Merge pull request"
   - Click "Confirm merge"

2. **Sync to Your PC:**
   - Open **GitHub Desktop**
   - Click "Fetch origin" to check for updates
   - Click "Pull origin" to download the changes
   - The changes are now on your PC!

3. **Compile in VS Code:**
   - Open the project folder in VS Code
   - Press `F7` or `Ctrl+Shift+P` → "CMake: Build"
   - Find the compiled `.uf2` file in the `build` folder
   - Flash to your Pico

---

## Troubleshooting: CMake Configuration Errors

### Problem: "nmake" Error or "no such file or directory"

**Symptoms:**
```
CMake Error at CMakeLists.txt:28 (project):
  Running
    'nmake' '-?'
  failed with:
    no such file or directory
```

### Solution:

#### Step 1: Set PICO_SDK_PATH
1. In VS Code, press `Ctrl+Shift+P`
2. Type "Preferences: Open Settings (JSON)"
3. Add these lines (use forward slashes `/` not backslashes `\`):

```json
{
    "cmake.configureEnvironment": {
        "PICO_SDK_PATH": "C:/Users/mattb/.pico-sdk/sdk/2.2.0"
    },
    "cmake.generator": "Ninja"
}
```

4. Save the file

#### Step 2: Clean and Reconfigure
1. Close VS Code completely
2. Go to your project folder: `Documents/GitHub/OuchiTaikoProject`
3. Delete the `build` folder (if it exists)
4. Reopen VS Code
5. Open the project folder
6. Press `Ctrl+Shift+P`
7. Type "CMake: Configure" and run it

#### Step 3: If Configuration Hangs
- If the blue "Configuring project" bar runs forever:
  1. Click "Cancel" in the notification
  2. Close the notification
  3. Press `F7` to build manually

### Expected Success Output:
```
[cmake] -- Configuring done (3.1s)
[cmake] -- Generating done (0.5s)
[cmake] -- Build files have been written to: C:/Users/mattb/OneDrive/Documents/GitHub/OuchiTaikoProject/build
```

---

## VS Code Setup: Opening the Project

### Correct Way to Open:
1. File → Open Folder
2. Navigate to: `Documents/GitHub/`
3. Click **once** on `OuchiTaikoProject` (highlight it, don't go inside)
4. Click "Select Folder" or "Use Folder"

### What You Should See:
- `.git` folder in the file explorer
- `src/` folder
- `include/` folder
- `CMakeLists.txt` at the root level

### Wrong Folder:
❌ Don't open `OuchiTaikoProject-main` (that's an old backup)

---

## Building the Project

### Quick Build:
- Press `F7` (fastest method)

OR

- Press `Ctrl+Shift+P`
- Type "CMake: Build"
- Select it

### Build Output:
You should see:
```
[build] Build finished with exit code 0
```

### Finding Your Firmware:
- Location: `Documents/GitHub/OuchiTaikoProject/build/`
- File: `OuchiTaikoProject.uf2`
- This is what you flash to the Pico

---

## Flashing Firmware to Pico

1. Hold the **BOOTSEL** button on the Pico
2. Plug in the USB cable (while holding BOOTSEL)
3. Release BOOTSEL
4. The Pico appears as a USB drive (RPI-RP2)
5. Copy `OuchiTaikoProject.uf2` to the drive
6. The Pico will automatically reboot with the new firmware

---

## Quick Reference

### VS Code Shortcuts:
- `F7` - Build project
- `Ctrl+Shift+P` - Command palette
- `Ctrl+Shift+G` - Toggle Source Control panel

### Important Paths:
- Project folder: `Documents/GitHub/OuchiTaikoProject`
- Pico SDK: `C:/Users/mattb/.pico-sdk/sdk/2.2.0`
- Build output: `Documents/GitHub/OuchiTaikoProject/build/`
- Firmware file: `OuchiTaikoProject.uf2`

### GitHub:
- Repository: https://github.com/ouchitaikoproject/OuchiTaikoProject
- Pull requests: https://github.com/ouchitaikoproject/OuchiTaikoProject/pulls

---

## Common Issues

### Issue: Source Control Panel Appears
**Solution:** Press `Ctrl+Shift+G` to toggle it off, or click the X to close it.

### Issue: Configuration Takes Forever
**Solution:** Click "Cancel" and build manually with `F7`.

### Issue: Build Folder Exists but Configuration Fails
**Solution:** Delete the `build` folder and try again.

### Issue: Changes Not Showing on PC
**Solution:** Open GitHub Desktop and click "Pull origin" to sync.

---

*Last updated: 2025-11-24*
