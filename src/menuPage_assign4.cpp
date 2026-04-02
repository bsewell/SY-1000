/****************************************************************************
**
** Copyright (C) 2007~2020 Colin Willcocks.
** Copyright (C) 2005~2007 Uco Mesdag.
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

#include "menuPage_assign4.h"
#include "Preferences.h"

#include <iostream>

menuPage_assign4::menuPage_assign4(QWidget *parent)
    : menuPage(parent)
{
    std::string st = QString("build Assign4 menu").toStdString();
    std::cerr << st << std::endl;
    setLSB("00", "05");
   // editDetails()->patchPos(442, 38, "01", "45");    //sysx file offset and data length of parameters x2,
    setEditPages();
}

void menuPage_assign4::updateSignal()
{
    QString hex1 = "00";
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){hex1 = "02"; };

    updateSwitch("10", hex1, "05", "40");
}

void menuPage_assign4::setEditPages()
{
    QString hex1 = "00";
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){hex1 = "02"; };

    editDetails()->setQmlPage("qrc:/qml/AssignPanel.qml", hex1, "03");  // assign 4 = index 3
}
