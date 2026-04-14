/****************************************************************************
**
** Copyright (C) 2007~2020 Colin Willcocks.
** Copyright (C) 2005~2007 Uco Mesdag. 
** All rights reserved.
** This file is part of "SY-1000 FloorBoard".
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

#include "stompbox_master.h"

stompbox_master::stompbox_master(QWidget *parent)
    : stompBox(parent)
{
    /* mixer */
    setOnImage(":/images/master.png");
    setOffImage(":/images/master.png");
    setHoverImage(":/images/effect_hover.png");
    setSelectedImage(":/images/effect_select.png");
    setLSB("12", "20");
    //setButton("10", "00", "39", "00");
  /*editDetails()->patchPos(1660, 6, "07", "5A"); */
    setEditPages();
}

void stompbox_master::updateSignal()
{
  //updateButton("10", "00", "39", "00");
}

void stompbox_master::setEditPages()
{
    editDetails()->page()->newGroupBox("Mode");
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", "00", "12", "20", "large");
    editDetails()->page()->addComboBox(0, 1, 1, 1, "10", "00", "12", "21", "large");
    editDetails()->page()->addGroupBox(0, 0, 1, 1);
    editDetails()->addPage();

}
