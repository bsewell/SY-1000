#ifndef PARAMETERBRIDGE_H
#define PARAMETERBRIDGE_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QVariantList>
#include <QVariantMap>

class ParameterBridge : public QObject
{
    Q_OBJECT

public:
    static ParameterBridge* Instance();

    Q_INVOKABLE int getValue(const QString &hex0, const QString &hex1,
                             const QString &hex2, const QString &hex3);

    Q_INVOKABLE void setValue(const QString &hex0, const QString &hex1,
                              const QString &hex2, const QString &hex3,
                              int value);

    Q_INVOKABLE QString getLabel(const QString &hex0, const QString &hex1,
                                 const QString &hex2, const QString &hex3);

    Q_INVOKABLE int getMin(const QString &hex0, const QString &hex1,
                           const QString &hex2, const QString &hex3);

    Q_INVOKABLE int getMax(const QString &hex0, const QString &hex1,
                           const QString &hex2, const QString &hex3);

    Q_INVOKABLE QString getDisplayValue(const QString &hex0, const QString &hex1,
                                        const QString &hex2, const QString &hex3,
                                        int value);

    Q_INVOKABLE QVariantList getOptions(const QString &hex0, const QString &hex1,
                                        const QString &hex2, const QString &hex3);

    // Multi-nibble "data knob" API — reads/writes N consecutive nibble-bytes
    // starting at hex3.  dataType selects the Tables lookup key and byte count:
    //   "DELAY2000" → Tables/00/00/0D, 4 bytes
    //   "DELAY1300" → Tables/00/00/0B, 3 bytes
    //   "PREDELAY"  → Tables/00/00/07, 4 bytes
    //   "BPM"       → Tables/00/00/0A, 4 bytes
    Q_INVOKABLE int getDataValue(const QString &hex0, const QString &hex1,
                                 const QString &hex2, const QString &hex3,
                                 const QString &dataType);
    Q_INVOKABLE void setDataValue(const QString &hex0, const QString &hex1,
                                  const QString &hex2, const QString &hex3,
                                  const QString &dataType, int value);
    Q_INVOKABLE int getDataMin(const QString &dataType);
    Q_INVOKABLE int getDataMax(const QString &dataType);
    Q_INVOKABLE QString getDataLabel(const QString &dataType);
    Q_INVOKABLE QString getDataDisplayValue(const QString &dataType, int value);

signals:
    void parameterChanged(const QString &hex0, const QString &hex1,
                          const QString &hex2, const QString &hex3,
                          int newValue);
    void dataRefreshed();

private:
    explicit ParameterBridge(QObject *parent = nullptr);
    static ParameterBridge *instance;

    struct DataSpec { QString hexC; int byteCount; };
    static DataSpec resolveDataType(const QString &dataType);
};

#endif // PARAMETERBRIDGE_H
