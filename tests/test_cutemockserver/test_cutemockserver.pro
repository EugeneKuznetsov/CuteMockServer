TEMPLATE = app

TARGET = test_cutemockserver

QT += testlib

CONFIG += warn_on testcase

INCLUDEPATH = ../../

HEADERS += \
    ../../lib/cutemockserver.h \
    test_cutemockserver.h

SOURCES += \
    ../../lib/cutemockserver.cpp \
    test_cutemockserver.cpp
