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

#include "midiIO.h"
#include "midiIODestroyer.h"
#include "SysxIO.h"
#include "globalVariables.h"
#include "RtMidi.h"
#include "Preferences.h"
#include <QMessageBox>
#include <QTimer>
#include <QDebug>
#include <QMutexLocker>

int midiIO::bytesTotal = 0;
QString midiIO::sysxBuffer;
QString midiIO::sysxOutMsg;
QString midiIO::rxMode;
QString midiIO::msgType = "name";
QString midiIO::unitID = "10";
std::string clientName = "FloorBoard";
midiIO* midiIO::_instance = 0;// initialize pointer
midiIODestroyer midiIO::_destroyer;
RtMidiOut *midiout = new RtMidiOut();
RtMidiIn *midiin = new RtMidiIn();
RtMidiIn *midiCCIn = nullptr;  // Dedicated CC controller input (lazy init)
/* BUG FIX (contributed): static mutex definitions for thread-safe access to shared globals */
QMutex midiIO::midiOutMutex;
QMutex midiIO::midiInMutex;
QMutex midiIO::sysxBufferMutex;
QString midiIO::_last;

midiIO::midiIO()
{
    this->midi = false; // Set this to false until required;
    this->timer = new QTimer(this);
    /* Connect signals */
    SysxIO *sysxIO = SysxIO::Instance();
    QObject::connect(this, SIGNAL(setStatusSymbol(int)), sysxIO, SIGNAL(setStatusSymbol(int)));
    QObject::connect(this, SIGNAL(setStatusProgress(int)), sysxIO, SIGNAL(setStatusProgress(int)));
    QObject::connect(this, SIGNAL(setStatusMessage(QString)), sysxIO, SIGNAL(setStatusMessage(QString)));
    QObject::connect(this, SIGNAL(setStatusdBugMessage(QString)), sysxIO, SIGNAL(setStatusdBugMessage(QString)));
    QObject::connect(this, SIGNAL(errorSignal(QString,QString)), sysxIO, SLOT(errorSignal(QString,QString)));
    QObject::connect(this, SIGNAL(replyMsg(QString)),	sysxIO, SLOT(receiveSysx(QString)));
    QObject::connect(this, SIGNAL(midiFinished()), sysxIO, SLOT(finishedSending()));
    QObject::connect(this, SIGNAL(errorEvent()), sysxIO, SLOT(errorEvent()));
    QObject::connect(this, SIGNAL(midiData(QString)), sysxIO, SLOT(receiveEvent(QString)));
    QObject::connect(this, SIGNAL(updateMode(QString)), sysxIO, SLOT(updatePatch(QString)));
    QObject::connect(this->timer, SIGNAL(timeout()), this, SLOT(getData()));
    this->timer->start(10);
    //this->stall_timer->start(10000);
    this->rxMode = "false";
    this->deviceID = "10";
    this->midiCCInPort = -1;
    this->cc_in_device_name = "None";
}

midiIO* midiIO::Instance()
{
    /* Multi-threading safe */
    if (!_instance)  // is it the first call?
    {
        _instance = new midiIO; // create sole instance
        _destroyer.SetmidiIO(_instance);
    };
    return _instance; // address of sole instance
}

void midiIO::closePorts()
{
    midiout->closePort();
    midiin->closePort();
    if (midiCCIn) midiCCIn->closePort();
}

/*********************** queryMidiOutDevices() *****************************
 * Retrieves all MIDI Out devices installed on your system and stores them
 * as a QList of QStrings and device id's.
 *************************************************************************/
void midiIO::queryMidiOutDevices()
{
    this->midiOutDevices.clear();
    this->midiOutPortMap.clear();
    std::string portName;
    unsigned int outPorts;
    try
    {
        outPorts = midiout->getPortCount();      /* Check outputs. */
        for (uint i=0; i<outPorts; i++)
        {
            portName = midiout->getPortName(i);
            QString name = QString::fromStdString(portName);
#if defined(Q_OS_WIN)
            name.chop(2);
#endif
            if(name.contains("SY-1000 DAW CTRL") || name.contains("SY-1000 MIDI 2"))
            {
                name = "SY-1000 DAW CTRL";
            };
            if(name.contains("SY-1000") && !name.contains("SY-1000 DAW CTRL") && !name.contains("SY-1000 MIDI 2"))
            {
                name = "SY-1000";
            };
            if(!name.contains("FloorBoard"))
            {
                this->midiOutDevices.append(name);
                this->midiOutPortMap.append(int(i));
            };
        };
    }
    catch (RtMidiError &error)
    {
        emit errorSignal(tr("Midi Output Error"), tr("port availability error"));
        emit setStatusdBugMessage(QString::fromStdString( error.getMessage()));
        this->midiOutDevices.append("Device access error");
        emit errorEvent();
        //exit( EXIT_FAILURE );
        return;
    };
    if (outPorts < 1)
    {
        this->midiOutDevices.push_back(tr("no midi device available"));
        this->midiOutPortMap.append(-1);
    };
}

