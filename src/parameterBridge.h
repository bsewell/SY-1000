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

signals:
    void parameterChanged(const QString &hex0, const QString &hex1,
                          const QString &hex2, const QString &hex3,
                          int newValue);
    void dataRefreshed();

private:
    explicit ParameterBridge(QObject *parent = nullptr);
    static ParameterBridge *instance;
};

#endif // PARAMETERBRIDGE_H
