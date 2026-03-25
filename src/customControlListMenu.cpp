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

#include "customControlListMenu.h"
#include "MidiTable.h"
#include "SysxIO.h"
#include "Preferences.h"
#include "globalVariables.h"

customControlListMenu::customControlListMenu(QWidget *parent,
                                             QString hex0, QString hex1, QString hex2, QString hex3, QString direction)
    : QWidget(parent)
{
    Preferences *preferences = Preferences::Instance();
    bool ok;
    const double ratio = preferences->getPreferences("Window", "Scale", "ratio").toDouble(&ok);
    const double fratio = preferences->getPreferences("Window", "Font", "ratio").toDouble(&ok);
    const int verticalGap = qRound(5 * ratio);
    const int controlMargin = qRound(4 * ratio);

    this->direction = direction;
    this->label = new customControlLabel(this);
    this->controlListComboBox = new customComboBox(this);
    this->controlListComboBox->view()->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    byteSize = 2;
    if(direction.contains("4Byte")){ byteSize = 4; };

    if(direction.contains("Tables") || direction.contains("large"))
    {
        this->controlListComboBox->setObjectName("largecombo");
        applyComboFont(true, fratio);
    }
    else
    {
        this->controlListComboBox->setObjectName("smallcombo");
        applyComboFont(false, fratio);
    };
    this->hex0 = hex0;
    this->hex1 = hex1;
    this->hex2 = hex2;
    this->hex3 = hex3;
    if (direction.contains("Tables") || hex0.contains("Tables")) {this->area = "Tables"; }
    else if(direction.contains("System") || hex0.toInt(&ok, 16)<10) {this->area = "System"; }
    else {this->area = "Structure"; };

    MidiTable *midiTable = MidiTable::Instance();
    Midi items;

    items = midiTable->getMidiMap(hex0, hex1, hex2, hex3);

    QString labeltxt = items.customdesc;

    this->label->setUpperCase(true);
    this->label->setFontRole("setting");
    this->label->setText(labeltxt);

    setComboBox();

    if(direction.contains("left"))
    {
        this->label->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);

        QHBoxLayout *mainLayout = new QHBoxLayout;
        mainLayout->setContentsMargins(controlMargin, verticalGap, controlMargin, verticalGap);
        mainLayout->setSpacing(qRound(12 * ratio));
        mainLayout->addWidget(this->label, 0, Qt::AlignVCenter | Qt::AlignLeft);
        mainLayout->addWidget(this->controlListComboBox, 0, Qt::AlignVCenter | Qt::AlignLeft);
        mainLayout->addStretch(1);
        this->setLayout(mainLayout);
    }
    else if(direction.contains("right"))
    {
        this->label->setAlignment(Qt::AlignVCenter | Qt::AlignRight);

        QHBoxLayout *mainLayout = new QHBoxLayout;
        mainLayout->setContentsMargins(controlMargin, verticalGap, controlMargin, verticalGap);
        mainLayout->setSpacing(qRound(12 * ratio));
        mainLayout->addStretch(1);
        mainLayout->addWidget(this->controlListComboBox, 0, Qt::AlignVCenter | Qt::AlignRight);
        mainLayout->addWidget(this->label, 0, Qt::AlignVCenter | Qt::AlignRight);
        this->setLayout(mainLayout);
    }
    else if(direction.contains("top"))
    {
        this->label->setAlignment(Qt::AlignCenter);

        QVBoxLayout *mainLayout = new QVBoxLayout;
        mainLayout->setContentsMargins(controlMargin, verticalGap, controlMargin, verticalGap);
        mainLayout->setSpacing(verticalGap);
        mainLayout->addWidget(this->controlListComboBox, 0, Qt::AlignCenter);
        mainLayout->addWidget(this->label, 0, Qt::AlignCenter);
        this->setLayout(mainLayout);
    }
    else if(direction.contains("bottom"))
    {
        this->label->setAlignment(Qt::AlignLeft);

        QVBoxLayout *mainLayout = new QVBoxLayout;
        mainLayout->setContentsMargins(controlMargin, verticalGap, controlMargin, verticalGap);
        if(direction.contains("icon_seq")){mainLayout->setSpacing(qMax(1, verticalGap / 2));}else{mainLayout->setSpacing(verticalGap);};
        mainLayout->addStretch(0);
        mainLayout->addWidget(this->label, 0, Qt::AlignLeft);
        mainLayout->addWidget(this->controlListComboBox, 0, Qt::AlignLeft);
        this->setLayout(mainLayout);
    }
    else
    {
        this->label->setAlignment(Qt::AlignCenter);

        QVBoxLayout *mainLayout = new QVBoxLayout;
        mainLayout->setContentsMargins(controlMargin, verticalGap, controlMargin, verticalGap);
        if(direction.contains("icon_seq")){mainLayout->setSpacing(qMax(1, verticalGap / 2));}else{mainLayout->setSpacing(verticalGap);};
        mainLayout->addStretch(0);
        mainLayout->addWidget(this->label, 0, Qt::AlignCenter);
        mainLayout->addWidget(this->controlListComboBox, 0, Qt::AlignCenter);
        this->setLayout(mainLayout);
    };

    if(midiTable->isData(this->area, hex1, hex2, hex3))
    {
        this->setFixedHeight((12 + 23 + 8)*ratio);
    }
    else if(direction.contains("Tables") || direction.contains("large"))
    {
        this->setFixedHeight((12 + 26 + 8)*ratio);
    }
    else
    {
        this->setFixedHeight((12 + 18 + 8)*ratio);
    };

    hover = false;
    timer = new QTimer(this);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(timed()) );

    setMouseTracking(true);

    QObject::connect(this->parent(), SIGNAL( dialogUpdateSignal() ), this, SLOT( dialogUpdateSignal() ));
    QObject::connect(this->parent(), SIGNAL( updateSignal() ), this, SLOT( changeComboBox() ));

    QObject::connect(this, SIGNAL( updateSignal() ), this->parent(), SIGNAL( updateSignal() ));

    QObject::connect(this->controlListComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(valueChanged(int)));

    QObject::connect(this->controlListComboBox, SIGNAL(currentIndexChanged(int)), this, SIGNAL(currentIndexChanged(int)));

    QObject::connect(this->controlListComboBox, SIGNAL(highlighted(int)), this, SLOT(highlight()));

    //QObject::connect(this->controlListComboBox, SIGNAL(highlighted(int)), this, SIGNAL(currentIndexChanged(int)));

    if(this->direction.contains("Tables"))
    {
        QObject::connect(this->controlListComboBox, SIGNAL(activated(int)), this->parent(), SLOT(select_patch()));
    };
}

