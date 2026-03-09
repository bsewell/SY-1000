# SY-1000 Original App UI Notes (Layout, Spacing, Terminology)

This note captures practical UI patterns from the BOSS SY-1000 screenshots and translates them into implementation guidance for SY-1000 FloorBoard.

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

## 5a) Signal-Chain Geometry Rules

- Treat the upper signal chain as a separate geometry system from the lower edit pages.
- Patch changes may rebuild the upper signal chain; lower-page parameter edits must not disturb upper signal-chain geometry.
- Balancers must preserve chain order left-to-right: `BAL1`, then `BAL2`, then `BAL3`.
- Balancers must stay on their branch risers; only the riser can move them.
- Any balancer icon and any square stomp block on the same vertical band must keep at least `50 px` horizontal clearance edge-to-edge.
- Reduce available branch span or move the riser before allowing a balancer to overlap an adjacent square FX block.

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

## 7) File targets in this repo

- `/Users/bsewell/010 MUSIC STUDIO /SY-1000/src/editPage.cpp`
- `/Users/bsewell/010 MUSIC STUDIO /SY-1000/src/customControlKnob.cpp`
- `/Users/bsewell/010 MUSIC STUDIO /SY-1000/src/customControlDataKnob.cpp`
- `/Users/bsewell/010 MUSIC STUDIO /SY-1000/src/customControlParaEQ.cpp`
- `/Users/bsewell/010 MUSIC STUDIO /SY-1000/qss/black.qss`
- `/Users/bsewell/010 MUSIC STUDIO /SY-1000/qss/blue.qss`
- `/Users/bsewell/010 MUSIC STUDIO /SY-1000/qss/white.qss`
- `/Users/bsewell/010 MUSIC STUDIO /SY-1000/qss/bts.qss`
- `/Users/bsewell/010 MUSIC STUDIO /SY-1000/qss/green.qss`

## 8) Acceptance checks for redesign

- Top and bottom of all boxes on a row align.
- Pages with an on/off state always place the switch at the far left of the first row.
- All line-to-box intersections hit mid-left/mid-right edges.
- Row spacing is identical between sections.
- Changing control availability does not move neighboring controls.
- Labels and values stay readable at app minimum width.
