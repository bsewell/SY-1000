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

#include "customControlSpinBox.h"
#include "MidiTable.h"
#include "SysxIO.h"
#include "Preferences.h"

#include <iostream>
#include <cstdlib>

customControlSpinBox::customControlSpinBox(QWidget *parent,
                                             QString hex0, QString hex1, QString hex2, QString hex3, QString direction)
                                                 : QWidget(parent)
{
    Preferences *preferences = Preferences::Instance();
    bool ok;
    const double ratio = preferences->getPreferences("Window", "Scale", "ratio").toDouble(&ok);

    this->direction = direction;
    this->label = new customControlLabel(this);
    this->controlSpinBox = new QSpinBox(this);
    int largeFont = 10;
    int smallFont = 6;
#ifdef Q_OS_MAC
    largeFont = 14;
    smallFont = 9;
#endif

    if(direction.contains("Tables") || direction.contains("large"))
    {
        this->controlSpinBox->setObjectName("spinbox");
        QFont Cfont( "Arial", largeFont*ratio, QFont::Normal);
        this->controlSpinBox->setFont(Cfont);
    }
    else
    {
        this->controlSpinBox->setObjectName("spinbox");
        QFont Cfont( "Arial", smallFont*ratio, QFont::Normal);
        this->controlSpinBox->setFont(Cfont);
    };
    this->hex0 = hex0;
    this->hex1 = hex1;
    this->hex2 = hex2;
    this->hex3 = hex3;
    if (direction.contains("System")) {this->area = "System"; }
    else if (direction.contains("Tables")) {this->area = "Tables"; }
    else if (hex0.contains("Tables")) {this->area = "Tables"; }
    else {this->area = "Structure"; };

    MidiTable *midiTable = MidiTable::Instance();
    Midi items;

    items = midiTable->getMidiMap(hex0, hex1, hex2, hex3);

    QString labeltxt = items.customdesc;

    this->label->setUpperCase(true);
    this->label->setText(labeltxt);

    rangeMin = midiTable->getRangeMinimum(hex0, hex1, hex2, hex3);
    QString min = QString::number(rangeMin, 16);
    if(min.size()<2){min.prepend("0");};
    QString minRange = midiTable->getValue(hex0, hex1, hex2, hex3, min);
    int minRng = minRange.toInt(&ok, 10);
    this->controlSpinBox->setMinimum(minRng);
    this->rangeOffset=minRng;

    rangeMax = midiTable->getRange(hex0, hex1, hex2, hex3);
    QString max = QString::number(rangeMax, 16);
    if(max.size()<2){max.prepend("0");};
    QString maxRange = midiTable->getValue(hex0, hex1, hex2, hex3, max);
    int maxRng = maxRange.toInt(&ok, 10);
    this->controlSpinBox->setMaximum(maxRng);
    this->max = maxRng-1;

    setSpinBox();

    if(direction == "left")
    {

    }
    else if(direction == "right")
    {

    }
    else if(direction == "top")
    {

    }
    else if(direction.contains("bottom"))
    {
        this->label->setAlignment(Qt::AlignLeft);

        QVBoxLayout *mainLayout = new QVBoxLayout;
        mainLayout->setContentsMargins(0, 0, 0, 0);
        mainLayout->setSpacing(0);
        mainLayout->addStretch(0);
        mainLayout->addWidget(this->label, 0, Qt::AlignLeft);
        mainLayout->addWidget(this->controlSpinBox, 0, Qt::AlignLeft);

        this->setLayout(mainLayout);
    }
    else
    {
        this->label->setAlignment(Qt::AlignCenter);

        QVBoxLayout *mainLayout = new QVBoxLayout;
        mainLayout->setContentsMargins(0, 0, 0, 0);
        mainLayout->setSpacing(0);
        mainLayout->addStretch(0);
        mainLayout->addWidget(this->label, 0, Qt::AlignCenter);
        mainLayout->addWidget(this->controlSpinBox, 0, Qt::AlignCenter);

        this->setLayout(mainLayout);
    };
    if(midiTable->isData(this->area, hex1, hex2, hex3))
        { 
          //this->setFixedHeight((12 + 25)*ratio);
        } 
      else if(direction.contains("Tables") || direction.contains("large"))
        { 
          //this->setFixedHeight((12 + 30)*ratio);
        }
      else
        { 
          //this->setFixedHeight((12 + 20)*ratio);
        };
       //this->controlSpinBox->setMinimumWidth(35*ratio);
    //setMouseTracking(true);

    QObject::connect(this->parent(), SIGNAL( dialogUpdateSignal() ), this, SLOT( dialogUpdateSignal() ));

    QObject::connect(this, SIGNAL( updateSignal() ), this->parent(), SIGNAL( updateSignal() ));

    QObject::connect(this->controlSpinBox, SIGNAL(valueChanged(int)), this, SLOT(valueChanged(int)));

    QObject::connect(this->controlSpinBox, SIGNAL(valueChanged(int)), this, SIGNAL(updateSignal()));

    hover = false;
    timer = new QTimer(this);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(timed()) );

    setMouseTracking(true);

}

