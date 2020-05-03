#include <QTest>
#include <QNetworkRequest>
#include "lib/private/cutesslserver.h"
#include "test_cutesslserver.h"

void CuteSslServerTestCase::test_configure_request()
{
    QNetworkRequest request;

    bool hasLocalhostCN = false;
    for (auto c : request.sslConfiguration().caCertificates()) {
        for (auto cn : c.issuerInfo(QSslCertificate::SubjectInfo::CommonName)) {
            if (cn == QString("localhost")
                    && (c.expiryDate().date() == QDateTime::fromString("02-05-2021", "dd-MM-yyyy").date())) {
                hasLocalhostCN = true;
                break;
            }
        }
    }
    QCOMPARE(hasLocalhostCN, false);

    CuteSslServer sslServer;
    sslServer.configureRequest(request);

    for (auto c : request.sslConfiguration().caCertificates()) {
        for (auto cn : c.issuerInfo(QSslCertificate::SubjectInfo::CommonName)) {
            if (cn == QString("localhost")
                    && (c.expiryDate().date() == QDateTime::fromString("02-05-2021", "dd-MM-yyyy").date())) {
                hasLocalhostCN = true;
                break;
            }
        }
    }
    QCOMPARE(hasLocalhostCN, true);
}

QTEST_GUILESS_MAIN(CuteSslServerTestCase)
