/****************************************************************************
**
** Copyright (C) 2007~2025 Colin Willcocks.
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

#include "stompbox_sub_out_right.h"
#include "Preferences.h"
#include "SysxIO.h"

stompbox_sub_out_right::stompbox_sub_out_right(QWidget *parent)
    : stompBox(parent)
{
    SysxIO *sysxIO = SysxIO::Instance();
    sysxIO->deBug("build Sub Right stomp");

    /* ns */
    setOnImage(":/images/sub_out_right.png");
    setOffImage(":/images/sub_out_right.png");
    setHoverImage(":/images/effect_hover.png");
    setSelectedImage(":/images/effect_select.png");
    setLSB("07", "5A");
    brushColor("purple");
  /*editDetails()->patchPos(1660, 6, "07", "5A"); */
    setEditPages();
}

void stompbox_sub_out_right::updateSignal()
{
    QString sys1 = "04";
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){sys1 = "0B";};
    updateLabel("00", sys1, "00", "17");

}

void stompbox_sub_out_right::setEditPages()
{
    QString sys1 = "04";
    QString hex1 = "00";
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){sys1 = "0B"; hex1 = "02";};
    
    // formerly "Global Sub Output Right" groupBox at addGroupBox(0, 0, 2, 1) — parentCol=0
    editDetails()->page()->addDataKnob(1, 0, 1, 1, "00", sys1, "00", "32", "0~200_ratio1.75");               // sub output right
    editDetails()->page()->addSystemOverride(0, 0, 3, 1, "00", sys1, "00", "35", "01", "equals");  // stereo link

    // formerly "Global Sub Output Select Right" groupBox at addGroupBox(0, 1, 1, 1) — parentCol=1
    editDetails()->page()->addComboBox(0, 1, 1, 1, "00", sys1, "00", "17", "large");              // sub output select
    editDetails()->page()->addSystemOverride(0, 1, 1, 1, "00", sys1, "00", "35", "01", "equals"); // stereo link

    // formerly "Patch Sub Out Phase Right" groupBox at addGroupBox(0, 2, 1, 1) — parentCol=2, inner col 1 → flat col 3
    editDetails()->page()->addComboBox(0, 3, 1, 1, "10", hex1, "12", "3B", "large");              // sub phase right
    editDetails()->page()->addSystemOverride(0, 2, 1, 1, "00", sys1, "00", "35", "01", "equals"); // stereo link

    // formerly "Sub Out Global EQ Right" groupBox at addGroupBox(1, 1, 1, 2) — parentRow=1, parentCol=1
    editDetails()->page()->addParaEQ(1, 1, 2, 4, "00", sys1, "00", "22", "System_output", "3B");         // sub global eq right
    editDetails()->page()->addSystemOverride(1, 1, 4, 6, "00", sys1, "00", "37", "01", "equals"); // eq link
    editDetails()->addPage();
}
