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

#include "customKnob.h"
#include "MidiTable.h"
#include "SysxIO.h"
#include "Preferences.h"

customKnob::customKnob(QWidget *parent, QString hex0, QString hex1, QString hex2, QString hex3, QString background, QString direction) : QWidget(parent)
{
    bool ok;
    // UI rule: all knobs must render at one uniform (small) size.
    // Ignore legacy *_ratioX hints embedded in background strings.
    const double multiplier = 1.0;
    Preferences *preferences = Preferences::Instance();
    double ratio = preferences->getPreferences("Window", "Scale", "ratio").toDouble(&ok)*multiplier;
    colour = preferences->getPreferences("Scheme", "Colour", "select");

    this->hex0 = hex0;
    this->hex1 = hex1;
    this->hex2 = hex2;
    this->hex3 = hex3;
    this->background = background;
    this->area = direction;
    MidiTable *midiTable = MidiTable::Instance();
    int range = midiTable->getRange(hex0, hex1, hex2, hex3);
    int rangeMin = midiTable->getRangeMinimum(hex0, hex1, hex2, hex3);

    QPoint bgPos = QPoint(0, -3)*ratio; // Correction needed.
    QPoint knobPos = QPoint(4, 2)*ratio; // Correction needed.

    QLabel *newBackGround = new QLabel(this);
    if(colour.contains("0"))
    {
        knobPos = QPoint(-3, -2)*ratio;
        newBackGround->setPixmap(QPixmap(":/images/knobbg_inv.png").scaled(50*ratio,50*ratio, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    else if (background.contains("normal") || background.contains("System") || background.contains("inverse"))
    {
        newBackGround->setPixmap(QPixmap(":/images/knobbgn.png").scaled(50*ratio,50*ratio, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    else if (background.contains("turbo"))
    {
        newBackGround->setPixmap(QPixmap(":/images/knobbgt.png").scaled(50*ratio,50*ratio, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    else if (background.contains("geq_slider"))
    {
        knobPos = QPoint(1*ratio, 1*ratio);
        newBackGround->setPixmap(QPixmap(":/images/geq_slider_bg.png").scaled(1*ratio, 1*ratio, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    /* else if (background.contains("slider"))
    {
        newBackGround->setPixmap(QPixmap(":/images/slider_knobbg.png").scaled(49*ratio,50*ratio, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }*/
    else
    {
        newBackGround->setPixmap(QPixmap(":/images/knobbg.png").scaled(50*ratio,50*ratio, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    };
    newBackGround->move(bgPos);
    //QString imagePath_slider(":/images/slider_knob.png");
    QString imagePath_knob(":/images/knob.png");
    QString imagePath_geq_slider(":/images/geq_slider_knob.png");
    QString imagePath_geq_bg(":/images/geq_slider_bg.png");

    unsigned int imageRange = 100;
    if (colour == "0")
    {
        imagePath_knob = ":/images/knob_bts_aqua.png";
        this->knob = new customDial(0, rangeMin, range, 1, 10, knobPos, this, hex0, hex1, hex2, hex3, imagePath_knob, imageRange, multiplier*1.17);
    }
    else if (background.contains("geq_slider"))
    {
        this->slide = new customSlider(0, rangeMin, range, 1, 10, knobPos, this, hex1, hex2, hex3, imagePath_geq_bg, imagePath_geq_slider);
    }
    /*else if (background.contains("slider"))
    {
        this->knob = new customDial(0, rangeMin, range, 1, 10, knobPos, this, hex0, hex1, hex2, hex3, imagePath_slider, imageRange);
    }*/
    else
    {
        this->knob = new customDial(0, rangeMin, range, 1, 10, knobPos, this, hex0, hex1, hex2, hex3, imagePath_knob, imageRange, multiplier*1.17);
    };

    this->setFixedSize(newBackGround->pixmap(Qt::ReturnByValue).size() - QSize(0, 4)*ratio); // Correction needed h-4 - space between knob and bottom label.


    QObject::connect(this, SIGNAL(updateSignal()),
                     this->parent(), SIGNAL(updateSignal()));

    QObject::connect(this, SIGNAL(updateDisplay(QString)),
                     this->parent(), SIGNAL(updateDisplay(QString)));

    QObject::connect(this, SIGNAL(highlightSignal()),
                     this->parent(), SLOT(highlight()));

    /*QObject::connect(this, SIGNAL(updateSignal()),
                     this->parent(), SLOT(knobUpdate()));*/
}

void customKnob::paintEvent(QPaintEvent *)
{
    /*DRAWS RED BACKGROUND FOR DEBUGGING PURPOSE */
    /*QPixmap image(":/images/dragbar.png");

    QRectF target(0.0, 0.0, this->width(), this->height());
    QRectF source(0.0, 0.0, this->width(), this->height());

    QPainter painter(this);
    painter.drawPixmap(target, image, source); */
}

void customKnob::setValue(int value)
{
    if (background.contains("geq_slider")){ this->slide->setValue(value); }
    else{ this->knob->setValue(value); };
}

void customKnob::valueChanged(int value, QString hex0, QString hex1, QString hex2, QString hex3)
{
    MidiTable *midiTable = MidiTable::Instance();
    if(background.contains("inverse")){value =  int (midiTable->getRange(hex0, hex1, hex2, hex3))+(midiTable->getRangeMinimum(hex0, hex1, hex2, hex3))-value;};

    QString valueHex = QString::number(value, 16).toUpper();
    if(valueHex.length() < 2) valueHex.prepend("0");

    SysxIO *sysxIO = SysxIO::Instance(); bool ok;
    if(midiTable->isData(hex0, hex1, hex2, hex3))
    {
        int maxRange = QString("7F").toInt(&ok, 16) + 1;
        int value = valueHex.toInt(&ok, 16);
        int dif = value/maxRange;
        QString valueHex1 = QString::number(dif, 16).toUpper();
        if(valueHex1.length() < 2) valueHex1.prepend("0");
        QString valueHex2 = QString::number(value - (dif * maxRange), 16).toUpper();
        if(valueHex2.length() < 2) valueHex2.prepend("0");

        sysxIO->setFileSource(hex0, hex1, hex2, hex3, valueHex1, valueHex2);
    }
    else
    {
        sysxIO->setFileSource(hex0, hex1, hex2, hex3, valueHex);
    };

    QString valueStr = midiTable->getValue(hex0, hex1, hex2, hex3, valueHex);
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
    emit updateSignal();
}
