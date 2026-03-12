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

#include <QLayout>
#include <QMessageBox>
#include <QFile>
#include <QDataStream>
#include <QByteArray>
#include "bankTreeList.h"
#include "Preferences.h"
#include "MidiTable.h"
#include "SysxIO.h"
#include "appservices.h"
#include "globalVariables.h"
#include <QTimer>

namespace {
enum BankItemRole
{
    BankRole = Qt::UserRole + 1,
    PatchRole
};
}

// Platform-dependent sleep routines.
#ifdef Q_OS_WIN
#include <windows.h>
#define SLEEP( milliseconds ) Sleep( (DWORD) milliseconds )
#else // Unix variants & Mac
#include <unistd.h>
#define SLEEP( milliseconds ) usleep( (unsigned long) (milliseconds * 1000.0) )
#endif

bankTreeList::bankTreeList(QWidget *parent)
    : QWidget(parent)
{
    this->systemRequested=false;
    this->autoLoadStartupUserPatchPending = true;
    this->presetModeActive = false;
    Preferences *preferences = &AppServices::instance().preferences();
    bool ok;
    const double ratio = preferences->getPreferences("Window", "Font", "ratio").toDouble(&ok);

    QFont font( "Roboto Condensed", 10*ratio, QFont::Normal);
    font.setStretch(90);
    this->treeList = newTreeList();
    this->treeList->setObjectName("banklist");
    this->treeList->setFont(font);
    this->treeList->installEventFilter(this);
    this->treeList->viewport()->installEventFilter(this);

    QObject::connect(treeList, &QTreeWidget::itemClicked, this, &bankTreeList::setItemClicked);
    QObject::connect(treeList, &QTreeWidget::itemDoubleClicked, this, &bankTreeList::setItemDoubleClicked);

    // Use QPushButtons instead of QTabBar — QTabBar was not receiving
    // mouse events in Qt 6 (zero Enter/Leave/Click events reached it).
    this->btnPreset = new QPushButton(tr("PRESET"), this);
    this->btnUser   = new QPushButton(tr("USER"),   this);
    this->btnPreset->setObjectName("bankModeBtn");
    this->btnUser->setObjectName("bankModeBtn");
    this->btnPreset->setCursor(QCursor(Qt::PointingHandCursor));
    this->btnUser->setCursor(QCursor(Qt::PointingHandCursor));
    this->btnPreset->setCheckable(true);
    this->btnUser->setCheckable(true);
    this->btnPreset->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    this->btnUser->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    this->btnPreset->setMinimumHeight(qMax(22, int(24 * ratio)));
    this->btnUser->setMinimumHeight(qMax(22, int(24 * ratio)));
    this->btnUser->setChecked(true);  // start in USER mode

    QString btnStyle = QStringLiteral(
        "QPushButton#bankModeBtn {"
        "  font-family: Arial; font-weight: bold; font-size: 11px;"
        "  color: #b0b0b0; background: #1a1a64;"
        "  border: 2px solid #787878; border-radius: 6px;"
        "  padding: 4px 8px; letter-spacing: 1px;"
        "}"
        "QPushButton#bankModeBtn:checked {"
        "  color: #ffffff; background: rgba(30, 30, 255, 150);"
        "  border-color: #9090ff;"
        "}"
        "QPushButton#bankModeBtn:hover {"
        "  background: rgba(2, 5, 255, 80);"
        "}"
    );
    this->btnPreset->setStyleSheet(btnStyle);
    this->btnUser->setStyleSheet(btnStyle);

    QObject::connect(this->btnPreset, &QPushButton::pressed, this, [this]() {
        qWarning("bankTreeList PRESET button pressed");
        this->btnPreset->setChecked(true);
        this->btnUser->setChecked(false);
        setBankListMode(0);
    });
    QObject::connect(this->btnUser, &QPushButton::pressed, this, [this]() {
        qWarning("bankTreeList USER button pressed");
        this->btnUser->setChecked(true);
        this->btnPreset->setChecked(false);
        setBankListMode(1);
    });

    SysxIO *sysxIO = &AppServices::instance().sysx();
    QObject::connect(this->parent(), SIGNAL(updateSignal()), this, SLOT(updateTree()));
    QObject::connect(sysxIO, &SysxIO::updateTreeSignal, this, &bankTreeList::updateTree);
    QObject::connect(sysxIO, &SysxIO::updateNameSignal, this, &bankTreeList::updateNameSignal);

    QHBoxLayout *modeLayout = new QHBoxLayout;
    modeLayout->setContentsMargins(1, 1, 1, 1);
    modeLayout->setSpacing(4);
    modeLayout->addWidget(btnPreset);
    modeLayout->addWidget(btnUser);

    QVBoxLayout *treeListLayout = new QVBoxLayout;
    treeListLayout->addLayout(modeLayout);
    treeListLayout->addWidget(treeList);
    treeListLayout->setContentsMargins(0, 0, 0, 0);
    treeListLayout->setSpacing(2);
    setLayout(treeListLayout);

    QTimer::singleShot(2000, this, [this]() {
        qWarning("bankTreeList tabs state widgetVisible=%d presetVisible=%d userVisible=%d presetGeom=%d,%d %dx%d userGeom=%d,%d %dx%d",
                 this->isVisible() ? 1 : 0,
                 this->btnPreset->isVisible() ? 1 : 0,
                 this->btnUser->isVisible() ? 1 : 0,
                 this->btnPreset->geometry().x(), this->btnPreset->geometry().y(),
                 this->btnPreset->geometry().width(), this->btnPreset->geometry().height(),
                 this->btnUser->geometry().x(), this->btnUser->geometry().y(),
                 this->btnUser->geometry().width(), this->btnUser->geometry().height());
    });

    QObject::connect(this, &bankTreeList::setStatusSymbol, sysxIO, &SysxIO::setStatusSymbol);
    QObject::connect(this, &bankTreeList::setStatusProgress, sysxIO, &SysxIO::setStatusProgress);
    QObject::connect(this, &bankTreeList::setStatusMessage, sysxIO, &SysxIO::setStatusMessage);
    QObject::connect(this, &bankTreeList::notConnectedSignal, sysxIO, &SysxIO::notConnectedSignal);

    setBankListMode(1);
}

