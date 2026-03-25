# SY-1000 FloorBoard — UX Improvement Research Report

Compiled from: Boss TONE STUDIO (73 CSS files), SYUI screenshots (31 PNGs),
current QSS themes (5 files), source code, and all existing layout docs.

> **Status note**: Gap analysis in §3 reflects C++ widget state as of early 2026-03-xx. QML pages migrated
> in Phase 5 (`HANDOFF.md`) may have addressed some color, typography, and tab gaps. Verify against
> current QML files before treating a gap as open. Design tokens in §1 are extracted from Boss CSS and
> remain accurate regardless of FloorBoard state.
>
> **Related docs**: Layout pattern rules → `docs/ui/sy1000_boss_layout_notes.md`

---

## 1. Boss TONE STUDIO Design Token Reference

These are the exact values extracted from the official Boss editor CSS.
All improvement work should converge toward these unless a QSS equivalent is documented.

### 1.1 Color Tokens

| Token name              | Value                          | Source CSS              |
|-------------------------|--------------------------------|-------------------------|
| bg-base                 | `#1A1C1F`                      | common.css, tab.css     |
| bg-surface              | `#292929`                      | bts-select-box.css      |
| bg-elevated             | `#252B32`                      | (pattern across files)  |
| bg-assign-band          | `#222C3D`                      | bts-ctl-exp.css         |
| bg-header-band          | `#4B4F52`                      | bts-ctl-exp.css         |
| bg-subheader            | `#313A47`                      | bts-ctl-exp.css         |
| bg-disabled-input       | `#424243`                      | bts-select-box.css      |
| accent-primary          | `#33D6FF`  ← THE BOSS CYAN     | dial.css, editor_custom |
| accent-selected         | `#2EC1E6`  (group buttons)     | group-button.css        |
| accent-selected-dim     | `#246F84`  (disabled selected) | group-button.css        |
| text-primary            | `#FFFFFF`                      | common.css              |
| text-secondary          | `rgba(255,255,255,0.80)`       | bts-select-box.css      |
| text-muted              | `rgba(255,255,255,0.64)`       | group-button.css        |
| text-disabled           | `rgba(255,255,255,0.32)`       | group-button.css        |
| border-hairline         | `rgba(255,255,255,0.32)`       | bts-menu-page.css       |
| border-prominent        | `rgba(255,255,255,0.64)`       | bts-stompbox.css        |
| hover-fill              | `rgba(255,255,255,0.16)`       | bts-select-box.css      |
| hover-fill-strong       | `rgba(255,255,255,0.32)`       | group-button.css        |
| tap-highlight           | `rgba(255,255,255,0.32)`       | common.css              |
| popup-scrim             | `rgba(0,0,0,0.86)`             | page.css                |

### 1.2 Dial / Knob Tokens

| Token                   | Value                          |
|-------------------------|--------------------------------|
| dial-fg (active arc)    | `#33D6FF`                      |
| dial-bg (track)         | `rgba(255,255,255,0.24)`       |
| dial-gap (notch)        | `#1A1C1F`                      |
| dial-na (inactive arc)  | `rgba(255,255,255,0.24)`       |
| dial-arc-radius         | `5px` (border-radius on span)  |
| dial-value-color        | `#FFFFFF`                      |
| dial-value-size         | `18px`                         |
| dial-disabled-overlay   | image: `disable_overlay@2x.png`|
| shadow-overlay          | image: `shadow_overlay@2x.png` |

### 1.3 Typography Tokens

| Role           | Font family              | Weight  | Size  | Notes                   |
|----------------|--------------------------|---------|-------|-------------------------|
| body-default   | RobotoCondensedBold      | bold    | 12pt  | body tag default        |
| regular        | RobotoCondensedRegular   | normal  | —     | `.regular-font` class   |
| light          | RobotoCondensedLight     | light   | —     | for secondary labels    |
| value-display  | (inherits bold)          | bold    | 18px  | dial value text         |
| list-item      | (inherits bold)          | bold    | 14px  | mobile list rows        |
| list-item-pc   | (inherits bold)          | bold    | 13px  | desktop list rows       |
| tab-label      | (inherits bold)          | bold    | 13px  | tab nav labels          |
| small-select   | (inherits bold)          | bold    | 12px  | dropdowns, spinners     |

