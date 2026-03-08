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
#include "tsl_GT1000_translation.h"
#include "fileDialog.h"
#include "globalVariables.h"
#include "MidiTable.h"
#include <QJsonArray>
#include <QRegularExpression>
#include "Preferences.h"


tsl_GT1000_translation::tsl_GT1000_translation(QByteArray patch, QByteArray default_hex, int mode, QString filename)
{
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){bass_mode = true;} else {bass_mode = false;};
    Q_UNUSED(mode);
    this->data = patch;
    this->SY1000_default = default_hex;
    this->fileName = filename;
    //convertFromTSL(0);
}

tsl_GT1000_translation::~tsl_GT1000_translation()
{

}

void tsl_GT1000_translation::patchIndex(int listIndex)
{
    Q_UNUSED(listIndex);
    SysxIO *sysxIO = SysxIO::Instance();
    this->index=sysxIO->patchListValue;
}

QByteArray tsl_GT1000_translation::GetJsonArray(QByteArray text, int pos )
{
    QByteArray null("ul");                                                      // when read later on without "" null = ul
    QByteArray str((char(34)+text+char(34)));
    int start_index = data.indexOf(str, pos)+(text.size()+4);                    //find pointer to start of Json value.
    int end_index = data.indexOf(",", start_index)-start_index-1;                  //find pointer to end of value to get the size of the value.
    QByteArray xq = data.mid(start_index , end_index );                           //copy the Json value and return as a QByteArray.
    if(data.mid(start_index, 4).contains(null)) {xq.clear(); };
    //if(x.at(0)==(char)34) {x.remove(0, 1); };
    //if(x.contains((char)34)) {x.truncate(x.indexOf((char)34)); };
    return xq;
}

QByteArray tsl_GT1000_translation::GetJsonString(QByteArray text, int pos )
{
    QByteArray str((char(34)+text+char(34)));
    int start_index = data.indexOf(str, pos)+(text.size()+4); //find pointer to start of Json value.
    int end_index = data.indexOf("]", start_index)-start_index;                  //find pointer to end of value to get the size of the value.
    QByteArray xq = data.mid(start_index , end_index );                 //copy the Json value and return as a QByteArray.
    bool ok;
    QByteArray zq;
    for(int f=0; f<xq.size(); f++){
        QByteArray yq;
        xq.remove(xq.indexOf(char(34)), 1);
        xq.remove(xq.indexOf(char(34)), 1);
        xq.remove(xq.indexOf(char(44)), 1);
        yq.append(xq.at(f)).append(xq.at(f+1));
        zq.append(char(yq.toInt(&ok, 16)));
        f=f+1;
    };
    return zq;
}

void tsl_GT1000_translation::SY1000_default_replace(int pos, int size, QByteArray value)
{

    if(!value.isEmpty() && !value.isNull())
    {
        SY1000_default.replace(pos, size, value);
    };
}

void tsl_GT1000_translation::SY1000_assign(int pos, const char* assign)
{
    QByteArray gt_assign = GetJsonString(assign, a);
    QByteArray sy_assign = gt_assign.mid(0, 1);

    bool bass_mode;
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){bass_mode = true;} else {bass_mode = false;};
    MidiTable *midiTable = MidiTable::Instance();
    bool ok;
    QString stn = QString(gt_assign.mid(1,4).toHex());
    stn.remove(6, 1);
    stn.remove(4, 1);
    stn.remove(2, 1);
    stn.remove(0, 1);
    int assign_num = stn.toInt(&ok, 16);
    // std::string stg = "ass-num="+QString::number(assign_num, 10).toStdString()+" string=" + stn.toStdString();
    // std::cerr << stg << std::endl;

    //QString assign_before(QString::number(assign_num, 10));
    int maxRange = 256;
    int dif = assign_num/maxRange;
    QString valueHex1 = QString::number(dif, 16).toUpper();
    if(valueHex1.length() < 2) valueHex1.prepend("0");
    QString valueHex2 = QString::number(assign_num - (dif * maxRange), 16).toUpper();
    if(valueHex2.length() < 2) valueHex2.prepend("0");
    QString hex4 = valueHex1;
    QString hex5 = valueHex2;                                     //convert valueStr to 7bit hex4, hex5
    Midi items = midiTable->getMidiMap("Tables", "00", "01", "0D", hex4, hex5);
    QString assign_after;
    if(bass_mode == true){assign_after = items.customdesc;}else{assign_after = items.desc;};
    assign_after.truncate(assign_after.indexOf(" "));
    assign_num = assign_after.toInt(&ok, 10);

    QString index = QString::number(assign_num, 16).toUpper();
    if(index.size()<2){index.prepend("000");};
    if(index.size()<3){index.prepend("00");};
    if(index.size()<4){index.prepend("0");};

    QString n3(index.at(3)); int b3 = n3.toInt(&ok, 16);
    QString n2(index.at(2)); int b2 = n2.toInt(&ok, 16);
    QString n1(index.at(1)); int b1 = n1.toInt(&ok, 16);
    QString n0(index.at(0)); int b0 = n0.toInt(&ok, 16);

    QByteArray new_value;
    new_value.append(char(b0)).append(char(b1)).append(char(b2)).append(char(b3));

    //qDebug() << "GT-1000 assign before = "+assign_before + "  SY-1000 assign after = " + assign_after + " Assign hex = " + index;


    sy_assign.append(new_value);           //target value
    sy_assign.append(gt_assign.mid(5, 8)); // tearget min/max
    sy_assign.append(convert_QByteArray_value(gt_assign.mid(13, 1), "0E")); //source
    sy_assign.append(gt_assign.mid(14, 1));  // source mode
    sy_assign.append(gt_assign.mid(20, 8));  //Act Range
    sy_assign.append(gt_assign.mid(18, 1));  //rise time (psudo)
    sy_assign.append(gt_assign.mid(18, 1));  //fall time (psudo)
    sy_assign.append(gt_assign.mid(15, 2));  //wave rate & form
    sy_assign.append(gt_assign.mid(28, 17)); //midi stuff

    SY1000_default_replace(pos, 43, sy_assign);
}

