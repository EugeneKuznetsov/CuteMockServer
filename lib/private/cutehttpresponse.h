#pragma once

#include <QByteArray>
#include "cutemockdata.h"

class CuteHttpResponse
{
    CuteHttpResponse(const CuteHttpResponse &) = delete;

public:
    CuteHttpResponse();
    CuteHttpResponse(const CuteMockData &replyData);
    CuteHttpResponse &operator=(const CuteHttpResponse &rightValue);

    const QByteArray &data() const {
        return m_data;
    }

private:
    QByteArray  m_data;
};
