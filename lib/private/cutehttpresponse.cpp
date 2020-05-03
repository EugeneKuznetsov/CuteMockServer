#include "cutehttpresponse.h"

CuteHttpResponse::CuteHttpResponse()
{
    m_data = "HTTP/1.1 404 Not Found\r\n" \
            "Content-Length: 0\r\n" \
             "\r\n";
}

CuteHttpResponse::CuteHttpResponse(const CuteMockData &replyData)
{
    // Status line
    m_data = "HTTP/1.1 ";
    m_data += QString::number(replyData.getStatusCode());
    m_data += " Mock Data\r\n";
    // Headers
    m_data += "Content-Type: ";
    switch (replyData.getContentType()) {
    case CuteMockData::ApplicationJson:
        m_data += "application/json\r\n";
        break;
    case CuteMockData::ImagePng:
        m_data += "image/png\r\n";
        break;
    default:
        m_data += "text/html\r\n";
        break;
    }
    m_data += "Content-Length: ";
    m_data += QString::number(replyData.getBody().length());
    m_data += "\r\n\r\n";
    // Body
    m_data += replyData.getBody();
}

CuteHttpResponse &CuteHttpResponse::operator=(const CuteHttpResponse &rightValue)
{
    this->m_data = rightValue.data();
    return *this;
}