QList<QString> midiIO::getMidiOutDevices()
{
    queryMidiOutDevices();
    return this->midiOutDevices;
}

int midiIO::getOutDevice()
{
    signed int listPos = -1;
    QList<QString> devList = getMidiOutDevices();
    Preferences *preferences = Preferences::Instance();
    QString devName = preferences->getPreferences("Midi", "MidiOut", "name");
    bool ok = false;
    const int savedIndex = preferences->getPreferences("Midi", "MidiOut", "device").toInt(&ok, 10);

    if(devName.size()<2)
    {
        devName="no device found";
    };

    // Prefer the stored numeric device index when it is valid for the current list.
    if(ok && savedIndex >= 0 && savedIndex < devList.size())
    {
        if(devName.isEmpty() || devList.at(savedIndex) == devName || !devList.contains(devName))
        {
            listPos = savedIndex;
        }
    }

    if(listPos < 0 && devList.contains(devName))  // if the midi device saved in preferences is currently present, then use that.
    {
        listPos = devList.indexOf(devName);
    };

    if(preferences->getPreferences("Midi", "Device", "bool")=="true" && devList.contains("SY-1000"))
    {
        listPos = devList.indexOf("SY-1000");
    };

    if(listPos < 0 || listPos >= this->midiOutPortMap.size())
    {
        listPos = -1;
    };
    midi_out_device_name = (listPos >= 0 && listPos < devList.size()) ? devList.at(listPos) : devName;
    midi_out_list_number = listPos;
    return (listPos >= 0) ? this->midiOutPortMap.at(listPos) : -1;
}

/*********************** queryMidiInDevices() ******************************
 * Retrieves all MIDI In devices installed on your system and stores them
 * as a QList of QStrings and device id's.
 *************************************************************************/
void midiIO::queryMidiInDevices()
{
    this->midiInDevices.clear();
    this->midiInPortMap.clear();
    std::string portName;
    uint inPorts;
    try
    {
        inPorts = midiin->getPortCount();      /* Check inputs. */
        for (uint i=0; i<inPorts; i++)
        {
            portName = midiin->getPortName(i);
            QString name = QString::fromStdString(portName);
#if defined(Q_OS_WIN)
            name.chop(2);
#endif
            if(name.contains("SY-1000 DAW CTRL") || name.contains("SY-1000 MIDI 2"))
            {
                name = "SY-1000 DAW CTRL";
            };
            if(name.contains("SY-1000") && !name.contains("SY-1000 DAW CTRL") && !name.contains("SY-1000 MIDI 2"))
            {
                name = "SY-1000";
            };
            if(!name.contains("FloorBoard"))
            {
                this->midiInDevices.append(name);
                this->midiInPortMap.append(int(i));
            };
        };
    }
    catch (RtMidiError &error)
    {
        emit errorSignal(tr("Midi Input Error"), tr("port availability error"));
        emit setStatusdBugMessage(QString::fromStdString( error.getMessage()));
        this->midiInDevices.append("Device access error");
        emit errorEvent();
        //exit( EXIT_FAILURE );
        return;
    };
    if (inPorts < 1)
    {
        this->midiInDevices.push_back(tr("no midi device available"));
        this->midiInPortMap.append(-1);
    };
}

QList<QString> midiIO::getMidiInDevices()
{
    queryMidiInDevices();
    return this->midiInDevices;
}

int midiIO::getInDevice()
{
    signed int listPos = -1;
    QList<QString> devList = getMidiInDevices();
    Preferences *preferences = Preferences::Instance();
    QString devName = preferences->getPreferences("Midi", "MidiIn", "name");
    bool ok = false;
    const int savedIndex = preferences->getPreferences("Midi", "MidiIn", "device").toInt(&ok, 10);
    if(devName.isEmpty())
    {
        devName="no device found";
    };
    // Prefer the stored numeric device index when it is valid for the current list.
    if(ok && savedIndex >= 0 && savedIndex < devList.size())
    {
        if(devName.isEmpty() || devList.at(savedIndex) == devName || !devList.contains(devName))
        {
            listPos = savedIndex;
        }
    }
    if(listPos < 0 && devList.contains(devName))
    {
        listPos = devList.indexOf(devName);
    };
    if(preferences->getPreferences("Midi", "Device", "bool")=="true" && devList.contains("SY-1000"))
    {
        listPos = devList.indexOf("SY-1000");
    };
    if(listPos < 0 || listPos >= this->midiInPortMap.size())
    {
        listPos = -1;
    };
    midi_in_device_name = (listPos >= 0 && listPos < devList.size()) ? devList.at(listPos) : devName;
    midi_in_list_number = listPos;
    return (listPos >= 0) ? this->midiInPortMap.at(listPos) : -1;
}

