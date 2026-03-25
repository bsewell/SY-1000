# SY-1000 FloorBoard

Qt desktop editor for the Boss SY-1000 guitar synthesizer, with current effort focused on accurate UI mapping, cleaner layout, and reliable sync between signal-chain tiles and effect detail pages.

## Credits

This project is a fork of **SY1000FloorBoard** by **Colin Willcocks** (gumtownbassman@yahoo.com), originally published on SourceForge:

> [https://sourceforge.net/projects/sy1000floorboard/files/SY1000FloorBoard/](https://sourceforge.net/projects/sy1000floorboard/files/SY1000FloorBoard/)

Colin is the originator of the entire application: MIDI SysEx communication, patch storage, signal-chain rendering, custom widget library, and Qt project structure. This fork extends his work with UI accuracy improvements, a QML migration, and a signal-chain layout rewrite.

See [`ABOUT.md`](ABOUT.md) for a full description of the technology stack, what has changed, and why.

## Start here

Read these in order:

1. `CLAUDE.md` — agent conventions, build command, key docs list
2. `docs/repo_layout.md` — repo structure and file placement policy
3. `docs/ui/sy1000_boss_layout_notes.md` — visual patterns and Boss terminology
4. `docs/ui/sy1000_claude_compare_matrix.md` — current implementation vs Boss spec
5. `HANDOFF.md` — active migration tasks (Phase 5 QML migration)

## Docs topic map

| Topic | Doc |
|---|---|
| Agent/AI conventions & build | `CLAUDE.md` |
| Repo structure | `docs/repo_layout.md` |
| Active migration tasks | `HANDOFF.md` |
| Visual patterns & Boss layout rules | `docs/ui/sy1000_boss_layout_notes.md` |
| Impl vs Boss spec (block-by-block) | `docs/ui/sy1000_claude_compare_matrix.md` |
| Dropdown typography & shared combo path | `docs/ui/dropdown_inventory.md` |
| Setting label typography | `docs/ui/setting_label_inventory.md` |
| Design tokens (Boss CSS extraction) | `docs/ui/ux_improvement_report.md` |
| Layout regression tracking | `docs/ui/layout_regression_log.md` |
| Signal-chain architecture & known bugs | `docs/agent_handoff.md` |
| Panel parameter audit (block-level) | `docs/panel_audit_report.md` |
| Page → control → MIDI address index | `docs/ui/page_control_inventory.md` |
| Official manual index | `docs/sy1000_manuals.md` |
| Parameter inventory (instrument + FX types) | `docs/sy1000_parameter_inventory.md` |
| Refactor direction (AppServices migration) | `docs/refactor_audit.md` |
| Diagnostics & log triage | `docs/diagnostics.md` |

## Repository shape

- `src/` - application source code
- `docs/` - manual mapping, UI notes, handoff docs
- `images/` - app assets
- `qss/` - theme stylesheets
- `build/` - generated output and archived local build artifacts

Qt project files remain at the repo root:

- `SY-1000FloorBoard.pro`
- `SY-1000FloorBoard.pri`
- `SY-1000FloorBoard.qrc`

## Build

From the repository root:

```bash
qmake SY-1000FloorBoard.pro
make -j$(sysctl -n hw.logicalcpu)
```

Built app bundle: `build/packager/SY-1000FloorBoard.app`

Deploy helper: `deploy.sh`

## Vibe-coding workflow

This project benefits from fast iteration, but only if the iteration is disciplined.

### 1. Pick one surface at a time

Do not mix these in one pass unless the change truly requires it:

- signal-chain layout
- effect-page layout
- power-toggle behavior
- instrumentation or debug capture
- repo-structure cleanup

If you mix them, review gets noisy and regressions get harder to localize.

### 2. Use the docs before changing UI

Before changing labels, spacing, tabs, or controls, compare against:

- `docs/ui/sy1000_boss_layout_notes.md`
- `docs/ui/sy1000_claude_compare_matrix.md`
- `docs/ui/dropdown_inventory.md`
- `docs/sy1000_manuals.md`

The target is not generic cleanup. The target is closer SY-1000 behavior and clearer presentation.

### 3. Treat state sync as non-negotiable

When a block can turn on or off, these must stay aligned:

- signal-chain rectangle color/state
- on/off switch on the detail page
- enabled or disabled parameter controls

If one changes without the others, the fix is incomplete.

### 4. Keep layout rules consistent

When editing effect pages:

- the on/off control should anchor hard left
- the first row should read as a single coherent strip
- spacing under the header should not drift by effect type
- rows should look intentionally aligned, not individually improvised

### 5. Build after each meaningful pass

Run the build command from the `## Build` section above.

Do not stack several speculative edits without rebuilding.

### 6. Commit by intent

Good commit boundaries:

- `signal-chain toggle sync`
- `effect-page left rail alignment`
- `repo layout cleanup`
- `documentation updates`

Bad commit boundaries:

- mixed UI, docs, instrumentation, and deploy changes in one commit

## Working expectations for another developer or agent

- Prefer narrow, explainable changes.
- Keep source under `src/`.
- Do not commit generated output from `build/`, `Makefile`, `.qmake.stash`, or `qrc_*.cpp`.
- If behavior is unclear, inspect the SY-1000 reference docs before redesigning it.
- If the task is handoff-oriented, update the relevant docs as part of the same pass.

## Current handoff goal

Prepare the project so another developer can:

- build it without guessing the structure
- understand where the UI guidance lives
- compare current implementation against Boss references
- work on one surface at a time without tripping over unrelated changes
