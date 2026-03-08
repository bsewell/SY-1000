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
#include <QWhatsThis>
#include <QFileInfo>
#include "mainWindow.h"
#include "Preferences.h"
#include "preferencesDialog.h"
#include "statusBarWidget.h"
#include "SysxIO.h"
#include "bulkSaveDialog.h"
#include "bulkLoadDialog.h"
#include "notesDialog.h"
#include "summaryDialog.h"
#include "summaryDialogPatchList.h"
#include "summaryDialogSystem.h"
#include "appservices.h"
#include "globalVariables.h"
#include "consoletoolbar.h"

// Platform-dependent sleep routines.
#ifdef Q_OS_WIN
#include <windows.h>
#define SLEEP( milliseconds ) Sleep( (DWORD) milliseconds )
#else // Unix variants
#include <unistd.h>
#define SLEEP( milliseconds ) usleep( (unsigned long) (milliseconds * 1000.0) )
#endif

static void showOfflineWriteSkippedNotice(QWidget *parent, const QString& sourceName)
{
    QMessageBox *msgBox = new QMessageBox(parent);
    msgBox->setWindowTitle(deviceType + QObject::tr(" not connected !!"));
    msgBox->setIcon(QMessageBox::Information);
    msgBox->setTextFormat(Qt::RichText);
    QString msgText;
    msgText.append("<b>");
    msgText.append(QObject::tr("Loaded into editor only."));
    msgText.append("</b><br>");
    msgText.append(QObject::tr("SY-1000 write skipped because no verified device connection was found."));
    if(!sourceName.isEmpty())
    {
        msgText.append("<br>");
        msgText.append(QObject::tr("Source: "));
        msgText.append(sourceName);
    }
    msgBox->setText(msgText);
    msgBox->show();
    QTimer::singleShot(3500, msgBox, SLOT(deleteLater()));
}

mainWindow::mainWindow()
{
    Preferences *preferences = &AppServices::instance().preferences();
    bool ok;
    const double ratio = preferences->getPreferences("Window", "Scale", "ratio").toDouble(&ok);
    const double fratio = preferences->getPreferences("Window", "Font", "ratio").toDouble(&ok);
    QFont Mfont( "Arial", 12*fratio, QFont::Normal);
    this->setFont(Mfont);

    /*int current_version = preferences->getPreferences("General", "Application", "version").toInt(&ok, 10);
    QDate date = QDate::currentDate();
    text = date.toString("yyyyMMdd");
    QString site_file;
    QUrl site("http://sourceforge.net/projects/grfloorboard/files/SY-1000/readme.txt");           // Read the default SY-1000 tsl file .
    site_file = site.hasQuery();
    if (site_file.isEmpty() ) { QApplication::beep(); };

    QMessageBox *msgBox = new QMessageBox();
    msgBox->setWindowTitle("Web site read version number");
    msgBox->setIcon(QMessageBox::Information);
    msgBox->setText(site_file);
    msgBox->setStandardButtons(QMessageBox::Ok);
    msgBox->exec();
    msgBox->deleteLater();*/
    //#ifdef Q_PROCESSOR_ARM
    qWarning("MW-A: setFixedSize ratio=%.2f", ratio);
    this->setFixedSize(1350*ratio, 845*ratio);
    qWarning("MW-B: constructing floorBoard...");

    this->fxsBoard = new floorBoard(this);
    qWarning("MW-C: floorBoard constructed OK");
    //this->fxsBoard->scroll(600, 300);

    QString setting = preferences->getPreferences("Scheme", "Style", "select");
    int choice = setting.toInt(&ok, 16);
    QString style;
    if(choice == 3) {style = "motif"; }
    else if(choice == 2) {style = "cleanlooks"; }
    else if(choice == 1) {style = "plastique"; }
    else {style = "Fusion"; };
#if defined(Q_OS_WIN)
    if(style=="motif"){style="windows";};
    if(style=="cleanlooks"){style="windowsxp";};
    if(style=="plastique"){style="windowsvista";};
#endif
#if defined(Q_OS_MAC)
    if(style=="motif"){style="Fusion";};
    if(style=="cleanlooks"){style="Fusion";};
    if(style=="plastique"){style="macintosh";};
#endif
    setting = preferences->getPreferences("Scheme", "Colour", "select");
    choice = setting.toInt(&ok, 16);
    QString colour;
    if(choice == 4) {colour = ":/qss/white.qss"; }
    else if(choice == 3) {colour = ":/qss/green.qss"; }
    else if(choice == 2) {colour = ":/qss/blue.qss"; }
    else if(choice == 1) {colour = ":/qss/black.qss"; }
    else {colour = ":/qss/bts.qss"; };
    qWarning("MW-D: loading stylesheet colour=%d path=%s", choice, colour.toUtf8().constData());
    /* Loads the stylesheet for the current platform if present */
    QApplication::setStyle(QStyleFactory::create(style));
    if(QFile(colour).exists())
    {
        QFile file(colour);
        if(file.open(QFile::ReadOnly))
        {
            QString styleSheet = QLatin1String(file.readAll());
            qApp->setStyleSheet(styleSheet);
            this->fxsBoard->setStyleSheet(styleSheet);
        };
    };
    qWarning("MW-E: stylesheet applied");

    QString version = preferences->getPreferences("General", "Application", "version");
    QString mode = "Guitar mode";
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true") mode = "Bass Mode";
    setWindowTitle(deviceType + " FloorBoard - version:" + version + " - " + mode);
    qWarning("MW-F: createActions...");
    createActions();
    qWarning("MW-G: createMenus...");
    createMenus();
    qWarning("MW-H: createStatusBar...");
    createStatusBar();
    qWarning("MW-I: setCentralWidget...");
    setCentralWidget(this->fxsBoard);

    // Header controls are rendered directly by floorBoardDisplay.

    statusBar()->setWhatsThis("StatusBar<br>midi activity is displayed here<br>and some status messages are displayed.");

    QObject::connect(this->fxsBoard, &floorBoard::sizeChanged,
                     this, &mainWindow::updateSize);

    qWarning("MW-J: SysxIO connect...");
    SysxIO *sysxIO = &AppServices::instance().sysx();
    QObject::connect(sysxIO, &SysxIO::restart, this, &mainWindow::restart);
    qWarning("MW-K: mainWindow constructor complete");
}