/*********************** sendMsg() **********************************
 * Prepares the sysx message before sending on the MIDI Out device. It
 * converts the message from a QString to a std::vector and opens, sends
 * and closes the MIDI device.
 *************************************************************************/
void midiIO::sendSyxMsg(QString sysxOutMsg, uint midiOutPort)
{
    Preferences *preferences = Preferences::Instance();// Load the preferences.
    SysxIO *sysxIO = SysxIO::Instance();
    unsigned int nPorts = midiout->getPortCount();   // Check available ports.
    if (nPorts < 1) { return; }

    int charCount = sysxOutMsg.size();
    if (charCount < 2) { return; }
    int msgLength = charCount / 2;
    if (msgLength <= 0) { return; }

    uchar *ptrBase = new uchar[msgLength];
    QString hex;
    QString debug;
    std::vector<uchar> message;
    int wait = 0;
    bool ok;

    try
    {
        QMutexLocker locker(&midiOutMutex);
        if(!midiout->isPortOpen())
        {
            midiout->openPort(midiOutPort);    // Open selected port.
        };

        uchar *ptr = ptrBase;
        for(int i=0;i<charCount;i+=2)
        {
            hex = sysxOutMsg.mid(i, 2);
            unsigned int value = hex.toUInt(&ok, 16);
            *ptr = uchar(value);
            message.push_back(*ptr);    // insert the char* string into a std::vector
            ptr++;
            wait++;
            emit setStatusProgress(wait);
            if(hex=="F7")
            {
                if(preferences->getPreferences("Midi", "DBug", "bool")=="true")
                {
                    sysxIO->deBug("sendSysx device=" + midi_out_device_name + " type=" + msgType + "  " + debug);
                };
                debug.clear();
                midiout->sendMessage(&message);
                message.clear();
                msleep(20);
            };
        };
    }
    catch (RtMidiError &error)
    {
        QString rtMsg = QString::fromStdString(error.getMessage());
        bool portIssue = !midiout->isPortOpen() || rtMsg.contains("port", Qt::CaseInsensitive);
        emit errorSignal(tr("Syx Output Error"),
                         portIssue ? tr("port availability error") : tr("data error"));
        emit setStatusdBugMessage(rtMsg);
        emit errorEvent();
        midiout->closePort();
        sysxIO->deBug("midiIO EXIT FAILURE !! sendSyxMsg");
    };

    delete[] ptrBase;
    msleep(1);
}
/*********** send midi messages *******************************/
void midiIO::sendMidiMsg(QString sysxOutMsg, uint midiOutPort)
{
    unsigned int nPorts = midiout->getPortCount();   // Check available ports.
    if (nPorts < 1) { return; }

    int msgLength = sysxOutMsg.length()/2;
    if (msgLength <= 0) { return; }

    char *ptrBase = nullptr;

    try
    {
        QMutexLocker locker(&midiOutMutex);
        if(!midiout->isPortOpen()) { midiout->openPort(midiOutPort); }
        std::vector<unsigned char> message;
        ptrBase = new char[msgLength];
        char *ptr = ptrBase;

        for(int i=0;i<msgLength*2;i+=2)
        {
            QString hex;
            bool ok;
            hex.append(sysxOutMsg.mid(i, 2));
            uint n = hex.toUInt(&ok, 16);
            if(!ok) { continue; }
            *ptr = char(n);
            message.push_back(*ptr);    // insert the char* string into a std::vector
            ptr++;
        };
        midiout->sendMessage(&message);  // send the midi data as a std::vector
    }
    catch(RtMidiError &error)
    {
        QString rtMsg = QString::fromStdString(error.getMessage());
        bool portIssue = !midiout->isPortOpen() || rtMsg.contains("port", Qt::CaseInsensitive);
        emit errorSignal(tr("Midi Output Error"),
                         portIssue ? tr("port availability error") : tr("data error"));
        emit setStatusdBugMessage(rtMsg);
        emit errorEvent();
        midiout->closePort();
    };
    delete[] ptrBase;
    msleep(1);
}
/*********************** receiveMsg() **********************************
 * Prepares the sysx message after receiving on the MIDI In device. It
 *  and opens, receives and closes the MIDI device then converts the
 *  message from a std::vector to a QString.
 *************************************************************************/

