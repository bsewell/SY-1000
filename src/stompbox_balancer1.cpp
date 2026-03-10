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

#include "stompbox_balancer1.h"
#include "Preferences.h"
#include "SysxIO.h"

stompbox_balancer1::stompbox_balancer1(QWidget *parent)
    : stompBox(parent)
{
    SysxIO *sysxIO = SysxIO::Instance();
    sysxIO->deBug("build Balancer1 stomp");

    /* mixer */
    setOnImage(":/images/balancer1.png");
    setOffImage(":/images/balancer1.png");
    setHoverImage(":/images/effect_hover.png");
    setSelectedImage(":/images/effect_select.png");
    setLSB("12", "20");
    brushColor("gray");
    //setButton("10", hex1, "39", "00");
  /*editDetails()->patchPos(1660, 6, "07", "5A"); */
    setEditPages();
}

void stompbox_balancer1::updateSignal()
{
  //updateButton("10", hex1, "39", "00");
}

void stompbox_balancer1::setEditPages()
{
    QString hex1 = "00";
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){hex1 = "02"; };
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, "12", "26", "normal_ratio1.5", "bottom", 60);
    editDetails()->page()->addKnob(1, 1, 1, 1, "10", hex1, "12", "25", "normal_ratio2", "bottom", 70);
    editDetails()->page()->addKnob(2, 0, 1, 1, "10", hex1, "12", "27", "normal_ratio1.5", "bottom", 60);
    editDetails()->addPage();

}
