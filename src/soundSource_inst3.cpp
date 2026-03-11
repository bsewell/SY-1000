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

#include "soundSource_inst3.h"
#include "Preferences.h"
#include "SysxIO.h"

soundsource_inst3::soundsource_inst3(QWidget *parent)
    : stompBox(parent)
{
    SysxIO *sysxIO = SysxIO::Instance();
    sysxIO->deBug("build Instrument3 stomp");

    hex1 = "00";
    hex2 = "2B";
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){hex1 = "02"; hex2 = "29";};

    /* INST 1 */
    setOnImage(":/images/inst3_on.png");
    setOffImage(":/images/inst3_off.png");
    setHoverImage(":/images/inst_hover.png");
    setSelectedImage(":/images/inst_select.png");
    setLSB(hex1, hex2);
    brushColor("red");
    setButton("10", hex1, hex2, "00");
    //editDetails()->patchPos(1756, 454, "10", "00");
    setEditPages();
}

void soundsource_inst3::updateSignal()
{
    hex1 = "00";
    hex2 = "2B";
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex1 = "02";
        hex2 = "29";
    };

    updateButton("10", hex1, hex2, "00");
    updateLabel("10", hex1, hex2, "01");
}

void soundsource_inst3::setEditPages()
{
    str1 = "String 1";
    str2 = "String 2";
    str3 = "String 3";
    str4 = "String 4";
    str5 = "String 5";
    str6 = "String 6";
    hex1 = "00";
    hex2 = "2B";
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){
        str1 = "String Hi C";
        str2 = "String 1";
        str3 = "String 2";
        str4 = "String 3";
        str5 = "String 4";
        str6 = "String Lo B";
        hex1 = "02";
        hex2 = "29";
    };

    editDetails()->page()->setFlatGroupBoxes(true);
    editDetails()->page()->newStackControl(10);
    editDetails()->page()->addStackControl();
    editDetails()->page()->newStackControl(0);
    editDetails()->page()->linkStackControl(10);
    editDetails()->page()->addComboBox(0, 0, 1, 2, "10", hex1, hex2, "01", "left_large"); // type
    editDetails()->page()->addStackControl();
    editDetails()->page()->insertStackField(10, 0, 2, 1, 8, Qt::AlignVCenter | Qt::AlignLeft);
    editDetails()->page()->addDivider(1, 0, 1, 10, Qt::AlignTop | Qt::AlignLeft);
    editDetails()->page()->insertStackField(0, 2, 0, 1, 10, Qt::AlignTop | Qt::AlignLeft);

    // *********************************************************************************************************** DYNAMIC SYNTH ********************
    editDetails()->page()->newStackControl(1);
    editDetails()->page()->newStackField(10, Qt::AlignVCenter | Qt::AlignLeft);
    editDetails()->page()->addStaticTabBar(0, 0, 1, 1,
                                           QStringList() << "COMMON" << "ALT TUNE" << "OSC" << "FILTER" << "AMP" << "LFO1" << "LFO2" << "SEQ" << "LAYER",
                                           QList<int>() << 7 << 6 << 0 << 1 << 8 << 2 << 9 << 3 << 5,
                                           "boss_inst");
    editDetails()->page()->addStackField();
    editDetails()->page()->addStackControl();
    editDetails()->page()->newStackField(0);
    editDetails()->page()->insertStackField(1, 0, 0, 1, 1, Qt::AlignTop | Qt::AlignLeft);
    editDetails()->page()->addStackField();
    // ***********************************************************************************************************************************************

    editDetails()->page()->newStackField(1, Qt::AlignTop | Qt::AlignLeft);
    editDetails()->page()->compactCurrentStackField(20, 24);
    QString hex2A;
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex2 = "2A";
        hex2A = "2B";
    }
    else
    {
        hex2 = "2C";
        hex2A = "2D";
    };
    editDetails()->page()->addComboBox(0, 0, 1, 2, "10", hex1,  hex2, "04", "left_large"); // wave type
    editDetails()->page()->addKnob(1, 0, 1, 1, "10", hex1,  hex2, "05", "normal_ratio1.5");   // pitch
    editDetails()->page()->addKnob(1, 1, 1, 1, "10", hex1,  hex2, "06", "normal_ratio1.25");  // fine
    editDetails()->page()->addKnob(1, 2, 1, 1, "10", hex1,  hex2, "0A", "normal_ratio1.25");  // detune
    editDetails()->page()->addSystemOverride(1, 2, 1, 1, "10", hex1,  hex2, "04", "05", "not_equal");
    editDetails()->page()->addKnob(1, 3, 1, 1, "10", hex1,  hex2, "0B", "normal_ratio1.25");  // sharpness
    editDetails()->page()->addSystemOverride(1, 3, 1, 1, "10", hex1,  hex2, "04", "06", "not_equal");
    editDetails()->page()->addKnob(1, 4, 1, 1, "10", hex1,  hex2, "0C", "normal_ratio1.25");  // feedback
    editDetails()->page()->addKnob(1, 5, 1, 1, "10", hex1,  hex2, "0D", "normal_ratio1.25");  // harmonics
    editDetails()->page()->addSystemOverride(1, 4, 1, 2, "10", hex1,  hex2, "04", "07", "not_equal");
    editDetails()->page()->addKnob(1, 6, 1, 1, "10", hex1,  hex2, "0E", "normal_ratio1.25");  // s-saw detune
    editDetails()->page()->addSystemOverride(1, 6, 1, 1, "10", hex1,  hex2, "04", "08", "not_equal");
    editDetails()->page()->addKnob(1, 7, 1, 1, "10", hex1,  hex2, "07", "normal_ratio1.25");  // pulse width
    editDetails()->page()->addSystemOverride(1, 7, 1, 1, "10", hex1,  hex2, "04", "04", "not_equal");
    editDetails()->page()->addSystemOverride(1, 0, 1, 8, "10", hex1,  hex2, "04", "09");
    editDetails()->page()->addKnob(2, 0, 1, 1, "10", hex1,  hex2, "08", "normal_ratio1.25");  // pwm env attack
    editDetails()->page()->addKnob(2, 1, 1, 1, "10", hex1,  hex2, "09", "normal_ratio1.25");  // pwm env depth
    editDetails()->page()->addSystemOverride(2, 0, 1, 2, "10", hex1,  hex2, "04", "04", "not_equal");
    editDetails()->page()->addKnob(2, 2, 1, 1, "10", hex1,  hex2, "0F", "normal_ratio1.25");  // p env attack
    editDetails()->page()->addKnob(2, 3, 1, 1, "10", hex1,  hex2, "10", "normal_ratio1.25");  // p env depth
    editDetails()->page()->addKnob(2, 4, 1, 1, "10", hex1,  hex2, "11", "normal_ratio1.25");  // p bend depth
    editDetails()->page()->addKnob(2, 5, 1, 1, "10", hex1,  hex2, "12", "normal_ratio1.25");  // p bend ctl
    editDetails()->page()->addSwitch(2, 6, 1, 1, "10", hex1,  hex2, "13");                    // sync sw
    editDetails()->page()->addComboBox(2, 7, 1, 1, "10", hex1,  hex2, "17", "bottom");        // ring sw (OFF/ON/INV)
    editDetails()->page()->addSystemOverride(2, 2, 1, 4, "10", hex1,  hex2, "04", "09");
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(1, Qt::AlignTop | Qt::AlignLeft);
    editDetails()->page()->compactCurrentStackField(20, 24);
    editDetails()->page()->addComboBox(0, 0, 1, 2, "10", hex1,  hex2, "1D", "large");  // type
    editDetails()->page()->addComboBox(0, 2, 1, 2, "10", hex1,  hex2, "1E", "large");  // slope
    editDetails()->page()->addSystemOverride(0, 2, 1, 2, "10", hex1,  hex2, "1D", "04", "less_than");
    editDetails()->page()->addSwitch(1, 0, 1, 1, "10", hex1,  hex2, "1C"); // off/on
    editDetails()->page()->addKnob(1, 1, 1, 1, "10", hex1,  hex2, "1F", "normal_ratio1.25");  // cutoff
    editDetails()->page()->addKnob(1, 2, 1, 1, "10", hex1,  hex2, "20", "normal_ratio1.25");  // resonance
    editDetails()->page()->addKnob(1, 3, 1, 1, "10", hex1,  hex2, "21", "normal_ratio1.25");  // flt env attack
    editDetails()->page()->addKnob(1, 4, 1, 1, "10", hex1,  hex2, "22", "normal_ratio1.25");  // flt env depth
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(1, Qt::AlignTop | Qt::AlignLeft);
    editDetails()->page()->compactCurrentStackField(20, 24);
    editDetails()->page()->addComboBox(0, 0, 1, 2, "10", hex1,  hex2, "28", "large");  // shape
    editDetails()->page()->addKnob(1, 0, 1, 1, "10", hex1,  hex2, "27", "normal_ratio1.25");  // on/off
    editDetails()->page()->addKnob(1, 1, 1, 1, "10", hex1,  hex2, "29", "normal_ratio1.25");  // rate
    editDetails()->page()->addKnob(1, 2, 1, 1, "10", hex1,  hex2, "2A", "normal_ratio1.25");  // dynamic depth
    editDetails()->page()->addKnob(1, 3, 1, 1, "10", hex1,  hex2, "2B", "normal_ratio1.25");  // pitch depth
    editDetails()->page()->addKnob(1, 4, 1, 1, "10", hex1,  hex2, "2C", "normal_ratio1.25");  // filter depth
    editDetails()->page()->addKnob(1, 5, 1, 1, "10", hex1,  hex2, "2D", "normal_ratio1.25");  // amp depth
    editDetails()->page()->addKnob(1, 6, 1, 1, "10", hex1,  hex2, "2E", "normal_ratio1.25");  // pwm depth
    editDetails()->page()->addKnob(2, 0, 1, 1, "10", hex1,  hex2, "2F", "normal_ratio1.25");  // fade time
    editDetails()->page()->addKnob(2, 1, 1, 1, "10", hex1,  hex2, "30", "normal_ratio1.25");  // sync
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(1);
    editDetails()->page()->newGroupBox("SEQUENCER 1 - Step values shared with SEQ 1 and 2");
    editDetails()->page()->addSwitch(8, 0, 1, 1, "10", hex1,  hex2A, "1E"); // seq 1 off/on
    editDetails()->page()->addComboBox(8, 1, 1, 2, "10", hex1,  hex2, "3B"); //pitch target
    editDetails()->page()->addComboBox(8, 3, 1, 2, "10", hex1,  hex2, "3C"); //cutoff target
    editDetails()->page()->addComboBox(8, 5, 1, 2, "10", hex1,  hex2, "3D"); //level target
    editDetails()->page()->addSwitch(8, 7, 1, 1, "10", hex1,  hex2A, "1F"); // sync 1
    editDetails()->page()->addSwitch(8, 8, 1, 1, "10", hex1,  hex2A, "22"); // 1 shot 1
    editDetails()->page()->addSwitch(8, 9, 1, 2, "10", hex1,  hex2A, "23"); // turbo 1
    editDetails()->page()->addKnob(8, 11, 1, 2, "10", hex1,  hex2A, "21", "normal", "right");  // rate 1

    editDetails()->page()->addLabel(0, 1, 1, 1, "STEP 1", Qt::AlignBottom);
    editDetails()->page()->addLabel(0, 2, 1, 1, "STEP 2", Qt::AlignBottom);
    editDetails()->page()->addLabel(0, 3, 1, 1, "STEP 3", Qt::AlignBottom);
    editDetails()->page()->addLabel(0, 4, 1, 1, "STEP 4", Qt::AlignBottom);
    editDetails()->page()->addLabel(0, 5, 1, 1, "STEP 5", Qt::AlignBottom);
    editDetails()->page()->addLabel(0, 6, 1, 1, "STEP 6", Qt::AlignBottom);
    editDetails()->page()->addLabel(0, 7, 1, 1, "STEP 7", Qt::AlignBottom);
    editDetails()->page()->addLabel(0, 8, 1, 1, "STEP 8", Qt::AlignBottom);
    editDetails()->page()->addLabel(0, 9, 1, 1, "STEP 9", Qt::AlignBottom);
    editDetails()->page()->addLabel(0, 10, 1, 1, "STEP 10", Qt::AlignBottom);
    editDetails()->page()->addLabel(0, 11, 1, 1, "STEP 11", Qt::AlignBottom);
    editDetails()->page()->addLabel(0, 12, 1, 1, "STEP 12", Qt::AlignBottom);
    editDetails()->page()->addLabel(0, 13, 1, 1, "STEP 13", Qt::AlignBottom);
    editDetails()->page()->addLabel(0, 14, 1, 1, "STEP 14", Qt::AlignBottom);
    editDetails()->page()->addLabel(0, 15, 1, 1, "STEP 15", Qt::AlignBottom);
    editDetails()->page()->addLabel(0, 16, 1, 1, "STEP 16", Qt::AlignBottom);

    editDetails()->page()->addLabel(1, 0, 1, 1, "Pitch Max ", Qt::AlignLeft);
    editDetails()->page()->addLabel(2, 0, 1, 1, "Pitch Min ", Qt::AlignLeft);
    editDetails()->page()->addSpinBox(2, 1, 1, 1, "10", hex1,  hex2, "3E");  //1 min pitch step
    editDetails()->page()->addSpinBox(1, 1, 1, 1, "10", hex1,  hex2, "3F");  //1 max
    editDetails()->page()->addSpinBox(2, 2, 1, 1, "10", hex1,  hex2, "40");  //2 min
    editDetails()->page()->addSpinBox(1, 2, 1, 1, "10", hex1,  hex2, "41");  //2 max
    editDetails()->page()->addSpinBox(2, 3, 1, 1, "10", hex1,  hex2, "42");  //3 min
    editDetails()->page()->addSpinBox(1, 3, 1, 1, "10", hex1,  hex2, "43");  //3 max
    editDetails()->page()->addSpinBox(2, 4, 1, 1, "10", hex1,  hex2, "44");  //4 min
    editDetails()->page()->addSpinBox(1, 4, 1, 1, "10", hex1,  hex2, "45");  //4 max
    editDetails()->page()->addSpinBox(2, 5, 1, 1, "10", hex1,  hex2, "46");  //5 min
    editDetails()->page()->addSpinBox(1, 5, 1, 1, "10", hex1,  hex2, "47");  //5 max
    editDetails()->page()->addSpinBox(2, 6, 1, 1, "10", hex1,  hex2, "48"); //6 min
    editDetails()->page()->addSpinBox(1, 6, 1, 1, "10", hex1,  hex2, "49"); //6 max
    editDetails()->page()->addSpinBox(2, 7, 1, 1, "10", hex1,  hex2, "4A"); //7 min
    editDetails()->page()->addSpinBox(1, 7, 1, 1, "10", hex1,  hex2, "4B"); //7 max
    editDetails()->page()->addSpinBox(2, 8, 1, 1, "10", hex1,  hex2, "4C"); //8 min
    editDetails()->page()->addSpinBox(1, 8, 1, 1, "10", hex1,  hex2, "4D"); //8 max
    editDetails()->page()->addSpinBox(2, 9, 1, 1, "10", hex1,  hex2, "4E"); //9 min
    editDetails()->page()->addSpinBox(1, 9, 1, 1, "10", hex1,  hex2, "4F"); //9 max
    editDetails()->page()->addSpinBox(2, 10, 1, 1, "10", hex1,  hex2, "50"); //10 min
    editDetails()->page()->addSpinBox(1, 10, 1, 1, "10", hex1,  hex2, "51"); //10 max
    editDetails()->page()->addSpinBox(2, 11, 1, 1, "10", hex1,  hex2, "52"); //11 min
    editDetails()->page()->addSpinBox(1, 11, 1, 1, "10", hex1,  hex2, "53"); //11 max
    editDetails()->page()->addSpinBox(2, 12, 1, 1, "10", hex1,  hex2, "54"); //12 min
    editDetails()->page()->addSpinBox(1, 12, 1, 1, "10", hex1,  hex2, "55"); //12 max
    editDetails()->page()->addSpinBox(2, 13, 1, 1, "10", hex1,  hex2, "56"); //13 min
    editDetails()->page()->addSpinBox(1, 13, 1, 1, "10", hex1,  hex2, "57"); //13 max
    editDetails()->page()->addSpinBox(2, 14, 1, 1, "10", hex1,  hex2, "58"); //14 min
    editDetails()->page()->addSpinBox(1, 14, 1, 1, "10", hex1,  hex2, "59"); //14 max
    editDetails()->page()->addSpinBox(2, 15, 1, 1, "10", hex1,  hex2, "5A"); //15 min
    editDetails()->page()->addSpinBox(1, 15, 1, 1, "10", hex1,  hex2, "5B"); //15 max
    editDetails()->page()->addSpinBox(2, 16, 1, 1, "10", hex1,  hex2, "5C"); //16 min
    editDetails()->page()->addSpinBox(1, 16, 1, 1, "10", hex1,  hex2, "5D"); //16 max

    editDetails()->page()->addLabel(3, 0, 1, 1, "Cutoff Max ", Qt::AlignLeft);
    editDetails()->page()->addLabel(4, 0, 1, 1, "Cutoff Min ", Qt::AlignLeft);
    editDetails()->page()->addSpinBox(4, 1, 1, 1, "10", hex1,  hex2, "5E");  //1 min filter cutoff step
    editDetails()->page()->addSpinBox(3, 1, 1, 1, "10", hex1,  hex2, "5F");  //1 max
    editDetails()->page()->addSpinBox(4, 2, 1, 1, "10", hex1,  hex2, "60");  //2 min
    editDetails()->page()->addSpinBox(3, 2, 1, 1, "10", hex1,  hex2, "61");  //2 max
    editDetails()->page()->addSpinBox(4, 3, 1, 1, "10", hex1,  hex2, "62");  //3 min
    editDetails()->page()->addSpinBox(3, 3, 1, 1, "10", hex1,  hex2, "63");  //3 max
    editDetails()->page()->addSpinBox(4, 4, 1, 1, "10", hex1,  hex2, "64");  //4 min
    editDetails()->page()->addSpinBox(3, 4, 1, 1, "10", hex1,  hex2, "65");  //4 max
    editDetails()->page()->addSpinBox(4, 5, 1, 1, "10", hex1,  hex2, "66");  //5 min
    editDetails()->page()->addSpinBox(3, 5, 1, 1, "10", hex1,  hex2, "67");  //5 max
    editDetails()->page()->addSpinBox(4, 6, 1, 1, "10", hex1,  hex2, "68");  //6 min
    editDetails()->page()->addSpinBox(3, 6, 1, 1, "10", hex1,  hex2, "69"); //6 max
    editDetails()->page()->addSpinBox(4, 7, 1, 1, "10", hex1,  hex2, "6A"); //7 min
    editDetails()->page()->addSpinBox(3, 7, 1, 1, "10", hex1,  hex2, "6B"); //7 max
    editDetails()->page()->addSpinBox(4, 8, 1, 1, "10", hex1,  hex2, "6C"); //8 min
    editDetails()->page()->addSpinBox(3, 8, 1, 1, "10", hex1,  hex2, "6D"); //8 max
    editDetails()->page()->addSpinBox(4, 9, 1, 1, "10", hex1,  hex2, "6E"); //9 min
    editDetails()->page()->addSpinBox(3, 9, 1, 1, "10", hex1,  hex2, "6F"); //9 max
    editDetails()->page()->addSpinBox(4, 10, 1, 1, "10", hex1,  hex2, "70"); //10 min
    editDetails()->page()->addSpinBox(3, 10, 1, 1, "10", hex1,  hex2, "71"); //10 max
    editDetails()->page()->addSpinBox(4, 11, 1, 1, "10", hex1,  hex2, "72"); //11 min
    editDetails()->page()->addSpinBox(3, 11, 1, 1, "10", hex1,  hex2, "73"); //11 max
    editDetails()->page()->addSpinBox(4, 12, 1, 1, "10", hex1,  hex2, "74"); //12 min
    editDetails()->page()->addSpinBox(3, 12, 1, 1, "10", hex1,  hex2, "75"); //12 max
    editDetails()->page()->addSpinBox(4, 13, 1, 1, "10", hex1,  hex2, "76"); //13 min
    editDetails()->page()->addSpinBox(3, 13, 1, 1, "10", hex1,  hex2, "77"); //13 max
    editDetails()->page()->addSpinBox(4, 14, 1, 1, "10", hex1,  hex2, "78"); //14 min
    editDetails()->page()->addSpinBox(3, 14, 1, 1, "10", hex1,  hex2, "79"); //14 max
    editDetails()->page()->addSpinBox(4, 15, 1, 1, "10", hex1,  hex2, "7A"); //15 min
    editDetails()->page()->addSpinBox(3, 15, 1, 1, "10", hex1,  hex2, "7B"); //15 max
    editDetails()->page()->addSpinBox(4, 16, 1, 1, "10", hex1,  hex2, "7C"); //16 min
    editDetails()->page()->addSpinBox(3, 16, 1, 1, "10", hex1,  hex2, "7D"); //16 max

    editDetails()->page()->addLabel(5, 0, 1, 1, "Level Max ", Qt::AlignLeft);
    editDetails()->page()->addLabel(6, 0, 1, 1, "Level Min ", Qt::AlignLeft);
    editDetails()->page()->addSpinBox(6, 1, 1, 1, "10", hex1,  hex2, "7E");  //1 min inst level step
    editDetails()->page()->addSpinBox(5, 1, 1, 1, "10", hex1,  hex2, "7F");  //1 max
    editDetails()->page()->addSpinBox(6, 2, 1, 1, "10", hex1,  hex2A, "00");  //2 min
    editDetails()->page()->addSpinBox(5, 2, 1, 1, "10", hex1,  hex2A, "01");  //2 max
    editDetails()->page()->addSpinBox(6, 3, 1, 1, "10", hex1,  hex2A, "02");  //3 min
    editDetails()->page()->addSpinBox(5, 3, 1, 1, "10", hex1,  hex2A, "03");  //3 max
    editDetails()->page()->addSpinBox(6, 4, 1, 1, "10", hex1,  hex2A, "04");  //4 min
    editDetails()->page()->addSpinBox(5, 4, 1, 1, "10", hex1,  hex2A, "05");  //4 max
    editDetails()->page()->addSpinBox(6, 5, 1, 1, "10", hex1,  hex2A, "06");  //5 min
    editDetails()->page()->addSpinBox(5, 5, 1, 1, "10", hex1,  hex2A, "07");  //5 max
    editDetails()->page()->addSpinBox(6, 6, 1, 1, "10", hex1,  hex2A, "08"); //6 min
    editDetails()->page()->addSpinBox(5, 6, 1, 1, "10", hex1,  hex2A, "09"); //6 max
    editDetails()->page()->addSpinBox(6, 7, 1, 1, "10", hex1,  hex2A, "0A"); //7 min
    editDetails()->page()->addSpinBox(5, 7, 1, 1, "10", hex1,  hex2A, "0B"); //7 max
    editDetails()->page()->addSpinBox(6, 8, 1, 1, "10", hex1,  hex2A, "0C"); //8 min
    editDetails()->page()->addSpinBox(5, 8, 1, 1, "10", hex1,  hex2A, "0D"); //8 max
    editDetails()->page()->addSpinBox(6, 9, 1, 1, "10", hex1,  hex2A, "0E"); //9 min
    editDetails()->page()->addSpinBox(5, 9, 1, 1, "10", hex1,  hex2A, "0F"); //9 max
    editDetails()->page()->addSpinBox(6, 10, 1, 1, "10", hex1,  hex2A, "10"); //10 min
    editDetails()->page()->addSpinBox(5, 10, 1, 1, "10", hex1,  hex2A, "11"); //10 max
    editDetails()->page()->addSpinBox(6, 11, 1, 1, "10", hex1,  hex2A, "12"); //11 min
    editDetails()->page()->addSpinBox(5, 11, 1, 1, "10", hex1,  hex2A, "13"); //11 max
    editDetails()->page()->addSpinBox(6, 12, 1, 1, "10", hex1,  hex2A, "14"); //12 min
    editDetails()->page()->addSpinBox(5, 12, 1, 1, "10", hex1,  hex2A, "15"); //12 max
    editDetails()->page()->addSpinBox(6, 13, 1, 1, "10", hex1,  hex2A, "16"); //13 min
    editDetails()->page()->addSpinBox(5, 13, 1, 1, "10", hex1,  hex2A, "17"); //13 max
    editDetails()->page()->addSpinBox(6, 14, 1, 1, "10", hex1,  hex2A, "18"); //14 min
    editDetails()->page()->addSpinBox(5, 14, 1, 1, "10", hex1,  hex2A, "19"); //14 max
    editDetails()->page()->addSpinBox(6, 15, 1, 1, "10", hex1,  hex2A, "1A"); //15 min
    editDetails()->page()->addSpinBox(5, 15, 1, 1, "10", hex1,  hex2A, "1B"); //15 max
    editDetails()->page()->addSpinBox(6, 16, 1, 1, "10", hex1,  hex2A, "1C"); //16 min
    editDetails()->page()->addSpinBox(5, 16, 1, 1, "10", hex1,  hex2A, "1D"); //16 max

    editDetails()->page()->addLabel(7, 0, 1, 1, "Seq Curve ", Qt::AlignLeft);
    editDetails()->page()->addComboBox(7, 1, 1, 1, "10", hex1,  hex2A, "24", "icon_seq");  //1 curve seq 1
    editDetails()->page()->addComboBox(7, 2, 1, 1, "10", hex1,  hex2A, "25", "icon_seq");  //2
    editDetails()->page()->addComboBox(7, 3, 1, 1, "10", hex1,  hex2A, "26", "icon_seq");  //3
    editDetails()->page()->addComboBox(7, 4, 1, 1, "10", hex1,  hex2A, "27", "icon_seq");  //4
    editDetails()->page()->addComboBox(7, 5, 1, 1, "10", hex1,  hex2A, "28", "icon_seq");  //5
    editDetails()->page()->addComboBox(7, 6, 1, 1, "10", hex1,  hex2A, "29", "icon_seq");  //6
    editDetails()->page()->addComboBox(7, 7, 1, 1, "10", hex1,  hex2A, "2A", "icon_seq");  //7
    editDetails()->page()->addComboBox(7, 8, 1, 1, "10", hex1,  hex2A, "2B", "icon_seq");  //8
    editDetails()->page()->addComboBox(7, 9, 1, 1, "10", hex1,  hex2A, "2C", "icon_seq");  //9
    editDetails()->page()->addComboBox(7, 10, 1, 1, "10", hex1,  hex2A, "2D", "icon_seq"); //10
    editDetails()->page()->addComboBox(7, 11, 1, 1, "10", hex1,  hex2A, "2E", "icon_seq"); //11
    editDetails()->page()->addComboBox(7, 12, 1, 1, "10", hex1,  hex2A, "2F", "icon_seq"); //12
    editDetails()->page()->addComboBox(7, 13, 1, 1, "10", hex1,  hex2A, "30", "icon_seq"); //13
    editDetails()->page()->addComboBox(7, 14, 1, 1, "10", hex1,  hex2A, "31", "icon_seq"); //14
    editDetails()->page()->addComboBox(7, 15, 1, 1, "10", hex1,  hex2A, "32", "icon_seq"); //15
    editDetails()->page()->addComboBox(7, 16, 1, 1, "10", hex1,  hex2A, "33", "icon_seq"); //16

    editDetails()->page()->addSystemOverride(0, 2, 8, 15, "10", hex1,  hex2A, "20", "01");
    editDetails()->page()->addSystemOverride(0, 3, 8, 14, "10", hex1,  hex2A, "20", "02");
    editDetails()->page()->addSystemOverride(0, 4, 8, 13, "10", hex1,  hex2A, "20", "03");
    editDetails()->page()->addSystemOverride(0, 5, 8, 12, "10", hex1,  hex2A, "20", "04");
    editDetails()->page()->addSystemOverride(0, 6, 8, 11, "10", hex1,  hex2A, "20", "05");
    editDetails()->page()->addSystemOverride(0, 7, 8, 10, "10", hex1,  hex2A, "20", "06");
    editDetails()->page()->addSystemOverride(0, 8, 8, 9, "10", hex1,  hex2A, "20", "07");
    editDetails()->page()->addSystemOverride(0, 9, 8, 8, "10", hex1,  hex2A, "20", "08");
    editDetails()->page()->addSystemOverride(0, 10, 8, 7, "10", hex1,  hex2A, "20", "09");
    editDetails()->page()->addSystemOverride(0, 11, 8, 6, "10", hex1,  hex2A, "20", "0A");
    editDetails()->page()->addSystemOverride(0, 12, 8, 5, "10", hex1,  hex2A, "20", "0B");
    editDetails()->page()->addSystemOverride(0, 13, 8, 4, "10", hex1,  hex2A, "20", "0C");
    editDetails()->page()->addSystemOverride(0, 14, 8, 3, "10", hex1,  hex2A, "20", "0D");
    editDetails()->page()->addSystemOverride(0, 15, 8, 2, "10", hex1,  hex2A, "20", "0E");
    editDetails()->page()->addSystemOverride(0, 16, 8, 1, "10", hex1,  hex2A, "20", "0F");
    editDetails()->page()->addKnob(8, 13, 1, 2, "10", hex1,  hex2A, "20", "normal", "right");  // SEQ 1 length.
    editDetails()->page()->addGroupBox(0, 0, 1, 1);
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(1);
    editDetails()->page()->newGroupBox("SEQUENCER 2 - Step values shared with SEQ 1 and 2");
    editDetails()->page()->addSwitch(8, 0, 1, 1, "10", hex1,  hex2A, "34"); // seq 2 off/on
    editDetails()->page()->addComboBox(8, 1, 1, 2, "10", hex1,  hex2, "3B"); //pitch target
    editDetails()->page()->addComboBox(8, 3, 1, 2, "10", hex1,  hex2, "3C"); //cutoff target
    editDetails()->page()->addComboBox(8, 5, 1, 2, "10", hex1,  hex2, "3D"); //level target
    editDetails()->page()->addSwitch(8, 7, 1, 1, "10", hex1,  hex2A, "35"); // sync 2
    editDetails()->page()->addSwitch(8, 8, 1, 1, "10", hex1,  hex2A, "38"); // 1 shot 2
    editDetails()->page()->addSwitch(8, 9, 1, 2, "10", hex1,  hex2A, "39"); // turbo 2
    editDetails()->page()->addKnob(8, 11, 1, 2, "10", hex1,  hex2A, "37", "normal", "right");  // rate 2

    editDetails()->page()->addLabel(0, 1, 1, 1, "STEP 1", Qt::AlignBottom);
    editDetails()->page()->addLabel(0, 2, 1, 1, "STEP 2", Qt::AlignBottom);
    editDetails()->page()->addLabel(0, 3, 1, 1, "STEP 3", Qt::AlignBottom);
    editDetails()->page()->addLabel(0, 4, 1, 1, "STEP 4", Qt::AlignBottom);
    editDetails()->page()->addLabel(0, 5, 1, 1, "STEP 5", Qt::AlignBottom);
    editDetails()->page()->addLabel(0, 6, 1, 1, "STEP 6", Qt::AlignBottom);
    editDetails()->page()->addLabel(0, 7, 1, 1, "STEP 7", Qt::AlignBottom);
    editDetails()->page()->addLabel(0, 8, 1, 1, "STEP 8", Qt::AlignBottom);
    editDetails()->page()->addLabel(0, 9, 1, 1, "STEP 9", Qt::AlignBottom);
    editDetails()->page()->addLabel(0, 10, 1, 1, "STEP 10", Qt::AlignBottom);
    editDetails()->page()->addLabel(0, 11, 1, 1, "STEP 11", Qt::AlignBottom);
    editDetails()->page()->addLabel(0, 12, 1, 1, "STEP 12", Qt::AlignBottom);
    editDetails()->page()->addLabel(0, 13, 1, 1, "STEP 13", Qt::AlignBottom);
    editDetails()->page()->addLabel(0, 14, 1, 1, "STEP 14", Qt::AlignBottom);
    editDetails()->page()->addLabel(0, 15, 1, 1, "STEP 15", Qt::AlignBottom);
    editDetails()->page()->addLabel(0, 16, 1, 1, "STEP 16", Qt::AlignBottom);

    editDetails()->page()->addLabel(1, 0, 1, 1, "Pitch Max", Qt::AlignLeft);
    editDetails()->page()->addLabel(2, 0, 1, 1, "Pitch Min", Qt::AlignLeft);
    editDetails()->page()->addSpinBox(2, 1, 1, 1, "10", hex1,  hex2, "3E");  //1 min pitch step
    editDetails()->page()->addSpinBox(1, 1, 1, 1, "10", hex1,  hex2, "3F");  //1 max
    editDetails()->page()->addSpinBox(2, 2, 1, 1, "10", hex1,  hex2, "40");  //2 min
    editDetails()->page()->addSpinBox(1, 2, 1, 1, "10", hex1,  hex2, "41");  //2 max
    editDetails()->page()->addSpinBox(2, 3, 1, 1, "10", hex1,  hex2, "42");  //3 min
    editDetails()->page()->addSpinBox(1, 3, 1, 1, "10", hex1,  hex2, "43");  //3 max
    editDetails()->page()->addSpinBox(2, 4, 1, 1, "10", hex1,  hex2, "44");  //4 min
    editDetails()->page()->addSpinBox(1, 4, 1, 1, "10", hex1,  hex2, "45");  //4 max
    editDetails()->page()->addSpinBox(2, 5, 1, 1, "10", hex1,  hex2, "46");  //5 min
    editDetails()->page()->addSpinBox(1, 5, 1, 1, "10", hex1,  hex2, "47");  //5 max
    editDetails()->page()->addSpinBox(2, 6, 1, 1, "10", hex1,  hex2, "48"); //6 min
    editDetails()->page()->addSpinBox(1, 6, 1, 1, "10", hex1,  hex2, "49"); //6 max
    editDetails()->page()->addSpinBox(2, 7, 1, 1, "10", hex1,  hex2, "4A"); //7 min
    editDetails()->page()->addSpinBox(1, 7, 1, 1, "10", hex1,  hex2, "4B"); //7 max
    editDetails()->page()->addSpinBox(2, 8, 1, 1, "10", hex1,  hex2, "4C"); //8 min
    editDetails()->page()->addSpinBox(1, 8, 1, 1, "10", hex1,  hex2, "4D"); //8 max
    editDetails()->page()->addSpinBox(2, 9, 1, 1, "10", hex1,  hex2, "4E"); //9 min
    editDetails()->page()->addSpinBox(1, 9, 1, 1, "10", hex1,  hex2, "4F"); //9 max
    editDetails()->page()->addSpinBox(2, 10, 1, 1, "10", hex1,  hex2, "50"); //10 min
    editDetails()->page()->addSpinBox(1, 10, 1, 1, "10", hex1,  hex2, "51"); //10 max
    editDetails()->page()->addSpinBox(2, 11, 1, 1, "10", hex1,  hex2, "52"); //11 min
    editDetails()->page()->addSpinBox(1, 11, 1, 1, "10", hex1,  hex2, "53"); //11 max
    editDetails()->page()->addSpinBox(2, 12, 1, 1, "10", hex1,  hex2, "54"); //12 min
    editDetails()->page()->addSpinBox(1, 12, 1, 1, "10", hex1,  hex2, "55"); //12 max
    editDetails()->page()->addSpinBox(2, 13, 1, 1, "10", hex1,  hex2, "56"); //13 min
    editDetails()->page()->addSpinBox(1, 13, 1, 1, "10", hex1,  hex2, "57"); //13 max
    editDetails()->page()->addSpinBox(2, 14, 1, 1, "10", hex1,  hex2, "58"); //14 min
    editDetails()->page()->addSpinBox(1, 14, 1, 1, "10", hex1,  hex2, "59"); //14 max
    editDetails()->page()->addSpinBox(2, 15, 1, 1, "10", hex1,  hex2, "5A"); //15 min
    editDetails()->page()->addSpinBox(1, 15, 1, 1, "10", hex1,  hex2, "5B"); //15 max
    editDetails()->page()->addSpinBox(2, 16, 1, 1, "10", hex1,  hex2, "5C"); //16 min
    editDetails()->page()->addSpinBox(1, 16, 1, 1, "10", hex1,  hex2, "5D"); //16 max

    editDetails()->page()->addLabel(3, 0, 1, 1, "Cutoff Max", Qt::AlignLeft);
    editDetails()->page()->addLabel(4, 0, 1, 1, "Cutoff Min", Qt::AlignLeft);
    editDetails()->page()->addSpinBox(4, 1, 1, 1, "10", hex1,  hex2, "5E");  //1 min filter cutoff step
    editDetails()->page()->addSpinBox(3, 1, 1, 1, "10", hex1,  hex2, "5F");  //1 max
    editDetails()->page()->addSpinBox(4, 2, 1, 1, "10", hex1,  hex2, "60");  //2 min
    editDetails()->page()->addSpinBox(3, 2, 1, 1, "10", hex1,  hex2, "61");  //2 max
    editDetails()->page()->addSpinBox(4, 3, 1, 1, "10", hex1,  hex2, "62");  //3 min
    editDetails()->page()->addSpinBox(3, 3, 1, 1, "10", hex1,  hex2, "63");  //3 max
    editDetails()->page()->addSpinBox(4, 4, 1, 1, "10", hex1,  hex2, "64");  //4 min
    editDetails()->page()->addSpinBox(3, 4, 1, 1, "10", hex1,  hex2, "65");  //4 max
    editDetails()->page()->addSpinBox(4, 5, 1, 1, "10", hex1,  hex2, "66");  //5 min
    editDetails()->page()->addSpinBox(3, 5, 1, 1, "10", hex1,  hex2, "67");  //5 max
    editDetails()->page()->addSpinBox(4, 6, 1, 1, "10", hex1,  hex2, "68");  //6 min
    editDetails()->page()->addSpinBox(3, 6, 1, 1, "10", hex1,  hex2, "69"); //6 max
    editDetails()->page()->addSpinBox(4, 7, 1, 1, "10", hex1,  hex2, "6A"); //7 min
    editDetails()->page()->addSpinBox(3, 7, 1, 1, "10", hex1,  hex2, "6B"); //7 max
    editDetails()->page()->addSpinBox(4, 8, 1, 1, "10", hex1,  hex2, "6C"); //8 min
    editDetails()->page()->addSpinBox(3, 8, 1, 1, "10", hex1,  hex2, "6D"); //8 max
    editDetails()->page()->addSpinBox(4, 9, 1, 1, "10", hex1,  hex2, "6E"); //9 min
    editDetails()->page()->addSpinBox(3, 9, 1, 1, "10", hex1,  hex2, "6F"); //9 max
    editDetails()->page()->addSpinBox(4, 10, 1, 1, "10", hex1,  hex2, "70"); //10 min
    editDetails()->page()->addSpinBox(3, 10, 1, 1, "10", hex1,  hex2, "71"); //10 max
    editDetails()->page()->addSpinBox(4, 11, 1, 1, "10", hex1,  hex2, "72"); //11 min
    editDetails()->page()->addSpinBox(3, 11, 1, 1, "10", hex1,  hex2, "73"); //11 max
    editDetails()->page()->addSpinBox(4, 12, 1, 1, "10", hex1,  hex2, "74"); //12 min
    editDetails()->page()->addSpinBox(3, 12, 1, 1, "10", hex1,  hex2, "75"); //12 max
    editDetails()->page()->addSpinBox(4, 13, 1, 1, "10", hex1,  hex2, "76"); //13 min
    editDetails()->page()->addSpinBox(3, 13, 1, 1, "10", hex1,  hex2, "77"); //13 max
    editDetails()->page()->addSpinBox(4, 14, 1, 1, "10", hex1,  hex2, "78"); //14 min
    editDetails()->page()->addSpinBox(3, 14, 1, 1, "10", hex1,  hex2, "79"); //14 max
    editDetails()->page()->addSpinBox(4, 15, 1, 1, "10", hex1,  hex2, "7A"); //15 min
    editDetails()->page()->addSpinBox(3, 15, 1, 1, "10", hex1,  hex2, "7B"); //15 max
    editDetails()->page()->addSpinBox(4, 16, 1, 1, "10", hex1,  hex2, "7C"); //16 min
    editDetails()->page()->addSpinBox(3, 16, 1, 1, "10", hex1,  hex2, "7D"); //16 max

    editDetails()->page()->addLabel(5, 0, 1, 1, "Level Max", Qt::AlignLeft);
    editDetails()->page()->addLabel(6, 0, 1, 1, "Level Min", Qt::AlignLeft);
    editDetails()->page()->addSpinBox(6, 1, 1, 1, "10", hex1,  hex2, "7E");  //1 min inst level step
    editDetails()->page()->addSpinBox(5, 1, 1, 1, "10", hex1,  hex2, "7F");  //1 max
    editDetails()->page()->addSpinBox(6, 2, 1, 1, "10", hex1,  hex2A, "00");  //2 min
    editDetails()->page()->addSpinBox(5, 2, 1, 1, "10", hex1,  hex2A, "01");  //2 max
    editDetails()->page()->addSpinBox(6, 3, 1, 1, "10", hex1,  hex2A, "02");  //3 min
    editDetails()->page()->addSpinBox(5, 3, 1, 1, "10", hex1,  hex2A, "03");  //3 max
    editDetails()->page()->addSpinBox(6, 4, 1, 1, "10", hex1,  hex2A, "04");  //4 min
    editDetails()->page()->addSpinBox(5, 4, 1, 1, "10", hex1,  hex2A, "05");  //4 max
    editDetails()->page()->addSpinBox(6, 5, 1, 1, "10", hex1,  hex2A, "06");  //5 min
    editDetails()->page()->addSpinBox(5, 5, 1, 1, "10", hex1,  hex2A, "07");  //5 max
    editDetails()->page()->addSpinBox(6, 6, 1, 1, "10", hex1,  hex2A, "08"); //6 min
    editDetails()->page()->addSpinBox(5, 6, 1, 1, "10", hex1,  hex2A, "09"); //6 max
    editDetails()->page()->addSpinBox(6, 7, 1, 1, "10", hex1,  hex2A, "0A"); //7 min
    editDetails()->page()->addSpinBox(5, 7, 1, 1, "10", hex1,  hex2A, "0B"); //7 max
    editDetails()->page()->addSpinBox(6, 8, 1, 1, "10", hex1,  hex2A, "0C"); //8 min
    editDetails()->page()->addSpinBox(5, 8, 1, 1, "10", hex1,  hex2A, "0D"); //8 max
    editDetails()->page()->addSpinBox(6, 9, 1, 1, "10", hex1,  hex2A, "0E"); //9 min
    editDetails()->page()->addSpinBox(5, 9, 1, 1, "10", hex1,  hex2A, "0F"); //9 max
    editDetails()->page()->addSpinBox(6, 10, 1, 1, "10", hex1,  hex2A, "10"); //10 min
    editDetails()->page()->addSpinBox(5, 10, 1, 1, "10", hex1,  hex2A, "11"); //10 max
    editDetails()->page()->addSpinBox(6, 11, 1, 1, "10", hex1,  hex2A, "12"); //11 min
    editDetails()->page()->addSpinBox(5, 11, 1, 1, "10", hex1,  hex2A, "13"); //11 max
    editDetails()->page()->addSpinBox(6, 12, 1, 1, "10", hex1,  hex2A, "14"); //12 min
    editDetails()->page()->addSpinBox(5, 12, 1, 1, "10", hex1,  hex2A, "15"); //12 max
    editDetails()->page()->addSpinBox(6, 13, 1, 1, "10", hex1,  hex2A, "16"); //13 min
    editDetails()->page()->addSpinBox(5, 13, 1, 1, "10", hex1,  hex2A, "17"); //13 max
    editDetails()->page()->addSpinBox(6, 14, 1, 1, "10", hex1,  hex2A, "18"); //14 min
    editDetails()->page()->addSpinBox(5, 14, 1, 1, "10", hex1,  hex2A, "19"); //14 max
    editDetails()->page()->addSpinBox(6, 15, 1, 1, "10", hex1,  hex2A, "1A"); //15 min
    editDetails()->page()->addSpinBox(5, 15, 1, 1, "10", hex1,  hex2A, "1B"); //15 max
    editDetails()->page()->addSpinBox(6, 16, 1, 1, "10", hex1,  hex2A, "1C"); //16 min
    editDetails()->page()->addSpinBox(5, 16, 1, 1, "10", hex1,  hex2A, "1D"); //16 max

    editDetails()->page()->addLabel(7, 0, 1, 1, "Seq Curve", Qt::AlignLeft);
    editDetails()->page()->addComboBox(7, 1, 1, 1, "10", hex1,  hex2A, "3A", "icon_seq");  //1 curve seq 2
    editDetails()->page()->addComboBox(7, 2, 1, 1, "10", hex1,  hex2A, "3B", "icon_seq");  //2
    editDetails()->page()->addComboBox(7, 3, 1, 1, "10", hex1,  hex2A, "3C", "icon_seq");  //3
    editDetails()->page()->addComboBox(7, 4, 1, 1, "10", hex1,  hex2A, "3D", "icon_seq");  //4
    editDetails()->page()->addComboBox(7, 5, 1, 1, "10", hex1,  hex2A, "3E", "icon_seq");  //5
    editDetails()->page()->addComboBox(7, 6, 1, 1, "10", hex1,  hex2A, "3F", "icon_seq");  //6
    editDetails()->page()->addComboBox(7, 7, 1, 1, "10", hex1,  hex2A, "40", "icon_seq");  //7
    editDetails()->page()->addComboBox(7, 8, 1, 1, "10", hex1,  hex2A, "41", "icon_seq");  //8
    editDetails()->page()->addComboBox(7, 9, 1, 1, "10", hex1,  hex2A, "42", "icon_seq");  //9
    editDetails()->page()->addComboBox(7, 10, 1, 1, "10", hex1,  hex2A, "43", "icon_seq"); //10
    editDetails()->page()->addComboBox(7, 11, 1, 1, "10", hex1,  hex2A, "44", "icon_seq"); //11
    editDetails()->page()->addComboBox(7, 12, 1, 1, "10", hex1,  hex2A, "45", "icon_seq"); //12
    editDetails()->page()->addComboBox(7, 13, 1, 1, "10", hex1,  hex2A, "46", "icon_seq"); //13
    editDetails()->page()->addComboBox(7, 14, 1, 1, "10", hex1,  hex2A, "47", "icon_seq"); //14
    editDetails()->page()->addComboBox(7, 15, 1, 1, "10", hex1,  hex2A, "48", "icon_seq"); //15
    editDetails()->page()->addComboBox(7, 16, 1, 1, "10", hex1,  hex2A, "49", "icon_seq"); //16
    editDetails()->page()->addSystemOverride(0, 2, 8, 15, "10", hex1,  hex2A, "36", "01");
    editDetails()->page()->addSystemOverride(0, 3, 8, 14, "10", hex1,  hex2A, "36", "02");
    editDetails()->page()->addSystemOverride(0, 4, 8, 13, "10", hex1,  hex2A, "36", "03");
    editDetails()->page()->addSystemOverride(0, 5, 8, 12, "10", hex1,  hex2A, "36", "04");
    editDetails()->page()->addSystemOverride(0, 6, 8, 11, "10", hex1,  hex2A, "36", "05");
    editDetails()->page()->addSystemOverride(0, 7, 8, 10, "10", hex1,  hex2A, "36", "06");
    editDetails()->page()->addSystemOverride(0, 8, 8, 9, "10", hex1,  hex2A, "36", "07");
    editDetails()->page()->addSystemOverride(0, 9, 8, 8, "10", hex1,  hex2A, "36", "08");
    editDetails()->page()->addSystemOverride(0, 10, 8, 7, "10", hex1,  hex2A, "36", "09");
    editDetails()->page()->addSystemOverride(0, 11, 8, 6, "10", hex1,  hex2A, "36", "0A");
    editDetails()->page()->addSystemOverride(0, 12, 8, 5, "10", hex1,  hex2A, "36", "0B");
    editDetails()->page()->addSystemOverride(0, 13, 8, 4, "10", hex1,  hex2A, "36", "0C");
    editDetails()->page()->addSystemOverride(0, 14, 8, 3, "10", hex1,  hex2A, "36", "0D");
    editDetails()->page()->addSystemOverride(0, 15, 8, 2, "10", hex1,  hex2A, "36", "0E");
    editDetails()->page()->addSystemOverride(0, 16, 8, 1, "10", hex1,  hex2A, "36", "0F");
    editDetails()->page()->addKnob(8, 13, 1, 2, "10", hex1,  hex2A, "36", "normal", "right");  // SEQ 2 length.
    editDetails()->page()->addGroupBox(0, 0, 1, 1);
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(1, Qt::AlignTop | Qt::AlignLeft);
    editDetails()->page()->compactCurrentStackField(20, 24);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex2 = "2B";
    }
    else
    {
        hex2 = "2D";
    };
    editDetails()->page()->addComboBox(0, 0, 1, 2, "10", hex1, hex2, "4B", "large"); // lower
    editDetails()->page()->addComboBox(0, 2, 1, 2, "10", hex1, hex2, "4C", "large"); // upper
    editDetails()->page()->addDivider(1, 0, 1, 6, Qt::AlignTop | Qt::AlignLeft);
    editDetails()->page()->addKnob(2, 0, 1, 1, "10", hex1, hex2, "4A", "normal_ratio1.25");   // lower fade
    editDetails()->page()->addKnob(2, 1, 1, 1, "10", hex1, hex2, "4D", "inverse_ratio1.25");  // upper fade
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(1);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex2 = "32";
    }
    else
    {
        hex2 = "35";
    };
    altTuning();
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(1, Qt::AlignTop | Qt::AlignLeft);
    editDetails()->page()->compactCurrentStackField(20, 24);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex2 = "29";
    }
    else
    {
        hex2 = "2B";
    };
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.5");    // inst level
    editDetails()->page()->addSwitch(0, 1, 1, 1, "10", hex1, hex2, "03");                       // nor mix sw
    editDetails()->page()->addDataKnob(0, 2, 1, 1, "10", hex1, hex2, "04", "0~200_ratio1.25"); // nor mix level
    stringLevels();
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(1, Qt::AlignTop | Qt::AlignLeft);
    editDetails()->page()->compactCurrentStackField(20, 24);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex2 = "2A";
    }
    else
    {
        hex2 = "2C";
    };
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "23", "normal_ratio1.25");                // amp env attack
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "25", "normal_ratio1.25", "bottom", 70);  // low cut
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "26", "normal_ratio1.25", "bottom", 70);  // high cut
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(1, Qt::AlignTop | Qt::AlignLeft);
    editDetails()->page()->compactCurrentStackField(20, 24);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex2 = "2A";
    }
    else
    {
        hex2 = "2C";
    };
    editDetails()->page()->addComboBox(0, 0, 1, 2, "10", hex1, hex2, "32", "large");  // shape
    editDetails()->page()->addKnob(1, 0, 1, 1, "10", hex1, hex2, "31", "normal_ratio1.25");  // on/off
    editDetails()->page()->addKnob(1, 1, 1, 1, "10", hex1, hex2, "33", "normal_ratio1.25");  // rate
    editDetails()->page()->addKnob(1, 2, 1, 1, "10", hex1, hex2, "34", "normal_ratio1.25");  // dynamic depth
    editDetails()->page()->addKnob(1, 3, 1, 1, "10", hex1, hex2, "35", "normal_ratio1.25");  // pitch depth
    editDetails()->page()->addKnob(1, 4, 1, 1, "10", hex1, hex2, "36", "normal_ratio1.25");  // filter depth
    editDetails()->page()->addKnob(1, 5, 1, 1, "10", hex1, hex2, "37", "normal_ratio1.25");  // amp depth
    editDetails()->page()->addKnob(1, 6, 1, 1, "10", hex1, hex2, "38", "normal_ratio1.25");  // pwm depth
    editDetails()->page()->addKnob(2, 0, 1, 1, "10", hex1, hex2, "39", "normal_ratio1.25");  // fade time
    editDetails()->page()->addKnob(2, 1, 1, 1, "10", hex1, hex2, "3A", "normal_ratio1.25");  // sync
    editDetails()->page()->addStackField();
    editDetails()->page()->setStackCurrentIndex(1, 7);


    // *********************************************************************************************************** OSC SYNTH ***********************
    editDetails()->page()->newStackField(0);
    editDetails()->page()->newStackControl(2);
    editDetails()->page()->addTabBar(0, 0, 1, 1, "Tables", "00", "00", "12"); //page select OSC synth
    editDetails()->page()->addStackControl();
    editDetails()->page()->insertStackField(2, 1, 0, 1, 1);
    editDetails()->page()->addStackField();
    // *********************************************************************************************************************************************


    editDetails()->page()->newStackField(2);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex2 = "2C";
    }
    else
    {
        hex2 = "2E";
    };
    editDetails()->page()->newStackField(2);
    editDetails()->page()->compactCurrentStackField(18, 18);
    editDetails()->page()->addComboBox(0, 0, 1, 2, "10", hex1,  hex2, "00", "left_large");
    editDetails()->page()->addComboBox(0, 2, 1, 2, "10", hex1,  hex2, "01", "left_large");
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1,  hex2, "02", "normal_ratio1.25");
    editDetails()->page()->addKnob(0, 5, 1, 1, "10", hex1,  hex2, "03", "normal");
    editDetails()->page()->addKnob(0, 6, 1, 1, "10", hex1,  hex2, "04", "normal_ratio1.25");
    editDetails()->page()->addKnob(0, 7, 1, 1, "10", hex1,  hex2, "05", "normal_ratio1.25");
    editDetails()->page()->addSystemOverride(0, 6, 1, 2, "10", hex1,  hex2, "01", "04", "not_equal");
    editDetails()->page()->addKnob(0, 8, 1, 1, "10", hex1,  hex2, "06", "normal");
    editDetails()->page()->addKnob(0, 9, 1, 1, "10", hex1,  hex2, "07", "normal");
    editDetails()->page()->addKnob(0, 10, 1, 1, "10", hex1,  hex2, "08", "normal");
    editDetails()->page()->addKnob(0, 11, 1, 1, "10", hex1,  hex2, "09", "normal_ratio1.25");
    editDetails()->page()->addDivider(1, 0, 1, 14);
    editDetails()->page()->addComboBox(2, 0, 1, 2, "10", hex1,  hex2, "13", "left_large");
    editDetails()->page()->addComboBox(2, 2, 1, 2, "10", hex1,  hex2, "18", "left_large");
    editDetails()->page()->addSwitch(2, 4, 1, 1, "10", hex1,  hex2, "15");
    editDetails()->page()->addKnob(2, 5, 1, 1, "10", hex1,  hex2, "16", "normal_ratio1.25");
    editDetails()->page()->addSwitch(2, 6, 1, 1, "10", hex1,  hex2, "14");
    editDetails()->page()->addKnob(2, 7, 1, 1, "10", hex1,  hex2, "19", "normal_ratio1.25");
    editDetails()->page()->addComboBox(2, 8, 1, 2, "10", hex1,  hex2, "0A", "left_large");
    editDetails()->page()->addKnob(2, 10, 1, 1, "10", hex1,  hex2, "0B", "normal_ratio1.25");
    editDetails()->page()->addKnob(2, 11, 1, 1, "10", hex1,  hex2, "0C", "normal");
    editDetails()->page()->addSystemOverride(2, 8, 1, 4, "10", hex1,  hex2, "00", "00");
    editDetails()->page()->addComboBox(3, 0, 1, 2, "10", hex1,  hex2, "17", "left_large");
    editDetails()->page()->addSystemOverride(3, 0, 1, 2, "10", hex1,  hex2, "13", "00");
    editDetails()->page()->addKnob(3, 8, 1, 1, "10", hex1,  hex2, "0D", "normal_ratio1.25");
    editDetails()->page()->addKnob(3, 9, 1, 1, "10", hex1,  hex2, "0E", "normal_ratio1.25");
    editDetails()->page()->addSystemOverride(3, 8, 1, 2, "10", hex1,  hex2, "00", "00");
    editDetails()->page()->addSystemOverride(3, 8, 1, 2, "10", hex1,  hex2, "0A", "04", "not_equal");
    editDetails()->page()->addKnob(3, 10, 1, 1, "10", hex1,  hex2, "0F", "normal");
    editDetails()->page()->addKnob(3, 11, 1, 1, "10", hex1,  hex2, "10", "normal");
    editDetails()->page()->addKnob(3, 12, 1, 1, "10", hex1,  hex2, "11", "normal");
    editDetails()->page()->addKnob(3, 13, 1, 1, "10", hex1,  hex2, "12", "normal_ratio1.25");
    editDetails()->page()->addSystemOverride(3, 10, 1, 4, "10", hex1,  hex2, "00", "00");
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(2);
    editDetails()->page()->compactCurrentStackField(18, 18);
    editDetails()->page()->addSwitch(0, 0, 1, 1, "10", hex1,  hex2, "1A");
    editDetails()->page()->addComboBox(0, 1, 1, 2, "10", hex1,  hex2, "1B", "left_large");
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1,  hex2, "1C", "normal_ratio1.25");
    editDetails()->page()->addSystemOverride(0, 3, 1, 1, "10", hex1,  hex2, "1B", "04", "less_than");
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1,  hex2, "1D", "normal_ratio1.25");
    editDetails()->page()->addKnob(0, 5, 1, 1, "10", hex1,  hex2, "1E", "normal_ratio1.25");
    editDetails()->page()->addKnob(0, 6, 1, 1, "10", hex1,  hex2, "1F", "normal_ratio1.25");
    editDetails()->page()->addKnob(0, 7, 1, 1, "10", hex1,  hex2, "20", "normal_ratio1.25");
    editDetails()->page()->addDivider(1, 0, 1, 8);
    editDetails()->page()->addKnob(2, 0, 1, 1, "10", hex1,  hex2, "21", "normal_ratio1.25");
    editDetails()->page()->addKnob(2, 1, 1, 1, "10", hex1,  hex2, "22", "normal_ratio1.25");
    editDetails()->page()->addKnob(2, 2, 1, 1, "10", hex1,  hex2, "23", "normal_ratio1.25");
    editDetails()->page()->addKnob(2, 3, 1, 1, "10", hex1,  hex2, "24", "normal_ratio1.25");
    editDetails()->page()->addKnob(2, 4, 1, 1, "10", hex1,  hex2, "25", "normal_ratio1.25");
    editDetails()->page()->addKnob(3, 0, 1, 1, "10", hex1,  hex2, "27", "normal_ratio1.25");
    editDetails()->page()->addKnob(3, 1, 1, 1, "10", hex1,  hex2, "28", "normal_ratio1.25");
    editDetails()->page()->addKnob(3, 2, 1, 1, "10", hex1,  hex2, "29", "normal_ratio1.25");
    editDetails()->page()->addKnob(3, 3, 1, 1, "10", hex1,  hex2, "2A", "normal_ratio1.25");
    editDetails()->page()->addKnob(3, 4, 1, 1, "10", hex1,  hex2, "26", "normal_ratio1.25");
    editDetails()->page()->addKnob(3, 5, 1, 1, "10", hex1,  hex2, "2B", "normal_ratio1.25", "bottom", 70);
    editDetails()->page()->addKnob(3, 6, 1, 1, "10", hex1,  hex2, "2C", "normal_ratio1.25", "bottom", 70);
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(2);
    editDetails()->page()->compactCurrentStackField(18, 18);
    editDetails()->page()->addSwitch(0, 0, 1, 1, "10", hex1,  hex2, "2D");
    editDetails()->page()->addComboBox(0, 1, 1, 2, "10", hex1,  hex2, "2E", "left_large");
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1,  hex2, "2F", "normal_ratio1.25");
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1,  hex2, "30", "normal_ratio1.25");
    editDetails()->page()->addKnob(0, 5, 1, 1, "10", hex1,  hex2, "31", "normal_ratio1.25");
    editDetails()->page()->addKnob(0, 6, 1, 1, "10", hex1,  hex2, "32", "normal_ratio1.25");
    editDetails()->page()->addKnob(0, 7, 1, 1, "10", hex1,  hex2, "33", "normal_ratio1.25");
    editDetails()->page()->addKnob(0, 8, 1, 1, "10", hex1,  hex2, "34", "normal_ratio1.25");
    editDetails()->page()->addKnob(0, 9, 1, 1, "10", hex1,  hex2, "35", "normal_ratio1.25");
    editDetails()->page()->addComboBox(0, 10, 1, 2, "10", hex1,  hex2, "36", "left_large");
    editDetails()->page()->addDivider(1, 0, 1, 12);
    editDetails()->page()->addSwitch(2, 0, 1, 1, "10", hex1,  hex2, "37");
    editDetails()->page()->addComboBox(2, 1, 1, 2, "10", hex1,  hex2, "38", "left_large");
    editDetails()->page()->addKnob(2, 3, 1, 1, "10", hex1,  hex2, "39", "normal_ratio1.25");
    editDetails()->page()->addKnob(2, 4, 1, 1, "10", hex1,  hex2, "3A", "normal_ratio1.25");
    editDetails()->page()->addKnob(2, 5, 1, 1, "10", hex1,  hex2, "3B", "normal_ratio1.25");
    editDetails()->page()->addKnob(2, 6, 1, 1, "10", hex1,  hex2, "3C", "normal_ratio1.25");
    editDetails()->page()->addKnob(2, 7, 1, 1, "10", hex1,  hex2, "3D", "normal_ratio1.25");
    editDetails()->page()->addKnob(2, 8, 1, 1, "10", hex1,  hex2, "3E", "normal_ratio1.25");
    editDetails()->page()->addKnob(2, 9, 1, 1, "10", hex1,  hex2, "3F", "normal_ratio1.25");
    editDetails()->page()->addComboBox(2, 10, 1, 2, "10", hex1,  hex2, "40", "left_large");
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(2);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex2 = "29";
    }
    else
    {
        hex2 = "2B";
    };
    stringLevels();
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(2);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex2 = "32";
    }
    else
    {
        hex2 = "35";
    };
    altTuning();
    editDetails()->page()->addStackField();


    // *********************************************************************************************************** GR-300 ***************
    editDetails()->page()->newStackField(10, Qt::AlignVCenter | Qt::AlignLeft);
    editDetails()->page()->addStackField();
    editDetails()->page()->newStackField(0);
    editDetails()->page()->newStackControl(3);
    editDetails()->page()->addTabBar(0, 0, 1, 1, "Tables", "00", "00", "13"); //page select GR300 synth
    editDetails()->page()->addStackControl();
    editDetails()->page()->insertStackField(3, 1, 0, 1, 1);
    editDetails()->page()->addStackField();
    // ***********************************************************************************************************************************

    editDetails()->page()->newStackField(3, Qt::AlignTop | Qt::AlignLeft);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){hex2 = "2D";}else{hex2 = "2F";};
    editDetails()->page()->compactCurrentStackField(14, 10);
    editDetails()->page()->addComboBox(0, 0, 1, 2, "10", hex1, hex2, "00", "left"); // mode
    editDetails()->page()->addSwitch(0, 2, 1, 1, "10", hex1, hex2, "01", "middle"); // comp
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.25"); // filter cutoff
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "03", "normal_ratio1.25"); // filter resonance
    editDetails()->page()->addComboBox(0, 5, 1, 2, "10", hex1, hex2, "04", "left"); // env mod sw
    editDetails()->page()->addKnob(0, 7, 1, 1, "10", hex1, hex2, "05", "normal_ratio1.25"); // env mod sens
    editDetails()->page()->addKnob(0, 8, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.25"); // env mod attack
    editDetails()->page()->addKnob(0, 9, 1, 1, "10", hex1, hex2, "13", "normal_ratio1.25", "bottom", 70); // low cut
    editDetails()->page()->addKnob(0, 10, 1, 1, "10", hex1, hex2, "14", "normal_ratio1.25", "bottom", 70); // high cut
    editDetails()->page()->addDivider(1, 0, 1, 11);
    editDetails()->page()->addComboBox(2, 0, 1, 2, "10", hex1, hex2, "07", "left"); // pitch sw
    editDetails()->page()->addKnob(2, 2, 1, 1, "10", hex1, hex2, "08", "normal_ratio1.5"); // pitch A
    editDetails()->page()->addKnob(2, 3, 1, 1, "10", hex1, hex2, "09", "normal_ratio1.25"); // pitch A fine
    editDetails()->page()->addKnob(2, 4, 1, 1, "10", hex1, hex2, "0A", "normal_ratio1.5"); // pitch B
    editDetails()->page()->addKnob(2, 5, 1, 1, "10", hex1, hex2, "0B", "normal_ratio1.25"); // pitch B fine
    editDetails()->page()->addSwitch(2, 6, 1, 1, "10", hex1, hex2, "0C", "middle"); // duet sw
    editDetails()->page()->addSwitch(2, 7, 1, 1, "10", hex1, hex2, "0D", "middle"); // sweep sw
    editDetails()->page()->addKnob(2, 8, 1, 1, "10", hex1, hex2, "0E", "normal_ratio1.25"); // sweep rise
    editDetails()->page()->addKnob(2, 9, 1, 1, "10", hex1, hex2, "0F", "normal_ratio1.25"); // sweep fall
    editDetails()->page()->addKnob(2, 10, 1, 1, "10", hex1, hex2, "11", "normal_ratio1.25"); // vibrato rate
    editDetails()->page()->addKnob(2, 11, 1, 1, "10", hex1, hex2, "12", "normal_ratio1.25"); // vibrato depth
    editDetails()->page()->addSwitch(2, 12, 1, 1, "10", hex1, hex2, "10", "middle"); // vibrato sw
    editDetails()->page()->addSystemOverride(2, 0, 1, 13, "10", hex1, hex2, "00", "02"); // mode
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(3);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){hex2 = "29";}else{hex2 = "2B";};
    stringLevels();
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(3);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){hex2 = "32";}else{hex2 = "35";};
    altTuning();
    editDetails()->page()->addStackField();


    // *********************************************************************************************************** E GUITAR ********************
    editDetails()->page()->newStackField(0);
    editDetails()->page()->newStackControl(4);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        editDetails()->page()->addTabBar(0, 0, 1, 1, "Tables", "00", "00", "16"); //page select Electric bass (bass mode)
    }
    else
    {
        editDetails()->page()->addTabBar(0, 0, 1, 1, "Tables", "00", "00", "14"); //page select Electric guitar (guitar mode)
    };
    editDetails()->page()->addStackControl();
    editDetails()->page()->insertStackField(4, 1, 0, 1, 1);
    editDetails()->page()->addStackField();
    // *****************************************************************************************************************************************


    editDetails()->page()->newStackField(4);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        editDetails()->page()->newGroupBox("Electric Bass");
        editDetails()->page()->newStackControl(5);
        editDetails()->page()->addComboBox(0, 0, 1, 2, "10", "02", "2E", "00", "large");         // type
        editDetails()->page()->addStackControl();
        editDetails()->page()->insertStackField(5, 2, 0, 1, 1);
        editDetails()->page()->addGroupBox(0, 0, 1, 9);
        editDetails()->page()->newGroupBox("Noise Suppressor");
        editDetails()->page()->addSwitch(0, 0, 1, 1, "10", "02", "2E", "2A");                    // ns off/on
        editDetails()->page()->addKnob(1, 0, 1, 1, "10", "02", "2E", "2B", "normal_ratio1.25");  // threshold
        editDetails()->page()->addKnob(2, 0, 1, 1, "10", "02", "2E", "2C", "normal_ratio1.25");  // release
        editDetails()->page()->addGroupBox(0, 9, 1, 1);

        editDetails()->page()->addStackField();

        editDetails()->page()->newStackField(5);
        editDetails()->page()->addKnob(0, 0, 1, 1, "10", "02", "2E", "01", "normal_ratio2");     // master vint J
        editDetails()->page()->addKnob(0, 1, 1, 1, "10", "02", "2E", "02", "normal_ratio1.75");  // rear vol
        editDetails()->page()->addKnob(0, 2, 1, 1, "10", "02", "2E", "03", "normal_ratio1.75");  // front vol
        editDetails()->page()->addKnob(0, 3, 1, 1, "10", "02", "2E", "15", "normal_ratio1.75");  // tone
        editDetails()->page()->addStackField();
        editDetails()->page()->newStackField(5);
        editDetails()->page()->addKnob(0, 0, 1, 1, "10", "02", "2E", "01", "normal_ratio2");     // master modern J
        editDetails()->page()->addKnob(0, 1, 1, 1, "10", "02", "2E", "02", "normal_ratio1.75");  // rear vol
        editDetails()->page()->addKnob(0, 2, 1, 1, "10", "02", "2E", "03", "normal_ratio1.75");  // front vol
        editDetails()->page()->addKnob(0, 3, 1, 1, "10", "02", "2E", "15", "normal_ratio1.75");  // tone
        editDetails()->page()->addStackField();
        editDetails()->page()->newStackField(5);
        editDetails()->page()->addKnob(0, 0, 1, 1, "10", "02", "2E", "14", "normal_ratio2");     // volume vint P
        editDetails()->page()->addKnob(0, 1, 1, 1, "10", "02", "2E", "15", "normal_ratio1.75");  // tone
        editDetails()->page()->addStackField();
        editDetails()->page()->newStackField(5);
        editDetails()->page()->addKnob(0, 0, 1, 1, "10", "02", "2E", "14", "normal_ratio2");     // volume modern P
        editDetails()->page()->addKnob(0, 1, 1, 1, "10", "02", "2E", "15", "normal_ratio1.75");  // tone
        editDetails()->page()->addStackField();
        editDetails()->page()->newStackField(5);
        editDetails()->page()->addKnob(0, 0, 1, 1, "10", "02", "2E", "14", "normal_ratio2");     // volume m man
        editDetails()->page()->addKnob(0, 1, 1, 1, "10", "02", "2E", "07", "normal_ratio1.75");  // mman bass
        editDetails()->page()->addKnob(0, 2, 1, 1, "10", "02", "2E", "06", "normal_ratio1.75");  // mman trebl
        editDetails()->page()->addStackField();
        editDetails()->page()->newStackField(5);
        editDetails()->page()->addKnob(0, 0, 1, 1, "10", "02", "2E", "01", "normal_ratio2");     // master rickenbacker
        editDetails()->page()->addKnob(0, 1, 1, 1, "10", "02", "2E", "02", "normal_ratio1.75");  // rear vol
        editDetails()->page()->addKnob(0, 2, 1, 1, "10", "02", "2E", "03", "normal_ratio1.75");  // front vol
        editDetails()->page()->addComboBox(0, 3, 1, 1, "10", "02", "2E", "0A", "large");         // pu select
        editDetails()->page()->addKnob(0, 4, 1, 1, "10", "02", "2E", "04", "normal_ratio1.75");  // rick rear tone
        editDetails()->page()->addKnob(0, 5, 1, 1, "10", "02", "2E", "05", "normal_ratio1.75");  // rick front tone
        editDetails()->page()->addStackField();
        editDetails()->page()->newStackField(5);
        editDetails()->page()->addKnob(0, 0, 1, 1, "10", "02", "2E", "01", "normal_ratio2");     // master t-bird
        editDetails()->page()->addKnob(0, 1, 1, 1, "10", "02", "2E", "02", "normal_ratio1.75");  // rear vol
        editDetails()->page()->addKnob(0, 2, 1, 1, "10", "02", "2E", "03", "normal_ratio1.75");  // front vol
        editDetails()->page()->addKnob(0, 3, 1, 1, "10", "02", "2E", "15", "normal_ratio1.75");  // tone
        editDetails()->page()->addStackField();
        editDetails()->page()->newStackField(5);
        editDetails()->page()->addKnob(0, 0, 1, 1, "10", "02", "2E", "01", "normal_ratio2");     // master active
        editDetails()->page()->addKnob(0, 1, 1, 1, "10", "02", "2E", "02", "normal_ratio1.75");  // rear vol
        editDetails()->page()->addKnob(0, 2, 1, 1, "10", "02", "2E", "03", "normal_ratio1.75");  // front vol
        editDetails()->page()->addKnob(0, 3, 1, 1, "10", "02", "2E", "09", "normal_ratio1.75");   // active bass
        editDetails()->page()->addKnob(0, 4, 1, 1, "10", "02", "2E", "08", "normal_ratio1.75");   // active treble
        editDetails()->page()->addStackField();
        editDetails()->page()->newStackField(5);
        editDetails()->page()->addKnob(0, 0, 1, 1, "10", "02", "2E", "01", "normal_ratio2");     // master violin
        editDetails()->page()->addKnob(0, 1, 1, 1, "10", "02", "2E", "02", "normal_ratio1.75");  // rear vol
        editDetails()->page()->addKnob(0, 2, 1, 1, "10", "02", "2E", "03", "normal_ratio1.75");  // front vol
        editDetails()->page()->addComboBox(0, 3, 1, 1, "10", "02", "2E", "0D", "large");         //solo
        editDetails()->page()->addSwitch(0, 4, 1, 1, "10", "02", "2E", "0C", "middle");          // bass on
        editDetails()->page()->addSwitch(0, 5, 1, 1, "10", "02", "2E", "0B", "middle");          // treble on
        editDetails()->page()->addStackField();
        editDetails()->page()->newStackField(5);
        editDetails()->page()->addComboBox(0, 0, 1, 1, "10", "02", "2E", "0E", "large");         // tone type fretless
        editDetails()->page()->addKnob(0, 1, 1, 1, "10", "02", "2E", "14", "normal_ratio2");     // volume
        editDetails()->page()->addKnob(0, 2, 1, 1, "10", "02", "2E", "15", "normal_ratio1.75");  // tone
        editDetails()->page()->addKnob(0, 3, 1, 1, "10", "02", "2E", "0F", "normal_ratio1.5");   // sense
        editDetails()->page()->addKnob(0, 4, 1, 1, "10", "02", "2E", "10", "normal_ratio1.5");   // depth
        editDetails()->page()->addKnob(0, 5, 1, 1, "10", "02", "2E", "11", "normal_ratio1.5");   // attack
        editDetails()->page()->addKnob(0, 6, 1, 1, "10", "02", "2E", "12", "normal_ratio1.5");   // resonance
        editDetails()->page()->addKnob(0, 7, 1, 1, "10", "02", "2E", "13", "normal_ratio1.5");   // direct level
        editDetails()->page()->addStackField();
    }
    else
    {
        editDetails()->page()->newGroupBox("Electric Guitar");
        editDetails()->page()->newStackControl(5);
        editDetails()->page()->addComboBox(0, 1, 1, 7, "10", "00", "30", "00", "large");         // type
        editDetails()->page()->addStackControl();
        editDetails()->page()->insertStackField(5, 8, 0, 1, 1);
        editDetails()->page()->addComboBox(1, 1, 1, 1, "10", "00", "30", "01", "pu_sel_gmode_g");// pu select
        editDetails()->page()->addSystemOverride(1, 1, 1, 1, "10", "00", "30", "00", "0B");
        editDetails()->page()->addComboBox(1, 2, 1, 1, "10", "00", "30", "00");                  // tone type
        editDetails()->page()->addKnob(1, 3, 1, 1, "10", "00", "30", "03", "normal_ratio1.25");  // sense
        editDetails()->page()->addKnob(1, 4, 1, 1, "10", "00", "30", "04", "normal_ratio1.25");  // depth
        editDetails()->page()->addKnob(1, 5, 1, 1, "10", "00", "30", "05", "normal_ratio1.25");  // attack
        editDetails()->page()->addKnob(1, 6, 1, 1, "10", "00", "30", "06", "normal_ratio1.25");  // resonance
        editDetails()->page()->addKnob(1, 7, 1, 1, "10", "00", "30", "07", "normal_ratio1.25");  // direct
        editDetails()->page()->addSystemOverride(1, 2, 1, 6, "10", "00", "30", "00", "0B", "not_equal");
        editDetails()->page()->addKnob(0, 6, 1, 1, "10", "00", "30", "08", "normal_ratio1.75");  // volume
        editDetails()->page()->addKnob(0, 7, 1, 1, "10", "00", "30", "09", "normal_ratio1.5");   // tone
        editDetails()->page()->addGroupBox(0, 0, 1, 7);
        editDetails()->page()->newGroupBox("Noise Suppressor");
        editDetails()->page()->addSwitch(0, 0, 1, 1, "10", "00", "30", "1E");                    // ns off/on
        editDetails()->page()->addKnob(0, 1, 1, 1, "10", "00", "30", "1F", "normal_ratio1.25");  // threshold
        editDetails()->page()->addKnob(0, 2, 1, 1, "10", "00", "30", "20", "normal_ratio1.25");  // release
        editDetails()->page()->addGroupBox(0, 7, 1, 3);

        editDetails()->page()->addStackField();

        editDetails()->page()->newStackField(5);
        editDetails()->page()->addStackField();
    };



    editDetails()->page()->newStackField(4);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        editDetails()->page()->newGroupBox("Amp");
        editDetails()->page()->addSwitch(0, 0, 1, 1, "10", "02", "2E", "16");                                 // off/on effect
        editDetails()->page()->addGroupBox(0, 0, 2, 1);
        editDetails()->page()->newGroupBox(tr("PreAmp"));
        editDetails()->page()->addComboBox(0, 1, 1, 5, "10", "02", "2E", "17", "large");                      //pre type
        editDetails()->page()->addComboBox(1, 1, 1, 1, "10", "02", "2E", "21", "bottom");                     //gain sw
        editDetails()->page()->addKnob(1, 2, 1, 1, "10", "02", "2E", "18", "turbo_ratio1.25");                // gain
        editDetails()->page()->addKnob(1, 3, 1, 1, "10", "02", "2E", "19", "normal_ratio1.25");               // sag
        editDetails()->page()->addKnob(1, 4, 1, 1, "10", "02", "2E", "1A", "normal_ratio1.25");               // resonance
        editDetails()->page()->addKnob(1, 5, 1, 1, "10", "02", "2E", "1C", "normal_ratio1.25");               // bass
        editDetails()->page()->addKnob(1, 6, 1, 1, "10", "02", "2E", "1D", "normal_ratio1.25");               // mid
        editDetails()->page()->addKnob(1, 7, 1, 1, "10", "02", "2E", "1E", "normal_ratio1.25");               // treble
        editDetails()->page()->addKnob(1, 8, 1, 1, "10", "02", "2E", "1F", "normal_ratio1.25");               // presence
        editDetails()->page()->addSwitch(1, 9, 1, 1, "10", "02", "2E", "20");                                 // bright button
        editDetails()->page()->addKnob(0, 10, 2, 1, "10", "02", "2E", "1B", "normal_ratio1.75");              // effect level
        editDetails()->page()->addGroupBox(0, 1, 1, 2);
        editDetails()->page()->newGroupBox(tr("Speaker"));
        editDetails()->page()->addComboBox(0, 0, 1, 1, "10", "02", "2E", "24", "large");                      // spkr type
        editDetails()->page()->addComboBox(0, 1, 1, 1, "10", "02", "2E", "25", "bottom");                     // mic type
        editDetails()->page()->addComboBox(0, 2, 1, 1, "10", "02", "2E", "26");                               // mic distance
        editDetails()->page()->addKnob(0, 3, 1, 1, "10", "02", "2E", "27", "normal_ratio1.25", "bottom", 70); // mic position
        editDetails()->page()->addKnob(0, 4, 1, 1, "10", "02", "2E", "28", "normal_ratio1.5");                // mic level
        editDetails()->page()->addKnob(0, 5, 1, 1, "10", "02", "2E", "29", "normal_ratio1.25");               // direct level
        editDetails()->page()->addGroupBox(1, 1, 1, 1);
        editDetails()->page()->newGroupBox(tr("Solo"));
        editDetails()->page()->addSwitch(0, 0, 1, 1, "10", "02", "2E", "22", "middle");                       // solo sw
        editDetails()->page()->addKnob(0, 1, 1, 1, "10", "02", "2E", "23", "normal_ratio1.25");               // solo level
        editDetails()->page()->addGroupBox(1, 2, 1, 1);
    }
    else
    {
        editDetails()->page()->newGroupBox("Amp");
        editDetails()->page()->addSwitch(0, 0, 1, 1, "10", "00", "30", "0A");   // off/on effect
        editDetails()->page()->addGroupBox(0, 0, 2, 1);
        editDetails()->page()->newGroupBox(tr("PreAmp"));
        editDetails()->page()->addComboBox(0, 1, 1, 5, "10", "00", "30", "0B", "large");                      //pre type
        editDetails()->page()->addComboBox(1, 1, 1, 1, "10", "00", "30", "15", "bottom");                     //gain sw
        editDetails()->page()->addKnob(1, 2, 1, 1, "10", "00", "30", "0C", "turbo_ratio1.25");                // gain
        editDetails()->page()->addKnob(1, 3, 1, 1, "10", "00", "30", "0D", "normal_ratio1.25");               //sag
        editDetails()->page()->addKnob(1, 4, 1, 1, "10", "00", "30", "0E", "normal_ratio1.25");               //resonance
        editDetails()->page()->addKnob(1, 5, 1, 1, "10", "00", "30", "10", "normal_ratio1.25");               //bass
        editDetails()->page()->addKnob(1, 6, 1, 1, "10", "00", "30", "11", "normal_ratio1.25");               // mid
        editDetails()->page()->addKnob(1, 7, 1, 1, "10", "00", "30", "12", "normal_ratio1.25");               // treble
        editDetails()->page()->addKnob(1, 8, 1, 1, "10", "00", "30", "13", "normal_ratio1.25");               // presence
        editDetails()->page()->addSwitch(1, 9, 1, 1, "10", "00", "30", "14");                                 // bright button
        editDetails()->page()->addKnob(0, 10, 2, 1, "10", "00", "30", "0F", "normal_ratio1.75");              // effect level
        editDetails()->page()->addGroupBox(0, 1, 1, 2);
        editDetails()->page()->newGroupBox(tr("Speaker"));
        editDetails()->page()->addComboBox(0, 0, 1, 1, "10", "00", "30", "18", "large");                      //spkr type
        editDetails()->page()->addComboBox(0, 1, 1, 1, "10", "00", "30", "19", "bottom");                     //mic type
        editDetails()->page()->addComboBox(0, 2, 1, 1, "10", "00", "30", "1A");                               //mic distance
        editDetails()->page()->addKnob(0, 3, 1, 1, "10", "00", "30", "1B", "normal_ratio1.25", "bottom", 70); //mic position
        editDetails()->page()->addKnob(0, 4, 1, 1, "10", "00", "30", "1C", "normal_ratio1.5");                // mic level
        editDetails()->page()->addKnob(0, 5, 1, 1, "10", "00", "30", "1D", "normal_ratio1.25");               // direct level
        editDetails()->page()->addGroupBox(1, 1, 1, 1);
        editDetails()->page()->newGroupBox(tr("Solo"));
        editDetails()->page()->addSwitch(0, 0, 1, 1, "10", "00", "30", "16", "middle");  //solo sw
        editDetails()->page()->addKnob(0, 1, 1, 1, "10", "00", "30", "17", "normal_ratio1.25");               // solo level
        editDetails()->page()->addGroupBox(1, 2, 1, 1);
    };
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(4);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        editDetails()->page()->newGroupBox("Equalizer", Qt::AlignTop | Qt::AlignHCenter);
        editDetails()->page()->addSwitch(0, 0, 1, 1, "10", "02", "2E", "2D");
        editDetails()->page()->addParaEQ(0, 1, 2, 4, "10", "02", "2E", "2E", "inst");
        editDetails()->page()->addGroupBox(0, 0, 1, 1);
    }
    else
    {
        editDetails()->page()->newGroupBox("Equalizer", Qt::AlignTop | Qt::AlignHCenter);
        editDetails()->page()->addSwitch(0, 0, 1, 1, "10", "00", "30", "21");
        editDetails()->page()->addParaEQ(0, 1, 2, 4, "10", "00", "30", "22", "inst");
        editDetails()->page()->addGroupBox(0, 0, 1, 1);
    };
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(4);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex2 = "29";
    }
    else
    {
        hex2 = "2B";
    };
    stringLevels();
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(4);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex2 = "32";
    }
    else
    {
        hex2 = "35";
    };
    altTuning();
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(4);
    twelveString();
    editDetails()->page()->addStackField();


    // *********************************************************************************************************** ACOUSTIC GUITAR ********************
    editDetails()->page()->newStackField(0);
    editDetails()->page()->newStackControl(6);
    editDetails()->page()->addTabBar(0, 0, 1, 1, "Tables", "00", "00", "15"); //page select Acoustic guitar
    editDetails()->page()->addStackControl();
    editDetails()->page()->insertStackField(6, 1, 0, 1, 1);
    editDetails()->page()->addStackField();
    // *****************************************************************************************************************************************


    editDetails()->page()->newStackField(6);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        editDetails()->page()->newGroupBox("Acoustic Bass");
        editDetails()->page()->addKnob(0, 0, 1, 1, "10", "02", "2F", "00", "normal_ratio1.75");  // volume
        editDetails()->page()->addKnob(0, 1, 1, 1, "10", "02", "2F", "01", "normal_ratio1.25");  // body
        editDetails()->page()->addKnob(0, 2, 1, 1, "10", "02", "2F", "02", "normal_ratio1.25");  // resonance
        editDetails()->page()->addKnob(0, 3, 1, 1, "10", "02", "2F", "03", "normal_ratio1.25");  // size
        editDetails()->page()->addKnob(0, 4, 1, 1, "10", "02", "2F", "04", "normal_ratio1.25");  // attack
        editDetails()->page()->addKnob(0, 5, 1, 1, "10", "02", "2F", "05", "normal_ratio1.25");  // bottom
        editDetails()->page()->addKnob(0, 6, 1, 1, "10", "02", "2F", "06", "normal_ratio1.25");  // buzz sense
        editDetails()->page()->addKnob(0, 7, 1, 1, "10", "02", "2F", "07", "normal_ratio1.25");  // decay
        editDetails()->page()->addGroupBox(0, 0, 1, 8);
        editDetails()->page()->newGroupBox("Noise Suppressor");
        editDetails()->page()->addSwitch(0, 0, 1, 1, "10", "02", "2F", "1C");                    // ns off/on
        editDetails()->page()->addKnob(0, 1, 1, 1, "10", "02", "2F", "1D", "normal_ratio1.25");  // threshold
        editDetails()->page()->addKnob(0, 2, 1, 1, "10", "02", "2F", "1E", "normal_ratio1.25");  // release
        editDetails()->page()->addGroupBox(0, 8, 1, 3);
    }
    else
    {
        editDetails()->page()->newGroupBox("Acoustic Guitar");
        editDetails()->page()->addComboBox(0, 1, 1, 7, "10", "00", "31", "00", "large");         // type
        editDetails()->page()->addKnob(1, 1, 1, 1, "10", "00", "31", "01", "normal_ratio1.25");  // body
        editDetails()->page()->addSystemOverride(1, 1, 1, 1, "10", "00", "31", "00", "06");
        editDetails()->page()->addSystemOverride(1, 1, 1, 1, "10", "00", "31", "00", "07");
        editDetails()->page()->addKnob(1, 2, 1, 1, "10", "00", "31", "08", "normal_ratio1.25");  // resonance
        editDetails()->page()->addSystemOverride(1, 2, 1, 1, "10", "00", "31", "00", "00");
        editDetails()->page()->addSystemOverride(1, 2, 1, 1, "10", "00", "31", "00", "01");
        editDetails()->page()->addSystemOverride(1, 2, 1, 1, "10", "00", "31", "00", "02");
        editDetails()->page()->addSystemOverride(1, 2, 1, 1, "10", "00", "31", "00", "03");
        editDetails()->page()->addSystemOverride(1, 2, 1, 1, "10", "00", "31", "00", "04");
        editDetails()->page()->addSystemOverride(1, 2, 1, 1, "10", "00", "31", "00", "05");
        editDetails()->page()->addSystemOverride(1, 2, 1, 1, "10", "00", "31", "00", "08");
        editDetails()->page()->addKnob(1, 3, 1, 1, "10", "00", "31", "02", "normal_ratio1.25");  // attack
        editDetails()->page()->addSystemOverride(1, 3, 1, 1, "10", "00", "31", "00", "00");
        editDetails()->page()->addSystemOverride(1, 3, 1, 1, "10", "00", "31", "00", "01");
        editDetails()->page()->addSystemOverride(1, 3, 1, 1, "10", "00", "31", "00", "02");
        editDetails()->page()->addSystemOverride(1, 3, 1, 1, "10", "00", "31", "00", "03");
        editDetails()->page()->addSystemOverride(1, 3, 1, 1, "10", "00", "31", "00", "04");
        editDetails()->page()->addSystemOverride(1, 3, 1, 1, "10", "00", "31", "00", "06");
        editDetails()->page()->addKnob(1, 4, 1, 1, "10", "00", "31", "09", "normal_ratio1.25");  // sustain
        editDetails()->page()->addSystemOverride(1, 4, 1, 1, "10", "00", "31", "00", "06", "not_equal");
        editDetails()->page()->addKnob(1, 5, 1, 1, "10", "00", "31", "04", "normal_ratio1.25");  // sense
        editDetails()->page()->addKnob(1, 6, 1, 1, "10", "00", "31", "05", "normal_ratio1.25");  // colour
        editDetails()->page()->addKnob(1, 7, 1, 1, "10", "00", "31", "06", "normal_ratio1.25");  // decay
        editDetails()->page()->addKnob(1, 8, 1, 1, "10", "00", "31", "07", "normal_ratio1.25");  // buzz
        editDetails()->page()->addComboBox(1, 9, 1, 1, "10", "00", "31", "03");                  // pu select
        editDetails()->page()->addSystemOverride(1, 5, 1, 5, "10", "00", "31", "00", "08", "not_equal");
        editDetails()->page()->addKnob(0, 8, 1, 1, "10", "00", "31", "0A", "normal_ratio1.75");  // volume
        editDetails()->page()->addKnob(0, 9, 1, 1, "10", "00", "31", "0B", "normal_ratio1.5");   // tone
        editDetails()->page()->addGroupBox(0, 0, 1, 1);
        editDetails()->page()->newGroupBox("Noise Suppressor");
        editDetails()->page()->addSwitch(0, 0, 1, 1, "10", "00", "31", "20");                    // ns off/on
        editDetails()->page()->addKnob(0, 1, 1, 1, "10", "00", "31", "21", "normal_ratio1.25");  // threshold
        editDetails()->page()->addKnob(0, 2, 1, 1, "10", "00", "31", "22", "normal_ratio1.25");  // release
        editDetails()->page()->addGroupBox(0, 1, 1, 1);
    };
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(6);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        editDetails()->page()->newGroupBox("Amp");
        editDetails()->page()->addSwitch(0, 0, 1, 1, "10", "02", "2F", "08");                                 // off/on effect
        editDetails()->page()->addGroupBox(0, 0, 2, 1);
        editDetails()->page()->newGroupBox(tr("PreAmp"));
        editDetails()->page()->addComboBox(0, 1, 1, 5, "10", "02", "2F", "09", "large");                      //pre type
        editDetails()->page()->addComboBox(1, 1, 1, 1, "10", "02", "2F", "13", "bottom");                     //gain sw
        editDetails()->page()->addKnob(1, 2, 1, 1, "10", "02", "2F", "0A", "turbo_ratio1.25");                //gain
        editDetails()->page()->addKnob(1, 3, 1, 1, "10", "02", "2F", "0B", "normal_ratio1.25");               //sag
        editDetails()->page()->addKnob(1, 4, 1, 1, "10", "02", "2F", "0C", "normal_ratio1.25");               //resonance
        editDetails()->page()->addKnob(1, 5, 1, 1, "10", "02", "2F", "0E", "normal_ratio1.25");               //bass
        editDetails()->page()->addKnob(1, 6, 1, 1, "10", "02", "2F", "0F", "normal_ratio1.25");               // mid
        editDetails()->page()->addKnob(1, 7, 1, 1, "10", "02", "2F", "10", "normal_ratio1.25");               // treble
        editDetails()->page()->addKnob(1, 8, 1, 1, "10", "02", "2F", "11", "normal_ratio1.25");               // presence
        editDetails()->page()->addSwitch(1, 9, 1, 1, "10", "02", "2F", "12");                                 // bright button
        editDetails()->page()->addKnob(0, 10, 2, 1, "10", "02", "2F", "0D", "normal_ratio1.75");              // effect level
        editDetails()->page()->addGroupBox(0, 1, 1, 2);
        editDetails()->page()->newGroupBox(tr("Speaker"));
        editDetails()->page()->addComboBox(0, 0, 1, 1, "10", "02", "2F", "16", "large");                      //spkr type
        editDetails()->page()->addComboBox(0, 1, 1, 1, "10", "02", "2F", "17", "bottom");                     //mic type
        editDetails()->page()->addComboBox(0, 2, 1, 1, "10", "02", "2F", "18");                               //mic distance
        editDetails()->page()->addKnob(0, 3, 1, 1, "10", "02", "2F", "19", "normal_ratio1.25", "bottom", 70); //mic position
        editDetails()->page()->addKnob(0, 4, 1, 1, "10", "02", "2F", "1A", "normal_ratio1.5");                // mic level
        editDetails()->page()->addKnob(0, 5, 1, 1, "10", "02", "2F", "1B", "normal_ratio1.25");               // direct level
        editDetails()->page()->addGroupBox(1, 1, 1, 1);
        editDetails()->page()->newGroupBox(tr("Solo"));
        editDetails()->page()->addSwitch(0, 0, 1, 1, "10", "02", "2F", "14", "middle");                        //solo sw
        editDetails()->page()->addKnob(0, 1, 1, 1, "10", "02", "2F", "15", "normal_ratio1.25");                // solo level
        editDetails()->page()->addGroupBox(1, 2, 1, 1);
    }
    else
    {
        editDetails()->page()->newGroupBox("Amp");
        editDetails()->page()->addSwitch(0, 0, 1, 1, "10", "00", "31", "0C");                                 // off/on effect
        editDetails()->page()->addGroupBox(0, 0, 2, 1);
        editDetails()->page()->newGroupBox(tr("PreAmp"));
        editDetails()->page()->addComboBox(0, 1, 1, 5, "10", "00", "31", "0D", "large");                      //pre type
        editDetails()->page()->addComboBox(1, 1, 1, 1, "10", "00", "31", "17", "bottom");                     //gain sw
        editDetails()->page()->addKnob(1, 2, 1, 1, "10", "00", "31", "0E", "turbo_ratio1.25");                // gain
        editDetails()->page()->addKnob(1, 3, 1, 1, "10", "00", "31", "0F", "normal_ratio1.25");               //sag
        editDetails()->page()->addKnob(1, 4, 1, 1, "10", "00", "31", "10", "normal_ratio1.25");               //resonance
        editDetails()->page()->addKnob(1, 5, 1, 1, "10", "00", "31", "12", "normal_ratio1.25");               //bass
        editDetails()->page()->addKnob(1, 6, 1, 1, "10", "00", "31", "13", "normal_ratio1.25");               // mid
        editDetails()->page()->addKnob(1, 7, 1, 1, "10", "00", "31", "14", "normal_ratio1.25");               // treble
        editDetails()->page()->addKnob(1, 8, 1, 1, "10", "00", "31", "15", "normal_ratio1.25");               // presence
        editDetails()->page()->addSwitch(1, 9, 1, 1, "10", "00", "31", "16");                                 // bright button
        editDetails()->page()->addKnob(0, 10, 2, 1, "10", "00", "31", "11", "normal_ratio1.75");              // effect level
        editDetails()->page()->addGroupBox(0, 1, 1, 2);
        editDetails()->page()->newGroupBox(tr("Speaker"));
        editDetails()->page()->addComboBox(0, 0, 1, 1, "10", "00", "31", "1A", "large");                      //spkr type
        editDetails()->page()->addComboBox(0, 1, 1, 1, "10", "00", "31", "1B", "bottom");                     //mic type
        editDetails()->page()->addComboBox(0, 2, 1, 1, "10", "00", "31", "1C");                               //mic distance
        editDetails()->page()->addKnob(0, 3, 1, 1, "10", "00", "31", "1D", "normal_ratio1.25", "bottom", 70); //mic position
        editDetails()->page()->addKnob(0, 4, 1, 1, "10", "00", "31", "1E", "normal_ratio1.5");                // mic level
        editDetails()->page()->addKnob(0, 5, 1, 1, "10", "00", "31", "1F", "normal_ratio1.25");               // direct level
        editDetails()->page()->addGroupBox(1, 1, 1, 1);
        editDetails()->page()->newGroupBox(tr("Solo"));
        editDetails()->page()->addSwitch(0, 0, 1, 1, "10", "00", "31", "18", "middle");   //solo sw
        editDetails()->page()->addKnob(0, 1, 1, 1, "10", "00", "31", "19", "normal_ratio1.25");               // solo level
        editDetails()->page()->addGroupBox(1, 2, 1, 1);
    }
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(6);
    editDetails()->page()->newGroupBox("Equalizer", Qt::AlignTop | Qt::AlignHCenter);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        editDetails()->page()->addSwitch(0, 0, 1, 1, "10", "02", "2F", "1F");
        editDetails()->page()->addParaEQ(0, 1, 2, 4, "10", "02", "2F", "20", "inst");
    }
    else
    {
        editDetails()->page()->addSwitch(0, 0, 1, 1, "10", "00", "31", "23");
        editDetails()->page()->addParaEQ(0, 1, 2, 4, "10", "00", "31", "24", "inst");
    };
    editDetails()->page()->addGroupBox(0, 0, 1, 1);
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(6);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex2 = "29";
    }
    else
    {
        hex2 = "2B";
    };
    stringLevels();
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(6);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex2 = "32";
    }
    else
    {
        hex2 = "35";
    };
    altTuning();
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(6);
    twelveString();
    editDetails()->page()->addStackField();


    // *********************************************************************************************************** ELECTRIC BASS (guitar for bass mode)********************
    editDetails()->page()->newStackField(0);
    editDetails()->page()->newStackControl(7);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        editDetails()->page()->addTabBar(0, 0, 1, 1, "Tables", "00", "00", "14"); //page select Electric guitar
    }
    else
    {
        editDetails()->page()->addTabBar(0, 0, 1, 1, "Tables", "00", "00", "16"); //page select Electric bass
    };
    editDetails()->page()->addStackControl();
    editDetails()->page()->insertStackField(7, 1, 0, 1, 1);
    editDetails()->page()->addStackField();
    // *****************************************************************************************************************************************


    editDetails()->page()->newStackField(7);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        editDetails()->page()->newGroupBox("Electric Guitar");
        editDetails()->page()->addComboBox(0, 1, 1, 5, "10", "02", "26", "00", "large");          // type
        editDetails()->page()->addComboBox(1, 1, 1, 1, "10", "02", "26", "01", "pu_sel_bmode_g"); // pu select
        editDetails()->page()->addSystemOverride(1, 1, 1, 1, "10", "02", "26", "00", "0B");
        editDetails()->page()->addComboBox(1, 2, 1, 1, "10", "02", "26", "02");                   // tone type
        editDetails()->page()->addKnob(1, 3, 1, 1, "10", "02", "26", "03", "normal_ratio1.25");   // sense
        editDetails()->page()->addKnob(1, 4, 1, 1, "10", "02", "26", "04", "normal_ratio1.25");   // depth
        editDetails()->page()->addKnob(1, 5, 1, 1, "10", "02", "26", "05", "normal_ratio1.25");   // attack
        editDetails()->page()->addKnob(1, 6, 1, 1, "10", "02", "26", "06", "normal_ratio1.25");   // resonance
        editDetails()->page()->addKnob(1, 7, 1, 1, "10", "02", "26", "07", "normal_ratio1.25");   // direct
        editDetails()->page()->addSystemOverride(1, 2, 1, 6, "10", "02", "26", "00", "02", "not_equal");
        editDetails()->page()->addKnob(0, 6, 1, 1, "10", "02", "26", "08", "normal_ratio1.75");   // volume
        editDetails()->page()->addKnob(0, 7, 1, 1, "10", "02", "26", "09", "normal_ratio1.5");    // tone
        editDetails()->page()->addGroupBox(0, 0, 1, 7);
    }
    else
    {
        editDetails()->page()->newGroupBox("Electric Bass");
        editDetails()->page()->addComboBox(0, 1, 1, 5, "10", "00", "32", "00", "large");         // type
        editDetails()->page()->addKnob(1, 1, 1, 1, "10", "00", "32", "01", "normal_ratio1.25");  // j front vol
        editDetails()->page()->addKnob(1, 2, 1, 1, "10", "00", "32", "02", "normal_ratio1.25");  // j rear vol
        editDetails()->page()->addSystemOverride(1, 1, 1, 2, "10", "00", "32", "00", "01");
        editDetails()->page()->addSystemOverride(1, 1, 1, 2, "10", "00", "32", "00", "02");
        editDetails()->page()->addKnob(1, 3, 1, 1, "10", "00", "32", "04", "normal_ratio1.25");  // sense
        editDetails()->page()->addKnob(1, 4, 1, 1, "10", "00", "32", "05", "normal_ratio1.25");  // depth
        editDetails()->page()->addKnob(1, 5, 1, 1, "10", "00", "32", "06", "normal_ratio1.25");  // attack
        editDetails()->page()->addKnob(1, 6, 1, 1, "10", "00", "32", "07", "normal_ratio1.25");  // resonance
        editDetails()->page()->addKnob(1, 7, 1, 1, "10", "00", "32", "08", "normal_ratio1.25");  // direct
        editDetails()->page()->addComboBox(1, 8, 1, 1, "10", "00", "32", "03");                  //tone type
        editDetails()->page()->addSystemOverride(1, 3, 1, 6, "10", "00", "32", "00", "02", "not_equal");
        editDetails()->page()->addKnob(0, 7, 1, 1, "10", "00", "32", "09", "normal_ratio1.75");  // volume
        editDetails()->page()->addKnob(0, 8, 1, 1, "10", "00", "32", "0A", "normal_ratio1.5");   // tone
        editDetails()->page()->addGroupBox(0, 0, 1, 8);
    };
    editDetails()->page()->newGroupBox("Noise Suppressor");
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        editDetails()->page()->addSwitch(0, 0, 1, 1, "10", "02", "26", "1E");                    // ns off/on
        editDetails()->page()->addKnob(0, 1, 1, 1, "10", "02", "26", "1F", "normal_ratio1.25");  // threshold
        editDetails()->page()->addKnob(0, 2, 1, 1, "10", "02", "26", "20", "normal_ratio1.25");  // release
    }
    else
    {
        editDetails()->page()->addSwitch(0, 0, 1, 1, "10", "00", "32", "1F");                    // ns off/on
        editDetails()->page()->addKnob(0, 1, 1, 1, "10", "00", "32", "20", "normal_ratio1.25");  // threshold
        editDetails()->page()->addKnob(0, 2, 1, 1, "10", "00", "32", "21", "normal_ratio1.25");  // release
    };
    editDetails()->page()->addGroupBox(0, 8, 1, 3);
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(7);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        editDetails()->page()->newGroupBox("Amp");
        editDetails()->page()->addSwitch(0, 0, 1, 1, "10", "02", "26", "0A");                                 // off/on effect
        editDetails()->page()->addGroupBox(0, 0, 2, 1);
        editDetails()->page()->newGroupBox(tr("PreAmp"));
        editDetails()->page()->addComboBox(0, 1, 1, 5, "10", "02", "26", "0B", "large");                      //pre type
        editDetails()->page()->addComboBox(1, 1, 1, 1, "10", "02", "26", "15", "bottom");                     //gain sw
        editDetails()->page()->addKnob(1, 2, 1, 1, "10", "02", "26", "0C", "turbo_ratio1.25");                // gain
        editDetails()->page()->addKnob(1, 3, 1, 1, "10", "02", "26", "0D", "normal_ratio1.25");               //sag
        editDetails()->page()->addKnob(1, 4, 1, 1, "10", "02", "26", "0E", "normal_ratio1.25");               //resonance
        editDetails()->page()->addKnob(1, 5, 1, 1, "10", "02", "26", "10", "normal_ratio1.25");               //bass
        editDetails()->page()->addKnob(1, 6, 1, 1, "10", "02", "26", "11", "normal_ratio1.25");               // mid
        editDetails()->page()->addKnob(1, 7, 1, 1, "10", "02", "26", "12", "normal_ratio1.25");               // treble
        editDetails()->page()->addKnob(1, 8, 1, 1, "10", "02", "26", "13", "normal_ratio1.25");               // presence
        editDetails()->page()->addSwitch(1, 9, 1, 1, "10", "02", "26", "14");                                 // bright button
        editDetails()->page()->addKnob(0, 10, 2, 1, "10", "02", "26", "0F", "normal_ratio1.75");              // effect level
        editDetails()->page()->addGroupBox(0, 1, 1, 2);
        editDetails()->page()->newGroupBox(tr("Speaker"));
        editDetails()->page()->addComboBox(0, 0, 1, 1, "10", "02", "26", "18", "large");                      //spkr type
        editDetails()->page()->addComboBox(0, 1, 1, 1, "10", "02", "26", "19", "bottom");                     //mic type
        editDetails()->page()->addComboBox(0, 2, 1, 1, "10", "02", "26", "1A");                               //mic distance
        editDetails()->page()->addKnob(0, 3, 1, 1, "10", "02", "26", "1B", "normal_ratio1.25", "bottom", 70); //mic position
        editDetails()->page()->addKnob(0, 4, 1, 1, "10", "02", "26", "1C", "normal_ratio1.5");                // mic level
        editDetails()->page()->addKnob(0, 5, 1, 1, "10", "02", "26", "1D", "normal_ratio1.25");               // direct level
        editDetails()->page()->addGroupBox(1, 1, 1, 1);
        editDetails()->page()->newGroupBox(tr("Solo"));
        editDetails()->page()->addSwitch(0, 0, 1, 1, "10", "02", "26", "16", "middle");                       //solo sw
        editDetails()->page()->addKnob(0, 2, 1, 1, "10", "02", "26", "17", "normal_ratio1.25");               // solo level
        editDetails()->page()->addGroupBox(1, 2, 1, 1);
    }
    else
    {
        editDetails()->page()->newGroupBox("Amp");
        editDetails()->page()->addSwitch(0, 0, 1, 1, "10", "00", "32", "0B");                                 // off/on effect
        editDetails()->page()->addGroupBox(0, 0, 2, 1);
        editDetails()->page()->newGroupBox(tr("PreAmp"));
        editDetails()->page()->addComboBox(0, 1, 1, 5, "10", "00", "32", "0C", "large");                      //pre type
        editDetails()->page()->addComboBox(1, 1, 1, 1, "10", "00", "32", "16", "bottom");                     //gain sw
        editDetails()->page()->addKnob(1, 2, 1, 1, "10", "00", "32", "0D", "turbo_ratio1.25");                // gain
        editDetails()->page()->addKnob(1, 3, 1, 1, "10", "00", "32", "0E", "normal_ratio1.25");               //sag
        editDetails()->page()->addKnob(1, 4, 1, 1, "10", "00", "32", "0F", "normal_ratio1.25");               //resonance
        editDetails()->page()->addKnob(1, 5, 1, 1, "10", "00", "32", "11", "normal_ratio1.25");               //bass
        editDetails()->page()->addKnob(1, 6, 1, 1, "10", "00", "32", "12", "normal_ratio1.25");               // mid
        editDetails()->page()->addKnob(1, 7, 1, 1, "10", "00", "32", "13", "normal_ratio1.25");               // treble
        editDetails()->page()->addKnob(1, 8, 1, 1, "10", "00", "32", "14", "normal_ratio1.25");               // presence
        editDetails()->page()->addSwitch(1, 9, 1, 1, "10", "00", "32", "15");                                 // bright button
        editDetails()->page()->addKnob(0, 10, 2, 1, "10", "00", "32", "10", "normal_ratio1.75");              // effect level
        editDetails()->page()->addGroupBox(0, 1, 1, 2);
        editDetails()->page()->newGroupBox(tr("Speaker"));
        editDetails()->page()->addComboBox(0, 0, 1, 1, "10", "00", "32", "19", "large");                      //spkr type
        editDetails()->page()->addComboBox(0, 1, 1, 1, "10", "00", "32", "1A", "bottom");                     //mic type
        editDetails()->page()->addComboBox(0, 2, 1, 1, "10", "00", "32", "1B");                               //mic distance
        editDetails()->page()->addKnob(0, 3, 1, 1, "10", "00", "32", "1C", "normal_ratio1.25", "bottom", 70); //mic position
        editDetails()->page()->addKnob(0, 4, 1, 1, "10", "00", "32", "1D", "normal_ratio1.5");                // mic level
        editDetails()->page()->addKnob(0, 5, 1, 1, "10", "00", "32", "1E", "normal_ratio1.25");               // direct level
        editDetails()->page()->addGroupBox(1, 1, 1, 1);
        editDetails()->page()->newGroupBox(tr("Solo"));
        editDetails()->page()->addSwitch(0, 0, 1, 1, "10", "00", "32", "17", "middle");                       //solo sw
        editDetails()->page()->addKnob(0, 1, 1, 1, "10", "00", "32", "18", "normal_ratio1.25");               // solo level
        editDetails()->page()->addGroupBox(1, 2, 1, 1);
    };
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(7);
    editDetails()->page()->newGroupBox("Equalizer", Qt::AlignTop | Qt::AlignHCenter);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        editDetails()->page()->addSwitch(0, 0, 1, 1, "10", "02", "26", "21");
        editDetails()->page()->addParaEQ(0, 1, 2, 4, "10", "02", "26", "22", "inst");
    }else{
        editDetails()->page()->addSwitch(0, 0, 1, 1, "10", "00", "32", "22");
        editDetails()->page()->addParaEQ(0, 1, 2, 4, "10", "00", "32", "23", "inst");
    };
    editDetails()->page()->addGroupBox(0, 0, 1, 1);
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(7);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex2 = "29";
    }
    else
    {
        hex2 = "2B";
    };
    stringLevels();
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(7);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex2 = "32";
    }
    else
    {
        hex2 = "35";
    };
    altTuning();
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(7);
    twelveString();
    editDetails()->page()->addStackField();


    // *********************************************************************************************************** VIO GUITAR / POLY FX ********
    editDetails()->page()->newStackField(0);
    editDetails()->page()->newStackControl(8);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        editDetails()->page()->addTabBar(0, 0, 1, 1, "Tables", "00", "00", "18");        //page select Poly FX (bass mode)
    }
    else
    {
        editDetails()->page()->addTabBar(0, 0, 1, 1, "Tables", "00", "00", "17");          //page select VIO Guitar (guitar mode)
    };
    editDetails()->page()->addStackControl();
    editDetails()->page()->insertStackField(8, 1, 0, 1, 1);
    editDetails()->page()->addStackField();
    // *****************************************************************************************************************************************


    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        editDetails()->page()->newStackField(8);
        editDetails()->page()->newGroupBox("Poly FX");
        editDetails()->page()->addComboBox(0, 0, 1, 2, "10", "02", "27", "00", "large");        //type
        editDetails()->page()->addKnob(1, 0, 1, 2, "10", "02", "27", "01", "normal_ratio1.75"); //vol
        editDetails()->page()->addKnob(0, 2, 1, 1, "10", "02", "27", "02", "normal_ratio1.5");  //gain
        editDetails()->page()->addKnob(0, 3, 1, 1, "10", "02", "27", "03", "normal_ratio1.25"); //bal
        editDetails()->page()->addSystemOverride(0, 2, 1, 2, "10", "02", "27", "00", "00", "not_equal");
        editDetails()->page()->addKnob(1, 2, 1, 1, "10", "02", "27", "04", "normal_ratio1.25"); //color
        editDetails()->page()->addKnob(1, 3, 1, 1, "10", "02", "27", "05", "normal_ratio1.25"); //tone
        editDetails()->page()->addGroupBox(0, 0, 1, 4);

        editDetails()->page()->newGroupBox("Filter");
        editDetails()->page()->addComboBox(0, 0, 1, 1, "10", "02", "27", "06");                 //flt mode
        editDetails()->page()->addComboBox(1, 0, 1, 1, "10", "02", "27", "07");                 //polarity
        editDetails()->page()->addKnob(0, 1, 1, 1, "10", "02", "27", "08", "normal_ratio1.25"); //sense
        editDetails()->page()->addKnob(0, 2, 1, 1, "10", "02", "27", "09", "normal_ratio1.25"); //freq
        editDetails()->page()->addKnob(1, 1, 1, 1, "10", "02", "27", "0A", "normal_ratio1.25"); //decay
        editDetails()->page()->addKnob(1, 2, 1, 1, "10", "02", "27", "0B", "normal_ratio1.25"); //peak
        editDetails()->page()->addComboBox(0, 3, 2, 1, "10", "02", "27", "0C");                 //tone type
        editDetails()->page()->addSystemOverride(0, 0, 2, 4, "10", "02", "27", "00", "04", "not_equal");
        editDetails()->page()->addGroupBox(0, 4, 1, 4);

        editDetails()->page()->newGroupBox("Compressor");
        editDetails()->page()->addSwitch(0, 0, 1, 2, "10", "02", "27", "0D");                   //comp sw
        editDetails()->page()->addKnob(1, 0, 1, 1, "10", "02", "27", "0E", "normal_ratio1.25"); //sustain
        editDetails()->page()->addKnob(2, 0, 1, 1, "10", "02", "27", "0F", "normal_ratio1.25"); //attack
        editDetails()->page()->addSystemOverride(0, 0, 3, 1, "10", "02", "27", "00", "04", "not_equal");
        editDetails()->page()->addGroupBox(0, 8, 1, 1);

        editDetails()->page()->newGroupBox("Noise Suppressor");
        editDetails()->page()->addSwitch(0, 0, 1, 2, "10", "02", "27", "10");                   //ns sw
        editDetails()->page()->addKnob(1, 0, 1, 1, "10", "02", "27", "11", "normal_ratio1.25"); //thresh
        editDetails()->page()->addKnob(2, 0, 1, 1, "10", "02", "27", "12", "normal_ratio1.25"); //release
        editDetails()->page()->addGroupBox(0, 9, 1, 1);
        editDetails()->page()->addStackField();
    }
    else
    {
        editDetails()->page()->newStackField(8);
        editDetails()->page()->newGroupBox("VIO Guitar");
        editDetails()->page()->addComboBox(0, 1, 1, 5, "10", "00", "33", "00", "large");         // type
        editDetails()->page()->addComboBox(1, 1, 1, 1, "10", "00", "33", "01", "pu_sel");        // pu select
        editDetails()->page()->addSystemOverride(1, 1, 1, 1, "10", "00", "33", "00", "0B");
        editDetails()->page()->addComboBox(1, 2, 1, 1, "10", "00", "33", "02");                  // tone type
        editDetails()->page()->addKnob(1, 3, 1, 1, "10", "00", "33", "03", "normal_ratio1.25");  // sense
        editDetails()->page()->addKnob(1, 4, 1, 1, "10", "00", "33", "04", "normal_ratio1.25");  // depth
        editDetails()->page()->addKnob(1, 5, 1, 1, "10", "00", "33", "05", "normal_ratio1.25");  // attack
        editDetails()->page()->addKnob(1, 6, 1, 1, "10", "00", "33", "06", "normal_ratio1.25");  // resonance
        editDetails()->page()->addKnob(1, 7, 1, 1, "10", "00", "33", "07", "normal_ratio1.25");  // direct
        editDetails()->page()->addSystemOverride(1, 2, 1, 6, "10", "00", "33", "00", "0B", "not_equal");
        editDetails()->page()->addKnob(0, 6, 1, 1, "10", "00", "33", "08", "normal_ratio1.75");  // volume
        editDetails()->page()->addKnob(0, 7, 1, 1, "10", "00", "33", "09", "normal_ratio1.5");   // tone
        editDetails()->page()->addGroupBox(0, 0, 1, 1);
        editDetails()->page()->newGroupBox("Noise Suppressor");
        editDetails()->page()->addSwitch(0, 0, 1, 1, "10", "00", "33", "16");                    // ns off/on
        editDetails()->page()->addKnob(0, 1, 1, 1, "10", "00", "33", "17", "normal_ratio1.25");  // threshold
        editDetails()->page()->addKnob(0, 2, 1, 1, "10", "00", "33", "18", "normal_ratio1.25");  // release
        editDetails()->page()->addGroupBox(0, 1, 1, 1);
        editDetails()->page()->addStackField();

        editDetails()->page()->newStackField(8);
        editDetails()->page()->newGroupBox("Filter");
        editDetails()->page()->addKnob(0, 0, 1, 1, "10", "00", "33", "0E", "normal_ratio1.5");   // overtone
        editDetails()->page()->addKnob(0, 1, 1, 1, "10", "00", "33", "0F", "normal_ratio1.5");   // attack
        editDetails()->page()->addKnob(0, 2, 1, 1, "10", "00", "33", "10", "normal_ratio1.5");   // power bend
        editDetails()->page()->addKnob(0, 3, 1, 1, "10", "00", "33", "11", "normal_ratio1.5");   // slide time
        editDetails()->page()->addComboBox(0, 4, 1, 1, "10", "00", "33", "12", "large");         // octave
        editDetails()->page()->addKnob(0, 5, 1, 1, "10", "00", "33", "13", "normal_ratio1.5");   // colour
        editDetails()->page()->addKnob(0, 6, 1, 1, "10", "00", "33", "14", "normal_ratio1.5");   // touch sense
        editDetails()->page()->addKnob(0, 7, 1, 1, "10", "00", "33", "15", "normal_ratio1.5");   // lead emph
        editDetails()->page()->addGroupBox(0, 0, 1, 1);
        editDetails()->page()->newGroupBox("Harmony");
        editDetails()->page()->addKnob(0, 0, 1, 1, "10", "00", "33", "0A", "normal_ratio1.5");   // pitch
        editDetails()->page()->addKnob(0, 1, 1, 1, "10", "00", "33", "0B", "normal_ratio1.5");   // gain
        editDetails()->page()->addKnob(0, 2, 1, 1, "10", "00", "33", "0C", "normal_ratio1.5");   // attack
        editDetails()->page()->addKnob(0, 3, 1, 1, "10", "00", "33", "0D", "normal_ratio1.5");   // direct
        editDetails()->page()->addGroupBox(1, 0, 1, 1);
        editDetails()->page()->addStackField();

        editDetails()->page()->newStackField(8);
        editDetails()->page()->newGroupBox("Equalizer", Qt::AlignTop | Qt::AlignHCenter);
        editDetails()->page()->addSwitch(0, 0, 1, 1, "10", "00", "33", "19");
        editDetails()->page()->addParaEQ(0, 1, 2, 4, "10", "00", "33", "1A", "inst");
        editDetails()->page()->addGroupBox(0, 0, 1, 1);
        editDetails()->page()->addStackField();
    };

    editDetails()->page()->newStackField(8);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex2 = "29";
    }
    else
    {
        hex2 = "2B";
    };
    stringLevels();
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(8);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex2 = "28";
    }
    else
    {
        hex2 = "2A";
    };
    altTuning();
    editDetails()->page()->addStackField();


    if(preferences->getPreferences("Window", "BassMode", "bool")=="false")
    {
        // *********************************************************************************************************** POLY FX ********************
        editDetails()->page()->newStackField(0);
        editDetails()->page()->newStackControl(9);
        editDetails()->page()->addTabBar(0, 0, 1, 1, "Tables", "00", "00", "18");        //page select Poly FX
        editDetails()->page()->addStackControl();
        editDetails()->page()->insertStackField(9, 1, 0, 1, 1);
        editDetails()->page()->addStackField();
        // *****************************************************************************************************************************************

        hex2 = "34";
        editDetails()->page()->newStackField(9);
        editDetails()->page()->newGroupBox("Poly FX");
        editDetails()->page()->addComboBox(0, 0, 1, 2, "10", hex1, hex2, "00", "large");        //type
        editDetails()->page()->addKnob(1, 0, 1, 2, "10", hex1, hex2, "01", "normal_ratio1.75"); //vol
        editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.5");  //gain
        editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "03", "normal_ratio1.25"); //bal
        editDetails()->page()->addSystemOverride(0, 2, 1, 2, "10", hex1, hex2, "00", "00", "not_equal");
        editDetails()->page()->addKnob(1, 2, 1, 1, "10", hex1, hex2, "04", "normal_ratio1.25"); //color
        editDetails()->page()->addKnob(1, 3, 1, 1, "10", hex1, hex2, "05", "normal_ratio1.25"); //tone
        editDetails()->page()->addGroupBox(0, 0, 1, 4);

        editDetails()->page()->newGroupBox("Filter");
        editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, hex2, "06");                 //flt mode
        editDetails()->page()->addComboBox(1, 0, 1, 1, "10", hex1, hex2, "07");                 //polarity
        editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "08", "normal_ratio1.25"); //sense
        editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "09", "normal_ratio1.25"); //freq
        editDetails()->page()->addKnob(1, 1, 1, 1, "10", hex1, hex2, "0A", "normal_ratio1.25"); //decay
        editDetails()->page()->addKnob(1, 2, 1, 1, "10", hex1, hex2, "0B", "normal_ratio1.25"); //peak
        editDetails()->page()->addComboBox(0, 3, 2, 1, "10", hex1, hex2, "0C");                 //tone type
        editDetails()->page()->addSystemOverride(0, 0, 2, 4, "10", hex1, hex2, "00", "04", "not_equal");
        editDetails()->page()->addGroupBox(0, 4, 1, 4);

        editDetails()->page()->newGroupBox("Compressor");
        editDetails()->page()->addSwitch(0, 0, 1, 2, "10", hex1, hex2, "0D");                   //comp sw
        editDetails()->page()->addKnob(1, 0, 1, 1, "10", hex1, hex2, "0E", "normal_ratio1.25"); //sustain
        editDetails()->page()->addKnob(2, 0, 1, 1, "10", hex1, hex2, "0F", "normal_ratio1.25"); //attack
        editDetails()->page()->addSystemOverride(0, 0, 3, 1, "10", hex1, hex2, "00", "04", "not_equal");
        editDetails()->page()->addGroupBox(0, 8, 1, 1);

        editDetails()->page()->newGroupBox("Noise Suppressor");
        editDetails()->page()->addSwitch(0, 0, 1, 2, "10", hex1, hex2, "10");                   //ns sw
        editDetails()->page()->addKnob(1, 0, 1, 1, "10", hex1, hex2, "11", "normal_ratio1.25"); //thresh
        editDetails()->page()->addKnob(2, 0, 1, 1, "10", hex1, hex2, "12", "normal_ratio1.25"); //release
        editDetails()->page()->addGroupBox(0, 9, 1, 1);
        editDetails()->page()->addStackField();

        editDetails()->page()->newStackField(9);
        hex2 = "2B";
        stringLevels();
        editDetails()->page()->addStackField();

        editDetails()->page()->newStackField(9);
        hex2 = "2A";
        altTuning();
        editDetails()->page()->addStackField();
    };

    editDetails()->page()->clearPowerControl();
    editDetails()->addPage();
};

