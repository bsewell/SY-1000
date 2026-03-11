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

#include "customControlKnob.h"
#include "MidiTable.h"
#include "SysxIO.h"
#include "Preferences.h"
//#include <QtCore>


customControlKnob::customControlKnob(QWidget *parent, 
                                     QString hex0, QString hex1, QString hex2, QString hex3,
                                     QString background, QString direction, int lenght)
    : QWidget(parent)
{
    Preferences *preferences = Preferences::Instance();
    bool ok;
    // UI rule: all knobs must render at one uniform (small) size.
    // Ignore legacy *_ratioX hints embedded in background strings.
    const double multiplier = 1.0;
    this->ratio = preferences->getPreferences("Window", "Scale", "ratio").toDouble(&ok)*multiplier;
    double fratio = preferences->getPreferences("Window", "Font", "ratio").toDouble(&ok)*multiplier;
    this->colour = preferences->getPreferences("Scheme", "Colour", "select");
#ifdef Q_OS_MAC
    //fratio = fratio*1.4;
#endif

    this->previous_text = "hello!";
    counter = 0;
    this->label = new customControlLabel(this);
    this->hex0 = hex0;
    this->hex1 = hex1;
    this->hex2 = hex2;
    this->hex3 = hex3;
    this->background = background;
    this->lenght = lenght;
    _lastValue = -1;
    const int verticalGap = qRound(5 * ratio);
    const int sideGap = qRound(4 * ratio);
    const int stackedControlMinHeight = qRound(78 * ratio);
    const int sideControlMinHeight = qRound(60 * ratio);

    MidiTable *midiTable = MidiTable::Instance();
    Midi items = midiTable->getMidiMap(hex0, hex1, hex2, hex3);
    this->label->setFontRole("setting");
    this->label->setText(items.customdesc);
    this->label->setUpperCase(true);
    this->label->setTextRaise(5);

    this->knob = new customKnob(this, hex0, hex1, hex2, hex3, background, direction);
    this->knob->setWhatsThis(tr("hold down mouse button and drag up/down for quick adjustment")
                             + "<br>" + tr("use scroll wheel or up/down arrow keys for fine adjustment"));
    if(colour.contains("0"))
    {
        this->displayBTS = new QTextEdit(this);
        this->displayBTS->setObjectName("btsdisplay");
        QFont Sfont( "Arial", 8*fratio, QFont::Normal);
        this->displayBTS->setFont(Sfont);
        this->displayBTS->setDisabled(true);
        this->displayBTS->setAlignment(Qt::AlignCenter);
        this->displayBTS->setFixedSize(40*ratio, 40*ratio);
        //this->displayBTS->setFrame(false);


        //QLabel *knobbg = new QLabel(this);
        //knobbg->setPixmap(QPixmap(":/images/sourceBG.png").scaled(this->width()*2*ratio, this->height()*4*ratio, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

        QHBoxLayout *displayLayout = new QHBoxLayout;
        displayLayout->addWidget(this->displayBTS);

        QHBoxLayout *knobLayout = new QHBoxLayout;
        knobLayout->addWidget(this->knob);

       // QHBoxLayout *bgLayout = new QHBoxLayout;
      //  bgLayout->addWidget(this->knobbg, 0 , Qt::AlignCenter);

        QStackedLayout *stackLayout = new QStackedLayout;
        stackLayout->setStackingMode(QStackedLayout::StackAll);
        stackLayout->addWidget(this->knob);
        stackLayout->addWidget(this->displayBTS);
        //stackLayout->addWidget(knobbg);

        this->label->setAlignment(Qt::AlignCenter);
        this->label->setTextWidth(qRound(80 * ratio));

        QVBoxLayout *mainLayout = new QVBoxLayout;
        //mainLayout->addStretch(1);
        mainLayout->addLayout(stackLayout);
        mainLayout->addWidget(this->label);
        // mainLayout->setAlignment(Qt::AlignCenter);

        this->setLayout(mainLayout);
        //this->setFixedHeight(60*ratio);

        this->displayBTS->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        this->displayBTS->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }
    else
    {
        this->display = new QLineEdit(this);
        this->display->setObjectName("editdisplay");
        QFont Sfont( "Arial", 8*fratio, QFont::Normal);
        this->display->setFont(Sfont);
        this->display->setFixedWidth(lenght*ratio);
        this->display->setFixedHeight(20*ratio);
        this->display->setAlignment(Qt::AlignCenter);
        this->display->setDisabled(true);

        if(direction.contains("left"))
        {
            this->label->setAlignment(Qt::AlignRight);
            this->display->setFixedWidth(lenght*ratio);

            QVBoxLayout *displayLayout = new QVBoxLayout;
            displayLayout->setContentsMargins(0, verticalGap, 0, verticalGap);
            displayLayout->setSpacing(verticalGap);
            displayLayout->addStretch(0);
            displayLayout->addWidget(this->label, 0, Qt::AlignRight);
            displayLayout->addWidget(this->display, 0, Qt::AlignRight);
            displayLayout->addStretch(0);

            QHBoxLayout *mainLayout = new QHBoxLayout;
            mainLayout->setContentsMargins(0, 0, 0, 0);
            mainLayout->setSpacing(sideGap);
            mainLayout->addLayout(displayLayout);
            mainLayout->addWidget(this->knob, 0, Qt::AlignCenter);

            this->setLayout(mainLayout);
            this->setMinimumHeight(sideControlMinHeight);
            //this->setFixedHeight(this->knob->height()*ratio);
        }
        else if(direction.contains("right"))
        {
            this->label->setAlignment(Qt::AlignLeft);
            this->display->setFixedWidth(lenght*ratio);

            QVBoxLayout *displayLayout = new QVBoxLayout;
            displayLayout->setContentsMargins(0, verticalGap, 0, verticalGap);
            displayLayout->setSpacing(verticalGap);
            displayLayout->addStretch(0);
            displayLayout->addWidget(this->label, 0, Qt::AlignLeft);
            displayLayout->addWidget(this->display, 0, Qt::AlignLeft);
            displayLayout->addStretch(0);

            QHBoxLayout *mainLayout = new QHBoxLayout;
            mainLayout->setContentsMargins(0, 0, 0, 0);
            mainLayout->setSpacing(sideGap);
            mainLayout->addWidget(this->knob, 0, Qt::AlignCenter);
            mainLayout->addLayout(displayLayout);

            this->setLayout(mainLayout);
            this->setMinimumHeight(sideControlMinHeight);
            //this->setFixedHeight(this->knob->height()*ratio);
        }
        else if(direction.contains("bottom"))
        {
            this->label->setAlignment(Qt::AlignCenter);
            this->label->setTextWidth(qRound(118 * ratio));
            this->display->setFixedWidth(lenght*ratio);

            QVBoxLayout *mainLayout = new QVBoxLayout;
            mainLayout->setContentsMargins(0, verticalGap, 0, verticalGap);
            mainLayout->setSpacing(verticalGap);
            mainLayout->addWidget(this->label, 0, Qt::AlignCenter);
            mainLayout->addWidget(this->knob, 0, Qt::AlignCenter);
            mainLayout->addWidget(this->display, 0, Qt::AlignCenter);
            mainLayout->addStretch(0);

            this->setLayout(mainLayout);
            this->setMinimumHeight(stackedControlMinHeight);
            //this->setFixedHeight((this->knob->height() + 16 + 12)*ratio);

        }
        else if(direction.contains("System"))
        {
            this->label->setAlignment(Qt::AlignCenter);
            this->label->setTextWidth(qRound(118 * ratio));
            this->display->setFixedWidth(lenght*ratio);

            QVBoxLayout *mainLayout = new QVBoxLayout;
            mainLayout->setContentsMargins(0, verticalGap, 0, verticalGap);
            mainLayout->setSpacing(verticalGap);
            mainLayout->addWidget(this->label, 0, Qt::AlignCenter);
            mainLayout->addWidget(this->knob, 0, Qt::AlignCenter);
            mainLayout->addWidget(this->display, 0, Qt::AlignCenter);
            mainLayout->addStretch(0);


            this->setLayout(mainLayout);
            this->setMinimumHeight(stackedControlMinHeight);
            //this->setFixedHeight((this->knob->height() + 16 + 12)*ratio);
        }
        else
        {
            this->label->setAlignment(Qt::AlignCenter);
            this->label->setTextWidth(qRound(118 * ratio));
            this->display->setFixedWidth(lenght*ratio);

            QVBoxLayout *mainLayout = new QVBoxLayout;
            mainLayout->setContentsMargins(0, verticalGap, 0, verticalGap);
            mainLayout->setSpacing(verticalGap);
            mainLayout->addWidget(this->label, 0, Qt::AlignCenter);
            mainLayout->addWidget(this->knob, 0, Qt::AlignCenter);
            mainLayout->addWidget(this->display, 0, Qt::AlignCenter);
            mainLayout->addStretch(0);


            this->setLayout(mainLayout);
            this->setMinimumHeight(stackedControlMinHeight);
            //this->setFixedHeight((this->knob->height() + 16 + 12)*ratio);
        };
    };

    QObject::connect(this->parent(), SIGNAL( dialogUpdateSignal() ),
                     this, SLOT( dialogUpdateSignal() ));

    QObject::connect(this, SIGNAL( updateSignal() ),
                     this->parent(), SIGNAL( updateSignal() ));


    if(colour.contains("0"))
    {
        QObject::connect(this, SIGNAL( updateDisplay(QString) ),
                         this->displayBTS, SLOT( setText(QString) ));
    }
    else
    {
        QObject::connect(this, SIGNAL( updateDisplay(QString) ),
                         this->display, SLOT( setText(QString) ));
    };

    // QObject::connect(this, SIGNAL( updateDisplay(QString) ),
    //this->parent(), SLOT( updateDisplay(QString) ));

    hover = false;
    timer = new QTimer(this);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(timed()) );

    setMouseTracking(true);
}

