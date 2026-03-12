# SY-1000 Dropdown Inventory

This file isolates the effect-detail dropdown system so future typography and layout changes can be made from one place instead of chasing individual pages.

## Shared implementation

- `src/editPage.cpp` - `addComboBox(...)` is the single entry point used by stomp/effect detail pages.
- `src/customControlListMenu.cpp` / `src/customControlListMenu.h` - shared effect-detail dropdown widget, label stack, sizing, and SysEx write-back.
- `src/customComboBox.cpp` / `src/customComboBox.h` - popup sizing behavior for the shared dropdown.

## Typography control point

- Effect-detail selectors are intentionally centralized in `src/customControlListMenu.cpp`.
- `customControlListMenu::applyComboFont(...)` is the place to change selector text size.
- The current pass reduces selector text by exactly `2pt` from the old large/small bases so the selected value does not overpower the label above it.
- Width sizing for the collapsed selector now also uses that reduced combo font, so the box width tracks the visible text instead of an older, larger widget font.

## Scope

- The inventory below covers stomp/effect detail selectors created through `addComboBox(...)`.
- No `stompbox_*.cpp` or `soundSource_*.cpp` file creates a `QComboBox` directly; they all route through `addComboBox(...)`.
- Other dropdown families still exist elsewhere in the app and are listed separately so future work does not confuse them with stomp/effect selectors.

## Direction variants in use

- `bottom` - 39 uses
- `default` - 289 uses
- `icon_large_40` - 1 uses
- `icon_seq` - 96 uses
- `large` - 236 uses
- `large_inst` - 3 uses
- `pu_sel` - 3 uses
- `pu_sel_bmode_g` - 3 uses
- `pu_sel_gmode_g` - 3 uses

## Effect-detail files with dropdowns

- `soundSource_inst1.cpp` - 122 dropdowns; first entries: `large_inst`, `default`, `default`, `default`
- `soundSource_inst2.cpp` - 122 dropdowns; first entries: `large_inst`, `default`, `default`, `default`
- `soundSource_inst3.cpp` - 122 dropdowns; first entries: `large_inst`, `default`, `default`, `default`
- `soundSource_normal.cpp` - 2 dropdowns; first entries: `large`, `large`
- `stompbox_amp.cpp` - 5 dropdowns; first entries: `large`, `bottom`, `large`, `bottom`
- `stompbox_ce.cpp` - 7 dropdowns; first entries: `large`, `default`, `default`, `default`
- `stompbox_cs.cpp` - 1 dropdowns; first entries: `large`
- `stompbox_dd3.cpp` - 2 dropdowns; first entries: `large`, `large`
- `stompbox_divider.cpp` - 2 dropdowns; first entries: `large`, `large`
- `stompbox_ds.cpp` - 1 dropdowns; first entries: `large`
- `stompbox_fx1.cpp` - 66 dropdowns; first entries: `large`, `large`, `large`, `large`
- `stompbox_fx2.cpp` - 66 dropdowns; first entries: `large`, `large`, `large`, `large`
- `stompbox_fx3.cpp` - 66 dropdowns; first entries: `large`, `large`, `large`, `large`
- `stompbox_lp.cpp` - 2 dropdowns; first entries: `large`, `large`
- `stompbox_main_out_left.cpp` - 6 dropdowns; first entries: `large`, `large`, `large`, `large`
- `stompbox_main_out_right.cpp` - 2 dropdowns; first entries: `large`, `large`
- `stompbox_master.cpp` - 68 dropdowns; first entries: `large`, `large`, `icon_large_40`, `large`
- `stompbox_ns.cpp` - 1 dropdowns; first entries: `bottom`
- `stompbox_rv.cpp` - 2 dropdowns; first entries: `large`, `default`
- `stompbox_sub_out_left.cpp` - 6 dropdowns; first entries: `large`, `large`, `large`, `large`
- `stompbox_sub_out_right.cpp` - 2 dropdowns; first entries: `large`, `large`

## Effect-detail files with no dropdowns

- `stompbox_balancer1.cpp`
- `stompbox_balancer2.cpp`
- `stompbox_balancer3.cpp`
- `stompbox_branch_AB.cpp`
- `stompbox_dd1.cpp`
- `stompbox_dd2.cpp`
- `stompbox_eq1.cpp`
- `stompbox_eq2.cpp`
- `stompbox_fv1.cpp`
- `stompbox_fv2.cpp`
- `stompbox_main_out_stereo.cpp`
- `stompbox_mixer.cpp`
- `stompbox_split.cpp`
- `stompbox_sub_out_stereo.cpp`

## Other dropdown families outside stomp/effect selectors

