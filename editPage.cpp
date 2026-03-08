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

#include "editPage.h"
#include <QLabel>
#include <QTimer>
#include "customSwitch.h"
#include "customControlParaEQ.h"
#include "customControlKnob.h"
#include "customControlDataKnob.h"
#include "customControlTabMenu.h"
#include "customControlTarget.h"
#include "customControlRange.h"
#include "customControlLabel.h"
#include "customControlSpinBox.h"
#include "customControlSwitch.h"
#include "customControlListMenu.h"
#include "customMultiComboBox.h"
#include "customSystemOverride.h"
#include "customRenameWidget.h"
#include "customControlVU.h"
#include "Preferences.h"

editPage::editPage(QWidget *parent)
    : QWidget(parent)
{
    Preferences *preferences = Preferences::Instance();
    bool ok;
    const double ratio = preferences->getPreferences("Window", "Scale", "ratio").toDouble(&ok);
    const bool singleWindow = (preferences->getPreferences("Window", "Single", "bool")=="true");

    this->layout = new QGridLayout;
    this->layout->setAlignment(Qt::AlignTop | (singleWindow ? Qt::AlignLeft : Qt::AlignHCenter));
    this->layout->setContentsMargins(singleWindow ? qRound(1*ratio) : qRound(3*ratio),
                                     qRound(3*ratio),
                                     singleWindow ? qRound(1*ratio) : qRound(3*ratio),
                                     qRound(3*ratio));
    this->layout->setSpacing(singleWindow ? qRound(3*ratio) : qRound(5*ratio));

    this->groupBoxMode = false;
    this->stackControlMode = false;
    this->stackFieldMode = false;

    this->timer = new QTimer(this);

    QObject::connect(timer, SIGNAL(timeout()), this, SIGNAL(timerUpdateSignal()) );
    timer->start(3000);

    //QObject::connect(this->parent(), SIGNAL( dialogUpdateSignal() ),
    //this, SIGNAL( dialogUpdateSignal() ));

    // QObject::connect(this, SIGNAL( updateDisplay(QString) ), this->parent(), SIGNAL( updateDisplay(QString) ));
}

void editPage::paintEvent(QPaintEvent *)
{
    /*DRAWS RED BACKGROUND FOR DEBUGGING PURPOSE */
    /*QPixmap image(":/images/dragbar.png");

    QRectF target(0.0, 0.0, this->width(), this->height());
    QRectF source(0.0, 0.0, this->width(), this->height());

    QPainter painter(this);
    painter.drawPixmap(target, image, source);*/
}

void editPage::addParaEQ(int row, int column, int rowSpan, int columnSpan,
                         QString hex0, QString hex1, QString hex2, QString hex3,
                         QString background, QString direction, int lenght,
                         Qt::Alignment alignment)
{
    customControlParaEQ *ParaEQ = new customControlParaEQ(this, hex0, hex1, hex2, hex3, background, direction, lenght);
    if(this->groupBoxMode)
    {
        this->groupBoxLayout->addWidget(ParaEQ, row, column, rowSpan, columnSpan, alignment);
    }
    else if(this->stackFieldMode)
    {
        this->stackField->addWidget(ParaEQ, row, column, rowSpan, columnSpan, alignment);
    }
    else
    {
        this->layout->addWidget(ParaEQ, row, column, rowSpan, columnSpan, alignment);
    };
}

void editPage::addSystemOverride(int row, int column, int rowSpan, int columnSpan,
                                           QString hex0, QString hex1, QString hex2, QString hex3,
                                           QString index, QString type, QString hex_0, QString hex_1, QString hex_2, QString hex_3, QString value)
{
        customSystemOverride *SystemOverride = new customSystemOverride(this, hex0, hex1, hex2, hex3, index, type, rowSpan, columnSpan, hex_0, hex_1, hex_2, hex_3, value);
        if(this->groupBoxMode)
        {
                this->groupBoxLayout->addWidget(SystemOverride, row, column, rowSpan, columnSpan);
        }
        else if(this->stackFieldMode)
        {
                this->stackField->addWidget(SystemOverride, row, column, rowSpan, columnSpan);
        }
        else
        {
                this->layout->addWidget(SystemOverride, row, column, rowSpan, columnSpan);
        };
}

