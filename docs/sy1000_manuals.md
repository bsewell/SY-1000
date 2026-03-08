# SY-1000 Manuals (Official BOSS)

This project now includes the official SY-1000 documentation PDFs and extracted text snapshots for quick reference.

## Source (official)

- Product support page: https://www.boss.info/us/products/sy-1000/support/
- Owner's Manual page: https://www.boss.info/us/support/by_product/sy-1000/owners_manuals/ad88dc22-cae9-4e2a-a294-c84bbff79755/
- Reference Manual page: https://www.boss.info/us/support/by_product/sy-1000/owners_manuals/fde3ebe8-ffe1-4537-918f-d0479f428e4d/
- BOSS Tone Studio manual page: https://www.boss.info/us/support/by_product/sy-1000/owners_manuals/0c311c5f-0101-478d-98f6-d3afe92d1aeb/
- Sound List page: https://www.boss.info/us/support/by_product/sy-1000/owners_manuals/2aaef246-f00e-4701-98a5-9691728dd51d/
- MIDI Implementation page: https://www.boss.info/us/support/by_product/sy-1000/owners_manuals/cbaa51f9-efdd-4604-a202-7eed46ab3526/

## Local files (downloaded)

### PDFs (`docs/manuals`)

- `docs/manuals/SY-1000_eng01_W.pdf` (Owner's Manual, 22 pages)
- `docs/manuals/SY-1000_reference_eng02_W.pdf` (Reference Manual, 99 pages)
- `docs/manuals/BTS_SY_1000_eng02_W.pdf` (BOSS Tone Studio manual, 14 pages)
- `docs/manuals/SY-1000_soundlist_eng01_W.pdf` (Sound List, 17 pages)
- `docs/manuals/SY-1000_MIDI_Imple_eng02_W.pdf` (MIDI Implementation, 97 pages, v1.01 dated 2024-12-24)

### Extracted text (`docs/manuals/text`)

- `docs/manuals/text/SY-1000_eng01_W.txt`
- `docs/manuals/text/SY-1000_reference_eng02_W.txt`
- `docs/manuals/text/BTS_SY_1000_eng02_W.txt`
- `docs/manuals/text/SY-1000_soundlist_eng01_W.txt`
- `docs/manuals/text/SY-1000_MIDI_Imple_eng02_W.txt`

## What was reviewed (high-level)

### Owner's Manual

- Basic setup and startup flow: cabling, power, auto-off.
- Core UI operation: play screen, patch switching, editing INST/effects, saving patches.
- Required first-use setup points for this app's users:
  - `MODE SETTING` (Guitar/Bass)
  - `OUTPUT SELECT`
  - `GK SETTING` (pickup type, scale length, sensitivity, bridge distance)
  - Tuner settings

### Reference Manual

- Full parameter coverage for INST blocks and effect blocks.
- Internal structure and routing model (INST + EFFECT block architecture).
- Parameter-level definitions useful for validating editor field names and ranges.

### BOSS Tone Studio manual

- USB driver prerequisite and connection/startup sequence.
- Offline liveset behavior vs connected behavior.
- Helpful for support docs and troubleshooting USB/driver reports.

### Sound List

- Factory patch names and INST compositions in Guitar mode.
- Useful for validating default patch expectations and naming behavior.

### MIDI Implementation

- Confirms Roland SysEx model ID for SY-1000 (`00 00 00 69`).
- Documents RQ1 (`11H`) and DT1 (`12H`) packet formats and checksum usage.
- Confirms channel/control/program behaviors and thru behavior conditions.
- This should be treated as the authoritative external reference for MIDI message interpretation.

## Notes for FloorBoard development

- Prefer the MIDI Implementation PDF as source-of-truth when adjusting SysEx parsing/writing logic.
- Use the Reference Manual to verify parameter semantics before changing labels/ranges in editor controls.
- Keep this document updated if BOSS publishes revised manual versions.
- For owner-manual workflow coverage, see `docs/sy1000_owner_manual_ui_map.md`.
- For reference-manual page mapping, see `docs/sy1000_reference_manual_ui_map.md`.
- For MIDI-implementation page mapping, see `docs/sy1000_midi_implementation_ui_map.md`.
- For screenshot-to-code parity checks (Claude handoff), see `docs/ui/sy1000_claude_compare_matrix.md`.

## Added on

- Added on: 2026-03-07 (local timezone)
