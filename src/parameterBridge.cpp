#include "parameterBridge.h"
#include "SysxIO.h"
#include "MidiTable.h"

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

void ParameterBridge::scanAndRegisterKnobs(QObject *root)
{
    if (!root) return;

    // Check if this object has hex0 property (FilmstripKnob or SyComboBox)
    QVariant hex0Var = root->property("hex0");
    if (hex0Var.isValid() && !hex0Var.toString().isEmpty()) {
        QString hex0 = hex0Var.toString();
        QString hex1 = root->property("hex1").toString();
        QString hex2 = root->property("hex2").toString();
        QString hex3 = root->property("hex3").toString();
        if (!hex0.isEmpty() && !hex3.isEmpty()) {
            registerKnob(hex0, hex1, hex2, hex3);
        }
    }

    // Recurse into children
    for (QObject *child : root->children()) {
        scanAndRegisterKnobs(child);
    }
}
