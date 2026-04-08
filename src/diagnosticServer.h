#ifndef DIAGNOSTICSERVER_H
#define DIAGNOSTICSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QString>
#include <QList>
#include <QPointer>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QMutex>
#include <QElapsedTimer>

class QmlHost;

class DiagnosticServer : public QObject
{
    Q_OBJECT

public:
    explicit DiagnosticServer(QObject *parent = nullptr);
    ~DiagnosticServer();
    bool start(quint16 port = 47321);
    void log(const QString &category, const QString &message);

signals:
    void commandReceived(const QString &command);
    void connectRequest();
    void disconnectRequest();
    void patchChangeRequest(int bank, int patch);

private slots:
    void onNewConnection();
    void onReadyRead();
    void onClientDisconnected();

private:
    void handleCommand(QTcpSocket *socket, const QString &command);
    void sendResponse(QTcpSocket *socket, const QJsonObject &response);

    // Command handlers organized by category
    QJsonObject cmdPing();
    QJsonObject cmdStatus();
    QJsonObject cmdHelp();
    QJsonObject cmdLog(const QString &args);
    QJsonObject cmdLogLevel(const QString &args);

    // Connection commands
    QJsonObject cmdConnect();
    QJsonObject cmdDisconnect();
    QJsonObject cmdMidiDevices();
    QJsonObject cmdMidiPorts();

    // Parameter commands
    QJsonObject cmdGet(const QString &args);
    QJsonObject cmdSet(const QString &args);
    QJsonObject cmdDump(const QString &args);
    QJsonObject cmdDumpSection(const QString &args);

    // Patch commands
    QJsonObject cmdPatch();
    QJsonObject cmdPatchChange(const QString &args);
    QJsonObject cmdPatchName();
    QJsonObject cmdPatchList();
    QJsonObject cmdPatchLoad(const QString &args);
    QJsonObject cmdPatchSave(const QString &args);
    QJsonObject cmdPatchWrite();

    // System commands
    QJsonObject cmdSystem();
    QJsonObject cmdSystemRequest();
    QJsonObject cmdSystemWrite();
    QJsonObject cmdMode();
    QJsonObject cmdModeSet(const QString &args);

    // Effect chain commands
    QJsonObject cmdEffectChain();
    QJsonObject cmdEffectToggle(const QString &args);
    QJsonObject cmdInstrument(const QString &args);

    // Assign commands
    QJsonObject cmdAssign(const QString &args);

    // Preferences commands
    QJsonObject cmdPrefGet(const QString &args);
    QJsonObject cmdPrefSet(const QString &args);

    // Screenshot commands
    QJsonObject cmdScreenshot(const QString &args);
    QJsonObject cmdScreenshotWindow(const QString &args);

    // QML commands
    QJsonObject cmdOpenQml(const QString &args);
    QJsonObject cmdCloseQml();
    QJsonObject cmdListQml();

    // Bulk operations
    QJsonObject cmdBulkNames();
    QJsonObject cmdIdentity();
    QJsonObject cmdSysex(const QString &args);

    // Monitoring
    QJsonObject cmdWatch(QTcpSocket *socket, const QString &args);
    QJsonObject cmdUnwatch(QTcpSocket *socket);

    // Layout diagnostics
    QJsonObject cmdLayoutScore();
    QJsonObject cmdLayoutTest(const QString &args);
    QJsonObject cmdChainPositions();

    QTcpServer *server;
    QList<QPointer<QmlHost>> qmlPreviews;

    // Logging
    QFile *logFile;
    QTextStream *logStream;
    QMutex logMutex;
    QString logFilePath;
    int logLevel; // 0=off, 1=errors, 2=commands, 3=verbose
    QElapsedTimer uptime;

    // Watchers - sockets subscribed to parameter changes
    QList<QPointer<QTcpSocket>> watchSockets;
    QStringList watchFilters;

    int clientCount;
};

#endif // DIAGNOSTICSERVER_H
