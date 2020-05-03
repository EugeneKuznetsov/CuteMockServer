TEMPLATE = app

TARGET = test_cutemockserver

QT += testlib network

CONFIG += warn_on testcase

INCLUDEPATH = ../../

HEADERS += \
    ../../lib/cutemockserver.h \
    ../../lib/private/cutehttprequest.h \
    ../../lib/private/cutehttpresponse.h \
    ../../lib/private/cutemockdata.h \
    ../../lib/private/cutesslserver.h \
    test_cutemockserver.h

SOURCES += \
    ../../lib/cutemockserver.cpp \
    ../../lib/private/cutehttprequest.cpp \
    ../../lib/private/cutehttpresponse.cpp \
    ../../lib/private/cutemockdata.cpp \
    ../../lib/private/cutesslserver.cpp \
    test_cutemockserver.cpp

RESOURCES += test_resources.qrc \
    ../../lib/certificates.qrc
