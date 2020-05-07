#pragma once

#include <QObject>

class CuteMockServerTestCase : public QObject
{
    Q_OBJECT

private slots:
    void listenHttpFailure();
    void listenSecureHttpFailure();
    void listenHttp();
    void listenSecureHttp();
    void listenHttpAndSecureHttp();

    void nonexistentHttpRoute();
    void existingHttpRoute();

    void nonexistentSecureHttpRoute();
    void existingSecureHttpRoute();

    void existingHttpRouteIgnoredQuery();
};
