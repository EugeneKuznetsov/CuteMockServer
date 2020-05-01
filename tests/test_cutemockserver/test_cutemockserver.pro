TEMPLATE = app

TARGET = test_cutemockserver

QT += testlib network

CONFIG += warn_on testcase

INCLUDEPATH = ../../

HEADERS += \
    ../../lib/cutemockserver.h \
    ../../lib/private/cutesslserver.h \
    test_cutemockserver.h

SOURCES += \
    ../../lib/cutemockserver.cpp \
    ../../lib/private/cutesslserver.cpp \
    test_cutemockserver.cpp

RESOURCES += test_resources.qrc
