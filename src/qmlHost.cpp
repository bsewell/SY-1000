#include "qmlHost.h"
#include "parameterBridge.h"
#include <QQmlEngine>
#include <QQuickItem>
#include <QUrl>
#include <QCoreApplication>
#include <QFile>
#include <QTextStream>
#include <QDir>

static void logToFile(const QString &msg)
{
    QFile f(QDir::homePath() + "/010-MUSIC-STUDIO/SY-1000/qml_debug.log");
    f.open(QIODevice::Append | QIODevice::Text);
    QTextStream out(&f);
    out << msg << "\n";
    f.close();
}

QmlHost::QmlHost(const QString &qmlSource, QWidget *parent)
    : QQuickWidget(parent)
{
    logToFile("=== QmlHost constructor ===");
    logToFile("Source: " + qmlSource);

    // Tell QML engine where to find modules in the app bundle
    QString appDir = QCoreApplication::applicationDirPath();
    QString qmlDir = appDir + "/../Resources/qml";

    // Set environment before engine initializes import paths
    qputenv("QML2_IMPORT_PATH", qmlDir.toUtf8());
    qputenv("QML_IMPORT_PATH", qmlDir.toUtf8());

    engine()->addImportPath(qmlDir);
    // Also add the PlugIns/quick path
    engine()->addImportPath(appDir + "/../PlugIns");
    logToFile("Added import path: " + qmlDir);
    logToFile("Added import path: " + appDir + "/../PlugIns");

    // Log all import paths for debugging
    for (const auto &p : engine()->importPathList()) {
        logToFile("  Import path: " + p);
    }

    setResizeMode(QQuickWidget::SizeRootObjectToView);
    setupContext();

    logToFile("Setting source...");
    setSource(QUrl(qmlSource));

    logToFile("Status: " + QString::number(status()));
    // Status: 0=Null, 1=Ready, 2=Loading, 3=Error

    if (status() == QQuickWidget::Error) {
        for (const auto &error : errors()) {
            logToFile("QML ERROR: " + error.toString());
        }
    }

    if (rootObject()) {
        logToFile("Root object created OK: " + QString(rootObject()->metaObject()->className()));
    } else {
        logToFile("Root object is NULL");
    }

    // Also log any warnings from the QML engine
    connect(engine(), &QQmlEngine::warnings, this, [](const QList<QQmlError> &warnings) {
        for (const auto &w : warnings) {
            logToFile("QML WARNING: " + w.toString());
        }
    });
}

void QmlHost::setupContext()
{
    rootContext()->setContextProperty("paramBridge", ParameterBridge::Instance());
    logToFile("Context property 'paramBridge' set");
}

void QmlHost::setInstHex(const QString &hex1, const QString &hex2)
{
    rootContext()->setContextProperty("instHex1", hex1);
    rootContext()->setContextProperty("instHex2", hex2);
    if (rootObject()) {
        rootObject()->setProperty("hex1", hex1);
        rootObject()->setProperty("hex2", hex2);
    }
}

void QmlHost::setTitle(const QString &title)
{
    if (rootObject()) {
        rootObject()->setProperty("instLabel", title);
    }
}

void QmlHost::setAccentColor(const QColor &color)
{
    if (rootObject()) {
        rootObject()->setProperty("accentColor", color);
    }
}
