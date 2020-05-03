#include <QDebug>
#include "cutemockdata.h"

CuteMockData::CuteMockData(int statusCode, CuteMockData::ContentType type, const QByteArray &body)
    : m_statusCode(statusCode)
    , m_type(type)
    , m_data(body)
{
}

CuteMockData::CuteMockData(int statusCode, CuteMockData::ContentType type, QFile &body)
    : m_statusCode(statusCode)
    , m_type(type)
{
    if (!body.isOpen())
        if (!body.open(QFile::ReadOnly))
            qCritical() << "Could not open" << body.fileName();
    m_data = body.readAll();
}

CuteMockData::CuteMockData(const CuteMockData &rightValue)
    : m_statusCode(rightValue.getStatusCode())
    , m_type(rightValue.getContentType())
    , m_data(rightValue.getBody())
{
}

CuteMockData &CuteMockData::operator=(const CuteMockData &rightValue)
{
    m_statusCode = rightValue.getStatusCode();
    m_type = rightValue.getContentType();
    m_data = rightValue.getBody();
    return *this;
}
