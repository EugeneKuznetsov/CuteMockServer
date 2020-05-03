TEMPLATE = app

TARGET = test_cutesslserver

QT += testlib network

CONFIG += warn_on testcase

INCLUDEPATH = ../../../

HEADERS += \
    ../../../lib/private/cutesslserver.h \
    test_cutesslserver.h

SOURCES += \
    ../../../lib/private/cutesslserver.cpp \
    test_cutesslserver.cpp

RESOURCES += \
    ../../../lib/certificates.qrc
