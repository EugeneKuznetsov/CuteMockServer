#pragma once

#include <QByteArray>

class CuteHttpResponse
{
    CuteHttpResponse(const CuteHttpResponse &) = delete;
    CuteHttpResponse &operator =(const CuteHttpResponse &) = delete;

public:
    CuteHttpResponse();

    const QByteArray &data() const {
        return m_data;
    }

private:
    QByteArray  m_data;
};
