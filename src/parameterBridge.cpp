#include "parameterBridge.h"
#include "SysxIO.h"
#include "MidiTable.h"
#include <QQuickItem>

ParameterBridge* ParameterBridge::instance = nullptr;

ParameterBridge::ParameterBridge(QObject *parent)
    : QObject(parent)
{
    SysxIO *sysxIO = SysxIO::Instance();
    connect(sysxIO, &SysxIO::updateSignal, this, &ParameterBridge::dataRefreshed);
    connect(sysxIO, &SysxIO::isChanged,    this, &ParameterBridge::dataRefreshed);
}

ParameterBridge* ParameterBridge::Instance()
{
    if(!instance)
    {
        instance = new ParameterBridge();
    }
    return instance;
}

int ParameterBridge::getValue(const QString &hex0, const QString &hex1,
                              const QString &hex2, const QString &hex3)
{
    SysxIO *sysxIO = SysxIO::Instance();
    return sysxIO->getSourceValue(hex0, hex1, hex2, hex3);
}

void ParameterBridge::setValue(const QString &hex0, const QString &hex1,
                               const QString &hex2, const QString &hex3,
                               int value)
{
    SysxIO *sysxIO = SysxIO::Instance();
    bool ok;
    int range = MidiTable::Instance()->getRange(hex0, hex1, hex2, hex3);

    if(range > 127)
    {
        int maxRange = 128;
        QString hex4 = QString::number(value / maxRange, 16).toUpper().rightJustified(2, '0');
        QString hex5 = QString::number(value % maxRange, 16).toUpper().rightJustified(2, '0');
        sysxIO->setFileSource(hex0, hex1, hex2, hex3, hex4, hex5);
    }
    else
    {
        QString hex4 = QString::number(value, 16).toUpper().rightJustified(2, '0');
        sysxIO->setFileSource(hex0, hex1, hex2, hex3, hex4);
    }

    emit parameterChanged(hex0, hex1, hex2, hex3, value);
}

QString ParameterBridge::getLabel(const QString &hex0, const QString &hex1,
                                  const QString &hex2, const QString &hex3)
{
    MidiTable *midiTable = MidiTable::Instance();
    Midi items = midiTable->getMidiMap(hex0, hex1, hex2, hex3);
    return items.customdesc.isEmpty() ? items.desc : items.customdesc;
}

int ParameterBridge::getMin(const QString &hex0, const QString &hex1,
                            const QString &hex2, const QString &hex3)
{
    MidiTable *midiTable = MidiTable::Instance();
    return midiTable->getRangeMinimum(hex0, hex1, hex2, hex3);
}

int ParameterBridge::getMax(const QString &hex0, const QString &hex1,
                            const QString &hex2, const QString &hex3)
{
    MidiTable *midiTable = MidiTable::Instance();
    return midiTable->getRange(hex0, hex1, hex2, hex3);
}

QString ParameterBridge::getDisplayValue(const QString &hex0, const QString &hex1,
                                         const QString &hex2, const QString &hex3,
                                         int value)
{
    MidiTable *midiTable = MidiTable::Instance();
    QString hex4 = QString::number(value, 16).toUpper().rightJustified(2, '0');
    return midiTable->getValue(hex0, hex1, hex2, hex3, hex4);
}

QVariantList ParameterBridge::getOptions(const QString &hex0, const QString &hex1,
                                         const QString &hex2, const QString &hex3)
{
    MidiTable *midiTable = MidiTable::Instance();
    Midi items = midiTable->getMidiMap(hex0, hex1, hex2, hex3);
    QVariantList options;
    for(int i = 0; i < items.level.size(); ++i)
    {
        QVariantMap option;
        QString label = items.level.at(i).customdesc.isEmpty()
                          ? items.level.at(i).desc
                          : items.level.at(i).customdesc;
        bool ok;
        int val = items.level.at(i).value.toInt(&ok, 16);
        // If desc is empty, use getDisplayValue to get the formatted label
        if (label.isEmpty()) {
            QString hex4 = QString::number(val, 16).toUpper().rightJustified(2, '0');
            label = midiTable->getValue(hex0, hex1, hex2, hex3, hex4).trimmed();
        }
        option["label"] = label;
        option["value"] = val;
        options.append(option);
    }
    return options;
}