mainWindow::~mainWindow()
{
    Preferences *preferences = &AppServices::instance().preferences();
    bool ok;
    const double ratio = preferences->getPreferences("Window", "Scale", "ratio").toDouble(&ok);
    if(preferences->getPreferences("Window", "Restore", "window")=="true")
    {
        QString posx, width;
        if(preferences->getPreferences("Window", "Restore", "sidepanel")=="true" &&
            preferences->getPreferences("Window", "Collapsed", "bool")=="true")
        {
            int Wcalc = this->geometry().width()/ratio;
            width = QString::number(Wcalc, 10);
            posx = QString::number(this->geometry().x()-8, 10);
        }
        else
        {
            width = preferences->getPreferences("Window", "Size", "minwidth");
            posx = QString::number(this->geometry().x()+((this->geometry().width()-width.toInt(&ok,10))/2), 10);
        };
        preferences->setPreferences("Window", "Position", "x", posx);
        preferences->setPreferences("Window", "Position", "y", QString::number(this->geometry().y()-30, 10));
        preferences->setPreferences("Window", "Size", "width", width);
        int Hcalc = (this->geometry().height()*1.2)/ratio;
        preferences->setPreferences("Window", "Size", "height", QString::number(Hcalc, 10));
    }
    else
    {
        preferences->setPreferences("Window", "Position", "x", "");
        preferences->setPreferences("Window", "Position", "y", "");
        preferences->setPreferences("Window", "Size", "width", "");
        preferences->setPreferences("Window", "Size", "height", "");
    };
    preferences->savePreferences();

    SysxIO *sysxIO = &AppServices::instance().sysx();
    sysxIO->sendSysx("F0411000000069127F000002007FF7F0411000000069127F0000010000F7", 0);
    sysxIO->deBug("SY-1000FloorBoard shut down - GOODBYE !!!");
}

void mainWindow::exportDebugScreens(const QString &dirPath)
{
    if(!this->fxsBoard)
    {
        this->fxsBoard = this->findChild<floorBoard *>();
    }

    if(this->fxsBoard)
    {
        this->fxsBoard->exportDebugScreens(dirPath);
    }
}

void mainWindow::updateSize(QSize floorSize, QSize oldFloorSize)
{
    Preferences *preferences = &AppServices::instance().preferences();
    bool ok;
    const double ratio = preferences->getPreferences("Window", "Scale", "ratio").toDouble(&ok);

    this->setFixedWidth(floorSize.width()*ratio);
    int x = this->geometry().x() - ((floorSize.width() - oldFloorSize.width()) / 2);
    int y = this->geometry().y();
    this->setGeometry(x, y, floorSize.width(), this->geometry().height());
}

