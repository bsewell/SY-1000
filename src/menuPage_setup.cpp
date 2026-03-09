/****************************************************************************
**
** Copyright (C) 2007~2020 Colin Willcocks.
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

#include "menuPage_setup.h"
#include "Preferences.h"
#include "SysxIO.h"

menuPage_setup::menuPage_setup(QWidget *parent)
    : menuPage(parent)
{
    SysxIO *sysxIO = SysxIO::Instance();
    sysxIO->deBug("build setup menu");

    setLSB("00", "05");
  //  editDetails()->patchPos(404, 38, "01", "32");    //sysx file offset and data length of parameters x 2.
    setEditPages();
}

void menuPage_setup::updateSignal()
{

}

void menuPage_setup::setEditPages()
{
    QString hex1 = "00";
    QString hex2A = "20";
    QString hex2B = "2B";
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){hex1 = "02"; hex2A = "1F"; hex2B = "29";};

    editDetails()->page()->newGroupBox(tr("Input Levels"));
    editDetails()->page()->newStackControl(0);
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, "12", "00", "icon_large_40");  // gk/normal patch type
    editDetails()->page()->addStackControl();
    editDetails()->page()->addSystemOverride(1, 1, 1, 1, "10", hex1, "12", "00", "00", "command_equal", "10", hex1, "15", "01", "00" ); //control Synth 1 to dynamic
    editDetails()->page()->addSystemOverride(1, 1, 1, 1, "10", hex1, "12", "00", "00", "command_equal", "10", hex1, hex2A, "01", "00" ); //control Synth 2 to dynamic
    editDetails()->page()->addSystemOverride(1, 1, 1, 1, "10", hex1, "12", "00", "00", "command_equal", "10", hex1, hex2B, "01", "00" ); //control Synth 3 to dynamic
    editDetails()->page()->insertStackField(0, 1, 0, 1, 1);
    editDetails()->page()->addGroupBox(0, 0, 1, 1);

    editDetails()->page()->newStackField(0);
    editDetails()->page()->newGroupBox(tr("GK Input Levels"));
    editDetails()->page()->addVU(0, 0, 1, 1, "10", hex1, "12", "25", "command", "6_band");
    editDetails()->page()->addGroupBox(0, 0, 1, 1);
    editDetails()->page()->addStackField();
    editDetails()->page()->newStackField(0);
    editDetails()->page()->newGroupBox(tr("Normal Input Levels"));
    editDetails()->page()->addVU(0, 0, 1, 1, "10", hex1, "12", "25", "command", "54_band");
    editDetails()->page()->addGroupBox(0, 0, 1, 1);
    editDetails()->page()->addStackField();

    editDetails()->addPage();
}