void ParameterBridge::registerKnob(const QString &hex0, const QString &hex1,
                                   const QString &hex2, const QString &hex3)
{
    KnobAddress addr;
    addr.hex0 = hex0;
    addr.hex1 = hex1;
    addr.hex2 = hex2;
    addr.hex3 = hex3;
    m_registeredKnobs.append(addr);
    MidiCCHandler *handler = MidiCCHandler::Instance();
    handler->setActiveKnobs(m_registeredKnobs);
    // Show registration count in status bar
    emit handler->ccActivity(QString("CC: %1 knobs registered").arg(m_registeredKnobs.size()));
}

void ParameterBridge::clearRegisteredKnobs()
{
    m_registeredKnobs.clear();
    MidiCCHandler::Instance()->clearActiveKnobs();
}

// Collect VISIBLE controls with their Y position for row grouping
struct ScannedControl {
    KnobAddress addr;
    qreal y;
};

static void collectVisibleControls(QObject *obj, QVector<ScannedControl> &out)
{
    if (!obj) return;

    // Skip invisible items and all their children (hidden tabs, etc.)
    QQuickItem *item = qobject_cast<QQuickItem*>(obj);
    if (item && !item->isVisible()) return;

    QVariant hex0Var = obj->property("hex0");
    if (hex0Var.isValid() && !hex0Var.toString().isEmpty()) {
        QString hex0 = hex0Var.toString();
        QString hex3 = obj->property("hex3").toString();
        if (!hex0.isEmpty() && !hex3.isEmpty() && item) {
            ScannedControl sc;
            sc.addr.hex0 = hex0;
            sc.addr.hex1 = obj->property("hex1").toString();
            sc.addr.hex2 = obj->property("hex2").toString();
            sc.addr.hex3 = hex3;
            QPointF pos = item->mapToScene(QPointF(0, 0));
            sc.y = pos.y();
            out.append(sc);
        }
    }

    for (QObject *child : obj->children())
        collectVisibleControls(child, out);
}

void ParameterBridge::scanAndRegisterKnobs(QObject *root)
{
    // Step 1: collect only VISIBLE controls with scene Y position
    QVector<ScannedControl> controls;
    collectVisibleControls(root, controls);
    if (controls.isEmpty()) return;

    // Step 2: sort by Y (top to bottom), stable sort preserves left-to-right order
    std::stable_sort(controls.begin(), controls.end(),
        [](const ScannedControl &a, const ScannedControl &b) {
            return a.y < b.y;
        });

    // Step 3: group into visual rows and pad each to 8 for LCXL alignment
    // Controls within 60px Y of each other = same visual row
    // (merges header dropdowns with adjacent knob row, keeps Grid rows separate at ~108px gap)
    KnobAddress empty = {"", "", "", ""};
    QVector<KnobAddress> result;
    qreal rowStartY = controls.first().y;
    int countInRow = 0;
    int rowCount = 0;
    QString debugRows;

    for (int i = 0; i < controls.size(); i++) {
        bool newRow = (controls[i].y - rowStartY > 60.0) && countInRow > 0;

        if (newRow) {
            int padding = (8 - (countInRow % 8)) % 8;
            for (int p = 0; p < padding; p++)
                result.append(empty);
            debugRows += QString("r%1:%2 ").arg(rowCount).arg(countInRow);
            rowCount++;
            countInRow = 0;
            rowStartY = controls[i].y;
        }

        result.append(controls[i].addr);
        countInRow++;
    }
    debugRows += QString("r%1:%2").arg(rowCount).arg(countInRow);

    m_registeredKnobs = result;
    MidiCCHandler *handler = MidiCCHandler::Instance();
    handler->setActiveKnobs(m_registeredKnobs);

    emit handler->ccActivity(QString("CC: %1 knobs %2 [%3 slots]")
                             .arg(controls.size()).arg(debugRows).arg(result.size()));
}
