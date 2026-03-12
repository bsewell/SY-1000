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

#include "customControlVU.h"
#include "Preferences.h"
#include "MidiTable.h"
#include "SysxIO.h"
#include <QRandomGenerator>

#include <iostream>

customControlVU::customControlVU(QWidget *parent,
                                 QString hex0, QString hex1, QString hex2, QString hex3,
                                 QString background, QString type, int lenght)
    : QWidget(parent)
{
    QString hex_1 = "00";
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){hex_1 = "02";};
    bool ok;
    const double ratio = preferences->getPreferences("Window", "Scale", "ratio").toDouble(&ok);
    const double fratio = preferences->getPreferences("Window", "Font", "ratio").toDouble(&ok);

    this->type = type;
    this->hex0 = hex0;
    this->hex1 = hex1;
    this->hex2 = hex2;
    this->hex3 = hex3;
    this->area = background;

    if(type=="6_band")
    {
        this->label_1 = new customControlLabel(this);
        this->label_1->setText("STRING 1");
        this->label_1->setAlignment(Qt::AlignCenter);
        this->vu_1 = new customVU(this, type);
        this->display_1 = new QLineEdit(this);
        this->display_1->setObjectName("editdisplay");
        this->display_1->setFixedWidth(lenght);
        this->display_1->setFixedHeight(18*ratio);
        this->display_1->setAlignment(Qt::AlignCenter);
        this->display_1->setDisabled(true);

        QVBoxLayout *vu_1Layout = new QVBoxLayout;
        //vu_1Layout->setMargin(5*ratio);
        vu_1Layout->setSpacing(5*ratio);
        vu_1Layout->addWidget(this->label_1, 0, Qt::AlignCenter);
        vu_1Layout->addWidget(this->vu_1, 0, Qt::AlignCenter);
        vu_1Layout->addWidget(this->display_1, 0, Qt::AlignCenter);
        vu_1Layout->addStretch(0);

        this->label_2 = new customControlLabel(this);
        this->label_2->setText("STRING 2");
        this->label_2->setAlignment(Qt::AlignCenter);
        this->vu_2 = new customVU(this, type);
        this->display_2 = new QLineEdit(this);
        this->display_2->setObjectName("editdisplay");
        this->display_2->setFixedWidth(lenght);
        this->display_2->setFixedHeight(18*ratio);
        this->display_2->setAlignment(Qt::AlignCenter);
        this->display_2->setDisabled(true);

        QVBoxLayout *vu_2Layout = new QVBoxLayout;
        //vu_2Layout->setMargin(5*ratio);
        vu_2Layout->setSpacing(5*ratio);
        vu_2Layout->addWidget(this->label_2, 0, Qt::AlignCenter);
        vu_2Layout->addWidget(this->vu_2, 0, Qt::AlignCenter);
        vu_2Layout->addWidget(this->display_2, 0, Qt::AlignCenter);
        vu_2Layout->addStretch(0);

        this->label_3 = new customControlLabel(this);
        this->label_3->setText("STRING 3");
        this->label_3->setAlignment(Qt::AlignCenter);
        this->vu_3 = new customVU(this, type);
        this->display_3 = new QLineEdit(this);
        this->display_3->setObjectName("editdisplay");
        this->display_3->setFixedWidth(lenght);
        this->display_3->setFixedHeight(18*ratio);
        this->display_3->setAlignment(Qt::AlignCenter);
        this->display_3->setDisabled(true);

        QVBoxLayout *vu_3Layout = new QVBoxLayout;
        //vu_3Layout->setMargin(5*ratio);
        vu_3Layout->setSpacing(5*ratio);
        vu_3Layout->addWidget(this->label_3, 0, Qt::AlignCenter);
        vu_3Layout->addWidget(this->vu_3, 0, Qt::AlignCenter);
        vu_3Layout->addWidget(this->display_3, 0, Qt::AlignCenter);
        vu_3Layout->addStretch(0);

        this->label_4 = new customControlLabel(this);
        this->label_4->setText("STRING 4");
        this->label_4->setAlignment(Qt::AlignCenter);
        this->vu_4 = new customVU(this, type);
        this->display_4 = new QLineEdit(this);
        this->display_4->setObjectName("editdisplay");
        this->display_4->setFixedWidth(lenght);
        this->display_4->setFixedHeight(18*ratio);
        this->display_4->setAlignment(Qt::AlignCenter);
        this->display_4->setDisabled(true);

        QVBoxLayout *vu_4Layout = new QVBoxLayout;
        //vu_4Layout->setMargin(5*ratio);
        vu_4Layout->setSpacing(5*ratio);
        vu_4Layout->addWidget(this->label_4, 0, Qt::AlignCenter);
        vu_4Layout->addWidget(this->vu_4, 1, Qt::AlignCenter);
        vu_4Layout->addWidget(this->display_4, 0, Qt::AlignCenter);
        vu_4Layout->addStretch(0);

        this->label_5 = new customControlLabel(this);
        this->label_5->setText("STRING 5");
        this->label_5->setAlignment(Qt::AlignCenter);
        this->vu_5 = new customVU(this, type);
        this->display_5 = new QLineEdit(this);
        this->display_5->setObjectName("editdisplay");
        this->display_5->setFixedWidth(lenght);
        this->display_5->setFixedHeight(18*ratio);
        this->display_5->setAlignment(Qt::AlignCenter);
        this->display_5->setDisabled(true);

        QVBoxLayout *vu_5Layout = new QVBoxLayout;
        //vu_5Layout->setMargin(5*ratio);
        vu_5Layout->setSpacing(5*ratio);
        vu_5Layout->addWidget(this->label_5, 0, Qt::AlignCenter);
        vu_5Layout->addWidget(this->vu_5, 0, Qt::AlignCenter);
        vu_5Layout->addWidget(this->display_5, 0, Qt::AlignCenter);
        vu_5Layout->addStretch(0);

        this->label_6 = new customControlLabel(this);
        this->label_6->setText("STRING 6");
        this->label_6->setAlignment(Qt::AlignCenter);
        this->vu_6 = new customVU(this, type);
        this->display_6 = new QLineEdit(this);
        this->display_6->setObjectName("editdisplay");
        this->display_6->setFixedWidth(lenght);
        this->display_6->setFixedHeight(18*ratio);
        this->display_6->setAlignment(Qt::AlignCenter);
        this->display_6->setDisabled(true);

        QVBoxLayout *vu_6Layout = new QVBoxLayout;
        //vu_6Layout->setMargin(5*ratio);
        vu_6Layout->setSpacing(5*ratio);
        vu_6Layout->addWidget(this->label_6, 0, Qt::AlignCenter);
        vu_6Layout->addWidget(this->vu_6, 0, Qt::AlignCenter);
        vu_6Layout->addWidget(this->display_6, 0, Qt::AlignCenter);
        vu_6Layout->addStretch(0);

        QHBoxLayout *vuLayout = new QHBoxLayout;
        //vuLayout->setContentsMargins(0, 0, 0, 0);
        vuLayout->setSpacing(10*ratio);
        vuLayout->addLayout(vu_1Layout);  //Band_1
        vuLayout->addLayout(vu_2Layout);  //Band_2
        vuLayout->addLayout(vu_3Layout);  //Band_3
        vuLayout->addLayout(vu_4Layout);  //Band_4
        vuLayout->addLayout(vu_5Layout);  //Band_5
        vuLayout->addLayout(vu_6Layout);  //Band_6
        vuLayout->addStretch(0);

        QVBoxLayout *frameLayout = new QVBoxLayout;
        //frameLayout->setMargin(10*ratio);
        frameLayout->setSpacing(10*ratio);
        frameLayout->addLayout(vuLayout);
        this->setLayout(frameLayout);

        x1=0;
        x2=15;
        x3=30;
        x4=45;
        x5=60;
        x6=75;
    }
    else if(type=="54_band")
    {
        this->in_1 = new customVU(this, type);
        this->in_2 = new customVU(this, type);
        this->in_3 = new customVU(this, type);
        this->in_4 = new customVU(this, type);
        this->in_5 = new customVU(this, type);
        this->in_6 = new customVU(this, type);
        this->in_7 = new customVU(this, type);
        this->in_8 = new customVU(this, type);
        this->in_9 = new customVU(this, type);
        this->in_10 = new customVU(this, type);
        this->in_11 = new customVU(this, type);
        this->in_12 = new customVU(this, type);
        this->in_13 = new customVU(this, type);
        this->in_14 = new customVU(this, type);
        this->in_15 = new customVU(this, type);
        this->in_16 = new customVU(this, type);
        this->in_17 = new customVU(this, type);
        this->in_18 = new customVU(this, type);
        this->in_19 = new customVU(this, type);
        this->in_20 = new customVU(this, type);
        this->in_21 = new customVU(this, type);
        this->in_22 = new customVU(this, type);
        this->in_23 = new customVU(this, type);
        this->in_24 = new customVU(this, type);
        this->in_25 = new customVU(this, type);
        this->in_26 = new customVU(this, type);
        this->in_27 = new customVU(this, type);
        this->in_28 = new customVU(this, type);
        this->in_29 = new customVU(this, type);
        this->in_30 = new customVU(this, type);
        this->in_31 = new customVU(this, type);
        this->in_32 = new customVU(this, type);
        this->in_33 = new customVU(this, type);
        this->in_34 = new customVU(this, type);
        this->in_35 = new customVU(this, type);
        this->in_36 = new customVU(this, type);
        this->in_37 = new customVU(this, type);
        this->in_38 = new customVU(this, type);
        this->in_39 = new customVU(this, type);
        this->in_40 = new customVU(this, type);
        this->in_41 = new customVU(this, type);
        this->in_42 = new customVU(this, type);
        this->in_43 = new customVU(this, type);
        this->in_44 = new customVU(this, type);
        this->in_45 = new customVU(this, type);
        this->in_46 = new customVU(this, type);
        this->in_47 = new customVU(this, type);
        this->in_48 = new customVU(this, type);
        this->in_49 = new customVU(this, type);
        this->in_50 = new customVU(this, type);
        this->in_51 = new customVU(this, type);
        this->in_52 = new customVU(this, type);
        this->in_53 = new customVU(this, type);
        this->in_54 = new customVU(this, type);

        QHBoxLayout *inLayout = new QHBoxLayout;
        //inLayout->setMargin(10*ratio);
        inLayout->setSpacing(2*ratio);
        inLayout->addWidget(this->in_1, 0, Qt::AlignCenter);
        inLayout->addWidget(this->in_2, 0, Qt::AlignCenter);
        inLayout->addWidget(this->in_3, 0, Qt::AlignCenter);
        inLayout->addWidget(this->in_4, 0, Qt::AlignCenter);
        inLayout->addWidget(this->in_5, 0, Qt::AlignCenter);
        inLayout->addWidget(this->in_6, 0, Qt::AlignCenter);
        inLayout->addWidget(this->in_7, 0, Qt::AlignCenter);
        inLayout->addWidget(this->in_8, 0, Qt::AlignCenter);
        inLayout->addWidget(this->in_9, 0, Qt::AlignCenter);
        inLayout->addWidget(this->in_10, 0, Qt::AlignCenter);
        inLayout->addWidget(this->in_11, 0, Qt::AlignCenter);
        inLayout->addWidget(this->in_12, 0, Qt::AlignCenter);
        inLayout->addWidget(this->in_13, 0, Qt::AlignCenter);
        inLayout->addWidget(this->in_14, 0, Qt::AlignCenter);
        inLayout->addWidget(this->in_15, 0, Qt::AlignCenter);
        inLayout->addWidget(this->in_16, 0, Qt::AlignCenter);
        inLayout->addWidget(this->in_17, 0, Qt::AlignCenter);
        inLayout->addWidget(this->in_18, 0, Qt::AlignCenter);
        inLayout->addWidget(this->in_19, 0, Qt::AlignCenter);
        inLayout->addWidget(this->in_20, 0, Qt::AlignCenter);
        inLayout->addWidget(this->in_21, 0, Qt::AlignCenter);
        inLayout->addWidget(this->in_22, 0, Qt::AlignCenter);
        inLayout->addWidget(this->in_23, 0, Qt::AlignCenter);
        inLayout->addWidget(this->in_24, 0, Qt::AlignCenter);
        inLayout->addWidget(this->in_25, 0, Qt::AlignCenter);
        inLayout->addWidget(this->in_26, 0, Qt::AlignCenter);
        inLayout->addWidget(this->in_27, 0, Qt::AlignCenter);
        inLayout->addWidget(this->in_28, 0, Qt::AlignCenter);
        inLayout->addWidget(this->in_29, 0, Qt::AlignCenter);
        inLayout->addWidget(this->in_30, 0, Qt::AlignCenter);
        inLayout->addWidget(this->in_31, 0, Qt::AlignCenter);
        inLayout->addWidget(this->in_32, 0, Qt::AlignCenter);
        inLayout->addWidget(this->in_33, 0, Qt::AlignCenter);
        inLayout->addWidget(this->in_34, 0, Qt::AlignCenter);
        inLayout->addWidget(this->in_35, 0, Qt::AlignCenter);
        inLayout->addWidget(this->in_36, 0, Qt::AlignCenter);
        inLayout->addWidget(this->in_37, 0, Qt::AlignCenter);
        inLayout->addWidget(this->in_38, 0, Qt::AlignCenter);
        inLayout->addWidget(this->in_39, 0, Qt::AlignCenter);
        inLayout->addWidget(this->in_40, 0, Qt::AlignCenter);
        inLayout->addWidget(this->in_41, 0, Qt::AlignCenter);
        inLayout->addWidget(this->in_42, 0, Qt::AlignCenter);
        inLayout->addWidget(this->in_43, 0, Qt::AlignCenter);
        inLayout->addWidget(this->in_44, 0, Qt::AlignCenter);
        inLayout->addWidget(this->in_45, 0, Qt::AlignCenter);
        inLayout->addWidget(this->in_46, 0, Qt::AlignCenter);
        inLayout->addWidget(this->in_47, 0, Qt::AlignCenter);
        inLayout->addWidget(this->in_48, 0, Qt::AlignCenter);
        inLayout->addWidget(this->in_49, 0, Qt::AlignCenter);
        inLayout->addWidget(this->in_50, 0, Qt::AlignCenter);
        inLayout->addWidget(this->in_51, 0, Qt::AlignCenter);
        inLayout->addWidget(this->in_52, 0, Qt::AlignCenter);
        inLayout->addWidget(this->in_53, 0, Qt::AlignCenter);
        inLayout->addWidget(this->in_54, 0, Qt::AlignCenter);
        inLayout->addStretch(0);
        this->setLayout(inLayout);
    }
    else if(type=="poly_tuner")
    {
        int lenght = 50*ratio;
        QFont Sfont( "Roboto Condensed", 14*fratio, QFont::Normal);

        this->label_1 = new customControlLabel(this);
        this->label_1->setText("STRING 1");
        this->label_1->setAlignment(Qt::AlignCenter);
        this->vu_1 = new customVU(this, type);
        this->display_1 = new QLineEdit(this);
        this->display_1->setObjectName("editdisplay");
        this->display_1->setFont(Sfont);
        this->display_1->setFixedWidth(lenght);
        this->display_1->setFixedHeight(45*ratio);
        this->display_1->setAlignment(Qt::AlignCenter);
        this->display_1->setDisabled(true);
        this->hiLed_1 = new customLed(false, 0.15, this, ":/images/led_hi.png");
        this->loLed_1 = new customLed(false, 0.15, this, ":/images/led_lo.png");
        QVBoxLayout *vu_1loLayout = new QVBoxLayout;
        vu_1loLayout->addWidget(loLed_1);
        QVBoxLayout *vu_1hiLayout = new QVBoxLayout;
        vu_1hiLayout->addWidget(hiLed_1);
        QHBoxLayout *vu_1HLayout = new QHBoxLayout;
        vu_1HLayout->addLayout(vu_1loLayout, Qt::AlignCenter);
        vu_1HLayout->addSpacing(1*ratio);
        vu_1HLayout->addWidget(this->display_1, Qt::AlignCenter);
        vu_1HLayout->addSpacing(1*ratio);
        vu_1HLayout->addLayout(vu_1hiLayout, Qt::AlignCenter);
        vu_1HLayout->setSpacing(5*ratio);
        vu_1HLayout->addWidget(this->vu_1, Qt::AlignCenter);
        QVBoxLayout *vu_1VLayout = new QVBoxLayout;
        vu_1VLayout->setSpacing(2*ratio);
        vu_1VLayout->addWidget(this->label_1, Qt::AlignCenter);
        vu_1VLayout->addLayout(vu_1HLayout);

        this->label_2 = new customControlLabel(this);
        this->label_2->setText("STRING 2");
        this->label_2->setAlignment(Qt::AlignCenter);
        this->vu_2 = new customVU(this, type);
        this->display_2 = new QLineEdit(this);
        this->display_2->setObjectName("editdisplay");
        this->display_2->setFont(Sfont);
        this->display_2->setFixedWidth(lenght);
        this->display_2->setFixedHeight(45*ratio);
        this->display_2->setAlignment(Qt::AlignCenter);
        this->display_2->setDisabled(true);
        this->hiLed_2 = new customLed(false, 0.15, this, ":/images/led_hi.png");
        this->loLed_2 = new customLed(false, 0.15, this, ":/images/led_lo.png");
        QVBoxLayout *vu_2loLayout = new QVBoxLayout;
        vu_2loLayout->addWidget(loLed_2);
        QVBoxLayout *vu_2hiLayout = new QVBoxLayout;
        vu_2hiLayout->addWidget(hiLed_2);
        QHBoxLayout *vu_2HLayout = new QHBoxLayout;
        vu_2HLayout->addLayout(vu_2loLayout, Qt::AlignCenter);
        vu_2HLayout->addSpacing(1*ratio);
        vu_2HLayout->addWidget(this->display_2, Qt::AlignCenter);
        vu_2HLayout->addSpacing(1*ratio);
        vu_2HLayout->addLayout(vu_2hiLayout, Qt::AlignCenter);
        vu_2HLayout->setSpacing(5*ratio);
        vu_2HLayout->addWidget(this->vu_2, Qt::AlignCenter);
        QVBoxLayout *vu_2VLayout = new QVBoxLayout;
        vu_2VLayout->setSpacing(2*ratio);
        vu_2VLayout->addWidget(this->label_2, Qt::AlignCenter);
        vu_2VLayout->addLayout(vu_2HLayout);

        this->label_3 = new customControlLabel(this);
        this->label_3->setText("STRING 3");
        this->label_3->setAlignment(Qt::AlignCenter);
        this->vu_3 = new customVU(this, type);
        this->display_3 = new QLineEdit(this);
        this->display_3->setObjectName("editdisplay");
        this->display_3->setFont(Sfont);
        this->display_3->setFixedWidth(lenght);
        this->display_3->setFixedHeight(45*ratio);
        this->display_3->setAlignment(Qt::AlignCenter);
        this->display_3->setDisabled(true);
        this->hiLed_3 = new customLed(false, 0.15, this, ":/images/led_hi.png");
        this->loLed_3 = new customLed(false, 0.15, this, ":/images/led_lo.png");
        QVBoxLayout *vu_3loLayout = new QVBoxLayout;
        vu_3loLayout->addWidget(loLed_3);
        QVBoxLayout *vu_3hiLayout = new QVBoxLayout;
        vu_3hiLayout->addWidget(hiLed_3);
        QHBoxLayout *vu_3HLayout = new QHBoxLayout;
        vu_3HLayout->addLayout(vu_3loLayout, Qt::AlignCenter);
        vu_3HLayout->addSpacing(1*ratio);
        vu_3HLayout->addWidget(this->display_3, Qt::AlignCenter);
        vu_3HLayout->addSpacing(1*ratio);
        vu_3HLayout->addLayout(vu_3hiLayout, Qt::AlignCenter);
        vu_3HLayout->setSpacing(5*ratio);
        vu_3HLayout->addWidget(this->vu_3, Qt::AlignCenter);
        QVBoxLayout *vu_3VLayout = new QVBoxLayout;
        vu_3VLayout->setSpacing(2*ratio);
        vu_3VLayout->addWidget(this->label_3, Qt::AlignCenter);
        vu_3VLayout->addLayout(vu_3HLayout);

        this->label_4 = new customControlLabel(this);
        this->label_4->setText("STRING 4");
        this->label_4->setAlignment(Qt::AlignCenter);
        this->vu_4 = new customVU(this, type);
        this->display_4 = new QLineEdit(this);
        this->display_4->setObjectName("editdisplay");
        this->display_4->setFont(Sfont);
        this->display_4->setFixedWidth(lenght);
        this->display_4->setFixedHeight(45*ratio);
        this->display_4->setAlignment(Qt::AlignCenter);
        this->display_4->setDisabled(true);
        this->hiLed_4 = new customLed(false, 0.15, this, ":/images/led_hi.png");
        this->loLed_4 = new customLed(false, 0.15, this, ":/images/led_lo.png");
        QVBoxLayout *vu_4loLayout = new QVBoxLayout;
        vu_4loLayout->addWidget(loLed_4);
        QVBoxLayout *vu_4hiLayout = new QVBoxLayout;
        vu_4hiLayout->addWidget(hiLed_4);
        QHBoxLayout *vu_4HLayout = new QHBoxLayout;
        vu_4HLayout->addLayout(vu_4loLayout, Qt::AlignCenter);
        vu_4HLayout->addSpacing(1*ratio);
        vu_4HLayout->addWidget(this->display_4, Qt::AlignCenter);
        vu_4HLayout->addSpacing(1*ratio);
        vu_4HLayout->addLayout(vu_4hiLayout, Qt::AlignCenter);
        vu_4HLayout->setSpacing(5*ratio);
        vu_4HLayout->addWidget(this->vu_4, Qt::AlignCenter);
        QVBoxLayout *vu_4VLayout = new QVBoxLayout;
        vu_4VLayout->setSpacing(2*ratio);
        vu_4VLayout->addWidget(this->label_4, Qt::AlignCenter);
        vu_4VLayout->addLayout(vu_4HLayout);

        this->label_5 = new customControlLabel(this);
        this->label_5->setText("STRING 5");
        this->label_5->setAlignment(Qt::AlignCenter);
        this->vu_5 = new customVU(this, type);
        this->display_5 = new QLineEdit(this);
        this->display_5->setObjectName("editdisplay");
        this->display_5->setFont(Sfont);
        this->display_5->setFixedWidth(lenght);
        this->display_5->setFixedHeight(45*ratio);
        this->display_5->setAlignment(Qt::AlignCenter);
        this->display_5->setDisabled(true);
        this->hiLed_5 = new customLed(false, 0.15, this, ":/images/led_hi.png");
        this->loLed_5 = new customLed(false, 0.15, this, ":/images/led_lo.png");
        QVBoxLayout *vu_5loLayout = new QVBoxLayout;
        vu_5loLayout->addWidget(loLed_5);
        QVBoxLayout *vu_5hiLayout = new QVBoxLayout;
        vu_5hiLayout->addWidget(hiLed_5);
        QHBoxLayout *vu_5HLayout = new QHBoxLayout;
        vu_5HLayout->addLayout(vu_5loLayout, Qt::AlignCenter);
        vu_5HLayout->addSpacing(1*ratio);
        vu_5HLayout->addWidget(this->display_5, Qt::AlignCenter);
        vu_5HLayout->addSpacing(1*ratio);
        vu_5HLayout->addLayout(vu_5hiLayout, Qt::AlignCenter);
        vu_5HLayout->setSpacing(5*ratio);
        vu_5HLayout->addWidget(this->vu_5, Qt::AlignCenter);
        QVBoxLayout *vu_5VLayout = new QVBoxLayout;
        vu_5VLayout->setSpacing(2*ratio);
        vu_5VLayout->addWidget(this->label_5, Qt::AlignCenter);
        vu_5VLayout->addLayout(vu_5HLayout);

        this->label_6 = new customControlLabel(this);
        this->label_6->setText("STRING 6");
        this->label_6->setAlignment(Qt::AlignCenter);
        this->vu_6 = new customVU(this, type);
        this->display_6 = new QLineEdit(this);
        this->display_6->setObjectName("editdisplay");
        this->display_6->setFont(Sfont);
        this->display_6->setFixedWidth(lenght);
        this->display_6->setFixedHeight(45*ratio);
        this->display_6->setAlignment(Qt::AlignCenter);
        this->display_6->setDisabled(true);
        this->hiLed_6 = new customLed(false, 0.15, this, ":/images/led_hi.png");
        this->loLed_6 = new customLed(false, 0.15, this, ":/images/led_lo.png");
        QVBoxLayout *vu_6loLayout = new QVBoxLayout;
        vu_6loLayout->addWidget(loLed_6);
        QVBoxLayout *vu_6hiLayout = new QVBoxLayout;
        vu_6hiLayout->addWidget(hiLed_6);
        QHBoxLayout *vu_6HLayout = new QHBoxLayout;
        vu_6HLayout->addLayout(vu_6loLayout, Qt::AlignCenter);
        vu_6HLayout->addSpacing(1*ratio);
        vu_6HLayout->addWidget(this->display_6, Qt::AlignCenter);
        vu_6HLayout->addSpacing(1*ratio);
        vu_6HLayout->addLayout(vu_6hiLayout, Qt::AlignCenter);
        vu_6HLayout->setSpacing(5*ratio);
        vu_6HLayout->addWidget(this->vu_6, Qt::AlignCenter);
        QVBoxLayout *vu_6VLayout = new QVBoxLayout;
        vu_6VLayout->setSpacing(2*ratio);
        vu_6VLayout->addWidget(this->label_6, Qt::AlignCenter);
        vu_6VLayout->addLayout(vu_6HLayout);

        QVBoxLayout *vuLayout = new QVBoxLayout;
        //vuLayout->setMargin(10*ratio);
        vuLayout->setSpacing(10*ratio);
        vuLayout->addLayout(vu_1VLayout);  //Band_1
        vuLayout->addLayout(vu_2VLayout);  //Band_2
        vuLayout->addLayout(vu_3VLayout);  //Band_3
        vuLayout->addLayout(vu_4VLayout);  //Band_4
        vuLayout->addLayout(vu_5VLayout);  //Band_5
        vuLayout->addLayout(vu_6VLayout);  //Band_6
        this->setLayout(vuLayout);
    }
    else
    {  // mono_tuner.
        int lenght = 200*ratio;
        QFont Sfont( "Roboto Condensed", 96*fratio, QFont::Bold);
        this->vu_1 = new customVU(this, type);
        this->display_1 = new QLineEdit(this);
        this->display_1->setObjectName("editdisplay");
        this->display_1->setFont(Sfont);
        this->display_1->setFixedWidth(lenght);
        this->display_1->setFixedHeight(160*ratio);
        this->display_1->setAlignment(Qt::AlignCenter);
        this->display_1->setDisabled(true);
        this->hiLed_1 = new customLed(false, 1.00, this, ":/images/led_hi.png");
        this->loLed_1 = new customLed(false, 1.00, this, ":/images/led_lo.png");

        QVBoxLayout *vu_1loLayout = new QVBoxLayout;
        vu_1loLayout->addWidget(loLed_1);
        QVBoxLayout *vu_1hiLayout = new QVBoxLayout;
        vu_1hiLayout->addWidget(hiLed_1);

        QHBoxLayout *vu_1HLayout = new QHBoxLayout;
        vu_1HLayout->addLayout(vu_1loLayout, Qt::AlignHCenter);
        vu_1HLayout->addSpacing(10*ratio);
        vu_1HLayout->addWidget(this->display_1, Qt::AlignHCenter);
        vu_1HLayout->addSpacing(15*ratio);
        vu_1HLayout->addLayout(vu_1hiLayout, Qt::AlignHCenter);

        QVBoxLayout *vu_1VLayout = new QVBoxLayout;
        vu_1VLayout->setSpacing(10*ratio);
        vu_1VLayout->addLayout(vu_1HLayout);
        vu_1VLayout->addWidget(this->vu_1, 0, Qt::AlignCenter);
        this->setLayout(vu_1VLayout);

    };

    if(preferences->getPreferences("Midi", "DBug", "bool")=="true")
    {
        timer = new QTimer(this);
        QObject::connect(timer, SIGNAL(timeout()), this, SLOT(demo()) );
        timer->start(300);
    };

    timer2 = new QTimer(this);
    QObject::connect(timer2, SIGNAL(timeout()), this, SLOT(updatePeak()) );
    timer2->start(3000);

    SysxIO *sysxIO = SysxIO::Instance();
    QObject::connect(sysxIO, SIGNAL(meterSignal(QString)), this, SLOT(updateDevice(QString)) );
};


