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

#ifndef CUSTOMSYSTEMOVERRIDE_H
#define CUSTOMSYSTEMOVERRIDE_H

#include <QWidget>
#include <QtWidgets>
#include "customControlLabel.h"

class customSystemOverride : public QWidget
{
    Q_OBJECT

public:
    customSystemOverride(QWidget *parent = 0,
        QString hex0 = "void",
        QString hex1 = "void",
		QString hex2 = "void",
		QString hex3 = "void",
        QString index = "01",
        QString type = "equals",
        int rowSpan = 1,
        int columnSpan = 1,
        QString hex_0 = "void",
        QString hex_1 = "void",
        QString hex_2 = "void",
        QString hex_3 = "void",
        QString value = "void");

protected:
	void paintEvent(QPaintEvent *event);

signals:
    void command_update();

public slots:
	void dialogUpdateSignal();
    void cmd_update();

private:
	customControlLabel* label;
    customControlLabel* label2;
    QString hex0;
	QString hex1;
	QString hex2;
	QString hex3;
    QString index;
    QString type;
    int rowSpan;
    int columnSpan;
    QString hex_0;
    QString hex_1;
    QString hex_2;
    QString hex_3;
    QString value;
    int previous;
    int _lastValue;
    bool disable = false;
};

#endif // CUSTOMSYSTEMOVERRIDE_H
