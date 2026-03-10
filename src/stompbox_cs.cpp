/****************************************************************************
**
** Copyright (C) 2007~2013 Colin Willcocks.
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

#include "stompbox_cs.h"
#include "Preferences.h"
#include "SysxIO.h"

stompbox_cs::stompbox_cs(QWidget *parent)
    : stompBox(parent)
{
    SysxIO *sysxIO = SysxIO::Instance();
    sysxIO->deBug("build Compressor stomp");

    QString hex1 = "00";
    QString hex2 = "36";
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){hex1 = "02"; hex2 = "33";};

	/* COMP */
    setOnImage(":/images/cs_on.png");
    setOffImage(":/images/cs_off.png");
    setHoverImage(":/images/effect_hover.png");
    setSelectedImage(":/images/effect_select.png");
    //setLSB("00", "00");
    brushColor("blue");
    setButton("10", hex1, hex2, "00");
    //editDetails()->patchPos(150, 16, "00", "40");
	setEditPages();
};

void stompbox_cs::updateSignal()
{
    QString hex1 = "00";
    QString hex2 = "36";
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){hex1 = "02"; hex2 = "33";};

    updateButton("10", hex1, hex2, "00");
    updateLabel("10", hex1, hex2, "01");
};

void stompbox_cs::setEditPages()
{
    QString hex1 = "00";
    QString hex2 = "36";
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){hex1 = "02"; hex2 = "33";};

    editDetails()->page()->addSwitch(0, 0, 1, 1, "10", hex1, hex2, "00");
    editDetails()->page()->addComboBox(1, 0, 1, 1, "10", hex1, hex2, "01", "large");
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "03", "normal_ratio1.5"); // attack
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.5"); // sustain
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.5"); // tone
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "04", "normal_ratio2");   // effect level
    editDetails()->page()->addKnob(0, 5, 1, 1, "10", hex1, hex2, "05", "normal_ratio1.75"); // direct level


	editDetails()->addPage();
};
