#pragma once

#include <QObject>

class QTcpServer;
class CuteSslServer;

class CuteMockServer : public QObject
{
    Q_OBJECT

public:
    explicit CuteMockServer(QObject *parent = nullptr);

    bool listen_http(const ushort port);
    bool listen_https(const ushort port);

private:
    QTcpServer      *m_tcpServer;
    CuteSslServer   *m_sslServer;
};
