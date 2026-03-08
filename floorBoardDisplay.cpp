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

#include <QChar>
#include <QComboBox>
#include <QLabel>
#include "floorBoardDisplay.h"
#include "Preferences.h"
#include "preferencesDialog.h"
//#include "MidiTable.h"
#include "SysxIO.h"
#include "midiIO.h"
#include "appservices.h"
#include "renameWidget.h"
//#include "customRenameWidget.h"
#include "globalVariables.h"
#include <QTimer>
#include <QDebug>
//#include <QtTest/QtTest>

// Platform-dependent sleep routines.
#ifdef Q_OS_WIN
#include <windows.h>
#define SLEEP( milliseconds ) Sleep( (DWORD) milliseconds )
#else // Unix variants & Mac
#include <unistd.h>
#define SLEEP( milliseconds ) usleep( (unsigned long) (milliseconds * 1000.0) )
#endif

namespace {
bool hasSy1000IdentityReply(const QString& sysxMsg)
{
    const QString msg = sysxMsg.trimmed().toUpper();
    return msg.startsWith("F07E") && msg.contains(idReplyPatern.toUpper()) && msg.endsWith("F7");
}
}


floorBoardDisplay::floorBoardDisplay(QWidget *parent, QPoint pos)
    : QWidget(parent)
{
    Preferences *preferences = &AppServices::instance().preferences();
    bool ok;
    const double ratio = preferences->getPreferences("Window", "Scale", "ratio").toDouble(&ok);
    const double fratio = preferences->getPreferences("Window", "Font", "ratio").toDouble(&ok);
    //QFont Lfont( "Arial", 16*fratio, QFont::Normal);
    QFont Mfont( "Arial", 9*fratio, QFont::Normal);
    const int headerFontPointSize = qMax(9, int(9 * fratio));
    const int headerTop = int(5 * ratio);
    const int headerHeight = int(34 * ratio);
    //QFont Sfont( "Arial", 8*fratio, QFont::Normal);

    this->pos = pos;
    this->timer = new QTimer(this);
    this->patchLoadError = false;
    this->blinkCount = 0;

    this->patchNumDisplay = new customDisplay(QRect(20*ratio, 5*ratio, 60*ratio, 34*ratio), this);
    this->patchNumDisplay->setLabelPosition(true);
    this->patchNumDisplay->setMainObjectName("bankMain");
    this->patchNumDisplay->setSubObjectName("bankSub");
    this->patchNumDisplay->setFrameVisible(false);
    this->patchNumDisplay->setAttribute(Qt::WA_TranslucentBackground, true);
    this->patchNumDisplay->setWhatsThis(tr("Patch Number Display.<br>displays the currently selected patch<br>and patch write memory location."));
    this->patchDisplay = new customDisplay(QRect(100*ratio, 5*ratio, 145*ratio, 34*ratio), this);
    this->patchDisplay->setMainObjectName("nameMain");
    this->patchDisplay->setSubObjectName("nameSub");
    this->patchDisplay->setFrameVisible(false);
    this->patchDisplay->setAttribute(Qt::WA_TranslucentBackground, true);

    /* this->temp1Display = new customLabelDisplay(QRect(28*ratio, tempRowOffset+(22*ratio), 166*ratio, 18*ratio), this);
    this->temp1Display->setLabelPosition(true);
    this->temp1Display->setMainObjectName("nameMain");
    this->temp1Display->setMainText(tr("Empty"), Qt::AlignCenter);
    this->temp1Display->setWhatsThis(tr("Name of the currently stored patch in the clipboard."));
    this->temp1Display->setFont(Lfont);
    this->temp2Display = new customLabelDisplay(QRect(222*ratio, tempRowOffset+(22*ratio), 166*ratio, 18*ratio), this);
    this->temp2Display->setLabelPosition(true);
    this->temp2Display->setMainObjectName("nameMain");
    this->temp2Display->setMainText(tr("Empty"), Qt::AlignCenter);
    this->temp2Display->setWhatsThis(tr("Name of the currently stored patch in the clipboard."));
    this->temp3Display = new customLabelDisplay(QRect(417*ratio, tempRowOffset+(22*ratio), 166*ratio, 18*ratio), this);
    this->temp3Display->setLabelPosition(true);
    this->temp3Display->setMainObjectName("nameMain");
    this->temp3Display->setMainText(tr("Empty"), Qt::AlignCenter);
    this->temp3Display->setWhatsThis(tr("Name of the currently stored patch in the clipboard."));
    this->temp4Display = new customLabelDisplay(QRect(611*ratio, tempRowOffset+(22*ratio), 166*ratio, 18*ratio), this);
    this->temp4Display->setLabelPosition(true);
    this->temp4Display->setMainObjectName("nameMain");
    this->temp4Display->setMainText(tr("Empty"), Qt::AlignCenter);
    this->temp4Display->setWhatsThis(tr("Name of the currently stored patch in the clipboard."));
    this->temp5Display = new customLabelDisplay(QRect(804*ratio, tempRowOffset+(22*ratio), 166*ratio, 18*ratio), this);
    this->temp5Display->setLabelPosition(true);
    this->temp5Display->setMainObjectName("nameMain");
    this->temp5Display->setMainText(tr("Empty"), Qt::AlignCenter);
    this->temp5Display->setWhatsThis(tr("Name of the currently stored patch in the clipboard."));
*/
    //Preferences *preferences = &AppServices::instance().preferences();
    QString version = preferences->getPreferences("General", "Application", "version");
    this->patchDisplay->setMainText(deviceType + tr(" FloorBoard"));
    this->patchDisplay->setSubText(tr("version"), version);

    initPatch = new initPatchListMenu(QRect(265*ratio, 5*ratio, 300*ratio, 34*ratio), this);
    initPatch->setFont(Mfont);
    initPatch->setFlatMode(true);
    initPatch->setAttribute(Qt::WA_TranslucentBackground, true);
    initPatch->setWhatsThis(tr("Clicking on this will load a patch from a predefined selection.<br>patches place in the init_patches folder will appear in this list at the start of the next session."));

    renameWidget *nameEdit = new renameWidget(this);
    nameEdit->setGeometry(100*ratio, 5*ratio, 150*ratio, 34*ratio);
    nameEdit->setWhatsThis(tr("Clicking on this will open<br>a text dialog window<br>allowing user text input."));

    this->writeDialog = new patchWriteDialog();
    this->writeDialog->hide();

    this->tuner_Button = new customPanelButton(tr("Tuner"), false, QPoint(570*ratio, 5*ratio), this, ":/images/switch.png");
    this->tuner_Button->setBackgroundVisible(false);
    this->tuner_Button->setLabelPointSize(headerFontPointSize);
    this->tuner_Button->setWhatsThis(tr("Deep editing of the selected effect<br>pressing this button will open an edit page<br>allowing detailed setting of this effects parameters."));

    this->setup_Button = new customPanelButton(tr("Input"), false, QPoint(620*ratio, 5*ratio), this, ":/images/switch.png");
    this->setup_Button->setBackgroundVisible(false);
    this->setup_Button->setLabelPointSize(headerFontPointSize);
    this->setup_Button->setWhatsThis(tr("Deep editing of the selected effect<br>pressing this button will open an edit page<br>allowing detailed setting of this effects parameters."));

    this->connectButton = new customButton(tr("Off Line/On Line"), false, QPoint(665*ratio, 5*ratio), this, ":/images/greenledbutton.png");
    this->connectButton->setBackgroundVisible(false);
    this->connectButton->setLabelPointSize(headerFontPointSize);
    this->connectButton->setWhatsThis(tr("Connect Button<br>used to establish a continuous midi connection<br>when lit green, the connection is valid"));

    this->writeButton = new customButton(tr("Write"), false, QPoint(750*ratio, 5*ratio), this, ":/images/ledbutton.png");
    this->writeButton->setBackgroundVisible(false);
    this->writeButton->setLabelPointSize(headerFontPointSize);
    this->writeButton->setWhatsThis(tr("Write Button<br>if the patch number displays [temp buffer]<br>the current patch is sent to the SY-1000 temporary memory only<br>or else the patch will be written to the displayed patch memory location."));

    this->system_Button = new customPanelButton(tr("System"), false, QPoint(840*ratio, 5*ratio), this, ":/images/switch.png");
    this->system_Button->setBackgroundVisible(false);
    this->system_Button->setLabelPointSize(headerFontPointSize);
    this->system_Button->setWhatsThis(tr("Deep editing of the selected effect<br>pressing this button will open an edit page<br>allowing detailed setting of this effects parameters."));

    this->pedal_Button = new customPanelButton(tr("Pedal"), false, QPoint(890*ratio, 5*ratio), this, ":/images/switch.png");
    this->pedal_Button->setBackgroundVisible(false);
    this->pedal_Button->setLabelPointSize(headerFontPointSize);
    this->pedal_Button->setWhatsThis(tr("Deep editing of the selected effect<br>pressing this button will open an edit page<br>allowing detailed setting of this effects parameters."));

    this->assign1_Button = new customPanelButton(tr("Assign"), false, QPoint(940*ratio, 5*ratio), this, ":/images/switch.png");
    this->assign1_Button->setBackgroundVisible(false);
    this->assign1_Button->setLabelPointSize(headerFontPointSize);
    this->assign1_Button->setWhatsThis(tr("Deep editing of the selected effect<br>pressing this button will open an edit page<br>allowing detailed setting of this effects parameters."));

    // Restore the SY-1000 top header label.
    this->deviceHeader = new QLabel(deviceType, this);
    QFont headerTitleFont("Arial", qMax(14, int(14 * fratio)), QFont::Bold);
    this->deviceHeader->setFont(headerTitleFont);
    this->deviceHeader->setAlignment(Qt::AlignCenter);
    this->deviceHeader->setAttribute(Qt::WA_TranslucentBackground, true);
    this->deviceHeader->setStyleSheet("background: transparent; color: #AEEFFF;");
    const QRect assignRect = this->assign1_Button->geometry();
    const int headerX = assignRect.right() + qRound(16 * ratio);
    const int headerW = qMax(qRound(95 * ratio), this->deviceHeader->fontMetrics().horizontalAdvance(deviceType) + qRound(14 * ratio));
    this->deviceHeader->setGeometry(headerX, headerTop, headerW, headerHeight);

    // Keep every top-row control on the same baseline and font sizing.
    auto alignHeaderWidget = [headerTop, headerHeight](QWidget *widget)
    {
        if(!widget)
        {
            return;
        }

        QRect g = widget->geometry();
        g.moveTop(headerTop);
        g.setHeight(headerHeight);
        widget->setGeometry(g);
    };

    auto normalizeDisplayFonts = [headerFontPointSize](customDisplay *display)
    {
        const QList<QLabel*> labels = display->findChildren<QLabel*>();
        for(QLabel *label : labels)
        {
            QFont font = label->font();
            font.setPointSize(headerFontPointSize);
            label->setFont(font);
        }
    };

    alignHeaderWidget(this->patchNumDisplay);
    alignHeaderWidget(this->patchDisplay);
    alignHeaderWidget(initPatch);
    alignHeaderWidget(this->tuner_Button);
    alignHeaderWidget(this->setup_Button);
    alignHeaderWidget(this->connectButton);
    alignHeaderWidget(this->writeButton);
    alignHeaderWidget(this->system_Button);
    alignHeaderWidget(this->pedal_Button);
    alignHeaderWidget(this->assign1_Button);
    alignHeaderWidget(this->deviceHeader);

    // Geometry changed above; reapply font sizes so each button recenters its label.
    this->tuner_Button->setLabelPointSize(headerFontPointSize);
    this->setup_Button->setLabelPointSize(headerFontPointSize);
    this->connectButton->setLabelPointSize(headerFontPointSize);
    this->writeButton->setLabelPointSize(headerFontPointSize);
    this->system_Button->setLabelPointSize(headerFontPointSize);
    this->pedal_Button->setLabelPointSize(headerFontPointSize);
    this->assign1_Button->setLabelPointSize(headerFontPointSize);

    normalizeDisplayFonts(this->patchNumDisplay);
    normalizeDisplayFonts(this->patchDisplay);
    if(QComboBox *quickLoadCombo = initPatch->findChild<QComboBox*>())
    {
        QFont comboFont = quickLoadCombo->font();
        comboFont.setPointSize(headerFontPointSize);
        quickLoadCombo->setFont(comboFont);
    }

    // Keep these controls visible in the main strip.
    this->tuner_Button->show();
    this->setup_Button->show();
    this->system_Button->show();
    this->deviceHeader->show();

    /*    this->temp1_copy_Button = new customButton(tr("Patch Copy"), false, QPoint(26*ratio, tempRowOffset), this, ":/images/pushbutton.png");
    this->temp1_copy_Button->setWhatsThis(tr("Copy current patch to clipboard<br>pressing this button will save the current patch to a clipboard<br>the clipboard is saved to file and is re-loaded<br>on the next session startup."));
    this->temp1_paste_Button = new customButton(tr("Patch Paste"), false, QPoint(112*ratio, tempRowOffset), this, ":/images/pushbutton.png");
    this->temp1_paste_Button->setWhatsThis(tr("Paste current patch from clipboard<br>pressing this button will load the current patch to a clipboard<br>the clipboard is re-loaded<br>from the previous session copy."));
    this->temp2_copy_Button = new customButton(tr("Patch Copy"), false, QPoint(222*ratio, tempRowOffset), this, ":/images/pushbutton.png");
    this->temp2_copy_Button->setWhatsThis(tr("Copy current patch to clipboard<br>pressing this button will save the current patch to a clipboard<br>the clipboard is saved to file and is re-loaded<br>on the next session startup."));
    this->temp2_paste_Button = new customButton(tr("Patch Paste"), false, QPoint(306*ratio, tempRowOffset), this, ":/images/pushbutton.png");
    this->temp2_paste_Button->setWhatsThis(tr("Paste current patch from clipboard<br>pressing this button will load the current patch to a clipboard<br>the clipboard is re-loaded<br>from the previous session copy."));
    this->temp3_copy_Button = new customButton(tr("Patch Copy"), false, QPoint(417*ratio, tempRowOffset), this, ":/images/pushbutton.png");
    this->temp3_copy_Button->setWhatsThis(tr("Copy current patch to clipboard<br>pressing this button will save the current patch to a clipboard<br>the clipboard is saved to file and is re-loaded<br>on the next session startup."));
    this->temp3_paste_Button = new customButton(tr("Patch Paste"), false, QPoint(501*ratio, tempRowOffset), this, ":/images/pushbutton.png");
    this->temp3_paste_Button->setWhatsThis(tr("Paste current patch from clipboard<br>pressing this button will load the current patch to a clipboard<br>the clipboard is re-loaded<br>from the previous session copy."));
    this->temp4_copy_Button = new customButton(tr("Patch Copy"), false, QPoint(612*ratio, tempRowOffset), this, ":/images/pushbutton.png");
    this->temp4_copy_Button->setWhatsThis(tr("Copy current patch to clipboard<br>pressing this button will save the current patch to a clipboard<br>the clipboard is saved to file and is re-loaded<br>on the next session startup."));
    this->temp4_paste_Button = new customButton(tr("Patch Paste"), false, QPoint(696*ratio, tempRowOffset), this, ":/images/pushbutton.png");
    this->temp4_paste_Button->setWhatsThis(tr("Paste current patch from clipboard<br>pressing this button will load the current patch to a clipboard<br>the clipboard is re-loaded<br>from the previous session copy."));
    this->temp5_copy_Button = new customButton(tr("Patch Copy"), false, QPoint(805*ratio, tempRowOffset), this, ":/images/pushbutton.png");
    this->temp5_copy_Button->setWhatsThis(tr("Copy current patch to clipboard<br>pressing this button will save the current patch to a clipboard<br>the clipboard is saved to file and is re-loaded<br>on the next session startup."));
    this->temp5_paste_Button = new customButton(tr("Patch Paste"), false, QPoint(889*ratio, tempRowOffset), this, ":/images/pushbutton.png");
    this->temp5_paste_Button->setWhatsThis(tr("Paste current patch from clipboard<br>pressing this button will load the current patch to a clipboard<br>the clipboard is re-loaded<br>from the previous session copy."));
*/
    SysxIO *sysxIO = &AppServices::instance().sysx();
    QObject::connect(this, &floorBoardDisplay::setStatusSymbol, sysxIO, &SysxIO::setStatusSymbol);
    QObject::connect(this, &floorBoardDisplay::setStatusProgress, sysxIO, &SysxIO::setStatusProgress);
    QObject::connect(this, &floorBoardDisplay::setStatusMessage, sysxIO, &SysxIO::setStatusMessage);

    QObject::connect(sysxIO, &SysxIO::notConnectedSignal, this, &floorBoardDisplay::notConnected);
    QObject::connect(this, &floorBoardDisplay::notConnectedSignal, this, &floorBoardDisplay::notConnected);

    QObject::connect(this->parent(), SIGNAL(updateSignal()), this, SLOT(updateDisplay()));
    QObject::connect(this, SIGNAL(updateSignal()), this->parent(), SIGNAL(updateSignal()));

    QObject::connect(this->connectButton, qOverload<bool>(&customButton::valueChanged), this, &floorBoardDisplay::connectSignal);
    QObject::connect(this->writeButton, qOverload<bool>(&customButton::valueChanged), this, &floorBoardDisplay::writeSignal);

    QObject::connect(this->assign1_Button, SIGNAL(valueChanged(bool)), this->parent(), SIGNAL(assign1_buttonSignal(bool)));
    QObject::connect(this->parent(), SIGNAL(assign1_statusSignal(bool)), this->assign1_Button, SLOT(setValue(bool)));
    QObject::connect(this->tuner_Button, SIGNAL(valueChanged(bool)), this->parent(), SIGNAL(tuner_buttonSignal(bool)));
    QObject::connect(this->setup_Button, SIGNAL(valueChanged(bool)), this->parent(), SIGNAL(setup_buttonSignal(bool)));
    QObject::connect(this->pedal_Button, SIGNAL(valueChanged(bool)), this->parent(), SIGNAL(pedal_buttonSignal(bool)));
    QObject::connect(this->system_Button, SIGNAL(valueChanged(bool)), this->parent(), SIGNAL(system_buttonSignal(bool)));

    //QObject::connect(this->parent(), SIGNAL(assign1_statusSignal(bool)), this->assign1_Button, SLOT(setValue(bool)));

    /* QObject::connect(this->temp1_copy_Button, SIGNAL(valueChanged(bool)),  this, SLOT(temp1_copy(bool)));
    QObject::connect(this->temp1_paste_Button, SIGNAL(valueChanged(bool)), this, SLOT(temp1_paste(bool)));
    QObject::connect(this->temp2_copy_Button, SIGNAL(valueChanged(bool)),  this, SLOT(temp2_copy(bool)));
    QObject::connect(this->temp2_paste_Button, SIGNAL(valueChanged(bool)), this, SLOT(temp2_paste(bool)));
    QObject::connect(this->temp3_copy_Button, SIGNAL(valueChanged(bool)),  this, SLOT(temp3_copy(bool)));
    QObject::connect(this->temp3_paste_Button, SIGNAL(valueChanged(bool)), this, SLOT(temp3_paste(bool)));
    QObject::connect(this->temp4_copy_Button, SIGNAL(valueChanged(bool)),  this, SLOT(temp4_copy(bool)));
    QObject::connect(this->temp4_paste_Button, SIGNAL(valueChanged(bool)), this, SLOT(temp4_paste(bool)));
    QObject::connect(this->temp5_copy_Button, SIGNAL(valueChanged(bool)),  this, SLOT(temp5_copy(bool)));
    QObject::connect(this->temp5_paste_Button, SIGNAL(valueChanged(bool)), this, SLOT(temp5_paste(bool)));

*/
    QObject::connect(this, SIGNAL(updateNameSignal(QString,int,int)), this->parent(), SIGNAL(updateNameSignal(QString,int,int)));
    //   set_temp();
    if(preferences->getPreferences("Midi", "DBug", "bool")=="true") sysxIO->deBug(QString("Auto connect initiate"));

    autoconnect();
}

