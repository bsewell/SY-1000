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

#ifndef CUSTOMCONTROLTABMENU_H
#define CUSTOMCONTROLTABMENU_H

#include <QWidget>
#include <QtWidgets>
#include "customControlLabel.h"

class customControlTabMenu : public QWidget
{
    Q_OBJECT

public:
    customControlTabMenu(QWidget *parent = 0,
        QString hex0 = "void",
		QString hex1 = "void",
		QString hex2 = "void",
		QString hex3 = "void",
		QString direction = "bottom");
		
        QTabBar* controlListTab;

protected:
    void paintEvent(QPaintEvent *event);

public slots:
	void valueChanged(int index);
	void dialogUpdateSignal();
    void set_index(int index);

signals:
	void updateSignal();
	void updateDisplay(QString text);
	void currentIndexChanged(int index);

private:
	void setComboBox();
	customControlLabel* label;
    QString hex0;
	QString hex1;
	QString hex2;
	QString hex3;
	QString area;
	QString direction;
    int comboWidth;
    int _lastValue;
};

#endif // CUSTOMCONTROLLISTMENU_H
