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

#include <QFileInfo>
#include <QFile>
#include <QDir>
#include <QRegularExpression>
#include "SysxIO.h"
#include "sysxWriter.h"
#include "Preferences.h"
#include "appservices.h"
#include "initPatchListMenu.h"
QRegularExpression rexp("$(/)");
QRegularExpression qexp(".{1}(mid|syx|tsl)");

initPatchListMenu::initPatchListMenu(QRect geometry, QWidget *parent)
    : QWidget(parent)
{
    this->available = false;
    this->flatMode = false;
    setInitPatchComboBox(geometry);

    QObject::connect(this->parent()->parent(), SIGNAL(updateSignal()), this, SLOT(updateDisplay()));

}

initPatchListMenu::~initPatchListMenu()
{
    this->deleteLater();
}

QDir initPatchListMenu::getInitPatchDir()
{
    Preferences *preferences = &AppServices::instance().preferences();
    QDir preferencesDir = QDir(preferences->getPreferences("General", "Files", "dir").remove(rexp));
    QString initPatchesDirName = "saved_patches";
    QString symlinkExstention;

#ifdef Q_OS_UNIX
    symlinkExstention = "";
#endif

#ifdef Q_OS_WIN
    symlinkExstention = ".lnk";
#endif

    QDir initPatchesDir; /* The "init_patches" directory. */
    if ( QFileInfo( preferencesDir, initPatchesDirName + symlinkExstention ).exists() &&
         QFileInfo( preferencesDir, initPatchesDirName + symlinkExstention ).canonicalPath() ==
         QFileInfo( preferencesDir, initPatchesDirName + symlinkExstention ).symLinkTarget() )
    {	/* If the "Init Pathces" directory is a symlink and lives in the user sellected patch directory. */
        initPatchesDir.setPath(QFileInfo( preferencesDir, initPatchesDirName + symlinkExstention ).symLinkTarget());
    }
    else if ( QFileInfo( preferencesDir, initPatchesDirName ).exists() )
    {	/* If the "Init Pathces" directory lives in the user sellected patch directory. */
        initPatchesDir.setPath(QFileInfo( preferencesDir, initPatchesDirName ).absoluteFilePath() );
    }
    else if ( QFileInfo::exists(initPatchesDirName))
    {	/* If the "Init Pathces" directory lives in the application directory. */
        initPatchesDir.setPath(QFileInfo( initPatchesDirName ).absoluteFilePath());
        if( QFileInfo( preferencesDir, initPatchesDirName ).absolutePath() != initPatchesDir.absolutePath() &&
                preferencesDir.exists() )
        {	/* Add a symlink to the user selected patch directory (if it is set and exists). */
            QString symlinkPath = QFileInfo( preferencesDir, initPatchesDirName + symlinkExstention ).absoluteFilePath();
            QFile::link(initPatchesDir.absolutePath(), symlinkPath);
        };
    };
    return initPatchesDir;
}

void initPatchListMenu::setInitPatchComboBox(QRect geometry)
{
    Preferences *preferences = &AppServices::instance().preferences();
    bool ok;
    const double ratio = preferences->getPreferences("Window", "Font", "ratio").toDouble(&ok);
    
    this->initPatchComboBox = new QComboBox(this);
    this->available = true;
    this->initPatchComboBox->setObjectName("smallcombo");
    QFont Sfont( "Roboto Condensed", 8*ratio, QFont::Normal);
    this->initPatchComboBox->setFont(Sfont);
    initPatchComboBox->addItem(tr("[ Quick load patches ]"));
    initPatchComboBox->view()->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    updateCombo();

    // Use a layout so the combo fills this widget when managed by a parent layout.
    QHBoxLayout *innerLayout = new QHBoxLayout(this);
    innerLayout->setContentsMargins(0, 0, 0, 0);
    innerLayout->addWidget(initPatchComboBox);

    initPatchComboBox->setEditable(false);
    initPatchComboBox->setFrame(false);
    initPatchComboBox->setMaxVisibleItems(initPatches.count()+1);
    applyComboStyle();

    QObject::connect(initPatchComboBox, qOverload<int>(&QComboBox::activated),
                     this, &initPatchListMenu::loadInitPatch);

    QObject::connect(this, SIGNAL(updateSignal()),
                     this->parent()->parent(), SIGNAL(updateSignal()));

    SysxIO *sysxIO = &AppServices::instance().sysx();
    QObject::connect(sysxIO, &SysxIO::updateSignal, this, &initPatchListMenu::updateCombo);
}