void bankTreeList::updateSize(QRect newrect)
{
    this->setGeometry(newrect);
}

void bankTreeList::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    qWarning("bankTreeList showEvent widgetVisible=%d presetVisible=%d userVisible=%d presetGeom=%d,%d %dx%d userGeom=%d,%d %dx%d",
             this->isVisible() ? 1 : 0,
             this->btnPreset->isVisible() ? 1 : 0,
             this->btnUser->isVisible() ? 1 : 0,
             this->btnPreset->geometry().x(), this->btnPreset->geometry().y(),
             this->btnPreset->geometry().width(), this->btnPreset->geometry().height(),
             this->btnUser->geometry().x(), this->btnUser->geometry().y(),
             this->btnUser->geometry().width(), this->btnUser->geometry().height());
}

bool bankTreeList::eventFilter(QObject *watched, QEvent *event)
{
    if((watched == this->treeList || watched == this->treeList->viewport())
            && event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        QPoint localPos = mouseEvent->pos();
        if(watched == this->treeList->viewport())
        {
            localPos = this->treeList->viewport()->mapTo(this, mouseEvent->pos());
        }
        else
        {
            localPos = this->treeList->mapTo(this, mouseEvent->pos());
        }

        // Fallback: if the tree overlaps the top strip on some Qt/macOS builds,
        // still allow PRESET/USER tab switching from that click region.
        if(localPos.y() <= this->btnPreset->geometry().bottom() + 2)
        {
            if(this->btnPreset->geometry().contains(localPos))
            {
                this->btnPreset->setChecked(true);
                this->btnUser->setChecked(false);
                setBankListMode(0);
                return true;
            }
            if(this->btnUser->geometry().contains(localPos))
            {
                this->btnUser->setChecked(true);
                this->btnPreset->setChecked(false);
                setBankListMode(1);
                return true;
            }
        }
    }

    return QWidget::eventFilter(watched, event);
}

void bankTreeList::setClosedItems(QTreeWidgetItem *item)
{
    Q_UNUSED(item);
}

void bankTreeList::closeChildren(QTreeWidgetItem *item)
{
    Q_UNUSED(item);
}

