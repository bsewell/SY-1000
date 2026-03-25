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
#include "tsl_VG99_VB99_translation.h"
#include "fileDialog.h"
#include "SysxIO.h"
#include "globalVariables.h"
#include "MidiTable.h"
#include <QJsonArray>
#include <QRegularExpression>
#include "Preferences.h"
#include <QDebug>

tsl_VG99_VB99_translation::tsl_VG99_VB99_translation(QByteArray patch, QByteArray default_hex, int mode, QString filename)
{
    Q_UNUSED(default_hex);
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        bass_mode = true;
    }
    else
    {
        bass_mode = false;
    };
    Q_UNUSED(mode);
    this->data = patch;
    this->fileName = filename;
    QFile file(":/vg99_default.syx");
    if (file.open(QIODevice::ReadOnly))
    {
        this->VG99_default = file.readAll();
        file.close();
    };

    QFile dfile(":/default_guitar.syx");
    if (dfile.open(QIODevice::ReadOnly))
    {
        this->SY1000_default = dfile.readAll();
        dfile.close();
    };

    if(this->data.mid(0, 50).toHex().toUpper().contains("410000001D12")) // if a VB-99 patch
    {
        //qDebug() << " about to convert VB99 to VG99";
        loadMidiMap();
        this->data = convert_VB99mid_to_VG99default(0);
    }
    else if(filename.contains(".g9l"))
    {
        convert_VG99g9l_to_VG99default();
    }
    else
    {
        convert_VG99mid_to_VG99default(0);
    };
    // convertFromVG99default_to_SY1000default(0);
}

tsl_VG99_VB99_translation::~tsl_VG99_VB99_translation()
{

}

void tsl_VG99_VB99_translation::patchIndex(int listIndex)
{
    Q_UNUSED(listIndex);
    SysxIO *sysxIO = SysxIO::Instance();
    this->index=sysxIO->patchListValue;
}

QByteArray tsl_VG99_VB99_translation::copyVG99(QString addr1, QString addr2, int size)
{
    QByteArray value;
    int y = -1;
    bool ok;
    for(int x=0; x<data.size(); x++)
    {
        y = this->data.indexOf(QByteArray::fromHex(QString("F0").toLatin1().data()),y+1);
        if(y==-1)break;
        //qDebug() << "y=" << y;
        if(this->data.mid(y+9,1) == QByteArray::fromHex(addr1.toLatin1().data()))
        {
            value.append(this->data.mid((y+11+addr2.toInt(&ok, 16)), size));
            //qDebug() << "copyVG99 x=" << x << " y=" << y << " value=" << value.data();
            x=data.size();
        };
    };
    return value;
}

QByteArray tsl_VG99_VB99_translation::copySY1000(QString addr1, QString addr2, QString addr3, int size)
{
    QByteArray value;
    int y = -1;
    bool ok;
    for(int x=0; x<data.size(); x++)
    {
        y = this->data.indexOf(QByteArray::fromHex(QString("F0").toLatin1().data()),y+1);
        if(y==-1)break;
        //qDebug() << "y=" << y;
        if(this->data.mid(y+9,1) == QByteArray::fromHex(QString(addr1+addr2).toLatin1().data()))
        {
            value.append(this->data.mid((y+11+addr3.toInt(&ok, 16)), size));
            //qDebug() << "copyVG99 x=" << x << " y=" << y << " value=" << value.data();
            x=data.size();
        };
    };
    return value;
}

QByteArray tsl_VG99_VB99_translation::copyVBmid(QString addr1, QString addr2, int size)
{
    QByteArray value;
    int y = -1;
    int z = -1;
    bool ok;
    for(int x=0; x<data.size(); x++)
    {
        y = this->data.indexOf(QByteArray::fromHex(QString("F0").toLatin1().data()),y+1);
        //z = this->data.indexOf(QByteArray::fromHex(QString("F0").toLatin1().data()),y+1);
        if(y==-1)break; // ******* reached end of file. ********

        if(this->data.mid(y+10,1) == QByteArray::fromHex(addr1.toLatin1().data()) && this->data.mid(y+11,1) == QByteArray::fromHex(addr2.toLatin1().data()))
        { // ************ if address1 and address2 match ************
            value.append(this->data.mid(y+12), size);
            //qDebug() << "copyVBmid x=" << x << " y=" << y << " value=" << value.toHex().toUpper() << " matching addr1 and addr2";
            x=data.size();
        }
    };
    y = -1;
    //z = -1;

    if(value.isEmpty())
    {
        for(int x=0; x<data.size(); x++)
        {
            y = this->data.indexOf(QByteArray::fromHex(QString("F0").toLatin1().data()),y+1);
            z = this->data.indexOf(QByteArray::fromHex(QString("F0").toLatin1().data()),y+14);

            int adrNext = this->data.mid(z+10,2).toHex().toUpper().toInt(&ok,16);
            int adr2 = (addr2).toInt(&ok,16)-this->data.mid(y+11,1).toHex().toUpper().toInt(&ok,16);
            int address = (addr1+addr2).toInt(&ok,16);

            if(this->data.mid(y+10,1) == QByteArray::fromHex(addr1.toLatin1().data()) && adrNext>address)
            { // ******* if address1 match and address2 is less than address in next block. ********
                value.append(this->data.mid((y+12+adr2), size));
                //qDebug() << "adr2=" << adr2 << "address=" << QString(addr1+addr2+"("+QString::number(address)+")") << " adrNext=" << QString(this->data.mid(z+10,2).toHex().toUpper()+"("+QString::number(adrNext)+")");
                //qDebug() << "copyVBmid x=" << x << " y=" << y << "z=" << z << " value=" << value.toHex().toUpper() << " addr1&2" << addr1 << addr2 << " this->data.mid(z+8,4)=" << this->data.mid(z+8,4).toHex().toUpper();
                x=data.size();
            }
            else if(x==data.size()-1 && value.size()<1)
            {
                for(int y=0; y<size; y++)
                {
                    value.append(QByteArray::fromHex("00"));
                };
                qDebug() << " missed address in copyVBmid() this->data.mid(z+10,2)=" << this->data.mid(z+10,2).toHex().toUpper();
            };
        };
    };
    //qDebug() << "copyVBmid=" << value.toHex().toUpper() << "at address:" << addr1 << addr2;
    return value;
}

void tsl_VG99_VB99_translation::SY1000_default_replace(QString addr1, QString addr2, QString addr3, int size, QByteArray value)
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
        rangeMax = midiTable->getRange("Structure", hex1, hex2, hex3);
        uint val = value.at(i);
        QString valu = QString::number(val, 16).toUpper();
        if(valu.size()<2)valu.prepend("0");
        if((val > rangeMax) && size<2)
        {
            // qDebug() << "max address:" << hex1 << hex2 << addr3 << "to" << hex3 << " val=" << valu << " max=" << QString::number(rangeMax, 16).data()->toUpper();
            value_is_good = false;
            QString num = QString::number(rangeMax,16).toUpper();
            if(num.size()<2) num.prepend("0");
            value.replace(i, 1, QByteArray::fromHex(num.toLatin1()));
        };
        rangeMin = midiTable->getRangeMinimum("Structure", hex1, hex2, hex3);
        if((val < rangeMin) && size<2)
        {
            qDebug() << "SY1000_default_replace min address:" << hex1 << hex2 << addr3 << "to" << hex3 << " val=" << valu << " min=" << QString::number(rangeMin, 16).data()->toUpper();
            value_is_good = false;
            QString num = QString::number(rangeMin,16).toUpper();
            if(num.size()<2) num.prepend("0");
            value.replace(i, 1, QByteArray::fromHex(num.toLatin1()));
        };
        hex++;
    };
    //qDebug() << "address:" << hex1 << hex2 << hex3 <<  "value:" << value.toHex().toUpper();
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
            qDebug() << "ERROR in tsl_VG99_VB99_translation::SY1000_default_replace * sy1000 address:" << SY1000_default.mid(8,1).toHex() << addr1 << addr2 << addr3 << "pos=" << pos << " size=" << size <<
                        " value=" << value.toHex().toUpper() << " rangeMax=" << rangeMax << " rangeMin=" << rangeMin;
        };
    };
}

void tsl_VG99_VB99_translation::VG99_default_replace(QString addr2, QString addr3, int size, QByteArray value)
{
    bool value_is_good = true;
    bool ok;
    int rangeMax=127;
    int rangeMin=0;
    QString hex1 = "00";
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
        rangeMax = getRange("Structure", hex1, hex2, hex3);
        //qDebug() << "before" << i << hex1 << hex2 << hex3 << "range:" << rangeMax;
        int val = value.mid(i, 1).toInt(&ok, 16);//value.at(i);
        QString valu = QString::number(val, 16).toUpper();
        if(valu.size()<2)valu.prepend("0");
        if(val > rangeMax && rangeMax!=-1)
        {
            qDebug() << "max address:" << hex2 << addr3 << "to" << hex3 << " val=" << valu << " max=" << QString::number(rangeMax, 16).toUpper();
            value_is_good = false;
            QString num = QString::number(rangeMax,16).toUpper();
            if(num.size()<2) num.prepend("0");
            value.replace(i, 1, QByteArray::fromHex(num.toLatin1()));
        };
        rangeMin = getRangeMinimum("Structure", hex1, hex2, hex3);
        if(val < rangeMin && rangeMin!=-1)
        {
            qDebug() << "VG99_default_replace min address:" << hex2 << addr3 << "to" << hex3 << " val=" << valu << " min=" << QString::number(rangeMin, 16).toUpper();;
            value_is_good = false;
            QString num = QString::number(rangeMin,16).toUpper();
            if(num.size()<2) num.prepend("0");
            value.replace(i, 1, QByteArray::fromHex(num.toLatin1()));
        };
        hex++;
    };
    //qDebug() << "address:" << hex2 << hex3 <<  "value:" << value.toHex().toUpper();
    if(!value.isEmpty() && !value.isNull())
    {
        int y = -1;
        int z = -1;
        int pos = -1;
        bool ok;

        for(int x=0; x<VG99_default.size(); x++)
        {
            y = VG99_default.indexOf(QByteArray::fromHex("F0"), y+1);          // looking for a sysx header start byte.
            z = VG99_default.indexOf(QByteArray::fromHex("F0"), y+14);         // looking for sysx header start byte of next packet ahead.
            if(y==-1)break;                                                    // break if reached end of file.
            QString SOF = this->VG99_default.mid(y,1).toHex().toUpper();
            QString bytes = this->VG99_default.mid(y+9,1).toHex().toUpper();   // should be same as addr2

            //qDebug()<<"SOF:"<<SOF<<"bytes:"<<bytes<<"addr2:"<<addr2<<"less than:"<<(addr2+addr3).toInt(&ok, 16)<<"<"<<this->VG99_default.mid(z+9,2).toHex().toUpper().toInt(&ok,16);

            if(SOF=="F0" && bytes == QString(addr2) && (((addr2+addr3).toInt(&ok, 16)<this->VG99_default.mid(z+9,2).toHex().toUpper().toInt(&ok,16)) || this->VG99_default.mid(z+9,2).toHex().toUpper().toInt(&ok,16)==-1))
            {
                pos = y+12+addr3.toInt(&ok, 16)-1;
                x=VG99_default.size();
            };
        };
        if(pos > -1 && size == value.size() && value_is_good)
        {
            VG99_default.replace(pos, size, value);
        }
        else
        {
            qDebug() << "ERROR in tsl_VG99_VB99_translation::VG99_default_replace * VG-99 address:" << addr2 << addr3 << "pos=" << pos << " size=" << size <<
                        " value=" << value.toHex().toUpper() << " rangeMin=" << rangeMin << " rangeMax=" << rangeMax;
        };
    };
}

void tsl_VG99_VB99_translation::SY1000_assign(QByteArray assign)
{
    if(assign_number>15)
    {
        qDebug() << "run out of assigns";
        return;
    };
    bool ok;
    QByteArray sy_assign;
    QString target;
    MidiTable *midiTable = MidiTable::Instance();
    Midi items;
    if(assign.mid(0,1).toHex().toUpper()=="01" && assign_number<16)                                   //if assign A is 'on'
    {
        //qDebug() << "assign A = on, assign number=" << assign_number;
        sy_assign.append(assign.mid(0,1));                                                            // assign on/off
        int max = 256;
        for(int y=0; y<6; y++)
        {
            if(y==5)max=136;
            QString addr = "0" + QString::number(y, 16);
            items = midiTable->getMidiMap("Tables", "00", "01", "52", addr);
            for(int x = 0; x<max; x++)
            {
                QString find = items.level.at(x).name.split(' ').at(0)+items.level.at(x).name.split(' ').at(1);
                if(find == assign.mid(1, 2).toHex().toUpper())
                {
                    target = items.level.at(x).desc.split(' ').at(0);
                };
            };
        };
        if(target.isEmpty()) target = "3";
        target = QString::number(target.toInt(&ok, 10), 16).toUpper();
        target.prepend("0000000");
        target = target.mid(target.size()-8, 8);
        sy_assign.append(QByteArray::fromHex(target.toLatin1()));                                     // target
        QString tmin = QString::number(assign.mid(3, 2).toHex().toInt(&ok, 16)+32768, 16).toUpper();
        sy_assign.append(convert_to_4Byte(QByteArray::fromHex(tmin.toLatin1()), 1));                  // min
        QString tmax = QString::number(assign.mid(5, 2).toHex().toInt(&ok, 16)+32768, 16).toUpper();
        sy_assign.append(convert_to_4Byte(QByteArray::fromHex(tmax.toLatin1()), 1));                  // max
        sy_assign.append(convertVG99_value(assign.mid(13, 1), "01", "75"));                           // source
        sy_assign.append(convertVG99_value(assign.mid(7, 1), "01", "76"));                            // source mod
        sy_assign.append(convert_to_4Byte(assign.mid(8, 1), 128));                                    // range low
        sy_assign.append(convert_to_4Byte(assign.mid(9, 1), 128));                                    // range high
        sy_assign.append(QByteArray::fromHex("16"));                                                  // rise
        sy_assign.append(QByteArray::fromHex("32"));                                                  // fall
        sy_assign.append(assign.mid(11, 1));                                                          // rate
        sy_assign.append(assign.mid(10, 1));                                                          // waveform
        items = midiTable->getMidiMap("Tables", "00", "01", "77");                                    //Tables "00", "00", "1B" = SY assign address
        SY1000_default_replace("00", items.level.at(assign_number).desc, items.level.at(assign_number).customdesc, 27, sy_assign);
        //qDebug() << "assign result" << sy_assign.toHex().toUpper() << " size=" << sy_assign.size() << tmin << tmax;
        assign_number++;
    };

    if(assign.mid(14,1).toHex().toUpper()=="01" && assign_number<16)                                  //if assign B is 'on'
    {
        sy_assign.clear();
        //qDebug() << "assign B = on, assign number=" << assign_number;
        sy_assign.append(assign.mid(0,1));
        int max = 256;
        for(int y=0; y<6; y++)
        {
            if(y==5)max=136;
            QString addr = "0" + QString::number(y, 16);
            items = midiTable->getMidiMap("Tables", "00", "01", "52", addr);
            for(int x = 0; x<max; x++)
            {
                QString find = items.level.at(x).name.split(' ').at(0)+items.level.at(x).name.split(' ').at(1);
                if(find == assign.mid(15, 2).toHex().toUpper())
                {
                    target = items.level.at(x).desc.split(' ').at(0);
                };
            };
        };
        if(target.isEmpty()) target = "3";
        target = QString::number(target.toInt(&ok, 10), 16).toUpper();
        target.prepend("0000000");
        target = target.mid(target.size()-8, 8);                                                      // target
        sy_assign.append(QByteArray::fromHex(target.toLatin1()));
        QString tmin = QString::number(assign.mid(17, 2).toHex().toInt(&ok, 16)+32768, 16).toUpper();
        sy_assign.append(convert_to_4Byte(QByteArray::fromHex(tmin.toLatin1()), 1));                  // min
        QString tmax = QString::number(assign.mid(19, 2).toHex().toInt(&ok, 16)+32768, 16).toUpper();
        sy_assign.append(convert_to_4Byte(QByteArray::fromHex(tmax.toLatin1()), 1));                  // max
        sy_assign.append(convertVG99_value(assign.mid(13, 1), "01", "75"));                           // source
        sy_assign.append(convertVG99_value(assign.mid(21, 1), "01", "76"));                           // source mode
        sy_assign.append(convert_to_4Byte(assign.mid(22, 1), 128));                                   // range low
        sy_assign.append(convert_to_4Byte(assign.mid(23, 1), 128));                                   // range high
        sy_assign.append(QByteArray::fromHex("16"));                                                  // rise
        sy_assign.append(QByteArray::fromHex("32"));                                                  // fall
        sy_assign.append(assign.mid(11, 1));                                                          // rate
        sy_assign.append(assign.mid(10, 1));                                                          // waveform
        items = midiTable->getMidiMap("Tables", "00", "01", "77");                                    //Tables "00", "00", "1B" = SY assign address
        SY1000_default_replace("00", items.level.at(assign_number).desc, items.level.at(assign_number).customdesc, 27, sy_assign);
        //qDebug() << "assign result" << sy_assign.toHex().toUpper();
        assign_number++;
    };
}

QByteArray tsl_VG99_VB99_translation::convert_to_4Byte(QByteArray value, double multiplier)
{
    bool ok;
    int val = value.toHex().toUpper().toInt(&ok, 16)*multiplier;
    QString index = "000" + QString::number(val, 16);
    QByteArray new_value;
    new_value.append(QByteArray::fromHex(("0" + index.mid(index.size()-4, 1)).toLatin1()));
    new_value.append(QByteArray::fromHex(("0" + index.mid(index.size()-3, 1)).toLatin1()));
    new_value.append(QByteArray::fromHex(("0" + index.mid(index.size()-2, 1)).toLatin1()));
    new_value.append(QByteArray::fromHex(("0" + index.mid(index.size()-1, 1)).toLatin1()));
    //qDebug() << "convert_to_4Byte() * QByte value=" << value.toHex().toUpper() << "int val=" << val << "return new_value=" << new_value.toHex().toUpper();
    return new_value;
}

QByteArray tsl_VG99_VB99_translation::convertVG99_value(QByteArray value, QString hex2, QString hex3)
{
    bool ok;
    int v = value.toHex().toInt(&ok, 16);
    MidiTable *midiTable = MidiTable::Instance();
    Midi items;
    items = midiTable->getMidiMap("Tables", "00", hex2, hex3);
    QString desc = items.level.at(v).desc;
    //QString before = items.level.at(v).name;
    //QString after = items.level.at(v).customdesc;
    QByteArray value2 = QByteArray::fromHex(desc.toLatin1()).toUpper();
    //qDebug() << "convertVG99_value() * before:" << before << "=" << items.level.at(v).value << " after:" << after << "=" << value2.toHex().toUpper();
    return value2;
}

QByteArray tsl_VG99_VB99_translation::convertVB99_value(QByteArray value, QString hex2, QString hex3)
{
    bool ok;
    int v = value.toHex().toInt(&ok, 16);
    VG99_Midi items;
    items = getMidiMap("Tables", "00", hex2, hex3);
    QString desc = items.level.at(v).desc;
    //QString before = items.level.at(v).name;
    //QString after = items.level.at(v).customdesc;
    QByteArray value2 = QByteArray::fromHex(desc.toLatin1()).toUpper();
    //qDebug() << "convertVG99_value() * before:" << before << "=" << items.level.at(v).value << " after:" << after << "=" << value2.toHex().toUpper();
    return value2;
}

QByteArray tsl_VG99_VB99_translation::scaleVG99_value(QByteArray val, QString maxVal, QString min, QString max)
{
    bool ok;
    uint value = val.toHex().toUpper().toInt(&ok, 16);
    float maxV = maxVal.toInt(&ok, 16);
    uint minimum = min.toInt(&ok, 16);
    uint maximum = max.toInt(&ok, 16);
    float range = maximum-minimum;
    float result = (value*range/maxV)+minimum;
    if(result<minimum){result = minimum;};
    if(result>maximum){result = maximum;};
    int integer = result;
    QString newNum = QString::number(integer, 16);
    if(newNum.size()<2) newNum.prepend("0");
    QByteArray value2 = QByteArray::fromHex(newNum.toLatin1()).toUpper();
    //qDebug() << "scaleVG99_value() * before:" << value << val.toHex()  << " after:" << value2.toHex().toUpper();
    return value2;
}

