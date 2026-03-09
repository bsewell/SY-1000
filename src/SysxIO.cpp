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

#include <QApplication>
#include <QMessageBox>
#include "SysxIO.h"
#include "SysxIODestroyer.h"
#include "midiIO.h"
#include "Preferences.h"
#include "sysxWriter.h"
#include "MidiTable.h"
#include "globalVariables.h"
#include <QThread>
#include <QTimer>
#include <QDebug>
#include <QDir>
#include <QStandardPaths>
//#include <QtTest/QtTest>

// Platform-dependent sleep routines.
#ifdef Q_OS_WIN
#include <windows.h>
#define SLEEP( milliseconds ) Sleep( (DWORD) milliseconds )
#else // Unix variants & Mac
#include <unistd.h>
#define SLEEP( milliseconds ) usleep( (unsigned long) (milliseconds * 1000.0) )
#endif

SysxIO::SysxIO() 
{
    this->setConnected(false);
    this->setDevice(false);
    this->setDeviceReady(true);
    this->setSyncStatus(false);
    this->setNoError(true);
    this->setBank(0);
    this->setPatch(0);
    this->setLoadedBank(0);
    this->setLoadedPatch(0);
    this->changeCount = 0;
    this->rxTimer = new QTimer(this);
    QObject::connect(this->rxTimer, SIGNAL(timeout()), this, SLOT(relayUpdateSignal()));
    this->mode = 0;
    patch_loaded = false;
    last_index = -1;
    start_sequence = 0;
    this->spoolTimer = new QTimer(this);
    QObject::connect(this->spoolTimer, SIGNAL(timeout()), this, SLOT(processSpooler()));
    this->spoolTimer->start(3000);
}

SysxIO* SysxIO::_instance = 0;// initialize pointer
SysxIODestroyer SysxIO::_destroyer;

SysxIO* SysxIO::Instance() 
{
    /* Multi-threading safe */
    if (!_instance)  // is it the first call?
    {
        _instance = new SysxIO; // create sole instance
        _destroyer.SetSysxIO(_instance);
    };
    return _instance; // address of sole instance
}

void SysxIO::connectResetDeviceSlot()
{
    QObject::disconnect(this, SIGNAL(sysxReply(QString)),
                        this, SLOT(resetDevice(QString)));
    QObject::connect(this, SIGNAL(sysxReply(QString)),
                     this, SLOT(resetDevice(QString)));
}

void SysxIO::queueOrSendDeviceWrite(const QString &sysxMsg, int replySize)
{
    if(!this->isConnected() || sysxMsg.isEmpty())
    {
        return;
    }

    connectResetDeviceSlot();

    if(this->deviceReady())
    {
        this->setDeviceReady(false);
        emit setStatusSymbol(2);
        emit setStatusMessage(tr("Sending"));
        this->sysxOutput(sysxMsg, replySize);
        return;
    }

    this->deferredWriteSpooler.append(sysxMsg);
    this->deferredReplySizes.append(qMax(0, replySize));
    emit setStatusSymbol(2);
    emit setStatusMessage(tr("Queued"));
}

bool SysxIO::flushDeferredDeviceWrite()
{
    if(this->deferredWriteSpooler.isEmpty())
    {
        return false;
    }

    const QString sysxMsg = this->deferredWriteSpooler.takeFirst();
    const int replySize = this->deferredReplySizes.isEmpty() ? 0
                                                             : this->deferredReplySizes.takeFirst();

    connectResetDeviceSlot();
    this->setDeviceReady(false);
    emit setStatusSymbol(2);
    emit setStatusMessage(tr("Sending"));
    this->sysxOutput(sysxMsg, replySize);
    return true;
}

void SysxIO::setFileSource(QString area, SysxData fileSource)
{ 
    if (area.contains("System"))
    {
        this->systemSource = fileSource;
    }
    else
    {
        this->fileSource = fileSource;
    };
}

void SysxIO::setFileSource(QString area, QByteArray data)
{
    if (area.contains("System"))
    {
        this->systemSource.address.clear();
        this->systemSource.hex.clear();
    }
    else
    {
        this->fileSource.address.clear();
        this->fileSource.hex.clear();
    };

    QString errorList;
    QList<QString> sysxBuffer;
    int dataSize = 0;
    int offset = 0;

    for(int i=0;i<data.size();i++)
    {
        uint n = uint(uchar(data[i]));
        QString hex = QString::number(n, 16).toUpper();
        if (hex.length() < 2) hex.prepend("0");
        sysxBuffer.append(hex);

        uchar nextbyte = uchar(data[i+1]);
        uint nextn = uint(nextbyte);
        QString nexthex = QString::number(nextn, 16).toUpper();
        if (nexthex.length() < 2) nexthex.prepend("0");

        if(offset >= checksumOffset && nexthex != "F7")
        {
            dataSize += n;
        };

        if(nexthex == "F7")
        {
            QString checksum = hex;

            if(getCheckSum(dataSize) != checksum)
            {
                QString errorString;
                errorString.append(tr("Address") + ": ");
                errorString.append(sysxBuffer.at(sysxAddressOffset) + " ");
                errorString.append(sysxBuffer.at(sysxAddressOffset + 1) + " ");
                errorString.append(sysxBuffer.at(sysxAddressOffset + 2) + " ");
                errorString.append(sysxBuffer.at(sysxAddressOffset + 3) + " - ");
                errorString.append(tr("checksum") + " (" + checksum + ") ");
                errorString.append(tr("should have been") + " (" + getCheckSum(dataSize) + ")");
                errorString.append("      ");
                errorList.append(errorString);

                //sysxBuffer = correctSysxMsg(sysxBuffer);
            };
            //if(area != "System") {sysxBuffer = correctSysxMsg(sysxBuffer);};
        };
        offset++;

        if(hex == "F7")
        {
            if (area.contains("System"))
            {
                this->systemSource.address.append( sysxBuffer.at(sysxAddressOffset) + sysxBuffer.at(sysxAddressOffset + 1) + sysxBuffer.at(sysxAddressOffset + 2));
                this->systemSource.hex.append(sysxBuffer);
                for(int x=0; x<sysxDataOffset; x++){sysxBuffer.removeFirst();};  //strip the sysx header and address, and the checksum and EOF from the data.
                sysxBuffer.removeLast();
                sysxBuffer.removeLast();
            }
            else
            {
                this->fileSource.address.append( sysxBuffer.at(sysxAddressOffset) + sysxBuffer.at(sysxAddressOffset + 1) + sysxBuffer.at(sysxAddressOffset + 2));
                this->fileSource.hex.append(sysxBuffer);
                for(int x=0; x<sysxDataOffset; x++){sysxBuffer.removeFirst();};  //strip the sysx header and address, and the checksum and EOF from the data.
                sysxBuffer.removeLast();
                sysxBuffer.removeLast();
            };
            sysxBuffer.clear();
            dataSize = 0;
            offset = 0;
        };
    };

    //deBug() << snork;

    if(!errorList.isEmpty())
    {
        //qDebug() << "setFileSource checksum error=" << errorList;
    };
}

