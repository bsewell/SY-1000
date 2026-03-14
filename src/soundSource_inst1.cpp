/****************************************************************************
**
** FxFloorBoard Project.
** Copyright (C) 2007~2025 Colin Willcocks.
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

#include "soundSource_inst1.h"
#include "Preferences.h"
#include "SysxIO.h"
#include "editWindow.h"

soundsource_inst1::soundsource_inst1(QWidget *parent)
    : stompBox(parent)
{
    SysxIO *sysxIO = SysxIO::Instance();
    sysxIO->deBug("build Instrument1 stomp");

    hex1 = "00";
    hex2 = "15";
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex1 = "02";
        hex2 = "15";
    };

    /* INST 1 */
    setOnImage(":/images/inst1_on.png");
    setOffImage(":/images/inst1_off.png");
    setHoverImage(":/images/inst_hover.png");
    setSelectedImage(":/images/inst_select.png");
    setLSB(hex1, hex2);
    brushColor("red");
    setButton("10", hex1, hex2, "00");
    //editDetails()->patchPos(1756, 454, "10", "00");
    setEditPages();
}

void soundsource_inst1::updateSignal()
{
    QString hex1 = "00";
    QString hex2 = "15";
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex1 = "02";
        hex2 = "15";
    };

    updateButton("10", hex1, hex2, "00");
    updateLabel("10", hex1, "15", "01");
}

void soundsource_inst1::setEditPages()
{
    hex1 = "00";
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex1 = "02";
    };

    editDetails()->setQmlPage("qrc:/qml/InstrumentPanel.qml", hex1);
}
