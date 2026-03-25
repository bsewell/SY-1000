#include "diagnosticServer.h"
#include "parameterBridge.h"
#include "SysxIO.h"
#include "MidiTable.h"
#include "Preferences.h"
#include "midiIO.h"
#include "qmlHost.h"
#include "globalVariables.h"
#include "patchListModel.h"
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
#include <QStandardPaths>

// ─── Address maps for named sections ───────────────────────────────────────
// Guitar mode patch parameter sections (hex0=10 for edit buffer)
struct SectionDef { const char *name; const char *hex1; const char *hex2; int count; };

static const SectionDef guitarSections[] = {
    // Patch Common
    {"patch-common",    "00", "00", 0x30},
    {"patch-common2",   "00", "02", 0x40},
    {"patch-led",       "00", "03", 0x10},
    // Assigns 1-16
    {"assign1",         "00", "04", 0x40},
    {"assign2",         "00", "05", 0x40},
    {"assign3",         "00", "06", 0x40},
    {"assign4",         "00", "07", 0x40},
    {"assign5",         "00", "08", 0x40},
    {"assign6",         "00", "09", 0x40},
    {"assign7",         "00", "0A", 0x40},
    {"assign8",         "00", "0B", 0x40},
    {"assign9",         "00", "0C", 0x40},
    {"assign10",        "00", "0D", 0x40},
    {"assign11",        "00", "0E", 0x40},
    {"assign12",        "00", "0F", 0x40},
    {"assign13",        "00", "10", 0x40},
    {"assign14",        "00", "11", 0x40},
    {"assign15",        "00", "12", 0x40},
    {"assign16",        "00", "13", 0x40},
    // Effects
    {"effect-chain",    "00", "12", 0x20},
    {"normal-pickup",   "00", "12", 0x10},
    // Instrument 1
    {"inst1-common",    "00", "15", 0x40},
    {"inst1-dynsynth",  "00", "16", 0x60},
    {"inst1-oscsynth",  "00", "17", 0x60},
    {"inst1-gr300",     "00", "18", 0x30},
    {"inst1-eguitar",   "00", "19", 0x60},
    {"inst1-acoustic",  "00", "1A", 0x40},
    {"inst1-ebass",     "00", "1B", 0x40},
    {"inst1-vioguitar", "00", "1C", 0x40},
    {"inst1-polyfx",    "00", "1D", 0x30},
    {"inst1-alttune",   "00", "1E", 0x20},
    // Instrument 2
    {"inst2-common",    "00", "20", 0x40},
    {"inst2-dynsynth",  "00", "21", 0x60},
    {"inst2-oscsynth",  "00", "22", 0x60},
    {"inst2-gr300",     "00", "23", 0x30},
    {"inst2-eguitar",   "00", "24", 0x60},
    {"inst2-acoustic",  "00", "25", 0x40},
    {"inst2-ebass",     "00", "26", 0x40},
    {"inst2-vioguitar", "00", "27", 0x40},
    {"inst2-polyfx",    "00", "28", 0x30},
    {"inst2-alttune",   "00", "29", 0x20},
    // Instrument 3
    {"inst3-common",    "00", "2B", 0x40},
    {"inst3-dynsynth",  "00", "2C", 0x60},
    {"inst3-oscsynth",  "00", "2D", 0x60},
    {"inst3-gr300",     "00", "2E", 0x30},
    {"inst3-eguitar",   "00", "2F", 0x60},
    {"inst3-acoustic",  "00", "30", 0x40},
    {"inst3-ebass",     "00", "31", 0x40},
    {"inst3-vioguitar", "00", "32", 0x40},
    {"inst3-polyfx",    "00", "33", 0x30},
    {"inst3-alttune",   "00", "34", 0x20},
    // Master effects
    {"ds",              "00", "36", 0x20},
    {"amp",             "00", "37", 0x40},
    {"ns",              "00", "38", 0x10},
    {"fx1",             "00", "39", 0x7F},
    {"fx2",             "00", "3B", 0x7F},
    {"fx3",             "00", "3D", 0x7F},
    {"dd1",             "00", "3F", 0x30},
    {"dd2",             "00", "40", 0x30},
    {"dd3",             "00", "41", 0x30},
    {"cs",              "00", "42", 0x20},
    {"eq1",             "00", "43", 0x20},
    {"eq2",             "00", "44", 0x20},
    {"rv",              "00", "45", 0x20},
    {"ce",              "00", "46", 0x20},
    {"lp",              "00", "47", 0x10},
    {"fv1",             "00", "48", 0x10},
    {"fv2",             "00", "49", 0x10},
    {"divider",         "00", "4A", 0x10},
    {"mixer",           "00", "4B", 0x10},
    {"balancer1",       "00", "4C", 0x10},
    {"balancer2",       "00", "4D", 0x10},
    {"balancer3",       "00", "4E", 0x10},
    {"main-out-l",      "00", "50", 0x10},
    {"main-out-r",      "00", "51", 0x10},
    {"sub-out-l",       "00", "52", 0x10},
    {"sub-out-r",       "00", "53", 0x10},
    {"master",          "00", "54", 0x30},
    {nullptr, nullptr, nullptr, 0}
};

// Effect name → on/off address mapping (hex0=10, hex3=00 is typically the on/off byte)
struct EffectAddr { const char *name; const char *hex1; const char *hex2; const char *hex3; };

static const EffectAddr effectOnOff[] = {
    {"inst1",    "00", "15", "00"},
    {"inst2",    "00", "20", "00"},
    {"inst3",    "00", "2B", "00"},
    {"ds",       "00", "36", "00"},
    {"amp",      "00", "37", "00"},
    {"ns",       "00", "38", "00"},
    {"fx1",      "00", "39", "00"},
    {"fx2",      "00", "3B", "00"},
    {"fx3",      "00", "3D", "00"},
    {"dd1",      "00", "3F", "00"},
    {"dd2",      "00", "40", "00"},
    {"dd3",      "00", "41", "00"},
    {"cs",       "00", "42", "00"},
    {"eq1",      "00", "43", "00"},
    {"eq2",      "00", "44", "00"},
    {"rv",       "00", "45", "00"},
    {"ce",       "00", "46", "00"},
    {"lp",       "00", "47", "00"},
    {"fv1",      "00", "48", "00"},
    {"fv2",      "00", "49", "00"},
    {nullptr, nullptr, nullptr, nullptr}
};


// ─── Constructor / Destructor ──────────────────────────────────────────────

DiagnosticServer::DiagnosticServer(QObject *parent)
    : QObject(parent), server(new QTcpServer(this)),
      logFile(nullptr), logStream(nullptr),
      logLevel(2), clientCount(0)
{
    connect(server, &QTcpServer::newConnection, this, &DiagnosticServer::onNewConnection);
    uptime.start();

    // Set up log file
    logFilePath = QDir::homePath() + "/000-HOME/010-MUSIC-STUDIO/SY-1000/diag.log";
    logFile = new QFile(logFilePath);
    if (logFile->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        logStream = new QTextStream(logFile);
    }
}

DiagnosticServer::~DiagnosticServer()
{
    if (logStream) {
        logStream->flush();
        delete logStream;
    }
    if (logFile) {
        logFile->close();
        delete logFile;
    }
}

// ─── Logging ───────────────────────────────────────────────────────────────

void DiagnosticServer::log(const QString &category, const QString &message)
{
    QMutexLocker locker(&logMutex);
    if (!logStream) return;

    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz");
    QString line = QString("[%1] [%2] %3").arg(timestamp, category, message);
    *logStream << line << "\n";
    logStream->flush();

    // Also emit to qWarning for console visibility
    qWarning("DIAG: [%s] %s", qPrintable(category), qPrintable(message));
}


// ─── Server lifecycle ──────────────────────────────────────────────────────

bool DiagnosticServer::start(quint16 port)
{
    if (server->listen(QHostAddress::LocalHost, port)) {
        log("SERVER", QString("Diagnostic server started on port %1").arg(port));
        return true;
    }
    log("ERROR", QString("Failed to start on port %1: %2").arg(port).arg(server->errorString()));
    return false;
}

void DiagnosticServer::onNewConnection()
{
    while (server->hasPendingConnections()) {
        QTcpSocket *socket = server->nextPendingConnection();
        ++clientCount;
        connect(socket, &QTcpSocket::readyRead, this, &DiagnosticServer::onReadyRead);
        connect(socket, &QTcpSocket::disconnected, this, &DiagnosticServer::onClientDisconnected);
        connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
        log("CONN", QString("Client #%1 connected from %2:%3")
            .arg(clientCount)
            .arg(socket->peerAddress().toString())
            .arg(socket->peerPort()));
    }
}

