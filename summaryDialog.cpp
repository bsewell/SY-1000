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
#include "summaryDialog.h"
#include "Preferences.h"
#include "SysxIO.h"
#include "MidiTable.h"
//#include "globalVariables.h"

summaryDialog::summaryDialog(QWidget *parent)
    : QWidget(parent)
{
    hex1 = "00";
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){hex1 = "02";};
    bool ok;
    const double ratio = preferences->getPreferences("Window", "Scale", "ratio").toDouble(&ok);

    this->mode = "Compact";
    this->filter = "off";
    this->textDialog = new QTextEdit(parent);
    textDialog->setReadOnly(true);
    //textDialog->setWordWrapMode(QTextOption::NoWrap);
    textDialog->setDocumentTitle("SY-1000 Patch settings");
    textDialog->setFixedHeight(700*ratio);


    SysxIO *sysxIO = SysxIO::Instance();
    QString sysxMsg;
    QList< QList<QString> > patchData = sysxIO->getFileSource().hex; // Get the loaded patch data.
    for(int i=0;i<patchData.size();++i)
    {
        QList<QString> data = patchData.at(i);
        for(int x=0;x<data.size();++x)
        {
            QString hex;
            hex = data.at(x);
            if (hex.length() < 2) hex.prepend("0");
            sysxMsg.append(hex);
            if(hex == "F7"){ sysxMsg.append("<br>"); };
        };
    };
    /******************************************************
  ******QString "sysxMsg" contains current patch data *****
  ******************************************************/
    QDateTime dateTime = QDateTime::currentDateTime();
    text = dateTime.toString();
    text.append("<br>");
    small_text = text;
    small_text.append("<b><u>Boss SY-1000 Compact Patch Summary</u></b><br>");
    small_text.append("a listing of active only effects.<br>");
    large_text = text;
    large_text.append("<b><u>Boss SY-1000 Complete Patch Summary</u></b><br>");
    large_text.append("a list of all parameters with-in this patch.<br>");


    QString patchName = sysxIO->getCurrentPatchName();
    text = "<br><b>Patch name = " + patchName + "</b>";

    text.append("<br><br><b>Patch Mode = ");
    if(hex1=="00"){text.append("Guitar</b>");}else{text.append("Bass</b>");};

    small_text.append(text);
    large_text.append(text);

    text = "<br><br><b><u>Patch Common***********</b></u>";
    address= "00";
    start = 16;
    finish = 128;
    makeList();
    large_text.append(text);
    small_text.append(text);

    address= "01";
    start = 1;
    finish = 27;
    makeList();
    large_text.append(text);
    small_text.append(text);

    address= "02";
    start = 0;
    finish = 2;
    makeList();
    large_text.append(text);
    small_text.append(text);

    text = "<br><br><b><u>LED***********</b></u>";
    address= "03";
    start = 0;
    finish = 27;
    makeList();
    large_text.append(text);
    small_text.append(text);

    this->effect = "off";
    text = "<br><br><b><u>Assign 1***********</b></u>";
    address= "04";
    start = 0;
    finish = 43;
    makeList();
    large_text.append(text);
    if(effect == "on") { small_text.append(text); };

    this->effect = "off";
    text = "<br><br><b><u>Assign 2***********</b></u>";
    address= "04";
    start = 64;
    finish = 107;
    makeList();
    large_text.append(text);
    if(effect == "on") { small_text.append(text); };

    this->effect = "off";
    text = "<br><br><b><u>Assign 3***********</b></u>";
    address= "05";
    start = 0;
    finish = 43;
    makeList();
    large_text.append(text);
    if(effect == "on") { small_text.append(text); };

    this->effect = "off";
    text = "<br><br><b><u>Assign 4***********</b></u>";
    address= "05";
    start = 64;
    finish = 107;
    makeList();
    large_text.append(text);
    if(effect == "on") { small_text.append(text); };

    this->effect = "off";
    text = "<br><br><b><u>Assign 5***********</b></u>";
    address= "06";
    start = 0;
    finish = 43;
    makeList();
    large_text.append(text);
    if(effect == "on") { small_text.append(text); };

    this->effect = "off";
    text = "<br><br><b><u>Assign 6***********</b></u>";
    address= "06";
    start = 64;
    finish = 107;
    makeList();
    large_text.append(text);
    if(effect == "on") { small_text.append(text); };

    this->effect = "off";
    text = "<br><br><b><u>Assign 7***********</b></u>";
    address= "07";
    start = 0;
    finish = 43;
    makeList();
    large_text.append(text);
    if(effect == "on") { small_text.append(text); };

    this->effect = "off";
    text = "<br><br><b><u>Assign 8***********</b></u>";
    address= "07";
    start = 64;
    finish = 107;
    makeList();
    large_text.append(text);
    if(effect == "on") { small_text.append(text); };

    this->effect = "off";
    text = "<br><br><b><u>Assign 9***********</b></u>";
    address= "08";
    start = 0;
    finish = 43;
    makeList();
    large_text.append(text);
    if(effect == "on") { small_text.append(text); };

    this->effect = "off";
    text = "<br><br><b><u>Assign 10***********</b></u>";
    address= "08";
    start = 64;
    finish = 107;
    makeList();
    large_text.append(text);
    if(effect == "on") { small_text.append(text); };

    this->effect = "off";
    text = "<br><br><b><u>Assign 11***********</b></u>";
    address= "09";
    start = 0;
    finish = 43;
    makeList();
    large_text.append(text);
    if(effect == "on") { small_text.append(text); };

    this->effect = "off";
    text = "<br><br><b><u>Assign 12***********</b></u>";
    address= "09";
    start = 64;
    finish = 107;
    makeList();
    large_text.append(text);
    if(effect == "on") { small_text.append(text); };

    this->effect = "off";
    text = "<br><br><b><u>Assign 13***********</b></u>";
    address= "0A";
    start = 0;
    finish = 43;
    makeList();
    large_text.append(text);
    if(effect == "on") { small_text.append(text); };

    this->effect = "off";
    text = "<br><br><b><u>Assign 14***********</b></u>";
    address= "0A";
    start = 64;
    finish = 107;
    makeList();
    large_text.append(text);
    if(effect == "on") { small_text.append(text); };

    this->effect = "off";
    text = "<br><br><b><u>Assign 15***********</b></u>";
    address= "0B";
    start = 0;
    finish = 43;
    makeList();
    large_text.append(text);
    if(effect == "on") { small_text.append(text); };

    this->effect = "off";
    text = "<br><br><b><u>Assign 16***********</b></u>";
    address= "0B";
    start = 64;
    finish = 107;
    makeList();
    large_text.append(text);
    if(effect == "on") { small_text.append(text); };

    text = "<br><br><b><u>Patch Effect***********</b></u>";
    address= "12";
    start = 0;
    finish = 103;
    makeList();
    large_text.append(text);
    small_text.append(text);

    text = "<br><br><b><u>Guitar to Midi***********</b></u>";
    address= "14";
    start = 0;
    finish = 7;
    makeList();
    large_text.append(text);
    small_text.append(text);

    this->effect = "off";
    text = "<br><br><b><u>INSTRUMENT 1***********</b></u>";
    address= "15";
    start = 0;
    finish = 18;
    makeList();
    text.append("<br><br><b><u>Dynamic Synth 1***********</b></u>");
    address= "16";
    start = 4;
    finish = 128;
    makeList();
    address= "17";
    start = 0;
    finish = 78;
    makeList();
    text.append("<br><br><b><u>OSC Synth 1***********</b></u>");
    address= "18";
    start = 0;
    finish = 65;
    makeList();
    text.append("<br><br><b><u>GR-300 Synth 1***********</b></u>");
    address= "19";
    start = 0;
    finish = 21;
    makeList();

    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){bass();}
    else{guitar();};

    textDialog->setText(small_text);
    textDialog->show();


    QPushButton *cancelButton = new QPushButton(tr("Close"));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancel()));
    cancelButton->setWhatsThis(tr("Will close the currently open Dialog page."));

    QPushButton *printButton = new QPushButton(tr("Print"));
    connect(printButton, SIGNAL(clicked()), this, SLOT(printFile()));
    printButton->setWhatsThis(tr("Will Print the current Dialog to the prefered printer<br>printed text will be simular to the screen layout."));

    QPushButton *printPreviewButton = new QPushButton(tr("Print Preview"));
    connect(printPreviewButton, SIGNAL(clicked()), this, SLOT(printPreview()));
    printPreviewButton->setWhatsThis(tr("Will Print the current Dialog to the prefered printer<br>printed text will be simular to the screen layout."));

    QPushButton *saveAsButton = new QPushButton(tr("Save As"));
    connect(saveAsButton, SIGNAL(clicked()), this, SLOT(saveAs()));
    saveAsButton->setWhatsThis(tr("Will save the current dialog page to file in a *.txt format."));

    viewButton = new QPushButton(tr("More Details"));
    connect(viewButton, SIGNAL(clicked()), this, SLOT(view()));
    viewButton->setWhatsThis(tr("Will Expand the summary dialog to include all patch parameters."));

    QHBoxLayout *horizontalLayout = new QHBoxLayout;
    horizontalLayout->addWidget(textDialog);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch(1);
    //buttonsLayout->addWidget(printPreviewButton);
    //buttonsLayout->addSpacing(12);
    buttonsLayout->addWidget(printButton);
    buttonsLayout->addSpacing(12);
    buttonsLayout->addWidget(saveAsButton);
    buttonsLayout->addSpacing(12);
    buttonsLayout->addWidget(viewButton);
    buttonsLayout->addSpacing(12);
    buttonsLayout->addWidget(cancelButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(horizontalLayout);
    mainLayout->addStretch(1);
    mainLayout->addSpacing(12);
    mainLayout->addLayout(buttonsLayout);
    setLayout(mainLayout);

    setWindowTitle(tr("SY-1000 Patch Summary of ")+ patchName);
}

