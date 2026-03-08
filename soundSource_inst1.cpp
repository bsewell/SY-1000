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
    str1 = "String 1";
    str2 = "String 2";
    str3 = "String 3";
    str4 = "String 4";
    str5 = "String 5";
    str6 = "String 6";
    hex1 = "00";
    hex2 = "1F";
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex1 = "02";
        hex2 = "1E";
        str1 = "String Hi C";
        str2 = "String 1";
        str3 = "String 2";
        str4 = "String 3";
        str5 = "String 4";
        str6 = "String Lo B";
    };

    editDetails()->page()->newGroupBox("Instrument 1");
    editDetails()->page()->newStackControl(0);
    editDetails()->page()->addSwitch(0, 0, 1, 1, "10", hex1, "15", "00", "left");  // on-off
    editDetails()->page()->addComboBox(0, 1, 1, 2, "10", hex1, "15", "01", "large_inst"); //type
    editDetails()->page()->addStackControl();
    //editDetails()->page()->addSystemOverride(0, 0, 1, 2, "10", hex1, "12", "00", "01", "less_than");
    editDetails()->page()->insertStackField(0, 1, 1, 2, 8);
    editDetails()->page()->addKnob(0, 3, 1, 2, "10", hex1, "15", "02", "normal_ratio1.5", "right");  // LEVEL
    editDetails()->page()->addSwitch(0, 5, 1, 1, "10", hex1, "15", "03");  //nor-mix
    editDetails()->page()->addDataKnob(0, 6, 1, 2, "10", hex1, "15", "04", "0~200_ratio1.25", "left");  //nor-mix level
    editDetails()->page()->addGroupBox(0, 0, 1, 1);

    // *********************************************************************************************************** DYNAMIC SYNTH ********************
    editDetails()->page()->newStackField(0);
    editDetails()->page()->newStackControl(1);
    editDetails()->page()->addTabBar(0, 0, 1, 20, "Tables", "00", "00", "11"); //page select Dynamic synth
    editDetails()->page()->addStackControl();
    editDetails()->page()->insertStackField(1, 1, 0, 2, 20);
    editDetails()->page()->addStackField();
    // ***********************************************************************************************************************************************

    editDetails()->page()->newStackField(1);
    editDetails()->page()->newGroupBox("Dynamic Synth");
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, "16", "04"); //wave type
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, "16", "05", "normal_ratio1.5");  // pitch
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, "16", "06", "normal_ratio1.25");  // fine
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, "16", "0A", "normal_ratio1.25");  // detune
    editDetails()->page()->addSystemOverride(0, 3, 1, 1, "10", hex1, "16", "04", "05", "not_equal"); // saw detune
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, "16", "0B", "normal_ratio1.25");  // sharpness
    editDetails()->page()->addSystemOverride(0, 4, 1, 1, "10", hex1, "16", "04", "06", "not_equal"); // noise sharpness
    editDetails()->page()->addKnob(0, 5, 1, 1, "10", hex1, "16", "0C", "normal_ratio1.25");  // feedback
    editDetails()->page()->addKnob(0, 6, 1, 1, "10", hex1, "16", "0D", "normal_ratio1.25");  // harmonics
    editDetails()->page()->addSystemOverride(0, 5, 1, 2, "10", hex1, "16", "04", "07", "not_equal"); // fb & harmonics
    editDetails()->page()->addKnob(0, 7, 1, 1, "10", hex1, "16", "0E", "normal_ratio1.25");  // s-saw detune
    editDetails()->page()->addSystemOverride(0, 7, 1, 1, "10", hex1, "16", "04", "08", "not_equal"); // s-saw detune
    editDetails()->page()->addSystemOverride(0, 1, 1, 7, "10", hex1, "16", "04", "09"); // input
    editDetails()->page()->addGroupBox(0, 0, 1, 1);
    editDetails()->page()->newGroupBox("PWM");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, "16", "07", "normal_ratio1.25");  //
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, "16", "08", "normal_ratio1.25");  //
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, "16", "09", "normal_ratio1.25");  //
    editDetails()->page()->addSystemOverride(0, 0, 1, 3, "10", hex1, "16", "04", "04", "not_equal");
    editDetails()->page()->addGroupBox(0, 1, 1, 1);
    editDetails()->page()->newGroupBox("Pitch");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, "16", "0F", "normal_ratio1.25");  //
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, "16", "10", "normal_ratio1.25");  //
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, "16", "11", "normal_ratio1.25");  //
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, "16", "12", "normal_ratio1.25");  //
    editDetails()->page()->addSystemOverride(0, 0, 1, 4, "10", hex1, "16", "04", "09"); // input
    editDetails()->page()->addGroupBox(0, 2, 1, 1);
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(1);
    editDetails()->page()->newGroupBox("Filter");
    editDetails()->page()->addSwitch(0, 0, 1, 1, "10", hex1, "16", "1C"); //off/on
    editDetails()->page()->addComboBox(0, 1, 1, 1, "10", hex1, "16", "1D");  // type
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, "16", "1E", "normal_ratio1.25");  // slope
    editDetails()->page()->addSystemOverride(0, 2, 1, 1, "10", hex1, "16", "1D", "04", "less_than"); // slope
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, "16", "1F", "normal_ratio1.25");  // cutoff
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, "16", "20", "normal_ratio1.25");  // resonance
    editDetails()->page()->addGroupBox(0, 0, 1, 1);
    editDetails()->page()->newGroupBox("Envelope");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, "16", "21", "normal_ratio1.25");  // flt env attack
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, "16", "22", "normal_ratio1.25");  // flt env depth
    editDetails()->page()->addGroupBox(0, 1, 1, 1);
    editDetails()->page()->newGroupBox("Amp");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, "16", "23", "normal_ratio1.25");  // amp env attack
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, "16", "25", "normal_ratio1.25", "bottom", 70);  // low cut
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, "16", "26", "normal_ratio1.25", "bottom", 70);  // high cut
    editDetails()->page()->addGroupBox(0, 2, 1, 1);
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(1);
    editDetails()->page()->newGroupBox("LFO 1");
    editDetails()->page()->addSwitch(0, 0, 1, 1, "10", hex1, "16", "27"); // 1 off/on
    editDetails()->page()->addComboBox(0, 1, 1, 1, "10", hex1, "16", "28");  // shape
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, "16", "29", "normal_ratio1.25");  // rate
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, "16", "2A", "normal_ratio1.25");  //
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, "16", "2B", "normal_ratio1.25");  //
    editDetails()->page()->addKnob(0, 5, 1, 1, "10", hex1, "16", "2C", "normal_ratio1.25");  //
    editDetails()->page()->addKnob(0, 6, 1, 1, "10", hex1, "16", "2D", "normal_ratio1.25");  //
    editDetails()->page()->addKnob(0, 7, 1, 1, "10", hex1, "16", "2E", "normal_ratio1.25");  //
    editDetails()->page()->addKnob(0, 8, 1, 1, "10", hex1, "16", "2F", "normal_ratio1.25");  //
    editDetails()->page()->addComboBox(0, 9, 1, 1, "10", hex1, "16", "30");  //pitch sync
    editDetails()->page()->addGroupBox(0, 0, 1, 1);

    editDetails()->page()->newGroupBox("LFO 2");
    editDetails()->page()->addSwitch(1, 0, 1, 1, "10", hex1, "16", "31"); // 2 off/on
    editDetails()->page()->addComboBox(1, 1, 1, 1, "10", hex1, "16", "32");  // shape
    editDetails()->page()->addKnob(1, 2, 1, 1, "10", hex1, "16", "33", "normal_ratio1.25");  // rate
    editDetails()->page()->addKnob(1, 3, 1, 1, "10", hex1, "16", "34", "normal_ratio1.25");  //
    editDetails()->page()->addKnob(1, 4, 1, 1, "10", hex1, "16", "35", "normal_ratio1.25");  //
    editDetails()->page()->addKnob(1, 5, 1, 1, "10", hex1, "16", "36", "normal_ratio1.25");  //
    editDetails()->page()->addKnob(1, 6, 1, 1, "10", hex1, "16", "37", "normal_ratio1.25");  //
    editDetails()->page()->addKnob(1, 7, 1, 1, "10", hex1, "16", "38", "normal_ratio1.25");  //
    editDetails()->page()->addKnob(1, 8, 1, 1, "10", hex1, "16", "39", "normal_ratio1.25");  //
    editDetails()->page()->addComboBox(1, 9, 1, 1, "10", hex1, "16", "3A");  //pitch sync
    editDetails()->page()->addGroupBox(1, 0, 1, 1);
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(1);
    editDetails()->page()->newGroupBox("SEQUENCER 1 - Step values shared with SEQ 1 and 2");
    editDetails()->page()->addSwitch(8, 0, 1, 1, "10", hex1, "17", "1E"); // seq 1 off/on
    editDetails()->page()->addComboBox(8, 1, 1, 2, "10", hex1, "16", "3B"); //pitch target
    editDetails()->page()->addComboBox(8, 3, 1, 2, "10", hex1, "16", "3C"); //cutoff target
    editDetails()->page()->addComboBox(8, 5, 1, 2, "10", hex1, "16", "3D"); //level target
    editDetails()->page()->addSwitch(8, 7, 1, 1, "10", hex1, "17", "1F"); // sync 1
    editDetails()->page()->addSwitch(8, 8, 1, 1, "10", hex1, "17", "22"); // 1 shot 1
    editDetails()->page()->addSwitch(8, 9, 1, 2, "10", hex1, "17", "23"); // turbo 1
    editDetails()->page()->addKnob(8, 11, 1, 2, "10", hex1, "17", "21", "normal", "right");  // rate 1

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
    editDetails()->page()->addSpinBox(2, 1, 1, 1, "10", hex1, "16", "3E");  //1 min pitch step
    editDetails()->page()->addSpinBox(1, 1, 1, 1, "10", hex1, "16", "3F");  //1 max
    editDetails()->page()->addSpinBox(2, 2, 1, 1, "10", hex1, "16", "40");  //2 min
    editDetails()->page()->addSpinBox(1, 2, 1, 1, "10", hex1, "16", "41");  //2 max
    editDetails()->page()->addSpinBox(2, 3, 1, 1, "10", hex1, "16", "42");  //3 min
    editDetails()->page()->addSpinBox(1, 3, 1, 1, "10", hex1, "16", "43");  //3 max
    editDetails()->page()->addSpinBox(2, 4, 1, 1, "10", hex1, "16", "44");  //4 min
    editDetails()->page()->addSpinBox(1, 4, 1, 1, "10", hex1, "16", "45");  //4 max
    editDetails()->page()->addSpinBox(2, 5, 1, 1, "10", hex1, "16", "46");  //5 min
    editDetails()->page()->addSpinBox(1, 5, 1, 1, "10", hex1, "16", "47");  //5 max
    editDetails()->page()->addSpinBox(2, 6, 1, 1, "10", hex1, "16", "48"); //6 min
    editDetails()->page()->addSpinBox(1, 6, 1, 1, "10", hex1, "16", "49"); //6 max
    editDetails()->page()->addSpinBox(2, 7, 1, 1, "10", hex1, "16", "4A"); //7 min
    editDetails()->page()->addSpinBox(1, 7, 1, 1, "10", hex1, "16", "4B"); //7 max
    editDetails()->page()->addSpinBox(2, 8, 1, 1, "10", hex1, "16", "4C"); //8 min
    editDetails()->page()->addSpinBox(1, 8, 1, 1, "10", hex1, "16", "4D"); //8 max
    editDetails()->page()->addSpinBox(2, 9, 1, 1, "10", hex1, "16", "4E"); //9 min
    editDetails()->page()->addSpinBox(1, 9, 1, 1, "10", hex1, "16", "4F"); //9 max
    editDetails()->page()->addSpinBox(2, 10, 1, 1, "10", hex1, "16", "50"); //10 min
    editDetails()->page()->addSpinBox(1, 10, 1, 1, "10", hex1, "16", "51"); //10 max
    editDetails()->page()->addSpinBox(2, 11, 1, 1, "10", hex1, "16", "52"); //11 min
    editDetails()->page()->addSpinBox(1, 11, 1, 1, "10", hex1, "16", "53"); //11 max
    editDetails()->page()->addSpinBox(2, 12, 1, 1, "10", hex1, "16", "54"); //12 min
    editDetails()->page()->addSpinBox(1, 12, 1, 1, "10", hex1, "16", "55"); //12 max
    editDetails()->page()->addSpinBox(2, 13, 1, 1, "10", hex1, "16", "56"); //13 min
    editDetails()->page()->addSpinBox(1, 13, 1, 1, "10", hex1, "16", "57"); //13 max
    editDetails()->page()->addSpinBox(2, 14, 1, 1, "10", hex1, "16", "58"); //14 min
    editDetails()->page()->addSpinBox(1, 14, 1, 1, "10", hex1, "16", "59"); //14 max
    editDetails()->page()->addSpinBox(2, 15, 1, 1, "10", hex1, "16", "5A"); //15 min
    editDetails()->page()->addSpinBox(1, 15, 1, 1, "10", hex1, "16", "5B"); //15 max
    editDetails()->page()->addSpinBox(2, 16, 1, 1, "10", hex1, "16", "5C"); //16 min
    editDetails()->page()->addSpinBox(1, 16, 1, 1, "10", hex1, "16", "5D"); //16 max

    editDetails()->page()->addLabel(3, 0, 1, 1, "Cutoff Max ", Qt::AlignLeft);
    editDetails()->page()->addLabel(4, 0, 1, 1, "Cutoff Min ", Qt::AlignLeft);
    editDetails()->page()->addSpinBox(4, 1, 1, 1, "10", hex1, "16", "5E");  //1 min filter cutoff step
    editDetails()->page()->addSpinBox(3, 1, 1, 1, "10", hex1, "16", "5F");  //1 max
    editDetails()->page()->addSpinBox(4, 2, 1, 1, "10", hex1, "16", "60");  //2 min
    editDetails()->page()->addSpinBox(3, 2, 1, 1, "10", hex1, "16", "61");  //2 max
    editDetails()->page()->addSpinBox(4, 3, 1, 1, "10", hex1, "16", "62");  //3 min
    editDetails()->page()->addSpinBox(3, 3, 1, 1, "10", hex1, "16", "63");  //3 max
    editDetails()->page()->addSpinBox(4, 4, 1, 1, "10", hex1, "16", "64");  //4 min
    editDetails()->page()->addSpinBox(3, 4, 1, 1, "10", hex1, "16", "65");  //4 max
    editDetails()->page()->addSpinBox(4, 5, 1, 1, "10", hex1, "16", "66");  //5 min
    editDetails()->page()->addSpinBox(3, 5, 1, 1, "10", hex1, "16", "67");  //5 max
    editDetails()->page()->addSpinBox(4, 6, 1, 1, "10", hex1, "16", "68");  //6 min
    editDetails()->page()->addSpinBox(3, 6, 1, 1, "10", hex1, "16", "69"); //6 max
    editDetails()->page()->addSpinBox(4, 7, 1, 1, "10", hex1, "16", "6A"); //7 min
    editDetails()->page()->addSpinBox(3, 7, 1, 1, "10", hex1, "16", "6B"); //7 max
    editDetails()->page()->addSpinBox(4, 8, 1, 1, "10", hex1, "16", "6C"); //8 min
    editDetails()->page()->addSpinBox(3, 8, 1, 1, "10", hex1, "16", "6D"); //8 max
    editDetails()->page()->addSpinBox(4, 9, 1, 1, "10", hex1, "16", "6E"); //9 min
    editDetails()->page()->addSpinBox(3, 9, 1, 1, "10", hex1, "16", "6F"); //9 max
    editDetails()->page()->addSpinBox(4, 10, 1, 1, "10", hex1, "16", "70"); //10 min
    editDetails()->page()->addSpinBox(3, 10, 1, 1, "10", hex1, "16", "71"); //10 max
    editDetails()->page()->addSpinBox(4, 11, 1, 1, "10", hex1, "16", "72"); //11 min
    editDetails()->page()->addSpinBox(3, 11, 1, 1, "10", hex1, "16", "73"); //11 max
    editDetails()->page()->addSpinBox(4, 12, 1, 1, "10", hex1, "16", "74"); //12 min
    editDetails()->page()->addSpinBox(3, 12, 1, 1, "10", hex1, "16", "75"); //12 max
    editDetails()->page()->addSpinBox(4, 13, 1, 1, "10", hex1, "16", "76"); //13 min
    editDetails()->page()->addSpinBox(3, 13, 1, 1, "10", hex1, "16", "77"); //13 max
    editDetails()->page()->addSpinBox(4, 14, 1, 1, "10", hex1, "16", "78"); //14 min
    editDetails()->page()->addSpinBox(3, 14, 1, 1, "10", hex1, "16", "79"); //14 max
    editDetails()->page()->addSpinBox(4, 15, 1, 1, "10", hex1, "16", "7A"); //15 min
    editDetails()->page()->addSpinBox(3, 15, 1, 1, "10", hex1, "16", "7B"); //15 max
    editDetails()->page()->addSpinBox(4, 16, 1, 1, "10", hex1, "16", "7C"); //16 min
    editDetails()->page()->addSpinBox(3, 16, 1, 1, "10", hex1, "16", "7D"); //16 max

    editDetails()->page()->addLabel(5, 0, 1, 1, "Level Max ", Qt::AlignLeft);
    editDetails()->page()->addLabel(6, 0, 1, 1, "Level Min ", Qt::AlignLeft);
    editDetails()->page()->addSpinBox(6, 1, 1, 1, "10", hex1, "16", "7E");  //1 min inst level step
    editDetails()->page()->addSpinBox(5, 1, 1, 1, "10", hex1, "16", "7F");  //1 max
    editDetails()->page()->addSpinBox(6, 2, 1, 1, "10", hex1, "17", "00");  //2 min
    editDetails()->page()->addSpinBox(5, 2, 1, 1, "10", hex1, "17", "01");  //2 max
    editDetails()->page()->addSpinBox(6, 3, 1, 1, "10", hex1, "17", "02");  //3 min
    editDetails()->page()->addSpinBox(5, 3, 1, 1, "10", hex1, "17", "03");  //3 max
    editDetails()->page()->addSpinBox(6, 4, 1, 1, "10", hex1, "17", "04");  //4 min
    editDetails()->page()->addSpinBox(5, 4, 1, 1, "10", hex1, "17", "05");  //4 max
    editDetails()->page()->addSpinBox(6, 5, 1, 1, "10", hex1, "17", "06");  //5 min
    editDetails()->page()->addSpinBox(5, 5, 1, 1, "10", hex1, "17", "07");  //5 max
    editDetails()->page()->addSpinBox(6, 6, 1, 1, "10", hex1, "17", "08"); //6 min
    editDetails()->page()->addSpinBox(5, 6, 1, 1, "10", hex1, "17", "09"); //6 max
    editDetails()->page()->addSpinBox(6, 7, 1, 1, "10", hex1, "17", "0A"); //7 min
    editDetails()->page()->addSpinBox(5, 7, 1, 1, "10", hex1, "17", "0B"); //7 max
    editDetails()->page()->addSpinBox(6, 8, 1, 1, "10", hex1, "17", "0C"); //8 min
    editDetails()->page()->addSpinBox(5, 8, 1, 1, "10", hex1, "17", "0D"); //8 max
    editDetails()->page()->addSpinBox(6, 9, 1, 1, "10", hex1, "17", "0E"); //9 min
    editDetails()->page()->addSpinBox(5, 9, 1, 1, "10", hex1, "17", "0F"); //9 max
    editDetails()->page()->addSpinBox(6, 10, 1, 1, "10", hex1, "17", "10"); //10 min
    editDetails()->page()->addSpinBox(5, 10, 1, 1, "10", hex1, "17", "11"); //10 max
    editDetails()->page()->addSpinBox(6, 11, 1, 1, "10", hex1, "17", "12"); //11 min
    editDetails()->page()->addSpinBox(5, 11, 1, 1, "10", hex1, "17", "13"); //11 max
    editDetails()->page()->addSpinBox(6, 12, 1, 1, "10", hex1, "17", "14"); //12 min
    editDetails()->page()->addSpinBox(5, 12, 1, 1, "10", hex1, "17", "15"); //12 max
    editDetails()->page()->addSpinBox(6, 13, 1, 1, "10", hex1, "17", "16"); //13 min
    editDetails()->page()->addSpinBox(5, 13, 1, 1, "10", hex1, "17", "17"); //13 max
    editDetails()->page()->addSpinBox(6, 14, 1, 1, "10", hex1, "17", "18"); //14 min
    editDetails()->page()->addSpinBox(5, 14, 1, 1, "10", hex1, "17", "19"); //14 max
    editDetails()->page()->addSpinBox(6, 15, 1, 1, "10", hex1, "17", "1A"); //15 min
    editDetails()->page()->addSpinBox(5, 15, 1, 1, "10", hex1, "17", "1B"); //15 max
    editDetails()->page()->addSpinBox(6, 16, 1, 1, "10", hex1, "17", "1C"); //16 min
    editDetails()->page()->addSpinBox(5, 16, 1, 1, "10", hex1, "17", "1D"); //16 max

    editDetails()->page()->addLabel(7, 0, 1, 1, "Seq Curve ", Qt::AlignLeft);
    editDetails()->page()->addComboBox(7, 1, 1, 1, "10", hex1, "17", "24", "icon_seq");  //1 curve seq 1
    editDetails()->page()->addComboBox(7, 2, 1, 1, "10", hex1, "17", "25", "icon_seq");  //2
    editDetails()->page()->addComboBox(7, 3, 1, 1, "10", hex1, "17", "26", "icon_seq");  //3
    editDetails()->page()->addComboBox(7, 4, 1, 1, "10", hex1, "17", "27", "icon_seq");  //4
    editDetails()->page()->addComboBox(7, 5, 1, 1, "10", hex1, "17", "28", "icon_seq");  //5
    editDetails()->page()->addComboBox(7, 6, 1, 1, "10", hex1, "17", "29", "icon_seq");  //6
    editDetails()->page()->addComboBox(7, 7, 1, 1, "10", hex1, "17", "2A", "icon_seq");  //7
    editDetails()->page()->addComboBox(7, 8, 1, 1, "10", hex1, "17", "2B", "icon_seq");  //8
    editDetails()->page()->addComboBox(7, 9, 1, 1, "10", hex1, "17", "2C", "icon_seq");  //9
    editDetails()->page()->addComboBox(7, 10, 1, 1, "10", hex1, "17", "2D", "icon_seq"); //10
    editDetails()->page()->addComboBox(7, 11, 1, 1, "10", hex1, "17", "2E", "icon_seq"); //11
    editDetails()->page()->addComboBox(7, 12, 1, 1, "10", hex1, "17", "2F", "icon_seq"); //12
    editDetails()->page()->addComboBox(7, 13, 1, 1, "10", hex1, "17", "30", "icon_seq"); //13
    editDetails()->page()->addComboBox(7, 14, 1, 1, "10", hex1, "17", "31", "icon_seq"); //14
    editDetails()->page()->addComboBox(7, 15, 1, 1, "10", hex1, "17", "32", "icon_seq"); //15
    editDetails()->page()->addComboBox(7, 16, 1, 1, "10", hex1, "17", "33", "icon_seq"); //16

    editDetails()->page()->addSystemOverride(0, 2, 8, 15, "10", hex1, "17", "20", "01");
    editDetails()->page()->addSystemOverride(0, 3, 8, 14, "10", hex1, "17", "20", "02");
    editDetails()->page()->addSystemOverride(0, 4, 8, 13, "10", hex1, "17", "20", "03");
    editDetails()->page()->addSystemOverride(0, 5, 8, 12, "10", hex1, "17", "20", "04");
    editDetails()->page()->addSystemOverride(0, 6, 8, 11, "10", hex1, "17", "20", "05");
    editDetails()->page()->addSystemOverride(0, 7, 8, 10, "10", hex1, "17", "20", "06");
    editDetails()->page()->addSystemOverride(0, 8, 8, 9, "10", hex1, "17", "20", "07");
    editDetails()->page()->addSystemOverride(0, 9, 8, 8, "10", hex1, "17", "20", "08");
    editDetails()->page()->addSystemOverride(0, 10, 8, 7, "10", hex1, "17", "20", "09");
    editDetails()->page()->addSystemOverride(0, 11, 8, 6, "10", hex1, "17", "20", "0A");
    editDetails()->page()->addSystemOverride(0, 12, 8, 5, "10", hex1, "17", "20", "0B");
    editDetails()->page()->addSystemOverride(0, 13, 8, 4, "10", hex1, "17", "20", "0C");
    editDetails()->page()->addSystemOverride(0, 14, 8, 3, "10", hex1, "17", "20", "0D");
    editDetails()->page()->addSystemOverride(0, 15, 8, 2, "10", hex1, "17", "20", "0E");
    editDetails()->page()->addSystemOverride(0, 16, 8, 1, "10", hex1, "17", "20", "0F");
    editDetails()->page()->addKnob(8, 13, 1, 2, "10", hex1, "17", "20", "normal", "right");  // SEQ 1 length.
    editDetails()->page()->addGroupBox(0, 0, 1, 1);
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(1);
    editDetails()->page()->newGroupBox("SEQUENCER 2 - Step values shared with SEQ 1 and 2");
    editDetails()->page()->addSwitch(8, 0, 1, 1, "10", hex1, "17", "34"); // seq 2 off/on
    editDetails()->page()->addComboBox(8, 1, 1, 2, "10", hex1, "16", "3B"); //pitch target
    editDetails()->page()->addComboBox(8, 3, 1, 2, "10", hex1, "16", "3C"); //cutoff target
    editDetails()->page()->addComboBox(8, 5, 1, 2, "10", hex1, "16", "3D"); //level target
    editDetails()->page()->addSwitch(8, 7, 1, 1, "10", hex1, "17", "35"); // sync 2
    editDetails()->page()->addSwitch(8, 8, 1, 1, "10", hex1, "17", "38"); // 1 shot 2
    editDetails()->page()->addSwitch(8, 9, 1, 2, "10", hex1, "17", "39"); // turbo 2
    editDetails()->page()->addKnob(8, 11, 1, 2, "10", hex1, "17", "37", "normal", "right");  // rate 2

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
    editDetails()->page()->addSpinBox(2, 1, 1, 1, "10", hex1, "16", "3E");  //1 min pitch step
    editDetails()->page()->addSpinBox(1, 1, 1, 1, "10", hex1, "16", "3F");  //1 max
    editDetails()->page()->addSpinBox(2, 2, 1, 1, "10", hex1, "16", "40");  //2 min
    editDetails()->page()->addSpinBox(1, 2, 1, 1, "10", hex1, "16", "41");  //2 max
    editDetails()->page()->addSpinBox(2, 3, 1, 1, "10", hex1, "16", "42");  //3 min
    editDetails()->page()->addSpinBox(1, 3, 1, 1, "10", hex1, "16", "43");  //3 max
    editDetails()->page()->addSpinBox(2, 4, 1, 1, "10", hex1, "16", "44");  //4 min
    editDetails()->page()->addSpinBox(1, 4, 1, 1, "10", hex1, "16", "45");  //4 max
    editDetails()->page()->addSpinBox(2, 5, 1, 1, "10", hex1, "16", "46");  //5 min
    editDetails()->page()->addSpinBox(1, 5, 1, 1, "10", hex1, "16", "47");  //5 max
    editDetails()->page()->addSpinBox(2, 6, 1, 1, "10", hex1, "16", "48"); //6 min
    editDetails()->page()->addSpinBox(1, 6, 1, 1, "10", hex1, "16", "49"); //6 max
    editDetails()->page()->addSpinBox(2, 7, 1, 1, "10", hex1, "16", "4A"); //7 min
    editDetails()->page()->addSpinBox(1, 7, 1, 1, "10", hex1, "16", "4B"); //7 max
    editDetails()->page()->addSpinBox(2, 8, 1, 1, "10", hex1, "16", "4C"); //8 min
    editDetails()->page()->addSpinBox(1, 8, 1, 1, "10", hex1, "16", "4D"); //8 max
    editDetails()->page()->addSpinBox(2, 9, 1, 1, "10", hex1, "16", "4E"); //9 min
    editDetails()->page()->addSpinBox(1, 9, 1, 1, "10", hex1, "16", "4F"); //9 max
    editDetails()->page()->addSpinBox(2, 10, 1, 1, "10", hex1, "16", "50"); //10 min
    editDetails()->page()->addSpinBox(1, 10, 1, 1, "10", hex1, "16", "51"); //10 max
    editDetails()->page()->addSpinBox(2, 11, 1, 1, "10", hex1, "16", "52"); //11 min
    editDetails()->page()->addSpinBox(1, 11, 1, 1, "10", hex1, "16", "53"); //11 max
    editDetails()->page()->addSpinBox(2, 12, 1, 1, "10", hex1, "16", "54"); //12 min
    editDetails()->page()->addSpinBox(1, 12, 1, 1, "10", hex1, "16", "55"); //12 max
    editDetails()->page()->addSpinBox(2, 13, 1, 1, "10", hex1, "16", "56"); //13 min
    editDetails()->page()->addSpinBox(1, 13, 1, 1, "10", hex1, "16", "57"); //13 max
    editDetails()->page()->addSpinBox(2, 14, 1, 1, "10", hex1, "16", "58"); //14 min
    editDetails()->page()->addSpinBox(1, 14, 1, 1, "10", hex1, "16", "59"); //14 max
    editDetails()->page()->addSpinBox(2, 15, 1, 1, "10", hex1, "16", "5A"); //15 min
    editDetails()->page()->addSpinBox(1, 15, 1, 1, "10", hex1, "16", "5B"); //15 max
    editDetails()->page()->addSpinBox(2, 16, 1, 1, "10", hex1, "16", "5C"); //16 min
    editDetails()->page()->addSpinBox(1, 16, 1, 1, "10", hex1, "16", "5D"); //16 max

    editDetails()->page()->addLabel(3, 0, 1, 1, "Cutoff Max", Qt::AlignLeft);
    editDetails()->page()->addLabel(4, 0, 1, 1, "Cutoff Min", Qt::AlignLeft);
    editDetails()->page()->addSpinBox(4, 1, 1, 1, "10", hex1, "16", "5E");  //1 min filter cutoff step
    editDetails()->page()->addSpinBox(3, 1, 1, 1, "10", hex1, "16", "5F");  //1 max
    editDetails()->page()->addSpinBox(4, 2, 1, 1, "10", hex1, "16", "60");  //2 min
    editDetails()->page()->addSpinBox(3, 2, 1, 1, "10", hex1, "16", "61");  //2 max
    editDetails()->page()->addSpinBox(4, 3, 1, 1, "10", hex1, "16", "62");  //3 min
    editDetails()->page()->addSpinBox(3, 3, 1, 1, "10", hex1, "16", "63");  //3 max
    editDetails()->page()->addSpinBox(4, 4, 1, 1, "10", hex1, "16", "64");  //4 min
    editDetails()->page()->addSpinBox(3, 4, 1, 1, "10", hex1, "16", "65");  //4 max
    editDetails()->page()->addSpinBox(4, 5, 1, 1, "10", hex1, "16", "66");  //5 min
    editDetails()->page()->addSpinBox(3, 5, 1, 1, "10", hex1, "16", "67");  //5 max
    editDetails()->page()->addSpinBox(4, 6, 1, 1, "10", hex1, "16", "68");  //6 min
    editDetails()->page()->addSpinBox(3, 6, 1, 1, "10", hex1, "16", "69"); //6 max
    editDetails()->page()->addSpinBox(4, 7, 1, 1, "10", hex1, "16", "6A"); //7 min
    editDetails()->page()->addSpinBox(3, 7, 1, 1, "10", hex1, "16", "6B"); //7 max
    editDetails()->page()->addSpinBox(4, 8, 1, 1, "10", hex1, "16", "6C"); //8 min
    editDetails()->page()->addSpinBox(3, 8, 1, 1, "10", hex1, "16", "6D"); //8 max
    editDetails()->page()->addSpinBox(4, 9, 1, 1, "10", hex1, "16", "6E"); //9 min
    editDetails()->page()->addSpinBox(3, 9, 1, 1, "10", hex1, "16", "6F"); //9 max
    editDetails()->page()->addSpinBox(4, 10, 1, 1, "10", hex1, "16", "70"); //10 min
    editDetails()->page()->addSpinBox(3, 10, 1, 1, "10", hex1, "16", "71"); //10 max
    editDetails()->page()->addSpinBox(4, 11, 1, 1, "10", hex1, "16", "72"); //11 min
    editDetails()->page()->addSpinBox(3, 11, 1, 1, "10", hex1, "16", "73"); //11 max
    editDetails()->page()->addSpinBox(4, 12, 1, 1, "10", hex1, "16", "74"); //12 min
    editDetails()->page()->addSpinBox(3, 12, 1, 1, "10", hex1, "16", "75"); //12 max
    editDetails()->page()->addSpinBox(4, 13, 1, 1, "10", hex1, "16", "76"); //13 min
    editDetails()->page()->addSpinBox(3, 13, 1, 1, "10", hex1, "16", "77"); //13 max
    editDetails()->page()->addSpinBox(4, 14, 1, 1, "10", hex1, "16", "78"); //14 min
    editDetails()->page()->addSpinBox(3, 14, 1, 1, "10", hex1, "16", "79"); //14 max
    editDetails()->page()->addSpinBox(4, 15, 1, 1, "10", hex1, "16", "7A"); //15 min
    editDetails()->page()->addSpinBox(3, 15, 1, 1, "10", hex1, "16", "7B"); //15 max
    editDetails()->page()->addSpinBox(4, 16, 1, 1, "10", hex1, "16", "7C"); //16 min
    editDetails()->page()->addSpinBox(3, 16, 1, 1, "10", hex1, "16", "7D"); //16 max

    editDetails()->page()->addLabel(5, 0, 1, 1, "Level Max", Qt::AlignLeft);
    editDetails()->page()->addLabel(6, 0, 1, 1, "Level Min", Qt::AlignLeft);
    editDetails()->page()->addSpinBox(6, 1, 1, 1, "10", hex1, "16", "7E");  //1 min inst level step
    editDetails()->page()->addSpinBox(5, 1, 1, 1, "10", hex1, "16", "7F");  //1 max
    editDetails()->page()->addSpinBox(6, 2, 1, 1, "10", hex1, "17", "00");  //2 min
    editDetails()->page()->addSpinBox(5, 2, 1, 1, "10", hex1, "17", "01");  //2 max
    editDetails()->page()->addSpinBox(6, 3, 1, 1, "10", hex1, "17", "02");  //3 min
    editDetails()->page()->addSpinBox(5, 3, 1, 1, "10", hex1, "17", "03");  //3 max
    editDetails()->page()->addSpinBox(6, 4, 1, 1, "10", hex1, "17", "04");  //4 min
    editDetails()->page()->addSpinBox(5, 4, 1, 1, "10", hex1, "17", "05");  //4 max
    editDetails()->page()->addSpinBox(6, 5, 1, 1, "10", hex1, "17", "06");  //5 min
    editDetails()->page()->addSpinBox(5, 5, 1, 1, "10", hex1, "17", "07");  //5 max
    editDetails()->page()->addSpinBox(6, 6, 1, 1, "10", hex1, "17", "08"); //6 min
    editDetails()->page()->addSpinBox(5, 6, 1, 1, "10", hex1, "17", "09"); //6 max
    editDetails()->page()->addSpinBox(6, 7, 1, 1, "10", hex1, "17", "0A"); //7 min
    editDetails()->page()->addSpinBox(5, 7, 1, 1, "10", hex1, "17", "0B"); //7 max
    editDetails()->page()->addSpinBox(6, 8, 1, 1, "10", hex1, "17", "0C"); //8 min
    editDetails()->page()->addSpinBox(5, 8, 1, 1, "10", hex1, "17", "0D"); //8 max
    editDetails()->page()->addSpinBox(6, 9, 1, 1, "10", hex1, "17", "0E"); //9 min
    editDetails()->page()->addSpinBox(5, 9, 1, 1, "10", hex1, "17", "0F"); //9 max
    editDetails()->page()->addSpinBox(6, 10, 1, 1, "10", hex1, "17", "10"); //10 min
    editDetails()->page()->addSpinBox(5, 10, 1, 1, "10", hex1, "17", "11"); //10 max
    editDetails()->page()->addSpinBox(6, 11, 1, 1, "10", hex1, "17", "12"); //11 min
    editDetails()->page()->addSpinBox(5, 11, 1, 1, "10", hex1, "17", "13"); //11 max
    editDetails()->page()->addSpinBox(6, 12, 1, 1, "10", hex1, "17", "14"); //12 min
    editDetails()->page()->addSpinBox(5, 12, 1, 1, "10", hex1, "17", "15"); //12 max
    editDetails()->page()->addSpinBox(6, 13, 1, 1, "10", hex1, "17", "16"); //13 min
    editDetails()->page()->addSpinBox(5, 13, 1, 1, "10", hex1, "17", "17"); //13 max
    editDetails()->page()->addSpinBox(6, 14, 1, 1, "10", hex1, "17", "18"); //14 min
    editDetails()->page()->addSpinBox(5, 14, 1, 1, "10", hex1, "17", "19"); //14 max
    editDetails()->page()->addSpinBox(6, 15, 1, 1, "10", hex1, "17", "1A"); //15 min
    editDetails()->page()->addSpinBox(5, 15, 1, 1, "10", hex1, "17", "1B"); //15 max
    editDetails()->page()->addSpinBox(6, 16, 1, 1, "10", hex1, "17", "1C"); //16 min
    editDetails()->page()->addSpinBox(5, 16, 1, 1, "10", hex1, "17", "1D"); //16 max

    editDetails()->page()->addLabel(7, 0, 1, 1, "Seq Curve", Qt::AlignLeft);
    editDetails()->page()->addComboBox(7, 1, 1, 1, "10", hex1, "17", "3A", "icon_seq");  //1 curve seq 2
    editDetails()->page()->addComboBox(7, 2, 1, 1, "10", hex1, "17", "3B", "icon_seq");  //2
    editDetails()->page()->addComboBox(7, 3, 1, 1, "10", hex1, "17", "3C", "icon_seq");  //3
    editDetails()->page()->addComboBox(7, 4, 1, 1, "10", hex1, "17", "3D", "icon_seq");  //4
    editDetails()->page()->addComboBox(7, 5, 1, 1, "10", hex1, "17", "3E", "icon_seq");  //5
    editDetails()->page()->addComboBox(7, 6, 1, 1, "10", hex1, "17", "3F", "icon_seq");  //6
    editDetails()->page()->addComboBox(7, 7, 1, 1, "10", hex1, "17", "40", "icon_seq");  //7
    editDetails()->page()->addComboBox(7, 8, 1, 1, "10", hex1, "17", "41", "icon_seq");  //8
    editDetails()->page()->addComboBox(7, 9, 1, 1, "10", hex1, "17", "42", "icon_seq");  //9
    editDetails()->page()->addComboBox(7, 10, 1, 1, "10", hex1, "17", "43", "icon_seq"); //10
    editDetails()->page()->addComboBox(7, 11, 1, 1, "10", hex1, "17", "44", "icon_seq"); //11
    editDetails()->page()->addComboBox(7, 12, 1, 1, "10", hex1, "17", "45", "icon_seq"); //12
    editDetails()->page()->addComboBox(7, 13, 1, 1, "10", hex1, "17", "46", "icon_seq"); //13
    editDetails()->page()->addComboBox(7, 14, 1, 1, "10", hex1, "17", "47", "icon_seq"); //14
    editDetails()->page()->addComboBox(7, 15, 1, 1, "10", hex1, "17", "48", "icon_seq"); //15
    editDetails()->page()->addComboBox(7, 16, 1, 1, "10", hex1, "17", "49", "icon_seq"); //16
    editDetails()->page()->addSystemOverride(0, 2, 8, 15, "10", hex1, "17", "36", "01");
    editDetails()->page()->addSystemOverride(0, 3, 8, 14, "10", hex1, "17", "36", "02");
    editDetails()->page()->addSystemOverride(0, 4, 8, 13, "10", hex1, "17", "36", "03");
    editDetails()->page()->addSystemOverride(0, 5, 8, 12, "10", hex1, "17", "36", "04");
    editDetails()->page()->addSystemOverride(0, 6, 8, 11, "10", hex1, "17", "36", "05");
    editDetails()->page()->addSystemOverride(0, 7, 8, 10, "10", hex1, "17", "36", "06");
    editDetails()->page()->addSystemOverride(0, 8, 8, 9, "10", hex1, "17", "36", "07");
    editDetails()->page()->addSystemOverride(0, 9, 8, 8, "10", hex1, "17", "36", "08");
    editDetails()->page()->addSystemOverride(0, 10, 8, 7, "10", hex1, "17", "36", "09");
    editDetails()->page()->addSystemOverride(0, 11, 8, 6, "10", hex1, "17", "36", "0A");
    editDetails()->page()->addSystemOverride(0, 12, 8, 5, "10", hex1, "17", "36", "0B");
    editDetails()->page()->addSystemOverride(0, 13, 8, 4, "10", hex1, "17", "36", "0C");
    editDetails()->page()->addSystemOverride(0, 14, 8, 3, "10", hex1, "17", "36", "0D");
    editDetails()->page()->addSystemOverride(0, 15, 8, 2, "10", hex1, "17", "36", "0E");
    editDetails()->page()->addSystemOverride(0, 16, 8, 1, "10", hex1, "17", "36", "0F");
    editDetails()->page()->addKnob(8, 13, 1, 2, "10", hex1, "17", "36", "normal", "right");  // SEQ 2 length.
    editDetails()->page()->addGroupBox(0, 0, 1, 1);
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(1);
    stringLevels();
    editDetails()->page()->newGroupBox("Synth Layer (note range)");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, "17", "4A", "normal_ratio1.25"); //lwr fade
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, "17", "4B", "normal_ratio1.5");  //lwr
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, "17", "4C", "normal_ratio1.5");  //upper
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, "17", "4D", "inverse_ratio1.25");//upper fade
    editDetails()->page()->addGroupBox(0, 6, 1, 4);
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(1);
    altTuning();
    editDetails()->page()->addStackField();


    // *********************************************************************************************************** OSC SYNTH ***********************
    editDetails()->page()->newStackField(0);
    editDetails()->page()->newStackControl(2);
    editDetails()->page()->addTabBar(0, 0, 1, 1, "Tables", "00", "00", "12"); //page select OSC synth
    editDetails()->page()->addStackControl();
    editDetails()->page()->insertStackField(2, 1, 0, 1, 1);
    editDetails()->page()->addStackField();
    // *********************************************************************************************************************************************


    editDetails()->page()->newStackField(2);
    editDetails()->page()->newGroupBox("OSC Synth");
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, "18", "00"); //wave type
    editDetails()->page()->addGroupBox(0, 0, 2, 1);

    editDetails()->page()->newGroupBox("OSC 1");
    editDetails()->page()->addComboBox(0, 1, 1, 1, "10", hex1, "18", "01"); //wave type 1
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, "18", "02", "normal_ratio1.25"); //pitch
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, "18", "03", "normal"); //fine
    editDetails()->page()->addGroupBox(0, 1, 1, 1);
    editDetails()->page()->newGroupBox("Pulse Width 1");
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, "18", "04", "normal_ratio1.25"); //wave type
    editDetails()->page()->addKnob(0, 5, 1, 1, "10", hex1, "18", "05", "normal_ratio1.25"); //wave type
    editDetails()->page()->addSystemOverride(0, 4, 1, 2, "10", hex1, "18", "01", "04", "not_equal"); // pwm
    editDetails()->page()->addGroupBox(0, 2, 1, 1);
    editDetails()->page()->newGroupBox("Pitch Envelope 1");
    editDetails()->page()->addKnob(0, 6, 1, 1, "10", hex1, "18", "06", "normal"); //wave type
    editDetails()->page()->addKnob(0, 7, 1, 1, "10", hex1, "18", "07", "normal"); //wave type
    editDetails()->page()->addKnob(0, 8, 1, 1, "10", hex1, "18", "08", "normal"); //wave type
    editDetails()->page()->addGroupBox(0, 3, 1, 1);
    editDetails()->page()->newGroupBox("Level 1");
    editDetails()->page()->addKnob(0, 9, 1, 1, "10", hex1, "18", "09", "normal_ratio1.25"); //level
    editDetails()->page()->addGroupBox(0, 4, 1, 1);

    editDetails()->page()->newGroupBox("OSC 2");
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, "18", "0A"); //wave type 2
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, "18", "0B", "normal_ratio1.25"); //pitch
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, "18", "0C", "normal"); //fine
    editDetails()->page()->addSystemOverride(0, 0, 1, 3, "10", hex1, "18", "00", "00");
    editDetails()->page()->addGroupBox(1, 1, 1, 1);
    editDetails()->page()->newGroupBox("Pulse Width 2");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, "18", "0D", "normal_ratio1.25"); //wave type
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, "18", "0E", "normal_ratio1.25"); //wave type
    editDetails()->page()->addSystemOverride(0, 0, 1, 2, "10", hex1, "18", "00", "00");
    editDetails()->page()->addSystemOverride(0, 0, 1, 2, "10", hex1, "18", "0A", "04", "not_equal"); // pwm
    editDetails()->page()->addGroupBox(1, 2, 1, 1);
    editDetails()->page()->newGroupBox("Pitch Envelope 2");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, "18", "0F", "normal"); //wave type
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, "18", "10", "normal"); //wave type
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, "18", "11", "normal"); //wave type
    editDetails()->page()->addSystemOverride(0, 0, 1, 3, "10", hex1, "18", "00", "00");
    editDetails()->page()->addGroupBox(1, 3, 1, 1);
    editDetails()->page()->newGroupBox("Level 2");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, "18", "12", "normal_ratio1.25"); //level
    editDetails()->page()->addSystemOverride(0, 0, 1, 1, "10", hex1, "18", "00", "00");
    editDetails()->page()->addGroupBox(1, 4, 1, 1);

    editDetails()->page()->newGroupBox("OSC CONTROL");
    editDetails()->page()->addComboBox(0, 0, 1, 2, "10", hex1, "18", "13"); // mono/poly
    editDetails()->page()->addComboBox(3, 1, 1, 2, "10", hex1, "18", "17"); // porta mode
    editDetails()->page()->addSystemOverride(2, 0, 1, 1, "10", hex1, "18", "13", "00"); //porta mode
    editDetails()->page()->addComboBox(0, 2, 1, 2, "10", hex1, "18", "18"); // hold mode
    editDetails()->page()->addSwitch(1, 1, 1, 1, "10", hex1, "18", "15"); // porta sw
    editDetails()->page()->addKnob(2, 1, 1, 1, "10", hex1, "18", "16", "normal_ratio1.25"); // porta time
    editDetails()->page()->addSwitch(1, 2, 1, 1, "10", hex1, "18", "14"); // chromatic
    editDetails()->page()->addKnob(2, 2, 1, 1, "10", hex1, "18", "19", "normal_ratio1.25"); // low vel cut
    editDetails()->page()->addGroupBox(0, 5, 2, 1);
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(2);
    editDetails()->page()->newGroupBox("Filter");
    editDetails()->page()->addSwitch(0, 0, 1, 1, "10", hex1, "18", "1A"); //off/on
    editDetails()->page()->addComboBox(0, 1, 1, 1, "10", hex1, "18", "1B");  // type
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, "18", "1C", "normal_ratio1.25");  // slope
    editDetails()->page()->addSystemOverride(0, 2, 1, 1, "10", hex1, "18", "1B", "04", "less_than"); // slope
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, "18", "1D", "normal_ratio1.25");  // cutoff
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, "18", "1E", "normal_ratio1.25");  // cutoff follow
    editDetails()->page()->addKnob(0, 5, 1, 1, "10", hex1, "18", "1F", "normal_ratio1.25");  // resonance
    editDetails()->page()->addKnob(0, 6, 1, 1, "10", hex1, "18", "20", "normal_ratio1.25");  // velocity sense
    editDetails()->page()->addGroupBox(0, 0, 1, 1);
    editDetails()->page()->newGroupBox("Filter Envelope");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, "18", "21", "normal_ratio1.25");  // attack
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, "18", "22", "normal_ratio1.25");  // decay
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, "18", "23", "normal_ratio1.25");  // sustain
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, "18", "24", "normal_ratio1.25");  // release
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, "18", "25", "normal_ratio1.25");  // depth
    editDetails()->page()->addGroupBox(0, 1, 1, 1);
    editDetails()->page()->newGroupBox("Amp Envelope");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, "18", "27", "normal_ratio1.25");  // attack
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, "18", "28", "normal_ratio1.25");  // decay
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, "18", "29", "normal_ratio1.25");  // sustain
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, "18", "2A", "normal_ratio1.25");  // release
    editDetails()->page()->addGroupBox(1, 0, 1, 1);
    editDetails()->page()->newGroupBox("Amp");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, "18", "26", "normal_ratio1.25");  // velocity sense
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, "18", "2B", "normal_ratio1.25", "bottom", 70);  // low cut
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, "18", "2C", "normal_ratio1.25", "bottom", 70);  // high cut
    editDetails()->page()->addGroupBox(1, 1, 1, 1);
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(2);
    editDetails()->page()->newGroupBox("LFO 1");
    editDetails()->page()->addSwitch(0, 0, 1, 1, "10", hex1, "18", "2D"); // 1 off/on
    editDetails()->page()->addComboBox(0, 1, 1, 1, "10", hex1, "18", "2E");  // shape
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, "18", "2F", "normal_ratio1.25");  // rate
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, "18", "30", "normal_ratio1.25");  //
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, "18", "31", "normal_ratio1.25");  // pitch 2 depth
    editDetails()->page()->addSystemOverride(0, 4, 1, 1, "10", hex1, "18", "00", "00"); //
    editDetails()->page()->addKnob(0, 5, 1, 1, "10", hex1, "18", "32", "normal_ratio1.25");  //
    editDetails()->page()->addKnob(0, 6, 1, 1, "10", hex1, "18", "33", "normal_ratio1.25");  //
    editDetails()->page()->addKnob(0, 7, 1, 1, "10", hex1, "18", "34", "normal_ratio1.25");  //
    editDetails()->page()->addKnob(0, 8, 1, 1, "10", hex1, "18", "35", "normal_ratio1.25");  //
    editDetails()->page()->addComboBox(0, 9, 1, 1, "10", hex1, "18", "36");  //pitch sync
    editDetails()->page()->addGroupBox(0, 0, 1, 1);

    editDetails()->page()->newGroupBox("LFO 2");
    editDetails()->page()->addSwitch(0, 0, 1, 1, "10", hex1, "18", "37"); // 2 off/on
    editDetails()->page()->addComboBox(0, 1, 1, 1, "10", hex1, "18", "38");  // shape
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, "18", "39", "normal_ratio1.25");  // rate
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, "18", "3A", "normal_ratio1.25");  //
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, "18", "3B", "normal_ratio1.25");  // pitch 2 depth
    editDetails()->page()->addSystemOverride(0, 4, 1, 1, "10", hex1, "18", "00", "00"); //
    editDetails()->page()->addKnob(0, 5, 1, 1, "10", hex1, "18", "3C", "normal_ratio1.25");  //
    editDetails()->page()->addKnob(0, 6, 1, 1, "10", hex1, "18", "3D", "normal_ratio1.25");  //
    editDetails()->page()->addKnob(0, 7, 1, 1, "10", hex1, "18", "3E", "normal_ratio1.25");  //
    editDetails()->page()->addKnob(0, 8, 1, 1, "10", hex1, "18", "3F", "normal_ratio1.25");  //
    editDetails()->page()->addComboBox(0, 9, 1, 1, "10", hex1, "18", "40");  //pitch sync
    editDetails()->page()->addGroupBox(1, 0, 1, 1);
    editDetails()->page()->addStackField();


    editDetails()->page()->newStackField(2);
    stringLevels();
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(2);
    altTuning();
    editDetails()->page()->addStackField();


    // *********************************************************************************************************** GR-300 ***************
    editDetails()->page()->newStackField(0);
    editDetails()->page()->newStackControl(3);
    editDetails()->page()->addTabBar(0, 0, 1, 1, "Tables", "00", "00", "13"); //page select GR300 synth
    editDetails()->page()->addStackControl();
    editDetails()->page()->insertStackField(3, 1, 0, 1, 1);
    editDetails()->page()->addStackField();
    // ***********************************************************************************************************************************


    editDetails()->page()->newStackField(3);
    editDetails()->page()->newGroupBox("GR-300 Synth");
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, "19", "00"); //mode
    editDetails()->page()->addSwitch(1, 0, 1, 1, "10", hex1, "19", "01", "middle"); //comp
    editDetails()->page()->addKnob(1, 1, 2, 1, "10", hex1, "19", "02", "normal_ratio1.25");  //filter cutoff
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, "19", "03", "normal_ratio1.25");  //filter reso
    editDetails()->page()->addGroupBox(0, 0, 2, 1);
    editDetails()->page()->newGroupBox("Envelope Modulation");
    editDetails()->page()->addComboBox(1, 0, 1, 2, "10", hex1, "19", "04"); //env mod sw
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, "19", "05", "normal_ratio1.25");  //env mod sens
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, "19", "06", "normal_ratio1.25");  //env mod attack
    editDetails()->page()->addGroupBox(0, 2, 1, 1);
    editDetails()->page()->newGroupBox("Pitch");
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, "19", "07");  //pitch sw
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, "19", "08", "normal_ratio1.5");  //pitch A
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, "19", "09", "normal_ratio1.25");  //pitch A fine
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, "19", "0A", "normal_ratio1.5");  //pitch B
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, "19", "0B", "normal_ratio1.25");  //pitch B fine
    editDetails()->page()->addSwitch(1, 4, 1, 1, "10", hex1, "19", "0C", "middle"); //duet sw
    editDetails()->page()->addSwitch(1, 0, 1, 1, "10", hex1, "19", "0D", "middle"); //sweep sw
    editDetails()->page()->addKnob(1, 1, 2, 1, "10", hex1, "19", "0E", "normal_ratio1.25"); //sweep rise
    editDetails()->page()->addKnob(1, 3, 2, 1, "10", hex1, "19", "0F", "normal_ratio1.25"); //sweep fall
    editDetails()->page()->addSystemOverride(0, 0, 3, 5, "10", hex1, "19", "00", "02"); // mode
    editDetails()->page()->addGroupBox(0, 3, 2, 2);
    editDetails()->page()->newGroupBox("Vibrato");
    editDetails()->page()->addSwitch(1, 0, 1, 2, "10", hex1, "19", "10", "middle"); //vibrato sw
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, "19", "11", "normal_ratio1.25");  //vibrato rate
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, "19", "12", "normal_ratio1.25");  //vibrato depth
    editDetails()->page()->addSystemOverride(0, 0, 2, 2, "10", hex1, "19", "00", "02", "equal"); // mode
    editDetails()->page()->addGroupBox(1, 2, 1, 1);
    editDetails()->page()->newGroupBox("Filter");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, "19", "13", "normal_ratio1.25", "bottom", 70);  //low cut
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, "19", "14", "normal_ratio1.25", "bottom", 70);  //high cut
    editDetails()->page()->addGroupBox(0, 5, 2, 1);
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(3);
    stringLevels();
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(3);
    altTuning();
    editDetails()->page()->addStackField();


    // *********************************************************************************************************** E GUITAR /E BASS ********************
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
        editDetails()->page()->addComboBox(0, 0, 1, 2, "10", "02", "1A", "00", "large");         // type
        editDetails()->page()->addStackControl();
        editDetails()->page()->insertStackField(5, 2, 0, 1, 1);
        editDetails()->page()->addGroupBox(0, 0, 1, 9);
        editDetails()->page()->newGroupBox("Noise Suppressor");
        editDetails()->page()->addSwitch(0, 0, 1, 1, "10", "02", "1A", "2A");  // ns off/on
        editDetails()->page()->addKnob(1, 0, 1, 1, "10", "02", "1A", "2B", "normal_ratio1.25");  // threshold
        editDetails()->page()->addKnob(2, 0, 1, 1, "10", "02", "1A", "2C", "normal_ratio1.25");  // release
        editDetails()->page()->addGroupBox(0, 9, 1, 1);

        editDetails()->page()->addStackField();

        editDetails()->page()->newStackField(5);
        editDetails()->page()->addKnob(0, 0, 1, 1, "10", "02", "1A", "01", "normal_ratio2");     // master vint J
        editDetails()->page()->addKnob(0, 1, 1, 1, "10", "02", "1A", "02", "normal_ratio1.75");  // rear vol
        editDetails()->page()->addKnob(0, 2, 1, 1, "10", "02", "1A", "03", "normal_ratio1.75");  // front vol
        editDetails()->page()->addKnob(0, 3, 1, 1, "10", "02", "1A", "15", "normal_ratio1.75");  // tone
        editDetails()->page()->addStackField();
        editDetails()->page()->newStackField(5);
        editDetails()->page()->addKnob(0, 0, 1, 1, "10", "02", "1A", "01", "normal_ratio2");     // master modern J
        editDetails()->page()->addKnob(0, 1, 1, 1, "10", "02", "1A", "02", "normal_ratio1.75");  // rear vol
        editDetails()->page()->addKnob(0, 2, 1, 1, "10", "02", "1A", "03", "normal_ratio1.75");  // front vol
        editDetails()->page()->addKnob(0, 3, 1, 1, "10", "02", "1A", "15", "normal_ratio1.75");  // tone
        editDetails()->page()->addStackField();
        editDetails()->page()->newStackField(5);
        editDetails()->page()->addKnob(0, 0, 1, 1, "10", "02", "1A", "14", "normal_ratio2");     // volume vint P
        editDetails()->page()->addKnob(0, 1, 1, 1, "10", "02", "1A", "15", "normal_ratio1.75");  // tone
        editDetails()->page()->addStackField();
        editDetails()->page()->newStackField(5);
        editDetails()->page()->addKnob(0, 0, 1, 1, "10", "02", "1A", "14", "normal_ratio2");     // volume modern P
        editDetails()->page()->addKnob(0, 1, 1, 1, "10", "02", "1A", "15", "normal_ratio1.75");  // tone
        editDetails()->page()->addStackField();
        editDetails()->page()->newStackField(5);
        editDetails()->page()->addKnob(0, 0, 1, 1, "10", "02", "1A", "14", "normal_ratio2");     // volume m man
        editDetails()->page()->addKnob(0, 1, 1, 1, "10", "02", "1A", "07", "normal_ratio1.75");  // mman bass
        editDetails()->page()->addKnob(0, 2, 1, 1, "10", "02", "1A", "06", "normal_ratio1.75");  // mman trebl
        editDetails()->page()->addStackField();
        editDetails()->page()->newStackField(5);
        editDetails()->page()->addKnob(0, 0, 1, 1, "10", "02", "1A", "01", "normal_ratio2");     // master rickenbacker
        editDetails()->page()->addKnob(0, 1, 1, 1, "10", "02", "1A", "02", "normal_ratio1.75");  // rear vol
        editDetails()->page()->addKnob(0, 2, 1, 1, "10", "02", "1A", "03", "normal_ratio1.75");  // front vol
        editDetails()->page()->addComboBox(0, 3, 1, 1, "10", "02", "1A", "0A", "large");         // pu select
        editDetails()->page()->addKnob(0, 4, 1, 1, "10", "02", "1A", "04", "normal_ratio1.75");  // rick rear tone
        editDetails()->page()->addKnob(0, 5, 1, 1, "10", "02", "1A", "05", "normal_ratio1.75");  // rick front tone
        editDetails()->page()->addStackField();
        editDetails()->page()->newStackField(5);
        editDetails()->page()->addKnob(0, 0, 1, 1, "10", "02", "1A", "01", "normal_ratio2");     // master t-bird
        editDetails()->page()->addKnob(0, 1, 1, 1, "10", "02", "1A", "02", "normal_ratio1.75");  // rear vol
        editDetails()->page()->addKnob(0, 2, 1, 1, "10", "02", "1A", "03", "normal_ratio1.75");  // front vol
        editDetails()->page()->addKnob(0, 3, 1, 1, "10", "02", "1A", "15", "normal_ratio1.75");  // tone
        editDetails()->page()->addStackField();
        editDetails()->page()->newStackField(5);
        editDetails()->page()->addKnob(0, 0, 1, 1, "10", "02", "1A", "01", "normal_ratio2");     // master active
        editDetails()->page()->addKnob(0, 1, 1, 1, "10", "02", "1A", "02", "normal_ratio1.75");  // rear vol
        editDetails()->page()->addKnob(0, 2, 1, 1, "10", "02", "1A", "03", "normal_ratio1.75");  // front vol
        editDetails()->page()->addKnob(0, 3, 1, 1, "10", "02", "1A", "09", "normal_ratio1.75");   // active bass
        editDetails()->page()->addKnob(0, 4, 1, 1, "10", "02", "1A", "08", "normal_ratio1.75");   // active treble
        editDetails()->page()->addStackField();
        editDetails()->page()->newStackField(5);
        editDetails()->page()->addKnob(0, 0, 1, 1, "10", "02", "1A", "01", "normal_ratio2");     // master violin
        editDetails()->page()->addKnob(0, 1, 1, 1, "10", "02", "1A", "02", "normal_ratio1.75");  // rear vol
        editDetails()->page()->addKnob(0, 2, 1, 1, "10", "02", "1A", "03", "normal_ratio1.75");  // front vol
        editDetails()->page()->addComboBox(0, 3, 1, 1, "10", "02", "1A", "0D", "large");         //solo
        editDetails()->page()->addSwitch(0, 4, 1, 1, "10", "02", "1A", "0C", "middle");          // bass on
        editDetails()->page()->addSwitch(0, 5, 1, 1, "10", "02", "1A", "0B", "middle");          // treble on
        editDetails()->page()->addStackField();
        editDetails()->page()->newStackField(5);
        editDetails()->page()->addComboBox(0, 0, 1, 1, "10", "02", "1A", "0E", "large");         // tone type fretless
        editDetails()->page()->addKnob(0, 1, 1, 1, "10", "02", "1A", "14", "normal_ratio2");     // volume
        editDetails()->page()->addKnob(0, 2, 1, 1, "10", "02", "1A", "15", "normal_ratio1.75");  // tone
        editDetails()->page()->addKnob(0, 3, 1, 1, "10", "02", "1A", "0F", "normal_ratio1.5");   // sense
        editDetails()->page()->addKnob(0, 4, 1, 1, "10", "02", "1A", "10", "normal_ratio1.5");   // depth
        editDetails()->page()->addKnob(0, 5, 1, 1, "10", "02", "1A", "11", "normal_ratio1.5");   // attack
        editDetails()->page()->addKnob(0, 6, 1, 1, "10", "02", "1A", "12", "normal_ratio1.5");   // resonance
        editDetails()->page()->addKnob(0, 7, 1, 1, "10", "02", "1A", "13", "normal_ratio1.5");   // direct level
        editDetails()->page()->addStackField();
    }
    else
    {
        editDetails()->page()->newGroupBox("Electric Guitar");
        editDetails()->page()->newStackControl(5);
        editDetails()->page()->addComboBox(0, 1, 1, 7, "10", "00", "1A", "00", "large"); // type
        editDetails()->page()->addStackControl();
        editDetails()->page()->insertStackField(5, 0, 8, 1, 1);
        editDetails()->page()->addComboBox(1, 1, 1, 1, "10", "00", "1A", "01", "pu_sel_gmode_g");// pu select
        editDetails()->page()->addSystemOverride(1, 1, 1, 1, "10", "00", "1A", "00", "0B");
        editDetails()->page()->addComboBox(1, 2, 1, 1, "10", "00", "1A", "02");                  // tone type
        editDetails()->page()->addKnob(1, 3, 1, 1, "10", "00", "1A", "03", "normal_ratio1.25");  // sense
        editDetails()->page()->addKnob(1, 4, 1, 1, "10", "00", "1A", "04", "normal_ratio1.25");  // depth
        editDetails()->page()->addKnob(1, 5, 1, 1, "10", "00", "1A", "05", "normal_ratio1.25");  // attack
        editDetails()->page()->addKnob(1, 6, 1, 1, "10", "00", "1A", "06", "normal_ratio1.25");  // resonance
        editDetails()->page()->addKnob(1, 7, 1, 1, "10", "00", "1A", "07", "normal_ratio1.25");  // direct
        editDetails()->page()->addSystemOverride(1, 2, 1, 6, "10", "00", "1A", "00", "0B", "not_equal");
        editDetails()->page()->addKnob(0, 6, 1, 1, "10", "00", "1A", "08", "normal_ratio1.75");  // volume
        editDetails()->page()->addKnob(0, 7, 1, 1, "10", "00", "1A", "09", "normal_ratio1.5");   // tone
        editDetails()->page()->addGroupBox(0, 0, 1, 7);
        editDetails()->page()->newGroupBox("Noise Suppressor");
        editDetails()->page()->addSwitch(0, 0, 1, 1, "10", "00", "1A", "1E");  // ns off/on
        editDetails()->page()->addKnob(0, 1, 1, 1, "10", "00", "1A", "1F", "normal_ratio1.25");  // threshold
        editDetails()->page()->addKnob(0, 2, 1, 1, "10", "00", "1A", "20", "normal_ratio1.25");  // release
        editDetails()->page()->addGroupBox(0, 7, 1, 3);

        editDetails()->page()->addStackField();

        editDetails()->page()->newStackField(5);
        editDetails()->page()->addStackField();
    };



    editDetails()->page()->newStackField(4);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        editDetails()->page()->newGroupBox("Amp");
        editDetails()->page()->addSwitch(0, 0, 1, 1, "10", "02", "1A", "16");                                 // off/on effect
        editDetails()->page()->addGroupBox(0, 0, 2, 1);
        editDetails()->page()->newGroupBox(tr("PreAmp"));
        editDetails()->page()->addComboBox(0, 1, 1, 5, "10", "02", "1A", "17", "large");                      //pre type
        editDetails()->page()->addComboBox(1, 1, 1, 1, "10", "02", "1A", "21", "bottom");                     //gain sw
        editDetails()->page()->addKnob(1, 2, 1, 1, "10", "02", "1A", "18", "turbo_ratio1.25");                // gain
        editDetails()->page()->addKnob(1, 3, 1, 1, "10", "02", "1A", "19", "normal_ratio1.25");               //sag
        editDetails()->page()->addKnob(1, 4, 1, 1, "10", "02", "1A", "1A", "normal_ratio1.25");               //resonance
        editDetails()->page()->addKnob(1, 5, 1, 1, "10", "02", "1A", "1C", "normal_ratio1.25");               //bass
        editDetails()->page()->addKnob(1, 6, 1, 1, "10", "02", "1A", "1D", "normal_ratio1.25");               // mid
        editDetails()->page()->addKnob(1, 7, 1, 1, "10", "02", "1A", "1E", "normal_ratio1.25");               // treble
        editDetails()->page()->addKnob(1, 8, 1, 1, "10", "02", "1A", "1F", "normal_ratio1.25");               // presence
        editDetails()->page()->addSwitch(1, 9, 1, 1, "10", "02", "1A", "20");                                 // bright button
        editDetails()->page()->addKnob(0, 10, 2, 1, "10", "02", "1A", "1B", "normal_ratio1.75");              // effect level
        editDetails()->page()->addGroupBox(0, 1, 1, 2);
        editDetails()->page()->newGroupBox(tr("Speaker"));
        editDetails()->page()->addComboBox(0, 0, 1, 1, "10", "02", "1A", "24", "large");                      //spkr type
        editDetails()->page()->addComboBox(0, 1, 1, 1, "10", "02", "1A", "25", "bottom");                     //mic type
        editDetails()->page()->addComboBox(0, 2, 1, 1, "10", "02", "1A", "26");                               //mic distance
        editDetails()->page()->addKnob(0, 3, 1, 1, "10", "02", "1A", "27", "normal_ratio1.25", "bottom", 70); //mic position
        editDetails()->page()->addKnob(0, 4, 1, 1, "10", "02", "1A", "28", "normal_ratio1.5");                // mic level
        editDetails()->page()->addKnob(0, 5, 1, 1, "10", "02", "1A", "29", "normal_ratio1.25");               // direct level
        editDetails()->page()->addGroupBox(1, 1, 1, 1);
        editDetails()->page()->newGroupBox(tr("Solo"));
        editDetails()->page()->addSwitch(0, 0, 1, 1, "10", "02", "1A", "22", "middle");                       //solo sw
        editDetails()->page()->addKnob(0, 1, 1, 1, "10", "02", "1A", "23", "normal_ratio1.25");               // solo level
        editDetails()->page()->addGroupBox(1, 2, 1, 1);
    }
    else
    {
        editDetails()->page()->newGroupBox("Amp");
        editDetails()->page()->addSwitch(0, 0, 1, 1, "10", "00", "1A", "0A");   // off/on effect
        editDetails()->page()->addGroupBox(0, 0, 2, 1);
        editDetails()->page()->newGroupBox(tr("PreAmp"));
        editDetails()->page()->addComboBox(0, 1, 1, 5, "10", "00", "1A", "0B", "large");                      // pre type
        editDetails()->page()->addComboBox(1, 1, 1, 1, "10", "00", "1A", "15", "bottom");                     // gain sw
        editDetails()->page()->addKnob(1, 2, 1, 1, "10", "00", "1A", "0C", "turbo_ratio1.25");                // gain
        editDetails()->page()->addKnob(1, 3, 1, 1, "10", "00", "1A", "0D", "normal_ratio1.25");               // sag
        editDetails()->page()->addKnob(1, 4, 1, 1, "10", "00", "1A", "0E", "normal_ratio1.25");               // resonance
        editDetails()->page()->addKnob(1, 5, 1, 1, "10", "00", "1A", "10", "normal_ratio1.25");               // bass
        editDetails()->page()->addKnob(1, 6, 1, 1, "10", "00", "1A", "11", "normal_ratio1.25");               // mid
        editDetails()->page()->addKnob(1, 7, 1, 1, "10", "00", "1A", "12", "normal_ratio1.25");               // treble
        editDetails()->page()->addKnob(1, 8, 1, 1, "10", "00", "1A", "13", "normal_ratio1.25");               // presence
        editDetails()->page()->addSwitch(1, 9, 1, 1, "10", "00", "1A", "14");                                 // bright button
        editDetails()->page()->addKnob(0, 10, 2, 1, "10", "00", "1A", "0F", "normal_ratio1.75");              // effect level
        editDetails()->page()->addGroupBox(0, 1, 1, 2);
        editDetails()->page()->newGroupBox(tr("Speaker"));
        editDetails()->page()->addComboBox(0, 0, 1, 1, "10", "00", "1A", "18", "large");                      //spkr type
        editDetails()->page()->addComboBox(0, 1, 1, 1, "10", "00", "1A", "19", "bottom");                     //mic type
        editDetails()->page()->addComboBox(0, 2, 1, 1, "10", "00", "1A", "1A");                               //mic distance
        editDetails()->page()->addKnob(0, 3, 1, 1, "10", "00", "1A", "1B", "normal_ratio1.25", "bottom", 70); // mic position
        editDetails()->page()->addKnob(0, 4, 1, 1, "10", "00", "1A", "1C", "normal_ratio1.5");                // mic level
        editDetails()->page()->addKnob(0, 5, 1, 1, "10", "00", "1A", "1D", "normal_ratio1.25");               // direct level
        editDetails()->page()->addGroupBox(1, 1, 1, 1);
        editDetails()->page()->newGroupBox(tr("Solo"));
        editDetails()->page()->addSwitch(0, 0, 1, 1, "10", "00", "1A", "16", "middle");  //solo sw
        editDetails()->page()->addKnob(0, 1, 1, 1, "10", "00", "1A", "17", "normal_ratio1.25");               // solo level
        editDetails()->page()->addGroupBox(1, 2, 1, 1);
    };
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(4);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        editDetails()->page()->newGroupBox("Equalizer", Qt::AlignTop | Qt::AlignHCenter);
        editDetails()->page()->addSwitch(0, 0, 1, 1, "10", "02", "1A", "2D");
        editDetails()->page()->addParaEQ(0, 1, 2, 4, "10", "02", "1A", "2E", "inst");
        editDetails()->page()->addGroupBox(0, 0, 1, 1);
    }
    else
    {
        editDetails()->page()->newGroupBox("Equalizer", Qt::AlignTop | Qt::AlignHCenter);
        editDetails()->page()->addSwitch(0, 0, 1, 1, "10", "00", "1A", "21");
        editDetails()->page()->addParaEQ(0, 1, 2, 4, "10", "00", "1A", "22", "inst");
        editDetails()->page()->addGroupBox(0, 0, 1, 1);
    };
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(4);
    stringLevels();
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(4);
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
        editDetails()->page()->addKnob(0, 0, 1, 1, "10", "02", "1B", "00", "normal_ratio1.75");  // volume
        editDetails()->page()->addKnob(0, 1, 1, 1, "10", "02", "1B", "01", "normal_ratio1.25");  // body
        editDetails()->page()->addKnob(0, 2, 1, 1, "10", "02", "1B", "02", "normal_ratio1.25");  // resonance
        editDetails()->page()->addKnob(0, 3, 1, 1, "10", "02", "1B", "03", "normal_ratio1.25");  // size
        editDetails()->page()->addKnob(0, 4, 1, 1, "10", "02", "1B", "04", "normal_ratio1.25");  // attack
        editDetails()->page()->addKnob(0, 5, 1, 1, "10", "02", "1B", "05", "normal_ratio1.25");  // bottom
        editDetails()->page()->addKnob(0, 6, 1, 1, "10", "02", "1B", "06", "normal_ratio1.25");  // buzz sense
        editDetails()->page()->addKnob(0, 7, 1, 1, "10", "02", "1B", "07", "normal_ratio1.25");  // decay
        editDetails()->page()->addGroupBox(0, 0, 1, 8);
        editDetails()->page()->newGroupBox("Noise Suppressor");
        editDetails()->page()->addSwitch(0, 0, 1, 1, "10", "02", "1B", "1C");                    // ns off/on
        editDetails()->page()->addKnob(0, 1, 1, 1, "10", "02", "1B", "1D", "normal_ratio1.25");  // threshold
        editDetails()->page()->addKnob(0, 2, 1, 1, "10", "02", "1B", "1E", "normal_ratio1.25");  // release
        editDetails()->page()->addGroupBox(0, 8, 1, 3);
    }
    else
    {
        editDetails()->page()->newGroupBox("Acoustic Guitar");
        editDetails()->page()->addComboBox(0, 1, 1, 7, "10", "00", "1B", "00", "large");         // type
        editDetails()->page()->addKnob(1, 1, 1, 1, "10", "00", "1B", "01", "normal_ratio1.25");  // body
        editDetails()->page()->addSystemOverride(1, 1, 1, 1, "10", "00", "1B", "00", "06");
        editDetails()->page()->addSystemOverride(1, 1, 1, 1, "10", "00", "1B", "00", "07");
        editDetails()->page()->addKnob(1, 2, 1, 1, "10", "00", "1B", "08", "normal_ratio1.25");  // resonance
        editDetails()->page()->addSystemOverride(1, 2, 1, 1, "10", "00", "1B", "00", "00");
        editDetails()->page()->addSystemOverride(1, 2, 1, 1, "10", "00", "1B", "00", "01");
        editDetails()->page()->addSystemOverride(1, 2, 1, 1, "10", "00", "1B", "00", "02");
        editDetails()->page()->addSystemOverride(1, 2, 1, 1, "10", "00", "1B", "00", "03");
        editDetails()->page()->addSystemOverride(1, 2, 1, 1, "10", "00", "1B", "00", "04");
        editDetails()->page()->addSystemOverride(1, 2, 1, 1, "10", "00", "1B", "00", "05");
        editDetails()->page()->addSystemOverride(1, 2, 1, 1, "10", "00", "1B", "00", "08");
        editDetails()->page()->addKnob(1, 3, 1, 1, "10", "00", "1B", "02", "normal_ratio1.25");  // attack
        editDetails()->page()->addSystemOverride(1, 3, 1, 1, "10", "00", "1B", "00", "00");
        editDetails()->page()->addSystemOverride(1, 3, 1, 1, "10", "00", "1B", "00", "01");
        editDetails()->page()->addSystemOverride(1, 3, 1, 1, "10", "00", "1B", "00", "02");
        editDetails()->page()->addSystemOverride(1, 3, 1, 1, "10", "00", "1B", "00", "03");
        editDetails()->page()->addSystemOverride(1, 3, 1, 1, "10", "00", "1B", "00", "04");
        editDetails()->page()->addSystemOverride(1, 3, 1, 1, "10", "00", "1B", "00", "06");
        editDetails()->page()->addKnob(1, 4, 1, 1, "10", "00", "1B", "09", "normal_ratio1.25");  // sustain
        editDetails()->page()->addSystemOverride(1, 4, 1, 1, "10", "00", "1B", "00", "06", "not_equal");
        editDetails()->page()->addKnob(1, 5, 1, 1, "10", "00", "1B", "04", "normal_ratio1.25");  // sense
        editDetails()->page()->addKnob(1, 6, 1, 1, "10", "00", "1B", "05", "normal_ratio1.25");  // colour
        editDetails()->page()->addKnob(1, 7, 1, 1, "10", "00", "1B", "06", "normal_ratio1.25");  // decay
        editDetails()->page()->addKnob(1, 8, 1, 1, "10", "00", "1B", "07", "normal_ratio1.25");  // buzz
        editDetails()->page()->addComboBox(1, 9, 1, 1, "10", "00", "1B", "03");                  // pu select
        editDetails()->page()->addSystemOverride(1, 5, 1, 5, "10", "00", "1B", "00", "08", "not_equal");
        editDetails()->page()->addKnob(0, 8, 1, 1, "10", "00", "1B", "0A", "normal_ratio1.75");  // volume
        editDetails()->page()->addKnob(0, 9, 1, 1, "10", "00", "1B", "0B", "normal_ratio1.5");   // tone
        editDetails()->page()->addGroupBox(0, 0, 1, 1);
        editDetails()->page()->newGroupBox("Noise Suppressor");
        editDetails()->page()->addSwitch(0, 0, 1, 1, "10", "00", "1B", "20");                    // ns off/on
        editDetails()->page()->addKnob(0, 1, 1, 1, "10", "00", "1B", "21", "normal_ratio1.25");  // threshold
        editDetails()->page()->addKnob(0, 2, 1, 1, "10", "00", "1B", "22", "normal_ratio1.25");  // release
        editDetails()->page()->addGroupBox(0, 1, 1, 1);
    };
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(6);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        editDetails()->page()->newGroupBox("Amp");
        editDetails()->page()->addSwitch(0, 0, 1, 1, "10", "02", "1B", "08");                                 // off/on effect
        editDetails()->page()->addGroupBox(0, 0, 2, 1);
        editDetails()->page()->newGroupBox(tr("PreAmp"));
        editDetails()->page()->addComboBox(0, 1, 1, 5, "10", "02", "1B", "09", "large");                      //pre type
        editDetails()->page()->addComboBox(1, 1, 1, 1, "10", "02", "1B", "13", "bottom");                     //gain sw
        editDetails()->page()->addKnob(1, 2, 1, 1, "10", "02", "1B", "0A", "turbo_ratio1.25");                // gain
        editDetails()->page()->addKnob(1, 3, 1, 1, "10", "02", "1B", "0B", "normal_ratio1.25");               //sag
        editDetails()->page()->addKnob(1, 4, 1, 1, "10", "02", "1B", "0C", "normal_ratio1.25");               //resonance
        editDetails()->page()->addKnob(1, 5, 1, 1, "10", "02", "1B", "0E", "normal_ratio1.25");               //bass
        editDetails()->page()->addKnob(1, 6, 1, 1, "10", "02", "1B", "0F", "normal_ratio1.25");               // mid
        editDetails()->page()->addKnob(1, 7, 1, 1, "10", "02", "1B", "10", "normal_ratio1.25");               // treble
        editDetails()->page()->addKnob(1, 8, 1, 1, "10", "02", "1B", "11", "normal_ratio1.25");               // presence
        editDetails()->page()->addSwitch(1, 9, 1, 1, "10", "02", "1B", "12");                                 // bright button
        editDetails()->page()->addKnob(0, 10, 2, 1, "10", "02", "1B", "0D", "normal_ratio1.75");              // effect level
        editDetails()->page()->addGroupBox(0, 1, 1, 2);
        editDetails()->page()->newGroupBox(tr("Speaker"));
        editDetails()->page()->addComboBox(0, 0, 1, 1, "10", "02", "1B", "16", "large");                       //spkr type
        editDetails()->page()->addComboBox(0, 1, 1, 1, "10", "02", "1B", "17", "bottom");                      //mic type
        editDetails()->page()->addComboBox(0, 2, 1, 1, "10", "02", "1B", "18");                                //mic distance
        editDetails()->page()->addKnob(0, 3, 1, 1, "10", "02", "1B", "19", "normal_ratio1.25", "bottom", 70);  //mic position
        editDetails()->page()->addKnob(0, 4, 1, 1, "10", "02", "1B", "1A", "normal_ratio1.5");                 // mic level
        editDetails()->page()->addKnob(0, 5, 1, 1, "10", "02", "1B", "1B", "normal_ratio1.25");                // direct level
        editDetails()->page()->addGroupBox(1, 1, 1, 1);
        editDetails()->page()->newGroupBox(tr("Solo"));
        editDetails()->page()->addSwitch(0, 0, 1, 1, "10", "02", "1B", "14", "middle");                        //solo sw
        editDetails()->page()->addKnob(0, 1, 1, 1, "10", "02", "1B", "15", "normal_ratio1.25");                // solo level
        editDetails()->page()->addGroupBox(1, 2, 1, 1);
    }
    else
    {
        editDetails()->page()->newGroupBox("Amp");
        editDetails()->page()->addSwitch(0, 0, 1, 1, "10", "00", "1B", "0C");                                 // off/on effect
        editDetails()->page()->addGroupBox(0, 0, 2, 1);
        editDetails()->page()->newGroupBox(tr("PreAmp"));
        editDetails()->page()->addComboBox(0, 1, 1, 5, "10", "00", "1B", "0D", "large");                      //pre type
        editDetails()->page()->addComboBox(1, 1, 1, 1, "10", "00", "1B", "17", "bottom"); //gain sw
        editDetails()->page()->addKnob(1, 2, 1, 1, "10", "00", "1B", "0E", "turbo_ratio1.25");                // gain
        editDetails()->page()->addKnob(1, 3, 1, 1, "10", "00", "1B", "0F", "normal_ratio1.25");               //sag
        editDetails()->page()->addKnob(1, 4, 1, 1, "10", "00", "1B", "10", "normal_ratio1.25");               //resonance
        editDetails()->page()->addKnob(1, 5, 1, 1, "10", "00", "1B", "12", "normal_ratio1.25");               //bass
        editDetails()->page()->addKnob(1, 6, 1, 1, "10", "00", "1B", "13", "normal_ratio1.25");               // mid
        editDetails()->page()->addKnob(1, 7, 1, 1, "10", "00", "1B", "14", "normal_ratio1.25");               // treble
        editDetails()->page()->addKnob(1, 8, 1, 1, "10", "00", "1B", "15", "normal_ratio1.25");               // presence
        editDetails()->page()->addSwitch(1, 9, 1, 1, "10", "00", "1B", "16");                                 // bright button
        editDetails()->page()->addKnob(0, 10, 2, 1, "10", "00", "1B", "11", "normal_ratio1.75");              // effect level
        editDetails()->page()->addGroupBox(0, 1, 1, 2);
        editDetails()->page()->newGroupBox(tr("Speaker"));
        editDetails()->page()->addComboBox(0, 0, 1, 1, "10", "00", "1B", "1A", "large");  //spkr type
        editDetails()->page()->addComboBox(0, 1, 1, 1, "10", "00", "1B", "1B", "bottom"); //mic type
        editDetails()->page()->addComboBox(0, 2, 1, 1, "10", "00", "1B", "1C");           //mic distance
        editDetails()->page()->addKnob(0, 3, 1, 1, "10", "00", "1B", "1D", "normal_ratio1.25", "bottom", 70); //mic position
        editDetails()->page()->addKnob(0, 4, 1, 1, "10", "00", "1B", "1E", "normal_ratio1.5");                // mic level
        editDetails()->page()->addKnob(0, 5, 1, 1, "10", "00", "1B", "1F", "normal_ratio1.25");               // direct level
        editDetails()->page()->addGroupBox(1, 1, 1, 1);
        editDetails()->page()->newGroupBox(tr("Solo"));
        editDetails()->page()->addSwitch(0, 0, 1, 1, "10", "00", "1B", "18", "middle");                       //solo sw
        editDetails()->page()->addKnob(0, 1, 1, 1, "10", "00", "1B", "19", "normal_ratio1.25");               // solo level
        editDetails()->page()->addGroupBox(1, 2, 1, 1);
    }
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(6);
    editDetails()->page()->newGroupBox("Equalizer", Qt::AlignTop | Qt::AlignHCenter);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        editDetails()->page()->addSwitch(0, 0, 1, 1, "10", "02", "1B", "1F");
        editDetails()->page()->addParaEQ(0, 1, 2, 4, "10", "02", "1B", "20", "inst");
    }
    else
    {
        editDetails()->page()->addSwitch(0, 0, 1, 1, "10", "00", "1B", "23");
        editDetails()->page()->addParaEQ(0, 1, 2, 4, "10", "00", "1B", "24", "inst");
    };
    editDetails()->page()->addGroupBox(0, 0, 1, 1);
    editDetails()->page()->addStackField();


    editDetails()->page()->newStackField(6);
    stringLevels();
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(6);
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
        editDetails()->page()->addComboBox(0, 1, 1, 5, "10", "02", "1C", "00", "large");         // type
        editDetails()->page()->addComboBox(1, 1, 1, 1, "10", "02", "1C", "01", "pu_sel_bmode_g");// pu select
        editDetails()->page()->addSystemOverride(1, 1, 1, 1, "10", "02", "1C", "00", "0B");
        editDetails()->page()->addComboBox(1, 2, 1, 1, "10", "02", "1C", "02"); // tone type
        editDetails()->page()->addKnob(1, 3, 1, 1, "10", "02", "1C", "03", "normal_ratio1.25");  // sense
        editDetails()->page()->addKnob(1, 4, 1, 1, "10", "02", "1C", "04", "normal_ratio1.25");  // depth
        editDetails()->page()->addKnob(1, 5, 1, 1, "10", "02", "1C", "05", "normal_ratio1.25");  // attack
        editDetails()->page()->addKnob(1, 6, 1, 1, "10", "02", "1C", "06", "normal_ratio1.25");  // resonance
        editDetails()->page()->addKnob(1, 7, 1, 1, "10", "02", "1C", "07", "normal_ratio1.25");  // direct
        editDetails()->page()->addSystemOverride(1, 2, 1, 6, "10", "02", "1C", "00", "02", "not_equal");
        editDetails()->page()->addKnob(0, 6, 1, 1, "10", "02", "1C", "08", "normal_ratio1.75");  // volume
        editDetails()->page()->addKnob(0, 7, 1, 1, "10", "02", "1C", "09", "normal_ratio1.5");   // tone
        editDetails()->page()->addGroupBox(0, 0, 1, 7);
    }
    else
    {
        editDetails()->page()->newGroupBox("Electric Bass");
        editDetails()->page()->addComboBox(0, 1, 1, 5, "10", "00", "1C", "00", "large");         // type
        editDetails()->page()->addKnob(1, 1, 1, 1, "10", "00", "1C", "01", "normal_ratio1.25");  // j front vol
        editDetails()->page()->addKnob(1, 2, 1, 1, "10", "00", "1C", "02", "normal_ratio1.25");  // j rear vol
        editDetails()->page()->addSystemOverride(1, 1, 1, 2, "10", "00", "1C", "00", "01");
        editDetails()->page()->addSystemOverride(1, 1, 1, 2, "10", "00", "1C", "00", "02");
        editDetails()->page()->addKnob(1, 3, 1, 1, "10", "00", "1C", "04", "normal_ratio1.25");  // sense
        editDetails()->page()->addKnob(1, 4, 1, 1, "10", "00", "1C", "05", "normal_ratio1.25");  // depth
        editDetails()->page()->addKnob(1, 5, 1, 1, "10", "00", "1C", "06", "normal_ratio1.25");  // attack
        editDetails()->page()->addKnob(1, 6, 1, 1, "10", "00", "1C", "07", "normal_ratio1.25");  // resonance
        editDetails()->page()->addKnob(1, 7, 1, 1, "10", "00", "1C", "08", "normal_ratio1.25");  // direct
        editDetails()->page()->addComboBox(1, 8, 1, 1, "10", "00", "1C", "03");                  //tone type
        editDetails()->page()->addSystemOverride(1, 3, 1, 6, "10", "00", "1C", "00", "02", "not_equal");
        editDetails()->page()->addKnob(0, 7, 1, 1, "10", "00", "1C", "09", "normal_ratio1.75");  // volume
        editDetails()->page()->addKnob(0, 8, 1, 1, "10", "00", "1C", "0A", "normal_ratio1.5");   // tone
        editDetails()->page()->addGroupBox(0, 0, 1, 8);
    };
    editDetails()->page()->newGroupBox("Noise Suppressor");
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        editDetails()->page()->addSwitch(0, 0, 1, 1, "10", "02", "1C", "1E");                    // ns off/on
        editDetails()->page()->addKnob(0, 1, 1, 1, "10", "02", "1C", "1F", "normal_ratio1.25");  // threshold
        editDetails()->page()->addKnob(0, 2, 1, 1, "10", "02", "1C", "20", "normal_ratio1.25");  // release
    }
    else
    {
        editDetails()->page()->addSwitch(0, 0, 1, 1, "10", "00", "1C", "1F");                    // ns off/on
        editDetails()->page()->addKnob(0, 1, 1, 1, "10", "00", "1C", "20", "normal_ratio1.25");  // threshold
        editDetails()->page()->addKnob(0, 2, 1, 1, "10", "00", "1C", "21", "normal_ratio1.25");  // release
    };
    editDetails()->page()->addGroupBox(0, 8, 1, 3);
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(7);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        editDetails()->page()->newGroupBox("Amp");
        editDetails()->page()->addSwitch(0, 0, 1, 1, "10", "02", "1C", "0A");                                 // off/on effect
        editDetails()->page()->addGroupBox(0, 0, 2, 1);
        editDetails()->page()->newGroupBox(tr("PreAmp"));
        editDetails()->page()->addComboBox(0, 1, 1, 5, "10", "02", "1C", "0B", "large");                      //pre type
        editDetails()->page()->addComboBox(1, 1, 1, 1, "10", "02", "1C", "15", "bottom");                     //gain sw
        editDetails()->page()->addKnob(1, 2, 1, 1, "10", "02", "1C", "0C", "turbo_ratio1.25");                // gain
        editDetails()->page()->addKnob(1, 3, 1, 1, "10", "02", "1C", "0D", "normal_ratio1.25");               //sag
        editDetails()->page()->addKnob(1, 4, 1, 1, "10", "02", "1C", "0E", "normal_ratio1.25");               //resonance
        editDetails()->page()->addKnob(1, 5, 1, 1, "10", "02", "1C", "10", "normal_ratio1.25");               //bass
        editDetails()->page()->addKnob(1, 6, 1, 1, "10", "02", "1C", "11", "normal_ratio1.25");               // mid
        editDetails()->page()->addKnob(1, 7, 1, 1, "10", "02", "1C", "12", "normal_ratio1.25");               // treble
        editDetails()->page()->addKnob(1, 8, 1, 1, "10", "02", "1C", "13", "normal_ratio1.25");               // presence
        editDetails()->page()->addSwitch(1, 9, 1, 1, "10", "02", "1C", "14");                                 // bright button
        editDetails()->page()->addKnob(0, 10, 2, 1, "10", "02", "1C", "0F", "normal_ratio1.75");              // effect level
        editDetails()->page()->addGroupBox(0, 1, 1, 2);
        editDetails()->page()->newGroupBox(tr("Speaker"));
        editDetails()->page()->addComboBox(0, 0, 1, 1, "10", "02", "1C", "18", "large");                      //spkr type
        editDetails()->page()->addComboBox(0, 1, 1, 1, "10", "02", "1C", "19", "bottom");                     //mic type
        editDetails()->page()->addComboBox(0, 2, 1, 1, "10", "02", "1C", "1A");                               //mic distance
        editDetails()->page()->addKnob(0, 3, 1, 1, "10", "02", "1C", "1B", "normal_ratio1.25", "bottom", 70); //mic position
        editDetails()->page()->addKnob(0, 4, 1, 1, "10", "02", "1C", "1C", "normal_ratio1.5");                // mic level
        editDetails()->page()->addKnob(0, 5, 1, 1, "10", "02", "1C", "1D", "normal_ratio1.25");               // direct level
        editDetails()->page()->addGroupBox(1, 1, 1, 1);
        editDetails()->page()->newGroupBox(tr("Solo"));
        editDetails()->page()->addSwitch(0, 0, 1, 1, "10", "02", "1C", "16", "middle");  //solo sw
        editDetails()->page()->addKnob(0, 1, 1, 1, "10", "02", "1C", "17", "normal_ratio1.25");                // solo level
        editDetails()->page()->addGroupBox(1, 2, 1, 1);
    }
    else
    {
        editDetails()->page()->newGroupBox("Amp");
        editDetails()->page()->addSwitch(0, 0, 1, 1, "10", "00", "1C", "0B");                                 // off/on effect
        editDetails()->page()->addGroupBox(0, 0, 2, 1);
        editDetails()->page()->newGroupBox(tr("PreAmp"));
        editDetails()->page()->addComboBox(0, 1, 1, 5, "10", "00", "1C", "0C", "large");                      // pre type
        editDetails()->page()->addComboBox(1, 1, 1, 1, "10", "00", "1C", "16", "bottom");                     // gain sw
        editDetails()->page()->addKnob(1, 2, 1, 1, "10", "00", "1C", "0D", "turbo_ratio1.25");                // gain
        editDetails()->page()->addKnob(1, 3, 1, 1, "10", "00", "1C", "0E", "normal_ratio1.25");               // sag
        editDetails()->page()->addKnob(1, 4, 1, 1, "10", "00", "1C", "0F", "normal_ratio1.25");               // resonance
        editDetails()->page()->addKnob(1, 5, 1, 1, "10", "00", "1C", "11", "normal_ratio1.25");               // bass
        editDetails()->page()->addKnob(1, 6, 1, 1, "10", "00", "1C", "12", "normal_ratio1.25");               // mid
        editDetails()->page()->addKnob(1, 7, 1, 1, "10", "00", "1C", "13", "normal_ratio1.25");               // treble
        editDetails()->page()->addKnob(1, 8, 1, 1, "10", "00", "1C", "14", "normal_ratio1.25");               // presence
        editDetails()->page()->addSwitch(1, 9, 1, 1, "10", "00", "1C", "15");                                 // bright button
        editDetails()->page()->addKnob(0, 10, 2, 1, "10", "00", "1C", "10", "normal_ratio1.75");              // effect level
        editDetails()->page()->addGroupBox(0, 1, 1, 2);
        editDetails()->page()->newGroupBox(tr("Speaker"));
        editDetails()->page()->addComboBox(0, 0, 1, 1, "10", "00", "1C", "19", "large");                      // spkr type
        editDetails()->page()->addComboBox(0, 1, 1, 1, "10", "00", "1C", "1A", "bottom");                     // mic type
        editDetails()->page()->addComboBox(0, 2, 1, 1, "10", "00", "1C", "1B");                               // mic distance
        editDetails()->page()->addKnob(0, 3, 1, 1, "10", "00", "1C", "1C", "normal_ratio1.25", "bottom", 70); // mic position
        editDetails()->page()->addKnob(0, 4, 1, 1, "10", "00", "1C", "1D", "normal_ratio1.5");                // mic level
        editDetails()->page()->addKnob(0, 5, 1, 1, "10", "00", "1C", "1E", "normal_ratio1.25");               // direct level
        editDetails()->page()->addGroupBox(1, 1, 1, 1);
        editDetails()->page()->newGroupBox(tr("Solo"));
        editDetails()->page()->addSwitch(0, 0, 1, 1, "10", "00", "1C", "17", "middle");                       //solo sw
        editDetails()->page()->addKnob(0, 1, 1, 1, "10", "00", "1C", "18", "normal_ratio1.25");               // solo level
        editDetails()->page()->addGroupBox(1, 2, 1, 1);
    };
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(7);
    editDetails()->page()->newGroupBox("Equalizer", Qt::AlignTop | Qt::AlignHCenter);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        editDetails()->page()->addSwitch(0, 0, 1, 1, "10", "02", "1C", "21");
        editDetails()->page()->addParaEQ(0, 1, 2, 4, "10", "02", "1C", "22", "inst");
    }
    else
    {
        editDetails()->page()->addSwitch(0, 0, 1, 1, "10", "00", "1C", "22");
        editDetails()->page()->addParaEQ(0, 1, 2, 4, "10", "00", "1C", "23", "inst");
    };
    editDetails()->page()->addGroupBox(0, 0, 1, 1);
    editDetails()->page()->addStackField();


    editDetails()->page()->newStackField(7);
    stringLevels();
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(7);
    altTuning();
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(7);
    twelveString();
    editDetails()->page()->addStackField();



    // *********************************************************************************************************** VIO GUITAR / POLY FX *******
    editDetails()->page()->newStackField(0);
    editDetails()->page()->newStackControl(8);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        editDetails()->page()->addTabBar(0, 0, 1, 1, "Tables", "00", "00", "18");          //page select Poly FX (bass mode)
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
        editDetails()->page()->addComboBox(0, 0, 1, 2, "10", "02", "1D", "00", "large");        //type
        editDetails()->page()->addKnob(1, 0, 1, 2, "10", "02", "1D", "01", "normal_ratio1.75"); //vol
        editDetails()->page()->addKnob(0, 2, 1, 1, "10", "02", "1D", "02", "normal_ratio1.25"); //gain
        editDetails()->page()->addKnob(0, 3, 1, 1, "10", "02", "1D", "03", "normal_ratio1.25"); //bal
        editDetails()->page()->addSystemOverride(0, 2, 1, 2, "10", "02", "1D", "00", "00", "not_equal");
        editDetails()->page()->addKnob(1, 2, 1, 1, "10", "02", "1D", "04", "normal_ratio1.25"); //color
        editDetails()->page()->addKnob(1, 3, 1, 1, "10", "02", "1D", "05", "normal_ratio1.25"); //tone
        editDetails()->page()->addGroupBox(0, 0, 1, 4);

        editDetails()->page()->newGroupBox("Filter");
        editDetails()->page()->addComboBox(0, 0, 1, 1, "10", "02", "1D", "06");                 //flt mode
        editDetails()->page()->addComboBox(1, 0, 1, 1, "10", "02", "1D", "07");                 //polarity
        editDetails()->page()->addKnob(0, 1, 1, 1, "10", "02", "1D", "08", "normal_ratio1.25"); //sense
        editDetails()->page()->addKnob(0, 2, 1, 1, "10", "02", "1D", "09", "normal_ratio1.25"); //freq
        editDetails()->page()->addKnob(1, 1, 1, 1, "10", "02", "1D", "0A", "normal_ratio1.25"); //decay
        editDetails()->page()->addKnob(1, 2, 1, 1, "10", "02", "1D", "0B", "normal_ratio1.25"); //peak
        editDetails()->page()->addComboBox(0, 3, 2, 1, "10", "02", "1D", "0C");                 //tone type
        editDetails()->page()->addSystemOverride(0, 0, 2, 4, "10", "02", "1D", "00", "04", "not_equal");
        editDetails()->page()->addGroupBox(0, 4, 1, 4);

        editDetails()->page()->newGroupBox("Compressor");
        editDetails()->page()->addSwitch(0, 0, 1, 2, "10", "02", "1D", "0D");                   //comp sw
        editDetails()->page()->addKnob(1, 0, 1, 1, "10", "02", "1D", "0E", "normal_ratio1.25"); //sustain
        editDetails()->page()->addKnob(2, 0, 1, 1, "10", "02", "1D", "0F", "normal_ratio1.25"); //attack
        editDetails()->page()->addSystemOverride(0, 0, 3, 1, "10", "02", "1D", "00", "04", "not_equal");
        editDetails()->page()->addGroupBox(0, 8, 1, 1);

        editDetails()->page()->newGroupBox("Noise Suppressor");
        editDetails()->page()->addSwitch(0, 0, 1, 2, "10", "02", "1D", "10");                   //ns sw
        editDetails()->page()->addKnob(1, 0, 1, 1, "10", "02", "1D", "11", "normal_ratio1.25"); //thresh
        editDetails()->page()->addKnob(2, 0, 1, 1, "10", "02", "1D", "12", "normal_ratio1.25"); //release
        editDetails()->page()->addGroupBox(0, 9, 1, 1);
        editDetails()->page()->addStackField();
    }
    else
    {
        editDetails()->page()->newStackField(8);
        editDetails()->page()->newGroupBox("VIO Guitar");
        editDetails()->page()->addComboBox(0, 1, 1, 5, "10", "00", "1D", "00", "large");         // type
        editDetails()->page()->addComboBox(1, 1, 1, 1, "10", "00", "1D", "01", "pu_sel");        // pu select
        editDetails()->page()->addSystemOverride(1, 1, 1, 1, "10", "00", "1D", "00", "0B");
        editDetails()->page()->addComboBox(1, 2, 1, 1, "10", "00", "1D", "02");                  // tone type
        editDetails()->page()->addKnob(1, 3, 1, 1, "10", "00", "1D", "03", "normal_ratio1.25");  // sense
        editDetails()->page()->addKnob(1, 4, 1, 1, "10", "00", "1D", "04", "normal_ratio1.25");  // depth
        editDetails()->page()->addKnob(1, 5, 1, 1, "10", "00", "1D", "05", "normal_ratio1.25");  // attack
        editDetails()->page()->addKnob(1, 6, 1, 1, "10", "00", "1D", "06", "normal_ratio1.25");  // resonance
        editDetails()->page()->addKnob(1, 7, 1, 1, "10", "00", "1D", "07", "normal_ratio1.25");  // direct
        editDetails()->page()->addSystemOverride(1, 2, 1, 6, "10", "00", "1D", "00", "0B", "not_equal");
        editDetails()->page()->addKnob(0, 6, 1, 1, "10", "00", "1D", "08", "normal_ratio1.75");  // volume
        editDetails()->page()->addKnob(0, 7, 1, 1, "10", "00", "1D", "09", "normal_ratio1.5");   // tone
        editDetails()->page()->addGroupBox(0, 0, 1, 1);
        editDetails()->page()->newGroupBox("Noise Suppressor");
        editDetails()->page()->addSwitch(0, 0, 1, 1, "10", "00", "1D", "16");                    // ns off/on
        editDetails()->page()->addKnob(0, 1, 1, 1, "10", "00", "1D", "17", "normal_ratio1.25");  // threshold
        editDetails()->page()->addKnob(0, 2, 1, 1, "10", "00", "1D", "18", "normal_ratio1.25");  // release
        editDetails()->page()->addGroupBox(0, 1, 1, 1);
        editDetails()->page()->addStackField();

        editDetails()->page()->newStackField(8);
        editDetails()->page()->newGroupBox("Filter");
        editDetails()->page()->addKnob(0, 0, 1, 1, "10", "00", "1D", "0E", "normal_ratio1.5");   // overtone
        editDetails()->page()->addKnob(0, 1, 1, 1, "10", "00", "1D", "0F", "normal_ratio1.5");   // attack
        editDetails()->page()->addKnob(0, 2, 1, 1, "10", "00", "1D", "10", "normal_ratio1.5");   // power bend
        editDetails()->page()->addKnob(0, 3, 1, 1, "10", "00", "1D", "11", "normal_ratio1.5");   // slide time
        editDetails()->page()->addComboBox(0, 4, 1, 1, "10", "00", "1D", "12", "large");         // octave
        editDetails()->page()->addKnob(0, 5, 1, 1, "10", "00", "1D", "13", "normal_ratio1.5");   // colour
        editDetails()->page()->addKnob(0, 6, 1, 1, "10", "00", "1D", "14", "normal_ratio1.5");   // touch sense
        editDetails()->page()->addKnob(0, 7, 1, 1, "10", "00", "1D", "15", "normal_ratio1.5");   // lead emph
        editDetails()->page()->addGroupBox(0, 0, 1, 1);
        editDetails()->page()->newGroupBox("Harmony");
        editDetails()->page()->addKnob(0, 0, 1, 1, "10", "00", "1D", "0A", "normal_ratio1.5");   // pitch
        editDetails()->page()->addKnob(0, 1, 1, 1, "10", "00", "1D", "0B", "normal_ratio1.5");   // gain
        editDetails()->page()->addKnob(0, 2, 1, 1, "10", "00", "1D", "0C", "normal_ratio1.5");   // attack
        editDetails()->page()->addKnob(0, 3, 1, 1, "10", "00", "1D", "0D", "normal_ratio1.5");   // direct
        editDetails()->page()->addGroupBox(1, 0, 1, 1);
        editDetails()->page()->addStackField();

        editDetails()->page()->newStackField(8);
        editDetails()->page()->newGroupBox("Equalizer", Qt::AlignTop | Qt::AlignHCenter);
        editDetails()->page()->addSwitch(0, 0, 1, 1, "10", "00", "1D", "19");
        editDetails()->page()->addParaEQ(0, 1, 2, 4, "10", "00", "1D", "1A", "inst");
        editDetails()->page()->addGroupBox(0, 0, 1, 1);
        editDetails()->page()->addStackField();
    };

    editDetails()->page()->newStackField(8);
    stringLevels();
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(8);
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

        hex1 = "00";
        hex2 = "1E";
        if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
        {
            hex1 = "02";
            hex2 = "1D";
        };
        editDetails()->page()->newStackField(9);
        editDetails()->page()->newGroupBox("Poly FX");
        editDetails()->page()->addComboBox(1, 1, 1, 1, "10", hex1, hex2, "00", "large");        //type
        editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "01", "normal_ratio1.75"); //vol
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
        stringLevels();
        editDetails()->page()->addStackField();

        editDetails()->page()->newStackField(9);
        altTuning();
        editDetails()->page()->addStackField();

    };

    editDetails()->addPage();
};

