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

#include "editWindow.h"
#include "MidiTable.h"
#include "SysxIO.h"
#include "globalVariables.h"
#include "bulkEditDialog.h"
#include "Preferences.h"
#include "qmlHost.h"
#include "parameterBridge.h"
#include <QQuickItem>

editWindow::editWindow(QWidget *parent)
    : QDialog(parent)
{
    Preferences *preferences = Preferences::Instance();
    bool ok;
    const double ratio = preferences->getPreferences("Window", "Scale", "ratio").toDouble(&ok);
    const bool singleWindow = (preferences->getPreferences("Window", "Single", "bool")=="true");
    const int shellLeftInset = qRound(22 * ratio);

    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    if(singleWindow)
    {
        QString setting = preferences->getPreferences("Scheme", "Colour", "select");
        bool ok;
        int choice = setting.toInt(&ok, 16);
        QString mesh;
        // BUG FIX: All Qt resource paths require ":/prefix/..." (colon-slash).
        // Old paths were missing ":" entirely or had ":path" (no slash).
        if(choice == 4) { mesh = ":/images/editwindow_white.png"; }
        else if(choice == 3) { mesh = ":/images/editwindow_green.png"; }
        else if(choice == 2) { mesh = ":/images/editwindow_aqua.png"; }
        else if(choice == 1) { mesh = ":/images/editwindow_black.png"; }
        else { mesh = ":/images/editwindow_blue.png"; };
        this->image = QPixmap(mesh);
        const int compactHeight = int(image.height()*ratio*0.9);
        this->setFixedSize(image.width()*ratio, compactHeight);
        this->setWindowFlags(Qt::WindowStaysOnTopHint);
        this->setAttribute(Qt::WA_TranslucentBackground, true);
        this->setAutoFillBackground(false);
    }
    else
    {
        QString setting = preferences->getPreferences("Scheme", "Colour", "select");
        bool ok;
        int choice = setting.toInt(&ok, 16);
        QString mesh;
        // BUG FIX: All Qt resource paths require ":/prefix/..." (colon-slash).
        if(choice == 4) { mesh = ":/images/meshWindow_white.png"; }
        else if(choice == 3) { mesh = ":/images/meshWindow_green.png"; }
        else if(choice == 2) { mesh = ":/images/meshWindow_aqua.png"; }
        else if(choice == 1) { mesh = ":/images/meshWindow_black.png"; }
        else { mesh = ":/images/meshWindow_blue.png"; };
        this->image = QPixmap(mesh);
    };

    this->headerBar = new QWidget;
    this->headerBar->setObjectName("editHeaderBar");

    this->title = new QLabel;
    this->title->setObjectName("title");
    QFont titleFont = this->title->font();
    titleFont.setPixelSize(qRound(18 * ratio));
    titleFont.setBold(true);
    this->title->setFont(titleFont);
    this->title->setStyleSheet("color: white;");

    this->headerPowerButton = new customSwitch(false, this, "void", "void", "void", "void", ":/images/ampswitch.png");
    this->headerPowerButton->hide();

    this->comboBoxLabel = new QLabel(tr("Select"));
    this->comboBoxLabel->setObjectName("selectlabel");
    this->comboBoxLabel->setVisible(false);

    this->pageComboBox = new QTabBar;
    this->pageComboBox->setObjectName("pagecombobox");;
    this->pageComboBox->setShape(QTabBar::RoundedNorth);
    this->pageComboBox->setVisible(false);

    this->bulkEdit_Button = new customControlLabel;
    this->bulkEdit_Button->setButton(true);
    this->bulkEdit_Button->setImage(":/images/pushbutton_dark.png");
    this->bulkEdit_Button->setText(tr("Bulk Write"));
    this->bulkEdit_Button->setAlignment(Qt::AlignCenter);
    this->bulkEdit_Button->setWhatsThis(tr("Multiple Patch Edit Button<br>will write only the currently displayed effect edit-page to a selected range of User Patches."));

    this->temp1_Button = new customControlLabel;
    this->temp1_Button->setButton(true);
    this->temp1_Button->setImage(":/images/pushbutton_dark.png");
    this->temp1_Button->setText(tr("ClipBd 1 Paste"));
    this->temp1_Button->setAlignment(Qt::AlignCenter);
    this->temp1_Button->setWhatsThis(tr("Effect Partial Paste Button<br>will paste only the currently displayed effect part from the selected Patch clipboard."
                                        "<br>can be used to quickly compare settings between patches."));

    this->temp2_Button = new customControlLabel;
    this->temp2_Button->setButton(true);
    this->temp2_Button->setImage(":/images/pushbutton_dark.png");
    this->temp2_Button->setText(tr("ClipBd 2 Paste"));
    this->temp2_Button->setAlignment(Qt::AlignCenter);
    this->temp2_Button->setWhatsThis(tr("Effect Partial Paste Button<br>will paste only the currently displayed effect part from the selected Patch clipboard."
                                        "<br>can be used to quicky compare settings between patches."));


    this->temp3_Button = new customControlLabel;
    this->temp3_Button->setButton(true);
    this->temp3_Button->setImage(":/images/pushbutton_dark.png");
    this->temp3_Button->setText(tr("ClipBd 3 Paste"));
    this->temp3_Button->setAlignment(Qt::AlignCenter);
    this->temp3_Button->setWhatsThis(tr("Effect Partial Paste Button<br>will paste only the currently displayed effect part from the selected Patch clipboard."
                                        "<br>can be used to quicky compare settings between patches."));


    this->temp4_Button = new customControlLabel;
    this->temp4_Button->setButton(true);
    this->temp4_Button->setImage(":/images/pushbutton_dark.png");
    this->temp4_Button->setText(tr("ClipBd 4 Paste"));
    this->temp4_Button->setAlignment(Qt::AlignCenter);
    this->temp4_Button->setWhatsThis(tr("Effect Partial Paste Button<br>will paste only the currently displayed effect part from the selected Patch clipboard."
                                        "<br>can be used to quicky compare settings between patches."));


    this->temp5_Button = new customControlLabel;
    this->temp5_Button->setButton(true);
    this->temp5_Button->setImage(":/images/pushbutton_dark.png");
    this->temp5_Button->setText(tr("ClipBd 5 Paste"));
    this->temp5_Button->setAlignment(Qt::AlignCenter);
    this->temp5_Button->setWhatsThis(tr("Effect Partial Paste Button<br>will paste only the currently displayed effect part from the selected Patch clipboard."
                                        "<br>can be used to quicky compare settings between patches."));

    this->assign1_Button = new customControlLabel;
    this->assign1_Button->setButton(true);
    this->assign1_Button->setImage(":/images/pushbutton_dark.png");
    this->assign1_Button->setText(tr("Assign 1 Paste"));
    this->assign1_Button->setAlignment(Qt::AlignCenter);
    this->assign1_Button->setWhatsThis(tr("Assign Paste Button<br>will paste an Assign setting from another Assign from the same patch."
                                          "<br>can be used to copy or shift an Assign setting."));

    this->assign2_Button = new customControlLabel;
    this->assign2_Button->setButton(true);
    this->assign2_Button->setImage(":/images/pushbutton_dark.png");
    this->assign2_Button->setText(tr("Assign 2 Paste"));
    this->assign2_Button->setAlignment(Qt::AlignCenter);
    this->assign2_Button->setWhatsThis(tr("Assign Paste Button<br>will paste an Assign setting from another Assign from the same patch."
                                          "<br>can be used to copy or shift an Assign setting."));

    this->assign3_Button = new customControlLabel;
    this->assign3_Button->setButton(true);
    this->assign3_Button->setImage(":/images/pushbutton_dark.png");
    this->assign3_Button->setText(tr("Assign 3 Paste"));
    this->assign3_Button->setAlignment(Qt::AlignCenter);
    this->assign3_Button->setWhatsThis(tr("Assign Paste Button<br>will paste an Assign setting from another Assign from the same patch."
                                          "<br>can be used to copy or shift an Assign setting."));

    this->assign4_Button = new customControlLabel;
    this->assign4_Button->setButton(true);
    this->assign4_Button->setImage(":/images/pushbutton_dark.png");
    this->assign4_Button->setText(tr("Assign 4 Paste"));
    this->assign4_Button->setAlignment(Qt::AlignCenter);
    this->assign4_Button->setWhatsThis(tr("Assign Paste Button<br>will paste an Assign setting from another Assign from the same patch."
                                          "<br>can be used to copy or shift an Assign setting."));

    this->assign5_Button = new customControlLabel;
    this->assign5_Button->setButton(true);
    this->assign5_Button->setImage(":/images/pushbutton_dark.png");
    this->assign5_Button->setText(tr("Assign 5 Paste"));
    this->assign5_Button->setAlignment(Qt::AlignCenter);
    this->assign5_Button->setWhatsThis(tr("Assign Paste Button<br>will paste an Assign setting from another Assign from the same patch."
                                          "<br>can be used to copy or shift an Assign setting."));

    this->assign6_Button = new customControlLabel;
    this->assign6_Button->setButton(true);
    this->assign6_Button->setImage(":/images/pushbutton_dark.png");
    this->assign6_Button->setText(tr("Assign 6 Paste"));
    this->assign6_Button->setAlignment(Qt::AlignCenter);
    this->assign6_Button->setWhatsThis(tr("Assign Paste Button<br>will paste an Assign setting from another Assign from the same patch."
                                          "<br>can be used to copy or shift an Assign setting."));

    this->assign7_Button = new customControlLabel;
    this->assign7_Button->setButton(true);
    this->assign7_Button->setImage(":/images/pushbutton_dark.png");
    this->assign7_Button->setText(tr("Assign 7 Paste"));
    this->assign7_Button->setAlignment(Qt::AlignCenter);
    this->assign7_Button->setWhatsThis(tr("Assign Paste Button<br>will paste an Assign setting from another Assign from the same patch."
                                          "<br>can be used to copy or shift an Assign setting."));

    this->assign8_Button = new customControlLabel;
    this->assign8_Button->setButton(true);
    this->assign8_Button->setImage(":/images/pushbutton_dark.png");
    this->assign8_Button->setText(tr("Assign 8 Paste"));
    this->assign8_Button->setAlignment(Qt::AlignCenter);
    this->assign8_Button->setWhatsThis(tr("Assign Paste Button<br>will paste an Assign setting from another Assign from the same patch."
                                          "<br>can be used to copy or shift an Assign setting."));


    this->closeButton = new customControlLabel;
    this->closeButton->setButton(true);
    this->closeButton->setImage(":/images/closebutton.png");
    this->closeButton->setWhatsThis(tr("Will close the current edit page window."));

    QHBoxLayout *pastebuttonLayout = new QHBoxLayout;
    pastebuttonLayout->addWidget(this->bulkEdit_Button);
    pastebuttonLayout->addWidget(this->temp1_Button);
    pastebuttonLayout->addWidget(this->temp2_Button);
    pastebuttonLayout->addWidget(this->temp3_Button);
    pastebuttonLayout->addWidget(this->temp4_Button);
    pastebuttonLayout->addWidget(this->temp5_Button);

    QHBoxLayout *top4buttonLayout = new QHBoxLayout;
    top4buttonLayout->addSpacing(170*ratio);
    top4buttonLayout->addWidget(this->assign1_Button);
    top4buttonLayout->addWidget(this->assign2_Button);
    top4buttonLayout->addWidget(this->assign3_Button);
    top4buttonLayout->addWidget(this->assign4_Button);

    QHBoxLayout *bottom4buttonLayout = new QHBoxLayout;
    bottom4buttonLayout->addSpacing(170*ratio);
    bottom4buttonLayout->addWidget(this->assign5_Button);
    bottom4buttonLayout->addWidget(this->assign6_Button);
    bottom4buttonLayout->addWidget(this->assign7_Button);
    bottom4buttonLayout->addWidget(this->assign8_Button);

    QVBoxLayout *buttonLayout = new QVBoxLayout;
    buttonLayout->addLayout(pastebuttonLayout);
    buttonLayout->addSpacing(8*ratio);
    buttonLayout->addLayout(top4buttonLayout);
    buttonLayout->addLayout(bottom4buttonLayout);

    QHBoxLayout *headerLayout = new QHBoxLayout;
    headerLayout->setContentsMargins(shellLeftInset, qRound(4*ratio), qRound(6*ratio), qRound(4*ratio));
    headerLayout->setSpacing(qRound(6*ratio));
    headerLayout->addWidget(this->headerPowerButton, 0, Qt::AlignLeft | Qt::AlignVCenter);
    headerLayout->addWidget(this->title);
    headerLayout->addStretch();
    headerLayout->addWidget(this->comboBoxLabel);
    headerLayout->addWidget(this->pageComboBox);
    headerLayout->addStretch();
    headerLayout->addLayout(buttonLayout);
    headerLayout->addStretch();
    headerLayout->addWidget(this->closeButton);
    this->headerBar->setLayout(headerLayout);

    this->pagesWidget = new QStackedWidget;

    QHBoxLayout *pagesLayout = new QHBoxLayout;
    pagesLayout->setContentsMargins(shellLeftInset, 0, 0, 0);
    pagesLayout->setSpacing(0);
    pagesLayout->addSpacing(0);
    pagesLayout->addWidget(this->pagesWidget, 0, Qt::AlignTop | Qt::AlignLeft);
    if(!singleWindow)
    {
        pagesLayout->addStretch();
    }

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(this->headerBar);
    mainLayout->addSpacing(0);
    mainLayout->addLayout(pagesLayout);
    mainLayout->addStretch();
    setLayout(mainLayout);

    this->tempPage = new editPage;

    QObject::connect(this->pageComboBox, SIGNAL(tabBarClicked(int)), this->pagesWidget, SLOT(setCurrentIndex(int)));


    /* QObject::connect(this->bulkEdit_Button, SIGNAL(mouseReleased()), this, SLOT(bulkEdit()));
    QObject::connect(this->temp1_Button, SIGNAL(mouseReleased()), this, SLOT(temp1()));
    QObject::connect(this->temp2_Button, SIGNAL(mouseReleased()), this, SLOT(temp2()));
    QObject::connect(this->temp3_Button, SIGNAL(mouseReleased()), this, SLOT(temp3()));
    QObject::connect(this->temp4_Button, SIGNAL(mouseReleased()), this, SLOT(temp4()));
    QObject::connect(this->temp5_Button, SIGNAL(mouseReleased()), this, SLOT(temp5()));

    QObject::connect(this->assign1_Button, SIGNAL(mouseReleased()), this, SLOT(assign1_paste()));
    QObject::connect(this->assign2_Button, SIGNAL(mouseReleased()), this, SLOT(assign2_paste()));
    QObject::connect(this->assign3_Button, SIGNAL(mouseReleased()), this, SLOT(assign3_paste()));
    QObject::connect(this->assign4_Button, SIGNAL(mouseReleased()), this, SLOT(assign4_paste()));
    QObject::connect(this->assign5_Button, SIGNAL(mouseReleased()), this, SLOT(assign5_paste()));
    QObject::connect(this->assign6_Button, SIGNAL(mouseReleased()), this, SLOT(assign6_paste()));
    QObject::connect(this->assign7_Button, SIGNAL(mouseReleased()), this, SLOT(assign7_paste()));
    QObject::connect(this->assign8_Button, SIGNAL(mouseReleased()), this, SLOT(assign8_paste()));*/

    QObject::connect(this->closeButton, SIGNAL(mousePressed()), this, SLOT(hide()));
    QObject::connect(this->headerPowerButton, SIGNAL(valueChanged(bool, QString, QString, QString, QString)),
                     this, SLOT(headerPowerChanged(bool, QString, QString, QString, QString)));

    QObject::connect(this, SIGNAL( closeWindow() ), this, SLOT(hide()));

    QObject::connect(this, SIGNAL( dialogUpdateSignal() ), this, SLOT( pageUpdateSignal() ));

    QObject::connect(this, SIGNAL( updateSignal() ), this, SLOT( pageUpdateSignal() ));

    QObject::connect(this->pageComboBox, SIGNAL(tabBarClicked(int)), this, SLOT(valueChanged(int)));

    this->pageIndex = 0;

    if(preferences->getPreferences("Window", "Single", "bool")=="true"){this->closeButton->hide();};

    this->bulkEdit_Button->hide();
    this->temp1_Button->hide();
    this->temp2_Button->hide();
    this->temp3_Button->hide();
    this->temp4_Button->hide();
    this->temp5_Button->hide();
    this->assign1_Button->hide();
    this->assign2_Button->hide();
    this->assign3_Button->hide();
    this->assign4_Button->hide();
    this->assign5_Button->hide();
    this->assign6_Button->hide();
    this->assign7_Button->hide();
    this->assign8_Button->hide();
    this->comboBoxLabel->hide();
    refreshHeaderBar();
}

