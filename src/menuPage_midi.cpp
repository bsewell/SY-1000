/****************************************************************************
**
** Copyright (C) 2020 Colin Willcocks.
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

#include "menuPage_midi.h"
#include "Preferences.h"

menuPage_midi::menuPage_midi(QWidget *parent)
    : menuPage(parent)
{
    setEditPages();
}

void menuPage_midi::updateSignal()
{
}

void menuPage_midi::setEditPages()
{
    // System MIDI settings block: hex0="00", hex1=sys1, hex2="30"
    // sys1: "01" = guitar system, "08" = bass system
    QString sys1 = "01";
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){sys1 = "08";};

    editDetails()->setQmlPage("qrc:/qml/MidiPanel.qml", sys1, "30");
}