floorBoardDisplay::~floorBoardDisplay()
{

}

QPoint floorBoardDisplay::getPos()
{
    return this->pos;
}

void floorBoardDisplay::setPos(QPoint newPos)
{
    this->move(newPos);
    this->pos = newPos;
}

void floorBoardDisplay::setValueDisplay(QString fxName, QString valueName, QString value)
{
    this->valueDisplay->setMainText(fxName);
    this->valueDisplay->setSubText(valueName, value);
}

void floorBoardDisplay::setPatchDisplay(QString patchName)
{
    SysxIO *sysxIO = &AppServices::instance().sysx();
    if(sysxIO->getFileName() != ":/default_guitar.syx" && sysxIO->getFileName() != ":/default_bass.syx") // Keep the initial version display if the default.syx is loaded at startup.
    {
        QString fileName = sysxIO->getFileName();
        this->patchDisplay->setMainText(patchName);
        this->patchDisplay->setSubText(fileName.section('/', -1, -1));
        this->patchName = patchName;
    };
    if(sysxIO->getFileName() == tr("init patch") || sysxIO->getFileName() == ":/default_guitar.syx" || sysxIO->getFileName() == ":/default_bass.syx")
    {
        sysxIO->setFileName("");
        this->patchName = tr("INIT PATCH");
    }
    else
    {
        if(sysxIO->getFileName() == deviceType + tr(" patch"))
        {
            sysxIO->setFileName("");
            if(this->patchLoadError)
            {
                QMessageBox *msgBox = new QMessageBox();
                msgBox->setWindowTitle(deviceType + tr(" FloorBoard"));
                msgBox->setIcon(QMessageBox::Warning);
                msgBox->setTextFormat(Qt::RichText);
                QString msgText;
                msgText.append("<font size='+1'><b>");
                msgText.append(tr("Error while changing banks."));
                msgText.append("<b></font><br>");
                msgText.append(tr("An incorrect patch has been loaded. Please try to load the patch again."));
                msgBox->setText(msgText);
                msgBox->show();
                QTimer::singleShot(3000, msgBox, SLOT(deleteLater()));

                sysxIO->setBank(0);
                sysxIO->setPatch(0);
            };
        };
        //this->initPatch->setIndex(0);
    };
}

