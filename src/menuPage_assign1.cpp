/****************************************************************************
**
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

#include "menuPage_assign1.h"
#include "Preferences.h"
#include "SysxIO.h"

menuPage_assign1::menuPage_assign1(QWidget *parent)
    : menuPage(parent)
{
    SysxIO *sysxIO = SysxIO::Instance();
    sysxIO->deBug("build Assign menu");

    setLSB("00", "04");
   // editDetails()->patchPos(328, 38, "01", "0C");    //sysx file offset and data length of parameters x2,
    setEditPages();
}

void menuPage_assign1::updateSignal()
{
    QString hex1 = "00";
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){hex1 = "02"; };

    updateSwitch("10", hex1, "04", "00");
}

void menuPage_assign1::setEditPages()
{
    QString hex1 = "00";
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){hex1 = "02"; };

    editDetails()->page()->newGroupBox(tr("Assign 1"));
    editDetails()->page()->newGroupBox(tr(""));
    editDetails()->page()->addSwitch(0, 0, 1, 1, "10", hex1, "04", "00", "middle", Qt::AlignCenter);
    editDetails()->page()->addGroupBox(0, 0, 1, 1);
    editDetails()->page()->newGroupBox("Assign 1 Target");
    editDetails()->page()->addTarget(0, 0, 1, 5, "10", hex1, "04", "01", "target","target", 140); // Target
    editDetails()->page()->addGroupBox(0, 1, 1, 7);
    editDetails()->page()->newGroupBox("Assign 1 Source");
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, "04", "0D");                    // Source
    editDetails()->page()->addStackControl();
    editDetails()->page()->addComboBox(0, 1, 1, 1, "10", hex1, "04", "0E");                    // Source Mode
    editDetails()->page()->addRange(0, 2, 1, 2, "10", hex1, "04", "0F", "Structure");         // min/max range widget
    editDetails()->page()->addGroupBox(1, 0, 1, 8);

    editDetails()->page()->newGroupBox("Assign 1 Switch timing");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, "04", "17");   // rise time
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, "04", "18");   // fall time
    editDetails()->page()->addSystemOverride(0, 0, 1, 2, "10", hex1, "04", "0D", "12", "less_than");
    editDetails()->page()->addGroupBox(2, 0, 1, 2);

    editDetails()->page()->newGroupBox("Assign 1 Wave Pedal");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, "04", "19", "normal", "bottom", 70);// Wave Form Rate
    editDetails()->page()->addComboBox(0, 1, 1, 1, "10", hex1, "04", "1A");                    // Wave Form
    editDetails()->page()->addSystemOverride(0, 0, 1, 2, "10", hex1, "04", "0D", "12", "not_equals");
    editDetails()->page()->addGroupBox(2, 2, 1, 2);

    editDetails()->page()->newGroupBox("Assign Common Input Level");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, "12", "44");   // input sense
    editDetails()->page()->addSystemOverride(0, 0, 1, 1, "10", hex1, "04", "0D", "12", "more_than");
    editDetails()->page()->addSystemOverride(0, 0, 1, 1, "10", hex1, "04", "0D", "1B", "less_than");
    editDetails()->page()->addGroupBox(2, 4, 1, 1);

    editDetails()->page()->newGroupBox("Assign 1 Midi CC#");
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, "04", "1B");                    //midi ch
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, "04", "1C");   // midi cc#
    editDetails()->page()->addDataKnob(0, 2, 1, 1, "10", hex1, "04", "1D", "ASSIGN", "bottom", 60);   // cc# min
    //editDetails()->page()->addDataKnob(0, 3, 1, 1, "10", hex1, "04", "21", "ASSIGN", "bottom", 60);   // cc# max
    editDetails()->page()->addSystemOverride(0, 0, 1, 4, "10", hex1, "04", "0D", "1A", "more_than");
    editDetails()->page()->addGroupBox(2, 5, 1, 4);

    editDetails()->page()->addGroupBox(0, 0, 3, 8);

    editDetails()->addPage("Tables", "00", "02", "05", "00");       // Assign 1 ******************************************************************


    editDetails()->page()->newGroupBox(tr("Assign 2"));
    editDetails()->page()->newGroupBox(tr(""));
    editDetails()->page()->addSwitch(0, 0, 1, 1, "10", hex1, "04", "40", "middle", Qt::AlignCenter);
    editDetails()->page()->addGroupBox(0, 0, 1, 1);
    editDetails()->page()->newGroupBox("Assign 2 Target");
    editDetails()->page()->addTarget(0, 0, 1, 5, "10", hex1, "04", "41", "target","target", 140); // Target
    editDetails()->page()->addGroupBox(0, 1, 1, 7);
    editDetails()->page()->newGroupBox("Assign 2 Source");
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, "04", "4D");                    // Source
    editDetails()->page()->addStackControl();
    editDetails()->page()->addComboBox(0, 1, 1, 1, "10", hex1, "04", "4E");                    // Source Mode
    editDetails()->page()->addRange(0, 2, 1, 2, "10", hex1, "04", "4F", "Structure");         // min/max range widget
    editDetails()->page()->addGroupBox(1, 0, 1, 8);

    editDetails()->page()->newGroupBox("Assign 2 Switch timing");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, "04", "57");   // rise time
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, "04", "58");   // fall time
    editDetails()->page()->addSystemOverride(0, 0, 1, 2, "10", hex1, "04", "4D", "12", "less_than");
    editDetails()->page()->addGroupBox(2, 0, 1, 2);

    editDetails()->page()->newGroupBox("Assign 2 Wave Pedal");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, "04", "59", "normal", "bottom", 70);// Wave Form Rate
    editDetails()->page()->addComboBox(0, 1, 1, 1, "10", hex1, "04", "5A");                    // Wave Form
    editDetails()->page()->addSystemOverride(0, 0, 1, 2, "10", hex1, "04", "4D", "12", "not_equals");
    editDetails()->page()->addGroupBox(2, 2, 1, 2);

    editDetails()->page()->newGroupBox("Assign Common Input Level");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, "12", "44");   // input sense
    editDetails()->page()->addSystemOverride(0, 0, 1, 1, "10", hex1, "04", "4D", "12", "more_than");
    editDetails()->page()->addSystemOverride(0, 0, 1, 1, "10", hex1, "04", "4D", "1B", "less_than");
    editDetails()->page()->addGroupBox(2, 4, 1, 1);

    editDetails()->page()->newGroupBox("Assign 2 Midi CC#");
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, "04", "5B");                    //midi ch
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, "04", "5C");   // midi cc#
    editDetails()->page()->addDataKnob(0, 2, 1, 1, "10", hex1, "04", "5D", "ASSIGN", "bottom", 60);   // cc# min
    //editDetails()->page()->addDataKnob(0, 3, 1, 1, "10", hex1, "04", "61", "ASSIGN", "bottom", 60);   // cc# max
    editDetails()->page()->addSystemOverride(0, 0, 1, 4, "10", hex1, "04", "4D", "1A", "more_than");
    editDetails()->page()->addGroupBox(2, 5, 1, 4);

    editDetails()->page()->addGroupBox(0, 0, 3, 8);

    editDetails()->addPage("Tables", "00", "02", "05", "01");       // Assign 2 ******************************************************************


    editDetails()->page()->newGroupBox(tr("Assign 3"));
    editDetails()->page()->newGroupBox(tr(""));
    editDetails()->page()->addSwitch(0, 0, 1, 1, "10", hex1, "05", "00", "middle", Qt::AlignCenter);
    editDetails()->page()->addGroupBox(0, 0, 1, 1);
    editDetails()->page()->newGroupBox("Assign 3 Target");
    editDetails()->page()->addTarget(0, 0, 1, 5, "10", hex1, "05", "01", "target","target", 140); // Target
    editDetails()->page()->addGroupBox(0, 1, 1, 7);
    editDetails()->page()->newGroupBox("Assign 3 Source");
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, "05", "0D");                    // Source
    editDetails()->page()->addStackControl();
    editDetails()->page()->addComboBox(0, 1, 1, 1, "10", hex1, "05", "0E");                    // Source Mode
    editDetails()->page()->addRange(0, 2, 1, 2, "10", hex1, "05", "0F", "Structure");         // min/max range widget
    editDetails()->page()->addGroupBox(1, 0, 1, 8);

    editDetails()->page()->newGroupBox("Assign 3 Switch timing");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, "05", "17");   // rise time
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, "05", "18");   // fall time
    editDetails()->page()->addSystemOverride(0, 0, 1, 2, "10", hex1, "05", "0D", "12", "less_than");
    editDetails()->page()->addGroupBox(2, 0, 1, 2);

    editDetails()->page()->newGroupBox("Assign 3 Wave Pedal");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, "05", "19", "normal", "bottom", 70);// Wave Form Rate
    editDetails()->page()->addComboBox(0, 1, 1, 1, "10", hex1, "05", "1A");                    // Wave Form
    editDetails()->page()->addSystemOverride(0, 0, 1, 2, "10", hex1, "05", "0D", "12", "not_equals");
    editDetails()->page()->addGroupBox(2, 2, 1, 2);

    editDetails()->page()->newGroupBox("Assign Common Input Level");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, "12", "44");   // input sense
    editDetails()->page()->addSystemOverride(0, 0, 1, 1, "10", hex1, "05", "0D", "12", "more_than");
    editDetails()->page()->addSystemOverride(0, 0, 1, 1, "10", hex1, "05", "0D", "1B", "less_than");
    editDetails()->page()->addGroupBox(2, 4, 1, 1);

    editDetails()->page()->newGroupBox("Assign 3 Midi CC#");
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, "05", "1B");                    //midi ch
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, "05", "1C");   // midi cc#
    editDetails()->page()->addDataKnob(0, 2, 1, 1, "10", hex1, "05", "1D", "ASSIGN", "bottom", 60);   // cc# min
    //editDetails()->page()->addDataKnob(0, 3, 1, 1, "10", hex1, "05", "21", "ASSIGN", "bottom", 60);   // cc# max
    editDetails()->page()->addSystemOverride(0, 0, 1, 4, "10", hex1, "05", "0D", "1A", "more_than");
    editDetails()->page()->addGroupBox(2, 5, 1, 4);

    editDetails()->page()->addGroupBox(0, 0, 3, 8);

    editDetails()->addPage("Tables", "00", "02", "05", "02");       // Assign 3 ******************************************************************


    editDetails()->page()->newGroupBox(tr("Assign 4"));
    editDetails()->page()->newGroupBox(tr(""));
    editDetails()->page()->addSwitch(0, 0, 1, 1, "10", hex1, "05", "40", "middle", Qt::AlignCenter);
    editDetails()->page()->addGroupBox(0, 0, 1, 1);
    editDetails()->page()->newGroupBox("Assign 4 Target");
    editDetails()->page()->addTarget(0, 0, 1, 5, "10", hex1, "05", "41", "target","target", 140); // Target
    editDetails()->page()->addGroupBox(0, 1, 1, 7);
    editDetails()->page()->newGroupBox("Assign 4 Source");
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, "05", "4D");                    // Source
    editDetails()->page()->addStackControl();
    editDetails()->page()->addComboBox(0, 1, 1, 1, "10", hex1, "05", "4E");                    // Source Mode
    editDetails()->page()->addRange(0, 2, 1, 2, "10", hex1, "05", "4F", "Structure");         // min/max range widget
    editDetails()->page()->addGroupBox(1, 0, 1, 8);

    editDetails()->page()->newGroupBox("Assign 4 Switch timing");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, "05", "57");   // rise time
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, "05", "58");   // fall time
    editDetails()->page()->addSystemOverride(0, 0, 1, 2, "10", hex1, "05", "4D", "12", "less_than");
    editDetails()->page()->addGroupBox(2, 0, 1, 2);

    editDetails()->page()->newGroupBox("Assign 4 Wave Pedal");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, "05", "59", "normal", "bottom", 70);// Wave Form Rate
    editDetails()->page()->addComboBox(0, 1, 1, 1, "10", hex1, "05", "5A");                    // Wave Form
    editDetails()->page()->addSystemOverride(0, 0, 1, 2, "10", hex1, "05", "4D", "12", "not_equals");
    editDetails()->page()->addGroupBox(2, 2, 1, 2);

    editDetails()->page()->newGroupBox("Assign Common Input Level");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, "12", "44");   // input sense
    editDetails()->page()->addSystemOverride(0, 0, 1, 1, "10", hex1, "05", "4D", "12", "more_than");
    editDetails()->page()->addSystemOverride(0, 0, 1, 1, "10", hex1, "05", "4D", "1B", "less_than");
    editDetails()->page()->addGroupBox(2, 4, 1, 1);

    editDetails()->page()->newGroupBox("Assign 4 Midi CC#");
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, "05", "5B");                    //midi ch
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, "05", "5C");   // midi cc#
    editDetails()->page()->addDataKnob(0, 2, 1, 1, "10", hex1, "05", "5D", "ASSIGN", "bottom", 60);   // cc# min
    //editDetails()->page()->addDataKnob(0, 3, 1, 1, "10", hex1, "05", "61", "ASSIGN", "bottom", 60);   // cc# max
    editDetails()->page()->addSystemOverride(0, 0, 1, 4, "10", hex1, "05", "4D", "1A", "more_than");
    editDetails()->page()->addGroupBox(2, 5, 1, 4);

    editDetails()->page()->addGroupBox(0, 0, 3, 8);

   editDetails()->addPage("Tables", "00", "02", "05", "03");       // Assign 4 ******************************************************************


   editDetails()->page()->newGroupBox(tr("Assign 5"));
   editDetails()->page()->newGroupBox(tr(""));
   editDetails()->page()->addSwitch(0, 0, 1, 1, "10", hex1, "06", "00", "middle", Qt::AlignCenter);
   editDetails()->page()->addGroupBox(0, 0, 1, 1);
   editDetails()->page()->newGroupBox("Assign 5 Target");
   editDetails()->page()->addTarget(0, 0, 1, 5, "10", hex1, "06", "01", "target","target", 140); // Target
   editDetails()->page()->addGroupBox(0, 1, 1, 7);
   editDetails()->page()->newGroupBox("Assign 5 Source");
   editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, "06", "0D");                    // Source
   editDetails()->page()->addStackControl();
   editDetails()->page()->addComboBox(0, 1, 1, 1, "10", hex1, "06", "0E");                    // Source Mode
   editDetails()->page()->addRange(0, 2, 1, 2, "10", hex1, "06", "0F", "Structure");         // min/max range widget
   editDetails()->page()->addGroupBox(1, 0, 1, 8);

   editDetails()->page()->newGroupBox("Assign 5 Switch timing");
   editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, "06", "17");   // rise time
   editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, "06", "18");   // fall time
   editDetails()->page()->addSystemOverride(0, 0, 1, 2, "10", hex1, "06", "0D", "12", "less_than");
   editDetails()->page()->addGroupBox(2, 0, 1, 2);

   editDetails()->page()->newGroupBox("Assign 5 Wave Pedal");
   editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, "06", "19", "normal", "bottom", 70);// Wave Form Rate
   editDetails()->page()->addComboBox(0, 1, 1, 1, "10", hex1, "06", "1A");                    // Wave Form
   editDetails()->page()->addSystemOverride(0, 0, 1, 2, "10", hex1, "06", "0D", "12", "not_equals");
   editDetails()->page()->addGroupBox(2, 2, 1, 2);

   editDetails()->page()->newGroupBox("Assign Common Input Level");
   editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, "12", "44");   // input sense
   editDetails()->page()->addSystemOverride(0, 0, 1, 1, "10", hex1, "06", "0D", "12", "more_than");
   editDetails()->page()->addSystemOverride(0, 0, 1, 1, "10", hex1, "06", "0D", "1B", "less_than");
   editDetails()->page()->addGroupBox(2, 4, 1, 1);

   editDetails()->page()->newGroupBox("Assign 5 Midi CC#");
   editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, "06", "1B");                    //midi ch
   editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, "06", "1C");   // midi cc#
   editDetails()->page()->addDataKnob(0, 2, 1, 1, "10", hex1, "06", "1D", "ASSIGN", "bottom", 60);   // cc# min
   //editDetails()->page()->addDataKnob(0, 3, 1, 1, "10", hex1, "06", "21", "ASSIGN", "bottom", 60);   // cc# max
   editDetails()->page()->addSystemOverride(0, 0, 1, 4, "10", hex1, "06", "0D", "1A", "more_than");
   editDetails()->page()->addGroupBox(2, 5, 1, 4);

   editDetails()->page()->addGroupBox(0, 0, 3, 8);

   editDetails()->addPage("Tables", "00", "02", "05", "04");       // Assign 5 ******************************************************************


   editDetails()->page()->newGroupBox(tr("Assign 6"));
   editDetails()->page()->newGroupBox(tr(""));
   editDetails()->page()->addSwitch(0, 0, 1, 1, "10", hex1, "06", "40", "middle", Qt::AlignCenter);
   editDetails()->page()->addGroupBox(0, 0, 1, 1);
   editDetails()->page()->newGroupBox("Assign 6 Target");
   editDetails()->page()->addTarget(0, 0, 1, 5, "10", hex1, "06", "41", "target","target", 140); // Target
   editDetails()->page()->addGroupBox(0, 1, 1, 7);
   editDetails()->page()->newGroupBox("Assign 6 Source");
   editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, "06", "4D");                    // Source
   editDetails()->page()->addStackControl();
   editDetails()->page()->addComboBox(0, 1, 1, 1, "10", hex1, "06", "4E");                    // Source Mode
   editDetails()->page()->addRange(0, 2, 1, 2, "10", hex1, "06", "4F", "Structure");         // min/max range widget
   editDetails()->page()->addGroupBox(1, 0, 1, 8);

   editDetails()->page()->newGroupBox("Assign 6 Switch timing");
   editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, "06", "57");   // rise time
   editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, "06", "58");   // fall time
   editDetails()->page()->addSystemOverride(0, 0, 1, 2, "10", hex1, "06", "4D", "12", "less_than");
   editDetails()->page()->addGroupBox(2, 0, 1, 2);

   editDetails()->page()->newGroupBox("Assign 6 Wave Pedal");
   editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, "06", "59", "normal", "bottom", 70);// Wave Form Rate
   editDetails()->page()->addComboBox(0, 1, 1, 1, "10", hex1, "06", "5A");                    // Wave Form
   editDetails()->page()->addSystemOverride(0, 0, 1, 2, "10", hex1, "06", "4D", "12", "not_equals");
   editDetails()->page()->addGroupBox(2, 2, 1, 2);

   editDetails()->page()->newGroupBox("Assign Common Input Level");
   editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, "12", "44");   // input sense
   editDetails()->page()->addSystemOverride(0, 0, 1, 1, "10", hex1, "06", "4D", "12", "more_than");
   editDetails()->page()->addSystemOverride(0, 0, 1, 1, "10", hex1, "06", "4D", "1B", "less_than");
   editDetails()->page()->addGroupBox(2, 4, 1, 1);

   editDetails()->page()->newGroupBox("Assign 6 Midi CC#");
   editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, "06", "5B");                    //midi ch
   editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, "06", "5C");   // midi cc#
   editDetails()->page()->addDataKnob(0, 2, 1, 1, "10", hex1, "06", "5D", "ASSIGN", "bottom", 60);   // cc# min
   //editDetails()->page()->addDataKnob(0, 3, 1, 1, "10", hex1, "06", "61", "ASSIGN", "bottom", 60);   // cc# max
   editDetails()->page()->addSystemOverride(0, 0, 1, 4, "10", hex1, "06", "4D", "1A", "more_than");
   editDetails()->page()->addGroupBox(2, 5, 1, 4);

   editDetails()->page()->addGroupBox(0, 0, 3, 8);

   editDetails()->addPage("Tables", "00", "02", "05", "05");       // Assign 6 ******************************************************************


   editDetails()->page()->newGroupBox(tr("Assign 7"));
   editDetails()->page()->newGroupBox(tr(""));
   editDetails()->page()->addSwitch(0, 0, 1, 1, "10", hex1, "07", "00", "middle", Qt::AlignCenter);
   editDetails()->page()->addGroupBox(0, 0, 1, 1);
   editDetails()->page()->newGroupBox("Assign 7 Target");
   editDetails()->page()->addTarget(0, 0, 1, 5, "10", hex1, "07", "01", "target","target", 140); // Target
   editDetails()->page()->addGroupBox(0, 1, 1, 7);
   editDetails()->page()->newGroupBox("Assign 7 Source");
   editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, "07", "0D");                    // Source
   editDetails()->page()->addStackControl();
   editDetails()->page()->addComboBox(0, 1, 1, 1, "10", hex1, "07", "0E");                    // Source Mode
   editDetails()->page()->addRange(0, 2, 1, 2, "10", hex1, "07", "0F", "Structure");         // min/max range widget
   editDetails()->page()->addGroupBox(1, 0, 1, 8);

   editDetails()->page()->newGroupBox("Assign 7 Switch timing");
   editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, "07", "17");   // rise time
   editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, "07", "18");   // fall time
   editDetails()->page()->addSystemOverride(0, 0, 1, 2, "10", hex1, "07", "0D", "12", "less_than");
   editDetails()->page()->addGroupBox(2, 0, 1, 2);

   editDetails()->page()->newGroupBox("Assign 7 Wave Pedal");
   editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, "07", "19", "normal", "bottom", 70);// Wave Form Rate
   editDetails()->page()->addComboBox(0, 1, 1, 1, "10", hex1, "07", "1A");                    // Wave Form
   editDetails()->page()->addSystemOverride(0, 0, 1, 2, "10", hex1, "07", "0D", "12", "not_equals");
   editDetails()->page()->addGroupBox(2, 2, 1, 2);

   editDetails()->page()->newGroupBox("Assign Common Input Level");
   editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, "12", "44");   // input sense
   editDetails()->page()->addSystemOverride(0, 0, 1, 1, "10", hex1, "07", "0D", "12", "more_than");
   editDetails()->page()->addSystemOverride(0, 0, 1, 1, "10", hex1, "07", "0D", "1B", "less_than");
   editDetails()->page()->addGroupBox(2, 4, 1, 1);

   editDetails()->page()->newGroupBox("Assign 7 Midi CC#");
   editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, "07", "1B");                    //midi ch
   editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, "07", "1C");   // midi cc#
   editDetails()->page()->addDataKnob(0, 2, 1, 1, "10", hex1, "07", "1D", "ASSIGN", "bottom", 60);   // cc# min
   //editDetails()->page()->addDataKnob(0, 3, 1, 1, "10", hex1, "07", "21", "ASSIGN", "bottom", 60);   // cc# max
   editDetails()->page()->addSystemOverride(0, 0, 1, 4, "10", hex1, "07", "0D", "1A", "more_than");
   editDetails()->page()->addGroupBox(2, 5, 1, 4);

   editDetails()->page()->addGroupBox(0, 0, 3, 8);

   editDetails()->addPage("Tables", "00", "02", "05", "06");       // Assign 7 ******************************************************************


   editDetails()->page()->newGroupBox(tr("Assign 8"));
   editDetails()->page()->newGroupBox(tr(""));
   editDetails()->page()->addSwitch(0, 0, 1, 1, "10", hex1, "07", "40", "middle", Qt::AlignCenter);
   editDetails()->page()->addGroupBox(0, 0, 1, 1);
   editDetails()->page()->newGroupBox("Assign 8 Target");
   editDetails()->page()->addTarget(0, 0, 1, 5, "10", hex1, "07", "41", "target","target", 140); // Target
   editDetails()->page()->addGroupBox(0, 1, 1, 7);
   editDetails()->page()->newGroupBox("Assign 8 Source");
   editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, "07", "4D");                    // Source
   editDetails()->page()->addStackControl();
   editDetails()->page()->addComboBox(0, 1, 1, 1, "10", hex1, "07", "4E");                    // Source Mode
   editDetails()->page()->addRange(0, 2, 1, 2, "10", hex1, "07", "4F", "Structure");         // min/max range widget
   editDetails()->page()->addGroupBox(1, 0, 1, 8);

   editDetails()->page()->newGroupBox("Assign 8 Switch timing");
   editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, "07", "57");   // rise time
   editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, "07", "58");   // fall time
   editDetails()->page()->addSystemOverride(0, 0, 1, 2, "10", hex1, "07", "4D", "12", "less_than");
   editDetails()->page()->addGroupBox(2, 0, 1, 2);

   editDetails()->page()->newGroupBox("Assign 8 Wave Pedal");
   editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, "07", "59", "normal", "bottom", 70);// Wave Form Rate
   editDetails()->page()->addComboBox(0, 1, 1, 1, "10", hex1, "07", "5A");                    // Wave Form
   editDetails()->page()->addSystemOverride(0, 0, 1, 2, "10", hex1, "07", "4D", "12", "not_equals");
   editDetails()->page()->addGroupBox(2, 2, 1, 2);

   editDetails()->page()->newGroupBox("Assign Common Input Level");
   editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, "12", "44");   // input sense
   editDetails()->page()->addSystemOverride(0, 0, 1, 1, "10", hex1, "07", "4D", "12", "more_than");
   editDetails()->page()->addSystemOverride(0, 0, 1, 1, "10", hex1, "07", "4D", "1B", "less_than");
   editDetails()->page()->addGroupBox(2, 4, 1, 1);

   editDetails()->page()->newGroupBox("Assign 8 Midi CC#");
   editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, "07", "5B");                    //midi ch
   editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, "07", "5C");   // midi cc#
   editDetails()->page()->addDataKnob(0, 2, 1, 1, "10", hex1, "07", "5D", "ASSIGN", "bottom", 60);   // cc# min
   //editDetails()->page()->addDataKnob(0, 3, 1, 1, "10", hex1, "07", "61", "ASSIGN", "bottom", 60);   // cc# max
   editDetails()->page()->addSystemOverride(0, 0, 1, 4, "10", hex1, "07", "4D", "1A", "more_than");
   editDetails()->page()->addGroupBox(2, 5, 1, 4);

   editDetails()->page()->addGroupBox(0, 0, 3, 8);

   editDetails()->addPage("Tables", "00", "02", "05", "07");       // *Assign 8 *****************************************************************


   editDetails()->page()->newGroupBox(tr("Assign 9"));
   editDetails()->page()->newGroupBox(tr(""));
   editDetails()->page()->addSwitch(0, 0, 1, 1, "10", hex1, "08", "00", "middle", Qt::AlignCenter);
   editDetails()->page()->addGroupBox(0, 0, 1, 1);
   editDetails()->page()->newGroupBox("Assign 9 Target");
   editDetails()->page()->addTarget(0, 0, 1, 5, "10", hex1, "08", "01", "target","target", 140); // Target
   editDetails()->page()->addGroupBox(0, 1, 1, 7);
   editDetails()->page()->newGroupBox("Assign 9 Source");
   editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, "08", "0D");                    // Source
   editDetails()->page()->addStackControl();
   editDetails()->page()->addComboBox(0, 1, 1, 1, "10", hex1, "08", "0E");                    // Source Mode
   editDetails()->page()->addRange(0, 2, 1, 2, "10", hex1, "08", "0F", "Structure");         // min/max range widget
   editDetails()->page()->addGroupBox(1, 0, 1, 8);

   editDetails()->page()->newGroupBox("Assign 9 Switch timing");
   editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, "08", "17");   // rise time
   editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, "08", "18");   // fall time
   editDetails()->page()->addSystemOverride(0, 0, 1, 2, "10", hex1, "08", "0D", "12", "less_than");
   editDetails()->page()->addGroupBox(2, 0, 1, 2);

   editDetails()->page()->newGroupBox("Assign 9 Wave Pedal");
   editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, "08", "19", "normal", "bottom", 70);// Wave Form Rate
   editDetails()->page()->addComboBox(0, 1, 1, 1, "10", hex1, "08", "1A");                    // Wave Form
   editDetails()->page()->addSystemOverride(0, 0, 1, 2, "10", hex1, "08", "0D", "12", "not_equals");
   editDetails()->page()->addGroupBox(2, 2, 1, 2);

   editDetails()->page()->newGroupBox("Assign Common Input Level");
   editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, "12", "44");   // input sense
   editDetails()->page()->addSystemOverride(0, 0, 1, 1, "10", hex1, "08", "0D", "12", "more_than");
   editDetails()->page()->addSystemOverride(0, 0, 1, 1, "10", hex1, "08", "0D", "1B", "less_than");
   editDetails()->page()->addGroupBox(2, 4, 1, 1);

   editDetails()->page()->newGroupBox("Assign 9 Midi CC#");
   editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, "08", "1B");                    //midi ch
   editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, "08", "1C");   // midi cc#
   editDetails()->page()->addDataKnob(0, 2, 1, 1, "10", hex1, "08", "1D", "ASSIGN", "bottom", 60);   // cc# min
   //editDetails()->page()->addDataKnob(0, 3, 1, 1, "10", hex1, "08", "21", "ASSIGN", "bottom", 60);   // cc# max
   editDetails()->page()->addSystemOverride(0, 0, 1, 4, "10", hex1, "08", "0D", "1A", "more_than");
   editDetails()->page()->addGroupBox(2, 5, 1, 4);

   editDetails()->page()->addGroupBox(0, 0, 3, 8);

   editDetails()->addPage("Tables", "00", "02", "05", "08");       // *Assign 9 *****************************************************************


   editDetails()->page()->newGroupBox(tr("Assign 10"));
   editDetails()->page()->newGroupBox(tr(""));
   editDetails()->page()->addSwitch(0, 0, 1, 1, "10", hex1, "08", "40", "middle", Qt::AlignCenter);
   editDetails()->page()->addGroupBox(0, 0, 1, 1);
   editDetails()->page()->newGroupBox("Assign 10 Target");
   editDetails()->page()->addTarget(0, 0, 1, 5, "10", hex1, "08", "41", "target","target", 140); // Target
   editDetails()->page()->addGroupBox(0, 1, 1, 7);
   editDetails()->page()->newGroupBox("Assign 10 Source");
   editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, "08", "4D");                    // Source
   editDetails()->page()->addStackControl();
   editDetails()->page()->addComboBox(0, 1, 1, 1, "10", hex1, "08", "4E");                    // Source Mode
   editDetails()->page()->addRange(0, 2, 1, 2, "10", hex1, "08", "4F", "Structure");         // min/max range widget
   editDetails()->page()->addGroupBox(1, 0, 1, 8);

   editDetails()->page()->newGroupBox("Assign 10 Switch timing");
   editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, "08", "57");   // rise time
   editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, "08", "58");   // fall time
   editDetails()->page()->addSystemOverride(0, 0, 1, 2, "10", hex1, "08", "4D", "12", "less_than");
   editDetails()->page()->addGroupBox(2, 0, 1, 2);

   editDetails()->page()->newGroupBox("Assign 10 Wave Pedal");
   editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, "08", "59", "normal", "bottom", 70);// Wave Form Rate
   editDetails()->page()->addComboBox(0, 1, 1, 1, "10", hex1, "08", "5A");                    // Wave Form
   editDetails()->page()->addSystemOverride(0, 0, 1, 2, "10", hex1, "08", "4D", "12", "not_equals");
   editDetails()->page()->addGroupBox(2, 2, 1, 2);

   editDetails()->page()->newGroupBox("Assign Common Input Level");
   editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, "12", "44");   // input sense
   editDetails()->page()->addSystemOverride(0, 0, 1, 1, "10", hex1, "08", "4D", "12", "more_than");
   editDetails()->page()->addSystemOverride(0, 0, 1, 1, "10", hex1, "08", "4D", "1B", "less_than");
   editDetails()->page()->addGroupBox(2, 4, 1, 1);

   editDetails()->page()->newGroupBox("Assign 10 Midi CC#");
   editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, "08", "5B");                    //midi ch
   editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, "08", "5C");   // midi cc#
   editDetails()->page()->addDataKnob(0, 2, 1, 1, "10", hex1, "08", "5D", "ASSIGN", "bottom", 60);   // cc# min
   //editDetails()->page()->addDataKnob(0, 3, 1, 1, "10", hex1, "08", "61", "ASSIGN", "bottom", 60);   // cc# max
   editDetails()->page()->addSystemOverride(0, 0, 1, 4, "10", hex1, "08", "4D", "1A", "more_than");
   editDetails()->page()->addGroupBox(2, 5, 1, 4);

   editDetails()->page()->addGroupBox(0, 0, 3, 8);

    editDetails()->addPage("Tables", "00", "02", "05", "09");       // *Assign 10 *****************************************************************


    editDetails()->page()->newGroupBox(tr("Assign 11"));
    editDetails()->page()->newGroupBox(tr(""));
    editDetails()->page()->addSwitch(0, 0, 1, 1, "10", hex1, "09", "00", "middle", Qt::AlignCenter);
    editDetails()->page()->addGroupBox(0, 0, 1, 1);
    editDetails()->page()->newGroupBox("Assign 11 Target");
    editDetails()->page()->addTarget(0, 0, 1, 5, "10", hex1, "09", "01", "target","target", 140); // Target
    editDetails()->page()->addGroupBox(0, 1, 1, 7);
    editDetails()->page()->newGroupBox("Assign 11 Source");
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, "09", "0D");                    // Source
    editDetails()->page()->addStackControl();
    editDetails()->page()->addComboBox(0, 1, 1, 1, "10", hex1, "09", "0E");                    // Source Mode
    editDetails()->page()->addRange(0, 2, 1, 2, "10", hex1, "09", "0F", "Structure");         // min/max range widget
    editDetails()->page()->addGroupBox(1, 0, 1, 8);

    editDetails()->page()->newGroupBox("Assign 11 Switch timing");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, "09", "17");   // rise time
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, "09", "18");   // fall time
    editDetails()->page()->addSystemOverride(0, 0, 1, 2, "10", hex1, "09", "0D", "12", "less_than");
    editDetails()->page()->addGroupBox(2, 0, 1, 2);

    editDetails()->page()->newGroupBox("Assign 11 Wave Pedal");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, "09", "19", "normal", "bottom", 70);// Wave Form Rate
    editDetails()->page()->addComboBox(0, 1, 1, 1, "10", hex1, "09", "1A");                    // Wave Form
    editDetails()->page()->addSystemOverride(0, 0, 1, 2, "10", hex1, "09", "0D", "12", "not_equals");
    editDetails()->page()->addGroupBox(2, 2, 1, 2);

    editDetails()->page()->newGroupBox("Assign Common Input Level");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, "12", "44");   // input sense
    editDetails()->page()->addSystemOverride(0, 0, 1, 1, "10", hex1, "09", "0D", "12", "more_than");
    editDetails()->page()->addSystemOverride(0, 0, 1, 1, "10", hex1, "09", "0D", "1B", "less_than");
    editDetails()->page()->addGroupBox(2, 4, 1, 1);

    editDetails()->page()->newGroupBox("Assign 11 Midi CC#");
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, "09", "1B");                    //midi ch
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, "09", "1C");   // midi cc#
    editDetails()->page()->addDataKnob(0, 2, 1, 1, "10", hex1, "09", "1D", "ASSIGN", "bottom", 60);   // cc# min
    //editDetails()->page()->addDataKnob(0, 3, 1, 1, "10", hex1, "09", "21", "ASSIGN", "bottom", 60);   // cc# max
    editDetails()->page()->addSystemOverride(0, 0, 1, 4, "10", hex1, "09", "0D", "1A", "more_than");
    editDetails()->page()->addGroupBox(2, 5, 1, 4);

    editDetails()->page()->addGroupBox(0, 0, 3, 8);

    editDetails()->addPage("Tables", "00", "02", "05", "0A");       // *Assign 11 *****************************************************************


    editDetails()->page()->newGroupBox(tr("Assign 12"));
    editDetails()->page()->newGroupBox(tr(""));
    editDetails()->page()->addSwitch(0, 0, 1, 1, "10", hex1, "09", "40", "middle", Qt::AlignCenter);
    editDetails()->page()->addGroupBox(0, 0, 1, 1);
    editDetails()->page()->newGroupBox("Assign 12 Target");
    editDetails()->page()->addTarget(0, 0, 1, 5, "10", hex1, "09", "41", "target","target", 140); // Target
    editDetails()->page()->addGroupBox(0, 1, 1, 7);
    editDetails()->page()->newGroupBox("Assign 12 Source");
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, "09", "4D");                    // Source
    editDetails()->page()->addStackControl();
    editDetails()->page()->addComboBox(0, 1, 1, 1, "10", hex1, "09", "4E");                    // Source Mode
    editDetails()->page()->addRange(0, 2, 1, 2, "10", hex1, "09", "4F", "Structure");         // min/max range widget
    editDetails()->page()->addGroupBox(1, 0, 1, 8);

    editDetails()->page()->newGroupBox("Assign 12 Switch timing");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, "09", "57");   // rise time
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, "09", "58");   // fall time
    editDetails()->page()->addSystemOverride(0, 0, 1, 2, "10", hex1, "09", "4D", "12", "less_than");
    editDetails()->page()->addGroupBox(2, 0, 1, 2);

    editDetails()->page()->newGroupBox("Assign 12 Wave Pedal");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, "09", "59", "normal", "bottom", 70);// Wave Form Rate
    editDetails()->page()->addComboBox(0, 1, 1, 1, "10", hex1, "09", "5A");                    // Wave Form
    editDetails()->page()->addSystemOverride(0, 0, 1, 2, "10", hex1, "09", "4D", "12", "not_equals");
    editDetails()->page()->addGroupBox(2, 2, 1, 2);

    editDetails()->page()->newGroupBox("Assign Common Input Level");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, "12", "44");   // input sense
    editDetails()->page()->addSystemOverride(0, 0, 1, 1, "10", hex1, "09", "0D", "12", "more_than");
    editDetails()->page()->addSystemOverride(0, 0, 1, 1, "10", hex1, "09", "0D", "1B", "less_than");
    editDetails()->page()->addGroupBox(2, 4, 1, 1);

    editDetails()->page()->newGroupBox("Assign 12 Midi CC#");
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, "09", "5B");                    //midi ch
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, "09", "5C");   // midi cc#
    editDetails()->page()->addDataKnob(0, 2, 1, 1, "10", hex1, "09", "5D", "ASSIGN", "bottom", 60);   // cc# min
    //editDetails()->page()->addDataKnob(0, 3, 1, 1, "10", hex1, "09", "61", "ASSIGN", "bottom", 60);   // cc# max
    editDetails()->page()->addSystemOverride(0, 0, 1, 4, "10", hex1, "09", "4D", "1A", "more_than");
    editDetails()->page()->addGroupBox(2, 5, 1, 4);

    editDetails()->page()->addGroupBox(0, 0, 3, 8);

    editDetails()->addPage("Tables", "00", "02", "05", "0B");       // *Assign 12 *****************************************************************


    editDetails()->page()->newGroupBox(tr("Assign 13"));
    editDetails()->page()->newGroupBox(tr(""));
    editDetails()->page()->addSwitch(0, 0, 1, 1, "10", hex1, "0A", "00", "middle", Qt::AlignCenter);
    editDetails()->page()->addGroupBox(0, 0, 1, 1);
    editDetails()->page()->newGroupBox("Assign 13 Target");
    editDetails()->page()->addTarget(0, 0, 1, 5, "10", hex1, "0A", "01", "target","target", 140); // Target
    editDetails()->page()->addGroupBox(0, 1, 1, 7);
    editDetails()->page()->newGroupBox("Assign 13 Source");
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, "0A", "0D");                    // Source
    editDetails()->page()->addStackControl();
    editDetails()->page()->addComboBox(0, 1, 1, 1, "10", hex1, "0A", "0E");                    // Source Mode
    editDetails()->page()->addRange(0, 2, 1, 2, "10", hex1, "0A", "0F", "Structure");         // min/max range widget
    editDetails()->page()->addGroupBox(1, 0, 1, 8);

    editDetails()->page()->newGroupBox("Assign 13 Switch timing");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, "0A", "17");   // rise time
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, "0A", "18");   // fall time
    editDetails()->page()->addSystemOverride(0, 0, 1, 2, "10", hex1, "0A", "0D", "12", "less_than");
    editDetails()->page()->addGroupBox(2, 0, 1, 2);

    editDetails()->page()->newGroupBox("Assign 13 Wave Pedal");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, "0A", "19", "normal", "bottom", 70);// Wave Form Rate
    editDetails()->page()->addComboBox(0, 1, 1, 1, "10", hex1, "0A", "1A");                    // Wave Form
    editDetails()->page()->addSystemOverride(0, 0, 1, 2, "10", hex1, "0A", "0D", "12", "not_equals");
    editDetails()->page()->addGroupBox(2, 2, 1, 2);

    editDetails()->page()->newGroupBox("Assign Common Input Level");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, "12", "44");   // input sense
    editDetails()->page()->addSystemOverride(0, 0, 1, 1, "10", hex1, "0A", "0D", "12", "more_than");
    editDetails()->page()->addSystemOverride(0, 0, 1, 1, "10", hex1, "0A", "0D", "1B", "less_than");
    editDetails()->page()->addGroupBox(2, 4, 1, 1);

    editDetails()->page()->newGroupBox("Assign 13 Midi CC#");
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, "0A", "1B");                    //midi ch
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, "0A", "1C");   // midi cc#
    editDetails()->page()->addDataKnob(0, 2, 1, 1, "10", hex1, "0A", "1D", "ASSIGN", "bottom", 60);   // cc# min
    //editDetails()->page()->addDataKnob(0, 3, 1, 1, "10", hex1, "0A", "21", "ASSIGN", "bottom", 60);   // cc# max
    editDetails()->page()->addSystemOverride(0, 0, 1, 4, "10", hex1, "0A", "0D", "1A", "more_than");
    editDetails()->page()->addGroupBox(2, 5, 1, 4);

    editDetails()->page()->addGroupBox(0, 0, 3, 8);

    editDetails()->addPage("Tables", "00", "02", "05", "0C");       // *Assign 13 *****************************************************************


    editDetails()->page()->newGroupBox(tr("Assign 14"));
    editDetails()->page()->newGroupBox(tr(""));
    editDetails()->page()->addSwitch(0, 0, 1, 1, "10", hex1, "0A", "40", "middle", Qt::AlignCenter);
    editDetails()->page()->addGroupBox(0, 0, 1, 1);
    editDetails()->page()->newGroupBox("Assign 14 Target");
    editDetails()->page()->addTarget(0, 0, 1, 5, "10", hex1, "0A", "41", "target","target", 140); // Target
    editDetails()->page()->addGroupBox(0, 1, 1, 7);
    editDetails()->page()->newGroupBox("Assign 14 Source");
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, "0A", "4D");                    // Source
    editDetails()->page()->addStackControl();
    editDetails()->page()->addComboBox(0, 1, 1, 1, "10", hex1, "0A", "4E");                    // Source Mode
    editDetails()->page()->addRange(0, 2, 1, 2, "10", hex1, "0A", "4F", "Structure");         // min/max range widget
    editDetails()->page()->addGroupBox(1, 0, 1, 8);

    editDetails()->page()->newGroupBox("Assign 14 Switch timing");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, "0A", "57");   // rise time
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, "0A", "58");   // fall time
    editDetails()->page()->addSystemOverride(0, 0, 1, 2, "10", hex1, "0A", "4D", "12", "less_than");
    editDetails()->page()->addGroupBox(2, 0, 1, 2);

    editDetails()->page()->newGroupBox("Assign 14 Wave Pedal");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, "0A", "59", "normal", "bottom", 70);// Wave Form Rate
    editDetails()->page()->addComboBox(0, 1, 1, 1, "10", hex1, "0A", "5A");                    // Wave Form
    editDetails()->page()->addSystemOverride(0, 0, 1, 2, "10", hex1, "0A", "4D", "12", "not_equals");
    editDetails()->page()->addGroupBox(2, 2, 1, 2);

    editDetails()->page()->newGroupBox("Assign Common Input Level");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, "12", "44");   // input sense
    editDetails()->page()->addSystemOverride(0, 0, 1, 1, "10", hex1, "0A", "4D", "12", "more_than");
    editDetails()->page()->addSystemOverride(0, 0, 1, 1, "10", hex1, "0A", "4D", "1B", "less_than");
    editDetails()->page()->addGroupBox(2, 4, 1, 1);

    editDetails()->page()->newGroupBox("Assign 14 Midi CC#");
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, "0A", "5B");                    //midi ch
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, "0A", "5C");   // midi cc#
    editDetails()->page()->addDataKnob(0, 2, 1, 1, "10", hex1, "0A", "5D", "ASSIGN", "bottom", 60);   // cc# min
    //editDetails()->page()->addDataKnob(0, 3, 1, 1, "10", hex1, "0A", "61", "ASSIGN", "bottom", 60);   // cc# max
    editDetails()->page()->addSystemOverride(0, 0, 1, 4, "10", hex1, "0A", "4D", "1A", "more_than");
    editDetails()->page()->addGroupBox(2, 5, 1, 4);

    editDetails()->page()->addGroupBox(0, 0, 3, 8);

    editDetails()->addPage("Tables", "00", "02", "05", "0D");       // *Assign 14 *****************************************************************


    editDetails()->page()->newGroupBox(tr("Assign 15"));
    editDetails()->page()->newGroupBox(tr(""));
    editDetails()->page()->addSwitch(0, 0, 1, 1, "10", hex1, "0B", "00", "middle", Qt::AlignCenter);
    editDetails()->page()->addGroupBox(0, 0, 1, 1);
    editDetails()->page()->newGroupBox("Assign 15 Target");
    editDetails()->page()->addTarget(0, 0, 1, 5, "10", hex1, "0B", "01", "target","target", 140); // Target
    editDetails()->page()->addGroupBox(0, 1, 1, 7);
    editDetails()->page()->newGroupBox("Assign 15 Source");
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, "0B", "0D");                    // Source
    editDetails()->page()->addStackControl();
    editDetails()->page()->addComboBox(0, 1, 1, 1, "10", hex1, "0B", "0E");                    // Source Mode
    editDetails()->page()->addRange(0, 2, 1, 2, "10", hex1, "0B", "0F", "Structure");         // min/max range widget
    editDetails()->page()->addGroupBox(1, 0, 1, 8);

    editDetails()->page()->newGroupBox("Assign 15 Switch timing");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, "0B", "17");   // rise time
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, "0B", "18");   // fall time
    editDetails()->page()->addSystemOverride(0, 0, 1, 2, "10", hex1, "0B", "0D", "12", "less_than");
    editDetails()->page()->addGroupBox(2, 0, 1, 2);

    editDetails()->page()->newGroupBox("Assign 15 Wave Pedal");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, "0B", "19", "normal", "bottom", 70);// Wave Form Rate
    editDetails()->page()->addComboBox(0, 1, 1, 1, "10", hex1, "0B", "1A");                    // Wave Form
    editDetails()->page()->addSystemOverride(0, 0, 1, 2, "10", hex1, "0B", "0D", "12", "not_equals");
    editDetails()->page()->addGroupBox(2, 2, 1, 2);

    editDetails()->page()->newGroupBox("Assign Common Input Level");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, "12", "44");   // input sense
    editDetails()->page()->addSystemOverride(0, 0, 1, 1, "10", hex1, "0B", "0D", "12", "more_than");
    editDetails()->page()->addSystemOverride(0, 0, 1, 1, "10", hex1, "0B", "0D", "1B", "less_than");
    editDetails()->page()->addGroupBox(2, 4, 1, 1);

    editDetails()->page()->newGroupBox("Assign 15 Midi CC#");
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, "0B", "1B");                    //midi ch
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, "0B", "1C");   // midi cc#
    editDetails()->page()->addDataKnob(0, 2, 1, 1, "10", hex1, "0B", "1D", "ASSIGN", "bottom", 60);   // cc# min
    //editDetails()->page()->addDataKnob(0, 3, 1, 1, "10", hex1, "0B", "21", "ASSIGN", "bottom", 60);   // cc# max
    editDetails()->page()->addSystemOverride(0, 0, 1, 4, "10", hex1, "0B", "0D", "1A", "more_than");
    editDetails()->page()->addGroupBox(2, 5, 1, 4);

    editDetails()->page()->addGroupBox(0, 0, 3, 8);

    editDetails()->addPage("Tables", "00", "02", "05", "0E");       // *Assign 15 *****************************************************************


    editDetails()->page()->newGroupBox(tr("Assign 16"));
    editDetails()->page()->newGroupBox(tr(""));
    editDetails()->page()->addSwitch(0, 0, 1, 1, "10", hex1, "0B", "40", "middle", Qt::AlignCenter);
    editDetails()->page()->addGroupBox(0, 0, 1, 1);
    editDetails()->page()->newGroupBox("Assign 16 Target");
    editDetails()->page()->addTarget(0, 0, 1, 5, "10", hex1, "0B", "41", "target","target", 140); // Target
    editDetails()->page()->addGroupBox(0, 1, 1, 7);
    editDetails()->page()->newGroupBox("Assign 16 Source");
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, "0B", "4D");                    // Source
    editDetails()->page()->addStackControl();
    editDetails()->page()->addComboBox(0, 1, 1, 1, "10", hex1, "0B", "4E");                    // Source Mode
    editDetails()->page()->addRange(0, 2, 1, 2, "10", hex1, "0B", "4F", "Structure");         // min/max range widget
    editDetails()->page()->addGroupBox(1, 0, 1, 8);

    editDetails()->page()->newGroupBox("Assign 16 Switch timing");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, "0B", "57");   // rise time
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, "0B", "58");   // fall time
    editDetails()->page()->addSystemOverride(0, 0, 1, 2, "10", hex1, "0B", "4D", "12", "less_than");
    editDetails()->page()->addGroupBox(2, 0, 1, 2);

    editDetails()->page()->newGroupBox("Assign 16 Wave Pedal");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, "0B", "59", "normal", "bottom", 70);// Wave Form Rate
    editDetails()->page()->addComboBox(0, 1, 1, 1, "10", hex1, "0B", "5A");                    // Wave Form
    editDetails()->page()->addSystemOverride(0, 0, 1, 2, "10", hex1, "0B", "4D", "12", "not_equals");
    editDetails()->page()->addGroupBox(2, 2, 1, 2);

    editDetails()->page()->newGroupBox("Assign Common Input Level");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, "12", "44");   // input sense
    editDetails()->page()->addSystemOverride(0, 0, 1, 1, "10", hex1, "0B", "4D", "12", "more_than");
    editDetails()->page()->addSystemOverride(0, 0, 1, 1, "10", hex1, "0B", "4D", "1B", "less_than");
    //editDetails()->page()->addSystemOverride(0, 0, 0, 0, "10", hex1, "12", "44", "FF", "command_update_system", "FF"); //
    editDetails()->page()->addGroupBox(2, 4, 1, 1);

    editDetails()->page()->newGroupBox("Assign 16 Midi CC#");
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, "0B", "5B");                    //midi ch
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, "0B", "5C");   // midi cc#
    editDetails()->page()->addDataKnob(0, 2, 1, 1, "10", hex1, "0B", "5D", "ASSIGN", "bottom", 60);   // cc# min
    //editDetails()->page()->addDataKnob(0, 3, 1, 1, "10", hex1, "0B", "61", "ASSIGN", "bottom", 60);   // cc# max
    editDetails()->page()->addSystemOverride(0, 0, 1, 4, "10", hex1, "0B", "4D", "1A", "more_than");
    editDetails()->page()->addGroupBox(2, 5, 1, 4);

    editDetails()->page()->addGroupBox(0, 0, 3, 8);

    editDetails()->addPage("Tables", "00", "02", "05", "0F");       // *Assign 16 *****************************************************************
}
