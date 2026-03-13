#include "qmlHost.h"
#include "parameterBridge.h"
#include <QQmlEngine>
#include <QQuickItem>
#include <QUrl>

QmlHost::QmlHost(const QString &qmlSource, QWidget *parent)
    : QQuickWidget(parent)
{
    setResizeMode(QQuickWidget::SizeRootObjectToView);
    setupContext();
    setSource(QUrl(qmlSource));

    if (status() == QQuickWidget::Error) {
        for (const auto &error : errors()) {
            qWarning() << "QML error:" << error.toString();
        }
    }
}

void QmlHost::setupContext()
{
    rootContext()->setContextProperty("paramBridge", ParameterBridge::Instance());
}

void QmlHost::setInstHex(const QString &hex1)
{
    rootContext()->setContextProperty("instHex1", hex1);
    if (rootObject()) {
        rootObject()->setProperty("hex1", hex1);
    }
}