void editWindow::paintEvent(QPaintEvent *)
{
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "Single", "bool")=="true")
    {
        return;
    }
    bool ok;
    const double ratio = preferences->getPreferences("Window", "Scale", "ratio").toDouble(&ok);
    int style = preferences->getPreferences("Scheme", "Colour", "select").toInt(&ok, 16);

    QRectF target(0.0, 0.0, image.width()*ratio, image.height()*ratio);
    QRectF source(0.0, 0.0, image.width(), image.height());
    QPainter painter(this);
    painter.drawPixmap(target, image, source);

    painter.setPen(QPen(this->setBrush, 36*ratio, Qt::SolidLine, Qt::FlatCap));
    painter.setOpacity(0.4);
    if(style>0){ painter.drawLine(20*ratio, 18*ratio, (image.width()-20)*ratio, 18*ratio); }
    else { painter.drawLine(0, 18*ratio, image.width()*ratio, 18*ratio); };
    painter.end();
}

editWindow::~editWindow()
{

}

void editWindow::BrushColor(QBrush setBrush){

    this->setBrush = setBrush;
    refreshHeaderBar();
}

void editWindow::setLSB(QString hex1, QString hex2)
{
    this->hex1 = hex1;
    this->hex2 = hex2;
}

void editWindow::setPowerAddress(QString hex0, QString hex1, QString hex2, QString hex3)
{
    this->explicitPowerHex0 = hex0;
    this->explicitPowerHex1 = hex1;
    this->explicitPowerHex2 = hex2;
    this->explicitPowerHex3 = hex3;
}

