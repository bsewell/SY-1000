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

#include <QList>
#include <QVector>
#include <QDir>
#include <QApplication>
#include <climits>
//#include <QMovie>

#include "bankTreeList.h"
#include "floorBoardDisplay.h"
#include "floorBoard.h"
#include "Preferences.h"
#include "MidiTable.h"
#include "sysxWriter.h"
#include "SysxIO.h"
#include "globalVariables.h"
#include "menuPage_assign1.h"
#include "menuPage_setup.h"
#include "menuPage_system.h"
#include "menuPage_tuner.h"
#include "menuPage_master.h"
#include "menuPage_pdl.h"
#include "soundSource_inst1.h"
#include "soundSource_inst2.h"
#include "soundSource_inst3.h"
#include "soundSource_normal.h"
#include "stompbox_amp.h"
#include "stompbox_eq1.h"
#include "stompbox_eq2.h"
#include "stompbox_fx1.h"
#include "stompbox_fx2.h"
#include "stompbox_fx3.h"
#include "stompbox_fv1.h"
#include "stompbox_fv2.h"
#include "stompbox_dd1.h"
#include "stompbox_dd2.h"
#include "stompbox_dd3.h"
#include "stompbox_ds.h"
#include "stompbox_ce.h"
#include "stompbox_cs.h"
#include "stompbox_rv.h"
#include "stompbox_lp.h"
#include "stompbox_ns.h"
#include "stompbox_balancer1.h"
#include "stompbox_balancer2.h"
#include "stompbox_balancer3.h"
#include "stompbox_branch_AB.h"
#include "stompbox_divider.h"
#include "stompbox_master.h"
#include "stompbox_mixer.h"
#include "stompbox_main_out_left.h"
#include "stompbox_main_out_right.h"
#include "stompbox_sub_out_left.h"
#include "stompbox_sub_out_right.h"
#include "stompbox_split.h"
#include "customControlParaEQ.h"
#include "customControlVU.h"

floorBoard::floorBoard(QWidget *parent,
                       QString imagePathFloor,
                       QString imagePathStompBG,
                       QString imagePathInfoBar,
                       unsigned int marginStompBoxesTop,
                       unsigned int marginStompBoxesBottom,
                       unsigned int marginStompBoxesWidth,
                       unsigned int panelBarOffset,
                       unsigned int borderWidth,
                       QPoint pos)
    : QWidget(parent)
{
    Preferences *preferences = Preferences::Instance();
    bool ok;
    const double ratio = preferences->getPreferences("Window", "Scale", "ratio").toDouble(&ok);

    QString setting = preferences->getPreferences("Scheme", "Colour", "select");

    int choice = setting.toInt(&ok, 16);
    // BUG FIX: Qt resource paths require ":/prefix/..." (colon-slash).
    // The old ":/images/..." format produced null QPixmaps, triggering
    // "QPainter::begin: Paint device returned engine == 0" and a blank floor.
    if(choice == 4) { imagePathFloor = ":/images/floor_white.png"; }
    else if(choice == 3) { imagePathFloor = ":/images/floor_green.png"; }
    else if(choice == 2) { imagePathFloor = ":/images/floor_aqua.png"; }
    else if(choice == 1) { imagePathFloor = ":/images/floor_black.png"; }
    else { imagePathFloor = ":/images/floor_blue.png"; };
    this->imagePathFloor = imagePathFloor;
    this->imagePathStompBG = imagePathStompBG;
    this->imagePathInfoBar = imagePathInfoBar;

    marginStompBoxesTop = 135*ratio;
    marginStompBoxesBottom = 72*ratio;
    marginStompBoxesWidth = 25*ratio;
    panelBarOffset = 10*ratio;
    borderWidth = 3*ratio;

    this->marginStompBoxesTop = marginStompBoxesTop;
    this->marginStompBoxesBottom = marginStompBoxesBottom;
    this->marginStompBoxesWidth = marginStompBoxesWidth;
    this->panelBarOffset = panelBarOffset;
    this->borderWidth = borderWidth;
    this->pos = pos;
    SysxIO *sysxIO = SysxIO::Instance();
    sysxIO->midi = new midiIO();
    bankTreeList *bankList = new bankTreeList(this);
    floorBoardDisplay *display = new floorBoardDisplay(this);
    display->setPos(displayPos);

    setFloorBoard();
    initStomps();
    initMenuPages();

    this->editDialog = new editWindow(this);
    this->oldDialog = this->editDialog;
    this->structureRefreshTimer = new QTimer(this);
    this->structureRefreshTimer->setSingleShot(true);
    //QObject::connect(this, SIGNAL( pageUpdateSignal() ), this->editDialog, SIGNAL(  update() ));

    QObject::connect(sysxIO, SIGNAL(updateSignal()), this, SIGNAL(updateSignal()));
    QObject::connect(this->structureRefreshTimer, SIGNAL(timeout()), this, SLOT(applyStructureRefresh()));

    QObject::connect(this, SIGNAL( resizeSignal(QRect) ), bankList, SLOT( updateSize(QRect) ) );
    QObject::connect(display, SIGNAL(connectedSignal()), bankList, SLOT(connectedSignal()));
    //  QObject::connect(this->parent(), SIGNAL(connectedSignal()), bankList, SLOT(connectedSignal()));
    QObject::connect(this, SIGNAL(setDisplayPos(QPoint)), display, SLOT(setPos(QPoint)));
    //QObject::connect(this, SIGNAL(setFloorPanelBarPos(QPoint)), panelBar, SLOT(setPos(QPoint)));
    QObject::connect(this->parent(), SIGNAL(updateSignal()), this, SIGNAL(updateSignal()));
    QObject::connect(this, SIGNAL(updateSignal()), this, SLOT(scheduleStructureRefresh()));
    QObject::connect(this->parent(), SIGNAL(updateSignal()), this, SLOT(schedulePatchStructureRefresh()));
    QObject::connect(bankList, SIGNAL(updateSignal()), this, SLOT(schedulePatchStructureRefresh()));
    //QObject::connect(this, SIGNAL(updateSignal()), this, SLOT(update_structure()));
    QObject::connect(bankList, SIGNAL(patchSelectSignal(int, int)), display, SLOT(patchSelectSignal(int, int)));
    QObject::connect(bankList, SIGNAL(patchLoadSignal(int, int)), display, SLOT(patchLoadSignal(int, int)));
    QObject::connect(this, SIGNAL(notConnected()), display, SLOT(notConnected()));
    QObject::connect(this, SIGNAL(updateNameSignal(QString, int, int)), bankList, SLOT(updateNameSignal(QString, int, int)));

    QString collapseState = preferences->getPreferences("Window", "Collapsed", "bool");
    QString width = preferences->getPreferences("Window", "Collapsed", "width");
    QString defaultwidth = preferences->getPreferences("Window", "Collapsed", "defaultwidth");
    if(width.isEmpty()){ width = defaultwidth; }

    this->l_floorSize = QSize(width.toInt(&ok, 10), floorSize.height());

    if(preferences->getPreferences("Window", "Restore", "sidepanel")=="true")
    {
        if(collapseState=="true")
        {
            this->setSize(l_floorSize);
            this->colapseState = true;
        }
        else
        {
            this->setSize(minSize);
            this->colapseState = false;
        };
    }
    else
    {
        this->setSize(minSize);
        this->colapseState = false;
    };
    emit setCollapseState(this->colapseState);
    emit mod_buttonSignal(bool (true));
    // Keep the bank browser panel above floor widgets so PRESET/USER tab clicks
    // are never swallowed by overlapping transparent child widgets.
    bankList->raise();
    emit updateSignal();
    schedulePatchStructureRefresh();
}

floorBoard::~floorBoard()
{
    emit notConnected();
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "Restore", "sidepanel")=="true")
    {
        preferences->setPreferences("Window", "Collapsed", "width", QString::number(this->l_floorSize.width(), 10));
    }
    else
    {
        preferences->setPreferences("Window", "Collapsed", "width", "");
    };
    preferences->setPreferences("Window", "Collapsed", "bool", QString(this->colapseState?"true":"false"));
    preferences->savePreferences();
}