void floorBoardDisplay::setPatchNumDisplay(int bank, int patch)
{
    if(bank > 0)
    {
        if(bank <= bankTotalUser)
        {
            this->patchNumDisplay->resetAllColor();
            this->patchNumDisplay->setSubText(tr("User"));
        }
        else
        {
            this->patchNumDisplay->setSubText(tr("Preset"));
        };

        QString str;
        if(bank < 51)
        {
            str.append("U");
            if(bank < 10) {	str.append("0"); };
            str.append(QString::number(bank, 10));
        }
        else
        {
            str.append("P");
            if(bank < 60) {	str.append("0"); };
            str.append(QString::number(bank-50, 10));
        };
        str.append(":");
        str.append(QString::number(patch, 10));
        this->patchNumDisplay->setMainText(str, Qt::AlignCenter);
    }
    else
    {
        this->patchNumDisplay->setSubText(tr("Temp"));
        QString str = tr("Buffer");
        this->patchNumDisplay->setMainText(str, Qt::AlignCenter);
    };
}

void floorBoardDisplay::updateDisplay()
{
    SysxIO *sysxIO = &AppServices::instance().sysx();
    Preferences *preferences = &AppServices::instance().preferences();
    QString hex1 = "00";
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){hex1 = "02";};
    QList<QString> nameArray = sysxIO->getFileSource("10", hex1, "00", "00");
    QString name;
    for(int i=sysxNameOffset;i<(sysxNameOffset+nameLength);i++ )
    {

        QString hexStr = nameArray.at(i);
        bool ok;
        name.append( char(hexStr.toInt(&ok, 16)) );
    };

    QString patchName = name.trimmed();
    sysxIO->setCurrentPatchName(patchName);
    if(sysxIO->getRequestName() != patchName)
    {
        this->patchLoadError = true;
    }
    else
    {
        this->patchLoadError = false;
    };


    this->setPatchDisplay(patchName);
    if(sysxIO->isDevice())
    {
        int bank = sysxIO->getBank();
        int patch = sysxIO->getPatch();
        this->setPatchNumDisplay(bank, patch);
    }
    else
    {
        this->patchNumDisplay->clearAll();
    };
    // this->valueDisplay->clearAll();

    if(sysxIO->isDevice() )  // comment out from here
    {
        if(sysxIO->getBank() > 50)
        {
            this->writeButton->setBlink(false);
            this->writeButton->setValue(true);
        }
        else
        {
            this->writeButton->setBlink(false);
            this->writeButton->setValue(true);
        };
        int bank = sysxIO->getBank();
        int patch = sysxIO->getPatch();
        this->setPatchNumDisplay(bank, patch);
    }
    else if(sysxIO->getBank() != 0)
    {
        if(sysxIO->isConnected())
        {
            this->writeButton->setBlink(false);
            this->writeButton->setValue(false);
        };
        int bank = sysxIO->getBank();
        int patch = sysxIO->getPatch();
        this->setPatchNumDisplay(bank, patch);
    }
    else
    {
        patchNumDisplay->clearAll();
        this->writeButton->setBlink(false);
        this->writeButton->setValue(false);
    };
}