void customControlListMenu::applyComboFont(bool largeVariant, double fratio)
{
    int baseFont = largeVariant ? 13 : 8;
#ifdef Q_OS_MAC
    baseFont = largeVariant ? 14 : 9;
#endif
    // Keep the selected dropdown text slightly smaller than the label stack so
    // effect detail pages read as one control instead of a heavy menu block.
    baseFont = qMax(6, baseFont - 2);
    QFont comboFont("Roboto Condensed", baseFont * fratio, QFont::Normal);
    this->controlListComboBox->setFont(comboFont);
    if(this->controlListComboBox->view())
    {
        this->controlListComboBox->view()->setFont(comboFont);
    }
}

void customControlListMenu::paintEvent(QPaintEvent *)
{
    // Suppress the legacy red hover wash behind selectors. It adds no state
    // information and competes with the actual control styling.
}

void customControlListMenu::mouseMoveEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    //if(this->underMouse() ){this->setFocus(); hover=true;  repaint(); this->timer->start(500);};
    highlight();
    //std::string st = QString("combobox mouse event = "  ).toStdString();
    //std::cerr << st << std::endl;
}

void customControlListMenu::setComboBox()
{
    Preferences *preferences = Preferences::Instance();
    bool ok;
    const double ratio = preferences->getPreferences("Window", "Scale", "ratio").toDouble(&ok);
    const double fratio = preferences->getPreferences("Window", "Font", "ratio").toDouble(&ok);

    QString longestItem = "";
    int itemcount = 0;
    QList<QString> list;
    MidiTable *midiTable = MidiTable::Instance();

    Midi items;
    items = midiTable->getMidiMap(this->hex0, this->hex1, this->hex2, this->hex3);
    controlListComboBox->clear();
    int itemSize = items.level.size();
    int itemTotal = 0;

    if(direction.contains("TARGET"))
    {
        for(int x=0; x<12; x++)
        {
            QString addr = QString::number(x, 16).toUpper();
            if(addr.size()<2){addr.prepend("0");};
            Midi items = midiTable->getMidiMap("Tables", "00", "00", "00", addr);
            int itemSize = items.level.size();
            itemTotal = itemTotal+itemSize;
            for(itemcount=0;itemcount<itemSize;itemcount++ )
            {
                QString item = items.level.at(itemcount).name;
                QString Litem = item.mid(0,2)+item.mid(3,2);
                this->comboDataList.append(Litem);  // read the string of address numbers and use as an index lookup list.
                list.append(" "+item.remove(0,8));
            };
        };
        this->controlListComboBox->addItems(list);
        this->controlListComboBox->setFixedWidth(350*ratio);
        this->controlListComboBox->setFixedHeight(16*ratio);
        this->controlListComboBox->setEditable(false);
        this->controlListComboBox->setFrame(false);
        this->controlListComboBox->setMaxVisibleItems(50);
    }
    else
        if(direction.contains("data"))
        {
            int size = midiTable->getMidiMap(hex0, hex1, hex2, hex3).level.last().value.toInt(&ok, 16)+1;
            for(int x=0; x<size; x++)
            {
                QString addr = QString::number(x, 16).toUpper();
                if(addr.size()<2){addr.prepend("0");};
                Midi items = midiTable->getMidiMap(hex0, hex1, hex2, hex3, addr);
                int itemSize = items.level.size();
                itemTotal = itemTotal+itemSize;
                for(itemcount=0;itemcount<itemSize;itemcount++ )
                {
                    QString item = items.level.at(itemcount).name;
                    QString Litem = addr+items.level.at(itemcount).value;
                    this->comboDataList.append(Litem);  // read the string of address numbers and use as an index lookup list.
                    list.append(" "+item);
                };
            };
            this->controlListComboBox->addItems(list);
        }
        else
        {
            for(itemcount=0;itemcount<itemSize;itemcount++ )
            {
                QString item;
                QString name = items.level.at(itemcount).name;
                QString desc = items.level.at(itemcount).desc;
                QString customdesc = items.level.at(itemcount).customdesc;
                QString addressData = items.level.at(itemcount).value;
                if(!desc.isEmpty()){item = desc;}
                else if(!customdesc.isEmpty()){item = customdesc;}
                else{item = name;};

                if(!direction.contains("icon"))   // if a regular combobox.
                {
                    list.append(item);
                    this->comboDataList.append(addressData);
                    if(longestItem.size() < item.size()) longestItem = item;
                    this->controlListComboBox->setFixedHeight(16*ratio);
                }
                else
                {
                    QString image = (":/images/"+name+".png");
                    QIcon icon(image);
                    int size = 16;
                    if(direction.contains("40")){size = 38;};
                    this->controlListComboBox->setIconSize(QSize(size*ratio, size*ratio));
                    this->controlListComboBox->addItem(icon, desc);
                    this->comboDataList.append(addressData);
                    if(longestItem.size() < item.size()) longestItem = item;
                    if(direction.contains("40")){longestItem.append("____");};
                };
            };
            this->controlListComboBox->addItems(list);
            // Size the collapsed control from the same reduced combo font used
            // for the visible selector text and popup rows.
            int popupWidth = QFontMetrics(this->controlListComboBox->font()).horizontalAdvance(longestItem) + (20 * fratio);
            if(popupWidth<60){popupWidth=60;};
            this->controlListComboBox->setMaxVisibleItems(itemcount+1);
            if(this->direction.contains("Tables") || this->direction.contains("large"))
            {
                this->controlListComboBox->setFixedHeight(26*ratio);
                this->controlListComboBox->setFixedWidth(qMax(int(popupWidth * 1.35), qRound(190 * ratio)));
            }
            else if(this->direction.contains("seq"))
            {
                this->controlListComboBox->setFixedHeight(18*ratio);
                this->controlListComboBox->setFixedWidth(38*ratio);
            }
            else
            {

#ifdef Q_OS_MAC
                this->controlListComboBox->setFixedWidth(qMax(popupWidth, qRound(88 * ratio)));
#else
                this->controlListComboBox->setFixedWidth(popupWidth);
#endif
                this->controlListComboBox->setFixedHeight(18*ratio);
            };
        };
    this->controlListComboBox->setEditable(false);
    this->controlListComboBox->setFrame(false);
    this->max = itemcount-1;
    this->min = 0;
}