void bankTreeList::setOpenItems(QTreeWidgetItem *item)
{
    Q_UNUSED(item);
}

QTreeWidget* bankTreeList::newTreeList()
{
    Preferences *preferences = &AppServices::instance().preferences();
    bool ok;
    const double ratio = preferences->getPreferences("Window", "Font", "ratio").toDouble(&ok);

    QFont Lfont( "Roboto Condensed", 10*ratio, QFont::Normal);
    const int rowHeight = QFontMetrics(Lfont).height() + 10;
    const QSize rowSizeHint(0, rowHeight);

    QTreeWidget *newTreeList = new QTreeWidget();
    newTreeList->setColumnCount(1);
    newTreeList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    newTreeList->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    QStringList headers;
    headers << QObject::tr("Select tree item to load patch");
    newTreeList->setHeaderLabels(headers);
    newTreeList->setRootIsDecorated(false);
    // Keep a fixed horizontal offset so PRESET/USER list text sits 10px
    // further right regardless of active theme stylesheet.
    newTreeList->setStyleSheet("QTreeWidget::item { padding-left: 10px; }");

    int x = 0;
    QList<QString> list;
    MidiTable *midiTable = &AppServices::instance().midiTable();
    Midi items;

    items = midiTable->getMidiMap("Tables", "00", "00", "19", "00"); // read Preset preset names list.
    for(int itemcount=0;itemcount<128;itemcount++ )
    {
        QString item;
        if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){item = items.level.at(itemcount).customdesc;}else{item = items.level.at(itemcount).desc;};
        list.append(item);
    };
    items = midiTable->getMidiMap("Tables", "00", "00", "19", "01"); // read Preset preset names list.
    for(int itemcount=0;itemcount<72;itemcount++ )
    {
        QString item;
        if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){item = items.level.at(itemcount).customdesc;}else{item = items.level.at(itemcount).desc;};
        list.append(item);
    };
    this->presetNames = QStringList(list.begin(), list.end());
    this->userItems.clear();
    this->presetItems.clear();
    b = 1;
    p = 1;
    Lfont.setUnderline(false);
    for (int c=1; c<=200; c++)
    {
        QString bnum = QString::number(b, 10);
        if(bnum.size()<2){bnum.prepend("0");}
        QString pnum = QString::number(p, 10);

        QTreeWidgetItem *userItem = new QTreeWidgetItem();
        userItem->setFont(0, Lfont);
        userItem->setText(0, "U" + bnum + ":" + pnum + " init patch");
        userItem->setData(0, BankRole, b);
        userItem->setData(0, PatchRole, p);
        userItem->setSizeHint(0, rowSizeHint);
        userItem->setWhatsThis(0, tr("User Patches.<br>A single mouse click will change patch and load data in."));
        this->userItems.append(userItem);

        QTreeWidgetItem *presetItem = new QTreeWidgetItem();
        presetItem->setFont(0, Lfont);
        presetItem->setText(0, "P" + bnum + ":" + pnum + " " + this->presetNames.at(x));
        presetItem->setData(0, BankRole, b + 50);
        presetItem->setData(0, PatchRole, p);
        presetItem->setSizeHint(0, rowSizeHint);
        presetItem->setWhatsThis(0, tr("Preset Patches.<br>A single mouse click will change patch and load data in."));
        this->presetItems.append(presetItem);

        p++;
        x++;
        if(p>4){p=1; b++;}
    }

    return newTreeList;
}

/*********************** setItemClicked() ***********************************
 * Expands and colapses on a single click and sets patch sellection.
 ****************************************************************************/
