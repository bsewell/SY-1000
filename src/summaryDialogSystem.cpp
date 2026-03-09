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
#include "summaryDialogSystem.h"
#include "Preferences.h"
#include "SysxIO.h"
#include "MidiTable.h"
#include "globalVariables.h"

#include <iostream>


summaryDialogSystem::summaryDialogSystem(QWidget *parent)
    : QWidget(parent)
{
    sys1 = "00";
    Preferences *preferences = Preferences::Instance();
    bool ok;
    const double ratio = preferences->getPreferences("Window", "Scale", "ratio").toDouble(&ok);

    SysxIO *sysxIO = SysxIO::Instance();
    QObject::connect(this, SIGNAL(setStatusSymbol(int)), sysxIO, SIGNAL(setStatusSymbol(int)));
    QObject::connect(this, SIGNAL(setStatusProgress(int)), sysxIO, SIGNAL(setStatusProgress(int)));
    QObject::connect(this, SIGNAL(setStatusMessage(QString)), sysxIO, SIGNAL(setStatusMessage(QString)));

    this->textDialog = new QTextEdit(parent);
    textDialog->setReadOnly(true);
    textDialog->setDocumentTitle(deviceType + tr(" Patch List"));
    textDialog->setMinimumHeight(700*ratio);
    
    if(sysxIO->deviceReady())
    {
        emit setStatusMessage(tr("Opening Page..."));
        emit setStatusSymbol(3);;
        if (sysxIO->isConnected())
        {
            sysxIO->setDeviceReady(false); // Reserve the device for interaction.
            QObject::disconnect(sysxIO, SIGNAL(sysxReply(QString)));
            QObject::connect(sysxIO, SIGNAL(sysxReply(QString)), this, SLOT(systemReply(QString)));
            emit setStatusProgress(100);
            emit setStatusSymbol(2);
            emit setStatusProgress(0);
            emit setStatusMessage(tr("Request System data"));
            sysxIO->sysxOutput(systemRequest, systemReplySize); // SY-1000 System area data Request.
        }
        else
        {
            QString snork = tr("Ensure connection is active and retry<br>");
            snork.append(tr("System data not transfered, current default settings are to be used<br>"));
            QMessageBox *msgBox = new QMessageBox();
            msgBox->setWindowTitle(deviceType + tr(" connection not found!!"));
            msgBox->setIcon(QMessageBox::Information);
            msgBox->setText(snork);
            msgBox->show();
            QTimer::singleShot(3000, msgBox, SLOT(deleteLater()));
            emit setStatusMessage(tr("System data not transferred"));
            emit setStatusSymbol(0);
            do_the_list();
        };
    };
}

