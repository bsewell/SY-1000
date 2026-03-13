#include "parameterBridge.h"
#include "SysxIO.h"
#include "MidiTable.h"

ParameterBridge* ParameterBridge::instance = nullptr;

ParameterBridge::ParameterBridge(QObject *parent)
    : QObject(parent)
{
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
        option["label"] = items.level.at(i).customdesc.isEmpty()
                          ? items.level.at(i).desc
                          : items.level.at(i).customdesc;
        bool ok;
        option["value"] = items.level.at(i).value.toInt(&ok, 16);
        options.append(option);
    }
    return options;
}