void bankTreeList::setItemClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);
    SysxIO *sysxIO = &AppServices::instance().sysx();
    const bool connected = sysxIO->isConnected();
    const int bank = item ? item->data(0, BankRole).toInt() : -1;
    const int patch = item ? item->data(0, PatchRole).toInt() : -1;
    qWarning("bankTreeList itemClicked mode=%s connected=%d bank=%d patch=%d text='%s'",
             this->presetModeActive ? "PRESET" : "USER",
             connected ? 1 : 0,
             bank,
             patch,
             item ? item->text(0).toUtf8().constData() : "<null>");

    if (item && sysxIO->isConnected())
    {
        if (!item->text(0).contains("Double-Click"))
        {
            if(bank <= 0 || patch <= 0) { return; }
            sysxIO->deBug("bankTreeList click mode="
                          + QString(this->presetModeActive ? "PRESET" : "USER")
                          + " bank=" + QString::number(bank)
                          + " patch=" + QString::number(patch));

            sysxIO->setMode("mode1");

            sysxIO->requestPatchChange(bank, patch);

            // Wait for the SY-1000 to confirm the patch change (COMMAND PATCH CHANGE
            // SysEx, ~3-4 s after request) before reading back patch data.
            // SysxIO::receiveEvent emits isChanged() when that confirmation arrives.
            // requestPatch() disconnects this connection at its top to make it one-shot.
            QObject::connect(sysxIO, &SysxIO::isChanged,
                             this,   qOverload<>(&bankTreeList::requestPatch),
                             Qt::UniqueConnection);
        }
    }
    else if (item && !sysxIO->isConnected())
    {
        qWarning("bankTreeList click ignored: SY-1000 not connected");
        emit setStatusMessage(tr("Not connected — connect SY-1000 via MIDI first"));
    }
}

/*********************** setItemDoubleClicked() *****************************
 * Handles when a patch is double clicked in the tree list. Patch will be
 * loaded into the temp buffer and will tell to request the data afterwards.
 ****************************************************************************/
void bankTreeList::setItemDoubleClicked(QTreeWidgetItem *item, int column)
{
    SysxIO *sysxIO = &AppServices::instance().sysx();
    sysxIO->setMode("mode1");
    Q_UNUSED(item);
    Q_UNUSED(column);
}
/*********************** requestPatch() *******************************
 * Does the actual requesting of the patch data and hands the
 * reception over to updatePatch function.
 **********************************************************************/
void bankTreeList::requestPatch()
{
    SysxIO *sysxIO = &AppServices::instance().sysx();

    QObject::disconnect(sysxIO, &SysxIO::isChanged,
                        this, qOverload<>(&bankTreeList::requestPatch));

    //sysxIO->setMode("mode0");
    if(sysxIO->isConnected())
    {
        QObject::connect(sysxIO, &SysxIO::sysxReply,	// Connect the result of the request
                         this, &bankTreeList::updatePatch);					// to updatePatch function.

        emit setStatusSymbol(3);
        emit setStatusMessage(tr("Receiving Patch"));
        sysxIO->setMode("mode1");
        sysxIO->setMode("getPNum");
        sysxIO->requestPatch(0, 0);
    };
}

void bankTreeList::requestPatch(int bank, int patch)
{
    SysxIO *sysxIO = &AppServices::instance().sysx();
    //sysxIO->setMode("mode0");
    if(sysxIO->isConnected())
    {
        QObject::connect(sysxIO, &SysxIO::sysxReply,	// Connect the result of the request
                         this, &bankTreeList::updatePatch);					// to updatePatch function.

        emit setStatusSymbol(3);
        emit setStatusMessage(tr("Receiving Patch"));

        sysxIO->requestPatch(bank, patch);
    };
}

/*********************** updatePatch() *******************************
 * Updates the source of the currently handled patch and set the
 * attributes accordingly.
 *********************************************************************/