void customControlKnob::paintEvent(QPaintEvent *)
{
    //QString current_text = display->document()->toPlainText();
    //if(colour == "0" /*&& current_text != previous_text && counter<2*/)
    //{
    /* this->display->setAlignment(Qt::AlignCenter);
        if(display->document()->toPlainText().contains(":")){this->display->move(3*ratio, 4*ratio);}
        else if(display->document()->toPlainText().contains("kHz")){this->display->move(3*ratio, 6*ratio);}
        else if(display->document()->toPlainText().contains("Hz")){this->display->move(3*ratio, 6*ratio);}
        else if(display->document()->toPlainText().contains("cm")){this->display->move(3*ratio, 6*ratio);}
        else if(display->document()->toPlainText().contains("ms")){this->display->move(3*ratio, 6*ratio);}
        else if(display->document()->toPlainText().contains("dB")){this->display->move(3*ratio, 6*ratio);}
        else if(display->document()->toPlainText().contains("mm")){this->display->move(3*ratio, 6*ratio);}
        else if(display->document()->toPlainText().contains("User")){this->display->move(4*ratio, 7*ratio);}
        else if(display->document()->toPlainText().contains("Preset")){this->display->move(4*ratio, 7*ratio);}
        else if(display->document()->toPlainText().contains(" BPM")){this->display->move(3*ratio, 6*ratio);}
        else{this->display->move(5*ratio, 10*ratio);};

        this->label->setAlignment(Qt::AlignHCenter);
        this->knob->move(((this->width()/2)-(knob->width()/2))-(3*ratio), 5*ratio);*/
    //};
    counter++;
    /* DRAWS RED BACKGROUND  */
    if(hover==true)
    {
        QPixmap image(":/images/dragbar.png");
        QRectF target(0.0, 0.0, this->width(), this->height());
        QRectF source(0.0, 0.0, 600, 600);
        QPainter painter(this);
        painter.drawPixmap(target, image, source);
    };
}

