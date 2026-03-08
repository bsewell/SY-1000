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

#include "stompbox_master.h"
#include "Preferences.h"
#include "SysxIO.h"

stompbox_master::stompbox_master(QWidget *parent)
    : stompBox(parent)
{
    SysxIO *sysxIO = SysxIO::Instance();
    sysxIO->deBug("build Master stomp");

    /* mixer */
    setOnImage(":/images/master.png");
    setOffImage(":/images/master.png");
    setHoverImage(":/images/effect_hover.png");
    setSelectedImage(":/images/effect_select.png");
    //setLSB("12", "20");
    brushColor("green");
  /*editDetails()->patchPos(1660, 6, "07", "5A"); */
    setEditPages();
}

void stompbox_master::updateSignal()
{

}

void stompbox_master::setEditPages()
{
    QString hex1 = "00";
    QString hex2A = "20";
    QString hex2B = "2B";
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex1 = "02";
        hex2A = "1F";
        hex2B = "29";
    };

    editDetails()->page()->newGroupBox("Master");
    editDetails()->page()->addDataKnob(0, 0, 2, 1, "10", hex1, "12", "3C", "0~200_ratio3");              // patch level
    editDetails()->page()->addDataKnob(0, 2, 1, 1, "10", hex1, "12", "3E", "BPM_ratio2", "bottom", 70);  // tempo BPM
    editDetails()->page()->addComboBox(1, 2, 1, 1, "10", hex1, "12", "43", "large");                     // tempo hold
    editDetails()->page()->addComboBox(0, 3, 2, 1, "10", hex1, "12", "42", "large");                     // key
    editDetails()->page()->addComboBox(0, 4, 2, 1, "10", hex1, "12", "00", "icon_large_40");             // gk/normal patch type
    editDetails()->page()->addSystemOverride(1, 1, 1, 1, "10", hex1, "12", "00", "00", "command_equal", "10", hex1, "15", "01", "00" ); //control Synth 1 to dynamic
    editDetails()->page()->addSystemOverride(1, 1, 1, 1, "10", hex1, "12", "00", "00", "command_equal", "10", hex1, hex2A, "01", "00" ); //control Synth 2 to dynamic
    editDetails()->page()->addSystemOverride(1, 1, 1, 1, "10", hex1, "12", "00", "00", "command_equal", "10", hex1, hex2B, "01", "00" ); //control Synth 3 to dynamic
    editDetails()->page()->addComboBox(0, 5, 1, 1, "10", hex1, "02", "00", "large");                     // GK Set
    editDetails()->page()->addSystemOverride(0, 5, 1, 1, "10", hex1, "12", "00", "01");
    editDetails()->page()->addComboBox(1, 5, 1, 1, "10", hex1, "02", "01", "large");                     // normal set
    editDetails()->page()->addSystemOverride(1, 5, 1, 1, "10", hex1, "12", "00", "00");
    editDetails()->page()->addGroupBox(0, 0, 1, 6);

    editDetails()->addPage("Tables", "00", "02", "03", "00");       // ******************************************************************

    editDetails()->page()->newGroupBox("Patch Midi 1");
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, "00", "39");  // channel
    editDetails()->page()->addComboBox(1, 0, 1, 1, "10", hex1, "00", "3E");  // patch
    editDetails()->page()->addComboBox(2, 0, 1, 1, "10", hex1, "00", "3A");  // msb
    editDetails()->page()->addComboBox(3, 0, 1, 1, "10", hex1, "00", "3C");  // lsb
    editDetails()->page()->addSystemOverride(2, 0, 2, 1, "10", hex1, "00", "3E", "00", "more_than");
    editDetails()->page()->addComboBox(4, 0, 1, 1, "10", hex1, "00", "40");  // cc#1
    editDetails()->page()->addComboBox(5, 0, 1, 1, "10", hex1, "00", "42");  // cc# 1 value
    editDetails()->page()->addComboBox(6, 0, 1, 1, "10", hex1, "00", "43");  // cc#2
    editDetails()->page()->addComboBox(7, 0, 1, 1, "10", hex1, "00", "45");  // cc#2 value
    editDetails()->page()->addGroupBox(1, 0, 7, 1);

    editDetails()->page()->newGroupBox("Patch Midi 2");
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, "00", "46");  // channel
    editDetails()->page()->addComboBox(1, 0, 1, 1, "10", hex1, "00", "4B");  // patch
    editDetails()->page()->addComboBox(2, 0, 1, 1, "10", hex1, "00", "47");  // msb
    editDetails()->page()->addComboBox(3, 0, 1, 1, "10", hex1, "00", "49");  // lsb
    editDetails()->page()->addSystemOverride(2, 0, 2, 1, "10", hex1, "00", "4B", "00", "more_than");
    editDetails()->page()->addComboBox(4, 0, 1, 1, "10", hex1, "00", "4D");  // cc#1
    editDetails()->page()->addComboBox(5, 0, 1, 1, "10", hex1, "00", "4F");  // cc# 1 value
    editDetails()->page()->addComboBox(6, 0, 1, 1, "10", hex1, "00", "50");  // cc#2
    editDetails()->page()->addComboBox(7, 0, 1, 1, "10", hex1, "00", "52");  // cc#2 value    
    editDetails()->page()->addGroupBox(1, 1, 7, 1);

    editDetails()->page()->newGroupBox("Patch Midi 3");
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, "00", "53");  // channel
    editDetails()->page()->addComboBox(1, 0, 1, 1, "10", hex1, "00", "58");  // patch
    editDetails()->page()->addComboBox(2, 0, 1, 1, "10", hex1, "00", "54");  // msb
    editDetails()->page()->addComboBox(3, 0, 1, 1, "10", hex1, "00", "56");  // lsb
    editDetails()->page()->addSystemOverride(2, 0, 2, 1, "10", hex1, "00", "58", "00", "more_than");
    editDetails()->page()->addComboBox(4, 0, 1, 1, "10", hex1, "00", "5A");  // cc#1
    editDetails()->page()->addComboBox(5, 0, 1, 1, "10", hex1, "00", "5C");  // cc# 1 value
    editDetails()->page()->addComboBox(6, 0, 1, 1, "10", hex1, "00", "5D");  // cc#2
    editDetails()->page()->addComboBox(7, 0, 1, 1, "10", hex1, "00", "5F");  // cc#2 value    
    editDetails()->page()->addGroupBox(1, 2, 7, 1);

    editDetails()->page()->newGroupBox("Patch Midi 4");
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, "00", "60");  // channel
    editDetails()->page()->addComboBox(1, 0, 1, 1, "10", hex1, "00", "65");  // patch
    editDetails()->page()->addComboBox(2, 0, 1, 1, "10", hex1, "00", "61");  // msb
    editDetails()->page()->addComboBox(3, 0, 1, 1, "10", hex1, "00", "63");  // lsb
    editDetails()->page()->addSystemOverride(2, 0, 2, 1, "10", hex1, "00", "65", "00", "more_than");
    editDetails()->page()->addComboBox(4, 0, 1, 1, "10", hex1, "00", "67");  // cc#1
    editDetails()->page()->addComboBox(5, 0, 1, 1, "10", hex1, "00", "69");  // cc# 1 value
    editDetails()->page()->addComboBox(6, 0, 1, 1, "10", hex1, "00", "6A");  // cc#2
    editDetails()->page()->addComboBox(7, 0, 1, 1, "10", hex1, "00", "6C");  // cc#2 value    
    editDetails()->page()->addGroupBox(1, 3, 7, 1);

    editDetails()->page()->newGroupBox("Midi CC# A Controller");
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, "00", "6D");  // cc#1 source
    editDetails()->page()->addComboBox(1, 0, 1, 1, "10", hex1, "00", "6E");  // cc#1 value
    editDetails()->page()->addGroupBox(1, 4, 7, 1);

    editDetails()->page()->newGroupBox("Midi CC# B Controller");
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, "00", "6F");  // cc#2 source
    editDetails()->page()->addComboBox(1, 0, 1, 1, "10", hex1, "00", "70");  // cc#2 value
    editDetails()->page()->addGroupBox(1, 5, 7, 1);


    editDetails()->addPage("Tables", "00", "02", "03", "01");       // ******************************************************************


    editDetails()->page()->newGroupBox("Guitar to Midi");
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, "14", "00");  // mode
    editDetails()->page()->addComboBox(1, 0, 1, 1, "10", hex1, "14", "01");  // alt tune
    editDetails()->page()->addComboBox(2, 0, 1, 1, "10", hex1, "14", "02");  // play feel
    editDetails()->page()->addComboBox(3, 0, 1, 1, "10", hex1, "14", "03");  // chromatic
    editDetails()->page()->addComboBox(4, 0, 1, 1, "10", hex1, "14", "04");  // hold type
    editDetails()->page()->addComboBox(5, 0, 1, 1, "10", hex1, "14", "05");  // dynamics
    editDetails()->page()->addComboBox(6, 0, 1, 1, "10", hex1, "14", "06");  // low velo cut
    editDetails()->page()->addGroupBox(1, 0, 7, 1);

    editDetails()->page()->newGroupBox("Poly Mode Midi Patch");
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, "00", "71");  // msb
    editDetails()->page()->addComboBox(1, 0, 1, 1, "10", hex1, "00", "73");  // lsb
    editDetails()->page()->addComboBox(2, 0, 1, 1, "10", hex1, "00", "75");  // patch
    editDetails()->page()->addSystemOverride(0, 0, 7, 1, "10", hex1, "14", "00", "00", "more_than");
    editDetails()->page()->addGroupBox(1, 1, 7, 1);

    editDetails()->page()->newGroupBox("Mono Mode String Midi Patch");

    editDetails()->page()->newGroupBox("String 1 Patch");
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, "00", "77");  // msb
    editDetails()->page()->addComboBox(1, 0, 1, 1, "10", hex1, "00", "79");  // lsb
    editDetails()->page()->addComboBox(2, 0, 1, 1, "10", hex1, "00", "7B");  // patch

    editDetails()->page()->addGroupBox(1, 2, 7, 1);

    editDetails()->page()->newGroupBox("String 2 Patch");
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, "00", "7D");  // msb
    //editDetails()->page()->addComboBox(1, 0, 1, 1, "10", hex1, "00", "7F");  // lsb
    editDetails()->page()->addComboBox(2, 0, 1, 1, "10", hex1, "01", "01");  // patch
    editDetails()->page()->addGroupBox(1, 3, 7, 1);

    editDetails()->page()->newGroupBox("String 3 Patch");
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, "01", "03");  // msb
    editDetails()->page()->addComboBox(1, 0, 1, 1, "10", hex1, "01", "05");  // lsb
    editDetails()->page()->addComboBox(2, 0, 1, 1, "10", hex1, "01", "07");  // patch
    editDetails()->page()->addGroupBox(1, 4, 7, 1);

    editDetails()->page()->newGroupBox("String 4 Patch");
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, "01", "09");  // msb
    editDetails()->page()->addComboBox(1, 0, 1, 1, "10", hex1, "01", "0B");  // lsb
    editDetails()->page()->addComboBox(2, 0, 1, 1, "10", hex1, "01", "0D");  // patch
    editDetails()->page()->addGroupBox(1, 5, 7, 1);

    editDetails()->page()->newGroupBox("String 5 Patch");
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, "01", "0F");  // msb
    editDetails()->page()->addComboBox(1, 0, 1, 1, "10", hex1, "01", "11");  // lsb
    editDetails()->page()->addComboBox(2, 0, 1, 1, "10", hex1, "01", "13");  // patch
    editDetails()->page()->addGroupBox(1, 6, 7, 1);

    editDetails()->page()->newGroupBox("String 6 Patch");
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, "01", "15");  // msb
    editDetails()->page()->addComboBox(1, 0, 1, 1, "10", hex1, "01", "17");  // lsb
    editDetails()->page()->addComboBox(2, 0, 1, 1, "10", hex1, "01", "19");  // patch
    editDetails()->page()->addGroupBox(1, 7, 7, 1);

    editDetails()->page()->addSystemOverride(0, 0, 8, 8, "10", hex1, "14", "00", "01", "less_than");
    editDetails()->page()->addGroupBox(1, 7, 7, 7);


    editDetails()->addPage("Tables", "00", "02", "03", "03");       // ******************************************************************

}