void bankTreeList::updatePatch(QString replyMsg)
{
    SysxIO *sysxIO = &AppServices::instance().sysx();
    sysxIO->deBug("patch replysize= " + QString::number(replyMsg.size(), 10) + " guitar patch size=" + QString::number(patchReplySize, 10) + " bass patch size=" + QString::number(patchReplySizeBass, 10));
    Preferences *preferences = &AppServices::instance().preferences(); // Load the preferences.

    QString unitID = replyMsg.mid(4,2);
    sysxIO->setDeviceReady(true); // Free the device after finishing interaction.

    QObject::disconnect(sysxIO, &SysxIO::sysxReply,
                        this, &bankTreeList::updatePatch);

    QString rebuild;
    /* TRANSLATE SYSX MESSAGE FORMAT to 128 byte data blocks */
    if (replyMsg.size()/2 == patchReplySize || replyMsg.size()/2 == patchReplySizeBass)
    {

        if(replyMsg.size()/2==patchReplySize)
        {
            if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
            {
                QMessageBox *msgBox = new QMessageBox();
                msgBox->setWindowTitle(QObject::tr("Warning - Patch data does not match the current mode")+(QString::number(replyMsg.size()/2, 10)));
                msgBox->setIcon(QMessageBox::Warning);
                msgBox->setTextFormat(Qt::RichText);
                QString msgText;
                msgText.append("<font size='+1'><b>");
                msgText.append(QObject::tr("Check the SY-1000 is set to Guitar Mode"));
                msgText.append("<b></font><br>");
                msgText.append(QObject::tr(" and restart."));
                msgBox->setText(msgText);
                msgBox->show();
                QTimer::singleShot(5000, msgBox, SLOT(deleteLater()));
            }
            else
            {
                rebuild.clear();
                QString zeros("000000000000000000000000000000000000000000");
                rebuild.append(replyMsg.mid(0, 140*2));
                rebuild.append(replyMsg.mid(167*2, 12*2)+"0100"); // footer & header
                rebuild.append(replyMsg.mid(140*2, 141*2));   //10 02 ~ 10 04
                rebuild.append(zeros);
                rebuild.append(replyMsg.mid(295*2, 100*2));   //10 04 ~ 10 05
                rebuild.append(zeros);
                rebuild.append(replyMsg.mid(409*2, 100*2));   //10 05 ~ 10 06
                rebuild.append(zeros);
                rebuild.append(replyMsg.mid(523*2, 100*2));   //10 06 ~ 10 07
                rebuild.append(zeros);
                rebuild.append(replyMsg.mid(637*2, 100*2));   //10 07 ~ 10 08
                rebuild.append(zeros);
                rebuild.append(replyMsg.mid(751*2, 100*2));   //10 08 ~ 10 09
                rebuild.append(zeros);
                rebuild.append(replyMsg.mid(865*2, 100*2));   //10 09 ~ 10 0A
                rebuild.append(zeros);
                rebuild.append(replyMsg.mid(979*2, 100*2));   //10 0A ~ 10 0B
                rebuild.append(zeros);
                rebuild.append(replyMsg.mid(1093*2, 355*2));   //10 0B ~ 10 16

                rebuild.append(replyMsg.mid(167*2, 12*2)+"1700"); // footer & header
                rebuild.append(replyMsg.mid(1448*2, 692*2)); // 10 17 ~ 10 21  //648
                rebuild.append(replyMsg.mid(167*2, 12*2)+"2200"); // footer & header
                rebuild.append(replyMsg.mid(2140*2, 692*2)); // 10 22 ~ 10 2C
                rebuild.append(replyMsg.mid(167*2, 12*2)+"2D00"); // footer & header
                rebuild.append(replyMsg.mid(2832*2, 3458*2));  // to end..

                int nextIndex=0;
                QString addr1="00";
                for(int x=0; x<172; x++)
                {
                    rebuild.replace(rebuild.indexOf("F041"+unitID+"0000006912", nextIndex)+16, 4, "10"+addr1);
                    nextIndex=rebuild.indexOf("F041"+unitID+"0000006912", nextIndex+1);
                    if(x>119){addr1="01";};
                };
            };
        };

        if(replyMsg.size()/2==patchReplySizeBass)
        {
            if(preferences->getPreferences("Window", "BassMode", "bool")=="false")
            {
                QMessageBox *msgBox = new QMessageBox();
                msgBox->setWindowTitle(QObject::tr("Warning - Patch data does not match the current mode")+(QString::number(replyMsg.size()/2, 10)));
                msgBox->setIcon(QMessageBox::Warning);
                msgBox->setTextFormat(Qt::RichText);
                QString msgText;
                msgText.append("<font size='+1'><b>");
                msgText.append(QObject::tr("Check the SY-1000 is set to Bass Mode"));
                msgText.append("<b></font><br>");
                msgText.append(QObject::tr(" and restart."));
                msgBox->setText(msgText);
                msgBox->show();
                QTimer::singleShot(5000, msgBox, SLOT(deleteLater()));
            }
            else
            {
                rebuild.clear();
                QString zeros("000000000000000000000000000000000000000000");
                rebuild.append(replyMsg.mid(0, 140*2));
                rebuild.append(replyMsg.mid(167*2, 12*2)+"0100"); // footer & header
                rebuild.append(replyMsg.mid(140*2, 141*2));   //10 02 ~ 10 04
                rebuild.append(zeros);
                rebuild.append(replyMsg.mid(295*2, 100*2));   //10 04 ~ 10 05
                rebuild.append(zeros);
                rebuild.append(replyMsg.mid(409*2, 100*2));   //10 05 ~ 10 06
                rebuild.append(zeros);
                rebuild.append(replyMsg.mid(523*2, 100*2));   //10 06 ~ 10 07
                rebuild.append(zeros);
                rebuild.append(replyMsg.mid(637*2, 100*2));   //10 07 ~ 10 08
                rebuild.append(zeros);
                rebuild.append(replyMsg.mid(751*2, 100*2));   //10 08 ~ 10 09
                rebuild.append(zeros);
                rebuild.append(replyMsg.mid(865*2, 100*2));   //10 09 ~ 10 0A
                rebuild.append(zeros);
                rebuild.append(replyMsg.mid(979*2, 100*2));   //10 0A ~ 10 0B
                rebuild.append(zeros);
                rebuild.append(replyMsg.mid(1093*2, 355*2));   //10 0B ~ 10 16

                rebuild.append(replyMsg.mid(167*2, 12*2)+"1700"); // footer & header
                rebuild.append(replyMsg.mid(1448*2, 648*2)); // 10 17 ~ 10 20
                rebuild.append(replyMsg.mid(167*2, 12*2)+"2100"); // footer & header
                rebuild.append(replyMsg.mid(2096*2, 648*2)); // 10 22 ~ 10 2A
                rebuild.append(replyMsg.mid(167*2, 12*2)+"2B00"); // footer & header
                rebuild.append(replyMsg.mid(2744*2, 3414*2));  // to end..

                int nextIndex=0;
                QString addr1="02";
                for(int x=0; x<169; x++)
                {
                    rebuild.replace(rebuild.indexOf("F041"+unitID+"0000006912", nextIndex)+16, 4, "10"+addr1);
                    nextIndex=rebuild.indexOf("F041"+unitID+"0000006912", nextIndex+1);
                    if(x>119){addr1="03";};
                };
            };
        };

        sysxIO->setFileSource("Structure", rebuild);		// Set the source to the data received.
        sysxIO->setFileName(tr("Patch from ") + deviceType);	// Set the file name to SY-1000 patch for the display.
        sysxIO->setDevice(true);				// Patch received from the device so this is set to true.
        sysxIO->setSyncStatus(true);			// We can't be more in sync than right now! :)

        sysxIO->setLoadedBank(sysxIO->getBank());
        sysxIO->setLoadedPatch(sysxIO->getPatch());
        sysxIO->emitStatusdBugMessage("received patch "/*size = "+QString::number( rebuild.size()/2,10)+" bytes."*/);
        emit updateSignal();

        if(this->systemRequested==false) {QTimer::singleShot(200, this, &bankTreeList::systemRequest); };

    }
    else if(!replyMsg.isEmpty())
    {
        sysxIO->deBug("bankTreeList patch data reply wrong size !!");
        /* NO-REPLY WARNING */
        QMessageBox *msgBox = new QMessageBox();
        msgBox->setWindowTitle(QObject::tr("Warning - Patch data received is incorrect!")+(QString::number(replyMsg.size()/2, 10)));
        msgBox->setIcon(QMessageBox::Warning);
        msgBox->setTextFormat(Qt::RichText);
        QString msgText;
        msgText.append("<font size='+1'><b>");
        msgText.append(QObject::tr("Patch data transfer wrong size or data error"));
        msgText.append("<b></font><br>");
        msgText.append(QObject::tr("Please make sure the ") + deviceType + QObject::tr(" is connected correctly and re-try."));
        msgBox->setText(msgText);
        msgBox->show();
        QTimer::singleShot(3000, msgBox, SLOT(deleteLater()));
        /* END WARNING */
    }
    else if(replyMsg.isEmpty() && preferences->getPreferences("Midi", "DBug", "bool")!="true")
    {
        sysxIO->deBug("bankTreeList patch data reply missing !!");
        //emit notConnectedSignal();				// No message returned so connection must be lost.
        /* NO-REPLY WARNING */
        QMessageBox *msgBox = new QMessageBox();
        msgBox->setWindowTitle(QObject::tr("Warning - Patch data not received!"));
        msgBox->setIcon(QMessageBox::Warning);
        msgBox->setTextFormat(Qt::RichText);
        QString msgText;
        msgText.append("<font size='+1'><b>");
        msgText.append(QObject::tr("Patch data transfer failed, are the correct midi ports selected?"));
        msgText.append("<b></font><br>");
        msgText.append(QObject::tr("Please make sure the ") + deviceType + QObject::tr(" is connected correctly and re-try."));
        msgBox->setText(msgText);
        msgBox->show();
        QTimer::singleShot(3000, msgBox, SLOT(deleteLater()));
        /* END WARNING */
    };
    emit setStatusProgress(0);
    //sysxIO->setMode("mode1");
}

