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
#include "midiIO.h"
#include "preferencesPages.h"
#include "Preferences.h"
#include "SysxIO.h"

GeneralPage::GeneralPage(QWidget *parent)
    : QWidget(parent)
{
    Preferences *preferences = Preferences::Instance();
    QString dir = preferences->getPreferences("General", "Files", "dir");

    QGroupBox *patchGroup = new QGroupBox(QObject::tr("Patch folder"));

    QLabel *descriptionLabel = new QLabel(QObject::tr("Select the default folder for storing patches."));
    QLabel *dirLabel = new QLabel(QObject::tr("Default patch folder:"));
    QLineEdit *dirEdit = new QLineEdit(dir);
    QPushButton *browseButton = new QPushButton(QObject::tr("Browse"));

    connect(browseButton, SIGNAL(clicked()), this, SLOT(browseDir()));

    this->dirEdit = dirEdit;

    QHBoxLayout *dirEditLayout = new QHBoxLayout;
    dirEditLayout->addWidget(dirEdit);
    dirEditLayout->addWidget(browseButton);

    QVBoxLayout *dirLayout = new QVBoxLayout;
    dirLayout->addWidget(descriptionLabel);
    dirLayout->addSpacing(12);
    dirLayout->addWidget(dirLabel);
    dirLayout->addLayout(dirEditLayout);

    QVBoxLayout *patchLayout = new QVBoxLayout;
    patchLayout->addLayout(dirLayout);
    patchGroup->setLayout(patchLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(patchGroup);
    mainLayout->addStretch(1);
    setLayout(mainLayout);
}

MidiPage::MidiPage(QWidget *parent)
    : QWidget(parent)
{
    bool ok;
    Preferences *preferences = Preferences::Instance();
    QString midiInDevice = preferences->getPreferences("Midi", "MidiIn", "device");
    QString midiOutDevice = preferences->getPreferences("Midi", "MidiOut", "device");
    this->midiInDeviceName = preferences->getPreferences("Midi", "MidiIn", "name");
    this->midiOutDeviceName = preferences->getPreferences("Midi", "MidiOut", "name");
    QString dBugScreen = preferences->getPreferences("Midi", "DBug", "bool");
    this->device = preferences->getPreferences("Midi", "Device", "bool");
    const double ratio = preferences->getPreferences("Window", "Font", "ratio").toDouble(&ok);

    this->midiInDeviceID = midiInDevice.toInt(&ok, 10);
    this->midiOutDeviceID = midiOutDevice.toInt(&ok, 10);

    QGroupBox *midiGroup = new QGroupBox(QObject::tr("Midi settings"));

    QLabel *mididescriptionLabel = new QLabel(QObject::tr("Select your midi in and out device."));
    QLabel *midiInLabel = new QLabel(QObject::tr("Midi in:"));
    QLabel *midiOutLabel = new QLabel(QObject::tr("Midi out:"));

    this->midiInCombo = new QComboBox(this);
    this->midiOutCombo = new QComboBox(this);
    
    timer = new QTimer(this);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(updateDevice()) );
    QObject::connect(this->midiInCombo, SIGNAL(activated(int)), this, SLOT(midiInDeviceChanged(int)));
    QObject::connect(this->midiOutCombo, SIGNAL(activated(int)), this, SLOT(midiOutDeviceChanged(int)));
    QObject::connect(this->midiInCombo, SIGNAL(highlighted(int)), this, SLOT(midiInDeviceChanged(int)));
    QObject::connect(this->midiOutCombo, SIGNAL(highlighted(int)), this, SLOT(midiOutDeviceChanged(int)));
    updateDevice();

    timer->start(1000);   

    QCheckBox *autoCheckBox = new QCheckBox(QObject::tr("Auto select SY-1000 USB"));
    this->autoCheckBox = autoCheckBox;
    if(this->device=="true")
    {
        autoCheckBox->setChecked(true);
    };
    QObject::connect(autoCheckBox, SIGNAL(stateChanged(int)), this, SLOT(checkBoxChanged(int)));

    QVBoxLayout *midiLabelLayout = new QVBoxLayout;
    midiLabelLayout->addWidget(midiInLabel);
    midiLabelLayout->addWidget(midiOutLabel);
    midiLabelLayout->addStretch();

    QVBoxLayout *midiComboLayout = new QVBoxLayout;
    midiComboLayout->addWidget(midiInCombo);
    midiComboLayout->addWidget(midiOutCombo);
    midiComboLayout->addWidget(autoCheckBox);

    QHBoxLayout *midiSelectLayout = new QHBoxLayout;
    midiSelectLayout->addLayout(midiLabelLayout);
    midiSelectLayout->addLayout(midiComboLayout);

    QVBoxLayout *midiDevLayout = new QVBoxLayout;
    midiDevLayout->addWidget(mididescriptionLabel);
    midiDevLayout->addSpacing(12*ratio);
    midiDevLayout->addLayout(midiSelectLayout);

    QVBoxLayout *midiLayout = new QVBoxLayout;
    midiLayout->addLayout(midiDevLayout);
    midiGroup->setLayout(midiLayout);

    QGroupBox *advancedSettingsGroup = new QGroupBox(QObject::tr("Advanced settings"));

    QLabel *midiTxChDescriptionLabel = new QLabel(tr("Midi Tx Channel:"));

    QCheckBox *dBugCheckBox = new QCheckBox(QObject::tr("deBug Mode (not for normal use)"));
    QSpinBox *midiTxChSpinBox = new QSpinBox;

    this->dBugCheckBox = dBugCheckBox;
    if(dBugScreen=="true")
    {
        dBugCheckBox->setChecked(true);
    };

    this->midiTxChSpinBox = midiTxChSpinBox;
    const int tempDataWrite = preferences->getPreferences("Midi", "TxCh", "set").toInt(&ok, 10);
    midiTxChSpinBox->setValue(tempDataWrite);
    midiTxChSpinBox->setRange(1, 16);
    midiTxChSpinBox->setPrefix("Channel ");

    QVBoxLayout *advancedBoxLayout = new QVBoxLayout;
    advancedBoxLayout->addWidget(midiTxChDescriptionLabel);
    advancedBoxLayout->addWidget(midiTxChSpinBox);
    advancedBoxLayout->addSpacing(20*ratio);
    advancedBoxLayout->addWidget(dBugCheckBox);

    QHBoxLayout *advancedLayout = new QHBoxLayout;
    advancedLayout->addLayout(advancedBoxLayout);

    advancedSettingsGroup->setLayout(advancedLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(midiGroup);
    mainLayout->addWidget(advancedSettingsGroup);
    mainLayout->addStretch(1);
    setLayout(mainLayout);
}

void MidiPage::updateDevice()
{
    midiIO *midi = midiIO::Instance();
    Preferences *preferences = Preferences::Instance();
    QString midiInDevice = preferences->getPreferences("Midi", "MidiIn", "name");
    QString midiOutDevice = preferences->getPreferences("Midi", "MidiOut", "name");
    QString device = preferences->getPreferences("Midi", "Device", "bool");
    if(device=="true"){midiInCombo->setDisabled(true);}else{midiInCombo->setDisabled(false);};

    QList<QString> midiInDevices = midi->getMidiInDevices();
    QList<QString> midiOutDevices = midi->getMidiOutDevices();

    midiInCombo->clear();

    for (QList<QString>::iterator dev = midiInDevices.begin(); dev != midiInDevices.end(); ++dev)
    {
        QString str(*dev);
        str.toLatin1().data();
        if(str.isEmpty() && device=="true"){midiInCombo->addItem("SY-1000 USB not found");};
        midiInCombo->addItem(str);
    };

    if(midiInDevices.contains(midiInDevice)){midiInCombo->setCurrentIndex(midiInDevices.indexOf(midiInDevice));};
    if(device=="true"){midiOutCombo->setDisabled(true);}else{midiOutCombo->setDisabled(false);};

    if (midiInDevices.contains("SY-1000")  && device=="true")
    {
        midiInCombo->setCurrentIndex(midiInDevices.indexOf("SY-1000"));
        this->midiInDeviceName="SY-1000";
    };

    midiOutCombo->clear();

    for (QList<QString>::iterator dev = midiOutDevices.begin(); dev != midiOutDevices.end(); ++dev)
    {
        QString str(*dev);
        midiOutCombo->addItem(str.toLatin1().data());
    };

    if(midiOutDevices.contains(midiOutDevice)){midiOutCombo->setCurrentIndex(midiOutDevices.indexOf(midiOutDevice));};

    if ( midiOutDevices.contains("SY-1000") && device=="true" )
    {
        midiOutCombo->setCurrentIndex(midiOutDevices.indexOf("SY-1000"));
        this->midiOutDeviceName="SY-1000";
    };
}

void MidiPage::midiInDeviceChanged(int value)
{
    Preferences *preferences = Preferences::Instance();
    midiIO *midi = midiIO::Instance();
    QList<QString> midiInDevices = midi->getMidiInDevices();
    if(value < 0 || value >= midiInDevices.size())
    {
        return;
    }
    midiInDeviceID = value;
    midiInDeviceName = midiInDevices.at(value);
    if(midiInDevices.contains(midiInDeviceName)){midiInCombo->setCurrentIndex(midiInDevices.indexOf(midiInDeviceName));};
    preferences->setPreferences("Midi", "MidiIn", "device", QString::number(value, 10));
    preferences->setPreferences("Midi", "MidiIn", "name", midiInDeviceName);
    if(preferences->getPreferences("Midi", "DBug", "bool")=="true")
    {
        SysxIO *sysxIO = SysxIO::Instance();
        sysxIO->deBug(QString("in device = "+QString::number(value)+" "+midiInDeviceName));
    };
}

void MidiPage::midiOutDeviceChanged(int value)
{
    Preferences *preferences = Preferences::Instance();

    midiIO *midi = midiIO::Instance();
    QList<QString> midiOutDevices = midi->getMidiOutDevices();
    if(value < 0 || value >= midiOutDevices.size())
    {
        return;
    }
    midiOutDeviceID = value;
    midiOutDeviceName = midiOutDevices.at(value);
    if(midiOutDevices.contains(midiOutDeviceName)){midiOutCombo->setCurrentIndex(midiOutDevices.indexOf(midiOutDeviceName));};
    preferences->setPreferences("Midi", "MidiOut", "device", QString::number(value, 10));
    preferences->setPreferences("Midi", "MidiOut", "name", midiOutDeviceName);
    if(preferences->getPreferences("Midi", "DBug", "bool")=="true")
    {
        SysxIO *sysxIO = SysxIO::Instance();
        sysxIO->deBug(QString("out device = "+QString::number(value)+" "+midiOutDeviceName));
    };
}

void MidiPage::checkBoxChanged(int value)
{
    Preferences *preferences = Preferences::Instance();
    QString auto_device = "false";
    if(value>0){auto_device = "true";};
    preferences->setPreferences("Midi", "Device", "bool", auto_device);
}


WindowPage::WindowPage(QWidget *parent)
    : QWidget(parent)
{
    Preferences *preferences = Preferences::Instance();
    QString windowRestore = preferences->getPreferences("Window", "Restore", "window");
    QString sidepanelRestore = preferences->getPreferences("Window", "Restore", "sidepanel");
    QString splashScreen = preferences->getPreferences("Window", "Splash", "bool");
    QString SingleWindow = preferences->getPreferences("Window", "Single", "bool");
    QString WidgetsUse = preferences->getPreferences("Window", "AutoScale", "bool");

    QGroupBox *windowGroup = new QGroupBox(QObject::tr("Window settings"));

    QLabel *restoreDescriptionLabel = new QLabel(QObject::tr("Select if you want the window position to be saved on exit."));
    QCheckBox *windowCheckBox = new QCheckBox(QObject::tr("Restore window"));
    QCheckBox *sidepanelCheckBox = new QCheckBox(QObject::tr("Restore sidepanel"));
    QCheckBox *singleWindowCheckBox = new QCheckBox(QObject::tr("Single Window Layout"));
    QCheckBox *autoRatioCheckBox = new QCheckBox(QObject::tr("Auto Window Scaling"));
    this->windowCheckBox = windowCheckBox;
    this->sidepanelCheckBox = sidepanelCheckBox;
    this->singleWindowCheckBox = singleWindowCheckBox;
    this->autoRatioCheckBox = autoRatioCheckBox;

    if(windowRestore=="true") { windowCheckBox->setChecked(true); };
    if(sidepanelRestore=="true") { sidepanelCheckBox->setChecked(true); };
    if(SingleWindow=="true") { singleWindowCheckBox->setChecked(true); };
    if(WidgetsUse=="true") { autoRatioCheckBox->setChecked(true); };

    QDoubleSpinBox *ratioSpinBox = new QDoubleSpinBox;
    bool ok;
    this->ratioSpinBox = ratioSpinBox;
    const double ratio = preferences->getPreferences("Window", "Scale", "ratio").toDouble(&ok);
    ratioSpinBox->setDecimals(2);
    ratioSpinBox->setValue(ratio);
    ratioSpinBox->setRange(0.5, 10.0);
    ratioSpinBox->setSingleStep(0.01);
    ratioSpinBox->setPrefix("Window Resizing Scale = ");
    ratioSpinBox->setSuffix(QObject::tr(" :1"));

    QDoubleSpinBox *fontRatioSpinBox = new QDoubleSpinBox;
    this->fontRatioSpinBox = fontRatioSpinBox;
    const double fratio = preferences->getPreferences("Window", "Font", "ratio").toDouble(&ok);
    fontRatioSpinBox->setDecimals(2);
    fontRatioSpinBox->setValue(fratio);
    fontRatioSpinBox->setRange(0.5, 10.0);
    fontRatioSpinBox->setSingleStep(0.01);
    fontRatioSpinBox->setPrefix("Text Font Scale = ");
    fontRatioSpinBox->setSuffix(QObject::tr(" :1"));


    QVBoxLayout *restoreLayout = new QVBoxLayout;
    restoreLayout->addWidget(restoreDescriptionLabel);
    restoreLayout->addSpacing(12*ratio);
    restoreLayout->addWidget(windowCheckBox);
    restoreLayout->addWidget(sidepanelCheckBox);
    restoreLayout->addWidget(singleWindowCheckBox);
    restoreLayout->addWidget(autoRatioCheckBox);
    restoreLayout->addWidget(ratioSpinBox);
    restoreLayout->addWidget(fontRatioSpinBox);

    QVBoxLayout *windowLayout = new QVBoxLayout;
    windowLayout->addLayout(restoreLayout);
    windowGroup->setLayout(windowLayout);

    QGroupBox *splashScreenGroup = new QGroupBox(QObject::tr("Show splash screen"));

    QLabel *splashDescriptionLabel = new QLabel(QObject::tr("Disable or enable the splash screen."));
    QCheckBox *splashCheckBox = new QCheckBox(QObject::tr("Splash screen"));
    this->splashCheckBox = splashCheckBox;

    if(splashScreen=="true") { splashCheckBox->setChecked(true); };

    QVBoxLayout *splashLayout = new QVBoxLayout;
    splashLayout->addWidget(splashDescriptionLabel);
    splashLayout->addSpacing(12*ratio);
    splashLayout->addWidget(splashCheckBox);

    QVBoxLayout *splashScreenLayout = new QVBoxLayout;
    splashScreenLayout->addLayout(splashLayout);
    splashScreenGroup->setLayout(splashScreenLayout);
    
    QLabel *note = new QLabel(QObject::tr("<b>***Changes take effect on next startup***</b>" ));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(windowGroup);
    mainLayout->addWidget(splashScreenGroup);
    mainLayout->addWidget(note);

    mainLayout->addStretch(1);
    setLayout(mainLayout);
}

LanguagePage::LanguagePage(QWidget *parent)
    : QWidget(parent)
{
    Preferences *preferences = Preferences::Instance();
    QString lang = preferences->getPreferences("Language", "Locale", "select");
    bool ok;
    int choice = lang.toInt(&ok, 16);
    const double ratio = preferences->getPreferences("Window", "Font", "ratio").toDouble(&ok);

    QGroupBox *languageGroup = new QGroupBox(QObject::tr("Language Selection"));

    QRadioButton *englishButton = new QRadioButton(QObject::tr("English"));
    //this->englishButton = englishButton;
    this->frenchButton = new QRadioButton(QObject::tr("French"));
    this->germanButton = new QRadioButton(QObject::tr("German"));
    this->portugueseButton = new QRadioButton(QObject::tr("Portuguese"));
    this->spanishButton = new QRadioButton(QObject::tr("Spanish"));
    this->japaneseButton = new QRadioButton(QObject::tr("Japanese"));
    this->chineseButton = new QRadioButton(QObject::tr("Chinese (simplified)"));
    if (choice == 6)      {chineseButton->setChecked(true); }
    else if (choice == 5) {japaneseButton->setChecked(true); }
    else if (choice == 4) {spanishButton->setChecked(true); }
    else if (choice == 3) {portugueseButton->setChecked(true); }
    else if (choice == 2) {germanButton->setChecked(true); }
    else if (choice == 1) {frenchButton->setChecked(true); }
    else {englishButton->setChecked(true); };

    QVBoxLayout *languageLayout = new QVBoxLayout;
    languageLayout->addWidget(englishButton);
    languageLayout->addWidget(frenchButton);
    languageLayout->addWidget(germanButton);
    languageLayout->addWidget(portugueseButton);
    languageLayout->addWidget(spanishButton);
    languageLayout->addWidget(japaneseButton);
    languageLayout->addWidget(chineseButton);

    languageGroup->setLayout(languageLayout);

    QLabel *note = new QLabel(QObject::tr("<b>***Changes take effect on next startup***</b>" ));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(languageGroup);
    languageLayout->addSpacing(12*ratio);
    mainLayout->addStretch(1);
    mainLayout->addWidget(note);
    setLayout(mainLayout);
}

StylePage::StylePage(QWidget *parent)
    : QWidget(parent)
{
    Preferences *preferences = Preferences::Instance();
    QString setting = preferences->getPreferences("Scheme", "Style", "select");
    bool ok;
    int choice = setting.toInt(&ok, 16);
    const double ratio = preferences->getPreferences("Window", "Font", "ratio").toDouble(&ok);

    QGroupBox *styleGroup = new QGroupBox(QObject::tr("Window Style Selection"));
    QString plastique = "Plastique";
    QString cleanlooks = "CleanLooks";
    QString motif = "Motif";
#if defined(Q_OS_WIN)
    plastique = "WindowsVista";
    cleanlooks = "WindowsXp";
    motif = "Windows";
#endif
#if defined(Q_OS_MAC)
    plastique = "Macintosh";
    cleanlooks = "CDE";
    motif = "Windows";
#endif
    QRadioButton *standardButton = new QRadioButton("Fusion");
    this->standardButton = standardButton;
    this->plastiqueButton = new QRadioButton(plastique);
    this->cdeButton = new QRadioButton(cleanlooks);
    this->motifButton = new QRadioButton(motif);
    if (choice == 3) {motifButton->setChecked(true); }
    else if (choice == 2) {cdeButton->setChecked(true); }
    else if (choice == 1) {plastiqueButton->setChecked(true); }
    else {standardButton->setChecked(true); };

    QVBoxLayout *styleLayout = new QVBoxLayout;
    styleLayout->addWidget(standardButton);
    styleLayout->addWidget(plastiqueButton);
    styleLayout->addWidget(cdeButton);
    styleLayout->addWidget(motifButton);

    styleGroup->setLayout(styleLayout);


    setting = preferences->getPreferences("Scheme", "Colour", "select");
    choice = setting.toInt(&ok, 16);
    QGroupBox *colourGroup = new QGroupBox(QObject::tr("Colour Scheme Selection"));


    QRadioButton *blueButton = new QRadioButton(QObject::tr("BTS Black"));
    this->blueButton = blueButton;
    this->blackButton = new QRadioButton(QObject::tr("FloorBoard"));
    this->aquaButton = new QRadioButton(QObject::tr("Aqua"));
    this->greenButton = new QRadioButton(QObject::tr("Gumtown Green"));
    this->whiteButton = new QRadioButton(QObject::tr("Snow White"));
    if (choice == 4) {whiteButton->setChecked(true); }
    else if (choice == 3) {greenButton->setChecked(true); }
    else if (choice == 2) {aquaButton->setChecked(true); }
    else if (choice == 1) {blackButton->setChecked(true); }
    else {blueButton->setChecked(true); };

    QVBoxLayout *colourLayout = new QVBoxLayout;
    colourLayout->addWidget(blueButton);
    colourLayout->addWidget(blackButton);
    colourLayout->addWidget(aquaButton);
    colourLayout->addWidget(greenButton);
    colourLayout->addWidget(whiteButton);

    colourGroup->setLayout(colourLayout);

    QLabel *note = new QLabel(QObject::tr("<b>***Changes take effect on next startup***</b>" ));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(styleGroup);
    mainLayout->addWidget(colourGroup);
    styleLayout->addSpacing(12*ratio);
    mainLayout->addStretch(1);
    mainLayout->addWidget(note);
    setLayout(mainLayout);
}

ModePage::ModePage(QWidget *parent)
    : QWidget(parent)
{
    bool ok;
    Preferences *preferences = Preferences::Instance();
    QString AutoMode = preferences->getPreferences("Window", "AutoMode", "bool");
    QString BassMode = preferences->getPreferences("Window", "BassMode", "bool");
    const double ratio = preferences->getPreferences("Window", "Scale", "ratio").toDouble(&ok);

    QLabel *modeDescriptionLabel = new QLabel(QObject::tr("Select startup Mode"));
    QCheckBox *autoModeCheckBox = new QCheckBox(QObject::tr("Auto Mode Selection"));
    this->autoModeCheckBox = autoModeCheckBox;
    this->guitarModeButton = new QRadioButton(QObject::tr("Guitar Mode"));
    this->bassModeButton = new QRadioButton(QObject::tr("Bass Mode"));

    if(AutoMode=="true") { autoModeCheckBox->setChecked(true); this->guitarModeButton->setDisabled(true); this->bassModeButton->setDisabled(true);}
    else{ autoModeCheckBox->setChecked(false); this->guitarModeButton->setDisabled(false); this->bassModeButton->setDisabled(false);};

    if(BassMode=="true") {guitarModeButton->setChecked(false);  bassModeButton->setChecked(true); }
    else{guitarModeButton->setChecked(true);  bassModeButton->setChecked(false); };

    QGroupBox *editorModeGroup = new QGroupBox(QObject::tr("Editor Guitar/Bass Mode"));


    QVBoxLayout *modeLayout = new QVBoxLayout;
    modeLayout->addWidget(modeDescriptionLabel);
    modeLayout->addSpacing(12*ratio);
    modeLayout->addWidget(autoModeCheckBox);
    modeLayout->addWidget(guitarModeButton);
    modeLayout->addWidget(bassModeButton);

    QVBoxLayout *editorModeLayout = new QVBoxLayout;
    editorModeLayout->addLayout(modeLayout);
    editorModeGroup->setLayout(editorModeLayout);

    QLabel *note = new QLabel(QObject::tr("<b>***Changes take effect on next startup***</b>" ));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(editorModeGroup);
    mainLayout->addWidget(note);

    mainLayout->addStretch(1);
    setLayout(mainLayout);

    QObject::connect(this->autoModeCheckBox, SIGNAL(stateChanged(int)), this, SLOT(updateButton()));
}

void ModePage::updateButton()
{
    //Preferences *preferences = Preferences::Instance();
    //QString AutoMode = preferences->getPreferences("Window", "AutoMode", "bool");

    if(autoModeCheckBox->isChecked())
    {
        autoModeCheckBox->setChecked(true);
        this->guitarModeButton->setDisabled(true);
        this->bassModeButton->setDisabled(true);
    }
    else
    {
        autoModeCheckBox->setChecked(false);
        this->guitarModeButton->setDisabled(false);
        this->bassModeButton->setDisabled(false);
    };

}

void GeneralPage::browseDir()
{
    QString dirName = QFileDialog::getExistingDirectory(this, QObject::tr("Select the default folder for storing patches."),
                                                        this->dirEdit->text(),
                                                        QFileDialog::ShowDirsOnly);
    if(!dirName.isEmpty())
    {
        this->dirEdit->setText(dirName);
    };
}
