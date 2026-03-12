# SY-1000 Agent Handoff

This repository is set up so a coding agent can review, build, and modify the app without first reverse-engineering the project layout.

## Start here

Read these files first:

1. `/Users/bsewell/010 MUSIC STUDIO /SY-1000/docs/repo_layout.md`
2. `/Users/bsewell/010 MUSIC STUDIO /SY-1000/docs/ui/sy1000_boss_layout_notes.md`
3. `/Users/bsewell/010 MUSIC STUDIO /SY-1000/docs/ui/sy1000_claude_compare_matrix.md`
4. `/Users/bsewell/010 MUSIC STUDIO /SY-1000/docs/ui/dropdown_inventory.md`
4. `/Users/bsewell/010 MUSIC STUDIO /SY-1000/docs/sy1000_manuals.md`

## Repository layout

- `src/` contains real application source.
- `docs/` contains handoff notes, UI comparisons, and manual mapping.
- `images/` contains image assets.
- `qss/` contains themes.
- `build/` contains generated output and local archived artifacts.

Do not move files out of `src/` unless the task is specifically a repo-structure refactor.

## Build

From the repo root:

```bash
qmake SY-1000FloorBoard.pro
make -j$(sysctl -n hw.logicalcpu)
```

App bundle output:

`/Users/bsewell/010 MUSIC STUDIO /SY-1000/build/packager/SY-1000FloorBoard.app`

Deploy script:

`/Users/bsewell/010 MUSIC STUDIO /SY-1000/deploy.sh`

## Working rules

- Keep functional changes separate from repo-structure changes.
- Keep signal-chain layout work separate from lower-level instrumentation/debug work.
- Do not commit generated files from `build/`, `Makefile`, `.qmake.stash`, or `qrc_*.cpp`.
- When changing UI behavior, verify both the signal-chain tile state and the corresponding page controls stay in sync.
- When changing effect-page layouts, preserve a consistent left-edge on/off rail and consistent row alignment.

## Handoff focus

The current project focus is accurate SY-1000 UI/function mapping and improved consistency of the floorboard editor UI. When in doubt, compare implementation against the documentation and Boss reference screenshots before redesigning behavior.
