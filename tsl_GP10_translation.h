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

#ifndef TSL_GP10_TRANSLATION_H
#define TSL_GP10_TRANSLATION_H

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

class tsl_GP10_translation : public QWidget
{
    Q_OBJECT

public:
    tsl_GP10_translation(QByteArray patch, QByteArray default_hex, int mode, QString filename);
    ~tsl_GP10_translation();
    QList<QString> patchList;
    uint index;
    QByteArray convertFromTSL(int patch);
    QByteArray TSL_default;

public slots:
void patchIndex(int listIndex);

private: 
    QByteArray data;
    QString headerType;
    int ArrayToInt(QByteArray Array);
    QByteArray GetJsonArray(QByteArray text, int pos);
    QByteArray GetJsonValue(QByteArray text, int pos);
    QByteArray GetJsonHex(QByteArray text, int pos);
    QByteArray convert_Assign(QByteArray target);
    QString fileName;
    int a;
    int x;
    int incr = 0;
    QByteArray SY1000_default;
    void SY1000_default_replace(QString addr1, QString addr2, QString addr3, int size, QByteArray value);
    QByteArray convert_to_2Byte(QByteArray value, double multiplier);
    QByteArray convert_to_4Byte(QByteArray value, double multiplier);
    QByteArray convert_GP10_value(QByteArray value, QString hex2, QString hex3);
    QByteArray convert_MinMax(QByteArray value);
};

#endif // tsl_GP10_translation_H