// *** Replaces sections the existing default.syx or system.syx file data. Data is generally incoming from SY-1000 via midi ***
void SysxIO::setFileSource(QString area, QString data)
{
    if((data.size()/2!=patchSizeBass) || (data.size()/2!=patchSize))
    {
        QString hex0, hex1, hex2, hex3;
        int x=0;
        QList<QString> list;
        QString str;
        for(int i=0; i<(data.size()); i++)
        {
            if(i==x+16){hex0 = data.at(i); hex0.append(data.at(i+1));};
            if(i==x+18){hex1 = data.at(i); hex1.append(data.at(i+1));};
            if(i==x+20){hex2 = data.at(i); hex2.append(data.at(i+1));};
            if(i==x+22){hex3 = data.at(i); hex3.append(data.at(i+1));};
            QString hex = data.at(i); hex.append(data.at(i+1));
            if(i>x+23){list.append(hex);};
            if(i>x+23){str.append(hex);};

            if(hex == "F7")
            {
                list.removeLast();
                list.removeLast();
                setFileSource(hex0, hex1, hex2, hex3, list, false); // false = do not send sysx to midi out.
                x=i+2;
                list.clear();
                str.truncate(str.size()-4);

                /*QString deBug;
                deBug.append(hex0 + " " + hex1 + " " + hex2 + " " + hex3 + " size=" + QString::number(str.size()/2, 10) + " string="+ QString(str));
                std::string st = deBug.toStdString();
                std::cerr << st << std::endl; */

                str.clear();
            };
            i=i+1;
        };
    }
    else
    {
        if (area.contains("System"))
        {
            this->systemSource.address.clear();
            this->systemSource.hex.clear();
            QList<QString> sysxBuffer;
            for(int i=0;i<data.size();i++)
            {
                QString hex = data.mid(i, 2);
                sysxBuffer.append(hex);
                i++;
                if(hex == "F7")
                {
                    this->systemSource.address.append( sysxBuffer.at(sysxAddressOffset) + sysxBuffer.at(sysxAddressOffset + 1) + sysxBuffer.at(sysxAddressOffset + 2) );
                    this->systemSource.hex.append(sysxBuffer);
                    sysxBuffer.clear();
                };
            };
        }
        else
        {
            this->fileSource.address.clear();
            this->fileSource.hex.clear();
            QList<QString> sysxBuffer;
            for(int i=0;i<data.size();i++)
            {
                QString hex = data.mid(i, 2);
                sysxBuffer.append(hex);
                i++;
                if(hex == "F7")
                {
                    this->fileSource.address.append( sysxBuffer.at(sysxAddressOffset) + sysxBuffer.at(sysxAddressOffset + 1) + sysxBuffer.at(sysxAddressOffset + 2) );
                    this->fileSource.hex.append(sysxBuffer);
                    sysxBuffer.clear();
                };
            };
        };
    };
}

void SysxIO::setFileSource(QString hex0, QString hex1, QString hex2, QString hex3, QString hex4, bool send)
{
    MidiTable *midiTable = MidiTable::Instance();
    bool ok;
    bool assign = false;
    if ((hex1=="00" || hex1=="02") && (hex2.toInt(&ok, 16)>3) && (hex2.toInt(&ok, 16)<12) && (hex0.toInt(&ok, 16)>=10)){ assign = true; };
    QString area = "Structure";
    if (hex0.toInt(&ok, 16)<10){area = "System";};  //check if a system address. < 10.00.00.00

    QString address;
    address.append(hex0);
    address.append(hex1);
    address.append(hex2);

    int index = hex3.toInt(&ok, 16) + sysxDataOffset;

    QList<QString> sysxList;
    if(area.contains("System"))
    {
        sysxList = this->systemSource.hex.at(this->systemSource.address.indexOf(address));
    }
    else
    {
        sysxList = this->fileSource.hex.at(this->fileSource.address.indexOf(address));
    };
    sysxList.replace(index, hex4);

    int dataSize = 0;
    for(int i=sysxList.size() - 3; i>=checksumOffset;i--)
    {
        dataSize += sysxList.at(i).toInt(&ok, 16);
    };
    sysxList.replace(sysxList.size() - 2, getCheckSum(dataSize));

    if(area.contains("System"))
    {
        this->systemSource.hex.replace(this->systemSource.address.indexOf(address), sysxList);
    }
    else
    {
        this->fileSource.hex.replace(this->fileSource.address.indexOf(address), sysxList);
    };
    if(send == true)
    {
        if(assign)
        {
            QList<QString> hex; this->setFileSource(hex0, hex1, hex2, hex3, hex);
        }
        else
        {
            QString sysxMsg = midiTable->dataChange(hex0, hex1, hex2, hex3, hex4);

            queueOrSendDeviceWrite(sysxMsg, 0);
        };
    };
}

void SysxIO::setFileSource(QString hex0, QString hex1, QString hex2, QString hex3, QString hex4, QString hex5, bool send)
{
    MidiTable *midiTable = MidiTable::Instance();
    bool ok;
    QString area = "Structure";
    if (hex0.toInt(&ok, 16)<10){area = "System";};  //check if a system address. < 10.00.00.00

    QString address;
    address.append(hex0);
    address.append(hex1);
    address.append(hex2);

    QList<QString> sysxList;
    int addrIdx = -1;
    if (area.contains("System"))
    {
        addrIdx = this->systemSource.address.indexOf(address);
        if(addrIdx == -1) { return; }
        sysxList = this->systemSource.hex.at(addrIdx);
    }
    else
    {
        addrIdx = this->fileSource.address.indexOf(address);
        if(addrIdx == -1) { return; }
        sysxList = this->fileSource.hex.at(addrIdx);
    };


    int index = hex3.toInt(&ok, 16) + sysxDataOffset;
    if(index < 0 || index >= sysxList.size()) { return; }
    sysxList.replace(index, hex4);
    sysxList.replace(index + 1, hex5);

    int dataSize = 0;
    for(int i=sysxList.size() - 3; i>=checksumOffset;i--)
    {
        dataSize += sysxList.at(i).toInt(&ok, 16);
    };
    sysxList.replace(sysxList.size() - 2, getCheckSum(dataSize));

    if (area.contains("System"))
    {
        this->systemSource.hex.replace(addrIdx, sysxList);
    }
    else
    {
        this->fileSource.hex.replace(addrIdx, sysxList);
    };

    if(send == true)
    {
        QString sysxMsg = midiTable->dataChange(hex0, hex1, hex2, hex3, hex4, hex5);

        queueOrSendDeviceWrite(sysxMsg, 0);
    };
}

void SysxIO:: setFileSource(QString hex0, QString hex1, QString hex2, QString hex3, QList<QString> hexData, bool send)
{
    QString sysxMsg;
    QList<QString> sysxList;
    bool ok;
    bool assign = false;
    if ((hex1=="00" || hex1=="02") && (hex2.toInt(&ok, 16)>3) && (hex2.toInt(&ok, 16)<12) && (hex0.toInt(&ok, 16)>=10)){ assign = true; };  // if an Assign, send whole data block.
    QString address;
    address.append(hex0);
    address.append(hex1);
    address.append(hex2);
    QString area = "Structure";
    if (hex0.toInt(&ok, 16)<10){area = "System";};  //check if a system address. < 10.00.00.00
    int addrIdx = -1;
    if(hexData.isEmpty())
    {
        addrIdx = this->fileSource.address.indexOf(address);
        if(addrIdx == -1) { return; }
        sysxList = this->fileSource.hex.at(addrIdx);
    }
    else
    {
        int pointerOffset2 = hex3.toInt(&ok, 16);
        if (area != "System")
        {
            addrIdx = this->fileSource.address.indexOf(address);
            if(addrIdx == -1) { return; }
            sysxList = this->fileSource.hex.at(addrIdx);
            int x = 0;
            for(int i=0; i<hexData.size();++i)
            {
                if((i + sysxDataOffset + pointerOffset2) > (sysxDataOffset + 127))  // if data string crosses page boundry limit >127
                {
                    pointerOffset2 = 0;
                    x=0;
                    this->fileSource.hex.replace(addrIdx, sysxList);                         // save the data and start a sysxList with a new address
                    addrIdx = this->fileSource.address.indexOf(address);
                    if(addrIdx == -1) { return; }
                    sysxList = this->fileSource.hex.at(addrIdx);
                };
                int replaceIdx = x + (sysxDataOffset + pointerOffset2);
                if(replaceIdx >= 0 && replaceIdx < sysxList.size())
                {
                    sysxList.replace(replaceIdx, hexData.at(i));
                };
                x=x+1;
            };
            this->fileSource.hex.replace(addrIdx, sysxList);
        }
        else
        {
            addrIdx = this->systemSource.address.indexOf(address);
            if(addrIdx == -1) { return; }
            sysxList = this->systemSource.hex.at(addrIdx);
            for(int i=0; i<hexData.size();++i)
            {
                int replaceIdx = i + (sysxDataOffset + pointerOffset2);
                if(replaceIdx >= 0 && replaceIdx < sysxList.size())
                {
                    sysxList.replace(replaceIdx, hexData.at(i));
                };

            };
            this->systemSource.hex.replace(addrIdx, sysxList);
        };
    };
    if(send == true)
    {
        // fileSource has been modified, next part sends modified data to midi output.
        sysxMsg = "F041100000006912";
        sysxMsg.append(hex0);
        sysxMsg.append(hex1);
        sysxMsg.append(hex2);

        if(assign){if(hex3.toInt(&ok, 16)<45){hexData = sysxList.mid(12, 43); sysxMsg.append("00");}else{hexData = sysxList.mid(75, 43); sysxMsg.append("40");};}
        else{sysxMsg.append(hex3);};
        for(int i=0;i<hexData.size();++i)
        {
            sysxMsg.append(hexData.at(i));
        };
        int dataSize = 0;
        for(int i=checksumOffset; i<sysxMsg.size()-1;i++)
        {dataSize += sysxMsg.mid(i*2, 2).toInt(&ok, 16); };
        sysxMsg.append(getCheckSum(dataSize));
        sysxMsg.append("F7");

        queueOrSendDeviceWrite(sysxMsg, 0);
    };
}

