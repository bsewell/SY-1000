# SY-1000 Original App UI Notes (Layout, Spacing, Terminology)

This note captures practical UI patterns from the BOSS SY-1000 screenshots and translates them into implementation guidance for SY-1000 FloorBoard.

## 1) Visual/Interaction Patterns to Copy

- Header rail per block
  - Fixed-height title bar with: power icon, block name, optional Variation selector.
  - Strong section identity via muted accent color (not neon).

- Two-zone parameter layout
  - Zone A (optional): mode/type selectors (combo row) under header.
  - Zone B: knob matrix below a divider line.

- Predictable control cell
  - Every control cell has the same stack:
    1) Value dial (or value ring)
    2) Numeric/text value centered
    3) Label in uppercase
  - Disabled controls keep position but are dimmed (no reflow).

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

## 6) Concrete FloorBoard changes (high impact)

1. Standardize `QGridLayout` spacing/margins in edit pages (`editPage.cpp`) to tokens above.
2. Unify control cell construction in knob widgets (`customControlKnob.cpp`, `customControlDataKnob.cpp`) so label/value spacing is constant.
3. Add a reusable block header widget style for all effect pages.
4. Prevent reflow when params are unavailable; render dimmed placeholders instead.
5. Replace ad-hoc font sizes with 3 text roles: title, value, label.

## 7) File targets in this repo

- `/Users/bsewell/010 MUSIC STUDIO /SY-1000/editPage.cpp`
- `/Users/bsewell/010 MUSIC STUDIO /SY-1000/customControlKnob.cpp`
- `/Users/bsewell/010 MUSIC STUDIO /SY-1000/customControlDataKnob.cpp`
- `/Users/bsewell/010 MUSIC STUDIO /SY-1000/customControlParaEQ.cpp`
- `/Users/bsewell/010 MUSIC STUDIO /SY-1000/qss/black.qss`
- `/Users/bsewell/010 MUSIC STUDIO /SY-1000/qss/blue.qss`
- `/Users/bsewell/010 MUSIC STUDIO /SY-1000/qss/white.qss`
- `/Users/bsewell/010 MUSIC STUDIO /SY-1000/qss/bts.qss`
- `/Users/bsewell/010 MUSIC STUDIO /SY-1000/qss/green.qss`

## 8) Acceptance checks for redesign

- Top and bottom of all boxes on a row align.
- All line-to-box intersections hit mid-left/mid-right edges.
- Row spacing is identical between sections.
- Changing control availability does not move neighboring controls.
- Labels and values stay readable at app minimum width.
