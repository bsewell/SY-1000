# SY-1000 Original App UI Notes (Layout, Spacing, Terminology)

This note captures practical UI patterns from the BOSS SY-1000 screenshots and translates them into implementation guidance for SY-1000 FloorBoard.

> **Related docs**: Exact Boss CSS design tokens (colors, sizes, fonts) → `docs/ui/ux_improvement_report.md §1`

## 1) Visual/Interaction Patterns to Copy

- Header rail per block
  - Fixed-height title bar with: power icon, block name, optional Variation selector.
  - Strong section identity via muted accent color (not neon).

- Two-zone parameter layout
  - Zone A (optional): left power rail plus mode/type selectors in the same first row under header.
  - Zone B: knob matrix below a divider line.

- Predictable control cell
  - Every control cell has the same stack:
    1) Value dial (or value ring)
    2) Numeric/text value centered
    3) Label in uppercase
  - Disabled controls keep position but are dimmed (no reflow).

- Predictable leading switch rail
  - If a page has an enable state, the first control at the far left is always the on/off switch.
  - The switch rail keeps a consistent width so adjacent parameter boxes start on the same vertical line.
  - The same SysEx address must drive both the edit-page switch and the signal-chain rectangle state.
  - When the effect is off, the parameter controls for that page are disabled while the power switch remains active.
  - Clicking the signal-chain rectangle must toggle the same power state as the page switch; both views stay in sync.
  - Dropdown typography and sizing are shared behavior; the control point is `src/customControlListMenu.cpp` and the inventory/direction variants live in `docs/ui/dropdown_inventory.md`.
  - Stomp/effect setting labels share a separate typography control in `src/customControlLabel.cpp`; the inventory of the shared label families lives in `docs/ui/setting_label_inventory.md`.

- Horizontal rhythm first
  - Controls align to a strict column grid across each row.
  - No irregular per-control offsets.

- Typography hierarchy
  - Block title: large, condensed, bold.
  - Labels: uppercase, medium contrast.
  - Values: largest text in the control.

## 2) Suggested Terminology for FloorBoard docs/code

- `block_header`
- `block_accent`
- `selector_row`
- `param_grid`
- `param_cell`
- `param_value`
- `param_label`
- `param_disabled`
- `section_divider`

Use these terms in docs and comments so UI discussions are consistent.

## 3) Spacing System (recommended tokens)

Use one spacing scale everywhere:

- `space_4 = 4`
- `space_8 = 8`
- `space_12 = 12`
- `space_16 = 16`
- `space_24 = 24`
- `space_32 = 32`

Recommended layout values from screenshot proportions:

- Outer panel padding: `24` left/right, `16` top/bottom
- Header height: `64-72`
- Header icon/title gap: `16`
- Selector row top padding: `16`
- Selector label -> combo gap: `10-12`
- Divider top margin: `12`, bottom margin: `16`
- Param grid column gap: `20-28`
- Param grid row gap: `24-32`
- Dial center to label baseline: constant per row

## 4) Control Sizing Tokens (recommended)

- Dial size classes:
  - `dial_l = 132`
  - `dial_m = 116`
  - `dial_s = 96`
- Label width per cell: fixed to cell width; centered text.
- Minimum cell width: `150-170`.

## 5) Color/State Rules

- Keep base background near-black.
- Accent is per block type only (delay/chorus/eq/etc).
- Active ring: one cyan hue globally.
- Inactive ring: neutral gray.
- Disabled controls: keep layout; reduce alpha to ~35-45%.

## 5b) No-Cutoff / No-Overlap Contract

- No visible UI item may be partially hidden, overlapped, or cut off by the page viewport.
- This applies to:
  - knob rings
  - value boxes
  - labels
  - dropdowns
  - tab bars
  - header text
  - page footer rows such as `PAN`, `LEVEL`, and similar last-row controls
