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

#ifndef MIDIIO_H
#define MIDIIO_H
#include <QThread>
#include <QString>
#include <QStringList>
#include <QList>
#include <QMutex>
#include <QTimer>


class midiIODestroyer;

class midiIO: public QThread
{
    Q_OBJECT

public:
    midiIO();
    static midiIO* Instance();
    void sendSysxMsg(QString sysxOutMsg, int replySize, uint midiOutport, uint midiInPort);
    void sendMidi(QString midiMsg, uint midiOutport);
    QList<QString> getMidiOutDevices();
    QList<QString> getMidiInDevices();
    signed int getOutDevice();
    signed int getInDevice();
    static QString sysxBuffer;
    static QString sysxOutMsg;
    static QString rxMode;
    void closePorts();
    static QString msgType;
    /* BUG FIX (contributed): mutexes protecting shared globals from concurrent thread access */
    static QMutex midiOutMutex;   /* protects global midiout RtMidiOut* */
    static QMutex midiInMutex;    /* protects global midiin RtMidiIn* */
    static QMutex sysxBufferMutex; /* protects static sysxBuffer QString */
    static QString unitID;
    QTimer* stall_timer;

protected :
    friend class midiIODestroyer;
    virtual ~midiIO() { }

signals:
    void errorSignal(QString windowTitle, QString errorMsg);
    void replyMsg(QString sysxBuffer);
    void midiFinished();
    void setStatusSymbol(int value);
    void setStatusProgress(int value);
    void setStatusMessage(QString message);
    void setStatusdBugMessage(QString dBug);
    void errorEvent();
    void midiData(QString message);
    void updateMode(QString mode);

private slots:
    void run();
    void midiIO_stalled();

public slots:
    void getData();

private:
    static midiIO* _instance;
    static midiIODestroyer _destroyer;
    void queryMidiInDevices();
    void queryMidiOutDevices();
    void sendSyxMsg(QString sysxOut, uint midiOutport);
    void sendMidiMsg(QString sysxOutMsg, uint midiOutport);
    QList<QString> midiOutDevices;
    QList<QString> midiInDevices;
    QList<int> midiOutPortMap;
    QList<int> midiInPortMap;
    void receiveMsg();
    void getMsg();
    static int bytesTotal;
    static int bytesReceived;
    signed int midiOutPort;
    signed int midiInPort;
    QString midiMsg;
    int dataSize;
    bool midi = false;
    int replySize;
    QTimer* timer;
    static QString _last;
    QString midi_in_device_name;
    QString midi_out_device_name;
    signed int midi_in_list_number;
    signed int midi_out_list_number;
    QList<QString> leftOvers;
    int last_progress;
    QString deviceID;
};

#endif // MIDIIO_H
