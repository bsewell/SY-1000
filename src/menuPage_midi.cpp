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

#include "menuPage_pdl.h"
#include "Preferences.h"

#include <iostream>

menuPage_pdl::menuPage_pdl(QWidget *parent)
    : menuPage(parent)
{
    std::string st = QString("build Pedal menu").toStdString();
    std::cerr << st << std::endl;
    /* PDL */
   // setLSB("00", "11");
  //  editDetails()->patchPos(56, 272, "00", "11"); //correct
    setEditPages();
}

void menuPage_pdl::updateSignal()
{

}

void menuPage_pdl::setEditPages()
{
    QString hex1 = "00";
    QString sys1 = "01";
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){hex1 = "02"; sys1 = "08"; };

    editDetails()->page()->newGroupBox("NUMBER 1");
    editDetails()->page()->newGroupBox("MEMORY MODE");
    editDetails()->page()->newStackControl(0);
    editDetails()->page()->addTabBar(2, 0, 1, 1, "00", sys1, "10", "21"); // patch/system
    editDetails()->page()->addStackControl();
    editDetails()->page()->insertStackField(0, 0, 0, 1, 1);
    editDetails()->page()->addGroupBox(0, 0, 1, 1);
    editDetails()->page()->newGroupBox("MANUAL MODE");
    editDetails()->page()->newStackControl(1);
    editDetails()->page()->addTabBar(2, 0, 1, 1, "00", sys1, "10", "3B"); // patch/system
    editDetails()->page()->addStackControl();
    editDetails()->page()->insertStackField(1, 0, 0, 1, 1);
    editDetails()->page()->addGroupBox(1, 0, 1, 1);
    editDetails()->page()->addGroupBox(0, 0, 1, 1);

    editDetails()->page()->newStackField(0);
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, "00", "10"); // patch function
    editDetails()->page()->addComboBox(1, 0, 1, 1, "10", hex1, "00", "11"); // patch mode
    editDetails()->page()->addStackField();
    editDetails()->page()->newStackField(0);
    editDetails()->page()->addComboBox(0, 0, 1, 1, "00", sys1, "10", "00"); //  function
    editDetails()->page()->addComboBox(1, 0, 1, 1, "00", sys1, "10", "01"); // patch mode
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(1);
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, "00", "10"); // patch function
    editDetails()->page()->addComboBox(1, 0, 1, 1, "10", hex1, "00", "11"); // patch mode
    editDetails()->page()->addStackField();
    editDetails()->page()->newStackField(1);
    editDetails()->page()->addComboBox(0, 0, 1, 1, "00", sys1, "10", "33"); // system function
    editDetails()->page()->addComboBox(1, 0, 1, 1, "00", sys1, "10", "34"); // system mode
    editDetails()->page()->addStackField();

    editDetails()->page()->newGroupBox("NUMBER 2");
    editDetails()->page()->newGroupBox("MEMORY MODE");
    editDetails()->page()->newStackControl(2);
    editDetails()->page()->addTabBar(2, 0, 1, 1, "00", sys1, "10", "22"); // patch/system
    editDetails()->page()->addStackControl();
    editDetails()->page()->insertStackField(2, 0, 0, 1, 1);
    editDetails()->page()->addGroupBox(0, 0, 1, 1);
    editDetails()->page()->newGroupBox("MANUAL MODE");
    editDetails()->page()->newStackControl(3);
    editDetails()->page()->addTabBar(2, 0, 1, 1, "00", sys1, "10", "3C"); // patch/system
    editDetails()->page()->addStackControl();
    editDetails()->page()->insertStackField(3, 0, 0, 1, 1);
    editDetails()->page()->addGroupBox(1, 0, 1, 1);
    editDetails()->page()->addGroupBox(0, 1, 1, 1);

    editDetails()->page()->newStackField(2);
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, "00", "12"); // patch function
    editDetails()->page()->addComboBox(1, 0, 1, 1, "10", hex1, "00", "13"); // patch mode
    editDetails()->page()->addStackField();
    editDetails()->page()->newStackField(2);
    editDetails()->page()->addComboBox(0, 0, 1, 1, "00", sys1, "10", "02"); //  function
    editDetails()->page()->addComboBox(1, 0, 1, 1, "00", sys1, "10", "03"); // patch mode
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(3);
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, "00", "10"); // patch function
    editDetails()->page()->addComboBox(1, 0, 1, 1, "10", hex1, "00", "11"); // patch mode
    editDetails()->page()->addStackField();
    editDetails()->page()->newStackField(3);
    editDetails()->page()->addComboBox(0, 0, 1, 1, "00", sys1, "10", "35"); // system function
    editDetails()->page()->addComboBox(1, 0, 1, 1, "00", sys1, "10", "36"); // system mode
    editDetails()->page()->addStackField();


    editDetails()->page()->newGroupBox("NUMBER 3");
    editDetails()->page()->newGroupBox("MEMORY MODE");
    editDetails()->page()->newStackControl(4);
    editDetails()->page()->addTabBar(2, 0, 1, 1, "00", sys1, "10", "23"); // patch/system
    editDetails()->page()->addStackControl();
    editDetails()->page()->insertStackField(4, 0, 0, 1, 1);
    editDetails()->page()->addGroupBox(0, 0, 1, 1);
    editDetails()->page()->newGroupBox("MANUAL MODE");
    editDetails()->page()->newStackControl(5);
    editDetails()->page()->addTabBar(2, 0, 1, 1, "00", sys1, "10", "3D"); // patch/system
    editDetails()->page()->addStackControl();
    editDetails()->page()->insertStackField(5, 0, 0, 1, 1);
    editDetails()->page()->addGroupBox(1, 0, 1, 1);
    editDetails()->page()->addGroupBox(0, 2, 1, 1);

    editDetails()->page()->newStackField(4);
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, "00", "14"); // patch function
    editDetails()->page()->addComboBox(1, 0, 1, 1, "10", hex1, "00", "15"); // patch mode
    editDetails()->page()->addStackField();
    editDetails()->page()->newStackField(4);
    editDetails()->page()->addComboBox(0, 0, 1, 1, "00", sys1, "10", "04"); //  function
    editDetails()->page()->addComboBox(1, 0, 1, 1, "00", sys1, "10", "05"); // patch mode
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(5);
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, "00", "14"); // patch function
    editDetails()->page()->addComboBox(1, 0, 1, 1, "10", hex1, "00", "15"); // patch mode
    editDetails()->page()->addStackField();
    editDetails()->page()->newStackField(5);
    editDetails()->page()->addComboBox(0, 0, 1, 1, "00", sys1, "10", "37"); // system function
    editDetails()->page()->addComboBox(1, 0, 1, 1, "00", sys1, "10", "38"); // system mode
    editDetails()->page()->addStackField();



    editDetails()->page()->newGroupBox("NUMBER 4");
    editDetails()->page()->newGroupBox("MEMORY MODE");
    editDetails()->page()->newStackControl(6);
    editDetails()->page()->addTabBar(2, 0, 1, 1, "00", sys1, "10", "24"); // patch/system
    editDetails()->page()->addStackControl();
    editDetails()->page()->insertStackField(6, 0, 0, 1, 1);
    editDetails()->page()->addGroupBox(0, 0, 1, 1);
    editDetails()->page()->newGroupBox("MANUAL MODE");
    editDetails()->page()->newStackControl(7);
    editDetails()->page()->addTabBar(2, 0, 1, 1, "00", sys1, "10", "3E"); // patch/system
    editDetails()->page()->addStackControl();
    editDetails()->page()->insertStackField(7, 0, 0, 1, 1);
    editDetails()->page()->addGroupBox(1, 0, 1, 1);
    editDetails()->page()->addGroupBox(0, 3, 1, 1);

    editDetails()->page()->newStackField(6);
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, "00", "16"); // patch function
    editDetails()->page()->addComboBox(1, 0, 1, 1, "10", hex1, "00", "17"); // patch mode
    editDetails()->page()->addStackField();
    editDetails()->page()->newStackField(6);
    editDetails()->page()->addComboBox(0, 0, 1, 1, "00", sys1, "10", "06"); //  function
    editDetails()->page()->addComboBox(1, 0, 1, 1, "00", sys1, "10", "07"); // patch mode
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(7);
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, "00", "16"); // patch function
    editDetails()->page()->addComboBox(1, 0, 1, 1, "10", hex1, "00", "17"); // patch mode
    editDetails()->page()->addStackField();
    editDetails()->page()->newStackField(7);
    editDetails()->page()->addComboBox(0, 0, 1, 1, "00", sys1, "10", "39"); // system function
    editDetails()->page()->addComboBox(1, 0, 1, 1, "00", sys1, "10", "3A"); // system mode
    editDetails()->page()->addStackField();

    editDetails()->page()->newGroupBox("MEMORY/PATCH MODE");
    editDetails()->page()->addTabBar(2, 0, 1, 1, "00", sys1, "10", "3F", "large_top"); // memory/patch
    editDetails()->page()->addGroupBox(1, 0, 1, 4);

    editDetails()->addPage("Tables", "00", "00", "7E", "00");       // ******************************************************************

    editDetails()->page()->newGroupBox("BANK DOWN");
    editDetails()->page()->newGroupBox("FUNCTION");
    editDetails()->page()->newStackControl(0);
    editDetails()->page()->addTabBar(2, 0, 1, 1, "00", sys1, "10", "26"); // patch/system
    editDetails()->page()->addStackControl();
    editDetails()->page()->insertStackField(0, 0, 0, 1, 1);
    editDetails()->page()->addGroupBox(0, 0, 1, 1);
    editDetails()->page()->addGroupBox(0, 0, 1, 1);

    editDetails()->page()->newStackField(0);
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, "00", "18"); // patch function
    editDetails()->page()->addComboBox(1, 0, 1, 1, "10", hex1, "00", "19"); // patch mode
    editDetails()->page()->addStackField();
    editDetails()->page()->newStackField(0);
    editDetails()->page()->addComboBox(0, 0, 1, 1, "00", sys1, "10", "08"); //  function
    editDetails()->page()->addComboBox(1, 0, 1, 1, "00", sys1, "10", "09"); // patch mode
    editDetails()->page()->addStackField();




    editDetails()->page()->newGroupBox("BANK UP");
    editDetails()->page()->newGroupBox("FUNCTION");
    editDetails()->page()->newStackControl(1);
    editDetails()->page()->addTabBar(2, 0, 1, 1, "00", sys1, "10", "27"); // patch/system
    editDetails()->page()->addStackControl();
    editDetails()->page()->insertStackField(1, 0, 0, 1, 1);
    editDetails()->page()->addGroupBox(0, 0, 1, 1);
    editDetails()->page()->addGroupBox(0, 1, 1, 1);

    editDetails()->page()->newStackField(1);
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, "00", "1A"); // patch function
    editDetails()->page()->addComboBox(1, 0, 1, 1, "10", hex1, "00", "1B"); // patch mode
    editDetails()->page()->addStackField();
    editDetails()->page()->newStackField(1);
    editDetails()->page()->addComboBox(0, 0, 1, 1, "00", sys1, "10", "0A"); //  function
    editDetails()->page()->addComboBox(1, 0, 1, 1, "00", sys1, "10", "0B"); // patch mode
    editDetails()->page()->addStackField();



    editDetails()->page()->newGroupBox("CURRENT NUMBER");
    editDetails()->page()->newGroupBox("FUNCTION");
    editDetails()->page()->newStackControl(2);
    editDetails()->page()->addTabBar(2, 0, 1, 1, "00", sys1, "10", "2D"); // patch/system
    editDetails()->page()->addStackControl();
    editDetails()->page()->insertStackField(2, 0, 0, 1, 1);
    editDetails()->page()->addGroupBox(0, 0, 1, 1);
    editDetails()->page()->addGroupBox(0, 2, 1, 1);

    editDetails()->page()->newStackField(2);
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, "00", "28"); // patch function
    editDetails()->page()->addComboBox(1, 0, 1, 1, "10", hex1, "00", "29"); // patch mode
    editDetails()->page()->addStackField();
    editDetails()->page()->newStackField(2);
    editDetails()->page()->addComboBox(0, 0, 1, 1, "00", sys1, "10", "18"); //  function
    editDetails()->page()->addComboBox(1, 0, 1, 1, "00", sys1, "10", "19"); // patch mode
    editDetails()->page()->addStackField();

    editDetails()->page()->newGroupBox("BANK UP + BANK DOWN");
    editDetails()->page()->newGroupBox("FUNCTION");
    editDetails()->page()->addTabBar(2, 0, 1, 1, "00", sys1, "00", "08"); // tuner/off
    editDetails()->page()->addGroupBox(0, 0, 1, 1);
    editDetails()->page()->addGroupBox(0, 3, 1, 1);

    editDetails()->addPage("Tables", "00", "00", "7E", "01"); // *********************************************************************


    editDetails()->page()->newGroupBox("CTL 1");
    editDetails()->page()->newGroupBox("FUNCTION");
    editDetails()->page()->newStackControl(0);
    editDetails()->page()->addTabBar(2, 0, 1, 1, "00", sys1, "10", "27"); // patch/system
    editDetails()->page()->addStackControl();
    editDetails()->page()->insertStackField(0, 0, 0, 1, 1);
    editDetails()->page()->addGroupBox(0, 0, 1, 1);
    editDetails()->page()->addGroupBox(0, 0, 1, 1);

    editDetails()->page()->newStackField(0);
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, "00", "1C"); // patch function
    editDetails()->page()->addComboBox(1, 0, 1, 1, "10", hex1, "00", "1D"); // patch mode
    editDetails()->page()->addStackField();
    editDetails()->page()->newStackField(0);
    editDetails()->page()->addComboBox(0, 0, 1, 1, "00", sys1, "10", "0C"); //  function
    editDetails()->page()->addComboBox(1, 0, 1, 1, "00", sys1, "10", "0D"); // patch mode
    editDetails()->page()->addStackField();

    editDetails()->page()->newGroupBox("CTL 2");
    editDetails()->page()->newGroupBox("FUNCTION");
    editDetails()->page()->newStackControl(1);
    editDetails()->page()->addTabBar(2, 0, 1, 1, "00", sys1, "10", "28"); // patch/system
    editDetails()->page()->addStackControl();
    editDetails()->page()->insertStackField(1, 0, 0, 1, 1);
    editDetails()->page()->addGroupBox(0, 0, 1, 1);
    editDetails()->page()->addGroupBox(0, 1, 1, 1);

    editDetails()->page()->newStackField(1);
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, "00", "1E"); // patch function
    editDetails()->page()->addComboBox(1, 0, 1, 1, "10", hex1, "00", "1F"); // patch mode
    editDetails()->page()->addStackField();
    editDetails()->page()->newStackField(1);
    editDetails()->page()->addComboBox(0, 0, 1, 1, "00", sys1, "10", "0E"); //  function
    editDetails()->page()->addComboBox(1, 0, 1, 1, "00", sys1, "10", "0F"); // patch mode
    editDetails()->page()->addStackField();

    editDetails()->page()->newGroupBox("CTL 3");
    editDetails()->page()->newGroupBox("FUNCTION");
    editDetails()->page()->newStackControl(2);
    editDetails()->page()->addTabBar(2, 0, 1, 1, "00", sys1, "10", "29"); // patch/system
    editDetails()->page()->addStackControl();
    editDetails()->page()->insertStackField(2, 0, 0, 1, 1);
    editDetails()->page()->addGroupBox(0, 0, 1, 1);
    editDetails()->page()->addGroupBox(0, 2, 1, 1);

    editDetails()->page()->newStackField(2);
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, "00", "20"); // patch function
    editDetails()->page()->addComboBox(1, 0, 1, 1, "10", hex1, "00", "21"); // patch mode
    editDetails()->page()->addStackField();
    editDetails()->page()->newStackField(2);
    editDetails()->page()->addComboBox(0, 0, 1, 1, "00", sys1, "10", "10"); //  function
    editDetails()->page()->addComboBox(1, 0, 1, 1, "00", sys1, "10", "11"); // patch mode
    editDetails()->page()->addStackField();

    editDetails()->page()->newGroupBox("CTL 4");
    editDetails()->page()->newGroupBox("FUNCTION");
    editDetails()->page()->newStackControl(3);
    editDetails()->page()->addTabBar(2, 0, 1, 1, "00", sys1, "10", "2A"); // patch/system
    editDetails()->page()->addStackControl();
    editDetails()->page()->insertStackField(3, 0, 0, 1, 1);
    editDetails()->page()->addGroupBox(0, 0, 1, 1);
    editDetails()->page()->addGroupBox(1, 0, 1, 1);

    editDetails()->page()->newStackField(3);
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, "00", "22"); // patch function
    editDetails()->page()->addComboBox(1, 0, 1, 1, "10", hex1, "00", "23"); // patch mode
    editDetails()->page()->addStackField();
    editDetails()->page()->newStackField(3);
    editDetails()->page()->addComboBox(0, 0, 1, 1, "00", sys1, "10", "12"); //  function
    editDetails()->page()->addComboBox(1, 0, 1, 1, "00", sys1, "10", "13"); // patch mode
    editDetails()->page()->addStackField();

    editDetails()->page()->newGroupBox("CTL 5");
    editDetails()->page()->newGroupBox("FUNCTION");
    editDetails()->page()->newStackControl(4);
    editDetails()->page()->addTabBar(2, 0, 1, 1, "00", sys1, "10", "2B"); // patch/system
    editDetails()->page()->addStackControl();
    editDetails()->page()->insertStackField(4, 0, 0, 1, 1);
    editDetails()->page()->addGroupBox(0, 0, 1, 1);
    editDetails()->page()->addGroupBox(1, 1, 1, 1);

    editDetails()->page()->newStackField(4);
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, "00", "24"); // patch function
    editDetails()->page()->addComboBox(1, 0, 1, 1, "10", hex1, "00", "25"); // patch mode
    editDetails()->page()->addStackField();
    editDetails()->page()->newStackField(4);
    editDetails()->page()->addComboBox(0, 0, 1, 1, "00", sys1, "10", "14"); //  function
    editDetails()->page()->addComboBox(1, 0, 1, 1, "00", sys1, "10", "15"); // patch mode
    editDetails()->page()->addStackField();

    editDetails()->page()->newGroupBox("CTL 6");
    editDetails()->page()->newGroupBox("FUNCTION");
    editDetails()->page()->newStackControl(5);
    editDetails()->page()->addTabBar(2, 0, 1, 1, "00", sys1, "10", "2C"); // patch/system
    editDetails()->page()->addStackControl();
    editDetails()->page()->insertStackField(5, 0, 0, 1, 1);
    editDetails()->page()->addGroupBox(0, 0, 1, 1);
    editDetails()->page()->addGroupBox(1, 2, 1, 1);

    editDetails()->page()->newStackField(5);
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, "00", "26"); // patch function
    editDetails()->page()->addComboBox(1, 0, 1, 1, "10", hex1, "00", "27"); // patch mode
    editDetails()->page()->addStackField();
    editDetails()->page()->newStackField(5);
    editDetails()->page()->addComboBox(0, 0, 1, 1, "00", sys1, "10", "16"); //  function
    editDetails()->page()->addComboBox(1, 0, 1, 1, "00", sys1, "10", "17"); // patch mode
    editDetails()->page()->addStackField();

    editDetails()->addPage("Tables", "00", "00", "7E", "02"); // *********************************************************************



    editDetails()->page()->newGroupBox("EXP 1");
    editDetails()->page()->newGroupBox("FUNCTION");
    editDetails()->page()->newStackControl(0);
    editDetails()->page()->addTabBar(2, 0, 1, 1, "00", sys1, "10", "2E"); // patch/system
    editDetails()->page()->addStackControl();
    editDetails()->page()->insertStackField(0, 0, 0, 1, 1);
    editDetails()->page()->addGroupBox(0, 0, 1, 1);
    editDetails()->page()->addGroupBox(0, 0, 1, 1);

    editDetails()->page()->newStackField(0);
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, "00", "2A"); // patch function
    editDetails()->page()->addStackField();
    editDetails()->page()->newStackField(0);
    editDetails()->page()->addComboBox(0, 0, 1, 1, "00", sys1, "10", "1A"); //  function
    editDetails()->page()->addStackField();




    editDetails()->page()->newGroupBox("EXP 2");
    editDetails()->page()->newGroupBox("FUNCTION");
    editDetails()->page()->newStackControl(1);
    editDetails()->page()->addTabBar(2, 0, 1, 1, "00", sys1, "10", "2F"); // patch/system
    editDetails()->page()->addStackControl();
    editDetails()->page()->insertStackField(1, 0, 0, 1, 1);
    editDetails()->page()->addGroupBox(0, 0, 1, 1);
    editDetails()->page()->addGroupBox(0, 1, 1, 1);

    editDetails()->page()->newStackField(1);
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, "00", "2B"); // patch function
    editDetails()->page()->addStackField();
    editDetails()->page()->newStackField(1);
    editDetails()->page()->addComboBox(0, 0, 1, 1, "00", sys1, "10", "1B"); //  function
    editDetails()->page()->addStackField();



    editDetails()->page()->newGroupBox("EXP 1 ASSIGN HOLD");
    editDetails()->page()->newGroupBox("GLOBAL");
    editDetails()->page()->addTabBar(2, 0, 1, 1, "00", sys1, "00", "0A"); // exp 1 hold
    editDetails()->page()->addGroupBox(0, 0, 1, 1);
    editDetails()->page()->addGroupBox(1, 0, 1, 1);

    editDetails()->page()->newGroupBox("EXP 2 ASSIGN HOLD");
    editDetails()->page()->newGroupBox("GLOBAL");
    editDetails()->page()->addTabBar(2, 0, 1, 1, "00", sys1, "00", "0B"); // exp 2 hold
    editDetails()->page()->addGroupBox(0, 0, 1, 1);
    editDetails()->page()->addGroupBox(1, 1, 1, 1);

    editDetails()->addPage("Tables", "00", "00", "7E", "03"); // *********************************************************************



    editDetails()->page()->newGroupBox("GK VOLUME");
    editDetails()->page()->newGroupBox("FUNCTION");
    editDetails()->page()->newStackControl(0);
    editDetails()->page()->addTabBar(2, 0, 1, 1, "00", sys1, "10", "30"); // patch/system
    editDetails()->page()->addStackControl();
    editDetails()->page()->insertStackField(0, 0, 0, 1, 1);
    editDetails()->page()->addGroupBox(0, 0, 1, 1);
    editDetails()->page()->addGroupBox(0, 0, 1, 1);

    editDetails()->page()->newStackField(0);
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, "00", "2C"); // patch function
    editDetails()->page()->addStackField();
    editDetails()->page()->newStackField(0);
    editDetails()->page()->addComboBox(0, 0, 1, 1, "00", sys1, "10", "1C"); //  function
    editDetails()->page()->addStackField();



    editDetails()->page()->newGroupBox("GK VOLUME ASSIGN HOLD");
    editDetails()->page()->newGroupBox("GLOBAL");
    editDetails()->page()->addTabBar(2, 0, 1, 1, "00", sys1, "00", "09"); // exp 1 hold
    editDetails()->page()->addGroupBox(0, 0, 1, 1);
    editDetails()->page()->addGroupBox(1, 0, 1, 1);


    editDetails()->page()->newGroupBox("GK SWITCH 1");
    editDetails()->page()->newGroupBox("FUNCTION");
    editDetails()->page()->newStackControl(1);
    editDetails()->page()->addTabBar(2, 0, 1, 1, "00", sys1, "10", "31"); // patch/system
    editDetails()->page()->addStackControl();
    editDetails()->page()->insertStackField(1, 0, 0, 1, 1);
    editDetails()->page()->addGroupBox(0, 0, 1, 1);
    editDetails()->page()->addGroupBox(0, 1, 2, 1);

    editDetails()->page()->newStackField(1);
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, "00", "2D"); // patch function
    editDetails()->page()->addComboBox(1, 0, 1, 1, "10", hex1, "00", "2E"); // patch mode
    editDetails()->page()->addStackField();
    editDetails()->page()->newStackField(1);
    editDetails()->page()->addComboBox(0, 0, 1, 1, "00", sys1, "10", "1D"); //  function
    editDetails()->page()->addComboBox(1, 0, 1, 1, "00", sys1, "10", "1E"); // patch mode
    editDetails()->page()->addStackField();

    editDetails()->page()->newGroupBox("GK SWITCH 2");
    editDetails()->page()->newGroupBox("FUNCTION");
    editDetails()->page()->newStackControl(1);
    editDetails()->page()->addTabBar(2, 0, 1, 1, "00", sys1, "10", "32"); // patch/system
    editDetails()->page()->addStackControl();
    editDetails()->page()->insertStackField(1, 0, 0, 1, 1);
    editDetails()->page()->addGroupBox(0, 0, 1, 1);
    editDetails()->page()->addGroupBox(0, 2, 2, 1);

    editDetails()->page()->newStackField(1);
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, "00", "2F"); // patch function
    editDetails()->page()->addComboBox(1, 0, 1, 1, "10", hex1, "00", "30"); // patch mode
    editDetails()->page()->addStackField();
    editDetails()->page()->newStackField(1);
    editDetails()->page()->addComboBox(0, 0, 1, 1, "00", sys1, "10", "1F"); //  function
    editDetails()->page()->addComboBox(1, 0, 1, 1, "00", sys1, "10", "20"); // patch mode
    editDetails()->page()->addStackField();

    editDetails()->addPage("Tables", "00", "00", "7E", "04"); // *********************************************************************


}
