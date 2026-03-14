/****************************************************************************
**
** FxFloorBoard Project.
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

#include "soundSource_inst2.h"
#include "Preferences.h"
#include "SysxIO.h"

soundsource_inst2::soundsource_inst2(QWidget *parent)
    : stompBox(parent)
{
    SysxIO *sysxIO = SysxIO::Instance();
    sysxIO->deBug("build Instrument2 stomp");

    hex1 = "00";
    hex2 = "20";
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){hex1 = "02"; hex2 = "1F";};

    /* INST 1 */
    setOnImage(":/images/inst2_on.png");
    setOffImage(":/images/inst2_off.png");
    setHoverImage(":/images/inst_hover.png");
    setSelectedImage(":/images/inst_select.png");
    setLSB(hex1, hex2);
    brushColor("red");
    setButton("10", hex1, hex2, "00");
    //editDetails()->patchPos(1756, 454, "10", "00");
    setEditPages();
}

void soundsource_inst2::updateSignal()
{
    hex1 = "00";
    hex2 = "20";
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex1 = "02";
        hex2 = "1F";
    };

    updateButton("10", hex1, hex2, "00");
    updateLabel("10", hex1, hex2, "01");
}

void soundsource_inst2::setEditPages()
{
    hex1 = "00";
    hex2 = "20";
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){hex1 = "02"; hex2 = "1F";};
    editDetails()->setQmlPage("qrc:/qml/InstrumentPanel.qml", hex1, hex2);
}

void soundsource_inst2::stringLevels()
{
    QString hexA = "00";
    QString hexB = "20";
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hexA = "02";
        hexB = "1F";
    };
    editDetails()->page()->compactCurrentStackField(20, 24);
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hexA, hexB, "0B", "normal_ratio1.5");  //6
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hexA, hexB, "0A", "normal_ratio1.5");  //5
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hexA, hexB, "09", "normal_ratio1.5");  //4
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hexA, hexB, "08", "normal_ratio1.5");  //3
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hexA, hexB, "07", "normal_ratio1.5");  //2
    editDetails()->page()->addKnob(0, 5, 1, 1, "10", hexA, hexB, "06", "normal_ratio1.5");  //1
    editDetails()->page()->addKnob(1, 0, 1, 1, "10", hexA, hexB, "11", "normal_ratio1.5");  //6
    editDetails()->page()->addKnob(1, 1, 1, 1, "10", hexA, hexB, "10", "normal_ratio1.5");  //5
    editDetails()->page()->addKnob(1, 2, 1, 1, "10", hexA, hexB, "0F", "normal_ratio1.5");  //4
    editDetails()->page()->addKnob(1, 3, 1, 1, "10", hexA, hexB, "0E", "normal_ratio1.5");  //3
    editDetails()->page()->addKnob(1, 4, 1, 1, "10", hexA, hexB, "0D", "normal_ratio1.5");  //2
    editDetails()->page()->addKnob(1, 5, 1, 1, "10", hexA, hexB, "0C", "normal_ratio1.5");  //1
}

void soundsource_inst2::altTuning()
{
    QString hexA = "00";
    QString hexB = "2A";
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hexA = "02";
        hexB = "28";
    };
    editDetails()->page()->compactCurrentStackField(20, 24);
    editDetails()->page()->addComboBox(0, 0, 1, 2, "10", hexA, hexB, "01", "left_large");    // tuning type
    editDetails()->page()->addDivider(1, 0, 1, 10, Qt::AlignTop | Qt::AlignLeft);
    editDetails()->page()->addKnob(2, 0, 1, 1, "10", hexA, hexB, "28", "normal_ratio1.25");  // str bend sw
    editDetails()->page()->addKnob(2, 1, 1, 1, "10", hexA, hexB, "2F", "normal_ratio1.25");  // bend control
    editDetails()->page()->addKnob(2, 2, 1, 1, "10", hexA, hexB, "07", "normal_ratio1.25");  // pitch 6
    editDetails()->page()->addKnob(2, 3, 1, 1, "10", hexA, hexB, "06", "normal_ratio1.25");  // pitch 5
    editDetails()->page()->addKnob(2, 4, 1, 1, "10", hexA, hexB, "05", "normal_ratio1.25");  // pitch 4
    editDetails()->page()->addKnob(2, 5, 1, 1, "10", hexA, hexB, "04", "normal_ratio1.25");  // pitch 3
    editDetails()->page()->addKnob(2, 6, 1, 1, "10", hexA, hexB, "03", "normal_ratio1.25");  // pitch 2
    editDetails()->page()->addKnob(2, 7, 1, 1, "10", hexA, hexB, "02", "normal_ratio1.25");  // pitch 1
    editDetails()->page()->addKnob(3, 2, 1, 1, "10", hexA, hexB, "2E", "normal_ratio1.25");  // bend depth 6
    editDetails()->page()->addKnob(3, 3, 1, 1, "10", hexA, hexB, "2D", "normal_ratio1.25");  // bend depth 5
    editDetails()->page()->addKnob(3, 4, 1, 1, "10", hexA, hexB, "2C", "normal_ratio1.25");  // bend depth 4
    editDetails()->page()->addKnob(3, 5, 1, 1, "10", hexA, hexB, "2B", "normal_ratio1.25");  // bend depth 3
    editDetails()->page()->addKnob(3, 6, 1, 1, "10", hexA, hexB, "2A", "normal_ratio1.25");  // bend depth 2
    editDetails()->page()->addKnob(3, 7, 1, 1, "10", hexA, hexB, "29", "normal_ratio1.25");  // bend depth 1
    editDetails()->page()->addKnob(4, 2, 1, 1, "10", hexA, hexB, "0D", "normal_ratio1.25");  // fine 6
    editDetails()->page()->addKnob(4, 3, 1, 1, "10", hexA, hexB, "0C", "normal_ratio1.25");  // fine 5
    editDetails()->page()->addKnob(4, 4, 1, 1, "10", hexA, hexB, "0B", "normal_ratio1.25");  // fine 4
    editDetails()->page()->addKnob(4, 5, 1, 1, "10", hexA, hexB, "0A", "normal_ratio1.25");  // fine 3
    editDetails()->page()->addKnob(4, 6, 1, 1, "10", hexA, hexB, "09", "normal_ratio1.25");  // fine 2
    editDetails()->page()->addKnob(4, 7, 1, 1, "10", hexA, hexB, "08", "normal_ratio1.25");  // fine 1
}

