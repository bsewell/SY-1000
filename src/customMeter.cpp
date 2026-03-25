/****************************************************************************
**
** Copyright (C) 2007~2025 Colin Willcocks. 
** All rights reserved.
** This file is part of "SY-1000 FloorBoard".
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

#include <QtWidgets>
#include <QLinearGradient>
#include "customMeter.h"
#include "Preferences.h"

#include <iostream>
#include <cstdlib>

customMeter::customMeter(double value, QPoint dialPos, QWidget *parent, QString type, QString imagePath, uint imageRange)
                           : QWidget(parent)
{
    Preferences *preferences = Preferences::Instance();
    bool ok;
    const double ratio = preferences->getPreferences("Window", "Scale", "ratio").toDouble(&ok);
    _x = 1; _y = 1;
    if(type=="6_band"){ _x = 1; _y = 1; };
    if(type=="54_band"){ _x = 0.4; _y = 0.8; };
    if(type=="poly_tuner"){_x = 2.0; _y = 1;};
    if(type=="mono_tuner"){_x = 2.5; _y = 3;};
    this->imageRange = imageRange;
    this->imagePath = imagePath;
    this->type = type;

    if(this->type == "mono_tuner")
    {
        strobeTimer = new QTimer(this);
        strobeTimer->setInterval(16); // ~60 FPS
        connect(strobeTimer, &QTimer::timeout, this, [this]() {
            const int period = 22; // stripeW + gap in paintEvent
            strobePhase = (strobePhase + strobeSpeed) % period;
            if(strobePhase < 0) { strobePhase += period; }
            this->update();
        });
    }

    QSize imageSize = QPixmap(imagePath).size();
    this->dialSize = QSize(imageSize.width(), imageSize.height());
    setOffset(value);
    setGeometry(dialPos.x(), dialPos.y(), dialSize.width()*ratio*_x, dialSize.height()*ratio*_y);
}

void customMeter::paintEvent(QPaintEvent *)
{
    Preferences *preferences = Preferences::Instance();
    bool ok;
    const double ratio = preferences->getPreferences("Window", "Scale", "ratio").toDouble(&ok);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    QRectF target(0.0 , 0.0, dialSize.width()*ratio*_x, dialSize.height()*ratio*_y);

    if(type=="mono_tuner")
    {
        // Pedersen-style strobe: moving vertical bands driven by detune (xOffset).
        painter.fillRect(target, QColor(8,10,12));
        const int stripeW = 12;
        const int gap = 10;
        const int period = stripeW + gap;
        const int phase = (strobeTimer ? strobePhase : static_cast<int>(xOffset));
        const int wrappedPhase = (phase % period + period) % period;
        painter.save();
        painter.translate(-wrappedPhase, 0);
        for(int x = -period; x < target.width() + period; x += period)
        {
            QRectF stripe(x, 0, stripeW, target.height());
            QLinearGradient g(stripe.topLeft(), stripe.topRight());
            g.setColorAt(0.0, QColor(30, 180, 255, 140));
            g.setColorAt(0.5, QColor(80, 220, 255, 200));
            g.setColorAt(1.0, QColor(30, 180, 255, 140));
            painter.fillRect(stripe, g);
        }
        painter.restore();
        // Center reference line
        painter.setPen(QPen(QColor(255,255,255,200), 2));
        painter.drawLine(QPointF(target.width()/2.0, 0), QPointF(target.width()/2.0, target.height()));
        painter.setPen(Qt::NoPen);
    }
    else if(type=="6_band" || type=="54_band"){
        QRectF source(0.0, xOffset, dialSize.width(), dialSize.height());
        QPixmap image(imagePath);
        painter.drawPixmap(target, image, source);
    }else{
        QRectF source((xOffset*7)+0, 0.0, dialSize.width(), dialSize.height());
        QPixmap image(imagePath);
        painter.drawPixmap(target, image, source);
    }



}

void customMeter::setOffset(int _newValue)
{
    if(type=="mono_tuner")
    {
        // Centered at 50; detune drives strobe scroll speed/direction.
        this->xOffset = (_newValue - 50) * 4;
        strobeSpeed = this->xOffset;
        if(strobeTimer)
        {
            if(strobeSpeed != 0)
            {
                if(!strobeTimer->isActive()) { strobeTimer->start(); }
            }
            else
            {
                strobeTimer->stop();
            }
        }
    }
    else
    {
        this->xOffset = (this->dialSize.height()*_newValue)/100;
    }
    this->update();
}
void customMeter::setValue(int value)
{
    setOffset(value);
}