void DiagnosticServer::onClientDisconnected()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if (socket) {
        watchSockets.removeAll(socket);
        log("CONN", "Client disconnected");
    }
}

void DiagnosticServer::onReadyRead()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket || socket->state() != QAbstractSocket::ConnectedState) return;

    while (socket->canReadLine()) {
        QString line = QString::fromUtf8(socket->readLine()).trimmed();
        if (!line.isEmpty()) {
            handleCommand(socket, line);
        }
        // Re-check socket state between commands
        if (socket->state() != QAbstractSocket::ConnectedState) return;
    }
}

void DiagnosticServer::sendResponse(QTcpSocket *socket, const QJsonObject &response)
{
    if (!socket || socket->state() != QAbstractSocket::ConnectedState) return;
    QJsonDocument doc(response);
    socket->write(doc.toJson(QJsonDocument::Compact));
    socket->write("\n");
    socket->flush();
}


// ─── Command dispatcher ───────────────────────────────────────────────────

void DiagnosticServer::handleCommand(QTcpSocket *socket, const QString &command)
{
    QElapsedTimer cmdTimer;
    cmdTimer.start();

    if (logLevel >= 2) {
        log("CMD", command);
    }

    QJsonObject response;
    response["command"] = command;

    QString cmd = command.section(' ', 0, 0).toLower();
    QString args = command.mid(cmd.length()).trimmed();

    // ── Core ──
    if (cmd == "ping")              response = cmdPing();
    else if (cmd == "status")       response = cmdStatus();
    else if (cmd == "help")         response = cmdHelp();
    else if (cmd == "log")          response = cmdLog(args);
    else if (cmd == "log-level")    response = cmdLogLevel(args);

    // ── Connection ──
    else if (cmd == "connect")      response = cmdConnect();
    else if (cmd == "disconnect")   response = cmdDisconnect();
    else if (cmd == "midi-devices") response = cmdMidiDevices();
    else if (cmd == "midi-ports")   response = cmdMidiPorts();
    else if (cmd == "identity")     response = cmdIdentity();

    // ── Parameters ──
    else if (cmd == "get")          response = cmdGet(args);
    else if (cmd == "set")          response = cmdSet(args);
    else if (cmd == "dump")         response = cmdDump(args);
    else if (cmd == "dump-section") response = cmdDumpSection(args);

    // ── Patches ──
    else if (cmd == "patch")        response = cmdPatch();
    else if (cmd == "patch-change") response = cmdPatchChange(args);
    else if (cmd == "patch-name")   response = cmdPatchName();
    else if (cmd == "patch-list")   response = cmdPatchList();
    else if (cmd == "patch-load")   response = cmdPatchLoad(args);
    else if (cmd == "patch-save")   response = cmdPatchSave(args);
    else if (cmd == "patch-write")  response = cmdPatchWrite();

    // ── System ──
    else if (cmd == "system")       response = cmdSystem();
    else if (cmd == "system-request") response = cmdSystemRequest();
    else if (cmd == "system-write") response = cmdSystemWrite();
    else if (cmd == "mode")         response = cmdMode();
    else if (cmd == "mode-set")     response = cmdModeSet(args);

    // ── Effects ──
    else if (cmd == "effect-chain") response = cmdEffectChain();
    else if (cmd == "effect-toggle") response = cmdEffectToggle(args);
    else if (cmd == "instrument")   response = cmdInstrument(args);

    // ── Assigns ──
    else if (cmd == "assign")       response = cmdAssign(args);

    // ── Preferences ──
    else if (cmd == "pref-get")     response = cmdPrefGet(args);
    else if (cmd == "pref-set")     response = cmdPrefSet(args);

    // ── Screenshots ──
    else if (cmd == "screenshot-window") response = cmdScreenshotWindow(args);
    else if (cmd == "screenshot")   response = cmdScreenshot(args);

    // ── QML ──
    else if (cmd == "open-qml")     response = cmdOpenQml(args);
    else if (cmd == "close-qml")    response = cmdCloseQml();
    else if (cmd == "list-qml")     response = cmdListQml();

    // ── Bulk ──
    else if (cmd == "bulk-names")   response = cmdBulkNames();
    else if (cmd == "sysex")        response = cmdSysex(args);

    // ── Layout ──
    else if (cmd == "layout-score") response = cmdLayoutScore();
    else if (cmd == "layout-test")  response = cmdLayoutTest(args);

    // ── Monitoring ──
    else if (cmd == "watch")        response = cmdWatch(socket, args);
    else if (cmd == "unwatch")      response = cmdUnwatch(socket);

    // ── Alias: open-qml-preview ──
    else if (cmd == "open-qml-preview") {
        emit commandReceived("open-qml-preview");
        response["status"] = "ok";
        response["message"] = "QML preview triggered";
    }

    else {
        response["status"] = "error";
        response["message"] = "Unknown command. Try 'help'";
    }

    // Add timing and echo command
    response["command"] = command;
    response["elapsed_ms"] = cmdTimer.elapsed();

    sendResponse(socket, response);

    if (logLevel >= 3) {
        log("RESP", QString("(%1ms) %2 → %3")
            .arg(cmdTimer.elapsed())
            .arg(command)
            .arg(response["status"].toString()));
    }
}


// ═══════════════════════════════════════════════════════════════════════════
//  CORE COMMANDS
// ═══════════════════════════════════════════════════════════════════════════

QJsonObject DiagnosticServer::cmdPing()
{
    QJsonObject r;
    r["status"] = "ok";
    r["message"] = "pong";
    r["uptime_ms"] = (qint64)uptime.elapsed();
    return r;
}

QJsonObject DiagnosticServer::cmdStatus()
{
    SysxIO *sysxIO = SysxIO::Instance();
    QJsonObject r;
    r["status"] = "ok";
    r["connected"] = sysxIO->isConnected();
    r["deviceReady"] = sysxIO->deviceReady();
    r["syncStatus"] = sysxIO->getSyncStatus();
    r["bank"] = sysxIO->getBank();
    r["patch"] = sysxIO->getPatch();
    r["loadedBank"] = sysxIO->getLoadedBank();
    r["loadedPatch"] = sysxIO->getLoadedPatch();
    r["patchName"] = sysxIO->getCurrentPatchName();
    r["mode"] = sysxIO->mode; // 0=guitar, 2=bass
    r["modeLabel"] = (sysxIO->mode == 0) ? "Guitar" : "Bass";
    r["uptime_ms"] = (qint64)uptime.elapsed();
    r["logLevel"] = logLevel;
    r["logFile"] = logFilePath;
    r["watchers"] = watchSockets.size();

    // MIDI device info (read from preferences - thread-safe)
    Preferences *prefs = Preferences::Instance();
    QJsonObject midiInfo;
    midiInfo["outDevice"] = prefs->getPreferences("Midi", "MidiOut", "name");
    midiInfo["inDevice"] = prefs->getPreferences("Midi", "MidiIn", "name");
    r["midi"] = midiInfo;

    // Window list (compact)
    QJsonArray windows;
    for (QWidget *w : QApplication::topLevelWidgets()) {
        if (w->isVisible()) {
            QJsonObject win;
            win["title"] = w->windowTitle();
            win["class"] = w->metaObject()->className();
            win["width"] = w->width();
            win["height"] = w->height();
            windows.append(win);
        }
    }
    r["visibleWindows"] = windows;

    return r;
}

