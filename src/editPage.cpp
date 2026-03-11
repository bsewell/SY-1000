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
#include "SysxIO.h"
#include <QLayout>

editPage::editPage(QWidget *parent)
    : QWidget(parent)
{
    Preferences *preferences = Preferences::Instance();
    bool ok;
    const double ratio = preferences->getPreferences("Window", "Scale", "ratio").toDouble(&ok);
    const bool singleWindow = (preferences->getPreferences("Window", "Single", "bool")=="true");

    this->layout = new QGridLayout;
    // Embedded instrument/effect pages must stay anchored to the shared left shell guide.
    this->layout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    this->layout->setContentsMargins(singleWindow ? qRound(1*ratio) : qRound(3*ratio),
                                     qRound(1*ratio),
                                     singleWindow ? qRound(1*ratio) : qRound(3*ratio),
                                     qRound(1*ratio));
    this->layout->setSpacing(singleWindow ? qRound(6*ratio) : qRound(8*ratio));

    this->groupBoxMode = false;
    this->stackControlMode = false;
    this->stackFieldMode = false;

    this->timer = new QTimer(this);

    QObject::connect(timer, SIGNAL(timeout()), this, SIGNAL(timerUpdateSignal()) );
    timer->start(3000);

    //QObject::connect(this->parent(), SIGNAL( dialogUpdateSignal() ),
    //this, SIGNAL( dialogUpdateSignal() ));

    // QObject::connect(this, SIGNAL( updateDisplay(QString) ), this->parent(), SIGNAL( updateDisplay(QString) ));
    QObject::connect(this, &editPage::dialogUpdateSignal, this, &editPage::syncPowerState);
    QObject::connect(this, &editPage::updateSignal, this, &editPage::syncPowerState);
}

int editPage::groupBoxPresetMinimumHeight(const QString &preset) const
{
    Preferences *preferences = Preferences::Instance();
    bool ok;
    const double ratio = preferences->getPreferences("Window", "Scale", "ratio").toDouble(&ok);

    if(preset == "select")
    {
        return qRound(90 * ratio);
    }
    if(preset == "combo")
    {
        return qRound(76 * ratio);
    }
    if(preset == "level")
    {
        return qRound(92 * ratio);
    }
    if(preset == "knob3")
    {
        return qRound(126 * ratio);
    }
    if(preset == "knob4")
    {
        return qRound(126 * ratio);
    }
    if(preset == "mixed")
    {
        return qRound(126 * ratio);
    }
    return 0;
}

QGridLayout *editPage::targetGridLayout() const
{
    if(this->groupBoxMode)
    {
        return this->groupBoxLayout;
    }
    if(this->stackFieldMode)
    {
        return this->stackField;
    }
    return this->layout;
}

void editPage::applyControlCellMetrics(QWidget *widget, QGridLayout *grid, int row, int rowSpan,
                                       int column, int columnSpan,
                                       const QString &direction, int minWidth) const
{
    if(widget == nullptr || grid == nullptr)
    {
        return;
    }

    Preferences *preferences = Preferences::Instance();
    bool ok;
    const double ratio = preferences->getPreferences("Window", "Scale", "ratio").toDouble(&ok);
    const int resolvedHeight = qMax(widget->minimumSizeHint().height(), widget->sizeHint().height());
    if(resolvedHeight > 0)
    {
        widget->setMinimumHeight(resolvedHeight);
        const int perRowHeight = qMax(qRound(30 * ratio), resolvedHeight / qMax(1, rowSpan));
        for(int idx = 0; idx < qMax(1, rowSpan); ++idx)
        {
            grid->setRowMinimumHeight(row + idx, perRowHeight);
        }
    }

    if(direction.contains("left") || direction.contains("right"))
    {
        const int resolvedWidth = qMax(qRound(88 * ratio), minWidth);
        widget->setMinimumWidth(resolvedWidth);
        widget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        const int perColumnWidth = qMax(qRound(80 * ratio), resolvedWidth / qMax(1, columnSpan));
        for(int idx = 0; idx < qMax(1, columnSpan); ++idx)
        {
            grid->setColumnMinimumWidth(column + idx, perColumnWidth);
        }
        return;
    }

    const int resolvedWidth = qMax(qRound(88 * ratio), minWidth);
    widget->setMinimumWidth(resolvedWidth);
    widget->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);

    const int perColumnWidth = qMax(qRound(80 * ratio), resolvedWidth / qMax(1, columnSpan));
    for(int idx = 0; idx < qMax(1, columnSpan); ++idx)
    {
        grid->setColumnMinimumWidth(column + idx, perColumnWidth);
    }
}

