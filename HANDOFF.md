# SY-1000 FloorBoard — Grid Layout Consistency Handoff

## Goal
Make every tab in the INST 1/2/3 editor panels visually consistent. Currently each tab uses different column counts, spacing, groupbox nesting, and widget sizes, creating a disjointed UI.

## What's Already Done (don't repeat this work)
- **SIGBUS crash fix**: `SysxIO::setFileSource()` — added bounds check for `-1` indexOf (was causing UB)
- **INST TYPE dropdown**: reduced min-width from 190→130, direction string `large_inst_left`
- **Deploy script**: `/Users/bsewell/010-MUSIC-STUDIO/SY-1000/deploy.sh`
- **gridConstants.h created**: `/Users/bsewell/010-MUSIC-STUDIO/SY-1000/src/gridConstants.h` — 2 spacing tiers + ratio/combo macros
- **compactCurrentStackField added** to all content pages (108 calls across 3 files)
- **ALT TUNE reflowed** to 6-column grid on all 3 inst files

## Why The Current Changes Don't Work
`compactCurrentStackField(h, v)` only sets **pixel gaps between grid cells**. It does NOT:
- Change column count (tabs still use 3, 4, 7, 8, 11, 13 columns)
- Change how `applyControlCellMetrics` calculates minimum cell widths
- Change groupbox placement coordinates
- Make different pages align visually

**The real fix requires restructuring the grid coordinates** in every `addKnob`/`addComboBox` call.

## How The Layout System Works

### Key files
- **editPage.cpp** (`/Users/bsewell/010-MUSIC-STUDIO/SY-1000/src/editPage.cpp`) — all layout functions
- **editPage.h** — declarations
- **soundSource_inst1.cpp** / **inst2.cpp** / **inst3.cpp** — the 3 instrument panel builders
- **customControlListMenu.cpp** — dropdown/combo widget, direction string parsing
- **gridConstants.h** — spacing constants and macros

### Layout function chain
1. `newStackField(id)` → creates a new `QGridLayout` for a tab page
2. `compactCurrentStackField(h, v)` → sets H/V spacing on that grid, sets `stackFieldLockWidth=true` (prevents expansion)
3. `addKnob(row, col, rowSpan, colSpan, ...)` → creates widget, places in grid, calls `applyControlCellMetrics`
4. `applyControlCellMetrics(widget, grid, row, rowSpan, col, colSpan, direction, minWidth)`:
   - Gets widget's `minimumSizeHint().height()`
   - Sets `grid->setRowMinimumHeight(row, max(30*ratio, height/rowSpan))` for each spanned row
   - Sets `grid->setColumnMinimumWidth(col, max(80*ratio, width/colSpan))` for each spanned col
   - **This dominates layout** — the minimum widths/heights from this function determine cell sizes
5. `addStackField()` → wraps grid in QWidget, adds to QStackedWidget

### Widget minimum widths (from applyControlCellMetrics)
- Knob: `118 * ratio` pixels wide
- ComboBox (standard): `148 * ratio`
- ComboBox (large): `240 * ratio`
- ComboBox (left_large): `148 * ratio` + label
- ParaEQ: `720 * ratio`
- Switch: smaller, inherits from label

### Ratio (DPI scale)
- `ratio = Preferences::Instance()->getPreferences("Window", "Scale", "ratio").toFloat()`
- Typically 1.0. All pixel values scale by this.

### GroupBox mechanics
- `newGroupBox(title)` → creates a QGroupBox with its own nested QGridLayout
- Widgets added after this go into the groupbox's grid (not the page grid)
- `addGroupBox(row, col, rowSpan, colSpan)` → finalizes groupbox and places it in the parent grid
- GroupBoxes can nest (tracked by `groupBoxLevel`)

## The Reference Grid: COMMON Tab

**This is what "good" looks like** — the COMMON tab at `soundSource_inst1.cpp:525`:

```
Row 0: [Knob ratio1.5] [Switch] [DataKnob ratio1.25]  ___  ___  ___
Row 1: [Knob 1.25]     [Knob]   [Knob]    [Knob]  [Knob]  [Knob]   ← 6 string levels
Row 2: [Knob 1.25]     [Knob]   [Knob]    [Knob]  [Knob]  [Knob]   ← 6 string pans
```
- **6 columns max** (cols 0-5)
- **3 rows** (rows 0-2)
- Row 0 has 3 wider widgets (the first is ratio1.5 = larger)
- Rows 1-2 have 6 uniform knobs at ratio1.25
- Spacing: `compactCurrentStackField(20, 14)`

## Current Tab Grid Structures (What Needs Fixing)

### Dynamic Synth sub-tabs (stackField 1):

