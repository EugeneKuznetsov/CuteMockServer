#include <QTest>
#include <QSignalSpy>
#include <QTcpSocket>
#include <QSslSocket>
#include <QHostAddress>
#include "lib/cutemockserver.h"
#include "test_cutemockserver.h"

void CuteMockServerTestCase::test_listen_fail_http()
{
    CuteMockServer mockServer;
    QCOMPARE(mockServer.listen_https(8080), true);
    QCOMPARE(mockServer.listen_http(8080), false);
}

void CuteMockServerTestCase::test_listen_fail_https()
{
    CuteMockServer mockServer;
    QCOMPARE(mockServer.listen_http(4443), true);
    QCOMPARE(mockServer.listen_https(4443), false);
}

void CuteMockServerTestCase::test_listen_http()
{
    CuteMockServer mockServer;
    QCOMPARE(mockServer.listen_http(8080), true);

    QTcpSocket tcpClient;
    tcpClient.connectToHost("localhost", 8080);

    QSignalSpy clientConnected(&tcpClient, &QTcpSocket::connected);
    QVERIFY(clientConnected.wait(50));
}

void CuteMockServerTestCase::test_listen_https()
{
    CuteMockServer mockServer;
    QCOMPARE(mockServer.listen_https(4443), true);

    QSslSocket sslClient;
    sslClient.connectToHost("localhost", 4443);

    QSignalSpy clientConnected(&sslClient, &QSslSocket::connected);
    QVERIFY(clientConnected.wait(50));
}

void CuteMockServerTestCase::test_listen_http_and_https()
{
    CuteMockServer mockServer;
    QCOMPARE(mockServer.listen_http(8080), true);
    QCOMPARE(mockServer.listen_https(4443), true);

    QTcpSocket tcpClient;
    tcpClient.connectToHost("localhost", 8080);
    QSignalSpy tcpClientConnected(&tcpClient, &QTcpSocket::connected);
    QVERIFY(tcpClientConnected.wait(50));

    QSslSocket sslClient;
    sslClient.connectToHost("localhost", 4443);
    QSignalSpy sslClientConnected(&sslClient, &QSslSocket::connected);
    QVERIFY(sslClientConnected.wait(50));
}

QTEST_GUILESS_MAIN(CuteMockServerTestCase)