QByteArray tsl_VG99_VB99_translation::convertFromVG99default_to_SY1000default(int patch)
{
    Q_UNUSED(patch);
    bool ok;
    a=0;
    Preferences *preferences = Preferences::Instance();
    QByteArray t;
    int x=0;
    t.append(char(1));
    //SY1000_default.replace(1221, 1, convertVG99_value(t, "01", "02")); // switch on normal pu
    SY1000_default_replace("00", "12", "01", 1, QByteArray::fromHex("00"));   // NPU off
    SY1000_default_replace("00", "38", "00", 1, QByteArray::fromHex("00"));   // NPU Amp off
    t.clear();
    t.append(char(0));
    SY1000_default.replace(1358, 1, convertVG99_value(t, "01", "02")); // switch off inst 1
    x = 0;
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){x = -44;};
    SY1000_default.replace(x+2064, 1, convertVG99_value(t, "01", "02")); // switch off inst 2
    x = 0;
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){x = -88;};
    SY1000_default.replace(x+2770, 1, convertVG99_value(t, "01", "02")); // switch off inst 3


    SY1000_default_replace("00", "00", "00", 16, copyVG99("00", "00", 16));        // patch name
    SY1000_default_replace("00", "02", "00", 1, convertVG99_value(copyVG99("00", "11", 1), "01", "55")); // GK set *USEAGE: sy1000 patch address 3 byte, size to replace, VG-99 patch address 2 byte, size to copy, midi.xml lookup "Tables" address 2 byte.
    if(copyVG99("00", "11", 1).toHex()=="00")
    {
        SY1000_default_replace("00", "02", "00", 1, QByteArray::fromHex("00"));    // GK set system/patch
    };
    SY1000_default_replace("00", "00", "71", 2, copyVG99("00", "12", 2));          // midi bank
    SY1000_default_replace("00", "00", "75", 2, copyVG99("00", "14", 2));          // midi patch
    SY1000_default_replace("00", "12", "3E", 4, convert_to_4Byte(scaleVG99_value(copyVG99("00", "15", 2), "152", "190", "9C4"), 1));  // tempo
    SY1000_default_replace("00", "12", "42", 1, copyVG99("00", "17", 1));          //Master Key
    SY1000_default_replace("00", "00", "77", 2, copyVG99("00", "26", 2));          //String 1 midi msb
    SY1000_default_replace("00", "00", "79", 2, copyVG99("00", "28", 2));          //String 1 midi lsb
    SY1000_default_replace("00", "00", "7B", 2, copyVG99("00", "2A", 2));          //String 1 midi patch
    SY1000_default_replace("00", "00", "7D", 2, copyVG99("00", "2C", 2));          //String 2 midi msb
    SY1000_default_replace("00", "00", "7F", 2, copyVG99("00", "2E", 2));          //String 2 midi lsb
    SY1000_default_replace("00", "01", "01", 2, copyVG99("00", "30", 2));          //String 2 midi patch
    SY1000_default_replace("00", "01", "03", 2, copyVG99("00", "32", 2));          //String 3 midi msb
    SY1000_default_replace("00", "01", "05", 2, copyVG99("00", "34", 2));          //String 3 midi lsb
    SY1000_default_replace("00", "01", "07", 2, copyVG99("00", "36", 2));          //String 3 midi patch
    SY1000_default_replace("00", "01", "09", 2, copyVG99("00", "42", 2));          //String 4 midi msb
    SY1000_default_replace("00", "01", "0B", 2, copyVG99("00", "44", 2));          //String 4 midi lsb
    SY1000_default_replace("00", "01", "0D", 2, copyVG99("00", "46", 2));          //String 4 midi patch
    SY1000_default_replace("00", "01", "0F", 2, copyVG99("00", "48", 2));          //String 5 midi msb
    SY1000_default_replace("00", "01", "11", 2, copyVG99("00", "4A", 2));          //String 5 midi lsb
    SY1000_default_replace("00", "01", "13", 2, copyVG99("00", "4C", 2));          //String 5 midi patch
    SY1000_default_replace("00", "01", "15", 2, copyVG99("00", "4E", 2));          //String 6 midi msb
    SY1000_default_replace("00", "01", "17", 2, copyVG99("00", "50", 2));          //String 6 midi lsb
    SY1000_default_replace("00", "01", "19", 2, copyVG99("00", "52", 2));          //String 6 midi patch
    SY1000_default_replace("00", "14", "01", 1, copyVG99("00", "38", 1));          //gtr midi mode
    SY1000_default_replace("00", "14", "03", 1, copyVG99("00", "3A", 1));          //gtr midi chromatic
    SY1000_default_replace("00", "14", "04", 1, copyVG99("00", "3B", 1));          //gtr midi hold type
    SY1000_default_replace("00", "02", "00", 1, convertVG99_value(copyVG99("00", "39", 1), "01", "56")); //gtr midi play/feel

    assign_number = 0;
    SY1000_assign(copyVG99("70", "00", 27)); //VG-99 assign 1
    SY1000_assign(copyVG99("70", "1C", 27)); //VG-99 assign 2
    SY1000_assign(copyVG99("70", "38", 27)); //VG-99 assign 3
    SY1000_assign(copyVG99("70", "54", 27)); //VG-99 assign 4
    SY1000_assign(copyVG99("71", "00", 27)); //VG-99 assign 5
    SY1000_assign(copyVG99("71", "1C", 27)); //VG-99 assign 6
    SY1000_assign(copyVG99("71", "38", 27)); //VG-99 assign 7
    SY1000_assign(copyVG99("71", "54", 27)); //VG-99 assign 8
    SY1000_assign(copyVG99("72", "00", 27)); //VG-99 assign 9
    SY1000_assign(copyVG99("72", "1C", 27)); //VG-99 assign 10
    SY1000_assign(copyVG99("72", "38", 27)); //VG-99 assign 11
    SY1000_assign(copyVG99("72", "54", 27)); //VG-99 assign 12
    SY1000_assign(copyVG99("73", "00", 27)); //VG-99 assign 13
    SY1000_assign(copyVG99("73", "1C", 27)); //VG-99 assign 14
    SY1000_assign(copyVG99("73", "38", 27)); //VG-99 assign 15
    SY1000_assign(copyVG99("73", "54", 27)); //VG-99 assign 16



    SY1000_default_replace("00", "1F", "00", 1, copyVG99("10", "01", 1));                                //alt tuning A on/off
    SY1000_default_replace("00", "1F", "01", 1, convertVG99_value(copyVG99("10", "02", 1), "01", "57")); // type
    SY1000_default_replace("00", "1F", "02", 1, convertVG99_value(copyVG99("10", "11", 1), "01", "58")); //alt tuning A pitch
    SY1000_default_replace("00", "1F", "03", 1, convertVG99_value(copyVG99("10", "12", 1), "01", "58")); //alt tuning A pitch
    SY1000_default_replace("00", "1F", "04", 1, convertVG99_value(copyVG99("10", "13", 1), "01", "58")); //alt tuning A pitch
    SY1000_default_replace("00", "1F", "05", 1, convertVG99_value(copyVG99("10", "14", 1), "01", "58")); //alt tuning A pitch
    SY1000_default_replace("00", "1F", "06", 1, convertVG99_value(copyVG99("10", "15", 1), "01", "58")); //alt tuning A pitch
    SY1000_default_replace("00", "1F", "07", 1, convertVG99_value(copyVG99("10", "16", 1), "01", "58")); //alt tuning A pitch
    SY1000_default_replace("00", "1F", "08", 1, convertVG99_value(copyVG99("10", "17", 1), "01", "59")); //alt tuning A fine
    SY1000_default_replace("00", "1F", "09", 1, convertVG99_value(copyVG99("10", "18", 1), "01", "59")); //alt tuning A fine
    SY1000_default_replace("00", "1F", "0A", 1, convertVG99_value(copyVG99("10", "19", 1), "01", "59")); //alt tuning A fine
    SY1000_default_replace("00", "1F", "0B", 1, convertVG99_value(copyVG99("10", "1A", 1), "01", "59")); //alt tuning A fine
    SY1000_default_replace("00", "1F", "0C", 1, convertVG99_value(copyVG99("10", "1B", 1), "01", "59")); //alt tuning A fine
    SY1000_default_replace("00", "1F", "0D", 1, convertVG99_value(copyVG99("10", "1C", 1), "01", "59")); //alt tuning A fine
    SY1000_default_replace("00", "1F", "0E", 1, copyVG99("10", "04", 1));                                //A 12 str on/off

    SY1000_default_replace("00", "2A", "00", 1, copyVG99("10", "09", 1));                                //alt tuning B on/off
    SY1000_default_replace("00", "2A", "01", 1, convertVG99_value(copyVG99("10", "0A", 1), "01", "57")); // type
    SY1000_default_replace("00", "2A", "02", 1, convertVG99_value(copyVG99("10", "51", 1), "01", "58")); //alt tuning B pitch
    SY1000_default_replace("00", "2A", "03", 1, convertVG99_value(copyVG99("10", "52", 1), "01", "58")); //alt tuning B pitch
    SY1000_default_replace("00", "2A", "04", 1, convertVG99_value(copyVG99("10", "53", 1), "01", "58")); //alt tuning B pitch
    SY1000_default_replace("00", "2A", "05", 1, convertVG99_value(copyVG99("10", "54", 1), "01", "58")); //alt tuning B pitch
    SY1000_default_replace("00", "2A", "06", 1, convertVG99_value(copyVG99("10", "55", 1), "01", "58")); //alt tuning B pitch
    SY1000_default_replace("00", "2A", "07", 1, convertVG99_value(copyVG99("10", "56", 1), "01", "58")); //alt tuning B pitch
    SY1000_default_replace("00", "2A", "08", 1, convertVG99_value(copyVG99("10", "57", 1), "01", "59")); //alt tuning B fine
    SY1000_default_replace("00", "2A", "09", 1, convertVG99_value(copyVG99("10", "58", 1), "01", "59")); //alt tuning B fine
    SY1000_default_replace("00", "2A", "0A", 1, convertVG99_value(copyVG99("10", "59", 1), "01", "59")); //alt tuning B fine
    SY1000_default_replace("00", "2A", "0B", 1, convertVG99_value(copyVG99("10", "5A", 1), "01", "59")); //alt tuning B fine
    SY1000_default_replace("00", "2A", "0C", 1, convertVG99_value(copyVG99("10", "5B", 1), "01", "59")); //alt tuning B fine
    SY1000_default_replace("00", "2A", "0D", 1, convertVG99_value(copyVG99("10", "5C", 1), "01", "59")); //alt tuning B fine
    SY1000_default_replace("00", "2A", "0E", 1, copyVG99("10", "0C", 1));                                //B 12 str on/off


    SY1000_default_replace("00", "15", "00", 1, copyVG99("30", "00", 1));          //COSM A off/on
    SY1000_default_replace("00", "15", "01", 1, convertVG99_value(copyVG99("30", "01", 1), "01", "5A")); // COSM A type
    SY1000_default_replace("00", "1A", "21", 1, copyVG99("30", "02", 1));          //COSM A EQ off/on                          sy1000 e guitar
    SY1000_default_replace("00", "1A", "23", 1, convertVG99_value(copyVG99("30", "03", 1), "01", "5C")); // COSM A EQ low gain
    SY1000_default_replace("00", "1A", "24", 1, copyVG99("30", "04", 1));          //COSM A EQ low mid freq
    SY1000_default_replace("00", "1A", "25", 1, copyVG99("30", "05", 1));          //COSM A EQ low mid Q
    SY1000_default_replace("00", "1A", "26", 1, convertVG99_value(copyVG99("30", "06", 1), "01", "5C")); // COSM A EQ low mid gain
    SY1000_default_replace("00", "1A", "27", 1, copyVG99("30", "07", 1));          //COSM A EQ high mid freq
    SY1000_default_replace("00", "1A", "28", 1, copyVG99("30", "08", 1));          //COSM A EQ high mid Q
    SY1000_default_replace("00", "1A", "29", 1, convertVG99_value(copyVG99("30", "09", 1), "01", "5C")); // COSM A EQ high mid gain
    SY1000_default_replace("00", "1A", "2A", 1, convertVG99_value(copyVG99("30", "0A", 1), "01", "5C")); // COSM A EQ high gain
    SY1000_default_replace("00", "1A", "2C", 1, convertVG99_value(copyVG99("30", "0B", 1), "01", "5C")); // COSM A EQ level
    SY1000_default_replace("00", "1B", "23", 1, copyVG99("30", "02", 1));          //COSM A EQ off/on                          sy1000 acoustic
    SY1000_default_replace("00", "1B", "25", 1, convertVG99_value(copyVG99("30", "03", 1), "01", "5C")); // COSM A EQ low gain
    SY1000_default_replace("00", "1B", "26", 1, copyVG99("30", "04", 1));          //COSM A EQ low mid freq
    SY1000_default_replace("00", "1B", "27", 1, copyVG99("30", "05", 1));          //COSM A EQ low mid Q
    SY1000_default_replace("00", "1B", "28", 1, convertVG99_value(copyVG99("30", "06", 1), "01", "5C")); // COSM A EQ low mid gain
    SY1000_default_replace("00", "1B", "29", 1, copyVG99("30", "07", 1));          //COSM A EQ high mid freq
    SY1000_default_replace("00", "1B", "2A", 1, copyVG99("30", "08", 1));          //COSM A EQ high mid Q
    SY1000_default_replace("00", "1B", "2B", 1, convertVG99_value(copyVG99("30", "09", 1), "01", "5C")); // COSM A EQ high mid gain
    SY1000_default_replace("00", "1B", "2C", 1, convertVG99_value(copyVG99("30", "0A", 1), "01", "5C")); // COSM A EQ high gain
    SY1000_default_replace("00", "1B", "2E", 1, convertVG99_value(copyVG99("30", "0B", 1), "01", "5C")); // COSM A EQ level
    SY1000_default_replace("00", "1C", "22", 1, copyVG99("30", "02", 1));          //COSM A EQ off/on                            sy1000 e bass
    SY1000_default_replace("00", "1C", "24", 1, convertVG99_value(copyVG99("30", "03", 1), "01", "5C")); // COSM A EQ low gain
    SY1000_default_replace("00", "1C", "25", 1, copyVG99("30", "04", 1));          //COSM A EQ low mid freq
    SY1000_default_replace("00", "1C", "26", 1, copyVG99("30", "05", 1));          //COSM A EQ low mid Q
    SY1000_default_replace("00", "1C", "27", 1, convertVG99_value(copyVG99("30", "06", 1), "01", "5C")); // COSM A EQ low mid gain
    SY1000_default_replace("00", "1C", "28", 1, copyVG99("30", "07", 1));          //COSM A EQ high mid freq
    SY1000_default_replace("00", "1C", "29", 1, copyVG99("30", "08", 1));          //COSM A EQ high mid Q
    SY1000_default_replace("00", "1C", "2A", 1, convertVG99_value(copyVG99("30", "09", 1), "01", "5C")); // COSM A EQ high mid gain
    SY1000_default_replace("00", "1C", "2B", 1, convertVG99_value(copyVG99("30", "0A", 1), "01", "5C")); // COSM A EQ high gain
    SY1000_default_replace("00", "1C", "2D", 1, convertVG99_value(copyVG99("30", "0B", 1), "01", "5C")); // COSM A EQ level
    SY1000_default_replace("00", "1D", "19", 1, copyVG99("30", "02", 1));          //COSM A EQ off/on                            sy1000 vio guitar
    SY1000_default_replace("00", "1D", "1B", 1, convertVG99_value(copyVG99("30", "03", 1), "01", "5C")); // COSM A EQ low gain
    SY1000_default_replace("00", "1D", "1C", 1, copyVG99("30", "04", 1));          //COSM A EQ low mid freq
    SY1000_default_replace("00", "1D", "1D", 1, copyVG99("30", "05", 1));          //COSM A EQ low mid Q
    SY1000_default_replace("00", "1D", "1E", 1, convertVG99_value(copyVG99("30", "06", 1), "01", "5C")); // COSM A EQ low mid gain
    SY1000_default_replace("00", "1D", "1F", 1, copyVG99("30", "07", 1));          //COSM A EQ high mid freq
    SY1000_default_replace("00", "1D", "20", 1, copyVG99("30", "08", 1));          //COSM A EQ high mid Q
    SY1000_default_replace("00", "1D", "21", 1, convertVG99_value(copyVG99("30", "09", 1), "01", "5C")); // COSM A EQ high mid gain
    SY1000_default_replace("00", "1D", "22", 1, convertVG99_value(copyVG99("30", "0A", 1), "01", "5C")); // COSM A EQ high gain
    SY1000_default_replace("00", "1D", "24", 1, convertVG99_value(copyVG99("30", "0B", 1), "01", "5C")); // COSM A EQ level
    SY1000_default_replace("00", "15", "0C", 6, copyVG99("30", "0C", 6));          //COSM A string pan 1~6
    SY1000_default_replace("00", "15", "06", 6, copyVG99("30", "12", 6));          //COSM A string level 1~6
    SY1000_default_replace("00", "15", "02", 1, copyVG99("30", "18", 1));          //COSM A mix level
    SY1000_default_replace("00", "15", "04", 2, convertVG99_value(copyVG99("30", "1A", 1), "01", "5E")); // COSM A NPU level
    if(copyVG99("30", "1A", 1).toHex().toInt(&ok,16)>0)
    {
        SY1000_default_replace("00", "12", "01", 1, QByteArray::fromHex("01"));   // NPU off/on
    };
    SY1000_default_replace("00", "1A", "00", 1, convertVG99_value(copyVG99("30", "1B", 1), "01", "5D")); // COSM A E Guitar type

    //reserved for vio select depending on e guitar model

    if(copyVG99("30", "1B", 1).toHex().toUpper().toInt(&ok, 16)<2)                     // if strat models
    {
        SY1000_default_replace("00", "1A", "01", 1, copyVG99("30", "1E", 1));          //COSM A e guitar pu select 6 way
    }
    else if(copyVG99("30", "1B", 1).toHex().toUpper().toInt(&ok, 16)==5)               // if lips model
    {
        SY1000_default_replace("00", "1A", "01", 1, copyVG99("30", "1D", 1));          //COSM A e guitar pu select 5 way
    }
    else
    {
        SY1000_default_replace("00", "1A", "01", 1, convertVG99_value(copyVG99("30", "1C", 1), "01", "78")); //COSM A e guitar pu select 3 way
    };

    SY1000_default_replace("00", "1A", "08", 1, copyVG99("30", "1F", 1));          //COSM A e guitar volume
    SY1000_default_replace("00", "1A", "09", 1, copyVG99("30", "22", 1));          //COSM A e guitar tone

    //reserved for e guitar vari settings

    SY1000_default_replace("00", "1B", "00", 1, convertVG99_value(copyVG99("30", "45", 1), "01", "5F")); // COSM A Acoustic Guitar type
    if(convertVG99_value(copyVG99("30", "45", 1), "01", "5F").toHex()=="00")           //steel acoustic
    {
        SY1000_default_replace("00", "1B", "00", 1, copyVG99("30", "46", 1));          //select guitar body type
        SY1000_default_replace("00", "1B", "01", 1, copyVG99("30", "47", 1));          //COSM A a guitar body
        SY1000_default_replace("00", "1B", "0B", 1, convertVG99_value(copyVG99("30", "48", 1), "01", "59")); //COSM A a guitar tone
        SY1000_default_replace("00", "1B", "0A", 1, copyVG99("30", "49", 1));          //COSM A a guitar level
    };

    //reserved for AC Vari model settings.

    if(convertVG99_value(copyVG99("30", "45", 1), "01", "5F").toHex()=="01")           //nylon acoustic
    {
        SY1000_default_replace("00", "1B", "01", 1, copyVG99("30", "54", 1));          //COSM A a guitar body
        SY1000_default_replace("00", "1B", "02", 1, copyVG99("30", "55", 1));          //COSM A a guitar attack
        SY1000_default_replace("00", "1B", "0B", 1, convertVG99_value(copyVG99("30", "48", 1), "01", "59")); //COSM A a guitar tone
        SY1000_default_replace("00", "1B", "0A", 1, copyVG99("30", "57", 1));          //COSM A a guitar level
    };
    if(convertVG99_value(copyVG99("30", "45", 1), "01", "5F").toHex()=="02")           //sitar
    {
        SY1000_default_replace("00", "1B", "03", 1, copyVG99("30", "58", 1));          //COSM A sitar pu select
        SY1000_default_replace("00", "1B", "04", 1, copyVG99("30", "59", 1));          //COSM A sitar sense
        SY1000_default_replace("00", "1B", "01", 1, copyVG99("30", "5A", 1));          //COSM A sitar body
        SY1000_default_replace("00", "1B", "05", 1, copyVG99("30", "5B", 1));          //COSM A sitar color
        SY1000_default_replace("00", "1B", "06", 1, copyVG99("30", "5C", 1));          //COSM A sitar decay
        SY1000_default_replace("00", "1B", "07", 1, copyVG99("30", "5D", 1));          //COSM A sitar buzz
        SY1000_default_replace("00", "1B", "02", 1, copyVG99("30", "5E", 1));          //COSM A sitar attack level
        SY1000_default_replace("00", "1B", "0B", 1, convertVG99_value(copyVG99("30", "48", 1), "01", "59")); //COSM A sitar tone
        SY1000_default_replace("00", "1B", "0A", 1, copyVG99("30", "60", 1));          //COSM A sitar level
    };
    if(convertVG99_value(copyVG99("30", "45", 1), "01", "5F").toHex()=="03")           //banjo
    {
        SY1000_default_replace("00", "1B", "02", 1, copyVG99("30", "61", 1));          //COSM A banjo attack
        SY1000_default_replace("00", "1B", "08", 1, copyVG99("30", "62", 1));          //COSM A banjo resonance
        SY1000_default_replace("00", "1B", "0B", 1, convertVG99_value(copyVG99("30", "48", 1), "01", "59")); //COSM A banjo tone
        SY1000_default_replace("00", "1B", "0A", 1, copyVG99("30", "64", 1));          //COSM A banjo level
    };
    if(convertVG99_value(copyVG99("30", "45", 1), "01", "5F").toHex()=="04")           //resonator
    {
        SY1000_default_replace("00", "1B", "09", 1, copyVG99("30", "65", 1));          //COSM A resonator sustain
        SY1000_default_replace("00", "1B", "08", 1, copyVG99("30", "66", 1));          //COSM A resonator resonance
        SY1000_default_replace("00", "1B", "0B", 1, convertVG99_value(copyVG99("30", "48", 1), "01", "59")); //COSM A resonator tone
        SY1000_default_replace("00", "1B", "0A", 1, copyVG99("30", "68", 1));          //COSM A resonator level
    };
    SY1000_default_replace("00", "1C", "00", 1, copyVG99("30", "69", 1));              //COSM A e bass type
    SY1000_default_replace("00", "1C", "09", 1, copyVG99("30", "6A", 1));              //COSM A e bass vol
    SY1000_default_replace("00", "1C", "02", 1, copyVG99("30", "6B", 1));              //COSM A e bass rear vol
    SY1000_default_replace("00", "1C", "01", 1, copyVG99("30", "6C", 1));              //COSM A e bass front vol
    SY1000_default_replace("00", "1C", "0A", 1, copyVG99("30", "6D", 1));              //COSM A e bass tone


    if(copyVG99("30", "01", 1).toHex()=="03")
    {
        SY1000_default_replace("00", "15", "01", 1, convertVG99_value(copyVG99("34", "00", 1), "01", "60")); //COSM A synth type
    };
    if(copyVG99("30", "01", 1).toHex()=="03" && copyVG99("34", "00", 1).toHex()=="00"){SY1000_default_replace("00", "15", "01", 1, QByteArray::fromHex("02"));}; //set type to GR300 synth
    SY1000_default_replace("00", "19", "00", 1, copyVG99("34", "01", 1));              //COSM A GR300 mode
    SY1000_default_replace("00", "19", "01", 1, copyVG99("34", "04", 1));              //COSM A GR300 comp sw
    SY1000_default_replace("00", "19", "02", 1, copyVG99("34", "05", 1));              //COSM A GR300 cut off
    SY1000_default_replace("00", "19", "03", 1, copyVG99("34", "06", 1));              //COSM A GR300 reso
    SY1000_default_replace("00", "19", "04", 1, copyVG99("34", "07", 1));              //COSM A GR300 mod mode
    SY1000_default_replace("00", "19", "06", 1, copyVG99("34", "08", 1));              //COSM A GR300 attack
    SY1000_default_replace("00", "19", "05", 1, copyVG99("34", "09", 1));              //COSM A GR300 sense
    SY1000_default_replace("00", "19", "0C", 1, copyVG99("34", "0A", 1));              //COSM A GR300 duet
    SY1000_default_replace("00", "19", "08", 1, copyVG99("34", "0B", 1));              //COSM A GR300 pitch A
    SY1000_default_replace("00", "19", "09", 1, copyVG99("34", "0C", 1));              //COSM A GR300 pitch fine A
    SY1000_default_replace("00", "19", "07", 1, copyVG99("34", "0D", 1));              //COSM A GR300 pitch sw
    SY1000_default_replace("00", "19", "0A", 1, copyVG99("34", "0E", 1));              //COSM A GR300 pitch B
    SY1000_default_replace("00", "19", "0B", 1, copyVG99("34", "0F", 1));              //COSM A GR300 pitch fine B
    SY1000_default_replace("00", "19", "0D", 6, copyVG99("34", "10", 6));              //COSM A GR300

    //reserved for all the other VG-99 synth types using dyna synth

    SY1000_default_replace("00", "1A", "1E", 1, copyVG99("34", "57", 1));              //COSM A NS switch
    SY1000_default_replace("00", "1A", "1F", 1, copyVG99("34", "58", 1));              //COSM A NS threshold
    SY1000_default_replace("00", "1A", "20", 1, copyVG99("34", "59", 1));              //COSM A NS release
    SY1000_default_replace("00", "1B", "20", 1, copyVG99("34", "57", 1));              //COSM A NS switch
    SY1000_default_replace("00", "1B", "21", 1, copyVG99("34", "58", 1));              //COSM A NS threshold
    SY1000_default_replace("00", "1B", "22", 1, copyVG99("34", "59", 1));              //COSM A NS release
    SY1000_default_replace("00", "1C", "1F", 1, copyVG99("34", "57", 1));              //COSM A NS switch
    SY1000_default_replace("00", "1C", "20", 1, copyVG99("34", "58", 1));              //COSM A NS threshold
    SY1000_default_replace("00", "1C", "21", 1, copyVG99("34", "59", 1));              //COSM A NS release
    SY1000_default_replace("00", "1D", "16", 1, copyVG99("34", "57", 1));              //COSM A NS switch
    SY1000_default_replace("00", "1D", "17", 1, copyVG99("34", "58", 1));              //COSM A NS threshold
    SY1000_default_replace("00", "1D", "18", 1, copyVG99("34", "59", 1));              //COSM A NS release
    SY1000_default_replace("00", "1E", "10", 1, copyVG99("34", "57", 1));              //COSM A NS switch
    SY1000_default_replace("00", "1E", "11", 1, copyVG99("34", "58", 1));              //COSM A NS threshold
    SY1000_default_replace("00", "1E", "12", 1, copyVG99("34", "59", 1));              //COSM A NS release


    SY1000_default_replace("00", "20", "00", 1, copyVG99("38", "00", 1));          //COSM B off/on
    SY1000_default_replace("00", "20", "01", 1, convertVG99_value(copyVG99("38", "01", 1), "01", "5A")); // COSM B type
    SY1000_default_replace("00", "25", "21", 1, copyVG99("38", "02", 1));          //COSM B EQ off/on                          sy1000 e guitar
    SY1000_default_replace("00", "25", "23", 1, convertVG99_value(copyVG99("38", "03", 1), "01", "5C")); // COSM B EQ low gain
    SY1000_default_replace("00", "25", "24", 1, copyVG99("38", "04", 1));          //COSM B EQ low mid freq
    SY1000_default_replace("00", "25", "25", 1, copyVG99("38", "05", 1));          //COSM B EQ low mid Q
    SY1000_default_replace("00", "25", "26", 1, convertVG99_value(copyVG99("38", "06", 1), "01", "5C")); // COSM B EQ low mid gain
    SY1000_default_replace("00", "25", "27", 1, copyVG99("38", "07", 1));          //COSM B EQ high mid freq
    SY1000_default_replace("00", "25", "28", 1, copyVG99("38", "08", 1));                           //COSM B EQ high mid Q
    SY1000_default_replace("00", "25", "29", 1, convertVG99_value(copyVG99("38", "09", 1), "01", "5C")); // COSM B EQ high mid gain
    SY1000_default_replace("00", "25", "2A", 1, convertVG99_value(copyVG99("38", "0A", 1), "01", "5C")); // COSM B EQ high gain
    SY1000_default_replace("00", "25", "2C", 1, convertVG99_value(copyVG99("38", "0B", 1), "01", "5C")); // COSM B EQ level
    SY1000_default_replace("00", "26", "23", 1, copyVG99("38", "02", 1));          //COSM B EQ off/on                          sy1000 acoustic
    SY1000_default_replace("00", "26", "25", 1, convertVG99_value(copyVG99("38", "03", 1), "01", "5C")); // COSM B EQ low gain
    SY1000_default_replace("00", "26", "26", 1, copyVG99("38", "04", 1));          //COSM B EQ low mid freq
    SY1000_default_replace("00", "26", "27", 1, copyVG99("38", "05", 1));          //COSM B EQ low mid Q
    SY1000_default_replace("00", "26", "28", 1, convertVG99_value(copyVG99("38", "06", 1), "01", "5C")); // COSM B EQ low mid gain
    SY1000_default_replace("00", "26", "29", 1, copyVG99("38", "07", 1));          //COSM B EQ high mid freq
    SY1000_default_replace("00", "26", "2A", 1, copyVG99("38", "08", 1));          //COSM B EQ high mid Q
    SY1000_default_replace("00", "26", "2B", 1, convertVG99_value(copyVG99("38", "09", 1), "01", "5C")); // COSM B EQ high mid gain
    SY1000_default_replace("00", "26", "2C", 1, convertVG99_value(copyVG99("38", "0A", 1), "01", "5C")); // COSM B EQ high gain
    SY1000_default_replace("00", "26", "2E", 1, convertVG99_value(copyVG99("38", "0B", 1), "01", "5C")); // COSM B EQ level
    SY1000_default_replace("00", "27", "22", 1, copyVG99("38", "02", 1));          //COSM B EQ off/on                            sy1000 e bass
    SY1000_default_replace("00", "27", "24", 1, convertVG99_value(copyVG99("38", "03", 1), "01", "5C")); // COSM B EQ low gain
    SY1000_default_replace("00", "27", "25", 1, copyVG99("38", "04", 1));          //COSM B EQ low mid freq
    SY1000_default_replace("00", "27", "26", 1, copyVG99("38", "05", 1));          //COSM B EQ low mid Q
    SY1000_default_replace("00", "27", "27", 1, convertVG99_value(copyVG99("38", "06", 1), "01", "5C")); // COSM B EQ low mid gain
    SY1000_default_replace("00", "27", "28", 1, copyVG99("38", "07", 1));          //COSM B EQ high mid freq
    SY1000_default_replace("00", "27", "29", 1, copyVG99("38", "08", 1));          //COSM B EQ high mid Q
    SY1000_default_replace("00", "27", "2A", 1, convertVG99_value(copyVG99("38", "09", 1), "01", "5C")); // COSM B EQ high mid gain
    SY1000_default_replace("00", "27", "2B", 1, convertVG99_value(copyVG99("38", "0A", 1), "01", "5C")); // COSM B EQ high gain
    SY1000_default_replace("00", "27", "2D", 1, convertVG99_value(copyVG99("38", "0B", 1), "01", "5C")); // COSM B EQ level
    SY1000_default_replace("00", "28", "19", 1, copyVG99("38", "02", 1));          //COSM B EQ off/on                            sy1000 vio guitar
    SY1000_default_replace("00", "28", "1B", 1, convertVG99_value(copyVG99("38", "03", 1), "01", "5C")); // COSM B EQ low gain
    SY1000_default_replace("00", "28", "1C", 1, copyVG99("38", "04", 1));          //COSM B EQ low mid freq
    SY1000_default_replace("00", "28", "1D", 1, copyVG99("38", "05", 1));          //COSM B EQ low mid Q
    SY1000_default_replace("00", "28", "1E", 1, convertVG99_value(copyVG99("38", "06", 1), "01", "5C")); // COSM B EQ low mid gain
    SY1000_default_replace("00", "28", "1F", 1, copyVG99("38", "07", 1));          //COSM B EQ high mid freq
    SY1000_default_replace("00", "28", "20", 1, copyVG99("38", "08", 1));          //COSM B EQ high mid Q
    SY1000_default_replace("00", "28", "21", 1, convertVG99_value(copyVG99("38", "09", 1), "01", "5C")); // COSM B EQ high mid gain
    SY1000_default_replace("00", "28", "22", 1, convertVG99_value(copyVG99("38", "0A", 1), "01", "5C")); // COSM B EQ high gain
    SY1000_default_replace("00", "28", "24", 1, convertVG99_value(copyVG99("38", "0B", 1), "01", "5C")); // COSM B EQ level
    SY1000_default_replace("00", "20", "0C", 6, copyVG99("38", "0C", 6));          //COSM B string pan 1~6
    SY1000_default_replace("00", "20", "06", 6, copyVG99("38", "12", 6));          //COSM B string level 1~6
    SY1000_default_replace("00", "20", "02", 1, copyVG99("38", "18", 1));          //COSM B mix level
    SY1000_default_replace("00", "20", "04", 2, convertVG99_value(copyVG99("38", "1A", 1), "01", "5E")); // COSM B NPU level

    if(copyVG99("38", "1A", 1).toHex().toInt(&ok,16)>0)                            // if npu level more than 0
    {
        SY1000_default_replace("00", "12", "01", 1, QByteArray::fromHex("01"));    // NPU off/on
    };
    SY1000_default_replace("00", "25", "00", 1, convertVG99_value(copyVG99("38", "1B", 1), "01", "5D")); // COSM B E Guitar type

    //reserved for vio select depending on e guitar model

    if(copyVG99("38", "1B", 1).toHex().toUpper().toInt(&ok, 16)<2)                     // if strat models
    {
        SY1000_default_replace("00", "25", "01", 1, copyVG99("38", "1E", 1));          //COSM B e guitar pu select 6 way
    }
    else if(copyVG99("38", "1B", 1).toHex().toUpper().toInt(&ok, 16)==8)               // if lips model
    {
        SY1000_default_replace("00", "25", "01", 1, copyVG99("38", "1D", 1));          //COSM B e guitar pu select 5 way
    }
    else
    {
        SY1000_default_replace("00", "25", "01", 1, convertVG99_value(copyVG99("38", "1C", 1), "01", "78")); //COSM B e guitar pu select 3 way
    };


    SY1000_default_replace("00", "25", "08", 1, copyVG99("38", "1F", 1));          //COSM B e guitar volume
    SY1000_default_replace("00", "25", "09", 1, copyVG99("38", "22", 1));          //COSM B e guitar tone

    //reserved for e guitar vari settings

    SY1000_default_replace("00", "26", "00", 1, convertVG99_value(copyVG99("38", "45", 1), "01", "5F")); // COSM B Acoustic Guitar type
    if(convertVG99_value(copyVG99("38", "45", 1), "01", "5F").toHex()=="00")           //steel acoustic
    {
        SY1000_default_replace("00", "26", "00", 1, copyVG99("38", "46", 1));          //select guitar body type
        SY1000_default_replace("00", "26", "01", 1, copyVG99("38", "47", 1));          //COSM B a guitar body
        SY1000_default_replace("00", "26", "0B", 1, convertVG99_value(copyVG99("38", "48", 1), "01", "59")); //COSM B a guitar tone
        SY1000_default_replace("00", "26", "0A", 1, copyVG99("38", "49", 1));          //COSM B a guitar level
    };

    //reserved for AC Vari model settings.

    if(convertVG99_value(copyVG99("38", "45", 1), "01", "5F").toHex()=="01")           //nylon acoustic
    {
        SY1000_default_replace("00", "26", "01", 1, copyVG99("38", "54", 1));          //COSM B a guitar body
        SY1000_default_replace("00", "26", "02", 1, copyVG99("38", "55", 1));          //COSM B a guitar attack
        SY1000_default_replace("00", "26", "0B", 1, convertVG99_value(copyVG99("38", "48", 1), "01", "59")); //COSM B a guitar tone
        SY1000_default_replace("00", "26", "0A", 1, copyVG99("38", "57", 1));          //COSM B a guitar level
    };
    if(convertVG99_value(copyVG99("38", "45", 1), "01", "5F").toHex()=="02")           //sitar
    {
        SY1000_default_replace("00", "26", "03", 1, copyVG99("38", "58", 1));          //COSM B sitar pu select
        SY1000_default_replace("00", "26", "04", 1, copyVG99("38", "59", 1));          //COSM B sitar sense
        SY1000_default_replace("00", "26", "01", 1, copyVG99("38", "5A", 1));          //COSM B sitar body
        SY1000_default_replace("00", "26", "05", 1, copyVG99("38", "5B", 1));          //COSM B sitar color
        SY1000_default_replace("00", "26", "06", 1, copyVG99("38", "5C", 1));          //COSM B sitar decay
        SY1000_default_replace("00", "26", "07", 1, copyVG99("38", "5D", 1));          //COSM B sitar buzz
        SY1000_default_replace("00", "26", "02", 1, copyVG99("38", "5E", 1));          //COSM B sitar attack level
        SY1000_default_replace("00", "26", "0B", 1, convertVG99_value(copyVG99("38", "48", 1), "01", "59")); //COSM B sitar tone
        SY1000_default_replace("00", "26", "0A", 1, copyVG99("38", "60", 1));          //COSM B sitar level
    };
    if(convertVG99_value(copyVG99("38", "45", 1), "01", "5F").toHex()=="03")           //banjo
    {
        SY1000_default_replace("00", "26", "02", 1, copyVG99("38", "61", 1));          //COSM B banjo attack
        SY1000_default_replace("00", "26", "08", 1, copyVG99("38", "62", 1));          //COSM B banjo resonance
        SY1000_default_replace("00", "26", "0B", 1, convertVG99_value(copyVG99("38", "48", 1), "01", "59")); //COSM B banjo tone
        SY1000_default_replace("00", "26", "0A", 1, copyVG99("38", "64", 1));          //COSM B banjo level
    };
    if(convertVG99_value(copyVG99("38", "45", 1), "01", "5F").toHex()=="04")           //resonator
    {
        SY1000_default_replace("00", "26", "09", 1, copyVG99("38", "65", 1));          //COSM B resonator sustain
        SY1000_default_replace("00", "26", "08", 1, copyVG99("38", "66", 1));          //COSM B resonator resonance
        SY1000_default_replace("00", "26", "0B", 1, convertVG99_value(copyVG99("38", "48", 1), "01", "59")); //COSM B resonator tone
        SY1000_default_replace("00", "26", "0A", 1, copyVG99("38", "68", 1));          //COSM B resonator level
    };
    SY1000_default_replace("00", "27", "00", 1, copyVG99("38", "69", 1));              //COSM B e bass type
    SY1000_default_replace("00", "27", "09", 1, copyVG99("38", "6A", 1));              //COSM B e bass vol
    SY1000_default_replace("00", "27", "02", 1, copyVG99("38", "6B", 1));              //COSM B e bass rear vol
    SY1000_default_replace("00", "27", "01", 1, copyVG99("38", "6C", 1));              //COSM B e bass front vol
    SY1000_default_replace("00", "27", "0A", 1, copyVG99("38", "6D", 1));              //COSM B e bass tone

    if(copyVG99("38", "01", 1).toHex()=="03")
    {
        SY1000_default_replace("00", "20", "01", 1, convertVG99_value(copyVG99("3C", "00", 1), "01", "60")); //COSM B synth type
    };
    if(copyVG99("38", "01", 1).toHex()=="03" && copyVG99("3C", "00", 1).toHex()=="00")
    {
        SY1000_default_replace("00", "20", "01", 1, QByteArray::fromHex("02"));        //set type to GR300 synth
    };
    SY1000_default_replace("00", "24", "00", 1, copyVG99("3C", "01", 1));              //COSM B GR300 mode
    SY1000_default_replace("00", "24", "01", 1, copyVG99("3C", "04", 1));              //COSM B GR300 comp sw
    SY1000_default_replace("00", "24", "02", 1, copyVG99("3C", "05", 1));              //COSM B GR300 cut off
    SY1000_default_replace("00", "24", "03", 1, copyVG99("3C", "06", 1));              //COSM B GR300 reso
    SY1000_default_replace("00", "24", "04", 1, copyVG99("3C", "07", 1));              //COSM B GR300 mod mode
    SY1000_default_replace("00", "24", "06", 1, copyVG99("3C", "08", 1));              //COSM B GR300 attack
    SY1000_default_replace("00", "24", "05", 1, copyVG99("3C", "09", 1));              //COSM B GR300 sense
    SY1000_default_replace("00", "24", "0C", 1, copyVG99("3C", "0A", 1));              //COSM B GR300 duet
    SY1000_default_replace("00", "24", "08", 1, copyVG99("3C", "0B", 1));              //COSM B GR300 pitch A
    SY1000_default_replace("00", "24", "09", 1, copyVG99("3C", "0C", 1));              //COSM B GR300 pitch fine A
    SY1000_default_replace("00", "24", "07", 1, copyVG99("3C", "0D", 1));              //COSM B GR300 pitch sw
    SY1000_default_replace("00", "24", "0A", 1, copyVG99("3C", "0E", 1));              //COSM B GR300 pitch B
    SY1000_default_replace("00", "24", "0B", 1, copyVG99("3C", "0F", 1));              //COSM B GR300 pitch fine B
    SY1000_default_replace("00", "24", "0D", 6, copyVG99("3C", "10", 6));              //COSM B GR300

    //reserved for all the other VG-99 synth types using dyna synth

    SY1000_default_replace("00", "25", "1E", 1, copyVG99("3C", "57", 1));              //COSM B NS switch
    SY1000_default_replace("00", "25", "1F", 1, copyVG99("3C", "58", 1));              //COSM B NS threshold
    SY1000_default_replace("00", "25", "20", 1, copyVG99("3C", "59", 1));              //COSM B NS release
    SY1000_default_replace("00", "26", "20", 1, copyVG99("3C", "57", 1));              //COSM B NS switch
    SY1000_default_replace("00", "26", "21", 1, copyVG99("3C", "58", 1));              //COSM B NS threshold
    SY1000_default_replace("00", "26", "22", 1, copyVG99("3C", "59", 1));              //COSM B NS release
    SY1000_default_replace("00", "27", "1F", 1, copyVG99("3C", "57", 1));              //COSM B NS switch
    SY1000_default_replace("00", "27", "20", 1, copyVG99("3C", "58", 1));              //COSM B NS threshold
    SY1000_default_replace("00", "27", "21", 1, copyVG99("3C", "59", 1));              //COSM B NS release
    SY1000_default_replace("00", "28", "16", 1, copyVG99("3C", "57", 1));              //COSM B NS switch
    SY1000_default_replace("00", "28", "17", 1, copyVG99("3C", "58", 1));              //COSM B NS threshold
    SY1000_default_replace("00", "28", "18", 1, copyVG99("3C", "59", 1));              //COSM B NS release
    SY1000_default_replace("00", "1E", "10", 1, copyVG99("3C", "57", 1));              //COSM B NS switch
    SY1000_default_replace("00", "1E", "11", 1, copyVG99("3C", "58", 1));              //COSM B NS threshold
    SY1000_default_replace("00", "1E", "12", 1, copyVG99("3C", "59", 1));              //COSM B NS release

    if(copyVG99("40", "00", 1).toHex()=="00"){SY1000_default_replace("00", "12", "2A", 1,QByteArray::fromHex("00"));} //polyfx A/B
    else{SY1000_default_replace("00", "12", "2A", 1,QByteArray::fromHex("64"));};
    SY1000_default_replace("00", "2B", "00", 1, copyVG99("40", "01", 1));              //polyfx on/off (inst 3)
    SY1000_default_replace("00", "2B", "01", 1, QByteArray::fromHex("07"));            //set inst 3 to polyfx
    SY1000_default_replace("00", "34", "00", 1, convertVG99_value(copyVG99("40", "02", 1), "01", "62")); //polyfx type
    //reserved for polyfx


    SY1000_default_replace("00", "1A", "0A", 1, copyVG99("50", "0D", 1));              //COSM A Amp on/off                  e guitar
    SY1000_default_replace("00", "1A", "0B", 1, convertVG99_value(copyVG99("50", "0E", 1), "01", "63")); //COSM A amp type
    SY1000_default_replace("00", "1A", "0C", 1, copyVG99("50", "0F", 1));              //COSM A Amp gain
    SY1000_default_replace("00", "1A", "10", 4, copyVG99("50", "10", 4));              //COSM A Amp bass~presence
    SY1000_default_replace("00", "1A", "0F", 1, copyVG99("50", "14", 1));              //COSM A Amp level
    SY1000_default_replace("00", "1A", "14", 4, copyVG99("50", "15", 4));              //COSM A Amp bright~solo
    SY1000_default_replace("00", "1A", "18", 1, convertVG99_value(copyVG99("50", "19", 1), "01", "64")); //COSM A speaker type
    SY1000_default_replace("00", "1A", "19", 5, copyVG99("50", "1A", 5));              //COSM A speaker mic

    SY1000_default_replace("00", "1B", "0C", 1, copyVG99("50", "0D", 1));              //COSM A Amp on/off                  acoustic
    SY1000_default_replace("00", "1B", "0D", 1, convertVG99_value(copyVG99("50", "0E", 1), "01", "63")); //COSM A amp type
    SY1000_default_replace("00", "1B", "0E", 1, copyVG99("50", "0F", 1));              //COSM A Amp gain
    SY1000_default_replace("00", "1B", "12", 4, copyVG99("50", "10", 4));              //COSM A Amp bass~presence
    SY1000_default_replace("00", "1B", "11", 1, copyVG99("50", "14", 1));              //COSM A Amp level
    SY1000_default_replace("00", "1B", "16", 4, copyVG99("50", "15", 4));              //COSM A Amp bright~solo
    SY1000_default_replace("00", "1B", "1A", 1, convertVG99_value(copyVG99("50", "19", 1), "01", "64")); //COSM A speaker type
    SY1000_default_replace("00", "1B", "1B", 5, copyVG99("50", "1A", 5));              //COSM A speaker mic

    SY1000_default_replace("00", "1C", "0B", 1, copyVG99("50", "0D", 1));              //COSM A Amp on/off                  e bass
    SY1000_default_replace("00", "1C", "0C", 1, convertVG99_value(copyVG99("50", "0E", 1), "01", "63")); //COSM A BASS amp type
    SY1000_default_replace("00", "1C", "0D", 1, copyVG99("5C", "00", 1));              //COSM A Bass amp gain
    SY1000_default_replace("00", "1C", "11", 3, copyVG99("5C", "01", 3));              //COSM A Bass amp b/m/t
    SY1000_default_replace("00", "1C", "10", 1, copyVG99("5C", "04", 1));              //COSM A Bass amp level
    SY1000_default_replace("00", "1C", "15", 1, copyVG99("5C", "05", 1));              //COSM A Bass amp bright
    SY1000_default_replace("00", "1C", "0F", 1, convertVG99_value(copyVG99("5C", "06", 1), "01", "65")); //COSM A mid freq
    SY1000_default_replace("00", "1C", "0E", 1, convertVG99_value(copyVG99("5C", "07", 1), "01", "65")); //COSM A response
    SY1000_default_replace("00", "1C", "14", 1, copyVG99("5C", "0C", 1));              //COSM A Bass amp enhancer
    SY1000_default_replace("00", "1C", "19", 1, convertVG99_value(copyVG99("5C", "0D", 1), "01", "66")); //COSM A Bass speaker type
    SY1000_default_replace("00", "1C", "1C", 3,  copyVG99("5C", "0E", 3));             //COSM A Bass speaker mic


    SY1000_default_replace("00", "25", "0A", 1, copyVG99("60", "0D", 1));              //COSM B Amp on/off                 e guitar
    SY1000_default_replace("00", "25", "0B", 1, convertVG99_value(copyVG99("60", "0E", 1), "01", "63")); //COSM B amp type
    SY1000_default_replace("00", "25", "0C", 1, copyVG99("60", "0F", 1));              //COSM B Amp gain
    SY1000_default_replace("00", "25", "10", 4, copyVG99("60", "10", 4));              //COSM B Amp bass~presence
    SY1000_default_replace("00", "25", "0F", 1, copyVG99("60", "14", 1));              //COSM B Amp level
    SY1000_default_replace("00", "25", "14", 4, copyVG99("60", "15", 4));              //COSM B Amp bright~solo
    SY1000_default_replace("00", "25", "18", 1, convertVG99_value(copyVG99("60", "19", 1), "01", "64")); //COSM B speaker type
    SY1000_default_replace("00", "25", "19", 5, copyVG99("60", "1A", 5));              //COSM B speaker mic

    SY1000_default_replace("00", "26", "0C", 1, copyVG99("60", "0D", 1));              //COSM B Amp on/off                 acoustic
    SY1000_default_replace("00", "26", "0D", 1, convertVG99_value(copyVG99("60", "0E", 1), "01", "63")); //COSM B amp type
    SY1000_default_replace("00", "26", "0E", 1, copyVG99("60", "0F", 1));              //COSM B Amp gain
    SY1000_default_replace("00", "26", "12", 4, copyVG99("60", "10", 4));              //COSM B Amp bass~presence
    SY1000_default_replace("00", "26", "11", 1, copyVG99("60", "14", 1));              //COSM B Amp level
    SY1000_default_replace("00", "26", "16", 4, copyVG99("60", "15", 4));              //COSM B Amp bright~solo
    SY1000_default_replace("00", "26", "1A", 1, convertVG99_value(copyVG99("60", "19", 1), "01", "64")); //COSM B speaker type
    SY1000_default_replace("00", "26", "1B", 5, copyVG99("60", "1A", 5));              //COSM B speaker mic

    SY1000_default_replace("00", "27", "0B", 1, copyVG99("60", "0D", 1));              //COSM B Amp on/off                 e bass
    SY1000_default_replace("00", "27", "0C", 1, convertVG99_value(copyVG99("60", "0E", 1), "01", "63")); //COSM B amp type
    SY1000_default_replace("00", "27", "0D", 1, copyVG99("6C", "00", 1));              //COSM B Bass amp gain
    SY1000_default_replace("00", "27", "11", 3, copyVG99("6C", "01", 3));              //COSM B Bass amp b/m/t
    SY1000_default_replace("00", "27", "10", 1, copyVG99("6C", "04", 1));              //COSM B Bass amp level
    SY1000_default_replace("00", "27", "15", 1, copyVG99("6C", "05", 1));              //COSM B Bass amp bright
    SY1000_default_replace("00", "27", "0F", 1, convertVG99_value(copyVG99("6C", "06", 1), "01", "65")); //COSM B mid freq
    SY1000_default_replace("00", "27", "0E", 1, convertVG99_value(copyVG99("6C", "07", 1), "01", "65")); //COSM B response
    SY1000_default_replace("00", "27", "14", 1, copyVG99("6C", "0C", 1));              //COSM B Bass amp enhancer
    SY1000_default_replace("00", "27", "19", 1, convertVG99_value(copyVG99("6C", "0D", 1), "01", "66")); //COSM B Bass speaker type
    SY1000_default_replace("00", "27", "1C", 3,  copyVG99("6C", "0E", 3));             //COSM B Bass speaker mic

    // normal Amp
    SY1000_default_replace("00", "38", "00", 1, copyVG99("50", "0D", 1));    //NPU Amp on/off
    SY1000_default_replace("00", "38", "01", 1, convertVG99_value(copyVG99("50", "0E", 1), "01", "63")); //NPU amp type
    SY1000_default_replace("00", "38", "02", 1, copyVG99("50", "0F", 1));              //NPU Amp gain
    SY1000_default_replace("00", "38", "06", 4, copyVG99("50", "10", 4));              //NPU Amp bass~presence
    SY1000_default_replace("00", "38", "05", 1, copyVG99("50", "14", 1));              //NPU Amp level
    SY1000_default_replace("00", "38", "0A", 4, copyVG99("50", "15", 4));              //NPU Amp bright~solo
    SY1000_default_replace("00", "38", "0E", 1, convertVG99_value(copyVG99("50", "19", 1), "01", "64")); //NPU speaker type
    SY1000_default_replace("00", "38", "0F", 5, copyVG99("50", "1A", 5));              //NPU speaker mic
    if(copyVG99("38", "1A", 1).toHex().toInt(&ok,16)>copyVG99("30", "1A", 1).toHex().toInt(&ok,16))  // if NPU A > NPU B then copy COSM B Amp settings
    {
        SY1000_default_replace("00", "38", "00", 1, copyVG99("60", "0D", 1));    //NPU Amp on/off
        SY1000_default_replace("00", "38", "01", 1, convertVG99_value(copyVG99("60", "0E", 1), "01", "63")); //NPU amp type
        SY1000_default_replace("00", "38", "02", 1, copyVG99("60", "0F", 1));              //NPU Amp gain
        SY1000_default_replace("00", "38", "06", 4, copyVG99("60", "10", 4));              //NPU Amp bass~presence
        SY1000_default_replace("00", "38", "05", 1, copyVG99("60", "14", 1));              //NPU Amp level
        SY1000_default_replace("00", "38", "0A", 4, copyVG99("60", "15", 4));              //NPU Amp bright~solo
        SY1000_default_replace("00", "38", "0E", 1, convertVG99_value(copyVG99("60", "19", 1), "01", "64")); //NPU speaker type
        SY1000_default_replace("00", "38", "0F", 5, copyVG99("60", "1A", 5));              //NPU speaker mic
    };


    SY1000_default_replace("00", "36", "00", 4, copyVG99("50", "2B", 4));              //COSM A compressor                    patch effect A
    SY1000_default_replace("00", "36", "06", 1, copyVG99("50", "31", 1));              //COSM A compressor tone
    SY1000_default_replace("00", "36", "04", 1, copyVG99("50", "32", 1));              //COSM A compressor effect

    SY1000_default_replace("00", "37", "00", 1, copyVG99("50", "33", 1));              //COSM A od/ds on/off                  patch effect A
    SY1000_default_replace("00", "37", "01", 1, convertVG99_value(copyVG99("50", "34", 1), "01", "67")); //COSM A od/ds type
    SY1000_default_replace("00", "37", "02", 1, copyVG99("50", "35", 1));              //COSM A od/ds drive
    SY1000_default_replace("00", "37", "05", 1, convertVG99_value(copyVG99("50", "36", 1), "01", "59")); //COSM A od/ds bottom
    SY1000_default_replace("00", "37", "03", 1, convertVG99_value(copyVG99("50", "37", 1), "01", "59")); //COSM A od/ds tone
    SY1000_default_replace("00", "37", "04", 1, copyVG99("50", "38", 1));              //COSM A od/ds level
    SY1000_default_replace("00", "37", "06", 1, copyVG99("50", "39", 1));              //COSM A od/ds direct



    //reserved for VG-99 WAH

    SY1000_default_replace("00", "3A", "00", 1, copyVG99("50", "48", 1));                                //COSM A effect EQ off/on   patch effect A
    SY1000_default_replace("00", "3A", "09", 1, convertVG99_value(copyVG99("50", "49", 1), "01", "68")); //COSM A EQ low cut
    SY1000_default_replace("00", "3A", "01", 1, convertVG99_value(copyVG99("50", "4A", 1), "01", "6A")); //COSM A EQ low gain
    SY1000_default_replace("00", "3A", "03", 1, copyVG99("50", "4B", 1));                                //COSM A EQ low mid freq
    SY1000_default_replace("00", "3A", "04", 1, copyVG99("50", "4C", 1));                                //COSM A EQ low mid Q
    SY1000_default_replace("00", "3A", "05", 1, convertVG99_value(copyVG99("50", "4D", 1), "01", "6A")); //COSM A EQ low mid gain
    SY1000_default_replace("00", "3A", "06", 1, copyVG99("50", "4E", 1));                                //COSM A EQ high mid freq
    SY1000_default_replace("00", "3A", "07", 1, copyVG99("50", "4F", 1));                                //COSM A EQ high mid Q
    SY1000_default_replace("00", "3A", "08", 1, convertVG99_value(copyVG99("50", "50", 1), "01", "6A")); //COSM A EQ high mid gain
    SY1000_default_replace("00", "3A", "02", 1, convertVG99_value(copyVG99("50", "51", 1), "01", "6A")); //COSM A EQ high gain
    SY1000_default_replace("00", "3A", "0A", 1, convertVG99_value(copyVG99("50", "52", 1), "01", "69")); //COSM A EQ high cut
    SY1000_default_replace("00", "3A", "0B", 1, convertVG99_value(copyVG99("50", "53", 1), "01", "6A")); //COSM A EQ level

    SY1000_default_replace("00", "3B", "00", 1, copyVG99("60", "48", 1));                                //COSM B effect EQ off/on   patch effect B
    SY1000_default_replace("00", "3B", "09", 1, convertVG99_value(copyVG99("60", "49", 1), "01", "68")); //COSM B EQ low cut
    SY1000_default_replace("00", "3B", "01", 1, convertVG99_value(copyVG99("60", "4A", 1), "01", "6A")); //COSM B EQ low gain
    SY1000_default_replace("00", "3B", "03", 1, copyVG99("60", "4B", 1));                                //COSM B EQ low mid freq
    SY1000_default_replace("00", "3B", "04", 1, copyVG99("60", "4C", 1));                                //COSM B EQ low mid Q
    SY1000_default_replace("00", "3B", "05", 1, convertVG99_value(copyVG99("60", "4D", 1), "01", "6A")); //COSM B EQ low mid gain
    SY1000_default_replace("00", "3B", "06", 1, copyVG99("60", "4E", 1));                                //COSM B EQ high mid freq
    SY1000_default_replace("00", "3B", "07", 1, copyVG99("60", "4F", 1));                                //COSM B EQ high mid Q
    SY1000_default_replace("00", "3B", "08", 1, convertVG99_value(copyVG99("60", "50", 1), "01", "6A")); //COSM B EQ high mid gain
    SY1000_default_replace("00", "3B", "02", 1, convertVG99_value(copyVG99("60", "51", 1), "01", "6A")); //COSM B EQ high gain
    SY1000_default_replace("00", "3B", "0A", 1, convertVG99_value(copyVG99("60", "52", 1), "01", "69")); //COSM B EQ high cut
    SY1000_default_replace("00", "3B", "0B", 1, convertVG99_value(copyVG99("60", "53", 1), "01", "6A")); //COSM B EQ level

    SY1000_default_replace("00", "3E", "00", 1, copyVG99("50", "54", 1));                                //COSM A effect delay on/off  patch effect A
    SY1000_default_replace("00", "3E", "01", 1, convertVG99_value(copyVG99("50", "55", 1), "01", "6B")); //COSM A effect delay type
    SY1000_default_replace("00", "3E", "02", 4, convert_to_4Byte(copyVG99("50", "56", 2), 0.54));        //COSM A effect delay time
    SY1000_default_replace("00", "3E", "1F", 1, copyVG99("50", "58", 1));                                //COSM A effect delay tap time
    SY1000_default_replace("00", "3E", "06", 1, copyVG99("50", "59", 1));                                //COSM A effect delay feedback
    SY1000_default_replace("00", "3E", "07", 1, convertVG99_value(copyVG99("50", "5A", 1), "01", "69")); //COSM A effect delay high cut
    SY1000_default_replace("00", "3E", "0A", 4, convert_to_4Byte(copyVG99("50", "5B", 2), 0.54));        //COSM A effect delay time 1
    SY1000_default_replace("00", "3E", "0E", 1, copyVG99("50", "5D", 1));                                //COSM A effect delay feedback 1
    SY1000_default_replace("00", "3E", "0F", 1, convertVG99_value(copyVG99("50", "5E", 1), "01", "69")); //COSM A effect delay high cut 1
    SY1000_default_replace("00", "3E", "10", 1, copyVG99("50", "5F", 1));                                //COSM A effect delay level 1
    SY1000_default_replace("00", "3E", "11", 4, convert_to_4Byte(copyVG99("50", "60", 2), 0.54));        //COSM A effect delay time 2
    SY1000_default_replace("00", "3E", "15", 1, copyVG99("50", "62", 1));                                //COSM A effect delay feedback 2
    SY1000_default_replace("00", "3E", "16", 1, convertVG99_value(copyVG99("50", "63", 1), "01", "69")); //COSM A effect delay high cut 2
    SY1000_default_replace("00", "3E", "17", 1, copyVG99("50", "64", 1));                                //COSM A effect delay level 2
    SY1000_default_replace("00", "3E", "1A", 1, copyVG99("50", "65", 1));                                //COSM A effect delay warp sw
    SY1000_default_replace("00", "3E", "1C", 1, copyVG99("50", "66", 1));                                //COSM A effect delay rise time
    SY1000_default_replace("00", "3E", "1E", 1, copyVG99("50", "67", 1));                                //COSM A effect delay w feedback
    SY1000_default_replace("00", "3E", "1D", 1, copyVG99("50", "68", 1));                                //COSM A effect delay w level
    SY1000_default_replace("00", "3E", "18", 2, copyVG99("50", "69", 2));                                //COSM A effect delay mod rate/depth
    SY1000_default_replace("00", "3E", "08", 2, copyVG99("50", "6B", 2));                                //COSM A effect delay effect/direct


    SY1000_default_replace("00", "3C", "00", 1, copyVG99("60", "54", 1));                                //COSM B effect delay on/off  patch effect B
    SY1000_default_replace("00", "3C", "01", 4, convert_to_4Byte(copyVG99("60", "56", 2), 0.54));        //COSM B effect delay time
    SY1000_default_replace("00", "3C", "05", 1, copyVG99("60", "59", 1));                                //COSM B effect delay feedback
    SY1000_default_replace("00", "3C", "06", 1, convertVG99_value(copyVG99("60", "5A", 1), "01", "69")); //COSM B effect delay high cut
    SY1000_default_replace("00", "3C", "07", 2, copyVG99("60", "6B", 2));                                //COSM B effect delay level/direct

    SY1000_default_replace("00", "3F", "00", 2, copyVG99("50", "6D", 2));                                //COSM A effect chorus on/off  patch effect A
    SY1000_default_replace("00", "3F", "02", 1, convertVG99_value(copyVG99("50", "6F", 1), "01", "6C")); //COSM A effect chorus rate
    SY1000_default_replace("00", "3F", "03", 2, copyVG99("50", "70", 2));                                //COSM A effect chorus depth/predelay
    SY1000_default_replace("00", "3F", "07", 1, convertVG99_value(copyVG99("50", "72", 1), "01", "68")); //COSM A effect chorus low cut
    SY1000_default_replace("00", "3F", "08", 1, convertVG99_value(copyVG99("50", "73", 1), "01", "69")); //COSM A effect chorus high cut
    SY1000_default_replace("00", "3F", "09", 1, copyVG99("50", "74", 1));                                //COSM A effect chorus level

    if(copyVG99("50", "75", 1)==QByteArray::fromHex("01"))
    {
        SY1000_default_replace("01", "32", "00", 3, copyVG99("50", "75", 3));                                //COSM A effect reverb on/off  patch effect A
        SY1000_default_replace("01", "32", "04", 4, convert_to_4Byte(copyVG99("50", "78", 1), 1));           //COSM A effect reverb predelay
        SY1000_default_replace("01", "32", "09", 1, convertVG99_value(copyVG99("50", "79", 1), "01", "68")); //COSM A effect reverb low cut
        SY1000_default_replace("01", "32", "0A", 1, convertVG99_value(copyVG99("50", "7A", 1), "01", "69")); //COSM A effect reverb high cut
        SY1000_default_replace("01", "32", "0B", 1, copyVG99("50", "7B", 1));                                //COSM A effect reverb density
        SY1000_default_replace("01", "32", "08", 1, copyVG99("50", "7C", 1));                                //COSM A effect reverb effect
        SY1000_default_replace("01", "32", "0C", 1, copyVG99("50", "7D", 1));                                //COSM A effect reverb direct
    };
    if(copyVG99("60", "75", 1)==QByteArray::fromHex("01"))
    {
        SY1000_default_replace("01", "32", "00", 3, copyVG99("60", "75", 3));                                //COSM B effect reverb on/off  patch effect A
        SY1000_default_replace("01", "32", "04", 4, convert_to_4Byte(copyVG99("60", "78", 1), 1));           //COSM B effect reverb predelay
        SY1000_default_replace("01", "32", "09", 1, convertVG99_value(copyVG99("60", "79", 1), "01", "68")); //COSM B effect reverb low cut
        SY1000_default_replace("01", "32", "0A", 1, convertVG99_value(copyVG99("60", "7A", 1), "01", "69")); //COSM B effect reverb high cut
        SY1000_default_replace("01", "32", "0B", 1, copyVG99("60", "7B", 1));                                //COSM B effect reverb density
        SY1000_default_replace("01", "32", "08", 1, copyVG99("60", "7C", 1));                                //COSM B effect reverb effect
        SY1000_default_replace("01", "32", "0C", 1, copyVG99("60", "7D", 1));                                //COSM B effect reverb direct
    };
    if(copyVG99("20", "20", 1)==QByteArray::fromHex("01"))
    {
        SY1000_default_replace("01", "32", "00", 3, copyVG99("20", "20", 3));                                //EFFECT effect reverb on/off  patch effect A
        SY1000_default_replace("01", "32", "04", 4, convert_to_4Byte(copyVG99("20", "23", 1), 1));           //EFFECT effect reverb predelay
        SY1000_default_replace("01", "32", "09", 1, convertVG99_value(copyVG99("20", "24", 1), "01", "68")); //EFFECT effect reverb low cut
        SY1000_default_replace("01", "32", "0A", 1, convertVG99_value(copyVG99("20", "25", 1), "01", "69")); //EFFECT effect reverb high cut
        SY1000_default_replace("01", "32", "0B", 1, copyVG99("20", "26", 1));                                //EFFECT effect reverb density
        SY1000_default_replace("01", "32", "08", 1, copyVG99("20", "27", 1));                                //EFFECT effect reverb effect
        SY1000_default_replace("01", "32", "0C", 1, copyVG99("20", "28", 1));                                //EFFECT effect reverb direct
    };
    SY1000_default_replace("01", "32", "0C", 1, QByteArray::fromHex("64"));                              //EFFECT reverb direct forced to 100

    SY1000_default_replace("00", "3D", "00", 1, copyVG99("20", "28", 1));                                //EFFECT effect delay on/off  patch effect B
    SY1000_default_replace("00", "3D", "01", 4, convert_to_4Byte(copyVG99("20", "29", 2), 0.54));        //EFFECT effect delay time
    SY1000_default_replace("00", "3D", "05", 1, copyVG99("20", "2B", 1));                                //EFFECT effect delay feedback
    SY1000_default_replace("00", "3D", "06", 1, convertVG99_value(copyVG99("20", "2C", 1), "01", "69")); //EFFECT effect delay high cut
    SY1000_default_replace("00", "3D", "07", 2, copyVG99("20", "2D", 2));                                //EFFECT effect delay level/direct
    SY1000_default_replace("00", "3D", "08", 1, QByteArray::fromHex("64"));                              //EFFECT delay direct forced to 100

    SY1000_default_replace("00", "39", "00", 2, copyVG99("50", "7E", 2));                                //COSM A effect ns on/off       patch effect A
    SY1000_default_replace("00", "39", "02", 2, copyVG99("51", "00", 2));                                //COSM A effect ns detect

    SY1000_default_replace("00", "12", "0A", 4, convert_to_4Byte(copyVG99("51", "02", 1), 10));          //COSM A FV max
    SY1000_default_replace("00", "12", "12", 1, copyVG99("51", "03", 1));                                //COSM A FV slope
    SY1000_default_replace("00", "12", "17", 4, convert_to_4Byte(copyVG99("61", "02", 1), 10));          //COSM A FV max
    SY1000_default_replace("00", "12", "1F", 1, copyVG99("61", "03", 1));                                //COSM B FV slope

    SY1000_default_replace("00", "40", "00", 1, copyVG99("54", "00", 1));                                //COSM A MOD1 on/off       patch effect A
    SY1000_default_replace("00", "40", "01", 1, convertVG99_value(copyVG99("54", "01", 1), "01", "6D")); //COSM A MOD1 type
    SY1000_default_replace("00", "45", "01", 1, convertVG99_value(copyVG99("54", "02", 1), "01", "6E")); //COSM A MOD1 comp type
    SY1000_default_replace("00", "45", "02", 2, copyVG99("54", "03", 2));                                //COSM A MOD1 comp sustain/attack
    SY1000_default_replace("00", "45", "06", 1, convertVG99_value(copyVG99("54", "05", 1), "01", "59")); //COSM A MOD1 comp tone
    SY1000_default_replace("00", "45", "04", 1, copyVG99("54", "06", 1));                                //COSM A MOD1 comp level
    SY1000_default_replace("00", "50", "00", 1, copyVG99("54", "07", 1));                                //COSM A MOD1 limiter type
    SY1000_default_replace("00", "50", "04", 1, copyVG99("54", "08", 1));                                //COSM A MOD1 limiter attack
    SY1000_default_replace("00", "50", "01", 1, copyVG99("54", "09", 1));                                //COSM A MOD1 limiter threshold
    SY1000_default_replace("00", "50", "02", 1, copyVG99("54", "0A", 1));                                //COSM A MOD1 limiter ratio
    SY1000_default_replace("00", "50", "05", 1, copyVG99("54", "0B", 1));                                //COSM A MOD1 limiter release
    SY1000_default_replace("00", "50", "03", 1, copyVG99("54", "0C", 1));                                //COSM A MOD1 limiter level
    SY1000_default_replace("00", "61", "00", 5, copyVG99("54", "0D", 5));                                //COSM A MOD1 twah first 5
    SY1000_default_replace("00", "61", "06", 2, copyVG99("54", "12", 2));                                //COSM A MOD1 twah last 2
    SY1000_default_replace("00", "42", "00", 1, copyVG99("54", "14", 1));                                //COSM A MOD1 awah mode
    SY1000_default_replace("00", "42", "04", 2, copyVG99("54", "15", 2));                                //COSM A MOD1 awah freq/reso
    SY1000_default_replace("00", "42", "01", 1, convertVG99_value(copyVG99("54", "17", 1), "01", "6C")); //COSM A MOD1 awah rate
    SY1000_default_replace("00", "42", "02", 1, copyVG99("54", "18", 1));                                //COSM A MOD1 awah depth
    SY1000_default_replace("00", "42", "07", 1, copyVG99("54", "19", 1));                                //COSM A MOD1 awah direct
    SY1000_default_replace("00", "42", "03", 1, copyVG99("54", "1A", 1));                                //COSM A MOD1 awah effect
    SY1000_default_replace("00", "63", "00", 3, copyVG99("54", "1B", 3));                                //COSM A MOD1 tremolo
    SY1000_default_replace("00", "57", "00", 1, copyVG99("54", "1E", 1));                                //COSM A MOD1 phaser
    SY1000_default_replace("00", "57", "01", 1, convertVG99_value(copyVG99("54", "1F", 1), "01", "6C")); //COSM A MOD1 phaser rate
    SY1000_default_replace("00", "57", "02", 1, copyVG99("54", "20", 1));                                //COSM A MOD1 phaser
    SY1000_default_replace("00", "57", "04", 1, copyVG99("54", "21", 1));                                //COSM A MOD1 phaser manual
    SY1000_default_replace("00", "57", "03", 1, copyVG99("54", "22", 1));                                //COSM A MOD1 phaser reso
    SY1000_default_replace("00", "57", "05", 3, copyVG99("54", "23", 3));                                //COSM A MOD1 phaser last 3
    SY1000_default_replace("00", "49", "00", 2, copyVG99("54", "26", 2));                                //COSM A MOD1 flanger rate/depth
    SY1000_default_replace("00", "49", "03", 2, copyVG99("54", "28", 2));                                //COSM A MOD1 flanger manual
    SY1000_default_replace("00", "49", "04", 1, copyVG99("54", "2A", 1));                                //COSM A MOD1 flanger separartion
    SY1000_default_replace("00", "49", "05", 1, convertVG99_value(copyVG99("54", "2B", 1), "01", "68")); //COSM A MOD1 flanger low cut
    SY1000_default_replace("00", "49", "00", 2, copyVG99("54", "2C", 2));                                //COSM A MOD1 flanger direct/effect
    SY1000_default_replace("00", "54", "01", 3, copyVG99("54", "2E", 3));                                //COSM A MOD1 panner
    SY1000_default_replace("00", "64", "00", 4, copyVG99("54", "31", 4));                                //COSM A MOD1 vibrato
    SY1000_default_replace("00", "44", "00", 1, convertVG99_value(copyVG99("54", "35", 1), "01", "6C")); //COSM A MOD1 uni-vibe rate
    SY1000_default_replace("00", "44", "01", 1, copyVG99("54", "36", 1));                                //COSM A MOD1 uni-vibe
    SY1000_default_replace("00", "44", "02", 1, copyVG99("54", "37", 1));                                //COSM A MOD1 uni-vibe
    SY1000_default_replace("00", "5A", "00", 2, copyVG99("54", "38", 2));                                //COSM A MOD1 ring mod
    SY1000_default_replace("00", "5A", "04", 2, copyVG99("54", "3A", 2));                                //COSM A MOD1 ring mod
    SY1000_default_replace("00", "5E", "00", 2, copyVG99("54", "3C", 2));                                //COSM A MOD1 slow gear
    SY1000_default_replace("00", "41", "00", 1, copyVG99("54", "3E", 1));                                //COSM A MOD1 feedbacker mode
    SY1000_default_replace("00", "41", "03", 1, copyVG99("54", "41", 1));                                //COSM A MOD1 feedbacker fb level
    SY1000_default_replace("00", "41", "01", 1, copyVG99("54", "3F", 1));                                //COSM A MOD1 feedbacker time
    SY1000_default_replace("00", "41", "02", 1, convertVG99_value(copyVG99("54", "44", 1), "01", "59")); //COSM A MOD1 feedbacker depth
    if(copyVG99("54", "01", 1).toHex()=="11")
    {
        SY1000_default_replace("00", "55", "09", 1, scaleVG99_value(copyVG99("54", "45", 1), "64", "00", "10")); //COSM A MOD1 afb freq1
        SY1000_default_replace("00", "55", "01", 1, scaleVG99_value(copyVG99("54", "46", 1), "64", "0C", "34")); //COSM A MOD1 afb depth1
        SY1000_default_replace("00", "55", "03", 1, scaleVG99_value(copyVG99("54", "47", 1), "64", "00", "1D")); //COSM A MOD1 afb freq2
        SY1000_default_replace("00", "55", "05", 1, scaleVG99_value(copyVG99("54", "48", 1), "64", "0C", "34")); //COSM A MOD1 afb depth2
        SY1000_default_replace("00", "55", "06", 1, scaleVG99_value(copyVG99("54", "49", 1), "64", "00", "1D")); //COSM A MOD1 afb freq3
        SY1000_default_replace("00", "55", "08", 1, scaleVG99_value(copyVG99("54", "4A", 1), "64", "0C", "34")); //COSM A MOD1 afb depth3
    };
    SY1000_default_replace("00", "4E", "00", 1, scaleVG99_value(copyVG99("54", "4B", 1), "02", "00", "01")); //COSM A MOD1 humanizer mode
    SY1000_default_replace("00", "4E", "01", 7, copyVG99("54", "4C", 7));                                //COSM A MOD1 humanizer
    if(copyVG99("54", "01", 1).toHex()=="15")
    {
        SY1000_default_replace("00", "55", "09", 1, convertVG99_value(copyVG99("54", "56", 1), "01", "68")); //COSM A MOD1 eq low cut
        SY1000_default_replace("00", "55", "01", 1, convertVG99_value(copyVG99("54", "57", 1), "01", "6A")); //COSM A MOD1 eq low gain
        SY1000_default_replace("00", "55", "03", 1, copyVG99("54", "58", 1));                                //COSM A MOD1 eq low mid freq
        SY1000_default_replace("00", "55", "04", 1, copyVG99("54", "59", 1));                                //COSM A MOD1 eq low mid Q
        SY1000_default_replace("00", "55", "05", 1, convertVG99_value(copyVG99("54", "5A", 1), "01", "6A")); //COSM A MOD1 eq low mid gain
        SY1000_default_replace("00", "55", "06", 1, copyVG99("54", "5B", 1));                                //COSM A MOD1 eq high mid freq
        SY1000_default_replace("00", "55", "07", 1, copyVG99("54", "5C", 1));                                //COSM A MOD1 eq high mid Q
        SY1000_default_replace("00", "55", "08", 1, convertVG99_value(copyVG99("54", "5D", 1), "01", "6A")); //COSM A MOD1 eq high mid gain
        SY1000_default_replace("00", "55", "02", 1, convertVG99_value(copyVG99("54", "5E", 1), "01", "6A")); //COSM A MOD1 eq high gain
        SY1000_default_replace("00", "55", "0A", 1, convertVG99_value(copyVG99("54", "5F", 1), "01", "69")); //COSM A MOD1 eq high cut
        SY1000_default_replace("00", "55", "0B", 1, convertVG99_value(copyVG99("54", "60", 1), "01", "6A")); //COSM A MOD1 eq level
    };
    SY1000_default_replace("00", "4D", "00", 2, copyVG99("54", "61", 2));                                //COSM A MOD1 harmony mode/h1
    SY1000_default_replace("00", "4D", "04", 4, convert_to_4Byte(copyVG99("54", "63", 1), 2));           //COSM A MOD1 harmony pre delay 1
    SY1000_default_replace("00", "4D", "03", 1, copyVG99("54", "65", 1));                                //COSM A MOD1 harmony voice 1
    SY1000_default_replace("00", "4D", "02", 1, copyVG99("54", "66", 1));                                //COSM A MOD1 harmony h2
    SY1000_default_replace("00", "4D", "0B", 4, convert_to_4Byte(copyVG99("54", "67", 1), 2));           //COSM A MOD1 harmony pre delay 2
    SY1000_default_replace("00", "4D", "0A", 1, copyVG99("54", "69", 1));                                //COSM A MOD1 harmony level 2
    SY1000_default_replace("00", "4D", "08", 2, copyVG99("54", "6A", 2));                                //COSM A MOD1 harmony fb/direct
    SY1000_default_replace("00", "58", "00", 1, copyVG99("54", "6C", 1));                                //COSM A MOD1 pitchshifter voice 1
    SY1000_default_replace("00", "58", "04", 1, copyVG99("54", "6D", 1));                                //COSM A MOD1 pitchshifter mode 1
    SY1000_default_replace("00", "58", "01", 1, convertVG99_value(copyVG99("54", "6E", 1), "01", "58")); //COSM A MOD1 pitchshifter pitch 1
    SY1000_default_replace("00", "58", "05", 1, convertVG99_value(copyVG99("54", "6F", 1), "01", "59")); //COSM A MOD1 pitchshifter fine 1
    SY1000_default_replace("00", "58", "06", 4, convert_to_4Byte(copyVG99("54", "70", 1), 2));           //COSM A MOD1 pitchshifter pre delay 1
    SY1000_default_replace("00", "58", "0A", 1, copyVG99("54", "72", 1));                                //COSM A MOD1 pitchshifter level 1
    SY1000_default_replace("00", "58", "0C", 1, copyVG99("54", "73", 1));                                //COSM A MOD1 pitchshifter mode 2
    SY1000_default_replace("00", "58", "02", 1, convertVG99_value(copyVG99("54", "74", 1), "01", "58")); //COSM A MOD1 pitchshifter pitch 2
    SY1000_default_replace("00", "58", "0D", 1, convertVG99_value(copyVG99("54", "75", 1), "01", "59")); //COSM A MOD1 pitchshifter fine 2
    SY1000_default_replace("00", "58", "0E", 4, convert_to_4Byte(copyVG99("54", "76", 1), 2));           //COSM A MOD1 pitchshifter pre delay 2
    SY1000_default_replace("00", "58", "12", 1, copyVG99("54", "78", 1));                                //COSM A MOD1 pitchshifter level 2
    SY1000_default_replace("00", "58", "0B", 1, copyVG99("54", "79", 1));                                //COSM A MOD1 pitchshifter feedback
    SY1000_default_replace("00", "56", "00", 1, convertVG99_value(copyVG99("54", "7C", 1), "01", "58")); //COSM A MOD1 pedalbend pitch max
    SY1000_default_replace("00", "56", "02", 4, convert_to_4Byte(copyVG99("54", "7D", 1), 1));           //COSM A MOD1 pedalbend position
    SY1000_default_replace("00", "56", "01", 1, copyVG99("54", "7E", 1));                                //COSM A MOD1 pedalbend effect
    SY1000_default_replace("00", "56", "06", 1, copyVG99("54", "7F", 1));                                //COSM A MOD1 pedalbend direct
    SY1000_default_replace("00", "52", "00", 1, QByteArray::fromHex("00"));                              //COSM A MOD1 octave -1 set to 00
    SY1000_default_replace("00", "52", "01", 1, QByteArray::fromHex("00"));                              //COSM A MOD1 octave -2 set to 00
    if(copyVG99("55", "00", 1).toHex()!="00")
    {
        SY1000_default_replace("00", "52", "00", 1, copyVG99("55", "01", 1));                            //COSM A MOD1 octave -1
    };
    if(copyVG99("55", "00", 1).toHex()!="01")
    {
        SY1000_default_replace("00", "52", "01", 1, copyVG99("55", "01", 1));                            //COSM A MOD1 octave -2
    };
    SY1000_default_replace("00", "52", "02", 1, copyVG99("55", "02", 1));                                //COSM A MOD1 octave direct
    SY1000_default_replace("00", "5B", "00", 3, copyVG99("55", "03", 3));                                //COSM A MOD1 rotary
    SY1000_default_replace("00", "5B", "04", 2, copyVG99("55", "06", 2));                                //COSM A MOD1 rotary rise/fall
    SY1000_default_replace("00", "5B", "03", 1, copyVG99("55", "08", 1));                                //COSM A MOD1 depth
    SY1000_default_replace("00", "43", "02", 1, convertVG99_value(copyVG99("55", "0A", 1), "01", "6C")); //COSM A MOD1 chorus rate low
    SY1000_default_replace("00", "43", "09", 1, copyVG99("55", "0A", 1));                                //COSM A MOD1 chorus rate low 1
    SY1000_default_replace("00", "43", "03", 3, copyVG99("55", "0B", 3));                                //COSM A MOD1 chorus depth/pre/level
    SY1000_default_replace("00", "43", "0A", 3, copyVG99("55", "0B", 3));                                //COSM A MOD1 chorus depth/pre/level 1
    SY1000_default_replace("00", "43", "10", 1, convertVG99_value(copyVG99("55", "0E", 1), "01", "6C"));//COSM A MOD1 chorus rate high 2
    SY1000_default_replace("00", "43", "11", 3, copyVG99("55", "0F", 3));                                //COSM A MOD1 chorus depth/pre/level 2
    SY1000_default_replace("00", "48", "01", 1, QByteArray::fromHex("00"));                              //COSM A MOD1 delay set type to stereo 1
    SY1000_default_replace("00", "48", "02", 4, convert_to_4Byte(copyVG99("55", "12", 2), 2));           //COSM A MOD1 delay time
    SY1000_default_replace("00", "48", "08", 1, copyVG99("55", "14", 1));                                //COSM A MOD1 delay level
    SY1000_default_replace("00", "46", "02", 1, convertVG99_value(copyVG99("55", "17", 1), "01", "59")); //COSM A MOD1 defretter tone
    SY1000_default_replace("00", "46", "00", 1, copyVG99("55", "17", 1));                                //COSM A MOD1 defretter sense
    SY1000_default_replace("00", "46", "04", 1, copyVG99("55", "18", 1));                                //COSM A MOD1 defretter attack
    SY1000_default_replace("00", "46", "01", 1, copyVG99("55", "19", 1));                                //COSM A MOD1 defretter depth
    SY1000_default_replace("00", "46", "05", 1, copyVG99("55", "1A", 1));                                //COSM A MOD1 defretter reso
    SY1000_default_replace("00", "46", "03", 1, copyVG99("55", "1B", 1));                                //COSM A MOD1 defretter effect
    SY1000_default_replace("00", "46", "06", 1, copyVG99("55", "1C", 1));                                //COSM A MOD1 defretter direct
    SY1000_default_replace("00", "4D", "0F", 24, copyVG99("55", "1D", 24));                              //COSM A MOD1 user scale



    SY1000_default_replace("00", "66", "00", 1, copyVG99("58", "00", 1));                                //COSM A MOD2 on/off       patch effect A
    SY1000_default_replace("00", "66", "01", 1, convertVG99_value(copyVG99("58", "01", 1), "01", "6D")); //COSM A MOD2 type
    SY1000_default_replace("00", "6B", "01", 1, convertVG99_value(copyVG99("58", "02", 1), "01", "6E")); //COSM A MOD2 comp type
    SY1000_default_replace("00", "6B", "02", 2, copyVG99("58", "03", 2));                                //COSM A MOD2 comp sustain/attack
    SY1000_default_replace("00", "6B", "06", 1, convertVG99_value(copyVG99("58", "05", 1), "01", "59")); //COSM A MOD2 comp tone
    SY1000_default_replace("00", "6B", "04", 1, copyVG99("58", "06", 1));                                //COSM A MOD2 comp level
    SY1000_default_replace("00", "76", "00", 1, copyVG99("58", "07", 1));                                //COSM A MOD2 limiter type
    SY1000_default_replace("00", "76", "04", 1, copyVG99("58", "08", 1));                                //COSM A MOD2 limiter attack
    SY1000_default_replace("00", "76", "01", 1, copyVG99("58", "09", 1));                                //COSM A MOD2 limiter threshold
    SY1000_default_replace("00", "76", "02", 1, copyVG99("58", "0A", 1));                                //COSM A MOD2 limiter ratio
    SY1000_default_replace("00", "76", "05", 1, copyVG99("58", "0B", 1));                                //COSM A MOD2 limiter release
    SY1000_default_replace("00", "76", "03", 1, copyVG99("58", "0C", 1));                                //COSM A MOD2 limiter level
    SY1000_default_replace("01", "07", "00", 5, copyVG99("58", "0D", 5));                                //COSM A MOD2 twah first 5
    SY1000_default_replace("01", "07", "06", 2, copyVG99("58", "12", 2));                                //COSM A MOD2 twah last 2
    SY1000_default_replace("00", "68", "00", 1, copyVG99("58", "14", 1));                                //COSM A MOD2 awah mode
    SY1000_default_replace("00", "68", "04", 2, copyVG99("58", "15", 2));                                //COSM A MOD2 awah freq/reso
    SY1000_default_replace("00", "68", "01", 1, convertVG99_value(copyVG99("58", "17", 1), "01", "6C")); //COSM A MOD2 awah rate
    SY1000_default_replace("00", "68", "02", 1, copyVG99("58", "18", 1));                                //COSM A MOD2 awah depth
    SY1000_default_replace("00", "68", "07", 1, copyVG99("58", "19", 1));                                //COSM A MOD2 awah direct
    SY1000_default_replace("00", "68", "03", 1, copyVG99("58", "1A", 1));                                //COSM A MOD2 awah effect
    SY1000_default_replace("01", "09", "00", 3, copyVG99("58", "1B", 3));                                //COSM A MOD2 tremolo
    SY1000_default_replace("00", "7D", "00", 1, copyVG99("58", "1E", 1));                                //COSM A MOD1 phaser
    SY1000_default_replace("00", "7D", "01", 1, convertVG99_value(copyVG99("58", "1F", 1), "01", "6C")); //COSM A MOD1 phaser rate
    SY1000_default_replace("00", "7D", "02", 1, copyVG99("58", "20", 1));                                //COSM A MOD1 phaser
    SY1000_default_replace("00", "7D", "04", 1, copyVG99("58", "21", 1));                                //COSM A MOD2 phaser manual
    SY1000_default_replace("00", "7D", "03", 1, copyVG99("58", "22", 1));                                //COSM A MOD2 phaser reso
    SY1000_default_replace("00", "7D", "05", 3, copyVG99("58", "23", 3));                                //COSM A MOD2 phaser last 3
    SY1000_default_replace("00", "6F", "00", 2, copyVG99("58", "26", 2));                                //COSM A MOD2 flanger rate/depth
    SY1000_default_replace("00", "6F", "03", 2, copyVG99("58", "28", 2));                                //COSM A MOD2 flanger manual
    SY1000_default_replace("00", "6F", "04", 1, copyVG99("58", "2A", 1));                                //COSM A MOD2 flanger separartion
    SY1000_default_replace("00", "6F", "05", 1, convertVG99_value(copyVG99("58", "2B", 1), "01", "68")); //COSM A MOD2 flanger low cut
    SY1000_default_replace("00", "6F", "00", 2, copyVG99("58", "2C", 2));                                //COSM A MOD2 flanger direct/effect
    SY1000_default_replace("00", "7A", "01", 3, copyVG99("58", "2E", 3));                                //COSM A MOD2 panner
    SY1000_default_replace("01", "0A", "00", 4, copyVG99("58", "31", 4));                                //COSM A MOD2 vibrato
    SY1000_default_replace("00", "6A", "00", 1, convertVG99_value(copyVG99("58", "35", 1), "01", "6C")); //COSM A MOD2 uni-vibe rate
    SY1000_default_replace("00", "6A", "01", 1, copyVG99("58", "36", 1));                                //COSM A MOD2 uni-vibe
    SY1000_default_replace("00", "6A", "02", 1, copyVG99("58", "37", 1));                                //COSM A MOD2 uni-vibe
    SY1000_default_replace("01", "00", "00", 2, copyVG99("58", "38", 2));                                //COSM A MOD2 ring mod
    SY1000_default_replace("01", "00", "04", 2, copyVG99("58", "3A", 2));                                //COSM A MOD2 ring mod
    SY1000_default_replace("01", "04", "00", 2, copyVG99("58", "3C", 2));                                //COSM A MOD2 slow gear
    SY1000_default_replace("00", "67", "00", 1, copyVG99("58", "3E", 1));                                //COSM A MOD2 feedbacker mode
    SY1000_default_replace("00", "67", "03", 1, copyVG99("58", "41", 1));                                //COSM A MOD2 feedbacker fb level
    SY1000_default_replace("00", "67", "01", 1, copyVG99("58", "3F", 1));                                //COSM A MOD2 feedbacker time
    SY1000_default_replace("00", "67", "02", 1, convertVG99_value(copyVG99("58", "44", 1), "01", "59")); //COSM A MOD2 feedbacker depth
    if(copyVG99("58", "01", 1).toHex()=="11")
    {
        SY1000_default_replace("00", "7B", "09", 1, scaleVG99_value(copyVG99("58", "45", 1), "64", "00", "10")); //COSM A MOD2 afb freq1
        SY1000_default_replace("00", "7B", "01", 1, scaleVG99_value(copyVG99("58", "46", 1), "64", "0C", "34")); //COSM A MOD2 afb depth1
        SY1000_default_replace("00", "7B", "03", 1, scaleVG99_value(copyVG99("58", "47", 1), "64", "00", "1D")); //COSM A MOD2 afb freq2
        SY1000_default_replace("00", "7B", "05", 1, scaleVG99_value(copyVG99("58", "48", 1), "64", "0C", "34")); //COSM A MOD2 afb depth2
        SY1000_default_replace("00", "7B", "06", 1, scaleVG99_value(copyVG99("58", "49", 1), "64", "00", "1D")); //COSM A MOD2 afb freq3
        SY1000_default_replace("00", "7B", "08", 1, scaleVG99_value(copyVG99("58", "4A", 1), "64", "0C", "34")); //COSM A MOD2 afb depth3
    };
    SY1000_default_replace("00", "74", "00", 1, scaleVG99_value(copyVG99("58", "4B", 1), "02", "00", "01")); //COSM A MOD2 humanizer mode
    SY1000_default_replace("00", "74", "01", 7, copyVG99("58", "4C", 7));                                //COSM A MOD2 humanizer
    if(copyVG99("58", "01", 1).toHex()=="15")
    {
        SY1000_default_replace("00", "7B", "09", 1, convertVG99_value(copyVG99("58", "56", 1), "01", "68")); //COSM A MOD2 eq low cut
        SY1000_default_replace("00", "7B", "01", 1, convertVG99_value(copyVG99("58", "57", 1), "01", "6A")); //COSM A MOD2 eq low gain
        SY1000_default_replace("00", "7B", "03", 1, copyVG99("58", "58", 1));                                //COSM A MOD2 eq low mid freq
        SY1000_default_replace("00", "7B", "04", 1, copyVG99("58", "59", 1));                                //COSM A MOD2 eq low mid Q
        SY1000_default_replace("00", "7B", "05", 1, convertVG99_value(copyVG99("58", "5A", 1), "01", "6A")); //COSM A MOD2 eq low mid gain
        SY1000_default_replace("00", "7B", "06", 1, copyVG99("58", "5B", 1));                                //COSM A MOD2 eq high mid freq
        SY1000_default_replace("00", "7B", "07", 1, copyVG99("58", "5C", 1));                                //COSM A MOD2 eq high mid Q
        SY1000_default_replace("00", "7B", "08", 1, convertVG99_value(copyVG99("58", "5D", 1), "01", "6A")); //COSM A MOD2 eq high mid gain
        SY1000_default_replace("00", "7B", "02", 1, convertVG99_value(copyVG99("58", "5E", 1), "01", "6A")); //COSM A MOD2 eq high gain
        SY1000_default_replace("00", "7B", "0A", 1, convertVG99_value(copyVG99("58", "5F", 1), "01", "69")); //COSM A MOD2 eq high cut
        SY1000_default_replace("00", "7B", "0B", 1, convertVG99_value(copyVG99("58", "60", 1), "01", "6A")); //COSM A MOD2 eq level
    };
    SY1000_default_replace("00", "73", "00", 2, copyVG99("58", "61", 2));                                //COSM A MOD2 harmony mode/h1
    SY1000_default_replace("00", "73", "04", 4, convert_to_4Byte(copyVG99("58", "63", 1), 2));           //COSM A MOD2 harmony pre delay 1
    SY1000_default_replace("00", "73", "03", 1, copyVG99("58", "65", 1));                                //COSM A MOD2 harmony voice 1
    SY1000_default_replace("00", "73", "02", 1, copyVG99("58", "66", 1));                                //COSM A MOD2 harmony h2
    SY1000_default_replace("00", "73", "0B", 4, convert_to_4Byte(copyVG99("58", "67", 1), 2));           //COSM A MOD2 harmony pre delay 2
    SY1000_default_replace("00", "73", "0A", 1, copyVG99("58", "69", 1));                                //COSM A MOD2 harmony level 2
    SY1000_default_replace("00", "73", "08", 2, copyVG99("58", "6A", 2));                                //COSM A MOD2 harmony fb/direct
    SY1000_default_replace("00", "7E", "00", 1, copyVG99("58", "6C", 1));                                //COSM A MOD2 pitchshifter voice 1
    SY1000_default_replace("00", "7E", "04", 1, copyVG99("58", "6D", 1));                                //COSM A MOD2 pitchshifter mode 1
    SY1000_default_replace("00", "7E", "01", 1, convertVG99_value(copyVG99("58", "6E", 1), "01", "58")); //COSM A MOD2 pitchshifter pitch 1
    SY1000_default_replace("00", "7E", "05", 1, convertVG99_value(copyVG99("58", "6F", 1), "01", "59")); //COSM A MOD2 pitchshifter fine 1
    SY1000_default_replace("00", "7E", "06", 4, convert_to_4Byte(copyVG99("58", "70", 1), 2));           //COSM A MOD2 pitchshifter pre delay 1
    SY1000_default_replace("00", "7E", "0A", 1, copyVG99("58", "72", 1));                                //COSM A MOD2 pitchshifter level 1
    SY1000_default_replace("00", "7E", "0C", 1, copyVG99("58", "73", 1));                                //COSM A MOD2 pitchshifter mode 2
    SY1000_default_replace("00", "7E", "02", 1, convertVG99_value(copyVG99("58", "74", 1), "01", "58")); //COSM A MOD2 pitchshifter pitch 2
    SY1000_default_replace("00", "7E", "0D", 1, convertVG99_value(copyVG99("58", "75", 1), "01", "59")); //COSM A MOD2 pitchshifter fine 2
    SY1000_default_replace("00", "7E", "0E", 4, convert_to_4Byte(copyVG99("58", "76", 1), 2));           //COSM A MOD2 pitchshifter pre delay 2
    SY1000_default_replace("00", "7E", "12", 1, copyVG99("58", "78", 1));                                //COSM A MOD2 pitchshifter level 2
    SY1000_default_replace("00", "7E", "0B", 1, copyVG99("58", "79", 1));                                //COSM A MOD2 pitchshifter feedback
    SY1000_default_replace("00", "7C", "00", 1, convertVG99_value(copyVG99("58", "7C", 1), "01", "58")); //COSM A MOD2 pedalbend pitch max
    SY1000_default_replace("00", "7C", "02", 4, convert_to_4Byte(copyVG99("58", "7D", 1), 1));           //COSM A MOD2 pedalbend position
    SY1000_default_replace("00", "7C", "01", 1, copyVG99("58", "7E", 1));                                //COSM A MOD2 pedalbend effect
    SY1000_default_replace("00", "7C", "06", 1, copyVG99("58", "7F", 1));                                //COSM A MOD2 pedalbend direct
    SY1000_default_replace("00", "78", "00", 1, QByteArray::fromHex("00"));                              //COSM A MOD2 octave -1 set to 00
    SY1000_default_replace("00", "78", "01", 1, QByteArray::fromHex("00"));                              //COSM A MOD2 octave -2 set to 00
    if(copyVG99("59", "00", 1).toHex()!="00")
    {
        SY1000_default_replace("00", "78", "00", 1, copyVG99("59", "01", 1));                            //COSM A MOD2 octave -1
    };
    if(copyVG99("59", "00", 1).toHex()!="01")
    {
        SY1000_default_replace("00", "78", "01", 1, copyVG99("59", "01", 1));                            //COSM A MOD2 octave -2
    };
    SY1000_default_replace("00", "78", "02", 1, copyVG99("59", "02", 1));                                //COSM A MOD2 octave direct
    SY1000_default_replace("01", "01", "00", 3, copyVG99("59", "03", 3));                                //COSM A MOD2 rotary
    SY1000_default_replace("01", "01", "04", 2, copyVG99("59", "06", 2));                                //COSM A MOD2 rotary rise/fall
    SY1000_default_replace("01", "01", "03", 1, copyVG99("59", "08", 1));                                //COSM A MOD2 depth
    SY1000_default_replace("00", "69", "02", 1, convertVG99_value(copyVG99("59", "0A", 1), "01", "6C")); //COSM A MOD2 chorus rate low
    SY1000_default_replace("00", "69", "09", 1, convertVG99_value(copyVG99("59", "0A", 1), "01", "6C")); //COSM A MOD2 chorus rate low 1
    SY1000_default_replace("00", "69", "03", 3, copyVG99("59", "0B", 3));                                //COSM A MOD2 chorus depth/pre/level
    SY1000_default_replace("00", "69", "0A", 3, copyVG99("59", "0B", 3));                                //COSM A MOD2 chorus depth/pre/level 1
    SY1000_default_replace("00", "69", "10", 1, convertVG99_value(copyVG99("59", "0E", 1), "01", "6C")); //COSM A MOD2 chorus rate high 2
    SY1000_default_replace("00", "69", "11", 3, copyVG99("59", "0F", 3));                                //COSM A MOD2 chorus depth/pre/level 2
    SY1000_default_replace("00", "6E", "01", 1, QByteArray::fromHex("00"));                              //COSM A MOD2 delay set type to stereo 1
    SY1000_default_replace("00", "6E", "02", 4, convert_to_4Byte(copyVG99("59", "12", 2), 2));           //COSM A MOD2 delay time
    SY1000_default_replace("00", "6E", "08", 1, copyVG99("59", "14", 1));                                //COSM A MOD2 delay level
    SY1000_default_replace("00", "6C", "02", 1, convertVG99_value(copyVG99("59", "17", 1), "01", "59")); //COSM A MOD2 defretter tone
    SY1000_default_replace("00", "6C", "00", 1, copyVG99("59", "17", 1));                                //COSM A MOD2 defretter sense
    SY1000_default_replace("00", "6C", "04", 1, copyVG99("59", "18", 1));                                //COSM A MOD2 defretter attack
    SY1000_default_replace("00", "6C", "01", 1, copyVG99("59", "19", 1));                                //COSM A MOD2 defretter depth
    SY1000_default_replace("00", "6C", "05", 1, copyVG99("59", "1A", 1));                                //COSM A MOD2 defretter reso
    SY1000_default_replace("00", "6C", "03", 1, copyVG99("59", "1B", 1));                                //COSM A MOD2 defretter effect
    SY1000_default_replace("00", "6C", "06", 1, copyVG99("59", "1C", 1));                                //COSM A MOD2 defretter direct
    SY1000_default_replace("00", "73", "0F", 24, copyVG99("59", "1D", 24));                              //COSM A MOD2 user scale



    SY1000_default_replace("01", "0C", "00", 1, copyVG99("64", "00", 1));                                //COSM B MOD1 on/off       patch effect A
    SY1000_default_replace("01", "0C", "01", 1, convertVG99_value(copyVG99("64", "01", 1), "01", "6D")); //COSM B MOD1 type
    SY1000_default_replace("01", "11", "01", 1, convertVG99_value(copyVG99("64", "02", 1), "01", "6E")); //COSM B MOD1 comp type
    SY1000_default_replace("01", "11", "02", 2, copyVG99("64", "03", 2));                                //COSM B MOD1 comp sustain/attack
    SY1000_default_replace("01", "11", "06", 1, convertVG99_value(copyVG99("64", "05", 1), "01", "59")); //COSM B MOD1 comp tone
    SY1000_default_replace("01", "11", "04", 1, copyVG99("64", "06", 1));                                //COSM B MOD1 comp level
    SY1000_default_replace("01", "1C", "00", 1, copyVG99("64", "07", 1));                                //COSM B MOD1 limiter type
    SY1000_default_replace("01", "1C", "04", 1, copyVG99("64", "08", 1));                                //COSM B MOD1 limiter attack
    SY1000_default_replace("01", "1C", "01", 1, copyVG99("64", "09", 1));                                //COSM B MOD1 limiter threshold
    SY1000_default_replace("01", "1C", "02", 1, copyVG99("64", "0A", 1));                                //COSM B MOD1 limiter ratio
    SY1000_default_replace("01", "1C", "05", 1, copyVG99("64", "0B", 1));                                //COSM B MOD1 limiter release
    SY1000_default_replace("01", "1C", "03", 1, copyVG99("64", "0C", 1));                                //COSM B MOD1 limiter level
    SY1000_default_replace("01", "2D", "00", 5, copyVG99("64", "0D", 5));                                //COSM B MOD1 twah first 5
    SY1000_default_replace("01", "2D", "06", 2, copyVG99("64", "12", 2));                                //COSM B MOD1 twah last 2
    SY1000_default_replace("01", "0E", "00", 1, copyVG99("64", "14", 1));                                //COSM B MOD1 awah mode
    SY1000_default_replace("01", "0E", "04", 2, copyVG99("64", "15", 2));                                //COSM B MOD1 awah freq/reso
    SY1000_default_replace("01", "0E", "01", 1, convertVG99_value(copyVG99("64", "17", 1), "01", "6C")); //COSM B MOD1 awah rate
    SY1000_default_replace("01", "0E", "02", 1, copyVG99("64", "18", 1));                                //COSM B MOD1 awah depth
    SY1000_default_replace("01", "0E", "07", 1, copyVG99("64", "19", 1));                                //COSM B MOD1 awah direct
    SY1000_default_replace("01", "0E", "03", 1, copyVG99("64", "1A", 1));                                //COSM B MOD1 awah effect
    SY1000_default_replace("01", "2F", "00", 3, copyVG99("64", "1B", 3));                                //COSM B MOD1 tremolo
    SY1000_default_replace("01", "23", "00", 1, copyVG99("64", "1E", 1));                                //COSM A MOD1 phaser
    SY1000_default_replace("01", "23", "01", 1, convertVG99_value(copyVG99("64", "1F", 1), "01", "6C")); //COSM A MOD1 phaser rate
    SY1000_default_replace("01", "23", "02", 1, copyVG99("64", "20", 1));                                //COSM A MOD1 phaser
    SY1000_default_replace("01", "23", "04", 1, copyVG99("64", "21", 1));                                //COSM B MOD1 phaser manual
    SY1000_default_replace("01", "23", "03", 1, copyVG99("64", "22", 1));                                //COSM B MOD1 phaser reso
    SY1000_default_replace("01", "23", "05", 3, copyVG99("64", "23", 3));                                //COSM B MOD1 phaser last 3
    SY1000_default_replace("01", "15", "00", 2, copyVG99("64", "26", 2));                                //COSM B MOD1 flanger rate/depth
    SY1000_default_replace("01", "15", "03", 2, copyVG99("64", "28", 2));                                //COSM B MOD1 flanger manual
    SY1000_default_replace("01", "15", "04", 1, copyVG99("64", "2A", 1));                                //COSM B MOD1 flanger separartion
    SY1000_default_replace("01", "15", "05", 1, convertVG99_value(copyVG99("64", "2B", 1), "01", "68")); //COSM B MOD1 flanger low cut
    SY1000_default_replace("01", "15", "00", 2, copyVG99("64", "2C", 2));                                //COSM B MOD1 flanger direct/effect
    SY1000_default_replace("01", "20", "01", 3, copyVG99("64", "2E", 3));                                //COSM B MOD1 panner
    SY1000_default_replace("01", "30", "00", 4, copyVG99("64", "31", 4));                                //COSM B MOD1 vibrato
    SY1000_default_replace("01", "10", "00", 1, convertVG99_value(copyVG99("64", "35", 1), "01", "6C")); //COSM B MOD1 uni-vibe rate
    SY1000_default_replace("01", "10", "01", 1, copyVG99("64", "36", 1));                                //COSM B MOD1 uni-vibe
    SY1000_default_replace("01", "10", "02", 1, copyVG99("64", "37", 1));                                //COSM B MOD1 uni-vibe
    SY1000_default_replace("01", "26", "00", 2, copyVG99("64", "38", 2));                                //COSM B MOD1 ring mod
    SY1000_default_replace("01", "26", "04", 2, copyVG99("64", "3A", 2));                                //COSM B MOD1 ring mod
    SY1000_default_replace("01", "2A", "00", 2, copyVG99("64", "3C", 2));                                //COSM B MOD1 slow gear
    SY1000_default_replace("01", "0D", "00", 1, copyVG99("64", "3E", 1));                                //COSM B MOD1 feedbacker mode
    SY1000_default_replace("01", "0D", "03", 1, copyVG99("64", "41", 1));                                //COSM B MOD1 feedbacker fb level
    SY1000_default_replace("01", "0D", "01", 1, copyVG99("64", "3F", 1));                                //COSM B MOD1 feedbacker time
    SY1000_default_replace("01", "0D", "02", 1, convertVG99_value(copyVG99("64", "44", 1), "01", "59")); //COSM B MOD1 feedbacker depth
    if(copyVG99("64", "01", 1).toHex()=="11")
    {
        SY1000_default_replace("01", "21", "09", 1, scaleVG99_value(copyVG99("64", "45", 1), "64", "00", "10")); //COSM B MOD1 afb freq1
        SY1000_default_replace("01", "21", "01", 1, scaleVG99_value(copyVG99("64", "46", 1), "64", "0C", "34")); //COSM B MOD1 afb depth1
        SY1000_default_replace("01", "21", "03", 1, scaleVG99_value(copyVG99("64", "47", 1), "64", "00", "1D")); //COSM B MOD1 afb freq2
        SY1000_default_replace("01", "21", "05", 1, scaleVG99_value(copyVG99("64", "48", 1), "64", "0C", "34")); //COSM B MOD1 afb depth2
        SY1000_default_replace("01", "21", "06", 1, scaleVG99_value(copyVG99("64", "49", 1), "64", "00", "1D")); //COSM B MOD1 afb freq3
        SY1000_default_replace("01", "21", "08", 1, scaleVG99_value(copyVG99("64", "4A", 1), "64", "0C", "34")); //COSM B MOD1 afb depth3
    };
    SY1000_default_replace("01", "1A", "00", 1, scaleVG99_value(copyVG99("64", "4B", 1), "02", "00", "01")); //COSM B MOD1 humanizer mode
    SY1000_default_replace("01", "1A", "01", 7, copyVG99("64", "4C", 7));                                //COSM B MOD1 humanizer
    if(copyVG99("64", "01", 1).toHex()=="15")
    {
        SY1000_default_replace("01", "21", "09", 1, convertVG99_value(copyVG99("64", "56", 1), "01", "68")); //COSM B MOD1 eq low cut
        SY1000_default_replace("01", "21", "01", 1, convertVG99_value(copyVG99("64", "57", 1), "01", "6A")); //COSM B MOD1 eq low gain
        SY1000_default_replace("01", "21", "03", 1, copyVG99("64", "58", 1));                                //COSM B MOD1 eq low mid freq
        SY1000_default_replace("01", "21", "04", 1, copyVG99("64", "59", 1));                                //COSM B MOD1 eq low mid Q
        SY1000_default_replace("01", "21", "05", 1, convertVG99_value(copyVG99("64", "5A", 1), "01", "6A")); //COSM B MOD1 eq low mid gain
        SY1000_default_replace("01", "21", "06", 1, copyVG99("64", "5B", 1));                                //COSM B MOD1 eq high mid freq
        SY1000_default_replace("01", "21", "07", 1, copyVG99("64", "5C", 1));                                //COSM B MOD1 eq high mid Q
        SY1000_default_replace("01", "21", "08", 1, convertVG99_value(copyVG99("64", "5D", 1), "01", "6A")); //COSM B MOD1 eq high mid gain
        SY1000_default_replace("01", "21", "02", 1, convertVG99_value(copyVG99("64", "5E", 1), "01", "6A")); //COSM B MOD1 eq high gain
        SY1000_default_replace("01", "21", "0A", 1, convertVG99_value(copyVG99("64", "5F", 1), "01", "69")); //COSM B MOD1 eq high cut
        SY1000_default_replace("01", "21", "0B", 1, convertVG99_value(copyVG99("64", "60", 1), "01", "6A")); //COSM B MOD1 eq level
    };
    SY1000_default_replace("01", "19", "00", 2, copyVG99("64", "61", 2));                                //COSM B MOD1 harmony mode/h1
    SY1000_default_replace("01", "19", "04", 4, convert_to_4Byte(copyVG99("64", "63", 1), 2));           //COSM B MOD1 harmony pre delay 1
    SY1000_default_replace("01", "19", "03", 1, copyVG99("64", "65", 1));                                //COSM B MOD1 harmony voice 1
    SY1000_default_replace("01", "19", "02", 1, copyVG99("64", "66", 1));                                //COSM B MOD1 harmony h2
    SY1000_default_replace("01", "19", "0B", 4, convert_to_4Byte(copyVG99("64", "67", 1), 2));           //COSM B MOD1 harmony pre delay 2
    SY1000_default_replace("01", "19", "0A", 1, copyVG99("64", "69", 1));                                //COSM B MOD1 harmony level 2
    SY1000_default_replace("01", "19", "08", 2, copyVG99("64", "6A", 2));                                //COSM B MOD1 harmony fb/direct
    SY1000_default_replace("01", "24", "00", 1, copyVG99("64", "6C", 1));                                //COSM B MOD1 pitchshifter voice 1
    SY1000_default_replace("01", "24", "04", 1, copyVG99("64", "6D", 1));                                //COSM B MOD1 pitchshifter mode 1
    SY1000_default_replace("01", "24", "01", 1, convertVG99_value(copyVG99("64", "6E", 1), "01", "58")); //COSM B MOD1 pitchshifter pitch 1
    SY1000_default_replace("01", "24", "05", 1, convertVG99_value(copyVG99("64", "6F", 1), "01", "59")); //COSM B MOD1 pitchshifter fine 1
    SY1000_default_replace("01", "24", "06", 4, convert_to_4Byte(copyVG99("64", "70", 1), 2));           //COSM B MOD1 pitchshifter pre delay 1
    SY1000_default_replace("01", "24", "0A", 1, copyVG99("64", "72", 1));                                //COSM B MOD1 pitchshifter level 1
    SY1000_default_replace("01", "24", "0C", 1, copyVG99("64", "73", 1));                                //COSM B MOD1 pitchshifter mode 2
    SY1000_default_replace("01", "24", "02", 1, convertVG99_value(copyVG99("64", "74", 1), "01", "58")); //COSM B MOD1 pitchshifter pitch 2
    SY1000_default_replace("01", "24", "0D", 1, convertVG99_value(copyVG99("64", "75", 1), "01", "59")); //COSM B MOD1 pitchshifter fine 2
    SY1000_default_replace("01", "24", "0E", 4, convert_to_4Byte(copyVG99("64", "76", 1), 2));           //COSM B MOD1 pitchshifter pre delay 2
    SY1000_default_replace("01", "24", "12", 1, copyVG99("64", "78", 1));                                //COSM B MOD1 pitchshifter level 2
    SY1000_default_replace("01", "24", "0B", 1, copyVG99("64", "79", 1));                                //COSM B MOD1 pitchshifter feedback
    SY1000_default_replace("01", "22", "00", 1, convertVG99_value(copyVG99("64", "7C", 1), "01", "58")); //COSM B MOD1 pedalbend pitch max
    SY1000_default_replace("01", "22", "02", 4, convert_to_4Byte(copyVG99("64", "7D", 1), 1));           //COSM B MOD1 pedalbend position
    SY1000_default_replace("01", "22", "01", 1, copyVG99("64", "7E", 1));                                //COSM B MOD1 pedalbend effect
    SY1000_default_replace("01", "22", "06", 1, copyVG99("64", "7F", 1));                                //COSM B MOD1 pedalbend direct
    SY1000_default_replace("01", "1E", "00", 1, QByteArray::fromHex("00"));                              //COSM B MOD1 octave -1 set to 00
    SY1000_default_replace("01", "1E", "01", 1, QByteArray::fromHex("00"));                              //COSM B MOD1 octave -2 set to 00
    if(copyVG99("65", "00", 1).toHex()!="00")
    {
        SY1000_default_replace("01", "1E", "00", 1, copyVG99("65", "01", 1));                            //COSM B MOD1 octave -1
    };
    if(copyVG99("65", "00", 1).toHex()!="01")
    {
        SY1000_default_replace("01", "1E", "01", 1, copyVG99("65", "01", 1));                            //COSM B MOD1 octave -2
    };
    SY1000_default_replace("01", "1E", "02", 1, copyVG99("65", "02", 1));                                //COSM B MOD1 octave direct
    SY1000_default_replace("01", "27", "00", 3, copyVG99("65", "03", 3));                                //COSM B MOD1 rotary
    SY1000_default_replace("01", "27", "04", 2, copyVG99("65", "06", 2));                                //COSM B MOD1 rotary rise/fall
    SY1000_default_replace("01", "27", "03", 1, copyVG99("65", "08", 1));                                //COSM B MOD1 depth
    SY1000_default_replace("01", "0F", "02", 1, convertVG99_value(copyVG99("65", "0A", 1), "01", "6C")); //COSM B MOD1 chorus rate low
    SY1000_default_replace("01", "0F", "09", 1, convertVG99_value(copyVG99("65", "0A", 1), "01", "6C")); //COSM B MOD1 chorus rate low 1
    SY1000_default_replace("01", "0F", "03", 3, copyVG99("65", "0B", 3));                                //COSM B MOD1 chorus depth/pre/level
    SY1000_default_replace("01", "0F", "0A", 3, copyVG99("65", "0B", 3));                                //COSM B MOD1 chorus depth/pre/level 1
    SY1000_default_replace("01", "0F", "10", 1, convertVG99_value(copyVG99("65", "0E", 1), "01", "6C")); //COSM B MOD1 chorus rate high 2
    SY1000_default_replace("01", "0F", "11", 3, copyVG99("65", "0F", 3));                                //COSM B MOD1 chorus depth/pre/level 2
    SY1000_default_replace("01", "14", "01", 1, QByteArray::fromHex("00"));                              //COSM B MOD1 delay set type to stereo 1
    SY1000_default_replace("01", "14", "02", 4, convert_to_4Byte(copyVG99("65", "12", 2), 2));           //COSM B MOD1 delay time
    SY1000_default_replace("01", "14", "08", 1, copyVG99("65", "14", 1));                                //COSM B MOD1 delay level
    SY1000_default_replace("01", "12", "02", 1, convertVG99_value(copyVG99("65", "17", 1), "01", "59")); //COSM B MOD1 defretter tone
    SY1000_default_replace("01", "12", "00", 1, copyVG99("65", "17", 1));                                //COSM B MOD1 defretter sense
    SY1000_default_replace("01", "12", "04", 1, copyVG99("65", "18", 1));                                //COSM B MOD1 defretter attack
    SY1000_default_replace("01", "12", "01", 1, copyVG99("65", "19", 1));                                //COSM B MOD1 defretter depth
    SY1000_default_replace("01", "12", "05", 1, copyVG99("65", "1A", 1));                                //COSM B MOD1 defretter reso
    SY1000_default_replace("01", "12", "03", 1, copyVG99("65", "1B", 1));                                //COSM B MOD1 defretter effect
    SY1000_default_replace("01", "12", "06", 1, copyVG99("65", "1C", 1));                                //COSM B MOD1 defretter direct
    SY1000_default_replace("01", "19", "0F", 24, copyVG99("65", "1D", 24));                              //COSM B MOD1 user scale


    if(copyVG99("64", "00", 1).toHex()=="00")   // if MOD1 is not used, then use this for MOD2
    {
        SY1000_default_replace("01", "0C", "00", 1, copyVG99("68", "00", 1));                                //COSM B MOD2 on/off       patch effect A
        SY1000_default_replace("01", "0C", "01", 1, convertVG99_value(copyVG99("68", "01", 1), "01", "6D")); //COSM B MOD2 type
        SY1000_default_replace("01", "11", "01", 1, convertVG99_value(copyVG99("68", "02", 1), "01", "6E")); //COSM B MOD2 comp type
        SY1000_default_replace("01", "11", "02", 2, copyVG99("68", "03", 2));                                //COSM B MOD2 comp sustain/attack
        SY1000_default_replace("01", "11", "06", 1, convertVG99_value(copyVG99("68", "05", 1), "01", "59")); //COSM B MOD2 comp tone
        SY1000_default_replace("01", "11", "04", 1, copyVG99("68", "06", 1));                                //COSM B MOD2 comp level
        SY1000_default_replace("01", "1C", "00", 1, copyVG99("68", "07", 1));                                //COSM B MOD2 limiter type
        SY1000_default_replace("01", "1C", "04", 1, copyVG99("68", "08", 1));                                //COSM B MOD2 limiter attack
        SY1000_default_replace("01", "1C", "01", 1, copyVG99("68", "09", 1));                                //COSM B MOD2 limiter threshold
        SY1000_default_replace("01", "1C", "02", 1, copyVG99("68", "0A", 1));                                //COSM B MOD2 limiter ratio
        SY1000_default_replace("01", "1C", "05", 1, copyVG99("68", "0B", 1));                                //COSM B MOD2 limiter release
        SY1000_default_replace("01", "1C", "03", 1, copyVG99("68", "0C", 1));                                //COSM B MOD2 limiter level
        SY1000_default_replace("01", "2D", "00", 5, copyVG99("68", "0D", 5));                                //COSM B MOD2 twah first 5
        SY1000_default_replace("01", "2D", "06", 2, copyVG99("68", "12", 2));                                //COSM B MOD2 twah last 2
        SY1000_default_replace("01", "0E", "00", 1, copyVG99("68", "14", 1));                                //COSM B MOD2 awah mode
        SY1000_default_replace("01", "0E", "04", 2, copyVG99("68", "15", 2));                                //COSM B MOD2 awah freq/reso
        SY1000_default_replace("01", "0E", "01", 1, convertVG99_value(copyVG99("68", "17", 1), "01", "6C")); //COSM B MOD2 awah rate
        SY1000_default_replace("01", "0E", "02", 1, copyVG99("68", "18", 1));                                //COSM B MOD2 awah depth
        SY1000_default_replace("01", "0E", "07", 1, copyVG99("68", "19", 1));                                //COSM B MOD2 awah direct
        SY1000_default_replace("01", "0E", "03", 1, copyVG99("68", "1A", 1));                                //COSM B MOD2 awah effect
        SY1000_default_replace("01", "2F", "00", 3, copyVG99("68", "1B", 3));                                //COSM B MOD2 tremolo
        SY1000_default_replace("01", "23", "00", 1, copyVG99("68", "1E", 1));                                //COSM A MOD1 phaser
        SY1000_default_replace("01", "23", "01", 1, convertVG99_value(copyVG99("68", "1F", 1), "01", "6C")); //COSM A MOD1 phaser rate
        SY1000_default_replace("01", "23", "02", 1, copyVG99("68", "20", 1));                                //COSM A MOD1 phaser
        SY1000_default_replace("01", "23", "04", 1, copyVG99("68", "21", 1));                                //COSM B MOD2 phaser manual
        SY1000_default_replace("01", "23", "03", 1, copyVG99("68", "22", 1));                                //COSM B MOD2 phaser reso
        SY1000_default_replace("01", "23", "05", 3, copyVG99("68", "23", 3));                                //COSM B MOD2 phaser last 3
        SY1000_default_replace("01", "15", "00", 2, copyVG99("68", "26", 2));                                //COSM B MOD2 flanger rate/depth
        SY1000_default_replace("01", "15", "03", 2, copyVG99("68", "28", 2));                                //COSM B MOD2 flanger manual
        SY1000_default_replace("01", "15", "04", 1, copyVG99("68", "2A", 1));                                //COSM B MOD2 flanger separartion
        SY1000_default_replace("01", "15", "05", 1, convertVG99_value(copyVG99("68", "2B", 1), "01", "68")); //COSM B MOD2 flanger low cut
        SY1000_default_replace("01", "15", "00", 2, copyVG99("68", "2C", 2));                                //COSM B MOD2 flanger direct/effect
        SY1000_default_replace("01", "20", "01", 3, copyVG99("68", "2E", 3));                                //COSM B MOD2 panner
        SY1000_default_replace("01", "30", "00", 4, copyVG99("68", "31", 4));                                //COSM B MOD2 vibrato
        //SY1000_default_replace("01", "10", "00", 1, convertVG99_value(copyVG99("68", "35", 1), "01", "6C")); //COSM B MOD2 uni-vibe rate
        SY1000_default_replace("01", "10", "01", 1, copyVG99("68", "36", 1));                                //COSM B MOD2 uni-vibe
        SY1000_default_replace("01", "10", "02", 1, copyVG99("68", "37", 1));                                //COSM B MOD2 uni-vibe
        SY1000_default_replace("01", "26", "00", 2, copyVG99("68", "38", 2));                                //COSM B MOD2 ring mod
        SY1000_default_replace("01", "26", "04", 2, copyVG99("68", "3A", 2));                                //COSM B MOD2 ring mod
        SY1000_default_replace("01", "2A", "00", 2, copyVG99("68", "3C", 2));                                //COSM B MOD2 slow gear
        SY1000_default_replace("01", "0D", "00", 1, copyVG99("68", "3E", 1));                                //COSM B MOD2 feedbacker mode
        SY1000_default_replace("01", "0D", "03", 1, copyVG99("68", "41", 1));                                //COSM B MOD2 feedbacker fb level
        SY1000_default_replace("01", "0D", "01", 1, copyVG99("68", "3F", 1));                                //COSM B MOD2 feedbacker time
        SY1000_default_replace("01", "0D", "02", 1, convertVG99_value(copyVG99("68", "44", 1), "01", "59")); //COSM B MOD2 feedbacker depth
        if(copyVG99("68", "01", 1).toHex()=="11")
        {
            SY1000_default_replace("01", "21", "09", 1, scaleVG99_value(copyVG99("68", "45", 1), "64", "00", "10")); //COSM B MOD2 afb freq1
            SY1000_default_replace("01", "21", "01", 1, scaleVG99_value(copyVG99("68", "46", 1), "64", "0C", "34")); //COSM B MOD2 afb depth1
            SY1000_default_replace("01", "21", "03", 1, scaleVG99_value(copyVG99("68", "47", 1), "64", "00", "1D")); //COSM B MOD2 afb freq2
            SY1000_default_replace("01", "21", "05", 1, scaleVG99_value(copyVG99("68", "48", 1), "64", "0C", "34")); //COSM B MOD2 afb depth2
            SY1000_default_replace("01", "21", "06", 1, scaleVG99_value(copyVG99("68", "49", 1), "64", "00", "1D")); //COSM B MOD2 afb freq3
            SY1000_default_replace("01", "21", "08", 1, scaleVG99_value(copyVG99("68", "4A", 1), "64", "0C", "34")); //COSM B MOD2 afb depth3
        };
        SY1000_default_replace("01", "1A", "00", 1, scaleVG99_value(copyVG99("68", "4B", 1), "02", "00", "01")); //COSM B MOD2 humanizer mode
        SY1000_default_replace("01", "1A", "01", 7, copyVG99("68", "4C", 7));                                    //COSM B MOD2 humanizer
        if(copyVG99("68", "01", 1).toHex()=="15")
        {
            SY1000_default_replace("01", "21", "09", 1, convertVG99_value(copyVG99("68", "56", 1), "01", "68")); //COSM B MOD2 eq low cut
            SY1000_default_replace("01", "21", "01", 1, convertVG99_value(copyVG99("68", "57", 1), "01", "6A")); //COSM B MOD2 eq low gain
            SY1000_default_replace("01", "21", "03", 1, copyVG99("68", "58", 1));                                //COSM B MOD2 eq low mid freq
            SY1000_default_replace("01", "21", "04", 1, copyVG99("68", "59", 1));                                //COSM B MOD2 eq low mid Q
            SY1000_default_replace("01", "21", "05", 1, convertVG99_value(copyVG99("68", "5A", 1), "01", "6A")); //COSM B MOD2 eq low mid gain
            SY1000_default_replace("01", "21", "06", 1, copyVG99("68", "5B", 1));                                //COSM B MOD2 eq high mid freq
            SY1000_default_replace("01", "21", "07", 1, copyVG99("68", "5C", 1));                                //COSM B MOD2 eq high mid Q
            SY1000_default_replace("01", "21", "08", 1, convertVG99_value(copyVG99("68", "5D", 1), "01", "6A")); //COSM B MOD2 eq high mid gain
            SY1000_default_replace("01", "21", "02", 1, convertVG99_value(copyVG99("68", "5E", 1), "01", "6A")); //COSM B MOD2 eq high gain
            SY1000_default_replace("01", "21", "0A", 1, convertVG99_value(copyVG99("68", "5F", 1), "01", "69")); //COSM B MOD2 eq high cut
            SY1000_default_replace("01", "21", "0B", 1, convertVG99_value(copyVG99("68", "60", 1), "01", "6A")); //COSM B MOD2 eq level
        };
        SY1000_default_replace("01", "19", "00", 2, copyVG99("68", "61", 2));                                //COSM B MOD2 harmony mode/h1
        SY1000_default_replace("01", "19", "04", 4, convert_to_4Byte(copyVG99("68", "63", 1), 2));           //COSM B MOD2 harmony pre delay 1
        SY1000_default_replace("01", "19", "03", 1, copyVG99("68", "65", 1));                                //COSM B MOD2 harmony voice 1
        SY1000_default_replace("01", "19", "02", 1, copyVG99("68", "66", 1));                                //COSM B MOD2 harmony h2
        SY1000_default_replace("01", "19", "0B", 4, convert_to_4Byte(copyVG99("68", "67", 1), 2));           //COSM B MOD2 harmony pre delay 2
        SY1000_default_replace("01", "19", "0A", 1, copyVG99("68", "69", 1));                                //COSM B MOD2 harmony level 2
        SY1000_default_replace("01", "19", "08", 2, copyVG99("68", "6A", 2));                                //COSM B MOD2 harmony fb/direct
        SY1000_default_replace("01", "24", "00", 1, copyVG99("68", "6C", 1));                                //COSM B MOD2 pitchshifter voice 1
        SY1000_default_replace("01", "24", "04", 1, copyVG99("68", "6D", 1));                                //COSM B MOD2 pitchshifter mode 1
        SY1000_default_replace("01", "24", "01", 1, convertVG99_value(copyVG99("68", "6E", 1), "01", "58")); //COSM B MOD2 pitchshifter pitch 1
        SY1000_default_replace("01", "24", "05", 1, convertVG99_value(copyVG99("68", "6F", 1), "01", "59")); //COSM B MOD2 pitchshifter fine 1
        SY1000_default_replace("01", "24", "06", 4, convert_to_4Byte(copyVG99("68", "70", 1), 2));           //COSM B MOD2 pitchshifter pre delay 1
        SY1000_default_replace("01", "24", "0A", 1, copyVG99("68", "72", 1));                                //COSM B MOD2 pitchshifter level 1
        SY1000_default_replace("01", "24", "0C", 1, copyVG99("68", "73", 1));                                //COSM B MOD2 pitchshifter mode 2
        SY1000_default_replace("01", "24", "02", 1, convertVG99_value(copyVG99("68", "74", 1), "01", "58")); //COSM B MOD2 pitchshifter pitch 2
        SY1000_default_replace("01", "24", "0D", 1, convertVG99_value(copyVG99("68", "75", 1), "01", "59")); //COSM B MOD2 pitchshifter fine 2
        SY1000_default_replace("01", "24", "0E", 4, convert_to_4Byte(copyVG99("68", "76", 1), 2));           //COSM B MOD2 pitchshifter pre delay 2
        SY1000_default_replace("01", "24", "12", 1, copyVG99("68", "78", 1));                                //COSM B MOD2 pitchshifter level 2
        SY1000_default_replace("01", "24", "0B", 1, copyVG99("68", "79", 1));                                //COSM B MOD2 pitchshifter feedback
        SY1000_default_replace("01", "22", "00", 1, convertVG99_value(copyVG99("68", "7C", 1), "01", "58")); //COSM B MOD2 pedalbend pitch max
        SY1000_default_replace("01", "22", "02", 4, convert_to_4Byte(copyVG99("68", "7D", 1), 1));           //COSM B MOD2 pedalbend position
        SY1000_default_replace("01", "22", "01", 1, copyVG99("68", "7E", 1));                                //COSM B MOD2 pedalbend effect
        SY1000_default_replace("01", "22", "06", 1, copyVG99("68", "7F", 1));                                //COSM B MOD2 pedalbend direct
        SY1000_default_replace("01", "1E", "00", 1, QByteArray::fromHex("00"));                              //COSM B MOD2 octave -1 set to 00
        SY1000_default_replace("01", "1E", "01", 1, QByteArray::fromHex("00"));                              //COSM B MOD2 octave -2 set to 00
        if(copyVG99("69", "00", 1).toHex()!="00")
        {
            SY1000_default_replace("01", "1E", "00", 1, copyVG99("69", "01", 1));                            //COSM B MOD2 octave -1
        };
        if(copyVG99("69", "00", 1).toHex()!="01")
        {
            SY1000_default_replace("01", "1E", "01", 1, copyVG99("69", "01", 1));                            //COSM B MOD2 octave -2
        };
        SY1000_default_replace("01", "1E", "02", 1, copyVG99("69", "02", 1));                                //COSM B MOD2 octave direct
        SY1000_default_replace("01", "27", "00", 3, copyVG99("69", "03", 3));                                //COSM B MOD2 rotary
        SY1000_default_replace("01", "27", "04", 2, copyVG99("69", "06", 2));                                //COSM B MOD2 rotary rise/fall
        SY1000_default_replace("01", "27", "03", 1, copyVG99("69", "08", 1));                                //COSM B MOD2 depth
        SY1000_default_replace("01", "0F", "02", 1, convertVG99_value(copyVG99("69", "0A", 1), "01", "6C")); //COSM B MOD2 chorus rate low
        SY1000_default_replace("01", "0F", "09", 1, convertVG99_value(copyVG99("69", "0A", 1), "01", "6C")); //COSM B MOD2 chorus rate low 1
        SY1000_default_replace("01", "0F", "03", 3, copyVG99("69", "0B", 3));                                //COSM B MOD2 chorus depth/pre/level
        SY1000_default_replace("01", "0F", "0A", 3, copyVG99("69", "0B", 3));                                //COSM B MOD2 chorus depth/pre/level 1
        SY1000_default_replace("01", "0F", "10", 1, convertVG99_value(copyVG99("69", "0A", 1), "01", "6C")); //COSM B MOD2 chorus rate high 2
        SY1000_default_replace("01", "0F", "11", 3, copyVG99("69", "0F", 3));                                //COSM B MOD2 chorus depth/pre/level 2
        SY1000_default_replace("01", "14", "01", 1, QByteArray::fromHex("00"));                              //COSM B MOD2 delay set type to stereo 1
        SY1000_default_replace("01", "14", "02", 4, convert_to_4Byte(copyVG99("69", "12", 2), 2));           //COSM B MOD2 delay time
        SY1000_default_replace("01", "14", "08", 1, copyVG99("69", "14", 1));                                //COSM B MOD2 delay level
        SY1000_default_replace("01", "12", "02", 1, convertVG99_value(copyVG99("69", "17", 1), "01", "59")); //COSM B MOD2 defretter tone
        SY1000_default_replace("01", "12", "00", 1, copyVG99("69", "17", 1));                                //COSM B MOD2 defretter sense
        SY1000_default_replace("01", "12", "04", 1, copyVG99("69", "18", 1));                                //COSM B MOD2 defretter attack
        SY1000_default_replace("01", "12", "01", 1, copyVG99("69", "19", 1));                                //COSM B MOD2 defretter depth
        SY1000_default_replace("01", "12", "05", 1, copyVG99("69", "1A", 1));                                //COSM B MOD2 defretter reso
        SY1000_default_replace("01", "12", "03", 1, copyVG99("69", "1B", 1));                                //COSM B MOD2 defretter effect
        SY1000_default_replace("01", "12", "06", 1, copyVG99("69", "1C", 1));                                //COSM B MOD2 defretter direct
        SY1000_default_replace("01", "19", "0F", 24, copyVG99("69", "1D", 24));                              //COSM B MOD2 user scalet
    };

    SY1000_default_replace("00", "12", "45", 34,QByteArrayLiteral("\x02\x01\x03\x00\x18\x18\x18\x02\x18\x04\x05\x01\x00\x0C\x03\x0E\x07\x18\x06\x0F\x0A\x08\x18\x09\x0B\x10\x0D\x11\x12\x13\x14\x15\x16\x17"));
    // 02 01 03 00 18 18 18 02 18 04 05 01 00 0C 03 0E 07 18 06 0F 0A 08 18 09 0B 10 0D 11 12 13 14 15 16 17
    // qDebug() << "VG-99 convert" << copyVG99("00", "00", 16).data();

    QString moad = "Guitar";
    if(bass_mode == true)
    {
        moad = "Bass";
    };
    QString vx ="VG-99";
    if(this->data.size()==6060)
    {
        vx = "VB-99";
    };
    SysxIO *sysxIO = SysxIO::Instance();
    sysxIO->deBug("(" + moad +" Mode)" + vx + " conversion size = " + QString::number(this->SY1000_default.size(), 10));

    if(this->SY1000_default.size() == patchSize || this->SY1000_default.size() == patchSizeBass)
    {
        return(this->SY1000_default);
    }
    else
    {
        sysxIO->deBug("SHIT HAPPENED IN VG-99/VB-99 FILE TRANSLATION");

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
};

