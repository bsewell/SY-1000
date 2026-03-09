/****************************************************************************
**
** Copyright (C) 2007~2025 Colin Willcocks.
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
#include <QFile>
#include <QDataStream>
#include <QByteArray>
#include <QMessageBox>
#include "tsl_GP10_translation.h"
#include "fileDialog.h"
#include "globalVariables.h"
#include "MidiTable.h"
#include <QJsonArray>
#include <QRegularExpression>

#include <QDebug>

tsl_GP10_translation::tsl_GP10_translation(QByteArray patch, QByteArray default_hex, int mode, QString filename)
{
    Q_UNUSED(mode);
    this->data = patch;
    this->SY1000_default = default_hex;
    this->fileName = filename;
}

tsl_GP10_translation::~tsl_GP10_translation()
{
    this->deleteLater();
}

void tsl_GP10_translation::patchIndex(int listIndex)
{
    Q_UNUSED(listIndex);
    SysxIO *sysxIO = SysxIO::Instance();
    this->index=sysxIO->patchListValue;
}

int tsl_GP10_translation::ArrayToInt(QByteArray Array)
{
    bool ok;
    char aw = Array.at(0);
    QString val = QString::number(aw, 16).toUpper();
    int msb = val.toInt(&ok, 16);
    aw = Array.at(1);
    val = QString::number(aw, 16).toUpper();
    int lsb = val.toInt(&ok, 16);
    int value = (msb*128) + lsb;
    return value;
}

QByteArray tsl_GP10_translation::GetJsonArray(QByteArray text, int pos )
{
    QByteArray null("ul");                                                      // when read later on without "" null = ul
    QByteArray str((char(34)+text+char(34)));
    int start_index = data.indexOf(str, pos)+(text.size()+4);                    //find pointer to start of Json value.
    int end_index = data.indexOf(",", start_index)-start_index-1;                  //find pointer to end of value to get the size of the value.
    QByteArray xq = data.mid(start_index , end_index );                           //copy the Json value and return as a QByteArray.
    if(data.mid(start_index, 4).contains(null)) {xq.clear(); };
    return xq;
}

QByteArray tsl_GP10_translation::GetJsonValue(QByteArray text, int pos )
{
    QByteArray str((char(34)+text+char(34)));
    int start_index = data.indexOf(str, pos)+(text.size()+3); //find pointer to start of Json value.
    int end_index = data.indexOf(",", start_index)-start_index;                  //find pointer to end of value to get the size of the value.
    QByteArray xq = data.mid(start_index , end_index );                 //copy the Json value and return as a QByteArray.
    return xq;
}

QByteArray tsl_GP10_translation::GetJsonHex(QByteArray text, int pos )
{
    QByteArray str((char(34)+text+char(34)));
    int start_index = data.indexOf(str, pos)+(text.size()+3); //find pointer to start of Json value.
    bool ok;
    int end_index = data.indexOf(",", start_index)-start_index;                  //find pointer to end of value to get the size of the value.
    QByteArray xq(data.mid(start_index , end_index ));
    if(xq.at(0)==(char)34) {xq.remove(0, 1); };
    if(xq.contains((char)34)) {xq.truncate(xq.indexOf((char)34)); };
    int value =  xq.toInt(&ok, 10);
    QByteArray Array;
    Array.append(char(value));
    return Array;                                    // return one byte QByteArray
}

QByteArray tsl_GP10_translation::convert_Assign(QByteArray target)
{
    QString recon;
    recon = target.toHex().toUpper().mid(1,1).append(target.toHex().toUpper().mid(3,1)).append(target.toHex().toUpper().mid(5,1));
    MidiTable *midiTable = MidiTable::Instance();
    bool ok;
    int assign = recon.toInt(&ok, 16);

    //QString assign_before(QString::number(assign, 10));
    int maxRange = 256;
    int dif = assign/maxRange;
    QString valueHex1 = QString::number(dif, 16).toUpper();
    if(valueHex1.length() < 2) valueHex1.prepend("0");
    QString valueHex2 = QString::number(assign - (dif * maxRange), 16).toUpper();
    if(valueHex2.length() < 2) valueHex2.prepend("0");
    QString hex4 = valueHex1;
    QString hex5 = valueHex2;                                     //convert valueStr to 7bit hex4, hex5
    Midi items = midiTable->getMidiMap("Tables", "00", "02", "2A", hex4, hex5);
    QString assign_after;
    assign_after = items.desc;
    assign_after.truncate(assign_after.indexOf(" "));
    assign = assign_after.toInt(&ok, 10);

    QString index = QString::number(assign, 16).toUpper();
    if(index.size()<2){index.prepend("000");};
    if(index.size()<3){index.prepend("00");};
    if(index.size()<4){index.prepend("0");};

    QString n3(index.at(3)); int b3 = n3.toInt(&ok, 16);
    QString n2(index.at(2)); int b2 = n2.toInt(&ok, 16);
    QString n1(index.at(1)); int b1 = n1.toInt(&ok, 16);
    QString n0(index.at(0)); int b0 = n0.toInt(&ok, 16);

    QByteArray new_value;
    new_value.append(char(b0)).append(char(b1)).append(char(b2)).append(char(b3));
    //qDebug() << "Assign before = " << assign_before << " Assign after = " << assign_after << " Assign actual = " << index;
    return new_value;
}

QByteArray tsl_GP10_translation::convertFromTSL(int patch)
{
    SysxIO *sysxIO = SysxIO::Instance();
    QByteArray temp;
    int pnum = data.count("params"); // count the number of times 'params' appears to get patch count
    int pindex = data.size()/(pnum);
    QString device=GetJsonArray("device", 1);

    int patchCount = pnum;
    if (patchCount>1)
    {
        QString msgText;
        QString patchText;
        QString patchNumber;
        this->patchList.clear();
        this->patchList.append(QObject::tr("Select Patch"));
        int a = 74; // locate patch text start position from the start of the file

        for (int h=0;h<patchCount;h++)
        {
            patchText.append(GetJsonValue("patchname", a).trimmed().remove(0, 1) );
            patchText.chop(1);
            patchNumber = QString::number(h+1, 10).toUpper();
            msgText.append(patchNumber + " : ");
            msgText.append(patchText + "   ");
            this->patchList.append(msgText);
            patchText.clear();
            msgText.clear();
            a=a+pindex;                      // offset is set in front of marker
        };

        SysxIO *sysxIO = SysxIO::Instance();
        if(patch<1){
            fileDialog *dialog = new fileDialog(this->fileName, patchList);
            dialog->exec();
            dialog->deleteLater();
            patchIndex(this->index);
            if(sysxIO->patchListValue == 0)
            {
                return(0);
            };
        }
        else
        {
            if(patch>patchCount)
            {
                patch=patchCount;
            };
            sysxIO->patchListValue = patch;
            this->index = sysxIO->patchListValue;
        };
    };

    a=0;
    if (patchCount>1)
    {
        int q=index-1;      // find start of required patch
        a = q*pindex;
    };

    if(device.contains("GP-10"))
    {
        Q_UNUSED(patch);
        bool ok;

        QFile dfile(":/default_guitar.syx");
        if (dfile.open(QIODevice::ReadOnly))
        {
            this->SY1000_default = dfile.readAll();
            dfile.close();
        };

        temp.append((char)GetJsonValue("patchName1", a).toInt() );  //copy patch name
        temp.append((char)GetJsonValue("patchName2", a).toInt() );  //copy patch name
        temp.append((char)GetJsonValue("patchName3", a).toInt() );  //copy patch name
        temp.append((char)GetJsonValue("patchName4", a).toInt() );  //copy patch name
        temp.append((char)GetJsonValue("patchName5", a).toInt() );  //copy patch name
        temp.append((char)GetJsonValue("patchName6", a).toInt() );  //copy patch name
        temp.append((char)GetJsonValue("patchName7", a).toInt() );  //copy patch name
        temp.append((char)GetJsonValue("patchName8", a).toInt() );  //copy patch name
        temp.append((char)GetJsonValue("patchName9", a).toInt() );  //copy patch name
        temp.append((char)GetJsonValue("patchName10", a).toInt() ); //copy patch name
        temp.append((char)GetJsonValue("patchName11", a).toInt() ); //copy patch name
        temp.append((char)GetJsonValue("patchName12", a).toInt() ); //copy patch name
        temp.append(QByteArray::fromHex("20202020"));
        SY1000_default_replace("00", "00", "00", 16, temp );        // patch name
        SY1000_default_replace("00", "12", "3C", 2, convert_to_2Byte(GetJsonHex("patchLevel", a), 2));
        QString recon = GetJsonHex("patchTempo_l", a).toHex().toUpper() + GetJsonHex("patchTempo_h", a).toHex().toUpper();
        int bpm = (recon.mid(1,1) + recon.mid(3,1)).toInt(&ok, 16)*10;
        recon = QString::number(bpm, 16);
        temp = QByteArray::fromHex(recon.toLatin1());
        SY1000_default_replace("00", "12", "3E", 4, convert_to_4Byte(temp, 1));  // master tempo
        SY1000_default_replace("00", "02", "00", 1, convert_GP10_value(GetJsonHex("patchGkSet", a), "02", "16"));
        SY1000_default_replace("00", "12", "01", 1, GetJsonHex("normlpuOnOff", a));
        SY1000_default_replace("00", "12", "02", 2, convert_to_2Byte(GetJsonHex("normlpuVolume", a),1));
        SY1000_default_replace("00", "12", "04", 1, GetJsonHex("normlpuCableSim", a));
        //SY1000_default_replace("00", "00", "00", 1, GetJsonHex("mixerMdlInLv", a));
        //SY1000_default_replace("00", "00", "00", 1, GetJsonHex("mixerNpuInLv", a));
        SY1000_default_replace("00", "12", "2B", 1, GetJsonHex("mixerBalance", a));  // balancer 3
        SY1000_default_replace("00", "15", "00", 1, GetJsonHex("mdlOnOff", a));  // inst 1
        SY1000_default_replace("00", "20", "00", 1, GetJsonHex("mdlOnOff", a));  // inst 2
        SY1000_default_replace("00", "2B", "00", 1, GetJsonHex("mdlOnOff", a));  // inst 3
        SY1000_default_replace("00", "15", "01", 1, convert_GP10_value(GetJsonHex("mdlType", a), "02", "17")); // inst 1
        SY1000_default_replace("00", "20", "01", 1, convert_GP10_value(GetJsonHex("mdlType", a), "02", "17")); // inst 2
        SY1000_default_replace("00", "2B", "01", 1, convert_GP10_value(GetJsonHex("mdlType", a), "02", "17")); // inst 3
        SY1000_default_replace("00", "1A", "1E", 1, GetJsonHex("mdlNsNsOnOff", a));  // inst 1
        SY1000_default_replace("00", "1B", "20", 1, GetJsonHex("mdlNsNsOnOff", a));  // inst 1
        SY1000_default_replace("00", "1C", "1F", 1, GetJsonHex("mdlNsNsOnOff", a));  // inst 1
        SY1000_default_replace("00", "1D", "16", 1, GetJsonHex("mdlNsNsOnOff", a));  // inst 1
        SY1000_default_replace("00", "1E", "10", 1, GetJsonHex("mdlNsNsOnOff", a));  // inst 1
        SY1000_default_replace("00", "25", "1E", 1, GetJsonHex("mdlNsNsOnOff", a));  // inst 2
        SY1000_default_replace("00", "26", "20", 1, GetJsonHex("mdlNsNsOnOff", a));  // inst 2
        SY1000_default_replace("00", "27", "1F", 1, GetJsonHex("mdlNsNsOnOff", a));  // inst 2
        SY1000_default_replace("00", "28", "16", 1, GetJsonHex("mdlNsNsOnOff", a));  // inst 2
        SY1000_default_replace("00", "29", "10", 1, GetJsonHex("mdlNsNsOnOff", a));  // inst 2
        SY1000_default_replace("00", "30", "1E", 1, GetJsonHex("mdlNsNsOnOff", a));  // inst 3
        SY1000_default_replace("00", "31", "20", 1, GetJsonHex("mdlNsNsOnOff", a));  // inst 3
        SY1000_default_replace("00", "32", "1F", 1, GetJsonHex("mdlNsNsOnOff", a));  // inst 3
        SY1000_default_replace("00", "33", "16", 1, GetJsonHex("mdlNsNsOnOff", a));  // inst 3
        SY1000_default_replace("00", "34", "10", 1, GetJsonHex("mdlNsNsOnOff", a));  // inst 3
        SY1000_default_replace("00", "1A", "1F", 1, GetJsonHex("mdlNsThreshld", a));  // inst 1
        SY1000_default_replace("00", "1B", "21", 1, GetJsonHex("mdlNsThreshld", a));  // inst 1
        SY1000_default_replace("00", "1C", "20", 1, GetJsonHex("mdlNsThreshld", a));  // inst 1
        SY1000_default_replace("00", "1D", "17", 1, GetJsonHex("mdlNsThreshld", a));  // inst 1
        SY1000_default_replace("00", "1E", "11", 1, GetJsonHex("mdlNsThreshld", a));  // inst 1
        SY1000_default_replace("00", "25", "1F", 1, GetJsonHex("mdlNsThreshld", a));  // inst 2
        SY1000_default_replace("00", "26", "21", 1, GetJsonHex("mdlNsThreshld", a));  // inst 2
        SY1000_default_replace("00", "27", "20", 1, GetJsonHex("mdlNsThreshld", a));  // inst 2
        SY1000_default_replace("00", "28", "17", 1, GetJsonHex("mdlNsThreshld", a));  // inst 2
        SY1000_default_replace("00", "29", "11", 1, GetJsonHex("mdlNsThreshld", a));  // inst 2
        SY1000_default_replace("00", "30", "1F", 1, GetJsonHex("mdlNsThreshld", a));  // inst 3
        SY1000_default_replace("00", "31", "21", 1, GetJsonHex("mdlNsThreshld", a));  // inst 3
        SY1000_default_replace("00", "32", "20", 1, GetJsonHex("mdlNsThreshld", a));  // inst 3
        SY1000_default_replace("00", "33", "17", 1, GetJsonHex("mdlNsThreshld", a));  // inst 3
        SY1000_default_replace("00", "34", "11", 1, GetJsonHex("mdlNsThreshld", a));  // inst 3
        SY1000_default_replace("00", "1A", "20", 1, GetJsonHex("mdlNsRelease", a));  // inst 1
        SY1000_default_replace("00", "1B", "22", 1, GetJsonHex("mdlNsRelease", a));  // inst 1
        SY1000_default_replace("00", "1C", "21", 1, GetJsonHex("mdlNsRelease", a));  // inst 1
        SY1000_default_replace("00", "1D", "18", 1, GetJsonHex("mdlNsRelease", a));  // inst 1
        SY1000_default_replace("00", "1E", "12", 1, GetJsonHex("mdlNsRelease", a));  // inst 1
        SY1000_default_replace("00", "25", "20", 1, GetJsonHex("mdlNsRelease", a));  // inst 2
        SY1000_default_replace("00", "26", "22", 1, GetJsonHex("mdlNsRelease", a));  // inst 2
        SY1000_default_replace("00", "27", "21", 1, GetJsonHex("mdlNsRelease", a));  // inst 2
        SY1000_default_replace("00", "28", "18", 1, GetJsonHex("mdlNsRelease", a));  // inst 2
        SY1000_default_replace("00", "29", "12", 1, GetJsonHex("mdlNsRelease", a));  // inst 2
        SY1000_default_replace("00", "30", "20", 1, GetJsonHex("mdlNsRelease", a));  // inst 3
        SY1000_default_replace("00", "31", "22", 1, GetJsonHex("mdlNsRelease", a));  // inst 3
        SY1000_default_replace("00", "32", "21", 1, GetJsonHex("mdlNsRelease", a));  // inst 3
        SY1000_default_replace("00", "33", "18", 1, GetJsonHex("mdlNsRelease", a));  // inst 3
        SY1000_default_replace("00", "34", "12", 1, GetJsonHex("mdlNsRelease", a));  // inst 3
        SY1000_default_replace("00", "15", "06", 1, GetJsonHex("mdlStringLv1", a));  // inst 1
        SY1000_default_replace("00", "15", "07", 1, GetJsonHex("mdlStringLv2", a));  // inst 1
        SY1000_default_replace("00", "15", "08", 1, GetJsonHex("mdlStringLv3", a));  // inst 1
        SY1000_default_replace("00", "15", "09", 1, GetJsonHex("mdlStringLv4", a));  // inst 1
        SY1000_default_replace("00", "15", "0A", 1, GetJsonHex("mdlStringLv5", a));  // inst 1
        SY1000_default_replace("00", "15", "0B", 1, GetJsonHex("mdlStringLv6", a));  // inst 1
        SY1000_default_replace("00", "15", "0C", 1, GetJsonHex("mdlStringPan1", a)); // inst 1
        SY1000_default_replace("00", "15", "0D", 1, GetJsonHex("mdlStringPan2", a)); // inst 1
        SY1000_default_replace("00", "15", "0E", 1, GetJsonHex("mdlStringPan3", a)); // inst 1
        SY1000_default_replace("00", "15", "0F", 1, GetJsonHex("mdlStringPan4", a)); // inst 1
        SY1000_default_replace("00", "15", "10", 1, GetJsonHex("mdlStringPan5", a)); // inst 1
        SY1000_default_replace("00", "15", "11", 1, GetJsonHex("mdlStringPan6", a)); // inst 1
        SY1000_default_replace("00", "20", "06", 1, GetJsonHex("mdlStringLv1", a));  // inst 2
        SY1000_default_replace("00", "20", "07", 1, GetJsonHex("mdlStringLv2", a));  // inst 2
        SY1000_default_replace("00", "20", "08", 1, GetJsonHex("mdlStringLv3", a));  // inst 2
        SY1000_default_replace("00", "20", "09", 1, GetJsonHex("mdlStringLv4", a));  // inst 2
        SY1000_default_replace("00", "20", "0A", 1, GetJsonHex("mdlStringLv5", a));  // inst 2
        SY1000_default_replace("00", "20", "0B", 1, GetJsonHex("mdlStringLv6", a));  // inst 2
        SY1000_default_replace("00", "20", "0C", 1, GetJsonHex("mdlStringPan1", a)); // inst 2
        SY1000_default_replace("00", "20", "0D", 1, GetJsonHex("mdlStringPan2", a)); // inst 2
        SY1000_default_replace("00", "20", "0E", 1, GetJsonHex("mdlStringPan3", a)); // inst 2
        SY1000_default_replace("00", "20", "0F", 1, GetJsonHex("mdlStringPan4", a)); // inst 2
        SY1000_default_replace("00", "20", "10", 1, GetJsonHex("mdlStringPan5", a)); // inst 2
        SY1000_default_replace("00", "20", "11", 1, GetJsonHex("mdlStringPan6", a)); // inst 2
        SY1000_default_replace("00", "2B", "06", 1, GetJsonHex("mdlStringLv1", a));  // inst 3
        SY1000_default_replace("00", "2B", "07", 1, GetJsonHex("mdlStringLv2", a));  // inst 3
        SY1000_default_replace("00", "2B", "08", 1, GetJsonHex("mdlStringLv3", a));  // inst 3
        SY1000_default_replace("00", "2B", "09", 1, GetJsonHex("mdlStringLv4", a));  // inst 3
        SY1000_default_replace("00", "2B", "0A", 1, GetJsonHex("mdlStringLv5", a));  // inst 3
        SY1000_default_replace("00", "2B", "0B", 1, GetJsonHex("mdlStringLv6", a));  // inst 3
        SY1000_default_replace("00", "2B", "0C", 1, GetJsonHex("mdlStringPan1", a)); // inst 3
        SY1000_default_replace("00", "2B", "0D", 1, GetJsonHex("mdlStringPan2", a)); // inst 3
        SY1000_default_replace("00", "2B", "0E", 1, GetJsonHex("mdlStringPan3", a)); // inst 3
        SY1000_default_replace("00", "2B", "0F", 1, GetJsonHex("mdlStringPan4", a)); // inst 3
        SY1000_default_replace("00", "2B", "10", 1, GetJsonHex("mdlStringPan5", a)); // inst 3
        SY1000_default_replace("00", "2B", "11", 1, GetJsonHex("mdlStringPan6", a)); // inst 3

        SY1000_default_replace("00", "1A", "00", 1, GetJsonHex("egType", a));        // inst 1
        SY1000_default_replace("00", "1A", "08", 1, GetJsonHex("egVolume", a));      // inst 1
        SY1000_default_replace("00", "1A", "09", 1, GetJsonHex("egTone", a));        // inst 1
        SY1000_default_replace("00", "1A", "02", 1, GetJsonHex("egFretlessToneType", a));     // inst 1
        SY1000_default_replace("00", "1A", "03", 1, GetJsonHex("egFretlessSens", a));         // inst 1
        SY1000_default_replace("00", "1A", "04", 1, GetJsonHex("egFretlessDepth", a));        // inst 1
        SY1000_default_replace("00", "1A", "05", 1, GetJsonHex("egFretlessAttack", a));       // inst 1
        SY1000_default_replace("00", "1A", "06", 1, GetJsonHex("egFretlessResonance", a));    // inst 1
        SY1000_default_replace("00", "1A", "07", 1, GetJsonHex("egFretlessDirectLevel", a));  // inst 1
        if(GetJsonHex("egType", a).toHex().toUpper() == "00")
        {
            SY1000_default_replace("00", "1A", "01", 1, GetJsonHex("egClaStPuSelect", a));    // inst 1
        }
        else if(GetJsonHex("egType", a).toHex().toUpper() == "01")
        {
            SY1000_default_replace("00", "1A", "01", 1, GetJsonHex("egModStPuSelect", a));    // inst 1
        }
        else if(GetJsonHex("egType", a).toHex().toUpper() == "02")
        {
            SY1000_default_replace("00", "1A", "01", 1, GetJsonHex("egTePuSelect", a));       // inst 1
        }
        else if(GetJsonHex("egType", a).toHex().toUpper() == "03")
        {
            SY1000_default_replace("00", "1A", "01", 1, GetJsonHex("egLpPuSelect", a));       // inst 1
        }
        else if(GetJsonHex("egType", a).toHex().toUpper() == "04")
        {
            SY1000_default_replace("00", "1A", "01", 1, GetJsonHex("egP90PuSelect", a));      // inst 1
        }
        else if(GetJsonHex("egType", a).toHex().toUpper() == "05")
        {
            SY1000_default_replace("00", "1A", "01", 1, GetJsonHex("eg335PuSelect", a));      // inst 1
        }
        else if(GetJsonHex("egType", a).toHex().toUpper() == "06")
        {
            SY1000_default_replace("00", "1A", "01", 1, GetJsonHex("egL4PuSelect", a));       // inst 1
        }
        else if(GetJsonHex("egType", a).toHex().toUpper() == "07")
        {
            SY1000_default_replace("00", "1A", "01", 1, GetJsonHex("egRickPuSelect", a));     // inst 1
        }
        else if(GetJsonHex("egType", a).toHex().toUpper() == "08")
        {
            SY1000_default_replace("00", "1A", "01", 1, GetJsonHex("egLipsPuSelect", a));     // inst 1
        }
        else if(GetJsonHex("egType", a).toHex().toUpper() == "09")
        {
            SY1000_default_replace("00", "1A", "01", 1, GetJsonHex("egAdvSinglePuSelect", a)); // inst 1
        }
        else if(GetJsonHex("egType", a).toHex().toUpper() == "0A")
        {
            SY1000_default_replace("00", "1A", "01", 1, GetJsonHex("egAdvHumPuSelect", a));   // inst 1
        };
        SY1000_default_replace("00", "1B", "00", 1, GetJsonHex("acType", a));                 // inst 1
        SY1000_default_replace("00", "1B", "0A", 1, GetJsonHex("acVolume", a));               // inst 1
        SY1000_default_replace("00", "1B", "0B", 1, convert_GP10_value(GetJsonHex("acTone", a), "01", "59")); // inst 1
        SY1000_default_replace("00", "1B", "01", 1, GetJsonHex("acBody", a));                 // inst 1
        //SY1000_default_replace("00", "1B", "02", 1, GetJsonHex("acAttackLevel", a));          // inst 1
        SY1000_default_replace("00", "1B", "04", 1, GetJsonHex("acSens", a));                 // inst 1
        SY1000_default_replace("00", "1B", "05", 1, GetJsonHex("acColor", a));                // inst 1
        SY1000_default_replace("00", "1B", "06", 1, GetJsonHex("acDecay", a));                // inst 1
        SY1000_default_replace("00", "1B", "07", 1, GetJsonHex("acBuzz", a));                 // inst 1
        SY1000_default_replace("00", "1B", "02", 1, GetJsonHex("acAttack", a));               // inst 1
        SY1000_default_replace("00", "1B", "03", 1, GetJsonHex("acPUSelect", a));             // inst 1
        if(GetJsonHex("acType", a).toHex().toUpper() == "07")
        {
            SY1000_default_replace("00", "1B", "08", 1, GetJsonHex("acBanjoResonance", a));   // inst 1
        }
        else
        {
            SY1000_default_replace("00", "1B", "08", 1, GetJsonHex("acResonatorResonance", a)); // inst 1
        };
        SY1000_default_replace("00", "1B", "09", 1, GetJsonHex("acSustain", a));               // inst 1
        SY1000_default_replace("00", "1C", "00", 1, GetJsonHex("ebType", a));                  // inst 1
        SY1000_default_replace("00", "1C", "09", 1, GetJsonHex("ebVolume", a));                // inst 1
        SY1000_default_replace("00", "1C", "0A", 1, GetJsonHex("ebTone", a));                  // inst 1
        SY1000_default_replace("00", "1C", "04", 1, GetJsonHex("ebSens", a));                  // inst 1
        SY1000_default_replace("00", "1C", "06", 1, GetJsonHex("ebAttack", a));                // inst 1
        SY1000_default_replace("00", "1C", "03", 1, GetJsonHex("ebToneType", a));              // inst 1
        SY1000_default_replace("00", "1C", "05", 1, GetJsonHex("ebDepth", a));                 // inst 1
        SY1000_default_replace("00", "1C", "07", 1, GetJsonHex("ebResonance", a));             // inst 1
        SY1000_default_replace("00", "1C", "08", 1, GetJsonHex("ebDirectLevel", a));           // inst 1
        SY1000_default_replace("00", "1C", "02", 1, GetJsonHex("ebRearVolume", a));            // inst 1
        SY1000_default_replace("00", "1C", "01", 1, GetJsonHex("ebFrontVolume", a));           // inst 1
        if(GetJsonHex("mdlType", a).toHex().toUpper() == "03")
        {
            SY1000_default_replace("00", "15", "01", 1, convert_GP10_value(GetJsonHex("synthSynthType", a), "02", "18")); // inst 1
        };
        SY1000_default_replace("00", "19", "00", 1, GetJsonHex("synthGR300Mode", a));  // inst 1
        //SY1000_default_replace("00", "19", "00", 1, GetJsonHex("synthGR300Volume", a));  // inst 1
        SY1000_default_replace("00", "19", "01", 1, GetJsonHex("synthGR300CompSw", a));  // inst 1
        SY1000_default_replace("00", "19", "02", 1, GetJsonHex("synthGR300Cutoff", a));  // inst 1
        SY1000_default_replace("00", "19", "03", 1, GetJsonHex("synthGR300Resonance", a));  // inst 1
        SY1000_default_replace("00", "19", "04", 1, GetJsonHex("synthGR300EnvModSw", a));  // inst 1
        SY1000_default_replace("00", "19", "05", 1, GetJsonHex("synthGR300EnvModSens", a));  // inst 1
        SY1000_default_replace("00", "19", "06", 1, GetJsonHex("synthGR300EnvModAtck", a));  // inst 1
        SY1000_default_replace("00", "19", "07", 1, GetJsonHex("synthGR300PitchSw", a));  // inst 1
        SY1000_default_replace("00", "19", "08", 1, convert_GP10_value(GetJsonHex("synthGR300PShiftA", a), "01", "61"));  // inst 1
        SY1000_default_replace("00", "19", "09", 1, convert_GP10_value(GetJsonHex("synthGR300PFineA", a), "01", "59"));  // inst 1
        SY1000_default_replace("00", "19", "0A", 1, convert_GP10_value(GetJsonHex("synthGR300PShiftB", a), "01", "61"));  // inst 1
        SY1000_default_replace("00", "19", "0B", 1, convert_GP10_value(GetJsonHex("synthGR300PFineB", a), "01", "59"));  // inst 1
        SY1000_default_replace("00", "19", "0C", 1, GetJsonHex("synthGR300PDuet", a));  // inst 1
        SY1000_default_replace("00", "19", "0D", 1, GetJsonHex("synthGR300SweepSw", a));  // inst 1
        SY1000_default_replace("00", "19", "0E", 1, GetJsonHex("synthGR300SweepRise", a));  // inst 1
        SY1000_default_replace("00", "19", "0F", 1, GetJsonHex("synthGR300SweepFall", a));  // inst 1
        SY1000_default_replace("00", "19", "10", 1, GetJsonHex("synthGR300VibratoSw", a));  // inst 1
        SY1000_default_replace("00", "19", "11", 1, GetJsonHex("synthGR300VibRate", a));  // inst 1
        SY1000_default_replace("00", "19", "12", 1, GetJsonHex("synthGR300VibDepth", a));  // inst 1

        SY1000_default_replace("00", "18", "00", 1, GetJsonHex("synthOscMode", a));  // inst 1
        //SY1000_default_replace("00", "18", "00", 1, GetJsonHex("synthOscVolume", a));  // inst 1
        SY1000_default_replace("00", "18", "01", 1, GetJsonHex("synthOscWaveform1", a));  // inst 1
        SY1000_default_replace("00", "18", "02", 1, convert_GP10_value(GetJsonHex("synthOscPitch1", a), "01", "58"));  // inst 1
        SY1000_default_replace("00", "18", "03", 1, convert_GP10_value(GetJsonHex("synthOscPitchFine1", a), "01", "59"));  // inst 1
        SY1000_default_replace("00", "18", "04", 1, GetJsonHex("synthOscPWWidth1", a));  // inst 1
        SY1000_default_replace("00", "18", "05", 1, GetJsonHex("synthOscPWModRate1", a));  // inst 1
        SY1000_default_replace("00", "18", "06", 1, GetJsonHex("synthOscPEnvAttck1", a));  // inst 1
        SY1000_default_replace("00", "18", "07", 1, GetJsonHex("synthOscPEnvDecay1", a));  // inst 1
        SY1000_default_replace("00", "18", "08", 1, GetJsonHex("synthOscPEnvDepth1", a));  // inst 1
        SY1000_default_replace("00", "18", "09", 1, GetJsonHex("synthOscLevel1", a));  // inst 1
        SY1000_default_replace("00", "18", "0A", 1, GetJsonHex("synthOscWaveform2", a));  // inst 1
        SY1000_default_replace("00", "18", "0B", 1, convert_GP10_value(GetJsonHex("synthOscPitch2", a), "01", "58"));  // inst 1
        SY1000_default_replace("00", "18", "0C", 1, convert_GP10_value(GetJsonHex("synthOscPitchFine2", a), "01", "59"));  // inst 1
        SY1000_default_replace("00", "18", "0D", 1, GetJsonHex("synthOscPWWidth2", a));  // inst 1
        SY1000_default_replace("00", "18", "0E", 1, GetJsonHex("synthOscPWModRate2", a));  // inst 1
        SY1000_default_replace("00", "18", "0F", 1, GetJsonHex("synthOscPEnvAttck2", a));  // inst 1
        SY1000_default_replace("00", "18", "10", 1, GetJsonHex("synthOscPEnvDecay2", a));  // inst 1
        SY1000_default_replace("00", "18", "11", 1, GetJsonHex("synthOscPEnvDepth2", a));  // inst 1
        SY1000_default_replace("00", "18", "12", 1, GetJsonHex("synthOscLevel2", a));  // inst 1
        if(GetJsonHex("synthOscFilterType", a).toHex().toUpper().toInt(&ok,16)>0)  // inst 1
        {
            SY1000_default_replace("00", "18", "1A", 1, QByteArray::fromHex("01"));    // inst 1 filter sw
        };
        SY1000_default_replace("00", "18", "1B", 1, convert_GP10_value(GetJsonHex("synthOscFilterType", a), "02", "19"));  // inst 1
        SY1000_default_replace("00", "18", "1C", 1, GetJsonHex("synthOscFilterSlope", a));  // inst 1
        SY1000_default_replace("00", "18", "1D", 1, GetJsonHex("synthOscFilterCutoff", a));  // inst 1
        SY1000_default_replace("00", "18", "1E", 1, GetJsonHex("synthOscFltrCtOffFlw", a));  // inst 1
        SY1000_default_replace("00", "18", "1F", 1, GetJsonHex("synthOscFltrReso", a));  // inst 1
        SY1000_default_replace("00", "18", "20", 1, convert_GP10_value(GetJsonHex("synthOscFltrVeloSens", a), "01", "59"));  // inst 1
        SY1000_default_replace("00", "18", "21", 1, GetJsonHex("synthOscFltrEnvAttck", a));  // inst 1
        SY1000_default_replace("00", "18", "22", 1, GetJsonHex("synthOscFltrEnvDecay", a));  // inst 1
        SY1000_default_replace("00", "18", "23", 1, GetJsonHex("synthOscFltrEnvSustn", a));  // inst 1
        SY1000_default_replace("00", "18", "24", 1, GetJsonHex("synthOscFltrEnvRels", a));  // inst 1
        SY1000_default_replace("00", "18", "25", 1, convert_GP10_value(GetJsonHex("synthOscFltrEnvDepth", a), "01", "59"));  // inst 1
        SY1000_default_replace("00", "18", "26", 1, GetJsonHex("synthOscAmpVeloSens", a));  // inst 1
        SY1000_default_replace("00", "18", "27", 1, GetJsonHex("synthOscAmpEnvAttck", a));  // inst 1
        SY1000_default_replace("00", "18", "28", 1, GetJsonHex("synthOscAmpEnvDecay", a));  // inst 1
        SY1000_default_replace("00", "18", "29", 1, GetJsonHex("synthOscAmpEnvSustn", a));  // inst 1
        SY1000_default_replace("00", "18", "2A", 1, GetJsonHex("synthOscAmpEnvRels", a));  // inst 1
        SY1000_default_replace("00", "18", "2E", 1, GetJsonHex("synthOscLFO1Shape", a));  // inst 1
        if(GetJsonHex("synthOscLFO1Rate", a).toHex().toUpper().toInt(&ok,16)>0)  // inst 1
        {
            SY1000_default_replace("00", "18", "2D", 1, QByteArray::fromHex("01"));    // inst 1 lfo 1 sw
        };
        SY1000_default_replace("00", "18", "2F", 1, convert_GP10_value(GetJsonHex("synthOscLFO1Rate", a), "02", "1D"));  // inst 1
        SY1000_default_replace("00", "18", "30", 1, GetJsonHex("synthOscLFO1PtchDpt1", a));  // inst 1
        SY1000_default_replace("00", "18", "31", 1, GetJsonHex("synthOscLFO1PtchDpt2", a));  // inst 1
        SY1000_default_replace("00", "18", "32", 1, GetJsonHex("synthOscLFO1FltDepth", a));  // inst 1
        SY1000_default_replace("00", "18", "33", 1, GetJsonHex("synthOscLFO1AmpDepth", a));  // inst 1
        SY1000_default_replace("00", "18", "34", 1, GetJsonHex("synthOscLFO1DlyTime", a));  // inst 1
        SY1000_default_replace("00", "18", "35", 1, GetJsonHex("synthOscLFO1FadeTime", a));  // inst 1
        SY1000_default_replace("00", "18", "38", 1, GetJsonHex("synthOscLFO2Shape", a));  // inst 1
        if(GetJsonHex("synthOscLFO2Rate", a).toHex().toUpper().toInt(&ok,16)>0)  // inst 1
        {
            SY1000_default_replace("00", "18", "37", 1, QByteArray::fromHex("01"));    // inst 1 lfo 2 sw
        };
        SY1000_default_replace("00", "18", "39", 1, convert_GP10_value(GetJsonHex("synthOscLFO2Rate", a), "02", "1D"));  // inst 1
        SY1000_default_replace("00", "18", "3A", 1, GetJsonHex("synthOscLFO2PtchDpt1", a));  // inst 1
        SY1000_default_replace("00", "18", "3B", 1, GetJsonHex("synthOscLFO2PtchDpt2", a));  // inst 1
        SY1000_default_replace("00", "18", "3C", 1, GetJsonHex("synthOscLFO2FltDepth", a));  // inst 1
        SY1000_default_replace("00", "18", "3D", 1, GetJsonHex("synthOscLFO2AmpDepth", a));  // inst 1
        SY1000_default_replace("00", "18", "3E", 1, GetJsonHex("synthOscLFO2DlyTime", a));  // inst 1
        SY1000_default_replace("00", "18", "3F", 1, GetJsonHex("synthOscLFO2FadeTime", a));  // inst 1
        SY1000_default_replace("00", "18", "13", 1, GetJsonHex("synthOscPolyMono", a));  // inst 1
        SY1000_default_replace("00", "18", "14", 1, GetJsonHex("synthOscChromatic", a));  // inst 1
        SY1000_default_replace("00", "18", "15", 1, GetJsonHex("synthOscPortamento", a));  // inst 1
        SY1000_default_replace("00", "18", "16", 1, GetJsonHex("synthOscPortaRate", a));  // inst 1
        SY1000_default_replace("00", "18", "17", 1, GetJsonHex("synthOscPortaMode", a));  // inst 1
        SY1000_default_replace("00", "18", "18", 1, GetJsonHex("synthOscHoldMode", a));  // inst 1
        SY1000_default_replace("00", "18", "19", 1, GetJsonHex("synthOscLowVeloCut", a));  // inst 1

        SY1000_default_replace("00", "16", "04", 1, convert_GP10_value(GetJsonHex("synthWaveType", a), "02", "1A"));  // inst 1
        //SY1000_default_replace("00", "16", "04", 1, GetJsonHex("synthWaveVolume", a));  // inst 1
        SY1000_default_replace("00", "16", "1F", 1, GetJsonHex("synthWaveCutoff", a));  // inst 1
        SY1000_default_replace("00", "16", "20", 1, GetJsonHex("synthWaveResonance", a));  // inst 1
        SY1000_default_replace("00", "16", "30", 1, GetJsonHex("synthWaveOctave", a));  // inst 1

        SY1000_default_replace("00", "1E", "00", 1, GetJsonHex("polyfxPolyfxType", a));  // inst 1
        SY1000_default_replace("00", "1E", "01", 1, GetJsonHex("polyfxGtrVol", a));  // inst 1
        SY1000_default_replace("00", "1E", "02", 1, GetJsonHex("pfxdistGainBal", a));  // inst 1
        SY1000_default_replace("00", "1E", "03", 1, GetJsonHex("pfxdistGain", a));  // inst 1
        if(GetJsonHex("polyfxPolyfxType", a).toHex().toUpper() == "00")  // inst 1
        {
            SY1000_default_replace("00", "1E", "04", 1, GetJsonHex("pfxdistColor", a));  // inst 1
            SY1000_default_replace("00", "1E", "05", 1, convert_GP10_value(GetJsonHex("pfxdistTone", a), "01", "59"));  // inst 1
            //SY1000_default_replace("00", "1E", "00", 1, GetJsonHex("pfxdistLevel", a));  // inst 1
        };
        if(GetJsonHex("polyfxPolyfxType", a).toHex().toUpper() == "01")  // inst 1
        {
            SY1000_default_replace("00", "1E", "04", 1, GetJsonHex("pfxcrysalColor", a));  // inst 1
            SY1000_default_replace("00", "1E", "05", 1, convert_GP10_value(GetJsonHex("pfxcrysalTone", a), "01", "59"));  // inst 1
            //SY1000_default_replace("00", "1E", "00", 1, GetJsonHex("pfxcrysalLevel", a));  // inst 1
        };
        if(GetJsonHex("polyfxPolyfxType", a).toHex().toUpper() == "02")  // inst 1
        {
            SY1000_default_replace("00", "1E", "04", 1, GetJsonHex("pfxrichmodColor", a));  // inst 1
            SY1000_default_replace("00", "1E", "05", 1, convert_GP10_value(GetJsonHex("pfxrichmodTone", a), "01", "59"));  // inst 1
            //SY1000_default_replace("00", "1E", "00", 1, GetJsonHex("pfxrichmodLevel", a));  // inst 1
        };
        if(GetJsonHex("polyfxPolyfxType", a).toHex().toUpper() == "03")  // inst 1
        {
            SY1000_default_replace("00", "1E", "04", 1, GetJsonHex("pfxslowpadColor", a));  // inst 1
            SY1000_default_replace("00", "1E", "05", 1, convert_GP10_value(GetJsonHex("pfxslowpadTone", a), "01", "59"));  // inst 1
            //SY1000_default_replace("00", "1E", "00", 1, GetJsonHex("pfxslowpadLevel", a));  // inst 1
        };
        SY1000_default_replace("00", "1E", "06", 1, GetJsonHex("pfxtwahMode", a));  // inst 1
        SY1000_default_replace("00", "1E", "07", 1, GetJsonHex("pfxtwahPolar", a));  // inst 1
        SY1000_default_replace("00", "1E", "08", 1, GetJsonHex("pfxtwahSens", a));  // inst 1
        SY1000_default_replace("00", "1E", "09", 1, GetJsonHex("pfxtwahFreq", a));  // inst 1
        SY1000_default_replace("00", "1E", "0B", 1, GetJsonHex("pfxtwahPeak", a));  // inst 1
        //SY1000_default_replace("00", "1E", "00", 1, GetJsonHex("pfxtwahVolume", a));  // inst 1
        SY1000_default_replace("00", "1E", "0C", 1, GetJsonHex("pfxtwahToneType", a));  // inst 1
        SY1000_default_replace("00", "1E", "0D", 1, GetJsonHex("pfxtwahCompSw", a));  // inst 1
        SY1000_default_replace("00", "1E", "0E", 1, GetJsonHex("pfxtwahCompSus", a));  // inst 1
        SY1000_default_replace("00", "1E", "0F", 1, GetJsonHex("pfxtwahCompAtk", a));  // inst 1
        SY1000_default_replace("00", "1E", "0A", 1, GetJsonHex("pfxtwahDecay", a));  // inst 1

        SY1000_default_replace("00", "1F", "00", 1, GetJsonHex("alttuneOnOff", a));  // inst 1
        SY1000_default_replace("00", "1F", "01", 1, GetJsonHex("alttuneType", a));  // inst 1
        SY1000_default_replace("00", "1F", "02", 1, convert_GP10_value(GetJsonHex("alttuneShift1", a), "01", "58"));  // inst 1
        SY1000_default_replace("00", "1F", "03", 1, convert_GP10_value(GetJsonHex("alttuneShift2", a), "01", "58"));  // inst 1
        SY1000_default_replace("00", "1F", "04", 1, convert_GP10_value(GetJsonHex("alttuneShift3", a), "01", "58"));  // inst 1
        SY1000_default_replace("00", "1F", "05", 1, convert_GP10_value(GetJsonHex("alttuneShift4", a), "01", "58"));  // inst 1
        SY1000_default_replace("00", "1F", "06", 1, convert_GP10_value(GetJsonHex("alttuneShift5", a), "01", "58"));  // inst 1
        SY1000_default_replace("00", "1F", "07", 1, convert_GP10_value(GetJsonHex("alttuneShift6", a), "01", "58"));  // inst 1
        SY1000_default_replace("00", "1F", "08", 1, convert_GP10_value(GetJsonHex("alttuneFine1", a), "01", "59"));  // inst 1
        SY1000_default_replace("00", "1F", "09", 1, convert_GP10_value(GetJsonHex("alttuneFine2", a), "01", "59"));  // inst 1
        SY1000_default_replace("00", "1F", "0A", 1, convert_GP10_value(GetJsonHex("alttuneFine3", a), "01", "59"));  // inst 1
        SY1000_default_replace("00", "1F", "0B", 1, convert_GP10_value(GetJsonHex("alttuneFine4", a), "01", "59"));  // inst 1
        SY1000_default_replace("00", "1F", "0C", 1, convert_GP10_value(GetJsonHex("alttuneFine5", a), "01", "59"));  // inst 1
        SY1000_default_replace("00", "1F", "0D", 1, convert_GP10_value(GetJsonHex("alttuneFine6", a), "01", "59"));  // inst 1
        SY1000_default_replace("00", "1F", "0E", 1, GetJsonHex("str12OnOff", a));  // inst 1
        SY1000_default_replace("00", "1F", "0F", 1, GetJsonHex("str12Type", a));  // inst 1
        SY1000_default_replace("00", "1F", "10", 1, convert_GP10_value(GetJsonHex("str12PitchShft1", a), "01", "58"));  // inst 1
        SY1000_default_replace("00", "1F", "11", 1, convert_GP10_value(GetJsonHex("str12PitchShft2", a), "01", "58"));  // inst 1
        SY1000_default_replace("00", "1F", "12", 1, convert_GP10_value(GetJsonHex("str12PitchShft3", a), "01", "58"));  // inst 1
        SY1000_default_replace("00", "1F", "13", 1, convert_GP10_value(GetJsonHex("str12PitchShft4", a), "01", "58"));  // inst 1
        SY1000_default_replace("00", "1F", "14", 1, convert_GP10_value(GetJsonHex("str12PitchShft5", a), "01", "58"));  // inst 1
        SY1000_default_replace("00", "1F", "15", 1, convert_GP10_value(GetJsonHex("str12PitchShft6", a), "01", "58"));  // inst 1
        SY1000_default_replace("00", "1F", "16", 1, convert_GP10_value(GetJsonHex("str12PitchFine1", a), "01", "59"));  // inst 1
        SY1000_default_replace("00", "1F", "17", 1, convert_GP10_value(GetJsonHex("str12PitchFine2", a), "01", "59"));  // inst 1
        SY1000_default_replace("00", "1F", "18", 1, convert_GP10_value(GetJsonHex("str12PitchFine3", a), "01", "59"));  // inst 1
        SY1000_default_replace("00", "1F", "19", 1, convert_GP10_value(GetJsonHex("str12PitchFine4", a), "01", "59"));  // inst 1
        SY1000_default_replace("00", "1F", "1A", 1, convert_GP10_value(GetJsonHex("str12PitchFine5", a), "01", "59"));  // inst 1
        SY1000_default_replace("00", "1F", "1B", 1, convert_GP10_value(GetJsonHex("str12PitchFine6", a), "01", "59"));  // inst 1
        SY1000_default_replace("00", "1F", "1C", 1, GetJsonHex("str12Level1", a));  // inst 1
        SY1000_default_replace("00", "1F", "1D", 1, GetJsonHex("str12Level2", a));  // inst 1
        SY1000_default_replace("00", "1F", "1E", 1, GetJsonHex("str12Level3", a));  // inst 1
        SY1000_default_replace("00", "1F", "1F", 1, GetJsonHex("str12Level4", a));  // inst 1
        SY1000_default_replace("00", "1F", "20", 1, GetJsonHex("str12Level5", a));  // inst 1
        SY1000_default_replace("00", "1F", "21", 1, GetJsonHex("str12Level6", a));  // inst 1
        SY1000_default_replace("00", "1F", "22", 1, GetJsonHex("str12Delay1", a));  // inst 1
        SY1000_default_replace("00", "1F", "23", 1, GetJsonHex("str12Delay2", a));  // inst 1
        SY1000_default_replace("00", "1F", "24", 1, GetJsonHex("str12Delay3", a));  // inst 1
        SY1000_default_replace("00", "1F", "25", 1, GetJsonHex("str12Delay4", a));  // inst 1
        SY1000_default_replace("00", "1F", "26", 1, GetJsonHex("str12Delay5", a));  // inst 1
        SY1000_default_replace("00", "1F", "27", 1, GetJsonHex("str12Delay6", a));  // inst 1
        SY1000_default_replace("00", "1F", "28", 1, GetJsonHex("strbendOnOff", a));  // inst 1
        SY1000_default_replace("00", "1F", "29", 1, convert_GP10_value(GetJsonHex("strbendDepth1", a), "01", "58"));  // inst 1
        SY1000_default_replace("00", "1F", "2A", 1, convert_GP10_value(GetJsonHex("strbendDepth2", a), "01", "58"));  // inst 1
        SY1000_default_replace("00", "1F", "2B", 1, convert_GP10_value(GetJsonHex("strbendDepth3", a), "01", "58"));  // inst 1
        SY1000_default_replace("00", "1F", "2C", 1, convert_GP10_value(GetJsonHex("strbendDepth4", a), "01", "58"));  // inst 1
        SY1000_default_replace("00", "1F", "2D", 1, convert_GP10_value(GetJsonHex("strbendDepth5", a), "01", "58"));  // inst 1
        SY1000_default_replace("00", "1F", "2E", 1, convert_GP10_value(GetJsonHex("strbendDepth6", a), "01", "58"));  // inst 1
        SY1000_default_replace("00", "1F", "2F", 1, GetJsonHex("strbendControl", a));  // inst 1


        SY1000_default_replace("00", "25", "00", 1, GetJsonHex("egType", a));        // inst 2
        SY1000_default_replace("00", "25", "08", 1, GetJsonHex("egVolume", a));      // inst 2
        SY1000_default_replace("00", "25", "09", 1, GetJsonHex("egTone", a));        // inst 2
        SY1000_default_replace("00", "25", "02", 1, GetJsonHex("egFretlessToneType", a));     // inst 2
        SY1000_default_replace("00", "25", "03", 1, GetJsonHex("egFretlessSens", a));         // inst 2
        SY1000_default_replace("00", "25", "04", 1, GetJsonHex("egFretlessDepth", a));        // inst 2
        SY1000_default_replace("00", "25", "05", 1, GetJsonHex("egFretlessAttack", a));       // inst 2
        SY1000_default_replace("00", "25", "06", 1, GetJsonHex("egFretlessResonance", a));    // inst 2
        SY1000_default_replace("00", "25", "07", 1, GetJsonHex("egFretlessDirectLevel", a));  // inst 2
        if(GetJsonHex("egType", a).toHex().toUpper() == "00")
        {
            SY1000_default_replace("00", "25", "01", 1, GetJsonHex("egClaStPuSelect", a));    // inst 2
        }
        else if(GetJsonHex("egType", a).toHex().toUpper() == "01")
        {
            SY1000_default_replace("00", "25", "01", 1, GetJsonHex("egModStPuSelect", a));    // inst 2
        }
        else if(GetJsonHex("egType", a).toHex().toUpper() == "02")
        {
            SY1000_default_replace("00", "25", "01", 1, GetJsonHex("egTePuSelect", a));       // inst 2
        }
        else if(GetJsonHex("egType", a).toHex().toUpper() == "03")
        {
            SY1000_default_replace("00", "25", "01", 1, GetJsonHex("egLpPuSelect", a));       // inst 2
        }
        else if(GetJsonHex("egType", a).toHex().toUpper() == "04")
        {
            SY1000_default_replace("00", "25", "01", 1, GetJsonHex("egP90PuSelect", a));      // inst 2
        }
        else if(GetJsonHex("egType", a).toHex().toUpper() == "05")
        {
            SY1000_default_replace("00", "25", "01", 1, GetJsonHex("eg335PuSelect", a));      // inst 2
        }
        else if(GetJsonHex("egType", a).toHex().toUpper() == "06")
        {
            SY1000_default_replace("00", "25", "01", 1, GetJsonHex("egL4PuSelect", a));       // inst 2
        }
        else if(GetJsonHex("egType", a).toHex().toUpper() == "07")
        {
            SY1000_default_replace("00", "25", "01", 1, GetJsonHex("egRickPuSelect", a));     // inst 2
        }
        else if(GetJsonHex("egType", a).toHex().toUpper() == "08")
        {
            SY1000_default_replace("00", "25", "01", 1, GetJsonHex("egLipsPuSelect", a));     // inst 2
        }
        else if(GetJsonHex("egType", a).toHex().toUpper() == "09")
        {
            SY1000_default_replace("00", "25", "01", 1, GetJsonHex("egAdvSinglePuSelect", a)); // inst 2
        }
        else if(GetJsonHex("egType", a).toHex().toUpper() == "0A")
        {
            SY1000_default_replace("00", "25", "01", 1, GetJsonHex("egAdvHumPuSelect", a));   // inst 2
        };
        SY1000_default_replace("00", "26", "00", 1, GetJsonHex("acType", a));                 // inst 2
        SY1000_default_replace("00", "26", "0A", 1, GetJsonHex("acVolume", a));               // inst 2
        SY1000_default_replace("00", "26", "0B", 1, convert_GP10_value(GetJsonHex("acTone", a), "01", "59")); // inst 2
        SY1000_default_replace("00", "26", "01", 1, GetJsonHex("acBody", a));                 // inst 2
        //SY1000_default_replace("00", "26", "02", 1, GetJsonHex("acAttackLevel", a));          // inst 2
        SY1000_default_replace("00", "26", "04", 1, GetJsonHex("acSens", a));                 // inst 2
        SY1000_default_replace("00", "26", "05", 1, GetJsonHex("acColor", a));                // inst 2
        SY1000_default_replace("00", "26", "06", 1, GetJsonHex("acDecay", a));                // inst 2
        SY1000_default_replace("00", "26", "07", 1, GetJsonHex("acBuzz", a));                 // inst 2
        SY1000_default_replace("00", "26", "02", 1, GetJsonHex("acAttack", a));               // inst 2
        SY1000_default_replace("00", "26", "03", 1, GetJsonHex("acPUSelect", a));             // inst 2
        if(GetJsonHex("acType", a).toHex().toUpper() == "07")
        {
            SY1000_default_replace("00", "26", "08", 1, GetJsonHex("acBanjoResonance", a));   // inst 2
        }
        else
        {
            SY1000_default_replace("00", "26", "08", 1, GetJsonHex("acResonatorResonance", a)); // inst 2
        };
        SY1000_default_replace("00", "26", "09", 1, GetJsonHex("acSustain", a));               // inst 2
        SY1000_default_replace("00", "27", "00", 1, GetJsonHex("ebType", a));                  // inst 2
        SY1000_default_replace("00", "27", "09", 1, GetJsonHex("ebVolume", a));                // inst 2
        SY1000_default_replace("00", "27", "0A", 1, GetJsonHex("ebTone", a));                  // inst 2
        SY1000_default_replace("00", "27", "04", 1, GetJsonHex("ebSens", a));                  // inst 2
        SY1000_default_replace("00", "27", "06", 1, GetJsonHex("ebAttack", a));                // inst 2
        SY1000_default_replace("00", "27", "03", 1, GetJsonHex("ebToneType", a));              // inst 2
        SY1000_default_replace("00", "27", "05", 1, GetJsonHex("ebDepth", a));                 // inst 2
        SY1000_default_replace("00", "27", "07", 1, GetJsonHex("ebResonance", a));             // inst 2
        SY1000_default_replace("00", "27", "08", 1, GetJsonHex("ebDirectLevel", a));           // inst 2
        SY1000_default_replace("00", "27", "02", 1, GetJsonHex("ebRearVolume", a));            // inst 2
        SY1000_default_replace("00", "27", "01", 1, GetJsonHex("ebFrontVolume", a));           // inst 2
        if(GetJsonHex("mdlType", a).toHex().toUpper() == "03")
        {
            SY1000_default_replace("00", "20", "01", 1, convert_GP10_value(GetJsonHex("synthSynthType", a), "02", "18")); // inst 2
        };
        SY1000_default_replace("00", "24", "00", 1, GetJsonHex("synthGR300Mode", a));  // inst 2
        //SY1000_default_replace("00", "24", "00", 1, GetJsonHex("synthGR300Volume", a));  // inst 2
        SY1000_default_replace("00", "24", "01", 1, GetJsonHex("synthGR300CompSw", a));  // inst 2
        SY1000_default_replace("00", "24", "02", 1, GetJsonHex("synthGR300Cutoff", a));  // inst 2
        SY1000_default_replace("00", "24", "03", 1, GetJsonHex("synthGR300Resonance", a));  // inst 2
        SY1000_default_replace("00", "24", "04", 1, GetJsonHex("synthGR300EnvModSw", a));  // inst 2
        SY1000_default_replace("00", "24", "05", 1, GetJsonHex("synthGR300EnvModSens", a));  // inst 2
        SY1000_default_replace("00", "24", "06", 1, GetJsonHex("synthGR300EnvModAtck", a));  // inst 2
        SY1000_default_replace("00", "24", "07", 1, GetJsonHex("synthGR300PitchSw", a));  // inst 2
        SY1000_default_replace("00", "24", "08", 1, convert_GP10_value(GetJsonHex("synthGR300PShiftA", a), "01", "61"));  // inst 2
        SY1000_default_replace("00", "24", "09", 1, convert_GP10_value(GetJsonHex("synthGR300PFineA", a), "01", "59"));  // inst 2
        SY1000_default_replace("00", "24", "0A", 1, convert_GP10_value(GetJsonHex("synthGR300PShiftB", a), "01", "61"));  // inst 2
        SY1000_default_replace("00", "24", "0B", 1, convert_GP10_value(GetJsonHex("synthGR300PFineB", a), "01", "59"));  // inst 2
        SY1000_default_replace("00", "24", "0C", 1, GetJsonHex("synthGR300PDuet", a));  // inst 2
        SY1000_default_replace("00", "24", "0D", 1, GetJsonHex("synthGR300SweepSw", a));  // inst 2
        SY1000_default_replace("00", "24", "0E", 1, GetJsonHex("synthGR300SweepRise", a));  // inst 2
        SY1000_default_replace("00", "24", "0F", 1, GetJsonHex("synthGR300SweepFall", a));  // inst 2
        SY1000_default_replace("00", "24", "10", 1, GetJsonHex("synthGR300VibratoSw", a));  // inst 2
        SY1000_default_replace("00", "24", "11", 1, GetJsonHex("synthGR300VibRate", a));  // inst 2
        SY1000_default_replace("00", "24", "12", 1, GetJsonHex("synthGR300VibDepth", a));  // inst 2

        SY1000_default_replace("00", "23", "00", 1, GetJsonHex("synthOscMode", a));  // inst 2
        //SY1000_default_replace("00", "23", "00", 1, GetJsonHex("synthOscVolume", a));  // inst 2
        SY1000_default_replace("00", "23", "01", 1, GetJsonHex("synthOscWaveform1", a));  // inst 2
        SY1000_default_replace("00", "23", "02", 1, convert_GP10_value(GetJsonHex("synthOscPitch1", a), "01", "58"));  // inst 2
        SY1000_default_replace("00", "23", "03", 1, convert_GP10_value(GetJsonHex("synthOscPitchFine1", a), "01", "59"));  // inst 2
        SY1000_default_replace("00", "23", "04", 1, GetJsonHex("synthOscPWWidth1", a));  // inst 2
        SY1000_default_replace("00", "23", "05", 1, GetJsonHex("synthOscPWModRate1", a));  // inst 2
        SY1000_default_replace("00", "23", "06", 1, GetJsonHex("synthOscPEnvAttck1", a));  // inst 2
        SY1000_default_replace("00", "23", "07", 1, GetJsonHex("synthOscPEnvDecay1", a));  // inst 2
        SY1000_default_replace("00", "23", "08", 1, GetJsonHex("synthOscPEnvDepth1", a));  // inst 2
        SY1000_default_replace("00", "23", "09", 1, GetJsonHex("synthOscLevel1", a));  // inst 2
        SY1000_default_replace("00", "23", "0A", 1, GetJsonHex("synthOscWaveform2", a));  // inst 2
        SY1000_default_replace("00", "23", "0B", 1, convert_GP10_value(GetJsonHex("synthOscPitch2", a), "01", "58"));  // inst 2
        SY1000_default_replace("00", "23", "0C", 1, convert_GP10_value(GetJsonHex("synthOscPitchFine2", a), "01", "59"));  // inst 2
        SY1000_default_replace("00", "23", "0D", 1, GetJsonHex("synthOscPWWidth2", a));  // inst 2
        SY1000_default_replace("00", "23", "0E", 1, GetJsonHex("synthOscPWModRate2", a));  // inst 2
        SY1000_default_replace("00", "23", "0F", 1, GetJsonHex("synthOscPEnvAttck2", a));  // inst 2
        SY1000_default_replace("00", "23", "10", 1, GetJsonHex("synthOscPEnvDecay2", a));  // inst 2
        SY1000_default_replace("00", "23", "11", 1, GetJsonHex("synthOscPEnvDepth2", a));  // inst 2
        SY1000_default_replace("00", "23", "12", 1, GetJsonHex("synthOscLevel2", a));  // inst 2
        if(GetJsonHex("synthOscFilterType", a).toHex().toUpper().toInt(&ok,16)>0)  // inst 2
        {
            SY1000_default_replace("00", "23", "1A", 1, QByteArray::fromHex("01"));    // inst 2 filter sw
        };
        SY1000_default_replace("00", "23", "1B", 1, convert_GP10_value(GetJsonHex("synthOscFilterType", a), "02", "19"));  // inst 2
        SY1000_default_replace("00", "23", "1C", 1, GetJsonHex("synthOscFilterSlope", a));  // inst 2
        SY1000_default_replace("00", "23", "1D", 1, GetJsonHex("synthOscFilterCutoff", a));  // inst 2
        SY1000_default_replace("00", "23", "1E", 1, GetJsonHex("synthOscFltrCtOffFlw", a));  // inst 2
        SY1000_default_replace("00", "23", "1F", 1, GetJsonHex("synthOscFltrReso", a));  // inst 2
        SY1000_default_replace("00", "23", "20", 1, convert_GP10_value(GetJsonHex("synthOscFltrVeloSens", a), "01", "59"));  // inst 2
        SY1000_default_replace("00", "23", "21", 1, GetJsonHex("synthOscFltrEnvAttck", a));  // inst 2
        SY1000_default_replace("00", "23", "22", 1, GetJsonHex("synthOscFltrEnvDecay", a));  // inst 2
        SY1000_default_replace("00", "23", "23", 1, GetJsonHex("synthOscFltrEnvSustn", a));  // inst 2
        SY1000_default_replace("00", "23", "24", 1, GetJsonHex("synthOscFltrEnvRels", a));  // inst 2
        SY1000_default_replace("00", "23", "25", 1, convert_GP10_value(GetJsonHex("synthOscFltrEnvDepth", a), "01", "59"));  // inst 2
        SY1000_default_replace("00", "23", "26", 1, GetJsonHex("synthOscAmpVeloSens", a));  // inst 2
        SY1000_default_replace("00", "23", "27", 1, GetJsonHex("synthOscAmpEnvAttck", a));  // inst 2
        SY1000_default_replace("00", "23", "28", 1, GetJsonHex("synthOscAmpEnvDecay", a));  // inst 2
        SY1000_default_replace("00", "23", "29", 1, GetJsonHex("synthOscAmpEnvSustn", a));  // inst 2
        SY1000_default_replace("00", "23", "2A", 1, GetJsonHex("synthOscAmpEnvRels", a));  // inst 2
        SY1000_default_replace("00", "23", "2E", 1, GetJsonHex("synthOscLFO1Shape", a));  // inst 2
        if(GetJsonHex("synthOscLFO1Rate", a).toHex().toUpper().toInt(&ok,16)>0)  // inst 2
        {
            SY1000_default_replace("00", "23", "2D", 1, QByteArray::fromHex("01"));    // inst 2 lfo 1 sw
        };
        SY1000_default_replace("00", "23", "2F", 1, convert_GP10_value(GetJsonHex("synthOscLFO1Rate", a), "02", "1D"));  // inst 2
        SY1000_default_replace("00", "23", "30", 1, GetJsonHex("synthOscLFO1PtchDpt1", a));  // inst 2
        SY1000_default_replace("00", "23", "31", 1, GetJsonHex("synthOscLFO1PtchDpt2", a));  // inst 2
        SY1000_default_replace("00", "23", "32", 1, GetJsonHex("synthOscLFO1FltDepth", a));  // inst 2
        SY1000_default_replace("00", "23", "33", 1, GetJsonHex("synthOscLFO1AmpDepth", a));  // inst 2
        SY1000_default_replace("00", "23", "34", 1, GetJsonHex("synthOscLFO1DlyTime", a));  // inst 2
        SY1000_default_replace("00", "23", "35", 1, GetJsonHex("synthOscLFO1FadeTime", a));  // inst 2
        SY1000_default_replace("00", "23", "38", 1, GetJsonHex("synthOscLFO2Shape", a));  // inst 2
        if(GetJsonHex("synthOscLFO2Rate", a).toHex().toUpper().toInt(&ok,16)>0)  // inst 2
        {
            SY1000_default_replace("00", "23", "37", 1, QByteArray::fromHex("01"));    // inst 2 lfo 2 sw
        };
        SY1000_default_replace("00", "23", "39", 1, convert_GP10_value(GetJsonHex("synthOscLFO2Rate", a), "02", "1D"));  // inst 2
        SY1000_default_replace("00", "23", "3A", 1, GetJsonHex("synthOscLFO2PtchDpt1", a));  // inst 2
        SY1000_default_replace("00", "23", "3B", 1, GetJsonHex("synthOscLFO2PtchDpt2", a));  // inst 2
        SY1000_default_replace("00", "23", "3C", 1, GetJsonHex("synthOscLFO2FltDepth", a));  // inst 2
        SY1000_default_replace("00", "23", "3D", 1, GetJsonHex("synthOscLFO2AmpDepth", a));  // inst 2
        SY1000_default_replace("00", "23", "3E", 1, GetJsonHex("synthOscLFO2DlyTime", a));  // inst 2
        SY1000_default_replace("00", "23", "3F", 1, GetJsonHex("synthOscLFO2FadeTime", a));  // inst 2
        SY1000_default_replace("00", "23", "13", 1, GetJsonHex("synthOscPolyMono", a));  // inst 2
        SY1000_default_replace("00", "23", "14", 1, GetJsonHex("synthOscChromatic", a));  // inst 2
        SY1000_default_replace("00", "23", "15", 1, GetJsonHex("synthOscPortamento", a));  // inst 2
        SY1000_default_replace("00", "23", "16", 1, GetJsonHex("synthOscPortaRate", a));  // inst 2
        SY1000_default_replace("00", "23", "17", 1, GetJsonHex("synthOscPortaMode", a));  // inst 2
        SY1000_default_replace("00", "23", "18", 1, GetJsonHex("synthOscHoldMode", a));  // inst 2
        SY1000_default_replace("00", "23", "19", 1, GetJsonHex("synthOscLowVeloCut", a));  // inst 2

        SY1000_default_replace("00", "21", "04", 1, convert_GP10_value(GetJsonHex("synthWaveType", a), "02", "1A"));  // inst 2
        //SY1000_default_replace("00", "21", "04", 1, GetJsonHex("synthWaveVolume", a));  // inst 2
        SY1000_default_replace("00", "21", "1F", 1, GetJsonHex("synthWaveCutoff", a));  // inst 2
        SY1000_default_replace("00", "21", "20", 1, GetJsonHex("synthWaveResonance", a));  // inst 2
        SY1000_default_replace("00", "21", "30", 1, GetJsonHex("synthWaveOctave", a));  // inst 2

        SY1000_default_replace("00", "29", "00", 1, GetJsonHex("polyfxPolyfxType", a));  // inst 2
        SY1000_default_replace("00", "29", "01", 1, GetJsonHex("polyfxGtrVol", a));  // inst 2
        SY1000_default_replace("00", "29", "02", 1, GetJsonHex("pfxdistGainBal", a));  // inst 2
        SY1000_default_replace("00", "29", "03", 1, GetJsonHex("pfxdistGain", a));  // inst 2
        if(GetJsonHex("polyfxPolyfxType", a).toHex().toUpper() == "00")  // inst 2
        {
            SY1000_default_replace("00", "29", "04", 1, GetJsonHex("pfxdistColor", a));  // inst 2
            SY1000_default_replace("00", "29", "05", 1, convert_GP10_value(GetJsonHex("pfxdistTone", a), "01", "59"));  // inst 2
            //SY1000_default_replace("00", "29", "00", 1, GetJsonHex("pfxdistLevel", a));  // inst 2
        };
        if(GetJsonHex("polyfxPolyfxType", a).toHex().toUpper() == "01")  // inst 2
        {
            SY1000_default_replace("00", "29", "04", 1, GetJsonHex("pfxcrysalColor", a));  // inst 2
            SY1000_default_replace("00", "29", "05", 1, convert_GP10_value(GetJsonHex("pfxcrysalTone", a), "01", "59"));  // inst 2
            //SY1000_default_replace("00", "29", "00", 1, GetJsonHex("pfxcrysalLevel", a));  // inst 2
        };
        if(GetJsonHex("polyfxPolyfxType", a).toHex().toUpper() == "02")  // inst 2
        {
            SY1000_default_replace("00", "29", "04", 1, GetJsonHex("pfxrichmodColor", a));  // inst 2
            SY1000_default_replace("00", "29", "05", 1, convert_GP10_value(GetJsonHex("pfxrichmodTone", a), "01", "59"));  // inst 2
            //SY1000_default_replace("00", "29", "00", 1, GetJsonHex("pfxrichmodLevel", a));  // inst 2
        };
        if(GetJsonHex("polyfxPolyfxType", a).toHex().toUpper() == "03")  // inst 2
        {
            SY1000_default_replace("00", "29", "04", 1, GetJsonHex("pfxslowpadColor", a));  // inst 2
            SY1000_default_replace("00", "29", "05", 1, convert_GP10_value(GetJsonHex("pfxslowpadTone", a), "01", "59"));  // inst 2
            //SY1000_default_replace("00", "29", "00", 1, GetJsonHex("pfxslowpadLevel", a));  // inst 2
        };
        SY1000_default_replace("00", "29", "06", 1, GetJsonHex("pfxtwahMode", a));  // inst 2
        SY1000_default_replace("00", "29", "07", 1, GetJsonHex("pfxtwahPolar", a));  // inst 2
        SY1000_default_replace("00", "29", "08", 1, GetJsonHex("pfxtwahSens", a));  // inst 2
        SY1000_default_replace("00", "29", "09", 1, GetJsonHex("pfxtwahFreq", a));  // inst 2
        SY1000_default_replace("00", "29", "0B", 1, GetJsonHex("pfxtwahPeak", a));  // inst 2
        //SY1000_default_replace("00", "29", "00", 1, GetJsonHex("pfxtwahVolume", a));  // inst 2
        SY1000_default_replace("00", "29", "0C", 1, GetJsonHex("pfxtwahToneType", a));  // inst 2
        SY1000_default_replace("00", "29", "0D", 1, GetJsonHex("pfxtwahCompSw", a));  // inst 2
        SY1000_default_replace("00", "29", "0E", 1, GetJsonHex("pfxtwahCompSus", a));  // inst 2
        SY1000_default_replace("00", "29", "0F", 1, GetJsonHex("pfxtwahCompAtk", a));  // inst 2
        SY1000_default_replace("00", "29", "0A", 1, GetJsonHex("pfxtwahDecay", a));  // inst 2

        SY1000_default_replace("00", "2A", "00", 1, GetJsonHex("alttuneOnOff", a));  // inst 2
        SY1000_default_replace("00", "2A", "01", 1, GetJsonHex("alttuneType", a));  // inst 2
        SY1000_default_replace("00", "2A", "02", 1, convert_GP10_value(GetJsonHex("alttuneShift1", a), "01", "58"));  // inst 2
        SY1000_default_replace("00", "2A", "03", 1, convert_GP10_value(GetJsonHex("alttuneShift2", a), "01", "58"));  // inst 2
        SY1000_default_replace("00", "2A", "04", 1, convert_GP10_value(GetJsonHex("alttuneShift3", a), "01", "58"));  // inst 2
        SY1000_default_replace("00", "2A", "05", 1, convert_GP10_value(GetJsonHex("alttuneShift4", a), "01", "58"));  // inst 2
        SY1000_default_replace("00", "2A", "06", 1, convert_GP10_value(GetJsonHex("alttuneShift5", a), "01", "58"));  // inst 2
        SY1000_default_replace("00", "2A", "07", 1, convert_GP10_value(GetJsonHex("alttuneShift6", a), "01", "58"));  // inst 2
        SY1000_default_replace("00", "2A", "08", 1, convert_GP10_value(GetJsonHex("alttuneFine1", a), "01", "59"));  // inst 2
        SY1000_default_replace("00", "2A", "09", 1, convert_GP10_value(GetJsonHex("alttuneFine2", a), "01", "59"));  // inst 2
        SY1000_default_replace("00", "2A", "0A", 1, convert_GP10_value(GetJsonHex("alttuneFine3", a), "01", "59"));  // inst 2
        SY1000_default_replace("00", "2A", "0B", 1, convert_GP10_value(GetJsonHex("alttuneFine4", a), "01", "59"));  // inst 2
        SY1000_default_replace("00", "2A", "0C", 1, convert_GP10_value(GetJsonHex("alttuneFine5", a), "01", "59"));  // inst 2
        SY1000_default_replace("00", "2A", "0D", 1, convert_GP10_value(GetJsonHex("alttuneFine6", a), "01", "59"));  // inst 2
        SY1000_default_replace("00", "2A", "0E", 1, GetJsonHex("str12OnOff", a));  // inst 2
        SY1000_default_replace("00", "2A", "0F", 1, GetJsonHex("str12Type", a));  // inst 2
        SY1000_default_replace("00", "2A", "10", 1, convert_GP10_value(GetJsonHex("str12PitchShft1", a), "01", "58"));  // inst 2
        SY1000_default_replace("00", "2A", "11", 1, convert_GP10_value(GetJsonHex("str12PitchShft2", a), "01", "58"));  // inst 2
        SY1000_default_replace("00", "2A", "12", 1, convert_GP10_value(GetJsonHex("str12PitchShft3", a), "01", "58"));  // inst 2
        SY1000_default_replace("00", "2A", "13", 1, convert_GP10_value(GetJsonHex("str12PitchShft4", a), "01", "58"));  // inst 2
        SY1000_default_replace("00", "2A", "14", 1, convert_GP10_value(GetJsonHex("str12PitchShft5", a), "01", "58"));  // inst 2
        SY1000_default_replace("00", "2A", "15", 1, convert_GP10_value(GetJsonHex("str12PitchShft6", a), "01", "58"));  // inst 2
        SY1000_default_replace("00", "2A", "16", 1, convert_GP10_value(GetJsonHex("str12PitchFine1", a), "01", "59"));  // inst 2
        SY1000_default_replace("00", "2A", "17", 1, convert_GP10_value(GetJsonHex("str12PitchFine2", a), "01", "59"));  // inst 2
        SY1000_default_replace("00", "2A", "18", 1, convert_GP10_value(GetJsonHex("str12PitchFine3", a), "01", "59"));  // inst 2
        SY1000_default_replace("00", "2A", "19", 1, convert_GP10_value(GetJsonHex("str12PitchFine4", a), "01", "59"));  // inst 2
        SY1000_default_replace("00", "2A", "1A", 1, convert_GP10_value(GetJsonHex("str12PitchFine5", a), "01", "59"));  // inst 2
        SY1000_default_replace("00", "2A", "1B", 1, convert_GP10_value(GetJsonHex("str12PitchFine6", a), "01", "59"));  // inst 2
        SY1000_default_replace("00", "2A", "1C", 1, GetJsonHex("str12Level1", a));  // inst 2
        SY1000_default_replace("00", "2A", "1D", 1, GetJsonHex("str12Level2", a));  // inst 2
        SY1000_default_replace("00", "2A", "1E", 1, GetJsonHex("str12Level3", a));  // inst 2
        SY1000_default_replace("00", "2A", "1F", 1, GetJsonHex("str12Level4", a));  // inst 2
        SY1000_default_replace("00", "2A", "20", 1, GetJsonHex("str12Level5", a));  // inst 2
        SY1000_default_replace("00", "2A", "21", 1, GetJsonHex("str12Level6", a));  // inst 2
        SY1000_default_replace("00", "2A", "22", 1, GetJsonHex("str12Delay1", a));  // inst 2
        SY1000_default_replace("00", "2A", "23", 1, GetJsonHex("str12Delay2", a));  // inst 2
        SY1000_default_replace("00", "2A", "24", 1, GetJsonHex("str12Delay3", a));  // inst 2
        SY1000_default_replace("00", "2A", "25", 1, GetJsonHex("str12Delay4", a));  // inst 2
        SY1000_default_replace("00", "2A", "26", 1, GetJsonHex("str12Delay5", a));  // inst 2
        SY1000_default_replace("00", "2A", "27", 1, GetJsonHex("str12Delay6", a));  // inst 2
        SY1000_default_replace("00", "2A", "28", 1, GetJsonHex("strbendOnOff", a));  // inst 2
        SY1000_default_replace("00", "2A", "29", 1, convert_GP10_value(GetJsonHex("strbendDepth1", a), "01", "58"));  // inst 2
        SY1000_default_replace("00", "2A", "2A", 1, convert_GP10_value(GetJsonHex("strbendDepth2", a), "01", "58"));  // inst 2
        SY1000_default_replace("00", "2A", "2B", 1, convert_GP10_value(GetJsonHex("strbendDepth3", a), "01", "58"));  // inst 2
        SY1000_default_replace("00", "2A", "2C", 1, convert_GP10_value(GetJsonHex("strbendDepth4", a), "01", "58"));  // inst 2
        SY1000_default_replace("00", "2A", "2D", 1, convert_GP10_value(GetJsonHex("strbendDepth5", a), "01", "58"));  // inst 2
        SY1000_default_replace("00", "2A", "2E", 1, convert_GP10_value(GetJsonHex("strbendDepth6", a), "01", "58"));  // inst 2
        SY1000_default_replace("00", "2A", "2F", 1, GetJsonHex("strbendControl", a));  // inst 2


        SY1000_default_replace("00", "30", "00", 1, GetJsonHex("egType", a));        // inst 3
        SY1000_default_replace("00", "30", "08", 1, GetJsonHex("egVolume", a));      // inst 3
        SY1000_default_replace("00", "30", "09", 1, GetJsonHex("egTone", a));        // inst 3
        SY1000_default_replace("00", "30", "02", 1, GetJsonHex("egFretlessToneType", a));     // inst 3
        SY1000_default_replace("00", "30", "03", 1, GetJsonHex("egFretlessSens", a));         // inst 3
        SY1000_default_replace("00", "30", "04", 1, GetJsonHex("egFretlessDepth", a));        // inst 3
        SY1000_default_replace("00", "30", "05", 1, GetJsonHex("egFretlessAttack", a));       // inst 3
        SY1000_default_replace("00", "30", "06", 1, GetJsonHex("egFretlessResonance", a));    // inst 3
        SY1000_default_replace("00", "30", "07", 1, GetJsonHex("egFretlessDirectLevel", a));  // inst 3
        if(GetJsonHex("egType", a).toHex().toUpper() == "00")
        {
            SY1000_default_replace("00", "30", "01", 1, GetJsonHex("egClaStPuSelect", a));    // inst 3
        }
        else if(GetJsonHex("egType", a).toHex().toUpper() == "01")
        {
            SY1000_default_replace("00", "30", "01", 1, GetJsonHex("egModStPuSelect", a));    // inst 3
        }
        else if(GetJsonHex("egType", a).toHex().toUpper() == "02")
        {
            SY1000_default_replace("00", "30", "01", 1, GetJsonHex("egTePuSelect", a));       // inst 3
        }
        else if(GetJsonHex("egType", a).toHex().toUpper() == "03")
        {
            SY1000_default_replace("00", "30", "01", 1, GetJsonHex("egLpPuSelect", a));       // inst 3
        }
        else if(GetJsonHex("egType", a).toHex().toUpper() == "04")
        {
            SY1000_default_replace("00", "30", "01", 1, GetJsonHex("egP90PuSelect", a));      // inst 3
        }
        else if(GetJsonHex("egType", a).toHex().toUpper() == "05")
        {
            SY1000_default_replace("00", "30", "01", 1, GetJsonHex("eg335PuSelect", a));      // inst 3
        }
        else if(GetJsonHex("egType", a).toHex().toUpper() == "06")
        {
            SY1000_default_replace("00", "30", "01", 1, GetJsonHex("egL4PuSelect", a));       // inst 3
        }
        else if(GetJsonHex("egType", a).toHex().toUpper() == "07")
        {
            SY1000_default_replace("00", "30", "01", 1, GetJsonHex("egRickPuSelect", a));     // inst 3
        }
        else if(GetJsonHex("egType", a).toHex().toUpper() == "08")
        {
            SY1000_default_replace("00", "30", "01", 1, GetJsonHex("egLipsPuSelect", a));     // inst 3
        }
        else if(GetJsonHex("egType", a).toHex().toUpper() == "09")
        {
            SY1000_default_replace("00", "30", "01", 1, GetJsonHex("egAdvSinglePuSelect", a)); // inst 3
        }
        else if(GetJsonHex("egType", a).toHex().toUpper() == "0A")
        {
            SY1000_default_replace("00", "30", "01", 1, GetJsonHex("egAdvHumPuSelect", a));   // inst 3
        };
        SY1000_default_replace("00", "31", "00", 1, GetJsonHex("acType", a));                 // inst 3
        SY1000_default_replace("00", "31", "0A", 1, GetJsonHex("acVolume", a));               // inst 3
        SY1000_default_replace("00", "31", "0B", 1, convert_GP10_value(GetJsonHex("acTone", a), "01", "59")); // inst 3
        SY1000_default_replace("00", "31", "01", 1, GetJsonHex("acBody", a));                 // inst 3
        //SY1000_default_replace("00", "31", "02", 1, GetJsonHex("acAttackLevel", a));          // inst 3
        SY1000_default_replace("00", "31", "04", 1, GetJsonHex("acSens", a));                 // inst 3
        SY1000_default_replace("00", "31", "05", 1, GetJsonHex("acColor", a));                // inst 3
        SY1000_default_replace("00", "31", "06", 1, GetJsonHex("acDecay", a));                // inst 3
        SY1000_default_replace("00", "31", "07", 1, GetJsonHex("acBuzz", a));                 // inst 3
        SY1000_default_replace("00", "31", "02", 1, GetJsonHex("acAttack", a));               // inst 3
        SY1000_default_replace("00", "31", "03", 1, GetJsonHex("acPUSelect", a));             // inst 3
        if(GetJsonHex("acType", a).toHex().toUpper() == "07")
        {
            SY1000_default_replace("00", "31", "08", 1, GetJsonHex("acBanjoResonance", a));   // inst 3
        }
        else
        {
            SY1000_default_replace("00", "31", "08", 1, GetJsonHex("acResonatorResonance", a)); // inst 3
        };
        SY1000_default_replace("00", "31", "09", 1, GetJsonHex("acSustain", a));               // inst 3
        SY1000_default_replace("00", "32", "00", 1, GetJsonHex("ebType", a));                  // inst 3
        SY1000_default_replace("00", "32", "09", 1, GetJsonHex("ebVolume", a));                // inst 3
        SY1000_default_replace("00", "32", "0A", 1, GetJsonHex("ebTone", a));                  // inst 3
        SY1000_default_replace("00", "32", "04", 1, GetJsonHex("ebSens", a));                  // inst 3
        SY1000_default_replace("00", "32", "06", 1, GetJsonHex("ebAttack", a));                // inst 3
        SY1000_default_replace("00", "32", "03", 1, GetJsonHex("ebToneType", a));              // inst 3
        SY1000_default_replace("00", "32", "05", 1, GetJsonHex("ebDepth", a));                 // inst 3
        SY1000_default_replace("00", "32", "07", 1, GetJsonHex("ebResonance", a));             // inst 3
        SY1000_default_replace("00", "32", "08", 1, GetJsonHex("ebDirectLevel", a));           // inst 3
        SY1000_default_replace("00", "32", "02", 1, GetJsonHex("ebRearVolume", a));            // inst 3
        SY1000_default_replace("00", "32", "01", 1, GetJsonHex("ebFrontVolume", a));           // inst 3
        if(GetJsonHex("mdlType", a).toHex().toUpper() == "03")
        {
            SY1000_default_replace("00", "2B", "01", 1, convert_GP10_value(GetJsonHex("synthSynthType", a), "02", "18")); // inst 3
        };
        SY1000_default_replace("00", "2F", "00", 1, GetJsonHex("synthGR300Mode", a));  // inst 3
        //SY1000_default_replace("00", "2F", "00", 1, GetJsonHex("synthGR300Volume", a));  // inst 3
        SY1000_default_replace("00", "2F", "01", 1, GetJsonHex("synthGR300CompSw", a));  // inst 3
        SY1000_default_replace("00", "2F", "02", 1, GetJsonHex("synthGR300Cutoff", a));  // inst 3
        SY1000_default_replace("00", "2F", "03", 1, GetJsonHex("synthGR300Resonance", a));  // inst 3
        SY1000_default_replace("00", "2F", "04", 1, GetJsonHex("synthGR300EnvModSw", a));  // inst 3
        SY1000_default_replace("00", "2F", "05", 1, GetJsonHex("synthGR300EnvModSens", a));  // inst 3
        SY1000_default_replace("00", "2F", "06", 1, GetJsonHex("synthGR300EnvModAtck", a));  // inst 3
        SY1000_default_replace("00", "2F", "07", 1, GetJsonHex("synthGR300PitchSw", a));  // inst 3
        SY1000_default_replace("00", "2F", "08", 1, convert_GP10_value(GetJsonHex("synthGR300PShiftA", a), "01", "61"));  // inst 3
        SY1000_default_replace("00", "2F", "09", 1, convert_GP10_value(GetJsonHex("synthGR300PFineA", a), "01", "59"));  // inst 3
        SY1000_default_replace("00", "2F", "0A", 1, convert_GP10_value(GetJsonHex("synthGR300PShiftB", a), "01", "61"));  // inst 3
        SY1000_default_replace("00", "2F", "0B", 1, convert_GP10_value(GetJsonHex("synthGR300PFineB", a), "01", "59"));  // inst 3
        SY1000_default_replace("00", "2F", "0C", 1, GetJsonHex("synthGR300PDuet", a));  // inst 3
        SY1000_default_replace("00", "2F", "0D", 1, GetJsonHex("synthGR300SweepSw", a));  // inst 3
        SY1000_default_replace("00", "2F", "0E", 1, GetJsonHex("synthGR300SweepRise", a));  // inst 3
        SY1000_default_replace("00", "2F", "0F", 1, GetJsonHex("synthGR300SweepFall", a));  // inst 3
        SY1000_default_replace("00", "2F", "10", 1, GetJsonHex("synthGR300VibratoSw", a));  // inst 3
        SY1000_default_replace("00", "2F", "11", 1, GetJsonHex("synthGR300VibRate", a));  // inst 3
        SY1000_default_replace("00", "2F", "12", 1, GetJsonHex("synthGR300VibDepth", a));  // inst 3

        SY1000_default_replace("00", "2E", "00", 1, GetJsonHex("synthOscMode", a));  // inst 3
        //SY1000_default_replace("00", "2E", "00", 1, GetJsonHex("synthOscVolume", a));  // inst 3
        SY1000_default_replace("00", "2E", "01", 1, GetJsonHex("synthOscWaveform1", a));  // inst 3
        SY1000_default_replace("00", "2E", "02", 1, convert_GP10_value(GetJsonHex("synthOscPitch1", a), "01", "58"));  // inst 3
        SY1000_default_replace("00", "2E", "03", 1, convert_GP10_value(GetJsonHex("synthOscPitchFine1", a), "01", "59"));  // inst 3
        SY1000_default_replace("00", "2E", "04", 1, GetJsonHex("synthOscPWWidth1", a));  // inst 3
        SY1000_default_replace("00", "2E", "05", 1, GetJsonHex("synthOscPWModRate1", a));  // inst 3
        SY1000_default_replace("00", "2E", "06", 1, GetJsonHex("synthOscPEnvAttck1", a));  // inst 3
        SY1000_default_replace("00", "2E", "07", 1, GetJsonHex("synthOscPEnvDecay1", a));  // inst 3
        SY1000_default_replace("00", "2E", "08", 1, GetJsonHex("synthOscPEnvDepth1", a));  // inst 3
        SY1000_default_replace("00", "2E", "09", 1, GetJsonHex("synthOscLevel1", a));  // inst 3
        SY1000_default_replace("00", "2E", "0A", 1, GetJsonHex("synthOscWaveform2", a));  // inst 3
        SY1000_default_replace("00", "2E", "0B", 1, convert_GP10_value(GetJsonHex("synthOscPitch2", a), "01", "58"));  // inst 3
        SY1000_default_replace("00", "2E", "0C", 1, convert_GP10_value(GetJsonHex("synthOscPitchFine2", a), "01", "59"));  // inst 3
        SY1000_default_replace("00", "2E", "0D", 1, GetJsonHex("synthOscPWWidth2", a));  // inst 3
        SY1000_default_replace("00", "2E", "0E", 1, GetJsonHex("synthOscPWModRate2", a));  // inst 3
        SY1000_default_replace("00", "2E", "0F", 1, GetJsonHex("synthOscPEnvAttck2", a));  // inst 3
        SY1000_default_replace("00", "2E", "10", 1, GetJsonHex("synthOscPEnvDecay2", a));  // inst 3
        SY1000_default_replace("00", "2E", "11", 1, GetJsonHex("synthOscPEnvDepth2", a));  // inst 3
        SY1000_default_replace("00", "2E", "12", 1, GetJsonHex("synthOscLevel2", a));  // inst 3
        if(GetJsonHex("synthOscFilterType", a).toHex().toUpper().toInt(&ok,16)>0)  // inst 3
        {
            SY1000_default_replace("00", "2E", "1A", 1, QByteArray::fromHex("01"));    // inst 3 filter sw
        };
        SY1000_default_replace("00", "2E", "1B", 1, convert_GP10_value(GetJsonHex("synthOscFilterType", a), "02", "19"));  // inst 3
        SY1000_default_replace("00", "2E", "1C", 1, GetJsonHex("synthOscFilterSlope", a));  // inst 3
        SY1000_default_replace("00", "2E", "1D", 1, GetJsonHex("synthOscFilterCutoff", a));  // inst 3
        SY1000_default_replace("00", "2E", "1E", 1, GetJsonHex("synthOscFltrCtOffFlw", a));  // inst 3
        SY1000_default_replace("00", "2E", "1F", 1, GetJsonHex("synthOscFltrReso", a));  // inst 3
        SY1000_default_replace("00", "2E", "20", 1, convert_GP10_value(GetJsonHex("synthOscFltrVeloSens", a), "01", "59"));  // inst 3
        SY1000_default_replace("00", "2E", "21", 1, GetJsonHex("synthOscFltrEnvAttck", a));  // inst 3
        SY1000_default_replace("00", "2E", "22", 1, GetJsonHex("synthOscFltrEnvDecay", a));  // inst 3
        SY1000_default_replace("00", "2E", "23", 1, GetJsonHex("synthOscFltrEnvSustn", a));  // inst 3
        SY1000_default_replace("00", "2E", "24", 1, GetJsonHex("synthOscFltrEnvRels", a));  // inst 3
        SY1000_default_replace("00", "2E", "25", 1, convert_GP10_value(GetJsonHex("synthOscFltrEnvDepth", a), "01", "59"));  // inst 3
        SY1000_default_replace("00", "2E", "26", 1, GetJsonHex("synthOscAmpVeloSens", a));  // inst 3
        SY1000_default_replace("00", "2E", "27", 1, GetJsonHex("synthOscAmpEnvAttck", a));  // inst 3
        SY1000_default_replace("00", "2E", "28", 1, GetJsonHex("synthOscAmpEnvDecay", a));  // inst 3
        SY1000_default_replace("00", "2E", "29", 1, GetJsonHex("synthOscAmpEnvSustn", a));  // inst 3
        SY1000_default_replace("00", "2E", "2A", 1, GetJsonHex("synthOscAmpEnvRels", a));  // inst 3
        SY1000_default_replace("00", "2E", "2E", 1, GetJsonHex("synthOscLFO1Shape", a));  // inst 3
        if(GetJsonHex("synthOscLFO1Rate", a).toHex().toUpper().toInt(&ok,16)>0)  // inst 3
        {
            SY1000_default_replace("00", "2E", "2D", 1, QByteArray::fromHex("01"));    // inst 3 lfo 1 sw
        };
        SY1000_default_replace("00", "2E", "2F", 1, convert_GP10_value(GetJsonHex("synthOscLFO1Rate", a), "02", "1D"));  // inst 3
        SY1000_default_replace("00", "2E", "30", 1, GetJsonHex("synthOscLFO1PtchDpt1", a));  // inst 3
        SY1000_default_replace("00", "2E", "31", 1, GetJsonHex("synthOscLFO1PtchDpt2", a));  // inst 3
        SY1000_default_replace("00", "2E", "32", 1, GetJsonHex("synthOscLFO1FltDepth", a));  // inst 3
        SY1000_default_replace("00", "2E", "33", 1, GetJsonHex("synthOscLFO1AmpDepth", a));  // inst 3
        SY1000_default_replace("00", "2E", "34", 1, GetJsonHex("synthOscLFO1DlyTime", a));  // inst 3
        SY1000_default_replace("00", "2E", "35", 1, GetJsonHex("synthOscLFO1FadeTime", a));  // inst 3
        SY1000_default_replace("00", "2E", "38", 1, GetJsonHex("synthOscLFO2Shape", a));  // inst 3
        if(GetJsonHex("synthOscLFO2Rate", a).toHex().toUpper().toInt(&ok,16)>0)  // inst 3
        {
            SY1000_default_replace("00", "2E", "37", 1, QByteArray::fromHex("01"));    // inst 3 lfo 2 sw
        };
        SY1000_default_replace("00", "2E", "39", 1, convert_GP10_value(GetJsonHex("synthOscLFO2Rate", a), "02", "1D"));  // inst 3
        SY1000_default_replace("00", "2E", "3A", 1, GetJsonHex("synthOscLFO2PtchDpt1", a));  // inst 3
        SY1000_default_replace("00", "2E", "3B", 1, GetJsonHex("synthOscLFO2PtchDpt2", a));  // inst 3
        SY1000_default_replace("00", "2E", "3C", 1, GetJsonHex("synthOscLFO2FltDepth", a));  // inst 3
        SY1000_default_replace("00", "2E", "3D", 1, GetJsonHex("synthOscLFO2AmpDepth", a));  // inst 3
        SY1000_default_replace("00", "2E", "3E", 1, GetJsonHex("synthOscLFO2DlyTime", a));  // inst 3
        SY1000_default_replace("00", "2E", "3F", 1, GetJsonHex("synthOscLFO2FadeTime", a));  // inst 3
        SY1000_default_replace("00", "2E", "13", 1, GetJsonHex("synthOscPolyMono", a));  // inst 3
        SY1000_default_replace("00", "2E", "14", 1, GetJsonHex("synthOscChromatic", a));  // inst 3
        SY1000_default_replace("00", "2E", "15", 1, GetJsonHex("synthOscPortamento", a));  // inst 3
        SY1000_default_replace("00", "2E", "16", 1, GetJsonHex("synthOscPortaRate", a));  // inst 3
        SY1000_default_replace("00", "2E", "17", 1, GetJsonHex("synthOscPortaMode", a));  // inst 3
        SY1000_default_replace("00", "2E", "18", 1, GetJsonHex("synthOscHoldMode", a));  // inst 3
        SY1000_default_replace("00", "2E", "19", 1, GetJsonHex("synthOscLowVeloCut", a));  // inst 3

        SY1000_default_replace("00", "2C", "04", 1, convert_GP10_value(GetJsonHex("synthWaveType", a), "02", "1A"));  // inst 3
        //SY1000_default_replace("00", "2C", "04", 1, GetJsonHex("synthWaveVolume", a));  // inst 3
        SY1000_default_replace("00", "2C", "1F", 1, GetJsonHex("synthWaveCutoff", a));  // inst 3
        SY1000_default_replace("00", "2C", "20", 1, GetJsonHex("synthWaveResonance", a));  // inst 3
        SY1000_default_replace("00", "2C", "30", 1, GetJsonHex("synthWaveOctave", a));  // inst 3

        SY1000_default_replace("00", "34", "00", 1, GetJsonHex("polyfxPolyfxType", a));  // inst 3
        SY1000_default_replace("00", "34", "01", 1, GetJsonHex("polyfxGtrVol", a));  // inst 3
        SY1000_default_replace("00", "34", "02", 1, GetJsonHex("pfxdistGainBal", a));  // inst 3
        SY1000_default_replace("00", "34", "03", 1, GetJsonHex("pfxdistGain", a));  // inst 3
        if(GetJsonHex("polyfxPolyfxType", a).toHex().toUpper() == "00")  // inst 3
        {
            SY1000_default_replace("00", "34", "04", 1, GetJsonHex("pfxdistColor", a));  // inst 3
            SY1000_default_replace("00", "34", "05", 1, convert_GP10_value(GetJsonHex("pfxdistTone", a), "01", "59"));  // inst 3
            //SY1000_default_replace("00", "34", "00", 1, GetJsonHex("pfxdistLevel", a));  // inst 3
        };
        if(GetJsonHex("polyfxPolyfxType", a).toHex().toUpper() == "01")  // inst 3
        {
            SY1000_default_replace("00", "34", "04", 1, GetJsonHex("pfxcrysalColor", a));  // inst 3
            SY1000_default_replace("00", "34", "05", 1, convert_GP10_value(GetJsonHex("pfxcrysalTone", a), "01", "59"));  // inst 3
            //SY1000_default_replace("00", "34", "00", 1, GetJsonHex("pfxcrysalLevel", a));  // inst 3
        };
        if(GetJsonHex("polyfxPolyfxType", a).toHex().toUpper() == "02")  // inst 3
        {
            SY1000_default_replace("00", "34", "04", 1, GetJsonHex("pfxrichmodColor", a));  // inst 3
            SY1000_default_replace("00", "34", "05", 1, convert_GP10_value(GetJsonHex("pfxrichmodTone", a), "01", "59"));  // inst 3
            //SY1000_default_replace("00", "34", "00", 1, GetJsonHex("pfxrichmodLevel", a));  // inst 3
        };
        if(GetJsonHex("polyfxPolyfxType", a).toHex().toUpper() == "03")  // inst 3
        {
            SY1000_default_replace("00", "34", "04", 1, GetJsonHex("pfxslowpadColor", a));  // inst 3
            SY1000_default_replace("00", "34", "05", 1, convert_GP10_value(GetJsonHex("pfxslowpadTone", a), "01", "59"));  // inst 3
            //SY1000_default_replace("00", "34", "00", 1, GetJsonHex("pfxslowpadLevel", a));  // inst 3
        };
        SY1000_default_replace("00", "34", "06", 1, GetJsonHex("pfxtwahMode", a));  // inst 3
        SY1000_default_replace("00", "34", "07", 1, GetJsonHex("pfxtwahPolar", a));  // inst 3
        SY1000_default_replace("00", "34", "08", 1, GetJsonHex("pfxtwahSens", a));  // inst 3
        SY1000_default_replace("00", "34", "09", 1, GetJsonHex("pfxtwahFreq", a));  // inst 3
        SY1000_default_replace("00", "34", "0B", 1, GetJsonHex("pfxtwahPeak", a));  // inst 3
        //SY1000_default_replace("00", "34", "00", 1, GetJsonHex("pfxtwahVolume", a));  // inst 3
        SY1000_default_replace("00", "34", "0C", 1, GetJsonHex("pfxtwahToneType", a));  // inst 3
        SY1000_default_replace("00", "34", "0D", 1, GetJsonHex("pfxtwahCompSw", a));  // inst 3
        SY1000_default_replace("00", "34", "0E", 1, GetJsonHex("pfxtwahCompSus", a));  // inst 3
        SY1000_default_replace("00", "34", "0F", 1, GetJsonHex("pfxtwahCompAtk", a));  // inst 3
        SY1000_default_replace("00", "34", "0A", 1, GetJsonHex("pfxtwahDecay", a));  // inst 3

        SY1000_default_replace("00", "35", "00", 1, GetJsonHex("alttuneOnOff", a));  // inst 3
        SY1000_default_replace("00", "35", "01", 1, GetJsonHex("alttuneType", a));  // inst 3
        SY1000_default_replace("00", "35", "02", 1, convert_GP10_value(GetJsonHex("alttuneShift1", a), "01", "58"));  // inst 3
        SY1000_default_replace("00", "35", "03", 1, convert_GP10_value(GetJsonHex("alttuneShift2", a), "01", "58"));  // inst 3
        SY1000_default_replace("00", "35", "04", 1, convert_GP10_value(GetJsonHex("alttuneShift3", a), "01", "58"));  // inst 3
        SY1000_default_replace("00", "35", "05", 1, convert_GP10_value(GetJsonHex("alttuneShift4", a), "01", "58"));  // inst 3
        SY1000_default_replace("00", "35", "06", 1, convert_GP10_value(GetJsonHex("alttuneShift5", a), "01", "58"));  // inst 3
        SY1000_default_replace("00", "35", "07", 1, convert_GP10_value(GetJsonHex("alttuneShift6", a), "01", "58"));  // inst 3
        SY1000_default_replace("00", "35", "08", 1, convert_GP10_value(GetJsonHex("alttuneFine1", a), "01", "59"));  // inst 3
        SY1000_default_replace("00", "35", "09", 1, convert_GP10_value(GetJsonHex("alttuneFine2", a), "01", "59"));  // inst 3
        SY1000_default_replace("00", "35", "0A", 1, convert_GP10_value(GetJsonHex("alttuneFine3", a), "01", "59"));  // inst 3
        SY1000_default_replace("00", "35", "0B", 1, convert_GP10_value(GetJsonHex("alttuneFine4", a), "01", "59"));  // inst 3
        SY1000_default_replace("00", "35", "0C", 1, convert_GP10_value(GetJsonHex("alttuneFine5", a), "01", "59"));  // inst 3
        SY1000_default_replace("00", "35", "0D", 1, convert_GP10_value(GetJsonHex("alttuneFine6", a), "01", "59"));  // inst 3
        SY1000_default_replace("00", "35", "0E", 1, GetJsonHex("str12OnOff", a));  // inst 3
        SY1000_default_replace("00", "35", "0F", 1, GetJsonHex("str12Type", a));  // inst 3
        SY1000_default_replace("00", "35", "10", 1, convert_GP10_value(GetJsonHex("str12PitchShft1", a), "01", "58"));  // inst 3
        SY1000_default_replace("00", "35", "11", 1, convert_GP10_value(GetJsonHex("str12PitchShft2", a), "01", "58"));  // inst 3
        SY1000_default_replace("00", "35", "12", 1, convert_GP10_value(GetJsonHex("str12PitchShft3", a), "01", "58"));  // inst 3
        SY1000_default_replace("00", "35", "13", 1, convert_GP10_value(GetJsonHex("str12PitchShft4", a), "01", "58"));  // inst 3
        SY1000_default_replace("00", "35", "14", 1, convert_GP10_value(GetJsonHex("str12PitchShft5", a), "01", "58"));  // inst 3
        SY1000_default_replace("00", "35", "15", 1, convert_GP10_value(GetJsonHex("str12PitchShft6", a), "01", "58"));  // inst 3
        SY1000_default_replace("00", "35", "16", 1, convert_GP10_value(GetJsonHex("str12PitchFine1", a), "01", "59"));  // inst 3
        SY1000_default_replace("00", "35", "17", 1, convert_GP10_value(GetJsonHex("str12PitchFine2", a), "01", "59"));  // inst 3
        SY1000_default_replace("00", "35", "18", 1, convert_GP10_value(GetJsonHex("str12PitchFine3", a), "01", "59"));  // inst 3
        SY1000_default_replace("00", "35", "19", 1, convert_GP10_value(GetJsonHex("str12PitchFine4", a), "01", "59"));  // inst 3
        SY1000_default_replace("00", "35", "1A", 1, convert_GP10_value(GetJsonHex("str12PitchFine5", a), "01", "59"));  // inst 3
        SY1000_default_replace("00", "35", "1B", 1, convert_GP10_value(GetJsonHex("str12PitchFine6", a), "01", "59"));  // inst 3
        SY1000_default_replace("00", "35", "1C", 1, GetJsonHex("str12Level1", a));  // inst 3
        SY1000_default_replace("00", "35", "1D", 1, GetJsonHex("str12Level2", a));  // inst 3
        SY1000_default_replace("00", "35", "1E", 1, GetJsonHex("str12Level3", a));  // inst 3
        SY1000_default_replace("00", "35", "1F", 1, GetJsonHex("str12Level4", a));  // inst 3
        SY1000_default_replace("00", "35", "20", 1, GetJsonHex("str12Level5", a));  // inst 3
        SY1000_default_replace("00", "35", "21", 1, GetJsonHex("str12Level6", a));  // inst 3
        SY1000_default_replace("00", "35", "22", 1, GetJsonHex("str12Delay1", a));  // inst 3
        SY1000_default_replace("00", "35", "23", 1, GetJsonHex("str12Delay2", a));  // inst 3
        SY1000_default_replace("00", "35", "24", 1, GetJsonHex("str12Delay3", a));  // inst 3
        SY1000_default_replace("00", "35", "25", 1, GetJsonHex("str12Delay4", a));  // inst 3
        SY1000_default_replace("00", "35", "26", 1, GetJsonHex("str12Delay5", a));  // inst 3
        SY1000_default_replace("00", "35", "27", 1, GetJsonHex("str12Delay6", a));  // inst 3
        SY1000_default_replace("00", "35", "28", 1, GetJsonHex("strbendOnOff", a));  // inst 3
        SY1000_default_replace("00", "35", "29", 1, convert_GP10_value(GetJsonHex("strbendDepth1", a), "01", "58"));  // inst 3
        SY1000_default_replace("00", "35", "2A", 1, convert_GP10_value(GetJsonHex("strbendDepth2", a), "01", "58"));  // inst 3
        SY1000_default_replace("00", "35", "2B", 1, convert_GP10_value(GetJsonHex("strbendDepth3", a), "01", "58"));  // inst 3
        SY1000_default_replace("00", "35", "2C", 1, convert_GP10_value(GetJsonHex("strbendDepth4", a), "01", "58"));  // inst 3
        SY1000_default_replace("00", "35", "2D", 1, convert_GP10_value(GetJsonHex("strbendDepth5", a), "01", "58"));  // inst 3
        SY1000_default_replace("00", "35", "2E", 1, convert_GP10_value(GetJsonHex("strbendDepth6", a), "01", "58"));  // inst 3
        SY1000_default_replace("00", "35", "2F", 1, GetJsonHex("strbendControl", a));  // inst 3

        SY1000_default_replace("00", "38", "00", 1, GetJsonHex("ampOnOff", a));
        SY1000_default_replace("00", "38", "01", 1, convert_GP10_value(GetJsonHex("ampType", a), "02", "1B"));
        SY1000_default_replace("00", "38", "02", 1, GetJsonHex("ampGain", a));
        SY1000_default_replace("00", "38", "03", 1, convert_GP10_value(GetJsonHex("ampTComp", a), "02", "1C"));
        SY1000_default_replace("00", "38", "05", 1, GetJsonHex("ampLevel", a));
        SY1000_default_replace("00", "38", "06", 1, GetJsonHex("ampBass", a));
        SY1000_default_replace("00", "38", "07", 1, GetJsonHex("ampMiddle", a));
        SY1000_default_replace("00", "38", "08", 1, GetJsonHex("ampTreble", a));
        SY1000_default_replace("00", "38", "09", 1, GetJsonHex("ampPresence", a));
        SY1000_default_replace("00", "38", "0A", 1, GetJsonHex("ampBright", a));
        SY1000_default_replace("00", "38", "0B", 1, GetJsonHex("ampGainSw", a));
        SY1000_default_replace("00", "38", "0C", 1, GetJsonHex("ampSoloSw", a));
        SY1000_default_replace("00", "38", "0D", 1, GetJsonHex("ampSoloLevel", a));
        SY1000_default_replace("00", "38", "13", 1, GetJsonHex("ampDirectLevel", a));
        SY1000_default_replace("00", "38", "0E", 1, GetJsonHex("ampSpeakerType", a));
        SY1000_default_replace("00", "38", "0F", 1, GetJsonHex("ampMicType", a));
        SY1000_default_replace("00", "38", "10", 1, GetJsonHex("ampMicDistance", a));
        SY1000_default_replace("00", "38", "11", 1, GetJsonHex("ampMicPosition", a));
        SY1000_default_replace("00", "38", "12", 1, GetJsonHex("ampMicLevel", a));

        SY1000_default_replace("00", "40", "00", 1, GetJsonHex("fxFxOnOff", a));
        if(GetJsonHex("fxFxType", a).toHex().toUpper() == "00")
        {
            SY1000_default_replace("00", "40", "00", 1, QByteArray::fromHex("00")); // switch off FX1
            SY1000_default_replace("00", "37", "00", 1, QByteArray::fromHex("01")); // switch on od/ds
        };
        SY1000_default_replace("00", "40", "01", 1, convert_GP10_value(GetJsonHex("fxFxType", a), "02", "1E"));
        SY1000_default_replace("00", "37", "01", 1, convert_GP10_value(GetJsonHex("fxFxoddsType", a), "02", "1F"));
        SY1000_default_replace("00", "37", "02", 1, GetJsonHex("fxFxoddsDrive", a));
        SY1000_default_replace("00", "37", "03", 1, convert_GP10_value(GetJsonHex("fxFxoddsTone", a), "01", "59"));
        SY1000_default_replace("00", "37", "04", 1, GetJsonHex("fxFxoddsLevel", a));
        SY1000_default_replace("00", "37", "05", 1, convert_GP10_value(GetJsonHex("fxFxoddsBottom", a), "01", "59"));
        SY1000_default_replace("00", "37", "06", 1, GetJsonHex("fxFxoddsDlevel", a));
        SY1000_default_replace("00", "37", "07", 1, GetJsonHex("fxFxoddsSoloSw", a));
        SY1000_default_replace("00", "37", "08", 1, GetJsonHex("fxFxoddsSoloLv", a));
        SY1000_default_replace("00", "57", "00", 1, GetJsonHex("fxFxPhaserType", a));
        SY1000_default_replace("00", "57", "01", 1, convert_GP10_value(GetJsonHex("fxFxPhaserRate", a), "02", "1D"));
        SY1000_default_replace("00", "57", "02", 1, GetJsonHex("fxFxPhaserDepth", a));
        SY1000_default_replace("00", "57", "03", 1, GetJsonHex("fxFxPhaserReso", a));
        SY1000_default_replace("00", "57", "04", 1, GetJsonHex("fxFxPhaserManual", a));
        SY1000_default_replace("00", "57", "05", 1, convert_GP10_value(GetJsonHex("fxFxPhaserStepRate", a), "02", "1D"));
        SY1000_default_replace("00", "57", "06", 1, GetJsonHex("fxFxPhaserElevel", a));
        SY1000_default_replace("00", "57", "07", 1, GetJsonHex("fxFxPhaserDlevel", a));
        SY1000_default_replace("00", "49", "00", 1, convert_GP10_value(GetJsonHex("fxFxFlngrRate", a), "02", "1D"));
        SY1000_default_replace("00", "49", "01", 1, GetJsonHex("fxFxFlngrDepth", a));
        SY1000_default_replace("00", "49", "02", 1, GetJsonHex("fxFxFlngrReso", a));
        SY1000_default_replace("00", "49", "03", 1, GetJsonHex("fxFxFlngrManual", a));
        SY1000_default_replace("00", "49", "04", 1, GetJsonHex("fxFxFlngrSeparat", a));
        SY1000_default_replace("00", "49", "05", 1, convert_GP10_value(GetJsonHex("fxFxFlngrLowCut", a), "02", "20"));
        SY1000_default_replace("00", "49", "07", 1, GetJsonHex("fxFxFlngrElevel", a));
        SY1000_default_replace("00", "49", "08", 1, GetJsonHex("fxFxFlngrDlevel", a));
        SY1000_default_replace("00", "63", "01", 1, convert_GP10_value(GetJsonHex("fxFxTrmlRate", a), "02", "1D"));
        SY1000_default_replace("00", "63", "02", 1, GetJsonHex("fxFxTrmlDepth", a));
        SY1000_default_replace("00", "63", "00", 1, GetJsonHex("fxFxTrmlWaveShape", a));
        SY1000_default_replace("00", "63", "03", 1, GetJsonHex("fxFxTrmlElevel", a));
        SY1000_default_replace("00", "54", "00", 1, GetJsonHex("fxFxPanType", a));
        SY1000_default_replace("00", "54", "02", 1, convert_GP10_value(GetJsonHex("fxFxPanRate", a), "02", "1D"));
        SY1000_default_replace("00", "54", "03", 1, GetJsonHex("fxFxPanDepth", a));
        SY1000_default_replace("00", "54", "01", 1, GetJsonHex("fxFxPanWaveShape", a));
        SY1000_default_replace("00", "54", "05", 1, GetJsonHex("fxFxPanMPosition_l", a));
        //SY1000_default_replace("00", "54", "00", 1, GetJsonHex("fxFxPanMPosition_h", a));
        SY1000_default_replace("00", "54", "04", 1, GetJsonHex("fxFxPanElevel", a));
        SY1000_default_replace("00", "5B", "00", 1, GetJsonHex("fxFxRotSpeedSelct", a));
        SY1000_default_replace("00", "5B", "01", 1, convert_GP10_value(GetJsonHex("fxFxRotRateSlow", a), "02", "1D"));
        SY1000_default_replace("00", "5B", "02", 1, convert_GP10_value(GetJsonHex("fxFxRotRateFast", a), "02", "1D"));
        SY1000_default_replace("00", "5B", "03", 1, GetJsonHex("fxFxRotDepth", a));
        SY1000_default_replace("00", "5B", "04", 1, GetJsonHex("fxFxRotRiseTime", a));
        SY1000_default_replace("00", "5B", "05", 1, GetJsonHex("fxFxRotFallTime", a));
        SY1000_default_replace("00", "5B", "06", 1, GetJsonHex("fxFxRotElevel", a));
        SY1000_default_replace("00", "44", "00", 1, convert_GP10_value(GetJsonHex("fxFxUniRate", a), "02", "1D"));
        SY1000_default_replace("00", "44", "01", 1, GetJsonHex("fxFxUniDepth", a));
        SY1000_default_replace("00", "44", "02", 1, GetJsonHex("fxFxUniElevel", a));
        SY1000_default_replace("00", "45", "01", 1, convert_GP10_value(GetJsonHex("fxFxCompType", a), "02", "22"));
        SY1000_default_replace("00", "45", "02", 1, GetJsonHex("fxFxCompSustain", a));
        SY1000_default_replace("00", "45", "03", 1, GetJsonHex("fxFxCompAttack", a));
        SY1000_default_replace("00", "45", "04", 1, GetJsonHex("fxFxCompLevel", a));
        SY1000_default_replace("00", "45", "06", 1, convert_GP10_value(GetJsonHex("fxFxCompTone", a), "01", "59"));
        SY1000_default_replace("00", "50", "00", 1, GetJsonHex("fxFxLmtrType", a));
        SY1000_default_replace("00", "50", "01", 1, GetJsonHex("fxFxLmtrThreshold", a));
        SY1000_default_replace("00", "50", "02", 1, GetJsonHex("fxFxLmtrRatio", a));
        SY1000_default_replace("00", "50", "03", 1, GetJsonHex("fxFxLmtrAttack", a));
        SY1000_default_replace("00", "50", "04", 1, GetJsonHex("fxFxLmtrRelease", a));
        SY1000_default_replace("00", "50", "05", 1, GetJsonHex("fxFxLmtrLevel", a));
        SY1000_default_replace("00", "55", "09", 1, GetJsonHex("fxFxEqLowCut", a));
        SY1000_default_replace("00", "55", "01", 1, convert_GP10_value(GetJsonHex("fxFxEqLowGain", a), "02", "21"));
        SY1000_default_replace("00", "55", "05", 1, convert_GP10_value(GetJsonHex("fxFxEqLowMidGain", a), "02", "21"));
        SY1000_default_replace("00", "55", "03", 1, GetJsonHex("fxFxEqLowMidFrq", a));
        SY1000_default_replace("00", "55", "04", 1, GetJsonHex("fxFxEqLowMidQ", a));
        SY1000_default_replace("00", "55", "08", 1, convert_GP10_value(GetJsonHex("fxFxEqHiMidGain", a), "02", "21"));
        SY1000_default_replace("00", "55", "06", 1, GetJsonHex("fxFxEqHiMidFrq", a));
        SY1000_default_replace("00", "55", "07", 1, GetJsonHex("fxFxEqHiMidQ", a));
        SY1000_default_replace("00", "55", "02", 1, convert_GP10_value(GetJsonHex("fxFxEqHiGain", a), "02", "21"));
        SY1000_default_replace("00", "55", "0A", 1, convert_GP10_value(GetJsonHex("fxFxEqHiCut", a), "02", "23"));
        SY1000_default_replace("00", "55", "0B", 1, convert_GP10_value(GetJsonHex("fxFxEqLevel", a), "02", "21"));
        SY1000_default_replace("00", "43", "01", 1, GetJsonHex("fxFxChorusMode", a));
        SY1000_default_replace("00", "43", "02", 1, convert_GP10_value(GetJsonHex("fxFxChorusRate", a), "02", "1D"));
        SY1000_default_replace("00", "43", "03", 1, GetJsonHex("fxFxChorusDepth", a));
        SY1000_default_replace("00", "43", "04", 1, GetJsonHex("fxFxChorusPreDly", a));
        SY1000_default_replace("00", "43", "07", 1, GetJsonHex("fxFxChorusLowCut", a));
        SY1000_default_replace("00", "43", "08", 1, convert_GP10_value(GetJsonHex("fxFxChorusHiCut", a), "02", "23"));
        SY1000_default_replace("00", "43", "05", 1, GetJsonHex("fxFxChorusElevel", a));
        SY1000_default_replace("00", "43", "17", 1, GetJsonHex("fxFxChorusDlevel", a));
        SY1000_default_replace("00", "58", "00", 1, GetJsonHex("fxFxPsVoice", a));
        SY1000_default_replace("00", "58", "04", 1, GetJsonHex("fxFxPsMode1", a));
        SY1000_default_replace("00", "58", "0C", 1, GetJsonHex("fxFxPsMode2", a));
        SY1000_default_replace("00", "58", "01", 1, convert_GP10_value(GetJsonHex("fxFxPsPitch1", a), "01", "58"));
        SY1000_default_replace("00", "58", "05", 1, convert_GP10_value(GetJsonHex("fxFxPsFine1", a), "01", "59"));
        SY1000_default_replace("00", "58", "02", 1, convert_GP10_value(GetJsonHex("fxFxPsPitch2", a), "01", "58"));
        SY1000_default_replace("00", "58", "0D", 1, convert_GP10_value(GetJsonHex("fxFxPsFine2", a), "01", "59"));
        SY1000_default_replace("00", "58", "0B", 1, GetJsonHex("fxFxPsFBack1", a));
        temp = QByteArray::fromHex("00");
        temp.append(GetJsonHex("fxFxPsPDelay1_l", a)).append(GetJsonHex("fxFxPsPDelay1_m", a)).append(GetJsonHex("fxFxPsPDelay1_h", a));
        SY1000_default_replace("00", "58", "06", 4, temp);
        temp = QByteArray::fromHex("00");
        temp.append(GetJsonHex("fxFxPsPDelay2_l", a)).append(GetJsonHex("fxFxPsPDelay2_m", a)).append(GetJsonHex("fxFxPsPDelay2_h", a));
        SY1000_default_replace("00", "58", "0E", 4, temp);
        SY1000_default_replace("00", "58", "0A", 1, GetJsonHex("fxFxPsElevel1", a));
        SY1000_default_replace("00", "58", "12", 1, GetJsonHex("fxFxPsElevel2", a));
        SY1000_default_replace("00", "58", "03", 1, GetJsonHex("fxFxPsDlevel", a));
        SY1000_default_replace("00", "4D", "00", 1, GetJsonHex("fxFxHrmVoice", a));
        SY1000_default_replace("00", "12", "42", 1, GetJsonHex("fxFxHrmMasterKey", a));
        SY1000_default_replace("00", "4D", "01", 1, GetJsonHex("fxFxHrmHarm1", a));
        SY1000_default_replace("00", "4D", "02", 1, GetJsonHex("fxFxHrmHarm2", a));
        SY1000_default_replace("00", "4D", "08", 1, GetJsonHex("fxFxHrmFback1", a));
        temp = QByteArray::fromHex("00");
        temp.append(GetJsonHex("fxFxHrmPdelay1_l", a)).append(GetJsonHex("fxFxHrmPdelay1_m", a)).append(GetJsonHex("fxFxHrmPdelay1_h", a));
        SY1000_default_replace("00", "4D", "04", 4, temp);
        temp = QByteArray::fromHex("00");
        temp.append(GetJsonHex("fxFxHrmPdelay2_l", a)).append(GetJsonHex("fxFxHrmPdelay2_m", a)).append(GetJsonHex("fxFxHrmPdelay2_h", a));
        SY1000_default_replace("00", "4D", "0B", 4, temp);;
        SY1000_default_replace("00", "4D", "03", 1, GetJsonHex("fxFxHrmElevel1", a));
        SY1000_default_replace("00", "4D", "0A", 1, GetJsonHex("fxFxHrmElevel2", a));
        SY1000_default_replace("00", "4D", "09", 1, GetJsonHex("fxFxHrmDlevel", a));
        SY1000_default_replace("00", "48", "00", 1, convert_GP10_value(GetJsonHex("fxFxDlyType", a), "02", "24"));
        recon = GetJsonHex("fxFxDlyTime_l", a).toHex().toUpper() + GetJsonHex("fxFxDlyTime_m", a).toHex().toUpper() + GetJsonHex("fxFxDlyTime_h", a).toHex().toUpper();
        int dt = (recon.mid(1,1) + recon.mid(3,1) + recon.mid(5,1)).toInt(&ok, 16);
        if(dt>1000){dt = 2016 - (dt-1000);};
        recon = QString::number(dt, 16);
        temp = QByteArray::fromHex(recon.toLatin1());
        SY1000_default_replace("00", "48", "02", 4, convert_to_4Byte(temp, 1));  //Fx delay time
        SY1000_default_replace("00", "48", "06", 1, GetJsonHex("fxFxDlyFeedback", a));
        SY1000_default_replace("00", "48", "07", 1, convert_GP10_value(GetJsonHex("fxFxDlyHighCut", a), "02", "23"));
        SY1000_default_replace("00", "48", "1F", 1, GetJsonHex("fxFxDlyPanTapTime", a));
        SY1000_default_replace("00", "48", "08", 1, GetJsonHex("fxFxDlyElevel", a));
        SY1000_default_replace("00", "48", "09", 1, GetJsonHex("fxFxDlyDlevel", a));
        SY1000_default_replace("00", "61", "00", 1, GetJsonHex("fxFxTwahMode", a));
        SY1000_default_replace("00", "61", "01", 1, GetJsonHex("fxFxTwahPolar", a));
        SY1000_default_replace("00", "61", "02", 1, GetJsonHex("fxFxTwahSens", a));
        SY1000_default_replace("00", "61", "03", 1, GetJsonHex("fxFxTwahFreq", a));
        SY1000_default_replace("00", "61", "04", 1, GetJsonHex("fxFxTwahPeak", a));
        SY1000_default_replace("00", "61", "06", 1, GetJsonHex("fxFxTwahElevel", a));
        SY1000_default_replace("00", "61", "07", 1, GetJsonHex("fxFxTwahDlevel", a));
        SY1000_default_replace("00", "56", "00", 1, convert_GP10_value(GetJsonHex("fxFxPbndPitch", a), "01", "58"));
        SY1000_default_replace("00", "56", "02", 4, convert_to_4Byte(GetJsonHex("fxFxPbndPosition", a), 10));
        SY1000_default_replace("00", "56", "01", 1, GetJsonHex("fxFxPbndElevel", a));
        SY1000_default_replace("00", "56", "06", 1, GetJsonHex("fxFxPbndDlevel", a));
        SY1000_default_replace("00", "66", "00", 1, GetJsonHex("wahOnOff", a));  // fx2 sw
        SY1000_default_replace("00", "66", "01", 1, QByteArray::fromHex("24"));  // set FX2 type to WAH
        SY1000_default_replace("01", "0B", "00", 1, GetJsonHex("wahType", a));
        SY1000_default_replace("01", "0B", "01", 1, GetJsonHex("wahElevel", a));
        SY1000_default_replace("01", "0B", "02", 4, convert_to_4Byte(GetJsonHex("wahPdlPosition", a), 10));
        SY1000_default_replace("01", "0B", "06", 4, convert_to_4Byte(GetJsonHex("wahPedalMin", a), 10));
        SY1000_default_replace("01", "0B", "0A", 4, convert_to_4Byte(GetJsonHex("wahPedalMax", a), 10));
        SY1000_default_replace("01", "0B", "0E", 1, GetJsonHex("wahDlevel", a));
        SY1000_default_replace("00", "3F", "00", 1, GetJsonHex("chorusOnOff", a));
        SY1000_default_replace("00", "3F", "01", 1, GetJsonHex("chorusMode", a));
        SY1000_default_replace("00", "3F", "02", 1, convert_GP10_value(GetJsonHex("chorusRate", a), "02", "1D"));
        SY1000_default_replace("00", "3F", "03", 1, GetJsonHex("chorusDepth", a));
        SY1000_default_replace("00", "3F", "04", 1, GetJsonHex("chorusPreDelay", a));
        SY1000_default_replace("00", "3F", "07", 1, GetJsonHex("chorusLowCut", a));
        SY1000_default_replace("00", "3F", "08", 1, convert_GP10_value(GetJsonHex("chorusHiCut", a), "02", "23"));
        SY1000_default_replace("00", "3F", "05", 1, GetJsonHex("chorusElevel", a));
        SY1000_default_replace("00", "3F", "17", 1, GetJsonHex("chorusDlevel", a));
        SY1000_default_replace("00", "3E", "00", 1, GetJsonHex("delayOnOff", a));
        SY1000_default_replace("00", "3E", "01", 1, GetJsonHex("delayType", a));
        recon = GetJsonHex("delayTime_l", a).toHex().toUpper() + GetJsonHex("delayTime_m", a).toHex().toUpper() + GetJsonHex("delayTime_h", a).toHex().toUpper();
        dt = (recon.mid(1,1) + recon.mid(3,1) + recon.mid(5,1)).toInt(&ok, 16);
        if(dt>2000){dt = 2016 - (dt-2000);};
        recon = QString::number(dt, 16);
        temp = QByteArray::fromHex(recon.toLatin1());
        SY1000_default_replace("00", "3E", "02", 4, convert_to_4Byte(temp, 1));  // delay time
        SY1000_default_replace("00", "3E", "06", 1, GetJsonHex("delayFeedback", a));
        SY1000_default_replace("00", "3E", "07", 1, convert_GP10_value(GetJsonHex("delayHighCut", a), "02", "23"));
        SY1000_default_replace("00", "3E", "08", 1, GetJsonHex("delayElevel", a));
        SY1000_default_replace("00", "3E", "09", 1, GetJsonHex("delayDlevel", a));
        SY1000_default_replace("00", "3E", "1F", 1, GetJsonHex("delayPanTapTime", a));
        temp = QByteArray::fromHex("00");
        temp.append(GetJsonHex("delayD1Time_l", a)).append(GetJsonHex("delayD1Time_m", a)).append(GetJsonHex("delayD1Time_h", a));
        SY1000_default_replace("00", "3E", "0A", 4, temp);        
        recon = GetJsonHex("delayD1Time_l", a).toHex().toUpper() + GetJsonHex("delayD1Time_m", a).toHex().toUpper() + GetJsonHex("delayD1Time_h", a).toHex().toUpper();
        dt = (recon.mid(1,1) + recon.mid(3,1) + recon.mid(5,1)).toInt(&ok, 16);
        if(dt>1000){dt = 2016 - (dt-1000);};
        recon = QString::number(dt, 16);
        temp = QByteArray::fromHex(recon.toLatin1());
        SY1000_default_replace("00", "3E", "0A", 4, convert_to_4Byte(temp, 1));
        recon = GetJsonHex("delayD2Time_l", a).toHex().toUpper() + GetJsonHex("delayD2Time_m", a).toHex().toUpper() + GetJsonHex("delayD2Time_h", a).toHex().toUpper();
        dt = (recon.mid(1,1) + recon.mid(3,1) + recon.mid(5,1)).toInt(&ok, 16);
        if(dt>1000){dt = 2016 - (dt-1000);};
        recon = QString::number(dt, 16);
        temp = QByteArray::fromHex(recon.toLatin1());
        SY1000_default_replace("00", "3E", "11", 4, convert_to_4Byte(temp, 1));
        SY1000_default_replace("00", "3E", "0E", 1, GetJsonHex("delayD1Fback", a));
        SY1000_default_replace("00", "3E", "15", 1, GetJsonHex("delayD2Fback", a));
        SY1000_default_replace("00", "3E", "0F", 1, convert_GP10_value(GetJsonHex("delayD1HiCut", a), "02", "23"));
        SY1000_default_replace("00", "3E", "16", 1, convert_GP10_value(GetJsonHex("delayD2HiCut", a), "02", "23"));
        SY1000_default_replace("00", "3E", "10", 1, GetJsonHex("delayD1Elevel", a));
        SY1000_default_replace("00", "3E", "17", 1, GetJsonHex("delayD2Elevel", a));
        SY1000_default_replace("00", "3E", "18", 1, GetJsonHex("delayModRate", a));
        SY1000_default_replace("00", "3E", "19", 1, GetJsonHex("delayModDepth", a));
        SY1000_default_replace("01", "32", "00", 1, GetJsonHex("reverbOnOff", a));
        SY1000_default_replace("01", "32", "01", 1, GetJsonHex("reverbType", a));
        SY1000_default_replace("01", "32", "02", 1, GetJsonHex("reverbTime", a));
        temp = QByteArray::fromHex("00");
        temp.append(GetJsonHex("reverbPreDelay_l", a)).append(GetJsonHex("reverbPreDelay_m", a)).append(GetJsonHex("reverbPreDelay_h", a));
        SY1000_default_replace("01", "32", "04", 4, temp);
        SY1000_default_replace("01", "32", "09", 1, GetJsonHex("reverbLowCut", a));
        SY1000_default_replace("01", "32", "0A", 1, convert_GP10_value(GetJsonHex("reverbHighCut", a), "02", "23"));
        SY1000_default_replace("01", "32", "0B", 1, GetJsonHex("reverbDensity", a));
        SY1000_default_replace("01", "32", "0D", 1, GetJsonHex("reverbSpringSns", a));
        SY1000_default_replace("01", "32", "08", 1, GetJsonHex("reverbElevel", a));
        SY1000_default_replace("01", "32", "0C", 1, GetJsonHex("reverbDlevel", a));
        SY1000_default_replace("00", "3A", "00", 1, GetJsonHex("eqOnOff", a));
        SY1000_default_replace("00", "3A", "09", 1, GetJsonHex("eqLowCut", a));
        SY1000_default_replace("00", "3A", "01", 1, convert_GP10_value(GetJsonHex("eqLowGain", a), "02", "21"));
        SY1000_default_replace("00", "3A", "05", 1, convert_GP10_value(GetJsonHex("eqLowMidGain", a), "02", "21"));
        SY1000_default_replace("00", "3A", "03", 1, GetJsonHex("eqLowMidFrq", a));
        SY1000_default_replace("00", "3A", "04", 1, GetJsonHex("eqLowMidQ", a));
        SY1000_default_replace("00", "3A", "08", 1, convert_GP10_value(GetJsonHex("eqHiMidGain", a), "02", "21"));
        SY1000_default_replace("00", "3A", "06", 1, GetJsonHex("eqHiMidFrq", a));
        SY1000_default_replace("00", "3A", "07", 1, GetJsonHex("eqHiMidQ", a));
        SY1000_default_replace("00", "3A", "02", 1, convert_GP10_value(GetJsonHex("eqHiGain", a), "02", "21"));
        SY1000_default_replace("00", "3A", "0A", 1, convert_GP10_value(GetJsonHex("eqHiCut", a), "02", "23"));
        SY1000_default_replace("00", "3A", "0B", 1, convert_GP10_value(GetJsonHex("eqLevel", a), "02", "21"));
        SY1000_default_replace("00", "12", "06", 4, convert_to_4Byte(GetJsonHex("fvMin", a), 10));
        SY1000_default_replace("00", "12", "0A", 4, convert_to_4Byte(GetJsonHex("fvMax", a), 10));
        SY1000_default_replace("00", "12", "12", 1, GetJsonHex("fvCurve", a));
        SY1000_default_replace("00", "12", "0E", 4, convert_to_4Byte(GetJsonHex("fvLevel", a), 10));
        SY1000_default_replace("00", "39", "00", 1, GetJsonHex("nsOnOff", a));
        SY1000_default_replace("00", "39", "01", 1, GetJsonHex("nsThreshold", a));
        SY1000_default_replace("00", "39", "02", 1, GetJsonHex("nsRelease", a));
        SY1000_default_replace("00", "00", "1C", 1, convert_GP10_value(GetJsonHex("ctrlCtl1Func", a), "02", "25"));
        SY1000_default_replace("00", "00", "1D", 1, GetJsonHex("ctrlCtl1Mode", a));
        SY1000_default_replace("00", "00", "1E", 1, convert_GP10_value(GetJsonHex("ctrlCtl2Func", a), "02", "25"));
        SY1000_default_replace("00", "00", "1F", 1, GetJsonHex("ctrlCtl2Mode", a));
        SY1000_default_replace("00", "00", "20", 1, convert_GP10_value(GetJsonHex("ctrlCtl3Func", a), "02", "25"));
        SY1000_default_replace("00", "00", "21", 1, GetJsonHex("ctrlCtl3Mode", a));
        SY1000_default_replace("00", "00", "22", 1, convert_GP10_value(GetJsonHex("ctrlCtl4Func", a), "02", "25"));
        SY1000_default_replace("00", "00", "23", 1, GetJsonHex("ctrlCtl4Mode", a));
        SY1000_default_replace("00", "00", "2D", 1, convert_GP10_value(GetJsonHex("ctrlGksw1Func", a), "02", "25"));
        SY1000_default_replace("00", "00", "2E", 1, GetJsonHex("ctrlGksw1Mode", a));
        SY1000_default_replace("00", "00", "2F", 1, convert_GP10_value(GetJsonHex("ctrlGksw2Func", a), "02", "25"));
        SY1000_default_replace("00", "00", "30", 1, GetJsonHex("ctrlGksw2Mode", a));
        SY1000_default_replace("00", "00", "24", 1, convert_GP10_value(GetJsonHex("ctrlExp1SwFunc", a), "02", "26")); // exp sw = ctl 5
        SY1000_default_replace("00", "00", "25", 1, GetJsonHex("ctrlExp1SwMode", a));
        SY1000_default_replace("00", "00", "2A", 1, convert_GP10_value(GetJsonHex("ctrlExp1offFunc", a), "02", "27"));
        //SY1000_default_replace("00", "00", "2A", 1, convert_GP10_value(GetJsonHex("ctrlExp1onFunc", a), "02", "27"));
        SY1000_default_replace("00", "00", "2B", 1, convert_GP10_value(GetJsonHex("ctrlExp2Func", a), "02", "27"));
        SY1000_default_replace("00", "00", "2C", 1, convert_GP10_value(GetJsonHex("ctrlGkvolFunc", a), "02", "27"));
        //SY1K_hex_replace(1024, 1, GetJsonHex("ctrlExpSwLed", a));
        //SY1K_hex_replace(1025, 1, GetJsonHex("ctrlCtl1SwLed", a));
        //SY1K_hex_replace(1026, 1, GetJsonHex("ctrlCtl2SwLed", a));
        SY1000_default_replace("00", "04", "00", 1, GetJsonHex("asgn1OnOff", a));
        SY1000_default_replace("00", "04", "40", 1, GetJsonHex("asgn2OnOff", a));
        SY1000_default_replace("00", "05", "00", 1, GetJsonHex("asgn3OnOff", a));
        SY1000_default_replace("00", "05", "40", 1, GetJsonHex("asgn4OnOff", a));
        SY1000_default_replace("00", "06", "00", 1, GetJsonHex("asgn5OnOff", a));
        SY1000_default_replace("00", "06", "40", 1, GetJsonHex("asgn6OnOff", a));
        SY1000_default_replace("00", "07", "00", 1, GetJsonHex("asgn7OnOff", a));
        SY1000_default_replace("00", "07", "40", 1, GetJsonHex("asgn8OnOff", a));
        temp.clear();
        temp.append(GetJsonHex("asgn1Target_l", a)).append(GetJsonHex("asgn1Target_m", a)).append(GetJsonHex("asgn1Target_h", a));
        SY1000_default_replace("00", "04", "01", 4, convert_Assign(temp));
        temp.clear();
        temp.append(GetJsonHex("asgn2Target_l", a)).append(GetJsonHex("asgn2Target_m", a)).append(GetJsonHex("asgn2Target_h", a));
        SY1000_default_replace("00", "04", "41", 4, convert_Assign(temp));
        temp.clear();
        temp.append(GetJsonHex("asgn3Target_l", a)).append(GetJsonHex("asgn3Target_m", a)).append(GetJsonHex("asgn3Target_h", a));
        SY1000_default_replace("00", "05", "01", 4, convert_Assign(temp));
        temp.clear();
        temp.append(GetJsonHex("asgn4Target_l", a)).append(GetJsonHex("asgn4Target_m", a)).append(GetJsonHex("asgn4Target_h", a));
        SY1000_default_replace("00", "05", "41", 4, convert_Assign(temp));
        temp.clear();
        temp.append(GetJsonHex("asgn5Target_l", a)).append(GetJsonHex("asgn5Target_m", a)).append(GetJsonHex("asgn5Target_h", a));
        SY1000_default_replace("00", "06", "01", 4, convert_Assign(temp));
        temp.clear();
        temp.append(GetJsonHex("asgn6Target_l", a)).append(GetJsonHex("asgn6Target_m", a)).append(GetJsonHex("asgn6Target_h", a));
        SY1000_default_replace("00", "06", "41", 4, convert_Assign(temp));
        temp.clear();
        temp.append(GetJsonHex("asgn7Target_l", a)).append(GetJsonHex("asgn7Target_m", a)).append(GetJsonHex("asgn7Target_h", a));
        SY1000_default_replace("00", "07", "01", 4, convert_Assign(temp));
        temp.clear();
        temp.append(GetJsonHex("asgn8Target_l", a)).append(GetJsonHex("asgn8Target_m", a)).append(GetJsonHex("asgn8Target_h", a));
        SY1000_default_replace("00", "07", "41", 4, convert_Assign(temp));

        temp.clear();
        temp.append(GetJsonHex("asgn1TargetMin_l", a)).append(GetJsonHex("asgn1TargetMin_m1", a)).append(GetJsonHex("asgn1TargetMin_m2", a)).append(GetJsonHex("asgn1TargetMin_h", a));
        SY1000_default_replace("00", "04", "05", 4, convert_MinMax(temp));
        temp.clear();
        temp.append(GetJsonHex("asgn2TargetMin_l", a)).append(GetJsonHex("asgn2TargetMin_m1", a)).append(GetJsonHex("asgn2TargetMin_m2", a)).append(GetJsonHex("asgn2TargetMin_h", a));
        SY1000_default_replace("00", "04", "45", 4, convert_MinMax(temp));
        temp.clear();
        temp.append(GetJsonHex("asgn3TargetMin_l", a)).append(GetJsonHex("asgn3TargetMin_m1", a)).append(GetJsonHex("asgn3TargetMin_m2", a)).append(GetJsonHex("asgn3TargetMin_h", a));
        SY1000_default_replace("00", "05", "05", 4, convert_MinMax(temp));
        temp.clear();
        temp.append(GetJsonHex("asgn4TargetMin_l", a)).append(GetJsonHex("asgn4TargetMin_m1", a)).append(GetJsonHex("asgn4TargetMin_m2", a)).append(GetJsonHex("asgn4TargetMin_h", a));
        SY1000_default_replace("00", "05", "45", 4, convert_MinMax(temp));
        temp.clear();
        temp.append(GetJsonHex("asgn5TargetMin_l", a)).append(GetJsonHex("asgn5TargetMin_m1", a)).append(GetJsonHex("asgn5TargetMin_m2", a)).append(GetJsonHex("asgn5TargetMin_h", a));
        SY1000_default_replace("00", "06", "05", 4, convert_MinMax(temp));
        temp.clear();
        temp.append(GetJsonHex("asgn6TargetMin_l", a)).append(GetJsonHex("asgn6TargetMin_m1", a)).append(GetJsonHex("asgn6TargetMin_m2", a)).append(GetJsonHex("asgn6TargetMin_h", a));
        SY1000_default_replace("00", "06", "45", 4, convert_MinMax(temp));
        temp.clear();
        temp.append(GetJsonHex("asgn7TargetMin_l", a)).append(GetJsonHex("asgn7TargetMin_m1", a)).append(GetJsonHex("asgn7TargetMin_m2", a)).append(GetJsonHex("asgn7TargetMin_h", a));
        SY1000_default_replace("00", "07", "05", 4, convert_MinMax(temp));
        temp.clear();
        temp.append(GetJsonHex("asgn8TargetMin_l", a)).append(GetJsonHex("asgn8TargetMin_m1", a)).append(GetJsonHex("asgn8TargetMin_m2", a)).append(GetJsonHex("asgn8TargetMin_h", a));
        SY1000_default_replace("00", "07", "45", 4, convert_MinMax(temp));

        temp.clear();
        temp.append(GetJsonHex("asgn1TargetMax_l", a)).append(GetJsonHex("asgn1TargetMax_m1", a)).append(GetJsonHex("asgn1TargetMax_m2", a)).append(GetJsonHex("asgn1TargetMax_h", a));
        SY1000_default_replace("00", "04", "09", 4, convert_MinMax(temp));
        temp.clear();
        temp.append(GetJsonHex("asgn2TargetMax_l", a)).append(GetJsonHex("asgn2TargetMax_m1", a)).append(GetJsonHex("asgn2TargetMax_m2", a)).append(GetJsonHex("asgn2TargetMax_h", a));
        SY1000_default_replace("00", "04", "49", 4, convert_MinMax(temp));
        temp.clear();
        temp.append(GetJsonHex("asgn3TargetMax_l", a)).append(GetJsonHex("asgn3TargetMax_m1", a)).append(GetJsonHex("asgn3TargetMax_m2", a)).append(GetJsonHex("asgn3TargetMax_h", a));
        SY1000_default_replace("00", "05", "09", 4, convert_MinMax(temp));
        temp.clear();
        temp.append(GetJsonHex("asgn4TargetMax_l", a)).append(GetJsonHex("asgn4TargetMax_m1", a)).append(GetJsonHex("asgn4TargetMax_m2", a)).append(GetJsonHex("asgn4TargetMax_h", a));
        SY1000_default_replace("00", "05", "49", 4, convert_MinMax(temp));
        temp.clear();
        temp.append(GetJsonHex("asgn5TargetMax_l", a)).append(GetJsonHex("asgn5TargetMax_m1", a)).append(GetJsonHex("asgn5TargetMax_m2", a)).append(GetJsonHex("asgn5TargetMax_h", a));
        SY1000_default_replace("00", "06", "09", 4, convert_MinMax(temp));
        temp.clear();
        temp.append(GetJsonHex("asgn6TargetMax_l", a)).append(GetJsonHex("asgn6TargetMax_m1", a)).append(GetJsonHex("asgn6TargetMax_m2", a)).append(GetJsonHex("asgn6TargetMax_h", a));
        SY1000_default_replace("00", "06", "49", 4, convert_MinMax(temp));
        temp.clear();
        temp.append(GetJsonHex("asgn7TargetMax_l", a)).append(GetJsonHex("asgn7TargetMax_m1", a)).append(GetJsonHex("asgn7TargetMax_m2", a)).append(GetJsonHex("asgn7TargetMax_h", a));
        SY1000_default_replace("00", "07", "09", 4, convert_MinMax(temp));
        temp.clear();
        temp.append(GetJsonHex("asgn8TargetMax_l", a)).append(GetJsonHex("asgn8TargetMax_m1", a)).append(GetJsonHex("asgn8TargetMax_m2", a)).append(GetJsonHex("asgn8TargetMax_h", a));
        SY1000_default_replace("00", "07", "49", 4, convert_MinMax(temp));

        SY1000_default_replace("00", "04", "0D", 1, convert_GP10_value(GetJsonHex("asgn1Source", a), "02", "28"));
        SY1000_default_replace("00", "04", "4D", 1, convert_GP10_value(GetJsonHex("asgn2Source", a), "02", "28"));
        SY1000_default_replace("00", "05", "0D", 1, convert_GP10_value(GetJsonHex("asgn3Source", a), "02", "28"));
        SY1000_default_replace("00", "05", "4D", 1, convert_GP10_value(GetJsonHex("asgn4Source", a), "02", "28"));
        SY1000_default_replace("00", "06", "0D", 1, convert_GP10_value(GetJsonHex("asgn5Source", a), "02", "28"));
        SY1000_default_replace("00", "06", "4D", 1, convert_GP10_value(GetJsonHex("asgn6Source", a), "02", "28"));
        SY1000_default_replace("00", "07", "0D", 1, convert_GP10_value(GetJsonHex("asgn7Source", a), "02", "28"));
        SY1000_default_replace("00", "07", "4D", 1, convert_GP10_value(GetJsonHex("asgn8Source", a), "02", "28"));
        SY1000_default_replace("00", "04", "0E", 1, convert_GP10_value(GetJsonHex("asgn1SrcMode", a), "02", "29"));
        SY1000_default_replace("00", "04", "4E", 1, convert_GP10_value(GetJsonHex("asgn2SrcMode", a), "02", "29"));
        SY1000_default_replace("00", "05", "0E", 1, convert_GP10_value(GetJsonHex("asgn3SrcMode", a), "02", "29"));
        SY1000_default_replace("00", "05", "4E", 1, convert_GP10_value(GetJsonHex("asgn4SrcMode", a), "02", "29"));
        SY1000_default_replace("00", "06", "0E", 1, convert_GP10_value(GetJsonHex("asgn5SrcMode", a), "02", "29"));
        SY1000_default_replace("00", "06", "4E", 1, convert_GP10_value(GetJsonHex("asgn6SrcMode", a), "02", "29"));
        SY1000_default_replace("00", "07", "0E", 1, convert_GP10_value(GetJsonHex("asgn7SrcMode", a), "02", "29"));
        SY1000_default_replace("00", "07", "4E", 1, convert_GP10_value(GetJsonHex("asgn8SrcMode", a), "02", "29"));
        SY1000_default_replace("00", "04", "0F", 4, convert_to_4Byte(GetJsonHex("asgn1SrangeMin", a), 128));
        SY1000_default_replace("00", "04", "4F", 4, convert_to_4Byte(GetJsonHex("asgn2SrangeMin", a), 128));
        SY1000_default_replace("00", "05", "0F", 4, convert_to_4Byte(GetJsonHex("asgn3SrangeMin", a), 128));
        SY1000_default_replace("00", "05", "4F", 4, convert_to_4Byte(GetJsonHex("asgn4SrangeMin", a), 128));
        SY1000_default_replace("00", "06", "0F", 4, convert_to_4Byte(GetJsonHex("asgn5SrangeMin", a), 128));
        SY1000_default_replace("00", "06", "4F", 4, convert_to_4Byte(GetJsonHex("asgn6SrangeMin", a), 128));
        SY1000_default_replace("00", "07", "0F", 4, convert_to_4Byte(GetJsonHex("asgn7SrangeMin", a), 128));
        SY1000_default_replace("00", "07", "4F", 4, convert_to_4Byte(GetJsonHex("asgn8SrangeMin", a), 128));
        SY1000_default_replace("00", "04", "13", 4, convert_to_4Byte(GetJsonHex("asgn1SrangeMax", a), 128));
        SY1000_default_replace("00", "04", "53", 4, convert_to_4Byte(GetJsonHex("asgn2SrangeMax", a), 128));
        SY1000_default_replace("00", "05", "13", 4, convert_to_4Byte(GetJsonHex("asgn3SrangeMax", a), 128));
        SY1000_default_replace("00", "05", "53", 4, convert_to_4Byte(GetJsonHex("asgn4SrangeMax", a), 128));
        SY1000_default_replace("00", "06", "13", 4, convert_to_4Byte(GetJsonHex("asgn5SrangeMax", a), 128));
        SY1000_default_replace("00", "06", "53", 4, convert_to_4Byte(GetJsonHex("asgn6SrangeMax", a), 128));
        SY1000_default_replace("00", "07", "13", 4, convert_to_4Byte(GetJsonHex("asgn7SrangeMax", a), 128));
        SY1000_default_replace("00", "07", "53", 4, convert_to_4Byte(GetJsonHex("asgn8SrangeMax", a), 128));

        SY1000_default_replace("00", "04", "19", 1, convert_GP10_value(GetJsonHex("asgn1WavPdlRate", a), "02", "1D"));
        SY1000_default_replace("00", "04", "59", 1, convert_GP10_value(GetJsonHex("asgn2WavPdlRate", a), "02", "1D"));
        SY1000_default_replace("00", "05", "19", 1, convert_GP10_value(GetJsonHex("asgn3WavPdlRate", a), "02", "1D"));
        SY1000_default_replace("00", "05", "59", 1, convert_GP10_value(GetJsonHex("asgn4WavPdlRate", a), "02", "1D"));
        SY1000_default_replace("00", "06", "19", 1, convert_GP10_value(GetJsonHex("asgn5WavPdlRate", a), "02", "1D"));
        SY1000_default_replace("00", "06", "59", 1, convert_GP10_value(GetJsonHex("asgn6WavPdlRate", a), "02", "1D"));
        SY1000_default_replace("00", "07", "19", 1, convert_GP10_value(GetJsonHex("asgn7WavPdlRate", a), "02", "1D"));
        SY1000_default_replace("00", "07", "59", 1, convert_GP10_value(GetJsonHex("asgn8WavPdlRate", a), "02", "1D"));
        SY1000_default_replace("00", "04", "1A", 1, GetJsonHex("asgn1WpdlWavFrm", a));
        SY1000_default_replace("00", "04", "5A", 1, GetJsonHex("asgn2WpdlWavFrm", a));
        SY1000_default_replace("00", "05", "1A", 1, GetJsonHex("asgn3WpdlWavFrm", a));
        SY1000_default_replace("00", "05", "5A", 1, GetJsonHex("asgn4WpdlWavFrm", a));
        SY1000_default_replace("00", "06", "1A", 1, GetJsonHex("asgn5WpdlWavFrm", a));
        SY1000_default_replace("00", "06", "5A", 1, GetJsonHex("asgn6WpdlWavFrm", a));
        SY1000_default_replace("00", "07", "1A", 1, GetJsonHex("asgn7WpdlWavFrm", a));
        SY1000_default_replace("00", "07", "5A", 1, GetJsonHex("asgn8WpdlWavFrm", a));

        SY1000_default_replace("00", "12", "27", 1, QByteArray::fromHex("00"));  // balancer 1 pan B left
        SY1000_default_replace("00", "12", "2A", 1, QByteArray::fromHex("64"));  // balancer 2 pan B right
        SY1000_default_replace("00", "3C", "00", 10, QByteArray::fromHex("0100000010141A501001")); // Delay 1 left
        SY1000_default_replace("00", "3D", "00", 10, QByteArray::fromHex("0100000020141A501001")); // Delay 2 right

        temp.clear();
        temp.append(QByteArray::fromHex("0001020306180A18180B18"));  // first part of chain
        temp.append(convert_GP10_value(GetJsonHex("fxChain1", a), "02", "2B")); // gp-10 chain order
        temp.append(convert_GP10_value(GetJsonHex("fxChain2", a), "02", "2B"));
        temp.append(convert_GP10_value(GetJsonHex("fxChain3", a), "02", "2B"));
        temp.append(convert_GP10_value(GetJsonHex("fxChain4", a), "02", "2B"));
        temp.append(convert_GP10_value(GetJsonHex("fxChain5", a), "02", "2B"));
        temp.append(convert_GP10_value(GetJsonHex("fxChain6", a), "02", "2B"));
        temp.append(convert_GP10_value(GetJsonHex("fxChain7", a), "02", "2B"));
        temp.append(convert_GP10_value(GetJsonHex("fxChain8", a), "02", "2B"));
        temp.append(convert_GP10_value(GetJsonHex("fxChain9", a), "02", "2B"));
        temp.append(convert_GP10_value(GetJsonHex("fxChain10", a), "02", "2B"));
        temp.append(convert_GP10_value(GetJsonHex("fxChain11", a), "02", "2B"));
        temp.append(QByteArray::fromHex("08100F11121314151617")); // last part of the chain
        temp.insert(temp.indexOf(QByteArray::fromHex("04")), QByteArray::fromHex("0100")); // insert comp & od/ds before FX1
        SY1000_default_replace("00", "12", "45", 34, temp);

        sysxIO->deBug("GP-10 conversion size = " + QString::number(this->SY1000_default.size(), 10));

        if(this->SY1000_default.size() == patchSize)
        {
            return(this->SY1000_default);
        }
        else
        {
            sysxIO->deBug("SHIT HAPPENED IN GP-10 FILE TRANSLATION");

            QMessageBox *msgBox = new QMessageBox();
            msgBox->setWindowTitle(QObject::tr("Corrupted data in Patch File"));
            msgBox->setIcon(QMessageBox::Warning);
            msgBox->setTextFormat(Qt::RichText);
            QString msgText;
            msgText.append("This patch file was not loaded due to a conversion error<br>");
            msgText.append("default/init settings or previous patch applied instead<br>");
            msgBox->setText(msgText);
            msgBox->setStandardButtons(QMessageBox::Ok);
            msgBox->exec();
            return(0);
        };
    }
    else
    {
        QMessageBox *msgBox = new QMessageBox();
        msgBox->setWindowTitle(QObject::tr("Unknown Device Patch File"));
        msgBox->setIcon(QMessageBox::Warning);
        msgBox->setTextFormat(Qt::RichText);
        QString msgText;
        msgText.append("This device type currrently not supported<br>");
        msgText.append("Device type recognised as "+device+"<br>");
        msgBox->setText(msgText);
        msgBox->setStandardButtons(QMessageBox::Ok);
        msgBox->exec();
        return(0);
    };
}

void tsl_GP10_translation::SY1000_default_replace(QString addr1, QString addr2, QString addr3, int size, QByteArray value)
{
    bool value_is_good = true;
    bool ok;
    uint rangeMax=127;
    uint rangeMin=0;
    QString hex1 = addr1;
    QString hex2 = addr2;
    QString hex3 = addr3;
    int hex = addr3.toInt(&ok, 16);
    for(int i=0; i<size; i++)
    {
        if(hex>127)
        {
            hex=hex-128;
            hex2=QString::number(addr2.toInt(&ok,16)+1,16);
            if(hex2.size()<2)hex2.prepend("0");
        };
        hex3 = QString::number(hex, 16).toUpper();
        if(hex3.size()<2)hex3.prepend("0");
        MidiTable *midiTable = MidiTable::Instance();

        QString valu = value.mid(i).toHex().toUpper();
        if(valu.size()<2)valu.prepend("0");
        uint val = valu.toUInt(&ok, 16);

        rangeMin = midiTable->getRangeMinimum("Structure", hex1, hex2, hex3);
        if((val < rangeMin) && size<2)
        {
            SysxIO *sysxIO = SysxIO::Instance();
            sysxIO->deBug("min address:" + hex1 + hex2 + addr3 + " to " + hex3 + " val=" + valu + " min=" + QString::number(rangeMin, 16).toUpper());
            value_is_good = false;
            QString num = QString::number(rangeMin,16).toUpper();
            if(num.size()<2) num.prepend("0");
            value.replace(i, 1, QByteArray::fromHex(num.toLatin1()));
        };

        rangeMax = midiTable->getRange("Structure", hex1, hex2, hex3);   
        if((val > rangeMax) && size<2)
        {
            SysxIO *sysxIO = SysxIO::Instance();
            sysxIO->deBug("max address:" + hex1 + hex2 + addr3 + " to " + hex3 + " val=" + valu + " max=" + QString::number(rangeMax, 16).toUpper());
            value_is_good = false;
            QString num = QString::number(rangeMax,16).toUpper();
            if(num.size()<2) num.prepend("0");
            value.replace(i, 1, QByteArray::fromHex(num.toLatin1()));
        };
        hex++;
    };

    if(!value.isEmpty() && !value.isNull())
    {
        int y = -1;
        int pos = -1;
        bool ok;

        for(int x=0; x<SY1000_default.size(); x++)
        {
            y = SY1000_default.indexOf(QByteArray::fromHex(QString("x/F0").toLatin1().data()), y+1);
            if(y==-1)break;
            QString SOF = this->SY1000_default.mid(y,1).toHex().toUpper();
            QString bytes = this->SY1000_default.mid(y+9,2).toHex().toUpper();

            if(SOF=="F0" && bytes == QString(addr1+addr2))
            {
                pos = y+12+addr3.toInt(&ok, 16);
                x=SY1000_default.size();
            };
        };
        if(pos > -1 && size == value.size() && value_is_good)
        {
            SY1000_default.replace(pos, size, value);
        }
        else
        {
            SysxIO *sysxIO = SysxIO::Instance();
            sysxIO->deBug("ERROR in tsl_GP10_translation::SY1000_default_replace * SY-1000 address:" + SY1000_default.mid(8,1).toHex() + addr1 + addr2 + addr3 + /*" pos=" + pos + " size=" + size +
              */  " value=" + value.toHex().toUpper() + " rangeMin=" + QString::number(rangeMin,16).toUpper() + " rangeMax=" + QString::number(rangeMax,16).toUpper());
        };
    };
}