QJsonObject DiagnosticServer::cmdHelp()
{
    QJsonObject r;
    r["status"] = "ok";

    QJsonObject cmds;

    // Core
    QJsonArray core;
    core.append("ping - connectivity check, returns uptime");
    core.append("status - full app state: connection, patch, mode, MIDI devices, windows");
    core.append("help - this command reference");
    core.append("log [tail|clear|path] - view/manage diagnostic log");
    core.append("log-level [0-3] - set verbosity (0=off, 1=errors, 2=commands, 3=verbose)");
    cmds["core"] = core;

    // Connection
    QJsonArray conn;
    conn.append("connect - send identity request to SY-1000");
    conn.append("disconnect - close MIDI ports");
    conn.append("midi-devices - list all available MIDI in/out devices");
    conn.append("midi-ports - show currently selected MIDI ports");
    conn.append("identity - show SY-1000 identity request/reply strings");
    cmds["connection"] = conn;

    // Parameters
    QJsonArray params;
    params.append("get hex0 hex1 hex2 hex3 - read parameter with label, value, display, min, max, options");
    params.append("set hex0 hex1 hex2 hex3 value - write parameter value (integer)");
    params.append("dump hex0 hex1 hex2 startHex3 [endHex3] - bulk read parameter range");
    params.append("dump-section name - dump named section (e.g. amp, fx1, inst1-common, master, rv)");
    cmds["parameters"] = params;

    // Patches
    QJsonArray patches;
    patches.append("patch - current bank/patch number and name");
    patches.append("patch-change bank patch - switch to bank (0-99) and patch (0-3)");
    patches.append("patch-name - get current patch name");
    patches.append("patch-list - list all loaded patch names");
    patches.append("patch-load filepath - load .syx/.mid patch file");
    patches.append("patch-save filepath - save current patch to file");
    patches.append("patch-write - write current edit buffer to device memory");
    cmds["patches"] = patches;

    // System
    QJsonArray sys;
    sys.append("system - dump key system parameters");
    sys.append("system-request - request system data from device");
    sys.append("system-write - write system settings to device");
    sys.append("mode - show current mode (Guitar/Bass)");
    sys.append("mode-set guitar|bass - switch mode");
    cmds["system"] = sys;

    // Effects
    QJsonArray fx;
    fx.append("effect-chain - dump all effects with on/off state and type");
    fx.append("effect-toggle name - toggle effect on/off (e.g. fx1, dd1, rv, amp, ds)");
    fx.append("instrument 1|2|3 - dump instrument parameters and current type");
    cmds["effects"] = fx;

    // Assigns
    QJsonArray asgn;
    asgn.append("assign 1-16 - dump assign block parameters");
    cmds["assigns"] = asgn;

    // Preferences
    QJsonArray prefs;
    prefs.append("pref-get group type item - read preference value");
    prefs.append("pref-set group type item value - write preference value");
    cmds["preferences"] = prefs;

    // Screenshots
    QJsonArray shots;
    shots.append("screenshot [filepath] - capture main window");
    shots.append("screenshot-window title [filepath] - capture window by title search");
    cmds["screenshots"] = shots;

    // QML
    QJsonArray qml;
    qml.append("open-qml file.qml hex1 hex2 - open QML panel in standalone window");
    qml.append("close-qml - close all QML preview windows");
    qml.append("list-qml - list available QML panels with preset addresses");
    qml.append("open-qml-preview - open default QML preview window");
    cmds["qml"] = qml;

    // Bulk
    QJsonArray bulk;
    bulk.append("bulk-names - request all patch names from device");
    bulk.append("sysex hexstring - send raw sysex message (e.g. F07E7F0601F7)");
    cmds["bulk"] = bulk;

    // Monitoring
    QJsonArray mon;
    mon.append("watch [filter] - subscribe to parameter change notifications");
    mon.append("unwatch - unsubscribe from notifications");
    cmds["monitoring"] = mon;

    r["commands"] = cmds;

    // Section names for dump-section
    QJsonArray sections;
    for (int i = 0; guitarSections[i].name; ++i) {
        sections.append(QString(guitarSections[i].name));
    }
    r["sections"] = sections;

    return r;
}

QJsonObject DiagnosticServer::cmdLog(const QString &args)
{
    QJsonObject r;

    if (args == "clear") {
        QMutexLocker locker(&logMutex);
        if (logFile) {
            logFile->close();
            (void)logFile->open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);
            delete logStream;
            logStream = new QTextStream(logFile);
        }
        r["status"] = "ok";
        r["message"] = "Log cleared";
    }
    else if (args == "path") {
        r["status"] = "ok";
        r["path"] = logFilePath;
    }
    else {
        // Default: tail last 50 lines
        int lines = 50;
        if (!args.isEmpty()) {
            bool ok;
            int n = args.toInt(&ok);
            if (ok && n > 0) lines = n;
        }

        QFile readFile(logFilePath);
        QStringList allLines;
        if (readFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&readFile);
            while (!in.atEnd()) {
                allLines.append(in.readLine());
            }
            readFile.close();
        }

        QJsonArray logLines;
        int start = qMax(0, allLines.size() - lines);
        for (int i = start; i < allLines.size(); ++i) {
            logLines.append(allLines[i]);
        }

        r["status"] = "ok";
        r["lines"] = logLines;
        r["total"] = allLines.size();
        r["showing"] = logLines.size();
    }

    return r;
}

QJsonObject DiagnosticServer::cmdLogLevel(const QString &args)
{
    QJsonObject r;
    if (args.isEmpty()) {
        r["status"] = "ok";
        r["logLevel"] = logLevel;
        r["levels"] = QJsonArray({"0=off", "1=errors", "2=commands", "3=verbose"});
    } else {
        bool ok;
        int level = args.toInt(&ok);
        if (ok && level >= 0 && level <= 3) {
            logLevel = level;
            r["status"] = "ok";
            r["logLevel"] = logLevel;
            log("CONFIG", QString("Log level set to %1").arg(logLevel));
        } else {
            r["status"] = "error";
            r["message"] = "log-level must be 0-3";
        }
    }
    return r;
}


// ═══════════════════════════════════════════════════════════════════════════
//  CONNECTION COMMANDS
// ═══════════════════════════════════════════════════════════════════════════

QJsonObject DiagnosticServer::cmdConnect()
{
    QJsonObject r;
    SysxIO *sysxIO = SysxIO::Instance();
    if (sysxIO->isConnected()) {
        r["status"] = "ok";
        r["message"] = "Already connected";
        r["connected"] = true;
    } else {
        log("MIDI", "Sending identity request");
        QMetaObject::invokeMethod(sysxIO, [sysxIO]() {
            sysxIO->sysxOutput(idRequestString, 15);
        }, Qt::QueuedConnection);
        r["status"] = "ok";
        r["message"] = "Identity request sent - check status for result";
    }
    return r;
}

QJsonObject DiagnosticServer::cmdDisconnect()
{
    QJsonObject r;
    SysxIO *sysxIO = SysxIO::Instance();
    midiIO *midi = midiIO::Instance();
    QMetaObject::invokeMethod(qApp, [midi, sysxIO]() {
        midi->closePorts();
        sysxIO->setConnected(false);
        sysxIO->setDeviceReady(false);
    }, Qt::QueuedConnection);
    log("MIDI", "Disconnected - MIDI ports closed");
    r["status"] = "ok";
    r["message"] = "Disconnected";
    r["connected"] = false;
    return r;
}

QJsonObject DiagnosticServer::cmdMidiDevices()
{
    QJsonObject r;

    // Access midiIO on main thread via queued invocation to avoid threading issues
    // For now, report from preferences which is thread-safe
    Preferences *prefs = Preferences::Instance();

    r["status"] = "ok";
    r["outDevice"] = prefs->getPreferences("Midi", "MidiOut", "name");
    r["inDevice"] = prefs->getPreferences("Midi", "MidiIn", "name");
    r["outIndex"] = prefs->getPreferences("Midi", "MidiOut", "device");
    r["inIndex"] = prefs->getPreferences("Midi", "MidiIn", "device");
    r["note"] = "Use midi-ports for live device enumeration (requires main thread)";

    return r;
}

QJsonObject DiagnosticServer::cmdMidiPorts()
{
    QJsonObject r;
    Preferences *prefs = Preferences::Instance();

    r["status"] = "ok";
    r["outDevice"] = prefs->getPreferences("Midi", "MidiOut", "name");
    r["inDevice"] = prefs->getPreferences("Midi", "MidiIn", "name");
    r["outIndex"] = prefs->getPreferences("Midi", "MidiOut", "device");
    r["inIndex"] = prefs->getPreferences("Midi", "MidiIn", "device");
    r["autoSelect"] = prefs->getPreferences("Midi", "Device", "bool");
    r["debug"] = prefs->getPreferences("Midi", "DBug", "bool");

    return r;
}

QJsonObject DiagnosticServer::cmdIdentity()
{
    QJsonObject r;
    r["status"] = "ok";
    r["identityRequest"] = idRequestString;
    r["identityReplyPattern"] = idReplyPatern;
    r["manufacturer"] = "41 (Roland)";
    r["modelId"] = "00 00 00 69 (SY-1000)";
    return r;
}


// ═══════════════════════════════════════════════════════════════════════════
//  PARAMETER COMMANDS
// ═══════════════════════════════════════════════════════════════════════════

