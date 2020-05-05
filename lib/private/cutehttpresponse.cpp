#include <QDebug>
#include "cutehttpresponse.h"

CuteHttpResponse::CuteHttpResponse()
{
    m_data = "HTTP/1.1 404 Not Found\r\n" \
            "Content-Length: 0\r\n" \
             "\r\n";
}

CuteHttpResponse::CuteHttpResponse(const int statusCode, const QString &contentType, const QByteArray &content)
{
    setData(statusCode, contentType, content);
}

void CuteHttpResponse::setData(const int statusCode, const QString &contentType, const QByteArray &content)
{
    // Status line
    m_data = "HTTP/1.1 ";
    m_data += QString::number(statusCode);
    m_data += " Mock Data\r\n";
    // Headers
    m_data += "Content-Type: ";
    m_data += contentType;
    m_data += "\r\n";
    m_data += "Content-Length: ";
    m_data += QString::number(content.length());
    m_data += "\r\n\r\n";
    // Body
    m_data += content;
}