void soundsource_inst1::stringLevels()
{
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex1 = "02";
    }
    else
    {
        hex1 = "00";
    };
    editDetails()->page()->newGroupBox(str6);
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, "15", "0B", "normal_ratio1.5");  //6
    editDetails()->page()->addKnob(1, 0, 1, 1, "10", hex1, "15", "11", "normal_ratio1.5");  //6
    editDetails()->page()->addGroupBox(0, 0, 1, 1);
    editDetails()->page()->newGroupBox(str5);
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, "15", "0A", "normal_ratio1.5");  //5
    editDetails()->page()->addKnob(1, 0, 1, 1, "10", hex1, "15", "10", "normal_ratio1.5");  //5
    editDetails()->page()->addGroupBox(0, 1, 1, 1);
    editDetails()->page()->newGroupBox(str4);
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, "15", "09", "normal_ratio1.5");  //4
    editDetails()->page()->addKnob(1, 0, 1, 1, "10", hex1, "15", "0F", "normal_ratio1.5");  //4
    editDetails()->page()->addGroupBox(0, 2, 1, 1);
    editDetails()->page()->newGroupBox(str3);
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, "15", "08", "normal_ratio1.5");  //3
    editDetails()->page()->addKnob(1, 0, 1, 1, "10", hex1, "15", "0E", "normal_ratio1.5");  //3
    editDetails()->page()->addGroupBox(0, 3, 1, 1);
    editDetails()->page()->newGroupBox(str2);
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, "15", "07", "normal_ratio1.5");  //2
    editDetails()->page()->addKnob(1, 0, 1, 1, "10", hex1, "15", "0D", "normal_ratio1.5");  //2
    editDetails()->page()->addGroupBox(0, 4, 1, 1);
    editDetails()->page()->newGroupBox(str1);
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, "15", "06", "normal_ratio1.5");  //string 1
    editDetails()->page()->addKnob(1, 0, 1, 1, "10", hex1, "15", "0C", "normal_ratio1.5");  //string 1
    editDetails()->page()->addGroupBox(0, 5, 1, 1);
}

