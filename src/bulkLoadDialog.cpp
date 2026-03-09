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

#include <QtWidgets>
#include <QFile>
#include <QDataStream>
#include <QByteArray>
#include <QMessageBox>
#include <QDebug>
#include "bulkLoadDialog.h"
#include "Preferences.h"
#include "globalVariables.h"

#include <iostream>
#include <cstdlib>

// Platform-dependent sleep routines.
#ifdef Q_OS_WIN
#include <windows.h>
#define SLEEP( milliseconds ) Sleep( (DWORD) milliseconds )
#else // Unix variants & Mac
#include <unistd.h>
#define SLEEP( milliseconds ) usleep( (unsigned long) (milliseconds * 1000.0) )
#endif


bulkLoadDialog::bulkLoadDialog()
{ 
    Preferences *preferences = Preferences::Instance();
    bool ok;
    const double ratio = preferences->getPreferences("Window", "Font", "ratio").toDouble(&ok);

    failed = true;
    QLabel *startListLabel = new QLabel(tr("Starting from"));
    this->startPatchCombo = new QComboBox(this);
    startPatchCombo->setMaxVisibleItems(200);
    QLabel *finishListLabel = new QLabel(tr("Finishing at"));
    this->finishPatchCombo = new QComboBox(this);
    finishPatchCombo->setMaxVisibleItems(200);
    QObject::connect(startPatchCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(comboValueChanged(int)));
    QObject::connect(finishPatchCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(comboValueChanged(int)));
    QVBoxLayout *comboBoxLayout = new QVBoxLayout;
    comboBoxLayout->addWidget(startListLabel);
    comboBoxLayout->addWidget(startPatchCombo);
    comboBoxLayout->addStretch(1);
    comboBoxLayout->addSpacing(12*ratio);
    comboBoxLayout->addWidget(finishListLabel);
    comboBoxLayout->addWidget(finishPatchCombo);

    QGroupBox *patchListGroup = new QGroupBox(tr("Set the Range of Patch data to restore"));
    patchListGroup->setLayout(comboBoxLayout);

    QLabel *startRangeLabel = new QLabel(tr("Start Bank."));
    QLabel *finishRangeLabel = new QLabel(tr("Finish Bank."));

    this->startRangeComboBox = new QComboBox(this);
    startRangeComboBox->setMaxVisibleItems(200);
    QObject::connect(startRangeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(comboValueChanged(int)));
    this->finishRange = new QLineEdit(this);
    this->finishRange->setReadOnly(true);

    QVBoxLayout *rangeLabelLayout = new QVBoxLayout;
    rangeLabelLayout->addSpacing(12*ratio);
    rangeLabelLayout->addWidget(startRangeLabel);
    rangeLabelLayout->addSpacing(12*ratio);
    rangeLabelLayout->addWidget(finishRangeLabel);

    QHBoxLayout *spinBoxLayout = new QHBoxLayout;
    spinBoxLayout->addWidget(startRangeComboBox);
    
    QVBoxLayout *rangeBoxLayout = new QVBoxLayout;
    rangeBoxLayout->addSpacing(12*ratio);
    rangeBoxLayout->addLayout(spinBoxLayout);
    rangeBoxLayout->addSpacing(12*ratio);
    rangeBoxLayout->addWidget(finishRange);

    QHBoxLayout *dataRangeLayout = new QHBoxLayout;
    dataRangeLayout->addSpacing(20*ratio);
    dataRangeLayout->addLayout(rangeLabelLayout);
    dataRangeLayout->addLayout(rangeBoxLayout);
    dataRangeLayout->addSpacing(20*ratio);

    QGroupBox *patchRangeGroup = new QGroupBox(tr("Set start location of Patch data to restore"));
    patchRangeGroup->setLayout(dataRangeLayout);

    this->startButton = new QPushButton(this);
    this->startButton->setText(tr("Start"));
    connect(startButton, SIGNAL(clicked()), this, SLOT(sendData()));

    this->completedButton = new QPushButton(this);
    this->completedButton->setText(tr("DATA TRANSFER COMPLETED"));
    this->completedButton->hide();
    connect(completedButton, SIGNAL(clicked()), this, SLOT(DialogClose()));

    this->cancelButton = new QPushButton(this);
    this->cancelButton->setText(tr("Cancel"));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(DialogClose()));

    this->progressLabel = new QLabel(this);
    this->progressLabel->setText(tr("Full Restoration may take up to 25 minutes"));
    this->bytesLabel = new QLabel(this);
    this->bytesLabel->setText("");

    this->progressBar = new QProgressBar(this);
    this->progressBar->setTextVisible(false);
    this->progressBar->setRange(0, 100);
    this->progressBar->setValue(0);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addSpacing(40*ratio);
    buttonsLayout->addWidget(startButton);
    buttonsLayout->addStretch(1);
    buttonsLayout->addSpacing(20*ratio);
    buttonsLayout->addWidget(cancelButton);
    buttonsLayout->addSpacing(40*ratio);

    QHBoxLayout *progressBarLayout = new QHBoxLayout;
    progressBarLayout->addWidget(progressBar);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(patchListGroup, Qt::AlignCenter);
    mainLayout->addStretch(1);
    mainLayout->addSpacing(12*ratio);
    mainLayout->addLayout(buttonsLayout, Qt::AlignCenter);
    mainLayout->addStretch(1);
    mainLayout->addSpacing(12*ratio);
    mainLayout->addWidget(patchRangeGroup, Qt::AlignCenter);
    mainLayout->addStretch(1);
    mainLayout->addSpacing(12*ratio);
    mainLayout->addWidget(progressLabel, Qt::AlignCenter);

    mainLayout->addWidget(bytesLabel, Qt::AlignCenter);
    mainLayout->addStretch(1);
    mainLayout->addSpacing(12*ratio);
    mainLayout->addWidget(completedButton);
    mainLayout->addStretch(1);
    mainLayout->addSpacing(12*ratio);
    mainLayout->addLayout(progressBarLayout, Qt::AlignCenter);
    setLayout(mainLayout);
    QString mode(tr("Guitar Mode"));
    QString filetype("*_G.syx");
    this->patch_size = patchReplySize;
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){mode = (tr("Bass Mode")); filetype = "*_B.syx"; patch_size = patchReplySizeBass;};
    setWindowTitle(mode + tr(" Bulk File Restoration"));
    this->bulk.clear();

    SysxIO *sysxIO = SysxIO::Instance();
    QObject::connect(this, SIGNAL(setStatusSymbol(int)), sysxIO, SIGNAL(setStatusSymbol(int)));
    QObject::connect(this, SIGNAL(setStatusProgress(int)), sysxIO, SIGNAL(setStatusProgress(int)));
    QObject::connect(this, SIGNAL(setStatusMessage(QString)), sysxIO, SIGNAL(setStatusMessage(QString)));
    QObject::connect(this, SIGNAL(setdeBugStatusMessage(QString)), sysxIO, SIGNAL(setStatusdBugMessage(QString)));

    QString dir = preferences->getPreferences("General", "Files", "dir");

    QString fileName = QFileDialog::getOpenFileName(
                this,
                tr("Choose a file"),
                dir,
                tr("Backup Patch File (")+filetype+")");
    if (!fileName.isEmpty())
    {

        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly))
        {
            this->data = file.readAll();     // read the pre-selected file, copy to 'data'

            if(this->data.size() == patchSize){patch_size = patchSize;};
            if(this->data.size() == patchSizeBass){patch_size = patchSizeBass;};

            QByteArray default_header = QByteArrayLiteral("\xF0\x41\x10\x00\x00\x00\x69");
            QByteArray file_header = data.mid(0, 7);                      // copy header from read file.syx
            uchar r = char (data[8]);     // find patch number in file (msb))
            bool ok;
            int patchNum;
            patchNum = QString::number(r, 16).toUpper().toInt(&ok, 16);
            bool isPatch = false;
            if (patchNum >= 16) { isPatch = true; };    // check the sysx file is a valid patch & not system data.

            bool isPatchSize = false;
            if(this->data.mid((patch_size)-1, 1) == QByteArrayLiteral("\xF7")){isPatchSize = true;};

            bool isHeader = false;
            if (default_header == file_header) {isHeader = true;};
            failed = false;
            if (isHeader == true && isPatch == true && isPatchSize == true)
            {
                loadSYX();
                emit setdeBugStatusMessage("Bulk load file is a *.syx compatible file type");
                std::string st = QString("Bulk Load file is a compatible *.syx file type").toStdString();
                std::cerr << st << std::endl;
            }
            else
            {
                failed = true;
                QMessageBox *msgBox = new QMessageBox();
                msgBox->setWindowTitle(QObject::tr("File Restore Error"));
                msgBox->setIcon(QMessageBox::Warning);
                msgBox->setTextFormat(Qt::RichText);
                QString msgText;
                msgText.append("<font size='+1'><b>");
                msgText.append(QObject::tr("This is not a known ") + deviceType + QObject::tr(" file!"));
                msgText.append("<b></font><br>");
                msgText.append(QObject::tr("Patch file not within allowable parameters or<br>"));
                msgText.append(QObject::tr("file format unknown, please try another file."));
                msgBox->setText(msgText);
                msgBox->setWindowFlags(Qt::WindowStaysOnTopHint);
                msgBox->raise();
                msgBox->exec();
                QTimer::singleShot(5000, msgBox, SLOT(deleteLater()));
                this->DialogClose();
            };
        };
    };
    if (failed == true)
    {
        this->startButton->hide();
    };
}