void floorBoard::paintEvent(QPaintEvent *)
{
    Preferences *preferences = Preferences::Instance();
    bool ok;
    const double ratio = preferences->getPreferences("Window", "Scale", "ratio").toDouble(&ok);

    QRectF target(pos.x(), pos.y(), floorSize.width()*ratio, floorSize.height()*ratio);
    QRectF source(0.0, 0.0, floorSize.width(), floorSize.height());
    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    painter.drawPixmap(target, this->image, source);
    this->setAcceptDrops(true);

    QPen whitePen(Qt::white, 2*ratio, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
    QPen greyPen(Qt::darkGray, 2*ratio, Qt::DashLine, Qt::FlatCap, Qt::MiterJoin);
    painter.setPen(whitePen);

    if(polygon.size() < 20)
    {
        return;
    }

    // Enforce orthogonal routing: every connector is horizontal/vertical only.
    auto drawOrthogonal = [&painter](QPoint a, QPoint b, bool preferHorizontal = true)
    {
        if(a.x() == b.x() || a.y() == b.y())
        {
            painter.drawLine(a, b);
            return;
        }

        if(preferHorizontal)
        {
            const QPoint corner(b.x(), a.y());
            painter.drawLine(a, corner);
            painter.drawLine(corner, b);
        }
        else
        {
            const QPoint corner(a.x(), b.y());
            painter.drawLine(a, corner);
            painter.drawLine(corner, b);
        }
    };

    drawOrthogonal(polygon.at(0), polygon.at(1), true);    //index1 = instrument 1 to balancer 1 A in.
    drawOrthogonal(polygon.at(2), polygon.at(3), true);    //index2 = instrument 2 to balancer 1 B in.
    drawOrthogonal(polygon.at(1), polygon.at(3), false);   //link 1-2 = draw balancer 1 A to B.
    drawOrthogonal(polygon.at(4), polygon.at(5), true);    //index3  = balancer 1 out to balancer 2 A in.
    drawOrthogonal(polygon.at(6), polygon.at(7), true);    //index4  = instrument 3 to balancer 2 B in.
    drawOrthogonal(polygon.at(5), polygon.at(7), false);   //link 3-4 = balancer 2 A in to balancer 2 B in.
    drawOrthogonal(polygon.at(8), polygon.at(9), true);    //index5 = balancer 2 out to balancer 3 A in.
    drawOrthogonal(polygon.at(10), polygon.at(11), true);  //index6 = normal input to balancer 3 B in.
    drawOrthogonal(polygon.at(9), polygon.at(11), false);  //link 5-6 = balancer 3 A in to balancer 3 B in.
    drawOrthogonal(polygon.at(12), polygon.at(13), true);  //index7-end  = balancer 3 out to divider center.
    drawOrthogonal(polygon.at(18), polygon.at(19), true);  // mixer center to  Master block.
    if(dual_channel>0)
    {   // if dual channel, draw both A & B routes in white.
        drawOrthogonal(polygon.at(14), polygon.at(15), true);    //index8 = divider upper to mixer upper.
        drawOrthogonal(polygon.at(16), polygon.at(17), true);    //index9 = divider lower t mixer lower.
        drawOrthogonal(polygon.at(14), polygon.at(16), false);   //link 8-9 DIV = divider upper to divider lower.
        drawOrthogonal(polygon.at(15), polygon.at(17), false);   //link 8-9 MIX = mixer upper to mixer lower.
    }
    else
    {
        if(B_channel>0)
        {  // if single B channel, draw B route white, A route grey.
            drawOrthogonal(polygon.at(16), polygon.at(17), true);    //index9 = divider lower to mixer lower.
            drawOrthogonal(polygon.at(13), polygon.at(16), false);   //link 9 = divider centre to divider lower.
            drawOrthogonal(polygon.at(18), polygon.at(17), false);   //link 9 = mixer centre to mixer lower.

            painter.setPen(greyPen);
            drawOrthogonal(polygon.at(14), polygon.at(15), true);    //index8 = divider upper to mixer upper.
            drawOrthogonal(polygon.at(13), polygon.at(14), false);   //link 8 = divider centre to divider upper.
            drawOrthogonal(polygon.at(18), polygon.at(15), false);   //link 8 = mixer center to mixer upper.
        }
        else
        {
            drawOrthogonal(polygon.at(14), polygon.at(15), true);    //index8 = divider upper to mixer upper.
            drawOrthogonal(polygon.at(13), polygon.at(14), false);   //link 8 = divider centre to divider upper.
            drawOrthogonal(polygon.at(18), polygon.at(15), false);   //link 8 = mixer center to mixer upper.

            painter.setPen(greyPen);
            drawOrthogonal(polygon.at(16), polygon.at(17), true);    //index9 = divider lower to mixer lower.
            drawOrthogonal(polygon.at(13), polygon.at(16), false);   //link 9 = divider centre to divider lower.
            drawOrthogonal(polygon.at(18), polygon.at(17), false);   //link 9  = mixer centre to mixer lower.
        };
    };
    //painter.setPen(blackPen);
    //painter.drawLine(polygon.at(13), polygon.at(18));

    if(dropIndicatorVisible)
    {
        QPen insertPen(QColor(255, 0, 0), qMax(2, qRound(2*ratio)), Qt::SolidLine);
        painter.setPen(insertPen);
        painter.drawLine(QPoint(dropIndicatorX, dropIndicatorTopY), QPoint(dropIndicatorX, dropIndicatorBottomY));
    }

}

void floorBoard::setFloorBoard()
{
    Preferences *preferences = Preferences::Instance();
    bool ok;
    double ratio = preferences->getPreferences("Window", "Scale", "ratio").toDouble(&ok);

    QPixmap imageFloor(imagePathFloor);
    QPixmap imagestompBG(imagePathStompBG);
    QPixmap buffer = imageFloor;
    QPainter painter(&buffer);

    // Keep the left patch browser compact so the signal-flow/editor area has more room.
    const int rawOffset = qMax(0, qRound(imageFloor.width()*ratio - (997*ratio)));
    this->offset = qMax(int(150*ratio), rawOffset/2);
    this->stompSize = imagestompBG.size()*ratio/3;
    this->structureHeight = 60*ratio;

    initSize(imageFloor.size());

    this->maxSize = floorSize;
    this->minSize = QSize(1000 + borderWidth + panelBarOffset, imageFloor.height());

    this->baseImage = buffer;
    this->image = buffer;
    this->floorHeight = imageFloor.height()*ratio;

    QPoint newDisplayPos = QPoint(offset, 0);
    this->displayPos = newDisplayPos;

    QRect newBankListRect = QRect(borderWidth, borderWidth, offset - panelBarOffset - (borderWidth*2), floorHeight - (borderWidth*2));
    emit resizeSignal(newBankListRect);
}

void floorBoard::initSize(QSize floorSize)
{
    this->floorSize = floorSize;
    this->l_floorSize = floorSize;
    update_structure();
}

QPoint floorBoard::getStompPos(int id)
{
    return this->fxPos.at(id);
}

void floorBoard::setCollapse()
{
    if(floorSize.width() > minSize.width())
    {
        this->l_floorSize = floorSize;
        this->setSize(minSize);
        this->colapseState = false;
    }
    else
    {
        this->setSize(l_floorSize);        
        this->colapseState = true;
    };
    emit setCollapseState(this->colapseState);
}

void floorBoard::setSize(QSize newSize)
{
    Preferences *preferences = Preferences::Instance();
    bool ok;
    double ratio = preferences->getPreferences("Window", "Scale", "ratio").toDouble(&ok);

    uint oldOffset = offset;
    const int rawOffset = qMax(0, qRound(newSize.width()*ratio - (997*ratio)));
    this->offset = qMax(int(150*ratio), rawOffset/2);
    QSize oldFloorSize = this->floorSize;
    this->floorSize = newSize;

    for(int i=0;i<this->fxPos.size();i++)
    {
        this->fxPos[i] = this->fxPos[i] + QPoint(offset - oldOffset, 0);
    };
    emit updateStompOffset(offset - oldOffset);

    QPoint newDisplayPos = QPoint(offset, displayPos.y());
    emit setDisplayPos(newDisplayPos);
    this->displayPos = newDisplayPos;

    QPixmap buffer = QPixmap(floorSize);
    QPainter painter(&buffer);

    // Redraw new resized floor
    QRectF source(maxSize.width() - floorSize.width(), 0.0, floorSize.width(), floorSize.height());
    QRectF target(0.0, 0.0, floorSize.width(), floorSize.height());
    painter.drawPixmap(target, baseImage, source);
    painter.end();

    this->image = buffer;

    QRect newBankListRect = QRect(borderWidth, borderWidth, offset - panelBarOffset - (borderWidth*2), floorHeight - (borderWidth*2));
    emit resizeSignal(newBankListRect);

    emit sizeChanged(floorSize, oldFloorSize);
    this->centerEditDialog();
}

void floorBoard::setWidth(int dist)
{
    QSize newSize;
    if(floorSize.width() + dist < minSize.width())
    {
        newSize = minSize;
        this->colapseState = false;
    }
    else if(floorSize.width() + dist > maxSize.width())
    {
        newSize = maxSize;
        this->l_floorSize = newSize;
        this->colapseState = true;
    }
    else
    {
        newSize = QSize(floorSize.width() + dist, floorSize.height());
        this->l_floorSize = newSize;
        this->colapseState = true;
    };
    emit setCollapseState(this->colapseState);
    this->setSize(newSize);
}

void floorBoard::initSoundSource()
{

}

void floorBoard::initStomps()
{
    Preferences *preferences = Preferences::Instance();
    bool ok;
    const double ratio = preferences->getPreferences("Window", "Scale", "ratio").toDouble(&ok);

    QList<signed int> fx;
    fx << 0 << 1 << 2 << 3 << 4 << 5 << 6 << 7 << 8 << 9 << 10 << 11 << 12 << 13 << 14 << 15 << 16 << 17 << 18 << 19 << 20 << 21 << 22 << 23 << 25 << 26 << 27 << 28 << 29 << 30 << 31 << 32 << 33 << 34;
    this->fx = fx;

    QVector<QString> initStompNames(35);
    this->stompNames = initStompNames.toList();;

    QVector<stompBox *> initStompBoxes(35);
    this->stompBoxes = initStompBoxes.toList();;

    QString hex1 = "00";
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true")
    {
        hex1 = "02";
    };

    //Normal Pick Up
    stompBox *normal = new soundsource_normal(this);
    normal->setId(0);
    normal->setPos(QPoint(offset+(15*ratio), 260*ratio));
    this->stompBoxes.replace(0, normal);
    this->stompNames.replace(0, "3");

    //Inst 1
    stompBox *inst1 = new soundsource_inst1(this);
    inst1->setId(1);
    inst1->setPos(QPoint(offset+(15*ratio), 65*ratio));
    this->stompBoxes.replace(1, inst1);
    this->stompNames.replace(1, "0");

    //Inst 2
    stompBox *inst2 = new soundsource_inst2(this);
    inst2->setId(2);
    inst2->setPos(QPoint(offset+(15*ratio), 130*ratio));
    this->stompBoxes.replace(2, inst2);
    this->stompNames.replace(2, "1");

    //Inst 3
    stompBox *inst3 = new soundsource_inst3(this);
    inst3->setId(3);
    inst3->setPos(QPoint(offset+(15*ratio), 195*ratio));
    this->stompBoxes.replace(3, inst3);
    this->stompNames.replace(3, "2");

    //Comp
    stompBox *cs = new stompbox_cs(this);
    cs->setId(4);
    cs->setPos(QPoint(offset + (160*ratio), 60*ratio));
    this->stompBoxes.replace(4, cs);
    this->stompNames.replace(4, "4");

    //Distortion
    stompBox *ds = new stompbox_ds(this);
    ds->setId(5);
    ds->setPos(QPoint(offset + (360*ratio), 100*ratio));
    this->stompBoxes.replace(5, ds);
    this->stompNames.replace(5, "5");

    //Amp
    stompBox *amp = new stompbox_amp(this);
    amp->setId(6);
    amp->setPos(QPoint(offset + (80*ratio), 20*ratio));
    this->stompBoxes.replace(6, amp);
    this->stompNames.replace(6, "6");

    //Noise suppressor
    stompBox *ns = new stompbox_ns(this);
    ns->setId(7);
    ns->setPos(QPoint(offset + (80*ratio), 60*ratio));
    this->stompBoxes.replace(7, ns);
    this->stompNames.replace(7, "7");

    //FX1
    stompBox *fx1 = new stompbox_fx1(this);
    fx1->setId(8);
    fx1->setPos(QPoint(offset + (80*ratio), 140*ratio));
    this->stompBoxes.replace(8, fx1);
    this->stompNames.replace(8, "8");

    //FX2
    stompBox *fx2 = new stompbox_fx2(this);
    fx2->setId(9);
    fx2->setPos(QPoint(offset + (80*ratio), 100*ratio));
    this->stompBoxes.replace(9, fx2);
    this->stompNames.replace(9, "9");

    //FX3
    stompBox *fx3 = new stompbox_fx3(this);
    fx3->setId(10);
    fx3->setPos(QPoint(offset + (120*ratio), 60*ratio));
    this->stompBoxes.replace(10, fx3);
    this->stompNames.replace(10, "10");

    //Eq1
    stompBox *eq1 = new stompbox_eq1(this);
    eq1->setId(11);
    eq1->setPos(QPoint(offset + (280*ratio), 100*ratio));
    this->stompBoxes.replace(11, eq1);
    this->stompNames.replace(11, "11");

    //Eq2
    stompBox *eq2 = new stompbox_eq2(this);
    eq2->setId(12);
    eq2->setPos(QPoint(offset + (320*ratio), 100*ratio));
    this->stompBoxes.replace(12, eq2);
    this->stompNames.replace(12, "12");

    //Chorus
    stompBox *ce = new stompbox_ce(this);
    ce->setId(13);
    ce->setPos(QPoint(offset + (120*ratio), 100*ratio));
    this->stompBoxes.replace(13, ce);
    this->stompNames.replace(13, "13");

    // Delay1
    stompBox *dd1 = new stompbox_dd1(this);
    dd1->setId(14);
    dd1->setPos(QPoint(offset + (200*ratio), 60*ratio));
    this->stompBoxes.replace(14, dd1);
    this->stompNames.replace(14, "14");

    // Delay2
    stompBox *dd2 = new stompbox_dd2(this);
    dd2->setId(15);
    dd2->setPos(QPoint(offset + (200*ratio), 100*ratio));
    this->stompBoxes.replace(15, dd2);
    this->stompNames.replace(15, "15");

    // Delay3
    stompBox *dd3 = new stompbox_dd3(this);
    dd3->setId(16);
    dd3->setPos(QPoint(offset + (240*ratio), 100*ratio));
    this->stompBoxes.replace(16, dd3);
    this->stompNames.replace(16, "16");

    //Reverb
    stompBox *rv = new stompbox_rv(this);
    rv->setId(17);
    rv->setPos(QPoint(offset + (160*ratio), 100*ratio));
    this->stompBoxes.replace(17, rv);
    this->stompNames.replace(17, "17");

    //FV1
    stompBox *fv1 = new stompbox_fv1(this);
    fv1->setId(18);
    fv1->setPos(QPoint(offset + (440*ratio), 100*ratio));
    this->stompBoxes.replace(18, fv1);
    this->stompNames.replace(18, "18");

    //FV2
    stompBox *fv2 = new stompbox_fv2(this);
    fv2->setId(19);
    fv2->setPos(QPoint(offset + (480*ratio), 100*ratio));
    this->stompBoxes.replace(19, fv2);
    this->stompNames.replace(19, "19");

    //SR Loop
    stompBox *lp = new stompbox_lp(this);
    lp->setId(20);
    lp->setPos(QPoint(offset + (400*ratio), 100*ratio));
    this->stompBoxes.replace(20, lp);
    this->stompNames.replace(20, "20");

    //divider
    stompBox *divider = new stompbox_divider(this);
    divider->setId(21);
    divider->setPos(QPoint(offset + (640*ratio), 100*ratio));
    this->stompBoxes.replace(21, divider);
    this->stompNames.replace(21, "21");

    stompBox *branch_AB = new stompbox_branch_AB(this);
    branch_AB->setId(22);
    branch_AB->setPos(QPoint(offset + (120*ratio), 140*ratio));
    this->stompBoxes.replace(22, branch_AB);
    this->stompNames.replace(22, "22");

    //mixer
    stompBox *mixer = new stompbox_mixer(this);
    mixer->setId(23);
    mixer->setPos(QPoint(offset + (680*ratio), 100*ratio));
    this->stompBoxes.replace(23, mixer);
    this->stompNames.replace(23, "23");

    //sub left
    stompBox *sub_out_left = new stompbox_sub_out_left(this);
    sub_out_left->setId(24);
    sub_out_left->setPos(QPoint(offset + (800*ratio), 100*ratio));
    this->stompBoxes.replace(24, sub_out_left);
    this->stompNames.replace(24, "24");

    //sub right
    stompBox *sub_out_right = new stompbox_sub_out_right(this);
    sub_out_right->setId(25);
    sub_out_right->setPos(QPoint(offset + (840*ratio), 100*ratio));
    this->stompBoxes.replace(25, sub_out_right);
    this->stompNames.replace(25, "25");

    //main left
    stompBox *main_out_left = new stompbox_main_out_left(this);
    main_out_left->setId(26);
    main_out_left->setPos(QPoint(offset + (720*ratio), 100*ratio));
    this->stompBoxes.replace(26, main_out_left);
    this->stompNames.replace(26, "26");

    //main right
    stompBox *main_out_right = new stompbox_main_out_right(this);
    main_out_right->setId(27);
    main_out_right->setPos(QPoint(offset + (760*ratio), 100*ratio));
    this->stompBoxes.replace(27, main_out_right);
    this->stompNames.replace(27, "27");

    stompBox *split1 = new stompbox_split(this);
    split1->setId(28);
    split1->setPos(QPoint(offset + (160*ratio), 140*ratio));
    this->stompBoxes.replace(28, split1);
    this->stompNames.replace(28, "28");

    //Balancer1
    stompBox *balancer1 = new stompbox_balancer1(this);
    balancer1->setId(29);
    balancer1->setPos(QPoint(offset + (520*ratio), 100*ratio));
    this->stompBoxes.replace(29, balancer1);
    this->stompNames.replace(29, "29");

    stompBox *split2 = new stompbox_split(this);
    split2->setId(30);
    split2->setPos(QPoint(offset + (200*ratio), 140*ratio));
    this->stompBoxes.replace(30, split2);
    this->stompNames.replace(30, "30");

    //Balancer2
    stompBox *balancer2 = new stompbox_balancer2(this);
    balancer2->setId(31);
    balancer2->setPos(QPoint(offset + (560*ratio), 100*ratio));
    this->stompBoxes.replace(31, balancer2);
    this->stompNames.replace(31, "31");

    stompBox *split3 = new stompbox_split(this);
    split3->setId(32);
    split3->setPos(QPoint(offset + (240*ratio), 140*ratio));
    this->stompBoxes.replace(32, split3);
    this->stompNames.replace(32, "32");

    //Balancer3
    stompBox *balancer3 = new stompbox_balancer3(this);
    balancer3->setId(33);
    balancer3->setPos(QPoint(offset + (600*ratio), 100*ratio));
    this->stompBoxes.replace(33, balancer3);
    this->stompNames.replace(33, "33");

    //master
    stompBox *master = new stompbox_master(this);
    master->setId(34);
    master->setPos(QPoint(offset + (880*ratio), 100*ratio));
    this->stompBoxes.replace(34, master);
    this->stompNames.replace(34, "34");
}

void floorBoard::setStomps(QList<QString> stompOrder)
{
    stompOrder = normalizeInputOrder(stompOrder);

    for(int i=0;i<stompOrder.size();i++)
    {
        QString id = stompOrder.at(i);
        setStompPos(id, i);
        this->fx.replace(i, stompNames.indexOf(id));
    };
}

QList<QString> floorBoard::normalizeInputOrder(const QList<QString> &stompOrder) const
{
    // Keep source chain fixed forever: INST1, INST2, INST3, NORMAL.
    // In stomp-name space this is "0","1","2","3".
    const QList<QString> fixedInputs = {"0", "1", "2", "3"};
    QList<QString> normalized = fixedInputs;

    for(const QString &item : stompOrder)
    {
        if(!fixedInputs.contains(item))
        {
            normalized.append(item);
        }
    }

    if(normalized.size() > stompOrder.size())
    {
        normalized = normalized.mid(0, stompOrder.size());
    }

    return normalized;
}

void floorBoard::setStompPos(QString name, int order)
{
    this->stompBoxes.at(stompNames.indexOf(name))->setPos(this->getStompPos(order));
    this->fx.replace(order, stompNames.indexOf(name));
}

void floorBoard::setStompPos(int index, int order)
{
    this->stompBoxes.at(index)->setPos(this->getStompPos(order));
    this->fx.replace(order, index);
}

void floorBoard::dragEnterEvent(QDragEnterEvent *event)
{
    QByteArray data = event->mimeData()->data("text/uri-list");
    QString uri(data);

    // NOTE: do NOT use event->answerRect().intersects(this->geometry()) here.
    // answerRect() is in widget-LOCAL coordinates; geometry() is in PARENT
    // coordinates.  Mixing them causes valid drops to be silently rejected
    // whenever the widget has any non-zero offset in its parent.  The event
    // itself is only dispatched when the cursor is already over this widget,
    // so the geometry check is redundant anyway.
    if (event->mimeData()->hasFormat("application/x-stompbox") ||
        uri.contains(".syx", Qt::CaseInsensitive))
    {
        if (children().contains(event->source()))
        {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        };
    } else {
        event->ignore();
    };
}

void floorBoard::dragMoveEvent(QDragMoveEvent *event)
{
    QByteArray data = event->mimeData()->data("text/uri-list");
    QString uri(data);

    // Same coordinate-system fix as dragEnterEvent — see comment there.
    if (event->mimeData()->hasFormat("application/x-stompbox") ||
        uri.contains(".syx", Qt::CaseInsensitive))
    {
        if (children().contains(event->source()))
        {
            int stompId = -1;
            QPoint stompPos;
            QSize draggedSize;
            QPoint topLeftOffset;
            if(decodeDraggedStompData(event->mimeData(), stompId, stompPos, draggedSize, topLeftOffset))
            {
                Q_UNUSED(stompPos);
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
                QPoint dragPoint = (event->position().toPoint() - topLeftOffset) + QPoint(draggedSize.width()/2, draggedSize.height()/2);
#else
                QPoint dragPoint = (event->pos() - topLeftOffset) + QPoint(draggedSize.width()/2, draggedSize.height()/2);
#endif
                int destIndex = calculateDropIndex(dragPoint, stompId);
                updateDropIndicator(destIndex, stompId, dragPoint.y());
            }
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            clearDropIndicator();
            event->acceptProposedAction();
        };
    } else {
        clearDropIndicator();
        event->ignore();
    };
}

void floorBoard::dragLeaveEvent(QDragLeaveEvent *event)
{
    clearDropIndicator();
    event->accept();
}

bool floorBoard::decodeDraggedStompData(const QMimeData *mimeData, int &stompId, QPoint &stompPos, QSize &draggedSize, QPoint &topLeftOffset) const
{
    if(!mimeData || !mimeData->hasFormat("application/x-stompbox"))
    {
        return false;
    }

    QByteArray itemData = mimeData->data("application/x-stompbox");
    if(itemData.isEmpty())
    {
        return false;
    }

    QDataStream dataStream(&itemData, QIODevice::ReadOnly);
    dataStream >> stompId >> stompPos >> draggedSize >> topLeftOffset;
    if(dataStream.status() != QDataStream::Ok)
    {
        return false;
    }
    return (stompId >= 0 && stompId < 34);
}

int floorBoard::calculateDropIndex(const QPoint &dragPoint, int stompId) const
{
    if(stompId < 0 || stompId >= 34 || fx.isEmpty() || fxPos.isEmpty())
    {
        return -1;
    }

    int dragWidth = stompSize.width();
    int dragHeight = stompSize.height();
    if(stompId < stompBoxes.size() && stompBoxes.at(stompId))
    {
        dragWidth = stompBoxes.at(stompId)->width();
        dragHeight = stompBoxes.at(stompId)->height();
    }
    if(dragWidth <= 0 || dragHeight <= 0)
    {
        return -1;
    }

    int stompSpacing = 0;
    if(fxPos.size() > 1)
    {
        stompSpacing = fxPos.at(1).x() - (fxPos.at(0).x() + dragWidth);
    }

    int destIndex = -1;
    for(int x=0; x<fx.size(); ++x)
    {
        QRect dropRect(fxPos.at(x).x(), fxPos.at(x).y(), dragWidth, dragHeight);
        QRect lastDropRect(fxPos.at(x).x() + (dragWidth/2), fxPos.at(x).y(), dragWidth + stompSpacing, dragHeight);
        if(dropRect.contains(dragPoint))
        {
            destIndex = fx.indexOf(fx.at(x));
        }
        else if(x == (fx.size()-1) && lastDropRect.contains(dragPoint))
        {
            destIndex = fx.indexOf(fx.at(x)) + 1;
        }
    }

    if(destIndex < 0)
    {
        return -1;
    }

    bool fit = false;
    if(destIndex < 4 && stompId < 4) { fit = true; }
    if(destIndex > 3 && stompId > 3) { fit = true; }
    if(!fit)
    {
        return -1;
    }

    if(destIndex >= fx.size())
    {
        destIndex = fx.size()-1;
    }
    return destIndex;
}

int floorBoard::indicatorXForIndex(int index, int stompId) const
{
    if(index < 0 || fx.isEmpty() || fxPos.isEmpty())
    {
        return -1;
    }

    int dragWidth = stompSize.width();
    if(stompId >= 0 && stompId < stompBoxes.size() && stompBoxes.at(stompId))
    {
        dragWidth = stompBoxes.at(stompId)->width();
    }

    if(index == 0)
    {
        return fxPos.at(0).x() - 6;
    }

    if(index >= fx.size())
    {
        int lastId = fx.last();
        int lastWidth = dragWidth;
        if(lastId >= 0 && lastId < stompBoxes.size() && stompBoxes.at(lastId))
        {
            lastWidth = stompBoxes.at(lastId)->width();
        }
        return fxPos.last().x() + lastWidth + 6;
    }

    int prev = index - 1;
    int prevId = fx.at(prev);
    int prevWidth = dragWidth;
    if(prevId >= 0 && prevId < stompBoxes.size() && stompBoxes.at(prevId))
    {
        prevWidth = stompBoxes.at(prevId)->width();
    }
    int prevRight = fxPos.at(prev).x() + prevWidth;
    int currLeft = fxPos.at(index).x();
    if(currLeft <= prevRight)
    {
        return currLeft;
    }
    return prevRight + ((currLeft - prevRight) / 2);
}

void floorBoard::updateDropIndicator(int index, int stompId, int dragY)
{
    if(index < 0)
    {
        clearDropIndicator();
        return;
    }

    int x = indicatorXForIndex(index, stompId);
    if(x < 0)
    {
        clearDropIndicator();
        return;
    }

    int top = INT_MAX;
    int bottom = INT_MIN;
    int nearestCenterY = INT_MIN;
    int nearestDelta = INT_MAX;
    for(int i=0; i<fx.size(); ++i)
    {
        int id = fx.at(i);
        if(id < 0 || id >= 28 || id >= stompBoxes.size() || !stompBoxes.at(id))
        {
            continue;
        }
        const QPoint &p = fxPos.at(i);
        const int boxTop = p.y();
        const int boxBottom = p.y() + stompBoxes.at(id)->height();
        const int centerY = boxTop + ((boxBottom - boxTop) / 2);
        top = qMin(top, boxTop);
        bottom = qMax(bottom, boxBottom);

        if(dragY >= 0)
        {
            const int delta = qAbs(centerY - dragY);
            if(delta < nearestDelta)
            {
                nearestDelta = delta;
                nearestCenterY = centerY;
            }
        }
    }

    if(top == INT_MAX || bottom == INT_MIN)
    {
        clearDropIndicator();
        return;
    }

    if(nearestCenterY == INT_MIN)
    {
        nearestCenterY = top + ((bottom - top) / 2);
    }

    const int halfLengthPx = 30;
    top = nearestCenterY - halfLengthPx;
    bottom = nearestCenterY + halfLengthPx;

    if(dropIndicatorVisible &&
       dropIndicatorX == x &&
       dropIndicatorTopY == top &&
       dropIndicatorBottomY == bottom &&
       dropIndicatorIndex == index)
    {
        return;
    }

    dropIndicatorVisible = true;
    dropIndicatorX = x;
    dropIndicatorTopY = top;
    dropIndicatorBottomY = bottom;
    dropIndicatorIndex = index;
    update();
}

void floorBoard::clearDropIndicator()
{
    if(!dropIndicatorVisible)
    {
        return;
    }

    dropIndicatorVisible = false;
    dropIndicatorIndex = -1;
    update();
}

void floorBoard::dropEvent(QDropEvent *event)
{
    clearDropIndicator();
    if (event->mimeData()->hasFormat("application/x-stompbox"))
    {
        int stompId = -1;
        QPoint stompPos;
        QSize draggedSize;
        QPoint topLeftOffset;
        if(!decodeDraggedStompData(event->mimeData(), stompId, stompPos, draggedSize, topLeftOffset))
        {
            event->ignore();
            return;
        }
        Q_UNUSED(stompPos);
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
        QPoint dragPoint = (event->position().toPoint() - topLeftOffset) + QPoint(draggedSize.width()/2, draggedSize.height()/2);
#else
        QPoint dragPoint = (event->pos() - topLeftOffset) + QPoint(draggedSize.width()/2, draggedSize.height()/2);
#endif
        int destIndex = calculateDropIndex(dragPoint, stompId);
        int orgIndex = fx.indexOf(stompId);

        // Lock source ordering: INST1, INST2, INST3, NORMAL.
        // Source blocks cannot be dragged, and no block can be dropped into
        // the first four source slots.
        if(stompId >= 0 && stompId <= 3)
        {
            event->ignore();
            return;
        }
        if(destIndex >= 0 && destIndex < 4)
        {
            destIndex = 4;
        }

        if((destIndex > -1 && destIndex < fx.size()) && (stompId < 34))
        // Make sure we are not dropping the stomp out of range.
        {
            if( orgIndex < destIndex )
            {
                for(int i=orgIndex;i<destIndex ;i++)
                {
                    setStompPos(fx.at(i + 1), i);
                };
                setStompPos(stompId, destIndex );
            }
            else if( orgIndex >= destIndex )
            {
                for(int i=orgIndex;i>destIndex;i--)
                {
                    setStompPos(fx.at(i - 1), i);
                };
                setStompPos(stompId, destIndex);
            };

            // Re-apply canonical source ordering after every drop so this
            // cannot drift in saved or live layouts.
            QList<QString> normalizedOrder;
            for(int i = 0; i < fx.size(); ++i)
            {
                normalizedOrder.append(stompNames.at(fx.at(i)));
            }
            setStomps(normalizeInputOrder(normalizedOrder));

            if(orgIndex != destIndex) // Prevent sending data when stomp was dropped in the same place.
            {
                SysxIO *sysxIO = SysxIO::Instance();
                //MidiTable *midiTable = MidiTable::Instance();
                QList<QString> hexData;
                QString deta;
                QString hex1 = "00";
                Preferences *preferences = Preferences::Instance();
                if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){hex1 = "02"; };

                for(int index=0;index<4;index++)
                {
                    QString value = QString::number(fx.at(index), 10);
                    if(value.length() < 2) value.prepend("0");
                    QString stomp = value;
                    hexData.append(stomp);
                };

                for(int index=4;index<34;index++)
                {
                    bool ok;
                    QString value = QString::number(fx.at(index)-4, 16).toUpper();
                    if(value.length() < 2) value.prepend("0");
                    if(value.toInt(&ok, 16) > 24){value = "18";};
                    QString stomp = value;
                    hexData.append(stomp);
                };

                QString sys1 = "04";
                if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){sys1 = "0B";};

                int main_link = sysxIO->getSourceValue("00", sys1, "00", "34");  // main stereo  link
                int sub_link = sysxIO->getSourceValue("00", sys1, "00", "35");  // sub stereo  link

                auto moveLinkedBlock = [&hexData](const QString &leftId, const QString &rightId)
                {
                    const int rightIndex = hexData.indexOf(rightId);
                    if(rightIndex < 0)
                    {
                        return;
                    }
                    hexData.removeAt(rightIndex);
                    const int leftIndex = hexData.indexOf(leftId);
                    if(leftIndex < 0)
                    {
                        return;
                    }
                    hexData.insert(leftIndex + 1, rightId);
                };
                if(main_link==1){moveLinkedBlock("16", "17");}; //if main linked, move together
                if(sub_link==1){moveLinkedBlock("14", "15");}; //if sub linked, move together
                if(stompId==21) // if moving divider, copy last chain order between divider and mixer, and move them all together.
                {
                    deta.clear();
                    const int dividerIndex = _last_chain.indexOf("11", 4);
                    const int mixerIndex = _last_chain.indexOf("13", 4);
                    if(dividerIndex >= 0 && mixerIndex > dividerIndex)
                    {
                        const int offset = dividerIndex + 1;  // start position after the divider.
                        const int offset2 = mixerIndex + 1;   // end position including the mixer.
                        const int length = offset2-offset;
                        QList<QString> ab_branch = _last_chain.mid(offset, length); // copy the chain items between divider and mixer.

                        for(int x=0; x<length; x++)
                        {
                            const int removeIndex = hexData.indexOf(ab_branch.at(x), 4);
                            if(removeIndex >= 0)
                            {
                                hexData.removeAt(removeIndex);  //remove the chain items.
                            }
                        };
                        const int insertBase = hexData.indexOf("11");
                        if(insertBase >= 0)
                        {
                            for(int x=0; x<length; x++)
                            {
                                hexData.insert(insertBase + x + 1, ab_branch.at(x));  // replace the chain items after the divider.
                                deta.append(ab_branch.at(x)+" ");
                            };
                        }
                        if(preferences->getPreferences("Midi", "DBug", "bool")=="true")
                        {
                            sysxIO->deBug("ab_branch="+deta+" offset="+ QString::number(offset, 10) + " length="+QString::number(length, 10));
                        };
                    }
                };

                int chk = 0;  // check all chain bytes add up to 426, and check 4 chain input bytes add up to 6.
                int chk2 = 0;
                bool ok;
                deta.clear();
                for(int k=0; k<34; k++)
                {
                    deta.append(hexData.at(k));
                    deta.append(QString(" "));
                    chk=chk+hexData.at(k).toInt(&ok, 16);
                };
                if(preferences->getPreferences("Midi", "DBug", "bool")=="true")
                {
                    sysxIO->deBug(QString("  floorBoard::dropEvent = "+ deta));
                };

                for(int k=0; k<4; k++){chk2=chk2+hexData.at(k).toInt(&ok, 16);};
                if(chk!=426 || chk2 != 6)
                {
                    event->ignore();
                }
                else
                {
                    sysxIO->setFileSource("10", hex1, "12", "45", hexData);
                    scheduleChainStructureRefresh();
                };
            };
        }
        else
        {
            event->ignore();
        };
    }
    else
    {
        if(event->mimeData()->hasFormat("text/uri-list"))
        {
            QByteArray data = event->mimeData()->data("text/uri-list");
            QString uri(data);
            if(uri.contains(".syx", Qt::CaseInsensitive))
            {
                QString removeFromStart = "file:///";
                QString removeFromEnd = ".syx";
                QString filePath = uri;
                filePath.replace(0, filePath.indexOf(removeFromStart) + removeFromStart.length(), "");
                filePath.truncate(filePath.indexOf(removeFromEnd) + removeFromEnd.length());
                filePath.replace("%20", " ");

                SysxIO *sysxIO = SysxIO::Instance();
                sysxIO->setFileName(filePath);
                sysxIO->setSyncStatus(false);
                sysxIO->setDevice(false);

                sysxWriter file;
                file.setFile(filePath);
                if(file.readFile())
                {
                    emit updateSignal();
                }
                else
                {
                    event->ignore();
                };
            }
            else
            {
                event->ignore();
            };
        }
        else
        {
            event->ignore();
        };
    };
    updateStompBoxes();
}

