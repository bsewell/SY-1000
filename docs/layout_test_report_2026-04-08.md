# Signal Chain Layout Test Report

**Date:** 2026-04-08
**Build:** 2026.04.08.40
**Test Method:** Diagnostic server (`layout-test` + `chain-positions`) on port 47321

## Summary

| # | Test Name | Topology (i1 i2 i3 i4 i5 i6) | Dual | Score | Result |
|---|-----------|-------------------------------|------|-------|--------|
| 1 | Empty preset | 0 0 0 0 0 0 | No | 100 | PASS |
| 2 | Balanced (1 per zone) | 1 1 1 1 1 1 | No | 100 | PASS |
| 3 | Stock default | 1 1 2 0 2 6 | No | 100 | PASS |
| 4 | Heavy all zones | 4 4 4 4 4 4 | No | 100 | PASS |
| 5 | Asymmetric front-heavy | 5 3 0 0 0 0 | No | 100 | PASS |
| 6 | Asymmetric rear-heavy | 0 0 0 0 3 8 | No | 100 | PASS |
| 7 | Dual channel balanced | 1 1 2 2 2 2 | Yes | 100 | PASS |
| 8 | Dual channel heavy | 3 3 3 3 3 3 | Yes | 100 | PASS |
| 9 | Extreme single-zone (17) | 0 0 0 0 0 17 | No | 100 | PASS |
| 10 | Mixed asymmetric dual | 2 0 3 1 0 4 | Yes | 100 | PASS |

**All 10 tests: PASS (100/100)**

## Balancer Column Positions

| # | BAL1 col | BAL2 col | BAL3 col | Violations |
|---|----------|----------|----------|------------|
| 1 | 1 | 1 | 1 | 0 |
| 2 | 2 | 4 | 6 | 0 |
| 3 | 2 | 5 | 7 | 0 |
| 4 | 5 | 10 | 15 | 0 |
| 5 | 6 | 7 | 8 | 0 |
| 6 | 1 | 1 | 9 | 0 |
| 7 | 2 | 5 | 8 | 0 |
| 8 | 4 | 8 | 12 | 0 |
| 9 | 1 | 1 | 18 | 0 |
| 10 | 3 | 7 | 8 | 0 |

## Live Chain Position Analysis (Stock Default Preset)

**Visible blocks on chain:** 29
**Layout constants:** flowStep=53px, touchGap=9px

### Row-by-Row Flow Bounds

| Row Y | Blocks | Gaps (px) |
|-------|--------|-----------|
| 55 (INST1) | Inst1 → FX1 | 9 |
| 83 (BAL1Out) | Bal1 → LP | 0 |
| 110 (INST2) | Inst2 → FX2 | 9 |
| 124 (BAL2Out) | EQ1 → FV1 → DD1 | 9, 9 |
| 125 | Bal2 (solo) | — |
| 165 (INST3) | Inst3 → FX3 | 9 |
| 172 (BAL3Out) | DD3 → RV → DIV → MIX → SubL → MainL → MST | 9, 9, 9, 9, 9, 9 |
| 173 | Bal3 (solo) | — |
| 220 (Normal) | NRM → CS → DS → AMP → EQ2 → NS → CE → FV2 → DD2 | 9, 9, 9, 9, 9, 9, 9, 9 |

### Measurements

- **Gap range:** 0px min, 9px max, 8.6px average
- **Overlaps:** 0
- **Excessive gaps (>flowStep+touchGap):** 0
- **BAL3 → DD3 gap:** 9px (standard touchGap — previously was 30px before fix)

## Validation Rules Applied

- **§L1 (MAX_BAL_SPREAD=2):** Balancer column positions must not exceed cap relative to zone content. All 10 topologies pass.
- **Overlap check:** No visible blocks share pixel space on same row.
- **Gap check:** All inter-block gaps are within standard flowStep tolerance.
- **Row alignment:** All blocks within a row share the same Y coordinate (verified via flow bounds grouping).