void summaryDialog::makeList()
{
    // construct a text string using address and locator parameters to read from midi.xml
    SysxIO *sysxIO = SysxIO::Instance();
    MidiTable *midiTable = MidiTable::Instance();
    bool ok;
    for(int i=start;i<finish;i++ )  //start and finish range defined above.
    {
        QString temp;
        QString pos2;
        QString pos3;
        QString pos4;
        QString address2 = address;
        int pos_int = i;

        QString pos = QString::number(pos_int, 16).toUpper();
        if(pos.size()<2){ pos.prepend("0"); };
        QString txt = midiTable->getMidiMap("10", hex1, address, pos).customdesc;  //trawl through midi.xml

        pos_int = i+1; if(pos_int > 127){pos_int=pos_int-128; address2 = QString::number(address.toInt(&ok, 16)+1).toUpper(); };
        if (address2.size()<2){address2.prepend("0"); };
        pos2 = QString::number(pos_int, 16).toUpper();
        if(pos2.size()<2){ pos2.prepend("0"); };

        pos_int = i+2; if(pos_int > 127){pos_int=pos_int-128; address2 = QString::number(address.toInt(&ok, 16)+1).toUpper(); };
        if (address2.size()<2){address2.prepend("0"); };
        pos3 = QString::number(pos_int, 16).toUpper();
        if(pos3.size()<2){ pos3.prepend("0"); };

        pos_int = i+3; if(pos_int > 127){pos_int=pos_int-128; address2 = QString::number(address.toInt(&ok, 16)+1).toUpper(); };
        if (address2.size()<2){address2.prepend("0"); };
        pos4 = QString::number(pos_int, 16).toUpper();
        if(pos4.size()<2){ pos4.prepend("0"); };

        if(!txt.isEmpty() && txt != "") // skip the empty midi.xml .desc section and move to the next.
        {
            QString pretxt =  midiTable->getMidiMap("10", hex1, address, pos).desc;
            int value = sysxIO->getSourceValue("10", hex1, address, pos);
            QString valueHex = QString::number(value, 16).toUpper();
            if(valueHex.length() < 2) {valueHex.prepend("0"); };
            temp.append("<br>");
            temp.append("[");
            if(!pretxt.isEmpty() && txt != "") { temp.append(pretxt + " "); };
            temp.append(txt);
            temp.append("] = ");
            QString x;
            if(pretxt.contains("Assign") && (txt.contains("Target")) )
            {
                value = sysxIO->getSourceValue("10", hex1, address, pos);
                valueHex = QString::number(value, 16).toUpper();
                value = sysxIO->getSourceValue("10", hex1, address2, pos2);
                QString valueHex2 = QString::number(value, 16).toUpper();
                value = sysxIO->getSourceValue("10", hex1, address2, pos3);
                QString valueHex3 = QString::number(value, 16).toUpper();
                value = sysxIO->getSourceValue("10", hex1, address2, pos4);
                QString valueHex4 = QString::number(value, 16).toUpper();
                valueHex.append(valueHex2).append(valueHex3).append(valueHex4);
                int maxRange = 256;
                value = valueHex.toInt(&ok, 16);
                int dif = value/maxRange;
                QString valueHex1 = QString::number(dif, 16).toUpper();
                if(valueHex1.length() < 2) valueHex1.prepend("0");
                valueHex2 = QString::number(value - (dif * maxRange), 16).toUpper();
                if(valueHex2.length() < 2) valueHex2.prepend("0");
                QString hex4 = valueHex1;
                QString hex5 = valueHex2;
                Midi items = midiTable->getMidiMap("Tables", "00", "00", hex1, hex4, hex5);
                assign_desc = items.desc;
                assign_customdesc = items.customdesc;
                temp.append( items.name);
            }
            else if((pretxt.contains("Assign") && (txt.contains("Min") || txt.contains("Max")))
                    || (pretxt.contains("D") && txt=="Time")
                    || (pretxt.contains("Foot") && (txt.contains("Volume") || txt.contains("Position")))
                    || (pretxt.contains("PedalBend") && txt.contains("Position"))
                    || (pretxt.contains("Master") && txt.contains("TEMPO"))
                    || (txt.contains("Pre-Delay"))
                    || (pretxt.contains("`"))
                    )
            {
                value = sysxIO->getSourceValue("10", hex1, address, pos);
                valueHex = QString::number(value, 16).toUpper();
                value = sysxIO->getSourceValue("10", hex1, address, pos2);
                QString valueHex2 = QString::number(value, 16).toUpper();
                value = sysxIO->getSourceValue("10", hex1, address, pos3);
                QString valueHex3 = QString::number(value, 16).toUpper();
                value = sysxIO->getSourceValue("10", hex1, address, pos4);
                QString valueHex4 = QString::number(value, 16).toUpper();
                valueHex.append(valueHex2).append(valueHex3).append(valueHex4);

                if(txt.contains("CC#")){value = (valueHex.toInt(&ok, 16))/128;}
                else if(txt.contains("Time") || txt.contains("Pre-Delay")){value = valueHex.toInt(&ok, 16);}
                else if(pretxt.contains("`")){value = valueHex.mid(0, 2).toInt(&ok, 16);}
                else if(pretxt.contains("Foot") || pretxt.contains("PedalBend") || txt.contains("TEMPO")){value = valueHex.toInt(&ok, 16)/10;}
                else{value = (valueHex.toInt(&ok, 16)) - int(QString("8000").toInt(&ok, 16));};
                valueHex = QString::number(value, 10).toUpper();
                if(valueHex.length() < 2) valueHex.prepend("0");
                //temp.append(midiTable->getValue("Tables", "00", "00", assign_customdesc, valueHex));
                temp.append(valueHex);
                if(txt.contains("Time") || txt.contains("Pre-Delay")){temp.append("ms");};
            }
            else
            {
                temp.append(midiTable->getValue("10", hex1, address, pos, valueHex));
            };
            text2.append(temp);

            if (this->filter != "off")
            {
                if (pretxt == this->filter) { text.append(temp); };
            } else if(!pretxt.contains("Custom:")){text.append(temp); };
            if((i == start || i == start+1 || i == start+2) && x == "On") { this->effect = "on"; }; // first byte is usually the effect on/off switch
            if((pretxt == "MOD:" || pretxt == "MFX:") && (txt == "Type"))
            {this->filter = midiTable->getMidiMap("10", hex1, address, pos, valueHex).desc;};
        };
    };
}

summaryDialog::~summaryDialog()
{}

void summaryDialog::view()
{
    if (mode == "Compact")
    {
        viewButton->setText(tr("Less Details"));
        textDialog->setText(large_text);
        this->mode = "Expanded";
    }
    else
    {
        viewButton->setText(tr("More Details"));
        textDialog->setText(small_text);
        this->mode = "Compact";
    };
}

void summaryDialog::cancel()
{
    this->close();
}

