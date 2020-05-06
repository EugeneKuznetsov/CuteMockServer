#include <QSslConfiguration>
#include <QSslSocket>
#include <QDebug>
#include <QFile>
#include "cutesslserver.h"

CuteSslServer::CuteSslServer(QObject *parent/* = nullptr*/)
    : QTcpServer(parent)
    , m_certificate()
    , m_privateKey()
    , m_certificateFile(":/cute/mock/ssl/cert/localhost.crt")
{
    QFile certificateFile(":/cute/mock/ssl/cert/localhost.crt");
    if (!certificateFile.open(QFile::ReadOnly))
        qCritical() << "Could not open" << certificateFile.fileName();
    else
        m_certificate = QSslCertificate(&certificateFile, QSsl::Pem);

    QFile privateKeyFile(":/cute/mock/ssl/cert/localhost.key");
    if (!privateKeyFile.open(QFile::ReadOnly))
        qCritical() << "Could not open" << privateKeyFile.fileName();
    else
        m_privateKey = QSslKey(&privateKeyFile, QSsl::Rsa, QSsl::Pem);
}

void CuteSslServer::configureRequest(QNetworkRequest &request) const
{
    QSslConfiguration configuration(QSslConfiguration::defaultConfiguration());
    configuration.setProtocol(QSsl::TlsV1_3OrLater);
    configuration.setCaCertificates({m_certificate});
    request.setSslConfiguration(configuration);
}

void CuteSslServer::incomingConnection(qintptr handle)
{
    QSslSocket *peer = new QSslSocket(this);
    peer->setSocketDescriptor(handle);
    peer->setPeerVerifyMode(QSslSocket::VerifyNone);
    peer->setLocalCertificate(m_certificate);
    peer->setPrivateKey(m_privateKey);
    peer->setProtocol(QSsl::TlsV1_3OrLater);
    peer->startServerEncryption();
    this->addPendingConnection(peer);
}
