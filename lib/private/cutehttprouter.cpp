#include <QDebug>
#include "cutehttprouter.h"

CuteHttpRouter::CuteHttpRouter()
    : m_routes()
    , m_defaultResponse()
{
}

void CuteHttpRouter::set(const QString &method, const QUrl &uri, const int statusCode, const QString &contentType, const QByteArray &content)
{
    m_routes[method + uri.toString()] = CuteHttpResponse(statusCode, contentType, content);
}

void CuteHttpRouter::set(const QString &method, const QUrl &uri, const int statusCode, const QString &contentType, QFile &content)
{
    if (!content.isOpen())
        if (!content.open(QFile::ReadOnly))
            qCritical() << "Could not open" << content.fileName();

    m_routes[method + uri.toString()] = CuteHttpResponse(statusCode, contentType, content.readAll());
}

const CuteHttpResponse &CuteHttpRouter::find(const QString &method, const QUrl &uri) const
{
    auto response = m_routes.find(method + uri.toString());
    if (response != m_routes.cend())
        return response.value();

    return m_defaultResponse;
}