void floorBoardDisplay::autoconnect()
{
    SysxIO *sysxIO = &AppServices::instance().sysx();
    if(sysxIO->deviceReady())
    {
        emit setStatusSymbol(2);
        emit setStatusMessage(tr("Connecting"));

        this->connectButton->setBlink(true);
        sysxIO->setDeviceReady(false); // Reserve the device for interaction.

        QObject::disconnect(sysxIO, SIGNAL(sysxReply(QString)));
        QObject::connect(sysxIO, SIGNAL(sysxReply(QString)), this, SLOT(autoConnectionResult(QString)));

        sysxIO->sysxOutput(idRequestString, 15);
    }
    else
    {
        notConnected();
        sysxIO->setNoError(true);		// Reset the error status (else we could never retry :) ).
    };
}

void floorBoardDisplay::autoConnectionResult(QString sysxMsg)
{
    Preferences *preferences = &AppServices::instance().preferences(); // Load the preferences.
    SysxIO *sysxIO = &AppServices::instance().sysx();
    QObject::disconnect(sysxIO, SIGNAL(sysxReply(QString)),
                        this, SLOT(autoConnectionResult(QString)));

    sysxIO->setDeviceReady(true); // Free the device after finishing interaction.
    const bool identityOk = hasSy1000IdentityReply(sysxMsg);
    if(identityOk)
    {
        if(preferences->getPreferences("Midi", "DBug", "bool")=="true")
        {
            sysxIO->deBug(QString("Auto connect succeeded, identity reply bytes=" + QString::number(sysxMsg.size()/2, 10)));
        };
        this->connectButton->setBlink(false);
        this->connectButton->setValue(true);
        sysxIO->setConnected(true);
        emit connectedSignal();
    }
    else
    {
        qWarning("CONNECT_AUTO_FAIL no valid identity reply bytes=%d", int(sysxMsg.size()/2));
        this->connectButton->setBlink(false);
        this->connectButton->setValue(false);
        sysxIO->setConnected(false);
        emit notConnectedSignal();
    };
}

