/****************************************************************************
**
** Copyleft (C) 2007~2025 Colin Willcocks.
** All lefts reserved.
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

#include "stompbox_sub_out_left.h"
#include "Preferences.h"
#include "SysxIO.h"

stompbox_sub_out_left::stompbox_sub_out_left(QWidget *parent)
    : stompBox(parent)
{
    SysxIO *sysxIO = SysxIO::Instance();
    sysxIO->deBug("build Sub Left stomp");

    /* ns */
    setOnImage(":/images/sub_out_left.png");
    setOffImage(":/images/sub_out_left.png");
    setHoverImage(":/images/effect_hover.png");
    setSelectedImage(":/images/effect_select.png");
    setLSB("07", "5A");
    brushColor("purple");
  /*editDetails()->patchPos(1660, 6, "07", "5A"); */
    setEditPages();
}

void stompbox_sub_out_left::updateSignal()
{
    QString sys1 = "04";
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){sys1 = "0B";};
    updateLabel("00", sys1, "00", "16");
}

void stompbox_sub_out_left::setEditPages()
{
    QString sys1 = "04";
    QString hex1 = "00";
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){sys1 = "0B"; hex1 = "02";};

    editDetails()->page()->newGroupBox("Global Sub Output");
    editDetails()->page()->addDataKnob(0, 0, 1, 1, "00", sys1, "00", "30", "0~200_ratio1.75"); // l output
    editDetails()->page()->newStackControl(0);
    editDetails()->page()->addComboBox(1, 0, 1, 1, "00", sys1, "00", "35", "large"); // stereo link
    editDetails()->page()->addStackControl();
    editDetails()->page()->newStackControl(1);
    editDetails()->page()->addComboBox(2, 0, 1, 1, "00", sys1, "00", "37", "large"); // eq link
    editDetails()->page()->addStackControl();
    editDetails()->page()->addGroupBox(0, 0, 4, 1);

    editDetails()->page()->newGroupBox("Global Sub EQ Select");
    editDetails()->page()->newStackControl(2);
    editDetails()->page()->addTabBar(0, 0, 1, 1, "Tables", "00", "02", "04"); // eq L/R
    editDetails()->page()->addSystemOverride(0, 0, 1, 1, "00", sys1, "00", "37", "01", "equals");
    editDetails()->page()->addStackControl();
    editDetails()->page()->addGroupBox(4, 0, 1, 1);

    editDetails()->page()->insertStackField(0, 0, 1, 1, 4);  // stereo link
    editDetails()->page()->insertStackField(1, 1, 1, 4, 4);  // eq link

    editDetails()->page()->newStackField(0);  // stereo link
    editDetails()->page()->newGroupBox("Global Sub Output Select Left");
    editDetails()->page()->addComboBox(0, 0, 1, 2, "00", sys1, "00", "16", "large"); // output select
    editDetails()->page()->addGroupBox(0, 0, 1, 2);
    editDetails()->page()->newGroupBox("Patch Sub Phase Left");
    editDetails()->page()->addComboBox(0, 0, 1, 2, "10", hex1, "12", "3A", "large");  // phase left
    editDetails()->page()->addGroupBox(0, 2, 1, 2);
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(0);  // stereo link
    editDetails()->page()->newGroupBox("Global Sub Output Select Stereo");
    editDetails()->page()->addComboBox(0, 0, 1, 2, "00", sys1, "00", "16", "large"); // output select
    editDetails()->page()->addGroupBox(0, 0, 1, 2);
    editDetails()->page()->newGroupBox("Patch Sub Phase Stereo");
    editDetails()->page()->addComboBox(0, 0, 1, 2, "10", hex1, "12", "3A", "large");  // phase left
    editDetails()->page()->addGroupBox(0, 2, 1, 2);
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(1);  // eq link
    editDetails()->page()->insertStackField(2, 0, 0, 1, 1);
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(1);  // eq link
    editDetails()->page()->newGroupBox("Sub Out Global EQ Stereo Linked");
    editDetails()->page()->addParaEQ(1, 0, 1, 1, "00", sys1, "00", "18", "System_output", "3A");  // left EQ
    editDetails()->page()->addGroupBox(0, 0, 1, 1);
    editDetails()->page()->addStackField();


    editDetails()->page()->newStackField(2);
    editDetails()->page()->newGroupBox("Sub Out Global EQ Left");
    editDetails()->page()->addParaEQ(0, 0, 1, 1, "00", sys1, "00", "18", "System_output", "3A");  // left EQ
    editDetails()->page()->addGroupBox(0, 0, 1, 1);
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(2);
    editDetails()->page()->newGroupBox("Sub Out Global EQ Right");
    editDetails()->page()->addParaEQ(0, 0, 1, 1, "00", sys1, "00", "22", "System_output", "3B");  // right EQ
    editDetails()->page()->addGroupBox(0, 0, 1, 1);
    editDetails()->page()->addStackField();

    editDetails()->addPage();

}
