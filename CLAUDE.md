# SY-1000 Claude Handoff

Use this repository as a documented Qt desktop application, not as an ad hoc reverse-engineering exercise.

## Read first

1. `/Users/bsewell/010 MUSIC STUDIO /SY-1000/AGENTS.md`
2. `/Users/bsewell/010 MUSIC STUDIO /SY-1000/docs/repo_layout.md`
3. `/Users/bsewell/010 MUSIC STUDIO /SY-1000/docs/ui/sy1000_claude_compare_matrix.md`
4. `/Users/bsewell/010 MUSIC STUDIO /SY-1000/docs/ui/sy1000_boss_layout_notes.md`
5. `/Users/bsewell/010 MUSIC STUDIO /SY-1000/docs/ui/dropdown_inventory.md`

## Key conventions

- Source code lives under `src/`.
- Build output lives under `build/`.
- Qt project files remain at the repository root.
- Generated files should not be committed.
- UI behavior must stay synchronized between signal-chain tiles, on/off switches, and parameter enabled state.

## Build command

```bash
qmake SY-1000FloorBoard.pro
make -j$(sysctl -n hw.logicalcpu)
```

## Review posture

- Prefer matching documented SY-1000 behavior over inventing new UI behavior.
- Keep changes narrow and explainable.
- Separate layout cleanup, behavior fixes, and instrumentation changes into different commits when possible.