- If a page body becomes taller than the embedded edit viewport:
  - first use the available width to the right before introducing more rows
  - then reduce row gaps and unused padding within the existing page contract
  - do not clip the last row and do not collapse controls into overlapping positions
- Overflow handling rules:
  - do not introduce vertical scrolling for normal instrument/effect pages unless the user explicitly approves it
  - no nested white panels, default Qt frames, or background fills may appear
  - horizontal scrolling is not allowed for normal parameter pages; content must still fit the page width contract
- Acceptance rule:
  - before a page layout change is considered complete, confirm that the first visible row, the lowest visible row, and all labels/value boxes between them are fully readable in the installed app

## 5a) Signal-Chain Geometry Rules

### Signal-Chain Row Model

The signal chain has a **fixed 5-row structure** with a **dynamic X-axis** determined by patch content.

| Row | Content | Y (top, ratio=1) | Signal line Y (ratio=1) |
|---|---|---|---|
| Row 0 | INST1 source + pre-BAL1 FX blocks | 65 | 65 + inst1MidY |
| Row 1 | INST2 source + pre-BAL1 FX blocks | 130 | 130 + inst2MidY |
| Row B | BAL1 output chain (Zone B→C→output) | midpoint rows 1/2 | bal1MidY |
| Row 2 | INST3 source + pre-BAL2 FX blocks | 195 | 195 + inst3MidY |
| Row 3 | NORMAL source + pre-BAL3 FX blocks | 260 | 260 + normalMidY |

Row spacing = **65 px** (ratio=1). Row B is the midpoint of rows 1 and 2 signal lines.

BAL2 output row center = midpoint of Row B and Row 2.
BAL3 output row center = midpoint of BAL2 output row and Row 3.

### Signal-Chain Block Pitch Constants (ratio=1)

| Constant | Value | Purpose |
|---|---|---|
| `flowStep` | 55 px | Horizontal pitch between adjacent FX blocks |
| `instStartX` | 15 px | Source block left edge |
| `instWidth` | 80 px | Source block rendered width (192 / kFlowBlockScale 2.4) |
| `touchGap` | 15 px | Gap between source block and first FX block |
| `firstFlowX` | 110 px | First FX block left edge (= instStartX + instWidth + touchGap) |
| `MAX_BAL_SPREAD` | 2 columns | Maximum extra spread allowed beyond longest input path |

All values scale linearly with `ratio`.

### 3-Zone Compaction Algorithm

The signal chain is divided into three zones. All zones are left-packed from C1.

```
Zone A  — INST1 and INST2 branches (rows 0 and 1), blocks before BAL1
          Left-packed from C1.
          BAL1.col = max(INST1_count, INST2_count)

Zone B  — Row B chain between BAL1 and BAL2 + INST3 branch (row 2)
          BAL2.col = max(BAL1.col + ZoneB_count, INST3_count)
          Cap: max(max(BAL1.col + ZoneB_count, INST3_count) + MAX_BAL_SPREAD,
                   BAL1.col + 1)    ← floor preserves BAL1 < BAL2 invariant

Zone C  — Row B chain between BAL2 and BAL3 + NORMAL branch (row 3)
          BAL3.col = max(BAL2.col + ZoneC_count, NORMAL_count)
          Cap: max(max(BAL2.col + ZoneC_count, NORMAL_count) + MAX_BAL_SPREAD,
                   BAL2.col + 1)    ← floor preserves BAL2 < BAL3 invariant

Output  — After BAL3 on Row B; free-form order from patch data. MST always last.
```

Empty branch = wire only, no blocks. Active/inactive is **visual only** — block positions never change with on/off state.

### Block Label Rule

Labels appear below each block rectangle and show the current type selection.