void summaryDialogSystem::do_the_list()
{
    /*********************************************************************/
    /************** The System List **************************************/
    /*********************************************************************/
    //MidiTable *midiTable = MidiTable::Instance();
    text = "<b>Boss " + deviceType + " System/Global Summary</b>    ";
    QDateTime dateTime = QDateTime::currentDateTime();
    QString dateTimeString = dateTime.toString();
    text.append(dateTimeString);
    
    text.append("<br><br><b><u>********** Common ***********</b></u><br>");
    address1= "00";
    start = 0;
    finish = 2;
    makeList();

    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){sys1 = "08";}else{sys1 = "01";};

    address1= "00";
    start = 0;
    finish = 40;
    makeList();

    text.append("<br><br><b><u>********** Control **********</b></u><br>");
    address1= "10";
    start = 0;
    finish = 64;
    makeList();

    text.append("<br><br><b><u>********** LED **********</b></u><br>");
    address1= "20";
    start = 0;
    finish = 27;
    makeList();

    text.append("<br><br><b><u>********** Midi **********</b></u><br>");
    address1= "30";
    start = 0;
    finish = 27;
    makeList();

    text.append("<br><br><b><u>********** GK set 1 **********</b></u><br>");
    address1= "40";
    start = 0;
    finish = 29;
    makeList();

    text.append("<br><br><b><u>********** GK set 2 **********</b></u><br>");
    address1= "50";
    start = 0;
    finish = 29;
    makeList();

    text.append("<br><br><b><u>********** GK set 3 **********</b></u><br>");
    address1= "60";
    start = 0;
    finish = 29;
    makeList();

    text.append("<br><br><b><u>********** GK set 4 **********</b></u><br>");
    address1= "70";
    start = 0;
    finish = 29;
    makeList();

    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){sys1 = "09";}else{sys1 = "02";};

    text.append("<br><br><b><u>********** GK set 5 **********</b></u><br>");
    address1= "00";
    start = 0;
    finish = 29;
    makeList();

    text.append("<br><br><b><u>********** GK set 6 **********</b></u><br>");
    address1= "10";
    start = 0;
    finish = 29;
    makeList();

    text.append("<br><br><b><u>********** GK set 7 **********</b></u><br>");
    address1= "20";
    start = 0;
    finish = 29;
    makeList();

    text.append("<br><br><b><u>********** GK set 8 **********</b></u><br>");
    address1= "30";
    start = 0;
    finish = 29;
    makeList();

    text.append("<br><br><b><u>********** GK set 9 **********</b></u><br>");
    address1= "40";
    start = 0;
    finish = 29;

    makeList();

    text.append("<br><br><b><u>********** GK set 10 **********</b></u><br>");
    address1= "50";
    start = 0;
    finish = 29;
    makeList();

    text.append("<br><br><b><u>********** Normal set 1 **********</b></u><br>");
    address1= "60";
    start = 0;
    finish = 15;
    makeList();

    text.append("<br><br><b><u>********** Normal set 2 **********</b></u><br>");
    address1= "70";
    start = 0;
    finish = 15;
    makeList();

    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){sys1 = "0A";}else{sys1 = "03";};

    text.append("<br><br><b><u>********** Normal set 3 **********</b></u><br>");
    address1= "00";
    start = 0;
    finish = 15;
    makeList();

    text.append("<br><br><b><u>********** Normal set 4 **********</b></u><br>");
    address1= "10";
    start = 0;
    finish = 15;
    makeList();

    text.append("<br><br><b><u>********** Normal set 5 **********</b></u><br>");
    address1= "20";
    start = 0;
    finish = 15;
    makeList();

    text.append("<br><br><b><u>********** Normal set 6 **********</b></u><br>");
    address1= "30";
    start = 0;
    finish = 15;
    makeList();

    text.append("<br><br><b><u>********** Normal set 7 **********</b></u><br>");
    address1= "40";
    start = 0;
    finish = 15;
    makeList();

    text.append("<br><br><b><u>********** Normal set 8 **********</b></u><br>");
    address1= "50";
    start = 0;
    finish = 15;
    makeList();

    text.append("<br><br><b><u>********** Normal set 9 **********</b></u><br>");
    address1= "60";
    start = 0;
    finish = 15;
    makeList();

    text.append("<br><br><b><u>********** Normal set 10 **********</b></u><br>");
    address1= "70";
    start = 0;
    finish = 15;
    makeList();

    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){sys1 = "0B";}else{sys1 = "04";};


    text.append("<br><br><b><u>********** Input and Output **********</b></u><br>");
    address1= "00";
    start = 0;
    finish = 70;
    makeList();
    address1= "10";
    start = 0;
    finish = 1;
    makeList();

    text.append("<br><br><b><u>********** Tuner **********</b></u><br>");
    address1= "20";
    start = 3;
    finish = 7;
    makeList();

    text.append("<br><br><b><u>********** Pitch to Midi **********</b></u><br>");
    address1= "21";
    start = 0;
    finish = 6;
    makeList();

    /*
  text.append("<br><br><b><u>********** MIDI ***********</b></u><br>");
  address1= "02";
  address2= "00";
  start = 0;
  finish = 14;
  makeList();
  
  text.append("<br><br><b><u>********** MIDI MAPPING ***********</b></u><br><br>");
  address1= "02";
  address2= "01";
  finish = 127;
  makeMidiMapList();
  address1= "02";
  address2= "02";
  finish = 127;
  makeMidiMapList();
  address1= "02";
  address2= "03";
  finish = 127;
  makeMidiMapList();
  address1= "02";
  address2= "04";
  finish = 127;
  makeMidiMapList();
  address1= "02";
  address2= "05";
  finish = 127;
  makeMidiMapList();
  address1= "02";
  address2= "06";
  finish = 127;
  makeMidiMapList();
  address1= "02";
  address2= "07";
  finish = 127;
  makeMidiMapList();
  address1= "02";
  address2= "08";
  finish = 72;
  makeMidiMapList();
  
  */




    textDialog->setText(text);
    textDialog->show();

    QPushButton *cancelButton = new QPushButton(tr("Close"));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancel()));
    cancelButton->setWhatsThis(tr("Will close the currently open Dialog page."));
    //cancelButton->hide();

    QPushButton *printButton = new QPushButton(tr("Print"));
    connect(printButton, SIGNAL(clicked()), this, SLOT(printFile()));
    printButton->setWhatsThis(tr("Will Print the current Dialog to the prefered printer<br>printed text will be simular to the screen layout."));
    //printButton->hide();

    QPushButton *saveAsButton = new QPushButton(tr("Save As"));
    connect(saveAsButton, SIGNAL(clicked()), this, SLOT(saveAs()));
    saveAsButton->setWhatsThis(tr("Will save the current dialog page to file in a *.txt format."));
    //saveAsButton->hide();

    QHBoxLayout *horizontalLayout = new QHBoxLayout;
    horizontalLayout->addWidget(textDialog);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch(1);
    buttonsLayout->addWidget(printButton);
    buttonsLayout->addSpacing(12);
    buttonsLayout->addWidget(saveAsButton);
    buttonsLayout->addSpacing(12);
    buttonsLayout->addWidget(cancelButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(horizontalLayout);
    mainLayout->addStretch(1);
    mainLayout->addSpacing(12);
    mainLayout->addLayout(buttonsLayout);
    setLayout(mainLayout);

    setWindowTitle(deviceType + tr(" System and Global List Summary"));
}