void editWindow::setPowerState(bool enabled)
{
    this->explicitPowerState = enabled;
    this->explicitPowerStateValid = true;

    // Forward power state to QML
    if (this->qmlHost && this->qmlHost->rootObject()) {
        this->qmlHost->rootObject()->setProperty("powerValue", enabled ? 1 : 0);
    }
}

void editWindow::setWindow(QString title)
{
    this->title->setText(title);
    this->pagesWidget->setCurrentIndex(0);
    configureHeaderPower(qobject_cast<editPage*>(this->pagesWidget->currentWidget()));
    refreshHeaderBar();

    // Forward title and accent color to QML host if present
    if (this->qmlHost) {
        this->qmlHost->setTitle(title);
        QColor color = this->setBrush.color();
        if (color.isValid()) {
            this->qmlHost->setAccentColor(color);
        }
    }
}

QString editWindow::getTitle()
{
    return this->title->text();
}

void editWindow::addPage(QString area, QString hex1, QString hex2, QString hex3, QString hex4 )
{
    this->hex1 = hex1;
    this->hex2 = hex2;
    this->hex3 = hex3;
    this->area = area;
    this->tempPage->setGridLayout();
    this->editPages.append(this->tempPage);
    this->pagesWidget->addWidget(editPages.last());
    this->pages = this->pagesWidget->count();

    QObject::connect(this, SIGNAL( dialogUpdateSignal() ),
                     editPages.last(), SIGNAL( dialogUpdateSignal() ));

    QObject::connect(editPages.last(), SIGNAL( updateSignal() ),
                     this, SIGNAL( updateSignal() ));
    if(this->explicitPowerHex0 != "void" &&
       this->explicitPowerHex1 != "void" &&
       this->explicitPowerHex2 != "void" &&
       this->explicitPowerHex3 != "void")
    {
        editPages.last()->setExternalPowerAddress(this->explicitPowerHex0,
                                                  this->explicitPowerHex1,
                                                  this->explicitPowerHex2,
                                                  this->explicitPowerHex3);
    }

    // if (this->area != "Structure" || this->temp_hex1.isEmpty() || this->temp_hex1.contains("void"))
    // {
    this->bulkEdit_Button->hide();
    this->temp1_Button->hide();
    this->temp2_Button->hide();
    this->temp3_Button->hide();
    this->temp4_Button->hide();
    this->temp5_Button->hide();
    // };

    //If not within assigns area then hide assigns paste buttons.
    // if ((this->temp_hex1 != "01" && this->temp_hex1 != "02") || this->position < 327 || this->position > 620)
    // {
    this->assign1_Button->hide();
    this->assign2_Button->hide();
    this->assign3_Button->hide();
    this->assign4_Button->hide();
    this->assign5_Button->hide();
    this->assign6_Button->hide();
    this->assign7_Button->hide();
    this->assign8_Button->hide();
    // };
    /* if(this->position == 328) {this->assign1_Button->hide(); };
    if(this->position == 366) {this->assign2_Button->hide(); };
    if(this->position == 404) {this->assign3_Button->hide(); };
    if(this->position == 442) {this->assign4_Button->hide(); };
    if(this->position == 480) {this->assign5_Button->hide(); };
    if(this->position == 518) {this->assign6_Button->hide(); };
    if(this->position == 556) {this->assign7_Button->hide(); };
    if(this->position == 620) {this->assign8_Button->hide(); };*/

    if(hex1 != "void" && hex2 != "void" && hex3 != "void")
    {
        MidiTable *midiTable = MidiTable::Instance();
        Midi items = midiTable->getMidiMap(this->area, hex1, hex2, hex3);

        int itemsCount;
        if(hex4 == "void")
        {
            itemsCount = this->pagesWidget->count() - 1;
        }
        else
        {
            bool ok;
            itemsCount = hex4.toInt(&ok, 16);
        };

        QString item;
        QString desc = items.level.at(itemsCount).desc;
        QString customdesc = items.level.at(itemsCount).customdesc;
        if(!customdesc.isEmpty())
        {
            item = customdesc;
        }
        else
        {
            item = desc;
        };


        //this->pageComboBox->addItem(item);
        this->pageComboBox->addTab(item);
        this->tempPage = new editPage;
        //this->pageComboBox->setMaxVisibleItems(this->pages);

        if(this->pages > 1)
        {
            this->pageComboBox->setVisible(true);
            this->comboBoxLabel->setVisible(true);
        };
    };

}