QList<QString> SysxIO::getSourceItems(QString hex0, QString hex1, QString hex2, QString hex3)
{
    QList<QString> items = this->getFileSource(hex0, hex1, hex2, hex3);
    return items;
}

int SysxIO::getSourceValue(QString hex0, QString hex1, QString hex2, QString hex3)
{
    MidiTable *midiTable = MidiTable::Instance();
    int value;
    bool ok;
    QList<QString> items = this->getSourceItems(hex0, hex1, hex2, hex3);
    if(items.isEmpty()) { return 0; }   // data not yet loaded – avoid null-deref crash
    if(hex3=="7F")
    {
        QString hex_2 = QString::number(hex2.toInt(&ok, 16)+1, 16);
        if(hex_2.size()<2){hex_2.prepend("0");};
        items.append(this->getSourceItems(hex0, hex1, hex_2, "00"));
    }

    if(midiTable->isData(hex0, hex1, hex2, hex3) || midiTable->getRange(hex0, hex1, hex2, hex3)>127)
    {
        int maxRange = 128;
        int listindex = sysxDataOffset + QString(hex3).toInt(&ok, 16);
        if(listindex + 1 >= items.size()) { return 0; }
        int valueData1 = items.at(listindex).toInt(&ok, 16);
        int valueData2 = items.at(listindex + 1).toInt(&ok, 16);
        value = (valueData1 * maxRange) + valueData2;
    }
    else
    {
        int listindex = sysxDataOffset + QString(hex3).toInt(&ok, 16);
        if(listindex >= items.size()) { return 0; }
        value = items.at(listindex).toInt(&ok, 16);
    };
    return value;
}

/************************ resetDevice() ******************************
* Reset the device after sending a sysexmesage.
* And starts to processes the spooler if the device is free.
**********************************************************************/
void SysxIO::resetDevice(QString replyMsg)
{
    Q_UNUSED(replyMsg);
    if(this->sendSpooler.size() == 0)
    {
        this->setDeviceReady(true);	// Free the device after finishing interaction.
        if(flushDeferredDeviceWrite())
        {
            return;
        }

        QObject::disconnect(this, SIGNAL(sysxReply(QString)),
                            this, SLOT(resetDevice(QString)));

        emit setStatusSymbol(this->isConnected() ? 1 : 0);
        emit setStatusProgress(0);
        emit setStatusMessage(this->isConnected() ? tr("Ready") : tr("Not connected"));
    }
    else
    {
        processSpooler();
    };
}

/************************ processSpooler() ******************************
* Send message that are in the spooler due to that the device was busy.
* And eliminates multiple messages where only the value changes.
**********************************************************************/
void SysxIO::processSpooler()
{
    if(!this->sendSpooler.isEmpty())
    {
        QString sysxMsg;
        int size = 0;
        //for(int i=0; i<this->sendSpooler.size(); ++i)
        //{
            if(this->sendSpooler.size()>1)
            {
                QString currentMsg = this->sendSpooler.at(0);
                QString nextMsg = this->sendSpooler.at(1);

                if(currentMsg != nextMsg)
                {
                    sysxMsg = this->sendSpooler.at(0);
                    size = this->replySize.at(0);
                    if(sendSpooler.size()>5)
                    {
                        for(int r=0; r<sendSpooler.size()-1; r++)
                        {
                            this->sendSpooler.removeFirst();
                            this->replySize.removeFirst();
                        };
                    };
                }
                else
                {
                    this->sendSpooler.removeFirst();
                    this->replySize.removeFirst();
                };
            }
            else
            {
                    sysxMsg = this->sendSpooler.at(0);
                    size = this->replySize.at(0);

            };
            if(size<0){size = 0;};
            if(sysxMsg.size() > 100) {size = 0;};
            this->spoolTimer->start(this->replySize.at(0)+500);
            if(sysxMsg.size()>0)
            {
                //deBug("PROCESS SPOOLER - stack size=" + QString::number(this->sendSpooler.size(), 10) + " reply size=" + QString::number(this->replySize.size(), 10) + " device Ready =" + QString(QVariant(this->status).toString()) + " " + sysxMsg);
            };
            this->sendSpooler.removeFirst();
            this->replySize.removeFirst();
            if(!sysxMsg.mid(0, 17).contains("F0410000000069121") && sysxMsg.size()>11)
            {
                this->sendSysx(sysxMsg, size);
                //break;
            }
            else
            {
                //qDebug() << "processSpooler bad data" << sysxMsg;
                resetDevice("");
            };
            sysxMsg.clear();
        //};
    }
    else
    {
        resetDevice("");
    };
    //this->sendSpooler.clear();
    //this->replySize.clear();
}

void SysxIO::sysxOutput(QString sysxMsg, int size)
{
    if(sysxMsg.contains("F0410000000069127F"))
    {
        this->sendSpooler.prepend(sysxMsg);
        this->replySize.prepend(size);
    }
    else
    {
        this->sendSpooler.append(sysxMsg);
        this->replySize.append(size);
    };

    resetDevice("");
}

void SysxIO::setFileName(QString fileName)
{
    this->fileName = fileName;
}

QString SysxIO::getFileName()
{
    return this->fileName;
}

SysxData SysxIO::getFileSource()
{
    return this->fileSource;
}

SysxData SysxIO::getSystemSource()
{
    return this->systemSource;
}

QList<QString> SysxIO::getFileSource(QString hex0, QString hex1, QString hex2, QString hex3)
{
    Preferences *preferences = Preferences::Instance();

    Q_UNUSED(hex3);
    bool ok;
    QString address;
    address.append(hex0.toUpper());
    address.append(hex1.toUpper());
    address.append(hex2.toUpper());
    QList<QString> sysxMsg;
    if (hex0.toInt(&ok, 16)<10)  //check if in system address range. < 10.00.00.00
    {
        if(this->systemSource.address.indexOf(address) == -1)
        {
            sysxWriter file;
            file.setFile(":system.syx");  // Read the default system sysex file so whe don't start empty handed.
            if(file.readFile())
            {
                setFileSource("System", file.getSystemSource());
            };
        };
        int sysIdx = this->systemSource.address.indexOf(address);
        if(sysIdx == -1) { return sysxMsg; }
        sysxMsg = this->systemSource.hex.at( sysIdx );

    }
    else
    {
        if(this->fileSource.address.indexOf(address) == -1)
        {
            sysxWriter file;
            if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
            {
                file.setFile(":/default_bass.syx");
            }
            else
            {
                file.setFile(":/default_guitar.syx");
            };  // Read the default patch sysex file so whe don't start empty handed.

            if(file.readFile())
            {
                setFileSource("Structure", file.getFileSource());
            };
        };
        int fileIdx = this->fileSource.address.indexOf(address);
        if(fileIdx == -1) { return sysxMsg; }
        sysxMsg = this->fileSource.hex.at( fileIdx );
    };

    return sysxMsg;
}

QString SysxIO::getCheckSum(int dataSize)
{
    MidiTable *midiTable = MidiTable::Instance();
    return midiTable->getCheckSum(dataSize);
}

