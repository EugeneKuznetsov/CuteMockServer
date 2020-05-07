#include <QDebug>
#include "cutehttprouter.h"

CuteHttpRouter::CuteHttpRouter()
    : m_routes()
    , m_routesWithIgnoredQueries()
    , m_defaultResponse()
{
}

void CuteHttpRouter::set(const QString &method, const QUrl &uri, const int statusCode, const QString &contentType,
                         const QByteArray &content, const bool ignoreUriQuery)
{
    if (ignoreUriQuery)
        m_routesWithIgnoredQueries[method + uri.adjusted(QUrl::RemoveQuery).toString()]
                = CuteHttpResponse(statusCode, contentType, content);
    else
        m_routes[method + uri.toString()] = CuteHttpResponse(statusCode, contentType, content);
}

const CuteHttpResponse &CuteHttpRouter::find(const QString &method, const QUrl &uri) const
{
    auto response = m_routes.find(method + uri.toString());
    if (response != m_routes.cend())
        return response.value();

    const QString uriWithIgnoredQuery = method + uri.adjusted(QUrl::RemoveQuery).toString();
    auto ignoredQueryResponse = m_routesWithIgnoredQueries.find(uriWithIgnoredQuery);
    if (ignoredQueryResponse != m_routesWithIgnoredQueries.cend())
        return ignoredQueryResponse.value();

    return m_defaultResponse;
}
