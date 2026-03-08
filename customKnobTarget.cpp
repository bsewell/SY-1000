/****************************************************************************
**
** Copyright (C) 2007~2025 Colin Willcocks.
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

#include "customKnobTarget.h"
#include "MidiTable.h"
#include "SysxIO.h"
#include "Preferences.h"

customKnobTarget::customKnobTarget(QWidget *parent, 
                                   QString hex0, QString hex1, QString hex2, QString hex3,
                                   QString hexMsb, QString hexLsb, QString background)
    : QWidget(parent)
{
    Preferences *preferences = Preferences::Instance();
    bool ok;
    const double ratio = preferences->getPreferences("Window", "Scale", "ratio").toDouble(&ok);

    this->hex0 = hex0;
    this->hex1 = hex1;
    this->hex2 = hex2;
    this->hex3 = hex3;
    this->hexMsb = hexMsb;
    this->hexLsb = hexLsb;
    this->background = background;

    QString mode_hex = "00";

    MidiTable *midiTable = MidiTable::Instance();

    QPoint bgPos = QPoint(-2*ratio, -3*ratio); // Correction needed y-3.
    QPoint knobPos = QPoint(5*ratio, 4*ratio); // Correction needed y+1 x-1.
    QLabel *newBackGround = new QLabel(this);
    if (this->background.contains("target"))
    {
        this->range = midiTable->getRange("Tables", "00", "00", mode_hex);
        this->rangeMin = midiTable->getRangeMinimum("Tables", "00", "00", mode_hex);
    }
    else
    {
        this->range = midiTable->getRange("Tables", "00", "00", hexLsb);
        this->rangeMin = midiTable->getRangeMinimum("Tables", "00", "00", hexLsb);
    };

    newBackGround->setPixmap(QPixmap(":/images/knobbgn.png").scaled(49*ratio,50*ratio, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    newBackGround->move(bgPos);

    QString imagePath(":/images/knob.png");
    unsigned int imageRange = 100;
    this->knob = new customTargetDial(0, rangeMin, range, 1, 10, knobPos, this, hex0, hex1, hex2, hex3, imagePath, imageRange, background);
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    this->setFixedSize(newBackGround->pixmap(Qt::ReturnByValue).size() - QSize(0, 4)*ratio);
#else
    this->setFixedSize(newBackGround->pixmap(Qt::ReturnByValue).size() - QSize(0, 4)*ratio); // Correction needed h-4 - space between knob and bottom label.
#endif


    QObject::connect(this, SIGNAL( updateSignal() ),
                     this->parent(), SIGNAL( updateSignal() ));

    QObject::connect(this, SIGNAL( updateDisplayTarget(QString) ),
                     this->parent(), SIGNAL( updateDisplayTarget(QString) ));

    QObject::connect(this, SIGNAL( updateDisplayMin(QString) ),
                     this->parent(), SIGNAL( updateDisplayMin(QString) ));

    QObject::connect(this, SIGNAL( updateDisplayMax(QString) ),
                     this->parent(), SIGNAL( updateDisplayMax(QString) ));

    QObject::connect(this, SIGNAL( updateTarget(QString, QString, QString, QString) ),
                     this->parent(), SIGNAL( updateTarget(QString, QString, QString, QString) ));

    QObject::connect(this, SIGNAL( updateHex(QString, QString, QString, QString) ),
                     this, SLOT( knobSignal(QString, QString, QString, QString) ));

    QObject::connect(this->parent(), SIGNAL( updateHex(QString, QString, QString, QString) ),
                     this, SIGNAL( updateHex(QString, QString, QString, QString) ));

    QObject::connect(this, SIGNAL( highlightSignal(QString) ),
                     this->parent(), SLOT( highlight(QString) ));

}

void customKnobTarget::paintEvent(QPaintEvent *)
{
    /*DRAWS RED BACKGROUND FOR DEBUGGING PURPOSE */
    /*QPixmap image(":/images/dragbar.png");

    QRectF target(0.0, 0.0, this->width(), this->height());
    QRectF source(0.0, 0.0, this->width(), this->height());

    QPainter painter(this);
    painter.drawPixmap(target, image, source);*/
}

