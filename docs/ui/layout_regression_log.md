# UI Layout Regression Log

Each entry records a confirmed layout failure: what broke, the patch topology that triggered it,
what was done (or not done), and the score before/after. Use `layout-score` in the diagnostic
server to measure current state.

---

## Rule Index

| Rule | Description |
|------|-------------|
| §L1  | No balancer may exceed `max(direct_input_A, direct_input_B) + MAX_BAL_SPREAD` columns. Prevents excessively long empty-wire gaps on shorter input rows. `MAX_BAL_SPREAD = 2` (tunable constant in `floorBoard.cpp`). |
| §L2  | FX1 and FX2 must share the same x-column (enforced by `bal1xpos = max(index1, index2) + 1`). **Spec-only** — invariant guaranteed by the formula; no runtime violation check needed. |
| §L3  | INST1–INST3 and NORMAL must all share the same leftmost x-column (enforced by fixed `offset + 15*ratio`). **Spec-only** — requires rendered widget positions not available to the diagnostic server. |

---

## Entries

### LR-001 — BAL2 pushed far right in single-chain mode
- **Date**: 2026-03-23
- **Patch topology**: INST1→FX1→BAL1, INST2→FX2→BAL1, BAL1→FV1→DLY1→BAL2, INST3→BAL2, BAL2→EQ1→FX3→BAL3, NORMAL→CMP→DS→AMP→EQ2→NS→CHO→BAL3
- **Chain segment lengths**: index1=1, index2=1, index3=2 (FV1, DLY1), index4=0 (INST3 direct), index5=2, index6=6
- **Computed positions**: bal1xpos=2, bal2xpos=5, bal3xpos=7
- **Violation (original)**: §L1 — BAL2 at col 6, INST3 row has 6-column empty wire. Initial cap formula (`qMax(index3,index4) + MAX_BAL_SPREAD = 4`) capped bal2xpos from 5 → 4 but used a wrong Zone-B base (ignored that Zone B starts from bal1xpos, not C1).
- **Fix (2026-03-24 session 2)**: `floorBoard.cpp` cap corrected to `qMax(qMax(bal1xpos+index3, index4)+2, bal1xpos+1)` = 6. bal2xpos=5 ≤ 6 → no cap fires. Topology now passes §L1 cleanly.
- **Fix (2026-03-24 session 3)**: `diagnosticServer.cpp` `computeLayoutScore` updated to match corrected formula. Old formula was still reporting a false-positive violation for this topology.
- **Architectural note**: When BAL1 output path (index3) has items AND INST3 path (index4) has none, the gap on INST3 row equals `bal1xpos + index3 + 1`. This is a topology mismatch. True architectural fix (showing items after merge or visual wire shortcut) deferred.
- **Status**: Resolved — `layout-test 1 1 2 0 2 6 0` now returns score 100.
- **Files changed**: `src/floorBoard.cpp`, `src/diagnosticServer.cpp`, `src/diagnosticServer.h`, `src/chainLayout.h` (comment update)

---

## How to run regression tests

```bash
./scripts/ui-verify.sh          # all tests
./scripts/ui-verify.sh --fail-fast  # stop on first failure
```

Requires the app to be running. Tests call `layout-test` (headless — no patch needed).

## How to use `layout-score`

```bash
echo "layout-score" | nc localhost 47321
```

## How to test a specific topology headlessly

```bash
# layout-test i1 i2 i3 i4 i5 i6 dual_channel
echo "layout-test 1 1 2 0 2 6 0" | nc localhost 47321
```

Example response:
```json
{
  "status": "ok",
  "score": 75,
  "rule": "§L1 MAX_BAL_SPREAD=2",
  "dual_channel": 0,
  "bal1xpos": 2,
  "bal2xpos": 5,
  "bal3xpos": 7,
  "index1": 1, "index2": 1,
  "index3": 2, "index4": 0,
  "index5": 2, "index6": 6,
  "violations": [
    {
      "rule": "§L1",
      "balancer": "BAL2",
      "computed": 5,
      "cap": 4,
      "gap_inst3": 6
    }
  ],
  "message": "1 violation(s) — score 75/100"
}
```

## How to add a new entry

When a layout bug is found (visually or via `layout-score`):
1. Load the offending patch
2. Run `layout-score` and copy the JSON
3. Add an entry to this file with the topology, violation details, and resolution
4. Increment the LR number
