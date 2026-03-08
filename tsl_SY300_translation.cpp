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
#include "tsl_SY300_translation.h"
#include "fileDialog.h"
#include "globalVariables.h"
#include "MidiTable.h"
#include <QJsonArray>
#include <QRegularExpression>
#include "Preferences.h"


tsl_translation::tsl_translation(QByteArray patch, QByteArray default_hex, int mode, QString filename)
{
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){bass_mode = true;} else {bass_mode = false;};
    Q_UNUSED(mode);
    this->data = patch;
    this->SY1K_default = default_hex;
    this->fileName = filename;
    //convertFromTSL(0);
}

tsl_translation::~tsl_translation()
{

}

void tsl_translation::patchIndex(int listIndex)
{
    Q_UNUSED(listIndex);
    SysxIO *sysxIO = SysxIO::Instance();
    this->index=sysxIO->patchListValue;
}

void tsl_translation::AppendTSL(QByteArray hex, const char* Json_name)
{
    bool ok;
    char aq = hex.at(0);
    QString val = QString::number(aq, 16).toUpper();
    if(hex.size()>1){ val = QString::number(aq*128, 16).toUpper(); };
    char cq = hex.at(1);
    QString val2 = QString::number(cq, 16).toUpper();
    int value;
    if(hex.size()>1){ value = ((val2).toInt(&ok, 16)+(val).toInt(&ok, 16)); }
    else{ value = val.toInt(&ok, 16); };
    QByteArray name(Json_name);
    int start_index = this->TSL_default.indexOf(char(34)+name+char(34))+(name.size()+3); //find pointer to start of Json value.
    QByteArray bq(":");
    unsigned int incr = 1;
LOOP:
    incr--;
    if((this->TSL_default.mid((start_index-1), 1).contains(bq)) && (this->TSL_default.mid((start_index-name.size()-3), 1 ).contains(char(34))))
    {
        int end_index = this->TSL_default.indexOf(",", start_index)-start_index; //find pointer to end of value to get the size of the value.
        if(this->TSL_default.at(start_index+end_index-1)==char(125)) { end_index=end_index-1; }; // if find a } then back 2 steps
        QByteArray vq;
        vq.setNum(value);
        this->TSL_default.replace(start_index, end_index, vq);
        incr = 0;
    }
    else if (incr>0)
    {
        start_index = this->TSL_default.indexOf(char(34)+name+char(34), start_index)+(name.size()+3);
        goto LOOP;
    };
}

void tsl_translation::TextTSL(QByteArray hex, const char* Json_name)
{
    QByteArray name(Json_name);   // name of function to be searched for
    int start_index = this->TSL_default.indexOf(char(34)+name+char(34))+name.size()+4;   //find pointer to start of Json value after :".
    QByteArray b(":");
    int incr = 1;
    if(name == "name"){ incr = 2; }; // name listed twice.
    QByteArray null("null");
LOOP2:
    incr--;
    if(this->TSL_default.mid(start_index-2, 1) != b)  // check if correct name is found - a ": should be after the name.
    {
        start_index = this->TSL_default.indexOf(char(34)+name+char(34), start_index+name.size())+2;   // if not correct, move to the next name instance
    };
    if(this->TSL_default.mid(start_index-2, 1) == b)    // if name": is a match - find end of string field ",
    {

        int end_index = this->TSL_default.indexOf(",", start_index)-start_index-1; //find pointer to end of value to get the size of the value.
        if(name=="name"){end_index = this->TSL_default.indexOf(char(125), start_index)-start_index-1;}; // "name": only uses } after and not ,

        if(this->TSL_default.at(start_index+end_index-1)==char(125)) { end_index=end_index-1; }; // if find a } then back 2 steps.
        if(this->TSL_default.at(start_index+end_index-1)==char(93)) { end_index=end_index-2; }; // if find a ] then back 3 steps.

        if(this->TSL_default.mid(start_index-1, 4).contains(null) && hex.simplified().isEmpty()) //if no text and file contains null - do nothing.
        {

        }
        else if(hex.simplified().isEmpty()) // if no text and file probably has existing quoted text.
        {
            this->TSL_default.replace(start_index-1, end_index+1, "null");   //replace the old string with null.
        }
        else if(this->TSL_default.mid(start_index-1, 4).contains(null) && !hex.isEmpty())
        {
            this->TSL_default.replace(start_index-1, end_index+2, (char(34)+hex+char(34)));   //replace 'null' with the new string
        }
        else
        {
            this->TSL_default.replace(start_index, end_index, hex);   //replace the old string with the new string
        };
    };
    start_index = this->TSL_default.lastIndexOf(char(34)+name+char(34))+name.size()+4;
    if (incr>0) {goto LOOP2; };
}

int tsl_translation::ArrayToInt(QByteArray Array)
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

QByteArray tsl_translation::IntToArray(int value)
{
    QByteArray Array;
    int msb = value/128;
    int lsb = value-(msb*128);
    Array.append(char(msb));
    Array.append(char(lsb));
    return Array;                                  // return two byte QByteArray
}

QByteArray tsl_translation::GetJsonArray(QByteArray text, int pos )
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

QByteArray tsl_translation::GetJsonValue(QByteArray text, int pos )
{
    QByteArray str((char(34)+text+char(34)));
    int start_index = data.indexOf(str, pos)+(text.size()+3); //find pointer to start of Json value.
    int end_index = data.indexOf(",", start_index)-start_index;                  //find pointer to end of value to get the size of the value.
    QByteArray xq = data.mid(start_index , end_index );                 //copy the Json value and return as a QByteArray.
    //if(x.at(0)==(char)34) {x.remove(0, 1); };
    //if(x.contains((char)34)) {x.truncate(x.indexOf((char)34)); };
    return xq;
}

QByteArray tsl_translation::GetJsonHex(QByteArray text, int pos )
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

QByteArray tsl_translation::GetJsonString(QByteArray text, int pos )
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

void tsl_translation::SY1K_hex_replace(int pos, int size, QByteArray value)
{
    if(!value.isEmpty() && !value.isNull())
    {
        this->SY1K_default.replace(pos, size, value);
    };
}

QByteArray tsl_translation::convert_QByteArray_value(QByteArray value, QString hex3)
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
    if(value2.size()>1){value2.remove(0, 1); };
    //nv = value2.toHex().toInt(&ok, 16);
    return value2;
}

QByteArray tsl_translation::get_chain(int value)
{
    QByteArray split =  QByteArrayLiteral("\x18");
    QByteArray fx1 =    QByteArrayLiteral("\x04\x0A");
    QByteArray fx2 =    QByteArrayLiteral("\x05\x0B");
    QByteArray fx3 =    QByteArrayLiteral("\x06");
    QByteArray fx4 =    QByteArrayLiteral("\x00\x07\x01\x02\x03\x09\x0C\x0D");
    QByteArray subout = QByteArrayLiteral("\x11\x14\x15\x12\x13");
    QByteArray retmix = QByteArrayLiteral("\x10\x0E\x08\x0F");
    QByteArray farend = QByteArrayLiteral("\x16\x17");
    QByteArray chain;
    if(value==0){chain = split;};
    if(value==1){chain = fx1;};
    if(value==2){chain = fx2;};
    if(value==3){chain = fx3;};
    if(value==4){chain = fx4;};
    if(value==5){chain = subout;};
    if(value==6){chain = retmix;};
    if(value==7){chain = farend;};
    return chain;
}

QByteArray tsl_translation::convert_Assign(const char* Json_name)
{
    MidiTable *midiTable = MidiTable::Instance();
    bool ok;
    int assign = GetJsonHex(Json_name, a).toHex().toUpper().toInt(&ok, 16);
    //QString assign_before(QString::number(assign, 10));
    int maxRange = 256;
    int dif = assign/maxRange;
    QString valueHex1 = QString::number(dif, 16).toUpper();
    if(valueHex1.length() < 2) valueHex1.prepend("0");
    QString valueHex2 = QString::number(assign - (dif * maxRange), 16).toUpper();
    if(valueHex2.length() < 2) valueHex2.prepend("0");
    QString hex4 = valueHex1;
    QString hex5 = valueHex2;                                     //convert valueStr to 7bit hex4, hex5
    Midi items = midiTable->getMidiMap("Tables", "00", "01", "1D", hex4, hex5);
    QString assign_after;
    if(bass_mode == true){assign_after = items.customdesc;}else{assign_after = items.desc;};
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

    return new_value;
}

void tsl_translation::write_Assign(const char* ctrl, const char* num)
{
    bool ok;
    QByteArray CTRL =ctrl;
    QByteArray NUM = num;
    QString source = ctrl;
    QByteArray SRC = QByteArrayLiteral("\x00");
    if(QString::number(GetJsonHex(("patchCTL"+CTRL+"AssignSwitch"+NUM), a).at(0), 16).toInt(&ok, 16)>0 && incr<16)
    {
        if(source.contains("CTL1"))  {SRC = QByteArrayLiteral("\x07");};
        if(source.contains("CTL2"))  {SRC = QByteArrayLiteral("\x08");};
        if(source.contains("CTL3"))  {SRC = QByteArrayLiteral("\x09");};
        if(source.contains("CTL4"))  {SRC = QByteArrayLiteral("\x0A");};
        if(source.contains("CTL5"))  {SRC = QByteArrayLiteral("\x0B");};
        if(source.contains("EXP"))   {SRC = QByteArrayLiteral("\x0D");};
        if(source.contains("MIDICC")){SRC = QByteArrayLiteral("\x1B");};
        if(source.contains("WAVE"))  {SRC = QByteArrayLiteral("\x12");};
        SY1K_hex_replace(x+0, 1, QByteArrayLiteral("\x01"));
        SY1K_hex_replace(x+1, 4, convert_Assign("patchCTL"+CTRL+"AssignTarget"+NUM));
        SY1K_hex_replace(x+5, 1, GetJsonHex("patchCTL"+CTRL+"AssignTargetMin"+NUM+"_l", a));
        SY1K_hex_replace(x+6, 1, GetJsonHex("patchCTL"+CTRL+"AssignTargetMin"+NUM+"_m1", a));
        SY1K_hex_replace(x+7, 1, GetJsonHex("patchCTL"+CTRL+"AssignTargetMin"+NUM+"_m2", a));
        SY1K_hex_replace(x+8, 1, GetJsonHex("patchCTL"+CTRL+"AssignTargetMin"+NUM+"_h", a));
        SY1K_hex_replace(x+9, 1, GetJsonHex("patchCTL"+CTRL+"AssignTargetMax"+NUM+"_l", a));
        SY1K_hex_replace(x+10, 1, GetJsonHex("patchCTL"+CTRL+"AssignTargetMax"+NUM+"_m1", a));
        SY1K_hex_replace(x+11, 1, GetJsonHex("patchCTL"+CTRL+"AssignTargetMax"+NUM+"_m2", a));
        SY1K_hex_replace(x+12, 1, GetJsonHex("patchCTL"+CTRL+"AssignTargetMax"+NUM+"_h", a));
        SY1K_hex_replace(x+13, 1, SRC);
        SY1K_hex_replace(x+14, 1, GetJsonHex("patchCTL"+CTRL+"SourceMode"+NUM, a));
        SY1K_hex_replace(x+15, 4,convert_to_4Byte("patchCTL"+CTRL+"AssignSourceRangeMin"+NUM, 128));
        SY1K_hex_replace(x+19, 4,convert_to_4Byte("patchCTL"+CTRL+"AssignSourceRangeMax"+NUM, 128));
        SY1K_hex_replace(x+23, 1, GetJsonHex("patchCTL"+CTRL+"LegatoRiseTime"+NUM, a));
        SY1K_hex_replace(x+24, 1, GetJsonHex("patchCTL"+CTRL+"LegatoFallTime"+NUM, a));
        SY1K_hex_replace(x+24, 1, GetJsonHex("patchCTL"+CTRL+"PatchSelectNumber"+NUM, a));
        if(source.contains("WAVE")){
            SY1K_hex_replace(x+25, 1, GetJsonHex("patchCTL"+CTRL+"PedalRate"+NUM, a));
            SY1K_hex_replace(x+26, 1, GetJsonHex("patchCTL"+CTRL+"PedalWaveForm"+NUM, a));
        };
        incr++;
        if(incr%2){x+=64;}else{x+=57;};  //odd assigns increment 64 bytes, even assigns increment 57 bytes.
    };
}

QByteArray tsl_translation::convert_to_4Byte(const char* Json_name, int multiplier)
{
    bool ok;
    int assign = QString::number(GetJsonHex(Json_name, a).at(0), 16).toInt(&ok, 16)*multiplier;

    QString index = QString::number(assign, 16);
    if(index.size()<2){index.prepend("000");};
    if(index.size()<3){index.prepend("00");};
    if(index.size()<4){index.prepend("0");};

    QString n3(index.at(3)); int b3 = n3.toInt(&ok, 16);
    QString n2(index.at(2)); int b2 = n2.toInt(&ok, 16);
    QString n1(index.at(1)); int b1 = n1.toInt(&ok, 16);
    QString n0(index.at(0)); int b0 = n0.toInt(&ok, 16);

    QByteArray new_value;
    new_value.append(char(b0)).append(char(b1)).append(char(b2)).append(char(b3));

    return new_value;
}

