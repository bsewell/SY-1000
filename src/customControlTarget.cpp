/****************************************************************************
**
** Copyright (C) 2007~2025 Colin Willcocks.
** Copyright (C) 2005~2007 Uco Mesdag.
** All rights reserved.
** This file is part of "SY-1000B FloorBoard".
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

#include "customControlTarget.h"
#include "MidiTable.h"
#include "SysxIO.h"
#include "Preferences.h"

customControlTarget::customControlTarget(QWidget *parent,
                                         QString hex0, QString hex1, QString hex2, QString hex3,
                                         QString background, QString direction, int lenght)
    : QWidget(parent)
{
    Q_UNUSED(background);
    Q_UNUSED(direction);
    Preferences *preferences = Preferences::Instance();
    bool ok;
    const double ratio = preferences->getPreferences("Window", "Scale", "ratio").toDouble(&ok);
    const double fratio = preferences->getPreferences("Window", "Font", "ratio").toDouble(&ok);
    const int verticalGap = qRound(5 * ratio);
    const int controlMargin = qRound(4 * ratio);
    QFont Sfont( "Roboto Condensed", 9*fratio, QFont::Normal);
    if(preferences->getPreferences("Window", "BassMode", "bool")=="true"){mode = "02";}else{mode = "00";};

    this->displayMin = new QLineEdit(this);
    this->displayMax = new QLineEdit(this);
    this->label = new customControlLabel(this);
    this->labelMin = new customControlLabel(this);
    this->labelMax = new customControlLabel(this);
    this->hex0 = hex0;
    this->hex1 = hex1;
    this->hex2 = hex2;
    this->hex3 = hex3;

    //bool ok;                                                                          // Target = hex3, hex3_1, hex3_2, hex3_3
    this->hexMin = QString::number((hex3.toInt(&ok, 16) + 4), 16).toUpper();             // go forward 4 to select target Min address
    if(this->hexMin.length() < 2) this->hexMin.prepend("0");                             // prepend with "0" if single digit.

    this->hexMax = QString::number((hex3.toInt(&ok, 16) + 8), 16).toUpper();             // go forward 8 to select target Max address
    if(this->hexMax.length() < 2){ this->hexMax.prepend("0"); };                         // prepend with "0" if single digit.

    this->hex3_1 = QString::number((hex3.toInt(&ok, 16) + 1), 16).toUpper();           // go forward 1 to select target hex3_1 address
    if(this->hex3_1.length() < 2) this->hex3_1.prepend("0");                         // prepend with "0" if single digit.

    this->hex3_2 = QString::number((hex3.toInt(&ok, 16) + 2), 16).toUpper();           // go forward 2 to select target hex3_2 address
    if(this->hex3_2.length() < 2) this->hex3_2.prepend("0");

    this->hex3_3 = QString::number((hex3.toInt(&ok, 16) + 3), 16).toUpper();           // go forward 3 to select target hex3_3 address
    if(this->hex3_3.length() < 2) this->hex3_3.prepend("0");


    SysxIO *sysxIO = SysxIO::Instance();

    int value = sysxIO->getSourceValue(this->hex0, this->hex1, this->hex2, this->hex3);        // read target value as integer from sysx.
    QString valueHex = QString::number(value, 16).toUpper();                                    // convert to hex qstring.
    value = sysxIO->getSourceValue(this->hex0, this->hex1, this->hex2, this->hex3_1);        // read target value as integer from sysx.
    valueHex.append(QString::number(value, 16).toUpper());
    value = sysxIO->getSourceValue(this->hex0, this->hex1, this->hex2, this->hex3_2);        // read target value as integer from sysx.
    valueHex.append(QString::number(value, 16).toUpper());
    value = sysxIO->getSourceValue(this->hex0, this->hex1, this->hex2, this->hex3_3);        // read target value as integer from sysx.
    valueHex.append(QString::number(value, 16).toUpper());

    this->displayCombo = new customTargetListMenu(this, this->hex0, this->hex1, this->hex2, this->hex3, this->hex3_1, this->hex3_2, this->hex3_3);
    MidiTable *midiTable = MidiTable::Instance();

    QString valueStr = midiTable->getValue("Tables", "00", "00", mode, valueHex);

    int maxRange = 256;
    value = valueStr.toInt(&ok, 16);
    int dif = value/maxRange;
    QString valueHex1 = QString::number(dif, 16).toUpper();
    if(valueHex1.length() < 2) valueHex1.prepend("0");
    QString valueHex2 = QString::number(value - (dif * maxRange), 16).toUpper();
    if(valueHex2.length() < 2) valueHex2.prepend("0");
    this->hex4 = valueHex1;
    this->hex5 = valueHex2;                                                                     //convert valueStr to 7bit hex4, hex5
    Midi items = midiTable->getMidiMap("Tables", "00", "00", mode, hex4, hex5);
    this->hexMsb = items.desc;
    this->hexLsb = items.customdesc;

    //this->knobTarget = new customKnobTarget(this, hex0, hex1, hex2, hex3, hexMsb, hexLsb, "target");  // create knob with target address

    this->label->setText("TARGET");
    this->label->setFontRole("setting");
    this->label->setAlignment(Qt::AlignCenter);

    this->knobMin = new customKnobTarget(this, hex0, hex1, hex2, hexMin, hexMsb, hexLsb, "min");      // create knob with target address
    this->displayMin->setObjectName("editdisplay");
    this->displayMin->setFont(Sfont);
    this->displayMin->setFixedWidth(lenght*ratio);
    this->displayMin->setFixedHeight(20*ratio);
    this->displayMin->setAlignment(Qt::AlignCenter);
    this->displayMin->setDisabled(true);
    this->labelMin->setText("MINIMUM");
    this->labelMin->setFontRole("setting");
    this->labelMin->setAlignment(Qt::AlignCenter);

    this->knobMax = new customKnobTarget(this, hex0, hex1, hex2, hexMax, hexMsb, hexLsb, "max");      // create knob with target address
    this->displayMax->setObjectName("editdisplay");
    this->displayMax->setFont(Sfont);
    this->displayMax->setFixedWidth(lenght*ratio);
    this->displayMax->setFixedHeight(20*ratio);
    this->displayMax->setAlignment(Qt::AlignCenter);
    this->displayMax->setDisabled(true);
    this->labelMax->setText("MAXIMUM");
    this->labelMax->setFontRole("setting");
    this->labelMax->setAlignment(Qt::AlignCenter);

    QVBoxLayout *targetLayout = new QVBoxLayout;
    targetLayout->setContentsMargins(controlMargin, verticalGap, controlMargin, verticalGap);
    targetLayout->setSpacing(verticalGap);
    targetLayout->addWidget(this->label, 0, Qt::AlignCenter);
    targetLayout->addSpacing(qRound(3*ratio));
    //targetLayout->addWidget(this->knobTarget, 0, Qt::AlignCenter);
    targetLayout->addWidget(this->displayCombo, 0, Qt::AlignCenter);
    targetLayout->addStretch(0);

    QVBoxLayout *minLayout = new QVBoxLayout;
    minLayout->setContentsMargins(controlMargin, verticalGap, controlMargin, verticalGap);
    minLayout->setSpacing(verticalGap);
    minLayout->addWidget(this->labelMin, 0, Qt::AlignCenter);
    minLayout->addWidget(this->knobMin, 0, Qt::AlignCenter);
    minLayout->addWidget(this->displayMin, 0, Qt::AlignCenter);
    minLayout->addStretch(0);

    QVBoxLayout *maxLayout = new QVBoxLayout;
    maxLayout->setContentsMargins(controlMargin, verticalGap, controlMargin, verticalGap);
    maxLayout->setSpacing(verticalGap);
    maxLayout->addWidget(this->labelMax, 0, Qt::AlignCenter);
    maxLayout->addWidget(this->knobMax, 0, Qt::AlignCenter);
    maxLayout->addWidget(this->displayMax, 0, Qt::AlignCenter);
    maxLayout->addStretch(0);

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->setContentsMargins(controlMargin, 0, controlMargin, 0);
    mainLayout->setSpacing(verticalGap);
    mainLayout->addLayout(targetLayout);
    mainLayout->addLayout(minLayout);
    mainLayout->addLayout(maxLayout);
    mainLayout->addStretch(0);

    this->setLayout(mainLayout);
    //this->setFixedHeight((this->knobTarget->height() + 20 + 12)*ratio);
    //this->setFixedHeight((this->knobMin->height() + 20 + 12)*ratio);
    //this->setFixedHeight((this->knobMax->height() + 20 + 12)*ratio);

    QObject::connect(this->parent(), SIGNAL( dialogUpdateSignal() ),
                     this, SLOT( dialogUpdateSignal() ));

    QObject::connect(this, SIGNAL( updateSignal() ),
                     this->parent(), SIGNAL( updateSignal() ));

    QObject::connect(this, SIGNAL( updateDisplayMin(QString) ),
                     this->displayMin, SLOT( setText(QString) ));

    QObject::connect(this, SIGNAL( updateDisplayMax(QString) ),
                     this->displayMax, SLOT( setText(QString) ));

    QObject::connect(this, SIGNAL( updateTarget(QString, QString, QString, QString) ),
                     this, SLOT( knobSignal(QString, QString, QString, QString) ));

    QObject::connect(this, SIGNAL( updateTarget(QString, QString, QString, QString) ),
                     this, SIGNAL( updateHex(QString, QString, QString, QString) ));

    //knobTarget->hide();

    hover = false;
    timer = new QTimer(this);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(timed()) );

    setMouseTracking(true);
}

void customControlTarget::paintEvent(QPaintEvent *)
{
    Preferences *preferences = Preferences::Instance();
    bool ok;
    const double ratio = preferences->getPreferences("Window", "Scale", "ratio").toDouble(&ok);

    /* DRAWS RED BACKGROUND  */
    if(hover==true && this->id.contains("min")){
        QPixmap image(":/images/dragbar.png");
        QRectF target(434.0*ratio, 0.0, 142.0*ratio, this->height());
        QRectF source(0.0, 0.0, 600, 600);
        QPainter painter(this);
        painter.drawPixmap(target, image, source);
    }
    else if(hover==true && this->id.contains("max")){
        QPixmap image(":/images/dragbar.png");
        QRectF target(576.0*ratio, 0.0, 142.0*ratio, this->height());
        QRectF source(0.0, 0.0, 600, 600);
        QPainter painter(this);
        painter.drawPixmap(target, image, source);
    };
}

