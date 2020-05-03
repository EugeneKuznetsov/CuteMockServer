#pragma once

#include <QObject>
#include <QUrl>
#include <QMap>
#include "private/cutemockdata.h"

class QTcpServer;
class QTcpSocket;
class QNetworkRequest;
class CuteSslServer;
class CuteHttpRequest;

typedef QMap<QUrl, CuteMockData> ResourceData;
typedef QMap<CuteMockData::Method, ResourceData> ResourceRoute;

class CuteMockServer : public QObject
{
    Q_OBJECT

public:
    explicit CuteMockServer(QObject *parent = nullptr);

    bool listenHttp(const ushort port);
    bool listenHttps(const ushort port);

    void setHttpRoute(CuteMockData::Method requestMethod, const QUrl &uri, const CuteMockData &replyMockData);

    void configureSecureRequest(QNetworkRequest *request) const;

private slots:
    void httpRequest();
    void httpResponse(QTcpSocket *client, const CuteHttpRequest &request);

    void secureHttpRequest();

private:
    QTcpServer      *m_tcpServer;
    CuteSslServer   *m_sslServer;
    ResourceRoute       m_routes;
};