void customControlListMenu::valueChanged(int index)
{
    if(direction.contains("TARGET") || direction.contains("data"))
    {
        if(index>-1 && index!=_lastValue)
        {
            SysxIO *sysxIO = SysxIO::Instance();
            if(byteSize==4)
            {
                QList<QString> Dword;
                Dword.append("0"+comboDataList.at(index).mid(0,1));
                Dword.append("0"+comboDataList.at(index).mid(1,1));
                Dword.append("0"+comboDataList.at(index).mid(2,1));
                Dword.append("0"+comboDataList.at(index).mid(3,1));
                sysxIO->setFileSource(hex0, hex1, hex2, hex3, Dword);
            }
            else
            {
                QString valueHex1 = comboDataList.at(index).mid(0,2);
                QString valueHex2 = comboDataList.at(index).mid(2,2);
                sysxIO->setFileSource(hex0, hex1, hex2, hex3, valueHex1, valueHex2);
            }
            _lastValue = index;
            emit updateSignal();
        };

    }
    else
    {
        if(!this->direction.contains("Tables") && !this->hex0.contains("Tables"))
        {
            QList<QString> hexData;
            QString valueHex = "00";
            int listSize = this->comboDataList.size()+1;
            if(listSize>index && index>-1){ valueHex = this->comboDataList.at(index); };
            if(valueHex.length() < 2) valueHex.prepend("0");

            SysxIO *sysxIO = SysxIO::Instance();
            MidiTable *midiTable = MidiTable::Instance();
            bool ok;
            if(listSize>128 || midiTable->isData(this->hex0, this->hex1, this->hex2, this->hex3))
            {
                int maxRange = 128;
                int value = valueHex.toInt(&ok, 16);
                int dif = value/maxRange;
                QString valueHex1 = QString::number(dif, 16).toUpper();
                if(valueHex1.length() < 2) valueHex1.prepend("0");
                QString valueHex2 = QString::number(value - (dif * maxRange), 16).toUpper();
                if(valueHex2.length() < 2) valueHex2.prepend("0");
                hexData.append(valueHex1);
                hexData.append(valueHex2);
                sysxIO->setFileSource(this->hex0, this->hex1, this->hex2, this->hex3, hexData);
            }
            else
            {
                sysxIO->setFileSource(this->hex0, this->hex1, this->hex2, this->hex3, valueHex);
            };
            _lastValue = index;
            emit updateSignal();
        };
    };
}

