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

#ifndef CUSTOMKNOBRANGE_H
#define CUSTOMKNOBRANGE_H

#include <QWidget>
#include <QtWidgets>
#include "customRangeDial.h"

class customKnobRange : public QWidget
{
    Q_OBJECT

public:
    customKnobRange(QWidget *parent = 0,
                    QString hex0 = "void",
                    QString hex1 = "void",
                    QString hex2 = "void",
                    QString hexMin = "void",
                    QString hexMax = "void",
                    QString type = "Min");

    void setValue(int value);
    customRangeDial* knob;

public slots:
    void valueChanged(int, QString, QString, QString, QString);

signals:
    void updateSignal();
    void updateDisplayMin(QString text);
    void updateDisplayMax(QString text);
    void highlightSignal(QString id);

protected:
    void paintEvent(QPaintEvent *event);

private:
    QString hex0;
    QString hex1;
    QString hex2;
    QString hex3;
    QString hexMin;
    QString hexMax; 
    QString type;
};

#endif // CUSTOMKNOB_H
