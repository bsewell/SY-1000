/****************************************************************************
**
** Copyright (C) 2007~2025 Colin Willcocks.
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

#ifndef CUSTOMCONTROLVU_H
#define CUSTOMCONTROLVU_H

#include <QWidget>
#include <QtGui>
#include "customGraphicEQGraph.h"
#include "customVU.h"
#include "customControlLabel.h"
#include "customLed.h"

class customControlVU : public QWidget
{
    Q_OBJECT

public:
    customControlVU(QWidget *parent = 0,
                    QString hex0 = "void",
                    QString hex1 = "void",
                    QString hex2 = "void",
                    QString hex3 = "void",
                    QString background = "normal",
                    QString direction = "bottom",
                    int lenght = 34);
    QTimer* timer;
    QTimer* timer2;

protected:
    void paintEvent(QPaintEvent *event);

public slots:
    void updateDevice(QString rxData);
    void updatePeak();
    void demo();

signals:
    /*  void updateDisplay(QString text);
        void updateDisplay_1(QString text);
        void updateDisplay_2(QString text);
        void updateDisplay_3(QString text);
        void updateDisplay_4(QString text);
        void updateDisplay_5(QString text);
        void updateDisplay_6(QString text);*/

private:
    QString note(int num);
    customControlLabel* label_1;
    customControlLabel* label_2;
    customControlLabel* label_3;
    customControlLabel* label_4;
    customControlLabel* label_5;
    customControlLabel* label_6;
    QLineEdit* display_1;
    QLineEdit* display_2;
    QLineEdit* display_3;
    QLineEdit* display_4;
    QLineEdit* display_5;
    QLineEdit* display_6;
    customVU* vu_1;
    customVU* vu_2;
    customVU* vu_3;
    customVU* vu_4;
    customVU* vu_5;
    customVU* vu_6;
    QString hex_1;
    QString hex_2;
    QString hex_3;
    QString hex_4;
    QString hex_5;
    QString hex_6;
    QString hex0;
    QString hex1;
    QString hex2;
    QString hex3;
    QString area;
    int x1;
    int x2;
    int x3;
    int x4;
    int x5;
    int x6;
    int p1;
    int p2;
    int p3;
    int p4;
    int p5;
    int p6;
    customVU* in_1;
    customVU* in_2;
    customVU* in_3;
    customVU* in_4;
    customVU* in_5;
    customVU* in_6;
    customVU* in_7;
    customVU* in_8;
    customVU* in_9;
    customVU* in_10;
    customVU* in_11;
    customVU* in_12;
    customVU* in_13;
    customVU* in_14;
    customVU* in_15;
    customVU* in_16;
    customVU* in_17;
    customVU* in_18;
    customVU* in_19;
    customVU* in_20;
    customVU* in_21;
    customVU* in_22;
    customVU* in_23;
    customVU* in_24;
    customVU* in_25;
    customVU* in_26;
    customVU* in_27;
    customVU* in_28;
    customVU* in_29;
    customVU* in_30;
    customVU* in_31;
    customVU* in_32;
    customVU* in_33;
    customVU* in_34;
    customVU* in_35;
    customVU* in_36;
    customVU* in_37;
    customVU* in_38;
    customVU* in_39;
    customVU* in_40;
    customVU* in_41;
    customVU* in_42;
    customVU* in_43;
    customVU* in_44;
    customVU* in_45;
    customVU* in_46;
    customVU* in_47;
    customVU* in_48;
    customVU* in_49;
    customVU* in_50;
    customVU* in_51;
    customVU* in_52;
    customVU* in_53;
    customVU* in_54;
    customLed* hiLed_1;
    customLed* hiLed_2;
    customLed* hiLed_3;
    customLed* hiLed_4;
    customLed* hiLed_5;
    customLed* hiLed_6;
    customLed* loLed_1;
    customLed* loLed_2;
    customLed* loLed_3;
    customLed* loLed_4;
    customLed* loLed_5;
    customLed* loLed_6;
    QString type;
    void set6BandValue(QString rxData);
    void set54BandValue(QString rxData);
    void setTunerMonoValue(QString rxData);
    void setTunerPolyValue(QString rxData);
};

#endif // CUSTOMCONTROLGRAPHICEQ_H
