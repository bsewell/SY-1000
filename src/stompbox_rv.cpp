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

#include "stompbox_rv.h"
#include "Preferences.h"
#include "SysxIO.h"

stompbox_rv::stompbox_rv(QWidget *parent)
    : stompBox(parent)
{
    SysxIO *sysxIO = SysxIO::Instance();
    sysxIO->deBug("build Reverb stomp");

    QString hex1 = "01";
    QString hex2 = "32";
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){hex1 = "03"; hex2 = "2F";};

    /* REVERB */
    setOnImage(":/images/rv_on.png");
    setOffImage(":/images/rv_off.png");
    setHoverImage(":/images/effect_hover.png");
    setSelectedImage(":/images/effect_select.png");
    setLSB(hex1, hex2);
    brushColor("cyan");
    setButton("10", hex1, hex2, "00");
   /*editDetails()->patchPos(1418, 10, "06", "0C"); */
    setEditPages();
}

void stompbox_rv::updateSignal()
{
    QString hex1 = "01";
    QString hex2 = "32";
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){hex1 = "03"; hex2 = "2F";};

  updateButton("10", hex1, hex2, "00");
  updateLabel("10", hex1, hex2, "01");
}

void stompbox_rv::setEditPages()
{
    QString hex1 = "01";
    QString hex2 = "32";
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){hex1 = "03"; hex2 = "2F";};

    editDetails()->page()->addSwitch(0, 0, 1, 1, "10", hex1, hex2, "00"); //sw
    editDetails()->page()->addComboBox(1, 0, 1, 1, "10", hex1, hex2, "01", "large");   //type
    editDetails()->page()->addComboBox(2, 0, 1, 1, "10", hex1, hex2, "03");   //carry over
    editDetails()->page()->addDataKnob(0, 2, 1, 1, "10", hex1, hex2, "04", "0~500_ratio1.25");  //predelay
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.75");  //time
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "0B", "normal_ratio1.25");  //density
    editDetails()->page()->addKnob(0, 5, 1, 1, "10", hex1, hex2, "0D", "normal_ratio1.25");  //spring sense
    editDetails()->page()->addSystemOverride(0, 5, 1, 1, "10", hex1, hex2, "01", "05", "not_equals");
    editDetails()->page()->addKnob(0, 6, 1, 1, "10", hex1, hex2, "09", "normal_ratio1.25", "bottom", 60);  //low cut
    editDetails()->page()->addKnob(0, 7, 1, 1, "10", hex1, hex2, "0A", "normal_ratio1.25", "bottom", 60);  //high cut
    editDetails()->page()->addKnob(0, 8, 1, 1, "10", hex1, hex2, "08", "normal_ratio1.75");  //effect level
    editDetails()->page()->addKnob(0, 9, 1, 1, "10", hex1, hex2, "0C", "normal_ratio1.5");  //direct level


    editDetails()->addPage();

}
