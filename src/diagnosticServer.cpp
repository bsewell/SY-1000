#include "diagnosticServer.h"
#include "parameterBridge.h"
#include "SysxIO.h"
#include "qmlHost.h"
#include <QApplication>
#include <QWidget>
#include <QScreen>
#include <QPixmap>
#include <QBuffer>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDir>

DiagnosticServer::DiagnosticServer(QObject *parent)
    : QObject(parent), server(new QTcpServer(this))
{
    connect(server, &QTcpServer::newConnection, this, &DiagnosticServer::onNewConnection);
}

bool DiagnosticServer::start(quint16 port)
{
    if (server->listen(QHostAddress::LocalHost, port)) {
        qWarning("DIAG: Diagnostic server listening on port %d", port);
        return true;
    }
    qWarning("DIAG: Failed to start diagnostic server on port %d", port);
    return false;
}

void DiagnosticServer::onNewConnection()
{
    while (server->hasPendingConnections()) {
        QTcpSocket *socket = server->nextPendingConnection();
        connect(socket, &QTcpSocket::readyRead, this, &DiagnosticServer::onReadyRead);
        connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
    }
}

void DiagnosticServer::onReadyRead()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket) return;

    while (socket->canReadLine()) {
        QString line = QString::fromUtf8(socket->readLine()).trimmed();
        handleCommand(socket, line);
    }
}

