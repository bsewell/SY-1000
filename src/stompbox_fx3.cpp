/****************************************************************************
**
** Copyright (C) 2007~2025 Colin Willcocks.
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

#include "stompbox_fx3.h"
#include "Preferences.h"
#include "SysxIO.h"

namespace {

// formerly addKnobSection: removed newGroupBox/addGroupBox; colOffset shifts knob cols into flat page grid
void addKnobSection(editPage *page,
                    const QString &hex1,
                    const QString &hex2,
                    const QStringList &addresses,
                    const QStringList &backgrounds,
                    int colOffset)
{
    for(int i = 0; i < addresses.size(); ++i)
    {
        page->addKnob(0, colOffset + i, 1, 1, "10", hex1, hex2, addresses.at(i),
                      backgrounds.value(i, "normal_ratio1.5"), "bottom", 60);
    }
}

// formerly addFilterSection: removed newGroupBox/addGroupBox; colOffset shifts controls into flat page grid
void addFilterSection(editPage *page,
                      const QString &hex1,
                      const QString &hex2,
                      const QString &waveAddr,
                      const QString &lowAddr,
                      const QString &highAddr,
                      int colOffset)
{
    page->addComboBox(0, colOffset, 1, 1, "10", hex1, hex2, waveAddr);
    page->addKnob(0, colOffset + 1, 1, 1, "10", hex1, hex2, lowAddr, "normal_ratio1.25", "bottom", 60);
    page->addKnob(0, colOffset + 2, 1, 1, "10", hex1, hex2, highAddr, "normal_ratio1.25", "bottom", 60);
}

// addStereoChorusPage: knob section at colOffset=0, filter section at colOffset=4 (sequential after 4 knobs)
void addStereoChorusPage(editPage *page,
                         const QString &hex1,
                         const QString &hex2,
                         const QString &chorusPrefix,
                         const QString &waveAddr,
                         const QString &lowAddr,
                         const QString &highAddr)
{
    page->newStackField(1);
    addKnobSection(page, hex1, hex2,
                   QStringList{chorusPrefix, QString::number(chorusPrefix.toInt(nullptr, 16) + 1, 16).rightJustified(2, '0').toUpper(),
                               QString::number(chorusPrefix.toInt(nullptr, 16) + 2, 16).rightJustified(2, '0').toUpper(),
                               QString::number(chorusPrefix.toInt(nullptr, 16) + 3, 16).rightJustified(2, '0').toUpper()},
                   QStringList{"normal_ratio1.5", "normal_ratio1.5", "normal_ratio1.5", "normal_ratio1.75"},
                   0);
    addFilterSection(page, hex1, hex2, waveAddr, lowAddr, highAddr, 4);
    page->addStackField();
}

// formerly addLevelSection: removed newGroupBox/addGroupBox; colOffset=1 since LEVEL always placed at parentCol=1
void addLevelSection(editPage *page,
                     const QString &hex1,
                     const QString &hex2,
                     const QString &directAddr,
                     const QString &outputAddr)
{
    // formerly addGroupBox(0, 1, 1, 1) — parentCol=1
    page->addKnob(0, 1, 1, 1, "10", hex1, hex2, directAddr, "normal_ratio1.5");
    page->addComboBox(0, 2, 1, 1, "10", hex1, hex2, outputAddr, "large");
}

}

stompbox_fx3::stompbox_fx3(QWidget *parent)
    : stompBox(parent)
{
    SysxIO *sysxIO = SysxIO::Instance();
    sysxIO->deBug("build FX3 stomp");

    QString hex1 = "01";
    QString hex2 = "0C";
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex1 = "03";
        hex2 = "09";
    };

    /* FX3 */
    setOnImage(":/images/fx3_on.png");
    setOffImage(":/images/fx3_off.png");
    setHoverImage(":/images/effect_hover.png");
    setSelectedImage(":/images/effect_select.png");
    setButton("10", hex1, hex2, "00");
    setLSB(hex1, hex2);
    brushColor("magenta");
   /* editDetails()->patchPos(776, 530, "03", "04"); */
    setEditPages();
}

void stompbox_fx3::updateSignal()
{
    QString hex1 = "01";
    QString hex2 = "0C";
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex1 = "03";
        hex2 = "09";
    };

  updateButton("10", hex1, hex2, "00");
  updateLabel("10", hex1, hex2, "01");
}