QList<QString> SysxIO::correctSysxMsg(QList<QString> sysxMsg)
{
    /* QString address1 = sysxMsg.at(sysxAddressOffset + 2);
    QString address2 = sysxMsg.at(sysxAddressOffset + 3);

    bool ok;
    int dataSize = 0;
    for(int i=sysxMsg.size() - 1; i>=checksumOffset;i--)
    {
        dataSize += sysxMsg.at(i).toInt(&ok, 16);
    };
    sysxMsg.replace(sysxMsg.size() - 1, getCheckSum(dataSize));

    return sysxMsg;*/

    //QList<QString> before = sysxMsg;

    QString address0 = sysxMsg.at(sysxAddressOffset);
    QString address1 = sysxMsg.at(sysxAddressOffset + 1);
    QString address2 = sysxMsg.at(sysxAddressOffset + 2);

    bool ok;

    MidiTable *midiTable = MidiTable::Instance();
    QString address3 = "00";
    for(int i=sysxDataOffset;i<sysxMsg.size() - 1;i++)
    {
        //if((address1=="00" || address1=="00") && address2=="01" && address3=="0"){i++;};
        address3 = QString::number(i - sysxDataOffset, 16).toUpper();
        if(address3.length()<2) address3.prepend("0");

        //int oldval=0;
        //int newval=0;
        int range = midiTable->getRange(address0, address1, address2, address3);

        if(midiTable->isData(address0, address1, address2, address3))
        {
            /* int maxRange = QString("7F").toInt(&ok, 16) + 1; // index starts at 0 -> 0-127 = 128 entry's.
            int value1 = sysxMsg.at(i).toInt(&ok, 16);
            int value2 = sysxMsg.at(i + 1).toInt(&ok, 16);
            int value = (value1 * maxRange) + value2;

            if(value > range)
            {
                oldval=value;
                value = int(range/2);
                int dif = int(value/maxRange);
                QString valueHex1 = QString::number(dif, 16).toUpper();
                if(valueHex1.length() < 2) valueHex1.prepend("0");
                QString valueHex2 = QString::number(value - (dif * maxRange), 16).toUpper();
                if(valueHex2.length() < 2) valueHex2.prepend("0");

                sysxMsg.replace(i, valueHex1);
                sysxMsg.replace(i + 1, valueHex2);
            };

            i++;
            */
            if(sysxMsg.at(i).toInt(&ok, 16) > range)
            {
                int value = int(range/2);
                QString valueHex = QString::number(value, 16).toUpper();
                if(valueHex.length() < 2) valueHex.prepend("0");
                sysxMsg.replace(i, valueHex);
            };
        }
        else
        {
            if(sysxMsg.at(i).toInt(&ok, 16) > range)
            {
                int value = int(range/2);
                QString valueHex = QString::number(value, 16).toUpper();
                if(valueHex.length() < 2) valueHex.prepend("0");
                sysxMsg.replace(i, valueHex);
            };
        };

    };

    int dataSize = 0;
    for(int i=sysxMsg.size() - 1; i>=checksumOffset;i--)
    {
        dataSize += sysxMsg.at(i).toInt(&ok, 16);
    };
    sysxMsg.replace(sysxMsg.size() - 1, getCheckSum(dataSize));

    return sysxMsg;
}

/***************************** isConnected() ******************************
* Connection status that's globaly accesible.
****************************************************************************/
bool SysxIO::isConnected()
{
    return this->connected;
}

void SysxIO::setConnected(bool connected)
{
    this->connected = connected;
    emit setStatusProgress(0);
    if(connected)
    {
        emit setStatusSymbol(1);
        emit setStatusMessage(tr("Ready"));
    }
    else
    {
        emit setStatusSymbol(0);
        emit setStatusMessage(tr("Not connected"));
    }
}

/***************************** deviceReady() ******************************
* Midi busy or READY status that's globaly accesible. 
* To prevent multiple messages sent at once.
****************************************************************************/
bool SysxIO::deviceReady()
{
    return this->status;
}

void SysxIO::setDeviceReady(bool status)
{
    this->status = status;
}

/***************************** isDevice() **********************************
* Flag that hold if the current sysex data we are editing is from file or DEVICE.
****************************************************************************/
bool SysxIO::isDevice()
{
    return this->isdevice;
}

void SysxIO::setDevice(bool isdevice)
{
    this->isdevice = isdevice;
}

/***************************** getSyncStatus() **********************************
* Flag that hold if the sysex data we are editing is synchronized with what's 
* on the device.
****************************************************************************/
bool SysxIO::getSyncStatus()
{
    return this->syncStatus;
}

void SysxIO::setSyncStatus(bool syncStatus)
{
    this->syncStatus = syncStatus;
}

void SysxIO::setBank(int bank)
{
    this->bank = bank;
}

void SysxIO::setPatch(int patch)
{
    this->patch = patch;
}

int SysxIO::getBank(){
    return this->bank;
}

int SysxIO::getPatch(){
    return this->patch;
}

void SysxIO::setLoadedBank(int bank)
{
    this->loadedBank = bank;
}

void SysxIO::setLoadedPatch(int patch)
{
    this->loadedPatch = patch;
}

int SysxIO::getLoadedBank(){
    return this->loadedBank;
}

int SysxIO::getLoadedPatch(){
    return this->loadedPatch;
}

/*********************** getRequestName() ***********************************
* Set the name for check of the patch that we are going to load.
***************************************************************************/
void SysxIO::setRequestName(QString requestName)
{
    this->requestName = requestName;
}

/*********************** returnRequestName() ***********************************
* Return the name for check of the patch that should have been loaded.
***************************************************************************/
QString SysxIO::getRequestName()
{
    return this->requestName;
}

/*********************** getPatchChangeMsg() *****************************
* Formats the midi message for bank changing (temp buffer) and returns it.
*************************************************************************/
QString SysxIO::getPatchChangeMsg(int bank, int patch)
{
    int bankOffset = ((bank - 1) * (patchPerBank+1)) + (patch - 1);
    int bankSize = 128;
    int bankMsbNum = int(bankOffset / bankSize);
    int programChangeNum = bankOffset - (bankSize * bankMsbNum);
    QString bankMsb = QString::number(bankMsbNum, 16).toUpper();
    QString programChange = QString::number(programChangeNum, 16).toUpper();

    if (bankMsb.length() < 2) bankMsb.prepend("0");
    if (programChange.length() < 2) programChange.prepend("0");
    Preferences *preferences = Preferences::Instance();
    bool ok;
    int midiTxCh = preferences->getPreferences("Midi", "TxCh", "set").toInt(&ok, 10);
    QString TxCh = QString::number(midiTxCh-1, 16).toUpper();

    QString midiMsg ="";                     // universal bank change method
    midiMsg.append("B"+TxCh+"00"+bankMsb);
    midiMsg.append("B"+TxCh+"2000");
    midiMsg.append("C"+TxCh+programChange);
    return midiMsg;
}

/***************************** sendMidi() ***********************************
* Sends a midi message over the midiOut device sellected in the preferences.
****************************************************************************/
void SysxIO::sendMidi(QString midiMsg)
{
    Q_UNUSED(midiMsg);
    /*  if(isConnected())
    {
        Preferences *preferences = Preferences::Instance(); bool ok;
        int midiOutPort = preferences->getPreferences("Midi", "MidiOut", "device").toUInt(&ok, 10);	// Get midi out device from preferences.

        midi = new midiIO();
        midi->sendMidi(midiMsg, midiOutPort);

        if(preferences->getPreferences("Midi", "DBug", "bool")=="true")
        {
            QString dBug =("      ");
            dBug.append(midiMsg);
            emit setStatusdBugMessage(dBug);
        }
    };*/
}

/***************************** finishedSending() *************************************
* Signals that we are finished sending a midi message, so we can go one with our life.
*************************************************************************************/
void SysxIO::finishedSending()
{
    emit isFinished();
    emit setStatusSymbol(this->isConnected() ? 1 : 0);
    emit setStatusProgress(0);
    emit setStatusMessage(this->isConnected() ? tr("Ready") : tr("Not connected"));
}

/***************************** requestPatchChange() *************************
* Send a patch change request. Result will be checked with checkPatchChange.
****************************************************************************/
void SysxIO::requestPatchChange(int bank, int patch)
{
    this->bankChange = bank;
    this->patchChange = patch;

    QObject::connect(this, SIGNAL(isFinished()),	// Connect the result of the request
                     this, SLOT(namePatchChange()));				// to returnPatchName function.

    int pnum = ((bank-1)*4)+(patch-1);
    QString valueHex = QString::number(pnum, 16).toUpper();
    if(valueHex.size()<2){valueHex.prepend("000");};
    if(valueHex.size()<3){valueHex.prepend("00");};
    if(valueHex.size()<4){valueHex.prepend("0");};
    valueHex.insert(3, "0");
    valueHex.insert(2, "0");
    valueHex.insert(1, "0");
    valueHex.prepend("0");

    QString sysxMsg = "F04110000000691200";
    QString hex1 = "01";
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex1 = "08";
    };
    sysxMsg.append(hex1 + "0000");
    sysxMsg.append(valueHex);
    bool ok;
    int dataSize = 0;
    for(int i=checksumOffset; i<sysxMsg.size();i++)
    {
        dataSize += sysxMsg.mid(i*2, 2).toInt(&ok, 16);
    };
    sysxMsg.append(getCheckSum(dataSize));
    sysxMsg.append("F7");
    qWarning("requestPatchChange bank=%d patch=%d pnum=%d modeHex=%s sysx=%s",
             bank,
             patch,
             pnum,
             hex1.toUtf8().constData(),
             sysxMsg.toUtf8().constData());
    setDeviceReady(false);
    QObject::connect(this, SIGNAL(sysxReply(QString)), this, SLOT(resetDevice(QString)));
    this->sysxOutput(sysxMsg, 0);
    //this->setMode("mode2");
}