Japanese fallback: NotoSansJP (Bold/Regular/Light variants also loaded)

**Current FloorBoard uses: Arial — this is the #1 typography mismatch.**

### 1.4 Spacing / Sizing Tokens (from chain.css + stompbox.css)

| Token               | Value      | Source                              |
|---------------------|------------|-------------------------------------|
| chain-block-size    | 48×48px    | chain.css                           |
| chain-block-inst    | 96×48px    | chain.css `.chain-block-inst`       |
| list-row-height-mob | 48px       | bts-stompbox.css                    |
| list-row-height-pc  | 40px       | bts-stompbox.css pc variant         |
| list-padding-left   | 48px       | bts-stompbox.css                    |
| scrollbar-width     | 8px        | common.css (desktop only)           |
| tab-height          | 48px       | tab.css (line-height 48px)          |
| tab-active-bar      | 4px        | tab.css `inset 0 -4px 0 0 #33D6FF` |
| popup-border-radius | 4px        | bts-ctl-exp.css                     |
| popup-anim          | fadeIn 0.3s| bts-ctl-exp.css                     |

### 1.5 Tab / Navigation Token

Active tab:
- text color: `#33D6FF`
- bottom underline: `box-shadow: inset 0 -4px 0 0 #33D6FF`

Inactive tab:
- text color: `rgba(255,255,255,0.8)`

Hover (desktop):
- text color: `#33D6FF`
- bottom underline: same as active

Tab container bottom border:
- `box-shadow: inset 0 -1px 0 0 rgba(255,255,255,0.64)`

**Current FloorBoard uses: `rgba(30,30,255,150)` fill for selected tab — completely wrong.**

### 1.6 Group Button / Toggle Token

Normal:
- text: `rgba(255,255,255,0.64)`
- bg: transparent
- border: `box-shadow: 0 0 0 1px #A3A3A3 inset`

Selected:
- text: `#FFFFFF`
- bg: `#2EC1E6`
- border: transparent

Hover:
- bg: `rgba(255,255,255,0.32)`

Disabled:
- text: `rgba(255,255,255,0.32)`
- border: `rgba(163,163,163,0.5)`
- no pointer events

Disabled-selected:
- text: `rgba(255,255,255,0.32)`
- bg: `#246F84`

---

## 2. SYUI Screenshot Analysis — Boss Hardware UI Patterns

### 2.1 Block Header System

Every effect block has a colored header rail:

