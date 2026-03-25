/****************************************************************************
**
** Copyright (C) 2007~2025 Colin Willcocks.
** Copyright (C) 2005~2007 Uco Mesdag.
** All rights reserved.
** This file is part of "GT-100B Fx FloorBoard".
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License along
** with this program; if not, write to the Free Software Foundation, Inc.,
** 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA. 
**
****************************************************************************/

#include "customParaEQGraph.h"
#include <QPainter>
#include <QMouseEvent>
#include <QGraphicsPathItem>
#include <QLineF>
#include <QtMath>

namespace {

QRectF eqPlotRect(const QWidget *widget)
{
    // Leave extra room on the left for gain labels (-12 .. +12 dB).
    const qreal marginLeft = 44.0;
    const qreal margin = 12.0;
    return QRectF(marginLeft,
                  margin,
                  qMax(1.0, widget->width() - (marginLeft + margin)),
                  qMax(1.0, widget->height() - (margin * 2.0)));
}

void drawEqSurface(QPainter &painter, const QRectF &plot)
{
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing, true);

    QLinearGradient bg(plot.topLeft(), plot.bottomLeft());
    bg.setColorAt(0.0, QColor(20, 25, 32));
    bg.setColorAt(1.0, QColor(9, 13, 18));
    painter.setPen(QPen(QColor(110, 126, 146, 120), 1.0));
    painter.setBrush(bg);
    painter.drawRoundedRect(plot, 10.0, 10.0);

    painter.setClipRect(plot.adjusted(1.0, 1.0, -1.0, -1.0));
    const QVector<qreal> freqStops = {0.04, 0.09, 0.16, 0.25, 0.36, 0.50, 0.64, 0.76, 0.86, 0.93, 0.98};
    painter.setPen(QPen(QColor(185, 197, 214, 26), 1.0));
    for(qreal stop : freqStops)
    {
        const qreal x = plot.left() + (plot.width() * stop);
        painter.drawLine(QPointF(x, plot.top()), QPointF(x, plot.bottom()));
    }

    for(int i = 0; i <= 4; ++i)
    {
        const qreal y = plot.top() + ((plot.height() / 4.0) * i);
        const QColor lineColor = (i == 2) ? QColor(94, 206, 255, 84)
                                          : QColor(185, 197, 214, 20);
        painter.setPen(QPen(lineColor, i == 2 ? 1.2 : 1.0));
        painter.drawLine(QPointF(plot.left(), y), QPointF(plot.right(), y));
    }

    painter.restore();
}

void drawGainLabels(QPainter &painter, const QRectF &plot)
{
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing, false);
    QFont f = painter.font();
    f.setPointSizeF(qMax(7.5, f.pointSizeF() - 1.0));
    painter.setFont(f);
    painter.setPen(QColor(185, 197, 214, 150));

    const QList<int> gains = {-20, -10, 0, 10, 20};
    for(int g : gains)
    {
        const qreal y = plot.center().y() - (static_cast<qreal>(g) * (plot.height() / 44.0));
        painter.drawText(QRectF(0.0, y - 7.0, plot.left() - 6.0, 14.0),
                         Qt::AlignRight | Qt::AlignVCenter,
                         QString::number(g));
    }

    painter.restore();
}

