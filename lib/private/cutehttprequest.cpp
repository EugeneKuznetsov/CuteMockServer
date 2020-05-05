#include <QTcpSocket>
#include "cutehttprequest.h"

CuteHttpRequest::CuteHttpRequest(QTcpSocket *parent)
    : QObject(parent)
    , m_method()
    , m_uri()
    , m_headers()
    , m_body()
    , m_contentLength(-1)
{
    connect(parent, &QTcpSocket::readyRead, this, &CuteHttpRequest::parse);
}

void CuteHttpRequest::parse()
{
    QTcpSocket *client = qobject_cast<QTcpSocket *>(this->parent());
    if (m_uri.isEmpty()) {
        auto requestLine = client->readLine().split(' ');
        if (requestLine.size() != 3) {
            qWarning() << "Request-Line is incorrect";
        } else {
            m_method = requestLine[0];
            m_uri = QUrl(requestLine[1]);
        }
    }
    if (m_headers.isEmpty()) {
        QByteArray line;
        while (line != "\r\n") {
            line = client->readLine();
            m_headers.push_back(line);
            if (line.startsWith("Content-Length: "))
                m_contentLength = line.mid(line.indexOf(' ')).toInt();
        }
        if (m_contentLength == -1)
            m_contentLength = 0;
    }
    while (m_contentLength > 0 && !client->atEnd())
        m_body += client->readLine();

    if (!m_uri.isEmpty() && !m_headers.isEmpty() && m_contentLength == m_body.length()) {
        parent()->disconnect(this);
        emit parsed(client, *this);        
    }
}