void summaryDialog::printFile()
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

void summaryDialog::printPreview()
{
#ifdef  Q_PROCESSOR_ARM
#elif QT_NO_PRINTER

    QPrinter printer;
    //QPrintDialog *dialog = new QPrintDialog(&printer, this);
    QPrintPreviewDialog *dialog = new QPrintPreviewDialog(&printer, this);
    dialog->setWindowTitle(tr("Print Preview"));
    if (dialog->exec() != QDialog::Accepted) { return; }
    else { textDialog->print(&printer); };
    dialog->deleteLater();
#endif
}

void summaryDialog::saveAs()
{

    Preferences *preferences = Preferences::Instance();
    QString dir = preferences->getPreferences("General", "Files", "dir");

    QString fileName = QFileDialog::getSaveFileName(
                this,
                tr("Save As"),
                dir,
                tr("text Document (*.txt)"));
    if (!fileName.isEmpty())
    {
        if(!fileName.contains(".txt"))
        {
            fileName.append(".txt");
        };
        QFile file(fileName);

        QByteArray out;
        large_text.remove("<b>");
        large_text.remove("</b>");
        large_text.remove("<u>");
        large_text.remove("</u>");
        QString newLine;
        newLine.append((char)13);
        newLine.append((char)10);
        large_text.replace("<br>", newLine);

        unsigned int size = large_text.size();

        for (unsigned int x=0; x<size; x++)
        {
            QString str(large_text.at(x));
            out.append(str.toLatin1());
        };

        if (file.open(QIODevice::WriteOnly))
        {
            file.write(out);
        };
    }
}

