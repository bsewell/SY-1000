# SY-1000 FloorBoard — Handoff: Signal Chain Layout Spec + Phase 5 QML Migration

## Session Summary (2026-03-24, fourth session)

Two signal chain wire rendering bugs fixed in `src/floorBoard.cpp`.

### Bug 1 — "Long bars" (branch wires stretched far right)

**Root cause:** `rightEdgeForId(8/9/10)` returns the right edge of FX1/FX2/FX3 by type ID regardless of which row the block is currently on. When FX3 is in Zone B or C (compacted past BAL2), its large X position inflated `branch2SourceRight`, stretching polygon[5]/[7] (the INST3 branch wire endpoints) far to the right.

**Fix (lines 1994-2022):** Added Y-row guards before using each typed stompBox's right edge as a branch endpoint. FX1 is only used if its Y is within the INST1 row band (`lev1 ± rowBand, < lev2`); FX2 within INST2 row; FX3 within INST3 row. If a box is not on its expected row, its contribution is `-1` (ignored).

### Bug 2 — "Col 2 bad alignment" (BAL2 placed wrong relative to wires)

**Root cause:** Polygon points 4, 5, 7-9, 11-12 (the BAL riser endpoints) were initialized with `offset + (58/60/65)*ratio + n*flowStep`. But blocks are placed at `offset + layout.colX(n) = offset + firstFlowX + n*flowStep`, where `firstFlowX = 110px` at ratio=1. The polygon offsets (58-65) were ~50px short of `firstFlowX=110`. This made the upper-bound clamp for BAL2 placement (`polygon[9].x() - minBalancerOrderGap`) ~50px too far left, constraining BAL2 behind its correct position.

**Fix (lines 1875-1883):** Replaced all `(58/60/65)*ratio + n*flowStep` with `layout.colX(n)` in the polygon BAL point initializations. The riser wire endpoints now start at the same X origin as the blocks, so `placeBalancerOnRiser` bounds are consistent.

Build: clean, 0 errors.

---

## Session Summary (2026-03-24, third session)

QML UI quality pass — Boss color tokens, external parameter sync, magic number cleanup.

### Changes made this session

**`qml/SyTheme.qml`** — Boss-exact design tokens:
- `accent`: `#00ccff` → `#33D6FF` (Boss accent-primary from TONE STUDIO CSS)
- `bgPanel`: `#1a1a1a` → `#1A1C1F` (Boss bg-base)
- `bgControl`: `#2a2a2a` → `#292929` (Boss bg-surface)
- Text colors: now opacity-based white matching Boss: `textSecondary`=80%, `textLabel`=64%, `textDimmed`/`textSection`=32%
- `divider`/`border`: `#333333`/`#666666` → `#52ffffff` (Boss hairline 32% white)
- New tokens: `bgElevated: "#252B32"`, `borderProminent: "#a3ffffff"` (64% white)

**`qml/FilmstripKnob.qml`** — two fixes:
1. Value text: `SyTheme.accent` (cyan) → `SyTheme.textPrimary` (white) + `font.bold: true` (per Boss dial spec)
2. Added `Connections { onDataRefreshed, onParameterChanged }` — knobs now sync in real time when parameters change externally (MIDI, stompbox tile toggle, bulk load)

**`qml/SyKnob.qml`** — added `onParameterChanged` to existing Connections block; canvas repaints on external change

**`qml/SySwitch.qml`** — added `onParameterChanged` to existing Connections block

**`qml/SyComboBox.qml`** — added `onParameterChanged` to existing Connections block

**`qml/Fx1Panel.qml`, `qml/Fx2Panel.qml`, `qml/Fx3Panel.qml`** — fixed `height: parent.height - 70` (content overflowed by 3px) → `parent.height - SyTheme.headerHeight - SyTheme.modeSelectorH - 1`; also fixed hardcoded `font.pixelSize: 11` → `SyTheme.fontLabel`

**`qml/NormalInputPanel.qml`** — fixed `height: parent.height - 75` (was 1px too tall) → `parent.height - SyTheme.headerHeight - 1 - SyTheme.modeSelectorH - 1`

Build: clean, 0 errors.

### Known debt cleared
- QML controls now subscribe to `paramBridge.parameterChanged` for external sync ✓
- `Fx1/2/3Panel` magic height numbers removed ✓

---

## Session Summary (2026-03-24, second session)

Signal chain layout spec fully implemented. Three code bugs fixed, §5a doc updated.

### Changes made this session

**`src/floorBoard.cpp`** — three fixes:
1. C1 rule for INST3 (incr==2): `x_axis=firstFlowX` → `x_axis=firstFlowX-flowStep`. INST3's first FX block was starting one step too far right.
2. C1 rule for NORMAL (incr==4): same fix.
3. BAL cap ordering invariant: `bal2cap = qMax(index3, index4) + MAX_BAL_SPREAD` → `qMax(qMax(bal1xpos + index3, index4) + MAX_BAL_SPREAD, bal1xpos + 1)`. The old cap ignored that Zone B starts from BAL1's column, causing BAL2 to be capped behind BAL1 on patches with an empty Zone B and a non-trivial BAL1 offset. Same fix for BAL3 using bal2xpos.

