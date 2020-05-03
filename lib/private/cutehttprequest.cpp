#include <QTcpSocket>
#include "cutehttprequest.h"

CuteHttpRequest::CuteHttpRequest(QTcpSocket *parent)
    : QObject(parent)
    , m_method(CuteMockData::UNDEFINED)
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
            if (requestLine[0] == "GET")
                m_method = CuteMockData::GET;
            else if (requestLine[0] == "POST")
                m_method = CuteMockData::POST;
            else if (requestLine[0] == "PUT")
                m_method = CuteMockData::PUT;
            else if (requestLine[0] == "DELETE")
                m_method = CuteMockData::DELETE;
            else
                qWarning() << "Request Method is not one of [GET, POST, PUT, DELETE]."
                           << requestLine[0];
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
