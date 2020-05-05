#pragma once

#include <QObject>
#include <QFile>

class CuteFile : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QByteArray data READ data NOTIFY dataChanged)

public:
    explicit CuteFile(QObject *parent = nullptr);
    CuteFile(const QString &filepath, QObject *parent = nullptr);

    const QByteArray &data() const {
        return m_data;
    }

public slots:
    bool openFile(const QString &filepath);

signals:
    void dataChanged();

private:
    QFile      m_file;
    QByteArray m_data;
};
