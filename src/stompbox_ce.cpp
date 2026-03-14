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

#include "stompbox_ce.h"
#include "Preferences.h"
#include "SysxIO.h"

namespace {

constexpr int kChoKnobLength = 60;
const char * const kChoComboDirection = "left_large";

struct ChoControlSpec
{
    int row;
    int column;
    const char *address;
    const char *background;
};

void addChoKnob(editPage *page,
                const QString &hex1,
                const QString &hex2,
                const ChoControlSpec &spec)
{
    page->addKnob(spec.row, spec.column, 1, 1, "10", hex1, hex2, spec.address, spec.background, "bottom", kChoKnobLength, Qt::AlignTop | Qt::AlignHCenter);
}

void addChoSimpleMatrixPage(editPage *page,
                            int stackId,
                            const QString &hex1,
                            const QString &hex2)
{
    page->newStackField(stackId, Qt::AlignTop | Qt::AlignLeft);
    page->compactCurrentStackField(20);

    const QList<ChoControlSpec> knobControls = {
        {0, 0, "02", "normal_ratio1.5"},
        {0, 1, "03", "normal_ratio1.5"},
        {0, 2, "04", "normal_ratio1.5"},
        {0, 3, "05", "normal_ratio1.75"},
        {0, 4, "06", "normal_ratio1.5"},
        {0, 5, "07", "normal_ratio1.25"},
        {0, 6, "08", "normal_ratio1.25"},
        {0, 7, "09", "normal_ratio1.5"},
        {0, 8, "18", "normal_ratio1.5"}
    };

    for(const ChoControlSpec &control : knobControls)
    {
        addChoKnob(page, hex1, hex2, control);
    }
    page->addStackField();
}

void addChoDualModePage(editPage *page,
                        int stackId,
                        const QString &hex1,
                        const QString &hex2,
                        const QList<ChoControlSpec> &controls)
{
    page->newStackField(stackId, Qt::AlignTop | Qt::AlignLeft);
    page->compactCurrentStackField(20);
    for(const ChoControlSpec &control : controls)
    {
        page->addKnob(control.row, control.column, 1, 1, "10", hex1, hex2, control.address, control.background, "bottom", kChoKnobLength, Qt::AlignTop | Qt::AlignHCenter);
    }
    page->addStackField();
}

}

stompbox_ce::stompbox_ce(QWidget *parent)
    : stompBox(parent)
{
    SysxIO *sysxIO = SysxIO::Instance();
    sysxIO->deBug("build Chorus stomp");

    QString hex1 = "00";
    QString hex2 = "3F";
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){hex1 = "02"; hex2 = "3C";};

    /* Chorus */
    setOnImage(":/images/ce_on.png");
    setOffImage(":/images/ce_off.png");
    setHoverImage(":/images/effect_hover.png");
    setSelectedImage(":/images/effect_select.png");
    setLSB(hex2, "00");
    brushColor("cyan");
    setButton("10", hex1, hex2, "00");
   /* editDetails()->patchPos(1394, 10, "06", "00");  */
    setEditPages();
}

void stompbox_ce::updateSignal()
{
    QString hex1 = "00";
    QString hex2 = "3F";
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){hex1 = "02"; hex2 = "3C";};

    updateButton("10", hex1, hex2, "00");
    updateLabel("10", hex1, hex2, "01");
};

void stompbox_ce::setEditPages()
{
    QString hex1 = "00";
    QString hex2 = "3F";
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){hex1 = "02"; hex2 = "3C";};

    editDetails()->setQmlPage("qrc:/qml/CePanel.qml", hex1, hex2);
}

#if 0 // Original setEditPages body — preserved for reference
    editPage *page = editDetails()->page();

    const QList<ChoControlSpec> dualModeControls = {
        {0, 0, "0A", "normal_ratio1.5"},
        {0, 1, "0B", "normal_ratio1.5"},
        {0, 2, "0C", "normal_ratio1.5"},
        {0, 3, "0D", "normal_ratio1.75"},
        {0, 4, "0E", "normal_ratio1.5"},
        {0, 5, "0F", "normal_ratio1.25"},
        {0, 6, "10", "normal_ratio1.25"},
        {0, 7, "09", "normal_ratio1.5"},
        {1, 0, "11", "normal_ratio1.5"},
        {1, 1, "12", "normal_ratio1.5"},
        {1, 2, "13", "normal_ratio1.5"},
        {1, 3, "14", "normal_ratio1.75"},
        {1, 4, "15", "normal_ratio1.5"},
        {1, 5, "16", "normal_ratio1.25"},
        {1, 6, "17", "normal_ratio1.25"},
        {1, 7, "18", "normal_ratio1.5"}
    };

    // CHO contract: MODE is the only dropdown; every other Chorus parameter
    // stays in the flat knob matrix below the divider.
    page->newStackControl(0);
    page->addComboBox(0, 0, 1, 1, "10", hex1, hex2, "01", kChoComboDirection, Qt::AlignTop | Qt::AlignLeft);
    page->addStackControl();

    page->addDivider(1, 0, 1, 1, Qt::AlignTop | Qt::AlignLeft);

    addChoSimpleMatrixPage(page, 0, hex1, hex2);
    addChoSimpleMatrixPage(page, 0, hex1, hex2);
    addChoSimpleMatrixPage(page, 0, hex1, hex2);
    addChoDualModePage(page, 0, hex1, hex2, dualModeControls);
    page->insertStackField(0, 2, 0, 1, 1, Qt::AlignTop | Qt::AlignLeft);

	editDetails()->addPage();
#endif
