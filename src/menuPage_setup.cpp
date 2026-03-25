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
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){hex1 = "02";};

    editDetails()->setQmlPage("qrc:/qml/SetupPanel.qml", hex1, "00");
}
