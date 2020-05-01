#include <QTcpServer>
#include <QHostAddress>
#include <QDebug>
#include "private/cutesslserver.h"
#include "cutemockserver.h"

CuteMockServer::CuteMockServer(QObject *parent/*= nullptr*/)
    : QObject(parent)
    , m_tcpServer(new QTcpServer(this))
    , m_sslServer(new CuteSslServer(this))
{
}

bool CuteMockServer::listen_http(const ushort port)
{
    const bool started = m_tcpServer->listen(QHostAddress::Any, port);
    if (!started)
        qCritical() << "Could not start Cute Mock HTTP server on port" << port << m_tcpServer->errorString();
    return started;
}

bool CuteMockServer::listen_https(const ushort port)
{
    const bool started = m_sslServer->listen(QHostAddress::Any, port);
    if (!started)
        qCritical() << "Could not start Cute Mock HTTPS server on port" << port << m_sslServer->errorString();
    return started;
}