void summaryDialogSystem::systemReply(QString replyMsg)
{
    SysxIO *sysxIO = SysxIO::Instance();
    QObject::disconnect(sysxIO, SIGNAL(sysxReply(QString)), this, SLOT(systemReply(QString)));
    sysxIO->setDeviceReady(true); // Free the device after finishing interaction.

    //if(sysxIO->noError())
   // {
        if(replyMsg.size()/2 == systemReplySize)  // data format from the SY-1000
        {
            sysxIO->setFileSource("System", replyMsg);		// Set the source to the data received.
            //setFileName(tr("System Data from ") + deviceType);	// Set the file name to SY-1000 system for the display.
            sysxIO->setDevice(true);				// Patch received from the device so this is set to true.
            sysxIO->setSyncStatus(true);			// We can't be more in sync than right now! :)
            do_the_list();
        }
        else
        {
            QMessageBox *msgBox = new QMessageBox();
            msgBox->setWindowTitle(deviceType + tr(" FloorBoard connection Error !!"));
            msgBox->setIcon(QMessageBox::Warning);
            msgBox->setTextFormat(Qt::RichText);
            QString msgText;
            msgText.append("<font size='+1'><b>");
            msgText.append(tr("The Boss ") + deviceType + tr(" System data was not transfered !!."));
            msgText.append("<b></font><br>");
            msgBox->setText(msgText);
            msgBox->show();
            QTimer::singleShot(3000, msgBox, SLOT(deleteLater()));
            do_the_list();
        };
   // };
    //emit setStatusMessage(tr("Ready"));
}

void summaryDialogSystem::cancel()
{
    SysxIO *sysxIO = SysxIO::Instance();
    QObject::disconnect(sysxIO, SIGNAL(patchName(QString)),
                        this, SLOT(updatePatchNames(QString)));
    sysxIO->setDeviceReady(true);
    emit setStatusSymbol(1);
    emit setStatusMessage(tr("Ready"));
    this->close();
}

void summaryDialogSystem::printFile()
{
//#ifdef  Q_PROCESSOR_ARM
//#elif QT_NO_PRINTER

    QPrinter printer;
    QPrintDialog *dialog = new QPrintDialog(&printer, this);
    //QPrintPreviewDialog *dialog = new QPrintPreviewDialog(&printer, this);
    dialog->setWindowTitle(tr("Print Document"));
    if (dialog->exec() != QDialog::Accepted) { return; }
    else { textDialog->print(&printer); };
    dialog->deleteLater();
//#endif
}

void summaryDialogSystem::saveAs()
{

    Preferences *preferences = Preferences::Instance();
    QString dir = preferences->getPreferences("General", "Files", "dir");

    QString fileName = QFileDialog::getSaveFileName(
                this,
                tr("Save As"),
                dir,
                tr("Text Document (*.txt)"));
    if (!fileName.isEmpty())
    {
        if(!fileName.contains(".txt"))
        {
            fileName.append(".txt");
        };
        QFile file(fileName);

        QByteArray out;
        text.remove("<b>");
        text.remove("</b>");
        text.remove("<u>");
        text.remove("</u>");
        QString newLine;
        newLine.append((char)13);
        newLine.append((char)10);
        text.replace("<br>", newLine);

        unsigned int size = text.size();

        for (unsigned int x=0; x<size; x++)
        {
            QString str(text.at(x));
            out.append(str.toLatin1());
        };

        if (file.open(QIODevice::WriteOnly))
        {
            file.write(out);
        };

    };
}