/***************************** namePatchChange() *************************
* Get the name of the patch we are switching to and check it with the 
* one requested.
****************************************************************************/
void SysxIO::namePatchChange()
{
    QObject::disconnect(this, SIGNAL(isFinished()),
                        this, SLOT(namePatchChange()));
    QObject::disconnect(SIGNAL(patchName(QString)));

    QObject::connect(this, SIGNAL(patchName(QString)),
                     this, SLOT(checkPatchChange(QString)));
}

/***************************** checkPatchChange() *************************namePatchChange()
* Emits a signal if the patch change was confirmed else it will retry until
* the maximum retry's has been reached.
****************************************************************************/
void SysxIO::checkPatchChange(QString name)
{
    Q_UNUSED(name);
    QObject::disconnect(this, SIGNAL(patchName(QString)),
                        this, SLOT(checkPatchChange(QString)));

    emit isChanged();
    this->changeCount = 0;
}

/***************************** sendSysx() ***********************************
* Sends a sysex message over the midiOut device sellected in the preferences.
*****************************************************************************/
void SysxIO::sendSysx(QString sysxMsg, int replySize)
{
    Preferences *preferences = Preferences::Instance(); // Load the preferences.
    if(preferences->getPreferences("Midi", "DBug", "bool")=="true")
    {
        emit setStatusdBugMessage(sysxMsg);
    };

    if(!midi)
    {
        this->deBug("new midiIO instance generated.... ********" );
        midi = new midiIO();
    };

    if(sysxMsg!=_lastSyx || sysxMsg.contains(idRequestString) || sysxMsg.contains("F0411000000069127F"))
    {
        //this->spoolTimer->start(replySize);
        midi->sendSysxMsg(sysxMsg, replySize, 0, 0);
        _lastSyx = sysxMsg;
    }
    else
    {
        //qDebug() << "sendSyx() repeated message" << sysxMsg;
        receiveSysx("");
    };

}

/***************************** receiveSysx() *******************************
* Receives possible replied sysex message on sendSysex.
****************************************************************************/
void SysxIO::receiveSysx(QString sysxMsg)
{
    Preferences *preferences = Preferences::Instance(); // Load the preferences.
    if(preferences->getPreferences("Midi", "DBug", "bool")=="true"){ emit setStatusdBugMessage(sysxMsg);};

    emit sysxReply(sysxMsg);
}

/***************************** requestBulkPatchNames() ***************************
* Send a patch name request. Result will be send with the returnBulkPatchNames
* function.
****************************************************************************/
void SysxIO::requestBulkPatchNames()
{
    /* Bulk Patch names request. */
    Preferences *preferences = Preferences::Instance();
    QString sysxMsg = bulkPatchNames;
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        sysxMsg = bulkPatchNames_Bass;
    };

    QObject::disconnect(this, SIGNAL(sysxReply(QString)),
                        this, SLOT(returnBulkPatchNames(QString)));

    QObject::connect(this, SIGNAL(sysxReply(QString)),	// Connect the result of the request
                     this, SLOT(returnBulkPatchNames(QString)));			// to returnPatchName function.



    if(start_sequence<4)
    {
        if(preferences->getPreferences("Midi", "DBug", "bool")=="true")
        {
            deBug("REQUEST BULK PATCH NAMES x" + QString::number(start_sequence, 10) + " sendSysx(sysxMsg, bulkPatchNamesSize);");
        };
        this->sysxOutput(sysxMsg, bulkPatchNamesSize);
    };
}

/***************************** returnBulkPatchNames() ***************************
* Emits a signal with the retrieved patch names list.
****************************************************************************/
void SysxIO::returnBulkPatchNames(QString sysxMsg)
{
    // **********************************************************************************************
    // ** detect if first time through loop and check the current running mode vs the SY-1000 mode **
    // ** If the Mode is not matching and preferences set to Auto Mode, then save the preferences  **
    // ** mode to match and restart in the opposite mode to match the SY-1000.                     **
    // ** If mode do match, increment the loop counter and continue with reading the patch names.  **
    // **********************************************************************************************
    Preferences *preferences = Preferences::Instance();
    const int replyBytes = sysxMsg.size()/2;
    // Accept shorter-but-valid bulk name replies (observed on some SY-1000 setups).
    // 200 names * 16 chars = 3200 bytes raw payload; allow anything around that range.
    const int minBulkPatchNamesReplySize = 3000;
    if(preferences->getPreferences("Midi", "DBug", "bool")=="true")
    {
        deBug("Bulk names reply size = " + QString::number(replyBytes, 10) + "  sequence = " + QString::number(start_sequence, 10));
    };
    QString unitID = sysxMsg.mid(4, 2);

    QObject::disconnect(this, SIGNAL(sysxReply(QString)),
                        this, SLOT(returnBulkPatchNames(QString)));

    if(start_sequence==0 && replyBytes<minBulkPatchNamesReplySize)
    {
        QString current_mode = "guitar";
        if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
        {
            current_mode = "bass";
        };
        QString required_mode = "guitar";

        if(sysxMsg.isEmpty() && current_mode=="guitar")
        {
            required_mode="bass";
        }
        else if(sysxMsg.isEmpty() && current_mode=="bass")
        {
            required_mode="guitar";
        };

        deBug("Bulk names not received, current mode=" + current_mode);

        const bool autoModeEnabled = (preferences->getPreferences("Window", "AutoMode", "bool")=="true");
        const bool autoModeRetryUsed = qEnvironmentVariableIsSet("SY1000_AUTOMODE_RESTART");
        if(autoModeEnabled && (required_mode != current_mode) && !autoModeRetryUsed /*&& (preferences->getPreferences("Midi", "DBug", "bool")!="true")*/)
        {
            qWarning("AUTOMODE_RESTART request current=%s required=%s connected=%d replyBytes=%d sequence=%d",
                     current_mode.toUtf8().constData(),
                     required_mode.toUtf8().constData(),
                     this->isConnected() ? 1 : 0,
                     replyBytes,
                     start_sequence);
            if(required_mode=="guitar")
            {
                preferences->setPreferences("Window", "BassMode", "bool", "false");
            }
            else
            {
                preferences->setPreferences("Window", "BassMode", "bool", "true");
            };
            preferences->savePreferences();  // check if the editor mode and SY-1000 modes match, if editor mode not correct, change mode setting and restart.
            midi = new midiIO();
            midi->closePorts();
            if(this->isConnected())
            {
                // Allow only one automatic restart in a launch chain so missing MIDI replies do not loop forever.
                qputenv("SY1000_RESTART_REASON", "automode");
                qputenv("SY1000_AUTOMODE_RESTART", "1");
                emit restart();  // restart is in mainWindow()
            };
            if(!this->isConnected())
            {
                qWarning("AUTOMODE_RESTART skipped: not connected. No restart performed.");
                start_sequence++;
                requestBulkPatchNames();
            }
        }
        else if(autoModeEnabled && (required_mode != current_mode) && autoModeRetryUsed)
        {
            qCritical("AUTOMODE_RESTART blocked: loop prevention active current=%s required=%s replyBytes=%d sequence=%d",
                      current_mode.toUtf8().constData(),
                      required_mode.toUtf8().constData(),
                      replyBytes,
                      start_sequence);
            deBug("Auto mode mismatch persists after one restart; suppressing further auto restarts.");
            emit errorSignal(tr("Auto Mode Restart Blocked"),
                             tr("SY-1000 did not reply after one auto-mode restart. Restart loop blocked; check MIDI In/Out and device mode."));
            emit errorEvent();
            start_sequence++;
            requestBulkPatchNames();
        }
        else
        {
            start_sequence++;
            requestBulkPatchNames();
        };
    }
    else
    {
        QString name;

        if(replyBytes >= minBulkPatchNamesReplySize)
        {
            if(replyBytes >= bulkPatchNamesSize)
            {
                sysxMsg.truncate(bulkPatchNamesSize*2);
            }

            const QString bulkHeader = "F041"+unitID+"0000006912";
            int headerPos = sysxMsg.indexOf(bulkHeader);
            if(headerPos >= 0)
            {
                sysxMsg.remove(headerPos, 24);
            }
            for(int i=0; i<16; i++)
            {
                headerPos = sysxMsg.indexOf(bulkHeader);
                if(headerPos < 0)
                {
                    break;
                }
                const int removePos = qMax(0, headerPos-4);
                sysxMsg.remove(removePos, 28);
            }
            if(sysxMsg.size() >= 4)
            {
                sysxMsg.remove(sysxMsg.size()-4, 4);
            }


            for(int i=0;i<(sysxMsg.size());i++ )
            {
                QString hexStr = sysxMsg.at(i);
                hexStr.append(sysxMsg.at(i+1));
                bool ok;
                name.append(char(hexStr.toInt(&ok, 16)));
                i=i+1;
            };
            this->namesList = name;
        }
        else
        {
            Preferences *preferences = Preferences::Instance();// Load the preferences.
            if(preferences->getPreferences("Midi", "DBug", "bool")=="true")
            {
                deBug(QString("last try - bulknames size incorrect " + QString::number(replyBytes, 10) + " should be >= " + QString::number(minBulkPatchNamesReplySize, 10)));
            };
            for(int x=0; x<200; x++)
            {
                name.append("data not loaded ");
            };
        };
        emit patchName(name);
    };
}

