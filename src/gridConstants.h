#ifndef GRIDCONSTANTS_H
#define GRIDCONSTANTS_H

// Grid spacing constants: (hSpacing, vSpacing) for compactCurrentStackField()
// DEFAULT: all normal knob/combo pages (COMMON, ALT TUNE, OSC, FILTER, AMP, LFO, E.Guitar, etc.)
constexpr int GRID_H = 20;
constexpr int GRID_V = 14;

// DENSE: sequencer and tightly-packed pages only
constexpr int GRID_DENSE_H = 14;
constexpr int GRID_DENSE_V = 10;

// Standard column count for uniform grids
constexpr int GRID_COLS = 6;

// Knob ratio strings
#define RATIO_LARGE   "normal_ratio1.5"
#define RATIO_MEDIUM  "normal_ratio1.25"
#define RATIO_RANGE   "0~200_ratio1.25"

// Combo direction strings
#define COMBO_INST_LEFT  "large_inst_left"
#define COMBO_LEFT_LARGE "left_large"
#define COMBO_LARGE      "large"

#endif // GRIDCONSTANTS_H
