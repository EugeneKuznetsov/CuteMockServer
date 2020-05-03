TEMPLATE = lib

VERSION = 0.2

TARGET = $$qtLibraryTarget(qmljiraplugin)

CONFIG += warn_on qt static

QT = network

HEADERS += \
    cutemockserver.h \
    private/cutehttprequest.h \
    private/cutehttpresponse.h \
    private/cutesslserver.h

SOURCES += \
    cutemockserver.cpp \
    private/cutehttprequest.cpp \
    private/cutehttpresponse.cpp \
    private/cutesslserver.cpp

RESOURCES += \
    certificates.qrc