/***************************** requestPatch() ******************************
* Send a patch request. Result will be send directly with receiveSysx signal
****************************************************************************/
void SysxIO::requestPatch(int bank, int patch)
{
    Preferences *preferences = Preferences::Instance();
    /* Patch request. */
    MidiTable *midiTable = MidiTable::Instance();
    QString sysxMsg = midiTable->patchRequest(bank, patch);
    int replySize = patchReplySize;
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        replySize = patchReplySizeBass;
    };

    //setMode("mode1");
    deBug("SENDING PATCH REQUEST");
    this->sysxOutput(sysxMsg, replySize);
}

/**************************** setMode(QString mode) ************************
 * set the operating mode of the SY-1000
 ***************************************************************************/
void SysxIO::setMode(QString mode)
{
    sendSpooler.clear();
    replySize.clear();
    deBug("SET MODE = " + mode);
    emit setStatusSymbol(2);
    emit setStatusMessage(tr("Sending"));

    QObject::disconnect(this, SIGNAL(sysxReply(QString)), this, SLOT(setModeReply(QString)));
    QObject::connect(this, SIGNAL(sysxReply(QString)), this, SLOT(setModeReply(QString)));
    this->setDeviceReady(false);

    QString pnumString = "F04110000000691100010000000000040AF7";
    Preferences *preferences = Preferences::Instance(); // Load the preferences.
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){pnumString = "F04110000000691100080000000000040AF7";};

    if(mode.contains("mode0"))       {this->sysxOutput("F0411000000069127F0000010000F7", 30); this->mode=0;} // editor mode off. stops sysx data updates.
    else if(mode.contains("mode1"))  {this->sysxOutput("F0411000000069127F000001017FF7", 30); this->mode=1;} // editor mode on. starts syx data updates (verbose mode).
    else if(mode.contains("mode2"))  {this->sysxOutput("F0411000000069127F000002007FF7", 30); this->mode=2;} //runing mode PLAY: 0x00,
    else if(mode.contains("mode3"))  {this->sysxOutput("F0411000000069127F000002017EF7", 0); this->mode=3;} //runing mode POLY_TUNER: 0x01,
    else if(mode.contains("mode4"))  {this->sysxOutput("F0411000000069127F000002027DF7", 0); this->mode=4;} //runing mode MONO_TUNER: 0x02,
    else if(mode.contains("mode5"))  {this->sysxOutput("F0411000000069127F000002037CF7", 0); this->mode=5;} //runing mode GK_SENS: 0x03,
    else if(mode.contains("mode6"))  {this->sysxOutput("F0411000000069127F000002047BF7", 0); this->mode=6;} //runing mode NORMAL_SENS: 0x04
    else if(mode.contains("getPNum")){this->sysxOutput(pnumString, 0);}  // request current patch number.
    else                             {this->sysxOutput("F0411000000069127F0000010000F7", 0); this->mode=0;}; // stops sysx data updates.

    //this->sysxOutput("F0411000000069117F000001000000027EF7", 16);
}

void SysxIO::setModeReply(QString mode)
{
    deBug("SET MODE REPLY = " + mode);
    QObject::disconnect(this, SIGNAL(sysxReply(QString)), this, SLOT(setModeReply(QString)));
    this->setDeviceReady(true);
    emit setStatusSymbol(1);
}

/***************************** errorSignal() ******************************
* Displays all midi related error messages.
****************************************************************************/
void SysxIO::errorSignal(QString errorType, QString errorMsg)
{
    setNoError(false);
    emit setStatusdBugMessage(errorType + "  " + errorMsg);
    this->errorType = "";
    this->errorMsg = "";
}

void SysxIO::errorReturn(QString errorType, QString errorMsg)
{
    setNoError(false);
    emit setStatusdBugMessage(errorType + "  " + errorMsg);
    this->errorType = "";
    this->errorMsg = "";
}

/***************************** noError() ******************************
* Error flag set on midi error to prevent (double) connexion faillure 
* messages and a midi messages.
************************************************************************/
bool SysxIO::noError()
{
    return this->noerror;
}

void SysxIO::setNoError(bool status)
{
    this->noerror = status;
}

/***************************** CurrentPatchName() ******************************
* This is to make it possible to verify the patch name that we are trying to receive 
* corresponds in case we had a name change that was not yet written permanantly
* to the device. (See floorBoardDisplay.cpp for more info)
************************************************************************/
void SysxIO::setCurrentPatchName(QString patchName)
{
    this->currentName = patchName;
}

QString SysxIO::getCurrentPatchName()
{
    return this->currentName;
}

/***************************** emit() *********************************
* Added to make status update possible from static methods and classes 
* like the CALLBACK in midiIO.
************************************************************************/
void SysxIO::emitStatusSymbol(int value)
{
    emit setStatusSymbol(value);
}

void SysxIO::emitStatusProgress(int value)
{
    emit setStatusProgress(value);
}

void SysxIO::emitStatusMessage(QString message)
{
    emit setStatusMessage(message);
}
void SysxIO::emitStatusdBugMessage(QString dBug)
{
    emit setStatusdBugMessage(dBug);
}

void SysxIO::systemWrite()
{
    setDeviceReady(false);			// Reserve the device for interaction.

    QString sysxMsg;
    QList< QList<QString> > systemData = getSystemSource().hex; // Get the loaded system data.
    for(int i=0;i<systemData.size();++i)
    {
        QList<QString> data = systemData.at(i);
        for(int x=0;x<data.size();++x)
        {
            QString hex;
            hex = data.at(x);
            if (hex.length() < 2) hex.prepend("0");
            sysxMsg.append(hex);
        };
    };
    if(this->isConnected() && this->deviceReady())
    {
        this->setDeviceReady(false);

        emit setStatusSymbol(2);
        emit setStatusMessage(tr("Sending"));

        QObject::connect(this, SIGNAL(sysxReply(QString)),
                         this, SLOT(resetDevice(QString)));

        this->sysxOutput(sysxMsg, 0);
    }
    else if(this->isConnected())
    {
        QObject::connect(this, SIGNAL(sysxReply(QString)),
                         this, SLOT(resetDevice(QString)));
        this->sysxOutput(sysxMsg, 0);
    };
    setDeviceReady(true);
}

void SysxIO::systemDataRequest()
{
    QObject::disconnect(this, SIGNAL(sysxReply(QString)));
    emit setStatusSymbol(2);
    emit setStatusMessage(tr("Request System data"));
    setDeviceReady(false); // Reserve the device for interaction.

    QObject::connect(this, SIGNAL(sysxReply(QString)), this, SLOT(systemReply(QString)));

    this->sysxOutput(systemRequest, systemReplySize);  // SY-1000 System area data Request.
}

