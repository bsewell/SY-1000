# CC Input Preferences — Debug Failure Log

## Date: 2026-04-11

## Problem
User cannot set CC Input device in FloorBoard preferences. Multiple attempts failed.

## Root Cause Chain (each fix exposed the next layer)

### Attempt 1: Set preference in file manually
- **Action**: Edited `preferences.xml` to set `CCIn device="0" name="MIDI Out"`
- **Result**: App overwrote it on launch because version number changed
- **Root cause**: `Preferences::Preferences()` constructor reloads defaults when version mismatches

### Attempt 2: Fix version reset
- **Action**: Changed constructor to stamp version instead of resetting all prefs
- **Result**: Preference survived launch but selecting CC device in dialog caused app to close
- **Root cause**: `mainWindow::settings()` calls `restart()` after ANY preference change (by design)

### Attempt 3: Fix CC save on OK
- **Action**: Added CCIn to the save block in `mainWindow::settings()`
- **Result**: User never reaches OK button — app closes when selecting CC device in dropdown
- **Root cause**: `updateDevice()` runs every 1s, calls `ccInCombo->clear()`, race condition with dropdown interaction

### Attempt 4: Block signals during rebuild
- **Action**: Added `blockSignals(true)` and only rebuild when device count changes
- **Result**: Still closes — the `midiInCombo` and `midiOutCombo` also get rebuilt every 1s and have `highlighted` signal connected, possibly triggering cascading device changes

### Attempt 5: Set preference in file, don't touch dialog
- **Action**: Manually set CCIn in preferences.xml, don't open preferences dialog
- **Result**: CCIn set correctly in file but `pollCCInput()` uses static resolution that may not find the port

## What We Learned

1. **The preferences dialog is fragile** — it rebuilds all combos every 1 second via `updateDevice()` timer, creating race conditions with user interaction
2. **The app restarts on ANY preference change** — `restart()` in `mainWindow::settings()` is a nuclear option inherited from the original Gumtown codebase
3. **Version-stamped preferences are brittle** — every build increment was wiping user settings
4. **Fixing symptoms instead of root causes wastes time** — we should have bypassed the preferences system entirely after the first failure

## Correct Approach (not yet implemented)

**Don't use the preferences dialog for CC input at all.** Instead:
1. Auto-detect CC input ports by scanning for known device names at runtime
2. Let the user select CC input from within the QML CC Map panel (which we already built)
3. Store CC input device in `cc_mappings.json` (which has its own persistence, separate from preferences.xml)
4. Or: add a simple in-app dropdown that doesn't trigger a restart

## Files Involved
- `src/Preferences.cpp` — constructor, version check
- `src/preferencesPages.cpp` — `updateDevice()`, `ccInDeviceChanged()`
- `src/preferencesDialog.cpp` — OK button triggers accept()
- `src/mainWindow.cpp` — `settings()` save block, `restart()` call
- `src/midiIO.cpp` — `pollCCInput()`, `getCCInDevice()`