void initPatchListMenu::setFlatMode(bool flat)
{
    this->flatMode = flat;
    applyComboStyle();
}

void initPatchListMenu::applyComboStyle()
{
    if(!this->initPatchComboBox)
    {
        return;
    }

    if(this->flatMode)
    {
        this->initPatchComboBox->setStyleSheet(
            "QComboBox {"
            " background: transparent;"
            " border: 0px;"
            " color: rgb(185,224,243);"
            " padding-left: 4px;"
            "}"
            "QComboBox::drop-down {"
            " background: transparent;"
            " border: 0px;"
            " width: 18px;"
            "}"
            "QComboBox::down-arrow {"
            " image: url(:/images/down_arrow.png);"
            " width: 9px;"
            " height: 10px;"
            "}");
    }
    else
    {
        this->initPatchComboBox->setStyleSheet(QString());
    }
}


void initPatchListMenu::updateCombo()
{
    if(!this->initPatchComboBox)
    {
        return;
    }

    // Do not rebuild while the popup is open; it interrupts wheel/scroll use.
    if(this->initPatchComboBox->view() && this->initPatchComboBox->view()->isVisible())
    {
        return;
    }

    this->current = initPatchComboBox->currentIndex();
    this->initPatches.clear();
    QSignalBlocker blocker(this->initPatchComboBox);
    this->initPatchComboBox->clear();
    initPatchComboBox->addItem(tr("[ Quick load patches ]"));
    QDir initPatchesDir = getInitPatchDir();
    QStringList filters;
    filters << "*.mid" << "*.syx" << "*.tsl";
    QDir::Filters df = QDir::AllDirs | QDir::NoDotAndDotDot;
    QDirIterator::IteratorFlag dff = QDirIterator::Subdirectories;
    QString root = initPatchesDir.filePath("");
    //std::string sr = root.toStdString();
    //std::cerr << sr << std::endl;
    QDirIterator it(root,df,dff);
    int x=0;

    while(it.hasNext() && x<20)
    {
        QString lt = it.next();
        QString prefix = "[" + lt.mid(lt.lastIndexOf("/")+1, 20).trimmed() + "] ";
        QStringList initPatchesList = QDir(lt).entryList(filters);
        int itemcount;
        for(itemcount=0; itemcount<initPatchesList.size(); itemcount++)
        {	/* Filling the combobox with the patches. */
            QString path = QDir(lt).absolutePath().append("/").append(initPatchesList.at(itemcount));
            this->initPatches.append(path);
            QString item = initPatchesList.at(itemcount);	// Start formatting the item name.
            item.remove(qexp);
            item.prepend(prefix);
            initPatchComboBox->addItem(item);				// Finished formatting the item name.
        };
        x++;
    };
    const int maxIndex = initPatchComboBox->count() - 1;
    const int safeIndex = qBound(0, current, maxIndex);
    initPatchComboBox->setCurrentIndex(safeIndex);
    current = safeIndex;
    // Keep the popup usable when many preset files exist.
    initPatchComboBox->setMaxVisibleItems(qMin(20, initPatchComboBox->count()));
}

void initPatchListMenu::setIndex(int index)
{
    Q_UNUSED(index);
    if(this->available)
    {
       // this->initPatchComboBox->setCurrentIndex(index);
        this->initPatchComboBox->setCurrentIndex(current);
    };
}

void initPatchListMenu::loadInitPatch(int index)
{
    if(index > 0)
    {
        QString fileName = this->initPatches.at(index-1);
        if (!fileName.isEmpty())
        {
            sysxWriter file;
            file.setFile(fileName);
            if(file.readFile())
            {
                // DO SOMETHING AFTER READING THE FILE (UPDATE THE GUI)
                SysxIO *sysxIO = &AppServices::instance().sysx();
                sysxIO->setFileSource("Structure", file.getFileSource());
                sysxIO->setFileName(tr("saved patch"));
                sysxIO->setSyncStatus(false);
                sysxIO->setDevice(false);
                if(sysxIO->isConnected()) { sysxIO->writeToBuffer(); };
                emit updateSignal();
                updateCombo();
            };
        };
    };
}

void initPatchListMenu::highLightInitPatch(int index)
{
    Q_UNUSED(index);
}

void initPatchListMenu::updateDisplay()
{
    SysxIO *sysxIO = &AppServices::instance().sysx();
    QString name = sysxIO->getFileName();
    if(name != "saved patch"){this->initPatchComboBox->setCurrentIndex(current); };
}