void customControlVU::paintEvent(QPaintEvent *)
{

};

void customControlVU::updateDevice(QString rxData)
{
        if(this->type=="poly_tuner" && rxData.size()==112){
            setTunerPolyValue(rxData);
        };

        if(this->type=="mono_tuner" && rxData.size()==112){
            setTunerMonoValue(rxData);
        };

        if(this->type=="54_band" && rxData.size()==140){
            set54BandValue(rxData);
        };

        if(this->type=="6_band" && rxData.size()==38){
            set6BandValue(rxData);
        };
}

QString customControlVU::note(int num)
{
    QString note="C";
    if(num==1){note="C#";};
    if(num==2){note="D";};
    if(num==3){note="D#";};
    if(num==4){note="E";};
    if(num==5){note="F";};
    if(num==6){note="F#";};
    if(num==7){note="G";};
    if(num==8){note="G#";};
    if(num==9){note="A";};
    if(num==10){note="A#";};
    if(num==11){note="B";};
    return(note);
}
void customControlVU::updatePeak()
{
    p1=0;
    p2=0;
    p3=0;
    p4=0;
    p5=0;
    p6=0;
    if(type.contains("mono_tuner"))
    {
        vu_1->setValue(100);
        hiLed_1->setValue(false);
        loLed_1->setValue(false);
    };
    if(type.contains("poly_tuner"))
    {
        vu_1->setValue(100);
        vu_2->setValue(100);
        vu_3->setValue(100);
        vu_4->setValue(100);
        vu_5->setValue(100);
        vu_6->setValue(100);
        hiLed_1->setValue(false);
        loLed_1->setValue(false);
        hiLed_2->setValue(false);
        loLed_2->setValue(false);
        hiLed_3->setValue(false);
        loLed_3->setValue(false);
        hiLed_4->setValue(false);
        loLed_4->setValue(false);
        hiLed_5->setValue(false);
        loLed_5->setValue(false);
        hiLed_6->setValue(false);
        loLed_6->setValue(false);
    };
}