void customControlListMenu::dialogUpdateSignal()
{
    if(this->direction.contains("TARGET") || this->direction.contains("data"))
    {
        SysxIO *sysxIO = SysxIO::Instance();
        int value=0;
        bool ok;
        if(byteSize==4)
        {
            QList<QString> list = sysxIO->getFileSource(this->hex0, this->hex1, this->hex2, "void").mid((hex3.toInt(&ok,16)+sysxDataOffset), 8);
            QString Dword;
            Dword.append(list.at(0)).append(list.at(1)).append(list.at(2)).append(list.at(3));
            Dword.remove(6,1).remove(4,1).remove(2,1).remove(0,1);
            value = Dword.toInt(&ok, 16);
            //std::string st = QString("Dword="+Dword).toStdString();
            //std::cerr << st << std::endl;
        }
        else
        {
            value = sysxIO->getSourceValue(this->hex0, this->hex1, this->hex2, this->hex3);
        };
        QString valueHex = QString::number(value, 16).toUpper();
        if(valueHex.length() < 2) valueHex.prepend("0");
        int maxRange = 128;
        value = valueHex.toInt(&ok, 16);
        int dif = value/maxRange;
        QString valueHex1 = QString::number(dif, 16).toUpper();
        if(valueHex1.length() < 2) valueHex1.prepend("0");
        QString valueHex2 = QString::number(value - (dif * maxRange), 16).toUpper();
        if(valueHex2.length() < 2) valueHex2.prepend("0");
        int index = comboDataList.indexOf(valueHex1+valueHex2, 0);
        if(index != _lastValue){
            this->controlListComboBox->setCurrentIndex(index);
            this->valueChanged(index);
            _lastValue = index;
        };
    }
    else if(!this->direction.contains("Tables") && !this->hex0.contains("Tables"))
    {
        SysxIO *sysxIO = SysxIO::Instance();
        //int index = sysxIO->getSourceValue(this->hex0, this->hex1, this->hex2, this->hex3);
        int value = sysxIO->getSourceValue(this->hex0, this->hex1, this->hex2, this->hex3);
        QString valueHex = QString::number(value, 16).toUpper();
        if(valueHex.length() < 2) valueHex.prepend("0");
        int index = this->comboDataList.indexOf(valueHex);  // look up the hex data value and point to the combobox index the data is found at.
        if(index != _lastValue)
        {
            this->controlListComboBox->setCurrentIndex(index);
            this->valueChanged(index);
            _lastValue = index;
        };
    };
}

