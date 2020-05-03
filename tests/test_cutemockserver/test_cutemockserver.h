#pragma once

#include <QObject>

class CuteMockServerTestCase : public QObject
{
    Q_OBJECT

private slots:
    void test_listen_fail_http();
    void test_listen_fail_https();
    void test_listen_http();
    void test_listen_https();
    void test_listen_http_and_https();

    void test_nonexistent_http_route_via_get();
    void test_nonexistent_http_route_via_post();
    void test_nonexistent_http_route_via_put();
    void test_nonexistent_http_route_via_delete();
    void test_existing_http_route_via_get();
    void test_existing_http_route_via_post();
    void test_existing_http_route_via_put();
    void test_existing_http_route_via_delete();

    void test_nonexistent_https_route_via_get();
    void test_nonexistent_https_route_via_post();
    void test_nonexistent_https_route_via_put();
    void test_nonexistent_https_route_via_delete();
    void test_existing_https_route_via_get();
    void test_existing_https_route_via_post();
    void test_existing_https_route_via_put();
    void test_existing_https_route_via_delete();
};