void SysxIO::systemReply(QString replyMsg)
{
    QObject::disconnect(this, SIGNAL(sysxReply(QString)), this, SLOT(systemReply(QString)));
    setDeviceReady(true); // Free the device after finishing interaction.

    if(replyMsg.size()/2 == systemReplySize)  // data format from the SY-1000
    {
        QList< QList<QString> > systemData = this->getSystemSource().hex; // Get the loaded patch data.
        QString systemHex;
        for(int i=0;i<systemData.size();++i)
        {
            QList<QString> data = systemData.at(i);
            for(int x=0;x<data.size();++x)
            {
                QString hex = data.at(x);
                if (hex.length() < 2) hex.prepend("0");
                systemHex.append(hex);
            };
        };
        systemHex.replace(0, replyMsg.size(), replyMsg);
        setFileSource("System", systemHex);		// Set the source to the data received.
        setDevice(true);				// Patch received from the device so this is set to true.
        setSyncStatus(true);			// We can't be more in sync than right now! :)
        deBug("SYSTEM MIDI DATA LOADED SUCESSFULLY !!");
        if(!patch_loaded)
        {
            QObject::disconnect(this, SIGNAL(sysxReply(QString)), this, SLOT(updatePatch(QString)));
            QObject::connect(this, SIGNAL(sysxReply(QString)), this, SLOT(updatePatch(QString)));

            this->requestPatch(0,0);
        }
        else
        {
            relayUpdateSignal();
        };
    }
    else if(replyMsg.size()>0)
    {
        QMessageBox *msgBox = new QMessageBox();
        msgBox->setWindowTitle(deviceType + tr(" FloorBoard connection Error !!"));
        msgBox->setIcon(QMessageBox::Warning);
        msgBox->setTextFormat(Qt::RichText);
        QString msgText;
        msgText.append("<font size='+1'><b>");
        msgText.append(tr("The Boss ") + deviceType + tr(" Guitar Synthesizer was not found."));
        msgText.append("<b></font><br>");
        msgText.append(tr("any saved system data will be as shown on screen<br>"));
        msgText.append(tr("and the SY-1000 system data might not be in sync<br>"));
        msgBox->setText(msgText);
        msgBox->show();
        QTimer::singleShot(3000, msgBox, SLOT(deleteLater()));
    }
    else
    {
        deBug("SYSTEM MIDI DATA FAILED TO LOAD");
    };

    emit setStatusSymbol(1);
    emit setStatusProgress(0);
    emit setStatusMessage(tr("Ready"));

}

void SysxIO::writeToBuffer()
{
    QObject::disconnect(this, SIGNAL(isChanged()),
                        this, SLOT(writeToBuffer()));

    QString sysxMsg;
    QList< QList<QString> > patchData = getFileSource().hex; // Get the loaded patch data.
    emit setStatusSymbol(2);
    emit setStatusMessage(tr("Sync to ")+deviceType);
    QString addr1 = tempDataWrite;
    QString addr2 = "00";
    QString addr2A = "01";
    int range = 5236*2; // point in sys file when address range increases.
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){addr2 = "02"; addr2A = "03"; range = 5173*2;};

    for(int i=0;i<patchData.size();++i)
    {
        QList<QString> data = patchData.at(i);
        for(int x=0;x<data.size();++x)
        {
            QString hex;
            if(x == sysxAddressOffset)
            {
                hex = addr1;
            }
            else if(x == sysxAddressOffset + 1)
            {
                hex = addr2;
            }
            else
            {
                hex = data.at(x);
            };
            if (hex.length() < 2) hex.prepend("0");
            sysxMsg.append(hex);
            if(sysxMsg.size()>range)
            {
                addr2=addr2A;
            };
        };
    };
    setSyncStatus(true);		// In advance of the actual data transfer we set it already to sync.

    QObject::connect(this, SIGNAL(sysxReply(QString)),	// Connect the result signal
                     this, SLOT(resetDevice(QString)));					// to a slot that will reset the device after sending.
    this->sysxOutput(sysxMsg, 0);	// Send the data.
}

void SysxIO::relayUpdateSignal()
{
    tmo = 0;
    emit updateSignal();
}

void SysxIO::errorEvent()
{
    this->setConnected(false);
    this->setSyncStatus(false);
    this->setDeviceReady(true);	// Free the device after finishing interaction.

    emit setStatusSymbol(0);
    emit setStatusProgress(0);
    emit setStatusMessage(tr("Not connected"));

    midi = new midiIO();
    midi->closePorts();
}

void SysxIO::receiveEvent(QString rxData)
{
    //if(rxData.contains("F07E100602416903000000000000F7")){setConnected(true);};
    QString unitID = rxData.mid(4,2);
    if(rxData.mid(0,24).contains("F041"+unitID+"00000069127F000500")){emit meterSignal(rxData.mid(0, 140)); rxData.clear();}; //Normal input VU meter levels.
    if(rxData.mid(0,24).contains("F041"+unitID+"00000069127F000400")){emit meterSignal(rxData.mid(0, 38)); rxData.clear();};  //GK input VU meter levels.
    if(rxData.mid(0,24).contains("F041"+unitID+"00000069127F000300")){emit meterSignal(rxData.mid(0, 112)); rxData.clear();}; //Mono tuner VU meter levels.
    if(rxData.mid(0,26).contains("F041"+unitID+"00000069127F00000100")){this->mode=0; qDebug() << "received mode 0 off"; rxData.clear();};
    if(rxData.mid(0,26).contains("F041"+unitID+"00000069127F00000101")){this->mode=1; qDebug() << "received mode 1 on"; rxData.clear();};
    if(rxData.mid(0,24).contains("F041"+unitID+"00000069127F000002")){this->mode=1; qDebug() << "received mode 2 on"; rxData.clear();};
    if(rxData.mid(0,24).contains("F041"+unitID+"00000069127F000100"))
    {
        bool ok;
        int msb = rxData.mid(25, 1).toInt(&ok, 16)*128;
        int lsb = rxData.mid(26, 1).toInt(&ok, 16)*16;
        int ksb = rxData.mid(27, 1).toInt(&ok, 16);
        int index = msb+lsb+ksb;
        setBank((index/4)+1);
        setPatch((index-((index/4)*4))+1);
        deBug("receiveEvent COMMAND PATCH CHANGE Bank=" + QString::number((index/4)+1, 10) + " Patch=" + QString::number((index-((index/4)*4))+1, 10));
        emit updateTreeSignal();

        if(/*patch_loaded == true && */last_index != index)
        {
            last_index = index;
            setDeviceReady(false);
            changePatch();
            //this->setMode("mode2");
            //this->requestPatch(0, 0);
            emit isChanged(); // notify bankTreeList (and any other listeners) that the
                              // SY-1000 has confirmed the new patch is loaded and ready.
        };
        rxData.clear();
    };
    if(rxData.mid(0,20).contains("F041"+unitID+"00000069127F00")){qDebug() << "UNKNOWN COMMAND RECEIVED " + rxData; rxData.clear();};
    bool ok;

    if((rxData.contains("F041"+unitID+"000000691210") || rxData.contains("F041"+unitID+"000000691200")) && rxData != _lastSyx)
    {
        setFileSource("Structure", rxData);
        if(tmo == 0)
        {
            tmo = 1;
            rxTimer->singleShot(80, this, SLOT(relayUpdateSignal()));
        };
    };

    int addr = rxData.mid(16, 2).toInt(&ok,16);

    if(addr>=32 && addr<=80)
    {
        Preferences *preferences = Preferences::Instance(); // Load the preferences.
        int replySize = patchReplySize;
        if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
        {
            replySize = patchReplySizeBass;
        };

        _data.append(rxData);

        if(_data.size()/2>=replySize)  // receive complete accumulated patch.
        {
            // updatePatch(_data);
            _data.clear();
            // relayUpdateSignal();
        };
    };

    if(rxData.contains("F041"+unitID+"000000691200010000") || rxData.contains("F041"+unitID+"000000691200080000"))  // received current patch number F041100000006912000100000001080F67F7
    {            // read patch change data from VG-99 and set editor patch/bank.
        bool ok;
        int msb = rxData.mid(27, 1).toInt(&ok, 16)*256;
        int lsb = rxData.mid(29, 1).toInt(&ok, 16)*16;
        int ksb = rxData.mid(31, 1).toInt(&ok, 16);
        int index = msb+lsb+ksb;
        setBank((index/4)+1);
        setPatch((index-((index/4)*4))+1);
        emit updateTreeSignal();

        if(patch_loaded == true && last_index != index && count>0)
        {
            last_index = index;
            setDeviceReady(false);
            changePatch();
        };
        count++;
    };
}