void summaryDialogSystem::makeList()
{
    SysxIO *sysxIO = SysxIO::Instance();
    MidiTable *midiTable = MidiTable::Instance();
    for(int i=start;i<finish;i++ )
    {
        QString pos = QString::number(i, 16).toUpper();
        if(pos.size()<2){ pos.prepend("0"); };
        QString pretxt =  midiTable->getMidiMap("00", sys1, address1, pos).desc;
        QString txt =  midiTable->getMidiMap("00", sys1, address1, pos).customdesc;

        if(!txt.isEmpty() && txt != "")                                    // all the generic 1 byte parameters.
        {
            int value = sysxIO->getSourceValue("00", sys1, address1, pos);
            QString valueHex = QString::number(value, 16).toUpper();
            if(valueHex.length() < 2) {valueHex.prepend("0"); };
            if (txt != "#")
            {
                text.append("<br>");
                text.append("[");
                if(!pretxt.isEmpty() && txt != "") { text.append(pretxt + " "); };
                text.append(txt);
                text.append("] = ");
            };
            text.append(midiTable->getValue("00", sys1, address1, pos, valueHex) );
        };
        if(txt.contains("GK Set Name") || txt.contains("Normal Set Name"))      // add the remaing 7 charaters to the names.
        {
            for(int addr = i+1; addr<(i+8); addr++)
            {
                QString xpos = QString::number(addr, 16).toUpper();
                if(xpos.size()<2){xpos.prepend("0");};
                int value = sysxIO->getSourceValue("00", sys1, address1, xpos);
                QString valueHex = QString::number(value, 16).toUpper();
                if(valueHex.length() < 2) {valueHex.prepend("0"); };
                text.append(midiTable->getValue("00", sys1, address1, xpos, valueHex) );
            };
        };
        if((txt.contains("Knob") && txt.contains("Setting")) || (txt.contains("Patch Number")))                   // knob setting 4 byte parameters.
        {
            text.chop(1);
            QString valueHex;
            int addr=i;
            for(int z=0; z<4; z++)
            {
                QString xpos = QString::number(addr, 16).toUpper();
                if(xpos.size()<2){xpos.prepend("0");};
                int value = sysxIO->getSourceValue("00", sys1, address1, xpos);
                valueHex.append(QString::number(value, 16).toUpper());
                addr++;
            };
            if(valueHex.length() < 2) {valueHex.prepend("0"); };
            std::string st = valueHex.toStdString();
            std::cerr << st << std::endl;
            if(txt.contains("Knob")){text.append(midiTable->getValue("Tables", "00", "02", "07", valueHex) );}
            else{text.append(midiTable->getValue("Tables", "00", "02", "06", valueHex) );};
        };
        if(txt.contains("Scale"))
        {
            Preferences *preferences = Preferences::Instance();
            QString scale ="08";  // guitar scale length
            if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){scale = "09";};  // bass scale length
            text.chop(2); // remove the invalid entry from above generic function.
            QString valueHex;;
            int addr=i;
            for(int z=0; z<2; z++)
            {
                QString xpos = QString::number(addr, 16).toUpper();
                if(xpos.size()<2){xpos.prepend("0");};
                int value = sysxIO->getSourceValue("00", sys1, address1, xpos);
                valueHex.append(QString::number(value, 16).toUpper());
                addr++;
            }
            if(valueHex.length() < 2) {valueHex.prepend("0"); };
            text.append(midiTable->getValue("Tables", "00", "02", scale, valueHex) );
        }
    };
}

void summaryDialogSystem::makeMidiMapList()
{
 /*   SysxIO *sysxIO = SysxIO::Instance();
    MidiTable *midiTable = MidiTable::Instance();
    int row = 1;
    bool ok;
    QList <QString> sysxData = sysxIO->getFileSource("System", address1, address2);
    for(int i=0;i<finish;i++ )
    {
        QString pos = QString::number(i, 16).toUpper();
        if(pos.size()<2){ pos.prepend("0"); };
        QString txt =  midiTable->getMidiMap("System", "02", address2, pos).customdesc;
        if(!txt.isEmpty())
        {
            int value1 = sysxData.at(sysxDataOffset + i).toInt(&ok, 16);;
            int value2 = sysxData.at(sysxDataOffset + i + 1).toInt(&ok, 16);;
            int value = (value1*128) + value2;
            QString valueHex = QString::number(value, 16).toUpper();
            if(valueHex.length() < 2) {valueHex.prepend("0"); };

            text.append("[");
            text.append(txt);
            text.append("] = ");
            text.append(midiTable->getValue("System", "02", "01", "00", valueHex) );
            text.append(".    .");
            ++row;
            if (row >= 5) {row = 1; text.append("<br>"); };
        };
        ++i;
    };*/
}