void midiIO::receiveMsg()
{
    SysxIO *sysxIO = SysxIO::Instance();
    this->rxMode = "true";
    emit setStatusSymbol(3);
    int x = 0;
    int loopCount = replySize/4;
    int nPorts = midiin->getPortCount();	                        // check we have a midiIn port
    if (nPorts > 0)
    {
        try {
            QMutexLocker locker(&midiInMutex);  // BUG FIX: guard midiin against concurrent getData() access
            midiin->ignoreTypes(false, true, true);               //don,t ignore sysex messages, but ignore other crap like active-sensing
            if(!midiin->isPortOpen())
            {
                midiin->openPort(midiInPort);                       // open the midi in port
            };
            sendSyxMsg(this->sysxOutMsg, this->midiOutPort);        // send the data request message out
            int cycle = 0;
            while (x<loopCount && sysxBuffer.size()/2 < replySize)  // wait until exact bytes received or timeout
            {
                if(cycle>1000)
                {
                    sysxIO->deBug("midiIO::receiveMsg cycle>1000 break !!!!!");
                    break;
                };
                msleep(10);
                getMsg();

                if(!sysxIO->isConnected() && !msgType.contains("identity"))
                {
                    return;
                };

                int safeReplySize = (replySize > 0) ? replySize : 1;   // BUG FIX: prevent SIGFPE divide-by-zero
                int safeLoopCount = (loopCount > 0) ? loopCount : 1;   // BUG FIX: prevent SIGFPE divide-by-zero
                int s = (this->sysxBuffer.size()*50)/safeReplySize;     // time it takes, or size, to get all sysx message data in.
                int t = (x*100)/safeLoopCount;

                if(s>t)
                {
                    t=s;
                };
                if(t != last_progress)
                {
                    emit setStatusProgress(t);                          // status bar indicates % of data received.
                };
                last_progress = t;
                x++;
                if(sysxBuffer.isEmpty()){cycle++;};
            };
        }
        catch (RtMidiError &error)
        {
            QString rtMsg = QString::fromStdString(error.getMessage());
            bool portIssue = !midiin->isPortOpen() || rtMsg.contains("port", Qt::CaseInsensitive);
            emit errorSignal(tr("Midi Input Error"),
                             portIssue ? tr("port availability error") : tr("data error"));
            emit setStatusdBugMessage(rtMsg);
            emit errorEvent();
            { QMutexLocker cleanupLocker(&midiInMutex); if(midiin->isPortOpen()) { midiin->closePort(); } }
            //exit( EXIT_FAILURE );
            SysxIO *sysxIO = SysxIO::Instance();
            sysxIO->deBug("midiIO EXIT FAILURE !! receiveMsg");
        };
    };

    this->rxMode = "false";
    emit setStatusProgress(100);
    Preferences *preferences = Preferences::Instance();// Load the preferences.
    if(preferences->getPreferences("Midi", "DBug", "bool")=="true" && msgType=="identity" && sysxBuffer.isEmpty())
    {
        qWarning("IDENTITY_REPLY_MISSING no SysEx identity response received");
    };
    this->sysxOutMsg.clear();
}

