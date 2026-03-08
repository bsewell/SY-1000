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
#include "bulkSaveDialog.h"
#include "Preferences.h"
#include "globalVariables.h"


bulkSaveDialog::bulkSaveDialog()
{ 
    SysxIO *sysxIO = SysxIO::Instance();
    Preferences *preferences = Preferences::Instance();
    bool ok;
    const double ratio = preferences->getPreferences("Window", "Font", "ratio").toDouble(&ok);

    QLabel *startRangeLabel = new QLabel(tr("Starting Patch."));
    QLabel *finishRangeLabel = new QLabel(tr("Finishing Patch."));

    this->g5lButton = new QRadioButton(tr("*.tsl - Tone Studio compatable file - (TSL)"), this );
    this->g5lButton->resize(20*ratio, 20*ratio);
    this->syxButton = new QRadioButton(tr("*.syx - System Exclusive file - (SYX)"), this );
    this->syxButton->resize(20*ratio, 20*ratio);
    //this->midButton = new QRadioButton(tr("*.mid - Standard Midi file - (SMF)"), this );
    this->g5lButton->setChecked(true);
    
    QCheckBox *systemCheckBox = new QCheckBox(tr("Save System Data"));
    this->startRangeComboBox = new QComboBox(this);
    this->finishRangeComboBox = new QComboBox(this);
    //systemCheckBox->resize(20*ratio, 20*ratio);
    systemCheckBox->setMinimumSize(20*ratio, 20*ratio);
    this->systemCheckBox = systemCheckBox;

    for(int x=0; x<200; x++)
    {
        this->startRangeComboBox->addItem("Patch "+QString::number( x+1 ,10)+"    USER: "+QString::number((x/4)+1 ,10)+"-"+QString::number((x%4)+1 ,10)+"  "+sysxIO->namesList.mid(x*16, 16));
        this->finishRangeComboBox->addItem("Patch "+QString::number( x+1 ,10)+"    USER: "+QString::number((x/4)+1 ,10)+"-"+QString::number((x%4)+1 ,10)+"  "+sysxIO->namesList.mid(x*16, 16));
    };
    this->finishRangeComboBox->setCurrentIndex(199);
    this->startRangeComboBox->view()->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    this->finishRangeComboBox->view()->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    QVBoxLayout *rangeLabelLayout = new QVBoxLayout;
    rangeLabelLayout->addSpacing(12*ratio);
    rangeLabelLayout->addWidget(startRangeLabel);
    rangeLabelLayout->addWidget(finishRangeLabel);

    QVBoxLayout *rangeBoxLayout = new QVBoxLayout;
    //rangeBoxLayout->addWidget(systemCheckBox);
    //rangeBoxLayout->addSpacing(12*ratio);
    rangeBoxLayout->addWidget(startRangeComboBox);
    rangeBoxLayout->addSpacing(12*ratio);
    rangeBoxLayout->addWidget(finishRangeComboBox);

    QHBoxLayout *dataRangeLayout = new QHBoxLayout;
    dataRangeLayout->addSpacing(20*ratio);
    dataRangeLayout->addLayout(rangeLabelLayout);
    dataRangeLayout->addSpacing(12*ratio);
    dataRangeLayout->addLayout(rangeBoxLayout);
    dataRangeLayout->addSpacing(20*ratio);

    QGroupBox *patchRangeGroup = new QGroupBox(tr("Set range of Patch data to save"));
    patchRangeGroup->setLayout(dataRangeLayout);

    this->startButton = new QPushButton(this);
    this->startButton->setText(tr("Start"));
    connect(startButton, SIGNAL(clicked()), this, SLOT(backup()));

    this->completedButton = new QPushButton(this);
    this->completedButton->setText(tr("DATA TRANSFER COMPLETED"));
    this->completedButton->hide();
    connect(completedButton, SIGNAL(clicked()), this, SLOT(DialogClose()));

    this->cancelButton = new QPushButton(this);
    this->cancelButton->setText(tr("Cancel"));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(DialogClose()));

    this->progressLabel = new QLabel(this);
    this->progressLabel->setText(tr("Full Backup may take up to 25 minutes"));
    this->bytesLabel = new QLabel(this);
    this->bytesLabel->setText("");

    this->progressBar = new QProgressBar(this);
    this->progressBar->setTextVisible(false);
    this->progressBar->setRange(0, 100);
    this->progressBar->setValue(0);

    QVBoxLayout *fileFormatLayout = new QVBoxLayout;
    fileFormatLayout->addStretch(1);
    fileFormatLayout->addSpacing(6*ratio);
    //fileFormatLayout->addWidget(g5lButton);
    //fileFormatLayout->addStretch(1);
    //fileFormatLayout->addSpacing(6*ratio);
    fileFormatLayout->addWidget(syxButton);
    fileFormatLayout->addStretch(1);
    fileFormatLayout->addSpacing(6*ratio);
    //fileFormatLayout->addWidget(midButton);
    //fileFormatLayout->addSpacing(6);
    QGroupBox *fileFormatGroup = new QGroupBox(tr("Select File format to save"));
    fileFormatGroup->setLayout(fileFormatLayout);

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
    mainLayout->addWidget(fileFormatGroup);
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

    setWindowTitle(tr("Bulk File Backup"));

    QObject::connect(this->startRangeComboBox, SIGNAL( currentIndexChanged(int) ), this, SLOT( comboChanged() ));
    QObject::connect(this->finishRangeComboBox, SIGNAL( currentIndexChanged(int) ), this, SLOT( comboChanged() ));

    QObject::connect(this, SIGNAL(setStatusSymbol(int)), sysxIO, SIGNAL(setStatusSymbol(int)));
    QObject::connect(this, SIGNAL(setStatusProgress(int)), sysxIO, SIGNAL(setStatusProgress(int)));
    QObject::connect(this, SIGNAL(setStatusMessage(QString)), sysxIO, SIGNAL(setStatusMessage(QString)));

}