void editPage::addKnob(int row, int column, int rowSpan, int columnSpan,
                       QString hex0, QString hex1, QString hex2, QString hex3,
                       QString background, QString direction, int lenght,
                       Qt::Alignment alignment)
{
    customControlKnob *knob = new customControlKnob(this, hex0, hex1, hex2, hex3, background, direction, lenght);
    if(this->groupBoxMode)
    {
        this->groupBoxLayout->addWidget(knob, row, column, rowSpan, columnSpan, alignment);
    }
    else if(this->stackFieldMode)
    {
        this->stackField->addWidget(knob, row, column, rowSpan, columnSpan, alignment);
    }
    else
    {
        this->layout->addWidget(knob, row, column, rowSpan, columnSpan, alignment);
    };
}

void editPage::addDataKnob(int row, int column, int rowSpan, int columnSpan,
                           QString hex0, QString hex1, QString hex2, QString hex3,
                           QString background, QString direction, int lenght,
                           Qt::Alignment alignment)
{
    customControlDataKnob *knob = new customControlDataKnob(this, hex0, hex1, hex2, hex3, background, direction, lenght);
    if(this->groupBoxMode)
    {
        this->groupBoxLayout->addWidget(knob, row, column, rowSpan, columnSpan, alignment);
    }
    else if(this->stackFieldMode)
    {
        this->stackField->addWidget(knob, row, column, rowSpan, columnSpan, alignment);
    }
    else
    {
        this->layout->addWidget(knob, row, column, rowSpan, columnSpan, alignment);
    };
    knob->setWhatsThis(tr("hold down mouse button and drag up/down for quick adjustment")
                       + "<br>" + tr("use scroll wheel or up/down arrow keys for fine adjustment"));
}

void editPage::addTarget(int row, int column, int rowSpan, int columnSpan,
                         QString hex0, QString hex1, QString hex2, QString hex3,
                         QString background, QString direction, int lenght,
                         Qt::Alignment alignment)
{
    customControlTarget *target = new customControlTarget(this, hex0, hex1, hex2, hex3, background, direction, lenght);
    if(this->groupBoxMode)
    {
        this->groupBoxLayout->addWidget(target, row, column, rowSpan, columnSpan, alignment);
    }
    else if(this->stackFieldMode)
    {
        this->stackField->addWidget(target, row, column, rowSpan, columnSpan, alignment);
    }
    else
    {
        this->layout->addWidget(target, row, column, rowSpan, columnSpan, alignment);
    };
    target->setWhatsThis(tr("hold down mouse button and drag up/down for quick adjustment or")
                         + "<br>" + tr("press arrow to open list box and click on an item to select")
                         + "<br>" + tr(" or use scroll wheel or up/down arrow keys for fine adjustment"));
}

void editPage::addRange(int row, int column, int rowSpan, int columnSpan,
                        QString hex0 ,QString hex1, QString hex2, QString hex3,
                        QString area,
                        Qt::Alignment alignment)
{
    customControlRange *range = new customControlRange(this, hex0, hex1, hex2, hex3, area);
    if(this->groupBoxMode)
    {
        this->groupBoxLayout->addWidget(range, row, column, rowSpan, columnSpan, alignment);
    }
    else if(this->stackFieldMode)
    {
        this->stackField->addWidget(range, row, column, rowSpan, columnSpan, alignment);
    }
    else
    {
        this->layout->addWidget(range, row, column, rowSpan, columnSpan, alignment);
    };
    range->setWhatsThis(tr("hold down mouse button and drag up/down for quick adjustment")
                        + "<br>" + tr("use scroll wheel or up/down arrow keys for fine adjustment"));
}

void editPage::addSwitch(int row, int column, int rowSpan, int columnSpan,
                         QString hex0,
                         QString hex1,
                         QString hex2,
                         QString hex3,
                         QString direction,
                         Qt::Alignment alignment)
{
    customControlSwitch *switchbutton = new customControlSwitch(this, hex0, hex1, hex2, hex3, direction);
    if(this->groupBoxMode)
    {
        this->groupBoxLayout->addWidget(switchbutton, row, column, rowSpan, columnSpan, alignment);
    }
    else if(this->stackFieldMode)
    {
        this->stackField->addWidget(switchbutton, row, column, rowSpan, columnSpan, alignment);
    }
    else
    {
        this->layout->addWidget(switchbutton, row, column, rowSpan, columnSpan, alignment);
    };
    switchbutton->setWhatsThis(tr("press with mouse button to toggle switch state")
                               + "<br>" + tr("a lit button indicates and effect is ON"));
}

