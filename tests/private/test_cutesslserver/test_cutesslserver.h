#pragma once

#include <QObject>

class CuteSslServerTestCase : public QObject
{
    Q_OBJECT

private slots:
    void test_listen();

};