/*void floorBoardDisplay::set_temp()
{   
      SysxIO *sysxIO = &AppServices::instance().sysx();
    QByteArray data;

    QFile file1("temp-1.syx");   // Read the sysx file .
    if (file1.open(QIODevice::ReadOnly))
    {	data = file1.readAll(); };

    QString sysxBuffer;
    for(int i=0;i<data.size();i++)
    {
        uchar byte = uchar(data[i]);
        int n = int(byte);
        QString hex = QString::number(n, 16).toUpper();     // convert QByteArray to QString
        if (hex.length() < 2) hex.prepend("0");
        sysxBuffer.append(hex);
    };
    if( data.size() == fullPatchSize)
    {
        QString patchText;
        uchar r;
        int a = sysxNameOffset; // locate patch text start position from the start of the file
        for (int b=0;b<nameLength;b++)
        {
            r = uchar(data[a+b]);
            patchText.append(r);         // extract the text characters from the current patch name.
        };
        this->temp1Display->setMainText(patchText, Qt::AlignCenter);
        sysxIO->temp1_sysxMsg = sysxBuffer;
    };

    data.clear();
    QFile file2("temp-2.syx");   // Read the sysx file .
    if (file2.open(QIODevice::ReadOnly))
    {	data = file2.readAll(); };

    sysxBuffer.clear();
    for(int i=0;i<data.size();i++)
    {
        uchar byte = uchar(data[i]);
        uint n = uint(byte);
        QString hex = QString::number(n, 16).toUpper();     // convert QByteArray to QString
        if (hex.length() < 2) hex.prepend("0");
        sysxBuffer.append(hex);
    };
    if( data.size() == fullPatchSize)
    {
        QString patchText;
        uchar r;
        int a = sysxNameOffset; // locate patch text start position from the start of the file
        for (int b=0;b<nameLength;b++)
        {
            r = uchar(data[a+b]);
            patchText.append(r);         // extract the text characters from the current patch name.
        };
        this->temp2Display->setMainText(patchText, Qt::AlignCenter);
        sysxIO->temp2_sysxMsg = sysxBuffer;
    };

    data.clear();
    QFile file3("temp-3.syx");   // Read the default GT-3 sysx file so we don't start empty handed.
    if (file3.open(QIODevice::ReadOnly))
    {	data = file3.readAll(); };


    sysxBuffer.clear();
    for(int i=0;i<data.size();i++)
    {
        uchar byte = uchar(data[i]);
        int n = int(byte);
        QString hex = QString::number(n, 16).toUpper();     // convert QByteArray to QString
        if (hex.length() < 2) hex.prepend("0");
        sysxBuffer.append(hex);
    };
    if( data.size() == fullPatchSize)
    {
        QString patchText;
        uchar r;
        int a = sysxNameOffset; // locate patch text start position from the start of the file
        for (int b=0;b<nameLength;b++)
        {
            r = uchar(data[a+b]);
            patchText.append(r);         // extract the text characters from the current patch name.
        };
        this->temp3Display->setMainText(patchText, Qt::AlignCenter);
        sysxIO->temp3_sysxMsg = sysxBuffer;
    };

    data.clear();
    QFile file4("temp-4.syx");   // Read the sysx file .
    if (file4.open(QIODevice::ReadOnly))
    {	data = file4.readAll(); };

    sysxBuffer.clear();
    for(int i=0;i<data.size();i++)
    {
        uchar byte = uchar(data[i]);
        int n = int(byte);
        QString hex = QString::number(n, 16).toUpper();     // convert QByteArray to QString
        if (hex.length() < 2) hex.prepend("0");
        sysxBuffer.append(hex);
    };
    if( data.size() == fullPatchSize)

    {
        QString patchText;
        uchar r;
        int a = sysxNameOffset; // locate patch text start position from the start of the file
        for (int b=0;b<nameLength;b++)
        {
            r = uchar(data[a+b]);
            patchText.append(r);         // extract the text characters from the current patch name.
        };
        this->temp4Display->setMainText(patchText, Qt::AlignCenter);
        sysxIO->temp4_sysxMsg = sysxBuffer;
    };

    data.clear();
    QFile file5("temp-5.syx");   // Read the sysx file .
    if (file5.open(QIODevice::ReadOnly))
    {	data = file5.readAll(); };

    sysxBuffer.clear();
    for(int i=0;i<data.size();i++)
    {
        uchar byte = uchar (data[i]);
        uint n = uint(byte);
        QString hex = QString::number(n, 16).toUpper();     // convert QByteArray to QString
        if (hex.length() < 2) hex.prepend("0");
        sysxBuffer.append(hex);
    };
    if( data.size() == fullPatchSize)

    {
        QString patchText;
        uchar r;
        int a = sysxNameOffset; // locate patch text start position from the start of the file
        for (int b=0;b<nameLength;b++)
        {
            r = uchar(data[a+b]);
            patchText.append(r);         // extract the text characters from the current patch name.
        };
        this->temp5Display->setMainText(patchText, Qt::AlignCenter);
        sysxIO->temp5_sysxMsg = sysxBuffer;
    };
}*/

