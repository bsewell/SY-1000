/****************************************************************************
**
** Copyright (C) 2007~2025 Colin Willcocks.
** Copyright (C) 2005~2007 Uco Mesdag. 
** All rights reserved.
** This file is part of "VG-99 FloorBoard".
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

#include "stompbox_dd1.h"
#include "Preferences.h"
#include "SysxIO.h"

stompbox_dd1::stompbox_dd1(QWidget *parent)
    : stompBox(parent)
{
    SysxIO *sysxIO = SysxIO::Instance();
    sysxIO->deBug("build Delay1 stomp");

    QString hex1 = "00";
    QString hex2 = "3C";
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){hex1 = "02"; hex2 = "39";};

    /* DELAY */
    setOnImage(":/images/dd1_on.png");
    setOffImage(":/images/dd1_off.png");
    setHoverImage(":/images/effect_hover.png");
    setSelectedImage(":/images/effect_select.png");
    setLSB(hex1, hex2);
    brushColor("white");
    setButton("10", hex1, hex2, "00");
   /* editDetails()->patchPos(1404, 14, "06", "05"); */
    setEditPages();
}

void stompbox_dd1::updateSignal()
{
    QString hex1 = "00";
    QString hex2 = "3C";
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){hex1 = "02"; hex2 = "39";};

   updateButton("10", hex1, hex2, "00");
}

void stompbox_dd1::setEditPages()
{
    QString hex1 = "00";
    QString hex2 = "3C";
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){hex1 = "02"; hex2 = "39";};

    editDetails()->page()->newGroupBox("Effect");
    editDetails()->page()->addSwitch(0, 0, 1, 1, "10", hex1, hex2, "00"); //sw
    editDetails()->page()->addGroupBox(0, 0, 1, 1);

    editDetails()->page()->newGroupBox("Delay");
    editDetails()->page()->addDataKnob(0, 0, 1, 1, "10", hex1, hex2, "01", "DELAY2000_ratio1.75", "bottom", 60); // 4 byte time
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "05", "normal_ratio1.5");  //feedback
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.5", "bottom", 60);  //hi cut
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "09", "normal_ratio1.5"); //carry over
    editDetails()->page()->addGroupBox(0, 1, 1, 1);

    editDetails()->page()->newGroupBox("Level");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "07", "turbo_ratio1.75"); //effect
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "08", "normal_ratio1.5"); //direct
    editDetails()->page()->addGroupBox(0, 2, 1, 1);

    editDetails()->addPage();

}
