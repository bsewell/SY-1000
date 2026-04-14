/****************************************************************************
**
** Copyright (C) 2026 Colin Willcocks.
** All rights reserved.
** This file is part of "SY-1000 FloorBoard".
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
****************************************************************************/

#include "midiCCHandler.h"
#include "parameterBridge.h"
#include "MidiTable.h"
#include "SysxIO.h"
#include <QDebug>

MidiCCHandler* MidiCCHandler::_instance = nullptr;

MidiCCHandler::MidiCCHandler(QObject *parent)
    : QObject(parent)
{
}

MidiCCHandler* MidiCCHandler::Instance()
{
    if (!_instance)
        _instance = new MidiCCHandler();
    return _instance;
}

void MidiCCHandler::setActiveKnobs(const QVector<KnobAddress> &knobs)
{
    m_activeKnobs = knobs;
}

void MidiCCHandler::clearActiveKnobs()
{
    m_activeKnobs.clear();
}

void MidiCCHandler::setRowCCs(int topCC, int midCC, int botCC, int sliderCC)
{
    m_topCC    = topCC;
    m_midCC    = midCC;
    m_botCC    = botCC;
    m_sliderCC = sliderCC;
}

// LCXL3 template: all on MIDI ch 3, CC# determines row AND column
// Row 0 (top encoders):    CC 13-20, column = CC - 13
// Row 1 (middle encoders): CC 21-28, column = CC - 21
// Row 2 (bottom encoders): CC 29-36, column = CC - 29
// Row 3 (sliders):         CC  5-12, column = CC -  5

int MidiCCHandler::ccToRow(int ccNumber) const
{
    if (ccNumber >= 13 && ccNumber <= 20) return 0;
    if (ccNumber >= 21 && ccNumber <= 28) return 1;
    if (ccNumber >= 29 && ccNumber <= 36) return 2;
    if (ccNumber >=  5 && ccNumber <= 12) return 3;
    return -1;
}

int MidiCCHandler::ccToColumn(int ccNumber) const
{
    if (ccNumber >= 13 && ccNumber <= 20) return ccNumber - 13;
    if (ccNumber >= 21 && ccNumber <= 28) return ccNumber - 21;
    if (ccNumber >= 29 && ccNumber <= 36) return ccNumber - 29;
    if (ccNumber >=  5 && ccNumber <= 12) return ccNumber -  5;
    return -1;
}

int MidiCCHandler::scaleCC(int ccValue, int paramMin, int paramMax) const
{
    if (paramMax <= paramMin) return paramMin;
    // Linear scale: CC 0 → paramMin, CC 127 → paramMax
    double ratio = static_cast<double>(ccValue) / 127.0;
    int scaled = paramMin + static_cast<int>(ratio * (paramMax - paramMin) + 0.5);
    if (scaled < paramMin) scaled = paramMin;
    if (scaled > paramMax) scaled = paramMax;
    return scaled;
}

void MidiCCHandler::handleCC(int channel, int ccNumber, int value)
{
    Q_UNUSED(channel);

    int row = ccToRow(ccNumber);
    int column = ccToColumn(ccNumber);

    if (row < 0 || column < 0) {
        emit ccActivity(QString("CC#%1=%2 [%3 knobs] (unmapped)")
                        .arg(ccNumber).arg(value).arg(m_activeKnobs.size()));
        return;
    }

    // Flat sequential mapping: row 0 = knobs 0-7, row 1 = 8-15, etc.
    int knobIndex = row * 8 + column;

    if (knobIndex < 0 || knobIndex >= m_activeKnobs.size()) {
        emit ccActivity(QString("CC#%1=%2 r%3c%4 → idx %5 (%6 knobs)")
                        .arg(ccNumber).arg(value).arg(row).arg(column)
                        .arg(knobIndex).arg(m_activeKnobs.size()));
        return;
    }

    const KnobAddress &addr = m_activeKnobs[knobIndex];

    // Skip empty padding slots
    if (addr.hex0.isEmpty()) {
        emit ccActivity(QString("CC#%1=%2 r%3c%4 (padding)")
                        .arg(ccNumber).arg(value).arg(row).arg(column));
        return;
    }

    // Get parameter range from MidiTable
    MidiTable *midiTable = MidiTable::Instance();
    int paramMin = midiTable->getRangeMinimum(addr.hex0, addr.hex1, addr.hex2, addr.hex3);
    int paramMax = midiTable->getRange(addr.hex0, addr.hex1, addr.hex2, addr.hex3);

    // Scale CC value to parameter range
    int scaledValue = scaleCC(value, paramMin, paramMax);

    // Set the parameter
    ParameterBridge *bridge = ParameterBridge::Instance();
    bridge->setValue(addr.hex0, addr.hex1, addr.hex2, addr.hex3, scaledValue);

    emit ccActivity(QString("CC#%1=%2 → %3 = %4")
                    .arg(ccNumber).arg(value)
                    .arg(bridge->getLabel(addr.hex0, addr.hex1, addr.hex2, addr.hex3))
                    .arg(scaledValue));
}
