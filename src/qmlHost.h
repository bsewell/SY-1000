#ifndef QMLHOST_H
#define QMLHOST_H

#include <QQuickWidget>
#include <QQmlContext>
#include <QString>
#include <QColor>

class QmlHost : public QQuickWidget
{
    Q_OBJECT

public:
    explicit QmlHost(const QString &qmlSource, QWidget *parent = nullptr);
    void setInstHex(const QString &hex1, const QString &hex2 = "15");
    void setTitle(const QString &title);
    void setAccentColor(const QColor &color);

private:
    void setupContext();
};

#endif // QMLHOST_H