void editWindow::valueChanged(int index)
{
    this->pageIndex = index;
    if(hex1 != "void" && hex2 != "void")
    {
        pageUpdateSignal();
    };
}

void editWindow::pageUpdateSignal()
{
    if(this->pages > 1 && hex1 != "void" && hex2 != "void")
    {
        int index = this->pageIndex;
        this->pageComboBox->setCurrentIndex(index);
        this->pagesWidget->setCurrentIndex(index);
        if(this->title->text().contains("ASSIGN"))
        {
            this->pageComboBox->setTabTextColor(0, QColor(255,255,255));
            this->pageComboBox->setTabTextColor(1, QColor(255,255,255));
            this->pageComboBox->setTabTextColor(2, QColor(255,255,255));
            this->pageComboBox->setTabTextColor(3, QColor(255,255,255));
            this->pageComboBox->setTabTextColor(4, QColor(255,255,255));
            this->pageComboBox->setTabTextColor(5, QColor(255,255,255));
            this->pageComboBox->setTabTextColor(6, QColor(255,255,255));
            this->pageComboBox->setTabTextColor(7, QColor(255,255,255));
            this->pageComboBox->setTabTextColor(8, QColor(255,255,255));
            this->pageComboBox->setTabTextColor(9, QColor(255,255,255));
            this->pageComboBox->setTabTextColor(10, QColor(255,255,255));
            this->pageComboBox->setTabTextColor(11, QColor(255,255,255));
            this->pageComboBox->setTabTextColor(12, QColor(255,255,255));
            this->pageComboBox->setTabTextColor(13, QColor(255,255,255));
            this->pageComboBox->setTabTextColor(14, QColor(255,255,255));
            this->pageComboBox->setTabTextColor(15, QColor(255,255,255));
            Preferences *preferences = Preferences::Instance();
            SysxIO *sysxIO = SysxIO::Instance();
            QString hexx = "00";
            if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){hexx = "02";};
            if(sysxIO->getSourceValue("10", hexx, "04", "00")>0) this->pageComboBox->setTabTextColor(0, QColor(255,108,0));
            if(sysxIO->getSourceValue("10", hexx, "04", "40")>0) this->pageComboBox->setTabTextColor(1, QColor(255,108,0));
            if(sysxIO->getSourceValue("10", hexx, "05", "00")>0) this->pageComboBox->setTabTextColor(2, QColor(255,108,0));
            if(sysxIO->getSourceValue("10", hexx, "05", "40")>0) this->pageComboBox->setTabTextColor(3, QColor(255,108,0));
            if(sysxIO->getSourceValue("10", hexx, "06", "00")>0) this->pageComboBox->setTabTextColor(4, QColor(255,108,0));
            if(sysxIO->getSourceValue("10", hexx, "06", "40")>0) this->pageComboBox->setTabTextColor(5, QColor(255,108,0));
            if(sysxIO->getSourceValue("10", hexx, "07", "00")>0) this->pageComboBox->setTabTextColor(6, QColor(255,108,0));
            if(sysxIO->getSourceValue("10", hexx, "07", "40")>0) this->pageComboBox->setTabTextColor(7, QColor(255,108,0));
            if(sysxIO->getSourceValue("10", hexx, "08", "00")>0) this->pageComboBox->setTabTextColor(8, QColor(255,108,0));
            if(sysxIO->getSourceValue("10", hexx, "08", "40")>0) this->pageComboBox->setTabTextColor(9, QColor(255,108,0));
            if(sysxIO->getSourceValue("10", hexx, "09", "00")>0) this->pageComboBox->setTabTextColor(10, QColor(255,108,0));
            if(sysxIO->getSourceValue("10", hexx, "09", "40")>0) this->pageComboBox->setTabTextColor(11, QColor(255,108,0));
            if(sysxIO->getSourceValue("10", hexx, "0A", "00")>0) this->pageComboBox->setTabTextColor(12, QColor(255,108,0));
            if(sysxIO->getSourceValue("10", hexx, "0A", "40")>0) this->pageComboBox->setTabTextColor(13, QColor(255,108,0));
            if(sysxIO->getSourceValue("10", hexx, "0B", "00")>0) this->pageComboBox->setTabTextColor(14, QColor(255,108,0));
            if(sysxIO->getSourceValue("10", hexx, "0B", "40")>0) this->pageComboBox->setTabTextColor(15, QColor(255,108,0));
        };
    };

    editPage *activePage = qobject_cast<editPage*>(this->pagesWidget->currentWidget());
    configureHeaderPower(activePage);
}