| Block | Label source | Label shown |
|---|---|---|
| INST 1/2/3 | INST TYPE dropdown (`hex3 01`) | yes — e.g. "Dynamic Synth", "E.Guitar" |
| AMP | AMP model (`hex3 01`) | yes — e.g. "Natural", "JC-120" |
| CS | Compressor type (`hex3 01`) | yes — e.g. "Boss Comp" |
| DS | Distortion type (`hex3 01`) | yes — e.g. "X-OD" |
| RV | Reverb type (`hex3 01`) | yes — e.g. "Plate", "Hall" |
| DD1 / DD2 / DD3 | Delay type (`hex3 01`) | yes — e.g. "Stereo", "Tape Echo" |
| CE | Chorus mode (`hex3 01`) | yes |
| FX1 / FX2 / FX3 | FX type (`hex3 01`) | yes — e.g. "Slicer", "Harmonizer" |
| EQ1, EQ2 | — | no label |
| NS | — | no label |
| FV1, FV2 | — | no label |
| LP (effects loop) | — | no label |
| BAL1 / BAL2 / BAL3 | — | no label |
| DIV, MIX, MST, S LR, M LR | — | no label |

---

- Treat the upper signal chain as a separate geometry system from the lower edit pages.
- Patch changes may rebuild the upper signal chain; lower-page parameter edits must not disturb upper signal-chain geometry.
- Balancers must preserve chain order left-to-right: `BAL1`, then `BAL2`, then `BAL3`.
- Balancers must stay on their branch risers; only the riser can move them.
- Any balancer icon and any square stomp block on the same vertical band must keep the same horizontal clearance as the standard gap between adjacent stomp-box squares.
- Reduce available branch span or move the riser before allowing a balancer to overlap an adjacent square FX block.
- Vertical flow columns are based on the visible signal rectangle, not the raw widget left edge, so `FX1`, `FX2`, `FX3`, `CMP`, and other square flow blocks line up by their square bodies even when asset padding differs.
- Balancer collision and downstream spacing must be measured from the visible balancer circle/signal rect, not the full widget frame, so a balancer does not reserve more horizontal room than the adjacent stomp square.
- When compacting the segment after a balancer, compare the balancer's visible right signal anchor to the next visible stomp's visible left signal anchor. Never compare against the stomp widget origin, because square artwork padding will falsely add extra space.

### Signal-Chain Vertical Column Contract

- Define shared vertical columns from visible signal geometry, not widget origin geometry.
- Use these columns as layout guides for all patches so repeated block families land in the same visual channels whenever topology permits.

- Column `C0`: source blocks
  - `INST1`, `INST2`, `INST3`, `NORMAL`
  - All source rectangles share one visible left edge.

- Column `C1`: first branch FX lane
  - `FX1`, `FX2`, `FX3`, `CMP`
  - These square blocks must share one visible left edge.
  - This is the first major guide the eye reads after the source column.
  - **The first stomp block on every instrument branch (INST1, INST2, INST3, NORMAL) must start at C1 — immediately to the right of the source block with only the standard `instToFxGap` between them.**
  - If an instrument branch has no stomp blocks before its BAL merge, the wire runs directly from the source to BAL; no phantom space is inserted.
  - Stomp blocks must never float right to align with a downstream balancer or match another row's column. They always pack left from C1.

- Column `C2`: first merge lane
  - `BAL1`
  - `BAL1` sits on the first merge riser, visually centered between the `INST1/INST2` inputs and its output.

- Column `C3`: second branch processing lane
  - Hidden branch marker / optional square block immediately after `BAL1`
  - If no visible square block exists here, do not create fake width; keep the next visible block compact.

- Column `C4`: second merge lane
  - `BAL2`
  - `BAL2` sits on the second merge riser and must read as the next structural merge after `BAL1`.

- Column `C5..Cn`: post-merge serial FX lanes
  - `EQ1`, `FV1`, `DLY1`, `M DLY`, `REV`, `CHO`, `FX1`, etc.
  - Visible square blocks in this region use the standard stomp gap and occupy one column each.

- Column `Cb3`: final merge lane
  - `BAL3`
  - `BAL3` is the last merge before the divider/output section.
  - The branch riser for `BAL3` must be vertical and centered through the balancer artwork.

