#pragma once

#include <QObject>

class QTcpServer;
class QTcpSocket;
class CuteSslServer;
class CuteHttpRequest;

class CuteMockServer : public QObject
{
    Q_OBJECT

public:
    explicit CuteMockServer(QObject *parent = nullptr);

    bool listenHttp(const ushort port);
    bool listenHttps(const ushort port);

private slots:
    void httpRequest();
    void httpResponse(QTcpSocket *client, const CuteHttpRequest &request);

private:
    QTcpServer      *m_tcpServer;
    CuteSslServer   *m_sslServer;
};