void tsl_VG99_VB99_translation::convert_VG99mid_to_VG99default(int patch)
{
    Q_UNUSED(patch);
    if(data.size()==5923)
    {
        data = data.prepend(data.mid(0, 7));
    };
    bool ok;

    QString type = "a";
    if(data.mid(31, 1).toHex().toUpper().contains("60")) type = "b";

    if(type == "a")
    {
        VG99_default.replace(11, 16, data.mid(42, 16)); // 00 00
        VG99_default.replace(27, 22, data.mid(73, 22)); // 00 10
        VG99_default.replace(49, 46, data.mid(110, 46)); // 00 26
        VG99_default.replace(108, 10, data.mid(171, 10)); // 01 00
        VG99_default.replace(118, 10, data.mid(196, 10)); // 01 0A
        VG99_default.replace(128, 10, data.mid(221, 10)); // 01 14
        VG99_default.replace(138, 10, data.mid(246, 10)); // 01 1E
        VG99_default.replace(148, 10, data.mid(271, 10)); // 01 28
        VG99_default.replace(158, 10, data.mid(296, 10)); // 01 32
        VG99_default.replace(168, 10, data.mid(321, 10)); // 01 3C
        VG99_default.replace(178, 10, data.mid(346, 10)); // 01 46
        VG99_default.replace(188, 10, data.mid(371, 10)); // 01 50
        VG99_default.replace(198, 10, data.mid(396, 10)); // 01 5A
        VG99_default.replace(208, 10, data.mid(421, 10)); // 01 64
        VG99_default.replace(218, 10, data.mid(446, 10)); // 01 6E
        VG99_default.replace(228, 8, data.mid(471, 8));   // 01 78
        VG99_default.replace(249, 2, data.mid(479, 2));   // (02 00)
        VG99_default.replace(251, 10, data.mid(496, 10)); // 02 02
        VG99_default.replace(274, 10, data.mid(521, 10)); // 03 00
        VG99_default.replace(284, 10, data.mid(546, 10)); // 03 0A
        VG99_default.replace(294, 10, data.mid(571, 10)); // 03 14
        VG99_default.replace(304, 10, data.mid(596, 10)); // 03 1E
        VG99_default.replace(314, 10, data.mid(621, 10)); // 03 28
        VG99_default.replace(324, 10, data.mid(646, 10)); // 03 32
        VG99_default.replace(334, 10, data.mid(671, 10)); // 03 3C
        VG99_default.replace(344, 10, data.mid(696, 10)); // 03 46
        VG99_default.replace(367, 10, data.mid(721, 10)); // 05 00
        VG99_default.replace(377, 10, data.mid(746, 10)); // 05 0A
        VG99_default.replace(387, 10, data.mid(771, 10)); // 05 14
        VG99_default.replace(397, 10, data.mid(796, 10)); // 05 1E
        VG99_default.replace(407, 10, data.mid(821, 10)); // 05 28
        VG99_default.replace(417, 10, data.mid(846, 10)); // 05 32
        VG99_default.replace(427, 10, data.mid(871, 10)); // 05 3C
        VG99_default.replace(437, 10, data.mid(896, 10)); // 05 46
        VG99_default.replace(460, 10, data.mid(921, 10)); // 06 00
        VG99_default.replace(470, 10, data.mid(946, 10)); // 06 0A
        VG99_default.replace(480, 10, data.mid(971, 10)); // 06 14
        VG99_default.replace(490, 10, data.mid(996, 10)); // 06 1E
        VG99_default.replace(500, 10, data.mid(1021, 10)); // 06 28
        VG99_default.replace(510, 10, data.mid(1046, 10)); // 06 32
        VG99_default.replace(520, 10, data.mid(1071, 10)); // 06 3C
        VG99_default.replace(530, 10, data.mid(1096, 10)); // 06 46
        VG99_default.replace(540, 10, data.mid(1121, 10)); // 06 50
        VG99_default.replace(550, 10, data.mid(1146, 10)); // 06 5A
        VG99_default.replace(560, 10, data.mid(1171, 10)); // 06 64
        VG99_default.replace(570, 10, data.mid(1196, 10)); // 06 6e
        VG99_default.replace(580, 8, data.mid(1221, 8));   // 06 78
        VG99_default.replace(601, 2, data.mid(1229, 2));   // (07 00)
        VG99_default.replace(603, 10, data.mid(1246, 10)); // 07 02
        VG99_default.replace(613, 10, data.mid(1271, 10)); // 07 0C
        VG99_default.replace(623, 10, data.mid(1296, 10)); // 07 16
        VG99_default.replace(646, 10, data.mid(1321, 10)); // 0D 00
        VG99_default.replace(656, 10, data.mid(1346, 10)); // 0D 0A
        VG99_default.replace(666, 10, data.mid(1371, 10)); // 0D 14
        VG99_default.replace(676, 2, data.mid(1396, 2));   // 0D 1E
        VG99_default.replace(678, 2, data.mid(1413, 2));   // 0D 20
        VG99_default.replace(680, 4, data.mid(1430, 4));   // 0D 22
        VG99_default.replace(684, 1, data.mid(1449, 1));   // 0D 26
        VG99_default.replace(685, 4, data.mid(1465, 4));   // 0D 27
        VG99_default.replace(689, 4, data.mid(1484, 4));   // 0D 2B
        VG99_default.replace(693, 6, data.mid(1503, 6));   // 0D 2F
        VG99_default.replace(699, 1, data.mid(1524, 1));   // 0D 35
        VG99_default.replace(700, 2, data.mid(1540, 2));   // 0D 36
        VG99_default.replace(702, 4, data.mid(1557, 4));   // 0D 38
        VG99_default.replace(706, 6, data.mid(1576, 6));   // 0D 3C
        VG99_default.replace(725, 1, data.mid(1597, 1));   // 10 00
        VG99_default.replace(726, 8, data.mid(1613, 8));   // 10 01
        VG99_default.replace(734, 8, data.mid(1636, 8));   // 10 09
        VG99_default.replace(742, 6, data.mid(1659, 6));   // 10 11
        VG99_default.replace(748, 6, data.mid(1680, 6));   // 10 17
        VG99_default.replace(754, 12, data.mid(1701, 12)); // 10 1D
        VG99_default.replace(766, 12, data.mid(1728, 12)); // 10 29
        VG99_default.replace(778, 12, data.mid(1755, 12)); // 10 35
        VG99_default.replace(790, 6, data.mid(1782, 6));   // 10 41
        VG99_default.replace(796, 10, data.mid(1803, 10)); // 10 47
        VG99_default.replace(806, 6, data.mid(1828, 6));   // 10 51
        VG99_default.replace(812, 6, data.mid(1849, 6));   // 10 57
        VG99_default.replace(818, 12, data.mid(1870, 12)); // 10 5D
        VG99_default.replace(830, 12, data.mid(1897, 12)); // 10 69
        VG99_default.replace(842, 11, data.mid(1924, 11)); // 10 75
        VG99_default.replace(866, 1, data.mid(1935, 1));   // 11 00
        VG99_default.replace(867, 6, data.mid(1951, 6));   // 11 01
        VG99_default.replace(873, 10, data.mid(1972, 10)); // 11 07
        VG99_default.replace(896, 7, data.mid(1997, 7));   // 20 00
        VG99_default.replace(903, 13, data.mid(2019, 13)); // 20 07
        VG99_default.replace(916, 6, data.mid(2047, 6));   // 20 14
        VG99_default.replace(922, 6, data.mid(2068, 6));   // 20 1A
        VG99_default.replace(928, 8, data.mid(2089, 8));   // 20 20
        VG99_default.replace(936, 7, data.mid(2112, 7));   // 20 28
        VG99_default.replace(943, 10, data.mid(2134, 10)); // 20 2F
        VG99_default.replace(966, 27, data.mid(2159, 27)); // 30 00
        VG99_default.replace(993, 42, data.mid(2201, 42)); // 30 1B
        VG99_default.replace(1035, 36, data.mid(2258, 36)); // 30 45
        VG99_default.replace(1071, 11, data.mid(2309, 11)); // 30 69
        VG99_default.replace(1095, 51, data.mid(2335, 51)); // 34 00
        VG99_default.replace(1146, 36, data.mid(2401, 36)); // 34 33
        VG99_default.replace(1182, 3, data.mid(2452, 3));   // 34 57
        VG99_default.replace(1198, 27, data.mid(2470, 27)); // 38 00
        VG99_default.replace(1225, 42, data.mid(2512, 42)); // 38 1B
        VG99_default.replace(1267, 36, data.mid(2569, 36)); // 38 45
        VG99_default.replace(1303, 11, data.mid(2620, 11)); // 38 69
        VG99_default.replace(1327, 51, data.mid(2646, 51)); // 3C 00
        VG99_default.replace(1378, 36, data.mid(2712, 36)); // 3C 33
        VG99_default.replace(1414, 3, data.mid(2763, 3));   // 3C 57
        VG99_default.replace(1430, 3, data.mid(2781, 3));   // 40 00
        VG99_default.replace(1433, 8, data.mid(2799, 8));   // 40 03
        VG99_default.replace(1441, 18, data.mid(2822, 18)); // 40 0B
        VG99_default.replace(1459, 18, data.mid(2855, 18)); // 40 1D
        VG99_default.replace(1477, 2, data.mid(2888, 2));   // 40 2F
        VG99_default.replace(1479, 3, data.mid(2905, 3));   // 40 32
        VG99_default.replace(1495, 13, data.mid(2923, 13)); // 50 00
        VG99_default.replace(1508, 30, data.mid(2951, 30)); // 50 0D
        VG99_default.replace(1538, 8, data.mid(2996, 8));   // 50 2B
        VG99_default.replace(1546, 12, data.mid(3019, 12)); // 50 33
        VG99_default.replace(1558, 9, data.mid(3046, 9));   // 50 3F
        VG99_default.replace(1567, 12, data.mid(3070, 12)); // 50 48
        VG99_default.replace(1579, 25, data.mid(3097, 25)); // 50 54
        VG99_default.replace(1604, 8, data.mid(3137, 8));   // 50 6D
        VG99_default.replace(1612, 9, data.mid(3160, 9));   // 50 75
        VG99_default.replace(1621, 2, data.mid(3184, 2));   // 50 7E
        VG99_default.replace(1636, 4, data.mid(3186, 4));   // (51 00)
        VG99_default.replace(1653, 46, data.mid(3205, 46)); // 54 00
        VG99_default.replace(1699, 37, data.mid(3266, 37)); // 54 2E
        VG99_default.replace(1736, 45, data.mid(3318, 45)); // 54 53
        VG99_default.replace(1794, 18, data.mid(3363, 18)); // 55 00
        VG99_default.replace(1812, 35, data.mid(3396, 35)); // 55 12
        VG99_default.replace(1860, 46, data.mid(3446, 46)); // 58 00
        VG99_default.replace(1906, 37, data.mid(3507, 37)); // 58 2E
        VG99_default.replace(1943, 45, data.mid(3559, 45)); // 58 53
        VG99_default.replace(2001, 18, data.mid(3604, 18)); // 59 00
        VG99_default.replace(2019, 35, data.mid(3637, 35)); // 59 12
        VG99_default.replace(2067, 17, data.mid(3687, 17)); // 5C 00
        VG99_default.replace(2097, 13, data.mid(3719, 13)); // 60 00
        VG99_default.replace(2110, 30, data.mid(3747, 30)); // 60 0D
        VG99_default.replace(2140, 8, data.mid(3792, 8));   // 60 2B
        VG99_default.replace(2148, 12, data.mid(3815, 12)); // 60 33
        VG99_default.replace(2160, 9, data.mid(3842, 9));   // 60 3F
        VG99_default.replace(2169, 12, data.mid(3866, 12)); // 60 48
        VG99_default.replace(2181, 25, data.mid(3893, 25)); // 60 54
        VG99_default.replace(2206, 8, data.mid(3933, 8));   // 60 6D
        VG99_default.replace(2214, 9, data.mid(3956, 9));   // 60 75
        VG99_default.replace(2223, 2, data.mid(3980, 2));   // 60 7E
        VG99_default.replace(2238, 4, data.mid(3982, 4));   // 61 00
        VG99_default.replace(2255, 46, data.mid(4001, 46)); // 64 00
        VG99_default.replace(2301, 37, data.mid(4062, 37)); // 64 2E
        VG99_default.replace(2338, 45, data.mid(4114, 45)); // 64 53
        VG99_default.replace(2396, 18, data.mid(4159, 18)); // 65 00
        VG99_default.replace(2414, 35, data.mid(4192, 35)); // 65 12
        VG99_default.replace(2462, 46, data.mid(4242, 46)); // 68 00
        VG99_default.replace(2508, 37, data.mid(4303, 37)); // 68 2E
        VG99_default.replace(2545, 45, data.mid(4355, 45)); // 68 53
        VG99_default.replace(2603, 18, data.mid(4400, 18)); // 69 00
        VG99_default.replace(2621, 35, data.mid(4433, 35)); // 69 12
        VG99_default.replace(2669, 17, data.mid(4483, 17)); // 6C 00
        VG99_default.replace(2699, 10, data.mid(4515, 10)); // 70 00
        VG99_default.replace(2709, 4, data.mid(4540, 4));   // 70 0A
        VG99_default.replace(2713, 10, data.mid(4559, 10)); // 70 0E
        VG99_default.replace(2723, 4, data.mid(4584, 4));   // 70 18
        VG99_default.replace(2727, 10, data.mid(4603, 10)); // 70 1C
        VG99_default.replace(2737, 4, data.mid(4628, 4));   // 70 26
        VG99_default.replace(2741, 10, data.mid(4647, 10)); // 70 2A
        VG99_default.replace(2751, 4, data.mid(4672, 4));   // 70 34
        VG99_default.replace(2755, 10, data.mid(4691, 10)); // 70 38
        VG99_default.replace(2765, 4, data.mid(4716, 4));   // 70 42
        VG99_default.replace(2769, 10, data.mid(4735, 10)); // 70 46
        VG99_default.replace(2779, 4, data.mid(4760, 4));   // 70 50
        VG99_default.replace(2783, 10, data.mid(4779, 10)); // 70 54
        VG99_default.replace(2793, 4, data.mid(4804, 4));   // 70 5E
        VG99_default.replace(2797, 10, data.mid(4823, 10)); // 70 62
        VG99_default.replace(2807, 4, data.mid(4848, 4));   // 70 6C
        VG99_default.replace(2824, 10, data.mid(4867, 10)); // 71 00
        VG99_default.replace(2834, 4, data.mid(4892, 4));   // 71 0A
        VG99_default.replace(2838, 10, data.mid(4911, 10)); // 71 0E
        VG99_default.replace(2848, 4, data.mid(4936, 4));   // 71 18
        VG99_default.replace(2852, 10, data.mid(4955, 10)); // 71 1C
        VG99_default.replace(2862, 4, data.mid(4980, 4));   // 71 26
        VG99_default.replace(2866, 10, data.mid(4999, 10)); // 71 2A
        VG99_default.replace(2876, 4, data.mid(5024, 4));   // 71 34
        VG99_default.replace(2880, 10, data.mid(5043, 10)); // 71 38
        VG99_default.replace(2890, 4, data.mid(5068, 4));   // 71 42
        VG99_default.replace(2894, 10, data.mid(5087, 10)); // 71 46
        VG99_default.replace(2904, 4, data.mid(5112, 4));   // 71 50
        VG99_default.replace(2908, 10, data.mid(5131, 10)); // 71 54
        VG99_default.replace(2918, 4, data.mid(5156, 4));   // 71 5E
        VG99_default.replace(2922, 10, data.mid(5175, 10)); // 71 62
        VG99_default.replace(2932, 4, data.mid(5200, 4));   // 71 6C
        VG99_default.replace(2949, 10, data.mid(5219, 10)); // 72 00
        VG99_default.replace(2959, 4, data.mid(5244, 4));   // 72 0A
        VG99_default.replace(2963, 10, data.mid(5263, 10)); // 72 0E
        VG99_default.replace(2973, 4, data.mid(5288, 4));   // 72 18
        VG99_default.replace(2977, 10, data.mid(5307, 10)); // 72 1C
        VG99_default.replace(2987, 4, data.mid(5332, 4));   // 72 26
        VG99_default.replace(2991, 10, data.mid(5351, 10)); // 72 2A
        VG99_default.replace(3001, 4, data.mid(5376, 4));   // 72 34
        VG99_default.replace(3005, 10, data.mid(5395, 10)); // 72 38
        VG99_default.replace(3015, 4, data.mid(5420, 4));   // 72 42
        VG99_default.replace(3019, 10, data.mid(5439, 10)); // 72 46
        VG99_default.replace(3029, 4, data.mid(5464, 4));   // 72 50
        VG99_default.replace(3033, 10, data.mid(5483, 10)); // 72 54
        VG99_default.replace(3043, 4, data.mid(5508, 4));   // 72 5E
        VG99_default.replace(3047, 10, data.mid(5527, 10)); // 72 62
        VG99_default.replace(3057, 4, data.mid(5552, 4));   // 72 6C
        VG99_default.replace(3074, 10, data.mid(5571, 10)); // 73 00
        VG99_default.replace(3084, 4, data.mid(5596, 4));   // 73 0A
        VG99_default.replace(3088, 10, data.mid(5615, 10)); // 73 0E
        VG99_default.replace(3098, 4, data.mid(5640, 4));   // 73 18
        VG99_default.replace(3102, 10, data.mid(5659, 10)); // 73 1C
        VG99_default.replace(3112, 4, data.mid(5684, 4));   // 73 26
        VG99_default.replace(3116, 10, data.mid(5703, 10)); // 73 2A
        VG99_default.replace(3126, 4, data.mid(5728, 4));   // 73 34
        VG99_default.replace(3130, 10, data.mid(5747, 10)); // 73 38
        VG99_default.replace(3140, 4, data.mid(5772, 4));   // 73 42
        VG99_default.replace(3144, 10, data.mid(5791, 10)); // 73 46
        VG99_default.replace(3154, 4, data.mid(5816, 4));   // 73 50
        VG99_default.replace(3158, 10, data.mid(5835, 10)); // 73 54
        VG99_default.replace(3168, 4, data.mid(5860, 4));   // 73 5E
        VG99_default.replace(3172, 10, data.mid(5879, 10)); // 73 62
        VG99_default.replace(3182, 4, data.mid(5904, 4));   // 73 6C
        VG99_default.replace(3186, 1, data.mid(5923, 1));   // 73 70
        if(data.size()>5930)
        {
            VG99_default.replace(3200, 128, data.mid(5940, 128)); // 7C 00
            VG99_default.replace(3341, 51, data.mid(6068, 51));   // 7D 00
        };
    }
    else // type == "b"
    {
        double size = data.mid(18, 4).toHex().toUpper().toInt(&ok, 16);
        size = size - 33;
        int patchCount = (size/3520);

        if (patchCount>1)
        {
            QString msgText;
            QString patchText;
            QString patchNumber;
            this->patchList.clear();
            a = 0; // locate patch text start position from the start of the file
            for (int h=0; h<patchCount; h++)
            {
                QString hexData = data.mid(42+(h*3478), 16).toHex().toUpper();
                for(int x=0; x<16; x++) patchText.append(char(hexData.mid(x*2, 2).toInt(&ok, 16)));
                patchNumber = QString::number(h+1, 10).toUpper();
                msgText.append(patchNumber + " : ");
                msgText.append(patchText + "   ");
                this->patchList.append(msgText);
                patchText.clear();
                msgText.clear();
                a=a+3478;                      // offset is set in front of marker
            };

            fileDialog *dialog = new fileDialog(fileName, patchList);
            dialog->exec();
            dialog->deleteLater();
            patchIndex(this->index);

        };

        int offset = 0;
        if (patchCount>1)
        {
            offset = ((this->index-1)*3478);    // find start of required patch
        };

        VG99_default.replace(11, 84, data.mid(offset+42, 84));       // 00 00
        VG99_default.replace(108, 128, data.mid(offset+142, 128));   // 01 00
        VG99_default.replace(249, 12, data.mid(offset+285, 12));     // 02 00
        VG99_default.replace(274, 80, data.mid(offset+312, 80));     // 03 00
        VG99_default.replace(367, 80, data.mid(offset+407, 80));     // 05 00
        VG99_default.replace(460, 128, data.mid(offset+503, 128));   // 06 00
        VG99_default.replace(601, 32, data.mid(offset+646, 32));     // 07 00
        VG99_default.replace(646, 66, data.mid(offset+693, 66));     // 0D 00
        VG99_default.replace(725, 128, data.mid(offset+775, 128));   // 10 00
        VG99_default.replace(866, 17, data.mid(offset+918, 17));     // 11 00
        VG99_default.replace(896, 57, data.mid(offset+950, 57));     // 20 00
        VG99_default.replace(966, 116, data.mid(offset+1023, 116));  // 30 00
        VG99_default.replace(1095, 90, data.mid(offset+1154, 90));   // 34 00
        VG99_default.replace(1198, 116, data.mid(offset+1260, 116)); // 38 00
        VG99_default.replace(1327, 90, data.mid(offset+1391, 90));   // 3C 00
        VG99_default.replace(1430, 52, data.mid(offset+1496, 52));   // 40 00
        VG99_default.replace(1495, 128, data.mid(offset+1564, 128)); // 50 00
        VG99_default.replace(1636, 4, data.mid(offset+1707, 4));     // 51 00
        VG99_default.replace(1653, 128, data.mid(offset+1727, 128)); // 54 00
        VG99_default.replace(1794, 53, data.mid(offset+1870, 53));   // 55 00
        VG99_default.replace(1860, 128, data.mid(offset+1939, 128)); // 58 00
        VG99_default.replace(2001, 53, data.mid(offset+2082, 53));   // 59 00
        VG99_default.replace(2067, 17, data.mid(offset+2150, 17));   // 5C 00
        VG99_default.replace(2097, 128, data.mid(offset+2183, 128)); // 60 00
        VG99_default.replace(2238, 4, data.mid(offset+2326, 4));     // 61 00
        VG99_default.replace(2255, 128, data.mid(offset+2346, 128)); // 64 00
        VG99_default.replace(2396, 53, data.mid(offset+2489, 53));   // 65 00
        VG99_default.replace(2462, 128, data.mid(offset+2558, 128)); // 68 00
        VG99_default.replace(2603, 53, data.mid(offset+2701, 53));   // 69 00
        VG99_default.replace(2669, 17, data.mid(offset+2769, 17));   // 6C 00
        VG99_default.replace(2699, 112, data.mid(offset+2801, 112)); // 70 00
        VG99_default.replace(2824, 112, data.mid(offset+2928, 112)); // 71 00
        VG99_default.replace(2949, 112, data.mid(offset+3055, 112)); // 72 00
        VG99_default.replace(3074, 113, data.mid(offset+3182, 113)); // 73 00
        if(data.mid(offset+3309,2).toHex().toUpper().contains("7C00"))
        {
            VG99_default.replace(3200, 128, data.mid(offset+3311, 128)); // 7C 00
            VG99_default.replace(3341, 51, data.mid(offset+3454, 51));   // 7D 00
        };

    };

    data = VG99_default;
}