void editPage::addComboBox(int row, int column, int rowSpan, int columnSpan,
                           QString hex0,
                           QString hex1,
                           QString hex2,
                           QString hex3,
                           QString direction,
                           Qt::Alignment alignment)
{
    customControlListMenu *combobox = new customControlListMenu(this, hex0, hex1, hex2, hex3, direction);
    if(this->stackControlMode)
    {
        QObject::connect(combobox, SIGNAL( currentIndexChanged(int) ),
                         this->stackedFields.at(this->stackControlId), SLOT( setCurrentIndex(int) ));
        QObject::connect(combobox, SIGNAL( currentIndexChanged(int) ),
                         this, SLOT( updateDialog(int) ));
    };
    if(this->groupBoxMode)
    {
        this->groupBoxLayout->addWidget(combobox, row, column, rowSpan, columnSpan, alignment);
    }
    else if(this->stackFieldMode)
    {
        this->stackField->addWidget(combobox, row, column, rowSpan, columnSpan, alignment);
    }
    else
    {
        this->layout->addWidget(combobox, row, column, rowSpan, columnSpan, alignment);
    };
    combobox->setWhatsThis(tr("press arrow to open selection box and click on item to select")
                           + "<br>" + tr("also use scroll wheel or up/down arrow keys to change selection"));
}

void editPage::addMultiComboBox(int row, int column, int rowSpan, int columnSpan,
                                QString hex1,
                                QString hex2,
                                QString hex3,
                                QString direction,
                                Qt::Alignment alignment)
{
    customMultiComboBox *combobox = new customMultiComboBox(this, hex1, hex2, hex3, direction);
    if(this->stackControlMode)
    {
        QObject::connect(combobox, SIGNAL( currentIndexChanged(int) ),
                         this->stackedFields.at(this->stackControlId), SLOT( setCurrentIndex(int) ));
        QObject::connect(combobox, SIGNAL( currentIndexChanged(int) ),
                         this, SLOT( updateDialog(int) ));
    };
    if(this->groupBoxMode)
    {
        this->groupBoxLayout->addWidget(combobox, row, column, rowSpan, columnSpan, alignment);
    }
    else if(this->stackFieldMode)
    {
        this->stackField->addWidget(combobox, row, column, rowSpan, columnSpan, alignment);
    }
    else
    {
        this->layout->addWidget(combobox, row, column, rowSpan, columnSpan, alignment);
    };
}

void editPage::addTabBar(int row, int column, int rowSpan, int columnSpan,
                           QString hex0,
                           QString hex1,
                           QString hex2,
                           QString hex3,
                           QString direction,
                           Qt::Alignment alignment)
{
    customControlTabMenu *tabBar = new customControlTabMenu(this, hex0, hex1, hex2, hex3, direction);
    if(this->stackControlMode)
    {
        QObject::connect(tabBar, SIGNAL( currentIndexChanged(int) ),
                         this->stackedFields.at(this->stackControlId), SLOT( setCurrentIndex(int) ));
        QObject::connect(tabBar, SIGNAL( currentIndexChanged(int) ),
                         this, SLOT( updateDialog(int) ));
    };
    if(this->groupBoxMode)
    {
        this->groupBoxLayout->addWidget(tabBar, row, column, rowSpan, columnSpan, alignment);
    }
    else if(this->stackFieldMode)
    {
        this->stackField->addWidget(tabBar, row, column, rowSpan, columnSpan, alignment);
    }
    else
    {
        this->layout->addWidget(tabBar, row, column, rowSpan, columnSpan, alignment);
    };
    tabBar->setWhatsThis(tr("press arrow to open selection box and click on item to select")
                           + "<br>" + tr("also use scroll wheel or up/down arrow keys to change selection"));
}

void editPage::addSpinBox(int row, int column, int rowSpan, int columnSpan,
                           QString hex0,
                           QString hex1,
                           QString hex2,
                           QString hex3,
                           QString direction,
                           Qt::Alignment alignment)
{
    customControlSpinBox *spinbox = new customControlSpinBox(this, hex0, hex1, hex2, hex3, direction);
    if(this->stackControlMode)
    {
        QObject::connect(spinbox, SIGNAL( currentIndexChanged(int) ),
                         this->stackedFields.at(this->stackControlId), SLOT( setCurrentIndex(int) ));
        QObject::connect(spinbox, SIGNAL( currentIndexChanged(int) ),
                         this, SLOT( updateDialog(int) ));
    };
    if(this->groupBoxMode)
    {
        this->groupBoxLayout->addWidget(spinbox, row, column, rowSpan, columnSpan, alignment);
    }
    else if(this->stackFieldMode)
    {
        this->stackField->addWidget(spinbox, row, column, rowSpan, columnSpan, alignment);
    }
    else
    {
        this->layout->addWidget(spinbox, row, column, rowSpan, columnSpan, alignment);
    };
    spinbox->setWhatsThis(tr("press arrow to open selection box and click on item to select")
                           + "<br>" + tr("also use scroll wheel or up/down arrow keys to change selection"));
}

