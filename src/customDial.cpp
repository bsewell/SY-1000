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
#include "customDial.h"
#include "Preferences.h"

customDial::customDial(double value, double min, double max, double single, double page, 
                       QPoint dialPos, QWidget *parent, QString hex0, QString hex1, QString hex2, QString hex3,
                       QString imagePath, uint imageRange, double ratio)
                           : QWidget(parent)
{
    Preferences *preferences = Preferences::Instance();
    bool ok;
    this->ratio = ratio*(preferences->getPreferences("Window", "Scale", "ratio").toDouble(&ok))/3;
    QString colour = preferences->getPreferences("Scheme", "Colour", "select");

    this->hex0 = hex0;
    this->hex1 = hex1;
    this->hex2 = hex2;
    this->hex3 = hex3;
    this->value = value;
    this->min = min;
    this->max = max;
    this->single = single;
    this->page = page;
    this->imageRange = imageRange;
    this->imagePath = imagePath;
    QSize imageSize = QPixmap(imagePath).size();
    this->dialSize = QSize(imageSize.width()/(imageRange+1), imageSize.height());
    this->dialPos = dialPos;

    setOffset(value);
    setGeometry(dialPos.x(), dialPos.y(), dialSize.width()*ratio, dialSize.height()*ratio);
    QObject::connect(this, SIGNAL( valueChanged(int, QString, QString, QString, QString) ),
                     this->parent(), SLOT( valueChanged(int, QString, QString, QString, QString) ));

    QObject::connect(this, SIGNAL( highlightSignal() ),
                     this->parent(), SIGNAL( highlightSignal() ));
}

void customDial::paintEvent(QPaintEvent *)
{
    QRectF target(0.0 , 0.0, dialSize.width()*ratio, dialSize.height()*ratio);
    QRectF source(xOffset, 0.0, dialSize.width(), dialSize.height());
    QPixmap image(imagePath);
    //image.setMask(image.mask());

    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    painter.drawPixmap(target, image, source);

}

void customDial::setOffset(double _newValue)
{
    double dataRange = max - min;
    double range = imageRange - 0;
    double result = (_newValue - min) * (range / dataRange);

    int imageNr = int(result + 0.5);
    if(imageNr < 0)
    {
        imageNr = 0;
    }
    else if(imageNr > int(imageRange*dialSize.width()))
    {
        imageNr = int(imageRange*dialSize.width());
    };

    this->value = _newValue;
    this->xOffset = imageNr*dialSize.width();
    this->update();
}

void customDial::mouseHoverEvent(QHoverEvent *event)
{
    if ( event->HoverEnter != 0 )
    {
        this->_lastpos = event->position().toPoint();
        this->_startpos = event->position().toPoint();
        emitValue(value);
    };
    //std::string st = QString("custom dial mouse hover event "  ).toStdString();
    //std::cerr << st << std::endl;
}

void customDial::mousePressEvent(QMouseEvent *event)
{
    if ( event->button() == Qt::LeftButton )
    {
        this->_lastpos = event->position().toPoint();
        this->_startpos = event->position().toPoint();
        this->_lastValue = value;
        setFocus();
        emitValue(value);
    };
    if ( event->button() == Qt::RightButton )
    {
        this->_lastpos = event->position().toPoint();
        this->_startpos = event->position().toPoint();
        this->_lastValue = value;
        clearFocus();
        emitValue(value);
    };

    //std::string st = QString("custom dial mouse press event "  ).toStdString();
    //std::cerr << st << std::endl;

    emit highlightSignal();
}

void customDial::mouseMoveEvent(QMouseEvent *event)
{
    setFocus();
    double dataRange = max - min;
    //double range = imageRange - 0;
    double distY = double(event->position().toPoint().y()) - double(_startpos.y());
    double numSteps = int((distY/1.5) + 0.5);
    double relativeSteps = numSteps * (dataRange / 100); // To make the mousing distance the same for all.
    numSteps = relativeSteps;

    double _newValue = _lastValue - (numSteps * single);

    if( (_startpos.y() < _lastpos.y() && _newValue < min)
        || (_startpos.y() > _lastpos.y() && _newValue > max) )
	{
        this->_startpos =  _lastpos;
    };

    if(_newValue < min)
    {
        _newValue = min;
        this->_lastValue = value;
    }
    else if(_newValue > max)
    {
        _newValue = max;
        this->_lastValue = value;
    };

    this->_lastpos = event->position().toPoint();
    setOffset(_newValue);
    emitValue(_newValue);

    //std::string st = QString("custom dial mouse move event "  ).toStdString();
    //std::cerr << st << std::endl;
}

void customDial::wheelEvent(QWheelEvent *event)
{
    QPoint numDegrees = event->angleDelta() / 8;
    double numSteps = (numDegrees.ry() / 15);

    if (!event->angleDelta().isNull())
    {
        this->_lastValue = value;

        double _newValue = _lastValue + (numSteps * single);

        if(_newValue < min)
        {
            _newValue = min;
        }
        else if(_newValue > max)
        {
            _newValue = max;
        };
        setOffset(_newValue);
        emitValue(_newValue);
        event->accept();
    };
}

void customDial::keyPressEvent(QKeyEvent *event)
{
    double numSteps = 0;
    this->_lastValue = value;

    switch(event->key())
    {
    case Qt::Key_Up: numSteps = -single;break;
    case Qt::Key_Down: numSteps = single;break;
    case Qt::Key_Plus: numSteps = -single;break;
    case Qt::Key_Equal: numSteps = -single;break;
    case Qt::Key_Minus: numSteps = single;break;
    case Qt::Key_Q: numSteps = -single;break;
    case Qt::Key_A: numSteps = single;break;
    case Qt::Key_P: numSteps = -single;break;
    case Qt::Key_L: numSteps = single;break;

    case Qt::Key_PageUp: numSteps = -page;break;
    case Qt::Key_PageDown: numSteps = page;break;
    case Qt::Key_W: numSteps = -page;break;
    case Qt::Key_S: numSteps = page;break;
    case Qt::Key_O: numSteps = -page;break;
    case Qt::Key_K: numSteps = page;break;

    case Qt::Key_Right: numSteps = -(max-min);break;
    case Qt::Key_Left: numSteps = max-min;break;
    case Qt::Key_E: numSteps = -(max-min);break;
    case Qt::Key_D: numSteps = max-min;break;
    case Qt::Key_I: numSteps = -(max-min);break;
    case Qt::Key_J: numSteps = max-min;break;
    };

    if (numSteps!=0)
    {
        double _newValue = _lastValue - numSteps;
        if(_newValue < min)
        {
            _newValue = min;
            this->_lastValue = value;
        }
        else if(_newValue > max)
        {
            _newValue = max;
            this->_lastValue = value;
        };
        setOffset(_newValue);
        emitValue(_newValue);
    };
}

void customDial::emitValue(double value)
{
    if (value != m_value) {
        this->m_value = value;
        emit valueChanged(int (value), this->hex0, this->hex1, this->hex2, this->hex3);
    };
}

void customDial::setValue(int value)
{
    setOffset(double(value));
}