void floorBoard::setEditDialog(editWindow* editDialog)
{
    this->oldDialog = this->editDialog;
    this->editDialog = editDialog;
    if(this->oldDialog && this->oldDialog != this->editDialog)
    {
        this->oldDialog->hide();
    }
    this->editDialog->setParent(this);
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "Single", "bool")!="true")
    {
        this->editDialog->setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
    };
    this->centerEditDialog();
    this->editDialog->pageUpdateSignal();
    this->editDialog->show();
    this->editDialog->raise();
}

void floorBoard::centerEditDialog()
{
    if(!this->editDialog)
    {
        return;
    }

    Preferences *preferences = Preferences::Instance();
    bool ok;
    const double ratio = preferences->getPreferences("Window", "Scale", "ratio").toDouble(&ok);

    if(preferences->getPreferences("Window", "Single", "bool")=="true")
    {
        const int minLeft = this->displayPos.x() + qRound(6*ratio);
        int preferredLeft = minLeft;
        if(this->stompBoxes.size() > 0 && this->stompBoxes.at(0))
        {
            preferredLeft = this->stompBoxes.at(0)->x() - qRound(2*ratio);
        }
        int maxLeft = qRound(this->floorSize.width()*ratio) - this->editDialog->width() - qRound(8*ratio);
        if(maxLeft < minLeft)
        {
            maxLeft = minLeft;
        }
        int x = qBound(minLeft, preferredLeft, maxLeft);
        const int extraYOffset = qRound(300*ratio);
        int y = this->pos.y() + ((((this->floorSize.height()*ratio) + extraYOffset) - this->editDialog->height()) / 2);
        // Keep the edit panel below the 4-row signal-flow area to avoid top-border overlap.
        const int flowBottom = qRound((260.0 + (147.0 / 2.4)) * ratio);
        const int minTop = this->pos.y() + flowBottom + qRound(18*ratio);
        if(y < minTop)
        {
            y = minTop;
        }
        this->editDialog->move(x, y);
    };
}

