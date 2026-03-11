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

#include "stompbox_lp.h"
#include "Preferences.h"
#include "SysxIO.h"

stompbox_lp::stompbox_lp(QWidget *parent)
    : stompBox(parent)
{
    SysxIO *sysxIO = SysxIO::Instance();
    sysxIO->deBug("build S/R Loop stomp");

    QString hex1 = "00";
    QString hex2 = "12";
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){hex1 = "02"; hex2 = "12";};

    /* LOOP */
    setOnImage(":/images/lp_on.png");
    setOffImage(":/images/lp_off.png");
    setHoverImage(":/images/effect_hover.png");
    setSelectedImage(":/images/effect_select.png");
    setLSB(hex1, hex2);
    brushColor("green");
    setButton("10", hex1, "12", "2E");
    //editDetails()->patchPos(1884, 8, "06", "55");
    setEditPages();
}

void stompbox_lp::updateSignal()
{
    QString hex1 = "00";
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){hex1 = "02";};

    updateButton("10", hex1, "12", "2E");
}

void stompbox_lp::setEditPages()
{
    QString hex1 = "00";
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){hex1 = "02";};

    // formerly "Effect" groupBox at addGroupBox(0, 0, 1, 1) — parentCol=0
    editDetails()->page()->addSwitch(0, 0, 1, 1, "10", hex1, "12", "2E"); //on/off
    editDetails()->page()->newStackControl(0);
    editDetails()->page()->addComboBox(1, 0, 1, 1, "10", hex1, "12", "2F", "large"); //mode
    editDetails()->page()->addStackControl();

    // Send level — always visible
    editDetails()->page()->addDataKnob(0, 1, 1, 1, "10", hex1, "12", "30", "0~200_ratio1.5"); //send
    // Return level + Adjust — disabled when Mode = OUT (value 2)
    editDetails()->page()->addDataKnob(0, 2, 1, 1, "10", hex1, "12", "32", "0~200_ratio1.5"); //return
    editDetails()->page()->addSystemOverride(0, 2, 1, 1, "10", hex1, "12", "2F", "02", "equals"); //hide return on OUT
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, "12", "34", "normal_ratio1.5"); //adjust
    editDetails()->page()->addSystemOverride(0, 3, 1, 1, "10", hex1, "12", "2F", "02", "equals"); //hide adjust on OUT
    editDetails()->page()->addComboBox(0, 4, 1, 1, "10", hex1, "12", "35", "large"); //phase

    editDetails()->addPage();
}
