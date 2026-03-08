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

#ifndef SYSXWRITER_H
#define SYSXWRITER_H

#include <QString>
#include <QList>
#include "SysxIO.h"	
#include "tsl_SY300_translation.h"
#include "tsl_GP10_translation.h"
#include "tsl_GT1000_translation.h"
#include "tsl_VG99_VB99_translation.h"

class sysxWriter
{

public:
    sysxWriter();
    ~sysxWriter();
    void setFile(QString fileName);
    bool readFile();
    SysxData getFileSource();
    SysxData getSystemSource();
    void writeFile(QString fileName);
    void writeSystemFile(QString fileName);
    QString getFileName();
    QList<QString> patchList;
    int index;
    void convertTSL_to_default_G(int patch);
    void TSL_G_to_hex_G();
    void TSL_B_to_hex_G();
    void convertTSL_to_default_B(int patch);
    void TSL_G_to_hex_B();
    void TSL_B_to_hex_B();
    QByteArray TSL_default;
    void writeTSL_G(QString fileName);
    void writeTSL_B(QString fileName);
    void hex_G_to_TSL_G();
    void hex_G_to_TSL_B();
    void hex_B_to_TSL_G();
    void hex_B_to_TSL_B();
    void convertSY300tsl_to_default(int patch);
    void convert_GP10_TSL_to_default(int patch);
    void convert_GT1000_TSL_to_default(int patch);
    void convert_VG99_VB99_to_default(int patch);
    QByteArray SY1000_default;
    tsl_translation *SY300translate;
    tsl_GP10_translation *GP10translate;
    tsl_GT1000_translation *GT1000translate;
    tsl_VG99_VB99_translation *VG99_VB99translate;
    QByteArray convert_Assign(QByteArray value, int modeInt);

public slots:
    void patchIndex(int listIndex);

private:
    QString fileName;
    QByteArray data;
    bool SY1000;
    int a;
    void AppendTSL(QByteArray hex, const char* Json_name);
    void TextTSL(QByteArray hex, const char* Json_name);
    QByteArray GetJsonString(QByteArray text, int pos);
    QByteArray GetJsonArray(QByteArray text, int pos);
    QByteArray GetJsonValue(QByteArray text, int pos);
    void SY1000_default_replace(int pos, int size, QByteArray value);
    void fileLoadError();
    QByteArray convert_QByteArray_value(QByteArray value, QString hex2,QString hex3);
    void SY1000_assign(int pos, const char* assign);
    QByteArray temp;
    void hex_G_to_hex_B();
    QByteArray convert_value(QByteArray value, QString hex2, QString hex3);
    QByteArray altTuning(QString mode);
};

#endif // SYSXWRITER_H