void editPage::addNameEdit(int row, int column, int rowSpan, int columnSpan,
                           QString hex0,
                           QString hex1,
                           QString hex2,
                           QString hex3,
                           QString length,
                           Qt::Alignment alignment)
{
    customRenameWidget *nameEdit = new customRenameWidget(this, hex0, hex1, hex2, hex3, length);

    if(this->groupBoxMode)
    {
        this->groupBoxLayout->addWidget(nameEdit, row, column, rowSpan, columnSpan, alignment);
    }
    else if(this->stackFieldMode)
    {
        this->stackField->addWidget(nameEdit, row, column, rowSpan, columnSpan, alignment);
    }
    else
    {
        this->layout->addWidget(nameEdit, row, column, rowSpan, columnSpan, alignment);
    };
}

void editPage::addLabel(int row, int column, int rowSpan, int columnSpan, QString text, Qt::Alignment alignment)
{
    customControlLabel *label = new customControlLabel(this);
    label->setText(text);

    if(this->groupBoxMode)
    {
        this->groupBoxLayout->addWidget(label, row, column, rowSpan, columnSpan, alignment);
    }
    else if(this->stackFieldMode)
    {
        this->stackField->addWidget(label, row, column, rowSpan, columnSpan, alignment);
    }
    else
    {
        this->layout->addWidget(label, row, column, rowSpan, columnSpan, alignment);
    };
}

void editPage::addVU(int row, int column, int rowSpan, int columnSpan,
                       QString hex0, QString hex1, QString hex2, QString hex3,
                       QString background, QString direction, int lenght,
                       Qt::Alignment alignment)
{
    customControlVU *vu = new customControlVU(this, hex0, hex1, hex2, hex3, background, direction, lenght);
    if(this->groupBoxMode)
    {
        this->groupBoxLayout->addWidget(vu, row, column, rowSpan, columnSpan, alignment);
    }
    else if(this->stackFieldMode)
    {
        this->stackField->addWidget(vu, row, column, rowSpan, columnSpan, alignment);
    }
    else
    {
        this->layout->addWidget(vu, row, column, rowSpan, columnSpan, alignment);
    };
}

void editPage::valueChanged(bool value, QString hex1, QString hex2, QString hex3)
{
    Q_UNUSED(value);
    Q_UNUSED(hex1);
    Q_UNUSED(hex2);
    Q_UNUSED(hex3);
    QApplication::beep();
}

void editPage::newGroupBox(QString title, Qt::Alignment alignment)
{
    Preferences *preferences = Preferences::Instance();
    bool ok;
    const double ratio = preferences->getPreferences("Window", "Scale", "ratio").toDouble(&ok);
    const double fratio = preferences->getPreferences("Window", "Font", "ratio").toDouble(&ok);
    const bool singleWindow = (preferences->getPreferences("Window", "Single", "bool")=="true");
    QFont Sfont( "Arial", 8*fratio, QFont::Normal);

    if(this->groupBoxMode)
    {
        if(this->groupBoxIndex == 0 && this->groupBoxLevel != 0)
        {
            this->parentBoxDif++;
        };
        this->groupBoxLevel++;
        this->groupBoxIndex++;
    }
    else
    {
        this->groupBoxLevel = 0;
        this->groupBoxIndex = 0;
        this->parentBoxDif = 0;
    };
    this->groupBoxIndexList.append(this->groupBoxLevel);

    this->groupBox = new QGroupBox;
    this->groupBoxes.append(this->groupBox);

    this->groupBoxLayout = new QGridLayout;
    const int groupMargin = singleWindow ? qRound(6*ratio) : qRound(10*ratio);
    this->groupBoxLayout->setContentsMargins(groupMargin, groupMargin, groupMargin, groupMargin);
    this->groupBoxLayout->setSpacing(singleWindow ? qRound(2*ratio) : qRound(3*ratio));
    this->groupBoxLayout->setAlignment(alignment | Qt::AlignTop);
    this->groupBoxLayouts.append(this->groupBoxLayout);

    this->groupBox->setTitle(title);
    this->groupBox->setObjectName("groupbox");
    this->groupBox->setFont(Sfont);
    this->groupBox->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    this->groupBoxMode = true;
}

