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

#include "stompbox_fv2.h"
#include "Preferences.h"
#include "SysxIO.h"

stompbox_fv2::stompbox_fv2(QWidget *parent)
    : stompBox(parent)
{
    SysxIO *sysxIO = SysxIO::Instance();
    sysxIO->deBug("build FV2 stomp");

	/* FV2 */ 
    setOnImage(":/images/fv2_on.png");
    setOffImage(":/images/fv2_on.png");
    setHoverImage(":/images/effect_hover.png");
    setSelectedImage(":/images/effect_select.png");
	//setLSB("0A", "00");
    brushColor("gray");
	//setComboBox("0A", "00", "45");
	//setButton("0A", "00", "40", QPoint(0, 110), ":/images/fv1.png");
	//editDetails()->patchPos(2830, 60, "0A", "40");  	
	setEditPages();
};

void stompbox_fv2::updateSignal()
{
  //updateComboBox("0A", "00", "45");
	//updateButton("0A", "00", "40");
};

void stompbox_fv2::setEditPages()
{
    QString hex1 = "00";
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){hex1 = "02";};

        editDetails()->page()->addDataKnob(0, 0, 1, 1, "10", hex1, "12", "13", "0~1000_ratio1.75");
        editDetails()->page()->addDataKnob(0, 1, 1, 1, "10", hex1, "12", "17", "0~1000_ratio1.75");
        editDetails()->page()->addDataKnob(0, 2, 1, 1, "10", hex1, "12", "1B", "0~1000_ratio1.5");
        editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, "12", "1F", "normal_ratio1.5", "bottom", 60);
        
        editDetails()->addPage();    
};