void summaryDialog::guitar()
{

    text.append("<br><br><b><u>Electric Guitar 1***********</b></u>");
    address= "1A";
    start = 0;
    finish = 45;
    makeList();
    text.append("<br><br><b><u>Acoustic Guitar 1***********</b></u>");
    address= "1B";
    start = 0;
    finish = 47;
    makeList();
    text.append("<br><br><b><u>Electric Bass 1***********</b></u>");
    address= "1C";
    start = 0;
    finish = 46;
    makeList();
    text.append("<br><br><b><u>VIO Guitar 1***********</b></u>");
    address= "1D";
    start = 0;
    finish = 37;
    makeList();
    text.append("<br><br><b><u>POLY FX 1***********</b></u>");
    address= "1E";
    start = 0;
    finish = 19;
    makeList();
    text.append("<br><br><b><u>ALT Tuning 1***********</b></u>");
    address= "1F";
    start = 0;
    finish = 48;
    makeList();
    large_text.append(text);
    if(effect == "on") { small_text.append(text); };


    this->effect = "off";
    text = "<br><br><b><u>INSTRUMENT 2***********</b></u>";
    address= "20";
    start = 0;
    finish = 18;
    makeList();
    text.append("<br><br><b><u>Dynamic Synth 2***********</b></u>");
    address= "21";
    start = 4;
    finish = 128;
    makeList();
    address= "22";
    start = 0;
    finish = 78;
    makeList();
    text.append("<br><br><b><u>OSC Synth 2***********</b></u>");
    address= "23";
    start = 0;
    finish = 65;
    makeList();
    text.append("<br><br><b><u>GR-300 Synth 2***********</b></u>");
    address= "24";
    start = 0;
    finish = 21;
    makeList();
    text.append("<br><br><b><u>Electric Guitar 2***********</b></u>");
    address= "25";
    start = 0;
    finish = 45;
    makeList();
    text.append("<br><br><b><u>Acoustic Guitar 2***********</b></u>");
    address= "26";
    start = 0;
    finish = 47;
    makeList();
    text.append("<br><br><b><u>Electric Bass 2***********</b></u>");
    address= "27";
    start = 0;
    finish = 46;
    makeList();
    text.append("<br><br><b><u>VIO Guitar 2***********</b></u>");
    address= "28";
    start = 0;
    finish = 37;
    makeList();
    text.append("<br><br><b><u>POLY FX 2***********</b></u>");
    address= "29";
    start = 0;
    finish = 19;
    makeList();
    text.append("<br><br><b><u>ALT Tuning 2***********</b></u>");
    address= "2A";
    start = 0;
    finish = 48;
    makeList();
    large_text.append(text);
    if(effect == "on") { small_text.append(text); };


    this->effect = "off";
    text = "<br><br><b><u>INSTRUMENT 3***********</b></u>";
    address= "2B";
    start = 0;
    finish = 18;
    makeList();
    text.append("<br><br><b><u>Dynamic Synth 3***********</b></u>");
    address= "2C";
    start = 4;
    finish = 128;
    makeList();
    address= "2D";
    start = 0;
    finish = 78;
    makeList();
    text.append("<br><br><b><u>OSC Synth 3***********</b></u>");
    address= "2E";
    start = 0;
    finish = 65;
    makeList();
    text.append("<br><br><b><u>GR-300 Synth 3***********</b></u>");
    address= "2F";
    start = 0;
    finish = 21;
    makeList();
    text.append("<br><br><b><u>Electric Guitar 3***********</b></u>");
    address= "30";
    start = 0;
    finish = 45;
    makeList();
    text.append("<br><br><b><u>Acoustic Guitar 3***********</b></u>");
    address= "31";
    start = 0;
    finish = 47;
    makeList();
    text.append("<br><br><b><u>Electric Bass 3***********</b></u>");
    address= "32";
    start = 0;
    finish = 46;
    makeList();
    text.append("<br><br><b><u>VIO Guitar 3***********</b></u>");
    address= "33";
    start = 0;
    finish = 37;
    makeList();
    text.append("<br><br><b><u>POLY FX 3***********</b></u>");
    address= "34";
    start = 0;
    finish = 19;
    makeList();
    text.append("<br><br><b><u>ALT Tuning 3***********</b></u>");
    address= "35";
    start = 0;
    finish = 48;
    makeList();
    large_text.append(text);
    if(effect == "on") { small_text.append(text); };


    this->effect = "off";
    text = "<br><br><b><u>Compressor***********</b></u>";
    address= "36";
    start = 0;
    finish = 7;
    makeList();
    large_text.append(text);
    if(effect == "on") { small_text.append(text); };

    this->effect = "off";
    text = "<br><br><b><u>Distortion***********</b></u>";
    address= "37";
    start = 0;
    finish = 9;
    makeList();
    large_text.append(text);
    if(effect == "on") { small_text.append(text); };

    this->effect = "off";
    text = "<br><br><b><u>Amplifier and Cabinet***********</b></u>";
    address= "38";
    start = 0;
    finish = 20;
    makeList();
    large_text.append(text);
    if(effect == "on") { small_text.append(text); };

    this->effect = "off";
    text = "<br><br><b><u>Noise Suppressor***********</b></u>";
    address= "39";
    start = 0;
    finish = 4;
    makeList();
    large_text.append(text);
    if(effect == "on") { small_text.append(text); };

    this->effect = "off";
    text = "<br><br><b><u>Equalizer 1***********</b></u>";
    address= "3A";
    start = 0;
    finish = 12;
    makeList();
    large_text.append(text);
    if(effect == "on") { small_text.append(text); };

    this->effect = "off";
    text = "<br><br><b><u>Equalizer 2***********</b></u>";
    address= "3B";
    start = 0;
    finish = 12;
    makeList();
    large_text.append(text);
    if(effect == "on") { small_text.append(text); };

    this->effect = "off";
    text = "<br><br><b><u>Delay 1***********</b></u>";
    address= "3C";
    start = 0;
    finish = 10;
    makeList();
    large_text.append(text);
    if(effect == "on") { small_text.append(text); };

    this->effect = "off";
    text = "<br><br><b><u>Delay 2***********</b></u>";
    address= "3D";
    start = 0;
    finish = 10;
    makeList();
    large_text.append(text);
    if(effect == "on") { small_text.append(text); };

    this->effect = "off";
    text = "<br><br><b><u>Master Delay***********</b></u>";
    address= "3E";
    start = 0;
    finish = 33;
    makeList();
    large_text.append(text);
    if(effect == "on") { small_text.append(text); };

    this->effect = "off";
    text = "<br><br><b><u>Chorus***********</b></u>";
    address= "3F";
    start = 0;
    finish = 25;
    makeList();
    large_text.append(text);
    if(effect == "on") { small_text.append(text); };


    this->effect = "off";
    text = "<br><br><b><u>FX 1***********</b></u>";
    address= "40";
    start = 0;
    finish = 2;
    makeList();
    text.append("<br><br><b><u>FX1 Acoustic Resonator***********</b></u>");
    address= "41";
    start = 0;
    finish = 4;
    makeList();
    text.append("<br><br><b><u>FX1 Auto Wah***********</b></u>");
    address= "42";
    start = 0;
    finish = 8;
    makeList();
    text.append("<br><br><b><u>FX1 Chorus***********</b></u>");
    address= "43";
    start = 0;
    finish = 25;
    makeList();
    text.append("<br><br><b><u>FX1 Classic Vibe***********</b></u>");
    address= "44";
    start = 0;
    finish = 3;
    makeList();
    text.append("<br><br><b><u>FX1 Compressor***********</b></u>");
    address= "45";
    start = 0;
    finish = 7;
    makeList();
    text.append("<br><br><b><u>FX1 Defretter***********</b></u>");
    address= "46";
    start = 0;
    finish = 7;
    makeList();
    text.append("<br><br><b><u>FX1 Defretter Bass***********</b></u>");
    address= "47";
    start = 0;
    finish = 5;
    makeList();
    text.append("<br><br><b><u>FX1 Delay***********</b></u>");
    address= "48";
    start = 1;
    finish = 33;
    makeList();
    text.append("<br><br><b><u>FX1 Flanger***********</b></u>");
    address= "49";
    start = 0;
    finish = 9;
    makeList();
    text.append("<br><br><b><u>FX1 Flanger Bass***********</b></u>");
    address= "4A";
    start = 0;
    finish = 9;
    makeList();
    text.append("<br><br><b><u>FX1 Foot Volume***********</b></u>");
    address= "4B";
    start = 0;
    finish = 13;
    makeList();
    text.append("<br><br><b><u>FX1 Graphic EQ***********</b></u>");
    address= "4C";
    start = 0;
    finish = 8;
    makeList();
    text.append("<br><br><b><u>FX1 Harmonist***********</b></u>");
    address= "4D";
    start = 0;
    finish = 39;
    makeList();
    text.append("<br><br><b><u>FX1 Humanizer***********</b></u>");
    address= "4E";
    start = 0;
    finish = 8;
    makeList();
    text.append("<br><br><b><u>FX1 Isolator***********</b></u>");
    address= "4F";
    start = 0;
    finish = 4;
    makeList();
    text.append("<br><br><b><u>FX1 Limiter***********</b></u>");
    address= "50";
    start = 0;
    finish = 6;
    makeList();
    text.append("<br><br><b><u>FX1 Low Fidelity***********</b></u>");
    address= "51";
    start = 0;
    finish = 3;
    makeList();
    text.append("<br><br><b><u>FX1 Octave***********</b></u>");
    address= "52";
    start = 0;
    finish = 3;
    makeList();
    text.append("<br><br><b><u>FX1 Octave Bass***********</b></u>");
    address= "53";
    start = 0;
    finish = 3;
    makeList();
    text.append("<br><br><b><u>FX1 Panner***********</b></u>");
    address= "54";
    start = 0;
    finish = 6;
    makeList();
    text.append("<br><br><b><u>FX1 Parametric EQ***********</b></u>");
    address= "55";
    start = 1;
    finish = 12;
    makeList();
    text.append("<br><br><b><u>FX1 Pedal Bend***********</b></u>");
    address= "56";
    start = 0;
    finish = 7;
    makeList();
    text.append("<br><br><b><u>FX1 Phaser***********</b></u>");
    address= "57";
    start = 0;
    finish = 8;
    makeList();
    text.append("<br><br><b><u>FX1 Pitch Shifter***********</b></u>");
    address= "58";
    start = 0;
    finish = 19;
    makeList();
    text.append("<br><br><b><u>FX1 Reverb***********</b></u>");
    address= "59";
    start = 1;
    finish = 14;
    makeList();
    text.append("<br><br><b><u>FX1 Ring Modulator***********</b></u>");
    address= "5A";
    start = 0;
    finish = 6;
    makeList();
    text.append("<br><br><b><u>FX1 Rotary***********</b></u>");
    address= "5B";
    start = 0;
    finish = 8;
    makeList();
    text.append("<br><br><b><u>FX1 Sitar Simulator***********</b></u>");
    address= "5C";
    start = 0;
    finish = 7;
    makeList();
    text.append("<br><br><b><u>FX1 Slicer***********</b></u>");
    address= "5D";
    start = 0;
    finish = 7;
    makeList();
    text.append("<br><br><b><u>FX1 Slow Gear***********</b></u>");
    address= "5E";
    start = 0;
    finish = 3;
    makeList();
    text.append("<br><br><b><u>FX1 Slow Gear Bass***********</b></u>");
    address= "5F";
    start = 0;
    finish = 3;
    makeList();
    text.append("<br><br><b><u>FX1 Sound Hold***********</b></u>");
    address= "60";
    start = 0;
    finish = 3;
    makeList();
    text.append("<br><br><b><u>FX1 T-Wah***********</b></u>");
    address= "61";
    start = 0;
    finish = 8;
    makeList();
    text.append("<br><br><b><u>FX1 T-Wah Bass***********</b></u>");
    address= "62";
    start = 0;
    finish = 8;
    makeList();
    text.append("<br><br><b><u>FX1 Tremolo***********</b></u>");
    address= "63";
    start = 0;
    finish = 5;
    makeList();
    text.append("<br><br><b><u>FX1 Vibrato***********</b></u>");
    address= "64";
    start = 0;
    finish = 6;
    makeList();
    text.append("<br><br><b><u>FX1 Wah***********</b></u>");
    address= "65";
    start = 0;
    finish = 15;
    makeList();
    large_text.append(text);
    if(effect == "on") { small_text.append(text); };
    this->filter = "off";


    this->effect = "off";
    text = "<br><br><b><u>FX 2***********</b></u>";
    address= "66";
    start = 0;
    finish = 2;
    makeList();
    text.append("<br><br><b><u>FX2 Acoustic Resonator***********</b></u>");
    address= "67";
    start = 0;
    finish = 4;
    makeList();
    text.append("<br><br><b><u>FX2 Auto Wah***********</b></u>");
    address= "68";
    start = 0;
    finish = 8;
    makeList();
    text.append("<br><br><b><u>FX2 Chorus***********</b></u>");
    address= "69";
    start = 0;
    finish = 25;
    makeList();
    text.append("<br><br><b><u>FX2 Classic Vibe***********</b></u>");
    address= "6A";
    start = 0;
    finish = 3;
    makeList();
    text.append("<br><br><b><u>FX2 Compressor***********</b></u>");
    address= "6B";
    start = 0;
    finish = 7;
    makeList();
    text.append("<br><br><b><u>FX2 Defretter***********</b></u>");
    address= "6C";
    start = 0;
    finish = 7;
    makeList();
    text.append("<br><br><b><u>FX2 Defretter Bass***********</b></u>");
    address= "6D";
    start = 0;
    finish = 5;
    makeList();
    text.append("<br><br><b><u>FX2 Delay***********</b></u>");
    address= "6E";
    start = 1;
    finish = 33;
    makeList();
    text.append("<br><br><b><u>FX2 Flanger***********</b></u>");
    address= "6F";
    start = 0;
    finish = 9;
    makeList();
    text.append("<br><br><b><u>FX2 Flanger Bass***********</b></u>");
    address= "70";
    start = 0;
    finish = 9;
    makeList();
    text.append("<br><br><b><u>FX2 Foot Volume***********</b></u>");
    address= "71";
    start = 0;
    finish = 13;
    makeList();
    text.append("<br><br><b><u>FX2 Graphic EQ***********</b></u>");
    address= "72";
    start = 0;
    finish = 8;
    makeList();
    text.append("<br><br><b><u>FX2 Harmonist***********</b></u>");
    address= "73";
    start = 0;
    finish = 39;
    makeList();
    text.append("<br><br><b><u>FX2 Humanizer***********</b></u>");
    address= "74";
    start = 0;
    finish = 8;
    makeList();
    text.append("<br><br><b><u>FX2 Isolator***********</b></u>");
    address= "75";
    start = 0;
    finish = 4;
    makeList();
    text.append("<br><br><b><u>FX2 Limiter***********</b></u>");
    address= "76";
    start = 0;
    finish = 6;
    makeList();
    text.append("<br><br><b><u>FX2 Low Fidelity***********</b></u>");
    address= "77";
    start = 0;
    finish = 3;
    makeList();
    text.append("<br><br><b><u>FX2 Octave***********</b></u>");
    address= "78";
    start = 0;
    finish = 3;
    makeList();
    text.append("<br><br><b><u>FX2 Octave Bass***********</b></u>");
    address= "79";
    start = 0;
    finish = 3;
    makeList();
    text.append("<br><br><b><u>FX2 Panner***********</b></u>");
    address= "7A";
    start = 0;
    finish = 6;
    makeList();
    text.append("<br><br><b><u>FX2 Parametric EQ***********</b></u>");
    address= "7B";
    start = 1;
    finish = 12;
    makeList();
    text.append("<br><br><b><u>FX2 Pedal Bend***********</b></u>");
    address= "7C";
    start = 0;
    finish = 7;
    makeList();
    text.append("<br><br><b><u>FX2 Phaser***********</b></u>");
    address= "7D";
    start = 0;
    finish = 8;
    makeList();
    text.append("<br><br><b><u>FX2 Pitch Shifter***********</b></u>");
    address= "7E";
    start = 0;
    finish = 19;
    makeList();
    text.append("<br><br><b><u>FX2 Reverb***********</b></u>");
    address= "7F";
    start = 1;
    finish = 14;
    makeList();

    hex1 = "01";
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){hex1 = "03";};

    text.append("<br><br><b><u>FX2 Ring Modulator***********</b></u>");
    address= "00";
    start = 0;
    finish = 6;
    makeList();
    text.append("<br><br><b><u>FX2 Rotary***********</b></u>");
    address= "01";
    start = 0;
    finish = 8;
    makeList();
    text.append("<br><br><b><u>FX2 Sitar Simulator***********</b></u>");
    address= "02";
    start = 0;
    finish = 7;
    makeList();
    text.append("<br><br><b><u>FX2 Slicer***********</b></u>");
    address= "03";
    start = 0;
    finish = 7;
    makeList();
    text.append("<br><br><b><u>FX2 Slow Gear***********</b></u>");
    address= "04";
    start = 0;
    finish = 3;
    makeList();
    text.append("<br><br><b><u>FX2 Slow Gear Bass***********</b></u>");
    address= "05";
    start = 0;
    finish = 3;
    makeList();
    text.append("<br><br><b><u>FX2 Sound Hold***********</b></u>");
    address= "06";
    start = 0;
    finish = 3;
    makeList();
    text.append("<br><br><b><u>FX2 T-Wah***********</b></u>");
    address= "07";
    start = 0;
    finish = 8;
    makeList();
    text.append("<br><br><b><u>FX2 T-Wah Bass***********</b></u>");
    address= "08";
    start = 0;
    finish = 8;
    makeList();
    text.append("<br><br><b><u>FX2 Tremolo***********</b></u>");
    address= "09";
    start = 0;
    finish = 5;
    makeList();
    text.append("<br><br><b><u>FX2 Vibrato***********</b></u>");
    address= "0A";
    start = 0;
    finish = 6;
    makeList();
    text.append("<br><br><b><u>FX2 Wah***********</b></u>");
    address= "0B";
    start = 0;
    finish = 15;
    makeList();
    large_text.append(text);
    if(effect == "on") { small_text.append(text); };
    this->filter = "off";


    this->effect = "off";
    text = "<br><br><b><u>FX 3***********</b></u>";
    address= "0C";
    start = 0;
    finish = 2;
    makeList();
    text.append("<br><br><b><u>FX3 Acoustic Resonator***********</b></u>");
    address= "0D";
    start = 0;
    finish = 4;
    makeList();
    text.append("<br><br><b><u>FX3 Auto Wah***********</b></u>");
    address= "0E";
    start = 0;
    finish = 8;
    makeList();
    text.append("<br><br><b><u>FX3 Chorus***********</b></u>");
    address= "0F";
    start = 0;
    finish = 25;
    makeList();
    text.append("<br><br><b><u>FX3 Classic Vibe***********</b></u>");
    address= "10";
    start = 0;
    finish = 3;
    makeList();
    text.append("<br><br><b><u>FX3 Compressor***********</b></u>");
    address= "11";
    start = 0;
    finish = 7;
    makeList();
    text.append("<br><br><b><u>FX3 Defretter***********</b></u>");
    address= "12";
    start = 0;
    finish = 7;
    makeList();
    text.append("<br><br><b><u>FX3 Defretter Bass***********</b></u>");
    address= "13";
    start = 0;
    finish = 5;
    makeList();
    text.append("<br><br><b><u>FX3 Delay***********</b></u>");
    address= "14";
    start = 1;
    finish = 33;
    makeList();
    text.append("<br><br><b><u>FX3 Flanger***********</b></u>");
    address= "15";
    start = 0;
    finish = 9;
    makeList();
    text.append("<br><br><b><u>FX3 Flanger Bass***********</b></u>");
    address= "16";
    start = 0;
    finish = 9;
    makeList();
    text.append("<br><br><b><u>FX3 Foot Volume***********</b></u>");
    address= "17";
    start = 0;
    finish = 13;
    makeList();
    text.append("<br><br><b><u>FX3 Graphic EQ***********</b></u>");
    address= "18";
    start = 0;
    finish = 8;
    makeList();
    text.append("<br><br><b><u>FX3 Harmonist***********</b></u>");
    address= "19";
    start = 0;
    finish = 39;
    makeList();
    text.append("<br><br><b><u>FX3 Humanizer***********</b></u>");
    address= "1A";
    start = 0;
    finish = 8;
    makeList();
    text.append("<br><br><b><u>FX3 Isolator***********</b></u>");
    address= "1B";
    start = 0;
    finish = 4;
    makeList();
    text.append("<br><br><b><u>FX3 Limiter***********</b></u>");
    address= "1C";
    start = 0;
    finish = 6;
    makeList();
    text.append("<br><br><b><u>FX3 Low Fidelity***********</b></u>");
    address= "1D";
    start = 0;
    finish = 3;
    makeList();
    text.append("<br><br><b><u>FX3 Octave***********</b></u>");
    address= "1E";
    start = 0;
    finish = 3;
    makeList();
    text.append("<br><br><b><u>FX3 Octave Bass***********</b></u>");
    address= "1F";
    start = 0;
    finish = 3;
    makeList();
    text.append("<br><br><b><u>FX3 Panner***********</b></u>");
    address= "20";
    start = 0;
    finish = 6;
    makeList();
    text.append("<br><br><b><u>FX3 Parametric EQ***********</b></u>");
    address= "21";
    start = 1;
    finish = 12;
    makeList();
    text.append("<br><br><b><u>FX3 Pedal Bend***********</b></u>");
    address= "22";
    start = 0;
    finish = 7;
    makeList();
    text.append("<br><br><b><u>FX3 Phaser***********</b></u>");
    address= "23";
    start = 0;
    finish = 8;
    makeList();
    text.append("<br><br><b><u>FX3 Pitch Shifter***********</b></u>");
    address= "24";
    start = 0;
    finish = 19;
    makeList();
    text.append("<br><br><b><u>FX3 Reverb***********</b></u>");
    address= "25";
    start = 1;
    finish = 14;
    makeList();
    text.append("<br><br><b><u>FX3 Ring Modulator***********</b></u>");
    address= "26";
    start = 0;
    finish = 6;
    makeList();
    text.append("<br><br><b><u>FX3 Rotary***********</b></u>");
    address= "27";
    start = 0;
    finish = 8;
    makeList();
    text.append("<br><br><b><u>FX3 Sitar Simulator***********</b></u>");
    address= "28";
    start = 0;
    finish = 7;
    makeList();
    text.append("<br><br><b><u>FX3 Slicer***********</b></u>");
    address= "29";
    start = 0;
    finish = 7;
    makeList();
    text.append("<br><br><b><u>FX3 Slow Gear***********</b></u>");
    address= "2A";
    start = 0;
    finish = 3;
    makeList();
    text.append("<br><br><b><u>FX3 Slow Gear Bass***********</b></u>");
    address= "2B";
    start = 0;
    finish = 3;
    makeList();
    text.append("<br><br><b><u>FX3 Sound Hold***********</b></u>");
    address= "2C";
    start = 0;
    finish = 3;
    makeList();
    text.append("<br><br><b><u>FX3 T-Wah***********</b></u>");
    address= "2D";
    start = 0;
    finish = 8;
    makeList();
    text.append("<br><br><b><u>FX3 T-Wah Bass***********</b></u>");
    address= "2E";
    start = 0;
    finish = 8;
    makeList();
    text.append("<br><br><b><u>FX3 Tremolo***********</b></u>");
    address= "2F";
    start = 0;
    finish = 5;
    makeList();
    text.append("<br><br><b><u>FX3 Vibrato***********</b></u>");
    address= "30";
    start = 0;
    finish = 6;
    makeList();
    text.append("<br><br><b><u>FX3 Wah***********</b></u>");
    address= "31";
    start = 0;
    finish = 15;
    makeList();
    large_text.append(text);
    if(effect == "on") { small_text.append(text); };
    this->filter = "off";


    this->effect = "off";
    text = "<br><br><b><u>Reverb***********</b></u>";
    address= "32";
    start = 0;
    finish = 14;
    makeList();
    large_text.append(text);
    if(effect == "on") { small_text.append(text); };


}