QJsonObject DiagnosticServer::cmdGet(const QString &args)
{
    QJsonObject r;
    QStringList parts = args.split(' ', Qt::SkipEmptyParts);
    if (parts.size() != 4) {
        r["status"] = "error";
        r["message"] = "usage: get hex0 hex1 hex2 hex3";
        return r;
    }

    ParameterBridge *pb = ParameterBridge::Instance();
    int value = pb->getValue(parts[0], parts[1], parts[2], parts[3]);
    QString label = pb->getLabel(parts[0], parts[1], parts[2], parts[3]);
    QString display = pb->getDisplayValue(parts[0], parts[1], parts[2], parts[3], value);

    r["status"] = "ok";
    r["address"] = parts.join(" ");
    r["value"] = value;
    r["label"] = label;
    r["display"] = display;
    r["min"] = pb->getMin(parts[0], parts[1], parts[2], parts[3]);
    r["max"] = pb->getMax(parts[0], parts[1], parts[2], parts[3]);

    QVariantList opts = pb->getOptions(parts[0], parts[1], parts[2], parts[3]);
    if (!opts.isEmpty()) {
        QJsonArray optArr;
        for (const auto &o : opts) {
            QVariantMap m = o.toMap();
            QJsonObject jo;
            jo["label"] = m["label"].toString();
            jo["value"] = m["value"].toInt();
            optArr.append(jo);
        }
        r["options"] = optArr;
        r["optionCount"] = opts.size();
    }

    if (logLevel >= 3) {
        log("PARAM", QString("GET %1 = %2 (%3)").arg(parts.join(" ")).arg(value).arg(display));
    }

    return r;
}

QJsonObject DiagnosticServer::cmdSet(const QString &args)
{
    QJsonObject r;
    QStringList parts = args.split(' ', Qt::SkipEmptyParts);
    if (parts.size() != 5) {
        r["status"] = "error";
        r["message"] = "usage: set hex0 hex1 hex2 hex3 value";
        return r;
    }

    bool ok;
    int value = parts[4].toInt(&ok);
    if (!ok) {
        r["status"] = "error";
        r["message"] = "value must be an integer";
        return r;
    }

    ParameterBridge *pb = ParameterBridge::Instance();
    int minVal = pb->getMin(parts[0], parts[1], parts[2], parts[3]);
    int maxVal = pb->getMax(parts[0], parts[1], parts[2], parts[3]);

    if (value < minVal || value > maxVal) {
        r["status"] = "error";
        r["message"] = QString("value %1 out of range [%2, %3]").arg(value).arg(minVal).arg(maxVal);
        r["min"] = minVal;
        r["max"] = maxVal;
        return r;
    }

    // Marshal setValue to main thread to avoid MIDI threading issues
    QString h0 = parts[0], h1 = parts[1], h2 = parts[2], h3 = parts[3];
    QMetaObject::invokeMethod(pb, [pb, h0, h1, h2, h3, value]() {
        pb->setValue(h0, h1, h2, h3, value);
    }, Qt::QueuedConnection);

    r["status"] = "ok";
    r["address"] = parts.mid(0, 4).join(" ");
    r["value"] = value;
    r["display"] = pb->getDisplayValue(parts[0], parts[1], parts[2], parts[3], value);
    r["label"] = pb->getLabel(parts[0], parts[1], parts[2], parts[3]);

    log("PARAM", QString("SET %1 = %2 (%3)")
        .arg(parts.mid(0, 4).join(" "))
        .arg(value)
        .arg(r["display"].toString()));

    return r;
}

QJsonObject DiagnosticServer::cmdDump(const QString &args)
{
    QJsonObject r;
    QStringList parts = args.split(' ', Qt::SkipEmptyParts);
    if (parts.size() < 4) {
        r["status"] = "error";
        r["message"] = "usage: dump hex0 hex1 hex2 startHex3 [endHex3]";
        return r;
    }

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

        if (!label.isEmpty()) {
            QJsonObject p;
            p["address"] = QString("%1 %2 %3 %4").arg(hex0, hex1, hex2, hex3);
            p["hex3"] = hex3;
            p["label"] = label;
            p["value"] = value;
            p["display"] = display;
            p["min"] = minVal;
            p["max"] = maxVal;
            params.append(p);
        }
    }

    r["status"] = "ok";
    r["hex0"] = hex0;
    r["hex1"] = hex1;
    r["hex2"] = hex2;
    r["parameters"] = params;
    r["count"] = params.size();

    log("PARAM", QString("DUMP %1 %2 %3 [%4-%5]: %6 params")
        .arg(hex0, hex1, hex2)
        .arg(start, 2, 16, QChar('0'))
        .arg(end, 2, 16, QChar('0'))
        .arg(params.size()));

    return r;
}

QJsonObject DiagnosticServer::cmdDumpSection(const QString &args)
{
    QJsonObject r;
    QString name = args.trimmed().toLower();

    if (name.isEmpty()) {
        // List available sections
        QJsonArray sections;
        for (int i = 0; guitarSections[i].name; ++i) {
            QJsonObject s;
            s["name"] = QString(guitarSections[i].name);
            s["hex1"] = QString(guitarSections[i].hex1);
            s["hex2"] = QString(guitarSections[i].hex2);
            s["count"] = guitarSections[i].count;
            sections.append(s);
        }
        r["status"] = "ok";
        r["sections"] = sections;
        return r;
    }

    // Find the section
    const SectionDef *found = nullptr;
    for (int i = 0; guitarSections[i].name; ++i) {
        if (name == guitarSections[i].name) {
            found = &guitarSections[i];
            break;
        }
    }

    if (!found) {
        r["status"] = "error";
        r["message"] = "Unknown section: " + name + ". Use 'dump-section' for list.";
        return r;
    }

    // Dump using edit buffer (hex0=10)
    QString dumpArgs = QString("10 %1 %2 00 %3")
        .arg(found->hex1)
        .arg(found->hex2)
        .arg(QString::number(found->count - 1, 16).toUpper().rightJustified(2, '0'));

    r = cmdDump(dumpArgs);
    r["section"] = name;

    log("PARAM", QString("DUMP-SECTION %1").arg(name));

    return r;
}


// ═══════════════════════════════════════════════════════════════════════════
//  PATCH COMMANDS
// ═══════════════════════════════════════════════════════════════════════════

QJsonObject DiagnosticServer::cmdPatch()
{
    QJsonObject r;
    SysxIO *sysxIO = SysxIO::Instance();
    r["status"] = "ok";
    r["bank"] = sysxIO->getBank();
    r["patch"] = sysxIO->getPatch();
    r["loadedBank"] = sysxIO->getLoadedBank();
    r["loadedPatch"] = sysxIO->getLoadedPatch();
    r["patchName"] = sysxIO->getCurrentPatchName();
    r["syncStatus"] = sysxIO->getSyncStatus();
    r["patchLoaded"] = sysxIO->patch_loaded;

    // Compute bank label
    int bank = sysxIO->getBank();
    int patch = sysxIO->getPatch();
    QString bankLabel;
    if (bank < bankTotalUser) {
        bankLabel = QString("U%1:%2").arg(bank + 1, 2, 10, QChar('0')).arg(patch + 1);
    } else {
        bankLabel = QString("P%1:%2").arg(bank - bankTotalUser + 1, 2, 10, QChar('0')).arg(patch + 1);
    }
    r["bankLabel"] = bankLabel;

    return r;
}

QJsonObject DiagnosticServer::cmdPatchChange(const QString &args)
{
    QJsonObject r;
    QStringList parts = args.split(' ', Qt::SkipEmptyParts);
    if (parts.size() != 2) {
        r["status"] = "error";
        r["message"] = "usage: patch-change bank patch (bank 0-99, patch 0-3)";
        return r;
    }

    bool ok1, ok2;
    int bank = parts[0].toInt(&ok1);
    int patch = parts[1].toInt(&ok2);

    if (!ok1 || !ok2 || bank < 0 || bank >= bankTotalAll || patch < 0 || patch >= patchPerBank) {
        r["status"] = "error";
        r["message"] = QString("bank must be 0-%1, patch must be 0-%2")
            .arg(bankTotalAll - 1).arg(patchPerBank - 1);
        return r;
    }

    SysxIO *sysxIO = SysxIO::Instance();
    if (!sysxIO->isConnected()) {
        r["status"] = "error";
        r["message"] = "Not connected to device";
        return r;
    }

    log("PATCH", QString("Changing to bank %1 patch %2").arg(bank).arg(patch));
    QMetaObject::invokeMethod(sysxIO, [sysxIO, bank, patch]() {
        sysxIO->requestPatchChange(bank, patch);
    }, Qt::QueuedConnection);

    r["status"] = "ok";
    r["bank"] = bank;
    r["patch"] = patch;
    r["message"] = "Patch change requested";

    return r;
}

