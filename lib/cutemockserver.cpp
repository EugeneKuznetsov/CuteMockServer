#include <QQmlEngine>
#include <QTcpServer>
#include <QTcpSocket>
#include <QSslSocket>
#include <QNetworkRequest>
#include "private/cutesslserver.h"
#include "private/cutehttprequest.h"
#include "private/cutehttpresponse.h"
#include "private/cutehttprouter.h"
#include "cutemockserver.h"

CuteMockServer::CuteMockServer(QObject *parent/*= nullptr*/)
    : QObject(parent)
    , m_tcpServer(new QTcpServer(this))
    , m_router(new CuteHttpRouter())
{
    Q_INIT_RESOURCE(certificates);
    m_sslServer = new CuteSslServer(this);
    connect(m_tcpServer, &QTcpServer::newConnection, this, &CuteMockServer::httpRequest);
    connect(m_sslServer, &QTcpServer::newConnection, this, &CuteMockServer::secureHttpRequest);
}

CuteMockServer::~CuteMockServer()
{
    delete m_router;
}

void CuteMockServer::registerTypes()
{
    // ToDo: start using generated version header instead of hardcoded numbers
    qmlRegisterType<CuteMockServer>("CuteMockServer", 0, 4, "CuteMockServer");
}

void CuteMockServer::configureSecureRequest(QNetworkRequest *request) const
{
    if (nullptr == request)
        qCritical() << "Cannot configure nullptr";
    else
        m_sslServer->configureRequest(*request);
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

void CuteMockServer::setHttpRoute(const QString &method, const QUrl &uri, const int statusCode, const QString &contentType, const QByteArray &content)
{
    m_router->set(method, uri, statusCode, contentType, content);
}

void CuteMockServer::setHttpRoute(const QString &method, const QUrl &uri, const int statusCode, const QString &contentType, QFile &content)
{
    m_router->set(method, uri, statusCode, contentType, content);
}

void CuteMockServer::secureHttpRequest()
{
    QSslSocket *client = qobject_cast<QSslSocket *>(m_sslServer->nextPendingConnection());
    client->setParent(this);
    CuteHttpRequest *httpRequest = new CuteHttpRequest(client);
    connect(httpRequest, &CuteHttpRequest::parsed, this, &CuteMockServer::httpResponse);
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
    const CuteHttpResponse &response = m_router->find(request.getMethod(), request.getUri());
    client->write(response.data());
    client->flush();
}