bulkLoadDialog::~bulkLoadDialog()
{
}

void bulkLoadDialog::comboValueChanged(int value)
{
    Q_UNUSED(value);
    this->bankStart = this->startRangeComboBox->currentIndex();
    this->startList = this->startPatchCombo->currentIndex();
    this->finishList = this->finishPatchCombo->currentIndex();
    if ((this->finishList-this->startList)>(this->bankStart+(bankTotalUser*patchPerBank)))
    {this->startPatchCombo->setCurrentIndex(this->finishList-(bankTotalUser*patchPerBank)+1); };
    if (this->startList > this->finishList) {this->startPatchCombo->setCurrentIndex(finishList); }
    else if (this->finishList < this->startList) {this->finishPatchCombo->setCurrentIndex(startList); };
    int x = (this->bankStart+(this->finishList-this->startList));
    if (x<0) {x=0; } else if (x>((bankTotalUser*patchPerBank)-1))
    {
        x=((bankTotalUser*patchPerBank)-1);
        this->bankStart=((bankTotalUser*patchPerBank)-1)-(this->finishList-this->startList);
        this->startRangeComboBox->setCurrentIndex(((bankTotalUser*patchPerBank)-1)-(this->finishList-this->startList));
    };
    QString text = tr("Finish at U");
    int y = x/patchPerBank; y = y*patchPerBank; y=x-y;
    text.append(QString::number((x/patchPerBank)+1, 10).toUpper() );
    text.append("-");
    text.append(QString::number(y+1, 10).toUpper() );
    this->finishRange->setText(text);
}

