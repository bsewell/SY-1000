#pragma once
#include <QtCore>

// ============================================================
// ChainLayout — column and row grid for the signal-chain floor
// ============================================================
//
// The signal chain has 7 logical rows:
//   4 fixed instrument rows  (Inst1..Normal)
//   3 computed balancer rows (Bal1Out..Bal3Out)
//
// Columns are indexed from 0 (first FX column, immediately right
// of the source blocks).  Column X = firstFlowX + index * flowStep.
//
// Stage 3A complete: row centres wired into update_structure() via rowCenterY().
// colX() used for BAL1/2/3 column positions.  Local lev1..lev4 / flowStep
// variables in update_structure() remain as aliases; they may be removed in a
// future pass if the duplication causes drift.

// The seven logical rows of the signal chain.
enum class ChainRow : int {
    Inst1   = 0,   // INST1 chain — fixed base Y: lev1 = 65 * ratio
    Inst2   = 1,   // INST2 chain — fixed base Y: lev2 = 130 * ratio
    Inst3   = 2,   // INST3 chain — fixed base Y: lev3 = 195 * ratio
    Normal  = 3,   // NORMAL chain — fixed base Y: lev4 = 260 * ratio
    Bal1Out = 4,   // BAL1 right-side output: midpoint(Inst1, Inst2)
    Bal2Out = 5,   // BAL2 right-side output: midpoint(Bal1Out, Inst3)
    Bal3Out = 6,   // BAL3 right-side output: midpoint(Bal2Out, Normal)
                   //   — also the output chain row (S LR, DIV, MIX, MST)
};
constexpr int kChainRowCount = 7;

// ── Spacing constants (base values at ratio=1.0) ──
constexpr double kRowSpacing   = 52.0;   // vertical distance between instrument rows (20% tighter than 65)
constexpr double kFlowStep     = 48.0;   // horizontal column pitch (Boss-tight)
constexpr double kTouchGap     =  8.0;   // gap between source block and first FX (Boss-tight)
constexpr double kInstStartX   = 15.0;   // source block left edge
constexpr double kInstImageW   = 192.0;  // source block image width (before scale)
constexpr double kFlowBlockScale = 2.4;  // image-to-widget scale factor

// ChainLayout is computed once per patch load from the UI scale ratio
// and the measured signal-line mid-Y offsets of the four instrument stomps.
//
// rowMidY[0..3] = inst1MidY, inst2MidY, inst3MidY, normalMidY
// (measured from each stomp widget's signalCenterYOffset())
struct ChainLayout {
    ChainLayout(double ratio, int rowMidY[4]);

    // X coordinate (pixels) for column colIndex (0 = first FX column).
    int colX(int colIndex) const;

    // Signal-line centre Y (pixels) for the given row.
    int rowCenterY(ChainRow row) const;

    // Raw constants — exposed for wiring in Stage 3.
    int flowStep()   const { return m_flowStep; }
    int firstFlowX() const { return m_firstFlowX; }
    double ratio()   const { return m_ratio; }

private:
    double m_ratio;
    int    m_flowStep;
    int    m_firstFlowX;
    int    m_rowCenter[kChainRowCount];
};
