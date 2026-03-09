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

#include "menuPage_assign9.h"
#include "Preferences.h"

#include <iostream>

menuPage_assign9::menuPage_assign9(QWidget *parent)
    : menuPage(parent)
{
    std::string st = QString("build Assign9 menu").toStdString();
    std::cerr << st << std::endl;
    setLSB("00", "08");
   // editDetails()->patchPos(328, 38, "01", "0C");    //sysx file offset and data length of parameters x2,
    setEditPages();
}

void menuPage_assign9::updateSignal()
{
    QString hex1 = "00";
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){hex1 = "02"; };

    updateSwitch("10", hex1, "08", "00");
}

void menuPage_assign9::setEditPages()
{
    QString hex1 = "00";
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){hex1 = "02"; };

    editDetails()->page()->newGroupBox(tr("Assign 9"));
    editDetails()->page()->newGroupBox(tr(""));
    editDetails()->page()->addSwitch(0, 0, 1, 1, "10", hex1, "08", "00", "middle", Qt::AlignCenter);
    editDetails()->page()->addGroupBox(0, 0, 1, 1);
    editDetails()->page()->newGroupBox("Assign 9 Target");
    editDetails()->page()->addTarget(0, 0, 1, 5, "10", hex1, "08", "01", "target","target", 140); // Target
    editDetails()->page()->addGroupBox(0, 1, 1, 3);
    editDetails()->page()->newGroupBox("Assign 9 Source");
    editDetails()->page()->newStackControl(0);
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, "08", "0D");                    // Source
    editDetails()->page()->addStackControl();
    editDetails()->page()->addComboBox(0, 1, 1, 1, "10", hex1, "08", "0E");                    // Source Mode
    editDetails()->page()->addRange(0, 2, 1, 2, "10", hex1, "08", "0F", "Structure");         // min/max range widget
    editDetails()->page()->addGroupBox(1, 0, 1, 3);
    editDetails()->page()->insertStackField(0, 1, 3, 1, 1);
    editDetails()->page()->addGroupBox(0, 0, 2, 3);

    editDetails()->addPage();

    for(int x=0; x<13; x++){    // better than pasting the following 13 times.
    editDetails()->page()->newStackField(0);
    editDetails()->page()->newGroupBox("Switch timing");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, "08", "17");   // rise time
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, "08", "18");   // fall time
    editDetails()->page()->addGroupBox(0, 0, 1, 1);
    editDetails()->page()->addStackField();
    };
    editDetails()->page()->newStackField(0);editDetails()->page()->addStackField();
    editDetails()->page()->newStackField(0);editDetails()->page()->addStackField();
    editDetails()->page()->newStackField(0);
    editDetails()->page()->newGroupBox("Switch timing");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, "08", "17");   // rise time
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, "08", "18");   // fall time
    editDetails()->page()->addGroupBox(0, 0, 1, 1);
    editDetails()->page()->addStackField();
    editDetails()->page()->newStackField(0);
    editDetails()->page()->newGroupBox("Switch timing");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, "08", "17");   // rise time
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, "08", "18");   // fall time
    editDetails()->page()->addGroupBox(0, 0, 1, 1);
    editDetails()->page()->addStackField();    
    editDetails()->page()->newStackField(0);editDetails()->page()->addStackField();
    editDetails()->page()->newStackField(0);
    editDetails()->page()->newGroupBox("Assign 9 Wave Pedal");
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, "08", "19", "normal", "bottom", 70);// Wave Form Rate
    editDetails()->page()->addComboBox(0, 3, 1, 1, "10", hex1, "08", "1A");                    // Wave Form
    editDetails()->page()->addGroupBox(0, 0, 1, 1);
    editDetails()->page()->addStackField();
    for(int x=0; x<8; x++){    // paste input level sense 8x
    editDetails()->page()->newStackField(0);
    editDetails()->page()->newGroupBox("Assign Common Input Level");
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, "12", "44");   // input sense
    editDetails()->page()->addGroupBox(0, 0, 1, 1);
    editDetails()->page()->addStackField();
    };
    for(int x=0; x<94; x++){    // better than pasting the following 95 times.
    editDetails()->page()->newStackField(0);
    editDetails()->page()->newGroupBox("Assign 9 Midi CC#");
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, "08", "1B");                    //midi ch
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, "08", "1C");   // midi cc#
    editDetails()->page()->addDataKnob(0, 2, 1, 1, "10", hex1, "08", "1D", "ASSIGN", "bottom", 60);   // cc# min
    //editDetails()->page()->addDataKnob(0, 3, 1, 1, "10", hex1, "08", "21", "ASSIGN", "bottom", 60);   // cc# max
    editDetails()->page()->addGroupBox(0, 0, 1, 1);
    editDetails()->page()->addStackField();
};

}