void floorBoard::exportDebugScreens(const QString &dirPath)
{
    QDir dir;
    if(!dir.mkpath(dirPath))
    {
        return;
    }

    auto flushUi = []()
    {
        for(int i = 0; i < 4; ++i)
        {
            qApp->processEvents(QEventLoop::AllEvents, 50);
        }
    };

    auto saveWidget = [&](QWidget *widget, const QString &fileName)
    {
        if(widget)
        {
            widget->grab().save(QDir(dirPath).filePath(fileName));
        }
    };

    auto captureSelection = [&](int stompId, const QString &prefix, const QString &title)
    {
        if(stompId < 0 || stompId >= this->stompBoxes.size() || !this->stompBoxes.at(stompId))
        {
            return;
        }

        editWindow *dialog = this->stompBoxes.at(stompId)->editDetails();
        if(!dialog)
        {
            return;
        }

        dialog->setWindow(title);
        setEditDialog(dialog);
        flushUi();

        saveWidget(this->window(), prefix + "-window.png");
        saveWidget(dialog, prefix + "-editor.png");
    };

    flushUi();
    saveWidget(this->window(), "00-home-window.png");
    captureSelection(1, "01-inst1", "INSTRUMENT 1");
    captureSelection(2, "02-inst2", "INSTRUMENT 2");
    captureSelection(3, "03-inst3", "INSTRUMENT 3");
    captureSelection(0, "04-normal", "NORMAL PICKUP");
    captureSelection(8, "05-fx1", "FX1");

    QWidget probeWindow;
    probeWindow.setWindowTitle("PARA EQ PROBE");
    probeWindow.setStyleSheet("background-color: #050708;");
    QVBoxLayout *probeLayout = new QVBoxLayout(&probeWindow);
    probeLayout->setContentsMargins(12, 12, 12, 12);
    probeLayout->setSpacing(0);
    customControlParaEQ *probeEq = new customControlParaEQ(&probeWindow, "10", "00", "3A", "01");
    probeLayout->addWidget(probeEq);
    probeWindow.adjustSize();
    probeWindow.show();
    flushUi();
    saveWidget(&probeWindow, "06-paraeq-probe.png");
    probeWindow.hide();

    QWidget tunerProbe;
    tunerProbe.setWindowTitle("TUNER PROBE");
    tunerProbe.setStyleSheet("background-color: #050708;");
    QVBoxLayout *tunerLayout = new QVBoxLayout(&tunerProbe);
    tunerLayout->setContentsMargins(12, 12, 12, 12);
    tunerLayout->setSpacing(6);
    customControlVU *tuner = new customControlVU(&tunerProbe, "mono_tuner");
    tunerLayout->addWidget(tuner, 0, Qt::AlignCenter);
    tunerProbe.adjustSize();
    tunerProbe.show();
    flushUi();
    saveWidget(&tunerProbe, "07-tuner-probe.png");
    tunerProbe.hide();
}

