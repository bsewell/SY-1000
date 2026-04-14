/****************************************************************************
**
** Copyright (C) 2026 Brian Sewell.
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

#include "midiMonitor.h"
#include "midiCCHandler.h"
#include <QTime>
#include <QScrollBar>

MidiMonitor::MidiMonitor(QWidget *parent)
    : QWidget(parent), messageCount(0)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(4, 2, 4, 2);
    mainLayout->setSpacing(2);

    // Top toolbar row
    QHBoxLayout *toolbar = new QHBoxLayout();
    toolbar->setSpacing(8);

    showCC = new QCheckBox("CC");
    showCC->setChecked(true);
    showCC->setStyleSheet("QCheckBox { color: #00FF88; font: bold 11px; }");

    showSysEx = new QCheckBox("SysEx");
    showSysEx->setChecked(false);
    showSysEx->setStyleSheet("QCheckBox { color: #88AAFF; font: bold 11px; }");

    clearBtn = new QPushButton("Clear");
    clearBtn->setFixedSize(50, 20);
    clearBtn->setStyleSheet(
        "QPushButton { background: #333; color: #CCC; border: 1px solid #555; "
        "border-radius: 3px; font: 10px; }"
        "QPushButton:hover { background: #444; }"
    );
    connect(clearBtn, &QPushButton::clicked, this, &MidiMonitor::clear);

    lastCC = new QLabel("Waiting for MIDI...");
    lastCC->setStyleSheet("QLabel { color: #FFAA00; font: bold 12px; }");

    toolbar->addWidget(showCC);
    toolbar->addWidget(showSysEx);
    toolbar->addWidget(clearBtn);
    toolbar->addSpacing(16);
    toolbar->addWidget(lastCC);
    toolbar->addStretch();

    mainLayout->addLayout(toolbar);

    // Log area
    logView = new QTextEdit(this);
    logView->setReadOnly(true);
    logView->setFont(QFont("Menlo", 11));
    logView->setStyleSheet(
        "QTextEdit { background-color: #1A1A2E; color: #CCCCCC; "
        "border: 1px solid #333; border-radius: 4px; padding: 4px; }"
    );
    logView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    logView->document()->setMaximumBlockCount(MAX_LINES);

    mainLayout->addWidget(logView);

    setStyleSheet("background-color: #111122;");
}

void MidiMonitor::logCC(int channel, int ccNumber, int value)
{
    if(!showCC->isChecked()) return;

    messageCount++;
    QString time = QTime::currentTime().toString("hh:mm:ss.zzz");

    // Build a visual bar for the value (0-127)
    int barLen = (value * 20) / 127;
    QString bar = QString(barLen, QChar(0x2588)) + QString(20 - barLen, QChar(0x2591));

    // Check if this CC is mapped
    MidiCCHandler *handler = MidiCCHandler::Instance();
    QString mappingInfo;
    QList<CCMapping> allMappings = handler->getMappings();
    for(const CCMapping &m : allMappings)
    {
        if(m.cc == ccNumber && m.channel == channel && m.enabled)
        {
            mappingInfo = QString("  -> %1").arg(m.label);
            break;
        }
    }

    QString line = QString("<span style='color:#666'>%1</span> "
                           "<span style='color:#00FF88'>CC</span> "
                           "<span style='color:#FFF'>ch:%2 cc:%3 val:%4</span> "
                           "<span style='color:#555'>%5</span>"
                           "<span style='color:#FFAA00'>%6</span>")
        .arg(time)
        .arg(channel, 2, 10, QChar('0'))
        .arg(ccNumber, 3, 10, QChar('0'))
        .arg(value, 3, 10, QChar('0'))
        .arg(bar)
        .arg(mappingInfo);

    logView->append(line);

    // Update the "last CC" summary label
    QString summary = QString("CC %1  ch:%2  val:%3%4")
        .arg(ccNumber).arg(channel).arg(value).arg(mappingInfo);
    lastCC->setText(summary);
}

void MidiMonitor::logSysEx(const QString &direction, const QString &hexData)
{
    if(!showSysEx->isChecked()) return;

    messageCount++;
    QString time = QTime::currentTime().toString("hh:mm:ss.zzz");

    // Truncate long SysEx for display
    QString display = hexData.left(80);
    if(hexData.length() > 80) display += "...";

    // Insert spaces every 2 chars for readability
    QString spaced;
    for(int i = 0; i < display.length(); i += 2)
    {
        if(i > 0) spaced += " ";
        spaced += display.mid(i, 2);
    }

    QString color = (direction == "TX") ? "#FF8888" : "#88AAFF";
    QString line = QString("<span style='color:#666'>%1</span> "
                           "<span style='color:%2'>%3</span> "
                           "<span style='color:#AAA'>%4</span>")
        .arg(time).arg(color).arg(direction).arg(spaced);

    logView->append(line);
}

void MidiMonitor::clear()
{
    logView->clear();
    messageCount = 0;
    lastCC->setText("Waiting for MIDI...");
}
