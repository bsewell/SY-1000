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

#include "stompbox_dd3.h"
#include "Preferences.h"
#include "SysxIO.h"

stompbox_dd3::stompbox_dd3(QWidget *parent)
    : stompBox(parent)
{
    SysxIO *sysxIO = SysxIO::Instance();
    sysxIO->deBug("build Master Delay stomp");

    QString hex1 = "00";
    QString hex2 = "3E";
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){hex1 = "02"; hex2 = "3B";};

    /* DELAY */
    setOnImage(":/images/dd3_on.png");
    setOffImage(":/images/dd3_off.png");
    setHoverImage(":/images/effect_hover.png");
    setSelectedImage(":/images/effect_select.png");
    setLSB(hex1, hex2);
    brushColor("white");
    setButton("10", hex1, hex2, "00");
   /*editDetails()->patchPos(1404, 14, "06", "05"); */
    setEditPages();
}

void stompbox_dd3::updateSignal()
{
    QString hex1 = "00";
    QString hex2 = "3E";
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){hex1 = "02"; hex2 = "3B";};

   updateButton("10", hex1, hex2, "00");
   updateLabel("10", hex1, hex2, "01");
}

void stompbox_dd3::setEditPages()
{
    QString hex1 = "00";
    QString hex2 = "3E";
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){hex1 = "02"; hex2 = "3B";};

    // formerly "Effect" groupBox at addGroupBox(0, 0, 2, 1) — parentCol=0
    editDetails()->page()->addSwitch(0, 0, 1, 1, "10", hex1, hex2, "00");
    editDetails()->page()->newStackControl(0);
    editDetails()->page()->addComboBox(1, 0, 1, 1, "10", hex1, hex2, "01", "large");
    editDetails()->page()->addStackControl();

    editDetails()->page()->insertStackField(0, 0, 1, 2, 1);

    // STEREO 1 — formerly: Delay(0,0→col0-1), Filter(0,1→col2), Level(0,2→col3-5)
    editDetails()->page()->newStackField(0);
    editDetails()->page()->addDataKnob(0, 0, 1, 1, "10", hex1, hex2, "02", "DELAY2000_ratio1.75","bottom", 60); //4 byte time
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.25"); //feedback
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "07", "normal_ratio1.25", "bottom", 60); //high cut
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "08", "turbo_ratio1.75"); //effect
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "09", "normal_ratio1.5"); // direct
    editDetails()->page()->addKnob(0, 5, 1, 1, "10", hex1, hex2, "20"); //carry over
    editDetails()->page()->addStackField();


    // STEREO 2
    editDetails()->page()->newStackField(0);
    editDetails()->page()->addDataKnob(0, 0, 1, 1, "10", hex1, hex2, "02", "DELAY2000_ratio1.75","bottom", 60); //4 byte time
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.25"); //feedback
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "07", "normal_ratio1.25", "bottom", 60); //high cut
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "08", "turbo_ratio1.75"); //effect
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "09", "normal_ratio1.5"); // direct
    editDetails()->page()->addKnob(0, 5, 1, 1, "10", hex1, hex2, "20"); //carry over
    editDetails()->page()->addStackField();

    // PAN
    editDetails()->page()->newStackField(0);
    editDetails()->page()->addDataKnob(0, 0, 1, 1, "10", hex1, hex2, "02", "DELAY2000_ratio1.75","bottom", 60); //4 byte time
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.25"); //feedback
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "1F", "normal_ratio1.25"); //tap time %
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "07", "normal_ratio1.25", "bottom", 60); //high cut
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "08", "turbo_ratio1.75"); //effect
    editDetails()->page()->addKnob(0, 5, 1, 1, "10", hex1, hex2, "09", "normal_ratio1.5"); // direct
    editDetails()->page()->addKnob(0, 6, 1, 1, "10", hex1, hex2, "20"); //carry over
    editDetails()->page()->addStackField();

    // DUAL SERIES — 2 rows: row0=Delay1+Filter1+Level1, row1=Delay2+Filter2+Level2, Direct spans rows
    editDetails()->page()->newStackField(0);
    editDetails()->page()->addDataKnob(0, 0, 1, 1, "10", hex1, hex2, "0A", "DELAY2000_ratio1.75","bottom", 60); //Delay1 time
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "0E", "normal_ratio1.25"); //d1 feedback
    editDetails()->page()->addDataKnob(1, 0, 1, 1, "10", hex1, hex2, "11", "DELAY2000_ratio1.75","bottom", 60); //Delay2 time
    editDetails()->page()->addKnob(1, 1, 1, 1, "10", hex1, hex2, "15", "normal_ratio1.25"); //d2 feedback
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "0F", "normal_ratio1.25", "bottom", 60); //Filter1
    editDetails()->page()->addKnob(1, 2, 1, 1, "10", hex1, hex2, "16", "normal_ratio1.25", "bottom", 60); //Filter2
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "10", "normal_ratio1.75"); //Level1
    editDetails()->page()->addKnob(1, 3, 1, 1, "10", hex1, hex2, "17", "normal_ratio1.75"); //Level2
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "09", "normal_ratio1.5"); //Direct
    editDetails()->page()->addKnob(1, 4, 1, 1, "10", hex1, hex2, "20"); //carry over
    editDetails()->page()->addStackField();

    // DUAL PARALLEL
    editDetails()->page()->newStackField(0);
    editDetails()->page()->addDataKnob(0, 0, 1, 1, "10", hex1, hex2, "0A", "DELAY2000_ratio1.75","bottom", 60); //Delay1 time
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "0E", "normal_ratio1.25"); //d1 feedback
    editDetails()->page()->addDataKnob(1, 0, 1, 1, "10", hex1, hex2, "11", "DELAY2000_ratio1.75","bottom", 60); //Delay2 time
    editDetails()->page()->addKnob(1, 1, 1, 1, "10", hex1, hex2, "15", "normal_ratio1.25"); //d2 feedback
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "0F", "normal_ratio1.25", "bottom", 60); //Filter1
    editDetails()->page()->addKnob(1, 2, 1, 1, "10", hex1, hex2, "16", "normal_ratio1.25", "bottom", 60); //Filter2
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "10", "normal_ratio1.75"); //Level1
    editDetails()->page()->addKnob(1, 3, 1, 1, "10", hex1, hex2, "17", "normal_ratio1.75"); //Level2
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "09", "normal_ratio1.5"); //Direct
    editDetails()->page()->addKnob(1, 4, 1, 1, "10", hex1, hex2, "20"); //carry over
    editDetails()->page()->addStackField();

    // DUAL L/R
    editDetails()->page()->newStackField(0);
    editDetails()->page()->addDataKnob(0, 0, 1, 1, "10", hex1, hex2, "0A", "DELAY2000_ratio1.75","bottom", 60); //Delay1 time
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "0E", "normal_ratio1.25"); //d1 feedback
    editDetails()->page()->addDataKnob(1, 0, 1, 1, "10", hex1, hex2, "11", "DELAY2000_ratio1.75","bottom", 60); //Delay2 time
    editDetails()->page()->addKnob(1, 1, 1, 1, "10", hex1, hex2, "15", "normal_ratio1.25"); //d2 feedback
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "0F", "normal_ratio1.25", "bottom", 60); //Filter1
    editDetails()->page()->addKnob(1, 2, 1, 1, "10", hex1, hex2, "16", "normal_ratio1.25", "bottom", 60); //Filter2
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "10", "normal_ratio1.75"); //Level1
    editDetails()->page()->addKnob(1, 3, 1, 1, "10", hex1, hex2, "17", "normal_ratio1.75"); //Level2
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "09", "normal_ratio1.5"); //Direct
    editDetails()->page()->addKnob(1, 4, 1, 1, "10", hex1, hex2, "20"); //carry over
    editDetails()->page()->addStackField();

    // REVERSE
    editDetails()->page()->newStackField(0);
    editDetails()->page()->addDataKnob(0, 0, 1, 1, "10", hex1, hex2, "02", "DELAY2000_ratio1.75","bottom", 60); //4 byte time
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.25"); //feedback
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "07", "normal_ratio1.25", "bottom", 60); //high cut
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "08", "turbo_ratio1.75"); //effect
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "09", "normal_ratio1.5"); // direct
    editDetails()->page()->addKnob(0, 5, 1, 1, "10", hex1, hex2, "20"); //carry over
    editDetails()->page()->addStackField();

    // ANALOG
    editDetails()->page()->newStackField(0);
    editDetails()->page()->addDataKnob(0, 0, 1, 1, "10", hex1, hex2, "02", "DELAY2000_ratio1.75","bottom", 60); //4 byte time
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.25"); //feedback
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "07", "normal_ratio1.25", "bottom", 60); //high cut
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "08", "turbo_ratio1.75"); //effect
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "09", "normal_ratio1.5"); // direct
    editDetails()->page()->addKnob(0, 5, 1, 1, "10", hex1, hex2, "20"); //carry over
    editDetails()->page()->addStackField();

    // TAPE
    editDetails()->page()->newStackField(0);
    editDetails()->page()->addDataKnob(0, 0, 1, 1, "10", hex1, hex2, "02", "DELAY2000_ratio1.75","bottom", 60); //4 byte time
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.25"); //feedback
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "07", "normal_ratio1.25", "bottom", 60); //high cut
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "08", "turbo_ratio1.75"); //effect
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "09", "normal_ratio1.5"); // direct
    editDetails()->page()->addKnob(0, 5, 1, 1, "10", hex1, hex2, "20"); //carry over
    editDetails()->page()->addStackField();


    // MODULATE — Delay(0-1), Modulate(2-4), Filter(5), Level(6-8)
    editDetails()->page()->newStackField(0);
    editDetails()->page()->addDataKnob(0, 0, 1, 1, "10", hex1, hex2, "02", "DELAY2000_ratio1.75","bottom", 60); //4 byte time
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.25"); //feedback
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "18", "normal_ratio1.25");
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "19", "normal_ratio1.25");
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "1A", "normal_ratio1.25");
    editDetails()->page()->addKnob(0, 5, 1, 1, "10", hex1, hex2, "07", "normal_ratio1.25", "bottom", 60); //high cut
    editDetails()->page()->addKnob(0, 6, 1, 1, "10", hex1, hex2, "08", "turbo_ratio1.75"); //effect
    editDetails()->page()->addKnob(0, 7, 1, 1, "10", hex1, hex2, "09", "normal_ratio1.5"); // direct
    editDetails()->page()->addKnob(0, 8, 1, 1, "10", hex1, hex2, "20"); //carry over
    editDetails()->page()->addStackField();


    // WARP
    editDetails()->page()->newStackField(0);
    editDetails()->page()->addDataKnob(0, 0, 1, 1, "10", hex1, hex2, "02", "DELAY2000_ratio1.75","bottom", 60); //4 byte time
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.25"); //feedback
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "07", "normal_ratio1.25", "bottom", 60); //high cut
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "08", "turbo_ratio1.75"); //effect
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "09", "normal_ratio1.5"); // direct
    editDetails()->page()->addKnob(0, 5, 1, 1, "10", hex1, hex2, "20"); //carry over
    editDetails()->page()->addStackField();

    // TWIST — Delay(0-1), Twist(nested stackControl at cols 2-3), Filter(4), Level(5-7)
    editDetails()->page()->newStackField(0);
    editDetails()->page()->addDataKnob(0, 0, 1, 1, "10", hex1, hex2, "02", "DELAY2000_ratio1.75","bottom", 60); //4 byte time
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.25"); //feedback
    editDetails()->page()->newStackControl(1);
    editDetails()->page()->addComboBox(0, 2, 1, 1, "10", hex1, hex2, "1B", "large");
    editDetails()->page()->addStackControl();
    editDetails()->page()->insertStackField(1, 0, 2, 1, 2);
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "07", "normal_ratio1.25", "bottom", 60); //high cut
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "08", "turbo_ratio1.75"); //effect
    editDetails()->page()->addKnob(0, 5, 1, 1, "10", hex1, hex2, "09", "normal_ratio1.5"); // direct
    editDetails()->page()->addKnob(0, 6, 1, 1, "10", hex1, hex2, "20"); //carry over
    editDetails()->page()->addStackField();


    editDetails()->page()->newStackField(1);
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "1C", "normal_ratio1.5");     //rise
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "1D", "normal_ratio1.5");     //fall
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(1);
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "1C", "normal_ratio1.5");     //rise
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "1E", "normal_ratio1.5");     //fade
    editDetails()->page()->addStackField();

    editDetails()->addPage();
}
