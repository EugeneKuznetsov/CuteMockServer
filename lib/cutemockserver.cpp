#include <QTcpServer>
#include <QTcpSocket>
#include "private/cutesslserver.h"
#include "private/cutehttprequest.h"
#include "private/cutehttpresponse.h"
#include "cutemockserver.h"

CuteMockServer::CuteMockServer(QObject *parent/*= nullptr*/)
    : QObject(parent)
    , m_tcpServer(new QTcpServer(this))
{
    Q_INIT_RESOURCE(certificates);
    m_sslServer = new CuteSslServer(this);
    connect(m_tcpServer, &QTcpServer::newConnection, this, &CuteMockServer::httpRequest);
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
    client->write(response.data());
}