void customControlVU::setTunerPolyValue(QString rxData)
{
    bool ok;
    QString pitch = rxData.mid(27, 1) + rxData.mid(29, 1) + rxData.mid(31, 1) + rxData.mid(33, 1);
    x1=10000-(pitch.toUInt(&ok, 16));
    pitch = rxData.mid(39, 1) + rxData.mid(41, 1) + rxData.mid(43, 1) + rxData.mid(45, 1);
    x2=10000-(pitch.toUInt(&ok, 16));
    pitch = rxData.mid(51, 1) + rxData.mid(53, 1) + rxData.mid(55, 1) + rxData.mid(57, 1);
    x3=10000-(pitch.toUInt(&ok, 16));
    pitch = rxData.mid(63, 1) + rxData.mid(65, 1) + rxData.mid(67, 1) + rxData.mid(69, 1);
    x4=10000-(pitch.toUInt(&ok, 16));
    pitch = rxData.mid(75, 1) + rxData.mid(77, 1) + rxData.mid(79, 1) + rxData.mid(81, 1);
    x5=10000-(pitch.toUInt(&ok, 16));
    pitch = rxData.mid(87, 1) + rxData.mid(89, 1) + rxData.mid(91, 1) + rxData.mid(93, 1);
    x6=10000-(pitch.toUInt(&ok, 16));

    MidiTable *midiTable = MidiTable::Instance();
    QString valueStr = midiTable->getValue("Tables", "00", "00", "7F", rxData.mid(24, 2));
    this->display_1->setText(valueStr);
    if(valueStr != "  "){vu_1->setValue(x1/100);};
    valueStr = midiTable->getValue("Tables", "00", "00", "7F", rxData.mid(36, 2));
    this->display_2->setText(valueStr);
    if(valueStr != "  "){vu_2->setValue(x1/100);};
    valueStr = midiTable->getValue("Tables", "00", "00", "7F", rxData.mid(48, 2));
    this->display_3->setText(valueStr);
    if(valueStr != "  "){vu_3->setValue(x1/100);};
    valueStr = midiTable->getValue("Tables", "00", "00", "7F", rxData.mid(60, 2));
    this->display_4->setText(valueStr);
    if(valueStr != "  "){vu_4->setValue(x1/100);};
    valueStr = midiTable->getValue("Tables", "00", "00", "7F", rxData.mid(72, 2));
    this->display_5->setText(valueStr);
    if(valueStr != "  "){vu_5->setValue(x1/100);};
    valueStr = midiTable->getValue("Tables", "00", "00", "7F", rxData.mid(84, 2));
    this->display_6->setText(valueStr);
    if(valueStr != "  "){vu_6->setValue(x1/100);};
    bool hi = false;
    bool lo = false;
    if(QVariant(rxData.mid(35, 1)).toBool()==true){hi=true; lo=true;};
    if(QVariant(pitch.toInt(&ok, 16)).toInt()>4200 && (pitch.toInt(&ok, 16))<5000){lo=true;};
    if(QVariant(pitch.toInt(&ok, 16)).toInt()>5000 && (pitch.toInt(&ok, 16))<5800){hi=true;};
    this->hiLed_1->setValue(hi);
    this->loLed_1->setValue(lo);
    hi = false;
    lo = false;
    if(QVariant(rxData.mid(47, 1)).toBool()==true){hi=true; lo=true;};
    if(QVariant(pitch.toInt(&ok, 16)).toInt()>4200 && (pitch.toInt(&ok, 16))<5000){lo=true;};
    if(QVariant(pitch.toInt(&ok, 16)).toInt()>5000 && (pitch.toInt(&ok, 16))<5800){hi=true;};
    this->hiLed_2->setValue(hi);
    this->loLed_2->setValue(lo);
    hi = false;
    lo = false;
    if(QVariant(rxData.mid(59, 1)).toBool()==true){hi=true; lo=true;};
    if(QVariant(pitch.toInt(&ok, 16)).toInt()>4200 && (pitch.toInt(&ok, 16))<5000){lo=true;};
    if(QVariant(pitch.toInt(&ok, 16)).toInt()>5000 && (pitch.toInt(&ok, 16))<5800){hi=true;};
    this->hiLed_3->setValue(hi);
    this->loLed_3->setValue(lo);
    hi = false;
    lo = false;
    if(QVariant(rxData.mid(71, 1)).toBool()==true){hi=true; lo=true;};
    if(QVariant(pitch.toInt(&ok, 16)).toInt()>4200 && (pitch.toInt(&ok, 16))<5000){lo=true;};
    if(QVariant(pitch.toInt(&ok, 16)).toInt()>5000 && (pitch.toInt(&ok, 16))<5800){hi=true;};
    this->hiLed_4->setValue(hi);
    this->loLed_4->setValue(lo);
    hi = false;
    lo = false;
    if(QVariant(rxData.mid(83, 1)).toBool()==true){hi=true; lo=true;};
    if(QVariant(pitch.toInt(&ok, 16)).toInt()>4200 && (pitch.toInt(&ok, 16))<5000){lo=true;};
    if(QVariant(pitch.toInt(&ok, 16)).toInt()>5000 && (pitch.toInt(&ok, 16))<5800){hi=true;};
    this->hiLed_5->setValue(hi);
    this->loLed_5->setValue(lo);
    hi = false;
    lo = false;
    if(QVariant(rxData.mid(95, 1)).toBool()==true){hi=true; lo=true;};
    if(QVariant(pitch.toInt(&ok, 16)).toInt()>4200 && (pitch.toInt(&ok, 16))<5000){lo=true;};
    if(QVariant(pitch.toInt(&ok, 16)).toInt()>5000 && (pitch.toInt(&ok, 16))<5800){hi=true;};
    this->hiLed_6->setValue(hi);
    this->loLed_6->setValue(lo);
    QString debug;
    for(int v=96; v<108; v++){debug.append(rxData.mid(v,2)+" "); v=v+1;};
    //std::string st = QString("updateDevice= "+debug).toStdString();
    //std::cerr << st << std::endl;
}

