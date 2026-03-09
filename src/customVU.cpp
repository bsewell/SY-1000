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

#include "customVU.h"
//#include "MidiTable.h"
//#include "SysxIO.h"
#include "Preferences.h"

customVU::customVU(QWidget *parent, QString type) : QWidget(parent)
{
    Preferences *preferences = Preferences::Instance();
    bool ok;
    const double ratio = preferences->getPreferences("Window", "Scale", "ratio").toDouble(&ok);

    QPoint bgPos = QPoint(1*ratio, 1*ratio);
    QPoint knobPos = QPoint(1*ratio, 1*ratio);
    _x = 1; _y = 1;
    if(type=="6_band"){_x = 1; _y = 1;};
    if(type=="54_band"){_x = 0.4; _y = 0.8;};
    if(type=="poly_tuner"){_x = 2.0; _y = 1;};
    if(type=="mono_tuner"){_x = 2.5; _y = 3;};

    QLabel *newBackGround = new QLabel(this);
    QPixmap vu;
    if(type=="6_band" || type=="54_band"){
    vu=(QPixmap(":/images/led_vu.png"));}
    else{vu=(QPixmap(":/images/led_tuner.png"));};
    newBackGround->setPixmap(vu.scaled(vu.width()*ratio*_x,vu.height()*ratio*_y, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    newBackGround->move(bgPos);
    QString imagePath_meter;
    if(type=="6_band" || type=="54_band"){
    imagePath_meter=(":/images/led_vu_mask.png");}
    else{imagePath_meter=(":/images/led_tuner_mask.png");};

    unsigned int imageRange = 100;
    this->meter = new customMeter(10, knobPos, this, type, imagePath_meter, imageRange);
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    this->setFixedSize(newBackGround->pixmap(Qt::ReturnByValue).size() - QSize(0, 4)*ratio);
#else
    this->setFixedSize(newBackGround->pixmap(Qt::ReturnByValue).size() - QSize(0, 4)*ratio);
#endif
}

void customVU::paintEvent(QPaintEvent *)
{

}

void customVU::setValue(int value)
{
    this->meter->setValue(value);
}
