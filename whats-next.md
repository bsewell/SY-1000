<original_task>
Multiple UI polish tasks for the SY-1000 QML editor panels:
1. Fix top banner (header) to match Boss Tone Studio reference — power icon style, color, VARIATION dropdown, sizing
2. Fix FX Chorus showing empty when FX TYPE = Chorus in FX1/FX2/FX3 panels
3. Fix signal chain layout alignment for FX stompboxes
4. Match left side (patch browser) sizing to reference
</original_task>

<work_completed>
## Header Fixes (StompHeader.qml + InstrumentPanel.qml) — DONE
- Power button → Unicode ⏻ icon, dark muted header, VARIATION dropdown, 40px height
- InstrumentPanel header rows rewritten (INST TYPE left-aligned, tabs right-aligned)

## Chorus Fix (ChorusView.qml + CePanel.qml) — DONE
- ChorusView rewritten with correct hex3 addresses, always-visible main row, dual rows for mode>=3
- CePanel updated with same pattern

## Signal Chain FX Alignment — RESOLVED (no bug found)
- Investigation: all FX tiles use identical 96x96 ON images, `signalCenterYOffset()` caps at 96px
- FX1/FX2/FX3 QML panels are structurally identical (same heights, spacing, layout)
- Fixed: FX1_off.png was 96x147 while FX2/FX3 were 96x145 — trimmed to 96x145 for consistency
- The 2px difference was masked by the 96px rendering cap, but now all three match exactly

## Left Side Sizing — DONE
- Reduced left panel minimum width from 150*ratio to 130*ratio in floorBoard.cpp (both setFloorBoard and setSize)
- Gives ~20px more space to the signal-chain/editor area
- Left panel still scales dynamically with window width

## Version Bump — DONE
- preferences.xml.dist bumped to 20260317.26
</work_completed>

<work_remaining>
No remaining items from the original task list. All 4 items addressed.
</work_remaining>

<critical_context>
## Boss Reference Screenshots
Located at `<local SYUI project path>`:
- `INST 1 Common.png` — Header reference (power icon, VARIATION, tab alignment)
- `Chorus.png` — FX2 chorus in STEREO2 mode (main row + dual chorus rows)
- `FX1.png`, `FX2.png`, `FX 3.png` — FX panel references
- `Normal Input.png` — Normal Input header style

## Key Architecture
- StompHeader.qml is shared by ALL stomp/effect panels — any change affects ~15+ panels
- InstrumentPanel.qml is used by INST 1/2/3 only
- ChorusView.qml is loaded by FX1/FX2/FX3 panels when FX TYPE = Chorus (typeIndex=2 in Fx1TypeData.js)
- CePanel.qml is the standalone CE/Chorus stompbox panel (separate from FX chorus)
- Complex FX types (Chorus, Delay, Harmonizer, PitchShifter) use dedicated sub-components in `qml/fx1/`

## Build/Deploy Process
```bash
# Clean QRC caches (MUST do when QML files change)
rm -f qrc_qml.cpp build/release/qrc_qml.o qrc_SY-1000FloorBoard.cpp build/release/qrc_SY-1000FloorBoard.o
# Build
make -j$(sysctl -n hw.logicalcpu)
# Deploy
cp build/packager/SY-1000FloorBoard.app/Contents/MacOS/SY-1000FloorBoard /Applications/SY-1000FloorBoard.app/Contents/MacOS/SY-1000FloorBoard
```
</critical_context>

<current_state>
## All items resolved
- Signal chain FX alignment: no bug — image inconsistency fixed
- Left panel: minimum reduced from 150 to 130
- Version bumped to 20260317.26
- Build needed to verify
</current_state>
