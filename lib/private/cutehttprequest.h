#pragma once

#include <QObject>
#include <QUrl>
#include "cutemockdata.h"

class QTcpSocket;

class CuteHttpRequest : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(CuteHttpRequest)

public:
    explicit CuteHttpRequest(QTcpSocket *parent);

    const CuteMockData::Method &getMethod() const {
        return m_method;
    }
    const QUrl &getUri() const {
        return m_uri;
    }
    const QByteArray &getBody() const {
        return m_body;
    }

signals:
    void parsed(QTcpSocket *client, const CuteHttpRequest &request);

private slots:
    void parse();

private:
    CuteMockData::Method    m_method;
    QUrl                    m_uri;
    QStringList             m_headers;
    QByteArray              m_body;
    int                     m_contentLength;
};
