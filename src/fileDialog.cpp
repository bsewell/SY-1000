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
#include "fileDialog.h"
#include "Preferences.h"


fileDialog::fileDialog(QString fileName, QList<QString> patchList)
{
    Preferences *preferences = Preferences::Instance();
    bool ok;
    const double ratio = preferences->getPreferences("Window", "Font", "ratio").toDouble(&ok);
    const double fratio = preferences->getPreferences("Window", "Font", "ratio").toDouble(&ok);
    QFont Sfont( "Roboto Condensed", 9*fratio, QFont::Normal);
    this->setFont(Sfont);

    QLabel *patchLabel = new QLabel(tr("Select patch file to open<br><br>Double click to load"));
    QLabel *nameLabel = new QLabel(fileName);
    QListWidget *contentsWidget = new QListWidget;
    contentsWidget->selectionModel()->selectedIndexes();
    contentsWidget->addItems(patchList);
    int listHeight = patchList.size()*30*ratio;
    if(listHeight>500*ratio){listHeight=500*ratio;};
    contentsWidget->setFixedHeight(listHeight);

    contentsWidget->setWhatsThis(tr("To auditon a multi-patch file, hover the mouse cursor over a patch and the patch data will be loaded into the SY-1000 temporary buffer"
                                "<br>a click on the patch will load it into the editor."));

    QObject::connect(contentsWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
                     this, SLOT(valueChanged(QListWidgetItem*)));


    QObject::connect(contentsWidget, SIGNAL(itemClicked(QListWidgetItem*)),
                     this, SLOT(audition(QListWidgetItem*)));

    QPushButton *cancelButton = new QPushButton(tr("Cancel"));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancel()));
    cancelButton->setWhatsThis(tr("Selecting this will close the patch load window and reset the SY-1000 back to the current editor patch."));

    QHBoxLayout *horizontalLayout = new QHBoxLayout;
    horizontalLayout->addWidget(patchLabel);
    horizontalLayout->addWidget(contentsWidget);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch(1);
    buttonsLayout->addWidget(cancelButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(nameLabel);
    mainLayout->addLayout(horizontalLayout);
    mainLayout->addStretch(1);
    mainLayout->addSpacing(12);
    mainLayout->addLayout(buttonsLayout);
    setLayout(mainLayout);

    setWindowTitle(tr("Multiple Patch File"));

    SysxIO *sysxIO = SysxIO::Instance();
    previous_file_name = sysxIO->getFileName();
    //contentsWidget->setS
/*
    uint count=0;
    for (QList< QList<QString> >::iterator dev = sysxIO->fileSource.hex.begin(); dev != sysxIO->fileSource.hex.end(); ++dev)
    {
        QList<QString> outData(*dev);
        for (QList<QString>::iterator code = outData.begin(); code != outData.end(); ++code)
        {
            QString str(*code);
            bool ok;
            uint n = str.toInt(&ok, 16);
            previous_patch[count] = char(n);
            count++;
        };
    };*/
}

void fileDialog::valueChanged(QListWidgetItem *item)
{  
    SysxIO *sysxIO = SysxIO::Instance();
    bool ok;
    QString text = item->text();
    text.truncate(text.indexOf(":")-1);
    int value = text.toInt(&ok, 10);

    sysxIO->patchListValue = value;
    this->close();    
}

void fileDialog::cancel()
{
    SysxIO *sysxIO = SysxIO::Instance();    
    sysxIO->writeToBuffer();
    sysxIO->patchListValue = 0;
    sysxIO->setFileName(previous_file_name);
    sysxIO->setFileSource("Structure", previous_patch);
    this->close();
}

void fileDialog::audition(QListWidgetItem *item)
{
    Q_UNUSED(item);
    SysxIO *sysxIO = SysxIO::Instance();
    sysxIO->setFileName("patch audition");
    int value = 1;
    bool ok;
    QString k = item->text();
    k.truncate(k.indexOf(":")-1);
    value = k.toInt(&ok, 10);
    sysxIO->patchListValue = value;
}


void fileDialog::mouseRightClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
}