- Column `Cd..Co`: output section
  - divider, mixer, stereo/sub outputs, master
  - Divider/mixer circles may be visually different, but their left-to-right cadence should still match the serial stomp spacing as closely as the artwork allows.

- Placement rules
  - A visible square stomp block always snaps to the nearest legal shared column for its lane family.
  - Balancers occupy dedicated merge columns; square blocks may not steal merge columns.
  - If a patch omits a block in a column, the next visible block may compact left, but it should compact to the next legal column, not to an arbitrary x offset.
  - The left edge used for column snapping is the painted flow body / visible signal rect, not transparent asset padding.
  - Column spacing should feel uniform from `C1` forward; balancers should not visually consume more horizontal room than the adjacent square stomp columns.

### Signal-Chain Topology Contract

- Treat the signal chain as a directed graph, not a loose list of widget positions.
- Each visible node must have:
  - a stable `id`
  - a node kind: `source`, `fx`, `balancer`, `divider`, `mixer`, `output`
  - an assigned lane
  - an assigned column/order
  - a visible signal rectangle
  - explicit input/output anchors derived from that visible signal rectangle
- Geometry must flow in one direction only:
  - patch data -> normalized topology -> lane/column assignment -> node geometry -> routed connector lines -> paint
- Widget geometry must not be used as the source of truth for topology.

### Signal-Chain Anchor Rules

- Every visible non-source node must show at least one incoming connector at its input-side anchor.
- Every visible non-terminal node must show at least one outgoing connector at its output-side anchor.
- Connector lines must terminate on signal anchors, not hidden node centers.
- For normal square stomp blocks, the signal anchors are the mid-left and mid-right points of the visible signal rectangle.
- For balancers:
  - the vertical branch riser passes through the balancer's painted flow-rect centerline so the top and bottom connectors enter vertically into the visible circle
  - the merged output starts from the balancer output-side anchor
  - the visible balancer icon must sit between those two anchor sides, not hide the whole connection
- Divider and mixer routing may still use center routing internally, but their incoming and outgoing chain links must be anchored from visible signal geometry.

### Signal-Chain Structural Invariants

- `BAL1` merges `INST1` and `INST2`.
- `BAL2` merges `BAL1` output and `INST3`.
- `BAL3` merges `BAL2` output and `NORMAL`.
- Therefore the balancer X ordering is structural and must never invert:
  - `BAL1.x < BAL2.x < BAL3.x`
- Hidden branch markers do not participate in collision checks or visible connector routing.
- Patch changes may rebuild the topology and geometry.
- Lower-page edits may repaint labels or values but must not rebuild the upper topology unless the actual chain structure changed.

### Signal-Chain Validation Loop

- Validate the upper chain from live patch loads, not from one static screenshot.
- For regression checks:
  - capture the first visible loaded patch as the baseline
  - move eight patches up in the left browser and load that patch
  - capture the loaded result after the patch request settles
  - repeat one more patch jump if needed, because some geometry bugs only appear on the second patch switch
- On every captured comparison, verify:
  - the loaded patch name/number changed in the header, not just the browser highlight
  - `BAL1 -> BAL2 -> BAL3` still reads left-to-right
  - every visible source or stomp node has a connector entering or leaving at a visible signal anchor
  - incoming chain links land on the divider's left-side signal anchor
  - outgoing chain links leave from the mixer's right-side signal anchor
  - the final master/output block stays on-page without stretching the preceding segment disproportionately

## 6) Concrete FloorBoard changes (high impact)

1. Standardize `QGridLayout` spacing/margins in edit pages (`editPage.cpp`) to tokens above.
2. Unify control cell construction in knob widgets (`customControlKnob.cpp`, `customControlDataKnob.cpp`) so label/value spacing is constant.
3. Add a reusable left-edge switch rail / block header pattern for all effect pages.
4. Prevent reflow when params are unavailable; render dimmed placeholders instead.
5. Replace ad-hoc font sizes with 3 text roles: title, value, label.