void mainWindow::createActions()
{
    openAct = new QAction(QIcon(":/images/fileopen.png"), tr("&Load Patch File... (*.tsl, *.mid, *.syx)"), this);
    openAct->setShortcut(tr("Ctrl+O"));
    openAct->setStatusTip(tr("Open an existing file"));
    openAct->setWhatsThis(tr("Open an existing file"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    exportSyxAct = new QAction(QIcon(":/images/filesave.png"), tr("&Export &SYX Patch...  (*.syx)"), this);
    exportSyxAct->setShortcut(tr("Ctrl+Shift+S"));
    exportSyxAct->setStatusTip(tr("Export as a System Exclusive File"));
    exportSyxAct->setWhatsThis(tr("Export as a System Exclusive File"));
    connect(exportSyxAct, SIGNAL(triggered()), this, SLOT(exportSyx()));

    Preferences *preferences = &AppServices::instance().preferences();
    QString mode = "Guitar";
    QString abr = "(*_G.tsl)";
    QString op_mode = "Bass";
    QString op_abr = "(*_B.tsl)";
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){mode = "Bass"; abr = "(*_B.tsl)"; op_mode = "Guitar"; op_abr = "(*_G.tsl)";};

    saveTslAct = new QAction(QIcon(":/images/filesave.png"), tr("Save As SY-1000 Tone Studio ") + mode + tr(" Mode Patch... ") + abr, this);
    saveTslAct->setShortcut(tr("Ctrl+Shift+G"));
    saveTslAct->setStatusTip(tr("Save as a Boss Tone Studio format File"));
    saveTslAct->setWhatsThis(tr("Save as a Boss Tone Studio format File"));
    connect(saveTslAct, SIGNAL(triggered()), this, SLOT(saveTsl()));

    exportTslAct = new QAction(QIcon(":/images/filesave.png"), tr("Export As SY-1000 Tone Studio ") + op_mode + tr(" Mode Patch... ") + op_abr, this);
    exportTslAct->setShortcut(tr("Ctrl+Shift+E"));
    exportTslAct->setStatusTip(tr("Export as ") + op_mode + tr(" Mode Patch File"));
    exportTslAct->setWhatsThis(tr("Export as ") + op_mode + tr(" Mode Patch File"));
    connect(exportTslAct, SIGNAL(triggered()), this, SLOT(exportTSL()));

    systemLoadAct = new QAction(QIcon(":/images/fileopen.png"), tr("&Load System Backup Data..."), this);
    systemLoadAct->setShortcut(tr("Ctrl+L"));
    systemLoadAct->setStatusTip(tr("Load System Backup Data from file to SY-1000"));
    systemLoadAct->setWhatsThis(tr("Load System Data from file to SY-1000"));
    connect(systemLoadAct, SIGNAL(triggered()), this, SLOT(systemLoad()));

    systemSaveAct = new QAction(QIcon(":/images/filesave.png"), tr("Save System and Global Data to File..."), this);
    systemSaveAct->setShortcut(tr("Ctrl+D"));
    systemSaveAct->setStatusTip(tr("Backup SY-1000 System Data to File"));
    systemSaveAct->setWhatsThis(tr("Backup SY-1000 System Data to File"));
    connect(systemSaveAct, SIGNAL(triggered()), this, SLOT(systemSave()));

    bulkLoadAct = new QAction(QIcon(":/images/fileopen.png"), tr("&Load Bulk Patch Bundle to SY-1000..."), this);
    bulkLoadAct->setShortcut(tr("Ctrl+B"));
    bulkLoadAct->setStatusTip(tr("Load Bulk Patch bundled file to SY-1000"));
    bulkLoadAct->setWhatsThis(tr("Load Bulk Patch bundle file to SY-1000"));
    connect(bulkLoadAct, SIGNAL(triggered()), this, SLOT(bulkLoad()));

    bulkSaveAct = new QAction(QIcon(":/images/filesave.png"), tr("Save Bulk SY-1000 Patches to File..."), this);
    bulkSaveAct->setShortcut(tr("Ctrl+G"));
    bulkSaveAct->setStatusTip(tr("Save Bulk Patch Data to File"));
    bulkSaveAct->setWhatsThis(tr("Save Bulk Patch Data to File"));
    connect(bulkSaveAct, SIGNAL(triggered()), this, SLOT(bulkSave()));

    exitAct = new QAction(QIcon(":/images/exit.png"),tr("E&xit"), this);
    exitAct->setShortcut(tr("Ctrl+Q"));
    exitAct->setStatusTip(tr("Exit the application"));
    exitAct->setWhatsThis(tr("Exit the application"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    settingsAct = new QAction(QIcon(":/images/preferences.png"), tr("&Preferences"), this);
    settingsAct->setShortcut(tr("Ctrl+P"));
    settingsAct->setStatusTip(tr("SY-1000FloorBoard Preferences<br>Select midi device, language,splash, directories"));
    settingsAct->setWhatsThis(tr("SY-1000FloorBoard Preferences<br>Select midi device, language,splash, directories"));
    connect(settingsAct, SIGNAL(triggered()), this, SLOT(settings()));

    notesAct = new QAction(QIcon(":/images/bass_icon.png"), tr("&Text notes for Patch"), this);
    /*notesAct->setShortcut(tr("Shift+N"));
    notesAct->setStatusTip(tr("Write Text notes for saved Patch file - only saves in *.tsl file type"));
    notesAct->setWhatsThis(tr("Write Text notes for saved Patch file - only saves in *.tsl file type"));
    */connect(notesAct, SIGNAL(triggered()), this, SLOT(notes()));


    uploadAct = new QAction(QIcon(":/images/upload.png"), tr("Patch Repository at V-Guitar Forums"), this);
    uploadAct->setStatusTip(tr("Exchange patch files with a shared patch library via the internet."));
    uploadAct->setWhatsThis(tr("Exchange patch files with a shared patch library<br>via the internet."));
    connect(uploadAct, SIGNAL(triggered()), this, SLOT(upload()));

    summaryAct = new QAction(QIcon(":/images/copy.png"), tr("Patch Text Summary"), this);
    summaryAct->setStatusTip(tr("Display the current patch parameters in a readable text format, which can be printed or saved to file."));
    summaryAct->setWhatsThis(tr("Display the current patch parameters<br>in a readable text format, which<br>can be printed or saved to file."));
    connect(summaryAct, SIGNAL(triggered()), this, SLOT(summaryPage()));

    summarySystemAct = new QAction(QIcon(":/images/copy.png"), tr("System/Global Text Summary"), this);
    summarySystemAct->setStatusTip(tr("Display the current System and Global parameters in a readable text format, which can be printed or saved to file."));
    summarySystemAct->setWhatsThis(tr("Display the current System and Global parameters<br>in a readable text format, which<br>can be printed or saved to file."));
    connect(summarySystemAct, SIGNAL(triggered()), this, SLOT(summarySystemPage()));

    summaryPatchListAct = new QAction(QIcon(":/images/copy.png"), tr("SY-1000 User Patch List Summary"), this);
    summaryPatchListAct->setStatusTip(tr("Display the SY-1000 User patch listing names in a readable text format, which can be printed or saved to file."));
    summaryPatchListAct->setWhatsThis(tr("Display the SY-1000 User patch listing names<br>in a readable text format, which<br>can be printed or saved to file."));
    connect(summaryPatchListAct, SIGNAL(triggered()), this, SLOT(summaryPatchList()));

    helpAct = new QAction(QIcon(":/images/help.png"), tr("SY-1000 FloorBoard &Help"), this);
    helpAct->setShortcut(tr("Ctrl+F1"));
    helpAct->setStatusTip(tr("Help page to assist with SY-1000 FloorBoard functions."));
    helpAct->setWhatsThis(tr("Help page to assist with SY-1000 FloorBoard functions."));
    connect(helpAct, SIGNAL(triggered()), this, SLOT(help()));

    whatsThisAct = new QAction(QIcon(":/images/help.png"), tr("Whats This? description of items under the mouse cursor"), this);
    whatsThisAct->setShortcut(tr("F1"));
    whatsThisAct->setWhatsThis(tr("Whats This? description of items under the mouse cursor"));
    whatsThisAct->setStatusTip(tr("Whats This? description of items under the mouse cursor"));
    connect(whatsThisAct, SIGNAL(triggered()), this, SLOT(whatsThis()));

    homepageAct = new QAction(QIcon(":/images/SY-1000FloorBoard.png"), tr("SY-1000 FloorBoard &Webpage"), this);
    homepageAct->setStatusTip(tr("download Webpage for SY-1000FloorBoard find if the latest version is available."));
    homepageAct->setWhatsThis(tr("download Webpage for SY-1000FloorBoard<br>find if the latest version is available."));
    connect(homepageAct, SIGNAL(triggered()), this, SLOT(homepage()));

    donationAct = new QAction(QIcon(":/images/donate.png"), tr("Donate if you appreciate this software"), this);
    donationAct->setStatusTip(tr("Even though the software is free, a donation is very much appreciated for my un-paid efforts to provide this software."));
    donationAct->setWhatsThis(tr("Even though the software is free,<br>a donation is very much appreciated<br>for my un-paid efforts to provide this software."));
    connect(donationAct, SIGNAL(triggered()), this, SLOT(donate()));

    manualAct = new QAction(QIcon(":/images/manual.png"), tr("Owner's Manual"), this);
    manualAct->setStatusTip(tr("download the Owner's Manual"));
    manualAct->setWhatsThis(tr("download the Owner's Manual"));
    connect(manualAct, SIGNAL(triggered()), this, SLOT(manual()));

    licenseAct = new QAction(QIcon(":/images/licence.png"), tr("&License"), this);
    licenseAct->setStatusTip(tr("licence agreement which you have accepted by installing this software."));
    licenseAct->setWhatsThis(tr("licence agreement which you<br>have accepted by installing this software."));
    connect(licenseAct, SIGNAL(triggered()), this, SLOT(license()));

    aboutAct = new QAction(QIcon(":/images/SY-1000FloorBoard.png"), tr("&About SY-1000FloorBoard"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    aboutAct->setWhatsThis(tr("Show the application's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAct = new QAction(QIcon(":/images/qt-logo.png"),tr("About &Qt"), this);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
    aboutQtAct->setWhatsThis(tr("Show the Qt library's About box"));
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

void mainWindow::createMenus()
{
    Preferences *preferences = &AppServices::instance().preferences();
    bool ok;
    const double fratio = preferences->getPreferences("Window", "Font", "ratio").toDouble(&ok);
    QFont Mfont( "Arial", 12*fratio, QFont::Normal);
    
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->setFont(Mfont);
    fileMenu->addAction(openAct);
    fileMenu->addSeparator();
    fileMenu->addAction(saveTslAct);
    fileMenu->addAction(exportTslAct);
    fileMenu->addAction(exportSyxAct);
    fileMenu->addSeparator();
    fileMenu->addAction(bulkSaveAct);
    fileMenu->addAction(bulkLoadAct);
    fileMenu->addSeparator();
    fileMenu->addAction(systemLoadAct);
    fileMenu->addAction(systemSaveAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);
    fileMenu->setWhatsThis(tr("File Saving and Loading,<br> and application Exit."));

    toolsMenu = menuBar()->addMenu(tr("&Tools"));
    toolsMenu->setFont(Mfont);
    toolsMenu->addAction(uploadAct);
    fileMenu->addSeparator();
    toolsMenu->addAction(summaryAct);
    toolsMenu->addAction(summarySystemAct);
    toolsMenu->addAction(summaryPatchListAct);

    settingsMenu = menuBar()->addMenu(tr("&Settings"));
    settingsMenu->setFont(Mfont);
    settingsMenu->addAction(settingsAct);

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->setFont(Mfont);
    helpMenu->addAction(helpAct);
    helpMenu->addAction(whatsThisAct);
    helpMenu->addAction(homepageAct);
    helpMenu->addSeparator();
    helpMenu->addAction(donationAct);
    helpMenu->addSeparator();
    helpMenu->addAction(manualAct);
    helpMenu->addAction(licenseAct);
    helpMenu->addSeparator();
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);
}

void mainWindow::createStatusBar()
{
    SysxIO *sysxIO = &AppServices::instance().sysx();

    statusBarWidget *statusInfo = new statusBarWidget(this);
    statusInfo->setStatusSymbol(0);
    statusInfo->setStatusMessage(tr("Not connected"));

    QObject::connect(sysxIO, &SysxIO::setStatusSymbol, statusInfo, &statusBarWidget::setStatusSymbol);
    QObject::connect(sysxIO, &SysxIO::setStatusProgress, statusInfo, &statusBarWidget::setStatusProgress);
    QObject::connect(sysxIO, &SysxIO::setStatusMessage, statusInfo, &statusBarWidget::setStatusMessage);
    QObject::connect(sysxIO, &SysxIO::setStatusdBugMessage, statusInfo, &statusBarWidget::setStatusdBugMessage);

    statusBar()->addWidget(statusInfo);
    statusBar()->setSizeGripEnabled(false);
}

/* FILE MENU */
void mainWindow::open()
{
    Preferences *preferences = &AppServices::instance().preferences();
    QString dir = preferences->getPreferences("General", "Files", "dir");

    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Choose a file to open: SY-1000, SY-300, GT-1000, GP-10, VG-99, VB-99,"),
        dir,
        "*.tsl *.syx *.mid *.g9l");
    if (!fileName.isEmpty())
    {
        file.setFile(fileName);
        if(file.readFile())
        {
            // DO SOMETHING AFTER READING THE FILE (UPDATE THE GUI)
            SysxIO *sysxIO = &AppServices::instance().sysx();
            sysxIO->setFileSource("Structure", file.getFileSource());
            sysxIO->setFileName(fileName);
            sysxIO->setSyncStatus(false);
            sysxIO->setDevice(false);
            emit updateSignal();
            if(sysxIO->isConnected())
            {
                sysxIO->writeToBuffer();
            }
            else
            {
                showOfflineWriteSkippedNotice(this, QFileInfo(fileName).fileName());
            }
        };
    };
}

void mainWindow::exportSyx()
{
    Preferences *preferences = &AppServices::instance().preferences();
    QString dir = preferences->getPreferences("General", "Files", "dir");

    QString fileName = QFileDialog::getSaveFileName(
        this,
        tr("Export *.syx"),
        dir,
        "System Exclusive (*.syx)");
    if (!fileName.isEmpty())
    {
        if(!fileName.contains(".syx"))
        {
            fileName.append(".syx");
        };
        file.writeFile(fileName);

        file.setFile(fileName);
        if(file.readFile())
        {
            // DO SOMETHING AFTER READING THE FILE (UPDATE THE GUI)
            SysxIO *sysxIO = &AppServices::instance().sysx();
            sysxIO->setFileSource("Structure", file.getFileSource());
            emit updateSignal();
        };
        emit updateSignal();
    };
}

void mainWindow::importSMF()
{
    Preferences *preferences = &AppServices::instance().preferences();
    QString dir = preferences->getPreferences("General", "Files", "dir");

    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Choose a file"),
        dir,
        "Standard Midi File (*.mid)");
    if (!fileName.isEmpty())
    {
        file.setFile(fileName);
        if(file.readFile())
        {
            // DO SOMETHING AFTER READING THE FILE (UPDATE THE GUI)
            SysxIO *sysxIO = &AppServices::instance().sysx();
            QString area = "Structure";
            sysxIO->setFileSource(area, file.getFileSource());
            sysxIO->setFileName(fileName);
            sysxIO->setSyncStatus(false);
            sysxIO->setDevice(false);
            emit updateSignal();
            if(sysxIO->isConnected())
            {
                sysxIO->writeToBuffer();
            }
            else
            {
                showOfflineWriteSkippedNotice(this, QFileInfo(fileName).fileName());
            }
        };
    };
}

void mainWindow::exportTSL()
{
    Preferences *preferences = &AppServices::instance().preferences();
    QString dir = preferences->getPreferences("General", "Files", "dir");
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        QString fileName = QFileDialog::getSaveFileName(
            this,
            tr("Save Patch *_G.tsl"),
            dir,
            "Tone Studio File (*_G.tsl)");
        if (!fileName.isEmpty())
        {
            if(!fileName.contains("_G.tsl"))
            {
                fileName.append("_G.tsl");
            };
            file.writeTSL_G(fileName);  // convert and write as a bass mode patch
        };
        file.setFile(fileName);
        if(file.readFile())
        {
            // DO SOMETHING AFTER READING THE FILE (UPDATE THE GUI)
            SysxIO *sysxIO = &AppServices::instance().sysx();
            QString area = "Structure";
            sysxIO->setFileSource(area, file.getFileSource());

            emit updateSignal();
        };
    }
    else
    {
        QString fileName = QFileDialog::getSaveFileName(
            this,
            tr("Save Patch *_B.tsl"),
            dir,
            "Tone Studio File (*_B.tsl)");
        if (!fileName.isEmpty())
        {
            if(!fileName.contains("_B.tsl"))
            {
                fileName.append("_B.tsl");
            };
            file.writeTSL_B(fileName);  // convert and write as a bass mode patch
        };

        file.setFile(fileName);
        if(file.readFile())
        {
            // DO SOMETHING AFTER READING THE FILE (UPDATE THE GUI)
            SysxIO *sysxIO = &AppServices::instance().sysx();
            QString area = "Structure";
            sysxIO->setFileSource(area, file.getFileSource());

            emit updateSignal();
        };
    };
}

void mainWindow::openTsl()
{
    Preferences *preferences = &AppServices::instance().preferences();
    QString dir = preferences->getPreferences("General", "Files", "dir");

    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Choose a file"),
        dir,
        "Tone Studio File (*.tsl)");
    if (!fileName.isEmpty())
    {
        file.setFile(fileName);
        if(file.readFile())
        {
            // DO SOMETHING AFTER READING THE FILE (UPDATE THE GUI)
            SysxIO *sysxIO = &AppServices::instance().sysx();
            QString area = "Structure";
            sysxIO->setFileSource(area, file.getFileSource());
            sysxIO->setFileName(fileName);
            sysxIO->setSyncStatus(false);
            sysxIO->setDevice(false);

            emit updateSignal();
            if(sysxIO->isConnected())
            {
                sysxIO->writeToBuffer();
            }
            else
            {
                showOfflineWriteSkippedNotice(this, QFileInfo(fileName).fileName());
            }
        };
    };
}

void mainWindow::saveTsl()
{
    Preferences *preferences = &AppServices::instance().preferences();

    QString dir = preferences->getPreferences("General", "Files", "dir");
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        QString fileName = QFileDialog::getSaveFileName(
            this,
            tr("Save Patch *.tsl"),
            dir,
            "Tone Studio File (*.tsl)");
        if (!fileName.isEmpty())
        {
            if(!fileName.contains("_B.tsl"))
            {
                fileName.append("_B.tsl");
            };
            file.writeTSL_B(fileName);
        };
        file.setFile(fileName);
        if(file.readFile())
        {
            // DO SOMETHING AFTER READING THE FILE (UPDATE THE GUI)
            SysxIO *sysxIO = &AppServices::instance().sysx();
            QString area = "Structure";
            sysxIO->setFileSource(area, file.getFileSource());
            emit updateSignal();
        };
    }
    else
    {
        QString fileName = QFileDialog::getSaveFileName(
            this,
            tr("Save Patch *.tsl"),
            dir,
            "Tone Studio File (*.tsl)");
        if (!fileName.isEmpty())
        {
            if(!fileName.contains("_G.tsl"))
            {
                fileName.append("_G.tsl");
            };
            file.writeTSL_G(fileName);
        };
        file.setFile(fileName);
        if(file.readFile())
        {
            // DO SOMETHING AFTER READING THE FILE (UPDATE THE GUI)
            SysxIO *sysxIO = &AppServices::instance().sysx();
            QString area = "Structure";
            sysxIO->setFileSource(area, file.getFileSource());
            emit updateSignal();
        };
    };
}

void mainWindow::systemLoad()
{
    SysxIO *sysxIO = &AppServices::instance().sysx();
    if (sysxIO->isConnected())
    {
        Preferences *preferences = &AppServices::instance().preferences();
        QString dir = preferences->getPreferences("General", "Files", "dir");

        QString fileName = QFileDialog::getOpenFileName(
            this,
            tr("Choose a file"),
            dir,
            deviceType + " System Data File (*SY1K_system.syx)");
        if (!fileName.isEmpty())
        {
            file.setFile(fileName);
            if(file.readFile())
            {
                // DO SOMETHING AFTER READING THE FILE (UPDATE THE GUI)
                SysxIO *sysxIO = &AppServices::instance().sysx();
                QString area = "System";
                sysxIO->setFileSource(area, file.getSystemSource());
                sysxIO->setFileName(fileName);
                emit updateSignal();
                QMessageBox *msgBox = new QMessageBox();
                msgBox->setWindowTitle(deviceType + tr(" FloorBoard"));
                msgBox->setIcon(QMessageBox::Warning);
                msgBox->setTextFormat(Qt::RichText);
                QString msgText;
                msgText.append("<font size='+1'><b>");
                msgText.append(tr("You have chosen to load a SYSTEM DATA file."));
                msgText.append("<b></font><br>");
                msgText.append(tr("This will overwrite the SYSTEM DATA currently stored in the ")+ deviceType);
                msgText.append(tr ("<br> and can't be undone.<br>"));
                msgText.append(tr("Select 'NO' to only update the Editor - Select 'YES' to update the SY-1000 System<br>"));

                msgBox->setInformativeText(tr("Are you sure you want to write to the ")+ deviceType);
                msgBox->setText(msgText);
                msgBox->setStandardButtons(QMessageBox::Yes | QMessageBox::No);

                if(msgBox->exec() == QMessageBox::Yes)
                {	// Accepted to overwrite system data.
                    sysxIO->systemWrite();
                };
                msgBox->deleteLater();
            };
        };
    }
    else
    {
        QString snork = tr("Ensure connection is active and retry");
        QMessageBox *msgBox = new QMessageBox();
        msgBox->setWindowTitle(deviceType + tr(" not connected !!"));
        msgBox->setIcon(QMessageBox::Information);
        msgBox->setText(snork);
        msgBox->show();
        QTimer::singleShot(3000, msgBox, SLOT(deleteLater()));
    };
}

void mainWindow::systemSave()
{
    SysxIO *sysxIO = &AppServices::instance().sysx();
    if (sysxIO->isConnected())
    {
        sysxIO->systemDataRequest();

        Preferences *preferences = &AppServices::instance().preferences();
        QString dir = preferences->getPreferences("General", "Files", "dir");

        QString fileName = QFileDialog::getSaveFileName(
            this,
            tr("Save System Data"),
            dir,
            "System Exclusive File (*.syx)");
        if (!fileName.isEmpty())
        {
            if(!fileName.contains("_SY1K_system.syx"))
            {
                fileName.append("_SY1K_system.syx");
            };

            file.writeSystemFile(fileName);

            file.setFile(fileName);
            if(file.readFile())
            {
                SysxIO *sysxIO = &AppServices::instance().sysx();
                sysxIO->setFileSource("System", file.getSystemSource());
                emit updateSignal();
            };
        };
    }
    else
    {
        QString snork = tr("Ensure SY-1000 connection is active and retry<br>");
        QMessageBox *msgBox = new QMessageBox();
        msgBox->setWindowTitle(deviceType + tr(" not connected !!"));
        msgBox->setIcon(QMessageBox::Information);
        msgBox->setText(snork);
        msgBox->show();
        QTimer::singleShot(3000, msgBox, SLOT(deleteLater()));
    };
}

void mainWindow::bulkLoad()
{
    SysxIO *sysxIO = &AppServices::instance().sysx();
    if (sysxIO->isConnected())
    {
        bulkLoadDialog *loadDialog = new bulkLoadDialog();
        loadDialog->exec();
        loadDialog->deleteLater();
        SLEEP(1000);
        //emit connectedSignal();
    }
    else
    {
        QString snork = tr("Ensure SY-1000 connection is active and retry");
        QMessageBox *msgBox = new QMessageBox();
        msgBox->setWindowTitle(deviceType + tr(" not connected !!"));
        msgBox->setIcon(QMessageBox::Information);
        msgBox->setText(snork);
        msgBox->show();
        QTimer::singleShot(3000, msgBox, SLOT(deleteLater()));
    };
}

void mainWindow::bulkSave()
{
    SysxIO *sysxIO = &AppServices::instance().sysx();
    if (sysxIO->isConnected())
    {
        bulkSaveDialog *bulkDialog = new bulkSaveDialog();
        bulkDialog->exec();
        bulkDialog->deleteLater();
    }
    else
    {
        QString snork = tr("Ensure SY-1000 connection is active and retry");
        QMessageBox *msgBox = new QMessageBox();
        msgBox->setWindowTitle(deviceType + tr(" not connected !!"));
        msgBox->setIcon(QMessageBox::Information);
        msgBox->setText(snork);
        msgBox->show();
        QTimer::singleShot(3000, msgBox, SLOT(deleteLater()));
    };
}

/* TOOLS MENU */
void mainWindow::settings()
{
    preferencesDialog *dialog = new preferencesDialog(true);
    if (dialog->exec())
    {
        Preferences *preferences = &AppServices::instance().preferences();

        QString dir = dialog->generalSettings->dirEdit->text();
        QString sidepanel = (dialog->windowSettings->sidepanelCheckBox->checkState())?QString("true"):QString("false");
        QString window = (dialog->windowSettings->windowCheckBox->checkState())?QString("true"):QString("false");
        QString singleWindow = (dialog->windowSettings->singleWindowCheckBox->checkState())?QString("true"):QString("false");
        QString autoScale = (dialog->windowSettings->autoRatioCheckBox->checkState())?QString("true"):QString("false");
        QString scaleRatio =QString::number(dialog->windowSettings->ratioSpinBox->value());
        QString scaleFontRatio =QString::number(dialog->windowSettings->fontRatioSpinBox->value());
        QString AutoMode = (dialog->modeSettings->autoModeCheckBox->checkState())?QString("true"):QString("false");
        QString splash = (dialog->windowSettings->splashCheckBox->checkState())?QString("true"):QString("false");
        QString dBug = (dialog->midiSettings->dBugCheckBox->checkState())?QString("true"):QString("false");
        QString auto_device = (dialog->midiSettings->autoCheckBox->checkState())?QString("true"):QString("false");
        QString midiIn = QString::number(dialog->midiSettings->midiInCombo->currentIndex(), 10);
        QString midiOut = QString::number(dialog->midiSettings->midiOutCombo->currentIndex(), 10);
        QString midiInDeviceName = (dialog->midiSettings->midiInDeviceName);
        QString midiOutDeviceName = (dialog->midiSettings->midiOutDeviceName);
        QString midiTxChSet =QString::number(dialog->midiSettings->midiTxChSpinBox->value());
        QString BassMode;
        if (dialog->modeSettings->guitarModeButton->isChecked() ) {BassMode="false"; }
        else if (dialog->modeSettings->bassModeButton->isChecked() ) {BassMode="true"; }
        QString lang;
        if (dialog->languageSettings->chineseButton->isChecked() ) {lang="6"; }
        else if (dialog->languageSettings->japaneseButton->isChecked() ) {lang="5"; }
        else if (dialog->languageSettings->spanishButton->isChecked() ) {lang="4"; }
        else if (dialog->languageSettings->portugueseButton->isChecked() ) {lang="3"; }
        else if (dialog->languageSettings->germanButton->isChecked() ) {lang="2"; }
        else if (dialog->languageSettings->frenchButton->isChecked() ) {lang="1"; }
        else {lang="0"; };
        preferences->setPreferences("Language", "Locale", "select", lang);
        QString choice;
        if (dialog->styleSettings->motifButton->isChecked() ) {choice="3"; }
        else if (dialog->styleSettings->cdeButton->isChecked() ) {choice="2"; }
        else if (dialog->styleSettings->plastiqueButton->isChecked() ) {choice="1"; }
        else {choice="0"; };
        preferences->setPreferences("Scheme", "Style", "select", choice);
        if (dialog->styleSettings->whiteButton->isChecked() ) {choice="4"; }
        else if (dialog->styleSettings->greenButton->isChecked() ) {choice="3"; }
        else if (dialog->styleSettings->aquaButton->isChecked() ) {choice="2"; }
        else if (dialog->styleSettings->blackButton->isChecked() ) {choice="1"; }
        else {choice="0"; };
        preferences->setPreferences("Scheme", "Colour", "select", choice);

        if(midiIn=="-1") { midiIn = ""; };
        if(midiOut=="-1") {	midiOut = ""; };

        preferences->setPreferences("General", "Files", "dir", dir);
        preferences->setPreferences("Midi", "MidiIn", "device", midiIn);
        preferences->setPreferences("Midi", "MidiOut", "device", midiOut);
        preferences->setPreferences("Midi", "MidiIn", "name", midiInDeviceName);
        preferences->setPreferences("Midi", "MidiOut", "name", midiOutDeviceName);
        preferences->setPreferences("Midi", "DBug", "bool", dBug);
        preferences->setPreferences("Midi", "Device", "bool", auto_device);
        preferences->setPreferences("Midi", "TxCh", "set", midiTxChSet);
        preferences->setPreferences("Window", "AutoMode", "bool", AutoMode);
        preferences->setPreferences("Window", "BassMode", "bool", BassMode);
        preferences->setPreferences("Window", "AutoScale", "bool", autoScale);
        preferences->setPreferences("Window", "Restore", "sidepanel", sidepanel);
        preferences->setPreferences("Window", "Restore", "window", window);
        preferences->setPreferences("Window", "Scale", "ratio", scaleRatio);
        preferences->setPreferences("Window", "Font", "ratio", scaleFontRatio);
        preferences->setPreferences("Window", "Single", "bool", singleWindow);
        preferences->setPreferences("Window", "Splash", "bool", splash);

        preferences->savePreferences();

        QMessageBox *msgBox = new QMessageBox();
        msgBox->setWindowTitle(tr("Settings saving to File - Restarting"));
        msgBox->setIcon(QMessageBox::Warning);
        msgBox->setTextFormat(Qt::RichText);
        QString msgText;
        msgText.append("<font size='+2'><b>");
        msgText.append(tr("Preference settings have been saved"));
        msgText.append(tr("<br>SY-1000FloorBoard will now restart"));
        msgText.append(tr("<br>with new settings applied"));
        msgText.append("<b></font>");
        msgBox->setText(msgText);
        msgBox->show();
        uint x=10;
        while (x>0)
        {
            QApplication::processEvents();
            //QString num = QString::number(x, 10).toUpper();
            msgText.append("<font size='+2'><b>");
            msgText.append("* ");
            msgText.append("<b></font>");
            msgBox->setText(msgText);
            SLEEP(40);
            QApplication::processEvents();
            x--;
        };
        restart();
    };
    dialog->deleteLater();
}

void mainWindow::notes()
{
    Preferences *preferences = &AppServices::instance().preferences();
    bool ok;
    const double ratio = preferences->getPreferences("Window", "Scale", "ratio").toDouble(&ok);
    notesDialog *notes = new notesDialog();
    notes->setMinimumWidth(850*ratio);
    notes->setMinimumHeight(850*ratio);
    notes->show();
}

/* HELP MENU */
void mainWindow::help()
{
    QDesktopServices::openUrl(QUrl("file:help/help.html"));
}

void mainWindow::whatsThis()
{
    QWhatsThis::enterWhatsThisMode();
}


void mainWindow::upload()
{
    Preferences *preferences = &AppServices::instance().preferences();
    QDesktopServices::openUrl(QUrl( preferences->getPreferences("General", "Upload", "url") ));
}

void mainWindow::summaryPage()
{
    summaryDialog *summary = new summaryDialog();
    summary->showMaximized();
    summary->show();
}

void mainWindow::summarySystemPage()
{
    summaryDialogSystem *summarySystem = new summaryDialogSystem();
    summarySystem->showMaximized();
    summarySystem->show();
}

void mainWindow::summaryPatchList()
{
    summaryDialogPatchList *summaryPatchList = new summaryDialogPatchList();
    summaryPatchList->showMaximized();
    summaryPatchList->show();
}

void mainWindow::homepage()
{
    Preferences *preferences = &AppServices::instance().preferences();
    QDesktopServices::openUrl(QUrl( preferences->getPreferences("General", "Webpage", "url") ));
}

void mainWindow::donate()
{
    QDesktopServices::openUrl(QUrl( "https://paypal.me/gumtown?country.x=NZ&locale.x=en_US" ));
}

void mainWindow::manual()
{
    Preferences *preferences = &AppServices::instance().preferences();
    QDesktopServices::openUrl(QUrl( preferences->getPreferences("General", "Manual", "url") ));
}

void mainWindow::license()
{
    QDesktopServices::openUrl(QUrl("file:license.txt"));
}

void mainWindow::about()
{
    Preferences *preferences = &AppServices::instance().preferences();
    QString version = preferences->getPreferences("General", "Application", "version");
    QDate date = QDate::currentDate();
    QString dateText = date.toString("yyyy");

    QFile file(":/about");
    if(file.open(QIODevice::ReadOnly))
    {
        QMessageBox::about(this, deviceType + tr(" FloorBoard - About"),
                           deviceType + tr(" FloorBoard - ") + tr("version") + " " + version + "<br>"
                               + tr("© Copyright ") + dateText + "<br>"
                               + file.readAll());
    };
}

void mainWindow::closeEvent(QCloseEvent* ce)
{
    Preferences *preferences = &AppServices::instance().preferences();
    preferences->savePreferences();
    ce->accept();
    emit closed();
}
void mainWindow::restart()
{
    const QByteArray reason = qEnvironmentVariable("SY1000_RESTART_REASON", "manual").toUtf8();
    qWarning("APP_RESTART begin reason=%s pid=%d", reason.constData(), int(QCoreApplication::applicationPid()));
    bool started = QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
    qWarning("APP_RESTART detached=%d reason=%s", started ? 1 : 0, reason.constData());
    qunsetenv("SY1000_RESTART_REASON");
    QCoreApplication::exit(0);
}
