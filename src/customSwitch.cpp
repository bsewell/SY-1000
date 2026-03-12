/****************************************************************************
**
** Copyright (C) 2007~2025 Colin Willcocks.
** Copyright (C) 2005~2007 Uco Mesdag.
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
#include "customSwitch.h"
#include "Preferences.h"

customSwitch::customSwitch(bool active, QWidget *parent,QString hex0, QString hex1, QString hex2, QString hex3,
                           QString imagePath)
                               : QWidget(parent)
{
    Preferences *preferences = Preferences::Instance();
    bool ok;
    const double ratio = preferences->getPreferences("Window", "Scale", "ratio").toDouble(&ok);

    if(hex0 == "INVERT") {
        this->hex0 = "10";
        this->imagePath = ":/images/switch_invert.png";
    } else {
        this->hex0 = hex0;
        this->imagePath = imagePath;
    };
    this->hex1 = hex1;
    this->hex2 = hex2;
    this->hex3 = hex3;
    this->active = active;
    this->pressed = false;
    this->accentColor = QColor(244, 82, 104);
    this->usePowerButtonStyle = this->imagePath.contains("ampswitch");
    this->useModernToggle = !this->imagePath.contains("_invert") && !this->usePowerButtonStyle;

    if(usePowerButtonStyle)
    {
        this->switchSize = QSize(34, 34);
        this->setFixedSize(QSize(qRound(34*ratio), qRound(34*ratio)));
    }
    else if(useModernToggle)
    {
        this->switchSize = QSize(28, 28);
        this->setFixedSize(QSize(qRound(28*ratio), qRound(28*ratio)));
    }
    else
    {
        QSize imageSize = QPixmap(this->imagePath).size();
        this->switchSize = QSize(imageSize.width(), imageSize.height()/3);
        this->imageRange = 1;
        this->setOffset(0);
        this->setFixedSize(switchSize*ratio);
    }

    QObject::connect(this, SIGNAL( valueChanged(bool, QString, QString, QString, QString) ),
                     this->parent(), SLOT( valueChanged(bool, QString, QString, QString, QString) ));
}

void customSwitch::setAccentColor(const QColor &color)
{
    if(color.isValid())
    {
        this->accentColor = color;
        update();
    }
}

void customSwitch::paintEvent(QPaintEvent *)
{
    Preferences *preferences = Preferences::Instance();
    bool ok;
    const double ratio = preferences->getPreferences("Window", "Scale", "ratio").toDouble(&ok);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    if(usePowerButtonStyle)
    {
        QRectF outer(1.5 * ratio, 1.5 * ratio, this->width() - 3.0 * ratio, this->height() - 3.0 * ratio);
        QColor fill = active ? this->accentColor : QColor(18, 18, 20);
        if(pressed)
        {
            fill = fill.darker(118);
        }

        QRadialGradient buttonGrad(outer.center(), outer.width() * 0.6);
        buttonGrad.setColorAt(0.0, fill.lighter(118));
        buttonGrad.setColorAt(1.0, fill.darker(120));
        painter.setPen(QPen(QColor(255, 255, 255, active ? 180 : 70), 1.2 * ratio));
        painter.setBrush(buttonGrad);
        painter.drawEllipse(outer);

        const QPointF center = outer.center();
        const qreal radius = outer.width() * 0.26;
        QRectF iconRect(center.x() - radius, center.y() - radius, radius * 2.0, radius * 2.0);
        QPen iconPen(Qt::white, qMax(1.6, 2.2 * ratio), Qt::SolidLine, Qt::RoundCap);
        painter.setPen(iconPen);
        painter.drawArc(iconRect, 130 * 16, 280 * 16);
        painter.drawLine(QPointF(center.x(), iconRect.top() - 1.5 * ratio),
                         QPointF(center.x(), center.y() - radius * 0.08));
    }
    else if(useModernToggle)
    {
        const qreal w = this->width();
        const qreal h = this->height();
        const qreal pad = 2.0 * ratio;

        // Outer housing — dark metallic rectangle like a stomp switch
        QRectF housing(pad, pad, w - pad * 2.0, h - pad * 2.0);
        QLinearGradient housingGrad(housing.topLeft(), housing.bottomLeft());
        if(pressed)
        {
            housingGrad.setColorAt(0.0, QColor(30, 32, 36));
            housingGrad.setColorAt(1.0, QColor(50, 54, 58));
        }
        else
        {
            housingGrad.setColorAt(0.0, QColor(55, 58, 64));
            housingGrad.setColorAt(1.0, QColor(38, 40, 46));
        }
        painter.setPen(QPen(QColor(20, 22, 26), 1.0));
        painter.setBrush(housingGrad);
        painter.drawRoundedRect(housing, 4.0 * ratio, 4.0 * ratio);

        // LED indicator circle in the center
        const qreal ledR = qMin(w, h) * 0.28;
        QRectF ledRect(w / 2.0 - ledR, h / 2.0 - ledR, ledR * 2.0, ledR * 2.0);

        if(active)
        {
            // Glow effect behind LED
            QRadialGradient glow(ledRect.center(), ledR * 2.2);
            glow.setColorAt(0.0, QColor(0, 220, 80, 60));
            glow.setColorAt(1.0, QColor(0, 220, 80, 0));
            painter.setPen(Qt::NoPen);
            painter.setBrush(glow);
            painter.drawEllipse(ledRect.adjusted(-ledR * 0.8, -ledR * 0.8, ledR * 0.8, ledR * 0.8));

            // Bright green LED
            QRadialGradient ledGrad(ledRect.center(), ledR);
            ledGrad.setColorAt(0.0, QColor(120, 255, 160));
            ledGrad.setColorAt(0.5, QColor(0, 220, 80));
            ledGrad.setColorAt(1.0, QColor(0, 160, 50));
            painter.setPen(QPen(QColor(0, 100, 40), 0.5));
            painter.setBrush(ledGrad);
            painter.drawEllipse(ledRect);
        }
        else
        {
            // Dim LED — dark with faint color
            QRadialGradient ledGrad(ledRect.center(), ledR);
            ledGrad.setColorAt(0.0, QColor(40, 55, 42));
            ledGrad.setColorAt(1.0, QColor(25, 35, 28));
            painter.setPen(QPen(QColor(20, 28, 22), 0.5));
            painter.setBrush(ledGrad);
            painter.drawEllipse(ledRect);
        }
    }
    else
    {
        QRectF target(0.0, 0.0, switchSize.width()*ratio, switchSize.height()*ratio);
        QRectF source(0.0, yOffset, switchSize.width(), switchSize.height());
        QPixmap image(imagePath);
        painter.drawPixmap(target, image, source);
    }
}

void customSwitch::setOffset(signed int imageNr)
{
    this->yOffset = imageNr*switchSize.height();
    this->update();
}

void customSwitch::mousePressEvent(QMouseEvent *event)
{
    if ( event->button() == Qt::LeftButton )
    {
        this->dragStartPosition = event->position().toPoint();
        setFocus();
        if(usePowerButtonStyle || useModernToggle)
        {
            this->pressed = true;
            update();
        }
        else
        {
            setOffset(2);
        }
    };
}

void customSwitch::mouseReleaseEvent(QMouseEvent *event)
{
    if ( event->button() == Qt::LeftButton )
    {
        this->pressed = false;
        if(active)
        {
            if(!useModernToggle && !usePowerButtonStyle) setOffset(0);
            emitValue(false);
        }
        else
        {
            if(!useModernToggle && !usePowerButtonStyle) setOffset(1);
            emitValue(true);
        };
        update();
        clearFocus();
    };
}

void customSwitch::emitValue(bool value)
{
    this->active = value;
    if (value != m_value) { this->m_value = value; };
     emit valueChanged(bool (value), this->hex0, this->hex1, this->hex2, this->hex3);
}

void customSwitch::mouseMoveEvent(QMouseEvent *event)
{
    if (!(event->buttons() == Qt::LeftButton) && (event->position().toPoint() - dragStartPosition).manhattanLength() < QApplication::startDragDistance())
    {
        return;
    };
}

void customSwitch::setValue(bool value)
{
    this->active = value;
    if(usePowerButtonStyle || useModernToggle)
    {
        update();
    }
    else
    {
        if(active)
            setOffset(1);
        else
            setOffset(0);
    }
    clearFocus();
}