/********************************** connectedSignal() ****************************
* This slot reloads all patch names of expanded items, if any, on (re)connection to a
* device.
*********************************************************************************/
void bankTreeList::connectedSignal()
{
    this->systemRequested=false;
    SysxIO *sysxIO = &AppServices::instance().sysx();
    sysxIO->start_sequence = 0;
    if(sysxIO->deviceReady())
    {        
        sysxIO->setDeviceReady(false);

        this->listIndex = 0;
        this->itemIndex = 0;

        QObject::disconnect(sysxIO, &SysxIO::patchName, this, &bankTreeList::updatePatchNames);
        QObject::connect(sysxIO, &SysxIO::patchName, this, &bankTreeList::updatePatchNames);

        this->currentPatchTreeItems.clear();
        this->currentPatchTreeItems = this->openPatchTreeItems;
        emit setStatusSymbol(3);
        emit setStatusMessage(tr("Reading names"));
        sysxIO->requestBulkPatchNames(); // The bulk patch names request.
    };
}

void bankTreeList::autoLoadStartupUserPatch()
{
    if(!this->autoLoadStartupUserPatchPending) { return; }

    SysxIO *sysxIO = &AppServices::instance().sysx();
    if(!sysxIO->isConnected()) { return; }

    qWarning("bankTreeList startup auto-load USER patch U01:1");
    sysxIO->deBug("bankTreeList startup auto-load USER patch U01:1");

    // Force USER list visible first so the selection and click behavior stay consistent.
    setBankListMode(1);
    if(this->treeList->topLevelItemCount() < 1) { return; }

    QTreeWidgetItem *firstItem = this->treeList->topLevelItem(0);
    if(!firstItem) { return; }

    this->autoLoadStartupUserPatchPending = false;
    setItemClicked(firstItem, 0);
}

