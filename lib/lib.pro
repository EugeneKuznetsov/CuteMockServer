TEMPLATE = lib

VERSION = 0.1

TARGET = $$qtLibraryTarget(qmljiraplugin)

CONFIG += warn_on qt static

QT = network

HEADERS += \
    cutemockserver.h \
    private/cutesslserver.h

SOURCES += \
    cutemockserver.cpp \
    private/cutesslserver.cpp

RESOURCES += \
    certificates.qrc
