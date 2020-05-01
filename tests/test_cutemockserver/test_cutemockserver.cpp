#include <QTest>
#include <QSignalSpy>
#include <QTcpSocket>
#include <QSslSocket>
#include <QHostAddress>
#include "lib/cutemockserver.h"
#include "test_cutemockserver.h"

void CuteMockServerTestCase::test_listen_http()
{
    CuteMockServer mockServer;
    mockServer.listen_http(8080);

    QTcpSocket tcpClient;
    tcpClient.connectToHost("localhost", 8080);

    QSignalSpy clientConnected(&tcpClient, &QTcpSocket::connected);
    QVERIFY(clientConnected.wait(1));
}

void CuteMockServerTestCase::test_listen_https()
{
    CuteMockServer mockServer;
    mockServer.listen_https(4443);

    QSslSocket sslClient;
    sslClient.connectToHost("localhost", 4443);

    QSignalSpy clientConnected(&sslClient, &QSslSocket::connected);
    QVERIFY(clientConnected.wait(5));
}

void CuteMockServerTestCase::test_listen_http_and_https()
{
    CuteMockServer mockServer;
    mockServer.listen_http(8080);
    mockServer.listen_https(4443);

    QTcpSocket tcpClient;
    tcpClient.connectToHost("localhost", 8080);
    QSslSocket sslClient;
    sslClient.connectToHost("localhost", 4443);

    QSignalSpy tcpClientConnected(&tcpClient, &QTcpSocket::connected);
    QVERIFY(tcpClientConnected.wait(1));
    QSignalSpy sslClientConnected(&sslClient, &QSslSocket::connected);
    QVERIFY(sslClientConnected.wait(5));
}

QTEST_GUILESS_MAIN(CuteMockServerTestCase)
