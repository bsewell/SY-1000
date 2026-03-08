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
#include "sysxWriter.h"
#include "fileDialog.h"
#include "globalVariables.h"
#include <QTimer>
#include "SysxIO.h"
#include "MidiTable.h"
#include <QJsonArray>
#include <QRegularExpression>
#include "Preferences.h"
#include "tsl_GP10_translation.h"
#include "tsl_SY300_translation.h"
#include "tsl_GP10_translation.h"
#include "tsl_GT1000_translation.h"
#include <QDebug>


sysxWriter::sysxWriter()
{

}

sysxWriter::~sysxWriter()
{

}

void sysxWriter::setFile(QString fileName)
{
    this->fileName = fileName;
}

bool sysxWriter::readFile()
{
    Preferences *preferences = Preferences::Instance();

    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly))
    {
        this->data = file.readAll();                                                  // read the pre-selected file,
        // Read the default SY-1000 sysx file .

        if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
        {
            QFile file(":/default_bass.syx");                                          // if bass mode then load bass mode default.syx
            if (file.open(QIODevice::ReadOnly))
            {
                this->SY1000_default = file.readAll();
                file.close();
            };
        }
        else
        {
            QFile file(":/default_guitar.syx");                                        // if guitar mode then load guitar mode default.syx
            if (file.open(QIODevice::ReadOnly))
            {
                this->SY1000_default = file.readAll();
                file.close();
            };
        };
        file.close();
        QByteArray default_header = this->SY1000_default.mid(0, sysxAddressOffset);   // copy header from default.syx (guitar or bass mode).
        QByteArray file_header = data.mid(0, sysxAddressOffset);                      // copy header from read file
        bool isHeader = false;
        if (default_header == file_header)
        {
            isHeader = true;
        };

        if((data.size() == systemSize) && isHeader == true)
        {                          // if SY-1000 system file size is correct- load file.
            SysxIO *sysxIO = SysxIO::Instance();
            QString area = "System";
            sysxIO->setFileSource(area, data);
            return true;
        }
        else if((data.size() == patchSize || data.size() == patchSizeBass) && isHeader == true)   // if SY-1000 patch file size is correct- load file
        {
            if((preferences->getPreferences("Window", "BassMode", "bool")=="true" && data.size()==patchSizeBass)
                || (preferences->getPreferences("Window", "BassMode", "bool")!="true" && data.size()==patchSize))
            {
                SysxIO *sysxIO = SysxIO::Instance();
                QString area = "Structure";
                sysxIO->setFileSource(area, data);
                sysxIO->setFileName(this->fileName);
                sysxIO->fileSource = sysxIO->getFileSource();
                return true;
            };
        }
        else if (fileName.contains(".tsl"))
        {
            QString device=GetJsonArray("device", 0);
            if(device.contains("SY-300"))
            {
                convertSY300tsl_to_default(0);
                return true;
            }
            else if(device.contains("SY-1000"))
            {
                if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
                {
                    convertTSL_to_default_B(0);
                }
                else
                {
                    convertTSL_to_default_G(0);
                };
                return true;
            }
            else if(device.contains("GT-1000"))
            {
                convert_GT1000_TSL_to_default(0);
                return true;
            }
            else if(device.contains("GP-10"))
            {
                QFile file(":/default_guitar.syx");                                        // if guitar mode then load guitar mode default.syx
                if (file.open(QIODevice::ReadOnly))
                {
                    this->SY1000_default = file.readAll();
                    file.close();
                };
                convert_GP10_TSL_to_default(0);
                return true;
            };
        }
        else if(fileName.contains(".mid") && (data.mid(0,48).toHex().toUpper().contains("410000001C12") || data.mid(0,48).toHex().toUpper().contains("410000001D12"))/*data.size()==5923 || data.size()==6125 || data.size()==6060)*/) // is a VG-99 or VB-99 *.mid patch file.
        {
            convert_VG99_VB99_to_default(0);
            return true;
        }
        else if(fileName.contains(".g9l"))  // VG-99 librarian file.
        {
            convert_VG99_VB99_to_default(0);
            return true;
        }
        else
        {
            QMessageBox *msgBox = new QMessageBox();
            msgBox->setWindowTitle(QObject::tr("Patch size Error!"));
            msgBox->setIcon(QMessageBox::Warning);
            msgBox->setTextFormat(Qt::RichText);
            QString msgText;
            msgText.append("<font size='+1'><b>");
            msgText.append(QObject::tr("This is not a VG/VB or ") + deviceType + QObject::tr(" patch!"));
            msgText.append("<b></font><br>");
            msgText.append(QObject::tr("Patch size is not ") + (QString::number(patchSize, 10)) + QObject::tr(" bytes, please try another file."));
            msgText.append(QObject::tr("size reported to be ") + (QString::number(data.size(), 10)) + QObject::tr(" bytes."));
            msgBox->setText(msgText);
            msgBox->show();
            QTimer::singleShot(3000, msgBox, SLOT(deleteLater()));
            return false;
        };
        file.close();
    }
    else
    {
        return false;
    };
    return false;
}

void sysxWriter::patchIndex(int listIndex)
{
    Q_UNUSED(listIndex);
    SysxIO *sysxIO = SysxIO::Instance();
    this->index=sysxIO->patchListValue;
}

void sysxWriter::writeFile(QString fileName)
{
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly))
    {
        SysxIO *sysxIO = SysxIO::Instance();
        QByteArray out;
        for (QList< QList<QString> >::iterator dev = sysxIO->fileSource.hex.begin(); dev != sysxIO->fileSource.hex.end(); ++dev)
        {
            QList<QString> outData(*dev);
            for (QList<QString>::iterator code = outData.begin(); code != outData.end(); ++code)
            {
                QString str(*code);
                bool ok;
                uint n = str.toInt(&ok, 16);
                out.append(char(n));
            };
        };
        file.write(out);
        file.close();
    };

}

void sysxWriter::writeSystemFile(QString fileName)
{
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly))
    {
        SysxIO *sysxIO = SysxIO::Instance();
        QByteArray out;
        for (QList< QList<QString> >::iterator dev = sysxIO->systemSource.hex.begin(); dev != sysxIO->systemSource.hex.end(); ++dev)
        {
            QList<QString> outData(*dev);
            for (QList<QString>::iterator code = outData.begin(); code != outData.end(); ++code)
            {
                QString str(*code);
                bool ok;
                uint n = str.toUInt(&ok, 16);
                out.append(char(n));
            };
        };
        file.write(out);
        file.close();
    };
}

SysxData sysxWriter::getFileSource()
{
    SysxIO *sysxIO = SysxIO::Instance();
    return sysxIO->fileSource;
}

SysxData sysxWriter::getSystemSource()
{
    SysxIO *sysxIO = SysxIO::Instance();
    return sysxIO->systemSource;
}

QString sysxWriter::getFileName()
{
    return fileName;
}

void sysxWriter::writeTSL_G(QString fileName)                               // write current sysx patch data to TSL patch file.
{
    SysxIO *sysxIO = SysxIO::Instance();
    Preferences *preferences = Preferences::Instance();
    temp.clear();
    for (QList< QList<QString> >::iterator dev = sysxIO->fileSource.hex.begin(); dev != sysxIO->fileSource.hex.end(); ++dev)
    {   // convert QString data to char hex (QByteArray).
        QList<QString> outData(*dev);
        for (QList<QString>::iterator code = outData.begin(); code != outData.end(); ++code)
        {
            QString str(*code);
            bool ok;
            int n = str.toInt(&ok, 16);
            temp.append(char(n));
        };
    };

    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly))
    {
        if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
        {
            hex_B_to_TSL_G();                                               // convert from bass mode sysx hex to guitar mode TSL format
        }
        else
        {
            hex_G_to_TSL_G();                                               // convert from guitar mode sysx hex to guitar mode TSL format
        };
        file.write(this->TSL_default);                                      // write the patch file
        file.close();
    };
}

void sysxWriter::writeTSL_B(QString fileName)                               // write current sysx patch data to TSL patch file.
{
    Preferences *preferences = Preferences::Instance();
    SysxIO *sysxIO = SysxIO::Instance();
    temp.clear();
    for (QList< QList<QString> >::iterator dev = sysxIO->fileSource.hex.begin(); dev != sysxIO->fileSource.hex.end(); ++dev)
    {   // convert QString data to char hex (QByteArray).
        QList<QString> outData(*dev);
        for (QList<QString>::iterator code = outData.begin(); code != outData.end(); ++code)
        {
            QString str(*code);
            bool ok;
            int n = str.toInt(&ok, 16);
            temp.append(char(n));
        };
    };

    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly))
    {
        if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
        {
            hex_B_to_TSL_B();                                               // convert from bass mode sysx hex to bass mode TSL format
        }
        else
        {
            hex_G_to_TSL_B();                                               // convert from guitar mode sysx hex to bass mode TSL format
        };
        file.write(this->TSL_default);                                      // write the patch file
        file.close();
    };
}

void sysxWriter::SY1000_default_replace(int pos, int size, QByteArray value)
{
    if(!value.isEmpty() && !value.isNull())
    {
        SY1000_default.replace(pos, size, value);
    };
}

QByteArray sysxWriter::GetJsonString(QByteArray text, int pos )
{
    QByteArray str((char(34)+text+char(34)));
    int start_index = this->data.indexOf(str, pos)+(str.size()+2);           //find pointer to start of Json value.
    int end_index = this->data.indexOf("]", start_index)-start_index;        //find pointer to end of value to get the size of the value.
    QByteArray x = this->data.mid(start_index , end_index );                 //copy the Json value and return as a QByteArray.
    bool ok;
    QByteArray z;
    for(int f=0; f<x.size(); f++){
        QByteArray y;
        x.remove(x.indexOf(char(34)), 1);
        x.remove(x.indexOf(char(34)), 1);
        x.remove(x.indexOf(char(44)), 1);
        y.append(x.at(f)).append(x.at(f+1));
        z.append(char(y.toInt(&ok, 16)));
        f=f+1;
    };
    return z;
}

QByteArray sysxWriter::GetJsonArray(QByteArray text, int pos )
{
    QByteArray null("ul");                                                             // when read later on without "" null = ul
    QByteArray str((char(34)+text+char(34)));
    int start_index = this->data.indexOf(str, pos)+(text.size()+4);                    //find pointer to start of Json value.
    int end_index = this->data.indexOf(",", start_index)-start_index-1;                //find pointer to end of value to get the size of the value.
    QByteArray x = this->data.mid(start_index , end_index );                           //copy the Json value and return as a QByteArray.
    if(this->data.mid(start_index, 4).contains(null)) {x.clear(); };
    return x;
}

QByteArray sysxWriter::GetJsonValue(QByteArray text, int pos )
{
    QByteArray str((char(34)+text+char(34)));
    int start_index = this->data.indexOf(str, pos)+(text.size()+3);                    //find pointer to start of Json value.
    int end_index = this->data.indexOf(",", start_index)-start_index;                  //find pointer to end of value to get the size of the value.
    QByteArray x = this->data.mid(start_index , end_index );                           //copy the Json value and return as a QByteArray.
    if(SY1000)
    {
        bool ok;
        QByteArray z;
        for(int f=0; f<x.size()*2; f++)
        {
            QByteArray y;
            int value =  x.toInt(&ok, 10);
            y.append(char(value));
            z.append(y);
        };
        x=z;
    };
    return x;
}

void sysxWriter::AppendTSL(QByteArray hex, const char* Json_name)
{
    int s = hex.size();
    QByteArray d;
    for(int x=0; x<s; x++ )
    {
        d.append(char(34));
        char a = hex.at(x);
        QString val = QString::number(a, 16).toUpper();
        if(val.size()<2){val.prepend("0");};
        d.append(val.toLatin1());
        d.append(char(34));
        if(x<(s-1))
        {
            d.append(char(44));
        };
    };

    QByteArray name(Json_name);
    int start_index = this->TSL_default.indexOf(char(34)+name+char(34))+(name.size()+4);           //find pointer to start of Json value.
    int end_index = this->TSL_default.indexOf("]", start_index)-start_index;                       //find pointer to end of value to get the size of the value.
    this->TSL_default.replace(start_index, end_index, d);
}

