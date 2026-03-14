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

    editDetails()->setQmlPage("qrc:/qml/AmpPanel.qml", hex1, hex2);
}