void floorBoard::initMenuPages()
{
    QVector<menuPage *> initMenuPages(4);
    this->menuPages = initMenuPages.toList();

    // MENU_PAGES
    menuPage *tuner = new menuPage_tuner(this);
    tuner->setId(10);

    menuPage *setup = new menuPage_setup(this);
    setup->setId(11);

    menuPage *pdl = new menuPage_pdl(this);
    pdl->setId(12);

    menuPage *master = new menuPage_master(this);
    master->setId(13);

    menuPage *system = new menuPage_system(this);
    system->setId(14);

    menuPage *assign1 = new menuPage_assign1(this);
    assign1->setId(15);
}

void floorBoard::menuButtonSignal()
{
    if(this->oldDialog && this->oldDialog != this->editDialog)
    {
        this->oldDialog->hide();
    }
    if(this->editDialog)
    {
        this->editDialog->show();
        this->editDialog->raise();
    }
}

void floorBoard::structure(bool value)
{
    Q_UNUSED(value);
}

void floorBoard::scheduleStructureRefresh()
{
    if(!this->structureRefreshTimer)
    {
        updateStompBoxes();
        return;
    }

    this->structureRefreshTimer->start(120);
}

void floorBoard::schedulePatchStructureRefresh()
{
    if(!this->structureRefreshTimer)
    {
        updateStompBoxes();
        return;
    }

    this->structureRefreshNeedsNormalization = true;
    // Patch loads arrive as a burst of SysEx updates. Refresh the upper flow only
    // after that burst settles so the signal chain does not re-layout progressively.
    this->structureRefreshTimer->start(180);
}

