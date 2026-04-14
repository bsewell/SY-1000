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

#include "midiCCHandler.h"
#include "parameterBridge.h"
#include "MidiTable.h"
#include "SysxIO.h"
#include <QFile>
#include <QDir>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>
#include <QVariantList>
#include <QVariantMap>

MidiCCHandler* MidiCCHandler::instance = nullptr;

MidiCCHandler::MidiCCHandler(QObject *parent)
    : QObject(parent), learnActive(false)
{
}

MidiCCHandler* MidiCCHandler::Instance()
{
    if(!instance)
    {
        instance = new MidiCCHandler();
    }
    return instance;
}

int MidiCCHandler::makeKey(int channel, int cc) const
{
    return (channel << 8) | cc;
}

int MidiCCHandler::scaleValue(int ccValue, int paramMin, int paramMax) const
{
    // Scale 0-127 CC range to parameter's actual min/max range
    if(paramMax <= paramMin) return paramMin;
    double ratio = static_cast<double>(ccValue) / 127.0;
    int scaled = paramMin + static_cast<int>(ratio * (paramMax - paramMin) + 0.5);
    if(scaled < paramMin) scaled = paramMin;
    if(scaled > paramMax) scaled = paramMax;
    return scaled;
}

bool MidiCCHandler::loadMappings(const QString &filePath)
{
    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly))
    {
        qWarning() << "MidiCCHandler: could not open" << filePath;
        return false;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    if(!doc.isObject()) return false;

    QJsonObject root = doc.object();
    QJsonArray arr = root["mappings"].toArray();

    mappings.clear();
    for(int i = 0; i < arr.size(); ++i)
    {
        QJsonObject obj = arr.at(i).toObject();
        CCMapping m;
        m.cc      = obj["cc"].toInt();
        m.channel = obj["channel"].toInt(0);
        m.enabled = obj["enabled"].toBool(true);
        m.label   = obj["label"].toString();

        QJsonArray hexArr = obj["hex"].toArray();
        if(hexArr.size() >= 4)
        {
            m.hex0 = hexArr.at(0).toString();
            m.hex1 = hexArr.at(1).toString();
            m.hex2 = hexArr.at(2).toString();
            m.hex3 = hexArr.at(3).toString();
        }
        else
        {
            continue;
        }

        int key = makeKey(m.channel, m.cc);
        mappings.insert(key, m);
    }

    qDebug() << "MidiCCHandler: loaded" << mappings.size() << "mappings from" << filePath;
    return true;
}

bool MidiCCHandler::saveMappings(const QString &filePath)
{
    QJsonArray arr;
    for(auto it = mappings.constBegin(); it != mappings.constEnd(); ++it)
    {
        const CCMapping &m = it.value();
        QJsonObject obj;
        obj["cc"]      = m.cc;
        obj["channel"] = m.channel;
        obj["label"]   = m.label;
        obj["enabled"] = m.enabled;
        obj["hex"]     = QJsonArray({ m.hex0, m.hex1, m.hex2, m.hex3 });
        arr.append(obj);
    }

    QJsonObject root;
    root["version"]     = 1;
    root["description"] = "MIDI CC to SY-1000 SysEx parameter mappings.";
    root["mappings"]    = arr;
    root["learn_slots"] = QJsonArray();

    QFile file(filePath);
    if(!file.open(QIODevice::WriteOnly))
    {
        qWarning() << "MidiCCHandler: could not write" << filePath;
        return false;
    }

    file.write(QJsonDocument(root).toJson(QJsonDocument::Indented));
    file.close();
    return true;
}

void MidiCCHandler::addMapping(const CCMapping &mapping)
{
    int key = makeKey(mapping.channel, mapping.cc);
    mappings.insert(key, mapping);
}

void MidiCCHandler::removeMapping(int channel, int cc)
{
    mappings.remove(makeKey(channel, cc));
}

QList<CCMapping> MidiCCHandler::getMappings() const
{
    return mappings.values();
}

void MidiCCHandler::setLearnMode(bool active)
{
    learnActive = active;
    if(!active)
    {
        learnHex0.clear();
        learnHex1.clear();
        learnHex2.clear();
        learnHex3.clear();
    }
}

bool MidiCCHandler::isLearnMode() const
{
    return learnActive;
}

void MidiCCHandler::setLearnTarget(const QString &hex0, const QString &hex1,
                                    const QString &hex2, const QString &hex3)
{
    learnHex0 = hex0;
    learnHex1 = hex1;
    learnHex2 = hex2;
    learnHex3 = hex3;
}

