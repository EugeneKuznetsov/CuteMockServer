#pragma once

#include <QTcpServer>
#include <QObject>

class CuteSslServer : public QTcpServer
{
    Q_OBJECT

public:
    explicit CuteSslServer(QObject *parent = nullptr);

};