## 6a) Canonical CHO Page Contract

Use the dedicated CHO / Chorus page as the first replacement template for the old boxed layouts.

- Header
  - Shared colored title rail with power button at far left.
  - No inline power widget inside the content body.

- Selector strip
  - One thin row directly under the header.
  - Left-aligned `MODE` selector rendered inline, not in its own boxed panel.
  - `MODE` is the only dropdown on the CHO page.
  - One divider line below the selector row.

- Parameter field
  - No nested `QGroupBox` sections for Chorus, Filter, or Level.
  - One flat control matrix below the divider.
  - The matrix starts at the far left edge under the divider with no mode-specific horizontal offset.
  - Uniform control cells, left-to-right in a strict grid, with no section nesting.
  - Secondary rows remain in the same grid and are dimmed only when unavailable.

- Fixed row types
  - `selector_row`: inline `MODE` only.
  - `matrix_row`: contiguous left-aligned cells with even horizontal spacing.
  - Simple chorus modes use:
    - Matrix row: `RATE`, `DEPTH`, `PRE-DELAY`, `EFFECT LEVEL`, `WAVEFORM`, `LOW CUT`, `HIGH CUT`, `DIRECT LEVEL`, `OUTPUT MODE`
  - `DUAL` mode uses:
    - Row 1: channel 1 in the same knob-only order as the simple page
    - Row 2: channel 2 in the same knob-only order as the simple page

- Width and spacing rules
  - No page may exceed the visible edit-pane width just because controls are side-by-side.
  - Selector strip and parameter grid use separate height classes.
  - Keep one CHO spacing scale only: `space_8` inside the selector strip, `space_20` between CHO knob cells, `space_12` between selector strip and divider, `space_16` from divider to the first matrix row.
  - No right-edge clipping, no overlapping labels, and no mode-specific horizontal offsets.
  - Every CHO parameter except `MODE` renders as a knob, including `WAVEFORM`, `LOW CUT`, and `OUTPUT MODE`.

- Implementation rule
  - `stompbox_ce.cpp` is the canonical first conversion.
  - Future `FX` pages should follow the CHO contract before any instrument pages are reworked.

## 6b) Stomp Page Conversion Strategy

Use the CHO rewrite as the reference implementation for converting the other stomp pages away from boxed legacy layouts.

- Conversion order
  - Start with stomp/effect pages in `src/stompbox_*.cpp`.
  - Do not mix stomp-page cleanup with instrument-page redesign.
  - Convert one stomp page at a time and validate its signal tile, page controls, and disabled states before moving on.

- Page architecture
  - Keep the shared header rail and header power button behavior.
  - Put mode/type/variation selection into a single thin selector strip under the header.
  - Add one divider below the selector strip.
  - Put all remaining parameters into one flat matrix below the divider.
  - Remove decorative `QGroupBox` containers unless a page truly needs a separate logical mode stack.

- Control rules
  - Default to knobs for all page parameters.
  - Keep dropdowns only when the underlying parameter is a real enumerated selector that benefits from direct list selection.
  - If a control is available in some modes but not others, keep its cell position and dim it instead of reflowing the row.
  - Keep the same label/value/knob stack for every cell.

- Grid rules
  - Left-anchor the matrix to the divider, not to the visual center of the page.
  - Use fixed row types per page, then keep those row types stable across all modes.
  - Keep one column order for a page family and reuse it for every mode variant.
  - Reduce spacing before reducing control count.
  - Never let a mode-specific row stretch to fill extra width if that creates uneven gaps or right-edge clipping.

- Implementation pattern
  - In the stomp page file, define a small local spec struct for matrix controls when the page has repeated mode layouts.
  - Build repeated rows through local helper functions in the same stomp file first.
  - Only add a shared `editPage` helper when the helper solves a real layout behavior problem for that stomp conversion.
  - Shared helpers must be generic layout primitives, not effect-specific abstractions.