void floorBoard::scheduleChainStructureRefresh()
{
    if(!this->structureRefreshTimer)
    {
        updateStompBoxes();
        return;
    }

    this->structureRefreshNeedsNormalization = true;
    this->structureRefreshTimer->start(80);
}

void floorBoard::applyStructureRefresh()
{
    if(this->structureRefreshNeedsNormalization)
    {
        this->structureRefreshNeedsNormalization = false;
        updateDrop();
        return;
    }

    updateStompBoxes();
}

void floorBoard::updateStompBoxes()
{
    QString hex1 = "00";
    QString sys1 = "04";
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){hex1 = "02"; sys1 = "0B"; };

    SysxIO *sysxIO = SysxIO::Instance();
    QList<QString> fxChain = sysxIO->getFileSource("10", hex1, "12", "void");
    if(fxChain.size() < (sysxDataOffset + 69 + 34))
    {
        return;
    }

    MidiTable *midiTable = MidiTable::Instance();
    QList<QString> stompOrder;
    //QString debug;
    int incr=28;
    for(int i= sysxDataOffset+69; i< (sysxDataOffset+69+4); i++ ) // inst input 4 items
    {
        stompOrder.append( midiTable->getMidiMap("10", hex1, "12", "45", fxChain.at(i)).desc );
        //debug.append( midiTable->getMidiMap("10", hex1, "12", "45", fxChain.at(i)).desc + " " );
    };
    for(int i= sysxDataOffset+73; i< (sysxDataOffset+73+30); i++ ) // chain data offset + 30 effect chain items
    {
        QString stomp = midiTable->getMidiMap("10", hex1, "12", "49", fxChain.at(i)).desc;
        if(stomp=="28"){stomp = QString::number(incr, 10); incr++;};  // if items are hex "18" (split type), increment list items.

        stompOrder.append(stomp);
        //debug.append( stomp + " ");
    };
    const QList<QString> normalizedOrder = normalizeInputOrder(stompOrder);
    const QString structureSignature =
        normalizedOrder.join(",") +
        "|" + QString::number(sysxIO->getSourceValue("00", sys1, "00", "34")) +
        "|" + QString::number(sysxIO->getSourceValue("00", sys1, "00", "35"));

    if(!this->fxPos.isEmpty() && !this->fx.isEmpty() && this->_lastStructureSignature == structureSignature)
    {
        return;
    }

    this->_lastStructureSignature = structureSignature;
    setStomps(normalizedOrder);
    update_structure();
    emit master_pos_Signal(master_pos);
    //std::string st = QString("Stomp order = "+debug).toStdString();
    //std::cerr << st << std::endl;
}