void editPage::addGroupBox(int row, int column, int rowSpan, int columnSpan)
{
    int currentIndex = this->groupBoxIndexList.at(this->groupBoxIndex);

    int layoutIndex = this->groupBoxLayouts.size() - (this->groupBoxLevel - currentIndex) - 1;
    int boxesIndex = this->groupBoxes.size() - (this->groupBoxLevel - currentIndex) - 1;
    int parentIndex = this->groupBoxes.size() - this->groupBoxLevel - 1;

    int removeIndex = this->groupBoxIndexList.indexOf(currentIndex);
    this->groupBoxIndexList.removeAt(removeIndex);
    for(int i = removeIndex; i < this->groupBoxIndexList.size() - 1; ++i)
    {
        this->groupBoxIndexList.move(i + 1, i);
    };

    //QString snork = this->groupBoxes.at(boxesIndex)->title();
    this->groupBoxes.at(boxesIndex)->setLayout(this->groupBoxLayouts.at(layoutIndex));

    if(this->groupBoxIndex == 0)
    {
        if(this->stackFieldMode)
        {
            this->stackField->addWidget(this->groupBoxes.at(parentIndex), row, column, rowSpan, columnSpan, Qt::AlignTop | Qt::AlignLeft);
        }
        else
        {
            this->layout->addWidget(this->groupBoxes.at(parentIndex), row, column, rowSpan, columnSpan, Qt::AlignTop | Qt::AlignLeft);
        };
        this->groupBoxLevel = 0;
        this->groupBoxIndexList.clear();
        this->groupBoxMode = false;
    }
    else
    {
        this->groupBoxLayouts.at(layoutIndex - 1 - this->parentBoxDif)->addWidget(this->groupBoxes.at(boxesIndex), row, column, rowSpan, columnSpan, Qt::AlignTop | Qt::AlignLeft);
        this->groupBoxLayout = this->groupBoxLayouts.at(layoutIndex - 1 - this->parentBoxDif);
        this->groupBoxIndex--;
    };
}

void editPage::setGridLayout()
{
    Preferences *preferences = Preferences::Instance();
    bool ok;
    const double ratio = preferences->getPreferences("Window", "Scale", "ratio").toDouble(&ok);
    const bool singleWindow = (preferences->getPreferences("Window", "Single", "bool")=="true");

    QHBoxLayout *strechedLayout = new QHBoxLayout;
    strechedLayout->setContentsMargins(0, 0, 0, 0);
    strechedLayout->setSpacing(0);
    if(!singleWindow)
    {
        strechedLayout->addStretch();
    }
    strechedLayout->addLayout(this->layout);
    strechedLayout->addStretch();

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setContentsMargins(singleWindow ? qRound(2*ratio) : 0, 20*ratio, 0, 0);
    mainLayout->setSpacing(0);
    mainLayout->addLayout(strechedLayout);
    mainLayout->addStretch();

    this->setLayout(mainLayout);
}

void editPage::newStackControl(int id)
{
    this->stackControlMode = true;
    this->stackControlId = id;
    QStackedWidget *newStackField = new QStackedWidget;
    this->stackedFields.insert(id, newStackField);
}

void editPage::addStackControl()
{
    this->stackControlMode = false;
}

void editPage::insertStackField(int id,
                                int row, int column, int rowSpan, int columnSpan,
                                Qt::Alignment alignment)
{
    if(this->groupBoxMode)
    {
        this->groupBoxLayout->addWidget(this->stackedFields.at(id), row, column, rowSpan, columnSpan, alignment);
    }
    else if(this->stackFieldMode)
    {
        this->stackField->addWidget(this->stackedFields.at(id), row, column, rowSpan, columnSpan, alignment);
    }
    else
    {
        this->layout->addWidget(this->stackedFields.at(id), row, column, rowSpan, columnSpan, alignment);
    };
}

void editPage::newStackField(int id, Qt::Alignment alignment)
{
    Preferences *preferences = Preferences::Instance();
    bool ok;
    const double ratio = preferences->getPreferences("Window", "Scale", "ratio").toDouble(&ok);

    this->fieldIndexes.clear();
    this->stackFieldMode = true;
    this->stackFieldId = id;
    this->stackField = new QGridLayout;
    this->stackField->setContentsMargins(3*ratio, 3*ratio, 3*ratio, 3*ratio);
    this->stackField->setSpacing(3);
    this->stackField->setAlignment(alignment);
}

void editPage::addStackField()
{
    this->stackFieldMode = false;

    QWidget *tmpWidget = new QWidget;
    tmpWidget->setLayout(this->stackField);

    this->stackedFields.at(this->stackFieldId)->addWidget(tmpWidget);
}

void editPage::updateDialog(int index)
{
    Q_UNUSED(index);
    emit dialogUpdateSignal();
}

void editPage::updateDisplay(QString text)
{
    text = "nul";
    //
}