- Minimal helper policy
  - Prefer page-local helpers for address lists, row definitions, and repeated knob creation.
  - Promote behavior into `editPage.cpp` only for reusable mechanics such as stack spacing, content-width locking, or alignment control.
  - Do not introduce a global helper just to save a few lines in one stomp page.

- Migration checklist per page
  - Identify the canonical Boss/manual row order first.
  - Map every existing SysEx address to a fixed matrix position before changing widgets.
  - Rebuild the selector strip.
  - Rebuild the parameter matrix with no extra section boxes.
  - Verify there is no right-edge clipping at the target window size.
  - Verify labels do not overlap and disabled controls do not collapse the grid.
  - Verify the page power state matches the signal-chain tile state.

- Acceptance bar for future stomp conversions
  - One header rail, one selector strip, one divider, one flat matrix.
  - Stable control order across mode changes.
  - No red selector backplates or decorative hover blocks.
  - No boxed sub-sections unless the hardware UI clearly presents a separate section.
  - No clipping, no overlap, no ad-hoc per-page spacing constants outside the documented token scale.

## 6c) Instrument Page Family Contract

Use `INST 1` as the canonical layout source for `INST 2` and `INST 3`. Instrument pages should share one shell and one page-family contract per instrument type rather than each file defining its own variant.

- Reference-derived shell rules from Boss screenshots
  - The instrument shell is a 4-band layout:
    - band 1: colored header rail with power icon, title at left, optional variation dropdown at far right
    - band 2: thin selector strip with `INST TYPE` at left and the family subnav immediately to its right
    - band 3: one divider line spanning the working width
    - band 4: the selected page body, left-anchored and using the full remaining width
  - No band may be skipped by inserting blank vertical space.
  - The shell is compact vertically:
    - header rail stays visually shallow
    - selector strip sits close under the header
    - divider sits close under the selector strip
    - the first control row starts soon after the divider, not after a large empty gutter
  - The body uses width before it uses height.
    - If a page is tall, reduce horizontal gaps first by using the available right-side real estate.
    - Do not stack controls downward while large unused space remains to the right.
  - No scroll containers for normal instrument pages.
    - The intended layout is a single page-sized composition.
    - `SEQ` data tables and long curve strips still fit the page width contract rather than introducing nested viewports.

- Shell rules
  - Use one instrument header rail with the shared header power button only.
  - Put `INST TYPE` on the thin selector strip under the header.
  - Put the instrument-family subtab row on the same strip to the right of `INST TYPE`.
  - `INST TYPE` owns a fixed left block; the family subnav starts immediately to its right and takes the remaining width.
  - The `INST TYPE` selector text may not clip, and the first tab may not overlap or underlap the selector hit area.
  - Add one divider below the selector strip.
  - The selected family page body renders below the divider with no outer `QGroupBox`.
  - All shell content aligns to one left guide under the stomp/instrument tile body; nothing on the page body should start farther right without a reason.
  - Labels on the selector strip are compact and close to their controls.
    - Do not leave a large blank gap between `INST TYPE` and its dropdown.
    - `TUNING TYPE`, `WAVEFORM`, `SHAPE`, `TYPE`, and similar selector labels must sit close to their dropdowns on the same strip.
  - The family subnav must always remain visible and clickable.
    - No overlap from the `INST TYPE` combo.
    - No hidden tab arrows.
    - No clipping at the right edge of the strip.

