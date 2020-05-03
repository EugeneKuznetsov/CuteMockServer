#pragma once

#include <QByteArray>
#include <QFile>

class CuteMockData
{
public:
    enum Method {
        UNDEFINED,
        GET,
        POST,
        PUT,
        DELETE
    };

    enum ContentType {
        TextHtml,
        ImagePng,
        ApplicationJson
    };

    CuteMockData(int statusCode, ContentType type, const QByteArray &body);
    CuteMockData(int statusCode, ContentType type, QFile &body);
    CuteMockData(const CuteMockData &rightValue);
    CuteMockData &operator=(const CuteMockData &rightValue);

    const int &getStatusCode() const {
        return m_statusCode;
    }
    const ContentType &getContentType() const {
        return m_type;
    }
    const QByteArray &getBody() const {
        return m_data;
    }

private:
    int         m_statusCode;
    ContentType m_type;
    QByteArray  m_data;
};