QByteArray tsl_GT1000_translation::convert_QByteArray_value(QByteArray value, QString hex3)
{
    bool ok;
    int v = value.toHex().toInt(&ok, 16);
    MidiTable *midiTable = MidiTable::Instance();
    Midi items;
    items = midiTable->getMidiMap("Tables", "00", "01", hex3);
    QString desc = items.level.at(v).desc;
    //QString before = items.level.at(v).name;
    //QString after = items.level.at(v).customdesc;
    int nv=desc.toInt(&ok, 16);
    QByteArray value2;
    value2.append(char(nv));
    if(value2.size()>1)
    {
        value2.remove(0, 1);
    };
    //nv = value2.toHex().toInt(&ok, 16);
    //QString m = before + "   " + after;
    //qDebug() << m;
    return value2;
}

QByteArray tsl_GT1000_translation::convertFromTSL(int patch)
{
    int patchCount = this->data.count("paramSet");
    int pindex = this->data.size()/(patchCount+1);
    //QString device=GetJsonArray("device", 0);

    if (patchCount>1)
    {
        QString msgText;
        QString patchText;
        QString patchNumber;
        this->patchList.clear();
        a = 0; // locate patch text start position from the start of the file
        for (int h=0;h<patchCount;h++)
        {
            patchText.append(GetJsonString("User_patch%common", a).mid(0, 16));
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
            fileDialog *dialog = new fileDialog(fileName, patchList);
            dialog->exec();
            dialog->deleteLater();
            patchIndex(this->index);
            if(sysxIO->patchListValue == 0){return(0);};
        }else{
            if(patch>patchCount){ patch=patchCount; };
            sysxIO->patchListValue = patch;
            this->index=sysxIO->patchListValue;
        };
    };

    a=0;
    if (patchCount>1)
    {
        int q=index-1;      // find start of required patch
        a = q*pindex;
    };

    Preferences *preferences = Preferences::Instance();
    QByteArray t;
    int x=0;
    t.append(char(1));
    SY1000_default.replace(1221, 1, convert_QByteArray_value(t, "02")); // switch on normal pu
    t.clear();
    t.append(char(0));
    SY1000_default.replace(1358, 1, convert_QByteArray_value(t, "02")); // switch off inst 1
    x = 0;
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){x = -44;};
    SY1000_default.replace(x+2064, 1, convert_QByteArray_value(t, "02")); // switch off inst 2
    x = 0;
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){x = -88;};
    SY1000_default.replace(x+2770, 1, convert_QByteArray_value(t, "02")); // switch off inst 3
    t = GetJsonString("User_patch%common", a);
    SY1000_default.replace(12, 16, t.mid(0, 16));

    SY1000_default.replace(28, 1, convert_QByteArray_value(t.mid(35, 1), "0A")); //num1 func
    SY1000_default.replace(29, 1, t.mid(36, 1));                                 //num1 toggle
    SY1000_default.replace(30, 1, convert_QByteArray_value(t.mid(37, 1), "0A")); //num2 func
    SY1000_default.replace(31, 1, t.mid(38, 1));                                 //num2 toggle
    SY1000_default.replace(32, 1, convert_QByteArray_value(t.mid(39, 1), "0A")); //num3 func
    SY1000_default.replace(33, 1, t.mid(40, 1));                                 //num3 toggle
    SY1000_default.replace(34, 1, convert_QByteArray_value(t.mid(41, 1), "0A")); //num4 func
    SY1000_default.replace(35, 1, t.mid(42, 1));                                 //num4 toggle
    SY1000_default.replace(36, 1, convert_QByteArray_value(t.mid(45, 1), "0A")); //bank dn func
    SY1000_default.replace(37, 1, t.mid(46, 1));                                 //bank dn toggle
    SY1000_default.replace(38, 1, convert_QByteArray_value(t.mid(47, 1), "0A")); //bank up func
    SY1000_default.replace(39, 1, t.mid(48, 1));                                 //bank up toggle
    SY1000_default.replace(40, 1, convert_QByteArray_value(t.mid(49, 1), "0B")); //ctl1 func
    SY1000_default.replace(41, 1, t.mid(50, 1));                                 //ctl1 toggle
    SY1000_default.replace(42, 1, convert_QByteArray_value(t.mid(51, 1), "0B")); //ctl2 func
    SY1000_default.replace(43, 1, t.mid(52, 1));                                 //ctl2 toggle
    SY1000_default.replace(44, 1, convert_QByteArray_value(t.mid(53, 1), "0B")); //ctl3 func
    SY1000_default.replace(45, 1, t.mid(54, 1));                                 //ctl3 toggle
    SY1000_default.replace(46, 1, convert_QByteArray_value(t.mid(55, 1), "0B")); //ctl4 func
    SY1000_default.replace(47, 1, t.mid(56, 1));                                 //ctl4 toggle
    SY1000_default.replace(48, 1, convert_QByteArray_value(t.mid(57, 1), "0B")); //ctl5 func
    SY1000_default.replace(49, 1, t.mid(58, 1));                                 //ctl5 toggle
    SY1000_default.replace(50, 1, convert_QByteArray_value(t.mid(59, 1), "0B")); //ctl6 func
    SY1000_default.replace(51, 1, t.mid(60, 1));                                 //ctl6 toggle
    SY1000_default.replace(52, 1, convert_QByteArray_value(t.mid(53, 1), "09")); //CURNUM func
    SY1000_default.replace(53, 1, t.mid(64, 1));                                 //CURNUM toggle
    SY1000_default.replace(54, 1, convert_QByteArray_value(t.mid(67, 1), "0B")); //EXP1 func
    SY1000_default.replace(55, 1, convert_QByteArray_value(t.mid(68, 1), "0B")); //EXP2 func
    SY1000_default.replace(61, 1, convert_QByteArray_value(t.mid(16, 1), "09")); //man num1 func
    SY1000_default.replace(62, 1, t.mid(17, 1));                                 //man num1 toggle
    SY1000_default.replace(63, 1, convert_QByteArray_value(t.mid(18, 1), "09")); //man num2 func
    SY1000_default.replace(64, 1, t.mid(19, 1));                                 //man num2 toggle
    SY1000_default.replace(65, 1, convert_QByteArray_value(t.mid(20, 1), "09")); //man num3 func
    SY1000_default.replace(66, 1, t.mid(21, 1));                                 //man num3 toggle
    SY1000_default.replace(67, 1, convert_QByteArray_value(t.mid(22, 1), "09")); //man num4 func
    SY1000_default.replace(68, 1, t.mid(23, 1));                                 //man num4 toggle
    SY1000_default.replace(69, 52, t.mid(74, 52));                               //patch midi 1~4

    //SY1000_default.replace(154, 27, t.mid(128, 27));
    t = GetJsonString("User_patch%led", a);
    SY1000_default.replace(211, 1, convert_QByteArray_value(t.mid(0, 1), "07")); // num1 led off
    SY1000_default.replace(212, 1, convert_QByteArray_value(t.mid(1, 1), "08")); // num1 led on
    SY1000_default.replace(213, 1, convert_QByteArray_value(t.mid(2, 1), "07")); // num2 led off
    SY1000_default.replace(214, 1, convert_QByteArray_value(t.mid(3, 1), "08")); // num2 led on
    SY1000_default.replace(215, 1, convert_QByteArray_value(t.mid(4, 1), "07")); // num3 led off
    SY1000_default.replace(216, 1, convert_QByteArray_value(t.mid(5, 1), "08")); // num3 led on
    SY1000_default.replace(217, 1, convert_QByteArray_value(t.mid(6, 1), "07")); // num4 led off
    SY1000_default.replace(218, 1, convert_QByteArray_value(t.mid(7, 1), "08")); // num4 led on
    SY1000_default.replace(219, 1, convert_QByteArray_value(t.mid(10, 1), "07")); // bank dn led off
    SY1000_default.replace(220, 1, convert_QByteArray_value(t.mid(11, 1), "08")); // bank dn led on
    SY1000_default.replace(221, 1, convert_QByteArray_value(t.mid(12, 1), "07")); // bank up led off
    SY1000_default.replace(222, 1, convert_QByteArray_value(t.mid(13, 1), "08")); // bank up led on
    SY1000_default.replace(223, 1, convert_QByteArray_value(t.mid(14, 1), "07")); // ctl1 led off
    SY1000_default.replace(224, 1, convert_QByteArray_value(t.mid(15, 1), "08")); // ctl1 led on
    SY1000_default.replace(225, 1, convert_QByteArray_value(t.mid(16, 1), "07")); // ctl2 led off
    SY1000_default.replace(226, 1, convert_QByteArray_value(t.mid(17, 1), "08")); // ctl2 led on
    SY1000_default.replace(227, 1, convert_QByteArray_value(t.mid(30, 1), "07")); // curnum led off
    SY1000_default.replace(228, 1, convert_QByteArray_value(t.mid(31, 1), "08")); // curnum led on
    SY1000_assign(252, "User_patch%assign(1)");
    SY1000_assign(316, "User_patch%assign(2)");
    SY1000_assign(373, "User_patch%assign(3)");
    SY1000_assign(437, "User_patch%assign(4)");
    SY1000_assign(494, "User_patch%assign(5)");
    SY1000_assign(558, "User_patch%assign(6)");
    SY1000_assign(615, "User_patch%assign(7)");
    SY1000_assign(679, "User_patch%assign(8)");
    SY1000_assign(736, "User_patch%assign(9)");
    SY1000_assign(800, "User_patch%assign(10)");
    SY1000_assign(857, "User_patch%assign(11)");
    SY1000_assign(921, "User_patch%assign(12)");
    SY1000_assign(978, "User_patch%assign(13)");
    SY1000_assign(1042, "User_patch%assign(14)");
    SY1000_assign(1099, "User_patch%assign(15)");
    SY1000_assign(1163, "User_patch%assign(16)");

    t = GetJsonString("User_patch%efct", a);
    SY1000_default.replace(1226, 13, t.mid(0, 13)); // fv1
    SY1000_default.replace(1252, 2, t.mid(13, 2));  // divider
    SY1000_default.replace(1254, 1, t.mid(24, 1));  // mix1 > mix balance
    SY1000_default.replace(1266, 5, t.mid(53, 5));  // send/return
    SY1000_default.replace(1260, 1, t.mid(37, 1));  // mix2 > balancer2 A/B bal
    SY1000_default.replace(1263, 1, t.mid(50, 1));  // mix3 > balancer3 A/B bal
    SY1000_default.replace(1274, 1, t.mid(69, 1));  // main stereo link
    SY1000_default.replace(1277, 1, t.mid(82, 1));  // sub stereo link
    SY1000_default.replace(1280, 7, t.mid(95, 7));  // master lev/bpm/key


    t = GetJsonString("User_patch%efctB", a);
    QByteArray SY_chain(QByteArrayLiteral("\x00\x01\x02\x03\x18\x18\x18\x18\x18"));
    QString chn = "CHAIN ";
    for(int c=0; c<48; c++)
    {
        QByteArray d(convert_QByteArray_value(t.mid(c, 1), "0F"));
        QString e(d.mid(0,1).toHex().toUpper());
        if(e.contains("11")){SY_chain.append(QByteArrayLiteral("\x18"));};
        if(e.contains("11")){SY_chain.append(QByteArrayLiteral("\x0F"));};
        if(!e.contains("7F")){SY_chain.append(d.mid(0,1));};
    };
    chn.append(QString(SY_chain.toHex().toUpper()));

    SY1000_default_replace(1289, 34, SY_chain);

    /*SY1000_default_replace(1220, 103, GetJsonString("UserBass_patch%efct", a));*/
    //SY1000_default_replace(1337, 7, GetJsonString(  "UserBass_patch%gtr2midi", a));
    /*SY1000_default_replace(1358, 18, GetJsonString("UserBass_patch%inst(1)%inst", a));
    t = GetJsonString("UserBass_patch%inst(1)%instDynaSynth", a);
    SY1000_default.replace(1390, 128, t.mid(0, 128));
    SY1000_default.replace(1532, 78, t.mid(128, 78));
    SY1000_default_replace(1624, 65, GetJsonString("UserBass_patch%inst(1)%instOscSynth", a));
    SY1000_default_replace(1703, 21, GetJsonString("UserBass_patch%inst(1)%instAnalogGr", a));
    SY1000_default_replace(1738, 45, GetJsonString("UserBass_patch%inst(1)%instEGtr", a));
    t = GetJsonString("UserBass_patch%inst(1)%instAcBass", a);
    SY1000_default.replace(1809, 35, t.mid(8, 35));
    t = GetJsonString("UserBass_patch%inst(1)%instEBass", a);
    SY1000_default.replace(1861, 43, t.mid(14, 43));
    SY1000_default_replace(1969, 19, GetJsonString("UserBass_patch%inst(1)%instPolyFx", a));
    SY1000_default_replace(2002, 48, GetJsonString("UserBass_patch%inst(1)%instAltTune", a));
    SY1000_default_replace(2064, 18, GetJsonString("UserBass_patch%inst(2)%inst", a));
    t = GetJsonString("UserBass_patch%inst(2)%instDynaSynth", a);
    SY1000_default.replace(2096, 128, t.mid(0, 128));
    SY1000_default.replace(2238, 78, t.mid(128, 78));
    SY1000_default_replace(2330, 65, GetJsonString("UserBass_patch%inst(2)%instOscSynth", a));
    SY1000_default_replace(2409, 21, GetJsonString("UserBass_patch%inst(2)%instAnalogGr", a));
    SY1000_default_replace(2444, 45, GetJsonString("UserBass_patch%inst(2)%instEGtr", a));
    t = GetJsonString("UserBass_patch%inst(2)%instAcBass", a);
    SY1000_default.replace(2515, 35, t.mid(8, 35));
    t = GetJsonString("UserBass_patch%inst(2)%instEBass", a);
    SY1000_default.replace(2567, 43, t.mid(14, 43));
    SY1000_default_replace(2675, 19, GetJsonString("UserBass_patch%inst(2)%instPolyFx", a));
    SY1000_default_replace(2708, 48, GetJsonString("UserBass_patch%inst(2)%instAltTune", a));
    SY1000_default_replace(2770, 18, GetJsonString("UserBass_patch%inst(3)%inst", a));
    t = GetJsonString("UserBass_patch%inst(3)%instDynaSynth", a);
    SY1000_default.replace(2802, 128, t.mid(0, 128));
    SY1000_default.replace(2944, 78, t.mid(128, 78));
    SY1000_default_replace(3036, 65, GetJsonString("UserBass_patch%inst(3)%instOscSynth", a));
    SY1000_default_replace(3115, 21, GetJsonString("UserBass_patch%inst(3)%instAnalogGr", a));
    SY1000_default_replace(3150, 45, GetJsonString("UserBass_patch%inst(3)%instEGtr", a));
    t = GetJsonString("UserBass_patch%inst(3)%instAcBass", a);
    SY1000_default.replace(3221, 35, t.mid(8, 35));
    t = GetJsonString("UserBass_patch%inst(3)%instEBass", a);
    SY1000_default.replace(3273, 43, t.mid(14, 43));
    SY1000_default_replace(3381, 19, GetJsonString("UserBass_patch%inst(3)%instPolyFx", a));
    SY1000_default_replace(3414, 48, GetJsonString("UserBass_patch%inst(3)%instAltTune", a));*/
    x = 0;
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){x = -132;};
    /*
    "User_patch%ns(2)"
    "User_patch%pedalFx"
*/
    t = GetJsonString("User_patch%comp", a);
    SY1000_default.replace(x+3476, 5, t.mid(0, 5));
    SY1000_default.replace(x+3481, 1, t.mid(7, 1));
    SY1000_default.replace(x+3482, 1, t.mid(5, 1));
    t = GetJsonString("User_patch%dist(1)", a);
    SY1000_default_replace(x+3497, 1, t.mid(0, 1));
    SY1000_default.replace(x+3498, 1, convert_QByteArray_value(t.mid(1, 1), "02")); // dist type
    SY1000_default_replace(x+3499, 7, t.mid(2, 7));
    SY1000_default_replace(x+3520, 14, GetJsonString("User_patch%preamp(1)", a));
    SY1000_default_replace(x+3554, 4, GetJsonString("User_patch%ns(1)", a));
    SY1000_default_replace(x+3557, 1, QByteArrayLiteral("\x02")); // set NS sense to NS input
    t = GetJsonString("User_patch%eq(1)", a);
    SY1000_default.replace(x+3572, 1, t.mid(0, 1));
    SY1000_default.replace(x+3573, 2, t.mid(2, 2));
    SY1000_default.replace(x+3575, 6, t.mid(5, 6));
    SY1000_default.replace(x+3581, 1, convert_QByteArray_value(t.mid(11, 1), "00")); // low cut
    SY1000_default.replace(x+3582, 1, convert_QByteArray_value(t.mid(12, 1), "01")); // high cut
    SY1000_default.replace(x+3583, 1, t.mid(4, 1));
    t = GetJsonString("User_patch%eq(2)", a);
    SY1000_default.replace(x+3598, 1, t.mid(0, 1));
    SY1000_default.replace(x+3599, 2, t.mid(2, 2));
    SY1000_default.replace(x+3601, 6, t.mid(5, 6));
    SY1000_default.replace(x+3607, 1, convert_QByteArray_value(t.mid(11, 1), "00")); // low cut
    SY1000_default.replace(x+3608, 1, convert_QByteArray_value(t.mid(12, 1), "01")); // high cut
    SY1000_default.replace(x+3609, 1, t.mid(4, 1));
    t = GetJsonString("User_patch%delay(1)", a);
    SY1000_default.replace(x+3624, 6, t.mid(0, 6));
    SY1000_default.replace(x+3631, 2, t.mid(7, 2));
    t = GetJsonString("User_patch%delay(2)", a);
    SY1000_default.replace(x+3648, 6, t.mid(0, 6));
    SY1000_default.replace(x+3655, 2, t.mid(7, 2));
    t = GetJsonString("User_patch%mstDelay", a);
    SY1000_default.replace(x+3672, 1, t.mid(0, 1));  // MSTDELAY ON/OFF
    SY1000_default.replace(x+3673, 1, convert_QByteArray_value(t.mid(1, 1), "03")); // mst delay type
    SY1000_default.replace(x+3674, 5, t.mid(2, 5)); // time + feedback
    SY1000_default.replace(x+3679, 1, convert_QByteArray_value(t.mid(7, 1), "01")); // high cut
    SY1000_default.replace(x+3680, 1, t.mid(8, 1));   // effect
    SY1000_default.replace(x+3681, 1, t.mid(14, 1));  // direct
    SY1000_default.replace(x+3682, 5, t.mid(20, 5)); // D1 time + feedback
    SY1000_default.replace(x+3687, 1, convert_QByteArray_value(t.mid(25, 1), "01")); // high cut
    SY1000_default.replace(x+3688, 1, t.mid(26, 1));  // D1 effect
    SY1000_default.replace(x+3689, 5, t.mid(28, 5));  // D2 time + feedback
    SY1000_default.replace(x+3694, 1, convert_QByteArray_value(t.mid(33, 1), "01")); // high cut
    SY1000_default.replace(x+3695, 1, t.mid(34, 1));  // effect
    SY1000_default.replace(x+3699, 4, t.mid(35, 4));  // twist
    SY1000_default.replace(x+3696, 2, t.mid(9, 2));   // mod
    t = GetJsonString("User_patch%chorus", a);
    SY1000_default_replace(x+3719, 25, GetJsonString("User_patch%chorus", a));
    SY1000_default.replace(x+3726, 1, convert_QByteArray_value(t.mid(7, 1), "00"));  // low cut
    SY1000_default.replace(x+3727, 1, convert_QByteArray_value(t.mid(8, 1), "01"));  // high cut
    SY1000_default.replace(x+3733, 1, convert_QByteArray_value(t.mid(14, 1), "00")); // low cut
    SY1000_default.replace(x+3734, 1, convert_QByteArray_value(t.mid(15, 1), "01")); // high cut
    SY1000_default.replace(x+3740, 1, convert_QByteArray_value(t.mid(21, 1), "00")); // low cut
    SY1000_default.replace(x+3741, 1, convert_QByteArray_value(t.mid(22, 1), "01")); // high cut
    t = GetJsonString("User_patch%fx(1)%fx", a);
    SY1000_default_replace(x+3758, 2, GetJsonString("User_patch%fx(1)%fx", a));
    SY1000_default.replace(x+3759, 1, convert_QByteArray_value(t.mid(1, 1), "04")); // fx1 type
    SY1000_default_replace(x+3774, 4, GetJsonString("User_patch%fx(1)%fxAcReso", a));
    SY1000_default_replace(x+3792, 8, GetJsonString("User_patch%fx(1)%fxAWah", a));
    t = GetJsonString("User_patch%fx(1)%fxChorus", a);
    SY1000_default.replace(x+3815, 1, convert_QByteArray_value(t.mid(0, 1), "05")); // FX Chorus type
    SY1000_default.replace(x+3816, 21, t.mid(8, 21));
    SY1000_default.replace(x+3837, 2, t.mid(1, 2));
    SY1000_default.replace(x+3821, 1, convert_QByteArray_value(t.mid(13, 1), "00")); // low cut
    SY1000_default.replace(x+3822, 1, convert_QByteArray_value(t.mid(14, 1), "01")); // high cut
    SY1000_default.replace(x+3828, 1, convert_QByteArray_value(t.mid(20, 1), "00")); // low cut
    SY1000_default.replace(x+3829, 1, convert_QByteArray_value(t.mid(21, 1), "01")); // high cut
    SY1000_default.replace(x+3835, 1, convert_QByteArray_value(t.mid(27, 1), "00")); // low cut
    SY1000_default.replace(x+3836, 1, convert_QByteArray_value(t.mid(28, 1), "01")); // high cut
    t = GetJsonString("User_patch%fx(1)%fxCVibe", a);
    SY1000_default.replace(x+3853, 3, t.mid(1, 3));
    t = GetJsonString("User_patch%fx(1)%fxComp", a);
    SY1000_default.replace(x+3871, 4, t.mid(0, 4));
    SY1000_default.replace(x+3875, 1, t.mid(6, 1));
    SY1000_default.replace(x+3876, 1, t.mid(4, 1));
    t = GetJsonString("User_patch%fx(1)%fxDefretter", a);
    SY1000_default.replace(x+3891, 7, t.mid(0, 7));
    //SY1000_default_replace(x+3912, 5, GetJsonString("User_patch%fx(1)%fxDefretterBass", a)); //na
    //SY1000_default_replace(x+3931, 33, GetJsonString("User_patch%fx(1)%fxDelay", a)); //na
    t = GetJsonString("User_patch%fx(1)%fxFlanger", a);
    SY1000_default.replace(x+3978, 4, t.mid(0, 4));
    SY1000_default.replace(x+3982, 1, t.mid(7, 1));
    SY1000_default.replace(x+3983, 1, convert_QByteArray_value(t.mid(13, 1), "00")); // low cut
    SY1000_default.replace(x+3984, 1, convert_QByteArray_value(t.mid(14, 1), "01")); // high cut
    SY1000_default.replace(x+3985, 1, t.mid(8, 1));
    SY1000_default.replace(x+3986, 1, t.mid(15, 1));
    //SY1000_default_replace(x+4001, 9, GetJsonString("User_patch%fx(1)%fxFlangerBass", a)); //na
    //SY1000_default_replace(x+4024, 13, GetJsonString("User_patch%fx(1)%fxFv", a)); //na
    //SY1000_default_replace(x+4051, 8, GetJsonString("User_patch%fx(1)%fxGEq", a)); //na
    SY1000_default_replace(x+4073, 39, GetJsonString("User_patch%fx(1)%fxHarmonist", a));
    SY1000_default_replace(x+4126, 8, GetJsonString("User_patch%fx(1)%fxHumanizer", a));
    //SY1000_default_replace(x+4148, 4, GetJsonString("User_patch%fx(1)%fxIsolator", a)); //na
    //SY1000_default_replace(x+4166, 6, GetJsonString("User_patch%fx(1)%fxLimiter", a)); //na
    //SY1000_default_replace(x+4186, 3, GetJsonString("User_patch%fx(1)%fxLofi", a)); //na
    t = GetJsonString("User_patch%fx(1)%fxOctave", a);
    SY1000_default.replace(x+4203, 3, t.mid(1, 3));
    //SY1000_default_replace(x+4220, 3, GetJsonString("User_patch%fx(1)%fxOctaveBass", a)); //na
    t = GetJsonString("User_patch%fx(1)%fxPan", a);
    SY1000_default.replace(x+4238, 1, t.mid(2, 1));
    SY1000_default.replace(x+4239, 2, t.mid(0, 2));
    SY1000_default.replace(x+4241, 1, t.mid(3, 1));
    //SY1000_default_replace(x+4257, 12, GetJsonString("User_patch%fx(1)%fxPEq", a)); //na
    //SY1000_default_replace(x+4283, 7, GetJsonString("User_patch%fx(1)%fxPedalBend", a)); //na
    t = GetJsonString("User_patch%fx(1)%fxPhaser", a);
    SY1000_default.replace(x+4304, 6, t.mid(1, 6));
    SY1000_default.replace(x+4310, 2, t.mid(16, 2));
    SY1000_default_replace(x+4326, 19, GetJsonString("User_patch%fx(1)%fxPitchShift", a));
    // SY1000_default_replace(x+4359, 14, GetJsonString("User_patch%fx(1)%fxReverb", a));
    SY1000_default_replace(x+4387, 6, GetJsonString("User_patch%fx(1)%fxRingMod", a));
    t = GetJsonString("User_patch%fx(1)%fxRotary", a);
    SY1000_default.replace(x+4407, 6, t.mid(0, 6));
    SY1000_default.replace(x+4414, 1, t.mid(9, 1));
    SY1000_default_replace(x+4429, 7, GetJsonString("User_patch%fx(1)%fxSitarSim", a));
    SY1000_default_replace(x+4450, 7, GetJsonString("User_patch%fx(1)%fxSlicer", a));
    t = GetJsonString("User_patch%fx(1)%fxSlowGear", a);
    SY1000_default.replace(x+4471, 3, t.mid(0, 3));
    //SY1000_default_replace(x+4488, 3, GetJsonString("User_patch%fx(1)%fxSlowGearBass", a));
    //SY1000_default_replace(x+4505, 3, GetJsonString("User_patch%fx(1)%fxSoundHold", a));
    //SY1000_default_replace(x+4522, 8, GetJsonString("User_patch%fx(1)%fxTWah", a));
    //SY1000_default_replace(x+4544, 8, GetJsonString("User_patch%fx(1)%fxTWahBass", a));
    t = GetJsonString("User_patch%fx(1)%fxTremolo", a);
    SY1000_default.replace(x+4566, 1, t.mid(2, 1));
    SY1000_default.replace(x+4567, 2, t.mid(0, 2));
    SY1000_default.replace(x+4569, 1, t.mid(3, 1));
    SY1000_default.replace(x+4570, 1, t.mid(6, 1));
    t = GetJsonString("User_patch%fx(1)%fxVibrato", a);
    SY1000_default.replace(x+4585, 2, t.mid(0, 2));
    SY1000_default.replace(x+4587, 2, t.mid(4, 2));
    SY1000_default.replace(x+4589, 1, t.mid(6, 1));
    //SY1000_default_replace(x+4605, 15, GetJsonString("User_patch%fx(1)%fxWah", a));

    t = GetJsonString("User_patch%fx(2)%fx", a);
    SY1000_default.replace(x+4634, 2, t.mid(0, 2));
    SY1000_default.replace(x+4635, 1, convert_QByteArray_value(t.mid(1, 1), "04")); // fx type
    SY1000_default_replace(x+4650, 4, GetJsonString("User_patch%fx(2)%fxAcReso", a));
    SY1000_default_replace(x+4668, 8, GetJsonString("User_patch%fx(2)%fxAWah", a));
    t = GetJsonString("User_patch%fx(2)%fxChorus", a);
    SY1000_default.replace(x+4691, 1, convert_QByteArray_value(t.mid(0, 1), "05")); // FX Chorus type
    SY1000_default.replace(x+4692, 21, t.mid(8, 21));
    SY1000_default.replace(x+4713, 2, t.mid(1, 2));
    SY1000_default.replace(x+4697, 1, convert_QByteArray_value(t.mid(13, 1), "00")); // low cut
    SY1000_default.replace(x+4698, 1, convert_QByteArray_value(t.mid(14, 1), "01")); // high cut
    SY1000_default.replace(x+4704, 1, convert_QByteArray_value(t.mid(20, 1), "00")); // low cut
    SY1000_default.replace(x+4705, 1, convert_QByteArray_value(t.mid(21, 1), "01")); // high cut
    SY1000_default.replace(x+4711, 1, convert_QByteArray_value(t.mid(27, 1), "00")); // low cut
    SY1000_default.replace(x+4712, 1, convert_QByteArray_value(t.mid(28, 1), "01")); // high cut
    t = GetJsonString("User_patch%fx(2)%fxCVibe", a);
    SY1000_default.replace(x+4729, 3, t.mid(1, 3));
    t = GetJsonString("User_patch%fx(1)%fxComp", a);
    SY1000_default.replace(x+4747, 4, t.mid(0, 4));
    SY1000_default.replace(x+4751, 1, t.mid(6, 1));
    SY1000_default.replace(x+4752, 1, t.mid(4, 1));
    t = GetJsonString("User_patch%fx(2)%fxDefretter", a);
    SY1000_default.replace(x+4767, 7, t.mid(0, 7));
    //SY1000_default_replace(x+4788, 5, GetJsonString("User_patch%fx(2)%fxDefretterBass", a)); //na
    //SY1000_default_replace(x+4807, 33, GetJsonString("User_patch%fx(2)%fxDelay", a)); //na
    t = GetJsonString("User_patch%fx(2)%fxFlanger", a);
    SY1000_default.replace(x+4854, 4, t.mid(0, 4));
    SY1000_default.replace(x+4858, 1, t.mid(7, 1));
    SY1000_default.replace(x+4859, 1, convert_QByteArray_value(t.mid(13, 1), "00")); // low cut
    SY1000_default.replace(x+4860, 1, convert_QByteArray_value(t.mid(14, 1), "01")); // high cut
    SY1000_default.replace(x+4861, 1, t.mid(8, 1));
    SY1000_default.replace(x+4862, 1, t.mid(15, 1));
    //SY1000_default_replace(x+4877, 9, GetJsonString("User_patch%fx(2)%fxFlangerBass", a)); //na
    //SY1000_default_replace(x+4900, 13, GetJsonString("User_patch%fx(2)%fxFv", a)); //na
    //SY1000_default_replace(x+4927, 8, GetJsonString("User_patch%fx(2)%fxGEq", a)); //na
    SY1000_default_replace(x+4949, 39, GetJsonString("User_patch%fx(2)%fxHarmonist", a));
    SY1000_default_replace(x+5002, 8, GetJsonString("User_patch%fx(2)%fxHumanizer", a));
    //SY1000_default_replace(x+5024, 4, GetJsonString("User_patch%fx(2)%fxIsolator", a)); //na
    //SY1000_default_replace(x+5042, 6, GetJsonString("User_patch%fx(2)%fxLimiter", a)); //na
    //SY1000_default_replace(x+5062, 3, GetJsonString("User_patch%fx(2)%fxLofi", a)); //na
    t = GetJsonString("User_patch%fx(2)%fxOctave", a);
    SY1000_default.replace(x+5079, 3, t.mid(1, 3));
    //SY1000_default_replace(x+5096, 3, GetJsonString("User_patch%fx(2)%fxOctaveBass", a)); //na
    t = GetJsonString("User_patch%fx(2)%fxPan", a);
    SY1000_default.replace(x+5114, 1, t.mid(2, 1));
    SY1000_default.replace(x+5115, 2, t.mid(0, 2));
    SY1000_default.replace(x+5117, 1, t.mid(3, 1));
    //SY1000_default_replace(x+5133, 12, GetJsonString("User_patch%fx(2)%fxPEq", a)); //na
    //SY1000_default_replace(x+5159, 7, GetJsonString("User_patch%fx(2)%fxPedalBend", a)); //na
    t = GetJsonString("User_patch%fx(2)%fxPhaser", a);
    SY1000_default.replace(x+5180, 6, t.mid(1, 6));
    SY1000_default.replace(x+5186, 2, t.mid(16, 2));
    SY1000_default_replace(x+5202, 19, GetJsonString("User_patch%fx(2)%fxPitchShift", a));
    // SY1000_default_replace(x+5235, 14, GetJsonString("User_patch%fx(2)%fxReverb", a));
    SY1000_default_replace(x+5263, 6, GetJsonString("User_patch%fx(2)%fxRingMod", a));
    t = GetJsonString("User_patch%fx(2)%fxRotary", a);
    SY1000_default.replace(x+5283, 6, t.mid(0, 6));
    SY1000_default.replace(x+5290, 1, t.mid(9, 1));
    SY1000_default_replace(x+5305, 7, GetJsonString("User_patch%fx(2)%fxSitarSim", a));
    SY1000_default_replace(x+5326, 7, GetJsonString("User_patch%fx(2)%fxSlicer", a));
    t = GetJsonString("User_patch%fx(2)%fxSlowGear", a);
    SY1000_default.replace(x+5347, 3, t.mid(0, 3));
    //SY1000_default_replace(x+5364, 3, GetJsonString("User_patch%fx(2)%fxSlowGearBass", a));
    //SY1000_default_replace(x+5381, 3, GetJsonString("User_patch%fx(2)%fxSoundHold", a));
    //SY1000_default_replace(x+5398, 8, GetJsonString("User_patch%fx(2)%fxTWah", a));
    //SY1000_default_replace(x+5420, 8, GetJsonString("User_patch%fx(2)%fxTWahBass", a));
    t = GetJsonString("User_patch%fx(2)%fxTremolo", a);
    SY1000_default.replace(x+5442, 1, t.mid(2, 1));
    SY1000_default.replace(x+5443, 2, t.mid(0, 2));
    SY1000_default.replace(x+5445, 1, t.mid(3, 1));
    SY1000_default.replace(x+5446, 1, t.mid(6, 1));
    t = GetJsonString("User_patch%fx(2)%fxVibrato", a);
    SY1000_default.replace(x+5461, 2, t.mid(0, 2));
    SY1000_default.replace(x+5463, 2, t.mid(4, 2));
    SY1000_default.replace(x+5465, 1, t.mid(6, 1));
    //SY1000_default_replace(x+5481, 15, GetJsonString("User_patch%fx(2)%fxWah", a));

    t = GetJsonString("User_patch%fx(3)%fx", a);
    SY1000_default.replace(x+5510, 1, t.mid(0, 1));
    SY1000_default.replace(x+5511, 1, convert_QByteArray_value(t.mid(1, 1), "04")); // fx type
    SY1000_default_replace(x+5526, 4, GetJsonString("User_patch%fx(3)%fxAcReso", a));
    SY1000_default_replace(x+5544, 8, GetJsonString("User_patch%fx(3)%fxAWah", a));
    t = GetJsonString("User_patch%fx(3)%fxChorus", a);
    SY1000_default.replace(x+5567, 1, convert_QByteArray_value(t.mid(0, 1), "05")); // FX Chorus type
    SY1000_default.replace(x+5568, 21, t.mid(8, 21));
    SY1000_default.replace(x+5589, 2, t.mid(1, 2));
    SY1000_default.replace(x+5573, 1, convert_QByteArray_value(t.mid(13, 1), "00")); // low cut
    SY1000_default.replace(x+5574, 1, convert_QByteArray_value(t.mid(14, 1), "01")); // high cut
    SY1000_default.replace(x+5580, 1, convert_QByteArray_value(t.mid(20, 1), "00")); // low cut
    SY1000_default.replace(x+5581, 1, convert_QByteArray_value(t.mid(21, 1), "01")); // high cut
    SY1000_default.replace(x+5587, 1, convert_QByteArray_value(t.mid(27, 1), "00")); // low cut
    SY1000_default.replace(x+5588, 1, convert_QByteArray_value(t.mid(28, 1), "01")); // high cut
    t = GetJsonString("User_patch%fx(3)%fxCVibe", a);
    SY1000_default.replace(x+5605, 3, t.mid(1, 3));
    t = GetJsonString("User_patch%fx(3)%fxComp", a);
    SY1000_default.replace(x+5623, 4, t.mid(0, 4));
    SY1000_default.replace(x+5627, 1, t.mid(6, 1));
    SY1000_default.replace(x+5628, 1, t.mid(4, 1));
    t = GetJsonString("User_patch%fx(3)%fxDefretter", a);
    SY1000_default.replace(x+5643, 7, t.mid(0, 7));
    //SY1000_default_replace(x+5664, 5, GetJsonString("User_patch%fx(3)%fxDefretterBass", a)); //na
    //SY1000_default_replace(x+5683, 33, GetJsonString("User_patch%fx(3)%fxDelay", a)); //na
    t = GetJsonString("User_patch%fx(3)%fxFlanger", a);
    SY1000_default.replace(x+5730, 4, t.mid(0, 4));
    SY1000_default.replace(x+5734, 1, t.mid(7, 1));
    SY1000_default.replace(x+5735, 1, convert_QByteArray_value(t.mid(13, 1), "00")); // low cut
    SY1000_default.replace(x+5736, 1, convert_QByteArray_value(t.mid(14, 1), "01")); // high cut
    SY1000_default.replace(x+5737, 1, t.mid(8, 1));
    SY1000_default.replace(x+5738, 1, t.mid(15, 1));
    //SY1000_default_replace(x+5753, 9, GetJsonString("User_patch%fx(3)%fxFlangerBass", a)); //na
    //SY1000_default_replace(x+5776, 13, GetJsonString("User_patch%fx(3)%fxFv", a)); //na
    //SY1000_default_replace(x+5803, 8, GetJsonString("User_patch%fx(3)%fxGEq", a)); //na
    SY1000_default_replace(x+5825, 39, GetJsonString("User_patch%fx(3)%fxHarmonist", a));
    SY1000_default_replace(x+5878, 8, GetJsonString("User_patch%fx(3)%fxHumanizer", a));
    //SY1000_default_replace(x+5900, 4, GetJsonString("User_patch%fx(3)%fxIsolator", a)); //na
    //SY1000_default_replace(x+5918, 6, GetJsonString("User_patch%fx(3)%fxLimiter", a)); //na
    //SY1000_default_replace(x+5938, 3, GetJsonString("User_patch%fx(3)%fxLofi", a)); //na
    t = GetJsonString("User_patch%fx(3)%fxOctave", a);
    SY1000_default.replace(x+5955, 3, t.mid(1, 3));
    //SY1000_default_replace(x+5972, 3, GetJsonString("User_patch%fx(3)%fxOctaveBass", a)); //na
    t = GetJsonString("User_patch%fx(3)%fxPan", a);
    SY1000_default.replace(x+5990, 1, t.mid(2, 1));
    SY1000_default.replace(x+5991, 2, t.mid(0, 2));
    SY1000_default.replace(x+5993, 1, t.mid(3, 1));
    //SY1000_default_replace(x+6009, 12, GetJsonString("User_patch%fx(3)%fxPEq", a)); //na
    //SY1000_default_replace(x+6035, 7, GetJsonString("User_patch%fx(3)%fxPedalBend", a)); //na
    t = GetJsonString("User_patch%fx(3)%fxPhaser", a);
    SY1000_default.replace(x+6056, 6, t.mid(1, 6));
    SY1000_default.replace(x+6062, 2, t.mid(16, 2));
    SY1000_default_replace(x+6078, 19, GetJsonString("User_patch%fx(3)%fxPitchShift", a));
    // SY1000_default_replace(x+6111, 14, GetJsonString("User_patch%fx(3)%fxReverb", a));
    SY1000_default_replace(x+6139, 6, GetJsonString("User_patch%fx(3)%fxRingMod", a));
    t = GetJsonString("User_patch%fx(3)%fxRotary", a);
    SY1000_default.replace(x+6159, 6, t.mid(0, 6));
    SY1000_default.replace(x+6166, 1, t.mid(9, 1));
    SY1000_default_replace(x+6181, 7, GetJsonString("User_patch%fx(3)%fxSitarSim", a));
    SY1000_default_replace(x+6202, 7, GetJsonString("User_patch%fx(3)%fxSlicer", a));
    t = GetJsonString("User_patch%fx(3)%fxSlowGear", a);
    SY1000_default.replace(x+6223, 3, t.mid(0, 3));
    //SY1000_default_replace(x+6240, 3, GetJsonString("User_patch%fx(3)%fxSlowGearBass", a));
    //SY1000_default_replace(x+6257, 3, GetJsonString("User_patch%fx(3)%fxSoundHold", a));
    //SY1000_default_replace(x+6274, 8, GetJsonString("User_patch%fx(3)%fxTWah", a));
    //SY1000_default_replace(x+6296, 8, GetJsonString("User_patch%fx(3)%fxTWahBass", a));
    t = GetJsonString("User_patch%fx(3)%fxTremolo", a);
    SY1000_default.replace(x+6318, 1, t.mid(2, 1));
    SY1000_default.replace(x+6319, 2, t.mid(0, 2));
    SY1000_default.replace(x+6321, 1, t.mid(3, 1));
    SY1000_default.replace(x+6322, 1, t.mid(6, 1));
    t = GetJsonString("User_patch%fx(3)%fxVibrato", a);
    SY1000_default.replace(x+6337, 2, t.mid(0, 2));
    SY1000_default.replace(x+6339, 2, t.mid(4, 2));
    SY1000_default.replace(x+6341, 1, t.mid(6, 1));
    //SY1000_default_replace(x+6357, 15, GetJsonString("User_patch%fx(3)%fxWah", a));

    t = GetJsonString("User_patch%reverb", a);
    SY1000_default.replace(x+6386, 1, t.mid(0, 1));
    SY1000_default.replace(x+6387, 1, convert_QByteArray_value(t.mid(1, 1), "06")); // rev type
    SY1000_default.replace(x+6388, 1, t.mid(10, 1));
    SY1000_default.replace(x+6394, 1, t.mid(12, 1));
    SY1000_default.replace(x+6395, 1, convert_QByteArray_value(t.mid(16, 1), "00")); // low cut
    SY1000_default.replace(x+6396, 1, convert_QByteArray_value(t.mid(17, 1), "01")); // high cut
    SY1000_default.replace(x+6397, 1, t.mid(13, 1));
    SY1000_default.replace(x+6398, 1, t.mid(2, 1));
    SY1000_default.replace(x+6399, 1, t.mid(42, 1));

    QString moad = "Guitar";
    if(bass_mode == true)
    {
        moad = "Bass";
    };
    SysxIO *sysxIO = SysxIO::Instance();
    sysxIO->deBug("(" + moad +" Mode) GT-1000 conversion size = " + QString::number(this->SY1000_default.size(), 10));

    if(this->SY1000_default.size() == patchSize || this->SY1000_default.size() == patchSizeBass)
    {
        return(this->SY1000_default);
    }
    else
    {
        sysxIO->deBug("SHIT HAPPENED IN SY-1000 FILE TRANSLATION");

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