void customControlTarget::mouseMoveEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    if(this->knobMin->knob->underMouse()){this->knobMin->knob->setFocus(); this->id = "min"; hover=true;  repaint(); this->timer->start(500);};
    if(this->knobMax->knob->underMouse()){this->knobMax->knob->setFocus(); this->id = "max"; hover=true;  repaint(); this->timer->start(500);};
}

void customControlTarget::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    if(this->knobMin->knob->underMouse()){this->knobMin->knob->setFocus(); this->id = "min"; hover=true;  repaint(); this->timer->start(500);};
    if(this->knobMax->knob->underMouse()){this->knobMax->knob->setFocus(); this->id = "max"; hover=true;  repaint(); this->timer->start(500);};
}

void customControlTarget::knobSignal(QString hex0, QString hex1, QString hexMsb, QString hexLsb)
{
    Q_UNUSED(hex0);
    Q_UNUSED(hex1);
    // Set value for TARGET Knob

    this->hexMsb = hexMsb;
    this->hexLsb = hexLsb;

      //Set value for Target MIN
    MidiTable *midiTable = MidiTable::Instance();
    int value = midiTable->getRangeMinimum("Tables", "00", "00", hexLsb);
    this->knobMin->setValue(value);                                                             // sets knob initial position
    QString valueHex = QString::number(value, 16).toUpper();
    if(valueHex.length() < 2) valueHex.prepend("0");
    QString valueStr = midiTable->getValue("Tables", "00", "00", hexLsb, valueHex);
    emit updateDisplayMin(valueStr);                                                            // initial value only is displayed under knob

    // Set Value for Target MAX
    value = midiTable->getRange("Tables", "00", "00", hexLsb);
    this->knobMax->setValue(value);                                                             // sets knob initial position
    valueHex = QString::number(value, 16).toUpper();
    if(valueHex.length() < 2) valueHex.prepend("0");
    valueStr = midiTable->getValue("Tables", "00", "00", hexLsb, valueHex);
    emit updateDisplayMax(valueStr);

    //this->dialogUpdateSignal();                                                                     // initial value only is displayed under knob
}

