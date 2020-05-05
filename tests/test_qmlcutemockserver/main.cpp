#include <QtQuickTest>
#include <QQmlEngine>
#include "lib/cutemockserver.h"
#include "httpclient.h"
#include "main.h"

void SetupTestSuit::qmlEngineAvailable(QQmlEngine *)
{
    CuteMockServer::registerTypes();
    qmlRegisterType<HttpClient>("HttpClient", 1, 0, "HttpClient");
}

QUICK_TEST_MAIN_WITH_SETUP(QML Cute Mock Server, SetupTestSuit)
