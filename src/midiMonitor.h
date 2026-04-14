/****************************************************************************
**
** Copyright (C) 2026 Brian Sewell.
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

#ifndef MIDIMONITOR_H
#define MIDIMONITOR_H

#include <QWidget>
#include <QTextEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>

class MidiMonitor : public QWidget
{
    Q_OBJECT

public:
    explicit MidiMonitor(QWidget *parent = nullptr);

public slots:
    void logCC(int channel, int ccNumber, int value);
    void logSysEx(const QString &direction, const QString &hexData);
    void clear();

private:
    QTextEdit *logView;
    QCheckBox *showCC;
    QCheckBox *showSysEx;
    QPushButton *clearBtn;
    QLabel *lastCC;
    int messageCount;
    static const int MAX_LINES = 500;
};

#endif // MIDIMONITOR_H
