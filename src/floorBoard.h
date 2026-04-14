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

#ifndef FLOORBOARD_H
#define FLOORBOARD_H

#include <QWidget>
#include <QPixmap>
#include <QMimeData>
#include <QTimer>
#include "stompBox.h"
#include "menuPage.h"
#include "editWindow.h"
#include "customStructure.h"
#include "stompbox_master.h"

class floorBoard : public QWidget
{
    Q_OBJECT

public:
    floorBoard(
            QWidget *parent = 0,
            QString imagePathFloor = ":/images/floor_blue.png",
            QString imagePathStompBG = ":/images/stompbg.png",
            QString imagePathInfoBar = ":/images/infobar.png",
            unsigned int marginStompBoxesTop = 20,
            unsigned int marginStompBoxesBottom = 140,
            unsigned int marginStompBoxesWidth = 15,
            unsigned int panelBarOffset = 10,
            unsigned int borderWidth = 3,
            QPoint pos = QPoint(0, 0));
    ~floorBoard();
    QPoint getStompPos(int id);
    void exportDebugScreens(const QString &dirPath);
    QPoint master_pos = QPoint(0, 0);

    // Accessors for diagnostic server chain-positions command
    const QList<stompBox*>& chainStompBoxes() const { return stompBoxes; }
    const QList<QString>& chainStompNames() const { return stompNames; }
    const QList<QPoint>& chainFxPos() const { return fxPos; }
    const QList<int>& chainFxOrder() const { return fx; }
    editWindow* currentEditDialog() const { return editDialog; }

    // Per-step layout trace for diagnostics
    struct ChainStep {
        int i;
        int stomp;
        int incr;
        int x_axis;
        int y_axis;
        int alignedX;
        int alignedY;
    };
    const QList<ChainStep>& chainTrace() const { return m_chainTrace; }

public slots:
    void setWidth(int dist);
    void setCollapse();
    void updateStompBoxes();
    void scheduleStructureRefresh();
    void schedulePatchStructureRefresh();
    void scheduleChainStructureRefresh();
    void applyStructureRefresh();
    void setEditDialog(editWindow* editDialog);
    void menuButtonSignal();
    void structure(bool value);
    void update_structure();
    void updateDrop();

signals:
    void valueChanged(QString fxName, QString valueName, QString value);
    void setDisplayPos(QPoint newpos);
    void setFloorPanelBarPos(QPoint newpos);
    void updateStompOffset(signed int offsetDif);
    void sizeChanged(QSize newsize, QSize oldSize);
    void setCollapseState(bool state);
    void resizeSignal(QRect newrect);
    void showDragBar(QPoint newpos);
    void hideDragBar();
    void updateSignal();
    void pageUpdateSignal();
    void amp_buttonSignal(bool value);
    void ns1_buttonSignal(bool value);
    void mod_buttonSignal(bool value);
    void pedal_buttonSignal(bool value);
    //void master_buttonSignal(bool value);
    void system_buttonSignal(bool value);
    void assign1_buttonSignal(bool value);
    void tuner_buttonSignal(bool value);
    void setup_buttonSignal(bool value);
    void pedal_statusSignal(bool value);
    void mod_statusSignal(bool value);
    void amp_statusSignal(bool value);
    void ns1_statusSignal(bool value);
    void system_statusSignal(bool value);
    void assign1_statusSignal(bool value);
    void notConnected();
    void interStompSignal(uint id, int value);
    void master_pos_Signal(QPoint master_pos);
    void updateNameSignal(QString name, int bank, int patch);

protected:
    void paintEvent(QPaintEvent *event);

private:
    void initSize(QSize floorSize);
    void setSize(QSize newSize);
    void setFloorBoard();
    void initSoundSource();
    void initStomps();
    void setStomps(QList<QString> stompOrder);
    QList<QString> normalizeInputOrder(const QList<QString> &stompOrder) const;
    void setStompPos(QString name, int order);
    void setStompPos(int index, int order);
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *event);
    void dropEvent(QDropEvent *event);
    bool decodeDraggedStompData(const QMimeData *mimeData, int &stompId, QPoint &stompPos, QSize &draggedSize, QPoint &topLeftOffset) const;
    int calculateDropIndex(const QPoint &dragPoint, int stompId) const;
    int indicatorXForIndex(int index, int stompId) const;
    void updateDropIndicator(int index, int stompId, int dragY);
    void clearDropIndicator();
    void initMenuPages();
    void centerEditDialog();
    QString imagePathFloor;
    QString imagePathInfoBar;
    QString imagePathStompBG;
    unsigned int offset;
    unsigned int structureWidth;
    unsigned int structureHeight;
    unsigned int panelBarOffset;
    unsigned int borderWidth;
    unsigned int floorHeight;
    QSize minSize;
    QSize maxSize;
    QSize l_floorSize;
    QSize floorSize;
    unsigned int marginStompBoxesTop;
    unsigned int marginStompBoxesBottom;
    unsigned int marginStompBoxesWidth;
    QSize stompSize;
    QPixmap baseImage;
    QPixmap image;
    QPoint pos;
    QPoint displayPos;
    QPoint liberainPos;
    QPoint panelBarPos;
    QList<QPoint> fxPos;
    QList<int> fx;
    QList<int> snd;
    bool colapseState;
    QList<menuPage*> menuPages;
    QList<stompBox*> soundSources;
    QList<stompBox*> stompBoxes;
    QList<QString> stompNames;
    editWindow* editDialog;
    editWindow* oldDialog;
    int pageOffset;
    customStructure* structure_2;
    bool structure_state;
    QList<QPoint> polygon;
    int dual_channel;
    int B_channel;
    QList<QString> _lastDrop;
    QList<QString> _last_chain;
    QString _lastStructureSignature;
    QTimer *structureRefreshTimer;
    bool structureRefreshNeedsNormalization = false;
    QList<ChainStep> m_chainTrace;
    bool dropIndicatorVisible = false;
    int dropIndicatorX = 0;
    int dropIndicatorTopY = 0;
    int dropIndicatorBottomY = 0;
    int dropIndicatorIndex = -1;
    //stompBox* master;
};

#endif // FLOORBOARD_H
