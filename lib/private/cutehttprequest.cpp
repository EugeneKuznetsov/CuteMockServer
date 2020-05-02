#include <QTcpSocket>
#include "cutehttprequest.h"

CuteHttpRequest::CuteHttpRequest(QTcpSocket *parent)
    : QObject(parent)
{
    connect(parent, &QTcpSocket::readyRead, this, &CuteHttpRequest::parse);
}

void CuteHttpRequest::parse()
{
    QTcpSocket *client = qobject_cast<QTcpSocket*>(this->parent());
    while (!client->atEnd()) {
        const QByteArray line = client->readLine();
    }
    emit parsed(client, *this);
}