- Reference-derived body rules from Boss screenshots
  - Use explicit page types, not one generic spacing recipe for every page:
    - compact knob matrix page
    - selector row + compact knob matrix page
    - selector row + two-row matrix page
    - wide sequencer/grid page
  - Knob pages use tight, even spacing.
    - Controls feel evenly distributed left-to-right.
    - Top and bottom label/value boxes remain readable without needing extra whitespace.
  - Wide pages such as `SEQ` use almost the full width and divide it intentionally:
    - target row / global row across the top
    - section headers centered over each block (`SEQ1`, `SEQ2`)
    - data table spans the available width
    - lower curve strip spans the available width
  - Sparse pages such as `LAYER` and `AMP` do not expand controls to fake-fill the page.
    - Keep controls left-anchored and compact.
    - Empty space can remain at the right if the Boss reference does the same.
  - Selector-heavy rows still use compact dropdown widths.
    - Dropdown width should fit the expected text plus icon, not consume a whole page column.
    - Do not let dropdowns become wider than the content they commonly display.
  - No control may look stranded.
    - Every control belongs to a visible row or region.
    - No floating knobs in the middle of unused space.
    - No hidden last row, clipped value box, or truncated label.

- Dynamic Synth family rules
  - `INST 1`, `INST 2`, and `INST 3` must expose the same page order:
    - `OSC`, `FILTER`, `LFO1`, `SEQ1`, `SEQ2`, `LAYER`, `ALT TUNE`, `COMMON`, `AMP`, `LFO2`
  - The visible static tabs map to that fixed order:
    - `COMMON -> 7`
    - `ALT TUNE -> 6`
    - `OSC -> 0`
    - `FILTER -> 1`
    - `AMP -> 8`
    - `LFO1 -> 2`
    - `LFO2 -> 9`
    - `SEQ -> 3`
    - `LAYER -> 5`
  - `COMMON`, `AMP`, `LFO1`, `LFO2`, and `LAYER` must use the same compact flat layout on all three instrument pages.

- Shared helper rules
  - `stringLevels()` must render one flat two-row matrix with no per-string boxes.
  - `altTuning()` should be treated as a shared family page and kept visually consistent across all three instrument files.
  - Legacy instrument-type bodies that still use `newGroupBox()/addGroupBox()` may keep the logical grouping, but they must render as flat titled regions:
    - no framed decorative border
    - no oversized title gutter
    - compact margins and spacing so the page uses the full width before adding rows
  - This flat-group treatment applies across all instrument types in `INST 1`, `INST 2`, and `INST 3`:
    - `DYNAMIC SYNTH`
    - `OSC SYNTH`
    - `GR-300`
    - `E.GUITAR`
    - `ACOUSTIC`
    - `E.BASS`
    - `VIO GUITAR`
    - `POLY FX`
  - `altTuning()` should use one compact matrix:
    - top selector row for `TUNING TYPE`
    - divider
    - one compact control row with `STR BEND SW`, `BEND CONTROL`, then per-string `PITCH` knobs
    - one per-string `BEND DEPTH` row directly under the matching `PITCH` knobs
    - one per-string `FINE` row below the bend row
  - `ALT TUNE` should not render a separate inline `ON/OFF`; the page relies on the header power treatment instead.
  - The custom pitch/fine/bend grid stays gated by the existing `USER` tuning type override, but the grid position must remain fixed.
  - Shared helper pages should not own header power behavior.

- Implementation rules
  - Add pages in the same stack order across `soundSource_inst1.cpp`, `soundSource_inst2.cpp`, and `soundSource_inst3.cpp`.
  - After building the family stack, set the default Dynamic Synth page to `COMMON`.
  - Call `clearPowerControl()` before `addPage()` so the shared header power button remains the only power affordance.

### GR-300 Family Contract

- Apply the same `GR-300` body layout in `INST 1`, `INST 2`, and `INST 3`; only the SysEx page address may differ.
- Do not use decorative `QGroupBox` sections for:
  - `GR-300 Synth`
  - `Envelope Modulation`
  - `Pitch`
  - `Vibrato`
  - `Filter`