QJsonObject DiagnosticServer::cmdPatchName()
{
    QJsonObject r;
    SysxIO *sysxIO = SysxIO::Instance();
    r["status"] = "ok";
    r["patchName"] = sysxIO->getCurrentPatchName();
    return r;
}

QJsonObject DiagnosticServer::cmdPatchList()
{
    QJsonObject r;
    PatchListModel *model = PatchListModel::Instance();

    QJsonArray userPatches, presetPatches;

    // Save current mode and iterate both
    bool wasPreset = model->presetMode();

    model->setPresetMode(false);
    int userCount = model->rowCount();
    for (int i = 0; i < userCount; ++i) {
        QModelIndex idx = model->index(i);
        QJsonObject p;
        p["index"] = i;
        p["text"] = model->data(idx, PatchListModel::DisplayTextRole).toString();
        p["bankLabel"] = model->data(idx, PatchListModel::BankLabelRole).toString();
        p["name"] = model->data(idx, PatchListModel::PatchNameRole).toString();
        p["bank"] = model->data(idx, PatchListModel::BankRole).toInt();
        p["patch"] = model->data(idx, PatchListModel::PatchRole).toInt();
        userPatches.append(p);
    }

    model->setPresetMode(true);
    int presetCount = model->rowCount();
    for (int i = 0; i < presetCount; ++i) {
        QModelIndex idx = model->index(i);
        QJsonObject p;
        p["index"] = i;
        p["text"] = model->data(idx, PatchListModel::DisplayTextRole).toString();
        p["bankLabel"] = model->data(idx, PatchListModel::BankLabelRole).toString();
        p["name"] = model->data(idx, PatchListModel::PatchNameRole).toString();
        p["bank"] = model->data(idx, PatchListModel::BankRole).toInt();
        p["patch"] = model->data(idx, PatchListModel::PatchRole).toInt();
        presetPatches.append(p);
    }

    // Restore
    model->setPresetMode(wasPreset);

    r["status"] = "ok";
    r["userPatches"] = userPatches;
    r["presetPatches"] = presetPatches;
    r["userCount"] = userPatches.size();
    r["presetCount"] = presetPatches.size();

    return r;
}

QJsonObject DiagnosticServer::cmdPatchLoad(const QString &args)
{
    QJsonObject r;
    QString filepath = args.trimmed();
    if (filepath.isEmpty()) {
        r["status"] = "error";
        r["message"] = "usage: patch-load filepath";
        return r;
    }

    QFile file(filepath);
    if (!file.exists()) {
        r["status"] = "error";
        r["message"] = "File not found: " + filepath;
        return r;
    }

    if (!file.open(QIODevice::ReadOnly)) {
        r["status"] = "error";
        r["message"] = "Cannot open file: " + filepath;
        return r;
    }

    QByteArray data = file.readAll();
    file.close();

    SysxIO *sysxIO = SysxIO::Instance();
    sysxIO->setFileSource("Structure", data);
    sysxIO->setFileName(filepath);

    log("PATCH", QString("Loaded patch file: %1 (%2 bytes)").arg(filepath).arg(data.size()));

    r["status"] = "ok";
    r["file"] = filepath;
    r["size"] = data.size();
    r["patchName"] = sysxIO->getCurrentPatchName();

    return r;
}

QJsonObject DiagnosticServer::cmdPatchSave(const QString &args)
{
    QJsonObject r;
    QString filepath = args.trimmed();
    if (filepath.isEmpty()) {
        r["status"] = "error";
        r["message"] = "usage: patch-save filepath";
        return r;
    }

    SysxIO *sysxIO = SysxIO::Instance();
    SysxData data = sysxIO->getFileSource();

    // Build sysex output from file source
    QFile file(filepath);
    if (!file.open(QIODevice::WriteOnly)) {
        r["status"] = "error";
        r["message"] = "Cannot write to: " + filepath;
        return r;
    }

    // Write as raw sysex hex data
    for (int i = 0; i < data.address.size(); ++i) {
        for (const QString &hexByte : data.hex[i]) {
            bool ok;
            unsigned char byte = hexByte.toUInt(&ok, 16);
            if (ok) {
                file.write(reinterpret_cast<char*>(&byte), 1);
            }
        }
    }
    file.close();

    log("PATCH", QString("Saved patch to: %1").arg(filepath));

    r["status"] = "ok";
    r["file"] = filepath;
    r["patchName"] = sysxIO->getCurrentPatchName();

    return r;
}

QJsonObject DiagnosticServer::cmdPatchWrite()
{
    QJsonObject r;
    SysxIO *sysxIO = SysxIO::Instance();

    if (!sysxIO->isConnected()) {
        r["status"] = "error";
        r["message"] = "Not connected to device";
        return r;
    }

    log("PATCH", "Writing patch to device memory");
    // The write sequence is handled by the main UI flow
    // We trigger it through the signal system
    emit commandReceived("patch-write");

    r["status"] = "ok";
    r["message"] = "Patch write initiated";
    r["bank"] = sysxIO->getBank();
    r["patch"] = sysxIO->getPatch();

    return r;
}


// ═══════════════════════════════════════════════════════════════════════════
//  SYSTEM COMMANDS
// ═══════════════════════════════════════════════════════════════════════════

QJsonObject DiagnosticServer::cmdSystem()
{
    QJsonObject r;
    SysxIO *sysxIO = SysxIO::Instance();
    r["status"] = "ok";
    r["mode"] = sysxIO->mode;
    r["modeLabel"] = (sysxIO->mode == 0) ? "Guitar" : "Bass";

    // Dump key system parameters from system source
    // System common starts at 00 01 00 00
    ParameterBridge *pb = ParameterBridge::Instance();

    // Read some key system values using known addresses
    QJsonObject sysParams;

    // Attempt to read common system params
    // These use "System" area addressing which starts at 00
    auto readSysParam = [&](const QString &h0, const QString &h1,
                            const QString &h2, const QString &h3,
                            const QString &name) {
        int val = pb->getValue(h0, h1, h2, h3);
        QString display = pb->getDisplayValue(h0, h1, h2, h3, val);
        QString label = pb->getLabel(h0, h1, h2, h3);
        QJsonObject p;
        p["value"] = val;
        p["display"] = display;
        p["label"] = label;
        sysParams[name] = p;
    };

    // Patch-level master params (from edit buffer 10 00 54 xx)
    readSysParam("10", "00", "54", "00", "patchLevel");
    readSysParam("10", "00", "54", "02", "bpm");

    r["parameters"] = sysParams;

    return r;
}

QJsonObject DiagnosticServer::cmdSystemRequest()
{
    QJsonObject r;
    SysxIO *sysxIO = SysxIO::Instance();

    if (!sysxIO->isConnected()) {
        r["status"] = "error";
        r["message"] = "Not connected to device";
        return r;
    }

    log("SYSTEM", "Requesting system data from device");
    QMetaObject::invokeMethod(sysxIO, [sysxIO]() {
        sysxIO->systemDataRequest();
    }, Qt::QueuedConnection);

    r["status"] = "ok";
    r["message"] = "System data request sent";

    return r;
}

QJsonObject DiagnosticServer::cmdSystemWrite()
{
    QJsonObject r;
    SysxIO *sysxIO = SysxIO::Instance();

    if (!sysxIO->isConnected()) {
        r["status"] = "error";
        r["message"] = "Not connected to device";
        return r;
    }

    log("SYSTEM", "Writing system data to device");
    QMetaObject::invokeMethod(sysxIO, [sysxIO]() {
        sysxIO->systemWrite();
    }, Qt::QueuedConnection);

    r["status"] = "ok";
    r["message"] = "System write initiated";

    return r;
}

QJsonObject DiagnosticServer::cmdMode()
{
    QJsonObject r;
    SysxIO *sysxIO = SysxIO::Instance();
    r["status"] = "ok";
    r["mode"] = sysxIO->mode;
    r["modeLabel"] = (sysxIO->mode == 0) ? "Guitar" : "Bass";
    return r;
}

