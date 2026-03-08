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

#ifndef CUSTOMMETER_H
#define CUSTOMMETER_H

#include <QWidget>
#include <QTimer>

class customMeter : public QWidget
{
    Q_OBJECT

public:
    customMeter(
		double value = 50, 
        QPoint dialPos = QPoint(0, 0),
		QWidget *parent = 0,
        QString type = "6_band",
		QString imagePath = ":/images/stompknob.png",
		unsigned int imageRange = 10);
	void setValue(int value);

signals:

protected:
   void paintEvent(QPaintEvent *event);

private:
    void setOffset(int value);
	QString imagePath;
	QSize dialSize;
	unsigned int imageRange;
	signed int xOffset;
    double _x;
    double _y;
    QString type;
    QTimer *strobeTimer {nullptr};
    int strobePhase {0};
    int strobeSpeed {0};
};

#endif 