editPage* editWindow::page()
{
    return this->tempPage;
}

void editWindow::refreshHeaderBar()
{
    QColor color = this->setBrush.color();
    if(!color.isValid())
    {
        color = QColor(80, 80, 80);
    }

    const QString headerStyle = QString(
        "QWidget#editHeaderBar { background-color: rgba(%1, %2, %3, 210); }"
        "QTabBar::tab { color: rgba(255,255,255,190); background: transparent; padding: 5px 10px; font-weight: 700; }"
        "QTabBar::tab:selected { color: rgb(255,255,255); border-bottom: 3px solid rgb(54, 214, 255); }"
        "QTabBar::tab:hover { color: rgb(255,255,255); }")
        .arg(color.red()).arg(color.green()).arg(color.blue());
    this->headerBar->setStyleSheet(headerStyle);
    this->headerPowerButton->setAccentColor(color);
}

void editWindow::configureHeaderPower(editPage *page)
{
    if(this->explicitPowerHex0 != "void" &&
       this->explicitPowerHex1 != "void" &&
       this->explicitPowerHex2 != "void" &&
       this->explicitPowerHex3 != "void")
    {
        this->headerPowerHex0 = this->explicitPowerHex0;
        this->headerPowerHex1 = this->explicitPowerHex1;
        this->headerPowerHex2 = this->explicitPowerHex2;
        this->headerPowerHex3 = this->explicitPowerHex3;

        bool enabled = this->explicitPowerState;
        if(!this->explicitPowerStateValid)
        {
            SysxIO *sysxIO = SysxIO::Instance();
            enabled = (sysxIO->getSourceValue(headerPowerHex0, headerPowerHex1, headerPowerHex2, headerPowerHex3) > 0);
        }
        this->headerPowerButton->setValue(enabled);
        this->headerPowerButton->show();
        return;
    }

    editPage *powerPage = page;
    if(powerPage == nullptr || !powerPage->hasPowerControl())
    {
        for(editPage *candidate : this->editPages)
        {
            if(candidate != nullptr && candidate->hasPowerControl())
            {
                powerPage = candidate;
                break;
            }
        }
    }

    if(powerPage == nullptr || !powerPage->hasPowerControl())
    {
        this->headerPowerHex0 = "void";
        this->headerPowerHex1 = "void";
        this->headerPowerHex2 = "void";
        this->headerPowerHex3 = "void";
        this->headerPowerButton->hide();
        return;
    }

    this->headerPowerHex0 = powerPage->powerAddress0();
    this->headerPowerHex1 = powerPage->powerAddress1();
    this->headerPowerHex2 = powerPage->powerAddress2();
    this->headerPowerHex3 = powerPage->powerAddress3();
    powerPage->hideInlinePowerControl();

    SysxIO *sysxIO = SysxIO::Instance();
    const bool enabled = (sysxIO->getSourceValue(headerPowerHex0, headerPowerHex1, headerPowerHex2, headerPowerHex3) > 0);
    this->headerPowerButton->setValue(enabled);
    this->headerPowerButton->show();
}

