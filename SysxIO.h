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

#ifndef SYSXIO_H
#define SYSXIO_H

#include <QObject>
#include <QString>
#include <QList>
#include <qdom.h>
#include <QTimer>
#include "midiIO.h"

class SysxIODestroyer;

struct SysxData
{
    QList<QString> address;
    QList< QList<QString> > hex;
};

class SysxIO: public QObject
{
    Q_OBJECT

public:
    static SysxIO* Instance(); //Singleton patern design
    void setFileSource(QString area, SysxData fileSource);
    void setFileSource(QString area, QByteArray data);
    void setFileSource(QString area, QString data);
    void setFileSource(QString area, QString hex1, QString hex2, QString hex3, QString hex4, bool send = true);
    void setFileSource(QString area, QString hex1, QString hex2, QString hex3, QString hex4, QString hex5, bool send = true);
    void setFileSource(QString area, QString hex1, QString hex2, QString hex3, QList<QString> hexData, bool send = true);
    void setFileName(QString fileName);
    QList<QString> getSourceItems(QString hex0, QString hex1, QString hex2, QString hex3);
    int getSourceValue(QString area, QString hex1, QString hex2, QString hex3);
    QString getFileName();
    SysxData getFileSource();
    SysxData getSystemSource();
    QList<QString> getFileSource(QString hex0, QString hex1, QString hex2, QString hex3);
    QString getCheckSum(int dataSize);
    QList<QString> correctSysxMsg(QList<QString> sysxMsg);
    void setConnected(bool connected);
    bool isConnected();
    void setDeviceReady(bool status);
    bool deviceReady();
    void setDevice(bool isdevice);
    bool isDevice();
    void setSyncStatus(bool syncStatus);
    bool getSyncStatus();
    void setBank(int bank);
    void setPatch(int patch);
    int getBank();
    int getPatch();
    void setLoadedBank(int bank);
    void setLoadedPatch(int patch);
    int getLoadedBank();
    int getLoadedPatch();
    void setRequestName(QString requestName);
    QString getRequestName();
    QString getPatchChangeMsg(int bank, int patch);
    void setCurrentPatchName(QString patchName);
    QString getCurrentPatchName();
    void sendMidi(QString midiMsg);
    void requestPatch(int bank, int patch);
    void requestPatchChange(int bank, int patch);
    bool noError();
    void setNoError(bool status);
    void emitStatusSymbol(int value);
    void emitStatusProgress(int value);
    void emitStatusMessage(QString message);
    void emitStatusdBugMessage(QString dBug);
    void errorReturn(QString errorType, QString errorMsg);
    QString dBug;
    void systemWrite();
    void systemDataRequest();
    void sysxOutput(QString sysxMsg, int size);
    int patchListValue;
    QString temp1_sysxMsg;
    QString temp2_sysxMsg;
    QString temp3_sysxMsg;
    QString temp4_sysxMsg;
    QString temp5_sysxMsg;
    QString bulk;
    QList<QString> sendSpooler;
    QList<int> replySize;
    QByteArray memo1Array;
    QByteArray memo2Array;
    QByteArray memo3Array;
    QByteArray memo4Array;
    //QList<QString> getMPTitems(QString hex1, QString hex2, QString hex3);
    //void MPTDataRequest(QString hex1, QString hex2, QString hex3);
    QString progressMessage;
    SysxData fileSource;
    SysxData systemSource;
    QString namesList;
    int mode;
    bool patch_loaded;
    int last_index;
    int count;
    int start_sequence;
    void deBug(QString text);
    midiIO* midi;

public slots:
    void errorSignal(QString errorType, QString errorMsg);
    void writeToBuffer();
    void relayUpdateSignal();
    void progress(QString message);
    void sendSysx(QString sysxMsg, int replySize);
    void errorEvent();
    void receiveEvent(QString rxData);
    void setMode(QString mode);
    void updatePatch(QString replyMsg);
    void requestBulkPatchNames();
    void processSpooler();

private slots:
    void returnBulkPatchNames(QString sysxMsg);
    void receiveSysx(QString sysxMsg);
    void finishedSending();
    void namePatchChange();
    void resetDevice(QString sysxMsg);
    void checkPatchChange(QString name);
    void systemReply(QString replyMsg);
    void setModeReply(QString mode);
    //void MPTReply(QString replyMsg);
    void changePatch();

signals:
    void sysxReply(QString sysxMsg);
    void patchName(QString name);
    void isFinished();
    void isChanged();
    void updateSignal();
    void setStatusSymbol(int value);
    void setStatusProgress(int value);
    void setStatusMessage(QString message);
    void setStatusdBugMessage(QString dBug);
    void patchChangeFailed();
    void notConnectedSignal();
    void splashProgress(QString message);
    void updateTreeSignal();
    void updateNameSignal(QString, int, int);
    void restart();
    void meterSignal(QString data);
    void request_names();

protected :
    SysxIO();
    friend class SysxIODestroyer;
    virtual ~SysxIO() { }


private:
    void connectResetDeviceSlot();
    void queueOrSendDeviceWrite(const QString &sysxMsg, int replySize);
    bool flushDeferredDeviceWrite();
    static SysxIO* _instance;
    static SysxIODestroyer _destroyer;
    QString fileName;
    bool connected;
    bool status;
    int bank;
    int patch;
    int loadedBank;
    int loadedPatch;
    bool isdevice;
    bool syncStatus;
    bool noerror;
    int bankChange;
    int patchChange;
    int changeCount;
    QString requestName;
    QString currentName;
    QString errorType;
    QString errorMsg;
    QString tempMsg;
    int namesAddr = 0;
    QTimer* rxTimer;
    int tmo;
    QString _lastSyx;
    QString _data;
    QTimer* spoolTimer;
    QList<QString> deferredWriteSpooler;
    QList<int> deferredReplySizes;
};

#endif // SYSXIO_H
