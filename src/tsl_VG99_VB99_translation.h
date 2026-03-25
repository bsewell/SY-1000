/****************************************************************************
**
** Copyright (C) 2020 Colin Willcocks. 
** All rights reserved.
**
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

#ifndef TSL_VG99_VB99_TRANSLATION_H
#define TSL_VG99_VB99_TRANSLATION_H

#include <QString>
#include <QList>
#include <QtWidgets>
#include <QJsonArray>
#include <QRegularExpression>
#include <QFile>
#include <QDataStream>
#include <QByteArray>
#include <QMessageBox>
#include <qdom.h>

struct VG99_Midi
{
    QString value, name, abbr, desc, customdesc;
    QList<QString> type, id;
    QList<VG99_Midi> level;
};

class tsl_VG99_VB99_translation : public QWidget
{
    Q_OBJECT

public:
    tsl_VG99_VB99_translation(QByteArray patch, QByteArray default_hex, int mode, QString filename);
    ~tsl_VG99_VB99_translation();
    QList<QString> patchList;
    int index;
    QByteArray convertFromVG99default_to_SY1000default(int patch);
    QByteArray TSL_default;

public slots:
void patchIndex(int listIndex);

private: 
    QByteArray data;
    QByteArray SY1K_default;
    QByteArray SY1000_default;
    QByteArray VG99_default;
    void SY1000_default_replace(QString addr1, QString addr2, QString addr3, int size, QByteArray value);
    void VG99_default_replace(QString addr2, QString addr3, int size, QByteArray value);
    void SY1000_assign(QByteArray assign);
    QString headerType;
    int ArrayToInt(QByteArray Array);
    QByteArray IntToArray(int value);
    QByteArray convertVG99_value(QByteArray value, QString hex2, QString hex3);
    QByteArray convertVB99_value(QByteArray value, QString hex2, QString hex3);
    QByteArray get_chain(int value);
    QByteArray convert_Assign(const char* Json_name);
    QByteArray convert_to_4Byte(QByteArray value, double multiplier);
    void write_Assign(const char* ctrl, const char* num);
    void AppendTSL(QByteArray hex, const char* Json_name);
    void TextTSL(QByteArray hex, const char* Json_name);
    QString fileName;
    bool bass_mode;
    int a;
    int x;
    int incr = 0;
    void convert_VG99mid_to_VG99default(int patch);
    QByteArray copyVG99(QString addr1, QString addr2, int size);
    QByteArray copySY1000(QString addr1, QString addr2, QString addr3, int size);
    QByteArray scaleVG99_value(QByteArray val, QString maxVal, QString min, QString max);
    QByteArray convert_altVB99mid_to_default(int);
    QByteArray convert_VB99mid_to_VG99default(int);
    QByteArray convert_VB99_Assign(QByteArray target);
    QByteArray copyVBmid(QString addr1, QString addr2, int size);
    void loadMidiMap();
    QDomElement root;
    VG99_Midi midiMap;
    VG99_Midi getMidiMap(QString root, QString hex1, QString hex2, QString hex3);
    VG99_Midi getMidiMap(QString root, QString hex1, QString hex2, QString hex3, QString hex4);
    int getRange(QString root, QString hex1, QString hex2, QString hex3);
    int getRangeMinimum(QString root, QString hex1, QString hex2, QString hex3);
    QString getRoot(QString root);
    uint assign_number;
    void convert_VG99g9l_to_VG99default();
};

#endif // tsl_translation_H