bulkSaveDialog::~bulkSaveDialog()
{
}

void bulkSaveDialog::comboChanged()
{
    if(this->startRangeComboBox->currentIndex()>this->finishRangeComboBox->currentIndex())
    { this->finishRangeComboBox->setCurrentIndex(this->startRangeComboBox->currentIndex()); };

    if(this->finishRangeComboBox->currentIndex()<this->startRangeComboBox->currentIndex())
    { this->startRangeComboBox->setCurrentIndex(this->finishRangeComboBox->currentIndex()); };
}

void bulkSaveDialog::backup()
{
    this->startButton->hide();
    this->patch_start = this->startRangeComboBox->currentIndex();
    this->patch_finish = this->finishRangeComboBox->currentIndex();
    this->patch_range = this->patch_finish-this->patch_start+1;
    this->bank = (patch_start/4)+1;
    this->patch = (patch_start%4)+1;
    bulk.clear();
    this->progress = 0;
    this->patch_count = 0;
    range = 100/(this->patch_range/4);
    this->progressBar->setRange(0, this->patch_range);
    requestPatch(bank, patch);
}

void bulkSaveDialog::requestPatch(int bank, int patch) 
{	
    SysxIO *sysxIO = SysxIO::Instance();
    if(sysxIO->isConnected() && sysxIO->deviceReady() && bank<100 )
    {
        SysxIO *sysxIO = SysxIO::Instance();
        QObject::disconnect(sysxIO, SIGNAL(sysxReply(QString)), this, SLOT(updatePatch(QString)));
        QObject::connect(sysxIO, SIGNAL(sysxReply(QString)), this, SLOT(updatePatch(QString)));
        sysxIO->requestPatch(bank, patch);
    };
}

