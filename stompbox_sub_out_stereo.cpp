/****************************************************************************
**
** Copystereo (C) 2007~2025 Colin Willcocks.
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

#include "stompbox_sub_out_stereo.h"
#include "SysxIO.h"

stompbox_sub_out_stereo::stompbox_sub_out_stereo(QWidget *parent)
    : stompBox(parent)
{
    SysxIO *sysxIO = SysxIO::Instance();
    sysxIO->deBug("build Sub Stereo stomp");

    /* ns */
    setOnImage(":/images/sub_out_stereo.png");
    setOffImage(":/images/sub_out_stereo.png");
    setHoverImage(":/images/effect_hover.png");
    setSelectedImage(":/images/effect_select.png");
    setLSB("07", "5A");
    brushColor("purple");
  /*editDetails()->patchPos(1660, 6, "07", "5A"); */
    setEditPages();
}

void stompbox_sub_out_stereo::updateSignal()
{

}

void stompbox_sub_out_stereo::setEditPages()
{
  /*  editDetails()->page()->newGroupBox("Effect");

    editDetails()->page()->addGroupBox(0, 0, 1, 1);


    editDetails()->page()->newGroupBox("sub Global EQ");

    editDetails()->page()->addGroupBox(0, 1, 1, 1);
    editDetails()->addPage();
  */
}
