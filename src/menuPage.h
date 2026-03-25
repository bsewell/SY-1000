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

#ifndef MENUPAGE_H
#define MENUPAGE_H

#include <QtWidgets>
//#include "MidiTable.h"
#include "editWindow.h"
//#include "editPage.h"

class menuPage : public QWidget
{
    Q_OBJECT

public:
    menuPage(
            QWidget *parent = 0,
            unsigned int id = 0,
            QString imagePath = ":/images/menuPage_stomp.png",
            QPoint stompPos = QPoint(100, 24));
    void setPos(QPoint newPos);
    void setImage(QString imagePath);
    void setSize(QSize newSize);
    void setId(unsigned int id);
    void setLSB(QString hex1, QString hex2);
    unsigned int getId();
    void updateSwitch(QString hex0, QString hex1, QString hex2, QString hex3);
    virtual void setEditPages() {}
    editWindow* editDetails();

public slots:
    void updatePos(signed int offsetDif);
    void valueChanged(bool value, QString hex1, QString hex2, QString hex3);
    virtual void updateSignal() {}
    void setDisplayToFxName();
    void pedal_ButtonSignal(bool value);
    void system_ButtonSignal(bool value);
    void assign1_ButtonSignal(bool value);
    void setup_ButtonSignal(bool value);
    void tuner_ButtonSignal(bool value);
    void systemReply(QString);
    void generalUpdate();

signals:
    void valueChanged(QString fxName, QString valueName, QString valueStr);
    void currentIndexChanged(int index);
    void dialogUpdateSignal();
    void setEditDialog(editWindow* editDialog);
    void setStatusSymbol(int value);
    void setStatusProgress(int value);
    void setStatusMessage(QString message);
    void systemUpdateSignal();
    void assign1_statusSignal(bool value);

protected:
    void paintEvent(QPaintEvent *event);

private:
    void emitValueChanged(QString hex1, QString hex2, QString hex3, QString valueHex);
    editWindow *editDialog;
    QString imagePath;
    QSize stompSize;
    QPoint stompPos;
    uint id;
    QPoint dragStartPosition;
    QPixmap image;
    QString hex1;
    QString hex2;
    QString hex3;
    QString fxName;
    QString area_mode;
};

#endif
