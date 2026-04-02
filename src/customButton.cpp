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
#include "customButton.h"
#include "globalVariables.h"
#include "Preferences.h"

#include <iostream>
#include <cstdlib>

customButton::customButton(bool active, QWidget *parent,QString hex0, QString hex1, QString hex2, QString hex3,
                           QString imagePath)
    : QWidget(parent)
{
    Preferences *preferences = Preferences::Instance();
    bool ok;
    const double ratio = preferences->getPreferences("Window", "Scale", "ratio").toDouble(&ok);

    this->hex0 = hex0;
    this->hex1 = hex1;
    this->hex2 = hex2;
    this->hex3 = hex3;
    this->active = active;
    this->textLabel = nullptr;
    this->drawBackground = true;
    this->imagePath = imagePath;
    QSize imageSize = QPixmap(imagePath).size();
    this->buttonSize =  QSize(imageSize.width()*ratio/3, imageSize.height()*ratio/3);
    setOffset(0);
    setGeometry(0, 0, buttonSize.width()*ratio, buttonSize.height()*ratio);

    timer = new QTimer(this);
    QObject::connect(this, SIGNAL( valueChanged(bool, QString, QString, QString, QString) ),
                     this->parent(), SLOT( valueChanged(bool, QString, QString, QString, QString) ));
}

customButton::customButton(QString text, bool active, QPoint buttonPos, QWidget *parent, 
                           QString imagePath)
    : QWidget(parent)
{
    // button can have two names , one for each switch state. setup using "name1/name2" with forward slash separator.
    Preferences *preferences = Preferences::Instance();
    bool ok;
    const double ratio = preferences->getPreferences("Window", "Scale", "ratio").toDouble(&ok);
    const double fratio = preferences->getPreferences("Window", "Font", "ratio").toDouble(&ok);

    this->active = active;
    this->drawBackground = true;
    this->text1 = text;
    this->text1.truncate(text.indexOf("/"));
    this->text2 = text.right(text.indexOf("/")-1);
    if(!text.contains("/")){this->text1 = text2;};
    if(preferences->getPreferences("Midi", "DBug", "bool")=="true" && text2.contains("Line")){text2 = "deBug Mode";};
    this->imagePath = imagePath;
    QSize imageSize = QPixmap(imagePath).size();
    this->buttonSize =  QSize(imageSize.width(), imageSize.height()/4);
    this->buttonPos = buttonPos;
    setOffset(0);
    setGeometry(buttonPos.x(), buttonPos.y(), buttonSize.width()*ratio, buttonSize.height()*ratio);

    this->textLabel = new QLabel(this);
    this->textLabel->setObjectName("button");
    QFont Mfont( "Roboto Condensed", 9*fratio, QFont::Normal);
    this->textLabel->setFont(Mfont);
    this->textLabel->setText(this->text1);
    this->textLabel->setAlignment(Qt::AlignCenter);
    updateTextLabelGeometry();

    timer = new QTimer(this);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(blink()) );

    //std::string st = QString("text1 = " + this->text1 +" text2 = "+ text2).toStdString();
    //std::cerr << st << std::endl;
}

void customButton::paintEvent(QPaintEvent *)
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

void customButton::setOffset(signed int imageNr)
{
    this->yOffset = imageNr*buttonSize.height();
    this->update();
}

void customButton::mousePressEvent(QMouseEvent *event)
{
    if ( event->button() == Qt::LeftButton )
    {
        this->dragStartPosition = event->position().toPoint();
        if(active)
        {
            setOffset(3);
        }
        else
        {
            setOffset(1);
        };
        setFocus();
    };
}

void customButton::mouseReleaseEvent(QMouseEvent *event)
{
    if ( event->button() == Qt::LeftButton )
    {
        if(active)
        {
            setOffset(0);
            setBlink(false);
            emitValue(false);
        }
        else
        {
            setOffset(2);
            emitValue(true);
        };
        clearFocus();
    };
}

void customButton::emitValue(bool value)
{
    this->active = value;
    if(this->hex1.isEmpty() && this->hex2.isEmpty() && this->hex3.isEmpty())
    {
        emit valueChanged(bool(value));
    }
    else
    {
        emit valueChanged(bool(value), this->hex0, this->hex1, this->hex2, this->hex3);
    };
}

void customButton::mouseMoveEvent(QMouseEvent *event)
{
    if (!(event->buttons() == Qt::LeftButton) && (event->position().toPoint() - dragStartPosition).manhattanLength() < QApplication::startDragDistance())
    {
        return;
    };
}

void customButton::setValue(bool value)
{
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Midi", "DBug", "bool")=="true" && text2.contains("Line")){text2 = "deBug Mode";};
    this->active = value;
    if(active)
    {
        setOffset(2);
        this->textLabel->setText(this->text2);
    }
    else
    {
        setOffset(0);
        this->textLabel->setText(this->text1);
    };
    clearFocus();
}

void customButton::setBackgroundVisible(bool visible)
{
    this->drawBackground = visible;
    this->update();
}

void customButton::setLabelPointSize(int pointSize)
{
    if(!this->textLabel)
    {
        return;
    }

    QFont font = this->textLabel->font();
    font.setPointSize(pointSize);
    this->textLabel->setFont(font);
    updateTextLabelGeometry();
}

void customButton::setBlink(bool value)
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
            setOffset(2);
        }
        else
        {
            setOffset(0);
        };
    };
}

void customButton::blink()
{
    if(on)
    {
        on = false;
        setOffset(0);
    }
    else
    {
        on = true;
        setOffset(2);
    };
    clearFocus();
}

void customButton::updateTextLabelGeometry()
{
    if(!this->textLabel)
    {
        return;
    }

    const int labelHeight = this->textLabel->sizeHint().height();
    const int y = qMax(0, (height() - labelHeight) / 2);
    this->textLabel->setGeometry(0, y, width(), labelHeight);
}

QSize customButton::sizeHint() const
{
    Preferences *preferences = Preferences::Instance();
    bool ok;
    const double ratio = preferences->getPreferences("Window", "Scale", "ratio").toDouble(&ok);
    return QSize(buttonSize.width()*ratio, buttonSize.height()*ratio);
}