void tsl_VG99_VB99_translation::convert_VG99g9l_to_VG99default()
{
    bool ok;
    QString hex = data.mid(162, 2).toHex().toUpper();
    int patchCount = (hex.toInt(&ok, 16)/16)-1;

    if (patchCount>1)
    {
        QString msgText;
        QString patchText;
        QString patchNumber;
        this->patchList.clear();
        a = 0; // locate patch text start position from the start of the file
        for (int h=0; h<patchCount; h++)
        {
            QString hexData = data.mid(172+(h*3222), 16).toHex().toUpper();
            for(int x=0; x<16; x++) patchText.append(char(hexData.mid(x*2, 2).toInt(&ok, 16)));
            patchNumber = QString::number(h+1, 10).toUpper();
            msgText.append(patchNumber + " : ");
            msgText.append(patchText + "   ");
            this->patchList.append(msgText);
            patchText.clear();
            msgText.clear();
            a=a+3222;                      // offset is set in front of marker
        };

        fileDialog *dialog = new fileDialog(fileName, patchList);
        dialog->exec();
        dialog->deleteLater();
        patchIndex(this->index);
    };

    int offset = 0;
    if (patchCount>1)
    {
        offset = ((this->index-1)*3222);    // find start of required patch
    };

    VG99_default.replace(11, 84, data.mid(offset+172, 84));      // 00 00
    VG99_default.replace(108, 128, data.mid(offset+264, 128));   // 01 00
    VG99_default.replace(249, 12, data.mid(offset+400, 12));     // 02 00
    VG99_default.replace(274, 80, data.mid(offset+420, 80));     // 03 00
    VG99_default.replace(367, 80, data.mid(offset+508, 80));     // 05 00
    VG99_default.replace(460, 128, data.mid(offset+596, 128));   // 06 00
    VG99_default.replace(601, 32, data.mid(offset+732, 32));     // 07 00
    VG99_default.replace(646, 66, data.mid(offset+772, 66));     // 0D 00
    VG99_default.replace(725, 128, data.mid(offset+846, 128));   // 10 00
    VG99_default.replace(866, 17, data.mid(offset+982, 17));     // 11 00
    VG99_default.replace(896, 57, data.mid(offset+1007, 57));    // 20 00
    VG99_default.replace(966, 116, data.mid(offset+1072, 116));  // 30 00
    VG99_default.replace(1095, 90, data.mid(offset+1196, 90));   // 34 00
    VG99_default.replace(1198, 116, data.mid(offset+1294, 116)); // 38 00
    VG99_default.replace(1327, 90, data.mid(offset+1418, 90));   // 3C 00
    VG99_default.replace(1430, 52, data.mid(offset+1516, 52));   // 40 00
    VG99_default.replace(1495, 128, data.mid(offset+1576, 128)); // 50 00
    VG99_default.replace(1636, 4, data.mid(offset+1712, 4));     // 51 00
    VG99_default.replace(1653, 128, data.mid(offset+1724, 128)); // 54 00
    VG99_default.replace(1794, 53, data.mid(offset+1860, 53));   // 55 00
    VG99_default.replace(1860, 128, data.mid(offset+1921, 128)); // 58 00
    VG99_default.replace(2001, 53, data.mid(offset+2057, 53));   // 59 00
    VG99_default.replace(2067, 17, data.mid(offset+2118, 17));   // 5C 00
    VG99_default.replace(2097, 128, data.mid(offset+2143, 128)); // 60 00
    VG99_default.replace(2238, 4, data.mid(offset+2279, 4));     // 61 00
    VG99_default.replace(2255, 128, data.mid(offset+2291, 128)); // 64 00
    VG99_default.replace(2396, 53, data.mid(offset+2427, 53));   // 65 00
    VG99_default.replace(2462, 128, data.mid(offset+2488, 128)); // 68 00
    VG99_default.replace(2603, 53, data.mid(offset+2624, 53));   // 69 00
    VG99_default.replace(2669, 17, data.mid(offset+2685, 17));   // 6C 00
    VG99_default.replace(2699, 112, data.mid(offset+2710, 112)); // 70 00
    VG99_default.replace(2824, 112, data.mid(offset+2830, 112)); // 71 00
    VG99_default.replace(2949, 112, data.mid(offset+2950, 112)); // 72 00
    VG99_default.replace(3074, 113, data.mid(offset+3070, 113)); // 73 00
    VG99_default.replace(3200, 128, data.mid(offset+3191, 128)); // 7C 00
    VG99_default.replace(3341, 51, data.mid(offset+3327, 51));   // 7D 00

    data = VG99_default;

}

