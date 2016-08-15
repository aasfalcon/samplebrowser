# Audio file I/O with MP3 support

TARGET = audiofilempg123
VERSION = 1.0.0

include(../plugins.pri)

HEADERS += \
    audiofilempg123.h

SOURCES += \
    pluginaudiofilempg123.cpp \
    audiofilempg123.cpp

LIBS += \
    -L../../vendor/libsndfile -lsndfile

INCLUDEPATH += \
    ../../../vendor
