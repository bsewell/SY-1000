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

#ifndef CUSTOMCONTROLSPINBOX_H
#define CUSTOMCONTROLSPINBOX_H

#include <QWidget>
#include <QtWidgets>
#include <QSpinBox>
#include <QTimer>
#include <QHoverEvent>
#include "customControlLabel.h"

class customControlSpinBox : public QWidget
{
    Q_OBJECT

public:
    customControlSpinBox(QWidget *parent = 0,
                         QString hex0 = "void",
                         QString hex1 = "void",
                         QString hex2 = "void",
                         QString hex3 = "void",
                         QString direction = "bottom");
    QSpinBox* controlSpinBox;

protected:
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);

public slots:
    void valueChanged(int index);
    void dialogUpdateSignal();
    void timed();
    void highlight();

signals:
    void updateSignal();
    void updateDisplay(QString text);
    void currentIndexChanged(int index);

private:
    void setSpinBox();
    customControlLabel* label;
    QString hex0;
    QString hex1;
    QString hex2;
    QString hex3;
    QString area;
    QString direction;
    int spinWidth;
    int rangeMin;
    int rangeMax;
    int rangeOffset;
    int min;
    int max;
    int _lastValue;
    bool hover;
    QTimer* timer;
};

#endif // CUSTOMCONTROLSpinBox_H
