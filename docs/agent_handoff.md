# Agent Handoff Notes

This file is the neutral handoff document for either Codex, Claude, or another developer using AI assistance.

## What matters most

- The app is a Qt desktop editor for the Boss SY-1000.
- The repository has been normalized so source files are under `src/`.
- Documentation exists specifically to compare the current UI against Boss reference behavior and layout.

## Core docs

- Repo structure: `/Users/bsewell/010 MUSIC STUDIO /SY-1000/docs/repo_layout.md`
- UI/layout guidance: `/Users/bsewell/010 MUSIC STUDIO /SY-1000/docs/ui/sy1000_boss_layout_notes.md`
- Claude comparison matrix: `/Users/bsewell/010 MUSIC STUDIO /SY-1000/docs/ui/sy1000_claude_compare_matrix.md`
- Dropdown inventory and shared combo-control path: `/Users/bsewell/010 MUSIC STUDIO /SY-1000/docs/ui/dropdown_inventory.md`
- Shared stomp/effect setting-label typography: `/Users/bsewell/010 MUSIC STUDIO /SY-1000/src/customControlLabel.cpp`
- Setting-label inventory: `/Users/bsewell/010 MUSIC STUDIO /SY-1000/docs/ui/setting_label_inventory.md`
- Manual mapping/index: `/Users/bsewell/010 MUSIC STUDIO /SY-1000/docs/sy1000_manuals.md`

## Practical guidance

- Build from the repo root with `qmake` then `make`.
- Treat signal-chain tile logic and page-level enable/disable logic as one behavior surface.
- Treat repository organization as stable unless there is a deliberate follow-up cleanup pass.
- Use the documentation to validate terminology, control naming, and missing functionality before changing layouts.

## Current handoff task: upper signal-chain routing only

The active task is to finish stabilizing and cleaning up the upper signal chain in `/Users/bsewell/010 MUSIC STUDIO /SY-1000/src/floorBoard.cpp`.

Scope rules:

- Work only on the upper signal-chain renderer and its supporting geometry rules.
- Do not change lower instrument/effect page layouts as part of this task.
- Do not mix signal-chain routing fixes with unrelated UI cleanup.

### User-visible problem

The current installed app is better than the earlier regressions, but the signal chain still has logical connector issues:

- some balancer connectors still look like they terminate under the balancer instead of visibly entering/exiting it
- some intermediate utility blocks can appear visually detached or "floating"
- right-side spacing after `BAL3` has already been tightened, but connector anchors still need cleanup

### Current good deployed checkpoint

The last known better installed build is the revert state after removing the bad global row-snap experiment:

- commit: `5e2ed81` `Revert "Snap visible chain blocks to signal rows"`
- installed binary hash:
  - `4126f483c54dde67c516a8483fbf97a09c61be56ecf67a114c2f68d5beed300a`

That build keeps the improved spacing work from:

- `c04038f` `Tighten signal-chain segment spacing`

but does not include the later bad row-snap pass.

### Current local uncommitted work

There are local, uncommitted edits in:

- `/Users/bsewell/010 MUSIC STUDIO /SY-1000/docs/ui/sy1000_boss_layout_notes.md`
- `/Users/bsewell/010 MUSIC STUDIO /SY-1000/src/floorBoard.cpp`

These edits do two things:

1. document a proper signal-chain topology/anchor contract in the notes
2. begin switching balancer routing from hidden center points to visible left/right signal anchors

This local anchor-routing pass has not yet been validated in the installed app. Treat it as a starting point, not as proven-good state.

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

### Recommended next step

Continue the anchor-based fix in `/Users/bsewell/010 MUSIC STUDIO /SY-1000/src/floorBoard.cpp`:

- validate and finish the balancer anchor routing using `signalBounds(...)`
- prefer fixing anchor points and polygon endpoints over moving every stomp widget
- if a block still looks detached, fix the relevant anchor math for that node class rather than adding another global snap/reflow pass

### What not to do next

- Do not reintroduce a global "snap all visible blocks to row centers" pass.
- Do not let lower-page edits trigger upper-chain geometry rebuilds.
- Do not make another broad geometry guess without tying it to the documented contract above.

### Build/deploy expectation

After any further signal-chain change:

1. build from repo root
2. deploy to `/Applications/SY-1000FloorBoard.app`
3. verify the installed binary hash matches the built binary
4. report whether the installed app contains the exact new signal-chain routing behavior