QByteArray tsl_VG99_VB99_translation::convert_altVB99mid_to_default(int)
{
    QByteArray VG99_default;
    return VG99_default;
}

QByteArray tsl_VG99_VB99_translation::convert_VB99mid_to_VG99default(int)
{
    SysxIO *sysxIO = SysxIO::Instance();
    sysxIO->deBug(" VB convert input data.size()=" + QString::number(VG99_default.size(), 10) + " this->data.size()=" + QString::number(data.size(), 10));
    QByteArray temp;
    VG99_default_replace("00", "00", 16, copyVBmid("00", "00", 16));                        //patch common
    VG99_default_replace("00", "10", 22, copyVBmid("00", "10", 22));                        //control assigns
    VG99_default_replace("00", "26", 45, copyVBmid("00", "26", 45));
    VG99_default_replace("00", "18", 2, convert_VB99_Assign(copyVBmid("00", "18", 2)));
    VG99_default_replace("00", "1A", 2, convert_VB99_Assign(copyVBmid("00", "1A", 2)));
    VG99_default_replace("00", "1C", 2, convert_VB99_Assign(copyVBmid("00", "1C", 2)));
    VG99_default_replace("00", "1E", 2, convert_VB99_Assign(copyVBmid("00", "1E", 2)));
    VG99_default_replace("00", "20", 2, convert_VB99_Assign(copyVBmid("00", "20", 2)));
    VG99_default_replace("00", "22", 2, convert_VB99_Assign(copyVBmid("00", "22", 2)));
    VG99_default_replace("01", "00", 10, copyVBmid("01", "00", 10));
    VG99_default_replace("01", "0A", 10, copyVBmid("01", "0A", 10));
    VG99_default_replace("01", "14", 10, copyVBmid("01", "14", 10));
    VG99_default_replace("01", "1E", 10, copyVBmid("01", "1E", 10));
    VG99_default_replace("01", "28", 10, copyVBmid("01", "28", 10));
    VG99_default_replace("01", "32", 10, copyVBmid("01", "32", 10));
    VG99_default_replace("01", "3C", 10, copyVBmid("01", "3C", 10));
    VG99_default_replace("01", "46", 10, copyVBmid("01", "46", 10));
    VG99_default_replace("01", "50", 10, copyVBmid("01", "50", 10));
    VG99_default_replace("01", "5A", 10, copyVBmid("01", "5A", 10));
    VG99_default_replace("01", "64", 10, copyVBmid("01", "64", 10));
    VG99_default_replace("01", "6E", 10, copyVBmid("01", "6E", 10));
    VG99_default_replace("01", "78", 10, copyVBmid("01", "78", 10));
    VG99_default_replace("01", "01", 2, convert_VB99_Assign(copyVBmid("01", "01", 2)));
    VG99_default_replace("01", "0B", 2, convert_VB99_Assign(copyVBmid("01", "0B", 2)));
    VG99_default_replace("01", "15", 2, convert_VB99_Assign(copyVBmid("01", "15", 2)));
    VG99_default_replace("01", "1F", 2, convert_VB99_Assign(copyVBmid("01", "1F", 2)));
    VG99_default_replace("01", "29", 2, convert_VB99_Assign(copyVBmid("01", "33", 2)));
    VG99_default_replace("01", "33", 2, convert_VB99_Assign(copyVBmid("01", "00", 2)));
    VG99_default_replace("01", "3D", 2, convert_VB99_Assign(copyVBmid("01", "3D", 2)));
    VG99_default_replace("01", "47", 2, convert_VB99_Assign(copyVBmid("01", "47", 2)));
    VG99_default_replace("01", "51", 2, convert_VB99_Assign(copyVBmid("01", "51", 2)));
    VG99_default_replace("01", "5B", 2, convert_VB99_Assign(copyVBmid("01", "5B", 2)));
    VG99_default_replace("01", "65", 2, convert_VB99_Assign(copyVBmid("01", "65", 2)));
    VG99_default_replace("01", "6F", 2, convert_VB99_Assign(copyVBmid("01", "6F", 2)));
    VG99_default_replace("01", "79", 2, convert_VB99_Assign(copyVBmid("01", "79", 2)));
    //VG99_default_replace("02", "02", 10, copyVBmid("02", "02", 10));
    VG99_default_replace("02", "03", 2, convert_VB99_Assign(copyVBmid("02", "03", 2)));
    VG99_default_replace("03", "00", 10, copyVBmid("03", "00", 10));
    VG99_default_replace("03", "0A", 10, copyVBmid("03", "0A", 10));
    VG99_default_replace("03", "14", 10, copyVBmid("03", "14", 10));
    VG99_default_replace("03", "1E", 10, copyVBmid("03", "1E", 10));
    VG99_default_replace("03", "28", 10, copyVBmid("03", "28", 10));
    VG99_default_replace("03", "32", 10, copyVBmid("03", "32", 10));
    VG99_default_replace("03", "3C", 10, copyVBmid("03", "3C", 10));
    VG99_default_replace("03", "46", 10, copyVBmid("03", "46", 10));
    VG99_default_replace("03", "01", 2, convert_VB99_Assign(copyVBmid("03", "01", 2)));
    VG99_default_replace("03", "0B", 2, convert_VB99_Assign(copyVBmid("03", "0B", 2)));
    VG99_default_replace("03", "15", 2, convert_VB99_Assign(copyVBmid("03", "15", 2)));
    VG99_default_replace("03", "1F", 2, convert_VB99_Assign(copyVBmid("03", "1F", 2)));
    VG99_default_replace("03", "29", 2, convert_VB99_Assign(copyVBmid("03", "29", 2)));
    VG99_default_replace("03", "33", 2, convert_VB99_Assign(copyVBmid("03", "33", 2)));
    VG99_default_replace("03", "3D", 2, convert_VB99_Assign(copyVBmid("03", "3D", 2)));
    VG99_default_replace("03", "47", 2, convert_VB99_Assign(copyVBmid("03", "47", 2)));
    VG99_default_replace("05", "00", 10, copyVBmid("05", "00", 10));
    VG99_default_replace("05", "0A", 10, copyVBmid("05", "0A", 10));
    VG99_default_replace("05", "14", 10, copyVBmid("05", "14", 10));
    VG99_default_replace("05", "1E", 10, copyVBmid("05", "1E", 10));
    VG99_default_replace("05", "28", 10, copyVBmid("05", "28", 10));
    VG99_default_replace("05", "32", 10, copyVBmid("05", "32", 10));
    VG99_default_replace("05", "3C", 10, copyVBmid("05", "3C", 10));
    //VG99_default_replace("05", "46", 10, copyVBmid("05", "46", 10));
    VG99_default_replace("05", "01", 2, convert_VB99_Assign(copyVBmid("05", "00", 2)));
    VG99_default_replace("05", "0B", 2, convert_VB99_Assign(copyVBmid("05", "0B", 2)));
    VG99_default_replace("05", "15", 2, convert_VB99_Assign(copyVBmid("05", "15", 2)));
    VG99_default_replace("05", "1F", 2, convert_VB99_Assign(copyVBmid("05", "1F", 2)));
    VG99_default_replace("05", "29", 2, convert_VB99_Assign(copyVBmid("05", "29", 2)));
    VG99_default_replace("05", "33", 2, convert_VB99_Assign(copyVBmid("05", "33", 2)));
    VG99_default_replace("05", "3D", 2, convert_VB99_Assign(copyVBmid("05", "3D", 2)));
    VG99_default_replace("05", "47", 2, convert_VB99_Assign(copyVBmid("05", "47", 2)));
    VG99_default_replace("06", "00", 10, copyVBmid("06", "00", 10));
    VG99_default_replace("06", "0A", 10, copyVBmid("06", "0A", 10));
    VG99_default_replace("06", "14", 10, copyVBmid("06", "14", 10));
    VG99_default_replace("06", "1E", 10, copyVBmid("06", "1E", 10));
    VG99_default_replace("06", "28", 10, copyVBmid("06", "28", 10));
    VG99_default_replace("06", "32", 10, copyVBmid("06", "32", 10));
    VG99_default_replace("06", "3C", 10, copyVBmid("06", "3C", 10));
    VG99_default_replace("06", "46", 10, copyVBmid("06", "46", 10));
    VG99_default_replace("06", "50", 10, copyVBmid("06", "50", 10));
    VG99_default_replace("06", "5A", 10, copyVBmid("06", "5A", 10));
    VG99_default_replace("06", "64", 10, copyVBmid("06", "64", 10));
    VG99_default_replace("06", "6E", 10, copyVBmid("06", "6E", 10));
    VG99_default_replace("06", "78", 10, copyVBmid("06", "78", 10));
    VG99_default_replace("06", "01", 2, convert_VB99_Assign(copyVBmid("06", "01", 2)));
    VG99_default_replace("06", "0B", 2, convert_VB99_Assign(copyVBmid("06", "0B", 2)));
    VG99_default_replace("06", "15", 2, convert_VB99_Assign(copyVBmid("06", "15", 2)));
    VG99_default_replace("06", "1F", 2, convert_VB99_Assign(copyVBmid("06", "1F", 2)));
    VG99_default_replace("06", "29", 2, convert_VB99_Assign(copyVBmid("06", "29", 2)));
    VG99_default_replace("06", "33", 2, convert_VB99_Assign(copyVBmid("06", "33", 2)));
    VG99_default_replace("06", "3D", 2, convert_VB99_Assign(copyVBmid("06", "3D", 2)));
    VG99_default_replace("06", "47", 2, convert_VB99_Assign(copyVBmid("06", "47", 2)));
    VG99_default_replace("06", "51", 2, convert_VB99_Assign(copyVBmid("06", "51", 2)));
    VG99_default_replace("06", "5B", 2, convert_VB99_Assign(copyVBmid("06", "5B", 2)));
    VG99_default_replace("06", "65", 2, convert_VB99_Assign(copyVBmid("06", "65", 2)));
    VG99_default_replace("06", "6F", 2, convert_VB99_Assign(copyVBmid("06", "6F", 2)));
    VG99_default_replace("06", "79", 2, convert_VB99_Assign(copyVBmid("06", "79", 2)));
    VG99_default_replace("07", "00", 10, copyVBmid("07", "00", 10));
    VG99_default_replace("07", "02", 10, copyVBmid("07", "02", 10));
    VG99_default_replace("07", "0C", 10, copyVBmid("07", "0C", 10));
    //VG99_default_replace("07", "16", 10, copyVBmid("07", "16", 10));
    VG99_default_replace("07", "03", 2, convert_VB99_Assign(copyVBmid("07", "03", 2)));
    VG99_default_replace("07", "0D", 2, convert_VB99_Assign(copyVBmid("07", "0D", 2)));
    VG99_default_replace("07", "17", 2, convert_VB99_Assign(copyVBmid("07", "17", 2)));
    VG99_default_replace("0D", "00", 10, copyVBmid("0D", "00", 10));                             //vlink/dbeam
    VG99_default_replace("0D", "0A", 10, copyVBmid("0D", "0A", 10));
    VG99_default_replace("0D", "14", 10, copyVBmid("0D", "14", 10));
    VG99_default_replace("0D", "1E", 2, copyVBmid("0D", "1E", 2));
    VG99_default_replace("0D", "20", 2, copyVBmid("0D", "20", 2));
    VG99_default_replace("0D", "22", 4, copyVBmid("0D", "22", 4));
    VG99_default_replace("0D", "26", 1, copyVBmid("0D", "26", 1));
    VG99_default_replace("0D", "27", 4, copyVBmid("0D", "27", 4));
    VG99_default_replace("0D", "2B", 4, copyVBmid("0D", "2B", 4));
    VG99_default_replace("0D", "2F", 6, copyVBmid("0D", "2F", 6));
    VG99_default_replace("0D", "35", 1, copyVBmid("0D", "35", 1));
    VG99_default_replace("0D", "36", 2, copyVBmid("0D", "36", 2));
    VG99_default_replace("0D", "38", 4, copyVBmid("0D", "38", 4));
    VG99_default_replace("0D", "3C", 6, copyVBmid("0D", "3C", 6));
    VG99_default_replace("20", "00", 7, copyVBmid("20", "00", 7));                                //mixer
    VG99_default_replace("20", "07", 13, copyVBmid("20", "07", 13));                              //mixer
    VG99_default_replace("20", "14", 6, copyVBmid("20", "14", 6));                                //mixer
    VG99_default_replace("20", "1A", 6, copyVBmid("20", "1A", 6));                                //mixer
    VG99_default_replace("20", "20", 8, copyVBmid("20", "20", 8));                                //mixer
    VG99_default_replace("20", "28", 7, copyVBmid("20", "28", 7));                                //mixer
    VG99_default_replace("20", "2F", 10, copyVBmid("20", "2F", 10));                              //mixer

    VG99_default_replace("30", "00", 1, convertVB99_value(copyVBmid("30", "00", 1), "00", "0C")); //COSM A sw
    VG99_default_replace("30", "01", 26, copyVBmid("30", "01", 26));                              //COSM A
    VG99_default_replace("30", "1A", 1, copyVBmid("30", "1B", 1));                                //COSM A
    VG99_default_replace("30", "1B", 1, copyVBmid("30", "1C", 1));                                //COSM A type
    VG99_default_replace("30", "69", 1, convertVB99_value(copyVBmid("30", "1C", 1), "00", "0D")); //COSM A bass type
    VG99_default_replace("30", "6A", 3, copyVBmid("30", "1D", 3));                                //COSM A bass tone
    VG99_default_replace("30", "6D", 1, copyVBmid("30", "21", 1));                                //COSM A bass tone
    VG99_default_replace("30", "52", 1, copyVBmid("30", "2E", 1));                                //COSM A vari tone
    VG99_default_replace("30", "36", 1, copyVBmid("30", "33", 1));                                //COSM A r pu type
    VG99_default_replace("30", "37", 1, scaleVG99_value(copyVG99("30", "34", 1), "59", "00", "3F")); //pu position "59"=original max range, "00"/"3F"=min/max range output
    VG99_default_replace("30", "3A", 1, copyVBmid("30", "35", 1));                                //COSM A r pu phase
    VG99_default_replace("30", "3B", 1, copyVBmid("30", "42", 1));                                //COSM A f pu type
    VG99_default_replace("30", "3C", 1, scaleVG99_value(copyVG99("30", "43", 1), "59", "00", "3F")); //pu position
    VG99_default_replace("30", "49", 1, copyVBmid("30", "51", 1));                                //COSM A acou level
    VG99_default_replace("30", "50", 1, copyVBmid("30", "51", 1));                                //COSM A acou level
    VG99_default_replace("30", "57", 1, copyVBmid("30", "51", 1));                                //COSM A acou level
    VG99_default_replace("30", "60", 1, copyVBmid("30", "51", 1));                                //COSM A acou level
    VG99_default_replace("30", "64", 1, copyVBmid("30", "51", 1));                                //COSM A acou level
    VG99_default_replace("30", "68", 1, copyVBmid("30", "51", 1));                                //COSM A acou level
    VG99_default_replace("30", "47", 1, copyVBmid("30", "52", 1));                                //COSM A acou body
    VG99_default_replace("30", "54", 1, copyVBmid("30", "52", 1));                                //COSM A acou body
    VG99_default_replace("30", "5A", 1, copyVBmid("30", "52", 1));                                //COSM A acou body
    VG99_default_replace("30", "4B", 1, copyVBmid("30", "53", 1));                                //COSM A acou size
    VG99_default_replace("30", "4C", 1, copyVBmid("30", "54", 1));                                //COSM A acou reso
    VG99_default_replace("30", "62", 1, copyVBmid("30", "54", 1));                                //COSM A acou reso
    VG99_default_replace("30", "66", 1, copyVBmid("30", "54", 1));                                //COSM A acou reso
    VG99_default_replace("30", "4D", 1, copyVBmid("30", "55", 1));                                //COSM A acou attack
    VG99_default_replace("30", "55", 1, copyVBmid("30", "55", 1));                                //COSM A acou attack
    VG99_default_replace("30", "5E", 1, copyVBmid("30", "55", 1));                                //COSM A acou attack
    VG99_default_replace("30", "61", 1, copyVBmid("30", "55", 1));                                //COSM A acou attack
    VG99_default_replace("30", "5D", 1, copyVBmid("30", "57", 1));                                //COSM A acou buzz
    VG99_default_replace("30", "65", 1, copyVBmid("30", "5D", 1));                                //COSM A acou decay
    VG99_default_replace("30", "1B", 1, convertVB99_value(copyVBmid("30", "5E", 1), "00", "0E")); //COSM A e guitar type
    VG99_default_replace("30", "1C", 2, copyVBmid("30", "5E", 2));                                //COSM A pu select
    VG99_default_replace("30", "1F", 1, copyVBmid("30", "61", 1));                                //COSM A volume
    VG99_default_replace("30", "22", 1, copyVBmid("30", "62", 1));                                //COSM A tone
    VG99_default_replace("10", "01", 1, copyVBmid("30", "63", 1));                                //COSM A pitch sw
    VG99_default_replace("10", "05", 1, copyVBmid("30", "64", 1));                                //COSM A harmony sw
    VG99_default_replace("10", "1D", 12, copyVBmid("30", "65", 12));                              //COSM A pitch/fine
    VG99_default_replace("10", "11", 12, copyVBmid("30", "65", 12));                              //COSM A pitch/fine
    VG99_default_replace("10", "0E", 1, copyVBmid("30", "71", 1));                                //COSM A harmony type
    VG99_default_replace("10", "35", 12, copyVBmid("30", "72", 12));                              //COSM A harmony user
    VG99_default_replace("10", "41", 1, copyVBmid("30", "7E", 1));                                //COSM A bend
    VG99_default_replace("10", "42", 1, copyVBmid("30", "7E", 1));                                //COSM A bend
    VG99_default_replace("10", "43", 1, copyVBmid("30", "7E", 1));                                //COSM A bend
    VG99_default_replace("10", "44", 1, copyVBmid("30", "7E", 1));                                //COSM A bend
    VG99_default_replace("10", "45", 1, copyVBmid("30", "7E", 1));                                //COSM A bend
    VG99_default_replace("10", "46", 1, copyVBmid("30", "7E", 1));                                //COSM A bend

    VG99_default_replace("38", "00", 1, convertVB99_value(copyVBmid("38", "00", 1), "00", "0C")); //COSM B sw
    VG99_default_replace("38", "01", 26, copyVBmid("38", "01", 26));                              //COSM B
    VG99_default_replace("38", "1A", 1, copyVBmid("38", "1B", 1));                                //COSM B
    VG99_default_replace("38", "1B", 1, copyVBmid("38", "1C", 1));                                //COSM B type
    VG99_default_replace("38", "69", 1, convertVB99_value(copyVBmid("38", "1C", 1), "00", "0D")); //COSM B bass type
    VG99_default_replace("38", "6A", 3, copyVBmid("38", "1D", 3));                                //COSM B bass tone
    VG99_default_replace("38", "6D", 1, copyVBmid("38", "21", 1));                                //COSM B bass tone
    VG99_default_replace("38", "52", 1, copyVBmid("38", "2E", 1));                                //COSM B vari tone
    VG99_default_replace("38", "36", 1, copyVBmid("38", "33", 1));                                //COSM B r pu type
    VG99_default_replace("38", "37", 1, scaleVG99_value(copyVG99("38", "34", 1), "59", "00", "3F")); //pu position "59"=original max range, "00"/"3F"=min/max range output
    VG99_default_replace("38", "3A", 1, copyVBmid("38", "35", 1));                                //COSM B r pu phase
    VG99_default_replace("38", "3B", 1, copyVBmid("38", "42", 1));                                //COSM B f pu type
    VG99_default_replace("38", "3C", 1, scaleVG99_value(copyVG99("38", "43", 1), "59", "00", "3F")); //pu position
    VG99_default_replace("38", "49", 1, copyVBmid("38", "51", 1));                                //COSM B acou level
    VG99_default_replace("38", "50", 1, copyVBmid("38", "51", 1));                                //COSM B acou level
    VG99_default_replace("38", "57", 1, copyVBmid("38", "51", 1));                                //COSM B acou level
    VG99_default_replace("38", "60", 1, copyVBmid("38", "51", 1));                                //COSM B acou level
    VG99_default_replace("38", "64", 1, copyVBmid("38", "51", 1));                                //COSM B acou level
    VG99_default_replace("38", "68", 1, copyVBmid("38", "51", 1));                                //COSM B acou level
    VG99_default_replace("38", "47", 1, copyVBmid("38", "52", 1));                                //COSM B acou body
    VG99_default_replace("38", "54", 1, copyVBmid("38", "52", 1));                                //COSM B acou body
    VG99_default_replace("38", "5A", 1, copyVBmid("38", "52", 1));                                //COSM B acou body
    VG99_default_replace("38", "4B", 1, copyVBmid("38", "53", 1));                                //COSM B acou size
    VG99_default_replace("38", "4C", 1, copyVBmid("38", "54", 1));                                //COSM B acou reso
    VG99_default_replace("38", "62", 1, copyVBmid("38", "54", 1));                                //COSM B acou reso
    VG99_default_replace("38", "66", 1, copyVBmid("38", "54", 1));                                //COSM B acou reso
    VG99_default_replace("38", "4D", 1, copyVBmid("38", "55", 1));                                //COSM B acou attack
    VG99_default_replace("38", "55", 1, copyVBmid("38", "55", 1));                                //COSM B acou attack
    VG99_default_replace("38", "5E", 1, copyVBmid("38", "55", 1));                                //COSM B acou attack
    VG99_default_replace("38", "61", 1, copyVBmid("38", "55", 1));                                //COSM B acou attack
    VG99_default_replace("38", "5D", 1, copyVBmid("38", "57", 1));                                //COSM B acou buzz
    VG99_default_replace("38", "65", 1, copyVBmid("38", "5D", 1));                                //COSM B acou decay
    VG99_default_replace("38", "1B", 1, convertVB99_value(copyVBmid("38", "5E", 1), "00", "0E")); //COSM B e guitar type
    VG99_default_replace("38", "1C", 2, copyVBmid("38", "5E", 2));                                //COSM B pu select
    VG99_default_replace("38", "1F", 1, copyVBmid("38", "61", 1));                                //COSM B volume
    VG99_default_replace("38", "22", 1, copyVBmid("38", "62", 1));                                //COSM B tone
    if(copyVBmid("30", "63", 1).toHex().toUpper()=="00") //if A pitch sw off
    {
        VG99_default_replace("10", "01", 1, copyVBmid("38", "63", 1));                            //COSM B pitch sw
    };
    if(copyVBmid("30", "64", 1).toHex().toUpper()=="00") //if A harmony sw off
    {
        VG99_default_replace("10", "05", 1, copyVBmid("38", "64", 1));                            //COSM B harmony sw
    };
    VG99_default_replace("10", "5D", 12, copyVBmid("38", "65", 12));                              //COSM B pitch/fine
    VG99_default_replace("10", "51", 12, copyVBmid("38", "65", 12));                              //COSM B pitch/fine
    //VG99_default_replace("10", "0E", 1, copyVBmid("38", "71", 1));                                //COSM B harmony type
    VG99_default_replace("10", "75", 11, copyVBmid("38", "72", 11));                              //COSM B harmony user
    VG99_default_replace("11", "01", 1, copyVBmid("38", "7E", 1));                                //COSM B bend
    VG99_default_replace("11", "02", 1, copyVBmid("38", "7E", 1));                                //COSM B bend
    VG99_default_replace("11", "03", 1, copyVBmid("38", "7E", 1));                                //COSM B bend
    VG99_default_replace("11", "04", 1, copyVBmid("38", "7E", 1));                                //COSM B bend
    VG99_default_replace("11", "05", 1, copyVBmid("38", "7E", 1));                                //COSM B bend
    VG99_default_replace("11", "06", 1, copyVBmid("38", "7E", 1));                                //COSM B bend

    VG99_default_replace("40", "01", 1, copyVBmid("40", "01", 1));                                //polyfx sw
    VG99_default_replace("40", "02", 1, convertVB99_value(copyVBmid("40", "02", 1), "00", "1A")); //polyfx type
    VG99_default_replace("40", "03", 1, QByteArray::fromHex("00"));                               //polyfx comp type
    VG99_default_replace("40", "04", 1, copyVBmid("40", "05", 1));                                //polyfx comp sus
    VG99_default_replace("40", "05", 1, copyVBmid("40", "0B", 1));                                //polyfx comp attack
    VG99_default_replace("40", "08", 1, copyVBmid("40", "11", 1));                                //polyfx comp tone
    VG99_default_replace("40", "09", 1, copyVBmid("40", "17", 1));                                //polyfx comp level
    if(copyVBmid("40", "02", 1).toHex().toUpper()=="04") //if poly limiter
    {
        VG99_default_replace("40", "03", 1, QByteArray::fromHex("01"));                           //polyfx comp type
        VG99_default_replace("40", "05", 1, copyVBmid("40", "1D", 1));                            //polyfx limiter attack
        VG99_default_replace("40", "06", 1, copyVBmid("40", "23", 1));                            //polyfx limiter thresh
        VG99_default_replace("40", "07", 1, copyVBmid("40", "2F", 1));                            //polyfx limiter release
        VG99_default_replace("40", "08", 1, copyVBmid("40", "35", 1));                            //polyfx limiter tone
        VG99_default_replace("40", "09", 1, copyVBmid("40", "3B", 1));                            //polyfx limiter level
    };
    VG99_default_replace("40", "0B", 2, copyVBmid("40", "44", 2));                                //polyfx dist type/drive
    VG99_default_replace("40", "10", 1, copyVBmid("40", "47", 1));                                //polyfx dist effect
    VG99_default_replace("40", "0E", 2, copyVBmid("40", "49", 2));                                //polyfx dist balance

    // chain
    VG99_default_replace("50", "00", 1, convertVB99_value(copyVBmid("50", "00", 1), "00", "07")); //Chain A
    VG99_default_replace("50", "01", 1, convertVB99_value(copyVBmid("50", "01", 1), "00", "07")); //Chain A
    VG99_default_replace("50", "02", 1, convertVB99_value(copyVBmid("50", "02", 1), "00", "07")); //Chain A
    VG99_default_replace("50", "03", 1, convertVB99_value(copyVBmid("50", "03", 1), "00", "07")); //Chain A
    VG99_default_replace("50", "04", 1, convertVB99_value(copyVBmid("50", "04", 1), "00", "07")); //Chain A
    VG99_default_replace("50", "05", 1, convertVB99_value(copyVBmid("50", "05", 1), "00", "07")); //Chain A
    VG99_default_replace("50", "06", 1, convertVB99_value(copyVBmid("50", "06", 1), "00", "07")); //Chain A
    VG99_default_replace("50", "07", 1, convertVB99_value(copyVBmid("50", "07", 1), "00", "07")); //Chain A
    VG99_default_replace("50", "08", 1, convertVB99_value(copyVBmid("50", "08", 1), "00", "07")); //Chain A
    VG99_default_replace("50", "09", 1, convertVB99_value(copyVBmid("50", "09", 1), "00", "07")); //Chain A
    VG99_default_replace("50", "0A", 1, convertVB99_value(copyVBmid("50", "0A", 1), "00", "07")); //Chain A
    VG99_default_replace("50", "0B", 1, convertVB99_value(copyVBmid("50", "0B", 1), "00", "07")); //Chain A
    VG99_default_replace("50", "0C", 1, QByteArray::fromHex("0C"));                               //Chain A
    VG99_default_replace("60", "00", 1, convertVB99_value(copyVBmid("60", "00", 1), "00", "07")); //Chain B
    VG99_default_replace("60", "01", 1, convertVB99_value(copyVBmid("60", "01", 1), "00", "07")); //Chain B
    VG99_default_replace("60", "02", 1, convertVB99_value(copyVBmid("60", "02", 1), "00", "07")); //Chain B
    VG99_default_replace("60", "03", 1, convertVB99_value(copyVBmid("60", "03", 1), "00", "07")); //Chain B
    VG99_default_replace("60", "04", 1, convertVB99_value(copyVBmid("60", "04", 1), "00", "07")); //Chain B
    VG99_default_replace("60", "05", 1, convertVB99_value(copyVBmid("60", "05", 1), "00", "07")); //Chain B
    VG99_default_replace("60", "06", 1, convertVB99_value(copyVBmid("60", "06", 1), "00", "07")); //Chain B
    VG99_default_replace("60", "07", 1, convertVB99_value(copyVBmid("60", "07", 1), "00", "07")); //Chain B
    VG99_default_replace("60", "08", 1, convertVB99_value(copyVBmid("60", "08", 1), "00", "07")); //Chain B
    VG99_default_replace("60", "09", 1, convertVB99_value(copyVBmid("60", "09", 1), "00", "07")); //Chain B
    VG99_default_replace("60", "0A", 1, convertVB99_value(copyVBmid("60", "0A", 1), "00", "07")); //Chain B
    VG99_default_replace("60", "0B", 1, convertVB99_value(copyVBmid("60", "0B", 1), "00", "07")); //Chain B
    VG99_default_replace("60", "0C", 1, QByteArray::fromHex("0C"));                               //Chain B

    VG99_default_replace("50", "0D", 18, copyVBmid("50", "0D", 18));                              //preamp A
    VG99_default_replace("50", "0E", 1, convertVB99_value(copyVBmid("50", "0E", 1), "00", "08")); //preamp type
    VG99_default_replace("5C", "00", 17, copyVBmid("50", "20", 17));                              //bass preamp
    VG99_default_replace("50", "33", 7, copyVBmid("50", "33", 7));                                //odds
    VG99_default_replace("50", "34", 1, convertVB99_value(copyVBmid("50", "34", 1), "00", "09")); //odds type
    VG99_default_replace("50", "7E", 2, copyVBmid("50", "40", 2));                                //ns
    VG99_default_replace("51", "00", 4, copyVBmid("50", "42", 4));                                //ns/fv
    VG99_default_replace("50", "48", 12, copyVBmid("50", "48", 12));                              //eq
    VG99_default_replace("50", "54", 25, copyVBmid("50", "54", 25));                              //delay
    VG99_default_replace("50", "6D", 8, copyVBmid("50", "6D", 8));                                //chorus
    VG99_default_replace("50", "2B", 1, copyVBmid("50", "75", 1));                                //comp sw
    VG99_default_replace("50", "2C", 1, convertVB99_value(copyVBmid("50", "76", 1), "00", "0A")); //comp type
    VG99_default_replace("50", "31", 1, copyVBmid("50", "77", 1));                                //comp tone
    VG99_default_replace("50", "2D", 2, copyVBmid("50", "79", 2));                                //comp sus/attk
    VG99_default_replace("50", "32", 1, copyVBmid("50", "7B", 1));                                //comp effect
    VG99_default_replace("50", "30", 1, copyVBmid("50", "7E", 1));                                //comp release
    VG99_default_replace("50", "2F", 1, copyVBmid("50", "7F", 1));                                //comp thresh

    VG99_default_replace("60", "0D", 18, copyVBmid("60", "0D", 18));                              //preamp B
    VG99_default_replace("60", "0E", 1, convertVB99_value(copyVBmid("60", "0E", 1), "00", "08")); //preamp type
    VG99_default_replace("5C", "00", 17, copyVBmid("60", "20", 17));                              //bass preamp
    VG99_default_replace("60", "33", 7, copyVBmid("60", "33", 7));                                //odds
    VG99_default_replace("60", "34", 1, convertVB99_value(copyVBmid("60", "34", 1), "00", "09")); //odds type
    VG99_default_replace("60", "7E", 2, copyVBmid("60", "40", 2));                                //ns
    VG99_default_replace("61", "00", 4, copyVBmid("60", "42", 4));                                //ns/fv
    VG99_default_replace("60", "48", 12, copyVBmid("60", "48", 12));                              //eq
    VG99_default_replace("60", "54", 25, copyVBmid("60", "54", 25));                              //delay
    VG99_default_replace("60", "6D", 8, copyVBmid("60", "6D", 8));                                //chorus
    VG99_default_replace("60", "2B", 1, copyVBmid("60", "75", 1));                                //comp sw
    VG99_default_replace("60", "2C", 1, convertVB99_value(copyVBmid("60", "76", 1), "00", "0A")); //comp type
    VG99_default_replace("60", "31", 1, copyVBmid("60", "77", 1));                                //comp tone
    VG99_default_replace("60", "2D", 2, copyVBmid("60", "79", 2));                                //comp sus/attk
    VG99_default_replace("60", "32", 1, copyVBmid("60", "7B", 1));                                //comp effect
    VG99_default_replace("60", "30", 1, copyVBmid("60", "7E", 1));                                //comp release
    VG99_default_replace("60", "2F", 1, copyVBmid("60", "7F", 1));                                //comp thresh

    VG99_default_replace("54", "00", 1, copyVBmid("54", "00", 1));                                //A mod1 sw
    VG99_default_replace("54", "01", 1, convertVB99_value(copyVBmid("54", "01", 1), "00", "0B")); //A mod1 type
    VG99_default_replace("54", "02", 44, copyVBmid("54", "02", 44));                              //A mod1 fx
    VG99_default_replace("54", "2E", 16, copyVBmid("54", "2E", 16));                              //A mod1 fx
    if(copyVBmid("54", "01", 1).toHex().toUpper()=="21") //pedal wah
    {
        VG99_default_replace("54", "12", 2, copyVBmid("54", "42", 2));                            //A mod1 tw effect/direct
        VG99_default_replace("54", "10", 2, copyVBmid("54", "3F", 1));                            //A mod1 tw pos~freq
    };
    VG99_default_replace("54", "45", 14, copyVBmid("54", "45", 14));                              //A mod1 afb/hu
    temp.clear();
    temp.append(copyVBmid("54", "53", 63));
    VG99_default_replace("54", "53", 45, temp.mid(0, 45));                                        //A mod1 fx
    VG99_default_replace("55", "00", 18, temp.mid(45, 18));                                       //A mod1 fx
    VG99_default_replace("55", "12", 4, copyVBmid("55", "12", 4));                                //A mod1 sdd
    VG99_default_replace("55", "1D", 24, copyVBmid("55", "1D", 24));                              //A mod1 user scale
    if(copyVBmid("54", "01", 1).toHex().toUpper()=="24") //geq
    {
        VG99_default_replace("54", "57", 1, copyVBmid("55", "3B", 1));                            //A mod1 geq
        VG99_default_replace("54", "5A", 1, copyVBmid("55", "3C", 1));                            //A mod1 geq
        VG99_default_replace("54", "5D", 1, copyVBmid("55", "3F", 1));                            //A mod1 geq
        VG99_default_replace("54", "5E", 1, copyVBmid("55", "41", 1));                            //A mod1 geq
        VG99_default_replace("54", "60", 1, copyVBmid("55", "3A", 1));                            //A mod1 geq
    };
    VG99_default_replace("58", "00", 1, copyVBmid("58", "00", 1));                                //A mod2 sw
    VG99_default_replace("58", "01", 1, convertVB99_value(copyVBmid("58", "01", 1), "00", "0B")); //A mod2 type
    VG99_default_replace("58", "02", 44, copyVBmid("58", "02", 44));                              //A mod2 fx
    VG99_default_replace("58", "2E", 16, copyVBmid("58", "2E", 16));                              //A mod2 fx
    if(copyVBmid("58", "01", 1).toHex().toUpper()=="21") //pedal wah
    {
        VG99_default_replace("58", "12", 2, copyVBmid("58", "42", 2));                            //A mod2 tw effect/direct
        VG99_default_replace("58", "10", 2, copyVBmid("58", "3F", 1));                            //A mod2 tw pos~freq
    };
    VG99_default_replace("58", "45", 14, copyVBmid("58", "45", 14));                              //A mod2 afb/hu
    temp.clear();
    temp.append(copyVBmid("58", "53", 63));
    VG99_default_replace("58", "53", 45, temp.mid(0, 45));                                        //A mod2 fx
    VG99_default_replace("59", "00", 18, temp.mid(45, 18));                                       //A mod2 fx
    VG99_default_replace("59", "12", 4, copyVBmid("59", "12", 4));                                //A mod2 sdd
    VG99_default_replace("59", "1D", 24, copyVBmid("59", "1D", 24));                              //A mod2 user scale
    if(copyVBmid("58", "01", 1).toHex().toUpper()=="24") //geq
    {
        VG99_default_replace("58", "57", 1, copyVBmid("59", "3B", 1));                            //A mod2 geq
        VG99_default_replace("58", "5A", 1, copyVBmid("59", "3C", 1));                            //A mod2 geq
        VG99_default_replace("58", "5D", 1, copyVBmid("59", "3F", 1));                            //A mod2 geq
        VG99_default_replace("58", "5E", 1, copyVBmid("59", "41", 1));                            //A mod2 geq
        VG99_default_replace("58", "60", 1, copyVBmid("59", "3A", 1));                            //A mod2 geq
    };

    VG99_default_replace("64", "00", 1, copyVBmid("64", "00", 1));                                //B mod1 sw
    VG99_default_replace("64", "01", 1, convertVB99_value(copyVBmid("64", "01", 1), "00", "0B")); //B mod1 type
    VG99_default_replace("64", "02", 44, copyVBmid("64", "02", 44));                              //B mod1 fx
    VG99_default_replace("64", "2E", 16, copyVBmid("64", "2E", 16));                              //B mod1 fx
    if(copyVBmid("64", "01", 1).toHex().toUpper()=="21") //pedal wah
    {
        VG99_default_replace("64", "12", 2, copyVBmid("64", "42", 2));                            //B mod1 tw effect/direct
        VG99_default_replace("64", "10", 2, copyVBmid("64", "3F", 1));                            //B mod1 tw pos~freq
    };
    VG99_default_replace("64", "45", 14, copyVBmid("64", "45", 14));                              //B mod1 afb/hu
    temp.clear();
    temp.append(copyVBmid("64", "53", 63));
    VG99_default_replace("64", "53", 45, temp.mid(0, 45));                                        //B mod1 fx
    VG99_default_replace("65", "00", 18, temp.mid(0, 18));                                        //B mod1 fx
    VG99_default_replace("65", "12", 4, copyVBmid("65", "12", 4));                                //B mod1 sdd
    VG99_default_replace("65", "1D", 24, copyVBmid("65", "1D", 24));                              //B mod1 user scale
    if(copyVBmid("64", "01", 1).toHex().toUpper()=="24") //geq
    {
        VG99_default_replace("64", "57", 1, copyVBmid("65", "3B", 1));                            //B mod1 geq
        VG99_default_replace("64", "5A", 1, copyVBmid("65", "3C", 1));                            //B mod1 geq
        VG99_default_replace("64", "5D", 1, copyVBmid("65", "3F", 1));                            //B mod1 geq
        VG99_default_replace("64", "5E", 1, copyVBmid("65", "41", 1));                            //B mod1 geq
        VG99_default_replace("64", "60", 1, copyVBmid("65", "3A", 1));                            //B mod1 geq
    };
    VG99_default_replace("68", "00", 1, copyVBmid("68", "00", 1));                                //B mod2 sw
    VG99_default_replace("68", "01", 1, convertVB99_value(copyVBmid("68", "01", 1), "00", "0B")); //B mod2 type
    VG99_default_replace("68", "02", 44, copyVBmid("68", "02", 44));                              //B mod2 fx
    VG99_default_replace("68", "2E", 16, copyVBmid("68", "2E", 16));                              //B mod2 fx
    if(copyVBmid("68", "01", 1).toHex().toUpper()=="21") //pedal wah
    {
        VG99_default_replace("68", "12", 2, copyVBmid("68", "42", 2));                            //B mod2 tw effect/direct
        VG99_default_replace("68", "10", 2, copyVBmid("68", "3F", 1));                            //B mod2 tw pos~freq
    };
    VG99_default_replace("68", "45", 14, copyVBmid("68", "45", 14));                              //B mod2 afb/hu
    temp.clear();
    temp.append(copyVBmid("68", "53", 63));
    VG99_default_replace("68", "53", 45, temp.mid(0, 45));                                        //B mod2 fx
    VG99_default_replace("69", "00", 18, temp.mid(45, 18));                                       //B mod2 fx
    VG99_default_replace("69", "12", 4, copyVBmid("69", "12", 4));                                //B mod2 sdd
    VG99_default_replace("69", "1D", 24, copyVBmid("69", "1D", 24));                              //B mod2 user scale
    if(copyVBmid("68", "01", 1).toHex().toUpper()=="24") //geq
    {
        VG99_default_replace("68", "57", 1, copyVBmid("69", "3B", 1));                            //B mod2 geq
        VG99_default_replace("68", "5A", 1, copyVBmid("69", "3C", 1));                            //B mod2 geq
        VG99_default_replace("68", "5D", 1, copyVBmid("69", "3F", 1));                            //B mod2 geq
        VG99_default_replace("68", "5E", 1, copyVBmid("69", "41", 1));                            //B mod2 geq
        VG99_default_replace("68", "60", 1, copyVBmid("69", "3A", 1));                            //B mod2 geq
    };

    VG99_default_replace("70", "00", 10, copyVBmid("70", "00", 10));                              //Assigns
    VG99_default_replace("70", "0A", 4, copyVBmid("70", "0A", 4));
    VG99_default_replace("70", "0E", 10, copyVBmid("70", "0E", 10));
    VG99_default_replace("70", "18", 4, copyVBmid("70", "18", 4));
    VG99_default_replace("70", "1C", 10, copyVBmid("70", "1C", 10));
    VG99_default_replace("70", "26", 4, copyVBmid("70", "26", 4));
    VG99_default_replace("70", "2A", 10, copyVBmid("70", "2A", 10));
    VG99_default_replace("70", "34", 4, copyVBmid("70", "34", 4));
    VG99_default_replace("70", "38", 10, copyVBmid("70", "38", 10));
    VG99_default_replace("70", "42", 4, copyVBmid("70", "42", 4));
    VG99_default_replace("70", "46", 10, copyVBmid("70", "46", 10));
    VG99_default_replace("70", "50", 4, copyVBmid("70", "50", 4));
    VG99_default_replace("70", "54", 10, copyVBmid("70", "54", 10));
    VG99_default_replace("70", "5E", 4, copyVBmid("70", "5E", 4));
    VG99_default_replace("70", "62", 10, copyVBmid("70", "62", 10));
    VG99_default_replace("70", "6C", 4, copyVBmid("70", "6C", 4));
    VG99_default_replace("70", "01", 2, convert_VB99_Assign(copyVBmid("70", "01", 2)));
    VG99_default_replace("70", "0F", 2, convert_VB99_Assign(copyVBmid("70", "0F", 2)));
    VG99_default_replace("70", "1D", 2, convert_VB99_Assign(copyVBmid("70", "1D", 2)));
    VG99_default_replace("70", "2B", 2, convert_VB99_Assign(copyVBmid("70", "2B", 2)));
    VG99_default_replace("70", "39", 2, convert_VB99_Assign(copyVBmid("70", "39", 2)));
    VG99_default_replace("70", "47", 2, convert_VB99_Assign(copyVBmid("70", "47", 2)));
    VG99_default_replace("70", "55", 2, convert_VB99_Assign(copyVBmid("70", "55", 2)));
    VG99_default_replace("70", "63", 2, convert_VB99_Assign(copyVBmid("70", "63", 2)));
    VG99_default_replace("71", "00", 10, copyVBmid("71", "00", 10));
    VG99_default_replace("71", "0A", 4, copyVBmid("71", "0A", 4));
    VG99_default_replace("71", "0E", 10, copyVBmid("71", "0E", 10));
    VG99_default_replace("71", "18", 4, copyVBmid("71", "18", 4));
    VG99_default_replace("71", "1C", 10, copyVBmid("71", "1C", 10));
    VG99_default_replace("71", "26", 4, copyVBmid("71", "26", 4));
    VG99_default_replace("71", "2A", 10, copyVBmid("71", "2A", 10));
    VG99_default_replace("71", "34", 4, copyVBmid("71", "34", 4));
    VG99_default_replace("71", "38", 10, copyVBmid("71", "38", 10));
    VG99_default_replace("71", "42", 4, copyVBmid("71", "42", 4));
    VG99_default_replace("71", "46", 10, copyVBmid("71", "46", 10));
    VG99_default_replace("71", "50", 4, copyVBmid("71", "50", 4));
    VG99_default_replace("71", "54", 10, copyVBmid("71", "54", 10));
    VG99_default_replace("71", "5E", 4, copyVBmid("71", "5E", 4));
    VG99_default_replace("71", "62", 10, copyVBmid("71", "62", 10));
    VG99_default_replace("71", "6C", 4, copyVBmid("71", "6C", 4));
    VG99_default_replace("71", "01", 2, convert_VB99_Assign(copyVBmid("71", "01", 2)));
    VG99_default_replace("71", "0F", 2, convert_VB99_Assign(copyVBmid("71", "0F", 2)));
    VG99_default_replace("71", "1D", 2, convert_VB99_Assign(copyVBmid("71", "1D", 2)));
    VG99_default_replace("71", "2B", 2, convert_VB99_Assign(copyVBmid("71", "2B", 2)));
    VG99_default_replace("71", "39", 2, convert_VB99_Assign(copyVBmid("71", "39", 2)));
    VG99_default_replace("71", "47", 2, convert_VB99_Assign(copyVBmid("71", "47", 2)));
    VG99_default_replace("71", "55", 2, convert_VB99_Assign(copyVBmid("71", "55", 2)));
    VG99_default_replace("71", "63", 2, convert_VB99_Assign(copyVBmid("71", "63", 2)));
    VG99_default_replace("72", "00", 10, copyVBmid("72", "00", 10));
    VG99_default_replace("72", "0A", 4, copyVBmid("72", "0A", 4));
    VG99_default_replace("72", "0E", 10, copyVBmid("72", "0E", 10));
    VG99_default_replace("72", "18", 4, copyVBmid("72", "18", 4));
    VG99_default_replace("72", "1C", 10, copyVBmid("72", "1C", 10));
    VG99_default_replace("72", "26", 4, copyVBmid("72", "26", 4));
    VG99_default_replace("72", "2A", 10, copyVBmid("72", "2A", 10));
    VG99_default_replace("72", "34", 4, copyVBmid("72", "34", 4));
    VG99_default_replace("72", "38", 10, copyVBmid("72", "38", 10));
    VG99_default_replace("72", "42", 4, copyVBmid("72", "42", 4));
    VG99_default_replace("72", "46", 10, copyVBmid("72", "46", 10));
    VG99_default_replace("72", "50", 4, copyVBmid("72", "50", 4));
    VG99_default_replace("72", "54", 10, copyVBmid("72", "54", 10));
    VG99_default_replace("72", "5E", 4, copyVBmid("72", "5E", 4));
    VG99_default_replace("72", "62", 10, copyVBmid("72", "62", 10));
    VG99_default_replace("72", "6C", 4, copyVBmid("72", "6C", 4));
    VG99_default_replace("72", "01", 2, convert_VB99_Assign(copyVBmid("72", "01", 2)));
    VG99_default_replace("72", "0F", 2, convert_VB99_Assign(copyVBmid("72", "0F", 2)));
    VG99_default_replace("72", "1D", 2, convert_VB99_Assign(copyVBmid("72", "1D", 2)));
    VG99_default_replace("72", "2B", 2, convert_VB99_Assign(copyVBmid("72", "2B", 2)));
    VG99_default_replace("72", "39", 2, convert_VB99_Assign(copyVBmid("72", "39", 2)));
    VG99_default_replace("72", "47", 2, convert_VB99_Assign(copyVBmid("72", "47", 2)));
    VG99_default_replace("72", "55", 2, convert_VB99_Assign(copyVBmid("72", "55", 2)));
    VG99_default_replace("72", "63", 2, convert_VB99_Assign(copyVBmid("72", "63", 2)));
    VG99_default_replace("73", "00", 10, copyVBmid("73", "00", 10));
    VG99_default_replace("73", "0A", 4, copyVBmid("73", "0A", 4));
    VG99_default_replace("73", "0E", 10, copyVBmid("73", "0E", 10));
    VG99_default_replace("73", "18", 4, copyVBmid("73", "18", 4));
    VG99_default_replace("73", "1C", 10, copyVBmid("73", "1C", 10));
    VG99_default_replace("73", "26", 4, copyVBmid("73", "26", 4));
    VG99_default_replace("73", "2A", 10, copyVBmid("73", "2A", 10));
    VG99_default_replace("73", "34", 4, copyVBmid("73", "34", 4));
    VG99_default_replace("73", "38", 10, copyVBmid("73", "38", 10));
    VG99_default_replace("73", "42", 4, copyVBmid("73", "42", 4));
    VG99_default_replace("73", "46", 10, copyVBmid("73", "46", 10));
    VG99_default_replace("73", "50", 4, copyVBmid("73", "50", 4));
    VG99_default_replace("73", "54", 10, copyVBmid("73", "54", 10));
    VG99_default_replace("73", "5E", 4, copyVBmid("73", "5E", 4));
    VG99_default_replace("73", "62", 10, copyVBmid("73", "62", 10));
    VG99_default_replace("73", "6C", 4, copyVBmid("73", "6C", 4));
    VG99_default_replace("73", "01", 2, convert_VB99_Assign(copyVBmid("73", "01", 2)));
    VG99_default_replace("73", "0F", 2, convert_VB99_Assign(copyVBmid("73", "0F", 2)));
    VG99_default_replace("73", "1D", 2, convert_VB99_Assign(copyVBmid("73", "1D", 2)));
    VG99_default_replace("73", "2B", 2, convert_VB99_Assign(copyVBmid("73", "2B", 2)));
    VG99_default_replace("73", "39", 2, convert_VB99_Assign(copyVBmid("73", "39", 2)));
    VG99_default_replace("73", "47", 2, convert_VB99_Assign(copyVBmid("73", "47", 2)));
    VG99_default_replace("73", "55", 2, convert_VB99_Assign(copyVBmid("73", "55", 2)));
    VG99_default_replace("73", "63", 2, convert_VB99_Assign(copyVBmid("73", "63", 2)));
    VG99_default_replace("73", "70", 1, copyVBmid("73", "70", 1));
    VG99_default_replace("7C", "00", 128, copyVBmid("7C", "00", 128));                       //favourites
    //VG99_default_replace("7D", "00", 15, copyVBmid("7D", "00", 15));

    //reserved VB99 bass amp types to SY-1000 conversion
    //reserved VB99 MOD wah types to SY-1000 conversion


    qDebug() << " VB_to_VG convert output data.size()=" << VG99_default.size();

    /*QByteArray deBug_Array;
            QFile file("deBug_data_for_gumtown.syx");           // Read the deBug file if exists.
            if (file.open(QIODevice::ReadWrite ))
            {
                deBug_Array = file.readAll();
                deBug_Array.clear();
                file.write(VG99_default);
                file.close();
            };*/

    return VG99_default;
}

