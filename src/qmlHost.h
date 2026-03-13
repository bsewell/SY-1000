#ifndef QMLHOST_H
#define QMLHOST_H

#include <QQuickWidget>
#include <QQmlContext>
#include <QString>

class QmlHost : public QQuickWidget
{
    Q_OBJECT

public:
    explicit QmlHost(const QString &qmlSource, QWidget *parent = nullptr);
    void setInstHex(const QString &hex1);

private:
    void setupContext();
};

#endif // QMLHOST_H
