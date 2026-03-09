/****************************************************************************
**
** Copyright (C) 2007~2025 Colin Willcocks.
** Copyright (C) 2005~2007 Uco Mesdag. 
** All rights reserved.
** This file is part of "VG-99 Fx FloorBoard".
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

#include "stompbox_ds.h"
#include "Preferences.h"
#include "SysxIO.h"

stompbox_ds::stompbox_ds(QWidget *parent)
    : stompBox(parent)
{
    SysxIO *sysxIO = SysxIO::Instance();
    sysxIO->deBug("build Distortion stomp");

    QString hex1 = "00";
    QString hex2 = "37";
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){hex1 = "02"; hex2 = "34";};

    /* OD/DS */
    setOnImage(":/images/ds_on.png");
    setOffImage(":/images/ds_off.png");
    setHoverImage(":/images/effect_hover.png");
    setSelectedImage(":/images/effect_select.png");
    setLSB(hex1, hex2);
    brushColor("yellow");
    setButton("10", hex1, hex2, "00");
    //editDetails()->patchPos(108, 30, "00", "30");
    setEditPages();
}

void stompbox_ds::updateSignal()
{
    QString hex1 = "00";
    QString hex2 = "37";
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){hex1 = "02"; hex2 = "34";};

    updateButton("10", hex1, hex2, "00");
    updateLabel("10", hex1, hex2, "01");
}

void stompbox_ds::setEditPages()
{
    QString hex1 = "00";
    QString hex2 = "37";
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){hex1 = "02"; hex2 = "34";};

    editDetails()->page()->newGroupBox("Effect");
    editDetails()->page()->addSwitch(0, 0, 1, 1, "10", hex1, hex2, "00");
    editDetails()->page()->addComboBox(1, 0, 1, 1, "10", hex1, hex2, "01", "large");
    editDetails()->page()->addGroupBox(0, 0, 1, 1);

    editDetails()->page()->newGroupBox("Drive");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "02", "turbo_ratio1.5");
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "03", "normal_ratio1.25");
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "05", "normal_ratio1.25");
    editDetails()->page()->addGroupBox(0, 1, 1, 1);

    editDetails()->page()->newGroupBox("Level");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "04", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.75");
    editDetails()->page()->addGroupBox(0, 2, 1, 1);

    editDetails()->page()->newGroupBox("Solo");
    editDetails()->page()->addSwitch(0, 0, 1, 1, "10", hex1, hex2, "07");
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "08", "normal_ratio1.25");
    editDetails()->page()->addGroupBox(0, 3, 1, 1);

    editDetails()->addPage();
}
