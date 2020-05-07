#include <QQmlEngine>
#include <QTcpServer>
#include <QTcpSocket>
#include <QSslSocket>
#include <QNetworkRequest>
#include "private/cutesslserver.h"
#include "private/cutehttprequest.h"
#include "private/cutehttpresponse.h"
#include "private/cutehttprouter.h"
#include "cutefile.h"
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
    qmlRegisterType<CuteMockServer>("CuteMockServer", 0, 5, "CuteMockServer");
    qmlRegisterType<CuteFile>("CuteMockServer", 0, 5, "CuteFile");
}

void CuteMockServer::configureSecureRequest(QNetworkRequest *request) const
{
    if (nullptr == request)
        qCritical() << "Cannot configure nullptr";
    else
        m_sslServer->configureRequest(*request);
}

const QUrl &CuteMockServer::getCertificateFile() const
{
    return m_sslServer->getCertificateFile();
}

bool CuteMockServer::listen(const ushort port, const bool secure/* = false*/)
{
    const bool started = secure ? m_sslServer->listen(QHostAddress::Any, port)
                                : m_tcpServer->listen(QHostAddress::Any, port);
    if (!started)
        qCritical() << "Could not start" << (secure ? "Secure" : "")
                    << "Cute Mock HTTP server on port" << port << m_tcpServer->errorString();
    return started;
}

void CuteMockServer::setHttpRoute(const QString &method, const QUrl &uri, const int statusCode, const QString &contentType,
                                  const QByteArray &content, const bool ignoreUriQuery/* = false*/)
{
    m_router->set(method, uri, statusCode, contentType, content, ignoreUriQuery);
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