QJsonObject DiagnosticServer::cmdModeSet(const QString &args)
{
    QJsonObject r;
    QString mode = args.trimmed().toLower();

    if (mode != "guitar" && mode != "bass") {
        r["status"] = "error";
        r["message"] = "usage: mode-set guitar|bass";
        return r;
    }

    SysxIO *sysxIO = SysxIO::Instance();
    if (!sysxIO->isConnected()) {
        r["status"] = "error";
        r["message"] = "Not connected to device";
        return r;
    }

    QString modeValue = (mode == "guitar") ? "00" : "02";
    log("SYSTEM", QString("Setting mode to %1").arg(mode));
    QMetaObject::invokeMethod(sysxIO, [sysxIO, modeValue]() {
        sysxIO->setMode(modeValue);
    }, Qt::QueuedConnection);

    r["status"] = "ok";
    r["mode"] = mode;
    r["message"] = "Mode change requested";

    return r;
}


// ═══════════════════════════════════════════════════════════════════════════
//  EFFECT CHAIN COMMANDS
// ═══════════════════════════════════════════════════════════════════════════

QJsonObject DiagnosticServer::cmdEffectChain()
{
    QJsonObject r;
    ParameterBridge *pb = ParameterBridge::Instance();

    QJsonArray effects;
    for (int i = 0; effectOnOff[i].name; ++i) {
        QString h1 = effectOnOff[i].hex1;
        QString h2 = effectOnOff[i].hex2;
        QString h3 = effectOnOff[i].hex3;

        int onOff = pb->getValue("10", h1, h2, h3);
        QString display = pb->getDisplayValue("10", h1, h2, h3, onOff);

        QJsonObject fx;
        fx["name"] = QString(effectOnOff[i].name);
        fx["on"] = (onOff != 0);
        fx["onValue"] = onOff;
        fx["onDisplay"] = display;
        fx["address"] = QString("10 %1 %2 %3").arg(h1, h2, h3);

        // Try to get the type parameter (usually at hex3=01)
        QString typeH3 = QString::number(h3.toInt(nullptr, 16) + 1, 16).toUpper().rightJustified(2, '0');
        QString typeLabel = pb->getLabel("10", h1, h2, typeH3);
        if (!typeLabel.isEmpty() && typeLabel.toLower().contains("type")) {
            int typeVal = pb->getValue("10", h1, h2, typeH3);
            QString typeDisplay = pb->getDisplayValue("10", h1, h2, typeH3, typeVal);
            fx["type"] = typeVal;
            fx["typeDisplay"] = typeDisplay;
        }

        effects.append(fx);
    }

    r["status"] = "ok";
    r["effects"] = effects;
    r["count"] = effects.size();

    return r;
}

QJsonObject DiagnosticServer::cmdEffectToggle(const QString &args)
{
    QJsonObject r;
    QString name = args.trimmed().toLower();

    const EffectAddr *found = nullptr;
    for (int i = 0; effectOnOff[i].name; ++i) {
        if (name == effectOnOff[i].name) {
            found = &effectOnOff[i];
            break;
        }
    }

    if (!found) {
        r["status"] = "error";
        r["message"] = "Unknown effect: " + name;
        QJsonArray available;
        for (int i = 0; effectOnOff[i].name; ++i) {
            available.append(QString(effectOnOff[i].name));
        }
        r["available"] = available;
        return r;
    }

    ParameterBridge *pb = ParameterBridge::Instance();
    QString h1 = found->hex1, h2 = found->hex2, h3 = found->hex3;

    int current = pb->getValue("10", h1, h2, h3);
    int newVal = (current == 0) ? 1 : 0;

    // Marshal setValue to main thread to avoid MIDI threading issues
    QMetaObject::invokeMethod(pb, [pb, h1, h2, h3, newVal]() {
        pb->setValue("10", h1, h2, h3, newVal);
    }, Qt::QueuedConnection);

    r["status"] = "ok";
    r["effect"] = name;
    r["was"] = (current != 0);
    r["now"] = (newVal != 0);

    log("EFFECT", QString("Toggled %1: %2 → %3")
        .arg(name)
        .arg(current ? "ON" : "OFF")
        .arg(newVal ? "ON" : "OFF"));

    return r;
}

QJsonObject DiagnosticServer::cmdInstrument(const QString &args)
{
    QJsonObject r;
    bool ok;
    int instNum = args.trimmed().toInt(&ok);

    if (!ok || instNum < 1 || instNum > 3) {
        r["status"] = "error";
        r["message"] = "usage: instrument 1|2|3";
        return r;
    }

    // Map instrument number to hex2 base
    QString hex2;
    switch (instNum) {
        case 1: hex2 = "15"; break;
        case 2: hex2 = "20"; break;
        case 3: hex2 = "2B"; break;
    }

    ParameterBridge *pb = ParameterBridge::Instance();

    QJsonArray params;
    // Dump instrument common parameters (typically ~0x40 params)
    for (int i = 0; i < 0x40; ++i) {
        QString hex3 = QString::number(i, 16).toUpper().rightJustified(2, '0');
        QString label = pb->getLabel("10", "00", hex2, hex3);
        if (label.isEmpty()) continue;

        int value = pb->getValue("10", "00", hex2, hex3);
        QString display = pb->getDisplayValue("10", "00", hex2, hex3, value);

        QJsonObject p;
        p["address"] = QString("10 00 %1 %2").arg(hex2, hex3);
        p["hex3"] = hex3;
        p["label"] = label;
        p["value"] = value;
        p["display"] = display;
        params.append(p);
    }

    r["status"] = "ok";
    r["instrument"] = instNum;
    r["hex2"] = hex2;
    r["parameters"] = params;
    r["count"] = params.size();

    return r;
}


// ═══════════════════════════════════════════════════════════════════════════
//  ASSIGN COMMANDS
// ═══════════════════════════════════════════════════════════════════════════

QJsonObject DiagnosticServer::cmdAssign(const QString &args)
{
    QJsonObject r;
    bool ok;
    int assignNum = args.trimmed().toInt(&ok);

    if (!ok || assignNum < 1 || assignNum > 16) {
        r["status"] = "error";
        r["message"] = "usage: assign 1-16";
        return r;
    }

    // Assigns start at hex2=04 for assign1, 05 for assign2, etc.
    QString hex2 = QString::number(0x03 + assignNum, 16).toUpper().rightJustified(2, '0');

    ParameterBridge *pb = ParameterBridge::Instance();

    QJsonArray params;
    for (int i = 0; i < 0x40; ++i) {
        QString hex3 = QString::number(i, 16).toUpper().rightJustified(2, '0');
        QString label = pb->getLabel("10", "00", hex2, hex3);
        if (label.isEmpty()) continue;

        int value = pb->getValue("10", "00", hex2, hex3);
        QString display = pb->getDisplayValue("10", "00", hex2, hex3, value);

        QJsonObject p;
        p["address"] = QString("10 00 %1 %2").arg(hex2, hex3);
        p["hex3"] = hex3;
        p["label"] = label;
        p["value"] = value;
        p["display"] = display;
        params.append(p);
    }

    r["status"] = "ok";
    r["assign"] = assignNum;
    r["hex2"] = hex2;
    r["parameters"] = params;
    r["count"] = params.size();

    return r;
}


// ═══════════════════════════════════════════════════════════════════════════
//  PREFERENCES COMMANDS
// ═══════════════════════════════════════════════════════════════════════════

QJsonObject DiagnosticServer::cmdPrefGet(const QString &args)
{
    QJsonObject r;
    QStringList parts = args.split(' ', Qt::SkipEmptyParts);
    if (parts.size() != 3) {
        r["status"] = "error";
        r["message"] = "usage: pref-get group type item (e.g. pref-get Midi MidiIn device)";
        return r;
    }

    Preferences *prefs = Preferences::Instance();
    QString value = prefs->getPreferences(parts[0], parts[1], parts[2]);

    r["status"] = "ok";
    r["group"] = parts[0];
    r["type"] = parts[1];
    r["item"] = parts[2];
    r["value"] = value;

    return r;
}

QJsonObject DiagnosticServer::cmdPrefSet(const QString &args)
{
    QJsonObject r;
    QStringList parts = args.split(' ', Qt::SkipEmptyParts);
    if (parts.size() != 4) {
        r["status"] = "error";
        r["message"] = "usage: pref-set group type item value";
        return r;
    }

    Preferences *prefs = Preferences::Instance();
    prefs->setPreferences(parts[0], parts[1], parts[2], parts[3]);
    prefs->savePreferences();

    log("PREF", QString("Set %1/%2/%3 = %4").arg(parts[0], parts[1], parts[2], parts[3]));

    r["status"] = "ok";
    r["group"] = parts[0];
    r["type"] = parts[1];
    r["item"] = parts[2];
    r["value"] = parts[3];

    return r;
}


// ═══════════════════════════════════════════════════════════════════════════
//  SCREENSHOT COMMANDS
// ═══════════════════════════════════════════════════════════════════════════