/***************************** updatePatchNames() ********************************
* This updates the patch names in the treeList of all expanded items. this is
* done when a bank is expanded or when we (re)connect to a device.
*********************************************************************************/
void bankTreeList::updatePatchNames(QString name)
{
    //****** comes from sysxIO::bulkPatchNamesReply
    listIndex=0; b=1; p=1;
    for(int nameIndex = 0; listIndex < 200; listIndex++) //  If not empty we can assume that we did receive a patch name.
    {
        QString bnum = QString::number(b, 10);
        if(bnum.size()<2){bnum.prepend("0");};
        QString pnum = QString::number(p, 10);
        p++;
        if(p>4){p=1; b++;};
        this->userItems.at(listIndex)->setText(0, "U" + bnum + ":" + pnum + " " + name.mid(nameIndex, 16));
        // Set the patch name of the item in the tree list.
        nameIndex = nameIndex+16;
        if(name=="no reply" || name=="bad data"){listIndex=200;};
    };
    SysxIO *sysxIO = &AppServices::instance().sysx();
    sysxIO->setDeviceReady(true);

    QObject::disconnect(sysxIO, &SysxIO::patchName, this, &bankTreeList::updatePatchNames);

    if(!name.mid(0,16).contains("data not loaded"))
    {
        this->systemRequest();
    };
    autoLoadStartupUserPatch();

    // Do NOT call populateTreeForMode here — that would override the user's
    // PRESET/USER tab selection.  Instead, if we are currently showing USER
    // mode, update the already-visible tree items' text in-place so there is
    // no flicker and no tab-mode reset.
    if(!this->presetModeActive)
    {
        for(int i = 0; i < this->treeList->topLevelItemCount() && i < this->userItems.size(); i++)
        {
            this->treeList->topLevelItem(i)->setText(0, this->userItems.at(i)->text(0));
        }
    }
    // If presetModeActive is true the userItems are already updated in memory;
    // they will be shown correctly when the user switches back to USER mode.
    updateTree();
}