void floorBoard::update_structure()
{
    // ********* calculate stompbox chain position and x/y layout ************
    SysxIO *sysxIO = SysxIO::Instance();
    Preferences *preferences = Preferences::Instance();
    bool ok;
    const double ratio = preferences->getPreferences("Window", "Scale", "ratio").toDouble(&ok);
    QString hex1 = "00";
    QString sys1 = "04"; // guitar mode address.
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){hex1 = "02"; sys1 = "0B";};  // bass mode address.

    QList<QString> fxChain = sysxIO->getFileSource("10", hex1, "12", "void");            // read address level data block.
    int chainLength = 34;
    if(fxChain.size() < (sysxDataOffset + 69 + 34))
    {
        return;
    }

    int main_link = sysxIO->getSourceValue("00", sys1, "00", "34");                   // main stereo  link, one stereo or two mono blocks.
    int sub_link = sysxIO->getSourceValue("00", sys1, "00", "35");                   // sub stereo  link, one stereo or two mono blocks.
    if(main_link>0)
    {
        const int mainRightIndex = fxChain.indexOf("17", 85);
        if(mainRightIndex >= 0)
        {
            fxChain.removeAt(mainRightIndex);
            chainLength--;
        }
    }
    if(sub_link>0)
    {
        const int subRightIndex = fxChain.indexOf("15", 85);
        if(subRightIndex >= 0)
        {
            fxChain.removeAt(subRightIndex);
            chainLength--;
        }
    }
    dual_channel = sysxIO->getSourceValue("10", hex1, "12", "20");                // single or dual divider
    B_channel = sysxIO->getSourceValue("10", hex1, "12", "21");                  // divider B channel selected

    //QString debug;
    MidiTable *midiTable = MidiTable::Instance();

    int index1 = fxChain.indexOf("18", 85);                                 // from start of sysx header to end of 4th inst = 85 bytes.
    int index2 = fxChain.indexOf("18", index1+1);                          // each byte x18 marks split index points.

    int index3 = fxChain.indexOf("18", (index2+1));
    int index4 = fxChain.indexOf("18", (index3+1));

    int index5 = fxChain.indexOf("18", (index4+1));
    int index6 = fxChain.indexOf("18", (index5+1));

    int index7 = fxChain.indexOf("11", (85));                       // start of DIV
    int index8 = fxChain.indexOf("12", (85));                      // start of MIX path A
    int index9 = fxChain.indexOf("13", (85));                     // start of MIX path B
    if(index1 < 0 || index2 < 0 || index3 < 0 || index4 < 0 || index5 < 0 || index6 < 0 ||
       index7 < 0 || index8 < 0 || index9 < 0)
    {
        return;
    }
    fxChain = fxChain.mid(85, chainLength);
    const int mixerStart = fxChain.indexOf("13");
    if(mixerStart < 0)
    {
        return;
    }
    fxChain = fxChain.mid(mixerStart, chainLength-mixerStart);

    index9 = index9 - index8 - 1;
    index8 = index8 - index7 - 1;
    index7 = index7 - index6 - 1;
    index6 = index6 - index5 - 1;
    index5 = index5 - index4 - 1;
    index4 = index4 - index3 - 1;
    index3 = index3 - index2 - 1;
    index2 = index2 - index1 - 1;
    index1 = index1 - 85;

    int bal1xpos = index1 + 1;                          // these xpos values to be indexed from fxChain to find longest string to offset balancers positions.
    if(index2 > index1){ bal1xpos = index2 + 1; };

    int bal2xpos = index3 + 1;
    if(index4 > index3){ bal2xpos = index4 + 1; };
    if((bal1xpos + index3) > bal2xpos){ bal2xpos = bal1xpos + index3 + 1; };

    int bal3xpos = index5 + 1;
    if(index6 > index5){ bal3xpos = index6 + 1; };
    if((bal2xpos + index5) > bal3xpos){ bal3xpos = bal2xpos + index5 + 1; };

    // LAYOUT FIX: tighter horizontal spacing + more vertical room for bigger blocks.
    // FX images are 96x147 px; at kFlowBlockScale=2.4, ratio≈1.20 → blocks 48×73.5 px.
    // flowStep 55*ratio = 66 px → 18 px gap between blocks (was 29 px at 60*ratio).
    // Row spacing 65*ratio = 78 px → 4.5 px breathing gap above each block row.
    // hiddenFlowY pushed to 360*ratio so hidden branch blocks stay off-screen.
    const int flowStep = 55*ratio;
    const int hiddenFlowY = 360*ratio;
    // No additional bias – rowCenters are derived from actual source-block heights.
    const int fxLineBiasY = 0;
    int lev1 = 65*ratio;
    int lev2 = 130*ratio;
    int lev3 = 195*ratio;
    int lev4 = 260*ratio;
    int y_axis = lev1;
    const int instStartX = qRound(15*ratio);
    // Always derive instWidth from the ON-image dimensions and scale constant (192px / 2.0).
    // Reading stompBoxes[0]->width() is unreliable: the constructor sizes the widget from the
    // default amp_off.png, not the actual INST image, so the widget width is wrong until the
    // first paintEvent runs — which may be after update_structure has already computed fxPos.
    const int instWidth = qRound((192.0*ratio)/2.0);
    const int instToFxGap = qRound(16*ratio);
    const int firstFlowX = instStartX + instWidth + instToFxGap;
    int x_axis = firstFlowX;
    int incr = 0;
    auto centerOffsetYForId = [this, ratio](int stompId, int fallback) -> int
    {
        if(stompId < 0 || stompId >= this->stompBoxes.size() || !this->stompBoxes.at(stompId))
        {
            return fallback;
        }
        return this->stompBoxes.at(stompId)->signalCenterYOffset(ratio);
    };
    auto centerOffsetXForId = [this, ratio](int stompId, int fallback) -> int
    {
        if(stompId < 0 || stompId >= this->stompBoxes.size() || !this->stompBoxes.at(stompId))
        {
            return fallback;
        }
        QRect bounds = this->stompBoxes.at(stompId)->signalBounds(ratio);
        bounds.translate(-this->stompBoxes.at(stompId)->pos());
        return bounds.isValid() ? bounds.center().x() : fallback;
    };
    const int branchStemExtension = qRound(10 * ratio);
    auto flowRectForId = [this, ratio](int stompId) -> QRect
    {
        if(stompId < 0 || stompId >= this->stompBoxes.size() || !this->stompBoxes.at(stompId))
        {
            return QRect();
        }
        return this->stompBoxes.at(stompId)->flowLayoutBounds(ratio);
    };
    auto topOffsetYForId = [this, ratio, branchStemExtension](int stompId, int fallback) -> int
    {
        if(stompId < 0 || stompId >= this->stompBoxes.size() || !this->stompBoxes.at(stompId))
        {
            return fallback;
        }
        Q_UNUSED(fallback);
        Q_UNUSED(ratio);
        return -branchStemExtension;
    };
    auto bottomOffsetYForId = [this, ratio, branchStemExtension](int stompId, int fallback) -> int
    {
        if(stompId < 0 || stompId >= this->stompBoxes.size() || !this->stompBoxes.at(stompId))
        {
            return fallback;
        }
        const QRect flowRect = this->stompBoxes.at(stompId)->flowLayoutBounds(ratio);
        return flowRect.height() + branchStemExtension;
    };
    // ── All per-row centre helpers must be defined BEFORE alignedRowTop so the lambda
    //    can correctly centre blocks on every row type, including BAL output rows.
    const int flowMidY       = centerOffsetYForId(8, qRound(20*ratio));
    const int flowMidX       = centerOffsetXForId(8, qRound(20*ratio));
    const int inst1MidY      = centerOffsetYForId(1, flowMidY);
    const int inst2MidY      = centerOffsetYForId(2, flowMidY);
    const int inst3MidY      = centerOffsetYForId(3, flowMidY);
    const int normalMidY     = centerOffsetYForId(0, flowMidY);
    const int balMidYOffset  = centerOffsetYForId(29, flowMidY);
    const int dividerMidY    = centerOffsetYForId(21, flowMidY);
    const int dividerTopY    = topOffsetYForId(21, qRound(1*ratio));
    const int dividerBottomY = bottomOffsetYForId(21, qRound(36*ratio));
    const int mixerMidY      = centerOffsetYForId(23, flowMidY);
    const int mixerTopY      = topOffsetYForId(23, qRound(1*ratio));
    const int mixerBottomY   = bottomOffsetYForId(23, qRound(36*ratio));
    const int masterMidY     = centerOffsetYForId(34, flowMidY);
    int balancerHalfHeight = qRound((96.0*ratio)/(2.0*2.4)); // fallback for 96x96 balancer art at flow scale.
    if(this->stompBoxes.size() > 29 && this->stompBoxes.at(29))
        balancerHalfHeight = flowRectForId(29).height() / 2;
    // BAL1 sits centred between the INST1 and INST2 signal lines.
    const int bal1MidY      = qRound(((lev1 + inst1MidY) + (lev2 + inst2MidY)) / 2.0);
    const int bal1TopY      = bal1MidY - balancerHalfHeight;
    const int balTopYOffset = balMidYOffset - balancerHalfHeight;
    // Row centres – four INST rows + three BAL output rows.
    const int rowCenter1    = lev1 + inst1MidY;
    const int rowCenter2    = lev2 + inst2MidY;
    const int rowCenter3    = lev3 + inst3MidY;
    const int rowCenter4    = lev4 + normalMidY;
    const int rowCenterBal1 = bal1MidY;
    // BAL2/3 centres are the actual midpoints of their input signal lines, not hardcoded lev2/3.
    // BAL2 collects BAL1 output (bal1MidY) and INST3 (rowCenter3); its output is their midpoint.
    // BAL3 collects BAL2 output (rowCenterBal2) and NORMAL (rowCenter4); its output is their midpoint.
    const int rowCenterBal2 = qRound((bal1MidY + rowCenter3) / 2.0);
    const int rowCenterBal3 = qRound((rowCenterBal2 + rowCenter4) / 2.0);
    // topForRowCenter: return the widget top-left Y that centres stompId on a given signal-line Y.
    // Using the row centre directly avoids ambiguity when balTopYOffset==0 (which would make the
    // BAL output row top equal to the INST row top, breaking row-top comparisons).
    auto topForRowCenter = [&](int stompId, int rowCenter) -> int
    {
        if(stompId < 0 || stompId >= this->stompBoxes.size() || !this->stompBoxes.at(stompId))
            return rowCenter;
        const int ownCenterOffset = centerOffsetYForId(stompId, this->stompBoxes.at(stompId)->height() / 2);
        return rowCenter - ownCenterOffset;
    };
    auto leftOffsetXForId = [this](int stompId, int fallback) -> int
    {
        if(stompId < 0 || stompId >= this->stompBoxes.size() || !this->stompBoxes.at(stompId))
            return fallback;
        return 0;
    };
    // Use the right edge of MST so the signal line passes *through* S LR, M LR, and MST
    // rather than stopping at their left edges.
    int masterRightX = 0;
    if(34 < this->stompBoxes.size() && this->stompBoxes.at(34))
        masterRightX = this->stompBoxes.at(34)->width();
    const int masterLeftX = masterRightX;

    QList<QPoint> newFxPos;
    fxChain = sysxIO->getFileSource("10", hex1, "12", "void");
    _last_chain = fxChain.mid(sysxDataOffset+69, sysxDataOffset+69+34);
    if(_last_chain.size() < 34)
    {
        return;
    }
    int offset1 = _last_chain.indexOf("12", 0) - _last_chain.indexOf("11", 0);  // count of items inside a/b
    int offset2 = _last_chain.indexOf("13", 0) - _last_chain.indexOf("12", 0);  // count of items inside a/b
    int ab_count = offset1;
    if(offset2>offset1){ab_count=offset2;};
    int ab_y_axis = 0;
    int ab_start = 0;
    int ab_end = 0;
    // rowCenterCurrent tracks the signal-line Y for the current row.
    // Using the centre directly avoids the ambiguity that arises when balTopYOffset==0
    // (which makes the BAL output row top equal to the INST row top).
    int rowCenterCurrent = rowCenter1;
    int ab_rowCenter = rowCenter1; // mirrors ab_y_axis: saves/restores rowCenterCurrent at DIV/MIX

    for(uint i=0;i<34;i++)
    {
        uint stomp = QString(midiTable->getMidiMap("10", hex1, "12", "49", fxChain.at(i+sysxDataOffset+69)).desc).toInt(&ok, 10);
        if(i<4){stomp = stomp-4;};
        if(stomp==21){ab_y_axis = y_axis; ab_rowCenter = rowCenterCurrent; }; // record divider row
        if(stomp==23){y_axis = ab_y_axis; x_axis=ab_end+((ab_count-1)*flowStep); rowCenterCurrent = ab_rowCenter;}; // mixer is same row as divider.

        if     (i==0)                     { newFxPos.append(QPoint(offset+(15*ratio), lev1)); }  // instrument 1 – fixed start positions
        else if(i==1)                     { newFxPos.append(QPoint(offset+(15*ratio), lev2)); }  // instrument 2
        else if(i==2)                     { newFxPos.append(QPoint(offset+(15*ratio), lev3)); }  // instrument 3
        else if(i==3)                     { newFxPos.append(QPoint(offset+(15*ratio), lev4));    x_axis=firstFlowX; y_axis=lev1; rowCenterCurrent=rowCenter1; }  // normal input
        else if(stomp>27 && incr==0)      { newFxPos.append(QPoint(offset+ 0, hiddenFlowY));     x_axis=firstFlowX-flowStep; y_axis=lev2; rowCenterCurrent=rowCenter2; }  // fill lev2
        else if(stomp>27 && incr==1)      { newFxPos.append(QPoint(offset+((47*ratio)+(bal1xpos*flowStep)), bal1TopY)); x_axis=flowStep+(bal1xpos*flowStep); y_axis=bal1TopY; rowCenterCurrent=rowCenterBal1; }  // Bal1 out
        else if(stomp>27 && incr==2)      { newFxPos.append(QPoint(offset+ 0, hiddenFlowY));     x_axis=firstFlowX-flowStep; y_axis=lev3; rowCenterCurrent=rowCenter3; }  // fill lev3
        else if(stomp>27 && incr==3)      { newFxPos.append(QPoint(offset+((47*ratio)+(bal2xpos*flowStep)), rowCenterBal2 - balancerHalfHeight)); x_axis=flowStep+(bal2xpos*flowStep); y_axis=rowCenterBal2 - balancerHalfHeight; rowCenterCurrent=rowCenterBal2; }  // Bal2 out
        else if(stomp>27 && incr==4)      { newFxPos.append(QPoint(offset+ 0, hiddenFlowY));     x_axis=firstFlowX-flowStep; y_axis=lev4; rowCenterCurrent=rowCenter4; }  // fill lev4
        else if(stomp>27 && incr==5)      { newFxPos.append(QPoint(offset+((40*ratio)+(bal3xpos*flowStep)), rowCenterBal3 - balancerHalfHeight)); x_axis=flowStep+(bal3xpos*flowStep); y_axis=rowCenterBal3 - balancerHalfHeight; rowCenterCurrent=rowCenterBal3; }  // Bal3 out
        else
        {
            const int alignedY = topForRowCenter(stomp, rowCenterCurrent);
            const bool isVisibleFxBlock = (stomp >= 4 && stomp <= 27 && stomp != 22);
            newFxPos.append(QPoint(offset+(x_axis), alignedY + (isVisibleFxBlock ? fxLineBiasY : 0)));
        };

        if(main_link==1 && stomp==27){goto skip;};  // if main is linked as a singled unit
        if(sub_link==1 && stomp==25){goto skip;};   // if sub is linked as a single unit.
        if(stomp==22){goto skip;}                   // 22 is A/B branch marker only.

        if(i>3){ x_axis = x_axis+flowStep;}; // if not an instrument input, blocks are incremented to the right.
    skip:
        if(stomp>27){ incr++; }; // if one of 6 branch mixers (28) then increment for the next x-axis level.

        if(stomp==21){y_axis=y_axis-(20*ratio); ab_start = x_axis; if(x_axis>ab_end){ab_end=x_axis;};};
        if(stomp==22){y_axis=y_axis+(40*ratio); x_axis = ab_start; if(x_axis>ab_end){ab_end=x_axis;};};
    };
    this->master_pos = QPoint(offset+(x_axis), topForRowCenter(34, rowCenterCurrent));

    this->fxPos.clear();
    this->fxPos = newFxPos;

    // Reposition all chain stompbox widgets to the newly computed (centred) positions.
    // setStomps runs *before* update_structure and uses the previous fxPos; this pass
    // applies the corrected Y values immediately so every widget sits on its row line.
    for(int order = 0; order < qMin(this->fxPos.size(), this->fx.size()); ++order)
    {
        const int sbIdx = this->fx.at(order);
        if(sbIdx >= 0 && sbIdx < this->stompBoxes.size() && this->stompBoxes.at(sbIdx))
            this->stompBoxes.at(sbIdx)->setPos(this->fxPos.at(order));
    }
    // Master (stompBoxes index 34) is outside fxPos; position it directly.
    if(34 < this->stompBoxes.size() && this->stompBoxes.at(34))
        this->stompBoxes.at(34)->setPos(this->master_pos);

    polygon.clear();
    if(index1>index2){index2=index1;}else if(index2>index1){index1=index2;};
    const int dividerChainIndex = _last_chain.indexOf("11", 0);
    const int mixerChainIndex = _last_chain.indexOf("13", 0);
    if(dividerChainIndex < 0 || mixerChainIndex < 0 ||
       dividerChainIndex >= fxPos.size() || mixerChainIndex >= fxPos.size())
    {
        return;
    }
    polygon.append( QPoint(offset+(60*ratio),                      lev1+inst1MidY));  //index1 left    0
    polygon.append( QPoint(offset+(105*ratio)+(index1*flowStep),   lev1+inst1MidY));  //index1 right   1
    polygon.append( QPoint(offset+(60*ratio),                      lev2+inst2MidY));  //index2 left    2
    polygon.append( QPoint(offset+(105*ratio)+(index2*flowStep),   lev2+inst2MidY));  //index2 right   3
    polygon.append( QPoint(offset+(60*ratio)+(bal1xpos*flowStep),  bal1MidY));         //index3 left    4
    polygon.append( QPoint(offset+(65*ratio)+(bal2xpos*flowStep),  bal1MidY));         //index3 right   5
    polygon.append( QPoint(offset+(78*ratio),                      lev3+inst3MidY));  //index4 left    6
    polygon.append( QPoint(offset+(65*ratio)+(bal2xpos*flowStep),  lev3+inst3MidY));  //index4 right   7
    polygon.append( QPoint(offset+(58*ratio)+(bal2xpos*flowStep),  rowCenterBal2));    //index5 left    8  (BAL2 out; corrected by placeBalancerOnRiser)
    polygon.append( QPoint(offset+(58*ratio)+(bal3xpos*flowStep),  rowCenterBal2));    //index5 right   9  (BAL3 A in = BAL2 out level)
    polygon.append( QPoint(offset+(78*ratio),                      lev4+normalMidY)); //index6 left    10
    polygon.append( QPoint(offset+(58*ratio)+(bal3xpos*flowStep),  lev4+normalMidY)); //index6 right   11
    polygon.append( QPoint(offset+(58*ratio)+(bal3xpos*flowStep),  rowCenterBal3));    //index7 left    12 (BAL3 out; corrected by placeBalancerOnRiser)
    polygon.append(fxPos.at(dividerChainIndex)+QPoint(flowMidX, dividerMidY));   // at divider    13
    polygon.append(fxPos.at(dividerChainIndex)+QPoint(flowMidX, dividerTopY));   // above divider 14
    polygon.append(fxPos.at(mixerChainIndex)+QPoint(flowMidX, mixerTopY));       // above mixer   15
    polygon.append(fxPos.at(dividerChainIndex)+QPoint(flowMidX, dividerBottomY));// below divider 16
    polygon.append(fxPos.at(mixerChainIndex)+QPoint(flowMidX, mixerBottomY));    // below mixer   17
    polygon.append(fxPos.at(mixerChainIndex)+QPoint(flowMidX, mixerMidY));       // at mixer      18
    polygon.append(this->master_pos+QPoint(masterLeftX, masterMidY));                        // left-mid of MST 19

    // Keep branch riser lines visually clear from the right edge of the
    // source-FX squares when the user reorders blocks.
    const int minBranchGap = qRound(50 * ratio);
    auto rightEdgeForId = [flowRectForId](int stompId) -> int
    {
        const QRect flowRect = flowRectForId(stompId);
        if(!flowRect.isValid())
        {
            return -1;
        }
        return flowRect.right();
    };
    auto centerXForId = [flowRectForId](int stompId) -> int
    {
        const QRect flowRect = flowRectForId(stompId);
        if(!flowRect.isValid())
        {
            return -1;
        }
        return flowRect.center().x();
    };

    const int minBalancerOrderGap = qRound(24 * ratio);

    if(polygon.size() >= 12)
    {
        const int branch1SourceRight = qMax(rightEdgeForId(8), rightEdgeForId(9));   // FX1/FX2
        if(branch1SourceRight >= 0)
        {
            const int bal1CenterX = centerXForId(29); // BAL1
            int branch1X = qMax(branch1SourceRight + minBranchGap, qMax(polygon.at(1).x(), polygon.at(3).x()));
            if(bal1CenterX >= 0)
            {
                branch1X = qMax(branch1X, bal1CenterX);
            }
            branch1X = qMin(branch1X, polygon.at(5).x() - minBalancerOrderGap);
            polygon[1].setX(branch1X);
            polygon[3].setX(branch1X);
        }

        const int branch2SourceRight = rightEdgeForId(10); // FX3
        if(branch2SourceRight >= 0)
        {
            const int bal2CenterX = centerXForId(31); // BAL2
            int branch2X = qMax(branch2SourceRight + minBranchGap, qMax(polygon.at(5).x(), polygon.at(7).x()));
            if(bal2CenterX >= 0)
            {
                branch2X = qMax(branch2X, bal2CenterX);
            }
            branch2X = qBound(polygon.at(4).x() + minBalancerOrderGap,
                              branch2X,
                              polygon.at(9).x() - minBalancerOrderGap);
            polygon[5].setX(branch2X);
            polygon[7].setX(branch2X);
        }
    }

    // BAL nodes must stay on branch risers and keep the same side clearance
    // as the standard gap between adjacent square FX blocks.
    if(polygon.size() >= 14)
    {
        const QRect referenceFxRect = flowRectForId(8);
        const int standardFxGap = referenceFxRect.isValid()
            ? qMax(qRound(8 * ratio), flowStep - referenceFxRect.width())
            : qRound(12 * ratio);
        const int minFxSideGap = standardFxGap;
        auto placeBalancerOnRiser = [this, ratio, hiddenFlowY, minFxSideGap](int stompId, int riserTopIdx, int riserBottomIdx, int centerPointIdx, int minCenterXBound, int maxCenterXBound) -> bool
        {
            if(stompId < 0 || stompId >= this->stompBoxes.size() || !this->stompBoxes.at(stompId))
            {
                return false;
            }

            stompBox *bal = this->stompBoxes.at(stompId);
            const QRect balRect = bal->flowLayoutBounds(ratio);
            const int halfW = balRect.width() / 2;
            const int halfH = balRect.height() / 2;
            const int centerY = (this->polygon.at(riserTopIdx).y() + this->polygon.at(riserBottomIdx).y()) / 2;
            int centerX = this->polygon.at(riserTopIdx).x();

            const int balTop = centerY - halfH;
            const int balBottom = centerY + halfH;
            int minCenterX = INT_MIN / 4;
            int maxCenterX = INT_MAX / 4;

            for(int otherId = 4; otherId < this->stompBoxes.size(); ++otherId)
            {
                if(otherId == stompId || otherId == 29 || otherId == 31 || otherId == 33)
                {
                    continue;
                }
                if(!this->stompBoxes.at(otherId))
                {
                    continue;
                }

                stompBox *other = this->stompBoxes.at(otherId);
                if(other->y() >= hiddenFlowY - 8)
                {
                    continue;
                }

                const QRect r = other->flowLayoutBounds(ratio);
                if(r.bottom() < balTop || r.top() > balBottom)
                {
                    continue;
                }

                if(r.center().x() <= centerX)
                {
                    minCenterX = qMax(minCenterX, r.right() + minFxSideGap + halfW);
                }
                else
                {
                    maxCenterX = qMin(maxCenterX, r.left() - minFxSideGap - halfW);
                }
            }

            minCenterX = qMax(minCenterX, minCenterXBound);
            maxCenterX = qMin(maxCenterX, maxCenterXBound);

            if(minCenterX <= maxCenterX)
            {
                centerX = qBound(minCenterX, centerX, maxCenterX);
            }
            else if(centerX < minCenterX)
            {
                centerX = minCenterX;
            }
            else if(centerX > maxCenterX)
            {
                centerX = maxCenterX;
            }

            bal->setPos(QPoint(centerX - halfW, centerY - halfH));
            this->polygon[riserTopIdx].setX(centerX);
            this->polygon[riserBottomIdx].setX(centerX);
            this->polygon[centerPointIdx].setX(centerX);
            this->polygon[centerPointIdx].setY(centerY);
            return true;
        };

        placeBalancerOnRiser(29, 1, 3, 4, INT_MIN / 4, this->polygon.at(5).x() - minBalancerOrderGap);
        placeBalancerOnRiser(31, 5, 7, 8, this->polygon.at(4).x() + minBalancerOrderGap, this->polygon.at(9).x() - minBalancerOrderGap);
        placeBalancerOnRiser(33, 9, 11, 12, this->polygon.at(8).x() + minBalancerOrderGap, INT_MAX / 4);
    }

    update();
}

