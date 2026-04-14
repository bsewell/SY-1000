#include "parameterBridge.h"
#include "SysxIO.h"
#include "MidiTable.h"
#include <QQuickItem>
#include <QSet>
#include <QFile>
#include <QTextStream>
#include <QTimer>

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

void ParameterBridge::requestRescan()
{
    if (m_lastScanRoot) {
        clearRegisteredKnobs();
        // Delay to let QML Loader + Repeater finish recreating content
        // 500ms covers the Loader→Repeater→Loader chain
        QTimer::singleShot(500, this, [this]() {
            scanAndRegisterKnobs(m_lastScanRoot);
        });
    }
}

// Collect VISIBLE controls with their Y position for row grouping
struct ScannedControl {
    KnobAddress addr;
    qreal y;
};

static void collectVisibleControls(QObject *obj, QVector<ScannedControl> &out,
                                   QSet<QObject*> &visited)
{
    if (!obj || visited.contains(obj)) return;
    visited.insert(obj);

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

    // Walk QObject children (catches most QML items)
    for (QObject *child : obj->children())
        collectVisibleControls(child, out, visited);

    // Also walk QQuickItem visual children (catches Loader content, Repeater delegates)
    if (item) {
        for (QQuickItem *visualChild : item->childItems())
            collectVisibleControls(visualChild, out, visited);
    }
}

void ParameterBridge::scanAndRegisterKnobs(QObject *root)
{
    QQuickItem *rootItem = qobject_cast<QQuickItem*>(root);
    if (!rootItem) return;

    m_lastScanRoot = root;  // remember for requestRescan()

    // Collect ALL items via both QObject tree AND visual tree
    // Walk visual tree first (preserves layout order), then QObject tree as fallback
    QList<QQuickItem*> allItems;
    QSet<QQuickItem*> seen;
    std::function<void(QQuickItem*)> collectAll = [&](QQuickItem *item) {
        if (!item || seen.contains(item)) return;
        seen.insert(item);
        allItems.append(item);
        // Visual children first (preserves layout/document order)
        for (QQuickItem *vc : item->childItems()) {
            collectAll(vc);
        }
        // Then QObject children as fallback
        for (QObject *child : item->children()) {
            QQuickItem *qi = qobject_cast<QQuickItem*>(child);
            if (qi) collectAll(qi);
        }
    };
    collectAll(rootItem);

    // Deep debug: dump entire tree to file
    { QFile f("/tmp/cc_scan.log"); f.open(QIODevice::Append);
      QTextStream ts(&f);
      ts << "=== TREE DUMP (" << allItems.size() << " items) ===\n";
      int hexCount = 0;
      for (QQuickItem *item : allItems) {
          QVariant h0 = item->property("hex0");
          bool hasHex = h0.isValid() && !h0.toString().isEmpty();
          if (hasHex) {
              hexCount++;
              ts << "  HEX: " << item->metaObject()->className()
                 << " hex0=" << h0.toString()
                 << " hex3=" << item->property("hex3").toString()
                 << " visible=" << item->isVisible()
                 << " w=" << item->width() << " h=" << item->height()
                 << "\n";
          }
      }
      ts << "=== " << hexCount << " items with hex0 ===\n";
    }

    QVector<ScannedControl> controls;
    for (QQuickItem *item : allItems) {
        if (!item->isVisible()) continue;

        QVariant hex0Var = item->property("hex0");
        if (!hex0Var.isValid() || hex0Var.toString().isEmpty()) continue;

        QString hex0 = hex0Var.toString();
        QString hex3 = item->property("hex3").toString();
        if (hex0.isEmpty() || hex3.isEmpty()) continue;

        // Skip controls marked as ccExclude (e.g., FX TYPE dropdown that destroys panel on change)
        QVariant excludeVar = item->property("ccExclude");
        if (excludeVar.isValid() && excludeVar.toBool()) continue;

        ScannedControl sc;
        sc.addr.hex0 = hex0;
        sc.addr.hex1 = item->property("hex1").toString();
        sc.addr.hex2 = item->property("hex2").toString();
        sc.addr.hex3 = hex3;
        QPointF pos = item->mapToScene(QPointF(0, 0));
        sc.y = pos.y();
        controls.append(sc);
    }

    // Debug: log scan results to file
    { QFile f("/tmp/cc_scan.log"); f.open(QIODevice::Append);
      QTextStream ts(&f);
      ts << "SCAN: " << allItems.size() << " total items, " << controls.size() << " with hex0\n";
      for (int i = 0; i < controls.size(); i++) {
          ts << "  [" << i << "] " << controls[i].addr.hex0 << "/" << controls[i].addr.hex1
             << "/" << controls[i].addr.hex2 << "/" << controls[i].addr.hex3
             << " y=" << controls[i].y << "\n";
      }
    }

    if (controls.isEmpty()) return;

    // Step 2: sort by Y (top to bottom), stable sort preserves left-to-right order
    std::stable_sort(controls.begin(), controls.end(),
        [](const ScannedControl &a, const ScannedControl &b) {
            return a.y < b.y;
        });

    // Step 3: group into visual rows and pad each to 8 for LCXL alignment
    // Controls within 42px Y of each other = same visual row
    // FX TYPE header (y~42) separates from knobs (y~85) at 43px gap
    // INST header controls merge with first knobs at ~43px gap (borderline — INST uses Grid not Flow)
    KnobAddress empty = {"", "", "", ""};
    QVector<KnobAddress> result;
    qreal rowStartY = controls.first().y;
    int countInRow = 0;
    int rowCount = 0;
    QString debugRows;

    for (int i = 0; i < controls.size(); i++) {
        bool newRow = (controls[i].y - rowStartY > 42.0) && countInRow > 0;

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
