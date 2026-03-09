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

#include <QtWidgets>
#include <QFile>
#include <QDataStream>
#include <QByteArray>
#include <QTimer>
#include "patchWriteDialog.h"
#include "Preferences.h"
#include "globalVariables.h"
#include "SysxIO.h"

patchWriteDialog::patchWriteDialog() : QWidget()
{ 
    Preferences *preferences = Preferences::Instance();
    bool ok;
    double ratio = preferences->getPreferences("Window", "Scale", "ratio").toDouble(&ok);
    double fratio = preferences->getPreferences("Window", "Font", "ratio").toDouble(&ok);
    QFont Sfont( "Arial", 12*fratio, QFont::Normal);
    this->setFont(Sfont);

    SysxIO *sysxIO = SysxIO::Instance();
    QLabel *patchListLabel = new QLabel(tr("Patch Write destination"));
    this->patchListCombo = new QComboBox(this);
    this->patchListCombo->setCurrentIndex(1);

    QVBoxLayout *comboBoxLayout = new QVBoxLayout;
    comboBoxLayout->addWidget(patchListLabel);
    comboBoxLayout->addWidget(patchListCombo);
    comboBoxLayout->addStretch(1);
    comboBoxLayout->addSpacing(12*ratio);

    QString patchName = sysxIO->getCurrentPatchName();
    patchListGroup = new QGroupBox("Patch name: "+ patchName);
    patchListGroup->setLayout(comboBoxLayout);

    this->writeButton = new QPushButton(this);
    this->writeButton->setText(tr("WRITE TO SY-1000"));
    connect(writeButton, SIGNAL(clicked()), this, SLOT(writeToMemory()));

    this->save2FileButton = new QPushButton(this);
    this->save2FileButton->setText(tr("QUICK SAVE TO FILE"));
    connect(save2FileButton, SIGNAL(clicked()), this, SLOT(saveToFile()));

    this->cancelButton = new QPushButton(this);
    this->cancelButton->setText(tr("CANCEL/CLOSE"));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(close()));

    this->progressLabel = new QLabel(this);
    this->progressLabel->setText(tr("WARNING!!: Existing Patch data will be over-written"));
    this->bytesLabel = new QLabel(this);
    this->bytesLabel->setText("");

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addSpacing(40*ratio);
    buttonsLayout->addWidget(writeButton);
    buttonsLayout->addStretch(1);
    buttonsLayout->addSpacing(20*ratio);
    buttonsLayout->addWidget(cancelButton);
    buttonsLayout->addSpacing(40*ratio);

    QHBoxLayout *progressBarLayout = new QHBoxLayout;

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(patchListGroup, Qt::AlignCenter);
    mainLayout->addStretch(1);
    mainLayout->addSpacing(12*ratio);
    mainLayout->addLayout(buttonsLayout, Qt::AlignCenter);
    mainLayout->addStretch(1);
    mainLayout->addSpacing(12*ratio);
    mainLayout->addWidget(progressLabel, Qt::AlignCenter);

    mainLayout->addWidget(bytesLabel, Qt::AlignCenter);
    mainLayout->addStretch(1);
    mainLayout->addSpacing(12*ratio);
    mainLayout->addWidget(save2FileButton);
    mainLayout->addStretch(1);
    mainLayout->addSpacing(12*ratio);
    mainLayout->addLayout(progressBarLayout, Qt::AlignCenter);
    setLayout(mainLayout);

    setWindowTitle(tr("SY-1000 CURRENT PATCH WRITE/SAVE"));

    for(int z=0; z<200; z++)
    {
        QString patch = QString::number(z+1);
        this->patchList.append("[U:"+patch+"] ");
        QString text;
        text.append("[U_"+patch+"] ");
        this->patchListCombo->addItem(text);
    };
    this->bank = sysxIO->getBank()-1;
    this->patch = sysxIO->getPatch()-1;
    patchListCombo->view()->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    QObject::connect(patchListCombo, SIGNAL(activated(int)), this, SLOT(comboValueChanged(int)));
    QObject::connect(this, SIGNAL(updateTreeSignal()), sysxIO, SIGNAL(updateTreeSignal()));
    QObject::connect(this, SIGNAL(updateNameSignal(QString, int, int)), sysxIO, SIGNAL(updateNameSignal(QString, int, int)));
}

patchWriteDialog::~patchWriteDialog()
{
    this->deleteLater();
}

void patchWriteDialog::listUpdate()
{
    SysxIO *sysxIO = SysxIO::Instance();
    patchListCombo->clear();
    this->bank = sysxIO->getBank()-1;
    this->patch = sysxIO->getPatch()-1;
    int next = 0;
    int pth=1;
    for(int z=0; z<200; z++)
    {
        QString patch = QString::number((z/4)+1);
        if(patch.size()<2){patch.prepend("0");};
        patch.append("-"+QString::number(pth));
        pth++;
        if(pth>4){pth = 1;};

        this->patchList.append("[U:"+patch+"] ");
        this->patchList.append(sysxIO->namesList.mid(next, 16));       // Set the patch name of the item in the list.
        QString text;
        text.append("[U_"+patch+"] ");
        text.append(sysxIO->namesList.mid(next, 16));       // Set the patch name of the item in the list.
        next = next + 16;
        this->patchListCombo->addItem(text);
    };

    patchListCombo->view()->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    patchListCombo->setCurrentIndex((this->bank*4)+this->patch);
    int pnum = (this->bank*4)+patch+1;
    if (pnum > 200){patchListCombo->setCurrentIndex(0);};

    QString patchName = sysxIO->getCurrentPatchName();
    patchListGroup->setTitle("Patch name: "+ patchName);
}