void customControlTarget::dialogUpdateSignal()
{
    bool ok;
    SysxIO *sysxIO = SysxIO::Instance();

    QString addr0 = QString::number((this->hex3.toInt(&ok, 16)+0), 16); if(addr0.length()<2){addr0.prepend("0");};
    QString valueHex = QString::number(sysxIO->getSourceValue(this->hex0, this->hex1, this->hex2, addr0), 16).toUpper();

    QString addr1 = QString::number((this->hex3.toInt(&ok, 16)+1), 16); if(addr1.length()<2){addr1.prepend("0");};
    valueHex.append(QString::number(sysxIO->getSourceValue(this->hex0, this->hex1, this->hex2, addr1), 16).toUpper());

    QString addr2 = QString::number((this->hex3.toInt(&ok, 16)+2), 16); if(addr2.length()<2){addr2.prepend("0");};
    valueHex.append(QString::number(sysxIO->getSourceValue(this->hex0, this->hex1, this->hex2, addr2), 16).toUpper());

    QString addr3 = QString::number((this->hex3.toInt(&ok, 16)+3), 16); if(addr3.length()<2){addr3.prepend("0");};
    valueHex.append(QString::number(sysxIO->getSourceValue(this->hex0, this->hex1, this->hex2, addr3), 16).toUpper());

    MidiTable *midiTable = MidiTable::Instance();
    QString valueStr;
    QString MinVal;

    int value = valueHex.toInt(&ok, 16);
    if(value != _last_target){
        _last_target = value;
        //if(value>1999){value=1999;};
        QString conv = QString::number(value, 16).toUpper();
        //this->knobTarget->setValue(value);

        /*QString m = "DialogUpdate value = " + conv + " valueHex = "+valueHex;
         std::string st = m.toStdString();
         std::cerr << st << std::endl;*/



        valueStr = midiTable->getValue("Tables", "00", "00", mode, conv);          // lookup the target values

        int maxRange = 256;
        value = conv.toInt(&ok, 16);
        this->displayCombo->controlListComboBox->setCurrentIndex(value);

        int dif = value/maxRange;
        QString valueHex1 = QString::number(dif, 16).toUpper();
        if(valueHex1.length() < 2) valueHex1.prepend("0");
        QString valueHex2 = QString::number(value - (dif * maxRange), 16).toUpper();
        if(valueHex2.length() < 2) valueHex2.prepend("0");
        this->hex4 = valueHex1;
        this->hex5 = valueHex2;                                     //convert valueStr to 7bit hex4, hex5
        Midi items = midiTable->getMidiMap("Tables", "00", "00", mode, hex4, hex5);
        this->hexLsb = items.customdesc;
        emit updateHex("Tables", "00", "00", hexLsb);

        /*QString m = " hex4 = " + hex4 + " hex5 = "+hex5+ " hexLsb = "+hexLsb;
         std::string st = m.toStdString();
         std::cerr << st << std::endl;*/

        //emit knobSignal("00", this->hex2, hexLsb);
    };
    //////////////////////////
    value = this->hexMin.toInt(&ok, 16) +0;
    QString hexMin_0 = QString::number(value, 16).toUpper();
    if(hexMin_0.length() < 2) hexMin_0.prepend("0");

    value = this->hexMin.toInt(&ok, 16) +1;
    QString hexMin_1 = QString::number(value, 16).toUpper();
    if(hexMin_1.length() < 2) hexMin_1.prepend("0");

    value = this->hexMin.toInt(&ok, 16) +2;
    QString hexMin_2 = QString::number(value, 16).toUpper();
    if(hexMin_2.length() < 2) hexMin_2.prepend("0");

    value = this->hexMin.toInt(&ok, 16) +3;
    QString hexMin_3 = QString::number(value, 16).toUpper();
    if(hexMin_3.length() < 2) hexMin_3.prepend("0");

    value = sysxIO->getSourceValue(this->hex0, this->hex1, this->hex2, hexMin_0);      // read target value as integer from sysx.
    valueHex = QString::number(value, 16).toUpper();

    value = sysxIO->getSourceValue(this->hex0, this->hex1, this->hex2, hexMin_1);      // read target value as integer from sysx.
    valueHex.append(QString::number(value, 16).toUpper());

    value = sysxIO->getSourceValue(this->hex0, this->hex1, this->hex2, hexMin_2);      // read target value as integer from sysx.
    valueHex.append(QString::number(value, 16).toUpper());

    value = sysxIO->getSourceValue(this->hex0, this->hex1, this->hex2, hexMin_3);      // read target value as integer from sysx.
    valueHex.append(QString::number(value, 16).toUpper());

    value = (valueHex.toInt(&ok, 16));
    if(value != _last_min){
        _last_min = value;
        MinVal = valueHex;

        int minValue = midiTable->getRangeMinimum("Tables", "00", "00", hexLsb);
        if(value<minValue){value=minValue;};
        int maxValue = midiTable->getRange("Tables", "00", "00", hexLsb);
        if(value>maxValue){value=maxValue;};
        this->knobMin->setValue(value);   // sets knob initial position
        if(!midiTable->isData("Tables", "00", "00", hexLsb) && !midiTable->isRange("Tables", "00", "00", hexLsb)) {
            value = value - midiTable->getRangeMinimum("Tables", "00", "00", hexLsb); };
        valueHex = QString::number(value, 16).toUpper();
        if(valueHex.length() < 2) valueHex.prepend("0");
        valueStr = midiTable->getValue("Tables", "00", "00", hexLsb, valueHex);// };
        emit updateDisplayMin(valueStr);   // initial value only is displayed under knob
        QString knobmin = valueStr;
    };
    ////////////////////////////////////
    value = this->hexMax.toInt(&ok, 16) +0;
    QString hexMax_0 = QString::number(value, 16).toUpper();
    if(hexMax_0.length() < 2) hexMax_0.prepend("0");

    value = this->hexMax.toInt(&ok, 16) +1;
    QString hexMax_1 = QString::number(value, 16).toUpper();
    if(hexMax_1.length() < 2) hexMax_1.prepend("0");

    value = this->hexMax.toInt(&ok, 16) +2;
    QString hexMax_2 = QString::number(value, 16).toUpper();
    if(hexMax_2.length() < 2) hexMax_2.prepend("0");

    value = this->hexMax.toInt(&ok, 16) +3;
    QString hexMax_3 = QString::number(value, 16).toUpper();
    if(hexMax_3.length() < 2) hexMax_3.prepend("0");

    value = sysxIO->getSourceValue(this->hex0, this->hex1, this->hex2, hexMax_0);      // read target value as integer from sysx.
    if(value != _last_max){
        _last_max = value;
        valueHex = QString::number(value, 16).toUpper();

        value = sysxIO->getSourceValue(this->hex0, this->hex1, this->hex2, hexMax_1);      // read target value as integer from sysx.
        valueHex.append(QString::number(value, 16).toUpper());

        value = sysxIO->getSourceValue(this->hex0, this->hex1, this->hex2, hexMax_2);      // read target value as integer from sysx.
        valueHex.append(QString::number(value, 16).toUpper());

        value = sysxIO->getSourceValue(this->hex0, this->hex1, this->hex2, hexMax_3);      // read target value as integer from sysx.
        valueHex.append(QString::number(value, 16).toUpper());

        value = (valueHex.toInt(&ok, 16));
        int minValue = midiTable->getRangeMinimum("Tables", "00", "00", hexLsb);
        if(value<minValue){value=minValue;};
        int maxValue = midiTable->getRange("Tables", "00", "00", hexLsb);
        if(value>maxValue){value=maxValue;};
        this->knobMax->setValue(value);   // sets knob initial position
        if(!midiTable->isData("Tables", "00", "00", hexLsb) && !midiTable->isRange("Tables", "00", "00", hexLsb))
        { value = value - midiTable->getRangeMinimum("Tables", "00", "00", hexLsb); };
        valueHex = QString::number(value, 16).toUpper();
        if(valueHex.length() < 2) valueHex.prepend("0");
        valueStr = midiTable->getValue("Tables", "00", "00", hexLsb, valueHex);
        emit updateDisplayMax(valueStr);

        /*QString m = " valueMin = " + MinVal+" knobMin = "+knobmin+ " valueMax = "+ valueHex+" knobMax = "+valueStr;
         std::string st = m.toStdString();
         std::cerr << st << std::endl;*/
    };
}

void customControlTarget::timed()
{
    if(this->underMouse() || this->knobMin->knob->hasFocus() || this->knobMax->knob->hasFocus())
    {
        this->timer->stop();
        this->timer->start(500);

    }else{
        this->timer->stop();
        this->hover=false;
        this->repaint();
        this->knobMin->knob->clearFocus();
    };
}

void customControlTarget::highlight(QString id)
{
    this->id = id;
    this->hover=true;
    this->repaint();
    this->timer->start(500);
}
