# About This Project

## Origin

This project began as **SY1000FloorBoard**, a desktop patch editor for the Boss SY-1000 guitar synthesizer, originally written by **Colin Willcocks** (gumtownbassman@yahoo.com) and published on SourceForge:

> https://sourceforge.net/projects/sy1000floorboard/files/SY1000FloorBoard/

Colin is the originator of the entire application architecture: MIDI SysEx communication, patch storage and recall, signal-chain rendering, the custom widget library, and the Qt project structure. All of that work is his.

---

## What This Fork Adds

This fork, maintained on the `qml-ui-migration` branch, extends Colin's original with three areas of improvement:

### 1. UI Accuracy — Boss Tone Studio Fidelity

The original editor's panel layouts were functional but diverged visually from the official Boss TONE STUDIO software. This fork audits every effect panel against the SY-1000 reference manual and Boss TONE STUDIO screenshots, correcting:

- Label names, order, and grouping
- Tab layout and parameter order within tabs
- Design tokens (colors, typography, spacing) extracted directly from Boss TONE STUDIO CSS
- Dropdown option lists to match the official MIDI implementation spec

Reference documents driving this work are in `docs/`:
- `docs/sy1000_parameter_inventory.md` — every parameter by block and hex address
- `docs/ui/sy1000_boss_layout_notes.md` — Boss visual patterns and signal-chain spec
- `docs/ui/sy1000_claude_compare_matrix.md` — block-by-block current vs Boss spec comparison
- `docs/ui/dropdown_inventory.md` — dropdown option lists cross-referenced to MIDI implementation

### 2. QML Migration — Modern Qt6 UI Layer

The original editor used Qt Widgets throughout. This fork introduces a parallel QML/Qt Quick UI layer, migrating panel-by-panel from C++ widget code to declarative QML:

**Migrated panels (complete):**
- All 3 instrument sources (INST1, INST2, INST3) with 9 instrument-type sub-panels each
- All stomp boxes: NS, CS, DS, AMP, FX1, FX2, FX3, CE, DD1, DD2, DD3, RV, EQ1, EQ2, FV1, FV2, LP, and output blocks
- System pages (all sub-pages: MIDI, USB, Input/Output, Hardware, Knob, Play Option, Total, Auto Power Off, Guitar to MIDI, GK/Normal Set Select)
- Pedal/GK pages
- Tuner panel
- Master, Setup, and all 16 Assign pages
- Patch browser with patch list model

**QML component library (`qml/`):**
- `SyTheme.qml` — Boss-exact design tokens (colors, type scale, spacing)
- `SyKnob.qml` — filmstrip knob with real-time external parameter sync
- `SySwitch.qml` — on/off toggle matching Boss style
- `SyComboBox.qml` — styled dropdown with parameter bridge binding
- `FilmstripKnob.qml` — frame-animated knob from PNG filmstrip assets
- `SySettingRow.qml`, `SySectionLabel.qml`, `SyTabBar.qml`, `SyPanelBase.qml` — layout primitives

**C++ bridge (`src/parameterBridge.cpp`):**
All QML controls read and write SY-1000 parameters through `ParameterBridge`, a `QObject`-exposed singleton that translates between Qt property bindings and MIDI SysEx hex addresses. External changes (MIDI receive, bulk load, stompbox tile toggles) propagate back to QML via `parameterChanged` signals.

### 3. Signal-Chain Layout Engine Rewrite

The signal-chain visualizer (the graphical block diagram showing how effects are routed) had layout bugs causing blocks to be misplaced and branch wires to stretch incorrectly. This fork:

- Introduced `ChainLayout` — a column-based coordinate system replacing raw pixel offsets
- Implemented the correct 3-zone compaction algorithm (Zone A/B/C) matching Boss hardware behavior
- Fixed Y-row guards on branch wire endpoints so wires don't stretch when FX blocks move between rows
- Fixed BAL2/BAL3 placement caps so balancers never appear behind their correct column

The complete signal-chain layout spec is in `docs/ui/sy1000_boss_layout_notes.md` §5a.

---

## Technology Stack

| Layer | Technology |
|---|---|
| Language | C++17 |
| UI framework | Qt 6 (Widgets + QML/Qt Quick) |
| Build system | qmake |
| MIDI I/O | RtMidi (bundled) |
| Patch format | Roland/Boss SysEx (.syx) |
| Patch library format | XML |
| Theme system | Qt Style Sheets (.qss) + QML SyTheme tokens |
| Target platforms | macOS (primary), Windows (supported via .pro conditionals) |

---

## Repository Structure

```
src/          C++ application source
qml/          QML UI panels and component library
qss/          Qt Style Sheets (5 themes: aqua, black, blue, green, white)
docs/         Manual mappings, UI audit docs, signal-chain spec, handoff notes
images/       PNG assets for widgets and signal-chain tiles
build/        Generated output (not committed)
```

---

## Building

```bash
qmake SY-1000FloorBoard.pro
make -j$(sysctl -n hw.logicalcpu)
```

Requires Qt 6.0 or later. App bundle produced at `build/packager/SY-1000FloorBoard.app`.

---

## Credits

- **Colin Willcocks** (gumtownbassman@yahoo.com) — original author and architect of SY1000FloorBoard
- **AI-assisted development** — UI accuracy pass, QML migration, and signal-chain layout rewrite produced using Claude Code (Anthropic) as a coding assistant, working from Boss SY-1000 reference manuals and MIDI implementation documents
