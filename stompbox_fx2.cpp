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

    editDetails()->page()->newGroupBox("Select");
    editDetails()->page()->addSwitch(0, 0, 1, 1, "10", hex1, hex2, "00");
    editDetails()->page()->newStackControl(0);
    editDetails()->page()->addComboBox(1, 0, 1, 1, "10", hex1, hex2, "01", "large");  //type
    editDetails()->page()->addStackControl();
    editDetails()->page()->addGroupBox(0, 0, 2, 1);
    editDetails()->page()->insertStackField(0, 0, 1, 3, 2);

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
    editDetails()->page()->newGroupBox("AC RESONANCE");
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, hex2, "00", "large");
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "01", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "03", "normal_ratio2");
    editDetails()->page()->addGroupBox(0, 1, 1, 1);
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
    editDetails()->page()->newGroupBox("AUTO WAH");
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, hex2, "00", "large");
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "01", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "04", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "05", "normal_ratio1.75");
    editDetails()->page()->addComboBox(0, 5, 1, 1, "10", hex1, hex2, "06", "large");
    editDetails()->page()->addGroupBox(0, 1, 1, 1);
    editDetails()->page()->newGroupBox("LEVEL");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "03", "normal_ratio2");
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "07", "normal_ratio1.75");
    editDetails()->page()->addGroupBox(1, 1, 1, 1);
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
    editDetails()->page()->newGroupBox(tr("EFFECT"));
    editDetails()->page()->newStackControl(1);
    editDetails()->page()->addComboBox(1, 0, 1, 1, "10", hex1, hex2, "01", "large");           //mode
    editDetails()->page()->addStackControl();
    editDetails()->page()->addGroupBox(0, 0, 1, 1);
    editDetails()->page()->insertStackField(1, 1, 0, 1, 2);
    editDetails()->page()->newGroupBox(tr("LEVEL"));
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "17", "normal_ratio1.5");     //direct
    editDetails()->page()->addComboBox(0, 1, 1, 1, "10", hex1, hex2, "18", "large");           //output
    editDetails()->page()->addGroupBox(0, 1, 1, 1);
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(1);
    editDetails()->page()->newGroupBox(tr("CHORUS"));
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.5", "bottom", 60); //rate
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "03", "normal_ratio1.5", "bottom", 60); //depth
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "04", "normal_ratio1.5", "bottom", 60); //predelay
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "05", "normal_ratio1.75", "bottom", 60); //effect
    editDetails()->page()->addGroupBox(0, 0, 1, 1);
    editDetails()->page()->newGroupBox(tr("FILTER"));
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, hex2, "06"); //waveform
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "07", "normal_ratio1.25", "bottom", 60);
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "08", "normal_ratio1.25", "bottom", 60);
    editDetails()->page()->addGroupBox(0, 1, 1, 1);
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(1);
    editDetails()->page()->newGroupBox(tr("CHORUS"));
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.5", "bottom", 60); //rate
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "03", "normal_ratio1.5", "bottom", 60); //depth
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "04", "normal_ratio1.5", "bottom", 60); //predelay
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "05", "normal_ratio1.75", "bottom", 60); //effect
    editDetails()->page()->addGroupBox(0, 0, 1, 1);
    editDetails()->page()->newGroupBox(tr("FILTER"));
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, hex2, "06"); //waveform
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "07", "normal_ratio1.25", "bottom", 60);
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "08", "normal_ratio1.25", "bottom", 60);
    editDetails()->page()->addGroupBox(0, 1, 1, 1);
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(1);
    editDetails()->page()->newGroupBox(tr("CHORUS"));
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.5", "bottom", 60); //rate
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "03", "normal_ratio1.5", "bottom", 60); //depth
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "04", "normal_ratio1.5", "bottom", 60); //predelay
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "05", "normal_ratio1.75", "bottom", 60); //effect
    editDetails()->page()->addGroupBox(0, 0, 1, 1);
    editDetails()->page()->newGroupBox(tr("FILTER"));
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, hex2, "06"); //waveform
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "07", "normal_ratio1.25", "bottom", 60);
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "08", "normal_ratio1.25", "bottom", 60);
    editDetails()->page()->addGroupBox(0, 1, 1, 1);
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(1);
    editDetails()->page()->newGroupBox(tr("CHORUS 1"));
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "09", "normal_ratio1.5", "bottom", 60); //rate
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "0A", "normal_ratio1.5", "bottom", 60); //depth
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "0B", "normal_ratio1.5", "bottom", 60); //predelay
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "0C", "normal_ratio1.75", "bottom", 60); //effect
    editDetails()->page()->addGroupBox(0, 0, 1, 1);
    editDetails()->page()->newGroupBox(tr("FILTER 1"));
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, hex2, "0D"); //waveform
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "0E", "normal_ratio1.25", "bottom", 60);
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "0F", "normal_ratio1.25", "bottom", 60);
    editDetails()->page()->addGroupBox(0, 1, 1, 1);
    editDetails()->page()->newGroupBox(tr("CHORUS 2"));
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "10", "normal_ratio1.5", "bottom", 60); //rate
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "11", "normal_ratio1.5", "bottom", 60); //depth
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "12", "normal_ratio1.5", "bottom", 60); //predelay
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "13", "normal_ratio1.75", "bottom", 60); //effect
    editDetails()->page()->addGroupBox(1, 0, 1, 1);
    editDetails()->page()->newGroupBox(tr("FILTER 2"));
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, hex2, "14"); //waveform
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "15", "normal_ratio1.25", "bottom", 60);
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "16", "normal_ratio1.25", "bottom", 60);
    editDetails()->page()->addGroupBox(1, 1, 1, 1);
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
    editDetails()->page()->newGroupBox("CLASSIC VIBE");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "00", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "01", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "02", "normal_ratio2");
    editDetails()->page()->addGroupBox(0, 1, 1, 1);
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
    editDetails()->page()->newGroupBox("COMPRESSOR");
    editDetails()->page()->addComboBox(0, 0, 1, 2, "10", hex1, hex2, "01", "large");
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "03", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "04", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 5, 1, 1, "10", hex1, hex2, "05", "normal_ratio2");
    editDetails()->page()->addKnob(0, 6, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.75");
    editDetails()->page()->addGroupBox(0, 1, 1, 1);
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
    editDetails()->page()->newGroupBox("DEFRETTER");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "00", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "01", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "03", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "04", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 5, 1, 1, "10", hex1, hex2, "05", "normal_ratio2");
    editDetails()->page()->addKnob(0, 6, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.75");
    editDetails()->page()->addGroupBox(0, 1, 1, 1);
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
    editDetails()->page()->newGroupBox("DEFRETTER BASS");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "00", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "01", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "03", "normal_ratio2");
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "04", "normal_ratio1.75");
    editDetails()->page()->addGroupBox(0, 1, 1, 1);
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
    editDetails()->page()->newGroupBox("EFFECT");
    editDetails()->page()->newStackControl(2);
    editDetails()->page()->addComboBox(1, 0, 1, 1, "10", hex1, hex2, "01", "large");
    editDetails()->page()->addStackControl();
    editDetails()->page()->addGroupBox(0, 0, 2, 1);

    editDetails()->page()->insertStackField(2, 0, 1, 2, 1);
    editDetails()->page()->addStackField();

    // STEREO 1
    editDetails()->page()->newStackField(2);
    editDetails()->page()->newGroupBox("DELAY");
    editDetails()->page()->addDataKnob(0, 0, 1, 1, "10", hex1, hex2, "02", "DELAY2000_ratio1.75","bottom", 60); //4 byte time
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.25"); //feedback
    editDetails()->page()->addGroupBox(0, 0, 4, 1);
    editDetails()->page()->newGroupBox("FILTER");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "07", "normal_ratio1.25", "bottom", 60); //high cut
    editDetails()->page()->addGroupBox(0, 1, 4, 1);
    editDetails()->page()->newGroupBox("LEVEL");
    editDetails()->page()->addKnob(0, 0, 1, 2, "10", hex1, hex2, "08", "turbo_ratio1.75"); //EFFECT
    editDetails()->page()->addKnob(1, 0, 1, 1, "10", hex1, hex2, "09", "normal_ratio1.5"); // direct
    editDetails()->page()->addKnob(1, 1, 1, 1, "10", hex1, hex2, "20", "normal_ratio1.25"); //carry over
    editDetails()->page()->addGroupBox(0, 2, 4, 1);
    editDetails()->page()->addStackField();

    // STEREO 2
    editDetails()->page()->newStackField(2);
    editDetails()->page()->newGroupBox("DELAY");
    editDetails()->page()->addDataKnob(0, 0, 1, 1, "10", hex1, hex2, "02", "DELAY2000_ratio1.75","bottom", 60); //4 byte time
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.25"); //feedback
    editDetails()->page()->addGroupBox(0, 0, 4, 1);
    editDetails()->page()->newGroupBox("FILTER");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "07", "normal_ratio1.25", "bottom", 60); //high cut
    editDetails()->page()->addGroupBox(0, 1, 4, 1);
    editDetails()->page()->newGroupBox("LEVEL");
    editDetails()->page()->addKnob(0, 0, 1, 2, "10", hex1, hex2, "08", "turbo_ratio1.75"); //EFFECT
    editDetails()->page()->addKnob(1, 0, 1, 1, "10", hex1, hex2, "09", "normal_ratio1.5"); // direct
    editDetails()->page()->addKnob(1, 1, 1, 1, "10", hex1, hex2, "20", "normal_ratio1.25"); //carry over
    editDetails()->page()->addGroupBox(0, 2, 4, 1);
    editDetails()->page()->addStackField();

    // PAN
    editDetails()->page()->newStackField(2);
    editDetails()->page()->newGroupBox("DELAY");
    editDetails()->page()->addDataKnob(0, 0, 1, 1, "10", hex1, hex2, "02", "DELAY2000_ratio1.75","bottom", 60); //4 byte time
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.25"); //feedback
    editDetails()->page()->addKnob(1, 0, 1, 2, "10", hex1, hex2, "1F", "normal_ratio1.25"); //tap time %
    editDetails()->page()->addGroupBox(0, 0, 4, 1);
    editDetails()->page()->newGroupBox("FILTER");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "07", "normal_ratio1.25", "bottom", 60); //high cut
    editDetails()->page()->addGroupBox(0, 1, 4, 1);
    editDetails()->page()->newGroupBox("LEVEL");
    editDetails()->page()->addKnob(0, 0, 1, 2, "10", hex1, hex2, "08", "turbo_ratio1.75"); //EFFECT
    editDetails()->page()->addKnob(1, 0, 1, 1, "10", hex1, hex2, "09", "normal_ratio1.5"); // direct
    editDetails()->page()->addKnob(1, 1, 1, 1, "10", hex1, hex2, "20", "normal_ratio1.25"); //carry over
    editDetails()->page()->addGroupBox(0, 2, 4, 1);
    editDetails()->page()->addStackField();

    // DUAL SERIES
    editDetails()->page()->newStackField(2);
    editDetails()->page()->newGroupBox("DELAY 1");
    editDetails()->page()->addDataKnob(0, 0, 1, 1, "10", hex1, hex2, "0A", "DELAY2000_ratio1.25","bottom", 60); //4 byte time
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.25"); //feedback
    editDetails()->page()->addGroupBox(0, 0, 1, 1);
    editDetails()->page()->newGroupBox("DELAY 2");
    editDetails()->page()->addDataKnob(0, 0, 1, 1, "10", hex1, hex2, "11", "DELAY2000_ratio1.25","bottom", 60); //4 byte time
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.25"); //feedback
    editDetails()->page()->addGroupBox(1, 0, 1, 1);
    editDetails()->page()->newGroupBox("FILTER 1");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "0F", "normal_ratio1.25", "bottom", 60);
    editDetails()->page()->addGroupBox(0, 1, 1, 1);
    editDetails()->page()->newGroupBox("FILTER 2");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "16", "normal_ratio1.25", "bottom", 60);
    editDetails()->page()->addGroupBox(1, 1, 1, 1);
    editDetails()->page()->newGroupBox("LEVEL 1");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "10", "normal_ratio1.75");
    editDetails()->page()->addGroupBox(0, 2, 1, 1);
    editDetails()->page()->newGroupBox("LEVEL 2");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "17", "normal_ratio1.75");
    editDetails()->page()->addGroupBox(1, 2, 1, 1);
    editDetails()->page()->newGroupBox("Direct");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "09", "normal_ratio1.5");
    editDetails()->page()->addKnob(1, 0, 1, 1, "10", hex1, hex2, "20", "normal_ratio1.25"); //carry over
    editDetails()->page()->addGroupBox(0, 3, 2, 1);
    editDetails()->page()->addStackField();

    // DUAL PARALLEL
    editDetails()->page()->newStackField(2);
    editDetails()->page()->newGroupBox("DELAY 1");
    editDetails()->page()->addDataKnob(0, 0, 1, 1, "10", hex1, hex2, "0A", "DELAY2000_ratio1.25","bottom", 60); //4 byte time
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.25"); //feedback
    editDetails()->page()->addGroupBox(0, 0, 1, 1);
    editDetails()->page()->newGroupBox("DELAY 2");
    editDetails()->page()->addDataKnob(0, 0, 1, 1, "10", hex1, hex2, "11", "DELAY2000_ratio1.25","bottom", 60); //4 byte time
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.25"); //feedback
    editDetails()->page()->addGroupBox(1, 0, 1, 1);
    editDetails()->page()->newGroupBox("FILTER 1");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "0F", "normal_ratio1.25", "bottom", 60);
    editDetails()->page()->addGroupBox(0, 1, 1, 1);
    editDetails()->page()->newGroupBox("FILTER 2");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "16", "normal_ratio1.25", "bottom", 60);
    editDetails()->page()->addGroupBox(1, 1, 1, 1);
    editDetails()->page()->newGroupBox("LEVEL 1");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "10", "normal_ratio1.75");
    editDetails()->page()->addGroupBox(0, 2, 1, 1);
    editDetails()->page()->newGroupBox("LEVEL 2");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "17", "normal_ratio1.75");
    editDetails()->page()->addGroupBox(1, 2, 1, 1);
    editDetails()->page()->newGroupBox("Direct");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "09", "normal_ratio1.5");
    editDetails()->page()->addKnob(1, 0, 1, 1, "10", hex1, hex2, "20", "normal_ratio1.25"); //carry over
    editDetails()->page()->addGroupBox(0, 3, 2, 1);
    editDetails()->page()->addStackField();

    // DUAL L/R
    editDetails()->page()->newStackField(2);
    editDetails()->page()->newGroupBox("DELAY 1");
    editDetails()->page()->addDataKnob(0, 0, 1, 1, "10", hex1, hex2, "0A", "DELAY2000_ratio1.25","bottom", 60); //4 byte time
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.25"); //feedback
    editDetails()->page()->addGroupBox(0, 0, 1, 1);
    editDetails()->page()->newGroupBox("DELAY 2");
    editDetails()->page()->addDataKnob(0, 0, 1, 1, "10", hex1, hex2, "11", "DELAY2000_ratio1.25","bottom", 60); //4 byte time
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.25"); //feedback
    editDetails()->page()->addGroupBox(1, 0, 1, 1);
    editDetails()->page()->newGroupBox("FILTER 1");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "0F", "normal_ratio1.25", "bottom", 60);
    editDetails()->page()->addGroupBox(0, 1, 1, 1);
    editDetails()->page()->newGroupBox("FILTER 2");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "16", "normal_ratio1.25", "bottom", 60);
    editDetails()->page()->addGroupBox(1, 1, 1, 1);
    editDetails()->page()->newGroupBox("LEVEL 1");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "10", "normal_ratio1.75");
    editDetails()->page()->addGroupBox(0, 2, 1, 1);
    editDetails()->page()->newGroupBox("LEVEL 2");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "17", "normal_ratio1.75");
    editDetails()->page()->addGroupBox(1, 2, 1, 1);
    editDetails()->page()->newGroupBox("Direct");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "09", "normal_ratio1.5");
    editDetails()->page()->addKnob(1, 0, 1, 1, "10", hex1, hex2, "20", "normal_ratio1.25"); //carry over
    editDetails()->page()->addGroupBox(0, 3, 2, 1);
    editDetails()->page()->addStackField();

    // REVERSE
    editDetails()->page()->newStackField(2);
    editDetails()->page()->newGroupBox("DELAY");
    editDetails()->page()->addDataKnob(0, 0, 1, 1, "10", hex1, hex2, "02", "DELAY2000_ratio1.75","bottom", 60); //4 byte time
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.25"); //feedback
    editDetails()->page()->addGroupBox(0, 0, 4, 1);
    editDetails()->page()->newGroupBox("FILTER");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "07", "normal_ratio1.25", "bottom", 60); //high cut
    editDetails()->page()->addGroupBox(0, 1, 4, 1);
    editDetails()->page()->newGroupBox("LEVEL");
    editDetails()->page()->addKnob(0, 0, 1, 2, "10", hex1, hex2, "08", "turbo_ratio1.75"); //EFFECT
    editDetails()->page()->addKnob(1, 0, 1, 1, "10", hex1, hex2, "09", "normal_ratio1.5"); // direct
    editDetails()->page()->addKnob(1, 1, 1, 1, "10", hex1, hex2, "20", "normal_ratio1.25"); //carry over
    editDetails()->page()->addGroupBox(0, 2, 4, 1);
    editDetails()->page()->addStackField();

    // ANALOG
    editDetails()->page()->newStackField(2);
    editDetails()->page()->newGroupBox("DELAY");
    editDetails()->page()->addDataKnob(0, 0, 1, 1, "10", hex1, hex2, "02", "DELAY2000_ratio1.75","bottom", 60); //4 byte time
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.25"); //feedback
    editDetails()->page()->addGroupBox(0, 0, 4, 1);
    editDetails()->page()->newGroupBox("FILTER");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "07", "normal_ratio1.25", "bottom", 60); //high cut
    editDetails()->page()->addGroupBox(0, 1, 4, 1);
    editDetails()->page()->newGroupBox("LEVEL");
    editDetails()->page()->addKnob(0, 0, 1, 2, "10", hex1, hex2, "08", "turbo_ratio1.75"); //EFFECT
    editDetails()->page()->addKnob(1, 0, 1, 1, "10", hex1, hex2, "09", "normal_ratio1.5"); // direct
    editDetails()->page()->addKnob(1, 1, 1, 1, "10", hex1, hex2, "20", "normal_ratio1.25"); //carry over
    editDetails()->page()->addGroupBox(0, 2, 4, 1);
    editDetails()->page()->addStackField();

    // TAPE
    editDetails()->page()->newStackField(2);
    editDetails()->page()->newGroupBox("DELAY");
    editDetails()->page()->addDataKnob(0, 0, 1, 1, "10", hex1, hex2, "02", "DELAY2000_ratio1.75","bottom", 60); //4 byte time
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.25"); //feedback
    editDetails()->page()->addGroupBox(0, 0, 4, 1);
    editDetails()->page()->newGroupBox("FILTER");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "07", "normal_ratio1.25", "bottom", 60); //high cut
    editDetails()->page()->addGroupBox(0, 1, 4, 1);
    editDetails()->page()->newGroupBox("LEVEL");
    editDetails()->page()->addKnob(0, 0, 1, 2, "10", hex1, hex2, "08", "turbo_ratio1.75"); //EFFECT
    editDetails()->page()->addKnob(1, 0, 1, 1, "10", hex1, hex2, "09", "normal_ratio1.5"); // direct
    editDetails()->page()->addKnob(1, 1, 1, 1, "10", hex1, hex2, "20", "normal_ratio1.25"); //carry over
    editDetails()->page()->addGroupBox(0, 2, 4, 1);
    editDetails()->page()->addStackField();

    // MODULATE
    editDetails()->page()->newStackField(2);
    editDetails()->page()->newGroupBox("DELAY");
    editDetails()->page()->addDataKnob(0, 0, 1, 1, "10", hex1, hex2, "02", "DELAY2000_ratio1.75","bottom", 60); //4 byte time
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.25"); //feedback
    editDetails()->page()->addGroupBox(0, 0, 1, 1);
    editDetails()->page()->newGroupBox("MODULATE");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "18", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "19", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "1A", "normal_ratio1.25");
    editDetails()->page()->addGroupBox(1, 0, 1, 4);
    editDetails()->page()->newGroupBox("FILTER");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "07", "normal_ratio1.25", "bottom", 60); //high cut
    editDetails()->page()->addGroupBox(0, 1, 1, 1);
    editDetails()->page()->newGroupBox("LEVEL");
    editDetails()->page()->addKnob(0, 0, 1, 2, "10", hex1, hex2, "08", "turbo_ratio1.75"); //EFFECT
    editDetails()->page()->addKnob(1, 0, 1, 1, "10", hex1, hex2, "09", "normal_ratio1.5"); // direct
    editDetails()->page()->addKnob(1, 1, 1, 1, "10", hex1, hex2, "20", "normal_ratio1.25"); //carry over
    editDetails()->page()->addGroupBox(0, 2, 1, 1);
    editDetails()->page()->addStackField();

    // WARP
    editDetails()->page()->newStackField(2);
    editDetails()->page()->newGroupBox("DELAY");
    editDetails()->page()->addDataKnob(0, 0, 1, 1, "10", hex1, hex2, "02", "DELAY2000_ratio1.75","bottom", 60); //4 byte time
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.25"); //feedback
    editDetails()->page()->addGroupBox(0, 0, 4, 1);
    editDetails()->page()->newGroupBox("FILTER");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "07", "normal_ratio1.25", "bottom", 60); //high cut
    editDetails()->page()->addGroupBox(0, 1, 4, 1);
    editDetails()->page()->newGroupBox("LEVEL");
    editDetails()->page()->addKnob(0, 0, 1, 2, "10", hex1, hex2, "08", "turbo_ratio1.75"); //EFFECT
    editDetails()->page()->addKnob(1, 0, 1, 1, "10", hex1, hex2, "09", "normal_ratio1.5"); // direct
    editDetails()->page()->addKnob(1, 1, 1, 1, "10", hex1, hex2, "20", "normal_ratio1.25"); //carry over
    editDetails()->page()->addGroupBox(0, 2, 4, 1);
    editDetails()->page()->addStackField();

    // TWIST
    editDetails()->page()->newStackField(2);
    editDetails()->page()->newGroupBox("DELAY");
    editDetails()->page()->addDataKnob(0, 0, 1, 1, "10", hex1, hex2, "02", "DELAY2000_ratio1.75","bottom", 60); //4 byte time
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.25"); //feedback
    editDetails()->page()->addGroupBox(0, 0, 1, 2);
    editDetails()->page()->newGroupBox("TWIST");
    editDetails()->page()->newStackControl(3);
    editDetails()->page()->addComboBox(0, 0, 1, 2, "10", hex1, hex2, "1B", "large");
    editDetails()->page()->addStackControl();
    editDetails()->page()->insertStackField(3, 0, 2, 1, 2);
    editDetails()->page()->addGroupBox(1, 0, 1, 5);
    editDetails()->page()->newGroupBox("FILTER");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "07", "normal_ratio1.25", "bottom", 60); //high cut
    editDetails()->page()->addGroupBox(0, 2, 1, 1);
    editDetails()->page()->newGroupBox("LEVEL");
    editDetails()->page()->addKnob(0, 0, 1, 2, "10", hex1, hex2, "08", "turbo_ratio1.75"); //EFFECT
    editDetails()->page()->addKnob(1, 0, 1, 1, "10", hex1, hex2, "09", "normal_ratio1.5"); // direct
    editDetails()->page()->addKnob(1, 1, 1, 1, "10", hex1, hex2, "20", "normal_ratio1.25"); //carry over
    editDetails()->page()->addGroupBox(0, 3, 1, 2);
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
    editDetails()->page()->newGroupBox("FLANGER");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "00", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "01", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "03", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "04", "normal_ratio1.5");
    editDetails()->page()->addGroupBox(0, 1, 1, 2);
    editDetails()->page()->newGroupBox("FILTER");
    editDetails()->page()->addKnob(0, 5, 1, 1, "10", hex1, hex2, "05", "normal_ratio1.5", "bottom", 60);
    editDetails()->page()->addKnob(0, 6, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.5", "bottom", 60);
    editDetails()->page()->addGroupBox(1, 1, 1, 1);
    editDetails()->page()->newGroupBox("LEVEL");
    editDetails()->page()->addKnob(0, 7, 1, 1, "10", hex1, hex2, "07", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 8, 1, 1, "10", hex1, hex2, "08", "normal_ratio1.5");
    editDetails()->page()->addGroupBox(1, 2, 1, 1);
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
    editDetails()->page()->newGroupBox("FLANGER BASS");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "00", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "01", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "03", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "04", "normal_ratio1.5");
    editDetails()->page()->addGroupBox(0, 1, 1, 2);
    editDetails()->page()->newGroupBox("FILTER");
    editDetails()->page()->addKnob(0, 5, 1, 1, "10", hex1, hex2, "05", "normal_ratio1.5", "bottom", 60);
    editDetails()->page()->addKnob(0, 6, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.5", "bottom", 60);
    editDetails()->page()->addGroupBox(1, 1, 1, 1);
    editDetails()->page()->newGroupBox("LEVEL");
    editDetails()->page()->addKnob(0, 7, 1, 1, "10", hex1, hex2, "07", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 8, 1, 1, "10", hex1, hex2, "08", "normal_ratio1.5");
    editDetails()->page()->addGroupBox(1, 2, 1, 1);
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
    editDetails()->page()->newGroupBox("FOOT VOLUME");
    editDetails()->page()->addDataKnob(0, 0, 1, 1, "10", hex1, hex2, "00", "0~1000_ratio2");
    editDetails()->page()->addDataKnob(0, 1, 1, 1, "10", hex1, hex2, "04", "0~1000_ratio2");
    editDetails()->page()->addDataKnob(0, 2, 1, 1, "10", hex1, hex2, "08", "0~1000_ratio1.75");
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "0C", "normal_ratio1.5", "bottom", 60);
    editDetails()->page()->addGroupBox(0, 0, 1, 1);
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
    editDetails()->page()->newGroupBox("EQUALIZER");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "00", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "01", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "03", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "04", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 5, 1, 1, "10", hex1, hex2, "05", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 6, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.5");
    editDetails()->page()->addGroupBox(0, 2, 1, 7);
    editDetails()->page()->newGroupBox("LEVEL");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "07", "normal_ratio1.5");
    editDetails()->page()->addGroupBox(0, 8, 1, 1);
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
    editDetails()->page()->newGroupBox("Effect");
    editDetails()->page()->newStackControl(4);
    editDetails()->page()->addComboBox(1, 0, 1, 1, "10", hex1, hex2, "00", "large");
    editDetails()->page()->addStackControl();
    editDetails()->page()->addGroupBox(0, 0, 2, 3);
    editDetails()->page()->insertStackField(4, 0, 3, 3, 9);
    editDetails()->page()->newGroupBox("User Harmony 1");
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, hex2, "0F");
    editDetails()->page()->addComboBox(0, 1, 1, 1, "10", hex1, hex2, "10");
    editDetails()->page()->addComboBox(0, 2, 1, 1, "10", hex1, hex2, "11");
    editDetails()->page()->addComboBox(0, 3, 1, 1, "10", hex1, hex2, "12");
    editDetails()->page()->addComboBox(0, 4, 1, 1, "10", hex1, hex2, "13");
    editDetails()->page()->addComboBox(0, 5, 1, 1, "10", hex1, hex2, "14");
    editDetails()->page()->addComboBox(0, 6, 1, 1, "10", hex1, hex2, "15");
    editDetails()->page()->addComboBox(0, 7, 1, 1, "10", hex1, hex2, "16");
    editDetails()->page()->addComboBox(0, 8, 1, 1, "10", hex1, hex2, "17");
    editDetails()->page()->addComboBox(0, 9, 1, 1, "10", hex1, hex2, "18");
    editDetails()->page()->addComboBox(0, 10, 1, 1, "10", hex1, hex2, "19");
    editDetails()->page()->addComboBox(0, 11, 1, 1, "10", hex1, hex2, "1A");
    editDetails()->page()->addSystemOverride(0, 0, 1, 12, "10", hex1, hex2, "01", "1D", "not_equal"); //
    editDetails()->page()->addGroupBox(3, 0, 1, 12);
    editDetails()->page()->newGroupBox("User Harmony 2");
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, hex2, "1B");
    editDetails()->page()->addComboBox(0, 1, 1, 1, "10", hex1, hex2, "1C");
    editDetails()->page()->addComboBox(0, 2, 1, 1, "10", hex1, hex2, "1D");
    editDetails()->page()->addComboBox(0, 3, 1, 1, "10", hex1, hex2, "1E");
    editDetails()->page()->addComboBox(0, 4, 1, 1, "10", hex1, hex2, "1F");
    editDetails()->page()->addComboBox(0, 5, 1, 1, "10", hex1, hex2, "20");
    editDetails()->page()->addComboBox(0, 6, 1, 1, "10", hex1, hex2, "21");
    editDetails()->page()->addComboBox(0, 7, 1, 1, "10", hex1, hex2, "22");
    editDetails()->page()->addComboBox(0, 8, 1, 1, "10", hex1, hex2, "23");
    editDetails()->page()->addComboBox(0, 9, 1, 1, "10", hex1, hex2, "24");
    editDetails()->page()->addComboBox(0, 10, 1, 1, "10", hex1, hex2, "25");
    editDetails()->page()->addComboBox(0, 11, 1, 1, "10", hex1, hex2, "26");
    editDetails()->page()->addSystemOverride(0, 0, 1, 12, "10", hex1, hex2, "02", "1D", "not_equal"); //
    editDetails()->page()->addGroupBox(4, 0, 1, 12);
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(4);
    editDetails()->page()->newGroupBox("Harmonist");
    editDetails()->page()->newGroupBox("Voice 1");
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, hex2, "01", "large");
    editDetails()->page()->addDataKnob(0, 1, 1, 1, "10", hex1, hex2, "04", "PREDELAY_ratio1.25","bottom", 70);
    editDetails()->page()->addGroupBox(0, 0, 2, 1);
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "08", "normal_ratio1.25");
    editDetails()->page()->addGroupBox(0, 1, 2, 2);

    editDetails()->page()->newGroupBox("Level");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "03", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "09", "normal_ratio1.5");
    editDetails()->page()->addGroupBox(0, 3, 2, 1);
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(4);
    editDetails()->page()->newGroupBox("Harmonist");
    editDetails()->page()->newGroupBox("Voice 1");
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, hex2, "01", "large");
    editDetails()->page()->addDataKnob(0, 1, 1, 1, "10", hex1, hex2, "04", "PREDELAY_ratio1.25");
    editDetails()->page()->addGroupBox(0, 0, 1, 1);
    editDetails()->page()->newGroupBox("Voice 2");
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, hex2, "02", "large");
    editDetails()->page()->addDataKnob(0, 1, 1, 1, "10", hex1, hex2, "0B", "PREDELAY_ratio1.25");
    editDetails()->page()->addGroupBox(2, 0, 1, 1);
    editDetails()->page()->addKnob(0, 1, 2, 1, "10", hex1, hex2, "08", "normal_ratio1.5");
    editDetails()->page()->addGroupBox(0, 1, 2, 2);

    editDetails()->page()->newGroupBox("Level");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "03", "normal_ratio1.5");
    editDetails()->page()->addKnob(1, 0, 1, 1, "10", hex1, hex2, "0A", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 1, 2, 1, "10", hex1, hex2, "09", "normal_ratio1.25");
    editDetails()->page()->addGroupBox(0, 3, 2, 1);
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(4);
    editDetails()->page()->newGroupBox("Harmonist");
    editDetails()->page()->newGroupBox("Voice 1");
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, hex2, "01", "large");
    editDetails()->page()->addDataKnob(0, 1, 1, 1, "10", hex1, hex2, "04", "PREDELAY_ratio1.25");
    editDetails()->page()->addGroupBox(0, 0, 1, 1);
    editDetails()->page()->newGroupBox("Voice 2");
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, hex2, "02", "large");
    editDetails()->page()->addDataKnob(0, 1, 1, 1, "10", hex1, hex2, "0B", "PREDELAY_ratio1.25");
    editDetails()->page()->addGroupBox(2, 0, 1, 1);
    editDetails()->page()->addKnob(0, 1, 2, 1, "10", hex1, hex2, "08", "normal_ratio1.5");
    editDetails()->page()->addGroupBox(0, 1, 2, 2);

    editDetails()->page()->newGroupBox("Level");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "03", "normal_ratio1.5");
    editDetails()->page()->addKnob(1, 0, 1, 1, "10", hex1, hex2, "0A", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 1, 2, 1, "10", hex1, hex2, "09", "normal_ratio1.25");
    editDetails()->page()->addGroupBox(0, 3, 2, 1);
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
    editDetails()->page()->newGroupBox("HUMANIZER");
    editDetails()->page()->addComboBox(0, 0, 1, 2, "10", hex1, hex2, "00", "large");
    editDetails()->page()->addComboBox(0, 2, 1, 1, "10", hex1, hex2, "01", "large");
    editDetails()->page()->addComboBox(1, 2, 1, 1, "10", hex1, hex2, "02", "large");
    editDetails()->page()->addKnob(0, 4, 2, 1, "10", hex1, hex2, "03", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 5, 2, 1, "10", hex1, hex2, "04", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 6, 2, 1, "10", hex1, hex2, "05", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 7, 2, 1, "10", hex1, hex2, "06", "normal_ratio1.5");
    editDetails()->page()->addGroupBox(0, 2, 1, 8);
    editDetails()->page()->newGroupBox("LEVEL");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "07", "normal_ratio1.75");
    editDetails()->page()->addGroupBox(0, 9, 1, 1);
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
    editDetails()->page()->newGroupBox("ISOLATOR");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "00", "normal_ratio1.5", "bottom", 60);
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "01", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "03", "normal_ratio1.75");
    editDetails()->page()->addGroupBox(0, 1, 1, 1);
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
    // Limiter
    editDetails()->page()->newGroupBox("LIMITER");
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, hex2, "00", "large");
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "01", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "04", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "05", "normal_ratio1.5");
    editDetails()->page()->addGroupBox(0, 1, 1, 1);
    editDetails()->page()->newGroupBox("LEVEL");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "03", "normal_ratio1.75");
    editDetails()->page()->addGroupBox(0, 2, 1, 1);
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
    editDetails()->page()->newGroupBox("LOFI");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "00", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "01", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "02", "normal_ratio2","bottom", 70);
    editDetails()->page()->addGroupBox(0, 1, 1, 1);
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
    editDetails()->page()->newGroupBox("OCTAVE");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "00", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "01", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.75");
    editDetails()->page()->addGroupBox(0, 1, 1, 1);
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
    editDetails()->page()->newGroupBox("OCTAVE BASS");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "00", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "01", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.75");
    editDetails()->page()->addGroupBox(0, 1, 1, 1);
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
    editDetails()->page()->newGroupBox("AUTO PAN");
    editDetails()->page()->addComboBox(0, 0, 1, 2, "10", hex1, hex2, "00", "large");
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "01", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "03", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 5, 1, 1, "10", hex1, hex2, "05", "normal_ratio1.75");
    editDetails()->page()->addGroupBox(0, 1, 1, 6);
    editDetails()->page()->newGroupBox("LEVEL");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "04", "normal_ratio1.75");
    editDetails()->page()->addGroupBox(0, 7, 1, 1);
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
    editDetails()->page()->newGroupBox("PARAMETRIC EQUALIZER", Qt::AlignTop | Qt::AlignHCenter);
    editDetails()->page()->addParaEQ(0, 1, 2, 4, "10", hex1, hex2, "01");
    editDetails()->page()->addGroupBox(0, 0, 1, 1);
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
    editDetails()->page()->newGroupBox("PEDAL BEND");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "00", "normal_ratio1.75");
    editDetails()->page()->addDataKnob(0, 2, 1, 1, "10", hex1, hex2, "02", "0~1000_ratio1.75","bottom", 70);
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "01", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.75");
    editDetails()->page()->addGroupBox(0, 1, 1, 1);
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
    editDetails()->page()->newGroupBox("PHASER");
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, hex2, "00", "large");
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "01", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "03", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "04", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 5, 1, 1, "10", hex1, hex2, "05", "normal_ratio1.75");
    editDetails()->page()->addGroupBox(0, 1, 1, 1);
    editDetails()->page()->newGroupBox("LEVEL");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "07", "normal_ratio1.75");
    editDetails()->page()->addGroupBox(1, 1, 1, 1);
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
    editDetails()->page()->newGroupBox("Effect");
    editDetails()->page()->newStackControl(5);
    editDetails()->page()->addComboBox(1, 0, 1, 1, "10", hex1, hex2, "00", "large");
    editDetails()->page()->addStackControl();
    editDetails()->page()->addGroupBox(0, 0, 2, 1);
    editDetails()->page()->insertStackField(5, 0, 1, 3, 1);
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(5);
    editDetails()->page()->newGroupBox("PITCH SHIFTER");
    editDetails()->page()->newGroupBox("VOICE 1");
    editDetails()->page()->addComboBox(0, 0, 1, 3, "10", hex1, hex2, "04", "large");
    editDetails()->page()->addDataKnob(1, 0, 1, 1, "10", hex1, hex2, "06", "PREDELAY_ratio1.5","bottom", 70);
    editDetails()->page()->addKnob(1, 1, 1, 1, "10", hex1, hex2, "01", "normal_ratio1.5");
    editDetails()->page()->addKnob(1, 2, 1, 1, "10", hex1, hex2, "05", "normal_ratio1.25");
    editDetails()->page()->addGroupBox(0, 0, 3, 3);
    editDetails()->page()->addKnob(1, 4, 1, 1, "10", hex1, hex2, "0B", "normal_ratio1.5");
    editDetails()->page()->addGroupBox(0, 1, 2, 4);
    editDetails()->page()->newGroupBox("LEVEL");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "0A", "normal_ratio1.5");
    editDetails()->page()->addKnob(1, 0, 1, 1, "10", hex1, hex2, "03", "normal_ratio1.25");
    editDetails()->page()->addGroupBox(0, 5, 2, 1);
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(5);
    editDetails()->page()->newGroupBox("PITCH SHIFTER");
    editDetails()->page()->newGroupBox("VOICE 1");
    editDetails()->page()->addComboBox(0, 0, 1, 3, "10", hex1, hex2, "04", "large");
    editDetails()->page()->addDataKnob(1, 0, 1, 1, "10", hex1, hex2, "06", "PREDELAY_ratio1.5","bottom", 70);
    editDetails()->page()->addKnob(1, 1, 1, 1, "10", hex1, hex2, "01", "normal_ratio1.5");
    editDetails()->page()->addKnob(1, 2, 1, 1, "10", hex1, hex2, "05", "normal_ratio1.25");
    editDetails()->page()->addGroupBox(0, 0, 3, 3);
    editDetails()->page()->addKnob(1, 4, 1, 1, "10", hex1, hex2, "0B", "normal_ratio1.5");
    editDetails()->page()->newGroupBox("VOICE 2");
    editDetails()->page()->addComboBox(0, 0, 1, 3, "10", hex1, hex2, "0C", "large");
    editDetails()->page()->addDataKnob(1, 0, 1, 1, "10", hex1, hex2, "0E", "PREDELAY_ratio1.5","bottom", 70);
    editDetails()->page()->addKnob(1, 1, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.5");
    editDetails()->page()->addKnob(1, 2, 1, 1, "10", hex1, hex2, "0D", "normal_ratio1.25");
    editDetails()->page()->addGroupBox(3, 0, 1, 3);
    editDetails()->page()->addGroupBox(0, 1, 2, 4);
    editDetails()->page()->newGroupBox("LEVEL");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "0A", "normal_ratio1.75");
    editDetails()->page()->addKnob(2, 0, 1, 1, "10", hex1, hex2, "12", "normal_ratio1.75");
    editDetails()->page()->addKnob(1, 0, 1, 1, "10", hex1, hex2, "03", "normal_ratio1.5");
    editDetails()->page()->addGroupBox(0, 5, 2, 1);
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(5);
    editDetails()->page()->newGroupBox("PITCH SHIFTER");
    editDetails()->page()->newGroupBox("VOICE 1");
    editDetails()->page()->addComboBox(0, 0, 1, 3, "10", hex1, hex2, "04", "large");
    editDetails()->page()->addDataKnob(1, 0, 1, 1, "10", hex1, hex2, "06", "PREDELAY_ratio1.5","bottom", 70);
    editDetails()->page()->addKnob(1, 1, 1, 1, "10", hex1, hex2, "01", "normal_ratio1.5");
    editDetails()->page()->addKnob(1, 2, 1, 1, "10", hex1, hex2, "05", "normal_ratio1.25");
    editDetails()->page()->addGroupBox(0, 0, 3, 3);
    editDetails()->page()->addKnob(1, 4, 1, 1, "10", hex1, hex2, "0B", "normal_ratio1.5");
    editDetails()->page()->newGroupBox("VOICE 2");
    editDetails()->page()->addComboBox(0, 0, 1, 3, "10", hex1, hex2, "0C", "large");
    editDetails()->page()->addDataKnob(1, 0, 1, 1, "10", hex1, hex2, "0E", "PREDELAY_ratio1.5","bottom", 70);
    editDetails()->page()->addKnob(1, 1, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.5");
    editDetails()->page()->addKnob(1, 2, 1, 1, "10", hex1, hex2, "0D", "normal_ratio1.25");
    editDetails()->page()->addGroupBox(3, 0, 1, 3);
    editDetails()->page()->addGroupBox(0, 1, 2, 4);
    editDetails()->page()->newGroupBox("LEVEL");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "0A", "normal_ratio1.75");
    editDetails()->page()->addKnob(2, 0, 1, 1, "10", hex1, hex2, "12", "normal_ratio1.75");
    editDetails()->page()->addKnob(1, 0, 1, 1, "10", hex1, hex2, "03", "normal_ratio1.5");
    editDetails()->page()->addGroupBox(0, 5, 2, 1);
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
    editDetails()->page()->newGroupBox("EFFECT");
    editDetails()->page()->addComboBox(1, 0, 1, 1, "10", hex1, hex2, "01", "large");   //type
    editDetails()->page()->addComboBox(2, 0, 1, 1, "10", hex1, hex2, "03", "large");   //carry over
    editDetails()->page()->addGroupBox(0, 0, 1, 1);
    editDetails()->page()->newGroupBox("REVERB");
    editDetails()->page()->addDataKnob(0, 0, 1, 1, "10", hex1, hex2, "04", "0~500_ratio1.5");  //predelay
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.75");  //time
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "0B", "normal_ratio1.5");  //density
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "0D", "normal_ratio1.5");  //spring sense
    editDetails()->page()->addSystemOverride(0, 3, 1, 1, "10", hex1, hex2, "01", "05", "not_equal");
    editDetails()->page()->addGroupBox(0, 1, 1, 1);
    editDetails()->page()->newGroupBox("FILTER");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "09", "normal_ratio1.5", "bottom", 60);  //low cut
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "0A", "normal_ratio1.5", "bottom", 60);  //high cut
    editDetails()->page()->addGroupBox(1, 0, 1, 1);
    editDetails()->page()->newGroupBox("LEVEL");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "08", "normal_ratio1.75");  //effect
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "0C", "normal_ratio1.5");  //direct
    editDetails()->page()->addGroupBox(1, 1, 1, 1);
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
    editDetails()->page()->newGroupBox("RING MODULATOR");
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, hex2, "00", "large");
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "01", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "03", "normal_ratio1.5");
    editDetails()->page()->addGroupBox(0, 1, 1, 4);
    editDetails()->page()->newGroupBox("LEVEL");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "04", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "05", "normal_ratio1.5");
    editDetails()->page()->addGroupBox(0, 5, 1, 2);
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
    editDetails()->page()->newGroupBox("ROTARY");
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, hex2, "00", "large");
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "01", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "03", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "04", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 5, 1, 1, "10", hex1, hex2, "05", "normal_ratio1.5");
    editDetails()->page()->addGroupBox(0, 1, 1, 6);
    editDetails()->page()->newGroupBox("LEVEL");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "07", "normal_ratio1.5");
    editDetails()->page()->addGroupBox(0, 7, 1, 2);
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
    editDetails()->page()->newGroupBox("SITAR SIMULATOR");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "00", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "01", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "04", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "05", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.5");
    editDetails()->page()->addGroupBox(0, 1, 1, 5);
    editDetails()->page()->newGroupBox("LEVEL");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "03", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.5");
    editDetails()->page()->addGroupBox(0, 6, 1, 2);
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
    editDetails()->page()->newGroupBox("SLICER");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "00", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "01", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "05", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "04", "normal_ratio1.5");
    editDetails()->page()->addGroupBox(0, 1, 1, 5);
    editDetails()->page()->newGroupBox("LEVEL");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "03", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.5");
    editDetails()->page()->addGroupBox(0, 6, 1, 2);
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
    editDetails()->page()->newGroupBox("SLOW GEAR");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "00", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "01", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "02", "normal_ratio2");
    editDetails()->page()->addGroupBox(0, 1, 1, 1);
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
    editDetails()->page()->newGroupBox("SLOW GEAR BASS");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "00", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "01", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "02", "normal_ratio2");
    editDetails()->page()->addGroupBox(0, 1, 1, 1);
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
    editDetails()->page()->newGroupBox("SOUND HOLD");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "00", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "01", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "02", "normal_ratio2");
    editDetails()->page()->addGroupBox(0, 1, 1, 1);
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
    editDetails()->page()->newGroupBox("T-WAH");
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, hex2, "00", "large");
    editDetails()->page()->addComboBox(0, 1, 1, 1, "10", hex1, hex2, "01", "large");
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "03", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "04", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 5, 1, 1, "10", hex1, hex2, "05", "normal_ratio1.5");
    editDetails()->page()->addGroupBox(0, 1, 1, 6);
    editDetails()->page()->newGroupBox("LEVEL");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1,hex2, "07", "normal_ratio1.5");
    editDetails()->page()->addGroupBox(0, 7, 1, 2);
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
    editDetails()->page()->newGroupBox("T-WAH BASS");
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, hex2, "00", "large");
    editDetails()->page()->addComboBox(0, 1, 1, 1, "10", hex1, hex2, "01", "large");
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "03", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "04", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 5, 1, 1, "10", hex1, hex2, "05", "normal_ratio1.5");
    editDetails()->page()->addGroupBox(0, 1, 1, 6);
    editDetails()->page()->newGroupBox("LEVEL");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "07", "normal_ratio1.5");
    editDetails()->page()->addGroupBox(0, 7, 1, 2);
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
    editDetails()->page()->newGroupBox("TREMOLO");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "00", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "01", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.5");
    editDetails()->page()->addGroupBox(0, 1, 1, 1);
    editDetails()->page()->newGroupBox("LEVEL");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "03", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "04", "normal_ratio1.5");
    editDetails()->page()->addGroupBox(0, 2, 1, 1);
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
    editDetails()->page()->newGroupBox("VIBRATO");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "00", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "01", "normal_ratio1.5");
    editDetails()->page()->addComboBox(0, 2, 1, 1, "10", hex1, hex2, "02", "large");
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "03", "normal_ratio1.5");
    editDetails()->page()->addGroupBox(0, 1, 1, 1);
    editDetails()->page()->newGroupBox("LEVEL");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "04", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "05", "normal_ratio1.5");
    editDetails()->page()->addGroupBox(0, 2, 1, 1);
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
    editDetails()->page()->newGroupBox("WAH");
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, hex2, "00", "large");
    editDetails()->page()->addDataKnob(0, 1, 1, 1, "10", hex1, hex2, "02", "0~1000_ratio1.5","bottom", 70);
    editDetails()->page()->addDataKnob(0, 2, 1, 1, "10", hex1, hex2, "06", "0~1000_ratio1.75","bottom", 70);
    editDetails()->page()->addDataKnob(0, 3, 1, 1, "10", hex1, hex2, "0A", "0~1000_ratio1.75","bottom", 70);
    editDetails()->page()->addGroupBox(0, 1, 1, 1);
    editDetails()->page()->newGroupBox("LEVEL");
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "01", "normal_ratio2");
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "0E", "normal_ratio1.75");
    editDetails()->page()->addGroupBox(1, 1, 1, 1);
    editDetails()->page()->addStackField();


    editDetails()->addPage();

}
