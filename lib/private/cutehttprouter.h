#pragma once

#include <QMap>
#include <QUrl>
#include <QFile>
#include "cutehttpresponse.h"

class CuteHttpRouter
{
public:
    CuteHttpRouter();

    void set(const QString &method, const QUrl &uri, const int statusCode, const QString &contentType, const QByteArray &content);
    void set(const QString &method, const QUrl &uri, const int statusCode, const QString &contentType, QFile &content);

    const CuteHttpResponse &find(const QString &method, const QUrl &uri) const;

private:
    QMap<QString, CuteHttpResponse> m_routes;
    CuteHttpResponse                m_defaultResponse;
};
