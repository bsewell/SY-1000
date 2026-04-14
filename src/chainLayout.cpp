#include "chainLayout.h"

ChainLayout::ChainLayout(double ratio, int rowMidY[4])
    : m_ratio(ratio)
    , m_flowStep(qRound(kFlowStep * ratio))
    , m_firstFlowX(qRound(kInstStartX * ratio)
                   + qRound(kInstImageW * ratio / kFlowBlockScale)
                   + qRound(kTouchGap * ratio))
{
    // Instrument row centres = fixed base Y (lev) + measured widget mid offset.
    const int lev[4] = {
        qRound(kRowSpacing * 1.0 * ratio),   // lev1 — INST1
        qRound(kRowSpacing * 2.0 * ratio),   // lev2 — INST2
        qRound(kRowSpacing * 3.0 * ratio),   // lev3 — INST3
        qRound(kRowSpacing * 4.0 * ratio),   // lev4 — NORMAL
    };
    for (int i = 0; i < 4; ++i)
        m_rowCenter[i] = lev[i] + rowMidY[i];

    // Balancer output row centres — each is the midpoint of its two inputs.
    // This matches the rowCenterBal1/2/3 logic in updateStructure().
    const int bal1Mid = qRound((m_rowCenter[0] + m_rowCenter[1]) / 2.0);
    const int bal2Mid = qRound((bal1Mid         + m_rowCenter[2]) / 2.0);
    const int bal3Mid = qRound((bal2Mid         + m_rowCenter[3]) / 2.0);

    m_rowCenter[static_cast<int>(ChainRow::Bal1Out)] = bal1Mid;
    m_rowCenter[static_cast<int>(ChainRow::Bal2Out)] = bal2Mid;
    m_rowCenter[static_cast<int>(ChainRow::Bal3Out)] = bal3Mid;
}

int ChainLayout::colX(int colIndex) const
{
    return m_firstFlowX + colIndex * m_flowStep;
}

int ChainLayout::rowCenterY(ChainRow row) const
{
    return m_rowCenter[static_cast<int>(row)];
}
