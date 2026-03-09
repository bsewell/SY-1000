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
#include "customPanelButton.h"
#include "globalVariables.h"
#include "Preferences.h"

customPanelButton::customPanelButton(bool active, QPoint buttonPos, QWidget *parent, QString hex1, QString hex2, QString hex3,
                                     QString imagePath)
    : QWidget(parent)
{
    Preferences *preferences = Preferences::Instance();
    bool ok;
    const double ratio = preferences->getPreferences("Window", "Scale", "ratio").toDouble(&ok);

    this->hex1 = hex1;
    this->hex2 = hex2;
    this->hex3 = hex3;
    this->textLabel = nullptr;
    this->active = active;
    this->drawBackground = true;
    this->imagePath = imagePath;
    QSize imageSize = QPixmap(imagePath).size();
    this->buttonSize =  QSize(imageSize.width(), imageSize.height()/4);
    this->buttonPos = buttonPos;
    setOffset(0);
    setGeometry(buttonPos.x(), buttonPos.y(), buttonSize.width()*ratio, buttonSize.height()*ratio);

    timer = new QTimer(this);

    QObject::connect(this, SIGNAL( valueChanged(bool, QString, QString, QString) ),
                     this->parent(), SLOT( valueChanged(bool, QString, QString, QString) ));

}

customPanelButton::customPanelButton(QString text, bool active, QPoint buttonPos, QWidget *parent,
                                     QString imagePath)
    : QWidget(parent)
{
    Preferences *preferences = Preferences::Instance();
    bool ok;
    const double ratio = preferences->getPreferences("Window", "Scale", "ratio").toDouble(&ok);

    this->hex1 = "";
    this->hex2 = "";
    this->hex3 = "";
    this->textLabel = nullptr;
    this->active = active;
    this->drawBackground = true;
    this->text = text;
    this->imagePath = imagePath;
    QSize imageSize = QPixmap(imagePath).size();
    this->buttonSize =  QSize(imageSize.width(), imageSize.height()/3);
    this->buttonPos = buttonPos;

    setOffset(0);
    previousOffset = 0;
    setGeometry(buttonPos.x(), buttonPos.y(), buttonSize.width()*ratio, buttonSize.height()*ratio);
    customPanelLabel(text);
}


customPanelButton::customPanelButton(int x, bool active, QPoint buttonPos, QWidget *parent,
                                     QString imagePath)
    : QWidget(parent)
{
    Q_UNUSED(x);
    Preferences *preferences = Preferences::Instance();
    bool ok;
    const double ratio = preferences->getPreferences("Window", "Scale", "ratio").toDouble(&ok);
    
    this->hex1 = "";
    this->hex2 = "";
    this->hex3 = "";
    this->textLabel = nullptr;
    this->active = active;
    this->drawBackground = true;
    this->imagePath = imagePath;
    QSize imageSize = QPixmap(imagePath).size();
    this->buttonSize =  QSize(imageSize.width(), imageSize.height()/2);
    this->buttonPos = buttonPos;

    setOffset(0);
    previousOffset = 0;
    setGeometry(buttonPos.x(), buttonPos.y(), buttonSize.width()*ratio, buttonSize.height()*ratio);
}

void customPanelButton::customPanelLabel(QString text){

    Preferences *preferences = Preferences::Instance();
    bool ok;
    const double fratio = preferences->getPreferences("Window", "Font", "ratio").toDouble(&ok);

    textLabel = new QLabel(this);
    textLabel->setObjectName("panelButton");
    QFont Mfont( "Arial", 9*fratio, QFont::Normal);
    textLabel->setFont(Mfont);
    textLabel->setText(text);
    textLabel->setAlignment(Qt::AlignCenter);
    updateLabelGeometry();
}

void customPanelButton::paintEvent(QPaintEvent *)
{
    if(!this->drawBackground)
    {
        return;
    }

    Preferences *preferences = Preferences::Instance();
    bool ok;
    const double ratio = preferences->getPreferences("Window", "Scale", "ratio").toDouble(&ok);

    QRectF target(0.0 , 0.0, buttonSize.width()*ratio, buttonSize.height()*ratio);
    QRectF source(0.0, yOffset, buttonSize.width(), buttonSize.height());
    QPixmap image(imagePath);

    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    painter.drawPixmap(target, image, source);
}

void customPanelButton::setOffset(signed int imageNr)
{
    this->yOffset = imageNr*buttonSize.height();
    this->repaint();
}

void customPanelButton::mousePressEvent(QMouseEvent *event)
{

    if ( event->button() == Qt::LeftButton  || event->button() == Qt::RightButton )
    {
       // this->setFocus();
        setOffset(2);


    };
}

void customPanelButton::mouseReleaseEvent(QMouseEvent *event)
{
    if ( event->button() == Qt::LeftButton   || event->button() == Qt::RightButton)
    {
        //this->clearFocus();
         emitValue(true);
         setOffset(0);
    };
}

void customPanelButton::emitValue(bool value)
{
    this->active = value;
    if(this->hex1.isEmpty() && this->hex2.isEmpty() && this->hex3.isEmpty())
    {
        emit valueChanged((bool)value);
    }
    else
    {
        emit valueChanged((bool)value, this->hex1, this->hex2, this->hex3);
    };
}

void customPanelButton::mouseMoveEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
   /* if (!(event->buttons() == Qt::LeftButton) && (event->pos() - dragStartPosition).manhattanLength() < QApplication::startDragDistance())
    {
        return;
    };*/
}

void customPanelButton::setValue(bool value)
{
    this->active = value;
    if(active)
    {
        setOffset(1);
        previousOffset = 1;
    }
    else
    {
        setOffset(0);
        previousOffset = 0;
    };
}

void customPanelButton::setBackgroundVisible(bool visible)
{
    this->drawBackground = visible;
    this->update();
}

void customPanelButton::setLabelPointSize(int pointSize)
{
    if(!this->textLabel)
    {
        return;
    }

    QFont font = this->textLabel->font();
    font.setPointSize(pointSize);
    this->textLabel->setFont(font);
    updateLabelGeometry();
}

void customPanelButton::setBlink(bool value)
{
    if(value)
    {
        timer->start(buttonBlinkInterval);
    }
    else
    {
        timer->stop();
        if(active)
        {
            setOffset(1);
        }
        else
        {
            setOffset(0);
        };
    };
}

void customPanelButton::blink()
{
    if(on)
    {
        on = false;
        setOffset(0);
    }
    else
    {
        on = true;
        setOffset(1);
    };
    clearFocus();
}

void customPanelButton::updateLabelGeometry()
{
    if(!this->textLabel)
    {
        return;
    }

    const int labelHeight = this->textLabel->sizeHint().height();
    const int y = qMax(0, (height() - labelHeight) / 2);
    this->textLabel->setGeometry(0, y, width(), labelHeight);
}

