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

#include "customDataKnob.h"
#include "MidiTable.h"
#include "SysxIO.h"
#include "Preferences.h"

customDataKnob::customDataKnob(QWidget *parent, QString hex0, QString hex1, QString hex2, QString hex3, QString background, QString direction) : QWidget(parent)
{
    // UI rule: all knobs must render at one uniform (small) size.
    // Ignore legacy *_ratioX hints embedded in background strings.
    const double multiplier = 1.0;
    Preferences *preferences = Preferences::Instance();
    bool ok;
    double ratio = preferences->getPreferences("Window", "Scale", "ratio").toDouble(&ok)*multiplier;
    this->colour = preferences->getPreferences("Scheme", "Colour", "select");

    this->hex0 = hex0;
    this->hex1 = hex1;
    this->hex2 = hex2;
    this->hex3 = hex3;
    this->area = direction;
    this->hex_a ="00";
    this->hex_b ="00";
    this->hex_c ="00";
    this->byteSize = "3";
    this->dataRange = 1;
    MidiTable *midiTable = MidiTable::Instance();
    if (!this->area.contains("System")){ this->area = "Structure"; };
    if (background.contains("PREDELAY"))   { this->hex_c = "07"; this->byteSize = "4"; };
    if (background.contains("DELAY2000"))  { this->hex_c = "0D"; this->byteSize = "4"; };
    if (background.contains("DELAY1300"))  { this->hex_c = "0B"; this->byteSize = "3"; };
    if (background.contains("KNOBFUNCT"))  { this->hex_b = "02"; this->hex_c = "07"; this->byteSize = "4"; this->area = "System"; };
    if (background.contains("BPM"))        { this->hex_c = "0A"; this->byteSize = "4"; };
    if (background.contains("0~100"))      { this->hex_c = "07"; this->byteSize = "2"; };
    if (background.contains("0~200"))      { this->hex_c = "06"; this->byteSize = "2"; };
    if (background.contains("0~500"))      { this->hex_c = "08"; this->byteSize = "4"; };
    if (background.contains("0~1000"))     { this->hex_c = "09"; this->byteSize = "4"; };
    if (background.contains("RATE"))       { this->hex_c = "08"; this->byteSize = "2"; };
    if (background.contains("ASSIGN"))     { this->hex_c = "04"; this->byteSize = "4"; };
    if (background.contains("SCALE_G"))    { this->hex_c = "75"; this->byteSize = "2"; this->area = "System"; };
    if (background.contains("SCALE_B"))    { this->hex_c = "76"; this->byteSize = "2"; this->area = "System"; };
    this->background = background;

    int range = midiTable->getRange("Tables", hex_a, hex_b, hex_c);
    int rangeMin = midiTable->getRangeMinimum("Tables", hex_a, hex_b, hex_c);

    QPoint bgPos = QPoint(0, -3)*ratio; // Correction needed.
    QPoint knobPos = QPoint(4, 1)*ratio; // Correction needed.

    QLabel *newBackGround = new QLabel(this);
    if(colour.contains("0"))
    {
        newBackGround->setPixmap(QPixmap(":/images/knobbg_inv.png").scaled(50*ratio,50*ratio, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    else
    {
        newBackGround->setPixmap(QPixmap(":/images/knobbgn.png").scaled(50*ratio,50*ratio, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    };

    newBackGround->move(bgPos);

    QString imagePath_knob(":/images/knob.png");
    if(colour.contains("0")) {imagePath_knob = ":/images/knob_bts_aqua.png";};

    uint imageRange = 100;
    this->knob = new customDial(0, rangeMin, range, 1, 10, knobPos, this, hex0, hex1, hex2, hex3, imagePath_knob, imageRange, multiplier*1.17);  //0.87

    this->setFixedSize(newBackGround->pixmap(Qt::ReturnByValue).size() - QSize(0, 4)*ratio); // Correction needed h-4 - space between knob and bottom label.

    QObject::connect(this, SIGNAL( updateSignal() ),
                     this->parent(), SIGNAL( updateSignal() ));

    QObject::connect(this, SIGNAL( updateDisplay(QString) ),
                     this->parent(), SIGNAL( updateDisplay(QString) ));

    QObject::connect(this, SIGNAL( highlightSignal() ),
                     this->parent(), SLOT( highlight() ));
}

void customDataKnob::paintEvent(QPaintEvent *)
{
    /*DRAWS RED BACKGROUND FOR DEBUGGING PURPOSE */
    /*QPixmap image(":/images/dragbar.png");

	QRectF target(0.0, 0.0, this->width(), this->height());
	QRectF source(0.0, 0.0, this->width(), this->height());

	QPainter painter(this);
	painter.drawPixmap(target, image, source);*/
}

void customDataKnob::setValue(int value)
{
    this->knob->setValue(value);
}

void customDataKnob::valueChanged(int value, QString hex0, QString hex1, QString hex2, QString hex3)
{
    MidiTable *midiTable = MidiTable::Instance();
    int range = midiTable->getRange("Tables", hex_a, hex_b, hex_c);
    if(value>range){value=range;};
    QString valueHex = QString::number(value, 16).toUpper();

    if (this->byteSize == "4") {
        if(valueHex.length() < 2) { valueHex.prepend("000"); }
        else if(valueHex.length() < 3) { valueHex.prepend("00");}
        else if(valueHex.length() < 4) { valueHex.prepend("0"); }; }

    else if (this->byteSize == "3") {
        if(valueHex.length() < 2) { valueHex.prepend("00"); }
        else if(valueHex.length() < 3) { valueHex.prepend("0"); };}

    else { if(valueHex.length() < 2) { valueHex.prepend("0"); }; };  //if 2 bytes


    QList<QString> valueString;
    QString lsb = valueHex.at(0);
    lsb.prepend("0");
    valueString.append(lsb);

    lsb = valueHex.at(1);
    lsb.prepend("0");
    valueString.append(lsb);

    if (this->byteSize == "3")
    {
        lsb = valueHex.at(2);
        lsb.prepend("0");
        valueString.append(lsb);
    };

    if (this->byteSize == "4")
    {
        lsb = valueHex.at(2);
        lsb.prepend("0");
        valueString.append(lsb);
        lsb = valueHex.at(3);
        lsb.prepend("0");
        valueString.append(lsb);
    };

    SysxIO *sysxIO = SysxIO::Instance();
    sysxIO->setFileSource(hex0, hex1, hex2, hex3, valueString);

    QString valueStr = midiTable->getValue("Tables", hex_a, hex_b, hex_c, valueHex);

    if(background.contains("ASSIGN"))
    {
        value=((value*10)/131);
        emit updateDisplay(QString::number(value, 10));
    }
    else
    {
        if(colour.contains("0"))
        {
            if(valueStr.contains(":")){valueStr.insert(valueStr.indexOf(":")+1, "<br>");}
            else if(valueStr.contains("kHz")){valueStr.insert(valueStr.indexOf("kHz")+1, "<br>");}
            else if(valueStr.contains("Hz")){valueStr.insert(valueStr.indexOf("Hz"), "<br>");}
            else if(valueStr.contains("cm")){valueStr.insert(valueStr.indexOf("cm"), "<br>");}
            else if(valueStr.contains("ms")){valueStr.insert(valueStr.indexOf("ms"), "<br>");}
            else if(valueStr.contains("dB")){valueStr.insert(valueStr.indexOf("dB"), "<br>");}
            else if(valueStr.contains("mm")){valueStr.insert(valueStr.indexOf("mm"), "<br>");}
            else if(valueStr.contains("User")){valueStr.insert(valueStr.indexOf("User")+4, "<br>");}
            else if(valueStr.contains("Preset")){valueStr.insert(valueStr.indexOf("Preset")+6, "<br>");}
            else if(valueStr.contains(" BPM")){valueStr.insert(valueStr.indexOf(" BPM"), "<br>");}
            else{};
            valueStr.prepend("<br>");
        };
        emit updateDisplay(valueStr);
    };

    emit updateSignal();
}
