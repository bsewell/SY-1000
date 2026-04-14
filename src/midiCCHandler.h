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

#ifndef MIDICCHANDLER_H
#define MIDICCHANDLER_H

#include <QObject>
#include <QString>
#include <QJsonArray>
#include <QJsonObject>
#include <QMap>

struct CCMapping {
    int cc;
    int channel;
    QString hex0;
    QString hex1;
    QString hex2;
    QString hex3;
    QString label;
    bool enabled;
};

class MidiCCHandler : public QObject
{
    Q_OBJECT

public:
    static MidiCCHandler* Instance();

    bool loadMappings(const QString &filePath);
    bool saveMappings(const QString &filePath);
    void addMapping(const CCMapping &mapping);
    void removeMapping(int channel, int cc);
    QList<CCMapping> getMappings() const;
    void setLearnMode(bool active);
    bool isLearnMode() const;
    void setLearnTarget(const QString &hex0, const QString &hex1,
                        const QString &hex2, const QString &hex3);

    // QML-invokable API for CC mapping editor
    Q_INVOKABLE void startLearn(const QString &hex0, const QString &hex1,
                                const QString &hex2, const QString &hex3);
    Q_INVOKABLE void cancelLearn();
    Q_INVOKABLE bool learning() const;
    Q_INVOKABLE QVariantList mappingsModel() const;
    Q_INVOKABLE void removeMappingByIndex(int index);
    Q_INVOKABLE void setMappingEnabled(int channel, int cc, bool enabled);
    Q_INVOKABLE void saveUserMappings();
    Q_INVOKABLE int mappingCount() const;

public slots:
    void handleCC(int channel, int ccNumber, int value);

signals:
    void parameterMapped(int channel, int ccNumber, const QString &label);
    void learnComplete(int channel, int ccNumber, const QString &hex0,
                       const QString &hex1, const QString &hex2, const QString &hex3);
    void mappingsChanged();
    void learnStateChanged();

private:
    explicit MidiCCHandler(QObject *parent = nullptr);
    static MidiCCHandler *instance;

    // Key = (channel << 8) | cc
    QMap<int, CCMapping> mappings;
    bool learnActive;
    QString learnHex0, learnHex1, learnHex2, learnHex3;

    int makeKey(int channel, int cc) const;
    int scaleValue(int ccValue, int paramMin, int paramMax) const;
};

#endif // MIDICCHANDLER_H
