/****************************************************************************
**
** Copyright (C) 2007~2025 Colin Willcocks.
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

#include "menuPage_system.h"
#include "Preferences.h"
#include "SysxIO.h"

menuPage_system::menuPage_system(QWidget *parent)
    : menuPage(parent)
{
    SysxIO *sysxIO = SysxIO::Instance();
    sysxIO->deBug("build System menu");

    setEditPages();
}

void menuPage_system::updateSignal()
{

}

void menuPage_system::setEditPages()
{ 
    QString hex1 = "01";
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){hex1 = "08";};

    QString text = tr("***CAUTION*** System changes are automatically written to SY-1000");
    editDetails()->page()->addLabel(0, 0, 1, 4, text);

    editDetails()->page()->newGroupBox("Bank");
    editDetails()->page()->addComboBox(0, 0, 1, 2, "00", hex1, "00", "05"); // change type
    editDetails()->page()->addKnob(1, 0, 1, 1, "00", hex1, "00", "06", "normal", "bottom", 70); // extent min
    editDetails()->page()->addKnob(1, 1, 1, 1, "00", hex1, "00", "07", "normal", "bottom", 70); // extent max
    editDetails()->page()->addGroupBox(1, 0, 2, 2);

    editDetails()->page()->newGroupBox("Output Knob");
    editDetails()->page()->addComboBox(0, 0, 1, 1, "00", hex1, "00", "0D"); // output
    editDetails()->page()->addGroupBox(5, 0, 1, 2);

    editDetails()->page()->newGroupBox("Bank up/down sw");
    editDetails()->page()->addComboBox(0, 0, 1, 1, "00", hex1, "00", "08"); // up/down sw
    editDetails()->page()->addGroupBox(6, 0, 1, 1);

    editDetails()->page()->newGroupBox("Knob Lock 1~6");
    editDetails()->page()->addComboBox(0, 0, 1, 1, "00", hex1, "00", "0C"); // lock
    editDetails()->page()->addGroupBox(6, 1, 1, 1);

    editDetails()->page()->newGroupBox("Knob Function 1~6");
    editDetails()->page()->addComboBox(0, 0, 1, 1, "00", hex1, "00", "0E", "data_4Byte"); // 1
    editDetails()->page()->addComboBox(1, 0, 1, 1, "00", hex1, "00", "12", "data_4Byte"); // 2
    editDetails()->page()->addComboBox(2, 0, 1, 1, "00", hex1, "00", "16", "data_4Byte"); // 3
    editDetails()->page()->addComboBox(3, 0, 1, 1, "00", hex1, "00", "1A", "data_4Byte"); // 4
    editDetails()->page()->addComboBox(4, 0, 1, 1, "00", hex1, "00", "1E", "data_4Byte"); // 5
    editDetails()->page()->addComboBox(5, 0, 1, 1, "00", hex1, "00", "22", "data_4Byte"); // 6
    editDetails()->page()->addGroupBox(1, 2, 6, 1);

    editDetails()->page()->newGroupBox("Hardware");
    editDetails()->page()->addComboBox(0, 0, 1, 1, "00", "00", "00", "00"); // auto power off
    editDetails()->page()->addComboBox(0, 1, 1, 1, "00", "00", "00", "01"); // hidden feature
    hex1 = "04";
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){hex1 = "0B";};
    editDetails()->page()->addComboBox(0, 3, 1, 1, "00", hex1, "10", "00"); // lift
    editDetails()->page()->addGroupBox(6, 3, 1, 3);

    editDetails()->page()->newGroupBox("USB");
    editDetails()->page()->addKnob(0, 0, 1, 1, "00", hex1, "00", "41"); //
    editDetails()->page()->addKnob(1, 0, 1, 1, "00", hex1, "00", "42"); //
    editDetails()->page()->addComboBox(2, 0, 1, 1, "00", hex1, "00", "43"); //
    editDetails()->page()->addComboBox(3, 0, 1, 1, "00", hex1, "00", "44"); //
    editDetails()->page()->addComboBox(4, 0, 1, 1, "00", hex1, "00", "45"); //
    editDetails()->page()->addGroupBox(1, 3, 5, 3);

    editDetails()->page()->newGroupBox("Total");
    editDetails()->page()->addKnob(0, 0, 1, 1, "00", hex1, "00", "3C"); // ns
    editDetails()->page()->addDataKnob(0, 1, 1, 1, "00", hex1, "00", "3D", "0~200"); // reverb
    editDetails()->page()->addGroupBox(3, 0, 1, 2);



    editDetails()->addPage("Tables", "00", "02", "01", "00");       // ******************************************************************
    hex1 = "01";
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){hex1 = "08";};

    editDetails()->page()->newGroupBox("Midi I/O Function");
    editDetails()->page()->addComboBox(0, 0, 1, 1, "00", hex1, "30", "00"); //
    editDetails()->page()->addComboBox(1, 0, 1, 1, "00", hex1, "30", "01"); //
    editDetails()->page()->addComboBox(2, 0, 1, 1, "00", hex1, "30", "02"); //
    editDetails()->page()->addComboBox(3, 0, 1, 1, "00", hex1, "30", "03"); //
    editDetails()->page()->addComboBox(4, 0, 1, 1, "00", hex1, "30", "04"); //
    editDetails()->page()->addComboBox(0, 1, 1, 1, "00", hex1, "30", "05"); //
    editDetails()->page()->addComboBox(1, 1, 1, 1, "00", hex1, "30", "06"); //
    editDetails()->page()->addComboBox(2, 1, 1, 1, "00", hex1, "30", "07"); //
    editDetails()->page()->addComboBox(3, 1, 1, 1, "00", hex1, "30", "08"); //
    editDetails()->page()->addComboBox(4, 1, 1, 1, "00", hex1, "30", "09"); //
    editDetails()->page()->addGroupBox(0, 0, 10, 1);

    editDetails()->page()->newGroupBox("Midi CC# Send Function");
    editDetails()->page()->addComboBox(0, 1, 1, 1, "00", hex1, "30", "0A"); // No1
    editDetails()->page()->addComboBox(1, 1, 1, 1, "00", hex1, "30", "0B"); // No2
    editDetails()->page()->addComboBox(2, 1, 1, 1, "00", hex1, "30", "0C"); // No3
    editDetails()->page()->addComboBox(3, 1, 1, 1, "00", hex1, "30", "0D"); // No4
    editDetails()->page()->addComboBox(5, 1, 1, 1, "00", hex1, "30", "0E"); // Bank Down
    editDetails()->page()->addComboBox(6, 1, 1, 1, "00", hex1, "30", "0F"); // Bank Up

    editDetails()->page()->addComboBox(0, 2, 1, 1, "00", hex1, "30", "10"); // CTL1
    editDetails()->page()->addComboBox(1, 2, 1, 1, "00", hex1, "30", "11"); // CTL2
    editDetails()->page()->addComboBox(2, 2, 1, 1, "00", hex1, "30", "12"); // CTL3
    editDetails()->page()->addComboBox(3, 2, 1, 1, "00", hex1, "30", "13"); // CTL4
    editDetails()->page()->addComboBox(5, 2, 1, 1, "00", hex1, "30", "14"); // CTL5
    editDetails()->page()->addComboBox(6, 2, 1, 1, "00", hex1, "30", "15"); // CTL6

    editDetails()->page()->addComboBox(0, 3, 1, 1, "00", hex1, "30", "16"); // EXP 1
    editDetails()->page()->addComboBox(1, 3, 1, 1, "00", hex1, "30", "17"); // EXP 2
    editDetails()->page()->addComboBox(3, 3, 1, 1, "00", hex1, "30", "18"); // GK Vol
    editDetails()->page()->addComboBox(5, 3, 1, 1, "00", hex1, "30", "19"); // GK S1
    editDetails()->page()->addComboBox(6, 3, 1, 1, "00", hex1, "30", "1A"); // GK S2
    editDetails()->page()->addGroupBox(0, 1, 10, 1);


    editDetails()->addPage("Tables", "00", "02", "01", "01");       // ******************************************************************

    editDetails()->page()->newGroupBox("Tuner");
    editDetails()->page()->addComboBox(0, 0, 1, 2, "00", hex1, "00", "04"); // tuner type
    hex1 = "04";
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){hex1 = "0B";};
    editDetails()->page()->addComboBox(1, 0, 1, 2, "00", hex1, "20", "03"); // ref pitch
    //editDetails()->page()->addComboBox(2, 0, 1, 2, "00", hex1, "20", "04"); // poly type
    //editDetails()->page()->addComboBox(3, 0, 1, 2, "00", hex1, "20", "05"); // offset
    editDetails()->page()->addComboBox(4, 0, 1, 2, "00", hex1, "20", "06"); // output
    editDetails()->page()->addGroupBox(0, 0, 1, 2);




    editDetails()->addPage("Tables", "00", "02", "01", "02");       // ******************************************************************

    hex1 = "01";
    QString scale = "SCALE_G";
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){hex1 = "08"; scale = "SCALE_B";};

    editDetails()->page()->newGroupBox("System GK Set Selct");
    editDetails()->page()->addComboBox(0, 0, 1, 1, "00", hex1, "00", "26", "large"); // GK Set
    editDetails()->page()->addGroupBox(0, 0, 1, 1);

    hex1 = "01";
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){hex1 = "08";};

    editDetails()->page()->newGroupBox("System GK-Set Settings");
    editDetails()->page()->newStackControl(0);
    editDetails()->page()->addTabBar(0, 0, 1, 1, "Tables", "00", "02", "00"); //edit page select GK SET
    editDetails()->page()->addStackControl();
    editDetails()->page()->insertStackField(0, 1, 0, 1, 1);
    editDetails()->page()->addGroupBox(0, 1, 1, 1);

    if(preferences->getPreferences("Window", "BassMode", "bool")=="false"){
    editDetails()->page()->newStackField(0);
    editDetails()->page()->newGroupBox("GK SET 1");
    editDetails()->page()->addNameEdit(0, 0, 1, 1, "00", "01", "40", "00", "8"); //GK catagory
    editDetails()->page()->addComboBox(0, 1, 1, 1, "00", "01", "40", "08"); // GK type
    editDetails()->page()->addDataKnob(0, 2, 1, 2, "00", "01", "40", "09", scale, "right", 120);     //scale
    editDetails()->page()->addComboBox(0, 4, 1, 1, "00", "01", "40", "17"); //
    editDetails()->page()->addComboBox(0, 5, 1, 1, "00", "01", "40", "18"); //
    editDetails()->page()->addKnob(1, 0, 1, 1, "00", "01", "40", "0B", "normal", "bottom", 70);  // distance 1
    editDetails()->page()->addKnob(1, 1, 1, 1, "00", "01", "40", "0C", "normal", "bottom", 70);  // distance 2
    editDetails()->page()->addKnob(1, 2, 1, 1, "00", "01", "40", "0D", "normal", "bottom", 70);  // distance 3
    editDetails()->page()->addKnob(1, 3, 1, 1, "00", "01", "40", "0E", "normal", "bottom", 70);  // distance 4
    editDetails()->page()->addKnob(1, 4, 1, 1, "00", "01", "40", "0F", "normal", "bottom", 70);  // distance 5
    editDetails()->page()->addKnob(1, 5, 1, 1, "00", "01", "40", "10", "normal", "bottom", 70);  // distance 6
    editDetails()->page()->addKnob(2, 0, 1, 1, "00", "01", "40", "11");  // level 1
    editDetails()->page()->addKnob(2, 1, 1, 1, "00", "01", "40", "12");  // level 2
    editDetails()->page()->addKnob(2, 2, 1, 1, "00", "01", "40", "13");  // level 3
    editDetails()->page()->addKnob(2, 3, 1, 1, "00", "01", "40", "14");  // level 4
    editDetails()->page()->addKnob(2, 4, 1, 1, "00", "01", "40", "15");  // level 5
    editDetails()->page()->addKnob(2, 5, 1, 1, "00", "01", "40", "16");  // level 6
    editDetails()->page()->addKnob(4, 0, 1, 1, "00", "01", "40", "1C"); // pu gain
    editDetails()->page()->addComboBox(4, 1, 1, 1, "00", "01", "40", "1B"); //
    editDetails()->page()->addKnob(4, 2, 1, 1, "00", "01", "40", "19"); //
    editDetails()->page()->addKnob(4, 3, 1, 1, "00", "01", "40", "1A"); //
    editDetails()->page()->addSystemOverride(4, 2, 1, 2, "00", "01", "40", "08", "02", "more_than");
    editDetails()->page()->addSystemOverride(4, 4, 1, 2, "00", "01", "40", "08", "03", "less_than");
    editDetails()->page()->addGroupBox(0, 0, 7, 6);
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(0);
    editDetails()->page()->newGroupBox("GK SET 2");
    editDetails()->page()->addNameEdit(0, 0, 1, 1, "00", "01", "50", "00", "8"); //GK catagory
    editDetails()->page()->addComboBox(0, 1, 1, 1, "00", "01", "50", "08"); // GK type
    editDetails()->page()->addDataKnob(0, 2, 1, 2, "00", "01", "50", "09", scale, "right", 120);     //scale
    editDetails()->page()->addComboBox(0, 4, 1, 1, "00", "01", "50", "17"); //
    editDetails()->page()->addComboBox(0, 5, 1, 1, "00", "01", "50", "18"); //
    editDetails()->page()->addKnob(1, 0, 1, 1, "00", "01", "50", "0B", "normal", "bottom", 70);  // distance 1
    editDetails()->page()->addKnob(1, 1, 1, 1, "00", "01", "50", "0C", "normal", "bottom", 70);  // distance 2
    editDetails()->page()->addKnob(1, 2, 1, 1, "00", "01", "50", "0D", "normal", "bottom", 70);  // distance 3
    editDetails()->page()->addKnob(1, 3, 1, 1, "00", "01", "50", "0E", "normal", "bottom", 70);  // distance 4
    editDetails()->page()->addKnob(1, 4, 1, 1, "00", "01", "50", "0F", "normal", "bottom", 70);  // distance 5
    editDetails()->page()->addKnob(1, 5, 1, 1, "00", "01", "50", "10", "normal", "bottom", 70);  // distance 6
    editDetails()->page()->addKnob(2, 0, 1, 1, "00", "01", "50", "11");  // level 1
    editDetails()->page()->addKnob(2, 1, 1, 1, "00", "01", "50", "12");  // level 2
    editDetails()->page()->addKnob(2, 2, 1, 1, "00", "01", "50", "13");  // level 3
    editDetails()->page()->addKnob(2, 3, 1, 1, "00", "01", "50", "14");  // level 4
    editDetails()->page()->addKnob(2, 4, 1, 1, "00", "01", "50", "15");  // level 5
    editDetails()->page()->addKnob(2, 5, 1, 1, "00", "01", "50", "16");  // level 6
    editDetails()->page()->addKnob(4, 0, 1, 1, "00", "01", "50", "1C"); // pu gain
    editDetails()->page()->addComboBox(4, 1, 1, 1, "00", "01", "50", "1B"); //
    editDetails()->page()->addKnob(4, 2, 1, 1, "00", "01", "50", "19"); //
    editDetails()->page()->addKnob(4, 3, 1, 1, "00", "01", "50", "1A"); //
    editDetails()->page()->addSystemOverride(4, 2, 1, 2, "00", "01", "50", "08", "02", "more_than");
    editDetails()->page()->addSystemOverride(4, 4, 1, 2, "00", "01", "50", "08", "03", "less_than");
    editDetails()->page()->addGroupBox(0, 0, 7, 6);
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(0);
    editDetails()->page()->newGroupBox("GK SET 3");
    editDetails()->page()->addNameEdit(0, 0, 1, 1, "00", "01", "60", "00", "8"); //GK catagory
    editDetails()->page()->addComboBox(0, 1, 1, 1, "00", "01", "60", "08"); // GK type
    editDetails()->page()->addDataKnob(0, 2, 1, 2, "00", "01", "60", "09", scale, "right", 120);     //scale
    editDetails()->page()->addComboBox(0, 4, 1, 1, "00", "01", "60", "17"); //
    editDetails()->page()->addComboBox(0, 5, 1, 1, "00", "01", "60", "18"); //
    editDetails()->page()->addKnob(1, 0, 1, 1, "00", "01", "60", "0B", "normal", "bottom", 70);  // distance 1
    editDetails()->page()->addKnob(1, 1, 1, 1, "00", "01", "60", "0C", "normal", "bottom", 70);  // distance 2
    editDetails()->page()->addKnob(1, 2, 1, 1, "00", "01", "60", "0D", "normal", "bottom", 70);  // distance 3
    editDetails()->page()->addKnob(1, 3, 1, 1, "00", "01", "60", "0E", "normal", "bottom", 70);  // distance 4
    editDetails()->page()->addKnob(1, 4, 1, 1, "00", "01", "60", "0F", "normal", "bottom", 70);  // distance 5
    editDetails()->page()->addKnob(1, 5, 1, 1, "00", "01", "60", "10", "normal", "bottom", 70);  // distance 6
    editDetails()->page()->addKnob(2, 0, 1, 1, "00", "01", "60", "11");  // level 1
    editDetails()->page()->addKnob(2, 1, 1, 1, "00", "01", "60", "12");  // level 2
    editDetails()->page()->addKnob(2, 2, 1, 1, "00", "01", "60", "13");  // level 3
    editDetails()->page()->addKnob(2, 3, 1, 1, "00", "01", "60", "14");  // level 4
    editDetails()->page()->addKnob(2, 4, 1, 1, "00", "01", "60", "15");  // level 5
    editDetails()->page()->addKnob(2, 5, 1, 1, "00", "01", "60", "16");  // level 6
    editDetails()->page()->addKnob(4, 0, 1, 1, "00", "01", "60", "1C"); // pu gain
    editDetails()->page()->addComboBox(4, 1, 1, 1, "00", "01", "60", "1B"); //
    editDetails()->page()->addKnob(4, 2, 1, 1, "00", "01", "60", "19"); //
    editDetails()->page()->addKnob(4, 3, 1, 1, "00", "01", "60", "1A"); //
    editDetails()->page()->addSystemOverride(4, 2, 1, 2, "00", "01", "60", "08", "02", "more_than");
    editDetails()->page()->addSystemOverride(4, 4, 1, 2, "00", "01", "60", "08", "03", "less_than");
    editDetails()->page()->addGroupBox(0, 0, 7, 6);
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(0);
    editDetails()->page()->newGroupBox("GK SET 4");
    editDetails()->page()->addNameEdit(0, 0, 1, 1, "00", "01", "70", "00", "8"); //GK catagory
    editDetails()->page()->addComboBox(0, 1, 1, 1, "00", "01", "70", "08"); // GK type
    editDetails()->page()->addDataKnob(0, 2, 1, 2, "00", "01", "70", "09", scale, "right", 120);     //scale
    editDetails()->page()->addComboBox(0, 4, 1, 1, "00", "01", "70", "17"); //
    editDetails()->page()->addComboBox(0, 5, 1, 1, "00", "01", "70", "18"); //
    editDetails()->page()->addKnob(1, 0, 1, 1, "00", "01", "70", "0B", "normal", "bottom", 70);  // distance 1
    editDetails()->page()->addKnob(1, 1, 1, 1, "00", "01", "70", "0C", "normal", "bottom", 70);  // distance 2
    editDetails()->page()->addKnob(1, 2, 1, 1, "00", "01", "70", "0D", "normal", "bottom", 70);  // distance 3
    editDetails()->page()->addKnob(1, 3, 1, 1, "00", "01", "70", "0E", "normal", "bottom", 70);  // distance 4
    editDetails()->page()->addKnob(1, 4, 1, 1, "00", "01", "70", "0F", "normal", "bottom", 70);  // distance 5
    editDetails()->page()->addKnob(1, 5, 1, 1, "00", "01", "70", "10", "normal", "bottom", 70);  // distance 6
    editDetails()->page()->addKnob(2, 0, 1, 1, "00", "01", "70", "11");  // level 1
    editDetails()->page()->addKnob(2, 1, 1, 1, "00", "01", "70", "12");  // level 2
    editDetails()->page()->addKnob(2, 2, 1, 1, "00", "01", "70", "13");  // level 3
    editDetails()->page()->addKnob(2, 3, 1, 1, "00", "01", "70", "14");  // level 4
    editDetails()->page()->addKnob(2, 4, 1, 1, "00", "01", "70", "15");  // level 5
    editDetails()->page()->addKnob(2, 5, 1, 1, "00", "01", "70", "16");  // level 6
    editDetails()->page()->addKnob(4, 0, 1, 1, "00", "01", "70", "1C"); // pu gain
    editDetails()->page()->addComboBox(4, 1, 1, 1, "00", "01", "70", "1B"); //
    editDetails()->page()->addKnob(4, 2, 1, 1, "00", "01", "70", "19"); //
    editDetails()->page()->addKnob(4, 3, 1, 1, "00", "01", "70", "1A"); //
    editDetails()->page()->addSystemOverride(4, 2, 1, 2, "00", "01", "70", "08", "02", "more_than");
    editDetails()->page()->addSystemOverride(4, 4, 1, 2, "00", "01", "70", "08", "03", "less_than");
    editDetails()->page()->addGroupBox(0, 0, 7, 6);
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(0);
    editDetails()->page()->newGroupBox("GK SET 5");
    editDetails()->page()->addNameEdit(0, 0, 1, 1, "00", "02", "00", "00", "8"); //GK catagory
    editDetails()->page()->addComboBox(0, 1, 1, 1, "00", "02", "00", "08"); // GK type
    editDetails()->page()->addDataKnob(0, 2, 1, 2, "00", "02", "00", "09", scale, "right", 120);     //scale
    editDetails()->page()->addComboBox(0, 4, 1, 1, "00", "02", "00", "17"); //
    editDetails()->page()->addComboBox(0, 5, 1, 1, "00", "02", "00", "18"); //
    editDetails()->page()->addKnob(1, 0, 1, 1, "00", "02", "00", "0B", "normal", "bottom", 70);  // distance 1
    editDetails()->page()->addKnob(1, 1, 1, 1, "00", "02", "00", "0C", "normal", "bottom", 70);  // distance 2
    editDetails()->page()->addKnob(1, 2, 1, 1, "00", "02", "00", "0D", "normal", "bottom", 70);  // distance 3
    editDetails()->page()->addKnob(1, 3, 1, 1, "00", "02", "00", "0E", "normal", "bottom", 70);  // distance 4
    editDetails()->page()->addKnob(1, 4, 1, 1, "00", "02", "00", "0F", "normal", "bottom", 70);  // distance 5
    editDetails()->page()->addKnob(1, 5, 1, 1, "00", "02", "00", "10", "normal", "bottom", 70);  // distance 6
    editDetails()->page()->addKnob(2, 0, 1, 1, "00", "02", "00", "11");  // level 1
    editDetails()->page()->addKnob(2, 1, 1, 1, "00", "02", "00", "12");  // level 2
    editDetails()->page()->addKnob(2, 2, 1, 1, "00", "02", "00", "13");  // level 3
    editDetails()->page()->addKnob(2, 3, 1, 1, "00", "02", "00", "14");  // level 4
    editDetails()->page()->addKnob(2, 4, 1, 1, "00", "02", "00", "15");  // level 5
    editDetails()->page()->addKnob(2, 5, 1, 1, "00", "02", "00", "16");  // level 6
    editDetails()->page()->addKnob(4, 0, 1, 1, "00", "02", "00", "1C"); // pu gain
    editDetails()->page()->addComboBox(4, 1, 1, 1, "00", "02", "00", "1B"); //
    editDetails()->page()->addKnob(4, 2, 1, 1, "00", "02", "00", "19"); //
    editDetails()->page()->addKnob(4, 3, 1, 1, "00", "02", "00", "1A"); //
    editDetails()->page()->addSystemOverride(4, 2, 1, 2, "00", "02", "00", "08", "02", "more_than");
    editDetails()->page()->addSystemOverride(4, 4, 1, 2, "00", "02", "00", "08", "03", "less_than");
    editDetails()->page()->addGroupBox(0, 0, 7, 6);
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(0);
    editDetails()->page()->newGroupBox("GK SET 6");
    editDetails()->page()->addNameEdit(0, 0, 1, 1, "00", "02", "10", "00", "8"); //GK catagory
    editDetails()->page()->addComboBox(0, 1, 1, 1, "00", "02", "10", "08"); // GK type
    editDetails()->page()->addDataKnob(0, 2, 1, 2, "00", "02", "10", "09", scale, "right", 120);     //scale
    editDetails()->page()->addComboBox(0, 4, 1, 1, "00", "02", "10", "17"); //
    editDetails()->page()->addComboBox(0, 5, 1, 1, "00", "02", "10", "18"); //
    editDetails()->page()->addKnob(1, 0, 1, 1, "00", "02", "10", "0B", "normal", "bottom", 70);  // distance 1
    editDetails()->page()->addKnob(1, 1, 1, 1, "00", "02", "10", "0C", "normal", "bottom", 70);  // distance 2
    editDetails()->page()->addKnob(1, 2, 1, 1, "00", "02", "10", "0D", "normal", "bottom", 70);  // distance 3
    editDetails()->page()->addKnob(1, 3, 1, 1, "00", "02", "10", "0E", "normal", "bottom", 70);  // distance 4
    editDetails()->page()->addKnob(1, 4, 1, 1, "00", "02", "10", "0F", "normal", "bottom", 70);  // distance 5
    editDetails()->page()->addKnob(1, 5, 1, 1, "00", "02", "10", "10", "normal", "bottom", 70);  // distance 6
    editDetails()->page()->addKnob(2, 0, 1, 1, "00", "02", "10", "11");  // level 1
    editDetails()->page()->addKnob(2, 1, 1, 1, "00", "02", "10", "12");  // level 2
    editDetails()->page()->addKnob(2, 2, 1, 1, "00", "02", "10", "13");  // level 3
    editDetails()->page()->addKnob(2, 3, 1, 1, "00", "02", "10", "14");  // level 4
    editDetails()->page()->addKnob(2, 4, 1, 1, "00", "02", "10", "15");  // level 5
    editDetails()->page()->addKnob(2, 5, 1, 1, "00", "02", "10", "16");  // level 6
    editDetails()->page()->addKnob(4, 0, 1, 1, "00", "02", "10", "1C"); // pu gain
    editDetails()->page()->addComboBox(4, 1, 1, 1, "00", "02", "10", "1B"); //
    editDetails()->page()->addKnob(4, 2, 1, 1, "00", "02", "10", "19"); //
    editDetails()->page()->addKnob(4, 3, 1, 1, "00", "02", "10", "1A"); //
    editDetails()->page()->addSystemOverride(4, 2, 1, 2, "00", "02", "10", "08", "02", "more_than");
    editDetails()->page()->addSystemOverride(4, 4, 1, 2, "00", "02", "10", "08", "03", "less_than");
    editDetails()->page()->addGroupBox(0, 0, 7, 6);
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(0);
    editDetails()->page()->newGroupBox("GK SET 7");
    editDetails()->page()->addNameEdit(0, 0, 1, 1, "00", "02", "20", "00", "8"); //GK catagory
    editDetails()->page()->addComboBox(0, 1, 1, 1, "00", "02", "20", "08"); // GK type
    editDetails()->page()->addDataKnob(0, 2, 1, 2, "00", "02", "20", "09", scale, "right", 120);     //scale
    editDetails()->page()->addComboBox(0, 4, 1, 1, "00", "02", "20", "17"); //
    editDetails()->page()->addComboBox(0, 5, 1, 1, "00", "02", "20", "18"); //
    editDetails()->page()->addKnob(1, 0, 1, 1, "00", "02", "20", "0B", "normal", "bottom", 70);  // distance 1
    editDetails()->page()->addKnob(1, 1, 1, 1, "00", "02", "20", "0C", "normal", "bottom", 70);  // distance 2
    editDetails()->page()->addKnob(1, 2, 1, 1, "00", "02", "20", "0D", "normal", "bottom", 70);  // distance 3
    editDetails()->page()->addKnob(1, 3, 1, 1, "00", "02", "20", "0E", "normal", "bottom", 70);  // distance 4
    editDetails()->page()->addKnob(1, 4, 1, 1, "00", "02", "20", "0F", "normal", "bottom", 70);  // distance 5
    editDetails()->page()->addKnob(1, 5, 1, 1, "00", "02", "20", "10", "normal", "bottom", 70);  // distance 6
    editDetails()->page()->addKnob(2, 0, 1, 1, "00", "02", "20", "11");  // level 1
    editDetails()->page()->addKnob(2, 1, 1, 1, "00", "02", "20", "12");  // level 2
    editDetails()->page()->addKnob(2, 2, 1, 1, "00", "02", "20", "13");  // level 3
    editDetails()->page()->addKnob(2, 3, 1, 1, "00", "02", "20", "14");  // level 4
    editDetails()->page()->addKnob(2, 4, 1, 1, "00", "02", "20", "15");  // level 5
    editDetails()->page()->addKnob(2, 5, 1, 1, "00", "02", "20", "16");  // level 6
    editDetails()->page()->addKnob(4, 0, 1, 1, "00", "02", "20", "1C"); // pu gain
    editDetails()->page()->addComboBox(4, 1, 1, 1, "00", "02", "20", "1B"); //
    editDetails()->page()->addKnob(4, 2, 1, 1, "00", "02", "20", "19"); //
    editDetails()->page()->addKnob(4, 3, 1, 1, "00", "02", "20", "1A"); //
    editDetails()->page()->addSystemOverride(4, 2, 1, 2, "00", "02", "20", "08", "02", "more_than");
    editDetails()->page()->addSystemOverride(4, 4, 1, 2, "00", "02", "20", "08", "03", "less_than");
    editDetails()->page()->addGroupBox(0, 0, 7, 6);
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(0);
    editDetails()->page()->newGroupBox("GK SET 8");
    editDetails()->page()->addNameEdit(0, 0, 1, 1, "00", "02", "30", "00", "8"); //GK catagory
    editDetails()->page()->addComboBox(0, 1, 1, 1, "00", "02", "30", "08"); // GK type
    editDetails()->page()->addDataKnob(0, 2, 1, 2, "00", "02", "30", "09", scale, "right", 120);     //scale
    editDetails()->page()->addComboBox(0, 4, 1, 1, "00", "02", "30", "17"); //
    editDetails()->page()->addComboBox(0, 5, 1, 1, "00", "02", "30", "18"); //
    editDetails()->page()->addKnob(1, 0, 1, 1, "00", "02", "30", "0B", "normal", "bottom", 70);  // distance 1
    editDetails()->page()->addKnob(1, 1, 1, 1, "00", "02", "30", "0C", "normal", "bottom", 70);  // distance 2
    editDetails()->page()->addKnob(1, 2, 1, 1, "00", "02", "30", "0D", "normal", "bottom", 70);  // distance 3
    editDetails()->page()->addKnob(1, 3, 1, 1, "00", "02", "30", "0E", "normal", "bottom", 70);  // distance 4
    editDetails()->page()->addKnob(1, 4, 1, 1, "00", "02", "30", "0F", "normal", "bottom", 70);  // distance 5
    editDetails()->page()->addKnob(1, 5, 1, 1, "00", "02", "30", "10", "normal", "bottom", 70);  // distance 6
    editDetails()->page()->addKnob(2, 0, 1, 1, "00", "02", "30", "11");  // level 1
    editDetails()->page()->addKnob(2, 1, 1, 1, "00", "02", "30", "12");  // level 2
    editDetails()->page()->addKnob(2, 2, 1, 1, "00", "02", "30", "13");  // level 3
    editDetails()->page()->addKnob(2, 3, 1, 1, "00", "02", "30", "14");  // level 4
    editDetails()->page()->addKnob(2, 4, 1, 1, "00", "02", "30", "15");  // level 5
    editDetails()->page()->addKnob(2, 5, 1, 1, "00", "02", "30", "16");  // level 6
    editDetails()->page()->addKnob(4, 0, 1, 1, "00", "02", "30", "1C"); // pu gain
    editDetails()->page()->addComboBox(4, 1, 1, 1, "00", "02", "30", "1B"); //
    editDetails()->page()->addKnob(4, 2, 1, 1, "00", "02", "30", "19"); //
    editDetails()->page()->addKnob(4, 3, 1, 1, "00", "02", "30", "1A"); //
    editDetails()->page()->addSystemOverride(4, 2, 1, 2, "00", "02", "30", "08", "02", "more_than");
    editDetails()->page()->addSystemOverride(4, 4, 1, 2, "00", "02", "30", "08", "03", "less_than");
    editDetails()->page()->addGroupBox(0, 0, 7, 6);
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(0);
    editDetails()->page()->newGroupBox("GK SET 9");
    editDetails()->page()->addNameEdit(0, 0, 1, 1, "00", "02", "40", "00", "8"); //GK catagory
    editDetails()->page()->addComboBox(0, 1, 1, 1, "00", "02", "40", "08"); // GK type
    editDetails()->page()->addDataKnob(0, 2, 1, 2, "00", "02", "40", "09", scale, "right", 120);     //scale
    editDetails()->page()->addComboBox(0, 4, 1, 1, "00", "02", "40", "17"); //
    editDetails()->page()->addComboBox(0, 5, 1, 1, "00", "02", "40", "18"); //
    editDetails()->page()->addKnob(1, 0, 1, 1, "00", "02", "40", "0B", "normal", "bottom", 70);  // distance 1
    editDetails()->page()->addKnob(1, 1, 1, 1, "00", "02", "40", "0C", "normal", "bottom", 70);  // distance 2
    editDetails()->page()->addKnob(1, 2, 1, 1, "00", "02", "40", "0D", "normal", "bottom", 70);  // distance 3
    editDetails()->page()->addKnob(1, 3, 1, 1, "00", "02", "40", "0E", "normal", "bottom", 70);  // distance 4
    editDetails()->page()->addKnob(1, 4, 1, 1, "00", "02", "40", "0F", "normal", "bottom", 70);  // distance 5
    editDetails()->page()->addKnob(1, 5, 1, 1, "00", "02", "40", "10", "normal", "bottom", 70);  // distance 6
    editDetails()->page()->addKnob(2, 0, 1, 1, "00", "02", "40", "11");  // level 1
    editDetails()->page()->addKnob(2, 1, 1, 1, "00", "02", "40", "12");  // level 2
    editDetails()->page()->addKnob(2, 2, 1, 1, "00", "02", "40", "13");  // level 3
    editDetails()->page()->addKnob(2, 3, 1, 1, "00", "02", "40", "14");  // level 4
    editDetails()->page()->addKnob(2, 4, 1, 1, "00", "02", "40", "15");  // level 5
    editDetails()->page()->addKnob(2, 5, 1, 1, "00", "02", "40", "16");  // level 6
    editDetails()->page()->addKnob(4, 0, 1, 1, "00", "02", "40", "1C"); // pu gain
    editDetails()->page()->addComboBox(4, 1, 1, 1, "00", "02", "40", "1B"); //
    editDetails()->page()->addKnob(4, 2, 1, 1, "00", "02", "40", "19"); //
    editDetails()->page()->addKnob(4, 3, 1, 1, "00", "02", "40", "1A"); //
    editDetails()->page()->addSystemOverride(4, 2, 1, 2, "00", "02", "40", "08", "02", "more_than");
    editDetails()->page()->addSystemOverride(4, 4, 1, 2, "00", "02", "40", "08", "03", "less_than");
    editDetails()->page()->addGroupBox(0, 0, 7, 6);
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(0);
    editDetails()->page()->newGroupBox("GK SET 10");
    editDetails()->page()->addNameEdit(0, 0, 1, 1, "00", "02", "50", "00", "8"); //GK catagory
    editDetails()->page()->addComboBox(0, 1, 1, 1, "00", "02", "50", "08"); // GK type
    editDetails()->page()->addDataKnob(0, 2, 1, 2, "00", "02", "50", "09", scale, "right", 120);     //scale
    editDetails()->page()->addComboBox(0, 4, 1, 1, "00", "02", "50", "17"); //
    editDetails()->page()->addComboBox(0, 5, 1, 1, "00", "02", "50", "18"); //
    editDetails()->page()->addKnob(1, 0, 1, 1, "00", "02", "50", "0B", "normal", "bottom", 70);  // distance 1
    editDetails()->page()->addKnob(1, 1, 1, 1, "00", "02", "50", "0C", "normal", "bottom", 70);  // distance 2
    editDetails()->page()->addKnob(1, 2, 1, 1, "00", "02", "50", "0D", "normal", "bottom", 70);  // distance 3
    editDetails()->page()->addKnob(1, 3, 1, 1, "00", "02", "50", "0E", "normal", "bottom", 70);  // distance 4
    editDetails()->page()->addKnob(1, 4, 1, 1, "00", "02", "50", "0F", "normal", "bottom", 70);  // distance 5
    editDetails()->page()->addKnob(1, 5, 1, 1, "00", "02", "50", "10", "normal", "bottom", 70);  // distance 6
    editDetails()->page()->addKnob(2, 0, 1, 1, "00", "02", "50", "11");  // level 1
    editDetails()->page()->addKnob(2, 1, 1, 1, "00", "02", "50", "12");  // level 2
    editDetails()->page()->addKnob(2, 2, 1, 1, "00", "02", "50", "13");  // level 3
    editDetails()->page()->addKnob(2, 3, 1, 1, "00", "02", "50", "14");  // level 4
    editDetails()->page()->addKnob(2, 4, 1, 1, "00", "02", "50", "15");  // level 5
    editDetails()->page()->addKnob(2, 5, 1, 1, "00", "02", "50", "16");  // level 6
    editDetails()->page()->addKnob(4, 0, 1, 1, "00", "02", "50", "1C"); // pu gain
    editDetails()->page()->addComboBox(4, 1, 1, 1, "00", "02", "50", "1B"); //
    editDetails()->page()->addKnob(4, 2, 1, 1, "00", "02", "50", "19"); //
    editDetails()->page()->addKnob(4, 3, 1, 1, "00", "02", "50", "1A"); //
    editDetails()->page()->addSystemOverride(4, 2, 1, 2, "00", "02", "50", "08", "02", "more_than");
    editDetails()->page()->addSystemOverride(4, 4, 1, 2, "00", "02", "50", "08", "03", "less_than");
    editDetails()->page()->addGroupBox(0, 0, 7, 6);
    editDetails()->page()->addStackField();
    }else{
        editDetails()->page()->newStackField(0);
        editDetails()->page()->newGroupBox("GK SET 1");
        editDetails()->page()->addNameEdit(0, 0, 1, 1, "00", "08", "40", "00", "8"); //GK catagory
        editDetails()->page()->addComboBox(0, 1, 1, 1, "00", "08", "40", "08"); // GK type
        editDetails()->page()->addDataKnob(0, 2, 1, 2, "00", "08", "40", "09", scale, "right", 120);     //scale
        editDetails()->page()->addComboBox(0, 4, 1, 1, "00", "08", "40", "18"); // pu phase
        editDetails()->page()->addComboBox(0, 5, 1, 1, "00", "08", "40", "19"); // pu direction
        editDetails()->page()->addKnob(1, 0, 1, 1, "00", "08", "40", "0C", "normal", "bottom", 70);  // distance 1
        editDetails()->page()->addKnob(1, 1, 1, 1, "00", "08", "40", "0D", "normal", "bottom", 70);  // distance 2
        editDetails()->page()->addKnob(1, 2, 1, 1, "00", "08", "40", "0E", "normal", "bottom", 70);  // distance 3
        editDetails()->page()->addKnob(1, 3, 1, 1, "00", "08", "40", "0F", "normal", "bottom", 70);  // distance 4
        editDetails()->page()->addKnob(1, 4, 1, 1, "00", "08", "40", "10", "normal", "bottom", 70);  // distance 5
        editDetails()->page()->addKnob(1, 5, 1, 1, "00", "08", "40", "11", "normal", "bottom", 70);  // distance 6
        editDetails()->page()->addKnob(2, 0, 1, 1, "00", "08", "40", "12");  // level 1
        editDetails()->page()->addKnob(2, 1, 1, 1, "00", "08", "40", "13");  // level 2
        editDetails()->page()->addKnob(2, 2, 1, 1, "00", "08", "40", "14");  // level 3
        editDetails()->page()->addKnob(2, 3, 1, 1, "00", "08", "40", "15");  // level 4
        editDetails()->page()->addKnob(2, 4, 1, 1, "00", "08", "40", "16");  // level 5
        editDetails()->page()->addKnob(2, 5, 1, 1, "00", "08", "40", "17");  // level 6
        editDetails()->page()->addKnob(4, 0, 1, 1, "00", "08", "40", "1D"); // pu gain
        editDetails()->page()->addComboBox(4, 1, 1, 1, "00", "08", "40", "0B"); // pu position
        editDetails()->page()->addComboBox(4, 2, 1, 1, "00", "08", "40", "1C"); // s1/s2 position
        editDetails()->page()->addKnob(4, 3, 1, 1, "00", "08", "40", "1A"); // piezo low
        editDetails()->page()->addKnob(4, 4, 1, 1, "00", "08", "40", "1B"); // piezo high
        editDetails()->page()->addSystemOverride(4, 3, 1, 2, "00", "08", "40", "08", "02", "more_than");
        editDetails()->page()->addSystemOverride(4, 5, 1, 1, "00", "08", "40", "08", "03", "less_than");
        editDetails()->page()->addGroupBox(0, 0, 7, 6);
        editDetails()->page()->addStackField();

        editDetails()->page()->newStackField(0);
        editDetails()->page()->newGroupBox("GK SET 2");
        editDetails()->page()->addNameEdit(0, 0, 1, 1, "00", "08", "50", "00", "8"); //GK catagory
        editDetails()->page()->addComboBox(0, 1, 1, 1, "00", "08", "50", "08"); // GK type
        editDetails()->page()->addDataKnob(0, 2, 1, 2, "00", "08", "50", "09", scale, "right", 120);     //scale
        editDetails()->page()->addComboBox(0, 4, 1, 1, "00", "08", "50", "18"); // pu phase
        editDetails()->page()->addComboBox(0, 5, 1, 1, "00", "08", "50", "19"); // pu direction
        editDetails()->page()->addKnob(1, 0, 1, 1, "00", "08", "50", "0C", "normal", "bottom", 70);  // distance 1
        editDetails()->page()->addKnob(1, 1, 1, 1, "00", "08", "50", "0D", "normal", "bottom", 70);  // distance 2
        editDetails()->page()->addKnob(1, 2, 1, 1, "00", "08", "50", "0E", "normal", "bottom", 70);  // distance 3
        editDetails()->page()->addKnob(1, 3, 1, 1, "00", "08", "50", "0F", "normal", "bottom", 70);  // distance 4
        editDetails()->page()->addKnob(1, 4, 1, 1, "00", "08", "50", "10", "normal", "bottom", 70);  // distance 5
        editDetails()->page()->addKnob(1, 5, 1, 1, "00", "08", "50", "11", "normal", "bottom", 70);  // distance 6
        editDetails()->page()->addKnob(2, 0, 1, 1, "00", "08", "50", "12");  // level 1
        editDetails()->page()->addKnob(2, 1, 1, 1, "00", "08", "50", "13");  // level 2
        editDetails()->page()->addKnob(2, 2, 1, 1, "00", "08", "50", "14");  // level 3
        editDetails()->page()->addKnob(2, 3, 1, 1, "00", "08", "50", "15");  // level 4
        editDetails()->page()->addKnob(2, 4, 1, 1, "00", "08", "50", "16");  // level 5
        editDetails()->page()->addKnob(2, 5, 1, 1, "00", "08", "50", "17");  // level 6
        editDetails()->page()->addKnob(4, 0, 1, 1, "00", "08", "50", "1D"); // pu gain
        editDetails()->page()->addComboBox(4, 1, 1, 1, "00", "08", "50", "0B"); // pu position
        editDetails()->page()->addComboBox(4, 2, 1, 1, "00", "08", "50", "1C"); // s1/s2 position
        editDetails()->page()->addKnob(4, 3, 1, 1, "00", "08", "50", "1A"); // piezo low
        editDetails()->page()->addKnob(4, 4, 1, 1, "00", "08", "50", "1B"); // piezo high
        editDetails()->page()->addSystemOverride(4, 3, 1, 2, "00", "08", "50", "08", "02", "more_than");
        editDetails()->page()->addSystemOverride(4, 5, 1, 1, "00", "08", "50", "08", "03", "less_than");
        editDetails()->page()->addGroupBox(0, 0, 7, 6);
        editDetails()->page()->addStackField();

        editDetails()->page()->newStackField(0);
        editDetails()->page()->newGroupBox("GK SET 3");
        editDetails()->page()->addNameEdit(0, 0, 1, 1, "00", "08", "60", "00", "8"); //GK catagory
        editDetails()->page()->addComboBox(0, 1, 1, 1, "00", "08", "60", "08"); // GK type
        editDetails()->page()->addDataKnob(0, 2, 1, 2, "00", "08", "60", "09", scale, "right", 120);     //scale
        editDetails()->page()->addComboBox(0, 4, 1, 1, "00", "08", "60", "18"); // pu phase
        editDetails()->page()->addComboBox(0, 5, 1, 1, "00", "08", "60", "19"); // pu direction
        editDetails()->page()->addKnob(1, 0, 1, 1, "00", "08", "60", "0C", "normal", "bottom", 70);  // distance 1
        editDetails()->page()->addKnob(1, 1, 1, 1, "00", "08", "60", "0D", "normal", "bottom", 70);  // distance 2
        editDetails()->page()->addKnob(1, 2, 1, 1, "00", "08", "60", "0E", "normal", "bottom", 70);  // distance 3
        editDetails()->page()->addKnob(1, 3, 1, 1, "00", "08", "60", "0F", "normal", "bottom", 70);  // distance 4
        editDetails()->page()->addKnob(1, 4, 1, 1, "00", "08", "60", "10", "normal", "bottom", 70);  // distance 5
        editDetails()->page()->addKnob(1, 5, 1, 1, "00", "08", "60", "11", "normal", "bottom", 70);  // distance 6
        editDetails()->page()->addKnob(2, 0, 1, 1, "00", "08", "60", "12");  // level 1
        editDetails()->page()->addKnob(2, 1, 1, 1, "00", "08", "60", "13");  // level 2
        editDetails()->page()->addKnob(2, 2, 1, 1, "00", "08", "60", "14");  // level 3
        editDetails()->page()->addKnob(2, 3, 1, 1, "00", "08", "60", "15");  // level 4
        editDetails()->page()->addKnob(2, 4, 1, 1, "00", "08", "60", "16");  // level 5
        editDetails()->page()->addKnob(2, 5, 1, 1, "00", "08", "60", "17");  // level 6
        editDetails()->page()->addKnob(4, 0, 1, 1, "00", "08", "60", "1D"); // pu gain
        editDetails()->page()->addComboBox(4, 1, 1, 1, "00", "08", "60", "0B"); // pu position
        editDetails()->page()->addComboBox(4, 2, 1, 1, "00", "08", "60", "1C"); // s1/s2 position
        editDetails()->page()->addKnob(4, 3, 1, 1, "00", "08", "60", "1A"); // piezo low
        editDetails()->page()->addKnob(4, 4, 1, 1, "00", "08", "60", "1B"); // piezo high
        editDetails()->page()->addSystemOverride(4, 3, 1, 2, "00", "08", "60", "08", "02", "more_than");
        editDetails()->page()->addSystemOverride(4, 5, 1, 1, "00", "08", "60", "08", "03", "less_than");
        editDetails()->page()->addGroupBox(0, 0, 7, 6);
        editDetails()->page()->addStackField();

        editDetails()->page()->newStackField(0);
        editDetails()->page()->newGroupBox("GK SET 4");
        editDetails()->page()->addNameEdit(0, 0, 1, 1, "00", "08", "70", "00", "8"); //GK catagory
        editDetails()->page()->addComboBox(0, 1, 1, 1, "00", "08", "70", "08"); // GK type
        editDetails()->page()->addDataKnob(0, 2, 1, 2, "00", "08", "70", "09", scale, "right", 120);     //scale
        editDetails()->page()->addComboBox(0, 4, 1, 1, "00", "08", "70", "18"); // pu phase
        editDetails()->page()->addComboBox(0, 5, 1, 1, "00", "08", "70", "19"); // pu direction
        editDetails()->page()->addKnob(1, 0, 1, 1, "00", "08", "70", "0C", "normal", "bottom", 70);  // distance 1
        editDetails()->page()->addKnob(1, 1, 1, 1, "00", "08", "70", "0D", "normal", "bottom", 70);  // distance 2
        editDetails()->page()->addKnob(1, 2, 1, 1, "00", "08", "70", "0E", "normal", "bottom", 70);  // distance 3
        editDetails()->page()->addKnob(1, 3, 1, 1, "00", "08", "70", "0F", "normal", "bottom", 70);  // distance 4
        editDetails()->page()->addKnob(1, 4, 1, 1, "00", "08", "70", "10", "normal", "bottom", 70);  // distance 5
        editDetails()->page()->addKnob(1, 5, 1, 1, "00", "08", "70", "11", "normal", "bottom", 70);  // distance 6
        editDetails()->page()->addKnob(2, 0, 1, 1, "00", "08", "70", "12");  // level 1
        editDetails()->page()->addKnob(2, 1, 1, 1, "00", "08", "70", "13");  // level 2
        editDetails()->page()->addKnob(2, 2, 1, 1, "00", "08", "70", "14");  // level 3
        editDetails()->page()->addKnob(2, 3, 1, 1, "00", "08", "70", "15");  // level 4
        editDetails()->page()->addKnob(2, 4, 1, 1, "00", "08", "70", "16");  // level 5
        editDetails()->page()->addKnob(2, 5, 1, 1, "00", "08", "70", "17");  // level 6
        editDetails()->page()->addKnob(4, 0, 1, 1, "00", "08", "70", "1D");  // pu gain
        editDetails()->page()->addComboBox(4, 1, 1, 1, "00", "08", "70", "0B"); // pu position
        editDetails()->page()->addComboBox(4, 2, 1, 1, "00", "08", "70", "1C"); // s1/s2 position
        editDetails()->page()->addKnob(4, 3, 1, 1, "00", "08", "70", "1A"); // piezo low
        editDetails()->page()->addKnob(4, 4, 1, 1, "00", "08", "70", "1B"); // piezo high
        editDetails()->page()->addSystemOverride(4, 3, 1, 2, "00", "08", "70", "08", "02", "more_than");
        editDetails()->page()->addSystemOverride(4, 5, 1, 1, "00", "08", "70", "08", "03", "less_than");
        editDetails()->page()->addGroupBox(0, 0, 7, 6);
        editDetails()->page()->addStackField();

        editDetails()->page()->newStackField(0);
        editDetails()->page()->newGroupBox("GK SET 5");
        editDetails()->page()->addNameEdit(0, 0, 1, 1, "00", "09", "00", "00", "8"); //GK catagory
        editDetails()->page()->addComboBox(0, 1, 1, 1, "00", "09", "00", "08"); // GK type
        editDetails()->page()->addDataKnob(0, 2, 1, 2, "00", "09", "00", "09", scale, "right", 120);     //scale
        editDetails()->page()->addComboBox(0, 4, 1, 1, "00", "09", "00", "18"); // pu phase
        editDetails()->page()->addComboBox(0, 5, 1, 1, "00", "09", "00", "19"); // pu direction
        editDetails()->page()->addKnob(1, 0, 1, 1, "00", "09", "00", "0C", "normal", "bottom", 70);  // distance 1
        editDetails()->page()->addKnob(1, 1, 1, 1, "00", "09", "00", "0D", "normal", "bottom", 70);  // distance 2
        editDetails()->page()->addKnob(1, 2, 1, 1, "00", "09", "00", "0E", "normal", "bottom", 70);  // distance 3
        editDetails()->page()->addKnob(1, 3, 1, 1, "00", "09", "00", "0F", "normal", "bottom", 70);  // distance 4
        editDetails()->page()->addKnob(1, 4, 1, 1, "00", "09", "00", "10", "normal", "bottom", 70);  // distance 5
        editDetails()->page()->addKnob(1, 5, 1, 1, "00", "09", "00", "11", "normal", "bottom", 70);  // distance 6
        editDetails()->page()->addKnob(2, 0, 1, 1, "00", "09", "00", "12");  // level 1
        editDetails()->page()->addKnob(2, 1, 1, 1, "00", "09", "00", "13");  // level 2
        editDetails()->page()->addKnob(2, 2, 1, 1, "00", "09", "00", "14");  // level 3
        editDetails()->page()->addKnob(2, 3, 1, 1, "00", "09", "00", "15");  // level 4
        editDetails()->page()->addKnob(2, 4, 1, 1, "00", "09", "00", "16");  // level 5
        editDetails()->page()->addKnob(2, 5, 1, 1, "00", "09", "00", "17");  // level 6
        editDetails()->page()->addKnob(4, 0, 1, 1, "00", "09", "00", "1D"); // pu gain
        editDetails()->page()->addComboBox(4, 1, 1, 1, "00", "09", "00", "0B"); // pu position
        editDetails()->page()->addComboBox(4, 2, 1, 1, "00", "09", "00", "1C"); // s1/s2 position
        editDetails()->page()->addKnob(4, 3, 1, 1, "00", "09", "00", "1A"); // piezo low
        editDetails()->page()->addKnob(4, 4, 1, 1, "00", "09", "00", "1B"); // piezo high
        editDetails()->page()->addSystemOverride(4, 3, 1, 2, "00", "09", "00", "08", "02", "more_than");
        editDetails()->page()->addSystemOverride(4, 5, 1, 1, "00", "09", "00", "08", "03", "less_than");
        editDetails()->page()->addGroupBox(0, 0, 7, 6);
        editDetails()->page()->addStackField();

        editDetails()->page()->newStackField(0);
        editDetails()->page()->newGroupBox("GK SET 6");
        editDetails()->page()->addNameEdit(0, 0, 1, 1, "00", "09", "10", "00", "8"); //GK catagory
        editDetails()->page()->addComboBox(0, 1, 1, 1, "00", "09", "10", "08"); // GK type
        editDetails()->page()->addDataKnob(0, 2, 1, 2, "00", "09", "10", "09", scale, "right", 120);     //scale
        editDetails()->page()->addComboBox(0, 4, 1, 1, "00", "09", "10", "18"); // pu phase
        editDetails()->page()->addComboBox(0, 5, 1, 1, "00", "09", "10", "19"); // pu direction
        editDetails()->page()->addKnob(1, 0, 1, 1, "00", "09", "10", "0C", "normal", "bottom", 70);  // distance 1
        editDetails()->page()->addKnob(1, 1, 1, 1, "00", "09", "10", "0D", "normal", "bottom", 70);  // distance 2
        editDetails()->page()->addKnob(1, 2, 1, 1, "00", "09", "10", "0E", "normal", "bottom", 70);  // distance 3
        editDetails()->page()->addKnob(1, 3, 1, 1, "00", "09", "10", "0F", "normal", "bottom", 70);  // distance 4
        editDetails()->page()->addKnob(1, 4, 1, 1, "00", "09", "10", "10", "normal", "bottom", 70);  // distance 5
        editDetails()->page()->addKnob(1, 5, 1, 1, "00", "09", "10", "11", "normal", "bottom", 70);  // distance 6
        editDetails()->page()->addKnob(2, 0, 1, 1, "00", "09", "10", "12");  // level 1
        editDetails()->page()->addKnob(2, 1, 1, 1, "00", "09", "10", "13");  // level 2
        editDetails()->page()->addKnob(2, 2, 1, 1, "00", "09", "10", "14");  // level 3
        editDetails()->page()->addKnob(2, 3, 1, 1, "00", "09", "10", "15");  // level 4
        editDetails()->page()->addKnob(2, 4, 1, 1, "00", "09", "10", "16");  // level 5
        editDetails()->page()->addKnob(2, 5, 1, 1, "00", "09", "10", "17");  // level 6
        editDetails()->page()->addKnob(4, 0, 1, 1, "00", "09", "10", "1D"); // pu gain
        editDetails()->page()->addComboBox(4, 1, 1, 1, "00", "09", "10", "0B"); // pu position
        editDetails()->page()->addComboBox(4, 2, 1, 1, "00", "09", "10", "1C"); // s1/s2 position
        editDetails()->page()->addKnob(4, 3, 1, 1, "00", "09", "10", "1A"); // piezo low
        editDetails()->page()->addKnob(4, 4, 1, 1, "00", "09", "10", "1B"); // piezo high
        editDetails()->page()->addSystemOverride(4, 3, 1, 2, "00", "09", "10", "08", "02", "more_than");
        editDetails()->page()->addSystemOverride(4, 5, 1, 1, "00", "09", "10", "08", "03", "less_than");
        editDetails()->page()->addGroupBox(0, 0, 7, 6);
        editDetails()->page()->addStackField();

        editDetails()->page()->newStackField(0);
        editDetails()->page()->newGroupBox("GK SET 7");
        editDetails()->page()->addNameEdit(0, 0, 1, 1, "00", "09", "20", "00", "8"); //GK catagory
        editDetails()->page()->addComboBox(0, 1, 1, 1, "00", "09", "20", "08"); // GK type
        editDetails()->page()->addDataKnob(0, 2, 1, 2, "00", "09", "20", "09", scale, "right", 120);     //scale
        editDetails()->page()->addComboBox(0, 4, 1, 1, "00", "09", "20", "18"); // pu phase
        editDetails()->page()->addComboBox(0, 5, 1, 1, "00", "09", "20", "19"); // pu direction
        editDetails()->page()->addKnob(1, 0, 1, 1, "00", "09", "20", "0C", "normal", "bottom", 70);  // distance 1
        editDetails()->page()->addKnob(1, 1, 1, 1, "00", "09", "20", "0D", "normal", "bottom", 70);  // distance 2
        editDetails()->page()->addKnob(1, 2, 1, 1, "00", "09", "20", "0E", "normal", "bottom", 70);  // distance 3
        editDetails()->page()->addKnob(1, 3, 1, 1, "00", "09", "20", "0F", "normal", "bottom", 70);  // distance 4
        editDetails()->page()->addKnob(1, 4, 1, 1, "00", "09", "20", "10", "normal", "bottom", 70);  // distance 5
        editDetails()->page()->addKnob(1, 5, 1, 1, "00", "09", "20", "11", "normal", "bottom", 70);  // distance 6
        editDetails()->page()->addKnob(2, 0, 1, 1, "00", "09", "20", "12");  // level 1
        editDetails()->page()->addKnob(2, 1, 1, 1, "00", "09", "20", "13");  // level 2
        editDetails()->page()->addKnob(2, 2, 1, 1, "00", "09", "20", "14");  // level 3
        editDetails()->page()->addKnob(2, 3, 1, 1, "00", "09", "20", "15");  // level 4
        editDetails()->page()->addKnob(2, 4, 1, 1, "00", "09", "20", "16");  // level 5
        editDetails()->page()->addKnob(2, 5, 1, 1, "00", "09", "20", "17");  // level 6
        editDetails()->page()->addKnob(4, 0, 1, 1, "00", "09", "20", "1D"); // pu gain
        editDetails()->page()->addComboBox(4, 1, 1, 1, "00", "09", "20", "0B"); // pu position
        editDetails()->page()->addComboBox(4, 2, 1, 1, "00", "09", "20", "1C"); // s1/s2 position
        editDetails()->page()->addKnob(4, 3, 1, 1, "00", "09", "20", "1A"); // piezo low
        editDetails()->page()->addKnob(4, 4, 1, 1, "00", "09", "20", "1B"); // piezo high
        editDetails()->page()->addSystemOverride(4, 3, 1, 2, "00", "09", "20", "08", "02", "more_than");
        editDetails()->page()->addSystemOverride(4, 5, 1, 1, "00", "09", "20", "08", "03", "less_than");
        editDetails()->page()->addGroupBox(0, 0, 7, 6);
        editDetails()->page()->addStackField();


        editDetails()->page()->newStackField(0);
        editDetails()->page()->newGroupBox("GK SET 8");
        editDetails()->page()->addNameEdit(0, 0, 1, 1, "00", "09", "30", "00", "8"); //GK catagory
        editDetails()->page()->addComboBox(0, 1, 1, 1, "00", "09", "30", "08"); // GK type
        editDetails()->page()->addDataKnob(0, 2, 1, 2, "00", "09", "30", "09", scale, "right", 120);     //scale
        editDetails()->page()->addComboBox(0, 4, 1, 1, "00", "09", "30", "18"); // pu phase
        editDetails()->page()->addComboBox(0, 5, 1, 1, "00", "09", "30", "19"); // pu direction
        editDetails()->page()->addKnob(1, 0, 1, 1, "00", "09", "30", "0C", "normal", "bottom", 70);  // distance 1
        editDetails()->page()->addKnob(1, 1, 1, 1, "00", "09", "30", "0D", "normal", "bottom", 70);  // distance 2
        editDetails()->page()->addKnob(1, 2, 1, 1, "00", "09", "30", "0E", "normal", "bottom", 70);  // distance 3
        editDetails()->page()->addKnob(1, 3, 1, 1, "00", "09", "30", "0F", "normal", "bottom", 70);  // distance 4
        editDetails()->page()->addKnob(1, 4, 1, 1, "00", "09", "30", "10", "normal", "bottom", 70);  // distance 5
        editDetails()->page()->addKnob(1, 5, 1, 1, "00", "09", "30", "11", "normal", "bottom", 70);  // distance 6
        editDetails()->page()->addKnob(2, 0, 1, 1, "00", "09", "30", "12");  // level 1
        editDetails()->page()->addKnob(2, 1, 1, 1, "00", "09", "30", "13");  // level 2
        editDetails()->page()->addKnob(2, 2, 1, 1, "00", "09", "30", "14");  // level 3
        editDetails()->page()->addKnob(2, 3, 1, 1, "00", "09", "30", "15");  // level 4
        editDetails()->page()->addKnob(2, 4, 1, 1, "00", "09", "30", "16");  // level 5
        editDetails()->page()->addKnob(2, 5, 1, 1, "00", "09", "30", "17");  // level 6
        editDetails()->page()->addKnob(4, 0, 1, 1, "00", "09", "30", "1D"); // pu gain
        editDetails()->page()->addComboBox(4, 1, 1, 1, "00", "09", "30", "0B"); // pu position
        editDetails()->page()->addComboBox(4, 2, 1, 1, "00", "09", "30", "1C"); // s1/s2 position
        editDetails()->page()->addKnob(4, 3, 1, 1, "00", "09", "30", "1A"); // piezo low
        editDetails()->page()->addKnob(4, 4, 1, 1, "00", "09", "30", "1B"); // piezo high
        editDetails()->page()->addSystemOverride(4, 3, 1, 2, "00", "09", "30", "08", "02", "more_than");
        editDetails()->page()->addSystemOverride(4, 5, 1, 1, "00", "09", "30", "08", "03", "less_than");
        editDetails()->page()->addGroupBox(0, 0, 7, 6);
        editDetails()->page()->addStackField();

        editDetails()->page()->newStackField(0);
        editDetails()->page()->newGroupBox("GK SET 9");
        editDetails()->page()->addNameEdit(0, 0, 1, 1, "00", "09", "40", "00", "8"); //GK catagory
        editDetails()->page()->addComboBox(0, 1, 1, 1, "00", "09", "40", "08"); // GK type
        editDetails()->page()->addDataKnob(0, 2, 1, 2, "00", "09", "40", "09", scale, "right", 120);     //scale
        editDetails()->page()->addComboBox(0, 4, 1, 1, "00", "09", "40", "18"); // pu phase
        editDetails()->page()->addComboBox(0, 5, 1, 1, "00", "09", "40", "19"); // pu direction
        editDetails()->page()->addKnob(1, 0, 1, 1, "00", "09", "40", "0C", "normal", "bottom", 70);  // distance 1
        editDetails()->page()->addKnob(1, 1, 1, 1, "00", "09", "40", "0D", "normal", "bottom", 70);  // distance 2
        editDetails()->page()->addKnob(1, 2, 1, 1, "00", "09", "40", "0E", "normal", "bottom", 70);  // distance 3
        editDetails()->page()->addKnob(1, 3, 1, 1, "00", "09", "40", "0F", "normal", "bottom", 70);  // distance 4
        editDetails()->page()->addKnob(1, 4, 1, 1, "00", "09", "40", "10", "normal", "bottom", 70);  // distance 5
        editDetails()->page()->addKnob(1, 5, 1, 1, "00", "09", "40", "11", "normal", "bottom", 70);  // distance 6
        editDetails()->page()->addKnob(2, 0, 1, 1, "00", "09", "40", "12");  // level 1
        editDetails()->page()->addKnob(2, 1, 1, 1, "00", "09", "40", "13");  // level 2
        editDetails()->page()->addKnob(2, 2, 1, 1, "00", "09", "40", "14");  // level 3
        editDetails()->page()->addKnob(2, 3, 1, 1, "00", "09", "40", "15");  // level 4
        editDetails()->page()->addKnob(2, 4, 1, 1, "00", "09", "40", "16");  // level 5
        editDetails()->page()->addKnob(2, 5, 1, 1, "00", "09", "40", "17");  // level 6
        editDetails()->page()->addKnob(4, 0, 1, 1, "00", "09", "40", "1D"); // pu gain
        editDetails()->page()->addComboBox(4, 1, 1, 1, "00", "09", "40", "0B"); // pu position
        editDetails()->page()->addComboBox(4, 2, 1, 1, "00", "09", "40", "1C"); // s1/s2 position
        editDetails()->page()->addKnob(4, 3, 1, 1, "00", "09", "40", "1A"); // piezo low
        editDetails()->page()->addKnob(4, 4, 1, 1, "00", "09", "40", "1B"); // piezo high
        editDetails()->page()->addSystemOverride(4, 3, 1, 2, "00", "09", "40", "08", "02", "more_than");
        editDetails()->page()->addSystemOverride(4, 5, 1, 1, "00", "09", "40", "08", "03", "less_than");
        editDetails()->page()->addGroupBox(0, 0, 7, 6);
        editDetails()->page()->addStackField();

        editDetails()->page()->newStackField(0);
        editDetails()->page()->newGroupBox("GK SET 10");
        editDetails()->page()->addNameEdit(0, 0, 1, 1, "00", "09", "50", "00", "8"); //GK catagory
        editDetails()->page()->addComboBox(0, 1, 1, 1, "00", "09", "50", "08"); // GK type
        editDetails()->page()->addDataKnob(0, 2, 1, 2, "00", "09", "50", "09", scale, "right", 120);     //scale
        editDetails()->page()->addComboBox(0, 4, 1, 1, "00", "09", "50", "18"); // pu phase
        editDetails()->page()->addComboBox(0, 5, 1, 1, "00", "09", "50", "19"); // pu direction
        editDetails()->page()->addKnob(1, 0, 1, 1, "00", "09", "50", "0C", "normal", "bottom", 70);  // distance 1
        editDetails()->page()->addKnob(1, 1, 1, 1, "00", "09", "50", "0D", "normal", "bottom", 70);  // distance 2
        editDetails()->page()->addKnob(1, 2, 1, 1, "00", "09", "50", "0E", "normal", "bottom", 70);  // distance 3
        editDetails()->page()->addKnob(1, 3, 1, 1, "00", "09", "50", "0F", "normal", "bottom", 70);  // distance 4
        editDetails()->page()->addKnob(1, 4, 1, 1, "00", "09", "50", "10", "normal", "bottom", 70);  // distance 5
        editDetails()->page()->addKnob(1, 5, 1, 1, "00", "09", "50", "11", "normal", "bottom", 70);  // distance 6
        editDetails()->page()->addKnob(2, 0, 1, 1, "00", "09", "50", "12");  // level 1
        editDetails()->page()->addKnob(2, 1, 1, 1, "00", "09", "50", "13");  // level 2
        editDetails()->page()->addKnob(2, 2, 1, 1, "00", "09", "50", "14");  // level 3
        editDetails()->page()->addKnob(2, 3, 1, 1, "00", "09", "50", "15");  // level 4
        editDetails()->page()->addKnob(2, 4, 1, 1, "00", "09", "50", "16");  // level 5
        editDetails()->page()->addKnob(2, 5, 1, 1, "00", "09", "50", "17");  // level 6
        editDetails()->page()->addKnob(4, 0, 1, 1, "00", "09", "50", "1D"); // pu gain
        editDetails()->page()->addComboBox(4, 1, 1, 1, "00", "09", "50", "0B"); // pu position
        editDetails()->page()->addComboBox(4, 2, 1, 1, "00", "09", "50", "1C"); // s1/s2 position
        editDetails()->page()->addKnob(4, 3, 1, 1, "00", "09", "50", "1A"); // piezo low
        editDetails()->page()->addKnob(4, 4, 1, 1, "00", "09", "50", "1B"); // piezo high
        editDetails()->page()->addSystemOverride(4, 3, 1, 2, "00", "09", "50", "08", "02", "more_than");
        editDetails()->page()->addSystemOverride(4, 5, 1, 1, "00", "09", "50", "08", "03", "less_than");
        editDetails()->page()->addGroupBox(0, 0, 7, 6);
        editDetails()->page()->addStackField();
    }


    editDetails()->addPage("Tables", "00", "02", "01", "03");       // ******************************************************************

    hex1 = "01";
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){hex1 = "08";};

    editDetails()->page()->newGroupBox("System Normal Set Select");
    editDetails()->page()->addComboBox(1, 0, 1, 1, "00", hex1, "00", "27", "large"); // Normal Set
    editDetails()->page()->addGroupBox(0, 0, 1, 1);
    hex1 = "01";
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){hex1 = "08";};

    editDetails()->page()->newGroupBox("System Normal-Set Settings");
    editDetails()->page()->newStackControl(0);
    editDetails()->page()->addTabBar(0, 0, 1, 1, "Tables", "00", "02", "00"); //edit page select Normal SET
    editDetails()->page()->addStackControl();
    editDetails()->page()->insertStackField(0, 1, 0, 1, 1);
    editDetails()->page()->addGroupBox(0, 1, 1, 1);

    hex1 = "02";
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){hex1 = "09";};

    editDetails()->page()->newStackField(0);
    editDetails()->page()->newGroupBox("Normal SET 1");
    editDetails()->page()->addNameEdit(0, 0, 1, 2, "00", hex1, "60", "00", "8"); // Name
    editDetails()->page()->addSwitch(1, 0, 1, 1, "00", hex1, "60", "08"); // comp off/on
    editDetails()->page()->addKnob(1, 1, 1, 1, "00", hex1, "60", "09");  // attack
    editDetails()->page()->addKnob(1, 2, 1, 1, "00", hex1, "60", "0A");  // sustain
    editDetails()->page()->addKnob(1, 3, 1, 1, "00", hex1, "60", "0B");  // level
    editDetails()->page()->addKnob(2, 1, 1, 1, "00", hex1, "60", "0C");  // sense
    editDetails()->page()->addKnob(2, 2, 1, 1, "00", hex1, "60", "0D");  // low
    editDetails()->page()->addKnob(2, 3, 1, 1, "00", hex1, "60", "0E");  // high
    editDetails()->page()->addGroupBox(0, 0, 7, 6);
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(0);
    editDetails()->page()->newGroupBox("Normal SET 2");
    editDetails()->page()->addNameEdit(0, 0, 1, 2, "00", hex1, "70", "00", "8"); // Name
    editDetails()->page()->addSwitch(1, 0, 1, 1, "00", hex1, "70", "08"); // comp off/on
    editDetails()->page()->addKnob(1, 1, 1, 1, "00", hex1, "70", "09");  // attack
    editDetails()->page()->addKnob(1, 2, 1, 1, "00", hex1, "70", "0A");  // sustain
    editDetails()->page()->addKnob(1, 3, 1, 1, "00", hex1, "70", "0B");  // level
    editDetails()->page()->addKnob(2, 1, 1, 1, "00", hex1, "70", "0C");  // sense
    editDetails()->page()->addKnob(2, 2, 1, 1, "00", hex1, "70", "0D");  // low
    editDetails()->page()->addKnob(2, 3, 1, 1, "00", hex1, "70", "0E");  // high
    editDetails()->page()->addGroupBox(0, 0, 7, 6);
    editDetails()->page()->addStackField();

    hex1 = "03";
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){hex1 = "0A";};

    editDetails()->page()->newStackField(0);
    editDetails()->page()->newGroupBox("Normal SET 3");
    editDetails()->page()->addNameEdit(0, 0, 1, 2, "00", hex1, "00", "00", "8"); // Name
    editDetails()->page()->addSwitch(1, 0, 1, 1, "00", hex1, "00", "08"); // comp off/on
    editDetails()->page()->addKnob(1, 1, 1, 1, "00", hex1, "00", "09");  // attack
    editDetails()->page()->addKnob(1, 2, 1, 1, "00", hex1, "00", "0A");  // sustain
    editDetails()->page()->addKnob(1, 3, 1, 1, "00", hex1, "00", "0B");  // level
    editDetails()->page()->addKnob(2, 1, 1, 1, "00", hex1, "00", "0C");  // sense
    editDetails()->page()->addKnob(2, 2, 1, 1, "00", hex1, "00", "0D");  // low
    editDetails()->page()->addKnob(2, 3, 1, 1, "00", hex1, "00", "0E");  // high
    editDetails()->page()->addGroupBox(0, 0, 7, 6);
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(0);
    editDetails()->page()->newGroupBox("Normal SET 4");
    editDetails()->page()->addNameEdit(0, 0, 1, 2, "00", hex1, "10", "00", "8"); // Name
    editDetails()->page()->addSwitch(1, 0, 1, 1, "00", hex1, "10", "08"); // comp off/on
    editDetails()->page()->addKnob(1, 1, 1, 1, "00", hex1, "10", "09");  // attack
    editDetails()->page()->addKnob(1, 2, 1, 1, "00", hex1, "10", "0A");  // sustain
    editDetails()->page()->addKnob(1, 3, 1, 1, "00", hex1, "10", "0B");  // level
    editDetails()->page()->addKnob(2, 1, 1, 1, "00", hex1, "10", "0C");  // sense
    editDetails()->page()->addKnob(2, 2, 1, 1, "00", hex1, "10", "0D");  // low
    editDetails()->page()->addKnob(2, 3, 1, 1, "00", hex1, "10", "0E");  // high
    editDetails()->page()->addGroupBox(0, 0, 7, 6);
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(0);
    editDetails()->page()->newGroupBox("Normal SET 5");
    editDetails()->page()->addNameEdit(0, 0, 1, 2, "00", hex1, "20", "00", "8"); // Name
    editDetails()->page()->addSwitch(1, 0, 1, 1, "00", hex1, "20", "08"); // comp off/on
    editDetails()->page()->addKnob(1, 1, 1, 1, "00", hex1, "20", "09");  // attack
    editDetails()->page()->addKnob(1, 2, 1, 1, "00", hex1, "20", "0A");  // sustain
    editDetails()->page()->addKnob(1, 3, 1, 1, "00", hex1, "20", "0B");  // level
    editDetails()->page()->addKnob(2, 1, 1, 1, "00", hex1, "20", "0C");  // sense
    editDetails()->page()->addKnob(2, 2, 1, 1, "00", hex1, "20", "0D");  // low
    editDetails()->page()->addKnob(2, 3, 1, 1, "00", hex1, "20", "0E");  // high
    editDetails()->page()->addGroupBox(0, 0, 7, 6);
    editDetails()->page()->addStackField();


    editDetails()->page()->newStackField(0);
    editDetails()->page()->newGroupBox("Normal SET 6");
    editDetails()->page()->addNameEdit(0, 0, 1, 2, "00", hex1, "30", "00", "8"); // Name
    editDetails()->page()->addSwitch(1, 0, 1, 1, "00", hex1, "30", "08"); // comp off/on
    editDetails()->page()->addKnob(1, 1, 1, 1, "00", hex1, "30", "09");  // attack
    editDetails()->page()->addKnob(1, 2, 1, 1, "00", hex1, "30", "0A");  // sustain
    editDetails()->page()->addKnob(1, 3, 1, 1, "00", hex1, "30", "0B");  // level
    editDetails()->page()->addKnob(2, 1, 1, 1, "00", hex1, "30", "0C");  // sense
    editDetails()->page()->addKnob(2, 2, 1, 1, "00", hex1, "30", "0D");  // low
    editDetails()->page()->addKnob(2, 3, 1, 1, "00", hex1, "30", "0E");  // high
    editDetails()->page()->addGroupBox(0, 0, 7, 6);
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(0);
    editDetails()->page()->newGroupBox("Normal SET 7");
    editDetails()->page()->addNameEdit(0, 0, 1, 2, "00", hex1, "40", "00", "8"); // Name
    editDetails()->page()->addSwitch(1, 0, 1, 1, "00", hex1, "40", "08"); // comp off/on
    editDetails()->page()->addKnob(1, 1, 1, 1, "00", hex1, "40", "09");  // attack
    editDetails()->page()->addKnob(1, 2, 1, 1, "00", hex1, "40", "0A");  // sustain
    editDetails()->page()->addKnob(1, 3, 1, 1, "00", hex1, "40", "0B");  // level
    editDetails()->page()->addKnob(2, 1, 1, 1, "00", hex1, "40", "0C");  // sense
    editDetails()->page()->addKnob(2, 2, 1, 1, "00", hex1, "40", "0D");  // low
    editDetails()->page()->addKnob(2, 3, 1, 1, "00", hex1, "40", "0E");  // high
    editDetails()->page()->addGroupBox(0, 0, 7, 6);
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(0);
    editDetails()->page()->newGroupBox("Normal SET 8");
    editDetails()->page()->addNameEdit(0, 0, 1, 2, "00", hex1, "50", "00", "8"); // Name
    editDetails()->page()->addSwitch(1, 0, 1, 1, "00", hex1, "50", "08"); // comp off/on
    editDetails()->page()->addKnob(1, 1, 1, 1, "00", hex1, "50", "09");  // attack
    editDetails()->page()->addKnob(1, 2, 1, 1, "00", hex1, "50", "0A");  // sustain
    editDetails()->page()->addKnob(1, 3, 1, 1, "00", hex1, "50", "0B");  // level
    editDetails()->page()->addKnob(2, 1, 1, 1, "00", hex1, "50", "0C");  // sense
    editDetails()->page()->addKnob(2, 2, 1, 1, "00", hex1, "50", "0D");  // low
    editDetails()->page()->addKnob(2, 3, 1, 1, "00", hex1, "50", "0E");  // high
    editDetails()->page()->addGroupBox(0, 0, 7, 6);
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(0);
    editDetails()->page()->newGroupBox("Normal SET 9");
    editDetails()->page()->addNameEdit(0, 0, 1, 2, "00", hex1, "60", "00", "8"); // Name
    editDetails()->page()->addSwitch(1, 0, 1, 1, "00", hex1, "60", "08"); // comp off/on
    editDetails()->page()->addKnob(1, 1, 1, 1, "00", hex1, "60", "09");  // attack
    editDetails()->page()->addKnob(1, 2, 1, 1, "00", hex1, "60", "0A");  // sustain
    editDetails()->page()->addKnob(1, 3, 1, 1, "00", hex1, "60", "0B");  // level
    editDetails()->page()->addKnob(2, 1, 1, 1, "00", hex1, "60", "0C");  // sense
    editDetails()->page()->addKnob(2, 2, 1, 1, "00", hex1, "60", "0D");  // low
    editDetails()->page()->addKnob(2, 3, 1, 1, "00", hex1, "60", "0E");  // high
    editDetails()->page()->addGroupBox(0, 0, 7, 6);
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(0);
    editDetails()->page()->newGroupBox("Normal SET 10");
    editDetails()->page()->addNameEdit(0, 0, 1, 2, "00", hex1, "70", "00", "8"); // Name
    editDetails()->page()->addSwitch(1, 0, 1, 1, "00", hex1, "70", "08"); // comp off/on
    editDetails()->page()->addKnob(1, 1, 1, 1, "00", hex1, "70", "09");  // attack
    editDetails()->page()->addKnob(1, 2, 1, 1, "00", hex1, "70", "0A");  // sustain
    editDetails()->page()->addKnob(1, 3, 1, 1, "00", hex1, "70", "0B");  // level
    editDetails()->page()->addKnob(2, 1, 1, 1, "00", hex1, "70", "0C");  // sense
    editDetails()->page()->addKnob(2, 2, 1, 1, "00", hex1, "70", "0D");  // low
    editDetails()->page()->addKnob(2, 3, 1, 1, "00", hex1, "70", "0E");  // high
    editDetails()->page()->addGroupBox(0, 0, 7, 6);
    editDetails()->page()->addStackField();


    editDetails()->addPage("Tables", "00", "02", "01", "04");       // ******************************************************************

    hex1 = "04";
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){hex1 = "0B";};

    editDetails()->page()->newGroupBox("Guitar to midi ");
    editDetails()->page()->addSwitch(0, 0, 1, 1, "00", hex1, "21", "00");
    editDetails()->page()->addComboBox(1, 0, 1, 1, "00", hex1, "21", "01"); //
    editDetails()->page()->addComboBox(2, 0, 1, 1, "00", hex1, "21", "02"); //
    editDetails()->page()->addComboBox(3, 0, 1, 1, "00", hex1, "21", "03"); //
    editDetails()->page()->addComboBox(4, 0, 1, 1, "00", hex1, "21", "04"); //
    editDetails()->page()->addComboBox(6, 0, 1, 1, "00", hex1, "21", "05"); //
    editDetails()->page()->addGroupBox(0, 2, 1, 1);


    editDetails()->addPage("Tables", "00", "02", "01", "05");       // ******************************************************************





}