| Block type        | Header color                        |
|-------------------|-------------------------------------|
| INST 1/2/3        | Burgundy/rust (#8B3A2A approx)      |
| EQ 1/2            | Forest green                        |
| FX 1/2/3          | Magenta/pink                        |
| Compressor        | Purple                              |
| Chorus/Delay      | Gray-blue                           |
| Master            | Dark green                          |
| Mixer/Divider     | Medium gray                         |
| Utilities         | Gray                                |

**Current FloorBoard: no per-type colored headers — all use the same background.**

Header anatomy:
1. Power icon (circle with line) — far left
2. Block name — large, condensed, bold, uppercase
3. Optional VARIATION dropdown — far right

### 2.2 Control Cell Pattern

Every parameter control follows this exact stack (top to bottom):
```
┌──────────────────┐
│   [ DIAL/KNOB ]  │  ← rotating ring, value centered inside
│      "100"       │  ← value text, centered, largest text on cell
│  PARAM NAME      │  ← label, uppercase, smaller, centered
└──────────────────┘
```

Rules:
- Disabled controls: keep cell position, reduce alpha ~35-45%, no reflow
- Active arc: `#33D6FF` (Boss cyan)
- Inactive arc: neutral gray
- Knob sizes: L=132px, M=116px, S=96px (per layout notes)

### 2.3 Selector Row (under header)

One thin row directly under header:
- Left-aligned TYPE/MODE dropdown
- No border/box around the dropdown
- Minimal left padding (≈16px)
- Divider line below separates from parameter grid

### 2.4 Parameter Grid

- Left-anchored, starts at left edge
- Strict column grid, no irregular offsets
- Column gap: 20-28px
- Row gap: 24-32px
- On/off switch always anchors far-left on first row
- No QGroupBox decorative borders — FLAT layout only
- Dense but never scrolling for normal effect pages

### 2.5 Signal Chain Icons

From chain.css and SYUI screenshots:
- Chain blocks are 48×48 squares
- Instrument blocks are 96×48 (double width)
- Selected block: flashing overlay animation (0.6s infinite alternate, 0→0.5 opacity)
- Insert point indicator: flashing animation (0.5s, alternate)
- Balancer circles: separate asset (visible circle with branch risers)

---

## 3. Gap Analysis: FloorBoard vs Boss Reference

### 3.1 Color Gaps (HIGH PRIORITY)

| Issue                          | Current FloorBoard           | Boss Reference              | Fix                                      |
|--------------------------------|------------------------------|-----------------------------|------------------------------------------|
| Text color on controls         | `rgb(185,224,243)` (light blue) | `#FFFFFF` + alpha variants  | Change to white hierarchy                |
| Active tab indicator           | Blue fill `rgba(30,30,255)`  | Cyan `#33D6FF` 4px underline| Tab style overhaul in all .qss files     |
| Dial/knob active arc           | unknown (sprite-based)       | `#33D6FF`                   | Update knob sprite or draw arc in cyan   |
| Selection background           | `rgb(50,50,150)` (dark blue) | `rgba(255,255,255,0.16)`    | Lighter, less saturated hover            |
| Accent cyan value              | Not defined in QSS           | `#33D6FF`                   | Add as named constant to all themes      |
| Background base                | `rgb(20,20,20)` / `rgb(0,0,0)` | `#1A1C1F`                 | Standardize on `#1A1C1F` for bts theme   |
| No block-type header colors    | Single background            | Category-coded colors       | Add per-type header color system         |
| Disabled alpha                 | Not defined                  | `rgba(255,255,255,0.32)`    | Add disabled opacity to all control CSS  |

### 3.2 Typography Gaps (HIGH PRIORITY)

| Issue                          | Current                      | Boss Reference              |
|--------------------------------|------------------------------|-----------------------------|
| Body/control font              | Arial (generic)              | RobotoCondensed (Bold)      |
| Font weight consistency        | "Normal"/"thin" inconsistent | Bold for all control text   |
| Value text size                | Not explicitly sized         | 18px for dial values        |
| Label size                     | Not explicitly sized         | 12-13px uppercase           |
| Title size                     | Not explicitly sized         | Large condensed bold        |

Action: embed RobotoCondensed fonts in the app resources and add font-face rules to QSS or load via Qt font system.

### 3.3 Layout Gaps (MEDIUM PRIORITY)

| Issue                          | Current                      | Boss Reference              |
|--------------------------------|------------------------------|-----------------------------|
| QGroupBox decorative borders   | Present (many effect pages)  | Flat layout only            |
| Effect page left-anchor        | Variable/centered            | Always left-anchored        |
| On/off switch position         | Variable                     | Always far-left, first      |
| Disabled control behavior      | Collapse / reflow            | Hold position, dim alpha    |
| Mode change reflow             | Likely reflowing             | Stable grid, dims unused    |
| Selector strip separation      | Mixed with params            | Thin strip + divider        |
| Row label baseline             | Inconsistent                 | Constant per row            |

### 3.4 Interaction Gaps (MEDIUM PRIORITY)

| Issue                          | Current                      | Boss Reference              |
|--------------------------------|------------------------------|-----------------------------|
| Tab active indicator           | Fill background              | Underline only (4px cyan)   |
| Hover feedback on controls     | Minimal                      | `rgba(255,255,255,0.16)` fill |
| Disabled pointer events        | Unclear                      | `pointer-events: none`      |
| Animation on popups            | None                         | fadeIn 0.3s                 |
| Chain block selection feedback | Unknown                      | Flashing overlay animation  |

### 3.5 Signal Chain Gaps (KNOWN — from agent_handoff.md)

| Issue                                                | Status        |
|------------------------------------------------------|---------------|
| EQ/FV/DLY blocks float above signal chain line       | Known bug     |
| Balancer connectors terminate under balancer          | In progress   |
| Some blocks appear visually detached/floating         | In progress   |
| Connector anchor math uses widget origin not visible rect | Documented |
| BAL1 < BAL2 < BAL3 x-order must be preserved        | Documented    |
| Click crash (recursive repaint / widget parenting)   | Known bug     |

### 3.6 Feature Gaps (vs Boss screenshots)

| Missing feature                | Evidence from screenshots           |
|--------------------------------|-------------------------------------|
| VARIATION dropdown per block   | Visible on FX1, FX2, EQ2, LFO, SEQ |
| Block type color headers       | Every block has color-coded header  |
| SEQ 16-step matrix grid        | SEQ.png shows full step grid        |
| Sequencer curve shape selector | 13 curve types (seq_curve_0..12)    |
| Dual LFO variation selector    | LFO1.png "LFO VARIATION 1&2" label  |
| Layer note-range selector      | LOWER/UPPER dropdowns in Layer.png  |

---

## 4. Concrete Improvement Plan (Priority Order)

### Phase 1: Color System (1-2 sessions, pure QSS changes, zero risk)

**Files to edit:** `qss/bts.qss`, `qss/black.qss`, `qss/blue.qss`, `qss/green.qss`, `qss/white.qss`

Changes:
1. **bts.qss** (primary theme — targets Boss visual parity):
   - Change all `rgb(20,20,20)` / `rgb(0,0,0)` backgrounds to `#1A1C1F`
   - Change `rgb(185,224,243)` text to `#FFFFFF` for primary text
   - Change tab selected from `rgba(30,30,255,150)` fill to 4px bottom box-shadow in `#33D6FF`
   - Add `#33D6FF` as a named accent (via comment/convention) — QSS has no variables
   - Change `selection-background-color` from blue to `rgba(255,255,255,16%)` equivalent

2. **All themes**:
   - Define disabled opacity rule: `opacity: 0.35` or `color: rgba(255,255,255,0.32)` on disabled states
   - Add hover rules to `QComboBox`, `QTabBar`, `QPushButton`

QSS example for tab fix:
```qss
QTabBar#pagecombobox::tab:selected {
    color: #33D6FF;
    border-bottom: 4px solid #33D6FF;
    background-color: transparent;
}
QTabBar#pagecombobox::tab:hover {
    color: #33D6FF;
}
QTabBar#pagecombobox::tab {
    color: rgba(255,255,255,0.80);
    background-color: transparent;
    padding: 4px 16px;
    border: none;
}
```

### Phase 2: Typography (1 session, QSS + possibly font loading)

1. Add RobotoCondensed to app resources (already have RobotoCondensed family on macOS via Boss app at `SY-1000 Boss/Contents/Resources/html/js/fonts/`)
2. Update `font-family: Arial` → `font-family: "Roboto Condensed", Arial` in all QSS themes
3. Define 3 text size roles as QSS size values:
   - Title: `font-size: 14px; font-weight: bold`
   - Value: `font-size: 13px`
   - Label: `font-size: 11px`

### Phase 3: Block Header Colors (2-3 sessions, C++ + QSS)

Add a per-type header color system. Each stompbox/editPage header rail should take a color based on effect category:

| Category           | QColor (suggested)     |
|--------------------|------------------------|
| Instrument (INST)  | `#6B2A1A` (burgundy)   |
| EQ                 | `#1A4A1A` (forest green)|
| FX                 | `#5A1A4A` (magenta)    |
| Compressor         | `#3A1A5A` (purple)     |
| Delay/Chorus/Reverb| `#1A2A4A` (blue-gray)  |
| Master             | `#1A3A1A` (dark green) |
| Utility (Mixer etc)| `#2A2A2A` (medium gray)|

Implementation: add a `setBlockColor(QColor)` method to editPage header widget, call from each stompbox_*.cpp constructor.

### Phase 4: Layout Normalization (multiple sessions, per-page C++)

Follow the already-documented conversion order in `sy1000_boss_layout_notes.md § Morning Working Order`:

Priority queue (per compare matrix gaps):
1. `stompbox_fx1.cpp` — FX 1 (Partial)
2. `stompbox_fx2.cpp` — FX 2 (Partial)
3. `stompbox_fx3.cpp` — FX 3 (Partial)
4. `stompbox_dd3.cpp` — Master Delay (Partial)
5. `soundSource_inst1.cpp` — INST 1 all families (Partial)
6. `stompbox_main_out_left.cpp` — Main Out (Partial)
7. `stompbox_sub_out_left.cpp` — Sub Out (Partial)

For each page, follow the CHO contract (§ 6a of boss_layout_notes.md):
- One header rail
- One selector strip
- One divider
- One flat parameter matrix

Remove all `QGroupBox` decorative containers and replace with flat titled regions.

### Phase 5: Signal Chain Geometry (active — per agent_handoff.md)

Continue in `src/floorBoard.cpp`:
1. Finish balancer anchor routing using `signalBounds()`
2. Fix EQ/FV/DLY blocks floating above chain line
3. Fix click crash (recursive repaint)

Do NOT mix with layout passes.

### Phase 6: Missing Features (after Phase 3-5 stable)

1. VARIATION dropdown per block — add to each stompbox header that Boss shows it on
2. SEQ 16-step matrix grid — complete conversion from current layout to step grid
3. Sequencer curve selector — expose all 13 curve types (assets already exist: `seq_curve_0..12.png`)

---

## 5. Quick Wins (can do right now, low risk)

### 5.1 Fix tab active color (bts.qss only)
Change:
```qss
QTabBar#pagecombobox::tab:selected {
    background-color: rgba(30, 30, 255, 150);
}
```
To:
```qss
QTabBar#pagecombobox::tab:selected {
    background-color: transparent;
    color: #33D6FF;
    border-bottom: 3px solid #33D6FF;
}
```

### 5.2 Fix primary text color (bts.qss)
Change all `rgb(185,224,243)` → `#FFFFFF` or `rgba(255,255,255,0.80)` depending on hierarchy.

### 5.3 Fix background to Boss exact value (bts.qss)
Change `rgb(0,0,0)` backgrounds → `#1A1C1F` for better contrast match.

### 5.4 Add hover states to combos (all .qss)
```qss
QComboBox#smallcombo:hover {
    background-color: rgba(255,255,255,0.08);
}
```

### 5.5 Fix selection color (all .qss)
Change:
```qss
selection-background-color: rgb(30,30,255);
```
To:
```qss
selection-background-color: rgba(51,214,255,0.32);
```

---

## 6. Assets Available but Unused

From `.qrc` audit — these assets exist but may not be fully connected:

| Asset                         | Purpose                        | Status to verify         |
|-------------------------------|--------------------------------|--------------------------|
| `seq_curve_0.png`–`seq_curve_12.png` | Sequencer curve shapes | Used in SEQ page?       |
| `effect_hover.png`            | Chain block hover state        | Connected to hover?      |
| `effect_select.png`           | Chain block selected state     | Connected to selection?  |
| `inst_hover.png`              | Instrument block hover         | Connected to hover?      |
| `inst_select.png`             | Instrument block selected      | Connected to selection?  |
| `EQ_graph.png`                | EQ visualization               | Is it rendered?          |
| `structure_2.png`             | Signal chain structure visual  | Current use?             |
| `menuPage_stomp.png`          | Menu page stomp background     | Current use?             |

---

## 7. Font Path (usable from Boss app)

The Boss TONE STUDIO app ships the exact fonts at:
```
<Boss Tone Studio install path>/Contents/Resources/html/js/fonts/
  RobotoCondensed-Bold.ttf
  RobotoCondensed-Regular.ttf
  RobotoCondensed-Light.ttf
  NotoSansJP-Bold.otf
  NotoSansJP-Regular.otf
  NotoSansJP-Light.otf
```

These can be copied into `SY-1000/images/` or a new `fonts/` directory and loaded in the Qt app via `QFontDatabase::addApplicationFont()` in `mainWindow.cpp`.

---

## 8. Summary Table

| Priority | Area              | Effort | Risk  | Impact |
|----------|-------------------|--------|-------|--------|
| 1        | Tab active color  | 5 min  | None  | High   |
| 2        | Text color to white| 15 min | None | High   |
| 3        | Background to #1A1C1F | 10 min | None | Medium |
| 4        | Selection/hover colors | 15 min | None | Medium |
| 5        | Font → RobotoCondensed | 1 hr | Low  | High   |
| 6        | Block header colors | 2-3 hr | Low | High   |
| 7        | Remove QGroupBox borders | per page | Medium | High |
| 8        | Flat parameter grid | per page | Medium | High |
| 9        | Signal chain fix  | ongoing | Medium | Critical |
| 10       | VARIATION dropdowns | per block | Low | Medium |
