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

#include "customControlRange.h"
//#include "MidiTable.h"
#include "SysxIO.h"
#include "Preferences.h"

customControlRange::customControlRange(QWidget *parent,
                                       QString hex0, QString hex1, QString hex2, QString hex3,
                                       QString area)
                                           : QWidget(parent)
{
    Preferences *preferences = Preferences::Instance();
    bool ok;
    double ratio = preferences->getPreferences("Window", "Scale", "ratio").toDouble(&ok);
    double fratio = preferences->getPreferences("Window", "Font", "ratio").toDouble(&ok);
    const int verticalGap = qRound(5 * ratio);
    const int controlMargin = qRound(4 * ratio);
#ifdef Q_OS_MAC
    fratio = fratio*1.4;
#endif
    QFont Sfont( "Roboto Condensed", 8*fratio, QFont::Normal);

    this->displayMin = new QLineEdit(this);
    this->labelMin = new customControlLabel(this);
    this->displayMax = new QLineEdit(this);
    this->labelMax = new customControlLabel(this);
    this->hex0 = hex0;
    this->hex1 = hex1;
    this->hex2 = hex2;
    this->hex3 = hex3;
    this->area = area;
    this->hexMin = hex3;
    QString hexTemp = QString::number((hex3.toInt(&ok, 16) + 4), 16).toUpper();    // go forward 4 to select range Max address
    if(hexTemp.length() < 2) hexTemp.prepend("0");                                 // prepend with "0" if single digit.
    this->hexMax = hexTemp;
    _last_min = -1;
    _last_max = -1;

    this->labelMin->setText("Act Rng LO");
    this->labelMin->setFontRole("setting");
    this->labelMin->setUpperCase(true);
    this->knobMin = new customKnobRange(this, hex0, hex1, hex2, this->hexMin, this->hexMax, "Min");
    this->knobMin->setWhatsThis(tr("hold down mouse button and drag up/down for quick adjustment")
                             + "<br>" + tr("use scroll wheel or up/down arrow keys for fine adjustment"));
    this->displayMin->setObjectName("editdisplay");
    this->displayMin->setFont(Sfont);
    this->displayMin->setFixedWidth(45*ratio);
    this->displayMin->setFixedHeight(15*ratio);
    this->displayMin->setAlignment(Qt::AlignCenter);
    this->displayMin->setDisabled(true);

    this->labelMax->setText("Act Rng HI");
    this->labelMax->setFontRole("setting");
    this->labelMax->setUpperCase(true);
    this->knobMax = new customKnobRange(this, hex0, hex1, hex2, this->hexMin, this->hexMax, "Max");
    this->knobMax->setWhatsThis(tr("hold down mouse button and drag up/down for quick adjustment")
                             + "<br>" + tr("use scroll wheel or up/down arrow keys for fine adjustment"));
    this->displayMax->setObjectName("editdisplay");
    this->displayMax->setFont(Sfont);
    this->displayMax->setFixedWidth(45*ratio);
    this->displayMax->setFixedHeight(15*ratio);
    this->displayMax->setAlignment(Qt::AlignCenter);
    this->displayMax->setDisabled(true);

    QVBoxLayout *minLayout = new QVBoxLayout;
    minLayout->setContentsMargins(controlMargin, verticalGap, controlMargin, verticalGap);
    minLayout->setSpacing(verticalGap);
    minLayout->addWidget(this->labelMin, 0, Qt::AlignCenter);
    minLayout->addWidget(this->knobMin, 0, Qt::AlignCenter);
    minLayout->addWidget(this->displayMin, 0, Qt::AlignCenter);
    minLayout->addStretch(0);

    QVBoxLayout *maxLayout = new QVBoxLayout;
    maxLayout->setContentsMargins(controlMargin, verticalGap, controlMargin, verticalGap);
    maxLayout->setSpacing(verticalGap);
    maxLayout->addWidget(this->labelMax, 0, Qt::AlignCenter);
    maxLayout->addWidget(this->knobMax, 0, Qt::AlignCenter);
    maxLayout->addWidget(this->displayMax, 0, Qt::AlignCenter);
    maxLayout->addStretch(0);

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->setContentsMargins(controlMargin, 0, controlMargin, 0);
    mainLayout->setSpacing(qRound(10 * ratio));
    mainLayout->addLayout(minLayout);
    mainLayout->addLayout(maxLayout);
    mainLayout->addStretch(0);

    this->setLayout(mainLayout);
    this->setFixedHeight((this->knobMin->height() + 13 + 12)*ratio);
    this->setFixedHeight((this->knobMax->height() + 13 + 12)*ratio);

    QObject::connect(this->parent(), SIGNAL( dialogUpdateSignal() ),
                     this, SLOT( dialogUpdateSignal() ));

    QObject::connect(this, SIGNAL( updateSignal() ),
                     this->parent(), SIGNAL( updateSignal() ));

    QObject::connect(this, SIGNAL( updateDisplayMin(QString) ),
                     this->displayMin, SLOT( setText(QString) ));

    QObject::connect(this, SIGNAL( updateDisplayMax(QString) ),
                     this->displayMax, SLOT( setText(QString) ));

    hover = false;
    timer = new QTimer(this);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(timed()) );

    setMouseTracking(true);
}