void MidiCCHandler::handleCC(int channel, int ccNumber, int value)
{
    // Learn mode: first CC received gets mapped to the target parameter
    if(learnActive && !learnHex0.isEmpty())
    {
        CCMapping m;
        m.cc      = ccNumber;
        m.channel = channel;
        m.hex0    = learnHex0;
        m.hex1    = learnHex1;
        m.hex2    = learnHex2;
        m.hex3    = learnHex3;
        m.enabled = true;

        // Build label from MidiTable
        MidiTable *midiTable = MidiTable::Instance();
        Midi items = midiTable->getMidiMap(m.hex0, m.hex1, m.hex2, m.hex3);
        m.label = items.customdesc.isEmpty() ? items.desc : items.customdesc;

        addMapping(m);

        emit learnComplete(channel, ccNumber, learnHex0, learnHex1, learnHex2, learnHex3);
        setLearnMode(false);
        emit learnStateChanged();
        emit mappingsChanged();

        qDebug() << "MidiCCHandler: learned CC" << ccNumber << "ch" << channel << "->" << m.label;
        return;
    }

    // Normal mode: look up mapping and inject into parameter bridge
    int key = makeKey(channel, ccNumber);
    if(!mappings.contains(key)) return;

    const CCMapping &m = mappings[key];
    if(!m.enabled) return;

    // Get parameter range from MidiTable
    MidiTable *midiTable = MidiTable::Instance();
    int paramMin = midiTable->getRangeMinimum(m.hex0, m.hex1, m.hex2, m.hex3);
    int paramMax = midiTable->getRange(m.hex0, m.hex1, m.hex2, m.hex3);

    // Scale CC 0-127 to parameter range
    int scaled = scaleValue(value, paramMin, paramMax);

    // Inject through ParameterBridge — this updates SysxIO + sends SysEx + emits parameterChanged
    ParameterBridge *bridge = ParameterBridge::Instance();
    bridge->setValue(m.hex0, m.hex1, m.hex2, m.hex3, scaled);

    SysxIO *sysxIO = SysxIO::Instance();
    sysxIO->deBug("CC" + QString::number(ccNumber) + " ch" + QString::number(channel) +
                  " val=" + QString::number(value) + " -> " + m.label +
                  " scaled=" + QString::number(scaled));
}

// ── QML-invokable API ──────────────────────────────────────────────────

void MidiCCHandler::startLearn(const QString &hex0, const QString &hex1,
                                const QString &hex2, const QString &hex3)
{
    setLearnTarget(hex0, hex1, hex2, hex3);
    setLearnMode(true);
    emit learnStateChanged();
    qDebug() << "MidiCCHandler: learn started for" << hex0 << hex1 << hex2 << hex3;
}

void MidiCCHandler::cancelLearn()
{
    setLearnMode(false);
    emit learnStateChanged();
}

bool MidiCCHandler::learning() const
{
    return learnActive;
}

QVariantList MidiCCHandler::mappingsModel() const
{
    QVariantList list;
    for(auto it = mappings.constBegin(); it != mappings.constEnd(); ++it)
    {
        const CCMapping &m = it.value();
        QVariantMap map;
        map["cc"]      = m.cc;
        map["channel"] = m.channel;
        map["hex0"]    = m.hex0;
        map["hex1"]    = m.hex1;
        map["hex2"]    = m.hex2;
        map["hex3"]    = m.hex3;
        map["label"]   = m.label;
        map["enabled"] = m.enabled;
        list.append(map);
    }
    return list;
}

void MidiCCHandler::removeMappingByIndex(int index)
{
    QList<int> keys = mappings.keys();
    if(index >= 0 && index < keys.size())
    {
        mappings.remove(keys.at(index));
        emit mappingsChanged();
    }
}

void MidiCCHandler::setMappingEnabled(int channel, int cc, bool enabled)
{
    int key = makeKey(channel, cc);
    if(mappings.contains(key))
    {
        mappings[key].enabled = enabled;
        emit mappingsChanged();
    }
}

void MidiCCHandler::saveUserMappings()
{
    QString ccPath = QDir::homePath() + "/.sy1000floorboard/cc_mappings.json";
    saveMappings(ccPath);
    qDebug() << "MidiCCHandler: user mappings saved to" << ccPath;
}

int MidiCCHandler::mappingCount() const
{
    return mappings.size();
}