void bulkLoadDialog::sendData()
{	int addr_change =5130;  // patch file bytes before hex2 address reaches "7F"
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){addr_change = 5063;};
    if(this->data.size()==patchSize){addr_change = 5236;};              // if a formated single syx guitar mode patch
    if(this->data.size()==patchSizeBass){addr_change = 5173;};          // if a formated single syx bass mode patch
    bankStart = this->startRangeComboBox->currentIndex()+1;
    startList = this->startPatchCombo->currentIndex();
    finishList = this->finishPatchCombo->currentIndex();
    startButton->hide();
    progress = 0;
    patch = 1;
    range = (finishList-startList)+1;
    this->progressBar->setRange(0, range);

    int z = (bankStart*2)-1;                                            // z ranges from 0~399.
    QString address;
    QString msg;
    QString v;
    QString addrMSB = "20";                                             // guitar mode range 20~23, bass mode range 28~2B.
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){addrMSB = "28";};
    QString replyMsg;
    for (int a=startList;a<(finishList+1);a++)
    {
        int y = 1;
        if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){
            if (z>384 && addrMSB == "28") {z=z-384; addrMSB = "2B"; }
            if (z>256 && addrMSB == "28") {z=z-256; addrMSB = "2A"; }
            else if (z>128 && addrMSB == "28") {z=z-128; addrMSB = "29"; }  // bass mode next address range when > 28 7F.
            else if (z>128 && addrMSB == "29") {z=z-128; addrMSB = "2A"; }
            else if (z>128) {z=z-128; addrMSB = "2B"; };
        }else{
            if (z>384 && addrMSB == "20") {z=z-384; addrMSB = "23"; }
            if (z>256 && addrMSB == "20") {z=z-256; addrMSB = "22"; }
            else if (z>128 && addrMSB == "20") {z=z-128; addrMSB = "21"; }  // guitar mode next address range when > 20 7F.
            else if (z>128 && addrMSB == "21") {z=z-128; addrMSB = "22"; }
            else if (z>128) {z=z-128; addrMSB = "23"; };
        };
        address = QString::number(z-y, 16).toUpper();
        if (address.size()<2){ address.prepend("0"); };
        int b = a*patch_size;                                               // multiples of patch size.
        msg = this->bulk.mid(b*2, patch_size*2);                            // copy next patch from bulk patch list.
        for (int g=0;g<msg.size()/2;g++)
        {
            if(g==addr_change)
            {
                y=0;
                address = QString::number(z-y, 16).toUpper();
                if (address.size()<2){ address.prepend("0"); };
            };                                              // address increments if hex2 > 7F
            v = msg.mid(g*2, 2);
            if (v == "F0") {msg.replace((g*2)+(sysxAddressOffset*2), 2, addrMSB); msg.replace((g*2)+((sysxAddressOffset*2)+2), 2, address); };   // replace the message address
        };
        replyMsg.append(msg);
        z=z+2;
    };

    QString reBuild;                                                      // Add correct checksum to patch strings
    QString sysxEOF;
    QString hex;
    int msgLength = replyMsg.length()/2;
    for(int i=0;i<msgLength*2;++i)
    {
        hex.append(replyMsg.mid(i*2, 2));
        sysxEOF = (replyMsg.mid((i*2)+4, 2));
        if (sysxEOF == "F7")
        {
            int dataSize = 0; bool ok;
            for(int h=checksumOffset;h<hex.size()-1;++h)
            { dataSize += hex.mid(h*2, 2).toInt(&ok, 16); };
            QString base = "80";                                          // checksum calculate.
            unsigned int sum = dataSize % base.toInt(&ok, 16);
            if(sum!=0) { sum = base.toInt(&ok, 16) - sum; };
            QString checksum = QString::number(sum, 16).toUpper();
            if(checksum.length()<2) {checksum.prepend("0");};
            hex.append(checksum);
            hex.append("F7");
            reBuild.append(hex);
            hex.clear();
            sysxEOF.clear();
            i=i+2;
        };
    };
    this->bulk = reBuild.toUpper();
    bank = (bankStart+(patchPerBank-1));
    patch = bank/patchPerBank; patch = patch*patchPerBank;
    patch=(bank-patch)+1;
    steps=0;
    dataSent=0;
    SysxIO *sysxIO = SysxIO::Instance();
    sysxIO->setMode("mode0");
    sendSequence();
}

