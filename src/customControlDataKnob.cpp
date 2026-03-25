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

#include "customControlDataKnob.h"
#include "MidiTable.h"
#include "SysxIO.h"
#include "Preferences.h"

customControlDataKnob::customControlDataKnob(QWidget *parent,
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
#ifdef Q_OS_MAC
    fratio = fratio*1.4;
#endif
    this->colour = preferences->getPreferences("Scheme", "Colour", "select");
    this->label = new customControlLabel(this);
    this->hex0 = hex0;
    this->hex1 = hex1;
    this->hex2 = hex2;
    this->hex3 = hex3;
    this->area = "Structure";
    const int verticalGap = qRound(5 * ratio);
    const int sideGap = qRound(4 * ratio);
    const int stackedControlMinHeight = qRound(78 * ratio);
    const int sideControlMinHeight = qRound(60 * ratio);

    MidiTable *midiTable = MidiTable::Instance();
    this->hex_a = "00";
    this->hex_b = "00";
    this->hex_c = "00";
    this->byteSize = "3";
    _lastValue = -1;

    if (background.contains("PREDELAY"))   { this->hex_c = "07"; this->byteSize = "4"; };
    if (background.contains("DELAY2000"))  { this->hex_c = "0D"; this->byteSize = "4"; };
    if (background.contains("DELAY1300"))  { this->hex_c = "0B"; this->byteSize = "3"; };
    if (background.contains("KNOBFUNCT"))  { this->hex_b = "02"; this->hex_c = "07"; this->byteSize = "4"; this->area = "System"; };
    if (background.contains("BPM"))        { this->hex_c = "0A"; this->byteSize = "4"; };
    if (background.contains("0~100"))      { this->hex_c = "07"; this->byteSize = "2"; };
    if (background.contains("0~200"))      { this->hex_c = "06"; this->byteSize = "2"; };
    if (background.contains("0~500"))      { this->hex_c = "08"; this->byteSize = "4"; };
    if (background.contains("0~1000"))     { this->hex_c = "09"; this->byteSize = "4"; };
    if (background.contains("RATE"))       { this->hex_c = "08"; this->byteSize = "2"; };
    if (background.contains("ASSIGN"))     { this->hex_c = "04"; this->byteSize = "4"; };
    if (background.contains("SCALE_G"))    { this->hex_c = "75"; this->byteSize = "2"; this->area = "System"; };
    if (background.contains("SCALE_B"))    { this->hex_c = "76"; this->byteSize = "2"; this->area = "System"; };

    Midi items = midiTable->getMidiMap(hex0, hex1, hex2, hex3);
    this->label->setFontRole("setting");
    this->label->setText(items.customdesc);
    this->label->setUpperCase(true);
    this->label->setTextRaise(5);

    this->knob = new customDataKnob(this, hex0, hex1, hex2, hex3, background, this->area);
    this->knob->setWhatsThis(tr("hold down mouse button and drag up/down for quick adjustment")
                             + "<br>" + tr("use scroll wheel or up/down arrow keys for fine adjustment"));

    if(colour.contains("0"))
    {
        this->displayBTS = new QTextEdit(this);
        this->displayBTS->setObjectName("btsdisplayBTS");
        QFont Sfont( "Roboto Condensed", 8*fratio, QFont::Normal);
        this->displayBTS->setFont(Sfont);
        //this->displayBTS->setDisabled(true);
        this->displayBTS->setAlignment(Qt::AlignCenter);
        //this->displayBTS->setFrame(false);

        QLabel *knobbg = new QLabel(this);
        knobbg->setPixmap(QPixmap(":/images/sourceBG.png").scaled(this->width()*2*ratio, this->height()*4*ratio, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

        QStackedLayout *stackLayout = new QStackedLayout;
        stackLayout->setStackingMode(QStackedLayout::StackAll);
        stackLayout->addWidget(this->knob);
        stackLayout->addWidget(this->displayBTS);
        stackLayout->addWidget(knobbg);

        QVBoxLayout *mainLayout = new QVBoxLayout;
        mainLayout->addStretch(1);
        mainLayout->addLayout(stackLayout);
        mainLayout->addWidget(this->label);
        mainLayout->setAlignment(Qt::AlignCenter);

        this->setLayout(mainLayout);
        this->setFixedHeight(60*ratio);

        this->displayBTS->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        this->displayBTS->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }
    else
    {
        this->display = new QLineEdit(this);
        this->display->setObjectName("editdisplay");
        QFont Sfont( "Roboto Condensed", 8*fratio, QFont::Normal);
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
        else if(direction.contains("top"))
        {

        }
        else if(direction.contains("bottom"))
        {
            this->label->setAlignment(Qt::AlignCenter);
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

    hover = false;
    timer = new QTimer(this);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(timed()) );

    setMouseTracking(true);
}

void customControlDataKnob::paintEvent(QPaintEvent *)
{
    //if(colour == "0")
    //{
      /*  this->displayBTS->setAlignment(Qt::AlignCenter);
        if(displayBTS->document()->toPlainText().contains(":")){this->displayBTS->move(3*ratio, 4*ratio);}
        else if(displayBTS->document()->toPlainText().contains("kHz")){this->displayBTS->move(3*ratio, 6*ratio);}
        else if(displayBTS->document()->toPlainText().contains("Hz")){this->displayBTS->move(3*ratio, 6*ratio);}
        else if(displayBTS->document()->toPlainText().contains("cm")){this->displayBTS->move(3*ratio, 6*ratio);}
        else if(displayBTS->document()->toPlainText().contains("ms")){this->displayBTS->move(3*ratio, 6*ratio);}
        else if(displayBTS->document()->toPlainText().contains("dB")){this->displayBTS->move(3*ratio, 6*ratio);}
        else if(displayBTS->document()->toPlainText().contains("mm")){this->displayBTS->move(3*ratio, 6*ratio);}
        else if(displayBTS->document()->toPlainText().contains("User")){this->displayBTS->move(4*ratio, 7*ratio);}
        else if(displayBTS->document()->toPlainText().contains("Preset")){this->displayBTS->move(4*ratio, 7*ratio);}
        else if(displayBTS->document()->toPlainText().contains(" BPM")){this->displayBTS->move(3*ratio, 2*ratio);}
        else{this->displayBTS->move(4*ratio, 10*ratio);};

        this->label->setAlignment(Qt::AlignHCenter);
        this->knob->move(((this->width()/2)-(knob->width()/2))-(3*ratio), 5*ratio);*/
    //};
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

void customControlDataKnob::mouseMoveEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    if(this->underMouse() ){this->knob->knob->setFocus(); hover=true;  repaint(); this->timer->start(500);};
    this->knob->knob->setFocus(); hover=true;  repaint(); this->timer->start(500);
}

void customControlDataKnob::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    if(this->underMouse() ){this->knob->knob->setFocus(); hover=true;  repaint(); this->timer->start(500);};
    this->knob->knob->setFocus(); hover=true;  repaint(); this->timer->start(500);
}

void customControlDataKnob::dialogUpdateSignal()
{
    SysxIO *sysxIO = SysxIO::Instance();
    bool ok;
    int value = sysxIO->getSourceValue(this->hex0, this->hex1, this->hex2, this->hex3);        // read first byte target value as integer from sysx.
    QString valueHex = QString::number(value, 16).toUpper();
    QString next_Hex = QString::number( (hex3.toInt(&ok, 16) + 1), 16).toUpper(); if(next_Hex.size()<2){ next_Hex.prepend("0"); };
    value = sysxIO->getSourceValue(this->hex0, this->hex1, this->hex2, next_Hex);
    valueHex.append( QString::number( value, 16).toUpper() );

    if (this->byteSize == "3" || this->byteSize == "4")
    {
        next_Hex = QString::number( (hex3.toInt(&ok, 16) + 2), 16).toUpper(); if(next_Hex.size()<2){ next_Hex.prepend("0"); };
        value = sysxIO->getSourceValue(this->hex0, this->hex1, this->hex2, next_Hex);
        valueHex.append( QString::number( value, 16).toUpper() );
    };

    if (this->byteSize == "4")
    {

        next_Hex = QString::number( (hex3.toInt(&ok, 16) + 3), 16).toUpper(); if(next_Hex.size()<2){ next_Hex.prepend("0"); };
        value = sysxIO->getSourceValue(this->hex0, this->hex1, this->hex2, next_Hex);
        valueHex.append( QString::number( value, 16).toUpper() );
    };

    value = valueHex.toInt(&ok, 16);
    if(value != _lastValue)
    {
        _lastValue = value;
        this->knob->setValue(value);

        MidiTable *midiTable = MidiTable::Instance();
        QString valueStr = midiTable->getValue("Tables", hex_a, hex_b, hex_c, valueHex);

        if(colour.contains("0"))
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
        };

        emit updateDisplay(valueStr);
    };
}

void customControlDataKnob::timed()
{
    if(this->underMouse() || this->knob->knob->hasFocus())
    {
        this->timer->stop();
        this->timer->start(500);

    }
    else
    {
        this->timer->stop();
        this->hover=false;
        this->repaint();
        this->knob->knob->clearFocus();
    };
}

void customControlDataKnob::highlight()
{
    this->hover=true;
    this->repaint();
    this->timer->start(500);
}