void bulkSaveDialog::updatePatch(QString replyMsg)
{
    SysxIO *sysxIO = SysxIO::Instance();
    QObject::disconnect(sysxIO, SIGNAL(sysxReply(QString)), this, SLOT(updatePatch(QString)));
    sysxIO->setDeviceReady(true); // Free the device after finishing interaction.

    if (replyMsg.size()/2 == patchReplySize || replyMsg.size()/2 == patchReplySizeBass)
    {
        bad_hits=0;
        bulk.append(replyMsg); 	                                           // add patch to the bulk string.
        ++this->patch_count;
    }
    else
    {
            QMessageBox *msgBox = new QMessageBox();
            msgBox->setWindowTitle(deviceType + tr(" FloorBoard"));
            msgBox->setIcon(QMessageBox::Critical);
            msgBox->setTextFormat(Qt::RichText);
            QString msgText;
            msgText.append("<font size='+1'><b>");
            msgText.append(tr("Received patch data is corrupted or incomplete"));
            msgText.append("<b></font><br>");
            msgText.append(tr("Retry events have failed to retrieve the correct data<br>"));
            msgText.append(tr ("Do you wish to save the patch data you already have anyway and continue?"));
            msgBox->setInformativeText(tr("Are you sure you want to continue?"));
            msgBox->setText(msgText);
            msgBox->setStandardButtons(QMessageBox::Yes | QMessageBox::No);

            if(msgBox->exec() == QMessageBox::No)
            {
                DialogClose();
            };
            msgBox->deleteLater();
    };
    ++patch;
    if(patch>4) {patch=1; bank=bank+1;};	                      // increment patch.
    progress++;
    bulkStatusProgress(this->progress);                         // advance the progressbar.
    int bf = (patch_finish+1) -2;
    if (bank >= bf)
    {                                                            // check if nearly finished.
        this->completedButton->show();
        this->progressLabel->setText(tr("Bulk data transfer completed!!"));
    };
    if (this->patch_count<this->patch_range)
    {
        bool ok;
        QString patchText;
        QString name = replyMsg.mid(24, 32);                       // get name from loaded patch.
        QList<QString> x;
        for (int b=0;b<16;b++)
        {
            x.append(name.mid(b*2, 2));
        };
        for (int b=0;b<16;b++)
        {
            QString hexStr = x.at(b);
            patchText.append( (char)(hexStr.toInt(&ok, 16)) );      // convert name to readable text characters.
        };
        
        QString patchNumber = QString::number(bank, 10).toUpper();
        if (patchNumber.size()<2) { patchNumber.prepend("0"); };
        patchNumber.prepend(tr("User Patch U"));
        patchNumber.append("-");
        patchNumber.append( QString::number(patch, 10).toUpper() );
        patchNumber.append("     ");
        patchNumber.append(patchText);
        patchText=patchNumber;
        this->progressLabel->setText(patchText);                        //display the patch number and name.

        patchNumber = tr("File build size = ");
        int size = (bulk.size()/2)+patchSize;
        patchNumber.append(QString::number(size, 10).toUpper() );
        patchNumber.append(tr(" bytes"));
        this->bytesLabel->setText(patchNumber);                         //display the bulk data size.

        emit setStatusMessage(tr("Bulk Download"));
        requestPatch(bank, patch);                                   //request the next patch.
    } else {
        sysxIO->bulk = this->bulk;
        //if (this->g5lButton->isChecked() ) { writeG5L(); };                      // format and write bulk patches.
        /*if (this->syxButton->isChecked() )*/ { writeSYX(); };
        //if (this->midButton->isChecked() ) { writeSMF(); };
    };
}

void bulkSaveDialog::bulkStatusProgress(int value)
{
    value=value/4;
    if (value >100) {value = 100;};
    if (value<0) {value = 0; };
    this->progressBar->setValue(progress);//value);
}

void bulkSaveDialog::writeG5L()         // ************************************ G5L File Format***************************
{	

    DialogClose();      // close the dialog page after the file has been saved or cancelled.
}

void bulkSaveDialog::writeSYX()        //********************************* SYX File Format *****************************
{	
    Preferences *preferences = Preferences::Instance();
    QString dir = preferences->getPreferences("General", "Files", "dir");

    QString fileName = QFileDialog::getSaveFileName(
                this,
                tr("Save Bulk Data"),
                dir,
                tr("System Exclusive Backup File (*.syx)"));
    if (!fileName.isEmpty())
    {
        if(!fileName.contains(".syx"))
        {
            if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){
            fileName.append("_B.syx");}else{fileName.append("_G.syx");};
        };

        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly))
        {
            bool ok;
            QByteArray out;
            int size = this->bulk.size()/2;

            for (int x=0;x<size*2;x++)
            {
                QString hexStr = bulk.mid(x, 2);
                out.append( (char)(hexStr.toInt(&ok, 16)) );
                x=x+1;
            };
            file.write(out);
        };
    };
    DialogClose();
}

void bulkSaveDialog::writeSMF()    // **************************** SMF FILE FORMAT ***************************
{	
        DialogClose();
}

void bulkSaveDialog::DialogClose()
{
    bank = 100;
    SysxIO *sysxIO = SysxIO::Instance();
    QObject::disconnect(sysxIO, SIGNAL(sysxReply(QString)), this, SLOT(updatePatch(QString)));
    this->deleteLater();
    this->close();
}
