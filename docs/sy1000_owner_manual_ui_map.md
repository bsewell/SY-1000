# SY-1000 Owner's Manual to UI Mapping (Page-by-Page)

This document maps each page of the official **SY-1000 Owner's Manual** (`SY-1000_eng01_W.pdf`) to the SY-1000 FloorBoard user interface and implementation files.

## Scope

- Source manual: `docs/manuals/SY-1000_eng01_W.pdf`
- Source text extraction: `docs/manuals/text/SY-1000_eng01_plain.txt`
- Coverage: Owner's Manual pages 1 through 21 (page 22+ are blank in extraction)
- Date mapped: 2026-03-07 — re-check `Partial` rows after significant C++ or QML migrations

## Mapping Key

- `Implemented`: Exposed in current UI/workflow.
- `Partial`: Some parts exposed; some steps remain hardware-first or in other tools.
- `N/A (Hardware/Safety)`: Manual content is physical hardware guidance or safety text, not an editor function.

## Page Crosswalk

| Manual Page | Manual Topic | UI Mapping (FloorBoard) | Primary Code Areas | Status |
|---|---|---|---|---|
| 1 | Manual overview + links to PDF manuals | Documented in local project docs/manuals bundle | `docs/sy1000_manuals.md` | Implemented (docs) |
| 2 | Table of contents | Reference index only | `docs/sy1000_manuals.md`, this file | Implemented (docs) |
| 3 | Hardware connections (GK IN, guitar input, power, send/return) | No direct wiring wizard; app assumes connected device and presents editor data | `menuPage.cpp` (device mode triggers), `midiIO.cpp`, `SysxIO.cpp` | Partial |
| 4 | Output/MIDI/CTL jacks and physical routing | UI provides parameter editors for output and controller assignment, but not cable diagrams | `menuPage_system.cpp` (Output/Tables), `menuPage_assign1.cpp` | Partial |
| 5 | Panel descriptions ([1]-[6] knobs, display, controls) | App mirrors core edit model (knob-driven parameter edits and screens) | `editPage.cpp`, `customControlKnob.cpp`, `customControlListMenu.cpp` | Implemented |
| 6 | Mode setting (Guitar/Bass), OUTPUT SELECT entry | Exposed in System/Setup pages and bass mode branching in app | `Preferences.cpp` (BassMode), `menuPage_setup.cpp`, `menuPage_system.cpp` | Implemented |
| 7 | OUTPUT SELECT options, start GK setting flow | Output-select options exposed; amp target selection available via combo menus | `menuPage_setup.cpp`, `menuPage_system.cpp`, `customControlListMenu.cpp` | Implemented |
| 8 | GK pickup type, scale length, GK setup pages | System GK set screens and per-set parameters are exposed (including bass/guitar variants) | `menuPage_system.cpp` (GK SET 1..3 stacks), `customControlDataKnob.cpp` | Implemented |
| 9 | Bridge distance, tuner mode and operation | Tuner menu supports mono/poly tuner modes and visuals | `menuPage_tuner.cpp`, `customControlVU.cpp`, `customMeter.cpp` | Implemented |
| 10 | Play screen icons/meters overview | Main app view includes chain, meters, and block states; icon parity is not 1:1 with pedal LCD | `floorBoardDisplay.cpp`, `stompBox.cpp`, `statusBarWidget.cpp` | Partial |
| 11 | Screen operations + INST/EFFECT knob behavior | Core editor interaction model implemented (knob turns, toggles, all-params lists) | `customControlParaEQ.cpp`, `customGraphicEQGraph.cpp`, `customMasterEQGraph.cpp`, `editPage.cpp` | Implemented |
| 12 | Patch structure and chain concept | Signal chain UI with draggable block ordering and branch balancers | `floorBoard.cpp` (fx chain polygon/layout), `floorBoardDisplay.cpp`, `stompbox_*.cpp` | Implemented |
| 13 | Editing INST blocks | INST editing is available via edit pages and custom controls | `editPage.cpp`, `menuPage_*.cpp`, `customControlKnob.cpp`, `customControlDataKnob.cpp` | Implemented |
| 14 | Saving/recalling INST variation | Variation operations are controlled by SY-1000 firmware workflow; app can edit patch params and trigger writes but no dedicated "variation librarian" screen | `SysxIO.cpp`, `menuPage.cpp`, patch write plumbing in UI | Partial |
| 15 | Editing effects, all-parameter view, chain reorder | Effect edit pages and chain reordering are implemented | `floorBoard.cpp`, `customControl*`, `menuPage_*`, `stompbox_*` | Implemented |
| 16 | Saving/recalling effect variation | Similar to page 14: parameter edits and writes are present, explicit effect-variation manager is limited | `SysxIO.cpp`, `menuPage.cpp` | Partial |
| 17 | CTL/EXP assignments + quick assign | Assignment pages and control mapping are exposed (Assign pages + system tables) | `menuPage_assign1.cpp`, `menuPage_assign*.cpp`, `menuPage_system.cpp` | Implemented |
| 18 | Saving patch (WRITE) | Patch write workflow supported in app via sysex write flow/dialogs | `patchWriteDialog.cpp`, `SysxIO.cpp`, `menuPage.cpp` | Implemented |
| 19 | System parameters + factory reset + auto-off | System pages expose many global settings; factory-reset semantics are mostly device-side | `menuPage_system.cpp`, `Preferences.cpp` | Partial |
| 20 | Main specifications | Informational only; no direct UI mapping needed | N/A | N/A (informational) |
| 21 | Safety and important notes | Safety text is external/manual guidance, not app UI | N/A | N/A (hardware/safety) |

## UI Navigation Map (Practical)

Use this quick map when testing manual workflows against the app:

- Manual "TUNER" sections -> Tuner menu page  
  - `menuPage_tuner.cpp`
- Manual "SYSTEM", "GK SETTING", "OUTPUT SELECT", "USB", "MIDI tables" -> System/Setup pages  
  - `menuPage_system.cpp`, `menuPage_setup.cpp`
- Manual "Patch structure / effects chain / reorder" -> Main chain canvas  
  - `floorBoard.cpp`, `floorBoardDisplay.cpp`
- Manual "INST/EFFECT editing" -> Edit dialog controls (knobs, switches, combos, graphs)  
  - `editPage.cpp`, `customControl*.cpp`, `custom*Graph.cpp`
- Manual "WRITE (patch save)" -> Patch write flow  
  - `patchWriteDialog.cpp`, `SysxIO.cpp`
- Manual "ASSIGN/CTL/EXP" -> Assign pages + System MIDI function tables  
  - `menuPage_assign*.cpp`, `menuPage_system.cpp`

## Gaps / Follow-up Work

1. Add an explicit "Variation Manager" UI layer if exact manual parity for INST/EFFECT variation save/recall is required.
2. Add a dedicated "Play Screen parity" note with icon-by-icon mapping vs hardware LCD.
3. Add scripted mapping checks against `midi.xml`/`midi.xsd` to verify parameter-level completeness by page topic.