**`src/stompBox.cpp`** — one fix:
4. DD1 (id=14) and DD2 (id=15) removed from `hidesFlowDescriptionLabel`. Both delays already called `updateLabel` but the label was hidden at position time. All three DD blocks now show delay-type labels.

**`docs/ui/sy1000_boss_layout_notes.md` §5a** — added at top of section:
- Signal-Chain Row Model table (5 rows, Y values, row B position)
- Block Pitch Constants table (flowStep, instStartX, firstFlowX, MAX_BAL_SPREAD, etc.)
- 3-Zone Compaction Algorithm (Zone A/B/C, BAL2/3 formulae with corrected cap floors)
- Block Label Rule table (which blocks show labels and what source address)

Build: clean, 0 errors.

---

## Session Summary (2026-03-24, first session)

This session was a requirements-gathering session for the signal chain layout.
No code was changed. The output is a complete signal chain layout specification
ready to drive implementation. See `docs/ui/sy1000_boss_layout_notes.md` §5a for
the full spec — the section has been substantially expanded.

---

## What Was Established This Session

### Signal Chain Layout — Complete Model

The signal chain has a **fixed 5-row structure** with a **dynamic X-axis layout**.

#### Rows (fixed vertical, Y at ratio=1)
| Row | Content | Y (top) |
|---|---|---|
| Row 0 | INST1 source + pre-BAL1 branch blocks | 65px |
| Row 1 | INST2 source + pre-BAL1 branch blocks | 130px |
| Row B | BAL1 output chain (Zone B→C→output) | between rows 1 and 2 |
| Row 2 | INST3 source + pre-BAL2 branch blocks | 195px |
| Row 3 | NORMAL source + pre-BAL3 branch blocks | 260px |

Row spacing = **65px**. Row B Y = midpoint of rows 1 and 2 signal lines.

#### Block Pitch (from code)
| Constant | Value (ratio=1) |
|---|---|
| flowStep (block pitch) | 55px |
| Source block width | 96px |
| instStartX | 15px |
| instToFxGap | 16px |
| firstFlowX | 127px |
| MAX_BAL_SPREAD | 2 columns |

All values scale with `ratio`.

#### BAL Merge Topology (immutable)
- BAL1 always merges INST1 + INST2
- BAL2 always merges BAL1 output + INST3
- BAL3 always merges BAL2 output + NORMAL

BAL Y positions:
- BAL1: midpoint of rowCenter1 and rowCenter2
- BAL2: midpoint of rowCenter_B and rowCenter3
- BAL3: midpoint of rowCenter_B and rowCenter4

#### Compaction Algorithm (3 zones, all left-packed)

```
Zone A  — INST1/INST2 branches (rows 0 and 1), blocks before BAL1
          Blocks packed left from source.
          BAL1.x = firstFlowX + max(INST1_count, INST2_count) × flowStep

Zone B  — Row B shared chain between BAL1 and BAL2
          + INST3 branch (row 2, packed left from source)
          BAL2.x = max(BAL1.x + ZoneB_count × flowStep,
                       firstFlowX + INST3_count × flowStep)
          Capped at max(INST3_count, ZoneB_count) + MAX_BAL_SPREAD

Zone C  — Row B shared chain between BAL2 and BAL3
          + NORMAL branch (row 3, packed left from source)
          BAL3.x = max(BAL2.x + ZoneC_count × flowStep,
                       firstFlowX + NORMAL_count × flowStep)
          Capped at max(NORMAL_count, ZoneC_count) + MAX_BAL_SPREAD

Output  — After BAL3 on Row B, packed left from BAL3
          Free-form order from patch data. MST always last.
```

Empty branch = wire only, no blocks, branch length = 0.

Active/inactive state = **visual only**. Wires always drawn. Block positions
never change based on on/off state.

#### Branch First-Block Rule (added 2026-03-24)
The first stomp block on every instrument branch (INST1, INST2, INST3, NORMAL)
must start at **C1** — immediately right of the source block with only
`instToFxGap` between them. Blocks never float right to align with a downstream
BAL or match another row. See `docs/ui/sy1000_boss_layout_notes.md` §5a C1 rule.

#### Output Section Topology
The output blocks (S LR, M LR, DIV, MIX, MST) can be placed in any order
after BAL3. When DIV splits the signal, one path runs on Row B and the
other branches **upward** to a 7th output-only row above Row B. MST is
always the final node. S LR and M LR must stay in the output chain
(not mid-chain).

#### Block Labels (below each block rectangle)
Shown: the first/closest dropdown to the power button = the "type" selector.