QByteArray tsl_translation::convertFromTSL(int patch)
{
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
            patchText.append(GetJsonValue("patchname", a).trimmed().remove(0, 1) ); patchText.chop(1);

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
    if(device.contains("SY-300"))
    {
        temp.append(char(GetJsonValue("patchCommonPatchName1", a).toInt()) );  //copy patch name
        temp.append(char(GetJsonValue("patchCommonPatchName2", a).toInt()) );  //copy patch name
        temp.append(char(GetJsonValue("patchCommonPatchName3", a).toInt()) );  //copy patch name
        temp.append(char(GetJsonValue("patchCommonPatchName4", a).toInt()) );  //copy patch name
        temp.append(char(GetJsonValue("patchCommonPatchName5", a).toInt()) );  //copy patch name
        temp.append(char(GetJsonValue("patchCommonPatchName6", a).toInt()) );  //copy patch name
        temp.append(char(GetJsonValue("patchCommonPatchName7", a).toInt()) );  //copy patch name
        temp.append(char(GetJsonValue("patchCommonPatchName8", a).toInt()) );  //copy patch name
        temp.append(char(GetJsonValue("patchCommonPatchName9", a).toInt()) );  //copy patch name
        temp.append(char(GetJsonValue("patchCommonPatchName10", a).toInt()) ); //copy patch name
        temp.append(char(GetJsonValue("patchCommonPatchName11", a).toInt()) ); //copy patch name
        temp.append(char(GetJsonValue("patchCommonPatchName12", a).toInt()) ); //copy patch name
        temp.append(char(GetJsonValue("patchCommonPatchName13", a).toInt()) ); //copy patch name
        temp.append(char(GetJsonValue("patchCommonPatchName14", a).toInt()) ); //copy patch name
        temp.append(char(GetJsonValue("patchCommonPatchName15", a).toInt()) ); //copy patch name
        temp.append(char(GetJsonValue("patchCommonPatchName16", a).toInt()) ); //copy patch name
        SY1K_hex_replace(12, 16, temp );                        //copy patch name

        QByteArray t;
        x = 1220;

        //SY1K_hex_replace(x+51, 1, GetJsonHex("patchCommonReturnLevel", a));
        SY1K_hex_replace(x+53, 1, GetJsonHex("patchCommonReturnSignalPhase", a));

        t = QByteArrayLiteral("\x01");
        SY1K_hex_replace(x+53, 1, t);  // set S/R Loop to Direct Mix
        SY1K_hex_replace(x+49, 1, t);  // set S/R Loop On
        SY1K_hex_replace(x+1, 1, t);   // set Normal in on
        SY1K_hex_replace(x+40, 1, GetJsonHex("patchCommonOSCMixerBalance", a));
        SY1K_hex_replace(x+41, 1, GetJsonHex("patchCommonOSCMixerCHAPan", a));
        SY1K_hex_replace(x+42, 1, GetJsonHex("patchCommonOSCMixerCHBPan", a));
        SY1K_hex_replace(x+43, 1, GetJsonHex("patchCommonOUTPUTMixerBalance", a));
        SY1K_hex_replace(x+44, 1, GetJsonHex("patchCommonOUTPUTMixerSynthPan", a));
        SY1K_hex_replace(x+45, 1, GetJsonHex("patchCommonOUTPUTMixerReturnPan", a));

        bool ok;
        int oscAssign = QString::number(GetJsonHex("patchCommonOSCAssign", a).at(0), 16).toInt(&ok, 16);
        QByteArray box;
        if(oscAssign == 0){box= QByteArrayLiteral("\x01\x02\x03\x00");};
        if(oscAssign == 1){box= QByteArrayLiteral("\x01\x03\x02\x00");};
        if(oscAssign == 2){box= QByteArrayLiteral("\x02\x03\x01\x00");};
        if(oscAssign == 4){box= QByteArrayLiteral("\x02\x01\x03\x00");};
        if(oscAssign == 5){box= QByteArrayLiteral("\x03\x02\x01\x00");};
        if(oscAssign >= 6){box= QByteArrayLiteral("\x03\x01\x02\x00");};
        SY1K_hex_replace(x+69, 4, box);

        int patchChain1 = QString::number(GetJsonHex("patchChain1", a).at(0), 16).toInt(&ok, 16);
        int patchChain2 = QString::number(GetJsonHex("patchChain2", a).at(0), 16).toInt(&ok, 16);
        int patchChain3 = QString::number(GetJsonHex("patchChain3", a).at(0), 16).toInt(&ok, 16);
        int patchChain4 = QString::number(GetJsonHex("patchChain4", a).at(0), 16).toInt(&ok, 16);
        int patchChain5 = QString::number(GetJsonHex("patchChain5", a).at(0), 16).toInt(&ok, 16);
        int patchChain6 = QString::number(GetJsonHex("patchChain6", a).at(0), 16).toInt(&ok, 16);
        int patchChain7 = QString::number(GetJsonHex("patchChain7", a).at(0), 16).toInt(&ok, 16);
        int patchChain8 = QString::number(GetJsonHex("patchChain8", a).at(0), 16).toInt(&ok, 16);
        int patchChain9 = QString::number(GetJsonHex("patchChain9", a).at(0), 16).toInt(&ok, 16);
        int patchChain10 = QString::number(GetJsonHex("patchChain10", a).at(0), 16).toInt(&ok, 16);
        int patchChain11 = QString::number(GetJsonHex("patchChain11", a).at(0), 16).toInt(&ok, 16);
        int patchChain12 = QString::number(GetJsonHex("patchChain12", a).at(0), 16).toInt(&ok, 16);

        QByteArray chain;
        //QString fx;
        chain.append(get_chain(patchChain1));
        chain.append(get_chain(patchChain2));
        chain.append(get_chain(patchChain3));
        chain.append(get_chain(patchChain4));
        chain.append(get_chain(patchChain5));
        chain.append(get_chain(patchChain6));
        chain.append(get_chain(patchChain7));
        chain.append(get_chain(patchChain8));
        chain.append(get_chain(6)); // retmix
        chain.append(get_chain(patchChain9));
        chain.append(get_chain(patchChain10));
        chain.append(get_chain(patchChain11));
        chain.append(get_chain(patchChain12));
        chain.append(get_chain(7)); // farend
        if(chain.size()>30) chain.remove(chain.lastIndexOf(QByteArrayLiteral("\x18")),1);
        SY1K_hex_replace(x+73, 30, chain);
        /* chain items:
          04=fx1 18=bal1 05=fx2 18=bal1 10=S/R 18=bal2 06=fx3 18=bal2 07=eq1 0E=fv1 0A=dd1 18=bal3
          00=cmp 01=ds 02=amp 08=eq2 03=ns 09=cho 0F=fv2 0B=dd2 18=bal3 0C=dd3 0D=rev 11=div 12= div_path_B 13=mix
          14=sub/l 15=sub/r 16=main/l 17=main/r

        patchCommonSubOutputLevel	50
        patchCommonTempo	120
        patchCommonTempo_h	8
        patchCommonTempo_l	7
        patchCommonDirectLevel	100
        patchCommonInputSensPatchorSYSTEM	1
        patchCommonLevel	50
        patchCommonOSCAssign	0
        patchCommonOSCMixerCHALevel
        patchCommonOSCMixerCHALevel
        patchCommonOSCMixerCHBLevel
        patchCommonOUTPUTMixerReturnLevel
        patchCommonOUTPUTMixerSynthLevel
        patchCommonPatchHighBalance	50
        patchCommonPatchInputBassMode	0
        patchCommonPatchInputCompAttack	50
        patchCommonPatchInputCompLevel	50
        patchCommonPatchInputCompSustain	50
        patchCommonPatchInputCompSw	0
        patchCommonPatchInputSens	50
        patchCommonPatchLowBalance	50        */

        x = 1358;
        t = QByteArrayLiteral("\x00");
        SY1K_hex_replace(x+0, 1, GetJsonHex("patchOsc1OSCSW", a));
        SY1K_hex_replace(x+1, 1, convert_QByteArray_value(t, "02")); // switch to dynasynth
        SY1K_hex_replace(x+2, 1, GetJsonHex("patchOsc1AMPLevel", a));    // inst level
        t.clear();
        t.append(GetJsonHex("patchOsc1AMPPan", a));
        SY1K_hex_replace(x+12, 1, t);     // str pan x 6
        SY1K_hex_replace(x+13, 1, t);
        SY1K_hex_replace(x+14, 1, t);
        SY1K_hex_replace(x+15, 1, t);
        SY1K_hex_replace(x+16, 1, t);
        SY1K_hex_replace(x+17, 1, t);
        x = 1390;
        SY1K_hex_replace(x+4, 1, convert_QByteArray_value(GetJsonHex("patchOsc1Waveform", a), "10"));
        SY1K_hex_replace(x+5, 1, convert_QByteArray_value(GetJsonHex("patchOsc1Pitch", a), "11"));
        SY1K_hex_replace(x+6, 1, convert_QByteArray_value(GetJsonHex("patchOsc1PitchFine", a), "12"));
        SY1K_hex_replace(x+7, 1, GetJsonHex("patchOsc1PulseWidth", a));
        SY1K_hex_replace(x+8, 1, convert_QByteArray_value(GetJsonHex("patchOsc1PWMEnvAttack", a), "12"));
        SY1K_hex_replace(x+9, 1, convert_QByteArray_value(GetJsonHex("patchOsc1PWMEnvDepth", a), "12"));
        SY1K_hex_replace(x+10, 1, convert_QByteArray_value(GetJsonHex("patchOsc1Detune", a), "12"));
        SY1K_hex_replace(x+11, 1, GetJsonHex("patchOsc1Sharpness", a));
        SY1K_hex_replace(x+15, 1, convert_QByteArray_value(GetJsonHex("patchOsc1PitchEnvAttack", a), "12"));
        SY1K_hex_replace(x+16, 1, convert_QByteArray_value(GetJsonHex("patchOsc1PitchEnvDepth", a), "12"));
        SY1K_hex_replace(x+17, 1, convert_QByteArray_value(GetJsonHex("patchOsc1PitchBendDepth", a), "11"));
        SY1K_hex_replace(x+18, 1, GetJsonHex("patchOsc1PitchBendControl", a));
        SY1K_hex_replace(x+29, 1, convert_QByteArray_value(GetJsonHex("patchOsc1FilterType", a), "13"));
        SY1K_hex_replace(x+30, 1, convert_QByteArray_value(GetJsonHex("patchOsc1FilterSlope", a), "14"));
        SY1K_hex_replace(x+31, 1, GetJsonHex("patchOsc1FilterCutoff", a));
        SY1K_hex_replace(x+32, 1, GetJsonHex("patchOsc1FilterResonance", a));
        SY1K_hex_replace(x+33, 1, convert_QByteArray_value(GetJsonHex("patchOsc1FilterEnvAttack", a), "12"));
        SY1K_hex_replace(x+34, 1, convert_QByteArray_value(GetJsonHex("patchOsc1FilterEnvDepth", a), "12"));
        SY1K_hex_replace(x+35, 1, convert_QByteArray_value(GetJsonHex("patchOsc1AMPEnvAttack", a), "12"));
        SY1K_hex_replace(x+39, 1, GetJsonHex("patchOsc1LFOOffOn1", a));
        SY1K_hex_replace(x+40, 1, GetJsonHex("patchOsc1LFOWaveform1", a));
        SY1K_hex_replace(x+41, 1, GetJsonHex("patchOsc1LFORate1", a));
        SY1K_hex_replace(x+42, 1, GetJsonHex("patchOsc1LFODynamicDepth1", a));
        SY1K_hex_replace(x+43, 1, GetJsonHex("patchOsc1LFOPitchDepth1", a));
        SY1K_hex_replace(x+44, 1, GetJsonHex("patchOsc1LFOFilterDepth1", a));
        SY1K_hex_replace(x+45, 1, GetJsonHex("patchOsc1LFOAmpDepth1", a));
        SY1K_hex_replace(x+46, 1, GetJsonHex("patchOsc1LFOPWMDepth1", a));
        SY1K_hex_replace(x+47, 1, GetJsonHex("patchOsc1LFOFadeTime1", a));
        SY1K_hex_replace(x+49, 1, GetJsonHex("patchOsc1LFOOffOn2", a));
        SY1K_hex_replace(x+50, 1, GetJsonHex("patchOsc1LFOWaveform2", a));
        SY1K_hex_replace(x+51, 1, GetJsonHex("patchOsc1LFORate2", a));
        SY1K_hex_replace(x+52, 1, GetJsonHex("patchOsc1LFODynamicDepth2", a));
        SY1K_hex_replace(x+53, 1, GetJsonHex("patchOsc1LFOPitchDepth2", a));
        SY1K_hex_replace(x+54, 1, GetJsonHex("patchOsc1LFOFilterDepth2", a));
        SY1K_hex_replace(x+55, 1, GetJsonHex("patchOsc1LFOAmpDepth2", a));
        SY1K_hex_replace(x+56, 1, GetJsonHex("patchOsc1LFOPWMDepth2", a));
        SY1K_hex_replace(x+57, 1, GetJsonHex("patchOsc1LFOFadeTime2", a));
        SY1K_hex_replace(x+63, 1, convert_QByteArray_value(GetJsonHex("patchOsc1SEQStep1", a), "11"));
        SY1K_hex_replace(x+65, 1, convert_QByteArray_value(GetJsonHex("patchOsc1SEQStep2", a), "11"));
        SY1K_hex_replace(x+67, 1, convert_QByteArray_value(GetJsonHex("patchOsc1SEQStep3", a), "11"));
        SY1K_hex_replace(x+69, 1, convert_QByteArray_value(GetJsonHex("patchOsc1SEQStep4", a), "11"));
        SY1K_hex_replace(x+71, 1, convert_QByteArray_value(GetJsonHex("patchOsc1SEQStep5", a), "11"));
        SY1K_hex_replace(x+73, 1, convert_QByteArray_value(GetJsonHex("patchOsc1SEQStep6", a), "11"));
        SY1K_hex_replace(x+75, 1, convert_QByteArray_value(GetJsonHex("patchOsc1SEQStep7", a), "11"));
        SY1K_hex_replace(x+77, 1, convert_QByteArray_value(GetJsonHex("patchOsc1SEQStep8", a), "11"));
        SY1K_hex_replace(x+79, 1, convert_QByteArray_value(GetJsonHex("patchOsc1SEQStep9", a), "11"));
        SY1K_hex_replace(x+81, 1, convert_QByteArray_value(GetJsonHex("patchOsc1SEQStep10", a), "11"));
        SY1K_hex_replace(x+83, 1, convert_QByteArray_value(GetJsonHex("patchOsc1SEQStep11", a), "11"));
        SY1K_hex_replace(x+85, 1, convert_QByteArray_value(GetJsonHex("patchOsc1SEQStep12", a), "11"));
        SY1K_hex_replace(x+87, 1, convert_QByteArray_value(GetJsonHex("patchOsc1SEQStep13", a), "11"));
        SY1K_hex_replace(x+89, 1, convert_QByteArray_value(GetJsonHex("patchOsc1SEQStep14", a), "11"));
        SY1K_hex_replace(x+91, 1, convert_QByteArray_value(GetJsonHex("patchOsc1SEQStep15", a), "11"));
        SY1K_hex_replace(x+93, 1, convert_QByteArray_value(GetJsonHex("patchOsc1SEQStep16", a), "11"));
        t.clear();
        t.append(GetJsonHex("patchOsc1SEQDepth", a));
        SY1K_hex_replace(x+127, 1, t);
        x = 1532; // start of dynasynth part 2
        SY1K_hex_replace(x+1, 1, t);
        SY1K_hex_replace(x+3, 1, t);
        SY1K_hex_replace(x+5, 1, t);
        SY1K_hex_replace(x+7, 1, t);
        SY1K_hex_replace(x+9, 1, t);
        SY1K_hex_replace(x+11, 1, t);
        SY1K_hex_replace(x+13, 1, t);
        SY1K_hex_replace(x+15, 1, t);
        SY1K_hex_replace(x+17, 1, t);
        SY1K_hex_replace(x+19, 1, t);
        SY1K_hex_replace(x+21, 1, t);
        SY1K_hex_replace(x+23, 1, t);
        SY1K_hex_replace(x+25, 1, t);
        SY1K_hex_replace(x+27, 1, t);
        SY1K_hex_replace(x+29, 1, t);
        SY1K_hex_replace(x+30, 1, GetJsonHex("patchOsc1SEQSw", a));
        SY1K_hex_replace(x+32, 1, GetJsonHex("patchOsc1SEQLoopLength", a));
        SY1K_hex_replace(x+33, 1, GetJsonHex("patchOsc1SEQRate", a));
        SY1K_hex_replace(x+74, 1, convert_QByteArray_value(GetJsonHex("patchOsc1InputRangeLowerFade", a), "16"));
        SY1K_hex_replace(x+75, 1, convert_QByteArray_value(GetJsonHex("patchOsc1InputRangeLower", a), "15"));
        SY1K_hex_replace(x+76, 1, convert_QByteArray_value(GetJsonHex("patchOsc1InputRangeUpper", a), "15"));
        SY1K_hex_replace(x+77, 1, convert_QByteArray_value(GetJsonHex("patchOsc1InputRangeUpperFade", a), "16"));

        if(bass_mode == true){x = 2020;} else {x = 2064;};
        t.clear();
        t = QByteArrayLiteral("\x00");
        SY1K_hex_replace(x+0, 1, GetJsonHex("patchOsc2OSCSW", a));
        SY1K_hex_replace(x+1, 1, convert_QByteArray_value(t, "02")); // switch to dynasynth
        SY1K_hex_replace(x+2, 1, GetJsonHex("patchOsc2AMPLevel", a));    // inst level
        SY1K_hex_replace(x+12, 1, GetJsonHex("patchOsc2AMPPan", a));     // str pan x 6
        SY1K_hex_replace(x+13, 1, GetJsonHex("patchOsc2AMPPan", a));
        SY1K_hex_replace(x+14, 1, GetJsonHex("patchOsc2AMPPan", a));
        SY1K_hex_replace(x+15, 1, GetJsonHex("patchOsc2AMPPan", a));
        SY1K_hex_replace(x+16, 1, GetJsonHex("patchOsc2AMPPan", a));
        SY1K_hex_replace(x+17, 1, GetJsonHex("patchOsc2AMPPan", a));
        if(bass_mode == true){x = 2052;} else {x = 2096;};
        SY1K_hex_replace(x+4, 1, convert_QByteArray_value(GetJsonHex("patchOsc2Waveform", a), "10"));
        SY1K_hex_replace(x+5, 1, convert_QByteArray_value(GetJsonHex("patchOsc2Pitch", a), "11"));
        SY1K_hex_replace(x+6, 1, convert_QByteArray_value(GetJsonHex("patchOsc2PitchFine", a), "12"));
        SY1K_hex_replace(x+7, 1, GetJsonHex("patchOsc2PulseWidth", a));
        SY1K_hex_replace(x+8, 1, convert_QByteArray_value(GetJsonHex("patchOsc2PWMEnvAttack", a), "12"));
        SY1K_hex_replace(x+9, 1, convert_QByteArray_value(GetJsonHex("patchOsc2PWMEnvDepth", a), "12"));
        SY1K_hex_replace(x+10, 1, convert_QByteArray_value(GetJsonHex("patchOsc2Detune", a), "12"));
        SY1K_hex_replace(x+11, 1, GetJsonHex("patchOsc2Sharpness", a));
        SY1K_hex_replace(x+15, 1, convert_QByteArray_value(GetJsonHex("patchOsc2PitchEnvAttack", a), "12"));
        SY1K_hex_replace(x+16, 1, convert_QByteArray_value(GetJsonHex("patchOsc2PitchEnvDepth", a), "12"));
        SY1K_hex_replace(x+17, 1, convert_QByteArray_value(GetJsonHex("patchOsc2PitchBendDepth", a), "11"));
        SY1K_hex_replace(x+18, 1, GetJsonHex("patchOsc2PitchBendControl", a));
        SY1K_hex_replace(x+29, 1, convert_QByteArray_value(GetJsonHex("patchOsc2FilterType", a), "13"));
        SY1K_hex_replace(x+30, 1, convert_QByteArray_value(GetJsonHex("patchOsc2FilterSlope", a), "14"));
        SY1K_hex_replace(x+31, 1, GetJsonHex("patchOsc2FilterCutoff", a));
        SY1K_hex_replace(x+32, 1, GetJsonHex("patchOsc2FilterResonance", a));
        SY1K_hex_replace(x+33, 1, convert_QByteArray_value(GetJsonHex("patchOsc2FilterEnvAttack", a), "12"));
        SY1K_hex_replace(x+34, 1, convert_QByteArray_value(GetJsonHex("patchOsc2FilterEnvDepth", a), "12"));
        SY1K_hex_replace(x+35, 1, convert_QByteArray_value(GetJsonHex("patchOsc2AMPEnvAttack", a), "12"));
        SY1K_hex_replace(x+39, 1, GetJsonHex("patchOsc2LFOOffOn1", a));
        SY1K_hex_replace(x+40, 1, GetJsonHex("patchOsc2LFOWaveform1", a));
        SY1K_hex_replace(x+41, 1, GetJsonHex("patchOsc2LFORate1", a));
        SY1K_hex_replace(x+42, 1, GetJsonHex("patchOsc2LFODynamicDepth1", a));
        SY1K_hex_replace(x+43, 1, GetJsonHex("patchOsc2LFOPitchDepth1", a));
        SY1K_hex_replace(x+44, 1, GetJsonHex("patchOsc2LFOFilterDepth1", a));
        SY1K_hex_replace(x+45, 1, GetJsonHex("patchOsc2LFOAmpDepth1", a));
        SY1K_hex_replace(x+46, 1, GetJsonHex("patchOsc2LFOPWMDepth1", a));
        SY1K_hex_replace(x+47, 1, GetJsonHex("patchOsc2LFOFadeTime1", a));
        SY1K_hex_replace(x+49, 1, GetJsonHex("patchOsc2LFOOffOn2", a));
        SY1K_hex_replace(x+50, 1, GetJsonHex("patchOsc2LFOWaveform2", a));
        SY1K_hex_replace(x+51, 1, GetJsonHex("patchOsc2LFORate2", a));
        SY1K_hex_replace(x+52, 1, GetJsonHex("patchOsc2LFODynamicDepth2", a));
        SY1K_hex_replace(x+53, 1, GetJsonHex("patchOsc2LFOPitchDepth2", a));
        SY1K_hex_replace(x+54, 1, GetJsonHex("patchOsc2LFOFilterDepth2", a));
        SY1K_hex_replace(x+55, 1, GetJsonHex("patchOsc2LFOAmpDepth2", a));
        SY1K_hex_replace(x+56, 1, GetJsonHex("patchOsc2LFOPWMDepth2", a));
        SY1K_hex_replace(x+57, 1, GetJsonHex("patchOsc2LFOFadeTime2", a));
        SY1K_hex_replace(x+63, 1, convert_QByteArray_value(GetJsonHex("patchOsc2SEQStep1", a), "11"));
        SY1K_hex_replace(x+65, 1, convert_QByteArray_value(GetJsonHex("patchOsc2SEQStep2", a), "11"));
        SY1K_hex_replace(x+67, 1, convert_QByteArray_value(GetJsonHex("patchOsc2SEQStep3", a), "11"));
        SY1K_hex_replace(x+69, 1, convert_QByteArray_value(GetJsonHex("patchOsc2SEQStep4", a), "11"));
        SY1K_hex_replace(x+71, 1, convert_QByteArray_value(GetJsonHex("patchOsc2SEQStep5", a), "11"));
        SY1K_hex_replace(x+73, 1, convert_QByteArray_value(GetJsonHex("patchOsc2SEQStep6", a), "11"));
        SY1K_hex_replace(x+75, 1, convert_QByteArray_value(GetJsonHex("patchOsc2SEQStep7", a), "11"));
        SY1K_hex_replace(x+77, 1, convert_QByteArray_value(GetJsonHex("patchOsc2SEQStep8", a), "11"));
        SY1K_hex_replace(x+79, 1, convert_QByteArray_value(GetJsonHex("patchOsc2SEQStep9", a), "11"));
        SY1K_hex_replace(x+81, 1, convert_QByteArray_value(GetJsonHex("patchOsc2SEQStep10", a), "11"));
        SY1K_hex_replace(x+83, 1, convert_QByteArray_value(GetJsonHex("patchOsc2SEQStep11", a), "11"));
        SY1K_hex_replace(x+85, 1, convert_QByteArray_value(GetJsonHex("patchOsc2SEQStep12", a), "11"));
        SY1K_hex_replace(x+87, 1, convert_QByteArray_value(GetJsonHex("patchOsc2SEQStep13", a), "11"));
        SY1K_hex_replace(x+89, 1, convert_QByteArray_value(GetJsonHex("patchOsc2SEQStep14", a), "11"));
        SY1K_hex_replace(x+91, 1, convert_QByteArray_value(GetJsonHex("patchOsc2SEQStep15", a), "11"));
        SY1K_hex_replace(x+93, 1, convert_QByteArray_value(GetJsonHex("patchOsc2SEQStep16", a), "11"));
        t.clear();
        t.append(GetJsonHex("patchOsc2SEQDepth", a));
        SY1K_hex_replace(x+127, 1, t);
        if(bass_mode == true){x = 2194;} else {x = 2238;}; // start of dynasynth part 2
        SY1K_hex_replace(x+1, 1, t);
        SY1K_hex_replace(x+3, 1, t);
        SY1K_hex_replace(x+5, 1, t);
        SY1K_hex_replace(x+7, 1, t);
        SY1K_hex_replace(x+9, 1, t);
        SY1K_hex_replace(x+11, 1, t);
        SY1K_hex_replace(x+13, 1, t);
        SY1K_hex_replace(x+15, 1, t);
        SY1K_hex_replace(x+17, 1, t);
        SY1K_hex_replace(x+19, 1, t);
        SY1K_hex_replace(x+21, 1, t);
        SY1K_hex_replace(x+23, 1, t);
        SY1K_hex_replace(x+25, 1, t);
        SY1K_hex_replace(x+27, 1, t);
        SY1K_hex_replace(x+29, 1, t);
        SY1K_hex_replace(x+30, 1, GetJsonHex("patchOsc2SEQSw", a));
        SY1K_hex_replace(x+32, 1, GetJsonHex("patchOsc2SEQLoopLength", a));
        SY1K_hex_replace(x+33, 1, GetJsonHex("patchOsc2SEQRate", a));
        SY1K_hex_replace(x+74, 1, convert_QByteArray_value(GetJsonHex("patchOsc2InputRangeLowerFade", a), "16"));
        SY1K_hex_replace(x+75, 1, convert_QByteArray_value(GetJsonHex("patchOsc2InputRangeLower", a), "15"));
        SY1K_hex_replace(x+76, 1, convert_QByteArray_value(GetJsonHex("patchOsc2InputRangeUpper", a), "15"));
        SY1K_hex_replace(x+77, 1, convert_QByteArray_value(GetJsonHex("patchOsc2InputRangeUpperFade", a), "16"));
        t.clear();
        t = GetJsonHex("patchOscCommonpatchOsc2SEQLinkOffOn", a);

        if(bass_mode == true){x = 2682;} else {x = 2770;};
        t.clear();
        t = QByteArrayLiteral("\x00");
        SY1K_hex_replace(x+0, 1, GetJsonHex("patchOsc3OSCSW", a));
        this->SY1K_default.replace(x+1, 1, convert_QByteArray_value(t, "02")); // switch to dynasynth
        SY1K_hex_replace(x+2, 1, GetJsonHex("patchOsc3AMPLevel", a));    // inst level
        t.clear();
        t.append(GetJsonHex("patchOsc3AMPPan", a));
        SY1K_hex_replace(x+12, 1, t);     // str pan x 6
        SY1K_hex_replace(x+13, 1, t);
        SY1K_hex_replace(x+14, 1, t);
        SY1K_hex_replace(x+15, 1, t);
        SY1K_hex_replace(x+16, 1, t);
        SY1K_hex_replace(x+17, 1, t);
        if(bass_mode==true){x = 2714;} else {x = 2802;};
        SY1K_hex_replace(x+4, 1, convert_QByteArray_value(GetJsonHex("patchOsc3Waveform", a), "10"));
        SY1K_hex_replace(x+5, 1, convert_QByteArray_value(GetJsonHex("patchOsc3Pitch", a), "11"));
        SY1K_hex_replace(x+6, 1, convert_QByteArray_value(GetJsonHex("patchOsc3PitchFine", a), "12"));
        SY1K_hex_replace(x+7, 1, GetJsonHex("patchOsc3PulseWidth", a));
        SY1K_hex_replace(x+8, 1, convert_QByteArray_value(GetJsonHex("patchOsc3PWMEnvAttack", a), "12"));
        SY1K_hex_replace(x+9, 1, convert_QByteArray_value(GetJsonHex("patchOsc3PWMEnvDepth", a), "12"));
        SY1K_hex_replace(x+10, 1, convert_QByteArray_value(GetJsonHex("patchOsc3Detune", a), "12"));
        SY1K_hex_replace(x+11, 1, GetJsonHex("patchOsc3Sharpness", a));
        SY1K_hex_replace(x+15, 1, convert_QByteArray_value(GetJsonHex("patchOsc3PitchEnvAttack", a), "12"));
        SY1K_hex_replace(x+16, 1, convert_QByteArray_value(GetJsonHex("patchOsc3PitchEnvDepth", a), "12"));
        SY1K_hex_replace(x+17, 1, convert_QByteArray_value(GetJsonHex("patchOsc3PitchBendDepth", a), "11"));
        SY1K_hex_replace(x+18, 1, GetJsonHex("patchOsc3PitchBendControl", a));
        SY1K_hex_replace(x+29, 1, convert_QByteArray_value(GetJsonHex("patchOsc3FilterType", a), "13"));
        SY1K_hex_replace(x+30, 1, convert_QByteArray_value(GetJsonHex("patchOsc3FilterSlope", a), "14"));
        SY1K_hex_replace(x+31, 1, GetJsonHex("patchOsc3FilterCutoff", a));
        SY1K_hex_replace(x+32, 1, GetJsonHex("patchOsc3FilterResonance", a));
        SY1K_hex_replace(x+33, 1, convert_QByteArray_value(GetJsonHex("patchOsc3FilterEnvAttack", a), "12"));
        SY1K_hex_replace(x+34, 1, convert_QByteArray_value(GetJsonHex("patchOsc3FilterEnvDepth", a), "12"));
        SY1K_hex_replace(x+35, 1, convert_QByteArray_value(GetJsonHex("patchOsc3AMPEnvAttack", a), "12"));
        SY1K_hex_replace(x+39, 1, GetJsonHex("patchOsc3LFOOffOn1", a));
        SY1K_hex_replace(x+40, 1, GetJsonHex("patchOsc3LFOWaveform1", a));
        SY1K_hex_replace(x+41, 1, GetJsonHex("patchOsc3LFORate1", a));
        SY1K_hex_replace(x+42, 1, GetJsonHex("patchOsc3LFODynamicDepth1", a));
        SY1K_hex_replace(x+43, 1, GetJsonHex("patchOsc3LFOPitchDepth1", a));
        SY1K_hex_replace(x+44, 1, GetJsonHex("patchOsc3LFOFilterDepth1", a));
        SY1K_hex_replace(x+45, 1, GetJsonHex("patchOsc3LFOAmpDepth1", a));
        SY1K_hex_replace(x+46, 1, GetJsonHex("patchOsc3LFOPWMDepth1", a));
        SY1K_hex_replace(x+47, 1, GetJsonHex("patchOsc3LFOFadeTime1", a));
        SY1K_hex_replace(x+49, 1, GetJsonHex("patchOsc3LFOOffOn2", a));
        SY1K_hex_replace(x+50, 1, GetJsonHex("patchOsc3LFOWaveform2", a));
        SY1K_hex_replace(x+51, 1, GetJsonHex("patchOsc3LFORate2", a));
        SY1K_hex_replace(x+52, 1, GetJsonHex("patchOsc3LFODynamicDepth2", a));
        SY1K_hex_replace(x+53, 1, GetJsonHex("patchOsc3LFOPitchDepth2", a));
        SY1K_hex_replace(x+54, 1, GetJsonHex("patchOsc3LFOFilterDepth2", a));
        SY1K_hex_replace(x+55, 1, GetJsonHex("patchOsc3LFOAmpDepth2", a));
        SY1K_hex_replace(x+56, 1, GetJsonHex("patchOsc3LFOPWMDepth2", a));
        SY1K_hex_replace(x+57, 1, GetJsonHex("patchOsc3LFOFadeTime2", a));
        SY1K_hex_replace(x+63, 1, convert_QByteArray_value(GetJsonHex("patchOsc3SEQStep1", a), "11"));
        SY1K_hex_replace(x+65, 1, convert_QByteArray_value(GetJsonHex("patchOsc3SEQStep2", a), "11"));
        SY1K_hex_replace(x+67, 1, convert_QByteArray_value(GetJsonHex("patchOsc3SEQStep3", a), "11"));
        SY1K_hex_replace(x+69, 1, convert_QByteArray_value(GetJsonHex("patchOsc3SEQStep4", a), "11"));
        SY1K_hex_replace(x+71, 1, convert_QByteArray_value(GetJsonHex("patchOsc3SEQStep5", a), "11"));
        SY1K_hex_replace(x+73, 1, convert_QByteArray_value(GetJsonHex("patchOsc3SEQStep6", a), "11"));
        SY1K_hex_replace(x+75, 1, convert_QByteArray_value(GetJsonHex("patchOsc3SEQStep7", a), "11"));
        SY1K_hex_replace(x+77, 1, convert_QByteArray_value(GetJsonHex("patchOsc3SEQStep8", a), "11"));
        SY1K_hex_replace(x+79, 1, convert_QByteArray_value(GetJsonHex("patchOsc3SEQStep9", a), "11"));
        SY1K_hex_replace(x+81, 1, convert_QByteArray_value(GetJsonHex("patchOsc3SEQStep10", a), "11"));
        SY1K_hex_replace(x+83, 1, convert_QByteArray_value(GetJsonHex("patchOsc3SEQStep11", a), "11"));
        SY1K_hex_replace(x+85, 1, convert_QByteArray_value(GetJsonHex("patchOsc3SEQStep12", a), "11"));
        SY1K_hex_replace(x+87, 1, convert_QByteArray_value(GetJsonHex("patchOsc3SEQStep13", a), "11"));
        SY1K_hex_replace(x+89, 1, convert_QByteArray_value(GetJsonHex("patchOsc3SEQStep14", a), "11"));
        SY1K_hex_replace(x+91, 1, convert_QByteArray_value(GetJsonHex("patchOsc3SEQStep15", a), "11"));
        SY1K_hex_replace(x+93, 1, convert_QByteArray_value(GetJsonHex("patchOsc3SEQStep16", a), "11"));
        t.clear();
        t.append(GetJsonHex("patchOsc3SEQDepth", a));
        SY1K_hex_replace(x+127, 1, t);
        if(bass_mode==true){x = 2856;} else {x = 2944;}; // start of dynasynth part 2
        SY1K_hex_replace(x+1, 1, t);
        SY1K_hex_replace(x+3, 1, t);
        SY1K_hex_replace(x+5, 1, t);
        SY1K_hex_replace(x+7, 1, t);
        SY1K_hex_replace(x+9, 1, t);
        SY1K_hex_replace(x+11, 1, t);
        SY1K_hex_replace(x+13, 1, t);
        SY1K_hex_replace(x+15, 1, t);
        SY1K_hex_replace(x+17, 1, t);
        SY1K_hex_replace(x+19, 1, t);
        SY1K_hex_replace(x+21, 1, t);
        SY1K_hex_replace(x+23, 1, t);
        SY1K_hex_replace(x+25, 1, t);
        SY1K_hex_replace(x+27, 1, t);
        SY1K_hex_replace(x+29, 1, t);
        SY1K_hex_replace(x+30, 1, GetJsonHex("patchOsc3SEQSw", a));
        SY1K_hex_replace(x+32, 1, GetJsonHex("patchOsc3SEQLoopLength", a));
        SY1K_hex_replace(x+33, 1, GetJsonHex("patchOsc3SEQRate", a));
        SY1K_hex_replace(x+74, 1, convert_QByteArray_value(GetJsonHex("patchOsc3InputRangeLowerFade", a), "16"));
        SY1K_hex_replace(x+75, 1, convert_QByteArray_value(GetJsonHex("patchOsc3InputRangeLower", a), "15"));
        SY1K_hex_replace(x+76, 1, convert_QByteArray_value(GetJsonHex("patchOsc3InputRangeUpper", a), "15"));
        SY1K_hex_replace(x+77, 1, convert_QByteArray_value(GetJsonHex("patchOsc3InputRangeUpperFade", a), "16"));

     /* patchOscCommonOSCRing2	0
        patchOscCommonOSCRing3	1
        patchOscCommonOSCSync2	0
        patchOscCommonOSCSync3	2
        patchOscCommonpatchOsc3SEQLinkOffOn	0 */

        /*  FX fxtype
        "00"="CHORUS"
        "01"="CHO + DLY"
        "02"="CHO + REV"
        "03"="COMPRESSOR"
        "04"="DELAY"
        "05"="DLY + REV"
        "06"="EQ"
        "07"="FLANGER"
        "08"="ISOLATOR"
        "09"="LIMITER"
        "0A"="LO-FI"
        "0B"="ODDS"
        "0C"="PANNER"
        "0D"="PHASER"
        "0E"="REVERB"
        "0F"="ROTARY"
        "10"="SLICER"
        "11"="SLOW GEAR"
        "12"="TOUCH WAH"
        "13"="TREMOLO"
        "14"="UNIVIBE"*/

        t = QByteArrayLiteral("\x00");

        int amp;
        if(bass_mode==true){amp = 3388;} else {amp = 3520;}; // start of preamp
        SY1K_hex_replace(amp, 1, t); // switch off preamp block

        int dd1;
        if(bass_mode==true){dd1 = 3492;} else {dd1 = 3624;}; // start of Delay1
        SY1K_hex_replace(dd1, 1, t);  // switch off DD1 block

        int dd2;
        if(bass_mode==true){dd2 = 3516;} else {dd2 = 3648;}; // start of Delay2
        SY1K_hex_replace(dd2, 1, t);  // switch off DD2 block

        if(bass_mode==true){x = 3540;} else {x = 3672;}; // start of MstDelay block
        SY1K_hex_replace(x, 1, t);  // switch off MstDelay block

        if(bass_mode==true){x = 3587;} else {x = 3719;}; // start of chorus block
        SY1K_hex_replace(x, 1, t);  // switch off chorus block

        if(bass_mode==true){x = 6254;} else {x = 6386;}; // start of reverb block
        SY1K_hex_replace(x, 1, t);  // switch off reverb block

        if(bass_mode==true){x = 3344;} else {x = 3476;}; // start of comp block
        SY1K_hex_replace(x, 1, t);  // switch off comp block

        if(bass_mode==true){x = 3440;} else {x = 3572;}; // start of EQ block
        SY1K_hex_replace(x, 1, t);  // switch off EQ block

        if(bass_mode==true){x = 3365;} else {x = 3476;}; // start of ODDS block
        SY1K_hex_replace(x, 1, t); // switch off odds block

        if(bass_mode==true){x = 3422;} else {x = 3554;}; // start of NS block
        SY1K_hex_replace(x, 1, t); // switch off ns block

        if(bass_mode==true)
        {
            x = 3626;
        }
        else
        {
            x = 3758;           // start of FX1
        };
        SY1K_hex_replace(x, 1, GetJsonHex("patchFx1FXSW", a));
        t.clear();
        t.append(GetJsonHex("patchFx1FXTYPE", a));
        QString Fx1type = "Fx1_type = ";
        Fx1type.append(t.toHex().toUpper());
        SY1K_hex_replace(x+1, 1, convert_QByteArray_value(t, "17"));  // Fx1 fxtype
        if(Fx1type.contains("01"))
        {
            t = QByteArrayLiteral("\x01");
            SY1K_hex_replace(dd1, 1, t);  // switch on DD1 block
            SY1K_hex_replace(x+59, 1, GetJsonHex("patchFx1ChorusDelayChorusDepth", a));
            SY1K_hex_replace(x+79, 1, GetJsonHex("patchFx1ChorusDelayChorusDirectLevel", a));
            SY1K_hex_replace(x+61, 1, GetJsonHex("patchFx1ChorusDelayChorusEffectLevel", a));
            SY1K_hex_replace(x+64, 1, convert_QByteArray_value(GetJsonHex("patchFx1ChorusDelayChorusHighCut", a), "1A"));
            SY1K_hex_replace(x+63, 1, GetJsonHex("patchFx1ChorusDelayChorusLowCut", a));
            SY1K_hex_replace(x+57, 1, GetJsonHex("patchFx1ChorusDelayChorusMode", a));
            SY1K_hex_replace(x+60, 1, GetJsonHex("patchFx1ChorusDelayChorusPreDelay", a));
            SY1K_hex_replace(x+58, 1, GetJsonHex("patchFx1ChorusDelayChorusRate", a));
            SY1K_hex_replace(dd1+8, 1, GetJsonHex("patchFx1ChorusDelayDelayDirectLevel", a));
            SY1K_hex_replace(dd1+7, 1, GetJsonHex("patchFx1ChorusDelayDelayDualEffectLevel1", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx1ChorusDelayDelayDualEffectLevel2", a));
            SY1K_hex_replace(dd1+5, 1, GetJsonHex("patchFx1ChorusDelayDelayDualFeedback1", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx1ChorusDelayDelayDualFeedback2", a));
            SY1K_hex_replace(dd1+6, 1, GetJsonHex("patchFx1ChorusDelayDelayDualHighCut1", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx1ChorusDelayDelayDualHighCut2", a));
            //SY1K_hex_replace(dd1+4, 1, GetJsonHex("patchFx1ChorusDelayDelayDualTime1_h", a));
            //SY1K_hex_replace(dd1+2, 1, GetJsonHex("patchFx1ChorusDelayDelayDualTime1_l", a));
            //SY1K_hex_replace(dd1+3, 1, GetJsonHex("patchFx1ChorusDelayDelayDualTime1_m", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx1ChorusDelayDelayDualTime2_h", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx1ChorusDelayDelayDualTime2_l", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx1ChorusDelayDelayDualTime2_m", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx1ChorusDelayDelayEffectLevel", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx1ChorusDelayDelayFeedback", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx1ChorusDelayDelayHighCut", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx1ChorusDelayDelayModDepth", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx1ChorusDelayDelayModRate", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx1ChorusDelayDelayPanTapTime", a));
            SY1K_hex_replace(dd1+4, 1, GetJsonHex("patchFx1ChorusDelayDelayTime_h", a));
            SY1K_hex_replace(dd1+2, 1, GetJsonHex("patchFx1ChorusDelayDelayTime_l", a));
            SY1K_hex_replace(dd1+3, 1, GetJsonHex("patchFx1ChorusDelayDelayTime_m", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx1ChorusDelayDelayType", a));
        };
        if(Fx1type.contains("00"))
        {
            SY1K_hex_replace(x+59, 1, GetJsonHex("patchFx1ChorusDepth", a));
            SY1K_hex_replace(x+79, 1, GetJsonHex("patchFx1ChorusDirectLevel", a));
            SY1K_hex_replace(x+61, 1, GetJsonHex("patchFx1ChorusEffectLevel", a));
            SY1K_hex_replace(x+64, 1, convert_QByteArray_value(GetJsonHex("patchFx1ChorusHighCut", a), "1A"));
            SY1K_hex_replace(x+63, 1, GetJsonHex("patchFx1ChorusLowCut", a));
            SY1K_hex_replace(x+57, 1, GetJsonHex("patchFx1ChorusMode", a));
            SY1K_hex_replace(x+60, 1, GetJsonHex("patchFx1ChorusPreDelay", a));
            SY1K_hex_replace(x+58, 1, GetJsonHex("patchFx1ChorusRate", a));
        };
        if(Fx1type.contains("02"))
        {
            SY1K_hex_replace(x+59, 1, GetJsonHex("patchFx1ChorusReverbChorusDepth", a));
            SY1K_hex_replace(x+79, 1, GetJsonHex("patchFx1ChorusReverbChorusDirectLevel", a));
            SY1K_hex_replace(x+61, 1, GetJsonHex("patchFx1ChorusReverbChorusEffectLevel", a));
            SY1K_hex_replace(x+64, 1, convert_QByteArray_value(GetJsonHex("patchFx1ChorusReverbChorusHighCut", a), "1A"));
            SY1K_hex_replace(x+63, 1, GetJsonHex("patchFx1ChorusReverbChorusLowCut", a));
            SY1K_hex_replace(x+57, 1, GetJsonHex("patchFx1ChorusReverbChorusMode", a));
            SY1K_hex_replace(x+60, 1, GetJsonHex("patchFx1ChorusReverbChorusPreDelay", a));
            SY1K_hex_replace(x+58, 1, GetJsonHex("patchFx1ChorusReverbChorusRate", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx1ChorusReverbReverbDensity", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx1ChorusReverbReverbDirectLevel", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx1ChorusReverbReverbEffectLevel", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx1ChorusReverbReverbHighCut", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx1ChorusReverbReverbLowCut", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx1ChorusReverbReverbPreDelay_h", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx1ChorusReverbReverbPreDelay_l", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx1ChorusReverbReverbPreDelay_m", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx1ChorusReverbReverbSpringSens", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx1ChorusReverbReverbTime", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx1ChorusReverbReverbType", a));
        };
        SY1K_hex_replace(x+115, 1, GetJsonHex("patchFx1CompressorAttack", a));
        SY1K_hex_replace(x+116, 1, GetJsonHex("patchFx1CompressorEffectLevel", a));
        SY1K_hex_replace(x+114, 1, GetJsonHex("patchFx1CompressorSustain", a));
        SY1K_hex_replace(x+118, 1, GetJsonHex("patchFx1CompressorTone", a));
        SY1K_hex_replace(x+113, 1, GetJsonHex("patchFx1CompressorType", a));
       // if(Fx1type.contains("04")){
            SY1K_hex_replace(x+178, 1, GetJsonHex("patchFx1DelayTime_h", a));
            SY1K_hex_replace(x+176, 1, GetJsonHex("patchFx1DelayTime_l", a));
            SY1K_hex_replace(x+177, 1, GetJsonHex("patchFx1DelayTime_m", a));
            SY1K_hex_replace(x+174, 1, convert_QByteArray_value(GetJsonHex("patchFx1DelayType", a), "18"));
            SY1K_hex_replace(x+182, 1, GetJsonHex("patchFx1DelayDirectLevel", a));
            SY1K_hex_replace(x+189, 1, GetJsonHex("patchFx1DelayDualEffectLevel1", a));
            SY1K_hex_replace(x+196, 1, GetJsonHex("patchFx1DelayDualEffectLevel2", a));
            SY1K_hex_replace(x+187, 1, GetJsonHex("patchFx1DelayDualFeedback1", a));
            SY1K_hex_replace(x+194, 1, GetJsonHex("patchFx1DelayDualFeedback2", a));
            SY1K_hex_replace(x+188, 1, convert_QByteArray_value(GetJsonHex("patchFx1DelayDualHighCut1", a), "1A"));
            SY1K_hex_replace(x+195, 1, convert_QByteArray_value(GetJsonHex("patchFx1DelayDualHighCut2", a), "1A"));
            SY1K_hex_replace(x+186, 1, GetJsonHex("patchFx1DelayDualTime1_h", a));
            SY1K_hex_replace(x+184, 1, GetJsonHex("patchFx1DelayDualTime1_l", a));
            SY1K_hex_replace(x+185, 1, GetJsonHex("patchFx1DelayDualTime1_m", a));
            SY1K_hex_replace(x+193, 1, GetJsonHex("patchFx1DelayDualTime2_h", a));
            SY1K_hex_replace(x+191, 1, GetJsonHex("patchFx1DelayDualTime2_l", a));
            SY1K_hex_replace(x+192, 1, GetJsonHex("patchFx1DelayDualTime2_m", a));
            SY1K_hex_replace(x+181, 1, GetJsonHex("patchFx1DelayEffectLevel", a));
            SY1K_hex_replace(x+179, 1, GetJsonHex("patchFx1DelayFeedback", a));
            SY1K_hex_replace(x+180, 1, convert_QByteArray_value(GetJsonHex("patchFx1DelayHighCut", a), "1A"));
            SY1K_hex_replace(x+198, 1, GetJsonHex("patchFx1DelayModDepth", a));
            SY1K_hex_replace(x+197, 1, GetJsonHex("patchFx1DelayModRate", a));
            SY1K_hex_replace(x+204, 1, GetJsonHex("patchFx1DelayPanTapTime", a));
       // };
        if(Fx1type.contains("05"))
        {
            SY1K_hex_replace(x+182, 1, GetJsonHex("patchFx1DelayDirectLevel", a));
            SY1K_hex_replace(x+189, 1, GetJsonHex("patchFx1DelayDualEffectLevel1", a));
            SY1K_hex_replace(x+196, 1, GetJsonHex("patchFx1DelayDualEffectLevel2", a));
            SY1K_hex_replace(x+187, 1, GetJsonHex("patchFx1DelayDualFeedback1", a));
            SY1K_hex_replace(x+194, 1, GetJsonHex("patchFx1DelayDualFeedback2", a));
            SY1K_hex_replace(x+188, 1, convert_QByteArray_value(GetJsonHex("patchFx1DelayDualHighCut1", a), "1A"));
            SY1K_hex_replace(x+195, 1, convert_QByteArray_value(GetJsonHex("patchFx1DelayDualHighCut2", a), "1A"));
            SY1K_hex_replace(x+186, 1, GetJsonHex("patchFx1DelayDualTime1_h", a));
            SY1K_hex_replace(x+184, 1, GetJsonHex("patchFx1DelayDualTime1_l", a));
            SY1K_hex_replace(x+185, 1, GetJsonHex("patchFx1DelayDualTime1_m", a));
            SY1K_hex_replace(x+193, 1, GetJsonHex("patchFx1DelayDualTime2_h", a));
            SY1K_hex_replace(x+191, 1, GetJsonHex("patchFx1DelayDualTime2_l", a));
            SY1K_hex_replace(x+192, 1, GetJsonHex("patchFx1DelayDualTime2_m", a));
            SY1K_hex_replace(x+181, 1, GetJsonHex("patchFx1DelayReverbDelayEffectLevel", a));
            SY1K_hex_replace(x+179, 1, GetJsonHex("patchFx1DelayReverbDelayFeedback", a));
            SY1K_hex_replace(x+180, 1, convert_QByteArray_value(GetJsonHex("patchFx1DelayReverbDelayHighCut", a), "1A"));
            SY1K_hex_replace(x+198, 1, GetJsonHex("patchFx1DelayReverbDelayModDepth", a));
            SY1K_hex_replace(x+197, 1, GetJsonHex("patchFx1DelayReverbDelayModRate", a));
            SY1K_hex_replace(x+204, 1, GetJsonHex("patchFx1DelayReverbDelayPanTapTime", a));
            SY1K_hex_replace(x+178, 1, GetJsonHex("patchFx1DelayReverbDelayTime_h", a));
            SY1K_hex_replace(x+176, 1, GetJsonHex("patchFx1DelayReverbDelayTime_l", a));
            SY1K_hex_replace(x+177, 1, GetJsonHex("patchFx1DelayReverbDelayTime_m", a));
            SY1K_hex_replace(x+174, 1, GetJsonHex("patchFx1DelayReverbDelayType", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx1DelayReverbReverbDensity", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx1DelayReverbReverbDirectLevel", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx1DelayReverbReverbEffectLevel", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx1DelayReverbReverbHighCut", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx1DelayReverbReverbLowCut", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx1DelayReverbReverbPreDelay", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx1DelayReverbReverbPreDelay_h", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx1DelayReverbReverbPreDelay_l", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx1DelayReverbReverbPreDelay_m", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx1DelayReverbReverbSpringSens", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx1DelayReverbReverbTime", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx1DelayReverbReverbType", a));
        };
        SY1K_hex_replace(x+509, 1, convert_QByteArray_value(GetJsonHex("patchFx1EQHighCut", a), "1A"));
        SY1K_hex_replace(x+501, 1, convert_QByteArray_value(GetJsonHex("patchFx1EQHighGain4kHz", a), "19"));
        SY1K_hex_replace(x+505, 1, GetJsonHex("patchFx1EQHighMidFreq", a));
        SY1K_hex_replace(x+507, 1, convert_QByteArray_value(GetJsonHex("patchFx1EQHighMidGain", a), "19"));
        SY1K_hex_replace(x+506, 1, GetJsonHex("patchFx1EQHighMidQ", a));
        SY1K_hex_replace(x+510, 1, convert_QByteArray_value(GetJsonHex("patchFx1EQLevel", a), "19"));
        SY1K_hex_replace(x+508, 1, GetJsonHex("patchFx1EQLowCut", a));
        SY1K_hex_replace(x+500, 1, convert_QByteArray_value(GetJsonHex("patchFx1EQLowGain100Hz", a), "19"));
        SY1K_hex_replace(x+502, 1, GetJsonHex("patchFx1EQLowMidFreq", a));
        SY1K_hex_replace(x+504, 1, convert_QByteArray_value(GetJsonHex("patchFx1EQLowMidGain", a), "19"));
        SY1K_hex_replace(x+503, 1, GetJsonHex("patchFx1EQLowMidQ", a));
        SY1K_hex_replace(x+221, 1, GetJsonHex("patchFx1FlangerDepth", a));
        SY1K_hex_replace(x+228, 1, GetJsonHex("patchFx1FlangerDirectLevel", a));
        SY1K_hex_replace(x+227, 1, GetJsonHex("patchFx1FlangerEffectLevel", a));
        SY1K_hex_replace(x+225, 1, GetJsonHex("patchFx1FlangerLowCut", a));
        SY1K_hex_replace(x+223, 1, GetJsonHex("patchFx1FlangerManual", a));
        SY1K_hex_replace(x+220, 1, GetJsonHex("patchFx1FlangerRate", a));
        SY1K_hex_replace(x+222, 1, GetJsonHex("patchFx1FlangerResonance", a));
        SY1K_hex_replace(x+224, 1, GetJsonHex("patchFx1FlangerSeparation", a));
        SY1K_hex_replace(x+390, 1, GetJsonHex("patchFx1IsolatorBand", a));
        SY1K_hex_replace(x+393, 1, GetJsonHex("patchFx1IsolatorBandLevel", a));
        SY1K_hex_replace(x+392, 1, GetJsonHex("patchFx1IsolatorDepth", a));
        SY1K_hex_replace(x+391, 1, GetJsonHex("patchFx1IsolatorRate", a));
        SY1K_hex_replace(x+412, 1, GetJsonHex("patchFx1LimiterAttack", a));
        SY1K_hex_replace(x+411, 1, GetJsonHex("patchFx1LimiterEffectLevel", a));
        SY1K_hex_replace(x+410, 1, GetJsonHex("patchFx1LimiterRatio", a));
        SY1K_hex_replace(x+413, 1, GetJsonHex("patchFx1LimiterRelease", a));
        SY1K_hex_replace(x+409, 1, GetJsonHex("patchFx1LimiterThreshold", a));
        SY1K_hex_replace(x+408, 1, GetJsonHex("patchFx1LimiterType", a));
        SY1K_hex_replace(x+430, 1, GetJsonHex("patchFx1LOFIBalance", a));
        SY1K_hex_replace(x+428, 1, GetJsonHex("patchFx1LOFIBitDepth", a));
        SY1K_hex_replace(x+429, 1, GetJsonHex("patchFx1LOFISamplingRate", a));        
        if(Fx1type.contains("0B"))
        {
            t = QByteArrayLiteral("\x01");
            SY1K_hex_replace(amp, 1, t); // switch on preamp block
            SY1K_hex_replace(amp+14, 1, GetJsonHex("patchFx1ODDSAmpSW", a)); // cab off/on
            SY1K_hex_replace(amp+6, 1, GetJsonHex("patchFx1ODDSBottom", a));
            SY1K_hex_replace(amp+19, 1, GetJsonHex("patchFx1ODDSDirectLevel", a));
            SY1K_hex_replace(amp+2, 1, GetJsonHex("patchFx1ODDSDrive", a));
            SY1K_hex_replace(amp+5, 1, GetJsonHex("patchFx1ODDSLevel", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx1ODDSNSRELEASE", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx1ODDSNSSW", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx1ODDSNSTHRESHOLD", a));
            SY1K_hex_replace(amp+13, 1, GetJsonHex("patchFx1ODDSSoloLevel", a));
            SY1K_hex_replace(amp+12, 1, GetJsonHex("patchFx1ODDSSoloSwitch", a));
            SY1K_hex_replace(amp+8, 1, GetJsonHex("patchFx1ODDSTone", a));
            SY1K_hex_replace(amp+1, 1, GetJsonHex("patchFx1ODDSType", a));
        };
        SY1K_hex_replace(x+482, 1, GetJsonHex("patchFx1PanDepth", a));
        SY1K_hex_replace(x+483, 1, GetJsonHex("patchFx1PanEffectLevel", a));
        SY1K_hex_replace(x+484, 1, GetJsonHex("patchFx1PanPosition", a));
        SY1K_hex_replace(x+481, 1, GetJsonHex("patchFx1PanRate", a));
        SY1K_hex_replace(x+479, 1, GetJsonHex("patchFx1PanType", a));
        SY1K_hex_replace(x+480, 1, GetJsonHex("patchFx1PanWaveShape", a));
        SY1K_hex_replace(x+548, 1, GetJsonHex("patchFx1PhaserDepth", a));
        SY1K_hex_replace(x+553, 1, GetJsonHex("patchFx1PhaserDirectLevel", a));
        SY1K_hex_replace(x+552, 1, GetJsonHex("patchFx1PhaserEffectLevel", a));
        SY1K_hex_replace(x+550, 1, GetJsonHex("patchFx1PhaserManual", a));
        SY1K_hex_replace(x+547, 1, GetJsonHex("patchFx1PhaserRate", a));
        SY1K_hex_replace(x+549, 1, GetJsonHex("patchFx1PhaserResonance", a));
        SY1K_hex_replace(x+551, 1, GetJsonHex("patchFx1PhaserStepRate", a));
        SY1K_hex_replace(x+546, 1, GetJsonHex("patchFx1PhaserType", a));
        if(Fx1type.contains("0E"))
        {
            SY1K_hex_replace(x+612, 1, GetJsonHex("patchFx1ReverbDensity", a));
            SY1K_hex_replace(x+613, 1, GetJsonHex("patchFx1ReverbDirectLevel", a));
            SY1K_hex_replace(x+609, 1, GetJsonHex("patchFx1ReverbEffectLevel", a));
            SY1K_hex_replace(x+611, 1, convert_QByteArray_value(GetJsonHex("patchFx1ReverbHighCut", a), "1A"));
            SY1K_hex_replace(x+610, 1, GetJsonHex("patchFx1ReverbLowCut", a));
            SY1K_hex_replace(x+608, 1, GetJsonHex("patchFx1ReverbPreDelay_h", a));
            SY1K_hex_replace(x+606, 1, GetJsonHex("patchFx1ReverbPreDelay_l", a));
            SY1K_hex_replace(x+607, 1, GetJsonHex("patchFx1ReverbPreDelay_m", a));
            SY1K_hex_replace(x+614, 1, GetJsonHex("patchFx1ReverbSpringSens", a));
            SY1K_hex_replace(x+603, 1, GetJsonHex("patchFx1ReverbTime", a));
            SY1K_hex_replace(x+602, 1, GetJsonHex("patchFx1ReverbType", a));
        };
        SY1K_hex_replace(x+652, 1, GetJsonHex("patchFx1RotaryDepth", a));
        SY1K_hex_replace(x+655, 1, GetJsonHex("patchFx1RotaryEffectLevel", a));
        SY1K_hex_replace(x+654, 1, GetJsonHex("patchFx1RotaryFallTime", a));
        SY1K_hex_replace(x+651, 1, GetJsonHex("patchFx1RotaryRateFast", a));
        SY1K_hex_replace(x+650, 1, GetJsonHex("patchFx1RotaryRateSlow", a));
        SY1K_hex_replace(x+653, 1, GetJsonHex("patchFx1RotaryRiseTime", a));
        SY1K_hex_replace(x+649, 1, GetJsonHex("patchFx1RotarySpeedSelect", a));
        SY1K_hex_replace(x+695, 1, GetJsonHex("patchFx1SlicerDepth", a));
        SY1K_hex_replace(x+692, 1, GetJsonHex("patchFx1SlicerPattern", a));
        SY1K_hex_replace(x+693, 1, GetJsonHex("patchFx1SlicerRate", a));
        SY1K_hex_replace(x+715, 1, GetJsonHex("patchFx1SlowGearEffectLevel", a));
        SY1K_hex_replace(x+714, 1, GetJsonHex("patchFx1SlowGearRiseTime", a));
        SY1K_hex_replace(x+713, 1, GetJsonHex("patchFx1SlowGearSens", a));
        SY1K_hex_replace(x+771, 1, GetJsonHex("patchFx1TouchWahDirectLevel", a));
        SY1K_hex_replace(x+770, 1, GetJsonHex("patchFx1TouchWahEffectLevel", a));
        SY1K_hex_replace(x+767, 1, GetJsonHex("patchFx1TouchWahFreq", a));
        SY1K_hex_replace(x+764, 1, GetJsonHex("patchFx1TouchWahMode", a));
        SY1K_hex_replace(x+768, 1, GetJsonHex("patchFx1TouchWahPeak", a));
        SY1K_hex_replace(x+765, 1, GetJsonHex("patchFx1TouchWahPolar", a));
        SY1K_hex_replace(x+766, 1, GetJsonHex("patchFx1TouchWahSens", a));
        SY1K_hex_replace(x+810, 1, GetJsonHex("patchFx1TremoloDepth", a));
        SY1K_hex_replace(x+811, 1, GetJsonHex("patchFx1TremoloEffectLevel", a));
        SY1K_hex_replace(x+809, 1, GetJsonHex("patchFx1TremoloRate", a));
        SY1K_hex_replace(x+808, 1, GetJsonHex("patchFx1TremoloWaveShape", a));
        SY1K_hex_replace(x+96, 1, GetJsonHex("patchFx1UNIVDepth", a));
        SY1K_hex_replace(x+97, 1, GetJsonHex("patchFx1UNIVEffectLevel", a));
        SY1K_hex_replace(x+95, 1, GetJsonHex("patchFx1UNIVRate", a));

        if(bass_mode==true)
        {
            x = 4502;
        }
        else
        {
            x = 4634;                  // start of Fx2
        };
        SY1K_hex_replace(x, 1, GetJsonHex("patchFx2FXSW", a));
        t.clear();
        t.append(GetJsonHex("patchFx2FXTYPE", a));
        QString Fx2type = "Fx2_type = ";
        Fx2type.append(t.toHex().toUpper());
        SY1K_hex_replace(x+1, 1, convert_QByteArray_value(t, "17"));  // Fx2 fxtype
        if(Fx2type.contains("01"))
        {
            t = QByteArrayLiteral("\x01");
            SY1K_hex_replace(dd2, 1, t);  // switch on dd2 block
            SY1K_hex_replace(x+59, 1, GetJsonHex("patchFx2ChorusDelayChorusDepth", a));
            SY1K_hex_replace(x+79, 1, GetJsonHex("patchFx2ChorusDelayChorusDirectLevel", a));
            SY1K_hex_replace(x+61, 1, GetJsonHex("patchFx2ChorusDelayChorusEffectLevel", a));
            SY1K_hex_replace(x+64, 1, convert_QByteArray_value(GetJsonHex("patchFx2ChorusDelayChorusHighCut", a), "1A"));
            SY1K_hex_replace(x+63, 1, GetJsonHex("patchFx2ChorusDelayChorusLowCut", a));
            SY1K_hex_replace(x+57, 1, GetJsonHex("patchFx2ChorusDelayChorusMode", a));
            SY1K_hex_replace(x+60, 1, GetJsonHex("patchFx2ChorusDelayChorusPreDelay", a));
            SY1K_hex_replace(x+58, 1, GetJsonHex("patchFx2ChorusDelayChorusRate", a));
            SY1K_hex_replace(dd2+8, 1, GetJsonHex("patchFx2ChorusDelayDelayDirectLevel", a));
            SY1K_hex_replace(dd2+7, 1, GetJsonHex("patchFx2ChorusDelayDelayDualEffectLevel1", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx2ChorusDelayDelayDualEffectLevel2", a));
            SY1K_hex_replace(dd2+5, 1, GetJsonHex("patchFx2ChorusDelayDelayDualFeedback1", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx2ChorusDelayDelayDualFeedback2", a));
            SY1K_hex_replace(dd2+6, 1, GetJsonHex("patchFx2ChorusDelayDelayDualHighCut1", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx2ChorusDelayDelayDualHighCut2", a));
            //SY1K_hex_replace(dd2+4, 1, GetJsonHex("patchFx2ChorusDelayDelayDualTime1_h", a));
            //SY1K_hex_replace(dd2+2, 1, GetJsonHex("patchFx2ChorusDelayDelayDualTime1_l", a));
            //SY1K_hex_replace(dd2+3, 1, GetJsonHex("patchFx2ChorusDelayDelayDualTime1_m", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx2ChorusDelayDelayDualTime2_h", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx2ChorusDelayDelayDualTime2_l", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx2ChorusDelayDelayDualTime2_m", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx2ChorusDelayDelayEffectLevel", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx2ChorusDelayDelayFeedback", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx2ChorusDelayDelayHighCut", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx2ChorusDelayDelayModDepth", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx2ChorusDelayDelayModRate", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx2ChorusDelayDelayPanTapTime", a));
            SY1K_hex_replace(dd2+4, 1, GetJsonHex("patchFx2ChorusDelayDelayTime_h", a));
            SY1K_hex_replace(dd2+2, 1, GetJsonHex("patchFx2ChorusDelayDelayTime_l", a));
            SY1K_hex_replace(dd2+3, 1, GetJsonHex("patchFx2ChorusDelayDelayTime_m", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx2ChorusDelayDelayType", a));
        };
        if(Fx2type.contains("00"))
        {
            SY1K_hex_replace(x+59, 1, GetJsonHex("patchFx2ChorusDepth", a));
            SY1K_hex_replace(x+79, 1, GetJsonHex("patchFx2ChorusDirectLevel", a));
            SY1K_hex_replace(x+61, 1, GetJsonHex("patchFx2ChorusEffectLevel", a));
            SY1K_hex_replace(x+64, 1, convert_QByteArray_value(GetJsonHex("patchFx2ChorusHighCut", a), "1A"));
            SY1K_hex_replace(x+63, 1, GetJsonHex("patchFx2ChorusLowCut", a));
            SY1K_hex_replace(x+57, 1, GetJsonHex("patchFx2ChorusMode", a));
            SY1K_hex_replace(x+60, 1, GetJsonHex("patchFx2ChorusPreDelay", a));
            SY1K_hex_replace(x+58, 1, GetJsonHex("patchFx2ChorusRate", a));
        };
        if(Fx2type.contains("02"))
        {
            SY1K_hex_replace(x+59, 1, GetJsonHex("patchFx2ChorusReverbChorusDepth", a));
            SY1K_hex_replace(x+79, 1, GetJsonHex("patchFx2ChorusReverbChorusDirectLevel", a));
            SY1K_hex_replace(x+61, 1, GetJsonHex("patchFx2ChorusReverbChorusEffectLevel", a));
            SY1K_hex_replace(x+64, 1, convert_QByteArray_value(GetJsonHex("patchFx2ChorusReverbChorusHighCut", a), "1A"));
            SY1K_hex_replace(x+63, 1, GetJsonHex("patchFx2ChorusReverbChorusLowCut", a));
            SY1K_hex_replace(x+57, 1, GetJsonHex("patchFx2ChorusReverbChorusMode", a));
            SY1K_hex_replace(x+60, 1, GetJsonHex("patchFx2ChorusReverbChorusPreDelay", a));
            SY1K_hex_replace(x+58, 1, GetJsonHex("patchFx2ChorusReverbChorusRate", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx2ChorusReverbReverbDensity", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx2ChorusReverbReverbDirectLevel", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx2ChorusReverbReverbEffectLevel", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx2ChorusReverbReverbHighCut", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx2ChorusReverbReverbLowCut", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx2ChorusReverbReverbPreDelay_h", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx2ChorusReverbReverbPreDelay_l", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx2ChorusReverbReverbPreDelay_m", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx2ChorusReverbReverbSpringSens", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx2ChorusReverbReverbTime", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx2ChorusReverbReverbType", a));
        };
        SY1K_hex_replace(x+115, 1, GetJsonHex("patchFx2CompressorAttack", a));
        SY1K_hex_replace(x+116, 1, GetJsonHex("patchFx2CompressorEffectLevel", a));
        SY1K_hex_replace(x+114, 1, GetJsonHex("patchFx2CompressorSustain", a));
        SY1K_hex_replace(x+118, 1, GetJsonHex("patchFx2CompressorTone", a));
        SY1K_hex_replace(x+113, 1, GetJsonHex("patchFx2CompressorType", a));
        if(Fx2type.contains("04"))
        {
            SY1K_hex_replace(x+178, 1, GetJsonHex("patchFx2DelayTime_h", a));
            SY1K_hex_replace(x+176, 1, GetJsonHex("patchFx2DelayTime_l", a));
            SY1K_hex_replace(x+177, 1, GetJsonHex("patchFx2DelayTime_m", a));
            SY1K_hex_replace(x+174, 1, convert_QByteArray_value(GetJsonHex("patchFx2DelayType", a), "18"));
            SY1K_hex_replace(x+182, 1, GetJsonHex("patchFx2DelayDirectLevel", a));
            SY1K_hex_replace(x+189, 1, GetJsonHex("patchFx2DelayDualEffectLevel1", a));
            SY1K_hex_replace(x+196, 1, GetJsonHex("patchFx2DelayDualEffectLevel2", a));
            SY1K_hex_replace(x+187, 1, GetJsonHex("patchFx2DelayDualFeedback1", a));
            SY1K_hex_replace(x+194, 1, GetJsonHex("patchFx2DelayDualFeedback2", a));
            SY1K_hex_replace(x+188, 1, convert_QByteArray_value(GetJsonHex("patchFx2DelayDualHighCut1", a), "1A"));
            SY1K_hex_replace(x+195, 1, convert_QByteArray_value(GetJsonHex("patchFx2DelayDualHighCut2", a), "1A"));
            SY1K_hex_replace(x+186, 1, GetJsonHex("patchFx2DelayDualTime1_h", a));
            SY1K_hex_replace(x+184, 1, GetJsonHex("patchFx2DelayDualTime1_l", a));
            SY1K_hex_replace(x+185, 1, GetJsonHex("patchFx2DelayDualTime1_m", a));
            SY1K_hex_replace(x+193, 1, GetJsonHex("patchFx2DelayDualTime2_h", a));
            SY1K_hex_replace(x+191, 1, GetJsonHex("patchFx2DelayDualTime2_l", a));
            SY1K_hex_replace(x+192, 1, GetJsonHex("patchFx2DelayDualTime2_m", a));
            SY1K_hex_replace(x+181, 1, GetJsonHex("patchFx2DelayEffectLevel", a));
            SY1K_hex_replace(x+179, 1, GetJsonHex("patchFx2DelayFeedback", a));
            SY1K_hex_replace(x+180, 1, convert_QByteArray_value(GetJsonHex("patchFx2DelayHighCut", a), "1A"));
            SY1K_hex_replace(x+198, 1, GetJsonHex("patchFx2DelayModDepth", a));
            SY1K_hex_replace(x+197, 1, GetJsonHex("patchFx2DelayModRate", a));
            SY1K_hex_replace(x+204, 1, GetJsonHex("patchFx2DelayPanTapTime", a));
        };
        if(Fx2type.contains("05"))
        {
            SY1K_hex_replace(x+182, 1, GetJsonHex("patchFx2DelayDirectLevel", a));
            SY1K_hex_replace(x+189, 1, GetJsonHex("patchFx2DelayDualEffectLevel1", a));
            SY1K_hex_replace(x+196, 1, GetJsonHex("patchFx2DelayDualEffectLevel2", a));
            SY1K_hex_replace(x+187, 1, GetJsonHex("patchFx2DelayDualFeedback1", a));
            SY1K_hex_replace(x+194, 1, GetJsonHex("patchFx2DelayDualFeedback2", a));
            SY1K_hex_replace(x+188, 1, convert_QByteArray_value(GetJsonHex("patchFx2DelayDualHighCut1", a), "1A"));
            SY1K_hex_replace(x+195, 1, convert_QByteArray_value(GetJsonHex("patchFx2DelayDualHighCut2", a), "1A"));
            SY1K_hex_replace(x+186, 1, GetJsonHex("patchFx2DelayDualTime1_h", a));
            SY1K_hex_replace(x+184, 1, GetJsonHex("patchFx2DelayDualTime1_l", a));
            SY1K_hex_replace(x+185, 1, GetJsonHex("patchFx2DelayDualTime1_m", a));
            SY1K_hex_replace(x+193, 1, GetJsonHex("patchFx2DelayDualTime2_h", a));
            SY1K_hex_replace(x+191, 1, GetJsonHex("patchFx2DelayDualTime2_l", a));
            SY1K_hex_replace(x+192, 1, GetJsonHex("patchFx2DelayDualTime2_m", a));
            SY1K_hex_replace(x+181, 1, GetJsonHex("patchFx2DelayReverbDelayEffectLevel", a));
            SY1K_hex_replace(x+179, 1, GetJsonHex("patchFx2DelayReverbDelayFeedback", a));
            SY1K_hex_replace(x+180, 1, convert_QByteArray_value(GetJsonHex("patchFx2DelayReverbDelayHighCut", a), "1A"));
            SY1K_hex_replace(x+198, 1, GetJsonHex("patchFx2DelayReverbDelayModDepth", a));
            SY1K_hex_replace(x+197, 1, GetJsonHex("patchFx2DelayReverbDelayModRate", a));
            SY1K_hex_replace(x+204, 1, GetJsonHex("patchFx2DelayReverbDelayPanTapTime", a));
            SY1K_hex_replace(x+178, 1, GetJsonHex("patchFx2DelayReverbDelayTime_h", a));
            SY1K_hex_replace(x+176, 1, GetJsonHex("patchFx2DelayReverbDelayTime_l", a));
            SY1K_hex_replace(x+177, 1, GetJsonHex("patchFx2DelayReverbDelayTime_m", a));
            SY1K_hex_replace(x+174, 1, GetJsonHex("patchFx2DelayReverbDelayType", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx2DelayReverbReverbDensity", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx2DelayReverbReverbDirectLevel", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx2DelayReverbReverbEffectLevel", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx2DelayReverbReverbHighCut", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx2DelayReverbReverbLowCut", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx2DelayReverbReverbPreDelay", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx2DelayReverbReverbPreDelay_h", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx2DelayReverbReverbPreDelay_l", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx2DelayReverbReverbPreDelay_m", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx2DelayReverbReverbSpringSens", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx2DelayReverbReverbTime", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx2DelayReverbReverbType", a));
        };
        SY1K_hex_replace(x+509, 1, convert_QByteArray_value(GetJsonHex("patchFx2EQHighCut", a), "1A"));
        SY1K_hex_replace(x+501, 1, convert_QByteArray_value(GetJsonHex("patchFx2EQHighGain4kHz", a), "19"));
        SY1K_hex_replace(x+505, 1, GetJsonHex("patchFx2EQHighMidFreq", a));
        SY1K_hex_replace(x+507, 1, convert_QByteArray_value(GetJsonHex("patchFx2EQHighMidGain", a), "19"));
        SY1K_hex_replace(x+506, 1, GetJsonHex("patchFx2EQHighMidQ", a));
        SY1K_hex_replace(x+510, 1, convert_QByteArray_value(GetJsonHex("patchFx2EQLevel", a), "19"));
        SY1K_hex_replace(x+508, 1, GetJsonHex("patchFx2EQLowCut", a));
        SY1K_hex_replace(x+500, 1, convert_QByteArray_value(GetJsonHex("patchFx2EQLowGain100Hz", a), "19"));
        SY1K_hex_replace(x+502, 1, GetJsonHex("patchFx2EQLowMidFreq", a));
        SY1K_hex_replace(x+504, 1, convert_QByteArray_value(GetJsonHex("patchFx2EQLowMidGain", a), "19"));
        SY1K_hex_replace(x+503, 1, GetJsonHex("patchFx2EQLowMidQ", a));
        SY1K_hex_replace(x+221, 1, GetJsonHex("patchFx2FlangerDepth", a));
        SY1K_hex_replace(x+228, 1, GetJsonHex("patchFx2FlangerDirectLevel", a));
        SY1K_hex_replace(x+227, 1, GetJsonHex("patchFx2FlangerEffectLevel", a));
        SY1K_hex_replace(x+225, 1, GetJsonHex("patchFx2FlangerLowCut", a));
        SY1K_hex_replace(x+223, 1, GetJsonHex("patchFx2FlangerManual", a));
        SY1K_hex_replace(x+220, 1, GetJsonHex("patchFx2FlangerRate", a));
        SY1K_hex_replace(x+222, 1, GetJsonHex("patchFx2FlangerResonance", a));
        SY1K_hex_replace(x+224, 1, GetJsonHex("patchFx2FlangerSeparation", a));
        SY1K_hex_replace(x+390, 1, GetJsonHex("patchFx2IsolatorBand", a));
        SY1K_hex_replace(x+393, 1, GetJsonHex("patchFx2IsolatorBandLevel", a));
        SY1K_hex_replace(x+392, 1, GetJsonHex("patchFx2IsolatorDepth", a));
        SY1K_hex_replace(x+391, 1, GetJsonHex("patchFx2IsolatorRate", a));
        SY1K_hex_replace(x+412, 1, GetJsonHex("patchFx2LimiterAttack", a));
        SY1K_hex_replace(x+411, 1, GetJsonHex("patchFx2LimiterEffectLevel", a));
        SY1K_hex_replace(x+410, 1, GetJsonHex("patchFx2LimiterRatio", a));
        SY1K_hex_replace(x+413, 1, GetJsonHex("patchFx2LimiterRelease", a));
        SY1K_hex_replace(x+409, 1, GetJsonHex("patchFx2LimiterThreshold", a));
        SY1K_hex_replace(x+408, 1, GetJsonHex("patchFx2LimiterType", a));
        SY1K_hex_replace(x+430, 1, GetJsonHex("patchFx2LOFIBalance", a));
        SY1K_hex_replace(x+428, 1, GetJsonHex("patchFx2LOFIBitDepth", a));
        SY1K_hex_replace(x+429, 1, GetJsonHex("patchFx2LOFISamplingRate", a));        
        if(Fx2type.contains("0B"))
        {
            t = QByteArrayLiteral("\x01");
            SY1K_hex_replace(amp, 1, t); // switch on preamp block
            SY1K_hex_replace(amp+14, 1, GetJsonHex("patchFx2ODDSAmpSW", a)); // cab off/on
            SY1K_hex_replace(amp+6, 1, GetJsonHex("patchFx2ODDSBottom", a));
            SY1K_hex_replace(amp+19, 1, GetJsonHex("patchFx2ODDSDirectLevel", a));
            SY1K_hex_replace(amp+2, 1, GetJsonHex("patchFx2ODDSDrive", a));
            SY1K_hex_replace(amp+5, 1, GetJsonHex("patchFx2ODDSLevel", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx2ODDSNSRELEASE", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx2ODDSNSSW", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx2ODDSNSTHRESHOLD", a));
            SY1K_hex_replace(amp+13, 1, GetJsonHex("patchFx2ODDSSoloLevel", a));
            SY1K_hex_replace(amp+12, 1, GetJsonHex("patchFx2ODDSSoloSwitch", a));
            SY1K_hex_replace(amp+8, 1, GetJsonHex("patchFx2ODDSTone", a));
            SY1K_hex_replace(amp+1, 1, GetJsonHex("patchFx2ODDSType", a));
        };
        SY1K_hex_replace(x+482, 1, GetJsonHex("patchFx2PanDepth", a));
        SY1K_hex_replace(x+483, 1, GetJsonHex("patchFx2PanEffectLevel", a));
        SY1K_hex_replace(x+484, 1, GetJsonHex("patchFx2PanPosition", a));
        SY1K_hex_replace(x+481, 1, GetJsonHex("patchFx2PanRate", a));
        SY1K_hex_replace(x+479, 1, GetJsonHex("patchFx2PanType", a));
        SY1K_hex_replace(x+480, 1, GetJsonHex("patchFx2PanWaveShape", a));
        SY1K_hex_replace(x+548, 1, GetJsonHex("patchFx2PhaserDepth", a));
        SY1K_hex_replace(x+553, 1, GetJsonHex("patchFx2PhaserDirectLevel", a));
        SY1K_hex_replace(x+552, 1, GetJsonHex("patchFx2PhaserEffectLevel", a));
        SY1K_hex_replace(x+550, 1, GetJsonHex("patchFx2PhaserManual", a));
        SY1K_hex_replace(x+547, 1, GetJsonHex("patchFx2PhaserRate", a));
        SY1K_hex_replace(x+549, 1, GetJsonHex("patchFx2PhaserResonance", a));
        SY1K_hex_replace(x+551, 1, GetJsonHex("patchFx2PhaserStepRate", a));
        SY1K_hex_replace(x+546, 1, GetJsonHex("patchFx2PhaserType", a));
        if(Fx2type.contains("0E"))
        {
            SY1K_hex_replace(x+612, 1, GetJsonHex("patchFx2ReverbDensity", a));
            SY1K_hex_replace(x+613, 1, GetJsonHex("patchFx2ReverbDirectLevel", a));
            SY1K_hex_replace(x+609, 1, GetJsonHex("patchFx2ReverbEffectLevel", a));
            SY1K_hex_replace(x+611, 1, convert_QByteArray_value(GetJsonHex("patchFx2ReverbHighCut", a), "1A"));
            SY1K_hex_replace(x+610, 1, GetJsonHex("patchFx2ReverbLowCut", a));
            SY1K_hex_replace(x+608, 1, GetJsonHex("patchFx2ReverbPreDelay_h", a));
            SY1K_hex_replace(x+606, 1, GetJsonHex("patchFx2ReverbPreDelay_l", a));
            SY1K_hex_replace(x+607, 1, GetJsonHex("patchFx2ReverbPreDelay_m", a));
            SY1K_hex_replace(x+614, 1, GetJsonHex("patchFx2ReverbSpringSens", a));
            SY1K_hex_replace(x+603, 1, GetJsonHex("patchFx2ReverbTime", a));
            SY1K_hex_replace(x+602, 1, GetJsonHex("patchFx2ReverbType", a));
        };
        SY1K_hex_replace(x+652, 1, GetJsonHex("patchFx2RotaryDepth", a));
        SY1K_hex_replace(x+655, 1, GetJsonHex("patchFx2RotaryEffectLevel", a));
        SY1K_hex_replace(x+654, 1, GetJsonHex("patchFx2RotaryFallTime", a));
        SY1K_hex_replace(x+651, 1, GetJsonHex("patchFx2RotaryRateFast", a));
        SY1K_hex_replace(x+650, 1, GetJsonHex("patchFx2RotaryRateSlow", a));
        SY1K_hex_replace(x+653, 1, GetJsonHex("patchFx2RotaryRiseTime", a));
        SY1K_hex_replace(x+649, 1, GetJsonHex("patchFx2RotarySpeedSelect", a));
        SY1K_hex_replace(x+695, 1, GetJsonHex("patchFx2SlicerDepth", a));
        SY1K_hex_replace(x+692, 1, GetJsonHex("patchFx2SlicerPattern", a));
        SY1K_hex_replace(x+693, 1, GetJsonHex("patchFx2SlicerRate", a));
        SY1K_hex_replace(x+715, 1, GetJsonHex("patchFx2SlowGearEffectLevel", a));
        SY1K_hex_replace(x+714, 1, GetJsonHex("patchFx2SlowGearRiseTime", a));
        SY1K_hex_replace(x+713, 1, GetJsonHex("patchFx2SlowGearSens", a));
        SY1K_hex_replace(x+771, 1, GetJsonHex("patchFx2TouchWahDirectLevel", a));
        SY1K_hex_replace(x+770, 1, GetJsonHex("patchFx2TouchWahEffectLevel", a));
        SY1K_hex_replace(x+767, 1, GetJsonHex("patchFx2TouchWahFreq", a));
        SY1K_hex_replace(x+764, 1, GetJsonHex("patchFx2TouchWahMode", a));
        SY1K_hex_replace(x+768, 1, GetJsonHex("patchFx2TouchWahPeak", a));
        SY1K_hex_replace(x+765, 1, GetJsonHex("patchFx2TouchWahPolar", a));
        SY1K_hex_replace(x+766, 1, GetJsonHex("patchFx2TouchWahSens", a));
        SY1K_hex_replace(x+810, 1, GetJsonHex("patchFx2TremoloDepth", a));
        SY1K_hex_replace(x+811, 1, GetJsonHex("patchFx2TremoloEffectLevel", a));
        SY1K_hex_replace(x+809, 1, GetJsonHex("patchFx2TremoloRate", a));
        SY1K_hex_replace(x+808, 1, GetJsonHex("patchFx2TremoloWaveShape", a));
        SY1K_hex_replace(x+96, 1, GetJsonHex("patchFx2UNIVDepth", a));
        SY1K_hex_replace(x+97, 1, GetJsonHex("patchFx2UNIVEffectLevel", a));
        SY1K_hex_replace(x+95, 1, GetJsonHex("patchFx2UNIVRate", a));

        if(bass_mode==true)
        {
            x = 5378;
        }
        else
        {
            x = 5510;              // start of Fx3
        };
        SY1K_hex_replace(x, 1, GetJsonHex("patchFx3FXSW", a));
        t.clear();
        t.append(GetJsonHex("patchFx3FXTYPE", a));
        QString Fx3type = "Fx3_type = ";
        Fx3type.append(t.toHex().toUpper());
        SY1K_hex_replace(x+1, 1, convert_QByteArray_value(t, "17"));  // Fx3 fxtype
        if(Fx3type.contains("01"))
        {
            t = QByteArrayLiteral("\x01");
            SY1K_hex_replace(dd1, 1, t);  // switch on DD1 block
            SY1K_hex_replace(x+59, 1, GetJsonHex("patchFx3ChorusDelayChorusDepth", a));
            SY1K_hex_replace(x+79, 1, GetJsonHex("patchFx3ChorusDelayChorusDirectLevel", a));
            SY1K_hex_replace(x+61, 1, GetJsonHex("patchFx3ChorusDelayChorusEffectLevel", a));
            SY1K_hex_replace(x+64, 1, convert_QByteArray_value(GetJsonHex("patchFx3ChorusDelayChorusHighCut", a), "1A"));
            SY1K_hex_replace(x+63, 1, GetJsonHex("patchFx3ChorusDelayChorusLowCut", a));
            SY1K_hex_replace(x+57, 1, GetJsonHex("patchFx3ChorusDelayChorusMode", a));
            SY1K_hex_replace(x+60, 1, GetJsonHex("patchFx3ChorusDelayChorusPreDelay", a));
            SY1K_hex_replace(x+58, 1, GetJsonHex("patchFx3ChorusDelayChorusRate", a));
            //SY1K_hex_replace(dd1+8, 1, GetJsonHex("patchFx3ChorusDelayDelayDirectLevel", a));
            //SY1K_hex_replace(dd1+7, 1, GetJsonHex("patchFx3ChorusDelayDelayDualEffectLevel1", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx3ChorusDelayDelayDualEffectLevel2", a));
            //SY1K_hex_replace(dd1+5, 1, GetJsonHex("patchFx3ChorusDelayDelayDualFeedback1", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx3ChorusDelayDelayDualFeedback2", a));
            //SY1K_hex_replace(dd1+6, 1, GetJsonHex("patchFx3ChorusDelayDelayDualHighCut1", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx3ChorusDelayDelayDualHighCut2", a));
            //SY1K_hex_replace(dd1+2, 1, GetJsonHex("patchFx3ChorusDelayDelayDualTime1_h", a));
            //SY1K_hex_replace(dd1+4, 1, GetJsonHex("patchFx3ChorusDelayDelayDualTime1_l", a));
            //SY1K_hex_replace(dd1+3, 1, GetJsonHex("patchFx3ChorusDelayDelayDualTime1_m", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx3ChorusDelayDelayDualTime2_h", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx3ChorusDelayDelayDualTime2_l", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx3ChorusDelayDelayDualTime2_m", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx3ChorusDelayDelayEffectLevel", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx3ChorusDelayDelayFeedback", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx3ChorusDelayDelayHighCut", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx3ChorusDelayDelayModDepth", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx3ChorusDelayDelayModRate", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx3ChorusDelayDelayPanTapTime", a));
            //SY1K_hex_replace(dd1+2, 1, GetJsonHex("patchFx3ChorusDelayDelayTime_h", a));
            //SY1K_hex_replace(dd1+4, 1, GetJsonHex("patchFx3ChorusDelayDelayTime_l", a));
            //SY1K_hex_replace(dd1+3, 1, GetJsonHex("patchFx3ChorusDelayDelayTime_m", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx3ChorusDelayDelayType", a));
        };
        if(Fx3type.contains("00"))
        {
            SY1K_hex_replace(x+59, 1, GetJsonHex("patchFx3ChorusDepth", a));
            SY1K_hex_replace(x+79, 1, GetJsonHex("patchFx3ChorusDirectLevel", a));
            SY1K_hex_replace(x+61, 1, GetJsonHex("patchFx3ChorusEffectLevel", a));
            SY1K_hex_replace(x+64, 1, convert_QByteArray_value(GetJsonHex("patchFx3ChorusHighCut", a), "1A"));
            SY1K_hex_replace(x+63, 1, GetJsonHex("patchFx3ChorusLowCut", a));
            SY1K_hex_replace(x+57, 1, GetJsonHex("patchFx3ChorusMode", a));
            SY1K_hex_replace(x+60, 1, GetJsonHex("patchFx3ChorusPreDelay", a));
            SY1K_hex_replace(x+58, 1, GetJsonHex("patchFx3ChorusRate", a));
        };
        if(Fx3type.contains("02"))
        {
            SY1K_hex_replace(x+59, 1, GetJsonHex("patchFx3ChorusReverbChorusDepth", a));
            SY1K_hex_replace(x+79, 1, GetJsonHex("patchFx3ChorusReverbChorusDirectLevel", a));
            SY1K_hex_replace(x+61, 1, GetJsonHex("patchFx3ChorusReverbChorusEffectLevel", a));
            SY1K_hex_replace(x+64, 1, convert_QByteArray_value(GetJsonHex("patchFx3ChorusReverbChorusHighCut", a), "1A"));
            SY1K_hex_replace(x+63, 1, GetJsonHex("patchFx3ChorusReverbChorusLowCut", a));
            SY1K_hex_replace(x+57, 1, GetJsonHex("patchFx3ChorusReverbChorusMode", a));
            SY1K_hex_replace(x+60, 1, GetJsonHex("patchFx3ChorusReverbChorusPreDelay", a));
            SY1K_hex_replace(x+58, 1, GetJsonHex("patchFx3ChorusReverbChorusRate", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx3ChorusReverbReverbDensity", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx3ChorusReverbReverbDirectLevel", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx3ChorusReverbReverbEffectLevel", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx3ChorusReverbReverbHighCut", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx3ChorusReverbReverbLowCut", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx3ChorusReverbReverbPreDelay_h", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx3ChorusReverbReverbPreDelay_l", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx3ChorusReverbReverbPreDelay_m", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx3ChorusReverbReverbSpringSens", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx3ChorusReverbReverbTime", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx3ChorusReverbReverbType", a));
        };
        SY1K_hex_replace(x+115, 1, GetJsonHex("patchFx3CompressorAttack", a));
        SY1K_hex_replace(x+116, 1, GetJsonHex("patchFx3CompressorEffectLevel", a));
        SY1K_hex_replace(x+114, 1, GetJsonHex("patchFx3CompressorSustain", a));
        SY1K_hex_replace(x+118, 1, GetJsonHex("patchFx3CompressorTone", a));
        SY1K_hex_replace(x+113, 1, GetJsonHex("patchFx3CompressorType", a));
        if(Fx3type.contains("04"))
        {
            SY1K_hex_replace(x+178, 1, GetJsonHex("patchFx3DelayTime_h", a));
            SY1K_hex_replace(x+176, 1, GetJsonHex("patchFx3DelayTime_l", a));
            SY1K_hex_replace(x+177, 1, GetJsonHex("patchFx3DelayTime_m", a));
            SY1K_hex_replace(x+174, 1, convert_QByteArray_value(GetJsonHex("patchFx3DelayType", a), "18"));
            SY1K_hex_replace(x+182, 1, GetJsonHex("patchFx3DelayDirectLevel", a));
            SY1K_hex_replace(x+189, 1, GetJsonHex("patchFx3DelayDualEffectLevel1", a));
            SY1K_hex_replace(x+196, 1, GetJsonHex("patchFx3DelayDualEffectLevel2", a));
            SY1K_hex_replace(x+187, 1, GetJsonHex("patchFx3DelayDualFeedback1", a));
            SY1K_hex_replace(x+194, 1, GetJsonHex("patchFx3DelayDualFeedback2", a));
            SY1K_hex_replace(x+188, 1, convert_QByteArray_value(GetJsonHex("patchFx3DelayDualHighCut1", a), "1A"));
            SY1K_hex_replace(x+195, 1, convert_QByteArray_value(GetJsonHex("patchFx3DelayDualHighCut2", a), "1A"));
            SY1K_hex_replace(x+186, 1, GetJsonHex("patchFx3DelayDualTime1_h", a));
            SY1K_hex_replace(x+184, 1, GetJsonHex("patchFx3DelayDualTime1_l", a));
            SY1K_hex_replace(x+185, 1, GetJsonHex("patchFx3DelayDualTime1_m", a));
            SY1K_hex_replace(x+193, 1, GetJsonHex("patchFx3DelayDualTime2_h", a));
            SY1K_hex_replace(x+191, 1, GetJsonHex("patchFx3DelayDualTime2_l", a));
            SY1K_hex_replace(x+192, 1, GetJsonHex("patchFx3DelayDualTime2_m", a));
            SY1K_hex_replace(x+181, 1, GetJsonHex("patchFx3DelayEffectLevel", a));
            SY1K_hex_replace(x+179, 1, GetJsonHex("patchFx3DelayFeedback", a));
            SY1K_hex_replace(x+180, 1, convert_QByteArray_value(GetJsonHex("patchFx3DelayHighCut", a), "1A"));
            SY1K_hex_replace(x+198, 1, GetJsonHex("patchFx3DelayModDepth", a));
            SY1K_hex_replace(x+197, 1, GetJsonHex("patchFx3DelayModRate", a));
            SY1K_hex_replace(x+204, 1, GetJsonHex("patchFx3DelayPanTapTime", a));
        };
        if(Fx3type.contains("05"))
        {
            SY1K_hex_replace(x+182, 1, GetJsonHex("patchFx3DelayDirectLevel", a));
            SY1K_hex_replace(x+189, 1, GetJsonHex("patchFx3DelayDualEffectLevel1", a));
            SY1K_hex_replace(x+196, 1, GetJsonHex("patchFx3DelayDualEffectLevel2", a));
            SY1K_hex_replace(x+187, 1, GetJsonHex("patchFx3DelayDualFeedback1", a));
            SY1K_hex_replace(x+194, 1, GetJsonHex("patchFx3DelayDualFeedback2", a));
            SY1K_hex_replace(x+188, 1, convert_QByteArray_value(GetJsonHex("patchFx3DelayDualHighCut1", a), "1A"));
            SY1K_hex_replace(x+195, 1, convert_QByteArray_value(GetJsonHex("patchFx3DelayDualHighCut2", a), "1A"));
            SY1K_hex_replace(x+186, 1, GetJsonHex("patchFx3DelayDualTime1_h", a));
            SY1K_hex_replace(x+184, 1, GetJsonHex("patchFx3DelayDualTime1_l", a));
            SY1K_hex_replace(x+185, 1, GetJsonHex("patchFx3DelayDualTime1_m", a));
            SY1K_hex_replace(x+193, 1, GetJsonHex("patchFx3DelayDualTime2_h", a));
            SY1K_hex_replace(x+191, 1, GetJsonHex("patchFx3DelayDualTime2_l", a));
            SY1K_hex_replace(x+192, 1, GetJsonHex("patchFx3DelayDualTime2_m", a));
            SY1K_hex_replace(x+181, 1, GetJsonHex("patchFx3DelayReverbDelayEffectLevel", a));
            SY1K_hex_replace(x+179, 1, GetJsonHex("patchFx3DelayReverbDelayFeedback", a));
            SY1K_hex_replace(x+180, 1, convert_QByteArray_value(GetJsonHex("patchFx3DelayReverbDelayHighCut", a), "1A"));
            SY1K_hex_replace(x+198, 1, GetJsonHex("patchFx3DelayReverbDelayModDepth", a));
            SY1K_hex_replace(x+197, 1, GetJsonHex("patchFx3DelayReverbDelayModRate", a));
            SY1K_hex_replace(x+204, 1, GetJsonHex("patchFx3DelayReverbDelayPanTapTime", a));
            SY1K_hex_replace(x+178, 1, GetJsonHex("patchFx3DelayReverbDelayTime_h", a));
            SY1K_hex_replace(x+176, 1, GetJsonHex("patchFx3DelayReverbDelayTime_l", a));
            SY1K_hex_replace(x+177, 1, GetJsonHex("patchFx3DelayReverbDelayTime_m", a));
            SY1K_hex_replace(x+174, 1, GetJsonHex("patchFx3DelayReverbDelayType", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx3DelayReverbReverbDensity", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx3DelayReverbReverbDirectLevel", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx3DelayReverbReverbEffectLevel", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx3DelayReverbReverbHighCut", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx3DelayReverbReverbLowCut", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx3DelayReverbReverbPreDelay", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx3DelayReverbReverbPreDelay_h", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx3DelayReverbReverbPreDelay_l", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx3DelayReverbReverbPreDelay_m", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx3DelayReverbReverbSpringSens", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx3DelayReverbReverbTime", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx3DelayReverbReverbType", a));
        };
        SY1K_hex_replace(x+509, 1, convert_QByteArray_value(GetJsonHex("patchFx3EQHighCut", a), "1A"));
        SY1K_hex_replace(x+501, 1, convert_QByteArray_value(GetJsonHex("patchFx3EQHighGain4kHz", a), "19"));
        SY1K_hex_replace(x+505, 1, GetJsonHex("patchFx3EQHighMidFreq", a));
        SY1K_hex_replace(x+507, 1, convert_QByteArray_value(GetJsonHex("patchFx3EQHighMidGain", a), "19"));
        SY1K_hex_replace(x+506, 1, GetJsonHex("patchFx3EQHighMidQ", a));
        SY1K_hex_replace(x+510, 1, convert_QByteArray_value(GetJsonHex("patchFx3EQLevel", a), "19"));
        SY1K_hex_replace(x+508, 1, GetJsonHex("patchFx3EQLowCut", a));
        SY1K_hex_replace(x+500, 1, convert_QByteArray_value(GetJsonHex("patchFx3EQLowGain100Hz", a), "19"));
        SY1K_hex_replace(x+502, 1, GetJsonHex("patchFx3EQLowMidFreq", a));
        SY1K_hex_replace(x+504, 1, convert_QByteArray_value(GetJsonHex("patchFx3EQLowMidGain", a), "19"));
        SY1K_hex_replace(x+503, 1, GetJsonHex("patchFx3EQLowMidQ", a));
        SY1K_hex_replace(x+221, 1, GetJsonHex("patchFx3FlangerDepth", a));
        SY1K_hex_replace(x+228, 1, GetJsonHex("patchFx3FlangerDirectLevel", a));
        SY1K_hex_replace(x+227, 1, GetJsonHex("patchFx3FlangerEffectLevel", a));
        SY1K_hex_replace(x+225, 1, GetJsonHex("patchFx3FlangerLowCut", a));
        SY1K_hex_replace(x+223, 1, GetJsonHex("patchFx3FlangerManual", a));
        SY1K_hex_replace(x+220, 1, GetJsonHex("patchFx3FlangerRate", a));
        SY1K_hex_replace(x+222, 1, GetJsonHex("patchFx3FlangerResonance", a));
        SY1K_hex_replace(x+224, 1, GetJsonHex("patchFx3FlangerSeparation", a));
        SY1K_hex_replace(x+390, 1, GetJsonHex("patchFx3IsolatorBand", a));
        SY1K_hex_replace(x+393, 1, GetJsonHex("patchFx3IsolatorBandLevel", a));
        SY1K_hex_replace(x+392, 1, GetJsonHex("patchFx3IsolatorDepth", a));
        SY1K_hex_replace(x+391, 1, GetJsonHex("patchFx3IsolatorRate", a));
        SY1K_hex_replace(x+412, 1, GetJsonHex("patchFx3LimiterAttack", a));
        SY1K_hex_replace(x+411, 1, GetJsonHex("patchFx3LimiterEffectLevel", a));
        SY1K_hex_replace(x+410, 1, GetJsonHex("patchFx3LimiterRatio", a));
        SY1K_hex_replace(x+413, 1, GetJsonHex("patchFx3LimiterRelease", a));
        SY1K_hex_replace(x+409, 1, GetJsonHex("patchFx3LimiterThreshold", a));
        SY1K_hex_replace(x+408, 1, GetJsonHex("patchFx3LimiterType", a));
        SY1K_hex_replace(x+430, 1, GetJsonHex("patchFx3LOFIBalance", a));
        SY1K_hex_replace(x+428, 1, GetJsonHex("patchFx3LOFIBitDepth", a));
        SY1K_hex_replace(x+429, 1, GetJsonHex("patchFx3LOFISamplingRate", a));
        if(Fx3type.contains("0B"))
        {
            t = QByteArrayLiteral("\x01");
            SY1K_hex_replace(amp, 1, t); // switch on preamp block
            SY1K_hex_replace(amp+14, 1, GetJsonHex("patchFx3ODDSAmpSW", a)); // cab off/on
            SY1K_hex_replace(amp+6, 1, GetJsonHex("patchFx3ODDSBottom", a));
            SY1K_hex_replace(amp+19, 1, GetJsonHex("patchFx3ODDSDirectLevel", a));
            SY1K_hex_replace(amp+2, 1, GetJsonHex("patchFx3ODDSDrive", a));
            SY1K_hex_replace(amp+5, 1, GetJsonHex("patchFx3ODDSLevel", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx3ODDSNSRELEASE", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx3ODDSNSSW", a));
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx3ODDSNSTHRESHOLD", a));
            SY1K_hex_replace(amp+13, 1, GetJsonHex("patchFx3ODDSSoloLevel", a));
            SY1K_hex_replace(amp+12, 1, GetJsonHex("patchFx3ODDSSoloSwitch", a));
            SY1K_hex_replace(amp+8, 1, GetJsonHex("patchFx3ODDSTone", a));
            SY1K_hex_replace(amp+1, 1, GetJsonHex("patchFx3ODDSType", a));
        };
        SY1K_hex_replace(x+482, 1, GetJsonHex("patchFx3PanDepth", a));
        SY1K_hex_replace(x+483, 1, GetJsonHex("patchFx3PanEffectLevel", a));
        SY1K_hex_replace(x+484, 1, GetJsonHex("patchFx3PanPosition", a));
        SY1K_hex_replace(x+481, 1, GetJsonHex("patchFx3PanRate", a));
        SY1K_hex_replace(x+479, 1, GetJsonHex("patchFx3PanType", a));
        SY1K_hex_replace(x+480, 1, GetJsonHex("patchFx3PanWaveShape", a));
        SY1K_hex_replace(x+548, 1, GetJsonHex("patchFx3PhaserDepth", a));
        SY1K_hex_replace(x+553, 1, GetJsonHex("patchFx3PhaserDirectLevel", a));
        SY1K_hex_replace(x+552, 1, GetJsonHex("patchFx3PhaserEffectLevel", a));
        SY1K_hex_replace(x+550, 1, GetJsonHex("patchFx3PhaserManual", a));
        SY1K_hex_replace(x+547, 1, GetJsonHex("patchFx3PhaserRate", a));
        SY1K_hex_replace(x+549, 1, GetJsonHex("patchFx3PhaserResonance", a));
        SY1K_hex_replace(x+551, 1, GetJsonHex("patchFx3PhaserStepRate", a));
        SY1K_hex_replace(x+546, 1, GetJsonHex("patchFx3PhaserType", a));
        if(Fx3type.contains("0E"))
        {
            SY1K_hex_replace(x+612, 1, GetJsonHex("patchFx3ReverbDensity", a));
            SY1K_hex_replace(x+613, 1, GetJsonHex("patchFx3ReverbDirectLevel", a));
            SY1K_hex_replace(x+609, 1, GetJsonHex("patchFx3ReverbEffectLevel", a));
            SY1K_hex_replace(x+611, 1, convert_QByteArray_value(GetJsonHex("patchFx3ReverbHighCut", a), "1A"));
            SY1K_hex_replace(x+610, 1, GetJsonHex("patchFx3ReverbLowCut", a));
            SY1K_hex_replace(x+608, 1, GetJsonHex("patchFx3ReverbPreDelay_h", a));
            SY1K_hex_replace(x+606, 1, GetJsonHex("patchFx3ReverbPreDelay_l", a));
            SY1K_hex_replace(x+607, 1, GetJsonHex("patchFx3ReverbPreDelay_m", a));
            SY1K_hex_replace(x+614, 1, GetJsonHex("patchFx3ReverbSpringSens", a));
            SY1K_hex_replace(x+603, 1, GetJsonHex("patchFx3ReverbTime", a));
            SY1K_hex_replace(x+602, 1, GetJsonHex("patchFx3ReverbType", a));
        };
        SY1K_hex_replace(x+652, 1, GetJsonHex("patchFx3RotaryDepth", a));
        SY1K_hex_replace(x+655, 1, GetJsonHex("patchFx3RotaryEffectLevel", a));
        SY1K_hex_replace(x+654, 1, GetJsonHex("patchFx3RotaryFallTime", a));
        SY1K_hex_replace(x+651, 1, GetJsonHex("patchFx3RotaryRateFast", a));
        SY1K_hex_replace(x+650, 1, GetJsonHex("patchFx3RotaryRateSlow", a));
        SY1K_hex_replace(x+653, 1, GetJsonHex("patchFx3RotaryRiseTime", a));
        SY1K_hex_replace(x+649, 1, GetJsonHex("patchFx3RotarySpeedSelect", a));
        SY1K_hex_replace(x+695, 1, GetJsonHex("patchFx3SlicerDepth", a));
        SY1K_hex_replace(x+692, 1, GetJsonHex("patchFx3SlicerPattern", a));
        SY1K_hex_replace(x+693, 1, GetJsonHex("patchFx3SlicerRate", a));
        SY1K_hex_replace(x+715, 1, GetJsonHex("patchFx3SlowGearEffectLevel", a));
        SY1K_hex_replace(x+714, 1, GetJsonHex("patchFx3SlowGearRiseTime", a));
        SY1K_hex_replace(x+713, 1, GetJsonHex("patchFx3SlowGearSens", a));
        SY1K_hex_replace(x+771, 1, GetJsonHex("patchFx3TouchWahDirectLevel", a));
        SY1K_hex_replace(x+770, 1, GetJsonHex("patchFx3TouchWahEffectLevel", a));
        SY1K_hex_replace(x+767, 1, GetJsonHex("patchFx3TouchWahFreq", a));
        SY1K_hex_replace(x+764, 1, GetJsonHex("patchFx3TouchWahMode", a));
        SY1K_hex_replace(x+768, 1, GetJsonHex("patchFx3TouchWahPeak", a));
        SY1K_hex_replace(x+765, 1, GetJsonHex("patchFx3TouchWahPolar", a));
        SY1K_hex_replace(x+766, 1, GetJsonHex("patchFx3TouchWahSens", a));
        SY1K_hex_replace(x+810, 1, GetJsonHex("patchFx3TremoloDepth", a));
        SY1K_hex_replace(x+811, 1, GetJsonHex("patchFx3TremoloEffectLevel", a));
        SY1K_hex_replace(x+809, 1, GetJsonHex("patchFx3TremoloRate", a));
        SY1K_hex_replace(x+808, 1, GetJsonHex("patchFx3TremoloWaveShape", a));
        SY1K_hex_replace(x+96, 1, GetJsonHex("patchFx3UNIVDepth", a));
        SY1K_hex_replace(x+97, 1, GetJsonHex("patchFx3UNIVEffectLevel", a));
        SY1K_hex_replace(x+95, 1, GetJsonHex("patchFx3UNIVRate", a));

        QByteArray Fx4sw;
        Fx4sw.append(GetJsonHex("patchFx4FXSW", a));

        t.clear();
        t.append(GetJsonHex("patchFx4FXTYPE", a));
        QString Fx4type = "Fx4_type = ";
        Fx4type.append(t.toHex().toUpper());
        SY1K_hex_replace(x+1, 1, convert_QByteArray_value(t, "17"));  // Fx4 fxtype
        t = QByteArrayLiteral("\x01");
        if(Fx4type.contains("01"))
        {
            if(bass_mode==true)
            {
                x = 3587;
            }
            else
            {
                x = 3719;            // start of chorus block
            };
            SY1K_hex_replace(x, 1, t);  // switch on chorus block
            SY1K_hex_replace(x+3, 1, GetJsonHex("patchFx4ChorusDelayChorusDepth", a));
            SY1K_hex_replace(x+23, 1, GetJsonHex("patchFx4ChorusDelayChorusDirectLevel", a));
            SY1K_hex_replace(x+5, 1, GetJsonHex("patchFx4ChorusDelayChorusEffectLevel", a));
            SY1K_hex_replace(x+8, 1, convert_QByteArray_value(GetJsonHex("patchFx4ChorusDelayChorusHighCut", a), "1A"));
            SY1K_hex_replace(x+7, 1, GetJsonHex("patchFx4ChorusDelayChorusLowCut", a));
            SY1K_hex_replace(x+1, 1, GetJsonHex("patchFx4ChorusDelayChorusMode", a));
            SY1K_hex_replace(x+4, 1, GetJsonHex("patchFx4ChorusDelayChorusPreDelay", a));
            SY1K_hex_replace(x+2, 1, GetJsonHex("patchFx4ChorusDelayChorusRate", a));
            if(bass_mode==true)
            {
                x = 3540;
            }
            else
            {
                x = 3672;            // start of MstDelay block
            };
            SY1K_hex_replace(x, 1, t);  // switch on MstDelay block
            SY1K_hex_replace(x+9, 1, GetJsonHex("patchFx4ChorusDelayDelayDirectLevel", a));
            SY1K_hex_replace(x+16, 1, GetJsonHex("patchFx4ChorusDelayDelayDualEffectLevel1", a));
            SY1K_hex_replace(x+23, 1, GetJsonHex("patchFx4ChorusDelayDelayDualEffectLevel2", a));
            SY1K_hex_replace(x+14, 1, GetJsonHex("patchFx4ChorusDelayDelayDualFeedback1", a));
            SY1K_hex_replace(x+21, 1, GetJsonHex("patchFx4ChorusDelayDelayDualFeedback2", a));
            SY1K_hex_replace(x+7, 1, GetJsonHex("patchFx4ChorusDelayDelayDualHighCut1", a));
            SY1K_hex_replace(x+22, 1, GetJsonHex("patchFx4ChorusDelayDelayDualHighCut2", a));
            SY1K_hex_replace(x+13, 1, GetJsonHex("patchFx4ChorusDelayDelayDualTime1_h", a));
            SY1K_hex_replace(x+11, 1, GetJsonHex("patchFx4ChorusDelayDelayDualTime1_l", a));
            SY1K_hex_replace(x+12, 1, GetJsonHex("patchFx4ChorusDelayDelayDualTime1_m", a));
            SY1K_hex_replace(x+20, 1, GetJsonHex("patchFx4ChorusDelayDelayDualTime2_h", a));
            SY1K_hex_replace(x+18, 1, GetJsonHex("patchFx4ChorusDelayDelayDualTime2_l", a));
            SY1K_hex_replace(x+19, 1, GetJsonHex("patchFx4ChorusDelayDelayDualTime2_m", a));
            SY1K_hex_replace(x+8, 1, GetJsonHex("patchFx4ChorusDelayDelayEffectLevel", a));
            SY1K_hex_replace(x+6, 1, GetJsonHex("patchFx4ChorusDelayDelayFeedback", a));
            SY1K_hex_replace(x+7, 1, GetJsonHex("patchFx4ChorusDelayDelayHighCut", a));
            SY1K_hex_replace(x+25, 1, GetJsonHex("patchFx4ChorusDelayDelayModDepth", a));
            SY1K_hex_replace(x+24, 1, GetJsonHex("patchFx4ChorusDelayDelayModRate", a));
            SY1K_hex_replace(x+31, 1, GetJsonHex("patchFx4ChorusDelayDelayPanTapTime", a));
            SY1K_hex_replace(x+5, 1, GetJsonHex("patchFx4ChorusDelayDelayTime_h", a));
            SY1K_hex_replace(x+3, 1, GetJsonHex("patchFx4ChorusDelayDelayTime_l", a));
            SY1K_hex_replace(x+4, 1, GetJsonHex("patchFx4ChorusDelayDelayTime_m", a));
            SY1K_hex_replace(x+1, 1, GetJsonHex("patchFx4ChorusDelayDelayType", a));
        };
        if(Fx4type.contains("00"))
        {
            if(bass_mode==true)
            {
                x = 3687;
            }
            else
            {
                x = 3719;                // start of chorus block
            };
            SY1K_hex_replace(x, 1, t);  // switch on chorus block
            SY1K_hex_replace(x+3, 1, GetJsonHex("patchFx4ChorusDepth", a));
            SY1K_hex_replace(x+23, 1, GetJsonHex("patchFx4ChorusDirectLevel", a));
            SY1K_hex_replace(x+5, 1, GetJsonHex("patchFx4ChorusEffectLevel", a));
            SY1K_hex_replace(x+8, 1, convert_QByteArray_value(GetJsonHex("patchFx4ChorusHighCut", a), "1A"));
            SY1K_hex_replace(x+7, 1, GetJsonHex("patchFx4ChorusLowCut", a));
            SY1K_hex_replace(x+1, 1, GetJsonHex("patchFx4ChorusMode", a));
            SY1K_hex_replace(x+4, 1, GetJsonHex("patchFx4ChorusPreDelay", a));
            SY1K_hex_replace(x+2, 1, GetJsonHex("patchFx4ChorusRate", a));
        };
        if(Fx4type.contains("02"))
        {
            if(bass_mode==true)
            {
                x = 3687;
            }
            else
            {
                x = 3719;             // start of chorus block
            };
            SY1K_hex_replace(x, 1, t);  // switch on chorus block
            SY1K_hex_replace(x+3, 1, GetJsonHex("patchFx4ChorusReverbChorusDepth", a));
            SY1K_hex_replace(x+23, 1, GetJsonHex("patchFx4ChorusReverbChorusDirectLevel", a));
            SY1K_hex_replace(x+5, 1, GetJsonHex("patchFx4ChorusReverbChorusEffectLevel", a));
            SY1K_hex_replace(x+8, 1, convert_QByteArray_value(GetJsonHex("patchFx4ChorusReverbChorusHighCut", a), "1A"));
            SY1K_hex_replace(x+7, 1, GetJsonHex("patchFx4ChorusReverbChorusLowCut", a));
            SY1K_hex_replace(x+1, 1, GetJsonHex("patchFx4ChorusReverbChorusMode", a));
            SY1K_hex_replace(x+4, 1, GetJsonHex("patchFx4ChorusReverbChorusPreDelay", a));
            SY1K_hex_replace(x+2, 1, GetJsonHex("patchFx4ChorusReverbChorusRate", a));
            if(bass_mode==true)
            {
                x = 6254;
            }
            else
            {
                x = 6386;              // start of reverb block
            };
            SY1K_hex_replace(x, 1, t);  // switch on reverb block
            SY1K_hex_replace(x+11, 1, GetJsonHex("patchFx4ChorusReverbReverbDensity", a));
            SY1K_hex_replace(x+12, 1, GetJsonHex("patchFx4ChorusReverbReverbDirectLevel", a));
            SY1K_hex_replace(x+8, 1, GetJsonHex("patchFx4ChorusReverbReverbEffectLevel", a));
            SY1K_hex_replace(x+10, 1, GetJsonHex("patchFx4ChorusReverbReverbHighCut", a));
            SY1K_hex_replace(x+9, 1, GetJsonHex("patchFx4ChorusReverbReverbLowCut", a));
            SY1K_hex_replace(x+7, 1, GetJsonHex("patchFx4ChorusReverbReverbPreDelay_h", a));
            SY1K_hex_replace(x+5, 1, GetJsonHex("patchFx4ChorusReverbReverbPreDelay_l", a));
            SY1K_hex_replace(x+6, 1, GetJsonHex("patchFx4ChorusReverbReverbPreDelay_m", a));
            SY1K_hex_replace(x+13, 1, GetJsonHex("patchFx4ChorusReverbReverbSpringSens", a));
            SY1K_hex_replace(x+2, 1, GetJsonHex("patchFx4ChorusReverbReverbTime", a));
            SY1K_hex_replace(x+1, 1, GetJsonHex("patchFx4ChorusReverbReverbType", a));
        };
        if(Fx4type.contains("03"))
        {
        if(bass_mode==true)
        {
            x = 3344;
        }
        else
        {
            x = 3476;    // start of comp block
        };
        SY1K_hex_replace(x, 1, t);  // switch on comp block
        SY1K_hex_replace(x+3, 1, GetJsonHex("patchFx4CompressorAttack", a));
        SY1K_hex_replace(x+4, 1, GetJsonHex("patchFx4CompressorEffectLevel", a));
        SY1K_hex_replace(x+2, 1, GetJsonHex("patchFx4CompressorSustain", a));
        SY1K_hex_replace(x+6, 1, GetJsonHex("patchFx4CompressorTone", a));
        SY1K_hex_replace(x+1, 1, GetJsonHex("patchFx4CompressorType", a));
        };
        if(Fx4type.contains("04"))
        {
            if(bass_mode==true)
            {
                x = 3540;
            }
            else
            {
                x = 3672;         // start of MstDelay block
            };
            SY1K_hex_replace(x, 1, t);  // switch on MstDelay block
            SY1K_hex_replace(x+5, 1, GetJsonHex("patchFx4DelayTime_h", a));
            SY1K_hex_replace(x+3, 1, GetJsonHex("patchFx4DelayTime_l", a));
            SY1K_hex_replace(x+4, 1, GetJsonHex("patchFx4DelayTime_m", a));
            SY1K_hex_replace(x+1, 1, convert_QByteArray_value(GetJsonHex("patchFx4DelayType", a), "18"));
            SY1K_hex_replace(x+9, 1, GetJsonHex("patchFx4DelayDirectLevel", a));
            SY1K_hex_replace(x+16, 1, GetJsonHex("patchFx4DelayDualEffectLevel1", a));
            SY1K_hex_replace(x+23, 1, GetJsonHex("patchFx4DelayDualEffectLevel2", a));
            SY1K_hex_replace(x+14, 1, GetJsonHex("patchFx4DelayDualFeedback1", a));
            SY1K_hex_replace(x+21, 1, GetJsonHex("patchFx4DelayDualFeedback2", a));
            SY1K_hex_replace(x+15, 1, convert_QByteArray_value(GetJsonHex("patchFx4DelayDualHighCut1", a), "1A"));
            SY1K_hex_replace(x+22, 1, convert_QByteArray_value(GetJsonHex("patchFx4DelayDualHighCut2", a), "1A"));
            SY1K_hex_replace(x+13, 1, GetJsonHex("patchFx4DelayDualTime1_h", a));
            SY1K_hex_replace(x+11, 1, GetJsonHex("patchFx4DelayDualTime1_l", a));
            SY1K_hex_replace(x+12, 1, GetJsonHex("patchFx4DelayDualTime1_m", a));
            SY1K_hex_replace(x+20, 1, GetJsonHex("patchFx4DelayDualTime2_h", a));
            SY1K_hex_replace(x+18, 1, GetJsonHex("patchFx4DelayDualTime2_l", a));
            SY1K_hex_replace(x+19, 1, GetJsonHex("patchFx4DelayDualTime2_m", a));
            SY1K_hex_replace(x+8, 1, GetJsonHex("patchFx4DelayEffectLevel", a));
            SY1K_hex_replace(x+6, 1, GetJsonHex("patchFx4DelayFeedback", a));
            SY1K_hex_replace(x+7, 1, convert_QByteArray_value(GetJsonHex("patchFx4DelayHighCut", a), "1A"));
            SY1K_hex_replace(x+25, 1, GetJsonHex("patchFx4DelayModDepth", a));
            SY1K_hex_replace(x+24, 1, GetJsonHex("patchFx4DelayModRate", a));
            SY1K_hex_replace(x+31, 1, GetJsonHex("patchFx4DelayPanTapTime", a));
        };
        if(Fx4type.contains("05"))
        {
            if(bass_mode==true)
            {
                x = 3540;
            }
            else
            {
                x = 3672;      // start of MstDelay block
            };
            SY1K_hex_replace(x, 1, t);  // switch on MstDelay block
            SY1K_hex_replace(x+9, 1, GetJsonHex("patchFx4DelayDirectLevel", a));
            SY1K_hex_replace(x+16, 1, GetJsonHex("patchFx4DelayDualEffectLevel1", a));
            SY1K_hex_replace(x+23, 1, GetJsonHex("patchFx4DelayDualEffectLevel2", a));
            SY1K_hex_replace(x+14, 1, GetJsonHex("patchFx4DelayDualFeedback1", a));
            SY1K_hex_replace(x+21, 1, GetJsonHex("patchFx4DelayDualFeedback2", a));
            SY1K_hex_replace(x+15, 1, convert_QByteArray_value(GetJsonHex("patchFx4DelayDualHighCut1", a), "1A"));
            SY1K_hex_replace(x+22, 1, convert_QByteArray_value(GetJsonHex("patchFx4DelayDualHighCut2", a), "1A"));
            SY1K_hex_replace(x+13, 1, GetJsonHex("patchFx4DelayDualTime1_h", a));
            SY1K_hex_replace(x+11, 1, GetJsonHex("patchFx4DelayDualTime1_l", a));
            SY1K_hex_replace(x+12, 1, GetJsonHex("patchFx4DelayDualTime1_m", a));
            SY1K_hex_replace(x+14, 1, GetJsonHex("patchFx4DelayDualTime2_h", a));
            SY1K_hex_replace(x+12, 1, GetJsonHex("patchFx4DelayDualTime2_l", a));
            SY1K_hex_replace(x+13, 1, GetJsonHex("patchFx4DelayDualTime2_m", a));
            SY1K_hex_replace(x+8, 1, GetJsonHex("patchFx4DelayReverbDelayEffectLevel", a));
            SY1K_hex_replace(x+6, 1, GetJsonHex("patchFx4DelayReverbDelayFeedback", a));
            SY1K_hex_replace(x+7, 1, convert_QByteArray_value(GetJsonHex("patchFx4DelayReverbDelayHighCut", a), "1A"));
            SY1K_hex_replace(x+25, 1, GetJsonHex("patchFx4DelayReverbDelayModDepth", a));
            SY1K_hex_replace(x+24, 1, GetJsonHex("patchFx4DelayReverbDelayModRate", a));
            SY1K_hex_replace(x+31, 1, GetJsonHex("patchFx4DelayReverbDelayPanTapTime", a));
            SY1K_hex_replace(x+5, 1, GetJsonHex("patchFx4DelayReverbDelayTime_h", a));
            SY1K_hex_replace(x+3, 1, GetJsonHex("patchFx4DelayReverbDelayTime_l", a));
            SY1K_hex_replace(x+4, 1, GetJsonHex("patchFx4DelayReverbDelayTime_m", a));
            SY1K_hex_replace(x+1, 1, GetJsonHex("patchFx4DelayReverbDelayType", a));
            if(bass_mode==true)
            {
                x = 6254;
            }
            else
            {
                x = 6386;       // start of reverb block
            };
            SY1K_hex_replace(x, 1, t);  // switch on reverb block
            SY1K_hex_replace(x+11, 1, GetJsonHex("patchFx4DelayReverbReverbDensity", a));
            SY1K_hex_replace(x+12, 1, GetJsonHex("patchFx4DelayReverbReverbDirectLevel", a));
            SY1K_hex_replace(x+8, 1, GetJsonHex("patchFx4DelayReverbReverbEffectLevel", a));
            SY1K_hex_replace(x+10, 1, GetJsonHex("patchFx4DelayReverbReverbHighCut", a));
            SY1K_hex_replace(x+9, 1, GetJsonHex("patchFx4DelayReverbReverbLowCut", a));
            SY1K_hex_replace(x+7, 1, GetJsonHex("patchFx4DelayReverbReverbPreDelay_h", a));
            SY1K_hex_replace(x+5, 1, GetJsonHex("patchFx4DelayReverbReverbPreDelay_l", a));
            SY1K_hex_replace(x+6, 1, GetJsonHex("patchFx4DelayReverbReverbPreDelay_m", a));
            SY1K_hex_replace(x+13, 1, GetJsonHex("patchFx4DelayReverbReverbSpringSens", a));
            SY1K_hex_replace(x+2, 1, GetJsonHex("patchFx4DelayReverbReverbTime", a));
            SY1K_hex_replace(x+1, 1, GetJsonHex("patchFx4DelayReverbReverbType", a));
        };
        if(Fx4type.contains("06"))
        {
        if(bass_mode==true)
        {
            x = 3440;
        }
        else
        {
            x = 3572;     // start of EQ block
        };
        SY1K_hex_replace(x, 1, t);  // switch on EQ block
        SY1K_hex_replace(x+10, 1, convert_QByteArray_value(GetJsonHex("patchFx4EQHighCut", a), "1A"));
        SY1K_hex_replace(x+2, 1, convert_QByteArray_value(GetJsonHex("patchFx4EQHighGain4kHz", a), "19"));
        SY1K_hex_replace(x+6, 1, GetJsonHex("patchFx4EQHighMidFreq", a));
        SY1K_hex_replace(x+8, 1, convert_QByteArray_value(GetJsonHex("patchFx4EQHighMidGain", a), "19"));
        SY1K_hex_replace(x+7, 1, GetJsonHex("patchFx4EQHighMidQ", a));
        SY1K_hex_replace(x+11, 1, convert_QByteArray_value(GetJsonHex("patchFx4EQLevel", a), "19"));
        SY1K_hex_replace(x+9, 1, GetJsonHex("patchFx4EQLowCut", a));
        SY1K_hex_replace(x+1, 1, convert_QByteArray_value(GetJsonHex("patchFx4EQLowGain100Hz", a), "19"));
        SY1K_hex_replace(x+6, 1, GetJsonHex("patchFx4EQLowMidFreq", a));
        SY1K_hex_replace(x+5, 1, convert_QByteArray_value(GetJsonHex("patchFx4EQLowMidGain", a), "19"));
        SY1K_hex_replace(x+4, 1, GetJsonHex("patchFx4EQLowMidQ", a));
        };

        if(Fx4type.contains("0B"))
        {
            if(bass_mode==true)
            {
                x = 3365;
            }
            else
            {
                x = 3476;  // start of ODDS block
            };
            SY1K_hex_replace(x, 1, t); // switch on odds block
            //SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx4ODDSAmpSW", a));
            SY1K_hex_replace(x+5, 1, GetJsonHex("patchFx4ODDSBottom", a));
            SY1K_hex_replace(x+6, 1, GetJsonHex("patchFx4ODDSDirectLevel", a));
            SY1K_hex_replace(x+2, 1, GetJsonHex("patchFx4ODDSDrive", a));
            SY1K_hex_replace(x+4, 1, GetJsonHex("patchFx4ODDSLevel", a));
            SY1K_hex_replace(x+8, 1, GetJsonHex("patchFx4ODDSSoloLevel", a));
            SY1K_hex_replace(x+7, 1, GetJsonHex("patchFx4ODDSSoloSwitch", a));
            SY1K_hex_replace(x+3, 1, GetJsonHex("patchFx4ODDSTone", a));
            SY1K_hex_replace(x+1, 1, GetJsonHex("patchFx4ODDSType", a));
            if(bass_mode==true)
            {
                x = 3422;
            }
            else
            {
                x = 3554;   // start of NS block
            };
            SY1K_hex_replace(x+2, 1, GetJsonHex("patchFx4ODDSNSRELEASE", a));
            SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx4ODDSNSSW", a));
            SY1K_hex_replace(x+1, 1, GetJsonHex("patchFx4ODDSNSTHRESHOLD", a));
        };

        if(Fx4type.contains("0E"))
        {
            if(bass_mode==true){x = 6254;} else {x = 6386;}; // start of reverb block
            SY1K_hex_replace(x, 1, t);  // switch on reverb block
            SY1K_hex_replace(x+11, 1, GetJsonHex("patchFx4ReverbDensity", a));
            SY1K_hex_replace(x+12, 1, GetJsonHex("patchFx4ReverbDirectLevel", a));
            SY1K_hex_replace(x+8, 1, GetJsonHex("patchFx4ReverbEffectLevel", a));
            SY1K_hex_replace(x+10, 1, convert_QByteArray_value(GetJsonHex("patchFx4ReverbHighCut", a), "1A"));
            SY1K_hex_replace(x+9, 1, GetJsonHex("patchFx4ReverbLowCut", a));
            SY1K_hex_replace(x+7, 1, GetJsonHex("patchFx4ReverbPreDelay_h", a));
            SY1K_hex_replace(x+5, 1, GetJsonHex("patchFx4ReverbPreDelay_l", a));
            SY1K_hex_replace(x+6, 1, GetJsonHex("patchFx4ReverbPreDelay_m", a));
            SY1K_hex_replace(x+13, 1, GetJsonHex("patchFx4ReverbSpringSens", a));
            SY1K_hex_replace(x+2, 1, GetJsonHex("patchFx4ReverbTime", a));
            SY1K_hex_replace(x+1, 1, GetJsonHex("patchFx4ReverbType", a));
        };
        /*
        SY1K_hex_replace(x+0, 1, GetJsonHex("patchFx4FlangerDepth", a));
        SY1K_hex_replace(x+228, 1, GetJsonHex("patchFx4FlangerDirectLevel", a));
        SY1K_hex_replace(x+227, 1, GetJsonHex("patchFx4FlangerEffectLevel", a));
        SY1K_hex_replace(x+225, 1, GetJsonHex("patchFx4FlangerLowCut", a));
        SY1K_hex_replace(x+223, 1, GetJsonHex("patchFx4FlangerManual", a));
        SY1K_hex_replace(x+220, 1, GetJsonHex("patchFx4FlangerRate", a));
        SY1K_hex_replace(x+222, 1, GetJsonHex("patchFx4FlangerResonance", a));
        SY1K_hex_replace(x+224, 1, GetJsonHex("patchFx4FlangerSeparation", a));
        SY1K_hex_replace(x+390, 1, GetJsonHex("patchFx4IsolatorBand", a));
        SY1K_hex_replace(x+393, 1, GetJsonHex("patchFx4IsolatorBandLevel", a));
        SY1K_hex_replace(x+392, 1, GetJsonHex("patchFx4IsolatorDepth", a));
        SY1K_hex_replace(x+391, 1, GetJsonHex("patchFx4IsolatorRate", a));
        SY1K_hex_replace(x+412, 1, GetJsonHex("patchFx4LimiterAttack", a));
        SY1K_hex_replace(x+411, 1, GetJsonHex("patchFx4LimiterEffectLevel", a));
        SY1K_hex_replace(x+410, 1, GetJsonHex("patchFx4LimiterRatio", a));
        SY1K_hex_replace(x+413, 1, GetJsonHex("patchFx4LimiterRelease", a));
        SY1K_hex_replace(x+409, 1, GetJsonHex("patchFx4LimiterThreshold", a));
        SY1K_hex_replace(x+408, 1, GetJsonHex("patchFx4LimiterType", a));
        SY1K_hex_replace(x+430, 1, GetJsonHex("patchFx4LOFIBalance", a));
        SY1K_hex_replace(x+428, 1, GetJsonHex("patchFx4LOFIBitDepth", a));
        SY1K_hex_replace(x+429, 1, GetJsonHex("patchFx4LOFISamplingRate", a));
        SY1K_hex_replace(x+482, 1, GetJsonHex("patchFx4PanDepth", a));
        SY1K_hex_replace(x+483, 1, GetJsonHex("patchFx4PanEffectLevel", a));
        SY1K_hex_replace(x+484, 1, GetJsonHex("patchFx4PanPosition", a));
        SY1K_hex_replace(x+481, 1, GetJsonHex("patchFx4PanRate", a));
        SY1K_hex_replace(x+479, 1, GetJsonHex("patchFx4PanType", a));
        SY1K_hex_replace(x+480, 1, GetJsonHex("patchFx4PanWaveShape", a));
        SY1K_hex_replace(x+548, 1, GetJsonHex("patchFx4PhaserDepth", a));
        SY1K_hex_replace(x+553, 1, GetJsonHex("patchFx4PhaserDirectLevel", a));
        SY1K_hex_replace(x+552, 1, GetJsonHex("patchFx4PhaserEffectLevel", a));
        SY1K_hex_replace(x+550, 1, GetJsonHex("patchFx4PhaserManual", a));
        SY1K_hex_replace(x+547, 1, GetJsonHex("patchFx4PhaserRate", a));
        SY1K_hex_replace(x+549, 1, GetJsonHex("patchFx4PhaserResonance", a));
        SY1K_hex_replace(x+551, 1, GetJsonHex("patchFx4PhaserStepRate", a));
        SY1K_hex_replace(x+546, 1, GetJsonHex("patchFx4PhaserType", a));
        SY1K_hex_replace(x+652, 1, GetJsonHex("patchFx4RotaryDepth", a));
        SY1K_hex_replace(x+655, 1, GetJsonHex("patchFx4RotaryEffectLevel", a));
        SY1K_hex_replace(x+654, 1, GetJsonHex("patchFx4RotaryFallTime", a));
        SY1K_hex_replace(x+651, 1, GetJsonHex("patchFx4RotaryRateFast", a));
        SY1K_hex_replace(x+650, 1, GetJsonHex("patchFx4RotaryRateSlow", a));
        SY1K_hex_replace(x+653, 1, GetJsonHex("patchFx4RotaryRiseTime", a));
        SY1K_hex_replace(x+649, 1, GetJsonHex("patchFx4RotarySpeedSelect", a));
        SY1K_hex_replace(x+695, 1, GetJsonHex("patchFx4SlicerDepth", a));
        SY1K_hex_replace(x+692, 1, GetJsonHex("patchFx4SlicerPattern", a));
        SY1K_hex_replace(x+693, 1, GetJsonHex("patchFx4SlicerRate", a));
        SY1K_hex_replace(x+715, 1, GetJsonHex("patchFx4SlowGearEffectLevel", a));
        SY1K_hex_replace(x+714, 1, GetJsonHex("patchFx4SlowGearRiseTime", a));
        SY1K_hex_replace(x+713, 1, GetJsonHex("patchFx4SlowGearSens", a));
        SY1K_hex_replace(x+771, 1, GetJsonHex("patchFx4TouchWahDirectLevel", a));
        SY1K_hex_replace(x+770, 1, GetJsonHex("patchFx4TouchWahEffectLevel", a));
        SY1K_hex_replace(x+767, 1, GetJsonHex("patchFx4TouchWahFreq", a));
        SY1K_hex_replace(x+764, 1, GetJsonHex("patchFx4TouchWahMode", a));
        SY1K_hex_replace(x+768, 1, GetJsonHex("patchFx4TouchWahPeak", a));
        SY1K_hex_replace(x+765, 1, GetJsonHex("patchFx4TouchWahPolar", a));
        SY1K_hex_replace(x+766, 1, GetJsonHex("patchFx4TouchWahSens", a));
        SY1K_hex_replace(x+810, 1, GetJsonHex("patchFx4TremoloDepth", a));
        SY1K_hex_replace(x+811, 1, GetJsonHex("patchFx4TremoloEffectLevel", a));
        SY1K_hex_replace(x+809, 1, GetJsonHex("patchFx4TremoloRate", a));
        SY1K_hex_replace(x+808, 1, GetJsonHex("patchFx4TremoloWaveShape", a));
        SY1K_hex_replace(x+96, 1, GetJsonHex("patchFx4UNIVDepth", a));
        SY1K_hex_replace(x+97, 1, GetJsonHex("patchFx4UNIVEffectLevel", a));
        SY1K_hex_replace(x+95, 1, GetJsonHex("patchFx4UNIVRate", a));
        */
        x = 252;
        incr = 0;

        write_Assign("CTL1", "1");
        write_Assign("CTL2", "1");
        write_Assign("CTL3", "1");
        write_Assign("CTL4", "1");
        write_Assign("CTL5", "1");
        write_Assign("EXP", "1");
        write_Assign("MIDICC", "1");
        write_Assign("WAVE", "1");

        write_Assign("CTL1", "2");
        write_Assign("CTL2", "2");
        write_Assign("CTL3", "2");
        write_Assign("CTL4", "2");
        write_Assign("CTL5", "2");
        write_Assign("EXP", "2");
        write_Assign("MIDICC", "2");
        write_Assign("WAVE", "2");

        write_Assign("CTL1", "3");
        write_Assign("CTL2", "3");
        write_Assign("CTL3", "3");
        write_Assign("CTL4", "3");
        write_Assign("CTL5", "3");
        write_Assign("EXP", "3");
        write_Assign("MIDICC", "3");
        write_Assign("WAVE", "3");

        write_Assign("CTL1", "4");
        write_Assign("CTL2", "4");
        write_Assign("CTL3", "4");
        write_Assign("CTL4", "4");
        write_Assign("CTL5", "4");

        write_Assign("CTL1", "5");
        write_Assign("CTL2", "5");
        write_Assign("CTL3", "5");
        write_Assign("CTL4", "5");
        write_Assign("CTL5", "5");

        write_Assign("CTL1", "6");
        write_Assign("CTL2", "6");
        write_Assign("CTL3", "6");
        write_Assign("CTL4", "6");
        write_Assign("CTL5", "6");


        /*QString moad = "Guitar";
        if(bass_mode == true){moad = "Bass";};
        QString m = "(" + moad +" Mode) SY-300 conversion size = " + QString::number(this->SY1K_default.size(), 10);
        std::string std = m.toStdString();
        std::cerr << std << std::endl;*/

        if(this->SY1K_default.size() == patchSize || this->SY1K_default.size() == patchSizeBass)
        {
            return(this->SY1K_default);
        }else
        {
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