void patchWriteDialog::comboValueChanged(int value)
{
    this->patchListCombo->setCurrentIndex(value);
    this->bank = value/4;
    this->patch = value-((value/4)*4);
}

void patchWriteDialog::getTempPatch()
{
    SysxIO *sysxIO = SysxIO::Instance();
    QObject::disconnect(sysxIO, SIGNAL(isChanged()),
                        this, SLOT(requestPatch()));

    QObject::connect(sysxIO, SIGNAL(sysxReply(QString)),	// Connect the result of the request
                     this, SLOT(updatePatch(QString)));					// to updatePatch function.

    emit setStatusSymbol(3);
    emit setStatusMessage(tr("Receiving Patch"));
    sysxIO->requestPatch(0, 0);
}

void patchWriteDialog::saveToFile()
{ 
    SysxIO *sysxIO = SysxIO::Instance();
    Preferences *preferences = Preferences::Instance();
    QString dir = preferences->getPreferences("General", "Files", "dir");

    QString fileName = sysxIO->getCurrentPatchName();
    if (!fileName.isEmpty())
    {
        if(!fileName.contains(".tsl"))
        {
            if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
            {fileName.append("_B.tsl");}
            else{fileName.append("_G.tsl");};
        };
        //Preferences *preferences = Preferences::Instance();
        if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){
            file.writeTSL_B("./saved_patches/quick_saved/"+fileName);}else{
            file.writeTSL_G("./saved_patches/quick_saved/"+fileName);};

        file.setFile(fileName);
        if(file.readFile())
        {
            // DO SOMETHING AFTER READING THE FILE (UPDATE THE GUI)
            SysxIO *sysxIO = SysxIO::Instance();
            sysxIO->setFileSource("Structure", file.getFileSource());
            sysxIO->relayUpdateSignal();
        };
    };
    close();
}

void patchWriteDialog::writeToMemory()
{
    SysxIO *sysxIO = SysxIO::Instance();
    if(!sysxIO->isConnected())
    {
        emit setStatusSymbol(0);
        emit setStatusMessage(tr("Not connected"));
        QMessageBox *msgBox = new QMessageBox(this);
        msgBox->setWindowTitle(deviceType + tr(" not connected !!"));
        msgBox->setIcon(QMessageBox::Information);
        msgBox->setText(tr("Cannot write patch to SY-1000 without a verified connection."));
        msgBox->show();
        QTimer::singleShot(3000, msgBox, SLOT(deleteLater()));
        return;
    }

    int pnum = (this->bank*4)+patch+1;
    if (pnum < 201)
    {
        QString sysxMsg;
        QList< QList<QString> > patchData = sysxIO->getFileSource().hex; // Get the loaded patch data.
        emit setStatusSymbol(2);
        emit setStatusMessage(tr("Writing to Patch"));

        QString addr1;
        QString addr2;
        QString addr3;
        int range = 5236; // point in sys file when address range increases.
        Preferences *preferences = Preferences::Instance();
        if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){range = 5173;};

        if (bank < 51)
        {
            int patchOffset = ((((bank ) * patchPerBank) + patch))*2;
            int memmorySize = 128;
            int emptyAddresses = (memmorySize) - ((bankTotalUser * patchPerBank) - (memmorySize));
            if(bank > bankTotalUser)
            {
                patchOffset += emptyAddresses;    //System patches start at a new memmory range.
            };
            int addrMaxSize = 128;
            int n = int(patchOffset / addrMaxSize);
            int addrOffset = 32;
            Preferences *preferences = Preferences::Instance();
            if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
            {
                addrOffset = 40;
            };
            addr1 = QString::number(addrOffset + n, 16).toUpper();
            addr2 = QString::number(patchOffset - (addrMaxSize * n), 16).toUpper();
            addr3 = QString::number((patchOffset - (addrMaxSize * n))+1, 16).toUpper();
        };
        if (addr1.length() < 2) addr1.prepend("0");
        if (addr2.length() < 2) addr2.prepend("0");
        if (addr3.length() < 2) addr3.prepend("0");

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
                if(sysxMsg.size()/2 > range)
                {
                    addr2 = addr3;
                };
            };
        };
        sysxIO->setSyncStatus(true);		// Still in sync

        QObject::connect(sysxIO, SIGNAL(sysxReply(QString)), this, SLOT(close()));

        sysxIO->setBank(this->bank+1);
        sysxIO->setPatch(this->patch+1);
        emit updateNameSignal(sysxIO->getCurrentPatchName(), this->bank+1, this->patch+1);
        emit updateTreeSignal();
        sysxIO->sysxOutput(sysxMsg, 0);
    }
    else
    {
        patchListCombo->setCurrentIndex(0);
    };
}

void patchWriteDialog::close()
{
    this->setCursor(Qt::ArrowCursor);
    SysxIO *sysxIO = SysxIO::Instance();
    QObject::disconnect(sysxIO, SIGNAL(sysxReply(QString)), this, SLOT(close()));
    sysxIO->setDeviceReady(true); // Free the device after finishing interaction.
    this->lower();
    this->unsetCursor();
    this->hide();
}




