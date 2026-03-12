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

#include "customControlLabel.h"
#include "Preferences.h"

customControlLabel::customControlLabel(QWidget *parent)
	: QWidget(parent)
{
    Preferences *preferences = Preferences::Instance();
    bool ok;
    const double ratio = preferences->getPreferences("Window", "Scale", "ratio").toDouble(&ok);
    const double fratio = preferences->getPreferences("Window", "Font", "ratio").toDouble(&ok);

	this->label = new QLabel(this);
	this->uppercase = false;
	this->button = false;
	this->isImage = false;
	this->offset = 0;
    this->textWidth = 0;
    this->fontRole = "detail";
    this->fontPointDelta = 0;
    this->textRaise = 0;

	this->label->setObjectName("customlabel");
    this->label->setWordWrap(false);
    Q_UNUSED(fratio);
    applyFont();

	QHBoxLayout *layout = new QHBoxLayout;
	layout->addWidget(this->label);

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addLayout(layout );
    mainLayout->setContentsMargins(0, 0, 0, 0);
	this->setLayout(mainLayout);
    Q_UNUSED(ratio);
    updateControlHeight();
}

void customControlLabel::paintEvent(QPaintEvent *)
{
	if(button && isImage)
	{
        QRectF target(0.0, 0.0, this->image.width(), this->height());
		QRectF source(0.0, this->offset, this->image.width(), this->height());

		QPainter painter(this);
		painter.drawPixmap(target, this->image, source);
	}
}

void customControlLabel::setOffset(int imageNr)
{
	this->offset = imageNr * this->height();
	this->update();
}

void customControlLabel::setUpperCase(bool active)
{
	this->uppercase = active;
	this->setText(this->label->text());
}

void customControlLabel::setAlignment(Qt::Alignment flag)
{
	this->label->setAlignment(flag);
}

void customControlLabel::setFontRole(const QString &role)
{
    this->fontRole = role;
    applyFont();
    setSize();
}

void customControlLabel::applyFont()
{
    Preferences *preferences = Preferences::Instance();
    bool ok;
    const double fratio = preferences->getPreferences("Window", "Font", "ratio").toDouble(&ok);

    int basePointSize = 8;
    if(this->fontRole == "setting")
    {
        basePointSize = 11;
    }
    else if(this->fontRole == "detail")
    {
        basePointSize = 10;
    }
    else if(this->fontRole == "compact")
    {
        basePointSize = 8;
    }
    else if(this->fontRole == "header")
    {
        basePointSize = 12;
    }
#ifdef Q_OS_MAC
    if(this->fontRole == "setting")
    {
        basePointSize = 12;
    }
    else if(this->fontRole == "detail")
    {
        basePointSize = 11;
    }
    else if(this->fontRole == "header")
    {
        basePointSize = 13;
    }
    else
    {
        basePointSize = 9;
    }
#endif
    QFont font("Roboto Condensed", qMax(1, basePointSize + this->fontPointDelta) * fratio, QFont::Normal);
    this->label->setFont(font);
    updateControlHeight();
}

void customControlLabel::setFontPointDelta(int delta)
{
    this->fontPointDelta = delta;
    applyFont();
    setSize();
}

void customControlLabel::setTextWidth(int pixels)
{
    this->textWidth = qMax(0, pixels);
    if(this->textWidth > 0)
    {
        this->label->setWordWrap(true);
        this->label->setFixedWidth(this->textWidth);
        this->setFixedWidth(this->textWidth);
    }
    else
    {
        this->label->setWordWrap(false);
        this->label->setMinimumWidth(0);
        this->label->setMaximumWidth(QWIDGETSIZE_MAX);
        this->setMinimumWidth(0);
        this->setMaximumWidth(QWIDGETSIZE_MAX);
    }
    setSize();
}

void customControlLabel::setTextRaise(int pixels)
{
    this->textRaise = qMax(0, pixels);
    this->label->setContentsMargins(0, 0, 0, this->textRaise);
    updateControlHeight();
}

void customControlLabel::setText(QString text)
{
	if(this->uppercase)
	{
        this->label->setText(text/*.toUpper()*/);
	}
	else
	{
		this->label->setText(text);
	};

	setSize();
}

void customControlLabel::setSize()
{
    QFontMetrics metrics(this->label->font());
    int pixelWidth = 0;
    if(this->textWidth > 0)
    {
        const QRect wrapped = metrics.boundingRect(QRect(0, 0, this->textWidth, 1000),
                                                   Qt::TextWordWrap | Qt::AlignHCenter,
                                                   this->label->text());
        pixelWidth = this->textWidth;
        this->label->setFixedHeight(qMax(1, wrapped.height()));
    }
    else
    {
        pixelWidth = metrics.horizontalAdvance(this->label->text());
        this->label->setMinimumHeight(0);
        this->label->setMaximumHeight(QWIDGETSIZE_MAX);
    }
	if(pixelWidth<1) {pixelWidth = 1; };
	this->labelWidth = pixelWidth;
    updateControlHeight();
}

void customControlLabel::updateControlHeight()
{
    Preferences *preferences = Preferences::Instance();
    bool ok;
    const double ratio = preferences->getPreferences("Window", "Scale", "ratio").toDouble(&ok);
    int textHeight = QFontMetrics(this->label->font()).height() + 2;
    if(this->textWidth > 0)
    {
        const QRect wrapped = QFontMetrics(this->label->font()).boundingRect(QRect(0, 0, this->textWidth, 1000),
                                                                             Qt::TextWordWrap | Qt::AlignHCenter,
                                                                             this->label->text());
        textHeight = wrapped.height() + 2;
    }
    textHeight += this->textRaise;
    this->setFixedHeight(qMax(int(12*ratio), textHeight));
}

int customControlLabel::getLabelWidth()
{
	return this->labelWidth;
}

void customControlLabel::setButton(bool button)
{
	this->button = button;
}

void customControlLabel::setImage(QString imagePath)
{
    Preferences *preferences = Preferences::Instance();
    bool ok;
    const double ratio = preferences->getPreferences("Window", "Scale", "ratio").toDouble(&ok);

    this->image = QPixmap(imagePath);
    this->image = this->image.scaled(this->image.width()*ratio, this->image.height()*ratio, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	this->isImage = true;
	this->imageHeight = this->image.height() / 4;
	if(button)
	{
        this->setFixedSize(this->image.width(), this->imageHeight);
		setOffset(0);
	}
	else
	{
        this->setFixedSize(this->image.width()*ratio, this->image.height()*ratio);
        this->label->setPixmap(this->image.scaled(this->image.width()*ratio, this->image.height()*ratio, Qt::KeepAspectRatio, Qt::SmoothTransformation));
	};
}

void customControlLabel::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
	if(isImage && button)
	{
		setOffset(3);
	}
	emit mousePressed();
}

void customControlLabel::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
	if(isImage && button)
	{
		setOffset(2);
	}
	emit mouseReleased();
}

/*void customControlLabel::enterEvent(QEvent *event)
{
    Q_UNUSED(event);
	if(button)
	{
		if(isImage)
		{
			setOffset(1);
		}
		else
		{

		};
	};
}

void customControlLabel::leaveEvent( QEvent *event)
{
    Q_UNUSED(event);
	if(button)
	{
		if(isImage)
		{
			setOffset(0);
		}
		else
		{

		};
	};
}*/
