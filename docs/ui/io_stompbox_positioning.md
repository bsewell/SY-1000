# I/O Stomp Box Positioning — Signal Chain Layout

## Problem

The I/O stomp box (↑↓ swap/tuner icon) sits at the **corner of a 90-degree wire turn** between BAL1 output and the vertical riser to BAL2. This makes it look disconnected and floating.

### Current (bad) layout

```
          ┌─────────────┐
  FX 1 ───┤             │
          │   BAL 1  ── I/O ──┐    ← I/O at the 90° corner
  FX 2 ───┤                   │
          └─────────────┘     │
                              │    ← vertical riser
                           BAL 2 ──
```

The I/O box is at the END of the horizontal run, right where the wire bends 90° downward. It appears to float at the junction rather than sit inline on the signal path.

### Target (Boss Tone Studio) layout

```
          ┌─────────────┐
  FX 1 ───┤             │
          │   BAL 1  ── I/O ─────────┐    ← I/O inline, wire continues
  FX 2 ───┤                          │
          └─────────────┘            │
                                  BAL 2 ──
```

In the Boss reference, the I/O box sits **inline on the horizontal BAL1 output row**. The horizontal signal line extends PAST the I/O box before the vertical riser drops down to BAL2. The I/O box is part of the horizontal flow, not at a bend point.

## Root Cause

The polygon wire from BAL1 output to BAL2 input is drawn as a single orthogonal segment (`drawOrthogonal`). This creates one 90-degree turn, and the I/O box happens to sit right at that turn point because its X position matches where the wire bends.

## Fix Required

The horizontal signal wire on the BAL1 output row must extend **past** the I/O box to a point further right, THEN the vertical riser drops to BAL2. This means:

1. The polygon endpoint for BAL1 output (polygon point 5) should be at a column PAST the last item on the BAL1 output row, not at the BAL2 column.
2. OR: draw the BAL1→BAL2 connection as two segments — horizontal past I/O, then vertical down — instead of one orthogonal segment.

## Applies to all BAL output rows

The same pattern applies to BAL2 output row (items between BAL2 and BAL3) — any items on a BAL output row should be inline on the horizontal signal, not at the corner of the riser wire.
