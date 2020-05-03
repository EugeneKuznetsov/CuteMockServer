#pragma once

#include <QObject>

class QTcpServer;
class QTcpSocket;
class QNetworkRequest;
class CuteSslServer;
class CuteHttpRequest;

class CuteMockServer : public QObject
{
    Q_OBJECT

public:
    explicit CuteMockServer(QObject *parent = nullptr);

    bool listenHttp(const ushort port);
    bool listenHttps(const ushort port);

    void configureSecureRequest(QNetworkRequest *request) const;

private slots:
    void httpRequest();
    void httpResponse(QTcpSocket *client, const CuteHttpRequest &request);

    void secureHttpRequest();

private:
    QTcpServer      *m_tcpServer;
    CuteSslServer   *m_sslServer;
};
