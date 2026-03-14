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
            "get hex0 hex1 hex2 hex3 - read parameter",
            "screenshot [file] - capture main window",
            "screenshot-window title - capture specific window",
            "open-qml-preview - open QML preview window",
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
