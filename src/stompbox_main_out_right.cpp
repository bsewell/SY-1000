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

#include "stompbox_main_out_right.h"
#include "Preferences.h"
#include "SysxIO.h"

stompbox_main_out_right::stompbox_main_out_right(QWidget *parent)
    : stompBox(parent)
{
    SysxIO *sysxIO = SysxIO::Instance();
    sysxIO->deBug("build Main Right stomp");

    /* ns */
    setOnImage(":/images/main_out_right.png");
    setOffImage(":/images/main_out_right.png");
    setHoverImage(":/images/effect_hover.png");
    setSelectedImage(":/images/effect_select.png");
    //setLSB("07", "5A");
    brushColor("purple");
  /*editDetails()->patchPos(1660, 6, "07", "5A"); */
    setEditPages();
}

void stompbox_main_out_right::updateSignal()
{
    QString sys1 = "04";
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){sys1 = "0B";};
    updateLabel("00", sys1, "00", "01");
}

void stompbox_main_out_right::setEditPages()
{
    QString sys1 = "04";
    QString hex1 = "00";
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){sys1 = "0B"; hex1 = "02";};

    // formerly "Global Main Output Right" groupBox at addGroupBox(0, 0, 2, 1) — parentCol=0
    editDetails()->page()->addDataKnob(1, 0, 1, 1, "00", sys1, "00", "2E", "0~200_ratio1.75"); // r output
    editDetails()->page()->addSystemOverride(0, 0, 3, 1, "00", sys1, "00", "34", "01", "equals");

    // formerly "Global Main Output Select Right" groupBox at addGroupBox(0, 1, 1, 1) — parentCol=1
    editDetails()->page()->addComboBox(0, 1, 1, 1, "00", sys1, "00", "01", "large"); // output select
    editDetails()->page()->addSystemOverride(0, 1, 1, 1, "00", sys1, "00", "34", "01", "equals");

    // formerly "Patch Main Out Phase Right" groupBox at addGroupBox(0, 2, 1, 1) — parentCol=2
    editDetails()->page()->addComboBox(0, 3, 1, 1, "10", hex1, "12", "38", "large");  // phase right (inner col 1 + parentCol 2 = 3)
    editDetails()->page()->addSystemOverride(0, 2, 1, 1, "00", sys1, "00", "34", "01", "equals");

    // formerly "Main Out Global EQ Right" groupBox at addGroupBox(1, 1, 1, 2) — parentRow=1, parentCol=1
    editDetails()->page()->addParaEQ(1, 1, 2, 4, "00", sys1, "00", "0C", "System_output", "39");
    editDetails()->page()->addSystemOverride(1, 1, 4, 6, "00", sys1, "00", "36", "01", "equals");
    editDetails()->addPage();



}
