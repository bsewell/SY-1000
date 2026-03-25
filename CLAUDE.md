# SY-1000 Claude Handoff

Use this repository as a documented Qt desktop application, not as an ad hoc reverse-engineering exercise.

## Read first

Read these docs before making changes:

1. `docs/repo_layout.md` — repo structure and file placement policy
2. `docs/ui/sy1000_boss_layout_notes.md` — visual patterns and Boss terminology
3. `docs/ui/sy1000_claude_compare_matrix.md` — current implementation vs Boss spec
4. `docs/ui/dropdown_inventory.md` — dropdown typography and shared combo path
5. `docs/sy1000_manuals.md` — official Boss manual index and what each covers

For project overview, vibe-coding workflow, and working discipline: see `README.md`.
For active QML migration tasks: see `HANDOFF.md`.

## Key conventions

- Source code lives under `src/`.
- Build output lives under `build/`.
- Qt project files remain at the repository root.
- Generated files should not be committed.
- UI behavior must stay synchronized between signal-chain tiles, on/off switches, and parameter enabled state.
- Keep functional changes separate from repo-structure changes.
- Keep signal-chain layout work separate from lower-level instrumentation or debug work.
- When changing effect-page layouts, preserve a consistent left-edge on/off rail and consistent row alignment.

## Build command

**ALWAYS use `./build.sh` — never call `qmake` or `make` directly.**

```bash
./build.sh
```

`build.sh` increments today's date + build number in `SY-1000FloorBoard.pro`, touches `preferences.xml.dist` to bust the rcc cache, then compiles. Calling `qmake`/`make` directly skips the version bump and produces a binary with a stale version number. This has caused repeated failures across multiple sessions — do not bypass it.

App bundle: `build/packager/SY-1000FloorBoard.app`
Deploy: `rm -rf /Applications/SY-1000FloorBoard.app && cp -R build/packager/SY-1000FloorBoard.app /Applications/`

## Review posture

- Prefer matching documented SY-1000 behavior over inventing new UI behavior.
- Keep changes narrow and explainable.
- Separate layout cleanup, behavior fixes, and instrumentation changes into different commits when possible.
- When in doubt, compare against Boss reference docs before redesigning behavior.