- Use one compact left-anchored matrix instead:
  - row 1:
    - `MODE`
    - `COMP`
    - `FILTER CUTOFF`
    - `FILTER RESONANCE`
    - `ENV MOD SW`
    - `ENV MOD SENS`
    - `ENV MOD ATTACK`
    - `LOW CUT`
    - `HIGH CUT`
  - divider
  - row 2, mode-gated by the existing `MODE == 02` override:
    - `PITCH SW`
    - `PITCH A`
    - `PITCH A FINE`
    - `PITCH B`
    - `PITCH B FINE`
    - `DUET SW`
    - `SWEEP SW`
    - `SWEEP RISE`
    - `SWEEP FALL`
  - row 3, also gated by the existing `MODE == 02` override:
    - `VIBRATO RATE`
    - `VIBRATO DEPTH`
    - `VIBRATO SW`
- Preserve the existing overrides and addresses exactly; only the visual structure changes.

### Family Rewrite Workflow

- Fix one instrument family at a time.
- For that family only:
  - inventory every existing control and override in all three instrument files
  - confirm the family body order is the same in `INST 1`, `INST 2`, and `INST 3`
  - define one compact row/column contract before editing code
  - replace the boxed body with one flat left-anchored matrix
  - preserve all original SysEx addresses and override rules
  - build, deploy, kill, and reopen the installed app
  - verify the exact family live before touching the next family
- Do not count shared style/helper changes as completion for a family rewrite.
- Do not mix signal-chain edits into an instrument-family pass.

### Instrument Page Audit Queue

- Audit and fix `INST 1`, then mirror to `INST 2`, then `INST 3`.
- For each instrument type family, verify:
  - shell row is intact: `INST TYPE` visible, family subnav visible, no overlap
  - all family tabs switch correctly
  - no controls are cut off
  - dropdown widths are content-sized
  - body uses width before height
  - no decorative boxes remain unless the hardware clearly shows a separate region

- Current family queue
  - `DYNAMIC SYNTH`
    - pages: `COMMON`, `ALT TUNE`, `OSC`, `FILTER`, `AMP`, `LFO1`, `LFO2`, `SEQ`, `LAYER`
  - `OSC SYNTH`
    - body tabs/pages from table `12`
    - plus shared `STRING` and `ALT TUNE`
  - `GR-300`
    - body tabs/pages from table `13`
    - plus shared `STRING` and `ALT TUNE`
  - `E.GUITAR`
    - `COMMON`, `ALT TUNE`, `GUITAR`, `AMP`, `NS`, `EQ`
  - `ACOUSTIC`
    - body tabs/pages from table `15`
    - plus shared `STRING` and `ALT TUNE`
  - `E.BASS`
    - bass-mode guitar/bass family pages plus shared `COMMON`, `ALT TUNE`, `AMP`, `NS`, `EQ`
  - `VIO GUITAR`
    - body tabs/pages from table `17`
    - plus shared `STRING` and `ALT TUNE`
  - `POLY FX`
    - body tabs/pages from table `18`
    - plus shared `STRING` and `ALT TUNE`

### Morning Working Order

- Continue with one page at a time, in this order:
  - `INST 1 DYNAMIC SYNTH`
    - `SEQ`
    - `LAYER`
    - `LFO1`
    - `LFO2`
    - `OSC`
    - `FILTER`
    - `AMP`
    - `ALT TUNE`
    - `COMMON`
  - mirror each corrected page to `INST 2` and `INST 3`
  - then move to `OSC SYNTH`
  - then `GR-300`
  - then `E.GUITAR`
  - then `ACOUSTIC`
  - then `E.BASS`
  - then `VIO GUITAR`
  - then `POLY FX`

## 7) File targets in this repo

- `src/editPage.cpp`
- `src/customControlKnob.cpp`
- `src/customControlDataKnob.cpp`
- `src/customControlParaEQ.cpp`
- `qss/black.qss`
- `qss/blue.qss`
- `qss/white.qss`
- `qss/bts.qss`
- `qss/green.qss`

## 8) Acceptance checks for redesign

- Top and bottom of all boxes on a row align.
- Pages with an on/off state always place the switch at the far left of the first row.
- All line-to-box intersections hit mid-left/mid-right edges.
- Row spacing is identical between sections.
- Changing control availability does not move neighboring controls.
- Labels and values stay readable at app minimum width.