QByteArray tsl_GP10_translation::convert_to_2Byte(QByteArray value, double multiplier)
{
    bool ok;
    int val = value.toHex().toUpper().toInt(&ok, 16);
    val = val*multiplier;
    QString index = "0" + QString::number(val, 16);
    QByteArray new_value;
    new_value.append(QByteArray::fromHex(("0" + index.mid(index.size()-2, 1)).toLatin1()));
    new_value.append(QByteArray::fromHex(("0" + index.mid(index.size()-1, 1)).toLatin1()));
    return new_value;
}

QByteArray tsl_GP10_translation::convert_to_4Byte(QByteArray value, double multiplier)
{
    bool ok;
    int val = value.toHex().toUpper().toInt(&ok, 16);
    val = val*multiplier;
    QString index = "000" + QString::number(val, 16);
    QByteArray new_value;
    new_value.append(QByteArray::fromHex(("0" + index.mid(index.size()-4, 1)).toLatin1()));
    new_value.append(QByteArray::fromHex(("0" + index.mid(index.size()-3, 1)).toLatin1()));
    new_value.append(QByteArray::fromHex(("0" + index.mid(index.size()-2, 1)).toLatin1()));
    new_value.append(QByteArray::fromHex(("0" + index.mid(index.size()-1, 1)).toLatin1()));
    return new_value;
}

QByteArray tsl_GP10_translation::convert_GP10_value(QByteArray value, QString hex2, QString hex3)
{
    bool ok;
    int v = QString(value.toHex()).toInt(&ok, 16);
    MidiTable *midiTable = MidiTable::Instance();
    Midi items;
    items = midiTable->getMidiMap("Tables", "00", hex2, hex3);
    QString desc = items.level.at(v).desc;
    QByteArray value2 = QByteArray::fromHex(desc.toLatin1().toUpper());
    return value2;
}

QByteArray tsl_GP10_translation::convert_MinMax(QByteArray value)
{
    return value;
}