QByteArray tsl_VG99_VB99_translation::convert_VB99_Assign(QByteArray target)
{
    //qDebug() << "convert_VB99_Assign(target)=" << target.toHex().toUpper();
    QString VGitem("2001");
    for(int x=0; x<13; x++)
    {
        int itemcount;
        QString addr = QString::number(x, 16).toUpper();
        if(addr.size()<2)
        {
            addr.prepend("0");
        };
        VG99_Midi items = getMidiMap("Tables", "00", "00", "02", addr);
        int itemSize = items.level.size();
        for(itemcount=0;itemcount<itemSize;itemcount++ )
        {
            QString item = items.level.at(itemcount).name;
            QString Litem = item.mid(0,2)+item.mid(3,2);
            //qDebug() << "Litem=" << Litem << " target=" << target.toHex().toUpper();
            if(Litem == target.toHex().toUpper())
            {
                QString Vitem = items.level.at(itemcount).desc;
                VGitem = Vitem.mid(0,2)+Vitem.mid(3,2);
                //qDebug() <<  QString("Assign_Target: "+item+"  Output: "+Vitem );
                break;
            };
        };
    };

    QByteArray result;
    bool ok;
    result.append(char(VGitem.mid(0,2).toInt(&ok,16))).append(char(VGitem.mid(2,2).toInt(&ok,16)));
    //qDebug() << "convert_VB99_Assign return=" << result.toHex().toUpper();
    return result;
}

