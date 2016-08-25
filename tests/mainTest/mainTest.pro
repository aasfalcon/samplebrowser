TEMPLATE = app
CONFIG += console c++11 precompile_headers
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += ../../vendor/googletest/googletest/include
INCLUDEPATH += ../../src/main

PRECOMPILED_HEADER = stable.h

SOURCES += mainTest.cpp

LIBS += -lpthread
LIBS += -L../gtest_main -lgtest_main
LIBS += -L../gtest -lgtest
