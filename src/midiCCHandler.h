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

#ifndef MIDICCHANDLER_H
#define MIDICCHANDLER_H

#include <QObject>
#include <QVector>
#include <QString>

struct KnobAddress {
    QString hex0;
    QString hex1;
    QString hex2;
    QString hex3;
};

class MidiCCHandler : public QObject
{
    Q_OBJECT

public:
    static MidiCCHandler* Instance();

    // Set the ordered list of active knob addresses from the current panel
    void setActiveKnobs(const QVector<KnobAddress> &knobs);
    void clearActiveKnobs();
    int activeKnobCount() const { return m_activeKnobs.size(); }

    // Launch Control XL CC numbers (configurable)
    void setRowCCs(int topCC, int midCC, int botCC, int sliderCC);

signals:
    void ccActivity(const QString &message);

public slots:
    // Called when a CC message arrives from midiIO
    void handleCC(int channel, int ccNumber, int value);

private:
    MidiCCHandler(QObject *parent = nullptr);
    static MidiCCHandler *_instance;

    QVector<KnobAddress> m_activeKnobs;

    // Default Launch Control XL Factory Template 1 CC numbers
    int m_topCC    = 13;   // Top row knobs
    int m_midCC    = 29;   // Middle row knobs
    int m_botCC    = 49;   // Bottom row knobs
    int m_sliderCC = 77;   // Sliders

    // Map CC number to row (0-3) and column (0-7)
    int ccToRow(int ccNumber) const;
    int ccToColumn(int ccNumber) const;

    // Scale CC 0-127 to parameter min..max
    int scaleCC(int ccValue, int paramMin, int paramMax) const;
};

#endif // MIDICCHANDLER_H
