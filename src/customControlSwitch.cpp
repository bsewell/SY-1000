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

#include "customControlSwitch.h"
#include "MidiTable.h"
#include "SysxIO.h"
#include "Preferences.h"

#include <iostream>
#include <cstdlib>

customControlSwitch::customControlSwitch(QWidget *parent, 
                                         QString hex0, QString hex1, QString hex2, QString hex3,
                                         QString direction)
    : QWidget(parent)
{
    Preferences *preferences = Preferences::Instance();
    bool ok;
    const double ratio = preferences->getPreferences("Window", "Scale", "ratio").toDouble(&ok);
    const int verticalGap = qRound(5 * ratio);
    const int sideGap = qRound(8 * ratio);
    const int controlMargin = qRound(4 * ratio);

    this->label = new customControlLabel(this);
    this->hex0 = hex0;
    this->hex1 = hex1;
    this->hex2 = hex2;
    this->hex3 = hex3;
    this->area = direction;
    if (this->area != "System"){area = "Structure";};
    MidiTable *midiTable = MidiTable::Instance();
    Midi items = midiTable->getMidiMap(hex0, hex1, hex2, hex3);
    QString labeltxt = items.customdesc;

    this->label->setFontRole("setting");
    this->label->setUpperCase(true);
    this->label->setText(labeltxt);
    QString imagePath  = ":/images/ledbutton.png";
    if(direction.contains("invert")) { imagePath = ":/images/switch_invert.png"; };
    this->switchbutton = new customSwitch(false, this, hex0, hex1, hex2, hex3, imagePath);

    if(direction == "left")
    {
        this->label->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        this->label->setTextRaise(qRound(1 * ratio));

        QHBoxLayout *mainLayout = new QHBoxLayout;
        mainLayout->setContentsMargins(controlMargin, verticalGap, controlMargin, verticalGap);
        mainLayout->setSpacing(sideGap);
        mainLayout->addWidget(this->switchbutton, 0, Qt::AlignLeft | Qt::AlignVCenter);
        mainLayout->addWidget(this->label, 0, Qt::AlignLeft | Qt::AlignVCenter);
        mainLayout->addStretch(1);

        this->setLayout(mainLayout);
    }
    else if(direction == "right")
    {

    }
    else if(direction == "top")
    {

    }
    else if(direction.contains("bottom"))
    {
        this->label->setAlignment(Qt::AlignCenter);
        QVBoxLayout *mainLayout = new QVBoxLayout;
        mainLayout->setContentsMargins(controlMargin, verticalGap, controlMargin, verticalGap);
        mainLayout->setSpacing(verticalGap);
        mainLayout->addWidget(this->label, 0, Qt::AlignCenter);
        mainLayout->addWidget(this->switchbutton, 0, Qt::AlignCenter);
        mainLayout->addStretch(0);

        this->setLayout(mainLayout);
        //this->setFixedHeight(12 + 20);
    }
    else if(direction.contains("middle") || direction.contains("System")  || direction.contains("invert"))
    {
        this->label->setAlignment(Qt::AlignCenter);

        QVBoxLayout *mainLayout = new QVBoxLayout;
        mainLayout->setContentsMargins(controlMargin, verticalGap, controlMargin, verticalGap);
        mainLayout->setSpacing(verticalGap);
        mainLayout->addStretch(0);
        mainLayout->addWidget(this->label, 0, Qt::AlignCenter);
        mainLayout->addWidget(this->switchbutton, 0, Qt::AlignCenter);

        this->setLayout(mainLayout);
        //this->setFixedHeight(12 + 20);
    };

    QObject::connect(this->parent(), SIGNAL( dialogUpdateSignal() ),
                     this, SLOT( dialogUpdateSignal() ));

    QObject::connect(this, SIGNAL( updateSignal() ),
                     this->parent(), SIGNAL( updateSignal() ));

    hover = false;
    timer = new QTimer(this);


    setMouseTracking(true);
}

void customControlSwitch::setLabelText(const QString &text)
{
    this->label->setText(text);
}

void customControlSwitch::paintEvent(QPaintEvent *)
{
}

void customControlSwitch::valueChanged(bool value, QString hex0, QString hex1, QString hex2, QString hex3)
{
    QString valueHex;
    if(value)
    {
        valueHex = "01";
    }
    else
    {
        valueHex = "00";
    };

    SysxIO *sysxIO = SysxIO::Instance();
    sysxIO->setFileSource(hex0, hex1, hex2, hex3, valueHex);

    emit updateSignal();
}

void customControlSwitch::dialogUpdateSignal()
{
    SysxIO *sysxIO = SysxIO::Instance();
    int value = sysxIO->getSourceValue(this->hex0, this->hex1, this->hex2, this->hex3);
    if(value != _last){
        _last = value;
        if(value == 0)
        {
            this->switchbutton->setValue(false);
            this->_lastValue = false;
        }
        else
        {
            this->switchbutton->setValue(true);
            this->_lastValue = true;
        };
    };
}

void customControlSwitch::mouseMoveEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    if(this->underMouse() ){this->setFocus(); hover=true;  repaint();     QObject::connect(timer, SIGNAL(timeout()), this, SLOT(timed()) ); this->timer->start(500);};
    //std::string st = QString("combobox mouse event = "  ).toStdString();
    //std::cerr << st << std::endl;
}

void customControlSwitch::keyPressEvent(QKeyEvent *event)
{
    bool value = false;

    switch(event->key())
    {
    case Qt::Key_Up: value = true;break;
    case Qt::Key_Down: value = false;break;
    case Qt::Key_Plus: value = true;break;
    case Qt::Key_Equal: value = true;break;
    case Qt::Key_Minus: value = false;break;
    case Qt::Key_Q: value = true;break;
    case Qt::Key_A: value = false;break;
    case Qt::Key_P: value = true;break;
    case Qt::Key_L: value = false;break;

    case Qt::Key_PageUp: value = true;break;
    case Qt::Key_PageDown: value = false;break;
    case Qt::Key_W: value = true;break;
    case Qt::Key_S: value = false;break;
    case Qt::Key_O: value = true;break;
    case Qt::Key_K: value = false;break;

    case Qt::Key_Right: value = true;break;
    case Qt::Key_Left: value = false;break;
    case Qt::Key_E: value = true;break;
    case Qt::Key_D: value = false;break;
    case Qt::Key_I: value = true;break;
    case Qt::Key_J: value = false;break;
    };

    if(value!=_lastValue){

        _lastValue = value;
        this->switchbutton->setValue(value);
        valueChanged(value, this->hex0, this->hex1, this->hex2, this->hex3 );
    };

    //std::string st = QString("custom switch key press event = " ).toStdString();
    //std::cerr << st << std::endl;
}

void customControlSwitch::timed()
{
    if(this->underMouse() || this->hasFocus())
    {
        //this->timer->stop();
        this->timer->start(500);

    }else{
        this->timer->stop();
        QObject::disconnect(timer, SIGNAL(timeout()), this, SLOT(timed()) );
        this->hover=false;
        this->repaint();
        clearFocus();
    };
}
