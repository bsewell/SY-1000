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

#include "customKnobRange.h"
#include "SysxIO.h"
#include "Preferences.h"

customKnobRange::customKnobRange(QWidget *parent, QString hex0, QString hex1, QString hex2, QString hexMin, QString hexMax, QString type) : QWidget(parent)
{
    double multiplier = 1;
    if(type.contains("ratio1.25")){multiplier = 1.25;};
    if(type.contains("ratio1.5")){multiplier = 1.5;};
    if(type.contains("ratio1.75")){multiplier = 1.75;};
    if(type.contains("ratio2")){multiplier = 2;};
    Preferences *preferences = Preferences::Instance();
    bool ok;
    double ratio = preferences->getPreferences("Window", "Scale", "ratio").toDouble(&ok)*multiplier;
    QString colour = preferences->getPreferences("Scheme", "Colour", "select");

    this->hex0 = hex0;
    this->hex1 = hex1;
    this->hex2 = hex2;
    this->hexMin = hexMin;
    this->hexMax = hexMax;
    this->type = type;
    int range;
    int rangeMin;

    if (this->type == "Min")
    {
        range = 126;//16382;
        rangeMin = 0;
        this->hex3 = hexMin;
    }
    else
    {
        range = 127;//16383;
        rangeMin = 1;
        this->hex3 = hexMax;
    };

    QPoint bgPos = QPoint(-2*ratio, -3*ratio); // Correction needed y-3.
    QPoint knobPos = QPoint(5*ratio, 4*ratio); // Correction needed y+1 x-1.

    QLabel *newBackGround = new QLabel(this);
    newBackGround->setPixmap(QPixmap(":/images/knobbgn.png").scaled(50*ratio,50*ratio, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    newBackGround->move(bgPos);

    QString imagePath(":/images/knob.png");
    unsigned int imageRange = 100;
    this->knob = new customRangeDial(0, rangeMin, range, 1, 10, knobPos, this, hex0, hex1, hex2, hex3, this->type, imagePath, imageRange, multiplier);
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    this->setFixedSize(newBackGround->pixmap(Qt::ReturnByValue).size() - QSize(0, 4)*ratio);
#else
    this->setFixedSize(newBackGround->pixmap(Qt::ReturnByValue).size() - QSize(0, 4)*ratio); // Correction needed h-4 - space between knob and bottom label.
#endif

    QObject::connect(this, SIGNAL( updateSignal() ),
                     this->parent(), SIGNAL( updateSignal() ));

    QObject::connect(this, SIGNAL( updateDisplayMin(QString) ),
                     this->parent(), SIGNAL( updateDisplayMin(QString) ));

    QObject::connect(this, SIGNAL( updateDisplayMax(QString) ),
                     this->parent(), SIGNAL( updateDisplayMax(QString) ));

    QObject::connect(this, SIGNAL( highlightSignal(QString) ),
                     this->parent(), SLOT( highlight(QString) ));
}

void customKnobRange::paintEvent(QPaintEvent *)
{
    /*DRAWS RED BACKGROUND FOR DEBUGGING PURPOSE */
    /*QPixmap image(":/images/dragbar.png");

	QRectF target(0.0, 0.0, this->width(), this->height());
	QRectF source(0.0, 0.0, this->width(), this->height());

	QPainter painter(this);
	painter.drawPixmap(target, image, source);*/
}

void customKnobRange::setValue(int value)
{
    this->knob->setValue(value);
}

void customKnobRange::valueChanged(int value, QString hex0, QString hex1, QString hex2, QString hex3)
{
    value = value*128;
    QString valueHex = QString::number(value, 16).toUpper();
    if(valueHex.length() < 2) { valueHex.prepend("000"); }
    else if(valueHex.length() < 3) { valueHex.prepend("00"); }
    else if(valueHex.length() < 4) { valueHex.prepend("0"); };

    QList<QString> valueString;
    QString lsb_a = valueHex.at(0);
    lsb_a.prepend("0");
    valueString.append(lsb_a);
    QString lsb_b = valueHex.at(1);
    lsb_b.prepend("0");
    valueString.append(lsb_b);
    QString lsb_c = valueHex.at(2);
    lsb_c.prepend("0");
    valueString.append(lsb_c);
    QString lsb_d = valueHex.at(3);
    lsb_d.prepend("0");
    valueString.append(lsb_d);

    SysxIO *sysxIO = SysxIO::Instance();
    sysxIO->setFileSource(hex0, hex1, hex2, hex3, valueString);
    value=value/128;
    //if(this->type != "Min"){value=value+1; };
    QString valueStr = QString::number(value, 10).toUpper();
    if (this->type == "Min"){	emit updateDisplayMin(valueStr);} else { emit updateDisplayMax(valueStr); };
    emit updateSignal();
}
