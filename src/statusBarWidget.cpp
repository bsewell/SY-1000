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

#include <QtWidgets>
#include "statusBarWidget.h"
#include "Preferences.h"

statusBarWidget::statusBarWidget(QWidget *parent)
    : QWidget(parent)
{
    Preferences *preferences = Preferences::Instance();
    bool ok;
    const double ratio = preferences->getPreferences("Window", "Scale", "ratio").toDouble(&ok);

    this->progressBar = new QProgressBar(this);
    this->progressBar->setTextVisible(false);
    this->progressBar->setFixedSize(80*ratio, 13*ratio);
    this->progressBar->setRange(0, 100);
    this->progressBar->setValue(0);

    this->symbol = new statusBarSymbol(this);
    this->symbol->setValue(0);

    QFont labelFont;
    labelFont.setFamily("Roboto Condensed");
    labelFont.setPixelSize(11*ratio);

    this->label = new QLabel(this);
    this->label->setFixedWidth(150*ratio);
    this->label->setText("");
    this->label->setFont(labelFont);

    this->dBuglabel = new QStatusBar(this);
    this->dBuglabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    this->dBuglabel->showMessage(tr(""));
    this->dBuglabel->setFont(labelFont);

    this->midiMonitorBtn = new QPushButton("MIDI", this);
    this->midiMonitorBtn->setFixedSize(52*ratio, 18*ratio);
    this->midiMonitorBtn->setCheckable(true);
    this->midiMonitorBtn->setFont(labelFont);
    this->midiMonitorBtn->setStyleSheet(
        "QPushButton { background: #2A2A3A; color: #88AAFF; border: 1px solid #444; "
        "border-radius: 3px; font-weight: bold; }"
        "QPushButton:checked { background: #334; color: #00FF88; border-color: #00FF88; }"
        "QPushButton:hover { background: #333; }"
    );

    QHBoxLayout *widgetLayout = new QHBoxLayout;
    widgetLayout->addWidget(this->progressBar, 0, Qt::AlignCenter);
    widgetLayout->addWidget(this->symbol, 0, Qt::AlignCenter);
    widgetLayout->addWidget(this->label, 0, Qt::AlignCenter);
    widgetLayout->addWidget(this->dBuglabel, 1);
    widgetLayout->addWidget(this->midiMonitorBtn, 0, Qt::AlignCenter);

    this->setLayout(widgetLayout);
}

void statusBarWidget::setStatusMessage(QString message)
{
    this->label->setText(message);
}

void statusBarWidget::setStatusdBugMessage(QString dBug)
{
    Preferences *preferences = Preferences::Instance(); // Load the preferences.
    if(preferences->getPreferences("Midi", "DBug", "bool")=="true")
    { this->dBuglabel->showMessage(dBug, 10000); } else {
      this->dBuglabel->showMessage(dBug, 3000); };
}

void statusBarWidget::setStatusProgress(int value)
{
    this->progressBar->setValue(value);
}

void statusBarWidget::setStatusSymbol(int value)
{
    this->symbol->setValue(value);
}


