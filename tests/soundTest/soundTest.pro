TEMPLATE = app
CONFIG += console c++11 precompile_headers warn_off
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -Wall -Wno-sign-compare

INCLUDEPATH += ../../vendor/googletest/googletest/include
INCLUDEPATH += ../../src/sound ../../src

PRECOMPILED_HEADER = stable.h

SOURCES += \
    processor/messagebus_test.cpp

LIBS += -lpthread
LIBS += -L../gtest_main -lgtest_main
LIBS += -L../gtest -lgtest
LIBS += -llog4cplus
LIBS += -L../../src/sound -lsound

HEADERS += \
    stable.h
