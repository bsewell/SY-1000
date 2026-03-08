/****************************************************************************
**
** Copyright (C) 2007~2025 Colin Willcocks.
** Copyright (C) 2005~2007 Uco Mesdag. 
** All rights reserved.
** This file is part of "VG-99 FloorBoard".
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

#ifndef STOMPBOX_H
#define STOMPBOX_H

#include <QtWidgets>
#include <QHoverEvent>
#include <QTimer>
#include "MidiTable.h"
#include "editWindow.h"
#include "editPage.h"
#include "customControlLabel.h"

class stompBox : public QWidget
{
    Q_OBJECT

public:
    stompBox(
            QWidget *parent = 0,
            uint id = 0,
            QString imagePathOn = ":/images/amp_on.png",
            QString imagePathOff = ":/images/amp_off.png",
            QString imagePathHover = ":/images/effect_hover.png",
            QString imagePathSelected = ":/images/effect_selected.png",
            QPoint stompPos = QPoint(0, 0));
    void setPos(QPoint newPos);
    void setOnImage(QString imagePathOn);
    void setStereoImage(QString imagePathStereoOn);
    void setOffImage(QString imagePathOff);
    void setHoverImage(QString imagePathHover);
    void setSelectedImage(QString imagePathSelected);
    void setSize(QSize newSize);
    void setId(uint id);
    void setButton(QString hex0, QString hex1, QString hex2, QString hex3);
    void setSwitch(QString hex0, QString hex1, QString hex2, QString hex3);
    void setLSB(QString hex1, QString hex2);
    void brushColor(QString BarColor);
    void setLed();
    QRect signalBounds(double ratio) const;
    int signalCenterYOffset(double ratio) const;
    void updateButton(QString hex0, QString hex1, QString hex2, QString hex3);
    void updateSwitch(QString hex0, QString hex1, QString hex2, QString hex3);
    void updateLabel(QString hex0, QString hex1, QString hex2, QString hex3);
    uint getId();
    virtual void setEditPages() {}
    editWindow* editDetails();
    uint id;

public slots:
    void updatePos(signed int offsetDif);
    void setPosMaster(QPoint newPos);
    void valueChanged(int value, QString hex0, QString hex1, QString hex2, QString hex3);
    void valueChanged(bool value, QString hex0, QString hex1, QString hex2, QString hex3);
    void valueChanged(bool value);
    virtual void updateSignal() {}
    void setDisplayToFxName();
    void interStomp(uint id, int value);
    void updateStompPath();
    void amp_ButtonSignal(bool value);
    void ns1_ButtonSignal(bool value);
    void mod_ButtonSignal(bool value);
    /*void mfx_ButtonSignal(bool value);
    void reverb_ButtonSignal(bool value);
    void delay_ButtonSignal(bool value);
    void chorus_ButtonSignal(bool value);
    void eq_ButtonSignal(bool value);*/
    void updateDisplay(QString text);
    void blink();

signals:
    void valueChanged(QString fxName, QString valueName, QString valueStr);
    void currentIndexChanged(int index);
    void dialogUpdateSignal();
    void setEditDialog(editWindow* editDialog);
    void notConnectedSignal();
    void pageUpdateSignal();
    void updateStompBoxes();
    void switchSignal();
    void amp_statusSignal(bool value);
    void ns1_statusSignal(bool value);
    void mod_statusSignal(bool value);
    void sendStompSignal(uint id, int value);
    void update_structure();
    void updateDrop();
    /*void mfx_statusSignal(bool value);
    void chorus_statusSignal(bool value);
    void reverb_statusSignal(bool value);
    void delay_statusSignal(bool value);
    void eq_statusSignal(bool value);*/

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void showEvent(QShowEvent *event);
    void hideEvent(QHideEvent *event);

private:
    void refreshOpaqueBounds();
    QRect scaledOpaqueBounds(double ratio) const;
    void updateFlowLabelLayoutDefaults();
    void positionFlowDescriptionLabel(double ratio);
    void emitValueChanged(QString hex0, QString hex1, QString hex2, QString hex3, QString valueHex);
    editWindow *editDialog;
    QString imagePathOn;
    QString imagePathOff;
    QString imagePathHover;
    QString imagePathSelected;
    QSize stompSize;
    QPoint stompPos;
    QPoint dragStartPosition;
    QPixmap image;
    QPixmap imageOn;
    QPixmap imageOff;
    QPixmap imageHover;
    QPixmap imageSelected;
    QString hex0 = "void";
    QString hex1 = "void";
    QString hex2 = "void";
    QString hex3 = "void";
    QString fxName;
    bool selected =false;
    bool hovered =false;
    bool sw = false;
    uint sel_count;
    QTimer* timer;
    QList<QString> stompOrderName;
    QString stompOrderHex;
    int last_value;
    QString effectType;
    //customControlLabel* effectLabel;
    QLabel* effectLabel;
    int flowLabelGapPx = 10;
    Qt::Alignment flowLabelAlignment = Qt::AlignHCenter | Qt::AlignTop;
    QRect opaqueBounds;
    bool opaqueBoundsValid = false;
    void meter_reset();
    bool t_reset;
};

#endif // STOMPBOX_H