void midiIO::getMsg()
{
    QString rxData;
    SysxIO *sysxIO = SysxIO::Instance();
    Preferences *preferences = Preferences::Instance();// Load the preferences.
    emit setStatusSymbol(3);
    try {
        midiin->ignoreTypes(false, true, true);       //don,t ignore sysex messages, but ignore other crap like active-sensing
        if(!midiin->isPortOpen())
        {
            midiin->openPort(this->midiInPort);            // open the midi in port
        };
        std::vector<uchar> message;
        double stamp = midiin->getMessage(&message);
        Q_UNUSED(stamp);

        int nBytes = message.size();
        for (int i=0; i<nBytes; i++)
        {
            int n = int(message.at(i));					// convert std::vector to QString
            QString hex = QString::number(n, 16).toUpper();
            if (hex.length() < 2) hex.prepend("0");
            rxData.append(hex);
        };
    }
    catch (RtMidiError &error)
    {
        midiin->closePort();
        QString rtMsg = QString::fromStdString(error.getMessage());
        bool portIssue = rtMsg.contains("port", Qt::CaseInsensitive);
        emit errorSignal(tr("getData() Midi Input Error"),
                         portIssue ? tr("port availability error") : tr("data error"));
        emit setStatusdBugMessage("getData() " + rtMsg);
        emit errorEvent();
        //exit( EXIT_FAILURE );
        SysxIO *sysxIO = SysxIO::Instance();
        sysxIO->deBug("midiIO EXIT FAILURE !! getMsg");
    };

    if(rxData.size()>0)
    {
        for(int x=0; x<rxData.size(); x++)
        {
            if(rxData.at(x)==QString("8") || rxData.at(x)==QString("9") || rxData.at(x)==QString("B"))
            {            
                if(preferences->getPreferences("Midi", "DBug", "bool")=="true")
                {
                    sysxIO->deBug(midi_in_device_name + " data removed = " + rxData.mid(x, 6) + " from " + rxData + " at " + QString::number(x, 10));
                };
                rxData.remove(x, 6);
                x=x+4;
            };

            if(rxData.at(x)==QString("C"))
            {
                if(preferences->getPreferences("Midi", "DBug", "bool")=="true")
                {
                    sysxIO->deBug(midi_in_device_name + " data removed = " + rxData.mid(x, 4) + " from " + rxData + " at " + QString::number(x, 10));
                };
                rxData.remove(x, 4);
                x=x+2;
            };
            x=x+1;
        };

        if(msgType.contains("patch") && !sysxOutMsg.contains("F041"+unitID+"000000691110") && (rxData.contains("F041"+unitID+"000000691210") || rxData.contains("F041"+unitID+"00000069127F") || rxData.mid(16, 3)!=sysxOutMsg.mid(16, 3)))
        {
            if(rxData.contains("F041"+unitID+"000000691210") || rxData.contains("F041"+unitID+"00000069127F"))
            {
                //emit midiData(rxData.trimmed());
                leftOvers.append(rxData);
                sysxIO->deBug("data extracted from patch and sent to midiData() to process " + rxData.trimmed());
                rxData.clear();
            }

            if(preferences->getPreferences("Midi", "DBug", "bool")=="true" && rxData.size()>0)
            {
                sysxIO->deBug(midi_in_device_name + " " + msgType + "scrubbed = " + rxData);
            };

            rxData.clear();
        };

        if(msgType.contains("identity") && !rxData.contains(idReplyPatern))
        {
            if(preferences->getPreferences("Midi", "DBug", "bool")=="true") sysxIO->deBug(midi_in_device_name + " " + msgType + " scrubbed from Identity_receive=" + rxData);
            rxData.clear();
        };

        if(msgType.contains("bulkNames") && !rxData.contains("F041"+unitID+"000000691253") && !rxData.contains("F041"+unitID+"000000691252"))
        {
            if(preferences->getPreferences("Midi", "DBug", "bool")=="true")
            {
                sysxIO->deBug(midi_in_device_name + " " + msgType + " scrubbed from bulk_names= " + rxData);
            };
            rxData.clear();
        };

        if(msgType.contains("system") && rxData.contains("F041"+unitID+"0000006912100"))
        {
            if(preferences->getPreferences("Midi", "DBug", "bool")=="true")
            {
                sysxIO->deBug(midi_in_device_name + " " + msgType + " scrubbed from system = " + rxData);
            };
            rxData.clear();
        };

        for(int x=0; x<rxData.size(); x++)
        {
            if(rxData.mid(x, 2).contains("F8"))
            {
                if(preferences->getPreferences("Midi", "DBug", "bool")=="true")
                {
                    sysxIO->deBug("[F8] data removed = " + rxData.mid(x, 2) + /*" from " + rxData +*/ " at " + QString::number(x, 10));
                };
                rxData.remove(rxData.mid(x, 2));
            };
            if(rxData.mid(x, 2).contains("FE"))
            {
                if(preferences->getPreferences("Midi", "DBug", "bool")=="true")
                {
                    sysxIO->deBug("[FE] data removed = " + rxData.mid(x, 2) + /*" from " + rxData +*/ " at " + QString::number(x, 10));
                };
                rxData.remove(rxData.mid(x, 2));
            };
            x=x+1;
        };

        QString addr = sysxOutMsg.mid(16,2);
        QString header = "F041"+unitID+"0000006912";
        if(rxData.contains(sysxOutMsg))
        {
            rxData.remove(0, sysxOutMsg.size()*2);
        };
        if((msgType=="patch" || msgType=="system") && rxData.contains(header+addr))
        {
            QMutexLocker locker(&sysxBufferMutex);
            sysxBuffer.append(rxData.trimmed());
        }
        else if(msgType=="system" && rxData.contains(header+addr))
        {
            QMutexLocker locker(&sysxBufferMutex);
            sysxBuffer.append(rxData.trimmed());
        }
        else if(msgType=="bulkNames" && rxData.contains(header+addr))
        {
            QMutexLocker locker(&sysxBufferMutex);
            sysxBuffer.append(rxData.trimmed());
        }
        else if(msgType=="command_rx" && rxData.contains(header+addr))
        {
            QMutexLocker locker(&sysxBufferMutex);
            sysxBuffer.append(rxData.trimmed());
        }
        else if(msgType=="identity")
        {
            this->unitID=sysxBuffer.mid(4,2);
            QMutexLocker locker(&sysxBufferMutex);
            sysxBuffer.append(rxData.trimmed());
        }
        else if(rxData.size()>16)
        {
            //emit midiData(rxData.trimmed());
            leftOvers.append(rxData);
        };

        if(preferences->getPreferences("Midi", "DBug", "bool")=="true" && rxData.size()>0)
        {
            sysxIO->deBug(midi_in_device_name + " getMsg = " + rxData);
        };
        rxData.clear();
    };
}

