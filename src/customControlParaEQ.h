/****************************************************************************
**
** Copyright (C) 2007~2025 Colin Willcocks.
** Copyright (C) 2005~2007 Uco Mesdag. 
** All rights reserved.
** This file is part of "GT-100B Fx FloorBoard".
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

#ifndef CUSTOMCONTROLPARAEQ_H
#define CUSTOMCONTROLPARAEQ_H

#include <QWidget>
#include <QtWidgets>
#include <QFrame>
#include "customParaEQGraph.h"
#include "customKnob.h"
#include "customControlLabel.h"

class customControlParaEQ : public QWidget
{
    Q_OBJECT

public:
    customControlParaEQ(QWidget *parent = 0,
                        QString hex0 = "void",
                        QString hex1 = "void",
                        QString hex2 = "void",
                        QString hex3 = "void",
                        QString type = "void",
                        QString levHex = "void",
                        int lenght = 50);

protected:
    void paintEvent(QPaintEvent *event);

public slots:
    void dialogUpdateSignal();
    void LowCutChanged(ushort iLowCut);
    void LowGainChanged(ushort iLowGain);
    void LowMidFreqChanged(ushort iLowMidFreq);
    void LowMidQChanged(ushort iLowMidQ);
    void LowMidGainChanged(ushort iLowMidGain);
    void HighMidFreqChanged(ushort iHighMidFreq);
    void HighMidQChanged(ushort iHighMidQ);
    void HighMidGainChanged(ushort iHighMidGain);
    void HighGainChanged(ushort iHighGain);
    void HighCutChanged(ushort iHighCut);
    void LevelChanged(ushort iLevel);
    void timed();
    void highlight();

signals:
    void updateSignal();
    void updateDisplay(QString text);
    void updateDisplay_1(QString text);
    void updateDisplay_2(QString text);
    void updateDisplay_3(QString text);
    void updateDisplay_4(QString text);
    void updateDisplay_5(QString text);
    void updateDisplay_6(QString text);
    void updateDisplay_7(QString text);
    void updateDisplay_8(QString text);
    void updateDisplay_9(QString text);
    void updateDisplay_10(QString text);
    void updateDisplay_11(QString text);
    void graphUpdateSignal(QString hex_1, QString hex_2, QString hex_3,
                           QString hex_4, QString hex_5, QString hex_6,
                           QString hex_7, QString hex_8, QString hex_9,
                           QString hex_10, QString hex_11);

private:
    customParaEQGraph* frame;
    customControlLabel* label_1;
    customControlLabel* label_2;
    customControlLabel* label_3;
    customControlLabel* label_4;
    customControlLabel* label_5;
    customControlLabel* label_6;
    customControlLabel* label_7;
    customControlLabel* label_8;
    customControlLabel* label_9;
    customControlLabel* label_10;
    customControlLabel* label_11;
    QLineEdit* display_1;
    QLineEdit* display_2;
    QLineEdit* display_3;
    QLineEdit* display_4;
    QLineEdit* display_5;
    QLineEdit* display_6;
    QLineEdit* display_7;
    QLineEdit* display_8;
    QLineEdit* display_9;
    QLineEdit* display_10;
    QLineEdit* display_11;
    customKnob* knob_1;
    customKnob* knob_2;
    customKnob* knob_3;
    customKnob* knob_4;
    customKnob* knob_5;
    customKnob* knob_6;
    customKnob* knob_7;
    customKnob* knob_8;
    customKnob* knob_9;
    customKnob* knob_10;
    customKnob* knob_11;
    QString hex_1;
    QString hex_2;
    QString hex_3;
    QString hex_4;
    QString hex_5;
    QString hex_6;
    QString hex_7;
    QString hex_8;
    QString hex_9;
    QString hex_10;
    QString hex_11;
    QString addr_1;
    QString addr_2;
    QString addr_3;
    QString addr_4;
    QString addr_5;
    QString addr_6;
    QString addr_7;
    QString addr_8;
    QString addr_9;
    QString addr_10;
    QString addr_11;
    QString hex0;
    QString hex1;
    QString hex2;
    QString hex3;
    QString area;
    QString levHex;
    int min;
    int max;
    int _lastValue;
    bool hover;
    QTimer* timer;
    int _last_1;
    int _last_2;
    int _last_3;
    int _last_4;
    int _last_5;
    int _last_6;
    int _last_7;
    int _last_8;
    int _last_9;
    int _last_10;
    int _last_11;
};

#endif // customControlParaEQ_H
