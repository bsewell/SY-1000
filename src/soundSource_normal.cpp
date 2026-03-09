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

#include "soundSource_normal.h"
#include "Preferences.h"
#include "SysxIO.h"

soundsource_normal::soundsource_normal(QWidget *parent)
    : stompBox(parent)
{
    SysxIO *sysxIO = SysxIO::Instance();
    sysxIO->deBug("build Normal PU stomp");

    QString hex1 = "00";
    QString hex2 = "12";
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){hex1 = "02"; hex2 = "12";};

    /* ANALOG PICKUP */
    setOnImage(":/images/normal_on.png");
    setOffImage(":/images/normal_off.png");
    setHoverImage(":/images/inst_hover.png");
    setSelectedImage(":/images/inst_select.png");
    brushColor("blue");
    setLSB(hex1, hex2);
   // editDetails()->patchPos(686, 4, "02", "32");
    setButton("10", hex1, "12", "01");
    setEditPages();
}

void soundsource_normal::updateSignal()
{
    QString hex1 = "00";
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){hex1 = "02";};

    updateButton("10", hex1, "12", "01");
    // Keep the source descriptor current so the flow text never falls back to the
    // default placeholder ("effect") when the signal chain redraws.
    updateLabel("10", hex1, "12", "05");
}

void soundsource_normal::setEditPages()
{

    QString hex1 = "00";
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){hex1 = "02";};

    editDetails()->page()->newGroupBox(tr("Normal Input"));
    editDetails()->page()->addSwitch(0, 0, 1, 1, "10", hex1, "12", "01", "left");   // off/on effect
    editDetails()->page()->addDataKnob(0, 1, 1, 1, "10", hex1, "12", "02", "0~200_ratio1.75");  //level
    editDetails()->page()->addComboBox(0, 2, 1, 1, "10", hex1, "12", "04", "large");   //cable
    editDetails()->page()->addComboBox(0, 3, 1, 1, "10", hex1, "12", "05", "large");   //phase
    editDetails()->page()->addGroupBox(0, 0, 1, 1);

    editDetails()->addPage();
}
