#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFile>
#include <QSslCertificate>
#include <QDebug>
#include "httpclient.h"

HttpClient::HttpClient(QObject *parent)
    : QObject(parent)
    , m_network(new QNetworkAccessManager(this))
{
}

void HttpClient::get(const QUrl &uri, const QString &contentType/* = "text/html"*/)
{
    qDebug() << "GET" << uri;
    QNetworkRequest request(uri);
    if (uri.scheme() == "https")
        installCertificate(request);
    request.setHeader(QNetworkRequest::ContentTypeHeader, contentType);
    QNetworkReply *reply = m_network->get(request);
    reply->setParent(this);
    connect(reply, &QNetworkReply::finished, this, [this, uri, reply]() {
        const int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        const QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();
        const int contentLength = reply->header(QNetworkRequest::ContentLengthHeader).toInt();
        const QByteArray content = reply->readAll();
        qDebug() << "GET" << uri << "reply:" << statusCode << contentType << contentLength;
        if (0 != statusCode)
            emit success(statusCode, contentType, contentLength, content);
    });
    // https://wiki.qt.io/New_Signal_Slot_Syntax#Overload
    void (QNetworkReply:: *errorSignal)(QNetworkReply::NetworkError) = &QNetworkReply::error;
    connect(reply, errorSignal, this, [this, reply](QNetworkReply::NetworkError) {
        // we are interested only in network layer and proxy errors
        // as of Qt 5.14, they are defined before ContentAccessDenied literal
        if (reply->error() < QNetworkReply::ContentAccessDenied) {
            qDebug() << "GET error:" << reply->error() << reply->errorString();
            emit failure();
        }
    });
}

void HttpClient::post(const QUrl &uri, const QByteArray &data/* = ""*/, const QString &contentType/* = "text/html"*/)
{
    qDebug() << "POST" << uri;
    QNetworkRequest request(uri);
    if (uri.scheme() == "https")
        installCertificate(request);
    request.setHeader(QNetworkRequest::ContentTypeHeader, contentType);
    QNetworkReply *reply = m_network->post(request, data);
    reply->setParent(this);
    connect(reply, &QNetworkReply::finished, this, [this, uri, reply]() {
        const int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        const QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();
        const int contentLength = reply->header(QNetworkRequest::ContentLengthHeader).toInt();
        const QByteArray content = reply->readAll();
        qDebug() << "POST" << uri << "reply:" << statusCode << contentType << contentLength;
        if (0 != statusCode)
            emit success(statusCode, contentType, contentLength, content);
    });
    // https://wiki.qt.io/New_Signal_Slot_Syntax#Overload
    void (QNetworkReply:: *errorSignal)(QNetworkReply::NetworkError) = &QNetworkReply::error;
    connect(reply, errorSignal, this, [this, reply](QNetworkReply::NetworkError) {
        if (reply->error() < QNetworkReply::ContentAccessDenied) {
            qDebug() << "POST error:" << reply->error() << reply->errorString();
            emit failure();
        }
    });
}

void HttpClient::put(const QUrl &uri, const QByteArray &data/* = ""*/, const QString &contentType/* = "text/html"*/)
{
    qDebug() << "PUT" << uri;
    QNetworkRequest request(uri);
    if (uri.scheme() == "https")
        installCertificate(request);
    request.setHeader(QNetworkRequest::ContentTypeHeader, contentType);
    QNetworkReply *reply = m_network->put(request, data);
    reply->setParent(this);
    connect(reply, &QNetworkReply::finished, this, [this, uri, reply]() {
        const int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        const QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();
        const int contentLength = reply->header(QNetworkRequest::ContentLengthHeader).toInt();
        const QByteArray content = reply->readAll();
        qDebug() << "PUT" << uri << "reply:" << statusCode << contentType << contentLength;
        if (0 != statusCode)
            emit success(statusCode, contentType, contentLength, content);
    });
    // https://wiki.qt.io/New_Signal_Slot_Syntax#Overload
    void (QNetworkReply:: *errorSignal)(QNetworkReply::NetworkError) = &QNetworkReply::error;
    connect(reply, errorSignal, this, [this, reply](QNetworkReply::NetworkError) {
        if (reply->error() < QNetworkReply::ContentAccessDenied) {
            qDebug() << "PUT error:" << reply->error() << reply->errorString();
            emit failure();
        }
    });
}

void HttpClient::deleteResource(const QUrl &uri, const QString &contentType/* = "text/html"*/)
{
    qDebug() << "DELETE" << uri;
    QNetworkRequest request(uri);
    if (uri.scheme() == "https")
        installCertificate(request);
    request.setHeader(QNetworkRequest::ContentTypeHeader, contentType);
    QNetworkReply *reply = m_network->deleteResource(request);
    reply->setParent(this);
    connect(reply, &QNetworkReply::finished, this, [this, uri, reply]() {
        const int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        const QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();
        const int contentLength = reply->header(QNetworkRequest::ContentLengthHeader).toInt();
        const QByteArray content = reply->readAll();
        qDebug() << "DELETE" << uri << "reply:" << statusCode << contentType << contentLength;
        if (0 != statusCode)
            emit success(statusCode, contentType, contentLength, content);
    });
    // https://wiki.qt.io/New_Signal_Slot_Syntax#Overload
    void (QNetworkReply:: *errorSignal)(QNetworkReply::NetworkError) = &QNetworkReply::error;
    connect(reply, errorSignal, this, [this, reply](QNetworkReply::NetworkError) {
        if (reply->error() < QNetworkReply::ContentAccessDenied) {
            qDebug() << "DELETE error:" << reply->error() << reply->errorString();
            emit failure();
        }
    });
}

void HttpClient::installCertificate(QNetworkRequest &request)
{
    QFile certificateFile(":/cute/mock/ssl/cert/localhost.crt");
    if (!certificateFile.open(QFile::ReadOnly))
        qCritical() << "Could not open certificate" << certificateFile.fileName();
    QSslConfiguration configuration(QSslConfiguration::defaultConfiguration());
    configuration.setProtocol(QSsl::TlsV1_3OrLater);
    configuration.setCaCertificates({QSslCertificate(&certificateFile, QSsl::Pem)});
    request.setSslConfiguration(configuration);
}