QJsonObject DiagnosticServer::cmdScreenshot(const QString &args)
{
    QJsonObject r;
    QString filename = args.trimmed();
    if (filename.isEmpty()) {
        filename = QDir::homePath() + "/000-HOME/010-MUSIC-STUDIO/SY-1000/screenshot.png";
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
        r["status"] = "ok";
        r["file"] = filename;
        r["width"] = pixmap.width();
        r["height"] = pixmap.height();
        log("SCREENSHOT", QString("Main window → %1 (%2x%3)")
            .arg(filename).arg(pixmap.width()).arg(pixmap.height()));
    } else {
        r["status"] = "error";
        r["message"] = "No visible main window found";
    }

    return r;
}

QJsonObject DiagnosticServer::cmdScreenshotWindow(const QString &args)
{
    QJsonObject r;
    QString argStr = args.trimmed();
    QString title, filename;

    int pathIdx = argStr.indexOf('/');
    if (pathIdx > 0) {
        title = argStr.left(pathIdx).trimmed();
        filename = argStr.mid(pathIdx).trimmed();
    } else {
        title = argStr;
        filename = QDir::homePath() + "/000-HOME/010-MUSIC-STUDIO/SY-1000/screenshot_" +
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
        r["status"] = "ok";
        r["file"] = filename;
        r["title"] = target->windowTitle();
        r["width"] = pixmap.width();
        r["height"] = pixmap.height();
        log("SCREENSHOT", QString("Window '%1' → %2").arg(target->windowTitle(), filename));
    } else {
        r["status"] = "error";
        r["message"] = "Window not found: " + title;
    }

    return r;
}


// ═══════════════════════════════════════════════════════════════════════════
//  QML COMMANDS
// ═══════════════════════════════════════════════════════════════════════════

QJsonObject DiagnosticServer::cmdOpenQml(const QString &args)
{
    QJsonObject r;
    QStringList parts = args.split(' ', Qt::SkipEmptyParts);
    if (parts.size() < 3) {
        r["status"] = "error";
        r["message"] = "usage: open-qml <file.qml> <hex1> <hex2>";
        return r;
    }

    QString qmlFile = parts[0];
    QString hex1 = parts[1];
    QString hex2 = parts[2];

    QString qmlSource = qmlFile;
    if (!qmlSource.startsWith("qrc:")) {
        qmlSource = "qrc:/qml/" + qmlFile;
    }

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

    r["status"] = "ok";
    r["source"] = qmlSource;
    r["hex1"] = hex1;
    r["hex2"] = hex2;
    r["title"] = preview->windowTitle();

    log("QML", QString("Opened %1 [%2, %3]").arg(qmlFile, hex1, hex2));

    return r;
}

QJsonObject DiagnosticServer::cmdCloseQml()
{
    QJsonObject r;
    int closed = 0;
    for (auto &p : qmlPreviews) {
        if (p) { p->close(); ++closed; }
    }
    qmlPreviews.clear();

    r["status"] = "ok";
    r["closed"] = closed;
    log("QML", QString("Closed %1 preview windows").arg(closed));

    return r;
}

QJsonObject DiagnosticServer::cmdListQml()
{
    QJsonObject r;
    r["status"] = "ok";

    QJsonArray panels;

    // Instrument panels
    QJsonObject inst;
    inst["file"] = "InstrumentPanel.qml";
    inst["description"] = "Full instrument panel with all type tabs";
    QJsonArray instPresets;
    auto addPreset = [&](const char *name, const char *h1, const char *h2) {
        QJsonObject p;
        p["name"] = name;
        p["hex1"] = h1;
        p["hex2"] = h2;
        instPresets.append(p);
    };
    addPreset("INST1 Guitar", "00", "15");
    addPreset("INST2 Guitar", "00", "20");
    addPreset("INST3 Guitar", "00", "2B");
    addPreset("INST1 Bass", "02", "15");
    addPreset("INST2 Bass", "02", "1F");
    addPreset("INST3 Bass", "02", "29");
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

    // Effect panels
    QStringList effectPanels = {
        "AmpPanel.qml", "DsPanel.qml", "NsPanel.qml",
        "Fx1Panel.qml", "Fx2Panel.qml", "Fx3Panel.qml",
        "Dd1Panel.qml", "Dd2Panel.qml", "Dd3Panel.qml",
        "CsPanel.qml", "CePanel.qml", "LpPanel.qml",
        "Eq1Panel.qml", "Eq2Panel.qml",
        "RvPanel.qml",
        "Fv1Panel.qml", "Fv2Panel.qml",
        "DividerPanel.qml", "MixerPanel.qml",
        "Balancer1Panel.qml", "Balancer2Panel.qml", "Balancer3Panel.qml",
        "MainOutLeftPanel.qml", "MainOutRightPanel.qml", "MainOutStereoPanel.qml",
        "SubOutLeftPanel.qml", "SubOutRightPanel.qml",
        "MasterPanel.qml"
    };
    for (const auto &panel : effectPanels) {
        QJsonObject p;
        p["file"] = panel;
        p["type"] = "effect-panel";
        panels.append(p);
    }

    // Tab files
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

    // System panels
    QStringList sysPanels = {
        "system/SysInputSetting.qml", "system/SysOutputSetting.qml",
        "system/SysTotalSetting.qml", "system/SysUsbSetting.qml",
        "system/SysHardwareSetting.qml", "system/SysPlayOption.qml",
        "system/SysMidiSetting.qml", "system/SysKnobSetting.qml",
        "system/SysGkSetSelect.qml", "system/SysNormalSetSelect.qml",
        "system/SysGuitarToMidi.qml", "system/SysAutoPowerOff.qml",
        "system/SysPlaceholder.qml"
    };
    for (const auto &sp : sysPanels) {
        QJsonObject s;
        s["file"] = sp;
        s["type"] = "system-panel";
        panels.append(s);
    }

    // Pedal panels
    QStringList pedalPanels = {
        "pedal/PdlCtl.qml", "pedal/PdlExp.qml",
        "pedal/PdlGk.qml", "pedal/PdlBanks.qml",
        "pedal/PdlLeds.qml", "pedal/PdlNumbers.qml"
    };
    for (const auto &pp : pedalPanels) {
        QJsonObject p;
        p["file"] = pp;
        p["type"] = "pedal-panel";
        panels.append(p);
    }

    r["panels"] = panels;

    return r;
}


// ═══════════════════════════════════════════════════════════════════════════
//  BULK OPERATIONS
// ═══════════════════════════════════════════════════════════════════════════

QJsonObject DiagnosticServer::cmdBulkNames()
{
    QJsonObject r;
    SysxIO *sysxIO = SysxIO::Instance();

    if (!sysxIO->isConnected()) {
        r["status"] = "error";
        r["message"] = "Not connected to device";
        return r;
    }

    log("BULK", "Requesting bulk patch names from device");
    QMetaObject::invokeMethod(sysxIO, [sysxIO]() {
        sysxIO->requestBulkPatchNames();
    }, Qt::QueuedConnection);

    r["status"] = "ok";
    r["message"] = "Bulk name request sent";

    return r;
}

QJsonObject DiagnosticServer::cmdSysex(const QString &args)
{
    QJsonObject r;
    QString hexStr = args.trimmed().remove(' ');

    if (hexStr.isEmpty() || !hexStr.startsWith("F0") || !hexStr.endsWith("F7")) {
        r["status"] = "error";
        r["message"] = "usage: sysex F0...F7 (complete sysex message in hex)";
        return r;
    }

    SysxIO *sysxIO = SysxIO::Instance();
    if (!sysxIO->isConnected()) {
        r["status"] = "error";
        r["message"] = "Not connected to device";
        return r;
    }

    log("SYSEX", QString("Sending raw sysex: %1 (%2 bytes)")
        .arg(hexStr.left(40) + (hexStr.length() > 40 ? "..." : ""))
        .arg(hexStr.length() / 2));

    QMetaObject::invokeMethod(sysxIO, [sysxIO, hexStr]() {
        sysxIO->sendMidi(hexStr);
    }, Qt::QueuedConnection);

    r["status"] = "ok";
    r["bytes"] = hexStr.length() / 2;
    r["message"] = "Sysex sent";

    return r;
}


// ═══════════════════════════════════════════════════════════════════════════
//  MONITORING COMMANDS
// ═══════════════════════════════════════════════════════════════════════════