void soundsource_inst1::altTuning()
{
    QString hexA = "00";
    QString hexB = "1F";
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hexA = "02";
        hexB = "1E";
    };
    editDetails()->page()->newGroupBox("Alternate Tuning");
    editDetails()->page()->addSwitch(0, 0, 1, 1, "10", hexA, hexB, "00");                  // off/on
    editDetails()->page()->addKnob(1, 0, 1, 1, "10", hexA, hexB, "01", "normal_ratio1.5", "bottom", 90); // type
    editDetails()->page()->newGroupBox(str6+" Tuning");
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hexA, hexB, "07", "large");       // pitch 6
    editDetails()->page()->addKnob(1, 0, 2, 1, "10", hexA, hexB, "0D");                    // fine 6
    editDetails()->page()->addGroupBox(0, 1, 3, 1);
    editDetails()->page()->newGroupBox(str5+" Tuning");
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hexA, hexB, "06", "large");       // pitch 5
    editDetails()->page()->addKnob(1, 0, 2, 1, "10", hexA, hexB, "0C");                    // fine 5
    editDetails()->page()->addGroupBox(0, 2, 3, 1);
    editDetails()->page()->newGroupBox(str4+" Tuning");
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hexA, hexB, "05", "large");       // pitch 4
    editDetails()->page()->addKnob(1, 0, 2, 1, "10", hexA, hexB, "0B");                    // fine 4
    editDetails()->page()->addGroupBox(0, 3, 3, 1);
    editDetails()->page()->newGroupBox(str3+" Tuning");
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hexA, hexB, "04", "large");       // pitch 3
    editDetails()->page()->addKnob(1, 0, 2, 1, "10", hexA, hexB, "0A");                    // fine 3
    editDetails()->page()->addGroupBox(0, 4, 3, 1);
    editDetails()->page()->newGroupBox(str2+" Tuning");
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hexA, hexB, "03", "large");       // pitch 2
    editDetails()->page()->addKnob(1, 0, 2, 1, "10", hexA, hexB, "09");                    // fine 2
    editDetails()->page()->addGroupBox(0, 5, 3, 1);
    editDetails()->page()->newGroupBox(str1+" Tuning");
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hexA, hexB, "02", "large");       // pitch 1
    editDetails()->page()->addKnob(1, 0, 2, 1, "10", hexA, hexB, "08");                    // fine 1
    editDetails()->page()->addGroupBox(0, 6, 3, 1);
    QString usr = "24";
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        usr = "18";
    };
    editDetails()->page()->addSystemOverride(0, 1, 3, 6, "10", hexA, hexB, "01", usr, "not_equals");

    editDetails()->page()->addSwitch(2, 0, 1, 1, "10", hexA, hexB, "28");                    // off/on
    editDetails()->page()->addKnob(3, 0, 1, 1, "10", hexA, hexB, "2F", "normal_ratio1.25");  // bend control
    editDetails()->page()->newGroupBox(str6+" Bend");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hexA, hexB, "2E", "normal_ratio1.25");  // bend 6
    editDetails()->page()->addGroupBox(2, 1, 2, 1);
    editDetails()->page()->newGroupBox(str5+" Bend");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hexA, hexB, "2D", "normal_ratio1.25");  // bend 5
    editDetails()->page()->addGroupBox(2, 2, 2, 1);
    editDetails()->page()->newGroupBox(str4+" Bend");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hexA, hexB, "2C", "normal_ratio1.25");  // bend 4
    editDetails()->page()->addGroupBox(2, 3, 2, 1);
    editDetails()->page()->newGroupBox(str3+" Bend");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hexA, hexB, "2B", "normal_ratio1.25");  // bend 3
    editDetails()->page()->addGroupBox(2, 4, 2, 1);
    editDetails()->page()->newGroupBox(str2+" Bend");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hexA, hexB, "2A", "normal_ratio1.25");  // bend 2
    editDetails()->page()->addGroupBox(2, 5, 2, 1);
    editDetails()->page()->newGroupBox(str1+" Bend");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hexA, hexB, "29", "normal_ratio1.25");  // bend 1
    editDetails()->page()->addGroupBox(2, 6, 2, 1);
    editDetails()->page()->addGroupBox(0, 0, 1, 1);
}

