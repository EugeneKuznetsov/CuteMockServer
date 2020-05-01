#pragma once

#include <QObject>

class CuteMockServer : public QObject
{
    Q_OBJECT

public:
    explicit CuteMockServer(QObject *parent = nullptr);

    void listen_http(const ushort port);
    void listen_https(const ushort port);
};
