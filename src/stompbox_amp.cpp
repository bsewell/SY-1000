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

#include "stompbox_amp.h"
#include "Preferences.h"
#include "SysxIO.h"

stompbox_amp::stompbox_amp(QWidget *parent)
    : stompBox(parent)
{
    SysxIO *sysxIO = SysxIO::Instance();
    sysxIO->deBug("build Amp stomp");

    QString hex1 = "00";
    QString hex2 = "38";
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){hex1 = "02"; hex2 = "35";};
    /* AMP */
    setOnImage(":/images/amp_on.png");
    setOffImage(":/images/amp_off.png");
    setHoverImage(":/images/effect_hover.png");
    setSelectedImage(":/images/effect_select.png");
    setLSB(hex1, "38");
    brushColor("yellow");
    setButton("10", hex1, hex2, "00");
   // editDetails()->patchPos(1480, 34, "07", "00");
    setEditPages();
}

void stompbox_amp::updateSignal()
{
    QString hex1 = "00";
    QString hex2 = "38";
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){hex1 = "02"; hex2 = "35";};
   updateButton("10", hex1, hex2, "00");
   updateLabel("10", hex1, hex2, "01");
}

void stompbox_amp::setEditPages()
{
    QString hex1 = "00";
    QString hex2 = "38";
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){hex1 = "02"; hex2 = "35";};

    editDetails()->page()->addSwitch(0, 0, 1, 1, "10", hex1, hex2, "00");   // off/on

    editDetails()->page()->newStackControl(0);
    editDetails()->page()->addComboBox(0, 1, 1, 7, "10", hex1, hex2, "01", "large");         // pre type (spans cols 1-7)
    editDetails()->page()->addStackControl();
    editDetails()->page()->addComboBox(1, 1, 1, 1, "10", hex1, hex2, "0B", "bottom");        // gain sw
    editDetails()->page()->addKnob(1, 2, 1, 1, "10", hex1, hex2, "02", "turbo_ratio1.25");   // gain
    editDetails()->page()->addKnob(1, 3, 1, 1, "10", hex1, hex2, "03", "normal_ratio1.25");  // sag
    editDetails()->page()->addKnob(1, 4, 1, 1, "10", hex1, hex2, "04", "normal_ratio1.25");  // resonance
    editDetails()->page()->addKnob(1, 5, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.25");  // bass
    editDetails()->page()->addKnob(1, 6, 1, 1, "10", hex1, hex2, "07", "normal_ratio1.25");  // mid
    editDetails()->page()->addKnob(1, 7, 1, 1, "10", hex1, hex2, "08", "normal_ratio1.25");  // treble
    editDetails()->page()->addKnob(1, 8, 1, 1, "10", hex1, hex2, "09", "normal_ratio1.25");  // presence

    editDetails()->page()->addKnob(0, 9, 1, 1, "10", hex1, hex2, "05", "normal_ratio1.75");  // effect level
    editDetails()->page()->addSwitch(1, 9, 1, 1, "10", hex1, hex2, "0A");                    // bright

    editDetails()->page()->addSwitch(0, 10, 1, 1, "10", hex1, hex2, "0C", "middle");         // solo sw
    editDetails()->page()->addKnob(1, 10, 1, 1, "10", hex1, hex2, "0D", "normal_ratio1.25"); // solo level

    editDetails()->page()->addComboBox(2, 1, 1, 1, "10", hex1, hex2, "0E", "large");         // spkr type
    editDetails()->page()->addComboBox(2, 2, 1, 1, "10", hex1, hex2, "0F", "bottom");        // mic type
    editDetails()->page()->addComboBox(2, 3, 1, 1, "10", hex1, hex2, "10");                  // mic distance
    editDetails()->page()->addKnob(2, 4, 1, 1, "10", hex1, hex2, "11", "normal_ratio1.25", "bottom", 70); // mic position
    editDetails()->page()->addKnob(2, 5, 1, 1, "10", hex1, hex2, "12", "normal_ratio1.5");               // mic level
    editDetails()->page()->addKnob(2, 6, 1, 1, "10", hex1, hex2, "13", "normal_ratio1.5");               // direct level

    editDetails()->addPage();
}