void bulkLoadDialog::sendPatch(QString data)
{
    SysxIO *sysxIO = SysxIO::Instance();
    if(!sysxIO->isConnected())
    {
        qWarning("BULK_SEND_ABORT connection lost before sending chunk step=%d", steps);
        emit setStatusSymbol(0);
        emit setStatusMessage(tr("Bulk transfer aborted: Not connected"));
        this->progressLabel->setText(tr("Bulk transfer aborted: SY-1000 connection lost"));
        this->completedButton->show();
        return;
    }

    /* BUG FIX (contributed): Without Qt::UniqueConnection, each call to sendPatch() adds
     * another sysxReply→sendSequence connection. Over a 200-patch bulk transfer this
     * accumulates 200 duplicate connections, causing sendSequence() to be called 200 times
     * per signal emission — exponential invocations, stack overflow, and hang.
     * Qt::UniqueConnection ensures only one connection exists at any time. */
    QObject::connect(sysxIO, SIGNAL(sysxReply(QString)), this, SLOT(sendSequence()), Qt::UniqueConnection);
    int count=100;
    while(!sysxIO->deviceReady() && count>0)
    {
        SLEEP(20);
        --count;
    };
    sysxIO->sysxOutput(data, 0);
}

void bulkLoadDialog::sendSequence()
{ 
    SysxIO *sysxIO = SysxIO::Instance();
    QObject::disconnect(sysxIO, SIGNAL(sysxReply(QString)), this, SLOT(sendSequence()));
    sysxIO->setDeviceReady(true);                                      // Free the device after finishing interaction.
    if(!sysxIO->isConnected())
    {
        qWarning("BULK_SEQUENCE_ABORT connection lost while waiting for reply step=%d", steps);
        emit setStatusSymbol(0);
        emit setStatusMessage(tr("Bulk transfer aborted: Not connected"));
        this->progressLabel->setText(tr("Bulk transfer aborted: SY-1000 connection lost"));
        this->completedButton->show();
        return;
    }
    msg = bulk.mid(steps*(patch_size*2), (patch_size*2));
    if(msg.isEmpty() && steps<((finishList-startList)+1))
    {
        qWarning("BULK_SEQUENCE_ABORT empty patch chunk step=%d", steps);
        emit setStatusSymbol(0);
        emit setStatusMessage(tr("Bulk transfer aborted: Invalid patch data"));
        this->progressLabel->setText(tr("Bulk transfer aborted: Invalid patch data chunk"));
        this->completedButton->show();
        return;
    }

    progress++;
    bulkStatusProgress(this->progress);                                // advance the progressbar.

    if (steps<((finishList-startList)+1) )
    {
        bool ok;
        QString patchText;
        QString name = msg.mid((sysxNameOffset*2), (nameLength*2));    // get name from loaded patch.
        QList<QString> x;
        for (int b=0;b<nameLength;b++)
        {
            x.append(name.mid(b*2, 2));
        };
        for (int b=0;b<nameLength;b++)
        {
            QString hexStr = x.at(b);
            patchText.append( char(hexStr.toInt(&ok, 16)) );           // convert name to readable text characters.
        };
        int bf = (finishList-startList);
        if(steps>bf) {this->completedButton->show();
            this->progressLabel->setText(tr("Bulk data transfer completed!!"));
            this->progress=range;  };
        QString patchNumber = QString::number(bank/patchPerBank, 10).toUpper();
        if(patchNumber.size()<2){patchNumber.prepend("0");};
        patchNumber.prepend(tr("User Patch U" ));
        patchNumber.append("-");
        patchNumber.append( QString::number(patch, 10).toUpper());
        patchNumber.append("     ");
        patchNumber.append(patchText);
        patchText=patchNumber;
        this->progressLabel->setText(patchText);                        //display the patch number and name.

        patchNumber = tr("File transfer size = ");
        dataSent = dataSent+(msg.size()/2);
        patchNumber.append(QString::number(dataSent, 10).toUpper() );
        patchNumber.append(tr(" bytes"));
        this->bytesLabel->setText(patchNumber);                         //display the bulk data size.
        ++steps;
        ++patch;
        if(patch>patchPerBank) {patch=1; bank=bank+patchPerBank;};	  // increment patch.
        sendPatch(msg);                                 //request the next patch.
        emit setStatusMessage(tr("Sending Data"));
    } else {
        QObject::disconnect(sysxIO, SIGNAL(sysxReply(QString)), this, SLOT(sendSequence()));
        sysxIO->setDeviceReady(true); // Free the device after finishing interaction.
        DialogClose();
    };
}

