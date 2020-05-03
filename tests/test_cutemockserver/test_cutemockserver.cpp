#include <QTest>
#include <QSignalSpy>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QSslConfiguration>
#include "lib/cutemockserver.h"
#include "test_cutemockserver.h"

void CuteMockServerTestCase::test_listen_fail_http()
{
    CuteMockServer mockServer;
    QCOMPARE(mockServer.listenHttps(8080), true);
    QCOMPARE(mockServer.listenHttp(8080), false);
}

void CuteMockServerTestCase::test_listen_fail_https()
{
    CuteMockServer mockServer;
    QCOMPARE(mockServer.listenHttp(4443), true);
    QCOMPARE(mockServer.listenHttps(4443), false);
}

void CuteMockServerTestCase::test_listen_http()
{
    CuteMockServer mockServer;
    QCOMPARE(mockServer.listenHttp(8080), true);

    QTcpSocket tcpClient;
    tcpClient.connectToHost("localhost", 8080);

    QSignalSpy clientConnected(&tcpClient, &QTcpSocket::connected);
    QVERIFY(clientConnected.wait(50));
}

void CuteMockServerTestCase::test_listen_https()
{
    CuteMockServer mockServer;
    QCOMPARE(mockServer.listenHttps(4443), true);

    QSslSocket sslClient;
    sslClient.connectToHost("localhost", 4443);

    QSignalSpy clientConnected(&sslClient, &QSslSocket::connected);
    QVERIFY(clientConnected.wait(50));
}

void CuteMockServerTestCase::test_listen_http_and_https()
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

