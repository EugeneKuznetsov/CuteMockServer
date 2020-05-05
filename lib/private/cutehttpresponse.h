#pragma once

#include <QByteArray>
#include <QFile>

class CuteHttpResponse
{
public:
    CuteHttpResponse();
    CuteHttpResponse(const int statusCode, const QString &contentType, const QByteArray &content);

    const QByteArray &data() const {
        return m_data;
    }

private:
    void setData(const int statusCode, const QString &contentType, const QByteArray &content);

private:
    QByteArray m_data;
};