void soundsource_inst1::twelveString()
{
    QString hexA = "00";
    QString hexB = "1F";
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hexA = "02";
        hexB = "1E";
    };
    editDetails()->page()->newGroupBox("12 String");
    editDetails()->page()->addSwitch(0, 0, 1, 2, "10", hexA, hexB, "0E");                    // off/on
    editDetails()->page()->addComboBox(0, 3, 1, 2, "10", hexA, hexB, "0F");                  // 12 str type
    editDetails()->page()->newGroupBox(str6);
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hexA, hexB, "15", "normal_ratio1.25");  // pitch 6
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hexA, hexB, "21", "normal_ratio1.25");  // level 6
    editDetails()->page()->addKnob(1, 0, 1, 1, "10", hexA, hexB, "1B", "normal_ratio1.25");  // fine 6
    editDetails()->page()->addKnob(1, 1, 1, 1, "10", hexA, hexB, "27", "normal_ratio1.25");  // delay 6
    editDetails()->page()->addGroupBox(1, 0, 2, 2);
    editDetails()->page()->newGroupBox(str5);
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hexA, hexB, "14", "normal_ratio1.25");  // pitch 5
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hexA, hexB, "20", "normal_ratio1.25");  // level 5
    editDetails()->page()->addKnob(1, 0, 1, 1, "10", hexA, hexB, "1A", "normal_ratio1.25");  // fine 5
    editDetails()->page()->addKnob(1, 1, 1, 1, "10", hexA, hexB, "26", "normal_ratio1.25");  // delay 5
    editDetails()->page()->addGroupBox(1, 2, 2, 2);
    editDetails()->page()->newGroupBox(str4);
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hexA, hexB, "13", "normal_ratio1.25");  // pitch 4
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hexA, hexB, "1F", "normal_ratio1.25");  // level 4
    editDetails()->page()->addKnob(1, 0, 1, 1, "10", hexA, hexB, "19", "normal_ratio1.25");  // fine 4
    editDetails()->page()->addKnob(1, 1, 1, 1, "10", hexA, hexB, "25", "normal_ratio1.25");  // delay 4
    editDetails()->page()->addGroupBox(1, 4, 2, 2);
    editDetails()->page()->newGroupBox(str3);
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hexA, hexB, "12", "normal_ratio1.25");  // pitch 3
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hexA, hexB, "1E", "normal_ratio1.25");  // level 3
    editDetails()->page()->addKnob(1, 0, 1, 1, "10", hexA, hexB, "18", "normal_ratio1.25");  // fine 3
    editDetails()->page()->addKnob(1, 1, 1, 1, "10", hexA, hexB, "24", "normal_ratio1.25");  // delay 3
    editDetails()->page()->addGroupBox(1, 6, 2, 2);
    editDetails()->page()->newGroupBox(str2);
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hexA, hexB, "11", "normal_ratio1.25");  // pitch 2
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hexA, hexB, "1D", "normal_ratio1.25");  // level 2
    editDetails()->page()->addKnob(1, 0, 1, 1, "10", hexA, hexB, "17", "normal_ratio1.25");  // fine 2
    editDetails()->page()->addKnob(1, 1, 1, 1, "10", hexA, hexB, "23", "normal_ratio1.25");  // delay 2
    editDetails()->page()->addGroupBox(1, 8, 2, 2);
    editDetails()->page()->newGroupBox(str1);
    editDetails()->page()->addKnob(1, 5, 1, 1, "10", hexA, hexB, "10", "normal_ratio1.25");  // pitch 1
    editDetails()->page()->addKnob(1, 11, 1, 1, "10", hexA, hexB, "1C", "normal_ratio1.25"); // level 1
    editDetails()->page()->addKnob(2, 5, 1, 1, "10", hexA, hexB, "16", "normal_ratio1.25");  // fine 1
    editDetails()->page()->addKnob(2, 11, 1, 1, "10", hexA, hexB, "22", "normal_ratio1.25"); // delay 1
    editDetails()->page()->addGroupBox(1, 10, 2, 2);
    editDetails()->page()->addSystemOverride(1, 0, 2, 12, "10", hexA, hexB, "0F", "00");
    editDetails()->page()->addGroupBox(0, 1, 1, 1);
}