void customControlVU::setTunerMonoValue(QString rxData)
{
    bool ok;
    QString pitch = rxData.mid(99, 1) + rxData.mid(101, 1) + rxData.mid(103, 1) + rxData.mid(105, 1);
    x1=10000-(pitch.toUInt(&ok, 16));

    MidiTable *midiTable = MidiTable::Instance();
    QString valueStr = midiTable->getValue("Tables", "00", "00", "7F", rxData.mid(96, 2));
    this->display_1->setText(valueStr);
    if(valueStr != "  "){vu_1->setValue(x1/100);};
    bool hi = false;
    bool lo = false;
    if(QVariant(rxData.mid(107, 1)).toBool()==true){hi=true; lo=true;};
    if(QVariant(pitch.toInt(&ok, 16)).toInt()>4200 && (pitch.toInt(&ok, 16))<5000){lo=true;};
    if(QVariant(pitch.toInt(&ok, 16)).toInt()>5000 && (pitch.toInt(&ok, 16))<5800){hi=true;};
    this->hiLed_1->setValue(hi);
    this->loLed_1->setValue(lo);   //QVariant(rxData.mid(107, 1)).toBool()
    QString debug;
    for(int v=96; v<108; v++){debug.append(rxData.mid(v,2)+" "); v=v+1;};
    //std::string st = QString("updateDevice= "+debug).toStdString();
    //std::cerr << st << std::endl;
}