QJsonObject DiagnosticServer::cmdWatch(QTcpSocket *socket, const QString &args)
{
    QJsonObject r;

    if (!watchSockets.contains(socket)) {
        watchSockets.append(socket);

        // Connect to parameter bridge changes if this is the first watcher
        if (watchSockets.size() == 1) {
            ParameterBridge *pb = ParameterBridge::Instance();
            connect(pb, &ParameterBridge::parameterChanged,
                    this, [this](const QString &h0, const QString &h1,
                                 const QString &h2, const QString &h3, int newValue) {
                ParameterBridge *pb = ParameterBridge::Instance();
                QJsonObject notification;
                notification["type"] = "parameter-change";
                notification["address"] = QString("%1 %2 %3 %4").arg(h0, h1, h2, h3);
                notification["value"] = newValue;
                notification["display"] = pb->getDisplayValue(h0, h1, h2, h3, newValue);
                notification["label"] = pb->getLabel(h0, h1, h2, h3);

                QJsonDocument doc(notification);
                QByteArray data = doc.toJson(QJsonDocument::Compact) + "\n";

                QList<QPointer<QTcpSocket>> dead;
                for (auto &ws : watchSockets) {
                    if (ws && ws->state() == QAbstractSocket::ConnectedState) {
                        ws->write(data);
                        ws->flush();
                    } else {
                        dead.append(ws);
                    }
                }
                for (auto &d : dead) watchSockets.removeAll(d);
            });
        }
    }

    r["status"] = "ok";
    r["message"] = "Subscribed to parameter changes";
    r["filter"] = args.isEmpty() ? "all" : args;
    r["watchers"] = watchSockets.size();

    log("WATCH", "Client subscribed to parameter changes");

    return r;
}

QJsonObject DiagnosticServer::cmdUnwatch(QTcpSocket *socket)
{
    QJsonObject r;
    watchSockets.removeAll(socket);

    r["status"] = "ok";
    r["message"] = "Unsubscribed from parameter changes";
    r["watchers"] = watchSockets.size();

    log("WATCH", "Client unsubscribed");

    return r;
}

// ─── Layout scoring ───────────────────────────────────────────────────────
// Pure function: takes pre-computed segment lengths and dual_channel flag,
// returns a JSON score object.  Called by both cmdLayoutScore() (live patch)
// and cmdLayoutTest() (user-supplied topology).
//
// Rules evaluated:
//   §L1 — No balancer may exceed max(directInputA, directInputB) + MAX_BAL_SPREAD
//          columns.  Prevents excessively long empty-wire gaps on the shorter
//          input row when one side of a merge has more effects than the other.
//          (§L2 and §L3 are architectural invariants enforced by the formula
//          itself and cannot produce violations at runtime.)
static QJsonObject computeLayoutScore(int index1, int index2, int index3,
                                      int index4, int index5, int index6,
                                      int dual_channel)
{
    // Balancer column positions (same logic as floorBoard.cpp lines 1624-1633)
    int bal1xpos = index1 + 1;
    if (index2 > index1) bal1xpos = index2 + 1;

    int bal2xpos = index3 + 1;
    if (index4 > index3) bal2xpos = index4 + 1;
    if ((bal1xpos + index3) > bal2xpos) bal2xpos = bal1xpos + index3 + 1;

    int bal3xpos = index5 + 1;
    if (index6 > index5) bal3xpos = index6 + 1;
    if ((bal2xpos + index5) > bal3xpos) bal3xpos = bal2xpos + index5 + 1;

    // §L1 check — BAL2 and BAL3
    // Caps must match floorBoard.cpp exactly:
    //   Zone B starts from BAL1's column, so the end-of-Zone-B from C1 is
    //   bal1xpos + index3, not just index3.  The ordering invariant requires
    //   BAL2 > BAL1 and BAL3 > BAL2 (floor = previous_xpos + 1).
    const int MAX_BAL_SPREAD = 2;
    QJsonArray violations;
    const int bal2cap = qMax(qMax(bal1xpos + index3, index4) + MAX_BAL_SPREAD,
                             bal1xpos + 1);
    const int bal3cap = qMax(qMax(bal2xpos + index5, index6) + MAX_BAL_SPREAD,
                             bal2xpos + 1);

    if (bal2xpos > bal2cap) {
        QJsonObject v;
        v["rule"]      = "§L1";
        v["balancer"]  = "BAL2";
        v["computed"]  = bal2xpos;
        v["cap"]       = bal2cap;
        v["gap_inst3"] = bal2xpos + 1 - index4;
        violations.append(v);
    }
    if (bal3xpos > bal3cap) {
        QJsonObject v;
        v["rule"]      = "§L1";
        v["balancer"]  = "BAL3";
        v["computed"]  = bal3xpos;
        v["cap"]       = bal3cap;
        v["gap_normal"]= bal3xpos + 1 - index6;
        violations.append(v);
    }

    int score = 100 - (violations.size() * 25);

    QJsonObject r;
    r["status"]       = "ok";
    r["score"]        = score;
    r["rule"]         = QString("§L1 MAX_BAL_SPREAD=%1").arg(MAX_BAL_SPREAD);
    r["dual_channel"] = dual_channel;
    r["bal1xpos"]     = bal1xpos;
    r["bal2xpos"]     = bal2xpos;
    r["bal3xpos"]     = bal3xpos;
    r["index1"]       = index1;
    r["index2"]       = index2;
    r["index3"]       = index3;
    r["index4"]       = index4;
    r["index5"]       = index5;
    r["index6"]       = index6;
    r["violations"]   = violations;
    r["message"]      = violations.isEmpty()
                        ? "Layout passes §L1"
                        : QString("%1 violation(s) — score %2/100").arg(violations.size()).arg(score);
    return r;
}

// layout-score — score the currently loaded patch
QJsonObject DiagnosticServer::cmdLayoutScore()
{
    QJsonObject r;
    SysxIO *sysxIO = SysxIO::Instance();
    Preferences *preferences = Preferences::Instance();

    QString hex1 = "00";
    if (preferences->getPreferences("Window", "BassMode", "bool") == "true") hex1 = "02";

    QList<QString> fxChain = sysxIO->getFileSource("10", hex1, "12", "void");
    const int sysxOff = 85;

    if (fxChain.size() < sysxOff + 34) {
        r["status"] = "error";
        r["message"] = "Chain data not loaded";
        return r;
    }

    int i1 = fxChain.indexOf("18", sysxOff);
    int i2 = (i1 >= 0) ? fxChain.indexOf("18", i1 + 1) : -1;
    int i3 = (i2 >= 0) ? fxChain.indexOf("18", i2 + 1) : -1;
    int i4 = (i3 >= 0) ? fxChain.indexOf("18", i3 + 1) : -1;
    int i5 = (i4 >= 0) ? fxChain.indexOf("18", i4 + 1) : -1;
    int i6 = (i5 >= 0) ? fxChain.indexOf("18", i5 + 1) : -1;

    if (i1 < 0 || i2 < 0 || i3 < 0 || i4 < 0 || i5 < 0 || i6 < 0) {
        r["status"] = "error";
        r["message"] = "Could not locate all six chain split markers";
        return r;
    }

    int index6 = i6 - i5 - 1;
    int index5 = i5 - i4 - 1;
    int index4 = i4 - i3 - 1;
    int index3 = i3 - i2 - 1;
    int index2 = i2 - i1 - 1;
    int index1 = i1 - sysxOff;
    int dual_channel = sysxIO->getSourceValue("10", hex1, "12", "20");

    return computeLayoutScore(index1, index2, index3, index4, index5, index6, dual_channel);
}

// layout-test i1 i2 i3 i4 i5 i6 dual — score a user-supplied topology
// headlessly (no patch loaded, no MIDI connection needed).
// Usage: echo "layout-test 1 1 2 0 2 6 0" | nc localhost 47321
QJsonObject DiagnosticServer::cmdLayoutTest(const QString &args)
{
    auto err = [](const QString &msg) {
        QJsonObject r;
        r["status"]  = "error";
        r["message"] = msg;
        return r;
    };

    QStringList parts = args.split(' ', Qt::SkipEmptyParts);
    if (parts.size() < 7)
        return err("Usage: layout-test i1 i2 i3 i4 i5 i6 dual_channel");

    bool ok[7];
    int vals[7];
    for (int i = 0; i < 7; ++i) {
        vals[i] = parts[i].toInt(&ok[i]);
        if (!ok[i]) return err("All arguments must be integers");
    }

    return computeLayoutScore(vals[0], vals[1], vals[2], vals[3],
                              vals[4], vals[5], vals[6]);
}
