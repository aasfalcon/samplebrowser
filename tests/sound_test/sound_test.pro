TEMPLATE = app
CONFIG += console c++11 precompile_headers warn_off
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -Wall -Wno-sign-compare -O2

INCLUDEPATH += ../../vendor/googletest/googletest/include
INCLUDEPATH += ../../src/sound ../../src

PRECOMPILED_HEADER = stable.h

SOURCES += \
    processor/messagebus_test.cpp \
    buffer_test.cpp \
    sample_test.cpp \
    dither_test.cpp \
    frame_test.cpp

LIBS += -lpthread -lltdl
LIBS += -L../gtest_main -lgtest_main
LIBS += -L../gtest -lgtest
LIBS += -llog4cplus
LIBS += -L../../src/sound -lsound
LIBS += -L../../src/shared -lweplsh

LIBS += \
    -llog4cplus \
    -lpng12 \
    -lltdl

LIBS += \
    -L /home/android/Develop/SampleBrowser/vendor/libsndfile/build/src/.libs -lsndfile

HEADERS += \
    stable.h \
    common_test.h \
    generator_test.h
