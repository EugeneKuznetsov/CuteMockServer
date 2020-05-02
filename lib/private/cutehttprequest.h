#pragma once

#include <QObject>

class QTcpSocket;

class CuteHttpRequest : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(CuteHttpRequest)

public:
    explicit CuteHttpRequest(QTcpSocket *parent);

signals:
    void parsed(QTcpSocket *client, const CuteHttpRequest &request);

private slots:
    void parse();
};