void drawEqResponse(QPainter &painter, const QRectF &plot, const QPainterPath &curve, const QColor &accent)
{
    QPainterPath fill(curve);
    fill.lineTo(plot.right(), plot.bottom());
    fill.lineTo(plot.left(), plot.bottom());
    fill.closeSubpath();

    QColor fillColor = accent;
    fillColor.setAlpha(36);
    painter.fillPath(fill, fillColor);

    painter.strokePath(curve, QPen(QColor(0, 0, 0, 84), 4.5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.strokePath(curve, QPen(accent, 2.2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
}

void drawEqNode(QPainter &painter, const QPointF &point, const QColor &accent)
{
    painter.setPen(QPen(QColor(245, 248, 252, 210), 1.2));
    painter.setBrush(QColor(17, 24, 31, 245));
    painter.drawEllipse(point, 5.0, 5.0);
    painter.setPen(Qt::NoPen);
    painter.setBrush(accent);
    painter.drawEllipse(point, 2.2, 2.2);
}

qreal smoothStep(qreal edge0, qreal edge1, qreal x)
{
    if(qFuzzyCompare(edge0, edge1))
    {
        return x < edge0 ? 0.0 : 1.0;
    }
    const qreal t = qBound(0.0, (x - edge0) / (edge1 - edge0), 1.0);
    return t * t * (3.0 - (2.0 * t));
}

qreal bellShape(qreal x, qreal center, qreal width)
{
    const qreal distance = (x - center) / qMax(width, 0.001);
    return qExp(-(distance * distance));
}

qreal gainValue(unsigned short value)
{
    // SY-1000 EQ gain range: raw 12..52 maps to -20..+20 dB.
    // Raw 32 (0x20) = 0 dB center.
    return static_cast<qreal>(value) - 32.0;
}

}

customParaEQGraph::customParaEQGraph (QWidget *parent)
{
    Q_UNUSED(parent);
    QFrame::setFrameShape(QFrame::NoFrame);
    QFrame::setFrameShadow(QFrame::Plain);

    // Initialize graph state so first paint + hit-testing are stable.
    m_iLowCut = 0;
    m_iHighCut = 0;
    m_iLowGain = 32;
    m_iHighGain = 32;
    m_iLowMidGain = 32;
    m_iHighMidGain = 32;
    m_iLowMidFreq = 15;
    m_iHighMidFreq = 15;
    m_iLowMidQ = 3;
    m_iHighMidQ = 3;
    m_iLevel = 32;

    m_iDragNode = -1;
    m_lastPlot = QRectF();
    m_lastZeroY = 0.0;
    m_lastGainScale = 1.0;
    m_nodes.fill(QPointF());
}

customParaEQGraph::~customParaEQGraph (void)
{

}

void customParaEQGraph::setLowCut ( unsigned short iLowCut )
{
    if (m_iLowCut != iLowCut) {
        m_iLowCut  = iLowCut;
        update();
        emit LowCutChanged(LowCut());
    };
}

unsigned short customParaEQGraph::LowCut (void) const
{
    return m_iLowCut;
}


void customParaEQGraph::setLowGain ( unsigned short iLowGain )
{
    if (m_iLowGain != iLowGain) {
        m_iLowGain  = iLowGain;
        update();
        emit LowGainChanged(LowGain());
    };
}

unsigned short customParaEQGraph::LowGain (void) const
{
    return m_iLowGain;
}


void customParaEQGraph::setLowMidFreq ( unsigned short iLowMidFreq )
{
    if (m_iLowMidFreq != iLowMidFreq) {
        m_iLowMidFreq  = iLowMidFreq;
        update();
        emit LowMidFreqChanged(LowMidFreq());
    };
}

unsigned short customParaEQGraph::LowMidFreq (void) const
{
    return m_iLowMidFreq;
}


void customParaEQGraph::setLowMidQ ( unsigned short iLowMidQ )
{
    if (m_iLowMidQ != iLowMidQ) {
        m_iLowMidQ  = iLowMidQ;
        update();
        emit LowMidQChanged(LowMidQ());
    }
}

unsigned short customParaEQGraph::LowMidQ (void) const
{
    return m_iLowMidQ;
}


void customParaEQGraph::setLowMidGain ( unsigned short iLowMidGain )
{
    if (m_iLowMidGain != iLowMidGain) {
        m_iLowMidGain  = iLowMidGain;
        update();
        emit LowMidGainChanged(LowMidGain());
    }
}

unsigned short customParaEQGraph::LowMidGain (void) const
{
    return m_iLowMidGain;
}


void customParaEQGraph::setHighMidFreq ( unsigned short iHighMidFreq )
{
    if (m_iHighMidFreq != iHighMidFreq) {
        m_iHighMidFreq  = iHighMidFreq;
        update();
        emit HighMidFreqChanged(HighMidFreq());
    }
}

unsigned short customParaEQGraph::HighMidFreq (void) const
{
    return m_iHighMidFreq;
}

void customParaEQGraph::setHighMidQ ( unsigned short iHighMidQ )
{
    if (m_iHighMidQ != iHighMidQ) {
        m_iHighMidQ  = iHighMidQ;
        update();
        emit HighMidQChanged(HighMidQ());
    }
}

unsigned short customParaEQGraph::HighMidQ (void) const
{
    return m_iHighMidQ;
}


void customParaEQGraph::setHighMidGain ( unsigned short iHighMidGain )
{
    if (m_iHighMidGain != iHighMidGain) {
        m_iHighMidGain  = iHighMidGain;
        update();
        emit HighMidGainChanged(HighMidGain());
    }
}

unsigned short customParaEQGraph::HighMidGain (void) const
{
    return m_iHighMidGain;
}

void customParaEQGraph::setHighGain ( unsigned short iHighGain )
{
    if (m_iHighGain != iHighGain) {
        m_iHighGain  = iHighGain;
        update();
        emit HighGainChanged(HighGain());
    };
}

unsigned short customParaEQGraph::HighGain (void) const
{
    return m_iHighGain;
}

void customParaEQGraph::setHighCut ( unsigned short iHighCut )
{
    if (m_iHighCut != iHighCut) {
        m_iHighCut  = iHighCut;
        update();
        emit HighCutChanged(HighCut());
    };
}

unsigned short customParaEQGraph::HighCut (void) const
{
    return m_iHighCut;
}

void customParaEQGraph::setLevel ( unsigned short iLevel )
{
    if (m_iLevel != iLevel) {
        m_iLevel  = iLevel;
        update();
        emit LevelChanged(Level());
    };
}

unsigned short customParaEQGraph::Level (void) const
{
    return m_iLevel;
}


void customParaEQGraph::paintEvent ( QPaintEvent *pPaintEvent )
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

    m_lastPlot = eqPlotRect(this);
    drawEqSurface(painter, m_lastPlot);
    drawGainLabels(painter, m_lastPlot);

    // Anchor the 0 dB baseline to the visual center of the plot instead of drifting with Level.
    m_lastZeroY = m_lastPlot.center().y();
    m_lastGainScale = m_lastPlot.height() / 44.0;

    const qreal lowCutAmount = qBound(0.0, static_cast<qreal>(m_iLowCut) / 30.0, 1.0);
    const qreal highCutAmount = qBound(0.0, static_cast<qreal>(m_iHighCut) / 30.0, 1.0);
    const qreal lowCutX = m_lastPlot.left() + (m_lastPlot.width() * (0.02 + (lowCutAmount * 0.16)));
    const qreal highCutX = m_lastPlot.left() + (m_lastPlot.width() * (0.82 + (highCutAmount * 0.14)));
    const qreal lowMidCenter = m_lastPlot.left() + (m_lastPlot.width() * (0.20 + (qBound(0.0, static_cast<qreal>(m_iLowMidFreq) / 30.0, 1.0) * 0.18)));
    const qreal highMidCenter = m_lastPlot.left() + (m_lastPlot.width() * (0.58 + (qBound(0.0, static_cast<qreal>(m_iHighMidFreq) / 30.0, 1.0) * 0.18)));
    const qreal lowMidWidth = m_lastPlot.width() * (0.14 - (qBound(0.0, static_cast<qreal>(m_iLowMidQ) / 5.0, 1.0) * 0.08));
    const qreal highMidWidth = m_lastPlot.width() * (0.14 - (qBound(0.0, static_cast<qreal>(m_iHighMidQ) / 5.0, 1.0) * 0.08));
    const qreal lowShelfStart = m_lastPlot.left() + (m_lastPlot.width() * 0.16);
    const qreal highShelfStart = m_lastPlot.left() + (m_lastPlot.width() * 0.76);

    QPainterPath curve;
    QPointF lowShelfPoint;
    QPointF lowMidPoint;
    QPointF highMidPoint;
    QPointF highShelfPoint;

    for(int i = 0; i <= 160; ++i)
    {
        const qreal t = static_cast<qreal>(i) / 160.0;
        const qreal x = m_lastPlot.left() + (m_lastPlot.width() * t);

        qreal gain = 0.0;
        gain += gainValue(m_iLowGain) * (1.0 - smoothStep(lowShelfStart, lowShelfStart + (m_lastPlot.width() * 0.18), x));
        gain += gainValue(m_iHighGain) * smoothStep(highShelfStart - (m_lastPlot.width() * 0.18), highShelfStart, x);
        gain += gainValue(m_iLowMidGain) * bellShape(x, lowMidCenter, lowMidWidth);
        gain += gainValue(m_iHighMidGain) * bellShape(x, highMidCenter, highMidWidth);
        if(lowCutAmount > 0.0)
        {
            gain -= 18.0 * lowCutAmount * (1.0 - smoothStep(lowCutX - (m_lastPlot.width() * 0.08), lowCutX + (m_lastPlot.width() * 0.02), x));
        }
        if(highCutAmount > 0.0)
        {
            gain -= 18.0 * highCutAmount * smoothStep(highCutX - (m_lastPlot.width() * 0.02), highCutX + (m_lastPlot.width() * 0.08), x);
        }

        const qreal y = qBound(m_lastPlot.top(), m_lastZeroY - (gain * m_lastGainScale), m_lastPlot.bottom());
        if(i == 0)
        {
            curve.moveTo(x, y);
        }
        else
        {
            curve.lineTo(x, y);
        }

        if(i == 26) { lowShelfPoint = QPointF(x, y); }
        if(qAbs(x - lowMidCenter) < (m_lastPlot.width() / 160.0)) { lowMidPoint = QPointF(x, y); }
        if(qAbs(x - highMidCenter) < (m_lastPlot.width() / 160.0)) { highMidPoint = QPointF(x, y); }
        if(i == 134) { highShelfPoint = QPointF(x, y); }
    }

    drawEqResponse(painter, m_lastPlot, curve, QColor(94, 206, 255));
    m_nodes[0] = lowShelfPoint;
    m_nodes[1] = lowMidPoint;
    m_nodes[2] = highMidPoint;
    m_nodes[3] = highShelfPoint;
    drawEqNode(painter, m_nodes[0], QColor(94, 206, 255));
    drawEqNode(painter, m_nodes[1], QColor(94, 206, 255));
    drawEqNode(painter, m_nodes[2], QColor(94, 206, 255));
    drawEqNode(painter, m_nodes[3], QColor(94, 206, 255));

    Q_UNUSED(pPaintEvent);
}


// Mouse interaction.
void customParaEQGraph::mousePressEvent ( QMouseEvent *pMouseEvent )
{
    if(m_lastPlot.isNull()) { return; }
    const int hit = nodeIndex(pMouseEvent->pos());
    if(hit >= 0)
    {
        m_iDragNode = hit;
        m_posDrag = pMouseEvent->pos();
        setCursor(Qt::ClosedHandCursor);
    }
}


void customParaEQGraph::mouseMoveEvent ( QMouseEvent *pMouseEvent )
{
    if(m_iDragNode < 0) { return; }
    dragNode(pMouseEvent->pos());
}


void customParaEQGraph::mouseReleaseEvent ( QMouseEvent *pMouseEvent )
{
    Q_UNUSED(pMouseEvent);
    m_iDragNode = -1;
    unsetCursor();
}


int customParaEQGraph::nodeIndex ( const QPoint& pos ) const
{
    const qreal radius = 10.0;
    for(int i = 0; i < 4; ++i)
    {
        if(QLineF(pos, m_nodes[i]).length() <= radius)
        {
            return i;
        }
    }
    return -1;
}

void customParaEQGraph::dragNode ( const QPoint& pos )
{
    if(m_lastPlot.isNull()) { return; }

    auto gainToValue = [](qreal gainDb) -> unsigned short
    {
        // Reverse of gainValue: dB + 32 = raw value, clamped to 12..52.
        const int v = qRound(gainDb + 32.0);
        return static_cast<unsigned short>(qBound(12, v, 52));
    };

    switch(m_iDragNode)
    {
    case 0: { // low shelf gain only
        qreal gainDb = (m_lastZeroY - pos.y()) / m_lastGainScale;
        setLowGain(gainToValue(gainDb));
        break;
    }
    case 1: { // low-mid freq + gain
        qreal norm = qBound(0.0, (pos.x() - m_lastPlot.left()) / m_lastPlot.width(), 1.0);
        qreal frac = qBound(0.0, (norm - 0.20) / 0.18, 1.0);
        setLowMidFreq(static_cast<unsigned short>(qRound(frac * 30.0)));
        qreal gainDb = (m_lastZeroY - pos.y()) / m_lastGainScale;
        setLowMidGain(gainToValue(gainDb));
        break;
    }
    case 2: { // high-mid freq + gain
        qreal norm = qBound(0.0, (pos.x() - m_lastPlot.left()) / m_lastPlot.width(), 1.0);
        qreal frac = qBound(0.0, (norm - 0.58) / 0.18, 1.0);
        setHighMidFreq(static_cast<unsigned short>(qRound(frac * 30.0)));
        qreal gainDb = (m_lastZeroY - pos.y()) / m_lastGainScale;
        setHighMidGain(gainToValue(gainDb));
        break;
    }
    case 3: { // high shelf gain only
        qreal gainDb = (m_lastZeroY - pos.y()) / m_lastGainScale;
        setHighGain(gainToValue(gainDb));
        break;
    }
    default:
        break;
    }
}

void customParaEQGraph::updateSlot(  QString hex_1, QString hex_2, QString hex_3,
                                     QString hex_4, QString hex_5, QString hex_6,
                                     QString hex_7, QString hex_8, QString hex_9,
                                     QString hex_10, QString hex_11)
{
    this->hex_1 = hex_1;
    this->hex_2 = hex_2;
    this->hex_3 = hex_3;
    this->hex_4 = hex_4;
    this->hex_5 = hex_5;
    this->hex_6 = hex_6;
    this->hex_7 = hex_7;
    this->hex_8 = hex_8;
    this->hex_9 = hex_9;
    this->hex_10 = hex_10;
    this->hex_11 = hex_11;
    bool ok;
    setLowCut(this->hex_1.toUShort(&ok, 16));
    setLowGain(this->hex_2.toUShort(&ok, 16));
    setLowMidFreq(this->hex_3.toUShort(&ok, 16));
    setLowMidQ(this->hex_4.toUShort(&ok, 16));
    setLowMidGain(this->hex_5.toUShort(&ok, 16));
    setHighMidFreq(this->hex_6.toUShort(&ok, 16));
    setHighMidQ(this->hex_7.toUShort(&ok, 16));
    setHighMidGain(this->hex_8.toUShort(&ok, 16));
    setHighGain(this->hex_9.toUShort(&ok, 16));
    setHighCut(this->hex_10.toUShort(&ok, 16));
    setLevel(this->hex_11.toUShort(&ok, 16));

}
