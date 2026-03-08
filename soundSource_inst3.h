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

#ifndef SOUNDSOURCE_INST3_H
#define SOUNDSOURCE_INST3_H

#include <QtWidgets>
#include <QWidget>
#include "stompBox.h"

class soundsource_inst3 : public stompBox
{
    Q_OBJECT

public:
    soundsource_inst3(QWidget *parent);
	void setEditPages();

public slots:
	void updateSignal();

private:
    QString str1;
    QString str2;
    QString str3;
    QString str4;
    QString str5;
    QString str6;
    QString hex1;
    QString hex2;
    void stringLevels();
    void altTuning();
    void twelveString();
};

#endif // SOUNDSOURCE_INST3_H