/*void floorBoardDisplay::temp1_copy(bool value)
{
    Q_UNUSED(value);
    SysxIO *sysxIO = &AppServices::instance().sysx();

    QString sysxMsg;
    QList< QList<QString> > patchData = sysxIO->getFileSource().hex; // Get the loaded patch data.
    QString addr1 = tempBulkWrite;  // temp address
    QString addr2 = QString::number(0, 16).toUpper();

    for(int i=0;i<patchData.size();++i)
    {
        QList<QString> data = patchData.at(i);
        for(int x=0;x<data.size();++x)
        {
            QString hex;
            if(x == sysxAddressOffset)
            { hex = addr1; }
            else if(x == sysxAddressOffset + 1)
            {	hex = addr2; }
            else
            {	hex = data.at(x);	};
            if (hex.length() < 2) hex.prepend("0");
            sysxMsg.append(hex);
        };
    };
    if( sysxMsg.size()/2 == fullPatchSize)
    {
        this->patchName = sysxIO->getCurrentPatchName();
        this->temp1Display->setMainText(patchName, Qt::AlignCenter);
        sysxIO->temp1_sysxMsg = sysxMsg;
        save_temp("temp-1.syx", sysxMsg);
    } else {
        QApplication::beep();
        QString size = QString::number(sysxMsg.size()/2, 10);
        sysxIO->emitStatusdBugMessage(tr("in-consistant patch data detected ") + size + tr("bytes: re-save or re-load file to correct"));
    };
}

void floorBoardDisplay::temp1_paste(bool value)
{
    Q_UNUSED(value);
    SysxIO *sysxIO = &AppServices::instance().sysx();
    this->temp1_sysxMsg = sysxIO->temp1_sysxMsg;
    if (!temp1_sysxMsg.isEmpty() && sysxIO->deviceReady() )
    {
        SysxIO *sysxIO = &AppServices::instance().sysx();
        sysxIO->setFileSource("patch", temp1_sysxMsg);
        emit updateSignal();
        if(sysxIO->isConnected()) { sysxIO->writeToBuffer(); };
    }
    else
    {
        QApplication::beep();
        sysxIO->emitStatusdBugMessage(tr("patch must be copied to clipboard first"));
    };
}

void floorBoardDisplay::temp2_copy(bool value)
{
    Q_UNUSED(value);
    SysxIO *sysxIO = &AppServices::instance().sysx();

    QString sysxMsg;
    QList< QList<QString> > patchData = sysxIO->getFileSource().hex; // Get the loaded patch data.
    QString addr1 = tempBulkWrite;  // temp address
    QString addr2 = QString::number(0, 16).toUpper();

    for(int i=0;i<patchData.size();++i)
    {
        QList<QString> data = patchData.at(i);
        for(int x=0;x<data.size();++x)
        {
            QString hex;
            if(x == sysxAddressOffset)
            { hex = addr1; }
            else if(x == sysxAddressOffset + 1)
            {	hex = addr2; }
            else
            {	hex = data.at(x);	};
            if (hex.length() < 2) hex.prepend("0");
            sysxMsg.append(hex);
        };
    };
    if( sysxMsg.size()/2 == fullPatchSize)
    {
        this->patchName = sysxIO->getCurrentPatchName();
        this->temp2Display->setMainText(patchName, Qt::AlignCenter);
        sysxIO->temp2_sysxMsg = sysxMsg;
        save_temp("temp-2.syx", sysxMsg);
    } else {
        QApplication::beep();
        QString size = QString::number(sysxMsg.size()/2, 10);
        sysxIO->emitStatusdBugMessage(tr("in-consistant patch data detected ") + size + tr("bytes: re-save or re-load file to correct"));
    };
}

void floorBoardDisplay::temp2_paste(bool value)
{
    Q_UNUSED(value);
    SysxIO *sysxIO = &AppServices::instance().sysx();
    this->temp2_sysxMsg = sysxIO->temp2_sysxMsg;
    if (!temp2_sysxMsg.isEmpty() && sysxIO->deviceReady() )
    {
        SysxIO *sysxIO = &AppServices::instance().sysx();
        sysxIO->setFileSource("patch", temp2_sysxMsg);
        emit updateSignal();
        if(sysxIO->isConnected()) { sysxIO->writeToBuffer(); };
    }else
    {
        QApplication::beep();
        sysxIO->emitStatusdBugMessage(tr("patch must be copied to clipboard first"));
    };
}

void floorBoardDisplay::temp3_copy(bool value)
{
    Q_UNUSED(value);
    SysxIO *sysxIO = &AppServices::instance().sysx();

    QString sysxMsg;
    QList< QList<QString> > patchData = sysxIO->getFileSource().hex; // Get the loaded patch data.
    QString addr1 = tempBulkWrite;  // temp address
    QString addr2 = QString::number(0, 16).toUpper();

    for(int i=0;i<patchData.size();++i)
    {
        QList<QString> data = patchData.at(i);
        for(int x=0;x<data.size();++x)
        {
            QString hex;
            if(x == sysxAddressOffset)
            { hex = addr1; }
            else if(x == sysxAddressOffset + 1)
            {	hex = addr2; }
            else
            {	hex = data.at(x);	};
            if (hex.length() < 2) hex.prepend("0");
            sysxMsg.append(hex);
        };
    };
    if( sysxMsg.size()/2 == fullPatchSize)
    {
        this->patchName = sysxIO->getCurrentPatchName();
        this->temp3Display->setMainText(patchName, Qt::AlignCenter);
        sysxIO->temp3_sysxMsg = sysxMsg;
        save_temp("temp-3.syx", sysxMsg);
    } else {
        QApplication::beep();
        QString size = QString::number(sysxMsg.size()/2, 10);
        sysxIO->emitStatusdBugMessage(tr("in-consistant patch data detected ") + size + tr("bytes: re-save or re-load file to correct"));
    };
}

void floorBoardDisplay::temp3_paste(bool value)
{
    Q_UNUSED(value);
    SysxIO *sysxIO = &AppServices::instance().sysx();
    this->temp3_sysxMsg = sysxIO->temp3_sysxMsg;
    if (!temp3_sysxMsg.isEmpty() && sysxIO->deviceReady() )
    {
        SysxIO *sysxIO = &AppServices::instance().sysx();
        sysxIO->setFileSource("patch", temp3_sysxMsg);
        emit updateSignal();
        if(sysxIO->isConnected()) { sysxIO->writeToBuffer(); };
    }else
    {
        QApplication::beep();
        sysxIO->emitStatusdBugMessage(tr("patch must be copied to clipboard first"));
    };
}

void floorBoardDisplay::temp4_copy(bool value)
{
    Q_UNUSED(value);
    SysxIO *sysxIO = &AppServices::instance().sysx();

    QString sysxMsg;
    QList< QList<QString> > patchData = sysxIO->getFileSource().hex; // Get the loaded patch data.
    QString addr1 = tempBulkWrite;  // temp address
    QString addr2 = QString::number(0, 16).toUpper();

    for(int i=0;i<patchData.size();++i)
    {
        QList<QString> data = patchData.at(i);
        for(int x=0;x<data.size();++x)
        {
            QString hex;
            if(x == sysxAddressOffset)
            { hex = addr1; }
            else if(x == sysxAddressOffset + 1)
            {	hex = addr2; }
            else
            {	hex = data.at(x);	};
            if (hex.length() < 2) hex.prepend("0");
            sysxMsg.append(hex);
        };
    };
    if( sysxMsg.size()/2 == fullPatchSize)
    {
        this->patchName = sysxIO->getCurrentPatchName();
        this->temp4Display->setMainText(patchName, Qt::AlignCenter);
        sysxIO->temp4_sysxMsg = sysxMsg;
        save_temp("temp-4.syx", sysxMsg);
    } else {
        QApplication::beep();
        QString size = QString::number(sysxMsg.size()/2, 10);
        sysxIO->emitStatusdBugMessage(tr("in-consistant patch data detected ") + size + tr("bytes: re-save or re-load file to correct"));
    };
}

void floorBoardDisplay::temp4_paste(bool value)
{
    Q_UNUSED(value);
    SysxIO *sysxIO = &AppServices::instance().sysx();
    this->temp4_sysxMsg = sysxIO->temp4_sysxMsg;
    if (!temp4_sysxMsg.isEmpty() && sysxIO->deviceReady() )
    {
        SysxIO *sysxIO = &AppServices::instance().sysx();
        sysxIO->setFileSource("patch", temp4_sysxMsg);
        emit updateSignal();
        if(sysxIO->isConnected()) { sysxIO->writeToBuffer(); };
    }else
    {
        QApplication::beep();
        sysxIO->emitStatusdBugMessage(tr("patch must be copied to clipboard first"));
    };
}

void floorBoardDisplay::temp5_copy(bool value)
{
    Q_UNUSED(value);
    SysxIO *sysxIO = &AppServices::instance().sysx();

    QString sysxMsg;
    QList< QList<QString> > patchData = sysxIO->getFileSource().hex; // Get the loaded patch data.
    QString addr1 = tempBulkWrite;  // temp address
    QString addr2 = QString::number(0, 16).toUpper();

    for(int i=0;i<patchData.size();++i)
    {
        QList<QString> data = patchData.at(i);
        for(int x=0;x<data.size();++x)
        {
            QString hex;
            if(x == sysxAddressOffset)
            { hex = addr1; }
            else if(x == sysxAddressOffset + 1)
            {	hex = addr2; }
            else
            {	hex = data.at(x);	};
            if (hex.length() < 2) hex.prepend("0");
            sysxMsg.append(hex);
        };
    };
    if( sysxMsg.size()/2 == fullPatchSize)
    {
        this->patchName = sysxIO->getCurrentPatchName();
        this->temp5Display->setMainText(patchName, Qt::AlignCenter);
        sysxIO->temp5_sysxMsg = sysxMsg;
        save_temp("temp-5.syx", sysxMsg);
    } else {
        QApplication::beep();
        QString size = QString::number(sysxMsg.size()/2, 10);
        sysxIO->emitStatusdBugMessage(tr("in-consistant patch data detected ") + size + tr("bytes: re-save or re-load file to correct"));
    };
}

void floorBoardDisplay::temp5_paste(bool value)
{
    Q_UNUSED(value);
    SysxIO *sysxIO = &AppServices::instance().sysx();
    this->temp5_sysxMsg = sysxIO->temp5_sysxMsg;
    if (!temp5_sysxMsg.isEmpty() && sysxIO->deviceReady() )
    {
        SysxIO *sysxIO = &AppServices::instance().sysx();
        sysxIO->setFileSource("patch", temp5_sysxMsg);
        emit updateSignal();
        if(sysxIO->isConnected()) { sysxIO->writeToBuffer(); };
    }else
    {
        QApplication::beep();
        sysxIO->emitStatusdBugMessage(tr("patch must be copied to clipboard first"));
    };
}

void floorBoardDisplay::save_temp(QString fileName, QString sysxMsg)
{
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly))
    {
        QByteArray out;
        uint count=0;
        QString data = sysxMsg;
        int x = data.size()/2;
        for (int a=0;a<x;++a)
        {
            QString str = data.at(a*2);
            str.append(data.at((a*2)+1));
            bool ok;
            int n = str.toInt(&ok, 16);
            out[count] = char(n);
            count++;
        };
        file.write(out);
    };
}*/

