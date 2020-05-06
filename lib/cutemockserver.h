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

    Q_PROPERTY(QUrl certificateFile READ getCertificateFile CONSTANT)

public:
    explicit CuteMockServer(QObject *parent = nullptr);
    virtual ~CuteMockServer();

    static void registerTypes();

    void configureSecureRequest(QNetworkRequest *request) const;
    const QUrl &getCertificateFile() const;

public slots:
    bool listen(const ushort port, const bool secure = false);

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