void tsl_VG99_VB99_translation::loadMidiMap()
{
    QDomDocument doc( "VG-99 MIDI Transalation" );

    QFile file( ":/vg99_midi.xml" );
    if(file.open(QIODevice::ReadOnly))
    {
        doc.setContent( &file );                    // file is a QFile
        file.close();
    };
    QDomElement root = doc.documentElement();   // Points to <SysX>
    this->root = root;

    QDomNode node = root.firstChild();
    while ( !node.isNull() )
    {
        VG99_Midi section;
        section.type.append(node.nodeName());

        QDomNode level1Node = node.firstChild();
        while ( !level1Node.isNull() )
        {
            VG99_Midi level1;
            level1.type.append(level1Node.nodeName());
            level1.name = level1Node.attributes().namedItem("name").nodeValue();
            level1.value = level1Node.attributes().namedItem("value").nodeValue();
            level1.abbr = level1Node.attributes().namedItem("abbr").nodeValue();
            level1.desc = level1Node.attributes().namedItem("desc").nodeValue();
            level1.customdesc = level1Node.attributes().namedItem("customdesc").nodeValue();

            QDomNode level2Node = level1Node.firstChild();

            while ( !level2Node.isNull() )
            {
                VG99_Midi level2;
                level2.type.append(level2Node.nodeName());
                level2.name = level2Node.attributes().namedItem("name").nodeValue();
                level2.value = level2Node.attributes().namedItem("value").nodeValue();
                level2.abbr = level2Node.attributes().namedItem("abbr").nodeValue();
                level2.desc = level2Node.attributes().namedItem("desc").nodeValue();
                level2.customdesc = level2Node.attributes().namedItem("customdesc").nodeValue();

                QDomNode level3Node = level2Node.firstChild();

                while ( !level3Node.isNull() )
                {
                    VG99_Midi level3;
                    level3.type.append(level3Node.nodeName());
                    level3.name = level3Node.attributes().namedItem("name").nodeValue();
                    level3.value = level3Node.attributes().namedItem("value").nodeValue();
                    level3.abbr = level3Node.attributes().namedItem("abbr").nodeValue();
                    level3.desc = level3Node.attributes().namedItem("desc").nodeValue();
                    level3.customdesc = level3Node.attributes().namedItem("customdesc").nodeValue();

                    QDomNode  level4Node = level3Node.firstChild();

                    while ( !level4Node.isNull() )
                    {
                        VG99_Midi level4;
                        level4.type.append(level4Node.nodeName());
                        level4.name = level4Node.attributes().namedItem("name").nodeValue();
                        level4.value = level4Node.attributes().namedItem("value").nodeValue();
                        level4.abbr = level4Node.attributes().namedItem("abbr").nodeValue();
                        level4.desc = level4Node.attributes().namedItem("desc").nodeValue();
                        level4.customdesc = level4Node.attributes().namedItem("customdesc").nodeValue();

                        QDomNode level5Node = level4Node.firstChild();

                        while ( !level5Node.isNull() )
                        {
                            VG99_Midi level5;
                            level5.type.append(level5Node.nodeName());
                            level5.name = level5Node.attributes().namedItem("name").nodeValue();
                            level5.value = level5Node.attributes().namedItem("value").nodeValue();
                            level5.abbr = level5Node.attributes().namedItem("abbr").nodeValue();
                            level5.desc = level5Node.attributes().namedItem("desc").nodeValue();
                            level5.customdesc = level5Node.attributes().namedItem("customdesc").nodeValue();

                            level4.id.append(level5.value);
                            level4.level.append(level5);
                            level5Node = level5Node.nextSibling();
                        };

                        level3.id.append(level4.value);
                        level3.level.append(level4);
                        level4Node = level4Node.nextSibling();
                    };

                    level2.id.append(level3.value);
                    level2.level.append(level3);
                    level3Node = level3Node.nextSibling();
                };

                level1.id.append(level2.value);
                level1.level.append(level2);
                level2Node = level2Node.nextSibling();
            };

            section.id.append(level1.value);
            section.level.append(level1);
            level1Node = level1Node.nextSibling();
        };

        QString test = node.nodeName();
        this->midiMap.id.append(test);
        this->midiMap.level.append(section);
        node = node.nextSibling();
    };
}