| Block | Label source |
|---|---|
| INST 1/2/3 | INST TYPE dropdown (hex3 `01`) — e.g. "Dynamic Synth", "E.Guitar" |
| AMP | AMP type/model (hex3 `01`) — e.g. "Natural", "JC-120" |
| CS | Compressor type (hex3 `01`) — e.g. "Boss Comp" |
| DS | Distortion type (hex3 first combo) — e.g. "X-OD" |
| RV | Reverb type (hex3 `01`) — e.g. "Plate", "Hall" |
| DD1/DD2/DD3 | Delay type (hex3 `01`) — e.g. "Stereo", "Tape Echo" |
| CE | Chorus mode (hex3 `01`) |
| FX1/FX2/FX3 | FX type (hex3 `01`) — e.g. "Slicer", "Harmonizer" |
| EQ1, EQ2 | No label |
| NS | No label |
| FV1, FV2 | No label |
| ↑↓ (effects loop) | No label |
| BAL1/2/3 | No label |
| DIV, MIX, MST, S LR, M LR | No label |

#### Complete Stomp Box Inventory (maximum — no more exist)
FX1, FX2, FX3, EQ1, EQ2, AMP, CS, DS, NS, CE, DD1, DD2, DD3, RV, FV1, FV2, LP,
↑↓ (effects loop), DIV, MIX, S LR, M LR, MST
Any of these can appear in Zone A, B, C, or the output section.

---

## What Needs Updating in Docs

`docs/ui/sy1000_boss_layout_notes.md` §5a needs:
1. The complete 5-row model with Y values
2. The block pitch constants table
3. The 3-zone compaction algorithm (replace the partial column contract)
4. The BAL column rule (currently incomplete — doesn't account for Zone B chain driving BAL2)
5. The output topology rules (branching, MST-last, upward split)
6. The block label rule

**Status: spec complete, code implemented, doc updated.**

---

## Session Summary (2026-03-25)

Phase 5 QML migration completed. All assign pages were already migrated in prior sessions;
HANDOFF was outdated. Fixed `menuPage_midi.cpp` (contained wrong PDL code) and completed
the MIDI panel migration.

### Changes made this session

**`src/menuPage_midi.cpp`** — rewrote: was accidentally containing `menuPage_pdl` code.
Now correctly implements `menuPage_midi::setEditPages()` calling:
`editDetails()->setQmlPage("qrc:/qml/MidiPanel.qml", sys1, "30")`
where sys1 = "01" (guitar) or "08" (bass).

**`qml/MidiPanel.qml`** — new file. Standalone MIDI settings panel with two tabs:
- SETTING 1: RX/TX channel, OMNI MODE, MIDI IN THRU, USB IN THRU, SYNC CLOCK, CLOCK OUT, PC OUT, TX/RX PC MAP
- SETTING 2: CC# assignments (No1-4, BANK DN/UP, CTL1-6, EXP1-2, GK VOL/S1/S2)
All controls use `hex0="00"`, `hex1` (sys1), `hex2="30"`.

**`qml/qml.qrc`** — added `MidiPanel.qml`.

**`SY-1000FloorBoard.pri`** — added `menuPage_midi.h` and `menuPage_midi.cpp` (were missing).

**Note:** `menuPage_midi` is not yet wired into `floorBoard::initMenuPages()`.
It needs an ID assigned when the correct menu button slot is confirmed.
Currently `menuPage_pdl` (foot controllers) occupies ID 12.

Build: pre-existing SDK issue (`__yield` in MacOSX26.4.sdk) blocks full build.
Our files compile cleanly with no errors from our code.

---

## Phase 5 QML Migration — Status

**COMPLETE.** All assign pages and the MIDI panel are now migrated:
- `menuPage_assign1.cpp` + assigns 4-16: all call `setQmlPage("qrc:/qml/AssignPanel.qml", ...)`
- `menuPage_midi.cpp`: calls `setQmlPage("qrc:/qml/MidiPanel.qml", sys1, "30")`
- `AssignPanel.qml` + `assign/AssignSlot.qml`: all 16 assigns, full parameter set
- `MidiPanel.qml`: SETTING 1 + SETTING 2 tabs

**No wiring needed:** Menu IDs 10-15 are all taken (TUNER/SETUP/CTL/MASTER/SYSTEM/ASSIGN). System-level MIDI settings are already in `SysMidiSetting.qml` inside SystemPanel (ID 14). `menuPage_midi` is a standby component with no hardware button slot.

---

## Known Issues (pre-existing)
- User presets show "init patch" names until device connects and sends bulk SysEx
- MIDI port conflict if Boss Tone Studio is open (close BTS before launching FloorBoard)
- Crash diagnosis: no crash capture beyond macOS DiagnosticReports (no FloorBoard .ips found)
- Build broken: `__yield` missing from MacOSX26.4.sdk — Qt needs to be rebuilt or SDK downgraded
