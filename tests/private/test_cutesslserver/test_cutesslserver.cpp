#include <QTest>
#include <QNetworkRequest>
#include "lib/private/cutesslserver.h"
#include "test_cutesslserver.h"

void CuteSslServerTestCase::test_configure_request()
{
    QNetworkRequest request;
    QCOMPARE(request.sslConfiguration().caCertificates().size(), 0);
    CuteSslServer sslServer;
    sslServer.configureRequest(request);
    QCOMPARE(request.sslConfiguration().caCertificates().size(), 1);
}

QTEST_GUILESS_MAIN(CuteSslServerTestCase)