void floorBoardDisplay::connectSignal(bool value)
{
    SysxIO *sysxIO = &AppServices::instance().sysx();
    this->connectButtonActive = value;
    if(connectButtonActive == true && sysxIO->deviceReady())
    {
        emit setStatusSymbol(2);
        emit setStatusMessage(tr("Connecting"));
        this->connectButton->setBlink(true);
        sysxIO->setDeviceReady(false); // Reserve the device for interaction.

        QObject::disconnect(sysxIO, SIGNAL(sysxReply(QString)));
        QObject::connect(sysxIO, SIGNAL(sysxReply(QString)),
                         this, SLOT(connectionResult(QString)));

        sysxIO->sysxOutput(idRequestString, 15); // SY-1000 Identity Request.
    }
    else
    {
        notConnected();
        sysxIO->setNoError(true);		// Reset the error status (else we could never retry :) ).
    };
}

void floorBoardDisplay::connectionResult(QString sysxMsg)
{
    SysxIO *sysxIO = &AppServices::instance().sysx();
    QObject::disconnect(sysxIO, SIGNAL(sysxReply(QString)),
                        this, SLOT(connectionResult(QString)));

    sysxIO->setDeviceReady(true); // Free the device after finishing interaction.

    //DeBugGING OUTPUT
    Preferences *preferences = &AppServices::instance().preferences(); // Load the preferences.
    const bool identityOk = hasSy1000IdentityReply(sysxMsg);
    if(sysxIO->noError())
    {
        if(identityOk && connectButtonActive == true)
        {
            this->connectButton->setBlink(false);
            this->connectButton->setValue(true);
            sysxIO->setConnected(true);
            emit connectedSignal();
            qWarning("CONNECT_OK identity reply bytes=%d", int(sysxMsg.size()/2));

            if(sysxIO->getBank() != 0)
            {
                this->writeButton->setBlink(false);
                this->writeButton->setValue(false);
            };
        }
        else if(!sysxMsg.isEmpty())
        {
            this->connectButton->setBlink(false);
            this->connectButton->setValue(false);            
            sysxIO->setMode("mode0");
            sysxIO->setConnected(false);
            qWarning("CONNECT_FAIL wrong device reply bytes=%d", int(sysxMsg.size()/2));
            AppServices::instance().midi().closePorts();

            QMessageBox *msgBox = new QMessageBox();
            msgBox->setWindowTitle(deviceType + tr(" FloorBoard connection Error !!"));
            msgBox->setIcon(QMessageBox::Warning);
            msgBox->setTextFormat(Qt::RichText);
            QString msgText;
            msgText.append("<font size='+1'><b>");
            msgText.append(tr("The device connected is not a Boss ") + deviceType + tr(" Guitar Synthesizer."));
            if (sysxMsg.contains(idRequestString))
            {msgText.append(tr("<br>Midi loopback detected, ensure midi device 'thru' is switched off.")); };
            msgText.append("<b></font>");
            msgBox->setText(msgText);
            msgBox->show();
            QTimer::singleShot(3000, msgBox, SLOT(deleteLater()));
            notConnected();

            emit setStatusSymbol(0);
            emit setStatusProgress(0);
            emit setStatusMessage(tr("Not connected"));
        }
        else
        {
            this->connectButton->setBlink(false);
            this->connectButton->setValue(false);
            sysxIO->setMode("mode2");
            sysxIO->setMode("mode0");
            sysxIO->setConnected(false);
            midiIO *probe = &AppServices::instance().midi();
            const int inPort = probe->getInDevice();
            const int outPort = probe->getOutDevice();
            qWarning("CONNECT_FAIL no identity reply. midiIn=%s midiOut=%s",
                     preferences->getPreferences("Midi", "MidiIn", "name").toUtf8().constData(),
                     preferences->getPreferences("Midi", "MidiOut", "name").toUtf8().constData());
            qWarning("CONNECT_FAIL resolved ports in=%d out=%d auto=%s",
                     inPort,
                     outPort,
                     preferences->getPreferences("Midi", "Device", "bool").toUtf8().constData());
            AppServices::instance().midi().closePorts();

            preferencesDialog *dialog = new preferencesDialog(false);
            dialog->topLevelWidget();
            if(dialog->exec())
            {
                Preferences *preferences = &AppServices::instance().preferences();
                QString dBug = (dialog->midiSettings->dBugCheckBox->checkState())?QString("true"):QString("false");
                QString auto_device = (dialog->midiSettings->autoCheckBox->checkState())?QString("true"):QString("false");
                QString midiInDeviceName = (dialog->midiSettings->midiInDeviceName);
                QString midiOutDeviceName = (dialog->midiSettings->midiOutDeviceName);
                QString midiTxChSet =QString::number(dialog->midiSettings->midiTxChSpinBox->value());
                preferences->setPreferences("Midi", "MidiIn", "name", midiInDeviceName);
                preferences->setPreferences("Midi", "MidiOut", "name", midiOutDeviceName);
                preferences->setPreferences("Midi", "DBug", "bool", dBug);
                preferences->setPreferences("Midi", "Device", "bool", auto_device);
                preferences->setPreferences("Midi", "TxCh", "set", midiTxChSet);
                preferences->savePreferences();   
                dialog->deleteLater();
            };
        };
    }
    else
    {
        notConnected();
        sysxIO->setNoError(true);		// Reset the error status (else we could never retry :) ).
    };
}

