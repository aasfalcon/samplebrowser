TEMPLATE = app
CONFIG += console c++11 precompile_headers warn_off
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -Wall -Wno-sign-compare

INCLUDEPATH += ../../vendor/googletest/googletest/include
INCLUDEPATH += ../../src/shared ../../src

PRECOMPILED_HEADER = stable.h

SOURCES += \
    ring_test.cpp \
    value_test.cpp \
    bus_test.cpp

LIBS += -lpthread
LIBS += -L../gtest_main -lgtest_main
LIBS += -L../gtest -lgtest
LIBS += -llog4cplus
