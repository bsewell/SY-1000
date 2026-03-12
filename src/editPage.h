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

#ifndef EDITPAGE_H
#define EDITPAGE_H

#include <QWidget>
#include <QtWidgets>

class editPage : public QWidget
{
    Q_OBJECT

public:
    editPage(QWidget *parent = 0);
    bool hasPowerControl() const;
    QString powerAddress0() const;
    QString powerAddress1() const;
    QString powerAddress2() const;
    QString powerAddress3() const;
    void hideInlinePowerControl();
    void clearPowerControl();
    void setExternalPowerAddress(QString hex0, QString hex1, QString hex2, QString hex3);
    
    void addParaEQ(int row, int column, int rowSpan, int columnSpan,
                   QString hex0 = "void",
                   QString hex1 = "void",
                   QString hex2 = "void",
                   QString hex3 = "void",
                   QString background = "normal",
                   QString direction = "bottom",
                   int lenght = 55,
                   Qt::Alignment alignment = Qt::AlignCenter);
    void addKnob(int row, int column, int rowSpan, int columnSpan,
                 QString hex0 = "void",
                 QString hex1 = "void",
                 QString hex2 = "void",
                 QString hex3 = "void",
                 QString background = "normal",
                 QString direction = "bottom",
                 int lenght = 45,
                 Qt::Alignment alignment = Qt::AlignCenter);
    void addDataKnob(int row, int column, int rowSpan, int columnSpan,
                     QString hex0 = "void",
                     QString hex1 = "void",
                     QString hex2 = "void",
                     QString hex3 = "void",
                     QString background = "normal",
                     QString direction = "bottom",
                     int lenght = 45,
                     Qt::Alignment alignment = Qt::AlignCenter);
    void addTarget(int row, int column, int rowSpan, int columnSpan,
                   QString hex0 = "void",
                   QString hex1 = "void",
                   QString hex2 = "void",
                   QString hex3 = "void",
                   QString background = "normal",
                   QString direction = "bottom",
                   int lenght = 45,
                   Qt::Alignment alignment = Qt::AlignCenter);
    void addRange(int row, int column, int rowSpan, int columnSpan,
                  QString hex0 = "void",
                  QString hex1 = "void",
                  QString hex2 = "void",
                  QString hex3 = "void",
                  QString area = "Structure",
                  Qt::Alignment alignment = Qt::AlignCenter);
    void addSwitch(int row, int column, int rowSpan, int columnSpan,
                   QString hex0 = "void",
                   QString hex1 = "void",
                   QString hex2 = "void",
                   QString hex3 = "void",
                   QString direction = "bottom",
                   Qt::Alignment alignment = Qt::AlignCenter);
    void addComboBox(int row, int column, int rowSpan, int columnSpan,
                     QString hex0 = "void",
                     QString hex1 = "void",
                     QString hex2 = "void",
                     QString hex3 = "void",
                     QString direction = "bottom",
                     Qt::Alignment alignment = Qt::AlignCenter);
    void addTabBar(int row, int column, int rowSpan, int columnSpan,
                   QString hex0 = "void",
                   QString hex1 = "void",
                   QString hex2 = "void",
                   QString hex3 = "void",
                   QString direction = "bottom",
                   Qt::Alignment alignment = Qt::AlignCenter);
    void addStaticTabBar(int row, int column, int rowSpan, int columnSpan,
                         const QStringList &labels,
                         const QList<int> &mappedIndexes = QList<int>(),
                         QString direction = "bottom",
                         Qt::Alignment alignment = Qt::AlignCenter);
    void addMultiComboBox(int row, int column, int rowSpan, int columnSpan,
                          QString hex1 = "void",
                          QString hex2 = "void",
                          QString hex3 = "void",
                          QString direction = "bottom",
                          Qt::Alignment alignment = Qt::AlignCenter);
    void addSpinBox(int row, int column, int rowSpan, int columnSpan,
                    QString hex0 = "void",
                    QString hex1 = "void",
                    QString hex2 = "void",
                    QString hex3 = "void",
                    QString direction = "bottom",
                    Qt::Alignment alignment = Qt::AlignCenter);
    void addNameEdit(int row, int column, int rowSpan, int columnSpan,
                     QString hex0 = "void",
                     QString hex1 = "void",
                     QString hex2 = "void",
                     QString hex3 = "void",
                     QString length = "8",
                     Qt::Alignment alignment = Qt::AlignCenter);
    void addSystemOverride(int row, int column, int rowSpan, int columnSpan,
                           QString hex0 = "void",
                           QString hex1 = "void",
                           QString hex2 = "void",
                           QString hex3 = "void",
                           QString index = "01",
                           QString type = "equals",
                           QString hex_0 = "void",
                           QString hex_1 = "void",
                           QString hex_2 = "void",
                           QString hex_3 = "void",
                           QString value = "void");
    void addVU(int row, int column, int rowSpan, int columnSpan,
                 QString hex0 = "void",
                 QString hex1 = "void",
                 QString hex2 = "void",
                 QString hex3 = "void",
                 QString background = "normal",
                 QString direction = "bottom",
                 int lenght = 45,
                 Qt::Alignment alignment = Qt::AlignCenter);
    void addLabel(int row, int column, int rowSpan, int columnSpan, QString text, Qt::Alignment alignment = Qt::AlignCenter);
    void addDivider(int row, int column, int rowSpan, int columnSpan, Qt::Alignment alignment = Qt::AlignVCenter | Qt::AlignLeft);
    void newGroupBox(QString title, Qt::Alignment alignment = Qt::AlignTop | Qt::AlignLeft);
    void newGroupBox(QString title, QString preset, Qt::Alignment alignment = Qt::AlignTop | Qt::AlignLeft);
    void addGroupBox(int row, int column, int rowSpan, int columnSpan);
    void setFlatGroupBoxes(bool enabled);
    void setGridLayout();
    void newStackControl(int id);
    void linkStackControl(int id);
    void addStackControl();
    void insertStackField(int id,
                          int row, int column, int rowSpan, int columnSpan,
                          Qt::Alignment alignment = Qt::AlignCenter);
    void newStackField(int id, Qt::Alignment alignment = Qt::AlignTop | Qt::AlignLeft);
    void compactCurrentStackField(int horizontalSpacing, int verticalSpacing = -1);
    void setStackCurrentIndex(int id, int index);
    void addStackField();

protected:
    void paintEvent(QPaintEvent *event);

public slots:
    void valueChanged(bool value, QString hex1, QString hex2, QString hex3); // Not used.
    void updateDialog(int index);
    void updateDisplay(QString text);
    void syncPowerState();

signals:
    void dialogUpdateSignal();
    void updateSignal();
    void timerUpdateSignal();


private:
    void registerManagedControl(QWidget *widget);
    QGridLayout *targetGridLayout() const;
    void applyControlCellMetrics(QWidget *widget, QGridLayout *grid, int row, int rowSpan,
                                 int column, int columnSpan,
                                 const QString &direction, int minWidth) const;
    int currentIndex;
    QGridLayout* layout;
    QList<QGridLayout*> groupBoxLayouts;
    QList<QGroupBox*> groupBoxes;
    QGridLayout* groupBoxLayout;
    QGroupBox* groupBox;
    QList<int> groupBoxIndexList;
    bool groupBoxMode;
    int groupBoxLevel;
    int groupBoxIndex;
    int parentBoxDif;
    QList<QStackedWidget*> stackedFields;
    QGridLayout* stackField;
    bool stackControlMode;
    bool stackFieldMode;
    int stackFieldId;
    int stackControlId;
    QList<int> stackControlIds;
    bool stackFieldLockWidth;
    QList<int> fieldIndexes;
    int fieldItems;
    QTimer* timer;
    QList<QWidget*> managedControls;
    QList<int> groupBoxMinHeights;
    QWidget *powerSwitchControl = nullptr;
    QString powerHex0 = "void";
    QString powerHex1 = "void";
    QString powerHex2 = "void";
    QString powerHex3 = "void";
    QString externalPowerHex0 = "void";
    QString externalPowerHex1 = "void";
    QString externalPowerHex2 = "void";
    QString externalPowerHex3 = "void";
    bool flatGroupBoxes = false;
    int groupBoxPresetMinimumHeight(const QString &preset) const;
};

#endif // EDITPAGE_H

