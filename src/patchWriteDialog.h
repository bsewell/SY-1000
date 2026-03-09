/****************************************************************************
**
** Copyright (C) 2007~2025 Colin Willcocks. 
** All rights reserved.
**
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

#ifndef PATCHWRITEDIALOG_H
#define PATCHWRITEDIALOG_H

#include <QtWidgets>
#include <QWidget>
#include <QDialog>
#include <QLabel>
#include <QProgressBar>
#include <QSpinBox>
#include <QCheckBox>
#include <QRadioButton>
#include <QPushButton>
#include <QComboBox>
#include <QCursor>
#include "SysxIO.h"
#include "sysxWriter.h"


class patchWriteDialog : public QWidget
{
    Q_OBJECT

public:
    patchWriteDialog();
    ~patchWriteDialog();
    QString msg;
    void listUpdate();

signals:
    void setStatusMessage(QString message);
    void setStatusSymbol(int value);
    void setdeBugStatusMessage(QString message);
    void updateNameSignal(QString, int, int);
    void updateTreeSignal();

public slots:
    void writeToMemory();
    void saveToFile();
    void getTempPatch();

private slots:
    void comboValueChanged(int value);
    void close();

private: 
    QComboBox *patchListCombo;
    QLabel *progressLabel;
    QLabel *bytesLabel;
    QPushButton *writeButton;
    QPushButton *cancelButton;
    QPushButton *save2FileButton;
    int patch;
    int bank;
    QString patchName;
    QList<QString> patchList;
    sysxWriter file;
    QGroupBox *patchListGroup;
};

#endif // patchWriteDialog_H