void soundsource_inst2::twelveString()
{
    QString hexA = "00";
    QString hexB = "2A";
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hexA = "02";
        hexB = "28";
    };
    editDetails()->page()->newGroupBox("12 String");
    editDetails()->page()->addSwitch(0, 0, 1, 2, "10", hexA, hexB, "0E");                    // off/on
    editDetails()->page()->addComboBox(0, 3, 1, 2, "10", hexA, hexB, "0F");                  // 12 str type
    editDetails()->page()->newGroupBox(str6);
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hexA, hexB, "15", "normal_ratio1.25");  // pitch 6
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hexA, hexB, "21", "normal_ratio1.25");  // level 6
    editDetails()->page()->addKnob(1, 0, 1, 1, "10", hexA, hexB, "1B", "normal_ratio1.25");  // fine 6
    editDetails()->page()->addKnob(1, 1, 1, 1, "10", hexA, hexB, "27", "normal_ratio1.25");  // delay 6
    editDetails()->page()->addGroupBox(1, 0, 2, 2);
    editDetails()->page()->newGroupBox(str5);
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hexA, hexB, "14", "normal_ratio1.25");  // pitch 5
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hexA, hexB, "20", "normal_ratio1.25");  // level 5
    editDetails()->page()->addKnob(1, 0, 1, 1, "10", hexA, hexB, "1A", "normal_ratio1.25");  // fine 5
    editDetails()->page()->addKnob(1, 1, 1, 1, "10", hexA, hexB, "26", "normal_ratio1.25");  // delay 5
    editDetails()->page()->addGroupBox(1, 2, 2, 2);
    editDetails()->page()->newGroupBox(str4);
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hexA, hexB, "13", "normal_ratio1.25");  // pitch 4
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hexA, hexB, "1F", "normal_ratio1.25");  // level 4
    editDetails()->page()->addKnob(1, 0, 1, 1, "10", hexA, hexB, "19", "normal_ratio1.25");  // fine 4
    editDetails()->page()->addKnob(1, 1, 1, 1, "10", hexA, hexB, "25", "normal_ratio1.25");  // delay 4
    editDetails()->page()->addGroupBox(1, 4, 2, 2);
    editDetails()->page()->newGroupBox(str3);
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hexA, hexB, "12", "normal_ratio1.25");  // pitch 3
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hexA, hexB, "1E", "normal_ratio1.25");  // level 3
    editDetails()->page()->addKnob(1, 0, 1, 1, "10", hexA, hexB, "18", "normal_ratio1.25");  // fine 3
    editDetails()->page()->addKnob(1, 1, 1, 1, "10", hexA, hexB, "24", "normal_ratio1.25");  // delay 3
    editDetails()->page()->addGroupBox(1, 6, 2, 2);
    editDetails()->page()->newGroupBox(str2);
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hexA, hexB, "11", "normal_ratio1.25");  // pitch 2
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hexA, hexB, "1D", "normal_ratio1.25");  // level 2
    editDetails()->page()->addKnob(1, 0, 1, 1, "10", hexA, hexB, "17", "normal_ratio1.25");  // fine 2
    editDetails()->page()->addKnob(1, 1, 1, 1, "10", hexA, hexB, "23", "normal_ratio1.25");  // delay 2
    editDetails()->page()->addGroupBox(1, 8, 2, 2);
    editDetails()->page()->newGroupBox(str1);
    editDetails()->page()->addKnob(1, 5, 1, 1, "10", hexA, hexB, "10", "normal_ratio1.25");  // pitch 1
    editDetails()->page()->addKnob(1, 11, 1, 1, "10", hexA, hexB, "1C", "normal_ratio1.25"); // level 1
    editDetails()->page()->addKnob(2, 5, 1, 1, "10", hexA, hexB, "16", "normal_ratio1.25");  // fine 1
    editDetails()->page()->addKnob(2, 11, 1, 1, "10", hexA, hexB, "22", "normal_ratio1.25"); // delay 1
    editDetails()->page()->addGroupBox(1, 10, 2, 2);
    editDetails()->page()->addSystemOverride(1, 0, 2, 12, "10", hexA, hexB, "0F", "00");
    editDetails()->page()->addGroupBox(0, 1, 1, 1);
}