void soundsource_inst3::stringLevels()
{
    editDetails()->page()->compactCurrentStackField(20, 24);
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "0B", "normal_ratio1.5");  //6
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "0A", "normal_ratio1.5");  //5
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "09", "normal_ratio1.5");  //4
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "08", "normal_ratio1.5");  //3
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "07", "normal_ratio1.5");  //2
    editDetails()->page()->addKnob(0, 5, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.5");  //1
    editDetails()->page()->addKnob(1, 0, 1, 1, "10", hex1, hex2, "11", "normal_ratio1.5");  //6
    editDetails()->page()->addKnob(1, 1, 1, 1, "10", hex1, hex2, "10", "normal_ratio1.5");  //5
    editDetails()->page()->addKnob(1, 2, 1, 1, "10", hex1, hex2, "0F", "normal_ratio1.5");  //4
    editDetails()->page()->addKnob(1, 3, 1, 1, "10", hex1, hex2, "0E", "normal_ratio1.5");  //3
    editDetails()->page()->addKnob(1, 4, 1, 1, "10", hex1, hex2, "0D", "normal_ratio1.5");  //2
    editDetails()->page()->addKnob(1, 5, 1, 1, "10", hex1, hex2, "0C", "normal_ratio1.5");  //1
}

void soundsource_inst3::altTuning()
{
    editDetails()->page()->compactCurrentStackField(20, 24);
    editDetails()->page()->addComboBox(0, 0, 1, 2, "10", hex1, hex2, "01", "left_large");    // tuning type
    editDetails()->page()->addDivider(1, 0, 1, 10, Qt::AlignTop | Qt::AlignLeft);
    editDetails()->page()->addKnob(2, 0, 1, 1, "10", hex1, hex2, "28", "normal_ratio1.25");  // str bend sw
    editDetails()->page()->addKnob(2, 1, 1, 1, "10", hex1, hex2, "2F", "normal_ratio1.25");  // bend control
    editDetails()->page()->addKnob(2, 2, 1, 1, "10", hex1, hex2, "07", "normal_ratio1.25");  // pitch 6
    editDetails()->page()->addKnob(2, 3, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.25");  // pitch 5
    editDetails()->page()->addKnob(2, 4, 1, 1, "10", hex1, hex2, "05", "normal_ratio1.25");  // pitch 4
    editDetails()->page()->addKnob(2, 5, 1, 1, "10", hex1, hex2, "04", "normal_ratio1.25");  // pitch 3
    editDetails()->page()->addKnob(2, 6, 1, 1, "10", hex1, hex2, "03", "normal_ratio1.25");  // pitch 2
    editDetails()->page()->addKnob(2, 7, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.25");  // pitch 1
    editDetails()->page()->addKnob(3, 2, 1, 1, "10", hex1, hex2, "2E", "normal_ratio1.25");  // bend depth 6
    editDetails()->page()->addKnob(3, 3, 1, 1, "10", hex1, hex2, "2D", "normal_ratio1.25");  // bend depth 5
    editDetails()->page()->addKnob(3, 4, 1, 1, "10", hex1, hex2, "2C", "normal_ratio1.25");  // bend depth 4
    editDetails()->page()->addKnob(3, 5, 1, 1, "10", hex1, hex2, "2B", "normal_ratio1.25");  // bend depth 3
    editDetails()->page()->addKnob(3, 6, 1, 1, "10", hex1, hex2, "2A", "normal_ratio1.25");  // bend depth 2
    editDetails()->page()->addKnob(3, 7, 1, 1, "10", hex1, hex2, "29", "normal_ratio1.25");  // bend depth 1
    editDetails()->page()->addKnob(4, 2, 1, 1, "10", hex1, hex2, "0D", "normal_ratio1.25");  // fine 6
    editDetails()->page()->addKnob(4, 3, 1, 1, "10", hex1, hex2, "0C", "normal_ratio1.25");  // fine 5
    editDetails()->page()->addKnob(4, 4, 1, 1, "10", hex1, hex2, "0B", "normal_ratio1.25");  // fine 4
    editDetails()->page()->addKnob(4, 5, 1, 1, "10", hex1, hex2, "0A", "normal_ratio1.25");  // fine 3
    editDetails()->page()->addKnob(4, 6, 1, 1, "10", hex1, hex2, "09", "normal_ratio1.25");  // fine 2
    editDetails()->page()->addKnob(4, 7, 1, 1, "10", hex1, hex2, "08", "normal_ratio1.25");  // fine 1
}

void soundsource_inst3::twelveString()
{
    editDetails()->page()->newGroupBox("12 String");
    editDetails()->page()->addSwitch(0, 0, 1, 2, "10", hex1, hex2, "0E");                    // off/on
    editDetails()->page()->addComboBox(0, 3, 1, 2, "10", hex1, hex2, "0F");                  // 12 str type
    editDetails()->page()->newGroupBox(str6);
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "15", "normal_ratio1.25");  // pitch 6
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "21", "normal_ratio1.25");  // level 6
    editDetails()->page()->addKnob(1, 0, 1, 1, "10", hex1, hex2, "1B", "normal_ratio1.25");  // fine 6
    editDetails()->page()->addKnob(1, 1, 1, 1, "10", hex1, hex2, "27", "normal_ratio1.25");  // delay 6
    editDetails()->page()->addGroupBox(1, 0, 2, 2);
    editDetails()->page()->newGroupBox(str5);
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "14", "normal_ratio1.25");  // pitch 5
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "20", "normal_ratio1.25");  // level 5
    editDetails()->page()->addKnob(1, 0, 1, 1, "10", hex1, hex2, "1A", "normal_ratio1.25");  // fine 5
    editDetails()->page()->addKnob(1, 1, 1, 1, "10", hex1, hex2, "26", "normal_ratio1.25");  // delay 5
    editDetails()->page()->addGroupBox(1, 2, 2, 2);
    editDetails()->page()->newGroupBox(str4);
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "13", "normal_ratio1.25");  // pitch 4
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "1F", "normal_ratio1.25");  // level 4
    editDetails()->page()->addKnob(1, 0, 1, 1, "10", hex1, hex2, "19", "normal_ratio1.25");  // fine 4
    editDetails()->page()->addKnob(1, 1, 1, 1, "10", hex1, hex2, "25", "normal_ratio1.25");  // delay 4
    editDetails()->page()->addGroupBox(1, 4, 2, 2);
    editDetails()->page()->newGroupBox(str3);
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "12", "normal_ratio1.25");  // pitch 3
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "1E", "normal_ratio1.25");  // level 3
    editDetails()->page()->addKnob(1, 0, 1, 1, "10", hex1, hex2, "18", "normal_ratio1.25");  // fine 3
    editDetails()->page()->addKnob(1, 1, 1, 1, "10", hex1, hex2, "24", "normal_ratio1.25");  // delay 3
    editDetails()->page()->addGroupBox(1, 6, 2, 2);
    editDetails()->page()->newGroupBox(str2);
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "11", "normal_ratio1.25");  // pitch 2
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "1D", "normal_ratio1.25");  // level 2
    editDetails()->page()->addKnob(1, 0, 1, 1, "10", hex1, hex2, "17", "normal_ratio1.25");  // fine 2
    editDetails()->page()->addKnob(1, 1, 1, 1, "10", hex1, hex2, "23", "normal_ratio1.25");  // delay 2
    editDetails()->page()->addGroupBox(1, 8, 2, 2);
    editDetails()->page()->newGroupBox(str1);
    editDetails()->page()->addKnob(1, 5, 1, 1, "10", hex1, hex2, "10", "normal_ratio1.25");  // pitch 1
    editDetails()->page()->addKnob(1, 11, 1, 1, "10", hex1, hex2, "1C", "normal_ratio1.25"); // level 1
    editDetails()->page()->addKnob(2, 5, 1, 1, "10", hex1, hex2, "16", "normal_ratio1.25");  // fine 1
    editDetails()->page()->addKnob(2, 11, 1, 1, "10", hex1, hex2, "22", "normal_ratio1.25"); // delay 1
    editDetails()->page()->addGroupBox(1, 10, 2, 2);
    editDetails()->page()->addSystemOverride(1, 0, 2, 12, "10", hex1, hex2, "0F", "00");
    editDetails()->page()->addGroupBox(0, 1, 1, 1);
}
