#include <QDebug>
#include "cutefile.h"

CuteFile::CuteFile(QObject *parent)
    : QObject(parent)
    , m_file()
    , m_data()
{
}

CuteFile::CuteFile(const QString &filepath, QObject *parent /*= nullptr*/)
    : QObject(parent)
    , m_file()
    , m_data()
{
    openFile(filepath);
}

bool CuteFile::openFile(const QString &filepath)
{
    if (m_file.isOpen())
        m_file.close();

    m_file.setFileName(filepath);
    const bool success = m_file.open(QFile::ReadOnly);
    if (!success) {
        qWarning() << "Could not open" << filepath;
    } else {
        m_data = m_file.readAll();
        emit dataChanged();
    }
    return success;
}
