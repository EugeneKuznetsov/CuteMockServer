#pragma once

#include <QNetworkRequest>
#include <QObject>
#include <QUrl>

class QNetworkAccessManager;

class HttpClient : public QObject
{
    Q_OBJECT

public:
    explicit HttpClient(QObject *parent = nullptr);

public slots:
    void get(const QUrl &uri, const QString &contentType = "text/html");
    void post(const QUrl &uri, const QByteArray &data = "", const QString &contentType = "text/html");
    void put(const QUrl &uri, const QByteArray &data = "", const QString &contentType = "text/html");
    void deleteResource(const QUrl &uri, const QString &contentType = "text/html");

signals:
    void success(const int statusCode, const QString contentType, const int contentLength, const QByteArray data);
    void failure();

private:
    void installCertificate(QNetworkRequest &request);

private:
    QNetworkAccessManager *m_network;
};
