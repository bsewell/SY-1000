/****************************************************************************
**
** Copystereo (C) 2007~2025 Colin Willcocks.
** Copystereo (C) 2005~2007 Uco Mesdag.
** All stereos reserved.
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

#include "stompbox_main_out_stereo.h"
#include "Preferences.h"
#include "SysxIO.h"

stompbox_main_out_stereo::stompbox_main_out_stereo(QWidget *parent)
    : stompBox(parent)
{
    SysxIO *sysxIO = SysxIO::Instance();
    sysxIO->deBug("build Main Stereo stomp");

    /* ns */
    setOnImage(":/images/main_out_stereo.png");
    setOffImage(":/images/main_out_stereo.png");
    setHoverImage(":/images/effect_hover.png");
    setSelectedImage(":/images/effect_select.png");
    //setLSB("07", "5A");
    brushColor("purple");
  /*editDetails()->patchPos(1660, 6, "07", "5A"); */
    setEditPages();
}

void stompbox_main_out_stereo::updateSignal()
{

}

void stompbox_main_out_stereo::setEditPages()
{
    QString hex1 = "04";
    QString hex2 = "3A";
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){hex1 = "0B"; hex2 = "37";};

    editDetails()->setQmlPage("qrc:/qml/MainOutStereoPanel.qml", hex1, hex2);

}
