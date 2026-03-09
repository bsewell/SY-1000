/****************************************************************************
**
** Copyright (C) 2007~2020 Colin Willcocks.
** All rights reserved.
** This file is part of "SY-1000B FloorBoard".
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

#include "menuPage_tuner.h"
#include "Preferences.h"
#include "SysxIO.h"

menuPage_tuner::menuPage_tuner(QWidget *parent)
    : menuPage(parent)
{
    SysxIO *sysxIO = SysxIO::Instance();
    sysxIO->deBug("build tuner menu");

    setLSB("00", "04");
   // editDetails()->patchPos(366, 38, "01", "1F");    //sysx file offset and data length of parameters x2,
    setEditPages();
}

void menuPage_tuner::updateSignal()
{

}

void menuPage_tuner::setEditPages()
{
    QString hex1 = "01";
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){hex1 = "08"; };
    editDetails()->page()->newGroupBox(tr("TUNER"));
    editDetails()->page()->newStackControl(0);
    editDetails()->page()->addComboBox(1, 0, 1, 1, "00", hex1, "00", "04", "large"); // tuner type
    editDetails()->page()->addStackControl();
    hex1 = "04";
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){hex1 = "0B";};
    editDetails()->page()->addComboBox(1, 1, 1, 1, "00", hex1, "20", "03", "large"); // ref pitch
    editDetails()->page()->addComboBox(1, 2, 1, 1, "00", hex1, "20", "06", "large"); // output
    editDetails()->page()->insertStackField(0, 0, 0, 1, 3);
    editDetails()->page()->addGroupBox(0, 0, 1, 1);

    hex1 = "00";
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){hex1 = "02"; };

    editDetails()->page()->newStackField(0);
    editDetails()->page()->newGroupBox(tr("POLY TUNER"));
    editDetails()->page()->addVU(0, 0, 1, 1, "10", hex1, "12", "25", "command", "poly_tuner");
    editDetails()->page()->addGroupBox(0, 0, 1, 1);
    editDetails()->page()->addStackField();
    editDetails()->page()->newStackField(0);
    editDetails()->page()->newGroupBox(tr("MONO TUNER"));
    editDetails()->page()->addVU(0, 0, 1, 1, "10", hex1, "12", "25", "command", "mono_tuner");
    editDetails()->page()->addGroupBox(0, 0, 1, 1);
    editDetails()->page()->addStackField();


    editDetails()->addPage();
}
