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

#ifndef CUSTOMDATAKNOB_H
#define CUSTOMDATAKNOB_H

#include <QWidget>
#include <QtWidgets>
#include "customDial.h"

class customDataKnob : public QWidget
{
    Q_OBJECT

public:
    customDataKnob(QWidget *parent = 0,
                   QString hex0 = "void",
                   QString hex1 = "void",
                   QString hex2 = "void",
                   QString hex3 = "void",
                   QString background = "normal",
                   QString direction = "Structure");
    void setValue(int value);
    customDial* knob;

public slots:
    void valueChanged(int, QString, QString, QString, QString);

signals:
    void updateSignal();
    void updateDisplay(QString text);
    void highlightSignal();

protected:
    void paintEvent(QPaintEvent *event);

private:
    QString hex0;
    QString hex1;
    QString hex2;
    QString hex3;
    QString area;
    QString hex_a;
    QString hex_b;
    QString hex_c;
    QString byteSize;
    int dataRange;
    QString background;
    QString colour;
};

#endif // CUSTOMDATAKNOB_H