| Tab | Max Col | Rows | Problem |
|-----|---------|------|---------|
| OSC | 7 | 4 | 8 cols of knobs — should wrap to 6 |
| FILTER | 4 | 2 | 5 cols — ok but sparse, could use 6 |
| LFO1 | 6 | 3 | 7 cols — row 1 has cols 0-6 |
| SEQ | 12 | 16 | Special (16-step sequencer) — leave as-is |
| LAYER | 6 | 3 | 7 cols — needs reflow |
| COMMON | 5 | 3 | ✅ Already good (reference grid) |
| AMP | 2 | 1 | 3 cols — very sparse |
| LFO2 | 6 | 3 | 7 cols — same as LFO1, needs reflow |

### OSC Synth pages (stackField 2):

| Tab | Max Col | Rows | Problem |
|-----|---------|------|---------|
| OSC | 13 | 4 | 14 columns — very wide, 2 oscillators side by side |
| FILTER | 7 | 4 | 8 cols — should wrap |
| LFO | 11 | 3 | 12 cols — LFO1+LFO2 stacked with divider |

### GR-300 (stackField 3):
- Max col 12, rows 3 — very wide, currently uses GRID_DENSE

### E.Guitar pages (stackField 4):
- Uses groupboxes — "Electric Guitar" groupbox spans (0,0,1,7-9)
- Inner content uses 7-8 columns
- Has nested stackField(5) for instrument type variants

### Acoustic pages (stackField 6):
- Similar to E.Guitar but with different groupbox layout
- Inner content goes to col 9

### VIO/PolyFX (stackField 8, 9):
- GroupBox-based with 4-10 columns

## Layout Validator (NEW — built and working)

### How to run
```bash
SY1000_VALIDATE_LAYOUTS=1 /Applications/SY-1000FloorBoard.app/Contents/MacOS/SY-1000FloorBoard
# Report written to: /Users/bsewell/010-MUSIC-STUDIO/SY-1000/layout_report.md
```

### What it checks
- **COL_COUNT**: grid uses more than 6 columns
- **GRID_WIDTH**: total minimum width exceeds 900px (at ratio 1.0)
- **LABEL_CLIP**: label text wider than allocated width
- **WIDGET_CLIP**: widget's minimumSizeHint exceeds its grid cell allocation
- **OVERLAP**: two widgets assigned to the same grid cell

### Baseline (original state, no changes)
- 828 grids audited across all pages
- 338 grids with violations
- 813 total violations: 249 COL_COUNT, 162 GRID_WIDTH, 369 WIDGET_CLIP, 33 OVERLAP, 0 LABEL_CLIP

### Current state (after Phase 1 fixes)
- 828 grids audited
- 320 grids with violations (-18)
- 765 total violations (-48): 213 COL_COUNT, 162 GRID_WIDTH, 369 WIDGET_CLIP, 21 OVERLAP, 0 LABEL_CLIP

### Fixes applied
1. **Electric Guitar combo overlap** — `addComboBox(0, 1, 1, 7)` → `addComboBox(0, 1, 1, 5)` in stackField 4 non-BassMode branch across all 3 inst files. Eliminated 12 OVERLAP violations.
2. **12 String str1 inner grid coords** — knobs inside `newGroupBox(str1)` used wrong coords `(1,5)`, `(1,11)`, `(2,5)`, `(2,11)` instead of `(0,0)`, `(0,1)`, `(1,0)`, `(1,1)`. Bug fix across all 3 inst files. Eliminated 18 COL_COUNT violations (str1 inner grids went from 12c to 2c).
3. **12 String reflow to 2 rows of 3** — moved str3/str2/str1 groupboxes from `(1,6)`, `(1,8)`, `(1,10)` to `(3,0)`, `(3,2)`, `(3,4)`. SystemOverride changed from `(1,0,2,12)` to `(1,0,4,6)`. All 3 inst files. Eliminated 18 COL_COUNT violations (12 String parent went from 12c to 6c).

### Known remaining issues
- **12 String WIDGET_CLIP** (108 violations) — groupboxes need 253px but grid only allocates 88-171px. This is a layout engine limitation: groupboxes don't set column minimums like knobs do. Qt expands columns at runtime with available space.
- **SP Type WIDGET_CLIP** (18 violations) — combo needs 206px, cell allocates 187px (19px gap). Minor cosmetic clip.
- **PreAmp COL_COUNT** (11 cols × many instances) — would need major restructure of the Amp section.
- **Electric Guitar/Bass COL_COUNT** (9 cols) — insertStackField at col 8 makes it 9 cols. Would need restructuring.
- **Acoustic Guitar COL_COUNT** (10 cols) — similar structural issue.
- **VIO Guitar/Filter COL_COUNT** (8 cols) — would need reflow.
- **insertStackField discrepancy** — inst1 uses `insertStackField(5, 0, 8, 1, 1)` (row=0, col=8) while inst2/inst3 use `insertStackField(5, 8, 0, 1, 1)` (row=8, col=0). Potential bug — not yet investigated.