void customControlRange::paintEvent(QPaintEvent *)
{
    /* DRAWS RED BACKGROUND  */
    if(hover==true && this->id.contains("Min")){
        QPixmap image(":/images/dragbar.png");
        QRectF target(0.0, 0.0, this->width()/2, this->height());
        QRectF source(0.0, 0.0, 600, 600);
        QPainter painter(this);
        painter.drawPixmap(target, image, source);
    }
    else if(hover==true && this->id.contains("Max")){
        QPixmap image(":/images/dragbar.png");
        QRectF target(this->width()/2, 0.0, this->width(), this->height());
        QRectF source(0.0, 0.0, 600, 600);
        QPainter painter(this);
        painter.drawPixmap(target, image, source);
    };
}

void customControlRange::mouseMoveEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    if(this->knobMin->knob->underMouse()){this->knobMin->knob->setFocus(); this->id = "Min"; hover=true;  repaint(); this->timer->start(500);};
    if(this->knobMax->knob->underMouse()){this->knobMax->knob->setFocus(); this->id = "Max"; hover=true;  repaint(); this->timer->start(500);};
}

void customControlRange::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    if(this->knobMin->knob->underMouse()){this->knobMin->knob->setFocus(); this->id = "Min"; hover=true;  repaint(); this->timer->start(500);};
    if(this->knobMax->knob->underMouse()){this->knobMax->knob->setFocus(); this->id = "Max"; hover=true;  repaint(); this->timer->start(500);};
}

void customControlRange::dialogUpdateSignal()
{
    int value = getMultiByteValue(hexMin, 4);
    if(value != _last_min){
        _last_min = value;
    this->knobMin->setValue(value/128);
    emit updateDisplayMin(QString::number(value/128, 10));
    };

    value = getMultiByteValue(hexMax, 4);
    if(value != _last_max){
     _last_max = value;
    this->knobMax->setValue(value/128);
    emit updateDisplayMax(QString::number(value/128, 10));
    };
}

int customControlRange::getMultiByteValue(QString address, int size)
{
    SysxIO *sysxIO = SysxIO::Instance();
    int value = 0;
    int multiplier = 1;
    bool ok;
    for (int x=size; x>0; x--)
    {
        QString hexTemp = QString::number((address.toInt(&ok, 16) + (x-1)), 16).toUpper();
        if(hexTemp.length() < 2) hexTemp.prepend("0");

    value = value + (sysxIO->getSourceValue(this->hex0, this->hex1, this->hex2, hexTemp)*multiplier);
    multiplier = multiplier*16;

    }
return value;
}

void customControlRange::timed()
{
    if(this->underMouse() || this->knobMin->knob->hasFocus() || this->knobMax->knob->hasFocus())
    {
        this->timer->stop();
        this->timer->start(500);

    }else{
    this->timer->stop();
    this->hover=false;
    this->repaint();
    this->knobMin->knob->clearFocus();
    };
}

void customControlRange::highlight(QString id)
{
    this->id = id;
    this->hover=true;
    this->repaint();
    this->timer->start(500);
}
