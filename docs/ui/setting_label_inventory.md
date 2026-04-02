# Stomp/Effect Setting Label Inventory

This note identifies the shared code paths that render the small setting labels under stomp/effect controls.

## Related docs

- Complete dropdown inventory and direction variant usage: `docs/ui/dropdown_inventory.md`
- Visual layout rules (spacing, sizing, color) that affect label appearance: `docs/ui/sy1000_boss_layout_notes.md`

## Shared typography source of truth

- `src/customControlLabel.cpp`
  - Font roles are centralized here.
  - The `setting` role is the shared size for stomp/effect setting labels.
  - Increase this role first before touching individual pages.

## Control families using the shared `setting` label role

- `src/customControlKnob.cpp`
  - Standard stomp/effect knob labels.
- `src/customControlDataKnob.cpp`
  - Data knob labels.
- `src/customControlListMenu.cpp`
  - Stomp/effect dropdown labels.
- `src/customControlSwitch.cpp`
  - On/off and mode switch labels inside detail pages.
- `src/customControlRange.cpp`
  - Range min/max labels.
- `src/customControlTarget.cpp`
  - Target/min/max labels.

## Direct label-heavy widgets that still inherit `customControlLabel`

- `src/customControlGraphicEQ.cpp`
- `src/customControlMasterEQ.cpp`
- `src/customControlParaEQ.cpp`

These widgets still build their labels with `customControlLabel`, so the shared font-role changes will flow into them unless they override the label font directly.