- `src/customTargetListMenu.cpp` - target selector dropdown used by assign pages through `addTarget(...)`
- `src/editWindow.cpp` - page selector dropdown in the detail window header
- `src/initPatchListMenu.cpp` - quick-load patch dropdown
- `src/bulkEditDialog.cpp`, `src/bulkLoadDialog.cpp`, `src/bulkSaveDialog.cpp`, `src/patchWriteDialog.cpp` - utility dialog dropdowns
- `src/preferencesPages.cpp` - MIDI port selectors

These are intentionally documented here so future combo-box work can distinguish:

1. stomp/effect detail selectors
2. assign/target selectors
3. utility-dialog selectors
4. application/global selectors

## High-volume dropdown pages

### `soundSource_inst1.cpp` (122)
- line 96: `large_inst` - type
- line 116: `default` - wave type
- line 148: `default` - type
- line 168: `default` - shape
- line 176: `default` - pitch sync
- line 181: `default` - shape
- line 189: `default` - pitch sync
- line 196: `default` - pitch target
- line 197: `default` - cutoff target
- line 198: `default` - level target
- line 327: `icon_seq` - 1 curve seq 1
- line 328: `icon_seq` - 2
- ... 110 more in this file

### `soundSource_inst2.cpp` (122)
- line 92: `large_inst` - type
- line 123: `default` - wave type
- line 155: `default` - type
- line 175: `default` - shape
- line 183: `default` - pitch sync
- line 188: `default` - shape
- line 196: `default` - pitch sync
- line 203: `default` - pitch target
- line 204: `default` - cutoff target
- line 205: `default` - level target
- line 334: `icon_seq` - 1 curve seq 1
- line 335: `icon_seq` - 2
- ... 110 more in this file

### `soundSource_inst3.cpp` (122)
- line 91: `large_inst` - type
- line 121: `default` - wave type
- line 153: `default` - type
- line 173: `default` - shape
- line 181: `default` - pitch sync
- line 186: `default` - shape
- line 194: `default` - pitch sync
- line 201: `default` - pitch target
- line 202: `default` - cutoff target
- line 203: `default` - level target
- line 332: `icon_seq` - 1 curve seq 1
- line 333: `icon_seq` - 2
- ... 110 more in this file

### `stompbox_master.cpp` (68)
- line 66: `large` - tempo hold
- line 67: `large` - key
- line 68: `icon_large_40` - gk/normal patch type
- line 72: `large` - GK Set
- line 74: `large` - normal set
- line 81: `default` - channel
- line 82: `default` - patch
- line 83: `default` - msb
- line 84: `default` - lsb
- line 86: `default` - cc#1
- line 87: `default` - cc# 1 value
- line 88: `default` - cc#2
- ... 56 more in this file

### `stompbox_fx1.cpp` (66)
- line 84: `large` - type
- line 100: `large`
- line 118: `large`
- line 123: `large`
- line 143: `large` - mode
- line 149: `large` - output
- line 161: `default` - waveform
- line 175: `default` - waveform
- line 189: `default` - waveform
- line 203: `default` - waveform
- line 214: `default` - waveform
- line 250: `large`
- ... 54 more in this file

### `stompbox_fx2.cpp` (66)
- line 81: `large` - type
- line 97: `large`
- line 115: `large`
- line 120: `large`
- line 140: `large` - mode
- line 146: `large` - output
- line 158: `default` - waveform
- line 172: `default` - waveform
- line 186: `default` - waveform
- line 200: `default` - waveform
- line 211: `default` - waveform
- line 247: `large`
- ... 54 more in this file

### `stompbox_fx3.cpp` (66)
- line 83: `large` - type
- line 99: `large`
- line 117: `large`
- line 122: `large`
- line 142: `large` - mode
- line 148: `large` - output
- line 160: `default` - waveform
- line 174: `default` - waveform
- line 188: `default` - waveform
- line 202: `default` - waveform
- line 213: `default` - waveform
- line 249: `large`
- ... 54 more in this file

### `stompbox_ce.cpp` (7)
- line 72: `large` - mode
- line 88: `default` - waveform
- line 103: `default` - waveform
- line 118: `default` - waveform
- line 134: `default` - waveform
- line 147: `default` - waveform
- line 155: `large` - output

### `stompbox_main_out_left.cpp` (6)
- line 63: `large` - stereo link
- line 66: `large` - eq link
- line 82: `large` - output select
- line 85: `large` - phase left
- line 91: `large` - output select
- line 94: `large` - phase left

### `stompbox_sub_out_left.cpp` (6)
- line 62: `large` - stereo link
- line 65: `large` - eq link
- line 81: `large` - output select
- line 84: `large` - phase left
- line 90: `large` - output select
- line 93: `large` - phase left