void customControlKnob::mouseMoveEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    if(this->underMouse() ){this->knob->knob->setFocus(); hover=true;  repaint(); this->timer->start(500);};
    this->knob->knob->setFocus(); hover=true;  repaint(); this->timer->start(500);
}

void customControlKnob::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    if(this->underMouse() ){this->knob->knob->setFocus(); hover=true;  repaint(); this->timer->start(500);};
    this->knob->knob->setFocus(); hover=true;  repaint(); this->timer->start(500);
}

void customControlKnob::dialogUpdateSignal()
{
    SysxIO *sysxIO = SysxIO::Instance();
    MidiTable *midiTable = MidiTable::Instance();
    int value = sysxIO->getSourceValue(this->hex0, this->hex1, this->hex2, this->hex3);
    if(value != _lastValue)
    {
        _lastValue = value;
        QString valueHex = QString::number(value, 16).toUpper();
        if(valueHex.length() < 2) valueHex.prepend("0");

        QString valueStr = midiTable->getValue(this->hex0, hex1, hex2, hex3, valueHex);

        if(background.contains("inverse")){value =  int (midiTable->getRange(hex0, hex1, hex2, hex3))+(midiTable->getRangeMinimum(hex0, hex1, hex2, hex3))-value;};
        this->knob->setValue(value);

      /*  if(colour.contains("0"))
        {
            if(valueStr.contains(":")){valueStr.insert(valueStr.indexOf(":")+1, "<br>");}
            else if(valueStr.contains("kHz")){valueStr.insert(valueStr.indexOf("kHz")+1, "<br>");}
            else if(valueStr.contains("Hz")){valueStr.insert(valueStr.indexOf("Hz"), "<br>");}
            else if(valueStr.contains("cm")){valueStr.insert(valueStr.indexOf("cm"), "<br>");}
            else if(valueStr.contains("ms")){valueStr.insert(valueStr.indexOf("ms"), "<br>");}
            else if(valueStr.contains("dB")){valueStr.insert(valueStr.indexOf("dB"), "<br>");}
            else if(valueStr.contains("mm")){valueStr.insert(valueStr.indexOf("mm"), "<br>");}
            else if(valueStr.contains("User")){valueStr.insert(valueStr.indexOf("User")+4, "<br>");}
            else if(valueStr.contains("Preset")){valueStr.insert(valueStr.indexOf("Preset")+6, "<br>");}
            else if(valueStr.contains(" BPM")){valueStr.insert(valueStr.indexOf(" BPM"), "<br>");}
            else{};
            valueStr.prepend("<br>");
        };*/
        emit updateDisplay(valueStr);
        //emit knobUpdate();
    };
}