bool editPage::hasPowerControl() const
{
    return this->powerSwitchControl != nullptr &&
           this->powerHex0 != "void" &&
           this->powerHex1 != "void" &&
           this->powerHex2 != "void" &&
           this->powerHex3 != "void";
}

QString editPage::powerAddress0() const
{
    return this->powerHex0;
}

QString editPage::powerAddress1() const
{
    return this->powerHex1;
}

QString editPage::powerAddress2() const
{
    return this->powerHex2;
}

QString editPage::powerAddress3() const
{
    return this->powerHex3;
}

void editPage::hideInlinePowerControl()
{
    if(this->powerSwitchControl == nullptr)
    {
        return;
    }

    this->powerSwitchControl->hide();
    this->powerSwitchControl->setFixedSize(0, 0);
    this->powerSwitchControl->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

void editPage::clearPowerControl()
{
    this->powerSwitchControl = nullptr;
    this->powerHex0 = "void";
    this->powerHex1 = "void";
    this->powerHex2 = "void";
    this->powerHex3 = "void";
}

void editPage::setExternalPowerAddress(QString hex0, QString hex1, QString hex2, QString hex3)
{
    this->externalPowerHex0 = hex0;
    this->externalPowerHex1 = hex1;
    this->externalPowerHex2 = hex2;
    this->externalPowerHex3 = hex3;
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
    QGridLayout *grid = targetGridLayout();
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
    applyControlCellMetrics(ParaEQ, grid, row, rowSpan, column, columnSpan, direction, qRound(720 * Preferences::Instance()->getPreferences("Window", "Scale", "ratio").toDouble()));
    registerManagedControl(ParaEQ);
}

void editPage::addSystemOverride(int row, int column, int rowSpan, int columnSpan,
                                           QString hex0, QString hex1, QString hex2, QString hex3,
                                           QString index, QString type, QString hex_0, QString hex_1, QString hex_2, QString hex_3, QString value)
{
        customSystemOverride *SystemOverride = new customSystemOverride(this, hex0, hex1, hex2, hex3, index, type, row, column, rowSpan, columnSpan, hex_0, hex_1, hex_2, hex_3, value);
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
        registerManagedControl(SystemOverride);
}

void editPage::addKnob(int row, int column, int rowSpan, int columnSpan,
                       QString hex0, QString hex1, QString hex2, QString hex3,
                       QString background, QString direction, int lenght,
                       Qt::Alignment alignment)
{
    customControlKnob *knob = new customControlKnob(this, hex0, hex1, hex2, hex3, background, direction, lenght);
    QGridLayout *grid = targetGridLayout();
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
    applyControlCellMetrics(knob, grid, row, rowSpan, column, columnSpan, direction, qRound(118 * Preferences::Instance()->getPreferences("Window", "Scale", "ratio").toDouble()));
    registerManagedControl(knob);
}

void editPage::addDataKnob(int row, int column, int rowSpan, int columnSpan,
                           QString hex0, QString hex1, QString hex2, QString hex3,
                           QString background, QString direction, int lenght,
                           Qt::Alignment alignment)
{
    customControlDataKnob *knob = new customControlDataKnob(this, hex0, hex1, hex2, hex3, background, direction, lenght);
    QGridLayout *grid = targetGridLayout();
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
    applyControlCellMetrics(knob, grid, row, rowSpan, column, columnSpan, direction, qRound(118 * Preferences::Instance()->getPreferences("Window", "Scale", "ratio").toDouble()));
    registerManagedControl(knob);
    knob->setWhatsThis(tr("hold down mouse button and drag up/down for quick adjustment")
                       + "<br>" + tr("use scroll wheel or up/down arrow keys for fine adjustment"));
}

void editPage::addTarget(int row, int column, int rowSpan, int columnSpan,
                         QString hex0, QString hex1, QString hex2, QString hex3,
                         QString background, QString direction, int lenght,
                         Qt::Alignment alignment)
{
    customControlTarget *target = new customControlTarget(this, hex0, hex1, hex2, hex3, background, direction, lenght);
    QGridLayout *grid = targetGridLayout();
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
    applyControlCellMetrics(target, grid, row, rowSpan, column, columnSpan, direction, qRound(340 * Preferences::Instance()->getPreferences("Window", "Scale", "ratio").toDouble()));
    registerManagedControl(target);
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
    QGridLayout *grid = targetGridLayout();
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
    applyControlCellMetrics(range, grid, row, rowSpan, column, columnSpan, "bottom", qRound(240 * Preferences::Instance()->getPreferences("Window", "Scale", "ratio").toDouble()));
    registerManagedControl(range);
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
    if(this->groupBoxMode && row == 0 && column == 0 && direction == "bottom")
    {
        direction = "left";
    }

    customControlSwitch *switchbutton = new customControlSwitch(this, hex0, hex1, hex2, hex3, direction);
    QGridLayout *grid = targetGridLayout();
    if(this->groupBoxMode)
    {
        if(row == 0 && column == 0)
        {
            if(this->powerSwitchControl == nullptr)
            {
                this->powerSwitchControl = switchbutton;
                this->powerHex0 = hex0;
                this->powerHex1 = hex1;
                this->powerHex2 = hex2;
                this->powerHex3 = hex3;
                switchbutton->setLabelText("POWER");
            }
        }
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
    applyControlCellMetrics(switchbutton, grid, row, rowSpan, column, columnSpan, direction, qRound(112 * Preferences::Instance()->getPreferences("Window", "Scale", "ratio").toDouble()));
    registerManagedControl(switchbutton);
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
    QGridLayout *grid = targetGridLayout();
    if(this->stackControlMode)
    {
        const QList<int> stackIds = this->stackControlIds;
        for(int stackId : stackIds)
        {
            if(stackId >= 0 && stackId < this->stackedFields.size() && this->stackedFields.at(stackId) != nullptr)
            {
                QObject::connect(combobox, SIGNAL( currentIndexChanged(int) ),
                                 this->stackedFields.at(stackId), SLOT( setCurrentIndex(int) ));
            }
        }
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
    const double ratio = Preferences::Instance()->getPreferences("Window", "Scale", "ratio").toDouble();
    int comboMinWidth = qRound(148 * ratio);
    if(direction.contains("left_large") || direction.contains("right_large"))
    {
        comboMinWidth = qRound(220 * ratio);
    }
    else if(direction.contains("large_inst"))
    {
        comboMinWidth = qRound(240 * ratio);
    }
    else if(direction.contains("large") || direction.contains("Tables"))
    {
        comboMinWidth = qRound(180 * ratio);
    }
    applyControlCellMetrics(combobox, grid, row, rowSpan, column, columnSpan, direction, comboMinWidth);
    registerManagedControl(combobox);
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
        const QList<int> stackIds = this->stackControlIds;
        for(int stackId : stackIds)
        {
            if(stackId >= 0 && stackId < this->stackedFields.size() && this->stackedFields.at(stackId) != nullptr)
            {
                QObject::connect(combobox, SIGNAL( currentIndexChanged(int) ),
                                 this->stackedFields.at(stackId), SLOT( setCurrentIndex(int) ));
            }
        }
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
    registerManagedControl(combobox);
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
        const QList<int> stackIds = this->stackControlIds;
        for(int stackId : stackIds)
        {
            if(stackId >= 0 && stackId < this->stackedFields.size() && this->stackedFields.at(stackId) != nullptr)
            {
                QObject::connect(tabBar, SIGNAL( currentIndexChanged(int) ),
                                 this->stackedFields.at(stackId), SLOT( setCurrentIndex(int) ));
            }
        }
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
    registerManagedControl(tabBar);
    tabBar->setWhatsThis(tr("press arrow to open selection box and click on item to select")
                           + "<br>" + tr("also use scroll wheel or up/down arrow keys to change selection"));
}

void editPage::addStaticTabBar(int row, int column, int rowSpan, int columnSpan,
                               const QStringList &labels,
                               const QList<int> &mappedIndexes,
                               QString direction,
                               Qt::Alignment alignment)
{
    Preferences *preferences = Preferences::Instance();
    bool ok;
    const double ratio = preferences->getPreferences("Window", "Scale", "ratio").toDouble(&ok);

    QTabBar *tabBar = new QTabBar(this);
    if(direction.contains("boss_inst"))
    {
        tabBar->setObjectName("bossinsttabs");
        tabBar->setDrawBase(false);
        tabBar->setDocumentMode(true);
        tabBar->setExpanding(true);
        tabBar->setUsesScrollButtons(false);
        tabBar->setElideMode(Qt::ElideNone);
#ifdef Q_OS_MAC
        QFont font("Arial", qRound(15 * ratio), QFont::Bold);
#else
        QFont font("Arial", qRound(11 * ratio), QFont::Bold);
#endif
        tabBar->setFont(font);
        tabBar->setStyleSheet(
            "QTabBar#bossinsttabs { background: transparent; border: none; }"
            "QTabBar#bossinsttabs::tab { color: rgba(255,255,255,190); background: transparent; "
            "padding: 5px 10px 8px 10px; border: none; }"
            "QTabBar#bossinsttabs::tab:selected { color: rgb(54,214,255); border-bottom: 3px solid rgb(54,214,255); }"
            "QTabBar#bossinsttabs::tab:hover { color: rgb(255,255,255); }");
    }
    else
    {
        tabBar->setObjectName("pagecombobox");
        tabBar->setShape(QTabBar::RoundedNorth);
    }

    for(const QString &label : labels)
    {
        tabBar->addTab(label);
    }

    if(this->stackControlMode)
    {
        const QList<int> stackIds = this->stackControlIds;
        const QList<int> pageMap = mappedIndexes;
        QObject::connect(tabBar, &QTabBar::currentChanged, this, [this, stackIds, pageMap](int index)
        {
            const int targetIndex = (!pageMap.isEmpty() && index >= 0 && index < pageMap.size()) ? pageMap.at(index) : index;
            for(int stackId : stackIds)
            {
                if(stackId >= this->stackedFields.size() || this->stackedFields.at(stackId) == nullptr)
                {
                    continue;
                }
                this->stackedFields.at(stackId)->setCurrentIndex(targetIndex);
            }
            updateDialog(index);
        });

        const int initialIndex = tabBar->currentIndex();
        const int initialTargetIndex = (!pageMap.isEmpty() && initialIndex >= 0 && initialIndex < pageMap.size())
                                           ? pageMap.at(initialIndex)
                                           : initialIndex;
        for(int stackId : stackIds)
        {
            if(stackId >= this->stackedFields.size() || this->stackedFields.at(stackId) == nullptr)
            {
                continue;
            }
            this->stackedFields.at(stackId)->setCurrentIndex(initialTargetIndex);
        }
    }

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
    }
    registerManagedControl(tabBar);
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
        const QList<int> stackIds = this->stackControlIds;
        for(int stackId : stackIds)
        {
            if(stackId >= 0 && stackId < this->stackedFields.size() && this->stackedFields.at(stackId) != nullptr)
            {
                QObject::connect(spinbox, SIGNAL( currentIndexChanged(int) ),
                                 this->stackedFields.at(stackId), SLOT( setCurrentIndex(int) ));
            }
        }
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
    registerManagedControl(spinbox);
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
    registerManagedControl(nameEdit);
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

void editPage::addDivider(int row, int column, int rowSpan, int columnSpan, Qt::Alignment alignment)
{
    QFrame *divider = new QFrame(this);
    divider->setFrameShape(QFrame::HLine);
    divider->setFrameShadow(QFrame::Plain);
    divider->setLineWidth(1);
    divider->setMidLineWidth(0);
    divider->setMinimumHeight(1);
    divider->setMaximumHeight(1);
    divider->setStyleSheet("color: rgba(210, 220, 232, 110); background: rgba(210, 220, 232, 110);");

    QGridLayout *grid = targetGridLayout();
    if(this->groupBoxMode)
    {
        this->groupBoxLayout->addWidget(divider, row, column, rowSpan, columnSpan, alignment);
    }
    else if(this->stackFieldMode)
    {
        this->stackField->addWidget(divider, row, column, rowSpan, columnSpan, alignment);
    }
    else
    {
        this->layout->addWidget(divider, row, column, rowSpan, columnSpan, alignment);
    }

    const int perRowHeight = qMax(1, divider->minimumHeight() / qMax(1, rowSpan));
    for(int idx = 0; idx < qMax(1, rowSpan); ++idx)
    {
        grid->setRowMinimumHeight(row + idx, perRowHeight);
    }
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
    registerManagedControl(vu);
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
    newGroupBox(title, "auto", alignment);
}

void editPage::newGroupBox(QString title, QString preset, Qt::Alignment alignment)
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
    this->groupBoxMinHeights.append(groupBoxPresetMinimumHeight(preset));

    this->groupBoxLayout = new QGridLayout;
    const int groupMargin = this->flatGroupBoxes
                                ? (singleWindow ? qRound(1*ratio) : qRound(2*ratio))
                                : (singleWindow ? qRound(4*ratio) : qRound(8*ratio));
    this->groupBoxLayout->setContentsMargins(groupMargin, groupMargin, groupMargin, groupMargin);
    this->groupBoxLayout->setHorizontalSpacing(this->flatGroupBoxes
                                                   ? (singleWindow ? qRound(5*ratio) : qRound(6*ratio))
                                                   : (singleWindow ? qRound(8*ratio) : qRound(10*ratio)));
    this->groupBoxLayout->setVerticalSpacing(this->flatGroupBoxes
                                                 ? (singleWindow ? qRound(2*ratio) : qRound(3*ratio))
                                                 : (singleWindow ? qRound(4*ratio) : qRound(6*ratio)));
    this->groupBoxLayout->setSizeConstraint(QLayout::SetMinimumSize);
    this->groupBoxLayout->setAlignment(alignment | Qt::AlignTop);
    this->groupBoxLayouts.append(this->groupBoxLayout);

    this->groupBox->setTitle(title);
    this->groupBox->setObjectName("groupbox");
    this->groupBox->setFont(Sfont);
    this->groupBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    if(this->flatGroupBoxes)
    {
        this->groupBox->setFlat(true);
        this->groupBox->setStyleSheet(
            "QGroupBox#groupbox { border: none; margin-top: 0px; padding-top: 0px; }"
            "QGroupBox#groupbox::title { subcontrol-origin: margin; left: 0px; top: 0px; padding: 0px 0px 2px 0px; color: rgba(255,255,255,180); }");
    }
    this->groupBoxMode = true;
}

void editPage::addGroupBox(int row, int column, int rowSpan, int columnSpan)
{
    Preferences *preferences = Preferences::Instance();
    bool ok;
    const double ratio = preferences->getPreferences("Window", "Scale", "ratio").toDouble(&ok);
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
    QGroupBox *currentBox = this->groupBoxes.at(boxesIndex);
    currentBox->setLayout(this->groupBoxLayouts.at(layoutIndex));
    currentBox->layout()->activate();
    const int titlePad = this->flatGroupBoxes ? qRound(6 * ratio) : qRound(14 * ratio);
    const int presetMinHeight = this->groupBoxMinHeights.value(boxesIndex, 0);
    const int boxMinHeight = qMax(currentBox->layout()->sizeHint().height() + titlePad, presetMinHeight);
    currentBox->setMinimumHeight(boxMinHeight);
    currentBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    if(this->groupBoxIndex == 0)
    {
        if(this->stackFieldMode)
        {
            this->stackField->addWidget(this->groupBoxes.at(parentIndex), row, column, rowSpan, columnSpan, Qt::AlignTop | Qt::AlignLeft);
            const int perRowHeight = qMax(qRound(34 * ratio), boxMinHeight / qMax(1, rowSpan));
            for(int idx = 0; idx < qMax(1, rowSpan); ++idx)
            {
                this->stackField->setRowMinimumHeight(row + idx, perRowHeight);
            }
        }
        else
        {
            this->layout->addWidget(this->groupBoxes.at(parentIndex), row, column, rowSpan, columnSpan, Qt::AlignTop | Qt::AlignLeft);
            const int perRowHeight = qMax(qRound(34 * ratio), boxMinHeight / qMax(1, rowSpan));
            for(int idx = 0; idx < qMax(1, rowSpan); ++idx)
            {
                this->layout->setRowMinimumHeight(row + idx, perRowHeight);
            }
        };
        this->groupBoxLevel = 0;
        this->groupBoxIndexList.clear();
        this->groupBoxMode = false;
    }
    else
    {
        QGridLayout *parentLayout = this->groupBoxLayouts.at(layoutIndex - 1 - this->parentBoxDif);
        parentLayout->addWidget(this->groupBoxes.at(boxesIndex), row, column, rowSpan, columnSpan, Qt::AlignTop | Qt::AlignLeft);
        const int perRowHeight = qMax(qRound(34 * ratio), boxMinHeight / qMax(1, rowSpan));
        for(int idx = 0; idx < qMax(1, rowSpan); ++idx)
        {
            parentLayout->setRowMinimumHeight(row + idx, perRowHeight);
        }
        this->groupBoxLayout = parentLayout;
        this->groupBoxIndex--;
    };
}

void editPage::setFlatGroupBoxes(bool enabled)
{
    this->flatGroupBoxes = enabled;
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
    strechedLayout->addLayout(this->layout);
    strechedLayout->addStretch();

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setContentsMargins(singleWindow ? qRound(2*ratio) : 0, qRound(4*ratio), 0, 0);
    mainLayout->setSpacing(0);
    mainLayout->addLayout(strechedLayout);
    mainLayout->addStretch();

    this->setLayout(mainLayout);
    syncPowerState();
}

void editPage::registerManagedControl(QWidget *widget)
{
    if(widget)
    {
        this->managedControls.append(widget);
    }
}

void editPage::newStackControl(int id)
{
    this->stackControlMode = true;
    this->stackControlId = id;
    this->stackControlIds.clear();
    this->stackControlIds.append(id);
    QStackedWidget *newStackField = new QStackedWidget;
    while(this->stackedFields.size() <= id)
    {
        this->stackedFields.append(nullptr);
    }
    this->stackedFields[id] = newStackField;
}

void editPage::linkStackControl(int id)
{
    if(!this->stackControlIds.contains(id))
    {
        this->stackControlIds.append(id);
    }
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
    this->stackFieldLockWidth = false;
    this->stackField = new QGridLayout;
    this->stackField->setContentsMargins(2*ratio, 2*ratio, 2*ratio, 2*ratio);
    this->stackField->setHorizontalSpacing(qRound(4*ratio));
    this->stackField->setVerticalSpacing(qRound(4*ratio));
    this->stackField->setSizeConstraint(QLayout::SetMinimumSize);
    this->stackField->setAlignment(alignment);
}

void editPage::compactCurrentStackField(int horizontalSpacing, int verticalSpacing)
{
    if(this->stackField == nullptr)
    {
        return;
    }

    this->stackField->setHorizontalSpacing(horizontalSpacing);
    if(verticalSpacing >= 0)
    {
        this->stackField->setVerticalSpacing(verticalSpacing);
    }
    this->stackFieldLockWidth = true;
}

void editPage::setStackCurrentIndex(int id, int index)
{
    if(id < 0 || id >= this->stackedFields.size() || this->stackedFields.at(id) == nullptr)
    {
        return;
    }

    if(index < 0 || index >= this->stackedFields.at(id)->count())
    {
        return;
    }

    this->stackedFields.at(id)->setCurrentIndex(index);
}

void editPage::addStackField()
{
    this->stackFieldMode = false;

    QWidget *tmpWidget = new QWidget;
    tmpWidget->setLayout(this->stackField);
    tmpWidget->setSizePolicy(this->stackFieldLockWidth ? QSizePolicy::Maximum : QSizePolicy::Expanding,
                             QSizePolicy::Preferred);
    tmpWidget->setMinimumHeight(this->stackField->sizeHint().height());

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

void editPage::syncPowerState()
{
    QString activeHex0 = this->powerHex0;
    QString activeHex1 = this->powerHex1;
    QString activeHex2 = this->powerHex2;
    QString activeHex3 = this->powerHex3;
    QWidget *exemptControl = this->powerSwitchControl;

    if(activeHex0 == "void" || activeHex1 == "void" ||
       activeHex2 == "void" || activeHex3 == "void")
    {
        activeHex0 = this->externalPowerHex0;
        activeHex1 = this->externalPowerHex1;
        activeHex2 = this->externalPowerHex2;
        activeHex3 = this->externalPowerHex3;
        exemptControl = nullptr;
    }

    if(activeHex0 == "void" || activeHex1 == "void" ||
       activeHex2 == "void" || activeHex3 == "void")
    {
        return;
    }

    SysxIO *sysxIO = SysxIO::Instance();
    const bool enabled = (sysxIO->getSourceValue(activeHex0, activeHex1, activeHex2, activeHex3) > 0);
    const QList<QWidget*> &controls = this->managedControls;
    for(QWidget *widget : controls)
    {
        if(!widget)
        {
            continue;
        }

        if(widget == exemptControl)
        {
            widget->setEnabled(true);
            continue;
        }

        widget->setEnabled(enabled);
    }
}
