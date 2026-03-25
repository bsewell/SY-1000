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

#include "customTargetListMenu.h"
#include "MidiTable.h"
#include "SysxIO.h"
#include "Preferences.h"
#include <QComboBox>

customTargetListMenu::customTargetListMenu(QWidget *parent,
                                           QString hex0, QString hex1, QString hex2, QString hex3, QString hex3_1,
                                           QString hex3_2, QString hex3_3, QString direction)
    : QWidget(parent)
{
    Preferences *preferences = Preferences::Instance();
    bool ok;
    const double ratio = preferences->getPreferences("Window", "Scale", "ratio").toDouble(&ok);
    const double fratio = preferences->getPreferences("Window", "Font", "ratio").toDouble(&ok);
    QFont Sfont( "Roboto Condensed", 9*fratio, QFont::Normal);

    this->controlListComboBox = new QComboBox(this);
    this->controlListComboBox->setObjectName("smallcombo");
    this->controlListComboBox->setFont(Sfont);
    this->controlListComboBox->view()->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    this->hex0 = hex0;
    this->hex1 = hex1;
    this->hex2 = hex2;
    this->hex3 = hex3;
    this->hex3_1 = hex3_1;
    this->hex3_2 = hex3_2;
    this->hex3_3 = hex3_3;
    this->area = direction;

    setComboBox();

        QVBoxLayout *mainLayout = new QVBoxLayout;
        //mainLayout->setMargin(10);
        mainLayout->setSpacing(20);
        mainLayout->addStretch(0);
        mainLayout->addWidget(this->controlListComboBox, 0, Qt::AlignCenter);

        this->setLayout(mainLayout);
        this->setFixedHeight(65*ratio);

    QObject::connect(this->controlListComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(valueChanged(int)));

    //QObject::connect(this->controlListComboBox, SIGNAL(highlighted(int)), this, SLOT(valueChanged(int)));

    //QObject::connect(this->controlListComboBox, SIGNAL(currentIndexChanged(int)), this, SIGNAL(currentIndexChanged(int)));

    QObject::connect(this, SIGNAL( updateTarget(QString, QString, QString, QString) ),
                     this->parent(), SIGNAL( updateTarget(QString, QString, QString, QString) ));
}

void customTargetListMenu::setComboBox()
{
    Preferences *preferences = Preferences::Instance();
    bool ok;
    const double ratio = preferences->getPreferences("Window", "Scale", "ratio").toDouble(&ok);

    QString mode_hex = "00";
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){mode_hex = "02";};

    MidiTable *midiTable = MidiTable::Instance();
    Midi items;
    Midi item_0;
    Midi item_1;
    Midi item_2;
    Midi item_3;
    Midi item_4;
    Midi item_5;
    Midi item_6;
    Midi item_7;
    item_0 = midiTable->getMidiMap("Tables", "00", "00", mode_hex, "00");
    item_1 = midiTable->getMidiMap("Tables", "00", "00", mode_hex, "01");
    item_2 = midiTable->getMidiMap("Tables", "00", "00", mode_hex, "02");
    item_3 = midiTable->getMidiMap("Tables", "00", "00", mode_hex, "03");
    item_4 = midiTable->getMidiMap("Tables", "00", "00", mode_hex, "04");
    item_5 = midiTable->getMidiMap("Tables", "00", "00", mode_hex, "05");
    item_6 = midiTable->getMidiMap("Tables", "00", "00", mode_hex, "06");
    item_7 = midiTable->getMidiMap("Tables", "00", "00", mode_hex, "07");
    items = item_0;
    QList<QString> list;

    QString longestItem = "";
    int itemcount;
    int itemSize = items.level.size();
    int itemTotal = 0;
    for(itemcount=0;itemcount<itemSize;itemcount++ )
    {
        QString item = items.level.at(itemcount).name;
        if(longestItem.size() < item.size()) longestItem = item;
        list.append(item);
    };

    itemTotal = itemTotal + itemcount;
    items = item_1;
    itemSize = items.level.size();
    for(itemcount=0;itemcount<itemSize;itemcount++ )
    {
        QString item = items.level.at(itemcount).name;
        if(longestItem.size() < item.size()) longestItem = item;
        list.append(item);
    };

    itemTotal = itemTotal + itemcount;
    items = item_2;
    itemSize = items.level.size();
    for(itemcount=0;itemcount<itemSize;itemcount++ )
    {
        QString item = items.level.at(itemcount).name;
        if(longestItem.size() < item.size()) longestItem = item;
        list.append(item);
    };

    itemTotal = itemTotal + itemcount;
    items = item_3;
    itemSize = items.level.size();
    for(itemcount=0;itemcount<itemSize;itemcount++ )
    {
        QString item = items.level.at(itemcount).name;
        if(longestItem.size() < item.size()) longestItem = item;
        list.append(item);
    };

    itemTotal = itemTotal + itemcount;
    items = item_4;
    itemSize = items.level.size();
    for(itemcount=0;itemcount<itemSize;itemcount++ )
    {
        QString item = items.level.at(itemcount).name;
        if(longestItem.size() < item.size()) longestItem = item;
        list.append(item);
    };

    itemTotal = itemTotal + itemcount;
    items = item_5;
    itemSize = items.level.size();
    for(itemcount=0;itemcount<itemSize;itemcount++ )
    {
        QString item = items.level.at(itemcount).name;
        if(longestItem.size() < item.size()) longestItem = item;
        list.append(item);
    };

    itemTotal = itemTotal + itemcount;
    items = item_6;
    itemSize = items.level.size();
    for(itemcount=0;itemcount<itemSize;itemcount++ )
    {
        QString item = items.level.at(itemcount).name;
        if(longestItem.size() < item.size()) longestItem = item;
        list.append(item);
    };

    itemTotal = itemTotal + itemcount;
    items = item_7;
    itemSize = items.level.size();
    for(itemcount=0;itemcount<itemSize;itemcount++ )
    {
        QString item = items.level.at(itemcount).name;
        if(longestItem.size() < item.size()) longestItem = item;
        list.append(item);
    };

    this->controlListComboBox->addItems(list);
    itemTotal = itemTotal + itemcount;
    this->controlListComboBox->setFixedWidth(400*ratio);
    this->controlListComboBox->setFixedHeight(20*ratio);
    this->controlListComboBox->setEditable(false);
    this->controlListComboBox->setFrame(false);
    this->controlListComboBox->setMaxVisibleItems(itemTotal);

    min = 0;
    max = itemTotal-1;
    x = this->width();
    y = this->height();
    hover = false;
    timer = new QTimer(this);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(timed()) );

    setMouseTracking(true);
}

