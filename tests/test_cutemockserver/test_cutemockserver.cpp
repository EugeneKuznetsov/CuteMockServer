#include <QTest>
#include <QSignalSpy>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QSslConfiguration>
#include "lib/cutemockserver.h"
#include "test_cutemockserver.h"

void CuteMockServerTestCase::listenHttpFailure()
{
    CuteMockServer mockServer;
    QCOMPARE(mockServer.listenHttps(8080), true);
    QCOMPARE(mockServer.listenHttp(8080), false);
}

void CuteMockServerTestCase::listenSecureHttpFailure()
{
    CuteMockServer mockServer;
    QCOMPARE(mockServer.listenHttp(4443), true);
    QCOMPARE(mockServer.listenHttps(4443), false);
}

void CuteMockServerTestCase::listenHttp()
{
    CuteMockServer mockServer;
    QCOMPARE(mockServer.listenHttp(8080), true);

    QTcpSocket tcpClient;
    tcpClient.connectToHost("localhost", 8080);

    QSignalSpy clientConnected(&tcpClient, &QTcpSocket::connected);
    QVERIFY(clientConnected.wait(50));
}

void CuteMockServerTestCase::listenSecureHttp()
{
    CuteMockServer mockServer;
    QCOMPARE(mockServer.listenHttps(4443), true);

    QSslSocket sslClient;
    sslClient.connectToHost("localhost", 4443);

    QSignalSpy clientConnected(&sslClient, &QSslSocket::connected);
    QVERIFY(clientConnected.wait(50));
}

void CuteMockServerTestCase::listenHttpAndSecureHttp()
{
    CuteMockServer mockServer;
    QCOMPARE(mockServer.listenHttp(8080), true);
    QCOMPARE(mockServer.listenHttps(4443), true);

    QTcpSocket tcpClient;
    tcpClient.connectToHost("localhost", 8080);
    QSignalSpy tcpClientConnected(&tcpClient, &QTcpSocket::connected);
    QVERIFY(tcpClientConnected.wait(50));

    QSslSocket sslClient;
    sslClient.connectToHost("localhost", 4443);
    QSignalSpy sslClientConnected(&sslClient, &QSslSocket::connected);
    QVERIFY(sslClientConnected.wait(50));
}

void CuteMockServerTestCase::nonexistentHttpRoute()
{
    CuteMockServer mockServer;
    QCOMPARE(mockServer.listenHttp(8080), true);

    mockServer.setHttpRoute("GET", QUrl("/"), 200, "", "");

    QNetworkAccessManager network;
    QNetworkRequest request(QUrl("http://localhost:8080/wrong/route"));
    QNetworkReply *reply = network.get(request);
    reply->setParent(&network);

    QSignalSpy replyIsReady(reply, &QNetworkReply::finished);
    QVERIFY(replyIsReady.wait(50));
    QCOMPARE(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(), 404);
    QCOMPARE(reply->header(QNetworkRequest::ContentTypeHeader).toString().isEmpty(), true);
    QCOMPARE(reply->header(QNetworkRequest::ContentLengthHeader).toInt(), 0);
    QCOMPARE(reply->readAll().isEmpty(), true);
}

void CuteMockServerTestCase::existingHttpRoute()
{
    CuteMockServer mockServer;
    QCOMPARE(mockServer.listenHttp(8080), true);

    QFile testImage(":/test_image.png");
    mockServer.setHttpRoute("POST", QUrl("/images"), 201, "image/png", testImage);

    QNetworkAccessManager network;
    QNetworkRequest request(QUrl("http://localhost:8080/images"));
    QNetworkReply *reply = network.post(request, QByteArray());
    reply->setParent(&network);

    QSignalSpy replyIsReady(reply, &QNetworkReply::finished);
    QVERIFY(replyIsReady.wait(50));
    QCOMPARE(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(), 201);
    QCOMPARE(reply->header(QNetworkRequest::ContentTypeHeader).toString(), "image/png");
    QCOMPARE(reply->header(QNetworkRequest::ContentLengthHeader).toInt(), QFileInfo(":/test_image.png").size());
    QCOMPARE(reply->readAll().size(), QFileInfo(":/test_image.png").size());
}

void CuteMockServerTestCase::nonexistentSecureHttpRoute()
{
    CuteMockServer mockServer;
    QCOMPARE(mockServer.listenHttps(4443), true);

    mockServer.setHttpRoute("PUT", QUrl("/"), 202, "", "");

    QNetworkRequest request(QUrl("https://localhost:4443/wrong/route"));
    mockServer.configureSecureRequest(&request);
    QNetworkAccessManager network;
    QNetworkReply *reply = network.put(request, "");
    reply->setParent(&network);

    QSignalSpy replyIsReady(reply, &QNetworkReply::finished);
    QVERIFY(replyIsReady.wait(50));

    QCOMPARE(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(), 404);
    QCOMPARE(reply->header(QNetworkRequest::ContentTypeHeader).toString().isEmpty(), true);
    QCOMPARE(reply->header(QNetworkRequest::ContentLengthHeader).toInt(), 0);
    QCOMPARE(reply->readAll().isEmpty(), true);
}

void CuteMockServerTestCase::existingSecureHttpRoute()
{
    CuteMockServer mockServer;
    QCOMPARE(mockServer.listenHttps(4443), true);

    mockServer.setHttpRoute("GET", QUrl("/?query1=value&query2="), 200, "text/html", "Hello Cute Client");

    QNetworkAccessManager network;
    QNetworkRequest request(QUrl("https://localhost:4443/?query1=value&query2="));
    mockServer.configureSecureRequest(&request);
    QNetworkReply *reply = network.get(request);
    reply->setParent(&network);

    QSignalSpy replyIsReady(reply, &QNetworkReply::finished);
    QVERIFY(replyIsReady.wait(50));
    QCOMPARE(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(), 200);
    QCOMPARE(reply->header(QNetworkRequest::ContentTypeHeader).toString(), "text/html");
    QCOMPARE(reply->header(QNetworkRequest::ContentLengthHeader).toInt(), QByteArray("Hello Cute Client").length());
    QCOMPARE(reply->readAll(), QByteArray("Hello Cute Client"));
}

QTEST_GUILESS_MAIN(CuteMockServerTestCase)
