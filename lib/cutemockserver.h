#pragma once

#include <QObject>

class QTcpServer;
class CuteSslServer;

class CuteMockServer : public QObject
{
    Q_OBJECT

public:
    explicit CuteMockServer(QObject *parent = nullptr);

    bool listenHttp(const ushort port);
    bool listenHttps(const ushort port);

private:
    QTcpServer      *m_tcpServer;
    CuteSslServer   *m_sslServer;
};