void sysxWriter::convertTSL_to_default_G(int patch)
{
    SysxIO *sysxIO = SysxIO::Instance();
    int pnum;
    bool bass_patch = false;
    if(this->data.contains("UserBass_"))
    {
        bass_patch = true;
    };
    if(this->data.contains("paramSet"))
    {
        SY1000 = true;
        pnum = this->data.count("paramSet");                                                      // mk2 uses paramSet instead of params
    }
    else
    {
        SY1000 = false;
        pnum = this->data.count("params");                                                        // count the number of times 'params' appears to get patch count
    };

    int pindex = this->data.size()/(pnum+1);
    QString device=GetJsonArray("device", 0);

    int patchCount = pnum;
    if (patchCount>1)
    {
        QString msgText;
        QString patchText;
        QString patchNumber;
        this->patchList.clear();
        int a = 0;                                                                                // locate patch text start position from the start of the file
        if(SY1000)
        {
            a=0;
        };                                                                         //start of patchset data
        for (int h=0;h<patchCount;h++)
        {
            if(SY1000)
            {
                if(SY1000)
                {
                    if(bass_patch)
                    {
                        patchText.append(GetJsonString("UserBass_patch%common", a).mid(0, 16));
                    }
                    else
                    {
                        patchText.append(GetJsonString("User_patch%common", a).mid(0, 16));
                    }
                }
                else
                {
                    patchText.append(GetJsonValue("patchname", a).trimmed().remove(0, 1) ); patchText.chop(1);
                };
            };

            patchNumber = QString::number(h+1, 10).toUpper();
            msgText.append(patchNumber + " : ");
            msgText.append(patchText + "   ");
            this->patchList.append(msgText);
            patchText.clear();
            msgText.clear();
            a=a+pindex;                      // offset is set in front of marker
        };

        if(patch<1)
        {
            fileDialog *dialog = new fileDialog(fileName, patchList);
            dialog->exec();
            dialog->deleteLater();
            patchIndex(this->index);

            if(sysxIO->patchListValue == 0)
            {
                return;
            };
        }
        else
        {
            if(patch>patchCount)
            {
                patch=patchCount;
            };
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

    if(device.contains("SY-1000_guitarmode"))
    {
        TSL_G_to_hex_G();
        sysxIO->deBug("sysxWriter(convertTSL_to_default_G) (Guitar Mode) Guitar Patch size = " + QString::number(this->SY1000_default.size(), 10));

        if(this->SY1000_default.size() == 6402)
        {
            sysxIO->setFileSource("Structure", this->SY1000_default);
            sysxIO->setFileName(this->fileName);
            sysxIO->fileSource = sysxIO->getFileSource();
        }
        else
        {
            fileLoadError();
        };

    }
    else if(device.contains("SY-1000_bassmode"))
    {
        TSL_B_to_hex_G();

        if(this->SY1000_default.size() == 6402)
        {
            sysxIO->setFileSource("Structure", this->SY1000_default);
            sysxIO->setFileName(this->fileName);
            sysxIO->fileSource = sysxIO->getFileSource();
        }
        else
        {
            fileLoadError();
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
    };
}

void sysxWriter::convertTSL_to_default_B(int patch)
{
    SysxIO *sysxIO = SysxIO::Instance();
    int pnum;
    bool bass_patch = false;
    if(this->data.contains("UserBass_"))
    {
        bass_patch = true;
    };
    if(this->data.contains("paramSet"))
    {
        SY1000 = true;
        pnum = this->data.count("paramSet"); // mk2 uses paramSet instead of params
    }
    else
    {
        SY1000 = false;
        pnum = this->data.count("params"); // count the number of times 'params' appears to get patch count
    };

    int pindex = this->data.size()/(pnum+1);
    QString device=GetJsonArray("device", 0);

    int patchCount = pnum;
    if (patchCount>1)
    {
        QString msgText;
        QString patchText;
        QString patchNumber;
        this->patchList.clear();
        int a = 0;                  // locate patch text start position from the start of the file
        if(SY1000)
        {
            a=0;                    //start of patchset data
        };
        for (int h=0;h<patchCount;h++)
        {
            if(SY1000)
            {
                if(SY1000)
                {
                    if(bass_patch)
                    {
                        patchText.append(GetJsonString("UserBass_patch%common", a).mid(0, 16));
                    }
                    else
                    {
                        patchText.append(GetJsonString("User_patch%common", a).mid(0, 16));
                    }
                }
                else
                {
                    patchText.append(GetJsonValue("patchname", a).trimmed().remove(0, 1) );
                    patchText.chop(1);
                };
            };

            patchNumber = QString::number(h+1, 10).toUpper();
            msgText.append(patchNumber + " : ");
            msgText.append(patchText + "   ");
            this->patchList.append(msgText);
            patchText.clear();
            msgText.clear();
            a=a+pindex;                      // offset is set in front of marker
        };

        if(patch<1)
        {
            fileDialog *dialog = new fileDialog(fileName, patchList);
            dialog->exec();
            dialog->deleteLater();
            patchIndex(this->index);
            if(sysxIO->patchListValue == 0)
            {
                return;
            };
        }
        else
        {
            if(patch>patchCount)
            {
                patch=patchCount;
            };
            sysxIO->patchListValue = patch;
            this->index=sysxIO->patchListValue;
        };
    };

    this->a=0;
    if (patchCount>1)
    {
        int q=index-1;      // find start of required patch
        this->a = q*pindex;
    };
    if(device.contains("SY-1000_bassmode"))
    {
        TSL_B_to_hex_B();

        if(this->SY1000_default.size() == 6270)
        {
            sysxIO->setFileSource("Structure", this->SY1000_default);
            sysxIO->setFileName(this->fileName);
            sysxIO->fileSource = sysxIO->getFileSource();
        }
        else
        {
            fileLoadError();
        };

    }
    else if(device.contains("SY-1000_guitarmode"))
    {
        TSL_G_to_hex_B();

        sysxIO->deBug("(Bass Mode) Guitar Patch conversion size = " + QString::number(this->SY1000_default.size(), 10));

        if(this->SY1000_default.size() == 6270)
        {
            SysxIO *sysxIO = SysxIO::Instance();
            sysxIO->setFileSource("Structure", this->SY1000_default);
            sysxIO->setFileName(this->fileName);
            sysxIO->fileSource = sysxIO->getFileSource();
        }
        else
        {
            fileLoadError();
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
        msgText.append(QString::number(this->SY1000_default.size(), 10));
        msgBox->setText(msgText);
        msgBox->setStandardButtons(QMessageBox::Ok);
        msgBox->exec();
    };
}

void sysxWriter::convertSY300tsl_to_default(int patch)
{
    Q_UNUSED(patch);
    int mode = 1;
    this->SY300translate = new tsl_translation(this->data, SY1000_default, mode, fileName);
    QByteArray translatedData;
    translatedData.append(SY300translate->convertFromTSL(0));

    if(translatedData.size()>2)
    {
        SysxIO *sysxIO = SysxIO::Instance();
        sysxIO->setFileSource("Structure", translatedData);
        sysxIO->setFileName(this->fileName);
        sysxIO->fileSource = sysxIO->getFileSource();
    };
}

void sysxWriter::fileLoadError()
{
    QMessageBox *msgBox = new QMessageBox();
    msgBox->setWindowTitle(QObject::tr("File Load Error"));
    msgBox->setIcon(QMessageBox::Warning);
    msgBox->setTextFormat(Qt::RichText);
    QString msgText;
    msgText.append("Something went wrong loading this patch file<br>");
    msgText.append("the patch file might be corrupted or an incompatible type<br>");
    msgText.append("File load was aborted.<br>");
    msgText.append(QString::number(this->SY1000_default.size(), 10));
    msgBox->setText(msgText);
    msgBox->setStandardButtons(QMessageBox::Ok);
    msgBox->exec();
}

QByteArray sysxWriter::convert_QByteArray_value(QByteArray value, QString hex2, QString hex3)
{
    bool ok;
    int v = value.toHex().toInt(&ok, 16);
    MidiTable *midiTable = MidiTable::Instance();
    Midi items;
    items = midiTable->getMidiMap("Tables", "00", hex2, hex3);
    QString desc = items.level.at(v).desc;
    int nv=desc.toInt(&ok, 16);
    QByteArray value2;
    value2.append(char(nv));
    if(value2.size()>1)
    {
        value2.remove(0, 1);
    };

    return value2;
}

QByteArray sysxWriter::convert_Assign(QByteArray value, int modeInt)
{
    MidiTable *midiTable = MidiTable::Instance();
    QString mode = "02"; // look up bass assign in guitar mode
    if(modeInt==1) {mode = "00";}; // look up guitar assign in bass mode.
    bool ok;

    char a = value.at(0);
    QString val = QString::number(a, 16).toUpper();
    int b0 = val.toInt(&ok, 16);

    a = value.at(1);
    val = QString::number(a, 16).toUpper();
    int b1 = val.toInt(&ok, 16);

    a = value.at(2);
    val = QString::number(a, 16).toUpper();
    int b2 = val.toInt(&ok, 16);

    a = value.at(3);
    val = QString::number(a, 16).toUpper();
    int b3 = val.toInt(&ok, 16);

    int assign = (b0*4096) + (b1*256) + (b2*16) + b3;
    //QString assign_before(QString::number(assign, 10));
    int maxRange = 256;
    int dif = assign/maxRange;
    QString valueHex1 = QString::number(dif, 16).toUpper();
    if(valueHex1.length() < 2) valueHex1.prepend("0");
    QString valueHex2 = QString::number(assign - (dif * maxRange), 16).toUpper();
    if(valueHex2.length() < 2) valueHex2.prepend("0");
    QString hex4 = valueHex1;
    QString hex5 = valueHex2;                                     //convert valueStr to 7bit hex4, hex5
    Midi items = midiTable->getMidiMap("Tables", "00", "00", mode, hex4, hex5);
    assign += items.abbr.toInt(&ok, 10);

    QString index = QString::number(assign, 16);
    if(index.size()<2){index.prepend("000");};
    if(index.size()<3){index.prepend("00");};
    if(index.size()<4){index.prepend("0");};

    QString n3(index.at(3)); b3 = n3.toInt(&ok, 16);
    QString n2(index.at(2)); b2 = n2.toInt(&ok, 16);
    QString n1(index.at(1)); b1 = n1.toInt(&ok, 16);
    QString n0(index.at(0)); b0 = n0.toInt(&ok, 16);

    QByteArray new_value;
    new_value.append(char(b0)).append(char(b1)).append(char(b2)).append(char(b3));

    return new_value;
}

QByteArray sysxWriter::altTuning(QString mode)
{
    QByteArray tuning;
    if     (mode == "00"){tuning = QByteArray::fromHex("1E1E1F20201E");}  // Open D
    else if(mode == "01"){tuning = QByteArray::fromHex("202021222220");}  // Open E
    else if(mode == "02"){tuning = QByteArray::fromHex("1E2020201E1E");}  // Open G
    else if(mode == "03"){tuning = QByteArray::fromHex("202222222020");}  // Open A
    else if(mode == "04"){tuning = QByteArray::fromHex("20202020201E");}  // Drop D
    else if(mode == "05"){tuning = QByteArray::fromHex("1F1F1F1F1F1D");}  // Drop Db
    else if(mode == "06"){tuning = QByteArray::fromHex("1E1E1E1E1E1C");}  // Drop C
    else if(mode == "07"){tuning = QByteArray::fromHex("1D1D1D1D1D1B");}  // Drop B
    else if(mode == "08"){tuning = QByteArray::fromHex("1C1C1C1C1C1A");}  // Drop Bb
    else if(mode == "09"){tuning = QByteArray::fromHex("1B1B1B1B1B19");}  // Drop A
    else if(mode == "0A"){tuning = QByteArray::fromHex("1E1E2020201E");}  // D-Modal
    else if(mode == "0B"){tuning = QByteArray::fromHex("20202C2C2C2C");}  // Nashville
    else                 {tuning = QByteArray::fromHex("202020202020");}; // Standard
    return tuning;
}

void sysxWriter::convert_GT1000_TSL_to_default(int patch)
{
    Q_UNUSED(patch);
    int mode = 1;
    this->GT1000translate = new tsl_GT1000_translation(this->data, SY1000_default, mode, fileName);
    QByteArray translatedData;
    translatedData.append(GT1000translate->convertFromTSL(0));

    if(translatedData.size()>2)
    {
        SysxIO *sysxIO = SysxIO::Instance();
        sysxIO->setFileSource("Structure", translatedData);
        sysxIO->setFileName(this->fileName);
        sysxIO->fileSource = sysxIO->getFileSource();
    };
}

void sysxWriter::convert_GP10_TSL_to_default(int patch)
{
    Q_UNUSED(patch);
    Preferences *preferences = Preferences::Instance();
    int mode = 1;
    this->GP10translate = new tsl_GP10_translation(this->data, this->SY1000_default, mode, fileName);
    QByteArray translatedData;

    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        this->temp.clear();
        this->temp.append(GP10translate->convertFromTSL(0));
        hex_G_to_hex_B();
        translatedData.append(this->SY1000_default);
    }
    else
    {
        translatedData.append(GP10translate->convertFromTSL(0));
    };

    if(translatedData.size()>2)
    {
        SysxIO *sysxIO = SysxIO::Instance();
        sysxIO->setFileSource("Structure", translatedData);
        sysxIO->setFileName(this->fileName);
        sysxIO->fileSource = sysxIO->getFileSource();
    };
}

void sysxWriter::convert_VG99_VB99_to_default(int patch)
{
    int mode = 1;
    this->VG99_VB99translate = new tsl_VG99_VB99_translation(this->data, SY1000_default, mode, fileName);
    QByteArray translatedData;
    translatedData = VG99_VB99translate->convertFromVG99default_to_SY1000default(patch);

    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        temp.clear();
        temp = translatedData;
        hex_G_to_hex_B();
        translatedData = SY1000_default;
    };

    if(translatedData.size()>2)
    {
        SysxIO *sysxIO = SysxIO::Instance();
        sysxIO->setFileSource("Structure", translatedData);
        sysxIO->setFileName(this->fileName);
        sysxIO->fileSource = sysxIO->getFileSource();
    };
}

QByteArray sysxWriter::convert_value(QByteArray value, QString hex2, QString hex3)
{
    bool ok;
    int v = value.toHex().toInt(&ok, 16);
    MidiTable *midiTable = MidiTable::Instance();
    Midi items;
    items = midiTable->getMidiMap("Tables", "00", hex2, hex3);
    QString desc = items.level.at(v).desc;
    QByteArray value2 = QByteArray::fromHex(desc.toLatin1()).toUpper();

    return value2;
}

//*****************************************************************************  Guitar tsl to default Bass **********
void sysxWriter::TSL_G_to_hex_G()
{
    QByteArray t;
    t = GetJsonString("User_patch%common", a);
    SY1000_default.replace(12, 128, t.mid(0, 128));
    SY1000_default.replace(154, 27, t.mid(128, 27));
    SY1000_default_replace(211, 27, GetJsonString("User_patch%led", a));
    SY1000_default_replace(252, 43, GetJsonString("User_patch%assign(1)", a));
    SY1000_default_replace(316, 43, GetJsonString("User_patch%assign(2)", a));
    SY1000_default_replace(373, 43, GetJsonString("User_patch%assign(3)", a));
    SY1000_default_replace(437, 43, GetJsonString("User_patch%assign(4)", a));
    SY1000_default_replace(494, 43, GetJsonString("User_patch%assign(5)", a));
    SY1000_default_replace(558, 43, GetJsonString("User_patch%assign(6)", a));
    SY1000_default_replace(615, 43, GetJsonString("User_patch%assign(7)", a));
    SY1000_default_replace(679, 43, GetJsonString("User_patch%assign(8)", a));
    SY1000_default_replace(736, 43, GetJsonString("User_patch%assign(9)", a));
    SY1000_default_replace(800, 43, GetJsonString("User_patch%assign(10)", a));
    SY1000_default_replace(857, 43, GetJsonString("User_patch%assign(11)", a));
    SY1000_default_replace(921, 43, GetJsonString("User_patch%assign(12)", a));
    SY1000_default_replace(978, 43, GetJsonString("User_patch%assign(13)", a));
    SY1000_default_replace(1042, 43, GetJsonString("User_patch%assign(14)", a));
    SY1000_default_replace(1099, 43, GetJsonString("User_patch%assign(15)", a));
    SY1000_default_replace(1163, 43, GetJsonString("User_patch%assign(16)", a));
    SY1000_default_replace(1220, 103, GetJsonString("User_patch%efct", a));
    SY1000_default_replace(1337, 7, GetJsonString(  "User_patch%gtr2midi", a));
    SY1000_default_replace(1358, 18, GetJsonString("User_patch%inst(1)%inst", a));
    t = GetJsonString("User_patch%inst(1)%instDynaSynth", a);
    SY1000_default.replace(1390, 128, t.mid(0, 128));
    SY1000_default.replace(1532, 78, t.mid(128, 78));
    SY1000_default_replace(1624, 65, GetJsonString("User_patch%inst(1)%instOscSynth", a));
    SY1000_default_replace(1703, 21, GetJsonString("User_patch%inst(1)%instGr300", a));
    SY1000_default_replace(1738, 45, GetJsonString("User_patch%inst(1)%instEGtr", a));
    SY1000_default_replace(1797, 47, GetJsonString("User_patch%inst(1)%instAcoustic", a));
    SY1000_default_replace(1858, 46, GetJsonString("User_patch%inst(1)%instEBass", a));
    SY1000_default_replace(1918, 37, GetJsonString("User_patch%inst(1)%instVioGtr", a));
    SY1000_default_replace(1969, 19, GetJsonString("User_patch%inst(1)%instPolyFx", a));
    SY1000_default_replace(2002, 48, GetJsonString("User_patch%inst(1)%instAltTune", a));
    SY1000_default_replace(2064, 18, GetJsonString("User_patch%inst(2)%inst", a));
    t = GetJsonString("User_patch%inst(2)%instDynaSynth", a);
    SY1000_default.replace(2096, 128, t.mid(0, 128));
    SY1000_default.replace(2238, 78, t.mid(128, 78));
    SY1000_default_replace(2330, 65, GetJsonString("User_patch%inst(2)%instOscSynth", a));
    SY1000_default_replace(2409, 21, GetJsonString("User_patch%inst(2)%instGr300", a));
    SY1000_default_replace(2444, 45, GetJsonString("User_patch%inst(2)%instEGtr", a));
    SY1000_default_replace(2503, 47, GetJsonString("User_patch%inst(2)%instAcoustic", a));
    SY1000_default_replace(2564, 46, GetJsonString("User_patch%inst(2)%instEBass", a));
    SY1000_default_replace(2624, 37, GetJsonString("User_patch%inst(2)%instVioGtr", a));
    SY1000_default_replace(2675, 19, GetJsonString("User_patch%inst(2)%instPolyFx", a));
    SY1000_default_replace(2708, 48, GetJsonString("User_patch%inst(2)%instAltTune", a));
    SY1000_default_replace(2770, 18, GetJsonString("User_patch%inst(3)%inst", a));
    t = GetJsonString("User_patch%inst(3)%instDynaSynth", a);
    SY1000_default.replace(2802, 128, t.mid(0, 128));
    SY1000_default.replace(2944, 78, t.mid(128, 78));
    SY1000_default_replace(3036, 65, GetJsonString("User_patch%inst(3)%instOscSynth", a));
    SY1000_default_replace(3115, 21, GetJsonString("User_patch%inst(3)%instGr300", a));
    SY1000_default_replace(3150, 45, GetJsonString("User_patch%inst(3)%instEGtr", a));
    SY1000_default_replace(3209, 47, GetJsonString("User_patch%inst(3)%instAcoustic", a));
    SY1000_default_replace(3270, 46, GetJsonString("User_patch%inst(3)%instEBass", a));
    SY1000_default_replace(3330, 37, GetJsonString("User_patch%inst(3)%instVioGtr", a));
    SY1000_default_replace(3381, 19, GetJsonString("User_patch%inst(3)%instPolyFx", a));
    SY1000_default_replace(3414, 48, GetJsonString("User_patch%inst(3)%instAltTune", a));

    SY1000_default_replace(3476, 7, GetJsonString("User_patch%comp", a));
    SY1000_default_replace(3497, 9, GetJsonString("User_patch%dist", a));
    SY1000_default_replace(3520, 20, GetJsonString("User_patch%amp", a));
    SY1000_default_replace(3554, 4, GetJsonString("User_patch%ns", a));
    SY1000_default_replace(3572, 12, GetJsonString("User_patch%eq(1)", a));
    SY1000_default_replace(3598, 12, GetJsonString("User_patch%eq(2)", a));
    SY1000_default_replace(3624, 10, GetJsonString("User_patch%delay(1)", a));
    SY1000_default_replace(3648, 10, GetJsonString("User_patch%delay(2)", a));
    SY1000_default_replace(3672, 33, GetJsonString("User_patch%mstDelay", a));
    SY1000_default_replace(3719, 25, GetJsonString("User_patch%chorus", a));

    SY1000_default_replace(3758, 2, GetJsonString("User_patch%fx(1)%fx", a));
    SY1000_default_replace(3774, 4, GetJsonString("User_patch%fx(1)%fxAcReso", a));
    SY1000_default_replace(3792, 8, GetJsonString("User_patch%fx(1)%fxAWah", a));
    SY1000_default_replace(3814, 25, GetJsonString("User_patch%fx(1)%fxChorus", a));
    SY1000_default_replace(3853, 3, GetJsonString("User_patch%fx(1)%fxCVibe", a));
    SY1000_default_replace(3870, 7, GetJsonString("User_patch%fx(1)%fxComp", a));
    SY1000_default_replace(3891, 7, GetJsonString("User_patch%fx(1)%fxDefretter", a));
    SY1000_default_replace(3912, 5, GetJsonString("User_patch%fx(1)%fxDefretterBass", a));
    SY1000_default_replace(3931, 33, GetJsonString("User_patch%fx(1)%fxDelay", a));
    SY1000_default_replace(3978, 9, GetJsonString("User_patch%fx(1)%fxFlanger", a));
    SY1000_default_replace(4001, 9, GetJsonString("User_patch%fx(1)%fxFlangerBass", a));
    SY1000_default_replace(4024, 13, GetJsonString("User_patch%fx(1)%fxFv", a));
    SY1000_default_replace(4051, 8, GetJsonString("User_patch%fx(1)%fxGEq", a));
    SY1000_default_replace(4073, 39, GetJsonString("User_patch%fx(1)%fxHarmonist", a));
    SY1000_default_replace(4126, 8, GetJsonString("User_patch%fx(1)%fxHumanizer", a));
    SY1000_default_replace(4148, 4, GetJsonString("User_patch%fx(1)%fxIsolator", a));
    SY1000_default_replace(4166, 6, GetJsonString("User_patch%fx(1)%fxLimiter", a));
    SY1000_default_replace(4186, 3, GetJsonString("User_patch%fx(1)%fxLofi", a));
    SY1000_default_replace(4203, 3, GetJsonString("User_patch%fx(1)%fxOctave", a));
    SY1000_default_replace(4220, 3, GetJsonString("User_patch%fx(1)%fxOctaveBass", a));
    SY1000_default_replace(4237, 6, GetJsonString("User_patch%fx(1)%fxPan", a));
    SY1000_default_replace(4257, 12, GetJsonString("User_patch%fx(1)%fxPEq", a));
    SY1000_default_replace(4283, 7, GetJsonString("User_patch%fx(1)%fxPedalBend", a));
    SY1000_default_replace(4304, 8, GetJsonString("User_patch%fx(1)%fxPhaser", a));
    SY1000_default_replace(4326, 19, GetJsonString("User_patch%fx(1)%fxPitchShift", a));
    SY1000_default_replace(4359, 14, GetJsonString("User_patch%fx(1)%fxReverb", a));
    SY1000_default_replace(4387, 6, GetJsonString("User_patch%fx(1)%fxRingMod", a));
    SY1000_default_replace(4407, 8, GetJsonString("User_patch%fx(1)%fxRotary", a));
    SY1000_default_replace(4429, 7, GetJsonString("User_patch%fx(1)%fxSitarSim", a));
    SY1000_default_replace(4450, 7, GetJsonString("User_patch%fx(1)%fxSlicer", a));
    SY1000_default_replace(4471, 3, GetJsonString("User_patch%fx(1)%fxSlowGear", a));
    SY1000_default_replace(4488, 3, GetJsonString("User_patch%fx(1)%fxSlowGearBass", a));
    SY1000_default_replace(4505, 3, GetJsonString("User_patch%fx(1)%fxSoundHold", a));
    SY1000_default_replace(4522, 8, GetJsonString("User_patch%fx(1)%fxTWah", a));
    SY1000_default_replace(4544, 8, GetJsonString("User_patch%fx(1)%fxTWahBass", a));
    SY1000_default_replace(4566, 5, GetJsonString("User_patch%fx(1)%fxTremolo", a));
    SY1000_default_replace(4585, 6, GetJsonString("User_patch%fx(1)%fxVibrato", a));
    SY1000_default_replace(4605, 15, GetJsonString("User_patch%fx(1)%fxWah", a));

    SY1000_default_replace(4634, 2, GetJsonString("User_patch%fx(2)%fx", a));
    SY1000_default_replace(4650, 4, GetJsonString("User_patch%fx(2)%fxAcReso", a));
    SY1000_default_replace(4668, 8, GetJsonString("User_patch%fx(2)%fxAWah", a));
    SY1000_default_replace(4690, 25, GetJsonString("User_patch%fx(2)%fxChorus", a));
    SY1000_default_replace(4729, 3, GetJsonString("User_patch%fx(2)%fxCVibe", a));
    SY1000_default_replace(4746, 7, GetJsonString("User_patch%fx(2)%fxComp", a));
    SY1000_default_replace(4767, 7, GetJsonString("User_patch%fx(2)%fxDefretter", a));
    SY1000_default_replace(4788, 5, GetJsonString("User_patch%fx(2)%fxDefretterBass", a));
    SY1000_default_replace(4807, 33, GetJsonString("User_patch%fx(2)%fxDelay", a));
    SY1000_default_replace(4854, 9, GetJsonString("User_patch%fx(2)%fxFlanger", a));
    SY1000_default_replace(4877, 9, GetJsonString("User_patch%fx(2)%fxFlangerBass", a));
    SY1000_default_replace(4900, 13, GetJsonString("User_patch%fx(2)%fxFv", a));
    SY1000_default_replace(4927, 8, GetJsonString("User_patch%fx(2)%fxGEq", a));
    SY1000_default_replace(4949, 39, GetJsonString("User_patch%fx(2)%fxHarmonist", a));
    SY1000_default_replace(5002, 8, GetJsonString("User_patch%fx(2)%fxHumanizer", a));
    SY1000_default_replace(5024, 4, GetJsonString("User_patch%fx(2)%fxIsolator", a));
    SY1000_default_replace(5042, 6, GetJsonString("User_patch%fx(2)%fxLimiter", a));
    SY1000_default_replace(5062, 3, GetJsonString("User_patch%fx(2)%fxLofi", a));
    SY1000_default_replace(5079, 3, GetJsonString("User_patch%fx(2)%fxOctave", a));
    SY1000_default_replace(5096, 3, GetJsonString("User_patch%fx(2)%fxOctaveBass", a));
    SY1000_default_replace(5113, 6, GetJsonString("User_patch%fx(2)%fxPan", a));
    SY1000_default_replace(5133, 12, GetJsonString("User_patch%fx(2)%fxPEq", a));
    SY1000_default_replace(5159, 7, GetJsonString("User_patch%fx(2)%fxPedalBend", a));
    SY1000_default_replace(5180, 8, GetJsonString("User_patch%fx(2)%fxPhaser", a));
    SY1000_default_replace(5202, 19, GetJsonString("User_patch%fx(2)%fxPitchShift", a));
    SY1000_default_replace(5235, 14, GetJsonString("User_patch%fx(2)%fxReverb", a));
    SY1000_default_replace(5263, 6, GetJsonString("User_patch%fx(2)%fxRingMod", a));
    SY1000_default_replace(5283, 8, GetJsonString("User_patch%fx(2)%fxRotary", a));
    SY1000_default_replace(5305, 7, GetJsonString("User_patch%fx(2)%fxSitarSim", a));
    SY1000_default_replace(5326, 7, GetJsonString("User_patch%fx(2)%fxSlicer", a));
    SY1000_default_replace(5347, 3, GetJsonString("User_patch%fx(2)%fxSlowGear", a));
    SY1000_default_replace(5364, 3, GetJsonString("User_patch%fx(2)%fxSlowGearBass", a));
    SY1000_default_replace(5381, 3, GetJsonString("User_patch%fx(2)%fxSoundHold", a));
    SY1000_default_replace(5398, 8, GetJsonString("User_patch%fx(2)%fxTWah", a));
    SY1000_default_replace(5420, 8, GetJsonString("User_patch%fx(2)%fxTWahBass", a));
    SY1000_default_replace(5442, 5, GetJsonString("User_patch%fx(2)%fxTremolo", a));
    SY1000_default_replace(5461, 6, GetJsonString("User_patch%fx(2)%fxVibrato", a));
    SY1000_default_replace(5481, 15, GetJsonString("User_patch%fx(2)%fxWah", a));

    SY1000_default_replace(5510, 2, GetJsonString("User_patch%fx(3)%fx", a)); //+1752
    SY1000_default_replace(5526, 4, GetJsonString("User_patch%fx(3)%fxAcReso", a));
    SY1000_default_replace(5544, 8, GetJsonString("User_patch%fx(3)%fxAWah", a));
    SY1000_default_replace(5566, 25, GetJsonString("User_patch%fx(3)%fxChorus", a));
    SY1000_default_replace(5605, 3, GetJsonString("User_patch%fx(3)%fxCVibe", a));
    SY1000_default_replace(5622, 7, GetJsonString("User_patch%fx(3)%fxComp", a));
    SY1000_default_replace(5643, 7, GetJsonString("User_patch%fx(3)%fxDefretter", a));
    SY1000_default_replace(5664, 5, GetJsonString("User_patch%fx(3)%fxDefretterBass", a));
    SY1000_default_replace(5683, 33, GetJsonString("User_patch%fx(3)%fxDelay", a));
    SY1000_default_replace(5730, 9, GetJsonString("User_patch%fx(3)%fxFlanger", a));
    SY1000_default_replace(5753, 9, GetJsonString("User_patch%fx(3)%fxFlangerBass", a));
    SY1000_default_replace(5776, 13, GetJsonString("User_patch%fx(3)%fxFv", a));
    SY1000_default_replace(5803, 8, GetJsonString("User_patch%fx(3)%fxGEq", a));
    SY1000_default_replace(5825, 39, GetJsonString("User_patch%fx(3)%fxHarmonist", a));
    SY1000_default_replace(5878, 8, GetJsonString("User_patch%fx(3)%fxHumanizer", a));
    SY1000_default_replace(5900, 4, GetJsonString("User_patch%fx(3)%fxIsolator", a));
    SY1000_default_replace(5918, 6, GetJsonString("User_patch%fx(3)%fxLimiter", a));
    SY1000_default_replace(5938, 3, GetJsonString("User_patch%fx(3)%fxLofi", a));
    SY1000_default_replace(5955, 3, GetJsonString("User_patch%fx(3)%fxOctave", a));
    SY1000_default_replace(5972, 3, GetJsonString("User_patch%fx(3)%fxOctaveBass", a));
    SY1000_default_replace(5989, 6, GetJsonString("User_patch%fx(3)%fxPan", a));
    SY1000_default_replace(6009, 12, GetJsonString("User_patch%fx(3)%fxPEq", a));
    SY1000_default_replace(6035, 7, GetJsonString("User_patch%fx(3)%fxPedalBend", a));
    SY1000_default_replace(6056, 8, GetJsonString("User_patch%fx(3)%fxPhaser", a));
    SY1000_default_replace(6078, 19, GetJsonString("User_patch%fx(3)%fxPitchShift", a));
    SY1000_default_replace(6111, 14, GetJsonString("User_patch%fx(3)%fxReverb", a));
    SY1000_default_replace(6139, 6, GetJsonString("User_patch%fx(3)%fxRingMod", a));
    SY1000_default_replace(6159, 8, GetJsonString("User_patch%fx(3)%fxRotary", a));
    SY1000_default_replace(6181, 7, GetJsonString("User_patch%fx(3)%fxSitarSim", a));
    SY1000_default_replace(6202, 7, GetJsonString("User_patch%fx(3)%fxSlicer", a));
    SY1000_default_replace(6223, 3, GetJsonString("User_patch%fx(3)%fxSlowGear", a));
    SY1000_default_replace(6240, 3, GetJsonString("User_patch%fx(3)%fxSlowGearBass", a));
    SY1000_default_replace(6257, 3, GetJsonString("User_patch%fx(3)%fxSoundHold", a));
    SY1000_default_replace(6274, 8, GetJsonString("User_patch%fx(3)%fxTWah", a));
    SY1000_default_replace(6296, 8, GetJsonString("User_patch%fx(3)%fxTWahBass", a));
    SY1000_default_replace(6318, 5, GetJsonString("User_patch%fx(3)%fxTremolo", a));
    SY1000_default_replace(6337, 6, GetJsonString("User_patch%fx(3)%fxVibrato", a));
    SY1000_default_replace(6357, 15, GetJsonString("User_patch%fx(3)%fxWah", a));

    SY1000_default_replace(6386, 14, GetJsonString("User_patch%reverb", a));
}

//*****************************************************************************  Bass tsl to default Guitar **********
void sysxWriter::TSL_B_to_hex_G()
{
    QByteArray t;
    t = GetJsonString("UserBass_patch%common", a);
    SY1000_default.replace(12, 128, t.mid(0, 128));
    SY1000_default.replace(154, 27, t.mid(128, 27));
    SY1000_default_replace(211, 27, GetJsonString("UserBass_patch%led", a));
    SY1000_default_replace(252, 43, GetJsonString("UserBass_patch%assign(1)", a));
    SY1000_default_replace(253, 4,  convert_Assign(SY1000_default.mid(253, 4), 0));
    SY1000_default_replace(316, 43, GetJsonString("UserBass_patch%assign(2)", a));
    SY1000_default_replace(317, 4,  convert_Assign(SY1000_default.mid(317, 4), 0));
    SY1000_default_replace(373, 43, GetJsonString("UserBass_patch%assign(3)", a));
    SY1000_default_replace(374, 4,  convert_Assign(SY1000_default.mid(374, 4), 0));
    SY1000_default_replace(437, 43, GetJsonString("UserBass_patch%assign(4)", a));
    SY1000_default_replace(438, 4,  convert_Assign(SY1000_default.mid(438, 4), 0));
    SY1000_default_replace(494, 43, GetJsonString("UserBass_patch%assign(5)", a));
    SY1000_default_replace(495, 4,  convert_Assign(SY1000_default.mid(495, 4), 0));
    SY1000_default_replace(558, 43, GetJsonString("UserBass_patch%assign(6)", a));
    SY1000_default_replace(559, 4,  convert_Assign(SY1000_default.mid(559, 4), 0));
    SY1000_default_replace(615, 43, GetJsonString("UserBass_patch%assign(7)", a));
    SY1000_default_replace(616, 4,  convert_Assign(SY1000_default.mid(616, 4), 0));
    SY1000_default_replace(679, 43, GetJsonString("UserBass_patch%assign(8)", a));
    SY1000_default_replace(680, 4,  convert_Assign(SY1000_default.mid(680, 4), 0));
    SY1000_default_replace(736, 43, GetJsonString("UserBass_patch%assign(9)", a));
    SY1000_default_replace(737, 4,  convert_Assign(SY1000_default.mid(737, 4), 0));
    SY1000_default_replace(800, 43, GetJsonString("UserBass_patch%assign(10)", a));
    SY1000_default_replace(801, 4,  convert_Assign(SY1000_default.mid(801, 4), 0));
    SY1000_default_replace(857, 43, GetJsonString("UserBass_patch%assign(11)", a));
    SY1000_default_replace(858, 4,  convert_Assign(SY1000_default.mid(858, 4), 0));
    SY1000_default_replace(921, 43, GetJsonString("UserBass_patch%assign(12)", a));
    SY1000_default_replace(922, 4,  convert_Assign(SY1000_default.mid(922, 4), 0));
    SY1000_default_replace(978, 43, GetJsonString("UserBass_patch%assign(13)", a));
    SY1000_default_replace(979, 4,  convert_Assign(SY1000_default.mid(979, 4), 0));
    SY1000_default_replace(1042, 43, GetJsonString("UserBass_patch%assign(14)", a));
    SY1000_default_replace(1043, 4,  convert_Assign(SY1000_default.mid(1043, 4), 0));
    SY1000_default_replace(1099, 43, GetJsonString("UserBass_patch%assign(15)", a));
    SY1000_default_replace(1100, 4,  convert_Assign(SY1000_default.mid(1100, 4), 0));
    SY1000_default_replace(1163, 43, GetJsonString("UserBass_patch%assign(16)", a));
    SY1000_default_replace(1164, 4,  convert_Assign(SY1000_default.mid(1164, 4), 0));
    SY1000_default_replace(1220, 103, GetJsonString("UserBass_patch%efct", a));
    SY1000_default_replace(1337, 7, GetJsonString(  "UserBass_patch%gtr2midi", a));
    SY1000_default_replace(1358, 18, GetJsonString("UserBass_patch%inst(1)%inst", a));
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
    SY1000_default_replace(3414, 48, GetJsonString("UserBass_patch%inst(3)%instAltTune", a));

    SY1000_default_replace(3476, 7, GetJsonString("UserBass_patch%comp", a));
    SY1000_default_replace(3497, 9, GetJsonString("UserBass_patch%dist", a));
    SY1000_default_replace(3520, 20, GetJsonString("UserBass_patch%amp", a));
    SY1000_default_replace(3554, 4, GetJsonString("UserBass_patch%ns", a));
    SY1000_default_replace(3572, 12, GetJsonString("UserBass_patch%eq(1)", a));
    SY1000_default_replace(3598, 12, GetJsonString("UserBass_patch%eq(2)", a));
    SY1000_default_replace(3624, 10, GetJsonString("UserBass_patch%delay(1)", a));
    SY1000_default_replace(3648, 10, GetJsonString("UserBass_patch%delay(2)", a));
    SY1000_default_replace(3672, 33, GetJsonString("UserBass_patch%mstDelay", a));
    SY1000_default_replace(3719, 25, GetJsonString("UserBass_patch%chorus", a));

    SY1000_default_replace(3758, 2, GetJsonString("UserBass_patch%fx(1)%fx", a));
    SY1000_default_replace(3774, 4, GetJsonString("UserBass_patch%fx(1)%fxAcReso", a));
    SY1000_default_replace(3792, 8, GetJsonString("UserBass_patch%fx(1)%fxAWah", a));
    SY1000_default_replace(3814, 25, GetJsonString("UserBass_patch%fx(1)%fxChorus", a));
    SY1000_default_replace(3853, 3, GetJsonString("UserBass_patch%fx(1)%fxCVibe", a));
    SY1000_default_replace(3870, 7, GetJsonString("UserBass_patch%fx(1)%fxComp", a));
    SY1000_default_replace(3891, 7, GetJsonString("UserBass_patch%fx(1)%fxDefretter", a));
    SY1000_default_replace(3912, 5, GetJsonString("UserBass_patch%fx(1)%fxDefretterBass", a));
    SY1000_default_replace(3931, 33, GetJsonString("UserBass_patch%fx(1)%fxDelay", a));
    SY1000_default_replace(3978, 9, GetJsonString("UserBass_patch%fx(1)%fxFlanger", a));
    SY1000_default_replace(4001, 9, GetJsonString("UserBass_patch%fx(1)%fxFlangerBass", a));
    SY1000_default_replace(4024, 13, GetJsonString("UserBass_patch%fx(1)%fxFv", a));
    SY1000_default_replace(4051, 8, GetJsonString("UserBass_patch%fx(1)%fxGEq", a));
    SY1000_default_replace(4073, 39, GetJsonString("UserBass_patch%fx(1)%fxHarmonist", a));
    SY1000_default_replace(4126, 8, GetJsonString("UserBass_patch%fx(1)%fxHumanizer", a));
    SY1000_default_replace(4148, 4, GetJsonString("UserBass_patch%fx(1)%fxIsolator", a));
    SY1000_default_replace(4166, 6, GetJsonString("UserBass_patch%fx(1)%fxLimiter", a));
    SY1000_default_replace(4186, 3, GetJsonString("UserBass_patch%fx(1)%fxLofi", a));
    SY1000_default_replace(4203, 3, GetJsonString("UserBass_patch%fx(1)%fxOctave", a));
    SY1000_default_replace(4220, 3, GetJsonString("UserBass_patch%fx(1)%fxOctaveBass", a));
    SY1000_default_replace(4237, 6, GetJsonString("UserBass_patch%fx(1)%fxPan", a));
    SY1000_default_replace(4257, 12, GetJsonString("UserBass_patch%fx(1)%fxPEq", a));
    SY1000_default_replace(4283, 7, GetJsonString("UserBass_patch%fx(1)%fxPedalBend", a));
    SY1000_default_replace(4304, 8, GetJsonString("UserBass_patch%fx(1)%fxPhaser", a));
    SY1000_default_replace(4326, 19, GetJsonString("UserBass_patch%fx(1)%fxPitchShift", a));
    SY1000_default_replace(4359, 14, GetJsonString("UserBass_patch%fx(1)%fxReverb", a));
    SY1000_default_replace(4387, 6, GetJsonString("UserBass_patch%fx(1)%fxRingMod", a));
    SY1000_default_replace(4407, 8, GetJsonString("UserBass_patch%fx(1)%fxRotary", a));
    SY1000_default_replace(4429, 7, GetJsonString("UserBass_patch%fx(1)%fxSitarSim", a));
    SY1000_default_replace(4450, 7, GetJsonString("UserBass_patch%fx(1)%fxSlicer", a));
    SY1000_default_replace(4471, 3, GetJsonString("UserBass_patch%fx(1)%fxSlowGear", a));
    SY1000_default_replace(4488, 3, GetJsonString("UserBass_patch%fx(1)%fxSlowGearBass", a));
    SY1000_default_replace(4505, 3, GetJsonString("UserBass_patch%fx(1)%fxSoundHold", a));
    SY1000_default_replace(4522, 8, GetJsonString("UserBass_patch%fx(1)%fxTWah", a));
    SY1000_default_replace(4544, 8, GetJsonString("UserBass_patch%fx(1)%fxTWahBass", a));
    SY1000_default_replace(4566, 5, GetJsonString("UserBass_patch%fx(1)%fxTremolo", a));
    SY1000_default_replace(4585, 6, GetJsonString("UserBass_patch%fx(1)%fxVibrato", a));
    SY1000_default_replace(4605, 15, GetJsonString("UserBass_patch%fx(1)%fxWah", a));

    SY1000_default_replace(4634, 2, GetJsonString("UserBass_patch%fx(2)%fx", a));
    SY1000_default_replace(4650, 4, GetJsonString("UserBass_patch%fx(2)%fxAcReso", a));
    SY1000_default_replace(4668, 8, GetJsonString("UserBass_patch%fx(2)%fxAWah", a));
    SY1000_default_replace(4690, 25, GetJsonString("UserBass_patch%fx(2)%fxChorus", a));
    SY1000_default_replace(4729, 3, GetJsonString("UserBass_patch%fx(2)%fxCVibe", a));
    SY1000_default_replace(4746, 7, GetJsonString("UserBass_patch%fx(2)%fxComp", a));
    SY1000_default_replace(4767, 7, GetJsonString("UserBass_patch%fx(2)%fxDefretter", a));
    SY1000_default_replace(4788, 5, GetJsonString("UserBass_patch%fx(2)%fxDefretterBass", a));
    SY1000_default_replace(4807, 33, GetJsonString("UserBass_patch%fx(2)%fxDelay", a));
    SY1000_default_replace(4854, 9, GetJsonString("UserBass_patch%fx(2)%fxFlanger", a));
    SY1000_default_replace(4877, 9, GetJsonString("UserBass_patch%fx(2)%fxFlangerBass", a));
    SY1000_default_replace(4900, 13, GetJsonString("UserBass_patch%fx(2)%fxFv", a));
    SY1000_default_replace(4927, 8, GetJsonString("UserBass_patch%fx(2)%fxGEq", a));
    SY1000_default_replace(4949, 39, GetJsonString("UserBass_patch%fx(2)%fxHarmonist", a));
    SY1000_default_replace(5002, 8, GetJsonString("UserBass_patch%fx(2)%fxHumanizer", a));
    SY1000_default_replace(5024, 4, GetJsonString("UserBass_patch%fx(2)%fxIsolator", a));
    SY1000_default_replace(5042, 6, GetJsonString("UserBass_patch%fx(2)%fxLimiter", a));
    SY1000_default_replace(5062, 3, GetJsonString("UserBass_patch%fx(2)%fxLofi", a));
    SY1000_default_replace(5079, 3, GetJsonString("UserBass_patch%fx(2)%fxOctave", a));
    SY1000_default_replace(5096, 3, GetJsonString("UserBass_patch%fx(2)%fxOctaveBass", a));
    SY1000_default_replace(5113, 6, GetJsonString("UserBass_patch%fx(2)%fxPan", a));
    SY1000_default_replace(5133, 12, GetJsonString("UserBass_patch%fx(2)%fxPEq", a));
    SY1000_default_replace(5159, 7, GetJsonString("UserBass_patch%fx(2)%fxPedalBend", a));
    SY1000_default_replace(5180, 8, GetJsonString("UserBass_patch%fx(2)%fxPhaser", a));
    SY1000_default_replace(5202, 19, GetJsonString("UserBass_patch%fx(2)%fxPitchShift", a));
    SY1000_default_replace(5235, 14, GetJsonString("UserBass_patch%fx(2)%fxReverb", a));
    SY1000_default_replace(5263, 6, GetJsonString("UserBass_patch%fx(2)%fxRingMod", a));
    SY1000_default_replace(5283, 8, GetJsonString("UserBass_patch%fx(2)%fxRotary", a));
    SY1000_default_replace(5305, 7, GetJsonString("UserBass_patch%fx(2)%fxSitarSim", a));
    SY1000_default_replace(5326, 7, GetJsonString("UserBass_patch%fx(2)%fxSlicer", a));
    SY1000_default_replace(5347, 3, GetJsonString("UserBass_patch%fx(2)%fxSlowGear", a));
    SY1000_default_replace(5364, 3, GetJsonString("UserBass_patch%fx(2)%fxSlowGearBass", a));
    SY1000_default_replace(5381, 3, GetJsonString("UserBass_patch%fx(2)%fxSoundHold", a));
    SY1000_default_replace(5398, 8, GetJsonString("UserBass_patch%fx(2)%fxTWah", a));
    SY1000_default_replace(5420, 8, GetJsonString("UserBass_patch%fx(2)%fxTWahBass", a));
    SY1000_default_replace(5442, 5, GetJsonString("UserBass_patch%fx(2)%fxTremolo", a));
    SY1000_default_replace(5461, 6, GetJsonString("UserBass_patch%fx(2)%fxVibrato", a));
    SY1000_default_replace(5481, 15, GetJsonString("UserBass_patch%fx(2)%fxWah", a));

    SY1000_default_replace(5510, 2, GetJsonString("UserBass_patch%fx(3)%fx", a)); //+1752
    SY1000_default_replace(5526, 4, GetJsonString("UserBass_patch%fx(3)%fxAcReso", a));
    SY1000_default_replace(5544, 8, GetJsonString("UserBass_patch%fx(3)%fxAWah", a));
    SY1000_default_replace(5566, 25, GetJsonString("UserBass_patch%fx(3)%fxChorus", a));
    SY1000_default_replace(5605, 3, GetJsonString("UserBass_patch%fx(3)%fxCVibe", a));
    SY1000_default_replace(5622, 7, GetJsonString("UserBass_patch%fx(3)%fxComp", a));
    SY1000_default_replace(5643, 7, GetJsonString("UserBass_patch%fx(3)%fxDefretter", a));
    SY1000_default_replace(5664, 5, GetJsonString("UserBass_patch%fx(3)%fxDefretterBass", a));
    SY1000_default_replace(5683, 33, GetJsonString("UserBass_patch%fx(3)%fxDelay", a));
    SY1000_default_replace(5730, 9, GetJsonString("UserBass_patch%fx(3)%fxFlanger", a));
    SY1000_default_replace(5753, 9, GetJsonString("UserBass_patch%fx(3)%fxFlangerBass", a));
    SY1000_default_replace(5776, 13, GetJsonString("UserBass_patch%fx(3)%fxFv", a));
    SY1000_default_replace(5803, 8, GetJsonString("UserBass_patch%fx(3)%fxGEq", a));
    SY1000_default_replace(5825, 39, GetJsonString("UserBass_patch%fx(3)%fxHarmonist", a));
    SY1000_default_replace(5878, 8, GetJsonString("UserBass_patch%fx(3)%fxHumanizer", a));
    SY1000_default_replace(5900, 4, GetJsonString("UserBass_patch%fx(3)%fxIsolator", a));
    SY1000_default_replace(5918, 6, GetJsonString("UserBass_patch%fx(3)%fxLimiter", a));
    SY1000_default_replace(5938, 3, GetJsonString("UserBass_patch%fx(3)%fxLofi", a));
    SY1000_default_replace(5955, 3, GetJsonString("UserBass_patch%fx(3)%fxOctave", a));
    SY1000_default_replace(5972, 3, GetJsonString("UserBass_patch%fx(3)%fxOctaveBass", a));
    SY1000_default_replace(5989, 6, GetJsonString("UserBass_patch%fx(3)%fxPan", a));
    SY1000_default_replace(6009, 12, GetJsonString("UserBass_patch%fx(3)%fxPEq", a));
    SY1000_default_replace(6035, 7, GetJsonString("UserBass_patch%fx(3)%fxPedalBend", a));
    SY1000_default_replace(6056, 8, GetJsonString("UserBass_patch%fx(3)%fxPhaser", a));
    SY1000_default_replace(6078, 19, GetJsonString("UserBass_patch%fx(3)%fxPitchShift", a));
    SY1000_default_replace(6111, 14, GetJsonString("UserBass_patch%fx(3)%fxReverb", a));
    SY1000_default_replace(6139, 6, GetJsonString("UserBass_patch%fx(3)%fxRingMod", a));
    SY1000_default_replace(6159, 8, GetJsonString("UserBass_patch%fx(3)%fxRotary", a));
    SY1000_default_replace(6181, 7, GetJsonString("UserBass_patch%fx(3)%fxSitarSim", a));
    SY1000_default_replace(6202, 7, GetJsonString("UserBass_patch%fx(3)%fxSlicer", a));
    SY1000_default_replace(6223, 3, GetJsonString("UserBass_patch%fx(3)%fxSlowGear", a));
    SY1000_default_replace(6240, 3, GetJsonString("UserBass_patch%fx(3)%fxSlowGearBass", a));
    SY1000_default_replace(6257, 3, GetJsonString("UserBass_patch%fx(3)%fxSoundHold", a));
    SY1000_default_replace(6274, 8, GetJsonString("UserBass_patch%fx(3)%fxTWah", a));
    SY1000_default_replace(6296, 8, GetJsonString("UserBass_patch%fx(3)%fxTWahBass", a));
    SY1000_default_replace(6318, 5, GetJsonString("UserBass_patch%fx(3)%fxTremolo", a));
    SY1000_default_replace(6337, 6, GetJsonString("UserBass_patch%fx(3)%fxVibrato", a));
    SY1000_default_replace(6357, 15, GetJsonString("UserBass_patch%fx(3)%fxWah", a));
    SY1000_default_replace(6386, 14, GetJsonString("UserBass_patch%reverb", a));
}

//*****************************************************************************  Bass tsl to default Bass **********
void sysxWriter::TSL_B_to_hex_B()
{
    QByteArray t;
    t = GetJsonString("UserBass_patch%common", a);
    SY1000_default.replace(12, 128, t.mid(0, 128));
    SY1000_default.replace(154, 27, t.mid(128, 27));
    SY1000_default_replace(211, 27, GetJsonString("UserBass_patch%led", a));
    SY1000_default_replace(252, 43, GetJsonString("UserBass_patch%assign(1)", a));
    SY1000_default_replace(316, 43, GetJsonString("UserBass_patch%assign(2)", a));
    SY1000_default_replace(373, 43, GetJsonString("UserBass_patch%assign(3)", a));
    SY1000_default_replace(437, 43, GetJsonString("UserBass_patch%assign(4)", a));
    SY1000_default_replace(494, 43, GetJsonString("UserBass_patch%assign(5)", a));
    SY1000_default_replace(558, 43, GetJsonString("UserBass_patch%assign(6)", a));
    SY1000_default_replace(615, 43, GetJsonString("UserBass_patch%assign(7)", a));
    SY1000_default_replace(679, 43, GetJsonString("UserBass_patch%assign(8)", a));
    SY1000_default_replace(736, 43, GetJsonString("UserBass_patch%assign(9)", a));
    SY1000_default_replace(800, 43, GetJsonString("UserBass_patch%assign(10)", a));
    SY1000_default_replace(857, 43, GetJsonString("UserBass_patch%assign(11)", a));
    SY1000_default_replace(921, 43, GetJsonString("UserBass_patch%assign(12)", a));
    SY1000_default_replace(978, 43, GetJsonString("UserBass_patch%assign(13)", a));
    SY1000_default_replace(1042, 43, GetJsonString("UserBass_patch%assign(14)", a));
    SY1000_default_replace(1099, 43, GetJsonString("UserBass_patch%assign(15)", a));
    SY1000_default_replace(1163, 43, GetJsonString("UserBass_patch%assign(16)", a));
    SY1000_default_replace(1220, 103, GetJsonString("UserBass_patch%efct", a));
    SY1000_default_replace(1337, 7, GetJsonString(  "UserBass_patch%gtr2midi", a));
    SY1000_default_replace(1358, 18, GetJsonString("UserBass_patch%inst(1)%inst", a));
    t = GetJsonString("UserBass_patch%inst(1)%instDynaSynth", a);
    SY1000_default.replace(1390, 128, t.mid(0, 128));
    SY1000_default.replace(1532, 78, t.mid(128, 78));
    SY1000_default_replace(1624, 65, GetJsonString("UserBass_patch%inst(1)%instOscSynth", a));
    SY1000_default_replace(1703, 21, GetJsonString("UserBass_patch%inst(1)%instAnalogGr", a));
    SY1000_default_replace(1738, 57, GetJsonString("UserBass_patch%inst(1)%instEBass", a));
    SY1000_default_replace(1809, 43, GetJsonString("UserBass_patch%inst(1)%instAcBass", a));
    SY1000_default_replace(1866, 45, GetJsonString("UserBass_patch%inst(1)%instEGtr", a));
    SY1000_default_replace(1925, 19, GetJsonString("UserBass_patch%inst(1)%instPolyFx", a));
    SY1000_default_replace(1958, 48, GetJsonString("UserBass_patch%inst(1)%instAltTune", a));

    SY1000_default_replace(2020, 18, GetJsonString("UserBass_patch%inst(2)%inst", a));
    t = GetJsonString("UserBass_patch%inst(2)%instDynaSynth", a);
    SY1000_default.replace(2052, 128, t.mid(0, 128));
    SY1000_default.replace(2194, 78, t.mid(128, 78));
    SY1000_default_replace(2286, 65, GetJsonString("UserBass_patch%inst(2)%instOscSynth", a));
    SY1000_default_replace(2365, 21, GetJsonString("UserBass_patch%inst(2)%instAnalogGr", a));
    SY1000_default_replace(2400, 57, GetJsonString("UserBass_patch%inst(2)%instEBass", a));
    SY1000_default_replace(2471, 43, GetJsonString("UserBass_patch%inst(2)%instAcBass", a));
    SY1000_default_replace(2528, 45, GetJsonString("UserBass_patch%inst(2)%instEGtr", a));
    SY1000_default_replace(2587, 19, GetJsonString("UserBass_patch%inst(2)%instPolyFx", a));
    SY1000_default_replace(2620, 48, GetJsonString("UserBass_patch%inst(2)%instAltTune", a));


    SY1000_default_replace(2682, 18, GetJsonString("UserBass_patch%inst(3)%inst", a));
    t = GetJsonString("UserBass_patch%inst(3)%instDynaSynth", a);
    SY1000_default.replace(2714, 128, t.mid(0, 128));
    SY1000_default.replace(2856, 78, t.mid(128, 78));
    SY1000_default_replace(2948, 65, GetJsonString("UserBass_patch%inst(3)%instOscSynth", a));
    SY1000_default_replace(3027, 21, GetJsonString("UserBass_patch%inst(3)%instAnalogGr", a));
    SY1000_default_replace(3062, 57, GetJsonString("UserBass_patch%inst(3)%instEBass", a));
    SY1000_default_replace(3133, 43, GetJsonString("UserBass_patch%inst(3)%instAcBass", a));
    SY1000_default_replace(3190, 45, GetJsonString("UserBass_patch%inst(3)%instEGtr", a));
    SY1000_default_replace(3249, 19, GetJsonString("UserBass_patch%inst(3)%instPolyFx", a));
    SY1000_default_replace(3282, 48, GetJsonString("UserBass_patch%inst(3)%instAltTune", a));

    SY1000_default_replace(3344, 7, GetJsonString("UserBass_patch%comp", a)); //3476, 7
    SY1000_default_replace(3365, 9, GetJsonString("UserBass_patch%dist", a)); //3497, 9
    SY1000_default_replace(3388, 20, GetJsonString("UserBass_patch%amp", a));
    SY1000_default_replace(3422, 4, GetJsonString("UserBass_patch%ns", a));
    SY1000_default_replace(3440, 12, GetJsonString("UserBass_patch%eq(1)", a));
    SY1000_default_replace(3466, 12, GetJsonString("UserBass_patch%eq(2)", a));
    SY1000_default_replace(3492, 10, GetJsonString("UserBass_patch%delay(1)", a));
    SY1000_default_replace(3516, 10, GetJsonString("UserBass_patch%delay(2)", a));
    SY1000_default_replace(3540, 33, GetJsonString("UserBass_patch%mstDelay", a));
    SY1000_default_replace(3587, 25, GetJsonString("UserBass_patch%chorus", a));

    SY1000_default_replace(3626, 2, GetJsonString("UserBass_patch%fx(1)%fx", a));
    SY1000_default_replace(3642, 4, GetJsonString("UserBass_patch%fx(1)%fxAcReso", a));
    SY1000_default_replace(3660, 8, GetJsonString("UserBass_patch%fx(1)%fxAWah", a));
    SY1000_default_replace(3682, 25, GetJsonString("UserBass_patch%fx(1)%fxChorus", a));
    SY1000_default_replace(3721, 3, GetJsonString("UserBass_patch%fx(1)%fxCVibe", a));
    SY1000_default_replace(3738, 7, GetJsonString("UserBass_patch%fx(1)%fxComp", a));
    SY1000_default_replace(3759, 7, GetJsonString("UserBass_patch%fx(1)%fxDefretter", a));
    SY1000_default_replace(3780, 5, GetJsonString("UserBass_patch%fx(1)%fxDefretterBass", a));
    SY1000_default_replace(3799, 33, GetJsonString("UserBass_patch%fx(1)%fxDelay", a));
    SY1000_default_replace(3846, 9, GetJsonString("UserBass_patch%fx(1)%fxFlanger", a));
    SY1000_default_replace(3869, 9, GetJsonString("UserBass_patch%fx(1)%fxFlangerBass", a));
    SY1000_default_replace(3892, 13, GetJsonString("UserBass_patch%fx(1)%fxFv", a));
    SY1000_default_replace(3919, 8, GetJsonString("UserBass_patch%fx(1)%fxGEq", a));
    SY1000_default_replace(3941, 39, GetJsonString("UserBass_patch%fx(1)%fxHarmonist", a));
    SY1000_default_replace(3994, 8, GetJsonString("UserBass_patch%fx(1)%fxHumanizer", a));
    SY1000_default_replace(4016, 4, GetJsonString("UserBass_patch%fx(1)%fxIsolator", a));
    SY1000_default_replace(4034, 6, GetJsonString("UserBass_patch%fx(1)%fxLimiter", a));
    SY1000_default_replace(4054, 3, GetJsonString("UserBass_patch%fx(1)%fxLofi", a));
    SY1000_default_replace(4071, 3, GetJsonString("UserBass_patch%fx(1)%fxOctave", a));
    SY1000_default_replace(4088, 3, GetJsonString("UserBass_patch%fx(1)%fxOctaveBass", a));
    SY1000_default_replace(4105, 6, GetJsonString("UserBass_patch%fx(1)%fxPan", a));
    SY1000_default_replace(4125, 12, GetJsonString("UserBass_patch%fx(1)%fxPEq", a));
    SY1000_default_replace(4151, 7, GetJsonString("UserBass_patch%fx(1)%fxPedalBend", a));
    SY1000_default_replace(4172, 8, GetJsonString("UserBass_patch%fx(1)%fxPhaser", a));
    SY1000_default_replace(4194, 19, GetJsonString("UserBass_patch%fx(1)%fxPitchShift", a));
    SY1000_default_replace(4227, 14, GetJsonString("UserBass_patch%fx(1)%fxReverb", a));
    SY1000_default_replace(4255, 6, GetJsonString("UserBass_patch%fx(1)%fxRingMod", a));
    SY1000_default_replace(4275, 8, GetJsonString("UserBass_patch%fx(1)%fxRotary", a));
    SY1000_default_replace(4297, 7, GetJsonString("UserBass_patch%fx(1)%fxSitarSim", a));
    SY1000_default_replace(4318, 7, GetJsonString("UserBass_patch%fx(1)%fxSlicer", a));
    SY1000_default_replace(4339, 3, GetJsonString("UserBass_patch%fx(1)%fxSlowGear", a));
    SY1000_default_replace(4356, 3, GetJsonString("UserBass_patch%fx(1)%fxSlowGearBass", a));
    SY1000_default_replace(4373, 3, GetJsonString("UserBass_patch%fx(1)%fxSoundHold", a));
    SY1000_default_replace(4390, 8, GetJsonString("UserBass_patch%fx(1)%fxTWah", a));
    SY1000_default_replace(4412, 8, GetJsonString("UserBass_patch%fx(1)%fxTWahBass", a));
    SY1000_default_replace(4434, 5, GetJsonString("UserBass_patch%fx(1)%fxTremolo", a));
    SY1000_default_replace(4453, 6, GetJsonString("UserBass_patch%fx(1)%fxVibrato", a));
    SY1000_default_replace(4473, 15, GetJsonString("UserBass_patch%fx(1)%fxWah", a));

    SY1000_default_replace(4502, 2, GetJsonString("UserBass_patch%fx(2)%fx", a));
    SY1000_default_replace(4518, 4, GetJsonString("UserBass_patch%fx(2)%fxAcReso", a));
    SY1000_default_replace(4536, 8, GetJsonString("UserBass_patch%fx(2)%fxAWah", a));
    SY1000_default_replace(4558, 25, GetJsonString("UserBass_patch%fx(2)%fxChorus", a));
    SY1000_default_replace(4597, 3, GetJsonString("UserBass_patch%fx(2)%fxCVibe", a));
    SY1000_default_replace(4614, 7, GetJsonString("UserBass_patch%fx(2)%fxComp", a));
    SY1000_default_replace(4635, 7, GetJsonString("UserBass_patch%fx(2)%fxDefretter", a));
    SY1000_default_replace(4656, 5, GetJsonString("UserBass_patch%fx(2)%fxDefretterBass", a));
    SY1000_default_replace(4675, 33, GetJsonString("UserBass_patch%fx(2)%fxDelay", a));
    SY1000_default_replace(4722, 9, GetJsonString("UserBass_patch%fx(2)%fxFlanger", a));
    SY1000_default_replace(4745, 9, GetJsonString("UserBass_patch%fx(2)%fxFlangerBass", a));
    SY1000_default_replace(4768, 13, GetJsonString("UserBass_patch%fx(2)%fxFv", a));
    SY1000_default_replace(4795, 8, GetJsonString("UserBass_patch%fx(2)%fxGEq", a));
    SY1000_default_replace(4817, 39, GetJsonString("UserBass_patch%fx(2)%fxHarmonist", a));
    SY1000_default_replace(4870, 8, GetJsonString("UserBass_patch%fx(2)%fxHumanizer", a));
    SY1000_default_replace(4892, 4, GetJsonString("UserBass_patch%fx(2)%fxIsolator", a));
    SY1000_default_replace(4910, 6, GetJsonString("UserBass_patch%fx(2)%fxLimiter", a));
    SY1000_default_replace(4930, 3, GetJsonString("UserBass_patch%fx(2)%fxLofi", a));
    SY1000_default_replace(4947, 3, GetJsonString("UserBass_patch%fx(2)%fxOctave", a));
    SY1000_default_replace(4964, 3, GetJsonString("UserBass_patch%fx(2)%fxOctaveBass", a));
    SY1000_default_replace(4981, 6, GetJsonString("UserBass_patch%fx(2)%fxPan", a));
    SY1000_default_replace(5001, 12, GetJsonString("UserBass_patch%fx(2)%fxPEq", a));
    SY1000_default_replace(5027, 7, GetJsonString("UserBass_patch%fx(2)%fxPedalBend", a));
    SY1000_default_replace(5048, 8, GetJsonString("UserBass_patch%fx(2)%fxPhaser", a));
    SY1000_default_replace(5070, 19, GetJsonString("UserBass_patch%fx(2)%fxPitchShift", a));
    SY1000_default_replace(5103, 14, GetJsonString("UserBass_patch%fx(2)%fxReverb", a));
    SY1000_default_replace(5131, 6, GetJsonString("UserBass_patch%fx(2)%fxRingMod", a));
    SY1000_default_replace(5151, 8, GetJsonString("UserBass_patch%fx(2)%fxRotary", a));
    SY1000_default_replace(5173, 7, GetJsonString("UserBass_patch%fx(2)%fxSitarSim", a));
    SY1000_default_replace(5194, 7, GetJsonString("UserBass_patch%fx(2)%fxSlicer", a));
    SY1000_default_replace(5215, 3, GetJsonString("UserBass_patch%fx(2)%fxSlowGear", a));
    SY1000_default_replace(5232, 3, GetJsonString("UserBass_patch%fx(2)%fxSlowGearBass", a));
    SY1000_default_replace(5249, 3, GetJsonString("UserBass_patch%fx(2)%fxSoundHold", a));
    SY1000_default_replace(5266, 8, GetJsonString("UserBass_patch%fx(2)%fxTWah", a));
    SY1000_default_replace(5288, 8, GetJsonString("UserBass_patch%fx(2)%fxTWahBass", a));
    SY1000_default_replace(5310, 5, GetJsonString("UserBass_patch%fx(2)%fxTremolo", a));
    SY1000_default_replace(5329, 6, GetJsonString("UserBass_patch%fx(2)%fxVibrato", a));
    SY1000_default_replace(5349, 15, GetJsonString("UserBass_patch%fx(2)%fxWah", a));

    SY1000_default_replace(5378, 2, GetJsonString("UserBass_patch%fx(3)%fx", a)); //+1752
    SY1000_default_replace(5394, 4, GetJsonString("UserBass_patch%fx(3)%fxAcReso", a));
    SY1000_default_replace(5412, 8, GetJsonString("UserBass_patch%fx(3)%fxAWah", a));
    SY1000_default_replace(5434, 25, GetJsonString("UserBass_patch%fx(3)%fxChorus", a));
    SY1000_default_replace(5473, 3, GetJsonString("UserBass_patch%fx(3)%fxCVibe", a));
    SY1000_default_replace(5490, 7, GetJsonString("UserBass_patch%fx(3)%fxComp", a));
    SY1000_default_replace(5511, 7, GetJsonString("UserBass_patch%fx(3)%fxDefretter", a));
    SY1000_default_replace(5532, 5, GetJsonString("UserBass_patch%fx(3)%fxDefretterBass", a));
    SY1000_default_replace(5551, 33, GetJsonString("UserBass_patch%fx(3)%fxDelay", a));
    SY1000_default_replace(5598, 9, GetJsonString("UserBass_patch%fx(3)%fxFlanger", a));
    SY1000_default_replace(5621, 9, GetJsonString("UserBass_patch%fx(3)%fxFlangerBass", a));
    SY1000_default_replace(5644, 13, GetJsonString("UserBass_patch%fx(3)%fxFv", a));
    SY1000_default_replace(5671, 8, GetJsonString("UserBass_patch%fx(3)%fxGEq", a));
    SY1000_default_replace(5693, 39, GetJsonString("UserBass_patch%fx(3)%fxHarmonist", a));
    SY1000_default_replace(5746, 8, GetJsonString("UserBass_patch%fx(3)%fxHumanizer", a));
    SY1000_default_replace(5768, 4, GetJsonString("UserBass_patch%fx(3)%fxIsolator", a));
    SY1000_default_replace(5786, 6, GetJsonString("UserBass_patch%fx(3)%fxLimiter", a));
    SY1000_default_replace(5806, 3, GetJsonString("UserBass_patch%fx(3)%fxLofi", a));
    SY1000_default_replace(5823, 3, GetJsonString("UserBass_patch%fx(3)%fxOctave", a));
    SY1000_default_replace(5840, 3, GetJsonString("UserBass_patch%fx(3)%fxOctaveBass", a));
    SY1000_default_replace(5857, 6, GetJsonString("UserBass_patch%fx(3)%fxPan", a));
    SY1000_default_replace(5877, 12, GetJsonString("UserBass_patch%fx(3)%fxPEq", a));
    SY1000_default_replace(5903, 7, GetJsonString("UserBass_patch%fx(3)%fxPedalBend", a));
    SY1000_default_replace(5924, 8, GetJsonString("UserBass_patch%fx(3)%fxPhaser", a));
    SY1000_default_replace(5946, 19, GetJsonString("UserBass_patch%fx(3)%fxPitchShift", a));
    SY1000_default_replace(5979, 14, GetJsonString("UserBass_patch%fx(3)%fxReverb", a));
    SY1000_default_replace(6007, 6, GetJsonString("UserBass_patch%fx(3)%fxRingMod", a));
    SY1000_default_replace(6027, 8, GetJsonString("UserBass_patch%fx(3)%fxRotary", a));
    SY1000_default_replace(6049, 7, GetJsonString("UserBass_patch%fx(3)%fxSitarSim", a));
    SY1000_default_replace(6070, 7, GetJsonString("UserBass_patch%fx(3)%fxSlicer", a));
    SY1000_default_replace(6091, 3, GetJsonString("UserBass_patch%fx(3)%fxSlowGear", a));
    SY1000_default_replace(6108, 3, GetJsonString("UserBass_patch%fx(3)%fxSlowGearBass", a));
    SY1000_default_replace(6125, 3, GetJsonString("UserBass_patch%fx(3)%fxSoundHold", a));
    SY1000_default_replace(6142, 8, GetJsonString("UserBass_patch%fx(3)%fxTWah", a));
    SY1000_default_replace(6164, 8, GetJsonString("UserBass_patch%fx(3)%fxTWahBass", a));
    SY1000_default_replace(6186, 5, GetJsonString("UserBass_patch%fx(3)%fxTremolo", a));
    SY1000_default_replace(6205, 6, GetJsonString("UserBass_patch%fx(3)%fxVibrato", a));
    SY1000_default_replace(6225, 15, GetJsonString("UserBass_patch%fx(3)%fxWah", a));
    SY1000_default_replace(6254, 14, GetJsonString("UserBass_patch%reverb", a));
}


void sysxWriter::TSL_G_to_hex_B()
//*****************************************************************************  Guitar tsl to default Bass **********
{
    bool ok;
    QByteArray t;
    t = GetJsonString("User_patch%common", a);
    SY1000_default.replace(12, 128, t.mid(0, 128));
    SY1000_default.replace(154, 27, t.mid(128, 27));
    SY1000_default_replace(211, 27, GetJsonString("User_patch%led", a));
    SY1000_default_replace(252, 43, GetJsonString("User_patch%assign(1)", a));
    SY1000_default_replace(253, 4,  convert_Assign(SY1000_default.mid(253, 4), 1));
    SY1000_default_replace(316, 43, GetJsonString("User_patch%assign(2)", a));
    SY1000_default_replace(317, 4,  convert_Assign(SY1000_default.mid(317, 4), 1));
    SY1000_default_replace(373, 43, GetJsonString("User_patch%assign(3)", a));
    SY1000_default_replace(374, 4,  convert_Assign(SY1000_default.mid(374, 4), 1));
    SY1000_default_replace(437, 43, GetJsonString("User_patch%assign(4)", a));
    SY1000_default_replace(438, 4,  convert_Assign(SY1000_default.mid(438, 4), 1));
    SY1000_default_replace(494, 43, GetJsonString("User_patch%assign(5)", a));
    SY1000_default_replace(495, 4,  convert_Assign(SY1000_default.mid(495, 4), 1));
    SY1000_default_replace(558, 43, GetJsonString("User_patch%assign(6)", a));
    SY1000_default_replace(559, 4,  convert_Assign(SY1000_default.mid(559, 4), 1));
    SY1000_default_replace(615, 43, GetJsonString("User_patch%assign(7)", a));
    SY1000_default_replace(616, 4,  convert_Assign(SY1000_default.mid(616, 4), 1));
    SY1000_default_replace(679, 43, GetJsonString("User_patch%assign(8)", a));
    SY1000_default_replace(680, 4,  convert_Assign(SY1000_default.mid(680, 4), 1));
    SY1000_default_replace(736, 43, GetJsonString("User_patch%assign(9)", a));
    SY1000_default_replace(737, 4,  convert_Assign(SY1000_default.mid(737, 4), 1));
    SY1000_default_replace(800, 43, GetJsonString("User_patch%assign(10)", a));
    SY1000_default_replace(801, 4,  convert_Assign(SY1000_default.mid(801, 4), 1));
    SY1000_default_replace(857, 43, GetJsonString("User_patch%assign(11)", a));
    SY1000_default_replace(858, 4,  convert_Assign(SY1000_default.mid(858, 4), 1));
    SY1000_default_replace(921, 43, GetJsonString("User_patch%assign(12)", a));
    SY1000_default_replace(922, 4,  convert_Assign(SY1000_default.mid(922, 4), 1));
    SY1000_default_replace(978, 43, GetJsonString("User_patch%assign(13)", a));
    SY1000_default_replace(979, 4,  convert_Assign(SY1000_default.mid(979, 4), 1));
    SY1000_default_replace(1042, 43, GetJsonString("User_patch%assign(14)", a));
    SY1000_default_replace(1043, 4,  convert_Assign(SY1000_default.mid(1043, 4), 1));
    SY1000_default_replace(1099, 43, GetJsonString("User_patch%assign(15)", a));
    SY1000_default_replace(1100, 4,  convert_Assign(SY1000_default.mid(1100, 4), 1));
    SY1000_default_replace(1163, 43, GetJsonString("User_patch%assign(16)", a));
    SY1000_default_replace(1164, 4,  convert_Assign(SY1000_default.mid(1164, 4), 1));
    SY1000_default_replace(1220, 103, GetJsonString("User_patch%efct", a));
    SY1000_default_replace(1337, 7, GetJsonString(  "User_patch%gtr2midi", a));
    t = GetJsonString("User_patch%inst(1)%inst", a);
    t.replace(1, 1, convert_value(t.mid(1,1), "02", "0F"));
    SY1000_default_replace(1358, 18, t);
    t = GetJsonString("User_patch%inst(1)%instDynaSynth", a);
    SY1000_default.replace(1390, 128, t.mid(0, 128));
    SY1000_default.replace(1532, 78, t.mid(128, 78));
    SY1000_default_replace(1624, 65, GetJsonString("User_patch%inst(1)%instOscSynth", a));
    SY1000_default_replace(1703, 21, GetJsonString("User_patch%inst(1)%instGr300", a));
    t = GetJsonString("User_patch%inst(1)%instEGtr", a);
    t.replace(0, 1, convert_value(t.mid(1,1), "02", "0D"));
    t.replace(1, 1, convert_value(t.mid(1,1), "02", "0E"));
    SY1000_default_replace(1866, 45, t);
    t = GetJsonString("User_patch%inst(1)%instAcoustic", a);
    QByteArray part;
    part.append(t.mid(10,1));               // volume
    part.append(t.mid(1,1));                // body
    part.append(t.mid(8,1));                // resonance
    part.append(t.mid(11,1));               // size
    part.append(t.mid(2,1));                // attack
    part.append(t.mid(4,1));                // bottom
    part.append(t.mid(7,1));                // buzz sense
    part.append(t.mid(6,1));                // decay
    part.append(t.mid(12,35));              // all the other stuff
    SY1000_default_replace(1809, 43, part); // replace with rebuild
    t = GetJsonString("User_patch%inst(1)%instEBass", a);
    part.clear();
    part.append(convert_QByteArray_value(t.mid(0,1), "02", "0A")); // bass type
    part.append(QByteArray::fromHex("64")); // Master
    part.append(t.mid(2,1));                // front vol
    part.append(t.mid(1,1));                // rear vol
    part.append(t.mid(10,1));               // front tone
    part.append(t.mid(10,1));               // rear tone
    part.append(QByteArray::fromHex("32")); // mm treble
    part.append(QByteArray::fromHex("32")); // mm bass
    part.append(QByteArray::fromHex("32")); // act treble
    part.append(QByteArray::fromHex("32")); // act bass
    part.append(QByteArray::fromHex("01")); // pu sel
    part.append(QByteArray::fromHex("01")); // treble on
    part.append(QByteArray::fromHex("01")); // bass on
    part.append(QByteArray::fromHex("01")); // solo
    part.append(t.mid(3,8));                // rest of the bass stuff
    part.append(t.mid(11,35));              // all the other stuff
    SY1000_default_replace(1738, 57, part); // replace with part rebuild
    if(GetJsonString("User_patch%inst(1)%inst", a).mid(1,1).toHex().toUpper()=="06")
    {
        t = GetJsonString("User_patch%inst(1)%instVioGtr", a);
        part.clear();
        part.append(QByteArray::fromHex("32")); // bass type
        part.append(QByteArray::fromHex("64")); // Master
        part.append(t.mid(8,1));                // front vol
        part.append(t.mid(8,1));                // rear vol
        part.append(t.mid(9,1));                // front tone
        part.append(t.mid(9,1));                // rear tone
        part.append(QByteArray::fromHex("32")); // mm treble
        part.append(QByteArray::fromHex("32")); // mm bass
        part.append(QByteArray::fromHex("32")); // act treble
        part.append(QByteArray::fromHex("32")); // act bass
        part.append(QByteArray::fromHex("01")); // pu sel
        part.append(QByteArray::fromHex("01")); // treble on
        part.append(QByteArray::fromHex("01")); // bass on
        part.append(QByteArray::fromHex("01")); // solo
        part.append(t.mid(3,8));                // rest of the bass stuff
        SY1000_default_replace(1738, 22, part); // replace with part rebuild
        SY1000_default_replace(1760, 15, t.mid(22,15)); // replace with part rebuild
    };
    SY1000_default_replace(1925, 19, GetJsonString("User_patch%inst(1)%instPolyFx", a));
    t = GetJsonString("User_patch%inst(1)%instAltTune", a);
    if(t.mid(1, 1).toHex().toUpper().toInt(&ok, 16)<12)
    {
        t.replace(2, 6, altTuning(t.mid(1, 1).toHex().toUpper())); // if tuning mode, replace user tuning.
    };
    t.replace(1, 1, convert_QByteArray_value(t.mid(1, 1), "02", "0C")); // AltTune type
    SY1000_default_replace(1958, 48, t); // replace AltTuning

    t = GetJsonString("User_patch%inst(2)%inst", a);
    t.replace(1, 1, convert_value(t.mid(1,1), "02", "0F"));
    SY1000_default_replace(2020, 18, t);
    t = GetJsonString("User_patch%inst(2)%instDynaSynth", a);
    SY1000_default.replace(2052, 128, t.mid(0, 128));
    SY1000_default.replace(2194, 78, t.mid(128, 78));
    SY1000_default_replace(2286, 65, GetJsonString("User_patch%inst(2)%instOscSynth", a));
    SY1000_default_replace(2365, 21, GetJsonString("User_patch%inst(2)%instGr300", a));
    t = GetJsonString("User_patch%inst(2)%instEGtr", a);
    t.replace(0, 1, convert_value(t.mid(1,1), "02", "0D"));
    t.replace(1, 1, convert_value(t.mid(1,1), "02", "0E"));
    SY1000_default_replace(2528, 45, t);
    t = GetJsonString("User_patch%inst(2)%instAcoustic", a);
    part.clear();
    part.append(t.mid(10,1));               // volume
    part.append(t.mid(1,1));                // body
    part.append(t.mid(8,1));                // resonance
    part.append(t.mid(11,1));               // size
    part.append(t.mid(2,1));                // attack
    part.append(t.mid(4,1));                // bottom
    part.append(t.mid(7,1));                // buzz sense
    part.append(t.mid(6,1));                // decay
    part.append(t.mid(12,35));              // all the other stuff
    SY1000_default_replace(2471, 43, part); // replace with rebuild
    t = GetJsonString("User_patch%inst(2)%instEBass", a);
    part.clear();
    part.append(convert_QByteArray_value(t.mid(0,1), "02", "0A")); // bass type
    part.append(QByteArray::fromHex("64")); // Master
    part.append(t.mid(2,1));                // front vol
    part.append(t.mid(1,1));                // rear vol
    part.append(t.mid(10,1));               // front tone
    part.append(t.mid(10,1));               // rear tone
    part.append(QByteArray::fromHex("32")); // mm treble
    part.append(QByteArray::fromHex("32")); // mm bass
    part.append(QByteArray::fromHex("32")); // act treble
    part.append(QByteArray::fromHex("32")); // act bass
    part.append(QByteArray::fromHex("01")); // pu sel
    part.append(QByteArray::fromHex("01")); // treble on
    part.append(QByteArray::fromHex("01")); // bass on
    part.append(QByteArray::fromHex("01")); // solo
    part.append(t.mid(3,8));                // rest of the bass stuff
    part.append(t.mid(11,35));              // all the other stuff
    SY1000_default_replace(2400, 57, part); // replace with part rebuild
    if(GetJsonString("User_patch%inst(2)%inst", a).mid(1,1).toHex().toUpper()=="06")
    {
        t = GetJsonString("User_patch%inst(2)%instVioGtr", a);
        part.clear();
        part.append(QByteArray::fromHex("32")); // bass type
        part.append(QByteArray::fromHex("64")); // Master
        part.append(t.mid(8,1));                // front vol
        part.append(t.mid(8,1));                // rear vol
        part.append(t.mid(9,1));                // front tone
        part.append(t.mid(9,1));                // rear tone
        part.append(QByteArray::fromHex("32")); // mm treble
        part.append(QByteArray::fromHex("32")); // mm bass
        part.append(QByteArray::fromHex("32")); // act treble
        part.append(QByteArray::fromHex("32")); // act bass
        part.append(QByteArray::fromHex("01")); // pu sel
        part.append(QByteArray::fromHex("01")); // treble on
        part.append(QByteArray::fromHex("01")); // bass on
        part.append(QByteArray::fromHex("01")); // solo
        part.append(t.mid(3,8));                // rest of the bass stuff
        SY1000_default_replace(2400, 22, part); // replace with part rebuild
        SY1000_default_replace(2422, 15, t.mid(22,15)); // replace with part rebuild
    };
    SY1000_default_replace(2587, 19, GetJsonString("User_patch%inst(2)%instPolyFx", a));
    t = GetJsonString("User_patch%inst(2)%instAltTune", a);
    if(t.mid(1,1).toHex().toUpper().toInt(&ok, 16)<12)
    {
        t.replace(2, 6, altTuning(t.mid(1,1).toHex().toUpper())); // if tuning mode, replace user tuning.
    };
    t.replace(1, 1, convert_QByteArray_value(t.mid(1, 1), "02", "0C")); // AltTune type
    SY1000_default_replace(2620, 48, t);                        // replace AltTuning
    t = GetJsonString("User_patch%inst(3)%inst", a);
    t.replace(1, 1, convert_value(t.mid(1,1), "02", "0F"));
    SY1000_default_replace(2682, 18, t);
    t = GetJsonString("User_patch%inst(3)%instDynaSynth", a);
    SY1000_default.replace(2714, 128, t.mid(0, 128));
    SY1000_default.replace(2856, 78, t.mid(128, 78));
    SY1000_default_replace(2948, 65, GetJsonString("User_patch%inst(3)%instOscSynth", a));
    SY1000_default_replace(3027, 21, GetJsonString("User_patch%inst(3)%instGr300", a));
    t = GetJsonString("User_patch%inst(3)%instEGtr", a);
    t.replace(0, 1, convert_value(t.mid(1,1), "02", "0D"));
    t.replace(1, 1, convert_value(t.mid(1,1), "02", "0E"));
    SY1000_default_replace(3190, 45, t);
    t = GetJsonString("User_patch%inst(3)%instAcoustic", a);
    part.clear();
    part.append(t.mid(10,1));               // volume
    part.append(t.mid(1,1));                // body
    part.append(t.mid(8,1));                // resonance
    part.append(t.mid(11,1));               // size
    part.append(t.mid(2,1));                // attack
    part.append(t.mid(4,1));                // bottom
    part.append(t.mid(7,1));                // buzz sense
    part.append(t.mid(6,1));                // decay
    part.append(t.mid(12,35));              // all the other stuff
    SY1000_default_replace(3133, 43, part); // replace with rebuild
    t = GetJsonString("User_patch%inst(3)%instEBass", a);
    part.clear();
    part.append(convert_QByteArray_value(t.mid(0,1), "02", "0A")); // bass type
    part.append(QByteArray::fromHex("64")); // Master
    part.append(t.mid(2,1));                // front vol
    part.append(t.mid(1,1));                // rear vol
    part.append(t.mid(10,1));               // front tone
    part.append(t.mid(10,1));               // rear tone
    part.append(QByteArray::fromHex("32")); // mm treble
    part.append(QByteArray::fromHex("32")); // mm bass
    part.append(QByteArray::fromHex("32")); // act treble
    part.append(QByteArray::fromHex("32")); // act bass
    part.append(QByteArray::fromHex("01")); // pu sel
    part.append(QByteArray::fromHex("01")); // treble on
    part.append(QByteArray::fromHex("01")); // bass on
    part.append(QByteArray::fromHex("01")); // solo
    part.append(t.mid(3,8));                // rest of the bass stuff
    part.append(t.mid(11,35));              // all the other stuff
    SY1000_default_replace(3062, 57, part); // replace with part rebuild
    if(GetJsonString("User_patch%inst(3)%inst", a).mid(1,1).toHex().toUpper()=="06")
    {
        t = GetJsonString("User_patch%inst(3)%instVioGtr", a);
        part.clear();
        part.append(QByteArray::fromHex("32")); // bass type
        part.append(QByteArray::fromHex("64")); // Master
        part.append(t.mid(8,1));                // front vol
        part.append(t.mid(8,1));                // rear vol
        part.append(t.mid(9,1));                // front tone
        part.append(t.mid(9,1));                // rear tone
        part.append(QByteArray::fromHex("32")); // mm treble
        part.append(QByteArray::fromHex("32")); // mm bass
        part.append(QByteArray::fromHex("32")); // act treble
        part.append(QByteArray::fromHex("32")); // act bass
        part.append(QByteArray::fromHex("01")); // pu sel
        part.append(QByteArray::fromHex("01")); // treble on
        part.append(QByteArray::fromHex("01")); // bass on
        part.append(QByteArray::fromHex("01")); // solo
        part.append(t.mid(3,8));                // rest of the bass stuff
        SY1000_default_replace(3062, 22, part); // replace with part rebuild
        SY1000_default_replace(3084, 15, t.mid(22,15)); // replace with part rebuild
    };
    SY1000_default_replace(3249, 19, GetJsonString("User_patch%inst(3)%instPolyFx", a));
    t = GetJsonString("User_patch%inst(3)%instAltTune", a);
    if(t.mid(1,1).toHex().toUpper().toInt(&ok, 16)<12)
    {
        t.replace(2, 6, altTuning(t.mid(1,1).toHex().toUpper())); // if tuning mode, replace user tuning.
    };
    t.replace(1, 1, convert_QByteArray_value(t.mid(1, 1), "02", "0C")); // AltTune type
    SY1000_default_replace(3282, 48, t);                        // replace AltTuning

    SY1000_default_replace(3344, 7, GetJsonString("User_patch%comp", a)); //3476, 7
    SY1000_default_replace(3365, 9, GetJsonString("User_patch%dist", a)); //3497, 9
    SY1000_default_replace(3388, 20, GetJsonString("User_patch%amp", a));
    SY1000_default_replace(3422, 4, GetJsonString("User_patch%ns", a));
    SY1000_default_replace(3440, 12, GetJsonString("User_patch%eq(1)", a));
    SY1000_default_replace(3466, 12, GetJsonString("User_patch%eq(2)", a));
    SY1000_default_replace(3492, 10, GetJsonString("User_patch%delay(1)", a));
    SY1000_default_replace(3516, 10, GetJsonString("User_patch%delay(2)", a));
    SY1000_default_replace(3540, 33, GetJsonString("User_patch%mstDelay", a));
    SY1000_default_replace(3587, 25, GetJsonString("User_patch%chorus", a));

    SY1000_default_replace(3626, 2, GetJsonString("User_patch%fx(1)%fx", a));
    SY1000_default_replace(3642, 4, GetJsonString("User_patch%fx(1)%fxAcReso", a));
    SY1000_default_replace(3660, 8, GetJsonString("User_patch%fx(1)%fxAWah", a));
    SY1000_default_replace(3682, 25, GetJsonString("User_patch%fx(1)%fxChorus", a));
    SY1000_default_replace(3721, 3, GetJsonString("User_patch%fx(1)%fxCVibe", a));
    SY1000_default_replace(3738, 7, GetJsonString("User_patch%fx(1)%fxComp", a));
    SY1000_default_replace(3759, 7, GetJsonString("User_patch%fx(1)%fxDefretter", a));
    SY1000_default_replace(3780, 5, GetJsonString("User_patch%fx(1)%fxDefretterBass", a));
    SY1000_default_replace(3799, 33, GetJsonString("User_patch%fx(1)%fxDelay", a));
    SY1000_default_replace(3846, 9, GetJsonString("User_patch%fx(1)%fxFlanger", a));
    SY1000_default_replace(3869, 9, GetJsonString("User_patch%fx(1)%fxFlangerBass", a));
    SY1000_default_replace(3892, 13, GetJsonString("User_patch%fx(1)%fxFv", a));
    SY1000_default_replace(3919, 8, GetJsonString("User_patch%fx(1)%fxGEq", a));
    SY1000_default_replace(3941, 39, GetJsonString("User_patch%fx(1)%fxHarmonist", a));
    SY1000_default_replace(3994, 8, GetJsonString("User_patch%fx(1)%fxHumanizer", a));
    SY1000_default_replace(4016, 4, GetJsonString("User_patch%fx(1)%fxIsolator", a));
    SY1000_default_replace(4034, 6, GetJsonString("User_patch%fx(1)%fxLimiter", a));
    SY1000_default_replace(4054, 3, GetJsonString("User_patch%fx(1)%fxLofi", a));
    SY1000_default_replace(4071, 3, GetJsonString("User_patch%fx(1)%fxOctave", a));
    SY1000_default_replace(4088, 3, GetJsonString("User_patch%fx(1)%fxOctaveBass", a));
    SY1000_default_replace(4105, 6, GetJsonString("User_patch%fx(1)%fxPan", a));
    SY1000_default_replace(4125, 12, GetJsonString("User_patch%fx(1)%fxPEq", a));
    SY1000_default_replace(4151, 7, GetJsonString("User_patch%fx(1)%fxPedalBend", a));
    SY1000_default_replace(4172, 8, GetJsonString("User_patch%fx(1)%fxPhaser", a));
    SY1000_default_replace(4194, 19, GetJsonString("User_patch%fx(1)%fxPitchShift", a));
    SY1000_default_replace(4227, 14, GetJsonString("User_patch%fx(1)%fxReverb", a));
    SY1000_default_replace(4255, 6, GetJsonString("User_patch%fx(1)%fxRingMod", a));
    SY1000_default_replace(4275, 8, GetJsonString("User_patch%fx(1)%fxRotary", a));
    SY1000_default_replace(4297, 7, GetJsonString("User_patch%fx(1)%fxSitarSim", a));
    SY1000_default_replace(4318, 7, GetJsonString("User_patch%fx(1)%fxSlicer", a));
    SY1000_default_replace(4339, 3, GetJsonString("User_patch%fx(1)%fxSlowGear", a));
    SY1000_default_replace(4356, 3, GetJsonString("User_patch%fx(1)%fxSlowGearBass", a));
    SY1000_default_replace(4373, 3, GetJsonString("User_patch%fx(1)%fxSoundHold", a));
    SY1000_default_replace(4390, 8, GetJsonString("User_patch%fx(1)%fxTWah", a));
    SY1000_default_replace(4412, 8, GetJsonString("User_patch%fx(1)%fxTWahBass", a));
    SY1000_default_replace(4434, 5, GetJsonString("User_patch%fx(1)%fxTremolo", a));
    SY1000_default_replace(4453, 6, GetJsonString("User_patch%fx(1)%fxVibrato", a));
    SY1000_default_replace(4473, 15, GetJsonString("User_patch%fx(1)%fxWah", a));

    SY1000_default_replace(4502, 2, GetJsonString("User_patch%fx(2)%fx", a));
    SY1000_default_replace(4518, 4, GetJsonString("User_patch%fx(2)%fxAcReso", a));
    SY1000_default_replace(4536, 8, GetJsonString("User_patch%fx(2)%fxAWah", a));
    SY1000_default_replace(4558, 25, GetJsonString("User_patch%fx(2)%fxChorus", a));
    SY1000_default_replace(4597, 3, GetJsonString("User_patch%fx(2)%fxCVibe", a));
    SY1000_default_replace(4614, 7, GetJsonString("User_patch%fx(2)%fxComp", a));
    SY1000_default_replace(4635, 7, GetJsonString("User_patch%fx(2)%fxDefretter", a));
    SY1000_default_replace(4656, 5, GetJsonString("User_patch%fx(2)%fxDefretterBass", a));
    SY1000_default_replace(4675, 33, GetJsonString("User_patch%fx(2)%fxDelay", a));
    SY1000_default_replace(4722, 9, GetJsonString("User_patch%fx(2)%fxFlanger", a));
    SY1000_default_replace(4745, 9, GetJsonString("User_patch%fx(2)%fxFlangerBass", a));
    SY1000_default_replace(4768, 13, GetJsonString("User_patch%fx(2)%fxFv", a));
    SY1000_default_replace(4795, 8, GetJsonString("User_patch%fx(2)%fxGEq", a));
    SY1000_default_replace(4817, 39, GetJsonString("User_patch%fx(2)%fxHarmonist", a));
    SY1000_default_replace(4870, 8, GetJsonString("User_patch%fx(2)%fxHumanizer", a));
    SY1000_default_replace(4892, 4, GetJsonString("User_patch%fx(2)%fxIsolator", a));
    SY1000_default_replace(4910, 6, GetJsonString("User_patch%fx(2)%fxLimiter", a));
    SY1000_default_replace(4930, 3, GetJsonString("User_patch%fx(2)%fxLofi", a));
    SY1000_default_replace(4947, 3, GetJsonString("User_patch%fx(2)%fxOctave", a));
    SY1000_default_replace(4964, 3, GetJsonString("User_patch%fx(2)%fxOctaveBass", a));
    SY1000_default_replace(4981, 6, GetJsonString("User_patch%fx(2)%fxPan", a));
    SY1000_default_replace(5001, 12, GetJsonString("User_patch%fx(2)%fxPEq", a));
    SY1000_default_replace(5027, 7, GetJsonString("User_patch%fx(2)%fxPedalBend", a));
    SY1000_default_replace(5048, 8, GetJsonString("User_patch%fx(2)%fxPhaser", a));
    SY1000_default_replace(5070, 19, GetJsonString("User_patch%fx(2)%fxPitchShift", a));
    SY1000_default_replace(5103, 14, GetJsonString("User_patch%fx(2)%fxReverb", a));
    SY1000_default_replace(5131, 6, GetJsonString("User_patch%fx(2)%fxRingMod", a));
    SY1000_default_replace(5151, 8, GetJsonString("User_patch%fx(2)%fxRotary", a));
    SY1000_default_replace(5173, 7, GetJsonString("User_patch%fx(2)%fxSitarSim", a));
    SY1000_default_replace(5194, 7, GetJsonString("User_patch%fx(2)%fxSlicer", a));
    SY1000_default_replace(5215, 3, GetJsonString("User_patch%fx(2)%fxSlowGear", a));
    SY1000_default_replace(5232, 3, GetJsonString("User_patch%fx(2)%fxSlowGearBass", a));
    SY1000_default_replace(5249, 3, GetJsonString("User_patch%fx(2)%fxSoundHold", a));
    SY1000_default_replace(5266, 8, GetJsonString("User_patch%fx(2)%fxTWah", a));
    SY1000_default_replace(5288, 8, GetJsonString("User_patch%fx(2)%fxTWahBass", a));
    SY1000_default_replace(5310, 5, GetJsonString("User_patch%fx(2)%fxTremolo", a));
    SY1000_default_replace(5329, 6, GetJsonString("User_patch%fx(2)%fxVibrato", a));
    SY1000_default_replace(5349, 15, GetJsonString("User_patch%fx(2)%fxWah", a));

    SY1000_default_replace(5378, 2, GetJsonString("User_patch%fx(3)%fx", a)); //+1752
    SY1000_default_replace(5394, 4, GetJsonString("User_patch%fx(3)%fxAcReso", a));
    SY1000_default_replace(5412, 8, GetJsonString("User_patch%fx(3)%fxAWah", a));
    SY1000_default_replace(5434, 25, GetJsonString("User_patch%fx(3)%fxChorus", a));
    SY1000_default_replace(5473, 3, GetJsonString("User_patch%fx(3)%fxCVibe", a));
    SY1000_default_replace(5490, 7, GetJsonString("User_patch%fx(3)%fxComp", a));
    SY1000_default_replace(5511, 7, GetJsonString("User_patch%fx(3)%fxDefretter", a));
    SY1000_default_replace(5532, 5, GetJsonString("User_patch%fx(3)%fxDefretterBass", a));
    SY1000_default_replace(5551, 33, GetJsonString("User_patch%fx(3)%fxDelay", a));
    SY1000_default_replace(5598, 9, GetJsonString("User_patch%fx(3)%fxFlanger", a));
    SY1000_default_replace(5621, 9, GetJsonString("User_patch%fx(3)%fxFlangerBass", a));
    SY1000_default_replace(5644, 13, GetJsonString("User_patch%fx(3)%fxFv", a));
    SY1000_default_replace(5671, 8, GetJsonString("User_patch%fx(3)%fxGEq", a));
    SY1000_default_replace(5693, 39, GetJsonString("User_patch%fx(3)%fxHarmonist", a));
    SY1000_default_replace(5746, 8, GetJsonString("User_patch%fx(3)%fxHumanizer", a));
    SY1000_default_replace(5768, 4, GetJsonString("User_patch%fx(3)%fxIsolator", a));
    SY1000_default_replace(5786, 6, GetJsonString("User_patch%fx(3)%fxLimiter", a));
    SY1000_default_replace(5806, 3, GetJsonString("User_patch%fx(3)%fxLofi", a));
    SY1000_default_replace(5823, 3, GetJsonString("User_patch%fx(3)%fxOctave", a));
    SY1000_default_replace(5840, 3, GetJsonString("User_patch%fx(3)%fxOctaveBass", a));
    SY1000_default_replace(5857, 6, GetJsonString("User_patch%fx(3)%fxPan", a));
    SY1000_default_replace(5877, 12, GetJsonString("User_patch%fx(3)%fxPEq", a));
    SY1000_default_replace(5903, 7, GetJsonString("User_patch%fx(3)%fxPedalBend", a));
    SY1000_default_replace(5924, 8, GetJsonString("User_patch%fx(3)%fxPhaser", a));
    SY1000_default_replace(5946, 19, GetJsonString("User_patch%fx(3)%fxPitchShift", a));
    SY1000_default_replace(5979, 14, GetJsonString("User_patch%fx(3)%fxReverb", a));
    SY1000_default_replace(6007, 6, GetJsonString("User_patch%fx(3)%fxRingMod", a));
    SY1000_default_replace(6027, 8, GetJsonString("User_patch%fx(3)%fxRotary", a));
    SY1000_default_replace(6049, 7, GetJsonString("User_patch%fx(3)%fxSitarSim", a));
    SY1000_default_replace(6070, 7, GetJsonString("User_patch%fx(3)%fxSlicer", a));
    SY1000_default_replace(6091, 3, GetJsonString("User_patch%fx(3)%fxSlowGear", a));
    SY1000_default_replace(6108, 3, GetJsonString("User_patch%fx(3)%fxSlowGearBass", a));
    SY1000_default_replace(6125, 3, GetJsonString("User_patch%fx(3)%fxSoundHold", a));
    SY1000_default_replace(6142, 8, GetJsonString("User_patch%fx(3)%fxTWah", a));
    SY1000_default_replace(6164, 8, GetJsonString("User_patch%fx(3)%fxTWahBass", a));
    SY1000_default_replace(6186, 5, GetJsonString("User_patch%fx(3)%fxTremolo", a));
    SY1000_default_replace(6205, 6, GetJsonString("User_patch%fx(3)%fxVibrato", a));
    SY1000_default_replace(6225, 15, GetJsonString("User_patch%fx(3)%fxWah", a));
    SY1000_default_replace(6254, 14, GetJsonString("User_patch%reverb", a));
}

//*****************************************************************************  default Guitar to Guitar tsl **********
void sysxWriter::hex_G_to_TSL_G()
{

    QFile TSLfile(":default_G.tsl");           // Read the default KATANA TSL file .
    if (TSLfile.open(QIODevice::ReadOnly))
    {
        this->TSL_default = TSLfile.readAll();
        TSLfile.close();
    };

    QByteArray t;
    t.append(temp.mid(12, 128));
    t.append(temp.mid(154, 27));
    AppendTSL(t, "User_patch%common");
    AppendTSL(temp.mid(211, 27), "User_patch%led");
    AppendTSL(temp.mid(252, 43), "User_patch%assign(1)");
    AppendTSL(temp.mid(316, 43), "User_patch%assign(2)");
    AppendTSL(temp.mid(373, 43), "User_patch%assign(3)");
    AppendTSL(temp.mid(437, 43), "User_patch%assign(4)");
    AppendTSL(temp.mid(494, 43), "User_patch%assign(5)");
    AppendTSL(temp.mid(558, 43), "User_patch%assign(6)");
    AppendTSL(temp.mid(615, 43), "User_patch%assign(7)");
    AppendTSL(temp.mid(679, 43), "User_patch%assign(8)");
    AppendTSL(temp.mid(736, 43), "User_patch%assign(9)");
    AppendTSL(temp.mid(800, 43), "User_patch%assign(10)");
    AppendTSL(temp.mid(857, 43), "User_patch%assign(11)");
    AppendTSL(temp.mid(921, 43), "User_patch%assign(12)");
    AppendTSL(temp.mid(978, 43), "User_patch%assign(13)");
    AppendTSL(temp.mid(1042, 43), "User_patch%assign(14)");
    AppendTSL(temp.mid(1099, 43), "User_patch%assign(15)");
    AppendTSL(temp.mid(1163, 43), "User_patch%assign(16)");
    AppendTSL(temp.mid(1220, 103), "User_patch%efct");
    AppendTSL(temp.mid(1337, 7),   "User_patch%gtr2midi");
    AppendTSL(temp.mid(1358, 18), "User_patch%inst(1)%inst");
    t = temp.mid(1390, 128);
    t.append(temp.mid(1532, 78));
    AppendTSL(t, "User_patch%inst(1)%instDynaSynth");
    AppendTSL(temp.mid(1624, 65), "User_patch%inst(1)%instOscSynth");
    AppendTSL(temp.mid(1703, 21), "User_patch%inst(1)%instGr300");
    AppendTSL(temp.mid(1738, 45), "User_patch%inst(1)%instEGtr");
    AppendTSL(temp.mid(1797, 47), "User_patch%inst(1)%instAcoustic");
    AppendTSL(temp.mid(1858, 46), "User_patch%inst(1)%instEBass");
    AppendTSL(temp.mid(1918, 37), "User_patch%inst(1)%instVioGtr");
    AppendTSL(temp.mid(1969, 19), "User_patch%inst(1)%instPolyFx");
    AppendTSL(temp.mid(2002, 48), "User_patch%inst(1)%instAltTune");
    AppendTSL(temp.mid(2064, 18), "User_patch%inst(2)%inst");
    t = temp.mid(2096, 128);
    t.append(temp.mid(2238, 78));
    AppendTSL(t, "User_patch%inst(2)%instDynaSynth");
    AppendTSL(temp.mid(2330, 65), "User_patch%inst(2)%instOscSynth");
    AppendTSL(temp.mid(2409, 21), "User_patch%inst(2)%instGr300");
    AppendTSL(temp.mid(2444, 45), "User_patch%inst(2)%instEGtr");
    AppendTSL(temp.mid(2503, 47), "User_patch%inst(2)%instAcoustic");
    AppendTSL(temp.mid(2564, 46), "User_patch%inst(2)%instEBass");
    AppendTSL(temp.mid(2624, 37), "User_patch%inst(2)%instVioGtr");
    AppendTSL(temp.mid(2675, 19), "User_patch%inst(2)%instPolyFx");
    AppendTSL(temp.mid(2708, 48), "User_patch%inst(2)%instAltTune");
    AppendTSL(temp.mid(2770, 18), "User_patch%inst(3)%inst");
    t = temp.mid(2802, 128);
    t.append(temp.mid(2944, 78));
    AppendTSL(t, "User_patch%inst(3)%instDynaSynth");
    AppendTSL(temp.mid(3036, 65), "User_patch%inst(3)%instOscSynth");
    AppendTSL(temp.mid(3115, 21), "User_patch%inst(3)%instGr300");
    AppendTSL(temp.mid(3150, 45), "User_patch%inst(3)%instEGtr");
    AppendTSL(temp.mid(3209, 47), "User_patch%inst(3)%instAcoustic");
    AppendTSL(temp.mid(3270, 46), "User_patch%inst(3)%instEBass");
    AppendTSL(temp.mid(3330, 37), "User_patch%inst(3)%instVioGtr");
    AppendTSL(temp.mid(3381, 19), "User_patch%inst(3)%instPolyFx");
    AppendTSL(temp.mid(3414, 48), "User_patch%inst(3)%instAltTune");

    AppendTSL(temp.mid(3476, 7), "User_patch%comp");
    AppendTSL(temp.mid(3497, 9), "User_patch%dist");
    AppendTSL(temp.mid(3520, 20), "User_patch%amp");
    AppendTSL(temp.mid(3554, 4), "User_patch%ns");
    AppendTSL(temp.mid(3572, 12), "User_patch%eq(1)");
    AppendTSL(temp.mid(3598, 12), "User_patch%eq(2)");
    AppendTSL(temp.mid(3624, 10), "User_patch%delay(1)");
    AppendTSL(temp.mid(3648, 10), "User_patch%delay(2)");
    AppendTSL(temp.mid(3672, 33), "User_patch%mstDelay");
    AppendTSL(temp.mid(3719, 25), "User_patch%chorus");

    AppendTSL(temp.mid(3758, 2), "User_patch%fx(1)%fx");
    AppendTSL(temp.mid(3774, 4), "User_patch%fx(1)%fxAcReso");
    AppendTSL(temp.mid(3792, 8), "User_patch%fx(1)%fxAWah");
    AppendTSL(temp.mid(3814, 25), "User_patch%fx(1)%fxChorus");
    AppendTSL(temp.mid(3853, 3), "User_patch%fx(1)%fxCVibe");
    AppendTSL(temp.mid(3870, 7), "User_patch%fx(1)%fxComp");
    AppendTSL(temp.mid(3891, 7), "User_patch%fx(1)%fxDefretter");
    AppendTSL(temp.mid(3912, 5), "User_patch%fx(1)%fxDefretterBass");
    AppendTSL(temp.mid(3931, 33), "User_patch%fx(1)%fxDelay");
    AppendTSL(temp.mid(3978, 9), "User_patch%fx(1)%fxFlanger");
    AppendTSL(temp.mid(4001, 9), "User_patch%fx(1)%fxFlangerBass");
    AppendTSL(temp.mid(4024, 13), "User_patch%fx(1)%fxFv");
    AppendTSL(temp.mid(4051, 8), "User_patch%fx(1)%fxGEq");
    AppendTSL(temp.mid(4073, 39), "User_patch%fx(1)%fxHarmonist");
    AppendTSL(temp.mid(4126, 8), "User_patch%fx(1)%fxHumanizer");
    AppendTSL(temp.mid(4148, 4), "User_patch%fx(1)%fxIsolator");
    AppendTSL(temp.mid(4166, 6), "User_patch%fx(1)%fxLimiter");
    AppendTSL(temp.mid(4186, 3), "User_patch%fx(1)%fxLofi");
    AppendTSL(temp.mid(4203, 3), "User_patch%fx(1)%fxOctave");
    AppendTSL(temp.mid(4220, 3), "User_patch%fx(1)%fxOctaveBass");
    AppendTSL(temp.mid(4237, 6), "User_patch%fx(1)%fxPan");
    AppendTSL(temp.mid(4257, 12), "User_patch%fx(1)%fxPEq");
    AppendTSL(temp.mid(4283, 7), "User_patch%fx(1)%fxPedalBend");
    AppendTSL(temp.mid(4304, 8), "User_patch%fx(1)%fxPhaser");
    AppendTSL(temp.mid(4326, 19), "User_patch%fx(1)%fxPitchShift");
    AppendTSL(temp.mid(4359, 14), "User_patch%fx(1)%fxReverb");
    AppendTSL(temp.mid(4387, 6), "User_patch%fx(1)%fxRingMod");
    AppendTSL(temp.mid(4407, 8), "User_patch%fx(1)%fxRotary");
    AppendTSL(temp.mid(4429, 7), "User_patch%fx(1)%fxSitarSim");
    AppendTSL(temp.mid(4450, 7), "User_patch%fx(1)%fxSlicer");
    AppendTSL(temp.mid(4471, 3), "User_patch%fx(1)%fxSlowGear");
    AppendTSL(temp.mid(4488, 3), "User_patch%fx(1)%fxSlowGearBass");
    AppendTSL(temp.mid(4505, 3), "User_patch%fx(1)%fxSoundHold");
    AppendTSL(temp.mid(4522, 8), "User_patch%fx(1)%fxTWah");
    AppendTSL(temp.mid(4544, 8), "User_patch%fx(1)%fxTWahBass");
    AppendTSL(temp.mid(4566, 5), "User_patch%fx(1)%fxTremolo");
    AppendTSL(temp.mid(4585, 6), "User_patch%fx(1)%fxVibrato");
    AppendTSL(temp.mid(4605, 15), "User_patch%fx(1)%fxWah");

    AppendTSL(temp.mid(4634, 2), "User_patch%fx(2)%fx");
    AppendTSL(temp.mid(4650, 4), "User_patch%fx(2)%fxAcReso");
    AppendTSL(temp.mid(4668, 8), "User_patch%fx(2)%fxAWah");
    AppendTSL(temp.mid(4690, 25), "User_patch%fx(2)%fxChorus");
    AppendTSL(temp.mid(4729, 3), "User_patch%fx(2)%fxCVibe");
    AppendTSL(temp.mid(4746, 7), "User_patch%fx(2)%fxComp");
    AppendTSL(temp.mid(4767, 7), "User_patch%fx(2)%fxDefretter");
    AppendTSL(temp.mid(4788, 5), "User_patch%fx(2)%fxDefretterBass");
    AppendTSL(temp.mid(4807, 33), "User_patch%fx(2)%fxDelay");
    AppendTSL(temp.mid(4854, 9), "User_patch%fx(2)%fxFlanger");
    AppendTSL(temp.mid(4877, 9), "User_patch%fx(2)%fxFlangerBass");
    AppendTSL(temp.mid(4900, 13), "User_patch%fx(2)%fxFv");
    AppendTSL(temp.mid(4927, 8), "User_patch%fx(2)%fxGEq");
    AppendTSL(temp.mid(4949, 39), "User_patch%fx(2)%fxHarmonist");
    AppendTSL(temp.mid(5002, 8), "User_patch%fx(2)%fxHumanizer");
    AppendTSL(temp.mid(5024, 4), "User_patch%fx(2)%fxIsolator");
    AppendTSL(temp.mid(5042, 6), "User_patch%fx(2)%fxLimiter");
    AppendTSL(temp.mid(5062, 3), "User_patch%fx(2)%fxLofi");
    AppendTSL(temp.mid(5079, 3), "User_patch%fx(2)%fxOctave");
    AppendTSL(temp.mid(5096, 3), "User_patch%fx(2)%fxOctaveBass");
    AppendTSL(temp.mid(5113, 6), "User_patch%fx(2)%fxPan");
    AppendTSL(temp.mid(5133, 12), "User_patch%fx(2)%fxPEq");
    AppendTSL(temp.mid(5159, 7), "User_patch%fx(2)%fxPedalBend");
    AppendTSL(temp.mid(5180, 8), "User_patch%fx(2)%fxPhaser");
    AppendTSL(temp.mid(5202, 19), "User_patch%fx(2)%fxPitchShift");
    AppendTSL(temp.mid(5235, 14), "User_patch%fx(2)%fxReverb");
    AppendTSL(temp.mid(5263, 6), "User_patch%fx(2)%fxRingMod");
    AppendTSL(temp.mid(5283, 8), "User_patch%fx(2)%fxRotary");
    AppendTSL(temp.mid(5305, 7), "User_patch%fx(2)%fxSitarSim");
    AppendTSL(temp.mid(5326, 7), "User_patch%fx(2)%fxSlicer");
    AppendTSL(temp.mid(5347, 3), "User_patch%fx(2)%fxSlowGear");
    AppendTSL(temp.mid(5364, 3), "User_patch%fx(2)%fxSlowGearBass");
    AppendTSL(temp.mid(5381, 3), "User_patch%fx(2)%fxSoundHold");
    AppendTSL(temp.mid(5398, 8), "User_patch%fx(2)%fxTWah");
    AppendTSL(temp.mid(5420, 8), "User_patch%fx(2)%fxTWahBass");
    AppendTSL(temp.mid(5442, 5), "User_patch%fx(2)%fxTremolo");
    AppendTSL(temp.mid(5461, 6), "User_patch%fx(2)%fxVibrato");
    AppendTSL(temp.mid(5481, 15), "User_patch%fx(2)%fxWah");

    AppendTSL(temp.mid(5510, 2), "User_patch%fx(3)%fx"); //+1752
    AppendTSL(temp.mid(5526, 4), "User_patch%fx(3)%fxAcReso");
    AppendTSL(temp.mid(5544, 8), "User_patch%fx(3)%fxAWah");
    AppendTSL(temp.mid(5566, 25), "User_patch%fx(3)%fxChorus");
    AppendTSL(temp.mid(5605, 3), "User_patch%fx(3)%fxCVibe");
    AppendTSL(temp.mid(5622, 7), "User_patch%fx(3)%fxComp");
    AppendTSL(temp.mid(5643, 7), "User_patch%fx(3)%fxDefretter");
    AppendTSL(temp.mid(5664, 5), "User_patch%fx(3)%fxDefretterBass");
    AppendTSL(temp.mid(5683, 33), "User_patch%fx(3)%fxDelay");
    AppendTSL(temp.mid(5730, 9), "User_patch%fx(3)%fxFlanger");
    AppendTSL(temp.mid(5753, 9), "User_patch%fx(3)%fxFlangerBass");
    AppendTSL(temp.mid(5776, 13), "User_patch%fx(3)%fxFv");
    AppendTSL(temp.mid(5803, 8), "User_patch%fx(3)%fxGEq");
    AppendTSL(temp.mid(5825, 39), "User_patch%fx(3)%fxHarmonist");
    AppendTSL(temp.mid(5878, 8), "User_patch%fx(3)%fxHumanizer");
    AppendTSL(temp.mid(5900, 4), "User_patch%fx(3)%fxIsolator");
    AppendTSL(temp.mid(5918, 6), "User_patch%fx(3)%fxLimiter");
    AppendTSL(temp.mid(5938, 3), "User_patch%fx(3)%fxLofi");
    AppendTSL(temp.mid(5955, 3), "User_patch%fx(3)%fxOctave");
    AppendTSL(temp.mid(5972, 3), "User_patch%fx(3)%fxOctaveBass");
    AppendTSL(temp.mid(5989, 6), "User_patch%fx(3)%fxPan");
    AppendTSL(temp.mid(6009, 12), "User_patch%fx(3)%fxPEq");
    AppendTSL(temp.mid(6035, 7), "User_patch%fx(3)%fxPedalBend");
    AppendTSL(temp.mid(6056, 8), "User_patch%fx(3)%fxPhaser");
    AppendTSL(temp.mid(6078, 19), "User_patch%fx(3)%fxPitchShift");
    AppendTSL(temp.mid(6111, 14), "User_patch%fx(3)%fxReverb");
    AppendTSL(temp.mid(6139, 6), "User_patch%fx(3)%fxRingMod");
    AppendTSL(temp.mid(6160, 8), "User_patch%fx(3)%fxRotary");
    AppendTSL(temp.mid(6181, 7), "User_patch%fx(3)%fxSitarSim");
    AppendTSL(temp.mid(6202, 7), "User_patch%fx(3)%fxSlicer");
    AppendTSL(temp.mid(6223, 3), "User_patch%fx(3)%fxSlowGear");
    AppendTSL(temp.mid(6240, 3), "User_patch%fx(3)%fxSlowGearBass");
    AppendTSL(temp.mid(6257, 3), "User_patch%fx(3)%fxSoundHold");
    AppendTSL(temp.mid(6274, 8), "User_patch%fx(3)%fxTWah");
    AppendTSL(temp.mid(6296, 8), "User_patch%fx(3)%fxTWahBass");
    AppendTSL(temp.mid(6318, 5), "User_patch%fx(3)%fxTremolo");
    AppendTSL(temp.mid(6337, 6), "User_patch%fx(3)%fxVibrato");
    AppendTSL(temp.mid(6357, 15), "User_patch%fx(3)%fxWah");
    AppendTSL(temp.mid(6386, 14), "User_patch%reverb");
}

//*****************************************************************************  default Guitar to Bass tsl **********
void sysxWriter::hex_G_to_TSL_B()
{
    bool ok;
    QFile TSLfile(":default_B.tsl");           // Read the default KATANA BASS TSL file .
    if (TSLfile.open(QIODevice::ReadOnly))
    {
        this->TSL_default = TSLfile.readAll();
        TSLfile.close();
    };

    QByteArray t;
    t.append(temp.mid(12, 128));
    t.append(temp.mid(154, 27));
    AppendTSL(t, "UserBass_patch%common");
    AppendTSL(temp.mid(211, 27), "UserBass_patch%led");
    temp.replace(253, 4,  convert_Assign(temp.mid(253, 4), 1));
    AppendTSL(temp.mid(252, 43), "UserBass_patch%assign(1)");
    temp.replace(317, 4,  convert_Assign(temp.mid(317, 4), 1));
    AppendTSL(temp.mid(316, 43), "UserBass_patch%assign(2)");
    temp.replace(374, 4,  convert_Assign(temp.mid(374, 4), 1));
    AppendTSL(temp.mid(373, 43), "UserBass_patch%assign(3)");
    temp.replace(438, 4,  convert_Assign(temp.mid(438, 4), 1));
    AppendTSL(temp.mid(437, 43), "UserBass_patch%assign(4)");
    temp.replace(495, 4,  convert_Assign(temp.mid(495, 4), 1));
    AppendTSL(temp.mid(494, 43), "UserBass_patch%assign(5)");
    temp.replace(559, 4,  convert_Assign(temp.mid(559, 4), 1));
    AppendTSL(temp.mid(558, 43), "UserBass_patch%assign(6)");
    temp.replace(616, 4,  convert_Assign(temp.mid(616, 4), 1));
    AppendTSL(temp.mid(615, 43), "UserBass_patch%assign(7)");
    temp.replace(680, 4,  convert_Assign(temp.mid(680, 4), 1));
    AppendTSL(temp.mid(679, 43), "UserBass_patch%assign(8)");
    temp.replace(737, 4,  convert_Assign(temp.mid(737, 4), 1));
    AppendTSL(temp.mid(736, 43), "UserBass_patch%assign(9)");
    temp.replace(801, 4,  convert_Assign(temp.mid(801, 4), 1));
    AppendTSL(temp.mid(800, 43), "UserBass_patch%assign(10)");
    temp.replace(858, 4,  convert_Assign(temp.mid(858, 4), 1));
    AppendTSL(temp.mid(857, 43), "UserBass_patch%assign(11)");
    temp.replace(922, 4,  convert_Assign(temp.mid(922, 4), 1));
    AppendTSL(temp.mid(921, 43), "UserBass_patch%assign(12)");
    temp.replace(979, 4,  convert_Assign(temp.mid(979, 4), 1));
    AppendTSL(temp.mid(978, 43), "UserBass_patch%assign(13)");
    temp.replace(1043, 4,  convert_Assign(temp.mid(1043, 4), 1));
    AppendTSL(temp.mid(1042, 43), "UserBass_patch%assign(14)");
    temp.replace(1100, 4,  convert_Assign(temp.mid(1100, 4), 1));
    AppendTSL(temp.mid(1099, 43), "UserBass_patch%assign(15)");
    temp.replace(1164, 4,  convert_Assign(temp.mid(1164, 4), 1));
    AppendTSL(temp.mid(1163, 43), "UserBass_patch%assign(16)");
    AppendTSL(temp.mid(1220, 103), "UserBass_patch%efct");
    AppendTSL(temp.mid(1337, 7),   "UserBass_patch%gtr2midi");
    t = temp.mid(1358, 18);
    t.replace(1, 1, convert_value(t.mid(1, 1), "02", "0F"));   // inst type
    AppendTSL(t, "UserBass_patch%inst(1)%inst");
    t = temp.mid(1390, 128);
    t.append(temp.mid(1532, 78));
    AppendTSL(t, "UserBass_patch%inst(1)%instDynaSynth");
    AppendTSL(temp.mid(1624, 65), "UserBass_patch%inst(1)%instOscSynth");
    AppendTSL(temp.mid(1703, 21), "UserBass_patch%inst(1)%instAnalogGr");
    QByteArray tt;
    t = temp.mid(1738, 45);
    t.replace(0, 1, convert_value(t.mid(0, 1), "02", "0D"));   // guitar type
    t.replace(0, 1, convert_value(t.mid(0, 1), "02", "0E"));   // pu select
    AppendTSL(t, "UserBass_patch%inst(1)%instEGtr");
    t = temp.mid(1797, 47);
    tt.clear();
    tt.append(t.mid(10, 1));   // volume
    tt.append(t.mid(1, 1));    // body
    tt.append(t.mid(8, 1));    // resonance
    tt.append(convert_value(t.mid(4, 1), "01", "59"));  // size
    tt.append(t.mid(2, 1));    // attack
    tt.append(t.mid(5, 1));    // bottom
    tt.append(t.mid(7, 1));    // buzz sense
    tt.append(t.mid(6, 1));    // decay
    t.remove(0, 12);
    t.prepend(tt);
    AppendTSL(t, "UserBass_patch%inst(1)%instAcBass");
    t = temp.mid(1858, 46);
    tt.clear();
    tt= QByteArray::fromHex("00646464646432320000010101010032003232006464"); // copy default bass part
    tt.replace(0, 1, convert_value(t.mid(0, 1), "02", "0A"));   // bass type
    tt.replace(3, 1, t.mid(1, 1));                              // front volume
    tt.replace(2, 1, t.mid(2, 1));                              // rear volume
    tt.replace(14, 1, t.mid(3, 1));                             // tone type
    tt.replace(15, 1, t.mid(4, 1));                             // sense
    tt.replace(16, 1, t.mid(5, 1));                             // depth
    tt.replace(17, 1, t.mid(6, 1));                             // attack
    tt.replace(18, 1, t.mid(7, 1));                             // resonance
    tt.replace(19, 1, t.mid(8, 1));                             // direct
    tt.replace(20, 1, t.mid(9, 1));                             // volume
    tt.replace(21, 1, t.mid(10, 1));                            // tone
    tt.append(t.mid(11, 35));                                   // preamp/eq/ns
    AppendTSL(tt, "UserBass_patch%inst(1)%instEBass");
    AppendTSL(temp.mid(1969, 19), "UserBass_patch%inst(1)%instPolyFx");

    t = temp.mid(2002, 48);
    if(t.mid(1, 1).toHex().toUpper().toInt(&ok, 16)<12)
    {
        t.replace(2, 6, altTuning(t.mid(1, 1).toHex().toUpper())); // if tuning mode, replace user tuning.
    };
    t.replace(1, 1, convert_value(t.mid(1, 1), "02", "0C"));    // convert alt tuning type
    AppendTSL(t, "UserBass_patch%inst(1)%instAltTune");

    t = temp.mid(2064, 18);
    t.replace(1, 1, convert_value(t.mid(1, 1), "02", "0F"));   // inst type
    AppendTSL(t, "UserBass_patch%inst(2)%inst");
    t = temp.mid(2096, 128);
    t.append(temp.mid(2238, 78));
    AppendTSL(t, "UserBass_patch%inst(2)%instDynaSynth");
    AppendTSL(temp.mid(2330, 65), "UserBass_patch%inst(2)%instOscSynth");
    AppendTSL(temp.mid(2409, 21), "UserBass_patch%inst(2)%instAnalogGr");   
    t = temp.mid(2444, 45);
    t.replace(0, 1, convert_value(t.mid(0, 1), "02", "0D"));   // guitar type
    t.replace(0, 1, convert_value(t.mid(0, 1), "02", "0E"));   // pu select
    AppendTSL(t, "UserBass_patch%inst(2)%instEGtr");
    t = temp.mid(2503, 47);
    tt.clear();
    tt.append(t.mid(10, 1));   // volume
    tt.append(t.mid(1, 1));    // body
    tt.append(t.mid(8, 1));    // resonance
    tt.append(convert_value(t.mid(4, 1), "01", "59"));  // size
    tt.append(t.mid(2, 1));    // attack
    tt.append(t.mid(5, 1));    // bottom
    tt.append(t.mid(7, 1));    // buzz sense
    tt.append(t.mid(6, 1));    // decay
    t.remove(0, 12);
    t.prepend(tt);
    AppendTSL(t, "UserBass_patch%inst(2)%instAcBass");
    t = temp.mid(2564, 46);
    tt.clear();
    tt= QByteArray::fromHex("00646464646432320000010101010032003232006464"); // copy default bass part
    tt.replace(0, 1, convert_value(t.mid(0, 1), "02", "0A"));   // bass type
    tt.replace(3, 1, t.mid(1, 1));                              // front volume
    tt.replace(2, 1, t.mid(2, 1));                              // rear volume
    tt.replace(14, 1, t.mid(3, 1));                             // tone type
    tt.replace(15, 1, t.mid(4, 1));                             // sense
    tt.replace(16, 1, t.mid(5, 1));                             // depth
    tt.replace(17, 1, t.mid(6, 1));                             // attack
    tt.replace(18, 1, t.mid(7, 1));                             // resonance
    tt.replace(19, 1, t.mid(8, 1));                             // direct
    tt.replace(20, 1, t.mid(9, 1));                             // volume
    tt.replace(21, 1, t.mid(10, 1));                            // tone
    tt.append(t.mid(11, 35));                                   // preamp/eq/ns
    AppendTSL(tt, "UserBass_patch%inst(2)%instEBass");
    AppendTSL(temp.mid(2675, 19), "UserBass_patch%inst(2)%instPolyFx");
    t = temp.mid(2708, 48);
    if(t.mid(1, 1).toHex().toUpper().toInt(&ok, 16)<12)
    {
        t.replace(2, 6, altTuning(t.mid(1, 1).toHex().toUpper())); // if tuning mode, replace user tuning.
    };
    t.replace(1, 1, convert_value(t.mid(1, 1), "02", "0C"));    // convert alt tuning type
    AppendTSL(t, "UserBass_patch%inst(2)%instAltTune");

    t = temp.mid(2770, 18);
    t.replace(1, 1, convert_value(t.mid(1, 1), "02", "0F"));   // inst type
    AppendTSL(t, "UserBass_patch%inst(3)%inst");
    t = temp.mid(2802, 128);
    t.append(temp.mid(2944, 78));
    AppendTSL(t, "UserBass_patch%inst(3)%instDynaSynth");
    AppendTSL(temp.mid(3036, 65), "UserBass_patch%inst(3)%instOscSynth");
    AppendTSL(temp.mid(3115, 21), "UserBass_patch%inst(3)%instAnalogGr");
    t = temp.mid(3150, 45);
    t.replace(0, 1, convert_value(t.mid(0, 1), "02", "0D"));   // guitar type
    t.replace(0, 1, convert_value(t.mid(0, 1), "02", "0E"));   // pu select
    AppendTSL(t, "UserBass_patch%inst(3)%instEGtr");
    t = temp.mid(3209, 47);
    tt.clear();
    tt.append(t.mid(10, 1));   // volume
    tt.append(t.mid(1, 1));    // body
    tt.append(t.mid(8, 1));    // resonance
    tt.append(convert_value(t.mid(4, 1), "01", "59"));  // size
    tt.append(t.mid(2, 1));    // attack
    tt.append(t.mid(5, 1));    // bottom
    tt.append(t.mid(7, 1));    // buzz sense
    tt.append(t.mid(6, 1));    // decay
    t.remove(0, 12);
    t.prepend(tt);
    AppendTSL(t, "UserBass_patch%inst(3)%instAcBass");
    t = temp.mid(3270, 46);
    tt.clear();
    tt= QByteArray::fromHex("00646464646432320000010101010032003232006464"); // copy default bass part
    tt.replace(0, 1, convert_value(t.mid(0, 1), "02", "0A"));   // bass type
    tt.replace(3, 1, t.mid(1, 1));                              // front volume
    tt.replace(2, 1, t.mid(2, 1));                              // rear volume
    tt.replace(14, 1, t.mid(3, 1));                             // tone type
    tt.replace(15, 1, t.mid(4, 1));                             // sense
    tt.replace(16, 1, t.mid(5, 1));                             // depth
    tt.replace(17, 1, t.mid(6, 1));                             // attack
    tt.replace(18, 1, t.mid(7, 1));                             // resonance
    tt.replace(19, 1, t.mid(8, 1));                             // direct
    tt.replace(20, 1, t.mid(9, 1));                             // volume
    tt.replace(21, 1, t.mid(10, 1));                            // tone
    tt.append(t.mid(11, 35));                                   // preamp/eq/ns
    AppendTSL(tt, "UserBass_patch%inst(3)%instEBass");
    AppendTSL(temp.mid(3381, 19), "UserBass_patch%inst(3)%instPolyFx");
    t = temp.mid(3414, 48);
    if(t.mid(1, 1).toHex().toUpper().toInt(&ok, 16)<12)
    {
        t.replace(2, 6, altTuning(t.mid(1, 1).toHex().toUpper())); // if tuning mode, replace user tuning.
    };
    t.replace(1, 1, convert_value(t.mid(1, 1), "02", "0C"));    // convert alt tuning type
    AppendTSL(t, "UserBass_patch%inst(3)%instAltTune");
    AppendTSL(temp.mid(3476, 7), "UserBass_patch%comp");
    AppendTSL(temp.mid(3497, 9), "UserBass_patch%dist");
    AppendTSL(temp.mid(3520, 20), "UserBass_patch%amp");
    AppendTSL(temp.mid(3554, 4), "UserBass_patch%ns");
    AppendTSL(temp.mid(3572, 12), "UserBass_patch%eq(1)");
    AppendTSL(temp.mid(3598, 12), "UserBass_patch%eq(2)");
    AppendTSL(temp.mid(3624, 10), "UserBass_patch%delay(1)");
    AppendTSL(temp.mid(3648, 10), "UserBass_patch%delay(2)");
    AppendTSL(temp.mid(3672, 33), "UserBass_patch%mstDelay");
    AppendTSL(temp.mid(3719, 25), "UserBass_patch%chorus");

    AppendTSL(temp.mid(3758, 2), "UserBass_patch%fx(1)%fx");
    AppendTSL(temp.mid(3774, 4), "UserBass_patch%fx(1)%fxAcReso");
    AppendTSL(temp.mid(3792, 8), "UserBass_patch%fx(1)%fxAWah");
    AppendTSL(temp.mid(3814, 25), "UserBass_patch%fx(1)%fxChorus");
    AppendTSL(temp.mid(3853, 3), "UserBass_patch%fx(1)%fxCVibe");
    AppendTSL(temp.mid(3870, 7), "UserBass_patch%fx(1)%fxComp");
    AppendTSL(temp.mid(3891, 7), "UserBass_patch%fx(1)%fxDefretter");
    AppendTSL(temp.mid(3912, 5), "UserBass_patch%fx(1)%fxDefretterBass");
    AppendTSL(temp.mid(3931, 33), "UserBass_patch%fx(1)%fxDelay");
    AppendTSL(temp.mid(3978, 9), "UserBass_patch%fx(1)%fxFlanger");
    AppendTSL(temp.mid(4001, 9), "UserBass_patch%fx(1)%fxFlangerBass");
    AppendTSL(temp.mid(4024, 13), "UserBass_patch%fx(1)%fxFv");
    AppendTSL(temp.mid(4051, 8), "UserBass_patch%fx(1)%fxGEq");
    AppendTSL(temp.mid(4073, 39), "UserBass_patch%fx(1)%fxHarmonist");
    AppendTSL(temp.mid(4126, 8), "UserBass_patch%fx(1)%fxHumanizer");
    AppendTSL(temp.mid(4148, 4), "UserBass_patch%fx(1)%fxIsolator");
    AppendTSL(temp.mid(4166, 6), "UserBass_patch%fx(1)%fxLimiter");
    AppendTSL(temp.mid(4186, 3), "UserBass_patch%fx(1)%fxLofi");
    AppendTSL(temp.mid(4203, 3), "UserBass_patch%fx(1)%fxOctave");
    AppendTSL(temp.mid(4220, 3), "UserBass_patch%fx(1)%fxOctaveBass");
    AppendTSL(temp.mid(4237, 6), "UserBass_patch%fx(1)%fxPan");
    AppendTSL(temp.mid(4257, 12), "UserBass_patch%fx(1)%fxPEq");
    AppendTSL(temp.mid(4283, 7), "UserBass_patch%fx(1)%fxPedalBend");
    AppendTSL(temp.mid(4304, 8), "UserBass_patch%fx(1)%fxPhaser");
    AppendTSL(temp.mid(4326, 19), "UserBass_patch%fx(1)%fxPitchShift");
    AppendTSL(temp.mid(4359, 14), "UserBass_patch%fx(1)%fxReverb");
    AppendTSL(temp.mid(4387, 6), "UserBass_patch%fx(1)%fxRingMod");
    AppendTSL(temp.mid(4407, 8), "UserBass_patch%fx(1)%fxRotary");
    AppendTSL(temp.mid(4429, 7), "UserBass_patch%fx(1)%fxSitarSim");
    AppendTSL(temp.mid(4450, 7), "UserBass_patch%fx(1)%fxSlicer");
    AppendTSL(temp.mid(4471, 3), "UserBass_patch%fx(1)%fxSlowGear");
    AppendTSL(temp.mid(4488, 3), "UserBass_patch%fx(1)%fxSlowGearBass");
    AppendTSL(temp.mid(4505, 3), "UserBass_patch%fx(1)%fxSoundHold");
    AppendTSL(temp.mid(4522, 8), "UserBass_patch%fx(1)%fxTWah");
    AppendTSL(temp.mid(4544, 8), "UserBass_patch%fx(1)%fxTWahBass");
    AppendTSL(temp.mid(4566, 5), "UserBass_patch%fx(1)%fxTremolo");
    AppendTSL(temp.mid(4585, 6), "UserBass_patch%fx(1)%fxVibrato");
    AppendTSL(temp.mid(4605, 15), "UserBass_patch%fx(1)%fxWah");

    AppendTSL(temp.mid(4634, 2), "UserBass_patch%fx(2)%fx");
    AppendTSL(temp.mid(4650, 4), "UserBass_patch%fx(2)%fxAcReso");
    AppendTSL(temp.mid(4668, 8), "UserBass_patch%fx(2)%fxAWah");
    AppendTSL(temp.mid(4690, 25), "UserBass_patch%fx(2)%fxChorus");
    AppendTSL(temp.mid(4729, 3), "UserBass_patch%fx(2)%fxCVibe");
    AppendTSL(temp.mid(4746, 7), "UserBass_patch%fx(2)%fxComp");
    AppendTSL(temp.mid(4767, 7), "UserBass_patch%fx(2)%fxDefretter");
    AppendTSL(temp.mid(4788, 5), "UserBass_patch%fx(2)%fxDefretterBass");
    AppendTSL(temp.mid(4807, 33), "UserBass_patch%fx(2)%fxDelay");
    AppendTSL(temp.mid(4854, 9), "UserBass_patch%fx(2)%fxFlanger");
    AppendTSL(temp.mid(4877, 9), "UserBass_patch%fx(2)%fxFlangerBass");
    AppendTSL(temp.mid(4900, 13), "UserBass_patch%fx(2)%fxFv");
    AppendTSL(temp.mid(4927, 8), "UserBass_patch%fx(2)%fxGEq");
    AppendTSL(temp.mid(4949, 39), "UserBass_patch%fx(2)%fxHarmonist");
    AppendTSL(temp.mid(5002, 8), "UserBass_patch%fx(2)%fxHumanizer");
    AppendTSL(temp.mid(5024, 4), "UserBass_patch%fx(2)%fxIsolator");
    AppendTSL(temp.mid(5042, 6), "UserBass_patch%fx(2)%fxLimiter");
    AppendTSL(temp.mid(5062, 3), "UserBass_patch%fx(2)%fxLofi");
    AppendTSL(temp.mid(5079, 3), "UserBass_patch%fx(2)%fxOctave");
    AppendTSL(temp.mid(5096, 3), "UserBass_patch%fx(2)%fxOctaveBass");
    AppendTSL(temp.mid(5113, 6), "UserBass_patch%fx(2)%fxPan");
    AppendTSL(temp.mid(5133, 12), "UserBass_patch%fx(2)%fxPEq");
    AppendTSL(temp.mid(5159, 7), "UserBass_patch%fx(2)%fxPedalBend");
    AppendTSL(temp.mid(5180, 8), "UserBass_patch%fx(2)%fxPhaser");
    AppendTSL(temp.mid(5202, 19), "UserBass_patch%fx(2)%fxPitchShift");
    AppendTSL(temp.mid(5235, 14), "UserBass_patch%fx(2)%fxReverb");
    AppendTSL(temp.mid(5263, 6), "UserBass_patch%fx(2)%fxRingMod");
    AppendTSL(temp.mid(5283, 8), "UserBass_patch%fx(2)%fxRotary");
    AppendTSL(temp.mid(5305, 7), "UserBass_patch%fx(2)%fxSitarSim");
    AppendTSL(temp.mid(5326, 7), "UserBass_patch%fx(2)%fxSlicer");
    AppendTSL(temp.mid(5347, 3), "UserBass_patch%fx(2)%fxSlowGear");
    AppendTSL(temp.mid(5364, 3), "UserBass_patch%fx(2)%fxSlowGearBass");
    AppendTSL(temp.mid(5381, 3), "UserBass_patch%fx(2)%fxSoundHold");
    AppendTSL(temp.mid(5398, 8), "UserBass_patch%fx(2)%fxTWah");
    AppendTSL(temp.mid(5420, 8), "UserBass_patch%fx(2)%fxTWahBass");
    AppendTSL(temp.mid(5442, 5), "UserBass_patch%fx(2)%fxTremolo");
    AppendTSL(temp.mid(5461, 6), "UserBass_patch%fx(2)%fxVibrato");
    AppendTSL(temp.mid(5481, 15), "UserBass_patch%fx(2)%fxWah");

    AppendTSL(temp.mid(5510, 2), "UserBass_patch%fx(3)%fx"); //+1752
    AppendTSL(temp.mid(5526, 4), "UserBass_patch%fx(3)%fxAcReso");
    AppendTSL(temp.mid(5544, 8), "UserBass_patch%fx(3)%fxAWah");
    AppendTSL(temp.mid(5566, 25), "UserBass_patch%fx(3)%fxChorus");
    AppendTSL(temp.mid(5605, 3), "UserBass_patch%fx(3)%fxCVibe");
    AppendTSL(temp.mid(5622, 7), "UserBass_patch%fx(3)%fxComp");
    AppendTSL(temp.mid(5643, 7), "UserBass_patch%fx(3)%fxDefretter");
    AppendTSL(temp.mid(5664, 5), "UserBass_patch%fx(3)%fxDefretterBass");
    AppendTSL(temp.mid(5683, 33), "UserBass_patch%fx(3)%fxDelay");
    AppendTSL(temp.mid(5730, 9), "UserBass_patch%fx(3)%fxFlanger");
    AppendTSL(temp.mid(5753, 9), "UserBass_patch%fx(3)%fxFlangerBass");
    AppendTSL(temp.mid(5776, 13), "UserBass_patch%fx(3)%fxFv");
    AppendTSL(temp.mid(5803, 8), "UserBass_patch%fx(3)%fxGEq");
    AppendTSL(temp.mid(5825, 39), "UserBass_patch%fx(3)%fxHarmonist");
    AppendTSL(temp.mid(5878, 8), "UserBass_patch%fx(3)%fxHumanizer");
    AppendTSL(temp.mid(5900, 4), "UserBass_patch%fx(3)%fxIsolator");
    AppendTSL(temp.mid(5918, 6), "UserBass_patch%fx(3)%fxLimiter");
    AppendTSL(temp.mid(5938, 3), "UserBass_patch%fx(3)%fxLofi");
    AppendTSL(temp.mid(5955, 3), "UserBass_patch%fx(3)%fxOctave");
    AppendTSL(temp.mid(5972, 3), "UserBass_patch%fx(3)%fxOctaveBass");
    AppendTSL(temp.mid(5989, 6), "UserBass_patch%fx(3)%fxPan");
    AppendTSL(temp.mid(6009, 12), "UserBass_patch%fx(3)%fxPEq");
    AppendTSL(temp.mid(6035, 7), "UserBass_patch%fx(3)%fxPedalBend");
    AppendTSL(temp.mid(6056, 8), "UserBass_patch%fx(3)%fxPhaser");
    AppendTSL(temp.mid(6078, 19), "UserBass_patch%fx(3)%fxPitchShift");
    AppendTSL(temp.mid(6111, 14), "UserBass_patch%fx(3)%fxReverb");
    AppendTSL(temp.mid(6139, 6), "UserBass_patch%fx(3)%fxRingMod");
    AppendTSL(temp.mid(6160, 8), "UserBass_patch%fx(3)%fxRotary");
    AppendTSL(temp.mid(6181, 7), "UserBass_patch%fx(3)%fxSitarSim");
    AppendTSL(temp.mid(6202, 7), "UserBass_patch%fx(3)%fxSlicer");
    AppendTSL(temp.mid(6223, 3), "UserBass_patch%fx(3)%fxSlowGear");
    AppendTSL(temp.mid(6240, 3), "UserBass_patch%fx(3)%fxSlowGearBass");
    AppendTSL(temp.mid(6257, 3), "UserBass_patch%fx(3)%fxSoundHold");
    AppendTSL(temp.mid(6274, 8), "UserBass_patch%fx(3)%fxTWah");
    AppendTSL(temp.mid(6296, 8), "UserBass_patch%fx(3)%fxTWahBass");
    AppendTSL(temp.mid(6318, 5), "UserBass_patch%fx(3)%fxTremolo");
    AppendTSL(temp.mid(6337, 6), "UserBass_patch%fx(3)%fxVibrato");
    AppendTSL(temp.mid(6357, 15), "UserBass_patch%fx(3)%fxWah");
    AppendTSL(temp.mid(6386, 14), "UserBass_patch%reverb");
}

//*****************************************************************************  default Bass to Guitar tsl **********
void sysxWriter::hex_B_to_TSL_G()
{

    QFile TSLfile(":default_G.tsl");           // Read the default KATANA TSL file .
    if (TSLfile.open(QIODevice::ReadOnly))
    {
        this->TSL_default = TSLfile.readAll();
        TSLfile.close();
    };

    QByteArray t;
    t.append(temp.mid(12, 128));
    t.append(temp.mid(154, 27));
    AppendTSL(t, "User_patch%common");
    AppendTSL(temp.mid(211, 27), "User_patch%led");
    temp.replace(253, 4,  convert_Assign(temp.mid(253, 4), 0));
    AppendTSL(temp.mid(252, 43), "User_patch%assign(1)");
    temp.replace(317, 4,  convert_Assign(temp.mid(317, 4), 0));
    AppendTSL(temp.mid(316, 43), "User_patch%assign(2)");
    temp.replace(374, 4,  convert_Assign(temp.mid(374, 4), 0));
    AppendTSL(temp.mid(373, 43), "User_patch%assign(3)");
    temp.replace(438, 4,  convert_Assign(temp.mid(438, 4), 0));
    AppendTSL(temp.mid(437, 43), "User_patch%assign(4)");
    temp.replace(495, 4,  convert_Assign(temp.mid(495, 4), 0));
    AppendTSL(temp.mid(494, 43), "User_patch%assign(5)");
    temp.replace(559, 4,  convert_Assign(temp.mid(559, 4), 0));
    AppendTSL(temp.mid(558, 43), "User_patch%assign(6)");
    temp.replace(616, 4,  convert_Assign(temp.mid(616, 4), 0));
    AppendTSL(temp.mid(615, 43), "User_patch%assign(7)");
    temp.replace(680, 4,  convert_Assign(temp.mid(680, 4), 0));
    AppendTSL(temp.mid(679, 43), "User_patch%assign(8)");
    temp.replace(737, 4,  convert_Assign(temp.mid(737, 4), 0));
    AppendTSL(temp.mid(736, 43), "User_patch%assign(9)");
    temp.replace(801, 4,  convert_Assign(temp.mid(801, 4), 0));
    AppendTSL(temp.mid(800, 43), "User_patch%assign(10)");
    temp.replace(858, 4,  convert_Assign(temp.mid(858, 4), 0));
    AppendTSL(temp.mid(857, 43), "User_patch%assign(11)");
    temp.replace(922, 4,  convert_Assign(temp.mid(922, 4), 0));
    AppendTSL(temp.mid(921, 43), "User_patch%assign(12)");
    temp.replace(979, 4,  convert_Assign(temp.mid(979, 4), 0));
    AppendTSL(temp.mid(978, 43), "User_patch%assign(13)");
    temp.replace(1043, 4,  convert_Assign(temp.mid(1043, 4), 0));
    AppendTSL(temp.mid(1042, 43), "User_patch%assign(14)");
    temp.replace(1100, 4,  convert_Assign(temp.mid(1100, 4), 0));
    AppendTSL(temp.mid(1099, 43), "User_patch%assign(15)");
    temp.replace(1164, 4,  convert_Assign(temp.mid(1164, 4), 0));
    AppendTSL(temp.mid(1163, 43), "User_patch%assign(16)");
    AppendTSL(temp.mid(1220, 103), "User_patch%efct");
    AppendTSL(temp.mid(1337, 7),   "User_patch%gtr2midi");
    AppendTSL(temp.mid(1358, 18), "User_patch%inst(1)%inst");
    t = temp.mid(1390, 128);
    t.append(temp.mid(1532, 78));
    AppendTSL(t, "User_patch%inst(1)%instDynaSynth");
    AppendTSL(temp.mid(1624, 65), "User_patch%inst(1)%instOscSynth");
    AppendTSL(temp.mid(1703, 21), "User_patch%inst(1)%instGr300");

    //AppendTSL(temp.mid(1738, 46), "User_patch%inst(1)%instEBass");

    //t = temp.mid(1809, 43); t.prepend(char(0)); t.prepend(char(0)); t.append(char(0)); t.append(char(0));

    //AppendTSL(t, "User_patch%inst(1)%instAcoustic");

    AppendTSL(temp.mid(1866, 45), "User_patch%inst(1)%instEGtr");

    //AppendTSL(temp.mid(1918, 37), "User_patch%inst(1)%instVioGtr");

    AppendTSL(temp.mid(1925, 19), "User_patch%inst(1)%instPolyFx");
    AppendTSL(temp.mid(1958, 48), "User_patch%inst(1)%instAltTune"); // reserved tune type to convert
    AppendTSL(temp.mid(2020, 18), "User_patch%inst(2)%inst");
    t = temp.mid(2052, 128);
    t.append(temp.mid(2194, 78));
    AppendTSL(t, "User_patch%inst(2)%instDynaSynth");
    AppendTSL(temp.mid(2286, 65), "User_patch%inst(2)%instOscSynth");
    AppendTSL(temp.mid(2365, 21), "User_patch%inst(2)%instGr300");
    //AppendTSL(temp.mid(2400, 46), "User_patch%inst(2)%instEBass");
    //t = temp.mid(2471, 43); t.prepend(char(0)); t.prepend(char(0)); t.append(char(0)); t.append(char(0));
    //AppendTSL(t, "User_patch%inst(2)%instAcoustic");
    AppendTSL(temp.mid(2528, 45), "User_patch%inst(2)%instEGtr");
    //AppendTSL(temp.mid(2624, 37), "User_patch%inst(2)%instVioGtr");
    AppendTSL(temp.mid(2587, 19), "User_patch%inst(2)%instPolyFx");
    AppendTSL(temp.mid(2620, 48), "User_patch%inst(2)%instAltTune");
    AppendTSL(temp.mid(2682, 18), "User_patch%inst(3)%inst");
    t = temp.mid(2714, 128);
    t.append(temp.mid(2856, 78));
    AppendTSL(t, "User_patch%inst(3)%instDynaSynth");
    AppendTSL(temp.mid(2948, 65), "User_patch%inst(3)%instOscSynth");
    AppendTSL(temp.mid(3027, 21), "User_patch%inst(3)%instGr300");
    //AppendTSL(temp.mid(3062, 46), "User_patch%inst(3)%instEBass");
    //t = temp.mid(3133, 43); t.prepend(char(0)); t.prepend(char(0)); t.append(char(0)); t.append(char(0));
    //AppendTSL(t, "User_patch%inst(3)%instAcoustic");
    AppendTSL(temp.mid(3190, 45), "User_patch%inst(3)%instEGtr");
    //AppendTSL(temp.mid(3330, 37), "User_patch%inst(3)%instVioGtr");
    AppendTSL(temp.mid(3249, 19), "User_patch%inst(3)%instPolyFx");
    AppendTSL(temp.mid(3282, 48), "User_patch%inst(3)%instAltTune");

    AppendTSL(temp.mid(3344, 7), "User_patch%comp");
    AppendTSL(temp.mid(3365, 9), "User_patch%dist");
    AppendTSL(temp.mid(3388, 20), "User_patch%amp");
    AppendTSL(temp.mid(3422, 4), "User_patch%ns");
    AppendTSL(temp.mid(3440, 12), "User_patch%eq(1)");
    AppendTSL(temp.mid(3466, 12), "User_patch%eq(2)");
    AppendTSL(temp.mid(3492, 10), "User_patch%delay(1)");
    AppendTSL(temp.mid(3516, 10), "User_patch%delay(2)");
    AppendTSL(temp.mid(3540, 33), "User_patch%mstDelay");
    AppendTSL(temp.mid(3587, 25), "User_patch%chorus");

    AppendTSL(temp.mid(3626, 2), "User_patch%fx(1)%fx");
    AppendTSL(temp.mid(3642, 4), "User_patch%fx(1)%fxAcReso");
    AppendTSL(temp.mid(3660, 8), "User_patch%fx(1)%fxAWah");
    AppendTSL(temp.mid(3682, 25), "User_patch%fx(1)%fxChorus");
    AppendTSL(temp.mid(3721, 3), "User_patch%fx(1)%fxCVibe");
    AppendTSL(temp.mid(3738, 7), "User_patch%fx(1)%fxComp");
    AppendTSL(temp.mid(3759, 7), "User_patch%fx(1)%fxDefretter");
    AppendTSL(temp.mid(3790, 5), "User_patch%fx(1)%fxDefretterBass");
    AppendTSL(temp.mid(3799, 33), "User_patch%fx(1)%fxDelay");
    AppendTSL(temp.mid(3846, 9), "User_patch%fx(1)%fxFlanger");
    AppendTSL(temp.mid(3869, 9), "User_patch%fx(1)%fxFlangerBass");
    AppendTSL(temp.mid(3892, 13), "User_patch%fx(1)%fxFv");
    AppendTSL(temp.mid(3919, 8), "User_patch%fx(1)%fxGEq");
    AppendTSL(temp.mid(3941, 39), "User_patch%fx(1)%fxHarmonist");
    AppendTSL(temp.mid(3994, 8), "User_patch%fx(1)%fxHumanizer");
    AppendTSL(temp.mid(4016, 4), "User_patch%fx(1)%fxIsolator");
    AppendTSL(temp.mid(4034, 6), "User_patch%fx(1)%fxLimiter");
    AppendTSL(temp.mid(4054, 3), "User_patch%fx(1)%fxLofi");
    AppendTSL(temp.mid(4071, 3), "User_patch%fx(1)%fxOctave");
    AppendTSL(temp.mid(4088, 3), "User_patch%fx(1)%fxOctaveBass");
    AppendTSL(temp.mid(4105, 6), "User_patch%fx(1)%fxPan");
    AppendTSL(temp.mid(4125, 12), "User_patch%fx(1)%fxPEq");
    AppendTSL(temp.mid(4151, 7), "User_patch%fx(1)%fxPedalBend");
    AppendTSL(temp.mid(4172, 8), "User_patch%fx(1)%fxPhaser");
    AppendTSL(temp.mid(4194, 19), "User_patch%fx(1)%fxPitchShift");
    AppendTSL(temp.mid(4227, 14), "User_patch%fx(1)%fxReverb");
    AppendTSL(temp.mid(4255, 6), "User_patch%fx(1)%fxRingMod");
    AppendTSL(temp.mid(4275, 8), "User_patch%fx(1)%fxRotary");
    AppendTSL(temp.mid(4297, 7), "User_patch%fx(1)%fxSitarSim");
    AppendTSL(temp.mid(4318, 7), "User_patch%fx(1)%fxSlicer");
    AppendTSL(temp.mid(4339, 3), "User_patch%fx(1)%fxSlowGear");
    AppendTSL(temp.mid(4356, 3), "User_patch%fx(1)%fxSlowGearBass");
    AppendTSL(temp.mid(4373, 3), "User_patch%fx(1)%fxSoundHold");
    AppendTSL(temp.mid(4390, 8), "User_patch%fx(1)%fxTWah");
    AppendTSL(temp.mid(4412, 8), "User_patch%fx(1)%fxTWahBass");
    AppendTSL(temp.mid(4434, 5), "User_patch%fx(1)%fxTremolo");
    AppendTSL(temp.mid(4453, 6), "User_patch%fx(1)%fxVibrato");
    AppendTSL(temp.mid(4473, 15), "User_patch%fx(1)%fxWah");

    AppendTSL(temp.mid(4502, 2), "User_patch%fx(2)%fx");
    AppendTSL(temp.mid(4518, 4), "User_patch%fx(2)%fxAcReso");
    AppendTSL(temp.mid(4536, 8), "User_patch%fx(2)%fxAWah");
    AppendTSL(temp.mid(4558, 25), "User_patch%fx(2)%fxChorus");
    AppendTSL(temp.mid(4597, 3), "User_patch%fx(2)%fxCVibe");
    AppendTSL(temp.mid(4614, 7), "User_patch%fx(2)%fxComp");
    AppendTSL(temp.mid(4635, 7), "User_patch%fx(2)%fxDefretter");
    AppendTSL(temp.mid(4656, 5), "User_patch%fx(2)%fxDefretterBass");
    AppendTSL(temp.mid(4675, 33), "User_patch%fx(2)%fxDelay");
    AppendTSL(temp.mid(4722, 9), "User_patch%fx(2)%fxFlanger");
    AppendTSL(temp.mid(4745, 9), "User_patch%fx(2)%fxFlangerBass");
    AppendTSL(temp.mid(4768, 13), "User_patch%fx(2)%fxFv");
    AppendTSL(temp.mid(4795, 8), "User_patch%fx(2)%fxGEq");
    AppendTSL(temp.mid(4817, 39), "User_patch%fx(2)%fxHarmonist");
    AppendTSL(temp.mid(4870, 8), "User_patch%fx(2)%fxHumanizer");
    AppendTSL(temp.mid(4892, 4), "User_patch%fx(2)%fxIsolator");
    AppendTSL(temp.mid(4910, 6), "User_patch%fx(2)%fxLimiter");
    AppendTSL(temp.mid(4930, 3), "User_patch%fx(2)%fxLofi");
    AppendTSL(temp.mid(4947, 3), "User_patch%fx(2)%fxOctave");
    AppendTSL(temp.mid(4964, 3), "User_patch%fx(2)%fxOctaveBass");
    AppendTSL(temp.mid(4981, 6), "User_patch%fx(2)%fxPan");
    AppendTSL(temp.mid(5001, 12), "User_patch%fx(2)%fxPEq");
    AppendTSL(temp.mid(5027, 7), "User_patch%fx(2)%fxPedalBend");
    AppendTSL(temp.mid(5048, 8), "User_patch%fx(2)%fxPhaser");
    AppendTSL(temp.mid(5070, 19), "User_patch%fx(2)%fxPitchShift");
    AppendTSL(temp.mid(5103, 14), "User_patch%fx(2)%fxReverb");
    AppendTSL(temp.mid(5131, 6), "User_patch%fx(2)%fxRingMod");
    AppendTSL(temp.mid(5151, 8), "User_patch%fx(2)%fxRotary");
    AppendTSL(temp.mid(5173, 7), "User_patch%fx(2)%fxSitarSim");
    AppendTSL(temp.mid(5194, 7), "User_patch%fx(2)%fxSlicer");
    AppendTSL(temp.mid(5215, 3), "User_patch%fx(2)%fxSlowGear");
    AppendTSL(temp.mid(5232, 3), "User_patch%fx(2)%fxSlowGearBass");
    AppendTSL(temp.mid(5249, 3), "User_patch%fx(2)%fxSoundHold");
    AppendTSL(temp.mid(5266, 8), "User_patch%fx(2)%fxTWah");
    AppendTSL(temp.mid(5288, 8), "User_patch%fx(2)%fxTWahBass");
    AppendTSL(temp.mid(5310, 5), "User_patch%fx(2)%fxTremolo");
    AppendTSL(temp.mid(5329, 6), "User_patch%fx(2)%fxVibrato");
    AppendTSL(temp.mid(5349, 15), "User_patch%fx(2)%fxWah");

    AppendTSL(temp.mid(5378, 2), "User_patch%fx(3)%fx"); //+1752
    AppendTSL(temp.mid(5394, 4), "User_patch%fx(3)%fxAcReso");
    AppendTSL(temp.mid(5412, 8), "User_patch%fx(3)%fxAWah");
    AppendTSL(temp.mid(5434, 25), "User_patch%fx(3)%fxChorus");
    AppendTSL(temp.mid(5473, 3), "User_patch%fx(3)%fxCVibe");
    AppendTSL(temp.mid(5490, 7), "User_patch%fx(3)%fxComp");
    AppendTSL(temp.mid(5511, 7), "User_patch%fx(3)%fxDefretter");
    AppendTSL(temp.mid(5532, 5), "User_patch%fx(3)%fxDefretterBass");
    AppendTSL(temp.mid(5551, 33), "User_patch%fx(3)%fxDelay");
    AppendTSL(temp.mid(5598, 9), "User_patch%fx(3)%fxFlanger");
    AppendTSL(temp.mid(5621, 9), "User_patch%fx(3)%fxFlangerBass");
    AppendTSL(temp.mid(5644, 13), "User_patch%fx(3)%fxFv");
    AppendTSL(temp.mid(5671, 8), "User_patch%fx(3)%fxGEq");
    AppendTSL(temp.mid(5693, 39), "User_patch%fx(3)%fxHarmonist");
    AppendTSL(temp.mid(5746, 8), "User_patch%fx(3)%fxHumanizer");
    AppendTSL(temp.mid(5768, 4), "User_patch%fx(3)%fxIsolator");
    AppendTSL(temp.mid(5786, 6), "User_patch%fx(3)%fxLimiter");
    AppendTSL(temp.mid(5806, 3), "User_patch%fx(3)%fxLofi");
    AppendTSL(temp.mid(5823, 3), "User_patch%fx(3)%fxOctave");
    AppendTSL(temp.mid(5940, 3), "User_patch%fx(3)%fxOctaveBass");
    AppendTSL(temp.mid(5857, 6), "User_patch%fx(3)%fxPan");
    AppendTSL(temp.mid(5877, 12), "User_patch%fx(3)%fxPEq");
    AppendTSL(temp.mid(5903, 7), "User_patch%fx(3)%fxPedalBend");
    AppendTSL(temp.mid(5924, 8), "User_patch%fx(3)%fxPhaser");
    AppendTSL(temp.mid(5946, 19), "User_patch%fx(3)%fxPitchShift");
    AppendTSL(temp.mid(5979, 14), "User_patch%fx(3)%fxReverb");
    AppendTSL(temp.mid(6007, 6), "User_patch%fx(3)%fxRingMod");
    AppendTSL(temp.mid(6028, 8), "User_patch%fx(3)%fxRotary");
    AppendTSL(temp.mid(6049, 7), "User_patch%fx(3)%fxSitarSim");
    AppendTSL(temp.mid(6070, 7), "User_patch%fx(3)%fxSlicer");
    AppendTSL(temp.mid(6091, 3), "User_patch%fx(3)%fxSlowGear");
    AppendTSL(temp.mid(6108, 3), "User_patch%fx(3)%fxSlowGearBass");
    AppendTSL(temp.mid(6125, 3), "User_patch%fx(3)%fxSoundHold");
    AppendTSL(temp.mid(6142, 8), "User_patch%fx(3)%fxTWah");
    AppendTSL(temp.mid(6164, 8), "User_patch%fx(3)%fxTWahBass");
    AppendTSL(temp.mid(6186, 5), "User_patch%fx(3)%fxTremolo");
    AppendTSL(temp.mid(6205, 6), "User_patch%fx(3)%fxVibrato");
    AppendTSL(temp.mid(6225, 15), "User_patch%fx(3)%fxWah");
    AppendTSL(temp.mid(6254, 14), "User_patch%reverb");
}

//*****************************************************************************  default Bass to Guitar tsl **********
void sysxWriter::hex_B_to_TSL_B()
{

    QFile TSLfile(":default_B.tsl");           // Read the default KATANA BASS TSL file .
    if (TSLfile.open(QIODevice::ReadOnly))
    {
        this->TSL_default = TSLfile.readAll();
        TSLfile.close();
    };

    QByteArray t;
    t.append(temp.mid(12, 128));
    t.append(temp.mid(154, 27));
    AppendTSL(t, "UserBass_patch%common");
    AppendTSL(temp.mid(211, 27), "UserBass_patch%led");
    AppendTSL(temp.mid(252, 43), "UserBass_patch%assign(1)");
    AppendTSL(temp.mid(316, 43), "UserBass_patch%assign(2)");
    AppendTSL(temp.mid(373, 43), "UserBass_patch%assign(3)");
    AppendTSL(temp.mid(437, 43), "UserBass_patch%assign(4)");
    AppendTSL(temp.mid(494, 43), "UserBass_patch%assign(5)");
    AppendTSL(temp.mid(558, 43), "UserBass_patch%assign(6)");
    AppendTSL(temp.mid(615, 43), "UserBass_patch%assign(7)");
    AppendTSL(temp.mid(679, 43), "UserBass_patch%assign(8)");
    AppendTSL(temp.mid(736, 43), "UserBass_patch%assign(9)");
    AppendTSL(temp.mid(800, 43), "UserBass_patch%assign(10)");
    AppendTSL(temp.mid(857, 43), "UserBass_patch%assign(11)");
    AppendTSL(temp.mid(921, 43), "UserBass_patch%assign(12)");
    AppendTSL(temp.mid(978, 43), "UserBass_patch%assign(13)");
    AppendTSL(temp.mid(1042, 43), "UserBass_patch%assign(14)");
    AppendTSL(temp.mid(1099, 43), "UserBass_patch%assign(15)");
    AppendTSL(temp.mid(1163, 43), "UserBass_patch%assign(16)");
    AppendTSL(temp.mid(1220, 103), "UserBass_patch%efct");
    AppendTSL(temp.mid(1337, 7),   "UserBass_patch%gtr2midi");
    AppendTSL(temp.mid(1358, 18), "UserBass_patch%inst(1)%inst");
    t = temp.mid(1390, 128);
    t.append(temp.mid(1532, 78));
    AppendTSL(t, "UserBass_patch%inst(1)%instDynaSynth");
    AppendTSL(temp.mid(1624, 65), "UserBass_patch%inst(1)%instOscSynth");
    AppendTSL(temp.mid(1703, 21), "UserBass_patch%inst(1)%instAnalogGr");
    AppendTSL(temp.mid(1738, 57), "UserBass_patch%inst(1)%instEBass");
    AppendTSL(temp.mid(1809, 43), "UserBass_patch%inst(1)%instAcBass");
    AppendTSL(temp.mid(1866, 45), "UserBass_patch%inst(1)%instEGtr");
    AppendTSL(temp.mid(1925, 19), "UserBass_patch%inst(1)%instPolyFx");
    AppendTSL(temp.mid(1958, 48), "UserBass_patch%inst(1)%instAltTune");
    AppendTSL(temp.mid(2020, 18), "UserBass_patch%inst(2)%inst");
    t = temp.mid(2052, 128);
    t.append(temp.mid(2194, 78));
    AppendTSL(t, "UserBass_patch%inst(2)%instDynaSynth");
    AppendTSL(temp.mid(2286, 65), "UserBass_patch%inst(2)%instOscSynth");
    AppendTSL(temp.mid(2365, 21), "UserBass_patch%inst(2)%instAnalogGr");
    AppendTSL(temp.mid(2400, 57), "UserBass_patch%inst(2)%instEBass");
    AppendTSL(temp.mid(2471, 43), "UserBass_patch%inst(2)%instAcBass");
    AppendTSL(temp.mid(2528, 45), "UserBass_patch%inst(2)%instEGtr");
    AppendTSL(temp.mid(2587, 19), "UserBass_patch%inst(2)%instPolyFx");
    AppendTSL(temp.mid(2620, 48), "UserBass_patch%inst(2)%instAltTune");
    AppendTSL(temp.mid(2682, 18), "UserBass_patch%inst(3)%inst");
    t = temp.mid(2714, 128);
    t.append(temp.mid(2856, 78));
    AppendTSL(t, "UserBass_patch%inst(3)%instDynaSynth");
    AppendTSL(temp.mid(2948, 65), "UserBass_patch%inst(3)%instOscSynth");
    AppendTSL(temp.mid(3027, 21), "UserBass_patch%inst(3)%instAnalogGr");
    AppendTSL(temp.mid(3062, 57), "UserBass_patch%inst(3)%instEBass");
    AppendTSL(temp.mid(3133, 43), "UserBass_patch%inst(3)%instAcBass");
    AppendTSL(temp.mid(3190, 45), "UserBass_patch%inst(3)%instEGtr");
    AppendTSL(temp.mid(3249, 19), "UserBass_patch%inst(3)%instPolyFx");
    AppendTSL(temp.mid(3282, 48), "UserBass_patch%inst(3)%instAltTune");

    AppendTSL(temp.mid(3344, 7), "UserBass_patch%comp");
    AppendTSL(temp.mid(3365, 9), "UserBass_patch%dist");
    AppendTSL(temp.mid(3388, 20), "UserBass_patch%amp");
    AppendTSL(temp.mid(3422, 4), "UserBass_patch%ns");
    AppendTSL(temp.mid(3440, 12), "UserBass_patch%eq(1)");
    AppendTSL(temp.mid(3466, 12), "UserBass_patch%eq(2)");
    AppendTSL(temp.mid(3492, 10), "UserBass_patch%delay(1)");
    AppendTSL(temp.mid(3516, 10), "UserBass_patch%delay(2)");
    AppendTSL(temp.mid(3540, 33), "UserBass_patch%mstDelay");
    AppendTSL(temp.mid(3587, 25), "UserBass_patch%chorus");

    AppendTSL(temp.mid(3626, 2), "UserBass_patch%fx(1)%fx");
    AppendTSL(temp.mid(3642, 4), "UserBass_patch%fx(1)%fxAcReso");
    AppendTSL(temp.mid(3660, 8), "UserBass_patch%fx(1)%fxAWah");
    AppendTSL(temp.mid(3682, 25), "UserBass_patch%fx(1)%fxChorus");
    AppendTSL(temp.mid(3721, 3), "UserBass_patch%fx(1)%fxCVibe");
    AppendTSL(temp.mid(3738, 7), "UserBass_patch%fx(1)%fxComp");
    AppendTSL(temp.mid(3759, 7), "UserBass_patch%fx(1)%fxDefretter");
    AppendTSL(temp.mid(3780, 5), "UserBass_patch%fx(1)%fxDefretterBass");
    AppendTSL(temp.mid(3799, 33), "UserBass_patch%fx(1)%fxDelay");
    AppendTSL(temp.mid(3846, 9), "UserBass_patch%fx(1)%fxFlanger");
    AppendTSL(temp.mid(3869, 9), "UserBass_patch%fx(1)%fxFlangerBass");
    AppendTSL(temp.mid(3892, 13), "UserBass_patch%fx(1)%fxFv");
    AppendTSL(temp.mid(3919, 8), "UserBass_patch%fx(1)%fxGEq");
    AppendTSL(temp.mid(3941, 39), "UserBass_patch%fx(1)%fxHarmonist");
    AppendTSL(temp.mid(3994, 8), "UserBass_patch%fx(1)%fxHumanizer");
    AppendTSL(temp.mid(4016, 4), "UserBass_patch%fx(1)%fxIsolator");
    AppendTSL(temp.mid(4034, 6), "UserBass_patch%fx(1)%fxLimiter");
    AppendTSL(temp.mid(4054, 3), "UserBass_patch%fx(1)%fxLofi");
    AppendTSL(temp.mid(4071, 3), "UserBass_patch%fx(1)%fxOctave");
    AppendTSL(temp.mid(4088, 3), "UserBass_patch%fx(1)%fxOctaveBass");
    AppendTSL(temp.mid(4105, 6), "UserBass_patch%fx(1)%fxPan");
    AppendTSL(temp.mid(4125, 12), "UserBass_patch%fx(1)%fxPEq");
    AppendTSL(temp.mid(4151, 7), "UserBass_patch%fx(1)%fxPedalBend");
    AppendTSL(temp.mid(4172, 8), "UserBass_patch%fx(1)%fxPhaser");
    AppendTSL(temp.mid(4194, 19), "UserBass_patch%fx(1)%fxPitchShift");
    AppendTSL(temp.mid(4227, 14), "UserBass_patch%fx(1)%fxReverb");
    AppendTSL(temp.mid(4255, 6), "UserBass_patch%fx(1)%fxRingMod");
    AppendTSL(temp.mid(4275, 8), "UserBass_patch%fx(1)%fxRotary");
    AppendTSL(temp.mid(4297, 7), "UserBass_patch%fx(1)%fxSitarSim");
    AppendTSL(temp.mid(4318, 7), "UserBass_patch%fx(1)%fxSlicer");
    AppendTSL(temp.mid(4339, 3), "UserBass_patch%fx(1)%fxSlowGear");
    AppendTSL(temp.mid(4356, 3), "UserBass_patch%fx(1)%fxSlowGearBass");
    AppendTSL(temp.mid(4373, 3), "UserBass_patch%fx(1)%fxSoundHold");
    AppendTSL(temp.mid(4390, 8), "UserBass_patch%fx(1)%fxTWah");
    AppendTSL(temp.mid(4412, 8), "UserBass_patch%fx(1)%fxTWahBass");
    AppendTSL(temp.mid(4434, 5), "UserBass_patch%fx(1)%fxTremolo");
    AppendTSL(temp.mid(4453, 6), "UserBass_patch%fx(1)%fxVibrato");
    AppendTSL(temp.mid(4473, 15), "UserBass_patch%fx(1)%fxWah");

    AppendTSL(temp.mid(4502, 2), "UserBass_patch%fx(2)%fx");
    AppendTSL(temp.mid(4518, 4), "UserBass_patch%fx(2)%fxAcReso");
    AppendTSL(temp.mid(4536, 8), "UserBass_patch%fx(2)%fxAWah");
    AppendTSL(temp.mid(4558, 25), "UserBass_patch%fx(2)%fxChorus");
    AppendTSL(temp.mid(4597, 3), "UserBass_patch%fx(2)%fxCVibe");
    AppendTSL(temp.mid(4614, 7), "UserBass_patch%fx(2)%fxComp");
    AppendTSL(temp.mid(4635, 7), "UserBass_patch%fx(2)%fxDefretter");
    AppendTSL(temp.mid(4656, 5), "UserBass_patch%fx(2)%fxDefretterBass");
    AppendTSL(temp.mid(4675, 33), "UserBass_patch%fx(2)%fxDelay");
    AppendTSL(temp.mid(4722, 9), "UserBass_patch%fx(2)%fxFlanger");
    AppendTSL(temp.mid(4745, 9), "UserBass_patch%fx(2)%fxFlangerBass");
    AppendTSL(temp.mid(4768, 13), "UserBass_patch%fx(2)%fxFv");
    AppendTSL(temp.mid(4795, 8), "UserBass_patch%fx(2)%fxGEq");
    AppendTSL(temp.mid(4817, 39), "UserBass_patch%fx(2)%fxHarmonist");
    AppendTSL(temp.mid(4870, 8), "UserBass_patch%fx(2)%fxHumanizer");
    AppendTSL(temp.mid(4892, 4), "UserBass_patch%fx(2)%fxIsolator");
    AppendTSL(temp.mid(4910, 6), "UserBass_patch%fx(2)%fxLimiter");
    AppendTSL(temp.mid(4930, 3), "UserBass_patch%fx(2)%fxLofi");
    AppendTSL(temp.mid(4947, 3), "UserBass_patch%fx(2)%fxOctave");
    AppendTSL(temp.mid(4964, 3), "UserBass_patch%fx(2)%fxOctaveBass");
    AppendTSL(temp.mid(4981, 6), "UserBass_patch%fx(2)%fxPan");
    AppendTSL(temp.mid(5001, 12), "UserBass_patch%fx(2)%fxPEq");
    AppendTSL(temp.mid(5027, 7), "UserBass_patch%fx(2)%fxPedalBend");
    AppendTSL(temp.mid(5048, 8), "UserBass_patch%fx(2)%fxPhaser");
    AppendTSL(temp.mid(5070, 19), "UserBass_patch%fx(2)%fxPitchShift");
    AppendTSL(temp.mid(5103, 14), "UserBass_patch%fx(2)%fxReverb");
    AppendTSL(temp.mid(5131, 6), "UserBass_patch%fx(2)%fxRingMod");
    AppendTSL(temp.mid(5151, 8), "UserBass_patch%fx(2)%fxRotary");
    AppendTSL(temp.mid(5173, 7), "UserBass_patch%fx(2)%fxSitarSim");
    AppendTSL(temp.mid(5194, 7), "UserBass_patch%fx(2)%fxSlicer");
    AppendTSL(temp.mid(5215, 3), "UserBass_patch%fx(2)%fxSlowGear");
    AppendTSL(temp.mid(5232, 3), "UserBass_patch%fx(2)%fxSlowGearBass");
    AppendTSL(temp.mid(5249, 3), "UserBass_patch%fx(2)%fxSoundHold");
    AppendTSL(temp.mid(5266, 8), "UserBass_patch%fx(2)%fxTWah");
    AppendTSL(temp.mid(5288, 8), "UserBass_patch%fx(2)%fxTWahBass");
    AppendTSL(temp.mid(5310, 5), "UserBass_patch%fx(2)%fxTremolo");
    AppendTSL(temp.mid(5329, 6), "UserBass_patch%fx(2)%fxVibrato");
    AppendTSL(temp.mid(5349, 15), "UserBass_patch%fx(2)%fxWah");

    AppendTSL(temp.mid(5378, 2), "UserBass_patch%fx(3)%fx"); //+1752
    AppendTSL(temp.mid(5394, 4), "UserBass_patch%fx(3)%fxAcReso");
    AppendTSL(temp.mid(5412, 8), "UserBass_patch%fx(3)%fxAWah");
    AppendTSL(temp.mid(5434, 25), "UserBass_patch%fx(3)%fxChorus");
    AppendTSL(temp.mid(5473, 3), "UserBass_patch%fx(3)%fxCVibe");
    AppendTSL(temp.mid(5490, 7), "UserBass_patch%fx(3)%fxComp");
    AppendTSL(temp.mid(5511, 7), "UserBass_patch%fx(3)%fxDefretter");
    AppendTSL(temp.mid(5532, 5), "UserBass_patch%fx(3)%fxDefretterBass");
    AppendTSL(temp.mid(5551, 33), "UserBass_patch%fx(3)%fxDelay");
    AppendTSL(temp.mid(5598, 9), "UserBass_patch%fx(3)%fxFlanger");
    AppendTSL(temp.mid(5621, 9), "UserBass_patch%fx(3)%fxFlangerBass");
    AppendTSL(temp.mid(5644, 13), "UserBass_patch%fx(3)%fxFv");
    AppendTSL(temp.mid(5671, 8), "UserBass_patch%fx(3)%fxGEq");
    AppendTSL(temp.mid(5693, 39), "UserBass_patch%fx(3)%fxHarmonist");
    AppendTSL(temp.mid(5746, 8), "UserBass_patch%fx(3)%fxHumanizer");
    AppendTSL(temp.mid(5768, 4), "UserBass_patch%fx(3)%fxIsolator");
    AppendTSL(temp.mid(5786, 6), "UserBass_patch%fx(3)%fxLimiter");
    AppendTSL(temp.mid(5806, 3), "UserBass_patch%fx(3)%fxLofi");
    AppendTSL(temp.mid(5823, 3), "UserBass_patch%fx(3)%fxOctave");
    AppendTSL(temp.mid(5840, 3), "UserBass_patch%fx(3)%fxOctaveBass");
    AppendTSL(temp.mid(5857, 6), "UserBass_patch%fx(3)%fxPan");
    AppendTSL(temp.mid(5877, 12), "UserBass_patch%fx(3)%fxPEq");
    AppendTSL(temp.mid(5903, 7), "UserBass_patch%fx(3)%fxPedalBend");
    AppendTSL(temp.mid(5924, 8), "UserBass_patch%fx(3)%fxPhaser");
    AppendTSL(temp.mid(5946, 19), "UserBass_patch%fx(3)%fxPitchShift");
    AppendTSL(temp.mid(5979, 14), "UserBass_patch%fx(3)%fxReverb");
    AppendTSL(temp.mid(6007, 6), "UserBass_patch%fx(3)%fxRingMod");
    AppendTSL(temp.mid(6028, 8), "UserBass_patch%fx(3)%fxRotary");
    AppendTSL(temp.mid(6049, 7), "UserBass_patch%fx(3)%fxSitarSim");
    AppendTSL(temp.mid(6070, 7), "UserBass_patch%fx(3)%fxSlicer");
    AppendTSL(temp.mid(6091, 3), "UserBass_patch%fx(3)%fxSlowGear");
    AppendTSL(temp.mid(6108, 3), "UserBass_patch%fx(3)%fxSlowGearBass");
    AppendTSL(temp.mid(6125, 3), "UserBass_patch%fx(3)%fxSoundHold");
    AppendTSL(temp.mid(6142, 8), "UserBass_patch%fx(3)%fxTWah");
    AppendTSL(temp.mid(6164, 8), "UserBass_patch%fx(3)%fxTWahBass");
    AppendTSL(temp.mid(6186, 5), "UserBass_patch%fx(3)%fxTremolo");
    AppendTSL(temp.mid(6205, 6), "UserBass_patch%fx(3)%fxVibrato");
    AppendTSL(temp.mid(6225, 15), "UserBass_patch%fx(3)%fxWah");
    AppendTSL(temp.mid(6254, 14), "UserBass_patch%reverb");
}

void sysxWriter::hex_G_to_hex_B()
{
    hex_G_to_TSL_B();                  // from this->temp to this->tsl_default
    this->data.clear();
    this->data.append(this->TSL_default);

    QFile file(":/default_bass.syx");   // load bass mode default.syx
    if (file.open(QIODevice::ReadOnly))
    {
        this->SY1000_default.clear();
        this->SY1000_default.append(file.readAll());
    };
    file.close();
    this->a = 0;
    TSL_B_to_hex_B();                  // from this->data to SY1000_default
}
