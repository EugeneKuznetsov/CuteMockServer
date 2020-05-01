#pragma once

#include <QObject>

class CuteMockServerTestCase : public QObject
{
    Q_OBJECT

private slots:
    void test_listen_http();
    void test_listen_https();
    void test_listen_http_and_https();
};
