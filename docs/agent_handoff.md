# Agent Handoff Notes

This file is the neutral handoff document for either Codex, Claude, or another developer using AI assistance.

## What matters most

- The app is a Qt desktop editor for the Boss SY-1000.
- The repository has been normalized so source files are under `src/`.
- Documentation exists specifically to compare the current UI against Boss reference behavior and layout.

## Core docs

- Repo structure: `docs/repo_layout.md`
- UI/layout guidance: `docs/ui/sy1000_boss_layout_notes.md`
- Claude comparison matrix: `docs/ui/sy1000_claude_compare_matrix.md`
- Dropdown inventory and shared combo-control path: `docs/ui/dropdown_inventory.md`
- Shared stomp/effect setting-label typography: `src/customControlLabel.cpp`
- Setting-label inventory: `docs/ui/setting_label_inventory.md`
- Manual mapping/index: `docs/sy1000_manuals.md`

## Practical guidance

- Build from the repo root with `qmake` then `make`.
- Treat signal-chain tile logic and page-level enable/disable logic as one behavior surface.
- Treat repository organization as stable unless there is a deliberate follow-up cleanup pass.
- Use the documentation to validate terminology, control naming, and missing functionality before changing layouts.

## Signal-chain architecture reference

This section documents the architectural contract for the upper signal chain in `src/floorBoard.cpp`. See `docs/ui/layout_regression_log.md` for a running record of confirmed layout bugs and their resolutions.

### Known visual issues (as of last audit)

- Some balancer connectors terminate under the balancer instead of visibly entering/exiting it.
- Some intermediate utility blocks can appear visually detached or "floating."

### Signal-chain contract to follow

The signal chain must be treated as a graph-driven layout system:

- patch data -> normalized topology -> lane assignment -> column assignment -> node geometry -> connector routing -> paint

Do not infer topology from widget positions.

Structural invariants:

- `BAL1` merges `INST1` and `INST2`
- `BAL2` merges `BAL1` output and `INST3`
- `BAL3` merges `BAL2` output and `NORMAL`
- therefore `BAL1.x < BAL2.x < BAL3.x` must never invert

Connector invariants:

- every visible non-source node must show an incoming connector
- every visible non-terminal node must show an outgoing connector
- connector lines must terminate on visible signal anchors, not hidden centers

### Known remaining work

In `src/floorBoard.cpp`:

- validate and finish the balancer anchor routing using `signalBounds(...)`
- prefer fixing anchor points and polygon endpoints over moving every stomp widget
- if a block still looks detached, fix the relevant anchor math for that node class rather than adding another global snap/reflow pass

### What not to do

- Do not reintroduce a global "snap all visible blocks to row centers" pass.
- Do not let lower-page edits trigger upper-chain geometry rebuilds.
- Do not make another broad geometry guess without tying it to the documented contract above.

### Build/deploy expectation

After any further signal-chain change:

1. build from repo root
2. deploy to `/Applications/SY-1000FloorBoard.app`
3. verify the installed binary hash matches the built binary
4. report whether the installed app contains the exact new signal-chain routing behavior
