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

#include "customControlTabMenu.h"
#include "MidiTable.h"
#include "SysxIO.h"
#include "Preferences.h"

customControlTabMenu::customControlTabMenu(QWidget *parent,
                                           QString hex0, QString hex1, QString hex2, QString hex3, QString direction)
    : QWidget(parent)
{
    Preferences *preferences = Preferences::Instance();
    bool ok;
    const double ratio = preferences->getPreferences("Window", "Scale", "ratio").toDouble(&ok);

    this->direction = direction;
    this->label = new customControlLabel(this);
    this->controlListTab = new QTabBar(this);
    int largeFont = 13;
    int smallFont = 8;
#ifdef Q_OS_MAC
    largeFont = 20;
    smallFont = 12;
#endif

    if(direction.contains("Tables") || direction.contains("large"))
    {
        this->controlListTab->setObjectName("pagecombobox");
        QFont Cfont( "Arial", largeFont*ratio, QFont::Normal);
        this->controlListTab->setFont(Cfont);
    }
    else
    {
        this->controlListTab->setObjectName("pagecombobox");
        QFont Cfont( "Arial", smallFont*ratio, QFont::Normal);
        this->controlListTab->setFont(Cfont);
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
    this->label->setText(labeltxt);

    setComboBox();

    if(direction.contains("left"))
    {
        this->controlListTab->setShape(QTabBar::RoundedWest);
        this->label->setAlignment(Qt::AlignLeft);

        QVBoxLayout *mainLayout = new QVBoxLayout;
        /* mainLayout->setContentsMargins(0, 0, 0, 0);
        mainLayout->setSpacing(0);
        mainLayout->addStretch(0);*/
        mainLayout->addWidget(this->label, 0, Qt::AlignLeft);
        mainLayout->addWidget(this->controlListTab, 0, Qt::AlignLeft);

        this->setLayout(mainLayout);
    }
    else if(direction.contains("right"))
    {
        this->controlListTab->setShape(QTabBar::RoundedEast);
        this->label->setAlignment(Qt::AlignLeft);

        QVBoxLayout *mainLayout = new QVBoxLayout;
        mainLayout->setContentsMargins(0, 0, 0, 0);
        mainLayout->setSpacing(0);
        mainLayout->addStretch(0);
        mainLayout->addWidget(this->label, 0, Qt::AlignLeft);
        mainLayout->addWidget(this->controlListTab, 0, Qt::AlignLeft);

        this->setLayout(mainLayout);
    }
    else if(direction.contains("top"))
    {
        this->controlListTab->setShape(QTabBar::RoundedNorth);
        this->label->setAlignment(Qt::AlignLeft);

        QVBoxLayout *mainLayout = new QVBoxLayout;
        /*mainLayout->setContentsMargins(0, 0, 0, 0);
        mainLayout->setSpacing(0);
        mainLayout->addStretch(0);*/
        mainLayout->addWidget(this->label, 0, Qt::AlignLeft);
        mainLayout->addWidget(this->controlListTab, 0, Qt::AlignLeft);

        this->setLayout(mainLayout);
    }
    else if(direction.contains("bottom"))
    {
        this->controlListTab->setShape(QTabBar::RoundedSouth);
        this->label->setAlignment(Qt::AlignLeft);

        QVBoxLayout *mainLayout = new QVBoxLayout;
        mainLayout->setContentsMargins(0, 0, 0, 0);
        mainLayout->setSpacing(0);
        mainLayout->addStretch(0);
        mainLayout->addWidget(this->label, 0, Qt::AlignLeft);
        mainLayout->addWidget(this->controlListTab, 0, Qt::AlignLeft);

        this->setLayout(mainLayout);
    }
    else
    {
        this->controlListTab->setShape(QTabBar::RoundedWest);
        this->label->setAlignment(Qt::AlignLeft);

        QVBoxLayout *mainLayout = new QVBoxLayout;
        mainLayout->setContentsMargins(0, 0, 0, 0);
        mainLayout->setSpacing(0);
        mainLayout->addStretch(0);
        mainLayout->addWidget(this->label, 0, Qt::AlignLeft);
        mainLayout->addWidget(this->controlListTab, 0, Qt::AlignLeft);

        this->setLayout(mainLayout);

    };

    QObject::connect(this->parent(), SIGNAL( dialogUpdateSignal() ), this, SLOT( dialogUpdateSignal() ));

    QObject::connect(this, SIGNAL( updateSignal() ), this->parent(), SIGNAL( updateSignal() ));

    QObject::connect(this->controlListTab, SIGNAL(tabBarClicked(int)), this, SLOT(valueChanged(int)));

    QObject::connect(this->controlListTab, SIGNAL(tabBarClicked(int)), this, SIGNAL(currentIndexChanged(int)));

    if(this->direction.contains("Tables"))
    {
        QObject::connect(this->controlListTab, SIGNAL(tabBarClicked(int)), this->parent(), SLOT(select_patch()));
    };
}

void customControlTabMenu::paintEvent(QPaintEvent *)
{

}

void customControlTabMenu::setComboBox()
{
    QString longestItem = "";
    int itemcount;
    MidiTable *midiTable = MidiTable::Instance();
    Midi items;
    items = midiTable->getMidiMap(this->hex0, this->hex1, this->hex2, this->hex3);


    for(itemcount=0;itemcount<items.level.size();itemcount++ )
    {
        QString item;
        QString name = items.level.at(itemcount).name;
        QString desc = items.level.at(itemcount).desc;
        QString customdesc = items.level.at(itemcount).customdesc;
        if(!desc.isEmpty())
        {
            item = desc;
        }
        else if(!customdesc.isEmpty())
        {
            item = customdesc;
        }
        else
        {
            item = name;
        };
        if(longestItem.size() < item.size()) longestItem = item;
        this->controlListTab->addTab(item);
    };

}

void customControlTabMenu::valueChanged(int index)
{
    if(!this->direction.contains("Tables") && !this->hex0.contains("Tables"))
    {
        QList<QString> hexData;
        QString valueHex = QString::number(index, 16).toUpper();
        if(valueHex.length() < 2) valueHex.prepend("0");

        SysxIO *sysxIO = SysxIO::Instance();
        MidiTable *midiTable = MidiTable::Instance();
        bool ok;
        if(midiTable->isData(this->hex0, this->hex1, this->hex2, this->hex3))
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
        emit updateSignal();
    };
}

void customControlTabMenu::dialogUpdateSignal()
{
    if(!this->direction.contains("Tables") && !this->hex0.contains("Tables"))
    {
        SysxIO *sysxIO = SysxIO::Instance();
        int index = sysxIO->getSourceValue(this->hex0, this->hex1, this->hex2, this->hex3);
        if(index != _lastValue){
            _lastValue = index;
            this->controlListTab->setCurrentIndex(index);
            this->valueChanged(index);
        };
    };

    /*if(this->direction.contains("inst"))
    {
        this->controlListTab->setTabTextColor(0, QColor(255,255,255));
        this->controlListTab->setTabTextColor(1, QColor(255,255,255));
        this->controlListTab->setTabTextColor(2, QColor(255,255,255));
        this->controlListTab->setTabTextColor(3, QColor(255,255,255));
        this->controlListTab->setTabTextColor(4, QColor(255,255,255));
        this->controlListTab->setTabTextColor(5, QColor(255,255,255));
        this->controlListTab->setTabTextColor(6, QColor(255,255,255));
        this->controlListTab->setTabTextColor(7, QColor(255,255,255));
        if(this->hex3=="11")
        {
        qDebug() << "controlListTab" << this->hex0 << this->hex1 << this->hex2 << this->hex3 << this->direction;
        };
    };*/
}

void customControlTabMenu::set_index(int index)
{
    this->controlListTab->setCurrentIndex(index);
}

