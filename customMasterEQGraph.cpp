/****************************************************************************
**
** Copyright (C) 2007~2013 Colin Willcocks.
** Copyright (C) 2005~2007 Uco Mesdag.
** All rights reserved.
** This file is part of "SY-1000B FloorBoard".
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

#include "customMasterEQGraph.h"
#include <QPainter>
#include <QMouseEvent>
#include <QGraphicsPathItem>
#include <QVector>

namespace {

QRectF eqPlotRect(const QWidget *widget)
{
    const qreal margin = 12.0;
    return QRectF(margin, margin,
                  qMax(1.0, widget->width() - (margin * 2.0)),
                  qMax(1.0, widget->height() - (margin * 2.0)));
}

void drawEqSurface(QPainter &painter, const QRectF &plot)
{
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing, true);

    QLinearGradient bg(plot.topLeft(), plot.bottomLeft());
    bg.setColorAt(0.0, QColor(21, 26, 34));
    bg.setColorAt(1.0, QColor(9, 13, 18));
    painter.setPen(QPen(QColor(110, 126, 146, 120), 1.0));
    painter.setBrush(bg);
    painter.drawRoundedRect(plot, 10.0, 10.0);

    painter.setClipRect(plot.adjusted(1.0, 1.0, -1.0, -1.0));
    const QVector<qreal> freqStops = {0.08, 0.18, 0.32, 0.50, 0.68, 0.82, 0.92};
    painter.setPen(QPen(QColor(185, 197, 214, 24), 1.0));
    for(qreal stop : freqStops)
    {
        const qreal x = plot.left() + (plot.width() * stop);
        painter.drawLine(QPointF(x, plot.top()), QPointF(x, plot.bottom()));
    }

    for(int i = 0; i <= 4; ++i)
    {
        const qreal y = plot.top() + ((plot.height() / 4.0) * i);
        const QColor lineColor = (i == 2) ? QColor(255, 176, 92, 84)
                                          : QColor(185, 197, 214, 20);
        painter.setPen(QPen(lineColor, i == 2 ? 1.2 : 1.0));
        painter.drawLine(QPointF(plot.left(), y), QPointF(plot.right(), y));
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
    fillColor.setAlpha(38);
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

qreal gainYOffset(unsigned short value, const QRectF &plot)
{
    return (static_cast<qreal>(value) - 12.0) * (plot.height() / 30.0);
}

}

customMasterEQGraph::customMasterEQGraph (QWidget *parent)
{
	QFrame::setFrameShape(QFrame::NoFrame);
        QFrame::setFrameShadow(QFrame::Plain);

}

customMasterEQGraph::~customMasterEQGraph (void)
{
}

void customMasterEQGraph::setLowGain ( unsigned short iLowGain )
{
     if (iLowGain > 24) iLowGain = 24;
        if (m_iLowGain != iLowGain) {
                m_iLowGain  = iLowGain;
                update();
                emit LowGainChanged(LowGain());
        };
}

unsigned short customMasterEQGraph::LowGain (void) const
{
        return m_iLowGain;
}


void customMasterEQGraph::setMidFreq ( unsigned short iMidFreq )
{
     if (iMidFreq > 27) iMidFreq = 27;
        if (m_iMidFreq != iMidFreq) {
                m_iMidFreq  = iMidFreq;
                update();
                emit MidFreqChanged(MidFreq());
        };
}

unsigned short customMasterEQGraph::MidFreq (void) const
{
        return m_iMidFreq;
}


void customMasterEQGraph::setMidQ ( unsigned short iMidQ )
{
    if (iMidQ > 5) iMidQ = 5;
        if (m_iMidQ != iMidQ) {
                m_iMidQ  = iMidQ;
                update();
                emit MidQChanged(MidQ());
        }
}

unsigned short customMasterEQGraph::MidQ (void) const
{
        return m_iMidQ;
}


void customMasterEQGraph::setMidGain ( unsigned short iMidGain )
{
    if (iMidGain > 24) iMidGain = 24;
        if (m_iMidGain != iMidGain) {
                m_iMidGain  = iMidGain;
                update();
                emit MidGainChanged(MidGain());
        }
}

unsigned short customMasterEQGraph::MidGain (void) const
{
        return m_iMidGain;
}

void customMasterEQGraph::setHighGain ( unsigned short iHighGain )
{
    if (iHighGain > 24) iHighGain = 24;
        if (m_iHighGain != iHighGain) {
                m_iHighGain  = iHighGain;
                update();
                emit HighGainChanged(HighGain());
        };
}

unsigned short customMasterEQGraph::HighGain (void) const
{
        return m_iHighGain;
}

void customMasterEQGraph::setLevel ( unsigned short iLevel )
{
    if (iLevel > 100) iLevel = 100;
        if (m_iLevel != iLevel) {
                m_iLevel  = iLevel;
                update();
                emit LevelChanged(Level());
        };
}

unsigned short customMasterEQGraph::Level (void) const
{
        return m_iLevel;
}


void customMasterEQGraph::paintEvent ( QPaintEvent *pPaintEvent )
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

    const QRectF plot = eqPlotRect(this);
    drawEqSurface(painter, plot);

    const qreal levelShift = ((static_cast<qreal>(m_iLevel) / 100.0) - 0.5) * (plot.height() * 0.24);
    const qreal zeroY = plot.center().y() - levelShift;
    const qreal lowX = plot.left() + (plot.width() * 0.20);
    const qreal highX = plot.left() + (plot.width() * 0.80);
    const qreal midCenterX = plot.left() + plot.width() * (0.30 + (qBound(0.0, static_cast<qreal>(m_iMidFreq) / 27.0, 1.0) * 0.40));
    const qreal midHalfWidth = plot.width() * (0.16 - (qBound(0.0, static_cast<qreal>(m_iMidQ) / 5.0, 1.0) * 0.10));

    poly.clear();
    poly << QPoint(qRound(plot.left()), qRound(plot.bottom()))
         << QPoint(qRound(plot.left()), qRound(zeroY))
         << QPoint(qRound(lowX), qRound(zeroY - gainYOffset(m_iLowGain, plot)))
         << QPoint(qRound(plot.left() + (plot.width() * 0.34)), qRound(zeroY))
         << QPoint(qRound(midCenterX - midHalfWidth), qRound(zeroY))
         << QPoint(qRound(midCenterX), qRound(zeroY - gainYOffset(m_iMidGain, plot)))
         << QPoint(qRound(midCenterX + midHalfWidth), qRound(zeroY))
         << QPoint(qRound(plot.left() + (plot.width() * 0.66)), qRound(zeroY))
         << QPoint(qRound(highX), qRound(zeroY - gainYOffset(m_iHighGain, plot)))
         << QPoint(qRound(plot.right()), qRound(zeroY))
         << QPoint(qRound(plot.right()), qRound(plot.bottom()));

    QPainterPath curve;
    curve.moveTo(QPointF(plot.left(), zeroY));
    curve.cubicTo(QPointF(plot.left() + (plot.width() * 0.08), zeroY),
                  QPointF(lowX - (plot.width() * 0.06), poly.at(2).y()),
                  poly.at(2));
    curve.cubicTo(QPointF(lowX + (plot.width() * 0.08), poly.at(2).y()),
                  QPointF(poly.at(4).x() - (plot.width() * 0.03), zeroY),
                  poly.at(4));
    curve.cubicTo(poly.at(4), poly.at(5), poly.at(6));
    curve.cubicTo(QPointF(poly.at(6).x() + (plot.width() * 0.03), zeroY),
                  QPointF(highX - (plot.width() * 0.08), poly.at(8).y()),
                  poly.at(8));
    curve.cubicTo(QPointF(highX + (plot.width() * 0.06), poly.at(8).y()),
                  QPointF(plot.right() - (plot.width() * 0.08), zeroY),
                  QPointF(plot.right(), zeroY));

    drawEqResponse(painter, plot, curve, QColor(255, 176, 92));

    drawEqNode(painter, poly.at(2), QColor(255, 176, 92));
    drawEqNode(painter, poly.at(4), QColor(255, 176, 92));
    drawEqNode(painter, poly.at(5), QColor(255, 176, 92));
    drawEqNode(painter, poly.at(6), QColor(255, 176, 92));
    drawEqNode(painter, poly.at(8), QColor(255, 176, 92));

    Q_UNUSED(pPaintEvent);
}


// Mouse interaction.
void customMasterEQGraph::mousePressEvent ( QMouseEvent *pMouseEvent )
{
    if (pMouseEvent->button() == Qt::LeftButton) {
            const QPoint& pos = pMouseEvent->pos();
            int iDragNode = nodeIndex(pos);
            if (iDragNode >= 0) {
                    setCursor(iDragNode == 1 || iDragNode == 3 || iDragNode == 6 || iDragNode == 8
                            ? Qt::SizeAllCursor
                            : Qt::SizeHorCursor);
                    m_iDragNode = iDragNode;
                    m_posDrag = pos;
            }
    }

    QFrame::mousePressEvent(pMouseEvent);

}


void customMasterEQGraph::mouseMoveEvent ( QMouseEvent *pMouseEvent )
{

    dragNode(pMouseEvent->pos());
}


void customMasterEQGraph::mouseReleaseEvent ( QMouseEvent *pMouseEvent )
{
    QFrame::mouseReleaseEvent(pMouseEvent);

        dragNode(pMouseEvent->pos());

        if (m_iDragNode >= 0) {
                m_iDragNode = -1;
                unsetCursor();
        }
}



// Draw rectangular point.
QRect customMasterEQGraph::nodeRect ( int iNode ) const
{
        const QPoint& pos = poly.at(iNode);
        return QRect(pos.x() - 7, pos.y() - 7, 14, 14);
}


int customMasterEQGraph::nodeIndex ( const QPoint& pos ) const
{     
        if (nodeRect(8).contains(pos))
                return 5; // HighGain

        if (nodeRect(6).contains(pos))
                return 4; // MidQ

        if (nodeRect(5).contains(pos))
                return 3; // MidFreq/MidGain

        if (nodeRect(4).contains(pos))
                return 2; // MidQ

        if (nodeRect(2).contains(pos))
                return 1; //LowGain

        return -1;
}

void customMasterEQGraph::dragNode ( const QPoint& pos )
{
        static unsigned short m_iLevel0 = 0; // Dummy!
        unsigned short *piRate  = NULL;
        unsigned short *piLevel = NULL;
        switch (m_iDragNode) {
        case 1: // LowGain
                piRate  = &m_iLevel0;
                piLevel = &m_iLowGain;
                break;
        case 2: // MidQ
                piRate  = &m_iMidQ;
                piLevel = &m_iLevel0;
                break;
        case 3: // MidFreq/MidGain
                piRate  = &m_iMidFreq;
                piLevel = &m_iMidGain;
                break;
        case 4: // MidQ
                piRate  = &m_iMidQ;
                piLevel = &m_iLevel0;
                break;
        case 5: // HighGain
                piRate  = &m_iHighGain;
                piLevel = &m_iLevel0;
                break;
        }

        if (piRate && piLevel) {
                int iRate = int(*piRate) + ((pos.x() - m_posDrag.x()) << 6) / (width() >> 2);
                int iLevel = int(*piLevel) + ((m_posDrag.y() - pos.y()) << 7) / height();
                if (iLevel < 0) iLevel = 0;
                else
                if (iLevel > 40) iLevel = 40;
                if (iRate < 0) iRate = 0;
                else
                if (iRate > 30) iRate = 30;
                if (*piRate  != (unsigned short) iRate ||
                        *piLevel != (unsigned short) iLevel) {
                        m_posDrag = pos;
                        switch (m_iDragNode) {
                        case 1: // LowGain
                                setLowGain(iLevel);
                                break;
                        case 2: // MidQ
                                setMidQ(iRate);
                                break;
                        case 3: // MidFreq/MidGain
                                setMidFreq(iRate);
                                setMidGain(iLevel);
                                break;
                        case 4: // MidQ
                                setMidQ(iRate);
                                break;
                        case 5: // HighGain
                                setHighGain(iLevel);
                                break;

                        }
                }
        } else if (nodeIndex(pos) >= 0) {
                setCursor(Qt::PointingHandCursor);
        } else {
                unsetCursor();
        }
}

void customMasterEQGraph::updateSlot(  QString hex_1, QString hex_2, QString hex_3,
                               QString hex_4, QString hex_5, QString hex_6 )
{
    this->hex_1 = hex_1;
    this->hex_2 = hex_2;
    this->hex_3 = hex_3;
    this->hex_4 = hex_4;
    this->hex_5 = hex_5;
    this->hex_6 = hex_6;
    bool ok;
    setLowGain(this->hex_1.toShort(&ok, 16));
    setMidFreq(this->hex_2.toShort(&ok, 16));
    setMidQ(this->hex_3.toShort(&ok, 16));
    setMidGain(this->hex_4.toShort(&ok, 16));
    setHighGain(this->hex_5.toShort(&ok, 16));
    setLevel(this->hex_6.toShort(&ok, 16));

}

// Stubbed signal bodies (moc not used)
void customMasterEQGraph::LowGainChanged(unsigned short) {}
void customMasterEQGraph::MidFreqChanged(unsigned short) {}
void customMasterEQGraph::MidQChanged(unsigned short) {}
void customMasterEQGraph::MidGainChanged(unsigned short) {}
void customMasterEQGraph::HighGainChanged(unsigned short) {}
void customMasterEQGraph::LevelChanged(unsigned short) {}



