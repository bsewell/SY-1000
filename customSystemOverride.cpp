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

#include "customSystemOverride.h"
//#include "MidiTable.h"
#include "SysxIO.h"
#include "Preferences.h"


#include <iostream>
#include <cstdlib>

customSystemOverride::customSystemOverride(QWidget *parent,
                                           QString hex0, QString hex1, QString hex2, QString hex3,
                                           QString index, QString type, int rowSpan, int columnSpan,
                                           QString hex_0, QString hex_1, QString hex_2, QString hex_3, QString value)
    : QWidget(parent)
{
    Preferences *preferences = Preferences::Instance();
    bool ok;
    const double ratio = preferences->getPreferences("Window", "Scale", "ratio").toDouble(&ok);
    const double fratio = preferences->getPreferences("Window", "Font", "ratio").toDouble(&ok);
    QFont Sfont( "Arial", 8*fratio, QFont::Normal);

    this->label = new customControlLabel(this);
    this->label2 = new customControlLabel(this);
    this->hex0 = hex0;
    this->hex1 = hex1;
    this->hex2 = hex2;
    this->hex3 = hex3;
    this->index = index;
    this->type = type;
    this->rowSpan =rowSpan;
    this->columnSpan = columnSpan;
    this->hex_0 = hex_0;
    this->hex_1 = hex_1;
    this->hex_2 = hex_2;
    this->hex_3 = hex_3;
    this->value = value;
    _lastValue = -1;

    //if(hex0.toInt(&ok, 16)<16){this->label->setText("OVERRIDE BY");  // if hex0 address is less that "10" then is not in the patch address range.
    //    this->label2->setText("SYSTEM SETTING");};
    this->label->setFont(Sfont);
    this->label2->setFont(Sfont);

    this->label->setAlignment(Qt::AlignTop);
    this->label2->setAlignment(Qt::AlignTop);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    //mainLayout->setMargin(20*ratio);
    mainLayout->setSpacing(20*ratio);
    mainLayout->addStretch(0);
    mainLayout->addWidget(this->label, 0, Qt::AlignTop);
    mainLayout->addWidget(this->label2, 0, Qt::AlignTop);
    mainLayout->setSpacing(20*ratio);

    this->setLayout(mainLayout);

    QObject::connect(this->parent(), SIGNAL( dialogUpdateSignal() ),
                     this, SLOT( dialogUpdateSignal() ));

    QObject::connect(this->parent(), SIGNAL( updateSignal() ),
                     this, SLOT( dialogUpdateSignal() ));

    QObject::connect(this->parent(), SIGNAL( timerUpdateSignal() ),
                     this, SLOT( dialogUpdateSignal() ));

    QObject::connect(this, SIGNAL( command_update() ), this->parent(), SIGNAL( dialogUpdateSignal() ));

    disable = false;

    dialogUpdateSignal();
}

void customSystemOverride::paintEvent(QPaintEvent *)
{
    if(!this->type.contains("command")){
        Preferences *preferences = Preferences::Instance();
        bool ok;
        const double ratio = preferences->getPreferences("Window", "Scale", "ratio").toDouble(&ok);

        /*DRAWS BLACK BACKGROUND FOR MASKING */
        QPixmap image(":/images/override.png");

        QRectF target(0.0, 0.0, (this->width()*(this->columnSpan))*ratio, (this->height()*(this->rowSpan))*ratio);
        QRectF source(0.0, 0.0, this->width(), this->height());

        QPainter painter(this);
        painter.drawPixmap(target, image, source);
    };
}

void customSystemOverride::dialogUpdateSignal()
{
    if(disable==false){

        SysxIO *sysxIO = SysxIO::Instance();
        bool ok;
        int val = sysxIO->getSourceValue(this->hex0, this->hex1, this->hex2, this->hex3);
        if(val != _lastValue){
            _lastValue = val;
        int indexValue = this->index.toInt(&ok, 16);

            if(type.contains("command")){
                if(!hex_0.contains("void")){
                    if(type.contains("equal"))    {if(val != indexValue){ sysxIO->setFileSource(hex_0, hex_1, hex_2, hex_3, value, false); cmd_update(); }; };
                    if(type.contains("not_equal")){if(val == indexValue){ sysxIO->setFileSource(hex_0, hex_1, hex_2, hex_3, value, false); cmd_update(); }; };
                    if(type.contains("more_than")){if(val > indexValue) { sysxIO->setFileSource(hex_0, hex_1, hex_2, hex_3, value, false); cmd_update(); }; };
                    if(type.contains("less_than")){if(val < indexValue) { sysxIO->setFileSource(hex_0, hex_1, hex_2, hex_3, value, false); cmd_update(); }; };
                };
            }else
            {
                if(type.contains("equal")){if(val != indexValue){this->hide();} else {this->show();}; };
                if(type.contains("not_equal")){if(val == indexValue){this->hide();} else {this->show();}; };
                if(type.contains("more_than")){if(val > indexValue){this->hide();} else {this->show();}; };
                if(type.contains("less_than")){if(val < indexValue){this->hide();} else {this->show();}; };
            };
         };
    };
}

void customSystemOverride::cmd_update()
{
    SysxIO *sysxIO = SysxIO::Instance();
    int val = sysxIO->getSourceValue(this->hex0, this->hex1, this->hex2, this->hex3);
   if(val != previous){
    disable = true;
    //SysxIO *sysxIO = SysxIO::Instance();
    sysxIO->setFileSource("Stucture", sysxIO->getFileSource());
    sysxIO->relayUpdateSignal();
    disable = false;
    previous = val;
   };
}
