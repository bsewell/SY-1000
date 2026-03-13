#ifndef DIAGNOSTICSERVER_H
#define DIAGNOSTICSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QString>

class DiagnosticServer : public QObject
{
    Q_OBJECT

public:
    explicit DiagnosticServer(QObject *parent = nullptr);
    bool start(quint16 port = 47321);

signals:
    void commandReceived(const QString &command);

private slots:
    void onNewConnection();
    void onReadyRead();

private:
    void handleCommand(QTcpSocket *socket, const QString &command);
    QTcpServer *server;
};

#endif // DIAGNOSTICSERVER_H