/***********************updateNameSignal()**************************************
 *  Update the current selected patch name with name used in patch Write        *
 * *****************************************************************************/
void bankTreeList::updateNameSignal(QString name, int bank, int patch)
{
    QString bnum = QString::number(bank, 10);
    if(bnum.size()<2){bnum.prepend("0");};
    QString pnum = QString::number(patch, 10);
    int index = ((bank-1)*4)+(patch-1);
    if(index >= 0 && index < this->userItems.size())
    {
        this->userItems.at(index)->setText(0, "U"+bnum + ":" + pnum + " " + name);
        if(!this->presetModeActive && index < this->treeList->topLevelItemCount() && this->treeList->topLevelItem(index))
        {
            this->treeList->topLevelItem(index)->setText(0, this->userItems.at(index)->text(0));
        }
    }

    SysxIO *sysxIO = &AppServices::instance().sysx();
    sysxIO->requestPatchChange(bank, patch);
}

void bankTreeList::systemRequest()
{
    SysxIO *sysxIO = &AppServices::instance().sysx();
    sysxIO->systemDataRequest();
    this->systemRequested=true;
}

void bankTreeList::updateTree()
{
    for(int x=0; x<this->treeList->topLevelItemCount(); x++)
    {
        this->treeList->topLevelItem(x)->setSelected(false);
    };
    SysxIO *sysxIO = &AppServices::instance().sysx();
    const int bank = sysxIO->getBank();
    const int patch = sysxIO->getPatch();
    if(bank < 1 || patch < 1 || patch > 4) { return; }

    const bool currentIsPreset = (bank > 50);
    if(currentIsPreset != this->presetModeActive) { return; }

    int index = 0;
    if(currentIsPreset)
    {
        index = ((bank - 51) * 4) + (patch - 1);
    }
    else
    {
        index = ((bank - 1) * 4) + (patch - 1);
    }

    if(index >= 0 && index < this->treeList->topLevelItemCount())
    {
        this->treeList->topLevelItem(index)->setSelected(true);
        this->treeList->scrollToItem(this->treeList->topLevelItem(index));
    }
}

void bankTreeList::setBankListMode(int index)
{
    if(index < 0 || index > 1) { return; }
    const bool showPreset = (index == 0);
    qWarning("setBankListMode(%d) -> %s  presetItems=%d userItems=%d",
             index, showPreset ? "PRESET" : "USER",
             (int)presetItems.size(), (int)userItems.size());

    // Update button checked states
    this->btnPreset->setChecked(showPreset);
    this->btnUser->setChecked(!showPreset);

    populateTreeForMode(showPreset);
    // Update the tree header so the mode change is visually obvious.
    QStringList headers;
    headers << (showPreset ? tr("Preset Patches") : tr("Select tree item to load patch"));
    this->treeList->setHeaderLabels(headers);
    SysxIO *sysxIO = &AppServices::instance().sysx();
    sysxIO->deBug("bankTreeList mode switched to " + QString(showPreset ? "PRESET" : "USER"));
    const int topCount = this->treeList->topLevelItemCount();
    const QString firstItem = (topCount > 0 && this->treeList->topLevelItem(0))
            ? this->treeList->topLevelItem(0)->text(0)
            : QString("<none>");
    qWarning("setBankListMode(%d) done topLevelItems=%d firstItem='%s'",
             index, topCount, firstItem.toUtf8().constData());
    updateTree();
}

void bankTreeList::populateTreeForMode(bool presetMode)
{
    this->presetModeActive = presetMode;
    this->treeList->clear();

    const QList<QTreeWidgetItem*> &source = presetMode ? this->presetItems : this->userItems;
    for(int i=0; i<source.size(); ++i)
    {
        this->treeList->addTopLevelItem(source.at(i)->clone());
    }
}