int tsl_VG99_VB99_translation::getRange(QString root, QString hex1, QString hex2, QString hex3)
{
    VG99_Midi range;

    /* When FX has alot off settings (more than ) it's spanned over more then one entry in the
            midi.xml, so when out of range we jump to the next entry and start from 00. */
    bool ok;
    range = getMidiMap(this->getRoot(root), hex1, hex2, hex3);
    if(range.id.isEmpty())
    {
        return -1;
    };
    int lastIndex;
    if(range.level.last().value == "range")
    {
        lastIndex = range.level.last().name.split("/").at(1).toInt(&ok, 16);
    }
    else if(range.level.last().type.contains("DATA"))
    {
        int maxRange = 128;
        if (root == "Tables" && hex3 == "00") { maxRange = 256; };
        int lastIndex1 = range.level.last().value.toInt(&ok, 16);
        int lastIndex2 = range.level.last().level.last().value.toInt(&ok, 16);
        lastIndex = (lastIndex1 * maxRange) + lastIndex2;
    }
    else if(range.level.last().type.isEmpty() || range.level.last().type.contains(""))
    {
        lastIndex = 0;
    }
    else
    {
        lastIndex = range.level.last().value.toInt(&ok, 16);
    };
    return lastIndex;
}

int tsl_VG99_VB99_translation::getRangeMinimum(QString root, QString hex1, QString hex2, QString hex3)
{
    VG99_Midi range;

    /* When FX has alot off settings (more than ) it's spanned over more then one entry in the
                midi.xml, so when out of range we jump to the next entry and start from 00. */
    bool ok;

    range = getMidiMap(this->getRoot(root), hex1, hex2, hex3);
    if(range.id.isEmpty()){return -1;};
    int firstIndex;
    if(range.level.first().value == "range")
    {
        firstIndex = range.level.last().name.split("/").at(0).toInt(&ok, 16);
    }
    else if(range.level.last().type.contains("DATA"))
    {
        firstIndex = range.level.last().customdesc.toInt(&ok, 16);  //get manually entered value from level 4 bottom customdesc.
    }
    else
    {
        firstIndex = range.level.first().value.toInt(&ok, 16);
    };
    return firstIndex;
}

QString tsl_VG99_VB99_translation::getRoot(QString root)
{
    if(root=="10" || root=="Structure"){root="Structure";}
    else if(root=="00" || root=="System"){root="System";}
    else if(root=="Tables"){root="Tables";}
    else{root="Structure";};
    return root;
}

VG99_Midi tsl_VG99_VB99_translation::getMidiMap(QString root, QString hex1, QString hex2, QString hex3)
{
    VG99_Midi section = midiMap.level.at( midiMap.id.indexOf(this->getRoot(root)) );
    VG99_Midi level1 = section.level.at( section.id.indexOf(hex1) );
    VG99_Midi level2 = level1.level.at( level1.id.indexOf(hex2) );
    VG99_Midi level3;
    if(!level2.id.contains(hex3) && level2.id.contains("range"))
    {
        level3 = level2.level.at( level2.id.indexOf("range") );
    }
    else
    {
        level3 = level2.level.at( level2.id.indexOf(hex3) );
    };
    return level3;
}

VG99_Midi tsl_VG99_VB99_translation::getMidiMap(QString root, QString hex1, QString hex2, QString hex3, QString hex4)
{
    VG99_Midi section = midiMap.level.at( midiMap.id.indexOf(this->getRoot(root)) );
    VG99_Midi level1 = section.level.at( section.id.indexOf(hex1) );
    VG99_Midi level2 = level1.level.at( level1.id.indexOf(hex2) );
    VG99_Midi level3;
    if(!level2.id.contains(hex3) && level2.id.contains("range"))
    {
        level3 = level2.level.at( level2.id.indexOf("range") );
    }
    else
    {
        level3 = level2.level.at( level2.id.indexOf(hex3) );
    };

    VG99_Midi level4 = level3.level.at( level3.id.indexOf(hex4) );
    return level4;
}