void customControlVU::set6BandValue(QString rxData)
{
    bool ok;
    x1=(rxData.mid(24, 2).toUInt(&ok, 16))*9;
    x2=(rxData.mid(26, 2).toUInt(&ok, 16))*9;
    x3=(rxData.mid(28, 2).toUInt(&ok, 16))*9;
    x4=(rxData.mid(30, 2).toUInt(&ok, 16))*9;
    x5=(rxData.mid(32, 2).toUInt(&ok, 16))*9;
    x6=(rxData.mid(34, 2).toUInt(&ok, 16))*9;
    vu_1->setValue(x1);
    vu_2->setValue(x2);
    vu_3->setValue(x3);
    vu_4->setValue(x4);
    vu_5->setValue(x5);
    vu_6->setValue(x6);
    if(x1>p1)p1=x1;
    if(x2>p2)p2=x2;
    if(x3>p3)p3=x3;
    if(x4>p4)p4=x4;
    if(x5>p5)p5=x5;
    if(x6>p6)p6=x6;
    this->display_1->setText(QString::number(p1));
    this->display_2->setText(QString::number(p2));
    this->display_3->setText(QString::number(p3));
    this->display_4->setText(QString::number(p4));
    this->display_5->setText(QString::number(p5));
    this->display_6->setText(QString::number(p6));
}

