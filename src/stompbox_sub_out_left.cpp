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
    QString hex1 = "00";
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){hex1 = "02";};

    editDetails()->setQmlPage("qrc:/qml/SubOutLeftPanel.qml", hex1, "00");

}
