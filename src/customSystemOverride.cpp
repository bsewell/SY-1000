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
#include <utility>

customSystemOverride::customSystemOverride(QWidget *parent,
                                           QString hex0, QString hex1, QString hex2, QString hex3,
                                           QString index, QString type, int row, int column, int rowSpan, int columnSpan,
                                           QString hex_0, QString hex_1, QString hex_2, QString hex_3, QString value)
    : QWidget(parent)
{
    Preferences *preferences = Preferences::Instance();
    bool ok;
    const double ratio = preferences->getPreferences("Window", "Scale", "ratio").toDouble(&ok);
    const double fratio = preferences->getPreferences("Window", "Font", "ratio").toDouble(&ok);
    QFont Sfont( "Roboto Condensed", 8*fratio, QFont::Normal);

    this->label = new customControlLabel(this);
    this->label2 = new customControlLabel(this);
    this->hex0 = hex0;
    this->hex1 = hex1;
    this->hex2 = hex2;
    this->hex3 = hex3;
    this->index = index;
    this->type = type;
    this->row = row;
    this->column = column;
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
    this->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    this->hide();

    dialogUpdateSignal();
}

void customSystemOverride::paintEvent(QPaintEvent *)
{
    // Override widgets now only manage visibility of covered controls.
    // They should not paint a mask over the UI.
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
                bool shouldMask = false;
                if(type.contains("equal")) { shouldMask = (val == indexValue); };
                if(type.contains("not_equal")) { shouldMask = (val != indexValue); };
                if(type.contains("more_than")) { shouldMask = (val <= indexValue); };
                if(type.contains("less_than")) { shouldMask = (val >= indexValue); };
                updateMaskedWidgets(shouldMask);
                this->hide();
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

bool customSystemOverride::overlapsCell(int otherRow, int otherColumn, int otherRowSpan, int otherColumnSpan) const
{
    const int rowEnd = this->row + this->rowSpan;
    const int columnEnd = this->column + this->columnSpan;
    const int otherRowEnd = otherRow + otherRowSpan;
    const int otherColumnEnd = otherColumn + otherColumnSpan;

    return this->row < otherRowEnd &&
           rowEnd > otherRow &&
           this->column < otherColumnEnd &&
           columnEnd > otherColumn;
}

void customSystemOverride::updateMaskedWidgets(bool maskActive)
{
    QGridLayout *grid = qobject_cast<QGridLayout*>(this->parentWidget() ? this->parentWidget()->layout() : nullptr);
    if(grid == nullptr)
    {
        currentlyMasking = false;
        return;
    }

    if(maskedWidgets.isEmpty())
    {
        for(int index = 0; index < grid->count(); ++index)
        {
            int itemRow = 0;
            int itemColumn = 0;
            int itemRowSpan = 0;
            int itemColumnSpan = 0;
            grid->getItemPosition(index, &itemRow, &itemColumn, &itemRowSpan, &itemColumnSpan);
            if(!overlapsCell(itemRow, itemColumn, itemRowSpan, itemColumnSpan))
            {
                continue;
            }

            QWidget *widget = grid->itemAt(index)->widget();
            if(widget == nullptr || widget == this)
            {
                continue;
            }
            maskedWidgets.append(widget);
        }
    }

    if(maskActive == currentlyMasking)
    {
        return;
    }

    for(const QPointer<QWidget> &widget : std::as_const(maskedWidgets))
    {
        if(widget.isNull())
        {
            continue;
        }

        int maskCount = widget->property("systemOverrideMaskCount").toInt();
        maskCount += maskActive ? 1 : -1;
        if(maskCount < 0)
        {
            maskCount = 0;
        }
        widget->setProperty("systemOverrideMaskCount", maskCount);
        widget->setVisible(maskCount == 0);
    }

    currentlyMasking = maskActive;
}