void customControlListMenu::set_index(int index)
{
    this->controlListComboBox->setCurrentIndex(index);
}

void customControlListMenu::changeComboBox()
{

    if(direction.contains("pu_sel"))
    {
        QObject::disconnect(this->controlListComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(valueChanged(int)));

        QObject::disconnect(this->controlListComboBox, SIGNAL(currentIndexChanged(int)), this, SIGNAL(currentIndexChanged(int)));

        int itemcount;
        MidiTable *midiTable = MidiTable::Instance();
        controlListComboBox->clear();

        Midi items;
        items = midiTable->getMidiMap(this->hex0, this->hex1, this->hex2, this->hex3);

        int itemSize = items.level.size();
        int pu_type=0;


        SysxIO *sysxIO = SysxIO::Instance();
        bool ok;
        QString addr = QString::number(hex3.toInt(&ok, 16)-1, 16);
        if(addr.size()<2){addr.prepend("0");};
        pu_type = sysxIO->getSourceValue(this->hex0, this->hex1, this->hex2, addr);
        if(pu_type==8)
        {
            itemSize=6;
        }
        else if(pu_type<2)
        {
            itemSize=5;
        }
        else if(pu_type>1 && pu_type!=8)
        {
            itemSize=3;
        };


        for(itemcount=0;itemcount<itemSize;itemcount++ )
        {
            QString item;
            QString desc = items.level.at(itemcount).desc;
            QString customdesc = items.level.at(itemcount).customdesc;
            item = desc;
            if(!desc.isEmpty())
            {
                item = desc;
            }
            else if(!customdesc.isEmpty())
            {
                item = customdesc;
            };
            if(pu_type>1 && pu_type!=8)
            {
                item = customdesc;
            };

            this->controlListComboBox->addItem(item);
        };
        this->max = itemcount;
        int index = sysxIO->getSourceValue(this->hex0, this->hex1, this->hex2, this->hex3);
        if(index>=itemSize){index=itemSize-1; valueChanged(index);};
        this->controlListComboBox->setCurrentIndex(index);

        QObject::connect(this->controlListComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(valueChanged(int)));

        QObject::connect(this->controlListComboBox, SIGNAL(currentIndexChanged(int)), this, SIGNAL(currentIndexChanged(int)));

    };

    if(direction.contains("inst"))
    {
        QString hex1 = "00";
        Preferences *preferences = Preferences::Instance();
        if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
        {
            hex1 = "02";
        };

        SysxIO *sysxIO = SysxIO::Instance();

        int input_type = sysxIO->getSourceValue("10", hex1, "12", "00");

        int index = sysxIO->getSourceValue(this->hex0, this->hex1, this->hex2, this->hex3);
        if(input_type > 0)
        {
            index = 0;
            this->controlListComboBox->setDisabled(true);
        }
        else
        {
            this->controlListComboBox->setDisabled(false);
        };

        //valueChanged(index);
        this->controlListComboBox->setCurrentIndex(index);
        _lastValue = index;
        //std::string st = QString("input_type = "+ QString::number(input_type, 10)).toStdString();
        //std::cerr << st << std::endl;

    };
}

