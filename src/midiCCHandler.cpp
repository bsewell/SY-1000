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

int MidiCCHandler::ccToRow(int ccNumber) const
{
    // Accept CC ranges for each row — covers all 8 LCXL factory templates
    // Top knobs: CC 13-20, Middle: CC 29-36, Bottom: CC 49-56, Sliders: CC 77-84
    if (ccNumber >= 13 && ccNumber <= 20) return 0;
    if (ccNumber >= 29 && ccNumber <= 36) return 1;
    if (ccNumber >= 49 && ccNumber <= 56) return 2;
    if (ccNumber >= 77 && ccNumber <= 84) return 3;
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
    int row = ccToRow(ccNumber);
    if (row < 0) {
        // Show activity for unmapped CCs — helps identify correct CC numbers
        emit ccActivity(QString("CC: Ch%1 CC#%2=%3 [%4 knobs] (unmapped row)")
                        .arg(channel+1).arg(ccNumber).arg(value).arg(m_activeKnobs.size()));
        return;
    }

    // Column from channel (0-based). Knobs laid out: column 0 gets indices 0-3,
    // column 1 gets 4-7, etc. With 8 knobs across, ch1 top=knob0, ch2 top=knob4, etc.
    int column = channel;  // channel is already 0-based from midiIO

    // For panels with fewer knobs (e.g. 8), use sequential mapping:
    // ch1=knob0, ch2=knob1, ..., ch8=knob7 (top row only)
    int knobIndex;
    int knobCount = m_activeKnobs.size();
    if (knobCount <= 8) {
        // Single-row mode: each channel maps to one knob, top row only
        if (row != 0) {
            emit ccActivity(QString("CC: Ch%1 row%2=%3 [%4 knobs] (use top knobs)")
                            .arg(channel+1).arg(row).arg(value).arg(knobCount));
            return;
        }
        knobIndex = column;
    } else {
        // Multi-row mode: 4 knobs per column
        knobIndex = column * 4 + row;
    }

    if (knobIndex < 0 || knobIndex >= m_activeKnobs.size()) {
        emit ccActivity(QString("CC: Ch%1 CC#%2=%3 → knob[%4] (no panel)")
                        .arg(channel+1).arg(ccNumber).arg(value).arg(knobIndex));
        return;
    }

    const KnobAddress &addr = m_activeKnobs[knobIndex];

    // Get parameter range from MidiTable
    MidiTable *midiTable = MidiTable::Instance();
    int paramMin = midiTable->getRangeMinimum(addr.hex0, addr.hex1, addr.hex2, addr.hex3);
    int paramMax = midiTable->getRange(addr.hex0, addr.hex1, addr.hex2, addr.hex3);

    // Scale CC value to parameter range
    int scaledValue = scaleCC(value, paramMin, paramMax);

    // Set the parameter — this updates both the file source and the UI
    ParameterBridge *bridge = ParameterBridge::Instance();
    bridge->setValue(addr.hex0, addr.hex1, addr.hex2, addr.hex3, scaledValue);

    emit ccActivity(QString("CC: Ch%1 CC#%2=%3 → %4 = %5")
                    .arg(channel+1).arg(ccNumber).arg(value)
                    .arg(bridge->getLabel(addr.hex0, addr.hex1, addr.hex2, addr.hex3))
                    .arg(scaledValue));
}
