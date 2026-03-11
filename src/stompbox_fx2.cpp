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

#include "stompbox_fx2.h"
#include "Preferences.h"
#include "SysxIO.h"

stompbox_fx2::stompbox_fx2(QWidget *parent)
    : stompBox(parent)
{
    SysxIO *sysxIO = SysxIO::Instance();
    sysxIO->deBug("build FX2 stomp");

    QString hex1 = "00";
    QString hex2 = "66";
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){hex1 = "02"; hex2 = "63";};

    /* MOD */
    setOnImage(":/images/fx2_on.png");
    setOffImage(":/images/fx2_off.png");
    setHoverImage(":/images/effect_hover.png");
    setSelectedImage(":/images/effect_select.png");
    setLSB(hex1, hex2);
    brushColor("magenta");
    setButton("10", hex1, hex2, "00");
  /* editDetails()->patchPos(1522, 138, "07", "15"); */
    setEditPages();
}

void stompbox_fx2::updateSignal()
{
    QString hex1 = "00";
    QString hex2 = "66";
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex1 = "02";
        hex2 = "63";
    };

   updateButton("10", hex1, hex2, "00");
   updateLabel("10", hex1, hex2, "01");
}

void stompbox_fx2::setEditPages()
{
    QString hex1 = "00";
    QString hex1A = "01";
    QString hex2 = "66";
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex1 = "02";
        hex1A = "03"; hex2 = "63";
    };

    // formerly "Select" groupBox at addGroupBox(0, 0, 2, 1) — parentCol=0
    editDetails()->page()->newStackControl(0);
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, hex2, "01", "large");  //type
    editDetails()->page()->addStackControl();
    editDetails()->page()->addSwitch(0, 1, 1, 1, "10", hex1, hex2, "00");
    editDetails()->page()->insertStackField(0, 0, 2, 3, 5);

    // AC Reso
    editDetails()->page()->newStackField(0);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex2 = "64";
    }
    else
    {
        hex2 = "67";
    };
    // formerly "AC RESONANCE" groupBox at addGroupBox(0, 1, 1, 1) — parentCol=1
    editDetails()->page()->addComboBox(0, 1, 1, 1, "10", hex1, hex2, "00", "large");
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "01", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "03", "normal_ratio2");
    editDetails()->page()->addStackField();

    // Auto Wah
    editDetails()->page()->newStackField(0);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex2 = "65";
    }
    else
    {
        hex2 = "68";
    };
    // formerly "AUTO WAH" groupBox at addGroupBox(0, 1, 1, 1) — parentCol=1
    editDetails()->page()->addComboBox(0, 1, 1, 1, "10", hex1, hex2, "00", "large");
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "01", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "04", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 5, 1, 1, "10", hex1, hex2, "05", "normal_ratio1.75");
    editDetails()->page()->addComboBox(0, 6, 1, 1, "10", hex1, hex2, "06", "large");
    // formerly "LEVEL" groupBox at addGroupBox(0, 2, 1, 1) — sequential after col 6
    editDetails()->page()->addKnob(0, 7, 1, 1, "10", hex1, hex2, "03", "normal_ratio2");
    editDetails()->page()->addKnob(0, 8, 1, 1, "10", hex1, hex2, "07", "normal_ratio1.75");
    editDetails()->page()->addStackField();

    //Chorus
    editDetails()->page()->newStackField(0);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex2 = "66";
    }
    else
    {
        hex2 = "69";
    };
    // formerly "EFFECT" groupBox at addGroupBox(0, 0, 1, 1) — parentCol=0
    editDetails()->page()->newStackControl(1);
    editDetails()->page()->addComboBox(1, 0, 1, 1, "10", hex1, hex2, "01", "large");           //mode
    editDetails()->page()->addStackControl();
    editDetails()->page()->insertStackField(1, 1, 0, 1, 2);
    // formerly "LEVEL" groupBox at addGroupBox(0, 1, 1, 1) — parentCol=1
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "17", "normal_ratio1.5");     //direct
    editDetails()->page()->addComboBox(0, 2, 1, 1, "10", hex1, hex2, "18", "large");           //output
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(1);
    // formerly "CHORUS" groupBox at addGroupBox(0, 0, 1, 1) — parentCol=0
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.5", "bottom", 60); //rate
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "03", "normal_ratio1.5", "bottom", 60); //depth
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "04", "normal_ratio1.5", "bottom", 60); //predelay
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "05", "normal_ratio1.75", "bottom", 60); //effect
    // formerly "FILTER" groupBox at addGroupBox(0, 1, 1, 1) — sequential after col 3
    editDetails()->page()->addComboBox(0, 4, 1, 1, "10", hex1, hex2, "06"); //waveform
    editDetails()->page()->addKnob(0, 5, 1, 1, "10", hex1, hex2, "07", "normal_ratio1.25", "bottom", 60);
    editDetails()->page()->addKnob(0, 6, 1, 1, "10", hex1, hex2, "08", "normal_ratio1.25", "bottom", 60);
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(1);
    // formerly "CHORUS" groupBox at addGroupBox(0, 0, 1, 1) — parentCol=0
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.5", "bottom", 60); //rate
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "03", "normal_ratio1.5", "bottom", 60); //depth
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "04", "normal_ratio1.5", "bottom", 60); //predelay
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "05", "normal_ratio1.75", "bottom", 60); //effect
    // formerly "FILTER" groupBox at addGroupBox(0, 1, 1, 1) — sequential after col 3
    editDetails()->page()->addComboBox(0, 4, 1, 1, "10", hex1, hex2, "06"); //waveform
    editDetails()->page()->addKnob(0, 5, 1, 1, "10", hex1, hex2, "07", "normal_ratio1.25", "bottom", 60);
    editDetails()->page()->addKnob(0, 6, 1, 1, "10", hex1, hex2, "08", "normal_ratio1.25", "bottom", 60);
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(1);
    // formerly "CHORUS" groupBox at addGroupBox(0, 0, 1, 1) — parentCol=0
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.5", "bottom", 60); //rate
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "03", "normal_ratio1.5", "bottom", 60); //depth
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "04", "normal_ratio1.5", "bottom", 60); //predelay
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "05", "normal_ratio1.75", "bottom", 60); //effect
    // formerly "FILTER" groupBox at addGroupBox(0, 1, 1, 1) — sequential after col 3
    editDetails()->page()->addComboBox(0, 4, 1, 1, "10", hex1, hex2, "06"); //waveform
    editDetails()->page()->addKnob(0, 5, 1, 1, "10", hex1, hex2, "07", "normal_ratio1.25", "bottom", 60);
    editDetails()->page()->addKnob(0, 6, 1, 1, "10", hex1, hex2, "08", "normal_ratio1.25", "bottom", 60);
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(1);
    // formerly "CHORUS 1" groupBox at addGroupBox(0, 0, 1, 1) — parentCol=0 row 0
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "09", "normal_ratio1.5", "bottom", 60); //rate
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "0A", "normal_ratio1.5", "bottom", 60); //depth
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "0B", "normal_ratio1.5", "bottom", 60); //predelay
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "0C", "normal_ratio1.75", "bottom", 60); //effect
    // formerly "FILTER 1" groupBox at addGroupBox(0, 1, 1, 1) — sequential after col 3 row 0
    editDetails()->page()->addComboBox(0, 4, 1, 1, "10", hex1, hex2, "0D"); //waveform
    editDetails()->page()->addKnob(0, 5, 1, 1, "10", hex1, hex2, "0E", "normal_ratio1.25", "bottom", 60);
    editDetails()->page()->addKnob(0, 6, 1, 1, "10", hex1, hex2, "0F", "normal_ratio1.25", "bottom", 60);
    // formerly "CHORUS 2" groupBox at addGroupBox(1, 0, 1, 1) — parentRow=1 parentCol=0 row 1
    editDetails()->page()->addKnob(1, 0, 1, 1, "10", hex1, hex2, "10", "normal_ratio1.5", "bottom", 60); //rate
    editDetails()->page()->addKnob(1, 1, 1, 1, "10", hex1, hex2, "11", "normal_ratio1.5", "bottom", 60); //depth
    editDetails()->page()->addKnob(1, 2, 1, 1, "10", hex1, hex2, "12", "normal_ratio1.5", "bottom", 60); //predelay
    editDetails()->page()->addKnob(1, 3, 1, 1, "10", hex1, hex2, "13", "normal_ratio1.75", "bottom", 60); //effect
    // formerly "FILTER 2" groupBox at addGroupBox(1, 1, 1, 1) — sequential after col 3 row 1
    editDetails()->page()->addComboBox(1, 4, 1, 1, "10", hex1, hex2, "14"); //waveform
    editDetails()->page()->addKnob(1, 5, 1, 1, "10", hex1, hex2, "15", "normal_ratio1.25", "bottom", 60);
    editDetails()->page()->addKnob(1, 6, 1, 1, "10", hex1, hex2, "16", "normal_ratio1.25", "bottom", 60);
    editDetails()->page()->addStackField();


    // Clasic Vibe
    editDetails()->page()->newStackField(0);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex2 = "67";
    }
    else
    {
        hex2 = "6A";
    };
    // formerly "CLASSIC VIBE" groupBox at addGroupBox(0, 1, 1, 1) — parentCol=1
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "00", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "01", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "02", "normal_ratio2");
    editDetails()->page()->addStackField();


    // Compressor
    editDetails()->page()->newStackField(0);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex2 = "68";
    }
    else
    {
        hex2 = "6B";
    };
    // formerly "COMPRESSOR" groupBox at addGroupBox(0, 1, 1, 1) — parentCol=1
    editDetails()->page()->addComboBox(0, 1, 1, 2, "10", hex1, hex2, "01", "large");
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "03", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 5, 1, 1, "10", hex1, hex2, "04", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 6, 1, 1, "10", hex1, hex2, "05", "normal_ratio2");
    editDetails()->page()->addKnob(0, 7, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.75");
    editDetails()->page()->addStackField();


    // Defretter
    editDetails()->page()->newStackField(0);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex2 = "69";
    }
    else
    {
        hex2 = "6C";
    };
    // formerly "DEFRETTER" groupBox at addGroupBox(0, 1, 1, 1) — parentCol=1
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "00", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "01", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "03", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 5, 1, 1, "10", hex1, hex2, "04", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 6, 1, 1, "10", hex1, hex2, "05", "normal_ratio2");
    editDetails()->page()->addKnob(0, 7, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.75");
    editDetails()->page()->addStackField();


    // Defretter Bass
    editDetails()->page()->newStackField(0);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex2 = "6A";
    }
    else
    {
        hex2 = "6D";
    };
    // formerly "DEFRETTER BASS" groupBox at addGroupBox(0, 1, 1, 1) — parentCol=1
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "00", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "01", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "03", "normal_ratio2");
    editDetails()->page()->addKnob(0, 5, 1, 1, "10", hex1, hex2, "04", "normal_ratio1.75");
    editDetails()->page()->addStackField();


    // DELAY
    editDetails()->page()->newStackField(0);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex2 = "6B";
    }
    else
    {
        hex2 = "6E";
    };
    // formerly "EFFECT" groupBox at addGroupBox(0, 0, 2, 1) — parentCol=0
    editDetails()->page()->newStackControl(2);
    editDetails()->page()->addComboBox(1, 0, 1, 1, "10", hex1, hex2, "01", "large");
    editDetails()->page()->addStackControl();
    editDetails()->page()->insertStackField(2, 0, 1, 2, 1);
    editDetails()->page()->addStackField();

    // STEREO 1
    editDetails()->page()->newStackField(2);
    // formerly "DELAY" groupBox at addGroupBox(0, 0, 4, 1) — parentCol=0
    editDetails()->page()->addDataKnob(0, 0, 1, 1, "10", hex1, hex2, "02", "DELAY2000_ratio1.75","bottom", 60); //4 byte time
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.25"); //feedback
    // formerly "FILTER" groupBox at addGroupBox(0, 1, 4, 1) — sequential col 2
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "07", "normal_ratio1.25", "bottom", 60); //high cut
    // formerly "LEVEL" groupBox at addGroupBox(0, 2, 4, 1) — sequential col 3
    editDetails()->page()->addKnob(0, 3, 1, 2, "10", hex1, hex2, "08", "turbo_ratio1.75"); //EFFECT
    editDetails()->page()->addKnob(1, 3, 1, 1, "10", hex1, hex2, "09", "normal_ratio1.5"); // direct
    editDetails()->page()->addKnob(1, 4, 1, 1, "10", hex1, hex2, "20", "normal_ratio1.25"); //carry over
    editDetails()->page()->addStackField();

    // STEREO 2
    editDetails()->page()->newStackField(2);
    // formerly "DELAY" groupBox at addGroupBox(0, 0, 4, 1) — parentCol=0
    editDetails()->page()->addDataKnob(0, 0, 1, 1, "10", hex1, hex2, "02", "DELAY2000_ratio1.75","bottom", 60); //4 byte time
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.25"); //feedback
    // formerly "FILTER" groupBox at addGroupBox(0, 1, 4, 1) — sequential col 2
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "07", "normal_ratio1.25", "bottom", 60); //high cut
    // formerly "LEVEL" groupBox at addGroupBox(0, 2, 4, 1) — sequential col 3
    editDetails()->page()->addKnob(0, 3, 1, 2, "10", hex1, hex2, "08", "turbo_ratio1.75"); //EFFECT
    editDetails()->page()->addKnob(1, 3, 1, 1, "10", hex1, hex2, "09", "normal_ratio1.5"); // direct
    editDetails()->page()->addKnob(1, 4, 1, 1, "10", hex1, hex2, "20", "normal_ratio1.25"); //carry over
    editDetails()->page()->addStackField();

    // PAN
    editDetails()->page()->newStackField(2);
    // formerly "DELAY" groupBox at addGroupBox(0, 0, 4, 1) — parentCol=0
    editDetails()->page()->addDataKnob(0, 0, 1, 1, "10", hex1, hex2, "02", "DELAY2000_ratio1.75","bottom", 60); //4 byte time
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.25"); //feedback
    editDetails()->page()->addKnob(1, 0, 1, 2, "10", hex1, hex2, "1F", "normal_ratio1.25"); //tap time %
    // formerly "FILTER" groupBox at addGroupBox(0, 1, 4, 1) — sequential col 2
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "07", "normal_ratio1.25", "bottom", 60); //high cut
    // formerly "LEVEL" groupBox at addGroupBox(0, 2, 4, 1) — sequential col 3
    editDetails()->page()->addKnob(0, 3, 1, 2, "10", hex1, hex2, "08", "turbo_ratio1.75"); //EFFECT
    editDetails()->page()->addKnob(1, 3, 1, 1, "10", hex1, hex2, "09", "normal_ratio1.5"); // direct
    editDetails()->page()->addKnob(1, 4, 1, 1, "10", hex1, hex2, "20", "normal_ratio1.25"); //carry over
    editDetails()->page()->addStackField();

    // DUAL SERIES
    editDetails()->page()->newStackField(2);
    // formerly "DELAY 1" groupBox at addGroupBox(0, 0, 1, 1) — row 0 cols 0-1
    editDetails()->page()->addDataKnob(0, 0, 1, 1, "10", hex1, hex2, "0A", "DELAY2000_ratio1.25","bottom", 60); //4 byte time
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.25"); //feedback
    // formerly "DELAY 2" groupBox at addGroupBox(1, 0, 1, 1) — row 1 cols 0-1
    editDetails()->page()->addDataKnob(1, 0, 1, 1, "10", hex1, hex2, "11", "DELAY2000_ratio1.25","bottom", 60); //4 byte time
    editDetails()->page()->addKnob(1, 1, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.25"); //feedback
    // formerly "FILTER 1" groupBox at addGroupBox(0, 1, 1, 1) — sequential col 2 row 0
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "0F", "normal_ratio1.25", "bottom", 60);
    // formerly "FILTER 2" groupBox at addGroupBox(1, 1, 1, 1) — sequential col 2 row 1
    editDetails()->page()->addKnob(1, 2, 1, 1, "10", hex1, hex2, "16", "normal_ratio1.25", "bottom", 60);
    // formerly "LEVEL 1" groupBox at addGroupBox(0, 2, 1, 1) — sequential col 3 row 0
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "10", "normal_ratio1.75");
    // formerly "LEVEL 2" groupBox at addGroupBox(1, 2, 1, 1) — sequential col 3 row 1
    editDetails()->page()->addKnob(1, 3, 1, 1, "10", hex1, hex2, "17", "normal_ratio1.75");
    // formerly "Direct" groupBox at addGroupBox(0, 3, 2, 1) — sequential col 4 rows 0-1
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "09", "normal_ratio1.5");
    editDetails()->page()->addKnob(1, 4, 1, 1, "10", hex1, hex2, "20", "normal_ratio1.25"); //carry over
    editDetails()->page()->addStackField();

    // DUAL PARALLEL
    editDetails()->page()->newStackField(2);
    // formerly "DELAY 1" groupBox at addGroupBox(0, 0, 1, 1) — row 0 cols 0-1
    editDetails()->page()->addDataKnob(0, 0, 1, 1, "10", hex1, hex2, "0A", "DELAY2000_ratio1.25","bottom", 60); //4 byte time
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.25"); //feedback
    // formerly "DELAY 2" groupBox at addGroupBox(1, 0, 1, 1) — row 1 cols 0-1
    editDetails()->page()->addDataKnob(1, 0, 1, 1, "10", hex1, hex2, "11", "DELAY2000_ratio1.25","bottom", 60); //4 byte time
    editDetails()->page()->addKnob(1, 1, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.25"); //feedback
    // formerly "FILTER 1" groupBox at addGroupBox(0, 1, 1, 1) — sequential col 2 row 0
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "0F", "normal_ratio1.25", "bottom", 60);
    // formerly "FILTER 2" groupBox at addGroupBox(1, 1, 1, 1) — sequential col 2 row 1
    editDetails()->page()->addKnob(1, 2, 1, 1, "10", hex1, hex2, "16", "normal_ratio1.25", "bottom", 60);
    // formerly "LEVEL 1" groupBox at addGroupBox(0, 2, 1, 1) — sequential col 3 row 0
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "10", "normal_ratio1.75");
    // formerly "LEVEL 2" groupBox at addGroupBox(1, 2, 1, 1) — sequential col 3 row 1
    editDetails()->page()->addKnob(1, 3, 1, 1, "10", hex1, hex2, "17", "normal_ratio1.75");
    // formerly "Direct" groupBox at addGroupBox(0, 3, 2, 1) — sequential col 4 rows 0-1
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "09", "normal_ratio1.5");
    editDetails()->page()->addKnob(1, 4, 1, 1, "10", hex1, hex2, "20", "normal_ratio1.25"); //carry over
    editDetails()->page()->addStackField();

    // DUAL L/R
    editDetails()->page()->newStackField(2);
    // formerly "DELAY 1" groupBox at addGroupBox(0, 0, 1, 1) — row 0 cols 0-1
    editDetails()->page()->addDataKnob(0, 0, 1, 1, "10", hex1, hex2, "0A", "DELAY2000_ratio1.25","bottom", 60); //4 byte time
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.25"); //feedback
    // formerly "DELAY 2" groupBox at addGroupBox(1, 0, 1, 1) — row 1 cols 0-1
    editDetails()->page()->addDataKnob(1, 0, 1, 1, "10", hex1, hex2, "11", "DELAY2000_ratio1.25","bottom", 60); //4 byte time
    editDetails()->page()->addKnob(1, 1, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.25"); //feedback
    // formerly "FILTER 1" groupBox at addGroupBox(0, 1, 1, 1) — sequential col 2 row 0
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "0F", "normal_ratio1.25", "bottom", 60);
    // formerly "FILTER 2" groupBox at addGroupBox(1, 1, 1, 1) — sequential col 2 row 1
    editDetails()->page()->addKnob(1, 2, 1, 1, "10", hex1, hex2, "16", "normal_ratio1.25", "bottom", 60);
    // formerly "LEVEL 1" groupBox at addGroupBox(0, 2, 1, 1) — sequential col 3 row 0
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "10", "normal_ratio1.75");
    // formerly "LEVEL 2" groupBox at addGroupBox(1, 2, 1, 1) — sequential col 3 row 1
    editDetails()->page()->addKnob(1, 3, 1, 1, "10", hex1, hex2, "17", "normal_ratio1.75");
    // formerly "Direct" groupBox at addGroupBox(0, 3, 2, 1) — sequential col 4 rows 0-1
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "09", "normal_ratio1.5");
    editDetails()->page()->addKnob(1, 4, 1, 1, "10", hex1, hex2, "20", "normal_ratio1.25"); //carry over
    editDetails()->page()->addStackField();

    // REVERSE
    editDetails()->page()->newStackField(2);
    // formerly "DELAY" groupBox at addGroupBox(0, 0, 4, 1) — parentCol=0
    editDetails()->page()->addDataKnob(0, 0, 1, 1, "10", hex1, hex2, "02", "DELAY2000_ratio1.75","bottom", 60); //4 byte time
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.25"); //feedback
    // formerly "FILTER" groupBox at addGroupBox(0, 1, 4, 1) — sequential col 2
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "07", "normal_ratio1.25", "bottom", 60); //high cut
    // formerly "LEVEL" groupBox at addGroupBox(0, 2, 4, 1) — sequential col 3
    editDetails()->page()->addKnob(0, 3, 1, 2, "10", hex1, hex2, "08", "turbo_ratio1.75"); //EFFECT
    editDetails()->page()->addKnob(1, 3, 1, 1, "10", hex1, hex2, "09", "normal_ratio1.5"); // direct
    editDetails()->page()->addKnob(1, 4, 1, 1, "10", hex1, hex2, "20", "normal_ratio1.25"); //carry over
    editDetails()->page()->addStackField();

    // ANALOG
    editDetails()->page()->newStackField(2);
    // formerly "DELAY" groupBox at addGroupBox(0, 0, 4, 1) — parentCol=0
    editDetails()->page()->addDataKnob(0, 0, 1, 1, "10", hex1, hex2, "02", "DELAY2000_ratio1.75","bottom", 60); //4 byte time
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.25"); //feedback
    // formerly "FILTER" groupBox at addGroupBox(0, 1, 4, 1) — sequential col 2
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "07", "normal_ratio1.25", "bottom", 60); //high cut
    // formerly "LEVEL" groupBox at addGroupBox(0, 2, 4, 1) — sequential col 3
    editDetails()->page()->addKnob(0, 3, 1, 2, "10", hex1, hex2, "08", "turbo_ratio1.75"); //EFFECT
    editDetails()->page()->addKnob(1, 3, 1, 1, "10", hex1, hex2, "09", "normal_ratio1.5"); // direct
    editDetails()->page()->addKnob(1, 4, 1, 1, "10", hex1, hex2, "20", "normal_ratio1.25"); //carry over
    editDetails()->page()->addStackField();

    // TAPE
    editDetails()->page()->newStackField(2);
    // formerly "DELAY" groupBox at addGroupBox(0, 0, 4, 1) — parentCol=0
    editDetails()->page()->addDataKnob(0, 0, 1, 1, "10", hex1, hex2, "02", "DELAY2000_ratio1.75","bottom", 60); //4 byte time
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.25"); //feedback
    // formerly "FILTER" groupBox at addGroupBox(0, 1, 4, 1) — sequential col 2
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "07", "normal_ratio1.25", "bottom", 60); //high cut
    // formerly "LEVEL" groupBox at addGroupBox(0, 2, 4, 1) — sequential col 3
    editDetails()->page()->addKnob(0, 3, 1, 2, "10", hex1, hex2, "08", "turbo_ratio1.75"); //EFFECT
    editDetails()->page()->addKnob(1, 3, 1, 1, "10", hex1, hex2, "09", "normal_ratio1.5"); // direct
    editDetails()->page()->addKnob(1, 4, 1, 1, "10", hex1, hex2, "20", "normal_ratio1.25"); //carry over
    editDetails()->page()->addStackField();

    // MODULATE — Delay(0-1), Modulate(2-4), Filter(5), Level(6-8)
    editDetails()->page()->newStackField(2);
    // formerly "DELAY" groupBox at addGroupBox(0, 0, 1, 1) — parentCol=0
    editDetails()->page()->addDataKnob(0, 0, 1, 1, "10", hex1, hex2, "02", "DELAY2000_ratio1.75","bottom", 60); //4 byte time
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.25"); //feedback
    // formerly "MODULATE" groupBox at addGroupBox(1, 0, 1, 4) — sequential cols 2-4 (collapsed to row 0)
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "18", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "19", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "1A", "normal_ratio1.25");
    // formerly "FILTER" groupBox at addGroupBox(0, 1, 1, 1) — sequential col 5
    editDetails()->page()->addKnob(0, 5, 1, 1, "10", hex1, hex2, "07", "normal_ratio1.25", "bottom", 60); //high cut
    // formerly "LEVEL" groupBox at addGroupBox(0, 2, 1, 1) — sequential col 6
    editDetails()->page()->addKnob(0, 6, 1, 2, "10", hex1, hex2, "08", "turbo_ratio1.75"); //EFFECT
    editDetails()->page()->addKnob(1, 6, 1, 1, "10", hex1, hex2, "09", "normal_ratio1.5"); // direct
    editDetails()->page()->addKnob(1, 7, 1, 1, "10", hex1, hex2, "20", "normal_ratio1.25"); //carry over
    editDetails()->page()->addStackField();

    // WARP
    editDetails()->page()->newStackField(2);
    // formerly "DELAY" groupBox at addGroupBox(0, 0, 4, 1) — parentCol=0
    editDetails()->page()->addDataKnob(0, 0, 1, 1, "10", hex1, hex2, "02", "DELAY2000_ratio1.75","bottom", 60); //4 byte time
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.25"); //feedback
    // formerly "FILTER" groupBox at addGroupBox(0, 1, 4, 1) — sequential col 2
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "07", "normal_ratio1.25", "bottom", 60); //high cut
    // formerly "LEVEL" groupBox at addGroupBox(0, 2, 4, 1) — sequential col 3
    editDetails()->page()->addKnob(0, 3, 1, 2, "10", hex1, hex2, "08", "turbo_ratio1.75"); //EFFECT
    editDetails()->page()->addKnob(1, 3, 1, 1, "10", hex1, hex2, "09", "normal_ratio1.5"); // direct
    editDetails()->page()->addKnob(1, 4, 1, 1, "10", hex1, hex2, "20", "normal_ratio1.25"); //carry over
    editDetails()->page()->addStackField();

    // TWIST — Delay(0-1), Twist(nested stackControl at cols 2-3), Filter(4), Level(5-7)
    editDetails()->page()->newStackField(2);
    // formerly "DELAY" groupBox at addGroupBox(0, 0, 1, 2) — parentCol=0
    editDetails()->page()->addDataKnob(0, 0, 1, 1, "10", hex1, hex2, "02", "DELAY2000_ratio1.75","bottom", 60); //4 byte time
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.25"); //feedback
    // formerly "TWIST" groupBox at addGroupBox(1, 0, 1, 5) — sequential col 2 (collapsed to row 0)
    editDetails()->page()->newStackControl(3);
    editDetails()->page()->addComboBox(0, 2, 1, 2, "10", hex1, hex2, "1B", "large");
    editDetails()->page()->addStackControl();
    editDetails()->page()->insertStackField(3, 0, 2, 1, 2);
    // formerly "FILTER" groupBox at addGroupBox(0, 2, 1, 1) — sequential col 4
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "07", "normal_ratio1.25", "bottom", 60); //high cut
    // formerly "LEVEL" groupBox at addGroupBox(0, 3, 1, 2) — sequential col 5
    editDetails()->page()->addKnob(0, 5, 1, 2, "10", hex1, hex2, "08", "turbo_ratio1.75"); //EFFECT
    editDetails()->page()->addKnob(1, 5, 1, 1, "10", hex1, hex2, "09", "normal_ratio1.5"); // direct
    editDetails()->page()->addKnob(1, 6, 1, 1, "10", hex1, hex2, "20", "normal_ratio1.25"); //carry over
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(3);
    editDetails()->page()->addKnob(0, 2, 1, 2, "10", hex1, hex2, "1C", "normal_ratio1.5");     //rise
    editDetails()->page()->addKnob(0, 4, 1, 2, "10", hex1, hex2, "1D", "normal_ratio1.5");     //fall
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(3);
    editDetails()->page()->addKnob(0, 2, 1, 2, "10", hex1, hex2, "1C", "normal_ratio1.5");     //rise
    editDetails()->page()->addKnob(0, 4, 1, 2, "10", hex1, hex2, "1E", "normal_ratio1.5");     //fade
    editDetails()->page()->addStackField();



    // Flanger
    editDetails()->page()->newStackField(0);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex2 = "6C";
    }
    else
    {
        hex2 = "6F";
    };
    // formerly "FLANGER" groupBox at addGroupBox(0, 1, 1, 1) — parentCol=1
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "00", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "01", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "03", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 5, 1, 1, "10", hex1, hex2, "04", "normal_ratio1.5");
    // formerly "FILTER" groupBox at addGroupBox(0, 2, 1, 1) — sequential col 6
    editDetails()->page()->addKnob(0, 6, 1, 1, "10", hex1, hex2, "05", "normal_ratio1.5", "bottom", 60);
    editDetails()->page()->addKnob(0, 7, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.5", "bottom", 60);
    // formerly "LEVEL" groupBox at addGroupBox(0, 3, 1, 1) — sequential col 8
    editDetails()->page()->addKnob(0, 8, 1, 1, "10", hex1, hex2, "07", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 9, 1, 1, "10", hex1, hex2, "08", "normal_ratio1.5");
    editDetails()->page()->addStackField();


    // Flanger Bass
    editDetails()->page()->newStackField(0);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex2 = "6D";
    }
    else
    {
        hex2 = "70";
    };
    // formerly "FLANGER BASS" groupBox at addGroupBox(0, 1, 1, 1) — parentCol=1
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "00", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "01", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "03", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 5, 1, 1, "10", hex1, hex2, "04", "normal_ratio1.5");
    // formerly "FILTER" groupBox at addGroupBox(0, 2, 1, 1) — sequential col 6
    editDetails()->page()->addKnob(0, 6, 1, 1, "10", hex1, hex2, "05", "normal_ratio1.5", "bottom", 60);
    editDetails()->page()->addKnob(0, 7, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.5", "bottom", 60);
    // formerly "LEVEL" groupBox at addGroupBox(0, 3, 1, 1) — sequential col 8
    editDetails()->page()->addKnob(0, 8, 1, 1, "10", hex1, hex2, "07", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 9, 1, 1, "10", hex1, hex2, "08", "normal_ratio1.5");
    editDetails()->page()->addStackField();


    // Foot Volume
    editDetails()->page()->newStackField(0);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex2 = "6E";
    }
    else
    {
        hex2 = "71";
    };
    // formerly "FOOT VOLUME" groupBox at addGroupBox(0, 0, 1, 1) — parentCol=0
    editDetails()->page()->addDataKnob(0, 0, 1, 1, "10", hex1, hex2, "00", "0~1000_ratio2");
    editDetails()->page()->addDataKnob(0, 1, 1, 1, "10", hex1, hex2, "04", "0~1000_ratio2");
    editDetails()->page()->addDataKnob(0, 2, 1, 1, "10", hex1, hex2, "08", "0~1000_ratio1.75");
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "0C", "normal_ratio1.5", "bottom", 60);
    editDetails()->page()->addStackField();


    // Graphic EQ
    editDetails()->page()->newStackField(0);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex2 = "6F";
    }
    else
    {
        hex2 = "72";
    };
    // formerly "EQUALIZER" groupBox at addGroupBox(0, 2, 1, 7) — parentCol=2
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "00", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "01", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 5, 1, 1, "10", hex1, hex2, "03", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 6, 1, 1, "10", hex1, hex2, "04", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 7, 1, 1, "10", hex1, hex2, "05", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 8, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.5");
    // formerly "LEVEL" groupBox at addGroupBox(0, 8, 1, 1) — sequential col 9
    editDetails()->page()->addKnob(0, 9, 1, 1, "10", hex1, hex2, "07", "normal_ratio1.5");
    editDetails()->page()->addStackField();




    // Harmonizer
    editDetails()->page()->newStackField(0);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex2 = "70";
    }
    else
    {
        hex2 = "73";
    };
    // formerly "Effect" groupBox at addGroupBox(0, 0, 2, 3) — parentCol=0
    editDetails()->page()->newStackControl(4);
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, hex2, "00", "large");
    editDetails()->page()->addStackControl();
    editDetails()->page()->insertStackField(4, 0, 1, 3, 9);
    // formerly "User Harmony 1" groupBox at addGroupBox(3, 0, 1, 12) — parentRow=3, parentCol=0
    editDetails()->page()->addComboBox(3, 0, 1, 1, "10", hex1, hex2, "0F");
    editDetails()->page()->addComboBox(3, 1, 1, 1, "10", hex1, hex2, "10");
    editDetails()->page()->addComboBox(3, 2, 1, 1, "10", hex1, hex2, "11");
    editDetails()->page()->addComboBox(3, 3, 1, 1, "10", hex1, hex2, "12");
    editDetails()->page()->addComboBox(3, 4, 1, 1, "10", hex1, hex2, "13");
    editDetails()->page()->addComboBox(3, 5, 1, 1, "10", hex1, hex2, "14");
    editDetails()->page()->addComboBox(3, 6, 1, 1, "10", hex1, hex2, "15");
    editDetails()->page()->addComboBox(3, 7, 1, 1, "10", hex1, hex2, "16");
    editDetails()->page()->addComboBox(3, 8, 1, 1, "10", hex1, hex2, "17");
    editDetails()->page()->addComboBox(3, 9, 1, 1, "10", hex1, hex2, "18");
    editDetails()->page()->addComboBox(3, 10, 1, 1, "10", hex1, hex2, "19");
    editDetails()->page()->addComboBox(3, 11, 1, 1, "10", hex1, hex2, "1A");
    editDetails()->page()->addSystemOverride(3, 0, 1, 12, "10", hex1, hex2, "01", "1D", "not_equal"); //
    // formerly "User Harmony 2" groupBox at addGroupBox(4, 0, 1, 12) — parentRow=4, parentCol=0
    editDetails()->page()->addComboBox(4, 0, 1, 1, "10", hex1, hex2, "1B");
    editDetails()->page()->addComboBox(4, 1, 1, 1, "10", hex1, hex2, "1C");
    editDetails()->page()->addComboBox(4, 2, 1, 1, "10", hex1, hex2, "1D");
    editDetails()->page()->addComboBox(4, 3, 1, 1, "10", hex1, hex2, "1E");
    editDetails()->page()->addComboBox(4, 4, 1, 1, "10", hex1, hex2, "1F");
    editDetails()->page()->addComboBox(4, 5, 1, 1, "10", hex1, hex2, "20");
    editDetails()->page()->addComboBox(4, 6, 1, 1, "10", hex1, hex2, "21");
    editDetails()->page()->addComboBox(4, 7, 1, 1, "10", hex1, hex2, "22");
    editDetails()->page()->addComboBox(4, 8, 1, 1, "10", hex1, hex2, "23");
    editDetails()->page()->addComboBox(4, 9, 1, 1, "10", hex1, hex2, "24");
    editDetails()->page()->addComboBox(4, 10, 1, 1, "10", hex1, hex2, "25");
    editDetails()->page()->addComboBox(4, 11, 1, 1, "10", hex1, hex2, "26");
    editDetails()->page()->addSystemOverride(4, 0, 1, 12, "10", hex1, hex2, "02", "1D", "not_equal"); //
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(4);
    // formerly "Harmonist" groupBox at addGroupBox(0, 1, 2, 2) — parentCol=1
    //   formerly "Voice 1" groupBox at addGroupBox(0, 0, 2, 1) inside Harmonist — parentCol=0+1=1
    editDetails()->page()->addComboBox(0, 1, 1, 1, "10", hex1, hex2, "01", "large");
    editDetails()->page()->addDataKnob(0, 2, 1, 1, "10", hex1, hex2, "04", "PREDELAY_ratio1.25","bottom", 70);
    //   knob "08" at Harmonist(0,1) — sequential col 3
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "08", "normal_ratio1.25");
    // formerly "Level" groupBox at addGroupBox(0, 3, 2, 1) — sequential col 4
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "03", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 5, 1, 1, "10", hex1, hex2, "09", "normal_ratio1.5");
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(4);
    // formerly "Harmonist" groupBox at addGroupBox(0, 1, 2, 2) — parentCol=1
    //   formerly "Voice 1" groupBox at addGroupBox(0, 0, 1, 1) inside Harmonist — parentCol=0+1=1
    editDetails()->page()->addComboBox(0, 1, 1, 1, "10", hex1, hex2, "01", "large");
    editDetails()->page()->addDataKnob(0, 2, 1, 1, "10", hex1, hex2, "04", "PREDELAY_ratio1.25");
    //   formerly "Voice 2" groupBox at addGroupBox(2, 0, 1, 1) inside Harmonist — parentRow=2+0=2, parentCol=0+1=1
    editDetails()->page()->addComboBox(2, 1, 1, 1, "10", hex1, hex2, "02", "large");
    editDetails()->page()->addDataKnob(2, 2, 1, 1, "10", hex1, hex2, "0B", "PREDELAY_ratio1.25");
    //   knob "08" at Harmonist(0,1,2,1) — sequential col 3, rowSpan=2
    editDetails()->page()->addKnob(0, 3, 2, 1, "10", hex1, hex2, "08", "normal_ratio1.5");
    // formerly "Level" groupBox at addGroupBox(0, 3, 2, 1) — sequential col 4
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "03", "normal_ratio1.5");
    editDetails()->page()->addKnob(1, 4, 1, 1, "10", hex1, hex2, "0A", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 5, 2, 1, "10", hex1, hex2, "09", "normal_ratio1.25");
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(4);
    // formerly "Harmonist" groupBox at addGroupBox(0, 1, 2, 2) — parentCol=1
    //   formerly "Voice 1" groupBox at addGroupBox(0, 0, 1, 1) inside Harmonist — parentCol=0+1=1
    editDetails()->page()->addComboBox(0, 1, 1, 1, "10", hex1, hex2, "01", "large");
    editDetails()->page()->addDataKnob(0, 2, 1, 1, "10", hex1, hex2, "04", "PREDELAY_ratio1.25");
    //   formerly "Voice 2" groupBox at addGroupBox(2, 0, 1, 1) inside Harmonist — parentRow=2+0=2, parentCol=0+1=1
    editDetails()->page()->addComboBox(2, 1, 1, 1, "10", hex1, hex2, "02", "large");
    editDetails()->page()->addDataKnob(2, 2, 1, 1, "10", hex1, hex2, "0B", "PREDELAY_ratio1.25");
    //   knob "08" at Harmonist(0,1,2,1) — sequential col 3, rowSpan=2
    editDetails()->page()->addKnob(0, 3, 2, 1, "10", hex1, hex2, "08", "normal_ratio1.5");
    // formerly "Level" groupBox at addGroupBox(0, 3, 2, 1) — sequential col 4
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "03", "normal_ratio1.5");
    editDetails()->page()->addKnob(1, 4, 1, 1, "10", hex1, hex2, "0A", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 5, 2, 1, "10", hex1, hex2, "09", "normal_ratio1.25");
    editDetails()->page()->addStackField();


    // Humanizer
    editDetails()->page()->newStackField(0);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex2 = "71";
    }
    else
    {
        hex2 = "74";
    };
    // formerly "HUMANIZER" groupBox at addGroupBox(0, 2, 1, 8) — parentCol=2
    editDetails()->page()->addComboBox(0, 2, 1, 2, "10", hex1, hex2, "00", "large");
    editDetails()->page()->addComboBox(0, 4, 1, 1, "10", hex1, hex2, "01", "large");
    editDetails()->page()->addComboBox(1, 4, 1, 1, "10", hex1, hex2, "02", "large");
    editDetails()->page()->addKnob(0, 6, 2, 1, "10", hex1, hex2, "03", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 7, 2, 1, "10", hex1, hex2, "04", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 8, 2, 1, "10", hex1, hex2, "05", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 9, 2, 1, "10", hex1, hex2, "06", "normal_ratio1.5");
    // formerly "LEVEL" groupBox at addGroupBox(0, 9, 1, 1) — sequential col 10
    editDetails()->page()->addKnob(0, 10, 1, 1, "10", hex1, hex2, "07", "normal_ratio1.75");
    editDetails()->page()->addStackField();


    // Isolator
    editDetails()->page()->newStackField(0);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex2 = "72";
    }
    else
    {
        hex2 = "75";
    };
    // formerly "ISOLATOR" groupBox at addGroupBox(0, 1, 1, 1) — parentCol=1
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "00", "normal_ratio1.5", "bottom", 60);
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "01", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "03", "normal_ratio1.75");
    editDetails()->page()->addStackField();


    // Limiter
    editDetails()->page()->newStackField(0);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex2 = "73";
    }
    else
    {
        hex2 = "76";
    };
    // formerly "LIMITER" groupBox at addGroupBox(0, 1, 1, 1) — parentCol=1
    editDetails()->page()->addComboBox(0, 1, 1, 2, "10", hex1, hex2, "00", "large");
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "01", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 5, 1, 1, "10", hex1, hex2, "04", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 6, 1, 1, "10", hex1, hex2, "05", "normal_ratio1.5");
    // formerly "LEVEL" groupBox at addGroupBox(0, 2, 1, 1) — sequential col 7
    editDetails()->page()->addKnob(0, 7, 1, 1, "10", hex1, hex2, "03", "normal_ratio1.75");
    editDetails()->page()->addStackField();


    // LoFi
    editDetails()->page()->newStackField(0);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex2 = "74";
    }
    else
    {
        hex2 = "77";
    };
    // formerly "LOFI" groupBox at addGroupBox(0, 1, 1, 1) — parentCol=1
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "00", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "01", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "02", "normal_ratio2","bottom", 70);
    editDetails()->page()->addStackField();


    // Octave
    editDetails()->page()->newStackField(0);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex2 = "75";
    }
    else
    {
        hex2 = "78";
    };
    // formerly "OCTAVE" groupBox at addGroupBox(0, 1, 1, 1) — parentCol=1
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "00", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "01", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.75");
    editDetails()->page()->addStackField();


    // Octave Bass
    editDetails()->page()->newStackField(0);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex2 = "76";
    }
    else
    {
        hex2 = "79";
    };
    // formerly "OCTAVE BASS" groupBox at addGroupBox(0, 1, 1, 1) — parentCol=1
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "00", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "01", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.75");
    editDetails()->page()->addStackField();


    // Panner
    editDetails()->page()->newStackField(0);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex2 = "77";
    }
    else
    {
        hex2 = "7A";
    };
    // formerly "AUTO PAN" groupBox at addGroupBox(0, 1, 1, 6) — parentCol=1
    editDetails()->page()->addComboBox(0, 1, 1, 2, "10", hex1, hex2, "00", "large");
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "01", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 5, 1, 1, "10", hex1, hex2, "03", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 6, 1, 1, "10", hex1, hex2, "05", "normal_ratio1.75");
    // formerly "LEVEL" groupBox at addGroupBox(0, 7, 1, 1) — parentCol=7
    editDetails()->page()->addKnob(0, 7, 1, 1, "10", hex1, hex2, "04", "normal_ratio1.75");
    editDetails()->page()->addStackField();


    // EQ
    editDetails()->page()->newStackField(0);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex2 = "78";
    }
    else
    {
        hex2 = "7B";
    };
    // formerly "PARAMETRIC EQUALIZER" groupBox at addGroupBox(0, 0, 1, 1) — parentCol=0
    editDetails()->page()->addParaEQ(0, 1, 2, 4, "10", hex1, hex2, "01");
    editDetails()->page()->addStackField();


    // Pedal Bend
    editDetails()->page()->newStackField(0);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex2 = "79";
    }
    else
    {
        hex2 = "7C";
    };
    // formerly "PEDAL BEND" groupBox at addGroupBox(0, 1, 1, 1) — parentCol=1
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "00", "normal_ratio1.75");
    editDetails()->page()->addDataKnob(0, 3, 1, 1, "10", hex1, hex2, "02", "0~1000_ratio1.75","bottom", 70);
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "01", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 5, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.75");
    editDetails()->page()->addStackField();


    // Phaser
    editDetails()->page()->newStackField(0);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex2 = "7A";
    }
    else
    {
        hex2 = "7D";
    };
    // formerly "PHASER" groupBox at addGroupBox(0, 1, 1, 1) — parentCol=1
    editDetails()->page()->addComboBox(0, 1, 1, 1, "10", hex1, hex2, "00", "large");
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "01", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "03", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 5, 1, 1, "10", hex1, hex2, "04", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 6, 1, 1, "10", hex1, hex2, "05", "normal_ratio1.75");
    // formerly "LEVEL" groupBox at addGroupBox(0, 2, 1, 1) — sequential col 7
    editDetails()->page()->addKnob(0, 7, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 8, 1, 1, "10", hex1, hex2, "07", "normal_ratio1.75");
    editDetails()->page()->addStackField();


    // Pitch Shifter
    editDetails()->page()->newStackField(0);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex2 = "7B";
    }
    else
    {
        hex2 = "7E";
    };
    // formerly "Effect" groupBox at addGroupBox(0, 0, 2, 1) — parentCol=0
    editDetails()->page()->newStackControl(5);
    editDetails()->page()->addComboBox(1, 0, 1, 1, "10", hex1, hex2, "00", "large");
    editDetails()->page()->addStackControl();
    editDetails()->page()->insertStackField(5, 0, 1, 3, 1);
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(5);
    // formerly "PITCH SHIFTER" groupBox at addGroupBox(0, 1, 2, 4) — parentCol=1
    //   formerly "VOICE 1" groupBox at addGroupBox(0, 0, 3, 3) — parentCol=0+1=1
    editDetails()->page()->addComboBox(0, 1, 1, 3, "10", hex1, hex2, "04", "large");
    editDetails()->page()->addDataKnob(1, 1, 1, 1, "10", hex1, hex2, "06", "PREDELAY_ratio1.5","bottom", 70);
    editDetails()->page()->addKnob(1, 2, 1, 1, "10", hex1, hex2, "01", "normal_ratio1.5");
    editDetails()->page()->addKnob(1, 3, 1, 1, "10", hex1, hex2, "05", "normal_ratio1.25");
    //   knob "0B" at PITCH SHIFTER(1,4) → stackField(1,1+4)=(1,5)
    editDetails()->page()->addKnob(1, 5, 1, 1, "10", hex1, hex2, "0B", "normal_ratio1.5");
    // formerly "LEVEL" groupBox at addGroupBox(0, 5, 2, 1) — sequential col 6
    editDetails()->page()->addKnob(0, 6, 1, 1, "10", hex1, hex2, "0A", "normal_ratio1.5");
    editDetails()->page()->addKnob(1, 6, 1, 1, "10", hex1, hex2, "03", "normal_ratio1.25");
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(5);
    // formerly "PITCH SHIFTER" groupBox at addGroupBox(0, 1, 2, 4) — parentCol=1
    //   formerly "VOICE 1" groupBox at addGroupBox(0, 0, 3, 3) — parentCol=0+1=1
    editDetails()->page()->addComboBox(0, 1, 1, 3, "10", hex1, hex2, "04", "large");
    editDetails()->page()->addDataKnob(1, 1, 1, 1, "10", hex1, hex2, "06", "PREDELAY_ratio1.5","bottom", 70);
    editDetails()->page()->addKnob(1, 2, 1, 1, "10", hex1, hex2, "01", "normal_ratio1.5");
    editDetails()->page()->addKnob(1, 3, 1, 1, "10", hex1, hex2, "05", "normal_ratio1.25");
    //   knob "0B" at PS(1,4) → stackField(1,5)
    editDetails()->page()->addKnob(1, 5, 1, 1, "10", hex1, hex2, "0B", "normal_ratio1.5");
    //   formerly "VOICE 2" groupBox at addGroupBox(3, 0, 1, 3) — parentRow=3+0=3, parentCol=0+1=1
    editDetails()->page()->addComboBox(3, 1, 1, 3, "10", hex1, hex2, "0C", "large");
    editDetails()->page()->addDataKnob(4, 1, 1, 1, "10", hex1, hex2, "0E", "PREDELAY_ratio1.5","bottom", 70);
    editDetails()->page()->addKnob(4, 2, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.5");
    editDetails()->page()->addKnob(4, 3, 1, 1, "10", hex1, hex2, "0D", "normal_ratio1.25");
    // formerly "LEVEL" groupBox at addGroupBox(0, 5, 2, 1) — sequential col 6
    editDetails()->page()->addKnob(0, 6, 1, 1, "10", hex1, hex2, "0A", "normal_ratio1.75");
    editDetails()->page()->addKnob(2, 6, 1, 1, "10", hex1, hex2, "12", "normal_ratio1.75");
    editDetails()->page()->addKnob(1, 6, 1, 1, "10", hex1, hex2, "03", "normal_ratio1.5");
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(5);
    // formerly "PITCH SHIFTER" groupBox at addGroupBox(0, 1, 2, 4) — parentCol=1
    //   formerly "VOICE 1" groupBox at addGroupBox(0, 0, 3, 3) — parentCol=0+1=1
    editDetails()->page()->addComboBox(0, 1, 1, 3, "10", hex1, hex2, "04", "large");
    editDetails()->page()->addDataKnob(1, 1, 1, 1, "10", hex1, hex2, "06", "PREDELAY_ratio1.5","bottom", 70);
    editDetails()->page()->addKnob(1, 2, 1, 1, "10", hex1, hex2, "01", "normal_ratio1.5");
    editDetails()->page()->addKnob(1, 3, 1, 1, "10", hex1, hex2, "05", "normal_ratio1.25");
    //   knob "0B" at PS(1,4) → stackField(1,5)
    editDetails()->page()->addKnob(1, 5, 1, 1, "10", hex1, hex2, "0B", "normal_ratio1.5");
    //   formerly "VOICE 2" groupBox at addGroupBox(3, 0, 1, 3) — parentRow=3+0=3, parentCol=0+1=1
    editDetails()->page()->addComboBox(3, 1, 1, 3, "10", hex1, hex2, "0C", "large");
    editDetails()->page()->addDataKnob(4, 1, 1, 1, "10", hex1, hex2, "0E", "PREDELAY_ratio1.5","bottom", 70);
    editDetails()->page()->addKnob(4, 2, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.5");
    editDetails()->page()->addKnob(4, 3, 1, 1, "10", hex1, hex2, "0D", "normal_ratio1.25");
    // formerly "LEVEL" groupBox at addGroupBox(0, 5, 2, 1) — sequential col 6
    editDetails()->page()->addKnob(0, 6, 1, 1, "10", hex1, hex2, "0A", "normal_ratio1.75");
    editDetails()->page()->addKnob(2, 6, 1, 1, "10", hex1, hex2, "12", "normal_ratio1.75");
    editDetails()->page()->addKnob(1, 6, 1, 1, "10", hex1, hex2, "03", "normal_ratio1.5");
    editDetails()->page()->addStackField();


    // Reverb
    editDetails()->page()->newStackField(0);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex2 = "7C";
    }
    else
    {
        hex2 = "7F";
    };
    // formerly "EFFECT" groupBox at addGroupBox(0, 0, 1, 1) — parentCol=0
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, hex2, "01", "large");   //type
    editDetails()->page()->addComboBox(1, 0, 1, 1, "10", hex1, hex2, "03", "large");   //carry over
    // formerly "REVERB" groupBox at addGroupBox(0, 1, 1, 1) — parentCol=1
    editDetails()->page()->addDataKnob(0, 1, 1, 1, "10", hex1, hex2, "04", "0~500_ratio1.5");  //predelay
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.75");  //time
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "0B", "normal_ratio1.5");  //density
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "0D", "normal_ratio1.5");  //spring sense
    editDetails()->page()->addSystemOverride(0, 4, 1, 1, "10", hex1, hex2, "01", "05", "not_equal");
    // formerly "FILTER" groupBox at addGroupBox(0, 2, 1, 1) — sequential col 5
    editDetails()->page()->addKnob(0, 5, 1, 1, "10", hex1, hex2, "09", "normal_ratio1.5", "bottom", 60);  //low cut
    editDetails()->page()->addKnob(0, 6, 1, 1, "10", hex1, hex2, "0A", "normal_ratio1.5", "bottom", 60);  //high cut
    // formerly "LEVEL" groupBox at addGroupBox(0, 3, 1, 1) — sequential col 7
    editDetails()->page()->addKnob(0, 7, 1, 1, "10", hex1, hex2, "08", "normal_ratio1.75");  //effect
    editDetails()->page()->addKnob(0, 8, 1, 1, "10", hex1, hex2, "0C", "normal_ratio1.5");  //direct
    editDetails()->page()->addStackField();

    // Ring Modulator
    editDetails()->page()->newStackField(0);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex1 = "02";
        hex2 = "7D";
    }
    else
    {
        hex1 = "01";
        hex2 = "00";
    };
    // formerly "RING MODULATOR" groupBox at addGroupBox(0, 1, 1, 4) — parentCol=1
    editDetails()->page()->addComboBox(0, 1, 1, 1, "10", hex1, hex2, "00", "large");
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "01", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "03", "normal_ratio1.5");
    // formerly "LEVEL" groupBox at addGroupBox(0, 5, 1, 2) — parentCol=5
    editDetails()->page()->addKnob(0, 5, 1, 1, "10", hex1, hex2, "04", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 6, 1, 1, "10", hex1, hex2, "05", "normal_ratio1.5");
    editDetails()->page()->addStackField();


    // Rotary
    editDetails()->page()->newStackField(0);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex1 = "02";
        hex2 = "7E";
    }
    else
    {
        hex1 = "01";
        hex2 = "01";
    };
    // formerly "ROTARY" groupBox at addGroupBox(0, 1, 1, 6) — parentCol=1
    editDetails()->page()->addComboBox(0, 1, 1, 1, "10", hex1, hex2, "00", "large");
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "01", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "03", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 5, 1, 1, "10", hex1, hex2, "04", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 6, 1, 1, "10", hex1, hex2, "05", "normal_ratio1.5");
    // formerly "LEVEL" groupBox at addGroupBox(0, 7, 1, 2) — parentCol=7
    editDetails()->page()->addKnob(0, 7, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 8, 1, 1, "10", hex1, hex2, "07", "normal_ratio1.5");
    editDetails()->page()->addStackField();


    // Sitar Sim
    editDetails()->page()->newStackField(0);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex1 = "02";
        hex2 = "7F";
    }
    else
    {
        hex1 = "01";
        hex2 = "02";
    };
    // formerly "SITAR SIMULATOR" groupBox at addGroupBox(0, 1, 1, 5) — parentCol=1
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "00", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "01", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "04", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "05", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 5, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.5");
    // formerly "LEVEL" groupBox at addGroupBox(0, 6, 1, 2) — parentCol=6
    editDetails()->page()->addKnob(0, 6, 1, 1, "10", hex1, hex2, "03", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 7, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.5");
    editDetails()->page()->addStackField();


    // Slicer
    editDetails()->page()->newStackField(0);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex1 = "03";
        hex2 = "00";
    }
    else
    {
        hex1 = "01";
        hex2 = "03";
    };
    // formerly "SLICER" groupBox at addGroupBox(0, 1, 1, 5) — parentCol=1
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "00", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "01", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "05", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 5, 1, 1, "10", hex1, hex2, "04", "normal_ratio1.5");
    // formerly "LEVEL" groupBox at addGroupBox(0, 6, 1, 2) — parentCol=6
    editDetails()->page()->addKnob(0, 6, 1, 1, "10", hex1, hex2, "03", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 7, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.5");
    editDetails()->page()->addStackField();


    // Slow Gear
    editDetails()->page()->newStackField(0);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex1 = "03";
        hex2 = "01";
    }
    else
    {
        hex1 = "01";
        hex2 = "04";
    };
    // formerly "SLOW GEAR" groupBox at addGroupBox(0, 1, 1, 1) — parentCol=1
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "00", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "01", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "02", "normal_ratio2");
    editDetails()->page()->addStackField();


    // Slow Gear Bass
    editDetails()->page()->newStackField(0);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex1 = "03";
        hex2 = "02";
    }
    else
    {
        hex1 = "01";
        hex2 = "05";
    };
    // formerly "SLOW GEAR BASS" groupBox at addGroupBox(0, 1, 1, 1) — parentCol=1
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "00", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "01", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "02", "normal_ratio2");
    editDetails()->page()->addStackField();


    // Sound Hold
    editDetails()->page()->newStackField(0);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex1 = "03";
        hex2 = "03";
    }
    else
    {
        hex1 = "01";
        hex2 = "06";
    };
    // formerly "SOUND HOLD" groupBox at addGroupBox(0, 1, 1, 1) — parentCol=1
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "00", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "01", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "02", "normal_ratio2");
    editDetails()->page()->addStackField();


    // T-Wah
    editDetails()->page()->newStackField(0);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex1 = "03";
        hex2 = "04";
    }
    else
    {
        hex1 = "01";
        hex2 = "07";
    };
    // formerly "T-WAH" groupBox at addGroupBox(0, 1, 1, 6) — parentCol=1
    editDetails()->page()->addComboBox(0, 1, 1, 1, "10", hex1, hex2, "00", "large");
    editDetails()->page()->addComboBox(0, 2, 1, 1, "10", hex1, hex2, "01", "large");
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "03", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 5, 1, 1, "10", hex1, hex2, "04", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 6, 1, 1, "10", hex1, hex2, "05", "normal_ratio1.5");
    // formerly "LEVEL" groupBox at addGroupBox(0, 7, 1, 2) — parentCol=7
    editDetails()->page()->addKnob(0, 7, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 8, 1, 1, "10", hex1,hex2, "07", "normal_ratio1.5");
    editDetails()->page()->addStackField();


    // T-Wah Bass
    editDetails()->page()->newStackField(0);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex1 = "03";
        hex2 = "05";
    }
    else
    {
        hex1 = "01";
        hex2 = "08";
    };
    // formerly "T-WAH BASS" groupBox at addGroupBox(0, 1, 1, 6) — parentCol=1
    editDetails()->page()->addComboBox(0, 1, 1, 1, "10", hex1, hex2, "00", "large");
    editDetails()->page()->addComboBox(0, 2, 1, 1, "10", hex1, hex2, "01", "large");
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "03", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 5, 1, 1, "10", hex1, hex2, "04", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 6, 1, 1, "10", hex1, hex2, "05", "normal_ratio1.5");
    // formerly "LEVEL" groupBox at addGroupBox(0, 7, 1, 2) — parentCol=7
    editDetails()->page()->addKnob(0, 7, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 8, 1, 1, "10", hex1, hex2, "07", "normal_ratio1.5");
    editDetails()->page()->addStackField();



    // Tremolo
    editDetails()->page()->newStackField(0);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex1 = "03";
        hex2 = "06";
    }
    else
    {
        hex1 = "01";
        hex2 = "09";
    };
    // formerly "TREMOLO" groupBox at addGroupBox(0, 1, 1, 1) — parentCol=1
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "00", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "01", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.5");
    // formerly "LEVEL" groupBox at addGroupBox(0, 2, 1, 1) — sequential col 4
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "03", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 5, 1, 1, "10", hex1, hex2, "04", "normal_ratio1.5");
    editDetails()->page()->addStackField();


    // Vibrato
    editDetails()->page()->newStackField(0);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex1 = "03";
        hex2 = "07";
    }
    else
    {
        hex1 = "01";
        hex2 = "0A";
    };
    // formerly "VIBRATO" groupBox at addGroupBox(0, 1, 1, 1) — parentCol=1
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "00", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "01", "normal_ratio1.5");
    editDetails()->page()->addComboBox(0, 3, 1, 1, "10", hex1, hex2, "02", "large");
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "03", "normal_ratio1.5");
    // formerly "LEVEL" groupBox at addGroupBox(0, 2, 1, 1) — sequential col 5
    editDetails()->page()->addKnob(0, 5, 1, 1, "10", hex1, hex2, "04", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 6, 1, 1, "10", hex1, hex2, "05", "normal_ratio1.5");
    editDetails()->page()->addStackField();


    // Wah
    editDetails()->page()->newStackField(0);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex1 = "03";
        hex2 = "08";
    }
    else
    {
        hex1 = "01";
        hex2 = "0B";
    };
    // formerly "WAH" groupBox at addGroupBox(0, 1, 1, 1) — parentCol=1
    editDetails()->page()->addComboBox(0, 1, 1, 1, "10", hex1, hex2, "00", "large");
    editDetails()->page()->addDataKnob(0, 2, 1, 1, "10", hex1, hex2, "02", "0~1000_ratio1.5","bottom", 70);
    editDetails()->page()->addDataKnob(0, 3, 1, 1, "10", hex1, hex2, "06", "0~1000_ratio1.75","bottom", 70);
    editDetails()->page()->addDataKnob(0, 4, 1, 1, "10", hex1, hex2, "0A", "0~1000_ratio1.75","bottom", 70);
    // formerly "LEVEL" groupBox at addGroupBox(0, 2, 1, 1) — sequential col 5
    editDetails()->page()->addKnob(0, 5, 1, 1, "10", hex1, hex2, "01", "normal_ratio2");
    editDetails()->page()->addKnob(0, 6, 1, 1, "10", hex1, hex2, "0E", "normal_ratio1.75");
    editDetails()->page()->addStackField();


    editDetails()->addPage();

}