void customControlVU::set54BandValue(QString rxData)
{
    bool ok;
    in_1->setValue((rxData.mid(24, 2).toUInt(&ok, 16)-38)*3);
    in_2->setValue((rxData.mid(26, 2).toUInt(&ok, 16)-38)*3);
    in_3->setValue((rxData.mid(28, 2).toUInt(&ok, 16)-38)*3);
    in_4->setValue((rxData.mid(30, 2).toUInt(&ok, 16)-38)*3);
    in_5->setValue((rxData.mid(32, 2).toUInt(&ok, 16)-38)*3);
    in_6->setValue((rxData.mid(38, 2).toUInt(&ok, 16)-38)*3);
    in_7->setValue((rxData.mid(36, 2).toUInt(&ok, 16)-38)*3);
    in_8->setValue((rxData.mid(38, 2).toUInt(&ok, 16)-38)*3);
    in_9->setValue((rxData.mid(40, 2).toUInt(&ok, 16)-38)*3);
    in_10->setValue((rxData.mid(42, 2).toUInt(&ok, 16)-38)*3);
    in_11->setValue((rxData.mid(44, 2).toUInt(&ok, 16)-38)*3);
    in_12->setValue((rxData.mid(46, 2).toUInt(&ok, 16)-38)*3);
    in_13->setValue((rxData.mid(48, 2).toUInt(&ok, 16)-38)*3);
    in_14->setValue((rxData.mid(50, 2).toUInt(&ok, 16)-38)*3);
    in_15->setValue((rxData.mid(52, 2).toUInt(&ok, 16)-38)*3);
    in_16->setValue((rxData.mid(54, 2).toUInt(&ok, 16)-38)*3);
    in_17->setValue((rxData.mid(56, 2).toUInt(&ok, 16)-38)*3);
    in_18->setValue((rxData.mid(58, 2).toUInt(&ok, 16)-38)*3);
    in_19->setValue((rxData.mid(60, 2).toUInt(&ok, 16)-38)*3);
    in_20->setValue((rxData.mid(62, 2).toUInt(&ok, 16)-38)*3);
    in_21->setValue((rxData.mid(64, 2).toUInt(&ok, 16)-38)*3);
    in_22->setValue((rxData.mid(66, 2).toUInt(&ok, 16)-38)*3);
    in_23->setValue((rxData.mid(68, 2).toUInt(&ok, 16)-38)*3);
    in_24->setValue((rxData.mid(70, 2).toUInt(&ok, 16)-38)*3);
    in_25->setValue((rxData.mid(72, 2).toUInt(&ok, 16)-38)*3);
    in_26->setValue((rxData.mid(74, 2).toUInt(&ok, 16)-38)*3);
    in_27->setValue((rxData.mid(76, 2).toUInt(&ok, 16)-38)*3);
    in_28->setValue((rxData.mid(78, 2).toUInt(&ok, 16)-38)*3);
    in_29->setValue((rxData.mid(80, 2).toUInt(&ok, 16)-38)*3);
    in_30->setValue((rxData.mid(82, 2).toUInt(&ok, 16)-38)*3);
    in_31->setValue((rxData.mid(84, 2).toUInt(&ok, 16)-38)*3);
    in_32->setValue((rxData.mid(86, 2).toUInt(&ok, 16)-38)*3);
    in_33->setValue((rxData.mid(88, 2).toUInt(&ok, 16)-38)*3);
    in_34->setValue((rxData.mid(90, 2).toUInt(&ok, 16)-38)*3);
    in_35->setValue((rxData.mid(92, 2).toUInt(&ok, 16)-38)*3);
    in_36->setValue((rxData.mid(94, 2).toUInt(&ok, 16)-38)*3);
    in_37->setValue((rxData.mid(96, 2).toUInt(&ok, 16)-38)*3);
    in_38->setValue((rxData.mid(98, 2).toUInt(&ok, 16)-38)*3);
    in_39->setValue((rxData.mid(100, 2).toUInt(&ok, 16)-38)*3);
    in_40->setValue((rxData.mid(102, 2).toUInt(&ok, 16)-38)*3);
    in_41->setValue((rxData.mid(104, 2).toUInt(&ok, 16)-38)*3);
    in_42->setValue((rxData.mid(106, 2).toUInt(&ok, 16)-38)*3);
    in_43->setValue((rxData.mid(108, 2).toUInt(&ok, 16)-38)*3);
    in_44->setValue((rxData.mid(110, 2).toUInt(&ok, 16)-38)*3);
    in_45->setValue((rxData.mid(112, 2).toUInt(&ok, 16)-38)*3);
    in_46->setValue((rxData.mid(114, 2).toUInt(&ok, 16)-38)*3);
    in_47->setValue((rxData.mid(116, 2).toUInt(&ok, 16)-38)*3);
    in_48->setValue((rxData.mid(118, 2).toUInt(&ok, 16)-38)*3);
    in_49->setValue((rxData.mid(120, 2).toUInt(&ok, 16)-38)*3);
    in_50->setValue((rxData.mid(122, 2).toUInt(&ok, 16)-38)*3);
    in_51->setValue((rxData.mid(124, 2).toUInt(&ok, 16)-38)*3);
    in_52->setValue((rxData.mid(126, 2).toUInt(&ok, 16)-38)*3);
    in_53->setValue((rxData.mid(128, 2).toUInt(&ok, 16)-38)*3);
    in_54->setValue((rxData.mid(130, 2).toUInt(&ok, 16)-38)*3);
}