void midiIO::midiIO_stalled()
{
    SysxIO *sysxIO = SysxIO::Instance();
    sysxIO->deBug("midiIO stalled + buffer=" + sysxBuffer);
    emit replyMsg(sysxBuffer);
    sysxBuffer.clear();
    emit setStatusSymbol(1);
    emit setStatusProgress(0);
    emit midiFinished();
}


void midiIO::getData()
{
    try{
        if(this->midiInPort>=0)
        {
            SysxIO *sysxIO = SysxIO::Instance();
            if(this->rxMode == "false" && sysxIO->isConnected())
            {
                QMutexLocker locker(&midiInMutex);  // BUG FIX: guard midiin against concurrent receiveMsg() access
                midiin->ignoreTypes(false, true, true);
                if(!midiin->isPortOpen())
                {
                    midiin->openPort(getInDevice());
                };

                std::vector<unsigned char> message;
                double stamp = midiin->getMessage(&message);

                Q_UNUSED(stamp);
                QString rxData;
                uint nBytes = message.size();
                for (uint i=0; i<nBytes; i++)
                {
                    uint n = uint(message.at(i));					// convert std::vector to QString
                    QString hex = QString::number(n, 16).toUpper();
                    if (hex.length() < 2) hex.prepend("0");
                    rxData.append(hex);
                };

                if(rxData.size()>0 && sysxIO->isConnected())
                {
                    Preferences *preferences = Preferences::Instance();      // Load the preferences.
                    if(preferences->getPreferences("Midi", "DBug", "bool")=="true")
                    {
                        sysxIO->deBug(midi_in_device_name + " getData = " + rxData);
                    };
                    emit midiData(rxData.trimmed());
                };
            };
        };
        // Poll CC controller input (separate port) — runs independently of SY-1000 connection
        pollCCInput();
    }
    catch (RtMidiError &error)
    {
        midiin->closePort();
        QString rtMsg = QString::fromStdString(error.getMessage());
        bool portIssue = rtMsg.contains("port", Qt::CaseInsensitive);
        emit errorSignal(tr("getData() Midi Input Error"),
                         portIssue ? tr("port availability error") : tr("data error"));
        emit setStatusdBugMessage("getData() " + rtMsg);
        emit errorEvent();
        //exit( EXIT_FAILURE );
        SysxIO *sysxIO = SysxIO::Instance();
        sysxIO->deBug("midiIO EXIT FAILURE !! getData");
    };
}

/**************************** run() **************************************
 * New QThread that processes the sysex or midi message and handles if yes
 * or no it has to start receiving a reply on the MIDI In device midiIn. If
 * so midiCallback() will handle the receive of the incomming sysx message.
 *************************************************************************/
