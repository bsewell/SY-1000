/****************************************************************************
**
** Copyright (C) 2020 Colin Willcocks.
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

#include "menuPage.h"
//#include "MidiTable.h"
#include "SysxIO.h"
#include "globalVariables.h"
//#include "floorBoardDisplay.h"
//#include "floorBoard.h"
#include "Preferences.h"
#include <QTimer>
#include <QDebug>
// Platform-dependent sleep routines.
#ifdef Q_OS_WIN
#include <windows.h>
#define SLEEP( milliseconds ) Sleep( (DWORD) milliseconds )
#else // Unix variants & Mac
#include <unistd.h>
#define SLEEP( milliseconds ) usleep( (unsigned long) (milliseconds * 1000.0) )
#endif

menuPage::menuPage(QWidget *parent, unsigned int id, QString imagePath, QPoint stompPos)
    : QWidget(parent)
{
    Q_UNUSED(imagePath);
    Q_UNUSED(stompPos);
    this->id = id;
    this->setWhatsThis(tr("Deep editing of the selected effect<br>pressing this button will open an edit page<br>allowing detailed setting of this effects parameters."));


    //this->setFixedSize(stompSize);

    this->editDialog = new editWindow();

    //this->menuButton = new customButton(tr(""), false, QPoint(0, 0), this, ":/images/menu_pushbutton.png");

    //QObject::connect(this, SIGNAL( valueChanged(QString, QString, QString) ), this->parent(), SIGNAL( valueChanged(QString, QString, QString) ));

    //QObject::connect(this->parent(), SIGNAL( updateStompOffset(signed int) ), this, SLOT( updatePos(signed int) ));

    QObject::connect(this->parent(), SIGNAL( updateSignal() ), this, SLOT( generalUpdate() ));

    QObject::connect(this->parent(), SIGNAL( updateSignal() ), this, SLOT( updateSignal() ));

    QObject::connect(this, SIGNAL( systemUpdateSignal() ), this->parent()->parent(), SIGNAL( updateSignal() ));

    QObject::connect(this->editDialog, SIGNAL( updateSignal() ), this, SLOT( updateSignal() ));

    QObject::connect(this, SIGNAL( dialogUpdateSignal() ), this->editDialog, SIGNAL( dialogUpdateSignal() ));

    QObject::connect(this->parent(), SIGNAL( updateSignal() ), this->editDialog, SIGNAL( dialogUpdateSignal() ));

    //QObject::connect(this->editDialog, SIGNAL( updateSignal() ), this, SLOT( setDisplayToFxName() ));

    QObject::connect(this, SIGNAL( setEditDialog(editWindow*) ), this->parent(), SLOT( setEditDialog(editWindow*) ));

    QObject::connect(this->parent(), SIGNAL(pedal_buttonSignal(bool)), this, SLOT(pedal_ButtonSignal(bool) ));
    QObject::connect(this->parent(), SIGNAL(pedal_buttonSignal(bool)), this->parent(), SLOT(menuButtonSignal()));

    QObject::connect(this->parent(), SIGNAL(setup_buttonSignal(bool)), this, SLOT(setup_ButtonSignal(bool) ));
    QObject::connect(this->parent(), SIGNAL(setup_buttonSignal(bool)), this->parent(), SLOT(menuButtonSignal()));

    QObject::connect(this->parent(), SIGNAL(system_buttonSignal(bool)), this, SLOT(system_ButtonSignal(bool) ));
    QObject::connect(this->parent(), SIGNAL(system_buttonSignal(bool)), this->parent(), SLOT(menuButtonSignal()));

    QObject::connect(this->parent(), SIGNAL(tuner_buttonSignal(bool)), this, SLOT(tuner_ButtonSignal(bool) ));
    QObject::connect(this->parent(), SIGNAL(tuner_buttonSignal(bool)), this->parent(), SLOT(menuButtonSignal()));

    QObject::connect(this->parent(), SIGNAL(assign1_buttonSignal(bool)), this, SLOT(assign1_ButtonSignal(bool) ));
    QObject::connect(this, SIGNAL(assign1_statusSignal(bool)), this->parent(), SIGNAL(assign1_statusSignal(bool)));
    QObject::connect(this->parent(), SIGNAL(assign1_buttonSignal(bool)), this->parent(), SLOT(menuButtonSignal()));

    SysxIO *sysxIO = SysxIO::Instance();
    QObject::connect(this, SIGNAL(setStatusSymbol(int)), sysxIO, SIGNAL(setStatusSymbol(int)));
    QObject::connect(this, SIGNAL(setStatusProgress(int)), sysxIO, SIGNAL(setStatusProgress(int)));
    QObject::connect(this, SIGNAL(setStatusMessage(QString)), sysxIO, SIGNAL(setStatusMessage(QString)));
}

void menuPage::paintEvent(QPaintEvent *)
{

}

editWindow* menuPage::editDetails()
{
    return this->editDialog;
}

void menuPage::pedal_ButtonSignal(bool value)
{
    if (this->id == 12 && value == true)
    {
        emitValueChanged(this->hex1, this->hex2, "00", "void");
        this->editDialog->setWindow(this->fxName);
        emit setEditDialog(this->editDialog);
    };
}

void menuPage::system_ButtonSignal(bool value)
{
    Q_UNUSED(value);
    SysxIO *sysxIO = SysxIO::Instance();
    if(this->id == 14)
    {
        sysxIO->systemDataRequest();
        SLEEP(2000);
        emitValueChanged(this->hex1, this->hex2, "00", "void");
        this->editDialog->setWindow(this->fxName);
        emit setEditDialog(this->editDialog);
        emit systemUpdateSignal();
    };
}

void menuPage::assign1_ButtonSignal(bool value)
{
    if (this->id == 15 && value == true)
    {
        emitValueChanged(this->hex1, this->hex2, "00", "void");
        this->editDialog->setWindow(this->fxName);
        emit setEditDialog(this->editDialog);
    };
    generalUpdate();
}

void menuPage::setup_ButtonSignal(bool value)
{
    if (this->id == 11 && value == true)
    {
        SysxIO *sysxIO = SysxIO::Instance();
        if(!sysxIO->isConnected())
        {
            emit setStatusSymbol(0);
            emit setStatusMessage(tr("Not connected"));
            qWarning("SETUP_COMMAND_BLOCKED not connected");
            return;
        }
        emitValueChanged(this->hex1, this->hex2, "00", "void");
        this->editDialog->setWindow(this->fxName);
        emit setEditDialog(this->editDialog);
        QString hex1 = "00";
        Preferences *preferences = Preferences::Instance();
        if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
        {
            hex1 = "02";
        };
        int value = sysxIO->getSourceValue("10", hex1, "12", "00");   // input type
        if(value==0)
        {
            sysxIO->setMode("mode5");
            qWarning("SETUP_COMMAND_SENT mode=5");
        }
        else
        {
            sysxIO->setMode("mode6");
            qWarning("SETUP_COMMAND_SENT mode=6");
        };
    };
}

void menuPage::tuner_ButtonSignal(bool value)
{
    if (this->id == 10 && value == true)
    {
        SysxIO *sysxIO = SysxIO::Instance();
        if(!sysxIO->isConnected())
        {
            emit setStatusSymbol(0);
            emit setStatusMessage(tr("Not connected"));
            qWarning("TUNER_COMMAND_BLOCKED not connected");
            return;
        }
        emitValueChanged(this->hex1, this->hex2, "00", "void");
        this->editDialog->setWindow(this->fxName);
        emit setEditDialog(this->editDialog);
        QString hex1 = "01";
        Preferences *preferences = Preferences::Instance();
        if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
        {
            hex1 = "08";
        };
        int value = sysxIO->getSourceValue("00", hex1, "00", "04");   // tuner type
        if(value==0)
        {
            sysxIO->setMode("mode3");
            qWarning("TUNER_COMMAND_SENT mode=3");
        }
        else
        {
            sysxIO->setMode("mode4");
            qWarning("TUNER_COMMAND_SENT mode=4");
        };
    };
}

void menuPage::systemReply(QString replyMsg)
{
    SysxIO *sysxIO = SysxIO::Instance();
    QObject::disconnect(sysxIO, SIGNAL(sysxReply(QString)), this, SLOT(systemReply(QString)));
    sysxIO->setDeviceReady(true); // Free the device after finishing interaction.

    if(replyMsg.size()/2 == systemReplySize)  //  from the SY-1000
    {
        //QString area = "System";
        // sysxIO->setFileSource(area, replyMsg);		// Set the source to the data received.
        sysxIO->setFileName(tr("System Data from ") + deviceType);	// Set the file name to SY-1000B system for the display.
        sysxIO->setDevice(true);				// Patch received from the device so this is set to true.
        sysxIO->setSyncStatus(true);			// We can't be more in sync than right now! :)
        emit systemUpdateSignal();
    }
    else
    {
        QMessageBox *msgBox = new QMessageBox();
        msgBox->setWindowTitle(deviceType + tr(" FloorBoard connection Error !!"));
        msgBox->setIcon(QMessageBox::Warning);
        msgBox->setTextFormat(Qt::RichText);
        QString msgText;
        msgText.append("<font size='+1'><b>");
        msgText.append(tr("The Boss ") + deviceType + tr(" System data was not transferred !!."));
        msgText.append("<b></font><br>");
        msgBox->setText(msgText);
        msgBox->show();
        QTimer::singleShot(3000, msgBox, SLOT(deleteLater()));
    };

    // emit setStatusMessage(tr("Ready"));
}

void menuPage::setPos(QPoint newPos)
{
    Q_UNUSED(newPos);
}

void menuPage::updatePos(signed int offsetDif)
{
    Q_UNUSED(offsetDif);
}

void menuPage::setImage(QString imagePath)
{
    this->imagePath = imagePath;
}

void menuPage::setSize(QSize newSize)
{
    Q_UNUSED(newSize);
}

void menuPage::setId(unsigned int id)
{
    this->id = id;
}

unsigned int menuPage::getId()
{
    return this->id;
}

void menuPage::setLSB(QString hex1, QString hex2)
{
    this->hex1 = hex1;
    this->hex2 = hex2;
    this->editDialog->setLSB(hex1, hex2);
}

void menuPage::updateSwitch(QString hex0, QString hex1, QString hex2, QString hex3)
{
    SysxIO *sysxIO = SysxIO::Instance();
    int value = sysxIO->getSourceValue(hex0, hex1, hex2, hex3);
    bool set = false;
    if (value >= 1) {set = true; };
    if(this->id == 15) { emit assign1_statusSignal(set); };
}

void menuPage::valueChanged(bool value, QString hex1, QString hex2, QString hex3)
{
    Q_UNUSED(value);
    Q_UNUSED(hex1);
    Q_UNUSED(hex2);
    Q_UNUSED(hex3);
}


void menuPage::emitValueChanged(QString hex1, QString hex2, QString hex3, QString valueHex)
{
    Q_UNUSED(valueHex);
    Q_UNUSED(hex1);
    Q_UNUSED(hex2);
    Q_UNUSED(hex3);
    if (this->id == 10){this->fxName = tr("TUNER");             this->area_mode = "Structure";};
    if (this->id == 11){this->fxName = tr("INPUT SETUP");       this->area_mode = "Structure";};
    if (this->id == 12){this->fxName = tr("PEDAL/GK");          this->area_mode = "Structure";};
    if (this->id == 14){this->fxName = tr("SYSTEM/GLOBAL");     this->area_mode = "System";   };
    if (this->id == 15){this->fxName = tr("ASSIGN 1~16");       this->area_mode = "Structure";};
    if(this->id != 10)
    {
        SysxIO *sysxIO = SysxIO::Instance();
        if(sysxIO->mode>1)
        {
            sysxIO->setMode("mode2");
        };
    };
}

void menuPage::setDisplayToFxName()
{
    //  emit valueChanged(this->fxName, "", "");
}

void menuPage::generalUpdate()
{
    Preferences *preferences = Preferences::Instance();
    SysxIO *sysxIO = SysxIO::Instance();
    QString hexx = "00";
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){hexx = "02";};  // if any assigns are on, then light up assign button.
    if(    sysxIO->getSourceValue("10", hexx, "04", "00")>0
        || sysxIO->getSourceValue("10", hexx, "04", "40")>0
        || sysxIO->getSourceValue("10", hexx, "05", "00")>0
        || sysxIO->getSourceValue("10", hexx, "05", "40")>0
        || sysxIO->getSourceValue("10", hexx, "06", "00")>0
        || sysxIO->getSourceValue("10", hexx, "06", "40")>0
        || sysxIO->getSourceValue("10", hexx, "07", "00")>0
        || sysxIO->getSourceValue("10", hexx, "07", "40")>0
        || sysxIO->getSourceValue("10", hexx, "08", "00")>0
        || sysxIO->getSourceValue("10", hexx, "08", "40")>0
        || sysxIO->getSourceValue("10", hexx, "09", "00")>0
        || sysxIO->getSourceValue("10", hexx, "09", "40")>0
        || sysxIO->getSourceValue("10", hexx, "0A", "00")>0
        || sysxIO->getSourceValue("10", hexx, "0A", "40")>0
        || sysxIO->getSourceValue("10", hexx, "0B", "00")>0
        || sysxIO->getSourceValue("10", hexx, "0B", "40")>0
        )
    {
        emit assign1_statusSignal(true);
    }
    else
    {
        emit assign1_statusSignal(false);
    };
}
