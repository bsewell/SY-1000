/****************************************************************************
**
** Copyright (C) 2007~2025 Colin Willcocks. 
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

#include "stompbox_eq2.h"
#include "Preferences.h"
#include "SysxIO.h"

stompbox_eq2::stompbox_eq2(QWidget *parent)
    : stompBox(parent)
{
    SysxIO *sysxIO = SysxIO::Instance();
    sysxIO->deBug("build EQ2 stomp");

    QString hex1 = "00";
    QString hex2 = "3B";
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){hex1 = "02"; hex2 = "38";};

    /* EQ */
    setOnImage(":/images/eq2_on.png");
    setOffImage(":/images/eq2_off.png");
    setHoverImage(":/images/effect_hover.png");
    setSelectedImage(":/images/effect_select.png");
    setLSB(hex1, hex2);
    brushColor("green");
    setButton("10", hex1, hex2, "00");
   // editDetails()->patchPos(1428, 26, "06", "11");
    setEditPages();
}

void stompbox_eq2::updateSignal()
{
    QString hex1 = "00";
    QString hex2 = "3B";
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){hex1 = "02"; hex2 = "38";};

  updateButton("10", hex1, hex2, "00");
}

void stompbox_eq2::setEditPages()
{
    QString hex1 = "00";
    QString hex2 = "3B";
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){hex1 = "02"; hex2 = "38";};

    editDetails()->page()->addSwitch(0, 0, 1, 1, "10", hex1, hex2, "00");
    editDetails()->page()->addParaEQ(0, 1, 2, 4, "10", hex1, hex2, "01");

    editDetails()->addPage();    
}
