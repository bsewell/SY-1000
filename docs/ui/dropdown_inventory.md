# SY-1000 Dropdown Inventory

This file isolates the effect-detail dropdown system so future typography and layout changes can be made from one place instead of chasing individual pages.

## Shared implementation

- `src/editPage.cpp` - `addComboBox(...)` is the single entry point used by stomp/effect detail pages.
- `src/customControlListMenu.cpp` / `src/customControlListMenu.h` - shared effect-detail dropdown widget, label stack, sizing, and SysEx write-back.
- `src/customComboBox.cpp` / `src/customComboBox.h` - popup sizing behavior for the shared dropdown.

## Related docs

- Visual layout rules that govern dropdown placement: `docs/ui/sy1000_boss_layout_notes.md`
- Label typography shared by all stomp/effect controls: `docs/ui/setting_label_inventory.md`

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

## Finding per-line dropdown detail

To inspect which direction variant is used for a specific control in any file:

```bash
grep -n 'addComboBox' src/FILENAME.cpp
```

The second string argument to `addComboBox(...)` is the direction variant (e.g. `"large"`, `"default"`, `"icon_seq"`).