void CuteMockServerTestCase::test_nonexistent_http_route_via_get()
{
    CuteMockServer mockServer;
    QCOMPARE(mockServer.listenHttp(8080), true);

    CuteMockData cmd(200, CuteMockData::TextHtml, "Hello Cute Client");
    mockServer.setHttpRoute(CuteMockData::GET, QUrl("/"), cmd);

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

void CuteMockServerTestCase::test_nonexistent_http_route_via_post()
{
    CuteMockServer mockServer;
    QCOMPARE(mockServer.listenHttp(8080), true);

    CuteMockData cmd(200, CuteMockData::TextHtml, "Hello Cute Client");
    mockServer.setHttpRoute(CuteMockData::POST, QUrl("/"), cmd);

    QNetworkAccessManager network;
    QNetworkRequest request(QUrl("http://localhost:8080/wrong/route"));
    QNetworkReply *reply = network.post(request, QByteArray());
    reply->setParent(&network);

    QSignalSpy replyIsReady(reply, &QNetworkReply::finished);
    QVERIFY(replyIsReady.wait(50));
    QCOMPARE(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(), 404);
    QCOMPARE(reply->header(QNetworkRequest::ContentTypeHeader).toString().isEmpty(), true);
    QCOMPARE(reply->header(QNetworkRequest::ContentLengthHeader).toInt(), 0);
    QCOMPARE(reply->readAll().isEmpty(), true);
}

void CuteMockServerTestCase::test_nonexistent_http_route_via_put()
{
    CuteMockServer mockServer;
    QCOMPARE(mockServer.listenHttp(8080), true);

    CuteMockData cmd(200, CuteMockData::TextHtml, "Hello Cute Client");
    mockServer.setHttpRoute(CuteMockData::PUT, QUrl("/"), cmd);

    QNetworkAccessManager network;
    QNetworkRequest request(QUrl("http://localhost:8080/wrong/route"));
    QNetworkReply *reply = network.put(request, QByteArray());
    reply->setParent(&network);

    QSignalSpy replyIsReady(reply, &QNetworkReply::finished);
    QVERIFY(replyIsReady.wait(50));
    QCOMPARE(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(), 404);
    QCOMPARE(reply->header(QNetworkRequest::ContentTypeHeader).toString().isEmpty(), true);
    QCOMPARE(reply->header(QNetworkRequest::ContentLengthHeader).toInt(), 0);
    QCOMPARE(reply->readAll().isEmpty(), true);
}

void CuteMockServerTestCase::test_nonexistent_http_route_via_delete()
{
    CuteMockServer mockServer;
    QCOMPARE(mockServer.listenHttp(8080), true);

    CuteMockData cmd(200, CuteMockData::TextHtml, "Hello Cute Client");
    mockServer.setHttpRoute(CuteMockData::DELETE, QUrl("/"), cmd);

    QNetworkAccessManager network;
    QNetworkRequest request(QUrl("http://localhost:8080/wrong/route"));
    QNetworkReply *reply = network.deleteResource(request);
    reply->setParent(&network);

    QSignalSpy replyIsReady(reply, &QNetworkReply::finished);
    QVERIFY(replyIsReady.wait(50));
    QCOMPARE(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(), 404);
    QCOMPARE(reply->header(QNetworkRequest::ContentTypeHeader).toString().isEmpty(), true);
    QCOMPARE(reply->header(QNetworkRequest::ContentLengthHeader).toInt(), 0);
    QCOMPARE(reply->readAll().isEmpty(), true);
}

void CuteMockServerTestCase::test_existing_http_route_via_get()
{
    CuteMockServer mockServer;
    QCOMPARE(mockServer.listenHttp(8080), true);

    CuteMockData cmd(200, CuteMockData::TextHtml, "Hello Cute Client");
    mockServer.setHttpRoute(CuteMockData::GET, QUrl("/?query1=value&query2="), cmd);

    QNetworkAccessManager network;
    QNetworkRequest request(QUrl("http://localhost:8080/?query1=value&query2="));
    QNetworkReply *reply = network.get(request);
    reply->setParent(&network);

    QSignalSpy replyIsReady(reply, &QNetworkReply::finished);
    QVERIFY(replyIsReady.wait(50));
    QCOMPARE(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(), 200);
    QCOMPARE(reply->header(QNetworkRequest::ContentTypeHeader).toString(), "text/html");
    QCOMPARE(reply->header(QNetworkRequest::ContentLengthHeader).toInt(), QByteArray("Hello Cute Client").length());
    QCOMPARE(reply->readAll(), QByteArray("Hello Cute Client"));
}

void CuteMockServerTestCase::test_existing_http_route_via_post()
{
    CuteMockServer mockServer;
    QCOMPARE(mockServer.listenHttp(8080), true);

    CuteMockData cmd(202, CuteMockData::ApplicationJson, "{\"text\": \"Hello Cute Client\"}");
    mockServer.setHttpRoute(CuteMockData::POST, QUrl("/resource/1"), cmd);

    QNetworkAccessManager network;
    QNetworkRequest request(QUrl("http://localhost:8080/resource/1"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QNetworkReply *reply = network.post(request, QByteArray("{\"root\": \"object\"}"));
    reply->setParent(&network);

    QSignalSpy replyIsReady(reply, &QNetworkReply::finished);
    QVERIFY(replyIsReady.wait(50));
    QCOMPARE(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(), 202);
    QCOMPARE(reply->header(QNetworkRequest::ContentTypeHeader).toString(), "application/json");
    QCOMPARE(reply->header(QNetworkRequest::ContentLengthHeader).toInt(), QByteArray("{\"text\": \"Hello Cute Client\"}").length());
    QCOMPARE(reply->readAll(), QByteArray("{\"text\": \"Hello Cute Client\"}"));
}

void CuteMockServerTestCase::test_existing_http_route_via_put()
{
    CuteMockServer mockServer;
    QCOMPARE(mockServer.listenHttp(8080), true);

    QFile testImage(":/test_image.png");
    CuteMockData cmd(201, CuteMockData::ImagePng, testImage);
    mockServer.setHttpRoute(CuteMockData::PUT, QUrl("/images"), cmd);

    QNetworkAccessManager network;
    QNetworkRequest request(QUrl("http://localhost:8080/images"));
    QNetworkReply *reply = network.put(request, QByteArray());
    reply->setParent(&network);

    QSignalSpy replyIsReady(reply, &QNetworkReply::finished);
    QVERIFY(replyIsReady.wait(50));
    QCOMPARE(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(), 201);
    QCOMPARE(reply->header(QNetworkRequest::ContentTypeHeader).toString(), "image/png");
    QCOMPARE(reply->header(QNetworkRequest::ContentLengthHeader).toInt(), QFileInfo(":/test_image.png").size());
    QCOMPARE(reply->readAll().size(), QFileInfo(":/test_image.png").size());
}

void CuteMockServerTestCase::test_existing_http_route_via_delete()
{
    CuteMockServer mockServer;
    QCOMPARE(mockServer.listenHttp(8080), true);

    CuteMockData cmd(201, CuteMockData::TextHtml, "Hello Cute Client");
    mockServer.setHttpRoute(CuteMockData::DELETE, QUrl("/rest/resource/3"), cmd);

    QNetworkAccessManager network;
    QNetworkRequest request(QUrl("http://localhost:8080/rest/resource/3"));
    QNetworkReply *reply = network.deleteResource(request);
    reply->setParent(&network);

    QSignalSpy replyIsReady(reply, &QNetworkReply::finished);
    QVERIFY(replyIsReady.wait(50));
    QCOMPARE(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(), 201);
    QCOMPARE(reply->header(QNetworkRequest::ContentTypeHeader).toString(), "text/html");
    QCOMPARE(reply->header(QNetworkRequest::ContentLengthHeader).toInt(), QByteArray("Hello Cute Client").length());
    QCOMPARE(reply->readAll(), QByteArray("Hello Cute Client"));
}

void CuteMockServerTestCase::test_nonexistent_https_route_via_get()
{
    CuteMockServer mockServer;
    QCOMPARE(mockServer.listenHttps(4443), true);

    CuteMockData cmd(200, CuteMockData::TextHtml, "Hello Cute Client");
    mockServer.setHttpRoute(CuteMockData::GET, QUrl("/"), cmd);

    QNetworkRequest request(QUrl("https://localhost:4443/wrong/route"));
    mockServer.configureSecureRequest(&request);
    QNetworkAccessManager network;
    QNetworkReply *reply = network.get(request);
    reply->setParent(&network);

    QSignalSpy replyIsReady(reply, &QNetworkReply::finished);
    QVERIFY(replyIsReady.wait(50));

    QCOMPARE(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(), 404);
    QCOMPARE(reply->header(QNetworkRequest::ContentTypeHeader).toString().isEmpty(), true);
    QCOMPARE(reply->header(QNetworkRequest::ContentLengthHeader).toInt(), 0);
    QCOMPARE(reply->readAll().isEmpty(), true);
}

void CuteMockServerTestCase::test_nonexistent_https_route_via_post()
{
    CuteMockServer mockServer;
    QCOMPARE(mockServer.listenHttps(4443), true);

    CuteMockData cmd(200, CuteMockData::TextHtml, "Hello Cute Client");
    mockServer.setHttpRoute(CuteMockData::POST, QUrl("/"), cmd);

    QNetworkAccessManager network;
    QNetworkRequest request(QUrl("https://localhost:4443/wrong/route"));
    mockServer.configureSecureRequest(&request);
    QNetworkReply *reply = network.post(request, QByteArray());
    reply->setParent(&network);

    QSignalSpy replyIsReady(reply, &QNetworkReply::finished);
    QVERIFY(replyIsReady.wait(50));
    QCOMPARE(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(), 404);
    QCOMPARE(reply->header(QNetworkRequest::ContentTypeHeader).toString().isEmpty(), true);
    QCOMPARE(reply->header(QNetworkRequest::ContentLengthHeader).toInt(), 0);
    QCOMPARE(reply->readAll().isEmpty(), true);
}

void CuteMockServerTestCase::test_nonexistent_https_route_via_put()
{
    CuteMockServer mockServer;
    QCOMPARE(mockServer.listenHttps(4443), true);

    CuteMockData cmd(200, CuteMockData::TextHtml, "Hello Cute Client");
    mockServer.setHttpRoute(CuteMockData::PUT, QUrl("/"), cmd);

    QNetworkAccessManager network;
    QNetworkRequest request(QUrl("https://localhost:4443/wrong/route"));
    mockServer.configureSecureRequest(&request);
    QNetworkReply *reply = network.put(request, QByteArray());
    reply->setParent(&network);

    QSignalSpy replyIsReady(reply, &QNetworkReply::finished);
    QVERIFY(replyIsReady.wait(50));
    QCOMPARE(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(), 404);
    QCOMPARE(reply->header(QNetworkRequest::ContentTypeHeader).toString().isEmpty(), true);
    QCOMPARE(reply->header(QNetworkRequest::ContentLengthHeader).toInt(), 0);
    QCOMPARE(reply->readAll().isEmpty(), true);
}

void CuteMockServerTestCase::test_nonexistent_https_route_via_delete()
{
    CuteMockServer mockServer;
    QCOMPARE(mockServer.listenHttps(4443), true);

    CuteMockData cmd(200, CuteMockData::TextHtml, "Hello Cute Client");
    mockServer.setHttpRoute(CuteMockData::DELETE, QUrl("/"), cmd);

    QNetworkAccessManager network;
    QNetworkRequest request(QUrl("https://localhost:4443/wrong/route"));
    mockServer.configureSecureRequest(&request);
    QNetworkReply *reply = network.deleteResource(request);
    reply->setParent(&network);

    QSignalSpy replyIsReady(reply, &QNetworkReply::finished);
    QVERIFY(replyIsReady.wait(50));
    QCOMPARE(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(), 404);
    QCOMPARE(reply->header(QNetworkRequest::ContentTypeHeader).toString().isEmpty(), true);
    QCOMPARE(reply->header(QNetworkRequest::ContentLengthHeader).toInt(), 0);
    QCOMPARE(reply->readAll().isEmpty(), true);
}

void CuteMockServerTestCase::test_existing_https_route_via_get()
{
    CuteMockServer mockServer;
    QCOMPARE(mockServer.listenHttps(4443), true);

    CuteMockData cmd(200, CuteMockData::TextHtml, "Hello Cute Client");
    mockServer.setHttpRoute(CuteMockData::GET, QUrl("/?query1=value&query2="), cmd);

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

void CuteMockServerTestCase::test_existing_https_route_via_post()
{
    CuteMockServer mockServer;
    QCOMPARE(mockServer.listenHttps(4443), true);

    CuteMockData cmd(202, CuteMockData::ApplicationJson, "{\"text\": \"Hello Cute Client\"}");
    mockServer.setHttpRoute(CuteMockData::POST, QUrl("/resource/1"), cmd);

    QNetworkAccessManager network;
    QNetworkRequest request(QUrl("https://localhost:4443/resource/1"));
    mockServer.configureSecureRequest(&request);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QNetworkReply *reply = network.post(request, QByteArray("{\"root\": \"object\"}"));
    reply->setParent(&network);

    QSignalSpy replyIsReady(reply, &QNetworkReply::finished);
    QVERIFY(replyIsReady.wait(50));
    QCOMPARE(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(), 202);
    QCOMPARE(reply->header(QNetworkRequest::ContentTypeHeader).toString(), "application/json");
    QCOMPARE(reply->header(QNetworkRequest::ContentLengthHeader).toInt(), QByteArray("{\"text\": \"Hello Cute Client\"}").length());
    QCOMPARE(reply->readAll(), QByteArray("{\"text\": \"Hello Cute Client\"}"));
}

void CuteMockServerTestCase::test_existing_https_route_via_put()
{
    CuteMockServer mockServer;
    QCOMPARE(mockServer.listenHttps(4443), true);

    QFile testImage(":/test_image.png");
    CuteMockData cmd(201, CuteMockData::ImagePng, testImage);
    mockServer.setHttpRoute(CuteMockData::PUT, QUrl("/images"), cmd);

    QNetworkAccessManager network;
    QNetworkRequest request(QUrl("https://localhost:4443/images"));
    mockServer.configureSecureRequest(&request);
    QNetworkReply *reply = network.put(request, QByteArray());
    reply->setParent(&network);

    QSignalSpy replyIsReady(reply, &QNetworkReply::finished);
    QVERIFY(replyIsReady.wait(50));
    QCOMPARE(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(), 201);
    QCOMPARE(reply->header(QNetworkRequest::ContentTypeHeader).toString(), "image/png");
    QCOMPARE(reply->header(QNetworkRequest::ContentLengthHeader).toInt(), QFileInfo(":/test_image.png").size());
    QCOMPARE(reply->readAll().size(), QFileInfo(":/test_image.png").size());
}

void CuteMockServerTestCase::test_existing_https_route_via_delete()
{
    CuteMockServer mockServer;
    QCOMPARE(mockServer.listenHttps(4443), true);

    CuteMockData cmd(201, CuteMockData::TextHtml, "Hello Cute Client");
    mockServer.setHttpRoute(CuteMockData::DELETE, QUrl("/rest/resource/3"), cmd);

    QNetworkAccessManager network;
    QNetworkRequest request(QUrl("https://localhost:4443/rest/resource/3"));
    mockServer.configureSecureRequest(&request);
    QNetworkReply *reply = network.deleteResource(request);
    reply->setParent(&network);

    QSignalSpy replyIsReady(reply, &QNetworkReply::finished);
    QVERIFY(replyIsReady.wait(50));
    QCOMPARE(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(), 201);
    QCOMPARE(reply->header(QNetworkRequest::ContentTypeHeader).toString(), "text/html");
    QCOMPARE(reply->header(QNetworkRequest::ContentLengthHeader).toInt(), QByteArray("Hello Cute Client").length());
    QCOMPARE(reply->readAll(), QByteArray("Hello Cute Client"));
}


QTEST_GUILESS_MAIN(CuteMockServerTestCase)