void customControlListMenu::keyPressEvent(QKeyEvent *event)
{
    int value = 0;

    switch(event->key())
    {
    case Qt::Key_Up: value = 1;break;
    case Qt::Key_Down: value = -1;break;
    case Qt::Key_Plus: value = 1;break;
    case Qt::Key_Equal: value = 1;break;
    case Qt::Key_Minus: value = -1;break;
    case Qt::Key_Q: value = 1;break;
    case Qt::Key_A: value = -1;break;
    case Qt::Key_P: value = 1;break;
    case Qt::Key_L: value = -1;break;

    case Qt::Key_PageUp: value = 10;break;
    case Qt::Key_PageDown: value = -10;break;
    case Qt::Key_W: value = 10;break;
    case Qt::Key_S: value = -10;break;
    case Qt::Key_O: value = 10;break;
    case Qt::Key_K: value = -10;break;

    case Qt::Key_Right: value = max;break;
    case Qt::Key_Left: value = min;break;
    case Qt::Key_E: value = max;break;
    case Qt::Key_D: value = min-_lastValue;break;
    case Qt::Key_I: value = max;break;
    case Qt::Key_J: value = min-_lastValue;break;
    };

    if(value!=0)
    {
        int _newValue = _lastValue + value;

        if(_newValue <= min)
        {
            _newValue = min;
        }
        else if(_newValue >= max)
        {
            _newValue = max;
        };

        set_index(_newValue);
        valueChanged(_newValue);

        //QString val = QString::number(_newValue, 10);
        //std::string st = QString("combobox key press event = " + val + " "+QString::number(value,10)).toStdString();
        //std::cerr << st << std::endl;
        highlight();
    };
}

void customControlListMenu::timed()
{
    if(this->underMouse() || this->hasFocus())
    {
        this->timer->stop();
        this->timer->start(500);
    }
    else
    {
        this->timer->stop();
        this->hover=false;
        this->repaint();
        clearFocus();
    };
}

void customControlListMenu::highlight()
{
    this->setFocus();
    hover=true;
    repaint();
    this->timer->stop();
    this->timer->start(500);
}
