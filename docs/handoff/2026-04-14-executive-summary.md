# Executive Summary — SY-1000 FloorBoard Updates
**Date:** April 14, 2026  
**Repo:** https://github.com/bsewell/SY-1000  
**Build:** 2026.04.14.191  

## For Colin — What We Built

Hi Colin — this is a summary of significant work done on the SY-1000 FloorBoard editor over the past two sessions. The changes fall into three areas: bug fixes, QML panel standardization, and a new external MIDI controller feature.

---

## 1. Bug Fixes

### Preset Selection Fix
The `getPatchChangeMsg()` function had a multiplication error — it used `patchPerBank + 1` (= 5) instead of `patchPerBank` (= 4) when computing MIDI Program Change offsets. This caused the SY-1000 to load the wrong preset for any bank beyond bank 1, with the error growing larger for higher bank numbers. User bank 25 was off by 24 patches. Fixed in one line.

### Preferences Reset on Every Build
The `Preferences` constructor reloaded `preferences.xml.dist` whenever the version number didn't match — which happened on every new build since `build.sh` increments the version. This wiped all user settings (MIDI device selections, CC controller config, etc.) on every launch of a new build. Changed to update just the version stamp while preserving all user settings.

### Settings Dialog No Longer Restarts App
Previously, saving any preference in the Settings dialog triggered a full app restart. Removed the restart — settings save and apply without interruption.

---

## 2. QML Panel Standardization

### The Problem
Multiple stomp box edit panels had inconsistent layouts — dropdowns mixed inline with knobs, different knob sizes, and blank panel bugs caused by `SyPanelBase` wrapping content in a Flickable while the panel added its own Flickable inside (double nesting = zero-height content = white box).

### The Standard Layout Pattern
Established a consistent layout recipe based on the Chorus panel, now applied across all panels:

```
Rectangle (root — NOT SyPanelBase)
  → Column
    → StompHeader (power button, title, accent color)
    → SyModeSelector (TYPE/MODE dropdown in its own selector row at x=110)
    → Flickable
      → Column (spacing: 12, topPadding: 16)
        → Flow (x: 8, width: parent.width-16, spacing: 8)
          → FilmstripKnob (knobLargeSrc, knobLarge — uniform size)
```

### Panels Fixed
- **Distortion** — TYPE dropdown moved to selector row, knobs below
- **Compressor** — TYPE dropdown moved to selector row
- **Reverb** — TYPE + sub-type in two selector rows, all knobs unified to large
- **Chorus** — Removed SyPanelBase (was causing blank panel), all dual knobs → large
- **Delay 1, Delay 2, Master Delay** — Removed SyPanelBase nested Flickable
- **Harmonizer** — Full rewrite: KEY + BPM from shared Master params, HR2 always visible (dimmed in 1-voice), user harmony as knobs
- **Parametric EQ** — Reordered to match Boss layout, removed phantom hex3 address
- **Isolator** — BAND changed from knob to combo, removed phantom BPM
- **Humanizer** — VOWEL1/2 changed from combo to knob (matches Boss), added BPM
- **Panner** — Added missing BPM knob

### Signal Chain Labels
- All `fxName` labels converted to Init Cap: "NOISE SUPPRESSOR" → "Noise Suppressor"
- Sub-labels under tiles: Init Cap via title case conversion
- DLY 1 / DLY 2: labels cleared (no "effect" default text showing)
- M DLY: shows delay type name
- Effect label font size: 9 → 10 (×ratio)

### Automated Test Suite
Created `tests/test_panel_layout.py` with `panel_layout_spec.json` — validates all QML panels against the layout spec. Tests panel structure, knob sizes, MIDI address correctness, label conventions, dropdown alignment, and CC safety. **92 tests pass, 0 failures.**

---

## 3. Launch Control XL MIDI CC Controller Support

### What It Does
External MIDI controllers (tested with Novation Launch Control XL, template 3) can now drive on-screen knobs in real-time. Turn a physical encoder → the corresponding parameter knob moves on screen → SysEx is sent to the SY-1000.

### Architecture
```
LCXL (USB, all controls on MIDI Ch 3)
  → midiCCIn (dedicated RtMidiIn port, separate from SysEx port)
    → pollCCInput() every 10ms (independent of SY-1000 connection)
      → MidiCCHandler::handleCC()
        → lookup knob by (row, column) from CC number
        → scale CC 0-127 → parameter min/max
        → ParameterBridge::setValue() → SysEx to SY-1000 + UI update
```

### LCXL3 Template Mapping
All controls on MIDI channel 3. Column determined by CC number:

| Row | Physical Control | CC Range |
|-----|-----------------|----------|
| 0 | Top encoders | CC 13–20 |
| 1 | Middle encoders | CC 21–28 |
| 2 | Bottom encoders | CC 29–36 |
| 3 | Sliders | CC 5–12 |

### Key Features
- **Auto-detect**: Finds any device with "LCXL" or "Launch Control" in its name
- **Dynamic knob registration**: When you click a stompbox, C++ scans the QML visual tree (`findChildren` + `childItems`) for all visible controls with hex addresses, sorted by Y position, padded to 8 per row for LCXL grid alignment
- **Rescan on type change**: When FX TYPE or instrument type changes, knobs re-scan after 500ms delay
- **ccExclude safety**: FX TYPE and INST TYPE dropdowns are excluded from CC mapping — prevents accidentally changing the effect type and destroying the panel
- **Visibility filtering**: Only visible controls register (hidden tabs excluded)
- **Status bar**: Bottom-right shows CC activity in real-time (CC number, value, mapped parameter name)
- **Row alignment**: Visual rows on the panel align 1:1 with LCXL encoder rows (42px Y-position tolerance)

### New Files
| File | Purpose |
|------|---------|
| `src/midiCCHandler.h/cpp` | CC-to-parameter mapping engine |
| `tests/panel_layout_spec.json` | Panel layout specification |
| `tests/test_panel_layout.py` | Automated layout tests |
| `~/.claude/skills/midi-cc-mapping/` | Claude skill for CC debugging |
| `~/.claude/skills/sy1000-debug/` | Claude skill for app debugging |

### Modified Files (MIDI)
- `src/midiIO.h/cpp` — Added dedicated CC input port + pollCCInput
- `src/parameterBridge.h/cpp` — Knob registration, visual tree scanning, requestRescan
- `src/editWindow.h/cpp` — getQmlRoot() accessor
- `src/floorBoard.h/cpp` — CC handler wiring, knob scan on panel switch
- `src/mainWindow.h/cpp` — CC status bar label
- `src/Preferences.cpp` — Version mismatch fix
- `src/preferencesPages.h/cpp` — CC controller device dropdown
- `qml/FilmstripKnob.qml` — CC knob registration hook (removed — C++ scan used instead)
- `qml/Fx1Panel.qml, Fx2Panel.qml, Fx3Panel.qml` — ccExclude + requestRescan on type change
- `qml/InstrumentPanel.qml` — ccExclude + requestRescan

---

## Summary

| Area | Before | After |
|------|--------|-------|
| Preset selection | Wrong patch for bank >1 | Correct |
| Settings save | App restarts | No restart |
| Preferences | Reset every build | Preserved |
| Panel layout | Inconsistent, some blank | Standardized, all render |
| Knob sizes | Mixed large/small | Uniform large |
| Labels | ALL CAPS / lowercase | Init Cap |
| External controller | None | LCXL CC mapping with auto-detect |
| Tests | None | 92 automated layout tests |