void customControlSpinBox::paintEvent(QPaintEvent *)
{
    /* DRAWS RED BACKGROUND  */
    if(hover==true){
        QPixmap image(":/images/dragbar.png");
        QRectF target(0.0, 0.0, this->width(), this->height());
        QRectF source(0.0, 0.0, 600, 600);
        QPainter painter(this);
        painter.drawPixmap(target, image, source);
    };
}

void customControlSpinBox::mouseMoveEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    //if(this->underMouse() ){this->controlSpinBox->setFocus(); hover=true;  repaint(); this->timer->start(500);};
    this->controlSpinBox->setFocus(); hover=true;  repaint(); this->timer->start(500);
}

void customControlSpinBox::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    //if(this->underMouse() ){this->controlSpinBox->setFocus(); hover=true;  repaint(); this->timer->start(500);};
    this->controlSpinBox->setFocus(); hover=true;  repaint(); this->timer->start(500);
}

void customControlSpinBox::keyPressEvent(QKeyEvent *event)
{
    int value = 0;

    switch(event->key())
    {
    case Qt::Key_Up: value = 1;break;
    case Qt::Key_Down: value = -1;break;
    case Qt::Key_Plus: value = 1;break;
    case Qt::Key_Equal: value = 1;break;
    case Qt::Key_Minus: value = -1;break;
    case Qt::Key_Q: value = 1;break;
    case Qt::Key_A: value = -1;break;
    case Qt::Key_P: value = 1;break;
    case Qt::Key_L: value = -1;break;

    case Qt::Key_PageUp: value = 10;break;
    case Qt::Key_PageDown: value = -10;break;
    case Qt::Key_W: value = 10;break;
    case Qt::Key_S: value = -10;break;
    case Qt::Key_O: value = 10;break;
    case Qt::Key_K: value = -10;break;

    case Qt::Key_Right: value = max;break;
    case Qt::Key_Left: value = min;break;
    case Qt::Key_E: value = max;break;
    case Qt::Key_D: value = min-_lastValue;break;
    case Qt::Key_I: value = max;break;
    case Qt::Key_J: value = min-_lastValue;break;
    };

    if(value!=0){
        int _newValue = _lastValue + value;
        if(_newValue <= min)
        {
            _newValue = min;
        }
        else if(_newValue >= max)
        {
            _newValue = max;
        };
        //set_index(_newValue);
        valueChanged(_newValue);

        QString val = QString::number(_newValue, 10);
        std::string st = QString("spin-box key press event = " + val + " "+QString::number(value,10)).toStdString();
        std::cerr << st << std::endl;
    };
}

void customControlSpinBox::setSpinBox()
{
    /*Preferences *preferences = Preferences::Instance();
    bool ok;
    const double ratio = preferences->getPreferences("Window", "Scale", "ratio").toDouble(&ok);


      if(this->direction.contains("Tables") || this->direction.contains("large"))
    {
        this->controlSpinBox->setFixedHeight(28*ratio);
    }
    else
    {
        this->controlSpinBox->setFixedHeight(20*ratio);
    };*/

    this->controlSpinBox->setFrame(true);

      this->dialogUpdateSignal();
}

void customControlSpinBox::valueChanged(int index)
{
    index=index+(rangeMin)-(rangeOffset);
if(!this->direction.contains("Tables") && !this->hex0.contains("Tables"))
    {
    QList<QString> hexData;
    QString valueHex = QString::number(index, 16).toUpper();
    if(valueHex.length() < 2) valueHex.prepend("0");

    SysxIO *sysxIO = SysxIO::Instance();
    MidiTable *midiTable = MidiTable::Instance();
    bool ok;
    if(midiTable->isData(this->hex0, this->hex1, this->hex2, this->hex3))
    {
        int maxRange = 128;
        int value = valueHex.toInt(&ok, 16);
        int dif = value/maxRange;
        QString valueHex1 = QString::number(dif, 16).toUpper();
        if(valueHex1.length() < 2) valueHex1.prepend("0");
        QString valueHex2 = QString::number(value - (dif * maxRange), 16).toUpper();
        if(valueHex2.length() < 2) valueHex2.prepend("0");
        hexData.append(valueHex1);
        hexData.append(valueHex2);
        sysxIO->setFileSource(this->hex0, this->hex1, this->hex2, this->hex3, hexData);
    }
    else
    {
        sysxIO->setFileSource(this->hex0, this->hex1, this->hex2, this->hex3, valueHex);
    };
    emit updateSignal();
    };
}

void customControlSpinBox::dialogUpdateSignal()
{
   if(!this->direction.contains("Tables") && !this->hex0.contains("Tables"))
    {
    SysxIO *sysxIO = SysxIO::Instance();
    int index = sysxIO->getSourceValue(this->hex0, this->hex1, this->hex2, this->hex3);
    if(index != _lastValue){
        _lastValue = index;
    this->controlSpinBox->setValue(index+(rangeOffset)-rangeMin);
    //this->valueChanged(index);
    };
   };
}

void customControlSpinBox::timed()
{
    if(this->underMouse())
    {
        this->timer->stop();
        this->timer->start(500);

    }else{
    this->timer->stop();
    this->hover=false;
    this->repaint();
    this->controlSpinBox->clearFocus();
    };
}

void customControlSpinBox::highlight()
{
    this->hover=true;
    this->repaint();
    this->timer->start(500);
}