void stompbox_fx3::setEditPages()
{
    editPage *page = editDetails()->page();
    QString hex1 = "01";
    QString hex2 = "0C";
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex1 = "03";
        hex2 = "09";
    };

    page->addLabel(0, 0, 1, 1, "FX TYPE", Qt::AlignVCenter | Qt::AlignLeft);
    page->newStackControl(0);
    page->addComboBox(0, 1, 1, 1, "10", hex1, hex2, "01", "large");            //type
    page->addStackControl();
    page->addSwitch(0, 2, 1, 1, "10", hex1, hex2, "00");
    page->addDivider(1, 0, 1, 8);
    page->insertStackField(0, 2, 0, 2, 8);

    // AC Reso
    page->newStackField(0);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex2 = "0A";
    }
    else
    {
        hex2 = "0D";
    };
    // formerly "AC RESONANCE" groupBox at addGroupBox(0, 1, 1, 1) — parentCol=1
    editDetails()->page()->addComboBox(0, 1, 1, 1, "10", hex1, hex2, "00", "large");
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "01", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "03", "normal_ratio2");
    editDetails()->page()->addStackField();

    // Auto Wah
    editDetails()->page()->newStackField(0);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex2 = "0B";
    }
    else
    {
        hex2 = "0E";
    };
    // formerly "AUTO WAH" groupBox at addGroupBox(0, 1, 1, 1) — parentCol=1
    editDetails()->page()->addComboBox(0, 1, 1, 1, "10", hex1, hex2, "00", "large");
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "01", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "04", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 5, 1, 1, "10", hex1, hex2, "05", "normal_ratio1.75");
    editDetails()->page()->addComboBox(0, 6, 1, 1, "10", hex1, hex2, "06", "large");
    // formerly "LEVEL" groupBox at addGroupBox(0, 2, 1, 1) — sequential cols 7-8
    editDetails()->page()->addKnob(0, 7, 1, 1, "10", hex1, hex2, "03", "normal_ratio2");
    editDetails()->page()->addKnob(0, 8, 1, 1, "10", hex1, hex2, "07", "normal_ratio1.75");
    editDetails()->page()->addStackField();

    //Chorus
    editDetails()->page()->newStackField(0);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex2 = "0C";
    }
    else
    {
        hex2 = "0F";
    };
    // formerly "EFFECT" groupBox at addGroupBox(0, 0, 1, 1) — parentCol=0
    page->newStackControl(1);
    page->addComboBox(1, 0, 1, 1, "10", hex1, hex2, "01", "large");           //mode
    page->addStackControl();
    page->insertStackField(1, 1, 0, 1, 2);
    // formerly addLevelSection: LEVEL groupBox at addGroupBox(0, 1, 1, 1) — parentCol=1
    addLevelSection(page, hex1, hex2, "17", "18");
    page->addStackField();

    // Chorus stackField 1 entries (single chorus × 3)
    addStereoChorusPage(page, hex1, hex2, "02", "06", "07", "08");
    addStereoChorusPage(page, hex1, hex2, "02", "06", "07", "08");
    addStereoChorusPage(page, hex1, hex2, "02", "06", "07", "08");

    // DUAL Chorus
    editDetails()->page()->newStackField(1);
    // formerly "CHORUS 1" groupBox at addGroupBox(0, 0, 1, 1) — parentCol=0
    editDetails()->page()->addKnob(0, 0, 1, 1, "10", hex1, hex2, "09", "normal_ratio1.5", "bottom", 60); //rate
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "0A", "normal_ratio1.5", "bottom", 60); //depth
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "0B", "normal_ratio1.5", "bottom", 60); //predelay
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "0C", "normal_ratio1.75", "bottom", 60); //effect
    // formerly "FILTER 1" groupBox at addGroupBox(0, 1, 1, 1) — sequential cols 4-6
    editDetails()->page()->addComboBox(0, 4, 1, 1, "10", hex1, hex2, "0D"); //waveform
    editDetails()->page()->addKnob(0, 5, 1, 1, "10", hex1, hex2, "0E", "normal_ratio1.25", "bottom", 60);
    editDetails()->page()->addKnob(0, 6, 1, 1, "10", hex1, hex2, "0F", "normal_ratio1.25", "bottom", 60);
    // formerly "CHORUS 2" groupBox at addGroupBox(1, 0, 1, 1) — parentRow=1, parentCol=0
    editDetails()->page()->addKnob(1, 0, 1, 1, "10", hex1, hex2, "10", "normal_ratio1.5", "bottom", 60); //rate
    editDetails()->page()->addKnob(1, 1, 1, 1, "10", hex1, hex2, "11", "normal_ratio1.5", "bottom", 60); //depth
    editDetails()->page()->addKnob(1, 2, 1, 1, "10", hex1, hex2, "12", "normal_ratio1.5", "bottom", 60); //predelay
    editDetails()->page()->addKnob(1, 3, 1, 1, "10", hex1, hex2, "13", "normal_ratio1.75", "bottom", 60); //effect
    // formerly "FILTER 2" groupBox at addGroupBox(1, 1, 1, 1) — row 1 sequential cols 4-6
    editDetails()->page()->addComboBox(1, 4, 1, 1, "10", hex1, hex2, "14"); //waveform
    editDetails()->page()->addKnob(1, 5, 1, 1, "10", hex1, hex2, "15", "normal_ratio1.25", "bottom", 60);
    editDetails()->page()->addKnob(1, 6, 1, 1, "10", hex1, hex2, "16", "normal_ratio1.25", "bottom", 60);
    editDetails()->page()->addStackField();


    // Clasic Vibe
    editDetails()->page()->newStackField(0);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex2 = "0D";
    }
    else
    {
        hex2 = "10";
    };
    // formerly "CLASSIC VIBE" groupBox at addGroupBox(0, 1, 1, 1) — parentCol=1
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "00", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "01", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "02", "normal_ratio2");
    editDetails()->page()->addStackField();


    // Compressor
    editDetails()->page()->newStackField(0);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex2 = "0E";
    }
    else
    {
        hex2 = "11";
    };
    // formerly "COMPRESSOR" groupBox at addGroupBox(0, 1, 1, 1) — parentCol=1
    editDetails()->page()->addComboBox(0, 1, 1, 2, "10", hex1, hex2, "01", "large");
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "03", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 5, 1, 1, "10", hex1, hex2, "04", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 6, 1, 1, "10", hex1, hex2, "05", "normal_ratio2");
    editDetails()->page()->addKnob(0, 7, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.75");
    editDetails()->page()->addStackField();


    // Defretter
    editDetails()->page()->newStackField(0);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex2 = "0F";
    }
    else
    {
        hex2 = "12";
    };
    // formerly "DEFRETTER" groupBox at addGroupBox(0, 1, 1, 1) — parentCol=1
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "00", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "01", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "03", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 5, 1, 1, "10", hex1, hex2, "04", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 6, 1, 1, "10", hex1, hex2, "05", "normal_ratio2");
    editDetails()->page()->addKnob(0, 7, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.75");
    editDetails()->page()->addStackField();


    // Defretter Bass
    editDetails()->page()->newStackField(0);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex2 = "10";
    }
    else
    {
        hex2 = "13";
    };
    // formerly "DEFRETTER BASS" groupBox at addGroupBox(0, 1, 1, 1) — parentCol=1
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "00", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "01", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "03", "normal_ratio2");
    editDetails()->page()->addKnob(0, 5, 1, 1, "10", hex1, hex2, "04", "normal_ratio1.75");
    editDetails()->page()->addStackField();


    // DELAY
    editDetails()->page()->newStackField(0);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex2 = "11";
    }
    else
    {
        hex2 = "14";
    };
    // formerly "EFFECT" groupBox at addGroupBox(0, 0, 2, 1) — parentCol=0
    editDetails()->page()->newStackControl(2);
    editDetails()->page()->addComboBox(1, 0, 1, 1, "10", hex1, hex2, "01", "large");
    editDetails()->page()->addStackControl();
    editDetails()->page()->insertStackField(2, 0, 1, 2, 1);
    editDetails()->page()->addStackField();

    // STEREO 1
    editDetails()->page()->newStackField(2);
    // formerly "DELAY" groupBox at addGroupBox(0, 0, 4, 1) — parentCol=0
    editDetails()->page()->addDataKnob(0, 0, 1, 1, "10", hex1, hex2, "02", "DELAY2000_ratio1.75","bottom", 60); //4 byte time
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.25"); //feedback
    // formerly "FILTER" groupBox at addGroupBox(0, 1, 4, 1) — sequential col 2
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "07", "normal_ratio1.25", "bottom", 60); //high cut
    // formerly "LEVEL" groupBox at addGroupBox(0, 2, 4, 1) — sequential col 3
    editDetails()->page()->addKnob(0, 3, 1, 2, "10", hex1, hex2, "08", "turbo_ratio1.75"); //EFFECT
    editDetails()->page()->addKnob(1, 3, 1, 1, "10", hex1, hex2, "09", "normal_ratio1.5"); // direct
    editDetails()->page()->addKnob(1, 4, 1, 1, "10", hex1, hex2, "20", "normal_ratio1.25"); //carry over
    editDetails()->page()->addStackField();

    // STEREO 2
    editDetails()->page()->newStackField(2);
    // formerly "DELAY" groupBox at addGroupBox(0, 0, 4, 1) — parentCol=0
    editDetails()->page()->addDataKnob(0, 0, 1, 1, "10", hex1, hex2, "02", "DELAY2000_ratio1.75","bottom", 60); //4 byte time
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.25"); //feedback
    // formerly "FILTER" groupBox at addGroupBox(0, 1, 4, 1) — sequential col 2
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "07", "normal_ratio1.25", "bottom", 60); //high cut
    // formerly "LEVEL" groupBox at addGroupBox(0, 2, 4, 1) — sequential col 3
    editDetails()->page()->addKnob(0, 3, 1, 2, "10", hex1, hex2, "08", "turbo_ratio1.75"); //EFFECT
    editDetails()->page()->addKnob(1, 3, 1, 1, "10", hex1, hex2, "09", "normal_ratio1.5"); // direct
    editDetails()->page()->addKnob(1, 4, 1, 1, "10", hex1, hex2, "20", "normal_ratio1.25"); //carry over
    editDetails()->page()->addStackField();

    // PAN
    editDetails()->page()->newStackField(2);
    // formerly "DELAY" groupBox at addGroupBox(0, 0, 4, 1) — parentCol=0
    editDetails()->page()->addDataKnob(0, 0, 1, 1, "10", hex1, hex2, "02", "DELAY2000_ratio1.75","bottom", 60); //4 byte time
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.25"); //feedback
    editDetails()->page()->addKnob(1, 0, 1, 2, "10", hex1, hex2, "1F", "normal_ratio1.25"); //tap time %
    // formerly "FILTER" groupBox at addGroupBox(0, 1, 4, 1) — sequential col 2
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "07", "normal_ratio1.25", "bottom", 60); //high cut
    // formerly "LEVEL" groupBox at addGroupBox(0, 2, 4, 1) — sequential col 3
    editDetails()->page()->addKnob(0, 3, 1, 2, "10", hex1, hex2, "08", "turbo_ratio1.75"); //EFFECT
    editDetails()->page()->addKnob(1, 3, 1, 1, "10", hex1, hex2, "09", "normal_ratio1.5"); // direct
    editDetails()->page()->addKnob(1, 4, 1, 1, "10", hex1, hex2, "20", "normal_ratio1.25"); //carry over
    editDetails()->page()->addStackField();

    // DUAL SERIES
    editDetails()->page()->newStackField(2);
    // formerly "DELAY 1" groupBox at addGroupBox(0, 0, 1, 1) — parentCol=0
    editDetails()->page()->addDataKnob(0, 0, 1, 1, "10", hex1, hex2, "0A", "DELAY2000_ratio1.25","bottom", 60); //4 byte time
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.25"); //feedback
    // formerly "DELAY 2" groupBox at addGroupBox(1, 0, 1, 1) — parentRow=1, parentCol=0
    editDetails()->page()->addDataKnob(1, 0, 1, 1, "10", hex1, hex2, "11", "DELAY2000_ratio1.25","bottom", 60); //4 byte time
    editDetails()->page()->addKnob(1, 1, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.25"); //feedback
    // formerly "FILTER 1" groupBox at addGroupBox(0, 1, 1, 1) — sequential col 2
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "0F", "normal_ratio1.25", "bottom", 60);
    // formerly "FILTER 2" groupBox at addGroupBox(1, 1, 1, 1) — row 1 sequential col 2
    editDetails()->page()->addKnob(1, 2, 1, 1, "10", hex1, hex2, "16", "normal_ratio1.25", "bottom", 60);
    // formerly "LEVEL 1" groupBox at addGroupBox(0, 2, 1, 1) — sequential col 3
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "10", "normal_ratio1.75");
    // formerly "LEVEL 2" groupBox at addGroupBox(1, 2, 1, 1) — row 1 sequential col 3
    editDetails()->page()->addKnob(1, 3, 1, 1, "10", hex1, hex2, "17", "normal_ratio1.75");
    // formerly "Direct" groupBox at addGroupBox(0, 3, 2, 1) — sequential col 4
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "09", "normal_ratio1.5");
    editDetails()->page()->addKnob(1, 4, 1, 1, "10", hex1, hex2, "20", "normal_ratio1.25"); //carry over
    editDetails()->page()->addStackField();

    // DUAL PARALLEL
    editDetails()->page()->newStackField(2);
    // formerly "DELAY 1" groupBox at addGroupBox(0, 0, 1, 1) — parentCol=0
    editDetails()->page()->addDataKnob(0, 0, 1, 1, "10", hex1, hex2, "0A", "DELAY2000_ratio1.25","bottom", 60); //4 byte time
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.25"); //feedback
    // formerly "DELAY 2" groupBox at addGroupBox(1, 0, 1, 1) — parentRow=1, parentCol=0
    editDetails()->page()->addDataKnob(1, 0, 1, 1, "10", hex1, hex2, "11", "DELAY2000_ratio1.25","bottom", 60); //4 byte time
    editDetails()->page()->addKnob(1, 1, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.25"); //feedback
    // formerly "FILTER 1" groupBox at addGroupBox(0, 1, 1, 1) — sequential col 2
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "0F", "normal_ratio1.25", "bottom", 60);
    // formerly "FILTER 2" groupBox at addGroupBox(1, 1, 1, 1) — row 1 sequential col 2
    editDetails()->page()->addKnob(1, 2, 1, 1, "10", hex1, hex2, "16", "normal_ratio1.25", "bottom", 60);
    // formerly "LEVEL 1" groupBox at addGroupBox(0, 2, 1, 1) — sequential col 3
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "10", "normal_ratio1.75");
    // formerly "LEVEL 2" groupBox at addGroupBox(1, 2, 1, 1) — row 1 sequential col 3
    editDetails()->page()->addKnob(1, 3, 1, 1, "10", hex1, hex2, "17", "normal_ratio1.75");
    // formerly "Direct" groupBox at addGroupBox(0, 3, 2, 1) — sequential col 4
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "09", "normal_ratio1.5");
    editDetails()->page()->addKnob(1, 4, 1, 1, "10", hex1, hex2, "20", "normal_ratio1.25"); //carry over
    editDetails()->page()->addStackField();

    // DUAL L/R
    editDetails()->page()->newStackField(2);
    // formerly "DELAY 1" groupBox at addGroupBox(0, 0, 1, 1) — parentCol=0
    editDetails()->page()->addDataKnob(0, 0, 1, 1, "10", hex1, hex2, "0A", "DELAY2000_ratio1.25","bottom", 60); //4 byte time
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.25"); //feedback
    // formerly "DELAY 2" groupBox at addGroupBox(1, 0, 1, 1) — parentRow=1, parentCol=0
    editDetails()->page()->addDataKnob(1, 0, 1, 1, "10", hex1, hex2, "11", "DELAY2000_ratio1.25","bottom", 60); //4 byte time
    editDetails()->page()->addKnob(1, 1, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.25"); //feedback
    // formerly "FILTER 1" groupBox at addGroupBox(0, 1, 1, 1) — sequential col 2
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "0F", "normal_ratio1.25", "bottom", 60);
    // formerly "FILTER 2" groupBox at addGroupBox(1, 1, 1, 1) — row 1 sequential col 2
    editDetails()->page()->addKnob(1, 2, 1, 1, "10", hex1, hex2, "16", "normal_ratio1.25", "bottom", 60);
    // formerly "LEVEL 1" groupBox at addGroupBox(0, 2, 1, 1) — sequential col 3
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "10", "normal_ratio1.75");
    // formerly "LEVEL 2" groupBox at addGroupBox(1, 2, 1, 1) — row 1 sequential col 3
    editDetails()->page()->addKnob(1, 3, 1, 1, "10", hex1, hex2, "17", "normal_ratio1.75");
    // formerly "Direct" groupBox at addGroupBox(0, 3, 2, 1) — sequential col 4
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "09", "normal_ratio1.5");
    editDetails()->page()->addKnob(1, 4, 1, 1, "10", hex1, hex2, "20", "normal_ratio1.25"); //carry over
    editDetails()->page()->addStackField();

    // REVERSE
    editDetails()->page()->newStackField(2);
    // formerly "DELAY" groupBox at addGroupBox(0, 0, 4, 1) — parentCol=0
    editDetails()->page()->addDataKnob(0, 0, 1, 1, "10", hex1, hex2, "02", "DELAY2000_ratio1.75","bottom", 60); //4 byte time
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.25"); //feedback
    // formerly "FILTER" groupBox at addGroupBox(0, 1, 4, 1) — sequential col 2
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "07", "normal_ratio1.25", "bottom", 60); //high cut
    // formerly "LEVEL" groupBox at addGroupBox(0, 2, 4, 1) — sequential col 3
    editDetails()->page()->addKnob(0, 3, 1, 2, "10", hex1, hex2, "08", "turbo_ratio1.75"); //EFFECT
    editDetails()->page()->addKnob(1, 3, 1, 1, "10", hex1, hex2, "09", "normal_ratio1.5"); // direct
    editDetails()->page()->addKnob(1, 4, 1, 1, "10", hex1, hex2, "20", "normal_ratio1.25"); //carry over
    editDetails()->page()->addStackField();

    // ANALOG
    editDetails()->page()->newStackField(2);
    // formerly "DELAY" groupBox at addGroupBox(0, 0, 4, 1) — parentCol=0
    editDetails()->page()->addDataKnob(0, 0, 1, 1, "10", hex1, hex2, "02", "DELAY2000_ratio1.75","bottom", 60); //4 byte time
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.25"); //feedback
    // formerly "FILTER" groupBox at addGroupBox(0, 1, 4, 1) — sequential col 2
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "07", "normal_ratio1.25", "bottom", 60); //high cut
    // formerly "LEVEL" groupBox at addGroupBox(0, 2, 4, 1) — sequential col 3
    editDetails()->page()->addKnob(0, 3, 1, 2, "10", hex1, hex2, "08", "turbo_ratio1.75"); //EFFECT
    editDetails()->page()->addKnob(1, 3, 1, 1, "10", hex1, hex2, "09", "normal_ratio1.5"); // direct
    editDetails()->page()->addKnob(1, 4, 1, 1, "10", hex1, hex2, "20", "normal_ratio1.25"); //carry over
    editDetails()->page()->addStackField();

    // TAPE
    editDetails()->page()->newStackField(2);
    // formerly "DELAY" groupBox at addGroupBox(0, 0, 4, 1) — parentCol=0
    editDetails()->page()->addDataKnob(0, 0, 1, 1, "10", hex1, hex2, "02", "DELAY2000_ratio1.75","bottom", 60); //4 byte time
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.25"); //feedback
    // formerly "FILTER" groupBox at addGroupBox(0, 1, 4, 1) — sequential col 2
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "07", "normal_ratio1.25", "bottom", 60); //high cut
    // formerly "LEVEL" groupBox at addGroupBox(0, 2, 4, 1) — sequential col 3
    editDetails()->page()->addKnob(0, 3, 1, 2, "10", hex1, hex2, "08", "turbo_ratio1.75"); //EFFECT
    editDetails()->page()->addKnob(1, 3, 1, 1, "10", hex1, hex2, "09", "normal_ratio1.5"); // direct
    editDetails()->page()->addKnob(1, 4, 1, 1, "10", hex1, hex2, "20", "normal_ratio1.25"); //carry over
    editDetails()->page()->addStackField();

    // MODULATE — Delay(0-1), Modulate(2-4 collapsed to row 0), Filter(5), Level(6-8)
    editDetails()->page()->newStackField(2);
    // formerly "DELAY" groupBox at addGroupBox(0, 0, 1, 1) — parentCol=0
    editDetails()->page()->addDataKnob(0, 0, 1, 1, "10", hex1, hex2, "02", "DELAY2000_ratio1.75","bottom", 60); //4 byte time
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.25"); //feedback
    // formerly "MODULATE" groupBox at addGroupBox(1, 0, 1, 4) — sequential cols 2-4 (collapsed to row 0)
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "18", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "19", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "1A", "normal_ratio1.25");
    // formerly "FILTER" groupBox at addGroupBox(0, 1, 1, 1) — sequential col 5
    editDetails()->page()->addKnob(0, 5, 1, 1, "10", hex1, hex2, "07", "normal_ratio1.25", "bottom", 60); //high cut
    // formerly "LEVEL" groupBox at addGroupBox(0, 2, 1, 1) — sequential col 6
    editDetails()->page()->addKnob(0, 6, 1, 2, "10", hex1, hex2, "08", "turbo_ratio1.75"); //EFFECT
    editDetails()->page()->addKnob(1, 6, 1, 1, "10", hex1, hex2, "09", "normal_ratio1.5"); // direct
    editDetails()->page()->addKnob(1, 7, 1, 1, "10", hex1, hex2, "20", "normal_ratio1.25"); //carry over
    editDetails()->page()->addStackField();

    // WARP
    editDetails()->page()->newStackField(2);
    // formerly "DELAY" groupBox at addGroupBox(0, 0, 4, 1) — parentCol=0
    editDetails()->page()->addDataKnob(0, 0, 1, 1, "10", hex1, hex2, "02", "DELAY2000_ratio1.75","bottom", 60); //4 byte time
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.25"); //feedback
    // formerly "FILTER" groupBox at addGroupBox(0, 1, 4, 1) — sequential col 2
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "07", "normal_ratio1.25", "bottom", 60); //high cut
    // formerly "LEVEL" groupBox at addGroupBox(0, 2, 4, 1) — sequential col 3
    editDetails()->page()->addKnob(0, 3, 1, 2, "10", hex1, hex2, "08", "turbo_ratio1.75"); //EFFECT
    editDetails()->page()->addKnob(1, 3, 1, 1, "10", hex1, hex2, "09", "normal_ratio1.5"); // direct
    editDetails()->page()->addKnob(1, 4, 1, 1, "10", hex1, hex2, "20", "normal_ratio1.25"); //carry over
    editDetails()->page()->addStackField();

    // TWIST
    editDetails()->page()->newStackField(2);
    // formerly "DELAY" groupBox at addGroupBox(0, 0, 1, 2) — parentCol=0
    editDetails()->page()->addDataKnob(0, 0, 1, 1, "10", hex1, hex2, "02", "DELAY2000_ratio1.75","bottom", 60); //4 byte time
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.25"); //feedback
    // formerly "TWIST" groupBox at addGroupBox(1, 0, 1, 5) — sequential cols 2+ (collapsed to row 0)
    editDetails()->page()->newStackControl(3);
    editDetails()->page()->addComboBox(0, 2, 1, 2, "10", hex1, hex2, "1B", "large");
    editDetails()->page()->addStackControl();
    editDetails()->page()->insertStackField(3, 0, 2, 1, 2);
    // formerly "FILTER" groupBox at addGroupBox(0, 2, 1, 1) — sequential col 4
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "07", "normal_ratio1.25", "bottom", 60); //high cut
    // formerly "LEVEL" groupBox at addGroupBox(0, 3, 1, 2) — sequential col 5
    editDetails()->page()->addKnob(0, 5, 1, 2, "10", hex1, hex2, "08", "turbo_ratio1.75"); //EFFECT
    editDetails()->page()->addKnob(1, 5, 1, 1, "10", hex1, hex2, "09", "normal_ratio1.5"); // direct
    editDetails()->page()->addKnob(1, 6, 1, 1, "10", hex1, hex2, "20", "normal_ratio1.25"); //carry over
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(3);
    editDetails()->page()->addKnob(0, 2, 1, 2, "10", hex1, hex2, "1C", "normal_ratio1.5");     //rise
    editDetails()->page()->addKnob(0, 4, 1, 2, "10", hex1, hex2, "1D", "normal_ratio1.5");     //fall
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(3);
    editDetails()->page()->addKnob(0, 2, 1, 2, "10", hex1, hex2, "1C", "normal_ratio1.5");     //rise
    editDetails()->page()->addKnob(0, 4, 1, 2, "10", hex1, hex2, "1E", "normal_ratio1.5");     //fade
    editDetails()->page()->addStackField();



    // Flanger
    editDetails()->page()->newStackField(0);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex2 = "12";
    }
    else
    {
        hex2 = "15";
    };
    // formerly "FLANGER" groupBox at addGroupBox(0, 1, 1, 1) — parentCol=1
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "00", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "01", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "03", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 5, 1, 1, "10", hex1, hex2, "04", "normal_ratio1.5");
    // formerly "FILTER" groupBox at addGroupBox(0, 2, 1, 1) — sequential cols 6-7
    editDetails()->page()->addKnob(0, 6, 1, 1, "10", hex1, hex2, "05", "normal_ratio1.5", "bottom", 60);
    editDetails()->page()->addKnob(0, 7, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.5", "bottom", 60);
    // formerly "LEVEL" groupBox at addGroupBox(0, 3, 1, 1) — sequential cols 8-9
    editDetails()->page()->addKnob(0, 8, 1, 1, "10", hex1, hex2, "07", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 9, 1, 1, "10", hex1, hex2, "08", "normal_ratio1.5");
    editDetails()->page()->addStackField();


    // Flanger Bass
    editDetails()->page()->newStackField(0);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex2 = "13";
    }
    else
    {
        hex2 = "16";
    };
    // formerly "FLANGER BASS" groupBox at addGroupBox(0, 1, 1, 1) — parentCol=1
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "00", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "01", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "03", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 5, 1, 1, "10", hex1, hex2, "04", "normal_ratio1.5");
    // formerly "FILTER" groupBox at addGroupBox(0, 2, 1, 1) — sequential cols 6-7
    editDetails()->page()->addKnob(0, 6, 1, 1, "10", hex1, hex2, "05", "normal_ratio1.5", "bottom", 60);
    editDetails()->page()->addKnob(0, 7, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.5", "bottom", 60);
    // formerly "LEVEL" groupBox at addGroupBox(0, 3, 1, 1) — sequential cols 8-9
    editDetails()->page()->addKnob(0, 8, 1, 1, "10", hex1, hex2, "07", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 9, 1, 1, "10", hex1, hex2, "08", "normal_ratio1.5");
    editDetails()->page()->addStackField();


    // Foot Volume
    editDetails()->page()->newStackField(0);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex2 = "14";
    }
    else
    {
        hex2 = "17";
    };
    // formerly "FOOT VOLUME" groupBox at addGroupBox(0, 0, 1, 1) — parentCol=0
    editDetails()->page()->addDataKnob(0, 0, 1, 1, "10", hex1, hex2, "00", "0~1000_ratio2");
    editDetails()->page()->addDataKnob(0, 1, 1, 1, "10", hex1, hex2, "04", "0~1000_ratio2");
    editDetails()->page()->addDataKnob(0, 2, 1, 1, "10", hex1, hex2, "08", "0~1000_ratio1.75");
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "0C", "normal_ratio1.5", "bottom", 60);
    editDetails()->page()->addStackField();


    // Graphic EQ
    editDetails()->page()->newStackField(0);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex2 = "15";
    }
    else
    {
        hex2 = "18";
    };
    // formerly "EQUALIZER" groupBox at addGroupBox(0, 2, 1, 7) — parentCol=2
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "00", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "01", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 5, 1, 1, "10", hex1, hex2, "03", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 6, 1, 1, "10", hex1, hex2, "04", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 7, 1, 1, "10", hex1, hex2, "05", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 8, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.5");
    // formerly "LEVEL" groupBox at addGroupBox(0, 8, 1, 1) — sequential col 9
    editDetails()->page()->addKnob(0, 9, 1, 1, "10", hex1, hex2, "07", "normal_ratio1.5");
    editDetails()->page()->addStackField();




    // Harmonizer
    editDetails()->page()->newStackField(0);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex2 = "16";
    }
    else
    {
        hex2 = "19";
    };
    // formerly "Effect" groupBox at addGroupBox(0, 0, 2, 3) — parentCol=0
    editDetails()->page()->newStackControl(4);
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, hex2, "00", "large");
    editDetails()->page()->addStackControl();
    editDetails()->page()->insertStackField(4, 0, 1, 3, 9);
    // formerly "User Harmony 1" groupBox at addGroupBox(3, 0, 1, 12) — parentRow=3, parentCol=0
    editDetails()->page()->addComboBox(3, 0, 1, 1, "10", hex1, hex2, "0F");
    editDetails()->page()->addComboBox(3, 1, 1, 1, "10", hex1, hex2, "10");
    editDetails()->page()->addComboBox(3, 2, 1, 1, "10", hex1, hex2, "11");
    editDetails()->page()->addComboBox(3, 3, 1, 1, "10", hex1, hex2, "12");
    editDetails()->page()->addComboBox(3, 4, 1, 1, "10", hex1, hex2, "13");
    editDetails()->page()->addComboBox(3, 5, 1, 1, "10", hex1, hex2, "14");
    editDetails()->page()->addComboBox(3, 6, 1, 1, "10", hex1, hex2, "15");
    editDetails()->page()->addComboBox(3, 7, 1, 1, "10", hex1, hex2, "16");
    editDetails()->page()->addComboBox(3, 8, 1, 1, "10", hex1, hex2, "17");
    editDetails()->page()->addComboBox(3, 9, 1, 1, "10", hex1, hex2, "18");
    editDetails()->page()->addComboBox(3, 10, 1, 1, "10", hex1, hex2, "19");
    editDetails()->page()->addComboBox(3, 11, 1, 1, "10", hex1, hex2, "1A");
    editDetails()->page()->addSystemOverride(3, 0, 1, 12, "10", hex1, hex2, "01", "1D", "not_equal"); //
    // formerly "User Harmony 2" groupBox at addGroupBox(4, 0, 1, 12) — parentRow=4, parentCol=0
    editDetails()->page()->addComboBox(4, 0, 1, 1, "10", hex1, hex2, "1B");
    editDetails()->page()->addComboBox(4, 1, 1, 1, "10", hex1, hex2, "1C");
    editDetails()->page()->addComboBox(4, 2, 1, 1, "10", hex1, hex2, "1D");
    editDetails()->page()->addComboBox(4, 3, 1, 1, "10", hex1, hex2, "1E");
    editDetails()->page()->addComboBox(4, 4, 1, 1, "10", hex1, hex2, "1F");
    editDetails()->page()->addComboBox(4, 5, 1, 1, "10", hex1, hex2, "20");
    editDetails()->page()->addComboBox(4, 6, 1, 1, "10", hex1, hex2, "21");
    editDetails()->page()->addComboBox(4, 7, 1, 1, "10", hex1, hex2, "22");
    editDetails()->page()->addComboBox(4, 8, 1, 1, "10", hex1, hex2, "23");
    editDetails()->page()->addComboBox(4, 9, 1, 1, "10", hex1, hex2, "24");
    editDetails()->page()->addComboBox(4, 10, 1, 1, "10", hex1, hex2, "25");
    editDetails()->page()->addComboBox(4, 11, 1, 1, "10", hex1, hex2, "26");
    editDetails()->page()->addSystemOverride(4, 0, 1, 12, "10", hex1, hex2, "02", "1D", "not_equal"); //
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(4);
    // formerly "Harmonist" groupBox at addGroupBox(0, 1, 2, 2) — parentCol=1
    //   formerly "Voice 1" groupBox at addGroupBox(0, 0, 2, 1) inside Harmonist — parentCol=0+1=1
    editDetails()->page()->addComboBox(0, 1, 1, 1, "10", hex1, hex2, "01", "large");
    editDetails()->page()->addDataKnob(0, 2, 1, 1, "10", hex1, hex2, "04", "PREDELAY_ratio1.25","bottom", 70);
    //   knob "08" at Harmonist(0,1) — sequential col 3
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "08", "normal_ratio1.25");
    // formerly "Level" groupBox at addGroupBox(0, 3, 2, 1) — sequential col 4
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "03", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 5, 1, 1, "10", hex1, hex2, "09", "normal_ratio1.5");
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(4);
    // formerly "Harmonist" groupBox at addGroupBox(0, 1, 2, 2) — parentCol=1
    //   formerly "Voice 1" groupBox at addGroupBox(0, 0, 1, 1) inside Harmonist — parentCol=0+1=1
    editDetails()->page()->addComboBox(0, 1, 1, 1, "10", hex1, hex2, "01", "large");
    editDetails()->page()->addDataKnob(0, 2, 1, 1, "10", hex1, hex2, "04", "PREDELAY_ratio1.25");
    //   formerly "Voice 2" groupBox at addGroupBox(2, 0, 1, 1) inside Harmonist — parentRow=2+0=2, parentCol=0+1=1
    editDetails()->page()->addComboBox(2, 1, 1, 1, "10", hex1, hex2, "02", "large");
    editDetails()->page()->addDataKnob(2, 2, 1, 1, "10", hex1, hex2, "0B", "PREDELAY_ratio1.25");
    //   knob "08" at Harmonist(0,1,2,1) — sequential col 3, rowSpan=2
    editDetails()->page()->addKnob(0, 3, 2, 1, "10", hex1, hex2, "08", "normal_ratio1.5");
    // formerly "Level" groupBox at addGroupBox(0, 3, 2, 1) — sequential col 4
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "03", "normal_ratio1.5");
    editDetails()->page()->addKnob(1, 4, 1, 1, "10", hex1, hex2, "0A", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 5, 2, 1, "10", hex1, hex2, "09", "normal_ratio1.25");
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(4);
    // formerly "Harmonist" groupBox at addGroupBox(0, 1, 2, 2) — parentCol=1
    //   formerly "Voice 1" groupBox at addGroupBox(0, 0, 1, 1) inside Harmonist — parentCol=0+1=1
    editDetails()->page()->addComboBox(0, 1, 1, 1, "10", hex1, hex2, "01", "large");
    editDetails()->page()->addDataKnob(0, 2, 1, 1, "10", hex1, hex2, "04", "PREDELAY_ratio1.25");
    //   formerly "Voice 2" groupBox at addGroupBox(2, 0, 1, 1) inside Harmonist — parentRow=2+0=2, parentCol=0+1=1
    editDetails()->page()->addComboBox(2, 1, 1, 1, "10", hex1, hex2, "02", "large");
    editDetails()->page()->addDataKnob(2, 2, 1, 1, "10", hex1, hex2, "0B", "PREDELAY_ratio1.25");
    //   knob "08" at Harmonist(0,1,2,1) — sequential col 3, rowSpan=2
    editDetails()->page()->addKnob(0, 3, 2, 1, "10", hex1, hex2, "08", "normal_ratio1.5");
    // formerly "Level" groupBox at addGroupBox(0, 3, 2, 1) — sequential col 4
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "03", "normal_ratio1.5");
    editDetails()->page()->addKnob(1, 4, 1, 1, "10", hex1, hex2, "0A", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 5, 2, 1, "10", hex1, hex2, "09", "normal_ratio1.25");
    editDetails()->page()->addStackField();


    // Humanizer
    editDetails()->page()->newStackField(0);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex2 = "17";
    }
    else
    {
        hex2 = "1A";
    };
    // formerly "HUMANIZER" groupBox at addGroupBox(0, 2, 1, 8) — parentCol=2
    editDetails()->page()->addComboBox(0, 2, 1, 2, "10", hex1, hex2, "00", "large");
    editDetails()->page()->addComboBox(0, 4, 1, 1, "10", hex1, hex2, "01", "large");
    editDetails()->page()->addComboBox(1, 4, 1, 1, "10", hex1, hex2, "02", "large");
    editDetails()->page()->addKnob(0, 6, 2, 1, "10", hex1, hex2, "03", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 7, 2, 1, "10", hex1, hex2, "04", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 8, 2, 1, "10", hex1, hex2, "05", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 9, 2, 1, "10", hex1, hex2, "06", "normal_ratio1.5");
    // formerly "LEVEL" groupBox at addGroupBox(0, 9, 1, 1) — sequential col 10
    editDetails()->page()->addKnob(0, 10, 1, 1, "10", hex1, hex2, "07", "normal_ratio1.75");
    editDetails()->page()->addStackField();


    // Isolator
    editDetails()->page()->newStackField(0);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex2 = "18";
    }
    else
    {
        hex2 = "1B";
    };
    // formerly "ISOLATOR" groupBox at addGroupBox(0, 1, 1, 1) — parentCol=1
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "00", "normal_ratio1.5", "bottom", 60);
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "01", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "03", "normal_ratio1.75");
    editDetails()->page()->addStackField();


    // Limiter
    editDetails()->page()->newStackField(0);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex2 = "19";
    }
    else
    {
        hex2 = "1C";
    };
    // formerly "LIMITER" groupBox at addGroupBox(0, 1, 1, 6) — parentCol=1
    editDetails()->page()->addComboBox(0, 1, 1, 2, "10", hex1, hex2, "00", "large");
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "01", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 5, 1, 1, "10", hex1, hex2, "04", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 6, 1, 1, "10", hex1, hex2, "05", "normal_ratio1.5");
    // formerly "LEVEL" groupBox at addGroupBox(0, 7, 1, 1) — sequential col 7
    editDetails()->page()->addKnob(0, 7, 1, 1, "10", hex1, hex2, "03", "normal_ratio1.75");
    editDetails()->page()->addStackField();


    // LoFi
    editDetails()->page()->newStackField(0);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex2 = "1A";
    }
    else
    {
        hex2 = "1D";
    };
    // formerly "LOFI" groupBox at addGroupBox(0, 1, 1, 1) — parentCol=1
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "00", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "01", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "02", "normal_ratio2","bottom", 70);
    editDetails()->page()->addStackField();


    // Octave
    editDetails()->page()->newStackField(0);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex2 = "1B";
    }
    else
    {
        hex2 = "1E";
    };
    // formerly "OCTAVE" groupBox at addGroupBox(0, 1, 1, 1) — parentCol=1
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "00", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "01", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.75");
    editDetails()->page()->addStackField();


    // Octave Bass
    editDetails()->page()->newStackField(0);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex2 = "1C";
    }
    else
    {
        hex2 = "1F";
    };
    // formerly "OCTAVE BASS" groupBox at addGroupBox(0, 1, 1, 1) — parentCol=1
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "00", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "01", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.75");
    editDetails()->page()->addStackField();


    // Panner
    editDetails()->page()->newStackField(0);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex2 = "1D";
    }
    else
    {
        hex2 = "20";
    };
    // formerly "AUTO PAN" groupBox at addGroupBox(0, 1, 1, 1) — parentCol=1
    editDetails()->page()->addComboBox(0, 1, 1, 1, "10", hex1, hex2, "00", "large");
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "01", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "03", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 5, 1, 1, "10", hex1, hex2, "05", "normal_ratio1.75");
    // formerly "LEVEL" groupBox at addGroupBox(0, 2, 1, 1) — sequential col 6
    editDetails()->page()->addKnob(0, 6, 1, 1, "10", hex1, hex2, "04", "normal_ratio1.75");
    editDetails()->page()->addStackField();


    // EQ
    editDetails()->page()->newStackField(0);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex2 = "1E";
    }
    else
    {
        hex2 = "21";
    };
    // formerly "PARAMETRIC EQUALIZER" groupBox at addGroupBox(0, 0, 1, 1) — parentCol=0
    editDetails()->page()->addParaEQ(0, 1, 2, 4, "10", hex1, hex2, "01");
    editDetails()->page()->addStackField();


    // Pedal Bend
    editDetails()->page()->newStackField(0);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex2 = "1F";
    }
    else
    {
        hex2 = "22";
    };
    // formerly "PEDAL BEND" groupBox at addGroupBox(0, 1, 1, 1) — parentCol=1
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "00", "normal_ratio1.75");
    editDetails()->page()->addDataKnob(0, 3, 1, 1, "10", hex1, hex2, "02", "0~1000_ratio1.75","bottom", 70);
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "01", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 5, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.75");
    editDetails()->page()->addStackField();


    // Phaser
    editDetails()->page()->newStackField(0);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex2 = "20";
    }
    else
    {
        hex2 = "23";
    };
    // formerly "PHASER" groupBox at addGroupBox(0, 1, 1, 1) — parentCol=1
    editDetails()->page()->addComboBox(0, 1, 1, 1, "10", hex1, hex2, "00", "large");
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "01", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "03", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 5, 1, 1, "10", hex1, hex2, "04", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 6, 1, 1, "10", hex1, hex2, "05", "normal_ratio1.75");
    // formerly "LEVEL" groupBox at addGroupBox(0, 2, 1, 1) — sequential cols 7-8
    editDetails()->page()->addKnob(0, 7, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 8, 1, 1, "10", hex1, hex2, "07", "normal_ratio1.75");
    editDetails()->page()->addStackField();


    // Pitch Shifter
    editDetails()->page()->newStackField(0);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex2 = "21";
    }
    else
    {
        hex2 = "24";
    };
    // formerly "Effect" groupBox at addGroupBox(0, 0, 2, 1) — parentCol=0
    editDetails()->page()->newStackControl(5);
    editDetails()->page()->addComboBox(1, 0, 1, 1, "10", hex1, hex2, "00", "large");
    editDetails()->page()->addStackControl();
    editDetails()->page()->insertStackField(5, 0, 1, 3, 1);
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(5);
    // formerly "PITCH SHIFTER" groupBox at addGroupBox(0, 1, 2, 4) — parentCol=1
    //   formerly "VOICE 1" groupBox at addGroupBox(0, 0, 3, 3) — parentCol=0+1=1
    editDetails()->page()->addComboBox(0, 1, 1, 3, "10", hex1, hex2, "04", "large");
    editDetails()->page()->addDataKnob(1, 1, 1, 1, "10", hex1, hex2, "06", "PREDELAY_ratio1.5","bottom", 70);
    editDetails()->page()->addKnob(1, 2, 1, 1, "10", hex1, hex2, "01", "normal_ratio1.5");
    editDetails()->page()->addKnob(1, 3, 1, 1, "10", hex1, hex2, "05", "normal_ratio1.25");
    //   knob "0B" at PITCH SHIFTER(1,4) → stackField(1,1+4)=(1,5)
    editDetails()->page()->addKnob(1, 5, 1, 1, "10", hex1, hex2, "0B", "normal_ratio1.5");
    // formerly "LEVEL" groupBox at addGroupBox(0, 5, 2, 1) — sequential col 6
    editDetails()->page()->addKnob(0, 6, 1, 1, "10", hex1, hex2, "0A", "normal_ratio1.5");
    editDetails()->page()->addKnob(1, 6, 1, 1, "10", hex1, hex2, "03", "normal_ratio1.25");
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(5);
    // formerly "PITCH SHIFTER" groupBox at addGroupBox(0, 1, 2, 4) — parentCol=1
    //   formerly "VOICE 1" groupBox at addGroupBox(0, 0, 3, 3) — parentCol=0+1=1
    editDetails()->page()->addComboBox(0, 1, 1, 3, "10", hex1, hex2, "04", "large");
    editDetails()->page()->addDataKnob(1, 1, 1, 1, "10", hex1, hex2, "06", "PREDELAY_ratio1.5","bottom", 70);
    editDetails()->page()->addKnob(1, 2, 1, 1, "10", hex1, hex2, "01", "normal_ratio1.5");
    editDetails()->page()->addKnob(1, 3, 1, 1, "10", hex1, hex2, "05", "normal_ratio1.25");
    //   knob "0B" at PS(1,4) → stackField(1,5)
    editDetails()->page()->addKnob(1, 5, 1, 1, "10", hex1, hex2, "0B", "normal_ratio1.5");
    //   formerly "VOICE 2" groupBox at addGroupBox(3, 0, 1, 3) — parentRow=3+0=3, parentCol=0+1=1
    editDetails()->page()->addComboBox(3, 1, 1, 3, "10", hex1, hex2, "0C", "large");
    editDetails()->page()->addDataKnob(4, 1, 1, 1, "10", hex1, hex2, "0E", "PREDELAY_ratio1.5","bottom", 70);
    editDetails()->page()->addKnob(4, 2, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.5");
    editDetails()->page()->addKnob(4, 3, 1, 1, "10", hex1, hex2, "0D", "normal_ratio1.25");
    // formerly "LEVEL" groupBox at addGroupBox(0, 5, 2, 1) — sequential col 6
    editDetails()->page()->addKnob(0, 6, 1, 1, "10", hex1, hex2, "0A", "normal_ratio1.75");
    editDetails()->page()->addKnob(2, 6, 1, 1, "10", hex1, hex2, "12", "normal_ratio1.75");
    editDetails()->page()->addKnob(1, 6, 1, 1, "10", hex1, hex2, "03", "normal_ratio1.5");
    editDetails()->page()->addStackField();

    editDetails()->page()->newStackField(5);
    // formerly "PITCH SHIFTER" groupBox at addGroupBox(0, 1, 2, 4) — parentCol=1
    //   formerly "VOICE 1" groupBox at addGroupBox(0, 0, 3, 3) — parentCol=0+1=1
    editDetails()->page()->addComboBox(0, 1, 1, 3, "10", hex1, hex2, "04", "large");
    editDetails()->page()->addDataKnob(1, 1, 1, 1, "10", hex1, hex2, "06", "PREDELAY_ratio1.5","bottom", 70);
    editDetails()->page()->addKnob(1, 2, 1, 1, "10", hex1, hex2, "01", "normal_ratio1.5");
    editDetails()->page()->addKnob(1, 3, 1, 1, "10", hex1, hex2, "05", "normal_ratio1.25");
    //   knob "0B" at PS(1,4) → stackField(1,5)
    editDetails()->page()->addKnob(1, 5, 1, 1, "10", hex1, hex2, "0B", "normal_ratio1.5");
    //   formerly "VOICE 2" groupBox at addGroupBox(3, 0, 1, 3) — parentRow=3+0=3, parentCol=0+1=1
    editDetails()->page()->addComboBox(3, 1, 1, 3, "10", hex1, hex2, "0C", "large");
    editDetails()->page()->addDataKnob(4, 1, 1, 1, "10", hex1, hex2, "0E", "PREDELAY_ratio1.5","bottom", 70);
    editDetails()->page()->addKnob(4, 2, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.5");
    editDetails()->page()->addKnob(4, 3, 1, 1, "10", hex1, hex2, "0D", "normal_ratio1.25");
    // formerly "LEVEL" groupBox at addGroupBox(0, 5, 2, 1) — sequential col 6
    editDetails()->page()->addKnob(0, 6, 1, 1, "10", hex1, hex2, "0A", "normal_ratio1.75");
    editDetails()->page()->addKnob(2, 6, 1, 1, "10", hex1, hex2, "12", "normal_ratio1.75");
    editDetails()->page()->addKnob(1, 6, 1, 1, "10", hex1, hex2, "03", "normal_ratio1.5");
    editDetails()->page()->addStackField();


    // Reverb
    editDetails()->page()->newStackField(0);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex2 = "22";
    }
    else
    {
        hex2 = "25";
    };
    // formerly "EFFECT" groupBox at addGroupBox(0, 0, 1, 1) — parentCol=0
    editDetails()->page()->addComboBox(0, 0, 1, 1, "10", hex1, hex2, "01", "large");   //type
    editDetails()->page()->addComboBox(1, 0, 1, 1, "10", hex1, hex2, "03", "large");   //carry over
    // formerly "REVERB" groupBox at addGroupBox(0, 1, 1, 1) — parentCol=1
    editDetails()->page()->addDataKnob(0, 1, 1, 1, "10", hex1, hex2, "04", "0~500_ratio1.5");  //predelay
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.75");  //time
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "0B", "normal_ratio1.5");  //density
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "0D", "normal_ratio1.5");  //spring sense
    editDetails()->page()->addSystemOverride(0, 4, 1, 1, "10", hex1, hex2, "01", "05", "not_equal");
    // formerly "FILTER" groupBox at addGroupBox(0, 2, 1, 1) — sequential cols 5-6
    editDetails()->page()->addKnob(0, 5, 1, 1, "10", hex1, hex2, "09", "normal_ratio1.5", "bottom", 60);  //low cut
    editDetails()->page()->addKnob(0, 6, 1, 1, "10", hex1, hex2, "0A", "normal_ratio1.5", "bottom", 60);  //high cut
    // formerly "LEVEL" groupBox at addGroupBox(0, 3, 1, 1) — sequential cols 7-8
    editDetails()->page()->addKnob(0, 7, 1, 1, "10", hex1, hex2, "08", "normal_ratio1.75");  //effect
    editDetails()->page()->addKnob(0, 8, 1, 1, "10", hex1, hex2, "0C", "normal_ratio1.5");  //direct
    editDetails()->page()->addStackField();

    // Ring Modulator
    editDetails()->page()->newStackField(0);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex2 = "23";
    }
    else
    {
        hex2 = "26";
    };
    // formerly "RING MODULATOR" groupBox at addGroupBox(0, 1, 1, 4) — parentCol=1
    editDetails()->page()->addComboBox(0, 1, 1, 1, "10", hex1, hex2, "00", "large");
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "01", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "03", "normal_ratio1.5");
    // formerly "LEVEL" groupBox at addGroupBox(0, 5, 1, 2) — parentCol=5
    editDetails()->page()->addKnob(0, 5, 1, 1, "10", hex1, hex2, "04", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 6, 1, 1, "10", hex1, hex2, "05", "normal_ratio1.5");
    editDetails()->page()->addStackField();


    // Rotary
    editDetails()->page()->newStackField(0);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex2 = "24";
    }
    else
    {
        hex2 = "27";
    };
    // formerly "ROTARY" groupBox at addGroupBox(0, 1, 1, 6) — parentCol=1
    editDetails()->page()->addComboBox(0, 1, 1, 1, "10", hex1, hex2, "00", "large");
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "01", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "03", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 5, 1, 1, "10", hex1, hex2, "04", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 6, 1, 1, "10", hex1, hex2, "05", "normal_ratio1.5");
    // formerly "LEVEL" groupBox at addGroupBox(0, 7, 1, 2) — parentCol=7
    editDetails()->page()->addKnob(0, 7, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 8, 1, 1, "10", hex1, hex2, "07", "normal_ratio1.5");
    editDetails()->page()->addStackField();


    // Sitar Sim
    editDetails()->page()->newStackField(0);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex2 = "25";
    }
    else
    {
        hex2 = "28";
    };
    // formerly "SITAR SIMULATOR" groupBox at addGroupBox(0, 1, 1, 5) — parentCol=1
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "00", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "01", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "04", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "05", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 5, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.5");
    // formerly "LEVEL" groupBox at addGroupBox(0, 6, 1, 2) — parentCol=6
    editDetails()->page()->addKnob(0, 6, 1, 1, "10", hex1, hex2, "03", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 7, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.5");
    editDetails()->page()->addStackField();


    // Slicer
    editDetails()->page()->newStackField(0);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex2 = "26";
    }
    else
    {
        hex2 = "29";
    };
    // formerly "SLICER" groupBox at addGroupBox(0, 1, 1, 5) — parentCol=1
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "00", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "01", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "05", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 5, 1, 1, "10", hex1, hex2, "04", "normal_ratio1.5");
    // formerly "LEVEL" groupBox at addGroupBox(0, 6, 1, 2) — parentCol=6
    editDetails()->page()->addKnob(0, 6, 1, 1, "10", hex1, hex2, "03", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 7, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.5");
    editDetails()->page()->addStackField();


    // Slow Gear
    editDetails()->page()->newStackField(0);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex2 = "27";
    }
    else
    {
        hex2 = "2A";
    };
    // formerly "SLOW GEAR" groupBox at addGroupBox(0, 1, 1, 1) — parentCol=1
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "00", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "01", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "02", "normal_ratio2");
    editDetails()->page()->addStackField();


    // Slow Gear Bass
    editDetails()->page()->newStackField(0);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex2 = "28";
    }
    else
    {
        hex2 = "2B";
    };
    // formerly "SLOW GEAR BASS" groupBox at addGroupBox(0, 1, 1, 1) — parentCol=1
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "00", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "01", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "02", "normal_ratio2");
    editDetails()->page()->addStackField();


    // Sound Hold
    editDetails()->page()->newStackField(0);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex2 = "29";
    }
    else
    {
        hex2 = "2C";
    };
    // formerly "SOUND HOLD" groupBox at addGroupBox(0, 1, 1, 1) — parentCol=1
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "00", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "01", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "02", "normal_ratio2");
    editDetails()->page()->addStackField();


    // T-Wah
    editDetails()->page()->newStackField(0);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex2 = "2A";
    }
    else
    {
        hex2 = "2D";
    };
    // formerly "T-WAH" groupBox at addGroupBox(0, 1, 1, 6) — parentCol=1
    editDetails()->page()->addComboBox(0, 1, 1, 1, "10", hex1, hex2, "00", "large");
    editDetails()->page()->addComboBox(0, 2, 1, 1, "10", hex1, hex2, "01", "large");
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "03", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 5, 1, 1, "10", hex1, hex2, "04", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 6, 1, 1, "10", hex1, hex2, "05", "normal_ratio1.5");
    // formerly "LEVEL" groupBox at addGroupBox(0, 7, 1, 2) — parentCol=7
    editDetails()->page()->addKnob(0, 7, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 8, 1, 1, "10", hex1,hex2, "07", "normal_ratio1.5");
    editDetails()->page()->addStackField();


    // T-Wah Bass
    editDetails()->page()->newStackField(0);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex2 = "2B";
    }
    else
    {
        hex2 = "2E";
    };
    // formerly "T-WAH BASS" groupBox at addGroupBox(0, 1, 1, 6) — parentCol=1
    editDetails()->page()->addComboBox(0, 1, 1, 1, "10", hex1, hex2, "00", "large");
    editDetails()->page()->addComboBox(0, 2, 1, 1, "10", hex1, hex2, "01", "large");
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "03", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 5, 1, 1, "10", hex1, hex2, "04", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 6, 1, 1, "10", hex1, hex2, "05", "normal_ratio1.5");
    // formerly "LEVEL" groupBox at addGroupBox(0, 7, 1, 2) — parentCol=7
    editDetails()->page()->addKnob(0, 7, 1, 1, "10", hex1, hex2, "06", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 8, 1, 1, "10", hex1, hex2, "07", "normal_ratio1.5");
    editDetails()->page()->addStackField();



    // Tremolo
    editDetails()->page()->newStackField(0);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex2 = "2C";
    }
    else
    {
        hex2 = "2F";
    };
    // formerly "TREMOLO" groupBox at addGroupBox(0, 1, 1, 1) — parentCol=1
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "00", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "01", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 3, 1, 1, "10", hex1, hex2, "02", "normal_ratio1.5");
    // formerly "LEVEL" groupBox at addGroupBox(0, 2, 1, 1) — sequential cols 4-5
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "03", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 5, 1, 1, "10", hex1, hex2, "04", "normal_ratio1.5");
    editDetails()->page()->addStackField();


    // Vibrato
    editDetails()->page()->newStackField(0);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex2 = "2D";
    }
    else
    {
        hex2 = "30";
    };
    // formerly "VIBRATO" groupBox at addGroupBox(0, 1, 1, 1) — parentCol=1
    editDetails()->page()->addKnob(0, 1, 1, 1, "10", hex1, hex2, "00", "normal_ratio1.5");
    editDetails()->page()->addKnob(0, 2, 1, 1, "10", hex1, hex2, "01", "normal_ratio1.5");
    editDetails()->page()->addComboBox(0, 3, 1, 1, "10", hex1, hex2, "02", "large");
    editDetails()->page()->addKnob(0, 4, 1, 1, "10", hex1, hex2, "03", "normal_ratio1.5");
    // formerly "LEVEL" groupBox at addGroupBox(0, 2, 1, 1) — sequential cols 5-6
    editDetails()->page()->addKnob(0, 5, 1, 1, "10", hex1, hex2, "04", "normal_ratio1.75");
    editDetails()->page()->addKnob(0, 6, 1, 1, "10", hex1, hex2, "05", "normal_ratio1.5");
    editDetails()->page()->addStackField();


    // Wah
    editDetails()->page()->newStackField(0);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex2 = "2E";
    }
    else
    {
        hex2 = "31";
    };
    // formerly "WAH" groupBox at addGroupBox(0, 1, 1, 1) — parentCol=1
    editDetails()->page()->addComboBox(0, 1, 1, 1, "10", hex1, hex2, "00", "large");
    editDetails()->page()->addDataKnob(0, 2, 1, 1, "10", hex1, hex2, "02", "0~1000_ratio1.5","bottom", 70);
    editDetails()->page()->addDataKnob(0, 3, 1, 1, "10", hex1, hex2, "06", "0~1000_ratio1.75","bottom", 70);
    editDetails()->page()->addDataKnob(0, 4, 1, 1, "10", hex1, hex2, "0A", "0~1000_ratio1.75","bottom", 70);
    // formerly "LEVEL" groupBox at addGroupBox(0, 2, 1, 1) — sequential cols 5-6
    editDetails()->page()->addKnob(0, 5, 1, 1, "10", hex1, hex2, "01", "normal_ratio2");
    editDetails()->page()->addKnob(0, 6, 1, 1, "10", hex1, hex2, "0E", "normal_ratio1.75");
    editDetails()->page()->addStackField();


    editDetails()->addPage();

}