### Files
- `src/layoutValidator.h` — header with GridCellInfo and GridReport structs
- `src/layoutValidator.cpp` — implementation
- Hooked into `editWindow::addPage()` in `editWindow.cpp`
- Added to `SY-1000FloorBoard.pro` (HEADERS and SOURCES)

### CRITICAL RULE
**Every layout change must be validated.** Workflow:
1. Run validator → save baseline violation count
2. Make grid coordinate changes
3. Rebuild, deploy, run validator again
4. Violation count must go DOWN or stay same — never up
5. Only then do visual review

## Strategy for Real Consistency (REVISED)

The first attempt at blind reflow (changing row/col coordinates without validation) made layouts WORSE. The new strategy is validator-driven.

### Phase 1: Fix existing violations WITHOUT changing column count
Many violations are WIDGET_CLIP (widget too wide for cell) and OVERLAP (duplicate cell assignments). These can be fixed by:
- Increasing colSpan on widgets that need more space
- Fixing overlapping cell assignments
- These are safe changes that don't restructure the layout

### Phase 2: Reflow grids that exceed MAX_COLS
For each grid with COL_COUNT violations, reflow to ≤6 columns:
- Run validator before and after each change
- Handle SystemOverrides carefully (they reference grid coordinates)
- Move one tab at a time, validate each change independently

### Phase 3: Fix GRID_WIDTH violations
For grids where total minimum width exceeds 900px:
- May need to accept wider grids for genuinely wide content (SEQ, dual OSC)
- Or restructure with groupboxes / sub-tabs
- Some pages (E.Guitar with 11 cols, PreAmp, VIO) may need architectural changes

### Priority by violation density
1. **12 String groupboxes** — most WIDGET_CLIP violations (6 per instance × 3 inst files)
2. **Electric Guitar / Bass** — OVERLAPs + COL_COUNT + GRID_WIDTH
3. **Dynamic Synth OSC** — COL_COUNT (8 cols, needs 6)
4. **OSC Synth pages** — COL_COUNT + GRID_WIDTH (13-14 cols)
5. **GR-300** — COL_COUNT + GRID_WIDTH (13 cols)
6. **VIO Guitar / Poly FX** — COL_COUNT + GRID_WIDTH
6. Update the `compactCurrentStackField` call (should already be `GRID_H, GRID_V`)
7. Test by building and deploying

## File Structure

All 3 inst files follow the same structure:
```
soundSource_inst1.cpp  — hex addresses use "00" prefix
soundSource_inst2.cpp  — hex addresses use "00" or "02" (BassMode-dependent)
soundSource_inst3.cpp  — hex addresses use member vars hex1, hex2
```

Each file has identical tab structure:
- stackField 10 → tab bar
- stackField 0 → INST TYPE header
- stackField 1 → Dynamic Synth pages (COMMON, ALT TUNE, OSC, FILTER, AMP, LFO1, LFO2, SEQ, LAYER)
- stackField 2 → OSC Synth pages
- stackField 3 → GR-300 pages
- stackField 4 → E.Guitar/Bass pages
- stackField 5 → instrument type variant sub-pages (nested in stack 4)
- stackField 6 → Acoustic Guitar/Bass pages
- stackField 7 → alternate E.Guitar/Bass pages
- stackField 8 → VIO Guitar / Poly FX pages
- stackField 9 → Poly FX (guitar mode only)

## Build & Deploy

```bash
# Build (from Qt Creator or command line):
cd /Users/bsewell/010-MUSIC-STUDIO/SY-1000/build
# (use Qt Creator build, or qmake + make)

# Deploy:
/Users/bsewell/010-MUSIC-STUDIO/SY-1000/deploy.sh
```

## Important Gotchas

1. **Never change hex address parameters** — the 4 hex strings in each addKnob/addComboBox map to specific SY-1000 MIDI SysEx addresses. Changing them breaks functionality.
2. **BassMode conditional** — many pages have `if(BassMode==true)` branches with different content. Both branches need the same grid reflow.
3. **SystemOverride calls** — `addSystemOverride(row, col, ...)` must match the widget it controls. When you move a widget, move its override too.
4. **addDivider** colspan — dividers span the full width. Update their colspan when changing column count.
5. **inst2 uses BassMode-dependent hex variables** (`hexA`/`hexB`), inst3 uses member vars (`hex1`/`hex2`) — don't confuse them.
6. **Deploy script nuance** — must `rm -rf` before `cp -R` to avoid nested .app bundle.
7. **Qt rebuild needed** — changes to .cpp files require rebuild. gridConstants.h changes require rebuild of all files that include it.
