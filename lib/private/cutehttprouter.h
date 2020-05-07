#pragma once

#include <QMap>
#include <QUrl>
#include "cutehttpresponse.h"

class CuteHttpRouter
{
public:
    CuteHttpRouter();

    void set(const QString &method, const QUrl &uri, const int statusCode, const QString &contentType,
             const QByteArray &content, const bool ignoreUriQuery);

    const CuteHttpResponse &find(const QString &method, const QUrl &uri) const;

private:
    QMap<QString, CuteHttpResponse> m_routes;
    QMap<QString, CuteHttpResponse> m_routesWithIgnoredQueries;
    CuteHttpResponse                m_defaultResponse;
};
