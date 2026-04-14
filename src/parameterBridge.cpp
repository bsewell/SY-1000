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

// ── Multi-nibble "data knob" helpers ──────────────────────────────

ParameterBridge::DataSpec ParameterBridge::resolveDataType(const QString &dataType)
{
    // Mirror the keyword → Tables address mapping from customDataKnob.cpp
    if (dataType == "DELAY2000")  return { "0D", 4 };
    if (dataType == "DELAY1300")  return { "0B", 3 };
    if (dataType == "PREDELAY")   return { "07", 4 };
    if (dataType == "BPM")        return { "0A", 4 };
    if (dataType == "0~100")      return { "07", 2 };
    if (dataType == "0~200")      return { "06", 2 };
    if (dataType == "0~500")      return { "08", 4 };
    if (dataType == "0~1000")     return { "09", 4 };
    if (dataType == "RATE")       return { "08", 2 };
    if (dataType == "ASSIGN")     return { "04", 4 };
    return { "00", 3 };  // fallback
}

int ParameterBridge::getDataValue(const QString &hex0, const QString &hex1,
                                  const QString &hex2, const QString &hex3,
                                  const QString &dataType)
{
    DataSpec spec = resolveDataType(dataType);
    SysxIO *sysxIO = SysxIO::Instance();
    bool ok;
    int startAddr = hex3.toInt(&ok, 16);

    // Read consecutive nibble-bytes and reconstruct the hex value.
    // Each SysEx byte holds one hex digit (0x00-0x0F).
    int value = 0;
    for (int i = 0; i < spec.byteCount; ++i)
    {
        QString addr = QString::number(startAddr + i, 16).toUpper().rightJustified(2, '0');
        int nibble = sysxIO->getSourceValue(hex0, hex1, hex2, addr);
        value = (value << 4) | (nibble & 0x0F);
    }
    return value;
}

void ParameterBridge::setDataValue(const QString &hex0, const QString &hex1,
                                   const QString &hex2, const QString &hex3,
                                   const QString &dataType, int value)
{
    DataSpec spec = resolveDataType(dataType);
    SysxIO *sysxIO = SysxIO::Instance();

    // Clamp to range
    MidiTable *midiTable = MidiTable::Instance();
    int maxVal = midiTable->getRange("Tables", "00", "00", spec.hexC);
    if (value > maxVal) value = maxVal;
    if (value < 0) value = 0;

    // Convert value to hex string, pad to byteCount digits
    QString valueHex = QString::number(value, 16).toUpper();
    while (valueHex.length() < spec.byteCount)
        valueHex.prepend("0");

    // Split into individual nibble-bytes: "01F4" → ["00","01","0F","04"]
    QList<QString> nibbleBytes;
    for (int i = 0; i < spec.byteCount; ++i)
    {
        QString nibble = QString("0") + valueHex.at(i);
        nibbleBytes.append(nibble);
    }

    sysxIO->setFileSource(hex0, hex1, hex2, hex3, nibbleBytes);
    emit parameterChanged(hex0, hex1, hex2, hex3, value);
}

int ParameterBridge::getDataMin(const QString &dataType)
{
    DataSpec spec = resolveDataType(dataType);
    return MidiTable::Instance()->getRangeMinimum("Tables", "00", "00", spec.hexC);
}

int ParameterBridge::getDataMax(const QString &dataType)
{
    DataSpec spec = resolveDataType(dataType);
    return MidiTable::Instance()->getRange("Tables", "00", "00", spec.hexC);
}

QString ParameterBridge::getDataLabel(const QString &dataType)
{
    DataSpec spec = resolveDataType(dataType);
    MidiTable *midiTable = MidiTable::Instance();
    Midi items = midiTable->getMidiMap("Tables", "00", "00", spec.hexC);
    return items.customdesc.isEmpty() ? items.desc : items.customdesc;
}

QString ParameterBridge::getDataDisplayValue(const QString &dataType, int value)
{
    DataSpec spec = resolveDataType(dataType);
    MidiTable *midiTable = MidiTable::Instance();
    QString valueHex = QString::number(value, 16).toUpper();
    while (valueHex.length() < spec.byteCount)
        valueHex.prepend("0");
    return midiTable->getValue("Tables", "00", "00", spec.hexC, valueHex);
}