void floorBoard::updateDrop()
{
    /**************** Read chain block, check Main and Sub blocks are single stereo, or two mono blocks.             *****************
     **************** stereo requires L+R blocks consequitive. write chain block, and then update stompBox layout.   *****************/

    SysxIO *sysxIO = SysxIO::Instance();
    MidiTable *midiTable = MidiTable::Instance();
    QList<QString> hexData;
    //QString deta;
    QString hex1 = "00";
    bool ok;
    Preferences *preferences = Preferences::Instance();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){hex1 = "02"; };

    QList<QString> fxChain = sysxIO->getFileSource("10", hex1, "12", "void"); // read chain address data block.

    for(unsigned int i=0;i<34;i++)
    {
        int b = QString(midiTable->getMidiMap("10", hex1, "12", "49", fxChain.at(i+sysxDataOffset+69)).desc).toInt(&ok, 10)-4;
        QString byte = QString::number(b, 16).toUpper();
        if(byte.size()<2){byte.prepend("0");};
        hexData.append(byte);                     // read the 34 bytes of chain data,
    };
    _lastDrop = hexData;
    QString sys1 = "04";
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){sys1 = "0B";};

    int main_link = sysxIO->getSourceValue("00", sys1, "00", "34");  // main stereo  link
    int sub_link = sysxIO->getSourceValue("00", sys1, "00", "35");  // sub stereo  link

    auto moveLinkedBlock = [&hexData](const QString &leftId, const QString &rightId)
    {
        const int rightIndex = hexData.indexOf(rightId);
        if(rightIndex < 0)
        {
            return;
        }
        hexData.removeAt(rightIndex);
        const int leftIndex = hexData.indexOf(leftId);
        if(leftIndex < 0)
        {
            return;
        }
        hexData.insert(leftIndex + 1, rightId);
    };
    if(main_link==1){moveLinkedBlock("16", "17");}; //if main linked, move together
    if(sub_link==1){moveLinkedBlock("14", "15");}; //if sub linked, move together

    //deta.clear();
    //for(int k=0; k<34; k++)
    //{deta.append(hexData.at(k)); deta.append(QString(" "));};
    //std::string st = QString("  Drop data = "+ deta).toStdString();
    //std::cerr << st << std::endl;

    if(hexData != _lastDrop)
    {
        sysxIO->setFileSource("10", hex1, "12", "45", hexData);
    };
    updateStompBoxes();
}
