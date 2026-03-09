/****************************************************************************
**
** Copyright (C) 2007~2025 Colin Willcocks.
** Copyright (C) 2005~2007 Uco Mesdag.
** All rights reserved.
** This file is part of "VG-99 FloorBoard".
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

#include "stompBox.h"
#include "MidiTable.h"
#include "SysxIO.h"
#include "Preferences.h"
#include "appservices.h"

namespace {
// Visual scale for the effect-flow blocks (smaller value = bigger blocks).
// LAYOUT FIX: FX images are 96x147 px. At ratio≈1.20:
//   scale 2.4 → 48×73.5 px blocks, row spacing 65*ratio=78 px → 4.5 px gap ✓
//   scale 2.7 → 42.7×65.3 px blocks, row spacing 60*ratio=72 px → 6.7 px gap
// Using 2.4 gives bigger, easier-to-read blocks with tighter horizontal spacing.
// flowStep in floorBoard.cpp is 55*ratio (66 px) → 18 px h-gap between blocks ✓
const double kFlowBlockScale = 2.4;
// Source input blocks (NORMAL + INST 1-3, id 0-3) use the same scale as FX blocks
// so they render at the same height.  INST images are 192×96 (wider than 96×96 FX),
// so at scale 2.4 they become 80×40 — same height as FX blocks, just wider.
const double kInstBlockScale = 2.4;
// Keep all descriptor captions directly under their blocks so they stay aligned
// with the signal chain and never fall behind the lower settings panes.
const int kUniformLabelOffsetPx = 10;
// Input source descriptors (NORMAL + INST 1-3) sit close under the block.
const int kInputLabelOffsetPx = 10;

bool hidesFlowDescriptionLabel(unsigned int id)
{
    return id == 7 || id == 11 || id == 12 || id == 14 || id == 15 ||
           id == 18 || id == 19 || id == 20 || id == 21 || id == 23 || id > 27;
}

}

stompBox::stompBox(QWidget *parent, uint id, QString imagePathOn, QString imagePathOff, QString imagePathHover, QString imagePathSelected, QPoint stompPos)
    : QWidget(parent)
{
    Preferences *preferences = &AppServices::instance().preferences();
    bool ok;
    const double ratio = preferences->getPreferences("Window", "Scale", "ratio").toDouble(&ok);
    const double fratio = preferences->getPreferences("Window", "Font", "ratio").toDouble(&ok);

    this->id = id;
    this->imagePathOn = imagePathOn;
    this->imagePathOff = imagePathOff;
    this->imagePathHover = imagePathHover;
    this->imagePathSelected = imagePathSelected;
    // Source tiles (id 0-3) render at kInstBlockScale so NORMAL and INST boxes match width.
    // Use the ON image for initial sizing so the widget dimensions match paintEvent from the start.
    // (subclasses call setOnImage/setOffImage after construction, so imagePathOn here is the default
    //  amp_on.png — but the geometry will be corrected on first paintEvent with the real ON image.)
    const double initScale = (id < 4) ? kInstBlockScale : kFlowBlockScale;
    QSize initImgSize = QPixmap(imagePathOn).size();
    if(id >= 4) { initImgSize.setHeight(qMin(initImgSize.height(), 96)); }
    this->stompSize = initImgSize*ratio/initScale;
    this->stompPos = stompPos;
    this->setFixedSize(stompSize);
    this->editDialog = new editWindow();
    this->setWhatsThis(tr("StompBox effect<br>a double mouse click will open the effect edit page."));
    timer = new QTimer(this);
    this->effectType = "effect";
    this->effectLabel = new QLabel(this); //new customControlLabel();
    this->effectLabel->setObjectName("stomplabel");
    QFont Cfont( "Arial", 9*fratio);
    Cfont.setStretch(85);
    this->effectLabel->setFont(Cfont);
    this->effectLabel->setText(this->effectType);
    if(this->id>4)
    {
        //this->effectLabel->setFixedSize(45*ratio,10*ratio);
    }
    else
    {
        //this->effectLabel->setFixedSize(60*ratio,10*ratio);
    };
    this->effectLabel->setAlignment(Qt::AlignCenter);
    this->effectLabel->setWordWrap(true);
    this->effectLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
    this->effectLabel->setProperty("fxId", QVariant::fromValue<int>(this->id));
    refreshOpaqueBounds();
    updateFlowLabelLayoutDefaults();

    QObject::connect(timer, &QTimer::timeout, this, &stompBox::blink);

    QObject::connect(this, SIGNAL(valueChanged(QString,QString,QString)), this->parent(), SIGNAL(valueChanged(QString,QString,QString)));

    QObject::connect(this->parent(), SIGNAL(updateStompOffset(signed int) ), this, SLOT(updatePos(signed int)));

    QObject::connect(this->parent(), SIGNAL(master_pos_Signal(QPoint)), this, SLOT(setPosMaster(QPoint)));

    QObject::connect(this->parent(), SIGNAL(updateSignal()), this, SLOT(updateSignal()));

    QObject::connect(this->editDialog, &editWindow::updateSignal, this, &stompBox::updateSignal);

    QObject::connect(this, &stompBox::dialogUpdateSignal, this->editDialog, &editWindow::dialogUpdateSignal);

    QObject::connect(this->parent(), SIGNAL(updateSignal()), this->editDialog, SIGNAL(dialogUpdateSignal()));

    QObject::connect(this->editDialog, &editWindow::updateSignal, this, &stompBox::setDisplayToFxName);

    QObject::connect(this, SIGNAL(sendStompSignal(uint,int)), this->parent(), SIGNAL(interStompSignal(uint,int)));
    QObject::connect(this->parent(), SIGNAL(interStompSignal(uint,int)), this, SLOT(interStomp(uint,int)));

    QObject::connect(this, SIGNAL(setEditDialog(editWindow*)), this->parent(), SLOT(setEditDialog(editWindow*)));

    //QObject::connect(this->editDialog, SIGNAL( pageUpdateSignal(QString) ), this, SLOT( pageUpdateSignal(QString) ));
    //QObject::connect(this, SIGNAL( pageUpdateSignal() ), this->parent(), SIGNAL( pageUpdateSignal() ));

    QObject::connect(this->parent(), SIGNAL(amp_statusSignal(bool)), this, SLOT(amp_ButtonSignal(bool)));
    QObject::connect(this, SIGNAL(amp_statusSignal(bool)), this->parent(), SIGNAL(amp_statusSignal(bool)));
    //QObject::connect(this->parent(), SIGNAL(amp_buttonSignal(bool)), this->parent(), SLOT(menuButtonSignal()));

    QObject::connect(this->parent(), SIGNAL(ns1_buttonSignal(bool)), this, SLOT(ns1_ButtonSignal(bool)));
    QObject::connect(this, SIGNAL(ns1_statusSignal(bool)), this->parent(), SIGNAL(ns1_statusSignal(bool)));
    QObject::connect(this, SIGNAL(ns1_statusSignal(bool)), this->parent(), SLOT(menuButtonSignal()));

    QObject::connect(this->parent(), SIGNAL(mod_buttonSignal(bool)), this, SLOT(mod_ButtonSignal(bool)));
    QObject::connect(this, SIGNAL(mod_statusSignal(bool)), this->parent(), SLOT(menuButtonSignal()));
    QObject::connect(this->parent(), SIGNAL(mod_buttonSignal(bool)), this->parent(), SLOT(menuButtonSignal()));

    QObject::connect(this, SIGNAL(update_structure()), this->parent(), SLOT(update_structure()));
    QObject::connect(this, SIGNAL(updateStompBoxes()), this->parent(), SLOT(updateStompBoxes()));
    QObject::connect(this, SIGNAL(updateDrop()), this->parent(), SLOT(updateDrop()));

    setMouseTracking(true);
    sel_count=0;
    if(id==22){ this->hide(); };
}

void stompBox::refreshOpaqueBounds()
{
    opaqueBoundsValid = false;
    const QImage image(imagePathOff);
    if(image.isNull())
    {
        return;
    }

    int left = image.width();
    int right = -1;
    int top = image.height();
    int bottom = -1;

    for(int y = 0; y < image.height(); ++y)
    {
        for(int x = 0; x < image.width(); ++x)
        {
            if(qAlpha(image.pixel(x, y)) == 0)
            {
                continue;
            }
            if(x < left) { left = x; }
            if(x > right) { right = x; }
            if(y < top) { top = y; }
            if(y > bottom) { bottom = y; }
        }
    }

    if(right < left || bottom < top)
    {
        return;
    }

    opaqueBounds = QRect(QPoint(left, top), QPoint(right, bottom));
    opaqueBoundsValid = true;
}

QRect stompBox::scaledOpaqueBounds(double ratio) const
{
    if(!opaqueBoundsValid)
    {
        return QRect(QPoint(0, 0), stompSize);
    }

    const double scale = (id < 4) ? kInstBlockScale : kFlowBlockScale;
    const double scaleFactor = ratio / scale;
    const int x = qRound(opaqueBounds.x() * scaleFactor);
    const int y = qRound(opaqueBounds.y() * scaleFactor);
    const int w = qMax(1, qRound(opaqueBounds.width() * scaleFactor));
    const int h = qMax(1, qRound(opaqueBounds.height() * scaleFactor));
    return QRect(x, y, w, h);
}

QRect stompBox::signalBounds(double ratio) const
{
    return scaledOpaqueBounds(ratio).translated(this->pos());
}

QRect stompBox::flowLayoutBounds(double ratio) const
{
    const double scale = (this->id < 4) ? kInstBlockScale : kFlowBlockScale;
    QPixmap onPix(this->imagePathOn);
    if(!onPix.isNull() && onPix.height() > 0)
    {
        QSize drawSize = onPix.size();
        if(this->id >= 4)
        {
            drawSize.setHeight(qMin(drawSize.height(), 96));
        }
        const QSize scaled(qMax(1, qRound(drawSize.width() * ratio / scale)),
                           qMax(1, qRound(drawSize.height() * ratio / scale)));
        return QRect(this->pos(), scaled);
    }

    return QRect(this->pos(), this->size());
}

int stompBox::signalCenterYOffset(double ratio) const
{
    // The signal line passes through the vertical centre of the standard 96-pixel-tall
    // icon region.  Some output blocks (sub_out, main_out) have decorative content below
    // the 96-px area, making their full image taller (139–159 px).  Using the full image
    // height would push their centre offset too far down, causing the block to render
    // above the signal line.  Capping at 96 px keeps every flow block's connector on the
    // correct signal-line Y regardless of any extra artwork below the icon area.
    const double scale = (this->id < 4) ? kInstBlockScale : kFlowBlockScale;
    QPixmap onPix(this->imagePathOn);
    if(!onPix.isNull() && onPix.height() > 0)
    {
        const int signalHeight = (this->id < 4) ? onPix.height() : qMin(onPix.height(), 96);
        return qRound(signalHeight * ratio / scale / 2.0);
    }
    // Fallback: use half the full widget height
    return qRound(this->height() / 2.0);
}

void stompBox::positionFlowDescriptionLabel(double ratio)
{
    if(!this->effectLabel) { return; }
    if(hidesFlowDescriptionLabel(this->id))
    {
        this->effectLabel->setText(QString());
        this->effectLabel->hide();
        return;
    }

    QWidget *host = this->parentWidget();
    if(!host) { return; }

    if(this->effectLabel->parentWidget() != host)
    {
        this->effectLabel->setParent(host);
        this->effectLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
    }

    const bool isInputSource = (this->id < 4) ||
                               this->imagePathOff.contains("inst", Qt::CaseInsensitive) ||
                               this->imagePathOff.contains("normal", Qt::CaseInsensitive);
    const QRect visualRect = scaledOpaqueBounds(ratio);
    const int labelHeight = qMax(14, qRound(15 * ratio));
    const int gap = isInputSource ? flowLabelGapPx : qMax(0, flowLabelGapPx);
    const QPoint topLeft = this->pos();
    const int visualBottomY = topLeft.y() + visualRect.bottom();
    const int visualCenterX = topLeft.x() + visualRect.center().x();
    const int textWidth = QFontMetrics(this->effectLabel->font()).horizontalAdvance(this->effectLabel->text());
    const int minWidth = qMax(8, visualRect.width());
    const int labelWidth = isInputSource ? qMax(minWidth, textWidth + qMax(8, qRound(8 * ratio)))
                                         : minWidth;
    const int labelX = visualCenterX - (labelWidth / 2);
    this->effectLabel->setGeometry(labelX, visualBottomY + gap,
                                   labelWidth, labelHeight);
    this->effectLabel->setAlignment(flowLabelAlignment);
    this->effectLabel->setWordWrap(false);
    this->effectLabel->show();
}

void stompBox::updateFlowLabelLayoutDefaults()
{
    const bool isInputSource = (this->id < 4) ||
                               this->imagePathOff.contains("inst", Qt::CaseInsensitive) ||
                               this->imagePathOff.contains("normal", Qt::CaseInsensitive);
    flowLabelGapPx = isInputSource ? kInputLabelOffsetPx : kUniformLabelOffsetPx;
    flowLabelAlignment = Qt::AlignHCenter | Qt::AlignTop;
    if(this->effectLabel)
    {
        this->effectLabel->setProperty("fxId", QVariant::fromValue<int>(this->id));
        if(hidesFlowDescriptionLabel(this->id))
        {
            this->effectLabel->setText(QString());
            this->effectLabel->hide();
        }
    }
}

void stompBox::paintEvent(QPaintEvent *)
{
    t_reset = false;
    // Source tiles (id 0-3) use kInstBlockScale so they render larger than
    // the FX effect blocks.  All drawing code below uses 'scale' uniformly.
    double scale = (id < 4) ? kInstBlockScale : kFlowBlockScale;
    SysxIO *sysxIO = &AppServices::instance().sysx();
    Preferences *preferences = &AppServices::instance().preferences();
    QString sys1 = "04";
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){sys1 = "0B";};
    int main_link = sysxIO->getSourceValue("00", sys1, "00", "34");  // main stereo  link
    int sub_link = sysxIO->getSourceValue("00", sys1, "00", "35");  // sub stereo  link

    if(id==22 || id==28 || id==30 || id== 32 || (id==25 && sub_link>0) || (id==27 && main_link>0))
    {
        this->hide();
    }
    else
    {
        Preferences *preferences = &AppServices::instance().preferences();
        bool ok;
        double ratio = preferences->getPreferences("Window", "Scale", "ratio").toDouble(&ok);
        // Cap flow-block widget height to 96 px so the signal line passes through
        // the visual centre of every block (output blocks have taller decorative art).
        // Compute and apply size BEFORE creating QPainter — calling setFixedSize while
        // a QPainter is active triggers geometry invalidation on all siblings, causing
        // Qt to re-enter paintSiblingsRecursive recursively until stack overflow (SIGSEGV 0x28).
        QSize paintSize = QPixmap(imagePathOn).size();
        if(id >= 4) { paintSize.setHeight(qMin(paintSize.height(), 96)); }
        setSize(paintSize*ratio/scale);
        QPainter painter(this);
        painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
        if(this->hovered==true)
        {
            QPixmap imageHover(this->imagePathHover);
            const int drawH = (id >= 4) ? qMin(imageHover.height(), 96) : imageHover.height();
            QRectF target1(0.0, 0.0, imageHover.width()*ratio/scale, drawH*ratio/scale);
            QRectF source1(0.0, 0.0, imageHover.width(), drawH);
            painter.drawPixmap(target1, imageHover, source1);
        };

        if(this->selected==true)
        {
            QPixmap imageSelected(this->imagePathSelected);
            const int drawH = (id >= 4) ? qMin(imageSelected.height(), 96) : imageSelected.height();
            QRectF target2(0.0, 0.0, imageSelected.width()*ratio/scale, drawH*ratio/scale);
            QRectF source2(0.0, 0.0, imageSelected.width(), drawH);
            painter.drawPixmap(target2, imageSelected, source2);
            t_reset = true;
            this->selected=false;
        };

        if(this->sw==true)
        {
            QPixmap imageOn(this->imagePathOn);
            const int drawH = (id >= 4) ? qMin(imageOn.height(), 96) : imageOn.height();
            QRectF target3(0.0, 0.0, imageOn.width()*ratio/scale, drawH*ratio/scale);
            QRectF source3(0.0, 0.0, imageOn.width(), drawH);
            painter.drawPixmap(target3, imageOn, source3);
            this->image = imageOn;
        };

        if(this->sw==false)
        {
            QPixmap imageOff(this->imagePathOff);
            const int drawH = (id >= 4) ? qMin(imageOff.height(), 96) : imageOff.height();
            QRectF target(0.0, 0.0, imageOff.width()*ratio/scale, drawH*ratio/scale);
            QRectF source(0.0, 0.0, imageOff.width(), drawH);
            painter.drawPixmap(target, imageOff, source);
            this->image = imageOff;
        };

        painter.end();
        // Do NOT call setParent or positionFlowDescriptionLabel from paintEvent —
        // reparenting a widget mid-paint corrupts Qt's internal paint device pointer
        // and causes a SIGSEGV crash when amp_statusSignal triggers repaints on all
        // sibling stompBoxes simultaneously.  Label layout is handled by setPos().
    };
    if(t_reset==true)
    {
        t_reset=false;
        meter_reset();
    };
}

editWindow* stompBox::editDetails()
{
    return this->editDialog;
}

void stompBox::mousePressEvent(QMouseEvent *event)
{
    this->dragStartPosition = event->position().toPoint();
    this->dragInProgress = false;

    this->selected=true;
    this->hovered=false;
    QObject::disconnect(this->parent(), SIGNAL(ns1_statusSignal(bool)), this, SLOT(ns1_ButtonSignal(bool) ));
    emit this->ns1_statusSignal(true);                                                                             // clear all 'selected' icon except this one.
    QObject::connect(this->parent(), SIGNAL(ns1_statusSignal(bool)), this, SLOT(ns1_ButtonSignal(bool) ));
    //this->repaint();
    this->editDialog->setPowerState(this->sw);
    this->editDialog->setWindow(this->fxName);
    emit setEditDialog(this->editDialog);
    emit this->mod_statusSignal(true);             // hide old edit page.
    emit this->amp_statusSignal(true);             //clear all hover icon
    update();
}

void stompBox::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton && !this->dragInProgress)
    {
        const int dragDistance = (event->position().toPoint() - this->dragStartPosition).manhattanLength();
        if(dragDistance <= QApplication::startDragDistance() &&
           id != 24 && id != 25 && id != 26 && id != 27 &&
           this->hex0 != "void" && this->hex1 != "void" &&
           this->hex2 != "void" && this->hex3 != "void")
        {
            this->sw = !this->sw;
            valueChanged(this->sw);
            update();
        }
    }

    this->dragInProgress = false;
    QWidget::mouseReleaseEvent(event);
}

void stompBox::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
}

void stompBox::mouseMoveEvent(QMouseEvent *event)
{
    const bool dragging = event->buttons().testFlag(Qt::LeftButton);
    if(!dragging)
    {
        QObject::disconnect(this->parent(), SIGNAL(amp_statusSignal(bool)), this, SLOT(amp_ButtonSignal(bool) ));
        if(this->underMouse())
        {
            emit this->amp_statusSignal(true);
            this->hovered=true;
            this->update();
            this->timer->start(2000);
        }
        QObject::connect(this->parent(), SIGNAL(amp_statusSignal(bool)), this, SLOT(amp_ButtonSignal(bool) ));
    }


    if (event->buttons() == Qt::LeftButton && id<28 /*&& id!=21*/ && id!=22 && id!=23)
    {
        QPoint mousePoint = event->position().toPoint();
        QRect stompRect = this->rect();
        const int dragDistance = (event->position().toPoint() - dragStartPosition).manhattanLength();

        if ( /*(event->position().toPoint() - dragStartPosition).manhattanLength() > QApplication::startDragDistance() &&*/
            stompRect.contains(mousePoint) &&
            dragDistance > QApplication::startDragDistance() )
        {
            this->dragInProgress = true;

            QByteArray itemData;
            QDataStream dataStream(&itemData, QIODevice::WriteOnly);
            dataStream << id << stompPos << stompSize << event->position().toPoint() - rect().topLeft();

            QMimeData *mimeData = new QMimeData;
            mimeData->setData("application/x-stompbox", itemData);
            mimeData->setText(this->objectName());

            QDrag *drag = new QDrag(this);
            drag->setMimeData(mimeData);
            drag->setHotSpot(event->position().toPoint() - rect().topLeft());

            QPixmap screen = QWidget::grab().scaled(stompSize.width(), stompSize.height(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
            //screen.setMask(image.mask().scaled(stompSize.width(), stompSize.height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

            QRectF source(0, 0, stompSize.width(), stompSize.height());
            QRectF target(0, 0, stompSize.width(), stompSize.height());

            QPixmap buffer = image;
            buffer = buffer.scaled(stompSize.width(), stompSize.height(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
            QPainter painter(&buffer);
            painter.drawPixmap(target, screen, source);
            painter.end();

            drag->setPixmap(buffer);
            const Qt::DropAction dropAction = drag->exec(Qt::MoveAction);
            if (dropAction != Qt::MoveAction)
            {
                event->ignore();
            };
        };
    };
}

void stompBox::amp_ButtonSignal(bool value)
{
    Q_UNUSED(value);
    sel_count=0;
    this->hovered=false;
    this->update();
}

void stompBox::ns1_ButtonSignal(bool value)
{
    Q_UNUSED(value);
    if(this->selected)
    {
        this->selected=false;
        update();
    }
}

void stompBox::mod_ButtonSignal(bool value)
{
    Q_UNUSED(value);
    if (this->id == 1)
    {
        this->editDialog->setWindow(this->fxName);
        emit setEditDialog(this->editDialog);
    };
}

void stompBox::setPos(QPoint newPos)
{
    this->move(newPos);
    this->stompPos = newPos;
    Preferences *preferences = &AppServices::instance().preferences();
    bool ok;
    const double ratio = preferences->getPreferences("Window", "Scale", "ratio").toDouble(&ok);
    positionFlowDescriptionLabel(ratio);
    updateStompPath();
}

void stompBox::setPosMaster(QPoint newPos)
{
    if(this->id == 34){
        this->move(newPos);
        this->stompPos = newPos;
        updateStompPath(); };
}

void stompBox::updatePos(signed int offsetDif)
{
    this->stompPos = this->pos();
    QPoint newPos = stompPos + QPoint(offsetDif, 0);
    this->move(newPos);
    this->stompPos = newPos;
    Preferences *preferences = &AppServices::instance().preferences();
    bool ok;
    const double ratio = preferences->getPreferences("Window", "Scale", "ratio").toDouble(&ok);
    positionFlowDescriptionLabel(ratio);
}

void stompBox::setOnImage(QString imagePathOn)
{
    this->imagePathOn = imagePathOn;
}

void stompBox::setStereoImage(QString imagePathStereoOn)
{
    this->imagePathOn = imagePathStereoOn;
}

void stompBox::setOffImage(QString imagePathOff)
{
    this->imagePathOff = imagePathOff;
    refreshOpaqueBounds();
}

void stompBox::setHoverImage(QString imagePathHover)
{
    this->imagePathHover = imagePathHover;
}

void stompBox::setSelectedImage(QString imagePathSelected)
{
    this->imagePathSelected = imagePathSelected;
}

void stompBox::setSize(QSize newSize)
{
    if(newSize == this->stompSize) return;  // no-op: avoids spurious geometry invalidation
    this->stompSize = newSize;
    this->setFixedSize(stompSize);
    // NOTE: do NOT call positionFlowDescriptionLabel here.
    // setSize is called from paintEvent; calling setParent/setGeometry on the effectLabel
    // while a QPainter is active on this widget causes Qt to modify the widget tree mid-paint,
    // triggering recursive paintSiblingsRecursive calls that crash at null-deref 0x28.
    // Label positioning is handled separately via setPos(), setOnImage(), etc.
}

void stompBox::setId(unsigned int id)
{
    this->id = id;
    updateFlowLabelLayoutDefaults();
    if(this->id >= 4 && this->effectLabel->parentWidget() != this)
    {
        this->effectLabel->setParent(this);
    }
}

void stompBox::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    Preferences *preferences = &AppServices::instance().preferences();
    bool ok;
    const double ratio = preferences->getPreferences("Window", "Scale", "ratio").toDouble(&ok);
    positionFlowDescriptionLabel(ratio);
}

void stompBox::hideEvent(QHideEvent *event)
{
    if(this->effectLabel)
    {
        this->effectLabel->hide();
    }
    QWidget::hideEvent(event);
}

unsigned int stompBox::getId()
{
    return this->id;
}

void stompBox::setLSB(QString hex1, QString hex2)
{
    this->editDialog->setLSB(hex1, hex2);
}

void stompBox::brushColor(QString BarColor)
{
    QBrush brush(Qt::white);
    if(BarColor=="white"){ brush.setColor(Qt::white); };
    if(BarColor=="gray"){ brush.setColor(Qt::gray); };
    if(BarColor=="red"){ brush.setColor(Qt::red); };
    if(BarColor=="blue"){ brush.setColor(Qt::blue); };
    if(BarColor=="magenta"){ brush.setColor(Qt::magenta); };
    if(BarColor=="green"){ brush.setColor(Qt::green); };
    if(BarColor=="yellow"){ brush.setColor(Qt::yellow); };
    if(BarColor=="cyan"){ brush.setColor(Qt::cyan); };
    if(BarColor=="purple"){ brush.setColor(QColor(100, 0, 200)); };
    this->editDialog->BrushColor(brush);
}

void stompBox::setButton(QString hex0, QString hex1, QString hex2, QString hex3)
{
    this->hex0 = hex0;
    this->hex1 = hex1;
    this->hex2 = hex2;
    this->hex3 = hex3;
    this->editDialog->setPowerAddress(hex0, hex1, hex2, hex3);
}

void stompBox::updateButton(QString hex0, QString hex1, QString hex2, QString hex3)
{
    if(hex0!="void" && hex1!="void" &&  hex2!="void" && hex3!="void")
    {
        SysxIO *sysxIO = &AppServices::instance().sysx();
        int value = sysxIO->getSourceValue(hex0, hex1, hex2, hex3);
        this->sw = value;
        if(this->editDialog)
        {
            this->editDialog->setPowerState(this->sw);
        }
        update();
    };
}

void stompBox::updateLabel(QString hex0, QString hex1, QString hex2, QString hex3)
{
    SysxIO *sysxIO = &AppServices::instance().sysx();
    MidiTable *midiTable = &AppServices::instance().midiTable();
    int value = sysxIO->getSourceValue(hex0, hex1, hex2, hex3);
    QString valueHex = QString::number(value, 16).toUpper();
    if(valueHex.length() < 2) valueHex.prepend("0");
    QString valueStr = midiTable->getValue(hex0, hex1, hex2, hex3, valueHex).toLower();
    if(id < 4) {
        // Input source tiles: show only the type descriptor.
        // The image already colour-codes which instrument it is, so the
        // source prefix is redundant here.
        this->effectLabel->setText(valueStr);
    } else {
        if(id > 4) { valueStr.truncate(10); }
        this->effectLabel->setText(valueStr);
    }
}

void stompBox::valueChanged(int value, QString hex0, QString hex1, QString hex2, QString hex3)
{
    QString valueHex = QString::number(value, 16).toUpper();
    if(valueHex.length() < 2) valueHex.prepend("0");
    emitValueChanged(hex0, hex1, hex2, hex3, valueHex);
}

void stompBox::valueChanged(bool value, QString hex0, QString hex1, QString hex2, QString hex3)
{
    int valueInt;
    (value)? valueInt=1: valueInt=0;
    QString valueHex = QString::number(valueInt, 16).toUpper();
    if(valueHex.length() < 2) valueHex.prepend("0");
    emitValueChanged(hex0, hex1, hex2, hex3, valueHex);
}

void stompBox::valueChanged(bool value)
{
    if(hex0!="void" && hex1!="void" &&  hex2!="void" && hex3!="void")
    {
        if(this->editDialog)
        {
            this->editDialog->setPowerState(value);
        }
        valueChanged(value, this->hex0, this->hex1, this->hex2, this->hex3);
        if(this->editDialog)
        {
            QMetaObject::invokeMethod(this->editDialog, "dialogUpdateSignal", Qt::DirectConnection);
            this->editDialog->pageUpdateSignal();
        }
    };
}

void stompBox::emitValueChanged(QString hex0, QString hex1, QString hex2, QString hex3, QString valueHex)
{
    SysxIO *sysxIO = &AppServices::instance().sysx();
    QString valueName, valueStr;
    if(hex1 != "void" && hex2 != "void" && sysxIO->deviceReady())
    {
        MidiTable *midiTable = &AppServices::instance().midiTable();
        if(valueHex != "void")
        {
            bool ok;
            if(midiTable->isData("Structure", hex1, hex2, hex3))
            {
                int maxRange = QString("7F").toInt(&ok, 16) + 1;
                int value = valueHex.toInt(&ok, 16);
                int dif = value/maxRange;
                QString valueHex1 = QString::number(dif, 16).toUpper();
                if(valueHex1.length() < 2) valueHex1.prepend("0");
                QString valueHex2 = QString::number(value - (dif * maxRange), 16).toUpper();
                if(valueHex2.length() < 2) valueHex2.prepend("0");
                sysxIO->setFileSource(hex0, hex1, hex2, hex3, valueHex1, valueHex2);
            }
            else
            {
                sysxIO->setFileSource(hex0, hex1, hex2, hex3, valueHex);
                emit dialogUpdateSignal();
                //sysxIO->setFileSource(hex0, hex1, hex2, hex3, valueHex);
            };
            Midi items = midiTable->getMidiMap(hex0, hex1, hex2, hex3);
            valueName = items.customdesc;
            valueStr = midiTable->getValue(hex0, hex1, hex2, hex3, valueHex);
            emit valueChanged(this->fxName, valueName, valueStr);
        };
    };
}

void stompBox::setDisplayToFxName()
{
    //emit valueChanged(this->fxName, "", "");
    QString sys1 = "04";
    Preferences *preferences = &AppServices::instance().preferences();
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){sys1 = "0B";};
    SysxIO *sysxIO = &AppServices::instance().sysx();
    int value = sysxIO->getSourceValue("00", sys1, "00", "34"); // main l/r link
    if(this->id == 26)
    {
        if(value > 0)
        {
            this->fxName = tr("MAIN OUT STEREO");
            this->imagePathOff = ":/images/main_out_stereo.png";
        }
        else
        {
            this->fxName = tr("MAIN OUT LEFT");
            if(id == 26){this->imagePathOff = ":/images/main_out_left.png";};
        };
        this->editDialog->setWindow(this->fxName);
        emit sendStompSignal(27, value);
        update();
    };

    value = sysxIO->getSourceValue("00", sys1, "00", "35"); // sub l/r link
    if(this->id == 24)
    {
        if(value > 0)
        {
            this->fxName = tr("SUB OUT STEREO");
            this->imagePathOff = ":/images/sub_out_stereo.png";
        }
        else
        {
            this->fxName = tr("SUB OUT LEFT");
            this->imagePathOff = ":/images/sub_out_left.png";
        };
        this->editDialog->setWindow(this->fxName);
        emit sendStompSignal(25, value);
        update();
    };
}

void stompBox::interStomp(uint id, int value)
{
    if(this->id == id)   // this->id
    {
        if(value!=last_value)
        {
            if(value > 0)
            {
                this->hide();
            }
            else
            {
                this->show();
            };
            last_value=value;
            emit updateDrop();
        };
    };
}

void stompBox::updateDisplay(QString text)
{
    Q_UNUSED(text);
    // emit valueChanged(this->fxName, "", text);
}

void stompBox::updateStompPath()
{
    if (this->id == 0)  {this->fxName = tr("NORMAL PICKUP");};
    if (this->id == 1)  {this->fxName = tr("INSTRUMENT 1");};
    if (this->id == 2)  {this->fxName = tr("INSTRUMENT 2");};
    if (this->id == 3)  {this->fxName = tr("INSTRUMENT 3");};
    if (this->id == 4)  {this->fxName = tr("COMPRESSOR");};
    if (this->id == 5)  {this->fxName = tr("DISTORTION");};
    if (this->id == 6)  {this->fxName = tr("AMP/CABINET");};
    if (this->id == 7)  {this->fxName = tr("NOISE SUPPRESSOR");};
    if (this->id == 8)  {this->fxName = tr("FX1");};
    if (this->id == 9)  {this->fxName = tr("FX2");};
    if (this->id == 10) {this->fxName = tr("FX3");};
    if (this->id == 11) {this->fxName = tr("EQUALIZER 1");};
    if (this->id == 12) {this->fxName = tr("EQUALIZER 2");};
    if (this->id == 13) {this->fxName = tr("CHORUS");};
    if (this->id == 14) {this->fxName = tr("DELAY 1");};
    if (this->id == 15) {this->fxName = tr("DELAY 2");};
    if (this->id == 16) {this->fxName = tr("MASTER DELAY");};
    if (this->id == 17) {this->fxName = tr("REVERB");};
    if (this->id == 18) {this->fxName = tr("FOOT VOLUME 1");};
    if (this->id == 19) {this->fxName = tr("FOOT VOLUME 2");};
    if (this->id == 20) {this->fxName = tr("SEND/RETURN");};
    if (this->id == 21) {this->fxName = tr("DIVIDER");};
    if (this->id == 22) {this->fxName = tr("BRANCH A/B");};
    if (this->id == 23) {this->fxName = tr("MIXER");};
    if (this->id == 24) {this->fxName = tr("SUB OUT LEFT");};
    if (this->id == 25) {this->fxName = tr("SUB OUT RIGHT");};
    if (this->id == 26) {this->fxName = tr("MAIN OUT LEFT");};
    if (this->id == 27) {this->fxName = tr("MAIN OUT RIGHT");};
    if (this->id == 28) {this->fxName = tr("SPLIT 1");};
    if (this->id == 29) {this->fxName = tr("BALANCER 1");};
    if (this->id == 30) {this->fxName = tr("SPLIT 2");};
    if (this->id == 31) {this->fxName = tr("BALANCER 2");};
    if (this->id == 32) {this->fxName = tr("SPLIT 3");};
    if (this->id == 33) {this->fxName = tr("BALANCER 3");};
    if (this->id == 34) {this->fxName = tr("MASTER");};
    //setDisplayToFxName();
}

void stompBox::meter_reset()
{
    SysxIO *sysxIO = &AppServices::instance().sysx();
    if(sysxIO->mode>1){sysxIO->setMode("mode2");};
}

void stompBox::blink()
{
    this->timer->stop();
    emit this->amp_statusSignal(true);             //clear all hover icon
    //if(this->selected==true){this->selected=false;}else {this->selected=true;};
    this->update();
    clearFocus();
}
