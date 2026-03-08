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

#ifndef TSL_GT1000_TRANSLATION_H
#define TSL_GT1000_TRANSLATION_H

#include <QString>
#include <QList>
//#include "SysxIO.h"
#include <QtWidgets>
#include <QJsonArray>
#include <QRegularExpression>
#include <QFile>
#include <QDataStream>
#include <QByteArray>
#include <QMessageBox>

class tsl_GT1000_translation : public QWidget
{
    Q_OBJECT

public:
    tsl_GT1000_translation(QByteArray patch, QByteArray default_hex, int mode, QString filename);
    ~tsl_GT1000_translation();
    QList<QString> patchList;
    int index;
    QByteArray convertFromTSL(int patch);
    QByteArray TSL_default;

public slots:
void patchIndex(int listIndex);

private: 
    QByteArray data;
    QByteArray SY1K_default;
    QByteArray SY1000_default;
    void SY1000_default_replace(int pos, int size, QByteArray value);
    void SY1000_assign(int pos, const char* assign);
    QString headerType;
    int ArrayToInt(QByteArray Array);
    QByteArray IntToArray(int value);
    QByteArray GetJsonArray(QByteArray text, int pos);
    QByteArray GetJsonValue(QByteArray text, int pos);
    QByteArray GetJsonHex(QByteArray text, int pos);
    QByteArray GetJsonString(QByteArray text, int pos);
    QByteArray convert_QByteArray_value(QByteArray value, QString hex3);
    QByteArray get_chain(int value);
    QByteArray convert_Assign(const char* Json_name);
    QByteArray convert_to_4Byte(const char* Json_name, int multiplier);
    void write_Assign(const char* ctrl, const char* num);
    void AppendTSL(QByteArray hex, const char* Json_name);
    void TextTSL(QByteArray hex, const char* Json_name);
    QString fileName;
    bool bass_mode;
    int a;
    int x;
    int incr = 0;
};

#endif // tsl_translation_H