void customKnobTarget::knobSignal( QString hex0, QString hex1,QString hexMsb, QString hexLsb)
{
    Q_UNUSED(hex0);
    Q_UNUSED(hex1);
    if (!background.contains("target"))
    {
        this->hexMsb = hexMsb;
        this->hexLsb = hexLsb;
    };
}

void customKnobTarget::setValue(int value)
{
    this->knob->setValue(value);     // on initialisation only
}

void customKnobTarget::valueChanged(int value, QString hex0, QString hex1, QString hex2, QString hex3)
{
    MidiTable *midiTable = MidiTable::Instance();

    QString valueHex = QString::number(value, 16).toUpper();
    if(valueHex.length() < 2) { valueHex.prepend("000"); }
    else if(valueHex.length() < 3) { valueHex.prepend("00"); }
    else if(valueHex.length() < 4) { valueHex.prepend("0"); };

    QList<QString> valueString;
    valueString.append("0");
    valueString.append(valueHex.at(0));
    valueString.append("0");
    valueString.append(valueHex.at(1));
    valueString.append("0");
    valueString.append(valueHex.at(2));
    valueString.append("0");
    valueString.append(valueHex.at(3));

    SysxIO *sysxIO = SysxIO::Instance();
    sysxIO->setFileSource(hex0, hex1, hex2, hex3, valueString);

    QString valueStr;
    if (this->background.contains("target")) {
        valueStr = midiTable->getValue("Tables", "00", "00", "00", valueHex);
        emit updateDisplayTarget(valueStr);                                          // updates display values
    } else if (this->background.contains("min")) {
        valueStr = midiTable->getValue("Tables", "00", "00", hexLsb, valueHex);
        emit updateDisplayMin(valueStr);
    } else if (this->background.contains("max")) {
        valueStr = midiTable->getValue("Tables", "00", "00", hexLsb, valueHex);
        emit updateDisplayMax(valueStr);
    };                                                  // updates display values


    //******* DEbug ****************
    /* QString m = " knob valueHex = "+valueHex+" valueStr = "+valueStr;
     std::string st = m.toStdString();
     std::cerr << st << std::endl;*/

    if (this->background.contains("target"))                                                   // get the currently selected target value & set min/max address
    {
        bool ok;
        QString hex3_msb = QString::number((hex3.toInt(&ok, 16) + 1), 16).toUpper();    // go forward 1 to select target MSB address
        if(hex3_msb.length() < 2) hex3_msb.prepend("0");                                // prepend with "0" if single digit.

        QString hex3_lsb = QString::number((hex3.toInt(&ok, 16) + 2), 16).toUpper();    // go forward 2 to select target LSB address
        if(hex3_lsb.length() < 2) hex3_lsb.prepend("0");

        int value = sysxIO->getSourceValue(this->hex0, this->hex1, this->hex2, this->hex3);        // read target value as integer from sysx.
        QString valueHex = QString::number(value, 16).toUpper();                        // convert to hex qstring.
        value = sysxIO->getSourceValue(this->hex0, this->hex1, this->hex2, hex3_msb);        // read target value as integer from sysx.
        valueHex.append(QString::number(value, 16).toUpper());
        value = sysxIO->getSourceValue(this->hex0, this->hex1, this->hex2, hex3_lsb);        // read target value as integer from sysx.
        valueHex.append(QString::number(value, 16).toUpper());

        valueStr = midiTable->getValue("Tables", "00", "00", "00", valueHex);           // lookup the target values

        int maxRange = 256;
        value = valueHex.toInt(&ok, 16);
        int dif = value/maxRange;
        QString valueHex1 = QString::number(dif, 16).toUpper();
        if(valueHex1.length() < 2) valueHex1.prepend("0");
        QString valueHex2 = QString::number(value - (dif * maxRange), 16).toUpper();
        if(valueHex2.length() < 2) valueHex2.prepend("0");
        QString hex4 = valueHex1;
        QString hex5 = valueHex2;                                                       //convert valueStr to 7bit hex4, hex5
        Midi items = midiTable->getMidiMap("Tables", "00", "00", "00", hex4, hex5);
        this->hexMsb = items.desc;
        this->hexLsb = items.customdesc;

        emit updateTarget(hex0, hex1, hexMsb, hexLsb);                                        // hexMsb & hexLsb are lookup address for label value
    };
                                                     // updates on knob value change
}

