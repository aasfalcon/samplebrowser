TARGET = gtest_main
TEMPLATE = lib
CONFIG += staticlib
CONFIG -= qt

VPATH += ../../vendor/googletest/googletest

INCLUDEPATH += \
    ../../vendor/googletest/googletest/include \
    ../../vendor/googletest/googletest

HEADERS += \
    include/gtest/gtest.h

SOURCES += \
    src/gtest_main.cc

