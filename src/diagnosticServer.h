#ifndef DIAGNOSTICSERVER_H
#define DIAGNOSTICSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QString>
#include <QList>
#include <QPointer>

class QmlHost;

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
    QList<QPointer<QmlHost>> qmlPreviews;
};

#endif // DIAGNOSTICSERVER_H