void editWindow::headerPowerChanged(bool value, QString hex0, QString hex1, QString hex2, QString hex3)
{
    Q_UNUSED(hex0);
    Q_UNUSED(hex1);
    Q_UNUSED(hex2);
    Q_UNUSED(hex3);

    SysxIO *sysxIO = SysxIO::Instance();
    if(this->headerPowerHex0 == "void" || this->headerPowerHex1 == "void" ||
       this->headerPowerHex2 == "void" || this->headerPowerHex3 == "void")
    {
        return;
    }

    sysxIO->setFileSource(headerPowerHex0, headerPowerHex1, headerPowerHex2, headerPowerHex3, value ? "01" : "00");
    emit dialogUpdateSignal();
    emit updateSignal();
}

void editWindow::patchPos(int pos, int len, QString t_hex1, QString t_hex3)
{
    this->position = pos;
    this->length = len;
    this->temp_hex1 = t_hex1;
    this->temp_hex3 = t_hex3;
}

void editWindow::closeEvent(QCloseEvent* ce)
{
    ce->accept();
}

void editWindow::hideWindow()
{
    QApplication::beep();
    hide();
}

void editWindow::bulkEdit()
{
    SysxIO *sysxIO = SysxIO::Instance();
    if (sysxIO->isConnected())
    {	
        bulkEditDialog *editDialog = new bulkEditDialog(this->position, this->length, this->temp_hex1, this->temp_hex3);
        editDialog->exec();
        editDialog->deleteLater();
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

void editWindow::temp1()
{
    SysxIO *sysxIO = SysxIO::Instance();
    if (!sysxIO->temp1_sysxMsg.isEmpty() && !temp_hex1.isEmpty() && !temp_hex1.contains("void") )
    {
        temp = sysxIO->temp1_sysxMsg.mid(this->position, this->length);
        temp_paste();
    }
    else
    {
        QApplication::beep();
        sysxIO->emitStatusdBugMessage(tr("patch must be copied to clipboard first"));
    };
}

void editWindow::temp2()
{
    SysxIO *sysxIO = SysxIO::Instance();
    if (!sysxIO->temp2_sysxMsg.isEmpty() && !temp_hex1.isEmpty() && !temp_hex1.contains("void")  )
    {
        temp = sysxIO->temp2_sysxMsg.mid(this->position, this->length);
        temp_paste();
    }
    else
    {
        QApplication::beep();
        sysxIO->emitStatusdBugMessage(tr("patch must be copied to clipboard first"));
    };
}

void editWindow::temp3()
{
    SysxIO *sysxIO = SysxIO::Instance();
    if (!sysxIO->temp3_sysxMsg.isEmpty() && !temp_hex1.isEmpty() && !temp_hex1.contains("void")  )
    {
        temp = sysxIO->temp3_sysxMsg.mid(this->position, this->length);
        temp_paste();
    }
    else
    {
        QApplication::beep();
        sysxIO->emitStatusdBugMessage(tr("patch must be copied to clipboard first"));
    };
}

void editWindow::temp4()
{
    SysxIO *sysxIO = SysxIO::Instance();
    if (!sysxIO->temp4_sysxMsg.isEmpty() && !temp_hex1.isEmpty() && !temp_hex1.contains("void")  )
    {
        temp = sysxIO->temp4_sysxMsg.mid(this->position, this->length);
        temp_paste();
    }
    else
    {
        QApplication::beep();
        sysxIO->emitStatusdBugMessage(tr("patch must be copied to clipboard first"));
    };
}

void editWindow::temp5()
{
    SysxIO *sysxIO = SysxIO::Instance();
    if (!sysxIO->temp5_sysxMsg.isEmpty() && !temp_hex1.isEmpty() && !temp_hex1.contains("void")  )
    {
        temp = sysxIO->temp5_sysxMsg.mid(this->position, this->length);
        temp_paste();
    }
    else
    {
        QApplication::beep();
        sysxIO->emitStatusdBugMessage(tr("patch must be copied to clipboard first"));
    };
}

void editWindow::temp_paste()
{
    SysxIO *sysxIO = SysxIO::Instance();
    QList< QList<QString> > patchData = sysxIO->getFileSource().hex; // Get the loaded patch data.
    QString addr1 = tempBulkWrite;  // temp address
    QString addr2 = QString::number(0, 16).toUpper();
    QString sysxMsg;
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
    if(this->position == 2236){    //pcm synth 1 is in two sections
        sysxMsg = sysxMsg.replace(2236, 70, temp.mid(0, 70));
        sysxIO->setFileSource("Structure", this->temp_hex1, "00", this->temp_hex3, temp.mid(0, 70));
        sysxMsg = sysxMsg.replace(2428, 104, temp.mid(192, 104));
        sysxIO->setFileSource("Structure", this->temp_hex1, "00", this->temp_hex3, temp.mid(192, 104));
    } else if(this->position == 2332){    //pcm synth 2 is in two sections
        sysxMsg = sysxMsg.replace(2332, 70, temp.mid(0, 70));
        sysxIO->setFileSource("Structure", this->temp_hex1, "00", this->temp_hex3, temp.mid(0, 70));
        sysxMsg = sysxMsg.replace(2558, 104, temp.mid(226, 104));
        sysxIO->setFileSource("Structure", this->temp_hex1, "00", this->temp_hex3, temp.mid(226, 104));
    } else {
        sysxMsg = sysxMsg.replace(this->position, this->length, temp);
        sysxIO->setFileSource("Structure", this->temp_hex1, "00", this->temp_hex3, temp); };
    sysxIO->setFileSource("Structure", sysxMsg );
    emit dialogUpdateSignal();
}

void editWindow::assign1_paste()
{
    start_int = 328; length_int = 38;
    assign_paste();
}

void editWindow::assign2_paste()
{
    start_int = 366; length_int = 38;
    assign_paste();
}

void editWindow::assign3_paste()
{
    start_int = 404; length_int = 38;
    assign_paste();
}

void editWindow::assign4_paste()
{
    start_int = 442; length_int = 38;
    assign_paste();
}

void editWindow::assign5_paste()
{
    start_int = 480; length_int = 38;
    assign_paste();
}

void editWindow::assign6_paste()
{
    start_int = 518; length_int = 38;
    assign_paste();
}

void editWindow::assign7_paste()
{
    start_int = 556; length_int = 64;
    assign_paste();
}

void editWindow::assign8_paste()
{
    start_int = 620; length_int = 38;
    assign_paste();
}

void editWindow::assign_paste()
{
    SysxIO *sysxIO = SysxIO::Instance();
    QList< QList<QString> > patchData = sysxIO->getFileSource().hex; // Get the loaded patch data.
    QString addr1 = tempBulkWrite;  // temp address
    QString addr2 = QString::number(0, 16).toUpper();
    QString sysxMsg;
    for(int i=0;i<patchData.size();++i)
    {
        QList<QString> data = patchData.at(i);
        for(int x=0;x<data.size();++x)
        {
            QString hex;
            if(x == sysxAddressOffset)
            { hex = addr1; }
            else if(x == sysxAddressOffset + 1)
            { hex = addr2; }
            else
            { hex = data.at(x);	};
            if (hex.length() < 2) hex.prepend("0");
            sysxMsg.append(hex);
        };
    };
    temp = sysxMsg.mid(start_int, length_int);
    if(length_int == 64) {temp.remove(4, 26); };   // if the assign comes from assign 7
    if(this->position == 556){temp.insert(4, sysxMsg.mid(560, 26)); }; // if the assign is pasted to assign 7
    sysxMsg = sysxMsg.replace(this->position, this->length, temp);
    sysxIO->setFileSource("Structure", this->temp_hex1, "00", this->temp_hex3, temp);
    sysxIO->setFileSource("Structure", sysxMsg );
    emit dialogUpdateSignal();
}

void editWindow::setQmlPage(const QString &qmlSource, const QString &hex1)
{
    this->qmlHost = new QmlHost(qmlSource, this);
    this->qmlHost->setInstHex(hex1);
    this->qmlHost->setMinimumSize(800, 460);
    this->qmlHost->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->pagesWidget->addWidget(this->qmlHost);
    this->pagesWidget->setMinimumSize(800, 460);
    this->pagesWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Hide the C++ header — QML renders its own header with power button and tabs
    this->headerBar->setVisible(false);
    this->pageComboBox->setVisible(false);
    this->comboBoxLabel->setVisible(false);

    // Pass accent color to QML (will be set after setWindow is called)
    QColor color = this->setBrush.color();
    if (color.isValid()) {
        this->qmlHost->setAccentColor(color);
    }

    // When QML changes a parameter, propagate updateSignal so stompBox refreshes
    QObject::connect(ParameterBridge::Instance(), &ParameterBridge::parameterChanged,
                     this, [this](const QString &, const QString &, const QString &, const QString &, int) {
        emit updateSignal();
    });
}