void SysxIO::changePatch()
{
    QObject::disconnect(this, SIGNAL(sysxReply(QString)), this, SLOT(updatePatch(QString)));
    QObject::connect(this, SIGNAL(sysxReply(QString)), this, SLOT(updatePatch(QString)));
    deBug("changePatch " + QString::number(bank, 10) + " " + QString::number(patch, 10));
    requestPatch(bank, patch);
}

void SysxIO::updatePatch(QString replyMsg)
{
    QString unitID = replyMsg.mid(4,2);
    QObject::disconnect(this, SIGNAL(sysxReply(QString)), this, SLOT(updatePatch(QString)));
    QString rebuild;
    bool correct = false;
    if(replyMsg.mid(replyMsg.size()-2, 2)=="F7")
    {
        correct=true;
    };
    if((replyMsg.size()/2==patchReplySize || replyMsg.size()/2==patchReplySizeBass) && correct==true)
    {

        if(replyMsg.size()/2==patchReplySize)
        {
            rebuild.clear();
            QString zeros("000000000000000000000000000000000000000000");
            rebuild.append(replyMsg.mid(0, 140*2));
            rebuild.append(replyMsg.mid(167*2, 12*2)+"0100"); // footer & header
            rebuild.append(replyMsg.mid(140*2, 141*2));   //10 02 ~ 10 04
            rebuild.append(zeros);
            rebuild.append(replyMsg.mid(295*2, 100*2));   //10 04 ~ 10 05
            rebuild.append(zeros);
            rebuild.append(replyMsg.mid(409*2, 100*2));   //10 05 ~ 10 06
            rebuild.append(zeros);
            rebuild.append(replyMsg.mid(523*2, 100*2));   //10 06 ~ 10 07
            rebuild.append(zeros);
            rebuild.append(replyMsg.mid(637*2, 100*2));   //10 07 ~ 10 08
            rebuild.append(zeros);
            rebuild.append(replyMsg.mid(751*2, 100*2));   //10 08 ~ 10 09
            rebuild.append(zeros);
            rebuild.append(replyMsg.mid(865*2, 100*2));   //10 09 ~ 10 0A
            rebuild.append(zeros);
            rebuild.append(replyMsg.mid(979*2, 100*2));   //10 0A ~ 10 0B
            rebuild.append(zeros);
            rebuild.append(replyMsg.mid(1093*2, 355*2));   //10 0B ~ 10 16

            rebuild.append(replyMsg.mid(167*2, 12*2)+"1700"); // footer & header
            rebuild.append(replyMsg.mid(1448*2, 692*2)); // 10 17 ~ 10 21  //648
            rebuild.append(replyMsg.mid(167*2, 12*2)+"2200"); // footer & header
            rebuild.append(replyMsg.mid(2140*2, 692*2)); // 10 22 ~ 10 2C
            rebuild.append(replyMsg.mid(167*2, 12*2)+"2D00"); // footer & header
            rebuild.append(replyMsg.mid(2832*2, 3458*2));  // to end..

            int nextIndex=0;
            QString addr1="00";
            for(int x=0; x<172; x++)
            {
                rebuild.replace(rebuild.indexOf("F041"+unitID+"0000006912", nextIndex)+16, 4, "10"+addr1);
                nextIndex=rebuild.indexOf("F041"+unitID+"0000006912", nextIndex+1);
                if(x>119){addr1="01";};
            };
        };

        if(replyMsg.size()/2==patchReplySizeBass)
        {
            rebuild.clear();
            QString zeros("000000000000000000000000000000000000000000");
            rebuild.append(replyMsg.mid(0, 140*2));
            rebuild.append(replyMsg.mid(167*2, 12*2)+"0100"); // footer & header
            rebuild.append(replyMsg.mid(140*2, 141*2));   //10 02 ~ 10 04
            rebuild.append(zeros);
            rebuild.append(replyMsg.mid(295*2, 100*2));   //10 04 ~ 10 05
            rebuild.append(zeros);
            rebuild.append(replyMsg.mid(409*2, 100*2));   //10 05 ~ 10 06
            rebuild.append(zeros);
            rebuild.append(replyMsg.mid(523*2, 100*2));   //10 06 ~ 10 07
            rebuild.append(zeros);
            rebuild.append(replyMsg.mid(637*2, 100*2));   //10 07 ~ 10 08
            rebuild.append(zeros);
            rebuild.append(replyMsg.mid(751*2, 100*2));   //10 08 ~ 10 09
            rebuild.append(zeros);
            rebuild.append(replyMsg.mid(865*2, 100*2));   //10 09 ~ 10 0A
            rebuild.append(zeros);
            rebuild.append(replyMsg.mid(979*2, 100*2));   //10 0A ~ 10 0B
            rebuild.append(zeros);
            rebuild.append(replyMsg.mid(1093*2, 355*2));   //10 0B ~ 10 16

            rebuild.append(replyMsg.mid(167*2, 12*2)+"1700"); // footer & header
            rebuild.append(replyMsg.mid(1448*2, 648*2)); // 10 17 ~ 10 20
            rebuild.append(replyMsg.mid(167*2, 12*2)+"2100"); // footer & header
            rebuild.append(replyMsg.mid(2096*2, 648*2)); // 10 22 ~ 10 2A
            rebuild.append(replyMsg.mid(167*2, 12*2)+"2B00"); // footer & header
            rebuild.append(replyMsg.mid(2744*2, 3414*2));  // to end..

            int nextIndex=0;
            QString addr1="02";
            for(int x=0; x<169; x++)
            {
                rebuild.replace(rebuild.indexOf("F041"+unitID+"0000006912", nextIndex)+16, 4, "10"+addr1);
                nextIndex=rebuild.indexOf("F041"+unitID+"0000006912", nextIndex+1);
                if(x>119){addr1="03";};
            };
        };

        setFileSource("Structure", rebuild);		// Set the source to the data received.
        setFileName(tr("Patch from ") + deviceType);	// Set the file name to SY-1000 patch for the display.
        setDevice(true);				// Patch received from the device so this is set to true.
        setSyncStatus(true);			// We can't be more in sync than right now! :)
        this->setDeviceReady(true);	// Free the device after finishing interaction.
        //setLoadedBank(getBank());
        //setLoadedPatch(getPatch());

        relayUpdateSignal();
        emit setStatusSymbol(1);
        emit setStatusProgress(0);
        if(this->mode==0)
        {
            this->setMode("mode1");
        }
        else if(this->mode>=1)
        {
            this->setMode("mode2");
        };
        if(patch_loaded==false)
        {
            patch_loaded = true;
            this->setMode("getPNum");
        };

        patch_loaded = true;
    }
    else
    {
        deBug("*Warning: sysxIO::updatePatch - Received patch data size in-correct !!! - size=" + QString::number(replyMsg.size()/2, 10) );
        this->setMode("mode1");
        this->setMode("mode2");
        this->setMode("getPNum");
    };
}

void SysxIO::progress(QString message)
{
    progressMessage = message;
    emit splashProgress(message);
}

void SysxIO::deBug(QString text)
{
    Preferences *preferences = Preferences::Instance();// Load the preferences.
    if(preferences->getPreferences("Midi", "DBug", "bool")=="true")
    {
        QTime time = QTime::currentTime();
        qDebug() << "time=("+time.toString("ss")+"."+time.toString("zzz")+")" << "Mode="+QString::number(mode,10) << text;

        QString data = QString("time=("+time.toString("ss")+"."+time.toString("zzz")+") ");
        data.append(text);
        QByteArray deBug_Array;
        QByteArray syx_data;
        syx_data.append(data.toLatin1());
        syx_data.append(char(13));  // carriage return

        QString logDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        if(logDir.isEmpty())
        {
            logDir = QDir::homePath() + "/Library/Application Support/Gumtown/SY-1000FloorBoard";
        }
        QDir().mkpath(logDir);
        QFile file(logDir + "/deBug_data_for_gumtown.txt"); // Keep debug log in writable app data.
        if (file.open(QIODevice::ReadWrite))
        {
            deBug_Array = file.readAll();
            file.seek(file.size());
        };

        if(text.size()>0)
        {
            if(deBug_Array.size()>2500000)
            {
                file.resize(0);
            };
            file.write(syx_data);
        };
        file.close();
    };
}