void bulkLoadDialog::updatePatch()
{
    patchCount = sysxPatches.size()/(patch_size);
    QString msgText;
    QString patchText;
    QString patchNumber;
    char r;
    this->patchList.clear();
    uint a = sysxNameOffset;             // locate patch text start position from the start of the file
    for (int h=0;h<patchCount;h++)
    {
        for (int b=0;b<nameLength;b++)
        {
            r = uchar(sysxPatches[a+b]);
            patchText.append(r);
        };
        patchNumber = QString::number(h+1, 10).toUpper();
        if(patchNumber.size()<2){patchNumber.prepend("0");};
        msgText.append(patchNumber + " : ");
        msgText.append(patchText + "   ");
        this->patchList.append(msgText);
        patchText.clear();
        msgText.clear();
        a=a+patch_size;                                // advance to the next patch in the bulk file.
    };
    this->startPatchCombo->addItems(patchList);            // add patch names to the combobox lists.
    this->finishPatchCombo->addItems(patchList);
    this->finishPatchCombo->setCurrentIndex(patchCount-1);     // set the finish combobox index to the end of the list.
    this->startPatchCombo->setCurrentIndex(0);
    QString text = tr("Finish at U");
    if (patchCount<patchPerBank) {patchCount=patchPerBank; };
    QString bnum(QString::number(patchCount/patchPerBank, 10).toUpper());
    if(bnum.size()<2){bnum.prepend("0");};
    text.append(bnum);
    text.append("-x");
    this->finishRange->setText(text);
    QString bnk;

    for (int x=0; x<bankTotalUser; x++)
    {
        QString bnum(QString::number(x+1, 10).toUpper());
        if(bnum.size()<2){bnum.prepend("0");};
        bnk = "U" + bnum + "-1";
        this->startRangeComboBox->addItem(bnk);
        bnk = "U" + bnum + "-2";
        this->startRangeComboBox->addItem(bnk);
        bnk = "U" + bnum + "-3";
        this->startRangeComboBox->addItem(bnk);
        bnk = "U" + bnum + "-4";
        this->startRangeComboBox->addItem(bnk);
    };
    this->startRangeComboBox->setCurrentIndex(0);

    QString sysxBuffer;
    for(int i=0;i<sysxPatches.size();i++)
    {
        unsigned char byte = (char)sysxPatches[i];
        unsigned int n = (int)byte;
        QString hex = QString::number(n, 16).toUpper();     // convert QByteArray to QString
        if (hex.length() < 2) hex.prepend("0");
        sysxBuffer.append(hex);
    };
    bulk.append(sysxBuffer);
}

void bulkLoadDialog::bulkStatusProgress(int value)
{
    this->progressBar->setValue(value);
}

void bulkLoadDialog::loadG5L()         // ************************************ G5L File Format***************************
{	

}

void bulkLoadDialog::loadSYX()        //********************************* SYX File Format *****************************
{	
    sysxPatches = data;
    updatePatch();
}

void bulkLoadDialog::loadSMF()    // **************************** SMF FILE FORMAT ***************************
{	

}

void bulkLoadDialog::DialogClose()
{
    steps = 300;
    SysxIO *sysxIO = SysxIO::Instance();
    QObject::disconnect(sysxIO, SIGNAL(sysxReply(QString)), this, SLOT(sendSequence()));
    sysxIO->setDeviceReady(true); // Free the device after finishing interaction.
    sysxIO->setMode("mode1");
    this->deleteLater();
    close();
}
