#include <QTcpServer>
#include <QTcpSocket>
#include <QSslSocket>
#include <QNetworkRequest>
#include "private/cutesslserver.h"
#include "private/cutehttprequest.h"
#include "private/cutehttpresponse.h"
#include "cutemockserver.h"

CuteMockServer::CuteMockServer(QObject *parent/*= nullptr*/)
    : QObject(parent)
    , m_tcpServer(new QTcpServer(this))
    , m_routes()
{
    Q_INIT_RESOURCE(certificates);
    m_sslServer = new CuteSslServer(this);
    connect(m_tcpServer, &QTcpServer::newConnection, this, &CuteMockServer::httpRequest);
    connect(m_sslServer, &QTcpServer::newConnection, this, &CuteMockServer::secureHttpRequest);
}

bool CuteMockServer::listenHttp(const ushort port)
{
    const bool started = m_tcpServer->listen(QHostAddress::Any, port);
    if (!started)
        qCritical() << "Could not start Cute Mock HTTP server on port" << port << m_tcpServer->errorString();
    return started;
}

bool CuteMockServer::listenHttps(const ushort port)
{
    const bool started = m_sslServer->listen(QHostAddress::Any, port);
    if (!started)
        qCritical() << "Could not start Cute Mock HTTPS server on port" << port << m_sslServer->errorString();
    return started;
}

void CuteMockServer::setHttpRoute(CuteMockData::Method requestMethod, const QUrl &uri, const CuteMockData &replyMockData)
{
    m_routes.insert(requestMethod, {{uri, replyMockData}});
}

void CuteMockServer::configureSecureRequest(QNetworkRequest *request) const
{
    if (nullptr == request)
        qCritical() << "Cannot configure nullptr";
    else
        m_sslServer->configureRequest(*request);
}

void CuteMockServer::httpRequest()
{
    QTcpSocket *client = m_tcpServer->nextPendingConnection();
    client->setParent(this);
    CuteHttpRequest *httpRequest = new CuteHttpRequest(client);
    connect(httpRequest, &CuteHttpRequest::parsed, this, &CuteMockServer::httpResponse);
}

void CuteMockServer::httpResponse(QTcpSocket *client, const CuteHttpRequest &request)
{
    CuteHttpResponse response;
    auto methodRoute = m_routes.find(request.getMethod());
    if (methodRoute != m_routes.cend()) {
        auto mockData = methodRoute->find(request.getUri());
        if (mockData != methodRoute->cend()) {
            response = CuteHttpResponse(*mockData);
        }
    }
    client->write(response.data());
    client->flush();
}

void CuteMockServer::secureHttpRequest()
{
    QSslSocket *client = qobject_cast<QSslSocket *>(m_sslServer->nextPendingConnection());
    client->setParent(this);
    CuteHttpRequest *httpRequest = new CuteHttpRequest(client);
    connect(httpRequest, &CuteHttpRequest::parsed, this, &CuteMockServer::httpResponse);
}