void DiagnosticServer::handleCommand(QTcpSocket *socket, const QString &command)
{
    QJsonObject response;
    response["command"] = command;

    if (command == "ping") {
        response["status"] = "ok";
        response["message"] = "pong";
    }
    else if (command == "status") {
        SysxIO *sysxIO = SysxIO::Instance();
        response["status"] = "ok";
        response["connected"] = sysxIO->isConnected();
        response["deviceReady"] = sysxIO->deviceReady();
        response["bank"] = sysxIO->getBank();
        response["patch"] = sysxIO->getPatch();

        // List all top-level windows
        QJsonArray windows;
        for (QWidget *w : QApplication::topLevelWidgets()) {
            QJsonObject win;
            win["title"] = w->windowTitle();
            win["visible"] = w->isVisible();
            win["width"] = w->width();
            win["height"] = w->height();
            win["class"] = w->metaObject()->className();
            windows.append(win);
        }
        response["windows"] = windows;
    }
    else if (command.startsWith("get ")) {
        // get hex0 hex1 hex2 hex3
        QStringList parts = command.mid(4).split(' ');
        if (parts.size() == 4) {
            ParameterBridge *pb = ParameterBridge::Instance();
            int value = pb->getValue(parts[0], parts[1], parts[2], parts[3]);
            QString label = pb->getLabel(parts[0], parts[1], parts[2], parts[3]);
            QString display = pb->getDisplayValue(parts[0], parts[1], parts[2], parts[3], value);
            response["status"] = "ok";
            response["value"] = value;
            response["label"] = label;
            response["display"] = display;
            response["min"] = pb->getMin(parts[0], parts[1], parts[2], parts[3]);
            response["max"] = pb->getMax(parts[0], parts[1], parts[2], parts[3]);
            QVariantList opts = pb->getOptions(parts[0], parts[1], parts[2], parts[3]);
            QJsonArray optArr;
            for (const auto &o : opts) {
                QVariantMap m = o.toMap();
                QJsonObject jo;
                jo["label"] = m["label"].toString();
                jo["value"] = m["value"].toInt();
                optArr.append(jo);
            }
            response["options"] = optArr;
            response["optionCount"] = opts.size();
        } else {
            response["status"] = "error";
            response["message"] = "usage: get hex0 hex1 hex2 hex3";
        }
    }
    else if (command.startsWith("screenshot-window ")) {
        // Format: screenshot-window <search> [filepath]
        // or:     screenshot-window <search>
        QString args = command.mid(18).trimmed();
        QString title, filename;

        // If args contains a path (starts with /), split on last space before path
        int pathIdx = args.indexOf('/');
        if (pathIdx > 0) {
            title = args.left(pathIdx).trimmed();
            filename = args.mid(pathIdx).trimmed();
        } else {
            title = args;
            filename = QDir::homePath() + "/010-MUSIC-STUDIO/SY-1000/screenshot_" +
                      title.simplified().replace(' ', '_') + ".png";
        }

        QWidget *target = nullptr;
        for (QWidget *w : QApplication::topLevelWidgets()) {
            if (w->isVisible() && w->windowTitle().contains(title, Qt::CaseInsensitive)) {
                target = w;
                break;
            }
        }

        if (target) {
            QPixmap pixmap = target->grab();
            pixmap.save(filename);
            response["status"] = "ok";
            response["file"] = filename;
            response["title"] = target->windowTitle();
            response["width"] = pixmap.width();
            response["height"] = pixmap.height();
        } else {
            response["status"] = "error";
            response["message"] = "Window not found: " + title;
        }
    }
    else if (command.startsWith("screenshot")) {
        // screenshot [filename]
        QString filename = command.mid(11).trimmed();
        if (filename.isEmpty()) {
            filename = QDir::homePath() + "/010-MUSIC-STUDIO/SY-1000/screenshot.png";
        }

        QWidget *mainWin = nullptr;
        for (QWidget *w : QApplication::topLevelWidgets()) {
            if (w->isVisible() && w->windowTitle().contains("FloorBoard")) {
                mainWin = w;
                break;
            }
        }

        if (mainWin) {
            QPixmap pixmap = mainWin->grab();
            pixmap.save(filename);
            response["status"] = "ok";
            response["file"] = filename;
            response["width"] = pixmap.width();
            response["height"] = pixmap.height();
        } else {
            response["status"] = "error";
            response["message"] = "No visible main window found";
        }
    }
    else if (command.startsWith("set ")) {
        // set hex0 hex1 hex2 hex3 value
        QStringList parts = command.mid(4).split(' ');
        if (parts.size() == 5) {
            bool ok;
            int value = parts[4].toInt(&ok);
            if (ok) {
                ParameterBridge *pb = ParameterBridge::Instance();
                pb->setValue(parts[0], parts[1], parts[2], parts[3], value);
                response["status"] = "ok";
                response["value"] = value;
                response["display"] = pb->getDisplayValue(parts[0], parts[1], parts[2], parts[3], value);
                response["label"] = pb->getLabel(parts[0], parts[1], parts[2], parts[3]);
            } else {
                response["status"] = "error";
                response["message"] = "value must be an integer";
            }
        } else {
            response["status"] = "error";
            response["message"] = "usage: set hex0 hex1 hex2 hex3 value";
        }
    }
    else if (command.startsWith("dump ")) {
        // dump hex0 hex1 hex2 startHex3 endHex3
        QStringList parts = command.mid(5).split(' ');
        if (parts.size() >= 4) {
            bool ok;
            QString hex0 = parts[0], hex1 = parts[1], hex2 = parts[2];
            int start = parts[3].toInt(&ok, 16);
            int end = (parts.size() >= 5) ? parts[4].toInt(&ok, 16) : start + 0x1F;
            ParameterBridge *pb = ParameterBridge::Instance();
            QJsonArray params;
            for (int i = start; i <= end; ++i) {
                QString hex3 = QString::number(i, 16).toUpper().rightJustified(2, '0');
                int value = pb->getValue(hex0, hex1, hex2, hex3);
                QString label = pb->getLabel(hex0, hex1, hex2, hex3);
                QString display = pb->getDisplayValue(hex0, hex1, hex2, hex3, value);
                int minVal = pb->getMin(hex0, hex1, hex2, hex3);
                int maxVal = pb->getMax(hex0, hex1, hex2, hex3);
                QJsonObject p;
                p["hex3"] = hex3;
                p["label"] = label;
                p["value"] = value;
                p["display"] = display;
                p["min"] = minVal;
                p["max"] = maxVal;
                params.append(p);
            }
            response["status"] = "ok";
            response["hex0"] = hex0;
            response["hex1"] = hex1;
            response["hex2"] = hex2;
            response["parameters"] = params;
        } else {
            response["status"] = "error";
            response["message"] = "usage: dump hex0 hex1 hex2 startHex3 [endHex3]";
        }
    }
    else if (command.startsWith("open-qml ")) {
        // open-qml <qmlFile> <hex1> <hex2>
        // e.g. open-qml InstrumentPanel.qml 00 15
        // e.g. open-qml NormalInputPanel.qml 00 12
        QStringList parts = command.mid(9).split(' ');
        if (parts.size() >= 3) {
            QString qmlFile = parts[0];
            QString hex1 = parts[1];
            QString hex2 = parts[2];

            // Build qrc path if not already prefixed
            QString qmlSource = qmlFile;
            if (!qmlSource.startsWith("qrc:")) {
                qmlSource = "qrc:/qml/" + qmlFile;
            }

            // Find the parent mainWindow
            QWidget *mainWin = nullptr;
            for (QWidget *w : QApplication::topLevelWidgets()) {
                if (w->windowTitle().contains("FloorBoard")) {
                    mainWin = w;
                    break;
                }
            }

            QmlHost *preview = new QmlHost(qmlSource, mainWin);
            preview->setInstHex(hex1, hex2);
            preview->setWindowFlag(Qt::Window);
            preview->setWindowTitle(qmlFile + " [" + hex1 + "," + hex2 + "]");
            preview->resize(800, 500);
            preview->setAttribute(Qt::WA_DeleteOnClose);
            preview->show();

            qmlPreviews.append(preview);

            response["status"] = "ok";
            response["source"] = qmlSource;
            response["hex1"] = hex1;
            response["hex2"] = hex2;
            response["title"] = preview->windowTitle();
        } else {
            response["status"] = "error";
            response["message"] = "usage: open-qml <file.qml> <hex1> <hex2>";
        }
    }
    else if (command == "close-qml") {
        int closed = 0;
        for (auto &p : qmlPreviews) {
            if (p) { p->close(); ++closed; }
        }
        qmlPreviews.clear();
        response["status"] = "ok";
        response["closed"] = closed;
    }
    else if (command == "list-qml") {
        response["status"] = "ok";
        QJsonArray panels;
        // Instrument panels
        QJsonObject inst;
        inst["file"] = "InstrumentPanel.qml";
        inst["description"] = "Full instrument panel with all type tabs";
        QJsonArray instPresets;
        QJsonObject i1; i1["name"] = "INST1 Guitar"; i1["hex1"] = "00"; i1["hex2"] = "15"; instPresets.append(i1);
        QJsonObject i2g; i2g["name"] = "INST2 Guitar"; i2g["hex1"] = "00"; i2g["hex2"] = "20"; instPresets.append(i2g);
        QJsonObject i3g; i3g["name"] = "INST3 Guitar"; i3g["hex1"] = "00"; i3g["hex2"] = "2B"; instPresets.append(i3g);
        QJsonObject i1b; i1b["name"] = "INST1 Bass"; i1b["hex1"] = "02"; i1b["hex2"] = "15"; instPresets.append(i1b);
        QJsonObject i2b; i2b["name"] = "INST2 Bass"; i2b["hex1"] = "02"; i2b["hex2"] = "1F"; instPresets.append(i2b);
        QJsonObject i3b; i3b["name"] = "INST3 Bass"; i3b["hex1"] = "02"; i3b["hex2"] = "29"; instPresets.append(i3b);
        inst["presets"] = instPresets;
        panels.append(inst);

        QJsonObject normal;
        normal["file"] = "NormalInputPanel.qml";
        normal["description"] = "Normal pickup input panel";
        QJsonArray normalPresets;
        QJsonObject ng; ng["name"] = "Guitar"; ng["hex1"] = "00"; ng["hex2"] = "12"; normalPresets.append(ng);
        QJsonObject nb; nb["name"] = "Bass"; nb["hex1"] = "02"; nb["hex2"] = "12"; normalPresets.append(nb);
        normal["presets"] = normalPresets;
        panels.append(normal);

        // Individual tabs (for standalone testing)
        QStringList tabs = {
            "CommonTab.qml", "AltTuneTab.qml", "OscTab.qml", "FilterTab.qml",
            "AmpTab.qml", "Lfo1Tab.qml", "Lfo2Tab.qml", "SeqTab.qml", "LayerTab.qml",
            "OscSynthOscTab.qml", "OscSynthFilterAmpTab.qml", "OscSynthLfoTab.qml",
            "Gr300Tab.qml", "EGuitarTab.qml", "EGuitarAmpTab.qml", "EGuitarNsTab.qml",
            "EGuitarEqTab.qml", "AcousticTab.qml", "AcousticAmpTab.qml", "AcousticEqTab.qml",
            "EBassTab.qml", "EBassAmpTab.qml", "EBassEqTab.qml",
            "VioGuitarTab.qml", "VioGuitarHarmonyTab.qml", "VioGuitarEqTab.qml",
            "PolyFxTab.qml"
        };
        for (const auto &tab : tabs) {
            QJsonObject t;
            t["file"] = tab;
            t["type"] = "tab";
            panels.append(t);
        }
        response["panels"] = panels;
    }
    else if (command == "open-qml-preview") {
        // Find mainWindow and call showQmlPreview
        emit commandReceived("open-qml-preview");
        response["status"] = "ok";
        response["message"] = "QML preview triggered";
    }
    else if (command == "help") {
        response["status"] = "ok";
        response["commands"] = QJsonArray({
            "ping - check connectivity",
            "status - app state + window list",
            "get hex0 hex1 hex2 hex3 - read parameter with metadata",
            "set hex0 hex1 hex2 hex3 value - write parameter value",
            "dump hex0 hex1 hex2 startHex3 [endHex3] - bulk read parameter range",
            "open-qml file.qml hex1 hex2 - open QML panel in standalone window",
            "close-qml - close all QML preview windows",
            "list-qml - list available QML panels with preset addresses",
            "screenshot [file] - capture main window",
            "screenshot-window title [file] - capture specific window",
            "open-qml-preview - open default QML preview window",
            "help - this message"
        });
    }
    else {
        response["status"] = "error";
        response["message"] = "Unknown command. Try 'help'";
    }

    QJsonDocument doc(response);
    socket->write(doc.toJson(QJsonDocument::Compact));
    socket->write("\n");
    socket->flush();
}
