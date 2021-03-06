#pragma once

#include <QTcpServer>
#include <QNetworkRequest>
#include <QSslCertificate>
#include <QSslKey>

class CuteSslServer : public QTcpServer
{
    Q_OBJECT

public:
    explicit CuteSslServer(QObject *parent = nullptr);

    void configureRequest(QNetworkRequest &request) const;

    const QUrl &getCertificateFile() const {
        return m_certificateFile;
    }

protected:
    virtual void incomingConnection(qintptr handle) override;

private:
    QSslCertificate m_certificate;
    QSslKey         m_privateKey;
    QUrl            m_certificateFile;
};