void floorBoardDisplay::writeSignal(bool)
{
    this->writeButton->setValue(true);
    this->writeButton->setBlink(true);
    SysxIO *sysxIO = &AppServices::instance().sysx();
    if(sysxIO->isConnected()/* && sysxIO->deviceReady()*/) /* Check if we are connected and if the device is free. */
    {
        writeDialog->listUpdate();
        writeDialog->isWindow();
        writeDialog->raise();
        writeDialog->show();
        this->writeButton->setBlink(false);
        this->writeButton->setValue(false);
    };

}

void floorBoardDisplay::writeToBuffer()
{
    SysxIO *sysxIO = &AppServices::instance().sysx();
    QMessageBox *msgBox = new QMessageBox();
    msgBox->setWindowTitle(deviceType + tr(" FloorBoard"));
    msgBox->setIcon(QMessageBox::Warning);
    msgBox->setTextFormat(Qt::RichText);
    QString msgText;
    msgText.append("<font size='+1'><b>");
    msgText.append(tr("You have not chosen a User Patch address to Write the patch to"));
    msgText.append("<b></font><br>");
    msgText.append(tr("This will only update the SY-1000 Temporary Buffer memory<br>"));
    msgText.append(tr (" with the editor patch. "));
    msgBox->setInformativeText(tr("Select the required destination patch <br>by a single-click on the left panel Patch-Tree"));
    msgBox->setText(msgText);
    msgBox->show();
    QTimer::singleShot(8000, msgBox, SLOT(deleteLater()));

    sysxIO->writeToBuffer();
    sysxIO->setSyncStatus(true);

    this->writeButton->setBlink(false);	// Sync so we stop blinking the button
    this->writeButton->setValue(false);	// and activate the write button.
}

void floorBoardDisplay::writeToMemory()
{

}

void floorBoardDisplay::patchChangeFailed()
{
    SysxIO *sysxIO = &AppServices::instance().sysx();
    sysxIO->setBank(sysxIO->getLoadedBank());
    sysxIO->setPatch(sysxIO->getLoadedPatch());
    setPatchNumDisplay(sysxIO->getLoadedBank(), sysxIO->getLoadedPatch());
}

void floorBoardDisplay::resetDevice(QString sysxMsg)
{
    Q_UNUSED(sysxMsg);
    SysxIO *sysxIO = &AppServices::instance().sysx();
    QObject::disconnect(sysxIO, SIGNAL(sysxReply(QString)), this, SLOT(resetDevice(QString)));

    if(sysxIO->getBank() != sysxIO->getLoadedBank() || sysxIO->getPatch() != sysxIO->getLoadedPatch())
    {
        sysxIO->setLoadedBank(sysxIO->getBank());
        sysxIO->setLoadedPatch(sysxIO->getPatch());
    };
    sysxIO->setDeviceReady(true);	// Free the device after finishing interaction.
    qApp->restoreOverrideCursor ();
}

void floorBoardDisplay::patchSelectSignal(int bank, int patch)
{
    SysxIO *sysxIO = &AppServices::instance().sysx();
    if(blinkCount == 0)
    {
        currentSyncStatus = sysxIO->getSyncStatus();
        sysxIO->setSyncStatus(false);
        writeButton->setBlink(false);
    };

    sysxIO->setBank(bank);
    sysxIO->setPatch(patch);

    if(blinkCount == 0)
    {
        QObject::connect(timer, SIGNAL(timeout()), this, SLOT(blinkSellectedPatch()));
        timer->start(sellectionBlinkInterval);
    }
    else
    {
        blinkCount = 0;
    };
}

void floorBoardDisplay::blinkSellectedPatch(bool active)
{
    SysxIO *sysxIO = &AppServices::instance().sysx();
    int bank = sysxIO->getBank();
    int patch = sysxIO->getPatch();

    if(active && blinkCount <= (sellectionBlinks * 2) - 1)
    {
        if(blinkCount % 2 == 0)
        {
            this->patchNumDisplay->clearAll();
        }
        else
        {
            setPatchNumDisplay(bank, patch);
        };
        blinkCount++;
    }
    else
    {
        QObject::disconnect(timer, SIGNAL(timeout()), this, SLOT(blinkSellectedPatch()));
        timer->stop();
        blinkCount = 0;
        sysxIO->setSyncStatus(currentSyncStatus);
        if(currentSyncStatus || sysxIO->getLoadedBank() == 0)
        {
            writeButton->setBlink(false);
        };
        setPatchNumDisplay(bank,patch);
    };
}

void floorBoardDisplay::patchLoadSignal(int bank, int patch)
{
    blinkSellectedPatch(false);

    SysxIO *sysxIO = &AppServices::instance().sysx();
    sysxIO->setBank(bank);
    sysxIO->setPatch(patch);
}

void floorBoardDisplay::notConnected()
{
    this->connectButton->setBlink(false);
    this->connectButton->setValue(false);
    this->writeButton->setBlink(false);
    this->writeButton->setValue(false);

    SysxIO *sysxIO = &AppServices::instance().sysx();
    if(sysxIO->mode>2)
    {
        sysxIO->setMode("mode2");
    };
    if(sysxIO->mode>0)
    {
        sysxIO->setMode("mode0");
    };
    sysxIO->patch_loaded = false;
    sysxIO->setConnected(false);
    sysxIO->setSyncStatus(false);
    sysxIO->setDeviceReady(true);	// Free the device after finishing interaction.

    emit setStatusSymbol(0);
    emit setStatusProgress(0);
    emit setStatusMessage(tr("Not connected"));
}

void floorBoardDisplay::valueChanged(bool value, QString hex1, QString hex2, QString hex3)
{
    Q_UNUSED(value);
    Q_UNUSED(hex1);
    Q_UNUSED(hex2);
    Q_UNUSED(hex3);
}