void summaryDialog::bass()
{

    text.append("<br><br><b><u>Electric Bass 1***********</b></u>");
    address= "1A";
    start = 0;
    finish = 57;
    makeList();
    text.append("<br><br><b><u>Acoustic Bass 1***********</b></u>");
    address= "1B";
    start = 0;
    finish = 43;
    makeList();
    text.append("<br><br><b><u>Electric Guitar 1***********</b></u>");
    address= "1C";
    start = 0;
    finish = 45;
    makeList();
    text.append("<br><br><b><u>POLY FX 1***********</b></u>");
    address= "1D";
    start = 0;
    finish = 19;
    makeList();
    text.append("<br><br><b><u>ALT Tuning 1***********</b></u>");
    address= "1E";
    start = 0;
    finish = 48;
    makeList();
    large_text.append(text);
    if(effect == "on") { small_text.append(text); };


    this->effect = "off";
    text = "<br><br><b><u>INSTRUMENT 2***********</b></u>";
    address= "1F";
    start = 0;
    finish = 18;
    makeList();
    text.append("<br><br><b><u>Dynamic Synth 2***********</b></u>");
    address= "20";
    start = 4;
    finish = 128;
    makeList();
    address= "21";
    start = 0;
    finish = 78;
    makeList();
    text.append("<br><br><b><u>OSC Synth 2***********</b></u>");
    address= "22";
    start = 0;
    finish = 65;
    makeList();
    text.append("<br><br><b><u>GR-300 Synth 2***********</b></u>");
    address= "23";
    start = 0;
    finish = 21;
    makeList();
    text.append("<br><br><b><u>Electric Bass 2***********</b></u>");
    address= "24";
    start = 0;
    finish = 57;
    makeList();
    text.append("<br><br><b><u>Acoustic Bass 2***********</b></u>");
    address= "25";
    start = 0;
    finish = 43;
    makeList();
    text.append("<br><br><b><u>Electric Guitar 2***********</b></u>");
    address= "26";
    start = 0;
    finish = 45;
    makeList();
    text.append("<br><br><b><u>POLY FX 2***********</b></u>");
    address= "27";
    start = 0;
    finish = 19;
    makeList();
    text.append("<br><br><b><u>ALT Tuning 2***********</b></u>");
    address= "28";
    start = 0;
    finish = 48;
    makeList();
    large_text.append(text);
    if(effect == "on") { small_text.append(text); };


    this->effect = "off";
    text = "<br><br><b><u>INSTRUMENT 3***********</b></u>";
    address= "29";
    start = 0;
    finish = 18;
    makeList();
    text.append("<br><br><b><u>Dynamic Synth 3***********</b></u>");
    address= "2A";
    start = 4;
    finish = 128;
    makeList();
    address= "2B";
    start = 0;
    finish = 78;
    makeList();
    text.append("<br><br><b><u>OSC Synth 3***********</b></u>");
    address= "2C";
    start = 0;
    finish = 65;
    makeList();
    text.append("<br><br><b><u>GR-300 Synth 3***********</b></u>");
    address= "2D";
    start = 0;
    finish = 21;
    makeList();
    text.append("<br><br><b><u>Electric Bass 3***********</b></u>");
    address= "2E";
    start = 0;
    finish = 57;
    makeList();
    text.append("<br><br><b><u>Acoustic Bass 3***********</b></u>");
    address= "2F";
    start = 0;
    finish = 43;
    makeList();
    text.append("<br><br><b><u>Electric Guitar 3***********</b></u>");
    address= "30";
    start = 0;
    finish = 45;
    makeList();
    text.append("<br><br><b><u>POLY FX 3***********</b></u>");
    address= "31";
    start = 0;
    finish = 19;
    makeList();
    text.append("<br><br><b><u>ALT Tuning 3***********</b></u>");
    address= "32";
    start = 0;
    finish = 48;
    makeList();
    large_text.append(text);
    if(effect == "on") { small_text.append(text); };


    this->effect = "off";
    text = "<br><br><b><u>Compressor***********</b></u>";
    address= "33";
    start = 0;
    finish = 7;
    makeList();
    large_text.append(text);
    if(effect == "on") { small_text.append(text); };

    this->effect = "off";
    text = "<br><br><b><u>Distortion***********</b></u>";
    address= "34";
    start = 0;
    finish = 9;
    makeList();
    large_text.append(text);
    if(effect == "on") { small_text.append(text); };

    this->effect = "off";
    text = "<br><br><b><u>Amplifier and Cabinet***********</b></u>";
    address= "35";
    start = 0;
    finish = 20;
    makeList();
    large_text.append(text);
    if(effect == "on") { small_text.append(text); };

    this->effect = "off";
    text = "<br><br><b><u>Noise Suppressor***********</b></u>";
    address= "36";
    start = 0;
    finish = 4;
    makeList();
    large_text.append(text);
    if(effect == "on") { small_text.append(text); };

    this->effect = "off";
    text = "<br><br><b><u>Equalizer 1***********</b></u>";
    address= "37";
    start = 0;
    finish = 12;
    makeList();
    large_text.append(text);
    if(effect == "on") { small_text.append(text); };

    this->effect = "off";
    text = "<br><br><b><u>Equalizer 2***********</b></u>";
    address= "38";
    start = 0;
    finish = 12;
    makeList();
    large_text.append(text);
    if(effect == "on") { small_text.append(text); };

    this->effect = "off";
    text = "<br><br><b><u>Delay 1***********</b></u>";
    address= "39";
    start = 0;
    finish = 10;
    makeList();
    large_text.append(text);
    if(effect == "on") { small_text.append(text); };

    this->effect = "off";
    text = "<br><br><b><u>Delay 2***********</b></u>";
    address= "3A";
    start = 0;
    finish = 10;
    makeList();
    large_text.append(text);
    if(effect == "on") { small_text.append(text); };

    this->effect = "off";
    text = "<br><br><b><u>Master Delay***********</b></u>";
    address= "3B";
    start = 0;
    finish = 33;
    makeList();
    large_text.append(text);
    if(effect == "on") { small_text.append(text); };

    this->effect = "off";
    text = "<br><br><b><u>Chorus***********</b></u>";
    address= "3C";
    start = 0;
    finish = 25;
    makeList();
    large_text.append(text);
    if(effect == "on") { small_text.append(text); };


    this->effect = "off";
    text = "<br><br><b><u>FX 1***********</b></u>";
    address= "3D";
    start = 0;
    finish = 2;
    makeList();
    text.append("<br><br><b><u>FX1 Acoustic Resonator***********</b></u>");
    address= "3E";
    start = 0;
    finish = 4;
    makeList();
    text.append("<br><br><b><u>FX1 Auto Wah***********</b></u>");
    address= "3F";
    start = 0;
    finish = 8;
    makeList();
    text.append("<br><br><b><u>FX1 Chorus***********</b></u>");
    address= "40";
    start = 0;
    finish = 25;
    makeList();
    text.append("<br><br><b><u>FX1 Classic Vibe***********</b></u>");
    address= "41";
    start = 0;
    finish = 3;
    makeList();
    text.append("<br><br><b><u>FX1 Compressor***********</b></u>");
    address= "42";
    start = 0;
    finish = 7;
    makeList();
    text.append("<br><br><b><u>FX1 Defretter***********</b></u>");
    address= "43";
    start = 0;
    finish = 7;
    makeList();
    text.append("<br><br><b><u>FX1 Defretter Bass***********</b></u>");
    address= "44";
    start = 0;
    finish = 5;
    makeList();
    text.append("<br><br><b><u>FX1 Delay***********</b></u>");
    address= "45";
    start = 1;
    finish = 33;
    makeList();
    text.append("<br><br><b><u>FX1 Flanger***********</b></u>");
    address= "46";
    start = 0;
    finish = 9;
    makeList();
    text.append("<br><br><b><u>FX1 Flanger Bass***********</b></u>");
    address= "47";
    start = 0;
    finish = 9;
    makeList();
    text.append("<br><br><b><u>FX1 Foot Volume***********</b></u>");
    address= "48";
    start = 0;
    finish = 13;
    makeList();
    text.append("<br><br><b><u>FX1 Graphic EQ***********</b></u>");
    address= "49";
    start = 0;
    finish = 8;
    makeList();
    text.append("<br><br><b><u>FX1 Harmonist***********</b></u>");
    address= "4A";
    start = 0;
    finish = 39;
    makeList();
    text.append("<br><br><b><u>FX1 Humanizer***********</b></u>");
    address= "4B";
    start = 0;
    finish = 8;
    makeList();
    text.append("<br><br><b><u>FX1 Isolator***********</b></u>");
    address= "4C";
    start = 0;
    finish = 4;
    makeList();
    text.append("<br><br><b><u>FX1 Limiter***********</b></u>");
    address= "4D";
    start = 0;
    finish = 6;
    makeList();
    text.append("<br><br><b><u>FX1 Low Fidelity***********</b></u>");
    address= "4E";
    start = 0;
    finish = 3;
    makeList();
    text.append("<br><br><b><u>FX1 Octave***********</b></u>");
    address= "4F";
    start = 0;
    finish = 3;
    makeList();
    text.append("<br><br><b><u>FX1 Octave Bass***********</b></u>");
    address= "50";
    start = 0;
    finish = 3;
    makeList();
    text.append("<br><br><b><u>FX1 Panner***********</b></u>");
    address= "51";
    start = 0;
    finish = 6;
    makeList();
    text.append("<br><br><b><u>FX1 Parametric EQ***********</b></u>");
    address= "52";
    start = 1;
    finish = 12;
    makeList();
    text.append("<br><br><b><u>FX1 Pedal Bend***********</b></u>");
    address= "53";
    start = 0;
    finish = 7;
    makeList();
    text.append("<br><br><b><u>FX1 Phaser***********</b></u>");
    address= "54";
    start = 0;
    finish = 8;
    makeList();
    text.append("<br><br><b><u>FX1 Pitch Shifter***********</b></u>");
    address= "55";
    start = 0;
    finish = 19;
    makeList();
    text.append("<br><br><b><u>FX1 Reverb***********</b></u>");
    address= "56";
    start = 1;
    finish = 14;
    makeList();
    text.append("<br><br><b><u>FX1 Ring Modulator***********</b></u>");
    address= "57";
    start = 0;
    finish = 6;
    makeList();
    text.append("<br><br><b><u>FX1 Rotary***********</b></u>");
    address= "58";
    start = 0;
    finish = 8;
    makeList();
    text.append("<br><br><b><u>FX1 Sitar Simulator***********</b></u>");
    address= "59";
    start = 0;
    finish = 7;
    makeList();
    text.append("<br><br><b><u>FX1 Slicer***********</b></u>");
    address= "5A";
    start = 0;
    finish = 7;
    makeList();
    text.append("<br><br><b><u>FX1 Slow Gear***********</b></u>");
    address= "5B";
    start = 0;
    finish = 3;
    makeList();
    text.append("<br><br><b><u>FX1 Slow Gear Bass***********</b></u>");
    address= "5C";
    start = 0;
    finish = 3;
    makeList();
    text.append("<br><br><b><u>FX1 Sound Hold***********</b></u>");
    address= "5D";
    start = 0;
    finish = 3;
    makeList();
    text.append("<br><br><b><u>FX1 T-Wah***********</b></u>");
    address= "5E";
    start = 0;
    finish = 8;
    makeList();
    text.append("<br><br><b><u>FX1 T-Wah Bass***********</b></u>");
    address= "5F";
    start = 0;
    finish = 8;
    makeList();
    text.append("<br><br><b><u>FX1 Tremolo***********</b></u>");
    address= "60";
    start = 0;
    finish = 5;
    makeList();
    text.append("<br><br><b><u>FX1 Vibrato***********</b></u>");
    address= "61";
    start = 0;
    finish = 6;
    makeList();
    text.append("<br><br><b><u>FX1 Wah***********</b></u>");
    address= "62";
    start = 0;
    finish = 15;
    makeList();
    large_text.append(text);
    if(effect == "on") { small_text.append(text); };
    this->filter = "off";


    this->effect = "off";
    text = "<br><br><b><u>FX 2***********</b></u>";
    address= "63";
    start = 0;
    finish = 2;
    makeList();
    text.append("<br><br><b><u>FX2 Acoustic Resonator***********</b></u>");
    address= "64";
    start = 0;
    finish = 4;
    makeList();
    text.append("<br><br><b><u>FX2 Auto Wah***********</b></u>");
    address= "65";
    start = 0;
    finish = 8;
    makeList();
    text.append("<br><br><b><u>FX2 Chorus***********</b></u>");
    address= "66";
    start = 0;
    finish = 25;
    makeList();
    text.append("<br><br><b><u>FX2 Classic Vibe***********</b></u>");
    address= "67";
    start = 0;
    finish = 3;
    makeList();
    text.append("<br><br><b><u>FX2 Compressor***********</b></u>");
    address= "68";
    start = 0;
    finish = 7;
    makeList();
    text.append("<br><br><b><u>FX2 Defretter***********</b></u>");
    address= "69";
    start = 0;
    finish = 7;
    makeList();
    text.append("<br><br><b><u>FX2 Defretter Bass***********</b></u>");
    address= "6A";
    start = 0;
    finish = 5;
    makeList();
    text.append("<br><br><b><u>FX2 Delay***********</b></u>");
    address= "6B";
    start = 1;
    finish = 33;
    makeList();
    text.append("<br><br><b><u>FX2 Flanger***********</b></u>");
    address= "6C";
    start = 0;
    finish = 9;
    makeList();
    text.append("<br><br><b><u>FX2 Flanger Bass***********</b></u>");
    address= "6D";
    start = 0;
    finish = 9;
    makeList();
    text.append("<br><br><b><u>FX2 Foot Volume***********</b></u>");
    address= "6E";
    start = 0;
    finish = 13;
    makeList();
    text.append("<br><br><b><u>FX2 Graphic EQ***********</b></u>");
    address= "6F";
    start = 0;
    finish = 8;
    makeList();
    text.append("<br><br><b><u>FX2 Harmonist***********</b></u>");
    address= "70";
    start = 0;
    finish = 39;
    makeList();
    text.append("<br><br><b><u>FX2 Humanizer***********</b></u>");
    address= "71";
    start = 0;
    finish = 8;
    makeList();
    text.append("<br><br><b><u>FX2 Isolator***********</b></u>");
    address= "72";
    start = 0;
    finish = 4;
    makeList();
    text.append("<br><br><b><u>FX2 Limiter***********</b></u>");
    address= "73";
    start = 0;
    finish = 6;
    makeList();
    text.append("<br><br><b><u>FX2 Low Fidelity***********</b></u>");
    address= "74";
    start = 0;
    finish = 3;
    makeList();
    text.append("<br><br><b><u>FX2 Octave***********</b></u>");
    address= "75";
    start = 0;
    finish = 3;
    makeList();
    text.append("<br><br><b><u>FX2 Octave Bass***********</b></u>");
    address= "76";
    start = 0;
    finish = 3;
    makeList();
    text.append("<br><br><b><u>FX2 Panner***********</b></u>");
    address= "77";
    start = 0;
    finish = 6;
    makeList();
    text.append("<br><br><b><u>FX2 Parametric EQ***********</b></u>");
    address= "78";
    start = 1;
    finish = 12;
    makeList();
    text.append("<br><br><b><u>FX2 Pedal Bend***********</b></u>");
    address= "79";
    start = 0;
    finish = 7;
    makeList();
    text.append("<br><br><b><u>FX2 Phaser***********</b></u>");
    address= "7A";
    start = 0;
    finish = 8;
    makeList();
    text.append("<br><br><b><u>FX2 Pitch Shifter***********</b></u>");
    address= "7B";
    start = 0;
    finish = 19;
    makeList();
    text.append("<br><br><b><u>FX2 Reverb***********</b></u>");
    address= "7C";
    start = 1;
    finish = 14;
    makeList();

    text.append("<br><br><b><u>FX2 Ring Modulator***********</b></u>");
    address= "7D";
    start = 0;
    finish = 6;
    makeList();
    text.append("<br><br><b><u>FX2 Rotary***********</b></u>");
    address= "7E";
    start = 0;
    finish = 8;
    makeList();
    text.append("<br><br><b><u>FX2 Sitar Simulator***********</b></u>");
    address= "7F";
    start = 0;
    finish = 7;
    makeList();

    hex1 = "01";
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){hex1 = "03";};

    text.append("<br><br><b><u>FX2 Slicer***********</b></u>");
    address= "00";
    start = 0;
    finish = 7;
    makeList();
    text.append("<br><br><b><u>FX2 Slow Gear***********</b></u>");
    address= "01";
    start = 0;
    finish = 3;
    makeList();
    text.append("<br><br><b><u>FX2 Slow Gear Bass***********</b></u>");
    address= "02";
    start = 0;
    finish = 3;
    makeList();
    text.append("<br><br><b><u>FX2 Sound Hold***********</b></u>");
    address= "03";
    start = 0;
    finish = 3;
    makeList();
    text.append("<br><br><b><u>FX2 T-Wah***********</b></u>");
    address= "04";
    start = 0;
    finish = 8;
    makeList();
    text.append("<br><br><b><u>FX2 T-Wah Bass***********</b></u>");
    address= "05";
    start = 0;
    finish = 8;
    makeList();
    text.append("<br><br><b><u>FX2 Tremolo***********</b></u>");
    address= "06";
    start = 0;
    finish = 5;
    makeList();
    text.append("<br><br><b><u>FX2 Vibrato***********</b></u>");
    address= "07";
    start = 0;
    finish = 6;
    makeList();
    text.append("<br><br><b><u>FX2 Wah***********</b></u>");
    address= "08";
    start = 0;
    finish = 15;
    makeList();
    large_text.append(text);
    if(effect == "on") { small_text.append(text); };
    this->filter = "off";


    this->effect = "off";
    text = "<br><br><b><u>FX 3***********</b></u>";
    address= "09";
    start = 0;
    finish = 2;
    makeList();
    text.append("<br><br><b><u>FX3 Acoustic Resonator***********</b></u>");
    address= "0A";
    start = 0;
    finish = 4;
    makeList();
    text.append("<br><br><b><u>FX3 Auto Wah***********</b></u>");
    address= "0B";
    start = 0;
    finish = 8;
    makeList();
    text.append("<br><br><b><u>FX3 Chorus***********</b></u>");
    address= "0C";
    start = 0;
    finish = 25;
    makeList();
    text.append("<br><br><b><u>FX3 Classic Vibe***********</b></u>");
    address= "0D";
    start = 0;
    finish = 3;
    makeList();
    text.append("<br><br><b><u>FX3 Compressor***********</b></u>");
    address= "0E";
    start = 0;
    finish = 7;
    makeList();
    text.append("<br><br><b><u>FX3 Defretter***********</b></u>");
    address= "0F";
    start = 0;
    finish = 7;
    makeList();
    text.append("<br><br><b><u>FX3 Defretter Bass***********</b></u>");
    address= "10";
    start = 0;
    finish = 5;
    makeList();
    text.append("<br><br><b><u>FX3 Delay***********</b></u>");
    address= "11";
    start = 1;
    finish = 33;
    makeList();
    text.append("<br><br><b><u>FX3 Flanger***********</b></u>");
    address= "12";
    start = 0;
    finish = 9;
    makeList();
    text.append("<br><br><b><u>FX3 Flanger Bass***********</b></u>");
    address= "13";
    start = 0;
    finish = 9;
    makeList();
    text.append("<br><br><b><u>FX3 Foot Volume***********</b></u>");
    address= "14";
    start = 0;
    finish = 13;
    makeList();
    text.append("<br><br><b><u>FX3 Graphic EQ***********</b></u>");
    address= "15";
    start = 0;
    finish = 8;
    makeList();
    text.append("<br><br><b><u>FX3 Harmonist***********</b></u>");
    address= "16";
    start = 0;
    finish = 39;
    makeList();
    text.append("<br><br><b><u>FX3 Humanizer***********</b></u>");
    address= "17";
    start = 0;
    finish = 8;
    makeList();
    text.append("<br><br><b><u>FX3 Isolator***********</b></u>");
    address= "18";
    start = 0;
    finish = 4;
    makeList();
    text.append("<br><br><b><u>FX3 Limiter***********</b></u>");
    address= "19";
    start = 0;
    finish = 6;
    makeList();
    text.append("<br><br><b><u>FX3 Low Fidelity***********</b></u>");
    address= "1A";
    start = 0;
    finish = 3;
    makeList();
    text.append("<br><br><b><u>FX3 Octave***********</b></u>");
    address= "1B";
    start = 0;
    finish = 3;
    makeList();
    text.append("<br><br><b><u>FX3 Octave Bass***********</b></u>");
    address= "1C";
    start = 0;
    finish = 3;
    makeList();
    text.append("<br><br><b><u>FX3 Panner***********</b></u>");
    address= "1D";
    start = 0;
    finish = 6;
    makeList();
    text.append("<br><br><b><u>FX3 Parametric EQ***********</b></u>");
    address= "1E";
    start = 1;
    finish = 12;
    makeList();
    text.append("<br><br><b><u>FX3 Pedal Bend***********</b></u>");
    address= "1F";
    start = 0;
    finish = 7;
    makeList();
    text.append("<br><br><b><u>FX3 Phaser***********</b></u>");
    address= "20";
    start = 0;
    finish = 8;
    makeList();
    text.append("<br><br><b><u>FX3 Pitch Shifter***********</b></u>");
    address= "21";
    start = 0;
    finish = 19;
    makeList();
    text.append("<br><br><b><u>FX3 Reverb***********</b></u>");
    address= "22";
    start = 1;
    finish = 14;
    makeList();
    text.append("<br><br><b><u>FX3 Ring Modulator***********</b></u>");
    address= "23";
    start = 0;
    finish = 6;
    makeList();
    text.append("<br><br><b><u>FX3 Rotary***********</b></u>");
    address= "24";
    start = 0;
    finish = 8;
    makeList();
    text.append("<br><br><b><u>FX3 Sitar Simulator***********</b></u>");
    address= "25";
    start = 0;
    finish = 7;
    makeList();
    text.append("<br><br><b><u>FX3 Slicer***********</b></u>");
    address= "26";
    start = 0;
    finish = 7;
    makeList();
    text.append("<br><br><b><u>FX3 Slow Gear***********</b></u>");
    address= "27";
    start = 0;
    finish = 3;
    makeList();
    text.append("<br><br><b><u>FX3 Slow Gear Bass***********</b></u>");
    address= "28";
    start = 0;
    finish = 3;
    makeList();
    text.append("<br><br><b><u>FX3 Sound Hold***********</b></u>");
    address= "29";
    start = 0;
    finish = 3;
    makeList();
    text.append("<br><br><b><u>FX3 T-Wah***********</b></u>");
    address= "2A";
    start = 0;
    finish = 8;
    makeList();
    text.append("<br><br><b><u>FX3 T-Wah Bass***********</b></u>");
    address= "2B";
    start = 0;
    finish = 8;
    makeList();
    text.append("<br><br><b><u>FX3 Tremolo***********</b></u>");
    address= "2C";
    start = 0;
    finish = 5;
    makeList();
    text.append("<br><br><b><u>FX3 Vibrato***********</b></u>");
    address= "2D";
    start = 0;
    finish = 6;
    makeList();
    text.append("<br><br><b><u>FX3 Wah***********</b></u>");
    address= "2E";
    start = 0;
    finish = 15;
    makeList();
    large_text.append(text);
    if(effect == "on") { small_text.append(text); };
    this->filter = "off";


    this->effect = "off";
    text = "<br><br><b><u>Reverb***********</b></u>";
    address= "2F";
    start = 0;
    finish = 14;
    makeList();
    large_text.append(text);
    if(effect == "on") { small_text.append(text); };

}
