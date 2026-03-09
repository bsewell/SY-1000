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

#include "stompbox_ce.h"
#include "Preferences.h"
#include "SysxIO.h"

stompbox_ce::stompbox_ce(QWidget *parent)
    : stompBox(parent)
{
    SysxIO *sysxIO = SysxIO::Instance();
    sysxIO->deBug("build Chorus stomp");

    QString hex1 = "00";
    QString hex2 = "3F";
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){hex1 = "02"; hex2 = "3C";};

    /* Chorus */
    setOnImage(":/images/ce_on.png");
    setOffImage(":/images/ce_off.png");
    setHoverImage(":/images/effect_hover.png");
    setSelectedImage(":/images/effect_select.png");
    setLSB(hex2, "00");
    brushColor("cyan");
    setButton("10", hex1, hex2, "00");
   /* editDetails()->patchPos(1394, 10, "06", "00");  */
    setEditPages();
}

void stompbox_ce::updateSignal()
{
    QString hex1 = "00";
    QString hex2 = "3F";
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){hex1 = "02"; hex2 = "3C";};

    updateButton("10", hex1, hex2, "00");
    updateLabel("10", hex1, hex2, "01");
};

void stompbox_ce::setEditPages()
{
    QString hex1 = "00";
    QString hex2 = "3F";
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){hex1 = "02"; hex2 = "3C";};

    editDetails()->page()->newGroupBox(tr("Effect"));
    editDetails()->page()->addSwitch(0, 0, 1, 1, "10", hex1, hex2, "00", "middle");
    editDetails()->page()->newStackControl(0);
    editDetails()->page()->addComboBox(1, 0, 1, 1, "10", hex1, hex2, "01", "large"); //mode
    editDetails()->page()->addStackControl();
    editDetails()->page()->addGroupBox(0, 0, 1, 1);

    editDetails()->page()->insertStackField(0, 1, 0, 1, 2);


    editDetails()->page()->newStackField(0);
    editDetails()->page()->newGroupBox(tr("Chorus"));
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.5", "bottom", 60); //rate
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "03", "normal_ratio1.5", "bottom", 60); //depth
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "04", "normal_ratio1.5", "bottom", 60); //predelay
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "05", "normal_ratio1.75", "bottom", 60); //effect
    editDetails()->page()->addGroupBox(0, 0, 1, 1);

    editDetails()->page()->newGroupBox(tr("Filter"));
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, hex2, "06"); //waveform
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "07", "normal_ratio1.5", "bottom", 60);
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "08", "normal_ratio1.5", "bottom", 60);
    editDetails()->page()->addGroupBox(0, 1, 1, 1);
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(0);
    editDetails()->page()->newGroupBox(tr("Chorus"));
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.5", "bottom", 60); //rate
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "03", "normal_ratio1.5", "bottom", 60); //depth
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "04", "normal_ratio1.5", "bottom", 60); //predelay
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "05", "normal_ratio1.75", "bottom", 60); //effect
    editDetails()->page()->addGroupBox(0, 0, 1, 1);

    editDetails()->page()->newGroupBox(tr("Filter"));
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, hex2, "06"); //waveform
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "07", "normal_ratio1.5", "bottom", 60);
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "08", "normal_ratio1.5", "bottom", 60);
    editDetails()->page()->addGroupBox(0, 1, 1, 1);
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(0);
    editDetails()->page()->newGroupBox(tr("Chorus"));
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.5", "bottom", 60); //rate
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "03", "normal_ratio1.5", "bottom", 60); //depth
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "04", "normal_ratio1.5", "bottom", 60); //predelay
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "05", "normal_ratio1.75", "bottom", 60); //effect
    editDetails()->page()->addGroupBox(0, 0, 1, 1);

    editDetails()->page()->newGroupBox(tr("Filter"));
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, hex2, "06"); //waveform
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "07", "normal_ratio1.5", "bottom", 60);
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "08", "normal_ratio1.5", "bottom", 60);
    editDetails()->page()->addGroupBox(0, 1, 1, 1);
    editDetails()->page()->addStackField();


    editDetails()->page()->newStackField(0);
    editDetails()->page()->newGroupBox(tr("Chorus 1"));
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "09", "normal_ratio1.25", "bottom", 60); //rate
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "0A", "normal_ratio1.25", "bottom", 60); //depth
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "0B", "normal_ratio1.25", "bottom", 60); //predelay
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "0C", "normal_ratio1.5", "bottom", 60); //effect
    editDetails()->page()->addGroupBox(0, 0, 1, 1);

    editDetails()->page()->newGroupBox(tr("Filter 1"));
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, hex2, "0D"); //waveform
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "0E", "normal_ratio1.25", "bottom", 60);
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "0F", "normal_ratio1.25", "bottom", 60);
    editDetails()->page()->addGroupBox(0, 1, 1, 1);

    editDetails()->page()->newGroupBox(tr("Chorus 2"));
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "10", "normal_ratio1.25", "bottom", 60); //rate
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "11", "normal_ratio1.25", "bottom", 60); //depth
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "12", "normal_ratio1.25", "bottom", 60); //predelay
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "13", "normal_ratio1.5", "bottom", 60); //effect
    editDetails()->page()->addGroupBox(1, 0, 1, 1);

    editDetails()->page()->newGroupBox(tr("Filter 2"));
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, hex2, "14"); //waveform
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "15", "normal_ratio1.25", "bottom", 60);
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "16", "normal_ratio1.25", "bottom", 60);
    editDetails()->page()->addGroupBox(1, 1, 1, 1);
    editDetails()->page()->addStackField();

	editDetails()->page()->newGroupBox(tr("Level"));
    editDetails()->page()->addKnob(0, 0, 2, 1, "10", hex1, hex2, "17", "normal_ratio1.5"); //direct
    editDetails()->page()->addComboBox(0, 1, 1, 1, "10", hex1, hex2, "18", "large"); //output
    editDetails()->page()->addGroupBox(0, 1, 1, 1);

	editDetails()->addPage();
}