void customTargetListMenu::paintEvent(QPaintEvent *)
{
    /*DRAWS RED BACKGROUND FOR DeBugGING PURPOSE */
    if(hover==true){
    QPixmap image(":/images/dragbar.png");

        QRectF target(0.0, 0.0, this->width(), this->height());
        QRectF source(0.0, 0.0, 600, 600);

        QPainter painter(this);
        painter.drawPixmap(target, image, source);
    };
}

void customTargetListMenu::mouseMoveEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    //if(this->underMouse() ){this->setFocus(); hover=true;  repaint(); this->timer->start(500);};
    this->setFocus(); hover=true;  repaint(); this->timer->start(500);

    //std::string st = QString("target list mouse event = "  ).toStdString();
    //std::cerr << st << std::endl;
}

void customTargetListMenu::valueChanged(int index)
{
    //QObject::disconnect(this->controlListComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(valueChanged(int)));
    Preferences *preferences = Preferences::Instance();
    SysxIO *sysxIO = SysxIO::Instance();

    QString valueHex = QString::number(index, 16).toUpper();
    if(valueHex.length() < 2) { valueHex.prepend("000"); }
    else if(valueHex.length() < 3) { valueHex.prepend("00"); }  // check string size is 3
    else if(valueHex.length() < 4) { valueHex.prepend("0"); };

    QList<QString> valueString;
    QString lsb_a = valueHex.at(0);
    lsb_a.prepend("0");
    valueString.append(lsb_a);

    QString lsb_b = valueHex.at(1);
    lsb_b.prepend("0");
    valueString.append(lsb_b);

    QString lsb_c = valueHex.at(2);
    lsb_c.prepend("0");
    valueString.append(lsb_c);

    QString lsb_d = valueHex.at(3);
    lsb_d.prepend("0");
    valueString.append(lsb_d);

    MidiTable *midiTable = MidiTable::Instance();
    bool ok;
    int maxRange = 256;
    int value = valueHex.toInt(&ok, 16);
    int dif = value/maxRange;
    QString valueHex1 = QString::number(dif, 16).toUpper();
    if(valueHex1.length() < 2) valueHex1.prepend("0");
    QString valueHex2 = QString::number(value - (dif * maxRange), 16).toUpper();
    if(valueHex2.length() < 2) valueHex2.prepend("0");
    QString hex4 = valueHex1;
    QString hex5 = valueHex2;    //convert valueStr to 7-bit hex4, hex5

    QString mode_hex = "00";
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){mode_hex = "02";};
    Midi items = midiTable->getMidiMap("Tables", "00", "00", mode_hex, hex4, hex5);
    this->hex_msb = items.desc;
    this->hex_lsb = items.customdesc;

    /*QString m = "targetList hex_msb = "+ hex_msb + " hex_lsb = "+ hex_lsb+ " valueHex = "+ valueHex;
         std::string st = m.toStdString();
         std::cerr << st << std::endl;*/

    sysxIO->setFileSource(this->hex0, this->hex1, this->hex2, this->hex3, valueString);

    emit updateTarget(this->hex0, this->hex1, this->hex_msb, this->hex_lsb);

    //QObject::connect(this->controlListComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(valueChanged(int)));

}

void customTargetListMenu::dialogUpdateSignal(QString valueStr)
{
    Q_UNUSED(valueStr);
   /* bool ok;
    int index = valueStr.toInt(&ok, 16);
    this->controlListComboBox->setCurrentIndex(index);
    this->valueChanged(index);*/
}

void customTargetListMenu::keyPressEvent(QKeyEvent *event)
{
    int value = 0;
    _lastValue = this->controlListComboBox->currentIndex();
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

    if(value!=0){
        int _newValue = _lastValue + value;
        if(_newValue <= min)
        {
            _newValue = min;
        }
        else if(_newValue >= max)
        {
            _newValue = max;
        };
        //set_index(_newValue);
        valueChanged(_newValue);

        //QString val = QString::number(_newValue, 10);
        //std::string st = QString("combobox key press event = " + val + " "+QString::number(value,10)).toStdString();
        //std::cerr << st << std::endl;
    };
}

void customTargetListMenu::timed()
{
    if(this->underMouse() || this->hasFocus())
    {
        this->timer->stop();
        this->timer->start(500);
    }else{
    this->timer->stop();
    this->hover=false;
    this->repaint();
    clearFocus();
    };
}