void midiIO::run()
{
    //int repeat = 0;
    if(midi && (midiMsg.size()>1))	// Check if we are going to send sysx or midi data & have an actual midi message to send.
    {
        if (midiMsg.size() <= 6)		// if the midi message is <= 3 words
        {
            sysxOutMsg = midiMsg;   // use the same sending routine as sysx messages.
            this->sendMidiMsg(sysxOutMsg, midiOutPort);
        }
        else
        {
            sysxOutMsg = midiMsg.mid(0, 6);
            this->sendMidiMsg(sysxOutMsg, midiOutPort);

            sysxOutMsg = midiMsg.mid(6, 6);
            this->sendMidiMsg(sysxOutMsg, midiOutPort);

            sysxOutMsg = midiMsg.mid(12, 4);
            this->sendMidiMsg(sysxOutMsg, midiOutPort);
        };
        emit setStatusSymbol(2);
        emit setStatusProgress(100);
        msleep(20);
        emit midiFinished(); // We are finished so we send a signal to free the device.
    }
    else   // if not a midi message, then it must be a sysx message
    {

    /* Check if we are going to receive something on sending */
    if(this->sysxOutMsg.mid(14, 2)=="11" ||
       msgType=="identity" ||
       msgType=="command" ||
       msgType=="command_send_wait_reply")
    {
        emit setStatusSymbol(3);
        emit setStatusMessage(tr("Receive ")+msgType);
        if(sysxOutMsg != _last || msgType=="identity")
        {
            _last = sysxOutMsg;
            sysxBuffer.clear();
            receiveMsg();
        };
    }
    else
    {
        emit setStatusSymbol(2);
        emit setStatusMessage(tr("Sending"));
        emit setStatusProgress(100);
        this->sendSyxMsg(this->sysxOutMsg, midiOutPort);
    };
    if(msgType=="patch"){sysxBuffer.truncate(replySize*2);};
    if(msgType=="identity"){this->unitID=sysxBuffer.mid(4,2);};
    emit replyMsg(sysxBuffer);
    sysxBuffer.clear();
    emit setStatusSymbol(1);
    emit setStatusProgress(0);
    emit midiFinished();             // We are finished so we send a signal to free the device.

    if(msgType.contains("patch")/* || msgType.contains("system")*/ && sysxOutMsg.contains("F041000000006911"))
    {
        //emit updateMode("");
    };
    }  // end else (sysx path)
    //quit();
    if(!leftOvers.isEmpty())
    {
        SysxIO *sysxIO = SysxIO::Instance();
        for(int x=0; x<leftOvers.size(); x++)
        {
            sysxIO->deBug("midiIO::leftOvers=" + leftOvers.at(x));
            emit midiData(leftOvers.at(x));
        };
        leftOvers.clear();
    };
}

/*********************** sendSysxMsg() ***********************************
 * Starts a new thread that handles the send and receive of sysex messages.
 *************************************************************************/
void midiIO::sendSysxMsg(QString sysxOut, int replySize, uint midiOutPort, uint midiInPort)
{
    Q_UNUSED(midiOutPort);
    Q_UNUSED(midiInPort);
    if(sysxOut.contains(idRequestString) || sysxOut.size()<21)
    {
        this->sysxOutMsg = sysxOut;
    }
    else
    {
        QString reBuild;
        QString sysxEOF;
        QString hex;
        int msgLength = sysxOut.length();
        for(int i=0;i<msgLength;++i)
        {
            hex.append(sysxOut.mid(i*2, 2));
            sysxEOF = sysxOut.mid((i*2)+4, 2);
            if(sysxEOF == "F7")
            {
                int dataSize = 0; bool ok;
                for(int h=checksumOffset;h<hex.size()-1;++h)
                {
                    dataSize += hex.mid(h*2, 2).toInt(&ok, 16);
                };
                int sum = dataSize % 128;   // checksum calculate.
                if(sum!=0)
                {
                    sum = 128 - sum;
                };
                QString checksum = QString::number(sum, 16).toUpper();
                if(checksum.length()<2)
                {
                    checksum.prepend("0");
                };
                hex.append(checksum);
                hex.append("F7");
                hex.replace(4, 2, this->unitID);
                reBuild.append(hex);
                hex.clear();
                sysxEOF.clear();
                i=i+2;
            };
        };
        this->sysxOutMsg = reBuild;
    };

    msgType.clear();
    if (this->sysxOutMsg.contains(idRequestString))
    {
        msgType = "identity";
        emit setStatusdBugMessage(tr("identity request"));  // identity request not require checksum
    }
    else if(this->sysxOutMsg.mid(6, 12).contains("00000069127F"))
    {
        if(replySize > 0)
        {
            msgType = "command_send_wait_reply";
            emit setStatusdBugMessage(tr("command send + receive"));
        }
        else
        {
            msgType = "command_send_only";
            emit setStatusdBugMessage(tr("command send only"));
        }
    }
    else if(this->sysxOutMsg.mid(6, 12).contains("00000069117F"))
    {
        msgType = "command_send_wait_reply";
        emit setStatusdBugMessage(tr("command send + receive"));
    }
    else if(replySize==bulkPatchNamesSize)
    {
        msgType = "bulkNames";
    }
    else if(this->sysxOutMsg.mid(6, 10).contains("0000006911") && this->sysxOutMsg.mid(24, 8).contains(patchRequestDataSize))
    {
        msgType = "patch";
        emit setStatusdBugMessage(tr("patch request"));
    }
    else if(this->sysxOutMsg.mid(6, 10).contains("0000006911") && this->sysxOutMsg.mid(24, 8).contains("00000010"))
    {
        msgType = "name";
        emit setStatusdBugMessage(tr("name request"));
    }
    else if(this->sysxOutMsg.mid(6, 30).contains("000000691100000000000B220053F7"))
    {
        msgType = "system";
        emit setStatusdBugMessage(tr("system request"));
    }
    else if(replySize==0)
    {
        msgType = "send only";
    };
    this->replySize = replySize;
    this->midiOutPort = getOutDevice();   //midiOutPort;
    this->midiInPort = getInDevice();
    this->midi = false;
    qWarning("MIDI_ROUTE type=%s outPort=%d outName=%s inPort=%d inName=%s bytes=%d",
             msgType.toUtf8().constData(),
             this->midiOutPort,
             midi_out_device_name.toUtf8().constData(),
             this->midiInPort,
             midi_in_device_name.toUtf8().constData(),
             int(this->sysxOutMsg.size()/2));

    if(this->midiOutPort > -1)
    {
        start();
    }
    else
    {
        SysxIO *sysxIO = SysxIO::Instance();
        sysxIO->deBug("no midi device set " + this->sysxOutMsg);
        emit setStatusSymbol(0);
        emit setStatusMessage(tr("no midi device set"));
        emit replyMsg("");
        emit setStatusSymbol(1);
        emit setStatusProgress(0);
        emit midiFinished();
    };
}

