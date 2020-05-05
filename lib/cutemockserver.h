#pragma once

#include <QObject>
#include <QUrl>
#include <QFile>

class QTcpServer;
class QTcpSocket;
class QNetworkRequest;
class CuteSslServer;
class CuteHttpRequest;
class CuteHttpRouter;
class CuteFile;

class CuteMockServer : public QObject
{
    Q_OBJECT

public:
    explicit CuteMockServer(QObject *parent = nullptr);
    virtual ~CuteMockServer();

    static void registerTypes();

    void configureSecureRequest(QNetworkRequest *request) const;

public slots:
    bool listenHttp(const ushort port);
    bool listenHttps(const ushort port);

    void setHttpRoute(const QString &method, const QUrl &uri, const int statusCode, const QString &contentType, const QByteArray &content);

private slots:
    void secureHttpRequest();
    void httpRequest();
    void httpResponse(QTcpSocket *client, const CuteHttpRequest &request);

private:
    QTcpServer      *m_tcpServer;
    CuteSslServer   *m_sslServer;
    CuteHttpRouter  *m_router;
};