void customControlVU::demo()
{
        if(type=="6_band"){
            x1=QRandomGenerator::global()->bounded(101);
            x2=QRandomGenerator::global()->bounded(101);
            x3=QRandomGenerator::global()->bounded(101);
            x4=QRandomGenerator::global()->bounded(101);
            x5=QRandomGenerator::global()->bounded(101);
            x6=QRandomGenerator::global()->bounded(101);
            vu_1->setValue(x1);
            vu_2->setValue(x2);
            vu_3->setValue(x3);
            vu_4->setValue(x4);
            vu_5->setValue(x5);
            vu_6->setValue(x6);
            if(x1>p1)p1=x1;
            if(x2>p2)p2=x2;
            if(x3>p3)p3=x3;
            if(x4>p4)p4=x4;
            if(x5>p5)p5=x5;
            if(x6>p6)p6=x6;
            this->display_1->setText(QString::number(p1));
            this->display_2->setText(QString::number(p2));
            this->display_3->setText(QString::number(p3));
            this->display_4->setText(QString::number(p4));
            this->display_5->setText(QString::number(p5));
            this->display_6->setText(QString::number(p6));
        }
        else if(type=="54_band"){
            in_1->setValue(QRandomGenerator::global()->bounded(101));
            in_2->setValue(QRandomGenerator::global()->bounded(101));
            in_3->setValue(QRandomGenerator::global()->bounded(101));
            in_4->setValue(QRandomGenerator::global()->bounded(101));
            in_5->setValue(QRandomGenerator::global()->bounded(101));
            in_6->setValue(QRandomGenerator::global()->bounded(101));
            in_7->setValue(QRandomGenerator::global()->bounded(101));
            in_8->setValue(QRandomGenerator::global()->bounded(101));
            in_9->setValue(QRandomGenerator::global()->bounded(101));
            in_10->setValue(QRandomGenerator::global()->bounded(101));
            in_11->setValue(QRandomGenerator::global()->bounded(101));
            in_12->setValue(QRandomGenerator::global()->bounded(101));
            in_13->setValue(QRandomGenerator::global()->bounded(101));
            in_14->setValue(QRandomGenerator::global()->bounded(101));
            in_15->setValue(QRandomGenerator::global()->bounded(101));
            in_16->setValue(QRandomGenerator::global()->bounded(101));
            in_17->setValue(QRandomGenerator::global()->bounded(101));
            in_18->setValue(QRandomGenerator::global()->bounded(101));
            in_19->setValue(QRandomGenerator::global()->bounded(101));
            in_20->setValue(QRandomGenerator::global()->bounded(101));
            in_21->setValue(QRandomGenerator::global()->bounded(101));
            in_22->setValue(QRandomGenerator::global()->bounded(101));
            in_23->setValue(QRandomGenerator::global()->bounded(101));
            in_24->setValue(QRandomGenerator::global()->bounded(101));
            in_25->setValue(QRandomGenerator::global()->bounded(101));
            in_26->setValue(QRandomGenerator::global()->bounded(101));
            in_27->setValue(QRandomGenerator::global()->bounded(101));
            in_28->setValue(QRandomGenerator::global()->bounded(101));
            in_29->setValue(QRandomGenerator::global()->bounded(101));
            in_30->setValue(QRandomGenerator::global()->bounded(101));
            in_31->setValue(QRandomGenerator::global()->bounded(101));
            in_32->setValue(QRandomGenerator::global()->bounded(101));
            in_33->setValue(QRandomGenerator::global()->bounded(101));
            in_34->setValue(QRandomGenerator::global()->bounded(101));
            in_35->setValue(QRandomGenerator::global()->bounded(101));
            in_36->setValue(QRandomGenerator::global()->bounded(101));
            in_37->setValue(QRandomGenerator::global()->bounded(101));
            in_38->setValue(QRandomGenerator::global()->bounded(101));
            in_39->setValue(QRandomGenerator::global()->bounded(101));
            in_40->setValue(QRandomGenerator::global()->bounded(101));
            in_41->setValue(QRandomGenerator::global()->bounded(101));
            in_42->setValue(QRandomGenerator::global()->bounded(101));
            in_43->setValue(QRandomGenerator::global()->bounded(101));
            in_44->setValue(QRandomGenerator::global()->bounded(101));
            in_45->setValue(QRandomGenerator::global()->bounded(101));
            in_46->setValue(QRandomGenerator::global()->bounded(101));
            in_47->setValue(QRandomGenerator::global()->bounded(101));
            in_48->setValue(QRandomGenerator::global()->bounded(101));
            in_49->setValue(QRandomGenerator::global()->bounded(101));
            in_50->setValue(QRandomGenerator::global()->bounded(101));
            in_51->setValue(QRandomGenerator::global()->bounded(101));
            in_52->setValue(QRandomGenerator::global()->bounded(101));
            in_53->setValue(QRandomGenerator::global()->bounded(101));
            in_54->setValue(QRandomGenerator::global()->bounded(101));
        }
        else if(type=="poly_tuner"){
            x1=QRandomGenerator::global()->bounded(101);
            x2=QRandomGenerator::global()->bounded(101);
            x3++;//=QRandomGenerator::global()->bounded(101);
            if(x3>100)x3=0;
            x4=QRandomGenerator::global()->bounded(101);
            x5=QRandomGenerator::global()->bounded(101);
            x6=QRandomGenerator::global()->bounded(101);
            vu_1->setValue(x1);
            vu_2->setValue(x2);
            vu_3->setValue(x3);
            vu_4->setValue(x4);
            vu_5->setValue(x5);
            vu_6->setValue(x6);
            p1=x1/9;
            p2=x2/9;
            p3=x3/9;
            p4=x4/9;
            p5=x5/9;
            p6=x6/9;
            this->display_1->setText(note(p1));
            this->display_2->setText(note(p2));
            this->display_3->setText(note(p3));
            this->display_4->setText(note(p4));
            this->display_5->setText(note(p5));
            this->display_6->setText(note(p6));
        }
        else if(type=="mono_tuner"){
            x1=QRandomGenerator::global()->bounded(101);
            vu_1->setValue(x1);
            p1=x1/9;
            this->display_1->setText(note(p1));
        };
}