/*********************** sendMidi() **********************************
 * Starts a new thread that handles the sending of the midi messages.
 **********************************************************************/
void midiIO::sendMidi(QString midiMsg, uint midiOutPort)
{
    Q_UNUSED(midiMsg);
    Q_UNUSED(midiOutPort);
    this->midiOutPort = getOutDevice();   //midiOutPort;
    this->midiMsg = midiMsg;
    this->midi = true;
    if(this->midiOutPort > -1)
    {
        start();
    }
    else
    {
        emit setStatusSymbol(0);
        emit setStatusMessage(tr("no midi device set"));
        emit setStatusSymbol(1);
        emit setStatusProgress(0);
        emit midiFinished();
    };
}

/************************* CC Controller Input ******************************
* Separate RtMidiIn port for external CC controllers (Launch Control XL etc.)
* Called from getData() timer every 10ms.
****************************************************************************/
signed int midiIO::getCCInDevice()
{
    Preferences *preferences = Preferences::Instance();
    QString deviceName = preferences->getPreferences("Midi", "CCIn", "name");

    // Auto-detect: if no CC device configured, look for Launch Control XL
    if (deviceName.isEmpty() || deviceName == "None") {
        for (int i = 0; i < midiInDevices.size(); i++) {
            if (midiInDevices.at(i).contains("LCXL") || midiInDevices.at(i).contains("Launch Control")) {
                return midiInPortMap.at(i);
            }
        }
        return -1;
    }

    // Resolve by name first (port order can change)
    for (int i = 0; i < midiInDevices.size(); i++) {
        if (midiInDevices.at(i) == deviceName)
            return midiInPortMap.at(i);
    }
    // Fallback to index
    bool ok;
    int deviceIndex = preferences->getPreferences("Midi", "CCIn", "device").toInt(&ok, 10);
    if (ok && deviceIndex >= 0 && deviceIndex < midiInPortMap.size())
        return midiInPortMap.at(deviceIndex);
    return -1;
}

void midiIO::pollCCInput()
{
    try {
        signed int ccPort = getCCInDevice();
        if (ccPort < 0) {
            if (midiCCIn && midiCCIn->isPortOpen()) midiCCIn->closePort();
            return;
        }

        if (!midiCCIn) midiCCIn = new RtMidiIn();
        midiCCIn->ignoreTypes(true, true, true);  // Ignore SysEx, timing, active sense
        if (!midiCCIn->isPortOpen()) {
            midiCCIn->openPort(ccPort);
        }

        // Drain all queued messages (handles fast knob sweeps)
        std::vector<unsigned char> message;
        while (true) {
            midiCCIn->getMessage(&message);
            if (message.size() == 0) break;

            if (message.size() == 3) {
                unsigned char status = message[0];
                // CC message: 0xBn where n = channel
                if ((status & 0xF0) == 0xB0) {
                    int channel = status & 0x0F;        // 0-based channel
                    int ccNumber = (int)message[1];
                    int value = (int)message[2];
                    emit ccReceived(channel, ccNumber, value);
                }
            }
            message.clear();
        }
    }
    catch (RtMidiError &error) {
        if (midiCCIn->isPortOpen()) midiCCIn->closePort();
        SysxIO *sysxIO = SysxIO::Instance();
        sysxIO->deBug("pollCCInput error: " + QString::fromStdString(error.getMessage()));
    }
}