void customControlKnob::knobUpdate()
{
  /*  if(colour.contains("0"))
    {
        this->display->setAlignment(Qt::AlignCenter);
        if(displayBTS->document()->toPlainText().contains(":"))          {this->displayBTS->move(3*ratio, 4*ratio);}
        else if(displayBTS->document()->toPlainText().contains("kHz"))   {this->displayBTS->move(3*ratio, 6*ratio);}
        else if(displayBTS->document()->toPlainText().contains("Hz"))    {this->displayBTS->move(3*ratio, 6*ratio);}
        else if(displayBTS->document()->toPlainText().contains("cm"))    {this->displayBTS->move(3*ratio, 6*ratio);}
        else if(displayBTS->document()->toPlainText().contains("ms"))    {this->displayBTS->move(3*ratio, 6*ratio);}
        else if(displayBTS->document()->toPlainText().contains("dB"))    {this->displayBTS->move(3*ratio, 6*ratio);}
        else if(displayBTS->document()->toPlainText().contains("mm"))    {this->displayBTS->move(3*ratio, 6*ratio);}
        else if(displayBTS->document()->toPlainText().contains("User"))  {this->displayBTS->move(4*ratio, 7*ratio);}
        else if(displayBTS->document()->toPlainText().contains("Preset")){this->displayBTS->move(4*ratio, 7*ratio);}
        else if(displayBTS->document()->toPlainText().contains(" BPM"))  {this->displayBTS->move(3*ratio, 6*ratio);}
        else                                                             {this->displayBTS->move(5*ratio, 10*ratio);};
    };*/
    this->label->setAlignment(Qt::AlignHCenter);
    this->knob->move(((this->width()/2)-(knob->width()/2))-(3*ratio), 5*ratio);
}
void customControlKnob::timed()
{
    if(this->underMouse() || this->knob->knob->hasFocus())
    {
        this->timer->stop();
        this->timer->start(500);

    }else{
        this->timer->stop();
        this->hover=false;
        this->repaint();
        this->knob->knob->clearFocus();
    };
}

void customControlKnob::highlight()
{
    this->hover=true;
    this->repaint();
    this->timer->start(500);
}
