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

#include "stompbox_ns.h"
#include "Preferences.h"
#include "SysxIO.h"

stompbox_ns::stompbox_ns(QWidget *parent)
    : stompBox(parent)
{
    SysxIO *sysxIO = SysxIO::Instance();
    sysxIO->deBug("build Noise Suppressor stomp");

    QString hex1 = "00";
    QString hex2 = "39";
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){hex1 = "02"; hex2 = "36";};

    /* ns */
    setOnImage(":/images/ns_on.png");
    setOffImage(":/images/ns_off.png");
    setHoverImage(":/images/effect_hover.png");
    setSelectedImage(":/images/effect_select.png");
    setLSB(hex1, hex2);
    brushColor("green");
    setButton("10", hex1, hex2, "00");
  /*editDetails()->patchPos(1660, 6, "07", "5A"); */
    setEditPages();
}

void stompbox_ns::updateSignal()
{
    QString hex1 = "00";
    QString hex2 = "39";
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){hex1 = "02"; hex2 = "36";};

  updateButton("10", hex1, hex2, "00");
}

void stompbox_ns::setEditPages()
{
    QString hex1 = "00";
    QString hex2 = "39";
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){hex1 = "02"; hex2 = "36";};

    editDetails()->page()->addSwitch(0, 0, 1, 1, "10", hex1, hex2, "00", "middle");
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "01", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.75");
    editDetails()->page()->addComboBox(0, 3, 1, 1, "10", hex1, hex2, "03", "bottom");
    editDetails()->addPage();

}
