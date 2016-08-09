# Sound engine

TARGET = engine
TEMPLATE = lib
CONFIG += staticlib c++11

SOURCES += \
    scanner.cpp \
    helper.cpp \

HEADERS += \
    scanner.h \
    config.h \
    helper.h \
    common.h

INCLUDEPATH += ../../bundle ../../vendor ../plugins
