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
#include "preferencesDialog.h"
#include "preferencesPages.h"
#include "Preferences.h"


preferencesDialog::preferencesDialog(bool mode)
{
    Preferences *preferences = Preferences::Instance();
    bool ok;
    const double ratio = preferences->getPreferences("Window", "Scale", "ratio").toDouble(&ok);
    const double fratio = preferences->getPreferences("Window", "Font", "ratio").toDouble(&ok);

    QFont Mfont( "Roboto Condensed", 9*fratio, QFont::Normal);

    contentsWidget = new QListWidget;
    contentsWidget->setViewMode(QListView::IconMode);
    contentsWidget->setIconSize(QSize(45*ratio, 40*ratio));
    contentsWidget->setMovement(QListView::Static);
    contentsWidget->setSpacing(10*ratio);


    MidiPage *midiSettings = new MidiPage;
    WindowPage *windowSettings = new WindowPage;
    GeneralPage *generalSettings = new GeneralPage;
    LanguagePage *languageSettings = new LanguagePage;
    StylePage *styleSettings = new StylePage;
    ModePage *modeSettings = new ModePage;


    this->midiSettings = midiSettings;
    this->modeSettings = modeSettings;
    this->windowSettings = windowSettings;
    this->generalSettings = generalSettings;
    this->languageSettings = languageSettings;
    this->styleSettings = styleSettings;


    pagesWidget = new QStackedWidget;

    pagesWidget->addWidget(midiSettings);
    if(mode)
    {
        pagesWidget->addWidget(modeSettings);
        pagesWidget->addWidget(windowSettings);
        pagesWidget->addWidget(generalSettings);
        pagesWidget->addWidget(languageSettings);
        pagesWidget->addWidget(styleSettings);
    };

    this->setFont(Mfont);
    QPushButton *okButton = new QPushButton(tr("Ok"));
    QPushButton *cancelButton = new QPushButton(tr("Cancel"));

    createIcons(mode);
    contentsWidget->setCurrentRow(0);

    connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(close()));

    QHBoxLayout *horizontalLayout = new QHBoxLayout;
    horizontalLayout->addWidget(contentsWidget);
    horizontalLayout->addWidget(pagesWidget, 1);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch(1);
    buttonsLayout->addWidget(okButton);
    buttonsLayout->addWidget(cancelButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(horizontalLayout);
    mainLayout->addStretch(1);
    mainLayout->addSpacing(12*ratio);
    mainLayout->addLayout(buttonsLayout);
    setLayout(mainLayout);

    QString title = tr("SY-1000 FloorBoard - Settings/Preferences");
    if(!mode)
    {
        if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
        {
            title = "SY-1000 EDITOR CURRENTLY IN BASS MODE";}
        else
        {
            title = "SY-1000 EDITOR CURRENTLY IN GUITAR MODE";
        };
    };
    setWindowTitle(title);
}

void preferencesDialog::createIcons(bool mode)
{
    QListWidgetItem *midiButton = new QListWidgetItem(contentsWidget);
    midiButton->setIcon(QIcon(":/images/midi.png"));
    midiButton->setText(tr("USB/Midi"));
    midiButton->setTextAlignment(Qt::AlignHCenter);
    midiButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    if(mode)
    {
        QListWidgetItem *modeButton = new QListWidgetItem(contentsWidget);
        modeButton->setIcon(QIcon(":/images/mode_icon.png"));
        modeButton->setText(tr("Mode"));
        modeButton->setTextAlignment(Qt::AlignHCenter);
        modeButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

        QListWidgetItem *windowButton = new QListWidgetItem(contentsWidget);
        windowButton->setIcon(QIcon(":/images/window.png"));
        windowButton->setText(tr("Window"));
        windowButton->setTextAlignment(Qt::AlignHCenter);
        windowButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

        QListWidgetItem *generalButton = new QListWidgetItem(contentsWidget);
        generalButton->setIcon(QIcon(":/images/general.png"));
        generalButton->setText(tr("General"));
        generalButton->setTextAlignment(Qt::AlignHCenter);
        generalButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

        QListWidgetItem *languageButton = new QListWidgetItem(contentsWidget);
        languageButton->setIcon(QIcon(":/images/language.png"));
        languageButton->setText(tr("Language"));
        languageButton->setTextAlignment(Qt::AlignHCenter);
        languageButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

        QListWidgetItem *styleButton = new QListWidgetItem(contentsWidget);
        styleButton->setIcon(QIcon(":/images/style.png"));
        styleButton->setText(tr("Style"));
        styleButton->setTextAlignment(Qt::AlignHCenter);
        styleButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    };
    connect(contentsWidget,
            SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem *)),
            this, SLOT(changePage(QListWidgetItem *, QListWidgetItem*)));
}

void preferencesDialog::changePage(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (!current)
    {
        current = previous;
    };
    pagesWidget->setCurrentIndex(contentsWidget->row(current));
}
